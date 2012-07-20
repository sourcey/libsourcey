#ifndef VIDEO_ENCODER_H_
#define VIDEO_ENCODER_H_


#include "Sourcey/Media/Common.h"

// Forward declaring ffmpeg things
//typedef struct AVCodecContext AVCodecContext;
//typedef struct SwsContext SwsContext;
//typedef struct AVFrame AVFrame;
//typedef struct AVCodec AVCodec;


namespace Sourcey {
namespace Media {


class VideoTranscoder
{
private:
    static bool init_done_;
    int root;

    AVCodecContext *decode_context_;
    AVFrame* _iFrame;

    AVCodecContext *_encoderContext;
    AVFrame* _oFrame;
    SwsContext *_resizeContext;
    double _now;

protected:
    static int initialize();

public:
    VideoTranscoder();
    ~VideoTranscoder();

    inline double getTimeBase() { return 1.0/25; }
    inline double getNow() { return _now; }

    int initEncodeContext(VideoCodecID codec_id, unsigned int ow, unsigned int oh, unsigned long bit_rate);
    unsigned long encode(AVFrame* input, unsigned char *output, unsigned int outputSize);
    void resetEncodeContext();

    int initDecodeContext(VideoCodecID codec_id, unsigned char *extradata=0, unsigned long extradata_len=0);
    AVFrame* decode(unsigned char *input, unsigned int input_size);

    int initScaleContext(int width, int height);
    AVFrame* scaleFrame(AVFrame* orig_frame);
    void resetScaleContext();

    static AVFrame* MakeBlackFrame(int width, int height);
    static void FreeFrame(AVFrame* f);
};


} } // namespace Sourcey::Media


#endif // VIDEO_ENCODER_H_

