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

#ifndef SCY_WebRTC_ImageSequenceRecorder_H
#define SCY_WebRTC_ImageSequenceRecorder_H


#include "scy/av/videoencoder.h"

#include "webrtc/api/peerconnectioninterface.h"


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
    av::VideoEncoder _encoder;
    AVFrame* _avframe;
};


} // namespace scy


#endif
