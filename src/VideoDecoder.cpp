#include "VideoDecoder.hpp"
#include <iostream>
#include <stdexcept>

VideoDecoder::VideoDecoder(const char* url, int targetWidth, int targetHeight)
    : targetWidth(targetWidth), targetHeight(targetHeight), videoStreamIndex(-1)
{
    // Initialize FFmpeg
    avformat_open_input(&formatContext, url, NULL, NULL);
    if (!formatContext) {
        throw std::runtime_error("Could not open video file.");
    }

    avformat_find_stream_info(formatContext, NULL);
    if (!formatContext->streams)
    {
        throw std::runtime_error("Could not find stream info.");
    }

    // Find the video stream
    for (int i = 0; i < formatContext->nb_streams; i++) {
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStreamIndex = i;
            break;
        }
    }

    if (videoStreamIndex == -1) {
        throw std::runtime_error("Could not find video stream.");
    }

    // Get the codec
    AVCodec* decoder = avcodec_find_decoder(formatContext->streams[videoStreamIndex]->codecpar->codec_id);
    if (!decoder) {
        throw std::runtime_error("Could not find video decoder.");
    }

    // Open the codec
    codecContext = avcodec_alloc_context3(decoder);
    if (!codecContext) {
        throw std::runtime_error("Could not allocate video codec context.");
    }

    if (avcodec_parameters_to_context(codecContext, formatContext->streams[videoStreamIndex]->codecpar) < 0) {
        throw std::runtime_error("Could not copy codec parameters to context.");
    }

    if (avcodec_open2(codecContext, decoder, NULL) < 0) {
        throw std::runtime_error("Could not open video codec.");
    }

    // Allocate frames
    rawFrame = av_frame_alloc();
    rgbFrame = av_frame_alloc();
    packet = av_packet_alloc();

    if (!rawFrame || !rgbFrame || !packet) {
        throw std::runtime_error("Could not allocate frames or packet.");
    }

    // Allocate frame buffer for RGB data
    int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24, targetWidth, targetHeight, 1);
    frameBuffer = (uint8_t*)av_malloc(numBytes * sizeof(uint8_t));
    av_image_fill_arrays(rgbFrame->data, rgbFrame->linesize, frameBuffer, AV_PIX_FMT_RGB24, targetWidth, targetHeight, 1);

    // Setup scaler
    scalerContext = sws_getContext(codecContext->width, codecContext->height, codecContext->pix_fmt, targetWidth, targetHeight, AV_PIX_FMT_RGB24, SWS_BILINEAR, NULL, NULL, NULL);
    if (!scalerContext) {
        throw std::runtime_error("Could not initialize image scaler.");
    }
}

bool VideoDecoder::nextFrame() {
    while (av_read_frame(formatContext, packet) >= 0) {
        if (packet->stream_index == videoStreamIndex) {
            if (avcodec_send_packet(codecContext, packet) < 0) {
                // Handle error: Log or throw exception
                return false;
            }

            while (avcodec_receive_frame(codecContext, rawFrame) == 0) {
                sws_scale(scalerContext, (const uint8_t* const*)rawFrame->data, rawFrame->linesize, 0, codecContext->height, rgbFrame->data, rgbFrame->linesize);
                av_packet_unref(packet);
                return true;
            }
        }
        av_packet_unref(packet);
    }
    return false; // End of stream
}

uint8_t* VideoDecoder::getRGB() {
    return frameBuffer;
}

int VideoDecoder::getWidth() const {
    return targetWidth;
}

int VideoDecoder::getHeight() const {
    return targetHeight;
}
