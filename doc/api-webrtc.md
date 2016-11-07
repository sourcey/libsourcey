# Module <!-- group --> `webrtc`

The `webrtc` module contains WebRTC integrations.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`namespace `[``scy``](#namespacescy)    | 
`class `[``scy::AudioPacketModule``](#classscy_1_1AudioPacketModule)    | 
`class `[``scy::ImageSequenceRecorder``](#classscy_1_1ImageSequenceRecorder)    | 
`class `[``scy::MultiplexMediaCapturer``](#classscy_1_1MultiplexMediaCapturer)    | 
`class `[``scy::PeerConnection``](#classscy_1_1PeerConnection)    | 
`class `[``scy::DummySetSessionDescriptionObserver``](#classscy_1_1DummySetSessionDescriptionObserver)    | 
`class `[``scy::PeerConnectionManager``](#classscy_1_1PeerConnectionManager)    | 
`class `[``scy::StreamRecorder``](#classscy_1_1StreamRecorder)    | 
`class `[``scy::VideoPacketSource``](#classscy_1_1VideoPacketSource)    | 
# namespace `scy`



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[``scy::AudioPacketModule``](#classscy_1_1AudioPacketModule)    | 
`class `[``scy::DummySetSessionDescriptionObserver``](#classscy_1_1DummySetSessionDescriptionObserver)    | 
`class `[``scy::ImageSequenceRecorder``](#classscy_1_1ImageSequenceRecorder)    | 
`class `[``scy::MultiplexMediaCapturer``](#classscy_1_1MultiplexMediaCapturer)    | 
`class `[``scy::PeerConnection``](#classscy_1_1PeerConnection)    | 
`class `[``scy::PeerConnectionManager``](#classscy_1_1PeerConnectionManager)    | 
`class `[``scy::StreamRecorder``](#classscy_1_1StreamRecorder)    | 
`class `[``scy::VideoPacketSource``](#classscy_1_1VideoPacketSource)    | 
# class `scy::AudioPacketModule` 

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

#### `public void onAudioCaptured(void * sender,`[`av::AudioPacket`](#structscy_1_1av_1_1AudioPacket)` & packet)` 

Handles input packets from the capture for sending.



#### `public int64_t TimeUntilNextProcess()` 



Following functions are inherited from `webrtc::AudioDeviceModule`. Only functions called by `[PeerConnection](#classscy_1_1PeerConnection)` are implemented, the rest do nothing and return success. If a function is not expected to be called by `[PeerConnection](#classscy_1_1PeerConnection)` an assertion is triggered if it is in fact called.

#### `public void Process()` 





#### `public int32_t ActiveAudioLayer(AudioLayer * audio_layer) const` 





#### `public ErrorCode LastError() const` 





#### `public int32_t RegisterEventObserver(webrtc::AudioDeviceObserver * event_callback)` 





#### `public int32_t RegisterAudioCallback(webrtc::AudioTransport * audio_callback)` 

Note: Calling this method from a callback may result in deadlock.



#### `public int32_t Init()` 





#### `public int32_t Terminate()` 





#### `public bool Initialized() const` 





#### `public int16_t PlayoutDevices()` 





#### `public int16_t RecordingDevices()` 





#### `public int32_t PlayoutDeviceName(uint16_t index,char name,char guid)` 





#### `public int32_t RecordingDeviceName(uint16_t index,char name,char guid)` 





#### `public int32_t SetPlayoutDevice(uint16_t index)` 





#### `public int32_t SetPlayoutDevice(WindowsDeviceType device)` 





#### `public int32_t SetRecordingDevice(uint16_t index)` 





#### `public int32_t SetRecordingDevice(WindowsDeviceType device)` 





#### `public int32_t PlayoutIsAvailable(bool * available)` 





#### `public int32_t InitPlayout()` 





#### `public bool PlayoutIsInitialized() const` 





#### `public int32_t RecordingIsAvailable(bool * available)` 





#### `public int32_t InitRecording()` 





#### `public bool RecordingIsInitialized() const` 





#### `public int32_t StartPlayout()` 





#### `public int32_t StopPlayout()` 





#### `public bool Playing() const` 





#### `public int32_t StartRecording()` 





#### `public int32_t StopRecording()` 





#### `public bool Recording() const` 





#### `public int32_t SetAGC(bool enable)` 





#### `public bool AGC() const` 





#### `public int32_t SetWaveOutVolume(uint16_t volume_left,uint16_t volume_right)` 





#### `public int32_t WaveOutVolume(uint16_t * volume_left,uint16_t * volume_right) const` 





#### `public int32_t InitSpeaker()` 





#### `public bool SpeakerIsInitialized() const` 





#### `public int32_t InitMicrophone()` 





#### `public bool MicrophoneIsInitialized() const` 





#### `public int32_t SpeakerVolumeIsAvailable(bool * available)` 





#### `public int32_t SetSpeakerVolume(uint32_t volume)` 





#### `public int32_t SpeakerVolume(uint32_t * volume) const` 





#### `public int32_t MaxSpeakerVolume(uint32_t * max_volume) const` 





#### `public int32_t MinSpeakerVolume(uint32_t * min_volume) const` 





#### `public int32_t SpeakerVolumeStepSize(uint16_t * step_size) const` 





#### `public int32_t MicrophoneVolumeIsAvailable(bool * available)` 





#### `public int32_t SetMicrophoneVolume(uint32_t volume)` 





#### `public int32_t MicrophoneVolume(uint32_t * volume) const` 





#### `public int32_t MaxMicrophoneVolume(uint32_t * max_volume) const` 





#### `public int32_t MinMicrophoneVolume(uint32_t * min_volume) const` 





#### `public int32_t MicrophoneVolumeStepSize(uint16_t * step_size) const` 





#### `public int32_t SpeakerMuteIsAvailable(bool * available)` 





#### `public int32_t SetSpeakerMute(bool enable)` 





#### `public int32_t SpeakerMute(bool * enabled) const` 





#### `public int32_t MicrophoneMuteIsAvailable(bool * available)` 





#### `public int32_t SetMicrophoneMute(bool enable)` 





#### `public int32_t MicrophoneMute(bool * enabled) const` 





#### `public int32_t MicrophoneBoostIsAvailable(bool * available)` 





#### `public int32_t SetMicrophoneBoost(bool enable)` 





#### `public int32_t MicrophoneBoost(bool * enabled) const` 





#### `public int32_t StereoPlayoutIsAvailable(bool * available) const` 





#### `public int32_t SetStereoPlayout(bool enable)` 





#### `public int32_t StereoPlayout(bool * enabled) const` 





#### `public int32_t StereoRecordingIsAvailable(bool * available) const` 





#### `public int32_t SetStereoRecording(bool enable)` 





#### `public int32_t StereoRecording(bool * enabled) const` 





#### `public int32_t SetRecordingChannel(const ChannelType channel)` 





#### `public int32_t RecordingChannel(ChannelType * channel) const` 





#### `public int32_t SetPlayoutBuffer(const BufferType type,uint16_t size_ms)` 





#### `public int32_t PlayoutBuffer(BufferType * type,uint16_t * size_ms) const` 





#### `public int32_t PlayoutDelay(uint16_t * delay_ms) const` 





#### `public int32_t RecordingDelay(uint16_t * delay_ms) const` 





#### `public int32_t CPULoad(uint16_t * load) const` 





#### `public int32_t StartRawOutputFileRecording(const char pcm_file_name_utf8)` 





#### `public int32_t StopRawOutputFileRecording()` 





#### `public int32_t StartRawInputFileRecording(const char pcm_file_name_utf8)` 





#### `public int32_t StopRawInputFileRecording()` 





#### `public int32_t SetRecordingSampleRate(const uint32_t samples_per_sec)` 





#### `public int32_t RecordingSampleRate(uint32_t * samples_per_sec) const` 





#### `public int32_t SetPlayoutSampleRate(const uint32_t samples_per_sec)` 





#### `public int32_t PlayoutSampleRate(uint32_t * samples_per_sec) const` 





#### `public int32_t ResetAudioDevice()` 





#### `public int32_t SetLoudspeakerStatus(bool enable)` 





#### `public int32_t GetLoudspeakerStatus(bool * enabled) const` 





#### `public inline bool BuiltInAECIsAvailable() const` 





#### `public inline int32_t EnableBuiltInAEC(bool enable)` 





#### `public inline bool BuiltInAGCIsAvailable() const` 





#### `public inline int32_t EnableBuiltInAGC(bool enable)` 





#### `public inline bool BuiltInNSIsAvailable() const` 





#### `public inline int32_t EnableBuiltInNS(bool enable)` 





#### `public void OnMessage(rtc::Message * msg)` 

WEBRTC_IOS.

End of functions inherited from `webrtc::AudioDeviceModule`. The following function is inherited from `rtc::MessageHandler`.

#### `protected  explicit AudioPacketModule()` 



The constructor is protected because the class needs to be created as a reference counted object (for memory managment reasons). It could be exposed in which case the burden of proper instantiation would be put on the creator of a [AudioPacketModule](#classscy_1_1AudioPacketModule) instance. To create an instance of this class use the `[Create()](#group__webrtc_1gaf78f45016b72c9bdca424e9a31691db8)` API.

#### `protected virtual  ~AudioPacketModule()` 



The destructor is protected because it is reference counted and should not be deleted directly.

# class `scy::DummySetSessionDescriptionObserver` 

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

#### `public virtual void OnSuccess()` 





#### `public virtual void OnFailure(const std::string & error)` 





#### `protected inline  DummySetSessionDescriptionObserver()` 





#### `protected inline  ~DummySetSessionDescriptionObserver()` 





# class `scy::ImageSequenceRecorder` 

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

#### `public  ImageSequenceRecorder(webrtc::VideoTrackInterface * track_to_render,const std::string & basename)` 





#### `public virtual  ~ImageSequenceRecorder()` 





#### `public std::string getNextFilename()` 





#### `public void OnFrame(const cricket::VideoFrame & frame)` 

VideoSinkInterface implementation.



#### `protected rtc::scoped_refptr< webrtc::VideoTrackInterface > _renderedTrack` 





#### `protected const std::string _basename` 





#### `protected size_t _count` 





#### `protected int _width` 





#### `protected int _height` 





#### `protected av::VideoEncoder _encoder` 





#### `protected AVFrame * _avframe` 





# class `scy::MultiplexMediaCapturer` 






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
`protected `[`PacketStream`](#classscy_1_1PacketStream)` _stream` | 
`protected av::MediaCapture::Ptr _capture` | 
`protected rtc::scoped_refptr< `[`AudioPacketModule`](#classscy_1_1AudioPacketModule)` > _audioModule` | 

## Members

#### `public  MultiplexMediaCapturer()` 





#### `public virtual  ~MultiplexMediaCapturer()` 





#### `public virtual void openFile(const std::string & file)` 





#### `public virtual void addMediaTracks(webrtc::PeerConnectionFactoryInterface * factory,webrtc::MediaStreamInterface * stream)` 





#### `public virtual void start()` 





#### `public virtual void stop()` 





#### `public virtual rtc::scoped_refptr< `[`AudioPacketModule`](#classscy_1_1AudioPacketModule)` > getAudioModule()` 





#### `public virtual `[`VideoPacketSource`](#classscy_1_1VideoPacketSource)` * createVideoSource()` 





#### `protected `[`PacketStream`](#classscy_1_1PacketStream)` _stream` 





#### `protected av::MediaCapture::Ptr _capture` 





#### `protected rtc::scoped_refptr< `[`AudioPacketModule`](#classscy_1_1AudioPacketModule)` > _audioModule` 





# class `scy::PeerConnection` 

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
`protected `[`PeerConnectionManager`](#classscy_1_1PeerConnectionManager)` * _manager` | 
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

#### `public  PeerConnection(`[`PeerConnectionManager`](#classscy_1_1PeerConnectionManager)` * manager,const std::string & peerid,`[`Mode`](#group__webrtc_1ga574415d63d860395230c8d596c679c08)` mode)` 





#### `public virtual  ~PeerConnection()` 





#### `public virtual rtc::scoped_refptr< webrtc::MediaStreamInterface > createMediaStream()` 



Create the local media stream. Only necessary when we are creating the offer.

#### `public virtual void createConnection()` 



Create the peer connection once configuration, constraints and streams have been set.

#### `public virtual void closeConnection()` 

Close the peer connection.



#### `public virtual void createOffer()` 



Create the offer SDP tos end to the peer. No offer should be received after creating the offer. A call to `recvRemoteAnswer` is expected to initiate the session.

#### `public virtual void recvSDP(const std::string & type,const std::string & sdp)` 

Receive a remote offer or answer.



#### `public virtual void recvCandidate(const std::string & mid,int mlineindex,const std::string & sdp)` 

Receive a remote candidate.



#### `public void setPeerConnectionFactory(rtc::scoped_refptr< webrtc::PeerConnectionFactoryInterface > factory)` 



Set a custom PeerConnectionFactory object Must be done before any streams are initiated

#### `public std::string peerid() const` 





#### `public webrtc::FakeConstraints & constraints()` 





#### `public webrtc::PeerConnectionFactoryInterface * factory() const` 





#### `public rtc::scoped_refptr< webrtc::PeerConnectionInterface > peerConnection() const` 





#### `public rtc::scoped_refptr< webrtc::MediaStreamInterface > stream() const` 





#### `protected `[`PeerConnectionManager`](#classscy_1_1PeerConnectionManager)` * _manager` 





#### `protected std::string _peerid` 





#### `protected `[`Mode`](#group__webrtc_1ga574415d63d860395230c8d596c679c08)` _mode` 





#### `protected webrtc::PeerConnectionInterface::RTCConfiguration _config` 





#### `protected webrtc::FakeConstraints _constraints` 





#### `protected rtc::scoped_refptr< webrtc::PeerConnectionFactoryInterface > _factory` 





#### `protected rtc::scoped_refptr< webrtc::PeerConnectionInterface > _peerConnection` 





#### `protected rtc::scoped_refptr< webrtc::MediaStreamInterface > _stream` 





#### `protected virtual void OnAddStream(webrtc::MediaStreamInterface * stream)` 

inherited from PeerConnectionObserver



#### `protected virtual void OnRemoveStream(webrtc::MediaStreamInterface * stream)` 





#### `protected virtual void OnIceCandidate(const webrtc::IceCandidateInterface * candidate)` 





#### `protected virtual void OnSignalingChange(webrtc::PeerConnectionInterface::SignalingState new_state)` 





#### `protected virtual void OnIceConnectionChange(webrtc::PeerConnectionInterface::IceConnectionState new_state)` 





#### `protected virtual void OnIceGatheringChange(webrtc::PeerConnectionInterface::IceGatheringState new_state)` 





#### `protected virtual void OnRenegotiationNeeded()` 





#### `protected virtual void OnSuccess(webrtc::SessionDescriptionInterface * desc)` 

inherited from CreateSessionDescriptionObserver



#### `protected virtual void OnFailure(const std::string & error)` 





#### `protected inline virtual int AddRef() const` 





#### `protected inline virtual int Release() const` 





# class `scy::PeerConnectionManager` 

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

#### `public  PeerConnectionManager(rtc::scoped_refptr< webrtc::PeerConnectionFactoryInterface > factory)` 





#### `public virtual  ~PeerConnectionManager()` 





#### `public virtual void sendSDP(`[`PeerConnection`](#classscy_1_1PeerConnection)` * conn,const std::string & type,const std::string & sdp)` 





#### `public virtual void sendCandidate(`[`PeerConnection`](#classscy_1_1PeerConnection)` * conn,const std::string & mid,int mlineindex,const std::string & sdp)` 





#### `public virtual void recvSDP(const std::string & peerid,const json::Value & data)` 





#### `public virtual void recvCandidate(const std::string & peerid,const json::Value & data)` 





#### `public virtual void onAddRemoteStream(`[`PeerConnection`](#classscy_1_1PeerConnection)` * conn,webrtc::MediaStreamInterface * stream)` 





#### `public virtual void onRemoveRemoteStream(`[`PeerConnection`](#classscy_1_1PeerConnection)` * conn,webrtc::MediaStreamInterface * stream)` 





#### `public virtual void onStable(`[`PeerConnection`](#classscy_1_1PeerConnection)` * conn)` 





#### `public virtual void onClosed(`[`PeerConnection`](#classscy_1_1PeerConnection)` * conn)` 





#### `public virtual void onFailure(`[`PeerConnection`](#classscy_1_1PeerConnection)` * conn,const std::string & error)` 





#### `public webrtc::PeerConnectionFactoryInterface * factory() const` 





#### `protected rtc::scoped_refptr< webrtc::PeerConnectionFactoryInterface > _factory` 





# class `scy::StreamRecorder` 

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

#### `public  StreamRecorder(const `[`av::EncoderOptions`](#structscy_1_1av_1_1EncoderOptions)` & options)` 





#### `public  ~StreamRecorder()` 





#### `public void setVideoTrack(webrtc::VideoTrackInterface * track)` 





#### `public void setAudioTrack(webrtc::AudioTrackInterface * track)` 





#### `public void onPacketEncoded(`[`av::MediaPacket`](#structscy_1_1av_1_1MediaPacket)` & packet)` 





#### `public void OnFrame(const cricket::VideoFrame & frame)` 

VideoSinkInterface implementation.



#### `public void OnData(const void * audio_data,int bits_per_sample,int sample_rate,size_t number_of_channels,size_t number_of_frames)` 

AudioTrackSinkInterface implementation.



#### `protected av::MultiplexEncoder _encoder` 





#### `protected rtc::scoped_refptr< webrtc::VideoTrackInterface > _videoTrack` 





#### `protected rtc::scoped_refptr< webrtc::AudioTrackInterface > _audioTrack` 





#### `protected bool _awaitingVideo` 





#### `protected bool _awaitingAudio` 





# class `scy::VideoPacketSource` 

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

#### `public  VideoPacketSource()` 





#### `public virtual  ~VideoPacketSource()` 





#### `public virtual cricket::CaptureState Start(const cricket::VideoFormat & capture_format)` 

cricket::VideoCapturer implementation.



#### `public virtual void Stop()` 





#### `public virtual bool IsRunning()` 





#### `public virtual bool GetPreferredFourccs(std::vector< uint32_t > * fourccs)` 





#### `public virtual bool GetBestCaptureFormat(const cricket::VideoFormat & desired,cricket::VideoFormat * best_format)` 





#### `public virtual bool IsScreencast() const` 





#### `public void onVideoCaptured(void * sender,`[`av::VideoPacket`](#structscy_1_1av_1_1VideoPacket)` & packet)` 





# class `scy::AudioPacketModule` 

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

#### `public void onAudioCaptured(void * sender,`[`av::AudioPacket`](#structscy_1_1av_1_1AudioPacket)` & packet)` 

Handles input packets from the capture for sending.



#### `public int64_t TimeUntilNextProcess()` 



Following functions are inherited from `webrtc::AudioDeviceModule`. Only functions called by `[PeerConnection](#classscy_1_1PeerConnection)` are implemented, the rest do nothing and return success. If a function is not expected to be called by `[PeerConnection](#classscy_1_1PeerConnection)` an assertion is triggered if it is in fact called.

#### `public void Process()` 





#### `public int32_t ActiveAudioLayer(AudioLayer * audio_layer) const` 





#### `public ErrorCode LastError() const` 





#### `public int32_t RegisterEventObserver(webrtc::AudioDeviceObserver * event_callback)` 





#### `public int32_t RegisterAudioCallback(webrtc::AudioTransport * audio_callback)` 

Note: Calling this method from a callback may result in deadlock.



#### `public int32_t Init()` 





#### `public int32_t Terminate()` 





#### `public bool Initialized() const` 





#### `public int16_t PlayoutDevices()` 





#### `public int16_t RecordingDevices()` 





#### `public int32_t PlayoutDeviceName(uint16_t index,char name,char guid)` 





#### `public int32_t RecordingDeviceName(uint16_t index,char name,char guid)` 





#### `public int32_t SetPlayoutDevice(uint16_t index)` 





#### `public int32_t SetPlayoutDevice(WindowsDeviceType device)` 





#### `public int32_t SetRecordingDevice(uint16_t index)` 





#### `public int32_t SetRecordingDevice(WindowsDeviceType device)` 





#### `public int32_t PlayoutIsAvailable(bool * available)` 





#### `public int32_t InitPlayout()` 





#### `public bool PlayoutIsInitialized() const` 





#### `public int32_t RecordingIsAvailable(bool * available)` 





#### `public int32_t InitRecording()` 





#### `public bool RecordingIsInitialized() const` 





#### `public int32_t StartPlayout()` 





#### `public int32_t StopPlayout()` 





#### `public bool Playing() const` 





#### `public int32_t StartRecording()` 





#### `public int32_t StopRecording()` 





#### `public bool Recording() const` 





#### `public int32_t SetAGC(bool enable)` 





#### `public bool AGC() const` 





#### `public int32_t SetWaveOutVolume(uint16_t volume_left,uint16_t volume_right)` 





#### `public int32_t WaveOutVolume(uint16_t * volume_left,uint16_t * volume_right) const` 





#### `public int32_t InitSpeaker()` 





#### `public bool SpeakerIsInitialized() const` 





#### `public int32_t InitMicrophone()` 





#### `public bool MicrophoneIsInitialized() const` 





#### `public int32_t SpeakerVolumeIsAvailable(bool * available)` 





#### `public int32_t SetSpeakerVolume(uint32_t volume)` 





#### `public int32_t SpeakerVolume(uint32_t * volume) const` 





#### `public int32_t MaxSpeakerVolume(uint32_t * max_volume) const` 





#### `public int32_t MinSpeakerVolume(uint32_t * min_volume) const` 





#### `public int32_t SpeakerVolumeStepSize(uint16_t * step_size) const` 





#### `public int32_t MicrophoneVolumeIsAvailable(bool * available)` 





#### `public int32_t SetMicrophoneVolume(uint32_t volume)` 





#### `public int32_t MicrophoneVolume(uint32_t * volume) const` 





#### `public int32_t MaxMicrophoneVolume(uint32_t * max_volume) const` 





#### `public int32_t MinMicrophoneVolume(uint32_t * min_volume) const` 





#### `public int32_t MicrophoneVolumeStepSize(uint16_t * step_size) const` 





#### `public int32_t SpeakerMuteIsAvailable(bool * available)` 





#### `public int32_t SetSpeakerMute(bool enable)` 





#### `public int32_t SpeakerMute(bool * enabled) const` 





#### `public int32_t MicrophoneMuteIsAvailable(bool * available)` 





#### `public int32_t SetMicrophoneMute(bool enable)` 





#### `public int32_t MicrophoneMute(bool * enabled) const` 





#### `public int32_t MicrophoneBoostIsAvailable(bool * available)` 





#### `public int32_t SetMicrophoneBoost(bool enable)` 





#### `public int32_t MicrophoneBoost(bool * enabled) const` 





#### `public int32_t StereoPlayoutIsAvailable(bool * available) const` 





#### `public int32_t SetStereoPlayout(bool enable)` 





#### `public int32_t StereoPlayout(bool * enabled) const` 





#### `public int32_t StereoRecordingIsAvailable(bool * available) const` 





#### `public int32_t SetStereoRecording(bool enable)` 





#### `public int32_t StereoRecording(bool * enabled) const` 





#### `public int32_t SetRecordingChannel(const ChannelType channel)` 





#### `public int32_t RecordingChannel(ChannelType * channel) const` 





#### `public int32_t SetPlayoutBuffer(const BufferType type,uint16_t size_ms)` 





#### `public int32_t PlayoutBuffer(BufferType * type,uint16_t * size_ms) const` 





#### `public int32_t PlayoutDelay(uint16_t * delay_ms) const` 





#### `public int32_t RecordingDelay(uint16_t * delay_ms) const` 





#### `public int32_t CPULoad(uint16_t * load) const` 





#### `public int32_t StartRawOutputFileRecording(const char pcm_file_name_utf8)` 





#### `public int32_t StopRawOutputFileRecording()` 





#### `public int32_t StartRawInputFileRecording(const char pcm_file_name_utf8)` 





#### `public int32_t StopRawInputFileRecording()` 





#### `public int32_t SetRecordingSampleRate(const uint32_t samples_per_sec)` 





#### `public int32_t RecordingSampleRate(uint32_t * samples_per_sec) const` 





#### `public int32_t SetPlayoutSampleRate(const uint32_t samples_per_sec)` 





#### `public int32_t PlayoutSampleRate(uint32_t * samples_per_sec) const` 





#### `public int32_t ResetAudioDevice()` 





#### `public int32_t SetLoudspeakerStatus(bool enable)` 





#### `public int32_t GetLoudspeakerStatus(bool * enabled) const` 





#### `public inline bool BuiltInAECIsAvailable() const` 





#### `public inline int32_t EnableBuiltInAEC(bool enable)` 





#### `public inline bool BuiltInAGCIsAvailable() const` 





#### `public inline int32_t EnableBuiltInAGC(bool enable)` 





#### `public inline bool BuiltInNSIsAvailable() const` 





#### `public inline int32_t EnableBuiltInNS(bool enable)` 





#### `public void OnMessage(rtc::Message * msg)` 

WEBRTC_IOS.

End of functions inherited from `webrtc::AudioDeviceModule`. The following function is inherited from `rtc::MessageHandler`.

#### `protected  explicit AudioPacketModule()` 



The constructor is protected because the class needs to be created as a reference counted object (for memory managment reasons). It could be exposed in which case the burden of proper instantiation would be put on the creator of a [AudioPacketModule](#classscy_1_1AudioPacketModule) instance. To create an instance of this class use the `[Create()](#group__webrtc_1gaf78f45016b72c9bdca424e9a31691db8)` API.

#### `protected virtual  ~AudioPacketModule()` 



The destructor is protected because it is reference counted and should not be deleted directly.

# class `scy::ImageSequenceRecorder` 

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

#### `public  ImageSequenceRecorder(webrtc::VideoTrackInterface * track_to_render,const std::string & basename)` 





#### `public virtual  ~ImageSequenceRecorder()` 





#### `public std::string getNextFilename()` 





#### `public void OnFrame(const cricket::VideoFrame & frame)` 

VideoSinkInterface implementation.



#### `protected rtc::scoped_refptr< webrtc::VideoTrackInterface > _renderedTrack` 





#### `protected const std::string _basename` 





#### `protected size_t _count` 





#### `protected int _width` 





#### `protected int _height` 





#### `protected av::VideoEncoder _encoder` 





#### `protected AVFrame * _avframe` 





# class `scy::MultiplexMediaCapturer` 






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
`protected `[`PacketStream`](#classscy_1_1PacketStream)` _stream` | 
`protected av::MediaCapture::Ptr _capture` | 
`protected rtc::scoped_refptr< `[`AudioPacketModule`](#classscy_1_1AudioPacketModule)` > _audioModule` | 

## Members

#### `public  MultiplexMediaCapturer()` 





#### `public virtual  ~MultiplexMediaCapturer()` 





#### `public virtual void openFile(const std::string & file)` 





#### `public virtual void addMediaTracks(webrtc::PeerConnectionFactoryInterface * factory,webrtc::MediaStreamInterface * stream)` 





#### `public virtual void start()` 





#### `public virtual void stop()` 





#### `public virtual rtc::scoped_refptr< `[`AudioPacketModule`](#classscy_1_1AudioPacketModule)` > getAudioModule()` 





#### `public virtual `[`VideoPacketSource`](#classscy_1_1VideoPacketSource)` * createVideoSource()` 





#### `protected `[`PacketStream`](#classscy_1_1PacketStream)` _stream` 





#### `protected av::MediaCapture::Ptr _capture` 





#### `protected rtc::scoped_refptr< `[`AudioPacketModule`](#classscy_1_1AudioPacketModule)` > _audioModule` 





# class `scy::PeerConnection` 

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
`protected `[`PeerConnectionManager`](#classscy_1_1PeerConnectionManager)` * _manager` | 
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

#### `public  PeerConnection(`[`PeerConnectionManager`](#classscy_1_1PeerConnectionManager)` * manager,const std::string & peerid,`[`Mode`](#group__webrtc_1ga574415d63d860395230c8d596c679c08)` mode)` 





#### `public virtual  ~PeerConnection()` 





#### `public virtual rtc::scoped_refptr< webrtc::MediaStreamInterface > createMediaStream()` 



Create the local media stream. Only necessary when we are creating the offer.

#### `public virtual void createConnection()` 



Create the peer connection once configuration, constraints and streams have been set.

#### `public virtual void closeConnection()` 

Close the peer connection.



#### `public virtual void createOffer()` 



Create the offer SDP tos end to the peer. No offer should be received after creating the offer. A call to `recvRemoteAnswer` is expected to initiate the session.

#### `public virtual void recvSDP(const std::string & type,const std::string & sdp)` 

Receive a remote offer or answer.



#### `public virtual void recvCandidate(const std::string & mid,int mlineindex,const std::string & sdp)` 

Receive a remote candidate.



#### `public void setPeerConnectionFactory(rtc::scoped_refptr< webrtc::PeerConnectionFactoryInterface > factory)` 



Set a custom PeerConnectionFactory object Must be done before any streams are initiated

#### `public std::string peerid() const` 





#### `public webrtc::FakeConstraints & constraints()` 





#### `public webrtc::PeerConnectionFactoryInterface * factory() const` 





#### `public rtc::scoped_refptr< webrtc::PeerConnectionInterface > peerConnection() const` 





#### `public rtc::scoped_refptr< webrtc::MediaStreamInterface > stream() const` 





#### `protected `[`PeerConnectionManager`](#classscy_1_1PeerConnectionManager)` * _manager` 





#### `protected std::string _peerid` 





#### `protected `[`Mode`](#group__webrtc_1ga574415d63d860395230c8d596c679c08)` _mode` 





#### `protected webrtc::PeerConnectionInterface::RTCConfiguration _config` 





#### `protected webrtc::FakeConstraints _constraints` 





#### `protected rtc::scoped_refptr< webrtc::PeerConnectionFactoryInterface > _factory` 





#### `protected rtc::scoped_refptr< webrtc::PeerConnectionInterface > _peerConnection` 





#### `protected rtc::scoped_refptr< webrtc::MediaStreamInterface > _stream` 





#### `protected virtual void OnAddStream(webrtc::MediaStreamInterface * stream)` 

inherited from PeerConnectionObserver



#### `protected virtual void OnRemoveStream(webrtc::MediaStreamInterface * stream)` 





#### `protected virtual void OnIceCandidate(const webrtc::IceCandidateInterface * candidate)` 





#### `protected virtual void OnSignalingChange(webrtc::PeerConnectionInterface::SignalingState new_state)` 





#### `protected virtual void OnIceConnectionChange(webrtc::PeerConnectionInterface::IceConnectionState new_state)` 





#### `protected virtual void OnIceGatheringChange(webrtc::PeerConnectionInterface::IceGatheringState new_state)` 





#### `protected virtual void OnRenegotiationNeeded()` 





#### `protected virtual void OnSuccess(webrtc::SessionDescriptionInterface * desc)` 

inherited from CreateSessionDescriptionObserver



#### `protected virtual void OnFailure(const std::string & error)` 





#### `protected inline virtual int AddRef() const` 





#### `protected inline virtual int Release() const` 





# class `scy::DummySetSessionDescriptionObserver` 

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

#### `public virtual void OnSuccess()` 





#### `public virtual void OnFailure(const std::string & error)` 





#### `protected inline  DummySetSessionDescriptionObserver()` 





#### `protected inline  ~DummySetSessionDescriptionObserver()` 





# class `scy::PeerConnectionManager` 

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

#### `public  PeerConnectionManager(rtc::scoped_refptr< webrtc::PeerConnectionFactoryInterface > factory)` 





#### `public virtual  ~PeerConnectionManager()` 





#### `public virtual void sendSDP(`[`PeerConnection`](#classscy_1_1PeerConnection)` * conn,const std::string & type,const std::string & sdp)` 





#### `public virtual void sendCandidate(`[`PeerConnection`](#classscy_1_1PeerConnection)` * conn,const std::string & mid,int mlineindex,const std::string & sdp)` 





#### `public virtual void recvSDP(const std::string & peerid,const json::Value & data)` 





#### `public virtual void recvCandidate(const std::string & peerid,const json::Value & data)` 





#### `public virtual void onAddRemoteStream(`[`PeerConnection`](#classscy_1_1PeerConnection)` * conn,webrtc::MediaStreamInterface * stream)` 





#### `public virtual void onRemoveRemoteStream(`[`PeerConnection`](#classscy_1_1PeerConnection)` * conn,webrtc::MediaStreamInterface * stream)` 





#### `public virtual void onStable(`[`PeerConnection`](#classscy_1_1PeerConnection)` * conn)` 





#### `public virtual void onClosed(`[`PeerConnection`](#classscy_1_1PeerConnection)` * conn)` 





#### `public virtual void onFailure(`[`PeerConnection`](#classscy_1_1PeerConnection)` * conn,const std::string & error)` 





#### `public webrtc::PeerConnectionFactoryInterface * factory() const` 





#### `protected rtc::scoped_refptr< webrtc::PeerConnectionFactoryInterface > _factory` 





# class `scy::StreamRecorder` 

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

#### `public  StreamRecorder(const `[`av::EncoderOptions`](#structscy_1_1av_1_1EncoderOptions)` & options)` 





#### `public  ~StreamRecorder()` 





#### `public void setVideoTrack(webrtc::VideoTrackInterface * track)` 





#### `public void setAudioTrack(webrtc::AudioTrackInterface * track)` 





#### `public void onPacketEncoded(`[`av::MediaPacket`](#structscy_1_1av_1_1MediaPacket)` & packet)` 





#### `public void OnFrame(const cricket::VideoFrame & frame)` 

VideoSinkInterface implementation.



#### `public void OnData(const void * audio_data,int bits_per_sample,int sample_rate,size_t number_of_channels,size_t number_of_frames)` 

AudioTrackSinkInterface implementation.



#### `protected av::MultiplexEncoder _encoder` 





#### `protected rtc::scoped_refptr< webrtc::VideoTrackInterface > _videoTrack` 





#### `protected rtc::scoped_refptr< webrtc::AudioTrackInterface > _audioTrack` 





#### `protected bool _awaitingVideo` 





#### `protected bool _awaitingAudio` 





# class `scy::VideoPacketSource` 

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

#### `public  VideoPacketSource()` 





#### `public virtual  ~VideoPacketSource()` 





#### `public virtual cricket::CaptureState Start(const cricket::VideoFormat & capture_format)` 

cricket::VideoCapturer implementation.



#### `public virtual void Stop()` 





#### `public virtual bool IsRunning()` 





#### `public virtual bool GetPreferredFourccs(std::vector< uint32_t > * fourccs)` 





#### `public virtual bool GetBestCaptureFormat(const cricket::VideoFormat & desired,cricket::VideoFormat * best_format)` 





#### `public virtual bool IsScreencast() const` 





#### `public void onVideoCaptured(void * sender,`[`av::VideoPacket`](#structscy_1_1av_1_1VideoPacket)` & packet)` 





