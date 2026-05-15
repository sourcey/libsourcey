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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`start`](#start-3) | `variable` | Declared here |
| [`end`](#end) | `variable` | Declared here |
| [`frames`](#frames) | `variable` | Declared here |
| [`total`](#total) | `variable` | Declared here |
| [`fps`](#fps-1) | `variable` | Declared here |
| [`FPSCounter`](#fpscounter-1) | `function` | Declared here |
| [`tick`](#tick) | `function` | Declared here |
| [`reset`](#reset) | `function` | Declared here |
| [`started`](#started) | `function` | Declared here |
| [`startFrame`](#startframe) | `function` | Declared here |
| [`endFrame`](#endframe) | `function` | Declared here |

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`LinuxDeviceWatcher`](#linuxdevicewatcher-1) | `function` | Declared here |
| [`start`](#start) | `function` | Declared here |
| [`stop`](#stop) | `function` | Declared here |
| [`_impl`](#_impl) | `variable` | Declared here |
| [`_manager`](#_manager) | `variable` | Declared here |
| [`DeviceWatcher`](#devicewatcher-1) | `function` | Inherited from [`DeviceWatcher`](#devicewatcher) |
| [`start`](#start-5) | `function` | Inherited from [`DeviceWatcher`](#devicewatcher) |
| [`stop`](#stop-4) | `function` | Inherited from [`DeviceWatcher`](#devicewatcher) |

### Inherited from [`DeviceWatcher`](#devicewatcher)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`DeviceWatcher`](#devicewatcher-1) `inline` `explicit` |  |
| `function` | [`start`](#start-5) `virtual` `inline` |  |
| `function` | [`stop`](#stop-4) `virtual` `inline` |  |

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

##### Reimplements

- [`start`](#start-5)

---

{#stop}

#### stop

`virtual` `override`

```cpp
virtual void stop() override
```

Defined in src/av/include/icy/av/linux/v4l2.h:57

Stop monitoring and close the udev monitor.

##### Reimplements

- [`stop`](#stop-4)

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`emitter`](#emitter) | `variable` | Declared here |
| [`ICapture`](#icapture-1) | `function` | Declared here |
| [`start`](#start-1) | `function` | Declared here |
| [`stop`](#stop-1) | `function` | Declared here |
| [`openFile`](#openfile) | `function` | Declared here |
| [`close`](#close) | `function` | Declared here |
| [`getEncoderFormat`](#getencoderformat) | `function` | Declared here |
| [`getEncoderAudioCodec`](#getencoderaudiocodec) | `function` | Declared here |
| [`getEncoderVideoCodec`](#getencodervideocodec) | `function` | Declared here |
| [`openAudio`](#openaudio) | `function` | Declared here |
| [`openVideo`](#openvideo) | `function` | Declared here |
| [`onStreamStateChange`](#onstreamstatechange) | `function` | Declared here |
| [`openStream`](#openstream) | `function` | Declared here |
| [`_emitter`](base.md#classicy_1_1PacketStreamAdapter_1ad954a1cd2f20587a622abd9dcebb56cd) | `variable` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a904abfd9eb913c034a3648dc1fce3f57) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`~PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a48b7da7a145125866e041c3d2cc7396c) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a6762a9434263339d1a477e72de59704c) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a797c0aa60e4999527e8efc3b2fdc88fd) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a4c605d524a538bfcc1cdf30ad4c22ce3) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a10da09b2bf9b5b39c88e97813f99cbee) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`emit`](base.md#classicy_1_1PacketStreamAdapter_1abb0394d938214165fc89e999e6937831) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`getEmitter`](base.md#classicy_1_1PacketStreamAdapter_1a20cfe30f1db9ef46f9db060581fd2ff7) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`retention`](base.md#classicy_1_1PacketStreamAdapter_1a37a1aa11e35a20a7c527efa78e99e928) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`onStreamStateChange`](base.md#classicy_1_1PacketStreamAdapter_1a86f0e2dc118359a83b8cbb996da6dc83) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a5915b4a54729309a0fc12ee9c0107339) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`operator=`](base.md#classicy_1_1PacketStreamAdapter_1aeec4b3367101ffd07e3050cbfd726705) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1abf38bdf926762063adfd29427cad1362) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`operator=`](base.md#classicy_1_1PacketStreamAdapter_1ac7fe6a98e57151ad909715f77e192481) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`start`](base.md#start-7) | `function` | Inherited from [`Startable`](base.md#startable) |
| [`stop`](base.md#stop-6) | `function` | Inherited from [`Startable`](base.md#startable) |

### Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`_emitter`](base.md#classicy_1_1PacketStreamAdapter_1ad954a1cd2f20587a622abd9dcebb56cd)  |  |
| `function` | [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a904abfd9eb913c034a3648dc1fce3f57)  | Construct the adapter, binding it to the given packet signal. |
| `function` | [`~PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a48b7da7a145125866e041c3d2cc7396c) `virtual` `inline` |  |
| `function` | [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a6762a9434263339d1a477e72de59704c) `virtual` | Emit a mutable raw buffer as a packet. |
| `function` | [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a797c0aa60e4999527e8efc3b2fdc88fd) `virtual` | Emit a read-only raw buffer as a packet (data is copied internally). |
| `function` | [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a4c605d524a538bfcc1cdf30ad4c22ce3) `virtual` | Emit a string as a packet (data is copied internally). |
| `function` | [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a10da09b2bf9b5b39c88e97813f99cbee) `virtual` | Emit a flag-only packet carrying no payload data. |
| `function` | [`emit`](base.md#classicy_1_1PacketStreamAdapter_1abb0394d938214165fc89e999e6937831) `virtual` | Emit an existing packet directly onto the outgoing signal. |
| `function` | [`getEmitter`](base.md#classicy_1_1PacketStreamAdapter_1a20cfe30f1db9ef46f9db060581fd2ff7)  | Returns a reference to the outgoing packet signal. |
| `function` | [`retention`](base.md#classicy_1_1PacketStreamAdapter_1a37a1aa11e35a20a7c527efa78e99e928) `virtual` `const` `nodiscard` | Returns how this adapter treats incoming packet lifetime. Most adapters are synchronous and therefore only borrow the packet for the current call chain. Queue-style adapters override this to advertise that they clone before deferred use. Callers may treat the first adapter reporting Cloned or Retained as the explicit ownership boundary in the stream graph. |
| `function` | [`onStreamStateChange`](base.md#classicy_1_1PacketStreamAdapter_1a86f0e2dc118359a83b8cbb996da6dc83) `virtual` `inline` | Called by the [PacketStream](base.md#packetstream) to notify when the internal [Stream](base.md#stream) state changes. On receiving the Stopped state, it is the responsibility of the adapter to have ceased all outgoing packet transmission, especially in multi-thread scenarios. |
| `function` | [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a5915b4a54729309a0fc12ee9c0107339)  | NonCopyable and NonMovable. |
| `function` | [`operator=`](base.md#classicy_1_1PacketStreamAdapter_1aeec4b3367101ffd07e3050cbfd726705)  | Deleted assignment operator. |
| `function` | [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1abf38bdf926762063adfd29427cad1362)  | Deleted constructor. |
| `function` | [`operator=`](base.md#classicy_1_1PacketStreamAdapter_1ac7fe6a98e57151ad909715f77e192481)  | Deleted assignment operator. |

### Inherited from [`Startable`](base.md#startable)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`start`](base.md#start-7) `virtual` | Starts the object (e.g. begins processing or listening). |
| `function` | [`stop`](base.md#stop-6) `virtual` | Stops the object (e.g. halts processing or closes resources). |

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

##### Reimplements

- [`start`](base.md#start-7)

##### Reimplemented by

- [`start`](#start-4)

---

{#stop-1}

#### stop

`virtual` `override`

```cpp
virtual void stop() override
```

Defined in src/av/include/icy/av/icapture.h:45

Stop capturing and release device resources.

##### Reimplements

- [`stop`](base.md#stop-6)

##### Reimplemented by

- [`stop`](#stop-3)

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

##### Reimplemented by

- [`openFile`](#openfile-1)

---

{#close}

#### close

`virtual` `inline`

```cpp
virtual inline void close()
```

Defined in src/av/include/icy/av/icapture.h:52

Release the capture source and any associated resources.

##### Reimplemented by

- [`close`](#close-5)

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

##### Reimplemented by

- [`getEncoderFormat`](#getencoderformat-1)

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

##### Reimplemented by

- [`getEncoderAudioCodec`](#getencoderaudiocodec-1)

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

##### Reimplemented by

- [`getEncoderVideoCodec`](#getencodervideocodec-1)

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

##### Reimplemented by

- [`openAudio`](#openaudio-1)

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

##### Reimplemented by

- [`openVideo`](#openvideo-1)

---

{#onstreamstatechange}

#### onStreamStateChange

`virtual` `inline` `override`

```cpp
virtual inline void onStreamStateChange(const PacketStreamState & state) override
```

Defined in src/av/include/icy/av/icapture.h:77

React to [PacketStream](base.md#packetstream) state transitions by starting or stopping capture.

##### Reimplements

- [`onStreamStateChange`](base.md#classicy_1_1PacketStreamAdapter_1a86f0e2dc118359a83b8cbb996da6dc83)

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

##### Reimplemented by

- [`openStream`](#openstream-1)

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`init`](#init) | `function` | Declared here |
| [`uninit`](#uninit) | `function` | Declared here |
| [`cleanup`](#cleanup) | `function` | Declared here |
| [`options`](#options-1) | `function` | Declared here |
| [`createVideo`](#createvideo) | `function` | Declared here |
| [`freeVideo`](#freevideo) | `function` | Declared here |
| [`encodeVideo`](#encodevideo) | `function` | Declared here |
| [`createAudio`](#createaudio) | `function` | Declared here |
| [`freeAudio`](#freeaudio) | `function` | Declared here |
| [`flush`](#flush) | `function` | Declared here |
| [`isNone`](#isnone) | `function` | Declared here |
| [`isReady`](#isready) | `function` | Declared here |
| [`isEncoding`](#isencoding) | `function` | Declared here |
| [`isActive`](#isactive) | `function` | Declared here |
| [`isStopped`](#isstopped) | `function` | Declared here |
| [`isError`](#iserror) | `function` | Declared here |
| [`Type`](#type-3) | `enum` | Declared here |
| [`StateChange`](base.md#classicy_1_1Stateful_1adba38e2b8a737448d48fc56b4bbe1dc0) | `variable` | Inherited from [`Stateful`](base.md#stateful) |
| [`_state`](base.md#classicy_1_1Stateful_1a45326923d41e72e4afc70d9ed8acc15d) | `variable` | Inherited from [`Stateful`](base.md#stateful) |
| [`Stateful`](base.md#classicy_1_1Stateful_1a444344bc5f8563af4d289be6beb693a0) | `function` | Inherited from [`Stateful`](base.md#stateful) |
| [`~Stateful`](base.md#classicy_1_1Stateful_1a70818a671339a0468ef71e6d54c97c77) | `function` | Inherited from [`Stateful`](base.md#stateful) |
| [`stateEquals`](base.md#classicy_1_1Stateful_1a8c8bd62dbd4251749150ed7f95bdb875) | `function` | Inherited from [`Stateful`](base.md#stateful) |
| [`stateBetween`](base.md#classicy_1_1Stateful_1a702bf6338c45b6ce75540a2d7f03a18f) | `function` | Inherited from [`Stateful`](base.md#stateful) |
| [`state`](base.md#classicy_1_1Stateful_1ae1082bd323b27b8eb1022f160d77a6e6) | `function` | Inherited from [`Stateful`](base.md#stateful) |
| [`state`](base.md#classicy_1_1Stateful_1ab63e2709491c8fb841f7dd5720c15020) | `function` | Inherited from [`Stateful`](base.md#stateful) |
| [`beforeStateChange`](base.md#classicy_1_1Stateful_1a0ae9d2650c9a6fdb276b88e0b2d3ca34) | `function` | Inherited from [`Stateful`](base.md#stateful) |
| [`onStateChange`](base.md#classicy_1_1Stateful_1a06e5572c313c3b54db932c922166079d) | `function` | Inherited from [`Stateful`](base.md#stateful) |
| [`setState`](base.md#classicy_1_1Stateful_1a88cfa69863f505b8232c7cce5989a395) | `function` | Inherited from [`Stateful`](base.md#stateful) |
| [`setState`](base.md#classicy_1_1Stateful_1a9d5c3013be4fb7508266ed0b950694aa) | `function` | Inherited from [`Stateful`](base.md#stateful) |

### Inherited from [`Stateful`](base.md#stateful)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`StateChange`](base.md#classicy_1_1Stateful_1adba38e2b8a737448d48fc56b4bbe1dc0)  | Signals when the state changes. |
| `variable` | [`_state`](base.md#classicy_1_1Stateful_1a45326923d41e72e4afc70d9ed8acc15d)  |  |
| `function` | [`Stateful`](base.md#classicy_1_1Stateful_1a444344bc5f8563af4d289be6beb693a0) `inline` |  |
| `function` | [`~Stateful`](base.md#classicy_1_1Stateful_1a70818a671339a0468ef71e6d54c97c77) `virtual` `inline` |  |
| `function` | [`stateEquals`](base.md#classicy_1_1Stateful_1a8c8bd62dbd4251749150ed7f95bdb875) `virtual` `const` `inline` | Returns true if the current state ID equals the given ID. |
| `function` | [`stateBetween`](base.md#classicy_1_1Stateful_1a702bf6338c45b6ce75540a2d7f03a18f) `virtual` `const` `inline` | Returns true if the current state ID is in the inclusive range [lid, rid]. |
| `function` | [`state`](base.md#classicy_1_1Stateful_1ae1082bd323b27b8eb1022f160d77a6e6) `virtual` `inline` | Returns a mutable reference to the current state. |
| `function` | [`state`](base.md#classicy_1_1Stateful_1ab63e2709491c8fb841f7dd5720c15020) `virtual` `const` `inline` | Returns a copy of the current state. |
| `function` | [`beforeStateChange`](base.md#classicy_1_1Stateful_1a0ae9d2650c9a6fdb276b88e0b2d3ca34) `virtual` `inline` | Override to handle pre state change logic. Return false to prevent state change. |
| `function` | [`onStateChange`](base.md#classicy_1_1Stateful_1a06e5572c313c3b54db932c922166079d) `virtual` `inline` | Override to handle post state change logic. |
| `function` | [`setState`](base.md#classicy_1_1Stateful_1a88cfa69863f505b8232c7cce5989a395) `virtual` `inline` | Sets the state and sends the state signal if the state change was successful. |
| `function` | [`setState`](base.md#classicy_1_1Stateful_1a9d5c3013be4fb7508266ed0b950694aa) `virtual` `inline` | Sets the state and sends the state signal if the state change was successful. |

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

##### Reimplemented by

- [`init`](#init-1)

---

{#uninit}

#### uninit

`virtual`

```cpp
virtual void uninit()
```

Defined in src/av/include/icy/av/iencoder.h:96

Flush remaining packets, close codec contexts, and release all encoder resources.

##### Reimplemented by

- [`uninit`](#uninit-1)

---

{#cleanup}

#### cleanup

`virtual` `inline`

```cpp
virtual inline void cleanup()
```

Defined in src/av/include/icy/av/iencoder.h:99

Perform any additional cleanup after [uninit()](#uninit). Default is a no-op.

##### Reimplemented by

- [`cleanup`](#cleanup-1)

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

##### Reimplemented by

- [`options`](#options-2)

---

{#createvideo}

#### createVideo

`virtual` `inline`

```cpp
virtual inline void createVideo()
```

Defined in src/av/include/icy/av/iencoder.h:105

Initialise the video codec context and stream.

##### Reimplemented by

- [`createVideo`](#createvideo-1)

---

{#freevideo}

#### freeVideo

`virtual` `inline`

```cpp
virtual inline void freeVideo()
```

Defined in src/av/include/icy/av/iencoder.h:108

Free the video codec context and stream.

##### Reimplemented by

- [`freeVideo`](#freevideo-1)

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

##### Reimplemented by

- [`encodeVideo`](#encodevideo-1)

---

{#createaudio}

#### createAudio

`virtual` `inline`

```cpp
virtual inline void createAudio()
```

Defined in src/av/include/icy/av/iencoder.h:116

Initialise the audio codec context and stream.

##### Reimplemented by

- [`createAudio`](#createaudio-1)

---

{#freeaudio}

#### freeAudio

`virtual` `inline`

```cpp
virtual inline void freeAudio()
```

Defined in src/av/include/icy/av/iencoder.h:119

Free the audio codec context and stream.

##### Reimplemented by

- [`freeAudio`](#freeaudio-1)

---

{#flush}

#### flush

`virtual` `inline`

```cpp
virtual inline void flush()
```

Defined in src/av/include/icy/av/iencoder.h:122

Flush any internally buffered packets to the output.

##### Reimplemented by

- [`flush`](#flush-7)

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`AppleDeviceWatcher`](#appledevicewatcher-1) | `function` | Declared here |
| [`start`](#start-2) | `function` | Declared here |
| [`stop`](#stop-2) | `function` | Declared here |
| [`_impl`](#_impl-1) | `variable` | Declared here |
| [`_manager`](#_manager-1) | `variable` | Declared here |
| [`DeviceWatcher`](#devicewatcher-1) | `function` | Inherited from [`DeviceWatcher`](#devicewatcher) |
| [`start`](#start-5) | `function` | Inherited from [`DeviceWatcher`](#devicewatcher) |
| [`stop`](#stop-4) | `function` | Inherited from [`DeviceWatcher`](#devicewatcher) |

### Inherited from [`DeviceWatcher`](#devicewatcher)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`DeviceWatcher`](#devicewatcher-1) `inline` `explicit` |  |
| `function` | [`start`](#start-5) `virtual` `inline` |  |
| `function` | [`stop`](#stop-4) `virtual` `inline` |  |

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

##### Reimplements

- [`start`](#start-5)

---

{#stop-2}

#### stop

`virtual` `override`

```cpp
virtual void stop() override
```

Defined in src/av/include/icy/av/apple/coreaudio.h:58

Stop monitoring and release all notification observers.

##### Reimplements

- [`stop`](#stop-4)

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`fps`](#fps-2) | `variable` | Declared here |
| [`frames`](#frames-1) | `variable` | Declared here |
| [`FPSCounter`](#fpscounter-3) | `function` | Declared here |
| [`reset`](#reset-1) | `function` | Declared here |
| [`tick`](#tick-1) | `function` | Declared here |
| [`tickIndex`](#tickindex) | `variable` | Declared here |
| [`prevTick`](#prevtick) | `variable` | Declared here |
| [`tickSum`](#ticksum) | `variable` | Declared here |
| [`tickList`](#ticklist) | `variable` | Declared here |
| [`updateAvg`](#updateavg) | `function` | Declared here |
| [`MAX`](#max) | `variable` | Declared here |

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`emitter`](#emitter-1) | `variable` | Declared here |
| [`FPSLimiter`](#fpslimiter-1) | `function` | Declared here |
| [`process`](#process-1) | `function` | Declared here |
| [`onStreamStateChange`](#onstreamstatechange-1) | `function` | Declared here |
| [`_max`](#_max) | `variable` | Declared here |
| [`_videoOnly`](#_videoonly) | `variable` | Declared here |
| [`_counter`](#_counter) | `variable` | Declared here |
| [`PacketProcessor`](base.md#classicy_1_1PacketProcessor_1aa9a61f3eb4c53e7673b29d4c97686d9a) | `function` | Inherited from [`PacketProcessor`](base.md#packetprocessor) |
| [`process`](base.md#classicy_1_1PacketProcessor_1acd9fb1a5fa525c0a2d96971fe6c06294) | `function` | Inherited from [`PacketProcessor`](base.md#packetprocessor) |
| [`accepts`](base.md#classicy_1_1PacketProcessor_1a838d9d75f81bd943b40efcbe683394b7) | `function` | Inherited from [`PacketProcessor`](base.md#packetprocessor) |
| [`operator<<`](base.md#classicy_1_1PacketProcessor_1ae8908612aea63a6e5b3d3f7714a8e7a3) | `function` | Inherited from [`PacketProcessor`](base.md#packetprocessor) |
| [`_emitter`](base.md#classicy_1_1PacketStreamAdapter_1ad954a1cd2f20587a622abd9dcebb56cd) | `variable` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a904abfd9eb913c034a3648dc1fce3f57) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`~PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a48b7da7a145125866e041c3d2cc7396c) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a6762a9434263339d1a477e72de59704c) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a797c0aa60e4999527e8efc3b2fdc88fd) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a4c605d524a538bfcc1cdf30ad4c22ce3) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a10da09b2bf9b5b39c88e97813f99cbee) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`emit`](base.md#classicy_1_1PacketStreamAdapter_1abb0394d938214165fc89e999e6937831) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`getEmitter`](base.md#classicy_1_1PacketStreamAdapter_1a20cfe30f1db9ef46f9db060581fd2ff7) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`retention`](base.md#classicy_1_1PacketStreamAdapter_1a37a1aa11e35a20a7c527efa78e99e928) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`onStreamStateChange`](base.md#classicy_1_1PacketStreamAdapter_1a86f0e2dc118359a83b8cbb996da6dc83) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a5915b4a54729309a0fc12ee9c0107339) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`operator=`](base.md#classicy_1_1PacketStreamAdapter_1aeec4b3367101ffd07e3050cbfd726705) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1abf38bdf926762063adfd29427cad1362) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`operator=`](base.md#classicy_1_1PacketStreamAdapter_1ac7fe6a98e57151ad909715f77e192481) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |

### Inherited from [`PacketProcessor`](base.md#packetprocessor)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`PacketProcessor`](base.md#classicy_1_1PacketProcessor_1aa9a61f3eb4c53e7673b29d4c97686d9a) `inline` |  |
| `function` | [`process`](base.md#classicy_1_1PacketProcessor_1acd9fb1a5fa525c0a2d96971fe6c06294) `virtual` | This method performs processing on the given packet and emits the result. |
| `function` | [`accepts`](base.md#classicy_1_1PacketProcessor_1a838d9d75f81bd943b40efcbe683394b7) `virtual` `inline` | This method ensures compatibility with the given packet type. Return false to reject the packet. |
| `function` | [`operator<<`](base.md#classicy_1_1PacketProcessor_1ae8908612aea63a6e5b3d3f7714a8e7a3) `virtual` `inline` | [Stream](base.md#stream) operator alias for [process()](base.md#classicy_1_1PacketProcessor_1acd9fb1a5fa525c0a2d96971fe6c06294). |

### Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`_emitter`](base.md#classicy_1_1PacketStreamAdapter_1ad954a1cd2f20587a622abd9dcebb56cd)  |  |
| `function` | [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a904abfd9eb913c034a3648dc1fce3f57)  | Construct the adapter, binding it to the given packet signal. |
| `function` | [`~PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a48b7da7a145125866e041c3d2cc7396c) `virtual` `inline` |  |
| `function` | [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a6762a9434263339d1a477e72de59704c) `virtual` | Emit a mutable raw buffer as a packet. |
| `function` | [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a797c0aa60e4999527e8efc3b2fdc88fd) `virtual` | Emit a read-only raw buffer as a packet (data is copied internally). |
| `function` | [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a4c605d524a538bfcc1cdf30ad4c22ce3) `virtual` | Emit a string as a packet (data is copied internally). |
| `function` | [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a10da09b2bf9b5b39c88e97813f99cbee) `virtual` | Emit a flag-only packet carrying no payload data. |
| `function` | [`emit`](base.md#classicy_1_1PacketStreamAdapter_1abb0394d938214165fc89e999e6937831) `virtual` | Emit an existing packet directly onto the outgoing signal. |
| `function` | [`getEmitter`](base.md#classicy_1_1PacketStreamAdapter_1a20cfe30f1db9ef46f9db060581fd2ff7)  | Returns a reference to the outgoing packet signal. |
| `function` | [`retention`](base.md#classicy_1_1PacketStreamAdapter_1a37a1aa11e35a20a7c527efa78e99e928) `virtual` `const` `nodiscard` | Returns how this adapter treats incoming packet lifetime. Most adapters are synchronous and therefore only borrow the packet for the current call chain. Queue-style adapters override this to advertise that they clone before deferred use. Callers may treat the first adapter reporting Cloned or Retained as the explicit ownership boundary in the stream graph. |
| `function` | [`onStreamStateChange`](base.md#classicy_1_1PacketStreamAdapter_1a86f0e2dc118359a83b8cbb996da6dc83) `virtual` `inline` | Called by the [PacketStream](base.md#packetstream) to notify when the internal [Stream](base.md#stream) state changes. On receiving the Stopped state, it is the responsibility of the adapter to have ceased all outgoing packet transmission, especially in multi-thread scenarios. |
| `function` | [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a5915b4a54729309a0fc12ee9c0107339)  | NonCopyable and NonMovable. |
| `function` | [`operator=`](base.md#classicy_1_1PacketStreamAdapter_1aeec4b3367101ffd07e3050cbfd726705)  | Deleted assignment operator. |
| `function` | [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1abf38bdf926762063adfd29427cad1362)  | Deleted constructor. |
| `function` | [`operator=`](base.md#classicy_1_1PacketStreamAdapter_1ac7fe6a98e57151ad909715f77e192481)  | Deleted assignment operator. |

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

##### Reimplements

- [`process`](base.md#classicy_1_1PacketProcessor_1acd9fb1a5fa525c0a2d96971fe6c06294)

---

{#onstreamstatechange-1}

#### onStreamStateChange

`virtual` `inline`

```cpp
virtual inline void onStreamStateChange(const PacketStreamState &)
```

Defined in src/av/include/icy/av/fpscounter.h:187

Reset the FPS counter when the stream state changes.

##### Reimplements

- [`onStreamStateChange`](base.md#classicy_1_1PacketStreamAdapter_1a86f0e2dc118359a83b8cbb996da6dc83)

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`AudioCapture`](#audiocapture-1) | `function` | Declared here |
| [`AudioCapture`](#audiocapture-2) | `function` | Declared here |
| [`AudioCapture`](#audiocapture-3) | `function` | Declared here |
| [`openAudio`](#openaudio-1) | `function` | Declared here |
| [`openAudio`](#openaudio-2) | `function` | Declared here |
| [`Ptr`](#ptr) | `typedef` | Declared here |
| [`Closing`](#closing) | `variable` | Inherited from [`MediaCapture`](#mediacapture) |
| [`MediaCapture`](#mediacapture-1) | `function` | Inherited from [`MediaCapture`](#mediacapture) |
| [`MediaCapture`](#mediacapture-2) | `function` | Inherited from [`MediaCapture`](#mediacapture) |
| [`MediaCapture`](#mediacapture-3) | `function` | Inherited from [`MediaCapture`](#mediacapture) |
| [`openFile`](#openfile-1) | `function` | Inherited from [`MediaCapture`](#mediacapture) |
| [`close`](#close-5) | `function` | Inherited from [`MediaCapture`](#mediacapture) |
| [`start`](#start-4) | `function` | Inherited from [`MediaCapture`](#mediacapture) |
| [`stop`](#stop-3) | `function` | Inherited from [`MediaCapture`](#mediacapture) |
| [`run`](#run) | `function` | Inherited from [`MediaCapture`](#mediacapture) |
| [`getEncoderFormat`](#getencoderformat-1) | `function` | Inherited from [`MediaCapture`](#mediacapture) |
| [`getEncoderAudioCodec`](#getencoderaudiocodec-1) | `function` | Inherited from [`MediaCapture`](#mediacapture) |
| [`getEncoderVideoCodec`](#getencodervideocodec-1) | `function` | Inherited from [`MediaCapture`](#mediacapture) |
| [`setLoopInput`](#setloopinput) | `function` | Inherited from [`MediaCapture`](#mediacapture) |
| [`setLimitFramerate`](#setlimitframerate) | `function` | Inherited from [`MediaCapture`](#mediacapture) |
| [`setRealtimePTS`](#setrealtimepts) | `function` | Inherited from [`MediaCapture`](#mediacapture) |
| [`setOpenOptions`](#setopenoptions) | `function` | Inherited from [`MediaCapture`](#mediacapture) |
| [`setPassthroughVideo`](#setpassthroughvideo) | `function` | Inherited from [`MediaCapture`](#mediacapture) |
| [`formatCtx`](#formatctx) | `function` | Inherited from [`MediaCapture`](#mediacapture) |
| [`video`](#video-1) | `function` | Inherited from [`MediaCapture`](#mediacapture) |
| [`audio`](#audio-1) | `function` | Inherited from [`MediaCapture`](#mediacapture) |
| [`stopping`](#stopping) | `function` | Inherited from [`MediaCapture`](#mediacapture) |
| [`error`](#error-2) | `function` | Inherited from [`MediaCapture`](#mediacapture) |
| [`_mutex`](#_mutex) | `variable` | Inherited from [`MediaCapture`](#mediacapture) |
| [`_thread`](#_thread) | `variable` | Inherited from [`MediaCapture`](#mediacapture) |
| [`_formatCtx`](#_formatctx) | `variable` | Inherited from [`MediaCapture`](#mediacapture) |
| [`_video`](#_video) | `variable` | Inherited from [`MediaCapture`](#mediacapture) |
| [`_audio`](#_audio) | `variable` | Inherited from [`MediaCapture`](#mediacapture) |
| [`_error`](#_error) | `variable` | Inherited from [`MediaCapture`](#mediacapture) |
| [`_stopping`](#_stopping) | `variable` | Inherited from [`MediaCapture`](#mediacapture) |
| [`_looping`](#_looping) | `variable` | Inherited from [`MediaCapture`](#mediacapture) |
| [`_realtime`](#_realtime) | `variable` | Inherited from [`MediaCapture`](#mediacapture) |
| [`_ratelimit`](#_ratelimit) | `variable` | Inherited from [`MediaCapture`](#mediacapture) |
| [`_passthroughVideo`](#_passthroughvideo) | `variable` | Inherited from [`MediaCapture`](#mediacapture) |
| [`_openOptions`](#_openoptions) | `variable` | Inherited from [`MediaCapture`](#mediacapture) |
| [`_videoStream`](#_videostream) | `variable` | Inherited from [`MediaCapture`](#mediacapture) |
| [`openStream`](#openstream-1) | `function` | Inherited from [`MediaCapture`](#mediacapture) |
| [`emit`](#emit) | `function` | Inherited from [`MediaCapture`](#mediacapture) |
| [`Ptr`](#ptr-1) | `typedef` | Inherited from [`MediaCapture`](#mediacapture) |
| [`emitter`](#emitter) | `variable` | Inherited from [`ICapture`](#icapture) |
| [`ICapture`](#icapture-1) | `function` | Inherited from [`ICapture`](#icapture) |
| [`start`](#start-1) | `function` | Inherited from [`ICapture`](#icapture) |
| [`stop`](#stop-1) | `function` | Inherited from [`ICapture`](#icapture) |
| [`openFile`](#openfile) | `function` | Inherited from [`ICapture`](#icapture) |
| [`close`](#close) | `function` | Inherited from [`ICapture`](#icapture) |
| [`getEncoderFormat`](#getencoderformat) | `function` | Inherited from [`ICapture`](#icapture) |
| [`getEncoderAudioCodec`](#getencoderaudiocodec) | `function` | Inherited from [`ICapture`](#icapture) |
| [`getEncoderVideoCodec`](#getencodervideocodec) | `function` | Inherited from [`ICapture`](#icapture) |
| [`openAudio`](#openaudio) | `function` | Inherited from [`ICapture`](#icapture) |
| [`openVideo`](#openvideo) | `function` | Inherited from [`ICapture`](#icapture) |
| [`onStreamStateChange`](#onstreamstatechange) | `function` | Inherited from [`ICapture`](#icapture) |
| [`openStream`](#openstream) | `function` | Inherited from [`ICapture`](#icapture) |
| [`_emitter`](base.md#classicy_1_1PacketStreamAdapter_1ad954a1cd2f20587a622abd9dcebb56cd) | `variable` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a904abfd9eb913c034a3648dc1fce3f57) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`~PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a48b7da7a145125866e041c3d2cc7396c) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a6762a9434263339d1a477e72de59704c) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a797c0aa60e4999527e8efc3b2fdc88fd) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a4c605d524a538bfcc1cdf30ad4c22ce3) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a10da09b2bf9b5b39c88e97813f99cbee) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`emit`](base.md#classicy_1_1PacketStreamAdapter_1abb0394d938214165fc89e999e6937831) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`getEmitter`](base.md#classicy_1_1PacketStreamAdapter_1a20cfe30f1db9ef46f9db060581fd2ff7) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`retention`](base.md#classicy_1_1PacketStreamAdapter_1a37a1aa11e35a20a7c527efa78e99e928) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`onStreamStateChange`](base.md#classicy_1_1PacketStreamAdapter_1a86f0e2dc118359a83b8cbb996da6dc83) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a5915b4a54729309a0fc12ee9c0107339) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`operator=`](base.md#classicy_1_1PacketStreamAdapter_1aeec4b3367101ffd07e3050cbfd726705) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1abf38bdf926762063adfd29427cad1362) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`operator=`](base.md#classicy_1_1PacketStreamAdapter_1ac7fe6a98e57151ad909715f77e192481) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`start`](base.md#start-7) | `function` | Inherited from [`Startable`](base.md#startable) |
| [`stop`](base.md#stop-6) | `function` | Inherited from [`Startable`](base.md#startable) |
| [`Runnable`](base.md#runnable-1) | `function` | Inherited from [`Runnable`](base.md#runnable) |
| [`run`](base.md#run-4) | `function` | Inherited from [`Runnable`](base.md#runnable) |
| [`cancel`](base.md#cancel-1) | `function` | Inherited from [`Runnable`](base.md#runnable) |
| [`cancelled`](base.md#cancelled) | `function` | Inherited from [`Runnable`](base.md#runnable) |
| [`exit`](base.md#exit) | `variable` | Inherited from [`Runnable`](base.md#runnable) |

### Inherited from [`MediaCapture`](#mediacapture)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`Closing`](#closing)  | Signals that the capture thread is closing. This signal is emitted from the capture thread context. |
| `function` | [`MediaCapture`](#mediacapture-1)  |  |
| `function` | [`MediaCapture`](#mediacapture-2)  | Deleted constructor. |
| `function` | [`MediaCapture`](#mediacapture-3)  | Deleted constructor. |
| `function` | [`openFile`](#openfile-1) `virtual` `override` | Open a media file for decoding. Automatically detects video and audio streams. |
| `function` | [`close`](#close-5) `virtual` `override` | Stop the capture thread and close the media stream and all decoders. |
| `function` | [`start`](#start-4) `virtual` `override` | Start the background capture and decode thread. Throws std::runtime_error if no media streams have been opened. |
| `function` | [`stop`](#stop-3) `virtual` `override` | [Signal](base.md#signal) the capture thread to stop and join it before returning. |
| `function` | [`run`](#run) `virtual` `override` | Entry point for the background capture thread. Reads and decodes packets from the format context until EOF or [stop()](#stop-3) is called. |
| `function` | [`getEncoderFormat`](#getencoderformat-1) `virtual` `override` | Fill `format` with the combined encoder-ready video and audio codec parameters. |
| `function` | [`getEncoderAudioCodec`](#getencoderaudiocodec-1) `virtual` `override` | Fill `params` with the decoder's output audio codec parameters. Throws std::runtime_error if audio parameters have not been initialised. |
| `function` | [`getEncoderVideoCodec`](#getencodervideocodec-1) `virtual` `override` | Fill `params` with the decoder's output video codec parameters. Throws std::runtime_error if video parameters have not been initialised. |
| `function` | [`setLoopInput`](#setloopinput)  | Continuously loop the input file when set. |
| `function` | [`setLimitFramerate`](#setlimitframerate)  | Limit playback to video FPS. |
| `function` | [`setRealtimePTS`](#setrealtimepts)  | Set to use realtime PTS calculation. This is preferred when using live captures as FFmpeg-provided values are not always reliable. |
| `function` | [`setOpenOptions`](#setopenoptions)  | Set demuxer options applied at the next [openFile()](#openfile-1) call. Keys map to libavformat AVOption entries (e.g. "rtsp_transport", "fflags", "analyzeduration", "probesize"). Useful for live network sources that need low-latency hints. Pass an empty map to clear. |
| `function` | [`setPassthroughVideo`](#setpassthroughvideo)  | Skip the video decoder. Encoded video AVPackets from the input stream are emitted directly as `[av::VideoPacket](#videopacket)` without decoding. The emitted packets carry the source's encoded payload and a `time` in microseconds rescaled from the stream's timebase. The `iframe` flag is set from `AV_PKT_FLAG_KEY`. |
| `function` | [`formatCtx`](#formatctx) `const` |  |
| `function` | [`video`](#video-1) `const` |  |
| `function` | [`audio`](#audio-1) `const` |  |
| `function` | [`stopping`](#stopping) `const` |  |
| `function` | [`error`](#error-2) `const` |  |
| `variable` | [`_mutex`](#_mutex)  |  |
| `variable` | [`_thread`](#_thread)  |  |
| `variable` | [`_formatCtx`](#_formatctx)  |  |
| `variable` | [`_video`](#_video)  |  |
| `variable` | [`_audio`](#_audio)  |  |
| `variable` | [`_error`](#_error)  |  |
| `variable` | [`_stopping`](#_stopping)  |  |
| `variable` | [`_looping`](#_looping)  |  |
| `variable` | [`_realtime`](#_realtime)  |  |
| `variable` | [`_ratelimit`](#_ratelimit)  |  |
| `variable` | [`_passthroughVideo`](#_passthroughvideo)  |  |
| `variable` | [`_openOptions`](#_openoptions)  |  |
| `variable` | [`_videoStream`](#_videostream)  |  |
| `function` | [`openStream`](#openstream-1) `virtual` `override` | Open the underlying media stream. |
| `function` | [`emit`](#emit) `virtual` `override` | Emit an existing packet directly onto the outgoing signal. |
| `typedef` | [`Ptr`](#ptr-1)  |  |

### Inherited from [`ICapture`](#icapture)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`emitter`](#emitter)  |  |
| `function` | [`ICapture`](#icapture-1) `inline` |  |
| `function` | [`start`](#start-1) `virtual` `override` | Start capturing and emitting packets. |
| `function` | [`stop`](#stop-1) `virtual` `override` | Stop capturing and release device resources. |
| `function` | [`openFile`](#openfile) `virtual` `inline` | Open a media file as the capture source. |
| `function` | [`close`](#close) `virtual` `inline` | Release the capture source and any associated resources. |
| `function` | [`getEncoderFormat`](#getencoderformat) `virtual` | Populate `iformat` with the encoder-ready format derived from this capture source. |
| `function` | [`getEncoderAudioCodec`](#getencoderaudiocodec) `virtual` `inline` | Populate `params` with the encoder-ready audio codec parameters. |
| `function` | [`getEncoderVideoCodec`](#getencodervideocodec) `virtual` `inline` | Populate `params` with the encoder-ready video codec parameters. |
| `function` | [`openAudio`](#openaudio) `virtual` `inline` | Open an audio capture device with the given parameters. |
| `function` | [`openVideo`](#openvideo) `virtual` `inline` | Open a video capture device with the given parameters. |
| `function` | [`onStreamStateChange`](#onstreamstatechange) `virtual` `inline` `override` | React to [PacketStream](base.md#packetstream) state transitions by starting or stopping capture. |
| `function` | [`openStream`](#openstream) `virtual` `inline` |  |

### Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`_emitter`](base.md#classicy_1_1PacketStreamAdapter_1ad954a1cd2f20587a622abd9dcebb56cd)  |  |
| `function` | [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a904abfd9eb913c034a3648dc1fce3f57)  | Construct the adapter, binding it to the given packet signal. |
| `function` | [`~PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a48b7da7a145125866e041c3d2cc7396c) `virtual` `inline` |  |
| `function` | [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a6762a9434263339d1a477e72de59704c) `virtual` | Emit a mutable raw buffer as a packet. |
| `function` | [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a797c0aa60e4999527e8efc3b2fdc88fd) `virtual` | Emit a read-only raw buffer as a packet (data is copied internally). |
| `function` | [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a4c605d524a538bfcc1cdf30ad4c22ce3) `virtual` | Emit a string as a packet (data is copied internally). |
| `function` | [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a10da09b2bf9b5b39c88e97813f99cbee) `virtual` | Emit a flag-only packet carrying no payload data. |
| `function` | [`emit`](base.md#classicy_1_1PacketStreamAdapter_1abb0394d938214165fc89e999e6937831) `virtual` | Emit an existing packet directly onto the outgoing signal. |
| `function` | [`getEmitter`](base.md#classicy_1_1PacketStreamAdapter_1a20cfe30f1db9ef46f9db060581fd2ff7)  | Returns a reference to the outgoing packet signal. |
| `function` | [`retention`](base.md#classicy_1_1PacketStreamAdapter_1a37a1aa11e35a20a7c527efa78e99e928) `virtual` `const` `nodiscard` | Returns how this adapter treats incoming packet lifetime. Most adapters are synchronous and therefore only borrow the packet for the current call chain. Queue-style adapters override this to advertise that they clone before deferred use. Callers may treat the first adapter reporting Cloned or Retained as the explicit ownership boundary in the stream graph. |
| `function` | [`onStreamStateChange`](base.md#classicy_1_1PacketStreamAdapter_1a86f0e2dc118359a83b8cbb996da6dc83) `virtual` `inline` | Called by the [PacketStream](base.md#packetstream) to notify when the internal [Stream](base.md#stream) state changes. On receiving the Stopped state, it is the responsibility of the adapter to have ceased all outgoing packet transmission, especially in multi-thread scenarios. |
| `function` | [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a5915b4a54729309a0fc12ee9c0107339)  | NonCopyable and NonMovable. |
| `function` | [`operator=`](base.md#classicy_1_1PacketStreamAdapter_1aeec4b3367101ffd07e3050cbfd726705)  | Deleted assignment operator. |
| `function` | [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1abf38bdf926762063adfd29427cad1362)  | Deleted constructor. |
| `function` | [`operator=`](base.md#classicy_1_1PacketStreamAdapter_1ac7fe6a98e57151ad909715f77e192481)  | Deleted assignment operator. |

### Inherited from [`Startable`](base.md#startable)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`start`](base.md#start-7) `virtual` | Starts the object (e.g. begins processing or listening). |
| `function` | [`stop`](base.md#stop-6) `virtual` | Stops the object (e.g. halts processing or closes resources). |

### Inherited from [`Runnable`](base.md#runnable)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`Runnable`](base.md#runnable-1) `inline` |  |
| `function` | [`run`](base.md#run-4) `virtual` | The run method will be called by the asynchronous context. |
| `function` | [`cancel`](base.md#cancel-1) `virtual` `inline` | Cancel the current task. The [run()](base.md#run-4) method should return ASAP. |
| `function` | [`cancelled`](base.md#cancelled) `virtual` `const` `inline` | Returns true when the task has been cancelled. |
| `variable` | [`exit`](base.md#exit)  |  |

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

##### Reimplements

- [`openAudio`](#openaudio)

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`Closing`](#closing) | `variable` | Declared here |
| [`MediaCapture`](#mediacapture-1) | `function` | Declared here |
| [`MediaCapture`](#mediacapture-2) | `function` | Declared here |
| [`MediaCapture`](#mediacapture-3) | `function` | Declared here |
| [`openFile`](#openfile-1) | `function` | Declared here |
| [`close`](#close-5) | `function` | Declared here |
| [`start`](#start-4) | `function` | Declared here |
| [`stop`](#stop-3) | `function` | Declared here |
| [`run`](#run) | `function` | Declared here |
| [`getEncoderFormat`](#getencoderformat-1) | `function` | Declared here |
| [`getEncoderAudioCodec`](#getencoderaudiocodec-1) | `function` | Declared here |
| [`getEncoderVideoCodec`](#getencodervideocodec-1) | `function` | Declared here |
| [`setLoopInput`](#setloopinput) | `function` | Declared here |
| [`setLimitFramerate`](#setlimitframerate) | `function` | Declared here |
| [`setRealtimePTS`](#setrealtimepts) | `function` | Declared here |
| [`setOpenOptions`](#setopenoptions) | `function` | Declared here |
| [`setPassthroughVideo`](#setpassthroughvideo) | `function` | Declared here |
| [`formatCtx`](#formatctx) | `function` | Declared here |
| [`video`](#video-1) | `function` | Declared here |
| [`audio`](#audio-1) | `function` | Declared here |
| [`stopping`](#stopping) | `function` | Declared here |
| [`error`](#error-2) | `function` | Declared here |
| [`_mutex`](#_mutex) | `variable` | Declared here |
| [`_thread`](#_thread) | `variable` | Declared here |
| [`_formatCtx`](#_formatctx) | `variable` | Declared here |
| [`_video`](#_video) | `variable` | Declared here |
| [`_audio`](#_audio) | `variable` | Declared here |
| [`_error`](#_error) | `variable` | Declared here |
| [`_stopping`](#_stopping) | `variable` | Declared here |
| [`_looping`](#_looping) | `variable` | Declared here |
| [`_realtime`](#_realtime) | `variable` | Declared here |
| [`_ratelimit`](#_ratelimit) | `variable` | Declared here |
| [`_passthroughVideo`](#_passthroughvideo) | `variable` | Declared here |
| [`_openOptions`](#_openoptions) | `variable` | Declared here |
| [`_videoStream`](#_videostream) | `variable` | Declared here |
| [`openStream`](#openstream-1) | `function` | Declared here |
| [`emit`](#emit) | `function` | Declared here |
| [`Ptr`](#ptr-1) | `typedef` | Declared here |
| [`emitter`](#emitter) | `variable` | Inherited from [`ICapture`](#icapture) |
| [`ICapture`](#icapture-1) | `function` | Inherited from [`ICapture`](#icapture) |
| [`start`](#start-1) | `function` | Inherited from [`ICapture`](#icapture) |
| [`stop`](#stop-1) | `function` | Inherited from [`ICapture`](#icapture) |
| [`openFile`](#openfile) | `function` | Inherited from [`ICapture`](#icapture) |
| [`close`](#close) | `function` | Inherited from [`ICapture`](#icapture) |
| [`getEncoderFormat`](#getencoderformat) | `function` | Inherited from [`ICapture`](#icapture) |
| [`getEncoderAudioCodec`](#getencoderaudiocodec) | `function` | Inherited from [`ICapture`](#icapture) |
| [`getEncoderVideoCodec`](#getencodervideocodec) | `function` | Inherited from [`ICapture`](#icapture) |
| [`openAudio`](#openaudio) | `function` | Inherited from [`ICapture`](#icapture) |
| [`openVideo`](#openvideo) | `function` | Inherited from [`ICapture`](#icapture) |
| [`onStreamStateChange`](#onstreamstatechange) | `function` | Inherited from [`ICapture`](#icapture) |
| [`openStream`](#openstream) | `function` | Inherited from [`ICapture`](#icapture) |
| [`_emitter`](base.md#classicy_1_1PacketStreamAdapter_1ad954a1cd2f20587a622abd9dcebb56cd) | `variable` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a904abfd9eb913c034a3648dc1fce3f57) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`~PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a48b7da7a145125866e041c3d2cc7396c) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a6762a9434263339d1a477e72de59704c) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a797c0aa60e4999527e8efc3b2fdc88fd) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a4c605d524a538bfcc1cdf30ad4c22ce3) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a10da09b2bf9b5b39c88e97813f99cbee) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`emit`](base.md#classicy_1_1PacketStreamAdapter_1abb0394d938214165fc89e999e6937831) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`getEmitter`](base.md#classicy_1_1PacketStreamAdapter_1a20cfe30f1db9ef46f9db060581fd2ff7) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`retention`](base.md#classicy_1_1PacketStreamAdapter_1a37a1aa11e35a20a7c527efa78e99e928) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`onStreamStateChange`](base.md#classicy_1_1PacketStreamAdapter_1a86f0e2dc118359a83b8cbb996da6dc83) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a5915b4a54729309a0fc12ee9c0107339) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`operator=`](base.md#classicy_1_1PacketStreamAdapter_1aeec4b3367101ffd07e3050cbfd726705) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1abf38bdf926762063adfd29427cad1362) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`operator=`](base.md#classicy_1_1PacketStreamAdapter_1ac7fe6a98e57151ad909715f77e192481) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`start`](base.md#start-7) | `function` | Inherited from [`Startable`](base.md#startable) |
| [`stop`](base.md#stop-6) | `function` | Inherited from [`Startable`](base.md#startable) |
| [`Runnable`](base.md#runnable-1) | `function` | Inherited from [`Runnable`](base.md#runnable) |
| [`run`](base.md#run-4) | `function` | Inherited from [`Runnable`](base.md#runnable) |
| [`cancel`](base.md#cancel-1) | `function` | Inherited from [`Runnable`](base.md#runnable) |
| [`cancelled`](base.md#cancelled) | `function` | Inherited from [`Runnable`](base.md#runnable) |
| [`exit`](base.md#exit) | `variable` | Inherited from [`Runnable`](base.md#runnable) |

### Inherited from [`ICapture`](#icapture)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`emitter`](#emitter)  |  |
| `function` | [`ICapture`](#icapture-1) `inline` |  |
| `function` | [`start`](#start-1) `virtual` `override` | Start capturing and emitting packets. |
| `function` | [`stop`](#stop-1) `virtual` `override` | Stop capturing and release device resources. |
| `function` | [`openFile`](#openfile) `virtual` `inline` | Open a media file as the capture source. |
| `function` | [`close`](#close) `virtual` `inline` | Release the capture source and any associated resources. |
| `function` | [`getEncoderFormat`](#getencoderformat) `virtual` | Populate `iformat` with the encoder-ready format derived from this capture source. |
| `function` | [`getEncoderAudioCodec`](#getencoderaudiocodec) `virtual` `inline` | Populate `params` with the encoder-ready audio codec parameters. |
| `function` | [`getEncoderVideoCodec`](#getencodervideocodec) `virtual` `inline` | Populate `params` with the encoder-ready video codec parameters. |
| `function` | [`openAudio`](#openaudio) `virtual` `inline` | Open an audio capture device with the given parameters. |
| `function` | [`openVideo`](#openvideo) `virtual` `inline` | Open a video capture device with the given parameters. |
| `function` | [`onStreamStateChange`](#onstreamstatechange) `virtual` `inline` `override` | React to [PacketStream](base.md#packetstream) state transitions by starting or stopping capture. |
| `function` | [`openStream`](#openstream) `virtual` `inline` |  |

### Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`_emitter`](base.md#classicy_1_1PacketStreamAdapter_1ad954a1cd2f20587a622abd9dcebb56cd)  |  |
| `function` | [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a904abfd9eb913c034a3648dc1fce3f57)  | Construct the adapter, binding it to the given packet signal. |
| `function` | [`~PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a48b7da7a145125866e041c3d2cc7396c) `virtual` `inline` |  |
| `function` | [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a6762a9434263339d1a477e72de59704c) `virtual` | Emit a mutable raw buffer as a packet. |
| `function` | [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a797c0aa60e4999527e8efc3b2fdc88fd) `virtual` | Emit a read-only raw buffer as a packet (data is copied internally). |
| `function` | [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a4c605d524a538bfcc1cdf30ad4c22ce3) `virtual` | Emit a string as a packet (data is copied internally). |
| `function` | [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a10da09b2bf9b5b39c88e97813f99cbee) `virtual` | Emit a flag-only packet carrying no payload data. |
| `function` | [`emit`](base.md#classicy_1_1PacketStreamAdapter_1abb0394d938214165fc89e999e6937831) `virtual` | Emit an existing packet directly onto the outgoing signal. |
| `function` | [`getEmitter`](base.md#classicy_1_1PacketStreamAdapter_1a20cfe30f1db9ef46f9db060581fd2ff7)  | Returns a reference to the outgoing packet signal. |
| `function` | [`retention`](base.md#classicy_1_1PacketStreamAdapter_1a37a1aa11e35a20a7c527efa78e99e928) `virtual` `const` `nodiscard` | Returns how this adapter treats incoming packet lifetime. Most adapters are synchronous and therefore only borrow the packet for the current call chain. Queue-style adapters override this to advertise that they clone before deferred use. Callers may treat the first adapter reporting Cloned or Retained as the explicit ownership boundary in the stream graph. |
| `function` | [`onStreamStateChange`](base.md#classicy_1_1PacketStreamAdapter_1a86f0e2dc118359a83b8cbb996da6dc83) `virtual` `inline` | Called by the [PacketStream](base.md#packetstream) to notify when the internal [Stream](base.md#stream) state changes. On receiving the Stopped state, it is the responsibility of the adapter to have ceased all outgoing packet transmission, especially in multi-thread scenarios. |
| `function` | [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a5915b4a54729309a0fc12ee9c0107339)  | NonCopyable and NonMovable. |
| `function` | [`operator=`](base.md#classicy_1_1PacketStreamAdapter_1aeec4b3367101ffd07e3050cbfd726705)  | Deleted assignment operator. |
| `function` | [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1abf38bdf926762063adfd29427cad1362)  | Deleted constructor. |
| `function` | [`operator=`](base.md#classicy_1_1PacketStreamAdapter_1ac7fe6a98e57151ad909715f77e192481)  | Deleted assignment operator. |

### Inherited from [`Startable`](base.md#startable)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`start`](base.md#start-7) `virtual` | Starts the object (e.g. begins processing or listening). |
| `function` | [`stop`](base.md#stop-6) `virtual` | Stops the object (e.g. halts processing or closes resources). |

### Inherited from [`Runnable`](base.md#runnable)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`Runnable`](base.md#runnable-1) `inline` |  |
| `function` | [`run`](base.md#run-4) `virtual` | The run method will be called by the asynchronous context. |
| `function` | [`cancel`](base.md#cancel-1) `virtual` `inline` | Cancel the current task. The [run()](base.md#run-4) method should return ASAP. |
| `function` | [`cancelled`](base.md#cancelled) `virtual` `const` `inline` | Returns true when the task has been cancelled. |
| `variable` | [`exit`](base.md#exit)  |  |

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

##### Reimplements

- [`openFile`](#openfile)

---

{#close-5}

#### close

`virtual` `override`

```cpp
virtual void close() override
```

Defined in src/av/include/icy/av/mediacapture.h:60

Stop the capture thread and close the media stream and all decoders.

##### Reimplements

- [`close`](#close)

---

{#start-4}

#### start

`virtual` `override`

```cpp
virtual void start() override
```

Defined in src/av/include/icy/av/mediacapture.h:64

Start the background capture and decode thread. Throws std::runtime_error if no media streams have been opened.

##### Reimplements

- [`start`](#start-1)

---

{#stop-3}

#### stop

`virtual` `override`

```cpp
virtual void stop() override
```

Defined in src/av/include/icy/av/mediacapture.h:67

[Signal](base.md#signal) the capture thread to stop and join it before returning.

##### Reimplements

- [`stop`](#stop-1)

---

{#run}

#### run

`virtual` `override`

```cpp
virtual void run() override
```

Defined in src/av/include/icy/av/mediacapture.h:71

Entry point for the background capture thread. Reads and decodes packets from the format context until EOF or [stop()](#stop-3) is called.

##### Reimplements

- [`run`](base.md#run-4)

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

##### Reimplements

- [`getEncoderFormat`](#getencoderformat)

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

##### Reimplements

- [`getEncoderAudioCodec`](#getencoderaudiocodec)

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

##### Reimplements

- [`getEncoderVideoCodec`](#getencodervideocodec)

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

##### Reimplements

- [`openStream`](#openstream)

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

##### Reimplements

- [`emit`](base.md#classicy_1_1PacketStreamAdapter_1abb0394d938214165fc89e999e6937831)

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`VideoCapture`](#videocapture-1) | `function` | Declared here |
| [`VideoCapture`](#videocapture-2) | `function` | Declared here |
| [`VideoCapture`](#videocapture-3) | `function` | Declared here |
| [`openVideo`](#openvideo-1) | `function` | Declared here |
| [`openVideo`](#openvideo-2) | `function` | Declared here |
| [`Ptr`](#ptr-2) | `typedef` | Declared here |
| [`Closing`](#closing) | `variable` | Inherited from [`MediaCapture`](#mediacapture) |
| [`MediaCapture`](#mediacapture-1) | `function` | Inherited from [`MediaCapture`](#mediacapture) |
| [`MediaCapture`](#mediacapture-2) | `function` | Inherited from [`MediaCapture`](#mediacapture) |
| [`MediaCapture`](#mediacapture-3) | `function` | Inherited from [`MediaCapture`](#mediacapture) |
| [`openFile`](#openfile-1) | `function` | Inherited from [`MediaCapture`](#mediacapture) |
| [`close`](#close-5) | `function` | Inherited from [`MediaCapture`](#mediacapture) |
| [`start`](#start-4) | `function` | Inherited from [`MediaCapture`](#mediacapture) |
| [`stop`](#stop-3) | `function` | Inherited from [`MediaCapture`](#mediacapture) |
| [`run`](#run) | `function` | Inherited from [`MediaCapture`](#mediacapture) |
| [`getEncoderFormat`](#getencoderformat-1) | `function` | Inherited from [`MediaCapture`](#mediacapture) |
| [`getEncoderAudioCodec`](#getencoderaudiocodec-1) | `function` | Inherited from [`MediaCapture`](#mediacapture) |
| [`getEncoderVideoCodec`](#getencodervideocodec-1) | `function` | Inherited from [`MediaCapture`](#mediacapture) |
| [`setLoopInput`](#setloopinput) | `function` | Inherited from [`MediaCapture`](#mediacapture) |
| [`setLimitFramerate`](#setlimitframerate) | `function` | Inherited from [`MediaCapture`](#mediacapture) |
| [`setRealtimePTS`](#setrealtimepts) | `function` | Inherited from [`MediaCapture`](#mediacapture) |
| [`setOpenOptions`](#setopenoptions) | `function` | Inherited from [`MediaCapture`](#mediacapture) |
| [`setPassthroughVideo`](#setpassthroughvideo) | `function` | Inherited from [`MediaCapture`](#mediacapture) |
| [`formatCtx`](#formatctx) | `function` | Inherited from [`MediaCapture`](#mediacapture) |
| [`video`](#video-1) | `function` | Inherited from [`MediaCapture`](#mediacapture) |
| [`audio`](#audio-1) | `function` | Inherited from [`MediaCapture`](#mediacapture) |
| [`stopping`](#stopping) | `function` | Inherited from [`MediaCapture`](#mediacapture) |
| [`error`](#error-2) | `function` | Inherited from [`MediaCapture`](#mediacapture) |
| [`_mutex`](#_mutex) | `variable` | Inherited from [`MediaCapture`](#mediacapture) |
| [`_thread`](#_thread) | `variable` | Inherited from [`MediaCapture`](#mediacapture) |
| [`_formatCtx`](#_formatctx) | `variable` | Inherited from [`MediaCapture`](#mediacapture) |
| [`_video`](#_video) | `variable` | Inherited from [`MediaCapture`](#mediacapture) |
| [`_audio`](#_audio) | `variable` | Inherited from [`MediaCapture`](#mediacapture) |
| [`_error`](#_error) | `variable` | Inherited from [`MediaCapture`](#mediacapture) |
| [`_stopping`](#_stopping) | `variable` | Inherited from [`MediaCapture`](#mediacapture) |
| [`_looping`](#_looping) | `variable` | Inherited from [`MediaCapture`](#mediacapture) |
| [`_realtime`](#_realtime) | `variable` | Inherited from [`MediaCapture`](#mediacapture) |
| [`_ratelimit`](#_ratelimit) | `variable` | Inherited from [`MediaCapture`](#mediacapture) |
| [`_passthroughVideo`](#_passthroughvideo) | `variable` | Inherited from [`MediaCapture`](#mediacapture) |
| [`_openOptions`](#_openoptions) | `variable` | Inherited from [`MediaCapture`](#mediacapture) |
| [`_videoStream`](#_videostream) | `variable` | Inherited from [`MediaCapture`](#mediacapture) |
| [`openStream`](#openstream-1) | `function` | Inherited from [`MediaCapture`](#mediacapture) |
| [`emit`](#emit) | `function` | Inherited from [`MediaCapture`](#mediacapture) |
| [`Ptr`](#ptr-1) | `typedef` | Inherited from [`MediaCapture`](#mediacapture) |
| [`emitter`](#emitter) | `variable` | Inherited from [`ICapture`](#icapture) |
| [`ICapture`](#icapture-1) | `function` | Inherited from [`ICapture`](#icapture) |
| [`start`](#start-1) | `function` | Inherited from [`ICapture`](#icapture) |
| [`stop`](#stop-1) | `function` | Inherited from [`ICapture`](#icapture) |
| [`openFile`](#openfile) | `function` | Inherited from [`ICapture`](#icapture) |
| [`close`](#close) | `function` | Inherited from [`ICapture`](#icapture) |
| [`getEncoderFormat`](#getencoderformat) | `function` | Inherited from [`ICapture`](#icapture) |
| [`getEncoderAudioCodec`](#getencoderaudiocodec) | `function` | Inherited from [`ICapture`](#icapture) |
| [`getEncoderVideoCodec`](#getencodervideocodec) | `function` | Inherited from [`ICapture`](#icapture) |
| [`openAudio`](#openaudio) | `function` | Inherited from [`ICapture`](#icapture) |
| [`openVideo`](#openvideo) | `function` | Inherited from [`ICapture`](#icapture) |
| [`onStreamStateChange`](#onstreamstatechange) | `function` | Inherited from [`ICapture`](#icapture) |
| [`openStream`](#openstream) | `function` | Inherited from [`ICapture`](#icapture) |
| [`_emitter`](base.md#classicy_1_1PacketStreamAdapter_1ad954a1cd2f20587a622abd9dcebb56cd) | `variable` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a904abfd9eb913c034a3648dc1fce3f57) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`~PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a48b7da7a145125866e041c3d2cc7396c) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a6762a9434263339d1a477e72de59704c) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a797c0aa60e4999527e8efc3b2fdc88fd) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a4c605d524a538bfcc1cdf30ad4c22ce3) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a10da09b2bf9b5b39c88e97813f99cbee) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`emit`](base.md#classicy_1_1PacketStreamAdapter_1abb0394d938214165fc89e999e6937831) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`getEmitter`](base.md#classicy_1_1PacketStreamAdapter_1a20cfe30f1db9ef46f9db060581fd2ff7) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`retention`](base.md#classicy_1_1PacketStreamAdapter_1a37a1aa11e35a20a7c527efa78e99e928) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`onStreamStateChange`](base.md#classicy_1_1PacketStreamAdapter_1a86f0e2dc118359a83b8cbb996da6dc83) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a5915b4a54729309a0fc12ee9c0107339) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`operator=`](base.md#classicy_1_1PacketStreamAdapter_1aeec4b3367101ffd07e3050cbfd726705) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1abf38bdf926762063adfd29427cad1362) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`operator=`](base.md#classicy_1_1PacketStreamAdapter_1ac7fe6a98e57151ad909715f77e192481) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`start`](base.md#start-7) | `function` | Inherited from [`Startable`](base.md#startable) |
| [`stop`](base.md#stop-6) | `function` | Inherited from [`Startable`](base.md#startable) |
| [`Runnable`](base.md#runnable-1) | `function` | Inherited from [`Runnable`](base.md#runnable) |
| [`run`](base.md#run-4) | `function` | Inherited from [`Runnable`](base.md#runnable) |
| [`cancel`](base.md#cancel-1) | `function` | Inherited from [`Runnable`](base.md#runnable) |
| [`cancelled`](base.md#cancelled) | `function` | Inherited from [`Runnable`](base.md#runnable) |
| [`exit`](base.md#exit) | `variable` | Inherited from [`Runnable`](base.md#runnable) |

### Inherited from [`MediaCapture`](#mediacapture)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`Closing`](#closing)  | Signals that the capture thread is closing. This signal is emitted from the capture thread context. |
| `function` | [`MediaCapture`](#mediacapture-1)  |  |
| `function` | [`MediaCapture`](#mediacapture-2)  | Deleted constructor. |
| `function` | [`MediaCapture`](#mediacapture-3)  | Deleted constructor. |
| `function` | [`openFile`](#openfile-1) `virtual` `override` | Open a media file for decoding. Automatically detects video and audio streams. |
| `function` | [`close`](#close-5) `virtual` `override` | Stop the capture thread and close the media stream and all decoders. |
| `function` | [`start`](#start-4) `virtual` `override` | Start the background capture and decode thread. Throws std::runtime_error if no media streams have been opened. |
| `function` | [`stop`](#stop-3) `virtual` `override` | [Signal](base.md#signal) the capture thread to stop and join it before returning. |
| `function` | [`run`](#run) `virtual` `override` | Entry point for the background capture thread. Reads and decodes packets from the format context until EOF or [stop()](#stop-3) is called. |
| `function` | [`getEncoderFormat`](#getencoderformat-1) `virtual` `override` | Fill `format` with the combined encoder-ready video and audio codec parameters. |
| `function` | [`getEncoderAudioCodec`](#getencoderaudiocodec-1) `virtual` `override` | Fill `params` with the decoder's output audio codec parameters. Throws std::runtime_error if audio parameters have not been initialised. |
| `function` | [`getEncoderVideoCodec`](#getencodervideocodec-1) `virtual` `override` | Fill `params` with the decoder's output video codec parameters. Throws std::runtime_error if video parameters have not been initialised. |
| `function` | [`setLoopInput`](#setloopinput)  | Continuously loop the input file when set. |
| `function` | [`setLimitFramerate`](#setlimitframerate)  | Limit playback to video FPS. |
| `function` | [`setRealtimePTS`](#setrealtimepts)  | Set to use realtime PTS calculation. This is preferred when using live captures as FFmpeg-provided values are not always reliable. |
| `function` | [`setOpenOptions`](#setopenoptions)  | Set demuxer options applied at the next [openFile()](#openfile-1) call. Keys map to libavformat AVOption entries (e.g. "rtsp_transport", "fflags", "analyzeduration", "probesize"). Useful for live network sources that need low-latency hints. Pass an empty map to clear. |
| `function` | [`setPassthroughVideo`](#setpassthroughvideo)  | Skip the video decoder. Encoded video AVPackets from the input stream are emitted directly as `[av::VideoPacket](#videopacket)` without decoding. The emitted packets carry the source's encoded payload and a `time` in microseconds rescaled from the stream's timebase. The `iframe` flag is set from `AV_PKT_FLAG_KEY`. |
| `function` | [`formatCtx`](#formatctx) `const` |  |
| `function` | [`video`](#video-1) `const` |  |
| `function` | [`audio`](#audio-1) `const` |  |
| `function` | [`stopping`](#stopping) `const` |  |
| `function` | [`error`](#error-2) `const` |  |
| `variable` | [`_mutex`](#_mutex)  |  |
| `variable` | [`_thread`](#_thread)  |  |
| `variable` | [`_formatCtx`](#_formatctx)  |  |
| `variable` | [`_video`](#_video)  |  |
| `variable` | [`_audio`](#_audio)  |  |
| `variable` | [`_error`](#_error)  |  |
| `variable` | [`_stopping`](#_stopping)  |  |
| `variable` | [`_looping`](#_looping)  |  |
| `variable` | [`_realtime`](#_realtime)  |  |
| `variable` | [`_ratelimit`](#_ratelimit)  |  |
| `variable` | [`_passthroughVideo`](#_passthroughvideo)  |  |
| `variable` | [`_openOptions`](#_openoptions)  |  |
| `variable` | [`_videoStream`](#_videostream)  |  |
| `function` | [`openStream`](#openstream-1) `virtual` `override` | Open the underlying media stream. |
| `function` | [`emit`](#emit) `virtual` `override` | Emit an existing packet directly onto the outgoing signal. |
| `typedef` | [`Ptr`](#ptr-1)  |  |

### Inherited from [`ICapture`](#icapture)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`emitter`](#emitter)  |  |
| `function` | [`ICapture`](#icapture-1) `inline` |  |
| `function` | [`start`](#start-1) `virtual` `override` | Start capturing and emitting packets. |
| `function` | [`stop`](#stop-1) `virtual` `override` | Stop capturing and release device resources. |
| `function` | [`openFile`](#openfile) `virtual` `inline` | Open a media file as the capture source. |
| `function` | [`close`](#close) `virtual` `inline` | Release the capture source and any associated resources. |
| `function` | [`getEncoderFormat`](#getencoderformat) `virtual` | Populate `iformat` with the encoder-ready format derived from this capture source. |
| `function` | [`getEncoderAudioCodec`](#getencoderaudiocodec) `virtual` `inline` | Populate `params` with the encoder-ready audio codec parameters. |
| `function` | [`getEncoderVideoCodec`](#getencodervideocodec) `virtual` `inline` | Populate `params` with the encoder-ready video codec parameters. |
| `function` | [`openAudio`](#openaudio) `virtual` `inline` | Open an audio capture device with the given parameters. |
| `function` | [`openVideo`](#openvideo) `virtual` `inline` | Open a video capture device with the given parameters. |
| `function` | [`onStreamStateChange`](#onstreamstatechange) `virtual` `inline` `override` | React to [PacketStream](base.md#packetstream) state transitions by starting or stopping capture. |
| `function` | [`openStream`](#openstream) `virtual` `inline` |  |

### Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`_emitter`](base.md#classicy_1_1PacketStreamAdapter_1ad954a1cd2f20587a622abd9dcebb56cd)  |  |
| `function` | [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a904abfd9eb913c034a3648dc1fce3f57)  | Construct the adapter, binding it to the given packet signal. |
| `function` | [`~PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a48b7da7a145125866e041c3d2cc7396c) `virtual` `inline` |  |
| `function` | [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a6762a9434263339d1a477e72de59704c) `virtual` | Emit a mutable raw buffer as a packet. |
| `function` | [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a797c0aa60e4999527e8efc3b2fdc88fd) `virtual` | Emit a read-only raw buffer as a packet (data is copied internally). |
| `function` | [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a4c605d524a538bfcc1cdf30ad4c22ce3) `virtual` | Emit a string as a packet (data is copied internally). |
| `function` | [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a10da09b2bf9b5b39c88e97813f99cbee) `virtual` | Emit a flag-only packet carrying no payload data. |
| `function` | [`emit`](base.md#classicy_1_1PacketStreamAdapter_1abb0394d938214165fc89e999e6937831) `virtual` | Emit an existing packet directly onto the outgoing signal. |
| `function` | [`getEmitter`](base.md#classicy_1_1PacketStreamAdapter_1a20cfe30f1db9ef46f9db060581fd2ff7)  | Returns a reference to the outgoing packet signal. |
| `function` | [`retention`](base.md#classicy_1_1PacketStreamAdapter_1a37a1aa11e35a20a7c527efa78e99e928) `virtual` `const` `nodiscard` | Returns how this adapter treats incoming packet lifetime. Most adapters are synchronous and therefore only borrow the packet for the current call chain. Queue-style adapters override this to advertise that they clone before deferred use. Callers may treat the first adapter reporting Cloned or Retained as the explicit ownership boundary in the stream graph. |
| `function` | [`onStreamStateChange`](base.md#classicy_1_1PacketStreamAdapter_1a86f0e2dc118359a83b8cbb996da6dc83) `virtual` `inline` | Called by the [PacketStream](base.md#packetstream) to notify when the internal [Stream](base.md#stream) state changes. On receiving the Stopped state, it is the responsibility of the adapter to have ceased all outgoing packet transmission, especially in multi-thread scenarios. |
| `function` | [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a5915b4a54729309a0fc12ee9c0107339)  | NonCopyable and NonMovable. |
| `function` | [`operator=`](base.md#classicy_1_1PacketStreamAdapter_1aeec4b3367101ffd07e3050cbfd726705)  | Deleted assignment operator. |
| `function` | [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1abf38bdf926762063adfd29427cad1362)  | Deleted constructor. |
| `function` | [`operator=`](base.md#classicy_1_1PacketStreamAdapter_1ac7fe6a98e57151ad909715f77e192481)  | Deleted assignment operator. |

### Inherited from [`Startable`](base.md#startable)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`start`](base.md#start-7) `virtual` | Starts the object (e.g. begins processing or listening). |
| `function` | [`stop`](base.md#stop-6) `virtual` | Stops the object (e.g. halts processing or closes resources). |

### Inherited from [`Runnable`](base.md#runnable)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`Runnable`](base.md#runnable-1) `inline` |  |
| `function` | [`run`](base.md#run-4) `virtual` | The run method will be called by the asynchronous context. |
| `function` | [`cancel`](base.md#cancel-1) `virtual` `inline` | Cancel the current task. The [run()](base.md#run-4) method should return ASAP. |
| `function` | [`cancelled`](base.md#cancelled) `virtual` `const` `inline` | Returns true when the task has been cancelled. |
| `variable` | [`exit`](base.md#exit)  |  |

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

##### Reimplements

- [`openVideo`](#openvideo)

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`DeviceWatcher`](#devicewatcher-1) | `function` | Declared here |
| [`start`](#start-5) | `function` | Declared here |
| [`stop`](#stop-4) | `function` | Declared here |

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

##### Reimplemented by

- [`start`](#start-2)
- [`start`](#start)
- [`start`](#start-6)

---

{#stop-4}

#### stop

`virtual` `inline`

```cpp
virtual inline void stop()
```

Defined in src/av/include/icy/av/devicemanager.h:200

##### Reimplemented by

- [`stop`](#stop-2)
- [`stop`](#stop)
- [`stop`](#stop-5)

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`DevicesChanged`](#deviceschanged) | `variable` | Declared here |
| [`DeviceManager`](#devicemanager-1) | `function` | Declared here |
| [`getCameras`](#getcameras) | `function` | Declared here |
| [`getMicrophones`](#getmicrophones) | `function` | Declared here |
| [`getSpeakers`](#getspeakers) | `function` | Declared here |
| [`getDefaultCamera`](#getdefaultcamera) | `function` | Declared here |
| [`getDefaultMicrophone`](#getdefaultmicrophone) | `function` | Declared here |
| [`getDefaultSpeaker`](#getdefaultspeaker) | `function` | Declared here |
| [`findCamera`](#findcamera) | `function` | Declared here |
| [`findMicrophone`](#findmicrophone) | `function` | Declared here |
| [`findSpeaker`](#findspeaker) | `function` | Declared here |
| [`getDeviceList`](#getdevicelist-3) | `function` | Declared here |
| [`getCapabilities`](#getcapabilities) | `function` | Declared here |
| [`setWatcher`](#setwatcher) | `function` | Declared here |
| [`watcher`](#watcher) | `function` | Declared here |
| [`print`](#print-5) | `function` | Declared here |
| [`getHardwareCodecs`](#gethardwarecodecs) | `function` | Declared here |
| [`negotiateVideoCapture`](#negotiatevideocapture) | `function` | Declared here |
| [`negotiateAudioCapture`](#negotiateaudiocapture) | `function` | Declared here |
| [`findVideoInputFormat`](#findvideoinputformat) | `function` | Declared here |
| [`findAudioInputFormat`](#findaudioinputformat) | `function` | Declared here |
| [`instance`](#instance) | `function` | Declared here |
| [`shutdown`](#shutdown) | `function` | Declared here |
| [`_watcher`](#_watcher) | `variable` | Declared here |

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`name`](#name-3) | `variable` | Declared here |
| [`type`](#type-6) | `variable` | Declared here |

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`FormatRegistry`](#formatregistry-1) | `function` | Declared here |
| [`get`](#get) | `function` | Declared here |
| [`getByID`](#getbyid) | `function` | Declared here |
| [`getOrDefault`](#getordefault) | `function` | Declared here |
| [`getDefault`](#getdefault) | `function` | Declared here |
| [`registerFormat`](#registerformat) | `function` | Declared here |
| [`unregisterFormat`](#unregisterformat) | `function` | Declared here |
| [`setDefault`](#setdefault) | `function` | Declared here |
| [`exists`](#exists) | `function` | Declared here |
| [`clear`](#clear) | `function` | Declared here |
| [`formats`](#formats) | `function` | Declared here |
| [`instance`](#instance-1) | `function` | Declared here |
| [`_formats`](#_formats) | `variable` | Declared here |
| [`_default`](#_default) | `variable` | Declared here |
| [`_mutex`](#_mutex-1) | `variable` | Declared here |
| [`FormatRegistry`](#formatregistry-2) | `function` | Declared here |
| [`FormatRegistry`](#formatregistry-3) | `function` | Declared here |
| [`findByName`](#findbyname) | `function` | Declared here |
| [`defaultLocked`](#defaultlocked) | `function` | Declared here |

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`WindowsDeviceWatcher`](#windowsdevicewatcher-1) | `function` | Declared here |
| [`start`](#start-6) | `function` | Declared here |
| [`stop`](#stop-5) | `function` | Declared here |
| [`_impl`](#_impl-2) | `variable` | Declared here |
| [`_manager`](#_manager-2) | `variable` | Declared here |
| [`DeviceWatcher`](#devicewatcher-1) | `function` | Inherited from [`DeviceWatcher`](#devicewatcher) |
| [`start`](#start-5) | `function` | Inherited from [`DeviceWatcher`](#devicewatcher) |
| [`stop`](#stop-4) | `function` | Inherited from [`DeviceWatcher`](#devicewatcher) |

### Inherited from [`DeviceWatcher`](#devicewatcher)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`DeviceWatcher`](#devicewatcher-1) `inline` `explicit` |  |
| `function` | [`start`](#start-5) `virtual` `inline` |  |
| `function` | [`stop`](#stop-4) `virtual` `inline` |  |

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

##### Reimplements

- [`start`](#start-5)

---

{#stop-5}

#### stop

`virtual` `override`

```cpp
virtual void stop() override
```

Defined in src/av/include/icy/av/win32/mediafoundation.h:73

Stop monitoring and unregister all device notifications.

##### Reimplements

- [`stop`](#stop-4)

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`emitter`](#emitter-4) | `variable` | Declared here |
| [`MultiplexEncoder`](#multiplexencoder-1) | `function` | Declared here |
| [`MultiplexEncoder`](#multiplexencoder-2) | `function` | Declared here |
| [`MultiplexEncoder`](#multiplexencoder-3) | `function` | Declared here |
| [`init`](#init-1) | `function` | Declared here |
| [`uninit`](#uninit-1) | `function` | Declared here |
| [`cleanup`](#cleanup-1) | `function` | Declared here |
| [`createVideo`](#createvideo-1) | `function` | Declared here |
| [`freeVideo`](#freevideo-1) | `function` | Declared here |
| [`encodeVideo`](#encodevideo-1) | `function` | Declared here |
| [`encodeVideo`](#encodevideo-2) | `function` | Declared here |
| [`encodeVideo`](#encodevideo-3) | `function` | Declared here |
| [`createAudio`](#createaudio-1) | `function` | Declared here |
| [`freeAudio`](#freeaudio-1) | `function` | Declared here |
| [`encodeAudio`](#encodeaudio) | `function` | Declared here |
| [`encodeAudio`](#encodeaudio-1) | `function` | Declared here |
| [`flush`](#flush-7) | `function` | Declared here |
| [`options`](#options-2) | `function` | Declared here |
| [`video`](#video-2) | `function` | Declared here |
| [`audio`](#audio-2) | `function` | Declared here |
| [`_options`](#_options) | `variable` | Declared here |
| [`_formatCtx`](#_formatctx-1) | `variable` | Declared here |
| [`_video`](#_video-1) | `variable` | Declared here |
| [`_audio`](#_audio-1) | `variable` | Declared here |
| [`_ioCtx`](#_ioctx) | `variable` | Declared here |
| [`_ioBuffer`](#_iobuffer) | `variable` | Declared here |
| [`_pts`](#_pts) | `variable` | Declared here |
| [`_mutex`](#_mutex-2) | `variable` | Declared here |
| [`writeOutputPacket`](#writeoutputpacket) | `function` | Declared here |
| [`updateStreamPts`](#updatestreampts) | `function` | Declared here |
| [`onVideoEncoded`](#onvideoencoded) | `function` | Declared here |
| [`onAudioEncoded`](#onaudioencoded) | `function` | Declared here |
| [`init`](#init) | `function` | Inherited from [`IEncoder`](#iencoder) |
| [`uninit`](#uninit) | `function` | Inherited from [`IEncoder`](#iencoder) |
| [`cleanup`](#cleanup) | `function` | Inherited from [`IEncoder`](#iencoder) |
| [`options`](#options-1) | `function` | Inherited from [`IEncoder`](#iencoder) |
| [`createVideo`](#createvideo) | `function` | Inherited from [`IEncoder`](#iencoder) |
| [`freeVideo`](#freevideo) | `function` | Inherited from [`IEncoder`](#iencoder) |
| [`encodeVideo`](#encodevideo) | `function` | Inherited from [`IEncoder`](#iencoder) |
| [`createAudio`](#createaudio) | `function` | Inherited from [`IEncoder`](#iencoder) |
| [`freeAudio`](#freeaudio) | `function` | Inherited from [`IEncoder`](#iencoder) |
| [`flush`](#flush) | `function` | Inherited from [`IEncoder`](#iencoder) |
| [`isNone`](#isnone) | `function` | Inherited from [`IEncoder`](#iencoder) |
| [`isReady`](#isready) | `function` | Inherited from [`IEncoder`](#iencoder) |
| [`isEncoding`](#isencoding) | `function` | Inherited from [`IEncoder`](#iencoder) |
| [`isActive`](#isactive) | `function` | Inherited from [`IEncoder`](#iencoder) |
| [`isStopped`](#isstopped) | `function` | Inherited from [`IEncoder`](#iencoder) |
| [`isError`](#iserror) | `function` | Inherited from [`IEncoder`](#iencoder) |
| [`Type`](#type-3) | `enum` | Inherited from [`IEncoder`](#iencoder) |
| [`StateChange`](base.md#classicy_1_1Stateful_1adba38e2b8a737448d48fc56b4bbe1dc0) | `variable` | Inherited from [`Stateful`](base.md#stateful) |
| [`_state`](base.md#classicy_1_1Stateful_1a45326923d41e72e4afc70d9ed8acc15d) | `variable` | Inherited from [`Stateful`](base.md#stateful) |
| [`Stateful`](base.md#classicy_1_1Stateful_1a444344bc5f8563af4d289be6beb693a0) | `function` | Inherited from [`Stateful`](base.md#stateful) |
| [`~Stateful`](base.md#classicy_1_1Stateful_1a70818a671339a0468ef71e6d54c97c77) | `function` | Inherited from [`Stateful`](base.md#stateful) |
| [`stateEquals`](base.md#classicy_1_1Stateful_1a8c8bd62dbd4251749150ed7f95bdb875) | `function` | Inherited from [`Stateful`](base.md#stateful) |
| [`stateBetween`](base.md#classicy_1_1Stateful_1a702bf6338c45b6ce75540a2d7f03a18f) | `function` | Inherited from [`Stateful`](base.md#stateful) |
| [`state`](base.md#classicy_1_1Stateful_1ae1082bd323b27b8eb1022f160d77a6e6) | `function` | Inherited from [`Stateful`](base.md#stateful) |
| [`state`](base.md#classicy_1_1Stateful_1ab63e2709491c8fb841f7dd5720c15020) | `function` | Inherited from [`Stateful`](base.md#stateful) |
| [`beforeStateChange`](base.md#classicy_1_1Stateful_1a0ae9d2650c9a6fdb276b88e0b2d3ca34) | `function` | Inherited from [`Stateful`](base.md#stateful) |
| [`onStateChange`](base.md#classicy_1_1Stateful_1a06e5572c313c3b54db932c922166079d) | `function` | Inherited from [`Stateful`](base.md#stateful) |
| [`setState`](base.md#classicy_1_1Stateful_1a88cfa69863f505b8232c7cce5989a395) | `function` | Inherited from [`Stateful`](base.md#stateful) |
| [`setState`](base.md#classicy_1_1Stateful_1a9d5c3013be4fb7508266ed0b950694aa) | `function` | Inherited from [`Stateful`](base.md#stateful) |

### Inherited from [`IEncoder`](#iencoder)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`init`](#init) `virtual` | Initialise the encoder, open codec contexts, and transition to Ready or Encoding state. |
| `function` | [`uninit`](#uninit) `virtual` | Flush remaining packets, close codec contexts, and release all encoder resources. |
| `function` | [`cleanup`](#cleanup) `virtual` `inline` | Perform any additional cleanup after [uninit()](#uninit). Default is a no-op. |
| `function` | [`options`](#options-1) `virtual` `const` `nodiscard` |  |
| `function` | [`createVideo`](#createvideo) `virtual` `inline` | Initialise the video codec context and stream. |
| `function` | [`freeVideo`](#freevideo) `virtual` `inline` | Free the video codec context and stream. |
| `function` | [`encodeVideo`](#encodevideo) `virtual` `inline` `nodiscard` | Encode a single AVFrame of video. |
| `function` | [`createAudio`](#createaudio) `virtual` `inline` | Initialise the audio codec context and stream. |
| `function` | [`freeAudio`](#freeaudio) `virtual` `inline` | Free the audio codec context and stream. |
| `function` | [`flush`](#flush) `virtual` `inline` | Flush any internally buffered packets to the output. |
| `function` | [`isNone`](#isnone) `const` `inline` |  |
| `function` | [`isReady`](#isready) `const` `inline` |  |
| `function` | [`isEncoding`](#isencoding) `virtual` `const` `inline` |  |
| `function` | [`isActive`](#isactive) `virtual` `const` `inline` |  |
| `function` | [`isStopped`](#isstopped) `virtual` `const` `inline` |  |
| `function` | [`isError`](#iserror) `virtual` `const` `inline` |  |
| `enum` | [`Type`](#type-3)  |  |

### Inherited from [`Stateful`](base.md#stateful)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`StateChange`](base.md#classicy_1_1Stateful_1adba38e2b8a737448d48fc56b4bbe1dc0)  | Signals when the state changes. |
| `variable` | [`_state`](base.md#classicy_1_1Stateful_1a45326923d41e72e4afc70d9ed8acc15d)  |  |
| `function` | [`Stateful`](base.md#classicy_1_1Stateful_1a444344bc5f8563af4d289be6beb693a0) `inline` |  |
| `function` | [`~Stateful`](base.md#classicy_1_1Stateful_1a70818a671339a0468ef71e6d54c97c77) `virtual` `inline` |  |
| `function` | [`stateEquals`](base.md#classicy_1_1Stateful_1a8c8bd62dbd4251749150ed7f95bdb875) `virtual` `const` `inline` | Returns true if the current state ID equals the given ID. |
| `function` | [`stateBetween`](base.md#classicy_1_1Stateful_1a702bf6338c45b6ce75540a2d7f03a18f) `virtual` `const` `inline` | Returns true if the current state ID is in the inclusive range [lid, rid]. |
| `function` | [`state`](base.md#classicy_1_1Stateful_1ae1082bd323b27b8eb1022f160d77a6e6) `virtual` `inline` | Returns a mutable reference to the current state. |
| `function` | [`state`](base.md#classicy_1_1Stateful_1ab63e2709491c8fb841f7dd5720c15020) `virtual` `const` `inline` | Returns a copy of the current state. |
| `function` | [`beforeStateChange`](base.md#classicy_1_1Stateful_1a0ae9d2650c9a6fdb276b88e0b2d3ca34) `virtual` `inline` | Override to handle pre state change logic. Return false to prevent state change. |
| `function` | [`onStateChange`](base.md#classicy_1_1Stateful_1a06e5572c313c3b54db932c922166079d) `virtual` `inline` | Override to handle post state change logic. |
| `function` | [`setState`](base.md#classicy_1_1Stateful_1a88cfa69863f505b8232c7cce5989a395) `virtual` `inline` | Sets the state and sends the state signal if the state change was successful. |
| `function` | [`setState`](base.md#classicy_1_1Stateful_1a9d5c3013be4fb7508266ed0b950694aa) `virtual` `inline` | Sets the state and sends the state signal if the state change was successful. |

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

##### Reimplements

- [`init`](#init)

---

{#uninit-1}

#### uninit

`virtual` `override`

```cpp
virtual void uninit() override
```

Defined in src/av/include/icy/av/multiplexencoder.h:58

Flush encoded packets, write the format trailer, and close the output container.

##### Reimplements

- [`uninit`](#uninit)

---

{#cleanup-1}

#### cleanup

`virtual` `override`

```cpp
virtual void cleanup() override
```

Defined in src/av/include/icy/av/multiplexencoder.h:61

Release all resources allocated by [init()](#init-1) without writing a trailer.

##### Reimplements

- [`cleanup`](#cleanup)

---

{#createvideo-1}

#### createVideo

`virtual` `override`

```cpp
virtual void createVideo() override
```

Defined in src/av/include/icy/av/multiplexencoder.h:64

Create the video encoder and add the stream to the output container.

##### Reimplements

- [`createVideo`](#createvideo)

---

{#freevideo-1}

#### freeVideo

`virtual` `override`

```cpp
virtual void freeVideo() override
```

Defined in src/av/include/icy/av/multiplexencoder.h:67

Flush and free the video encoder and its stream.

##### Reimplements

- [`freeVideo`](#freevideo)

---

{#encodevideo-1}

#### encodeVideo

`virtual` `nodiscard` `override`

```cpp
[[nodiscard]] virtual bool encodeVideo(AVFrame * frame) override
```

Defined in src/av/include/icy/av/multiplexencoder.h:71

Encode a single video frame. All frame values must be set, such as size, pixel format, and PTS.

##### Reimplements

- [`encodeVideo`](#encodevideo)

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

##### Reimplements

- [`createAudio`](#createaudio)

---

{#freeaudio-1}

#### freeAudio

`virtual` `override`

```cpp
virtual void freeAudio() override
```

Defined in src/av/include/icy/av/multiplexencoder.h:100

Flush and free the audio encoder and its stream.

##### Reimplements

- [`freeAudio`](#freeaudio)

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

##### Reimplements

- [`flush`](#flush)

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

##### Reimplements

- [`options`](#options-1)

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`PacketStream`](#packetstream-1) | `friend` | Declared here |
| [`AudioPacketEncoder`](#audiopacketencoder-1) | `function` | Declared here |
| [`AudioPacketEncoder`](#audiopacketencoder-2) | `function` | Declared here |
| [`process`](#process-2) | `function` | Declared here |
| [`accepts`](#accepts) | `function` | Declared here |
| [`_initialized`](#_initialized) | `variable` | Declared here |
| [`_mutex`](#_mutex-3) | `variable` | Declared here |
| [`onStreamStateChange`](#onstreamstatechange-2) | `function` | Declared here |
| [`fifo`](#fifo-1) | `variable` | Inherited from [`AudioEncoder`](#audioencoder) |
| [`format`](#format-6) | `variable` | Inherited from [`AudioEncoder`](#audioencoder) |
| [`nextOutputPts`](#nextoutputpts) | `variable` | Inherited from [`AudioEncoder`](#audioencoder) |
| [`AudioEncoder`](#audioencoder-1) | `function` | Inherited from [`AudioEncoder`](#audioencoder) |
| [`create`](#create-2) | `function` | Inherited from [`AudioEncoder`](#audioencoder) |
| [`close`](#close-4) | `function` | Inherited from [`AudioEncoder`](#audioencoder) |
| [`encode`](#encode-3) | `function` | Inherited from [`AudioEncoder`](#audioencoder) |
| [`encode`](#encode-4) | `function` | Inherited from [`AudioEncoder`](#audioencoder) |
| [`encode`](#encode-5) | `function` | Inherited from [`AudioEncoder`](#audioencoder) |
| [`flush`](#flush-3) | `function` | Inherited from [`AudioEncoder`](#audioencoder) |
| [`emitter`](#emitter-2) | `variable` | Inherited from [`AudioContext`](#audiocontext) |
| [`iparams`](#iparams) | `variable` | Inherited from [`AudioContext`](#audiocontext) |
| [`oparams`](#oparams) | `variable` | Inherited from [`AudioContext`](#audiocontext) |
| [`stream`](#stream-1) | `variable` | Inherited from [`AudioContext`](#audiocontext) |
| [`ctx`](#ctx) | `variable` | Inherited from [`AudioContext`](#audiocontext) |
| [`codec`](#codec-5) | `variable` | Inherited from [`AudioContext`](#audiocontext) |
| [`frame`](#frame) | `variable` | Inherited from [`AudioContext`](#audiocontext) |
| [`resampler`](#resampler) | `variable` | Inherited from [`AudioContext`](#audiocontext) |
| [`outputFrameSize`](#outputframesize) | `variable` | Inherited from [`AudioContext`](#audiocontext) |
| [`time`](#time) | `variable` | Inherited from [`AudioContext`](#audiocontext) |
| [`pts`](#pts) | `variable` | Inherited from [`AudioContext`](#audiocontext) |
| [`seconds`](#seconds) | `variable` | Inherited from [`AudioContext`](#audiocontext) |
| [`error`](#error-1) | `variable` | Inherited from [`AudioContext`](#audiocontext) |
| [`AudioContext`](#audiocontext-1) | `function` | Inherited from [`AudioContext`](#audiocontext) |
| [`AudioContext`](#audiocontext-2) | `function` | Inherited from [`AudioContext`](#audiocontext) |
| [`AudioContext`](#audiocontext-3) | `function` | Inherited from [`AudioContext`](#audiocontext) |
| [`create`](#create) | `function` | Inherited from [`AudioContext`](#audiocontext) |
| [`open`](#open) | `function` | Inherited from [`AudioContext`](#audiocontext) |
| [`close`](#close-2) | `function` | Inherited from [`AudioContext`](#audiocontext) |
| [`decode`](#decode) | `function` | Inherited from [`AudioContext`](#audiocontext) |
| [`encode`](#encode) | `function` | Inherited from [`AudioContext`](#audiocontext) |
| [`encode`](#encode-1) | `function` | Inherited from [`AudioContext`](#audiocontext) |
| [`encode`](#encode-2) | `function` | Inherited from [`AudioContext`](#audiocontext) |
| [`flush`](#flush-1) | `function` | Inherited from [`AudioContext`](#audiocontext) |
| [`recreateResampler`](#recreateresampler) | `function` | Inherited from [`AudioContext`](#audiocontext) |
| [`PacketProcessor`](base.md#classicy_1_1PacketProcessor_1aa9a61f3eb4c53e7673b29d4c97686d9a) | `function` | Inherited from [`PacketProcessor`](base.md#packetprocessor) |
| [`process`](base.md#classicy_1_1PacketProcessor_1acd9fb1a5fa525c0a2d96971fe6c06294) | `function` | Inherited from [`PacketProcessor`](base.md#packetprocessor) |
| [`accepts`](base.md#classicy_1_1PacketProcessor_1a838d9d75f81bd943b40efcbe683394b7) | `function` | Inherited from [`PacketProcessor`](base.md#packetprocessor) |
| [`operator<<`](base.md#classicy_1_1PacketProcessor_1ae8908612aea63a6e5b3d3f7714a8e7a3) | `function` | Inherited from [`PacketProcessor`](base.md#packetprocessor) |
| [`_emitter`](base.md#classicy_1_1PacketStreamAdapter_1ad954a1cd2f20587a622abd9dcebb56cd) | `variable` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a904abfd9eb913c034a3648dc1fce3f57) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`~PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a48b7da7a145125866e041c3d2cc7396c) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a6762a9434263339d1a477e72de59704c) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a797c0aa60e4999527e8efc3b2fdc88fd) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a4c605d524a538bfcc1cdf30ad4c22ce3) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a10da09b2bf9b5b39c88e97813f99cbee) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`emit`](base.md#classicy_1_1PacketStreamAdapter_1abb0394d938214165fc89e999e6937831) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`getEmitter`](base.md#classicy_1_1PacketStreamAdapter_1a20cfe30f1db9ef46f9db060581fd2ff7) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`retention`](base.md#classicy_1_1PacketStreamAdapter_1a37a1aa11e35a20a7c527efa78e99e928) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`onStreamStateChange`](base.md#classicy_1_1PacketStreamAdapter_1a86f0e2dc118359a83b8cbb996da6dc83) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a5915b4a54729309a0fc12ee9c0107339) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`operator=`](base.md#classicy_1_1PacketStreamAdapter_1aeec4b3367101ffd07e3050cbfd726705) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1abf38bdf926762063adfd29427cad1362) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`operator=`](base.md#classicy_1_1PacketStreamAdapter_1ac7fe6a98e57151ad909715f77e192481) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |

### Inherited from [`AudioEncoder`](#audioencoder)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`fifo`](#fifo-1)  |  |
| `variable` | [`format`](#format-6)  |  |
| `variable` | [`nextOutputPts`](#nextoutputpts)  | Running PTS counter for encoder output, in encoder time_base units. Anchored to the first input packet's PTS, then advanced by frame->nb_samples per encoded frame. AV_NOPTS_VALUE before anchoring. |
| `function` | [`AudioEncoder`](#audioencoder-1)  | Construct an encoder, optionally tied to an existing muxer context. |
| `function` | [`create`](#create-2) `virtual` `override` | Initialise the AVCodecContext using oparams. Adds an audio stream to `format` if one was provided at construction. |
| `function` | [`close`](#close-4) `virtual` `override` | Close and free the AVCodecContext, FIFO buffer, and associated resources. |
| `function` | [`encode`](#encode-3) `virtual` `nodiscard` `override` | Encode interleaved audio samples. |
| `function` | [`encode`](#encode-4) `virtual` `nodiscard` `override` | Encode planar audio samples. |
| `function` | [`encode`](#encode-5) `virtual` `nodiscard` `override` | Encode a single AVFrame (typically from a decoder or resampler). |
| `function` | [`flush`](#flush-3) `virtual` `override` | Flush remaining packets to be encoded. This method should be called once before stream closure. |

### Inherited from [`AudioContext`](#audiocontext)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`emitter`](#emitter-2)  |  |
| `variable` | [`iparams`](#iparams)  | input parameters |
| `variable` | [`oparams`](#oparams)  | output parameters |
| `variable` | [`stream`](#stream-1)  | encoder or decoder stream |
| `variable` | [`ctx`](#ctx)  | encoder or decoder context |
| `variable` | [`codec`](#codec-5)  | encoder or decoder codec |
| `variable` | [`frame`](#frame)  | last encoded or decoded frame |
| `variable` | [`resampler`](#resampler)  | audio resampler |
| `variable` | [`outputFrameSize`](#outputframesize)  | encoder or decoder output frame size |
| `variable` | [`time`](#time)  | stream time in codec time base |
| `variable` | [`pts`](#pts)  | last packet pts value |
| `variable` | [`seconds`](#seconds)  | audio time in seconds |
| `variable` | [`error`](#error-1)  | error message |
| `function` | [`AudioContext`](#audiocontext-1)  |  |
| `function` | [`AudioContext`](#audiocontext-2)  | Deleted constructor. |
| `function` | [`AudioContext`](#audiocontext-3)  | Deleted constructor. |
| `function` | [`create`](#create) `virtual` | Initialise the AVCodecContext with codec-specific defaults. Implemented by [AudioEncoder](#audioencoder) and [AudioDecoder](#audiodecoder). |
| `function` | [`open`](#open) `virtual` | Open the codec and create the resampler if input/output parameters differ. Throws std::runtime_error if the codec context has not been created. |
| `function` | [`close`](#close-2) `virtual` | Close the codec context, free the frame, and reset timestamps. |
| `function` | [`decode`](#decode) `virtual` `nodiscard` | Decode a compressed audio packet and emit the resulting samples. |
| `function` | [`encode`](#encode) `virtual` `nodiscard` | Encode a buffer of interleaved audio samples. |
| `function` | [`encode`](#encode-1) `virtual` `nodiscard` | Encode a buffer of planar audio samples. |
| `function` | [`encode`](#encode-2) `virtual` `nodiscard` | Encode a single AVFrame. |
| `function` | [`flush`](#flush-1) `virtual` | Flush any frames buffered inside the codec and emit remaining output. |
| `function` | [`recreateResampler`](#recreateresampler) `virtual` | Recreate the [AudioResampler](#audioresampler) using the current iparams and oparams. Called automatically by [open()](#open) when format conversion is required. |

### Inherited from [`PacketProcessor`](base.md#packetprocessor)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`PacketProcessor`](base.md#classicy_1_1PacketProcessor_1aa9a61f3eb4c53e7673b29d4c97686d9a) `inline` |  |
| `function` | [`process`](base.md#classicy_1_1PacketProcessor_1acd9fb1a5fa525c0a2d96971fe6c06294) `virtual` | This method performs processing on the given packet and emits the result. |
| `function` | [`accepts`](base.md#classicy_1_1PacketProcessor_1a838d9d75f81bd943b40efcbe683394b7) `virtual` `inline` | This method ensures compatibility with the given packet type. Return false to reject the packet. |
| `function` | [`operator<<`](base.md#classicy_1_1PacketProcessor_1ae8908612aea63a6e5b3d3f7714a8e7a3) `virtual` `inline` | [Stream](base.md#stream) operator alias for [process()](base.md#classicy_1_1PacketProcessor_1acd9fb1a5fa525c0a2d96971fe6c06294). |

### Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`_emitter`](base.md#classicy_1_1PacketStreamAdapter_1ad954a1cd2f20587a622abd9dcebb56cd)  |  |
| `function` | [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a904abfd9eb913c034a3648dc1fce3f57)  | Construct the adapter, binding it to the given packet signal. |
| `function` | [`~PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a48b7da7a145125866e041c3d2cc7396c) `virtual` `inline` |  |
| `function` | [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a6762a9434263339d1a477e72de59704c) `virtual` | Emit a mutable raw buffer as a packet. |
| `function` | [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a797c0aa60e4999527e8efc3b2fdc88fd) `virtual` | Emit a read-only raw buffer as a packet (data is copied internally). |
| `function` | [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a4c605d524a538bfcc1cdf30ad4c22ce3) `virtual` | Emit a string as a packet (data is copied internally). |
| `function` | [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a10da09b2bf9b5b39c88e97813f99cbee) `virtual` | Emit a flag-only packet carrying no payload data. |
| `function` | [`emit`](base.md#classicy_1_1PacketStreamAdapter_1abb0394d938214165fc89e999e6937831) `virtual` | Emit an existing packet directly onto the outgoing signal. |
| `function` | [`getEmitter`](base.md#classicy_1_1PacketStreamAdapter_1a20cfe30f1db9ef46f9db060581fd2ff7)  | Returns a reference to the outgoing packet signal. |
| `function` | [`retention`](base.md#classicy_1_1PacketStreamAdapter_1a37a1aa11e35a20a7c527efa78e99e928) `virtual` `const` `nodiscard` | Returns how this adapter treats incoming packet lifetime. Most adapters are synchronous and therefore only borrow the packet for the current call chain. Queue-style adapters override this to advertise that they clone before deferred use. Callers may treat the first adapter reporting Cloned or Retained as the explicit ownership boundary in the stream graph. |
| `function` | [`onStreamStateChange`](base.md#classicy_1_1PacketStreamAdapter_1a86f0e2dc118359a83b8cbb996da6dc83) `virtual` `inline` | Called by the [PacketStream](base.md#packetstream) to notify when the internal [Stream](base.md#stream) state changes. On receiving the Stopped state, it is the responsibility of the adapter to have ceased all outgoing packet transmission, especially in multi-thread scenarios. |
| `function` | [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a5915b4a54729309a0fc12ee9c0107339)  | NonCopyable and NonMovable. |
| `function` | [`operator=`](base.md#classicy_1_1PacketStreamAdapter_1aeec4b3367101ffd07e3050cbfd726705)  | Deleted assignment operator. |
| `function` | [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1abf38bdf926762063adfd29427cad1362)  | Deleted constructor. |
| `function` | [`operator=`](base.md#classicy_1_1PacketStreamAdapter_1ac7fe6a98e57151ad909715f77e192481)  | Deleted assignment operator. |

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

##### Reimplements

- [`process`](base.md#classicy_1_1PacketProcessor_1acd9fb1a5fa525c0a2d96971fe6c06294)

---

{#accepts}

#### accepts

`virtual` `override`

```cpp
virtual bool accepts(IPacket * packet) override
```

Defined in src/av/include/icy/av/audiopacketencoder.h:59

Accept [AudioPacket](#audiopacket) and [PlanarAudioPacket](#planaraudiopacket) types.

##### Reimplements

- [`accepts`](base.md#classicy_1_1PacketProcessor_1a838d9d75f81bd943b40efcbe683394b7)

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

##### Reimplements

- [`onStreamStateChange`](base.md#classicy_1_1PacketStreamAdapter_1a86f0e2dc118359a83b8cbb996da6dc83)

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`PacketStream`](#packetstream-2) | `friend` | Declared here |
| [`VideoPacketEncoder`](#videopacketencoder-1) | `function` | Declared here |
| [`VideoPacketEncoder`](#videopacketencoder-2) | `function` | Declared here |
| [`process`](#process-3) | `function` | Declared here |
| [`accepts`](#accepts-1) | `function` | Declared here |
| [`_initialized`](#_initialized-1) | `variable` | Declared here |
| [`_mutex`](#_mutex-4) | `variable` | Declared here |
| [`onStreamStateChange`](#onstreamstatechange-3) | `function` | Declared here |
| [`format`](#format-7) | `variable` | Inherited from [`VideoEncoder`](#videoencoder) |
| [`VideoEncoder`](#videoencoder-1) | `function` | Inherited from [`VideoEncoder`](#videoencoder) |
| [`create`](#create-5) | `function` | Inherited from [`VideoEncoder`](#videoencoder) |
| [`close`](#close-8) | `function` | Inherited from [`VideoEncoder`](#videoencoder) |
| [`encode`](#encode-9) | `function` | Inherited from [`VideoEncoder`](#videoencoder) |
| [`encode`](#encode-10) | `function` | Inherited from [`VideoEncoder`](#videoencoder) |
| [`encode`](#encode-11) | `function` | Inherited from [`VideoEncoder`](#videoencoder) |
| [`flush`](#flush-6) | `function` | Inherited from [`VideoEncoder`](#videoencoder) |
| [`emitter`](#emitter-3) | `variable` | Inherited from [`VideoContext`](#videocontext) |
| [`iparams`](#iparams-1) | `variable` | Inherited from [`VideoContext`](#videocontext) |
| [`oparams`](#oparams-1) | `variable` | Inherited from [`VideoContext`](#videocontext) |
| [`stream`](#stream-2) | `variable` | Inherited from [`VideoContext`](#videocontext) |
| [`ctx`](#ctx-1) | `variable` | Inherited from [`VideoContext`](#videocontext) |
| [`codec`](#codec-6) | `variable` | Inherited from [`VideoContext`](#videocontext) |
| [`frame`](#frame-1) | `variable` | Inherited from [`VideoContext`](#videocontext) |
| [`conv`](#conv) | `variable` | Inherited from [`VideoContext`](#videocontext) |
| [`time`](#time-1) | `variable` | Inherited from [`VideoContext`](#videocontext) |
| [`pts`](#pts-1) | `variable` | Inherited from [`VideoContext`](#videocontext) |
| [`seconds`](#seconds-1) | `variable` | Inherited from [`VideoContext`](#videocontext) |
| [`error`](#error-3) | `variable` | Inherited from [`VideoContext`](#videocontext) |
| [`VideoContext`](#videocontext-1) | `function` | Inherited from [`VideoContext`](#videocontext) |
| [`VideoContext`](#videocontext-2) | `function` | Inherited from [`VideoContext`](#videocontext) |
| [`VideoContext`](#videocontext-3) | `function` | Inherited from [`VideoContext`](#videocontext) |
| [`create`](#create-3) | `function` | Inherited from [`VideoContext`](#videocontext) |
| [`open`](#open-1) | `function` | Inherited from [`VideoContext`](#videocontext) |
| [`close`](#close-6) | `function` | Inherited from [`VideoContext`](#videocontext) |
| [`decode`](#decode-2) | `function` | Inherited from [`VideoContext`](#videocontext) |
| [`encode`](#encode-6) | `function` | Inherited from [`VideoContext`](#videocontext) |
| [`encode`](#encode-7) | `function` | Inherited from [`VideoContext`](#videocontext) |
| [`encode`](#encode-8) | `function` | Inherited from [`VideoContext`](#videocontext) |
| [`flush`](#flush-4) | `function` | Inherited from [`VideoContext`](#videocontext) |
| [`convert`](#convert) | `function` | Inherited from [`VideoContext`](#videocontext) |
| [`recreateConverter`](#recreateconverter) | `function` | Inherited from [`VideoContext`](#videocontext) |
| [`PacketProcessor`](base.md#classicy_1_1PacketProcessor_1aa9a61f3eb4c53e7673b29d4c97686d9a) | `function` | Inherited from [`PacketProcessor`](base.md#packetprocessor) |
| [`process`](base.md#classicy_1_1PacketProcessor_1acd9fb1a5fa525c0a2d96971fe6c06294) | `function` | Inherited from [`PacketProcessor`](base.md#packetprocessor) |
| [`accepts`](base.md#classicy_1_1PacketProcessor_1a838d9d75f81bd943b40efcbe683394b7) | `function` | Inherited from [`PacketProcessor`](base.md#packetprocessor) |
| [`operator<<`](base.md#classicy_1_1PacketProcessor_1ae8908612aea63a6e5b3d3f7714a8e7a3) | `function` | Inherited from [`PacketProcessor`](base.md#packetprocessor) |
| [`_emitter`](base.md#classicy_1_1PacketStreamAdapter_1ad954a1cd2f20587a622abd9dcebb56cd) | `variable` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a904abfd9eb913c034a3648dc1fce3f57) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`~PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a48b7da7a145125866e041c3d2cc7396c) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a6762a9434263339d1a477e72de59704c) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a797c0aa60e4999527e8efc3b2fdc88fd) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a4c605d524a538bfcc1cdf30ad4c22ce3) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a10da09b2bf9b5b39c88e97813f99cbee) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`emit`](base.md#classicy_1_1PacketStreamAdapter_1abb0394d938214165fc89e999e6937831) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`getEmitter`](base.md#classicy_1_1PacketStreamAdapter_1a20cfe30f1db9ef46f9db060581fd2ff7) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`retention`](base.md#classicy_1_1PacketStreamAdapter_1a37a1aa11e35a20a7c527efa78e99e928) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`onStreamStateChange`](base.md#classicy_1_1PacketStreamAdapter_1a86f0e2dc118359a83b8cbb996da6dc83) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a5915b4a54729309a0fc12ee9c0107339) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`operator=`](base.md#classicy_1_1PacketStreamAdapter_1aeec4b3367101ffd07e3050cbfd726705) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1abf38bdf926762063adfd29427cad1362) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`operator=`](base.md#classicy_1_1PacketStreamAdapter_1ac7fe6a98e57151ad909715f77e192481) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |

### Inherited from [`VideoEncoder`](#videoencoder)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`format`](#format-7)  |  |
| `function` | [`VideoEncoder`](#videoencoder-1)  | Construct an encoder, optionally tied to an existing muxer context. |
| `function` | [`create`](#create-5) `virtual` `override` | Initialise the AVCodecContext using oparams. Adds a video stream to `format` if one was provided at construction. |
| `function` | [`close`](#close-8) `virtual` `override` | Close and free the AVCodecContext and associated resources. |
| `function` | [`encode`](#encode-9) `virtual` `nodiscard` `override` | Encode a single video frame. This method is for interleaved video formats. |
| `function` | [`encode`](#encode-10) `virtual` `nodiscard` `override` | Encode a single video frame. This method is for planar video formats. |
| `function` | [`encode`](#encode-11) `virtual` `nodiscard` `override` | Encode a single AVFrame (typically from a decoder or converter). |
| `function` | [`flush`](#flush-6) `virtual` `override` | Flush remaining packets to be encoded. This method should be called once before stream closure. |

### Inherited from [`VideoContext`](#videocontext)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`emitter`](#emitter-3)  |  |
| `variable` | [`iparams`](#iparams-1)  | input parameters |
| `variable` | [`oparams`](#oparams-1)  | output parameters |
| `variable` | [`stream`](#stream-2)  | encoder or decoder stream |
| `variable` | [`ctx`](#ctx-1)  | encoder or decoder context |
| `variable` | [`codec`](#codec-6)  | encoder or decoder codec |
| `variable` | [`frame`](#frame-1)  | encoder or decoder frame |
| `variable` | [`conv`](#conv)  | video conversion context |
| `variable` | [`time`](#time-1)  | stream time in codec time base |
| `variable` | [`pts`](#pts-1)  | last packet pts value |
| `variable` | [`seconds`](#seconds-1)  | video time in seconds |
| `variable` | [`error`](#error-3)  | error message |
| `function` | [`VideoContext`](#videocontext-1)  |  |
| `function` | [`VideoContext`](#videocontext-2)  | Deleted constructor. |
| `function` | [`VideoContext`](#videocontext-3)  | Deleted constructor. |
| `function` | [`create`](#create-3) `virtual` | Initialise the AVCodecContext with codec-specific defaults. Overridden by [VideoEncoder](#videoencoder) and [VideoDecoder](#videodecoder). |
| `function` | [`open`](#open-1) `virtual` | Open the codec and create the pixel format conversion context if required. Throws std::runtime_error if the codec context has not been created. |
| `function` | [`close`](#close-6) `virtual` | Close the codec context, free the frame, and reset timestamps. |
| `function` | [`decode`](#decode-2) `virtual` `nodiscard` | Decode a compressed video packet and emit the resulting frame. |
| `function` | [`encode`](#encode-6) `virtual` `nodiscard` | Encode a buffer of interleaved video data. |
| `function` | [`encode`](#encode-7) `virtual` `nodiscard` | Encode a planar video frame. |
| `function` | [`encode`](#encode-8) `virtual` `nodiscard` | Encode a single AVFrame. |
| `function` | [`flush`](#flush-4) `virtual` | Flush any frames buffered inside the codec and emit remaining output. |
| `function` | [`convert`](#convert) `virtual` | Convert the video frame and return the result. |
| `function` | [`recreateConverter`](#recreateconverter) `virtual` | Recreate the [VideoConverter](#videoconverter) if the input or output parameters have changed. Called automatically by [open()](#open-1) and [convert()](#convert). |

### Inherited from [`PacketProcessor`](base.md#packetprocessor)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`PacketProcessor`](base.md#classicy_1_1PacketProcessor_1aa9a61f3eb4c53e7673b29d4c97686d9a) `inline` |  |
| `function` | [`process`](base.md#classicy_1_1PacketProcessor_1acd9fb1a5fa525c0a2d96971fe6c06294) `virtual` | This method performs processing on the given packet and emits the result. |
| `function` | [`accepts`](base.md#classicy_1_1PacketProcessor_1a838d9d75f81bd943b40efcbe683394b7) `virtual` `inline` | This method ensures compatibility with the given packet type. Return false to reject the packet. |
| `function` | [`operator<<`](base.md#classicy_1_1PacketProcessor_1ae8908612aea63a6e5b3d3f7714a8e7a3) `virtual` `inline` | [Stream](base.md#stream) operator alias for [process()](base.md#classicy_1_1PacketProcessor_1acd9fb1a5fa525c0a2d96971fe6c06294). |

### Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`_emitter`](base.md#classicy_1_1PacketStreamAdapter_1ad954a1cd2f20587a622abd9dcebb56cd)  |  |
| `function` | [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a904abfd9eb913c034a3648dc1fce3f57)  | Construct the adapter, binding it to the given packet signal. |
| `function` | [`~PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a48b7da7a145125866e041c3d2cc7396c) `virtual` `inline` |  |
| `function` | [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a6762a9434263339d1a477e72de59704c) `virtual` | Emit a mutable raw buffer as a packet. |
| `function` | [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a797c0aa60e4999527e8efc3b2fdc88fd) `virtual` | Emit a read-only raw buffer as a packet (data is copied internally). |
| `function` | [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a4c605d524a538bfcc1cdf30ad4c22ce3) `virtual` | Emit a string as a packet (data is copied internally). |
| `function` | [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a10da09b2bf9b5b39c88e97813f99cbee) `virtual` | Emit a flag-only packet carrying no payload data. |
| `function` | [`emit`](base.md#classicy_1_1PacketStreamAdapter_1abb0394d938214165fc89e999e6937831) `virtual` | Emit an existing packet directly onto the outgoing signal. |
| `function` | [`getEmitter`](base.md#classicy_1_1PacketStreamAdapter_1a20cfe30f1db9ef46f9db060581fd2ff7)  | Returns a reference to the outgoing packet signal. |
| `function` | [`retention`](base.md#classicy_1_1PacketStreamAdapter_1a37a1aa11e35a20a7c527efa78e99e928) `virtual` `const` `nodiscard` | Returns how this adapter treats incoming packet lifetime. Most adapters are synchronous and therefore only borrow the packet for the current call chain. Queue-style adapters override this to advertise that they clone before deferred use. Callers may treat the first adapter reporting Cloned or Retained as the explicit ownership boundary in the stream graph. |
| `function` | [`onStreamStateChange`](base.md#classicy_1_1PacketStreamAdapter_1a86f0e2dc118359a83b8cbb996da6dc83) `virtual` `inline` | Called by the [PacketStream](base.md#packetstream) to notify when the internal [Stream](base.md#stream) state changes. On receiving the Stopped state, it is the responsibility of the adapter to have ceased all outgoing packet transmission, especially in multi-thread scenarios. |
| `function` | [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a5915b4a54729309a0fc12ee9c0107339)  | NonCopyable and NonMovable. |
| `function` | [`operator=`](base.md#classicy_1_1PacketStreamAdapter_1aeec4b3367101ffd07e3050cbfd726705)  | Deleted assignment operator. |
| `function` | [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1abf38bdf926762063adfd29427cad1362)  | Deleted constructor. |
| `function` | [`operator=`](base.md#classicy_1_1PacketStreamAdapter_1ac7fe6a98e57151ad909715f77e192481)  | Deleted assignment operator. |

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

##### Reimplements

- [`process`](base.md#classicy_1_1PacketProcessor_1acd9fb1a5fa525c0a2d96971fe6c06294)

---

{#accepts-1}

#### accepts

`virtual` `override`

```cpp
virtual bool accepts(IPacket * packet) override
```

Defined in src/av/include/icy/av/videopacketencoder.h:60

Accept [VideoPacket](#videopacket) and [PlanarVideoPacket](#planarvideopacket) types.

##### Reimplements

- [`accepts`](base.md#classicy_1_1PacketProcessor_1a838d9d75f81bd943b40efcbe683394b7)

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

##### Reimplements

- [`onStreamStateChange`](base.md#classicy_1_1PacketStreamAdapter_1a86f0e2dc118359a83b8cbb996da6dc83)

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`RealtimePacketQueue`](#realtimepacketqueue-1) | `function` | Declared here |
| [`push`](#push) | `function` | Declared here |
| [`realTime`](#realtime) | `function` | Declared here |
| [`_startTime`](#_starttime) | `variable` | Declared here |
| [`popNext`](#popnext) | `function` | Declared here |
| [`onStreamStateChange`](#onstreamstatechange-4) | `function` | Declared here |
| [`BaseQueue`](#basequeue) | `typedef` | Declared here |
| [`Queue`](base.md#classicy_1_1AsyncPacketQueue_1a3763065dd10e885cf0e2320bd4082fee) | `typedef` | Inherited from [`AsyncPacketQueue`](base.md#asyncpacketqueue) |
| [`Processor`](base.md#classicy_1_1AsyncPacketQueue_1adbccde7917d3283e9ee35cb1bb9b5b98) | `typedef` | Inherited from [`AsyncPacketQueue`](base.md#asyncpacketqueue) |
| [`emitter`](base.md#classicy_1_1AsyncPacketQueue_1abde947198221d9a966197bd5c78d9f8a) | `variable` | Inherited from [`AsyncPacketQueue`](base.md#asyncpacketqueue) |
| [`AsyncPacketQueue`](base.md#classicy_1_1AsyncPacketQueue_1aefce3e0be625f5be5747005d801bb44d) | `function` | Inherited from [`AsyncPacketQueue`](base.md#asyncpacketqueue) |
| [`~AsyncPacketQueue`](base.md#classicy_1_1AsyncPacketQueue_1ab7f1123ee0cff95c3c149f2ae5fd8598) | `function` | Inherited from [`AsyncPacketQueue`](base.md#asyncpacketqueue) |
| [`close`](base.md#classicy_1_1AsyncPacketQueue_1aaa410cd686acde18f9b4806664265509) | `function` | Inherited from [`AsyncPacketQueue`](base.md#asyncpacketqueue) |
| [`process`](base.md#classicy_1_1AsyncPacketQueue_1a5abef9a24f57c362b5dd4cca24763d0d) | `function` | Inherited from [`AsyncPacketQueue`](base.md#asyncpacketqueue) |
| [`accepts`](base.md#classicy_1_1AsyncPacketQueue_1aeba7d79e174d7dab6331b1e269bbdbf7) | `function` | Inherited from [`AsyncPacketQueue`](base.md#asyncpacketqueue) |
| [`retention`](base.md#classicy_1_1AsyncPacketQueue_1ab558156ee591962b33efe97c450cd281) | `function` | Inherited from [`AsyncPacketQueue`](base.md#asyncpacketqueue) |
| [`dispatch`](base.md#classicy_1_1AsyncPacketQueue_1a408c35d44bef57364c8c29d8ef71736a) | `function` | Inherited from [`AsyncPacketQueue`](base.md#asyncpacketqueue) |
| [`onStreamStateChange`](base.md#classicy_1_1AsyncPacketQueue_1af168c83b4468f735a0419fabe79b2cdd) | `function` | Inherited from [`AsyncPacketQueue`](base.md#asyncpacketqueue) |
| [`Queue`](base.md#classicy_1_1AsyncQueue_1a0efd9a7b5b236ed2b9f56ecdde9211a3) | `typedef` | Inherited from [`AsyncQueue`](base.md#asyncqueue) |
| [`_thread`](base.md#classicy_1_1AsyncQueue_1a37705714f84f4951fa2befc5a4d070c8) | `variable` | Inherited from [`AsyncQueue`](base.md#asyncqueue) |
| [`AsyncQueue`](base.md#classicy_1_1AsyncQueue_1a715382ae33e9191ff5fce23bf6880391) | `function` | Inherited from [`AsyncQueue`](base.md#asyncqueue) |
| [`cancel`](base.md#classicy_1_1AsyncQueue_1a23b99d0d77fb154c027fa717e7949840) | `function` | Inherited from [`AsyncQueue`](base.md#asyncqueue) |
| [`~AsyncQueue`](base.md#classicy_1_1AsyncQueue_1aa1523689e975842876d971d0083cccad) | `function` | Inherited from [`AsyncQueue`](base.md#asyncqueue) |
| [`ondispatch`](base.md#classicy_1_1RunnableQueue_1a02563ef55460453fd9e822af064316a6) | `variable` | Inherited from [`RunnableQueue`](base.md#runnablequeue) |
| [`_limit`](base.md#classicy_1_1RunnableQueue_1ad3f2479067668f92d6f07506337bfe32) | `variable` | Inherited from [`RunnableQueue`](base.md#runnablequeue) |
| [`_timeout`](base.md#classicy_1_1RunnableQueue_1a5da27c54589bb327589fd45ee56c5903) | `variable` | Inherited from [`RunnableQueue`](base.md#runnablequeue) |
| [`_dropped`](base.md#classicy_1_1RunnableQueue_1a62a495bba1ae7d7f3585619ca06a8949) | `variable` | Inherited from [`RunnableQueue`](base.md#runnablequeue) |
| [`RunnableQueue`](base.md#classicy_1_1RunnableQueue_1a22c19a890dfb624a9f5d31858a01fb97) | `function` | Inherited from [`RunnableQueue`](base.md#runnablequeue) |
| [`~RunnableQueue`](base.md#classicy_1_1RunnableQueue_1ae70216329e516b2c4b2bbba7019fc176) | `function` | Inherited from [`RunnableQueue`](base.md#runnablequeue) |
| [`push`](base.md#classicy_1_1RunnableQueue_1ab8e8d0837033ba51fe2166d109b69cf2) | `function` | Inherited from [`RunnableQueue`](base.md#runnablequeue) |
| [`flush`](base.md#classicy_1_1RunnableQueue_1aa64b944c8418d51a2ded9a24491e78af) | `function` | Inherited from [`RunnableQueue`](base.md#runnablequeue) |
| [`clear`](base.md#classicy_1_1RunnableQueue_1a188099034c08a78c8b48d3e9d1c68c25) | `function` | Inherited from [`RunnableQueue`](base.md#runnablequeue) |
| [`run`](base.md#classicy_1_1RunnableQueue_1a23ae7b15a81f939ea603d123d9aec2d5) | `function` | Inherited from [`RunnableQueue`](base.md#runnablequeue) |
| [`runTimeout`](base.md#classicy_1_1RunnableQueue_1a32e7d20cdbcd09993ba0c76830faf833) | `function` | Inherited from [`RunnableQueue`](base.md#runnablequeue) |
| [`dispatch`](base.md#classicy_1_1RunnableQueue_1a3c1108da9fee63978f063e7356ea9831) | `function` | Inherited from [`RunnableQueue`](base.md#runnablequeue) |
| [`timeout`](base.md#classicy_1_1RunnableQueue_1a5275ca6accec833cdf9b843fb0a60c06) | `function` | Inherited from [`RunnableQueue`](base.md#runnablequeue) |
| [`setTimeout`](base.md#classicy_1_1RunnableQueue_1a68e7e548b094964ee6054def7a44ce98) | `function` | Inherited from [`RunnableQueue`](base.md#runnablequeue) |
| [`dropped`](base.md#classicy_1_1RunnableQueue_1a66ee632ab0176617bacf4381b9b223dd) | `function` | Inherited from [`RunnableQueue`](base.md#runnablequeue) |
| [`RunnableQueue`](base.md#classicy_1_1RunnableQueue_1a48c9d6925977e89b420e3e80f8dbf6da) | `function` | Inherited from [`RunnableQueue`](base.md#runnablequeue) |
| [`operator=`](base.md#classicy_1_1RunnableQueue_1a1e020ab7cfabb36b6c9d3dc15d8f02de) | `function` | Inherited from [`RunnableQueue`](base.md#runnablequeue) |
| [`RunnableQueue`](base.md#classicy_1_1RunnableQueue_1a5429d61def63e54d3c8162eacdca71ed) | `function` | Inherited from [`RunnableQueue`](base.md#runnablequeue) |
| [`operator=`](base.md#classicy_1_1RunnableQueue_1a0b1f772d9fcdce516393df33b38ecb10) | `function` | Inherited from [`RunnableQueue`](base.md#runnablequeue) |
| [`popNext`](base.md#classicy_1_1RunnableQueue_1a56c74bc69705754b8670f60972b6e5a6) | `function` | Inherited from [`RunnableQueue`](base.md#runnablequeue) |
| [`dispatchNext`](base.md#classicy_1_1RunnableQueue_1a48df29dc62d24f471c445d77bdff4d70) | `function` | Inherited from [`RunnableQueue`](base.md#runnablequeue) |
| [`_queue`](base.md#classicy_1_1Queue_1ae99eb1f829857ecdc75119fa1291ab80) | `variable` | Inherited from [`Queue`](base.md#queue) |
| [`_mutex`](base.md#classicy_1_1Queue_1a8910d3c08198c72787af4a03054891b1) | `variable` | Inherited from [`Queue`](base.md#queue) |
| [`push`](base.md#classicy_1_1Queue_1ad76910b55b4c648794fd755a22317c15) | `function` | Inherited from [`Queue`](base.md#queue) |
| [`push`](base.md#classicy_1_1Queue_1a76cd03a71cdf41ea458e93079d8006a0) | `function` | Inherited from [`Queue`](base.md#queue) |
| [`empty`](base.md#classicy_1_1Queue_1abe8f5d6e2a4247cf5994b13cdfe7a5c6) | `function` | Inherited from [`Queue`](base.md#queue) |
| [`front`](base.md#classicy_1_1Queue_1a2461185a30f8ce5987787e28d94a98bf) | `function` | Inherited from [`Queue`](base.md#queue) |
| [`back`](base.md#classicy_1_1Queue_1a80fce4e8936f5b74f036549801bcd123) | `function` | Inherited from [`Queue`](base.md#queue) |
| [`pop`](base.md#classicy_1_1Queue_1a43400b817e9ffe6ac3109c720c674c68) | `function` | Inherited from [`Queue`](base.md#queue) |
| [`sort`](base.md#classicy_1_1Queue_1a5634cd774ad88850416d35995b5779eb) | `function` | Inherited from [`Queue`](base.md#queue) |
| [`size`](base.md#classicy_1_1Queue_1a5532fcef4a4a3ccdeea15924fda51a9a) | `function` | Inherited from [`Queue`](base.md#queue) |
| [`queue`](base.md#classicy_1_1Queue_1a6d1ad3ee070f37e25304cfde94494947) | `function` | Inherited from [`Queue`](base.md#queue) |
| [`Runnable`](base.md#runnable-1) | `function` | Inherited from [`Runnable`](base.md#runnable) |
| [`run`](base.md#run-4) | `function` | Inherited from [`Runnable`](base.md#runnable) |
| [`cancel`](base.md#cancel-1) | `function` | Inherited from [`Runnable`](base.md#runnable) |
| [`cancelled`](base.md#cancelled) | `function` | Inherited from [`Runnable`](base.md#runnable) |
| [`exit`](base.md#exit) | `variable` | Inherited from [`Runnable`](base.md#runnable) |
| [`PacketProcessor`](base.md#classicy_1_1PacketProcessor_1aa9a61f3eb4c53e7673b29d4c97686d9a) | `function` | Inherited from [`PacketProcessor`](base.md#packetprocessor) |
| [`process`](base.md#classicy_1_1PacketProcessor_1acd9fb1a5fa525c0a2d96971fe6c06294) | `function` | Inherited from [`PacketProcessor`](base.md#packetprocessor) |
| [`accepts`](base.md#classicy_1_1PacketProcessor_1a838d9d75f81bd943b40efcbe683394b7) | `function` | Inherited from [`PacketProcessor`](base.md#packetprocessor) |
| [`operator<<`](base.md#classicy_1_1PacketProcessor_1ae8908612aea63a6e5b3d3f7714a8e7a3) | `function` | Inherited from [`PacketProcessor`](base.md#packetprocessor) |
| [`_emitter`](base.md#classicy_1_1PacketStreamAdapter_1ad954a1cd2f20587a622abd9dcebb56cd) | `variable` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a904abfd9eb913c034a3648dc1fce3f57) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`~PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a48b7da7a145125866e041c3d2cc7396c) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a6762a9434263339d1a477e72de59704c) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a797c0aa60e4999527e8efc3b2fdc88fd) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a4c605d524a538bfcc1cdf30ad4c22ce3) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a10da09b2bf9b5b39c88e97813f99cbee) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`emit`](base.md#classicy_1_1PacketStreamAdapter_1abb0394d938214165fc89e999e6937831) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`getEmitter`](base.md#classicy_1_1PacketStreamAdapter_1a20cfe30f1db9ef46f9db060581fd2ff7) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`retention`](base.md#classicy_1_1PacketStreamAdapter_1a37a1aa11e35a20a7c527efa78e99e928) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`onStreamStateChange`](base.md#classicy_1_1PacketStreamAdapter_1a86f0e2dc118359a83b8cbb996da6dc83) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a5915b4a54729309a0fc12ee9c0107339) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`operator=`](base.md#classicy_1_1PacketStreamAdapter_1aeec4b3367101ffd07e3050cbfd726705) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1abf38bdf926762063adfd29427cad1362) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`operator=`](base.md#classicy_1_1PacketStreamAdapter_1ac7fe6a98e57151ad909715f77e192481) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |

### Inherited from [`AsyncPacketQueue`](base.md#asyncpacketqueue)

| Kind | Name | Description |
|------|------|-------------|
| `typedef` | [`Queue`](base.md#classicy_1_1AsyncPacketQueue_1a3763065dd10e885cf0e2320bd4082fee)  |  |
| `typedef` | [`Processor`](base.md#classicy_1_1AsyncPacketQueue_1adbccde7917d3283e9ee35cb1bb9b5b98)  |  |
| `variable` | [`emitter`](base.md#classicy_1_1AsyncPacketQueue_1abde947198221d9a966197bd5c78d9f8a)  |  |
| `function` | [`AsyncPacketQueue`](base.md#classicy_1_1AsyncPacketQueue_1aefce3e0be625f5be5747005d801bb44d) `inline` |  |
| `function` | [`~AsyncPacketQueue`](base.md#classicy_1_1AsyncPacketQueue_1ab7f1123ee0cff95c3c149f2ae5fd8598) `virtual` `inline` |  |
| `function` | [`close`](base.md#classicy_1_1AsyncPacketQueue_1aaa410cd686acde18f9b4806664265509) `virtual` `inline` | Flushes remaining packets, cancels the queue, and joins the dispatch thread. |
| `function` | [`process`](base.md#classicy_1_1AsyncPacketQueue_1a5abef9a24f57c362b5dd4cca24763d0d) `virtual` `inline` `override` | Clones the incoming packet and pushes it onto the async queue. This queue is therefore an explicit [PacketStream](base.md#packetstream) ownership boundary. Drops the packet with a warning if the queue has been cancelled. |
| `function` | [`accepts`](base.md#classicy_1_1AsyncPacketQueue_1aeba7d79e174d7dab6331b1e269bbdbf7) `virtual` `inline` `override` | Returns true if the packet can be cast to type `T`. |
| `function` | [`retention`](base.md#classicy_1_1AsyncPacketQueue_1ab558156ee591962b33efe97c450cd281) `virtual` `const` `inline` `nodiscard` `override` | Returns how this adapter treats incoming packet lifetime. Most adapters are synchronous and therefore only borrow the packet for the current call chain. Queue-style adapters override this to advertise that they clone before deferred use. Callers may treat the first adapter reporting Cloned or Retained as the explicit ownership boundary in the stream graph. |
| `function` | [`dispatch`](base.md#classicy_1_1AsyncPacketQueue_1a408c35d44bef57364c8c29d8ef71736a) `inline` `override` | Emits the packet to downstream processors from the async thread. |
| `function` | [`onStreamStateChange`](base.md#classicy_1_1AsyncPacketQueue_1af168c83b4468f735a0419fabe79b2cdd) `virtual` `inline` `override` | Closes the queue on `[Error](base.md#error)` or `Closed` stream state transitions. |

### Inherited from [`AsyncQueue`](base.md#asyncqueue)

| Kind | Name | Description |
|------|------|-------------|
| `typedef` | [`Queue`](base.md#classicy_1_1AsyncQueue_1a0efd9a7b5b236ed2b9f56ecdde9211a3)  |  |
| `variable` | [`_thread`](base.md#classicy_1_1AsyncQueue_1a37705714f84f4951fa2befc5a4d070c8)  |  |
| `function` | [`AsyncQueue`](base.md#classicy_1_1AsyncQueue_1a715382ae33e9191ff5fce23bf6880391) `inline` |  |
| `function` | [`cancel`](base.md#classicy_1_1AsyncQueue_1a23b99d0d77fb154c027fa717e7949840) `virtual` `inline` `override` | Cancels the queue and joins the dispatch thread. |
| `function` | [`~AsyncQueue`](base.md#classicy_1_1AsyncQueue_1aa1523689e975842876d971d0083cccad) `virtual` `inline` |  |

### Inherited from [`RunnableQueue`](base.md#runnablequeue)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`ondispatch`](base.md#classicy_1_1RunnableQueue_1a02563ef55460453fd9e822af064316a6)  | The default dispatch function. Must be set before the queue is running. |
| `variable` | [`_limit`](base.md#classicy_1_1RunnableQueue_1ad3f2479067668f92d6f07506337bfe32)  |  |
| `variable` | [`_timeout`](base.md#classicy_1_1RunnableQueue_1a5da27c54589bb327589fd45ee56c5903)  |  |
| `variable` | [`_dropped`](base.md#classicy_1_1RunnableQueue_1a62a495bba1ae7d7f3585619ca06a8949)  |  |
| `function` | [`RunnableQueue`](base.md#classicy_1_1RunnableQueue_1a22c19a890dfb624a9f5d31858a01fb97) `inline` |  |
| `function` | [`~RunnableQueue`](base.md#classicy_1_1RunnableQueue_1ae70216329e516b2c4b2bbba7019fc176) `virtual` `inline` |  |
| `function` | [`push`](base.md#classicy_1_1RunnableQueue_1ab8e8d0837033ba51fe2166d109b69cf2) `virtual` `inline` | Push an item onto the queue. The queue takes ownership of the item pointer. |
| `function` | [`flush`](base.md#classicy_1_1RunnableQueue_1aa64b944c8418d51a2ded9a24491e78af) `virtual` `inline` | Flush all outgoing items. |
| `function` | [`clear`](base.md#classicy_1_1RunnableQueue_1a188099034c08a78c8b48d3e9d1c68c25) `inline` |  |
| `function` | [`run`](base.md#classicy_1_1RunnableQueue_1a23ae7b15a81f939ea603d123d9aec2d5) `virtual` `inline` `override` | Called asynchronously to dispatch queued items. If not timeout is set this method blocks until [cancel()](base.md#cancel-1) is called, otherwise [runTimeout()](base.md#classicy_1_1RunnableQueue_1a32e7d20cdbcd09993ba0c76830faf833) will be called. |
| `function` | [`runTimeout`](base.md#classicy_1_1RunnableQueue_1a32e7d20cdbcd09993ba0c76830faf833) `virtual` `inline` | Called asynchronously to dispatch queued items until the queue is empty or the timeout expires. Pseudo protected for std::bind compatability. |
| `function` | [`dispatch`](base.md#classicy_1_1RunnableQueue_1a3c1108da9fee63978f063e7356ea9831) `virtual` `inline` | Dispatch a single item to listeners. |
| `function` | [`timeout`](base.md#classicy_1_1RunnableQueue_1a5275ca6accec833cdf9b843fb0a60c06) `inline` |  |
| `function` | [`setTimeout`](base.md#classicy_1_1RunnableQueue_1a68e7e548b094964ee6054def7a44ce98) `inline` | Sets the dispatch timeout. Must only be called when the queue is empty. |
| `function` | [`dropped`](base.md#classicy_1_1RunnableQueue_1a66ee632ab0176617bacf4381b9b223dd) `const` `inline` |  |
| `function` | [`RunnableQueue`](base.md#classicy_1_1RunnableQueue_1a48c9d6925977e89b420e3e80f8dbf6da)  | Deleted constructor. |
| `function` | [`operator=`](base.md#classicy_1_1RunnableQueue_1a1e020ab7cfabb36b6c9d3dc15d8f02de)  | Deleted assignment operator. |
| `function` | [`RunnableQueue`](base.md#classicy_1_1RunnableQueue_1a5429d61def63e54d3c8162eacdca71ed)  | Deleted constructor. |
| `function` | [`operator=`](base.md#classicy_1_1RunnableQueue_1a0b1f772d9fcdce516393df33b38ecb10)  | Deleted assignment operator. |
| `function` | [`popNext`](base.md#classicy_1_1RunnableQueue_1a56c74bc69705754b8670f60972b6e5a6) `virtual` `inline` | Pops the next waiting item. |
| `function` | [`dispatchNext`](base.md#classicy_1_1RunnableQueue_1a48df29dc62d24f471c445d77bdff4d70) `virtual` `inline` | Pops and dispatches the next waiting item. |

### Inherited from [`Queue`](base.md#queue)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`_queue`](base.md#classicy_1_1Queue_1ae99eb1f829857ecdc75119fa1291ab80)  |  |
| `variable` | [`_mutex`](base.md#classicy_1_1Queue_1a8910d3c08198c72787af4a03054891b1)  |  |
| `function` | [`push`](base.md#classicy_1_1Queue_1ad76910b55b4c648794fd755a22317c15) `inline` | Appends an item to the back of the queue (thread-safe). |
| `function` | [`push`](base.md#classicy_1_1Queue_1a76cd03a71cdf41ea458e93079d8006a0) `inline` | Appends an item to the back of the queue by move (thread-safe). |
| `function` | [`empty`](base.md#classicy_1_1Queue_1abe8f5d6e2a4247cf5994b13cdfe7a5c6) `const` `inline` |  |
| `function` | [`front`](base.md#classicy_1_1Queue_1a2461185a30f8ce5987787e28d94a98bf) `const` `inline` |  |
| `function` | [`back`](base.md#classicy_1_1Queue_1a80fce4e8936f5b74f036549801bcd123) `const` `inline` |  |
| `function` | [`pop`](base.md#classicy_1_1Queue_1a43400b817e9ffe6ac3109c720c674c68) `inline` | Removes the front item from the queue (thread-safe). |
| `function` | [`sort`](base.md#classicy_1_1Queue_1a5634cd774ad88850416d35995b5779eb) `inline` | Sorts all queued items using the given comparator (thread-safe). |
| `function` | [`size`](base.md#classicy_1_1Queue_1a5532fcef4a4a3ccdeea15924fda51a9a) `const` `inline` |  |
| `function` | [`queue`](base.md#classicy_1_1Queue_1a6d1ad3ee070f37e25304cfde94494947) `const` `inline` |  |

### Inherited from [`Runnable`](base.md#runnable)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`Runnable`](base.md#runnable-1) `inline` |  |
| `function` | [`run`](base.md#run-4) `virtual` | The run method will be called by the asynchronous context. |
| `function` | [`cancel`](base.md#cancel-1) `virtual` `inline` | Cancel the current task. The [run()](base.md#run-4) method should return ASAP. |
| `function` | [`cancelled`](base.md#cancelled) `virtual` `const` `inline` | Returns true when the task has been cancelled. |
| `variable` | [`exit`](base.md#exit)  |  |

### Inherited from [`PacketProcessor`](base.md#packetprocessor)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`PacketProcessor`](base.md#classicy_1_1PacketProcessor_1aa9a61f3eb4c53e7673b29d4c97686d9a) `inline` |  |
| `function` | [`process`](base.md#classicy_1_1PacketProcessor_1acd9fb1a5fa525c0a2d96971fe6c06294) `virtual` | This method performs processing on the given packet and emits the result. |
| `function` | [`accepts`](base.md#classicy_1_1PacketProcessor_1a838d9d75f81bd943b40efcbe683394b7) `virtual` `inline` | This method ensures compatibility with the given packet type. Return false to reject the packet. |
| `function` | [`operator<<`](base.md#classicy_1_1PacketProcessor_1ae8908612aea63a6e5b3d3f7714a8e7a3) `virtual` `inline` | [Stream](base.md#stream) operator alias for [process()](base.md#classicy_1_1PacketProcessor_1acd9fb1a5fa525c0a2d96971fe6c06294). |

### Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`_emitter`](base.md#classicy_1_1PacketStreamAdapter_1ad954a1cd2f20587a622abd9dcebb56cd)  |  |
| `function` | [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a904abfd9eb913c034a3648dc1fce3f57)  | Construct the adapter, binding it to the given packet signal. |
| `function` | [`~PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a48b7da7a145125866e041c3d2cc7396c) `virtual` `inline` |  |
| `function` | [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a6762a9434263339d1a477e72de59704c) `virtual` | Emit a mutable raw buffer as a packet. |
| `function` | [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a797c0aa60e4999527e8efc3b2fdc88fd) `virtual` | Emit a read-only raw buffer as a packet (data is copied internally). |
| `function` | [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a4c605d524a538bfcc1cdf30ad4c22ce3) `virtual` | Emit a string as a packet (data is copied internally). |
| `function` | [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a10da09b2bf9b5b39c88e97813f99cbee) `virtual` | Emit a flag-only packet carrying no payload data. |
| `function` | [`emit`](base.md#classicy_1_1PacketStreamAdapter_1abb0394d938214165fc89e999e6937831) `virtual` | Emit an existing packet directly onto the outgoing signal. |
| `function` | [`getEmitter`](base.md#classicy_1_1PacketStreamAdapter_1a20cfe30f1db9ef46f9db060581fd2ff7)  | Returns a reference to the outgoing packet signal. |
| `function` | [`retention`](base.md#classicy_1_1PacketStreamAdapter_1a37a1aa11e35a20a7c527efa78e99e928) `virtual` `const` `nodiscard` | Returns how this adapter treats incoming packet lifetime. Most adapters are synchronous and therefore only borrow the packet for the current call chain. Queue-style adapters override this to advertise that they clone before deferred use. Callers may treat the first adapter reporting Cloned or Retained as the explicit ownership boundary in the stream graph. |
| `function` | [`onStreamStateChange`](base.md#classicy_1_1PacketStreamAdapter_1a86f0e2dc118359a83b8cbb996da6dc83) `virtual` `inline` | Called by the [PacketStream](base.md#packetstream) to notify when the internal [Stream](base.md#stream) state changes. On receiving the Stopped state, it is the responsibility of the adapter to have ceased all outgoing packet transmission, especially in multi-thread scenarios. |
| `function` | [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a5915b4a54729309a0fc12ee9c0107339)  | NonCopyable and NonMovable. |
| `function` | [`operator=`](base.md#classicy_1_1PacketStreamAdapter_1aeec4b3367101ffd07e3050cbfd726705)  | Deleted assignment operator. |
| `function` | [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1abf38bdf926762063adfd29427cad1362)  | Deleted constructor. |
| `function` | [`operator=`](base.md#classicy_1_1PacketStreamAdapter_1ac7fe6a98e57151ad909715f77e192481)  | Deleted assignment operator. |

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

##### Reimplements

- [`popNext`](base.md#classicy_1_1RunnableQueue_1a56c74bc69705754b8670f60972b6e5a6)

---

{#onstreamstatechange-4}

#### onStreamStateChange

`virtual` `inline` `override`

```cpp
virtual inline void onStreamStateChange(const PacketStreamState & state) override
```

Defined in src/av/include/icy/av/realtimepacketqueue.h:78

Record the stream start time when the stream becomes active.

##### Reimplements

- [`onStreamStateChange`](base.md#classicy_1_1AsyncPacketQueue_1af168c83b4468f735a0419fabe79b2cdd)

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`operator()`](#operator-3) | `function` | Declared here |

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`PacketStream`](#packetstream-3) | `friend` | Declared here |
| [`MultiplexPacketEncoder`](#multiplexpacketencoder-1) | `function` | Declared here |
| [`MultiplexPacketEncoder`](#multiplexpacketencoder-2) | `function` | Declared here |
| [`MultiplexPacketEncoder`](#multiplexpacketencoder-3) | `function` | Declared here |
| [`encode`](#encode-12) | `function` | Declared here |
| [`encode`](#encode-13) | `function` | Declared here |
| [`accepts`](#accepts-2) | `function` | Declared here |
| [`process`](#process-4) | `function` | Declared here |
| [`_mutex`](#_mutex-5) | `variable` | Declared here |
| [`onStreamStateChange`](#onstreamstatechange-5) | `function` | Declared here |
| [`emitter`](#emitter-4) | `variable` | Inherited from [`MultiplexEncoder`](#multiplexencoder) |
| [`MultiplexEncoder`](#multiplexencoder-1) | `function` | Inherited from [`MultiplexEncoder`](#multiplexencoder) |
| [`MultiplexEncoder`](#multiplexencoder-2) | `function` | Inherited from [`MultiplexEncoder`](#multiplexencoder) |
| [`MultiplexEncoder`](#multiplexencoder-3) | `function` | Inherited from [`MultiplexEncoder`](#multiplexencoder) |
| [`init`](#init-1) | `function` | Inherited from [`MultiplexEncoder`](#multiplexencoder) |
| [`uninit`](#uninit-1) | `function` | Inherited from [`MultiplexEncoder`](#multiplexencoder) |
| [`cleanup`](#cleanup-1) | `function` | Inherited from [`MultiplexEncoder`](#multiplexencoder) |
| [`createVideo`](#createvideo-1) | `function` | Inherited from [`MultiplexEncoder`](#multiplexencoder) |
| [`freeVideo`](#freevideo-1) | `function` | Inherited from [`MultiplexEncoder`](#multiplexencoder) |
| [`encodeVideo`](#encodevideo-1) | `function` | Inherited from [`MultiplexEncoder`](#multiplexencoder) |
| [`encodeVideo`](#encodevideo-2) | `function` | Inherited from [`MultiplexEncoder`](#multiplexencoder) |
| [`encodeVideo`](#encodevideo-3) | `function` | Inherited from [`MultiplexEncoder`](#multiplexencoder) |
| [`createAudio`](#createaudio-1) | `function` | Inherited from [`MultiplexEncoder`](#multiplexencoder) |
| [`freeAudio`](#freeaudio-1) | `function` | Inherited from [`MultiplexEncoder`](#multiplexencoder) |
| [`encodeAudio`](#encodeaudio) | `function` | Inherited from [`MultiplexEncoder`](#multiplexencoder) |
| [`encodeAudio`](#encodeaudio-1) | `function` | Inherited from [`MultiplexEncoder`](#multiplexencoder) |
| [`flush`](#flush-7) | `function` | Inherited from [`MultiplexEncoder`](#multiplexencoder) |
| [`options`](#options-2) | `function` | Inherited from [`MultiplexEncoder`](#multiplexencoder) |
| [`video`](#video-2) | `function` | Inherited from [`MultiplexEncoder`](#multiplexencoder) |
| [`audio`](#audio-2) | `function` | Inherited from [`MultiplexEncoder`](#multiplexencoder) |
| [`_options`](#_options) | `variable` | Inherited from [`MultiplexEncoder`](#multiplexencoder) |
| [`_formatCtx`](#_formatctx-1) | `variable` | Inherited from [`MultiplexEncoder`](#multiplexencoder) |
| [`_video`](#_video-1) | `variable` | Inherited from [`MultiplexEncoder`](#multiplexencoder) |
| [`_audio`](#_audio-1) | `variable` | Inherited from [`MultiplexEncoder`](#multiplexencoder) |
| [`_ioCtx`](#_ioctx) | `variable` | Inherited from [`MultiplexEncoder`](#multiplexencoder) |
| [`_ioBuffer`](#_iobuffer) | `variable` | Inherited from [`MultiplexEncoder`](#multiplexencoder) |
| [`_pts`](#_pts) | `variable` | Inherited from [`MultiplexEncoder`](#multiplexencoder) |
| [`_mutex`](#_mutex-2) | `variable` | Inherited from [`MultiplexEncoder`](#multiplexencoder) |
| [`writeOutputPacket`](#writeoutputpacket) | `function` | Inherited from [`MultiplexEncoder`](#multiplexencoder) |
| [`updateStreamPts`](#updatestreampts) | `function` | Inherited from [`MultiplexEncoder`](#multiplexencoder) |
| [`onVideoEncoded`](#onvideoencoded) | `function` | Inherited from [`MultiplexEncoder`](#multiplexencoder) |
| [`onAudioEncoded`](#onaudioencoded) | `function` | Inherited from [`MultiplexEncoder`](#multiplexencoder) |
| [`init`](#init) | `function` | Inherited from [`IEncoder`](#iencoder) |
| [`uninit`](#uninit) | `function` | Inherited from [`IEncoder`](#iencoder) |
| [`cleanup`](#cleanup) | `function` | Inherited from [`IEncoder`](#iencoder) |
| [`options`](#options-1) | `function` | Inherited from [`IEncoder`](#iencoder) |
| [`createVideo`](#createvideo) | `function` | Inherited from [`IEncoder`](#iencoder) |
| [`freeVideo`](#freevideo) | `function` | Inherited from [`IEncoder`](#iencoder) |
| [`encodeVideo`](#encodevideo) | `function` | Inherited from [`IEncoder`](#iencoder) |
| [`createAudio`](#createaudio) | `function` | Inherited from [`IEncoder`](#iencoder) |
| [`freeAudio`](#freeaudio) | `function` | Inherited from [`IEncoder`](#iencoder) |
| [`flush`](#flush) | `function` | Inherited from [`IEncoder`](#iencoder) |
| [`isNone`](#isnone) | `function` | Inherited from [`IEncoder`](#iencoder) |
| [`isReady`](#isready) | `function` | Inherited from [`IEncoder`](#iencoder) |
| [`isEncoding`](#isencoding) | `function` | Inherited from [`IEncoder`](#iencoder) |
| [`isActive`](#isactive) | `function` | Inherited from [`IEncoder`](#iencoder) |
| [`isStopped`](#isstopped) | `function` | Inherited from [`IEncoder`](#iencoder) |
| [`isError`](#iserror) | `function` | Inherited from [`IEncoder`](#iencoder) |
| [`Type`](#type-3) | `enum` | Inherited from [`IEncoder`](#iencoder) |
| [`StateChange`](base.md#classicy_1_1Stateful_1adba38e2b8a737448d48fc56b4bbe1dc0) | `variable` | Inherited from [`Stateful`](base.md#stateful) |
| [`_state`](base.md#classicy_1_1Stateful_1a45326923d41e72e4afc70d9ed8acc15d) | `variable` | Inherited from [`Stateful`](base.md#stateful) |
| [`Stateful`](base.md#classicy_1_1Stateful_1a444344bc5f8563af4d289be6beb693a0) | `function` | Inherited from [`Stateful`](base.md#stateful) |
| [`~Stateful`](base.md#classicy_1_1Stateful_1a70818a671339a0468ef71e6d54c97c77) | `function` | Inherited from [`Stateful`](base.md#stateful) |
| [`stateEquals`](base.md#classicy_1_1Stateful_1a8c8bd62dbd4251749150ed7f95bdb875) | `function` | Inherited from [`Stateful`](base.md#stateful) |
| [`stateBetween`](base.md#classicy_1_1Stateful_1a702bf6338c45b6ce75540a2d7f03a18f) | `function` | Inherited from [`Stateful`](base.md#stateful) |
| [`state`](base.md#classicy_1_1Stateful_1ae1082bd323b27b8eb1022f160d77a6e6) | `function` | Inherited from [`Stateful`](base.md#stateful) |
| [`state`](base.md#classicy_1_1Stateful_1ab63e2709491c8fb841f7dd5720c15020) | `function` | Inherited from [`Stateful`](base.md#stateful) |
| [`beforeStateChange`](base.md#classicy_1_1Stateful_1a0ae9d2650c9a6fdb276b88e0b2d3ca34) | `function` | Inherited from [`Stateful`](base.md#stateful) |
| [`onStateChange`](base.md#classicy_1_1Stateful_1a06e5572c313c3b54db932c922166079d) | `function` | Inherited from [`Stateful`](base.md#stateful) |
| [`setState`](base.md#classicy_1_1Stateful_1a88cfa69863f505b8232c7cce5989a395) | `function` | Inherited from [`Stateful`](base.md#stateful) |
| [`setState`](base.md#classicy_1_1Stateful_1a9d5c3013be4fb7508266ed0b950694aa) | `function` | Inherited from [`Stateful`](base.md#stateful) |
| [`PacketProcessor`](base.md#classicy_1_1PacketProcessor_1aa9a61f3eb4c53e7673b29d4c97686d9a) | `function` | Inherited from [`PacketProcessor`](base.md#packetprocessor) |
| [`process`](base.md#classicy_1_1PacketProcessor_1acd9fb1a5fa525c0a2d96971fe6c06294) | `function` | Inherited from [`PacketProcessor`](base.md#packetprocessor) |
| [`accepts`](base.md#classicy_1_1PacketProcessor_1a838d9d75f81bd943b40efcbe683394b7) | `function` | Inherited from [`PacketProcessor`](base.md#packetprocessor) |
| [`operator<<`](base.md#classicy_1_1PacketProcessor_1ae8908612aea63a6e5b3d3f7714a8e7a3) | `function` | Inherited from [`PacketProcessor`](base.md#packetprocessor) |
| [`_emitter`](base.md#classicy_1_1PacketStreamAdapter_1ad954a1cd2f20587a622abd9dcebb56cd) | `variable` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a904abfd9eb913c034a3648dc1fce3f57) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`~PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a48b7da7a145125866e041c3d2cc7396c) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a6762a9434263339d1a477e72de59704c) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a797c0aa60e4999527e8efc3b2fdc88fd) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a4c605d524a538bfcc1cdf30ad4c22ce3) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a10da09b2bf9b5b39c88e97813f99cbee) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`emit`](base.md#classicy_1_1PacketStreamAdapter_1abb0394d938214165fc89e999e6937831) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`getEmitter`](base.md#classicy_1_1PacketStreamAdapter_1a20cfe30f1db9ef46f9db060581fd2ff7) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`retention`](base.md#classicy_1_1PacketStreamAdapter_1a37a1aa11e35a20a7c527efa78e99e928) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`onStreamStateChange`](base.md#classicy_1_1PacketStreamAdapter_1a86f0e2dc118359a83b8cbb996da6dc83) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a5915b4a54729309a0fc12ee9c0107339) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`operator=`](base.md#classicy_1_1PacketStreamAdapter_1aeec4b3367101ffd07e3050cbfd726705) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1abf38bdf926762063adfd29427cad1362) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |
| [`operator=`](base.md#classicy_1_1PacketStreamAdapter_1ac7fe6a98e57151ad909715f77e192481) | `function` | Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter) |

### Inherited from [`MultiplexEncoder`](#multiplexencoder)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`emitter`](#emitter-4)  |  |
| `function` | [`MultiplexEncoder`](#multiplexencoder-1)  | Construct the encoder with the given options. |
| `function` | [`MultiplexEncoder`](#multiplexencoder-2)  | Deleted constructor. |
| `function` | [`MultiplexEncoder`](#multiplexencoder-3)  | Deleted constructor. |
| `function` | [`init`](#init-1) `virtual` `override` | Open the output container, create codec streams, and write the format header. |
| `function` | [`uninit`](#uninit-1) `virtual` `override` | Flush encoded packets, write the format trailer, and close the output container. |
| `function` | [`cleanup`](#cleanup-1) `virtual` `override` | Release all resources allocated by [init()](#init-1) without writing a trailer. |
| `function` | [`createVideo`](#createvideo-1) `virtual` `override` | Create the video encoder and add the stream to the output container. |
| `function` | [`freeVideo`](#freevideo-1) `virtual` `override` | Flush and free the video encoder and its stream. |
| `function` | [`encodeVideo`](#encodevideo-1) `virtual` `nodiscard` `override` | Encode a single video frame. All frame values must be set, such as size, pixel format, and PTS. |
| `function` | [`encodeVideo`](#encodevideo-2) `virtual` `nodiscard` | Encode a single interleaved video frame. If the frame time is specified it should be the microseconds offset since the start of the input stream. If no time is specified a real-time value will be assigned to the frame. |
| `function` | [`encodeVideo`](#encodevideo-3) `virtual` `nodiscard` | Encode a single planar video frame. |
| `function` | [`createAudio`](#createaudio-1) `virtual` `override` | Create the audio encoder and add the stream to the output container. |
| `function` | [`freeAudio`](#freeaudio-1) `virtual` `override` | Flush and free the audio encoder and its stream. |
| `function` | [`encodeAudio`](#encodeaudio) `virtual` `nodiscard` | Encode a single interleaved audio frame. |
| `function` | [`encodeAudio`](#encodeaudio-1) `virtual` `nodiscard` | Encode a single planar audio frame. |
| `function` | [`flush`](#flush-7) `virtual` `override` | Flush any buffered or queued packets to the output container. |
| `function` | [`options`](#options-2) `virtual` `const` `nodiscard` `override` |  |
| `function` | [`video`](#video-2)  |  |
| `function` | [`audio`](#audio-2)  |  |
| `variable` | [`_options`](#_options)  |  |
| `variable` | [`_formatCtx`](#_formatctx-1)  |  |
| `variable` | [`_video`](#_video-1)  |  |
| `variable` | [`_audio`](#_audio-1)  |  |
| `variable` | [`_ioCtx`](#_ioctx)  |  |
| `variable` | [`_ioBuffer`](#_iobuffer)  |  |
| `variable` | [`_pts`](#_pts)  |  |
| `variable` | [`_mutex`](#_mutex-2)  |  |
| `function` | [`writeOutputPacket`](#writeoutputpacket)  |  |
| `function` | [`updateStreamPts`](#updatestreampts)  | Convert input microseconds to the stream time base. |
| `function` | [`onVideoEncoded`](#onvideoencoded)  |  |
| `function` | [`onAudioEncoded`](#onaudioencoded)  |  |

### Inherited from [`IEncoder`](#iencoder)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`init`](#init) `virtual` | Initialise the encoder, open codec contexts, and transition to Ready or Encoding state. |
| `function` | [`uninit`](#uninit) `virtual` | Flush remaining packets, close codec contexts, and release all encoder resources. |
| `function` | [`cleanup`](#cleanup) `virtual` `inline` | Perform any additional cleanup after [uninit()](#uninit). Default is a no-op. |
| `function` | [`options`](#options-1) `virtual` `const` `nodiscard` |  |
| `function` | [`createVideo`](#createvideo) `virtual` `inline` | Initialise the video codec context and stream. |
| `function` | [`freeVideo`](#freevideo) `virtual` `inline` | Free the video codec context and stream. |
| `function` | [`encodeVideo`](#encodevideo) `virtual` `inline` `nodiscard` | Encode a single AVFrame of video. |
| `function` | [`createAudio`](#createaudio) `virtual` `inline` | Initialise the audio codec context and stream. |
| `function` | [`freeAudio`](#freeaudio) `virtual` `inline` | Free the audio codec context and stream. |
| `function` | [`flush`](#flush) `virtual` `inline` | Flush any internally buffered packets to the output. |
| `function` | [`isNone`](#isnone) `const` `inline` |  |
| `function` | [`isReady`](#isready) `const` `inline` |  |
| `function` | [`isEncoding`](#isencoding) `virtual` `const` `inline` |  |
| `function` | [`isActive`](#isactive) `virtual` `const` `inline` |  |
| `function` | [`isStopped`](#isstopped) `virtual` `const` `inline` |  |
| `function` | [`isError`](#iserror) `virtual` `const` `inline` |  |
| `enum` | [`Type`](#type-3)  |  |

### Inherited from [`Stateful`](base.md#stateful)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`StateChange`](base.md#classicy_1_1Stateful_1adba38e2b8a737448d48fc56b4bbe1dc0)  | Signals when the state changes. |
| `variable` | [`_state`](base.md#classicy_1_1Stateful_1a45326923d41e72e4afc70d9ed8acc15d)  |  |
| `function` | [`Stateful`](base.md#classicy_1_1Stateful_1a444344bc5f8563af4d289be6beb693a0) `inline` |  |
| `function` | [`~Stateful`](base.md#classicy_1_1Stateful_1a70818a671339a0468ef71e6d54c97c77) `virtual` `inline` |  |
| `function` | [`stateEquals`](base.md#classicy_1_1Stateful_1a8c8bd62dbd4251749150ed7f95bdb875) `virtual` `const` `inline` | Returns true if the current state ID equals the given ID. |
| `function` | [`stateBetween`](base.md#classicy_1_1Stateful_1a702bf6338c45b6ce75540a2d7f03a18f) `virtual` `const` `inline` | Returns true if the current state ID is in the inclusive range [lid, rid]. |
| `function` | [`state`](base.md#classicy_1_1Stateful_1ae1082bd323b27b8eb1022f160d77a6e6) `virtual` `inline` | Returns a mutable reference to the current state. |
| `function` | [`state`](base.md#classicy_1_1Stateful_1ab63e2709491c8fb841f7dd5720c15020) `virtual` `const` `inline` | Returns a copy of the current state. |
| `function` | [`beforeStateChange`](base.md#classicy_1_1Stateful_1a0ae9d2650c9a6fdb276b88e0b2d3ca34) `virtual` `inline` | Override to handle pre state change logic. Return false to prevent state change. |
| `function` | [`onStateChange`](base.md#classicy_1_1Stateful_1a06e5572c313c3b54db932c922166079d) `virtual` `inline` | Override to handle post state change logic. |
| `function` | [`setState`](base.md#classicy_1_1Stateful_1a88cfa69863f505b8232c7cce5989a395) `virtual` `inline` | Sets the state and sends the state signal if the state change was successful. |
| `function` | [`setState`](base.md#classicy_1_1Stateful_1a9d5c3013be4fb7508266ed0b950694aa) `virtual` `inline` | Sets the state and sends the state signal if the state change was successful. |

### Inherited from [`PacketProcessor`](base.md#packetprocessor)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`PacketProcessor`](base.md#classicy_1_1PacketProcessor_1aa9a61f3eb4c53e7673b29d4c97686d9a) `inline` |  |
| `function` | [`process`](base.md#classicy_1_1PacketProcessor_1acd9fb1a5fa525c0a2d96971fe6c06294) `virtual` | This method performs processing on the given packet and emits the result. |
| `function` | [`accepts`](base.md#classicy_1_1PacketProcessor_1a838d9d75f81bd943b40efcbe683394b7) `virtual` `inline` | This method ensures compatibility with the given packet type. Return false to reject the packet. |
| `function` | [`operator<<`](base.md#classicy_1_1PacketProcessor_1ae8908612aea63a6e5b3d3f7714a8e7a3) `virtual` `inline` | [Stream](base.md#stream) operator alias for [process()](base.md#classicy_1_1PacketProcessor_1acd9fb1a5fa525c0a2d96971fe6c06294). |

### Inherited from [`PacketStreamAdapter`](base.md#packetstreamadapter)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`_emitter`](base.md#classicy_1_1PacketStreamAdapter_1ad954a1cd2f20587a622abd9dcebb56cd)  |  |
| `function` | [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a904abfd9eb913c034a3648dc1fce3f57)  | Construct the adapter, binding it to the given packet signal. |
| `function` | [`~PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a48b7da7a145125866e041c3d2cc7396c) `virtual` `inline` |  |
| `function` | [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a6762a9434263339d1a477e72de59704c) `virtual` | Emit a mutable raw buffer as a packet. |
| `function` | [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a797c0aa60e4999527e8efc3b2fdc88fd) `virtual` | Emit a read-only raw buffer as a packet (data is copied internally). |
| `function` | [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a4c605d524a538bfcc1cdf30ad4c22ce3) `virtual` | Emit a string as a packet (data is copied internally). |
| `function` | [`emit`](base.md#classicy_1_1PacketStreamAdapter_1a10da09b2bf9b5b39c88e97813f99cbee) `virtual` | Emit a flag-only packet carrying no payload data. |
| `function` | [`emit`](base.md#classicy_1_1PacketStreamAdapter_1abb0394d938214165fc89e999e6937831) `virtual` | Emit an existing packet directly onto the outgoing signal. |
| `function` | [`getEmitter`](base.md#classicy_1_1PacketStreamAdapter_1a20cfe30f1db9ef46f9db060581fd2ff7)  | Returns a reference to the outgoing packet signal. |
| `function` | [`retention`](base.md#classicy_1_1PacketStreamAdapter_1a37a1aa11e35a20a7c527efa78e99e928) `virtual` `const` `nodiscard` | Returns how this adapter treats incoming packet lifetime. Most adapters are synchronous and therefore only borrow the packet for the current call chain. Queue-style adapters override this to advertise that they clone before deferred use. Callers may treat the first adapter reporting Cloned or Retained as the explicit ownership boundary in the stream graph. |
| `function` | [`onStreamStateChange`](base.md#classicy_1_1PacketStreamAdapter_1a86f0e2dc118359a83b8cbb996da6dc83) `virtual` `inline` | Called by the [PacketStream](base.md#packetstream) to notify when the internal [Stream](base.md#stream) state changes. On receiving the Stopped state, it is the responsibility of the adapter to have ceased all outgoing packet transmission, especially in multi-thread scenarios. |
| `function` | [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1a5915b4a54729309a0fc12ee9c0107339)  | NonCopyable and NonMovable. |
| `function` | [`operator=`](base.md#classicy_1_1PacketStreamAdapter_1aeec4b3367101ffd07e3050cbfd726705)  | Deleted assignment operator. |
| `function` | [`PacketStreamAdapter`](base.md#classicy_1_1PacketStreamAdapter_1abf38bdf926762063adfd29427cad1362)  | Deleted constructor. |
| `function` | [`operator=`](base.md#classicy_1_1PacketStreamAdapter_1ac7fe6a98e57151ad909715f77e192481)  | Deleted assignment operator. |

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

##### Reimplements

- [`accepts`](base.md#classicy_1_1PacketProcessor_1a838d9d75f81bd943b40efcbe683394b7)

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

##### Reimplements

- [`process`](base.md#classicy_1_1PacketProcessor_1acd9fb1a5fa525c0a2d96971fe6c06294)

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

##### Reimplements

- [`onStreamStateChange`](base.md#classicy_1_1PacketStreamAdapter_1a86f0e2dc118359a83b8cbb996da6dc83)

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`name`](#name) | `variable` | Declared here |
| [`encoder`](#encoder) | `variable` | Declared here |
| [`sampleRate`](#samplerate) | `variable` | Declared here |
| [`bitRate`](#bitrate) | `variable` | Declared here |
| [`quality`](#quality) | `variable` | Declared here |
| [`compliance`](#compliance) | `variable` | Declared here |
| [`enabled`](#enabled) | `variable` | Declared here |
| [`options`](#options) | `variable` | Declared here |
| [`Codec`](#codec-1) | `function` | Declared here |
| [`Codec`](#codec-2) | `function` | Declared here |
| [`Codec`](#codec-3) | `function` | Declared here |
| [`~Codec`](#codec-4) | `function` | Declared here |
| [`toString`](#tostring) | `function` | Declared here |
| [`specified`](#specified) | `function` | Declared here |
| [`print`](#print) | `function` | Declared here |

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

##### Reimplemented by

- [`toString`](#tostring-1)
- [`toString`](#tostring-2)

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

##### Reimplemented by

- [`print`](#print-1)
- [`print`](#print-2)

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`channels`](#channels) | `variable` | Declared here |
| [`sampleFmt`](#samplefmt) | `variable` | Declared here |
| [`AudioCodec`](#audiocodec-1) | `function` | Declared here |
| [`AudioCodec`](#audiocodec-2) | `function` | Declared here |
| [`AudioCodec`](#audiocodec-3) | `function` | Declared here |
| [`AudioCodec`](#audiocodec-4) | `function` | Declared here |
| [`toString`](#tostring-1) | `function` | Declared here |
| [`print`](#print-1) | `function` | Declared here |
| [`name`](#name) | `variable` | Inherited from [`Codec`](#codec) |
| [`encoder`](#encoder) | `variable` | Inherited from [`Codec`](#codec) |
| [`sampleRate`](#samplerate) | `variable` | Inherited from [`Codec`](#codec) |
| [`bitRate`](#bitrate) | `variable` | Inherited from [`Codec`](#codec) |
| [`quality`](#quality) | `variable` | Inherited from [`Codec`](#codec) |
| [`compliance`](#compliance) | `variable` | Inherited from [`Codec`](#codec) |
| [`enabled`](#enabled) | `variable` | Inherited from [`Codec`](#codec) |
| [`options`](#options) | `variable` | Inherited from [`Codec`](#codec) |
| [`Codec`](#codec-1) | `function` | Inherited from [`Codec`](#codec) |
| [`Codec`](#codec-2) | `function` | Inherited from [`Codec`](#codec) |
| [`Codec`](#codec-3) | `function` | Inherited from [`Codec`](#codec) |
| [`~Codec`](#codec-4) | `function` | Inherited from [`Codec`](#codec) |
| [`toString`](#tostring) | `function` | Inherited from [`Codec`](#codec) |
| [`specified`](#specified) | `function` | Inherited from [`Codec`](#codec) |
| [`print`](#print) | `function` | Inherited from [`Codec`](#codec) |

### Inherited from [`Codec`](#codec)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`name`](#name)  | The display name for this codec. |
| `variable` | [`encoder`](#encoder)  | The encoder name for FFmpeg. |
| `variable` | [`sampleRate`](#samplerate)  | The sampling rate or RTP clock rate. |
| `variable` | [`bitRate`](#bitrate)  | The bit rate to encode at. |
| `variable` | [`quality`](#quality)  | Optional quality value, variable range depending on codec. |
| `variable` | [`compliance`](#compliance)  | FFmpeg strict_std_compliance level (default: FF_COMPLIANCE_EXPERIMENTAL). |
| `variable` | [`enabled`](#enabled)  | Weather or not the codec is available for use. |
| `variable` | [`options`](#options)  | Arbitrary encoder options passed to FFmpeg via av_opt_set(). Keys are FFmpeg option names (e.g. "preset", "tune", "profile"). Applied after codec-specific defaults, so they override everything. |
| `function` | [`Codec`](#codec-1)  | Construct a disabled codec with zeroed parameters. |
| `function` | [`Codec`](#codec-2)  | Construct a codec with a display name, sample rate, bit rate, and enabled flag. |
| `function` | [`Codec`](#codec-3)  | Construct a codec with an explicit FFmpeg encoder name. |
| `function` | [`~Codec`](#codec-4) `virtual` `noexcept` | Codec(const Codec& r);. |
| `function` | [`toString`](#tostring) `virtual` `const` | Return a compact string representation of this codec. |
| `function` | [`specified`](#specified) `const` `nodiscard` | Returns true when this codec explicitly names either an RTP/media codec or an FFmpeg encoder and is enabled for use. |
| `function` | [`print`](#print) `virtual` | Print a multi-line human-readable description to the given stream. |

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

##### Reimplements

- [`toString`](#tostring)

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

##### Reimplements

- [`print`](#print)

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`width`](#width) | `variable` | Declared here |
| [`height`](#height) | `variable` | Declared here |
| [`fps`](#fps) | `variable` | Declared here |
| [`pixelFmt`](#pixelfmt) | `variable` | Declared here |
| [`VideoCodec`](#videocodec-1) | `function` | Declared here |
| [`VideoCodec`](#videocodec-2) | `function` | Declared here |
| [`VideoCodec`](#videocodec-3) | `function` | Declared here |
| [`VideoCodec`](#videocodec-4) | `function` | Declared here |
| [`VideoCodec`](#videocodec-5) | `function` | Declared here |
| [`toString`](#tostring-2) | `function` | Declared here |
| [`print`](#print-2) | `function` | Declared here |
| [`name`](#name) | `variable` | Inherited from [`Codec`](#codec) |
| [`encoder`](#encoder) | `variable` | Inherited from [`Codec`](#codec) |
| [`sampleRate`](#samplerate) | `variable` | Inherited from [`Codec`](#codec) |
| [`bitRate`](#bitrate) | `variable` | Inherited from [`Codec`](#codec) |
| [`quality`](#quality) | `variable` | Inherited from [`Codec`](#codec) |
| [`compliance`](#compliance) | `variable` | Inherited from [`Codec`](#codec) |
| [`enabled`](#enabled) | `variable` | Inherited from [`Codec`](#codec) |
| [`options`](#options) | `variable` | Inherited from [`Codec`](#codec) |
| [`Codec`](#codec-1) | `function` | Inherited from [`Codec`](#codec) |
| [`Codec`](#codec-2) | `function` | Inherited from [`Codec`](#codec) |
| [`Codec`](#codec-3) | `function` | Inherited from [`Codec`](#codec) |
| [`~Codec`](#codec-4) | `function` | Inherited from [`Codec`](#codec) |
| [`toString`](#tostring) | `function` | Inherited from [`Codec`](#codec) |
| [`specified`](#specified) | `function` | Inherited from [`Codec`](#codec) |
| [`print`](#print) | `function` | Inherited from [`Codec`](#codec) |

### Inherited from [`Codec`](#codec)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`name`](#name)  | The display name for this codec. |
| `variable` | [`encoder`](#encoder)  | The encoder name for FFmpeg. |
| `variable` | [`sampleRate`](#samplerate)  | The sampling rate or RTP clock rate. |
| `variable` | [`bitRate`](#bitrate)  | The bit rate to encode at. |
| `variable` | [`quality`](#quality)  | Optional quality value, variable range depending on codec. |
| `variable` | [`compliance`](#compliance)  | FFmpeg strict_std_compliance level (default: FF_COMPLIANCE_EXPERIMENTAL). |
| `variable` | [`enabled`](#enabled)  | Weather or not the codec is available for use. |
| `variable` | [`options`](#options)  | Arbitrary encoder options passed to FFmpeg via av_opt_set(). Keys are FFmpeg option names (e.g. "preset", "tune", "profile"). Applied after codec-specific defaults, so they override everything. |
| `function` | [`Codec`](#codec-1)  | Construct a disabled codec with zeroed parameters. |
| `function` | [`Codec`](#codec-2)  | Construct a codec with a display name, sample rate, bit rate, and enabled flag. |
| `function` | [`Codec`](#codec-3)  | Construct a codec with an explicit FFmpeg encoder name. |
| `function` | [`~Codec`](#codec-4) `virtual` `noexcept` | Codec(const Codec& r);. |
| `function` | [`toString`](#tostring) `virtual` `const` | Return a compact string representation of this codec. |
| `function` | [`specified`](#specified) `const` `nodiscard` | Returns true when this codec explicitly names either an RTP/media codec or an FFmpeg encoder and is enabled for use. |
| `function` | [`print`](#print) `virtual` | Print a multi-line human-readable description to the given stream. |

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

##### Reimplements

- [`toString`](#tostring)

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

##### Reimplements

- [`print`](#print)

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`operator()`](#operator) | `function` | Declared here |

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`operator()`](#operator-1) | `function` | Declared here |

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`name`](#name-1) | `variable` | Declared here |
| [`id`](#id) | `variable` | Declared here |
| [`video`](#video) | `variable` | Declared here |
| [`audio`](#audio) | `variable` | Declared here |
| [`priority`](#priority) | `variable` | Declared here |
| [`Format`](#format-1) | `function` | Declared here |
| [`Format`](#format-2) | `function` | Declared here |
| [`Format`](#format-3) | `function` | Declared here |
| [`Format`](#format-4) | `function` | Declared here |
| [`Format`](#format-5) | `function` | Declared here |
| [`type`](#type) | `function` | Declared here |
| [`toString`](#tostring-3) | `function` | Declared here |
| [`print`](#print-3) | `function` | Declared here |
| [`preferable`](#preferable) | `function` | Declared here |
| [`Type`](#type-1) | `enum` | Declared here |

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`str`](#str) | `function` | Declared here |
| [`Type`](#type-2) | `enum` | Declared here |
| [`ID`](base.md#classicy_1_1State_1af5e8f0a00984df441608f8bedaaecea3) | `typedef` | Inherited from [`State`](base.md#state) |
| [`_id`](base.md#classicy_1_1State_1ab9d37cc2fcfbd3a73f34d3016b208fde) | `variable` | Inherited from [`State`](base.md#state) |
| [`State`](base.md#classicy_1_1State_1a59becee99ad348d1ee73bc01e68363a0) | `function` | Inherited from [`State`](base.md#state) |
| [`State`](base.md#classicy_1_1State_1a79e71a24ac604ffb994dcf7502bc66fc) | `function` | Inherited from [`State`](base.md#state) |
| [`operator=`](base.md#classicy_1_1State_1a3f125b4a7aaa9949d3adcdf6de4600dd) | `function` | Inherited from [`State`](base.md#state) |
| [`~State`](base.md#classicy_1_1State_1aaee58de683ac259ac707f5930893277d) | `function` | Inherited from [`State`](base.md#state) |
| [`id`](base.md#classicy_1_1State_1ad1f3cf6094d92da45a8863ccd05a4dd2) | `function` | Inherited from [`State`](base.md#state) |
| [`set`](base.md#classicy_1_1State_1a5e8499ade2da1c1ad1c668c0107dc4d0) | `function` | Inherited from [`State`](base.md#state) |
| [`str`](base.md#classicy_1_1State_1afaef9ebd5e0434dbd6c2d6cf0dde6f07) | `function` | Inherited from [`State`](base.md#state) |
| [`toString`](base.md#classicy_1_1State_1ab14617c2967319793f296006b3cb175c) | `function` | Inherited from [`State`](base.md#state) |
| [`equals`](base.md#classicy_1_1State_1a47caae3d559489f145840fa68fe3f66c) | `function` | Inherited from [`State`](base.md#state) |
| [`between`](base.md#classicy_1_1State_1a354d30c75b8097a61b34804a36fb6097) | `function` | Inherited from [`State`](base.md#state) |
| [`operator==`](base.md#classicy_1_1State_1a75c8aeafab4c7c8f314c5417e2907e3a) | `function` | Inherited from [`State`](base.md#state) |
| [`operator==`](base.md#classicy_1_1State_1a15607010e464e8dc2bb368274483d364) | `function` | Inherited from [`State`](base.md#state) |
| [`operator<<`](base.md#classicy_1_1State_1a6c293f08ac54988431d4f11454aa7bbd) | `friend` | Inherited from [`State`](base.md#state) |

### Inherited from [`State`](base.md#state)

| Kind | Name | Description |
|------|------|-------------|
| `typedef` | [`ID`](base.md#classicy_1_1State_1af5e8f0a00984df441608f8bedaaecea3)  |  |
| `variable` | [`_id`](base.md#classicy_1_1State_1ab9d37cc2fcfbd3a73f34d3016b208fde)  |  |
| `function` | [`State`](base.md#classicy_1_1State_1a59becee99ad348d1ee73bc01e68363a0)  |  |
| `function` | [`State`](base.md#classicy_1_1State_1a79e71a24ac604ffb994dcf7502bc66fc)  | Copy constructor. |
| `function` | [`operator=`](base.md#classicy_1_1State_1a3f125b4a7aaa9949d3adcdf6de4600dd)  | Copy assignment. |
| `function` | [`~State`](base.md#classicy_1_1State_1aaee58de683ac259ac707f5930893277d) `virtual` | Defaulted destructor. |
| `function` | [`id`](base.md#classicy_1_1State_1ad1f3cf6094d92da45a8863ccd05a4dd2) `const` | Returns the current state [ID](base.md#classicy_1_1State_1af5e8f0a00984df441608f8bedaaecea3). |
| `function` | [`set`](base.md#classicy_1_1State_1a5e8499ade2da1c1ad1c668c0107dc4d0)  | Sets the state [ID](base.md#classicy_1_1State_1af5e8f0a00984df441608f8bedaaecea3). |
| `function` | [`str`](base.md#classicy_1_1State_1afaef9ebd5e0434dbd6c2d6cf0dde6f07) `virtual` `const` | Returns a human-readable string for the given state [ID](base.md#classicy_1_1State_1af5e8f0a00984df441608f8bedaaecea3). Override in derived classes to provide meaningful names. |
| `function` | [`toString`](base.md#classicy_1_1State_1ab14617c2967319793f296006b3cb175c) `virtual` `const` | Returns a human-readable string for the current state [ID](base.md#classicy_1_1State_1af5e8f0a00984df441608f8bedaaecea3). |
| `function` | [`equals`](base.md#classicy_1_1State_1a47caae3d559489f145840fa68fe3f66c) `const` | Returns true if the current state [ID](base.md#classicy_1_1State_1af5e8f0a00984df441608f8bedaaecea3) equals the given [ID](base.md#classicy_1_1State_1af5e8f0a00984df441608f8bedaaecea3). |
| `function` | [`between`](base.md#classicy_1_1State_1a354d30c75b8097a61b34804a36fb6097) `const` | Returns true if the current state [ID](base.md#classicy_1_1State_1af5e8f0a00984df441608f8bedaaecea3) is in the inclusive range [lid, rid]. |
| `function` | [`operator==`](base.md#classicy_1_1State_1a75c8aeafab4c7c8f314c5417e2907e3a) `const` `inline` |  |
| `function` | [`operator==`](base.md#classicy_1_1State_1a15607010e464e8dc2bb368274483d364) `const` `inline` |  |
| `friend` | [`operator<<`](base.md#classicy_1_1State_1a6c293f08ac54988431d4f11454aa7bbd) `inline` |  |

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`iformat`](#iformat) | `variable` | Declared here |
| [`oformat`](#oformat) | `variable` | Declared here |
| [`ifile`](#ifile) | `variable` | Declared here |
| [`ofile`](#ofile) | `variable` | Declared here |
| [`duration`](#duration) | `variable` | Declared here |
| [`EncoderOptions`](#encoderoptions-1) | `function` | Declared here |

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`fifo`](#fifo) | `variable` | Declared here |
| [`AudioBuffer`](#audiobuffer-1) | `function` | Declared here |
| [`AudioBuffer`](#audiobuffer-2) | `function` | Declared here |
| [`AudioBuffer`](#audiobuffer-3) | `function` | Declared here |
| [`alloc`](#alloc) | `function` | Declared here |
| [`reset`](#reset-2) | `function` | Declared here |
| [`close`](#close-1) | `function` | Declared here |
| [`write`](#write) | `function` | Declared here |
| [`read`](#read) | `function` | Declared here |
| [`available`](#available) | `function` | Declared here |

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`emitter`](#emitter-2) | `variable` | Declared here |
| [`iparams`](#iparams) | `variable` | Declared here |
| [`oparams`](#oparams) | `variable` | Declared here |
| [`stream`](#stream-1) | `variable` | Declared here |
| [`ctx`](#ctx) | `variable` | Declared here |
| [`codec`](#codec-5) | `variable` | Declared here |
| [`frame`](#frame) | `variable` | Declared here |
| [`resampler`](#resampler) | `variable` | Declared here |
| [`outputFrameSize`](#outputframesize) | `variable` | Declared here |
| [`time`](#time) | `variable` | Declared here |
| [`pts`](#pts) | `variable` | Declared here |
| [`seconds`](#seconds) | `variable` | Declared here |
| [`error`](#error-1) | `variable` | Declared here |
| [`AudioContext`](#audiocontext-1) | `function` | Declared here |
| [`AudioContext`](#audiocontext-2) | `function` | Declared here |
| [`AudioContext`](#audiocontext-3) | `function` | Declared here |
| [`create`](#create) | `function` | Declared here |
| [`open`](#open) | `function` | Declared here |
| [`close`](#close-2) | `function` | Declared here |
| [`decode`](#decode) | `function` | Declared here |
| [`encode`](#encode) | `function` | Declared here |
| [`encode`](#encode-1) | `function` | Declared here |
| [`encode`](#encode-2) | `function` | Declared here |
| [`flush`](#flush-1) | `function` | Declared here |
| [`recreateResampler`](#recreateresampler) | `function` | Declared here |

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

##### Reimplemented by

- [`create`](#create-1)
- [`create`](#create-2)

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

##### Reimplemented by

- [`close`](#close-3)
- [`close`](#close-4)

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

##### Reimplemented by

- [`decode`](#decode-1)

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

##### Reimplemented by

- [`encode`](#encode-3)

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

##### Reimplemented by

- [`encode`](#encode-4)

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

##### Reimplemented by

- [`encode`](#encode-5)

---

{#flush-1}

#### flush

`virtual`

```cpp
virtual void flush()
```

Defined in src/av/include/icy/av/audiocontext.h:85

Flush any frames buffered inside the codec and emit remaining output.

##### Reimplemented by

- [`flush`](#flush-2)
- [`flush`](#flush-3)

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`AudioDecoder`](#audiodecoder-1) | `function` | Declared here |
| [`create`](#create-1) | `function` | Declared here |
| [`close`](#close-3) | `function` | Declared here |
| [`decode`](#decode-1) | `function` | Declared here |
| [`flush`](#flush-2) | `function` | Declared here |
| [`emitter`](#emitter-2) | `variable` | Inherited from [`AudioContext`](#audiocontext) |
| [`iparams`](#iparams) | `variable` | Inherited from [`AudioContext`](#audiocontext) |
| [`oparams`](#oparams) | `variable` | Inherited from [`AudioContext`](#audiocontext) |
| [`stream`](#stream-1) | `variable` | Inherited from [`AudioContext`](#audiocontext) |
| [`ctx`](#ctx) | `variable` | Inherited from [`AudioContext`](#audiocontext) |
| [`codec`](#codec-5) | `variable` | Inherited from [`AudioContext`](#audiocontext) |
| [`frame`](#frame) | `variable` | Inherited from [`AudioContext`](#audiocontext) |
| [`resampler`](#resampler) | `variable` | Inherited from [`AudioContext`](#audiocontext) |
| [`outputFrameSize`](#outputframesize) | `variable` | Inherited from [`AudioContext`](#audiocontext) |
| [`time`](#time) | `variable` | Inherited from [`AudioContext`](#audiocontext) |
| [`pts`](#pts) | `variable` | Inherited from [`AudioContext`](#audiocontext) |
| [`seconds`](#seconds) | `variable` | Inherited from [`AudioContext`](#audiocontext) |
| [`error`](#error-1) | `variable` | Inherited from [`AudioContext`](#audiocontext) |
| [`AudioContext`](#audiocontext-1) | `function` | Inherited from [`AudioContext`](#audiocontext) |
| [`AudioContext`](#audiocontext-2) | `function` | Inherited from [`AudioContext`](#audiocontext) |
| [`AudioContext`](#audiocontext-3) | `function` | Inherited from [`AudioContext`](#audiocontext) |
| [`create`](#create) | `function` | Inherited from [`AudioContext`](#audiocontext) |
| [`open`](#open) | `function` | Inherited from [`AudioContext`](#audiocontext) |
| [`close`](#close-2) | `function` | Inherited from [`AudioContext`](#audiocontext) |
| [`decode`](#decode) | `function` | Inherited from [`AudioContext`](#audiocontext) |
| [`encode`](#encode) | `function` | Inherited from [`AudioContext`](#audiocontext) |
| [`encode`](#encode-1) | `function` | Inherited from [`AudioContext`](#audiocontext) |
| [`encode`](#encode-2) | `function` | Inherited from [`AudioContext`](#audiocontext) |
| [`flush`](#flush-1) | `function` | Inherited from [`AudioContext`](#audiocontext) |
| [`recreateResampler`](#recreateresampler) | `function` | Inherited from [`AudioContext`](#audiocontext) |

### Inherited from [`AudioContext`](#audiocontext)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`emitter`](#emitter-2)  |  |
| `variable` | [`iparams`](#iparams)  | input parameters |
| `variable` | [`oparams`](#oparams)  | output parameters |
| `variable` | [`stream`](#stream-1)  | encoder or decoder stream |
| `variable` | [`ctx`](#ctx)  | encoder or decoder context |
| `variable` | [`codec`](#codec-5)  | encoder or decoder codec |
| `variable` | [`frame`](#frame)  | last encoded or decoded frame |
| `variable` | [`resampler`](#resampler)  | audio resampler |
| `variable` | [`outputFrameSize`](#outputframesize)  | encoder or decoder output frame size |
| `variable` | [`time`](#time)  | stream time in codec time base |
| `variable` | [`pts`](#pts)  | last packet pts value |
| `variable` | [`seconds`](#seconds)  | audio time in seconds |
| `variable` | [`error`](#error-1)  | error message |
| `function` | [`AudioContext`](#audiocontext-1)  |  |
| `function` | [`AudioContext`](#audiocontext-2)  | Deleted constructor. |
| `function` | [`AudioContext`](#audiocontext-3)  | Deleted constructor. |
| `function` | [`create`](#create) `virtual` | Initialise the AVCodecContext with codec-specific defaults. Implemented by [AudioEncoder](#audioencoder) and [AudioDecoder](#audiodecoder). |
| `function` | [`open`](#open) `virtual` | Open the codec and create the resampler if input/output parameters differ. Throws std::runtime_error if the codec context has not been created. |
| `function` | [`close`](#close-2) `virtual` | Close the codec context, free the frame, and reset timestamps. |
| `function` | [`decode`](#decode) `virtual` `nodiscard` | Decode a compressed audio packet and emit the resulting samples. |
| `function` | [`encode`](#encode) `virtual` `nodiscard` | Encode a buffer of interleaved audio samples. |
| `function` | [`encode`](#encode-1) `virtual` `nodiscard` | Encode a buffer of planar audio samples. |
| `function` | [`encode`](#encode-2) `virtual` `nodiscard` | Encode a single AVFrame. |
| `function` | [`flush`](#flush-1) `virtual` | Flush any frames buffered inside the codec and emit remaining output. |
| `function` | [`recreateResampler`](#recreateresampler) `virtual` | Recreate the [AudioResampler](#audioresampler) using the current iparams and oparams. Called automatically by [open()](#open) when format conversion is required. |

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

##### Reimplements

- [`create`](#create)

---

{#close-3}

#### close

`virtual` `override`

```cpp
virtual void close() override
```

Defined in src/av/include/icy/av/audiodecoder.h:40

Close and free the AVCodecContext and associated resources.

##### Reimplements

- [`close`](#close-2)

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

##### Reimplements

- [`decode`](#decode)

---

{#flush-2}

#### flush

`virtual` `override`

```cpp
virtual void flush() override
```

Defined in src/av/include/icy/av/audiodecoder.h:49

Flush any frames buffered inside the decoder. Call this after the last packet to retrieve all remaining decoded output.

##### Reimplements

- [`flush`](#flush-1)

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`fifo`](#fifo-1) | `variable` | Declared here |
| [`format`](#format-6) | `variable` | Declared here |
| [`nextOutputPts`](#nextoutputpts) | `variable` | Declared here |
| [`AudioEncoder`](#audioencoder-1) | `function` | Declared here |
| [`create`](#create-2) | `function` | Declared here |
| [`close`](#close-4) | `function` | Declared here |
| [`encode`](#encode-3) | `function` | Declared here |
| [`encode`](#encode-4) | `function` | Declared here |
| [`encode`](#encode-5) | `function` | Declared here |
| [`flush`](#flush-3) | `function` | Declared here |
| [`emitter`](#emitter-2) | `variable` | Inherited from [`AudioContext`](#audiocontext) |
| [`iparams`](#iparams) | `variable` | Inherited from [`AudioContext`](#audiocontext) |
| [`oparams`](#oparams) | `variable` | Inherited from [`AudioContext`](#audiocontext) |
| [`stream`](#stream-1) | `variable` | Inherited from [`AudioContext`](#audiocontext) |
| [`ctx`](#ctx) | `variable` | Inherited from [`AudioContext`](#audiocontext) |
| [`codec`](#codec-5) | `variable` | Inherited from [`AudioContext`](#audiocontext) |
| [`frame`](#frame) | `variable` | Inherited from [`AudioContext`](#audiocontext) |
| [`resampler`](#resampler) | `variable` | Inherited from [`AudioContext`](#audiocontext) |
| [`outputFrameSize`](#outputframesize) | `variable` | Inherited from [`AudioContext`](#audiocontext) |
| [`time`](#time) | `variable` | Inherited from [`AudioContext`](#audiocontext) |
| [`pts`](#pts) | `variable` | Inherited from [`AudioContext`](#audiocontext) |
| [`seconds`](#seconds) | `variable` | Inherited from [`AudioContext`](#audiocontext) |
| [`error`](#error-1) | `variable` | Inherited from [`AudioContext`](#audiocontext) |
| [`AudioContext`](#audiocontext-1) | `function` | Inherited from [`AudioContext`](#audiocontext) |
| [`AudioContext`](#audiocontext-2) | `function` | Inherited from [`AudioContext`](#audiocontext) |
| [`AudioContext`](#audiocontext-3) | `function` | Inherited from [`AudioContext`](#audiocontext) |
| [`create`](#create) | `function` | Inherited from [`AudioContext`](#audiocontext) |
| [`open`](#open) | `function` | Inherited from [`AudioContext`](#audiocontext) |
| [`close`](#close-2) | `function` | Inherited from [`AudioContext`](#audiocontext) |
| [`decode`](#decode) | `function` | Inherited from [`AudioContext`](#audiocontext) |
| [`encode`](#encode) | `function` | Inherited from [`AudioContext`](#audiocontext) |
| [`encode`](#encode-1) | `function` | Inherited from [`AudioContext`](#audiocontext) |
| [`encode`](#encode-2) | `function` | Inherited from [`AudioContext`](#audiocontext) |
| [`flush`](#flush-1) | `function` | Inherited from [`AudioContext`](#audiocontext) |
| [`recreateResampler`](#recreateresampler) | `function` | Inherited from [`AudioContext`](#audiocontext) |

### Inherited from [`AudioContext`](#audiocontext)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`emitter`](#emitter-2)  |  |
| `variable` | [`iparams`](#iparams)  | input parameters |
| `variable` | [`oparams`](#oparams)  | output parameters |
| `variable` | [`stream`](#stream-1)  | encoder or decoder stream |
| `variable` | [`ctx`](#ctx)  | encoder or decoder context |
| `variable` | [`codec`](#codec-5)  | encoder or decoder codec |
| `variable` | [`frame`](#frame)  | last encoded or decoded frame |
| `variable` | [`resampler`](#resampler)  | audio resampler |
| `variable` | [`outputFrameSize`](#outputframesize)  | encoder or decoder output frame size |
| `variable` | [`time`](#time)  | stream time in codec time base |
| `variable` | [`pts`](#pts)  | last packet pts value |
| `variable` | [`seconds`](#seconds)  | audio time in seconds |
| `variable` | [`error`](#error-1)  | error message |
| `function` | [`AudioContext`](#audiocontext-1)  |  |
| `function` | [`AudioContext`](#audiocontext-2)  | Deleted constructor. |
| `function` | [`AudioContext`](#audiocontext-3)  | Deleted constructor. |
| `function` | [`create`](#create) `virtual` | Initialise the AVCodecContext with codec-specific defaults. Implemented by [AudioEncoder](#audioencoder) and [AudioDecoder](#audiodecoder). |
| `function` | [`open`](#open) `virtual` | Open the codec and create the resampler if input/output parameters differ. Throws std::runtime_error if the codec context has not been created. |
| `function` | [`close`](#close-2) `virtual` | Close the codec context, free the frame, and reset timestamps. |
| `function` | [`decode`](#decode) `virtual` `nodiscard` | Decode a compressed audio packet and emit the resulting samples. |
| `function` | [`encode`](#encode) `virtual` `nodiscard` | Encode a buffer of interleaved audio samples. |
| `function` | [`encode`](#encode-1) `virtual` `nodiscard` | Encode a buffer of planar audio samples. |
| `function` | [`encode`](#encode-2) `virtual` `nodiscard` | Encode a single AVFrame. |
| `function` | [`flush`](#flush-1) `virtual` | Flush any frames buffered inside the codec and emit remaining output. |
| `function` | [`recreateResampler`](#recreateresampler) `virtual` | Recreate the [AudioResampler](#audioresampler) using the current iparams and oparams. Called automatically by [open()](#open) when format conversion is required. |

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

##### Reimplements

- [`create`](#create)

---

{#close-4}

#### close

`virtual` `override`

```cpp
virtual void close() override
```

Defined in src/av/include/icy/av/audioencoder.h:49

Close and free the AVCodecContext, FIFO buffer, and associated resources.

##### Reimplements

- [`close`](#close-2)

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

##### Reimplements

- [`encode`](#encode)

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

##### Reimplements

- [`encode`](#encode-1)

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

##### Reimplements

- [`encode`](#encode-2)

---

{#flush-3}

#### flush

`virtual` `override`

```cpp
virtual void flush() override
```

Defined in src/av/include/icy/av/audioencoder.h:72

Flush remaining packets to be encoded. This method should be called once before stream closure.

##### Reimplements

- [`flush`](#flush-1)

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`emitter`](#emitter-3) | `variable` | Declared here |
| [`iparams`](#iparams-1) | `variable` | Declared here |
| [`oparams`](#oparams-1) | `variable` | Declared here |
| [`stream`](#stream-2) | `variable` | Declared here |
| [`ctx`](#ctx-1) | `variable` | Declared here |
| [`codec`](#codec-6) | `variable` | Declared here |
| [`frame`](#frame-1) | `variable` | Declared here |
| [`conv`](#conv) | `variable` | Declared here |
| [`time`](#time-1) | `variable` | Declared here |
| [`pts`](#pts-1) | `variable` | Declared here |
| [`seconds`](#seconds-1) | `variable` | Declared here |
| [`error`](#error-3) | `variable` | Declared here |
| [`VideoContext`](#videocontext-1) | `function` | Declared here |
| [`VideoContext`](#videocontext-2) | `function` | Declared here |
| [`VideoContext`](#videocontext-3) | `function` | Declared here |
| [`create`](#create-3) | `function` | Declared here |
| [`open`](#open-1) | `function` | Declared here |
| [`close`](#close-6) | `function` | Declared here |
| [`decode`](#decode-2) | `function` | Declared here |
| [`encode`](#encode-6) | `function` | Declared here |
| [`encode`](#encode-7) | `function` | Declared here |
| [`encode`](#encode-8) | `function` | Declared here |
| [`flush`](#flush-4) | `function` | Declared here |
| [`convert`](#convert) | `function` | Declared here |
| [`recreateConverter`](#recreateconverter) | `function` | Declared here |

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

##### Reimplemented by

- [`create`](#create-4)
- [`create`](#create-5)

---

{#open-1}

#### open

`virtual`

```cpp
virtual void open()
```

Defined in src/av/include/icy/av/videocontext.h:58

Open the codec and create the pixel format conversion context if required. Throws std::runtime_error if the codec context has not been created.

##### Reimplemented by

- [`open`](#open-2)

---

{#close-6}

#### close

`virtual`

```cpp
virtual void close()
```

Defined in src/av/include/icy/av/videocontext.h:61

Close the codec context, free the frame, and reset timestamps.

##### Reimplemented by

- [`close`](#close-7)
- [`close`](#close-8)

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

##### Reimplemented by

- [`decode`](#decode-3)

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

##### Reimplemented by

- [`encode`](#encode-9)

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

##### Reimplemented by

- [`encode`](#encode-10)

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

##### Reimplemented by

- [`encode`](#encode-11)

---

{#flush-4}

#### flush

`virtual`

```cpp
virtual void flush()
```

Defined in src/av/include/icy/av/videocontext.h:88

Flush any frames buffered inside the codec and emit remaining output.

##### Reimplemented by

- [`flush`](#flush-5)
- [`flush`](#flush-6)

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`VideoDecoder`](#videodecoder-1) | `function` | Declared here |
| [`create`](#create-4) | `function` | Declared here |
| [`open`](#open-2) | `function` | Declared here |
| [`close`](#close-7) | `function` | Declared here |
| [`decode`](#decode-3) | `function` | Declared here |
| [`flush`](#flush-5) | `function` | Declared here |
| [`emitter`](#emitter-3) | `variable` | Inherited from [`VideoContext`](#videocontext) |
| [`iparams`](#iparams-1) | `variable` | Inherited from [`VideoContext`](#videocontext) |
| [`oparams`](#oparams-1) | `variable` | Inherited from [`VideoContext`](#videocontext) |
| [`stream`](#stream-2) | `variable` | Inherited from [`VideoContext`](#videocontext) |
| [`ctx`](#ctx-1) | `variable` | Inherited from [`VideoContext`](#videocontext) |
| [`codec`](#codec-6) | `variable` | Inherited from [`VideoContext`](#videocontext) |
| [`frame`](#frame-1) | `variable` | Inherited from [`VideoContext`](#videocontext) |
| [`conv`](#conv) | `variable` | Inherited from [`VideoContext`](#videocontext) |
| [`time`](#time-1) | `variable` | Inherited from [`VideoContext`](#videocontext) |
| [`pts`](#pts-1) | `variable` | Inherited from [`VideoContext`](#videocontext) |
| [`seconds`](#seconds-1) | `variable` | Inherited from [`VideoContext`](#videocontext) |
| [`error`](#error-3) | `variable` | Inherited from [`VideoContext`](#videocontext) |
| [`VideoContext`](#videocontext-1) | `function` | Inherited from [`VideoContext`](#videocontext) |
| [`VideoContext`](#videocontext-2) | `function` | Inherited from [`VideoContext`](#videocontext) |
| [`VideoContext`](#videocontext-3) | `function` | Inherited from [`VideoContext`](#videocontext) |
| [`create`](#create-3) | `function` | Inherited from [`VideoContext`](#videocontext) |
| [`open`](#open-1) | `function` | Inherited from [`VideoContext`](#videocontext) |
| [`close`](#close-6) | `function` | Inherited from [`VideoContext`](#videocontext) |
| [`decode`](#decode-2) | `function` | Inherited from [`VideoContext`](#videocontext) |
| [`encode`](#encode-6) | `function` | Inherited from [`VideoContext`](#videocontext) |
| [`encode`](#encode-7) | `function` | Inherited from [`VideoContext`](#videocontext) |
| [`encode`](#encode-8) | `function` | Inherited from [`VideoContext`](#videocontext) |
| [`flush`](#flush-4) | `function` | Inherited from [`VideoContext`](#videocontext) |
| [`convert`](#convert) | `function` | Inherited from [`VideoContext`](#videocontext) |
| [`recreateConverter`](#recreateconverter) | `function` | Inherited from [`VideoContext`](#videocontext) |

### Inherited from [`VideoContext`](#videocontext)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`emitter`](#emitter-3)  |  |
| `variable` | [`iparams`](#iparams-1)  | input parameters |
| `variable` | [`oparams`](#oparams-1)  | output parameters |
| `variable` | [`stream`](#stream-2)  | encoder or decoder stream |
| `variable` | [`ctx`](#ctx-1)  | encoder or decoder context |
| `variable` | [`codec`](#codec-6)  | encoder or decoder codec |
| `variable` | [`frame`](#frame-1)  | encoder or decoder frame |
| `variable` | [`conv`](#conv)  | video conversion context |
| `variable` | [`time`](#time-1)  | stream time in codec time base |
| `variable` | [`pts`](#pts-1)  | last packet pts value |
| `variable` | [`seconds`](#seconds-1)  | video time in seconds |
| `variable` | [`error`](#error-3)  | error message |
| `function` | [`VideoContext`](#videocontext-1)  |  |
| `function` | [`VideoContext`](#videocontext-2)  | Deleted constructor. |
| `function` | [`VideoContext`](#videocontext-3)  | Deleted constructor. |
| `function` | [`create`](#create-3) `virtual` | Initialise the AVCodecContext with codec-specific defaults. Overridden by [VideoEncoder](#videoencoder) and [VideoDecoder](#videodecoder). |
| `function` | [`open`](#open-1) `virtual` | Open the codec and create the pixel format conversion context if required. Throws std::runtime_error if the codec context has not been created. |
| `function` | [`close`](#close-6) `virtual` | Close the codec context, free the frame, and reset timestamps. |
| `function` | [`decode`](#decode-2) `virtual` `nodiscard` | Decode a compressed video packet and emit the resulting frame. |
| `function` | [`encode`](#encode-6) `virtual` `nodiscard` | Encode a buffer of interleaved video data. |
| `function` | [`encode`](#encode-7) `virtual` `nodiscard` | Encode a planar video frame. |
| `function` | [`encode`](#encode-8) `virtual` `nodiscard` | Encode a single AVFrame. |
| `function` | [`flush`](#flush-4) `virtual` | Flush any frames buffered inside the codec and emit remaining output. |
| `function` | [`convert`](#convert) `virtual` | Convert the video frame and return the result. |
| `function` | [`recreateConverter`](#recreateconverter) `virtual` | Recreate the [VideoConverter](#videoconverter) if the input or output parameters have changed. Called automatically by [open()](#open-1) and [convert()](#convert). |

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

##### Reimplements

- [`create`](#create-3)

---

{#open-2}

#### open

`virtual` `override`

```cpp
virtual void open() override
```

Defined in src/av/include/icy/av/videodecoder.h:40

Open the codec and initialise any required pixel format conversion context.

##### Reimplements

- [`open`](#open-1)

---

{#close-7}

#### close

`virtual` `override`

```cpp
virtual void close() override
```

Defined in src/av/include/icy/av/videodecoder.h:43

Close and free the AVCodecContext and associated resources.

##### Reimplements

- [`close`](#close-6)

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

##### Reimplements

- [`decode`](#decode-2)

---

{#flush-5}

#### flush

`virtual` `override`

```cpp
virtual void flush() override
```

Defined in src/av/include/icy/av/videodecoder.h:54

Flush any frames buffered inside the decoder. Call repeatedly after the last packet until false is returned.

##### Reimplements

- [`flush`](#flush-4)

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`format`](#format-7) | `variable` | Declared here |
| [`VideoEncoder`](#videoencoder-1) | `function` | Declared here |
| [`create`](#create-5) | `function` | Declared here |
| [`close`](#close-8) | `function` | Declared here |
| [`encode`](#encode-9) | `function` | Declared here |
| [`encode`](#encode-10) | `function` | Declared here |
| [`encode`](#encode-11) | `function` | Declared here |
| [`flush`](#flush-6) | `function` | Declared here |
| [`emitter`](#emitter-3) | `variable` | Inherited from [`VideoContext`](#videocontext) |
| [`iparams`](#iparams-1) | `variable` | Inherited from [`VideoContext`](#videocontext) |
| [`oparams`](#oparams-1) | `variable` | Inherited from [`VideoContext`](#videocontext) |
| [`stream`](#stream-2) | `variable` | Inherited from [`VideoContext`](#videocontext) |
| [`ctx`](#ctx-1) | `variable` | Inherited from [`VideoContext`](#videocontext) |
| [`codec`](#codec-6) | `variable` | Inherited from [`VideoContext`](#videocontext) |
| [`frame`](#frame-1) | `variable` | Inherited from [`VideoContext`](#videocontext) |
| [`conv`](#conv) | `variable` | Inherited from [`VideoContext`](#videocontext) |
| [`time`](#time-1) | `variable` | Inherited from [`VideoContext`](#videocontext) |
| [`pts`](#pts-1) | `variable` | Inherited from [`VideoContext`](#videocontext) |
| [`seconds`](#seconds-1) | `variable` | Inherited from [`VideoContext`](#videocontext) |
| [`error`](#error-3) | `variable` | Inherited from [`VideoContext`](#videocontext) |
| [`VideoContext`](#videocontext-1) | `function` | Inherited from [`VideoContext`](#videocontext) |
| [`VideoContext`](#videocontext-2) | `function` | Inherited from [`VideoContext`](#videocontext) |
| [`VideoContext`](#videocontext-3) | `function` | Inherited from [`VideoContext`](#videocontext) |
| [`create`](#create-3) | `function` | Inherited from [`VideoContext`](#videocontext) |
| [`open`](#open-1) | `function` | Inherited from [`VideoContext`](#videocontext) |
| [`close`](#close-6) | `function` | Inherited from [`VideoContext`](#videocontext) |
| [`decode`](#decode-2) | `function` | Inherited from [`VideoContext`](#videocontext) |
| [`encode`](#encode-6) | `function` | Inherited from [`VideoContext`](#videocontext) |
| [`encode`](#encode-7) | `function` | Inherited from [`VideoContext`](#videocontext) |
| [`encode`](#encode-8) | `function` | Inherited from [`VideoContext`](#videocontext) |
| [`flush`](#flush-4) | `function` | Inherited from [`VideoContext`](#videocontext) |
| [`convert`](#convert) | `function` | Inherited from [`VideoContext`](#videocontext) |
| [`recreateConverter`](#recreateconverter) | `function` | Inherited from [`VideoContext`](#videocontext) |

### Inherited from [`VideoContext`](#videocontext)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`emitter`](#emitter-3)  |  |
| `variable` | [`iparams`](#iparams-1)  | input parameters |
| `variable` | [`oparams`](#oparams-1)  | output parameters |
| `variable` | [`stream`](#stream-2)  | encoder or decoder stream |
| `variable` | [`ctx`](#ctx-1)  | encoder or decoder context |
| `variable` | [`codec`](#codec-6)  | encoder or decoder codec |
| `variable` | [`frame`](#frame-1)  | encoder or decoder frame |
| `variable` | [`conv`](#conv)  | video conversion context |
| `variable` | [`time`](#time-1)  | stream time in codec time base |
| `variable` | [`pts`](#pts-1)  | last packet pts value |
| `variable` | [`seconds`](#seconds-1)  | video time in seconds |
| `variable` | [`error`](#error-3)  | error message |
| `function` | [`VideoContext`](#videocontext-1)  |  |
| `function` | [`VideoContext`](#videocontext-2)  | Deleted constructor. |
| `function` | [`VideoContext`](#videocontext-3)  | Deleted constructor. |
| `function` | [`create`](#create-3) `virtual` | Initialise the AVCodecContext with codec-specific defaults. Overridden by [VideoEncoder](#videoencoder) and [VideoDecoder](#videodecoder). |
| `function` | [`open`](#open-1) `virtual` | Open the codec and create the pixel format conversion context if required. Throws std::runtime_error if the codec context has not been created. |
| `function` | [`close`](#close-6) `virtual` | Close the codec context, free the frame, and reset timestamps. |
| `function` | [`decode`](#decode-2) `virtual` `nodiscard` | Decode a compressed video packet and emit the resulting frame. |
| `function` | [`encode`](#encode-6) `virtual` `nodiscard` | Encode a buffer of interleaved video data. |
| `function` | [`encode`](#encode-7) `virtual` `nodiscard` | Encode a planar video frame. |
| `function` | [`encode`](#encode-8) `virtual` `nodiscard` | Encode a single AVFrame. |
| `function` | [`flush`](#flush-4) `virtual` | Flush any frames buffered inside the codec and emit remaining output. |
| `function` | [`convert`](#convert) `virtual` | Convert the video frame and return the result. |
| `function` | [`recreateConverter`](#recreateconverter) `virtual` | Recreate the [VideoConverter](#videoconverter) if the input or output parameters have changed. Called automatically by [open()](#open-1) and [convert()](#convert). |

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

##### Reimplements

- [`create`](#create-3)

---

{#close-8}

#### close

`virtual` `override`

```cpp
virtual void close() override
```

Defined in src/av/include/icy/av/videoencoder.h:42

Close and free the AVCodecContext and associated resources.

##### Reimplements

- [`close`](#close-6)

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

##### Reimplements

- [`encode`](#encode-6)

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

##### Reimplements

- [`encode`](#encode-7)

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

##### Reimplements

- [`encode`](#encode-8)

---

{#flush-6}

#### flush

`virtual` `override`

```cpp
virtual void flush() override
```

Defined in src/av/include/icy/av/videoencoder.h:67

Flush remaining packets to be encoded. This method should be called once before stream closure.

##### Reimplements

- [`flush`](#flush-4)

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`type`](#type-4) | `variable` | Declared here |
| [`id`](#id-1) | `variable` | Declared here |
| [`name`](#name-2) | `variable` | Declared here |
| [`isDefault`](#isdefault) | `variable` | Declared here |
| [`isConnected`](#isconnected) | `variable` | Declared here |
| [`isInUse`](#isinuse) | `variable` | Declared here |
| [`videoCapabilities`](#videocapabilities) | `variable` | Declared here |
| [`audioCapabilities`](#audiocapabilities) | `variable` | Declared here |
| [`Device`](#device-1) | `function` | Declared here |
| [`Device`](#device-2) | `function` | Declared here |
| [`print`](#print-4) | `function` | Declared here |
| [`operator==`](#operator-2) | `function` | Declared here |
| [`bestVideoCapability`](#bestvideocapability) | `function` | Declared here |
| [`bestAudioCapability`](#bestaudiocapability) | `function` | Declared here |
| [`Type`](#type-5) | `enum` | Declared here |

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`width`](#width-1) | `variable` | Declared here |
| [`height`](#height-1) | `variable` | Declared here |
| [`minFps`](#minfps) | `variable` | Declared here |
| [`maxFps`](#maxfps) | `variable` | Declared here |
| [`pixelFormat`](#pixelformat) | `variable` | Declared here |

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`sampleRate`](#samplerate-1) | `variable` | Declared here |
| [`channels`](#channels-1) | `variable` | Declared here |
| [`sampleFormat`](#sampleformat) | `variable` | Declared here |

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`ctx`](#ctx-2) | `variable` | Declared here |
| [`iparams`](#iparams-2) | `variable` | Declared here |
| [`oparams`](#oparams-2) | `variable` | Declared here |
| [`outSamples`](#outsamples) | `variable` | Declared here |
| [`outNumSamples`](#outnumsamples) | `variable` | Declared here |
| [`outBufferSize`](#outbuffersize) | `variable` | Declared here |
| [`maxNumSamples`](#maxnumsamples) | `variable` | Declared here |
| [`inSampleFmt`](#insamplefmt) | `variable` | Declared here |
| [`outSampleFmt`](#outsamplefmt) | `variable` | Declared here |
| [`AudioResampler`](#audioresampler-1) | `function` | Declared here |
| [`AudioResampler`](#audioresampler-2) | `function` | Declared here |
| [`AudioResampler`](#audioresampler-3) | `function` | Declared here |
| [`open`](#open-3) | `function` | Declared here |
| [`close`](#close-9) | `function` | Declared here |
| [`resample`](#resample) | `function` | Declared here |

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`ctx`](#ctx-3) | `variable` | Declared here |
| [`oframe`](#oframe) | `variable` | Declared here |
| [`iparams`](#iparams-3) | `variable` | Declared here |
| [`oparams`](#oparams-3) | `variable` | Declared here |
| [`VideoConverter`](#videoconverter-1) | `function` | Declared here |
| [`VideoConverter`](#videoconverter-2) | `function` | Declared here |
| [`VideoConverter`](#videoconverter-3) | `function` | Declared here |
| [`create`](#create-6) | `function` | Declared here |
| [`close`](#close-10) | `function` | Declared here |
| [`convert`](#convert-1) | `function` | Declared here |

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`time`](#time-2) | `variable` | Declared here |
| [`MediaPacket`](#mediapacket-1) | `function` | Declared here |
| [`MediaPacket`](#mediapacket-2) | `function` | Declared here |
| [`MediaPacket`](#mediapacket-3) | `function` | Declared here |
| [`clone`](#clone) | `function` | Declared here |
| [`className`](#classname) | `function` | Declared here |
| [`_data`](base.md#classicy_1_1RawPacket_1a31d0726502c953df1deea617ac639554) | `variable` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`_size`](base.md#classicy_1_1RawPacket_1a3e4ce8b933205ec4d1f62af467245dcd) | `variable` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`_owned`](base.md#classicy_1_1RawPacket_1a0a7cfa5a326df6b328d582f88f085ccb) | `variable` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`RawPacket`](base.md#classicy_1_1RawPacket_1afdfbea094d4a084cedc6889dc29a39d4) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`RawPacket`](base.md#classicy_1_1RawPacket_1a2cabea1456be6e46c7b21874f330b7ee) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`RawPacket`](base.md#classicy_1_1RawPacket_1a782274116d8a9ac3ade6d1f47a072f4c) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`~RawPacket`](base.md#classicy_1_1RawPacket_1a7018944eaa3a16a43a5b8de6554ffe56) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`clone`](base.md#classicy_1_1RawPacket_1a1ac09a8e23494bbd52939fedfe5a6a8c) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`copyData`](base.md#classicy_1_1RawPacket_1a1bb606fd34a1f45e1a90d046607886de) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`read`](base.md#classicy_1_1RawPacket_1a606a9688958cbc74f146d28c3042b769) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`write`](base.md#classicy_1_1RawPacket_1a6864137826313bac337b46de7d8ae2f4) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`data`](base.md#classicy_1_1RawPacket_1af40e88a78b327a4a3c609769ad1c9c20) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`size`](base.md#classicy_1_1RawPacket_1a9834f3f56c5ee160575a8fe6a4809dbf) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`className`](base.md#classicy_1_1RawPacket_1a41d94a78cbe6d6e7c074aa632deb6828) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`ownsBuffer`](base.md#classicy_1_1RawPacket_1a9e5982738a696bd9db11e74897194068) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`opaque`](base.md#classicy_1_1IPacket_1aa66babf1218198553ca9adede7618930) | `variable` | Inherited from [`IPacket`](base.md#ipacket) |
| [`info`](base.md#classicy_1_1IPacket_1ac2231c52e60bf60b402bc95cc9d6607d) | `variable` | Inherited from [`IPacket`](base.md#ipacket) |
| [`flags`](base.md#classicy_1_1IPacket_1a9b3947336184d08d69f70f406b84cdf0) | `variable` | Inherited from [`IPacket`](base.md#ipacket) |
| [`IPacket`](base.md#classicy_1_1IPacket_1af237b656e089ab2576cad8324769cbe0) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`IPacket`](base.md#classicy_1_1IPacket_1aae812f8e7e880df66d95eedf160c993b) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`operator=`](base.md#classicy_1_1IPacket_1adedceb54266652b65e183fd2c2882881) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`clone`](base.md#classicy_1_1IPacket_1a0a71c618520cdb93b89f5e29f8f145c7) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`~IPacket`](base.md#classicy_1_1IPacket_1a0419279bf5a558ec6d850fefcd44b437) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`read`](base.md#classicy_1_1IPacket_1ad42e0c52a5092acb3dd1488928902c5b) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`write`](base.md#classicy_1_1IPacket_1a87b978fc87c58eb05e1c01ad1ca60f4c) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`size`](base.md#classicy_1_1IPacket_1a86bfdad925cd6943165c3f8b4637db8c) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`hasData`](base.md#classicy_1_1IPacket_1afacbc458f0ce7820ef1345d85f7d6f80) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`data`](base.md#classicy_1_1IPacket_1a2e31cef156789c02205ec5c470d8c951) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`constData`](base.md#classicy_1_1IPacket_1a80c71c363bb1e20a5e24841d1f3e470c) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`className`](base.md#classicy_1_1IPacket_1abbbdc14886e839a7c09bd0eeac423ee1) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`print`](base.md#classicy_1_1IPacket_1acc8685ec343b3e9ed81687b8e8fa1acc) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`operator<<`](base.md#classicy_1_1IPacket_1a5157a8d7d50d1ff3c829e9c44c0a38ee) | `friend` | Inherited from [`IPacket`](base.md#ipacket) |

### Inherited from [`RawPacket`](base.md#rawpacket)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`_data`](base.md#classicy_1_1RawPacket_1a31d0726502c953df1deea617ac639554)  |  |
| `variable` | [`_size`](base.md#classicy_1_1RawPacket_1a3e4ce8b933205ec4d1f62af467245dcd)  |  |
| `variable` | [`_owned`](base.md#classicy_1_1RawPacket_1a0a7cfa5a326df6b328d582f88f085ccb)  |  |
| `function` | [`RawPacket`](base.md#classicy_1_1RawPacket_1afdfbea094d4a084cedc6889dc29a39d4) `inline` | Construct with borrowed (non-owning) buffer. |
| `function` | [`RawPacket`](base.md#classicy_1_1RawPacket_1a2cabea1456be6e46c7b21874f330b7ee) `inline` | Construct with const data (copied, owning). |
| `function` | [`RawPacket`](base.md#classicy_1_1RawPacket_1a782274116d8a9ac3ade6d1f47a072f4c) `inline` | Copy constructor (always copies data). |
| `function` | [`~RawPacket`](base.md#classicy_1_1RawPacket_1a7018944eaa3a16a43a5b8de6554ffe56) `virtual` | Defaulted destructor. |
| `function` | [`clone`](base.md#classicy_1_1RawPacket_1a1ac09a8e23494bbd52939fedfe5a6a8c) `virtual` `const` `inline` `override` |  |
| `function` | [`copyData`](base.md#classicy_1_1RawPacket_1a1bb606fd34a1f45e1a90d046607886de) `virtual` `inline` | Copies data into an internally owned buffer, replacing any prior content. |
| `function` | [`read`](base.md#classicy_1_1RawPacket_1a606a9688958cbc74f146d28c3042b769) `virtual` `inline` `override` | Reads from the buffer by copying its contents into an owned buffer. |
| `function` | [`write`](base.md#classicy_1_1RawPacket_1a6864137826313bac337b46de7d8ae2f4) `virtual` `const` `inline` `override` | Appends the packet data to the given output buffer. |
| `function` | [`data`](base.md#classicy_1_1RawPacket_1af40e88a78b327a4a3c609769ad1c9c20) `virtual` `const` `inline` `override` |  |
| `function` | [`size`](base.md#classicy_1_1RawPacket_1a9834f3f56c5ee160575a8fe6a4809dbf) `virtual` `const` `inline` `override` |  |
| `function` | [`className`](base.md#classicy_1_1RawPacket_1a41d94a78cbe6d6e7c074aa632deb6828) `virtual` `const` `inline` `override` | Returns the class name of this packet type for logging and diagnostics. |
| `function` | [`ownsBuffer`](base.md#classicy_1_1RawPacket_1a9e5982738a696bd9db11e74897194068) `const` `inline` |  |

### Inherited from [`IPacket`](base.md#ipacket)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`opaque`](base.md#classicy_1_1IPacket_1aa66babf1218198553ca9adede7618930)  | Optional type-safe context data. Use std::any_cast to retrieve. Lifetime of the stored value is tied to the packet's lifetime. |
| `variable` | [`info`](base.md#classicy_1_1IPacket_1ac2231c52e60bf60b402bc95cc9d6607d)  | Optional extra information about the packet. |
| `variable` | [`flags`](base.md#classicy_1_1IPacket_1a9b3947336184d08d69f70f406b84cdf0)  | Provides basic information about the packet. |
| `function` | [`IPacket`](base.md#classicy_1_1IPacket_1af237b656e089ab2576cad8324769cbe0) `inline` |  |
| `function` | [`IPacket`](base.md#classicy_1_1IPacket_1aae812f8e7e880df66d95eedf160c993b) `inline` | Copy constructor; clones the info object if present. |
| `function` | [`operator=`](base.md#classicy_1_1IPacket_1adedceb54266652b65e183fd2c2882881) `inline` | Copy assignment; clones the info object if present. |
| `function` | [`clone`](base.md#classicy_1_1IPacket_1a0a71c618520cdb93b89f5e29f8f145c7) `virtual` `const` | Returns a heap-allocated deep copy of this packet. |
| `function` | [`~IPacket`](base.md#classicy_1_1IPacket_1a0419279bf5a558ec6d850fefcd44b437) `virtual` | Defaulted destructor. |
| `function` | [`read`](base.md#classicy_1_1IPacket_1ad42e0c52a5092acb3dd1488928902c5b) `virtual` | Read/parse to the packet from the given input buffer. The number of bytes read is returned. |
| `function` | [`write`](base.md#classicy_1_1IPacket_1a87b978fc87c58eb05e1c01ad1ca60f4c) `virtual` `const` | Copy/generate to the packet given output buffer. The number of bytes written can be obtained from the buffer. |
| `function` | [`size`](base.md#classicy_1_1IPacket_1a86bfdad925cd6943165c3f8b4637db8c) `virtual` `const` `inline` | The size of the packet in bytes. |
| `function` | [`hasData`](base.md#classicy_1_1IPacket_1afacbc458f0ce7820ef1345d85f7d6f80) `virtual` `const` `inline` | Returns true if the packet has a non-null data pointer. |
| `function` | [`data`](base.md#classicy_1_1IPacket_1a2e31cef156789c02205ec5c470d8c951) `virtual` `const` `inline` | The packet data pointer for buffered packets. |
| `function` | [`constData`](base.md#classicy_1_1IPacket_1a80c71c363bb1e20a5e24841d1f3e470c) `virtual` `const` `inline` | The const packet data pointer for buffered packets. |
| `function` | [`className`](base.md#classicy_1_1IPacket_1abbbdc14886e839a7c09bd0eeac423ee1) `virtual` `const` | Returns the class name of this packet type for logging and diagnostics. |
| `function` | [`print`](base.md#classicy_1_1IPacket_1acc8685ec343b3e9ed81687b8e8fa1acc) `virtual` `const` `inline` | Prints a human-readable representation to the given stream. |
| `friend` | [`operator<<`](base.md#classicy_1_1IPacket_1a5157a8d7d50d1ff3c829e9c44c0a38ee) `inline` | [Stream](base.md#stream) insertion operator; delegates to [print()](base.md#classicy_1_1IPacket_1acc8685ec343b3e9ed81687b8e8fa1acc). |

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

##### Reimplements

- [`clone`](base.md#classicy_1_1RawPacket_1a1ac09a8e23494bbd52939fedfe5a6a8c)

##### Reimplemented by

- [`clone`](#clone-3)
- [`clone`](#clone-4)
- [`clone`](#clone-2)
- [`clone`](#clone-1)
- [`clone`](vision.md#clone-19)

---

{#classname}

#### className

`virtual` `const` `inline` `override`

```cpp
virtual inline const char * className() const override
```

Defined in src/av/include/icy/av/packet.h:64

Returns the class name of this packet type for logging and diagnostics.

##### Reimplements

- [`className`](base.md#classicy_1_1RawPacket_1a41d94a78cbe6d6e7c074aa632deb6828)

##### Reimplemented by

- [`className`](#classname-3)
- [`className`](#classname-4)
- [`className`](#classname-2)
- [`className`](#classname-1)
- [`className`](vision.md#classname-11)

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`width`](#width-2) | `variable` | Declared here |
| [`height`](#height-2) | `variable` | Declared here |
| [`iframe`](#iframe) | `variable` | Declared here |
| [`avpacket`](#avpacket) | `variable` | Declared here |
| [`VideoPacket`](#videopacket-1) | `function` | Declared here |
| [`VideoPacket`](#videopacket-2) | `function` | Declared here |
| [`clone`](#clone-1) | `function` | Declared here |
| [`className`](#classname-1) | `function` | Declared here |
| [`time`](#time-2) | `variable` | Inherited from [`MediaPacket`](#mediapacket) |
| [`MediaPacket`](#mediapacket-1) | `function` | Inherited from [`MediaPacket`](#mediapacket) |
| [`MediaPacket`](#mediapacket-2) | `function` | Inherited from [`MediaPacket`](#mediapacket) |
| [`MediaPacket`](#mediapacket-3) | `function` | Inherited from [`MediaPacket`](#mediapacket) |
| [`clone`](#clone) | `function` | Inherited from [`MediaPacket`](#mediapacket) |
| [`className`](#classname) | `function` | Inherited from [`MediaPacket`](#mediapacket) |
| [`_data`](base.md#classicy_1_1RawPacket_1a31d0726502c953df1deea617ac639554) | `variable` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`_size`](base.md#classicy_1_1RawPacket_1a3e4ce8b933205ec4d1f62af467245dcd) | `variable` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`_owned`](base.md#classicy_1_1RawPacket_1a0a7cfa5a326df6b328d582f88f085ccb) | `variable` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`RawPacket`](base.md#classicy_1_1RawPacket_1afdfbea094d4a084cedc6889dc29a39d4) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`RawPacket`](base.md#classicy_1_1RawPacket_1a2cabea1456be6e46c7b21874f330b7ee) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`RawPacket`](base.md#classicy_1_1RawPacket_1a782274116d8a9ac3ade6d1f47a072f4c) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`~RawPacket`](base.md#classicy_1_1RawPacket_1a7018944eaa3a16a43a5b8de6554ffe56) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`clone`](base.md#classicy_1_1RawPacket_1a1ac09a8e23494bbd52939fedfe5a6a8c) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`copyData`](base.md#classicy_1_1RawPacket_1a1bb606fd34a1f45e1a90d046607886de) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`read`](base.md#classicy_1_1RawPacket_1a606a9688958cbc74f146d28c3042b769) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`write`](base.md#classicy_1_1RawPacket_1a6864137826313bac337b46de7d8ae2f4) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`data`](base.md#classicy_1_1RawPacket_1af40e88a78b327a4a3c609769ad1c9c20) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`size`](base.md#classicy_1_1RawPacket_1a9834f3f56c5ee160575a8fe6a4809dbf) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`className`](base.md#classicy_1_1RawPacket_1a41d94a78cbe6d6e7c074aa632deb6828) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`ownsBuffer`](base.md#classicy_1_1RawPacket_1a9e5982738a696bd9db11e74897194068) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`opaque`](base.md#classicy_1_1IPacket_1aa66babf1218198553ca9adede7618930) | `variable` | Inherited from [`IPacket`](base.md#ipacket) |
| [`info`](base.md#classicy_1_1IPacket_1ac2231c52e60bf60b402bc95cc9d6607d) | `variable` | Inherited from [`IPacket`](base.md#ipacket) |
| [`flags`](base.md#classicy_1_1IPacket_1a9b3947336184d08d69f70f406b84cdf0) | `variable` | Inherited from [`IPacket`](base.md#ipacket) |
| [`IPacket`](base.md#classicy_1_1IPacket_1af237b656e089ab2576cad8324769cbe0) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`IPacket`](base.md#classicy_1_1IPacket_1aae812f8e7e880df66d95eedf160c993b) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`operator=`](base.md#classicy_1_1IPacket_1adedceb54266652b65e183fd2c2882881) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`clone`](base.md#classicy_1_1IPacket_1a0a71c618520cdb93b89f5e29f8f145c7) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`~IPacket`](base.md#classicy_1_1IPacket_1a0419279bf5a558ec6d850fefcd44b437) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`read`](base.md#classicy_1_1IPacket_1ad42e0c52a5092acb3dd1488928902c5b) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`write`](base.md#classicy_1_1IPacket_1a87b978fc87c58eb05e1c01ad1ca60f4c) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`size`](base.md#classicy_1_1IPacket_1a86bfdad925cd6943165c3f8b4637db8c) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`hasData`](base.md#classicy_1_1IPacket_1afacbc458f0ce7820ef1345d85f7d6f80) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`data`](base.md#classicy_1_1IPacket_1a2e31cef156789c02205ec5c470d8c951) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`constData`](base.md#classicy_1_1IPacket_1a80c71c363bb1e20a5e24841d1f3e470c) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`className`](base.md#classicy_1_1IPacket_1abbbdc14886e839a7c09bd0eeac423ee1) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`print`](base.md#classicy_1_1IPacket_1acc8685ec343b3e9ed81687b8e8fa1acc) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`operator<<`](base.md#classicy_1_1IPacket_1a5157a8d7d50d1ff3c829e9c44c0a38ee) | `friend` | Inherited from [`IPacket`](base.md#ipacket) |

### Inherited from [`MediaPacket`](#mediapacket)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`time`](#time-2)  | Presentation timestamp in microseconds. |
| `function` | [`MediaPacket`](#mediapacket-1) `inline` | Construct with a non-owning or owning mutable buffer. |
| `function` | [`MediaPacket`](#mediapacket-2) `inline` | Construct with const data (copied, owning). |
| `function` | [`MediaPacket`](#mediapacket-3) `inline` | Copy constructor. |
| `function` | [`clone`](#clone) `virtual` `const` `inline` `override` |  |
| `function` | [`className`](#classname) `virtual` `const` `inline` `override` | Returns the class name of this packet type for logging and diagnostics. |

### Inherited from [`RawPacket`](base.md#rawpacket)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`_data`](base.md#classicy_1_1RawPacket_1a31d0726502c953df1deea617ac639554)  |  |
| `variable` | [`_size`](base.md#classicy_1_1RawPacket_1a3e4ce8b933205ec4d1f62af467245dcd)  |  |
| `variable` | [`_owned`](base.md#classicy_1_1RawPacket_1a0a7cfa5a326df6b328d582f88f085ccb)  |  |
| `function` | [`RawPacket`](base.md#classicy_1_1RawPacket_1afdfbea094d4a084cedc6889dc29a39d4) `inline` | Construct with borrowed (non-owning) buffer. |
| `function` | [`RawPacket`](base.md#classicy_1_1RawPacket_1a2cabea1456be6e46c7b21874f330b7ee) `inline` | Construct with const data (copied, owning). |
| `function` | [`RawPacket`](base.md#classicy_1_1RawPacket_1a782274116d8a9ac3ade6d1f47a072f4c) `inline` | Copy constructor (always copies data). |
| `function` | [`~RawPacket`](base.md#classicy_1_1RawPacket_1a7018944eaa3a16a43a5b8de6554ffe56) `virtual` | Defaulted destructor. |
| `function` | [`clone`](base.md#classicy_1_1RawPacket_1a1ac09a8e23494bbd52939fedfe5a6a8c) `virtual` `const` `inline` `override` |  |
| `function` | [`copyData`](base.md#classicy_1_1RawPacket_1a1bb606fd34a1f45e1a90d046607886de) `virtual` `inline` | Copies data into an internally owned buffer, replacing any prior content. |
| `function` | [`read`](base.md#classicy_1_1RawPacket_1a606a9688958cbc74f146d28c3042b769) `virtual` `inline` `override` | Reads from the buffer by copying its contents into an owned buffer. |
| `function` | [`write`](base.md#classicy_1_1RawPacket_1a6864137826313bac337b46de7d8ae2f4) `virtual` `const` `inline` `override` | Appends the packet data to the given output buffer. |
| `function` | [`data`](base.md#classicy_1_1RawPacket_1af40e88a78b327a4a3c609769ad1c9c20) `virtual` `const` `inline` `override` |  |
| `function` | [`size`](base.md#classicy_1_1RawPacket_1a9834f3f56c5ee160575a8fe6a4809dbf) `virtual` `const` `inline` `override` |  |
| `function` | [`className`](base.md#classicy_1_1RawPacket_1a41d94a78cbe6d6e7c074aa632deb6828) `virtual` `const` `inline` `override` | Returns the class name of this packet type for logging and diagnostics. |
| `function` | [`ownsBuffer`](base.md#classicy_1_1RawPacket_1a9e5982738a696bd9db11e74897194068) `const` `inline` |  |

### Inherited from [`IPacket`](base.md#ipacket)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`opaque`](base.md#classicy_1_1IPacket_1aa66babf1218198553ca9adede7618930)  | Optional type-safe context data. Use std::any_cast to retrieve. Lifetime of the stored value is tied to the packet's lifetime. |
| `variable` | [`info`](base.md#classicy_1_1IPacket_1ac2231c52e60bf60b402bc95cc9d6607d)  | Optional extra information about the packet. |
| `variable` | [`flags`](base.md#classicy_1_1IPacket_1a9b3947336184d08d69f70f406b84cdf0)  | Provides basic information about the packet. |
| `function` | [`IPacket`](base.md#classicy_1_1IPacket_1af237b656e089ab2576cad8324769cbe0) `inline` |  |
| `function` | [`IPacket`](base.md#classicy_1_1IPacket_1aae812f8e7e880df66d95eedf160c993b) `inline` | Copy constructor; clones the info object if present. |
| `function` | [`operator=`](base.md#classicy_1_1IPacket_1adedceb54266652b65e183fd2c2882881) `inline` | Copy assignment; clones the info object if present. |
| `function` | [`clone`](base.md#classicy_1_1IPacket_1a0a71c618520cdb93b89f5e29f8f145c7) `virtual` `const` | Returns a heap-allocated deep copy of this packet. |
| `function` | [`~IPacket`](base.md#classicy_1_1IPacket_1a0419279bf5a558ec6d850fefcd44b437) `virtual` | Defaulted destructor. |
| `function` | [`read`](base.md#classicy_1_1IPacket_1ad42e0c52a5092acb3dd1488928902c5b) `virtual` | Read/parse to the packet from the given input buffer. The number of bytes read is returned. |
| `function` | [`write`](base.md#classicy_1_1IPacket_1a87b978fc87c58eb05e1c01ad1ca60f4c) `virtual` `const` | Copy/generate to the packet given output buffer. The number of bytes written can be obtained from the buffer. |
| `function` | [`size`](base.md#classicy_1_1IPacket_1a86bfdad925cd6943165c3f8b4637db8c) `virtual` `const` `inline` | The size of the packet in bytes. |
| `function` | [`hasData`](base.md#classicy_1_1IPacket_1afacbc458f0ce7820ef1345d85f7d6f80) `virtual` `const` `inline` | Returns true if the packet has a non-null data pointer. |
| `function` | [`data`](base.md#classicy_1_1IPacket_1a2e31cef156789c02205ec5c470d8c951) `virtual` `const` `inline` | The packet data pointer for buffered packets. |
| `function` | [`constData`](base.md#classicy_1_1IPacket_1a80c71c363bb1e20a5e24841d1f3e470c) `virtual` `const` `inline` | The const packet data pointer for buffered packets. |
| `function` | [`className`](base.md#classicy_1_1IPacket_1abbbdc14886e839a7c09bd0eeac423ee1) `virtual` `const` | Returns the class name of this packet type for logging and diagnostics. |
| `function` | [`print`](base.md#classicy_1_1IPacket_1acc8685ec343b3e9ed81687b8e8fa1acc) `virtual` `const` `inline` | Prints a human-readable representation to the given stream. |
| `friend` | [`operator<<`](base.md#classicy_1_1IPacket_1a5157a8d7d50d1ff3c829e9c44c0a38ee) `inline` | [Stream](base.md#stream) insertion operator; delegates to [print()](base.md#classicy_1_1IPacket_1acc8685ec343b3e9ed81687b8e8fa1acc). |

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

##### Reimplements

- [`clone`](#clone)

##### Reimplemented by

- [`clone`](#clone-2)
- [`clone`](vision.md#clone-19)

---

{#classname-1}

#### className

`virtual` `const` `inline` `override`

```cpp
virtual inline const char * className() const override
```

Defined in src/av/include/icy/av/packet.h:109

Returns the class name of this packet type for logging and diagnostics.

##### Reimplements

- [`className`](#classname)

##### Reimplemented by

- [`className`](#classname-2)
- [`className`](vision.md#classname-11)

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`buffer`](#buffer) | `variable` | Declared here |
| [`linesize`](#linesize) | `variable` | Declared here |
| [`pixelFmt`](#pixelfmt-1) | `variable` | Declared here |
| [`owns_buffer`](#owns_buffer) | `variable` | Declared here |
| [`avframe`](#avframe) | `variable` | Declared here |
| [`PlanarVideoPacket`](#planarvideopacket-1) | `function` | Declared here |
| [`PlanarVideoPacket`](#planarvideopacket-2) | `function` | Declared here |
| [`clone`](#clone-2) | `function` | Declared here |
| [`className`](#classname-2) | `function` | Declared here |
| [`width`](#width-2) | `variable` | Inherited from [`VideoPacket`](#videopacket) |
| [`height`](#height-2) | `variable` | Inherited from [`VideoPacket`](#videopacket) |
| [`iframe`](#iframe) | `variable` | Inherited from [`VideoPacket`](#videopacket) |
| [`avpacket`](#avpacket) | `variable` | Inherited from [`VideoPacket`](#videopacket) |
| [`VideoPacket`](#videopacket-1) | `function` | Inherited from [`VideoPacket`](#videopacket) |
| [`VideoPacket`](#videopacket-2) | `function` | Inherited from [`VideoPacket`](#videopacket) |
| [`clone`](#clone-1) | `function` | Inherited from [`VideoPacket`](#videopacket) |
| [`className`](#classname-1) | `function` | Inherited from [`VideoPacket`](#videopacket) |
| [`time`](#time-2) | `variable` | Inherited from [`MediaPacket`](#mediapacket) |
| [`MediaPacket`](#mediapacket-1) | `function` | Inherited from [`MediaPacket`](#mediapacket) |
| [`MediaPacket`](#mediapacket-2) | `function` | Inherited from [`MediaPacket`](#mediapacket) |
| [`MediaPacket`](#mediapacket-3) | `function` | Inherited from [`MediaPacket`](#mediapacket) |
| [`clone`](#clone) | `function` | Inherited from [`MediaPacket`](#mediapacket) |
| [`className`](#classname) | `function` | Inherited from [`MediaPacket`](#mediapacket) |
| [`_data`](base.md#classicy_1_1RawPacket_1a31d0726502c953df1deea617ac639554) | `variable` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`_size`](base.md#classicy_1_1RawPacket_1a3e4ce8b933205ec4d1f62af467245dcd) | `variable` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`_owned`](base.md#classicy_1_1RawPacket_1a0a7cfa5a326df6b328d582f88f085ccb) | `variable` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`RawPacket`](base.md#classicy_1_1RawPacket_1afdfbea094d4a084cedc6889dc29a39d4) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`RawPacket`](base.md#classicy_1_1RawPacket_1a2cabea1456be6e46c7b21874f330b7ee) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`RawPacket`](base.md#classicy_1_1RawPacket_1a782274116d8a9ac3ade6d1f47a072f4c) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`~RawPacket`](base.md#classicy_1_1RawPacket_1a7018944eaa3a16a43a5b8de6554ffe56) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`clone`](base.md#classicy_1_1RawPacket_1a1ac09a8e23494bbd52939fedfe5a6a8c) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`copyData`](base.md#classicy_1_1RawPacket_1a1bb606fd34a1f45e1a90d046607886de) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`read`](base.md#classicy_1_1RawPacket_1a606a9688958cbc74f146d28c3042b769) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`write`](base.md#classicy_1_1RawPacket_1a6864137826313bac337b46de7d8ae2f4) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`data`](base.md#classicy_1_1RawPacket_1af40e88a78b327a4a3c609769ad1c9c20) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`size`](base.md#classicy_1_1RawPacket_1a9834f3f56c5ee160575a8fe6a4809dbf) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`className`](base.md#classicy_1_1RawPacket_1a41d94a78cbe6d6e7c074aa632deb6828) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`ownsBuffer`](base.md#classicy_1_1RawPacket_1a9e5982738a696bd9db11e74897194068) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`opaque`](base.md#classicy_1_1IPacket_1aa66babf1218198553ca9adede7618930) | `variable` | Inherited from [`IPacket`](base.md#ipacket) |
| [`info`](base.md#classicy_1_1IPacket_1ac2231c52e60bf60b402bc95cc9d6607d) | `variable` | Inherited from [`IPacket`](base.md#ipacket) |
| [`flags`](base.md#classicy_1_1IPacket_1a9b3947336184d08d69f70f406b84cdf0) | `variable` | Inherited from [`IPacket`](base.md#ipacket) |
| [`IPacket`](base.md#classicy_1_1IPacket_1af237b656e089ab2576cad8324769cbe0) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`IPacket`](base.md#classicy_1_1IPacket_1aae812f8e7e880df66d95eedf160c993b) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`operator=`](base.md#classicy_1_1IPacket_1adedceb54266652b65e183fd2c2882881) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`clone`](base.md#classicy_1_1IPacket_1a0a71c618520cdb93b89f5e29f8f145c7) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`~IPacket`](base.md#classicy_1_1IPacket_1a0419279bf5a558ec6d850fefcd44b437) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`read`](base.md#classicy_1_1IPacket_1ad42e0c52a5092acb3dd1488928902c5b) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`write`](base.md#classicy_1_1IPacket_1a87b978fc87c58eb05e1c01ad1ca60f4c) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`size`](base.md#classicy_1_1IPacket_1a86bfdad925cd6943165c3f8b4637db8c) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`hasData`](base.md#classicy_1_1IPacket_1afacbc458f0ce7820ef1345d85f7d6f80) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`data`](base.md#classicy_1_1IPacket_1a2e31cef156789c02205ec5c470d8c951) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`constData`](base.md#classicy_1_1IPacket_1a80c71c363bb1e20a5e24841d1f3e470c) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`className`](base.md#classicy_1_1IPacket_1abbbdc14886e839a7c09bd0eeac423ee1) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`print`](base.md#classicy_1_1IPacket_1acc8685ec343b3e9ed81687b8e8fa1acc) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`operator<<`](base.md#classicy_1_1IPacket_1a5157a8d7d50d1ff3c829e9c44c0a38ee) | `friend` | Inherited from [`IPacket`](base.md#ipacket) |

### Inherited from [`VideoPacket`](#videopacket)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`width`](#width-2)  | Frame width in pixels. |
| `variable` | [`height`](#height-2)  | Frame height in pixels. |
| `variable` | [`iframe`](#iframe)  | True if this is an intra (keyframe) frame. |
| `variable` | [`avpacket`](#avpacket)  | Non-owning pointer to the encoded AVPacket from FFmpeg. Set by [VideoEncoder](#videoencoder), read by [MultiplexEncoder](#multiplexencoder). |
| `function` | [`VideoPacket`](#videopacket-1) `inline` | Construct a video packet with an interleaved buffer. |
| `function` | [`VideoPacket`](#videopacket-2) `inline` | Copy constructor. The avpacket pointer is shallow-copied (non-owning). |
| `function` | [`clone`](#clone-1) `virtual` `const` `inline` `override` |  |
| `function` | [`className`](#classname-1) `virtual` `const` `inline` `override` | Returns the class name of this packet type for logging and diagnostics. |

### Inherited from [`MediaPacket`](#mediapacket)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`time`](#time-2)  | Presentation timestamp in microseconds. |
| `function` | [`MediaPacket`](#mediapacket-1) `inline` | Construct with a non-owning or owning mutable buffer. |
| `function` | [`MediaPacket`](#mediapacket-2) `inline` | Construct with const data (copied, owning). |
| `function` | [`MediaPacket`](#mediapacket-3) `inline` | Copy constructor. |
| `function` | [`clone`](#clone) `virtual` `const` `inline` `override` |  |
| `function` | [`className`](#classname) `virtual` `const` `inline` `override` | Returns the class name of this packet type for logging and diagnostics. |

### Inherited from [`RawPacket`](base.md#rawpacket)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`_data`](base.md#classicy_1_1RawPacket_1a31d0726502c953df1deea617ac639554)  |  |
| `variable` | [`_size`](base.md#classicy_1_1RawPacket_1a3e4ce8b933205ec4d1f62af467245dcd)  |  |
| `variable` | [`_owned`](base.md#classicy_1_1RawPacket_1a0a7cfa5a326df6b328d582f88f085ccb)  |  |
| `function` | [`RawPacket`](base.md#classicy_1_1RawPacket_1afdfbea094d4a084cedc6889dc29a39d4) `inline` | Construct with borrowed (non-owning) buffer. |
| `function` | [`RawPacket`](base.md#classicy_1_1RawPacket_1a2cabea1456be6e46c7b21874f330b7ee) `inline` | Construct with const data (copied, owning). |
| `function` | [`RawPacket`](base.md#classicy_1_1RawPacket_1a782274116d8a9ac3ade6d1f47a072f4c) `inline` | Copy constructor (always copies data). |
| `function` | [`~RawPacket`](base.md#classicy_1_1RawPacket_1a7018944eaa3a16a43a5b8de6554ffe56) `virtual` | Defaulted destructor. |
| `function` | [`clone`](base.md#classicy_1_1RawPacket_1a1ac09a8e23494bbd52939fedfe5a6a8c) `virtual` `const` `inline` `override` |  |
| `function` | [`copyData`](base.md#classicy_1_1RawPacket_1a1bb606fd34a1f45e1a90d046607886de) `virtual` `inline` | Copies data into an internally owned buffer, replacing any prior content. |
| `function` | [`read`](base.md#classicy_1_1RawPacket_1a606a9688958cbc74f146d28c3042b769) `virtual` `inline` `override` | Reads from the buffer by copying its contents into an owned buffer. |
| `function` | [`write`](base.md#classicy_1_1RawPacket_1a6864137826313bac337b46de7d8ae2f4) `virtual` `const` `inline` `override` | Appends the packet data to the given output buffer. |
| `function` | [`data`](base.md#classicy_1_1RawPacket_1af40e88a78b327a4a3c609769ad1c9c20) `virtual` `const` `inline` `override` |  |
| `function` | [`size`](base.md#classicy_1_1RawPacket_1a9834f3f56c5ee160575a8fe6a4809dbf) `virtual` `const` `inline` `override` |  |
| `function` | [`className`](base.md#classicy_1_1RawPacket_1a41d94a78cbe6d6e7c074aa632deb6828) `virtual` `const` `inline` `override` | Returns the class name of this packet type for logging and diagnostics. |
| `function` | [`ownsBuffer`](base.md#classicy_1_1RawPacket_1a9e5982738a696bd9db11e74897194068) `const` `inline` |  |

### Inherited from [`IPacket`](base.md#ipacket)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`opaque`](base.md#classicy_1_1IPacket_1aa66babf1218198553ca9adede7618930)  | Optional type-safe context data. Use std::any_cast to retrieve. Lifetime of the stored value is tied to the packet's lifetime. |
| `variable` | [`info`](base.md#classicy_1_1IPacket_1ac2231c52e60bf60b402bc95cc9d6607d)  | Optional extra information about the packet. |
| `variable` | [`flags`](base.md#classicy_1_1IPacket_1a9b3947336184d08d69f70f406b84cdf0)  | Provides basic information about the packet. |
| `function` | [`IPacket`](base.md#classicy_1_1IPacket_1af237b656e089ab2576cad8324769cbe0) `inline` |  |
| `function` | [`IPacket`](base.md#classicy_1_1IPacket_1aae812f8e7e880df66d95eedf160c993b) `inline` | Copy constructor; clones the info object if present. |
| `function` | [`operator=`](base.md#classicy_1_1IPacket_1adedceb54266652b65e183fd2c2882881) `inline` | Copy assignment; clones the info object if present. |
| `function` | [`clone`](base.md#classicy_1_1IPacket_1a0a71c618520cdb93b89f5e29f8f145c7) `virtual` `const` | Returns a heap-allocated deep copy of this packet. |
| `function` | [`~IPacket`](base.md#classicy_1_1IPacket_1a0419279bf5a558ec6d850fefcd44b437) `virtual` | Defaulted destructor. |
| `function` | [`read`](base.md#classicy_1_1IPacket_1ad42e0c52a5092acb3dd1488928902c5b) `virtual` | Read/parse to the packet from the given input buffer. The number of bytes read is returned. |
| `function` | [`write`](base.md#classicy_1_1IPacket_1a87b978fc87c58eb05e1c01ad1ca60f4c) `virtual` `const` | Copy/generate to the packet given output buffer. The number of bytes written can be obtained from the buffer. |
| `function` | [`size`](base.md#classicy_1_1IPacket_1a86bfdad925cd6943165c3f8b4637db8c) `virtual` `const` `inline` | The size of the packet in bytes. |
| `function` | [`hasData`](base.md#classicy_1_1IPacket_1afacbc458f0ce7820ef1345d85f7d6f80) `virtual` `const` `inline` | Returns true if the packet has a non-null data pointer. |
| `function` | [`data`](base.md#classicy_1_1IPacket_1a2e31cef156789c02205ec5c470d8c951) `virtual` `const` `inline` | The packet data pointer for buffered packets. |
| `function` | [`constData`](base.md#classicy_1_1IPacket_1a80c71c363bb1e20a5e24841d1f3e470c) `virtual` `const` `inline` | The const packet data pointer for buffered packets. |
| `function` | [`className`](base.md#classicy_1_1IPacket_1abbbdc14886e839a7c09bd0eeac423ee1) `virtual` `const` | Returns the class name of this packet type for logging and diagnostics. |
| `function` | [`print`](base.md#classicy_1_1IPacket_1acc8685ec343b3e9ed81687b8e8fa1acc) `virtual` `const` `inline` | Prints a human-readable representation to the given stream. |
| `friend` | [`operator<<`](base.md#classicy_1_1IPacket_1a5157a8d7d50d1ff3c829e9c44c0a38ee) `inline` | [Stream](base.md#stream) insertion operator; delegates to [print()](base.md#classicy_1_1IPacket_1acc8685ec343b3e9ed81687b8e8fa1acc). |

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

##### Reimplements

- [`clone`](#clone-1)

##### Reimplemented by

- [`clone`](vision.md#clone-19)

---

{#classname-2}

#### className

`virtual` `const` `inline` `override`

```cpp
virtual inline const char * className() const override
```

Defined in src/av/include/icy/av/packet.h:149

Returns the class name of this packet type for logging and diagnostics.

##### Reimplements

- [`className`](#classname-1)

##### Reimplemented by

- [`className`](vision.md#classname-11)

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`numSamples`](#numsamples) | `variable` | Declared here |
| [`avpacket`](#avpacket-1) | `variable` | Declared here |
| [`AudioPacket`](#audiopacket-1) | `function` | Declared here |
| [`clone`](#clone-3) | `function` | Declared here |
| [`samples`](#samples) | `function` | Declared here |
| [`className`](#classname-3) | `function` | Declared here |
| [`time`](#time-2) | `variable` | Inherited from [`MediaPacket`](#mediapacket) |
| [`MediaPacket`](#mediapacket-1) | `function` | Inherited from [`MediaPacket`](#mediapacket) |
| [`MediaPacket`](#mediapacket-2) | `function` | Inherited from [`MediaPacket`](#mediapacket) |
| [`MediaPacket`](#mediapacket-3) | `function` | Inherited from [`MediaPacket`](#mediapacket) |
| [`clone`](#clone) | `function` | Inherited from [`MediaPacket`](#mediapacket) |
| [`className`](#classname) | `function` | Inherited from [`MediaPacket`](#mediapacket) |
| [`_data`](base.md#classicy_1_1RawPacket_1a31d0726502c953df1deea617ac639554) | `variable` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`_size`](base.md#classicy_1_1RawPacket_1a3e4ce8b933205ec4d1f62af467245dcd) | `variable` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`_owned`](base.md#classicy_1_1RawPacket_1a0a7cfa5a326df6b328d582f88f085ccb) | `variable` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`RawPacket`](base.md#classicy_1_1RawPacket_1afdfbea094d4a084cedc6889dc29a39d4) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`RawPacket`](base.md#classicy_1_1RawPacket_1a2cabea1456be6e46c7b21874f330b7ee) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`RawPacket`](base.md#classicy_1_1RawPacket_1a782274116d8a9ac3ade6d1f47a072f4c) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`~RawPacket`](base.md#classicy_1_1RawPacket_1a7018944eaa3a16a43a5b8de6554ffe56) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`clone`](base.md#classicy_1_1RawPacket_1a1ac09a8e23494bbd52939fedfe5a6a8c) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`copyData`](base.md#classicy_1_1RawPacket_1a1bb606fd34a1f45e1a90d046607886de) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`read`](base.md#classicy_1_1RawPacket_1a606a9688958cbc74f146d28c3042b769) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`write`](base.md#classicy_1_1RawPacket_1a6864137826313bac337b46de7d8ae2f4) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`data`](base.md#classicy_1_1RawPacket_1af40e88a78b327a4a3c609769ad1c9c20) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`size`](base.md#classicy_1_1RawPacket_1a9834f3f56c5ee160575a8fe6a4809dbf) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`className`](base.md#classicy_1_1RawPacket_1a41d94a78cbe6d6e7c074aa632deb6828) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`ownsBuffer`](base.md#classicy_1_1RawPacket_1a9e5982738a696bd9db11e74897194068) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`opaque`](base.md#classicy_1_1IPacket_1aa66babf1218198553ca9adede7618930) | `variable` | Inherited from [`IPacket`](base.md#ipacket) |
| [`info`](base.md#classicy_1_1IPacket_1ac2231c52e60bf60b402bc95cc9d6607d) | `variable` | Inherited from [`IPacket`](base.md#ipacket) |
| [`flags`](base.md#classicy_1_1IPacket_1a9b3947336184d08d69f70f406b84cdf0) | `variable` | Inherited from [`IPacket`](base.md#ipacket) |
| [`IPacket`](base.md#classicy_1_1IPacket_1af237b656e089ab2576cad8324769cbe0) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`IPacket`](base.md#classicy_1_1IPacket_1aae812f8e7e880df66d95eedf160c993b) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`operator=`](base.md#classicy_1_1IPacket_1adedceb54266652b65e183fd2c2882881) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`clone`](base.md#classicy_1_1IPacket_1a0a71c618520cdb93b89f5e29f8f145c7) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`~IPacket`](base.md#classicy_1_1IPacket_1a0419279bf5a558ec6d850fefcd44b437) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`read`](base.md#classicy_1_1IPacket_1ad42e0c52a5092acb3dd1488928902c5b) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`write`](base.md#classicy_1_1IPacket_1a87b978fc87c58eb05e1c01ad1ca60f4c) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`size`](base.md#classicy_1_1IPacket_1a86bfdad925cd6943165c3f8b4637db8c) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`hasData`](base.md#classicy_1_1IPacket_1afacbc458f0ce7820ef1345d85f7d6f80) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`data`](base.md#classicy_1_1IPacket_1a2e31cef156789c02205ec5c470d8c951) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`constData`](base.md#classicy_1_1IPacket_1a80c71c363bb1e20a5e24841d1f3e470c) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`className`](base.md#classicy_1_1IPacket_1abbbdc14886e839a7c09bd0eeac423ee1) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`print`](base.md#classicy_1_1IPacket_1acc8685ec343b3e9ed81687b8e8fa1acc) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`operator<<`](base.md#classicy_1_1IPacket_1a5157a8d7d50d1ff3c829e9c44c0a38ee) | `friend` | Inherited from [`IPacket`](base.md#ipacket) |

### Inherited from [`MediaPacket`](#mediapacket)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`time`](#time-2)  | Presentation timestamp in microseconds. |
| `function` | [`MediaPacket`](#mediapacket-1) `inline` | Construct with a non-owning or owning mutable buffer. |
| `function` | [`MediaPacket`](#mediapacket-2) `inline` | Construct with const data (copied, owning). |
| `function` | [`MediaPacket`](#mediapacket-3) `inline` | Copy constructor. |
| `function` | [`clone`](#clone) `virtual` `const` `inline` `override` |  |
| `function` | [`className`](#classname) `virtual` `const` `inline` `override` | Returns the class name of this packet type for logging and diagnostics. |

### Inherited from [`RawPacket`](base.md#rawpacket)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`_data`](base.md#classicy_1_1RawPacket_1a31d0726502c953df1deea617ac639554)  |  |
| `variable` | [`_size`](base.md#classicy_1_1RawPacket_1a3e4ce8b933205ec4d1f62af467245dcd)  |  |
| `variable` | [`_owned`](base.md#classicy_1_1RawPacket_1a0a7cfa5a326df6b328d582f88f085ccb)  |  |
| `function` | [`RawPacket`](base.md#classicy_1_1RawPacket_1afdfbea094d4a084cedc6889dc29a39d4) `inline` | Construct with borrowed (non-owning) buffer. |
| `function` | [`RawPacket`](base.md#classicy_1_1RawPacket_1a2cabea1456be6e46c7b21874f330b7ee) `inline` | Construct with const data (copied, owning). |
| `function` | [`RawPacket`](base.md#classicy_1_1RawPacket_1a782274116d8a9ac3ade6d1f47a072f4c) `inline` | Copy constructor (always copies data). |
| `function` | [`~RawPacket`](base.md#classicy_1_1RawPacket_1a7018944eaa3a16a43a5b8de6554ffe56) `virtual` | Defaulted destructor. |
| `function` | [`clone`](base.md#classicy_1_1RawPacket_1a1ac09a8e23494bbd52939fedfe5a6a8c) `virtual` `const` `inline` `override` |  |
| `function` | [`copyData`](base.md#classicy_1_1RawPacket_1a1bb606fd34a1f45e1a90d046607886de) `virtual` `inline` | Copies data into an internally owned buffer, replacing any prior content. |
| `function` | [`read`](base.md#classicy_1_1RawPacket_1a606a9688958cbc74f146d28c3042b769) `virtual` `inline` `override` | Reads from the buffer by copying its contents into an owned buffer. |
| `function` | [`write`](base.md#classicy_1_1RawPacket_1a6864137826313bac337b46de7d8ae2f4) `virtual` `const` `inline` `override` | Appends the packet data to the given output buffer. |
| `function` | [`data`](base.md#classicy_1_1RawPacket_1af40e88a78b327a4a3c609769ad1c9c20) `virtual` `const` `inline` `override` |  |
| `function` | [`size`](base.md#classicy_1_1RawPacket_1a9834f3f56c5ee160575a8fe6a4809dbf) `virtual` `const` `inline` `override` |  |
| `function` | [`className`](base.md#classicy_1_1RawPacket_1a41d94a78cbe6d6e7c074aa632deb6828) `virtual` `const` `inline` `override` | Returns the class name of this packet type for logging and diagnostics. |
| `function` | [`ownsBuffer`](base.md#classicy_1_1RawPacket_1a9e5982738a696bd9db11e74897194068) `const` `inline` |  |

### Inherited from [`IPacket`](base.md#ipacket)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`opaque`](base.md#classicy_1_1IPacket_1aa66babf1218198553ca9adede7618930)  | Optional type-safe context data. Use std::any_cast to retrieve. Lifetime of the stored value is tied to the packet's lifetime. |
| `variable` | [`info`](base.md#classicy_1_1IPacket_1ac2231c52e60bf60b402bc95cc9d6607d)  | Optional extra information about the packet. |
| `variable` | [`flags`](base.md#classicy_1_1IPacket_1a9b3947336184d08d69f70f406b84cdf0)  | Provides basic information about the packet. |
| `function` | [`IPacket`](base.md#classicy_1_1IPacket_1af237b656e089ab2576cad8324769cbe0) `inline` |  |
| `function` | [`IPacket`](base.md#classicy_1_1IPacket_1aae812f8e7e880df66d95eedf160c993b) `inline` | Copy constructor; clones the info object if present. |
| `function` | [`operator=`](base.md#classicy_1_1IPacket_1adedceb54266652b65e183fd2c2882881) `inline` | Copy assignment; clones the info object if present. |
| `function` | [`clone`](base.md#classicy_1_1IPacket_1a0a71c618520cdb93b89f5e29f8f145c7) `virtual` `const` | Returns a heap-allocated deep copy of this packet. |
| `function` | [`~IPacket`](base.md#classicy_1_1IPacket_1a0419279bf5a558ec6d850fefcd44b437) `virtual` | Defaulted destructor. |
| `function` | [`read`](base.md#classicy_1_1IPacket_1ad42e0c52a5092acb3dd1488928902c5b) `virtual` | Read/parse to the packet from the given input buffer. The number of bytes read is returned. |
| `function` | [`write`](base.md#classicy_1_1IPacket_1a87b978fc87c58eb05e1c01ad1ca60f4c) `virtual` `const` | Copy/generate to the packet given output buffer. The number of bytes written can be obtained from the buffer. |
| `function` | [`size`](base.md#classicy_1_1IPacket_1a86bfdad925cd6943165c3f8b4637db8c) `virtual` `const` `inline` | The size of the packet in bytes. |
| `function` | [`hasData`](base.md#classicy_1_1IPacket_1afacbc458f0ce7820ef1345d85f7d6f80) `virtual` `const` `inline` | Returns true if the packet has a non-null data pointer. |
| `function` | [`data`](base.md#classicy_1_1IPacket_1a2e31cef156789c02205ec5c470d8c951) `virtual` `const` `inline` | The packet data pointer for buffered packets. |
| `function` | [`constData`](base.md#classicy_1_1IPacket_1a80c71c363bb1e20a5e24841d1f3e470c) `virtual` `const` `inline` | The const packet data pointer for buffered packets. |
| `function` | [`className`](base.md#classicy_1_1IPacket_1abbbdc14886e839a7c09bd0eeac423ee1) `virtual` `const` | Returns the class name of this packet type for logging and diagnostics. |
| `function` | [`print`](base.md#classicy_1_1IPacket_1acc8685ec343b3e9ed81687b8e8fa1acc) `virtual` `const` `inline` | Prints a human-readable representation to the given stream. |
| `friend` | [`operator<<`](base.md#classicy_1_1IPacket_1a5157a8d7d50d1ff3c829e9c44c0a38ee) `inline` | [Stream](base.md#stream) insertion operator; delegates to [print()](base.md#classicy_1_1IPacket_1acc8685ec343b3e9ed81687b8e8fa1acc). |

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

##### Reimplements

- [`clone`](#clone)

##### Reimplemented by

- [`clone`](#clone-4)

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

##### Reimplements

- [`className`](#classname)

##### Reimplemented by

- [`className`](#classname-4)

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`buffer`](#buffer-1) | `variable` | Declared here |
| [`linesize`](#linesize-1) | `variable` | Declared here |
| [`channels`](#channels-2) | `variable` | Declared here |
| [`sampleFmt`](#samplefmt-1) | `variable` | Declared here |
| [`owns_buffer`](#owns_buffer-1) | `variable` | Declared here |
| [`PlanarAudioPacket`](#planaraudiopacket-1) | `function` | Declared here |
| [`PlanarAudioPacket`](#planaraudiopacket-2) | `function` | Declared here |
| [`clone`](#clone-4) | `function` | Declared here |
| [`className`](#classname-4) | `function` | Declared here |
| [`numSamples`](#numsamples) | `variable` | Inherited from [`AudioPacket`](#audiopacket) |
| [`avpacket`](#avpacket-1) | `variable` | Inherited from [`AudioPacket`](#audiopacket) |
| [`AudioPacket`](#audiopacket-1) | `function` | Inherited from [`AudioPacket`](#audiopacket) |
| [`clone`](#clone-3) | `function` | Inherited from [`AudioPacket`](#audiopacket) |
| [`samples`](#samples) | `function` | Inherited from [`AudioPacket`](#audiopacket) |
| [`className`](#classname-3) | `function` | Inherited from [`AudioPacket`](#audiopacket) |
| [`time`](#time-2) | `variable` | Inherited from [`MediaPacket`](#mediapacket) |
| [`MediaPacket`](#mediapacket-1) | `function` | Inherited from [`MediaPacket`](#mediapacket) |
| [`MediaPacket`](#mediapacket-2) | `function` | Inherited from [`MediaPacket`](#mediapacket) |
| [`MediaPacket`](#mediapacket-3) | `function` | Inherited from [`MediaPacket`](#mediapacket) |
| [`clone`](#clone) | `function` | Inherited from [`MediaPacket`](#mediapacket) |
| [`className`](#classname) | `function` | Inherited from [`MediaPacket`](#mediapacket) |
| [`_data`](base.md#classicy_1_1RawPacket_1a31d0726502c953df1deea617ac639554) | `variable` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`_size`](base.md#classicy_1_1RawPacket_1a3e4ce8b933205ec4d1f62af467245dcd) | `variable` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`_owned`](base.md#classicy_1_1RawPacket_1a0a7cfa5a326df6b328d582f88f085ccb) | `variable` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`RawPacket`](base.md#classicy_1_1RawPacket_1afdfbea094d4a084cedc6889dc29a39d4) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`RawPacket`](base.md#classicy_1_1RawPacket_1a2cabea1456be6e46c7b21874f330b7ee) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`RawPacket`](base.md#classicy_1_1RawPacket_1a782274116d8a9ac3ade6d1f47a072f4c) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`~RawPacket`](base.md#classicy_1_1RawPacket_1a7018944eaa3a16a43a5b8de6554ffe56) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`clone`](base.md#classicy_1_1RawPacket_1a1ac09a8e23494bbd52939fedfe5a6a8c) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`copyData`](base.md#classicy_1_1RawPacket_1a1bb606fd34a1f45e1a90d046607886de) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`read`](base.md#classicy_1_1RawPacket_1a606a9688958cbc74f146d28c3042b769) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`write`](base.md#classicy_1_1RawPacket_1a6864137826313bac337b46de7d8ae2f4) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`data`](base.md#classicy_1_1RawPacket_1af40e88a78b327a4a3c609769ad1c9c20) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`size`](base.md#classicy_1_1RawPacket_1a9834f3f56c5ee160575a8fe6a4809dbf) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`className`](base.md#classicy_1_1RawPacket_1a41d94a78cbe6d6e7c074aa632deb6828) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`ownsBuffer`](base.md#classicy_1_1RawPacket_1a9e5982738a696bd9db11e74897194068) | `function` | Inherited from [`RawPacket`](base.md#rawpacket) |
| [`opaque`](base.md#classicy_1_1IPacket_1aa66babf1218198553ca9adede7618930) | `variable` | Inherited from [`IPacket`](base.md#ipacket) |
| [`info`](base.md#classicy_1_1IPacket_1ac2231c52e60bf60b402bc95cc9d6607d) | `variable` | Inherited from [`IPacket`](base.md#ipacket) |
| [`flags`](base.md#classicy_1_1IPacket_1a9b3947336184d08d69f70f406b84cdf0) | `variable` | Inherited from [`IPacket`](base.md#ipacket) |
| [`IPacket`](base.md#classicy_1_1IPacket_1af237b656e089ab2576cad8324769cbe0) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`IPacket`](base.md#classicy_1_1IPacket_1aae812f8e7e880df66d95eedf160c993b) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`operator=`](base.md#classicy_1_1IPacket_1adedceb54266652b65e183fd2c2882881) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`clone`](base.md#classicy_1_1IPacket_1a0a71c618520cdb93b89f5e29f8f145c7) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`~IPacket`](base.md#classicy_1_1IPacket_1a0419279bf5a558ec6d850fefcd44b437) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`read`](base.md#classicy_1_1IPacket_1ad42e0c52a5092acb3dd1488928902c5b) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`write`](base.md#classicy_1_1IPacket_1a87b978fc87c58eb05e1c01ad1ca60f4c) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`size`](base.md#classicy_1_1IPacket_1a86bfdad925cd6943165c3f8b4637db8c) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`hasData`](base.md#classicy_1_1IPacket_1afacbc458f0ce7820ef1345d85f7d6f80) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`data`](base.md#classicy_1_1IPacket_1a2e31cef156789c02205ec5c470d8c951) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`constData`](base.md#classicy_1_1IPacket_1a80c71c363bb1e20a5e24841d1f3e470c) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`className`](base.md#classicy_1_1IPacket_1abbbdc14886e839a7c09bd0eeac423ee1) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`print`](base.md#classicy_1_1IPacket_1acc8685ec343b3e9ed81687b8e8fa1acc) | `function` | Inherited from [`IPacket`](base.md#ipacket) |
| [`operator<<`](base.md#classicy_1_1IPacket_1a5157a8d7d50d1ff3c829e9c44c0a38ee) | `friend` | Inherited from [`IPacket`](base.md#ipacket) |

### Inherited from [`AudioPacket`](#audiopacket)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`numSamples`](#numsamples)  | Number of audio samples per channel. |
| `variable` | [`avpacket`](#avpacket-1)  | Non-owning pointer to the encoded AVPacket from FFmpeg. Set by [AudioEncoder](#audioencoder), read by [MultiplexEncoder](#multiplexencoder). |
| `function` | [`AudioPacket`](#audiopacket-1) `inline` | Construct an audio packet with an interleaved sample buffer. |
| `function` | [`clone`](#clone-3) `virtual` `const` `inline` `override` |  |
| `function` | [`samples`](#samples) `virtual` `const` `inline` |  |
| `function` | [`className`](#classname-3) `virtual` `const` `inline` `override` | Returns the class name of this packet type for logging and diagnostics. |

### Inherited from [`MediaPacket`](#mediapacket)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`time`](#time-2)  | Presentation timestamp in microseconds. |
| `function` | [`MediaPacket`](#mediapacket-1) `inline` | Construct with a non-owning or owning mutable buffer. |
| `function` | [`MediaPacket`](#mediapacket-2) `inline` | Construct with const data (copied, owning). |
| `function` | [`MediaPacket`](#mediapacket-3) `inline` | Copy constructor. |
| `function` | [`clone`](#clone) `virtual` `const` `inline` `override` |  |
| `function` | [`className`](#classname) `virtual` `const` `inline` `override` | Returns the class name of this packet type for logging and diagnostics. |

### Inherited from [`RawPacket`](base.md#rawpacket)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`_data`](base.md#classicy_1_1RawPacket_1a31d0726502c953df1deea617ac639554)  |  |
| `variable` | [`_size`](base.md#classicy_1_1RawPacket_1a3e4ce8b933205ec4d1f62af467245dcd)  |  |
| `variable` | [`_owned`](base.md#classicy_1_1RawPacket_1a0a7cfa5a326df6b328d582f88f085ccb)  |  |
| `function` | [`RawPacket`](base.md#classicy_1_1RawPacket_1afdfbea094d4a084cedc6889dc29a39d4) `inline` | Construct with borrowed (non-owning) buffer. |
| `function` | [`RawPacket`](base.md#classicy_1_1RawPacket_1a2cabea1456be6e46c7b21874f330b7ee) `inline` | Construct with const data (copied, owning). |
| `function` | [`RawPacket`](base.md#classicy_1_1RawPacket_1a782274116d8a9ac3ade6d1f47a072f4c) `inline` | Copy constructor (always copies data). |
| `function` | [`~RawPacket`](base.md#classicy_1_1RawPacket_1a7018944eaa3a16a43a5b8de6554ffe56) `virtual` | Defaulted destructor. |
| `function` | [`clone`](base.md#classicy_1_1RawPacket_1a1ac09a8e23494bbd52939fedfe5a6a8c) `virtual` `const` `inline` `override` |  |
| `function` | [`copyData`](base.md#classicy_1_1RawPacket_1a1bb606fd34a1f45e1a90d046607886de) `virtual` `inline` | Copies data into an internally owned buffer, replacing any prior content. |
| `function` | [`read`](base.md#classicy_1_1RawPacket_1a606a9688958cbc74f146d28c3042b769) `virtual` `inline` `override` | Reads from the buffer by copying its contents into an owned buffer. |
| `function` | [`write`](base.md#classicy_1_1RawPacket_1a6864137826313bac337b46de7d8ae2f4) `virtual` `const` `inline` `override` | Appends the packet data to the given output buffer. |
| `function` | [`data`](base.md#classicy_1_1RawPacket_1af40e88a78b327a4a3c609769ad1c9c20) `virtual` `const` `inline` `override` |  |
| `function` | [`size`](base.md#classicy_1_1RawPacket_1a9834f3f56c5ee160575a8fe6a4809dbf) `virtual` `const` `inline` `override` |  |
| `function` | [`className`](base.md#classicy_1_1RawPacket_1a41d94a78cbe6d6e7c074aa632deb6828) `virtual` `const` `inline` `override` | Returns the class name of this packet type for logging and diagnostics. |
| `function` | [`ownsBuffer`](base.md#classicy_1_1RawPacket_1a9e5982738a696bd9db11e74897194068) `const` `inline` |  |

### Inherited from [`IPacket`](base.md#ipacket)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`opaque`](base.md#classicy_1_1IPacket_1aa66babf1218198553ca9adede7618930)  | Optional type-safe context data. Use std::any_cast to retrieve. Lifetime of the stored value is tied to the packet's lifetime. |
| `variable` | [`info`](base.md#classicy_1_1IPacket_1ac2231c52e60bf60b402bc95cc9d6607d)  | Optional extra information about the packet. |
| `variable` | [`flags`](base.md#classicy_1_1IPacket_1a9b3947336184d08d69f70f406b84cdf0)  | Provides basic information about the packet. |
| `function` | [`IPacket`](base.md#classicy_1_1IPacket_1af237b656e089ab2576cad8324769cbe0) `inline` |  |
| `function` | [`IPacket`](base.md#classicy_1_1IPacket_1aae812f8e7e880df66d95eedf160c993b) `inline` | Copy constructor; clones the info object if present. |
| `function` | [`operator=`](base.md#classicy_1_1IPacket_1adedceb54266652b65e183fd2c2882881) `inline` | Copy assignment; clones the info object if present. |
| `function` | [`clone`](base.md#classicy_1_1IPacket_1a0a71c618520cdb93b89f5e29f8f145c7) `virtual` `const` | Returns a heap-allocated deep copy of this packet. |
| `function` | [`~IPacket`](base.md#classicy_1_1IPacket_1a0419279bf5a558ec6d850fefcd44b437) `virtual` | Defaulted destructor. |
| `function` | [`read`](base.md#classicy_1_1IPacket_1ad42e0c52a5092acb3dd1488928902c5b) `virtual` | Read/parse to the packet from the given input buffer. The number of bytes read is returned. |
| `function` | [`write`](base.md#classicy_1_1IPacket_1a87b978fc87c58eb05e1c01ad1ca60f4c) `virtual` `const` | Copy/generate to the packet given output buffer. The number of bytes written can be obtained from the buffer. |
| `function` | [`size`](base.md#classicy_1_1IPacket_1a86bfdad925cd6943165c3f8b4637db8c) `virtual` `const` `inline` | The size of the packet in bytes. |
| `function` | [`hasData`](base.md#classicy_1_1IPacket_1afacbc458f0ce7820ef1345d85f7d6f80) `virtual` `const` `inline` | Returns true if the packet has a non-null data pointer. |
| `function` | [`data`](base.md#classicy_1_1IPacket_1a2e31cef156789c02205ec5c470d8c951) `virtual` `const` `inline` | The packet data pointer for buffered packets. |
| `function` | [`constData`](base.md#classicy_1_1IPacket_1a80c71c363bb1e20a5e24841d1f3e470c) `virtual` `const` `inline` | The const packet data pointer for buffered packets. |
| `function` | [`className`](base.md#classicy_1_1IPacket_1abbbdc14886e839a7c09bd0eeac423ee1) `virtual` `const` | Returns the class name of this packet type for logging and diagnostics. |
| `function` | [`print`](base.md#classicy_1_1IPacket_1acc8685ec343b3e9ed81687b8e8fa1acc) `virtual` `const` `inline` | Prints a human-readable representation to the given stream. |
| `friend` | [`operator<<`](base.md#classicy_1_1IPacket_1a5157a8d7d50d1ff3c829e9c44c0a38ee) `inline` | [Stream](base.md#stream) insertion operator; delegates to [print()](base.md#classicy_1_1IPacket_1acc8685ec343b3e9ed81687b8e8fa1acc). |

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

##### Reimplements

- [`clone`](#clone-3)

---

{#classname-4}

#### className

`virtual` `const` `inline` `override`

```cpp
virtual inline const char * className() const override
```

Defined in src/av/include/icy/av/packet.h:221

Returns the class name of this packet type for logging and diagnostics.

##### Reimplements

- [`className`](#classname-3)

