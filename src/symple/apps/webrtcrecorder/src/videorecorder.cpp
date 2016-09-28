#include "videorecorder.h"

#include "scy/logger.h"
#include "scy/filesystem.h"
#include "scy/media/ffmpeg.h"

#include "webrtc/modules/video_capture/video_capture_factory.h"
#include "webrtc/media/engine/webrtcvideocapturerfactory.h"
// #include "webrtc/media/engine/webrtcvideoframe.h"
#include "webrtc/common_video/libyuv/include/webrtc_libyuv.h"


namespace scy {


VideoRecorder::VideoRecorder(webrtc::VideoTrackInterface* track_to_render, const std::string& basename)
    : rendered_track_(track_to_render),
      _basename(basename),
      _count(0),
      _width(0),
      _height(0)
{
    rendered_track_->AddOrUpdateSink(this, rtc::VideoSinkWants());

    av::initializeFFmpeg();
}


VideoRecorder::~VideoRecorder()
{
    rendered_track_->RemoveSink(this);

    av::uninitializeFFmpeg();
}


void VideoRecorder::OnFrame(const cricket::VideoFrame& yuvframe)
{
    DebugL << "On video frame: " << yuvframe.width() << 'x' << yuvframe.height() << std::endl;

    if (_width != yuvframe.width() ||
        _height != yuvframe.height()) {
        _encoder.iparams.width = yuvframe.width();
        _encoder.iparams.height = yuvframe.height();
        _encoder.iparams.pixelFmt = "yuv420p";

        _encoder.oparams.encoder = "mjpeg"; // "png"
        _encoder.oparams.pixelFmt = "yuvj420p";
        _encoder.oparams.width = yuvframe.width();
        _encoder.oparams.height = yuvframe.height();
        _encoder.oparams.fps = 25; // avoid FFmpeg 0 fps bitrate tolerance error
        _encoder.oparams.enabled = true;

        _encoder.create();
        _encoder.open();
    }

    // Initialize the AVFrame
    AVFrame* avframe = av_frame_alloc();
    avframe->width = yuvframe.width();
    avframe->height = yuvframe.height();
    avframe->format = AV_PIX_FMT_YUV420P;

    // Initialize avframe->linesize
    avpicture_fill((AVPicture*)avframe, NULL, /*avframe->format*/AV_PIX_FMT_YUV420P, avframe->width, avframe->height);

    // Set avframe->data pointers manually
    avframe->data[0] = (uint8_t*)yuvframe.video_frame_buffer()->DataY();
    avframe->data[1] = (uint8_t*)yuvframe.video_frame_buffer()->DataU();
    avframe->data[2] = (uint8_t*)yuvframe.video_frame_buffer()->DataV();

    // Feed the decoded frame into the encoder
    AVPacket opacket;
    if (_encoder.encode(avframe, opacket)) {

        // Write the output file
        std::stringstream filename;
        filename << _basename;
        if (++_count > 1)
            filename << '-' << _count;
        filename << '_' << yuvframe.width() << 'x' << yuvframe.height()
                 << ".jpg";

        DebugL << "Write video frame: " << filename.str() << std::endl;
        fs::savefile(filename.str(), (const char*)opacket.data, (std::streamsize)opacket.size);
    }

    // Cleanup the frame
    av_free(avframe);
}


} // namespace scy
