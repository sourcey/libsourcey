///
//
// LibSourcey
// Copyright (C) 2005, Sourcey <https://sourcey.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include "scy/webrtc/opencvvideocapturer.h"


using std::endl;


namespace scy {
namespace wrtc {


OpenCVVideoCapturer::OpenCVVideoCapturer(int deviceId)
    : capture(av::MediaFactory::instance().createVideoCapture(deviceId))
{
    // Default supported formats. Use ResetSupportedFormats to over write.
    std::vector<cricket::VideoFormat> formats;
    formats.push_back(
        cricket::VideoFormat(1280, 720, cricket::VideoFormat::FpsToInterval(30),
                             cricket::FOURCC_I420));
    formats.push_back(
        cricket::VideoFormat(640, 480, cricket::VideoFormat::FpsToInterval(30),
                             cricket::FOURCC_I420));
    formats.push_back(
        cricket::VideoFormat(320, 240, cricket::VideoFormat::FpsToInterval(30),
                             cricket::FOURCC_I420));
    formats.push_back(
        cricket::VideoFormat(160, 120, cricket::VideoFormat::FpsToInterval(30),
                             cricket::FOURCC_I420));
}


OpenCVVideoCapturer::~OpenCVVideoCapturer()
{
}


cricket::CaptureState
OpenCVVideoCapturer::Start(const cricket::VideoFormat& capture_format)
{
    try {
        if (capture_state() == cricket::CS_RUNNING) {
            LWarn("Start called when it's already started.")
            return capture_state();
        }
        LInfo("Start")

        // TODO: Check and verify cricket::VideoFormat

        // Connect and start the packet stream.
        // Output packets must be av::MatrixPacket types so we can access
        // the underlying cv::Mat.
        capture->start();
        capture->emitter +=
            packetSlot(this, &OpenCVVideoCapturer::onFrameCaptured);

        SetCaptureFormat(&capture_format);
        return cricket::CS_RUNNING;
    } catch (...) {
    }
    return cricket::CS_FAILED;
}


void OpenCVVideoCapturer::Stop()
{
    try {
        if (capture_state() == cricket::CS_STOPPED) {
            LWarn("Stop called when it's already stopped.")
            return;
        }
        LInfo("Stop")

        capture->emitter.detach(this); // for cleanup()

        SetCaptureFormat(NULL);
        SetCaptureState(cricket::CS_STOPPED);
        return;
    } catch (...) {
    }
    return;
}


void OpenCVVideoCapturer::onFrameCaptured(void* sender,
                                          av::MatrixPacket& packet)
{
    // LTrace("On frame")

    // Convert the packet from BGR to I420 for WebRTC
    cv::Mat yuv(packet.width, packet.height, CV_8UC4);
    cv::cvtColor(*packet.mat, yuv, CV_BGR2YUV_I420);

    cricket::CapturedFrame frame;
    frame.width = packet.width;
    frame.height = packet.height;
    frame.fourcc = cricket::FOURCC_I420;
    frame.data_size = yuv.rows * yuv.step;
    frame.data = yuv.data;

    SignalFrameCaptured(this, &frame);
}


bool OpenCVVideoCapturer::IsRunning()
{
    return capture_state() == cricket::CS_RUNNING;
}


bool OpenCVVideoCapturer::GetPreferredFourccs(std::vector<uint32_t>* fourccs)
{
    if (!fourccs)
        return false;

    // This class does not yet support multiple pixel formats.
    fourccs->push_back(cricket::FOURCC_I420);
    return true;
}


bool OpenCVVideoCapturer::GetBestCaptureFormat(
    const cricket::VideoFormat& desired, cricket::VideoFormat* best_format)
{
    if (!best_format)
        return false;

    // Use the supported format as the best format.
    best_format->width = desired.width;
    best_format->height = desired.height;
    best_format->fourcc = cricket::FOURCC_I420;
    best_format->interval = desired.interval;

    return true;
}


bool OpenCVVideoCapturer::IsScreencast() const
{
    return false;
}


} } // namespace scy::wrtc
