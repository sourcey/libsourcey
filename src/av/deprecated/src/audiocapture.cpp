///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup av
/// @{


#include "scy/av/mediacapture.h"
#include "scy/logger.h"

#ifdef HAVE_RTAUDIO


using std::endl;


namespace scy {
namespace av {


AudioCapture::AudioCapture(int deviceId, int channels, int sampleRate,
                           RtAudioFormat format)
    : _deviceId(deviceId)
    , _channels(channels)
    , _sampleRate(sampleRate)
    , _format(format)
    , _opened(false)
{
    LTrace("Create")

    _iParams.deviceId = _deviceId;
    _iParams.nChannels = _channels;
    _iParams.firstChannel = 0;

    if (_audio.getDeviceCount() < 1) {
        LWarn("No audio devices found!")
        return;
    }

    // Let RtAudio print messages to stderr.
    _audio.showWarnings(true);

    // Open the audio stream or throw an exception.
    open(); // channels, sampleRate
    LTrace("Create: OK")
}


AudioCapture::~AudioCapture()
{
    LTrace("Destroy")
}


void AudioCapture::open() // int channels, int sampleRate, RtAudioFormat format
{
    if (isOpen())
        close();

    std::lock_guard<std::mutex> guard(_mutex);
    LTrace("Opening: ",  _channels,  ": ", _sampleRate)

    // 1024 is a common frame size for many codecs.
    unsigned int nBufferFrames = 1024; // 256, 512

    RtAudio::StreamOptions options;
    options.flags = RTAUDIO_SCHEDULE_REALTIME;

    // TODO: Implement planar audio format to pass the `RTAUDIO_NONINTERLEAVED`
    // flag to RtAudio::StreamOptions. Some work will be required for the packet
    // interface to support planar buffer formats.

    try {
        _audio.openStream(nullptr, &_iParams, _format, _sampleRate,
                          &nBufferFrames, &AudioCapture::audioCallback,
                          (void*)this, &options, AudioCapture::errorCallback);

        _error = "";
        _opened = true;
        LTrace("Opening: OK")
    } catch (RtAudioError& e) {
        setError("Cannot open audio capture: " + e.getMessage());
    } catch (...) {
        setError("Cannot open audio capture.");
    }
}


void AudioCapture::close()
{
    LTrace("Closing")
    try {
        std::lock_guard<std::mutex> guard(_mutex);
        _opened = false;
        if (_audio.isStreamOpen())
            _audio.closeStream();
        LTrace("Closing: OK")
    } catch (RtAudioError& e) {
        setError("Cannot close audio capture: " + e.getMessage());
    } catch (...) {
        setError("Cannot close audio capture.");
    }
}


void AudioCapture::start()
{
    LTrace("Starting")

    if (!running()) {
        try {
            std::lock_guard<std::mutex> guard(_mutex);
            _audio.startStream();
            _error = "";
            LTrace("Starting: OK")
        } catch (RtAudioError& e) {
            setError("Cannot start audio capture: " + e.getMessage());
        } catch (...) {
            setError("Cannot start audio capture.");
        }
    }
}


void AudioCapture::stop()
{
    LTrace("Stopping")

    if (running()) {
        try {
            std::lock_guard<std::mutex> guard(_mutex);
            LTrace("Stopping: Before")
            _audio.stopStream();
            LTrace("Stopping: OK")
        } catch (RtAudioError& e) {
            setError("Cannot stop audio capture: " + e.getMessage());
        } catch (...) {
            setError("Cannot stop audio capture.");
        }
    }
}


#if 0
void AudioCapture::attach(const PacketDelegateBase& delegate)
{
    PacketSignal::attach(delegate);
    LTrace("Added Delegate: ", refCount())
    if (refCount() == 1)
        start();
}


bool AudioCapture::detach(const PacketDelegateBase& delegate)
{
    if (PacketSignal::detach(delegate)) {
        LTrace("Removed Delegate: ", refCount())
        if (refCount() == 0)
            stop();
        LTrace("Removed Delegate: OK")
        return true;
    }
    return false;
}
#endif


int AudioCapture::audioCallback(void* /* outputBuffer */, void* inputBuffer,
                                unsigned int nBufferFrames, double streamTime,
                                RtAudioStreamStatus status, void* data)
{
    auto self = reinterpret_cast<AudioCapture*>(data);
    AudioPacket packet;

    if (status)
        LError("Stream over/underflow detected")

    assert(inputBuffer);
    if (inputBuffer == nullptr) {
        LError("Input buffer is NULL")
        return 2;
    }

    {
        std::lock_guard<std::mutex> guard(_mutex);
        packet.setData((char*)inputBuffer,
                       nBufferFrames * self->_channels * sizeof(RtAudioFormat));
        packet.numSamples = nBufferFrames;
        packet.time = streamTime;
    }

    // STrace << "Captured audio packet: "
    //      << "\n\tPacket Ptr: " << inputBuffer
    //      << "\n\tPacket Size: " << packet.size()
    //      << "\n\tStream Time: " << packet.time
    //      << endl;

    LTrace("Emitting: ", packet.time)
    self->emit(packet);
    return 0;
}


void AudioCapture::errorCallback(RtAudioError::Type type,
                                 const std::string& errorText)
{
    LError("Audio system error: ", errorText)
}


void AudioCapture::setError(const std::string& message, bool throwExec)
{
    LError("Error: ", message)
    _error = message;
    if (throwExec)
        throw std::runtime_error(message);
}


RtAudioFormat AudioCapture::format() const
{
    std::lock_guard<std::mutex> guard(_mutex);
    return _format;
}


bool AudioCapture::isOpen() const
{
    std::lock_guard<std::mutex> guard(_mutex);
    return _opened;
}


bool AudioCapture::running() const
{
    std::lock_guard<std::mutex> guard(_mutex);
    return _audio.isStreamRunning();
}


int AudioCapture::deviceId() const
{
    std::lock_guard<std::mutex> guard(_mutex);
    return _deviceId;
}


int AudioCapture::sampleRate() const
{
    std::lock_guard<std::mutex> guard(_mutex);
    return _sampleRate;
}


int AudioCapture::channels() const
{
    std::lock_guard<std::mutex> guard(_mutex);
    return _channels;
}


std::string AudioCapture::formatString() const
{
    // FFmpeg sample formats: `ffmpeg -sample_fmts`
    // name   depth
    // u8        8
    // s16      16
    // s32      32
    // flt      32
    // dbl      64
    // u8p       8
    // s16p     16
    // s32p     32
    // fltp     32
    // dblp     64

    bool planar = false; // planar audio not implemented
    switch (format()) {
        case RTAUDIO_SINT8:
            return planar ? "u8p" : "u8";
        case RTAUDIO_SINT16:
            return planar ? "s16p" : "s16";
        case RTAUDIO_SINT24:
            return planar ? "s32p" : "s32";
        case RTAUDIO_SINT32:
            return planar ? "s32p" : "s32";
        case RTAUDIO_FLOAT32:
            return planar ? "fltp" : "flt";
        case RTAUDIO_FLOAT64:
            return planar ? "dblp" : "dbl";
    }
    assert(0 && "unsupported pixel format");
    return "";
}


void AudioCapture::getAudioCodec(AudioCodec& icodec)
{
    icodec.channels = channels();
    icodec.sampleRate = sampleRate();
    icodec.sampleFmt = formatString();
}

void AudioCapture::getEncoderFormat(Format& iformat)
{
    iformat.name = "PCM";
    iformat.audio.enabled = true;
    getAudioCodec(iformat.audio);
    // bool planar = true;
    // switch(_format) {
    //   case RTAUDIO_SINT8:
    //     iformat.audio.sampleFmt = planar ? "u8p" : "u8";
    //     break;
    //   case RTAUDIO_SINT16:
    //     iformat.audio.sampleFmt = planar ? "s16p" : "s16";
    //     break;
    //   case RTAUDIO_SINT24:
    //     iformat.audio.sampleFmt = planar ? "s32p" : "s32";
    //     break;
    //   case RTAUDIO_SINT32:
    //     iformat.audio.sampleFmt = planar ? "s32p" : "s32";
    //     break;
    //   case RTAUDIO_FLOAT32:
    //     iformat.audio.sampleFmt = planar ? "fltp" : "flt";
    //     break;
    //   case RTAUDIO_FLOAT64:
    //     iformat.audio.sampleFmt = planar ? "dblp" : "dbl";
    //     break;
    // }
}


} // namespace av
} // namespace scy


#endif
