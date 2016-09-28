#ifndef SCY_VideoRecorder_H
#define SCY_VideoRecorder_H


#include "scy/media/videocontext.h"

#include "webrtc/api/peerconnectioninterface.h"
#include "webrtc/api/jsep.h"
#include "webrtc/api/datachannelinterface.h"
#include "webrtc/api/test/fakeconstraints.h"


namespace scy {


class VideoRecorder: public rtc::VideoSinkInterface<cricket::VideoFrame>
{
public:
    VideoRecorder(webrtc::VideoTrackInterface* track_to_render, const std::string& basename);
    virtual ~VideoRecorder();

    // VideoSinkInterface implementation
    void OnFrame(const cricket::VideoFrame& frame) override;

protected:
    rtc::scoped_refptr<webrtc::VideoTrackInterface> rendered_track_;
    const std::string _basename;
    size_t _count;
    int _width;
    int _height;
    av::VideoCodecEncoderContext _encoder;
};


} // namespace scy


#endif
