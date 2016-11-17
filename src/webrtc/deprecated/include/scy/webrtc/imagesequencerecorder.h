///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup webrtc
/// @{

#ifndef SCY_WebRTC_ImageSequenceRecorder_H
#define SCY_WebRTC_ImageSequenceRecorder_H

#include "scy/av/videoencoder.h"

#include "webrtc/api/peerconnectioninterface.h"

namespace scy {

class ImageSequenceRecorder
    : public rtc::VideoSinkInterface<cricket::VideoFrame>
{
public:
    ImageSequenceRecorder(webrtc::VideoTrackInterface* track_to_render,
                          const std::string& basename);
    virtual ~ImageSequenceRecorder();

    std::string getNextFilename();

    /// VideoSinkInterface implementation
    void OnFrame(const cricket::VideoFrame& frame) override;

protected:
    rtc::scoped_refptr<webrtc::VideoTrackInterface> _renderedTrack;
    const std::string _basename;
    size_t _count;
    int _width;
    int _height;
    av::VideoEncoder _encoder;
    AVFrame* _avframe;
};

} // namespace scy

#endif

/// @\}
