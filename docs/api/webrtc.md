{#webrtcmodule}

# webrtc

WebRTC media transport via libdatachannel; peer sessions, media bridge, codec negotiation.

### Classes

| Name | Description |
|------|-------------|
| [`SignallingInterface`](#signallinginterface) | Transport-agnostic signalling interface for WebRTC session setup. |
| [`MediaBridge`](#mediabridge) | Convenience wrapper that creates WebRTC tracks on a PeerConnection and exposes per-track sender/receiver adapters for [PacketStream](base.md#packetstream) integration. |
| [`PeerSession`](#peersession) | Manages a WebRTC peer connection lifecycle over any signalling transport that implements [SignallingInterface](#signallinginterface). |
| [`WebRtcTrackSender`](#webrtctracksender) | [PacketProcessor](base.md#packetprocessor) that sends encoded media to a single libdatachannel Track via sendFrame(). |
| [`WebRtcTrackReceiver`](#webrtctrackreceiver) | [PacketStreamAdapter](base.md#packetstreamadapter) that receives depacketized frames from a single remote libdatachannel Track and emits them as VideoPacket or AudioPacket into a [PacketStream](base.md#packetstream). |
| [`CodecNegotiator`](#codecnegotiator) | Maps RTP codec names to FFmpeg encoders and queries FFmpeg at runtime to determine what codecs are available. |
| [`TrackHandle`](#trackhandle) | Result of creating a track: the track itself plus its RTP config. Keep the config around - you need it for [WebRtcTrackSender](#webrtctracksender). |
| [`JitterBufferConfig`](#jitterbufferconfig) | Receive-side jitter buffer behaviour for depacketized WebRTC media frames. |
| [`CodecSpec`](#codecspec) | Canonical description of a codec supported by icey's WebRTC helpers. |
| [`NegotiatedCodec`](#negotiatedcodec) | Result of codec negotiation between a remote SDP offer and the local FFmpeg codec inventory. |

### Enumerations

| Name | Description |
|------|-------------|
| [`CodecMediaType`](#codecmediatype)  | Whether a codec carries audio or video media. |
| [`CodecId`](#codecid)  | Stable codec identifiers used across negotiation and track setup. |

---

{#codecmediatype}

#### CodecMediaType

```cpp
enum CodecMediaType
```

Whether a codec carries audio or video media.

| Value | Description |
|-------|-------------|
| `Video` | Video RTP payload. |
| `Audio` | Audio RTP payload. |

---

{#codecid}

#### CodecId

```cpp
enum CodecId
```

Stable codec identifiers used across negotiation and track setup.

| Value | Description |
|-------|-------------|
| `Unknown` | Unrecognized or unsupported codec. |
| `H264` | H.264 / AVC. |
| `H265` | H.265 / HEVC. |
| `VP8` | Google VP8. |
| `VP9` | Google VP9. |
| `AV1` | AOMedia AV1. |
| `Opus` | Opus audio. |
| `PCMU` | G.711 mu-law audio. |
| `PCMA` | G.711 A-law audio. |
| `G722` | G.722 wideband audio. |
| `AAC` | AAC audio. |

### Functions

| Return | Name | Description |
|--------|------|-------------|
| `TrackHandle` | [`createVideoTrack`](#createvideotrack) `nodiscard` | Create a video send track on a PeerConnection. |
| `TrackHandle` | [`createAudioTrack`](#createaudiotrack) `nodiscard` | Create an audio send track on a PeerConnection. |
| `std::shared_ptr< rtc::Track >` | [`createVideoReceiveTrack`](#createvideoreceivetrack) `nodiscard` | Create a pure receive-side video track on a PeerConnection. |
| `std::shared_ptr< rtc::Track >` | [`createAudioReceiveTrack`](#createaudioreceivetrack) `nodiscard` | Create a pure receive-side audio track on a PeerConnection. |
| `bool` | [`setupReceiveTrack`](#setupreceivetrack) `nodiscard` | Set up the receive-side media handler chain on a remote track. |
| `uint32_t` | [`generateSsrc`](#generatessrc) `nodiscard` | Generate a random SSRC. |
| `const char *` | [`stateToString`](#statetostring) `nodiscard` | Convert a [PeerSession::State](#state-4) to a lowercase C string for logging. |

---

{#createvideotrack}

#### createVideoTrack

`nodiscard`

```cpp
[[nodiscard]] TrackHandle createVideoTrack(std::shared_ptr< rtc::PeerConnection > pc, const av::VideoCodec & codec, uint32_t ssrc = 0, const std::string & cname = {}, const std::string & mid = {}, rtc::Description::Direction direction = rtc::Description::Direction::SendRecv, unsigned nackBuffer = 512, std::function< void()> onPli = nullptr, std::function< void(unsigned int)> onRemb = nullptr, int payloadType = -1)
```

Create a video send track on a PeerConnection.

Sets up the full outgoing media handler chain: Packetizer → SrReporter → NackResponder → PliHandler → RembHandler

The packetizer is selected based on the codec: H264 → H264RtpPacketizer (Annex-B long start sequence) H265 → H265RtpPacketizer (Annex-B long start sequence) VP8/VP9/other → generic RtpPacketizer

#### Parameters
* `pc` PeerConnection to add the track to. 

* `codec` Video codec. Must name a supported RTP codec or FFmpeg encoder (e.g. "H264" or "libx264"). 

* `ssrc` RTP SSRC. 0 = auto-generate. 

* `cname` RTCP CNAME. Empty = "icey". 

* `mid` MID to use for the track when answering an existing offer. 

* `direction` Direction to advertise for the negotiated m-line. 

* `nackBuffer` Max packets stored for NACK retransmission. 

* `onPli` Callback when remote peer requests a keyframe. Connect to your encoder to force IDR. 

* `onRemb` Callback when remote peer reports bandwidth estimate. Bitrate in bits/sec. 

* `payloadType` Explicit RTP payload type to reuse when answering an offer. -1 = use the codec's default/preferred type. 

#### Returns
[TrackHandle](#trackhandle) with the track and its RTP config.

---

{#createaudiotrack}

#### createAudioTrack

`nodiscard`

```cpp
[[nodiscard]] TrackHandle createAudioTrack(std::shared_ptr< rtc::PeerConnection > pc, const av::AudioCodec & codec, uint32_t ssrc = 0, const std::string & cname = {}, const std::string & mid = {}, rtc::Description::Direction direction = rtc::Description::Direction::SendRecv, int payloadType = -1)
```

Create an audio send track on a PeerConnection.

Sets up the outgoing media handler chain: AudioRtpPacketizer → SrReporter

The packetizer clock rate is selected based on codec: opus → 48kHz, PCMU/PCMA → 8kHz, etc.

#### Parameters
* `pc` PeerConnection to add the track to. 

* `codec` Audio codec. Must name a supported RTP codec or FFmpeg encoder (e.g. "opus" or "libopus"). 

* `ssrc` RTP SSRC. 0 = auto-generate. 

* `cname` RTCP CNAME. Empty = "icey". 

* `mid` MID to use for the track when answering an existing offer. 

* `direction` Direction to advertise for the negotiated m-line. 

* `payloadType` Explicit RTP payload type to reuse when answering an offer. -1 = use the codec's default/preferred type. 

#### Returns
[TrackHandle](#trackhandle) with the track and its RTP config.

---

{#createvideoreceivetrack}

#### createVideoReceiveTrack

`nodiscard`

```cpp
[[nodiscard]] std::shared_ptr< rtc::Track > createVideoReceiveTrack(std::shared_ptr< rtc::PeerConnection > pc, const av::VideoCodec & codec, const std::string & mid = {}, rtc::Description::Direction direction = rtc::Description::Direction::RecvOnly, int payloadType = -1)
```

Create a pure receive-side video track on a PeerConnection.

Unlike [createVideoTrack()](#createvideotrack), this does not add a local SSRC or sender packetizer chain. It is for answers that only receive remote media.

---

{#createaudioreceivetrack}

#### createAudioReceiveTrack

`nodiscard`

```cpp
[[nodiscard]] std::shared_ptr< rtc::Track > createAudioReceiveTrack(std::shared_ptr< rtc::PeerConnection > pc, const av::AudioCodec & codec, const std::string & mid = {}, rtc::Description::Direction direction = rtc::Description::Direction::RecvOnly, int payloadType = -1)
```

Create a pure receive-side audio track on a PeerConnection.

Unlike [createAudioTrack()](#createaudiotrack), this does not add a local SSRC or sender packetizer chain. It is for answers that only receive remote media.

---

{#setupreceivetrack}

#### setupReceiveTrack

`nodiscard`

```cpp
[[nodiscard]] bool setupReceiveTrack(std::shared_ptr< rtc::Track > track)
```

Set up the receive-side media handler chain on a remote track.

Selects the correct depacketizer based on the track's SDP codec: Video: H264RtpDepacketizer, H265RtpDepacketizer, or generic Audio: OpusRtpDepacketizer, PCMURtpDepacketizer, etc.

Also chains an RtcpReceivingSession for receiver reports.

Call this from PeerConnection::onTrack() before binding a [WebRtcTrackReceiver](#webrtctrackreceiver).

#### Parameters
* `track` The remote track from onTrack callback. 

#### Returns
True when a supported depacketizer was installed.

---

{#generatessrc}

#### generateSsrc

`nodiscard`

```cpp
[[nodiscard]] uint32_t generateSsrc()
```

Generate a random SSRC.

---

{#statetostring}

#### stateToString

`nodiscard`

```cpp
[[nodiscard]] const char * stateToString(PeerSession::State state)
```

Convert a [PeerSession::State](#state-4) to a lowercase C string for logging. 
#### Parameters
* `state` [State](base.md#state) value to convert. 

#### Returns
One of: "idle", "outgoing-init", "incoming-init", "negotiating", "active", "ending", "ended".

{#signallinginterface}

## SignallingInterface

```cpp
#include <icy/webrtc/signalling.h>
```

```cpp
class SignallingInterface
```

Defined in src/webrtc/include/icy/webrtc/signalling.h:38

> **Subclassed by:** [`SympleServerSignaller`](webrtcsupport.md#sympleserversignaller), [`SympleSignaller`](webrtcsupport.md#symplesignaller), [`WebSocketSignaller`](webrtcsupport.md#websocketsignaller)

Transport-agnostic signalling interface for WebRTC session setup.

Implementations handle the exchange of SDP offers/answers and ICE candidates between peers over whatever transport is available: Symple, plain WebSocket, REST, MQTT, carrier pigeon, etc.

[PeerSession](#peersession) takes a reference to this interface. Implement it to plug in your own signalling backend.

The three message categories:

* SDP: offer/answer exchange (the session description)

* Candidate: trickle ICE candidates

* Control: call lifecycle (init, accept, reject, hangup)

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `ThreadSignal< void(const std::string &, const std::string &, const std::string &)>` | [`SdpReceived`](#sdpreceived)  | Fires when an SDP offer or answer arrives from a remote peer. Parameters: peerId, type ("offer"/"answer"), sdp. |
| `ThreadSignal< void(const std::string &, const std::string &, const std::string &)>` | [`CandidateReceived`](#candidatereceived)  | Fires when an ICE candidate arrives from a remote peer. Parameters: peerId, candidate, mid. |
| `ThreadSignal< void(const std::string &, const std::string &, const std::string &)>` | [`ControlReceived`](#controlreceived)  | Fires when a control message arrives from a remote peer. Parameters: peerId, type ("init"/"accept"/"reject"/"hangup"), reason. |

---

{#sdpreceived}

#### SdpReceived

```cpp
ThreadSignal< void(const std::string &, const std::string &, const std::string &)> SdpReceived
```

Defined in src/webrtc/include/icy/webrtc/signalling.h:77

Fires when an SDP offer or answer arrives from a remote peer. Parameters: peerId, type ("offer"/"answer"), sdp.

---

{#candidatereceived}

#### CandidateReceived

```cpp
ThreadSignal< void(const std::string &, const std::string &, const std::string &)> CandidateReceived
```

Defined in src/webrtc/include/icy/webrtc/signalling.h:81

Fires when an ICE candidate arrives from a remote peer. Parameters: peerId, candidate, mid.

---

{#controlreceived}

#### ControlReceived

```cpp
ThreadSignal< void(const std::string &, const std::string &, const std::string &)> ControlReceived
```

Defined in src/webrtc/include/icy/webrtc/signalling.h:85

Fires when a control message arrives from a remote peer. Parameters: peerId, type ("init"/"accept"/"reject"/"hangup"), reason.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`sendSdp`](#sendsdp) `virtual` | Send an SDP offer or answer to the remote peer. |
| `void` | [`sendCandidate`](#sendcandidate) `virtual` | Send an ICE candidate to the remote peer. |
| `void` | [`sendControl`](#sendcontrol) `virtual` | Send a control message to the remote peer. |

---

{#sendsdp}

#### sendSdp

`virtual`

```cpp
virtual void sendSdp(const std::string & peerId, const std::string & type, const std::string & sdp)
```

Defined in src/webrtc/include/icy/webrtc/signalling.h:51

Send an SDP offer or answer to the remote peer. 
#### Parameters
* `peerId` Remote peer identifier. 

* `type` "offer" or "answer". 

* `sdp` The SDP string.

---

{#sendcandidate}

#### sendCandidate

`virtual`

```cpp
virtual void sendCandidate(const std::string & peerId, const std::string & candidate, const std::string & mid)
```

Defined in src/webrtc/include/icy/webrtc/signalling.h:59

Send an ICE candidate to the remote peer. 
#### Parameters
* `peerId` Remote peer identifier. 

* `candidate` The candidate string (from RTCIceCandidate). 

* `mid` The sdpMid value.

---

{#sendcontrol}

#### sendControl

`virtual`

```cpp
virtual void sendControl(const std::string & peerId, const std::string & type, const std::string & reason = {})
```

Defined in src/webrtc/include/icy/webrtc/signalling.h:67

Send a control message to the remote peer. 
#### Parameters
* `peerId` Remote peer identifier. 

* `type` Control type: "init", "accept", "reject", "hangup". 

* `reason` Optional reason string (for reject/hangup).

{#mediabridge}

## MediaBridge

```cpp
#include <icy/webrtc/mediabridge.h>
```

```cpp
class MediaBridge
```

Defined in src/webrtc/include/icy/webrtc/mediabridge.h:69

Convenience wrapper that creates WebRTC tracks on a PeerConnection and exposes per-track sender/receiver adapters for [PacketStream](base.md#packetstream) integration.

For video-only, leave audioCodec default (disabled). For audio-only, leave videoCodec default (disabled). For data-channel-only, don't create a [MediaBridge](#mediabridge) at all.

For full control over individual tracks, use [createVideoTrack()](#createvideotrack), [createAudioTrack()](#createaudiotrack), [WebRtcTrackSender](#webrtctracksender), and [WebRtcTrackReceiver](#webrtctrackreceiver) directly instead of this class.

The sender and receiver adapter objects are stable for the lifetime of the [MediaBridge](#mediabridge). [attach()](#attach) and [detach()](#detach) rebind or unbind the underlying WebRTC tracks without invalidating references previously obtained from [videoSender()](#videosender), [audioSender()](#audiosender), [videoReceiver()](#videoreceiver), or [audioReceiver()](#audioreceiver).

Example - send camera to browser:

[MediaBridge](#mediabridge) bridge; bridge.attach(pc, {.videoCodec = {"H264", "libx264", 1280, 720, 30}});

[PacketStream](base.md#packetstream) stream; stream.attachSource(capture); stream.attach(encoder, 1, true); stream.attach(&bridge.videoSender(), 5, false); stream.start();

Example - receive from browser and record:

bridge.videoReceiver().emitter += packetSlot(&recorder, &Recorder::onEncodedVideo);

The receiver emits owning encoded packets. Feed those into a decoder or recorder callback; see `samples/media-recorder` for a complete example.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `ThreadSignal< void()>` | [`KeyframeRequested`](#keyframerequested)  | Remote peer requests a keyframe. Connect to encoder to force IDR. |
| `ThreadSignal< void(unsigned int)>` | [`BitrateEstimate`](#bitrateestimate)  | Remote peer reports estimated bandwidth (bits/sec). |

---

{#keyframerequested}

#### KeyframeRequested

```cpp
ThreadSignal< void()> KeyframeRequested
```

Defined in src/webrtc/include/icy/webrtc/mediabridge.h:149

Remote peer requests a keyframe. Connect to encoder to force IDR.

---

{#bitrateestimate}

#### BitrateEstimate

```cpp
ThreadSignal< void(unsigned int)> BitrateEstimate
```

Defined in src/webrtc/include/icy/webrtc/mediabridge.h:152

Remote peer reports estimated bandwidth (bits/sec).

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`MediaBridge`](#mediabridge-1)  | Construct a detached bridge with stable sender and receiver adapters. |
|  | [`~MediaBridge`](#mediabridge-2)  | Destroy the bridge and release any attached PeerConnection state. |
|  | [`MediaBridge`](#mediabridge-3)  | Deleted copy constructor; [MediaBridge](#mediabridge) owns live track and adapter state. |
| `void` | [`attach`](#attach)  | Create tracks on the PeerConnection and set up handler chains. Only creates tracks for codecs with a non-empty encoder name. |
| `void` | [`detach`](#detach)  | Detach all tracks and adapters. |
| `void` | [`requestKeyframe`](#requestkeyframe)  | Request an immediate keyframe (IDR) from the remote sender. Sends a PLI (Picture Loss Indication) RTCP message on the video track. No-op if no video track is attached. |
| `void` | [`requestBitrate`](#requestbitrate)  | Request that the remote sender reduce to a target bitrate. Sends a TMMBR RTCP message on the video track. |
| `WebRtcTrackSender &` | [`videoSender`](#videosender) `nodiscard` | Video send processor. Attach to a [PacketStream](base.md#packetstream) after a VideoEncoder. Throws if no video track was created. |
| `WebRtcTrackSender &` | [`audioSender`](#audiosender) `nodiscard` | Audio send processor. Attach to a [PacketStream](base.md#packetstream) after an AudioEncoder. Throws if no audio track was created. |
| `WebRtcTrackReceiver &` | [`videoReceiver`](#videoreceiver) `nodiscard` | Video receive adapter. Attach as a [PacketStream](base.md#packetstream) source. Only valid after a remote video track arrives. |
| `WebRtcTrackReceiver &` | [`audioReceiver`](#audioreceiver) `nodiscard` | Audio receive adapter. Attach as a [PacketStream](base.md#packetstream) source. Only valid after a remote audio track arrives. |
| `std::shared_ptr< rtc::Track >` | [`videoTrack`](#videotrack) `const` `nodiscard` | The underlying libdatachannel video track, or nullptr if none was created. |
| `std::shared_ptr< rtc::Track >` | [`audioTrack`](#audiotrack) `const` `nodiscard` | The underlying libdatachannel audio track, or nullptr if none was created. |
| `bool` | [`hasVideo`](#hasvideo) `const` `nodiscard` | True if a video track was created at [attach()](#attach). |
| `bool` | [`hasAudio`](#hasaudio) `const` `nodiscard` | True if an audio track was created at [attach()](#attach). |
| `bool` | [`attached`](#attached) `const` `nodiscard` | True if [attach()](#attach) has been called and a PeerConnection is held. |

---

{#mediabridge-1}

#### MediaBridge

```cpp
MediaBridge()
```

Defined in src/webrtc/include/icy/webrtc/mediabridge.h:98

Construct a detached bridge with stable sender and receiver adapters.

---

{#mediabridge-2}

#### ~MediaBridge

```cpp
~MediaBridge()
```

Defined in src/webrtc/include/icy/webrtc/mediabridge.h:100

Destroy the bridge and release any attached PeerConnection state.

---

{#mediabridge-3}

#### MediaBridge

```cpp
MediaBridge(const MediaBridge &) = delete
```

Defined in src/webrtc/include/icy/webrtc/mediabridge.h:103

Deleted copy constructor; [MediaBridge](#mediabridge) owns live track and adapter state.

---

{#attach}

#### attach

```cpp
void attach(std::shared_ptr< rtc::PeerConnection > pc, const Options & opts)
```

Defined in src/webrtc/include/icy/webrtc/mediabridge.h:108

Create tracks on the PeerConnection and set up handler chains. Only creates tracks for codecs with a non-empty encoder name.

---

{#detach}

#### detach

```cpp
void detach()
```

Defined in src/webrtc/include/icy/webrtc/mediabridge.h:112

Detach all tracks and adapters.

---

{#requestkeyframe}

#### requestKeyframe

```cpp
void requestKeyframe()
```

Defined in src/webrtc/include/icy/webrtc/mediabridge.h:117

Request an immediate keyframe (IDR) from the remote sender. Sends a PLI (Picture Loss Indication) RTCP message on the video track. No-op if no video track is attached.

---

{#requestbitrate}

#### requestBitrate

```cpp
void requestBitrate(unsigned int bitrate)
```

Defined in src/webrtc/include/icy/webrtc/mediabridge.h:122

Request that the remote sender reduce to a target bitrate. Sends a TMMBR RTCP message on the video track. 
#### Parameters
* `bitrate` Target bitrate in bits per second.

---

{#videosender}

#### videoSender

`nodiscard`

```cpp
[[nodiscard]] WebRtcTrackSender & videoSender()
```

Defined in src/webrtc/include/icy/webrtc/mediabridge.h:130

Video send processor. Attach to a [PacketStream](base.md#packetstream) after a VideoEncoder. Throws if no video track was created.

---

{#audiosender}

#### audioSender

`nodiscard`

```cpp
[[nodiscard]] WebRtcTrackSender & audioSender()
```

Defined in src/webrtc/include/icy/webrtc/mediabridge.h:134

Audio send processor. Attach to a [PacketStream](base.md#packetstream) after an AudioEncoder. Throws if no audio track was created.

---

{#videoreceiver}

#### videoReceiver

`nodiscard`

```cpp
[[nodiscard]] WebRtcTrackReceiver & videoReceiver()
```

Defined in src/webrtc/include/icy/webrtc/mediabridge.h:138

Video receive adapter. Attach as a [PacketStream](base.md#packetstream) source. Only valid after a remote video track arrives.

---

{#audioreceiver}

#### audioReceiver

`nodiscard`

```cpp
[[nodiscard]] WebRtcTrackReceiver & audioReceiver()
```

Defined in src/webrtc/include/icy/webrtc/mediabridge.h:142

Audio receive adapter. Attach as a [PacketStream](base.md#packetstream) source. Only valid after a remote audio track arrives.

---

{#videotrack}

#### videoTrack

`const` `nodiscard`

```cpp
[[nodiscard]] std::shared_ptr< rtc::Track > videoTrack() const
```

Defined in src/webrtc/include/icy/webrtc/mediabridge.h:159

The underlying libdatachannel video track, or nullptr if none was created.

---

{#audiotrack}

#### audioTrack

`const` `nodiscard`

```cpp
[[nodiscard]] std::shared_ptr< rtc::Track > audioTrack() const
```

Defined in src/webrtc/include/icy/webrtc/mediabridge.h:162

The underlying libdatachannel audio track, or nullptr if none was created.

---

{#hasvideo}

#### hasVideo

`const` `nodiscard`

```cpp
[[nodiscard]] bool hasVideo() const
```

Defined in src/webrtc/include/icy/webrtc/mediabridge.h:165

True if a video track was created at [attach()](#attach).

---

{#hasaudio}

#### hasAudio

`const` `nodiscard`

```cpp
[[nodiscard]] bool hasAudio() const
```

Defined in src/webrtc/include/icy/webrtc/mediabridge.h:168

True if an audio track was created at [attach()](#attach).

---

{#attached}

#### attached

`const` `nodiscard`

```cpp
[[nodiscard]] bool attached() const
```

Defined in src/webrtc/include/icy/webrtc/mediabridge.h:171

True if [attach()](#attach) has been called and a PeerConnection is held.

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::shared_ptr< rtc::PeerConnection >` | [`_pc`](#_pc)  |  |
| `WebRtcTrackSender` | [`_videoSender`](#_videosender)  |  |
| `WebRtcTrackSender` | [`_audioSender`](#_audiosender)  |  |
| `TrackHandle` | [`_videoHandle`](#_videohandle)  |  |
| `TrackHandle` | [`_audioHandle`](#_audiohandle)  |  |
| `WebRtcTrackReceiver` | [`_videoReceiver`](#_videoreceiver)  |  |
| `WebRtcTrackReceiver` | [`_audioReceiver`](#_audioreceiver)  |  |
| `std::shared_ptr< rtc::Track >` | [`_videoReceiveTrack`](#_videoreceivetrack)  |  |
| `std::shared_ptr< rtc::Track >` | [`_audioReceiveTrack`](#_audioreceivetrack)  |  |
| `std::mutex` | [`_mutex`](#_mutex-17)  |  |

---

{#_pc}

#### _pc

```cpp
std::shared_ptr< rtc::PeerConnection > _pc
```

Defined in src/webrtc/include/icy/webrtc/mediabridge.h:174

---

{#_videosender}

#### _videoSender

```cpp
WebRtcTrackSender _videoSender
```

Defined in src/webrtc/include/icy/webrtc/mediabridge.h:178

---

{#_audiosender}

#### _audioSender

```cpp
WebRtcTrackSender _audioSender
```

Defined in src/webrtc/include/icy/webrtc/mediabridge.h:179

---

{#_videohandle}

#### _videoHandle

```cpp
TrackHandle _videoHandle
```

Defined in src/webrtc/include/icy/webrtc/mediabridge.h:180

---

{#_audiohandle}

#### _audioHandle

```cpp
TrackHandle _audioHandle
```

Defined in src/webrtc/include/icy/webrtc/mediabridge.h:181

---

{#_videoreceiver}

#### _videoReceiver

```cpp
WebRtcTrackReceiver _videoReceiver
```

Defined in src/webrtc/include/icy/webrtc/mediabridge.h:184

---

{#_audioreceiver}

#### _audioReceiver

```cpp
WebRtcTrackReceiver _audioReceiver
```

Defined in src/webrtc/include/icy/webrtc/mediabridge.h:185

---

{#_videoreceivetrack}

#### _videoReceiveTrack

```cpp
std::shared_ptr< rtc::Track > _videoReceiveTrack
```

Defined in src/webrtc/include/icy/webrtc/mediabridge.h:186

---

{#_audioreceivetrack}

#### _audioReceiveTrack

```cpp
std::shared_ptr< rtc::Track > _audioReceiveTrack
```

Defined in src/webrtc/include/icy/webrtc/mediabridge.h:187

---

{#_mutex-17}

#### _mutex

```cpp
std::mutex _mutex
```

Defined in src/webrtc/include/icy/webrtc/mediabridge.h:189

{#options-17}

## Options

```cpp
#include <icy/webrtc/mediabridge.h>
```

```cpp
struct Options
```

Defined in src/webrtc/include/icy/webrtc/mediabridge.h:73

[Configuration](base.md#configuration) options for the WebRTC media bridge.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `av::VideoCodec` | [`videoCodec`](#videocodec-6)  | Video codec for the send track. Leave both name and encoder empty to skip creating a video track. |
| `av::AudioCodec` | [`audioCodec`](#audiocodec-5)  | Audio codec for the send track. Leave both name and encoder empty to skip creating an audio track. |
| `uint32_t` | [`videoSsrc`](#videossrc)  | 0 = auto-generate |
| `uint32_t` | [`audioSsrc`](#audiossrc)  | 0 = auto-generate |
| `int` | [`videoPayloadType`](#videopayloadtype)  | Reuse negotiated offer payload type when answering, -1 = default. |
| `int` | [`audioPayloadType`](#audiopayloadtype)  | Reuse negotiated offer payload type when answering, -1 = default. |
| `std::string` | [`cname`](#cname)  | CNAME for RTCP (auto if empty). |
| `std::string` | [`videoMid`](#videomid)  | Explicit MID for the negotiated video m-line when answering an offer. |
| `std::string` | [`audioMid`](#audiomid)  | Explicit MID for the negotiated audio m-line when answering an offer. |
| `rtc::Description::Direction` | [`videoDirection`](#videodirection)  |  |
| `rtc::Description::Direction` | [`audioDirection`](#audiodirection)  |  |
| `unsigned` | [`nackBufferSize`](#nackbuffersize)  | Max RTP packets retained for video NACK retransmission. |
| `JitterBufferConfig` | [`videoJitterBuffer`](#videojitterbuffer)  | Receive-side playout buffering for depacketized video frames. |
| `JitterBufferConfig` | [`audioJitterBuffer`](#audiojitterbuffer)  | Receive-side playout buffering for depacketized audio frames. |

---

{#videocodec-6}

#### videoCodec

```cpp
av::VideoCodec videoCodec
```

Defined in src/webrtc/include/icy/webrtc/mediabridge.h:77

Video codec for the send track. Leave both name and encoder empty to skip creating a video track.

---

{#audiocodec-5}

#### audioCodec

```cpp
av::AudioCodec audioCodec
```

Defined in src/webrtc/include/icy/webrtc/mediabridge.h:81

Audio codec for the send track. Leave both name and encoder empty to skip creating an audio track.

---

{#videossrc}

#### videoSsrc

```cpp
uint32_t videoSsrc = 0
```

Defined in src/webrtc/include/icy/webrtc/mediabridge.h:83

0 = auto-generate

---

{#audiossrc}

#### audioSsrc

```cpp
uint32_t audioSsrc = 0
```

Defined in src/webrtc/include/icy/webrtc/mediabridge.h:84

0 = auto-generate

---

{#videopayloadtype}

#### videoPayloadType

```cpp
int videoPayloadType = -1
```

Defined in src/webrtc/include/icy/webrtc/mediabridge.h:85

Reuse negotiated offer payload type when answering, -1 = default.

---

{#audiopayloadtype}

#### audioPayloadType

```cpp
int audioPayloadType = -1
```

Defined in src/webrtc/include/icy/webrtc/mediabridge.h:86

Reuse negotiated offer payload type when answering, -1 = default.

---

{#cname}

#### cname

```cpp
std::string cname
```

Defined in src/webrtc/include/icy/webrtc/mediabridge.h:87

CNAME for RTCP (auto if empty).

---

{#videomid}

#### videoMid

```cpp
std::string videoMid
```

Defined in src/webrtc/include/icy/webrtc/mediabridge.h:88

Explicit MID for the negotiated video m-line when answering an offer.

---

{#audiomid}

#### audioMid

```cpp
std::string audioMid
```

Defined in src/webrtc/include/icy/webrtc/mediabridge.h:89

Explicit MID for the negotiated audio m-line when answering an offer.

---

{#videodirection}

#### videoDirection

```cpp
rtc::Description::Direction videoDirection = rtc::Description::Direction::SendRecv
```

Defined in src/webrtc/include/icy/webrtc/mediabridge.h:90

---

{#audiodirection}

#### audioDirection

```cpp
rtc::Description::Direction audioDirection = rtc::Description::Direction::SendRecv
```

Defined in src/webrtc/include/icy/webrtc/mediabridge.h:91

---

{#nackbuffersize}

#### nackBufferSize

```cpp
unsigned nackBufferSize = 512
```

Defined in src/webrtc/include/icy/webrtc/mediabridge.h:92

Max RTP packets retained for video NACK retransmission.

---

{#videojitterbuffer}

#### videoJitterBuffer

```cpp
JitterBufferConfig videoJitterBuffer
```

Defined in src/webrtc/include/icy/webrtc/mediabridge.h:93

Receive-side playout buffering for depacketized video frames.

---

{#audiojitterbuffer}

#### audioJitterBuffer

```cpp
JitterBufferConfig audioJitterBuffer
```

Defined in src/webrtc/include/icy/webrtc/mediabridge.h:94

Receive-side playout buffering for depacketized audio frames.

{#peersession}

## PeerSession

```cpp
#include <icy/webrtc/peersession.h>
```

```cpp
class PeerSession
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:45

Manages a WebRTC peer connection lifecycle over any signalling transport that implements [SignallingInterface](#signallinginterface).

Works with [SympleSignaller](webrtcsupport.md#symplesignaller) (Symple call protocol), [WebSocketSignaller](webrtcsupport.md#websocketsignaller) (plain JSON over WSS), or any custom implementation.

Media is optional. Set `[Config::media](#media-2)` codecs to enable tracks. Leave codec encoders empty for data-channel-only sessions.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `ThreadSignal< void(State)>` | [`StateChanged`](#statechanged)  | Emitted whenever the session state changes. Parameter: new [State](base.md#state) value. |
| `ThreadSignal< void(const std::string &)>` | [`IncomingCall`](#incomingcall)  | Emitted when a remote peer initiates a call (state transitions to IncomingInit). Parameter: remote peer identifier. |
| `ThreadSignal< void(rtc::message_variant)>` | [`DataReceived`](#datareceived)  | Emitted when a message arrives on the data channel. Parameter: rtc::message_variant (string or binary). |

---

{#statechanged}

#### StateChanged

```cpp
ThreadSignal< void(State)> StateChanged
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:125

Emitted whenever the session state changes. Parameter: new [State](base.md#state) value.

---

{#incomingcall}

#### IncomingCall

```cpp
ThreadSignal< void(const std::string &)> IncomingCall
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:129

Emitted when a remote peer initiates a call (state transitions to IncomingInit). Parameter: remote peer identifier.

---

{#datareceived}

#### DataReceived

```cpp
ThreadSignal< void(rtc::message_variant)> DataReceived
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:133

Emitted when a message arrives on the data channel. Parameter: rtc::message_variant (string or binary).

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`PeerSession`](#peersession-1)  | Construct with any signalling implementation. The signaller must outlive this [PeerSession](#peersession). |
|  | [`PeerSession`](#peersession-2)  | Deleted copy constructor; [PeerSession](#peersession) owns live signalling and RTC callbacks. |
| `void` | [`call`](#call)  | Initiate an outgoing call to a remote peer. Sends a "init" control message and transitions to OutgoingInit. |
| `void` | [`accept`](#accept)  | Accept an incoming call. Creates the PeerConnection, sends "accept", and transitions to Negotiating. |
| `void` | [`reject`](#reject)  | Reject an incoming call. Sends a "reject" control message and transitions to Ended. |
| `void` | [`hangup`](#hangup)  | Terminate any non-idle call phase. Sends a "hangup" control message, closes the PeerConnection, and transitions to Ended. Safe to call from any non-Idle/Ended state. |
| `void` | [`sendData`](#senddata-2)  | Send a UTF-8 string message over the data channel. Silently dropped if the data channel is not open. |
| `void` | [`sendData`](#senddata-3)  | Send raw binary data over the data channel. Silently dropped if the data channel is not open. |
| `State` | [`state`](#state-3) `const` `nodiscard` | Current session state. Thread-safe. |
| `std::string` | [`remotePeerId`](#remotepeerid) `const` `nodiscard` | Identifier of the remote peer for the current or most recent call. Empty when Idle. |
| `MediaBridge &` | [`media`](#media) `nodiscard` | Media bridge for this session. Valid for the lifetime of the [PeerSession](#peersession). |
| `const MediaBridge &` | [`media`](#media-1) `const` `nodiscard` | Media bridge for this session. Valid for the lifetime of the [PeerSession](#peersession). |
| `std::shared_ptr< rtc::PeerConnection >` | [`peerConnection`](#peerconnection) `nodiscard` | The underlying PeerConnection, or nullptr when Idle/Ended. |
| `std::shared_ptr< rtc::DataChannel >` | [`dataChannel`](#datachannel) `nodiscard` | The data channel, or nullptr if none is open. |

---

{#peersession-1}

#### PeerSession

```cpp
PeerSession(SignallingInterface & signaller, const Config & config)
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:82

Construct with any signalling implementation. The signaller must outlive this [PeerSession](#peersession).

---

{#peersession-2}

#### PeerSession

```cpp
PeerSession(const PeerSession &) = delete
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:86

Deleted copy constructor; [PeerSession](#peersession) owns live signalling and RTC callbacks.

---

{#call}

#### call

```cpp
void call(const std::string & peerId)
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:93

Initiate an outgoing call to a remote peer. Sends a "init" control message and transitions to OutgoingInit. 
#### Parameters
* `peerId` Remote peer identifier passed to the signaller. 

#### Exceptions
* `std::logic_error` if not currently in the Idle state.

---

{#accept}

#### accept

```cpp
void accept()
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:98

Accept an incoming call. Creates the PeerConnection, sends "accept", and transitions to Negotiating. 
#### Exceptions
* `std::logic_error` if not currently in the IncomingInit state.

---

{#reject}

#### reject

```cpp
void reject(const std::string & reason = "declined")
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:104

Reject an incoming call. Sends a "reject" control message and transitions to Ended. 
#### Parameters
* `reason` Human-readable reason string forwarded to the remote peer. 

#### Exceptions
* `std::logic_error` if not currently in the IncomingInit state.

---

{#hangup}

#### hangup

```cpp
void hangup(const std::string & reason = "hangup")
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:110

Terminate any non-idle call phase. Sends a "hangup" control message, closes the PeerConnection, and transitions to Ended. Safe to call from any non-Idle/Ended state. 
#### Parameters
* `reason` Human-readable reason string forwarded to the remote peer.

---

{#senddata-2}

#### sendData

```cpp
void sendData(const std::string & message)
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:115

Send a UTF-8 string message over the data channel. Silently dropped if the data channel is not open. 
#### Parameters
* `message` String to send.

---

{#senddata-3}

#### sendData

```cpp
void sendData(const std::byte * data, size_t size)
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:121

Send raw binary data over the data channel. Silently dropped if the data channel is not open. 
#### Parameters
* `data` Pointer to the byte buffer. 

* `size` Number of bytes to send.

---

{#state-3}

#### state

`const` `nodiscard`

```cpp
[[nodiscard]] State state() const
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:136

Current session state. Thread-safe.

---

{#remotepeerid}

#### remotePeerId

`const` `nodiscard`

```cpp
[[nodiscard]] std::string remotePeerId() const
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:140

Identifier of the remote peer for the current or most recent call. Empty when Idle.

---

{#media}

#### media

`nodiscard`

```cpp
[[nodiscard]] MediaBridge & media()
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:143

Media bridge for this session. Valid for the lifetime of the [PeerSession](#peersession).

---

{#media-1}

#### media

`const` `nodiscard`

```cpp
[[nodiscard]] const MediaBridge & media() const
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:145

Media bridge for this session. Valid for the lifetime of the [PeerSession](#peersession).

---

{#peerconnection}

#### peerConnection

`nodiscard`

```cpp
[[nodiscard]] std::shared_ptr< rtc::PeerConnection > peerConnection()
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:148

The underlying PeerConnection, or nullptr when Idle/Ended.

---

{#datachannel}

#### dataChannel

`nodiscard`

```cpp
[[nodiscard]] std::shared_ptr< rtc::DataChannel > dataChannel()
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:151

The data channel, or nullptr if none is open.

### Public Types

| Name | Description |
|------|-------------|
| [`State`](#state-4)  | High-level lifecycle phases for a single peer-to-peer call session. |

---

{#state-4}

#### State

```cpp
enum State
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:49

High-level lifecycle phases for a single peer-to-peer call session.

| Value | Description |
|-------|-------------|
| `Idle` | No active call. |
| `OutgoingInit` | Outgoing call announced, waiting for accept/reject. |
| `IncomingInit` | Incoming call announced, waiting for accept/reject. |
| `Negotiating` | PeerConnection exists and SDP/ICE negotiation is in progress. |
| `Active` | Media or data is flowing. |
| `Ending` | Local teardown is in progress. |
| `Ended` | Call ended (transient; auto-resets to Idle). |

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `SignallingInterface &` | [`_signaller`](#_signaller)  |  |
| `Config` | [`_config`](#_config-4)  |  |
| `MediaBridge` | [`_media`](#_media)  |  |
| `State` | [`_state`](#_state-4)  |  |
| `std::string` | [`_remotePeerId`](#_remotepeerid)  |  |
| `std::shared_ptr< rtc::PeerConnection >` | [`_pc`](#_pc-1)  |  |
| `std::shared_ptr< rtc::DataChannel >` | [`_dc`](#_dc)  |  |
| `std::shared_ptr< CallbackGuard >` | [`_callbackGuard`](#_callbackguard)  |  |
| `bool` | [`_remoteDescriptionSet`](#_remotedescriptionset)  |  |
| `std::vector< PendingCandidate >` | [`_pendingRemoteCandidates`](#_pendingremotecandidates)  |  |
| `Synchronizer` | [`_callbackSync`](#_callbacksync)  |  |
| `std::deque< std::function< void()> >` | [`_pendingCallbacks`](#_pendingcallbacks)  |  |
| `std::mutex` | [`_callbackMutex`](#_callbackmutex)  |  |
| `std::mutex` | [`_mutex`](#_mutex-18)  |  |

---

{#_signaller}

#### _signaller

```cpp
SignallingInterface & _signaller
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:181

---

{#_config-4}

#### _config

```cpp
Config _config
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:182

---

{#_media}

#### _media

```cpp
MediaBridge _media
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:183

---

{#_state-4}

#### _state

```cpp
State _state = 
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:184

---

{#_remotepeerid}

#### _remotePeerId

```cpp
std::string _remotePeerId
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:185

---

{#_pc-1}

#### _pc

```cpp
std::shared_ptr< rtc::PeerConnection > _pc
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:186

---

{#_dc}

#### _dc

```cpp
std::shared_ptr< rtc::DataChannel > _dc
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:187

---

{#_callbackguard}

#### _callbackGuard

```cpp
std::shared_ptr< CallbackGuard > _callbackGuard = std::make_shared<CallbackGuard>()
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:188

---

{#_remotedescriptionset}

#### _remoteDescriptionSet

```cpp
bool _remoteDescriptionSet = false
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:189

---

{#_pendingremotecandidates}

#### _pendingRemoteCandidates

```cpp
std::vector< PendingCandidate > _pendingRemoteCandidates
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:190

---

{#_callbacksync}

#### _callbackSync

```cpp
Synchronizer _callbackSync
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:191

---

{#_pendingcallbacks}

#### _pendingCallbacks

```cpp
std::deque< std::function< void()> > _pendingCallbacks
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:192

---

{#_callbackmutex}

#### _callbackMutex

```cpp
std::mutex _callbackMutex
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:193

---

{#_mutex-18}

#### _mutex

```cpp
std::mutex _mutex
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:194

### Private Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`onSdpReceived`](#onsdpreceived)  |  |
| `void` | [`onCandidateReceived`](#oncandidatereceived)  |  |
| `void` | [`onControlReceived`](#oncontrolreceived)  |  |
| `std::shared_ptr< rtc::PeerConnection >` | [`createPeerConnection`](#createpeerconnection)  |  |
| `void` | [`setupPeerConnectionCallbacks`](#setuppeerconnectioncallbacks)  |  |
| `void` | [`beginEndCall`](#beginendcall)  |  |
| `void` | [`finishEndCall`](#finishendcall)  |  |
| `void` | [`transitionEndedToIdle`](#transitionendedtoidle)  |  |
| `void` | [`enqueueCallback`](#enqueuecallback)  |  |
| `void` | [`drainCallbacks`](#draincallbacks)  |  |

---

{#onsdpreceived}

#### onSdpReceived

```cpp
void onSdpReceived(const std::string & peerId, const std::string & type, const std::string & sdp)
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:165

---

{#oncandidatereceived}

#### onCandidateReceived

```cpp
void onCandidateReceived(const std::string & peerId, const std::string & candidate, const std::string & mid)
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:166

---

{#oncontrolreceived}

#### onControlReceived

```cpp
void onControlReceived(const std::string & peerId, const std::string & type, const std::string & reason)
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:167

---

{#createpeerconnection}

#### createPeerConnection

```cpp
std::shared_ptr< rtc::PeerConnection > createPeerConnection(bool createDataChannel, const MediaBridge::Options * mediaOpts = nullptr)
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:169

---

{#setuppeerconnectioncallbacks}

#### setupPeerConnectionCallbacks

```cpp
void setupPeerConnectionCallbacks(const std::shared_ptr< rtc::PeerConnection > & pc)
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:172

---

{#beginendcall}

#### beginEndCall

```cpp
void beginEndCall(const std::string & reason, std::shared_ptr< rtc::PeerConnection > & pc, std::shared_ptr< rtc::DataChannel > & dc)
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:173

---

{#finishendcall}

#### finishEndCall

```cpp
void finishEndCall()
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:176

---

{#transitionendedtoidle}

#### transitionEndedToIdle

```cpp
void transitionEndedToIdle()
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:177

---

{#enqueuecallback}

#### enqueueCallback

```cpp
void enqueueCallback(std::function< void()> callback)
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:178

---

{#draincallbacks}

#### drainCallbacks

```cpp
void drainCallbacks()
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:179

{#mediaconfig}

## MediaConfig

```cpp
#include <icy/webrtc/peersession.h>
```

```cpp
struct MediaConfig
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:61

[Configuration](base.md#configuration) for WebRTC peer session establishment.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `av::VideoCodec` | [`videoCodec`](#videocodec-7)  | Desired video codec for send/receive negotiation. |
| `av::AudioCodec` | [`audioCodec`](#audiocodec-6)  | Desired audio codec for send/receive negotiation. |
| `rtc::Description::Direction` | [`videoDirection`](#videodirection-1)  |  |
| `rtc::Description::Direction` | [`audioDirection`](#audiodirection-1)  |  |
| `JitterBufferConfig` | [`videoJitterBuffer`](#videojitterbuffer-1)  | Receive-side buffering for depacketized remote video frames. |
| `JitterBufferConfig` | [`audioJitterBuffer`](#audiojitterbuffer-1)  | Receive-side buffering for depacketized remote audio frames. |

---

{#videocodec-7}

#### videoCodec

```cpp
av::VideoCodec videoCodec
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:63

Desired video codec for send/receive negotiation.

---

{#audiocodec-6}

#### audioCodec

```cpp
av::AudioCodec audioCodec
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:64

Desired audio codec for send/receive negotiation.

---

{#videodirection-1}

#### videoDirection

```cpp
rtc::Description::Direction videoDirection = rtc::Description::Direction::SendRecv
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:65

---

{#audiodirection-1}

#### audioDirection

```cpp
rtc::Description::Direction audioDirection = rtc::Description::Direction::SendRecv
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:66

---

{#videojitterbuffer-1}

#### videoJitterBuffer

```cpp
JitterBufferConfig videoJitterBuffer
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:67

Receive-side buffering for depacketized remote video frames.

---

{#audiojitterbuffer-1}

#### audioJitterBuffer

```cpp
JitterBufferConfig audioJitterBuffer
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:68

Receive-side buffering for depacketized remote audio frames.

{#config-4}

## Config

```cpp
#include <icy/webrtc/peersession.h>
```

```cpp
struct Config
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:72

[Configuration](base.md#configuration) for WebRTC peer session establishment.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `rtc::Configuration` | [`rtcConfig`](#rtcconfig)  | libdatachannel connection options, ICE servers, and transport settings. |
| `MediaConfig` | [`media`](#media-2)  | Desired media codecs and directions for the session. |
| `bool` | [`enableDataChannel`](#enabledatachannel)  | True to create a data channel on outgoing calls and accept one on incoming calls. |
| `std::string` | [`dataChannelLabel`](#datachannellabel)  | Label to use for the application data channel. |

---

{#rtcconfig}

#### rtcConfig

```cpp
rtc::Configuration rtcConfig
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:74

libdatachannel connection options, ICE servers, and transport settings.

---

{#media-2}

#### media

```cpp
MediaConfig media
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:75

Desired media codecs and directions for the session.

---

{#enabledatachannel}

#### enableDataChannel

```cpp
bool enableDataChannel = true
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:76

True to create a data channel on outgoing calls and accept one on incoming calls.

---

{#datachannellabel}

#### dataChannelLabel

```cpp
std::string dataChannelLabel = "data"
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:77

Label to use for the application data channel.

{#callbackguard}

## CallbackGuard

```cpp
struct CallbackGuard
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:154

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::atomic< bool >` | [`alive`](#alive-1)  |  |

---

{#alive-1}

#### alive

```cpp
std::atomic< bool > alive {true}
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:156

{#pendingcandidate}

## PendingCandidate

```cpp
struct PendingCandidate
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:159

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`candidate`](#candidate)  |  |
| `std::string` | [`mid`](#mid)  |  |

---

{#candidate}

#### candidate

```cpp
std::string candidate
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:161

---

{#mid}

#### mid

```cpp
std::string mid
```

Defined in src/webrtc/include/icy/webrtc/peersession.h:162

{#webrtctracksender}

## WebRtcTrackSender

```cpp
#include <icy/webrtc/tracksender.h>
```

```cpp
class WebRtcTrackSender
```

Defined in src/webrtc/include/icy/webrtc/tracksender.h:56

> **Inherits:** [`PacketProcessor`](base.md#packetprocessor)

[PacketProcessor](base.md#packetprocessor) that sends encoded media to a single libdatachannel Track via sendFrame().

Bind to one track (video or audio). Accepts the corresponding packet type from the [PacketStream](base.md#packetstream) and converts timestamps from FFmpeg microseconds to the track's RTP clock rate.

Usage: auto vh = createVideoTrack(pc, codec); [WebRtcTrackSender](#webrtctracksender) videoSender(vh);

[PacketStream](base.md#packetstream) stream; stream.attachSource(capture); stream.attach(encoder, 1, true); stream.attach(&videoSender, 5, false); stream.start();

Only emits the packet downstream on successful send, so a chained recorder won't record frames that failed to transmit.

Accepts only the packet type that matches the bound track. Non-matching packets are passed through unchanged so mixed audio/video [PacketStream](base.md#packetstream) chains can share one source cleanly.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `PacketSignal` | [`emitter`](#emitter-10)  |  |

---

{#emitter-10}

#### emitter

```cpp
PacketSignal emitter
```

Defined in src/webrtc/include/icy/webrtc/tracksender.h:96

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`WebRtcTrackSender`](#webrtctracksender-1)  | Construct an unbound sender. Call [bind()](#bind-5) before use. |
|  | [`WebRtcTrackSender`](#webrtctracksender-2) `explicit` | Construct bound to a track handle from [createVideoTrack()](#createvideotrack) or [createAudioTrack()](#createaudiotrack). |
| `void` | [`bind`](#bind-5)  | Bind to a track. Can be called to rebind to a different track. |
| `void` | [`unbind`](#unbind-1)  | Unbind from the current track. |
| `void` | [`process`](#process-12) `virtual` `override` | Send an encoded media frame to the bound WebRTC track. Converts the FFmpeg microsecond timestamp to an RTP timestamp using the track's clock rate, then calls rtc::Track::sendFrame(). Only forwards the packet downstream on a successful send. |
| `bool` | [`accepts`](#accepts-5) `virtual` `override` | Return true only for the packet type that matches the bound track. |
| `void` | [`onStreamStateChange`](#onstreamstatechange-6) `virtual` `override` | Called by the [PacketStream](base.md#packetstream) when stream state changes. Logs when the stream is stopping; no other action is taken. |
| `bool` | [`isVideo`](#isvideo) `const` `nodiscard` | True if this sender is bound to a video track. |
| `bool` | [`bound`](#bound) `const` `nodiscard` | True if bound to any track. |

---

{#webrtctracksender-1}

#### WebRtcTrackSender

```cpp
WebRtcTrackSender()
```

Defined in src/webrtc/include/icy/webrtc/tracksender.h:60

Construct an unbound sender. Call [bind()](#bind-5) before use.

---

{#webrtctracksender-2}

#### WebRtcTrackSender

`explicit`

```cpp
explicit WebRtcTrackSender(const TrackHandle & handle)
```

Defined in src/webrtc/include/icy/webrtc/tracksender.h:64

Construct bound to a track handle from [createVideoTrack()](#createvideotrack) or [createAudioTrack()](#createaudiotrack).

---

{#bind-5}

#### bind

```cpp
void bind(const TrackHandle & handle)
```

Defined in src/webrtc/include/icy/webrtc/tracksender.h:67

Bind to a track. Can be called to rebind to a different track.

---

{#unbind-1}

#### unbind

```cpp
void unbind()
```

Defined in src/webrtc/include/icy/webrtc/tracksender.h:70

Unbind from the current track.

---

{#process-12}

#### process

`virtual` `override`

```cpp
virtual void process(IPacket & packet) override
```

Defined in src/webrtc/include/icy/webrtc/tracksender.h:78

Send an encoded media frame to the bound WebRTC track. Converts the FFmpeg microsecond timestamp to an RTP timestamp using the track's clock rate, then calls rtc::Track::sendFrame(). Only forwards the packet downstream on a successful send. 
#### Parameters
* `packet` An [av::VideoPacket](av.md#videopacket) or [av::AudioPacket](av.md#audiopacket) carrying the encoded frame data and a microsecond timestamp.

---

{#accepts-5}

#### accepts

`virtual` `override`

```cpp
virtual bool accepts(IPacket * packet) override
```

Defined in src/webrtc/include/icy/webrtc/tracksender.h:83

Return true only for the packet type that matches the bound track. 
#### Parameters
* `packet` Packet to test. May be nullptr. 

#### Returns
True if the packet can be processed by this sender.

---

{#onstreamstatechange-6}

#### onStreamStateChange

`virtual` `override`

```cpp
virtual void onStreamStateChange(const PacketStreamState & state) override
```

Defined in src/webrtc/include/icy/webrtc/tracksender.h:88

Called by the [PacketStream](base.md#packetstream) when stream state changes. Logs when the stream is stopping; no other action is taken. 
#### Parameters
* `state` New [PacketStream](base.md#packetstream) state.

---

{#isvideo}

#### isVideo

`const` `nodiscard`

```cpp
[[nodiscard]] bool isVideo() const
```

Defined in src/webrtc/include/icy/webrtc/tracksender.h:91

True if this sender is bound to a video track.

---

{#bound}

#### bound

`const` `nodiscard`

```cpp
[[nodiscard]] bool bound() const
```

Defined in src/webrtc/include/icy/webrtc/tracksender.h:94

True if bound to any track.

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::shared_ptr< rtc::Track >` | [`_track`](#_track)  |  |
| `std::shared_ptr< rtc::RtpPacketizationConfig >` | [`_rtpConfig`](#_rtpconfig)  |  |
| `std::atomic< TrackKind >` | [`_kind`](#_kind)  |  |
| `std::mutex` | [`_mutex`](#_mutex-19)  |  |

---

{#_track}

#### _track

```cpp
std::shared_ptr< rtc::Track > _track
```

Defined in src/webrtc/include/icy/webrtc/tracksender.h:106

---

{#_rtpconfig}

#### _rtpConfig

```cpp
std::shared_ptr< rtc::RtpPacketizationConfig > _rtpConfig
```

Defined in src/webrtc/include/icy/webrtc/tracksender.h:107

---

{#_kind}

#### _kind

```cpp
std::atomic< TrackKind > _kind {TrackKind::Unbound}
```

Defined in src/webrtc/include/icy/webrtc/tracksender.h:108

---

{#_mutex-19}

#### _mutex

```cpp
std::mutex _mutex
```

Defined in src/webrtc/include/icy/webrtc/tracksender.h:109

{#webrtctrackreceiver}

## WebRtcTrackReceiver

```cpp
#include <icy/webrtc/trackreceiver.h>
```

```cpp
class WebRtcTrackReceiver
```

Defined in src/webrtc/include/icy/webrtc/trackreceiver.h:61

> **Inherits:** [`PacketStreamAdapter`](base.md#packetstreamadapter)

[PacketStreamAdapter](base.md#packetstreamadapter) that receives depacketized frames from a single remote libdatachannel Track and emits them as VideoPacket or AudioPacket into a [PacketStream](base.md#packetstream).

Call [setupReceiveTrack()](#setupreceivetrack) on the track first to install the correct depacketizer, then bind this receiver to it.

Usage: pc->onTrack([&](shared_ptr<rtc::Track> track) { if (setupReceiveTrack(track)) videoReceiver.bind(track); });

videoReceiver.emitter += packetSlot(&recorder, &Recorder::onEncodedVideo);

Emits owning packets - the frame data is copied, so downstream processors can safely queue packets asynchronously.

Emits VideoPacket for video tracks, AudioPacket for audio tracks. Use those packets to drive a decoder, recorder, or custom pipeline. See `samples/media-recorder` for a complete receive -> decode -> file example.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `PacketSignal` | [`emitter`](#emitter-11)  |  |

---

{#emitter-11}

#### emitter

```cpp
PacketSignal emitter
```

Defined in src/webrtc/include/icy/webrtc/trackreceiver.h:87

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`WebRtcTrackReceiver`](#webrtctrackreceiver-1)  | Construct an unbound receiver. Call [bind()](#bind-6) to attach a remote track. |
| `void` | [`bind`](#bind-6)  | Bind to a remote track. Must be called after [setupReceiveTrack()](#setupreceivetrack) returned true. Installs an onFrame callback that converts each depacketized frame to a VideoPacket or AudioPacket and emits it on the [PacketStream](base.md#packetstream). The track type (video/audio) is detected from the SDP description. |
| `void` | [`configureJitterBuffer`](#configurejitterbuffer)  | Replace the receive-side jitter-buffer settings. |
| `JitterBufferConfig` | [`jitterBufferConfig`](#jitterbufferconfig-1) `const` `nodiscard` | Current jitter-buffer settings for this receiver. |
| `bool` | [`jitterBufferEnabled`](#jitterbufferenabled) `const` `nodiscard` | True when depacketized receive frames are buffered before emission. |

---

{#webrtctrackreceiver-1}

#### WebRtcTrackReceiver

```cpp
WebRtcTrackReceiver()
```

Defined in src/webrtc/include/icy/webrtc/trackreceiver.h:65

Construct an unbound receiver. Call [bind()](#bind-6) to attach a remote track.

---

{#bind-6}

#### bind

```cpp
void bind(std::shared_ptr< rtc::Track > track)
```

Defined in src/webrtc/include/icy/webrtc/trackreceiver.h:74

Bind to a remote track. Must be called after [setupReceiveTrack()](#setupreceivetrack) returned true. Installs an onFrame callback that converts each depacketized frame to a VideoPacket or AudioPacket and emits it on the [PacketStream](base.md#packetstream). The track type (video/audio) is detected from the SDP description. 
#### Parameters
* `track` Remote track from the PeerConnection::onTrack callback.

---

{#configurejitterbuffer}

#### configureJitterBuffer

```cpp
void configureJitterBuffer(const JitterBufferConfig & config)
```

Defined in src/webrtc/include/icy/webrtc/trackreceiver.h:79

Replace the receive-side jitter-buffer settings.

Reconfiguring resets any buffered media still waiting for release.

---

{#jitterbufferconfig-1}

#### jitterBufferConfig

`const` `nodiscard`

```cpp
[[nodiscard]] JitterBufferConfig jitterBufferConfig() const
```

Defined in src/webrtc/include/icy/webrtc/trackreceiver.h:82

Current jitter-buffer settings for this receiver.

---

{#jitterbufferenabled}

#### jitterBufferEnabled

`const` `nodiscard`

```cpp
[[nodiscard]] bool jitterBufferEnabled() const
```

Defined in src/webrtc/include/icy/webrtc/trackreceiver.h:85

True when depacketized receive frames are buffered before emission.

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `Synchronizer` | [`_dispatch`](#_dispatch)  |  |
| `Timer` | [`_timer`](#_timer-3)  |  |
| `std::mutex` | [`_mutex`](#_mutex-20)  |  |
| `std::deque< std::unique_ptr< IPacket > >` | [`_pending`](#_pending)  |  |
| `std::unique_ptr< detail::ReceiverJitterBuffer >` | [`_jitterBuffer`](#_jitterbuffer)  |  |
| `JitterBufferConfig` | [`_jitterConfig`](#_jitterconfig)  |  |
| `std::int64_t` | [`_timerTickMs`](#_timertickms)  |  |
| `bool` | [`_timerNeedsUpdate`](#_timerneedsupdate)  |  |
| `std::shared_ptr< DispatchState >` | [`_state`](#_state-5)  |  |
| `uint64_t` | [`_generation`](#_generation)  |  |

---

{#_dispatch}

#### _dispatch

```cpp
Synchronizer _dispatch
```

Defined in src/webrtc/include/icy/webrtc/trackreceiver.h:99

---

{#_timer-3}

#### _timer

```cpp
Timer _timer
```

Defined in src/webrtc/include/icy/webrtc/trackreceiver.h:100

---

{#_mutex-20}

#### _mutex

```cpp
std::mutex _mutex
```

Defined in src/webrtc/include/icy/webrtc/trackreceiver.h:101

---

{#_pending}

#### _pending

```cpp
std::deque< std::unique_ptr< IPacket > > _pending
```

Defined in src/webrtc/include/icy/webrtc/trackreceiver.h:102

---

{#_jitterbuffer}

#### _jitterBuffer

```cpp
std::unique_ptr< detail::ReceiverJitterBuffer > _jitterBuffer
```

Defined in src/webrtc/include/icy/webrtc/trackreceiver.h:103

---

{#_jitterconfig}

#### _jitterConfig

```cpp
JitterBufferConfig _jitterConfig
```

Defined in src/webrtc/include/icy/webrtc/trackreceiver.h:104

---

{#_timertickms}

#### _timerTickMs

```cpp
std::int64_t _timerTickMs = 5
```

Defined in src/webrtc/include/icy/webrtc/trackreceiver.h:105

---

{#_timerneedsupdate}

#### _timerNeedsUpdate

```cpp
bool _timerNeedsUpdate = false
```

Defined in src/webrtc/include/icy/webrtc/trackreceiver.h:106

---

{#_state-5}

#### _state

```cpp
std::shared_ptr< DispatchState > _state = std::make_shared<DispatchState>()
```

Defined in src/webrtc/include/icy/webrtc/trackreceiver.h:107

---

{#_generation}

#### _generation

```cpp
uint64_t _generation = 0
```

Defined in src/webrtc/include/icy/webrtc/trackreceiver.h:108

### Private Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`enqueue`](#enqueue)  |  |
| `void` | [`flushPending`](#flushpending)  |  |

---

{#enqueue}

#### enqueue

```cpp
void enqueue(std::unique_ptr< IPacket > packet)
```

Defined in src/webrtc/include/icy/webrtc/trackreceiver.h:96

---

{#flushpending}

#### flushPending

```cpp
void flushPending()
```

Defined in src/webrtc/include/icy/webrtc/trackreceiver.h:97

{#dispatchstate}

## DispatchState

```cpp
struct DispatchState
```

Defined in src/webrtc/include/icy/webrtc/trackreceiver.h:90

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::atomic< bool >` | [`alive`](#alive-2)  |  |
| `std::atomic< uint64_t >` | [`generation`](#generation)  |  |

---

{#alive-2}

#### alive

```cpp
std::atomic< bool > alive {true}
```

Defined in src/webrtc/include/icy/webrtc/trackreceiver.h:92

---

{#generation}

#### generation

```cpp
std::atomic< uint64_t > generation {0}
```

Defined in src/webrtc/include/icy/webrtc/trackreceiver.h:93

{#codecnegotiator}

## CodecNegotiator

```cpp
#include <icy/webrtc/codecnegotiator.h>
```

```cpp
class CodecNegotiator
```

Defined in src/webrtc/include/icy/webrtc/codecnegotiator.h:118

Maps RTP codec names to FFmpeg encoders and queries FFmpeg at runtime to determine what codecs are available.

This is stateless - all methods are static. Call negotiate*() with a list of RTP codec names offered by the remote peer, and it returns the best match that FFmpeg can encode.

### Public Static Methods

| Return | Name | Description |
|--------|------|-------------|
| `std::optional< NegotiatedCodec >` | [`negotiateVideo`](#negotiatevideo) `static` `nodiscard` | Negotiate the best video codec from a list of offered RTP codec names. Returns the first match that FFmpeg can encode, in preference order: H264 > VP8 > VP9 > AV1 > H265 |
| `std::optional< NegotiatedCodec >` | [`negotiateAudio`](#negotiateaudio) `static` `nodiscard` | Negotiate the best audio codec from a list of offered RTP codec names. Returns the first match that FFmpeg can encode, in preference order: opus > PCMU > PCMA |
| `bool` | [`hasEncoder`](#hasencoder) `static` `nodiscard` | Check if FFmpeg has an encoder for the given codec name. Accepts both FFmpeg names ("libx264") and RTP names ("H264"). |
| `std::string` | [`rtpToFfmpeg`](#rtptoffmpeg) `static` `nodiscard` | Map an RTP codec name to the preferred FFmpeg encoder name. Returns empty string if no mapping exists. |
| `std::string` | [`ffmpegToRtp`](#ffmpegtortp) `static` `nodiscard` | Map an FFmpeg encoder name to the RTP codec name. Returns empty string if no mapping exists. |
| `uint32_t` | [`clockRate`](#clockrate-2) `static` `nodiscard` | Get the standard RTP clock rate for a codec. |
| `int` | [`defaultPayloadType`](#defaultpayloadtype) `static` `nodiscard` | Get the default RTP payload type for a codec. Returns 0 for dynamic payload types (caller should assign). |
| `std::optional< CodecSpec >` | [`specFromRtp`](#specfromrtp) `static` `nodiscard` | Return the canonical codec spec for an RTP name, if known. |
| `std::optional< CodecSpec >` | [`specFromFfmpeg`](#specfromffmpeg) `static` `nodiscard` | Return the canonical codec spec for an FFmpeg encoder name, if known. |
| `std::optional< CodecSpec >` | [`specFromVideoCodec`](#specfromvideocodec) `static` `nodiscard` | Resolve the canonical codec spec from an explicit video codec config. Prefers the FFmpeg encoder when present, otherwise falls back to RTP name. |
| `std::optional< CodecSpec >` | [`specFromAudioCodec`](#specfromaudiocodec) `static` `nodiscard` | Resolve the canonical codec spec from an explicit audio codec config. Prefers the FFmpeg encoder when present, otherwise falls back to RTP name. |
| `CodecSpec` | [`requireVideoSpec`](#requirevideospec) `static` `nodiscard` | Resolve a strict canonical video codec spec or throw. |
| `CodecSpec` | [`requireAudioSpec`](#requireaudiospec) `static` `nodiscard` | Resolve a strict canonical audio codec spec or throw. |
| `av::VideoCodec` | [`resolveWebRtcVideoCodec`](#resolvewebrtcvideocodec) `static` `nodiscard` | Resolve a browser-safe WebRTC video codec config from an explicit codec. |
| `av::AudioCodec` | [`resolveWebRtcAudioCodec`](#resolvewebrtcaudiocodec) `static` `nodiscard` | Resolve a browser-safe WebRTC audio codec config from an explicit codec. |
| `std::optional< CodecSpec >` | [`detectCodec`](#detectcodec) `static` `nodiscard` | Detect the first known codec present in an SDP snippet for the given media type. |
| `std::optional< CodecSpec >` | [`detectCodecInMedia`](#detectcodecinmedia) `static` `nodiscard` | Detect the first known codec from a structured rtc::Description::Media object for the given media type. |
| `AVCodecID` | [`decoderCodecId`](#decodercodecid) `static` `nodiscard` | Return the FFmpeg decoder codec ID for a given codec spec. Returns AV_CODEC_ID_NONE if no mapping is known. |

---

{#negotiatevideo}

#### negotiateVideo

`static` `nodiscard`

```cpp
[[nodiscard]] static std::optional< NegotiatedCodec > negotiateVideo(const std::vector< std::string > & offeredCodecs)
```

Defined in src/webrtc/include/icy/webrtc/codecnegotiator.h:125

Negotiate the best video codec from a list of offered RTP codec names. Returns the first match that FFmpeg can encode, in preference order: H264 > VP8 > VP9 > AV1 > H265

---

{#negotiateaudio}

#### negotiateAudio

`static` `nodiscard`

```cpp
[[nodiscard]] static std::optional< NegotiatedCodec > negotiateAudio(const std::vector< std::string > & offeredCodecs)
```

Defined in src/webrtc/include/icy/webrtc/codecnegotiator.h:131

Negotiate the best audio codec from a list of offered RTP codec names. Returns the first match that FFmpeg can encode, in preference order: opus > PCMU > PCMA

---

{#hasencoder}

#### hasEncoder

`static` `nodiscard`

```cpp
[[nodiscard]] static bool hasEncoder(const std::string & name)
```

Defined in src/webrtc/include/icy/webrtc/codecnegotiator.h:135

Check if FFmpeg has an encoder for the given codec name. Accepts both FFmpeg names ("libx264") and RTP names ("H264").

---

{#rtptoffmpeg}

#### rtpToFfmpeg

`static` `nodiscard`

```cpp
[[nodiscard]] static std::string rtpToFfmpeg(const std::string & rtpName)
```

Defined in src/webrtc/include/icy/webrtc/codecnegotiator.h:139

Map an RTP codec name to the preferred FFmpeg encoder name. Returns empty string if no mapping exists.

---

{#ffmpegtortp}

#### ffmpegToRtp

`static` `nodiscard`

```cpp
[[nodiscard]] static std::string ffmpegToRtp(const std::string & ffmpegName)
```

Defined in src/webrtc/include/icy/webrtc/codecnegotiator.h:143

Map an FFmpeg encoder name to the RTP codec name. Returns empty string if no mapping exists.

---

{#clockrate-2}

#### clockRate

`static` `nodiscard`

```cpp
[[nodiscard]] static uint32_t clockRate(const std::string & rtpName)
```

Defined in src/webrtc/include/icy/webrtc/codecnegotiator.h:146

Get the standard RTP clock rate for a codec.

---

{#defaultpayloadtype}

#### defaultPayloadType

`static` `nodiscard`

```cpp
[[nodiscard]] static int defaultPayloadType(const std::string & rtpName)
```

Defined in src/webrtc/include/icy/webrtc/codecnegotiator.h:150

Get the default RTP payload type for a codec. Returns 0 for dynamic payload types (caller should assign).

---

{#specfromrtp}

#### specFromRtp

`static` `nodiscard`

```cpp
[[nodiscard]] static std::optional< CodecSpec > specFromRtp(const std::string & rtpName)
```

Defined in src/webrtc/include/icy/webrtc/codecnegotiator.h:154

Return the canonical codec spec for an RTP name, if known.

---

{#specfromffmpeg}

#### specFromFfmpeg

`static` `nodiscard`

```cpp
[[nodiscard]] static std::optional< CodecSpec > specFromFfmpeg(const std::string & ffmpegName)
```

Defined in src/webrtc/include/icy/webrtc/codecnegotiator.h:158

Return the canonical codec spec for an FFmpeg encoder name, if known.

---

{#specfromvideocodec}

#### specFromVideoCodec

`static` `nodiscard`

```cpp
[[nodiscard]] static std::optional< CodecSpec > specFromVideoCodec(const av::VideoCodec & codec)
```

Defined in src/webrtc/include/icy/webrtc/codecnegotiator.h:163

Resolve the canonical codec spec from an explicit video codec config. Prefers the FFmpeg encoder when present, otherwise falls back to RTP name.

---

{#specfromaudiocodec}

#### specFromAudioCodec

`static` `nodiscard`

```cpp
[[nodiscard]] static std::optional< CodecSpec > specFromAudioCodec(const av::AudioCodec & codec)
```

Defined in src/webrtc/include/icy/webrtc/codecnegotiator.h:168

Resolve the canonical codec spec from an explicit audio codec config. Prefers the FFmpeg encoder when present, otherwise falls back to RTP name.

---

{#requirevideospec}

#### requireVideoSpec

`static` `nodiscard`

```cpp
[[nodiscard]] static CodecSpec requireVideoSpec(const av::VideoCodec & codec)
```

Defined in src/webrtc/include/icy/webrtc/codecnegotiator.h:171

Resolve a strict canonical video codec spec or throw.

---

{#requireaudiospec}

#### requireAudioSpec

`static` `nodiscard`

```cpp
[[nodiscard]] static CodecSpec requireAudioSpec(const av::AudioCodec & codec)
```

Defined in src/webrtc/include/icy/webrtc/codecnegotiator.h:174

Resolve a strict canonical audio codec spec or throw.

---

{#resolvewebrtcvideocodec}

#### resolveWebRtcVideoCodec

`static` `nodiscard`

```cpp
[[nodiscard]] static av::VideoCodec resolveWebRtcVideoCodec(const av::VideoCodec & codec)
```

Defined in src/webrtc/include/icy/webrtc/codecnegotiator.h:178

Resolve a browser-safe WebRTC video codec config from an explicit codec.

---

{#resolvewebrtcaudiocodec}

#### resolveWebRtcAudioCodec

`static` `nodiscard`

```cpp
[[nodiscard]] static av::AudioCodec resolveWebRtcAudioCodec(const av::AudioCodec & codec)
```

Defined in src/webrtc/include/icy/webrtc/codecnegotiator.h:182

Resolve a browser-safe WebRTC audio codec config from an explicit codec.

---

{#detectcodec}

#### detectCodec

`static` `nodiscard`

```cpp
[[nodiscard]] static std::optional< CodecSpec > detectCodec(std::string_view sdp, CodecMediaType mediaType)
```

Defined in src/webrtc/include/icy/webrtc/codecnegotiator.h:187

Detect the first known codec present in an SDP snippet for the given media type.

---

{#detectcodecinmedia}

#### detectCodecInMedia

`static` `nodiscard`

```cpp
[[nodiscard]] static std::optional< CodecSpec > detectCodecInMedia(const rtc::Description::Media & media, CodecMediaType mediaType)
```

Defined in src/webrtc/include/icy/webrtc/codecnegotiator.h:192

Detect the first known codec from a structured rtc::Description::Media object for the given media type.

---

{#decodercodecid}

#### decoderCodecId

`static` `nodiscard`

```cpp
[[nodiscard]] static AVCodecID decoderCodecId(const CodecSpec & spec)
```

Defined in src/webrtc/include/icy/webrtc/codecnegotiator.h:197

Return the FFmpeg decoder codec ID for a given codec spec. Returns AV_CODEC_ID_NONE if no mapping is known.

{#trackhandle}

## TrackHandle

```cpp
#include <icy/webrtc/track.h>
```

```cpp
struct TrackHandle
```

Defined in src/webrtc/include/icy/webrtc/track.h:31

Result of creating a track: the track itself plus its RTP config. Keep the config around - you need it for [WebRtcTrackSender](#webrtctracksender).

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::shared_ptr< rtc::Track >` | [`track`](#track-1)  | The libdatachannel track added to the PeerConnection. |
| `std::shared_ptr< rtc::RtpPacketizationConfig >` | [`rtpConfig`](#rtpconfig)  | RTP packetization state required by [WebRtcTrackSender](#webrtctracksender). |

---

{#track-1}

#### track

```cpp
std::shared_ptr< rtc::Track > track
```

Defined in src/webrtc/include/icy/webrtc/track.h:33

The libdatachannel track added to the PeerConnection.

---

{#rtpconfig}

#### rtpConfig

```cpp
std::shared_ptr< rtc::RtpPacketizationConfig > rtpConfig
```

Defined in src/webrtc/include/icy/webrtc/track.h:34

RTP packetization state required by [WebRtcTrackSender](#webrtctracksender).

{#jitterbufferconfig}

## JitterBufferConfig

```cpp
#include <icy/webrtc/jitterbuffer.h>
```

```cpp
struct JitterBufferConfig
```

Defined in src/webrtc/include/icy/webrtc/jitterbuffer.h:30

Receive-side jitter buffer behaviour for depacketized WebRTC media frames.

The jitter buffer sits after libdatachannel depacketization and before icey emits encoded AudioPacket/VideoPacket objects to downstream decoders or recorders. It reorders frames by RTP-derived media timestamp and delays release long enough to absorb moderate network jitter.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `bool` | [`enabled`](#enabled-1)  | False keeps the current zero-buffer receive path. |
| `std::int64_t` | [`minDelayMs`](#mindelayms)  | Base playout delay in milliseconds. |
| `std::int64_t` | [`maxDelayMs`](#maxdelayms)  | Upper bound for the adaptive playout delay. |
| `double` | [`adaptiveFactor`](#adaptivefactor)  | Extra delay multiplier applied to observed inter-arrival variance. |
| `std::int64_t` | [`tickIntervalMs`](#tickintervalms)  | Poll interval for releasing buffered frames. |

---

{#enabled-1}

#### enabled

```cpp
bool enabled = false
```

Defined in src/webrtc/include/icy/webrtc/jitterbuffer.h:32

False keeps the current zero-buffer receive path.

---

{#mindelayms}

#### minDelayMs

```cpp
std::int64_t minDelayMs = 20
```

Defined in src/webrtc/include/icy/webrtc/jitterbuffer.h:33

Base playout delay in milliseconds.

---

{#maxdelayms}

#### maxDelayMs

```cpp
std::int64_t maxDelayMs = 120
```

Defined in src/webrtc/include/icy/webrtc/jitterbuffer.h:34

Upper bound for the adaptive playout delay.

---

{#adaptivefactor}

#### adaptiveFactor

```cpp
double adaptiveFactor = 1.5
```

Defined in src/webrtc/include/icy/webrtc/jitterbuffer.h:35

Extra delay multiplier applied to observed inter-arrival variance.

---

{#tickintervalms}

#### tickIntervalMs

```cpp
std::int64_t tickIntervalMs = 5
```

Defined in src/webrtc/include/icy/webrtc/jitterbuffer.h:36

Poll interval for releasing buffered frames.

{#codecspec}

## CodecSpec

```cpp
#include <icy/webrtc/codecnegotiator.h>
```

```cpp
struct CodecSpec
```

Defined in src/webrtc/include/icy/webrtc/codecnegotiator.h:65

Canonical description of a codec supported by icey's WebRTC helpers.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `CodecId` | [`id`](#id-10)  | Stable codec identifier. |
| `CodecMediaType` | [`mediaType`](#mediatype)  | Audio or video media kind. |
| `std::string` | [`rtpName`](#rtpname)  | Canonical RTP codec name. |
| `std::string` | [`ffmpegName`](#ffmpegname)  | Preferred FFmpeg encoder name. |
| `uint32_t` | [`clockRate`](#clockrate)  | RTP clock rate in Hz. |
| `int` | [`payloadType`](#payloadtype)  | Default static or preferred dynamic payload type. |
| `std::string` | [`fmtp`](#fmtp)  | Canonical fmtp line for SDP generation. |

---

{#id-10}

#### id

```cpp
CodecId id = 
```

Defined in src/webrtc/include/icy/webrtc/codecnegotiator.h:67

Stable codec identifier.

---

{#mediatype}

#### mediaType

```cpp
CodecMediaType mediaType = 
```

Defined in src/webrtc/include/icy/webrtc/codecnegotiator.h:68

Audio or video media kind.

---

{#rtpname}

#### rtpName

```cpp
std::string rtpName
```

Defined in src/webrtc/include/icy/webrtc/codecnegotiator.h:69

Canonical RTP codec name.

---

{#ffmpegname}

#### ffmpegName

```cpp
std::string ffmpegName
```

Defined in src/webrtc/include/icy/webrtc/codecnegotiator.h:70

Preferred FFmpeg encoder name.

---

{#clockrate}

#### clockRate

```cpp
uint32_t clockRate = 0
```

Defined in src/webrtc/include/icy/webrtc/codecnegotiator.h:71

RTP clock rate in Hz.

---

{#payloadtype}

#### payloadType

```cpp
int payloadType = 0
```

Defined in src/webrtc/include/icy/webrtc/codecnegotiator.h:72

Default static or preferred dynamic payload type.

---

{#fmtp}

#### fmtp

```cpp
std::string fmtp
```

Defined in src/webrtc/include/icy/webrtc/codecnegotiator.h:73

Canonical fmtp line for SDP generation.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
| `bool` | [`valid`](#valid-18) `const` `inline` `nodiscard` |  |

---

{#valid-18}

#### valid

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline bool valid() const
```

Defined in src/webrtc/include/icy/webrtc/codecnegotiator.h:75

{#negotiatedcodec}

## NegotiatedCodec

```cpp
#include <icy/webrtc/codecnegotiator.h>
```

```cpp
struct NegotiatedCodec
```

Defined in src/webrtc/include/icy/webrtc/codecnegotiator.h:81

Result of codec negotiation between a remote SDP offer and the local FFmpeg codec inventory.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`rtpName`](#rtpname-1)  | RTP codec name (e.g. "H264", "VP8", "opus"). |
| `std::string` | [`ffmpegName`](#ffmpegname-1)  | FFmpeg encoder name (e.g. "libx264", "libvpx", "libopus"). |
| `int` | [`payloadType`](#payloadtype-1)  | RTP payload type from SDP. |
| `uint32_t` | [`clockRate`](#clockrate-1)  | RTP clock rate (90000 for video, 48000 for opus). |
| `std::string` | [`fmtp`](#fmtp-1)  | Format parameters (e.g. "profile-level-id=42e01f"). |

---

{#rtpname-1}

#### rtpName

```cpp
std::string rtpName
```

Defined in src/webrtc/include/icy/webrtc/codecnegotiator.h:83

RTP codec name (e.g. "H264", "VP8", "opus").

---

{#ffmpegname-1}

#### ffmpegName

```cpp
std::string ffmpegName
```

Defined in src/webrtc/include/icy/webrtc/codecnegotiator.h:84

FFmpeg encoder name (e.g. "libx264", "libvpx", "libopus").

---

{#payloadtype-1}

#### payloadType

```cpp
int payloadType = 0
```

Defined in src/webrtc/include/icy/webrtc/codecnegotiator.h:85

RTP payload type from SDP.

---

{#clockrate-1}

#### clockRate

```cpp
uint32_t clockRate = 0
```

Defined in src/webrtc/include/icy/webrtc/codecnegotiator.h:86

RTP clock rate (90000 for video, 48000 for opus).

---

{#fmtp-1}

#### fmtp

```cpp
std::string fmtp
```

Defined in src/webrtc/include/icy/webrtc/codecnegotiator.h:87

Format parameters (e.g. "profile-level-id=42e01f").

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
| `av::VideoCodec` | [`toVideoCodec`](#tovideocodec) `const` `nodiscard` | Create an [av::VideoCodec](av.md#videocodec) from this negotiation result. Width, height, fps default to 0 (caller should set these). |
| `av::VideoCodec` | [`toWebRtcVideoCodec`](#towebrtcvideocodec) `const` `nodiscard` | Create an [av::VideoCodec](av.md#videocodec) configured for WebRTC browser playback. Sets low-latency options: ultrafast preset, zerolatency tune, constrained baseline profile for H.264, and appropriate defaults for VP8/VP9/AV1. |
| `av::AudioCodec` | [`toAudioCodec`](#toaudiocodec) `const` `nodiscard` | Create an [av::AudioCodec](av.md#audiocodec) from this negotiation result. Channels default to 2, sampleRate to the RTP clock rate. |
| `av::AudioCodec` | [`toWebRtcAudioCodec`](#towebrtcaudiocodec) `const` `nodiscard` | Create an [av::AudioCodec](av.md#audiocodec) configured for WebRTC browser playback. Forces 48000 Hz for Opus, sets appropriate options. |

---

{#tovideocodec}

#### toVideoCodec

`const` `nodiscard`

```cpp
[[nodiscard]] av::VideoCodec toVideoCodec(int width = 0, int height = 0, double fps = 0.0) const
```

Defined in src/webrtc/include/icy/webrtc/codecnegotiator.h:91

Create an [av::VideoCodec](av.md#videocodec) from this negotiation result. Width, height, fps default to 0 (caller should set these).

---

{#towebrtcvideocodec}

#### toWebRtcVideoCodec

`const` `nodiscard`

```cpp
[[nodiscard]] av::VideoCodec toWebRtcVideoCodec(int width = 0, int height = 0, double fps = 0.0) const
```

Defined in src/webrtc/include/icy/webrtc/codecnegotiator.h:98

Create an [av::VideoCodec](av.md#videocodec) configured for WebRTC browser playback. Sets low-latency options: ultrafast preset, zerolatency tune, constrained baseline profile for H.264, and appropriate defaults for VP8/VP9/AV1.

---

{#toaudiocodec}

#### toAudioCodec

`const` `nodiscard`

```cpp
[[nodiscard]] av::AudioCodec toAudioCodec(int channels = 2, int sampleRate = 0) const
```

Defined in src/webrtc/include/icy/webrtc/codecnegotiator.h:103

Create an [av::AudioCodec](av.md#audiocodec) from this negotiation result. Channels default to 2, sampleRate to the RTP clock rate.

---

{#towebrtcaudiocodec}

#### toWebRtcAudioCodec

`const` `nodiscard`

```cpp
[[nodiscard]] av::AudioCodec toWebRtcAudioCodec(int channels = 2) const
```

Defined in src/webrtc/include/icy/webrtc/codecnegotiator.h:108

Create an [av::AudioCodec](av.md#audiocodec) configured for WebRTC browser playback. Forces 48000 Hz for Opus, sets appropriate options.

