///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup webrtc
/// @{


#ifndef SCY_WebRTC_VideoPacketSource_H
#define SCY_WebRTC_VideoPacketSource_H


#include "scy/av/types.h"

#include "webrtc/media/base/videocapturer.h"


namespace scy {


/// VideoPacketSource implements a simple `cricket::VideoCapturer` that
/// gets decoded remote video frames from a local media channel.
/// It's used as the remote video source's `VideoCapturer` so that the remote
/// video can be used as a `cricket::VideoCapturer` and in that way a remote
/// video stream can implement the `MediaStreamSourceInterface`.
class VideoPacketSource : public cricket::VideoCapturer
{
public:
    VideoPacketSource();
    virtual ~VideoPacketSource();

    /// cricket::VideoCapturer implementation.
    virtual cricket::CaptureState
    Start(const cricket::VideoFormat& capture_format);
    virtual void Stop();
    virtual bool IsRunning();
    virtual bool GetPreferredFourccs(std::vector<uint32_t>* fourccs);
    virtual bool GetBestCaptureFormat(const cricket::VideoFormat& desired,
                                      cricket::VideoFormat* best_format);
    virtual bool IsScreencast() const;

    void onVideoCaptured(av::VideoPacket& packet);

protected:
    bool running_;
    int64_t initial_timestamp_;
    int64_t next_timestamp_;
    // const bool is_screencast_;
    webrtc::VideoRotation rotation_;
};


// class VideoPacketSourceFactory : public cricket::VideoDeviceCapturerFactory {
// public:
//     VideoPacketSourceFactory() {}
//     virtual ~VideoPacketSourceFactory() {}
//
//     virtual cricket::VideoCapturer* Create(const cricket::Device& device) {
//         return new VideoPacketSource(device.name);
//     }
// };


} // namespace scy


#endif


/// @\}
