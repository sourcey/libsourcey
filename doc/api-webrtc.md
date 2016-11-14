# Module <!-- group --> `webrtc`

The `webrtc` module contains WebRTC integrations.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`namespace `[`scy`](#namespacescy)    | 
# namespace `scy` {#namespacescy}



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[`scy::AudioPacketModule`](#classscy_1_1AudioPacketModule)    | 
`class `[`scy::DummySetSessionDescriptionObserver`](#classscy_1_1DummySetSessionDescriptionObserver)    | 
`class `[`scy::ImageSequenceRecorder`](#classscy_1_1ImageSequenceRecorder)    | 
`class `[`scy::MultiplexMediaCapturer`](#classscy_1_1MultiplexMediaCapturer)    | 
`class `[`scy::PeerConnection`](#classscy_1_1PeerConnection)    | 
`class `[`scy::PeerConnectionManager`](#classscy_1_1PeerConnectionManager)    | 
`class `[`scy::StreamRecorder`](#classscy_1_1StreamRecorder)    | 
`class `[`scy::VideoPacketSource`](#classscy_1_1VideoPacketSource)    | 
# class `scy::AudioPacketModule` {#classscy_1_1AudioPacketModule}

```
class scy::AudioPacketModule
  : public AudioDeviceModule
  : public MessageHandler
```  



This class implements an `AudioDeviceModule` that can be used to detect if audio is being received properly if it is fed by another `AudioDeviceModule` in some arbitrary audio pipeline where they are connected. It does not play out or record any audio so it does not need access to any hardware and can therefore be used in the gtest testing framework.

Note P postfix of a function indicates that it should only be called by the processing thread.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public void onAudioCaptured(void * sender,`[`av::AudioPacket`](#structscy_1_1av_1_1AudioPacket)` & packet)` | Handles input packets from the capture for sending.
`public int64_t TimeUntilNextProcess()` | 
`public void Process()` | 
`public int32_t ActiveAudioLayer(AudioLayer * audio_layer) const` | 
`public ErrorCode LastError() const` | 
`public int32_t RegisterEventObserver(webrtc::AudioDeviceObserver * event_callback)` | 
`public int32_t RegisterAudioCallback(webrtc::AudioTransport * audio_callback)` | Note: Calling this method from a callback may result in deadlock.
`public int32_t Init()` | 
`public int32_t Terminate()` | 
`public bool Initialized() const` | 
`public int16_t PlayoutDevices()` | 
`public int16_t RecordingDevices()` | 
`public int32_t PlayoutDeviceName(uint16_t index,char name,char guid)` | 
`public int32_t RecordingDeviceName(uint16_t index,char name,char guid)` | 
`public int32_t SetPlayoutDevice(uint16_t index)` | 
`public int32_t SetPlayoutDevice(WindowsDeviceType device)` | 
`public int32_t SetRecordingDevice(uint16_t index)` | 
`public int32_t SetRecordingDevice(WindowsDeviceType device)` | 
`public int32_t PlayoutIsAvailable(bool * available)` | 
`public int32_t InitPlayout()` | 
`public bool PlayoutIsInitialized() const` | 
`public int32_t RecordingIsAvailable(bool * available)` | 
`public int32_t InitRecording()` | 
`public bool RecordingIsInitialized() const` | 
`public int32_t StartPlayout()` | 
`public int32_t StopPlayout()` | 
`public bool Playing() const` | 
`public int32_t StartRecording()` | 
`public int32_t StopRecording()` | 
`public bool Recording() const` | 
`public int32_t SetAGC(bool enable)` | 
`public bool AGC() const` | 
`public int32_t SetWaveOutVolume(uint16_t volume_left,uint16_t volume_right)` | 
`public int32_t WaveOutVolume(uint16_t * volume_left,uint16_t * volume_right) const` | 
`public int32_t InitSpeaker()` | 
`public bool SpeakerIsInitialized() const` | 
`public int32_t InitMicrophone()` | 
`public bool MicrophoneIsInitialized() const` | 
`public int32_t SpeakerVolumeIsAvailable(bool * available)` | 
`public int32_t SetSpeakerVolume(uint32_t volume)` | 
`public int32_t SpeakerVolume(uint32_t * volume) const` | 
`public int32_t MaxSpeakerVolume(uint32_t * max_volume) const` | 
`public int32_t MinSpeakerVolume(uint32_t * min_volume) const` | 
`public int32_t SpeakerVolumeStepSize(uint16_t * step_size) const` | 
`public int32_t MicrophoneVolumeIsAvailable(bool * available)` | 
`public int32_t SetMicrophoneVolume(uint32_t volume)` | 
`public int32_t MicrophoneVolume(uint32_t * volume) const` | 
`public int32_t MaxMicrophoneVolume(uint32_t * max_volume) const` | 
`public int32_t MinMicrophoneVolume(uint32_t * min_volume) const` | 
`public int32_t MicrophoneVolumeStepSize(uint16_t * step_size) const` | 
`public int32_t SpeakerMuteIsAvailable(bool * available)` | 
`public int32_t SetSpeakerMute(bool enable)` | 
`public int32_t SpeakerMute(bool * enabled) const` | 
`public int32_t MicrophoneMuteIsAvailable(bool * available)` | 
`public int32_t SetMicrophoneMute(bool enable)` | 
`public int32_t MicrophoneMute(bool * enabled) const` | 
`public int32_t MicrophoneBoostIsAvailable(bool * available)` | 
`public int32_t SetMicrophoneBoost(bool enable)` | 
`public int32_t MicrophoneBoost(bool * enabled) const` | 
`public int32_t StereoPlayoutIsAvailable(bool * available) const` | 
`public int32_t SetStereoPlayout(bool enable)` | 
`public int32_t StereoPlayout(bool * enabled) const` | 
`public int32_t StereoRecordingIsAvailable(bool * available) const` | 
`public int32_t SetStereoRecording(bool enable)` | 
`public int32_t StereoRecording(bool * enabled) const` | 
`public int32_t SetRecordingChannel(const ChannelType channel)` | 
`public int32_t RecordingChannel(ChannelType * channel) const` | 
`public int32_t SetPlayoutBuffer(const BufferType type,uint16_t size_ms)` | 
`public int32_t PlayoutBuffer(BufferType * type,uint16_t * size_ms) const` | 
`public int32_t PlayoutDelay(uint16_t * delay_ms) const` | 
`public int32_t RecordingDelay(uint16_t * delay_ms) const` | 
`public int32_t CPULoad(uint16_t * load) const` | 
`public int32_t StartRawOutputFileRecording(const char pcm_file_name_utf8)` | 
`public int32_t StopRawOutputFileRecording()` | 
`public int32_t StartRawInputFileRecording(const char pcm_file_name_utf8)` | 
`public int32_t StopRawInputFileRecording()` | 
`public int32_t SetRecordingSampleRate(const uint32_t samples_per_sec)` | 
`public int32_t RecordingSampleRate(uint32_t * samples_per_sec) const` | 
`public int32_t SetPlayoutSampleRate(const uint32_t samples_per_sec)` | 
`public int32_t PlayoutSampleRate(uint32_t * samples_per_sec) const` | 
`public int32_t ResetAudioDevice()` | 
`public int32_t SetLoudspeakerStatus(bool enable)` | 
`public int32_t GetLoudspeakerStatus(bool * enabled) const` | 
`public inline bool BuiltInAECIsAvailable() const` | 
`public inline int32_t EnableBuiltInAEC(bool enable)` | 
`public inline bool BuiltInAGCIsAvailable() const` | 
`public inline int32_t EnableBuiltInAGC(bool enable)` | 
`public inline bool BuiltInNSIsAvailable() const` | 
`public inline int32_t EnableBuiltInNS(bool enable)` | 
`public void OnMessage(rtc::Message * msg)` | WEBRTC_IOS.
`protected  explicit AudioPacketModule()` | 
`protected virtual  ~AudioPacketModule()` | 

## Members

#### `public void onAudioCaptured(void * sender,`[`av::AudioPacket`](#structscy_1_1av_1_1AudioPacket)` & packet)` {#group__webrtc_1ga8c63541c4bc64a1bc0f577b826a56b50}

Handles input packets from the capture for sending.



#### `public int64_t TimeUntilNextProcess()` {#group__webrtc_1gaef67d455a076ef1c96d7e258be75c332}



Following functions are inherited from `webrtc::AudioDeviceModule`. Only functions called by `[PeerConnection](#classscy_1_1PeerConnection)` are implemented, the rest do nothing and return success. If a function is not expected to be called by `[PeerConnection](#classscy_1_1PeerConnection)` an assertion is triggered if it is in fact called.

#### `public void Process()` {#group__webrtc_1ga8662a1214cde3170c320ac0fbc9dea0e}





#### `public int32_t ActiveAudioLayer(AudioLayer * audio_layer) const` {#group__webrtc_1gaf67b88ff612aff8059d2bcf202b1dc7f}





#### `public ErrorCode LastError() const` {#group__webrtc_1gafbb638de3b9ceadd770cce3330d9b5e6}





#### `public int32_t RegisterEventObserver(webrtc::AudioDeviceObserver * event_callback)` {#group__webrtc_1ga3305bf21fd950cfef04a5591eedb3bee}





#### `public int32_t RegisterAudioCallback(webrtc::AudioTransport * audio_callback)` {#group__webrtc_1ga086e8993e93611de5116506e6b3c0b04}

Note: Calling this method from a callback may result in deadlock.



#### `public int32_t Init()` {#group__webrtc_1gae9115c5408cc2262d0620d8050f3db51}





#### `public int32_t Terminate()` {#group__webrtc_1ga0f71e79f211d6d1b771b8d53986cfe3f}





#### `public bool Initialized() const` {#group__webrtc_1ga2da04c191879b68ed10d59cc96066564}





#### `public int16_t PlayoutDevices()` {#group__webrtc_1ga70dd8889eb5b6903eb6d46d0cb93632c}





#### `public int16_t RecordingDevices()` {#group__webrtc_1ga06ea3de15953d2b3dd6ae90dd5a7a26b}





#### `public int32_t PlayoutDeviceName(uint16_t index,char name,char guid)` {#group__webrtc_1ga311ffddb4ed0edcc5f24afb450d46045}





#### `public int32_t RecordingDeviceName(uint16_t index,char name,char guid)` {#group__webrtc_1gac74110e7b20a12b398197c3fcbb1a37e}





#### `public int32_t SetPlayoutDevice(uint16_t index)` {#group__webrtc_1ga5d7e571348827a723f9e6a5e7aad3251}





#### `public int32_t SetPlayoutDevice(WindowsDeviceType device)` {#group__webrtc_1ga749797d718c3913a39f289631b29134f}





#### `public int32_t SetRecordingDevice(uint16_t index)` {#group__webrtc_1gadb26d2747690ed796cd2e45a3520371e}





#### `public int32_t SetRecordingDevice(WindowsDeviceType device)` {#group__webrtc_1ga2ee51f4209bd888631f3628694f912da}





#### `public int32_t PlayoutIsAvailable(bool * available)` {#group__webrtc_1ga677e5d89eefbb2be5350a744fe96398b}





#### `public int32_t InitPlayout()` {#group__webrtc_1gaa4f188f7ec1e77301d362e75d8c01652}





#### `public bool PlayoutIsInitialized() const` {#group__webrtc_1gadd2341c5aa403dec53356f6d86327f4c}





#### `public int32_t RecordingIsAvailable(bool * available)` {#group__webrtc_1gac74ae637ce047701289f9acd749b2f8d}





#### `public int32_t InitRecording()` {#group__webrtc_1ga7a5cc72a7c7d8d894f2e2b2d22fbf911}





#### `public bool RecordingIsInitialized() const` {#group__webrtc_1ga6951f8e572dd868806807cacb8d9ab5b}





#### `public int32_t StartPlayout()` {#group__webrtc_1gaa30bf69c6fde2fbdd527a04cf536d85b}





#### `public int32_t StopPlayout()` {#group__webrtc_1ga73624d2600485b5738f65fcdb86ba64b}





#### `public bool Playing() const` {#group__webrtc_1ga2752be22779b4f3d745044e6dd97ccf0}





#### `public int32_t StartRecording()` {#group__webrtc_1ga896c89ef87a27ba2acb1c6ec1eeaba8a}





#### `public int32_t StopRecording()` {#group__webrtc_1gacf89d05966828c1bdd53ceb6fa324064}





#### `public bool Recording() const` {#group__webrtc_1ga67534d4fe3ef93cbddb428df4d3f3e87}





#### `public int32_t SetAGC(bool enable)` {#group__webrtc_1ga9e86fc6d70f454582230b31772eb5b77}





#### `public bool AGC() const` {#group__webrtc_1gafcb9962d72ac53eb1ea1649b138a5897}





#### `public int32_t SetWaveOutVolume(uint16_t volume_left,uint16_t volume_right)` {#group__webrtc_1ga9c96dc72e7605379309ce52b67e84150}





#### `public int32_t WaveOutVolume(uint16_t * volume_left,uint16_t * volume_right) const` {#group__webrtc_1ga3c64abfc0799a5b2dc4cc0fb69767100}





#### `public int32_t InitSpeaker()` {#group__webrtc_1ga00a546d8934f7613b3fdd330789f8831}





#### `public bool SpeakerIsInitialized() const` {#group__webrtc_1ga413176256c6defc70178629ae5ba2a21}





#### `public int32_t InitMicrophone()` {#group__webrtc_1gaef3893479e67456257aad80179120783}





#### `public bool MicrophoneIsInitialized() const` {#group__webrtc_1ga15a63ba4dc88b57389c51dfe9345dbe6}





#### `public int32_t SpeakerVolumeIsAvailable(bool * available)` {#group__webrtc_1ga9a56ab014d84843ec3cc4c5ac9cc9648}





#### `public int32_t SetSpeakerVolume(uint32_t volume)` {#group__webrtc_1ga879ea5fd7d9424798a06c4f5d2924957}





#### `public int32_t SpeakerVolume(uint32_t * volume) const` {#group__webrtc_1ga5a5dfdf4ea4af03802a206ab6f2a1a48}





#### `public int32_t MaxSpeakerVolume(uint32_t * max_volume) const` {#group__webrtc_1ga3056ea233350dd54f426dd8e5df23fcf}





#### `public int32_t MinSpeakerVolume(uint32_t * min_volume) const` {#group__webrtc_1gac1c0500765d44de6b597ee08e557792d}





#### `public int32_t SpeakerVolumeStepSize(uint16_t * step_size) const` {#group__webrtc_1ga1795f9a14cc2358d4ad3a200e0f4aee2}





#### `public int32_t MicrophoneVolumeIsAvailable(bool * available)` {#group__webrtc_1ga9996d6b9e497ecd262a87740314bc8c4}





#### `public int32_t SetMicrophoneVolume(uint32_t volume)` {#group__webrtc_1ga20f79fc86f92f4318dc45296fea8e393}





#### `public int32_t MicrophoneVolume(uint32_t * volume) const` {#group__webrtc_1ga5d18d40e94c92a727691542b496ae30b}





#### `public int32_t MaxMicrophoneVolume(uint32_t * max_volume) const` {#group__webrtc_1ga8e237b3927e31b0f1786b18d0ce3c7eb}





#### `public int32_t MinMicrophoneVolume(uint32_t * min_volume) const` {#group__webrtc_1gabc43c56f3778c68315b66bbada8b34f4}





#### `public int32_t MicrophoneVolumeStepSize(uint16_t * step_size) const` {#group__webrtc_1gaf905e2f519baecabc68d133fe6ebe48b}





#### `public int32_t SpeakerMuteIsAvailable(bool * available)` {#group__webrtc_1ga63ef268c3d7cfc9101dd7bf2d595f2d1}





#### `public int32_t SetSpeakerMute(bool enable)` {#group__webrtc_1ga1e007603a093dbfc4b072e13c0d76ad5}





#### `public int32_t SpeakerMute(bool * enabled) const` {#group__webrtc_1ga6d351b0b9d5560f7f211e100c35862f2}





#### `public int32_t MicrophoneMuteIsAvailable(bool * available)` {#group__webrtc_1ga4889da98e98dfd3b2300df544c7cb1e4}





#### `public int32_t SetMicrophoneMute(bool enable)` {#group__webrtc_1ga8e66ed4ca50e76343fea4562ebea3bc0}





#### `public int32_t MicrophoneMute(bool * enabled) const` {#group__webrtc_1ga80c072e6c4c12f9d9af5a10d50f8da9b}





#### `public int32_t MicrophoneBoostIsAvailable(bool * available)` {#group__webrtc_1gaca47fda7ac25095704a1079e37d6a55a}





#### `public int32_t SetMicrophoneBoost(bool enable)` {#group__webrtc_1gaa50ae109823d6add661455108fe2b44d}





#### `public int32_t MicrophoneBoost(bool * enabled) const` {#group__webrtc_1gafcd2fb5819fab1c84332a5f9a149c774}





#### `public int32_t StereoPlayoutIsAvailable(bool * available) const` {#group__webrtc_1gaebcd86319e21a29df1c3a39ea78ecbd5}





#### `public int32_t SetStereoPlayout(bool enable)` {#group__webrtc_1ga651cb4d384935280b7fde225619c0418}





#### `public int32_t StereoPlayout(bool * enabled) const` {#group__webrtc_1gac65243946c8f407b29c06b5739b3dca8}





#### `public int32_t StereoRecordingIsAvailable(bool * available) const` {#group__webrtc_1ga718e055d7cc8f81cee6fb4ca47734596}





#### `public int32_t SetStereoRecording(bool enable)` {#group__webrtc_1ga0073e13498b4fad0f2ad7d61530140c5}





#### `public int32_t StereoRecording(bool * enabled) const` {#group__webrtc_1ga60fd5c45168c2211ef4160f169248c81}





#### `public int32_t SetRecordingChannel(const ChannelType channel)` {#group__webrtc_1ga9f186fe786b8dd5f6e8bdcd74ccf2fbd}





#### `public int32_t RecordingChannel(ChannelType * channel) const` {#group__webrtc_1ga423b128060618ec32e9b6f92b8403428}





#### `public int32_t SetPlayoutBuffer(const BufferType type,uint16_t size_ms)` {#group__webrtc_1gab44a210cedb18486fe0d0e715cf29c43}





#### `public int32_t PlayoutBuffer(BufferType * type,uint16_t * size_ms) const` {#group__webrtc_1gae9fd10d5130e8b696b9634e23100150f}





#### `public int32_t PlayoutDelay(uint16_t * delay_ms) const` {#group__webrtc_1gacfbb5f1036a34fa4e64884593abbbeb3}





#### `public int32_t RecordingDelay(uint16_t * delay_ms) const` {#group__webrtc_1ga73fff08c1db8a34042b636769bd94033}





#### `public int32_t CPULoad(uint16_t * load) const` {#group__webrtc_1ga7b3b2d92900e98ee0d6e7dfad1dccd6d}





#### `public int32_t StartRawOutputFileRecording(const char pcm_file_name_utf8)` {#group__webrtc_1gaf29b20cac56ce74bed82ad73af6aeb70}





#### `public int32_t StopRawOutputFileRecording()` {#group__webrtc_1ga4f5491642b40b5c00d692d4d3dd0b706}





#### `public int32_t StartRawInputFileRecording(const char pcm_file_name_utf8)` {#group__webrtc_1ga2baedbd6524c481a0a5931f44595a5b5}





#### `public int32_t StopRawInputFileRecording()` {#group__webrtc_1gab69481da491ad8f623b29cfa6eac2e88}





#### `public int32_t SetRecordingSampleRate(const uint32_t samples_per_sec)` {#group__webrtc_1ga942ab902d8a54813f7a6cfb1a72ad40c}





#### `public int32_t RecordingSampleRate(uint32_t * samples_per_sec) const` {#group__webrtc_1ga63707bd31a0950e8a19e173eba13e937}





#### `public int32_t SetPlayoutSampleRate(const uint32_t samples_per_sec)` {#group__webrtc_1gac5190ab65dc28c9ebb6764f01626e1d0}





#### `public int32_t PlayoutSampleRate(uint32_t * samples_per_sec) const` {#group__webrtc_1gabbcd7d964006f067daa3e5183931a63a}





#### `public int32_t ResetAudioDevice()` {#group__webrtc_1ga7e5afe830678a79bf5694498725a4325}





#### `public int32_t SetLoudspeakerStatus(bool enable)` {#group__webrtc_1gae46350f7694b63da7c417bdf0d6858a1}





#### `public int32_t GetLoudspeakerStatus(bool * enabled) const` {#group__webrtc_1ga72bd4896c13ee18d758ea99f2422cdf4}





#### `public inline bool BuiltInAECIsAvailable() const` {#group__webrtc_1ga1b979dc994e18db877edfdac6f6dcb1d}





#### `public inline int32_t EnableBuiltInAEC(bool enable)` {#group__webrtc_1ga60fbc59eaf09b843532f8d2438ef0992}





#### `public inline bool BuiltInAGCIsAvailable() const` {#group__webrtc_1gad71779aa595c550e184b3a2fecc0bc39}





#### `public inline int32_t EnableBuiltInAGC(bool enable)` {#group__webrtc_1ga2e5d5f289e3f6793b1c54da4a5f10bdf}





#### `public inline bool BuiltInNSIsAvailable() const` {#group__webrtc_1ga5298c18e8bcf1193880b75fe34c36886}





#### `public inline int32_t EnableBuiltInNS(bool enable)` {#group__webrtc_1ga39f613e200d2e243e753777d291fa2c4}





#### `public void OnMessage(rtc::Message * msg)` {#group__webrtc_1ga89431cdd51a84f9a12ee1aa5bf519d25}

WEBRTC_IOS.

End of functions inherited from `webrtc::AudioDeviceModule`. The following function is inherited from `rtc::MessageHandler`.

#### `protected  explicit AudioPacketModule()` {#group__webrtc_1ga03fa417954eab2b4a4a12a99712159b5}



The constructor is protected because the class needs to be created as a reference counted object (for memory managment reasons). It could be exposed in which case the burden of proper instantiation would be put on the creator of a [AudioPacketModule](#classscy_1_1AudioPacketModule) instance. To create an instance of this class use the `[Create()](#group__webrtc_1gaf78f45016b72c9bdca424e9a31691db8)` API.

#### `protected virtual  ~AudioPacketModule()` {#group__webrtc_1ga9a9d444142b0d87ade45a21768cf869e}



The destructor is protected because it is reference counted and should not be deleted directly.

# class `scy::DummySetSessionDescriptionObserver` {#classscy_1_1DummySetSessionDescriptionObserver}

```
class scy::DummySetSessionDescriptionObserver
  : public SetSessionDescriptionObserver
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public virtual void OnSuccess()` | 
`public virtual void OnFailure(const std::string & error)` | 
`protected inline  DummySetSessionDescriptionObserver()` | 
`protected inline  ~DummySetSessionDescriptionObserver()` | 

## Members

#### `public virtual void OnSuccess()` {#group__webrtc_1ga03d81703729edb76c58d5c200c3e2ca8}





#### `public virtual void OnFailure(const std::string & error)` {#group__webrtc_1gaa4a4a81505fc980817007607afd50390}





#### `protected inline  DummySetSessionDescriptionObserver()` {#group__webrtc_1gab18a06bf30eba18fe85725eb3f1b8998}





#### `protected inline  ~DummySetSessionDescriptionObserver()` {#group__webrtc_1ga6b37fc222fbd47bb6955822a5af54996}





# class `scy::ImageSequenceRecorder` {#classscy_1_1ImageSequenceRecorder}

```
class scy::ImageSequenceRecorder
  : public rtc::VideoSinkInterface< cricket::VideoFrame >
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  ImageSequenceRecorder(webrtc::VideoTrackInterface * track_to_render,const std::string & basename)` | 
`public virtual  ~ImageSequenceRecorder()` | 
`public std::string getNextFilename()` | 
`public void OnFrame(const cricket::VideoFrame & frame)` | VideoSinkInterface implementation.
`protected rtc::scoped_refptr< webrtc::VideoTrackInterface > _renderedTrack` | 
`protected const std::string _basename` | 
`protected size_t _count` | 
`protected int _width` | 
`protected int _height` | 
`protected av::VideoEncoder _encoder` | 
`protected AVFrame * _avframe` | 

## Members

#### `public  ImageSequenceRecorder(webrtc::VideoTrackInterface * track_to_render,const std::string & basename)` {#group__webrtc_1ga4d6630bef2650e6ae295e82f362df75d}





#### `public virtual  ~ImageSequenceRecorder()` {#group__webrtc_1ga13cc53ee778ddc0b7f54166f19eee6e2}





#### `public std::string getNextFilename()` {#group__webrtc_1ga3a3cbca75d46e29b862f220faa118ca5}





#### `public void OnFrame(const cricket::VideoFrame & frame)` {#group__webrtc_1ga6148d90a7d179dde3539a1850b84d591}

VideoSinkInterface implementation.



#### `protected rtc::scoped_refptr< webrtc::VideoTrackInterface > _renderedTrack` {#group__webrtc_1ga89f5732344a2665d59fce765a46505f4}





#### `protected const std::string _basename` {#group__webrtc_1ga6cfd045ea36a386be612049334318d85}





#### `protected size_t _count` {#group__webrtc_1gab334f0a58f57d2b3298b4ac370301674}





#### `protected int _width` {#group__webrtc_1gab5eb6de1a4cb8ae45f6ed097aff815d9}





#### `protected int _height` {#group__webrtc_1ga78b4034cb31cbeec40fe34348706f006}





#### `protected av::VideoEncoder _encoder` {#group__webrtc_1gabdda83d70f0f005fcb981e4f738ca12e}





#### `protected AVFrame * _avframe` {#group__webrtc_1ga359ef1080d697a035e2bc99c3453eb01}





# class `scy::MultiplexMediaCapturer` {#classscy_1_1MultiplexMediaCapturer}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  MultiplexMediaCapturer()` | 
`public virtual  ~MultiplexMediaCapturer()` | 
`public virtual void openFile(const std::string & file)` | 
`public virtual void addMediaTracks(webrtc::PeerConnectionFactoryInterface * factory,webrtc::MediaStreamInterface * stream)` | 
`public virtual void start()` | 
`public virtual void stop()` | 
`public virtual rtc::scoped_refptr< `[`AudioPacketModule`](#classscy_1_1AudioPacketModule)` > getAudioModule()` | 
`public virtual `[`VideoPacketSource`](#classscy_1_1VideoPacketSource)` * createVideoSource()` | 
`protected `[`PacketStream`](api-base.md#classscy_1_1PacketStream)` _stream` | 
`protected av::MediaCapture::Ptr _capture` | 
`protected rtc::scoped_refptr< `[`AudioPacketModule`](api-webrtc.md#classscy_1_1AudioPacketModule)` > _audioModule` | 

## Members

#### `public  MultiplexMediaCapturer()` {#group__webrtc_1ga3cfb7817624cc986d985d66df7a71b2e}





#### `public virtual  ~MultiplexMediaCapturer()` {#group__webrtc_1gab79cb06dc4ee0fc936227f5ccd9dc093}





#### `public virtual void openFile(const std::string & file)` {#group__webrtc_1ga835b6ed7faf1c5112687aad5f58d22aa}





#### `public virtual void addMediaTracks(webrtc::PeerConnectionFactoryInterface * factory,webrtc::MediaStreamInterface * stream)` {#group__webrtc_1ga183b3ed974a2458ab74c0248e6c10dcd}





#### `public virtual void start()` {#group__webrtc_1ga6d5dc051ef57555104df1bc089c88f9c}





#### `public virtual void stop()` {#group__webrtc_1ga64d870f37f55569563868aae560176f1}





#### `public virtual rtc::scoped_refptr< `[`AudioPacketModule`](#classscy_1_1AudioPacketModule)` > getAudioModule()` {#group__webrtc_1ga2419d6b98844a49944cba3540c8e6b71}





#### `public virtual `[`VideoPacketSource`](#classscy_1_1VideoPacketSource)` * createVideoSource()` {#group__webrtc_1gae834e7a1444faeca7032cb5d9a3392a6}





#### `protected `[`PacketStream`](api-base.md#classscy_1_1PacketStream)` _stream` {#group__webrtc_1gac7d182c62a7274dcc483b6011910d8d1}





#### `protected av::MediaCapture::Ptr _capture` {#group__webrtc_1ga9f70e1855de69ba6d57d0593807d6056}





#### `protected rtc::scoped_refptr< `[`AudioPacketModule`](api-webrtc.md#classscy_1_1AudioPacketModule)` > _audioModule` {#group__webrtc_1ga9ac6e39ec05db58fd218ee52bd844018}





# class `scy::PeerConnection` {#classscy_1_1PeerConnection}

```
class scy::PeerConnection
  : public PeerConnectionObserver
  : public CreateSessionDescriptionObserver
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  PeerConnection(`[`PeerConnectionManager`](#classscy_1_1PeerConnectionManager)` * manager,const std::string & peerid,`[`Mode`](#group__webrtc_1ga574415d63d860395230c8d596c679c08)` mode)` | 
`public virtual  ~PeerConnection()` | 
`public virtual rtc::scoped_refptr< webrtc::MediaStreamInterface > createMediaStream()` | 
`public virtual void createConnection()` | 
`public virtual void closeConnection()` | Close the peer connection.
`public virtual void createOffer()` | 
`public virtual void recvSDP(const std::string & type,const std::string & sdp)` | Receive a remote offer or answer.
`public virtual void recvCandidate(const std::string & mid,int mlineindex,const std::string & sdp)` | Receive a remote candidate.
`public void setPeerConnectionFactory(rtc::scoped_refptr< webrtc::PeerConnectionFactoryInterface > factory)` | 
`public std::string peerid() const` | 
`public webrtc::FakeConstraints & constraints()` | 
`public webrtc::PeerConnectionFactoryInterface * factory() const` | 
`public rtc::scoped_refptr< webrtc::PeerConnectionInterface > peerConnection() const` | 
`public rtc::scoped_refptr< webrtc::MediaStreamInterface > stream() const` | 
`protected `[`PeerConnectionManager`](api-webrtc.md#classscy_1_1PeerConnectionManager)` * _manager` | 
`protected std::string _peerid` | 
`protected `[`Mode`](#group__webrtc_1ga574415d63d860395230c8d596c679c08)` _mode` | 
`protected webrtc::PeerConnectionInterface::RTCConfiguration _config` | 
`protected webrtc::FakeConstraints _constraints` | 
`protected rtc::scoped_refptr< webrtc::PeerConnectionFactoryInterface > _factory` | 
`protected rtc::scoped_refptr< webrtc::PeerConnectionInterface > _peerConnection` | 
`protected rtc::scoped_refptr< webrtc::MediaStreamInterface > _stream` | 
`protected virtual void OnAddStream(webrtc::MediaStreamInterface * stream)` | inherited from PeerConnectionObserver
`protected virtual void OnRemoveStream(webrtc::MediaStreamInterface * stream)` | 
`protected virtual void OnIceCandidate(const webrtc::IceCandidateInterface * candidate)` | 
`protected virtual void OnSignalingChange(webrtc::PeerConnectionInterface::SignalingState new_state)` | 
`protected virtual void OnIceConnectionChange(webrtc::PeerConnectionInterface::IceConnectionState new_state)` | 
`protected virtual void OnIceGatheringChange(webrtc::PeerConnectionInterface::IceGatheringState new_state)` | 
`protected virtual void OnRenegotiationNeeded()` | 
`protected virtual void OnSuccess(webrtc::SessionDescriptionInterface * desc)` | inherited from CreateSessionDescriptionObserver
`protected virtual void OnFailure(const std::string & error)` | 
`protected inline virtual int AddRef() const` | 
`protected inline virtual int Release() const` | 

## Members

#### `public  PeerConnection(`[`PeerConnectionManager`](#classscy_1_1PeerConnectionManager)` * manager,const std::string & peerid,`[`Mode`](#group__webrtc_1ga574415d63d860395230c8d596c679c08)` mode)` {#group__webrtc_1ga50cfdf9385c9767f0c433cede34bc716}





#### `public virtual  ~PeerConnection()` {#group__webrtc_1ga5334edcc012e90e951588134957e905a}





#### `public virtual rtc::scoped_refptr< webrtc::MediaStreamInterface > createMediaStream()` {#group__webrtc_1ga23efe5604b51093691433fe79d29d689}



Create the local media stream. Only necessary when we are creating the offer.

#### `public virtual void createConnection()` {#group__webrtc_1ga847983d3e60af0824b8363e58f3dd85b}



Create the peer connection once configuration, constraints and streams have been set.

#### `public virtual void closeConnection()` {#group__webrtc_1ga41381a096fe8fefd8535ef607cfbbb7f}

Close the peer connection.



#### `public virtual void createOffer()` {#group__webrtc_1ga1c86d086b82b977a52516d596482b194}



Create the offer SDP tos end to the peer. No offer should be received after creating the offer. A call to `recvRemoteAnswer` is expected to initiate the session.

#### `public virtual void recvSDP(const std::string & type,const std::string & sdp)` {#group__webrtc_1gad38194bce5b5df8f62e841ece00047ca}

Receive a remote offer or answer.



#### `public virtual void recvCandidate(const std::string & mid,int mlineindex,const std::string & sdp)` {#group__webrtc_1ga013523e83cf0a23da36b367862756c4d}

Receive a remote candidate.



#### `public void setPeerConnectionFactory(rtc::scoped_refptr< webrtc::PeerConnectionFactoryInterface > factory)` {#group__webrtc_1gafb9db9d781efdf7f3bb5422bf61b1817}



Set a custom PeerConnectionFactory object Must be done before any streams are initiated

#### `public std::string peerid() const` {#group__webrtc_1gab43565b5ae2bf7407bb7e530e45846a5}





#### `public webrtc::FakeConstraints & constraints()` {#group__webrtc_1gaa6193e32c730d2934af4e30f2b2555f2}





#### `public webrtc::PeerConnectionFactoryInterface * factory() const` {#group__webrtc_1ga3a65559eb8d0d5ca2a7e95167ff1f0e0}





#### `public rtc::scoped_refptr< webrtc::PeerConnectionInterface > peerConnection() const` {#group__webrtc_1ga0760ed629b7890029d50dfb029fef86e}





#### `public rtc::scoped_refptr< webrtc::MediaStreamInterface > stream() const` {#group__webrtc_1ga7a8592b73fd560f57e64f42fbff72e03}





#### `protected `[`PeerConnectionManager`](api-webrtc.md#classscy_1_1PeerConnectionManager)` * _manager` {#group__webrtc_1ga51f57fc701e28fa4d3662a7bce74c406}





#### `protected std::string _peerid` {#group__webrtc_1ga5d9b56c3f5b27fd16edd821f901f4234}





#### `protected `[`Mode`](#group__webrtc_1ga574415d63d860395230c8d596c679c08)` _mode` {#group__webrtc_1gaa1a826ffe59248f3c29d1549c3f3f6af}





#### `protected webrtc::PeerConnectionInterface::RTCConfiguration _config` {#group__webrtc_1ga5674acd452c799fe3f72840eb77c1c91}





#### `protected webrtc::FakeConstraints _constraints` {#group__webrtc_1ga7598128694bb49d761d0e153349e0b2e}





#### `protected rtc::scoped_refptr< webrtc::PeerConnectionFactoryInterface > _factory` {#group__webrtc_1ga3611dfd6cdd3a4d0209a73accd7abbd6}





#### `protected rtc::scoped_refptr< webrtc::PeerConnectionInterface > _peerConnection` {#group__webrtc_1gaf989d47d3d07677e4d247fd6bdc50e62}





#### `protected rtc::scoped_refptr< webrtc::MediaStreamInterface > _stream` {#group__webrtc_1gada1ca1a4449a007814f572caceb6ac5c}





#### `protected virtual void OnAddStream(webrtc::MediaStreamInterface * stream)` {#group__webrtc_1ga4cd2143cf2f5c5d57ffd1282fc7280c1}

inherited from PeerConnectionObserver



#### `protected virtual void OnRemoveStream(webrtc::MediaStreamInterface * stream)` {#group__webrtc_1ga519c801b0f48c6f7561d4b8dd382620b}





#### `protected virtual void OnIceCandidate(const webrtc::IceCandidateInterface * candidate)` {#group__webrtc_1ga72155cef7f1829cde73849f026bd4944}





#### `protected virtual void OnSignalingChange(webrtc::PeerConnectionInterface::SignalingState new_state)` {#group__webrtc_1gadf80f89c75b258ee5dcf1b98e3fc1621}





#### `protected virtual void OnIceConnectionChange(webrtc::PeerConnectionInterface::IceConnectionState new_state)` {#group__webrtc_1gabf456aff20ba716711dfff2292423e1a}





#### `protected virtual void OnIceGatheringChange(webrtc::PeerConnectionInterface::IceGatheringState new_state)` {#group__webrtc_1ga0a3169c7a8d260b6fd9d013086e07cec}





#### `protected virtual void OnRenegotiationNeeded()` {#group__webrtc_1gafb319ec12020fc90241473edaa722ea2}





#### `protected virtual void OnSuccess(webrtc::SessionDescriptionInterface * desc)` {#group__webrtc_1ga49474fdbbc2b9da7277448e64aed6bcc}

inherited from CreateSessionDescriptionObserver



#### `protected virtual void OnFailure(const std::string & error)` {#group__webrtc_1gaa4e63f15cf87655d87ad957070d4bdb6}





#### `protected inline virtual int AddRef() const` {#group__webrtc_1gafa7b220d6b6e107685281ac5623c0466}





#### `protected inline virtual int Release() const` {#group__webrtc_1ga16ff7f8374afdda55af0a999d3ad0a51}





# class `scy::PeerConnectionManager` {#classscy_1_1PeerConnectionManager}

```
class scy::PeerConnectionManager
  : public scy::PointerCollection< std::string, PeerConnection >
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  PeerConnectionManager(rtc::scoped_refptr< webrtc::PeerConnectionFactoryInterface > factory)` | 
`public virtual  ~PeerConnectionManager()` | 
`public virtual void sendSDP(`[`PeerConnection`](#classscy_1_1PeerConnection)` * conn,const std::string & type,const std::string & sdp)` | 
`public virtual void sendCandidate(`[`PeerConnection`](#classscy_1_1PeerConnection)` * conn,const std::string & mid,int mlineindex,const std::string & sdp)` | 
`public virtual void recvSDP(const std::string & peerid,const json::Value & data)` | 
`public virtual void recvCandidate(const std::string & peerid,const json::Value & data)` | 
`public virtual void onAddRemoteStream(`[`PeerConnection`](#classscy_1_1PeerConnection)` * conn,webrtc::MediaStreamInterface * stream)` | 
`public virtual void onRemoveRemoteStream(`[`PeerConnection`](#classscy_1_1PeerConnection)` * conn,webrtc::MediaStreamInterface * stream)` | 
`public virtual void onStable(`[`PeerConnection`](#classscy_1_1PeerConnection)` * conn)` | 
`public virtual void onClosed(`[`PeerConnection`](#classscy_1_1PeerConnection)` * conn)` | 
`public virtual void onFailure(`[`PeerConnection`](#classscy_1_1PeerConnection)` * conn,const std::string & error)` | 
`public webrtc::PeerConnectionFactoryInterface * factory() const` | 
`protected rtc::scoped_refptr< webrtc::PeerConnectionFactoryInterface > _factory` | 

## Members

#### `public  PeerConnectionManager(rtc::scoped_refptr< webrtc::PeerConnectionFactoryInterface > factory)` {#group__webrtc_1ga1a9081a9544dfd28ae955f53320887de}





#### `public virtual  ~PeerConnectionManager()` {#group__webrtc_1ga0ba148dde4da97e8e8f375e2ea7503f6}





#### `public virtual void sendSDP(`[`PeerConnection`](#classscy_1_1PeerConnection)` * conn,const std::string & type,const std::string & sdp)` {#group__webrtc_1ga9d22a97ec83a528e1077dc3a050a5bc2}





#### `public virtual void sendCandidate(`[`PeerConnection`](#classscy_1_1PeerConnection)` * conn,const std::string & mid,int mlineindex,const std::string & sdp)` {#group__webrtc_1gaacb5de3f11349384f4e621bbf5a3824b}





#### `public virtual void recvSDP(const std::string & peerid,const json::Value & data)` {#group__webrtc_1ga68fe1328473271d753aa4ee14be5fdfd}





#### `public virtual void recvCandidate(const std::string & peerid,const json::Value & data)` {#group__webrtc_1ga37ef92bc5488ca9dfdb4660cbb8e9e0c}





#### `public virtual void onAddRemoteStream(`[`PeerConnection`](#classscy_1_1PeerConnection)` * conn,webrtc::MediaStreamInterface * stream)` {#group__webrtc_1gad52b271ba6751d98b1adf8f550033cc5}





#### `public virtual void onRemoveRemoteStream(`[`PeerConnection`](#classscy_1_1PeerConnection)` * conn,webrtc::MediaStreamInterface * stream)` {#group__webrtc_1ga1be5c9c5dc3d3c01e39beb2e2b05beef}





#### `public virtual void onStable(`[`PeerConnection`](#classscy_1_1PeerConnection)` * conn)` {#group__webrtc_1ga6c52b216b0c5f6b6b3afc924bc076a27}





#### `public virtual void onClosed(`[`PeerConnection`](#classscy_1_1PeerConnection)` * conn)` {#group__webrtc_1ga739c5816a68e153d374e081b7b7a97ef}





#### `public virtual void onFailure(`[`PeerConnection`](#classscy_1_1PeerConnection)` * conn,const std::string & error)` {#group__webrtc_1ga4db22343e946a91976230d4e1c7c82ed}





#### `public webrtc::PeerConnectionFactoryInterface * factory() const` {#group__webrtc_1gad16ca732eb26b2b8c2ded349e899fbca}





#### `protected rtc::scoped_refptr< webrtc::PeerConnectionFactoryInterface > _factory` {#group__webrtc_1ga6f999ca5c49cfd17e2ee1cef0e23f082}





# class `scy::StreamRecorder` {#classscy_1_1StreamRecorder}

```
class scy::StreamRecorder
  : public rtc::VideoSinkInterface< cricket::VideoFrame >
  : public AudioTrackSinkInterface
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  StreamRecorder(const `[`av::EncoderOptions`](#structscy_1_1av_1_1EncoderOptions)` & options)` | 
`public  ~StreamRecorder()` | 
`public void setVideoTrack(webrtc::VideoTrackInterface * track)` | 
`public void setAudioTrack(webrtc::AudioTrackInterface * track)` | 
`public void onPacketEncoded(`[`av::MediaPacket`](#structscy_1_1av_1_1MediaPacket)` & packet)` | 
`public void OnFrame(const cricket::VideoFrame & frame)` | VideoSinkInterface implementation.
`public void OnData(const void * audio_data,int bits_per_sample,int sample_rate,size_t number_of_channels,size_t number_of_frames)` | AudioTrackSinkInterface implementation.
`protected av::MultiplexEncoder _encoder` | 
`protected rtc::scoped_refptr< webrtc::VideoTrackInterface > _videoTrack` | 
`protected rtc::scoped_refptr< webrtc::AudioTrackInterface > _audioTrack` | 
`protected bool _awaitingVideo` | 
`protected bool _awaitingAudio` | 

## Members

#### `public  StreamRecorder(const `[`av::EncoderOptions`](#structscy_1_1av_1_1EncoderOptions)` & options)` {#group__webrtc_1ga6068e1fd7cf7cc31eac579ed1d3bcfbc}





#### `public  ~StreamRecorder()` {#group__webrtc_1ga07821b50f4ae39901d9278d667a39e4b}





#### `public void setVideoTrack(webrtc::VideoTrackInterface * track)` {#group__webrtc_1gae520cc7e565ce769ba5425318dda6529}





#### `public void setAudioTrack(webrtc::AudioTrackInterface * track)` {#group__webrtc_1ga1eb59f098ff9d21e54f34b86208b135b}





#### `public void onPacketEncoded(`[`av::MediaPacket`](#structscy_1_1av_1_1MediaPacket)` & packet)` {#group__webrtc_1gaaf4ef3e8b06ec760c8dcdd41acb2a7fc}





#### `public void OnFrame(const cricket::VideoFrame & frame)` {#group__webrtc_1gacf2c3e6b8692b29daa898f9a736860df}

VideoSinkInterface implementation.



#### `public void OnData(const void * audio_data,int bits_per_sample,int sample_rate,size_t number_of_channels,size_t number_of_frames)` {#group__webrtc_1ga7593a02a0face8e26c34651aacee4d66}

AudioTrackSinkInterface implementation.



#### `protected av::MultiplexEncoder _encoder` {#group__webrtc_1ga20a34774c58fe0b3087d43b3b5245182}





#### `protected rtc::scoped_refptr< webrtc::VideoTrackInterface > _videoTrack` {#group__webrtc_1gaf4e83c1875a6b097f00497a3a0a6d820}





#### `protected rtc::scoped_refptr< webrtc::AudioTrackInterface > _audioTrack` {#group__webrtc_1gacead832bb59c6f293b433045089f41d4}





#### `protected bool _awaitingVideo` {#group__webrtc_1gaec09b240fe6203cdafedee89b73e2c41}





#### `protected bool _awaitingAudio` {#group__webrtc_1ga31ac466728eb43b7910fc69ed072988d}





# class `scy::VideoPacketSource` {#classscy_1_1VideoPacketSource}

```
class scy::VideoPacketSource
  : public VideoCapturer
```  



[VideoPacketSource](#classscy_1_1VideoPacketSource) implements a simple `cricket::VideoCapturer` that gets decoded remote video frames from a local media channel. It's used as the remote video source's `VideoCapturer` so that the remote video can be used as a `cricket::VideoCapturer` and in that way a remote video stream can implement the `MediaStreamSourceInterface`.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  VideoPacketSource()` | 
`public virtual  ~VideoPacketSource()` | 
`public virtual cricket::CaptureState Start(const cricket::VideoFormat & capture_format)` | cricket::VideoCapturer implementation.
`public virtual void Stop()` | 
`public virtual bool IsRunning()` | 
`public virtual bool GetPreferredFourccs(std::vector< uint32_t > * fourccs)` | 
`public virtual bool GetBestCaptureFormat(const cricket::VideoFormat & desired,cricket::VideoFormat * best_format)` | 
`public virtual bool IsScreencast() const` | 
`public void onVideoCaptured(void * sender,`[`av::VideoPacket`](#structscy_1_1av_1_1VideoPacket)` & packet)` | 

## Members

#### `public  VideoPacketSource()` {#group__webrtc_1ga8da824426a8338ee02bb319feae78e56}





#### `public virtual  ~VideoPacketSource()` {#group__webrtc_1gac652531816f5733087557ef2fcf5b51f}





#### `public virtual cricket::CaptureState Start(const cricket::VideoFormat & capture_format)` {#group__webrtc_1ga0745886f5830c304fbc2c1fc1d2ef274}

cricket::VideoCapturer implementation.



#### `public virtual void Stop()` {#group__webrtc_1ga28218e24b6caed91239eaed214b92d7b}





#### `public virtual bool IsRunning()` {#group__webrtc_1ga939187c7519b9f57673ed9b011c9fec4}





#### `public virtual bool GetPreferredFourccs(std::vector< uint32_t > * fourccs)` {#group__webrtc_1gaec0128ac014ce2a792e37cefabfb508a}





#### `public virtual bool GetBestCaptureFormat(const cricket::VideoFormat & desired,cricket::VideoFormat * best_format)` {#group__webrtc_1gab8f8f2b8defc77f03a5ebef5a616236c}





#### `public virtual bool IsScreencast() const` {#group__webrtc_1ga124e023f05d711b4cb8fe8db18ee598b}





#### `public void onVideoCaptured(void * sender,`[`av::VideoPacket`](#structscy_1_1av_1_1VideoPacket)` & packet)` {#group__webrtc_1gae4d952af8305951cdb2a1bfb4dd46ca2}





