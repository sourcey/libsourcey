{#speechmodule}

# speech

Audio intelligence primitives for decoded media streams.

### Namespaces

| Name | Description |
|------|-------------|
| [`speech`](#speech) |  |

{#speech}

# speech

### Classes

| Name | Description |
|------|-------------|
| [`SpeechQueue`](#speechqueue) | Async queue for decoded audio packets. |
| [`VoiceActivityDetector`](#voiceactivitydetector) |  |
| [`AudioRef`](#audioref) |  |
| [`SpeechEvent`](#speechevent) |  |
| [`VoiceActivityDetectorConfig`](#voiceactivitydetectorconfig) |  |
| [`VoiceActivityDetectorStats`](#voiceactivitydetectorstats) |  |

### Functions

| Return | Name | Description |
|--------|------|-------------|
| `Speech_APIAudioRef` | [`makeAudioRef`](#makeaudioref) `nodiscard` |  |
| `Speech_APIjson::Value` | [`toJson`](#tojson-1) `nodiscard` |  |
| `Speech_APIjson::Value` | [`toJson`](#tojson-2) `nodiscard` |  |

---

{#makeaudioref}

#### makeAudioRef

`nodiscard`

```cpp
[[nodiscard]] Speech_APIAudioRef makeAudioRef(const av::PlanarAudioPacket & packet, uint64_t sequence = 0, int sampleRateHint = 0)
```

---

{#tojson-1}

#### toJson

`nodiscard`

```cpp
[[nodiscard]] Speech_APIjson::Value toJson(const AudioRef & audio)
```

---

{#tojson-2}

#### toJson

`nodiscard`

```cpp
[[nodiscard]] Speech_APIjson::Value toJson(const SpeechEvent & event)
```

{#speechqueue}

## SpeechQueue

```cpp
#include <icy/speech/speechqueue.h>
```

```cpp
class SpeechQueue
```

Defined in src/speech/include/icy/speech/speechqueue.h:28

> **Inherits:** [`PlanarAudioPacket >`](base.md#asyncpacketqueue)

Async queue for decoded audio packets.

This is the explicit clone boundary for speech processing. Upstream stages can stay borrowed/zero-copy until a packet enters this queue.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`SpeechQueue`](#speechqueue-1) `inline` `explicit` |  |

---

{#speechqueue-1}

#### SpeechQueue

`inline` `explicit`

```cpp
inline explicit SpeechQueue(int maxPackets = 64)
```

Defined in src/speech/include/icy/speech/speechqueue.h:31

{#voiceactivitydetector}

## VoiceActivityDetector

```cpp
#include <icy/speech/voiceactivitydetector.h>
```

```cpp
class VoiceActivityDetector
```

Defined in src/speech/include/icy/speech/voiceactivitydetector.h:50

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `Signal< void(const SpeechEvent &)>` | [`Event`](#event-1)  |  |

---

{#event-1}

#### Event

```cpp
Signal< void(const SpeechEvent &)> Event
```

Defined in src/speech/include/icy/speech/voiceactivitydetector.h:60

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`VoiceActivityDetector`](#voiceactivitydetector-1) `explicit` |  |
| `void` | [`setConfig`](#setconfig)  |  |
| `VoiceActivityDetectorConfig` | [`config`](#config) `const` `nodiscard` |  |
| `VoiceActivityDetectorStats` | [`stats`](#stats) `const` `nodiscard` |  |
| `void` | [`reset`](#reset-14)  |  |
| `void` | [`process`](#process-7)  |  |

---

{#voiceactivitydetector-1}

#### VoiceActivityDetector

`explicit`

```cpp
explicit VoiceActivityDetector(VoiceActivityDetectorConfig config = {})
```

Defined in src/speech/include/icy/speech/voiceactivitydetector.h:62

---

{#setconfig}

#### setConfig

```cpp
void setConfig(VoiceActivityDetectorConfig config)
```

Defined in src/speech/include/icy/speech/voiceactivitydetector.h:64

---

{#config}

#### config

`const` `nodiscard`

```cpp
[[nodiscard]] VoiceActivityDetectorConfig config() const
```

Defined in src/speech/include/icy/speech/voiceactivitydetector.h:65

---

{#stats}

#### stats

`const` `nodiscard`

```cpp
[[nodiscard]] VoiceActivityDetectorStats stats() const
```

Defined in src/speech/include/icy/speech/voiceactivitydetector.h:66

---

{#reset-14}

#### reset

```cpp
void reset()
```

Defined in src/speech/include/icy/speech/voiceactivitydetector.h:67

---

{#process-7}

#### process

```cpp
void process(const av::PlanarAudioPacket & packet)
```

Defined in src/speech/include/icy/speech/voiceactivitydetector.h:69

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `VoiceActivityDetectorConfig` | [`_config`](#_config)  |  |
| `uint64_t` | [`_sequence`](#_sequence)  |  |
| `uint64_t` | [`_seen`](#_seen)  |  |
| `uint64_t` | [`_emitted`](#_emitted)  |  |
| `int64_t` | [`_lastUpdateUsec`](#_lastupdateusec)  |  |
| `int64_t` | [`_lastBelowThresholdUsec`](#_lastbelowthresholdusec)  |  |
| `bool` | [`_active`](#_active-1)  |  |
| `float` | [`_lastLevel`](#_lastlevel)  |  |
| `float` | [`_lastPeak`](#_lastpeak)  |  |

---

{#_config}

#### _config

```cpp
VoiceActivityDetectorConfig _config
```

Defined in src/speech/include/icy/speech/voiceactivitydetector.h:79

---

{#_sequence}

#### _sequence

```cpp
uint64_t _sequence = 0
```

Defined in src/speech/include/icy/speech/voiceactivitydetector.h:80

---

{#_seen}

#### _seen

```cpp
uint64_t _seen = 0
```

Defined in src/speech/include/icy/speech/voiceactivitydetector.h:81

---

{#_emitted}

#### _emitted

```cpp
uint64_t _emitted = 0
```

Defined in src/speech/include/icy/speech/voiceactivitydetector.h:82

---

{#_lastupdateusec}

#### _lastUpdateUsec

```cpp
int64_t _lastUpdateUsec = 0
```

Defined in src/speech/include/icy/speech/voiceactivitydetector.h:83

---

{#_lastbelowthresholdusec}

#### _lastBelowThresholdUsec

```cpp
int64_t _lastBelowThresholdUsec = 0
```

Defined in src/speech/include/icy/speech/voiceactivitydetector.h:84

---

{#_active-1}

#### _active

```cpp
bool _active = false
```

Defined in src/speech/include/icy/speech/voiceactivitydetector.h:85

---

{#_lastlevel}

#### _lastLevel

```cpp
float _lastLevel = 0.0f
```

Defined in src/speech/include/icy/speech/voiceactivitydetector.h:86

---

{#_lastpeak}

#### _lastPeak

```cpp
float _lastPeak = 0.0f
```

Defined in src/speech/include/icy/speech/voiceactivitydetector.h:87

### Private Methods

| Return | Name | Description |
|--------|------|-------------|
| `Levels` | [`measure`](#measure) `const` `nodiscard` |  |
| `void` | [`emitEvent`](#emitevent)  |  |

---

{#measure}

#### measure

`const` `nodiscard`

```cpp
[[nodiscard]] Levels measure(const av::PlanarAudioPacket & packet) const
```

Defined in src/speech/include/icy/speech/voiceactivitydetector.h:73

---

{#emitevent}

#### emitEvent

```cpp
void emitEvent(const char * type, const av::PlanarAudioPacket & packet, const Levels & levels, bool active)
```

Defined in src/speech/include/icy/speech/voiceactivitydetector.h:74

### Private Static Methods

| Return | Name | Description |
|--------|------|-------------|
| `VoiceActivityDetectorConfig` | [`sanitize`](#sanitize) `static` |  |

---

{#sanitize}

#### sanitize

`static`

```cpp
static VoiceActivityDetectorConfig sanitize(VoiceActivityDetectorConfig config)
```

Defined in src/speech/include/icy/speech/voiceactivitydetector.h:72

{#levels}

## Levels

```cpp
#include <icy/speech/voiceactivitydetector.h>
```

```cpp
struct Levels
```

Defined in src/speech/include/icy/speech/voiceactivitydetector.h:53

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `float` | [`level`](#level-2)  |  |
| `float` | [`peak`](#peak-1)  |  |
| `bool` | [`valid`](#valid-9)  |  |

---

{#level-2}

#### level

```cpp
float level = 0.0f
```

Defined in src/speech/include/icy/speech/voiceactivitydetector.h:55

---

{#peak-1}

#### peak

```cpp
float peak = 0.0f
```

Defined in src/speech/include/icy/speech/voiceactivitydetector.h:56

---

{#valid-9}

#### valid

```cpp
bool valid = false
```

Defined in src/speech/include/icy/speech/voiceactivitydetector.h:57

{#audioref}

## AudioRef

```cpp
#include <icy/speech/event.h>
```

```cpp
struct AudioRef
```

Defined in src/speech/include/icy/speech/event.h:28

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `uint64_t` | [`sequence`](#sequence)  |  |
| `int64_t` | [`timeUsec`](#timeusec)  |  |
| `int64_t` | [`durationUsec`](#durationusec)  |  |
| `int` | [`sampleRate`](#samplerate-2)  |  |
| `int` | [`channels`](#channels-3)  |  |
| `size_t` | [`numSamples`](#numsamples-1)  |  |
| `std::string` | [`sampleFmt`](#samplefmt-2)  |  |

---

{#sequence}

#### sequence

```cpp
uint64_t sequence = 0
```

Defined in src/speech/include/icy/speech/event.h:30

---

{#timeusec}

#### timeUsec

```cpp
int64_t timeUsec = 0
```

Defined in src/speech/include/icy/speech/event.h:31

---

{#durationusec}

#### durationUsec

```cpp
int64_t durationUsec = 0
```

Defined in src/speech/include/icy/speech/event.h:32

---

{#samplerate-2}

#### sampleRate

```cpp
int sampleRate = 0
```

Defined in src/speech/include/icy/speech/event.h:33

---

{#channels-3}

#### channels

```cpp
int channels = 0
```

Defined in src/speech/include/icy/speech/event.h:34

---

{#numsamples-1}

#### numSamples

```cpp
size_t numSamples = 0
```

Defined in src/speech/include/icy/speech/event.h:35

---

{#samplefmt-2}

#### sampleFmt

```cpp
std::string sampleFmt
```

Defined in src/speech/include/icy/speech/event.h:36

{#speechevent}

## SpeechEvent

```cpp
#include <icy/speech/event.h>
```

```cpp
struct SpeechEvent
```

Defined in src/speech/include/icy/speech/event.h:40

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`type`](#type-18)  |  |
| `std::string` | [`source`](#source)  |  |
| `std::string` | [`detector`](#detector)  |  |
| `int64_t` | [`emittedAtUsec`](#emittedatusec)  |  |
| `AudioRef` | [`audio`](#audio-3)  |  |
| `bool` | [`active`](#active-1)  |  |
| `float` | [`level`](#level-1)  |  |
| `float` | [`peak`](#peak)  |  |
| `json::Value` | [`data`](#data-3)  |  |

---

{#type-18}

#### type

```cpp
std::string type = "speech:update"
```

Defined in src/speech/include/icy/speech/event.h:42

---

{#source}

#### source

```cpp
std::string source
```

Defined in src/speech/include/icy/speech/event.h:43

---

{#detector}

#### detector

```cpp
std::string detector
```

Defined in src/speech/include/icy/speech/event.h:44

---

{#emittedatusec}

#### emittedAtUsec

```cpp
int64_t emittedAtUsec = 0
```

Defined in src/speech/include/icy/speech/event.h:45

---

{#audio-3}

#### audio

```cpp
AudioRef audio
```

Defined in src/speech/include/icy/speech/event.h:46

---

{#active-1}

#### active

```cpp
bool active = false
```

Defined in src/speech/include/icy/speech/event.h:47

---

{#level-1}

#### level

```cpp
float level = 0.0f
```

Defined in src/speech/include/icy/speech/event.h:48

---

{#peak}

#### peak

```cpp
float peak = 0.0f
```

Defined in src/speech/include/icy/speech/event.h:49

---

{#data-3}

#### data

```cpp
json::Value data = json::Value::object()
```

Defined in src/speech/include/icy/speech/event.h:50

{#voiceactivitydetectorconfig}

## VoiceActivityDetectorConfig

```cpp
#include <icy/speech/voiceactivitydetector.h>
```

```cpp
struct VoiceActivityDetectorConfig
```

Defined in src/speech/include/icy/speech/voiceactivitydetector.h:28

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`source`](#source-1)  |  |
| `std::string` | [`detectorName`](#detectorname)  |  |
| `int` | [`sampleRateHint`](#sampleratehint)  |  |
| `float` | [`startThreshold`](#startthreshold)  |  |
| `float` | [`stopThreshold`](#stopthreshold)  |  |
| `int64_t` | [`minSilenceUsec`](#minsilenceusec)  |  |
| `int64_t` | [`updateIntervalUsec`](#updateintervalusec)  |  |

---

{#source-1}

#### source

```cpp
std::string source
```

Defined in src/speech/include/icy/speech/voiceactivitydetector.h:30

---

{#detectorname}

#### detectorName

```cpp
std::string detectorName = "energy_vad"
```

Defined in src/speech/include/icy/speech/voiceactivitydetector.h:31

---

{#sampleratehint}

#### sampleRateHint

```cpp
int sampleRateHint = 0
```

Defined in src/speech/include/icy/speech/voiceactivitydetector.h:32

---

{#startthreshold}

#### startThreshold

```cpp
float startThreshold = 0.045f
```

Defined in src/speech/include/icy/speech/voiceactivitydetector.h:33

---

{#stopthreshold}

#### stopThreshold

```cpp
float stopThreshold = 0.020f
```

Defined in src/speech/include/icy/speech/voiceactivitydetector.h:34

---

{#minsilenceusec}

#### minSilenceUsec

```cpp
int64_t minSilenceUsec = 250000
```

Defined in src/speech/include/icy/speech/voiceactivitydetector.h:35

---

{#updateintervalusec}

#### updateIntervalUsec

```cpp
int64_t updateIntervalUsec = 250000
```

Defined in src/speech/include/icy/speech/voiceactivitydetector.h:36

{#voiceactivitydetectorstats}

## VoiceActivityDetectorStats

```cpp
#include <icy/speech/voiceactivitydetector.h>
```

```cpp
struct VoiceActivityDetectorStats
```

Defined in src/speech/include/icy/speech/voiceactivitydetector.h:40

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `uint64_t` | [`seen`](#seen)  |  |
| `uint64_t` | [`emitted`](#emitted)  |  |
| `bool` | [`active`](#active-2)  |  |
| `float` | [`lastLevel`](#lastlevel)  |  |
| `float` | [`lastPeak`](#lastpeak)  |  |

---

{#seen}

#### seen

```cpp
uint64_t seen = 0
```

Defined in src/speech/include/icy/speech/voiceactivitydetector.h:42

---

{#emitted}

#### emitted

```cpp
uint64_t emitted = 0
```

Defined in src/speech/include/icy/speech/voiceactivitydetector.h:43

---

{#active-2}

#### active

```cpp
bool active = false
```

Defined in src/speech/include/icy/speech/voiceactivitydetector.h:44

---

{#lastlevel}

#### lastLevel

```cpp
float lastLevel = 0.0f
```

Defined in src/speech/include/icy/speech/voiceactivitydetector.h:45

---

{#lastpeak}

#### lastPeak

```cpp
float lastPeak = 0.0f
```

Defined in src/speech/include/icy/speech/voiceactivitydetector.h:46

