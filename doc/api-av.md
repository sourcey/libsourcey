# Module <!-- group --> `av`



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`namespace `[``scy::av``](#namespacescy_1_1av)    | 
`namespace `[``scy::av::legacy``](#namespacescy_1_1av_1_1legacy)    | 
`class `[``scy::av::IDeviceManager``](#classscy_1_1av_1_1IDeviceManager)    | 
`class `[``scy::av::DeviceWatcher``](#classscy_1_1av_1_1DeviceWatcher)    | 
`class `[``scy::av::DeviceManagerFactory``](#classscy_1_1av_1_1DeviceManagerFactory)    | 
`class `[``scy::av::DeviceManager``](#classscy_1_1av_1_1DeviceManager)    | 
`class `[``scy::av::FakeDeviceManager``](#classscy_1_1av_1_1FakeDeviceManager)    | 
`class `[``scy::av::LinuxDeviceManager``](#classscy_1_1av_1_1LinuxDeviceManager)    | 
`class `[``scy::av::MacDeviceManager``](#classscy_1_1av_1_1MacDeviceManager)    | 
`class `[``scy::av::FLVMetadataInjector``](#classscy_1_1av_1_1FLVMetadataInjector)    | 
`class `[``scy::av::MacDeviceWatcher``](#classscy_1_1av_1_1MacDeviceWatcher)    | 
`class `[``scy::av::FormatRegistry``](#classscy_1_1av_1_1FormatRegistry)    | 
`class `[``scy::av::FPSCounter``](#classscy_1_1av_1_1FPSCounter)    | An FPS counter based on the simple moving average (SMA) algorithm.
`class `[``scy::av::FPSLimiter``](#classscy_1_1av_1_1FPSLimiter)    | 
`class `[``scy::av::ICapture``](#classscy_1_1av_1_1ICapture)    | 
`class `[``scy::av::IEncoder``](#classscy_1_1av_1_1IEncoder)    | This is the abstract class for all encoders.
`class `[``scy::av::RealtimePacketQueue``](#classscy_1_1av_1_1RealtimePacketQueue)    | This class emits media packets based on their realtime pts value.
`struct `[``scy::av::Device``](#structscy_1_1av_1_1Device)    | Represents a system audio, video or render device.
`struct `[``scy::av::Codec``](#structscy_1_1av_1_1Codec)    | 
`struct `[``scy::av::AudioCodec``](#structscy_1_1av_1_1AudioCodec)    | 
`struct `[``scy::av::VideoCodec``](#structscy_1_1av_1_1VideoCodec)    | 
`struct `[``scy::av::Format``](#structscy_1_1av_1_1Format)    | 
`struct `[``scy::av::legacy::FPSCounter``](#structscy_1_1av_1_1legacy_1_1FPSCounter)    | 
`struct `[``scy::av::EncoderState``](#structscy_1_1av_1_1EncoderState)    | 
`struct `[``scy::av::EncoderOptions``](#structscy_1_1av_1_1EncoderOptions)    | 
`struct `[``scy::av::MediaPacket``](#structscy_1_1av_1_1MediaPacket)    | 
`struct `[``scy::av::VideoPacket``](#structscy_1_1av_1_1VideoPacket)    | 
`struct `[``scy::av::AudioPacket``](#structscy_1_1av_1_1AudioPacket)    | 
# namespace `scy::av`



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[``scy::av::DeviceManager``](#classscy_1_1av_1_1DeviceManager)    | 
`class `[``scy::av::DeviceManagerFactory``](#classscy_1_1av_1_1DeviceManagerFactory)    | 
`class `[``scy::av::DeviceWatcher``](#classscy_1_1av_1_1DeviceWatcher)    | 
`class `[``scy::av::FakeDeviceManager``](#classscy_1_1av_1_1FakeDeviceManager)    | 
`class `[``scy::av::FLVMetadataInjector``](#classscy_1_1av_1_1FLVMetadataInjector)    | 
`class `[``scy::av::FormatRegistry``](#classscy_1_1av_1_1FormatRegistry)    | 
`class `[``scy::av::FPSCounter``](#classscy_1_1av_1_1FPSCounter)    | An FPS counter based on the simple moving average (SMA) algorithm.
`class `[``scy::av::FPSLimiter``](#classscy_1_1av_1_1FPSLimiter)    | 
`class `[``scy::av::ICapture``](#classscy_1_1av_1_1ICapture)    | 
`class `[``scy::av::IDeviceManager``](#classscy_1_1av_1_1IDeviceManager)    | 
`class `[``scy::av::IEncoder``](#classscy_1_1av_1_1IEncoder)    | This is the abstract class for all encoders.
`class `[``scy::av::LinuxDeviceManager``](#classscy_1_1av_1_1LinuxDeviceManager)    | 
`class `[``scy::av::MacDeviceManager``](#classscy_1_1av_1_1MacDeviceManager)    | 
`class `[``scy::av::MacDeviceWatcher``](#classscy_1_1av_1_1MacDeviceWatcher)    | 
`class `[``scy::av::RealtimePacketQueue``](#classscy_1_1av_1_1RealtimePacketQueue)    | This class emits media packets based on their realtime pts value.
`struct `[``scy::av::AudioCodec``](#structscy_1_1av_1_1AudioCodec)    | 
`struct `[``scy::av::AudioPacket``](#structscy_1_1av_1_1AudioPacket)    | 
`struct `[``scy::av::Codec``](#structscy_1_1av_1_1Codec)    | 
`struct `[``scy::av::Device``](#structscy_1_1av_1_1Device)    | Represents a system audio, video or render device.
`struct `[``scy::av::EncoderOptions``](#structscy_1_1av_1_1EncoderOptions)    | 
`struct `[``scy::av::EncoderState``](#structscy_1_1av_1_1EncoderState)    | 
`struct `[``scy::av::Format``](#structscy_1_1av_1_1Format)    | 
`struct `[``scy::av::MediaPacket``](#structscy_1_1av_1_1MediaPacket)    | 
`struct `[``scy::av::VideoCodec``](#structscy_1_1av_1_1VideoCodec)    | 
`struct `[``scy::av::VideoPacket``](#structscy_1_1av_1_1VideoPacket)    | 
# class `scy::av::DeviceManager` 

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
`public virtual bool getAudioInputDevices(std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devices)` | [Device](#structscy_1_1av_1_1Device) enumeration.
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

#### `public  DeviceManager()` 





#### `public virtual  ~DeviceManager()` 





#### `public virtual bool initialize()` 

Initialization.



#### `public virtual void uninitialize()` 





#### `public virtual int getCapabilities()` 

Capabilities.



#### `public virtual bool getAudioInputDevices(std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devices)` 

[Device](#structscy_1_1av_1_1Device) enumeration.



#### `public virtual bool getAudioOutputDevices(std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devices)` 





#### `public virtual bool getAudioInputDevice(`[`Device`](#structscy_1_1av_1_1Device)` & out,const std::string & name,int id)` 





#### `public virtual bool getAudioInputDevice(`[`Device`](#structscy_1_1av_1_1Device)` & out,int id)` 





#### `public virtual bool getAudioOutputDevice(`[`Device`](#structscy_1_1av_1_1Device)` & out,const std::string & name,int id)` 





#### `public virtual bool getAudioOutputDevice(`[`Device`](#structscy_1_1av_1_1Device)` & out,int id)` 





#### `public virtual bool getVideoCaptureDevices(std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devs)` 





#### `public virtual bool getVideoCaptureDevice(`[`Device`](#structscy_1_1av_1_1Device)` & out,const std::string & name,int id)` 





#### `public virtual bool getVideoCaptureDevice(`[`Device`](#structscy_1_1av_1_1Device)` & out,int id)` 

Returns the video capture device at the given system index.



#### `public virtual bool getDefaultAudioInputDevice(`[`Device`](#structscy_1_1av_1_1Device)` & device)` 





#### `public virtual bool getDefaultAudioOutputDevice(`[`Device`](#structscy_1_1av_1_1Device)` & device)` 





#### `public virtual bool getDefaultVideoCaptureDevice(`[`Device`](#structscy_1_1av_1_1Device)` & device)` 





#### `public inline bool initialized() const` 





#### `public virtual void print(std::ostream & ost)` 





#### `protected virtual bool getDefaultAudioDevice(bool input,`[`Device`](#structscy_1_1av_1_1Device)` & device)` 





#### `protected virtual bool getAudioDevices(bool input,std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devs)` 





#### `protected virtual bool getAudioDevice(bool input,`[`Device`](#structscy_1_1av_1_1Device)` & out,const std::string & name,int id)` 





#### `protected virtual bool getAudioDevice(bool input,`[`Device`](#structscy_1_1av_1_1Device)` & out,int id)` 





#### `protected void setInitialized(bool initialized)` 





#### `protected void setWatcher(`[`DeviceWatcher`](#classscy_1_1av_1_1DeviceWatcher)` * watcher)` 





#### `protected `[`DeviceWatcher`](#classscy_1_1av_1_1DeviceWatcher)` * watcher()` 





# class `scy::av::DeviceManagerFactory` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------

## Members

# class `scy::av::DeviceWatcher` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  explicit DeviceWatcher(`[`IDeviceManager`](#classscy_1_1av_1_1IDeviceManager)` *)` | 
`public inline virtual  ~DeviceWatcher()` | 
`public inline virtual bool start()` | 
`public inline virtual void stop()` | 

## Members

#### `public inline  explicit DeviceWatcher(`[`IDeviceManager`](#classscy_1_1av_1_1IDeviceManager)` *)` 





#### `public inline virtual  ~DeviceWatcher()` 





#### `public inline virtual bool start()` 





#### `public inline virtual void stop()` 





# class `scy::av::FakeDeviceManager` 

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

#### `public inline  FakeDeviceManager()` 





#### `public inline virtual bool initialize()` 

Initialization.



#### `public inline virtual void uninitialize()` 





#### `public inline virtual int getCapabilities()` 

Capabilities.



#### `public inline virtual bool getMicrophones(std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devs)` 





#### `public inline virtual bool getSpeakers(std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devs)` 





#### `public inline virtual bool getMicrophone(const std::string & name,`[`Device`](#structscy_1_1av_1_1Device)` & out)` 





#### `public inline virtual bool getSpeaker(const std::string & name,`[`Device`](#structscy_1_1av_1_1Device)` & out)` 





#### `public inline virtual bool getCameras(std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devs)` 





#### `public inline virtual bool getDefaultCamera(`[`Device`](#structscy_1_1av_1_1Device)` & device)` 





#### `public inline void setMicrophones(const std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devices)` 





#### `public inline void setSpeakers(const std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devices)` 





#### `public inline void setCameras(const std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devices)` 





#### `public inline virtual bool getCamera(const std::string & name,`[`Device`](#structscy_1_1av_1_1Device)` & out)` 





#### `public inline bool getAudioDevice(bool input,const std::string & name,`[`Device`](#structscy_1_1av_1_1Device)` & out)` 





# class `scy::av::FLVMetadataInjector` 

```
class scy::av::FLVMetadataInjector
  : public scy::PacketProcessor
```  



This class implements a packetizer which appends correct stream headers and modifies the timestamp of FLV packets so Adobe's Flash Player will play our videos mid-stream.

This adapter is useful for multicast situations where we don't have the option of restarting the encoder stream.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`PacketSignal`](#classscy_1_1SignalBase)` emitter` | 
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
`protected `[`Format`](#structscy_1_1av_1_1Format)` _format` | 
`protected bool _initial` | 
`protected bool _modifyingStream` | 
`protected bool _waitingForKeyframe` | 
`protected std::uint32_t _timestampOffset` | 
`protected `[`legacy::FPSCounter`](#structscy_1_1av_1_1legacy_1_1FPSCounter)` _fpsCounter` | 

## Members

#### `public `[`PacketSignal`](#classscy_1_1SignalBase)` emitter` 





#### `public  IPacketizer(this-> emitter)` 





#### `public  _format(format)` 





#### `public  _initial(true)` 





#### `public  _modifyingStream(false)` 





#### `public  _waitingForKeyframe(false)` 





#### `public inline  _timestampOffset(0) const` 





#### `public inline virtual void onStreamStateChange(const `[`PacketStreamState`](#structscy_1_1PacketStreamState)` & state)` 

whenever the stream is restarted.

This method is called by the Packet [Stream](#classscy_1_1Stream)

#### `public inline virtual void process(`[`IPacket`](#classscy_1_1IPacket)` & packet)` 



This method performs processing on the given packet and emits the result.

Note: If packet processing is async (the packet is not in the current thread scope) then packet data must be copied. Copied data can be freed directly aFter the async call to emit() the outgoing packet.

#### `public inline virtual void fastUpdateTimestamp(char * buf,std::uint32_t timestamp)` 



Updates the timestamp in the given FLV tag buffer. No more need to copy data with this method. Caution: this method does not check buffer size.

#### `public inline virtual bool fastIsFLVHeader(char * buf)` 

Caution: this method does not check buffer size.



#### `public inline virtual bool fastIsFLVKeyFrame(char * buf)` 

Caution: this method does not check buffer size.



#### `public inline virtual void writeFLVHeader(`[`BitWriter`](#classscy_1_1BitWriter)` & writer)` 





#### `public inline std::int64_t doubleToInt(double d)` 





#### `public inline virtual void writeAMFSring(`[`BitWriter`](#classscy_1_1BitWriter)` & writer,const char * val)` 

AMF Helpers.



#### `public inline virtual void writeAMFDouble(`[`BitWriter`](#classscy_1_1BitWriter)` & writer,double val)` 





#### `public inline virtual void writeAMFBool(`[`BitWriter`](#classscy_1_1BitWriter)` & writer,bool val)` 





#### `protected `[`Format`](#structscy_1_1av_1_1Format)` _format` 





#### `protected bool _initial` 





#### `protected bool _modifyingStream` 





#### `protected bool _waitingForKeyframe` 





#### `protected std::uint32_t _timestampOffset` 





#### `protected `[`legacy::FPSCounter`](#structscy_1_1av_1_1legacy_1_1FPSCounter)` _fpsCounter` 





# class `scy::av::FormatRegistry` 






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

#### `public  FormatRegistry()` 





#### `public virtual  ~FormatRegistry()` 





#### `public virtual `[`Format`](#structscy_1_1av_1_1Format)` & get(const std::string & name)` 





#### `public virtual `[`Format`](#structscy_1_1av_1_1Format)` & getByID(const std::string & id)` 





#### `public virtual `[`Format`](#structscy_1_1av_1_1Format)` & getOrDefault(const std::string & name)` 





#### `public virtual `[`Format`](#structscy_1_1av_1_1Format)` & getDefault()` 



If a default has been specified it will be returned, other the format with the highest priority will take precedence.

#### `public virtual void registerFormat(const `[`Format`](#structscy_1_1av_1_1Format)` & format)` 



Registers the given media format overriding existing media formats of the same name.

#### `public virtual bool unregisterFormat(const std::string & name)` 



Unregisters the media format matching the given name.

#### `public virtual void setDefault(const std::string & name)` 

Sets the default fallback media format.



#### `public virtual bool exists(const std::string & name)` 





#### `public virtual void clear()` 





#### `public virtual FormatList formats() const` 





# class `scy::av::FPSCounter` 


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

#### `public double fps` 





#### `public std::int64_t frames` 





#### `public inline  FPSCounter()` 





#### `public inline void reset()` 





#### `public inline void tick()` 





# class `scy::av::FPSLimiter` 

```
class scy::av::FPSLimiter
  : public scy::PacketProcessor
```  



This class limits the throughput rate of IPackets in a [PacketStream](#classscy_1_1PacketStream). If the throughput rate exceeds the max specified FPS then packets will be dropped.

Note that revious processors must not fragment packets otherwise this class will not be accurate, and the packet drop rate will be too high.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public bool videoOnly` | 
`public  PacketProcessor(this-> emitter)` | 
`public  _max(max)` | 
`public  _videoOnly(videoOnly)` | 
`protected int _max` | 
`protected bool _videoOnly` | 
`protected `[`legacy::FPSCounter`](#structscy_1_1av_1_1legacy_1_1FPSCounter)` _counter` | 

## Members

#### `public bool videoOnly` 





#### `public  PacketProcessor(this-> emitter)` 





#### `public  _max(max)` 





#### `public  _videoOnly(videoOnly)` 





#### `protected int _max` 





#### `protected bool _videoOnly` 





#### `protected `[`legacy::FPSCounter`](#structscy_1_1av_1_1legacy_1_1FPSCounter)` _counter` 





# class `scy::av::ICapture` 

```
class scy::av::ICapture
  : public scy::PacketStreamAdapter
  : public scy::async::Startable
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`PacketSignal`](#classscy_1_1SignalBase)` emitter` | 
`public inline  ICapture()` | 
`public inline virtual  ~ICapture()` | 
`public void start()` | 
`public void stop()` | 
`public void getEncoderFormat(`[`Format`](#structscy_1_1av_1_1Format)` & iformat)` | Sets the input format for encoding with this capture device.
`public inline virtual void onStreamStateChange(const `[`PacketStreamState`](#structscy_1_1PacketStreamState)` &)` | 

## Members

#### `public `[`PacketSignal`](#classscy_1_1SignalBase)` emitter` 





#### `public inline  ICapture()` 





#### `public inline virtual  ~ICapture()` 





#### `public void start()` 





#### `public void stop()` 





#### `public void getEncoderFormat(`[`Format`](#structscy_1_1av_1_1Format)` & iformat)` 

Sets the input format for encoding with this capture device.



#### `public inline virtual void onStreamStateChange(const `[`PacketStreamState`](#structscy_1_1PacketStreamState)` &)` 



Called by the [PacketStream](#classscy_1_1PacketStream) to notify when the internal [Stream](#classscy_1_1Stream) state changes. On receiving the Stopped state, it is the responsibility of the adapter to have ceased all outgoing packet transmission, especially in multi-thread scenarios.

# class `scy::av::IDeviceManager` 




A platform independent interface to enumerate audio and video devices on the system.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`Signal2`](#classscy_1_1Signal2)`< bool &, bool & > DevicesChanged` | 
`public inline virtual  ~IDeviceManager()` | 
`public bool initialize()` | Initialization.
`public void uninitialize()` | 
`public bool getAudioInputDevices(std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devices)` | [Device](#structscy_1_1av_1_1Device) enumeration.
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

#### `public `[`Signal2`](#classscy_1_1Signal2)`< bool &, bool & > DevicesChanged` 



Signals on DevicesChanged. Arg 1 is true when device is video, false for audio Arg 2 is true when device connects, flase on disconnection

#### `public inline virtual  ~IDeviceManager()` 





#### `public bool initialize()` 

Initialization.



#### `public void uninitialize()` 





#### `public bool getAudioInputDevices(std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devices)` 

[Device](#structscy_1_1av_1_1Device) enumeration.



#### `public bool getAudioOutputDevices(std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devices)` 





#### `public bool getAudioInputDevice(`[`Device`](#structscy_1_1av_1_1Device)` & out,const std::string & name,int id)` 





#### `public bool getAudioInputDevice(`[`Device`](#structscy_1_1av_1_1Device)` & out,int id)` 





#### `public bool getAudioOutputDevice(`[`Device`](#structscy_1_1av_1_1Device)` & out,const std::string & name,int id)` 





#### `public bool getAudioOutputDevice(`[`Device`](#structscy_1_1av_1_1Device)` & out,int id)` 





#### `public bool getVideoCaptureDevices(std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devs)` 





#### `public bool getVideoCaptureDevice(`[`Device`](#structscy_1_1av_1_1Device)` & out,int id)` 





#### `public bool getVideoCaptureDevice(`[`Device`](#structscy_1_1av_1_1Device)` & out,const std::string & name,int id)` 





#### `public bool getDefaultAudioInputDevice(`[`Device`](#structscy_1_1av_1_1Device)` & device)` 





#### `public bool getDefaultAudioOutputDevice(`[`Device`](#structscy_1_1av_1_1Device)` & device)` 





#### `public bool getDefaultVideoCaptureDevice(`[`Device`](#structscy_1_1av_1_1Device)` & device)` 





#### `public int getCapabilities()` 

Capabilities.



#### `public void print(std::ostream & ost)` 





# class `scy::av::IEncoder` 

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

#### `public void initialize()` 





#### `public void uninitialize()` 





#### `public `[`EncoderOptions`](#structscy_1_1av_1_1EncoderOptions)` & options()` 





#### `public inline virtual bool isNone() const` 





#### `public inline virtual bool isReady() const` 





#### `public inline virtual bool isEncoding() const` 





#### `public inline virtual bool isActive() const` 





#### `public inline virtual bool isStopped() const` 





#### `public inline virtual bool isError() const` 





# class `scy::av::LinuxDeviceManager` 

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

#### `public  LinuxDeviceManager()` 





#### `public virtual  ~LinuxDeviceManager()` 





#### `public virtual bool getCameras(std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devs)` 





# class `scy::av::MacDeviceManager` 

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

#### `public  MacDeviceManager()` 





#### `public virtual  ~MacDeviceManager()` 





#### `public virtual bool getCameras(std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devs)` 





# class `scy::av::MacDeviceWatcher` 

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

#### `public  explicit MacDeviceWatcher(`[`IDeviceManager`](#classscy_1_1av_1_1IDeviceManager)` * dm)` 





#### `public virtual  ~MacDeviceWatcher()` 





#### `public virtual bool start()` 





#### `public virtual void stop()` 





# class `scy::av::RealtimePacketQueue` 

```
class scy::av::RealtimePacketQueue
  : public scy::AsyncPacketQueue< PacketT >
```  

This class emits media packets based on their realtime pts value.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  base_t(maxSize)` | 

## Members

#### `public  base_t(maxSize)` 





# struct `scy::av::AudioCodec` 

```
struct scy::av::AudioCodec
  : public scy::av::Codec
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public int channels` | 
`public std::string sampleFmt` | 
`public  AudioCodec()` | int frameSize; // Frame size (optional value set by encoder/decoder) /// int bufferSize; // Buffer size (optional value set by encoder/decoder)
`public  AudioCodec(const std::string & name,int channels,int sampleRate,int bitRate,const std::string & sampleFmt)` | 
`public  AudioCodec(const std::string & name,const std::string & encoder,int channels,int sampleRate,int bitRate,const std::string & sampleFmt)` | 
`public virtual  ~AudioCodec()` | AudioCodec(const AudioCodec& r);.
`public virtual std::string toString() const` | 
`public virtual void print(std::ostream & ost)` | 

## Members

#### `public int channels` 





#### `public std::string sampleFmt` 



One of: u8, s16, s32, flt, dbl, u8p, s16p, s32p, fltp, dblp

#### `public  AudioCodec()` 

int frameSize; // Frame size (optional value set by encoder/decoder) /// int bufferSize; // Buffer size (optional value set by encoder/decoder)



#### `public  AudioCodec(const std::string & name,int channels,int sampleRate,int bitRate,const std::string & sampleFmt)` 





#### `public  AudioCodec(const std::string & name,const std::string & encoder,int channels,int sampleRate,int bitRate,const std::string & sampleFmt)` 





#### `public virtual  ~AudioCodec()` 

AudioCodec(const AudioCodec& r);.



#### `public virtual std::string toString() const` 





#### `public virtual void print(std::ostream & ost)` 





# struct `scy::av::AudioPacket` 

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

#### `public std::size_t numSamples` 





#### `public inline  AudioPacket(std::uint8_t * data,std::size_t size,std::size_t numSamples,std::int64_t time)` 





#### `public inline virtual  ~AudioPacket()` 

AudioPacket(const AudioPacket& r) : /// MediaPacket(r) {}.



#### `public inline virtual `[`IPacket`](#classscy_1_1IPacket)` * clone() const` 





#### `public inline virtual std::uint8_t * samples() const` 





#### `public inline virtual const char * className() const` 





# struct `scy::av::Codec` 






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

#### `public std::string name` 

The display name for this codec.



#### `public std::string encoder` 

The encoder name for FFmpeg.



#### `public int sampleRate` 

The sampling rate or RTP clock rate.



#### `public int bitRate` 

The bit rate to encode at.



#### `public int quality` 

Optional quality value, variable range depending on codec.



#### `public bool enabled` 

Weather or not the codec is available for use.



#### `public  Codec()` 





#### `public  Codec(const std::string & name,int sampleRate,int bitRate,bool enabled)` 





#### `public  Codec(const std::string & name,const std::string & encoder,int sampleRate,int bitRate,bool enabled)` 





#### `public virtual  ~Codec()` 

Codec(const Codec& r);.



#### `public virtual std::string toString() const` 





#### `public virtual void print(std::ostream & ost)` 





# struct `scy::av::Device` 


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

#### `public std::string type` 





#### `public int id` 





#### `public std::string name` 





#### `public std::string guid` 





#### `public bool isDefault` 





#### `public bool isAvailable` 





#### `public  Device()` 





#### `public  Device(const std::string & type,int id,const std::string & name,const std::string & guid,bool isDefault,bool isAvailable)` 





#### `public void print(std::ostream & os)` 





#### `public inline bool operator==(const `[`Device`](#structscy_1_1av_1_1Device)` & that) const` 





# struct `scy::av::EncoderOptions` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`Format`](#structscy_1_1av_1_1Format)` iformat` | 
`public `[`Format`](#structscy_1_1av_1_1Format)` oformat` | 
`public std::string ifile` | 
`public std::string ofile` | 
`public long duration` | 
`public inline  EncoderOptions(const `[`Format`](#structscy_1_1av_1_1Format)` & iformat,const `[`Format`](#structscy_1_1av_1_1Format)` & oformat,const std::string & ifile,const std::string & ofile,long duration)` | 
`public inline virtual  ~EncoderOptions()` | 

## Members

#### `public `[`Format`](#structscy_1_1av_1_1Format)` iformat` 





#### `public `[`Format`](#structscy_1_1av_1_1Format)` oformat` 





#### `public std::string ifile` 





#### `public std::string ofile` 





#### `public long duration` 





#### `public inline  EncoderOptions(const `[`Format`](#structscy_1_1av_1_1Format)` & iformat,const `[`Format`](#structscy_1_1av_1_1Format)` & oformat,const std::string & ifile,const std::string & ofile,long duration)` 





#### `public inline virtual  ~EncoderOptions()` 





# struct `scy::av::EncoderState` 

```
struct scy::av::EncoderState
  : public scy::State
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual std::string str(unsigned int id) const` | 

## Members

#### `public inline virtual std::string str(unsigned int id) const` 





# struct `scy::av::Format` 




Defines a media container format which is available through the [Format](#structscy_1_1av_1_1Format) Registry for encoding/decoding. A format defined preferred default values for each codec.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::string name` | Base [Format](#structscy_1_1av_1_1Format) Variables.
`public std::string id` | The short name of this format.
`public `[`VideoCodec`](#structscy_1_1av_1_1VideoCodec)` video` | The video codec.
`public `[`AudioCodec`](#structscy_1_1av_1_1AudioCodec)` audio` | The audio codec.
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

#### `public std::string name` 

Base [Format](#structscy_1_1av_1_1Format) Variables.

The display name of this format.

#### `public std::string id` 

The short name of this format.



#### `public `[`VideoCodec`](#structscy_1_1av_1_1VideoCodec)` video` 

The video codec.



#### `public `[`AudioCodec`](#structscy_1_1av_1_1AudioCodec)` audio` 

The audio codec.



#### `public int priority` 

The priority this format will be displayed on the list.



#### `public  Format()` 

Ctors/Dtors.



#### `public  Format(const std::string & name,const std::string & id,const `[`VideoCodec`](#structscy_1_1av_1_1VideoCodec)` & video,const `[`AudioCodec`](#structscy_1_1av_1_1AudioCodec)` & audio,int priority)` 





#### `public  Format(const std::string & name,const std::string & id,const `[`VideoCodec`](#structscy_1_1av_1_1VideoCodec)` & video,int priority)` 





#### `public  Format(const std::string & name,const std::string & id,const `[`AudioCodec`](#structscy_1_1av_1_1AudioCodec)` & audio,int priority)` 





#### `public  Format(const `[`Format`](#structscy_1_1av_1_1Format)` & r)` 





#### `public `[`Type`](#group__av_1gaee45ab380f3ab82ddee6a904902e441a)` type() const` 

The format type.



#### `public virtual std::string toString() const` 





#### `public virtual void print(std::ostream & ost)` 





# struct `scy::av::MediaPacket` 

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

#### `public std::int64_t time` 





#### `public inline  MediaPacket(std::uint8_t * data,std::size_t size,std::int64_t time)` 





#### `public inline  MediaPacket(const `[`MediaPacket`](#structscy_1_1av_1_1MediaPacket)` & r)` 





#### `public inline virtual  ~MediaPacket()` 





#### `public inline virtual `[`IPacket`](#classscy_1_1IPacket)` * clone() const` 





#### `public inline virtual const char * className() const` 





# struct `scy::av::VideoCodec` 

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

#### `public int width` 





#### `public int height` 





#### `public double fps` 





#### `public std::string pixelFmt` 





#### `public  VideoCodec()` 





#### `public  VideoCodec(const std::string & name,int width,int height,double fps,int bitRate,int sampleRate,const std::string & pixelFmt)` 





#### `public  VideoCodec(const std::string & name,const std::string & encoder,int width,int height,double fps,int bitRate,int sampleRate,const std::string & pixelFmt)` 





#### `public  VideoCodec(const `[`VideoCodec`](#structscy_1_1av_1_1VideoCodec)` & r)` 





#### `public virtual  ~VideoCodec()` 





#### `public virtual std::string toString() const` 





#### `public virtual void print(std::ostream & ost)` 





# struct `scy::av::VideoPacket` 

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

#### `public int width` 





#### `public int height` 





#### `public bool iframe` 





#### `public inline  VideoPacket(std::uint8_t * data,std::size_t size,int width,int height,std::int64_t time)` 





#### `public inline  VideoPacket(const `[`VideoPacket`](#structscy_1_1av_1_1VideoPacket)` & r)` 





#### `public inline virtual  ~VideoPacket()` 





#### `public inline virtual `[`IPacket`](#classscy_1_1IPacket)` * clone() const` 





#### `public inline virtual const char * className() const` 





# namespace `scy::av::legacy`



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`struct `[``scy::av::legacy::FPSCounter``](#structscy_1_1av_1_1legacy_1_1FPSCounter)    | 
# struct `scy::av::legacy::FPSCounter` 






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

#### `public clock_t start` 





#### `public clock_t end` 





#### `public std::int64_t frames` 





#### `public double total` 





#### `public double fps` 





#### `public inline  FPSCounter()` 





#### `public inline void tick()` 





#### `public inline void reset()` 





#### `public inline bool started()` 





#### `public inline void startFrame()` 





#### `public inline double endFrame()` 





# class `scy::av::IDeviceManager` 




A platform independent interface to enumerate audio and video devices on the system.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`Signal2`](#classscy_1_1Signal2)`< bool &, bool & > DevicesChanged` | 
`public inline virtual  ~IDeviceManager()` | 
`public bool initialize()` | Initialization.
`public void uninitialize()` | 
`public bool getAudioInputDevices(std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devices)` | [Device](#structscy_1_1av_1_1Device) enumeration.
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

#### `public `[`Signal2`](#classscy_1_1Signal2)`< bool &, bool & > DevicesChanged` 



Signals on DevicesChanged. Arg 1 is true when device is video, false for audio Arg 2 is true when device connects, flase on disconnection

#### `public inline virtual  ~IDeviceManager()` 





#### `public bool initialize()` 

Initialization.



#### `public void uninitialize()` 





#### `public bool getAudioInputDevices(std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devices)` 

[Device](#structscy_1_1av_1_1Device) enumeration.



#### `public bool getAudioOutputDevices(std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devices)` 





#### `public bool getAudioInputDevice(`[`Device`](#structscy_1_1av_1_1Device)` & out,const std::string & name,int id)` 





#### `public bool getAudioInputDevice(`[`Device`](#structscy_1_1av_1_1Device)` & out,int id)` 





#### `public bool getAudioOutputDevice(`[`Device`](#structscy_1_1av_1_1Device)` & out,const std::string & name,int id)` 





#### `public bool getAudioOutputDevice(`[`Device`](#structscy_1_1av_1_1Device)` & out,int id)` 





#### `public bool getVideoCaptureDevices(std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devs)` 





#### `public bool getVideoCaptureDevice(`[`Device`](#structscy_1_1av_1_1Device)` & out,int id)` 





#### `public bool getVideoCaptureDevice(`[`Device`](#structscy_1_1av_1_1Device)` & out,const std::string & name,int id)` 





#### `public bool getDefaultAudioInputDevice(`[`Device`](#structscy_1_1av_1_1Device)` & device)` 





#### `public bool getDefaultAudioOutputDevice(`[`Device`](#structscy_1_1av_1_1Device)` & device)` 





#### `public bool getDefaultVideoCaptureDevice(`[`Device`](#structscy_1_1av_1_1Device)` & device)` 





#### `public int getCapabilities()` 

Capabilities.



#### `public void print(std::ostream & ost)` 





# class `scy::av::DeviceWatcher` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  explicit DeviceWatcher(`[`IDeviceManager`](#classscy_1_1av_1_1IDeviceManager)` *)` | 
`public inline virtual  ~DeviceWatcher()` | 
`public inline virtual bool start()` | 
`public inline virtual void stop()` | 

## Members

#### `public inline  explicit DeviceWatcher(`[`IDeviceManager`](#classscy_1_1av_1_1IDeviceManager)` *)` 





#### `public inline virtual  ~DeviceWatcher()` 





#### `public inline virtual bool start()` 





#### `public inline virtual void stop()` 





# class `scy::av::DeviceManagerFactory` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------

## Members

# class `scy::av::DeviceManager` 

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
`public virtual bool getAudioInputDevices(std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devices)` | [Device](#structscy_1_1av_1_1Device) enumeration.
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

#### `public  DeviceManager()` 





#### `public virtual  ~DeviceManager()` 





#### `public virtual bool initialize()` 

Initialization.



#### `public virtual void uninitialize()` 





#### `public virtual int getCapabilities()` 

Capabilities.



#### `public virtual bool getAudioInputDevices(std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devices)` 

[Device](#structscy_1_1av_1_1Device) enumeration.



#### `public virtual bool getAudioOutputDevices(std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devices)` 





#### `public virtual bool getAudioInputDevice(`[`Device`](#structscy_1_1av_1_1Device)` & out,const std::string & name,int id)` 





#### `public virtual bool getAudioInputDevice(`[`Device`](#structscy_1_1av_1_1Device)` & out,int id)` 





#### `public virtual bool getAudioOutputDevice(`[`Device`](#structscy_1_1av_1_1Device)` & out,const std::string & name,int id)` 





#### `public virtual bool getAudioOutputDevice(`[`Device`](#structscy_1_1av_1_1Device)` & out,int id)` 





#### `public virtual bool getVideoCaptureDevices(std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devs)` 





#### `public virtual bool getVideoCaptureDevice(`[`Device`](#structscy_1_1av_1_1Device)` & out,const std::string & name,int id)` 





#### `public virtual bool getVideoCaptureDevice(`[`Device`](#structscy_1_1av_1_1Device)` & out,int id)` 

Returns the video capture device at the given system index.



#### `public virtual bool getDefaultAudioInputDevice(`[`Device`](#structscy_1_1av_1_1Device)` & device)` 





#### `public virtual bool getDefaultAudioOutputDevice(`[`Device`](#structscy_1_1av_1_1Device)` & device)` 





#### `public virtual bool getDefaultVideoCaptureDevice(`[`Device`](#structscy_1_1av_1_1Device)` & device)` 





#### `public inline bool initialized() const` 





#### `public virtual void print(std::ostream & ost)` 





#### `protected virtual bool getDefaultAudioDevice(bool input,`[`Device`](#structscy_1_1av_1_1Device)` & device)` 





#### `protected virtual bool getAudioDevices(bool input,std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devs)` 





#### `protected virtual bool getAudioDevice(bool input,`[`Device`](#structscy_1_1av_1_1Device)` & out,const std::string & name,int id)` 





#### `protected virtual bool getAudioDevice(bool input,`[`Device`](#structscy_1_1av_1_1Device)` & out,int id)` 





#### `protected void setInitialized(bool initialized)` 





#### `protected void setWatcher(`[`DeviceWatcher`](#classscy_1_1av_1_1DeviceWatcher)` * watcher)` 





#### `protected `[`DeviceWatcher`](#classscy_1_1av_1_1DeviceWatcher)` * watcher()` 





# class `scy::av::FakeDeviceManager` 

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

#### `public inline  FakeDeviceManager()` 





#### `public inline virtual bool initialize()` 

Initialization.



#### `public inline virtual void uninitialize()` 





#### `public inline virtual int getCapabilities()` 

Capabilities.



#### `public inline virtual bool getMicrophones(std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devs)` 





#### `public inline virtual bool getSpeakers(std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devs)` 





#### `public inline virtual bool getMicrophone(const std::string & name,`[`Device`](#structscy_1_1av_1_1Device)` & out)` 





#### `public inline virtual bool getSpeaker(const std::string & name,`[`Device`](#structscy_1_1av_1_1Device)` & out)` 





#### `public inline virtual bool getCameras(std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devs)` 





#### `public inline virtual bool getDefaultCamera(`[`Device`](#structscy_1_1av_1_1Device)` & device)` 





#### `public inline void setMicrophones(const std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devices)` 





#### `public inline void setSpeakers(const std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devices)` 





#### `public inline void setCameras(const std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devices)` 





#### `public inline virtual bool getCamera(const std::string & name,`[`Device`](#structscy_1_1av_1_1Device)` & out)` 





#### `public inline bool getAudioDevice(bool input,const std::string & name,`[`Device`](#structscy_1_1av_1_1Device)` & out)` 





# class `scy::av::LinuxDeviceManager` 

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

#### `public  LinuxDeviceManager()` 





#### `public virtual  ~LinuxDeviceManager()` 





#### `public virtual bool getCameras(std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devs)` 





# class `scy::av::MacDeviceManager` 

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

#### `public  MacDeviceManager()` 





#### `public virtual  ~MacDeviceManager()` 





#### `public virtual bool getCameras(std::vector< `[`Device`](#structscy_1_1av_1_1Device)` > & devs)` 





# class `scy::av::FLVMetadataInjector` 

```
class scy::av::FLVMetadataInjector
  : public scy::PacketProcessor
```  



This class implements a packetizer which appends correct stream headers and modifies the timestamp of FLV packets so Adobe's Flash Player will play our videos mid-stream.

This adapter is useful for multicast situations where we don't have the option of restarting the encoder stream.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`PacketSignal`](#classscy_1_1SignalBase)` emitter` | 
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
`protected `[`Format`](#structscy_1_1av_1_1Format)` _format` | 
`protected bool _initial` | 
`protected bool _modifyingStream` | 
`protected bool _waitingForKeyframe` | 
`protected std::uint32_t _timestampOffset` | 
`protected `[`legacy::FPSCounter`](#structscy_1_1av_1_1legacy_1_1FPSCounter)` _fpsCounter` | 

## Members

#### `public `[`PacketSignal`](#classscy_1_1SignalBase)` emitter` 





#### `public  IPacketizer(this-> emitter)` 





#### `public  _format(format)` 





#### `public  _initial(true)` 





#### `public  _modifyingStream(false)` 





#### `public  _waitingForKeyframe(false)` 





#### `public inline  _timestampOffset(0) const` 





#### `public inline virtual void onStreamStateChange(const `[`PacketStreamState`](#structscy_1_1PacketStreamState)` & state)` 

whenever the stream is restarted.

This method is called by the Packet [Stream](#classscy_1_1Stream)

#### `public inline virtual void process(`[`IPacket`](#classscy_1_1IPacket)` & packet)` 



This method performs processing on the given packet and emits the result.

Note: If packet processing is async (the packet is not in the current thread scope) then packet data must be copied. Copied data can be freed directly aFter the async call to emit() the outgoing packet.

#### `public inline virtual void fastUpdateTimestamp(char * buf,std::uint32_t timestamp)` 



Updates the timestamp in the given FLV tag buffer. No more need to copy data with this method. Caution: this method does not check buffer size.

#### `public inline virtual bool fastIsFLVHeader(char * buf)` 

Caution: this method does not check buffer size.



#### `public inline virtual bool fastIsFLVKeyFrame(char * buf)` 

Caution: this method does not check buffer size.



#### `public inline virtual void writeFLVHeader(`[`BitWriter`](#classscy_1_1BitWriter)` & writer)` 





#### `public inline std::int64_t doubleToInt(double d)` 





#### `public inline virtual void writeAMFSring(`[`BitWriter`](#classscy_1_1BitWriter)` & writer,const char * val)` 

AMF Helpers.



#### `public inline virtual void writeAMFDouble(`[`BitWriter`](#classscy_1_1BitWriter)` & writer,double val)` 





#### `public inline virtual void writeAMFBool(`[`BitWriter`](#classscy_1_1BitWriter)` & writer,bool val)` 





#### `protected `[`Format`](#structscy_1_1av_1_1Format)` _format` 





#### `protected bool _initial` 





#### `protected bool _modifyingStream` 





#### `protected bool _waitingForKeyframe` 





#### `protected std::uint32_t _timestampOffset` 





#### `protected `[`legacy::FPSCounter`](#structscy_1_1av_1_1legacy_1_1FPSCounter)` _fpsCounter` 





# class `scy::av::MacDeviceWatcher` 

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

#### `public  explicit MacDeviceWatcher(`[`IDeviceManager`](#classscy_1_1av_1_1IDeviceManager)` * dm)` 





#### `public virtual  ~MacDeviceWatcher()` 





#### `public virtual bool start()` 





#### `public virtual void stop()` 





# class `scy::av::FormatRegistry` 






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

#### `public  FormatRegistry()` 





#### `public virtual  ~FormatRegistry()` 





#### `public virtual `[`Format`](#structscy_1_1av_1_1Format)` & get(const std::string & name)` 





#### `public virtual `[`Format`](#structscy_1_1av_1_1Format)` & getByID(const std::string & id)` 





#### `public virtual `[`Format`](#structscy_1_1av_1_1Format)` & getOrDefault(const std::string & name)` 





#### `public virtual `[`Format`](#structscy_1_1av_1_1Format)` & getDefault()` 



If a default has been specified it will be returned, other the format with the highest priority will take precedence.

#### `public virtual void registerFormat(const `[`Format`](#structscy_1_1av_1_1Format)` & format)` 



Registers the given media format overriding existing media formats of the same name.

#### `public virtual bool unregisterFormat(const std::string & name)` 



Unregisters the media format matching the given name.

#### `public virtual void setDefault(const std::string & name)` 

Sets the default fallback media format.



#### `public virtual bool exists(const std::string & name)` 





#### `public virtual void clear()` 





#### `public virtual FormatList formats() const` 





# class `scy::av::FPSCounter` 


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

#### `public double fps` 





#### `public std::int64_t frames` 





#### `public inline  FPSCounter()` 





#### `public inline void reset()` 





#### `public inline void tick()` 





# class `scy::av::FPSLimiter` 

```
class scy::av::FPSLimiter
  : public scy::PacketProcessor
```  



This class limits the throughput rate of IPackets in a [PacketStream](#classscy_1_1PacketStream). If the throughput rate exceeds the max specified FPS then packets will be dropped.

Note that revious processors must not fragment packets otherwise this class will not be accurate, and the packet drop rate will be too high.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public bool videoOnly` | 
`public  PacketProcessor(this-> emitter)` | 
`public  _max(max)` | 
`public  _videoOnly(videoOnly)` | 
`protected int _max` | 
`protected bool _videoOnly` | 
`protected `[`legacy::FPSCounter`](#structscy_1_1av_1_1legacy_1_1FPSCounter)` _counter` | 

## Members

#### `public bool videoOnly` 





#### `public  PacketProcessor(this-> emitter)` 





#### `public  _max(max)` 





#### `public  _videoOnly(videoOnly)` 





#### `protected int _max` 





#### `protected bool _videoOnly` 





#### `protected `[`legacy::FPSCounter`](#structscy_1_1av_1_1legacy_1_1FPSCounter)` _counter` 





# class `scy::av::ICapture` 

```
class scy::av::ICapture
  : public scy::PacketStreamAdapter
  : public scy::async::Startable
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`PacketSignal`](#classscy_1_1SignalBase)` emitter` | 
`public inline  ICapture()` | 
`public inline virtual  ~ICapture()` | 
`public void start()` | 
`public void stop()` | 
`public void getEncoderFormat(`[`Format`](#structscy_1_1av_1_1Format)` & iformat)` | Sets the input format for encoding with this capture device.
`public inline virtual void onStreamStateChange(const `[`PacketStreamState`](#structscy_1_1PacketStreamState)` &)` | 

## Members

#### `public `[`PacketSignal`](#classscy_1_1SignalBase)` emitter` 





#### `public inline  ICapture()` 





#### `public inline virtual  ~ICapture()` 





#### `public void start()` 





#### `public void stop()` 





#### `public void getEncoderFormat(`[`Format`](#structscy_1_1av_1_1Format)` & iformat)` 

Sets the input format for encoding with this capture device.



#### `public inline virtual void onStreamStateChange(const `[`PacketStreamState`](#structscy_1_1PacketStreamState)` &)` 



Called by the [PacketStream](#classscy_1_1PacketStream) to notify when the internal [Stream](#classscy_1_1Stream) state changes. On receiving the Stopped state, it is the responsibility of the adapter to have ceased all outgoing packet transmission, especially in multi-thread scenarios.

# class `scy::av::IEncoder` 

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

#### `public void initialize()` 





#### `public void uninitialize()` 





#### `public `[`EncoderOptions`](#structscy_1_1av_1_1EncoderOptions)` & options()` 





#### `public inline virtual bool isNone() const` 





#### `public inline virtual bool isReady() const` 





#### `public inline virtual bool isEncoding() const` 





#### `public inline virtual bool isActive() const` 





#### `public inline virtual bool isStopped() const` 





#### `public inline virtual bool isError() const` 





# class `scy::av::RealtimePacketQueue` 

```
class scy::av::RealtimePacketQueue
  : public scy::AsyncPacketQueue< PacketT >
```  

This class emits media packets based on their realtime pts value.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  base_t(maxSize)` | 

## Members

#### `public  base_t(maxSize)` 





# struct `scy::av::Device` 


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

#### `public std::string type` 





#### `public int id` 





#### `public std::string name` 





#### `public std::string guid` 





#### `public bool isDefault` 





#### `public bool isAvailable` 





#### `public  Device()` 





#### `public  Device(const std::string & type,int id,const std::string & name,const std::string & guid,bool isDefault,bool isAvailable)` 





#### `public void print(std::ostream & os)` 





#### `public inline bool operator==(const `[`Device`](#structscy_1_1av_1_1Device)` & that) const` 





# struct `scy::av::Codec` 






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

#### `public std::string name` 

The display name for this codec.



#### `public std::string encoder` 

The encoder name for FFmpeg.



#### `public int sampleRate` 

The sampling rate or RTP clock rate.



#### `public int bitRate` 

The bit rate to encode at.



#### `public int quality` 

Optional quality value, variable range depending on codec.



#### `public bool enabled` 

Weather or not the codec is available for use.



#### `public  Codec()` 





#### `public  Codec(const std::string & name,int sampleRate,int bitRate,bool enabled)` 





#### `public  Codec(const std::string & name,const std::string & encoder,int sampleRate,int bitRate,bool enabled)` 





#### `public virtual  ~Codec()` 

Codec(const Codec& r);.



#### `public virtual std::string toString() const` 





#### `public virtual void print(std::ostream & ost)` 





# struct `scy::av::AudioCodec` 

```
struct scy::av::AudioCodec
  : public scy::av::Codec
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public int channels` | 
`public std::string sampleFmt` | 
`public  AudioCodec()` | int frameSize; // Frame size (optional value set by encoder/decoder) /// int bufferSize; // Buffer size (optional value set by encoder/decoder)
`public  AudioCodec(const std::string & name,int channels,int sampleRate,int bitRate,const std::string & sampleFmt)` | 
`public  AudioCodec(const std::string & name,const std::string & encoder,int channels,int sampleRate,int bitRate,const std::string & sampleFmt)` | 
`public virtual  ~AudioCodec()` | AudioCodec(const AudioCodec& r);.
`public virtual std::string toString() const` | 
`public virtual void print(std::ostream & ost)` | 

## Members

#### `public int channels` 





#### `public std::string sampleFmt` 



One of: u8, s16, s32, flt, dbl, u8p, s16p, s32p, fltp, dblp

#### `public  AudioCodec()` 

int frameSize; // Frame size (optional value set by encoder/decoder) /// int bufferSize; // Buffer size (optional value set by encoder/decoder)



#### `public  AudioCodec(const std::string & name,int channels,int sampleRate,int bitRate,const std::string & sampleFmt)` 





#### `public  AudioCodec(const std::string & name,const std::string & encoder,int channels,int sampleRate,int bitRate,const std::string & sampleFmt)` 





#### `public virtual  ~AudioCodec()` 

AudioCodec(const AudioCodec& r);.



#### `public virtual std::string toString() const` 





#### `public virtual void print(std::ostream & ost)` 





# struct `scy::av::VideoCodec` 

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

#### `public int width` 





#### `public int height` 





#### `public double fps` 





#### `public std::string pixelFmt` 





#### `public  VideoCodec()` 





#### `public  VideoCodec(const std::string & name,int width,int height,double fps,int bitRate,int sampleRate,const std::string & pixelFmt)` 





#### `public  VideoCodec(const std::string & name,const std::string & encoder,int width,int height,double fps,int bitRate,int sampleRate,const std::string & pixelFmt)` 





#### `public  VideoCodec(const `[`VideoCodec`](#structscy_1_1av_1_1VideoCodec)` & r)` 





#### `public virtual  ~VideoCodec()` 





#### `public virtual std::string toString() const` 





#### `public virtual void print(std::ostream & ost)` 





# struct `scy::av::Format` 




Defines a media container format which is available through the [Format](#structscy_1_1av_1_1Format) Registry for encoding/decoding. A format defined preferred default values for each codec.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::string name` | Base [Format](#structscy_1_1av_1_1Format) Variables.
`public std::string id` | The short name of this format.
`public `[`VideoCodec`](#structscy_1_1av_1_1VideoCodec)` video` | The video codec.
`public `[`AudioCodec`](#structscy_1_1av_1_1AudioCodec)` audio` | The audio codec.
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

#### `public std::string name` 

Base [Format](#structscy_1_1av_1_1Format) Variables.

The display name of this format.

#### `public std::string id` 

The short name of this format.



#### `public `[`VideoCodec`](#structscy_1_1av_1_1VideoCodec)` video` 

The video codec.



#### `public `[`AudioCodec`](#structscy_1_1av_1_1AudioCodec)` audio` 

The audio codec.



#### `public int priority` 

The priority this format will be displayed on the list.



#### `public  Format()` 

Ctors/Dtors.



#### `public  Format(const std::string & name,const std::string & id,const `[`VideoCodec`](#structscy_1_1av_1_1VideoCodec)` & video,const `[`AudioCodec`](#structscy_1_1av_1_1AudioCodec)` & audio,int priority)` 





#### `public  Format(const std::string & name,const std::string & id,const `[`VideoCodec`](#structscy_1_1av_1_1VideoCodec)` & video,int priority)` 





#### `public  Format(const std::string & name,const std::string & id,const `[`AudioCodec`](#structscy_1_1av_1_1AudioCodec)` & audio,int priority)` 





#### `public  Format(const `[`Format`](#structscy_1_1av_1_1Format)` & r)` 





#### `public `[`Type`](#group__av_1gaee45ab380f3ab82ddee6a904902e441a)` type() const` 

The format type.



#### `public virtual std::string toString() const` 





#### `public virtual void print(std::ostream & ost)` 





# struct `scy::av::legacy::FPSCounter` 






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

#### `public clock_t start` 





#### `public clock_t end` 





#### `public std::int64_t frames` 





#### `public double total` 





#### `public double fps` 





#### `public inline  FPSCounter()` 





#### `public inline void tick()` 





#### `public inline void reset()` 





#### `public inline bool started()` 





#### `public inline void startFrame()` 





#### `public inline double endFrame()` 





# struct `scy::av::EncoderState` 

```
struct scy::av::EncoderState
  : public scy::State
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual std::string str(unsigned int id) const` | 

## Members

#### `public inline virtual std::string str(unsigned int id) const` 





# struct `scy::av::EncoderOptions` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`Format`](#structscy_1_1av_1_1Format)` iformat` | 
`public `[`Format`](#structscy_1_1av_1_1Format)` oformat` | 
`public std::string ifile` | 
`public std::string ofile` | 
`public long duration` | 
`public inline  EncoderOptions(const `[`Format`](#structscy_1_1av_1_1Format)` & iformat,const `[`Format`](#structscy_1_1av_1_1Format)` & oformat,const std::string & ifile,const std::string & ofile,long duration)` | 
`public inline virtual  ~EncoderOptions()` | 

## Members

#### `public `[`Format`](#structscy_1_1av_1_1Format)` iformat` 





#### `public `[`Format`](#structscy_1_1av_1_1Format)` oformat` 





#### `public std::string ifile` 





#### `public std::string ofile` 





#### `public long duration` 





#### `public inline  EncoderOptions(const `[`Format`](#structscy_1_1av_1_1Format)` & iformat,const `[`Format`](#structscy_1_1av_1_1Format)` & oformat,const std::string & ifile,const std::string & ofile,long duration)` 





#### `public inline virtual  ~EncoderOptions()` 





# struct `scy::av::MediaPacket` 

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

#### `public std::int64_t time` 





#### `public inline  MediaPacket(std::uint8_t * data,std::size_t size,std::int64_t time)` 





#### `public inline  MediaPacket(const `[`MediaPacket`](#structscy_1_1av_1_1MediaPacket)` & r)` 





#### `public inline virtual  ~MediaPacket()` 





#### `public inline virtual `[`IPacket`](#classscy_1_1IPacket)` * clone() const` 





#### `public inline virtual const char * className() const` 





# struct `scy::av::VideoPacket` 

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

#### `public int width` 





#### `public int height` 





#### `public bool iframe` 





#### `public inline  VideoPacket(std::uint8_t * data,std::size_t size,int width,int height,std::int64_t time)` 





#### `public inline  VideoPacket(const `[`VideoPacket`](#structscy_1_1av_1_1VideoPacket)` & r)` 





#### `public inline virtual  ~VideoPacket()` 





#### `public inline virtual `[`IPacket`](#classscy_1_1IPacket)` * clone() const` 





#### `public inline virtual const char * className() const` 





# struct `scy::av::AudioPacket` 

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

#### `public std::size_t numSamples` 





#### `public inline  AudioPacket(std::uint8_t * data,std::size_t size,std::size_t numSamples,std::int64_t time)` 





#### `public inline virtual  ~AudioPacket()` 

AudioPacket(const AudioPacket& r) : /// MediaPacket(r) {}.



#### `public inline virtual `[`IPacket`](#classscy_1_1IPacket)` * clone() const` 





#### `public inline virtual std::uint8_t * samples() const` 





#### `public inline virtual const char * className() const` 





