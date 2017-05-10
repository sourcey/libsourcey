///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup av
/// @{


#ifndef SCY_AV_AudioCapture_H
#define SCY_AV_AudioCapture_H


#include "scy/base.h"

#ifdef HAVE_RTAUDIO

#include "scy/av/format.h"
#include "scy/av/icapture.h"
#include "scy/av/types.h"
#include "scy/signal.h"

#include "RtAudio.h"

#include <iostream>
#include <map>
#include <queue>


namespace scy {
namespace av {


DefinePolymorphicDelegateWithArg(audioDelegate, IPacket, PacketDelegateBase,
                                 void*, nullptr)

    /// Implements a cross platfrom audio device capture class.
    ///
    /// NOTE: Audio buffer samples are always in interleaved, not planar channel
    /// format for ease of use ie. `c1 c1 c2 c2 c1 c1 c2 c2...`
    class AudioCapture : public ICapture
{
public:
    typedef std::shared_ptr<AudioCapture> Ptr;

    AudioCapture(int deviceId, int channels, int sampleRate,
                 RtAudioFormat format = RTAUDIO_SINT16);
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
    /// Sets the error message and throws an exception.
    virtual void setError(const std::string& message, bool throwExec = true);


    static int audioCallback(
        void* outputBuffer, void* inputBuffer,
        unsigned int nBufferFrames, // The system audio capture callback.
        /// Samples will be dispatched from the audio capture thread,    // so
        /// proper synchronization is required.
        double streamTime, RtAudioStreamStatus status, void* data);

    /// The system audio error callback.
    /// Since this static method provides no client data argument we just log
    /// the error.
    /// Errors *may* be dispatched from the audio capture thread,    // so
    /// proper synchronization is required.
    static void errorCallback(RtAudioError::Type type,
                              const std::string& errorText);


private:
    mutable std::mutex _mutex;
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


} // namespace av
} // namespace scy


#endif
#endif
