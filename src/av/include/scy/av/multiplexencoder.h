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


#ifndef SCY_AV_MultiplexEncoder_H
#define SCY_AV_MultiplexEncoder_H


#include "scy/base.h"

#ifdef HAVE_FFMPEG

#include "scy/packetstream.h"
#include "scy/av/types.h"
#include "scy/av/ffmpeg.h"
#include "scy/av/iencoder.h"
#include "scy/av/videoencoder.h"
#include "scy/av/audioencoder.h"
#include "scy/mutex.h"
#include <fstream>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/fifo.h>
#include <libswscale/swscale.h>
}


namespace scy {
namespace av {


class MultiplexEncoder: public IEncoder
    /// This class implements a multiplex audio and video encoder.
{
public:
    MultiplexEncoder(const EncoderOptions& options = EncoderOptions());
    virtual ~MultiplexEncoder();

    virtual void initialize();
    virtual void uninitialize();
    virtual void cleanup();

    virtual void createVideo();
    virtual void freeVideo();
    virtual bool encodeVideo(AVFrame* frame);
    virtual bool encodeVideo(std::uint8_t* buffer, int bufferSize, int width, int height, std::int64_t time = AV_NOPTS_VALUE);
        // Encode a single video frame
        // If the time is specified it should be the millisecond offset since
        // the start of the input stream. The value will be converted to the
        // stream time base internally.
        // If no time is specified a realtime time value will be assigned to
        // the frame.

    virtual void createAudio();
    virtual void freeAudio();
    // virtual bool encodeAudio(AVFrame* frame);
    virtual bool encodeAudio(std::uint8_t* buffer, int numSamples, std::int64_t time = AV_NOPTS_VALUE);
        // Encode a single audio frame

    virtual void flush();
        // Flush and beffered or queued packets.

    EncoderOptions& options();
    VideoEncoder* video();
    AudioEncoder* audio();

    PacketSignal emitter;

protected:
    bool writeOutputPacket(AVPacket& packet);
    void updateStreamPts(AVStream* stream, std::int64_t* pts);

    void onVideoEncoded(av::VideoPacket& packet);
    void onAudioEncoded(av::AudioPacket& packet);

    //static Mutex _mutex; // Protects avcodec_open/close()

    EncoderOptions _options;
    AVFormatContext* _formatCtx;
    VideoEncoder* _video;
    AudioEncoder* _audio;
    AVIOContext* _ioCtx;
    std::uint8_t* _ioBuffer;
    int _ioBufferSize;
    std::uint64_t _pts;
};


// bool writeOutputPacket(AVFormatContext *formatCtx, AVPacket& packet);


} } // namespace scy::av


#endif
#endif // SCY_AV_MultiplexEncoder_H
