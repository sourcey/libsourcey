# Module <!-- group --> `webrtc`

The `webrtc` module contains WebRTC integrations.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`namespace `[`scy::wrtc`](#namespacescy_1_1wrtc)    | 
# namespace `scy::wrtc` 



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[`scy::wrtc::DummySetSessionDescriptionObserver`](#classscy_1_1wrtc_1_1DummySetSessionDescriptionObserver)    | 
`class `[`scy::wrtc::FakeAudioDeviceModule`](#classscy_1_1wrtc_1_1FakeAudioDeviceModule)    | This class implements a fake `AudioDeviceModule` that does absolutely nothing.
`class `[`scy::wrtc::FFmpeg420Buffer`](#classscy_1_1wrtc_1_1FFmpeg420Buffer)    | 
`class `[`scy::wrtc::ImageSequenceRecorder`](#classscy_1_1wrtc_1_1ImageSequenceRecorder)    | 
`class `[`scy::wrtc::Peer`](#classscy_1_1wrtc_1_1Peer)    | 
`class `[`scy::wrtc::PeerFactoryContext`](#classscy_1_1wrtc_1_1PeerFactoryContext)    | 
`class `[`scy::wrtc::PeerManager`](#classscy_1_1wrtc_1_1PeerManager)    | 
`class `[`scy::wrtc::StreamingPeerConnection`](#classscy_1_1wrtc_1_1StreamingPeerConnection)    | 
`class `[`scy::wrtc::YuvFrameGenerator`](#classscy_1_1wrtc_1_1YuvFrameGenerator)    | 
`class `[`scy::wrtc::YuvVideoCapturer`](#classscy_1_1wrtc_1_1YuvVideoCapturer)    | 
# class `scy::wrtc::DummySetSessionDescriptionObserver` 

```
class scy::wrtc::DummySetSessionDescriptionObserver
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





# class `scy::wrtc::FakeAudioDeviceModule` 

```
class scy::wrtc::FakeAudioDeviceModule
  : public AudioDeviceModule
  : public MessageHandler
```  

This class implements a fake `AudioDeviceModule` that does absolutely nothing.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
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
`public void OnMessage(rtc::Message * msg)` | End of functions inherited from `webrtc::AudioDeviceModule`.
`protected  explicit FakeAudioDeviceModule()` | 
`protected virtual  ~FakeAudioDeviceModule()` | 

## Members

#### `public int64_t TimeUntilNextProcess()` 



Following functions are inherited from `webrtc::AudioDeviceModule`. Only functions called by `[Peer](#classscy_1_1wrtc_1_1Peer)` are implemented, the rest do nothing and return success. If a function is not expected to be called by `[Peer](#classscy_1_1wrtc_1_1Peer)` an assertion is triggered if it is in fact called.

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

End of functions inherited from `webrtc::AudioDeviceModule`.

The following function is inherited from `rtc::MessageHandler`.

#### `protected  explicit FakeAudioDeviceModule()` 



The constructor is protected because the class needs to be created as a reference counted object (for memory managment reasons). It could be exposed in which case the burden of proper instantiation would be put on the creator of a [FakeAudioDeviceModule](#classscy_1_1wrtc_1_1FakeAudioDeviceModule) instance. To create an instance of this class use the `[Create()](#group__webrtc_1gabca7cbc7b427f7e5dd8c55584f0848da)` API.

#### `protected virtual  ~FakeAudioDeviceModule()` 



The destructor is protected because it is reference counted and should not be deleted directly.

# class `scy::wrtc::FFmpeg420Buffer` 

```
class scy::wrtc::FFmpeg420Buffer
  : public VideoFrameBuffer
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::vector< uint8_t > ydata` | 
`public std::vector< uint8_t > udata` | 
`public std::vector< uint8_t > vdata` | 
`public  FFmpeg420Buffer(int width,int height,const uint8_t * y_plane,int y_stride,const uint8_t * u_plane,int u_stride,const uint8_t * v_plane,int v_stride)` | 
`public  FFmpeg420Buffer(const `[`av::PlanarVideoPacket`](#structscy_1_1av_1_1PlanarVideoPacket)` & packet)` | 
`public int width() const` | 
`public int height() const` | 
`public const uint8_t * DataY() const` | 
`public const uint8_t * DataU() const` | 
`public const uint8_t * DataV() const` | 
`public int StrideY() const` | 
`public int StrideU() const` | 
`public int StrideV() const` | 
`public void * native_handle() const` | 
`public rtc::scoped_refptr< webrtc::VideoFrameBuffer > NativeToI420Buffer()` | 

## Members

#### `public std::vector< uint8_t > ydata` 





#### `public std::vector< uint8_t > udata` 





#### `public std::vector< uint8_t > vdata` 





#### `public  FFmpeg420Buffer(int width,int height,const uint8_t * y_plane,int y_stride,const uint8_t * u_plane,int u_stride,const uint8_t * v_plane,int v_stride)` 





#### `public  FFmpeg420Buffer(const `[`av::PlanarVideoPacket`](#structscy_1_1av_1_1PlanarVideoPacket)` & packet)` 





#### `public int width() const` 





#### `public int height() const` 





#### `public const uint8_t * DataY() const` 





#### `public const uint8_t * DataU() const` 





#### `public const uint8_t * DataV() const` 





#### `public int StrideY() const` 





#### `public int StrideU() const` 





#### `public int StrideV() const` 





#### `public void * native_handle() const` 





#### `public rtc::scoped_refptr< webrtc::VideoFrameBuffer > NativeToI420Buffer()` 





# class `scy::wrtc::ImageSequenceRecorder` 

```
class scy::wrtc::ImageSequenceRecorder
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





# class `scy::wrtc::Peer` 

```
class scy::wrtc::Peer
  : public PeerConnectionObserver
  : public CreateSessionDescriptionObserver
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Peer(`[`PeerManager`](#classscy_1_1wrtc_1_1PeerManager)` * manager,`[`PeerFactoryContext`](#classscy_1_1wrtc_1_1PeerFactoryContext)` * context,const std::string & peerid,const std::string & token,`[`Mode`](#group__webrtc_1ga0a5220c6e568323fb5dfb9769e016a20)` mode)` | 
`public virtual  ~Peer()` | 
`public virtual rtc::scoped_refptr< webrtc::MediaStreamInterface > createMediaStream()` | 
`public virtual void createConnection()` | 
`public virtual void closeConnection()` | Close the peer connection.
`public virtual void createOffer()` | 
`public virtual void recvSDP(const std::string & type,const std::string & sdp)` | Receive a remote offer or answer.
`public virtual void recvCandidate(const std::string & mid,int mlineindex,const std::string & sdp)` | Receive a remote candidate.
`public void setPeerFactory(rtc::scoped_refptr< webrtc::PeerConnectionFactoryInterface > factory)` | 
`public void setPortRange(int minPort,int maxPort)` | 
`public std::string peerid() const` | 
`public std::string token() const` | 
`public webrtc::FakeConstraints & constraints()` | 
`public webrtc::PeerConnectionFactoryInterface * factory() const` | 
`public rtc::scoped_refptr< webrtc::PeerConnectionInterface > peerConnection() const` | 
`public rtc::scoped_refptr< webrtc::MediaStreamInterface > stream() const` | 
`protected `[`PeerManager`](./doc/api-webrtc.md#classscy_1_1wrtc_1_1PeerManager)` * _manager` | 
`protected `[`PeerFactoryContext`](./doc/api-webrtc.md#classscy_1_1wrtc_1_1PeerFactoryContext)` * _context` | 
`protected std::string _peerid` | 
`protected std::string _token` | 
`protected `[`Mode`](#group__webrtc_1ga0a5220c6e568323fb5dfb9769e016a20)` _mode` | 
`protected webrtc::PeerConnectionInterface::RTCConfiguration _config` | 
`protected webrtc::FakeConstraints _constraints` | 
`protected rtc::scoped_refptr< webrtc::PeerConnectionInterface > _peerConnection` | 
`protected rtc::scoped_refptr< webrtc::MediaStreamInterface > _stream` | 
`protected std::unique_ptr< cricket::BasicPortAllocator > _portAllocator` | 
`protected virtual void OnAddStream(webrtc::MediaStreamInterface * stream)` | inherited from PeerConnectionObserver
`protected virtual void OnRemoveStream(webrtc::MediaStreamInterface * stream)` | 
`protected virtual void OnAddStream(rtc::scoped_refptr< webrtc::MediaStreamInterface > stream)` | since 7f0676
`protected virtual void OnRemoveStream(rtc::scoped_refptr< webrtc::MediaStreamInterface > stream)` | since 7f0676
`protected virtual void OnDataChannel(rtc::scoped_refptr< webrtc::DataChannelInterface > stream)` | since 7f0676
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

#### `public  Peer(`[`PeerManager`](#classscy_1_1wrtc_1_1PeerManager)` * manager,`[`PeerFactoryContext`](#classscy_1_1wrtc_1_1PeerFactoryContext)` * context,const std::string & peerid,const std::string & token,`[`Mode`](#group__webrtc_1ga0a5220c6e568323fb5dfb9769e016a20)` mode)` 





#### `public virtual  ~Peer()` 





#### `public virtual rtc::scoped_refptr< webrtc::MediaStreamInterface > createMediaStream()` 



Create the local media stream. Only necessary when we are creating the offer.

#### `public virtual void createConnection()` 



Create the peer connection once configuration, constraints and streams have been created.

#### `public virtual void closeConnection()` 

Close the peer connection.



#### `public virtual void createOffer()` 



Create the offer SDP tos end to the peer. No offer should be received after creating the offer. A call to `recvSDP` with answer is expected in order to initiate the session.

#### `public virtual void recvSDP(const std::string & type,const std::string & sdp)` 

Receive a remote offer or answer.



#### `public virtual void recvCandidate(const std::string & mid,int mlineindex,const std::string & sdp)` 

Receive a remote candidate.



#### `public void setPeerFactory(rtc::scoped_refptr< webrtc::PeerConnectionFactoryInterface > factory)` 



Set a custom PeerFactory object. Must be set before any streams are initiated.

#### `public void setPortRange(int minPort,int maxPort)` 



Set the port range for WebRTC connections. Must be done before the connection is initiated.

#### `public std::string peerid() const` 





#### `public std::string token() const` 





#### `public webrtc::FakeConstraints & constraints()` 





#### `public webrtc::PeerConnectionFactoryInterface * factory() const` 





#### `public rtc::scoped_refptr< webrtc::PeerConnectionInterface > peerConnection() const` 





#### `public rtc::scoped_refptr< webrtc::MediaStreamInterface > stream() const` 





#### `protected `[`PeerManager`](./doc/api-webrtc.md#classscy_1_1wrtc_1_1PeerManager)` * _manager` 





#### `protected `[`PeerFactoryContext`](./doc/api-webrtc.md#classscy_1_1wrtc_1_1PeerFactoryContext)` * _context` 





#### `protected std::string _peerid` 





#### `protected std::string _token` 





#### `protected `[`Mode`](#group__webrtc_1ga0a5220c6e568323fb5dfb9769e016a20)` _mode` 





#### `protected webrtc::PeerConnectionInterface::RTCConfiguration _config` 





#### `protected webrtc::FakeConstraints _constraints` 





#### `protected rtc::scoped_refptr< webrtc::PeerConnectionInterface > _peerConnection` 





#### `protected rtc::scoped_refptr< webrtc::MediaStreamInterface > _stream` 





#### `protected std::unique_ptr< cricket::BasicPortAllocator > _portAllocator` 





#### `protected virtual void OnAddStream(webrtc::MediaStreamInterface * stream)` 

inherited from PeerConnectionObserver

> Deprecated:

#### `protected virtual void OnRemoveStream(webrtc::MediaStreamInterface * stream)` 



> Deprecated:

#### `protected virtual void OnAddStream(rtc::scoped_refptr< webrtc::MediaStreamInterface > stream)` 

since 7f0676



#### `protected virtual void OnRemoveStream(rtc::scoped_refptr< webrtc::MediaStreamInterface > stream)` 

since 7f0676



#### `protected virtual void OnDataChannel(rtc::scoped_refptr< webrtc::DataChannelInterface > stream)` 

since 7f0676



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





# class `scy::wrtc::PeerFactoryContext` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::unique_ptr< rtc::Thread > networkThread` | 
`public std::unique_ptr< rtc::Thread > workerThread` | 
`public std::unique_ptr< rtc::NetworkManager > networkManager` | 
`public std::unique_ptr< rtc::PacketSocketFactory > socketFactory` | 
`public rtc::scoped_refptr< webrtc::PeerConnectionFactoryInterface > factory` | 
`public  PeerFactoryContext(webrtc::AudioDeviceModule * default_adm,cricket::WebRtcVideoEncoderFactory * video_encoder_factory,cricket::WebRtcVideoDecoderFactory * video_decoder_factory,rtc::scoped_refptr< webrtc::AudioEncoderFactory > audio_encoder_factory,rtc::scoped_refptr< webrtc::AudioDecoderFactory > audio_decoder_factory)` | 
`public void initCustomNetworkManager()` | 

## Members

#### `public std::unique_ptr< rtc::Thread > networkThread` 





#### `public std::unique_ptr< rtc::Thread > workerThread` 





#### `public std::unique_ptr< rtc::NetworkManager > networkManager` 





#### `public std::unique_ptr< rtc::PacketSocketFactory > socketFactory` 





#### `public rtc::scoped_refptr< webrtc::PeerConnectionFactoryInterface > factory` 





#### `public  PeerFactoryContext(webrtc::AudioDeviceModule * default_adm,cricket::WebRtcVideoEncoderFactory * video_encoder_factory,cricket::WebRtcVideoDecoderFactory * video_decoder_factory,rtc::scoped_refptr< webrtc::AudioEncoderFactory > audio_encoder_factory,rtc::scoped_refptr< webrtc::AudioDecoderFactory > audio_decoder_factory)` 





#### `public void initCustomNetworkManager()` 





# class `scy::wrtc::PeerManager` 

```
class scy::wrtc::PeerManager
  : public scy::PointerCollection< std::string, Peer >
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  PeerManager()` | 
`public virtual  ~PeerManager()` | 
`public virtual void sendSDP(`[`Peer`](#classscy_1_1wrtc_1_1Peer)` * conn,const std::string & type,const std::string & sdp)` | 
`public virtual void sendCandidate(`[`Peer`](#classscy_1_1wrtc_1_1Peer)` * conn,const std::string & mid,int mlineindex,const std::string & sdp)` | 
`public virtual void recvSDP(const std::string & token,const json::value & data)` | 
`public virtual void recvCandidate(const std::string & token,const json::value & data)` | 
`public virtual void onAddRemoteStream(`[`Peer`](#classscy_1_1wrtc_1_1Peer)` * conn,webrtc::MediaStreamInterface * stream)` | 
`public virtual void onRemoveRemoteStream(`[`Peer`](#classscy_1_1wrtc_1_1Peer)` * conn,webrtc::MediaStreamInterface * stream)` | 
`public virtual void onStable(`[`Peer`](#classscy_1_1wrtc_1_1Peer)` * conn)` | 
`public virtual void onClosed(`[`Peer`](#classscy_1_1wrtc_1_1Peer)` * conn)` | 
`public virtual void onFailure(`[`Peer`](#classscy_1_1wrtc_1_1Peer)` * conn,const std::string & error)` | 

## Members

#### `public  PeerManager()` 





#### `public virtual  ~PeerManager()` 





#### `public virtual void sendSDP(`[`Peer`](#classscy_1_1wrtc_1_1Peer)` * conn,const std::string & type,const std::string & sdp)` 





#### `public virtual void sendCandidate(`[`Peer`](#classscy_1_1wrtc_1_1Peer)` * conn,const std::string & mid,int mlineindex,const std::string & sdp)` 





#### `public virtual void recvSDP(const std::string & token,const json::value & data)` 





#### `public virtual void recvCandidate(const std::string & token,const json::value & data)` 





#### `public virtual void onAddRemoteStream(`[`Peer`](#classscy_1_1wrtc_1_1Peer)` * conn,webrtc::MediaStreamInterface * stream)` 





#### `public virtual void onRemoveRemoteStream(`[`Peer`](#classscy_1_1wrtc_1_1Peer)` * conn,webrtc::MediaStreamInterface * stream)` 





#### `public virtual void onStable(`[`Peer`](#classscy_1_1wrtc_1_1Peer)` * conn)` 





#### `public virtual void onClosed(`[`Peer`](#classscy_1_1wrtc_1_1Peer)` * conn)` 





#### `public virtual void onFailure(`[`Peer`](#classscy_1_1wrtc_1_1Peer)` * conn,const std::string & error)` 





# class `scy::wrtc::StreamingPeerConnection` 

```
class scy::wrtc::StreamingPeerConnection
  : public PeerConnection
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  StreamingPeerConnection(PeerConnectionManager * manager,const std::string & peerid,const std::string & token,const std::string & file)` | Create the streaming peer connection.
`public virtual  ~StreamingPeerConnection()` | 
`public virtual rtc::scoped_refptr< webrtc::MediaStreamInterface > createMediaStream()` | 
`public virtual void OnIcePeerChange(webrtc::PeerConnectionInterface::IcePeerState new_state)` | 
`protected std::string _file` | 
`protected MultiplexMediaCapturer _capturer` | 
`protected std::unique_ptr< rtc::Thread > _networkThread` | 
`protected std::unique_ptr< rtc::Thread > _workerThread` | 

## Members

#### `public  StreamingPeerConnection(PeerConnectionManager * manager,const std::string & peerid,const std::string & token,const std::string & file)` 

Create the streaming peer connection.



#### `public virtual  ~StreamingPeerConnection()` 





#### `public virtual rtc::scoped_refptr< webrtc::MediaStreamInterface > createMediaStream()` 





#### `public virtual void OnIcePeerChange(webrtc::PeerConnectionInterface::IcePeerState new_state)` 





#### `protected std::string _file` 





#### `protected MultiplexMediaCapturer _capturer` 





#### `protected std::unique_ptr< rtc::Thread > _networkThread` 





#### `protected std::unique_ptr< rtc::Thread > _workerThread` 





# class `scy::wrtc::YuvFrameGenerator` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  YuvFrameGenerator(int width,int height,bool enable_barcode)` | 
`public  ~YuvFrameGenerator()` | 
`public void GenerateNextFrame(int32_t barcode_value)` | 
`public void CopyInterleavedData(uint8_t * frame_buffer)` | 
`public const uint8_t * DataY() const` | 
`public const uint8_t * DataU() const` | 
`public const uint8_t * DataV() const` | 
`public inline int GetFrameSize()` | 
`public inline int GetHeight()` | 
`public inline int GetWidth()` | 
`public void GetBarcodeBounds(int * top,int * left,int * width,int * height)` | 

## Members

#### `public  YuvFrameGenerator(int width,int height,bool enable_barcode)` 





#### `public  ~YuvFrameGenerator()` 





#### `public void GenerateNextFrame(int32_t barcode_value)` 





#### `public void CopyInterleavedData(uint8_t * frame_buffer)` 





#### `public const uint8_t * DataY() const` 





#### `public const uint8_t * DataU() const` 





#### `public const uint8_t * DataV() const` 





#### `public inline int GetFrameSize()` 





#### `public inline int GetHeight()` 





#### `public inline int GetWidth()` 





#### `public void GetBarcodeBounds(int * top,int * left,int * width,int * height)` 





# class `scy::wrtc::YuvVideoCapturer` 

```
class scy::wrtc::YuvVideoCapturer
  : public VideoCapturer
  : public Thread
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  YuvVideoCapturer()` | 
`public inline virtual  ~YuvVideoCapturer()` | 
`public inline void SignalFrameCapturedOnStartThread()` | 
`public inline void Run()` | 
`public inline virtual cricket::CaptureState Start(const cricket::VideoFormat & format)` | 
`public inline virtual void Stop()` | 
`public inline virtual bool GetPreferredFourccs(std::vector< uint32_t > * fourccs)` | 
`public inline virtual bool IsScreencast() const` | 
`public inline virtual bool IsRunning()` | 

## Members

#### `public inline  YuvVideoCapturer()` 





#### `public inline virtual  ~YuvVideoCapturer()` 





#### `public inline void SignalFrameCapturedOnStartThread()` 





#### `public inline void Run()` 





#### `public inline virtual cricket::CaptureState Start(const cricket::VideoFormat & format)` 





#### `public inline virtual void Stop()` 





#### `public inline virtual bool GetPreferredFourccs(std::vector< uint32_t > * fourccs)` 





#### `public inline virtual bool IsScreencast() const` 





#### `public inline virtual bool IsRunning()` 





