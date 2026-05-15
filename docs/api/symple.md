{#symplemodule}

# symple

Realtime messaging protocol for presence, rooms, and WebRTC signalling.

### Namespaces

| Name | Description |
|------|-------------|
| [`smpl`](#smpl) | Symple protocol messages, peers, client, and server helpers. |

{#smpl}

# smpl

Symple protocol messages, peers, client, and server helpers.

### Classes

| Name | Description |
|------|-------------|
| [`Client`](#client-5) | Symple v4 client. |
| [`Command`](#command) | Symple command message with a node path and action verb. |
| [`Event`](#event-2) | Symple event message carrying a named occurrence with a timestamp. |
| [`Form`](#form) | Interactive form with pages, sections, and fields for command data exchange. |
| [`FormElement`](#formelement) | Base element within a Symple form. |
| [`FormField`](#formfield) | [Form](#form) input field with typed values and selectable options. |
| [`Message`](#message-10) | Base Symple protocol message with addressing, status, data, and notes. |
| [`Peer`](#peer-1) | Symple peer record containing identity, presence, and custom fields. |
| [`Presence`](#presence) | Symple presence message indicating a peer's online status. |
| [`Roster`](#roster-2) | The [Roster](#roster-2) provides a registry for active network peers indexed by session ID. |
| [`Server`](#server-10) | Symple v4 server. |
| [`ServerPeer`](#serverpeer) | Per-connection state for a connected Symple peer. |
| [`Address`](#address-13) | The [Address](#address-13) structure is an endpoint identifier for a peer on the network. The format is like so: user\|id |
| [`ClientState`](#clientstate-1) | [Client](#client-5) connection states. |

{#client-5}

## Client

```cpp
#include <icy/symple/client.h>
```

```cpp
class Client
```

Defined in src/symple/include/icy/symple/client.h:88

> **Inherits:** [`Stateful< ClientState >`](base.md#stateful), [`Signal< void(IPacket &)>`](base.md#signal)

Symple v4 client.

Connects to a Symple server over plain WebSocket (no Socket.IO). Handles authentication, presence, roster management, rooms, and message routing.

Usage: [smpl::Client](#client-5) client({ .host = "localhost", .port = 4500, .user = "alice", .name = "Alice" }); client.start();

client += packetSlot(&handler, &Handler::onMessage); client.Announce += slot(&handler, &Handler::onAnnounce);

Messages are emitted as polymorphic [IPacket](base.md#ipacket) via [PacketSignal](base.md#packetsignal):

* [smpl::Message](#message-10) (type "message")

* [smpl::Presence](#presence) (type "presence")

* [smpl::Event](#event-2) (type "event")

* [smpl::Command](#command) (type "command")

Use [packetSlot()](base.md#packetslot) with the specific type to filter.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`Announce`](#announce) | `variable` | Declared here |
| [`PeerConnected`](#peerconnected) | `variable` | Declared here |
| [`PeerDisconnected`](#peerdisconnected) | `variable` | Declared here |
| [`CreatePresence`](#createpresence) | `variable` | Declared here |
| [`Client`](#client-6) | `function` | Declared here |
| [`Client`](#client-7) | `function` | Declared here |
| [`start`](#start-16) | `function` | Declared here |
| [`stop`](#stop-13) | `function` | Declared here |
| [`send`](#send-17) | `function` | Declared here |
| [`send`](#send-18) | `function` | Declared here |
| [`respond`](#respond-1) | `function` | Declared here |
| [`sendPresence`](#sendpresence) | `function` | Declared here |
| [`sendPresence`](#sendpresence-1) | `function` | Declared here |
| [`joinRoom`](#joinroom) | `function` | Declared here |
| [`leaveRoom`](#leaveroom) | `function` | Declared here |
| [`isOnline`](#isonline) | `function` | Declared here |
| [`ourID`](#ourid) | `function` | Declared here |
| [`ourPeer`](#ourpeer) | `function` | Declared here |
| [`rooms`](#rooms) | `function` | Declared here |
| [`roster`](#roster) | `function` | Declared here |
| [`options`](#options-12) | `function` | Declared here |
| [`setOptions`](#setoptions) | `function` | Declared here |
| [`setError`](#seterror-6) | `function` | Declared here |
| [`createPresence`](#createpresence-1) | `function` | Declared here |
| [`onPresenceData`](#onpresencedata) | `function` | Declared here |
| [`_data`](#_data-1) | `variable` | Declared here |
| [`doConnect`](#doconnect) | `function` | Declared here |
| [`onTransportError`](#ontransporterror) | `function` | Declared here |
| [`onSocketRecv`](#onsocketrecv-9) | `function` | Declared here |
| [`onSocketClose`](#onsocketclose-6) | `function` | Declared here |
| [`onSocketError`](#onsocketerror-4) | `function` | Declared here |
| [`onWelcome`](#onwelcome) | `function` | Declared here |
| [`onServerMessage`](#onservermessage) | `function` | Declared here |
| [`startReconnect`](#startreconnect) | `function` | Declared here |
| [`reset`](#reset-15) | `function` | Declared here |
| [`syncDesiredRooms`](#syncdesiredrooms) | `function` | Declared here |
| [`sendJson`](#sendjson) | `function` | Declared here |
| [`buildUrl`](#buildurl) | `function` | Declared here |
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

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `Signal< void(const int &)>` | [`Announce`](#announce)  | Authentication response status (200 = success, 401 = failed). |
| `Signal< void(Peer &)>` | [`PeerConnected`](#peerconnected)  | A peer has come online. |
| `Signal< void(Peer &)>` | [`PeerDisconnected`](#peerdisconnected)  | A peer has gone offline. |
| `Signal< void(Peer &)>` | [`CreatePresence`](#createpresence)  | Modify the outgoing peer object before presence broadcast. |

---

{#announce}

#### Announce

```cpp
Signal< void(const int &)> Announce
```

Defined in src/symple/include/icy/symple/client.h:174

Authentication response status (200 = success, 401 = failed).

---

{#peerconnected}

#### PeerConnected

```cpp
Signal< void(Peer &)> PeerConnected
```

Defined in src/symple/include/icy/symple/client.h:177

A peer has come online.

---

{#peerdisconnected}

#### PeerDisconnected

```cpp
Signal< void(Peer &)> PeerDisconnected
```

Defined in src/symple/include/icy/symple/client.h:180

A peer has gone offline.

---

{#createpresence}

#### CreatePresence

```cpp
Signal< void(Peer &)> CreatePresence
```

Defined in src/symple/include/icy/symple/client.h:183

Modify the outgoing peer object before presence broadcast.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Client`](#client-6)  |  |
|  | [`Client`](#client-7)  | Default constructor with default [Options](#options-13). |
| `void` | [`start`](#start-16)  | Start the Symple client. |
| `void` | [`stop`](#stop-13)  | Stop the Symple client. |
| `ssize_t` | [`send`](#send-17) `virtual` | Send a Symple message. Sets the `from` field automatically. |
| `ssize_t` | [`send`](#send-18) `virtual` | Send a string message (parsed as JSON). |
| `ssize_t` | [`respond`](#respond-1) `virtual` | Swap to/from and send. |
| `ssize_t` | [`sendPresence`](#sendpresence) `virtual` | Broadcast presence to joined rooms. |
| `ssize_t` | [`sendPresence`](#sendpresence-1) `virtual` | Send directed presence to a specific peer. |
| `int` | [`joinRoom`](#joinroom) `virtual` | Join a room on the server. |
| `int` | [`leaveRoom`](#leaveroom) `virtual` | Leave a room on the server. |
| `bool` | [`isOnline`](#isonline) `const` `nodiscard` | Return true if in Online state. |
| `std::string` | [`ourID`](#ourid) `const` `nodiscard` | Return the session ID assigned by the server. |
| `Peer *` | [`ourPeer`](#ourpeer) `nodiscard` | Return the local peer object (null if offline). |
| `StringVec` | [`rooms`](#rooms) `const` `nodiscard` | Return joined rooms. |
| `Roster &` | [`roster`](#roster) `nodiscard` | Return the roster of online peers. |
| `const Options &` | [`options`](#options-12) `const` `nodiscard` | Return the current client options. |
| `void` | [`setOptions`](#setoptions)  | Replace the client options while the client is closed. This is the only supported way to reconfigure a default-constructed client. |
| `void` | [`setError`](#seterror-6)  | Set an error and transition to [Error](base.md#error) state. |

---

{#client-6}

#### Client

```cpp
Client(const Options & options, uv::Loop * loop = uv::defaultLoop())
```

Defined in src/symple/include/icy/symple/client.h:110

---

{#client-7}

#### Client

```cpp
Client()
```

Defined in src/symple/include/icy/symple/client.h:112

Default constructor with default [Options](#options-13).

---

{#start-16}

#### start

```cpp
void start()
```

Defined in src/symple/include/icy/symple/client.h:116

Start the Symple client.

---

{#stop-13}

#### stop

```cpp
void stop()
```

Defined in src/symple/include/icy/symple/client.h:119

Stop the Symple client.

---

{#send-17}

#### send

`virtual`

```cpp
virtual ssize_t send(Message & message)
```

Defined in src/symple/include/icy/symple/client.h:123

Send a Symple message. Sets the `from` field automatically.

---

{#send-18}

#### send

`virtual`

```cpp
virtual ssize_t send(const std::string & message)
```

Defined in src/symple/include/icy/symple/client.h:126

Send a string message (parsed as JSON).

---

{#respond-1}

#### respond

`virtual`

```cpp
virtual ssize_t respond(Message & message)
```

Defined in src/symple/include/icy/symple/client.h:129

Swap to/from and send.

---

{#sendpresence}

#### sendPresence

`virtual`

```cpp
virtual ssize_t sendPresence(bool probe = false)
```

Defined in src/symple/include/icy/symple/client.h:132

Broadcast presence to joined rooms.

---

{#sendpresence-1}

#### sendPresence

`virtual`

```cpp
virtual ssize_t sendPresence(const Address & to, bool probe = false)
```

Defined in src/symple/include/icy/symple/client.h:135

Send directed presence to a specific peer.

---

{#joinroom}

#### joinRoom

`virtual`

```cpp
virtual int joinRoom(const std::string & room)
```

Defined in src/symple/include/icy/symple/client.h:138

Join a room on the server.

---

{#leaveroom}

#### leaveRoom

`virtual`

```cpp
virtual int leaveRoom(const std::string & room)
```

Defined in src/symple/include/icy/symple/client.h:141

Leave a room on the server.

---

{#isonline}

#### isOnline

`const` `nodiscard`

```cpp
[[nodiscard]] bool isOnline() const
```

Defined in src/symple/include/icy/symple/client.h:144

Return true if in Online state.

---

{#ourid}

#### ourID

`const` `nodiscard`

```cpp
[[nodiscard]] std::string ourID() const
```

Defined in src/symple/include/icy/symple/client.h:147

Return the session ID assigned by the server.

---

{#ourpeer}

#### ourPeer

`nodiscard`

```cpp
[[nodiscard]] Peer * ourPeer()
```

Defined in src/symple/include/icy/symple/client.h:150

Return the local peer object (null if offline).

---

{#rooms}

#### rooms

`const` `nodiscard`

```cpp
[[nodiscard]] StringVec rooms() const
```

Defined in src/symple/include/icy/symple/client.h:153

Return joined rooms.

---

{#roster}

#### roster

`nodiscard`

```cpp
[[nodiscard]] Roster & roster()
```

Defined in src/symple/include/icy/symple/client.h:156

Return the roster of online peers.

---

{#options-12}

#### options

`const` `nodiscard`

```cpp
[[nodiscard]] const Options & options() const
```

Defined in src/symple/include/icy/symple/client.h:159

Return the current client options.

---

{#setoptions}

#### setOptions

```cpp
void setOptions(Options options)
```

Defined in src/symple/include/icy/symple/client.h:164

Replace the client options while the client is closed. This is the only supported way to reconfigure a default-constructed client. 
#### Exceptions
* `std::logic_error` if the client is currently connecting, online, or reconnecting.

---

{#seterror-6}

#### setError

```cpp
void setError(const std::string & error)
```

Defined in src/symple/include/icy/symple/client.h:167

Set an error and transition to [Error](base.md#error) state.

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`createPresence`](#createpresence-1) `virtual` | Underlying connection state changed. Inherits StateChange from [Stateful<ClientState>](base.md#stateful). |
| `void` | [`onPresenceData`](#onpresencedata) `virtual` |  |

---

{#createpresence-1}

#### createPresence

`virtual`

```cpp
virtual void createPresence(Presence & p)
```

Defined in src/symple/include/icy/symple/client.h:189

Underlying connection state changed. Inherits StateChange from [Stateful<ClientState>](base.md#stateful).

---

{#onpresencedata}

#### onPresenceData

`virtual`

```cpp
virtual void onPresenceData(const json::Value & data, bool whiny = false)
```

Defined in src/symple/include/icy/symple/client.h:190

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::unique_ptr< ClientData >` | [`_data`](#_data-1)  |  |

---

{#_data-1}

#### _data

```cpp
std::unique_ptr< ClientData > _data
```

Defined in src/symple/include/icy/symple/client.h:209

### Private Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`doConnect`](#doconnect)  |  |
| `void` | [`onTransportError`](#ontransporterror)  |  |
| `void` | [`onSocketRecv`](#onsocketrecv-9)  |  |
| `void` | [`onSocketClose`](#onsocketclose-6)  |  |
| `void` | [`onSocketError`](#onsocketerror-4)  |  |
| `void` | [`onWelcome`](#onwelcome)  |  |
| `void` | [`onServerMessage`](#onservermessage)  |  |
| `void` | [`startReconnect`](#startreconnect)  |  |
| `void` | [`reset`](#reset-15)  |  |
| `void` | [`syncDesiredRooms`](#syncdesiredrooms)  |  |
| `ssize_t` | [`sendJson`](#sendjson)  |  |
| `std::string` | [`buildUrl`](#buildurl) `const` |  |

---

{#doconnect}

#### doConnect

```cpp
void doConnect()
```

Defined in src/symple/include/icy/symple/client.h:195

---

{#ontransporterror}

#### onTransportError

```cpp
void onTransportError(const icy::Error & error)
```

Defined in src/symple/include/icy/symple/client.h:196

---

{#onsocketrecv-9}

#### onSocketRecv

```cpp
void onSocketRecv(const std::string & data)
```

Defined in src/symple/include/icy/symple/client.h:197

---

{#onsocketclose-6}

#### onSocketClose

```cpp
void onSocketClose()
```

Defined in src/symple/include/icy/symple/client.h:198

---

{#onsocketerror-4}

#### onSocketError

```cpp
void onSocketError(const std::string & error)
```

Defined in src/symple/include/icy/symple/client.h:199

---

{#onwelcome}

#### onWelcome

```cpp
void onWelcome(const json::Value & msg)
```

Defined in src/symple/include/icy/symple/client.h:200

---

{#onservermessage}

#### onServerMessage

```cpp
void onServerMessage(const json::Value & msg)
```

Defined in src/symple/include/icy/symple/client.h:201

---

{#startreconnect}

#### startReconnect

```cpp
void startReconnect()
```

Defined in src/symple/include/icy/symple/client.h:202

---

{#reset-15}

#### reset

```cpp
void reset()
```

Defined in src/symple/include/icy/symple/client.h:203

---

{#syncdesiredrooms}

#### syncDesiredRooms

```cpp
void syncDesiredRooms()
```

Defined in src/symple/include/icy/symple/client.h:204

---

{#sendjson}

#### sendJson

```cpp
ssize_t sendJson(const json::Value & msg)
```

Defined in src/symple/include/icy/symple/client.h:206

---

{#buildurl}

#### buildUrl

`const`

```cpp
std::string buildUrl() const
```

Defined in src/symple/include/icy/symple/client.h:207

{#options-13}

## Options

```cpp
#include <icy/symple/client.h>
```

```cpp
struct Options
```

Defined in src/symple/include/icy/symple/client.h:93

Connection and authentication options for the Symple client.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`host`](#host-2) | `variable` | Declared here |
| [`port`](#port-2) | `variable` | Declared here |
| [`secure`](#secure-1) | `variable` | Declared here |
| [`reconnection`](#reconnection) | `variable` | Declared here |
| [`reconnectAttempts`](#reconnectattempts) | `variable` | Declared here |
| [`reconnectDelay`](#reconnectdelay) | `variable` | Declared here |
| [`user`](#user-1) | `variable` | Declared here |
| [`name`](#name-12) | `variable` | Declared here |
| [`type`](#type-19) | `variable` | Declared here |
| [`token`](#token) | `variable` | Declared here |
| [`Options`](#options-14) | `function` | Declared here |

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`host`](#host-2)  |  |
| `uint16_t` | [`port`](#port-2)  |  |
| `bool` | [`secure`](#secure-1)  | Use wss:// instead of ws://. |
| `bool` | [`reconnection`](#reconnection)  | Auto-reconnect on disconnect. |
| `int` | [`reconnectAttempts`](#reconnectattempts)  | 0 = unlimited |
| `int` | [`reconnectDelay`](#reconnectdelay)  | Milliseconds between retries. |
| `std::string` | [`user`](#user-1)  | User identifier (required). |
| `std::string` | [`name`](#name-12)  | Display name. |
| `std::string` | [`type`](#type-19)  | [Peer](#peer-1) type. |
| `std::string` | [`token`](#token)  | Auth token (optional). |

---

{#host-2}

#### host

```cpp
std::string host = "127.0.0.1"
```

Defined in src/symple/include/icy/symple/client.h:95

---

{#port-2}

#### port

```cpp
uint16_t port = 4500
```

Defined in src/symple/include/icy/symple/client.h:96

---

{#secure-1}

#### secure

```cpp
bool secure = false
```

Defined in src/symple/include/icy/symple/client.h:97

Use wss:// instead of ws://.

---

{#reconnection}

#### reconnection

```cpp
bool reconnection = true
```

Defined in src/symple/include/icy/symple/client.h:98

Auto-reconnect on disconnect.

---

{#reconnectattempts}

#### reconnectAttempts

```cpp
int reconnectAttempts = 0
```

Defined in src/symple/include/icy/symple/client.h:99

0 = unlimited

---

{#reconnectdelay}

#### reconnectDelay

```cpp
int reconnectDelay = 3000
```

Defined in src/symple/include/icy/symple/client.h:100

Milliseconds between retries.

---

{#user-1}

#### user

```cpp
std::string user
```

Defined in src/symple/include/icy/symple/client.h:102

User identifier (required).

---

{#name-12}

#### name

```cpp
std::string name
```

Defined in src/symple/include/icy/symple/client.h:103

Display name.

---

{#type-19}

#### type

```cpp
std::string type
```

Defined in src/symple/include/icy/symple/client.h:104

[Peer](#peer-1) type.

---

{#token}

#### token

```cpp
std::string token
```

Defined in src/symple/include/icy/symple/client.h:105

Auth token (optional).

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Options`](#options-14)  | Defaulted constructor. |

---

{#options-14}

#### Options

```cpp
Options() = default
```

Defined in src/symple/include/icy/symple/client.h:107

Defaulted constructor.

{#clientdata}

## ClientData

```cpp
#include <src/symple/src/client/detail.h>
```

```cpp
struct ClientData
```

Defined in src/symple/src/client/detail.h:13

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`options`](#options-15) | `variable` | Declared here |
| [`loop`](#loop-8) | `variable` | Declared here |
| [`ws`](#ws-1) | `variable` | Declared here |
| [`roster`](#roster-1) | `variable` | Declared here |
| [`ourID`](#ourid-1) | `variable` | Declared here |
| [`currentRooms`](#currentrooms) | `variable` | Declared here |
| [`desiredRooms`](#desiredrooms) | `variable` | Declared here |
| [`pendingJoins`](#pendingjoins) | `variable` | Declared here |
| [`pendingLeaves`](#pendingleaves) | `variable` | Declared here |
| [`announceStatus`](#announcestatus) | `variable` | Declared here |
| [`reconnectTimer`](#reconnecttimer) | `variable` | Declared here |
| [`reconnectCount`](#reconnectcount) | `variable` | Declared here |
| [`wasOnline`](#wasonline) | `variable` | Declared here |
| [`closing`](#closing-2) | `variable` | Declared here |
| [`remoteShutdown`](#remoteshutdown) | `variable` | Declared here |
| [`ClientData`](#clientdata-1) | `function` | Declared here |

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `Options` | [`options`](#options-15)  |  |
| `uv::Loop *` | [`loop`](#loop-8)  |  |
| `http::ClientConnection::Ptr` | [`ws`](#ws-1)  |  |
| `Roster` | [`roster`](#roster-1)  |  |
| `std::string` | [`ourID`](#ourid-1)  |  |
| `std::unordered_set< std::string >` | [`currentRooms`](#currentrooms)  | Authoritative rooms from welcome / acks. |
| `std::unordered_set< std::string >` | [`desiredRooms`](#desiredrooms)  | Rooms the client wants persisted across reconnects. |
| `std::unordered_set< std::string >` | [`pendingJoins`](#pendingjoins)  | Join requests sent but not yet acknowledged. |
| `std::unordered_set< std::string >` | [`pendingLeaves`](#pendingleaves)  | Leave requests sent but not yet acknowledged. |
| `int` | [`announceStatus`](#announcestatus)  |  |
| `Timer` | [`reconnectTimer`](#reconnecttimer)  |  |
| `int` | [`reconnectCount`](#reconnectcount)  |  |
| `bool` | [`wasOnline`](#wasonline)  |  |
| `bool` | [`closing`](#closing-2)  |  |
| `bool` | [`remoteShutdown`](#remoteshutdown)  |  |

---

{#options-15}

#### options

```cpp
Options options
```

Defined in src/symple/src/client/detail.h:22

---

{#loop-8}

#### loop

```cpp
uv::Loop * loop
```

Defined in src/symple/src/client/detail.h:23

---

{#ws-1}

#### ws

```cpp
http::ClientConnection::Ptr ws
```

Defined in src/symple/src/client/detail.h:24

---

{#roster-1}

#### roster

```cpp
Roster roster
```

Defined in src/symple/src/client/detail.h:25

---

{#ourid-1}

#### ourID

```cpp
std::string ourID
```

Defined in src/symple/src/client/detail.h:26

---

{#currentrooms}

#### currentRooms

```cpp
std::unordered_set< std::string > currentRooms
```

Defined in src/symple/src/client/detail.h:27

Authoritative rooms from welcome / acks.

---

{#desiredrooms}

#### desiredRooms

```cpp
std::unordered_set< std::string > desiredRooms
```

Defined in src/symple/src/client/detail.h:28

Rooms the client wants persisted across reconnects.

---

{#pendingjoins}

#### pendingJoins

```cpp
std::unordered_set< std::string > pendingJoins
```

Defined in src/symple/src/client/detail.h:29

Join requests sent but not yet acknowledged.

---

{#pendingleaves}

#### pendingLeaves

```cpp
std::unordered_set< std::string > pendingLeaves
```

Defined in src/symple/src/client/detail.h:30

Leave requests sent but not yet acknowledged.

---

{#announcestatus}

#### announceStatus

```cpp
int announceStatus = 0
```

Defined in src/symple/src/client/detail.h:31

---

{#reconnecttimer}

#### reconnectTimer

```cpp
Timer reconnectTimer
```

Defined in src/symple/src/client/detail.h:32

---

{#reconnectcount}

#### reconnectCount

```cpp
int reconnectCount = 0
```

Defined in src/symple/src/client/detail.h:33

---

{#wasonline}

#### wasOnline

```cpp
bool wasOnline = false
```

Defined in src/symple/src/client/detail.h:34

---

{#closing-2}

#### closing

```cpp
bool closing = false
```

Defined in src/symple/src/client/detail.h:35

---

{#remoteshutdown}

#### remoteShutdown

```cpp
bool remoteShutdown = false
```

Defined in src/symple/src/client/detail.h:36

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`ClientData`](#clientdata-1) `inline` `explicit` |  |

---

{#clientdata-1}

#### ClientData

`inline` `explicit`

```cpp
inline explicit ClientData(Options opts, uv::Loop * targetLoop)
```

Defined in src/symple/src/client/detail.h:15

{#command}

## Command

```cpp
#include <icy/symple/command.h>
```

```cpp
class Command
```

Defined in src/symple/include/icy/symple/command.h:28

> **Inherits:** [`Message`](#message-10)

Symple command message with a node path and action verb.

The `[node](#node)` field is a colon-delimited path (e.g. "camera:zoom:in"). Individual path segments can be retrieved with [param()](#param).

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`Command`](#command-1) | `function` | Declared here |
| [`Command`](#command-2) | `function` | Declared here |
| [`Command`](#command-3) | `function` | Declared here |
| [`node`](#node) | `function` | Declared here |
| [`action`](#action-2) | `function` | Declared here |
| [`setNode`](#setnode) | `function` | Declared here |
| [`setAction`](#setaction) | `function` | Declared here |
| [`valid`](#valid-11) | `function` | Declared here |
| [`param`](#param) | `function` | Declared here |
| [`params`](#params) | `function` | Declared here |
| [`matches`](#matches-3) | `function` | Declared here |
| [`Message`](#message-11) | `function` | Inherited from [`Message`](#message-10) |
| [`Message`](#message-12) | `function` | Inherited from [`Message`](#message-10) |
| [`Message`](#message-13) | `function` | Inherited from [`Message`](#message-10) |
| [`clone`](#clone-18) | `function` | Inherited from [`Message`](#message-10) |
| [`valid`](#valid-15) | `function` | Inherited from [`Message`](#message-10) |
| [`clear`](#clear-4) | `function` | Inherited from [`Message`](#message-10) |
| [`clearData`](#cleardata) | `function` | Inherited from [`Message`](#message-10) |
| [`clearNotes`](#clearnotes) | `function` | Inherited from [`Message`](#message-10) |
| [`type`](#type-22) | `function` | Inherited from [`Message`](#message-10) |
| [`id`](#id-7) | `function` | Inherited from [`Message`](#message-10) |
| [`to`](#to) | `function` | Inherited from [`Message`](#message-10) |
| [`from`](#from) | `function` | Inherited from [`Message`](#message-10) |
| [`toUser`](#touser) | `function` | Inherited from [`Message`](#message-10) |
| [`toId`](#toid) | `function` | Inherited from [`Message`](#message-10) |
| [`fromUser`](#fromuser) | `function` | Inherited from [`Message`](#message-10) |
| [`fromId`](#fromid) | `function` | Inherited from [`Message`](#message-10) |
| [`status`](#status-3) | `function` | Inherited from [`Message`](#message-10) |
| [`setType`](#settype-2) | `function` | Inherited from [`Message`](#message-10) |
| [`setTo`](#setto) | `function` | Inherited from [`Message`](#message-10) |
| [`setTo`](#setto-1) | `function` | Inherited from [`Message`](#message-10) |
| [`setTo`](#setto-2) | `function` | Inherited from [`Message`](#message-10) |
| [`setFrom`](#setfrom) | `function` | Inherited from [`Message`](#message-10) |
| [`setFrom`](#setfrom-1) | `function` | Inherited from [`Message`](#message-10) |
| [`setFrom`](#setfrom-2) | `function` | Inherited from [`Message`](#message-10) |
| [`setStatus`](#setstatus-1) | `function` | Inherited from [`Message`](#message-10) |
| [`notes`](#notes) | `function` | Inherited from [`Message`](#message-10) |
| [`setNote`](#setnote) | `function` | Inherited from [`Message`](#message-10) |
| [`addNote`](#addnote) | `function` | Inherited from [`Message`](#message-10) |
| [`data`](#data-4) | `function` | Inherited from [`Message`](#message-10) |
| [`data`](#data-5) | `function` | Inherited from [`Message`](#message-10) |
| [`setData`](#setdata) | `function` | Inherited from [`Message`](#message-10) |
| [`setData`](#setdata-1) | `function` | Inherited from [`Message`](#message-10) |
| [`setData`](#setdata-2) | `function` | Inherited from [`Message`](#message-10) |
| [`setData`](#setdata-3) | `function` | Inherited from [`Message`](#message-10) |
| [`setData`](#setdata-4) | `function` | Inherited from [`Message`](#message-10) |
| [`removeData`](#removedata) | `function` | Inherited from [`Message`](#message-10) |
| [`hasData`](#hasdata) | `function` | Inherited from [`Message`](#message-10) |
| [`read`](#read-13) | `function` | Inherited from [`Message`](#message-10) |
| [`read`](#read-14) | `function` | Inherited from [`Message`](#message-10) |
| [`write`](#write-28) | `function` | Inherited from [`Message`](#message-10) |
| [`isRequest`](#isrequest) | `function` | Inherited from [`Message`](#message-10) |
| [`size`](#size-12) | `function` | Inherited from [`Message`](#message-10) |
| [`print`](#print-16) | `function` | Inherited from [`Message`](#message-10) |
| [`className`](#classname-8) | `function` | Inherited from [`Message`](#message-10) |
| [`data`](#data-6) | `function` | Inherited from [`Message`](#message-10) |
| [`hasData`](#hasdata-1) | `function` | Inherited from [`Message`](#message-10) |
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

### Inherited from [`Message`](#message-10)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`Message`](#message-11)  | Constructs a new message with a random ID and type set to "message". |
| `function` | [`Message`](#message-12)  | Constructs a message from a JSON value; sets missing id/type fields. |
| `function` | [`Message`](#message-13)  | Copy constructor; preserves or generates id/type fields. |
| `function` | [`clone`](#clone-18) `virtual` `const` `override` | Returns a heap-allocated copy of this message. |
| `function` | [`valid`](#valid-15) `virtual` `const` | Returns true if the message has both `[type](#type-22)` and `[id](#id-7)` fields. |
| `function` | [`clear`](#clear-4) `virtual` | Clears all JSON fields from this message. |
| `function` | [`clearData`](#cleardata) `virtual` | Clears the `[data](#data-4)` sub-object. |
| `function` | [`clearNotes`](#clearnotes) `virtual` | Clears the `[notes](#notes)` array. |
| `function` | [`type`](#type-22) `const` `nodiscard` | Returns the message type string (defaults to "message"). |
| `function` | [`id`](#id-7) `const` `nodiscard` | Returns the message ID string. |
| `function` | [`to`](#to) `const` `nodiscard` | Returns the recipient address parsed from the `[to](#to)` field. |
| `function` | [`from`](#from) `const` `nodiscard` | Returns the sender address parsed from the `[from](#from)` field. |
| `function` | [`toUser`](#touser) `const` `nodiscard` | Returns the user component of the `[to](#to)` address without constructing an [Address](#address-13). |
| `function` | [`toId`](#toid) `const` `nodiscard` | Returns the id component of the `[to](#to)` address without constructing an [Address](#address-13). |
| `function` | [`fromUser`](#fromuser) `const` `nodiscard` | Returns the user component of the `[from](#from)` address without constructing an [Address](#address-13). |
| `function` | [`fromId`](#fromid) `const` `nodiscard` | Returns the id component of the `[from](#from)` address without constructing an [Address](#address-13). |
| `function` | [`status`](#status-3) `const` `nodiscard` | Returns the HTTP status code, or -1 if not set. |
| `function` | [`setType`](#settype-2)  | Sets the message type field. |
| `function` | [`setTo`](#setto)  | Sets the `[to](#to)` field from a peer's address. |
| `function` | [`setTo`](#setto-1)  | Sets the `[to](#to)` field from an address object. |
| `function` | [`setTo`](#setto-2)  | Sets the `[to](#to)` field from an address string. |
| `function` | [`setFrom`](#setfrom)  | Sets the `[from](#from)` field from a peer's address. |
| `function` | [`setFrom`](#setfrom-1)  | Sets the `[from](#from)` field from an address object. |
| `function` | [`setFrom`](#setfrom-2)  | Sets the `[from](#from)` field from an address string. |
| `function` | [`setStatus`](#setstatus-1)  | HTTP status codes are used to describe the message response. Throws std::invalid_argument if code is outside [101, 504]. |
| `function` | [`notes`](#notes)  | Returns a reference to the `[notes](#notes)` JSON array. |
| `function` | [`setNote`](#setnote)  | Replaces all notes with a single note. |
| `function` | [`addNote`](#addnote)  | Appends a note to the `[notes](#notes)` array. |
| `function` | [`data`](#data-4) `const` | Returns a copy of the named data field. |
| `function` | [`data`](#data-5)  | Returns a reference to the named data field (creates it if absent). |
| `function` | [`setData`](#setdata)  | Creates or replaces a named data field; returns a reference to it. |
| `function` | [`setData`](#setdata-1)  | Sets a named data field to a C-string value. |
| `function` | [`setData`](#setdata-2)  | Sets a named data field to a string value. |
| `function` | [`setData`](#setdata-3)  | Sets a named data field to a JSON value. |
| `function` | [`setData`](#setdata-4)  | Sets a named data field to an integer value. |
| `function` | [`removeData`](#removedata)  | Removes a named field from the `[data](#data-4)` sub-object. |
| `function` | [`hasData`](#hasdata)  | Returns true if the named field exists in the `[data](#data-4)` sub-object. |
| `function` | [`read`](#read-13) `virtual` `override` | Deserialises the message from a raw buffer. |
| `function` | [`read`](#read-14) `virtual` | Deserialises the message from a JSON string. |
| `function` | [`write`](#write-28) `virtual` `const` `override` | Serialises the message as JSON into a buffer. |
| `function` | [`isRequest`](#isrequest) `const` `nodiscard` | Returns true if no status code has been set (i.e. [status()](#status-3) == -1). |
| `function` | [`size`](#size-12) `virtual` `const` `override` | Returns the serialised JSON size in bytes. |
| `function` | [`print`](#print-16) `virtual` `const` `override` | Pretty-prints the message JSON to the given stream. |
| `function` | [`className`](#classname-8) `virtual` `const` `inline` `override` | Returns the class name of this packet type for logging and diagnostics. |
| `function` | [`data`](#data-6) `virtual` `const` `inline` | The packet data pointer for buffered packets. |
| `function` | [`hasData`](#hasdata-1) `virtual` `const` `inline` | Returns true if the packet has a non-null data pointer. |

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

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Command`](#command-1)  | Constructs an empty command with type set to "command". |
|  | [`Command`](#command-2)  | Constructs a command from a JSON value. |
|  | [`Command`](#command-3)  | Copy constructor. |
| `std::string` | [`node`](#node) `const` `nodiscard` | Returns the colon-delimited node path (e.g. "camera:zoom"). |
| `std::string` | [`action`](#action-2) `const` `nodiscard` | Returns the action verb (defaults to "execute"). |
| `void` | [`setNode`](#setnode)  | Sets the node path field. |
| `void` | [`setAction`](#setaction)  | Sets the action verb field. |
| `bool` | [`valid`](#valid-11) `virtual` `const` | Returns true if the base message is valid and the `[node](#node)` field is set. |
| `std::string` | [`param`](#param) `const` `nodiscard` | Returns the nth colon-separated segment of the node path (1-based). Throws std::out_of_range if n exceeds the number of segments. |
| `std::vector< std::string >` | [`params`](#params)  | Returns all colon-separated segments of the node path. |
| `bool` | [`matches`](#matches-3) `const` `nodiscard` | Returns true if the node path matches the given pattern. Uses wildcard node matching via [util::matchNodes](base.md#matchnodes). |

---

{#command-1}

#### Command

```cpp
Command()
```

Defined in src/symple/include/icy/symple/command.h:32

Constructs an empty command with type set to "command".

---

{#command-2}

#### Command

```cpp
Command(const json::Value & root)
```

Defined in src/symple/include/icy/symple/command.h:36

Constructs a command from a JSON value. 
#### Parameters
* `root` JSON object to initialise from.

---

{#command-3}

#### Command

```cpp
Command(const Command & root)
```

Defined in src/symple/include/icy/symple/command.h:40

Copy constructor. 
#### Parameters
* `root` Source command.

---

{#node}

#### node

`const` `nodiscard`

```cpp
[[nodiscard]] std::string node() const
```

Defined in src/symple/include/icy/symple/command.h:45

Returns the colon-delimited node path (e.g. "camera:zoom").

---

{#action-2}

#### action

`const` `nodiscard`

```cpp
[[nodiscard]] std::string action() const
```

Defined in src/symple/include/icy/symple/command.h:48

Returns the action verb (defaults to "execute").

---

{#setnode}

#### setNode

```cpp
void setNode(std::string_view node)
```

Defined in src/symple/include/icy/symple/command.h:52

Sets the node path field. 
#### Parameters
* `node` Colon-delimited node path string.

---

{#setaction}

#### setAction

```cpp
void setAction(std::string_view action)
```

Defined in src/symple/include/icy/symple/command.h:56

Sets the action verb field. 
#### Parameters
* `action` Action string (e.g. "execute", "get", "set").

---

{#valid-11}

#### valid

`virtual` `const`

```cpp
virtual bool valid() const
```

Defined in src/symple/include/icy/symple/command.h:59

Returns true if the base message is valid and the `[node](#node)` field is set.

##### Reimplements

- [`valid`](#valid-15)

---

{#param}

#### param

`const` `nodiscard`

```cpp
[[nodiscard]] std::string param(int n) const
```

Defined in src/symple/include/icy/symple/command.h:65

Returns the nth colon-separated segment of the node path (1-based). Throws std::out_of_range if n exceeds the number of segments. 
#### Parameters
* `n` 1-based segment index. 

#### Returns
The nth path segment.

---

{#params}

#### params

```cpp
std::vector< std::string > params()
```

Defined in src/symple/include/icy/symple/command.h:69

Returns all colon-separated segments of the node path. 
#### Returns
Vector of path segment strings.

---

{#matches-3}

#### matches

`const` `nodiscard`

```cpp
[[nodiscard]] bool matches(std::string_view xnode) const
```

Defined in src/symple/include/icy/symple/command.h:74

Returns true if the node path matches the given pattern. Uses wildcard node matching via [util::matchNodes](base.md#matchnodes). 
#### Parameters
* `xnode` Pattern to match against (colon-delimited, supports wildcards).

{#event-2}

## Event

```cpp
#include <icy/symple/event.h>
```

```cpp
class Event
```

Defined in src/symple/include/icy/symple/event.h:29

> **Inherits:** [`Message`](#message-10)

Symple event message carrying a named occurrence with a timestamp.

The `[name](#name-13)` field identifies the event. The `[time](base.md#time-3)` field is stored as a Unix timestamp (seconds since epoch).

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`Event`](#event-3) | `function` | Declared here |
| [`Event`](#event-4) | `function` | Declared here |
| [`Event`](#event-5) | `function` | Declared here |
| [`valid`](#valid-12) | `function` | Declared here |
| [`name`](#name-13) | `function` | Declared here |
| [`time`](#time-4) | `function` | Declared here |
| [`setName`](#setname-2) | `function` | Declared here |
| [`setTime`](#settime) | `function` | Declared here |
| [`Message`](#message-11) | `function` | Inherited from [`Message`](#message-10) |
| [`Message`](#message-12) | `function` | Inherited from [`Message`](#message-10) |
| [`Message`](#message-13) | `function` | Inherited from [`Message`](#message-10) |
| [`clone`](#clone-18) | `function` | Inherited from [`Message`](#message-10) |
| [`valid`](#valid-15) | `function` | Inherited from [`Message`](#message-10) |
| [`clear`](#clear-4) | `function` | Inherited from [`Message`](#message-10) |
| [`clearData`](#cleardata) | `function` | Inherited from [`Message`](#message-10) |
| [`clearNotes`](#clearnotes) | `function` | Inherited from [`Message`](#message-10) |
| [`type`](#type-22) | `function` | Inherited from [`Message`](#message-10) |
| [`id`](#id-7) | `function` | Inherited from [`Message`](#message-10) |
| [`to`](#to) | `function` | Inherited from [`Message`](#message-10) |
| [`from`](#from) | `function` | Inherited from [`Message`](#message-10) |
| [`toUser`](#touser) | `function` | Inherited from [`Message`](#message-10) |
| [`toId`](#toid) | `function` | Inherited from [`Message`](#message-10) |
| [`fromUser`](#fromuser) | `function` | Inherited from [`Message`](#message-10) |
| [`fromId`](#fromid) | `function` | Inherited from [`Message`](#message-10) |
| [`status`](#status-3) | `function` | Inherited from [`Message`](#message-10) |
| [`setType`](#settype-2) | `function` | Inherited from [`Message`](#message-10) |
| [`setTo`](#setto) | `function` | Inherited from [`Message`](#message-10) |
| [`setTo`](#setto-1) | `function` | Inherited from [`Message`](#message-10) |
| [`setTo`](#setto-2) | `function` | Inherited from [`Message`](#message-10) |
| [`setFrom`](#setfrom) | `function` | Inherited from [`Message`](#message-10) |
| [`setFrom`](#setfrom-1) | `function` | Inherited from [`Message`](#message-10) |
| [`setFrom`](#setfrom-2) | `function` | Inherited from [`Message`](#message-10) |
| [`setStatus`](#setstatus-1) | `function` | Inherited from [`Message`](#message-10) |
| [`notes`](#notes) | `function` | Inherited from [`Message`](#message-10) |
| [`setNote`](#setnote) | `function` | Inherited from [`Message`](#message-10) |
| [`addNote`](#addnote) | `function` | Inherited from [`Message`](#message-10) |
| [`data`](#data-4) | `function` | Inherited from [`Message`](#message-10) |
| [`data`](#data-5) | `function` | Inherited from [`Message`](#message-10) |
| [`setData`](#setdata) | `function` | Inherited from [`Message`](#message-10) |
| [`setData`](#setdata-1) | `function` | Inherited from [`Message`](#message-10) |
| [`setData`](#setdata-2) | `function` | Inherited from [`Message`](#message-10) |
| [`setData`](#setdata-3) | `function` | Inherited from [`Message`](#message-10) |
| [`setData`](#setdata-4) | `function` | Inherited from [`Message`](#message-10) |
| [`removeData`](#removedata) | `function` | Inherited from [`Message`](#message-10) |
| [`hasData`](#hasdata) | `function` | Inherited from [`Message`](#message-10) |
| [`read`](#read-13) | `function` | Inherited from [`Message`](#message-10) |
| [`read`](#read-14) | `function` | Inherited from [`Message`](#message-10) |
| [`write`](#write-28) | `function` | Inherited from [`Message`](#message-10) |
| [`isRequest`](#isrequest) | `function` | Inherited from [`Message`](#message-10) |
| [`size`](#size-12) | `function` | Inherited from [`Message`](#message-10) |
| [`print`](#print-16) | `function` | Inherited from [`Message`](#message-10) |
| [`className`](#classname-8) | `function` | Inherited from [`Message`](#message-10) |
| [`data`](#data-6) | `function` | Inherited from [`Message`](#message-10) |
| [`hasData`](#hasdata-1) | `function` | Inherited from [`Message`](#message-10) |
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

### Inherited from [`Message`](#message-10)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`Message`](#message-11)  | Constructs a new message with a random ID and type set to "message". |
| `function` | [`Message`](#message-12)  | Constructs a message from a JSON value; sets missing id/type fields. |
| `function` | [`Message`](#message-13)  | Copy constructor; preserves or generates id/type fields. |
| `function` | [`clone`](#clone-18) `virtual` `const` `override` | Returns a heap-allocated copy of this message. |
| `function` | [`valid`](#valid-15) `virtual` `const` | Returns true if the message has both `[type](#type-22)` and `[id](#id-7)` fields. |
| `function` | [`clear`](#clear-4) `virtual` | Clears all JSON fields from this message. |
| `function` | [`clearData`](#cleardata) `virtual` | Clears the `[data](#data-4)` sub-object. |
| `function` | [`clearNotes`](#clearnotes) `virtual` | Clears the `[notes](#notes)` array. |
| `function` | [`type`](#type-22) `const` `nodiscard` | Returns the message type string (defaults to "message"). |
| `function` | [`id`](#id-7) `const` `nodiscard` | Returns the message ID string. |
| `function` | [`to`](#to) `const` `nodiscard` | Returns the recipient address parsed from the `[to](#to)` field. |
| `function` | [`from`](#from) `const` `nodiscard` | Returns the sender address parsed from the `[from](#from)` field. |
| `function` | [`toUser`](#touser) `const` `nodiscard` | Returns the user component of the `[to](#to)` address without constructing an [Address](#address-13). |
| `function` | [`toId`](#toid) `const` `nodiscard` | Returns the id component of the `[to](#to)` address without constructing an [Address](#address-13). |
| `function` | [`fromUser`](#fromuser) `const` `nodiscard` | Returns the user component of the `[from](#from)` address without constructing an [Address](#address-13). |
| `function` | [`fromId`](#fromid) `const` `nodiscard` | Returns the id component of the `[from](#from)` address without constructing an [Address](#address-13). |
| `function` | [`status`](#status-3) `const` `nodiscard` | Returns the HTTP status code, or -1 if not set. |
| `function` | [`setType`](#settype-2)  | Sets the message type field. |
| `function` | [`setTo`](#setto)  | Sets the `[to](#to)` field from a peer's address. |
| `function` | [`setTo`](#setto-1)  | Sets the `[to](#to)` field from an address object. |
| `function` | [`setTo`](#setto-2)  | Sets the `[to](#to)` field from an address string. |
| `function` | [`setFrom`](#setfrom)  | Sets the `[from](#from)` field from a peer's address. |
| `function` | [`setFrom`](#setfrom-1)  | Sets the `[from](#from)` field from an address object. |
| `function` | [`setFrom`](#setfrom-2)  | Sets the `[from](#from)` field from an address string. |
| `function` | [`setStatus`](#setstatus-1)  | HTTP status codes are used to describe the message response. Throws std::invalid_argument if code is outside [101, 504]. |
| `function` | [`notes`](#notes)  | Returns a reference to the `[notes](#notes)` JSON array. |
| `function` | [`setNote`](#setnote)  | Replaces all notes with a single note. |
| `function` | [`addNote`](#addnote)  | Appends a note to the `[notes](#notes)` array. |
| `function` | [`data`](#data-4) `const` | Returns a copy of the named data field. |
| `function` | [`data`](#data-5)  | Returns a reference to the named data field (creates it if absent). |
| `function` | [`setData`](#setdata)  | Creates or replaces a named data field; returns a reference to it. |
| `function` | [`setData`](#setdata-1)  | Sets a named data field to a C-string value. |
| `function` | [`setData`](#setdata-2)  | Sets a named data field to a string value. |
| `function` | [`setData`](#setdata-3)  | Sets a named data field to a JSON value. |
| `function` | [`setData`](#setdata-4)  | Sets a named data field to an integer value. |
| `function` | [`removeData`](#removedata)  | Removes a named field from the `[data](#data-4)` sub-object. |
| `function` | [`hasData`](#hasdata)  | Returns true if the named field exists in the `[data](#data-4)` sub-object. |
| `function` | [`read`](#read-13) `virtual` `override` | Deserialises the message from a raw buffer. |
| `function` | [`read`](#read-14) `virtual` | Deserialises the message from a JSON string. |
| `function` | [`write`](#write-28) `virtual` `const` `override` | Serialises the message as JSON into a buffer. |
| `function` | [`isRequest`](#isrequest) `const` `nodiscard` | Returns true if no status code has been set (i.e. [status()](#status-3) == -1). |
| `function` | [`size`](#size-12) `virtual` `const` `override` | Returns the serialised JSON size in bytes. |
| `function` | [`print`](#print-16) `virtual` `const` `override` | Pretty-prints the message JSON to the given stream. |
| `function` | [`className`](#classname-8) `virtual` `const` `inline` `override` | Returns the class name of this packet type for logging and diagnostics. |
| `function` | [`data`](#data-6) `virtual` `const` `inline` | The packet data pointer for buffered packets. |
| `function` | [`hasData`](#hasdata-1) `virtual` `const` `inline` | Returns true if the packet has a non-null data pointer. |

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

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Event`](#event-3)  | Constructs an event with type "event" and time set to now. |
|  | [`Event`](#event-4)  | Constructs an event from a JSON value; sets missing time to now. |
|  | [`Event`](#event-5)  | Copy constructor; preserves or sets missing time to now. |
| `bool` | [`valid`](#valid-12) `virtual` `const` `nodiscard` | Returns true if the base message is valid and the `[name](#name-13)` field is set. |
| `std::string` | [`name`](#name-13) `const` `nodiscard` | Returns the event name string. |
| `std::time_t` | [`time`](#time-4) `const` `nodiscard` | Returns the event timestamp as a Unix time_t value. |
| `void` | [`setName`](#setname-2)  | Sets the event name field. |
| `void` | [`setTime`](#settime)  | Sets the event timestamp. |

---

{#event-3}

#### Event

```cpp
Event()
```

Defined in src/symple/include/icy/symple/event.h:33

Constructs an event with type "event" and time set to now.

---

{#event-4}

#### Event

```cpp
Event(const json::Value & root)
```

Defined in src/symple/include/icy/symple/event.h:37

Constructs an event from a JSON value; sets missing time to now. 
#### Parameters
* `root` JSON object to initialise from.

---

{#event-5}

#### Event

```cpp
Event(const Event & root)
```

Defined in src/symple/include/icy/symple/event.h:41

Copy constructor; preserves or sets missing time to now. 
#### Parameters
* `root` Source event.

---

{#valid-12}

#### valid

`virtual` `const` `nodiscard`

```cpp
[[nodiscard]] virtual bool valid() const
```

Defined in src/symple/include/icy/symple/event.h:46

Returns true if the base message is valid and the `[name](#name-13)` field is set.

##### Reimplements

- [`valid`](#valid-15)

---

{#name-13}

#### name

`const` `nodiscard`

```cpp
[[nodiscard]] std::string name() const
```

Defined in src/symple/include/icy/symple/event.h:49

Returns the event name string.

---

{#time-4}

#### time

`const` `nodiscard`

```cpp
[[nodiscard]] std::time_t time() const
```

Defined in src/symple/include/icy/symple/event.h:53

Returns the event timestamp as a Unix time_t value.

---

{#setname-2}

#### setName

```cpp
void setName(std::string_view name)
```

Defined in src/symple/include/icy/symple/event.h:57

Sets the event name field. 
#### Parameters
* `name` [Event](#event-2) name string.

---

{#settime}

#### setTime

```cpp
void setTime(std::time_t time)
```

Defined in src/symple/include/icy/symple/event.h:62

Sets the event timestamp. 
#### Parameters
* `time` Unix timestamp (seconds since epoch).

{#form}

## Form

```cpp
#include <icy/symple/form.h>
```

```cpp
class Form
```

Defined in src/symple/include/icy/symple/form.h:178

> **Inherits:** [`FormElement`](#formelement)

Interactive form with pages, sections, and fields for command data exchange.

Attach a [Form](#form) to a [Command](#command) message to carry structured input/output. The `[action](#action-3)` field governs the exchange direction; `[partial](#partial)` enables live/auto-complete field submission.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`Form`](#form-1) | `function` | Declared here |
| [`Form`](#form-2) | `function` | Declared here |
| [`Form`](#form-3) | `function` | Declared here |
| [`valid`](#valid-13) | `function` | Declared here |
| [`action`](#action-3) | `function` | Declared here |
| [`partial`](#partial) | `function` | Declared here |
| [`setAction`](#setaction-1) | `function` | Declared here |
| [`setPartial`](#setpartial) | `function` | Declared here |
| [`FormElement`](#formelement-1) | `function` | Inherited from [`FormElement`](#formelement) |
| [`FormElement`](#formelement-2) | `function` | Inherited from [`FormElement`](#formelement) |
| [`FormElement`](#formelement-3) | `function` | Inherited from [`FormElement`](#formelement) |
| [`operator=`](#operator-37) | `function` | Inherited from [`FormElement`](#formelement) |
| [`type`](#type-21) | `function` | Inherited from [`FormElement`](#formelement) |
| [`id`](#id-6) | `function` | Inherited from [`FormElement`](#formelement) |
| [`label`](#label) | `function` | Inherited from [`FormElement`](#formelement) |
| [`setType`](#settype-1) | `function` | Inherited from [`FormElement`](#formelement) |
| [`setId`](#setid) | `function` | Inherited from [`FormElement`](#formelement) |
| [`setLabel`](#setlabel) | `function` | Inherited from [`FormElement`](#formelement) |
| [`setHint`](#sethint) | `function` | Inherited from [`FormElement`](#formelement) |
| [`setError`](#seterror-7) | `function` | Inherited from [`FormElement`](#formelement) |
| [`addPage`](#addpage) | `function` | Inherited from [`FormElement`](#formelement) |
| [`addSection`](#addsection) | `function` | Inherited from [`FormElement`](#formelement) |
| [`addField`](#addfield) | `function` | Inherited from [`FormElement`](#formelement) |
| [`getField`](#getfield) | `function` | Inherited from [`FormElement`](#formelement) |
| [`getField`](#getfield-1) | `function` | Inherited from [`FormElement`](#formelement) |
| [`hasField`](#hasfield) | `function` | Inherited from [`FormElement`](#formelement) |
| [`setLive`](#setlive) | `function` | Inherited from [`FormElement`](#formelement) |
| [`live`](#live) | `function` | Inherited from [`FormElement`](#formelement) |
| [`clearElements`](#clearelements) | `function` | Inherited from [`FormElement`](#formelement) |
| [`clear`](#clear-3) | `function` | Inherited from [`FormElement`](#formelement) |
| [`valid`](#valid-14) | `function` | Inherited from [`FormElement`](#formelement) |
| [`numElements`](#numelements) | `function` | Inherited from [`FormElement`](#formelement) |
| [`hasErrors`](#haserrors) | `function` | Inherited from [`FormElement`](#formelement) |
| [`hasPages`](#haspages) | `function` | Inherited from [`FormElement`](#formelement) |
| [`root`](#root-4) | `function` | Inherited from [`FormElement`](#formelement) |
| [`_root`](#_root) | `variable` | Inherited from [`FormElement`](#formelement) |

### Inherited from [`FormElement`](#formelement)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`FormElement`](#formelement-1)  | Constructs an unbound element (root pointer is null). |
| `function` | [`FormElement`](#formelement-2)  | Constructs an element bound to the given JSON node. |
| `function` | [`FormElement`](#formelement-3)  | Copy constructor; copies the root pointer reference (shallow). |
| `function` | [`operator=`](#operator-37)  | Copy-assigns the root pointer reference. |
| `function` | [`type`](#type-21) `const` `nodiscard` | Returns the element type string. |
| `function` | [`id`](#id-6) `const` `nodiscard` | Returns the element ID string. |
| `function` | [`label`](#label) `const` `nodiscard` | Returns the display label string. |
| `function` | [`setType`](#settype-1)  | Sets the element type. Possible values: page, section, text, text-multi, list, list-multi, checkbox, media, custom |
| `function` | [`setId`](#setid)  | Sets the element ID field. |
| `function` | [`setLabel`](#setlabel)  | Sets the display label field. |
| `function` | [`setHint`](#sethint)  | Sets the hint/description field shown below the element. |
| `function` | [`setError`](#seterror-7)  | Sets an optional validation error message. |
| `function` | [`addPage`](#addpage)  | Appends a page child element and returns a handle to it. |
| `function` | [`addSection`](#addsection)  | Appends a section child element and returns a handle to it. |
| `function` | [`addField`](#addfield)  | Appends a typed field child element and returns a handle to it. Throws std::invalid_argument if type is not a recognised field type. |
| `function` | [`getField`](#getfield)  | Searches child elements for the field with the given ID. |
| `function` | [`getField`](#getfield-1)  | Populates a [FormField](#formfield) by searching child elements for the given ID. |
| `function` | [`hasField`](#hasfield)  | Returns true if any child element has an ID matching the given value. |
| `function` | [`setLive`](#setlive)  | Sets the live flag on this element. Live elements are used to submit partial form sections (e.g. for auto-complete) without sending the entire form. |
| `function` | [`live`](#live) `const` `nodiscard` | Returns true if this field is live, meaning the form-processing entity should auto-update this field's value whenever it changes. |
| `function` | [`clearElements`](#clearelements)  | Removes all child elements whose ID matches the given value. |
| `function` | [`clear`](#clear-3)  | Clears all fields from the underlying JSON node. |
| `function` | [`valid`](#valid-14) `const` `nodiscard` | Returns true if the form element is valid. |
| `function` | [`numElements`](#numelements)  | Returns the number of child elements. |
| `function` | [`hasErrors`](#haserrors)  | Returns true if any fields have errors. |
| `function` | [`hasPages`](#haspages)  | Returns true if the form has multiple pages. |
| `function` | [`root`](#root-4) `const` `nodiscard` | Returns a reference to the underlying JSON node. Throws std::runtime_error if the root pointer is null. |
| `variable` | [`_root`](#_root)  | The root pointer is just a reference to the externally managed JSON value memory. |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Form`](#form-1)  | Constructs an unbound form (root pointer is null). |
|  | [`Form`](#form-2)  | Constructs a form bound to the given JSON node. Sets the `[type](#type-21)` field to "form". |
|  | [`Form`](#form-3)  | Constructs a form from a [Command](#command), binding to `command["form"]`. Sets the `[type](#type-21)` field to "form". |
| `bool` | [`valid`](#valid-13)  | Returns true if the element is valid, non-empty, and has no errors. |
| `std::string` | [`action`](#action-3) `const` `nodiscard` | Returns the form action string (defaults to "form"). |
| `bool` | [`partial`](#partial) `const` `nodiscard` | Returns true if this is a partial form submission. |
| `void` | [`setAction`](#setaction-1)  | Sets the form action field. Throws std::invalid_argument for unrecognised values. Possible values: `form` - Form-processing entity requests form completion. `submit` - Form-submitting entity is sending data. `cancel` - Form-submitting entity cancelled submission. `result` - Form-processing entity is returning data. |
| `void` | [`setPartial`](#setpartial)  | Marks the form as a partial section for live/auto-complete updates. Partial forms transmit only the changed fields rather than the entire form payload. |

---

{#form-1}

#### Form

```cpp
Form()
```

Defined in src/symple/include/icy/symple/form.h:182

Constructs an unbound form (root pointer is null).

---

{#form-2}

#### Form

```cpp
Form(json::Value & root)
```

Defined in src/symple/include/icy/symple/form.h:187

Constructs a form bound to the given JSON node. Sets the `[type](#type-21)` field to "form". 
#### Parameters
* `root` JSON node to bind to.

---

{#form-3}

#### Form

```cpp
Form(Command & root)
```

Defined in src/symple/include/icy/symple/form.h:192

Constructs a form from a [Command](#command), binding to `command["form"]`. Sets the `[type](#type-21)` field to "form". 
#### Parameters
* `root` Parent command message.

---

{#valid-13}

#### valid

```cpp
bool valid()
```

Defined in src/symple/include/icy/symple/form.h:197

Returns true if the element is valid, non-empty, and has no errors.

---

{#action-3}

#### action

`const` `nodiscard`

```cpp
[[nodiscard]] std::string action() const
```

Defined in src/symple/include/icy/symple/form.h:200

Returns the form action string (defaults to "form").

---

{#partial}

#### partial

`const` `nodiscard`

```cpp
[[nodiscard]] bool partial() const
```

Defined in src/symple/include/icy/symple/form.h:203

Returns true if this is a partial form submission.

---

{#setaction-1}

#### setAction

```cpp
void setAction(std::string_view action)
```

Defined in src/symple/include/icy/symple/form.h:213

Sets the form action field. Throws std::invalid_argument for unrecognised values. Possible values: `form` - Form-processing entity requests form completion. `submit` - Form-submitting entity is sending data. `cancel` - Form-submitting entity cancelled submission. `result` - Form-processing entity is returning data. 
#### Parameters
* `action` Action string.

---

{#setpartial}

#### setPartial

```cpp
void setPartial(bool flag)
```

Defined in src/symple/include/icy/symple/form.h:219

Marks the form as a partial section for live/auto-complete updates. Partial forms transmit only the changed fields rather than the entire form payload. 
#### Parameters
* `flag` True to mark as partial.

{#formelement}

## FormElement

```cpp
#include <icy/symple/form.h>
```

```cpp
class FormElement
```

Defined in src/symple/include/icy/symple/form.h:33

> **Subclassed by:** [`Form`](#form), [`FormField`](#formfield)

Base element within a Symple form.

[FormElement](#formelement) wraps a reference to an external JSON node and provides typed accessors for the common `[type](#type-21)`, `[id](#id-6)`, `[label](#label)`, and `elements` fields. Pages, sections, and fields all derive from this base.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`FormElement`](#formelement-1) | `function` | Declared here |
| [`FormElement`](#formelement-2) | `function` | Declared here |
| [`FormElement`](#formelement-3) | `function` | Declared here |
| [`operator=`](#operator-37) | `function` | Declared here |
| [`type`](#type-21) | `function` | Declared here |
| [`id`](#id-6) | `function` | Declared here |
| [`label`](#label) | `function` | Declared here |
| [`setType`](#settype-1) | `function` | Declared here |
| [`setId`](#setid) | `function` | Declared here |
| [`setLabel`](#setlabel) | `function` | Declared here |
| [`setHint`](#sethint) | `function` | Declared here |
| [`setError`](#seterror-7) | `function` | Declared here |
| [`addPage`](#addpage) | `function` | Declared here |
| [`addSection`](#addsection) | `function` | Declared here |
| [`addField`](#addfield) | `function` | Declared here |
| [`getField`](#getfield) | `function` | Declared here |
| [`getField`](#getfield-1) | `function` | Declared here |
| [`hasField`](#hasfield) | `function` | Declared here |
| [`setLive`](#setlive) | `function` | Declared here |
| [`live`](#live) | `function` | Declared here |
| [`clearElements`](#clearelements) | `function` | Declared here |
| [`clear`](#clear-3) | `function` | Declared here |
| [`valid`](#valid-14) | `function` | Declared here |
| [`numElements`](#numelements) | `function` | Declared here |
| [`hasErrors`](#haserrors) | `function` | Declared here |
| [`hasPages`](#haspages) | `function` | Declared here |
| [`root`](#root-4) | `function` | Declared here |
| [`_root`](#_root) | `variable` | Declared here |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`FormElement`](#formelement-1)  | Constructs an unbound element (root pointer is null). |
|  | [`FormElement`](#formelement-2)  | Constructs an element bound to the given JSON node. |
|  | [`FormElement`](#formelement-3)  | Copy constructor; copies the root pointer reference (shallow). |
| `FormElement &` | [`operator=`](#operator-37)  | Copy-assigns the root pointer reference. |
| `std::string` | [`type`](#type-21) `const` `nodiscard` | Returns the element type string. |
| `std::string` | [`id`](#id-6) `const` `nodiscard` | Returns the element ID string. |
| `std::string` | [`label`](#label) `const` `nodiscard` | Returns the display label string. |
| `void` | [`setType`](#settype-1)  | Sets the element type. Possible values: page, section, text, text-multi, list, list-multi, checkbox, media, custom |
| `void` | [`setId`](#setid)  | Sets the element ID field. |
| `void` | [`setLabel`](#setlabel)  | Sets the display label field. |
| `void` | [`setHint`](#sethint)  | Sets the hint/description field shown below the element. |
| `void` | [`setError`](#seterror-7)  | Sets an optional validation error message. |
| `FormElement` | [`addPage`](#addpage)  | Appends a page child element and returns a handle to it. |
| `FormElement` | [`addSection`](#addsection)  | Appends a section child element and returns a handle to it. |
| `FormField` | [`addField`](#addfield)  | Appends a typed field child element and returns a handle to it. Throws std::invalid_argument if type is not a recognised field type. |
| `FormField` | [`getField`](#getfield)  | Searches child elements for the field with the given ID. |
| `bool` | [`getField`](#getfield-1)  | Populates a [FormField](#formfield) by searching child elements for the given ID. |
| `bool` | [`hasField`](#hasfield)  | Returns true if any child element has an ID matching the given value. |
| `void` | [`setLive`](#setlive)  | Sets the live flag on this element. Live elements are used to submit partial form sections (e.g. for auto-complete) without sending the entire form. |
| `bool` | [`live`](#live) `const` `nodiscard` | Returns true if this field is live, meaning the form-processing entity should auto-update this field's value whenever it changes. |
| `bool` | [`clearElements`](#clearelements)  | Removes all child elements whose ID matches the given value. |
| `void` | [`clear`](#clear-3)  | Clears all fields from the underlying JSON node. |
| `bool` | [`valid`](#valid-14) `const` `nodiscard` | Returns true if the form element is valid. |
| `int` | [`numElements`](#numelements)  | Returns the number of child elements. |
| `bool` | [`hasErrors`](#haserrors)  | Returns true if any fields have errors. |
| `bool` | [`hasPages`](#haspages)  | Returns true if the form has multiple pages. |
| `json::Value &` | [`root`](#root-4) `const` `nodiscard` | Returns a reference to the underlying JSON node. Throws std::runtime_error if the root pointer is null. |

---

{#formelement-1}

#### FormElement

```cpp
FormElement()
```

Defined in src/symple/include/icy/symple/form.h:37

Constructs an unbound element (root pointer is null).

---

{#formelement-2}

#### FormElement

```cpp
FormElement(json::Value & root, std::string_view type = "", std::string_view id = "", std::string_view label = "")
```

Defined in src/symple/include/icy/symple/form.h:44

Constructs an element bound to the given JSON node. 
#### Parameters
* `root` JSON node this element refers to. 

* `type` Optional element type string. 

* `id` Optional element ID string. 

* `label` Optional display label string.

---

{#formelement-3}

#### FormElement

```cpp
FormElement(const FormElement & r)
```

Defined in src/symple/include/icy/symple/form.h:49

Copy constructor; copies the root pointer reference (shallow). 
#### Parameters
* `r` Source element.

---

{#operator-37}

#### operator=

```cpp
FormElement & operator=(const FormElement & r)
```

Defined in src/symple/include/icy/symple/form.h:53

Copy-assigns the root pointer reference. 
#### Parameters
* `r` Source element.

---

{#type-21}

#### type

`const` `nodiscard`

```cpp
[[nodiscard]] std::string type() const
```

Defined in src/symple/include/icy/symple/form.h:58

Returns the element type string.

---

{#id-6}

#### id

`const` `nodiscard`

```cpp
[[nodiscard]] std::string id() const
```

Defined in src/symple/include/icy/symple/form.h:61

Returns the element ID string.

---

{#label}

#### label

`const` `nodiscard`

```cpp
[[nodiscard]] std::string label() const
```

Defined in src/symple/include/icy/symple/form.h:64

Returns the display label string.

---

{#settype-1}

#### setType

```cpp
void setType(std::string_view type)
```

Defined in src/symple/include/icy/symple/form.h:70

Sets the element type. Possible values: page, section, text, text-multi, list, list-multi, checkbox, media, custom 
#### Parameters
* `type` Element type string.

---

{#setid}

#### setId

```cpp
void setId(std::string_view id)
```

Defined in src/symple/include/icy/symple/form.h:74

Sets the element ID field. 
#### Parameters
* `id` Element ID string.

---

{#setlabel}

#### setLabel

```cpp
void setLabel(std::string_view text)
```

Defined in src/symple/include/icy/symple/form.h:78

Sets the display label field. 
#### Parameters
* `text` Label text.

---

{#sethint}

#### setHint

```cpp
void setHint(std::string_view text)
```

Defined in src/symple/include/icy/symple/form.h:82

Sets the hint/description field shown below the element. 
#### Parameters
* `text` Hint text.

---

{#seterror-7}

#### setError

```cpp
void setError(std::string_view error)
```

Defined in src/symple/include/icy/symple/form.h:86

Sets an optional validation error message. 
#### Parameters
* `error` [Error](base.md#error) text to display.

---

{#addpage}

#### addPage

```cpp
FormElement addPage(std::string_view id = "", std::string_view label = "")
```

Defined in src/symple/include/icy/symple/form.h:92

Appends a page child element and returns a handle to it. 
#### Parameters
* `id` Optional page ID. 

* `label` Optional page label. 

#### Returns
[FormElement](#formelement) referencing the new page node.

---

{#addsection}

#### addSection

```cpp
FormElement addSection(std::string_view id = "", std::string_view label = "")
```

Defined in src/symple/include/icy/symple/form.h:99

Appends a section child element and returns a handle to it. 
#### Parameters
* `id` Optional section ID. 

* `label` Optional section label. 

#### Returns
[FormElement](#formelement) referencing the new section node.

---

{#addfield}

#### addField

```cpp
FormField addField(std::string_view type, std::string_view id = "", std::string_view label = "")
```

Defined in src/symple/include/icy/symple/form.h:108

Appends a typed field child element and returns a handle to it. Throws std::invalid_argument if type is not a recognised field type. 
#### Parameters
* `type` Field type (e.g. "text", "list", "boolean"). 

* `id` Optional field ID. 

* `label` Optional field label. 

#### Returns
[FormField](#formfield) referencing the new field node.

---

{#getfield}

#### getField

```cpp
FormField getField(std::string_view id, bool partial = false)
```

Defined in src/symple/include/icy/symple/form.h:115

Searches child elements for the field with the given ID. 
#### Parameters
* `id` Field ID to search for. 

* `partial` If true, performs a substring match. 

#### Returns
[FormField](#formfield) handle (may be invalid if not found).

---

{#getfield-1}

#### getField

```cpp
bool getField(std::string_view id, FormField & field, bool partial = false)
```

Defined in src/symple/include/icy/symple/form.h:122

Populates a [FormField](#formfield) by searching child elements for the given ID. 
#### Parameters
* `id` Field ID to search for. 

* `field` Output field handle. 

* `partial` If true, performs a substring match. 

#### Returns
True if the field was found.

---

{#hasfield}

#### hasField

```cpp
bool hasField(std::string_view id, bool partial = false)
```

Defined in src/symple/include/icy/symple/form.h:128

Returns true if any child element has an ID matching the given value. 
#### Parameters
* `id` ID string to search for. 

* `partial` If true, a substring match is sufficient.

---

{#setlive}

#### setLive

```cpp
void setLive(bool flag)
```

Defined in src/symple/include/icy/symple/form.h:134

Sets the live flag on this element. Live elements are used to submit partial form sections (e.g. for auto-complete) without sending the entire form. 
#### Parameters
* `flag` True to enable live updates.

---

{#live}

#### live

`const` `nodiscard`

```cpp
[[nodiscard]] bool live() const
```

Defined in src/symple/include/icy/symple/form.h:139

Returns true if this field is live, meaning the form-processing entity should auto-update this field's value whenever it changes.

---

{#clearelements}

#### clearElements

```cpp
bool clearElements(std::string_view id, bool partial = false)
```

Defined in src/symple/include/icy/symple/form.h:145

Removes all child elements whose ID matches the given value. 
#### Parameters
* `id` ID string to match against. 

* `partial` If true, removes elements whose ID contains the string. 

#### Returns
True if at least one element was removed.

---

{#clear-3}

#### clear

```cpp
void clear()
```

Defined in src/symple/include/icy/symple/form.h:148

Clears all fields from the underlying JSON node.

---

{#valid-14}

#### valid

`const` `nodiscard`

```cpp
[[nodiscard]] bool valid() const
```

Defined in src/symple/include/icy/symple/form.h:151

Returns true if the form element is valid.

---

{#numelements}

#### numElements

```cpp
int numElements()
```

Defined in src/symple/include/icy/symple/form.h:154

Returns the number of child elements.

---

{#haserrors}

#### hasErrors

```cpp
bool hasErrors()
```

Defined in src/symple/include/icy/symple/form.h:157

Returns true if any fields have errors.

---

{#haspages}

#### hasPages

```cpp
bool hasPages()
```

Defined in src/symple/include/icy/symple/form.h:160

Returns true if the form has multiple pages.

---

{#root-4}

#### root

`const` `nodiscard`

```cpp
[[nodiscard]] json::Value & root() const
```

Defined in src/symple/include/icy/symple/form.h:164

Returns a reference to the underlying JSON node. Throws std::runtime_error if the root pointer is null.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `json::Value *` | [`_root`](#_root)  | The root pointer is just a reference to the externally managed JSON value memory. |

---

{#_root}

#### _root

```cpp
json::Value * _root
```

Defined in src/symple/include/icy/symple/form.h:169

The root pointer is just a reference to the externally managed JSON value memory.

{#formfield}

## FormField

```cpp
#include <icy/symple/form.h>
```

```cpp
class FormField
```

Defined in src/symple/include/icy/symple/form.h:227

> **Inherits:** [`FormElement`](#formelement)

[Form](#form) input field with typed values and selectable options.

Values are stored as strings in a JSON array. Typed accessors (intValue, doubleValue, boolValue) parse the first string on demand.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`FormField`](#formfield-1) | `function` | Declared here |
| [`FormField`](#formfield-2) | `function` | Declared here |
| [`addOption`](#addoption) | `function` | Declared here |
| [`addOption`](#addoption-1) | `function` | Declared here |
| [`setValue`](#setvalue-4) | `function` | Declared here |
| [`setValue`](#setvalue-5) | `function` | Declared here |
| [`setValue`](#setvalue-6) | `function` | Declared here |
| [`setValue`](#setvalue-7) | `function` | Declared here |
| [`addValue`](#addvalue) | `function` | Declared here |
| [`addValue`](#addvalue-1) | `function` | Declared here |
| [`addValue`](#addvalue-2) | `function` | Declared here |
| [`addValue`](#addvalue-3) | `function` | Declared here |
| [`values`](#values) | `function` | Declared here |
| [`value`](#value-4) | `function` | Declared here |
| [`intValue`](#intvalue) | `function` | Declared here |
| [`doubleValue`](#doublevalue) | `function` | Declared here |
| [`boolValue`](#boolvalue) | `function` | Declared here |
| [`FormElement`](#formelement-1) | `function` | Inherited from [`FormElement`](#formelement) |
| [`FormElement`](#formelement-2) | `function` | Inherited from [`FormElement`](#formelement) |
| [`FormElement`](#formelement-3) | `function` | Inherited from [`FormElement`](#formelement) |
| [`operator=`](#operator-37) | `function` | Inherited from [`FormElement`](#formelement) |
| [`type`](#type-21) | `function` | Inherited from [`FormElement`](#formelement) |
| [`id`](#id-6) | `function` | Inherited from [`FormElement`](#formelement) |
| [`label`](#label) | `function` | Inherited from [`FormElement`](#formelement) |
| [`setType`](#settype-1) | `function` | Inherited from [`FormElement`](#formelement) |
| [`setId`](#setid) | `function` | Inherited from [`FormElement`](#formelement) |
| [`setLabel`](#setlabel) | `function` | Inherited from [`FormElement`](#formelement) |
| [`setHint`](#sethint) | `function` | Inherited from [`FormElement`](#formelement) |
| [`setError`](#seterror-7) | `function` | Inherited from [`FormElement`](#formelement) |
| [`addPage`](#addpage) | `function` | Inherited from [`FormElement`](#formelement) |
| [`addSection`](#addsection) | `function` | Inherited from [`FormElement`](#formelement) |
| [`addField`](#addfield) | `function` | Inherited from [`FormElement`](#formelement) |
| [`getField`](#getfield) | `function` | Inherited from [`FormElement`](#formelement) |
| [`getField`](#getfield-1) | `function` | Inherited from [`FormElement`](#formelement) |
| [`hasField`](#hasfield) | `function` | Inherited from [`FormElement`](#formelement) |
| [`setLive`](#setlive) | `function` | Inherited from [`FormElement`](#formelement) |
| [`live`](#live) | `function` | Inherited from [`FormElement`](#formelement) |
| [`clearElements`](#clearelements) | `function` | Inherited from [`FormElement`](#formelement) |
| [`clear`](#clear-3) | `function` | Inherited from [`FormElement`](#formelement) |
| [`valid`](#valid-14) | `function` | Inherited from [`FormElement`](#formelement) |
| [`numElements`](#numelements) | `function` | Inherited from [`FormElement`](#formelement) |
| [`hasErrors`](#haserrors) | `function` | Inherited from [`FormElement`](#formelement) |
| [`hasPages`](#haspages) | `function` | Inherited from [`FormElement`](#formelement) |
| [`root`](#root-4) | `function` | Inherited from [`FormElement`](#formelement) |
| [`_root`](#_root) | `variable` | Inherited from [`FormElement`](#formelement) |

### Inherited from [`FormElement`](#formelement)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`FormElement`](#formelement-1)  | Constructs an unbound element (root pointer is null). |
| `function` | [`FormElement`](#formelement-2)  | Constructs an element bound to the given JSON node. |
| `function` | [`FormElement`](#formelement-3)  | Copy constructor; copies the root pointer reference (shallow). |
| `function` | [`operator=`](#operator-37)  | Copy-assigns the root pointer reference. |
| `function` | [`type`](#type-21) `const` `nodiscard` | Returns the element type string. |
| `function` | [`id`](#id-6) `const` `nodiscard` | Returns the element ID string. |
| `function` | [`label`](#label) `const` `nodiscard` | Returns the display label string. |
| `function` | [`setType`](#settype-1)  | Sets the element type. Possible values: page, section, text, text-multi, list, list-multi, checkbox, media, custom |
| `function` | [`setId`](#setid)  | Sets the element ID field. |
| `function` | [`setLabel`](#setlabel)  | Sets the display label field. |
| `function` | [`setHint`](#sethint)  | Sets the hint/description field shown below the element. |
| `function` | [`setError`](#seterror-7)  | Sets an optional validation error message. |
| `function` | [`addPage`](#addpage)  | Appends a page child element and returns a handle to it. |
| `function` | [`addSection`](#addsection)  | Appends a section child element and returns a handle to it. |
| `function` | [`addField`](#addfield)  | Appends a typed field child element and returns a handle to it. Throws std::invalid_argument if type is not a recognised field type. |
| `function` | [`getField`](#getfield)  | Searches child elements for the field with the given ID. |
| `function` | [`getField`](#getfield-1)  | Populates a [FormField](#formfield) by searching child elements for the given ID. |
| `function` | [`hasField`](#hasfield)  | Returns true if any child element has an ID matching the given value. |
| `function` | [`setLive`](#setlive)  | Sets the live flag on this element. Live elements are used to submit partial form sections (e.g. for auto-complete) without sending the entire form. |
| `function` | [`live`](#live) `const` `nodiscard` | Returns true if this field is live, meaning the form-processing entity should auto-update this field's value whenever it changes. |
| `function` | [`clearElements`](#clearelements)  | Removes all child elements whose ID matches the given value. |
| `function` | [`clear`](#clear-3)  | Clears all fields from the underlying JSON node. |
| `function` | [`valid`](#valid-14) `const` `nodiscard` | Returns true if the form element is valid. |
| `function` | [`numElements`](#numelements)  | Returns the number of child elements. |
| `function` | [`hasErrors`](#haserrors)  | Returns true if any fields have errors. |
| `function` | [`hasPages`](#haspages)  | Returns true if the form has multiple pages. |
| `function` | [`root`](#root-4) `const` `nodiscard` | Returns a reference to the underlying JSON node. Throws std::runtime_error if the root pointer is null. |
| `variable` | [`_root`](#_root)  | The root pointer is just a reference to the externally managed JSON value memory. |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`FormField`](#formfield-1)  | Constructs an unbound field (root pointer is null). |
|  | [`FormField`](#formfield-2)  | Constructs a field bound to the given JSON node. |
| `void` | [`addOption`](#addoption)  | Adds a labelled option for list-based fields. |
| `void` | [`addOption`](#addoption-1)  | Adds an option whose key and display value are identical. |
| `void` | [`setValue`](#setvalue-4)  | Replaces all current values with a single string value. |
| `void` | [`setValue`](#setvalue-5)  | Replaces all current values with a single integer value. |
| `void` | [`setValue`](#setvalue-6)  | Replaces all current values with a single double value. |
| `void` | [`setValue`](#setvalue-7)  | Replaces all current values with a single boolean value. |
| `void` | [`addValue`](#addvalue)  | Appends a string value to the values array. |
| `void` | [`addValue`](#addvalue-1)  | Appends an integer value to the values array. |
| `void` | [`addValue`](#addvalue-2)  | Appends a double value to the values array. |
| `void` | [`addValue`](#addvalue-3)  | Appends a boolean value to the values array. |
| `json::Value &` | [`values`](#values)  | Returns a reference to the JSON array of all values. |
| `std::string` | [`value`](#value-4) `const` `nodiscard` | Returns the first value as a string. Most field types (except multi-value) only use a single value. |
| `int` | [`intValue`](#intvalue) `const` `nodiscard` | Returns the first value parsed as an integer. |
| `double` | [`doubleValue`](#doublevalue) `const` `nodiscard` | Returns the first value parsed as a double. |
| `bool` | [`boolValue`](#boolvalue) `const` `nodiscard` | Returns the first value parsed as a boolean. Treats "1", "true", and "on" as true; all other strings as false. |

---

{#formfield-1}

#### FormField

```cpp
FormField()
```

Defined in src/symple/include/icy/symple/form.h:231

Constructs an unbound field (root pointer is null).

---

{#formfield-2}

#### FormField

```cpp
FormField(json::Value & root, std::string_view type = "", std::string_view id = "", std::string_view label = "")
```

Defined in src/symple/include/icy/symple/form.h:238

Constructs a field bound to the given JSON node. 
#### Parameters
* `root` JSON node to bind to. 

* `type` Field type string (e.g. "text", "list"). 

* `id` Optional field ID. 

* `label` Optional display label.

---

{#addoption}

#### addOption

```cpp
void addOption(std::string_view key, std::string_view value)
```

Defined in src/symple/include/icy/symple/form.h:245

Adds a labelled option for list-based fields. 
#### Parameters
* `key` Option key sent on submit. 

* `value` Display label for the option.

---

{#addoption-1}

#### addOption

```cpp
void addOption(std::string_view value)
```

Defined in src/symple/include/icy/symple/form.h:249

Adds an option whose key and display value are identical. 
#### Parameters
* `value` Option string.

---

{#setvalue-4}

#### setValue

```cpp
void setValue(std::string_view value)
```

Defined in src/symple/include/icy/symple/form.h:253

Replaces all current values with a single string value. 
#### Parameters
* `value` String value to set.

---

{#setvalue-5}

#### setValue

```cpp
void setValue(int value)
```

Defined in src/symple/include/icy/symple/form.h:257

Replaces all current values with a single integer value. 
#### Parameters
* `value` Integer value to set.

---

{#setvalue-6}

#### setValue

```cpp
void setValue(double value)
```

Defined in src/symple/include/icy/symple/form.h:261

Replaces all current values with a single double value. 
#### Parameters
* `value` Double value to set.

---

{#setvalue-7}

#### setValue

```cpp
void setValue(bool value)
```

Defined in src/symple/include/icy/symple/form.h:265

Replaces all current values with a single boolean value. 
#### Parameters
* `value` Boolean value to set.

---

{#addvalue}

#### addValue

```cpp
void addValue(std::string_view value)
```

Defined in src/symple/include/icy/symple/form.h:269

Appends a string value to the values array. 
#### Parameters
* `value` String value to append.

---

{#addvalue-1}

#### addValue

```cpp
void addValue(int value)
```

Defined in src/symple/include/icy/symple/form.h:273

Appends an integer value to the values array. 
#### Parameters
* `value` Integer value to append.

---

{#addvalue-2}

#### addValue

```cpp
void addValue(double value)
```

Defined in src/symple/include/icy/symple/form.h:277

Appends a double value to the values array. 
#### Parameters
* `value` Double value to append.

---

{#addvalue-3}

#### addValue

```cpp
void addValue(bool value)
```

Defined in src/symple/include/icy/symple/form.h:281

Appends a boolean value to the values array. 
#### Parameters
* `value` Boolean value to append.

---

{#values}

#### values

```cpp
json::Value & values()
```

Defined in src/symple/include/icy/symple/form.h:284

Returns a reference to the JSON array of all values.

---

{#value-4}

#### value

`const` `nodiscard`

```cpp
[[nodiscard]] std::string value() const
```

Defined in src/symple/include/icy/symple/form.h:288

Returns the first value as a string. Most field types (except multi-value) only use a single value.

---

{#intvalue}

#### intValue

`const` `nodiscard`

```cpp
[[nodiscard]] int intValue() const
```

Defined in src/symple/include/icy/symple/form.h:291

Returns the first value parsed as an integer.

---

{#doublevalue}

#### doubleValue

`const` `nodiscard`

```cpp
[[nodiscard]] double doubleValue() const
```

Defined in src/symple/include/icy/symple/form.h:294

Returns the first value parsed as a double.

---

{#boolvalue}

#### boolValue

`const` `nodiscard`

```cpp
[[nodiscard]] bool boolValue() const
```

Defined in src/symple/include/icy/symple/form.h:298

Returns the first value parsed as a boolean. Treats "1", "true", and "on" as true; all other strings as false.

{#message-10}

## Message

```cpp
#include <icy/symple/message.h>
```

```cpp
class Message
```

Defined in src/symple/include/icy/symple/message.h:27

> **Inherits:** `Value`, [`IPacket`](base.md#ipacket)
> **Subclassed by:** [`Command`](#command), [`Event`](#event-2), [`Presence`](#presence)

Base Symple protocol message with addressing, status, data, and notes.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`Message`](#message-11) | `function` | Declared here |
| [`Message`](#message-12) | `function` | Declared here |
| [`Message`](#message-13) | `function` | Declared here |
| [`clone`](#clone-18) | `function` | Declared here |
| [`valid`](#valid-15) | `function` | Declared here |
| [`clear`](#clear-4) | `function` | Declared here |
| [`clearData`](#cleardata) | `function` | Declared here |
| [`clearNotes`](#clearnotes) | `function` | Declared here |
| [`type`](#type-22) | `function` | Declared here |
| [`id`](#id-7) | `function` | Declared here |
| [`to`](#to) | `function` | Declared here |
| [`from`](#from) | `function` | Declared here |
| [`toUser`](#touser) | `function` | Declared here |
| [`toId`](#toid) | `function` | Declared here |
| [`fromUser`](#fromuser) | `function` | Declared here |
| [`fromId`](#fromid) | `function` | Declared here |
| [`status`](#status-3) | `function` | Declared here |
| [`setType`](#settype-2) | `function` | Declared here |
| [`setTo`](#setto) | `function` | Declared here |
| [`setTo`](#setto-1) | `function` | Declared here |
| [`setTo`](#setto-2) | `function` | Declared here |
| [`setFrom`](#setfrom) | `function` | Declared here |
| [`setFrom`](#setfrom-1) | `function` | Declared here |
| [`setFrom`](#setfrom-2) | `function` | Declared here |
| [`setStatus`](#setstatus-1) | `function` | Declared here |
| [`notes`](#notes) | `function` | Declared here |
| [`setNote`](#setnote) | `function` | Declared here |
| [`addNote`](#addnote) | `function` | Declared here |
| [`data`](#data-4) | `function` | Declared here |
| [`data`](#data-5) | `function` | Declared here |
| [`setData`](#setdata) | `function` | Declared here |
| [`setData`](#setdata-1) | `function` | Declared here |
| [`setData`](#setdata-2) | `function` | Declared here |
| [`setData`](#setdata-3) | `function` | Declared here |
| [`setData`](#setdata-4) | `function` | Declared here |
| [`removeData`](#removedata) | `function` | Declared here |
| [`hasData`](#hasdata) | `function` | Declared here |
| [`read`](#read-13) | `function` | Declared here |
| [`read`](#read-14) | `function` | Declared here |
| [`write`](#write-28) | `function` | Declared here |
| [`isRequest`](#isrequest) | `function` | Declared here |
| [`size`](#size-12) | `function` | Declared here |
| [`print`](#print-16) | `function` | Declared here |
| [`className`](#classname-8) | `function` | Declared here |
| [`data`](#data-6) | `function` | Declared here |
| [`hasData`](#hasdata-1) | `function` | Declared here |
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

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Message`](#message-11)  | Constructs a new message with a random ID and type set to "message". |
|  | [`Message`](#message-12)  | Constructs a message from a JSON value; sets missing id/type fields. |
|  | [`Message`](#message-13)  | Copy constructor; preserves or generates id/type fields. |
| `std::unique_ptr< IPacket >` | [`clone`](#clone-18) `virtual` `const` `override` | Returns a heap-allocated copy of this message. |
| `bool` | [`valid`](#valid-15) `virtual` `const` | Returns true if the message has both `[type](#type-22)` and `[id](#id-7)` fields. |
| `void` | [`clear`](#clear-4) `virtual` | Clears all JSON fields from this message. |
| `void` | [`clearData`](#cleardata) `virtual` | Clears the `[data](#data-4)` sub-object. |
| `void` | [`clearNotes`](#clearnotes) `virtual` | Clears the `[notes](#notes)` array. |
| `std::string` | [`type`](#type-22) `const` `nodiscard` | Returns the message type string (defaults to "message"). |
| `std::string` | [`id`](#id-7) `const` `nodiscard` | Returns the message ID string. |
| `Address` | [`to`](#to) `const` `nodiscard` | Returns the recipient address parsed from the `[to](#to)` field. |
| `Address` | [`from`](#from) `const` `nodiscard` | Returns the sender address parsed from the `[from](#from)` field. |
| `std::string` | [`toUser`](#touser) `const` `nodiscard` | Returns the user component of the `[to](#to)` address without constructing an [Address](#address-13). |
| `std::string` | [`toId`](#toid) `const` `nodiscard` | Returns the id component of the `[to](#to)` address without constructing an [Address](#address-13). |
| `std::string` | [`fromUser`](#fromuser) `const` `nodiscard` | Returns the user component of the `[from](#from)` address without constructing an [Address](#address-13). |
| `std::string` | [`fromId`](#fromid) `const` `nodiscard` | Returns the id component of the `[from](#from)` address without constructing an [Address](#address-13). |
| `int` | [`status`](#status-3) `const` `nodiscard` | Returns the HTTP status code, or -1 if not set. |
| `void` | [`setType`](#settype-2)  | Sets the message type field. |
| `void` | [`setTo`](#setto)  | Sets the `[to](#to)` field from a peer's address. |
| `void` | [`setTo`](#setto-1)  | Sets the `[to](#to)` field from an address object. |
| `void` | [`setTo`](#setto-2)  | Sets the `[to](#to)` field from an address string. |
| `void` | [`setFrom`](#setfrom)  | Sets the `[from](#from)` field from a peer's address. |
| `void` | [`setFrom`](#setfrom-1)  | Sets the `[from](#from)` field from an address object. |
| `void` | [`setFrom`](#setfrom-2)  | Sets the `[from](#from)` field from an address string. |
| `void` | [`setStatus`](#setstatus-1)  | HTTP status codes are used to describe the message response. Throws std::invalid_argument if code is outside [101, 504]. |
| `json::Value &` | [`notes`](#notes)  | Returns a reference to the `[notes](#notes)` JSON array. |
| `void` | [`setNote`](#setnote)  | Replaces all notes with a single note. |
| `void` | [`addNote`](#addnote)  | Appends a note to the `[notes](#notes)` array. |
| `json::Value` | [`data`](#data-4) `const` | Returns a copy of the named data field. |
| `json::Value &` | [`data`](#data-5)  | Returns a reference to the named data field (creates it if absent). |
| `json::Value &` | [`setData`](#setdata)  | Creates or replaces a named data field; returns a reference to it. |
| `void` | [`setData`](#setdata-1)  | Sets a named data field to a C-string value. |
| `void` | [`setData`](#setdata-2)  | Sets a named data field to a string value. |
| `void` | [`setData`](#setdata-3)  | Sets a named data field to a JSON value. |
| `void` | [`setData`](#setdata-4)  | Sets a named data field to an integer value. |
| `void` | [`removeData`](#removedata)  | Removes a named field from the `[data](#data-4)` sub-object. |
| `bool` | [`hasData`](#hasdata)  | Returns true if the named field exists in the `[data](#data-4)` sub-object. |
| `ssize_t` | [`read`](#read-13) `virtual` `override` | Deserialises the message from a raw buffer. |
| `ssize_t` | [`read`](#read-14) `virtual` | Deserialises the message from a JSON string. |
| `void` | [`write`](#write-28) `virtual` `const` `override` | Serialises the message as JSON into a buffer. |
| `bool` | [`isRequest`](#isrequest) `const` `nodiscard` | Returns true if no status code has been set (i.e. [status()](#status-3) == -1). |
| `size_t` | [`size`](#size-12) `virtual` `const` `override` | Returns the serialised JSON size in bytes. |
| `void` | [`print`](#print-16) `virtual` `const` `override` | Pretty-prints the message JSON to the given stream. |
| `const char *` | [`className`](#classname-8) `virtual` `const` `inline` `override` | Returns the class name of this packet type for logging and diagnostics. |
| `char *` | [`data`](#data-6) `virtual` `const` `inline` | The packet data pointer for buffered packets. |
| `bool` | [`hasData`](#hasdata-1) `virtual` `const` `inline` | Returns true if the packet has a non-null data pointer. |

---

{#message-11}

#### Message

```cpp
Message()
```

Defined in src/symple/include/icy/symple/message.h:35

Constructs a new message with a random ID and type set to "message".

---

{#message-12}

#### Message

```cpp
Message(const json::Value & root)
```

Defined in src/symple/include/icy/symple/message.h:39

Constructs a message from a JSON value; sets missing id/type fields. 
#### Parameters
* `root` JSON object to initialise from.

---

{#message-13}

#### Message

```cpp
Message(const Message & root)
```

Defined in src/symple/include/icy/symple/message.h:43

Copy constructor; preserves or generates id/type fields. 
#### Parameters
* `root` Source message.

---

{#clone-18}

#### clone

`virtual` `const` `override`

```cpp
virtual std::unique_ptr< IPacket > clone() const override
```

Defined in src/symple/include/icy/symple/message.h:49

Returns a heap-allocated copy of this message.

##### Reimplements

- [`clone`](base.md#classicy_1_1IPacket_1a0a71c618520cdb93b89f5e29f8f145c7)

---

{#valid-15}

#### valid

`virtual` `const`

```cpp
virtual bool valid() const
```

Defined in src/symple/include/icy/symple/message.h:52

Returns true if the message has both `[type](#type-22)` and `[id](#id-7)` fields.

##### Reimplemented by

- [`valid`](#valid-11)
- [`valid`](#valid-12)

---

{#clear-4}

#### clear

`virtual`

```cpp
virtual void clear()
```

Defined in src/symple/include/icy/symple/message.h:55

Clears all JSON fields from this message.

---

{#cleardata}

#### clearData

`virtual`

```cpp
virtual void clearData()
```

Defined in src/symple/include/icy/symple/message.h:58

Clears the `[data](#data-4)` sub-object.

---

{#clearnotes}

#### clearNotes

`virtual`

```cpp
virtual void clearNotes()
```

Defined in src/symple/include/icy/symple/message.h:61

Clears the `[notes](#notes)` array.

---

{#type-22}

#### type

`const` `nodiscard`

```cpp
[[nodiscard]] std::string type() const
```

Defined in src/symple/include/icy/symple/message.h:64

Returns the message type string (defaults to "message").

---

{#id-7}

#### id

`const` `nodiscard`

```cpp
[[nodiscard]] std::string id() const
```

Defined in src/symple/include/icy/symple/message.h:67

Returns the message ID string.

---

{#to}

#### to

`const` `nodiscard`

```cpp
[[nodiscard]] Address to() const
```

Defined in src/symple/include/icy/symple/message.h:70

Returns the recipient address parsed from the `[to](#to)` field.

---

{#from}

#### from

`const` `nodiscard`

```cpp
[[nodiscard]] Address from() const
```

Defined in src/symple/include/icy/symple/message.h:73

Returns the sender address parsed from the `[from](#from)` field.

---

{#touser}

#### toUser

`const` `nodiscard`

```cpp
[[nodiscard]] std::string toUser() const
```

Defined in src/symple/include/icy/symple/message.h:76

Returns the user component of the `[to](#to)` address without constructing an [Address](#address-13).

---

{#toid}

#### toId

`const` `nodiscard`

```cpp
[[nodiscard]] std::string toId() const
```

Defined in src/symple/include/icy/symple/message.h:79

Returns the id component of the `[to](#to)` address without constructing an [Address](#address-13).

---

{#fromuser}

#### fromUser

`const` `nodiscard`

```cpp
[[nodiscard]] std::string fromUser() const
```

Defined in src/symple/include/icy/symple/message.h:82

Returns the user component of the `[from](#from)` address without constructing an [Address](#address-13).

---

{#fromid}

#### fromId

`const` `nodiscard`

```cpp
[[nodiscard]] std::string fromId() const
```

Defined in src/symple/include/icy/symple/message.h:85

Returns the id component of the `[from](#from)` address without constructing an [Address](#address-13).

---

{#status-3}

#### status

`const` `nodiscard`

```cpp
[[nodiscard]] int status() const
```

Defined in src/symple/include/icy/symple/message.h:88

Returns the HTTP status code, or -1 if not set.

---

{#settype-2}

#### setType

```cpp
void setType(std::string_view type)
```

Defined in src/symple/include/icy/symple/message.h:92

Sets the message type field. 
#### Parameters
* `type` Type string (e.g. "message", "command").

---

{#setto}

#### setTo

```cpp
void setTo(const Peer & to)
```

Defined in src/symple/include/icy/symple/message.h:96

Sets the `[to](#to)` field from a peer's address. 
#### Parameters
* `to` Destination peer.

---

{#setto-1}

#### setTo

```cpp
void setTo(const Address & to)
```

Defined in src/symple/include/icy/symple/message.h:100

Sets the `[to](#to)` field from an address object. 
#### Parameters
* `to` Destination address.

---

{#setto-2}

#### setTo

```cpp
void setTo(std::string_view to)
```

Defined in src/symple/include/icy/symple/message.h:104

Sets the `[to](#to)` field from an address string. 
#### Parameters
* `to` Destination address string.

---

{#setfrom}

#### setFrom

```cpp
void setFrom(const Peer & from)
```

Defined in src/symple/include/icy/symple/message.h:108

Sets the `[from](#from)` field from a peer's address. 
#### Parameters
* `from` Sender peer.

---

{#setfrom-1}

#### setFrom

```cpp
void setFrom(const Address & from)
```

Defined in src/symple/include/icy/symple/message.h:112

Sets the `[from](#from)` field from an address object. 
#### Parameters
* `from` Sender address.

---

{#setfrom-2}

#### setFrom

```cpp
void setFrom(std::string_view from)
```

Defined in src/symple/include/icy/symple/message.h:116

Sets the `[from](#from)` field from an address string. 
#### Parameters
* `from` Sender address string.

---

{#setstatus-1}

#### setStatus

```cpp
void setStatus(int code)
```

Defined in src/symple/include/icy/symple/message.h:122

HTTP status codes are used to describe the message response. Throws std::invalid_argument if code is outside [101, 504]. 
#### Parameters
* `code` HTTP status code. 

**See also**: [http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html](http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html)

---

{#notes}

#### notes

```cpp
json::Value & notes()
```

Defined in src/symple/include/icy/symple/message.h:125

Returns a reference to the `[notes](#notes)` JSON array.

---

{#setnote}

#### setNote

```cpp
void setNote(std::string_view type, std::string_view text)
```

Defined in src/symple/include/icy/symple/message.h:130

Replaces all notes with a single note. 
#### Parameters
* `type` Note severity: `[info](base.md#classicy_1_1IPacket_1ac2231c52e60bf60b402bc95cc9d6607d)`, `warn`, or `error`. 

* `text` Note message text.

---

{#addnote}

#### addNote

```cpp
void addNote(std::string_view type, std::string_view text)
```

Defined in src/symple/include/icy/symple/message.h:135

Appends a note to the `[notes](#notes)` array. 
#### Parameters
* `type` Note severity: `[info](base.md#classicy_1_1IPacket_1ac2231c52e60bf60b402bc95cc9d6607d)`, `warn`, or `error`. 

* `text` Note message text.

---

{#data-4}

#### data

`const`

```cpp
json::Value data(std::string_view name) const
```

Defined in src/symple/include/icy/symple/message.h:139

Returns a copy of the named data field. 
#### Parameters
* `name` Field name within `[data](#data-4)`.

---

{#data-5}

#### data

```cpp
json::Value & data(std::string_view name)
```

Defined in src/symple/include/icy/symple/message.h:143

Returns a reference to the named data field (creates it if absent). 
#### Parameters
* `name` Field name within `[data](#data-4)`.

---

{#setdata}

#### setData

```cpp
json::Value & setData(std::string_view name)
```

Defined in src/symple/include/icy/symple/message.h:147

Creates or replaces a named data field; returns a reference to it. 
#### Parameters
* `name` Field name within `[data](#data-4)`.

---

{#setdata-1}

#### setData

```cpp
void setData(std::string_view name, const char * data)
```

Defined in src/symple/include/icy/symple/message.h:152

Sets a named data field to a C-string value. 
#### Parameters
* `name` Field name within `[data](#data-4)`. 

* `data` String value to assign.

---

{#setdata-2}

#### setData

```cpp
void setData(std::string_view name, std::string_view data)
```

Defined in src/symple/include/icy/symple/message.h:157

Sets a named data field to a string value. 
#### Parameters
* `name` Field name within `[data](#data-4)`. 

* `data` String value to assign.

---

{#setdata-3}

#### setData

```cpp
void setData(std::string_view name, const json::Value & data)
```

Defined in src/symple/include/icy/symple/message.h:162

Sets a named data field to a JSON value. 
#### Parameters
* `name` Field name within `[data](#data-4)`. 

* `data` JSON value to assign.

---

{#setdata-4}

#### setData

```cpp
void setData(std::string_view name, int data)
```

Defined in src/symple/include/icy/symple/message.h:167

Sets a named data field to an integer value. 
#### Parameters
* `name` Field name within `[data](#data-4)`. 

* `data` Integer value to assign.

---

{#removedata}

#### removeData

```cpp
void removeData(std::string_view name)
```

Defined in src/symple/include/icy/symple/message.h:171

Removes a named field from the `[data](#data-4)` sub-object. 
#### Parameters
* `name` Field name to remove.

---

{#hasdata}

#### hasData

```cpp
bool hasData(std::string_view name)
```

Defined in src/symple/include/icy/symple/message.h:175

Returns true if the named field exists in the `[data](#data-4)` sub-object. 
#### Parameters
* `name` Field name to look up.

---

{#read-13}

#### read

`virtual` `override`

```cpp
virtual ssize_t read(const ConstBuffer & buf) override
```

Defined in src/symple/include/icy/symple/message.h:180

Deserialises the message from a raw buffer. 
#### Parameters
* `buf` [Buffer](base.md#buffer-2) containing the JSON payload. 

#### Returns
Number of bytes consumed.

##### Reimplements

- [`read`](base.md#classicy_1_1IPacket_1ad42e0c52a5092acb3dd1488928902c5b)

---

{#read-14}

#### read

`virtual`

```cpp
virtual ssize_t read(const std::string & root)
```

Defined in src/symple/include/icy/symple/message.h:185

Deserialises the message from a JSON string. 
#### Parameters
* `root` JSON string to parse. 

#### Returns
Number of characters consumed.

---

{#write-28}

#### write

`virtual` `const` `override`

```cpp
virtual void write(Buffer & buf) const override
```

Defined in src/symple/include/icy/symple/message.h:189

Serialises the message as JSON into a buffer. 
#### Parameters
* `buf` [Buffer](base.md#buffer-2) to append to.

##### Reimplements

- [`write`](base.md#classicy_1_1IPacket_1a87b978fc87c58eb05e1c01ad1ca60f4c)

---

{#isrequest}

#### isRequest

`const` `nodiscard`

```cpp
[[nodiscard]] bool isRequest() const
```

Defined in src/symple/include/icy/symple/message.h:192

Returns true if no status code has been set (i.e. [status()](#status-3) == -1).

---

{#size-12}

#### size

`virtual` `const` `override`

```cpp
virtual size_t size() const override
```

Defined in src/symple/include/icy/symple/message.h:195

Returns the serialised JSON size in bytes.

##### Reimplements

- [`size`](base.md#classicy_1_1IPacket_1a86bfdad925cd6943165c3f8b4637db8c)

---

{#print-16}

#### print

`virtual` `const` `override`

```cpp
virtual void print(std::ostream & os) const override
```

Defined in src/symple/include/icy/symple/message.h:199

Pretty-prints the message JSON to the given stream. 
#### Parameters
* `os` Output stream.

##### Reimplements

- [`print`](base.md#classicy_1_1IPacket_1acc8685ec343b3e9ed81687b8e8fa1acc)

---

{#classname-8}

#### className

`virtual` `const` `inline` `override`

```cpp
virtual inline const char * className() const override
```

Defined in src/symple/include/icy/symple/message.h:201

Returns the class name of this packet type for logging and diagnostics.

##### Reimplements

- [`className`](base.md#classicy_1_1IPacket_1abbbdc14886e839a7c09bd0eeac423ee1)

---

{#data-6}

#### data

`virtual` `const` `inline`

```cpp
virtual inline char * data() const
```

Defined in src/symple/include/icy/symple/message.h:31

The packet data pointer for buffered packets.

##### Reimplements

- [`data`](base.md#classicy_1_1IPacket_1a2e31cef156789c02205ec5c470d8c951)

---

{#hasdata-1}

#### hasData

`virtual` `const` `inline`

```cpp
virtual inline bool hasData() const
```

Defined in src/symple/include/icy/symple/message.h:32

Returns true if the packet has a non-null data pointer.

##### Reimplements

- [`hasData`](base.md#classicy_1_1IPacket_1afacbc458f0ce7820ef1345d85f7d6f80)

{#peer-1}

## Peer

```cpp
#include <icy/symple/peer.h>
```

```cpp
class Peer
```

Defined in src/symple/include/icy/symple/peer.h:30

> **Inherits:** `Value`

Symple peer record containing identity, presence, and custom fields.

A [Peer](#peer-1) object may also contain arbitrary application data set by the client to share with other peers on the network. **See also**: [Address](#address-13) for further methods and [basic](base.md#basic) accessors.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`operator<<`](#operator-38) | `friend` | Declared here |
| [`Peer`](#peer-2) | `function` | Declared here |
| [`Peer`](#peer-3) | `function` | Declared here |
| [`operator=`](#operator-39) | `function` | Declared here |
| [`Peer`](#peer-4) | `function` | Declared here |
| [`address`](#address-17) | `function` | Declared here |
| [`id`](#id-8) | `function` | Declared here |
| [`user`](#user-2) | `function` | Declared here |
| [`name`](#name-14) | `function` | Declared here |
| [`type`](#type-23) | `function` | Declared here |
| [`host`](#host-3) | `function` | Declared here |
| [`setID`](#setid-1) | `function` | Declared here |
| [`setUser`](#setuser) | `function` | Declared here |
| [`setName`](#setname-3) | `function` | Declared here |
| [`setType`](#settype-3) | `function` | Declared here |
| [`setHost`](#sethost-2) | `function` | Declared here |
| [`valid`](#valid-16) | `function` | Declared here |
| [`print`](#print-17) | `function` | Declared here |
| [`className`](#classname-9) | `function` | Declared here |

### Friends

| Name | Description |
|------|-------------|
| [`operator<<`](#operator-38) `inline` |  |

---

{#operator-38}

#### operator<<

`inline`

```cpp
friend inline std::ostream & operator<<(std::ostream & os, const Peer & peer)
```

Defined in src/symple/include/icy/symple/peer.h:101

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Peer`](#peer-2)  | Constructs an empty peer with type set to "Peer". |
|  | [`Peer`](#peer-3)  | Copy constructor; preserves type field. |
| `Peer &` | [`operator=`](#operator-39)  | Copy-assigns peer data from another peer. |
|  | [`Peer`](#peer-4)  | Constructs a peer from a raw JSON value. |
| `Address` | [`address`](#address-17) `const` `nodiscard` | Returns the peer's address (user + session ID). |
| `std::string` | [`id`](#id-8) `const` `nodiscard` | Returns the session ID assigned by the server. |
| `std::string` | [`user`](#user-2) `const` `nodiscard` | Returns the user identifier. |
| `std::string` | [`name`](#name-14) `const` `nodiscard` | Returns the display name. |
| `std::string` | [`type`](#type-23) `const` `nodiscard` | Returns the peer type string (e.g. "Peer", "bot"). |
| `std::string` | [`host`](#host-3) `const` `nodiscard` | Returns the host address associated with this peer. |
| `void` | [`setID`](#setid-1)  | Sets the session ID field. |
| `void` | [`setUser`](#setuser)  | Sets the user identifier field. |
| `void` | [`setName`](#setname-3)  | Sets the display name field. |
| `void` | [`setType`](#settype-3)  | Sets the peer type field. |
| `void` | [`setHost`](#sethost-2)  | Sets the host address field. |
| `bool` | [`valid`](#valid-16) `virtual` | Updates the peer from the given data object. |
| `void` | [`print`](#print-17) `const` | Writes the peer's JSON representation to the given stream. |
| `const char *` | [`className`](#classname-9) `virtual` `const` `inline` |  |

---

{#peer-2}

#### Peer

```cpp
Peer()
```

Defined in src/symple/include/icy/symple/peer.h:34

Constructs an empty peer with type set to "Peer".

---

{#peer-3}

#### Peer

```cpp
Peer(const Peer & r)
```

Defined in src/symple/include/icy/symple/peer.h:38

Copy constructor; preserves type field. 
#### Parameters
* `r` Source peer.

---

{#operator-39}

#### operator=

```cpp
Peer & operator=(const Peer & r)
```

Defined in src/symple/include/icy/symple/peer.h:42

Copy-assigns peer data from another peer. 
#### Parameters
* `r` Source peer.

---

{#peer-4}

#### Peer

```cpp
Peer(const json::Value & r)
```

Defined in src/symple/include/icy/symple/peer.h:46

Constructs a peer from a raw JSON value. 
#### Parameters
* `r` JSON object containing peer fields.

---

{#address-17}

#### address

`const` `nodiscard`

```cpp
[[nodiscard]] Address address() const
```

Defined in src/symple/include/icy/symple/peer.h:52

Returns the peer's address (user + session ID). 
#### Returns
[Address](#address-13) constructed from the `[user](#user-2)` and `[id](#id-8)` fields.

---

{#id-8}

#### id

`const` `nodiscard`

```cpp
[[nodiscard]] std::string id() const
```

Defined in src/symple/include/icy/symple/peer.h:55

Returns the session ID assigned by the server.

---

{#user-2}

#### user

`const` `nodiscard`

```cpp
[[nodiscard]] std::string user() const
```

Defined in src/symple/include/icy/symple/peer.h:58

Returns the user identifier.

---

{#name-14}

#### name

`const` `nodiscard`

```cpp
[[nodiscard]] std::string name() const
```

Defined in src/symple/include/icy/symple/peer.h:61

Returns the display name.

---

{#type-23}

#### type

`const` `nodiscard`

```cpp
[[nodiscard]] std::string type() const
```

Defined in src/symple/include/icy/symple/peer.h:65

Returns the peer type string (e.g. "Peer", "bot").

---

{#host-3}

#### host

`const` `nodiscard`

```cpp
[[nodiscard]] std::string host() const
```

Defined in src/symple/include/icy/symple/peer.h:68

Returns the host address associated with this peer.

---

{#setid-1}

#### setID

```cpp
void setID(std::string_view id)
```

Defined in src/symple/include/icy/symple/peer.h:72

Sets the session ID field. 
#### Parameters
* `id` Session ID string.

---

{#setuser}

#### setUser

```cpp
void setUser(std::string_view user)
```

Defined in src/symple/include/icy/symple/peer.h:76

Sets the user identifier field. 
#### Parameters
* `user` User identifier string.

---

{#setname-3}

#### setName

```cpp
void setName(std::string_view name)
```

Defined in src/symple/include/icy/symple/peer.h:80

Sets the display name field. 
#### Parameters
* `name` Display name string.

---

{#settype-3}

#### setType

```cpp
void setType(std::string_view type)
```

Defined in src/symple/include/icy/symple/peer.h:85

Sets the peer type field. 
#### Parameters
* `type` Type string.

---

{#sethost-2}

#### setHost

```cpp
void setHost(std::string_view host)
```

Defined in src/symple/include/icy/symple/peer.h:89

Sets the host address field. 
#### Parameters
* `host` Host address string.

---

{#valid-16}

#### valid

`virtual`

```cpp
virtual bool valid()
```

Defined in src/symple/include/icy/symple/peer.h:95

Updates the peer from the given data object.

Returns true if the peer has `[id](#id-8)`, `[user](#user-2)`, and `[type](#type-23)` fields.

---

{#print-17}

#### print

`const`

```cpp
void print(std::ostream & os) const
```

Defined in src/symple/include/icy/symple/peer.h:99

Writes the peer's JSON representation to the given stream. 
#### Parameters
* `os` Output stream.

---

{#classname-9}

#### className

`virtual` `const` `inline`

```cpp
virtual inline const char * className() const
```

Defined in src/symple/include/icy/symple/peer.h:107

{#presence}

## Presence

```cpp
#include <icy/symple/presence.h>
```

```cpp
class Presence
```

Defined in src/symple/include/icy/symple/presence.h:27

> **Inherits:** [`Message`](#message-10)

Symple presence message indicating a peer's online status.

[Presence](#presence) messages carry peer data in the `[data](#data-4)` field. When `probe` is true the recipient should respond with their own presence.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`Presence`](#presence-1) | `function` | Declared here |
| [`Presence`](#presence-2) | `function` | Declared here |
| [`Presence`](#presence-3) | `function` | Declared here |
| [`isProbe`](#isprobe) | `function` | Declared here |
| [`setProbe`](#setprobe) | `function` | Declared here |
| [`Message`](#message-11) | `function` | Inherited from [`Message`](#message-10) |
| [`Message`](#message-12) | `function` | Inherited from [`Message`](#message-10) |
| [`Message`](#message-13) | `function` | Inherited from [`Message`](#message-10) |
| [`clone`](#clone-18) | `function` | Inherited from [`Message`](#message-10) |
| [`valid`](#valid-15) | `function` | Inherited from [`Message`](#message-10) |
| [`clear`](#clear-4) | `function` | Inherited from [`Message`](#message-10) |
| [`clearData`](#cleardata) | `function` | Inherited from [`Message`](#message-10) |
| [`clearNotes`](#clearnotes) | `function` | Inherited from [`Message`](#message-10) |
| [`type`](#type-22) | `function` | Inherited from [`Message`](#message-10) |
| [`id`](#id-7) | `function` | Inherited from [`Message`](#message-10) |
| [`to`](#to) | `function` | Inherited from [`Message`](#message-10) |
| [`from`](#from) | `function` | Inherited from [`Message`](#message-10) |
| [`toUser`](#touser) | `function` | Inherited from [`Message`](#message-10) |
| [`toId`](#toid) | `function` | Inherited from [`Message`](#message-10) |
| [`fromUser`](#fromuser) | `function` | Inherited from [`Message`](#message-10) |
| [`fromId`](#fromid) | `function` | Inherited from [`Message`](#message-10) |
| [`status`](#status-3) | `function` | Inherited from [`Message`](#message-10) |
| [`setType`](#settype-2) | `function` | Inherited from [`Message`](#message-10) |
| [`setTo`](#setto) | `function` | Inherited from [`Message`](#message-10) |
| [`setTo`](#setto-1) | `function` | Inherited from [`Message`](#message-10) |
| [`setTo`](#setto-2) | `function` | Inherited from [`Message`](#message-10) |
| [`setFrom`](#setfrom) | `function` | Inherited from [`Message`](#message-10) |
| [`setFrom`](#setfrom-1) | `function` | Inherited from [`Message`](#message-10) |
| [`setFrom`](#setfrom-2) | `function` | Inherited from [`Message`](#message-10) |
| [`setStatus`](#setstatus-1) | `function` | Inherited from [`Message`](#message-10) |
| [`notes`](#notes) | `function` | Inherited from [`Message`](#message-10) |
| [`setNote`](#setnote) | `function` | Inherited from [`Message`](#message-10) |
| [`addNote`](#addnote) | `function` | Inherited from [`Message`](#message-10) |
| [`data`](#data-4) | `function` | Inherited from [`Message`](#message-10) |
| [`data`](#data-5) | `function` | Inherited from [`Message`](#message-10) |
| [`setData`](#setdata) | `function` | Inherited from [`Message`](#message-10) |
| [`setData`](#setdata-1) | `function` | Inherited from [`Message`](#message-10) |
| [`setData`](#setdata-2) | `function` | Inherited from [`Message`](#message-10) |
| [`setData`](#setdata-3) | `function` | Inherited from [`Message`](#message-10) |
| [`setData`](#setdata-4) | `function` | Inherited from [`Message`](#message-10) |
| [`removeData`](#removedata) | `function` | Inherited from [`Message`](#message-10) |
| [`hasData`](#hasdata) | `function` | Inherited from [`Message`](#message-10) |
| [`read`](#read-13) | `function` | Inherited from [`Message`](#message-10) |
| [`read`](#read-14) | `function` | Inherited from [`Message`](#message-10) |
| [`write`](#write-28) | `function` | Inherited from [`Message`](#message-10) |
| [`isRequest`](#isrequest) | `function` | Inherited from [`Message`](#message-10) |
| [`size`](#size-12) | `function` | Inherited from [`Message`](#message-10) |
| [`print`](#print-16) | `function` | Inherited from [`Message`](#message-10) |
| [`className`](#classname-8) | `function` | Inherited from [`Message`](#message-10) |
| [`data`](#data-6) | `function` | Inherited from [`Message`](#message-10) |
| [`hasData`](#hasdata-1) | `function` | Inherited from [`Message`](#message-10) |
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

### Inherited from [`Message`](#message-10)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`Message`](#message-11)  | Constructs a new message with a random ID and type set to "message". |
| `function` | [`Message`](#message-12)  | Constructs a message from a JSON value; sets missing id/type fields. |
| `function` | [`Message`](#message-13)  | Copy constructor; preserves or generates id/type fields. |
| `function` | [`clone`](#clone-18) `virtual` `const` `override` | Returns a heap-allocated copy of this message. |
| `function` | [`valid`](#valid-15) `virtual` `const` | Returns true if the message has both `[type](#type-22)` and `[id](#id-7)` fields. |
| `function` | [`clear`](#clear-4) `virtual` | Clears all JSON fields from this message. |
| `function` | [`clearData`](#cleardata) `virtual` | Clears the `[data](#data-4)` sub-object. |
| `function` | [`clearNotes`](#clearnotes) `virtual` | Clears the `[notes](#notes)` array. |
| `function` | [`type`](#type-22) `const` `nodiscard` | Returns the message type string (defaults to "message"). |
| `function` | [`id`](#id-7) `const` `nodiscard` | Returns the message ID string. |
| `function` | [`to`](#to) `const` `nodiscard` | Returns the recipient address parsed from the `[to](#to)` field. |
| `function` | [`from`](#from) `const` `nodiscard` | Returns the sender address parsed from the `[from](#from)` field. |
| `function` | [`toUser`](#touser) `const` `nodiscard` | Returns the user component of the `[to](#to)` address without constructing an [Address](#address-13). |
| `function` | [`toId`](#toid) `const` `nodiscard` | Returns the id component of the `[to](#to)` address without constructing an [Address](#address-13). |
| `function` | [`fromUser`](#fromuser) `const` `nodiscard` | Returns the user component of the `[from](#from)` address without constructing an [Address](#address-13). |
| `function` | [`fromId`](#fromid) `const` `nodiscard` | Returns the id component of the `[from](#from)` address without constructing an [Address](#address-13). |
| `function` | [`status`](#status-3) `const` `nodiscard` | Returns the HTTP status code, or -1 if not set. |
| `function` | [`setType`](#settype-2)  | Sets the message type field. |
| `function` | [`setTo`](#setto)  | Sets the `[to](#to)` field from a peer's address. |
| `function` | [`setTo`](#setto-1)  | Sets the `[to](#to)` field from an address object. |
| `function` | [`setTo`](#setto-2)  | Sets the `[to](#to)` field from an address string. |
| `function` | [`setFrom`](#setfrom)  | Sets the `[from](#from)` field from a peer's address. |
| `function` | [`setFrom`](#setfrom-1)  | Sets the `[from](#from)` field from an address object. |
| `function` | [`setFrom`](#setfrom-2)  | Sets the `[from](#from)` field from an address string. |
| `function` | [`setStatus`](#setstatus-1)  | HTTP status codes are used to describe the message response. Throws std::invalid_argument if code is outside [101, 504]. |
| `function` | [`notes`](#notes)  | Returns a reference to the `[notes](#notes)` JSON array. |
| `function` | [`setNote`](#setnote)  | Replaces all notes with a single note. |
| `function` | [`addNote`](#addnote)  | Appends a note to the `[notes](#notes)` array. |
| `function` | [`data`](#data-4) `const` | Returns a copy of the named data field. |
| `function` | [`data`](#data-5)  | Returns a reference to the named data field (creates it if absent). |
| `function` | [`setData`](#setdata)  | Creates or replaces a named data field; returns a reference to it. |
| `function` | [`setData`](#setdata-1)  | Sets a named data field to a C-string value. |
| `function` | [`setData`](#setdata-2)  | Sets a named data field to a string value. |
| `function` | [`setData`](#setdata-3)  | Sets a named data field to a JSON value. |
| `function` | [`setData`](#setdata-4)  | Sets a named data field to an integer value. |
| `function` | [`removeData`](#removedata)  | Removes a named field from the `[data](#data-4)` sub-object. |
| `function` | [`hasData`](#hasdata)  | Returns true if the named field exists in the `[data](#data-4)` sub-object. |
| `function` | [`read`](#read-13) `virtual` `override` | Deserialises the message from a raw buffer. |
| `function` | [`read`](#read-14) `virtual` | Deserialises the message from a JSON string. |
| `function` | [`write`](#write-28) `virtual` `const` `override` | Serialises the message as JSON into a buffer. |
| `function` | [`isRequest`](#isrequest) `const` `nodiscard` | Returns true if no status code has been set (i.e. [status()](#status-3) == -1). |
| `function` | [`size`](#size-12) `virtual` `const` `override` | Returns the serialised JSON size in bytes. |
| `function` | [`print`](#print-16) `virtual` `const` `override` | Pretty-prints the message JSON to the given stream. |
| `function` | [`className`](#classname-8) `virtual` `const` `inline` `override` | Returns the class name of this packet type for logging and diagnostics. |
| `function` | [`data`](#data-6) `virtual` `const` `inline` | The packet data pointer for buffered packets. |
| `function` | [`hasData`](#hasdata-1) `virtual` `const` `inline` | Returns true if the packet has a non-null data pointer. |

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

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Presence`](#presence-1)  | Constructs a presence message with type set to "presence". |
|  | [`Presence`](#presence-2)  | Constructs a presence message from a JSON value. |
|  | [`Presence`](#presence-3)  | Copy constructor. |
| `bool` | [`isProbe`](#isprobe)  | Returns true if this is a presence probe request. Recipients of a probe should send back their own presence. |
| `void` | [`setProbe`](#setprobe)  | Sets or clears the probe flag on this presence message. |

---

{#presence-1}

#### Presence

```cpp
Presence()
```

Defined in src/symple/include/icy/symple/presence.h:31

Constructs a presence message with type set to "presence".

---

{#presence-2}

#### Presence

```cpp
Presence(const json::Value & root)
```

Defined in src/symple/include/icy/symple/presence.h:35

Constructs a presence message from a JSON value. 
#### Parameters
* `root` JSON object to initialise from.

---

{#presence-3}

#### Presence

```cpp
Presence(const Presence & root)
```

Defined in src/symple/include/icy/symple/presence.h:39

Copy constructor. 
#### Parameters
* `root` Source presence message.

---

{#isprobe}

#### isProbe

```cpp
bool isProbe()
```

Defined in src/symple/include/icy/symple/presence.h:45

Returns true if this is a presence probe request. Recipients of a probe should send back their own presence.

---

{#setprobe}

#### setProbe

```cpp
void setProbe(bool flag)
```

Defined in src/symple/include/icy/symple/presence.h:49

Sets or clears the probe flag on this presence message. 
#### Parameters
* `flag` True to mark this as a probe.

{#roster-2}

## Roster

```cpp
#include <icy/symple/roster.h>
```

```cpp
class Roster
```

Defined in src/symple/include/icy/symple/roster.h:29

> **Inherits:** [`string, Peer >`](base.md#keyedstore)

The [Roster](#roster-2) provides a registry for active network peers indexed by session ID.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`PeerAdded`](#peeradded) | `variable` | Declared here |
| [`PeerRemoved`](#peerremoved) | `variable` | Declared here |
| [`Roster`](#roster-3) | `function` | Declared here |
| [`getByHost`](#getbyhost) | `function` | Declared here |
| [`peers`](#peers) | `function` | Declared here |
| [`print`](#print-18) | `function` | Declared here |
| [`className`](#classname-10) | `function` | Declared here |
| [`onAdd`](#onadd) | `function` | Declared here |
| [`onRemove`](#onremove) | `function` | Declared here |
| [`Map`](base.md#classicy_1_1KeyedStore_1a36d59fd48cab6c0964ec71601c705ac0) | `typedef` | Inherited from [`KeyedStore`](base.md#keyedstore) |
| [`_map`](base.md#classicy_1_1KeyedStore_1a13893537ec3ed8f225451bd21ecc61f8) | `variable` | Inherited from [`KeyedStore`](base.md#keyedstore) |
| [`KeyedStore`](base.md#classicy_1_1KeyedStore_1a38afd30d2e186ebeaeabe1400d15a754) | `function` | Inherited from [`KeyedStore`](base.md#keyedstore) |
| [`~KeyedStore`](base.md#classicy_1_1KeyedStore_1a3c6b29a646771b0e0221873afce1e7b2) | `function` | Inherited from [`KeyedStore`](base.md#keyedstore) |
| [`KeyedStore`](base.md#classicy_1_1KeyedStore_1a7a5eee882aa83b90629a55b5843e7834) | `function` | Inherited from [`KeyedStore`](base.md#keyedstore) |
| [`operator=`](base.md#classicy_1_1KeyedStore_1af09e123a8b3b0e63e3557bf1e37b9cba) | `function` | Inherited from [`KeyedStore`](base.md#keyedstore) |
| [`KeyedStore`](base.md#classicy_1_1KeyedStore_1a02b030b0c8acbeadc8fd273e98e6cc6f) | `function` | Inherited from [`KeyedStore`](base.md#keyedstore) |
| [`operator=`](base.md#classicy_1_1KeyedStore_1a9bea475c7afe32a7ee63b281bc0a182d) | `function` | Inherited from [`KeyedStore`](base.md#keyedstore) |
| [`get`](base.md#classicy_1_1KeyedStore_1a80b4ffad8b24a21d1f56e90ba610cd4e) | `function` | Inherited from [`KeyedStore`](base.md#keyedstore) |
| [`add`](base.md#classicy_1_1KeyedStore_1a773198fbe5a781bb98d5f8e969e063c8) | `function` | Inherited from [`KeyedStore`](base.md#keyedstore) |
| [`tryAdd`](base.md#classicy_1_1KeyedStore_1a9c9cc5e6202efec22967709a55ddca99) | `function` | Inherited from [`KeyedStore`](base.md#keyedstore) |
| [`put`](base.md#classicy_1_1KeyedStore_1a7fe7fb266aefbf815206270b8a13794e) | `function` | Inherited from [`KeyedStore`](base.md#keyedstore) |
| [`erase`](base.md#classicy_1_1KeyedStore_1a0bac5c9b3814e9e5838fded3c9b0b811) | `function` | Inherited from [`KeyedStore`](base.md#keyedstore) |
| [`contains`](base.md#classicy_1_1KeyedStore_1a5b71fda08a410b5bcc62e0c963909656) | `function` | Inherited from [`KeyedStore`](base.md#keyedstore) |
| [`empty`](base.md#classicy_1_1KeyedStore_1ab1055567facaac7db3026732e49e0271) | `function` | Inherited from [`KeyedStore`](base.md#keyedstore) |
| [`size`](base.md#classicy_1_1KeyedStore_1ab0a21578a16ed774614383d3099de919) | `function` | Inherited from [`KeyedStore`](base.md#keyedstore) |
| [`clear`](base.md#classicy_1_1KeyedStore_1aa3d4f575a7ed510d26ee5b733912e642) | `function` | Inherited from [`KeyedStore`](base.md#keyedstore) |
| [`map`](base.md#classicy_1_1KeyedStore_1a72b696edc553ed4d19df38f3c77980f7) | `function` | Inherited from [`KeyedStore`](base.md#keyedstore) |
| [`map`](base.md#classicy_1_1KeyedStore_1ac048ac94fe6c90e6b49104caf154570a) | `function` | Inherited from [`KeyedStore`](base.md#keyedstore) |
| [`begin`](base.md#classicy_1_1KeyedStore_1a1a71782a34581fc6da3a8b727f717e6e) | `function` | Inherited from [`KeyedStore`](base.md#keyedstore) |
| [`end`](base.md#classicy_1_1KeyedStore_1a3130dd9916889fa505c1a62e9e08d6f9) | `function` | Inherited from [`KeyedStore`](base.md#keyedstore) |
| [`begin`](base.md#classicy_1_1KeyedStore_1a2489acbcc1e01460e8b340322349f523) | `function` | Inherited from [`KeyedStore`](base.md#keyedstore) |
| [`end`](base.md#classicy_1_1KeyedStore_1a70d706b031fd3a707c62077df4aa2965) | `function` | Inherited from [`KeyedStore`](base.md#keyedstore) |
| [`onAdd`](base.md#classicy_1_1KeyedStore_1a560a59fffcae55b224ea1783a0da3cec) | `function` | Inherited from [`KeyedStore`](base.md#keyedstore) |
| [`onRemove`](base.md#classicy_1_1KeyedStore_1a0320226740ba9782db07380fd100749a) | `function` | Inherited from [`KeyedStore`](base.md#keyedstore) |

### Inherited from [`KeyedStore`](base.md#keyedstore)

| Kind | Name | Description |
|------|------|-------------|
| `typedef` | [`Map`](base.md#classicy_1_1KeyedStore_1a36d59fd48cab6c0964ec71601c705ac0)  |  |
| `variable` | [`_map`](base.md#classicy_1_1KeyedStore_1a13893537ec3ed8f225451bd21ecc61f8)  |  |
| `function` | [`KeyedStore`](base.md#classicy_1_1KeyedStore_1a38afd30d2e186ebeaeabe1400d15a754)  | Defaulted constructor. |
| `function` | [`~KeyedStore`](base.md#classicy_1_1KeyedStore_1a3c6b29a646771b0e0221873afce1e7b2) `virtual` `inline` |  |
| `function` | [`KeyedStore`](base.md#classicy_1_1KeyedStore_1a7a5eee882aa83b90629a55b5843e7834)  | Deleted constructor. |
| `function` | [`operator=`](base.md#classicy_1_1KeyedStore_1af09e123a8b3b0e63e3557bf1e37b9cba)  | Deleted assignment operator. |
| `function` | [`KeyedStore`](base.md#classicy_1_1KeyedStore_1a02b030b0c8acbeadc8fd273e98e6cc6f)  | Defaulted constructor. |
| `function` | [`operator=`](base.md#classicy_1_1KeyedStore_1a9bea475c7afe32a7ee63b281bc0a182d)  | Defaulted assignment operator. |
| `function` | [`get`](base.md#classicy_1_1KeyedStore_1a80b4ffad8b24a21d1f56e90ba610cd4e) `const` `inline` | Returns the item for `key`, or nullptr if not found. |
| `function` | [`add`](base.md#classicy_1_1KeyedStore_1a773198fbe5a781bb98d5f8e969e063c8) `inline` | Inserts a uniquely owned item. Returns a reference to the stored item. |
| `function` | [`tryAdd`](base.md#classicy_1_1KeyedStore_1a9c9cc5e6202efec22967709a55ddca99) `inline` | Inserts if absent; returns false on duplicate (never throws). |
| `function` | [`put`](base.md#classicy_1_1KeyedStore_1a7fe7fb266aefbf815206270b8a13794e) `inline` | Inserts or replaces the item under `key`. |
| `function` | [`erase`](base.md#classicy_1_1KeyedStore_1a0bac5c9b3814e9e5838fded3c9b0b811) `inline` | Removes and destroys the item under `key`. |
| `function` | [`contains`](base.md#classicy_1_1KeyedStore_1a5b71fda08a410b5bcc62e0c963909656) `const` `inline` |  |
| `function` | [`empty`](base.md#classicy_1_1KeyedStore_1ab1055567facaac7db3026732e49e0271) `const` `inline` |  |
| `function` | [`size`](base.md#classicy_1_1KeyedStore_1ab0a21578a16ed774614383d3099de919) `const` `inline` |  |
| `function` | [`clear`](base.md#classicy_1_1KeyedStore_1aa3d4f575a7ed510d26ee5b733912e642) `inline` |  |
| `function` | [`map`](base.md#classicy_1_1KeyedStore_1a72b696edc553ed4d19df38f3c77980f7) `inline` | Direct map access for iteration. |
| `function` | [`map`](base.md#classicy_1_1KeyedStore_1ac048ac94fe6c90e6b49104caf154570a) `const` `inline` |  |
| `function` | [`begin`](base.md#classicy_1_1KeyedStore_1a1a71782a34581fc6da3a8b727f717e6e) `inline` |  |
| `function` | [`end`](base.md#classicy_1_1KeyedStore_1a3130dd9916889fa505c1a62e9e08d6f9) `inline` |  |
| `function` | [`begin`](base.md#classicy_1_1KeyedStore_1a2489acbcc1e01460e8b340322349f523) `const` `inline` |  |
| `function` | [`end`](base.md#classicy_1_1KeyedStore_1a70d706b031fd3a707c62077df4aa2965) `const` `inline` |  |
| `function` | [`onAdd`](base.md#classicy_1_1KeyedStore_1a560a59fffcae55b224ea1783a0da3cec) `virtual` `inline` | Override for lifecycle reactions. |
| `function` | [`onRemove`](base.md#classicy_1_1KeyedStore_1a0320226740ba9782db07380fd100749a) `virtual` `inline` |  |

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `Signal< void(Peer &)>` | [`PeerAdded`](#peeradded)  | Lifecycle signals for external observers (samples, UI). |
| `Signal< void(const Peer &)>` | [`PeerRemoved`](#peerremoved)  |  |

---

{#peeradded}

#### PeerAdded

```cpp
Signal< void(Peer &)> PeerAdded
```

Defined in src/symple/include/icy/symple/roster.h:46

Lifecycle signals for external observers (samples, UI).

---

{#peerremoved}

#### PeerRemoved

```cpp
Signal< void(const Peer &)> PeerRemoved
```

Defined in src/symple/include/icy/symple/roster.h:47

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Roster`](#roster-3)  |  |
| `Peer *` | [`getByHost`](#getbyhost)  | Returns the first peer which matches the given host address. |
| `Map` | [`peers`](#peers) `const` `nodiscard` | Returns a deep copy of the peer map. |
| `void` | [`print`](#print-18) `const` |  |
| `const char *` | [`className`](#classname-10) `virtual` `const` `inline` |  |

---

{#roster-3}

#### Roster

```cpp
Roster()
```

Defined in src/symple/include/icy/symple/roster.h:32

---

{#getbyhost}

#### getByHost

```cpp
Peer * getByHost(std::string_view host)
```

Defined in src/symple/include/icy/symple/roster.h:36

Returns the first peer which matches the given host address.

---

{#peers}

#### peers

`const` `nodiscard`

```cpp
[[nodiscard]] Map peers() const
```

Defined in src/symple/include/icy/symple/roster.h:39

Returns a deep copy of the peer map.

---

{#print-18}

#### print

`const`

```cpp
void print(std::ostream & os) const
```

Defined in src/symple/include/icy/symple/roster.h:41

---

{#classname-10}

#### className

`virtual` `const` `inline`

```cpp
virtual inline const char * className() const
```

Defined in src/symple/include/icy/symple/roster.h:43

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`onAdd`](#onadd) `inline` `override` |  |
| `void` | [`onRemove`](#onremove) `inline` `override` |  |

---

{#onadd}

#### onAdd

`inline` `override`

```cpp
inline void onAdd(const std::string &, Peer * peer) override
```

Defined in src/symple/include/icy/symple/roster.h:50

---

{#onremove}

#### onRemove

`inline` `override`

```cpp
inline void onRemove(const std::string &, Peer * peer) override
```

Defined in src/symple/include/icy/symple/roster.h:51

{#server-10}

## Server

```cpp
#include <icy/symple/server.h>
```

```cpp
class Server
```

Defined in src/symple/include/icy/symple/server.h:136

Symple v4 server.

Accepts WebSocket connections, authenticates peers, manages rooms, and routes messages. Implements the Symple v4 protocol over native WebSocket.

Usage: [smpl::Server](#server-10) server; server.start({.port = 4500});

// Optional: custom authentication server.Authenticate += []([ServerPeer](#serverpeer)& peer, const [json::Value](json.md#value)& auth, bool& allowed, std::vector<std::string>& rooms) { allowed = (auth.value("token", "") == "secret"); rooms.push_back("team-a"); };

The server also serves as an HTTP server, so you can serve static files (e.g. a web UI) on the same port.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`Factory`](#factory-2) | `friend` | Declared here |
| [`Authenticate`](#authenticate-2) | `variable` | Declared here |
| [`PeerConnected`](#peerconnected-1) | `variable` | Declared here |
| [`PeerDisconnected`](#peerdisconnected-1) | `variable` | Declared here |
| [`Server`](#server-11) | `function` | Declared here |
| [`Server`](#server-12) | `function` | Declared here |
| [`start`](#start-17) | `function` | Declared here |
| [`start`](#start-18) | `function` | Declared here |
| [`stop`](#stop-14) | `function` | Declared here |
| [`broadcast`](#broadcast) | `function` | Declared here |
| [`broadcastRooms`](#broadcastrooms) | `function` | Declared here |
| [`sendTo`](#sendto) | `function` | Declared here |
| [`sendToUser`](#sendtouser) | `function` | Declared here |
| [`getPeer`](#getpeer) | `function` | Declared here |
| [`getPeersInRoom`](#getpeersinroom) | `function` | Declared here |
| [`peerCount`](#peercount) | `function` | Declared here |
| [`addVirtualPeer`](#addvirtualpeer) | `function` | Declared here |
| [`removeVirtualPeer`](#removevirtualpeer) | `function` | Declared here |
| [`httpServer`](#httpserver) | `function` | Declared here |
| [`loop`](#loop-9) | `function` | Declared here |
| [`_opts`](#_opts) | `variable` | Declared here |
| [`_loop`](#_loop-3) | `variable` | Declared here |
| [`_http`](#_http) | `variable` | Declared here |
| [`_peerRegistry`](#_peerregistry) | `variable` | Declared here |
| [`_roomIndex`](#_roomindex) | `variable` | Declared here |
| [`_mutex`](#_mutex-16) | `variable` | Declared here |
| [`_shuttingDown`](#_shuttingdown) | `variable` | Declared here |
| [`_httpFallback`](#_httpfallback) | `variable` | Declared here |
| [`onAuth`](#onauth) | `function` | Declared here |
| [`onMessage`](#onmessage) | `function` | Declared here |
| [`onJoin`](#onjoin) | `function` | Declared here |
| [`onLeave`](#onleave) | `function` | Declared here |
| [`onDisconnect`](#ondisconnect) | `function` | Declared here |
| [`route`](#route) | `function` | Declared here |
| [`deliver`](#deliver) | `function` | Declared here |
| [`deliverSerialized`](#deliverserialized) | `function` | Declared here |
| [`sendPresenceSnapshot`](#sendpresencesnapshot) | `function` | Declared here |

### Friends

| Name | Description |
|------|-------------|
| [`Factory`](#factory-2)  |  |

---

{#factory-2}

#### Factory

```cpp
friend class Factory
```

Defined in src/symple/include/icy/symple/server.h:279

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `Signal< void(ServerPeer &, const json::Value &auth, bool &allowed, std::vector< std::string > &rooms)>` | [`Authenticate`](#authenticate-2)  | Custom authentication hook. Set `allowed` to false to reject the peer. Append to `rooms` to assign team/group memberships. If not connected, all peers with a valid `user` field are accepted. |
| `Signal< void(ServerPeer &)>` | [`PeerConnected`](#peerconnected-1)  | [Peer](#peer-1) authenticated and online. |
| `Signal< void(ServerPeer &)>` | [`PeerDisconnected`](#peerdisconnected-1)  | [Peer](#peer-1) disconnected. |

---

{#authenticate-2}

#### Authenticate

```cpp
Signal< void(ServerPeer &, const json::Value &auth, bool &allowed, std::vector< std::string > &rooms)> Authenticate
```

Defined in src/symple/include/icy/symple/server.h:213

Custom authentication hook. Set `allowed` to false to reject the peer. Append to `rooms` to assign team/group memberships. If not connected, all peers with a valid `user` field are accepted.

---

{#peerconnected-1}

#### PeerConnected

```cpp
Signal< void(ServerPeer &)> PeerConnected
```

Defined in src/symple/include/icy/symple/server.h:216

[Peer](#peer-1) authenticated and online.

---

{#peerdisconnected-1}

#### PeerDisconnected

```cpp
Signal< void(ServerPeer &)> PeerDisconnected
```

Defined in src/symple/include/icy/symple/server.h:219

[Peer](#peer-1) disconnected.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Server`](#server-11)  | Constructs a server using the given event loop. |
|  | [`Server`](#server-12)  | Deleted constructor. |
| `void` | [`start`](#start-17)  | Starts the server with the given options. Begins accepting WebSocket connections on opts.host:opts.port. |
| `void` | [`start`](#start-18)  | Starts the server with a custom HTTP factory for non-WebSocket requests. The Symple server handles WebSocket upgrades internally; any other HTTP request (e.g. static files, REST API) is delegated to this factory. |
| `void` | [`stop`](#stop-14)  | Broadcasts a shutdown notice to all peers, closes the listen socket, and releases all internal state. Safe to call more than once. |
| `void` | [`broadcast`](#broadcast)  | Broadcast a message to all peers in a room (excluding sender). |
| `void` | [`broadcastRooms`](#broadcastrooms)  | Broadcast to multiple rooms with per-recipient dedup. |
| `bool` | [`sendTo`](#sendto)  | Send a message to a specific peer by session ID. |
| `bool` | [`sendToUser`](#sendtouser)  | Send a message to any peer with the given user name. |
| `ServerPeer *` | [`getPeer`](#getpeer) `nodiscard` | Get a connected peer by session ID. |
| `std::vector< ServerPeer * >` | [`getPeersInRoom`](#getpeersinroom) `nodiscard` | Get all peers in a room. |
| `size_t` | [`peerCount`](#peercount) `const` `nodiscard` | Number of connected, authenticated peers. |
| `void` | [`addVirtualPeer`](#addvirtualpeer)  | Register a virtual peer that receives messages via callback. |
| `void` | [`removeVirtualPeer`](#removevirtualpeer)  | Remove a virtual peer by session ID. |
| `http::Server &` | [`httpServer`](#httpserver) `inline` `nodiscard` | Access the underlying HTTP server (e.g. to serve static files). |
| `uv::Loop *` | [`loop`](#loop-9) `const` `inline` `nodiscard` | [Event](#event-2) loop that owns the Symple server and all peer connections. |

---

{#server-11}

#### Server

```cpp
Server(uv::Loop * loop = uv::defaultLoop())
```

Defined in src/symple/include/icy/symple/server.h:157

Constructs a server using the given event loop. 
#### Parameters
* `loop` libuv event loop; defaults to [uv::defaultLoop()](uv.md#defaultloop).

---

{#server-12}

#### Server

```cpp
Server(const Server &) = delete
```

Defined in src/symple/include/icy/symple/server.h:160

Deleted constructor.

---

{#start-17}

#### start

```cpp
void start(const Options & opts)
```

Defined in src/symple/include/icy/symple/server.h:166

Starts the server with the given options. Begins accepting WebSocket connections on opts.host:opts.port. 
#### Parameters
* `opts` [Server](#server-10) configuration options.

---

{#start-18}

#### start

```cpp
void start(const Options & opts, std::unique_ptr< http::ServerConnectionFactory > httpFactory)
```

Defined in src/symple/include/icy/symple/server.h:173

Starts the server with a custom HTTP factory for non-WebSocket requests. The Symple server handles WebSocket upgrades internally; any other HTTP request (e.g. static files, REST API) is delegated to this factory. 
#### Parameters
* `opts` [Server](#server-10) configuration options. 

* `httpFactory` Factory for HTTP responders; may be nullptr.

---

{#stop-14}

#### stop

```cpp
void stop()
```

Defined in src/symple/include/icy/symple/server.h:178

Broadcasts a shutdown notice to all peers, closes the listen socket, and releases all internal state. Safe to call more than once.

---

{#broadcast}

#### broadcast

```cpp
void broadcast(const std::string & room, const json::Value & msg, const std::string & excludeId = {})
```

Defined in src/symple/include/icy/symple/server.h:182

Broadcast a message to all peers in a room (excluding sender).

---

{#broadcastrooms}

#### broadcastRooms

```cpp
void broadcastRooms(const std::unordered_set< std::string > & rooms, const json::Value & msg, const std::string & excludeId = {})
```

Defined in src/symple/include/icy/symple/server.h:186

Broadcast to multiple rooms with per-recipient dedup.

---

{#sendto}

#### sendTo

```cpp
bool sendTo(const std::string & peerId, const json::Value & msg)
```

Defined in src/symple/include/icy/symple/server.h:191

Send a message to a specific peer by session ID.

---

{#sendtouser}

#### sendToUser

```cpp
bool sendToUser(const std::string & user, const json::Value & msg)
```

Defined in src/symple/include/icy/symple/server.h:194

Send a message to any peer with the given user name.

---

{#getpeer}

#### getPeer

`nodiscard`

```cpp
[[nodiscard]] ServerPeer * getPeer(const std::string & id)
```

Defined in src/symple/include/icy/symple/server.h:197

Get a connected peer by session ID.

---

{#getpeersinroom}

#### getPeersInRoom

`nodiscard`

```cpp
[[nodiscard]] std::vector< ServerPeer * > getPeersInRoom(const std::string & room)
```

Defined in src/symple/include/icy/symple/server.h:200

Get all peers in a room.

---

{#peercount}

#### peerCount

`const` `nodiscard`

```cpp
[[nodiscard]] size_t peerCount() const
```

Defined in src/symple/include/icy/symple/server.h:203

Number of connected, authenticated peers.

---

{#addvirtualpeer}

#### addVirtualPeer

```cpp
void addVirtualPeer(const Peer & peer, const std::vector< std::string > & rooms, std::function< void(const json::Value &)> handler)
```

Defined in src/symple/include/icy/symple/server.h:230

Register a virtual peer that receives messages via callback.

The virtual peer appears in presence broadcasts and is routable like any WebSocket-connected peer. Messages addressed to it are delivered via the callback instead of a WebSocket connection.

#### Parameters
* `peer` [Peer](#peer-1) data (must have "id", "user", "name" fields). 

* `rooms` Rooms the virtual peer joins. 

* `handler` Called when a message is routed to this peer.

---

{#removevirtualpeer}

#### removeVirtualPeer

```cpp
void removeVirtualPeer(const std::string & peerId)
```

Defined in src/symple/include/icy/symple/server.h:235

Remove a virtual peer by session ID.

---

{#httpserver}

#### httpServer

`inline` `nodiscard`

```cpp
[[nodiscard]] inline http::Server & httpServer()
```

Defined in src/symple/include/icy/symple/server.h:238

Access the underlying HTTP server (e.g. to serve static files).

---

{#loop-9}

#### loop

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline uv::Loop * loop() const
```

Defined in src/symple/include/icy/symple/server.h:241

[Event](#event-2) loop that owns the Symple server and all peer connections.

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `Options` | [`_opts`](#_opts)  |  |
| `uv::Loop *` | [`_loop`](#_loop-3)  |  |
| `std::unique_ptr< http::Server >` | [`_http`](#_http)  |  |
| `std::unique_ptr< PeerRegistry >` | [`_peerRegistry`](#_peerregistry)  |  |
| `std::unique_ptr< RoomIndex >` | [`_roomIndex`](#_roomindex)  |  |
| `std::mutex` | [`_mutex`](#_mutex-16)  |  |
| `std::atomic< bool >` | [`_shuttingDown`](#_shuttingdown)  |  |
| `std::unique_ptr< http::ServerConnectionFactory >` | [`_httpFallback`](#_httpfallback)  | Fallback factory for non-WebSocket HTTP requests. |

---

{#_opts}

#### _opts

```cpp
Options _opts
```

Defined in src/symple/include/icy/symple/server.h:267

---

{#_loop-3}

#### _loop

```cpp
uv::Loop * _loop = nullptr
```

Defined in src/symple/include/icy/symple/server.h:268

---

{#_http}

#### _http

```cpp
std::unique_ptr< http::Server > _http
```

Defined in src/symple/include/icy/symple/server.h:269

---

{#_peerregistry}

#### _peerRegistry

```cpp
std::unique_ptr< PeerRegistry > _peerRegistry
```

Defined in src/symple/include/icy/symple/server.h:270

---

{#_roomindex}

#### _roomIndex

```cpp
std::unique_ptr< RoomIndex > _roomIndex
```

Defined in src/symple/include/icy/symple/server.h:271

---

{#_mutex-16}

#### _mutex

```cpp
std::mutex _mutex
```

Defined in src/symple/include/icy/symple/server.h:273

---

{#_shuttingdown}

#### _shuttingDown

```cpp
std::atomic< bool > _shuttingDown {false}
```

Defined in src/symple/include/icy/symple/server.h:274

---

{#_httpfallback}

#### _httpFallback

```cpp
std::unique_ptr< http::ServerConnectionFactory > _httpFallback
```

Defined in src/symple/include/icy/symple/server.h:277

Fallback factory for non-WebSocket HTTP requests.

### Private Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`onAuth`](#onauth)  |  |
| `void` | [`onMessage`](#onmessage)  |  |
| `void` | [`onJoin`](#onjoin)  |  |
| `void` | [`onLeave`](#onleave)  |  |
| `void` | [`onDisconnect`](#ondisconnect)  |  |
| `void` | [`route`](#route)  |  |
| `bool` | [`deliver`](#deliver)  |  |
| `bool` | [`deliverSerialized`](#deliverserialized)  |  |
| `void` | [`sendPresenceSnapshot`](#sendpresencesnapshot)  |  |

---

{#onauth}

#### onAuth

```cpp
void onAuth(ServerPeer & peer, const json::Value & msg, std::unique_lock< std::mutex > & lock)
```

Defined in src/symple/include/icy/symple/server.h:252

---

{#onmessage}

#### onMessage

```cpp
void onMessage(ServerPeer & peer, json::Value msg)
```

Defined in src/symple/include/icy/symple/server.h:253

---

{#onjoin}

#### onJoin

```cpp
void onJoin(ServerPeer & peer, const std::string & room)
```

Defined in src/symple/include/icy/symple/server.h:254

---

{#onleave}

#### onLeave

```cpp
void onLeave(ServerPeer & peer, const std::string & room)
```

Defined in src/symple/include/icy/symple/server.h:255

---

{#ondisconnect}

#### onDisconnect

```cpp
void onDisconnect(ServerPeer & peer, std::unique_lock< std::mutex > & lock)
```

Defined in src/symple/include/icy/symple/server.h:256

---

{#route}

#### route

```cpp
void route(ServerPeer & sender, const json::Value & msg)
```

Defined in src/symple/include/icy/symple/server.h:257

---

{#deliver}

#### deliver

```cpp
bool deliver(const std::string & peerId, const json::Value & msg)
```

Defined in src/symple/include/icy/symple/server.h:258

---

{#deliverserialized}

#### deliverSerialized

```cpp
bool deliverSerialized(const std::string & peerId, const char * data, size_t len, const json::Value & msg)
```

Defined in src/symple/include/icy/symple/server.h:259

---

{#sendpresencesnapshot}

#### sendPresenceSnapshot

```cpp
void sendPresenceSnapshot(ServerPeer & recipient, const std::unordered_set< std::string > & rooms, std::string_view excludeId = {})
```

Defined in src/symple/include/icy/symple/server.h:263

{#peerregistry}

## PeerRegistry

```cpp
#include <src/symple/src/server/detail.h>
```

```cpp
class PeerRegistry
```

Defined in src/symple/src/server/detail.h:27

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`add`](#add-3) | `function` | Declared here |
| [`addVirtual`](#addvirtual) | `function` | Declared here |
| [`bind`](#bind-4) | `function` | Declared here |
| [`unbind`](#unbind) | `function` | Declared here |
| [`erase`](#erase) | `function` | Declared here |
| [`eraseVirtual`](#erasevirtual) | `function` | Declared here |
| [`clear`](#clear-5) | `function` | Declared here |
| [`find`](#find) | `function` | Declared here |
| [`find`](#find-1) | `function` | Declared here |
| [`findVirtual`](#findvirtual) | `function` | Declared here |
| [`findVirtual`](#findvirtual-1) | `function` | Declared here |
| [`findByConnection`](#findbyconnection) | `function` | Declared here |
| [`findByConnection`](#findbyconnection-1) | `function` | Declared here |
| [`size`](#size-13) | `function` | Declared here |
| [`peers`](#peers-1) | `function` | Declared here |
| [`_peers`](#_peers) | `variable` | Declared here |
| [`_virtualPeers`](#_virtualpeers) | `variable` | Declared here |
| [`_connToPeer`](#_conntopeer) | `variable` | Declared here |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`add`](#add-3)  |  |
| `void` | [`addVirtual`](#addvirtual)  |  |
| `void` | [`bind`](#bind-4)  |  |
| `void` | [`unbind`](#unbind)  |  |
| `void` | [`erase`](#erase)  |  |
| `void` | [`eraseVirtual`](#erasevirtual)  |  |
| `void` | [`clear`](#clear-5)  |  |
| `ServerPeer *` | [`find`](#find) `nodiscard` |  |
| `const ServerPeer *` | [`find`](#find-1) `const` `nodiscard` |  |
| `VirtualPeer *` | [`findVirtual`](#findvirtual) `nodiscard` |  |
| `const VirtualPeer *` | [`findVirtual`](#findvirtual-1) `const` `nodiscard` |  |
| `ServerPeer *` | [`findByConnection`](#findbyconnection) `nodiscard` |  |
| `const ServerPeer *` | [`findByConnection`](#findbyconnection-1) `const` `nodiscard` |  |
| `size_t` | [`size`](#size-13) `const` `nodiscard` |  |
| `const std::unordered_map< std::string, std::unique_ptr< ServerPeer > > &` | [`peers`](#peers-1) `const` `inline` `nodiscard` |  |

---

{#add-3}

#### add

```cpp
void add(std::string id, std::unique_ptr< ServerPeer > peer)
```

Defined in src/symple/src/server/detail.h:30

---

{#addvirtual}

#### addVirtual

```cpp
void addVirtual(std::string id, VirtualPeer peer)
```

Defined in src/symple/src/server/detail.h:31

---

{#bind-4}

#### bind

```cpp
void bind(http::ServerConnection & conn, const std::string & id)
```

Defined in src/symple/src/server/detail.h:32

---

{#unbind}

#### unbind

```cpp
void unbind(http::ServerConnection & conn)
```

Defined in src/symple/src/server/detail.h:33

---

{#erase}

#### erase

```cpp
void erase(const std::string & id)
```

Defined in src/symple/src/server/detail.h:34

---

{#erasevirtual}

#### eraseVirtual

```cpp
void eraseVirtual(const std::string & id)
```

Defined in src/symple/src/server/detail.h:35

---

{#clear-5}

#### clear

```cpp
void clear()
```

Defined in src/symple/src/server/detail.h:36

---

{#find}

#### find

`nodiscard`

```cpp
[[nodiscard]] ServerPeer * find(const std::string & id)
```

Defined in src/symple/src/server/detail.h:38

---

{#find-1}

#### find

`const` `nodiscard`

```cpp
[[nodiscard]] const ServerPeer * find(const std::string & id) const
```

Defined in src/symple/src/server/detail.h:39

---

{#findvirtual}

#### findVirtual

`nodiscard`

```cpp
[[nodiscard]] VirtualPeer * findVirtual(const std::string & id)
```

Defined in src/symple/src/server/detail.h:40

---

{#findvirtual-1}

#### findVirtual

`const` `nodiscard`

```cpp
[[nodiscard]] const VirtualPeer * findVirtual(const std::string & id) const
```

Defined in src/symple/src/server/detail.h:41

---

{#findbyconnection}

#### findByConnection

`nodiscard`

```cpp
[[nodiscard]] ServerPeer * findByConnection(http::ServerConnection & conn)
```

Defined in src/symple/src/server/detail.h:42

---

{#findbyconnection-1}

#### findByConnection

`const` `nodiscard`

```cpp
[[nodiscard]] const ServerPeer * findByConnection(http::ServerConnection & conn) const
```

Defined in src/symple/src/server/detail.h:43

---

{#size-13}

#### size

`const` `nodiscard`

```cpp
[[nodiscard]] size_t size() const
```

Defined in src/symple/src/server/detail.h:44

---

{#peers-1}

#### peers

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline const std::unordered_map< std::string, std::unique_ptr< ServerPeer > > & peers() const
```

Defined in src/symple/src/server/detail.h:46

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::unordered_map< std::string, std::unique_ptr< ServerPeer > >` | [`_peers`](#_peers)  |  |
| `std::unordered_map< std::string, VirtualPeer >` | [`_virtualPeers`](#_virtualpeers)  |  |
| `std::unordered_map< http::ServerConnection *, std::string >` | [`_connToPeer`](#_conntopeer)  |  |

---

{#_peers}

#### _peers

```cpp
std::unordered_map< std::string, std::unique_ptr< ServerPeer > > _peers
```

Defined in src/symple/src/server/detail.h:52

---

{#_virtualpeers}

#### _virtualPeers

```cpp
std::unordered_map< std::string, VirtualPeer > _virtualPeers
```

Defined in src/symple/src/server/detail.h:53

---

{#_conntopeer}

#### _connToPeer

```cpp
std::unordered_map< http::ServerConnection *, std::string > _connToPeer
```

Defined in src/symple/src/server/detail.h:54

{#roomindex}

## RoomIndex

```cpp
#include <src/symple/src/server/detail.h>
```

```cpp
class RoomIndex
```

Defined in src/symple/src/server/detail.h:58

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`join`](#join) | `function` | Declared here |
| [`leave`](#leave) | `function` | Declared here |
| [`leaveAll`](#leaveall) | `function` | Declared here |
| [`clear`](#clear-6) | `function` | Declared here |
| [`members`](#members) | `function` | Declared here |
| [`collectRecipients`](#collectrecipients) | `function` | Declared here |
| [`MemberSet`](#memberset) | `typedef` | Declared here |
| [`_rooms`](#_rooms) | `variable` | Declared here |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`join`](#join)  |  |
| `void` | [`leave`](#leave)  |  |
| `void` | [`leaveAll`](#leaveall)  |  |
| `void` | [`clear`](#clear-6)  |  |
| `const MemberSet *` | [`members`](#members) `const` `nodiscard` |  |
| `std::unordered_set< std::string >` | [`collectRecipients`](#collectrecipients) `const` `nodiscard` |  |

---

{#join}

#### join

```cpp
void join(const std::string & room, const std::string & peerId)
```

Defined in src/symple/src/server/detail.h:63

---

{#leave}

#### leave

```cpp
void leave(const std::string & room, const std::string & peerId)
```

Defined in src/symple/src/server/detail.h:64

---

{#leaveall}

#### leaveAll

```cpp
void leaveAll(const std::string & peerId)
```

Defined in src/symple/src/server/detail.h:65

---

{#clear-6}

#### clear

```cpp
void clear()
```

Defined in src/symple/src/server/detail.h:66

---

{#members}

#### members

`const` `nodiscard`

```cpp
[[nodiscard]] const MemberSet * members(const std::string & room) const
```

Defined in src/symple/src/server/detail.h:68

---

{#collectrecipients}

#### collectRecipients

`const` `nodiscard`

```cpp
[[nodiscard]] std::unordered_set< std::string > collectRecipients(const std::unordered_set< std::string > & rooms, std::string_view excludeId = {}) const
```

Defined in src/symple/src/server/detail.h:69

### Public Types

| Name | Description |
|------|-------------|
| [`MemberSet`](#memberset)  |  |

---

{#memberset}

#### MemberSet

```cpp
using MemberSet = std::unordered_set< std::string >
```

Defined in src/symple/src/server/detail.h:61

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::unordered_map< std::string, MemberSet >` | [`_rooms`](#_rooms)  |  |

---

{#_rooms}

#### _rooms

```cpp
std::unordered_map< std::string, MemberSet > _rooms
```

Defined in src/symple/src/server/detail.h:74

{#options-16}

## Options

```cpp
#include <icy/symple/server.h>
```

```cpp
struct Options
```

Defined in src/symple/include/icy/symple/server.h:140

[Configuration](base.md#configuration) options for the Symple server.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`host`](#host-4) | `variable` | Declared here |
| [`port`](#port-3) | `variable` | Declared here |
| [`socket`](#socket-7) | `variable` | Declared here |
| [`authentication`](#authentication) | `variable` | Declared here |
| [`dynamicRooms`](#dynamicrooms) | `variable` | Declared here |
| [`maxConnections`](#maxconnections) | `variable` | Declared here |
| [`maxMessageSize`](#maxmessagesize) | `variable` | Declared here |
| [`rateLimit`](#ratelimit) | `variable` | Declared here |
| [`rateSeconds`](#rateseconds) | `variable` | Declared here |

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`host`](#host-4)  |  |
| `uint16_t` | [`port`](#port-3)  |  |
| `net::TCPSocket::Ptr` | [`socket`](#socket-7)  | Optional pre-created listen socket (e.g. SSLSocket for HTTPS/WSS). |
| `bool` | [`authentication`](#authentication)  | Require token in auth message. |
| `bool` | [`dynamicRooms`](#dynamicrooms)  | Allow clients to join/leave rooms. |
| `size_t` | [`maxConnections`](#maxconnections)  | Max WebSocket connections (0 = unlimited). |
| `size_t` | [`maxMessageSize`](#maxmessagesize)  | Max message payload in bytes (64KB default). |
| `double` | [`rateLimit`](#ratelimit)  | Messages per rate window. |
| `double` | [`rateSeconds`](#rateseconds)  | Rate window in seconds. |

---

{#host-4}

#### host

```cpp
std::string host = "0.0.0.0"
```

Defined in src/symple/include/icy/symple/server.h:142

---

{#port-3}

#### port

```cpp
uint16_t port = 4500
```

Defined in src/symple/include/icy/symple/server.h:143

---

{#socket-7}

#### socket

```cpp
net::TCPSocket::Ptr socket
```

Defined in src/symple/include/icy/symple/server.h:144

Optional pre-created listen socket (e.g. SSLSocket for HTTPS/WSS).

---

{#authentication}

#### authentication

```cpp
bool authentication = false
```

Defined in src/symple/include/icy/symple/server.h:145

Require token in auth message.

---

{#dynamicrooms}

#### dynamicRooms

```cpp
bool dynamicRooms = true
```

Defined in src/symple/include/icy/symple/server.h:146

Allow clients to join/leave rooms.

---

{#maxconnections}

#### maxConnections

```cpp
size_t maxConnections = 0
```

Defined in src/symple/include/icy/symple/server.h:149

Max WebSocket connections (0 = unlimited).

---

{#maxmessagesize}

#### maxMessageSize

```cpp
size_t maxMessageSize = 64 * 1024
```

Defined in src/symple/include/icy/symple/server.h:150

Max message payload in bytes (64KB default).

---

{#ratelimit}

#### rateLimit

```cpp
double rateLimit = 100.0
```

Defined in src/symple/include/icy/symple/server.h:151

Messages per rate window.

---

{#rateseconds}

#### rateSeconds

```cpp
double rateSeconds = 10.0
```

Defined in src/symple/include/icy/symple/server.h:152

Rate window in seconds.

{#virtualpeer}

## VirtualPeer

```cpp
#include <src/symple/src/server/detail.h>
```

```cpp
struct VirtualPeer
```

Defined in src/symple/src/server/detail.h:19

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`peer`](#peer-5) | `variable` | Declared here |
| [`rooms`](#rooms-1) | `variable` | Declared here |
| [`handler`](#handler) | `variable` | Declared here |

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `Peer` | [`peer`](#peer-5)  |  |
| `std::unordered_set< std::string >` | [`rooms`](#rooms-1)  |  |
| `std::function< void(const json::Value &)>` | [`handler`](#handler)  |  |

---

{#peer-5}

#### peer

```cpp
Peer peer
```

Defined in src/symple/src/server/detail.h:21

---

{#rooms-1}

#### rooms

```cpp
std::unordered_set< std::string > rooms
```

Defined in src/symple/src/server/detail.h:22

---

{#handler}

#### handler

```cpp
std::function< void(const json::Value &)> handler
```

Defined in src/symple/src/server/detail.h:23

{#routingpolicy}

## RoutingPolicy

```cpp
#include <src/symple/src/server/detail.h>
```

```cpp
struct RoutingPolicy
```

Defined in src/symple/src/server/detail.h:78

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`sharesAnyRoom`](#sharesanyroom) | `function` | Declared here |
| [`canDirectMessage`](#candirectmessage) | `function` | Declared here |
| [`canDirectMessage`](#candirectmessage-1) | `function` | Declared here |
| [`canBroadcastToRoom`](#canbroadcasttoroom) | `function` | Declared here |

### Public Static Methods

| Return | Name | Description |
|--------|------|-------------|
| `bool` | [`sharesAnyRoom`](#sharesanyroom) `static` `nodiscard` |  |
| `bool` | [`canDirectMessage`](#candirectmessage) `static` `nodiscard` |  |
| `bool` | [`canDirectMessage`](#candirectmessage-1) `static` `nodiscard` |  |
| `bool` | [`canBroadcastToRoom`](#canbroadcasttoroom) `static` `nodiscard` |  |

---

{#sharesanyroom}

#### sharesAnyRoom

`static` `nodiscard`

```cpp
[[nodiscard]] static bool sharesAnyRoom(const std::unordered_set< std::string > & a, const std::unordered_set< std::string > & b)
```

Defined in src/symple/src/server/detail.h:80

---

{#candirectmessage}

#### canDirectMessage

`static` `nodiscard`

```cpp
[[nodiscard]] static bool canDirectMessage(const ServerPeer & sender, const ServerPeer & recipient)
```

Defined in src/symple/src/server/detail.h:82

---

{#candirectmessage-1}

#### canDirectMessage

`static` `nodiscard`

```cpp
[[nodiscard]] static bool canDirectMessage(const ServerPeer & sender, const VirtualPeer & recipient)
```

Defined in src/symple/src/server/detail.h:84

---

{#canbroadcasttoroom}

#### canBroadcastToRoom

`static` `nodiscard`

```cpp
[[nodiscard]] static bool canBroadcastToRoom(const ServerPeer & sender, const std::string & room)
```

Defined in src/symple/src/server/detail.h:86

{#serverpeer}

## ServerPeer

```cpp
#include <icy/symple/server.h>
```

```cpp
class ServerPeer
```

Defined in src/symple/include/icy/symple/server.h:43

Per-connection state for a connected Symple peer.

Created by the server after successful authentication. Holds the peer data, room memberships, and a reference to the underlying WebSocket connection for sending.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`ServerPeer`](#serverpeer-1) | `function` | Declared here |
| [`send`](#send-19) | `function` | Declared here |
| [`sendSerialized`](#sendserialized) | `function` | Declared here |
| [`join`](#join-1) | `function` | Declared here |
| [`leave`](#leave-1) | `function` | Declared here |
| [`leaveAll`](#leaveall-1) | `function` | Declared here |
| [`peer`](#peer-6) | `function` | Declared here |
| [`peer`](#peer-7) | `function` | Declared here |
| [`id`](#id-9) | `function` | Declared here |
| [`rooms`](#rooms-2) | `function` | Declared here |
| [`authenticated`](#authenticated) | `function` | Declared here |
| [`setAuthenticated`](#setauthenticated) | `function` | Declared here |
| [`setPeer`](#setpeer) | `function` | Declared here |
| [`connection`](#connection-10) | `function` | Declared here |
| [`checkRate`](#checkrate) | `function` | Declared here |
| [`setRateLimit`](#setratelimit) | `function` | Declared here |
| [`_conn`](#_conn) | `variable` | Declared here |
| [`_peer`](#_peer-1) | `variable` | Declared here |
| [`_rooms`](#_rooms-1) | `variable` | Declared here |
| [`_rateLimiter`](#_ratelimiter) | `variable` | Declared here |
| [`_authenticated`](#_authenticated) | `variable` | Declared here |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`ServerPeer`](#serverpeer-1)  | Constructs a peer bound to the given server-side connection. |
| `void` | [`send`](#send-19)  | Serialises and sends a JSON message over the WebSocket connection. Logs a warning if the send fails; does not throw. |
| `void` | [`sendSerialized`](#sendserialized)  | Sends a pre-serialized JSON payload over the WebSocket connection. Use this on fanout paths that already serialized once. |
| `void` | [`join`](#join-1)  | Adds this peer to the named room (local tracking only). |
| `void` | [`leave`](#leave-1)  | Removes this peer from the named room (local tracking only). |
| `void` | [`leaveAll`](#leaveall-1)  | Removes this peer from all rooms (local tracking only). |
| `Peer &` | [`peer`](#peer-6) `inline` `nodiscard` | Returns a mutable reference to the peer data object. |
| `const Peer &` | [`peer`](#peer-7) `const` `inline` `nodiscard` | Returns a const reference to the peer data object. |
| `std::string` | [`id`](#id-9) `const` `nodiscard` | Returns the session ID assigned to this peer. |
| `const std::unordered_set< std::string > &` | [`rooms`](#rooms-2) `const` `inline` `nodiscard` | Returns the set of room names this peer is currently joined to. |
| `bool` | [`authenticated`](#authenticated) `const` `inline` `nodiscard` | Returns true if the peer has completed authentication. |
| `void` | [`setAuthenticated`](#setauthenticated) `inline` | Marks the peer as authenticated or unauthenticated. |
| `void` | [`setPeer`](#setpeer) `inline` | Replaces the peer's data object. |
| `http::ServerConnection &` | [`connection`](#connection-10) `inline` | Returns a reference to the underlying server connection. |
| `bool` | [`checkRate`](#checkrate) `inline` `nodiscard` | Per-peer rate limiter. Returns false if message should be dropped. |
| `void` | [`setRateLimit`](#setratelimit) `inline` | Configures the per-peer rate limit. |

---

{#serverpeer-1}

#### ServerPeer

```cpp
ServerPeer(http::ServerConnection & conn)
```

Defined in src/symple/include/icy/symple/server.h:48

Constructs a peer bound to the given server-side connection. 
#### Parameters
* `conn` The underlying WebSocket server connection.

---

{#send-19}

#### send

```cpp
void send(const json::Value & msg)
```

Defined in src/symple/include/icy/symple/server.h:53

Serialises and sends a JSON message over the WebSocket connection. Logs a warning if the send fails; does not throw. 
#### Parameters
* `msg` JSON value to send.

---

{#sendserialized}

#### sendSerialized

```cpp
void sendSerialized(const char * data, size_t len)
```

Defined in src/symple/include/icy/symple/server.h:57

Sends a pre-serialized JSON payload over the WebSocket connection. Use this on fanout paths that already serialized once.

---

{#join-1}

#### join

```cpp
void join(const std::string & room)
```

Defined in src/symple/include/icy/symple/server.h:61

Adds this peer to the named room (local tracking only). 
#### Parameters
* `room` Room name to join.

---

{#leave-1}

#### leave

```cpp
void leave(const std::string & room)
```

Defined in src/symple/include/icy/symple/server.h:65

Removes this peer from the named room (local tracking only). 
#### Parameters
* `room` Room name to leave.

---

{#leaveall-1}

#### leaveAll

```cpp
void leaveAll()
```

Defined in src/symple/include/icy/symple/server.h:68

Removes this peer from all rooms (local tracking only).

---

{#peer-6}

#### peer

`inline` `nodiscard`

```cpp
[[nodiscard]] inline Peer & peer()
```

Defined in src/symple/include/icy/symple/server.h:71

Returns a mutable reference to the peer data object.

---

{#peer-7}

#### peer

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline const Peer & peer() const
```

Defined in src/symple/include/icy/symple/server.h:74

Returns a const reference to the peer data object.

---

{#id-9}

#### id

`const` `nodiscard`

```cpp
[[nodiscard]] std::string id() const
```

Defined in src/symple/include/icy/symple/server.h:77

Returns the session ID assigned to this peer.

---

{#rooms-2}

#### rooms

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline const std::unordered_set< std::string > & rooms() const
```

Defined in src/symple/include/icy/symple/server.h:80

Returns the set of room names this peer is currently joined to.

---

{#authenticated}

#### authenticated

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline bool authenticated() const
```

Defined in src/symple/include/icy/symple/server.h:83

Returns true if the peer has completed authentication.

---

{#setauthenticated}

#### setAuthenticated

`inline`

```cpp
inline void setAuthenticated(bool v)
```

Defined in src/symple/include/icy/symple/server.h:87

Marks the peer as authenticated or unauthenticated. 
#### Parameters
* `v` True to mark as authenticated.

---

{#setpeer}

#### setPeer

`inline`

```cpp
inline void setPeer(const Peer & p)
```

Defined in src/symple/include/icy/symple/server.h:91

Replaces the peer's data object. 
#### Parameters
* `p` New peer data.

---

{#connection-10}

#### connection

`inline`

```cpp
inline http::ServerConnection & connection()
```

Defined in src/symple/include/icy/symple/server.h:94

Returns a reference to the underlying server connection.

---

{#checkrate}

#### checkRate

`inline` `nodiscard`

```cpp
[[nodiscard]] inline bool checkRate()
```

Defined in src/symple/include/icy/symple/server.h:97

Per-peer rate limiter. Returns false if message should be dropped.

---

{#setratelimit}

#### setRateLimit

`inline`

```cpp
inline void setRateLimit(double rate, double seconds)
```

Defined in src/symple/include/icy/symple/server.h:102

Configures the per-peer rate limit. 
#### Parameters
* `rate` Maximum messages allowed per window. 

* `seconds` Duration of the rate window in seconds.

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `http::ServerConnection &` | [`_conn`](#_conn)  |  |
| `Peer` | [`_peer`](#_peer-1)  |  |
| `std::unordered_set< std::string >` | [`_rooms`](#_rooms-1)  |  |
| `RateLimiter` | [`_rateLimiter`](#_ratelimiter)  | 100 messages per 10 seconds default |
| `bool` | [`_authenticated`](#_authenticated)  |  |

---

{#_conn}

#### _conn

```cpp
http::ServerConnection & _conn
```

Defined in src/symple/include/icy/symple/server.h:109

---

{#_peer-1}

#### _peer

```cpp
Peer _peer
```

Defined in src/symple/include/icy/symple/server.h:110

---

{#_rooms-1}

#### _rooms

```cpp
std::unordered_set< std::string > _rooms
```

Defined in src/symple/include/icy/symple/server.h:111

---

{#_ratelimiter}

#### _rateLimiter

```cpp
RateLimiter _rateLimiter {100.0, 10.0}
```

Defined in src/symple/include/icy/symple/server.h:112

100 messages per 10 seconds default

---

{#_authenticated}

#### _authenticated

```cpp
bool _authenticated = false
```

Defined in src/symple/include/icy/symple/server.h:113

{#address-13}

## Address

```cpp
#include <icy/symple/address.h>
```

```cpp
struct Address
```

Defined in src/symple/include/icy/symple/address.h:28

The [Address](#address-13) structure is an endpoint identifier for a peer on the network. The format is like so: user|id

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`operator<<`](#operator-34) | `friend` | Declared here |
| [`user`](#user) | `variable` | Declared here |
| [`id`](#id-5) | `variable` | Declared here |
| [`Address`](#address-14) | `function` | Declared here |
| [`Address`](#address-15) | `function` | Declared here |
| [`Address`](#address-16) | `function` | Declared here |
| [`parse`](#parse-2) | `function` | Declared here |
| [`valid`](#valid-10) | `function` | Declared here |
| [`print`](#print-15) | `function` | Declared here |
| [`toString`](#tostring-9) | `function` | Declared here |
| [`operator==`](#operator-35) | `function` | Declared here |
| [`operator==`](#operator-36) | `function` | Declared here |

### Friends

| Name | Description |
|------|-------------|
| [`operator<<`](#operator-34) `inline` |  |

---

{#operator-34}

#### operator<<

`inline`

```cpp
friend inline std::ostream & operator<<(std::ostream & os, const Address & addr)
```

Defined in src/symple/include/icy/symple/address.h:71

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`user`](#user)  |  |
| `std::string` | [`id`](#id-5)  |  |

---

{#user}

#### user

```cpp
std::string user
```

Defined in src/symple/include/icy/symple/address.h:77

---

{#id-5}

#### id

```cpp
std::string id
```

Defined in src/symple/include/icy/symple/address.h:79

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Address`](#address-14)  | Constructs an empty (invalid) address. |
|  | [`Address`](#address-15)  | Parses an address string of the form `user\|id`. |
|  | [`Address`](#address-16)  | Constructs an address from explicit user and session ID components. |
| `bool` | [`parse`](#parse-2)  | Parses an address string of the form `user\|id`. Populates the `[user](#user)` and `[id](#id-5)` fields. |
| `bool` | [`valid`](#valid-10) `const` `nodiscard` | Returns true if at least one of `[user](#user)` or `[id](#id-5)` is non-empty. |
| `void` | [`print`](#print-15) `const` | Writes the address in `user\|id` format to the given stream. |
| `std::string` | [`toString`](#tostring-9) `const` `nodiscard` | Returns the address as a string in `user\|id` format. |
| `bool` | [`operator==`](#operator-35) `const` | Compares two addresses for equality (both user and id must match). |
| `bool` | [`operator==`](#operator-36) `const` | Compares this address against a string in `user\|id` format without allocating. |

---

{#address-14}

#### Address

```cpp
Address()
```

Defined in src/symple/include/icy/symple/address.h:32

Constructs an empty (invalid) address.

---

{#address-15}

#### Address

```cpp
Address(std::string_view addr)
```

Defined in src/symple/include/icy/symple/address.h:36

Parses an address string of the form `user|id`. 
#### Parameters
* `addr` [Address](#address-13) string to parse.

---

{#address-16}

#### Address

```cpp
Address(const std::string & user, const std::string & id)
```

Defined in src/symple/include/icy/symple/address.h:41

Constructs an address from explicit user and session ID components. 
#### Parameters
* `user` User identifier. 

* `id` Session ID.

---

{#parse-2}

#### parse

```cpp
bool parse(std::string_view addr)
```

Defined in src/symple/include/icy/symple/address.h:50

Parses an address string of the form `user|id`. Populates the `[user](#user)` and `[id](#id-5)` fields. 
#### Parameters
* `addr` [Address](#address-13) string to parse. 

#### Returns
True if the result is a valid address.

---

{#valid-10}

#### valid

`const` `nodiscard`

```cpp
[[nodiscard]] bool valid() const
```

Defined in src/symple/include/icy/symple/address.h:53

Returns true if at least one of `[user](#user)` or `[id](#id-5)` is non-empty.

---

{#print-15}

#### print

`const`

```cpp
void print(std::ostream & os) const
```

Defined in src/symple/include/icy/symple/address.h:57

Writes the address in `user|id` format to the given stream. 
#### Parameters
* `os` Output stream.

---

{#tostring-9}

#### toString

`const` `nodiscard`

```cpp
[[nodiscard]] std::string toString() const
```

Defined in src/symple/include/icy/symple/address.h:61

Returns the address as a string in `user|id` format. 
#### Returns
Serialised address string.

---

{#operator-35}

#### operator==

`const`

```cpp
bool operator==(const Address & r) const
```

Defined in src/symple/include/icy/symple/address.h:65

Compares two addresses for equality (both user and id must match). 
#### Parameters
* `r` [Address](#address-13) to compare against.

---

{#operator-36}

#### operator==

`const`

```cpp
bool operator==(const std::string & r) const
```

Defined in src/symple/include/icy/symple/address.h:69

Compares this address against a string in `user|id` format without allocating. 
#### Parameters
* `r` String to compare against.

{#clientstate-1}

## ClientState

```cpp
#include <icy/symple/client.h>
```

```cpp
struct ClientState
```

Defined in src/symple/include/icy/symple/client.h:38

> **Inherits:** [`State`](base.md#state)

[Client](#client-5) connection states.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`str`](#str-3) | `function` | Declared here |
| [`Type`](#type-20) | `enum` | Declared here |
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
| `std::string` | [`str`](#str-3) `const` `inline` `override` |  |

---

{#str-3}

#### str

`const` `inline` `override`

```cpp
inline std::string str(unsigned int id) const override
```

Defined in src/symple/include/icy/symple/client.h:49

### Public Types

| Name | Description |
|------|-------------|
| [`Type`](#type-20)  |  |

---

{#type-20}

#### Type

```cpp
enum Type
```

Defined in src/symple/include/icy/symple/client.h:40

| Value | Description |
|-------|-------------|
| `Closed` |  |
| `Connecting` |  |
| `Authenticating` |  |
| `Online` |  |
| `Error` |  |

