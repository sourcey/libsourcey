///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup webrtc
/// @{


#include "scy/webrtc/util.h"

#include "scy/webrtc/webrtc.h"
#include "webrtc/media/engine/webrtcvideocapturerfactory.h"
#include "webrtc/modules/video_capture/video_capture_factory.h"


namespace scy {


std::vector<std::string> getVideoCaptureDevices()
{
    std::vector<std::string> deviceNames;
    std::unique_ptr<webrtc::VideoCaptureModule::DeviceInfo> info(
        webrtc::VideoCaptureFactory::CreateDeviceInfo());
    if (!info) {
        return deviceNames;
    }
    int numDevicess = info->NumberOfDevices();
    assert(numDevicess > 0);
    for (int i = 0; i < numDevicess; ++i) {
        const uint32_t kSize = 256;
        char name[kSize] = { 0 };
        char id[kSize] = { 0 };
        if (info->GetDeviceName(i, name, kSize, id, kSize) != -1) {
            deviceNames.push_back(name);
        }
    }
}


// std::unique_ptr<cricket::VideoCapturer> openWebRtcVideoCaptureDevice(const std::string& deviceName)
cricket::VideoCapturer* openWebRtcVideoCaptureDevice(const std::string& deviceName)
{
    cricket::WebRtcVideoDeviceCapturerFactory factory;
    // std::unique_ptr<cricket::VideoCapturer> capturer;
    cricket::VideoCapturer* capturer = nullptr;
    auto deviceNames = getVideoCaptureDevices();
    for (const auto& name : deviceNames) {
        if (name == deviceName) {
            capturer = factory.Create(cricket::Device(name, 0));
            break;
        }
    }
    if (!capturer && !deviceNames.empty()) {
        capturer = factory.Create(cricket::Device(deviceNames[0], 0));

    }

    assert(capturer);
    return capturer;
}



} // namespace scy


/// @\}