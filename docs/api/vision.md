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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`DetectionQueue`](#detectionqueue-1) | `function` | Declared here |
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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`Event`](#event-6) | `variable` | Declared here |
| [`reset`](#reset-16) | `function` | Declared here |
| [`process`](#process-8) | `function` | Declared here |

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

##### Reimplemented by

- [`reset`](#reset-19)

---

{#process-8}

#### process

`virtual`

```cpp
virtual void process(const VisionFramePacket & packet)
```

Defined in src/vision/include/icy/vision/detector.h:33

##### Reimplemented by

- [`process`](#process-11)

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`emitter`](#emitter-8) | `variable` | Declared here |
| [`FrameNormalizer`](#framenormalizer-1) | `function` | Declared here |
| [`setConfig`](#setconfig-1) | `function` | Declared here |
| [`config`](#config-1) | `function` | Declared here |
| [`stats`](#stats-1) | `function` | Declared here |
| [`reset`](#reset-17) | `function` | Declared here |
| [`process`](#process-9) | `function` | Declared here |
| [`accepts`](#accepts-3) | `function` | Declared here |
| [`_config`](#_config-1) | `variable` | Declared here |
| [`_seen`](#_seen-1) | `variable` | Declared here |
| [`_emitted`](#_emitted-1) | `variable` | Declared here |
| [`_dropped`](#_dropped) | `variable` | Declared here |
| [`_converted`](#_converted) | `variable` | Declared here |
| [`_frameId`](#_frameid) | `variable` | Declared here |
| [`_state`](#_state-3) | `variable` | Declared here |
| [`normalize`](#normalize-1) | `function` | Declared here |
| [`sanitize`](#sanitize-1) | `function` | Declared here |
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

##### Reimplements

- [`process`](base.md#classicy_1_1PacketProcessor_1acd9fb1a5fa525c0a2d96971fe6c06294)

---

{#accepts-3}

#### accepts

`virtual` `inline` `override`

```cpp
virtual inline bool accepts(IPacket *) override
```

Defined in src/vision/include/icy/vision/framenormalizer.h:63

This method ensures compatibility with the given packet type. Return false to reject the packet.

##### Reimplements

- [`accepts`](base.md#classicy_1_1PacketProcessor_1a838d9d75f81bd943b40efcbe683394b7)

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`emitter`](#emitter-9) | `variable` | Declared here |
| [`FrameSampler`](#framesampler-1) | `function` | Declared here |
| [`setConfig`](#setconfig-2) | `function` | Declared here |
| [`config`](#config-2) | `function` | Declared here |
| [`stats`](#stats-2) | `function` | Declared here |
| [`reset`](#reset-18) | `function` | Declared here |
| [`process`](#process-10) | `function` | Declared here |
| [`accepts`](#accepts-4) | `function` | Declared here |
| [`_config`](#_config-2) | `variable` | Declared here |
| [`_seen`](#_seen-2) | `variable` | Declared here |
| [`_forwarded`](#_forwarded) | `variable` | Declared here |
| [`_dropped`](#_dropped-1) | `variable` | Declared here |
| [`_lastForwardedTimeUsec`](#_lastforwardedtimeusec) | `variable` | Declared here |
| [`shouldForward`](#shouldforward) | `function` | Declared here |
| [`kUnsetTimeUsec`](#kunsettimeusec) | `variable` | Declared here |
| [`sanitize`](#sanitize-2) | `function` | Declared here |
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

##### Reimplements

- [`process`](base.md#classicy_1_1PacketProcessor_1acd9fb1a5fa525c0a2d96971fe6c06294)

---

{#accepts-4}

#### accepts

`virtual` `inline` `override`

```cpp
virtual inline bool accepts(IPacket *) override
```

Defined in src/vision/include/icy/vision/framesampler.h:107

This method ensures compatibility with the given packet type. Return false to reject the packet.

##### Reimplements

- [`accepts`](base.md#classicy_1_1PacketProcessor_1a838d9d75f81bd943b40efcbe683394b7)

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`MotionDetector`](#motiondetector-1) | `function` | Declared here |
| [`setConfig`](#setconfig-3) | `function` | Declared here |
| [`config`](#config-3) | `function` | Declared here |
| [`stats`](#stats-3) | `function` | Declared here |
| [`reset`](#reset-19) | `function` | Declared here |
| [`process`](#process-11) | `function` | Declared here |
| [`_config`](#_config-3) | `variable` | Declared here |
| [`_seen`](#_seen-3) | `variable` | Declared here |
| [`_emitted`](#_emitted-2) | `variable` | Declared here |
| [`_warmedFrames`](#_warmedframes) | `variable` | Declared here |
| [`_width`](#_width) | `variable` | Declared here |
| [`_height`](#_height) | `variable` | Declared here |
| [`_lastEventTimeUsec`](#_lasteventtimeusec) | `variable` | Declared here |
| [`_lastScore`](#_lastscore) | `variable` | Declared here |
| [`_previousGrid`](#_previousgrid) | `variable` | Declared here |
| [`_currentGrid`](#_currentgrid) | `variable` | Declared here |
| [`sampleLumaGrid`](#samplelumagrid) | `function` | Declared here |
| [`diffScore`](#diffscore) | `function` | Declared here |
| [`emitEvent`](#emitevent-1) | `function` | Declared here |
| [`sanitize`](#sanitize-3) | `function` | Declared here |
| [`Event`](#event-6) | `variable` | Inherited from [`Detector`](#detector-1) |
| [`reset`](#reset-16) | `function` | Inherited from [`Detector`](#detector-1) |
| [`process`](#process-8) | `function` | Inherited from [`Detector`](#detector-1) |

### Inherited from [`Detector`](#detector-1)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`Event`](#event-6)  |  |
| `function` | [`reset`](#reset-16) `virtual` |  |
| `function` | [`process`](#process-8) `virtual` |  |

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

##### Reimplements

- [`reset`](#reset-16)

---

{#process-11}

#### process

`virtual` `override`

```cpp
virtual void process(const VisionFramePacket & packet) override
```

Defined in src/vision/include/icy/vision/motiondetector.h:59

##### Reimplements

- [`process`](#process-8)

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`Event`](#event-7) | `variable` | Declared here |
| [`emit`](#emit-1) | `function` | Declared here |

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`label`](#label-1) | `variable` | Declared here |
| [`confidence`](#confidence) | `variable` | Declared here |
| [`trackId`](#trackid) | `variable` | Declared here |
| [`region`](#region) | `variable` | Declared here |
| [`data`](#data-7) | `variable` | Declared here |

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`sourceId`](#sourceid) | `variable` | Declared here |
| [`streamId`](#streamid) | `variable` | Declared here |
| [`width`](#width-3) | `variable` | Declared here |
| [`height`](#height-3) | `variable` | Declared here |
| [`pixelFmt`](#pixelfmt-2) | `variable` | Declared here |

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`seen`](#seen-1) | `variable` | Declared here |
| [`emitted`](#emitted-1) | `variable` | Declared here |
| [`dropped`](#dropped) | `variable` | Declared here |
| [`converted`](#converted) | `variable` | Declared here |

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`everyNthFrame`](#everynthframe) | `variable` | Declared here |
| [`minIntervalUsec`](#minintervalusec) | `variable` | Declared here |

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`seen`](#seen-2) | `variable` | Declared here |
| [`forwarded`](#forwarded) | `variable` | Declared here |
| [`dropped`](#dropped-1) | `variable` | Declared here |

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`source`](#source-2) | `variable` | Declared here |
| [`streamId`](#streamid-1) | `variable` | Declared here |
| [`detectorName`](#detectorname-1) | `variable` | Declared here |
| [`gridWidth`](#gridwidth) | `variable` | Declared here |
| [`gridHeight`](#gridheight) | `variable` | Declared here |
| [`warmupFrames`](#warmupframes) | `variable` | Declared here |
| [`threshold`](#threshold) | `variable` | Declared here |
| [`minEventIntervalUsec`](#mineventintervalusec) | `variable` | Declared here |

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`seen`](#seen-3) | `variable` | Declared here |
| [`emitted`](#emitted-2) | `variable` | Declared here |
| [`lastScore`](#lastscore) | `variable` | Declared here |

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`x`](#x) | `variable` | Declared here |
| [`y`](#y) | `variable` | Declared here |
| [`width`](#width-4) | `variable` | Declared here |
| [`height`](#height-4) | `variable` | Declared here |

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`trackId`](#trackid-1) | `variable` | Declared here |
| [`label`](#label-2) | `variable` | Declared here |
| [`confidence`](#confidence-1) | `variable` | Declared here |
| [`region`](#region-2) | `variable` | Declared here |
| [`data`](#data-8) | `variable` | Declared here |

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`schema`](#schema) | `variable` | Declared here |
| [`type`](#type-24) | `variable` | Declared here |
| [`source`](#source-3) | `variable` | Declared here |
| [`detector`](#detector-2) | `variable` | Declared here |
| [`emittedAtUsec`](#emittedatusec-1) | `variable` | Declared here |
| [`frame`](#frame-2) | `variable` | Declared here |
| [`detections`](#detections) | `variable` | Declared here |
| [`tracks`](#tracks) | `variable` | Declared here |
| [`data`](#data-9) | `variable` | Declared here |

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`sourceId`](#sourceid-1) | `variable` | Declared here |
| [`streamId`](#streamid-2) | `variable` | Declared here |
| [`frameId`](#frameid) | `variable` | Declared here |
| [`ptsUsec`](#ptsusec) | `variable` | Declared here |
| [`receivedAtUsec`](#receivedatusec) | `variable` | Declared here |
| [`width`](#width-5) | `variable` | Declared here |
| [`height`](#height-5) | `variable` | Declared here |
| [`pixelFmt`](#pixelfmt-3) | `variable` | Declared here |
| [`keyframe`](#keyframe) | `variable` | Declared here |

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`context`](#context-5) | `variable` | Declared here |
| [`VisionFramePacket`](#visionframepacket-1) | `function` | Declared here |
| [`VisionFramePacket`](#visionframepacket-2) | `function` | Declared here |
| [`VisionFramePacket`](#visionframepacket-3) | `function` | Declared here |
| [`clone`](#clone-19) | `function` | Declared here |
| [`className`](#classname-11) | `function` | Declared here |
| [`valid`](#valid-17) | `function` | Declared here |
| [`buffer`](av.md#buffer) | `variable` | Inherited from [`PlanarVideoPacket`](av.md#planarvideopacket) |
| [`linesize`](av.md#linesize) | `variable` | Inherited from [`PlanarVideoPacket`](av.md#planarvideopacket) |
| [`pixelFmt`](av.md#pixelfmt-1) | `variable` | Inherited from [`PlanarVideoPacket`](av.md#planarvideopacket) |
| [`owns_buffer`](av.md#owns_buffer) | `variable` | Inherited from [`PlanarVideoPacket`](av.md#planarvideopacket) |
| [`avframe`](av.md#avframe) | `variable` | Inherited from [`PlanarVideoPacket`](av.md#planarvideopacket) |
| [`PlanarVideoPacket`](av.md#planarvideopacket-1) | `function` | Inherited from [`PlanarVideoPacket`](av.md#planarvideopacket) |
| [`PlanarVideoPacket`](av.md#planarvideopacket-2) | `function` | Inherited from [`PlanarVideoPacket`](av.md#planarvideopacket) |
| [`clone`](av.md#clone-2) | `function` | Inherited from [`PlanarVideoPacket`](av.md#planarvideopacket) |
| [`className`](av.md#classname-2) | `function` | Inherited from [`PlanarVideoPacket`](av.md#planarvideopacket) |
| [`width`](av.md#width-2) | `variable` | Inherited from [`VideoPacket`](av.md#videopacket) |
| [`height`](av.md#height-2) | `variable` | Inherited from [`VideoPacket`](av.md#videopacket) |
| [`iframe`](av.md#iframe) | `variable` | Inherited from [`VideoPacket`](av.md#videopacket) |
| [`avpacket`](av.md#avpacket) | `variable` | Inherited from [`VideoPacket`](av.md#videopacket) |
| [`VideoPacket`](av.md#videopacket-1) | `function` | Inherited from [`VideoPacket`](av.md#videopacket) |
| [`VideoPacket`](av.md#videopacket-2) | `function` | Inherited from [`VideoPacket`](av.md#videopacket) |
| [`clone`](av.md#clone-1) | `function` | Inherited from [`VideoPacket`](av.md#videopacket) |
| [`className`](av.md#classname-1) | `function` | Inherited from [`VideoPacket`](av.md#videopacket) |
| [`time`](av.md#time-2) | `variable` | Inherited from [`MediaPacket`](av.md#mediapacket) |
| [`MediaPacket`](av.md#mediapacket-1) | `function` | Inherited from [`MediaPacket`](av.md#mediapacket) |
| [`MediaPacket`](av.md#mediapacket-2) | `function` | Inherited from [`MediaPacket`](av.md#mediapacket) |
| [`MediaPacket`](av.md#mediapacket-3) | `function` | Inherited from [`MediaPacket`](av.md#mediapacket) |
| [`clone`](av.md#clone) | `function` | Inherited from [`MediaPacket`](av.md#mediapacket) |
| [`className`](av.md#classname) | `function` | Inherited from [`MediaPacket`](av.md#mediapacket) |
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

### Inherited from [`PlanarVideoPacket`](av.md#planarvideopacket)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`buffer`](av.md#buffer)  |  |
| `variable` | [`linesize`](av.md#linesize)  |  |
| `variable` | [`pixelFmt`](av.md#pixelfmt-1)  |  |
| `variable` | [`owns_buffer`](av.md#owns_buffer)  |  |
| `variable` | [`avframe`](av.md#avframe)  | Non-owning pointer to the decoded AVFrame from FFmpeg. Set by [VideoDecoder](av.md#videodecoder). |
| `function` | [`PlanarVideoPacket`](av.md#planarvideopacket-1)  | Construct a planar video packet, copying the plane pointers (not the pixel data). |
| `function` | [`PlanarVideoPacket`](av.md#planarvideopacket-2)  | Copy constructor. Performs a deep copy of the owned buffer if owns_buffer is set. |
| `function` | [`clone`](av.md#clone-2) `virtual` `const` `inline` `override` |  |
| `function` | [`className`](av.md#classname-2) `virtual` `const` `inline` `override` | Returns the class name of this packet type for logging and diagnostics. |

### Inherited from [`VideoPacket`](av.md#videopacket)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`width`](av.md#width-2)  | Frame width in pixels. |
| `variable` | [`height`](av.md#height-2)  | Frame height in pixels. |
| `variable` | [`iframe`](av.md#iframe)  | True if this is an intra (keyframe) frame. |
| `variable` | [`avpacket`](av.md#avpacket)  | Non-owning pointer to the encoded AVPacket from FFmpeg. Set by [VideoEncoder](av.md#videoencoder), read by [MultiplexEncoder](av.md#multiplexencoder). |
| `function` | [`VideoPacket`](av.md#videopacket-1) `inline` | Construct a video packet with an interleaved buffer. |
| `function` | [`VideoPacket`](av.md#videopacket-2) `inline` | Copy constructor. The avpacket pointer is shallow-copied (non-owning). |
| `function` | [`clone`](av.md#clone-1) `virtual` `const` `inline` `override` |  |
| `function` | [`className`](av.md#classname-1) `virtual` `const` `inline` `override` | Returns the class name of this packet type for logging and diagnostics. |

### Inherited from [`MediaPacket`](av.md#mediapacket)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`time`](av.md#time-2)  | Presentation timestamp in microseconds. |
| `function` | [`MediaPacket`](av.md#mediapacket-1) `inline` | Construct with a non-owning or owning mutable buffer. |
| `function` | [`MediaPacket`](av.md#mediapacket-2) `inline` | Construct with const data (copied, owning). |
| `function` | [`MediaPacket`](av.md#mediapacket-3) `inline` | Copy constructor. |
| `function` | [`clone`](av.md#clone) `virtual` `const` `inline` `override` |  |
| `function` | [`className`](av.md#classname) `virtual` `const` `inline` `override` | Returns the class name of this packet type for logging and diagnostics. |

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

##### Reimplements

- [`clone`](av.md#clone-2)

---

{#classname-11}

#### className

`virtual` `const` `inline` `override`

```cpp
virtual inline const char * className() const override
```

Defined in src/vision/include/icy/vision/framepacket.h:42

Returns the class name of this packet type for logging and diagnostics.

##### Reimplements

- [`className`](av.md#classname-2)

---

{#valid-17}

#### valid

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline bool valid() const
```

Defined in src/vision/include/icy/vision/framepacket.h:44

