{#avmodule}

# av

Audio/video encoding, decoding, device capture, and media formats.

### Namespaces

| Name | Description |
|------|-------------|
| [`v4l2`](#v4l2) | Linux V4L2 device enumeration helpers. |
| [`coreaudio`](#coreaudio) | Apple CoreAudio device enumeration helpers. |
| [`legacy`](#legacy) | Legacy timing helpers kept for compatibility with older media pipelines. |
| [`avfoundation`](#avfoundation) | Apple AVFoundation device enumeration helpers. |
| [`mediafoundation`](#mediafoundation) | Windows Media Foundation device enumeration helpers. |
| [`wasapi`](#wasapi) | Windows WASAPI device enumeration helpers. |

### Classes

| Name | Description |
|------|-------------|
| [`LinuxDeviceWatcher`](#linuxdevicewatcher) | Monitors device add/remove events via libudev. |
| [`ICapture`](#icapture) | Abstract interface for audio and video capture devices. |
| [`IEncoder`](#iencoder) | This is the abstract class for all encoders. |
| [`AppleDeviceWatcher`](#appledevicewatcher) | Monitors device add/remove events via AVFoundation notifications and CoreAudio property listeners. |
| [`FPSCounter`](#fpscounter-2) | FPS counter based on the simple moving average (SMA) algorithm. |
| [`FPSLimiter`](#fpslimiter) | [PacketStream](base.md#packetstream) processor that caps packet throughput to a maximum FPS. |
| [`AudioCapture`](#audiocapture) | Cross-platform audio capture device backed by FFmpeg input devices. |
| [`MediaCapture`](#mediacapture) | Unified capture and decode source for files and live media devices. |
| [`VideoCapture`](#videocapture) | Cross-platform video device capturer backed by FFmpeg avdevice. |
| [`DeviceWatcher`](#devicewatcher) | Base class for platform-specific device change monitors. |
| [`DeviceManager`](#devicemanager) | Enumerates and manages system audio and video devices. |
| [`FormatRegistry`](#formatregistry) | [Singleton](base.md#singleton) registry of available media container formats for encoding and decoding. |
| [`WindowsDeviceWatcher`](#windowsdevicewatcher) | Monitors device add/remove events via IMMNotificationClient (audio) and RegisterDeviceNotification (video). |
| [`MultiplexEncoder`](#multiplexencoder) | Multiplexing encoder that writes synchronized audio and video streams. |
| [`AudioPacketEncoder`](#audiopacketencoder) | [PacketProcessor](base.md#packetprocessor) that encodes raw audio samples ([PlanarAudioPacket](#planaraudiopacket) or [AudioPacket](#audiopacket)) into compressed packets via [AudioEncoder](#audioencoder). |
| [`VideoPacketEncoder`](#videopacketencoder) | [PacketProcessor](base.md#packetprocessor) that encodes raw video frames ([PlanarVideoPacket](#planarvideopacket) or [VideoPacket](#videopacket)) into compressed packets via [VideoEncoder](#videoencoder). |
| [`RealtimePacketQueue`](#realtimepacketqueue) | [Queue](base.md#queue) that emits media packets in presentation-timestamp order relative to a realtime clock. |
| [`MultiplexPacketEncoder`](#multiplexpacketencoder) | Encodes and multiplexes a realtime video stream form audio / video capture sources. FFmpeg is used for encoding. |
| [`Codec`](#codec) | [Codec](#codec) for encoding/decoding media. |
| [`AudioCodec`](#audiocodec) | Audio codec parameters including channels, sample rate, and sample format. |
| [`VideoCodec`](#videocodec) | Video codec parameters including resolution, frame rate, and pixel format. |
| [`Deleter`](#deleter-1) | RAII helpers for owning FFmpeg allocation types. |
| [`Deleterp`](#deleterp) | [Deleter](#deleter-1) adaptor for functions like av_freep that take a pointer to a pointer. |
| [`Format`](#format) | Defines a media container format which is available through the [FormatRegistry](#formatregistry) for encoding or decoding. |
| [`EncoderState`](#encoderstate) | [State](base.md#state) machine states for the encoder pipeline. |
| [`EncoderOptions`](#encoderoptions) | [Configuration](base.md#configuration) options for audio and video encoders. |
| [`AudioBuffer`](#audiobuffer) | FIFO buffer for queuing audio samples between encoding stages. |
| [`AudioContext`](#audiocontext) | Base context for audio encoding and decoding via FFmpeg. |
| [`AudioDecoder`](#audiodecoder) | Decodes compressed audio packets into raw sample frames. |
| [`AudioEncoder`](#audioencoder) | Encodes raw audio samples into a compressed format. |
| [`VideoContext`](#videocontext) | Base video context from which all video encoders and decoders derive. |
| [`VideoDecoder`](#videodecoder) | Decodes compressed video packets into raw frames. |
| [`VideoEncoder`](#videoencoder) | Encodes raw video frames into a compressed format. |
| [`Device`](#device) | Represents a system audio, video or render device. |
| [`AudioResampler`](#audioresampler) | Converts audio samples between different formats, sample rates, and channel layouts. |
| [`VideoConverter`](#videoconverter) | Converts video frames between pixel formats and resolutions. |
| [`MediaPacket`](#mediapacket) | Timestamped media packet carrying raw audio or video data. |
| [`VideoPacket`](#videopacket) | Video packet for interleaved formats. |
| [`PlanarVideoPacket`](#planarvideopacket) | Video packet for planar formats. |
| [`AudioPacket`](#audiopacket) | Audio packet for interleaved formats. |
| [`PlanarAudioPacket`](#planaraudiopacket) | Audio packet for planar formats. |

### Macros

| Name | Description |
|------|-------------|
| [`MAX_AUDIO_PACKET_SIZE`](#max_audio_packet_size)  | 1 second of 48khz 32bit audio |
| [`MAX_ENCODE_DURATION`](#max_encode_duration)  | 3 hours |

---

{#max_audio_packet_size}

#### MAX_AUDIO_PACKET_SIZE

```cpp
MAX_AUDIO_PACKET_SIZE()
```

1 second of 48khz 32bit audio

---

{#max_encode_duration}

#### MAX_ENCODE_DURATION

```cpp
MAX_ENCODE_DURATION()
```

3 hours

### Enumerations

| Name | Description |
|------|-------------|
| [`MediaCapabilities`](#mediacapabilities)  | Bitmask of media capabilities detected on this system. |

---

{#mediacapabilities}

#### MediaCapabilities

```cpp
enum MediaCapabilities
```

Bitmask of media capabilities detected on this system.

| Value | Description |
|-------|-------------|
| `AUDIO_RECV` | Audio capture or decode is available. |
| `AUDIO_SEND` | Audio playback or encode is available. |
| `VIDEO_RECV` | Video capture or decode is available. |
| `VIDEO_SEND` | Video render or encode is available. |

### Typedefs

| Return | Name | Description |
|--------|------|-------------|
| `std::list< Codec >` | [`CodecList`](#codeclist)  | List of codec value objects. |
| `std::list< Codec * >` | [`CodecPList`](#codecplist)  | List of codec pointers. |
| `std::unique_ptr< AVFrame, Deleterp< AVFrame, void, av_frame_free > >` | [`AVFrameHolder`](#avframeholder)  | Owning AVFrame pointer released with av_frame_free(). |
| `std::unique_ptr< AVFormatContext, Deleter< AVFormatContext, void, avformat_free_context > >` | [`AVFormatContextHolder`](#avformatcontextholder)  | Owning AVFormatContext pointer released with avformat_free_context(). |
| `std::unique_ptr< AVCodecContext, Deleterp< AVCodecContext, void, avcodec_free_context > >` | [`AVCodecContextHolder`](#avcodeccontextholder)  | Owning AVCodecContext pointer released with avcodec_free_context(). |
| `std::unique_ptr< AVDictionary *, Deleter< AVDictionary *, void, av_dict_free > >` | [`AVDictionaryCleanup`](#avdictionarycleanup)  | Cleanup wrapper for AVDictionary* values freed with av_dict_free(). |
| `std::unique_ptr< AVPacket, Deleterp< AVPacket, void, av_packet_free > >` | [`AVPacketHolder`](#avpacketholder)  | Owning AVPacket pointer released with av_packet_free(). |
| `std::vector< Format >` | [`FormatList`](#formatlist)  | List of container format value objects. |
| `std::vector< Format * >` | [`FormatPList`](#formatplist)  | List of container format pointers. |
| `IEncoder` | [`IPacketEncoder`](#ipacketencoder)  | Legacy alias for [IEncoder](#iencoder) kept for 0.8.x compatibility. |

---

{#codeclist}

#### CodecList

```cpp
using CodecList = std::list< Codec >
```

List of codec value objects.

---

{#codecplist}

#### CodecPList

```cpp
using CodecPList = std::list< Codec * >
```

List of codec pointers.

---

{#avframeholder}

#### AVFrameHolder

```cpp
using AVFrameHolder = std::unique_ptr< AVFrame, Deleterp< AVFrame, void, av_frame_free > >
```

Owning AVFrame pointer released with av_frame_free().

---

{#avformatcontextholder}

#### AVFormatContextHolder

```cpp
using AVFormatContextHolder = std::unique_ptr< AVFormatContext, Deleter< AVFormatContext, void, avformat_free_context > >
```

Owning AVFormatContext pointer released with avformat_free_context().

---

{#avcodeccontextholder}

#### AVCodecContextHolder

```cpp
using AVCodecContextHolder = std::unique_ptr< AVCodecContext, Deleterp< AVCodecContext, void, avcodec_free_context > >
```

Owning AVCodecContext pointer released with avcodec_free_context().

---

{#avdictionarycleanup}

#### AVDictionaryCleanup

```cpp
using AVDictionaryCleanup = std::unique_ptr< AVDictionary *, Deleter< AVDictionary *, void, av_dict_free > >
```

Cleanup wrapper for AVDictionary* values freed with av_dict_free().

---

{#avpacketholder}

#### AVPacketHolder

```cpp
using AVPacketHolder = std::unique_ptr< AVPacket, Deleterp< AVPacket, void, av_packet_free > >
```

Owning AVPacket pointer released with av_packet_free().

---

{#formatlist}

#### FormatList

```cpp
using FormatList = std::vector< Format >
```

List of container format value objects.

---

{#formatplist}

#### FormatPList

```cpp
using FormatPList = std::vector< Format * >
```

List of container format pointers.

---

{#ipacketencoder}

#### IPacketEncoder

```cpp
using IPacketEncoder = IEncoder
```

Legacy alias for [IEncoder](#iencoder) kept for 0.8.x compatibility.

### Functions

| Return | Name | Description |
|--------|------|-------------|
| `int64_t` | [`fpsToInterval`](#fpstointerval) `inline` | Convert a frame rate to a nanosecond frame interval. |
| `int` | [`intervalToFps`](#intervaltofps) `inline` | Convert a nanosecond frame interval to a frame rate. |
| `float` | [`intervalToFpsFloat`](#intervaltofpsfloat) `inline` | Convert a nanosecond frame interval to a floating-point frame rate. |
| `void` | [`initializeFFmpeg`](#initializeffmpeg)  | Initialize the FFmpeg library. |
| `void` | [`uninitializeFFmpeg`](#uninitializeffmpeg)  | Uninitializes the FFmpeg library. |
| `std::string` | [`averror`](#averror)  | Get an error string for the given error code. |
| `void` | [`printInputFormats`](#printinputformats)  | Print all available FFmpeg demuxer (input) format names to the given stream. |
| `void` | [`printOutputFormats`](#printoutputformats)  | Print all available FFmpeg muxer (output) format names to the given stream. |
| `void` | [`printEncoders`](#printencoders)  | Print all available FFmpeg encoder names to the given stream. |
| `AVPacketHolder` | [`makeOwnedPacket`](#makeownedpacket)  | Allocate an owning AVPacket with FFmpeg-required padding and timestamp metadata. The packet payload is copied into FFmpeg-managed storage, which is required by decoders such as H.264 that may overread AV_INPUT_BUFFER_PADDING_SIZE bytes. |
| `void` | [`initAudioCodecFromContext`](#initaudiocodecfromcontext)  | Populate an [AudioCodec](#audiocodec) from an open AVCodecContext. |
| `AVSampleFormat` | [`selectSampleFormat`](#selectsampleformat)  | Select the best supported sample format for a codec given the requested parameters. Returns the requested format if supported, otherwise the first format with the same planarity. |
| `bool` | [`isSampleFormatSupported`](#issampleformatsupported)  | Check whether a specific sample format is in the codec's supported list. |
| `bool` | [`formatIsPlanar`](#formatisplanar)  | Return true if the named sample format is planar (e.g. "fltp", "s16p"). |
| `bool` | [`formatIsPlanar`](#formatisplanar-1)  | Return true if the given AVSampleFormat is planar. |
| `AVFrame *` | [`createVideoFrame`](#createvideoframe)  | Allocate a new AVFrame with the given pixel format and dimensions. Uses av_frame_get_buffer for reference-counted allocation with 16-byte alignment. |
| `AVFrame *` | [`cloneVideoFrame`](#clonevideoframe)  | Perform a deep copy of an AVFrame including its buffer data and properties. |
| `void` | [`initVideoCodecFromContext`](#initvideocodecfromcontext)  | Populate a [VideoCodec](#videocodec) from an open AVStream and AVCodecContext. |
| `AVPixelFormat` | [`selectPixelFormat`](#selectpixelformat)  | Select the best supported pixel format for a codec given the requested parameters. Returns the requested format if supported, otherwise the first format with the same plane count. |
| `std::optional< std::pair< const AVInputFormat *, std::string > >` | [`parseDeviceUrl`](#parsedeviceurl)  | Parse a libavdevice URL scheme. |

---

{#fpstointerval}

#### fpsToInterval

`inline`

```cpp
inline int64_t fpsToInterval(int fps)
```

Convert a frame rate to a nanosecond frame interval. 
#### Parameters
* `fps` The frame rate in frames per second. 

#### Returns
The interval in nanoseconds between frames, or the minimum interval if fps is zero.

---

{#intervaltofps}

#### intervalToFps

`inline`

```cpp
inline int intervalToFps(int64_t interval)
```

Convert a nanosecond frame interval to a frame rate. 
#### Parameters
* `interval` The nanosecond interval between frames. 

#### Returns
The frame rate in frames per second, or zero if interval is zero.

---

{#intervaltofpsfloat}

#### intervalToFpsFloat

`inline`

```cpp
inline float intervalToFpsFloat(int64_t interval)
```

Convert a nanosecond frame interval to a floating-point frame rate. 
#### Parameters
* `interval` The nanosecond interval between frames. 

#### Returns
The frame rate in frames per second as a float, or 0.0f if interval is zero.

---

{#initializeffmpeg}

#### initializeFFmpeg

```cpp
void initializeFFmpeg()
```

Initialize the FFmpeg library.

---

{#uninitializeffmpeg}

#### uninitializeFFmpeg

```cpp
void uninitializeFFmpeg()
```

Uninitializes the FFmpeg library.

---

{#averror}

#### averror

```cpp
std::string averror(const int error)
```

Get an error string for the given error code.

---

{#printinputformats}

#### printInputFormats

```cpp
void printInputFormats(std::ostream & ost, const char * delim = " ")
```

Print all available FFmpeg demuxer (input) format names to the given stream. 
#### Parameters
* `ost` The output stream to write to. 

* `delim` Delimiter inserted between each name.

---

{#printoutputformats}

#### printOutputFormats

```cpp
void printOutputFormats(std::ostream & ost, const char * delim = " ")
```

Print all available FFmpeg muxer (output) format names to the given stream. 
#### Parameters
* `ost` The output stream to write to. 

* `delim` Delimiter inserted between each name.

---

{#printencoders}

#### printEncoders

```cpp
void printEncoders(std::ostream & ost, const char * delim = " ")
```

Print all available FFmpeg encoder names to the given stream. 
#### Parameters
* `ost` The output stream to write to. 

* `delim` Delimiter inserted between each name.

---

{#makeownedpacket}

#### makeOwnedPacket

```cpp
AVPacketHolder makeOwnedPacket(const MediaPacket & packet, int streamIndex, AVRational timeBase)
```

Allocate an owning AVPacket with FFmpeg-required padding and timestamp metadata. The packet payload is copied into FFmpeg-managed storage, which is required by decoders such as H.264 that may overread AV_INPUT_BUFFER_PADDING_SIZE bytes.

---

{#initaudiocodecfromcontext}

#### initAudioCodecFromContext

```cpp
void initAudioCodecFromContext(const AVCodecContext * ctx, AudioCodec & params)
```

Populate an [AudioCodec](#audiocodec) from an open AVCodecContext. 
#### Parameters
* `ctx` The FFmpeg codec context to read from. 

* `params` The [AudioCodec](#audiocodec) struct to fill with codec, channel, sample rate, and format info.

---

{#selectsampleformat}

#### selectSampleFormat

```cpp
AVSampleFormat selectSampleFormat(const AVCodec * codec, av::AudioCodec & params)
```

Select the best supported sample format for a codec given the requested parameters. Returns the requested format if supported, otherwise the first format with the same planarity. 
#### Parameters
* `codec` The FFmpeg codec whose supported formats are queried. 

* `params` The audio codec parameters specifying the desired sample format. 

#### Returns
The chosen AVSampleFormat, or AV_SAMPLE_FMT_NONE if none is compatible.

---

{#issampleformatsupported}

#### isSampleFormatSupported

```cpp
bool isSampleFormatSupported(const AVCodec * codec, enum AVSampleFormat sampleFormat)
```

Check whether a specific sample format is in the codec's supported list. 
#### Parameters
* `codec` The FFmpeg codec to query. 

* `sampleFormat` The AVSampleFormat to look up. 

#### Returns
True if the format is supported.

---

{#formatisplanar}

#### formatIsPlanar

```cpp
bool formatIsPlanar(const std::string & pixfmt)
```

Return true if the named sample format is planar (e.g. "fltp", "s16p"). 
#### Parameters
* `pixfmt` The sample format name string.

---

{#formatisplanar-1}

#### formatIsPlanar

```cpp
bool formatIsPlanar(AVSampleFormat format)
```

Return true if the given AVSampleFormat is planar. 
#### Parameters
* `format` The AVSampleFormat to check.

---

{#createvideoframe}

#### createVideoFrame

```cpp
AVFrame * createVideoFrame(AVPixelFormat pixelFmt, int width, int height)
```

Allocate a new AVFrame with the given pixel format and dimensions. Uses av_frame_get_buffer for reference-counted allocation with 16-byte alignment. 
#### Parameters
* `pixelFmt` The pixel format for the frame. 

* `width` The frame width in pixels. 

* `height` The frame height in pixels. 

#### Returns
A newly allocated AVFrame, or nullptr on failure.

---

{#clonevideoframe}

#### cloneVideoFrame

```cpp
AVFrame * cloneVideoFrame(AVFrame * source)
```

Perform a deep copy of an AVFrame including its buffer data and properties. 
#### Parameters
* `source` The source frame to copy. 

#### Returns
A newly allocated AVFrame with copied data. Caller owns the result.

---

{#initvideocodecfromcontext}

#### initVideoCodecFromContext

```cpp
void initVideoCodecFromContext(const AVStream * stream, const AVCodecContext * ctx, VideoCodec & params)
```

Populate a [VideoCodec](#videocodec) from an open AVStream and AVCodecContext. 
#### Parameters
* `stream` The AVStream for frame rate information (may be null). 

* `ctx` The FFmpeg codec context to read from. 

* `params` The [VideoCodec](#videocodec) struct to fill with codec, dimension, and format info.

---

{#selectpixelformat}

#### selectPixelFormat

```cpp
AVPixelFormat selectPixelFormat(const AVCodec * codec, VideoCodec & params)
```

Select the best supported pixel format for a codec given the requested parameters. Returns the requested format if supported, otherwise the first format with the same plane count. 
#### Parameters
* `codec` The FFmpeg codec whose supported formats are queried. 

* `params` The video codec parameters specifying the desired pixel format. 

#### Returns
The chosen AVPixelFormat.

---

{#parsedeviceurl}

#### parseDeviceUrl

```cpp
std::optional< std::pair< const AVInputFormat *, std::string > > parseDeviceUrl(std::string_view source)
```

Parse a libavdevice URL scheme.

Recognises: avfoundation:<spec> (macOS, e.g. "0:0", "0:none") [v4l2](#v4l2):<path> (Linux, e.g. "/dev/video0") dshow:<spec> (Windows, e.g. "video=Logitech HD WebCam")

On match returns the libavformat input format (looked up via `av_find_input_format`) and the portion of `source` after the scheme prefix. Returns `std::nullopt` for non-device sources (file paths, `rtsp://`, `rtmp://`, etc.), leaving the caller to use FFmpeg's auto-detection.

Throws `std::runtime_error` if a recognised scheme has no matching input format on this build (libavdevice not linked, or the backend is not compiled in on this platform).

{#v4l2}

# v4l2

Linux V4L2 device enumeration helpers.

### Functions

| Return | Name | Description |
|--------|------|-------------|
| `bool` | [`getDeviceList`](#getdevicelist)  | Enumerate video input devices using V4L2. Populates device capabilities (resolutions, frame rates, pixel formats). |

---

{#getdevicelist}

#### getDeviceList

```cpp
bool getDeviceList(Device::Type type, std::vector< av::Device > & devices)
```

Enumerate video input devices using V4L2. Populates device capabilities (resolutions, frame rates, pixel formats). 
#### Parameters
* `type` Must be [Device::VideoInput](#structicy_1_1av_1_1Device_1aee01e78ad670f447f295fcdcc9b65c9fa88dbbc17e909f22c94224b6a98ebae87). 

* `devices` Output vector to fill with discovered devices. 

#### Returns
True if enumeration succeeded.

{#coreaudio}

# coreaudio

Apple CoreAudio device enumeration helpers.

### Functions

| Return | Name | Description |
|--------|------|-------------|
| `bool` | [`getDeviceList`](#getdevicelist-1)  | Enumerate audio input and output devices using CoreAudio. Populates audio capabilities (sample rates, channels). |

---

{#getdevicelist-1}

#### getDeviceList

```cpp
bool getDeviceList(Device::Type type, std::vector< av::Device > & devices)
```

Enumerate audio input and output devices using CoreAudio. Populates audio capabilities (sample rates, channels). 
#### Parameters
* `type` The device type to enumerate (AudioInput or AudioOutput). 

* `devices` Output vector to fill with discovered devices. 

#### Returns
True if enumeration succeeded.

{#legacy}

# legacy

Legacy timing helpers kept for compatibility with older media pipelines.

### Classes

| Name | Description |
|------|-------------|
| [`FPSCounter`](#fpscounter) | Legacy frames-per-second counter. |

{#fpscounter}

## FPSCounter

```cpp
#include <icy/av/fpscounter.h>
```

```cpp
struct FPSCounter
```

Defined in src/av/include/icy/av/fpscounter.h:86

Legacy frames-per-second counter.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `clock_t` | [`start`](#start-3)  |  |
| `clock_t` | [`end`](#end)  |  |
| `int64_t` | [`frames`](#frames)  |  |
| `double` | [`total`](#total)  |  |
| `double` | [`fps`](#fps-1)  |  |

---

{#start-3}

#### start

```cpp
clock_t start
```

Defined in src/av/include/icy/av/fpscounter.h:88

---

{#end}

#### end

```cpp
clock_t end
```

Defined in src/av/include/icy/av/fpscounter.h:89

---

{#frames}

#### frames

```cpp
int64_t frames
```

Defined in src/av/include/icy/av/fpscounter.h:90

---

{#total}

#### total

```cpp
double total
```

Defined in src/av/include/icy/av/fpscounter.h:91

---

{#fps-1}

#### fps

```cpp
double fps
```

Defined in src/av/include/icy/av/fpscounter.h:92

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`FPSCounter`](#fpscounter-1) `inline` |  |
| `void` | [`tick`](#tick) `inline` | Begin a new timing cycle (calls [endFrame()](#endframe) first if already started). |
| `void` | [`reset`](#reset) `inline` | Reset all counters to zero. |
| `bool` | [`started`](#started) `inline` |  |
| `void` | [`startFrame`](#startframe) `inline` | Record the frame start time. |
| `double` | [`endFrame`](#endframe) `inline` | Record the frame end time and update the cumulative FPS average. |

---

{#fpscounter-1}

#### FPSCounter

`inline`

```cpp
inline FPSCounter()
```

Defined in src/av/include/icy/av/fpscounter.h:94

---

{#tick}

#### tick

`inline`

```cpp
inline void tick()
```

Defined in src/av/include/icy/av/fpscounter.h:100

Begin a new timing cycle (calls [endFrame()](#endframe) first if already started).

---

{#reset}

#### reset

`inline`

```cpp
inline void reset()
```

Defined in src/av/include/icy/av/fpscounter.h:108

Reset all counters to zero.

---

{#started}

#### started

`inline`

```cpp
inline bool started()
```

Defined in src/av/include/icy/av/fpscounter.h:118

#### Returns
True if [startFrame()](#startframe) has been called and the timer is running.

---

{#startframe}

#### startFrame

`inline`

```cpp
inline void startFrame()
```

Defined in src/av/include/icy/av/fpscounter.h:124

Record the frame start time.

---

{#endframe}

#### endFrame

`inline`

```cpp
inline double endFrame()
```

Defined in src/av/include/icy/av/fpscounter.h:131

Record the frame end time and update the cumulative FPS average. 
#### Returns
The updated FPS value.

{#avfoundation}

# avfoundation

Apple AVFoundation device enumeration helpers.

### Functions

| Return | Name | Description |
|--------|------|-------------|
| `bool` | [`getDeviceList`](#getdevicelist-2)  | Enumerate video input devices using AVFoundation. Populates device capabilities (resolutions, frame rates, pixel formats). |

---

{#getdevicelist-2}

#### getDeviceList

```cpp
bool getDeviceList(Device::Type type, std::vector< av::Device > & devices)
```

Enumerate video input devices using AVFoundation. Populates device capabilities (resolutions, frame rates, pixel formats). 
#### Parameters
* `type` Must be [Device::VideoInput](#structicy_1_1av_1_1Device_1aee01e78ad670f447f295fcdcc9b65c9fa88dbbc17e909f22c94224b6a98ebae87). 

* `devices` Output vector to fill with discovered devices. 

#### Returns
True if enumeration succeeded.

{#mediafoundation}

# mediafoundation

Windows Media Foundation device enumeration helpers.

### Functions

| Return | Name | Description |
|--------|------|-------------|
| `bool` | [`getDeviceList`](#getdevicelist-4)  | Enumerate video and audio input devices using Media Foundation. Populates device capabilities (resolutions, frame rates, pixel formats). |

---

{#getdevicelist-4}

#### getDeviceList

```cpp
bool getDeviceList(Device::Type type, std::vector< av::Device > & devices)
```

Enumerate video and audio input devices using Media Foundation. Populates device capabilities (resolutions, frame rates, pixel formats). 
#### Parameters
* `type` The device type to enumerate (VideoInput or AudioInput). 

* `devices` Output vector to fill with discovered devices. 

#### Returns
True if enumeration succeeded.

{#wasapi}

# wasapi

Windows WASAPI device enumeration helpers.

### Functions

| Return | Name | Description |
|--------|------|-------------|
| `bool` | [`getDeviceList`](#getdevicelist-5)  | Enumerate audio input and output devices using WASAPI. Populates audio capabilities (sample rates, channels, formats). |

---

{#getdevicelist-5}

#### getDeviceList

```cpp
bool getDeviceList(Device::Type type, std::vector< av::Device > & devices)
```

Enumerate audio input and output devices using WASAPI. Populates audio capabilities (sample rates, channels, formats). 
#### Parameters
* `type` The device type to enumerate (AudioInput or AudioOutput). 

* `devices` Output vector to fill with discovered devices. 

#### Returns
True if enumeration succeeded.

{#linuxdevicewatcher}

## LinuxDeviceWatcher

```cpp
#include <icy/av/linux/v4l2.h>
```

```cpp
class LinuxDeviceWatcher
```

Defined in src/av/include/icy/av/linux/v4l2.h:45

> **Inherits:** [`DeviceWatcher`](#devicewatcher)

Monitors device add/remove events via libudev.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`LinuxDeviceWatcher`](#linuxdevicewatcher-1) `explicit` |  |
| `bool` | [`start`](#start) `virtual` `override` | Begin monitoring via libudev inotify events. |
| `void` | [`stop`](#stop) `virtual` `override` | Stop monitoring and close the udev monitor. |

---

{#linuxdevicewatcher-1}

#### LinuxDeviceWatcher

`explicit`

```cpp
explicit LinuxDeviceWatcher(DeviceManager * manager)
```

Defined in src/av/include/icy/av/linux/v4l2.h:49

#### Parameters
* `manager` The [DeviceManager](#devicemanager) whose DevicesChanged signal will be emitted on changes.

---

{#start}

#### start

`virtual` `override`

```cpp
virtual bool start() override
```

Defined in src/av/include/icy/av/linux/v4l2.h:54

Begin monitoring via libudev inotify events. 
#### Returns
True if the udev monitor was successfully started.

---

{#stop}

#### stop

`virtual` `override`

```cpp
virtual void stop() override
```

Defined in src/av/include/icy/av/linux/v4l2.h:57

Stop monitoring and close the udev monitor.

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::unique_ptr< Impl >` | [`_impl`](#_impl)  |  |
| `DeviceManager *` | [`_manager`](#_manager)  |  |

---

{#_impl}

#### _impl

```cpp
std::unique_ptr< Impl > _impl
```

Defined in src/av/include/icy/av/linux/v4l2.h:62

---

{#_manager}

#### _manager

```cpp
DeviceManager * _manager
```

Defined in src/av/include/icy/av/linux/v4l2.h:63

{#icapture}

## ICapture

```cpp
#include <icy/av/icapture.h>
```

```cpp
class ICapture
```

Defined in src/av/include/icy/av/icapture.h:31

> **Inherits:** [`PacketStreamAdapter`](base.md#packetstreamadapter), [`Startable`](base.md#startable)
> **Subclassed by:** [`MediaCapture`](#mediacapture)

Abstract interface for audio and video capture devices.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `PacketSignal` | [`emitter`](#emitter)  |  |

---

{#emitter}

#### emitter

```cpp
PacketSignal emitter
```

Defined in src/av/include/icy/av/icapture.h:89

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`ICapture`](#icapture-1) `inline` |  |
| `void` | [`start`](#start-1) `virtual` `override` | Start capturing and emitting packets. |
| `void` | [`stop`](#stop-1) `virtual` `override` | Stop capturing and release device resources. |
| `void` | [`openFile`](#openfile) `virtual` `inline` | Open a media file as the capture source. |
| `void` | [`close`](#close) `virtual` `inline` | Release the capture source and any associated resources. |
| `void` | [`getEncoderFormat`](#getencoderformat) `virtual` | Populate `iformat` with the encoder-ready format derived from this capture source. |
| `void` | [`getEncoderAudioCodec`](#getencoderaudiocodec) `virtual` `inline` | Populate `params` with the encoder-ready audio codec parameters. |
| `void` | [`getEncoderVideoCodec`](#getencodervideocodec) `virtual` `inline` | Populate `params` with the encoder-ready video codec parameters. |
| `void` | [`openAudio`](#openaudio) `virtual` `inline` | Open an audio capture device with the given parameters. |
| `void` | [`openVideo`](#openvideo) `virtual` `inline` | Open a video capture device with the given parameters. |
| `void` | [`onStreamStateChange`](#onstreamstatechange) `virtual` `inline` `override` | React to [PacketStream](base.md#packetstream) state transitions by starting or stopping capture. |

---

{#icapture-1}

#### ICapture

`inline`

```cpp
inline ICapture()
```

Defined in src/av/include/icy/av/icapture.h:35

---

{#start-1}

#### start

`virtual` `override`

```cpp
virtual void start() override
```

Defined in src/av/include/icy/av/icapture.h:42

Start capturing and emitting packets.

---

{#stop-1}

#### stop

`virtual` `override`

```cpp
virtual void stop() override
```

Defined in src/av/include/icy/av/icapture.h:45

Stop capturing and release device resources.

---

{#openfile}

#### openFile

`virtual` `inline`

```cpp
virtual inline void openFile(const std::string & file)
```

Defined in src/av/include/icy/av/icapture.h:49

Open a media file as the capture source. 
#### Parameters
* `file` Path to the media file.

---

{#close}

#### close

`virtual` `inline`

```cpp
virtual inline void close()
```

Defined in src/av/include/icy/av/icapture.h:52

Release the capture source and any associated resources.

---

{#getencoderformat}

#### getEncoderFormat

`virtual`

```cpp
virtual void getEncoderFormat(Format & iformat)
```

Defined in src/av/include/icy/av/icapture.h:56

Populate `iformat` with the encoder-ready format derived from this capture source. 
#### Parameters
* `iformat` Output [Format](#format) struct to fill.

---

{#getencoderaudiocodec}

#### getEncoderAudioCodec

`virtual` `inline`

```cpp
virtual inline void getEncoderAudioCodec(AudioCodec & params)
```

Defined in src/av/include/icy/av/icapture.h:60

Populate `params` with the encoder-ready audio codec parameters. 
#### Parameters
* `params` Output [AudioCodec](#audiocodec) struct to fill.

---

{#getencodervideocodec}

#### getEncoderVideoCodec

`virtual` `inline`

```cpp
virtual inline void getEncoderVideoCodec(VideoCodec & params)
```

Defined in src/av/include/icy/av/icapture.h:64

Populate `params` with the encoder-ready video codec parameters. 
#### Parameters
* `params` Output [VideoCodec](#videocodec) struct to fill.

---

{#openaudio}

#### openAudio

`virtual` `inline`

```cpp
virtual inline void openAudio(const std::string & device, const AudioCodec & params)
```

Defined in src/av/include/icy/av/icapture.h:69

Open an audio capture device with the given parameters. 
#### Parameters
* `device` The platform audio device identifier. 

* `params` The desired capture parameters.

---

{#openvideo}

#### openVideo

`virtual` `inline`

```cpp
virtual inline void openVideo(const std::string & device, const VideoCodec & params)
```

Defined in src/av/include/icy/av/icapture.h:74

Open a video capture device with the given parameters. 
#### Parameters
* `device` The platform video device identifier. 

* `params` The desired capture parameters.

---

{#onstreamstatechange}

#### onStreamStateChange

`virtual` `inline` `override`

```cpp
virtual inline void onStreamStateChange(const PacketStreamState & state) override
```

Defined in src/av/include/icy/av/icapture.h:77

React to [PacketStream](base.md#packetstream) state transitions by starting or stopping capture.

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`openStream`](#openstream) `virtual` `inline` |  |

---

{#openstream}

#### openStream

`virtual` `inline`

```cpp
virtual inline void openStream(const std::string & filename, const AVInputFormat * inputFormat, AVDictionary ** formatParams)
```

Defined in src/av/include/icy/av/icapture.h:92

{#iencoder}

## IEncoder

```cpp
#include <icy/av/iencoder.h>
```

```cpp
class IEncoder
```

Defined in src/av/include/icy/av/iencoder.h:81

> **Inherits:** [`Stateful< EncoderState >`](base.md#stateful)
> **Subclassed by:** [`MultiplexEncoder`](#multiplexencoder)

This is the abstract class for all encoders.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`init`](#init) `virtual` | Initialise the encoder, open codec contexts, and transition to Ready or Encoding state. |
| `void` | [`uninit`](#uninit) `virtual` | Flush remaining packets, close codec contexts, and release all encoder resources. |
| `void` | [`cleanup`](#cleanup) `virtual` `inline` | Perform any additional cleanup after [uninit()](#uninit). Default is a no-op. |
| `const EncoderOptions &` | [`options`](#options-1) `virtual` `const` `nodiscard` |  |
| `void` | [`createVideo`](#createvideo) `virtual` `inline` | Initialise the video codec context and stream. |
| `void` | [`freeVideo`](#freevideo) `virtual` `inline` | Free the video codec context and stream. |
| `bool` | [`encodeVideo`](#encodevideo) `virtual` `inline` `nodiscard` | Encode a single AVFrame of video. |
| `void` | [`createAudio`](#createaudio) `virtual` `inline` | Initialise the audio codec context and stream. |
| `void` | [`freeAudio`](#freeaudio) `virtual` `inline` | Free the audio codec context and stream. |
| `void` | [`flush`](#flush) `virtual` `inline` | Flush any internally buffered packets to the output. |
| `bool` | [`isNone`](#isnone) `const` `inline` |  |
| `bool` | [`isReady`](#isready) `const` `inline` |  |
| `bool` | [`isEncoding`](#isencoding) `virtual` `const` `inline` |  |
| `bool` | [`isActive`](#isactive) `virtual` `const` `inline` |  |
| `bool` | [`isStopped`](#isstopped) `virtual` `const` `inline` |  |
| `bool` | [`isError`](#iserror) `virtual` `const` `inline` |  |

---

{#init}

#### init

`virtual`

```cpp
virtual void init()
```

Defined in src/av/include/icy/av/iencoder.h:93

Initialise the encoder, open codec contexts, and transition to Ready or Encoding state.

---

{#uninit}

#### uninit

`virtual`

```cpp
virtual void uninit()
```

Defined in src/av/include/icy/av/iencoder.h:96

Flush remaining packets, close codec contexts, and release all encoder resources.

---

{#cleanup}

#### cleanup

`virtual` `inline`

```cpp
virtual inline void cleanup()
```

Defined in src/av/include/icy/av/iencoder.h:99

Perform any additional cleanup after [uninit()](#uninit). Default is a no-op.

---

{#options-1}

#### options

`virtual` `const` `nodiscard`

```cpp
[[nodiscard]] virtual const EncoderOptions & options() const
```

Defined in src/av/include/icy/av/iencoder.h:102

#### Returns
A read-only view of the encoder's configuration options.

---

{#createvideo}

#### createVideo

`virtual` `inline`

```cpp
virtual inline void createVideo()
```

Defined in src/av/include/icy/av/iencoder.h:105

Initialise the video codec context and stream.

---

{#freevideo}

#### freeVideo

`virtual` `inline`

```cpp
virtual inline void freeVideo()
```

Defined in src/av/include/icy/av/iencoder.h:108

Free the video codec context and stream.

---

{#encodevideo}

#### encodeVideo

`virtual` `inline` `nodiscard`

```cpp
[[nodiscard]] virtual inline bool encodeVideo(AVFrame * frame)
```

Defined in src/av/include/icy/av/iencoder.h:113

Encode a single AVFrame of video. 
#### Parameters
* `frame` The source video frame; all fields (format, width, height, pts) must be set. 

#### Returns
True if a packet was successfully encoded and written.

---

{#createaudio}

#### createAudio

`virtual` `inline`

```cpp
virtual inline void createAudio()
```

Defined in src/av/include/icy/av/iencoder.h:116

Initialise the audio codec context and stream.

---

{#freeaudio}

#### freeAudio

`virtual` `inline`

```cpp
virtual inline void freeAudio()
```

Defined in src/av/include/icy/av/iencoder.h:119

Free the audio codec context and stream.

---

{#flush}

#### flush

`virtual` `inline`

```cpp
virtual inline void flush()
```

Defined in src/av/include/icy/av/iencoder.h:122

Flush any internally buffered packets to the output.

---

{#isnone}

#### isNone

`const` `inline`

```cpp
inline bool isNone() const
```

Defined in src/av/include/icy/av/iencoder.h:125

#### Returns
True if the encoder is in the None state.

---

{#isready}

#### isReady

`const` `inline`

```cpp
inline bool isReady() const
```

Defined in src/av/include/icy/av/iencoder.h:128

#### Returns
True if the encoder is ready but not yet encoding.

---

{#isencoding}

#### isEncoding

`virtual` `const` `inline`

```cpp
virtual inline bool isEncoding() const
```

Defined in src/av/include/icy/av/iencoder.h:131

#### Returns
True if the encoder is actively encoding.

---

{#isactive}

#### isActive

`virtual` `const` `inline`

```cpp
virtual inline bool isActive() const
```

Defined in src/av/include/icy/av/iencoder.h:134

#### Returns
True if the encoder is in the Ready or Encoding state.

---

{#isstopped}

#### isStopped

`virtual` `const` `inline`

```cpp
virtual inline bool isStopped() const
```

Defined in src/av/include/icy/av/iencoder.h:137

#### Returns
True if the encoder has stopped.

---

{#iserror}

#### isError

`virtual` `const` `inline`

```cpp
virtual inline bool isError() const
```

Defined in src/av/include/icy/av/iencoder.h:140

#### Returns
True if the encoder is in an error state.

### Public Types

| Name | Description |
|------|-------------|
| [`Type`](#type-3)  |  |

---

{#type-3}

#### Type

```cpp
enum Type
```

Defined in src/av/include/icy/av/iencoder.h:84

| Value | Description |
|-------|-------------|
| `None` | huh? |
| `Video` | video only |
| `Audio` | audio only |
| `Multiplex` | both video & audio |

{#appledevicewatcher}

## AppleDeviceWatcher

```cpp
#include <icy/av/apple/coreaudio.h>
```

```cpp
class AppleDeviceWatcher
```

Defined in src/av/include/icy/av/apple/coreaudio.h:46

> **Inherits:** [`DeviceWatcher`](#devicewatcher)

Monitors device add/remove events via AVFoundation notifications and CoreAudio property listeners.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`AppleDeviceWatcher`](#appledevicewatcher-1) `explicit` |  |
| `bool` | [`start`](#start-2) `virtual` `override` | Begin monitoring for device connect/disconnect events. |
| `void` | [`stop`](#stop-2) `virtual` `override` | Stop monitoring and release all notification observers. |

---

{#appledevicewatcher-1}

#### AppleDeviceWatcher

`explicit`

```cpp
explicit AppleDeviceWatcher(DeviceManager * manager)
```

Defined in src/av/include/icy/av/apple/coreaudio.h:50

#### Parameters
* `manager` The [DeviceManager](#devicemanager) whose DevicesChanged signal will be emitted on changes.

---

{#start-2}

#### start

`virtual` `override`

```cpp
virtual bool start() override
```

Defined in src/av/include/icy/av/apple/coreaudio.h:55

Begin monitoring for device connect/disconnect events. 
#### Returns
True if monitoring was successfully started.

---

{#stop-2}

#### stop

`virtual` `override`

```cpp
virtual void stop() override
```

Defined in src/av/include/icy/av/apple/coreaudio.h:58

Stop monitoring and release all notification observers.

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::unique_ptr< Impl >` | [`_impl`](#_impl-1)  |  |
| `DeviceManager *` | [`_manager`](#_manager-1)  |  |

---

{#_impl-1}

#### _impl

```cpp
std::unique_ptr< Impl > _impl
```

Defined in src/av/include/icy/av/apple/coreaudio.h:63

---

{#_manager-1}

#### _manager

```cpp
DeviceManager * _manager
```

Defined in src/av/include/icy/av/apple/coreaudio.h:64

{#fpscounter-2}

## FPSCounter

```cpp
#include <icy/av/fpscounter.h>
```

```cpp
class FPSCounter
```

Defined in src/av/include/icy/av/fpscounter.h:27

FPS counter based on the simple moving average (SMA) algorithm.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `double` | [`fps`](#fps-2)  |  |
| `int64_t` | [`frames`](#frames-1)  |  |

---

{#fps-2}

#### fps

```cpp
double fps
```

Defined in src/av/include/icy/av/fpscounter.h:48

---

{#frames-1}

#### frames

```cpp
int64_t frames
```

Defined in src/av/include/icy/av/fpscounter.h:49

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`FPSCounter`](#fpscounter-3) `inline` | Initialise the counter with all zeroes. |
| `void` | [`reset`](#reset-1) `inline` | Reset all counters and the SMA window to zero. |
| `void` | [`tick`](#tick-1) `inline` | Record a new frame and update the FPS estimate. Must be called once per frame. Updates `[fps](#fps-2)` and increments `[frames](#frames-1)`. |

---

{#fpscounter-3}

#### FPSCounter

`inline`

```cpp
inline FPSCounter()
```

Defined in src/av/include/icy/av/fpscounter.h:52

Initialise the counter with all zeroes.

---

{#reset-1}

#### reset

`inline`

```cpp
inline void reset()
```

Defined in src/av/include/icy/av/fpscounter.h:55

Reset all counters and the SMA window to zero.

---

{#tick-1}

#### tick

`inline`

```cpp
inline void tick()
```

Defined in src/av/include/icy/av/fpscounter.h:68

Record a new frame and update the FPS estimate. Must be called once per frame. Updates `[fps](#fps-2)` and increments `[frames](#frames-1)`.

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `int` | [`tickIndex`](#tickindex)  |  |
| `clock_t` | [`prevTick`](#prevtick)  |  |
| `clock_t` | [`tickSum`](#ticksum)  |  |
| `clock_t` | [`tickList`](#ticklist)  |  |

---

{#tickindex}

#### tickIndex

```cpp
int tickIndex
```

Defined in src/av/include/icy/av/fpscounter.h:32

---

{#prevtick}

#### prevTick

```cpp
clock_t prevTick
```

Defined in src/av/include/icy/av/fpscounter.h:33

---

{#ticksum}

#### tickSum

```cpp
clock_t tickSum
```

Defined in src/av/include/icy/av/fpscounter.h:34

---

{#ticklist}

#### tickList

```cpp
clock_t tickList
```

Defined in src/av/include/icy/av/fpscounter.h:35

### Private Methods

| Return | Name | Description |
|--------|------|-------------|
| `double` | [`updateAvg`](#updateavg) `inline` |  |

---

{#updateavg}

#### updateAvg

`inline`

```cpp
inline double updateAvg(clock_t newTick)
```

Defined in src/av/include/icy/av/fpscounter.h:37

### Private Static Attributes

| Return | Name | Description |
|--------|------|-------------|
| `const int` | [`MAX`](#max) `static` |  |

---

{#max}

#### MAX

`static`

```cpp
const int MAX = 100
```

Defined in src/av/include/icy/av/fpscounter.h:30

{#fpslimiter}

## FPSLimiter

```cpp
#include <icy/av/fpscounter.h>
```

```cpp
class FPSLimiter
```

Defined in src/av/include/icy/av/fpscounter.h:152

> **Inherits:** [`PacketProcessor`](base.md#packetprocessor)

[PacketStream](base.md#packetstream) processor that caps packet throughput to a maximum FPS.

If the observed frame rate exceeds the configured FPS budget, packets are dropped until the rate falls back under the limit.

Note that previous processors must not fragment packets, otherwise this class will not be accurate, and the packet drop rate will be too high.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `PacketSignal` | [`emitter`](#emitter-1)  |  |

---

{#emitter-1}

#### emitter

```cpp
PacketSignal emitter
```

Defined in src/av/include/icy/av/fpscounter.h:192

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`FPSLimiter`](#fpslimiter-1) `inline` | Construct the limiter. |
| `void` | [`process`](#process-1) `virtual` `inline` | [Process](base.md#process) a packet: forward it if within the rate limit, drop it otherwise. |
| `void` | [`onStreamStateChange`](#onstreamstatechange-1) `virtual` `inline` | Reset the FPS counter when the stream state changes. |

---

{#fpslimiter-1}

#### FPSLimiter

`inline`

```cpp
inline FPSLimiter(int max, bool videoOnly = false)
```

Defined in src/av/include/icy/av/fpscounter.h:158

Construct the limiter. 
#### Parameters
* `max` The maximum allowed frame rate in frames per second. 

* `videoOnly` If true, non-VideoPacket packets are always forwarded regardless of rate.

---

{#process-1}

#### process

`virtual` `inline`

```cpp
virtual inline void process(IPacket & packet)
```

Defined in src/av/include/icy/av/fpscounter.h:167

[Process](base.md#process) a packet: forward it if within the rate limit, drop it otherwise. 
#### Parameters
* `packet` The incoming packet to evaluate.

---

{#onstreamstatechange-1}

#### onStreamStateChange

`virtual` `inline`

```cpp
virtual inline void onStreamStateChange(const PacketStreamState &)
```

Defined in src/av/include/icy/av/fpscounter.h:187

Reset the FPS counter when the stream state changes.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `int` | [`_max`](#_max)  |  |
| `bool` | [`_videoOnly`](#_videoonly)  |  |
| `legacy::FPSCounter` | [`_counter`](#_counter)  |  |

---

{#_max}

#### _max

```cpp
int _max
```

Defined in src/av/include/icy/av/fpscounter.h:195

---

{#_videoonly}

#### _videoOnly

```cpp
bool _videoOnly
```

Defined in src/av/include/icy/av/fpscounter.h:196

---

{#_counter}

#### _counter

```cpp
legacy::FPSCounter _counter
```

Defined in src/av/include/icy/av/fpscounter.h:197

{#audiocapture}

## AudioCapture

```cpp
#include <icy/av/audiocapture.h>
```

```cpp
class AudioCapture
```

Defined in src/av/include/icy/av/audiocapture.h:26

> **Inherits:** [`MediaCapture`](#mediacapture)

Cross-platform audio capture device backed by FFmpeg input devices.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`AudioCapture`](#audiocapture-1)  | Construct without opening a device. Call [openAudio()](#openaudio-1) before [start()](#start-4). |
|  | [`AudioCapture`](#audiocapture-2)  | Construct and immediately open the given audio device using an [AudioCodec](#audiocodec) params struct. |
|  | [`AudioCapture`](#audiocapture-3)  | Construct and immediately open the given audio device with individual parameters. |
| `void` | [`openAudio`](#openaudio-1) `virtual` `override` | Open the given audio device using an [AudioCodec](#audiocodec) params struct. |
| `void` | [`openAudio`](#openaudio-2) `virtual` | Open the given audio device with individual parameters. Configures the FFmpeg input format and passes device options via AVDictionary. If the device cannot satisfy the requested parameters, resampling will be applied. |

---

{#audiocapture-1}

#### AudioCapture

```cpp
AudioCapture()
```

Defined in src/av/include/icy/av/audiocapture.h:32

Construct without opening a device. Call [openAudio()](#openaudio-1) before [start()](#start-4).

---

{#audiocapture-2}

#### AudioCapture

```cpp
AudioCapture(const std::string & device, const av::AudioCodec & params)
```

Defined in src/av/include/icy/av/audiocapture.h:37

Construct and immediately open the given audio device using an [AudioCodec](#audiocodec) params struct. 
#### Parameters
* `device` The platform audio device identifier. 

* `params` The desired capture parameters (channels, sample rate, format).

---

{#audiocapture-3}

#### AudioCapture

```cpp
AudioCapture(const std::string & device, int channels = -1, int sampleRate = -1, const std::string & sampleFmt = "")
```

Defined in src/av/include/icy/av/audiocapture.h:44

Construct and immediately open the given audio device with individual parameters. 
#### Parameters
* `device` The platform audio device identifier. 

* `channels` The desired number of channels, or -1 for device default. 

* `sampleRate` The desired sample rate in Hz, or -1 for device default. 

* `sampleFmt` The desired sample format string, or "" for device default.

---

{#openaudio-1}

#### openAudio

`virtual` `override`

```cpp
virtual void openAudio(const std::string & device, const av::AudioCodec & params) override
```

Defined in src/av/include/icy/av/audiocapture.h:51

Open the given audio device using an [AudioCodec](#audiocodec) params struct. 
#### Parameters
* `device` The platform audio device identifier. 

* `params` The desired capture parameters.

---

{#openaudio-2}

#### openAudio

`virtual`

```cpp
virtual void openAudio(const std::string & device, int channels = -1, int sampleRate = -1, const std::string & sampleFmt = "")
```

Defined in src/av/include/icy/av/audiocapture.h:60

Open the given audio device with individual parameters. Configures the FFmpeg input format and passes device options via AVDictionary. If the device cannot satisfy the requested parameters, resampling will be applied. 
#### Parameters
* `device` The platform audio device identifier. 

* `channels` The desired number of channels, or -1 for device default. 

* `sampleRate` The desired sample rate in Hz, or -1 for device default. 

* `sampleFmt` The desired sample format string, or "" for device default.

### Public Types

| Name | Description |
|------|-------------|
| [`Ptr`](#ptr)  |  |

---

{#ptr}

#### Ptr

```cpp
using Ptr = std::shared_ptr< AudioCapture >
```

Defined in src/av/include/icy/av/audiocapture.h:29

{#mediacapture}

## MediaCapture

```cpp
#include <icy/av/mediacapture.h>
```

```cpp
class MediaCapture
```

Defined in src/av/include/icy/av/mediacapture.h:37

> **Inherits:** [`ICapture`](#icapture), [`Runnable`](base.md#runnable)
> **Subclassed by:** [`AudioCapture`](#audiocapture), [`VideoCapture`](#videocapture)

Unified capture and decode source for files and live media devices.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `NullSignal` | [`Closing`](#closing)  | Signals that the capture thread is closing. This signal is emitted from the capture thread context. |

---

{#closing}

#### Closing

```cpp
NullSignal Closing
```

Defined in src/av/include/icy/av/mediacapture.h:133

Signals that the capture thread is closing. This signal is emitted from the capture thread context.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`MediaCapture`](#mediacapture-1)  |  |
|  | [`MediaCapture`](#mediacapture-2)  | Deleted constructor. |
|  | [`MediaCapture`](#mediacapture-3)  | Deleted constructor. |
| `void` | [`openFile`](#openfile-1) `virtual` `override` | Open a media file for decoding. Automatically detects video and audio streams. |
| `void` | [`close`](#close-5) `virtual` `override` | Stop the capture thread and close the media stream and all decoders. |
| `void` | [`start`](#start-4) `virtual` `override` | Start the background capture and decode thread. Throws std::runtime_error if no media streams have been opened. |
| `void` | [`stop`](#stop-3) `virtual` `override` | [Signal](base.md#signal) the capture thread to stop and join it before returning. |
| `void` | [`run`](#run) `virtual` `override` | Entry point for the background capture thread. Reads and decodes packets from the format context until EOF or [stop()](#stop-3) is called. |
| `void` | [`getEncoderFormat`](#getencoderformat-1) `virtual` `override` | Fill `format` with the combined encoder-ready video and audio codec parameters. |
| `void` | [`getEncoderAudioCodec`](#getencoderaudiocodec-1) `virtual` `override` | Fill `params` with the decoder's output audio codec parameters. Throws std::runtime_error if audio parameters have not been initialised. |
| `void` | [`getEncoderVideoCodec`](#getencodervideocodec-1) `virtual` `override` | Fill `params` with the decoder's output video codec parameters. Throws std::runtime_error if video parameters have not been initialised. |
| `void` | [`setLoopInput`](#setloopinput)  | Continuously loop the input file when set. |
| `void` | [`setLimitFramerate`](#setlimitframerate)  | Limit playback to video FPS. |
| `void` | [`setRealtimePTS`](#setrealtimepts)  | Set to use realtime PTS calculation. This is preferred when using live captures as FFmpeg-provided values are not always reliable. |
| `void` | [`setOpenOptions`](#setopenoptions)  | Set demuxer options applied at the next [openFile()](#openfile-1) call. Keys map to libavformat AVOption entries (e.g. "rtsp_transport", "fflags", "analyzeduration", "probesize"). Useful for live network sources that need low-latency hints. Pass an empty map to clear. |
| `void` | [`setPassthroughVideo`](#setpassthroughvideo)  | Skip the video decoder. Encoded video AVPackets from the input stream are emitted directly as `[av::VideoPacket](#videopacket)` without decoding. The emitted packets carry the source's encoded payload and a `time` in microseconds rescaled from the stream's timebase. The `iframe` flag is set from `AV_PKT_FLAG_KEY`. |
| `AVFormatContext *` | [`formatCtx`](#formatctx) `const` |  |
| `VideoDecoder *` | [`video`](#video-1) `const` |  |
| `AudioDecoder *` | [`audio`](#audio-1) `const` |  |
| `bool` | [`stopping`](#stopping) `const` |  |
| `std::string` | [`error`](#error-2) `const` |  |

---

{#mediacapture-1}

#### MediaCapture

```cpp
MediaCapture()
```

Defined in src/av/include/icy/av/mediacapture.h:43

---

{#mediacapture-2}

#### MediaCapture

```cpp
MediaCapture(const MediaCapture &) = delete
```

Defined in src/av/include/icy/av/mediacapture.h:46

Deleted constructor.

---

{#mediacapture-3}

#### MediaCapture

```cpp
MediaCapture(MediaCapture &&) = delete
```

Defined in src/av/include/icy/av/mediacapture.h:48

Deleted constructor.

---

{#openfile-1}

#### openFile

`virtual` `override`

```cpp
virtual void openFile(const std::string & file) override
```

Defined in src/av/include/icy/av/mediacapture.h:53

Open a media file for decoding. Automatically detects video and audio streams. 
#### Parameters
* `file` Path to the media file.

---

{#close-5}

#### close

`virtual` `override`

```cpp
virtual void close() override
```

Defined in src/av/include/icy/av/mediacapture.h:60

Stop the capture thread and close the media stream and all decoders.

---

{#start-4}

#### start

`virtual` `override`

```cpp
virtual void start() override
```

Defined in src/av/include/icy/av/mediacapture.h:64

Start the background capture and decode thread. Throws std::runtime_error if no media streams have been opened.

---

{#stop-3}

#### stop

`virtual` `override`

```cpp
virtual void stop() override
```

Defined in src/av/include/icy/av/mediacapture.h:67

[Signal](base.md#signal) the capture thread to stop and join it before returning.

---

{#run}

#### run

`virtual` `override`

```cpp
virtual void run() override
```

Defined in src/av/include/icy/av/mediacapture.h:71

Entry point for the background capture thread. Reads and decodes packets from the format context until EOF or [stop()](#stop-3) is called.

---

{#getencoderformat-1}

#### getEncoderFormat

`virtual` `override`

```cpp
virtual void getEncoderFormat(Format & format) override
```

Defined in src/av/include/icy/av/mediacapture.h:75

Fill `format` with the combined encoder-ready video and audio codec parameters. 
#### Parameters
* `format` Output [Format](#format) struct to populate.

---

{#getencoderaudiocodec-1}

#### getEncoderAudioCodec

`virtual` `override`

```cpp
virtual void getEncoderAudioCodec(AudioCodec & params) override
```

Defined in src/av/include/icy/av/mediacapture.h:80

Fill `params` with the decoder's output audio codec parameters. Throws std::runtime_error if audio parameters have not been initialised. 
#### Parameters
* `params` Output [AudioCodec](#audiocodec) struct to populate.

---

{#getencodervideocodec-1}

#### getEncoderVideoCodec

`virtual` `override`

```cpp
virtual void getEncoderVideoCodec(VideoCodec & params) override
```

Defined in src/av/include/icy/av/mediacapture.h:85

Fill `params` with the decoder's output video codec parameters. Throws std::runtime_error if video parameters have not been initialised. 
#### Parameters
* `params` Output [VideoCodec](#videocodec) struct to populate.

---

{#setloopinput}

#### setLoopInput

```cpp
void setLoopInput(bool flag)
```

Defined in src/av/include/icy/av/mediacapture.h:88

Continuously loop the input file when set.

---

{#setlimitframerate}

#### setLimitFramerate

```cpp
void setLimitFramerate(bool flag)
```

Defined in src/av/include/icy/av/mediacapture.h:91

Limit playback to video FPS.

---

{#setrealtimepts}

#### setRealtimePTS

```cpp
void setRealtimePTS(bool flag)
```

Defined in src/av/include/icy/av/mediacapture.h:96

Set to use realtime PTS calculation. This is preferred when using live captures as FFmpeg-provided values are not always reliable.

---

{#setopenoptions}

#### setOpenOptions

```cpp
void setOpenOptions(const std::map< std::string, std::string > & options)
```

Defined in src/av/include/icy/av/mediacapture.h:102

Set demuxer options applied at the next [openFile()](#openfile-1) call. Keys map to libavformat AVOption entries (e.g. "rtsp_transport", "fflags", "analyzeduration", "probesize"). Useful for live network sources that need low-latency hints. Pass an empty map to clear.

---

{#setpassthroughvideo}

#### setPassthroughVideo

```cpp
void setPassthroughVideo(bool flag)
```

Defined in src/av/include/icy/av/mediacapture.h:114

Skip the video decoder. Encoded video AVPackets from the input stream are emitted directly as `[av::VideoPacket](#videopacket)` without decoding. The emitted packets carry the source's encoded payload and a `time` in microseconds rescaled from the stream's timebase. The `iframe` flag is set from `AV_PKT_FLAG_KEY`.

Use case: forwarding already-browser-compatible H.264 to a WebRTC sender without the cost of decode plus re-encode. Must be called before [openFile()](#openfile-1); changing the mode after the stream is open is not supported.

---

{#formatctx}

#### formatCtx

`const`

```cpp
AVFormatContext * formatCtx() const
```

Defined in src/av/include/icy/av/mediacapture.h:117

#### Returns
The underlying AVFormatContext (thread-safe, mutex-protected).

---

{#video-1}

#### video

`const`

```cpp
VideoDecoder * video() const
```

Defined in src/av/include/icy/av/mediacapture.h:120

#### Returns
The active [VideoDecoder](#videodecoder), or nullptr if no video stream was opened.

---

{#audio-1}

#### audio

`const`

```cpp
AudioDecoder * audio() const
```

Defined in src/av/include/icy/av/mediacapture.h:123

#### Returns
The active [AudioDecoder](#audiodecoder), or nullptr if no audio stream was opened.

---

{#stopping}

#### stopping

`const`

```cpp
bool stopping() const
```

Defined in src/av/include/icy/av/mediacapture.h:126

#### Returns
True if the capture thread has been asked to stop.

---

{#error-2}

#### error

`const`

```cpp
std::string error() const
```

Defined in src/av/include/icy/av/mediacapture.h:129

#### Returns
The last error message, or an empty string if no error has occurred.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::mutex` | [`_mutex`](#_mutex)  |  |
| `Thread` | [`_thread`](#_thread)  |  |
| `AVFormatContext *` | [`_formatCtx`](#_formatctx)  |  |
| `std::unique_ptr< VideoDecoder >` | [`_video`](#_video)  |  |
| `std::unique_ptr< AudioDecoder >` | [`_audio`](#_audio)  |  |
| `std::string` | [`_error`](#_error)  |  |
| `std::atomic< bool >` | [`_stopping`](#_stopping)  |  |
| `std::atomic< bool >` | [`_looping`](#_looping)  |  |
| `std::atomic< bool >` | [`_realtime`](#_realtime)  |  |
| `std::atomic< bool >` | [`_ratelimit`](#_ratelimit)  |  |
| `std::atomic< bool >` | [`_passthroughVideo`](#_passthroughvideo)  |  |
| `std::map< std::string, std::string >` | [`_openOptions`](#_openoptions)  |  |
| `AVStream *` | [`_videoStream`](#_videostream)  |  |

---

{#_mutex}

#### _mutex

```cpp
std::mutex _mutex
```

Defined in src/av/include/icy/av/mediacapture.h:146

---

{#_thread}

#### _thread

```cpp
Thread _thread
```

Defined in src/av/include/icy/av/mediacapture.h:147

---

{#_formatctx}

#### _formatCtx

```cpp
AVFormatContext * _formatCtx
```

Defined in src/av/include/icy/av/mediacapture.h:148

---

{#_video}

#### _video

```cpp
std::unique_ptr< VideoDecoder > _video
```

Defined in src/av/include/icy/av/mediacapture.h:149

---

{#_audio}

#### _audio

```cpp
std::unique_ptr< AudioDecoder > _audio
```

Defined in src/av/include/icy/av/mediacapture.h:150

---

{#_error}

#### _error

```cpp
std::string _error
```

Defined in src/av/include/icy/av/mediacapture.h:151

---

{#_stopping}

#### _stopping

```cpp
std::atomic< bool > _stopping
```

Defined in src/av/include/icy/av/mediacapture.h:152

---

{#_looping}

#### _looping

```cpp
std::atomic< bool > _looping
```

Defined in src/av/include/icy/av/mediacapture.h:153

---

{#_realtime}

#### _realtime

```cpp
std::atomic< bool > _realtime
```

Defined in src/av/include/icy/av/mediacapture.h:154

---

{#_ratelimit}

#### _ratelimit

```cpp
std::atomic< bool > _ratelimit
```

Defined in src/av/include/icy/av/mediacapture.h:155

---

{#_passthroughvideo}

#### _passthroughVideo

```cpp
std::atomic< bool > _passthroughVideo {false}
```

Defined in src/av/include/icy/av/mediacapture.h:156

---

{#_openoptions}

#### _openOptions

```cpp
std::map< std::string, std::string > _openOptions
```

Defined in src/av/include/icy/av/mediacapture.h:157

---

{#_videostream}

#### _videoStream

```cpp
AVStream * _videoStream = nullptr
```

Defined in src/av/include/icy/av/mediacapture.h:161

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`openStream`](#openstream-1) `virtual` `override` | Open the underlying media stream. |
| `void` | [`emit`](#emit) `virtual` `override` | Emit an existing packet directly onto the outgoing signal. |

---

{#openstream-1}

#### openStream

`virtual` `override`

```cpp
virtual void openStream(const std::string & filename, const AVInputFormat * inputFormat, AVDictionary ** formatParams) override
```

Defined in src/av/include/icy/av/mediacapture.h:141

Open the underlying media stream.

#### Parameters
* `filename` The file path or device name to open. 

* `inputFormat` The forced input format, or nullptr for auto-detect. 

* `formatParams` Optional format parameters; may be updated by FFmpeg on return.

---

{#emit}

#### emit

`virtual` `override`

```cpp
virtual void emit(IPacket & packet) override
```

Defined in src/av/include/icy/av/mediacapture.h:143

Emit an existing packet directly onto the outgoing signal. 
#### Parameters
* `packet` The packet to forward; must remain valid for the duration of the call.

### Public Types

| Name | Description |
|------|-------------|
| [`Ptr`](#ptr-1)  |  |

---

{#ptr-1}

#### Ptr

```cpp
using Ptr = std::shared_ptr< MediaCapture >
```

Defined in src/av/include/icy/av/mediacapture.h:41

{#videocapture}

## VideoCapture

```cpp
#include <icy/av/videocapture.h>
```

```cpp
class VideoCapture
```

Defined in src/av/include/icy/av/videocapture.h:26

> **Inherits:** [`MediaCapture`](#mediacapture)

Cross-platform video device capturer backed by FFmpeg avdevice.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`VideoCapture`](#videocapture-1)  | Construct without opening a device. Call [openVideo()](#openvideo-1) before [start()](#start-4). |
|  | [`VideoCapture`](#videocapture-2)  | Construct and immediately open the given video device using a [VideoCodec](#videocodec) params struct. |
|  | [`VideoCapture`](#videocapture-3)  | Construct and immediately open the given video device with individual parameters. |
| `void` | [`openVideo`](#openvideo-1) `virtual` `override` | Open the given video device using a [VideoCodec](#videocodec) params struct. |
| `void` | [`openVideo`](#openvideo-2) `virtual` | Open the given video device with individual parameters. Configures the FFmpeg input format and passes device options via AVDictionary. If the device cannot satisfy the requested parameters, pixel format conversion and scaling will be applied by the decoder. |

---

{#videocapture-1}

#### VideoCapture

```cpp
VideoCapture()
```

Defined in src/av/include/icy/av/videocapture.h:32

Construct without opening a device. Call [openVideo()](#openvideo-1) before [start()](#start-4).

---

{#videocapture-2}

#### VideoCapture

```cpp
VideoCapture(const std::string & device, const av::VideoCodec & params)
```

Defined in src/av/include/icy/av/videocapture.h:37

Construct and immediately open the given video device using a [VideoCodec](#videocodec) params struct. 
#### Parameters
* `device` The platform video device identifier (e.g. "/dev/video0" or "0" on Windows). 

* `params` The desired capture parameters (width, height, fps, pixel format).

---

{#videocapture-3}

#### VideoCapture

```cpp
VideoCapture(const std::string & device, int width = 0, int height = 0, double framerate = 0, const std::string & pixelFmt = "")
```

Defined in src/av/include/icy/av/videocapture.h:45

Construct and immediately open the given video device with individual parameters. 
#### Parameters
* `device` The platform video device identifier. 

* `width` The desired frame width in pixels, or 0 for device default. 

* `height` The desired frame height in pixels, or 0 for device default. 

* `framerate` The desired frame rate, or 0 for device default. 

* `pixelFmt` The desired pixel format string, or "" for "yuv420p".

---

{#openvideo-1}

#### openVideo

`virtual` `override`

```cpp
virtual void openVideo(const std::string & device, const av::VideoCodec & params) override
```

Defined in src/av/include/icy/av/videocapture.h:52

Open the given video device using a [VideoCodec](#videocodec) params struct. 
#### Parameters
* `device` The platform video device identifier. 

* `params` The desired capture parameters.

---

{#openvideo-2}

#### openVideo

`virtual`

```cpp
virtual void openVideo(const std::string & device, int width = 0, int height = 0, double framerate = 0, const std::string & pixelFmt = "")
```

Defined in src/av/include/icy/av/videocapture.h:63

Open the given video device with individual parameters. Configures the FFmpeg input format and passes device options via AVDictionary. If the device cannot satisfy the requested parameters, pixel format conversion and scaling will be applied by the decoder. 
#### Parameters
* `device` The platform video device identifier. 

* `width` The desired frame width in pixels, or 0 for device default. 

* `height` The desired frame height in pixels, or 0 for device default. 

* `framerate` The desired frame rate, or 0 for device default. 

* `pixelFmt` The desired pixel format string, or "" for "yuv420p".

### Public Types

| Name | Description |
|------|-------------|
| [`Ptr`](#ptr-2)  |  |

---

{#ptr-2}

#### Ptr

```cpp
using Ptr = std::shared_ptr< VideoCapture >
```

Defined in src/av/include/icy/av/videocapture.h:29

{#devicewatcher}

## DeviceWatcher

```cpp
#include <icy/av/devicemanager.h>
```

```cpp
class DeviceWatcher
```

Defined in src/av/include/icy/av/devicemanager.h:194

> **Subclassed by:** [`AppleDeviceWatcher`](#appledevicewatcher), [`LinuxDeviceWatcher`](#linuxdevicewatcher), [`WindowsDeviceWatcher`](#windowsdevicewatcher)

Base class for platform-specific device change monitors.

Call [start()](#start-5) to begin monitoring and [stop()](#stop-4) to end. The watcher is NOT auto-started; callers must explicitly start it.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`DeviceWatcher`](#devicewatcher-1) `inline` `explicit` |  |
| `bool` | [`start`](#start-5) `virtual` `inline` |  |
| `void` | [`stop`](#stop-4) `virtual` `inline` |  |

---

{#devicewatcher-1}

#### DeviceWatcher

`inline` `explicit`

```cpp
inline explicit DeviceWatcher(DeviceManager *)
```

Defined in src/av/include/icy/av/devicemanager.h:197

---

{#start-5}

#### start

`virtual` `inline`

```cpp
virtual inline bool start()
```

Defined in src/av/include/icy/av/devicemanager.h:199

---

{#stop-4}

#### stop

`virtual` `inline`

```cpp
virtual inline void stop()
```

Defined in src/av/include/icy/av/devicemanager.h:200

{#devicemanager}

## DeviceManager

```cpp
#include <icy/av/devicemanager.h>
```

```cpp
class DeviceManager
```

Defined in src/av/include/icy/av/devicemanager.h:210

Enumerates and manages system audio and video devices.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `NullSignal` | [`DevicesChanged`](#deviceschanged)  | Signals when a system device is connected or removed. Emitted by the platform-specific `[DeviceWatcher](#devicewatcher)`. |

---

{#deviceschanged}

#### DevicesChanged

```cpp
NullSignal DevicesChanged
```

Defined in src/av/include/icy/av/devicemanager.h:297

Signals when a system device is connected or removed. Emitted by the platform-specific `[DeviceWatcher](#devicewatcher)`.

NOTE: This signal may be emitted from a background thread (e.g. inotify thread on Linux, COM callback on Windows, CoreAudio listener on macOS). Handlers must be thread-safe and should NOT call back into [DeviceManager](#devicemanager) to enumerate. Instead, post to the main thread or set a flag.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`DeviceManager`](#devicemanager-1)  |  |
| `bool` | [`getCameras`](#getcameras) `const` | Populate `devices` with all connected video input (camera) devices. |
| `bool` | [`getMicrophones`](#getmicrophones) `const` | Populate `devices` with all connected audio input (microphone) devices. |
| `bool` | [`getSpeakers`](#getspeakers) `const` | Populate `devices` with all connected audio output (speaker) devices. |
| `bool` | [`getDefaultCamera`](#getdefaultcamera) `const` | Return the default (or first available) camera. |
| `bool` | [`getDefaultMicrophone`](#getdefaultmicrophone) `const` | Return the default (or first available) microphone. |
| `bool` | [`getDefaultSpeaker`](#getdefaultspeaker) `const` | Return the default (or first available) speaker. |
| `bool` | [`findCamera`](#findcamera) `const` | Find a camera by display name or device id. |
| `bool` | [`findMicrophone`](#findmicrophone) `const` | Find a microphone by display name or device id. |
| `bool` | [`findSpeaker`](#findspeaker) `const` | Find a speaker by display name or device id. |
| `bool` | [`getDeviceList`](#getdevicelist-3) `const` | Populate `devices` from the platform-specific backend for the given type. |
| `int` | [`getCapabilities`](#getcapabilities) `const` |  |
| `void` | [`setWatcher`](#setwatcher)  | Replace the active device watcher. Takes ownership. |
| `DeviceWatcher *` | [`watcher`](#watcher) `const` |  |
| `void` | [`print`](#print-5) `const` | Print all devices to the output stream. |
| `std::vector< HardwareCodec >` | [`getHardwareCodecs`](#gethardwarecodecs) `const` | Detect available hardware-accelerated codecs via FFmpeg. |
| `std::optional< std::pair< Device, Device::VideoCapability > >` | [`negotiateVideoCapture`](#negotiatevideocapture) `const` | Resolve the best video capture parameters for a device. |
| `std::optional< std::pair< Device, Device::AudioCapability > >` | [`negotiateAudioCapture`](#negotiateaudiocapture) `const` | Resolve the best audio capture parameters for a device. Returns std::nullopt if no microphone is available. |
| `const AVInputFormat *` | [`findVideoInputFormat`](#findvideoinputformat) `const` | Find base FFmpeg formats. |
| `const AVInputFormat *` | [`findAudioInputFormat`](#findaudioinputformat) `const` |  |

---

{#devicemanager-1}

#### DeviceManager

```cpp
DeviceManager()
```

Defined in src/av/include/icy/av/devicemanager.h:213

---

{#getcameras}

#### getCameras

`const`

```cpp
bool getCameras(std::vector< Device > & devices) const
```

Defined in src/av/include/icy/av/devicemanager.h:219

Populate `devices` with all connected video input (camera) devices. 
#### Parameters
* `devices` Output vector to fill. 

#### Returns
True if enumeration succeeded (even if the list is empty).

---

{#getmicrophones}

#### getMicrophones

`const`

```cpp
bool getMicrophones(std::vector< Device > & devices) const
```

Defined in src/av/include/icy/av/devicemanager.h:224

Populate `devices` with all connected audio input (microphone) devices. 
#### Parameters
* `devices` Output vector to fill. 

#### Returns
True if enumeration succeeded.

---

{#getspeakers}

#### getSpeakers

`const`

```cpp
bool getSpeakers(std::vector< Device > & devices) const
```

Defined in src/av/include/icy/av/devicemanager.h:229

Populate `devices` with all connected audio output (speaker) devices. 
#### Parameters
* `devices` Output vector to fill. 

#### Returns
True if enumeration succeeded.

---

{#getdefaultcamera}

#### getDefaultCamera

`const`

```cpp
bool getDefaultCamera(Device & device) const
```

Defined in src/av/include/icy/av/devicemanager.h:234

Return the default (or first available) camera. 
#### Parameters
* `device` Output [Device](#device) to fill. 

#### Returns
True if a camera was found.

---

{#getdefaultmicrophone}

#### getDefaultMicrophone

`const`

```cpp
bool getDefaultMicrophone(Device & device) const
```

Defined in src/av/include/icy/av/devicemanager.h:239

Return the default (or first available) microphone. 
#### Parameters
* `device` Output [Device](#device) to fill. 

#### Returns
True if a microphone was found.

---

{#getdefaultspeaker}

#### getDefaultSpeaker

`const`

```cpp
bool getDefaultSpeaker(Device & device) const
```

Defined in src/av/include/icy/av/devicemanager.h:244

Return the default (or first available) speaker. 
#### Parameters
* `device` Output [Device](#device) to fill. 

#### Returns
True if a speaker was found.

---

{#findcamera}

#### findCamera

`const`

```cpp
bool findCamera(std::string_view name, Device & device) const
```

Defined in src/av/include/icy/av/devicemanager.h:250

Find a camera by display name or device id. 
#### Parameters
* `name` The name or id to search for. 

* `device` Output [Device](#device) to fill if found. 

#### Returns
True if a matching device was found.

---

{#findmicrophone}

#### findMicrophone

`const`

```cpp
bool findMicrophone(std::string_view name, Device & device) const
```

Defined in src/av/include/icy/av/devicemanager.h:256

Find a microphone by display name or device id. 
#### Parameters
* `name` The name or id to search for. 

* `device` Output [Device](#device) to fill if found. 

#### Returns
True if a matching device was found.

---

{#findspeaker}

#### findSpeaker

`const`

```cpp
bool findSpeaker(std::string_view name, Device & device) const
```

Defined in src/av/include/icy/av/devicemanager.h:262

Find a speaker by display name or device id. 
#### Parameters
* `name` The name or id to search for. 

* `device` Output [Device](#device) to fill if found. 

#### Returns
True if a matching device was found.

---

{#getdevicelist-3}

#### getDeviceList

`const`

```cpp
bool getDeviceList(Device::Type type, std::vector< av::Device > & devices) const
```

Defined in src/av/include/icy/av/devicemanager.h:268

Populate `devices` from the platform-specific backend for the given type. 
#### Parameters
* `type` The device type to enumerate. 

* `devices` Output vector to fill. 

#### Returns
True if enumeration succeeded.

---

{#getcapabilities}

#### getCapabilities

`const`

```cpp
int getCapabilities() const
```

Defined in src/av/include/icy/av/devicemanager.h:271

#### Returns
A bitmask of [MediaCapabilities](#mediacapabilities) flags indicating which capture/render types are available.

---

{#setwatcher}

#### setWatcher

```cpp
void setWatcher(DeviceWatcher * watcher)
```

Defined in src/av/include/icy/av/devicemanager.h:275

Replace the active device watcher. Takes ownership. 
#### Parameters
* `watcher` The new [DeviceWatcher](#devicewatcher), or nullptr to disable watching.

---

{#watcher}

#### watcher

`const`

```cpp
DeviceWatcher * watcher() const
```

Defined in src/av/include/icy/av/devicemanager.h:278

#### Returns
The currently active [DeviceWatcher](#devicewatcher), or nullptr if none has been set.

---

{#print-5}

#### print

`const`

```cpp
void print(std::ostream & ost) const
```

Defined in src/av/include/icy/av/devicemanager.h:281

Print all devices to the output stream.

---

{#gethardwarecodecs}

#### getHardwareCodecs

`const`

```cpp
std::vector< HardwareCodec > getHardwareCodecs() const
```

Defined in src/av/include/icy/av/devicemanager.h:307

Detect available hardware-accelerated codecs via FFmpeg.

---

{#negotiatevideocapture}

#### negotiateVideoCapture

`const`

```cpp
std::optional< std::pair< Device, Device::VideoCapability > > negotiateVideoCapture(std::string_view deviceName, int width, int height, double fps) const
```

Defined in src/av/include/icy/av/devicemanager.h:321

Resolve the best video capture parameters for a device.

Looks up the device by name/id (or default camera if empty), queries its capabilities, and returns the closest match. Returns std::nullopt if no camera is available.

Usage: if (auto result = devman.negotiateVideoCapture("", 1280, 720, 30)) { auto& [dev, cap] = *result; capture.openVideo(dev.id, cap.width, cap.height, cap.maxFps, cap.pixelFormat); }

---

{#negotiateaudiocapture}

#### negotiateAudioCapture

`const`

```cpp
std::optional< std::pair< Device, Device::AudioCapability > > negotiateAudioCapture(std::string_view deviceName, int sampleRate, int channels) const
```

Defined in src/av/include/icy/av/devicemanager.h:326

Resolve the best audio capture parameters for a device. Returns std::nullopt if no microphone is available.

---

{#findvideoinputformat}

#### findVideoInputFormat

`const`

```cpp
const AVInputFormat * findVideoInputFormat() const
```

Defined in src/av/include/icy/av/devicemanager.h:330

Find base FFmpeg formats.

---

{#findaudioinputformat}

#### findAudioInputFormat

`const`

```cpp
const AVInputFormat * findAudioInputFormat() const
```

Defined in src/av/include/icy/av/devicemanager.h:331

### Public Static Methods

| Return | Name | Description |
|--------|------|-------------|
| `DeviceManager &` | [`instance`](#instance) `static` | Returns the default `[DeviceManager](#devicemanager)` singleton. |
| `void` | [`shutdown`](#shutdown) `static` | Shuts down the `MediaFactory` and deletes the singleton instance. |

---

{#instance}

#### instance

`static`

```cpp
static DeviceManager & instance()
```

Defined in src/av/include/icy/av/devicemanager.h:284

Returns the default `[DeviceManager](#devicemanager)` singleton.

---

{#shutdown}

#### shutdown

`static`

```cpp
static void shutdown()
```

Defined in src/av/include/icy/av/devicemanager.h:287

Shuts down the `MediaFactory` and deletes the singleton instance.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::unique_ptr< DeviceWatcher >` | [`_watcher`](#_watcher)  |  |

---

{#_watcher}

#### _watcher

```cpp
std::unique_ptr< DeviceWatcher > _watcher
```

Defined in src/av/include/icy/av/devicemanager.h:335

{#hardwarecodec}

## HardwareCodec

```cpp
#include <icy/av/devicemanager.h>
```

```cpp
struct HardwareCodec
```

Defined in src/av/include/icy/av/devicemanager.h:300

Hardware codec descriptor.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`name`](#name-3)  |  |
| `std::string` | [`type`](#type-6)  |  |

---

{#name-3}

#### name

```cpp
std::string name
```

Defined in src/av/include/icy/av/devicemanager.h:302

---

{#type-6}

#### type

```cpp
std::string type
```

Defined in src/av/include/icy/av/devicemanager.h:303

{#formatregistry}

## FormatRegistry

```cpp
#include <icy/av/formatregistry.h>
```

```cpp
class FormatRegistry
```

Defined in src/av/include/icy/av/formatregistry.h:26

[Singleton](base.md#singleton) registry of available media container formats for encoding and decoding.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`FormatRegistry`](#formatregistry-1)  |  |
| `Format &` | [`get`](#get) `virtual` | Return the format with the given display name. Throws std::runtime_error if no format with that name is registered. |
| `Format &` | [`getByID`](#getbyid) `virtual` | Return the format with the given short ID (e.g. "mp4"). Throws std::runtime_error if no format with that ID is registered. |
| `Format &` | [`getOrDefault`](#getordefault) `virtual` | Return the format with the given name, or the default format if not found. |
| `Format &` | [`getDefault`](#getdefault) `virtual` | If a default has been specified it will be returned, other the format with the highest priority will take precedence. |
| `void` | [`registerFormat`](#registerformat) `virtual` | Registers the given media format overriding existing media formats of the same name. |
| `bool` | [`unregisterFormat`](#unregisterformat) `virtual` | Unregisters the media format matching the given name. |
| `void` | [`setDefault`](#setdefault) `virtual` | Sets the default fallback media format. |
| `bool` | [`exists`](#exists) `virtual` |  |
| `void` | [`clear`](#clear) `virtual` | Remove all registered formats and clear the default. |
| `FormatList` | [`formats`](#formats) `virtual` `const` |  |

---

{#formatregistry-1}

#### FormatRegistry

```cpp
FormatRegistry()
```

Defined in src/av/include/icy/av/formatregistry.h:32

---

{#get}

#### get

`virtual`

```cpp
virtual Format & get(std::string_view name)
```

Defined in src/av/include/icy/av/formatregistry.h:38

Return the format with the given display name. Throws std::runtime_error if no format with that name is registered. 
#### Parameters
* `name` The display name to look up.

---

{#getbyid}

#### getByID

`virtual`

```cpp
virtual Format & getByID(std::string_view id)
```

Defined in src/av/include/icy/av/formatregistry.h:43

Return the format with the given short ID (e.g. "mp4"). Throws std::runtime_error if no format with that ID is registered. 
#### Parameters
* `id` The short format ID to look up.

---

{#getordefault}

#### getOrDefault

`virtual`

```cpp
virtual Format & getOrDefault(std::string_view name)
```

Defined in src/av/include/icy/av/formatregistry.h:47

Return the format with the given name, or the default format if not found. 
#### Parameters
* `name` The display name to look up.

---

{#getdefault}

#### getDefault

`virtual`

```cpp
virtual Format & getDefault()
```

Defined in src/av/include/icy/av/formatregistry.h:52

If a default has been specified it will be returned, other the format with the highest priority will take precedence.

---

{#registerformat}

#### registerFormat

`virtual`

```cpp
virtual void registerFormat(const Format & format)
```

Defined in src/av/include/icy/av/formatregistry.h:56

Registers the given media format overriding existing media formats of the same name.

---

{#unregisterformat}

#### unregisterFormat

`virtual`

```cpp
virtual bool unregisterFormat(std::string_view name)
```

Defined in src/av/include/icy/av/formatregistry.h:60

Unregisters the media format matching the given name.

---

{#setdefault}

#### setDefault

`virtual`

```cpp
virtual void setDefault(std::string_view name)
```

Defined in src/av/include/icy/av/formatregistry.h:63

Sets the default fallback media format.

---

{#exists}

#### exists

`virtual`

```cpp
virtual bool exists(std::string_view name)
```

Defined in src/av/include/icy/av/formatregistry.h:67

#### Returns
True if a format with the given display name is registered. 

#### Parameters
* `name` The display name to check.

---

{#clear}

#### clear

`virtual`

```cpp
virtual void clear()
```

Defined in src/av/include/icy/av/formatregistry.h:70

Remove all registered formats and clear the default.

---

{#formats}

#### formats

`virtual` `const`

```cpp
virtual FormatList formats() const
```

Defined in src/av/include/icy/av/formatregistry.h:73

#### Returns
A snapshot copy of all registered formats.

### Public Static Methods

| Return | Name | Description |
|--------|------|-------------|
| `FormatRegistry &` | [`instance`](#instance-1) `static` | Return the singleton [FormatRegistry](#formatregistry) instance. |

---

{#instance-1}

#### instance

`static`

```cpp
static FormatRegistry & instance()
```

Defined in src/av/include/icy/av/formatregistry.h:30

Return the singleton [FormatRegistry](#formatregistry) instance.

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `FormatList` | [`_formats`](#_formats)  |  |
| `std::string` | [`_default`](#_default)  |  |
| `std::mutex` | [`_mutex`](#_mutex-1)  |  |

---

{#_formats}

#### _formats

```cpp
FormatList _formats
```

Defined in src/av/include/icy/av/formatregistry.h:84

---

{#_default}

#### _default

```cpp
std::string _default
```

Defined in src/av/include/icy/av/formatregistry.h:85

---

{#_mutex-1}

#### _mutex

```cpp
std::mutex _mutex
```

Defined in src/av/include/icy/av/formatregistry.h:86

### Private Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`FormatRegistry`](#formatregistry-2)  | Deleted constructor. |
|  | [`FormatRegistry`](#formatregistry-3)  | Deleted constructor. |
| `Format &` | [`findByName`](#findbyname)  |  |
| `Format &` | [`defaultLocked`](#defaultlocked)  |  |

---

{#formatregistry-2}

#### FormatRegistry

```cpp
FormatRegistry(const FormatRegistry &) = delete
```

Defined in src/av/include/icy/av/formatregistry.h:76

Deleted constructor.

---

{#formatregistry-3}

#### FormatRegistry

```cpp
FormatRegistry(FormatRegistry &&) = delete
```

Defined in src/av/include/icy/av/formatregistry.h:78

Deleted constructor.

---

{#findbyname}

#### findByName

```cpp
Format & findByName(std::string_view name)
```

Defined in src/av/include/icy/av/formatregistry.h:81

---

{#defaultlocked}

#### defaultLocked

```cpp
Format & defaultLocked()
```

Defined in src/av/include/icy/av/formatregistry.h:82

{#windowsdevicewatcher}

## WindowsDeviceWatcher

```cpp
#include <icy/av/win32/mediafoundation.h>
```

```cpp
class WindowsDeviceWatcher
```

Defined in src/av/include/icy/av/win32/mediafoundation.h:61

> **Inherits:** [`DeviceWatcher`](#devicewatcher)

Monitors device add/remove events via IMMNotificationClient (audio) and RegisterDeviceNotification (video).

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`WindowsDeviceWatcher`](#windowsdevicewatcher-1) `explicit` |  |
| `bool` | [`start`](#start-6) `virtual` `override` | Begin monitoring via IMMNotificationClient and RegisterDeviceNotification. |
| `void` | [`stop`](#stop-5) `virtual` `override` | Stop monitoring and unregister all device notifications. |

---

{#windowsdevicewatcher-1}

#### WindowsDeviceWatcher

`explicit`

```cpp
explicit WindowsDeviceWatcher(DeviceManager * manager)
```

Defined in src/av/include/icy/av/win32/mediafoundation.h:65

#### Parameters
* `manager` The [DeviceManager](#devicemanager) whose DevicesChanged signal will be emitted on changes.

---

{#start-6}

#### start

`virtual` `override`

```cpp
virtual bool start() override
```

Defined in src/av/include/icy/av/win32/mediafoundation.h:70

Begin monitoring via IMMNotificationClient and RegisterDeviceNotification. 
#### Returns
True if monitoring was successfully started.

---

{#stop-5}

#### stop

`virtual` `override`

```cpp
virtual void stop() override
```

Defined in src/av/include/icy/av/win32/mediafoundation.h:73

Stop monitoring and unregister all device notifications.

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::unique_ptr< Impl >` | [`_impl`](#_impl-2)  |  |
| `DeviceManager *` | [`_manager`](#_manager-2)  |  |

---

{#_impl-2}

#### _impl

```cpp
std::unique_ptr< Impl > _impl
```

Defined in src/av/include/icy/av/win32/mediafoundation.h:78

---

{#_manager-2}

#### _manager

```cpp
DeviceManager * _manager
```

Defined in src/av/include/icy/av/win32/mediafoundation.h:79

{#multiplexencoder}

## MultiplexEncoder

```cpp
#include <icy/av/multiplexencoder.h>
```

```cpp
class MultiplexEncoder
```

Defined in src/av/include/icy/av/multiplexencoder.h:41

> **Inherits:** [`IEncoder`](#iencoder)
> **Subclassed by:** [`MultiplexPacketEncoder`](#multiplexpacketencoder)

Multiplexing encoder that writes synchronized audio and video streams.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `PacketSignal` | [`emitter`](#emitter-4)  |  |

---

{#emitter-4}

#### emitter

```cpp
PacketSignal emitter
```

Defined in src/av/include/icy/av/multiplexencoder.h:130

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`MultiplexEncoder`](#multiplexencoder-1)  | Construct the encoder with the given options. |
|  | [`MultiplexEncoder`](#multiplexencoder-2)  | Deleted constructor. |
|  | [`MultiplexEncoder`](#multiplexencoder-3)  | Deleted constructor. |
| `void` | [`init`](#init-1) `virtual` `override` | Open the output container, create codec streams, and write the format header. |
| `void` | [`uninit`](#uninit-1) `virtual` `override` | Flush encoded packets, write the format trailer, and close the output container. |
| `void` | [`cleanup`](#cleanup-1) `virtual` `override` | Release all resources allocated by [init()](#init-1) without writing a trailer. |
| `void` | [`createVideo`](#createvideo-1) `virtual` `override` | Create the video encoder and add the stream to the output container. |
| `void` | [`freeVideo`](#freevideo-1) `virtual` `override` | Flush and free the video encoder and its stream. |
| `bool` | [`encodeVideo`](#encodevideo-1) `virtual` `nodiscard` `override` | Encode a single video frame. All frame values must be set, such as size, pixel format, and PTS. |
| `bool` | [`encodeVideo`](#encodevideo-2) `virtual` `nodiscard` | Encode a single interleaved video frame. If the frame time is specified it should be the microseconds offset since the start of the input stream. If no time is specified a real-time value will be assigned to the frame. |
| `bool` | [`encodeVideo`](#encodevideo-3) `virtual` `nodiscard` | Encode a single planar video frame. |
| `void` | [`createAudio`](#createaudio-1) `virtual` `override` | Create the audio encoder and add the stream to the output container. |
| `void` | [`freeAudio`](#freeaudio-1) `virtual` `override` | Flush and free the audio encoder and its stream. |
| `bool` | [`encodeAudio`](#encodeaudio) `virtual` `nodiscard` | Encode a single interleaved audio frame. |
| `bool` | [`encodeAudio`](#encodeaudio-1) `virtual` `nodiscard` | Encode a single planar audio frame. |
| `void` | [`flush`](#flush-7) `virtual` `override` | Flush any buffered or queued packets to the output container. |
| `const EncoderOptions &` | [`options`](#options-2) `virtual` `const` `nodiscard` `override` |  |
| `VideoEncoder *` | [`video`](#video-2)  |  |
| `AudioEncoder *` | [`audio`](#audio-2)  |  |

---

{#multiplexencoder-1}

#### MultiplexEncoder

```cpp
MultiplexEncoder(const EncoderOptions & options = EncoderOptions())
```

Defined in src/av/include/icy/av/multiplexencoder.h:46

Construct the encoder with the given options. 
#### Parameters
* `options` The encoder configuration (input/output formats and file paths).

---

{#multiplexencoder-2}

#### MultiplexEncoder

```cpp
MultiplexEncoder(const MultiplexEncoder &) = delete
```

Defined in src/av/include/icy/av/multiplexencoder.h:49

Deleted constructor.

---

{#multiplexencoder-3}

#### MultiplexEncoder

```cpp
MultiplexEncoder(MultiplexEncoder &&) = delete
```

Defined in src/av/include/icy/av/multiplexencoder.h:51

Deleted constructor.

---

{#init-1}

#### init

`virtual` `override`

```cpp
virtual void init() override
```

Defined in src/av/include/icy/av/multiplexencoder.h:55

Open the output container, create codec streams, and write the format header.

---

{#uninit-1}

#### uninit

`virtual` `override`

```cpp
virtual void uninit() override
```

Defined in src/av/include/icy/av/multiplexencoder.h:58

Flush encoded packets, write the format trailer, and close the output container.

---

{#cleanup-1}

#### cleanup

`virtual` `override`

```cpp
virtual void cleanup() override
```

Defined in src/av/include/icy/av/multiplexencoder.h:61

Release all resources allocated by [init()](#init-1) without writing a trailer.

---

{#createvideo-1}

#### createVideo

`virtual` `override`

```cpp
virtual void createVideo() override
```

Defined in src/av/include/icy/av/multiplexencoder.h:64

Create the video encoder and add the stream to the output container.

---

{#freevideo-1}

#### freeVideo

`virtual` `override`

```cpp
virtual void freeVideo() override
```

Defined in src/av/include/icy/av/multiplexencoder.h:67

Flush and free the video encoder and its stream.

---

{#encodevideo-1}

#### encodeVideo

`virtual` `nodiscard` `override`

```cpp
[[nodiscard]] virtual bool encodeVideo(AVFrame * frame) override
```

Defined in src/av/include/icy/av/multiplexencoder.h:71

Encode a single video frame. All frame values must be set, such as size, pixel format, and PTS.

---

{#encodevideo-2}

#### encodeVideo

`virtual` `nodiscard`

```cpp
[[nodiscard]] virtual bool encodeVideo(uint8_t * buffer, int bufferSize, int width, int height, int64_t time = AV_NOPTS_VALUE)
```

Defined in src/av/include/icy/av/multiplexencoder.h:83

Encode a single interleaved video frame. If the frame time is specified it should be the microseconds offset since the start of the input stream. If no time is specified a real-time value will be assigned to the frame.

#### Parameters
* `buffer` The raw video frame buffer. 

* `bufferSize` The buffer size in bytes. 

* `width` The frame width in pixels. 

* `height` The frame height in pixels. 

* `time` The timestamp in microseconds, or AV_NOPTS_VALUE for realtime.

---

{#encodevideo-3}

#### encodeVideo

`virtual` `nodiscard`

```cpp
[[nodiscard]] virtual bool encodeVideo(uint8_t * data, int linesize, int width, int height, int64_t time = AV_NOPTS_VALUE)
```

Defined in src/av/include/icy/av/multiplexencoder.h:93

Encode a single planar video frame.

#### Parameters
* `data` Array of per-plane data pointers (up to 4 planes). 

* `linesize` Array of per-plane byte strides. 

* `width` The frame width in pixels. 

* `height` The frame height in pixels. 

* `time` The timestamp in microseconds, or AV_NOPTS_VALUE for realtime.

---

{#createaudio-1}

#### createAudio

`virtual` `override`

```cpp
virtual void createAudio() override
```

Defined in src/av/include/icy/av/multiplexencoder.h:97

Create the audio encoder and add the stream to the output container.

---

{#freeaudio-1}

#### freeAudio

`virtual` `override`

```cpp
virtual void freeAudio() override
```

Defined in src/av/include/icy/av/multiplexencoder.h:100

Flush and free the audio encoder and its stream.

---

{#encodeaudio}

#### encodeAudio

`virtual` `nodiscard`

```cpp
[[nodiscard]] virtual bool encodeAudio(uint8_t * buffer, int numSamples, int64_t time = AV_NOPTS_VALUE)
```

Defined in src/av/include/icy/av/multiplexencoder.h:107

Encode a single interleaved audio frame.

#### Parameters
* `buffer` The interleaved audio sample buffer. 

* `numSamples` The number of samples per channel. 

* `time` The timestamp in microseconds, or AV_NOPTS_VALUE for realtime.

---

{#encodeaudio-1}

#### encodeAudio

`virtual` `nodiscard`

```cpp
[[nodiscard]] virtual bool encodeAudio(uint8_t * data, int numSamples, int64_t time = AV_NOPTS_VALUE)
```

Defined in src/av/include/icy/av/multiplexencoder.h:115

Encode a single planar audio frame.

#### Parameters
* `data` Array of per-plane sample buffers (one per channel). 

* `numSamples` The number of samples per channel. 

* `time` The timestamp in microseconds, or AV_NOPTS_VALUE for realtime.

---

{#flush-7}

#### flush

`virtual` `override`

```cpp
virtual void flush() override
```

Defined in src/av/include/icy/av/multiplexencoder.h:119

Flush any buffered or queued packets to the output container.

---

{#options-2}

#### options

`virtual` `const` `nodiscard` `override`

```cpp
[[nodiscard]] virtual const EncoderOptions & options() const override
```

Defined in src/av/include/icy/av/multiplexencoder.h:122

#### Returns
A read-only view of the encoder's configuration options.

---

{#video-2}

#### video

```cpp
VideoEncoder * video()
```

Defined in src/av/include/icy/av/multiplexencoder.h:125

#### Returns
The active [VideoEncoder](#videoencoder), or nullptr if video has not been created.

---

{#audio-2}

#### audio

```cpp
AudioEncoder * audio()
```

Defined in src/av/include/icy/av/multiplexencoder.h:128

#### Returns
The active [AudioEncoder](#audioencoder), or nullptr if audio has not been created.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `EncoderOptions` | [`_options`](#_options)  |  |
| `AVFormatContext *` | [`_formatCtx`](#_formatctx-1)  |  |
| `std::unique_ptr< VideoEncoder >` | [`_video`](#_video-1)  |  |
| `std::unique_ptr< AudioEncoder >` | [`_audio`](#_audio-1)  |  |
| `AVIOContext *` | [`_ioCtx`](#_ioctx)  |  |
| `std::unique_ptr< uint8_t[]>` | [`_ioBuffer`](#_iobuffer)  |  |
| `int64_t` | [`_pts`](#_pts)  |  |
| `std::mutex` | [`_mutex`](#_mutex-2)  |  |

---

{#_options}

#### _options

```cpp
EncoderOptions _options
```

Defined in src/av/include/icy/av/multiplexencoder.h:144

---

{#_formatctx-1}

#### _formatCtx

```cpp
AVFormatContext * _formatCtx
```

Defined in src/av/include/icy/av/multiplexencoder.h:145

---

{#_video-1}

#### _video

```cpp
std::unique_ptr< VideoEncoder > _video
```

Defined in src/av/include/icy/av/multiplexencoder.h:146

---

{#_audio-1}

#### _audio

```cpp
std::unique_ptr< AudioEncoder > _audio
```

Defined in src/av/include/icy/av/multiplexencoder.h:147

---

{#_ioctx}

#### _ioCtx

```cpp
AVIOContext * _ioCtx
```

Defined in src/av/include/icy/av/multiplexencoder.h:148

---

{#_iobuffer}

#### _ioBuffer

```cpp
std::unique_ptr< uint8_t[]> _ioBuffer
```

Defined in src/av/include/icy/av/multiplexencoder.h:149

---

{#_pts}

#### _pts

```cpp
int64_t _pts
```

Defined in src/av/include/icy/av/multiplexencoder.h:150

---

{#_mutex-2}

#### _mutex

```cpp
std::mutex _mutex
```

Defined in src/av/include/icy/av/multiplexencoder.h:151

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `bool` | [`writeOutputPacket`](#writeoutputpacket)  |  |
| `bool` | [`updateStreamPts`](#updatestreampts)  | Convert input microseconds to the stream time base. |
| `void` | [`onVideoEncoded`](#onvideoencoded)  |  |
| `void` | [`onAudioEncoded`](#onaudioencoded)  |  |

---

{#writeoutputpacket}

#### writeOutputPacket

```cpp
bool writeOutputPacket(AVPacket & packet)
```

Defined in src/av/include/icy/av/multiplexencoder.h:133

---

{#updatestreampts}

#### updateStreamPts

```cpp
bool updateStreamPts(AVStream * stream, int64_t * pts)
```

Defined in src/av/include/icy/av/multiplexencoder.h:139

Convert input microseconds to the stream time base.

#### Parameters
* `stream` The target stream for time base conversion. 

* `pts` Pointer to the timestamp; converted in place.

---

{#onvideoencoded}

#### onVideoEncoded

```cpp
void onVideoEncoded(av::VideoPacket & packet)
```

Defined in src/av/include/icy/av/multiplexencoder.h:141

---

{#onaudioencoded}

#### onAudioEncoded

```cpp
void onAudioEncoded(av::AudioPacket & packet)
```

Defined in src/av/include/icy/av/multiplexencoder.h:142

{#audiopacketencoder}

## AudioPacketEncoder

```cpp
#include <icy/av/audiopacketencoder.h>
```

```cpp
class AudioPacketEncoder
```

Defined in src/av/include/icy/av/audiopacketencoder.h:44

> **Inherits:** [`AudioEncoder`](#audioencoder), [`PacketProcessor`](base.md#packetprocessor)

[PacketProcessor](base.md#packetprocessor) that encodes raw audio samples ([PlanarAudioPacket](#planaraudiopacket) or [AudioPacket](#audiopacket)) into compressed packets via [AudioEncoder](#audioencoder).

Drop this into a [PacketStream](base.md#packetstream) between a source that emits raw audio (e.g. [MediaCapture](#mediacapture)) and a sink that expects encoded data (e.g. WebRtcTrackSender or [MultiplexPacketEncoder](#multiplexpacketencoder)).

Example:

auto encoder = std::make_shared<av::AudioPacketEncoder>(); encoder->oparams = [av::AudioCodec](#audiocodec)("opus", "libopus", 2, 48000);

[PacketStream](base.md#packetstream) stream; stream.attachSource(capture); stream.attach(encoder, 1, true); stream.attach(&webrtcSender, 5, false); stream.start();

### Friends

| Name | Description |
|------|-------------|
| [`PacketStream`](#packetstream-1)  |  |

---

{#packetstream-1}

#### PacketStream

```cpp
friend class PacketStream
```

Defined in src/av/include/icy/av/audiopacketencoder.h:64

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`AudioPacketEncoder`](#audiopacketencoder-1)  |  |
|  | [`AudioPacketEncoder`](#audiopacketencoder-2)  | Deleted constructor. |
| `void` | [`process`](#process-2) `virtual` `override` | [Process](base.md#process) an [AudioPacket](#audiopacket) or [PlanarAudioPacket](#planaraudiopacket) from the stream. Encodes the samples and emits the resulting compressed packet. |
| `bool` | [`accepts`](#accepts) `virtual` `override` | Accept [AudioPacket](#audiopacket) and [PlanarAudioPacket](#planaraudiopacket) types. |

---

{#audiopacketencoder-1}

#### AudioPacketEncoder

```cpp
AudioPacketEncoder(AVFormatContext * format = nullptr)
```

Defined in src/av/include/icy/av/audiopacketencoder.h:48

---

{#audiopacketencoder-2}

#### AudioPacketEncoder

```cpp
AudioPacketEncoder(const AudioPacketEncoder &) = delete
```

Defined in src/av/include/icy/av/audiopacketencoder.h:51

Deleted constructor.

---

{#process-2}

#### process

`virtual` `override`

```cpp
virtual void process(IPacket & packet) override
```

Defined in src/av/include/icy/av/audiopacketencoder.h:56

[Process](base.md#process) an [AudioPacket](#audiopacket) or [PlanarAudioPacket](#planaraudiopacket) from the stream. Encodes the samples and emits the resulting compressed packet.

---

{#accepts}

#### accepts

`virtual` `override`

```cpp
virtual bool accepts(IPacket * packet) override
```

Defined in src/av/include/icy/av/audiopacketencoder.h:59

Accept [AudioPacket](#audiopacket) and [PlanarAudioPacket](#planaraudiopacket) types.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `bool` | [`_initialized`](#_initialized)  |  |
| `std::mutex` | [`_mutex`](#_mutex-3)  |  |

---

{#_initialized}

#### _initialized

```cpp
bool _initialized = false
```

Defined in src/av/include/icy/av/audiopacketencoder.h:66

---

{#_mutex-3}

#### _mutex

```cpp
std::mutex _mutex
```

Defined in src/av/include/icy/av/audiopacketencoder.h:67

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`onStreamStateChange`](#onstreamstatechange-2) `virtual` `override` | Called by the [PacketStream](base.md#packetstream) to notify when the internal [Stream](base.md#stream) state changes. On receiving the Stopped state, it is the responsibility of the adapter to have ceased all outgoing packet transmission, especially in multi-thread scenarios. |

---

{#onstreamstatechange-2}

#### onStreamStateChange

`virtual` `override`

```cpp
virtual void onStreamStateChange(const PacketStreamState & state) override
```

Defined in src/av/include/icy/av/audiopacketencoder.h:62

Called by the [PacketStream](base.md#packetstream) to notify when the internal [Stream](base.md#stream) state changes. On receiving the Stopped state, it is the responsibility of the adapter to have ceased all outgoing packet transmission, especially in multi-thread scenarios.

{#videopacketencoder}

## VideoPacketEncoder

```cpp
#include <icy/av/videopacketencoder.h>
```

```cpp
class VideoPacketEncoder
```

Defined in src/av/include/icy/av/videopacketencoder.h:45

> **Inherits:** [`VideoEncoder`](#videoencoder), [`PacketProcessor`](base.md#packetprocessor)

[PacketProcessor](base.md#packetprocessor) that encodes raw video frames ([PlanarVideoPacket](#planarvideopacket) or [VideoPacket](#videopacket)) into compressed packets via [VideoEncoder](#videoencoder).

Drop this into a [PacketStream](base.md#packetstream) between a source that emits raw frames (e.g. [MediaCapture](#mediacapture)) and a sink that expects encoded data (e.g. WebRtcTrackSender or [MultiplexPacketEncoder](#multiplexpacketencoder)).

Example:

auto encoder = std::make_shared<av::VideoPacketEncoder>(); encoder->iparams = capture->videoCodec(); // decoded format encoder->oparams = [av::VideoCodec](#videocodec)("H264", "libx264", 640, 480, 30);

[PacketStream](base.md#packetstream) stream; stream.attachSource(capture); stream.attach(encoder, 1, true); stream.attach(&webrtcSender, 5, false); stream.start();

### Friends

| Name | Description |
|------|-------------|
| [`PacketStream`](#packetstream-2)  |  |

---

{#packetstream-2}

#### PacketStream

```cpp
friend class PacketStream
```

Defined in src/av/include/icy/av/videopacketencoder.h:65

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`VideoPacketEncoder`](#videopacketencoder-1)  |  |
|  | [`VideoPacketEncoder`](#videopacketencoder-2)  | Deleted constructor. |
| `void` | [`process`](#process-3) `virtual` `override` | [Process](base.md#process) a [VideoPacket](#videopacket) or [PlanarVideoPacket](#planarvideopacket) from the stream. Encodes the frame and emits the resulting compressed packet. |
| `bool` | [`accepts`](#accepts-1) `virtual` `override` | Accept [VideoPacket](#videopacket) and [PlanarVideoPacket](#planarvideopacket) types. |

---

{#videopacketencoder-1}

#### VideoPacketEncoder

```cpp
VideoPacketEncoder(AVFormatContext * format = nullptr)
```

Defined in src/av/include/icy/av/videopacketencoder.h:49

---

{#videopacketencoder-2}

#### VideoPacketEncoder

```cpp
VideoPacketEncoder(const VideoPacketEncoder &) = delete
```

Defined in src/av/include/icy/av/videopacketencoder.h:52

Deleted constructor.

---

{#process-3}

#### process

`virtual` `override`

```cpp
virtual void process(IPacket & packet) override
```

Defined in src/av/include/icy/av/videopacketencoder.h:57

[Process](base.md#process) a [VideoPacket](#videopacket) or [PlanarVideoPacket](#planarvideopacket) from the stream. Encodes the frame and emits the resulting compressed packet.

---

{#accepts-1}

#### accepts

`virtual` `override`

```cpp
virtual bool accepts(IPacket * packet) override
```

Defined in src/av/include/icy/av/videopacketencoder.h:60

Accept [VideoPacket](#videopacket) and [PlanarVideoPacket](#planarvideopacket) types.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `bool` | [`_initialized`](#_initialized-1)  |  |
| `std::mutex` | [`_mutex`](#_mutex-4)  |  |

---

{#_initialized-1}

#### _initialized

```cpp
bool _initialized = false
```

Defined in src/av/include/icy/av/videopacketencoder.h:67

---

{#_mutex-4}

#### _mutex

```cpp
std::mutex _mutex
```

Defined in src/av/include/icy/av/videopacketencoder.h:68

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`onStreamStateChange`](#onstreamstatechange-3) `virtual` `override` | Called by the [PacketStream](base.md#packetstream) to notify when the internal [Stream](base.md#stream) state changes. On receiving the Stopped state, it is the responsibility of the adapter to have ceased all outgoing packet transmission, especially in multi-thread scenarios. |

---

{#onstreamstatechange-3}

#### onStreamStateChange

`virtual` `override`

```cpp
virtual void onStreamStateChange(const PacketStreamState & state) override
```

Defined in src/av/include/icy/av/videopacketencoder.h:63

Called by the [PacketStream](base.md#packetstream) to notify when the internal [Stream](base.md#stream) state changes. On receiving the Stopped state, it is the responsibility of the adapter to have ceased all outgoing packet transmission, especially in multi-thread scenarios.

{#realtimepacketqueue}

## RealtimePacketQueue

```cpp
#include <icy/av/realtimepacketqueue.h>
```

```cpp
template<class PacketT>
class RealtimePacketQueue
```

Defined in src/av/include/icy/av/realtimepacketqueue.h:30

> **Inherits:** [`AsyncPacketQueue< PacketT >`](base.md#asyncpacketqueue)

[Queue](base.md#queue) that emits media packets in presentation-timestamp order relative to a realtime clock.

Packets are sorted by their `[time](base.md#time-3)` field on insertion. On each pop attempt the queue checks whether the wall-clock time since stream activation has reached the next packet's timestamp; if not, the packet is held back. This provides a soft realtime playback scheduler.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`RealtimePacketQueue`](#realtimepacketqueue-1) `inline` | Construct the queue with a maximum capacity. |
| `void` | [`push`](#push) `virtual` `inline` `override` | Insert a packet into the queue and re-sort by presentation timestamp. |
| `int64_t` | [`realTime`](#realtime) `inline` | Return the elapsed time since stream activation in microseconds. |

---

{#realtimepacketqueue-1}

#### RealtimePacketQueue

`inline`

```cpp
inline RealtimePacketQueue(int maxSize = 1024)
```

Defined in src/av/include/icy/av/realtimepacketqueue.h:37

Construct the queue with a maximum capacity. 
#### Parameters
* `maxSize` The maximum number of packets the queue will hold.

---

{#push}

#### push

`virtual` `inline` `override`

```cpp
virtual inline void push(PacketT * item) override
```

Defined in src/av/include/icy/av/realtimepacketqueue.h:48

Insert a packet into the queue and re-sort by presentation timestamp. 
#### Parameters
* `item` The packet to enqueue; ownership is transferred.

---

{#realtime}

#### realTime

`inline`

```cpp
inline int64_t realTime()
```

Defined in src/av/include/icy/av/realtimepacketqueue.h:55

Return the elapsed time since stream activation in microseconds.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `int64_t` | [`_startTime`](#_starttime)  |  |

---

{#_starttime}

#### _startTime

```cpp
int64_t _startTime
```

Defined in src/av/include/icy/av/realtimepacketqueue.h:98

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `PacketT *` | [`popNext`](#popnext) `virtual` `inline` `override` | Return the next packet whose timestamp is <= [realTime()](#realtime), or nullptr if none is ready. |
| `void` | [`onStreamStateChange`](#onstreamstatechange-4) `virtual` `inline` `override` | Record the stream start time when the stream becomes active. |

---

{#popnext}

#### popNext

`virtual` `inline` `override`

```cpp
virtual inline PacketT * popNext() override
```

Defined in src/av/include/icy/av/realtimepacketqueue.h:62

Return the next packet whose timestamp is <= [realTime()](#realtime), or nullptr if none is ready.

---

{#onstreamstatechange-4}

#### onStreamStateChange

`virtual` `inline` `override`

```cpp
virtual inline void onStreamStateChange(const PacketStreamState & state) override
```

Defined in src/av/include/icy/av/realtimepacketqueue.h:78

Record the stream start time when the stream becomes active.

### Public Types

| Name | Description |
|------|-------------|
| [`BaseQueue`](#basequeue)  |  |

---

{#basequeue}

#### BaseQueue

```cpp
using BaseQueue = AsyncPacketQueue< PacketT >
```

Defined in src/av/include/icy/av/realtimepacketqueue.h:33

{#mediapackettimecompare}

## MediaPacketTimeCompare

```cpp
#include <icy/av/realtimepacketqueue.h>
```

```cpp
struct MediaPacketTimeCompare
```

Defined in src/av/include/icy/av/realtimepacketqueue.h:90

Comparator for ordering media packets by presentation timestamp.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
| `bool` | [`operator()`](#operator-3) `inline` |  |

---

{#operator-3}

#### operator()

`inline`

```cpp
inline bool operator()(const MediaPacket * a, const MediaPacket * b)
```

Defined in src/av/include/icy/av/realtimepacketqueue.h:92

{#multiplexpacketencoder}

## MultiplexPacketEncoder

```cpp
#include <icy/av/multiplexpacketencoder.h>
```

```cpp
class MultiplexPacketEncoder
```

Defined in src/av/include/icy/av/multiplexpacketencoder.h:28

> **Inherits:** [`MultiplexEncoder`](#multiplexencoder), [`PacketProcessor`](base.md#packetprocessor)

Encodes and multiplexes a realtime video stream form audio / video capture sources. FFmpeg is used for encoding.

### Friends

| Name | Description |
|------|-------------|
| [`PacketStream`](#packetstream-3)  |  |

---

{#packetstream-3}

#### PacketStream

```cpp
friend class PacketStream
```

Defined in src/av/include/icy/av/multiplexpacketencoder.h:61

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`MultiplexPacketEncoder`](#multiplexpacketencoder-1)  | Construct the encoder with the given options. |
|  | [`MultiplexPacketEncoder`](#multiplexpacketencoder-2)  | Deleted constructor. |
|  | [`MultiplexPacketEncoder`](#multiplexpacketencoder-3)  | Deleted constructor. |
| `void` | [`encode`](#encode-12) `virtual` | Encode a [VideoPacket](#videopacket), dispatching to the planar or interleaved encode path as appropriate. |
| `void` | [`encode`](#encode-13) `virtual` | Encode an [AudioPacket](#audiopacket), dispatching to the planar or interleaved encode path as appropriate. |
| `bool` | [`accepts`](#accepts-2) `virtual` `override` |  |
| `void` | [`process`](#process-4) `virtual` `override` | Dispatch the incoming packet to [encode(VideoPacket&)](#encode-12) or [encode(AudioPacket&)](#encode-13). Throws std::invalid_argument if the packet type is unrecognised. |

---

{#multiplexpacketencoder-1}

#### MultiplexPacketEncoder

```cpp
MultiplexPacketEncoder(const EncoderOptions & options = EncoderOptions())
```

Defined in src/av/include/icy/av/multiplexpacketencoder.h:34

Construct the encoder with the given options. 
#### Parameters
* `options` The encoder configuration (input/output formats and file paths).

---

{#multiplexpacketencoder-2}

#### MultiplexPacketEncoder

```cpp
MultiplexPacketEncoder(const MultiplexPacketEncoder &) = delete
```

Defined in src/av/include/icy/av/multiplexpacketencoder.h:37

Deleted constructor.

---

{#multiplexpacketencoder-3}

#### MultiplexPacketEncoder

```cpp
MultiplexPacketEncoder(MultiplexPacketEncoder &&) = delete
```

Defined in src/av/include/icy/av/multiplexpacketencoder.h:39

Deleted constructor.

---

{#encode-12}

#### encode

`virtual`

```cpp
virtual void encode(VideoPacket & packet)
```

Defined in src/av/include/icy/av/multiplexpacketencoder.h:44

Encode a [VideoPacket](#videopacket), dispatching to the planar or interleaved encode path as appropriate. 
#### Parameters
* `packet` The video packet to encode.

---

{#encode-13}

#### encode

`virtual`

```cpp
virtual void encode(AudioPacket & packet)
```

Defined in src/av/include/icy/av/multiplexpacketencoder.h:48

Encode an [AudioPacket](#audiopacket), dispatching to the planar or interleaved encode path as appropriate. 
#### Parameters
* `packet` The audio packet to encode.

---

{#accepts-2}

#### accepts

`virtual` `override`

```cpp
virtual bool accepts(IPacket * packet) override
```

Defined in src/av/include/icy/av/multiplexpacketencoder.h:51

#### Returns
True if the packet is an [av::MediaPacket](#mediapacket) (audio or video).

---

{#process-4}

#### process

`virtual` `override`

```cpp
virtual void process(IPacket & packet) override
```

Defined in src/av/include/icy/av/multiplexpacketencoder.h:56

Dispatch the incoming packet to [encode(VideoPacket&)](#encode-12) or [encode(AudioPacket&)](#encode-13). Throws std::invalid_argument if the packet type is unrecognised. 
#### Parameters
* `packet` The incoming media packet.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::mutex` | [`_mutex`](#_mutex-5)  |  |

---

{#_mutex-5}

#### _mutex

```cpp
std::mutex _mutex
```

Defined in src/av/include/icy/av/multiplexpacketencoder.h:63

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`onStreamStateChange`](#onstreamstatechange-5) `virtual` `override` | Called by the [PacketStream](base.md#packetstream) to notify when the internal [Stream](base.md#stream) state changes. On receiving the Stopped state, it is the responsibility of the adapter to have ceased all outgoing packet transmission, especially in multi-thread scenarios. |

---

{#onstreamstatechange-5}

#### onStreamStateChange

`virtual` `override`

```cpp
virtual void onStreamStateChange(const PacketStreamState & state) override
```

Defined in src/av/include/icy/av/multiplexpacketencoder.h:59

Called by the [PacketStream](base.md#packetstream) to notify when the internal [Stream](base.md#stream) state changes. On receiving the Stopped state, it is the responsibility of the adapter to have ceased all outgoing packet transmission, especially in multi-thread scenarios.

{#codec}

## Codec

```cpp
#include <icy/av/codec.h>
```

```cpp
struct Codec
```

Defined in src/av/include/icy/av/codec.h:29

> **Subclassed by:** [`AudioCodec`](#audiocodec), [`VideoCodec`](#videocodec)

[Codec](#codec) for encoding/decoding media.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`name`](#name)  | The display name for this codec. |
| `std::string` | [`encoder`](#encoder)  | The encoder name for FFmpeg. |
| `int` | [`sampleRate`](#samplerate)  | The sampling rate or RTP clock rate. |
| `int` | [`bitRate`](#bitrate)  | The bit rate to encode at. |
| `int` | [`quality`](#quality)  | Optional quality value, variable range depending on codec. |
| `int` | [`compliance`](#compliance)  | FFmpeg strict_std_compliance level (default: FF_COMPLIANCE_EXPERIMENTAL). |
| `bool` | [`enabled`](#enabled)  | Weather or not the codec is available for use. |
| `std::map< std::string, std::string >` | [`options`](#options)  | Arbitrary encoder options passed to FFmpeg via av_opt_set(). Keys are FFmpeg option names (e.g. "preset", "tune", "profile"). Applied after codec-specific defaults, so they override everything. |

---

{#name}

#### name

```cpp
std::string name
```

Defined in src/av/include/icy/av/codec.h:34

The display name for this codec.

---

{#encoder}

#### encoder

```cpp
std::string encoder
```

Defined in src/av/include/icy/av/codec.h:35

The encoder name for FFmpeg.

---

{#samplerate}

#### sampleRate

```cpp
int sampleRate
```

Defined in src/av/include/icy/av/codec.h:36

The sampling rate or RTP clock rate.

---

{#bitrate}

#### bitRate

```cpp
int bitRate
```

Defined in src/av/include/icy/av/codec.h:37

The bit rate to encode at.

---

{#quality}

#### quality

```cpp
int quality
```

Defined in src/av/include/icy/av/codec.h:38

Optional quality value, variable range depending on codec.

---

{#compliance}

#### compliance

```cpp
int compliance
```

Defined in src/av/include/icy/av/codec.h:39

FFmpeg strict_std_compliance level (default: FF_COMPLIANCE_EXPERIMENTAL).

---

{#enabled}

#### enabled

```cpp
bool enabled
```

Defined in src/av/include/icy/av/codec.h:40

Weather or not the codec is available for use.

---

{#options}

#### options

```cpp
std::map< std::string, std::string > options
```

Defined in src/av/include/icy/av/codec.h:45

Arbitrary encoder options passed to FFmpeg via av_opt_set(). Keys are FFmpeg option names (e.g. "preset", "tune", "profile"). Applied after codec-specific defaults, so they override everything.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Codec`](#codec-1)  | Construct a disabled codec with zeroed parameters. |
|  | [`Codec`](#codec-2)  | Construct a codec with a display name, sample rate, bit rate, and enabled flag. |
|  | [`Codec`](#codec-3)  | Construct a codec with an explicit FFmpeg encoder name. |
|  | [`~Codec`](#codec-4) `virtual` `noexcept` | Codec(const Codec& r);. |
| `std::string` | [`toString`](#tostring) `virtual` `const` | Return a compact string representation of this codec. |
| `bool` | [`specified`](#specified) `const` `nodiscard` | Returns true when this codec explicitly names either an RTP/media codec or an FFmpeg encoder and is enabled for use. |
| `void` | [`print`](#print) `virtual` | Print a multi-line human-readable description to the given stream. |

---

{#codec-1}

#### Codec

```cpp
Codec()
```

Defined in src/av/include/icy/av/codec.h:51

Construct a disabled codec with zeroed parameters.

---

{#codec-2}

#### Codec

```cpp
Codec(const std::string & name, int sampleRate = 0, int bitRate = 0, bool enabled = true)
```

Defined in src/av/include/icy/av/codec.h:58

Construct a codec with a display name, sample rate, bit rate, and enabled flag. 
#### Parameters
* `name` The codec display name. 

* `sampleRate` The sampling rate or RTP clock rate in Hz. 

* `bitRate` The target bit rate in bits per second. 

* `enabled` Whether the codec is available for use.

---

{#codec-3}

#### Codec

```cpp
Codec(const std::string & name, const std::string & encoder, int sampleRate = 0, int bitRate = 0, bool enabled = true)
```

Defined in src/av/include/icy/av/codec.h:67

Construct a codec with an explicit FFmpeg encoder name. 
#### Parameters
* `name` The codec display name. 

* `encoder` The FFmpeg encoder name (e.g. "libx264"). 

* `sampleRate` The sampling rate or RTP clock rate in Hz. 

* `bitRate` The target bit rate in bits per second. 

* `enabled` Whether the codec is available for use.

---

{#codec-4}

#### ~Codec

`virtual` `noexcept`

```cpp
virtual ~Codec() noexcept
```

Defined in src/av/include/icy/av/codec.h:70

Codec(const Codec& r);.

---

{#tostring}

#### toString

`virtual` `const`

```cpp
virtual std::string toString() const
```

Defined in src/av/include/icy/av/codec.h:77

Return a compact string representation of this codec. 
#### Returns
A string in the form "Codec[name:encoder:sampleRate:enabled]".

---

{#specified}

#### specified

`const` `nodiscard`

```cpp
[[nodiscard]] bool specified() const
```

Defined in src/av/include/icy/av/codec.h:81

Returns true when this codec explicitly names either an RTP/media codec or an FFmpeg encoder and is enabled for use.

---

{#print}

#### print

`virtual`

```cpp
virtual void print(std::ostream & ost)
```

Defined in src/av/include/icy/av/codec.h:85

Print a multi-line human-readable description to the given stream. 
#### Parameters
* `ost` The output stream to write to.

{#audiocodec}

## AudioCodec

```cpp
#include <icy/av/codec.h>
```

```cpp
struct AudioCodec
```

Defined in src/av/include/icy/av/codec.h:102

> **Inherits:** [`Codec`](#codec)

Audio codec parameters including channels, sample rate, and sample format.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `int` | [`channels`](#channels)  |  |
| `std::string` | [`sampleFmt`](#samplefmt)  | One of: u8, s16, s32, flt, dbl, u8p, s16p, s32p, fltp, dblp. |

---

{#channels}

#### channels

```cpp
int channels
```

Defined in src/av/include/icy/av/codec.h:104

---

{#samplefmt}

#### sampleFmt

```cpp
std::string sampleFmt
```

Defined in src/av/include/icy/av/codec.h:105

One of: u8, s16, s32, flt, dbl, u8p, s16p, s32p, fltp, dblp.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`AudioCodec`](#audiocodec-1)  | Construct a disabled audio codec with zeroed parameters. |
|  | [`AudioCodec`](#audiocodec-2)  | Construct an anonymous audio codec from raw parameters. |
|  | [`AudioCodec`](#audiocodec-3)  | Construct a named audio codec. |
|  | [`AudioCodec`](#audiocodec-4)  | Construct a named audio codec with an explicit FFmpeg encoder name. |
| `std::string` | [`toString`](#tostring-1) `virtual` `const` `override` |  |
| `void` | [`print`](#print-1) `virtual` `override` | Print a multi-line human-readable description to the given stream. |

---

{#audiocodec-1}

#### AudioCodec

```cpp
AudioCodec()
```

Defined in src/av/include/icy/av/codec.h:108

Construct a disabled audio codec with zeroed parameters.

---

{#audiocodec-2}

#### AudioCodec

```cpp
AudioCodec(int channels, int sampleRate, const std::string & sampleFmt = "", int bitRate = 0)
```

Defined in src/av/include/icy/av/codec.h:115

Construct an anonymous audio codec from raw parameters. 
#### Parameters
* `channels` The number of audio channels. 

* `sampleRate` The sample rate in Hz. 

* `sampleFmt` The sample format string (e.g. "s16", "fltp"). 

* `bitRate` The target bit rate in bits per second.

---

{#audiocodec-3}

#### AudioCodec

```cpp
AudioCodec(const std::string & name, int channels = 0, int sampleRate = 0, int bitRate = 0, const std::string & sampleFmt = "")
```

Defined in src/av/include/icy/av/codec.h:126

Construct a named audio codec. 
#### Parameters
* `name` The codec display name. 

* `channels` The number of audio channels. 

* `sampleRate` The sample rate in Hz. 

* `bitRate` The target bit rate in bits per second. 

* `sampleFmt` The sample format string.

---

{#audiocodec-4}

#### AudioCodec

```cpp
AudioCodec(const std::string & name, const std::string & encoder, int channels = 0, int sampleRate = 0, int bitRate = 0, const std::string & sampleFmt = "")
```

Defined in src/av/include/icy/av/codec.h:139

Construct a named audio codec with an explicit FFmpeg encoder name. 
#### Parameters
* `name` The codec display name. 

* `encoder` The FFmpeg encoder name (e.g. "libopus"). 

* `channels` The number of audio channels. 

* `sampleRate` The sample rate in Hz. 

* `bitRate` The target bit rate in bits per second. 

* `sampleFmt` The sample format string.

---

{#tostring-1}

#### toString

`virtual` `const` `override`

```cpp
virtual std::string toString() const override
```

Defined in src/av/include/icy/av/codec.h:149

#### Returns
A string in the form "AudioCodec[name:encoder:sampleRate:bitRate:channels:sampleFmt:enabled]".

---

{#print-1}

#### print

`virtual` `override`

```cpp
virtual void print(std::ostream & ost) override
```

Defined in src/av/include/icy/av/codec.h:153

Print a multi-line human-readable description to the given stream. 
#### Parameters
* `ost` The output stream to write to.

{#videocodec}

## VideoCodec

```cpp
#include <icy/av/codec.h>
```

```cpp
struct VideoCodec
```

Defined in src/av/include/icy/av/codec.h:169

> **Inherits:** [`Codec`](#codec)

Video codec parameters including resolution, frame rate, and pixel format.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `int` | [`width`](#width)  |  |
| `int` | [`height`](#height)  |  |
| `double` | [`fps`](#fps)  |  |
| `std::string` | [`pixelFmt`](#pixelfmt)  |  |

---

{#width}

#### width

```cpp
int width
```

Defined in src/av/include/icy/av/codec.h:171

---

{#height}

#### height

```cpp
int height
```

Defined in src/av/include/icy/av/codec.h:172

---

{#fps}

#### fps

```cpp
double fps
```

Defined in src/av/include/icy/av/codec.h:173

---

{#pixelfmt}

#### pixelFmt

```cpp
std::string pixelFmt
```

Defined in src/av/include/icy/av/codec.h:174

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`VideoCodec`](#videocodec-1)  | Construct a disabled video codec with zeroed parameters. |
|  | [`VideoCodec`](#videocodec-2)  | Construct an anonymous video codec from raw parameters. |
|  | [`VideoCodec`](#videocodec-3)  | Construct a named video codec. |
|  | [`VideoCodec`](#videocodec-4)  | Construct a named video codec with an explicit FFmpeg encoder name. |
|  | [`VideoCodec`](#videocodec-5)  |  |
| `std::string` | [`toString`](#tostring-2) `virtual` `const` `override` |  |
| `void` | [`print`](#print-2) `virtual` `override` | Print a multi-line human-readable description to the given stream. |

---

{#videocodec-1}

#### VideoCodec

```cpp
VideoCodec()
```

Defined in src/av/include/icy/av/codec.h:177

Construct a disabled video codec with zeroed parameters.

---

{#videocodec-2}

#### VideoCodec

```cpp
VideoCodec(int width, int height, double fps = 0.0, const std::string & pixelFmt = DEFAULT_VIDEO_PIXEL_FMT, int bitRate = 0, int sampleRate = 0)
```

Defined in src/av/include/icy/av/codec.h:186

Construct an anonymous video codec from raw parameters. 
#### Parameters
* `width` The frame width in pixels. 

* `height` The frame height in pixels. 

* `fps` The target frame rate in frames per second. 

* `pixelFmt` The pixel format string (e.g. "yuv420p"). 

* `bitRate` The target bit rate in bits per second. 

* `sampleRate` The RTP clock rate in Hz.

---

{#videocodec-3}

#### VideoCodec

```cpp
VideoCodec(const std::string & name, int width = 0, int height = 0, double fps = 0.0, int bitRate = 0, int sampleRate = 0, const std::string & pixelFmt = DEFAULT_VIDEO_PIXEL_FMT)
```

Defined in src/av/include/icy/av/codec.h:199

Construct a named video codec. 
#### Parameters
* `name` The codec display name. 

* `width` The frame width in pixels. 

* `height` The frame height in pixels. 

* `fps` The target frame rate in frames per second. 

* `bitRate` The target bit rate in bits per second. 

* `sampleRate` The RTP clock rate in Hz. 

* `pixelFmt` The pixel format string.

---

{#videocodec-4}

#### VideoCodec

```cpp
VideoCodec(const std::string & name, const std::string & encoder, int width = 0, int height = 0, double fps = 0.0, int bitRate = 0, int sampleRate = 0, const std::string & pixelFmt = DEFAULT_VIDEO_PIXEL_FMT)
```

Defined in src/av/include/icy/av/codec.h:213

Construct a named video codec with an explicit FFmpeg encoder name. 
#### Parameters
* `name` The codec display name. 

* `encoder` The FFmpeg encoder name (e.g. "libx264"). 

* `width` The frame width in pixels. 

* `height` The frame height in pixels. 

* `fps` The target frame rate in frames per second. 

* `bitRate` The target bit rate in bits per second. 

* `sampleRate` The RTP clock rate in Hz. 

* `pixelFmt` The pixel format string.

---

{#videocodec-5}

#### VideoCodec

```cpp
VideoCodec(const VideoCodec & r)
```

Defined in src/av/include/icy/av/codec.h:218

---

{#tostring-2}

#### toString

`virtual` `const` `override`

```cpp
virtual std::string toString() const override
```

Defined in src/av/include/icy/av/codec.h:223

#### Returns
A string in the form "VideoCodec[name:encoder:width:height:fps:pixelFmt:enabled]".

---

{#print-2}

#### print

`virtual` `override`

```cpp
virtual void print(std::ostream & ost) override
```

Defined in src/av/include/icy/av/codec.h:227

Print a multi-line human-readable description to the given stream. 
#### Parameters
* `ost` The output stream to write to.

{#deleter-1}

## Deleter

```cpp
#include <icy/av/ffmpeg.h>
```

```cpp
template<typename T, typename R, R(*)(T *) Fn>
struct Deleter
```

Defined in src/av/include/icy/av/ffmpeg.h:70

RAII helpers for owning FFmpeg allocation types.

[Deleter](#deleter-1) adaptor for functions like av_free that take a pointer.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`operator()`](#operator) `const` `inline` |  |

---

{#operator}

#### operator()

`const` `inline`

```cpp
inline void operator()(T * p) const
```

Defined in src/av/include/icy/av/ffmpeg.h:72

{#deleterp}

## Deleterp

```cpp
#include <icy/av/ffmpeg.h>
```

```cpp
template<typename T, typename R, R(*)(T **) Fn>
struct Deleterp
```

Defined in src/av/include/icy/av/ffmpeg.h:81

[Deleter](#deleter-1) adaptor for functions like av_freep that take a pointer to a pointer.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`operator()`](#operator-1) `const` `inline` |  |

---

{#operator-1}

#### operator()

`const` `inline`

```cpp
inline void operator()(T * p) const
```

Defined in src/av/include/icy/av/ffmpeg.h:83

{#format}

## Format

```cpp
#include <icy/av/format.h>
```

```cpp
struct Format
```

Defined in src/av/include/icy/av/format.h:30

Defines a media container format which is available through the [FormatRegistry](#formatregistry) for encoding or decoding.

A format bundles the preferred default audio and video codec settings for a named container such as MP4, MKV, or WAV.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`name`](#name-1)  | Base [Format](#format) Variables. |
| `std::string` | [`id`](#id)  | The short name of this format. |
| `VideoCodec` | [`video`](#video)  | The video codec. |
| `AudioCodec` | [`audio`](#audio)  | The audio codec. |
| `int` | [`priority`](#priority)  | The priority this format will be displayed on the list. |

---

{#name-1}

#### name

```cpp
std::string name
```

Defined in src/av/include/icy/av/format.h:43

Base [Format](#format) Variables.

The display name of this format.

---

{#id}

#### id

```cpp
std::string id
```

Defined in src/av/include/icy/av/format.h:44

The short name of this format.

---

{#video}

#### video

```cpp
VideoCodec video
```

Defined in src/av/include/icy/av/format.h:46

The video codec.

---

{#audio}

#### audio

```cpp
AudioCodec audio
```

Defined in src/av/include/icy/av/format.h:47

The audio codec.

---

{#priority}

#### priority

```cpp
int priority
```

Defined in src/av/include/icy/av/format.h:49

The priority this format will be displayed on the list.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Format`](#format-1)  | Ctors/Dtors. |
|  | [`Format`](#format-2)  | Construct a multiplex (audio + video) format. |
|  | [`Format`](#format-3)  | Construct a video-only format. |
|  | [`Format`](#format-4)  | Construct an audio-only format. |
|  | [`Format`](#format-5)  |  |
| `Type` | [`type`](#type) `const` | Return the media type (None, Video, Audio, or Multiplex) derived from which codecs are enabled. |
| `std::string` | [`toString`](#tostring-3) `virtual` `const` |  |
| `void` | [`print`](#print-3) `virtual` | Print a multi-line human-readable description to the given stream. |

---

{#format-1}

#### Format

```cpp
Format()
```

Defined in src/av/include/icy/av/format.h:56

Ctors/Dtors.

Construct an empty format with no codecs enabled.

---

{#format-2}

#### Format

```cpp
Format(const std::string & name, const std::string & id, const VideoCodec & video, const AudioCodec & audio, int priority = 0)
```

Defined in src/av/include/icy/av/format.h:64

Construct a multiplex (audio + video) format. 
#### Parameters
* `name` The display name of the format. 

* `id` The short FFmpeg muxer ID (e.g. "mp4"). 

* `video` The video codec parameters. 

* `audio` The audio codec parameters. 

* `priority` Sort priority; higher values appear first.

---

{#format-3}

#### Format

```cpp
Format(const std::string & name, const std::string & id, const VideoCodec & video, int priority = 0)
```

Defined in src/av/include/icy/av/format.h:73

Construct a video-only format. 
#### Parameters
* `name` The display name of the format. 

* `id` The short FFmpeg muxer ID. 

* `video` The video codec parameters. 

* `priority` Sort priority; higher values appear first.

---

{#format-4}

#### Format

```cpp
Format(const std::string & name, const std::string & id, const AudioCodec & audio, int priority = 0)
```

Defined in src/av/include/icy/av/format.h:82

Construct an audio-only format. 
#### Parameters
* `name` The display name of the format. 

* `id` The short FFmpeg muxer ID. 

* `audio` The audio codec parameters. 

* `priority` Sort priority; higher values appear first.

---

{#format-5}

#### Format

```cpp
Format(const Format & r)
```

Defined in src/av/include/icy/av/format.h:87

---

{#type}

#### type

`const`

```cpp
Type type() const
```

Defined in src/av/include/icy/av/format.h:93

Return the media type (None, Video, Audio, or Multiplex) derived from which codecs are enabled.

---

{#tostring-3}

#### toString

`virtual` `const`

```cpp
virtual std::string toString() const
```

Defined in src/av/include/icy/av/format.h:96

#### Returns
A compact string representation of the format and its enabled codecs.

---

{#print-3}

#### print

`virtual`

```cpp
virtual void print(std::ostream & ost)
```

Defined in src/av/include/icy/av/format.h:100

Print a multi-line human-readable description to the given stream. 
#### Parameters
* `ost` The output stream to write to.

### Public Static Methods

| Return | Name | Description |
|--------|------|-------------|
| `bool` | [`preferable`](#preferable) `static` `inline` | Comparator returning true if `first` has higher priority than `second`. |

---

{#preferable}

#### preferable

`static` `inline`

```cpp
static inline bool preferable(const Format & first, const Format & second)
```

Defined in src/av/include/icy/av/format.h:106

Comparator returning true if `first` has higher priority than `second`. 
#### Parameters
* `first` The format to compare. 

* `second` The format to compare against. 

#### Returns
True if first.priority > second.priority.

### Public Types

| Name | Description |
|------|-------------|
| [`Type`](#type-1)  |  |

---

{#type-1}

#### Type

```cpp
enum Type
```

Defined in src/av/include/icy/av/format.h:32

| Value | Description |
|-------|-------------|
| `None` |  |
| `Video` | video only |
| `Audio` | audio only |
| `Multiplex` | both video & audio |

{#encoderstate}

## EncoderState

```cpp
#include <icy/av/iencoder.h>
```

```cpp
struct EncoderState
```

Defined in src/av/include/icy/av/iencoder.h:26

> **Inherits:** [`State`](base.md#state)

[State](base.md#state) machine states for the encoder pipeline.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`str`](#str) `const` `inline` |  |

---

{#str}

#### str

`const` `inline`

```cpp
inline std::string str(unsigned int id) const
```

Defined in src/av/include/icy/av/iencoder.h:37

### Public Types

| Name | Description |
|------|-------------|
| [`Type`](#type-2)  |  |

---

{#type-2}

#### Type

```cpp
enum Type
```

Defined in src/av/include/icy/av/iencoder.h:28

| Value | Description |
|-------|-------------|
| `None` |  |
| `Ready` |  |
| `Encoding` |  |
| `Stopped` |  |
| `Error` |  |

{#encoderoptions}

## EncoderOptions

```cpp
#include <icy/av/iencoder.h>
```

```cpp
struct EncoderOptions
```

Defined in src/av/include/icy/av/iencoder.h:57

[Configuration](base.md#configuration) options for audio and video encoders.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `Format` | [`iformat`](#iformat)  | input media format. |
| `Format` | [`oformat`](#oformat)  | output media format. |
| `std::string` | [`ifile`](#ifile)  | input file path. |
| `std::string` | [`ofile`](#ofile)  | output file path. |
| `long` | [`duration`](#duration)  | duration of time to record in nanoseconds. |

---

{#iformat}

#### iformat

```cpp
Format iformat
```

Defined in src/av/include/icy/av/iencoder.h:59

input media format.

---

{#oformat}

#### oformat

```cpp
Format oformat
```

Defined in src/av/include/icy/av/iencoder.h:60

output media format.

---

{#ifile}

#### ifile

```cpp
std::string ifile
```

Defined in src/av/include/icy/av/iencoder.h:61

input file path.

---

{#ofile}

#### ofile

```cpp
std::string ofile
```

Defined in src/av/include/icy/av/iencoder.h:62

output file path.

---

{#duration}

#### duration

```cpp
long duration
```

Defined in src/av/include/icy/av/iencoder.h:63

duration of time to record in nanoseconds.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`EncoderOptions`](#encoderoptions-1) `inline` |  |

---

{#encoderoptions-1}

#### EncoderOptions

`inline`

```cpp
inline EncoderOptions(const Format & iformat = Format(), const Format & oformat = Format(), const std::string & ifile = "", const std::string & ofile = "", long duration = 0)
```

Defined in src/av/include/icy/av/iencoder.h:64

{#audiobuffer}

## AudioBuffer

```cpp
#include <icy/av/audiobuffer.h>
```

```cpp
struct AudioBuffer
```

Defined in src/av/include/icy/av/audiobuffer.h:31

FIFO buffer for queuing audio samples between encoding stages.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `AVAudioFifo *` | [`fifo`](#fifo)  | Underlying FFmpeg audio FIFO handle. |

---

{#fifo}

#### fifo

```cpp
AVAudioFifo * fifo
```

Defined in src/av/include/icy/av/audiobuffer.h:71

Underlying FFmpeg audio FIFO handle.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`AudioBuffer`](#audiobuffer-1)  |  |
|  | [`AudioBuffer`](#audiobuffer-2)  | Deleted constructor. |
|  | [`AudioBuffer`](#audiobuffer-3)  | Deleted constructor. |
| `void` | [`alloc`](#alloc)  | Allocate the audio FIFO buffer. |
| `void` | [`reset`](#reset-2)  | Discard all samples currently held in the FIFO without freeing the buffer. |
| `void` | [`close`](#close-1)  | Free the underlying AVAudioFifo buffer. |
| `void` | [`write`](#write)  | Write samples into the FIFO buffer. |
| `bool` | [`read`](#read)  | Read samples from the FIFO buffer. |
| `int` | [`available`](#available) `const` | Return the number of samples per channel currently available in the FIFO. |

---

{#audiobuffer-1}

#### AudioBuffer

```cpp
AudioBuffer()
```

Defined in src/av/include/icy/av/audiobuffer.h:33

---

{#audiobuffer-2}

#### AudioBuffer

```cpp
AudioBuffer(const AudioBuffer &) = delete
```

Defined in src/av/include/icy/av/audiobuffer.h:36

Deleted constructor.

---

{#audiobuffer-3}

#### AudioBuffer

```cpp
AudioBuffer(AudioBuffer &&) = delete
```

Defined in src/av/include/icy/av/audiobuffer.h:38

Deleted constructor.

---

{#alloc}

#### alloc

```cpp
void alloc(const std::string & sampleFmt, int channels, int numSamples = 1024)
```

Defined in src/av/include/icy/av/audiobuffer.h:46

Allocate the audio FIFO buffer.

#### Parameters
* `sampleFmt` The sample format name (e.g. "s16", "fltp"). 

* `channels` The number of audio channels. 

* `numSamples` The initial buffer capacity in samples per channel.

---

{#reset-2}

#### reset

```cpp
void reset()
```

Defined in src/av/include/icy/av/audiobuffer.h:49

Discard all samples currently held in the FIFO without freeing the buffer.

---

{#close-1}

#### close

```cpp
void close()
```

Defined in src/av/include/icy/av/audiobuffer.h:52

Free the underlying AVAudioFifo buffer.

---

{#write}

#### write

```cpp
void write(void ** samples, int numSamples)
```

Defined in src/av/include/icy/av/audiobuffer.h:58

Write samples into the FIFO buffer.

#### Parameters
* `samples` Array of per-channel sample buffers. 

* `numSamples` The number of samples per channel to write.

---

{#read}

#### read

```cpp
bool read(void ** samples, int numSamples)
```

Defined in src/av/include/icy/av/audiobuffer.h:65

Read samples from the FIFO buffer.

#### Parameters
* `samples` Array of per-channel sample buffers to fill. 

* `numSamples` The number of samples per channel to read. 

#### Returns
True if enough samples were available.

---

{#available}

#### available

`const`

```cpp
int available() const
```

Defined in src/av/include/icy/av/audiobuffer.h:69

Return the number of samples per channel currently available in the FIFO. 
#### Returns
The number of available samples, or zero if the buffer is not allocated.

{#audiocontext}

## AudioContext

```cpp
#include <icy/av/audiocontext.h>
```

```cpp
struct AudioContext
```

Defined in src/av/include/icy/av/audiocontext.h:39

> **Subclassed by:** [`AudioDecoder`](#audiodecoder), [`AudioEncoder`](#audioencoder)

Base context for audio encoding and decoding via FFmpeg.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `PacketSignal` | [`emitter`](#emitter-2)  |  |
| `AudioCodec` | [`iparams`](#iparams)  | input parameters |
| `AudioCodec` | [`oparams`](#oparams)  | output parameters |
| `AVStream *` | [`stream`](#stream-1)  | encoder or decoder stream |
| `AVCodecContext *` | [`ctx`](#ctx)  | encoder or decoder context |
| `const AVCodec *` | [`codec`](#codec-5)  | encoder or decoder codec |
| `AVFrame *` | [`frame`](#frame)  | last encoded or decoded frame |
| `std::unique_ptr< AudioResampler >` | [`resampler`](#resampler)  | audio resampler |
| `int` | [`outputFrameSize`](#outputframesize)  | encoder or decoder output frame size |
| `int64_t` | [`time`](#time)  | stream time in codec time base |
| `int64_t` | [`pts`](#pts)  | last packet pts value |
| `double` | [`seconds`](#seconds)  | audio time in seconds |
| `std::string` | [`error`](#error-1)  | error message |

---

{#emitter-2}

#### emitter

```cpp
PacketSignal emitter
```

Defined in src/av/include/icy/av/audiocontext.h:92

---

{#iparams}

#### iparams

```cpp
AudioCodec iparams
```

Defined in src/av/include/icy/av/audiocontext.h:94

input parameters

---

{#oparams}

#### oparams

```cpp
AudioCodec oparams
```

Defined in src/av/include/icy/av/audiocontext.h:95

output parameters

---

{#stream-1}

#### stream

```cpp
AVStream * stream
```

Defined in src/av/include/icy/av/audiocontext.h:96

encoder or decoder stream

---

{#ctx}

#### ctx

```cpp
AVCodecContext * ctx
```

Defined in src/av/include/icy/av/audiocontext.h:97

encoder or decoder context

---

{#codec-5}

#### codec

```cpp
const AVCodec * codec
```

Defined in src/av/include/icy/av/audiocontext.h:98

encoder or decoder codec

---

{#frame}

#### frame

```cpp
AVFrame * frame
```

Defined in src/av/include/icy/av/audiocontext.h:99

last encoded or decoded frame

---

{#resampler}

#### resampler

```cpp
std::unique_ptr< AudioResampler > resampler
```

Defined in src/av/include/icy/av/audiocontext.h:100

audio resampler

---

{#outputframesize}

#### outputFrameSize

```cpp
int outputFrameSize
```

Defined in src/av/include/icy/av/audiocontext.h:101

encoder or decoder output frame size

---

{#time}

#### time

```cpp
int64_t time
```

Defined in src/av/include/icy/av/audiocontext.h:102

stream time in codec time base

---

{#pts}

#### pts

```cpp
int64_t pts
```

Defined in src/av/include/icy/av/audiocontext.h:103

last packet pts value

---

{#seconds}

#### seconds

```cpp
double seconds
```

Defined in src/av/include/icy/av/audiocontext.h:104

audio time in seconds

---

{#error-1}

#### error

```cpp
std::string error
```

Defined in src/av/include/icy/av/audiocontext.h:105

error message

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`AudioContext`](#audiocontext-1)  |  |
|  | [`AudioContext`](#audiocontext-2)  | Deleted constructor. |
|  | [`AudioContext`](#audiocontext-3)  | Deleted constructor. |
| `void` | [`create`](#create) `virtual` | Initialise the AVCodecContext with codec-specific defaults. Implemented by [AudioEncoder](#audioencoder) and [AudioDecoder](#audiodecoder). |
| `void` | [`open`](#open) `virtual` | Open the codec and create the resampler if input/output parameters differ. Throws std::runtime_error if the codec context has not been created. |
| `void` | [`close`](#close-2) `virtual` | Close the codec context, free the frame, and reset timestamps. |
| `bool` | [`decode`](#decode) `virtual` `nodiscard` | Decode a compressed audio packet and emit the resulting samples. |
| `bool` | [`encode`](#encode) `virtual` `nodiscard` | Encode a buffer of interleaved audio samples. |
| `bool` | [`encode`](#encode-1) `virtual` `nodiscard` | Encode a buffer of planar audio samples. |
| `bool` | [`encode`](#encode-2) `virtual` `nodiscard` | Encode a single AVFrame. |
| `void` | [`flush`](#flush-1) `virtual` | Flush any frames buffered inside the codec and emit remaining output. |
| `bool` | [`recreateResampler`](#recreateresampler) `virtual` | Recreate the [AudioResampler](#audioresampler) using the current iparams and oparams. Called automatically by [open()](#open) when format conversion is required. |

---

{#audiocontext-1}

#### AudioContext

```cpp
AudioContext()
```

Defined in src/av/include/icy/av/audiocontext.h:41

---

{#audiocontext-2}

#### AudioContext

```cpp
AudioContext(const AudioContext &) = delete
```

Defined in src/av/include/icy/av/audiocontext.h:44

Deleted constructor.

---

{#audiocontext-3}

#### AudioContext

```cpp
AudioContext(AudioContext &&) = delete
```

Defined in src/av/include/icy/av/audiocontext.h:46

Deleted constructor.

---

{#create}

#### create

`virtual`

```cpp
virtual void create()
```

Defined in src/av/include/icy/av/audiocontext.h:51

Initialise the AVCodecContext with codec-specific defaults. Implemented by [AudioEncoder](#audioencoder) and [AudioDecoder](#audiodecoder).

---

{#open}

#### open

`virtual`

```cpp
virtual void open()
```

Defined in src/av/include/icy/av/audiocontext.h:55

Open the codec and create the resampler if input/output parameters differ. Throws std::runtime_error if the codec context has not been created.

---

{#close-2}

#### close

`virtual`

```cpp
virtual void close()
```

Defined in src/av/include/icy/av/audiocontext.h:58

Close the codec context, free the frame, and reset timestamps.

---

{#decode}

#### decode

`virtual` `nodiscard`

```cpp
[[nodiscard]] virtual bool decode(AVPacket & ipacket)
```

Defined in src/av/include/icy/av/audiocontext.h:63

Decode a compressed audio packet and emit the resulting samples. 
#### Parameters
* `ipacket` The compressed audio packet to decode. 

#### Returns
True if one or more decoded frames were emitted, false otherwise.

---

{#encode}

#### encode

`virtual` `nodiscard`

```cpp
[[nodiscard]] virtual bool encode(uint8_t * samples, int numSamples, int64_t pts)
```

Defined in src/av/include/icy/av/audiocontext.h:70

Encode a buffer of interleaved audio samples. 
#### Parameters
* `samples` Pointer to the interleaved sample buffer. 

* `numSamples` Number of samples per channel. 

* `pts` Presentation timestamp in stream time base units. 

#### Returns
True if an encoded packet was produced and emitted.

---

{#encode-1}

#### encode

`virtual` `nodiscard`

```cpp
[[nodiscard]] virtual bool encode(uint8_t * samples, int numSamples, int64_t pts)
```

Defined in src/av/include/icy/av/audiocontext.h:77

Encode a buffer of planar audio samples. 
#### Parameters
* `samples` Array of per-channel sample buffers (up to 4 planes). 

* `numSamples` Number of samples per channel. 

* `pts` Presentation timestamp in stream time base units. 

#### Returns
True if an encoded packet was produced and emitted.

---

{#encode-2}

#### encode

`virtual` `nodiscard`

```cpp
[[nodiscard]] virtual bool encode(AVFrame * iframe)
```

Defined in src/av/include/icy/av/audiocontext.h:82

Encode a single AVFrame. 
#### Parameters
* `iframe` The source audio frame; must have all fields set correctly. 

#### Returns
True if an encoded packet was produced and emitted.

---

{#flush-1}

#### flush

`virtual`

```cpp
virtual void flush()
```

Defined in src/av/include/icy/av/audiocontext.h:85

Flush any frames buffered inside the codec and emit remaining output.

---

{#recreateresampler}

#### recreateResampler

`virtual`

```cpp
virtual bool recreateResampler()
```

Defined in src/av/include/icy/av/audiocontext.h:90

Recreate the [AudioResampler](#audioresampler) using the current iparams and oparams. Called automatically by [open()](#open) when format conversion is required. 
#### Returns
True if the resampler was successfully created.

{#audiodecoder}

## AudioDecoder

```cpp
#include <icy/av/audiodecoder.h>
```

```cpp
struct AudioDecoder
```

Defined in src/av/include/icy/av/audiodecoder.h:28

> **Inherits:** [`AudioContext`](#audiocontext)

Decodes compressed audio packets into raw sample frames.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`AudioDecoder`](#audiodecoder-1)  | Construct a decoder for the given stream. The codec parameters are read from the stream's codecpar. |
| `void` | [`create`](#create-1) `virtual` `override` | Initialise the AVCodecContext from the stream's codec parameters. |
| `void` | [`close`](#close-3) `virtual` `override` | Close and free the AVCodecContext and associated resources. |
| `bool` | [`decode`](#decode-1) `virtual` `nodiscard` `override` | Decode the given compressed audio packet and emit the decoded samples. |
| `void` | [`flush`](#flush-2) `virtual` `override` | Flush any frames buffered inside the decoder. Call this after the last packet to retrieve all remaining decoded output. |

---

{#audiodecoder-1}

#### AudioDecoder

```cpp
AudioDecoder(AVStream * stream)
```

Defined in src/av/include/icy/av/audiodecoder.h:33

Construct a decoder for the given stream. The codec parameters are read from the stream's codecpar. 
#### Parameters
* `stream` The AVStream to decode; must remain valid for the lifetime of this decoder.

---

{#create-1}

#### create

`virtual` `override`

```cpp
virtual void create() override
```

Defined in src/av/include/icy/av/audiodecoder.h:37

Initialise the AVCodecContext from the stream's codec parameters.

---

{#close-3}

#### close

`virtual` `override`

```cpp
virtual void close() override
```

Defined in src/av/include/icy/av/audiodecoder.h:40

Close and free the AVCodecContext and associated resources.

---

{#decode-1}

#### decode

`virtual` `nodiscard` `override`

```cpp
[[nodiscard]] virtual bool decode(AVPacket & ipacket) override
```

Defined in src/av/include/icy/av/audiodecoder.h:45

Decode the given compressed audio packet and emit the decoded samples. 
#### Parameters
* `ipacket` The compressed audio packet to decode. 

#### Returns
True if one or more output frames were decoded and emitted, false otherwise.

---

{#flush-2}

#### flush

`virtual` `override`

```cpp
virtual void flush() override
```

Defined in src/av/include/icy/av/audiodecoder.h:49

Flush any frames buffered inside the decoder. Call this after the last packet to retrieve all remaining decoded output.

{#audioencoder}

## AudioEncoder

```cpp
#include <icy/av/audioencoder.h>
```

```cpp
struct AudioEncoder
```

Defined in src/av/include/icy/av/audioencoder.h:37

> **Inherits:** [`AudioContext`](#audiocontext)
> **Subclassed by:** [`AudioPacketEncoder`](#audiopacketencoder)

Encodes raw audio samples into a compressed format.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `av::AudioBuffer` | [`fifo`](#fifo-1)  |  |
| `AVFormatContext *` | [`format`](#format-6)  |  |
| `int64_t` | [`nextOutputPts`](#nextoutputpts)  | Running PTS counter for encoder output, in encoder time_base units. Anchored to the first input packet's PTS, then advanced by frame->nb_samples per encoded frame. AV_NOPTS_VALUE before anchoring. |

---

{#fifo-1}

#### fifo

```cpp
av::AudioBuffer fifo
```

Defined in src/av/include/icy/av/audioencoder.h:74

---

{#format-6}

#### format

```cpp
AVFormatContext * format
```

Defined in src/av/include/icy/av/audioencoder.h:75

---

{#nextoutputpts}

#### nextOutputPts

```cpp
int64_t nextOutputPts = AV_NOPTS_VALUE
```

Defined in src/av/include/icy/av/audioencoder.h:80

Running PTS counter for encoder output, in encoder time_base units. Anchored to the first input packet's PTS, then advanced by frame->nb_samples per encoded frame. AV_NOPTS_VALUE before anchoring.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`AudioEncoder`](#audioencoder-1)  | Construct an encoder, optionally tied to an existing muxer context. |
| `void` | [`create`](#create-2) `virtual` `override` | Initialise the AVCodecContext using oparams. Adds an audio stream to `format` if one was provided at construction. |
| `void` | [`close`](#close-4) `virtual` `override` | Close and free the AVCodecContext, FIFO buffer, and associated resources. |
| `bool` | [`encode`](#encode-3) `virtual` `nodiscard` `override` | Encode interleaved audio samples. |
| `bool` | [`encode`](#encode-4) `virtual` `nodiscard` `override` | Encode planar audio samples. |
| `bool` | [`encode`](#encode-5) `virtual` `nodiscard` `override` | Encode a single AVFrame (typically from a decoder or resampler). |
| `void` | [`flush`](#flush-3) `virtual` `override` | Flush remaining packets to be encoded. This method should be called once before stream closure. |

---

{#audioencoder-1}

#### AudioEncoder

```cpp
AudioEncoder(AVFormatContext * format = nullptr)
```

Defined in src/av/include/icy/av/audioencoder.h:41

Construct an encoder, optionally tied to an existing muxer context. 
#### Parameters
* `format` The AVFormatContext that will receive encoded packets, or nullptr for standalone use.

---

{#create-2}

#### create

`virtual` `override`

```cpp
virtual void create() override
```

Defined in src/av/include/icy/av/audioencoder.h:46

Initialise the AVCodecContext using oparams. Adds an audio stream to `format` if one was provided at construction.

---

{#close-4}

#### close

`virtual` `override`

```cpp
virtual void close() override
```

Defined in src/av/include/icy/av/audioencoder.h:49

Close and free the AVCodecContext, FIFO buffer, and associated resources.

---

{#encode-3}

#### encode

`virtual` `nodiscard` `override`

```cpp
[[nodiscard]] virtual bool encode(uint8_t * samples, const int numSamples, const int64_t pts) override
```

Defined in src/av/include/icy/av/audioencoder.h:56

Encode interleaved audio samples.

#### Parameters
* `samples` The input samples to encode. 

* `numSamples` The number of input samples per channel. 

* `pts` The input samples presentation timestamp.

---

{#encode-4}

#### encode

`virtual` `nodiscard` `override`

```cpp
[[nodiscard]] virtual bool encode(uint8_t * samples, const int numSamples, const int64_t pts) override
```

Defined in src/av/include/icy/av/audioencoder.h:63

Encode planar audio samples.

#### Parameters
* `samples` The input samples to encode. 

* `numSamples` The number of input samples per channel. 

* `pts` The input samples presentation timestamp.

---

{#encode-5}

#### encode

`virtual` `nodiscard` `override`

```cpp
[[nodiscard]] virtual bool encode(AVFrame * iframe) override
```

Defined in src/av/include/icy/av/audioencoder.h:68

Encode a single AVFrame (typically from a decoder or resampler). 
#### Parameters
* `iframe` The source audio frame with all fields set. 

#### Returns
True if an encoded packet was produced and emitted.

---

{#flush-3}

#### flush

`virtual` `override`

```cpp
virtual void flush() override
```

Defined in src/av/include/icy/av/audioencoder.h:72

Flush remaining packets to be encoded. This method should be called once before stream closure.

{#videocontext}

## VideoContext

```cpp
#include <icy/av/videocontext.h>
```

```cpp
struct VideoContext
```

Defined in src/av/include/icy/av/videocontext.h:42

> **Subclassed by:** [`VideoDecoder`](#videodecoder), [`VideoEncoder`](#videoencoder)

Base video context from which all video encoders and decoders derive.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `PacketSignal` | [`emitter`](#emitter-3)  |  |
| `VideoCodec` | [`iparams`](#iparams-1)  | input parameters |
| `VideoCodec` | [`oparams`](#oparams-1)  | output parameters |
| `AVStream *` | [`stream`](#stream-2)  | encoder or decoder stream |
| `AVCodecContext *` | [`ctx`](#ctx-1)  | encoder or decoder context |
| `const AVCodec *` | [`codec`](#codec-6)  | encoder or decoder codec |
| `AVFrame *` | [`frame`](#frame-1)  | encoder or decoder frame |
| `std::unique_ptr< VideoConverter >` | [`conv`](#conv)  | video conversion context |
| `int64_t` | [`time`](#time-1)  | stream time in codec time base |
| `int64_t` | [`pts`](#pts-1)  | last packet pts value |
| `double` | [`seconds`](#seconds-1)  | video time in seconds |
| `std::string` | [`error`](#error-3)  | error message |

---

{#emitter-3}

#### emitter

```cpp
PacketSignal emitter
```

Defined in src/av/include/icy/av/videocontext.h:103

---

{#iparams-1}

#### iparams

```cpp
VideoCodec iparams
```

Defined in src/av/include/icy/av/videocontext.h:105

input parameters

---

{#oparams-1}

#### oparams

```cpp
VideoCodec oparams
```

Defined in src/av/include/icy/av/videocontext.h:106

output parameters

---

{#stream-2}

#### stream

```cpp
AVStream * stream
```

Defined in src/av/include/icy/av/videocontext.h:107

encoder or decoder stream

---

{#ctx-1}

#### ctx

```cpp
AVCodecContext * ctx
```

Defined in src/av/include/icy/av/videocontext.h:108

encoder or decoder context

---

{#codec-6}

#### codec

```cpp
const AVCodec * codec
```

Defined in src/av/include/icy/av/videocontext.h:109

encoder or decoder codec

---

{#frame-1}

#### frame

```cpp
AVFrame * frame
```

Defined in src/av/include/icy/av/videocontext.h:110

encoder or decoder frame

---

{#conv}

#### conv

```cpp
std::unique_ptr< VideoConverter > conv
```

Defined in src/av/include/icy/av/videocontext.h:111

video conversion context

---

{#time-1}

#### time

```cpp
int64_t time
```

Defined in src/av/include/icy/av/videocontext.h:112

stream time in codec time base

---

{#pts-1}

#### pts

```cpp
int64_t pts
```

Defined in src/av/include/icy/av/videocontext.h:113

last packet pts value

---

{#seconds-1}

#### seconds

```cpp
double seconds
```

Defined in src/av/include/icy/av/videocontext.h:114

video time in seconds

---

{#error-3}

#### error

```cpp
std::string error
```

Defined in src/av/include/icy/av/videocontext.h:115

error message

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`VideoContext`](#videocontext-1)  |  |
|  | [`VideoContext`](#videocontext-2)  | Deleted constructor. |
|  | [`VideoContext`](#videocontext-3)  | Deleted constructor. |
| `void` | [`create`](#create-3) `virtual` | Initialise the AVCodecContext with codec-specific defaults. Overridden by [VideoEncoder](#videoencoder) and [VideoDecoder](#videodecoder). |
| `void` | [`open`](#open-1) `virtual` | Open the codec and create the pixel format conversion context if required. Throws std::runtime_error if the codec context has not been created. |
| `void` | [`close`](#close-6) `virtual` | Close the codec context, free the frame, and reset timestamps. |
| `bool` | [`decode`](#decode-2) `virtual` `nodiscard` | Decode a compressed video packet and emit the resulting frame. |
| `bool` | [`encode`](#encode-6) `virtual` `nodiscard` | Encode a buffer of interleaved video data. |
| `bool` | [`encode`](#encode-7) `virtual` `nodiscard` | Encode a planar video frame. |
| `bool` | [`encode`](#encode-8) `virtual` `nodiscard` | Encode a single AVFrame. |
| `void` | [`flush`](#flush-4) `virtual` | Flush any frames buffered inside the codec and emit remaining output. |
| `AVFrame *` | [`convert`](#convert) `virtual` | Convert the video frame and return the result. |
| `bool` | [`recreateConverter`](#recreateconverter) `virtual` | Recreate the [VideoConverter](#videoconverter) if the input or output parameters have changed. Called automatically by [open()](#open-1) and [convert()](#convert). |

---

{#videocontext-1}

#### VideoContext

```cpp
VideoContext()
```

Defined in src/av/include/icy/av/videocontext.h:44

---

{#videocontext-2}

#### VideoContext

```cpp
VideoContext(const VideoContext &) = delete
```

Defined in src/av/include/icy/av/videocontext.h:47

Deleted constructor.

---

{#videocontext-3}

#### VideoContext

```cpp
VideoContext(VideoContext &&) = delete
```

Defined in src/av/include/icy/av/videocontext.h:49

Deleted constructor.

---

{#create-3}

#### create

`virtual`

```cpp
virtual void create()
```

Defined in src/av/include/icy/av/videocontext.h:54

Initialise the AVCodecContext with codec-specific defaults. Overridden by [VideoEncoder](#videoencoder) and [VideoDecoder](#videodecoder).

---

{#open-1}

#### open

`virtual`

```cpp
virtual void open()
```

Defined in src/av/include/icy/av/videocontext.h:58

Open the codec and create the pixel format conversion context if required. Throws std::runtime_error if the codec context has not been created.

---

{#close-6}

#### close

`virtual`

```cpp
virtual void close()
```

Defined in src/av/include/icy/av/videocontext.h:61

Close the codec context, free the frame, and reset timestamps.

---

{#decode-2}

#### decode

`virtual` `nodiscard`

```cpp
[[nodiscard]] virtual bool decode(AVPacket & ipacket)
```

Defined in src/av/include/icy/av/videocontext.h:66

Decode a compressed video packet and emit the resulting frame. 
#### Parameters
* `ipacket` The compressed video packet to decode. 

#### Returns
True if an output frame was decoded and emitted, false otherwise.

---

{#encode-6}

#### encode

`virtual` `nodiscard`

```cpp
[[nodiscard]] virtual bool encode(uint8_t * data, int size, int64_t pts)
```

Defined in src/av/include/icy/av/videocontext.h:73

Encode a buffer of interleaved video data. 
#### Parameters
* `data` Pointer to the interleaved frame buffer. 

* `size` Size of the buffer in bytes. 

* `pts` Presentation timestamp in stream time base units. 

#### Returns
True if an encoded packet was produced and emitted.

---

{#encode-7}

#### encode

`virtual` `nodiscard`

```cpp
[[nodiscard]] virtual bool encode(uint8_t * data, int linesize, int64_t pts)
```

Defined in src/av/include/icy/av/videocontext.h:80

Encode a planar video frame. 
#### Parameters
* `data` Array of per-plane data pointers (up to 4 planes). 

* `linesize` Array of per-plane byte strides. 

* `pts` Presentation timestamp in stream time base units. 

#### Returns
True if an encoded packet was produced and emitted.

---

{#encode-8}

#### encode

`virtual` `nodiscard`

```cpp
[[nodiscard]] virtual bool encode(AVFrame * iframe)
```

Defined in src/av/include/icy/av/videocontext.h:85

Encode a single AVFrame. 
#### Parameters
* `iframe` The source video frame; must have all fields set correctly. 

#### Returns
True if an encoded packet was produced and emitted.

---

{#flush-4}

#### flush

`virtual`

```cpp
virtual void flush()
```

Defined in src/av/include/icy/av/videocontext.h:88

Flush any frames buffered inside the codec and emit remaining output.

---

{#convert}

#### convert

`virtual`

```cpp
virtual AVFrame * convert(AVFrame * iframe)
```

Defined in src/av/include/icy/av/videocontext.h:96

Convert the video frame and return the result.

The input frame will only be converted if it doesn't match the output format. If the frame is not converted the input frame will be returned. If the input frame format does not match the `[VideoConverter](#videoconverter)` context then the `[VideoConverter](#videoconverter)` will be recreated with the input frame params.

---

{#recreateconverter}

#### recreateConverter

`virtual`

```cpp
virtual bool recreateConverter()
```

Defined in src/av/include/icy/av/videocontext.h:101

Recreate the [VideoConverter](#videoconverter) if the input or output parameters have changed. Called automatically by [open()](#open-1) and [convert()](#convert). 
#### Returns
True if the converter was (re)created, false if it is already up to date.

{#videodecoder}

## VideoDecoder

```cpp
#include <icy/av/videodecoder.h>
```

```cpp
struct VideoDecoder
```

Defined in src/av/include/icy/av/videodecoder.h:28

> **Inherits:** [`VideoContext`](#videocontext)

Decodes compressed video packets into raw frames.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`VideoDecoder`](#videodecoder-1)  | Construct a decoder for the given stream. The codec parameters are read from the stream's codecpar. |
| `void` | [`create`](#create-4) `virtual` `override` | Initialise the AVCodecContext from the stream's codec parameters. |
| `void` | [`open`](#open-2) `virtual` `override` | Open the codec and initialise any required pixel format conversion context. |
| `void` | [`close`](#close-7) `virtual` `override` | Close and free the AVCodecContext and associated resources. |
| `bool` | [`decode`](#decode-3) `virtual` `nodiscard` `override` | Decode the given compressed video packet and emit the decoded frame. Input packets must use the raw AVStream time base; time base conversion to microseconds is performed internally. |
| `void` | [`flush`](#flush-5) `virtual` `override` | Flush any frames buffered inside the decoder. Call repeatedly after the last packet until false is returned. |

---

{#videodecoder-1}

#### VideoDecoder

```cpp
VideoDecoder(AVStream * stream)
```

Defined in src/av/include/icy/av/videodecoder.h:33

Construct a decoder for the given stream. The codec parameters are read from the stream's codecpar. 
#### Parameters
* `stream` The AVStream to decode; must remain valid for the lifetime of this decoder.

---

{#create-4}

#### create

`virtual` `override`

```cpp
virtual void create() override
```

Defined in src/av/include/icy/av/videodecoder.h:37

Initialise the AVCodecContext from the stream's codec parameters.

---

{#open-2}

#### open

`virtual` `override`

```cpp
virtual void open() override
```

Defined in src/av/include/icy/av/videodecoder.h:40

Open the codec and initialise any required pixel format conversion context.

---

{#close-7}

#### close

`virtual` `override`

```cpp
virtual void close() override
```

Defined in src/av/include/icy/av/videodecoder.h:43

Close and free the AVCodecContext and associated resources.

---

{#decode-3}

#### decode

`virtual` `nodiscard` `override`

```cpp
[[nodiscard]] virtual bool decode(AVPacket & ipacket) override
```

Defined in src/av/include/icy/av/videodecoder.h:50

Decode the given compressed video packet and emit the decoded frame. Input packets must use the raw AVStream time base; time base conversion to microseconds is performed internally. 
#### Parameters
* `ipacket` The compressed video packet to decode. 

#### Returns
True if an output frame was decoded and emitted, false otherwise.

---

{#flush-5}

#### flush

`virtual` `override`

```cpp
virtual void flush() override
```

Defined in src/av/include/icy/av/videodecoder.h:54

Flush any frames buffered inside the decoder. Call repeatedly after the last packet until false is returned.

{#videoencoder}

## VideoEncoder

```cpp
#include <icy/av/videoencoder.h>
```

```cpp
struct VideoEncoder
```

Defined in src/av/include/icy/av/videoencoder.h:30

> **Inherits:** [`VideoContext`](#videocontext)
> **Subclassed by:** [`VideoPacketEncoder`](#videopacketencoder)

Encodes raw video frames into a compressed format.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `AVFormatContext *` | [`format`](#format-7)  |  |

---

{#format-7}

#### format

```cpp
AVFormatContext * format
```

Defined in src/av/include/icy/av/videoencoder.h:69

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`VideoEncoder`](#videoencoder-1)  | Construct an encoder, optionally tied to an existing muxer context. |
| `void` | [`create`](#create-5) `virtual` `override` | Initialise the AVCodecContext using oparams. Adds a video stream to `format` if one was provided at construction. |
| `void` | [`close`](#close-8) `virtual` `override` | Close and free the AVCodecContext and associated resources. |
| `bool` | [`encode`](#encode-9) `virtual` `nodiscard` `override` | Encode a single video frame. This method is for interleaved video formats. |
| `bool` | [`encode`](#encode-10) `virtual` `nodiscard` `override` | Encode a single video frame. This method is for planar video formats. |
| `bool` | [`encode`](#encode-11) `virtual` `nodiscard` `override` | Encode a single AVFrame (typically from a decoder or converter). |
| `void` | [`flush`](#flush-6) `virtual` `override` | Flush remaining packets to be encoded. This method should be called once before stream closure. |

---

{#videoencoder-1}

#### VideoEncoder

```cpp
VideoEncoder(AVFormatContext * format = nullptr)
```

Defined in src/av/include/icy/av/videoencoder.h:34

Construct an encoder, optionally tied to an existing muxer context. 
#### Parameters
* `format` The AVFormatContext that will receive encoded packets, or nullptr for standalone use.

---

{#create-5}

#### create

`virtual` `override`

```cpp
virtual void create() override
```

Defined in src/av/include/icy/av/videoencoder.h:39

Initialise the AVCodecContext using oparams. Adds a video stream to `format` if one was provided at construction.

---

{#close-8}

#### close

`virtual` `override`

```cpp
virtual void close() override
```

Defined in src/av/include/icy/av/videoencoder.h:42

Close and free the AVCodecContext and associated resources.

---

{#encode-9}

#### encode

`virtual` `nodiscard` `override`

```cpp
[[nodiscard]] virtual bool encode(uint8_t * data, int size, int64_t pts) override
```

Defined in src/av/include/icy/av/videoencoder.h:50

Encode a single video frame. This method is for interleaved video formats.

#### Parameters
* `data` The raw video frame buffer. 

* `size` The buffer size in bytes. 

* `pts` The presentation timestamp in stream time base units.

---

{#encode-10}

#### encode

`virtual` `nodiscard` `override`

```cpp
[[nodiscard]] virtual bool encode(uint8_t * data, int linesize, int64_t pts) override
```

Defined in src/av/include/icy/av/videoencoder.h:58

Encode a single video frame. This method is for planar video formats.

#### Parameters
* `data` Array of per-plane data pointers (up to 4 planes). 

* `linesize` Array of per-plane byte strides. 

* `pts` The presentation timestamp in stream time base units.

---

{#encode-11}

#### encode

`virtual` `nodiscard` `override`

```cpp
[[nodiscard]] virtual bool encode(AVFrame * iframe) override
```

Defined in src/av/include/icy/av/videoencoder.h:63

Encode a single AVFrame (typically from a decoder or converter). 
#### Parameters
* `iframe` The source video frame with all fields set. 

#### Returns
True if an encoded packet was produced and emitted.

---

{#flush-6}

#### flush

`virtual` `override`

```cpp
virtual void flush() override
```

Defined in src/av/include/icy/av/videoencoder.h:67

Flush remaining packets to be encoded. This method should be called once before stream closure.

{#device}

## Device

```cpp
#include <icy/av/devicemanager.h>
```

```cpp
struct Device
```

Defined in src/av/include/icy/av/devicemanager.h:44

Represents a system audio, video or render device.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `Type` | [`type`](#type-4)  |  |
| `std::string` | [`id`](#id-1)  |  |
| `std::string` | [`name`](#name-2)  |  |
| `bool` | [`isDefault`](#isdefault)  |  |
| `bool` | [`isConnected`](#isconnected)  |  |
| `bool` | [`isInUse`](#isinuse)  |  |
| `std::vector< VideoCapability >` | [`videoCapabilities`](#videocapabilities)  |  |
| `std::vector< AudioCapability >` | [`audioCapabilities`](#audiocapabilities)  |  |

---

{#type-4}

#### type

```cpp
Type type {}
```

Defined in src/av/include/icy/av/devicemanager.h:73

---

{#id-1}

#### id

```cpp
std::string id
```

Defined in src/av/include/icy/av/devicemanager.h:74

---

{#name-2}

#### name

```cpp
std::string name
```

Defined in src/av/include/icy/av/devicemanager.h:75

---

{#isdefault}

#### isDefault

```cpp
bool isDefault {false}
```

Defined in src/av/include/icy/av/devicemanager.h:76

---

{#isconnected}

#### isConnected

```cpp
bool isConnected {true}
```

Defined in src/av/include/icy/av/devicemanager.h:77

---

{#isinuse}

#### isInUse

```cpp
bool isInUse {false}
```

Defined in src/av/include/icy/av/devicemanager.h:78

---

{#videocapabilities}

#### videoCapabilities

```cpp
std::vector< VideoCapability > videoCapabilities
```

Defined in src/av/include/icy/av/devicemanager.h:80

---

{#audiocapabilities}

#### audioCapabilities

```cpp
std::vector< AudioCapability > audioCapabilities
```

Defined in src/av/include/icy/av/devicemanager.h:81

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Device`](#device-1)  | Construct a device with Unknown type and empty fields. |
|  | [`Device`](#device-2)  | Construct a device with explicit fields. |
| `void` | [`print`](#print-4) `const` | Print device details (type, id, name, capabilities) to the given stream. |
| `bool` | [`operator==`](#operator-2) `const` `inline` | Equality based on type, id, and name. |
| `VideoCapability` | [`bestVideoCapability`](#bestvideocapability) `const` `inline` | Find the video capability closest to the requested parameters. |
| `AudioCapability` | [`bestAudioCapability`](#bestaudiocapability) `const` `inline` | Find the audio capability closest to the requested parameters. |

---

{#device-1}

#### Device

```cpp
Device()
```

Defined in src/av/include/icy/av/devicemanager.h:84

Construct a device with Unknown type and empty fields.

---

{#device-2}

#### Device

```cpp
Device(Type type, const std::string & id, const std::string & name, bool isDefault = false)
```

Defined in src/av/include/icy/av/devicemanager.h:91

Construct a device with explicit fields. 
#### Parameters
* `type` The device type (VideoInput, AudioInput, etc.). 

* `id` The platform-specific device identifier. 

* `name` The human-readable device name. 

* `isDefault` True if this is the system default device of its type.

---

{#print-4}

#### print

`const`

```cpp
void print(std::ostream & os) const
```

Defined in src/av/include/icy/av/devicemanager.h:95

Print device details (type, id, name, capabilities) to the given stream. 
#### Parameters
* `os` The output stream to write to.

---

{#operator-2}

#### operator==

`const` `inline`

```cpp
inline bool operator==(const Device & that) const
```

Defined in src/av/include/icy/av/devicemanager.h:98

Equality based on type, id, and name.

---

{#bestvideocapability}

#### bestVideoCapability

`const` `inline`

```cpp
inline VideoCapability bestVideoCapability(int width, int height, double fps) const
```

Defined in src/av/include/icy/av/devicemanager.h:112

Find the video capability closest to the requested parameters.

Scoring: resolution match weighted 70%, fps match weighted 30%. Both dimensions are normalised to [0,1] so the weights are meaningful. Prefers capabilities that are >= the requested resolution (upscaling is worse than slight downscaling).

Returns a default capability with the requested values if none are available (allows callers to skip the empty check).

---

{#bestaudiocapability}

#### bestAudioCapability

`const` `inline`

```cpp
inline AudioCapability bestAudioCapability(int sampleRate, int channels) const
```

Defined in src/av/include/icy/av/devicemanager.h:151

Find the audio capability closest to the requested parameters.

Scoring: sample rate match weighted 70%, channel count weighted 30%. Both dimensions are normalised to [0,1].

Returns a default capability with the requested values if none are available.

### Public Types

| Name | Description |
|------|-------------|
| [`Type`](#type-5)  |  |

---

{#type-5}

#### Type

```cpp
enum Type
```

Defined in src/av/include/icy/av/devicemanager.h:46

| Value | Description |
|-------|-------------|
| `Unknown` |  |
| `VideoInput` |  |
| `VideoOutput` |  |
| `AudioInput` |  |
| `AudioOutput` |  |

{#videocapability}

## VideoCapability

```cpp
#include <icy/av/devicemanager.h>
```

```cpp
struct VideoCapability
```

Defined in src/av/include/icy/av/devicemanager.h:56

A video format supported by the device.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `int` | [`width`](#width-1)  |  |
| `int` | [`height`](#height-1)  |  |
| `double` | [`minFps`](#minfps)  |  |
| `double` | [`maxFps`](#maxfps)  |  |
| `std::string` | [`pixelFormat`](#pixelformat)  |  |

---

{#width-1}

#### width

```cpp
int width {0}
```

Defined in src/av/include/icy/av/devicemanager.h:58

---

{#height-1}

#### height

```cpp
int height {0}
```

Defined in src/av/include/icy/av/devicemanager.h:59

---

{#minfps}

#### minFps

```cpp
double minFps {0}
```

Defined in src/av/include/icy/av/devicemanager.h:60

---

{#maxfps}

#### maxFps

```cpp
double maxFps {0}
```

Defined in src/av/include/icy/av/devicemanager.h:61

---

{#pixelformat}

#### pixelFormat

```cpp
std::string pixelFormat
```

Defined in src/av/include/icy/av/devicemanager.h:62

{#audiocapability}

## AudioCapability

```cpp
#include <icy/av/devicemanager.h>
```

```cpp
struct AudioCapability
```

Defined in src/av/include/icy/av/devicemanager.h:66

An audio format supported by the device.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `int` | [`sampleRate`](#samplerate-1)  |  |
| `int` | [`channels`](#channels-1)  |  |
| `std::string` | [`sampleFormat`](#sampleformat)  |  |

---

{#samplerate-1}

#### sampleRate

```cpp
int sampleRate {0}
```

Defined in src/av/include/icy/av/devicemanager.h:68

---

{#channels-1}

#### channels

```cpp
int channels {0}
```

Defined in src/av/include/icy/av/devicemanager.h:69

---

{#sampleformat}

#### sampleFormat

```cpp
std::string sampleFormat
```

Defined in src/av/include/icy/av/devicemanager.h:70

{#audioresampler}

## AudioResampler

```cpp
#include <icy/av/audioresampler.h>
```

```cpp
struct AudioResampler
```

Defined in src/av/include/icy/av/audioresampler.h:39

Converts audio samples between different formats, sample rates, and channel layouts.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `SwrContext *` | [`ctx`](#ctx-2)  | the conversion context |
| `AudioCodec` | [`iparams`](#iparams-2)  | input audio parameters |
| `AudioCodec` | [`oparams`](#oparams-2)  | output audio parameters |
| `uint8_t **` | [`outSamples`](#outsamples)  | the output samples buffer |
| `int` | [`outNumSamples`](#outnumsamples)  | the number of samples currently in the output buffer |
| `int` | [`outBufferSize`](#outbuffersize)  | the number of bytes currently in the buffer |
| `int` | [`maxNumSamples`](#maxnumsamples)  | the maximum number of samples that can be stored in |
| `enum AVSampleFormat` | [`inSampleFmt`](#insamplefmt)  | input sample format |
| `enum AVSampleFormat` | [`outSampleFmt`](#outsamplefmt)  | output sample format |

---

{#ctx-2}

#### ctx

```cpp
SwrContext * ctx
```

Defined in src/av/include/icy/av/audioresampler.h:69

the conversion context

---

{#iparams-2}

#### iparams

```cpp
AudioCodec iparams
```

Defined in src/av/include/icy/av/audioresampler.h:71

input audio parameters

---

{#oparams-2}

#### oparams

```cpp
AudioCodec oparams
```

Defined in src/av/include/icy/av/audioresampler.h:72

output audio parameters

---

{#outsamples}

#### outSamples

```cpp
uint8_t ** outSamples
```

Defined in src/av/include/icy/av/audioresampler.h:73

the output samples buffer

---

{#outnumsamples}

#### outNumSamples

```cpp
int outNumSamples
```

Defined in src/av/include/icy/av/audioresampler.h:74

the number of samples currently in the output buffer

---

{#outbuffersize}

#### outBufferSize

```cpp
int outBufferSize
```

Defined in src/av/include/icy/av/audioresampler.h:75

the number of bytes currently in the buffer

---

{#maxnumsamples}

#### maxNumSamples

```cpp
int maxNumSamples
```

Defined in src/av/include/icy/av/audioresampler.h:76

the maximum number of samples that can be stored in

---

{#insamplefmt}

#### inSampleFmt

```cpp
enum AVSampleFormat inSampleFmt
```

Defined in src/av/include/icy/av/audioresampler.h:77

input sample format

---

{#outsamplefmt}

#### outSampleFmt

```cpp
enum AVSampleFormat outSampleFmt
```

Defined in src/av/include/icy/av/audioresampler.h:78

output sample format

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`AudioResampler`](#audioresampler-1)  |  |
|  | [`AudioResampler`](#audioresampler-2)  | Deleted constructor. |
|  | [`AudioResampler`](#audioresampler-3)  | Deleted constructor. |
| `void` | [`open`](#open-3)  | Initialise the libswresample context using iparams and oparams. Throws std::runtime_error if the context is already open or if required parameters (channels, sample rate, format) are missing. |
| `void` | [`close`](#close-9)  | Free the libswresample context and release the output sample buffer. |
| `int` | [`resample`](#resample)  | Convert the input samples to the output format. NOTE: Input buffers must be contiguous, therefore only interleaved input formats are accepted at this point. |

---

{#audioresampler-1}

#### AudioResampler

```cpp
AudioResampler(const AudioCodec & iparams = AudioCodec(), const AudioCodec & oparams = AudioCodec())
```

Defined in src/av/include/icy/av/audioresampler.h:41

---

{#audioresampler-2}

#### AudioResampler

```cpp
AudioResampler(const AudioResampler &) = delete
```

Defined in src/av/include/icy/av/audioresampler.h:45

Deleted constructor.

---

{#audioresampler-3}

#### AudioResampler

```cpp
AudioResampler(AudioResampler &&) = delete
```

Defined in src/av/include/icy/av/audioresampler.h:47

Deleted constructor.

---

{#open-3}

#### open

```cpp
void open()
```

Defined in src/av/include/icy/av/audioresampler.h:53

Initialise the libswresample context using iparams and oparams. Throws std::runtime_error if the context is already open or if required parameters (channels, sample rate, format) are missing.

---

{#close-9}

#### close

```cpp
void close()
```

Defined in src/av/include/icy/av/audioresampler.h:56

Free the libswresample context and release the output sample buffer.

---

{#resample}

#### resample

```cpp
int resample(uint8_t ** inSamples, int inNumSamples)
```

Defined in src/av/include/icy/av/audioresampler.h:67

Convert the input samples to the output format. NOTE: Input buffers must be contiguous, therefore only interleaved input formats are accepted at this point.

Converted samples are accessible via the `[outSamples](#outsamples)` class member.

#### Parameters
* `inSamples` Pointer to the input sample buffer array. 

* `inNumSamples` The number of input samples per channel. 

#### Returns
The number of converted samples, or zero if samples were internally buffered.

{#videoconverter}

## VideoConverter

```cpp
#include <icy/av/videoconverter.h>
```

```cpp
struct VideoConverter
```

Defined in src/av/include/icy/av/videoconverter.h:32

Converts video frames between pixel formats and resolutions.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `SwsContext *` | [`ctx`](#ctx-3)  | libswscale conversion context. |
| `AVFrame *` | [`oframe`](#oframe)  | Reusable output frame allocated by [create()](#create-6). |
| `VideoCodec` | [`iparams`](#iparams-3)  | Expected input video parameters. |
| `VideoCodec` | [`oparams`](#oparams-3)  | Target output video parameters. |

---

{#ctx-3}

#### ctx

```cpp
SwsContext * ctx
```

Defined in src/av/include/icy/av/videoconverter.h:56

libswscale conversion context.

---

{#oframe}

#### oframe

```cpp
AVFrame * oframe
```

Defined in src/av/include/icy/av/videoconverter.h:57

Reusable output frame allocated by [create()](#create-6).

---

{#iparams-3}

#### iparams

```cpp
VideoCodec iparams
```

Defined in src/av/include/icy/av/videoconverter.h:58

Expected input video parameters.

---

{#oparams-3}

#### oparams

```cpp
VideoCodec oparams
```

Defined in src/av/include/icy/av/videoconverter.h:59

Target output video parameters.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`VideoConverter`](#videoconverter-1)  |  |
|  | [`VideoConverter`](#videoconverter-2)  | Deleted constructor. |
|  | [`VideoConverter`](#videoconverter-3)  | Deleted constructor. |
| `void` | [`create`](#create-6) `virtual` | Initialise the libswscale context and allocate the output frame. Uses iparams and oparams to configure the conversion pipeline. Throws std::runtime_error if already initialised or if parameters are invalid. |
| `void` | [`close`](#close-10) `virtual` | Free the libswscale context and the output frame. |
| `AVFrame *` | [`convert`](#convert-1) `virtual` | Convert `iframe` to the output pixel format and resolution. The returned frame is owned by this converter and is overwritten on the next call. |

---

{#videoconverter-1}

#### VideoConverter

```cpp
VideoConverter()
```

Defined in src/av/include/icy/av/videoconverter.h:34

---

{#videoconverter-2}

#### VideoConverter

```cpp
VideoConverter(const VideoConverter &) = delete
```

Defined in src/av/include/icy/av/videoconverter.h:37

Deleted constructor.

---

{#videoconverter-3}

#### VideoConverter

```cpp
VideoConverter(VideoConverter &&) = delete
```

Defined in src/av/include/icy/av/videoconverter.h:39

Deleted constructor.

---

{#create-6}

#### create

`virtual`

```cpp
virtual void create()
```

Defined in src/av/include/icy/av/videoconverter.h:45

Initialise the libswscale context and allocate the output frame. Uses iparams and oparams to configure the conversion pipeline. Throws std::runtime_error if already initialised or if parameters are invalid.

---

{#close-10}

#### close

`virtual`

```cpp
virtual void close()
```

Defined in src/av/include/icy/av/videoconverter.h:48

Free the libswscale context and the output frame.

---

{#convert-1}

#### convert

`virtual`

```cpp
virtual AVFrame * convert(AVFrame * iframe)
```

Defined in src/av/include/icy/av/videoconverter.h:54

Convert `iframe` to the output pixel format and resolution. The returned frame is owned by this converter and is overwritten on the next call. 
#### Parameters
* `iframe` The source AVFrame; must match iparams dimensions and pixel format. 

#### Returns
The converted output AVFrame.

{#mediapacket}

## MediaPacket

```cpp
#include <icy/av/packet.h>
```

```cpp
struct MediaPacket
```

Defined in src/av/include/icy/av/packet.h:28

> **Inherits:** [`RawPacket`](base.md#rawpacket)
> **Subclassed by:** [`AudioPacket`](#audiopacket), [`VideoPacket`](#videopacket)

Timestamped media packet carrying raw audio or video data.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `int64_t` | [`time`](#time-2)  | Presentation timestamp in microseconds. |

---

{#time-2}

#### time

```cpp
int64_t time
```

Defined in src/av/include/icy/av/packet.h:30

Presentation timestamp in microseconds.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`MediaPacket`](#mediapacket-1) `inline` | Construct with a non-owning or owning mutable buffer. |
|  | [`MediaPacket`](#mediapacket-2) `inline` | Construct with const data (copied, owning). |
|  | [`MediaPacket`](#mediapacket-3) `inline` | Copy constructor. |
| `std::unique_ptr< IPacket >` | [`clone`](#clone) `virtual` `const` `inline` `override` |  |
| `const char *` | [`className`](#classname) `virtual` `const` `inline` `override` | Returns the class name of this packet type for logging and diagnostics. |

---

{#mediapacket-1}

#### MediaPacket

`inline`

```cpp
inline MediaPacket(uint8_t * data = nullptr, size_t size = 0, int64_t time = 0)
```

Defined in src/av/include/icy/av/packet.h:36

Construct with a non-owning or owning mutable buffer. 
#### Parameters
* `data` Pointer to the raw data buffer (may be null). 

* `size` Size of the buffer in bytes. 

* `time` Presentation timestamp in microseconds.

---

{#mediapacket-2}

#### MediaPacket

`inline`

```cpp
inline MediaPacket(const uint8_t * data, size_t size, int64_t time = 0)
```

Defined in src/av/include/icy/av/packet.h:46

Construct with const data (copied, owning). 
#### Parameters
* `data` Pointer to the const raw data buffer (data is copied). 

* `size` Size of the buffer in bytes. 

* `time` Presentation timestamp in microseconds.

---

{#mediapacket-3}

#### MediaPacket

`inline`

```cpp
inline MediaPacket(const MediaPacket & r)
```

Defined in src/av/include/icy/av/packet.h:53

Copy constructor.

---

{#clone}

#### clone

`virtual` `const` `inline` `override`

```cpp
virtual inline std::unique_ptr< IPacket > clone() const override
```

Defined in src/av/include/icy/av/packet.h:62

#### Returns
A heap-allocated copy of this packet.

---

{#classname}

#### className

`virtual` `const` `inline` `override`

```cpp
virtual inline const char * className() const override
```

Defined in src/av/include/icy/av/packet.h:64

Returns the class name of this packet type for logging and diagnostics.

{#videopacket}

## VideoPacket

```cpp
#include <icy/av/packet.h>
```

```cpp
struct VideoPacket
```

Defined in src/av/include/icy/av/packet.h:69

> **Inherits:** [`MediaPacket`](#mediapacket)
> **Subclassed by:** [`PlanarVideoPacket`](#planarvideopacket)

Video packet for interleaved formats.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `int` | [`width`](#width-2)  | Frame width in pixels. |
| `int` | [`height`](#height-2)  | Frame height in pixels. |
| `bool` | [`iframe`](#iframe)  | True if this is an intra (keyframe) frame. |
| `AVPacket *` | [`avpacket`](#avpacket)  | Non-owning pointer to the encoded AVPacket from FFmpeg. Set by [VideoEncoder](#videoencoder), read by [MultiplexEncoder](#multiplexencoder). |

---

{#width-2}

#### width

```cpp
int width
```

Defined in src/av/include/icy/av/packet.h:71

Frame width in pixels.

---

{#height-2}

#### height

```cpp
int height
```

Defined in src/av/include/icy/av/packet.h:72

Frame height in pixels.

---

{#iframe}

#### iframe

```cpp
bool iframe
```

Defined in src/av/include/icy/av/packet.h:73

True if this is an intra (keyframe) frame.

---

{#avpacket}

#### avpacket

```cpp
AVPacket * avpacket = nullptr
```

Defined in src/av/include/icy/av/packet.h:77

Non-owning pointer to the encoded AVPacket from FFmpeg. Set by [VideoEncoder](#videoencoder), read by [MultiplexEncoder](#multiplexencoder).

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`VideoPacket`](#videopacket-1) `inline` | Construct a video packet with an interleaved buffer. |
|  | [`VideoPacket`](#videopacket-2) `inline` | Copy constructor. The avpacket pointer is shallow-copied (non-owning). |
| `std::unique_ptr< IPacket >` | [`clone`](#clone-1) `virtual` `const` `inline` `override` |  |
| `const char *` | [`className`](#classname-1) `virtual` `const` `inline` `override` | Returns the class name of this packet type for logging and diagnostics. |

---

{#videopacket-1}

#### VideoPacket

`inline`

```cpp
inline VideoPacket(uint8_t * data = nullptr, size_t size = 0, int width = 0, int height = 0, int64_t time = 0)
```

Defined in src/av/include/icy/av/packet.h:85

Construct a video packet with an interleaved buffer. 
#### Parameters
* `data` Pointer to the raw frame buffer (may be null). 

* `size` Size of the buffer in bytes. 

* `width` Frame width in pixels. 

* `height` Frame height in pixels. 

* `time` Presentation timestamp in microseconds.

---

{#videopacket-2}

#### VideoPacket

`inline`

```cpp
inline VideoPacket(const VideoPacket & r)
```

Defined in src/av/include/icy/av/packet.h:95

Copy constructor. The avpacket pointer is shallow-copied (non-owning).

---

{#clone-1}

#### clone

`virtual` `const` `inline` `override`

```cpp
virtual inline std::unique_ptr< IPacket > clone() const override
```

Defined in src/av/include/icy/av/packet.h:107

#### Returns
A heap-allocated copy of this packet.

---

{#classname-1}

#### className

`virtual` `const` `inline` `override`

```cpp
virtual inline const char * className() const override
```

Defined in src/av/include/icy/av/packet.h:109

Returns the class name of this packet type for logging and diagnostics.

{#planarvideopacket}

## PlanarVideoPacket

```cpp
#include <icy/av/packet.h>
```

```cpp
struct PlanarVideoPacket
```

Defined in src/av/include/icy/av/packet.h:121

> **Inherits:** [`VideoPacket`](#videopacket)
> **Subclassed by:** [`VisionFramePacket`](vision.md#visionframepacket)

Video packet for planar formats.

#### Parameters
* `data` Array of per-plane data pointers (up to 4 planes). 

* `linesize` Array of per-plane byte strides. 

* `pixelFmt` The pixel format name (e.g. "yuv420p"). 

* `width` The frame width in pixels. 

* `height` The frame height in pixels. 

* `time` The timestamp in microseconds.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `uint8_t *` | [`buffer`](#buffer)  |  |
| `int` | [`linesize`](#linesize)  |  |
| `std::string` | [`pixelFmt`](#pixelfmt-1)  |  |
| `bool` | [`owns_buffer`](#owns_buffer)  |  |
| `AVFrame *` | [`avframe`](#avframe)  | Non-owning pointer to the decoded AVFrame from FFmpeg. Set by [VideoDecoder](#videodecoder). |

---

{#buffer}

#### buffer

```cpp
uint8_t * buffer = {nullptr}
```

Defined in src/av/include/icy/av/packet.h:123

---

{#linesize}

#### linesize

```cpp
int linesize = {0}
```

Defined in src/av/include/icy/av/packet.h:124

---

{#pixelfmt-1}

#### pixelFmt

```cpp
std::string pixelFmt
```

Defined in src/av/include/icy/av/packet.h:125

---

{#owns_buffer}

#### owns_buffer

```cpp
bool owns_buffer = false
```

Defined in src/av/include/icy/av/packet.h:126

---

{#avframe}

#### avframe

```cpp
AVFrame * avframe = nullptr
```

Defined in src/av/include/icy/av/packet.h:130

Non-owning pointer to the decoded AVFrame from FFmpeg. Set by [VideoDecoder](#videodecoder).

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`PlanarVideoPacket`](#planarvideopacket-1)  | Construct a planar video packet, copying the plane pointers (not the pixel data). |
|  | [`PlanarVideoPacket`](#planarvideopacket-2)  | Copy constructor. Performs a deep copy of the owned buffer if owns_buffer is set. |
| `std::unique_ptr< IPacket >` | [`clone`](#clone-2) `virtual` `const` `inline` `override` |  |
| `const char *` | [`className`](#classname-2) `virtual` `const` `inline` `override` | Returns the class name of this packet type for logging and diagnostics. |

---

{#planarvideopacket-1}

#### PlanarVideoPacket

```cpp
PlanarVideoPacket(uint8_t * data, const int linesize, const std::string & pixelFmt = "", int width = 0, int height = 0, int64_t time = 0)
```

Defined in src/av/include/icy/av/packet.h:139

Construct a planar video packet, copying the plane pointers (not the pixel data). 
#### Parameters
* `data` Array of up to 4 per-plane data pointers. 

* `linesize` Array of per-plane byte strides. 

* `pixelFmt` The pixel format string (e.g. "yuv420p"). 

* `width` Frame width in pixels. 

* `height` Frame height in pixels. 

* `time` Presentation timestamp in microseconds.

---

{#planarvideopacket-2}

#### PlanarVideoPacket

```cpp
PlanarVideoPacket(const PlanarVideoPacket & r)
```

Defined in src/av/include/icy/av/packet.h:143

Copy constructor. Performs a deep copy of the owned buffer if owns_buffer is set.

---

{#clone-2}

#### clone

`virtual` `const` `inline` `override`

```cpp
virtual inline std::unique_ptr< IPacket > clone() const override
```

Defined in src/av/include/icy/av/packet.h:147

#### Returns
A heap-allocated deep copy of this packet.

---

{#classname-2}

#### className

`virtual` `const` `inline` `override`

```cpp
virtual inline const char * className() const override
```

Defined in src/av/include/icy/av/packet.h:149

Returns the class name of this packet type for logging and diagnostics.

{#audiopacket}

## AudioPacket

```cpp
#include <icy/av/packet.h>
```

```cpp
struct AudioPacket
```

Defined in src/av/include/icy/av/packet.h:154

> **Inherits:** [`MediaPacket`](#mediapacket)
> **Subclassed by:** [`PlanarAudioPacket`](#planaraudiopacket)

Audio packet for interleaved formats.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `size_t` | [`numSamples`](#numsamples)  | Number of audio samples per channel. |
| `AVPacket *` | [`avpacket`](#avpacket-1)  | Non-owning pointer to the encoded AVPacket from FFmpeg. Set by [AudioEncoder](#audioencoder), read by [MultiplexEncoder](#multiplexencoder). |

---

{#numsamples}

#### numSamples

```cpp
size_t numSamples
```

Defined in src/av/include/icy/av/packet.h:156

Number of audio samples per channel.

---

{#avpacket-1}

#### avpacket

```cpp
AVPacket * avpacket = nullptr
```

Defined in src/av/include/icy/av/packet.h:160

Non-owning pointer to the encoded AVPacket from FFmpeg. Set by [AudioEncoder](#audioencoder), read by [MultiplexEncoder](#multiplexencoder).

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`AudioPacket`](#audiopacket-1) `inline` | Construct an audio packet with an interleaved sample buffer. |
| `std::unique_ptr< IPacket >` | [`clone`](#clone-3) `virtual` `const` `inline` `override` |  |
| `uint8_t *` | [`samples`](#samples) `virtual` `const` `inline` |  |
| `const char *` | [`className`](#classname-3) `virtual` `const` `inline` `override` | Returns the class name of this packet type for logging and diagnostics. |

---

{#audiopacket-1}

#### AudioPacket

`inline`

```cpp
inline AudioPacket(uint8_t * data = nullptr, size_t size = 0, size_t numSamples = 0, int64_t time = 0)
```

Defined in src/av/include/icy/av/packet.h:167

Construct an audio packet with an interleaved sample buffer. 
#### Parameters
* `data` Pointer to the interleaved sample buffer (may be null). 

* `size` Size of the buffer in bytes. 

* `numSamples` Number of samples per channel. 

* `time` Presentation timestamp in microseconds.

---

{#clone-3}

#### clone

`virtual` `const` `inline` `override`

```cpp
virtual inline std::unique_ptr< IPacket > clone() const override
```

Defined in src/av/include/icy/av/packet.h:178

#### Returns
A heap-allocated copy of this packet.

---

{#samples}

#### samples

`virtual` `const` `inline`

```cpp
virtual inline uint8_t * samples() const
```

Defined in src/av/include/icy/av/packet.h:181

#### Returns
A pointer to the raw interleaved sample buffer.

---

{#classname-3}

#### className

`virtual` `const` `inline` `override`

```cpp
virtual inline const char * className() const override
```

Defined in src/av/include/icy/av/packet.h:186

Returns the class name of this packet type for logging and diagnostics.

{#planaraudiopacket}

## PlanarAudioPacket

```cpp
#include <icy/av/packet.h>
```

```cpp
struct PlanarAudioPacket
```

Defined in src/av/include/icy/av/packet.h:197

> **Inherits:** [`AudioPacket`](#audiopacket)

Audio packet for planar formats.

#### Parameters
* `data` Array of per-plane sample buffers (one per channel). 

* `channels` The number of audio channels. 

* `numSamples` The number of samples per channel. 

* `sampleFmt` The sample format name (e.g. "fltp"). 

* `time` The timestamp in microseconds.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `uint8_t *` | [`buffer`](#buffer-1)  |  |
| `int` | [`linesize`](#linesize-1)  |  |
| `int` | [`channels`](#channels-2)  |  |
| `std::string` | [`sampleFmt`](#samplefmt-1)  |  |
| `bool` | [`owns_buffer`](#owns_buffer-1)  |  |

---

{#buffer-1}

#### buffer

```cpp
uint8_t * buffer = {nullptr}
```

Defined in src/av/include/icy/av/packet.h:199

---

{#linesize-1}

#### linesize

```cpp
int linesize = 0
```

Defined in src/av/include/icy/av/packet.h:200

---

{#channels-2}

#### channels

```cpp
int channels = 0
```

Defined in src/av/include/icy/av/packet.h:201

---

{#samplefmt-1}

#### sampleFmt

```cpp
std::string sampleFmt
```

Defined in src/av/include/icy/av/packet.h:202

---

{#owns_buffer-1}

#### owns_buffer

```cpp
bool owns_buffer = false
```

Defined in src/av/include/icy/av/packet.h:203

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`PlanarAudioPacket`](#planaraudiopacket-1)  | Construct a planar audio packet, copying the plane pointers (not the sample data). |
|  | [`PlanarAudioPacket`](#planaraudiopacket-2)  | Copy constructor. Performs a deep copy of the owned buffer if owns_buffer is set. |
| `std::unique_ptr< IPacket >` | [`clone`](#clone-4) `virtual` `const` `inline` `override` |  |
| `const char *` | [`className`](#classname-4) `virtual` `const` `inline` `override` | Returns the class name of this packet type for logging and diagnostics. |

---

{#planaraudiopacket-1}

#### PlanarAudioPacket

```cpp
PlanarAudioPacket(uint8_t * data, int channels = 0, size_t numSamples = 0, const std::string & sampleFmt = "", int64_t time = 0)
```

Defined in src/av/include/icy/av/packet.h:211

Construct a planar audio packet, copying the plane pointers (not the sample data). 
#### Parameters
* `data` Array of up to 4 per-channel sample buffers. 

* `channels` Number of audio channels. 

* `numSamples` Number of samples per channel. 

* `sampleFmt` The sample format string (e.g. "fltp"). 

* `time` Presentation timestamp in microseconds.

---

{#planaraudiopacket-2}

#### PlanarAudioPacket

```cpp
PlanarAudioPacket(const PlanarAudioPacket & r)
```

Defined in src/av/include/icy/av/packet.h:215

Copy constructor. Performs a deep copy of the owned buffer if owns_buffer is set.

---

{#clone-4}

#### clone

`virtual` `const` `inline` `override`

```cpp
virtual inline std::unique_ptr< IPacket > clone() const override
```

Defined in src/av/include/icy/av/packet.h:219

#### Returns
A heap-allocated deep copy of this packet.

---

{#classname-4}

#### className

`virtual` `const` `inline` `override`

```cpp
virtual inline const char * className() const override
```

Defined in src/av/include/icy/av/packet.h:221

Returns the class name of this packet type for logging and diagnostics.

