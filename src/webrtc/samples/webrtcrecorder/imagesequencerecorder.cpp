//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// LibSourcey is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#include "imagesequencerecorder.h"

#include "scy/logger.h"
#include "scy/filesystem.h"
#include "scy/av/ffmpeg.h"

#include "webrtc/modules/video_capture/video_capture_factory.h"
#include "webrtc/media/engine/webrtcvideocapturerfactory.h"
#include "webrtc/common_video/libyuv/include/webrtc_libyuv.h"


namespace scy {


ImageSequenceRecorder::ImageSequenceRecorder(webrtc::VideoTrackInterface* track_to_render, const std::string& basename)
    : _renderedTrack(track_to_render),
      _basename(basename),
      _count(0),
      _width(0),
      _height(0)
{
    _renderedTrack->AddOrUpdateSink(this, rtc::VideoSinkWants());

    av::initializeFFmpeg();
}


ImageSequenceRecorder::~ImageSequenceRecorder()
{
    _renderedTrack->RemoveSink(this);

    av::uninitializeFFmpeg();

    // Cleanup the frame
    if (_avframe)
        av_free(_avframe);
}


std::string ImageSequenceRecorder::getNextFilename()
{
    std::stringstream filename;
    filename << _basename;
    if (++_count > 1)
        filename << '-' << _count;
    filename << '_' << _width << 'x' << _height << ".jpg";
    return filename.str();
}


void ImageSequenceRecorder::OnFrame(const cricket::VideoFrame& yuvframe)
{
    TraceL << "On video frame: " << yuvframe.width() << 'x' << yuvframe.height() << std::endl;

    if (_width != yuvframe.width() ||
        _height != yuvframe.height()) {
        _width = yuvframe.width();
        _height = yuvframe.height();

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

        // Cleanup the previous frame (if any)
        if (_avframe)
            av_free(_avframe);

        // Initialize the AVFrame
        _avframe = av_frame_alloc();
        _avframe->width = yuvframe.width();
        _avframe->height = yuvframe.height();
        _avframe->format = AV_PIX_FMT_YUV420P;

        // Initialize avframe->linesize
        avpicture_fill((AVPicture*)_avframe, NULL, /*_avframe->format*/AV_PIX_FMT_YUV420P, _avframe->width, _avframe->height);
    }

    // Set avframe->data pointers manually
    _avframe->data[0] = (uint8_t*)yuvframe.video_frame_buffer()->DataY();
    _avframe->data[1] = (uint8_t*)yuvframe.video_frame_buffer()->DataU();
    _avframe->data[2] = (uint8_t*)yuvframe.video_frame_buffer()->DataV();

    // Feed the raw frame into the encoder
    AVPacket opacket;
    if (_encoder.encode(_avframe, opacket)) {

        // Write the output file
        std::string filename(getNextFilename());
        TraceL << "Write video frame: " << filename << std::endl;
        fs::savefile(filename, (const char*)opacket.data, (std::streamsize)opacket.size);
    }
}


} // namespace scy
