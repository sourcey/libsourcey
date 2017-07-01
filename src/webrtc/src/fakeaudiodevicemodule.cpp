///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup webrtc
/// @{


#include "scy/webrtc/fakeaudiodevicemodule.h"

#include "webrtc/base/refcount.h"
#include "webrtc/base/thread.h"
#include "webrtc/base/timeutils.h"

#include "scy/logger.h"


using std::endl;


namespace scy {
namespace wrtc {


// Same value as src/modules/audio_device/main/source/audio_device_config.h in
// https://code.google.com/p/webrtc/
static const int kAdmMaxIdleTimeProcess = 1000;
static const uint32_t kMaxVolume = 14392;


FakeAudioDeviceModule::FakeAudioDeviceModule()
    : _lastProcessTimeMS(0)
    , _audioCallback(nullptr)
    , _recording(false)
    , _playing(false)
    , _playIsInitialized(false)
    , _recIsInitialized(false)
    , _currentMicLevel(kMaxVolume)
{
}

FakeAudioDeviceModule::~FakeAudioDeviceModule()
{
}

rtc::scoped_refptr<FakeAudioDeviceModule> FakeAudioDeviceModule::Create()
{
    rtc::scoped_refptr<FakeAudioDeviceModule> capture_module(
        new rtc::RefCountedObject<FakeAudioDeviceModule>());
    return capture_module;
}

void FakeAudioDeviceModule::OnMessage(rtc::Message* msg)
{
}

int64_t FakeAudioDeviceModule::TimeUntilNextProcess()
{
    const int64_t current_time = rtc::TimeMillis();
    if (current_time < _lastProcessTimeMS) {
        // TODO: wraparound could be handled more gracefully.
        return 0;
    }
    const int64_t elapsed_time = current_time - _lastProcessTimeMS;
    if (kAdmMaxIdleTimeProcess < elapsed_time) {
        return 0;
    }
    return kAdmMaxIdleTimeProcess - elapsed_time;
}

void FakeAudioDeviceModule::Process()
{
    _lastProcessTimeMS = rtc::TimeMillis();
}

int32_t FakeAudioDeviceModule::ActiveAudioLayer(AudioLayer* /*audio_layer*/) const
{
    assert(false);
    return 0;
}

webrtc::AudioDeviceModule::ErrorCode FakeAudioDeviceModule::LastError() const
{
    assert(false);
    return webrtc::AudioDeviceModule::kAdmErrNone;
}

int32_t FakeAudioDeviceModule::RegisterEventObserver(webrtc::AudioDeviceObserver* /*event_callback*/)
{
    // Only used to report warnings and errors. This fake implementation won't
    // generate any so discard this callback.
    return 0;
}

int32_t FakeAudioDeviceModule::RegisterAudioCallback(webrtc::AudioTransport* audio_callback)
{
    // rtc::CritScope cs(&_critCallback);
    // _audioCallback = audio_callback;
    return 0;
}

int32_t FakeAudioDeviceModule::Init()
{
    // Initialize is called by the factory method.
    // Safe to ignore this Init call.
    return 0;
}

int32_t FakeAudioDeviceModule::Terminate()
{
    // Clean up in the destructor. No action here, just success.
    return 0;
}

bool FakeAudioDeviceModule::Initialized() const
{
    assert(false);
    return 0;
}

int16_t FakeAudioDeviceModule::PlayoutDevices()
{
    assert(false);
    return 0;
}

int16_t FakeAudioDeviceModule::RecordingDevices()
{
    assert(false);
    return 0;
}

int32_t FakeAudioDeviceModule::PlayoutDeviceName(uint16_t /*index*/,
    char /*name*/[webrtc::kAdmMaxDeviceNameSize],
    char /*guid*/[webrtc::kAdmMaxGuidSize])
{
    assert(false);
    return 0;
}

int32_t FakeAudioDeviceModule::RecordingDeviceName(uint16_t /*index*/,
    char /*name*/[webrtc::kAdmMaxDeviceNameSize],
    char /*guid*/[webrtc::kAdmMaxGuidSize])
{
    assert(false);
    return 0;
}

int32_t FakeAudioDeviceModule::SetPlayoutDevice(uint16_t /*index*/)
{
    // No playout device, just playing from file. Return success.
    return 0;
}

int32_t FakeAudioDeviceModule::SetPlayoutDevice(WindowsDeviceType /*device*/)
{
    if (_playIsInitialized) {
        return -1;
    }
    return 0;
}

int32_t FakeAudioDeviceModule::SetRecordingDevice(uint16_t /*index*/)
{
    // No recording device, just dropping audio. Return success.
    return 0;
}

int32_t FakeAudioDeviceModule::SetRecordingDevice(WindowsDeviceType /*device*/)
{
    if (_recIsInitialized) {
        return -1;
    }
    return 0;
}

int32_t FakeAudioDeviceModule::PlayoutIsAvailable(bool* /*available*/)
{
    assert(false);
    return 0;
}

int32_t FakeAudioDeviceModule::InitPlayout()
{
    _playIsInitialized = true;
    return 0;
}

bool FakeAudioDeviceModule::PlayoutIsInitialized() const
{
    return _playIsInitialized;
}

int32_t FakeAudioDeviceModule::RecordingIsAvailable(bool* /*available*/)
{
    assert(false);
    return 0;
}

int32_t FakeAudioDeviceModule::InitRecording()
{
    _recIsInitialized = true;
    return 0;
}

bool FakeAudioDeviceModule::RecordingIsInitialized() const
{
    return _recIsInitialized;
}

int32_t FakeAudioDeviceModule::StartPlayout()
{
    LDebug("Start playout")
    if (!_playIsInitialized) {
        return -1;
    }
    {
        rtc::CritScope cs(&_crit);
        _playing = true;
    }
    return 0;
}

int32_t FakeAudioDeviceModule::StopPlayout()
{
    LDebug("Stop playout")
    {
        rtc::CritScope cs(&_crit);
        _playing = false;
    }
    return 0;
}

bool FakeAudioDeviceModule::Playing() const
{
    rtc::CritScope cs(&_crit);
    return _playing;
}

int32_t FakeAudioDeviceModule::StartRecording()
{
    LDebug("Start recording")
    if (!_recIsInitialized) {
        return -1;
    }
    {
        rtc::CritScope cs(&_crit);
        _recording = true;
    }
    return 0;
}

int32_t FakeAudioDeviceModule::StopRecording()
{
    LDebug("Stop recording")
    {
        rtc::CritScope cs(&_crit);
        _recording = false;
    }
    return 0;
}

bool FakeAudioDeviceModule::Recording() const
{
    rtc::CritScope cs(&_crit);
    return _recording;
}

int32_t FakeAudioDeviceModule::SetAGC(bool /*enable*/)
{
    // No AGC but not needed since audio is pregenerated. Return success.
    return 0;
}

bool FakeAudioDeviceModule::AGC() const
{
    assert(false);
    return 0;
}

int32_t FakeAudioDeviceModule::SetWaveOutVolume(uint16_t /*volume_left*/,
                                            uint16_t /*volume_right*/)
{
    assert(false);
    return 0;
}

int32_t FakeAudioDeviceModule::WaveOutVolume(uint16_t* /*volume_left*/,
                                         uint16_t* /*volume_right*/) const
{
    assert(false);
    return 0;
}

int32_t FakeAudioDeviceModule::InitSpeaker()
{
    // No speaker, just playing from file. Return success.
    return 0;
}

bool FakeAudioDeviceModule::SpeakerIsInitialized() const
{
    assert(false);
    return 0;
}

int32_t FakeAudioDeviceModule::InitMicrophone()
{
    // No microphone, just playing from file. Return success.
    return 0;
}

bool FakeAudioDeviceModule::MicrophoneIsInitialized() const
{
    assert(false);
    return 0;
}

int32_t FakeAudioDeviceModule::SpeakerVolumeIsAvailable(bool* /*available*/)
{
    assert(false);
    return 0;
}

int32_t FakeAudioDeviceModule::SetSpeakerVolume(uint32_t /*volume*/)
{
    assert(false);
    return 0;
}

int32_t FakeAudioDeviceModule::SpeakerVolume(uint32_t* /*volume*/) const
{
    assert(false);
    return 0;
}

int32_t FakeAudioDeviceModule::MaxSpeakerVolume(uint32_t* /*max_volume*/) const
{
    assert(false);
    return 0;
}

int32_t FakeAudioDeviceModule::MinSpeakerVolume(uint32_t* /*min_volume*/) const
{
    assert(false);
    return 0;
}

int32_t FakeAudioDeviceModule::SpeakerVolumeStepSize(uint16_t* /*step_size*/) const
{
    assert(false);
    return 0;
}

int32_t FakeAudioDeviceModule::MicrophoneVolumeIsAvailable(bool* /*available*/)
{
    assert(false);
    return 0;
}

int32_t FakeAudioDeviceModule::SetMicrophoneVolume(uint32_t volume)
{
    rtc::CritScope cs(&_crit);
    _currentMicLevel = volume;
    return 0;
}

int32_t FakeAudioDeviceModule::MicrophoneVolume(uint32_t* volume) const
{
    rtc::CritScope cs(&_crit);
    *volume = _currentMicLevel;
    return 0;
}

int32_t FakeAudioDeviceModule::MaxMicrophoneVolume(uint32_t* max_volume) const
{
    *max_volume = kMaxVolume;
    return 0;
}

int32_t FakeAudioDeviceModule::MinMicrophoneVolume(uint32_t* /*min_volume*/) const
{
    assert(false);
    return 0;
}

int32_t
FakeAudioDeviceModule::MicrophoneVolumeStepSize(uint16_t* /*step_size*/) const
{
    assert(false);
    return 0;
}

int32_t FakeAudioDeviceModule::SpeakerMuteIsAvailable(bool* /*available*/)
{
    assert(false);
    return 0;
}

int32_t FakeAudioDeviceModule::SetSpeakerMute(bool /*enable*/)
{
    assert(false);
    return 0;
}

int32_t FakeAudioDeviceModule::SpeakerMute(bool* /*enabled*/) const
{
    assert(false);
    return 0;
}

int32_t FakeAudioDeviceModule::MicrophoneMuteIsAvailable(bool* /*available*/)
{
    assert(false);
    return 0;
}

int32_t FakeAudioDeviceModule::SetMicrophoneMute(bool /*enable*/)
{
    assert(false);
    return 0;
}

int32_t FakeAudioDeviceModule::MicrophoneMute(bool* /*enabled*/) const
{
    assert(false);
    return 0;
}

int32_t FakeAudioDeviceModule::MicrophoneBoostIsAvailable(bool* /*available*/)
{
    assert(false);
    return 0;
}

int32_t FakeAudioDeviceModule::SetMicrophoneBoost(bool /*enable*/)
{
    assert(false);
    return 0;
}

int32_t FakeAudioDeviceModule::MicrophoneBoost(bool* /*enabled*/) const
{
    assert(false);
    return 0;
}

int32_t FakeAudioDeviceModule::StereoPlayoutIsAvailable(bool* available) const
{
    // No recording device, just dropping audio. Stereo can be dropped just
    // as easily as mono.
    *available = true;
    return 0;
}

int32_t FakeAudioDeviceModule::SetStereoPlayout(bool /*enable*/)
{
    // No recording device, just dropping audio. Stereo can be dropped just
    // as easily as mono.
    return 0;
}

int32_t FakeAudioDeviceModule::StereoPlayout(bool* /*enabled*/) const
{
    assert(false);
    return 0;
}

int32_t FakeAudioDeviceModule::StereoRecordingIsAvailable(bool* available) const
{
    // Keep thing simple. No stereo recording.
    *available = false;
    return 0;
}

int32_t FakeAudioDeviceModule::SetStereoRecording(bool enable)
{
    if (!enable) {
        return 0;
    }
    return -1;
}

int32_t FakeAudioDeviceModule::StereoRecording(bool* /*enabled*/) const
{
    assert(false);
    return 0;
}

int32_t FakeAudioDeviceModule::SetRecordingChannel(const ChannelType channel)
{
    if (channel != AudioDeviceModule::kChannelBoth) {
        // There is no right or left in mono. I.e. kChannelBoth should be used
        // for
        // mono.
        assert(false);
        return -1;
    }
    return 0;
}

int32_t FakeAudioDeviceModule::RecordingChannel(ChannelType* channel) const
{
    // Stereo recording not supported. However, WebRTC ADM returns kChannelBoth
    // in that case. Do the same here.
    *channel = AudioDeviceModule::kChannelBoth;
    return 0;
}

int32_t FakeAudioDeviceModule::SetPlayoutBuffer(const BufferType /*type*/,
                                            uint16_t /*size_ms*/)
{
    assert(false);
    return 0;
}

int32_t FakeAudioDeviceModule::PlayoutBuffer(BufferType* /*type*/,
                                         uint16_t* /*size_ms*/) const
{
    assert(false);
    return 0;
}

int32_t FakeAudioDeviceModule::PlayoutDelay(uint16_t* delay_ms) const
{
    // No delay since audio frames are dropped.
    *delay_ms = 0;
    return 0;
}

int32_t FakeAudioDeviceModule::RecordingDelay(uint16_t* /*delay_ms*/) const
{
    assert(false);
    return 0;
}

int32_t FakeAudioDeviceModule::CPULoad(uint16_t* /*load*/) const
{
    assert(false);
    return 0;
}

int32_t FakeAudioDeviceModule::StartRawOutputFileRecording(
    const char /*pcm_file_name_utf8*/[webrtc::kAdmMaxFileNameSize])
{
    assert(false);
    return 0;
}

int32_t FakeAudioDeviceModule::StopRawOutputFileRecording()
{
    assert(false);
    return 0;
}

int32_t FakeAudioDeviceModule::StartRawInputFileRecording(
    const char /*pcm_file_name_utf8*/[webrtc::kAdmMaxFileNameSize])
{
    assert(false);
    return 0;
}

int32_t FakeAudioDeviceModule::StopRawInputFileRecording()
{
    assert(false);
    return 0;
}

int32_t
FakeAudioDeviceModule::SetRecordingSampleRate(const uint32_t /*samples_per_sec*/)
{
    assert(false);
    return 0;
}

int32_t
FakeAudioDeviceModule::RecordingSampleRate(uint32_t* /*samples_per_sec*/) const
{
    assert(false);
    return 0;
}

int32_t
FakeAudioDeviceModule::SetPlayoutSampleRate(const uint32_t /*samples_per_sec*/)
{
    assert(false);
    return 0;
}

int32_t
FakeAudioDeviceModule::PlayoutSampleRate(uint32_t* /*samples_per_sec*/) const
{
    assert(false);
    return 0;
}

int32_t FakeAudioDeviceModule::ResetAudioDevice()
{
    assert(false);
    return 0;
}

int32_t FakeAudioDeviceModule::SetLoudspeakerStatus(bool /*enable*/)
{
    assert(false);
    return 0;
}

int32_t FakeAudioDeviceModule::GetLoudspeakerStatus(bool* /*enabled*/) const
{
    assert(false);
    return 0;
}


} } // namespace scy::wrtc


/// @\}
