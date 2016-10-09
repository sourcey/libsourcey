#ifndef SCY_ImageSequenceRecorder_H
#define SCY_ImageSequenceRecorder_H


#include "scy/av/videocontext.h"

#include "webrtc/api/peerconnectioninterface.h"
#include "webrtc/api/jsep.h"
#include "webrtc/api/datachannelinterface.h"
#include "webrtc/api/test/fakeconstraints.h"


namespace scy {


class ImageSequenceRecorder: public rtc::VideoSinkInterface<cricket::VideoFrame>
{
public:
    ImageSequenceRecorder(webrtc::VideoTrackInterface* track_to_render, const std::string& basename);
    virtual ~ImageSequenceRecorder();

    std::string getNextFilename();

    // VideoSinkInterface implementation
    void OnFrame(const cricket::VideoFrame& frame) override;

protected:
    rtc::scoped_refptr<webrtc::VideoTrackInterface> _renderedTrack;
    const std::string _basename;
    size_t _count;
    int _width;
    int _height;
    av::VideoEncoderContext _encoder;
    AVFrame* _avframe;
};


} // namespace scy


#endif
