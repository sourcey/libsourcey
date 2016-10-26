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


#ifndef SCY_AV_MediaCapture_H
#define SCY_AV_MediaCapture_H


#include "scy/base.h"

#ifdef HAVE_FFMPEG

#include "scy/packetsignal.h"
#include "scy/interface.h"
#include "scy/av/types.h"
#include "scy/av/ffmpeg.h"
#include "scy/av/icapture.h"
#include "scy/av/videodecoder.h"
#include "scy/av/audiodecoder.h"
#include "scy/mutex.h"


namespace scy {
namespace av {


class MediaCapture: public ICapture, public async::Runnable
    /// This class implements a cross platform audio, video, screen and
    /// video file capturer.
{
public:
    typedef std::shared_ptr<MediaCapture> Ptr;

    MediaCapture();
    virtual ~MediaCapture();

    virtual void openFile(const std::string& file);
// #ifdef HAVE_FFMPEG_AVDEVICE
    // virtual void openCamera(const std::string& device, int width = -1, int height = -1, double framerate = -1);
    // virtual void openMicrophone(const std::string& device, int channels = -1, int sampleRate = -1);
// #endif
    virtual void close();

    virtual void start();
    virtual void stop();

    virtual void run();

    virtual void getEncoderFormat(Format& format);
    virtual void getEncoderAudioCodec(AudioCodec& params);
    virtual void getEncoderVideoCodec(VideoCodec& params);

    AVFormatContext* formatCtx() const;
    VideoDecoder* video() const;
    AudioDecoder* audio() const;
    bool stopping() const;
    std::string error() const;

    NullSignal Closing;
        // Notifies that the capture thread is closing.
        // Careful, this signal is emitted from inside the tread contect.

protected:
    virtual void openStream(const std::string& filename, AVInputFormat* inputFormat, AVDictionary** formatParams);

    void emit(IPacket& packet);

protected:
    mutable Mutex _mutex;
    Thread _thread;
    AVFormatContext* _formatCtx;
    VideoDecoder* _video;
    AudioDecoder* _audio;
    std::string _error;
    bool _stopping;
};


DefinePolymorphicDelegateWithArg(audioDelegate, IPacket, PacketDelegateBase, void*, nullptr)


} } // namespace scy::av


#endif
#endif // SCY_AV_MediaCapture_H
