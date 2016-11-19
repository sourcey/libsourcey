# Module <!-- group --> `av`



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`namespace `[`scy::av`](#namespacescy_1_1av)    | 
`namespace `[`scy::av::legacy`](#namespacescy_1_1av_1_1legacy)    | 
# namespace `scy::av` {#namespacescy_1_1av}



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[`scy::av::DeviceManager`](#classscy_1_1av_1_1DeviceManager)    | 
`class `[`scy::av::DeviceManagerFactory`](#classscy_1_1av_1_1DeviceManagerFactory)    | 
`class `[`scy::av::DeviceWatcher`](#classscy_1_1av_1_1DeviceWatcher)    | 
`class `[`scy::av::FakeDeviceManager`](#classscy_1_1av_1_1FakeDeviceManager)    | 
`class `[`scy::av::FLVMetadataInjector`](#classscy_1_1av_1_1FLVMetadataInjector)    | 
`class `[`scy::av::FormatRegistry`](#classscy_1_1av_1_1FormatRegistry)    | 
`class `[`scy::av::FPSCounter`](#classscy_1_1av_1_1FPSCounter)    | An FPS counter based on the simple moving average (SMA) algorithm.
`class `[`scy::av::FPSLimiter`](#classscy_1_1av_1_1FPSLimiter)    | 
`class `[`scy::av::ICapture`](#classscy_1_1av_1_1ICapture)    | 
`class `[`scy::av::IDeviceManager`](#classscy_1_1av_1_1IDeviceManager)    | 
`class `[`scy::av::IEncoder`](#classscy_1_1av_1_1IEncoder)    | This is the abstract class for all encoders.
`class `[`scy::av::LinuxDeviceManager`](#classscy_1_1av_1_1LinuxDeviceManager)    | 
`class `[`scy::av::MacDeviceManager`](#classscy_1_1av_1_1MacDeviceManager)    | 
`class `[`scy::av::MacDeviceWatcher`](#classscy_1_1av_1_1MacDeviceWatcher)    | 
`class `[`scy::av::RealtimePacketQueue`](#classscy_1_1av_1_1RealtimePacketQueue)    | 
`struct `[`scy::av::AudioCodec`](#structscy_1_1av_1_1AudioCodec)    | 
`struct `[`scy::av::AudioPacket`](#structscy_1_1av_1_1AudioPacket)    | 
`struct `[`scy::av::Codec`](#structscy_1_1av_1_1Codec)    | 
`struct `[`scy::av::Device`](#structscy_1_1av_1_1Device)    | Represents a system audio, video or render device.
`struct `[`scy::av::EncoderOptions`](#structscy_1_1av_1_1EncoderOptions)    | 
`struct `[`scy::av::EncoderState`](#structscy_1_1av_1_1EncoderState)    | 
`struct `[`scy::av::Format`](#structscy_1_1av_1_1Format)    | 
`struct `[`scy::av::MediaPacket`](#structscy_1_1av_1_1MediaPacket)    | 
`struct `[`scy::av::VideoCodec`](#structscy_1_1av_1_1VideoCodec)    | 
`struct `[`scy::av::VideoPacket`](#structscy_1_1av_1_1VideoPacket)    | 
# class `scy::av::DeviceManager` {#classscy_1_1av_1_1DeviceManager}

```
class scy::av::DeviceManager
  : public scy::av::IDeviceManager
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  DeviceManager()` | 
`public virtual  ~DeviceManager()` | 
`public virtual bool initialize()` | Initialization.
`public virtual void uninitialize()` | 
`public virtual int getCapabilities()` | Capabilities.
`public virtual bool getAudioInputDevices(std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devices)` | [Device](./doc/api-av.md#structscy_1_1av_1_1Device) enumeration.
`public virtual bool getAudioOutputDevices(std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devices)` | 
`public virtual bool getAudioInputDevice(`[`Device`](#structscy_1_1av_1_1Device)` & out,const std::string & name,int id)` | 
`public virtual bool getAudioInputDevice(`[`Device`](#structscy_1_1av_1_1Device)` & out,int id)` | 
`public virtual bool getAudioOutputDevice(`[`Device`](#structscy_1_1av_1_1Device)` & out,const std::string & name,int id)` | 
`public virtual bool getAudioOutputDevice(`[`Device`](#structscy_1_1av_1_1Device)` & out,int id)` | 
`public virtual bool getVideoCaptureDevices(std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devs)` | 
`public virtual bool getVideoCaptureDevice(`[`Device`](#structscy_1_1av_1_1Device)` & out,const std::string & name,int id)` | 
`public virtual bool getVideoCaptureDevice(`[`Device`](#structscy_1_1av_1_1Device)` & out,int id)` | Returns the video capture device at the given system index.
`public virtual bool getDefaultAudioInputDevice(`[`Device`](#structscy_1_1av_1_1Device)` & device)` | 
`public virtual bool getDefaultAudioOutputDevice(`[`Device`](#structscy_1_1av_1_1Device)` & device)` | 
`public virtual bool getDefaultVideoCaptureDevice(`[`Device`](#structscy_1_1av_1_1Device)` & device)` | 
`public inline bool initialized() const` | 
`public virtual void print(std::ostream & ost)` | 
`protected virtual bool getDefaultAudioDevice(bool input,`[`Device`](#structscy_1_1av_1_1Device)` & device)` | 
`protected virtual bool getAudioDevices(bool input,std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devs)` | 
`protected virtual bool getAudioDevice(bool input,`[`Device`](#structscy_1_1av_1_1Device)` & out,const std::string & name,int id)` | 
`protected virtual bool getAudioDevice(bool input,`[`Device`](#structscy_1_1av_1_1Device)` & out,int id)` | 
`protected void setInitialized(bool initialized)` | 
`protected void setWatcher(`[`DeviceWatcher`](#classscy_1_1av_1_1DeviceWatcher)` * watcher)` | 
`protected `[`DeviceWatcher`](#classscy_1_1av_1_1DeviceWatcher)` * watcher()` | 

## Members

#### `public  DeviceManager()` {#group__av_1ga778c03d9b552d40b34bb8f8f9a1255d8}





#### `public virtual  ~DeviceManager()` {#group__av_1ga3635ac39fa55df574dcb0d98634c8140}





#### `public virtual bool initialize()` {#group__av_1gabe487635b243c75fae6d1ce1700f1def}

Initialization.



#### `public virtual void uninitialize()` {#group__av_1gae2fcfc68cc20dedeaa8ce913307faafc}





#### `public virtual int getCapabilities()` {#group__av_1ga42f596484bd7234fd92becaac4005e53}

Capabilities.



#### `public virtual bool getAudioInputDevices(std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devices)` {#group__av_1gafeeaa53b13b8537cfbe903a5a26b890b}

[Device](#structscy_1_1av_1_1Device) enumeration.



#### `public virtual bool getAudioOutputDevices(std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devices)` {#group__av_1ga2b4cd27667bb3e529de2ff4f3e3fc289}





#### `public virtual bool getAudioInputDevice(`[`Device`](#structscy_1_1av_1_1Device)` & out,const std::string & name,int id)` {#group__av_1gae37c4a33994f7ef8416490531d3d0907}





#### `public virtual bool getAudioInputDevice(`[`Device`](#structscy_1_1av_1_1Device)` & out,int id)` {#group__av_1ga49d30065d7ecc80f9b768a47868ff505}





#### `public virtual bool getAudioOutputDevice(`[`Device`](#structscy_1_1av_1_1Device)` & out,const std::string & name,int id)` {#group__av_1ga4cb9cac44ba585b23b8b07b3ab33dbb1}





#### `public virtual bool getAudioOutputDevice(`[`Device`](#structscy_1_1av_1_1Device)` & out,int id)` {#group__av_1ga115cb239e0244a5d40baf2a6239aec2e}





#### `public virtual bool getVideoCaptureDevices(std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devs)` {#group__av_1ga70c7a7ff64734a32709d26e345398c52}





#### `public virtual bool getVideoCaptureDevice(`[`Device`](#structscy_1_1av_1_1Device)` & out,const std::string & name,int id)` {#group__av_1ga2820413e3299477fb40fd84aebbf7ec8}





#### `public virtual bool getVideoCaptureDevice(`[`Device`](#structscy_1_1av_1_1Device)` & out,int id)` {#group__av_1gae4e7d0d99842dc18d5fef8536603ca19}

Returns the video capture device at the given system index.



#### `public virtual bool getDefaultAudioInputDevice(`[`Device`](#structscy_1_1av_1_1Device)` & device)` {#group__av_1gaa87c5fbc8299cf4a82e192d69977b16e}





#### `public virtual bool getDefaultAudioOutputDevice(`[`Device`](#structscy_1_1av_1_1Device)` & device)` {#group__av_1gab0bbdde6cab23808166819aafc305aa8}





#### `public virtual bool getDefaultVideoCaptureDevice(`[`Device`](#structscy_1_1av_1_1Device)` & device)` {#group__av_1gac994fdb7f9415bde77421efc8cd84e7f}





#### `public inline bool initialized() const` {#group__av_1gaabba96ca78dc27e34ccd588ab813f8dc}





#### `public virtual void print(std::ostream & ost)` {#group__av_1ga485b4609f4a69f5d603b96deb411b306}





#### `protected virtual bool getDefaultAudioDevice(bool input,`[`Device`](#structscy_1_1av_1_1Device)` & device)` {#group__av_1gae9e2effb9514f16af8a0654cf59b2bf1}





#### `protected virtual bool getAudioDevices(bool input,std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devs)` {#group__av_1ga04e3b90e4bac6ad1bb03d00ba8337b6e}





#### `protected virtual bool getAudioDevice(bool input,`[`Device`](#structscy_1_1av_1_1Device)` & out,const std::string & name,int id)` {#group__av_1ga95c8c01f83b7e0b5450892be78e031eb}





#### `protected virtual bool getAudioDevice(bool input,`[`Device`](#structscy_1_1av_1_1Device)` & out,int id)` {#group__av_1gabbe9f702e0082b2fc23f159bedf4fcfc}





#### `protected void setInitialized(bool initialized)` {#group__av_1gab6cf6a072abd179483f6f7d484f796e4}





#### `protected void setWatcher(`[`DeviceWatcher`](#classscy_1_1av_1_1DeviceWatcher)` * watcher)` {#group__av_1ga90edc6121010b8344ebd6a9b8606fdf1}





#### `protected `[`DeviceWatcher`](#classscy_1_1av_1_1DeviceWatcher)` * watcher()` {#group__av_1ga29bc8a0cfc10633446ae7b9dae73bb0e}





# class `scy::av::DeviceManagerFactory` {#classscy_1_1av_1_1DeviceManagerFactory}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------

## Members

# class `scy::av::DeviceWatcher` {#classscy_1_1av_1_1DeviceWatcher}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  explicit DeviceWatcher(`[`IDeviceManager`](#classscy_1_1av_1_1IDeviceManager)` *)` | 
`public inline virtual  ~DeviceWatcher()` | 
`public inline virtual bool start()` | 
`public inline virtual void stop()` | 

## Members

#### `public inline  explicit DeviceWatcher(`[`IDeviceManager`](#classscy_1_1av_1_1IDeviceManager)` *)` {#group__av_1ga66ffcd817f7ac8bc0a965e018271508c}





#### `public inline virtual  ~DeviceWatcher()` {#group__av_1ga191ccb64f6d1985e762bfdd54bb55bba}





#### `public inline virtual bool start()` {#group__av_1gab833459b9cf8c1cde8147159f265f7d8}





#### `public inline virtual void stop()` {#group__av_1ga028347525717f62dfccf5bb82031fe79}





# class `scy::av::FakeDeviceManager` {#classscy_1_1av_1_1FakeDeviceManager}

```
class scy::av::FakeDeviceManager
  : public scy::av::IDeviceManager
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  FakeDeviceManager()` | 
`public inline virtual bool initialize()` | Initialization.
`public inline virtual void uninitialize()` | 
`public inline virtual int getCapabilities()` | Capabilities.
`public inline virtual bool getMicrophones(std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devs)` | 
`public inline virtual bool getSpeakers(std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devs)` | 
`public inline virtual bool getMicrophone(const std::string & name,`[`Device`](#structscy_1_1av_1_1Device)` & out)` | 
`public inline virtual bool getSpeaker(const std::string & name,`[`Device`](#structscy_1_1av_1_1Device)` & out)` | 
`public inline virtual bool getCameras(std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devs)` | 
`public inline virtual bool getDefaultCamera(`[`Device`](#structscy_1_1av_1_1Device)` & device)` | 
`public inline void setMicrophones(const std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devices)` | 
`public inline void setSpeakers(const std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devices)` | 
`public inline void setCameras(const std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devices)` | 
`public inline virtual bool getCamera(const std::string & name,`[`Device`](#structscy_1_1av_1_1Device)` & out)` | 
`public inline bool getAudioDevice(bool input,const std::string & name,`[`Device`](#structscy_1_1av_1_1Device)` & out)` | 

## Members

#### `public inline  FakeDeviceManager()` {#group__av_1ga006594e6f296225b135e550be80dcb13}





#### `public inline virtual bool initialize()` {#group__av_1ga3f83116598bfa3b3d53a4c4c25cf1cee}

Initialization.



#### `public inline virtual void uninitialize()` {#group__av_1ga1ba541135e1a1c0039ade4343403a25b}





#### `public inline virtual int getCapabilities()` {#group__av_1gaa72540c5d7eb2f07ad1317944cd28565}

Capabilities.



#### `public inline virtual bool getMicrophones(std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devs)` {#group__av_1gaed7a9dee4d3dffe342c52d5d1edbebeb}





#### `public inline virtual bool getSpeakers(std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devs)` {#group__av_1ga89c33c93e68bc6bb55945952b8261f4e}





#### `public inline virtual bool getMicrophone(const std::string & name,`[`Device`](#structscy_1_1av_1_1Device)` & out)` {#group__av_1gabc947f13611657bf38e64b901449aa1d}





#### `public inline virtual bool getSpeaker(const std::string & name,`[`Device`](#structscy_1_1av_1_1Device)` & out)` {#group__av_1ga4d7895da9bbbcc0c57480d59c4c45dc4}





#### `public inline virtual bool getCameras(std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devs)` {#group__av_1gad2513f312a81350e6b936fc33ad21405}





#### `public inline virtual bool getDefaultCamera(`[`Device`](#structscy_1_1av_1_1Device)` & device)` {#group__av_1gaf3aaf6ebb5675f70bc0cc3acd70875ba}





#### `public inline void setMicrophones(const std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devices)` {#group__av_1gac6f0eef0d4653d05d1ed39ff61c3e775}





#### `public inline void setSpeakers(const std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devices)` {#group__av_1ga063298c41c2f469d96614f1c19d9cd4e}





#### `public inline void setCameras(const std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devices)` {#group__av_1ga7d8d1af543bbb57550820afe9d6d7b2f}





#### `public inline virtual bool getCamera(const std::string & name,`[`Device`](#structscy_1_1av_1_1Device)` & out)` {#group__av_1ga285e406d330f5c6c0f6ab8cdde3fd634}





#### `public inline bool getAudioDevice(bool input,const std::string & name,`[`Device`](#structscy_1_1av_1_1Device)` & out)` {#group__av_1gaafe3fffef143106836168cc7ac4d0716}





# class `scy::av::FLVMetadataInjector` {#classscy_1_1av_1_1FLVMetadataInjector}

```
class scy::av::FLVMetadataInjector
  : public scy::PacketProcessor
```  



This class implements a packetizer which appends correct stream headers and modifies the timestamp of FLV packets so Adobe's Flash Player will play our videos mid-stream.

This adapter is useful for multicast situations where we don't have the option of restarting the encoder stream.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`PacketSignal`](#group__base_1ga3ffb5bda6133fac97f3bed2b567b9b6a)` emitter` | 
`public  IPacketizer(this-> emitter)` | 
`public  _format(format)` | 
`public  _initial(true)` | 
`public  _modifyingStream(false)` | 
`public  _waitingForKeyframe(false)` | 
`public inline  _timestampOffset(0) const` | 
`public inline virtual void onStreamStateChange(const `[`PacketStreamState`](#structscy_1_1PacketStreamState)` & state)` | whenever the stream is restarted.
`public inline virtual void process(`[`IPacket`](#classscy_1_1IPacket)` & packet)` | 
`public inline virtual void fastUpdateTimestamp(char * buf,std::uint32_t timestamp)` | 
`public inline virtual bool fastIsFLVHeader(char * buf)` | Caution: this method does not check buffer size.
`public inline virtual bool fastIsFLVKeyFrame(char * buf)` | Caution: this method does not check buffer size.
`public inline virtual void writeFLVHeader(`[`BitWriter`](#classscy_1_1BitWriter)` & writer)` | 
`public inline std::int64_t doubleToInt(double d)` | 
`public inline virtual void writeAMFSring(`[`BitWriter`](#classscy_1_1BitWriter)` & writer,const char * val)` | AMF Helpers.
`public inline virtual void writeAMFDouble(`[`BitWriter`](#classscy_1_1BitWriter)` & writer,double val)` | 
`public inline virtual void writeAMFBool(`[`BitWriter`](#classscy_1_1BitWriter)` & writer,bool val)` | 
`protected `[`Format`](./doc/api-av.md#structscy_1_1av_1_1Format)` _format` | 
`protected bool _initial` | 
`protected bool _modifyingStream` | 
`protected bool _waitingForKeyframe` | 
`protected std::uint32_t _timestampOffset` | 
`protected `[`legacy::FPSCounter`](./doc/api-av.md#structscy_1_1av_1_1legacy_1_1FPSCounter)` _fpsCounter` | 

## Members

#### `public `[`PacketSignal`](#group__base_1ga3ffb5bda6133fac97f3bed2b567b9b6a)` emitter` {#group__av_1ga8b361da8e252816c458d4987247a06c0}





#### `public  IPacketizer(this-> emitter)` {#group__av_1ga9f887b63bbd57a16ebea78f42352cf80}





#### `public  _format(format)` {#group__av_1ga00b49e6d76e5c12d79a8b31f7f68aa16}





#### `public  _initial(true)` {#group__av_1ga5341424127e2b262009062d53f14dd89}





#### `public  _modifyingStream(false)` {#group__av_1ga620bcff399fc9e599fdb134d7f164a30}





#### `public  _waitingForKeyframe(false)` {#group__av_1ga595e597a9cb20e8a558501f7bbeff7ed}





#### `public inline  _timestampOffset(0) const` {#group__av_1gae40bbe2c2eda197672f9fad8f13690cb}





#### `public inline virtual void onStreamStateChange(const `[`PacketStreamState`](#structscy_1_1PacketStreamState)` & state)` {#group__av_1gad92da5518ddd902a7136e83dde6ba503}

whenever the stream is restarted.

This method is called by the Packet [Stream](#classscy_1_1Stream)

#### `public inline virtual void process(`[`IPacket`](#classscy_1_1IPacket)` & packet)` {#group__av_1gaf7675aec070348fb52657d0bbc81264d}



This method performs processing on the given packet and emits the result.

Note: If packet processing is async (the packet is not in the current thread scope) then packet data must be copied. Copied data can be freed directly aFter the async call to emit() the outgoing packet.

#### `public inline virtual void fastUpdateTimestamp(char * buf,std::uint32_t timestamp)` {#group__av_1gac6bdf4094f0164d4557b501346fa3f47}



Updates the timestamp in the given FLV tag buffer. No more need to copy data with this method. Caution: this method does not check buffer size.

#### `public inline virtual bool fastIsFLVHeader(char * buf)` {#group__av_1gaff079b714922df3458c65ba3ef60a3f6}

Caution: this method does not check buffer size.



#### `public inline virtual bool fastIsFLVKeyFrame(char * buf)` {#group__av_1ga34a40b333d9f32b4ef94f3e3f028665a}

Caution: this method does not check buffer size.



#### `public inline virtual void writeFLVHeader(`[`BitWriter`](#classscy_1_1BitWriter)` & writer)` {#group__av_1ga6aa18b140ad46d0e7a2d4ac44afe3e06}





#### `public inline std::int64_t doubleToInt(double d)` {#group__av_1ga371b1182cbe6cf19c17d7add8cec0274}





#### `public inline virtual void writeAMFSring(`[`BitWriter`](#classscy_1_1BitWriter)` & writer,const char * val)` {#group__av_1ga28c5478abb189d041c739068082455c8}

AMF Helpers.



#### `public inline virtual void writeAMFDouble(`[`BitWriter`](#classscy_1_1BitWriter)` & writer,double val)` {#group__av_1gad544d5db35df80ebb048dec5f5f3a286}





#### `public inline virtual void writeAMFBool(`[`BitWriter`](#classscy_1_1BitWriter)` & writer,bool val)` {#group__av_1ga2aa49c8d0d9ccf3602b3fe7bbbfb1b1d}





#### `protected `[`Format`](./doc/api-av.md#structscy_1_1av_1_1Format)` _format` {#group__av_1gad846cf859e5cea50a984de6f9544b165}





#### `protected bool _initial` {#group__av_1ga1d940a770234cb5ad44e7622110f4fb0}





#### `protected bool _modifyingStream` {#group__av_1ga0d0de9ad89aeb3c708219b126397d703}





#### `protected bool _waitingForKeyframe` {#group__av_1ga85b7dc7527c9cce0db4af6440700ffa8}





#### `protected std::uint32_t _timestampOffset` {#group__av_1ga612c3a994d9cd6472c5a195215b94b4d}





#### `protected `[`legacy::FPSCounter`](./doc/api-av.md#structscy_1_1av_1_1legacy_1_1FPSCounter)` _fpsCounter` {#group__av_1ga73a76b5f863b967f49a85cdc8c72e433}





# class `scy::av::FormatRegistry` {#classscy_1_1av_1_1FormatRegistry}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  FormatRegistry()` | 
`public virtual  ~FormatRegistry()` | 
`public virtual `[`Format`](#structscy_1_1av_1_1Format)` & get(const std::string & name)` | 
`public virtual `[`Format`](#structscy_1_1av_1_1Format)` & getByID(const std::string & id)` | 
`public virtual `[`Format`](#structscy_1_1av_1_1Format)` & getOrDefault(const std::string & name)` | 
`public virtual `[`Format`](#structscy_1_1av_1_1Format)` & getDefault()` | 
`public virtual void registerFormat(const `[`Format`](#structscy_1_1av_1_1Format)` & format)` | 
`public virtual bool unregisterFormat(const std::string & name)` | 
`public virtual void setDefault(const std::string & name)` | Sets the default fallback media format.
`public virtual bool exists(const std::string & name)` | 
`public virtual void clear()` | 
`public virtual FormatList formats() const` | 

## Members

#### `public  FormatRegistry()` {#group__av_1gaaf5a5ede3c5909f579a34c56e8186f1c}





#### `public virtual  ~FormatRegistry()` {#group__av_1gac6f77454c59e8bce0a602d0973f82515}





#### `public virtual `[`Format`](#structscy_1_1av_1_1Format)` & get(const std::string & name)` {#group__av_1gabd3e9f2a811539d26a8edb4ba4e3ed27}





#### `public virtual `[`Format`](#structscy_1_1av_1_1Format)` & getByID(const std::string & id)` {#group__av_1ga5139a694c99b22a01dd6cebdcdcf80eb}





#### `public virtual `[`Format`](#structscy_1_1av_1_1Format)` & getOrDefault(const std::string & name)` {#group__av_1ga0060ffcf37bbd8efa2362c598872c81a}





#### `public virtual `[`Format`](#structscy_1_1av_1_1Format)` & getDefault()` {#group__av_1gac0c655caad54b6192ecfdc73c04b00e1}



If a default has been specified it will be returned, other the format with the highest priority will take precedence.

#### `public virtual void registerFormat(const `[`Format`](#structscy_1_1av_1_1Format)` & format)` {#group__av_1ga2af81c756b1e9a18b4d9fec1d0aae501}



Registers the given media format overriding existing media formats of the same name.

#### `public virtual bool unregisterFormat(const std::string & name)` {#group__av_1ga35d5b38f24071b665dbe0895f9c21840}



Unregisters the media format matching the given name.

#### `public virtual void setDefault(const std::string & name)` {#group__av_1gabf704e68f4ba8ca65eb6ed45a7b7ef7e}

Sets the default fallback media format.



#### `public virtual bool exists(const std::string & name)` {#group__av_1ga4723ecceee10ce9b5c10500fcab53da1}





#### `public virtual void clear()` {#group__av_1ga11cf8bc02b9f1236a52b8a8ba6f04342}





#### `public virtual FormatList formats() const` {#group__av_1gac522b25711b01a224f02df815860a9d6}





# class `scy::av::FPSCounter` {#classscy_1_1av_1_1FPSCounter}


An FPS counter based on the simple moving average (SMA) algorithm.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public double fps` | 
`public std::int64_t frames` | 
`public inline  FPSCounter()` | 
`public inline void reset()` | 
`public inline void tick()` | 

## Members

#### `public double fps` {#group__av_1ga116ea3077cf60b7a576b59a6564e829d}





#### `public std::int64_t frames` {#group__av_1ga8450ea27d5f14c19296066413dd4cac8}





#### `public inline  FPSCounter()` {#group__av_1ga3a7ac11cae2ba7667dd8ee25c2c09dd7}





#### `public inline void reset()` {#group__av_1ga5e7f4d6a27bcddab6c42387f7fa10b17}





#### `public inline void tick()` {#group__av_1ga462bac68ff4e426f966bb1bf92e426f8}





# class `scy::av::FPSLimiter` {#classscy_1_1av_1_1FPSLimiter}

```
class scy::av::FPSLimiter
  : public scy::PacketProcessor
```  



This class limits the throughput rate of IPackets in a [PacketStream](#classscy_1_1PacketStream). If the throughput rate exceeds the max specified FPS then packets will be dropped.

Note that revious processors must not fragment packets otherwise this class will not be accurate, and the packet drop rate will be too high.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`PacketSignal`](#group__base_1ga3ffb5bda6133fac97f3bed2b567b9b6a)` emitter` | 
`public inline  FPSLimiter(int max,bool videoOnly)` | 
`public inline virtual void process(`[`IPacket`](#classscy_1_1IPacket)` & packet)` | 
`public inline virtual void onStreamStateChange(const `[`PacketStreamState`](#structscy_1_1PacketStreamState)` &)` | 
`protected int _max` | 
`protected bool _videoOnly` | 
`protected `[`legacy::FPSCounter`](./doc/api-av.md#structscy_1_1av_1_1legacy_1_1FPSCounter)` _counter` | 

## Members

#### `public `[`PacketSignal`](#group__base_1ga3ffb5bda6133fac97f3bed2b567b9b6a)` emitter` {#group__av_1ga59db66b46321468598d83db2a52b5495}





#### `public inline  FPSLimiter(int max,bool videoOnly)` {#group__av_1ga029c9650c19891d96c7d05b720ada193}





#### `public inline virtual void process(`[`IPacket`](#classscy_1_1IPacket)` & packet)` {#group__av_1ga44d11b0258baa981ef71b9b5647de97d}



This method performs processing on the given packet and emits the result.

Note: If packet processing is async (the packet is not in the current thread scope) then packet data must be copied. Copied data can be freed directly aFter the async call to emit() the outgoing packet.

#### `public inline virtual void onStreamStateChange(const `[`PacketStreamState`](#structscy_1_1PacketStreamState)` &)` {#group__av_1ga159f48189ca69a2e40d20376335249fb}



Called by the [PacketStream](#classscy_1_1PacketStream) to notify when the internal [Stream](#classscy_1_1Stream) state changes. On receiving the Stopped state, it is the responsibility of the adapter to have ceased all outgoing packet transmission, especially in multi-thread scenarios.

#### `protected int _max` {#group__av_1ga939b5b9fd83801ddc2fdea386c4e556d}





#### `protected bool _videoOnly` {#group__av_1ga1d2f345f16e43261b3432d15157e58d6}





#### `protected `[`legacy::FPSCounter`](./doc/api-av.md#structscy_1_1av_1_1legacy_1_1FPSCounter)` _counter` {#group__av_1ga5eb5923a12182c1c97382cf9da35f938}





# class `scy::av::ICapture` {#classscy_1_1av_1_1ICapture}

```
class scy::av::ICapture
  : public scy::PacketStreamAdapter
  : public scy::async::Startable
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`PacketSignal`](#group__base_1ga3ffb5bda6133fac97f3bed2b567b9b6a)` emitter` | 
`public inline  ICapture()` | 
`public inline virtual  ~ICapture()` | 
`public void start()` | 
`public void stop()` | 
`public void getEncoderFormat(`[`Format`](#structscy_1_1av_1_1Format)` & iformat)` | Sets the input format for encoding with this capture device.
`public inline virtual void onStreamStateChange(const `[`PacketStreamState`](#structscy_1_1PacketStreamState)` &)` | 

## Members

#### `public `[`PacketSignal`](#group__base_1ga3ffb5bda6133fac97f3bed2b567b9b6a)` emitter` {#group__av_1ga3a2c153ddf81cd8be57efe0a784a2339}





#### `public inline  ICapture()` {#group__av_1ga3f62aa3503362c5540dc612bce32a0b3}





#### `public inline virtual  ~ICapture()` {#group__av_1gaf147c988947128fb81db6bdb70f932cb}





#### `public void start()` {#group__av_1ga001da1b367e05e2a1975dd07e6daab35}





#### `public void stop()` {#group__av_1gad0feb65608ad2d11078abb46d5918284}





#### `public void getEncoderFormat(`[`Format`](#structscy_1_1av_1_1Format)` & iformat)` {#group__av_1ga909c7fab8e95be80ed78d8e847658183}

Sets the input format for encoding with this capture device.



#### `public inline virtual void onStreamStateChange(const `[`PacketStreamState`](#structscy_1_1PacketStreamState)` &)` {#group__av_1ga76231c9a9b50661929f4e5965c1fcb03}



Called by the [PacketStream](#classscy_1_1PacketStream) to notify when the internal [Stream](#classscy_1_1Stream) state changes. On receiving the Stopped state, it is the responsibility of the adapter to have ceased all outgoing packet transmission, especially in multi-thread scenarios.

# class `scy::av::IDeviceManager` {#classscy_1_1av_1_1IDeviceManager}




A platform independent interface to enumerate audio and video devices on the system.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`Signal`](#classscy_1_1Signal)< void(bool &, bool &)`> DevicesChanged` | 
`public inline virtual  ~IDeviceManager()` | 
`public bool initialize()` | Initialization.
`public void uninitialize()` | 
`public bool getAudioInputDevices(std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devices)` | [Device](./doc/api-av.md#structscy_1_1av_1_1Device) enumeration.
`public bool getAudioOutputDevices(std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devices)` | 
`public bool getAudioInputDevice(`[`Device`](#structscy_1_1av_1_1Device)` & out,const std::string & name,int id)` | 
`public bool getAudioInputDevice(`[`Device`](#structscy_1_1av_1_1Device)` & out,int id)` | 
`public bool getAudioOutputDevice(`[`Device`](#structscy_1_1av_1_1Device)` & out,const std::string & name,int id)` | 
`public bool getAudioOutputDevice(`[`Device`](#structscy_1_1av_1_1Device)` & out,int id)` | 
`public bool getVideoCaptureDevices(std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devs)` | 
`public bool getVideoCaptureDevice(`[`Device`](#structscy_1_1av_1_1Device)` & out,int id)` | 
`public bool getVideoCaptureDevice(`[`Device`](#structscy_1_1av_1_1Device)` & out,const std::string & name,int id)` | 
`public bool getDefaultAudioInputDevice(`[`Device`](#structscy_1_1av_1_1Device)` & device)` | 
`public bool getDefaultAudioOutputDevice(`[`Device`](#structscy_1_1av_1_1Device)` & device)` | 
`public bool getDefaultVideoCaptureDevice(`[`Device`](#structscy_1_1av_1_1Device)` & device)` | 
`public int getCapabilities()` | Capabilities.
`public void print(std::ostream & ost)` | 

## Members

#### `public `[`Signal`](#classscy_1_1Signal)< void(bool &, bool &)`> DevicesChanged` {#group__av_1ga69dbbe5f5c10c8aba8aac57c9a94e511}



Signals on DevicesChanged. Arg 1 is true when device is video, false for audio Arg 2 is true when device connects, flase on disconnection

#### `public inline virtual  ~IDeviceManager()` {#group__av_1ga99bf003f84ab4e04a07d17185dbf491c}





#### `public bool initialize()` {#group__av_1ga61c821fe1a7d181dbc439c90fa4524da}

Initialization.



#### `public void uninitialize()` {#group__av_1ga6438ea5095404fc8ded7619a7863288c}





#### `public bool getAudioInputDevices(std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devices)` {#group__av_1ga324ce38356335066bb2cb4fd6a8521ee}

[Device](#structscy_1_1av_1_1Device) enumeration.



#### `public bool getAudioOutputDevices(std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devices)` {#group__av_1ga389204a9d5fc74dfbfd1179f2cab8fe9}





#### `public bool getAudioInputDevice(`[`Device`](#structscy_1_1av_1_1Device)` & out,const std::string & name,int id)` {#group__av_1ga8eb682c21d928e20252dd682512799eb}





#### `public bool getAudioInputDevice(`[`Device`](#structscy_1_1av_1_1Device)` & out,int id)` {#group__av_1gadf949c6e35c2d03faeabb78ce72264df}





#### `public bool getAudioOutputDevice(`[`Device`](#structscy_1_1av_1_1Device)` & out,const std::string & name,int id)` {#group__av_1ga294eb11a87c7bf21e263a5080cf86da6}





#### `public bool getAudioOutputDevice(`[`Device`](#structscy_1_1av_1_1Device)` & out,int id)` {#group__av_1ga44f5d5891ec58d46f964814bdfdb9afc}





#### `public bool getVideoCaptureDevices(std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devs)` {#group__av_1gaf8af159e35b82018fe3c62ffd93e99b4}





#### `public bool getVideoCaptureDevice(`[`Device`](#structscy_1_1av_1_1Device)` & out,int id)` {#group__av_1gaf9ca7d9b804d2fd3b80a66c99a5ce994}





#### `public bool getVideoCaptureDevice(`[`Device`](#structscy_1_1av_1_1Device)` & out,const std::string & name,int id)` {#group__av_1ga519256a027ac96e90434aaed2b84621f}





#### `public bool getDefaultAudioInputDevice(`[`Device`](#structscy_1_1av_1_1Device)` & device)` {#group__av_1ga3143d54a8029ce31cd3c35aeb458d8ba}





#### `public bool getDefaultAudioOutputDevice(`[`Device`](#structscy_1_1av_1_1Device)` & device)` {#group__av_1gac7a4388e97359a1456a1d16c582affc5}





#### `public bool getDefaultVideoCaptureDevice(`[`Device`](#structscy_1_1av_1_1Device)` & device)` {#group__av_1ga0f87a257398f70cb9ad933de0148d110}





#### `public int getCapabilities()` {#group__av_1ga96164fb6b34888ccee6930b3c79d5cda}

Capabilities.



#### `public void print(std::ostream & ost)` {#group__av_1ga0c7df28789555440e61e55b0cf791ec1}





# class `scy::av::IEncoder` {#classscy_1_1av_1_1IEncoder}

```
class scy::av::IEncoder
  : public scy::Stateful< EncoderState >
```  

This is the abstract class for all encoders.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public void initialize()` | 
`public void uninitialize()` | 
`public `[`EncoderOptions`](#structscy_1_1av_1_1EncoderOptions)` & options()` | 
`public inline virtual bool isNone() const` | 
`public inline virtual bool isReady() const` | 
`public inline virtual bool isEncoding() const` | 
`public inline virtual bool isActive() const` | 
`public inline virtual bool isStopped() const` | 
`public inline virtual bool isError() const` | 

## Members

#### `public void initialize()` {#group__av_1ga41ee1900d4b4eb52b72371706cdf13ad}





#### `public void uninitialize()` {#group__av_1ga6dac4e637ea9221dd30b68ffede3e812}





#### `public `[`EncoderOptions`](#structscy_1_1av_1_1EncoderOptions)` & options()` {#group__av_1ga18100d2ec264a7bdd0215dc2eccaa79c}





#### `public inline virtual bool isNone() const` {#group__av_1ga773da7e63fe6afbdf30ad268ee61e512}





#### `public inline virtual bool isReady() const` {#group__av_1gae6f19cdf4e3c776bdef058997307f208}





#### `public inline virtual bool isEncoding() const` {#group__av_1ga2450b61a8a20b293a783bab685424285}





#### `public inline virtual bool isActive() const` {#group__av_1ga7bc5645b62a07a4249de5fdcfe24c5e9}





#### `public inline virtual bool isStopped() const` {#group__av_1gaa306a4894a57b67f41cc73b68060da93}





#### `public inline virtual bool isError() const` {#group__av_1gabf096b994cc26365bdc9f4edcfb1045e}





# class `scy::av::LinuxDeviceManager` {#classscy_1_1av_1_1LinuxDeviceManager}

```
class scy::av::LinuxDeviceManager
  : public scy::av::DeviceManager
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  LinuxDeviceManager()` | 
`public virtual  ~LinuxDeviceManager()` | 
`public virtual bool getCameras(std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devs)` | 

## Members

#### `public  LinuxDeviceManager()` {#group__av_1ga183809a0da4b49689b13e098f8d0ac03}





#### `public virtual  ~LinuxDeviceManager()` {#group__av_1gab60b8f07ec657f5cded2956320e10a48}





#### `public virtual bool getCameras(std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devs)` {#group__av_1gac931c97e65968e0f46d18038e5789939}





# class `scy::av::MacDeviceManager` {#classscy_1_1av_1_1MacDeviceManager}

```
class scy::av::MacDeviceManager
  : public scy::av::DeviceManager
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  MacDeviceManager()` | 
`public virtual  ~MacDeviceManager()` | 
`public virtual bool getCameras(std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devs)` | 

## Members

#### `public  MacDeviceManager()` {#group__av_1gac14003aeb093497f3a2930f26cae0acf}





#### `public virtual  ~MacDeviceManager()` {#group__av_1gaa0500c9b93a11442e3d505b0e7611848}





#### `public virtual bool getCameras(std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devs)` {#group__av_1ga689d89e5f57be3c868009b9ffdd11fe2}





# class `scy::av::MacDeviceWatcher` {#classscy_1_1av_1_1MacDeviceWatcher}

```
class scy::av::MacDeviceWatcher
  : public scy::av::DeviceWatcher
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  explicit MacDeviceWatcher(`[`IDeviceManager`](#classscy_1_1av_1_1IDeviceManager)` * dm)` | 
`public virtual  ~MacDeviceWatcher()` | 
`public virtual bool start()` | 
`public virtual void stop()` | 

## Members

#### `public  explicit MacDeviceWatcher(`[`IDeviceManager`](#classscy_1_1av_1_1IDeviceManager)` * dm)` {#group__av_1gaac48968d26c753ef2eb58b8a5b940221}





#### `public virtual  ~MacDeviceWatcher()` {#group__av_1ga32910b3b0f011d047bb4521df07b2d67}





#### `public virtual bool start()` {#group__av_1ga86ba79c6b43c5f35db2f660da0e72147}





#### `public virtual void stop()` {#group__av_1ga9133417d7365867fd021e28a52f08c90}





# class `scy::av::RealtimePacketQueue` {#classscy_1_1av_1_1RealtimePacketQueue}

```
class scy::av::RealtimePacketQueue
  : public scy::AsyncPacketQueue< PacketT >
```  



This class emits media packets based on their realtime pts value.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  RealtimePacketQueue(int maxSize)` | 
`public inline virtual  ~RealtimePacketQueue()` | 
`public inline std::int64_t realTime()` | Return the current duration from stream start in microseconds.
`protected std::int64_t _startTime` | 
`protected inline virtual PacketT * popNext()` | 
`protected inline virtual void onStreamStateChange(const `[`PacketStreamState`](#structscy_1_1PacketStreamState)` &)` | 

## Members

#### `public inline  RealtimePacketQueue(int maxSize)` {#group__av_1ga2fb259d001893d803875ed5d0c34ef31}





#### `public inline virtual  ~RealtimePacketQueue()` {#group__av_1ga72e0189b06eaa7b0a1d142adfc39b4cd}





#### `public inline std::int64_t realTime()` {#group__av_1ga46c986012ccba1ab34ee994bfabc191e}

Return the current duration from stream start in microseconds.



#### `protected std::int64_t _startTime` {#group__av_1ga57e15f84e917019ceb40954e39e0a749}





#### `protected inline virtual PacketT * popNext()` {#group__av_1ga91d9d6a313e36434d61237631e7202b4}





#### `protected inline virtual void onStreamStateChange(const `[`PacketStreamState`](#structscy_1_1PacketStreamState)` &)` {#group__av_1gacc30bdbf591ff42562b0feba8c0dd3c0}



Called by the [PacketStream](#classscy_1_1PacketStream) to notify when the internal [Stream](#classscy_1_1Stream) state changes. On receiving the Stopped state, it is the responsibility of the adapter to have ceased all outgoing packet transmission, especially in multi-thread scenarios.

# struct `scy::av::AudioCodec` {#structscy_1_1av_1_1AudioCodec}

```
struct scy::av::AudioCodec
  : public scy::av::Codec
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public int channels` | 
`public std::string sampleFmt` | 
`public  AudioCodec()` | 
`public  AudioCodec(const std::string & name,int channels,int sampleRate,int bitRate,const std::string & sampleFmt)` | 
`public  AudioCodec(const std::string & name,const std::string & encoder,int channels,int sampleRate,int bitRate,const std::string & sampleFmt)` | 
`public virtual  ~AudioCodec()` | AudioCodec(const AudioCodec& r);.
`public virtual std::string toString() const` | 
`public virtual void print(std::ostream & ost)` | 

## Members

#### `public int channels` {#group__av_1ga6127b16a7d5e1329e88e124366015071}





#### `public std::string sampleFmt` {#group__av_1ga4f3070e09c5d36971189f42402984670}



One of: u8, s16, s32, flt, dbl, u8p, s16p, s32p, fltp, dblp

#### `public  AudioCodec()` {#group__av_1gac7015ec1976fd68879c0af8e22c14de0}



int frameSize; // Frame size (optional value set by encoder/decoder) /// int bufferSize; // Buffer size (optional value set by encoder/decoder)

#### `public  AudioCodec(const std::string & name,int channels,int sampleRate,int bitRate,const std::string & sampleFmt)` {#group__av_1ga3c0da0a206873c832f0f7fdd83ac4ca4}





#### `public  AudioCodec(const std::string & name,const std::string & encoder,int channels,int sampleRate,int bitRate,const std::string & sampleFmt)` {#group__av_1gafac2a4dbaf98eec7a3a09184ecee2af1}





#### `public virtual  ~AudioCodec()` {#group__av_1gacef753a31b9526ed27724a3fa9edf9b5}

AudioCodec(const AudioCodec& r);.



#### `public virtual std::string toString() const` {#group__av_1gacb351e8e04b20d1c0ce2f852494f4447}





#### `public virtual void print(std::ostream & ost)` {#group__av_1ga65de987085ce74ab3bb75701ffc0c866}





# struct `scy::av::AudioPacket` {#structscy_1_1av_1_1AudioPacket}

```
struct scy::av::AudioPacket
  : public scy::av::MediaPacket
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::size_t numSamples` | 
`public inline  AudioPacket(std::uint8_t * data,std::size_t size,std::size_t numSamples,std::int64_t time)` | 
`public inline virtual  ~AudioPacket()` | AudioPacket(const AudioPacket& r) : /// MediaPacket(r) {}.
`public inline virtual `[`IPacket`](#classscy_1_1IPacket)` * clone() const` | 
`public inline virtual std::uint8_t * samples() const` | 
`public inline virtual const char * className() const` | 

## Members

#### `public std::size_t numSamples` {#group__av_1ga56c2329b40721887a20842b984987961}





#### `public inline  AudioPacket(std::uint8_t * data,std::size_t size,std::size_t numSamples,std::int64_t time)` {#group__av_1gaaa5e8ea8078caec522573c520cb703f6}





#### `public inline virtual  ~AudioPacket()` {#group__av_1gad90065fb502c8f36582d1712b71299cc}

AudioPacket(const AudioPacket& r) : /// MediaPacket(r) {}.



#### `public inline virtual `[`IPacket`](#classscy_1_1IPacket)` * clone() const` {#group__av_1gaa76be728a37ac7f25514c1004228e3ba}





#### `public inline virtual std::uint8_t * samples() const` {#group__av_1ga44ece9f38c8a05770e9512f079720910}





#### `public inline virtual const char * className() const` {#group__av_1gaf976258051fed7d6cb8657455ed705f6}





# struct `scy::av::Codec` {#structscy_1_1av_1_1Codec}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::string name` | The display name for this codec.
`public std::string encoder` | The encoder name for FFmpeg.
`public int sampleRate` | The sampling rate or RTP clock rate.
`public int bitRate` | The bit rate to encode at.
`public int quality` | Optional quality value, variable range depending on codec.
`public bool enabled` | Weather or not the codec is available for use.
`public  Codec()` | 
`public  Codec(const std::string & name,int sampleRate,int bitRate,bool enabled)` | 
`public  Codec(const std::string & name,const std::string & encoder,int sampleRate,int bitRate,bool enabled)` | 
`public virtual  ~Codec()` | Codec(const Codec& r);.
`public virtual std::string toString() const` | 
`public virtual void print(std::ostream & ost)` | 

## Members

#### `public std::string name` {#group__av_1ga6844966d0cc504e1a19d138e4787bc26}

The display name for this codec.



#### `public std::string encoder` {#group__av_1ga5552a927640813c68b25c07c89786f79}

The encoder name for FFmpeg.



#### `public int sampleRate` {#group__av_1gad3b634766c2f3458c07803c6d9036b47}

The sampling rate or RTP clock rate.



#### `public int bitRate` {#group__av_1ga1600230a934a25fb9d280681e16b0b98}

The bit rate to encode at.



#### `public int quality` {#group__av_1gab28adf6739a1653c39cd6b004f9f3e8f}

Optional quality value, variable range depending on codec.



#### `public bool enabled` {#group__av_1gab6735b630af06c573029766f5944be93}

Weather or not the codec is available for use.



#### `public  Codec()` {#group__av_1ga8b9bea5abd6c627618a66f5bb0cacdda}





#### `public  Codec(const std::string & name,int sampleRate,int bitRate,bool enabled)` {#group__av_1ga7e7561a995faa17643c97a585f92f042}





#### `public  Codec(const std::string & name,const std::string & encoder,int sampleRate,int bitRate,bool enabled)` {#group__av_1ga168efa478447f30e8f6c4921016335f6}





#### `public virtual  ~Codec()` {#group__av_1gad5850bf5554a8187668d0480d340f01d}

Codec(const Codec& r);.



#### `public virtual std::string toString() const` {#group__av_1ga8b8296b25c3b401f87f21090fd8de249}





#### `public virtual void print(std::ostream & ost)` {#group__av_1gadc2b86e08e23d3201f0c3430ae795f5d}





# struct `scy::av::Device` {#structscy_1_1av_1_1Device}


Represents a system audio, video or render device.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::string type` | 
`public int id` | 
`public std::string name` | 
`public std::string guid` | 
`public bool isDefault` | 
`public bool isAvailable` | 
`public  Device()` | 
`public  Device(const std::string & type,int id,const std::string & name,const std::string & guid,bool isDefault,bool isAvailable)` | 
`public void print(std::ostream & os)` | 
`public inline bool operator==(const `[`Device`](#structscy_1_1av_1_1Device)` & that) const` | 

## Members

#### `public std::string type` {#group__av_1ga0844c85f6319f6213ea026ca35732e1f}





#### `public int id` {#group__av_1gab7545dfaea8360e3bd2c071e5906e82e}





#### `public std::string name` {#group__av_1ga2aa878bff3aa5e36fc6f904eb36d9ddd}





#### `public std::string guid` {#group__av_1ga000efaabb0352c7d86870e649e2131c8}





#### `public bool isDefault` {#group__av_1gabf1ce9150645f511dd0b78dfbcc83522}





#### `public bool isAvailable` {#group__av_1ga1e1cd71321a4a5bf06474398eb14a39f}





#### `public  Device()` {#group__av_1ga53ea1e4acdb14bfacf39b439fbee83fb}





#### `public  Device(const std::string & type,int id,const std::string & name,const std::string & guid,bool isDefault,bool isAvailable)` {#group__av_1ga31dad4b6f0da95ac54d49666db8f8377}





#### `public void print(std::ostream & os)` {#group__av_1ga212e9032eac9a85662e69714b9a2d349}





#### `public inline bool operator==(const `[`Device`](#structscy_1_1av_1_1Device)` & that) const` {#group__av_1ga686faeba93e2f49350e5d317dab86a2c}





# struct `scy::av::EncoderOptions` {#structscy_1_1av_1_1EncoderOptions}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`Format`](./doc/api-av.md#structscy_1_1av_1_1Format)` iformat` | 
`public `[`Format`](./doc/api-av.md#structscy_1_1av_1_1Format)` oformat` | 
`public std::string ifile` | 
`public std::string ofile` | 
`public long duration` | 
`public inline  EncoderOptions(const `[`Format`](#structscy_1_1av_1_1Format)` & iformat,const `[`Format`](#structscy_1_1av_1_1Format)` & oformat,const std::string & ifile,const std::string & ofile,long duration)` | 
`public inline virtual  ~EncoderOptions()` | 

## Members

#### `public `[`Format`](./doc/api-av.md#structscy_1_1av_1_1Format)` iformat` {#group__av_1ga54c813e9943c855171e8279e8085548a}





#### `public `[`Format`](./doc/api-av.md#structscy_1_1av_1_1Format)` oformat` {#group__av_1gaaa1c4adc23b0755d9633cff732d931e9}





#### `public std::string ifile` {#group__av_1gaca1d7ab6e72cc562ef90860a01461b8a}





#### `public std::string ofile` {#group__av_1gae6f1fb56ae68ac24395c4f51dcf7c85b}





#### `public long duration` {#group__av_1ga65a112623aedce6dbb8f6a0f244dea05}





#### `public inline  EncoderOptions(const `[`Format`](#structscy_1_1av_1_1Format)` & iformat,const `[`Format`](#structscy_1_1av_1_1Format)` & oformat,const std::string & ifile,const std::string & ofile,long duration)` {#group__av_1ga387141c363e2ab3a4f500549fe359dd5}





#### `public inline virtual  ~EncoderOptions()` {#group__av_1ga2b7ca7389316246a2d16095a4cd62e70}





# struct `scy::av::EncoderState` {#structscy_1_1av_1_1EncoderState}

```
struct scy::av::EncoderState
  : public scy::State
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual std::string str(unsigned int id) const` | 

## Members

#### `public inline virtual std::string str(unsigned int id) const` {#group__av_1ga1b8ea9145750488a74eb407a32fe90ba}





# struct `scy::av::Format` {#structscy_1_1av_1_1Format}




Defines a media container format which is available through the [Format](#structscy_1_1av_1_1Format) Registry for encoding/decoding. A format defined preferred default values for each codec.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::string name` | Base [Format](./doc/api-av.md#structscy_1_1av_1_1Format) Variables.
`public std::string id` | The short name of this format.
`public `[`VideoCodec`](./doc/api-av.md#structscy_1_1av_1_1VideoCodec)` video` | The video codec.
`public `[`AudioCodec`](./doc/api-av.md#structscy_1_1av_1_1AudioCodec)` audio` | The audio codec.
`public int priority` | The priority this format will be displayed on the list.
`public  Format()` | Ctors/Dtors.
`public  Format(const std::string & name,const std::string & id,const `[`VideoCodec`](#structscy_1_1av_1_1VideoCodec)` & video,const `[`AudioCodec`](#structscy_1_1av_1_1AudioCodec)` & audio,int priority)` | 
`public  Format(const std::string & name,const std::string & id,const `[`VideoCodec`](#structscy_1_1av_1_1VideoCodec)` & video,int priority)` | 
`public  Format(const std::string & name,const std::string & id,const `[`AudioCodec`](#structscy_1_1av_1_1AudioCodec)` & audio,int priority)` | 
`public  Format(const `[`Format`](#structscy_1_1av_1_1Format)` & r)` | 
`public `[`Type`](#group__av_1gaee45ab380f3ab82ddee6a904902e441a)` type() const` | The format type.
`public virtual std::string toString() const` | 
`public virtual void print(std::ostream & ost)` | 

## Members

#### `public std::string name` {#group__av_1gab23291d998e8eeeaf780711adb47f279}

Base [Format](#structscy_1_1av_1_1Format) Variables.

The display name of this format.

#### `public std::string id` {#group__av_1gae301eb6d3562a0593b3cba4645944965}

The short name of this format.



#### `public `[`VideoCodec`](./doc/api-av.md#structscy_1_1av_1_1VideoCodec)` video` {#group__av_1ga8c72beb62c31705f7954bb74941165f8}

The video codec.



#### `public `[`AudioCodec`](./doc/api-av.md#structscy_1_1av_1_1AudioCodec)` audio` {#group__av_1ga2d7a7c6b006a17245c5e232d059f59be}

The audio codec.



#### `public int priority` {#group__av_1gad33f46119d0512791b3116b07af86423}

The priority this format will be displayed on the list.



#### `public  Format()` {#group__av_1gadc07003ad1a442a1a1f6430e46b07545}

Ctors/Dtors.



#### `public  Format(const std::string & name,const std::string & id,const `[`VideoCodec`](#structscy_1_1av_1_1VideoCodec)` & video,const `[`AudioCodec`](#structscy_1_1av_1_1AudioCodec)` & audio,int priority)` {#group__av_1ga6dfaf2481d30bc3a9172e3cd0fdfb021}





#### `public  Format(const std::string & name,const std::string & id,const `[`VideoCodec`](#structscy_1_1av_1_1VideoCodec)` & video,int priority)` {#group__av_1ga3b8c3f30d4bf073ae393bbce6d1b46f1}





#### `public  Format(const std::string & name,const std::string & id,const `[`AudioCodec`](#structscy_1_1av_1_1AudioCodec)` & audio,int priority)` {#group__av_1ga5b4c8a4300f9436641f5ae2e4934cf1c}





#### `public  Format(const `[`Format`](#structscy_1_1av_1_1Format)` & r)` {#group__av_1gac534811daa5a3274f418f327b1d24f55}





#### `public `[`Type`](#group__av_1gaee45ab380f3ab82ddee6a904902e441a)` type() const` {#group__av_1ga8cce3276f2c5399b7f7f86b2f3d431f8}

The format type.



#### `public virtual std::string toString() const` {#group__av_1ga55f0817773eba2440856dcac9cc8db04}





#### `public virtual void print(std::ostream & ost)` {#group__av_1gac2a467b9b251cc8274c3900360e9c58d}





# struct `scy::av::MediaPacket` {#structscy_1_1av_1_1MediaPacket}

```
struct scy::av::MediaPacket
  : public scy::RawPacket
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::int64_t time` | 
`public inline  MediaPacket(std::uint8_t * data,std::size_t size,std::int64_t time)` | 
`public inline  MediaPacket(const `[`MediaPacket`](#structscy_1_1av_1_1MediaPacket)` & r)` | 
`public inline virtual  ~MediaPacket()` | 
`public inline virtual `[`IPacket`](#classscy_1_1IPacket)` * clone() const` | 
`public inline virtual const char * className() const` | 

## Members

#### `public std::int64_t time` {#group__av_1ga4a94d6bc2a892cfac104f9d49b63d657}





#### `public inline  MediaPacket(std::uint8_t * data,std::size_t size,std::int64_t time)` {#group__av_1ga12c04dfb241c68e9260aa0e60bf38b36}





#### `public inline  MediaPacket(const `[`MediaPacket`](#structscy_1_1av_1_1MediaPacket)` & r)` {#group__av_1gaebb222de1256c5f9a53d3754634abf55}





#### `public inline virtual  ~MediaPacket()` {#group__av_1gaad5d3b078c34e951da445c033546c267}





#### `public inline virtual `[`IPacket`](#classscy_1_1IPacket)` * clone() const` {#group__av_1ga05692ee1e11569e9a5966a1dbd50913f}





#### `public inline virtual const char * className() const` {#group__av_1ga0ae1eb6db615ac70f6a850b20aef9102}





# struct `scy::av::VideoCodec` {#structscy_1_1av_1_1VideoCodec}

```
struct scy::av::VideoCodec
  : public scy::av::Codec
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public int width` | 
`public int height` | 
`public double fps` | 
`public std::string pixelFmt` | 
`public  VideoCodec()` | 
`public  VideoCodec(const std::string & name,int width,int height,double fps,int bitRate,int sampleRate,const std::string & pixelFmt)` | 
`public  VideoCodec(const std::string & name,const std::string & encoder,int width,int height,double fps,int bitRate,int sampleRate,const std::string & pixelFmt)` | 
`public  VideoCodec(const `[`VideoCodec`](#structscy_1_1av_1_1VideoCodec)` & r)` | 
`public virtual  ~VideoCodec()` | 
`public virtual std::string toString() const` | 
`public virtual void print(std::ostream & ost)` | 

## Members

#### `public int width` {#group__av_1ga0d2a616eaac095f875d8f2a0bd98091f}





#### `public int height` {#group__av_1ga6cbce4c67a21bce1e6a7ecee4d7b83b7}





#### `public double fps` {#group__av_1gab5abeb6ba9ae28522af6da214811592b}





#### `public std::string pixelFmt` {#group__av_1ga94555d9553e04922c805d3a4661462d2}





#### `public  VideoCodec()` {#group__av_1gaa44f4cc846f8b9aad8557078822c327a}





#### `public  VideoCodec(const std::string & name,int width,int height,double fps,int bitRate,int sampleRate,const std::string & pixelFmt)` {#group__av_1ga5f8969d1589b58227b2b1cdc5c41de30}





#### `public  VideoCodec(const std::string & name,const std::string & encoder,int width,int height,double fps,int bitRate,int sampleRate,const std::string & pixelFmt)` {#group__av_1gae92dd31f89eba15e494fafe4e609a3c1}





#### `public  VideoCodec(const `[`VideoCodec`](#structscy_1_1av_1_1VideoCodec)` & r)` {#group__av_1gada45746b74cf8b5c55f69c4a1ae29aec}





#### `public virtual  ~VideoCodec()` {#group__av_1ga97e3ee1f4ba76c2ef660b8ca79443c8b}





#### `public virtual std::string toString() const` {#group__av_1gac600532aab259c40a2df91c0fde5f56e}





#### `public virtual void print(std::ostream & ost)` {#group__av_1gafcf9a3f738f27fcd414b44d5566a020d}





# struct `scy::av::VideoPacket` {#structscy_1_1av_1_1VideoPacket}

```
struct scy::av::VideoPacket
  : public scy::av::MediaPacket
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public int width` | 
`public int height` | 
`public bool iframe` | 
`public inline  VideoPacket(std::uint8_t * data,std::size_t size,int width,int height,std::int64_t time)` | 
`public inline  VideoPacket(const `[`VideoPacket`](#structscy_1_1av_1_1VideoPacket)` & r)` | 
`public inline virtual  ~VideoPacket()` | 
`public inline virtual `[`IPacket`](#classscy_1_1IPacket)` * clone() const` | 
`public inline virtual const char * className() const` | 

## Members

#### `public int width` {#group__av_1ga73d26deb4e00050f9db4a1fd99e58cce}





#### `public int height` {#group__av_1ga25f5e1546e384451aef0d0e645e25a35}





#### `public bool iframe` {#group__av_1gaba12f8bdb6b1fe073f8a424dc59bc799}





#### `public inline  VideoPacket(std::uint8_t * data,std::size_t size,int width,int height,std::int64_t time)` {#group__av_1ga31e713b988c2a0a645fc38d3a1e6e725}





#### `public inline  VideoPacket(const `[`VideoPacket`](#structscy_1_1av_1_1VideoPacket)` & r)` {#group__av_1ga04b7a866edebd63332a1a852eba039e3}





#### `public inline virtual  ~VideoPacket()` {#group__av_1ga29a157f4432258c8468829601009457e}





#### `public inline virtual `[`IPacket`](#classscy_1_1IPacket)` * clone() const` {#group__av_1ga6e7e3dba2e11324dc06aedc72692d2c4}





#### `public inline virtual const char * className() const` {#group__av_1gacdf6d0efe5dbff5d2f153b71202ddd4c}





# namespace `scy::av::legacy` {#namespacescy_1_1av_1_1legacy}



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`struct `[`scy::av::legacy::FPSCounter`](#structscy_1_1av_1_1legacy_1_1FPSCounter)    | 
# struct `scy::av::legacy::FPSCounter` {#structscy_1_1av_1_1legacy_1_1FPSCounter}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public clock_t start` | 
`public clock_t end` | 
`public std::int64_t frames` | 
`public double total` | 
`public double fps` | 
`public inline  FPSCounter()` | 
`public inline void tick()` | 
`public inline void reset()` | 
`public inline bool started()` | 
`public inline void startFrame()` | 
`public inline double endFrame()` | 

## Members

#### `public clock_t start` {#group__av_1gab8adff3b20da64480d14945b72c9f103}





#### `public clock_t end` {#group__av_1ga4c0df024844c9f7d93f9a211e8280d7b}





#### `public std::int64_t frames` {#group__av_1ga778fae0365f8c552c4d095a0e2c8621e}





#### `public double total` {#group__av_1ga1c3c6dfc7993518d31340a13b3ce6f6f}





#### `public double fps` {#group__av_1ga1371516475e42a8a82e150a0374b7ddd}





#### `public inline  FPSCounter()` {#group__av_1ga08e60283f72b88f8ac4470e99b6c6a29}





#### `public inline void tick()` {#group__av_1ga8482fcfaf1b6130986021e717a31480f}





#### `public inline void reset()` {#group__av_1ga69069625ebaad7c2650f8b5694d9d236}





#### `public inline bool started()` {#group__av_1ga957474be4397f33d4de755fd6a6dfa35}





#### `public inline void startFrame()` {#group__av_1ga3a5def4cade260d217e3d04f612be349}





#### `public inline double endFrame()` {#group__av_1ga5a038d317369a4f232ed373d5248bf42}





