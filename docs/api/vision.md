{#visionmodule}

# vision

Video intelligence primitives for sampled frame processing and event output.

### Namespaces

| Name | Description |
|------|-------------|
| [`vision`](#vision) |  |

{#vision}

# vision

### Classes

| Name | Description |
|------|-------------|
| [`DetectionQueue`](#detectionqueue) | Async queue for normalized detector-ready frames. |
| [`Detector`](#detector-1) | Base interface for detectors that consume normalized vision frames. |
| [`FrameNormalizer`](#framenormalizer) | Converts sampled decoded video into owned detector-ready frames. |
| [`FrameSampler`](#framesampler) | [PacketProcessor](base.md#packetprocessor) that forwards a sampled subset of decoded video frames. |
| [`MotionDetector`](#motiondetector) |  |
| [`VisionEventEmitter`](#visioneventemitter) |  |
| [`Detection`](#detection) |  |
| [`FrameNormalizerConfig`](#framenormalizerconfig) |  |
| [`FrameNormalizerStats`](#framenormalizerstats) |  |
| [`FrameSamplerConfig`](#framesamplerconfig) |  |
| [`FrameSamplerStats`](#framesamplerstats) |  |
| [`MotionDetectorConfig`](#motiondetectorconfig) |  |
| [`MotionDetectorStats`](#motiondetectorstats) |  |
| [`Region`](#region-1) |  |
| [`Track`](#track) |  |
| [`VisionEvent`](#visionevent) |  |
| [`VisionFrameContext`](#visionframecontext) |  |
| [`VisionFramePacket`](#visionframepacket) | Owned detector-ready frame buffer carrying stable frame metadata. |

### Typedefs

| Return | Name | Description |
|--------|------|-------------|
| `Signal< void(const VisionEvent &)>` | [`VisionEventSignal`](#visioneventsignal)  |  |
| `VisionFrameContext` | [`FrameRef`](#frameref)  |  |

---

{#visioneventsignal}

#### VisionEventSignal

```cpp
using VisionEventSignal = Signal< void(const VisionEvent &)>
```

---

{#frameref}

#### FrameRef

```cpp
using FrameRef = VisionFrameContext
```

### Functions

| Return | Name | Description |
|--------|------|-------------|
| `Vision_APIVisionFrameContext` | [`makeVisionFrameContext`](#makevisionframecontext) `nodiscard` |  |
| `FrameRef` | [`makeFrameRef`](#makeframeref) `inline` `nodiscard` |  |
| `Vision_APIjson::Value` | [`toJson`](#tojson-3) `nodiscard` |  |
| `Vision_APIjson::Value` | [`toJson`](#tojson-4) `nodiscard` |  |
| `Vision_APIjson::Value` | [`toJson`](#tojson-5) `nodiscard` |  |
| `Vision_APIjson::Value` | [`toJson`](#tojson-6) `nodiscard` |  |
| `Vision_APIjson::Value` | [`toJson`](#tojson-7) `nodiscard` |  |

---

{#makevisionframecontext}

#### makeVisionFrameContext

`nodiscard`

```cpp
[[nodiscard]] Vision_APIVisionFrameContext makeVisionFrameContext(const av::PlanarVideoPacket & packet, uint64_t frameId = 0, const std::string & sourceId = {}, const std::string & streamId = {}, int64_t receivedAtUsec = 0)
```

---

{#makeframeref}

#### makeFrameRef

`inline` `nodiscard`

```cpp
[[nodiscard]] inline FrameRef makeFrameRef(const av::PlanarVideoPacket & packet, uint64_t sequence = 0, const std::string & sourceId = {}, const std::string & streamId = {}, int64_t receivedAtUsec = 0)
```

---

{#tojson-3}

#### toJson

`nodiscard`

```cpp
[[nodiscard]] Vision_APIjson::Value toJson(const Region & region)
```

---

{#tojson-4}

#### toJson

`nodiscard`

```cpp
[[nodiscard]] Vision_APIjson::Value toJson(const VisionFrameContext & frame)
```

---

{#tojson-5}

#### toJson

`nodiscard`

```cpp
[[nodiscard]] Vision_APIjson::Value toJson(const Detection & detection)
```

---

{#tojson-6}

#### toJson

`nodiscard`

```cpp
[[nodiscard]] Vision_APIjson::Value toJson(const Track & track)
```

---

{#tojson-7}

#### toJson

`nodiscard`

```cpp
[[nodiscard]] Vision_APIjson::Value toJson(const VisionEvent & event)
```

{#detectionqueue}

## DetectionQueue

```cpp
#include <icy/vision/detectionqueue.h>
```

```cpp
class DetectionQueue
```

Defined in src/vision/include/icy/vision/detectionqueue.h:30

> **Inherits:** [`AsyncPacketQueue< VisionFramePacket >`](base.md#asyncpacketqueue)

Async queue for normalized detector-ready frames.

This is the intentional async boundary for detection work. Upstream stages can stay borrowed until `[FrameNormalizer](#framenormalizer)` materializes an owned `[VisionFramePacket](#visionframepacket)`, after which this queue bounds in-flight detection work by dropping the oldest queued frames first.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`DetectionQueue`](#detectionqueue-1) `inline` `explicit` |  |

---

{#detectionqueue-1}

#### DetectionQueue

`inline` `explicit`

```cpp
inline explicit DetectionQueue(int maxFrames = 32)
```

Defined in src/vision/include/icy/vision/detectionqueue.h:33

{#detector-1}

## Detector

```cpp
#include <icy/vision/detector.h>
```

```cpp
class Detector
```

Defined in src/vision/include/icy/vision/detector.h:25

> **Subclassed by:** [`MotionDetector`](#motiondetector)

Base interface for detectors that consume normalized vision frames.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `VisionEventSignal` | [`Event`](#event-6)  |  |

---

{#event-6}

#### Event

```cpp
VisionEventSignal Event
```

Defined in src/vision/include/icy/vision/detector.h:30

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`reset`](#reset-16) `virtual` |  |
| `void` | [`process`](#process-8) `virtual` |  |

---

{#reset-16}

#### reset

`virtual`

```cpp
virtual void reset()
```

Defined in src/vision/include/icy/vision/detector.h:32

---

{#process-8}

#### process

`virtual`

```cpp
virtual void process(const VisionFramePacket & packet)
```

Defined in src/vision/include/icy/vision/detector.h:33

{#framenormalizer}

## FrameNormalizer

```cpp
#include <icy/vision/framenormalizer.h>
```

```cpp
class FrameNormalizer
```

Defined in src/vision/include/icy/vision/framenormalizer.h:49

> **Inherits:** [`PacketProcessor`](base.md#packetprocessor)

Converts sampled decoded video into owned detector-ready frames.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `PacketSignal` | [`emitter`](#emitter-8)  |  |

---

{#emitter-8}

#### emitter

```cpp
PacketSignal emitter
```

Defined in src/vision/include/icy/vision/framenormalizer.h:52

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`FrameNormalizer`](#framenormalizer-1) `explicit` |  |
| `void` | [`setConfig`](#setconfig-1)  |  |
| `FrameNormalizerConfig` | [`config`](#config-1) `const` `nodiscard` |  |
| `FrameNormalizerStats` | [`stats`](#stats-1) `const` `nodiscard` |  |
| `void` | [`reset`](#reset-17)  |  |
| `void` | [`process`](#process-9) `virtual` `override` | This method performs processing on the given packet and emits the result. |
| `bool` | [`accepts`](#accepts-3) `virtual` `inline` `override` | This method ensures compatibility with the given packet type. Return false to reject the packet. |

---

{#framenormalizer-1}

#### FrameNormalizer

`explicit`

```cpp
explicit FrameNormalizer(FrameNormalizerConfig config = {})
```

Defined in src/vision/include/icy/vision/framenormalizer.h:54

---

{#setconfig-1}

#### setConfig

```cpp
void setConfig(FrameNormalizerConfig config)
```

Defined in src/vision/include/icy/vision/framenormalizer.h:57

---

{#config-1}

#### config

`const` `nodiscard`

```cpp
[[nodiscard]] FrameNormalizerConfig config() const
```

Defined in src/vision/include/icy/vision/framenormalizer.h:58

---

{#stats-1}

#### stats

`const` `nodiscard`

```cpp
[[nodiscard]] FrameNormalizerStats stats() const
```

Defined in src/vision/include/icy/vision/framenormalizer.h:59

---

{#reset-17}

#### reset

```cpp
void reset()
```

Defined in src/vision/include/icy/vision/framenormalizer.h:60

---

{#process-9}

#### process

`virtual` `override`

```cpp
virtual void process(IPacket & packet) override
```

Defined in src/vision/include/icy/vision/framenormalizer.h:62

This method performs processing on the given packet and emits the result.

Processors that defer work asynchronously must either clone the packet or retain an owned equivalent. See [retention()](base.md#classicy_1_1PacketStreamAdapter_1a37a1aa11e35a20a7c527efa78e99e928).

---

{#accepts-3}

#### accepts

`virtual` `inline` `override`

```cpp
virtual inline bool accepts(IPacket *) override
```

Defined in src/vision/include/icy/vision/framenormalizer.h:63

This method ensures compatibility with the given packet type. Return false to reject the packet.

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `FrameNormalizerConfig` | [`_config`](#_config-1)  |  |
| `std::atomic< uint64_t >` | [`_seen`](#_seen-1)  |  |
| `std::atomic< uint64_t >` | [`_emitted`](#_emitted-1)  |  |
| `std::atomic< uint64_t >` | [`_dropped`](#_dropped)  |  |
| `std::atomic< uint64_t >` | [`_converted`](#_converted)  |  |
| `std::atomic< uint64_t >` | [`_frameId`](#_frameid)  |  |
| `std::unique_ptr< State >` | [`_state`](#_state-3)  |  |

---

{#_config-1}

#### _config

```cpp
FrameNormalizerConfig _config
```

Defined in src/vision/include/icy/vision/framenormalizer.h:76

---

{#_seen-1}

#### _seen

```cpp
std::atomic< uint64_t > _seen {0}
```

Defined in src/vision/include/icy/vision/framenormalizer.h:77

---

{#_emitted-1}

#### _emitted

```cpp
std::atomic< uint64_t > _emitted {0}
```

Defined in src/vision/include/icy/vision/framenormalizer.h:78

---

{#_dropped}

#### _dropped

```cpp
std::atomic< uint64_t > _dropped {0}
```

Defined in src/vision/include/icy/vision/framenormalizer.h:79

---

{#_converted}

#### _converted

```cpp
std::atomic< uint64_t > _converted {0}
```

Defined in src/vision/include/icy/vision/framenormalizer.h:80

---

{#_frameid}

#### _frameId

```cpp
std::atomic< uint64_t > _frameId {0}
```

Defined in src/vision/include/icy/vision/framenormalizer.h:81

---

{#_state-3}

#### _state

```cpp
std::unique_ptr< State > _state
```

Defined in src/vision/include/icy/vision/framenormalizer.h:82

### Private Methods

| Return | Name | Description |
|--------|------|-------------|
| `VisionFramePacket` | [`normalize`](#normalize-1) `nodiscard` |  |

---

{#normalize-1}

#### normalize

`nodiscard`

```cpp
[[nodiscard]] VisionFramePacket normalize(const av::PlanarVideoPacket & packet)
```

Defined in src/vision/include/icy/vision/framenormalizer.h:74

### Private Static Methods

| Return | Name | Description |
|--------|------|-------------|
| `FrameNormalizerConfig` | [`sanitize`](#sanitize-1) `static` |  |

---

{#sanitize-1}

#### sanitize

`static`

```cpp
static FrameNormalizerConfig sanitize(FrameNormalizerConfig config)
```

Defined in src/vision/include/icy/vision/framenormalizer.h:72

{#framesampler}

## FrameSampler

```cpp
#include <icy/vision/framesampler.h>
```

```cpp
class FrameSampler
```

Defined in src/vision/include/icy/vision/framesampler.h:49

> **Inherits:** [`PacketProcessor`](base.md#packetprocessor)

[PacketProcessor](base.md#packetprocessor) that forwards a sampled subset of decoded video frames.

The sampler is intentionally zero-copy on its synchronous path: sampled frames are forwarded by reference and dropped frames are discarded before any queue hop. Pair it with [DetectionQueue](#detectionqueue) when a worker-thread boundary is required.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `PacketSignal` | [`emitter`](#emitter-9)  |  |

---

{#emitter-9}

#### emitter

```cpp
PacketSignal emitter
```

Defined in src/vision/include/icy/vision/framesampler.h:52

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`FrameSampler`](#framesampler-1) `inline` `explicit` |  |
| `void` | [`setConfig`](#setconfig-2) `inline` |  |
| `FrameSamplerConfig` | [`config`](#config-2) `const` `inline` `nodiscard` |  |
| `FrameSamplerStats` | [`stats`](#stats-2) `const` `inline` `nodiscard` |  |
| `void` | [`reset`](#reset-18) `inline` |  |
| `void` | [`process`](#process-10) `virtual` `inline` `override` | This method performs processing on the given packet and emits the result. |
| `bool` | [`accepts`](#accepts-4) `virtual` `inline` `override` | This method ensures compatibility with the given packet type. Return false to reject the packet. |

---

{#framesampler-1}

#### FrameSampler

`inline` `explicit`

```cpp
inline explicit FrameSampler(FrameSamplerConfig config = {})
```

Defined in src/vision/include/icy/vision/framesampler.h:54

---

{#setconfig-2}

#### setConfig

`inline`

```cpp
inline void setConfig(FrameSamplerConfig config)
```

Defined in src/vision/include/icy/vision/framesampler.h:60

---

{#config-2}

#### config

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline FrameSamplerConfig config() const
```

Defined in src/vision/include/icy/vision/framesampler.h:66

---

{#stats-2}

#### stats

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline FrameSamplerStats stats() const
```

Defined in src/vision/include/icy/vision/framesampler.h:71

---

{#reset-18}

#### reset

`inline`

```cpp
inline void reset()
```

Defined in src/vision/include/icy/vision/framesampler.h:80

---

{#process-10}

#### process

`virtual` `inline` `override`

```cpp
virtual inline void process(IPacket & packet) override
```

Defined in src/vision/include/icy/vision/framesampler.h:88

This method performs processing on the given packet and emits the result.

Processors that defer work asynchronously must either clone the packet or retain an owned equivalent. See [retention()](base.md#classicy_1_1PacketStreamAdapter_1a37a1aa11e35a20a7c527efa78e99e928).

---

{#accepts-4}

#### accepts

`virtual` `inline` `override`

```cpp
virtual inline bool accepts(IPacket *) override
```

Defined in src/vision/include/icy/vision/framesampler.h:107

This method ensures compatibility with the given packet type. Return false to reject the packet.

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `FrameSamplerConfig` | [`_config`](#_config-2)  |  |
| `std::atomic< uint64_t >` | [`_seen`](#_seen-2)  |  |
| `std::atomic< uint64_t >` | [`_forwarded`](#_forwarded)  |  |
| `std::atomic< uint64_t >` | [`_dropped`](#_dropped-1)  |  |
| `std::atomic< int64_t >` | [`_lastForwardedTimeUsec`](#_lastforwardedtimeusec)  |  |

---

{#_config-2}

#### _config

```cpp
FrameSamplerConfig _config
```

Defined in src/vision/include/icy/vision/framesampler.h:141

---

{#_seen-2}

#### _seen

```cpp
std::atomic< uint64_t > _seen {0}
```

Defined in src/vision/include/icy/vision/framesampler.h:142

---

{#_forwarded}

#### _forwarded

```cpp
std::atomic< uint64_t > _forwarded {0}
```

Defined in src/vision/include/icy/vision/framesampler.h:143

---

{#_dropped-1}

#### _dropped

```cpp
std::atomic< uint64_t > _dropped {0}
```

Defined in src/vision/include/icy/vision/framesampler.h:144

---

{#_lastforwardedtimeusec}

#### _lastForwardedTimeUsec

```cpp
std::atomic< int64_t > _lastForwardedTimeUsec {kUnsetTimeUsec}
```

Defined in src/vision/include/icy/vision/framesampler.h:145

### Private Methods

| Return | Name | Description |
|--------|------|-------------|
| `bool` | [`shouldForward`](#shouldforward) `const` `inline` |  |

---

{#shouldforward}

#### shouldForward

`const` `inline`

```cpp
inline bool shouldForward(const av::PlanarVideoPacket & frame, uint64_t seen) const
```

Defined in src/vision/include/icy/vision/framesampler.h:124

### Private Static Attributes

| Return | Name | Description |
|--------|------|-------------|
| `int64_t` | [`kUnsetTimeUsec`](#kunsettimeusec) `static` `constexpr` |  |

---

{#kunsettimeusec}

#### kUnsetTimeUsec

`static` `constexpr`

```cpp
int64_t kUnsetTimeUsec = std::numeric_limits<int64_t>::min()
```

Defined in src/vision/include/icy/vision/framesampler.h:115

### Private Static Methods

| Return | Name | Description |
|--------|------|-------------|
| `FrameSamplerConfig` | [`sanitize`](#sanitize-2) `static` `inline` |  |

---

{#sanitize-2}

#### sanitize

`static` `inline`

```cpp
static inline FrameSamplerConfig sanitize(FrameSamplerConfig config)
```

Defined in src/vision/include/icy/vision/framesampler.h:117

{#motiondetector}

## MotionDetector

```cpp
#include <icy/vision/motiondetector.h>
```

```cpp
class MotionDetector
```

Defined in src/vision/include/icy/vision/motiondetector.h:49

> **Inherits:** [`Detector`](#detector-1)

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`MotionDetector`](#motiondetector-1) `explicit` |  |
| `void` | [`setConfig`](#setconfig-3)  |  |
| `MotionDetectorConfig` | [`config`](#config-3) `const` `nodiscard` |  |
| `MotionDetectorStats` | [`stats`](#stats-3) `const` `nodiscard` |  |
| `void` | [`reset`](#reset-19) `virtual` `override` |  |
| `void` | [`process`](#process-11) `virtual` `override` |  |

---

{#motiondetector-1}

#### MotionDetector

`explicit`

```cpp
explicit MotionDetector(MotionDetectorConfig config = {})
```

Defined in src/vision/include/icy/vision/motiondetector.h:52

---

{#setconfig-3}

#### setConfig

```cpp
void setConfig(MotionDetectorConfig config)
```

Defined in src/vision/include/icy/vision/motiondetector.h:54

---

{#config-3}

#### config

`const` `nodiscard`

```cpp
[[nodiscard]] MotionDetectorConfig config() const
```

Defined in src/vision/include/icy/vision/motiondetector.h:55

---

{#stats-3}

#### stats

`const` `nodiscard`

```cpp
[[nodiscard]] MotionDetectorStats stats() const
```

Defined in src/vision/include/icy/vision/motiondetector.h:56

---

{#reset-19}

#### reset

`virtual` `override`

```cpp
virtual void reset() override
```

Defined in src/vision/include/icy/vision/motiondetector.h:57

---

{#process-11}

#### process

`virtual` `override`

```cpp
virtual void process(const VisionFramePacket & packet) override
```

Defined in src/vision/include/icy/vision/motiondetector.h:59

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `MotionDetectorConfig` | [`_config`](#_config-3)  |  |
| `uint64_t` | [`_seen`](#_seen-3)  |  |
| `uint64_t` | [`_emitted`](#_emitted-2)  |  |
| `uint32_t` | [`_warmedFrames`](#_warmedframes)  |  |
| `int` | [`_width`](#_width)  |  |
| `int` | [`_height`](#_height)  |  |
| `int64_t` | [`_lastEventTimeUsec`](#_lasteventtimeusec)  |  |
| `float` | [`_lastScore`](#_lastscore)  |  |
| `std::vector< uint8_t >` | [`_previousGrid`](#_previousgrid)  |  |
| `std::vector< uint8_t >` | [`_currentGrid`](#_currentgrid)  |  |

---

{#_config-3}

#### _config

```cpp
MotionDetectorConfig _config
```

Defined in src/vision/include/icy/vision/motiondetector.h:67

---

{#_seen-3}

#### _seen

```cpp
uint64_t _seen = 0
```

Defined in src/vision/include/icy/vision/motiondetector.h:68

---

{#_emitted-2}

#### _emitted

```cpp
uint64_t _emitted = 0
```

Defined in src/vision/include/icy/vision/motiondetector.h:69

---

{#_warmedframes}

#### _warmedFrames

```cpp
uint32_t _warmedFrames = 0
```

Defined in src/vision/include/icy/vision/motiondetector.h:70

---

{#_width}

#### _width

```cpp
int _width = 0
```

Defined in src/vision/include/icy/vision/motiondetector.h:71

---

{#_height}

#### _height

```cpp
int _height = 0
```

Defined in src/vision/include/icy/vision/motiondetector.h:72

---

{#_lasteventtimeusec}

#### _lastEventTimeUsec

```cpp
int64_t _lastEventTimeUsec = 0
```

Defined in src/vision/include/icy/vision/motiondetector.h:73

---

{#_lastscore}

#### _lastScore

```cpp
float _lastScore = 0.0f
```

Defined in src/vision/include/icy/vision/motiondetector.h:74

---

{#_previousgrid}

#### _previousGrid

```cpp
std::vector< uint8_t > _previousGrid
```

Defined in src/vision/include/icy/vision/motiondetector.h:75

---

{#_currentgrid}

#### _currentGrid

```cpp
std::vector< uint8_t > _currentGrid
```

Defined in src/vision/include/icy/vision/motiondetector.h:76

### Private Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`sampleLumaGrid`](#samplelumagrid) `const` |  |
| `float` | [`diffScore`](#diffscore) `const` |  |
| `void` | [`emitEvent`](#emitevent-1)  |  |

---

{#samplelumagrid}

#### sampleLumaGrid

`const`

```cpp
void sampleLumaGrid(const VisionFramePacket & packet, std::vector< uint8_t > & out) const
```

Defined in src/vision/include/icy/vision/motiondetector.h:63

---

{#diffscore}

#### diffScore

`const`

```cpp
float diffScore(const std::vector< uint8_t > & current) const
```

Defined in src/vision/include/icy/vision/motiondetector.h:64

---

{#emitevent-1}

#### emitEvent

```cpp
void emitEvent(const VisionFramePacket & packet, float score)
```

Defined in src/vision/include/icy/vision/motiondetector.h:65

### Private Static Methods

| Return | Name | Description |
|--------|------|-------------|
| `MotionDetectorConfig` | [`sanitize`](#sanitize-3) `static` |  |

---

{#sanitize-3}

#### sanitize

`static`

```cpp
static MotionDetectorConfig sanitize(MotionDetectorConfig config)
```

Defined in src/vision/include/icy/vision/motiondetector.h:62

{#visioneventemitter}

## VisionEventEmitter

```cpp
#include <icy/vision/eventemitter.h>
```

```cpp
class VisionEventEmitter
```

Defined in src/vision/include/icy/vision/eventemitter.h:29

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `VisionEventSignal` | [`Event`](#event-7)  |  |

---

{#event-7}

#### Event

```cpp
VisionEventSignal Event
```

Defined in src/vision/include/icy/vision/eventemitter.h:32

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`emit`](#emit-1) `inline` |  |

---

{#emit-1}

#### emit

`inline`

```cpp
inline void emit(const VisionEvent & event)
```

Defined in src/vision/include/icy/vision/eventemitter.h:34

{#detection}

## Detection

```cpp
#include <icy/vision/types.h>
```

```cpp
struct Detection
```

Defined in src/vision/include/icy/vision/types.h:52

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`label`](#label-1)  |  |
| `float` | [`confidence`](#confidence)  |  |
| `int64_t` | [`trackId`](#trackid)  |  |
| `Region` | [`region`](#region)  |  |
| `json::Value` | [`data`](#data-7)  |  |

---

{#label-1}

#### label

```cpp
std::string label
```

Defined in src/vision/include/icy/vision/types.h:54

---

{#confidence}

#### confidence

```cpp
float confidence = 0.0f
```

Defined in src/vision/include/icy/vision/types.h:55

---

{#trackid}

#### trackId

```cpp
int64_t trackId = -1
```

Defined in src/vision/include/icy/vision/types.h:56

---

{#region}

#### region

```cpp
Region region
```

Defined in src/vision/include/icy/vision/types.h:57

---

{#data-7}

#### data

```cpp
json::Value data = json::Value::object()
```

Defined in src/vision/include/icy/vision/types.h:58

{#framenormalizerconfig}

## FrameNormalizerConfig

```cpp
#include <icy/vision/framenormalizer.h>
```

```cpp
struct FrameNormalizerConfig
```

Defined in src/vision/include/icy/vision/framenormalizer.h:29

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`sourceId`](#sourceid)  |  |
| `std::string` | [`streamId`](#streamid)  |  |
| `int` | [`width`](#width-3)  |  |
| `int` | [`height`](#height-3)  |  |
| `std::string` | [`pixelFmt`](#pixelfmt-2)  |  |

---

{#sourceid}

#### sourceId

```cpp
std::string sourceId
```

Defined in src/vision/include/icy/vision/framenormalizer.h:31

---

{#streamid}

#### streamId

```cpp
std::string streamId
```

Defined in src/vision/include/icy/vision/framenormalizer.h:32

---

{#width-3}

#### width

```cpp
int width = 0
```

Defined in src/vision/include/icy/vision/framenormalizer.h:33

---

{#height-3}

#### height

```cpp
int height = 0
```

Defined in src/vision/include/icy/vision/framenormalizer.h:34

---

{#pixelfmt-2}

#### pixelFmt

```cpp
std::string pixelFmt
```

Defined in src/vision/include/icy/vision/framenormalizer.h:35

{#framenormalizerstats}

## FrameNormalizerStats

```cpp
#include <icy/vision/framenormalizer.h>
```

```cpp
struct FrameNormalizerStats
```

Defined in src/vision/include/icy/vision/framenormalizer.h:39

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `uint64_t` | [`seen`](#seen-1)  |  |
| `uint64_t` | [`emitted`](#emitted-1)  |  |
| `uint64_t` | [`dropped`](#dropped)  |  |
| `uint64_t` | [`converted`](#converted)  |  |

---

{#seen-1}

#### seen

```cpp
uint64_t seen = 0
```

Defined in src/vision/include/icy/vision/framenormalizer.h:41

---

{#emitted-1}

#### emitted

```cpp
uint64_t emitted = 0
```

Defined in src/vision/include/icy/vision/framenormalizer.h:42

---

{#dropped}

#### dropped

```cpp
uint64_t dropped = 0
```

Defined in src/vision/include/icy/vision/framenormalizer.h:43

---

{#converted}

#### converted

```cpp
uint64_t converted = 0
```

Defined in src/vision/include/icy/vision/framenormalizer.h:44

{#framesamplerconfig}

## FrameSamplerConfig

```cpp
#include <icy/vision/framesampler.h>
```

```cpp
struct FrameSamplerConfig
```

Defined in src/vision/include/icy/vision/framesampler.h:28

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `uint32_t` | [`everyNthFrame`](#everynthframe)  |  |
| `int64_t` | [`minIntervalUsec`](#minintervalusec)  |  |

---

{#everynthframe}

#### everyNthFrame

```cpp
uint32_t everyNthFrame = 1
```

Defined in src/vision/include/icy/vision/framesampler.h:30

---

{#minintervalusec}

#### minIntervalUsec

```cpp
int64_t minIntervalUsec = 0
```

Defined in src/vision/include/icy/vision/framesampler.h:31

{#framesamplerstats}

## FrameSamplerStats

```cpp
#include <icy/vision/framesampler.h>
```

```cpp
struct FrameSamplerStats
```

Defined in src/vision/include/icy/vision/framesampler.h:35

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `uint64_t` | [`seen`](#seen-2)  |  |
| `uint64_t` | [`forwarded`](#forwarded)  |  |
| `uint64_t` | [`dropped`](#dropped-1)  |  |

---

{#seen-2}

#### seen

```cpp
uint64_t seen = 0
```

Defined in src/vision/include/icy/vision/framesampler.h:37

---

{#forwarded}

#### forwarded

```cpp
uint64_t forwarded = 0
```

Defined in src/vision/include/icy/vision/framesampler.h:38

---

{#dropped-1}

#### dropped

```cpp
uint64_t dropped = 0
```

Defined in src/vision/include/icy/vision/framesampler.h:39

{#motiondetectorconfig}

## MotionDetectorConfig

```cpp
#include <icy/vision/motiondetector.h>
```

```cpp
struct MotionDetectorConfig
```

Defined in src/vision/include/icy/vision/motiondetector.h:28

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`source`](#source-2)  |  |
| `std::string` | [`streamId`](#streamid-1)  |  |
| `std::string` | [`detectorName`](#detectorname-1)  |  |
| `uint32_t` | [`gridWidth`](#gridwidth)  |  |
| `uint32_t` | [`gridHeight`](#gridheight)  |  |
| `uint32_t` | [`warmupFrames`](#warmupframes)  |  |
| `float` | [`threshold`](#threshold)  |  |
| `int64_t` | [`minEventIntervalUsec`](#mineventintervalusec)  |  |

---

{#source-2}

#### source

```cpp
std::string source
```

Defined in src/vision/include/icy/vision/motiondetector.h:30

---

{#streamid-1}

#### streamId

```cpp
std::string streamId
```

Defined in src/vision/include/icy/vision/motiondetector.h:31

---

{#detectorname-1}

#### detectorName

```cpp
std::string detectorName = "motion"
```

Defined in src/vision/include/icy/vision/motiondetector.h:32

---

{#gridwidth}

#### gridWidth

```cpp
uint32_t gridWidth = 32
```

Defined in src/vision/include/icy/vision/motiondetector.h:33

---

{#gridheight}

#### gridHeight

```cpp
uint32_t gridHeight = 18
```

Defined in src/vision/include/icy/vision/motiondetector.h:34

---

{#warmupframes}

#### warmupFrames

```cpp
uint32_t warmupFrames = 2
```

Defined in src/vision/include/icy/vision/motiondetector.h:35

---

{#threshold}

#### threshold

```cpp
float threshold = 0.08f
```

Defined in src/vision/include/icy/vision/motiondetector.h:36

---

{#mineventintervalusec}

#### minEventIntervalUsec

```cpp
int64_t minEventIntervalUsec = 500000
```

Defined in src/vision/include/icy/vision/motiondetector.h:37

{#motiondetectorstats}

## MotionDetectorStats

```cpp
#include <icy/vision/motiondetector.h>
```

```cpp
struct MotionDetectorStats
```

Defined in src/vision/include/icy/vision/motiondetector.h:41

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `uint64_t` | [`seen`](#seen-3)  |  |
| `uint64_t` | [`emitted`](#emitted-2)  |  |
| `float` | [`lastScore`](#lastscore)  |  |

---

{#seen-3}

#### seen

```cpp
uint64_t seen = 0
```

Defined in src/vision/include/icy/vision/motiondetector.h:43

---

{#emitted-2}

#### emitted

```cpp
uint64_t emitted = 0
```

Defined in src/vision/include/icy/vision/motiondetector.h:44

---

{#lastscore}

#### lastScore

```cpp
float lastScore = 0.0f
```

Defined in src/vision/include/icy/vision/motiondetector.h:45

{#region-1}

## Region

```cpp
#include <icy/vision/types.h>
```

```cpp
struct Region
```

Defined in src/vision/include/icy/vision/types.h:43

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `float` | [`x`](#x)  |  |
| `float` | [`y`](#y)  |  |
| `float` | [`width`](#width-4)  |  |
| `float` | [`height`](#height-4)  |  |

---

{#x}

#### x

```cpp
float x = 0.0f
```

Defined in src/vision/include/icy/vision/types.h:45

---

{#y}

#### y

```cpp
float y = 0.0f
```

Defined in src/vision/include/icy/vision/types.h:46

---

{#width-4}

#### width

```cpp
float width = 0.0f
```

Defined in src/vision/include/icy/vision/types.h:47

---

{#height-4}

#### height

```cpp
float height = 0.0f
```

Defined in src/vision/include/icy/vision/types.h:48

{#track}

## Track

```cpp
#include <icy/vision/types.h>
```

```cpp
struct Track
```

Defined in src/vision/include/icy/vision/types.h:62

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `int64_t` | [`trackId`](#trackid-1)  |  |
| `std::string` | [`label`](#label-2)  |  |
| `float` | [`confidence`](#confidence-1)  |  |
| `Region` | [`region`](#region-2)  |  |
| `json::Value` | [`data`](#data-8)  |  |

---

{#trackid-1}

#### trackId

```cpp
int64_t trackId = -1
```

Defined in src/vision/include/icy/vision/types.h:64

---

{#label-2}

#### label

```cpp
std::string label
```

Defined in src/vision/include/icy/vision/types.h:65

---

{#confidence-1}

#### confidence

```cpp
float confidence = 0.0f
```

Defined in src/vision/include/icy/vision/types.h:66

---

{#region-2}

#### region

```cpp
Region region
```

Defined in src/vision/include/icy/vision/types.h:67

---

{#data-8}

#### data

```cpp
json::Value data = json::Value::object()
```

Defined in src/vision/include/icy/vision/types.h:68

{#visionevent}

## VisionEvent

```cpp
#include <icy/vision/event.h>
```

```cpp
struct VisionEvent
```

Defined in src/vision/include/icy/vision/event.h:30

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`schema`](#schema)  |  |
| `std::string` | [`type`](#type-24)  |  |
| `std::string` | [`source`](#source-3)  |  |
| `std::string` | [`detector`](#detector-2)  |  |
| `int64_t` | [`emittedAtUsec`](#emittedatusec-1)  |  |
| `VisionFrameContext` | [`frame`](#frame-2)  |  |
| `std::vector< Detection >` | [`detections`](#detections)  |  |
| `std::vector< Track >` | [`tracks`](#tracks)  |  |
| `json::Value` | [`data`](#data-9)  |  |

---

{#schema}

#### schema

```cpp
std::string schema = "vision.event.v1"
```

Defined in src/vision/include/icy/vision/event.h:32

---

{#type-24}

#### type

```cpp
std::string type = "detect"
```

Defined in src/vision/include/icy/vision/event.h:33

---

{#source-3}

#### source

```cpp
std::string source
```

Defined in src/vision/include/icy/vision/event.h:34

---

{#detector-2}

#### detector

```cpp
std::string detector
```

Defined in src/vision/include/icy/vision/event.h:35

---

{#emittedatusec-1}

#### emittedAtUsec

```cpp
int64_t emittedAtUsec = 0
```

Defined in src/vision/include/icy/vision/event.h:36

---

{#frame-2}

#### frame

```cpp
VisionFrameContext frame
```

Defined in src/vision/include/icy/vision/event.h:37

---

{#detections}

#### detections

```cpp
std::vector< Detection > detections
```

Defined in src/vision/include/icy/vision/event.h:38

---

{#tracks}

#### tracks

```cpp
std::vector< Track > tracks
```

Defined in src/vision/include/icy/vision/event.h:39

---

{#data-9}

#### data

```cpp
json::Value data = json::Value::object()
```

Defined in src/vision/include/icy/vision/event.h:40

{#visionframecontext}

## VisionFrameContext

```cpp
#include <icy/vision/types.h>
```

```cpp
struct VisionFrameContext
```

Defined in src/vision/include/icy/vision/types.h:26

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`sourceId`](#sourceid-1)  |  |
| `std::string` | [`streamId`](#streamid-2)  |  |
| `uint64_t` | [`frameId`](#frameid)  |  |
| `int64_t` | [`ptsUsec`](#ptsusec)  |  |
| `int64_t` | [`receivedAtUsec`](#receivedatusec)  |  |
| `int` | [`width`](#width-5)  |  |
| `int` | [`height`](#height-5)  |  |
| `std::string` | [`pixelFmt`](#pixelfmt-3)  |  |
| `bool` | [`keyframe`](#keyframe)  |  |

---

{#sourceid-1}

#### sourceId

```cpp
std::string sourceId
```

Defined in src/vision/include/icy/vision/types.h:28

---

{#streamid-2}

#### streamId

```cpp
std::string streamId
```

Defined in src/vision/include/icy/vision/types.h:29

---

{#frameid}

#### frameId

```cpp
uint64_t frameId = 0
```

Defined in src/vision/include/icy/vision/types.h:30

---

{#ptsusec}

#### ptsUsec

```cpp
int64_t ptsUsec = 0
```

Defined in src/vision/include/icy/vision/types.h:31

---

{#receivedatusec}

#### receivedAtUsec

```cpp
int64_t receivedAtUsec = 0
```

Defined in src/vision/include/icy/vision/types.h:32

---

{#width-5}

#### width

```cpp
int width = 0
```

Defined in src/vision/include/icy/vision/types.h:33

---

{#height-5}

#### height

```cpp
int height = 0
```

Defined in src/vision/include/icy/vision/types.h:34

---

{#pixelfmt-3}

#### pixelFmt

```cpp
std::string pixelFmt
```

Defined in src/vision/include/icy/vision/types.h:35

---

{#keyframe}

#### keyframe

```cpp
bool keyframe = false
```

Defined in src/vision/include/icy/vision/types.h:36

{#visionframepacket}

## VisionFramePacket

```cpp
#include <icy/vision/framepacket.h>
```

```cpp
struct VisionFramePacket
```

Defined in src/vision/include/icy/vision/framepacket.h:25

> **Inherits:** [`PlanarVideoPacket`](av.md#planarvideopacket)

Owned detector-ready frame buffer carrying stable frame metadata.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `VisionFrameContext` | [`context`](#context-5)  |  |

---

{#context-5}

#### context

```cpp
VisionFrameContext context
```

Defined in src/vision/include/icy/vision/framepacket.h:27

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`VisionFramePacket`](#visionframepacket-1)  |  |
|  | [`VisionFramePacket`](#visionframepacket-2)  |  |
|  | [`VisionFramePacket`](#visionframepacket-3)  | Deleted constructor. |
| `std::unique_ptr< IPacket >` | [`clone`](#clone-19) `virtual` `const` `inline` `override` |  |
| `const char *` | [`className`](#classname-11) `virtual` `const` `inline` `override` | Returns the class name of this packet type for logging and diagnostics. |
| `bool` | [`valid`](#valid-17) `const` `inline` `nodiscard` |  |

---

{#visionframepacket-1}

#### VisionFramePacket

```cpp
VisionFramePacket(const av::PlanarVideoPacket & packet, VisionFrameContext context = {})
```

Defined in src/vision/include/icy/vision/framepacket.h:29

---

{#visionframepacket-2}

#### VisionFramePacket

```cpp
VisionFramePacket(const VisionFramePacket & that)
```

Defined in src/vision/include/icy/vision/framepacket.h:31

---

{#visionframepacket-3}

#### VisionFramePacket

```cpp
VisionFramePacket(VisionFramePacket &&) = delete
```

Defined in src/vision/include/icy/vision/framepacket.h:33

Deleted constructor.

---

{#clone-19}

#### clone

`virtual` `const` `inline` `override`

```cpp
virtual inline std::unique_ptr< IPacket > clone() const override
```

Defined in src/vision/include/icy/vision/framepacket.h:37

#### Returns
A heap-allocated deep copy of this packet.

---

{#classname-11}

#### className

`virtual` `const` `inline` `override`

```cpp
virtual inline const char * className() const override
```

Defined in src/vision/include/icy/vision/framepacket.h:42

Returns the class name of this packet type for logging and diagnostics.

---

{#valid-17}

#### valid

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline bool valid() const
```

Defined in src/vision/include/icy/vision/framepacket.h:44

