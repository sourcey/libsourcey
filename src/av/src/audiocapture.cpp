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


#include "scy/av/audiocapture.h"
#include "scy/logger.h"

#ifdef HAVE_RTAUDIO


using std::endl;


namespace scy {
namespace av {


AudioCapture::AudioCapture(int deviceId, int channels, int sampleRate, RtAudioFormat format) :
    _deviceId(deviceId),
    _channels(channels),
    _sampleRate(sampleRate),
    _format(format),
    _opened(false)
{
    TraceS(this) << "Create" << endl;

    _iParams.deviceId = _deviceId;
    _iParams.nChannels = _channels;
    _iParams.firstChannel = 0;

    if (_audio.getDeviceCount() < 1) {
        WarnL << "No audio devices found!" << endl;
        return;
    }

    // Let RtAudio print messages to stderr.
    _audio.showWarnings(true);

    // Open the audio stream or throw an exception.
    open(); //channels, sampleRate
    TraceS(this) << "Create: OK" << endl;
}


AudioCapture::~AudioCapture()
{
    TraceS(this) << "Destroy" << endl;
}


void AudioCapture::open() //int channels, int sampleRate, RtAudioFormat format
{
    if (isOpen())
        close();

    Mutex::ScopedLock lock(_mutex);
    TraceS(this) << "Opening: " << _channels << ": " << _sampleRate << endl;

    //_channels = channels;
    //_sampleRate = sampleRate;
    //_format = format;
    //_iParams.nChannels = _channels;
    unsigned int nBufferFrames = 1024; //256; //512; / 2

    try {
        _audio.openStream(nullptr, &_iParams, _format, _sampleRate, &nBufferFrames, &AudioCapture::audioCallback, (void*)this, nullptr, AudioCapture::errorCallback);

        _error = "";
        _opened = true;
        TraceS(this) << "Opening: OK" << endl;
    }
    catch (RtAudioError& e) {
        setError("Cannot open audio capture: " + e.getMessage());
    }
    catch (...) {
        setError("Cannot open audio capture.");
    }
}


void AudioCapture::close()
{
    TraceS(this) << "Closing" << endl;
    try {
        Mutex::ScopedLock lock(_mutex);
        _opened = false;
        if (_audio.isStreamOpen())
            _audio.closeStream();
        TraceS(this) << "Closing: OK" << endl;
    }
    catch (RtAudioError& e) {
        setError("Cannot close audio capture: " + e.getMessage());
    }
    catch (...) {
        setError("Cannot close audio capture.");
    }
}


void AudioCapture::start()
{
    TraceS(this) << "Starting" << endl;

    if (!running()) {
        try {
            Mutex::ScopedLock lock(_mutex);
            _audio.startStream();
            _error = "";
            TraceS(this) << "Starting: OK" << endl;
        }
        catch (RtAudioError& e) {
            setError("Cannot start audio capture: " + e.getMessage());
        }
        catch (...) {
            setError("Cannot start audio capture.");
        }
    }
}


void AudioCapture::stop()
{
    TraceS(this) << "Stopping" << endl;

    if (running()) {
        try {
            Mutex::ScopedLock lock(_mutex);
            TraceS(this) << "Stopping: Before" << endl;
            _audio.stopStream();
            TraceS(this) << "Stopping: OK" << endl;
        }
        catch (RtAudioError& e) {
            setError("Cannot stop audio capture: " + e.getMessage());
        }
        catch (...) {
            setError("Cannot stop audio capture.");
        }
    }
}


#if 0
void AudioCapture::attach(const PacketDelegateBase& delegate)
{
    PacketSignal::attach(delegate);
    TraceS(this) << "Added Delegate: " << refCount() << endl;
    if (refCount() == 1)
        start();
}


bool AudioCapture::detach(const PacketDelegateBase& delegate)
{
    if (PacketSignal::detach(delegate)) {
        TraceS(this) << "Removed Delegate: " << refCount() << endl;
        if (refCount() == 0)
            stop();
        TraceS(this) << "Removed Delegate: OK" << endl;
        return true;
    }
    return false;
}
#endif


int AudioCapture::audioCallback(void* /* outputBuffer */, void* inputBuffer, unsigned int nBufferFrames,
    double streamTime, RtAudioStreamStatus status, void* data)
{
    auto self = reinterpret_cast<AudioCapture*>(data);
    AudioPacket packet;

    if (status)
        ErrorL << "Stream over/underflow detected" << endl;

    assert(inputBuffer);
    if (inputBuffer == nullptr) {
        ErrorL << "Input buffer is NULL" << endl;
        return 2;
    }

    {
        Mutex::ScopedLock lock(self->_mutex);

        // int size = 2;
        // RtAudioFormat format = self->_format;
        // // 8-bit signed integer.
        // if (format == RTAUDIO_SINT8)
        //     size = 1;
        // // 16-bit signed integer.
        // else if (format == RTAUDIO_SINT16)
        //     size = 2;
        // // Lower 3 bytes of 32-bit signed integer.
        // else if (format == RTAUDIO_SINT24)
        //     size = 4;
        // // 32-bit signed integer.
        // else if (format == RTAUDIO_SINT32)
        //     size = 4;
        // // Normalized between plus/minus 1.0.
        // else if (format == RTAUDIO_FLOAT32)
        //     size = 4;
        // // Normalized between plus/minus 1.0.
        // else if (format == RTAUDIO_FLOAT64)
        //     size = 8;
        // else assert(0 && "unknown audio capture format");
        //
        // packet.setData((char*)inputBuffer, nBufferFrames * self->_channels * size);
        packet.setData((char*)inputBuffer, nBufferFrames * self->_channels * sizeof(RtAudioFormat));
        packet.frameSize = nBufferFrames;
        packet.time = streamTime;
    }

    // TraceL << "Captured audio packet: "
    //      << "\n\tPacket Ptr: " << inputBuffer
    //      << "\n\tPacket Size: " << packet.size()
    //      << "\n\tStream Time: " << packet.time
    //      << endl;

    TraceS(self) << "Emitting: " << packet.time << std::endl;
    self->emit(packet);
    return 0;
}


void AudioCapture::errorCallback(RtAudioError::Type type, const std::string& errorText)
{
    ErrorL << "Audio system error: " << errorText << endl;
}


void AudioCapture::setError(const std::string& message, bool throwExec)
{
    ErrorS(this) << "Error: " << message << endl;
    _error = message;
    if (throwExec)
        throw std::runtime_error(message);
}


RtAudioFormat AudioCapture::format() const
{
    Mutex::ScopedLock lock(_mutex);
    return _format;
}


bool AudioCapture::isOpen() const
{
    Mutex::ScopedLock lock(_mutex);
    return _opened;
}


bool AudioCapture::running() const
{
    Mutex::ScopedLock lock(_mutex);
    return _audio.isStreamRunning();
}


int AudioCapture::deviceId() const
{
    Mutex::ScopedLock lock(_mutex);
    return _deviceId;
}


int AudioCapture::sampleRate() const
{
    Mutex::ScopedLock lock(_mutex);
    return _sampleRate;
}


int AudioCapture::channels() const
{
    Mutex::ScopedLock lock(_mutex);
    return _channels;
}


std::string AudioCapture::formatString(bool planar) const
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
  switch(format()) {
    case RTAUDIO_SINT8:
      return planar ? "u8p" : "u8";
    case RTAUDIO_SINT16:
      return planar ? "u16p" : "u16";
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


void AudioCapture::getEncoderFormat(Format& iformat)
{
    iformat.name = "PCM";
    iformat.audio.enabled = true;
    iformat.audio.channels = channels();
    iformat.audio.sampleRate = sampleRate();
    iformat.audio.sampleFmt = formatString(true); // TODO: Convert from RtAudioFormat to AVSampleFormat // s16p
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


} } // namespace scy::av


#endif
