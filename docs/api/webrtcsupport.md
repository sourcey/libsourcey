{#webrtcsupporthelpers}

# webrtcsupport

Shared signalling helpers and sample/app adapters layered on top of the core WebRTC module.

### Classes

| Name | Description |
|------|-------------|
| [`WebSocketSignaller`](#websocketsignaller) | [SignallingInterface](webrtc.md#signallinginterface) implementation using plain JSON over WebSocket. |
| [`SympleSignaller`](#symplesignaller) | [SignallingInterface](webrtc.md#signallinginterface) implementation using the Symple call protocol. |
| [`SympleServerSignaller`](#sympleserversignaller) | Server-side Symple signaller for virtual peers and embedded apps. |

{#websocketsignaller}

## WebSocketSignaller

```cpp
#include <icy/webrtc/support/wssignaller.h>
```

```cpp
class WebSocketSignaller
```

Defined in src/webrtc/support/include/icy/webrtc/support/wssignaller.h:29

> **Inherits:** [`SignallingInterface`](webrtc.md#signallinginterface)

[SignallingInterface](webrtc.md#signallinginterface) implementation using plain JSON over WebSocket.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `Signal< void(const std::string &)>` | [`SendMessage`](#sendmessage)  |  |

---

{#sendmessage}

#### SendMessage

```cpp
Signal< void(const std::string &)> SendMessage
```

Defined in src/webrtc/support/include/icy/webrtc/support/wssignaller.h:52

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`WebSocketSignaller`](#websocketsignaller-1) `explicit` |  |
|  | [`WebSocketSignaller`](#websocketsignaller-2)  | Deleted constructor. |
| `void` | [`sendSdp`](#sendsdp-1) `virtual` `override` | Send an SDP offer or answer to the remote peer. |
| `void` | [`sendCandidate`](#sendcandidate-1) `virtual` `override` | Send an ICE candidate to the remote peer. |
| `void` | [`sendControl`](#sendcontrol-1) `virtual` `override` | Send a control message to the remote peer. |
| `void` | [`receive`](#receive)  |  |
| `void` | [`setLocalPeerId`](#setlocalpeerid) `inline` |  |
| `const std::string &` | [`localPeerId`](#localpeerid) `const` `inline` `nodiscard` |  |

---

{#websocketsignaller-1}

#### WebSocketSignaller

`explicit`

```cpp
explicit WebSocketSignaller(const std::string & localPeerId = {})
```

Defined in src/webrtc/support/include/icy/webrtc/support/wssignaller.h:32

---

{#websocketsignaller-2}

#### WebSocketSignaller

```cpp
WebSocketSignaller(const WebSocketSignaller &) = delete
```

Defined in src/webrtc/support/include/icy/webrtc/support/wssignaller.h:35

Deleted constructor.

---

{#sendsdp-1}

#### sendSdp

`virtual` `override`

```cpp
virtual void sendSdp(const std::string & peerId, const std::string & type, const std::string & sdp) override
```

Defined in src/webrtc/support/include/icy/webrtc/support/wssignaller.h:38

Send an SDP offer or answer to the remote peer. 
#### Parameters
* `peerId` Remote peer identifier. 

* `type` "offer" or "answer". 

* `sdp` The SDP string.

---

{#sendcandidate-1}

#### sendCandidate

`virtual` `override`

```cpp
virtual void sendCandidate(const std::string & peerId, const std::string & candidate, const std::string & mid) override
```

Defined in src/webrtc/support/include/icy/webrtc/support/wssignaller.h:42

Send an ICE candidate to the remote peer. 
#### Parameters
* `peerId` Remote peer identifier. 

* `candidate` The candidate string (from RTCIceCandidate). 

* `mid` The sdpMid value.

---

{#sendcontrol-1}

#### sendControl

`virtual` `override`

```cpp
virtual void sendControl(const std::string & peerId, const std::string & type, const std::string & reason = {}) override
```

Defined in src/webrtc/support/include/icy/webrtc/support/wssignaller.h:46

Send a control message to the remote peer. 
#### Parameters
* `peerId` Remote peer identifier. 

* `type` Control type: "init", "accept", "reject", "hangup". 

* `reason` Optional reason string (for reject/hangup).

---

{#receive}

#### receive

```cpp
void receive(const std::string & json)
```

Defined in src/webrtc/support/include/icy/webrtc/support/wssignaller.h:50

---

{#setlocalpeerid}

#### setLocalPeerId

`inline`

```cpp
inline void setLocalPeerId(const std::string & id)
```

Defined in src/webrtc/support/include/icy/webrtc/support/wssignaller.h:54

---

{#localpeerid}

#### localPeerId

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline const std::string & localPeerId() const
```

Defined in src/webrtc/support/include/icy/webrtc/support/wssignaller.h:55

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`_localPeerId`](#_localpeerid)  |  |
| `std::mutex` | [`_mutex`](#_mutex-21)  |  |

---

{#_localpeerid}

#### _localPeerId

```cpp
std::string _localPeerId
```

Defined in src/webrtc/support/include/icy/webrtc/support/wssignaller.h:60

---

{#_mutex-21}

#### _mutex

```cpp
std::mutex _mutex
```

Defined in src/webrtc/support/include/icy/webrtc/support/wssignaller.h:61

### Private Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`sendJson`](#sendjson-1)  |  |

---

{#sendjson-1}

#### sendJson

```cpp
void sendJson(const json::Value & msg)
```

Defined in src/webrtc/support/include/icy/webrtc/support/wssignaller.h:58

{#symplesignaller}

## SympleSignaller

```cpp
#include <icy/webrtc/support/symplesignaller.h>
```

```cpp
class SympleSignaller
```

Defined in src/webrtc/support/include/icy/webrtc/support/symplesignaller.h:34

> **Inherits:** [`SignallingInterface`](webrtc.md#signallinginterface)

[SignallingInterface](webrtc.md#signallinginterface) implementation using the Symple call protocol.

Speaks the `call:init`, `call:accept`, `call:reject`, `call:offer`, `call:answer`, `call:candidate`, and `call:hangup` envelope over a connected Symple client.

The remote peer identifier on this signalling boundary is the full Symple address string (`user|id`).

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`SympleSignaller`](#symplesignaller-1) `explicit` |  |
|  | [`SympleSignaller`](#symplesignaller-2)  | Deleted constructor. |
| `void` | [`sendSdp`](#sendsdp-2) `virtual` `override` | Send an SDP offer or answer to the remote peer. |
| `void` | [`sendCandidate`](#sendcandidate-2) `virtual` `override` | Send an ICE candidate to the remote peer. |
| `void` | [`sendControl`](#sendcontrol-2) `virtual` `override` | Send a control message to the remote peer. |

---

{#symplesignaller-1}

#### SympleSignaller

`explicit`

```cpp
explicit SympleSignaller(smpl::Client & client)
```

Defined in src/webrtc/support/include/icy/webrtc/support/symplesignaller.h:37

---

{#symplesignaller-2}

#### SympleSignaller

```cpp
SympleSignaller(const SympleSignaller &) = delete
```

Defined in src/webrtc/support/include/icy/webrtc/support/symplesignaller.h:40

Deleted constructor.

---

{#sendsdp-2}

#### sendSdp

`virtual` `override`

```cpp
virtual void sendSdp(const std::string & peerId, const std::string & type, const std::string & sdp) override
```

Defined in src/webrtc/support/include/icy/webrtc/support/symplesignaller.h:43

Send an SDP offer or answer to the remote peer. 
#### Parameters
* `peerId` Remote peer identifier. 

* `type` "offer" or "answer". 

* `sdp` The SDP string.

---

{#sendcandidate-2}

#### sendCandidate

`virtual` `override`

```cpp
virtual void sendCandidate(const std::string & peerId, const std::string & candidate, const std::string & mid) override
```

Defined in src/webrtc/support/include/icy/webrtc/support/symplesignaller.h:47

Send an ICE candidate to the remote peer. 
#### Parameters
* `peerId` Remote peer identifier. 

* `candidate` The candidate string (from RTCIceCandidate). 

* `mid` The sdpMid value.

---

{#sendcontrol-2}

#### sendControl

`virtual` `override`

```cpp
virtual void sendControl(const std::string & peerId, const std::string & type, const std::string & reason = {}) override
```

Defined in src/webrtc/support/include/icy/webrtc/support/symplesignaller.h:51

Send a control message to the remote peer. 
#### Parameters
* `peerId` Remote peer identifier. 

* `type` Control type: "init", "accept", "reject", "hangup". 

* `reason` Optional reason string (for reject/hangup).

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `smpl::Client &` | [`_client`](#_client)  |  |

---

{#_client}

#### _client

```cpp
smpl::Client & _client
```

Defined in src/webrtc/support/include/icy/webrtc/support/symplesignaller.h:61

### Private Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`onMessage`](#onmessage-1)  |  |
| `void` | [`send`](#send-20)  |  |

---

{#onmessage-1}

#### onMessage

```cpp
void onMessage(smpl::Message & msg)
```

Defined in src/webrtc/support/include/icy/webrtc/support/symplesignaller.h:56

---

{#send-20}

#### send

```cpp
void send(const std::string & action, const std::string & to, const json::Value & data = {})
```

Defined in src/webrtc/support/include/icy/webrtc/support/symplesignaller.h:57

{#sympleserversignaller}

## SympleServerSignaller

```cpp
#include <icy/webrtc/support/sympleserversignaller.h>
```

```cpp
class SympleServerSignaller
```

Defined in src/webrtc/support/include/icy/webrtc/support/sympleserversignaller.h:39

> **Inherits:** [`SignallingInterface`](webrtc.md#signallinginterface)

Server-side Symple signaller for virtual peers and embedded apps.

This adapter sends signalling messages through a `[smpl::Server](symple.md#server-10)` on behalf of a local virtual peer address, and accepts inbound Symple call messages through `[onMessage()](#onmessage-2)`.

The remote peer identifier on this signalling boundary is the full Symple address string (`user|id`).

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`SympleServerSignaller`](#sympleserversignaller-1)  |  |
|  | [`SympleServerSignaller`](#sympleserversignaller-2)  | Deleted constructor. |
| `void` | [`sendSdp`](#sendsdp-3) `virtual` `override` | Send an SDP offer or answer to the remote peer. |
| `void` | [`sendCandidate`](#sendcandidate-3) `virtual` `override` | Send an ICE candidate to the remote peer. |
| `void` | [`sendControl`](#sendcontrol-3) `virtual` `override` | Send a control message to the remote peer. |
| `void` | [`onMessage`](#onmessage-2)  |  |
| `void` | [`setRemoteAddress`](#setremoteaddress)  |  |
| `const std::string &` | [`localAddress`](#localaddress-1) `const` `inline` `nodiscard` |  |
| `const std::string &` | [`remoteAddress`](#remoteaddress-1) `const` `inline` `nodiscard` |  |

---

{#sympleserversignaller-1}

#### SympleServerSignaller

```cpp
SympleServerSignaller(smpl::Server & server, std::string localAddress, std::string remoteAddress = {})
```

Defined in src/webrtc/support/include/icy/webrtc/support/sympleserversignaller.h:42

---

{#sympleserversignaller-2}

#### SympleServerSignaller

```cpp
SympleServerSignaller(const SympleServerSignaller &) = delete
```

Defined in src/webrtc/support/include/icy/webrtc/support/sympleserversignaller.h:47

Deleted constructor.

---

{#sendsdp-3}

#### sendSdp

`virtual` `override`

```cpp
virtual void sendSdp(const std::string & peerId, const std::string & type, const std::string & sdp) override
```

Defined in src/webrtc/support/include/icy/webrtc/support/sympleserversignaller.h:50

Send an SDP offer or answer to the remote peer. 
#### Parameters
* `peerId` Remote peer identifier. 

* `type` "offer" or "answer". 

* `sdp` The SDP string.

---

{#sendcandidate-3}

#### sendCandidate

`virtual` `override`

```cpp
virtual void sendCandidate(const std::string & peerId, const std::string & candidate, const std::string & mid) override
```

Defined in src/webrtc/support/include/icy/webrtc/support/sympleserversignaller.h:54

Send an ICE candidate to the remote peer. 
#### Parameters
* `peerId` Remote peer identifier. 

* `candidate` The candidate string (from RTCIceCandidate). 

* `mid` The sdpMid value.

---

{#sendcontrol-3}

#### sendControl

`virtual` `override`

```cpp
virtual void sendControl(const std::string & peerId, const std::string & type, const std::string & reason = {}) override
```

Defined in src/webrtc/support/include/icy/webrtc/support/sympleserversignaller.h:58

Send a control message to the remote peer. 
#### Parameters
* `peerId` Remote peer identifier. 

* `type` Control type: "init", "accept", "reject", "hangup". 

* `reason` Optional reason string (for reject/hangup).

---

{#onmessage-2}

#### onMessage

```cpp
void onMessage(const json::Value & msg)
```

Defined in src/webrtc/support/include/icy/webrtc/support/sympleserversignaller.h:62

---

{#setremoteaddress}

#### setRemoteAddress

```cpp
void setRemoteAddress(std::string remoteAddress)
```

Defined in src/webrtc/support/include/icy/webrtc/support/sympleserversignaller.h:64

---

{#localaddress-1}

#### localAddress

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline const std::string & localAddress() const
```

Defined in src/webrtc/support/include/icy/webrtc/support/sympleserversignaller.h:65

---

{#remoteaddress-1}

#### remoteAddress

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline const std::string & remoteAddress() const
```

Defined in src/webrtc/support/include/icy/webrtc/support/sympleserversignaller.h:66

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `smpl::Server &` | [`_server`](#_server-2)  |  |
| `std::string` | [`_localAddress`](#_localaddress)  |  |
| `std::string` | [`_remoteAddress`](#_remoteaddress)  |  |
| `Synchronizer` | [`_dispatch`](#_dispatch-1)  |  |
| `std::mutex` | [`_mutex`](#_mutex-22)  |  |
| `std::deque< OutboundMessage >` | [`_pending`](#_pending-1)  |  |
| `bool` | [`_closing`](#_closing)  |  |

---

{#_server-2}

#### _server

```cpp
smpl::Server & _server
```

Defined in src/webrtc/support/include/icy/webrtc/support/sympleserversignaller.h:81

---

{#_localaddress}

#### _localAddress

```cpp
std::string _localAddress
```

Defined in src/webrtc/support/include/icy/webrtc/support/sympleserversignaller.h:82

---

{#_remoteaddress}

#### _remoteAddress

```cpp
std::string _remoteAddress
```

Defined in src/webrtc/support/include/icy/webrtc/support/sympleserversignaller.h:83

---

{#_dispatch-1}

#### _dispatch

```cpp
Synchronizer _dispatch
```

Defined in src/webrtc/support/include/icy/webrtc/support/sympleserversignaller.h:84

---

{#_mutex-22}

#### _mutex

```cpp
std::mutex _mutex
```

Defined in src/webrtc/support/include/icy/webrtc/support/sympleserversignaller.h:85

---

{#_pending-1}

#### _pending

```cpp
std::deque< OutboundMessage > _pending
```

Defined in src/webrtc/support/include/icy/webrtc/support/sympleserversignaller.h:86

---

{#_closing}

#### _closing

```cpp
bool _closing = false
```

Defined in src/webrtc/support/include/icy/webrtc/support/sympleserversignaller.h:87

### Private Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`send`](#send-21)  |  |
| `bool` | [`sendNow`](#sendnow)  |  |
| `void` | [`flushPending`](#flushpending-1)  |  |

---

{#send-21}

#### send

```cpp
void send(const std::string & action, const std::string & to, const json::Value & data = {})
```

Defined in src/webrtc/support/include/icy/webrtc/support/sympleserversignaller.h:75

---

{#sendnow}

#### sendNow

```cpp
bool sendNow(const std::string & to, const json::Value & msg)
```

Defined in src/webrtc/support/include/icy/webrtc/support/sympleserversignaller.h:78

---

{#flushpending-1}

#### flushPending

```cpp
void flushPending()
```

Defined in src/webrtc/support/include/icy/webrtc/support/sympleserversignaller.h:79

{#outboundmessage}

## OutboundMessage

```cpp
struct OutboundMessage
```

Defined in src/webrtc/support/include/icy/webrtc/support/sympleserversignaller.h:69

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`peerAddress`](#peeraddress-6)  |  |
| `json::Value` | [`msg`](#msg)  |  |

---

{#peeraddress-6}

#### peerAddress

```cpp
std::string peerAddress
```

Defined in src/webrtc/support/include/icy/webrtc/support/sympleserversignaller.h:71

---

{#msg}

#### msg

```cpp
json::Value msg
```

Defined in src/webrtc/support/include/icy/webrtc/support/sympleserversignaller.h:72

