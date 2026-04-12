#include "VideoDecoder.hpp"
#include <iostream>
#include <stdexcept>

VideoDecoder::VideoDecoder(const char* url, int targetWidth, int height) : targetWidth(targetWidth), targetHeight(height), videoStreamIndex(-1) {
    // Initialize FFmpeg
    avformat_open_input(&formatContext, url, NULL, NULL);
    if (!formatContext) {
        throw std::runtime_error("Could not open input file");
    }

    avformat_find_stream_info(formatContext, NULL);

    // Find the video stream
    for (int i = 0; i < formatContext->nb_streams; i++) {
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStreamIndex = i;
            break;
        }
    }

    if (videoStreamIndex == -1) {
        throw std::runtime_error("Could not find video stream");
    }

    // Get the codec context
    AVCodecParameters* codecpar = formatContext->streams[videoStreamIndex]->codecpar;
    AVCodec* decoder = avcodec_find_decoder(codecpar->codec_id);
    codecContext = avcodec_alloc_context3(decoder, NULL);
    avcodec_parameters_to_context(codecContext, codecpar);

    // Open the codec
    avcodec_open2(codecContext, decoder, NULL);

    // Allocate frames
    rawFrame = av_frame_alloc();
    rgbFrame = av_frame_alloc();
    packet = av_packet_alloc();

    // Allocate frame buffer for RGB data
    int numBytes = av_image_get_buffer_size(AVPixelFormat::AV_PIX_FMT_RGB24, targetWidth, targetHeight, 1);
    frameBuffer = (uint8_t*)av_malloc(numBytes * sizeof(uint8_t));
    av_image_fill_arrays(rgbFrame->data, rgbFrame->linesize, frameBuffer, AVPixelFormat::AV_PIX_FMT_RGB24, targetWidth, targetHeight, 1);

    // Create scaler context
    scalerContext = sws_getContext(codecContext->width, codecContext->height, codecContext->pix_fmt, targetWidth, targetHeight, AVPixelFormat::AV_PIX_FMT_RGB24, SWS_BILINEAR, NULL, NULL, NULL);
}

bool VideoDecoder::nextFrame() {
    while (av_read_frame(formatContext, packet) >= 0) {
        if (packet->stream_index == videoStreamIndex) {
            avcodec_send_packet(codecContext, packet);
            if (avcodec_receive_frame(codecContext, rawFrame) == 0) {
                sws_scale(scalerContext, (const uint8_t* const*)rawFrame->data, rawFrame->linesize, 0, codecContext->height, rgbFrame->data, rgbFrame->linesize);
                av_packet_unref(packet);
                return true;
            }
        }
        av_packet_unref(packet);
    }
    return false;
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
