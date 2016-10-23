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


VideoPacketSource::VideoPacketSource(av::MediaCapture::Ptr capture) :
    _capture(capture)
{
    // Default supported formats. Use ResetSupportedFormats to over write.
    std::vector<cricket::VideoFormat> formats;
    formats.push_back(cricket::VideoFormat(1280, 720,
        cricket::VideoFormat::FpsToInterval(30), cricket::FOURCC_I420)); //FOURCC_I420));
    formats.push_back(cricket::VideoFormat(640, 480,
        cricket::VideoFormat::FpsToInterval(30), cricket::FOURCC_I420)); //FOURCC_I420));
    formats.push_back(cricket::VideoFormat(320, 240,
        cricket::VideoFormat::FpsToInterval(30), cricket::FOURCC_I420)); //FOURCC_I420));
    formats.push_back(cricket::VideoFormat(160, 120,
        cricket::VideoFormat::FpsToInterval(30), cricket::FOURCC_I420)); //FOURCC_I420));
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

        // TODO: Check and verify cricket::VideoFormat

        // Convert to compatible format on the fly
        assert(_capture->video());
        _capture->video()->oparams.pixelFmt = "yuv420p";
        _capture->video()->oparams.width = capture_format.width;
        _capture->video()->oparams.height = capture_format.height;

        TraceL << "^^^^^^^^^^^^^^^^^ Starting video capture: " << capture_format.width << 'x' << capture_format.height << std::endl;

        // Connect and start the packet stream.
        _capture->start();
        _capture->emitter += packetDelegate(this, &VideoPacketSource::onFrameCaptured);
        // _emitter += packetDelegate(this, &VideoPacketSource::onFrameCaptured);

        // v4l2 default input format is yuyv422 or FOURCC_I420

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

        _capture->emitter.detach(this); // for cleanup()

        SetCaptureFormat(NULL);
        SetCaptureState(cricket::CS_STOPPED);
        return;
    } catch (...) {}
    return;
}


void VideoPacketSource::onFrameCaptured(void* sender, av::VideoPacket& packet)
{
    // TraceS(this) << "On frame" << std::endl;
    TraceL << "^^^^^^^^^^^^^^^^^ On video frame captured: " << packet.width << 'x' << packet.height << std::endl;
    TraceL << "^^^^^^^^^^^^^^^^^ On video frame captured data_size: " << packet.size() << std::endl;

    // Convert the packet from BGR to I420 for WebRTC
    // cv::Mat yuv(packet.width, packet.height, CV_8UC4);
    // cv::cvtColor(*packet.mat, yuv, CV_BGR2YUV_I420);

    // assert(0);

    cricket::CapturedFrame frame;
    frame.width = packet.width;
    frame.height = packet.height;
    frame.fourcc = cricket::FOURCC_I420; //FOURCC_I420;
    frame.data_size = packet.size(); //yuv.rows * yuv.step;
    frame.data = packet.data(); //yuv.data;

        // scy::sleep(100); // testing


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
    fourccs->push_back(cricket::FOURCC_I420); //FOURCC_I420);
    return true;
}


bool VideoPacketSource::GetBestCaptureFormat(const cricket::VideoFormat& desired, cricket::VideoFormat* best_format)
{
    if (!best_format)
        return false;

    // Use the supported format as the best format.
    best_format->width = desired.width;
    best_format->height = desired.height;
    best_format->fourcc = cricket::FOURCC_I420; //FOURCC_I420;
    best_format->interval = desired.interval;

    return true;
}


bool VideoPacketSource::IsScreencast() const
{
    return false;
}


} // namespace scy
