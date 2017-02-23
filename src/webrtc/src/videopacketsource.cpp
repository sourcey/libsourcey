///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup webrtc
/// @{


#include "scy/webrtc/videopacketsource.h"


using std::endl;


namespace scy {


VideoPacketSource::VideoPacketSource()
    : running_(false)
    , initial_timestamp_(rtc::TimeNanos())
    , next_timestamp_(rtc::kNumNanosecsPerMillisec)
    //, is_screencast_(is_screencast)
    , rotation_(webrtc::kVideoRotation_0)
{
    // Default supported formats. Use SetSupportedFormats to over write.
    std::vector<cricket::VideoFormat> formats;
    formats.push_back(cricket::VideoFormat(1280, 720,
        cricket::VideoFormat::FpsToInterval(30), cricket::FOURCC_NV12)); // FOURCC_I420
    formats.push_back(cricket::VideoFormat(640, 480,
        cricket::VideoFormat::FpsToInterval(30), cricket::FOURCC_NV12)); // FOURCC_I420
    formats.push_back(cricket::VideoFormat(320, 240,
        cricket::VideoFormat::FpsToInterval(30), cricket::FOURCC_NV12)); // FOURCC_I420
    formats.push_back(cricket::VideoFormat(160, 120,
        cricket::VideoFormat::FpsToInterval(30), cricket::FOURCC_NV12)); // FOURCC_I420
    SetSupportedFormats(formats);
}


VideoPacketSource::~VideoPacketSource()
{
}


cricket::CaptureState VideoPacketSource::Start(const cricket::VideoFormat& format)
{
    // try {
        RTC_CHECK(format.fourcc == cricket::FOURCC_NV12);
        if (capture_state() == cricket::CS_RUNNING) {
            WarnL << "Start called when it's already started." << endl;
            return capture_state();
        }
        DebugL << "Start" << endl;

        // // Convert to compatible format on the fly
        // assert(_capture->video());
        // _capture->video()->oparams.pixelFmt = "nv12"; // yuv420p, yuyv422
        // _capture->video()->oparams.width = capture_format.width;
        // _capture->video()->oparams.height = capture_format.height;
        //
        // // Connect and start the packet stream.
        // _capture->start();
        // _capture->emitter += packetSlot(this, &VideoPacketSource::onVideoCaptured);

        SetCaptureFormat(&format);
        return cricket::CS_RUNNING;
    // } catch (...) {
    // }
    // return cricket::CS_FAILED;
}


void VideoPacketSource::Stop()
{
    // try {
        if (capture_state() == cricket::CS_STOPPED) {
            WarnL << "Stop called when it's already stopped." << endl;
            return;
        }
        DebugL << "Stop" << endl;

        // _capture->emitter.detach(this); // for cleanup()

        SetCaptureFormat(nullptr);
        SetCaptureState(cricket::CS_STOPPED);
    // } catch (...) {
    // }
}


void VideoPacketSource::onVideoCaptured(av::VideoPacket& packet)
{
    TraceL << "On video frame: " << packet.width << 'x' << packet.height << endl;
    //RTC_CHECK(fourcc == cricket::FOURCC_I420);
    RTC_CHECK(packet.width > 0);
    RTC_CHECK(packet.height > 0);
    
    int adapted_width;
    int adapted_height;
    int crop_width;
    int crop_height;
    int crop_x;
    int crop_y;

    // TODO(nisse): It's a bit silly to have this logic in a fake
    // class. Child classes of VideoCapturer are expected to call
    // AdaptFrame, and the test case
    // VideoCapturerTest.SinkWantsMaxPixelAndMaxPixelCountStepUp
    // depends on this.
    if (AdaptFrame(packet.width, packet.height, 0, 0, &adapted_width, &adapted_height,
        &crop_width, &crop_height, &crop_x, &crop_y, nullptr)) {
        rtc::scoped_refptr<webrtc::I420Buffer> buffer(
            webrtc::I420Buffer::Create(adapted_width, adapted_height));
        buffer->InitializeData();

        OnFrame(webrtc::VideoFrame(
            buffer, rotation_,
            packet.time), // next_timestamp_ / rtc::kNumNanosecsPerMicrosec),
            packet.width, packet.height);
    }
    // next_timestamp_ += timestamp_interval;

    /* Old code pre f5297a0
    cricket::CapturedFrame frame;
    frame.width = packet.width;
    frame.height = packet.height;
    frame.pixel_width = 1;
    frame.pixel_height = 1;
    frame.fourcc = cricket::FOURCC_NV12; // FOURCC_I420
    frame.data = packet.data();
    frame.data_size = packet.size();
    // frame.time_stamp = packet.time; // time in microseconds is ignored

    SignalFrameCaptured(this, &frame);
    */
}


bool VideoPacketSource::IsRunning()
{
    return capture_state() == cricket::CS_RUNNING;
}


bool VideoPacketSource::GetPreferredFourccs(std::vector<uint32_t>* fourccs)
{
    if (!fourccs)
        return false;

    // This class does not yet support multiple pixel formats.
    fourccs->push_back(cricket::FOURCC_NV12); // FOURCC_I420
    return true;
}


bool VideoPacketSource::GetBestCaptureFormat(const cricket::VideoFormat& desired, cricket::VideoFormat* best_format)
{
    if (!best_format)
        return false;

    // Use the supported format as the best format.
    best_format->width = desired.width;
    best_format->height = desired.height;
    best_format->fourcc = cricket::FOURCC_NV12; // FOURCC_I420
    best_format->interval = desired.interval;

    return true;
}


bool VideoPacketSource::IsScreencast() const
{
    return false;
}


} // namespace scy


/// @\}
