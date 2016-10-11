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


#ifndef SCY_AV_AudioCapture_H
#define SCY_AV_AudioCapture_H


#include "scy/base.h"

#ifdef HAVE_RTAUDIO

#include "scy/av/types.h"
#include "scy/av/icapture.h"
#include "scy/av/format.h"
#include "scy/signal.h"

#include "RtAudio.h"

#include <iostream>
#include <queue>
#include <map>


namespace scy {
namespace av {


DefinePolymorphicDelegateWithArg(audioDelegate, IPacket, PacketDelegateBase, void*, nullptr)


class AudioCapture: public ICapture
    /// Implements a cross platfrom audio device capture class.
    ///
    /// NOTE: Audio buffer samples are always in interleaved, not planar channel
    /// format for ease of use ie. `c1 c1 c2 c2 c1 c1 c2 c2...`
{
public:
    typedef std::shared_ptr<AudioCapture> Ptr;

    AudioCapture(int deviceId, int channels, int sampleRate, RtAudioFormat format = RTAUDIO_SINT16);
    virtual ~AudioCapture();

    virtual void open();
    virtual void close();

    virtual void start();
    virtual void stop();

    int deviceId() const;
    int sampleRate() const;
    int channels() const;
    bool running() const;
    bool isOpen() const;
    RtAudioFormat format() const;
    std::string formatString() const;

    void getEncoderFormat(Format& iformat);
    void getAudioCodec(AudioCodec& icodec);

protected:
    virtual void setError(const std::string& message, bool throwExec = true);
        // Sets the error message and throws an exception.

    static int audioCallback(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
                             double streamTime, RtAudioStreamStatus status, void *data);
        // The system audio capture callback.
        // Samples will be dispatched from the audio capture thread,
        // so proper synchronization is required.

    static void errorCallback(RtAudioError::Type type, const std::string &errorText);
        // The system audio error callback.
        // Since this static method provides no client data argument we just log the error.
        // Errors *may* be dispatched from the audio capture thread,
        // so proper synchronization is required.


private:
    mutable Mutex _mutex;
    int _deviceId;
    int _channels;
    int _sampleRate;
    RtAudio _audio;
    RtAudioFormat _format;
    RtAudio::StreamParameters _iParams;
    std::string _error;
    bool _opened;
};


typedef std::map<int, AudioCapture*> AudioCaptureMap;


} } // namespace scy::av


#endif
#endif
