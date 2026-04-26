#ifndef VIDEO_DECODER_HPP
#define VIDEO_DECODER_HPP

#include <cstdint>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>

class VideoDecoder {
public:
    VideoDecoder(const char* url, int targetWidth, int targetHeight);
    ~VideoDecoder();

    VideoDecoder(const VideoDecoder&)            = delete;
    VideoDecoder& operator=(const VideoDecoder&) = delete;

    VideoDecoder(VideoDecoder&&)                 = delete;
    VideoDecoder& operator=(VideoDecoder&&)      = delete;

    bool nextFrame();
    uint8_t* getRGB();

    int getWidth()  const;
    int getHeight() const;

private:
    int targetWidth;
    int targetHeight;
    int videoStreamIndex;

    AVFormatContext* formatContext  = nullptr;
    AVCodecContext*  codecContext   = nullptr;
    SwsContext*      scalerContext  = nullptr;
    AVFrame*         rawFrame       = nullptr;
    AVFrame*         rgbFrame       = nullptr;
    AVPacket*        packet         = nullptr;
    uint8_t*         frameBuffer    = nullptr;
};

#endif  // VIDEO_DECODER_HPP
