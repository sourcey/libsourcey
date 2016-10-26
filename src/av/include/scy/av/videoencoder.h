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


#ifndef SCY_AV_VideoEncoder_H
#define SCY_AV_VideoEncoder_H


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


struct VideoEncoder: public VideoContext
{
    VideoEncoder(AVFormatContext* format = nullptr);
    virtual ~VideoEncoder();

    virtual void create();
    //virtual void open();
    virtual void close();

    virtual bool encode(unsigned char* data, int size, std::int64_t pts); //, AVPacket& opacket
        // Encode a single video frame.
        // The pts argument should be in stream base time format.

    // virtual bool encode(AVPacket& ipacket, AVPacket& opacket);
    // virtual bool encode(AVFrame* iframe, AVPacket& opacket);
    // virtual bool flush(AVPacket& opacket);

    virtual bool encode(AVFrame* iframe); //, AVPacket& opacket
        // Encode a single AVFrame.

    virtual bool flush();
        // Flush remaining packets to be encoded.
        // This method should be called once before stream closure.

    AVFormatContext* format;
    // std::uint8_t* buffer;
    // int           bufferSize;
};


} } // namespace scy::av


#endif
#endif // SCY_AV_VideoEncoder_H
