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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`SpeechQueue`](#speechqueue-1) | `function` | Declared here |
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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`Event`](#event-1) | `variable` | Declared here |
| [`VoiceActivityDetector`](#voiceactivitydetector-1) | `function` | Declared here |
| [`setConfig`](#setconfig) | `function` | Declared here |
| [`config`](#config) | `function` | Declared here |
| [`stats`](#stats) | `function` | Declared here |
| [`reset`](#reset-14) | `function` | Declared here |
| [`process`](#process-7) | `function` | Declared here |
| [`_config`](#_config) | `variable` | Declared here |
| [`_sequence`](#_sequence) | `variable` | Declared here |
| [`_seen`](#_seen) | `variable` | Declared here |
| [`_emitted`](#_emitted) | `variable` | Declared here |
| [`_lastUpdateUsec`](#_lastupdateusec) | `variable` | Declared here |
| [`_lastBelowThresholdUsec`](#_lastbelowthresholdusec) | `variable` | Declared here |
| [`_active`](#_active-1) | `variable` | Declared here |
| [`_lastLevel`](#_lastlevel) | `variable` | Declared here |
| [`_lastPeak`](#_lastpeak) | `variable` | Declared here |
| [`measure`](#measure) | `function` | Declared here |
| [`emitEvent`](#emitevent) | `function` | Declared here |
| [`sanitize`](#sanitize) | `function` | Declared here |

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`level`](#level-2) | `variable` | Declared here |
| [`peak`](#peak-1) | `variable` | Declared here |
| [`valid`](#valid-9) | `variable` | Declared here |

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`sequence`](#sequence) | `variable` | Declared here |
| [`timeUsec`](#timeusec) | `variable` | Declared here |
| [`durationUsec`](#durationusec) | `variable` | Declared here |
| [`sampleRate`](#samplerate-2) | `variable` | Declared here |
| [`channels`](#channels-3) | `variable` | Declared here |
| [`numSamples`](#numsamples-1) | `variable` | Declared here |
| [`sampleFmt`](#samplefmt-2) | `variable` | Declared here |

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`type`](#type-18) | `variable` | Declared here |
| [`source`](#source) | `variable` | Declared here |
| [`detector`](#detector) | `variable` | Declared here |
| [`emittedAtUsec`](#emittedatusec) | `variable` | Declared here |
| [`audio`](#audio-3) | `variable` | Declared here |
| [`active`](#active-1) | `variable` | Declared here |
| [`level`](#level-1) | `variable` | Declared here |
| [`peak`](#peak) | `variable` | Declared here |
| [`data`](#data-3) | `variable` | Declared here |

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`source`](#source-1) | `variable` | Declared here |
| [`detectorName`](#detectorname) | `variable` | Declared here |
| [`sampleRateHint`](#sampleratehint) | `variable` | Declared here |
| [`startThreshold`](#startthreshold) | `variable` | Declared here |
| [`stopThreshold`](#stopthreshold) | `variable` | Declared here |
| [`minSilenceUsec`](#minsilenceusec) | `variable` | Declared here |
| [`updateIntervalUsec`](#updateintervalusec) | `variable` | Declared here |

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

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`seen`](#seen) | `variable` | Declared here |
| [`emitted`](#emitted) | `variable` | Declared here |
| [`active`](#active-2) | `variable` | Declared here |
| [`lastLevel`](#lastlevel) | `variable` | Declared here |
| [`lastPeak`](#lastpeak) | `variable` | Declared here |

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

