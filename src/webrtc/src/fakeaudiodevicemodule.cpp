///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
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


// // Audio sample value that is high enough that it doesn't occur naturally when
// // frames are being faked. E.g. NetEq will not generate this large sample value
// // unless it has received an audio frame containing a sample of this value.
// // Even simpler buffers would likely just contain audio sample values of 0.
// static const int kHighSampleValue = 10000;

// Same value as src/modules/audio_device/main/source/audio_device_config.h in
// https://code.google.com/p/webrtc/
static const int kAdmMaxIdleTimeProcess = 1000;

// // Constants here are derived by running VoE using a real ADM.
// // The constants correspond to 10ms of mono audio at 44kHz.
// static const int kTimePerFrameMs = 10;
// static const uint8_t kNumberOfChannels = 2; // 1;
// static const int kSamplesPerSecond = 44000;
// static const int kTotalDelayMs = 0;
// static const int kClockDriftMs = 0;
static const uint32_t kMaxVolume = 14392;
//
// // The value for the following constants have been derived by running VoE
// // using a real ADM. The constants correspond to 10ms of mono audio at 44kHz.
// static const size_t kNumberSamples = 440;
// static const size_t kNumberBytesPerSample = sizeof(FakeAudioDeviceModule::Sample);
// static const size_t kNumberBufferBytes = kNumberSamples * kNumberBytesPerSample * kNumberOfChannels; // 1760

enum
{
    MSG_START_PROCESS,
    MSG_RUN_PROCESS
};


FakeAudioDeviceModule::FakeAudioDeviceModule()
    : _lastProcessTimeMS(0)
    , _audioCallback(nullptr)
    , _recording(false)
    , _playing(false)
    , _playIsInitialized(false)
    , _recIsInitialized(false)
    , _currentMicLevel(kMaxVolume)
    // , _started(false)
    // , _nextFrameTime(0)
    // , _sendSamples(kNumberBufferBytes)
{
}

FakeAudioDeviceModule::~FakeAudioDeviceModule()
{
    // if (_processThread) {
    //     _processThread->Stop();
    // }
}

rtc::scoped_refptr<FakeAudioDeviceModule> FakeAudioDeviceModule::Create()
{
    rtc::scoped_refptr<FakeAudioDeviceModule> capture_module(
        new rtc::RefCountedObject<FakeAudioDeviceModule>());
    // if (!capture_module->Initialize()) {
    //     return nullptr;
    // }
    return capture_module;
}

// void FakeAudioDeviceModule::onAudioCaptured(av::AudioPacket& packet)
// {
//     DebugL << "Audio frame captured" << endl;
//
//     // assert(_processThread->IsCurrent());
//     rtc::CritScope cs(&_critCallback);
//     if (!_audioCallback) {
//         return;
//     }
//
//     // TODO: Implement planar formats
//     auto data = packet.data();
//     _sendFifo.write((void**)&data, packet.numSamples);
// }

void FakeAudioDeviceModule::OnMessage(rtc::Message* msg)
{
    switch (msg->message_id) {
        case MSG_START_PROCESS:
            // startProcessP();
            break;
        case MSG_RUN_PROCESS:
            // processFrameP();
            break;
        default:
            // All existing messages should be caught. Getting here should never
            // happen.
            assert(false);
    }
}

// bool FakeAudioDeviceModule::Initialize()
// {
//     // Allocate the send audio FIFO buffer
//     _sendFifo.close();
//     _sendFifo.alloc("s16", kNumberOfChannels);
//
//     // Set the send buffer samples high enough that it would not occur on the
//     // remote side unless a packet containing a sample of that magnitude has
//     // been sent to it. Note that the audio processing pipeline will likely
//     // distort the original signal.
//     // SetSendBuffer(kHighSampleValue);
//     _lastProcessTimeMS = rtc::TimeMillis();
//     return true;
// }
//
// bool FakeAudioDeviceModule::shouldStartProcessing()
// {
//     return _recording || _playing;
// }

// void FakeAudioDeviceModule::updateProcessing(bool start)
// {
//     // if (start) {
//     //     if (!_processThread) {
//     //         _processThread.reset(new rtc::Thread());
//     //         _processThread->Start();
//     //     }
//     //     _processThread->Post(RTC_FROM_HERE, this, MSG_START_PROCESS);
//     // } else {
//     //     if (_processThread) {
//     //         _processThread->Stop();
//     //         _processThread.reset(nullptr);
//     //     }
//     //     _started = false;
//     // }
// }
//
// void FakeAudioDeviceModule::startProcessP()
// {
//     assert(_processThread->IsCurrent());
//     if (_started) {
//         // Already started.
//         return;
//     }
//     processFrameP();
// }
//
// void FakeAudioDeviceModule::processFrameP()
// {
//     assert(_processThread->IsCurrent());
//     if (!_started) {
//         _nextFrameTime = rtc::TimeMillis();
//         _started = true;
//     }
//
//     {
//         rtc::CritScope cs(&_crit);
//         // Receive and send frames every kTimePerFrameMs.
//         if (_playing) {
//             receiveFrameP();
//         }
//         if (_recording) {
//             sendFrameP();
//         }
//     }
//
//     _nextFrameTime += kTimePerFrameMs;
//     const int64_t current_time = rtc::TimeMillis();
//     const int64_t wait_time =
//         (_nextFrameTime > current_time) ? _nextFrameTime - current_time : 0;
//     _processThread->PostDelayed(RTC_FROM_HERE, wait_time, this, MSG_RUN_PROCESS);
// }
//
// void FakeAudioDeviceModule::sendFrameP()
// {
//     assert(_processThread->IsCurrent());
//     rtc::CritScope cs(&_critCallback);
//     if (!_audioCallback) {
//         return;
//     }
//     bool key_pressed = false;
//     uint32_t current_mic_level = 0;
//     MicrophoneVolume(&current_mic_level);
//
//     auto samples = &_sendSamples[0];
//     if (!_sendFifo.read((void**)&samples, kNumberSamples)) {
//         InfoL << "No audio frames in send buffer" << endl;
//         return;
//     }
//
//     TraceA("Send audio")
//     if (_audioCallback->RecordedDataIsAvailable(
//             samples, kNumberSamples, kNumberBytesPerSample, kNumberOfChannels,
//             kSamplesPerSecond, kTotalDelayMs, kClockDriftMs, current_mic_level,
//             key_pressed, current_mic_level) != 0) {
//         assert(false);
//     }
//
//     SetMicrophoneVolume(current_mic_level);
// }
//
// void FakeAudioDeviceModule::receiveFrameP()
// {
//     assert(_processThread->IsCurrent());
//     assert(0 && "playout not implemented");
//
//     // {
//     //   rtc::CritScope cs(&_critCallback);
//     //   if (!_audioCallback) {
//     //     return;
//     //   }
//     //   size_t nSamplesOut = 0;
//     //   int64_t elapsed_time_ms = 0;
//     //   int64_t ntp_time_ms = 0;
//     //   if (_audioCallback->NeedMorePlayData(kNumberSamples,
//     //                                        kNumberBytesPerSample,
//     //                                        kNumberOfChannels,
//     //                                        kSamplesPerSecond,
//     //                                        rec_sendFifo_, nSamplesOut,
//     //                                        &elapsed_time_ms, &ntp_time_ms) !=
//     //                                        0) {
//     //     assert(false);
//     //   }
//     //   assert(nSamplesOut == kNumberSamples);
//     // }
// }

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
    DebugL << "Start playout" << endl;
    if (!_playIsInitialized) {
        return -1;
    }
    {
        rtc::CritScope cs(&_crit);
        _playing = true;
    }
    // bool start = true;
    // updateProcessing(start);
    return 0;
}

int32_t FakeAudioDeviceModule::StopPlayout()
{
    DebugL << "Stop playout" << endl;
    // bool start = false;
    {
        rtc::CritScope cs(&_crit);
        _playing = false;
        // start = shouldStartProcessing();
    }
    // updateProcessing(start);
    return 0;
}

bool FakeAudioDeviceModule::Playing() const
{
    rtc::CritScope cs(&_crit);
    return _playing;
}

int32_t FakeAudioDeviceModule::StartRecording()
{
    DebugL << "Start recording" << endl;
    if (!_recIsInitialized) {
        return -1;
    }
    {
        rtc::CritScope cs(&_crit);
        _recording = true;
    }
    // bool start = true;
    // updateProcessing(start);
    return 0;
}

int32_t FakeAudioDeviceModule::StopRecording()
{
    DebugL << "Stop recording" << endl;
    // bool start = false;
    {
        rtc::CritScope cs(&_crit);
        _recording = false;
        // start = shouldStartProcessing();
    }
    // updateProcessing(start);
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


} // namespace scy


/// @\}
