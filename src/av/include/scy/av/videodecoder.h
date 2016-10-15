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


#ifndef SCY_AV_VideoDecoder_H
#define SCY_AV_VideoDecoder_H


#include "scy/base.h"

#ifdef HAVE_FFMPEG

// #include "scy/mutex.h"
// #include "scy/timer.h"
#include "scy/av/types.h"
#include "scy/av/format.h"
#include "scy/av/ffmpeg.h"
#include "scy/av/videocontext.h"

// extern "C" {
// #include <libavcodec/avcodec.h>
// #include <libavformat/avformat.h>
// #include <libavutil/fifo.h>
// #include <libavutil/opt.h>
// #include <libavutil/pixdesc.h>
// #include <libswscale/swscale.h>
// #include <libavutil/avutil.h>
// #include <libavutil/imgutils.h>
// }


namespace scy {
namespace av {


struct VideoDecoderContext: public VideoContext
{
    VideoDecoderContext();
    virtual ~VideoDecoderContext();

    virtual void create(AVFormatContext *ic, int streamID);
    //virtual void open();
    virtual void close();

    virtual bool decode(std::uint8_t* data, int size, AVPacket& opacket);
    virtual bool decode(AVPacket& ipacket, AVPacket& opacket);
        // Decodes a the given input packet.
        // Returns true an output packet was returned,
        // false otherwise.

    virtual bool flush(AVPacket& opacket);
        // Flushes buffered frames.
        // This method should be called after decoding
        // until false is returned.

    //double maxFPS;
        // Maximum decoding FPS.
        // FPS is calculated from ipacket PTS.
        // Extra frames will be dropped.
};


} } // namespace scy::av


#endif
#endif // SCY_AV_VideoDecoder_H
