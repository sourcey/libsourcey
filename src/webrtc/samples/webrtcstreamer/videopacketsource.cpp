//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
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

#include "videopacketsource.h" //scy/webrtc/


using std::endl;


namespace scy {


VideoPacketSource::VideoPacketSource()
{
}


VideoPacketSource::~VideoPacketSource()
{
}


cricket::CaptureState VideoPacketSource::Start(const cricket::VideoFormat& capture_format)
{
    try {
        if (capture_state() == cricket::CS_RUNNING) {
            WarnL << "Start called when it's already started." << endl;
            return capture_state();
        }
        InfoL << "Start" << endl;

        // // Convert to compatible format on the fly
        // assert(_capture->video());
        // _capture->video()->oparams.pixelFmt = "nv12"; // yuv420p, yuyv422
        // _capture->video()->oparams.width = capture_format.width;
        // _capture->video()->oparams.height = capture_format.height;
        //
        // // Connect and start the packet stream.
        // _capture->start();
        // _capture->emitter += packetDelegate(this, &VideoPacketSource::onVideoCaptured);

        SetCaptureFormat(&capture_format);
        return cricket::CS_RUNNING;
    } catch (...) {}
    return cricket::CS_FAILED;
}


void VideoPacketSource::Stop()
{
    try {
        if (capture_state() == cricket::CS_STOPPED) {
            WarnL << "Stop called when it's already stopped." << endl;
            return;
        }
        InfoL << "Stop" << endl;

        // _capture->emitter.detach(this); // for cleanup()

        SetCaptureFormat(nullptr);
        SetCaptureState(cricket::CS_STOPPED);
        return;
    } catch (...) {}
    return;
}


void VideoPacketSource::onVideoCaptured(void* sender, av::VideoPacket& packet)
{
    TraceL << "On video frame: " << packet.width << 'x' << packet.height << std::endl;

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
