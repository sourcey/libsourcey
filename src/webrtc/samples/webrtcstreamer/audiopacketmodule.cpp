/*
 *  Copyright 2012 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include "audiopacketmodule.h"

#include "webrtc/base/common.h"
#include "webrtc/base/refcount.h"
#include "webrtc/base/thread.h"
#include "webrtc/base/timeutils.h"

#include "scy/logger.h"
using namespace scy;

// Audio sample value that is high enough that it doesn't occur naturally when
// frames are being faked. E.g. NetEq will not generate this large sample value
// unless it has received an audio frame containing a sample of this value.
// Even simpler buffers would likely just contain audio sample values of 0.
static const int kHighSampleValue = 10000;

// Same value as src/modules/audio_device/main/source/audio_device_config.h in
// https://code.google.com/p/webrtc/
static const int kAdmMaxIdleTimeProcess = 1000;

// Constants here are derived by running VoE using a real ADM.
// The constants correspond to 10ms of mono audio at 44kHz.
static const int kTimePerFrameMs = 10;
static const uint8_t kNumberOfChannels = 1;
static const int kSamplesPerSecond = 44000;
static const int kTotalDelayMs = 0;
static const int kClockDriftMs = 0;
static const uint32_t kMaxVolume = 14392;

enum {
  MSG_START_PROCESS,
  MSG_RUN_PROCESS,
};

AudioPacketModule::AudioPacketModule()
    : last_process_time_ms_(0),
      audio_callback_(nullptr),
      recording_(false),
      playing_(false),
      play_is_initialized_(false),
      rec_is_initialized_(false),
      current_mic_level_(kMaxVolume),
      started_(false),
      next_frame_time_(0),
      frames_received_(0),
      _emitter(nullptr) {
}

AudioPacketModule::~AudioPacketModule() {
  if (process_thread_) {
    process_thread_->Stop();
  }
}

rtc::scoped_refptr<AudioPacketModule> AudioPacketModule::Create() {
  rtc::scoped_refptr<AudioPacketModule> capture_module(
      new rtc::RefCountedObject<AudioPacketModule>());
  if (!capture_module->Initialize()) {
    return nullptr;
  }
  return capture_module;
}

int AudioPacketModule::frames_received() const {
  rtc::CritScope cs(&crit_);
  return frames_received_;
}

void AudioPacketModule::setPacketSignal(scy::PacketSignal* emitter) {
  rtc::CritScope cs(&crit_);
  if (_emitter)
      _emitter->detach(packetDelegate(this, &AudioPacketModule::onAudioCaptured));
  _emitter = emitter;
  _emitter->attach(packetDelegate(this, &AudioPacketModule::onAudioCaptured));
}


void AudioPacketModule::onAudioCaptured(void* sender, av::AudioPacket& packet) {
  // assert(0);
  // void AudioPacketModule::SendFrameP() {
  InfoL << "AudioPacketModule::onAudioCaptured()" << std::endl;

  // ASSERT(process_thread_->IsCurrent());
  rtc::CritScope cs(&crit_callback_);
  if (!audio_callback_) {
    return;
  }
  bool key_pressed = false;
  uint32_t current_mic_level = 0;
  MicrophoneVolume(&current_mic_level);
  // if (audio_callback_->RecordedDataIsAvailable(send_buffer_, kNumberSamples,
  //                                             kNumberBytesPerSample,
  //                                             kNumberOfChannels,
  //                                             kSamplesPerSecond, kTotalDelayMs,
  //                                             kClockDriftMs, current_mic_level,
  //                                             key_pressed,
  //                                             current_mic_level) != 0) {


  TraceL << "^^^^^^^^^^^^^^^^^ On audio frame captured: " << packet.numSamples << std::endl;

  // #
  // # Fatal error in ../../webrtc/common_audio/resampler/push_resampler.cc, line 52
  // # last system error: 0
  // # Check failed: src_length == src_size_10ms (1024 vs. 440)
  // #

  if (audio_callback_->RecordedDataIsAvailable(packet.data(), packet.numSamples, //kNumberSamples, //
                                              kNumberBytesPerSample,
                                              kNumberOfChannels, //2, //
                                              kSamplesPerSecond,
                                              kTotalDelayMs,
                                              kClockDriftMs,
                                              current_mic_level,
                                              key_pressed,
                                              current_mic_level) != 0) {
    ASSERT(false);
  }
  SetMicrophoneVolume(current_mic_level);
}

int64_t AudioPacketModule::TimeUntilNextProcess() {
  const int64_t current_time = rtc::TimeMillis();
  if (current_time < last_process_time_ms_) {
    // TODO: wraparound could be handled more gracefully.
    return 0;
  }
  const int64_t elapsed_time = current_time - last_process_time_ms_;
  if (kAdmMaxIdleTimeProcess < elapsed_time) {
    return 0;
  }
  return kAdmMaxIdleTimeProcess - elapsed_time;
}

void AudioPacketModule::Process() {
  last_process_time_ms_ = rtc::TimeMillis();
}

int32_t AudioPacketModule::ActiveAudioLayer(
    AudioLayer* /*audio_layer*/) const {
  ASSERT(false);
  return 0;
}

webrtc::AudioDeviceModule::ErrorCode AudioPacketModule::LastError() const {
  ASSERT(false);
  return webrtc::AudioDeviceModule::kAdmErrNone;
}

int32_t AudioPacketModule::RegisterEventObserver(
    webrtc::AudioDeviceObserver* /*event_callback*/) {
  // Only used to report warnings and errors. This fake implementation won't
  // generate any so discard this callback.
  return 0;
}

int32_t AudioPacketModule::RegisterAudioCallback(
    webrtc::AudioTransport* audio_callback) {
  rtc::CritScope cs(&crit_callback_);
  audio_callback_ = audio_callback;
  return 0;
}

int32_t AudioPacketModule::Init() {
  // Initialize is called by the factory method. Safe to ignore this Init call.
  return 0;
}

int32_t AudioPacketModule::Terminate() {
  // Clean up in the destructor. No action here, just success.
  return 0;
}

bool AudioPacketModule::Initialized() const {
  ASSERT(false);
  return 0;
}

int16_t AudioPacketModule::PlayoutDevices() {
  ASSERT(false);
  return 0;
}

int16_t AudioPacketModule::RecordingDevices() {
  ASSERT(false);
  return 0;
}

int32_t AudioPacketModule::PlayoutDeviceName(
    uint16_t /*index*/,
    char /*name*/[webrtc::kAdmMaxDeviceNameSize],
    char /*guid*/[webrtc::kAdmMaxGuidSize]) {
  ASSERT(false);
  return 0;
}

int32_t AudioPacketModule::RecordingDeviceName(
    uint16_t /*index*/,
    char /*name*/[webrtc::kAdmMaxDeviceNameSize],
    char /*guid*/[webrtc::kAdmMaxGuidSize]) {
  ASSERT(false);
  return 0;
}

int32_t AudioPacketModule::SetPlayoutDevice(uint16_t /*index*/) {
  // No playout device, just playing from file. Return success.
  return 0;
}

int32_t AudioPacketModule::SetPlayoutDevice(WindowsDeviceType /*device*/) {
  if (play_is_initialized_) {
    return -1;
  }
  return 0;
}

int32_t AudioPacketModule::SetRecordingDevice(uint16_t /*index*/) {
  // No recording device, just dropping audio. Return success.
  return 0;
}

int32_t AudioPacketModule::SetRecordingDevice(
    WindowsDeviceType /*device*/) {
  if (rec_is_initialized_) {
    return -1;
  }
  return 0;
}

int32_t AudioPacketModule::PlayoutIsAvailable(bool* /*available*/) {
  ASSERT(false);
  return 0;
}

int32_t AudioPacketModule::InitPlayout() {
  play_is_initialized_ = true;
  return 0;
}

bool AudioPacketModule::PlayoutIsInitialized() const {
  return play_is_initialized_;
}

int32_t AudioPacketModule::RecordingIsAvailable(bool* /*available*/) {
  ASSERT(false);
  return 0;
}

int32_t AudioPacketModule::InitRecording() {
  rec_is_initialized_ = true;
  return 0;
}

bool AudioPacketModule::RecordingIsInitialized() const {
  return rec_is_initialized_;
}

int32_t AudioPacketModule::StartPlayout() {
  InfoL << "$$$$$$$$$$$$$$$$$$$$$$ AudioPacketModule::StartPlayout()" << std::endl;
  if (!play_is_initialized_) {
    return -1;
  }
  {
    rtc::CritScope cs(&crit_);
    playing_ = true;
  }
  bool start = true;
  UpdateProcessing(start);
  return 0;
}

int32_t AudioPacketModule::StopPlayout() {
  InfoL << "$$$$$$$$$$$$$$$$$$$$$$ AudioPacketModule::StopPlayout()" << std::endl;
  bool start = false;
  {
    rtc::CritScope cs(&crit_);
    playing_ = false;
    start = ShouldStartProcessing();
  }
  UpdateProcessing(start);
  return 0;
}

bool AudioPacketModule::Playing() const {
  rtc::CritScope cs(&crit_);
  return playing_;
}

int32_t AudioPacketModule::StartRecording() {
  InfoL << "$$$$$$$$$$$$$$$$$$$$$$ AudioPacketModule::StartRecording()" << std::endl;
  if (!rec_is_initialized_) {
    return -1;
  }
  {
    rtc::CritScope cs(&crit_);
    recording_ = true;
  }
  bool start = true;
  UpdateProcessing(start);
  return 0;
}

int32_t AudioPacketModule::StopRecording() {
  InfoL << "$$$$$$$$$$$$$$$$$$$$$$ AudioPacketModule::StopRecording()" << std::endl;
  bool start = false;
  {
    rtc::CritScope cs(&crit_);
    recording_ = false;
    start = ShouldStartProcessing();
  }
  UpdateProcessing(start);
  return 0;
}

bool AudioPacketModule::Recording() const {
  rtc::CritScope cs(&crit_);
  return recording_;
}

int32_t AudioPacketModule::SetAGC(bool /*enable*/) {
  // No AGC but not needed since audio is pregenerated. Return success.
  return 0;
}

bool AudioPacketModule::AGC() const {
  ASSERT(false);
  return 0;
}

int32_t AudioPacketModule::SetWaveOutVolume(uint16_t /*volume_left*/,
                                                 uint16_t /*volume_right*/) {
  ASSERT(false);
  return 0;
}

int32_t AudioPacketModule::WaveOutVolume(
    uint16_t* /*volume_left*/,
    uint16_t* /*volume_right*/) const {
  ASSERT(false);
  return 0;
}

int32_t AudioPacketModule::InitSpeaker() {
  // No speaker, just playing from file. Return success.
  return 0;
}

bool AudioPacketModule::SpeakerIsInitialized() const {
  ASSERT(false);
  return 0;
}

int32_t AudioPacketModule::InitMicrophone() {
  // No microphone, just playing from file. Return success.
  return 0;
}

bool AudioPacketModule::MicrophoneIsInitialized() const {
  ASSERT(false);
  return 0;
}

int32_t AudioPacketModule::SpeakerVolumeIsAvailable(bool* /*available*/) {
  ASSERT(false);
  return 0;
}

int32_t AudioPacketModule::SetSpeakerVolume(uint32_t /*volume*/) {
  ASSERT(false);
  return 0;
}

int32_t AudioPacketModule::SpeakerVolume(uint32_t* /*volume*/) const {
  ASSERT(false);
  return 0;
}

int32_t AudioPacketModule::MaxSpeakerVolume(
    uint32_t* /*max_volume*/) const {
  ASSERT(false);
  return 0;
}

int32_t AudioPacketModule::MinSpeakerVolume(
    uint32_t* /*min_volume*/) const {
  ASSERT(false);
  return 0;
}

int32_t AudioPacketModule::SpeakerVolumeStepSize(
    uint16_t* /*step_size*/) const {
  ASSERT(false);
  return 0;
}

int32_t AudioPacketModule::MicrophoneVolumeIsAvailable(
    bool* /*available*/) {
  ASSERT(false);
  return 0;
}

int32_t AudioPacketModule::SetMicrophoneVolume(uint32_t volume) {
  rtc::CritScope cs(&crit_);
  current_mic_level_ = volume;
  return 0;
}

int32_t AudioPacketModule::MicrophoneVolume(uint32_t* volume) const {
  rtc::CritScope cs(&crit_);
  *volume = current_mic_level_;
  return 0;
}

int32_t AudioPacketModule::MaxMicrophoneVolume(
    uint32_t* max_volume) const {
  *max_volume = kMaxVolume;
  return 0;
}

int32_t AudioPacketModule::MinMicrophoneVolume(
    uint32_t* /*min_volume*/) const {
  ASSERT(false);
  return 0;
}

int32_t AudioPacketModule::MicrophoneVolumeStepSize(
    uint16_t* /*step_size*/) const {
  ASSERT(false);
  return 0;
}

int32_t AudioPacketModule::SpeakerMuteIsAvailable(bool* /*available*/) {
  ASSERT(false);
  return 0;
}

int32_t AudioPacketModule::SetSpeakerMute(bool /*enable*/) {
  ASSERT(false);
  return 0;
}

int32_t AudioPacketModule::SpeakerMute(bool* /*enabled*/) const {
  ASSERT(false);
  return 0;
}

int32_t AudioPacketModule::MicrophoneMuteIsAvailable(bool* /*available*/) {
  ASSERT(false);
  return 0;
}

int32_t AudioPacketModule::SetMicrophoneMute(bool /*enable*/) {
  ASSERT(false);
  return 0;
}

int32_t AudioPacketModule::MicrophoneMute(bool* /*enabled*/) const {
  ASSERT(false);
  return 0;
}

int32_t AudioPacketModule::MicrophoneBoostIsAvailable(
    bool* /*available*/) {
  ASSERT(false);
  return 0;
}

int32_t AudioPacketModule::SetMicrophoneBoost(bool /*enable*/) {
  ASSERT(false);
  return 0;
}

int32_t AudioPacketModule::MicrophoneBoost(bool* /*enabled*/) const {
  ASSERT(false);
  return 0;
}

int32_t AudioPacketModule::StereoPlayoutIsAvailable(
    bool* available) const {
  // No recording device, just dropping audio. Stereo can be dropped just
  // as easily as mono.
  *available = true;
  return 0;
}

int32_t AudioPacketModule::SetStereoPlayout(bool /*enable*/) {
  // No recording device, just dropping audio. Stereo can be dropped just
  // as easily as mono.
  return 0;
}

int32_t AudioPacketModule::StereoPlayout(bool* /*enabled*/) const {
  ASSERT(false);
  return 0;
}

int32_t AudioPacketModule::StereoRecordingIsAvailable(
    bool* available) const {
  // Keep thing simple. No stereo recording.
  *available = false;
  return 0;
}

int32_t AudioPacketModule::SetStereoRecording(bool enable) {
  if (!enable) {
    return 0;
  }
  return -1;
}

int32_t AudioPacketModule::StereoRecording(bool* /*enabled*/) const {
  ASSERT(false);
  return 0;
}

int32_t AudioPacketModule::SetRecordingChannel(
    const ChannelType channel) {
  if (channel != AudioDeviceModule::kChannelBoth) {
    // There is no right or left in mono. I.e. kChannelBoth should be used for
    // mono.
    ASSERT(false);
    return -1;
  }
  return 0;
}

int32_t AudioPacketModule::RecordingChannel(ChannelType* channel) const {
  // Stereo recording not supported. However, WebRTC ADM returns kChannelBoth
  // in that case. Do the same here.
  *channel = AudioDeviceModule::kChannelBoth;
  return 0;
}

int32_t AudioPacketModule::SetPlayoutBuffer(const BufferType /*type*/,
                                                 uint16_t /*size_ms*/) {
  ASSERT(false);
  return 0;
}

int32_t AudioPacketModule::PlayoutBuffer(BufferType* /*type*/,
                                              uint16_t* /*size_ms*/) const {
  ASSERT(false);
  return 0;
}

int32_t AudioPacketModule::PlayoutDelay(uint16_t* delay_ms) const {
  // No delay since audio frames are dropped.
  *delay_ms = 0;
  return 0;
}

int32_t AudioPacketModule::RecordingDelay(uint16_t* /*delay_ms*/) const {
  ASSERT(false);
  return 0;
}

int32_t AudioPacketModule::CPULoad(uint16_t* /*load*/) const {
  ASSERT(false);
  return 0;
}

int32_t AudioPacketModule::StartRawOutputFileRecording(
    const char /*pcm_file_name_utf8*/[webrtc::kAdmMaxFileNameSize]) {
  ASSERT(false);
  return 0;
}

int32_t AudioPacketModule::StopRawOutputFileRecording() {
  ASSERT(false);
  return 0;
}

int32_t AudioPacketModule::StartRawInputFileRecording(
    const char /*pcm_file_name_utf8*/[webrtc::kAdmMaxFileNameSize]) {
  ASSERT(false);
  return 0;
}

int32_t AudioPacketModule::StopRawInputFileRecording() {
  ASSERT(false);
  return 0;
}

int32_t AudioPacketModule::SetRecordingSampleRate(
    const uint32_t /*samples_per_sec*/) {
  ASSERT(false);
  return 0;
}

int32_t AudioPacketModule::RecordingSampleRate(
    uint32_t* /*samples_per_sec*/) const {
  ASSERT(false);
  return 0;
}

int32_t AudioPacketModule::SetPlayoutSampleRate(
    const uint32_t /*samples_per_sec*/) {
  ASSERT(false);
  return 0;
}

int32_t AudioPacketModule::PlayoutSampleRate(
    uint32_t* /*samples_per_sec*/) const {
  ASSERT(false);
  return 0;
}

int32_t AudioPacketModule::ResetAudioDevice() {
  ASSERT(false);
  return 0;
}

int32_t AudioPacketModule::SetLoudspeakerStatus(bool /*enable*/) {
  ASSERT(false);
  return 0;
}

int32_t AudioPacketModule::GetLoudspeakerStatus(bool* /*enabled*/) const {
  ASSERT(false);
  return 0;
}

void AudioPacketModule::OnMessage(rtc::Message* msg) {
  switch (msg->message_id) {
    case MSG_START_PROCESS:
      StartProcessP();
      break;
    case MSG_RUN_PROCESS:
      ProcessFrameP();
      break;
    default:
      // All existing messages should be caught. Getting here should never
      // happen.
      ASSERT(false);
  }
}

bool AudioPacketModule::Initialize() {
  // Set the send buffer samples high enough that it would not occur on the
  // remote side unless a packet containing a sample of that magnitude has been
  // sent to it. Note that the audio processing pipeline will likely distort the
  // original signal.
  SetSendBuffer(kHighSampleValue);
  last_process_time_ms_ = rtc::TimeMillis();
  return true;
}

void AudioPacketModule::SetSendBuffer(int value) {
  Sample* buffer_ptr = reinterpret_cast<Sample*>(send_buffer_);
  const size_t buffer_size_in_samples =
      sizeof(send_buffer_) / kNumberBytesPerSample;
  for (size_t i = 0; i < buffer_size_in_samples; ++i) {
    buffer_ptr[i] = value;
  }
}

void AudioPacketModule::ResetRecBuffer() {
  memset(rec_buffer_, 0, sizeof(rec_buffer_));
}

bool AudioPacketModule::CheckRecBuffer(int value) {
  const Sample* buffer_ptr = reinterpret_cast<const Sample*>(rec_buffer_);
  const size_t buffer_size_in_samples =
      sizeof(rec_buffer_) / kNumberBytesPerSample;
  for (size_t i = 0; i < buffer_size_in_samples; ++i) {
    if (buffer_ptr[i] >= value) return true;
  }
  return false;
}

bool AudioPacketModule::ShouldStartProcessing() {
  return recording_ || playing_;
}

void AudioPacketModule::UpdateProcessing(bool start) {
  if (start) {
    if (!process_thread_) {
      process_thread_.reset(new rtc::Thread());
      process_thread_->Start();
    }
    process_thread_->Post(RTC_FROM_HERE, this, MSG_START_PROCESS);
  } else {
    if (process_thread_) {
      process_thread_->Stop();
      process_thread_.reset(nullptr);
    }
    started_ = false;
  }
}

void AudioPacketModule::StartProcessP() {
  ASSERT(process_thread_->IsCurrent());
  if (started_) {
    // Already started.
    return;
  }
  ProcessFrameP();
}

void AudioPacketModule::ProcessFrameP() {
  ASSERT(process_thread_->IsCurrent());
  if (!started_) {
    next_frame_time_ = rtc::TimeMillis();
    started_ = true;
  }

  {
    rtc::CritScope cs(&crit_);
    // Receive and send frames every kTimePerFrameMs.
    if (playing_) {
      ReceiveFrameP();
    }
    if (recording_) {
      SendFrameP();
    }
  }

  // InfoL << "AudioPacketModule::ProcessFrameP()" << std::endl;

  next_frame_time_ += kTimePerFrameMs;
  const int64_t current_time = rtc::TimeMillis();
  const int64_t wait_time =
      (next_frame_time_ > current_time) ? next_frame_time_ - current_time : 0;
  process_thread_->PostDelayed(RTC_FROM_HERE, wait_time, this, MSG_RUN_PROCESS);
}

void AudioPacketModule::ReceiveFrameP() {
  ASSERT(process_thread_->IsCurrent());
  {
    rtc::CritScope cs(&crit_callback_);
    if (!audio_callback_) {
      return;
    }
    ResetRecBuffer();
    size_t nSamplesOut = 0;
    int64_t elapsed_time_ms = 0;
    int64_t ntp_time_ms = 0;
    if (audio_callback_->NeedMorePlayData(kNumberSamples, kNumberBytesPerSample,
                                         kNumberOfChannels, kSamplesPerSecond,
                                         rec_buffer_, nSamplesOut,
                                         &elapsed_time_ms, &ntp_time_ms) != 0) {
      ASSERT(false);
    }
    ASSERT(nSamplesOut == kNumberSamples);
  }

  // InfoL << "AudioPacketModule::ReceiveFrameP()" << std::endl;

  // The SetBuffer() function ensures that after decoding, the audio buffer
  // should contain samples of similar magnitude (there is likely to be some
  // distortion due to the audio pipeline). If one sample is detected to
  // have the same or greater magnitude somewhere in the frame, an actual frame
  // has been received from the remote side (i.e. faked frames are not being
  // pulled).
  if (CheckRecBuffer(kHighSampleValue)) {
    rtc::CritScope cs(&crit_);
    ++frames_received_;
  }
}

void AudioPacketModule::SendFrameP() {
  // InfoL << "AudioPacketModule::SendFrameP()" << std::endl;

  // ASSERT(process_thread_->IsCurrent());
  // rtc::CritScope cs(&crit_callback_);
  // if (!audio_callback_) {
  //   return;
  // }
  // bool key_pressed = false;
  // uint32_t current_mic_level = 0;
  // MicrophoneVolume(&current_mic_level);
  // if (audio_callback_->RecordedDataIsAvailable(send_buffer_, kNumberSamples,
  //                                             kNumberBytesPerSample,
  //                                             kNumberOfChannels,
  //                                             kSamplesPerSecond, kTotalDelayMs,
  //                                             kClockDriftMs, current_mic_level,
  //                                             key_pressed,
  //                                             current_mic_level) != 0) {
  //   ASSERT(false);
  // }
  // SetMicrophoneVolume(current_mic_level);
}
