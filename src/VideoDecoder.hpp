class VideoDecoder {
public:
    VideoDecoder(const char* url, int targetWidth, int targetHeight);

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
