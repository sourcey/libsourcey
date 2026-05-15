{#turnmodulerfc5766}

# turn

The `[TURN module (rfc5766)](#turnmodulerfc5766)` module contains TURN (rfc5766) client and server implementations.

### Namespaces

| Name | Description |
|------|-------------|
| [`turn`](#turn) | TURN allocation identity types and transport helpers. |

{#turn}

# turn

TURN allocation identity types and transport helpers.

### Classes

| Name | Description |
|------|-------------|
| [`Client`](#client-2) | TURN client that manages relay allocations, permissions, and data relay via RFC 5766. |
| [`FiveTuple`](#fivetuple) | TURN allocation identity tuple: client remote address, server local address, and transport protocol. |
| [`IAllocation`](#iallocation) | Base class for TURN relay allocations shared by client and server flows. |
| [`PermissionPolicy`](#permissionpolicy) | Lightweight server-side TURN permission policy. |
| [`Request`](#request-12) | A STUN message annotated with the transport type and source/destination addresses needed for server-side routing and response generation. |
| [`Server`](#server-6) | TURN server RFC 5766 / RFC 6062 implementation. Listens on UDP and/or TCP, authenticates requests via [ServerObserver](#serverobserver), and manages [ServerAllocation](#serverallocation) objects for each 5-tuple. |
| [`ServerAllocation`](#serverallocation) | Server-side TURN allocation that tracks permissions, lifetime, and relay state. Handles Refresh and CreatePermission requests common to both UDP and TCP allocations. Subclassed by [UDPAllocation](#udpallocation) and [TCPAllocation](#tcpallocation) for transport-specific behaviour. |
| [`TCPAllocation`](#tcpallocation) | Server-side TCP TURN allocation with connection pair management per RFC 6062. Maintains a control connection for STUN signalling and an acceptor socket for incoming peer TCP connections. Each peer connection is tracked as a [TCPConnectionPair](#tcpconnectionpair) and bridged to the client after a successful ConnectionBind. |
| [`TCPClient`](#tcpclient) | TCP transport TURN client with relay connection management per RFC 6062. Uses a single TCP control connection for STUN signalling and opens additional TCP relay connections for each peer via the Connect / ConnectionBind flow. |
| [`TCPConnectionPair`](#tcpconnectionpair) | Paired TCP connections forming a single TURN relay pipe between a client and a peer. Lifetime managed by [IntrusivePtr](base.md#intrusiveptr); the owning [TCPAllocation](#tcpallocation) holds the [Ptr](#ptr-15) in its pair map. |
| [`UDPAllocation`](#udpallocation) | Server-side UDP TURN allocation that relays datagrams to permitted peers. Binds a dedicated UDP relay socket for peer communication and forwards incoming datagrams to the client as Data Indications. |
| [`UDPClient`](#udpclient) | UDP transport TURN client for datagram-based relay allocations. Uses a single UDP socket for both STUN signalling and Send/Data Indications. All [Client](#client-2) methods apply; data is relayed via [sendData()](#senddata) using Send Indications. |
| [`ClientObserver`](#clientobserver) | Abstract observer interface for TURN client events. Callers subclass this to receive allocation lifecycle and data relay callbacks. |
| [`ClientState`](#clientstate) | [State](base.md#state) machine states for the TURN client. |
| [`Permission`](#permission) | A single TURN permission entry associating a peer IP with a 5-minute expiry timer. Per RFC 5766 section 8, permissions last exactly 300 seconds and must be refreshed via a new CreatePermission request before they expire. |
| [`RelayConnectionBinding`](#relayconnectionbinding) | Binding metadata stored as socket opaque data during a ConnectionBind handshake. Associates a relay socket with its TURN connection ID and peer address. |
| [`ServerObserver`](#serverobserver) | Observer interface that the application must implement to participate in server-side allocation management and authentication. |
| [`ServerOptions`](#serveroptions) | [Configuration](base.md#configuration) options for the TURN server. |
| [`TCPClientObserver`](#tcpclientobserver) | Observer interface for TCP TURN client events (RFC 6062). Extends [ClientObserver](#clientobserver) with callbacks specific to TCP relay connections. |

### Enumerations

| Name | Description |
|------|-------------|
| [`AuthenticationState`](#authenticationstate)  | Result returned by [ServerObserver::authenticateRequest()](#authenticaterequest-1) to control how the server proceeds with an incoming STUN request. |

---

{#authenticationstate}

#### AuthenticationState

```cpp
enum AuthenticationState
```

Result returned by [ServerObserver::authenticateRequest()](#authenticaterequest-1) to control how the server proceeds with an incoming STUN request.

| Value | Description |
|-------|-------------|
| `Authenticating` | Authentication is in progress; server holds the request. |
| `Authorized` | Credentials verified; request may proceed. |
| `QuotaReached` | User quota exceeded; respond with 486. |
| `NotAuthorized` | Credentials rejected; respond with 401. |

### Typedefs

| Return | Name | Description |
|--------|------|-------------|
| `KVCollection< net::Address, net::SocketEmitter >` | [`ConnectionManager`](#connectionmanager)  | Relay connection registry keyed by peer address for RFC 6062 TCP relays. |
| `ConnectionManager::Map` | [`ConnectionManagerMap`](#connectionmanagermap)  | Underlying map type used by the TCP relay connection registry. |
| `std::vector< Permission >` | [`PermissionList`](#permissionlist)  | Active TURN permissions associated with one allocation. |
| `std::map< FiveTuple, std::unique_ptr< ServerAllocation > >` | [`ServerAllocationMap`](#serverallocationmap)  | Active server allocations keyed by the client/server 5-tuple. |
| `std::map< uint32_t, TCPConnectionPair::Ptr >` | [`TCPConnectionPairMap`](#tcpconnectionpairmap)  | Accepted peer TCP connections keyed by TURN CONNECTION-ID. |
| `std::vector< std::string >` | [`IPList`](#iplist)  | List of peer IP strings used when creating or refreshing permissions. |

---

{#connectionmanager}

#### ConnectionManager

```cpp
using ConnectionManager = KVCollection< net::Address, net::SocketEmitter >
```

Relay connection registry keyed by peer address for RFC 6062 TCP relays.

---

{#connectionmanagermap}

#### ConnectionManagerMap

```cpp
using ConnectionManagerMap = ConnectionManager::Map
```

Underlying map type used by the TCP relay connection registry.

---

{#permissionlist}

#### PermissionList

```cpp
using PermissionList = std::vector< Permission >
```

Active TURN permissions associated with one allocation.

---

{#serverallocationmap}

#### ServerAllocationMap

```cpp
using ServerAllocationMap = std::map< FiveTuple, std::unique_ptr< ServerAllocation > >
```

Active server allocations keyed by the client/server 5-tuple.

---

{#tcpconnectionpairmap}

#### TCPConnectionPairMap

```cpp
using TCPConnectionPairMap = std::map< uint32_t, TCPConnectionPair::Ptr >
```

Accepted peer TCP connections keyed by TURN CONNECTION-ID.

---

{#iplist}

#### IPList

```cpp
using IPList = std::vector< std::string >
```

List of peer IP strings used when creating or refreshing permissions.

### Variables

| Return | Name | Description |
|--------|------|-------------|
| `std::int64_t` | [`kClientTimerInterval`](#kclienttimerinterval) `static` `constexpr` | [Timer](base.md#timer) interval for client maintenance (30 seconds). |
| `std::int64_t` | [`kClientDefaultLifetime`](#kclientdefaultlifetime) `static` `constexpr` | Default client allocation lifetime (5 minutes, in milliseconds). |
| `long` | [`kClientDefaultTimeout`](#kclientdefaulttimeout) `static` `constexpr` | Default client transaction timeout (10 seconds). |
| `std::int64_t` | [`kDefaultAllocationLifetime`](#kdefaultallocationlifetime) `static` `constexpr` | Default allocation lifetime: 10 minutes (in milliseconds). |
| `int` | [`PERMISSION_LIFETIME`](#permission_lifetime) `static` `constexpr` | [Permission](#permission) Lifetime MUST be 300 seconds (= 5 minutes). |
| `uint32_t` | [`kServerDefaultLifetime`](#kserverdefaultlifetime) `static` `constexpr` | Default server allocation lifetime (2 minutes, in milliseconds). |
| `uint32_t` | [`kServerMaxLifetime`](#kservermaxlifetime) `static` `constexpr` | Maximum server allocation lifetime (15 minutes, in milliseconds). |
| `int` | [`kServerMaxPermissions`](#kservermaxpermissions) `static` `constexpr` | Maximum number of permissions per allocation. |
| `int` | [`kServerTimerInterval`](#kservertimerinterval) `static` `constexpr` | [Server](#server-6) timer interval (10 seconds). |
| `int` | [`kServerEarlyMediaBufferSize`](#kserverearlymediabuffersize) `static` `constexpr` | Early media buffer size. |
| `int` | [`kConnectionBindTimeout`](#kconnectionbindtimeout) `static` `constexpr` | ConnectionBind request timeout (30 seconds, in milliseconds). |
| `int` | [`CLIENT_SOCK_BUF_SIZE`](#client_sock_buf_size) `static` `constexpr` | Default socket buffer size for TURN client sockets. |
| `int` | [`SERVER_SOCK_BUF_SIZE`](#server_sock_buf_size) `static` `constexpr` | Default socket buffer size for TURN server sockets. |
| `uint8_t` | [`kProtocolUDP`](#kprotocoludp) `static` `constexpr` | TURN protocol numbers. |
| `uint8_t` | [`kProtocolTCP`](#kprotocoltcp) `static` `constexpr` | IP protocol number for TCP. |
| `int` | [`kErrorTryAlternate`](#kerrortryalternate) `static` `constexpr` | TURN/STUN error codes used in this module. |
| `int` | [`kErrorBadRequest`](#kerrorbadrequest) `static` `constexpr` | [Request](#request-12) was malformed or missing required attributes. |
| `int` | [`kErrorNotAuthorized`](#kerrornotauthorized) `static` `constexpr` | Authentication failed or credentials were not accepted. |
| `int` | [`kErrorForbidden`](#kerrorforbidden) `static` `constexpr` | [Request](#request-12) was understood but is not permitted for this client. |
| `int` | [`kErrorUnknownAttribute`](#kerrorunknownattribute) `static` `constexpr` | [Request](#request-12) included an unsupported comprehension-required attribute. |
| `int` | [`kErrorUnsupportedTransport`](#kerrorunsupportedtransport) `static` `constexpr` | Requested transport protocol is not supported by the server. |
| `int` | [`kErrorAllocationMismatch`](#kerrorallocationmismatch) `static` `constexpr` | [Client](#client-2) attempted an allocation operation that conflicts with existing state. |
| `int` | [`kErrorStaleNonce`](#kerrorstalenonce) `static` `constexpr` | Authentication nonce expired and must be refreshed. |
| `int` | [`kErrorWrongCredentials`](#kerrorwrongcredentials) `static` `constexpr` | Supplied credentials were well formed but did not match the allocation. |
| `int` | [`kErrorConnectionAlreadyExists`](#kerrorconnectionalreadyexists) `static` `constexpr` | TURN TCP connection binding already exists. |
| `int` | [`kErrorConnectionTimeoutOrFailure`](#kerrorconnectiontimeoutorfailure) `static` `constexpr` | TURN TCP peer connection attempt timed out or failed. |
| `int` | [`kErrorAllocationQuotaReached`](#kerrorallocationquotareached) `static` `constexpr` | User or server allocation quota was exceeded. |
| `int` | [`kErrorInsufficientCapacity`](#kerrorinsufficientcapacity) `static` `constexpr` | [Server](#server-6) lacks capacity to satisfy the request. |
| `int` | [`kErrorOperationNotSupported`](#kerroroperationnotsupported) `static` `constexpr` | [Request](#request-12) is recognized but not implemented by this server. |

---

{#kclienttimerinterval}

#### kClientTimerInterval

`static` `constexpr`

```cpp
std::int64_t kClientTimerInterval = 30 * 1000
```

[Timer](base.md#timer) interval for client maintenance (30 seconds).

---

{#kclientdefaultlifetime}

#### kClientDefaultLifetime

`static` `constexpr`

```cpp
std::int64_t kClientDefaultLifetime = 5 * 60 * 1000
```

Default client allocation lifetime (5 minutes, in milliseconds).

---

{#kclientdefaulttimeout}

#### kClientDefaultTimeout

`static` `constexpr`

```cpp
long kClientDefaultTimeout = 10 * 1000
```

Default client transaction timeout (10 seconds).

---

{#kdefaultallocationlifetime}

#### kDefaultAllocationLifetime

`static` `constexpr`

```cpp
std::int64_t kDefaultAllocationLifetime = 10 * 60 * 1000
```

Default allocation lifetime: 10 minutes (in milliseconds).

---

{#permission_lifetime}

#### PERMISSION_LIFETIME

`static` `constexpr`

```cpp
int PERMISSION_LIFETIME = 5 * 60 * 1000
```

[Permission](#permission) Lifetime MUST be 300 seconds (= 5 minutes).

---

{#kserverdefaultlifetime}

#### kServerDefaultLifetime

`static` `constexpr`

```cpp
uint32_t kServerDefaultLifetime = 2 * 60 * 1000
```

Default server allocation lifetime (2 minutes, in milliseconds).

---

{#kservermaxlifetime}

#### kServerMaxLifetime

`static` `constexpr`

```cpp
uint32_t kServerMaxLifetime = 15 * 60 * 1000
```

Maximum server allocation lifetime (15 minutes, in milliseconds).

---

{#kservermaxpermissions}

#### kServerMaxPermissions

`static` `constexpr`

```cpp
int kServerMaxPermissions = 10
```

Maximum number of permissions per allocation.

---

{#kservertimerinterval}

#### kServerTimerInterval

`static` `constexpr`

```cpp
int kServerTimerInterval = 10 * 1000
```

[Server](#server-6) timer interval (10 seconds).

---

{#kserverearlymediabuffersize}

#### kServerEarlyMediaBufferSize

`static` `constexpr`

```cpp
int kServerEarlyMediaBufferSize = 8192
```

Early media buffer size.

---

{#kconnectionbindtimeout}

#### kConnectionBindTimeout

`static` `constexpr`

```cpp
int kConnectionBindTimeout = 30 * 1000
```

ConnectionBind request timeout (30 seconds, in milliseconds).

---

{#client_sock_buf_size}

#### CLIENT_SOCK_BUF_SIZE

`static` `constexpr`

```cpp
int CLIENT_SOCK_BUF_SIZE = 65536
```

Default socket buffer size for TURN client sockets.

---

{#server_sock_buf_size}

#### SERVER_SOCK_BUF_SIZE

`static` `constexpr`

```cpp
int SERVER_SOCK_BUF_SIZE = CLIENT_SOCK_BUF_SIZE * 32
```

Default socket buffer size for TURN server sockets.

---

{#kprotocoludp}

#### kProtocolUDP

`static` `constexpr`

```cpp
uint8_t kProtocolUDP = 17
```

TURN protocol numbers.

IP protocol number for UDP.

---

{#kprotocoltcp}

#### kProtocolTCP

`static` `constexpr`

```cpp
uint8_t kProtocolTCP = 6
```

IP protocol number for TCP.

---

{#kerrortryalternate}

#### kErrorTryAlternate

`static` `constexpr`

```cpp
int kErrorTryAlternate = 300
```

TURN/STUN error codes used in this module.

[Client](#client-2) should retry against an alternate TURN server.

---

{#kerrorbadrequest}

#### kErrorBadRequest

`static` `constexpr`

```cpp
int kErrorBadRequest = 400
```

[Request](#request-12) was malformed or missing required attributes.

---

{#kerrornotauthorized}

#### kErrorNotAuthorized

`static` `constexpr`

```cpp
int kErrorNotAuthorized = 401
```

Authentication failed or credentials were not accepted.

---

{#kerrorforbidden}

#### kErrorForbidden

`static` `constexpr`

```cpp
int kErrorForbidden = 403
```

[Request](#request-12) was understood but is not permitted for this client.

---

{#kerrorunknownattribute}

#### kErrorUnknownAttribute

`static` `constexpr`

```cpp
int kErrorUnknownAttribute = 420
```

[Request](#request-12) included an unsupported comprehension-required attribute.

---

{#kerrorunsupportedtransport}

#### kErrorUnsupportedTransport

`static` `constexpr`

```cpp
int kErrorUnsupportedTransport = 442
```

Requested transport protocol is not supported by the server.

---

{#kerrorallocationmismatch}

#### kErrorAllocationMismatch

`static` `constexpr`

```cpp
int kErrorAllocationMismatch = 437
```

[Client](#client-2) attempted an allocation operation that conflicts with existing state.

---

{#kerrorstalenonce}

#### kErrorStaleNonce

`static` `constexpr`

```cpp
int kErrorStaleNonce = 438
```

Authentication nonce expired and must be refreshed.

---

{#kerrorwrongcredentials}

#### kErrorWrongCredentials

`static` `constexpr`

```cpp
int kErrorWrongCredentials = 441
```

Supplied credentials were well formed but did not match the allocation.

---

{#kerrorconnectionalreadyexists}

#### kErrorConnectionAlreadyExists

`static` `constexpr`

```cpp
int kErrorConnectionAlreadyExists = 446
```

TURN TCP connection binding already exists.

---

{#kerrorconnectiontimeoutorfailure}

#### kErrorConnectionTimeoutOrFailure

`static` `constexpr`

```cpp
int kErrorConnectionTimeoutOrFailure = 447
```

TURN TCP peer connection attempt timed out or failed.

---

{#kerrorallocationquotareached}

#### kErrorAllocationQuotaReached

`static` `constexpr`

```cpp
int kErrorAllocationQuotaReached = 486
```

User or server allocation quota was exceeded.

---

{#kerrorinsufficientcapacity}

#### kErrorInsufficientCapacity

`static` `constexpr`

```cpp
int kErrorInsufficientCapacity = 508
```

[Server](#server-6) lacks capacity to satisfy the request.

---

{#kerroroperationnotsupported}

#### kErrorOperationNotSupported

`static` `constexpr`

```cpp
int kErrorOperationNotSupported = 600
```

[Request](#request-12) is recognized but not implemented by this server.

{#client-2}

## Client

```cpp
#include <icy/turn/client/client.h>
```

```cpp
class Client
```

Defined in src/turn/include/icy/turn/client/client.h:124

> **Inherits:** [`Stateful< ClientState >`](base.md#stateful), [`IAllocation`](#iallocation)
> **Subclassed by:** [`TCPClient`](#tcpclient), [`UDPClient`](#udpclient)

TURN client that manages relay allocations, permissions, and data relay via RFC 5766.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`Client`](#client-3) | `function` | Declared here |
| [`start`](#start-13) | `function` | Declared here |
| [`stop`](#stop-10) | `function` | Declared here |
| [`sendAllocate`](#sendallocate) | `function` | Declared here |
| [`addPermission`](#addpermission) | `function` | Declared here |
| [`addPermission`](#addpermission-1) | `function` | Declared here |
| [`sendCreatePermission`](#sendcreatepermission) | `function` | Declared here |
| [`sendChannelBind`](#sendchannelbind) | `function` | Declared here |
| [`sendRefresh`](#sendrefresh) | `function` | Declared here |
| [`sendData`](#senddata) | `function` | Declared here |
| [`handleResponse`](#handleresponse) | `function` | Declared here |
| [`handleAllocateResponse`](#handleallocateresponse) | `function` | Declared here |
| [`handleAllocateErrorResponse`](#handleallocateerrorresponse) | `function` | Declared here |
| [`handleCreatePermissionResponse`](#handlecreatepermissionresponse) | `function` | Declared here |
| [`handleCreatePermissionErrorResponse`](#handlecreatepermissionerrorresponse) | `function` | Declared here |
| [`handleRefreshResponse`](#handlerefreshresponse) | `function` | Declared here |
| [`handleDataIndication`](#handledataindication) | `function` | Declared here |
| [`transportProtocol`](#transportprotocol) | `function` | Declared here |
| [`createTransaction`](#createtransaction) | `function` | Declared here |
| [`authenticateRequest`](#authenticaterequest) | `function` | Declared here |
| [`sendAuthenticatedTransaction`](#sendauthenticatedtransaction) | `function` | Declared here |
| [`removeTransaction`](#removetransaction) | `function` | Declared here |
| [`mappedAddress`](#mappedaddress) | `function` | Declared here |
| [`relayedAddress`](#relayedaddress) | `function` | Declared here |
| [`closed`](#closed-5) | `function` | Declared here |
| [`observer`](#observer-1) | `function` | Declared here |
| [`options`](#options-8) | `function` | Declared here |
| [`addPermission`](#addpermission-2) | `function` | Declared here |
| [`hasPermission`](#haspermission) | `function` | Declared here |
| [`hasPermission`](#haspermission-1) | `function` | Declared here |
| [`removePermission`](#removepermission) | `function` | Declared here |
| [`removePermission`](#removepermission-1) | `function` | Declared here |
| [`_observer`](#_observer-1) | `variable` | Declared here |
| [`_options`](#_options-3) | `variable` | Declared here |
| [`_socket`](#_socket-3) | `variable` | Declared here |
| [`_timer`](#_timer-1) | `variable` | Declared here |
| [`_error`](#_error-5) | `variable` | Declared here |
| [`_mappedAddress`](#_mappedaddress) | `variable` | Declared here |
| [`_relayedAddress`](#_relayedaddress) | `variable` | Declared here |
| [`_realm`](#_realm) | `variable` | Declared here |
| [`_nonce`](#_nonce) | `variable` | Declared here |
| [`_pendingIndications`](#_pendingindications) | `variable` | Declared here |
| [`_transactions`](#_transactions) | `variable` | Declared here |
| [`setError`](#seterror-5) | `function` | Declared here |
| [`onSocketConnect`](#onsocketconnect-5) | `function` | Declared here |
| [`onSocketRecv`](#onsocketrecv-7) | `function` | Declared here |
| [`onSocketClose`](#onsocketclose-5) | `function` | Declared here |
| [`onTransactionProgress`](#ontransactionprogress) | `function` | Declared here |
| [`onStateChange`](#onstatechange-1) | `function` | Declared here |
| [`onTimer`](#ontimer-1) | `function` | Declared here |
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
| [`operator<<`](#operator-28) | `friend` | Inherited from [`IAllocation`](#iallocation) |
| [`IAllocation`](#iallocation-1) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`IAllocation`](#iallocation-2) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`IAllocation`](#iallocation-3) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`updateUsage`](#updateusage) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`setLifetime`](#setlifetime) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`setBandwidthLimit`](#setbandwidthlimit) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`expired`](#expired) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`deleted`](#deleted-1) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`bandwidthLimit`](#bandwidthlimit) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`bandwidthUsed`](#bandwidthused) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`bandwidthRemaining`](#bandwidthremaining) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`timeRemaining`](#timeremaining) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`tuple`](#tuple) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`username`](#username-3) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`lifetime`](#lifetime-1) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`permissions`](#permissions) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`relayedAddress`](#relayedaddress-1) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`addPermission`](#addpermission-3) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`addPermission`](#addpermission-4) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`addPermissions`](#addpermissions) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`removePermission`](#removepermission-2) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`removePermission`](#removepermission-3) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`removeAllPermissions`](#removeallpermissions) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`removeExpiredPermissions`](#removeexpiredpermissions) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`hasPermission`](#haspermission-2) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`hasPermission`](#haspermission-3) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`print`](#print-11) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`_tuple`](#_tuple) | `variable` | Inherited from [`IAllocation`](#iallocation) |
| [`_username`](#_username-2) | `variable` | Inherited from [`IAllocation`](#iallocation) |
| [`_permissions`](#_permissions) | `variable` | Inherited from [`IAllocation`](#iallocation) |
| [`_lifetime`](#_lifetime) | `variable` | Inherited from [`IAllocation`](#iallocation) |
| [`_bandwidthLimit`](#_bandwidthlimit) | `variable` | Inherited from [`IAllocation`](#iallocation) |
| [`_bandwidthUsed`](#_bandwidthused) | `variable` | Inherited from [`IAllocation`](#iallocation) |
| [`_createdAt`](#_createdat) | `variable` | Inherited from [`IAllocation`](#iallocation) |
| [`_updatedAt`](#_updatedat) | `variable` | Inherited from [`IAllocation`](#iallocation) |
| [`_deleted`](#_deleted) | `variable` | Inherited from [`IAllocation`](#iallocation) |

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

### Inherited from [`IAllocation`](#iallocation)

| Kind | Name | Description |
|------|------|-------------|
| `friend` | [`operator<<`](#operator-28) `inline` |  |
| `function` | [`IAllocation`](#iallocation-1)  |  |
| `function` | [`IAllocation`](#iallocation-2)  | Deleted constructor. |
| `function` | [`IAllocation`](#iallocation-3)  | Deleted constructor. |
| `function` | [`updateUsage`](#updateusage) `virtual` | Updates the last-activity timestamp and accumulates bandwidth usage. Call this whenever data is relayed through the allocation. |
| `function` | [`setLifetime`](#setlifetime) `virtual` | Sets the allocation lifetime in seconds and resets the activity timestamp, effectively extending the expiry from the current moment. |
| `function` | [`setBandwidthLimit`](#setbandwidthlimit) `virtual` | Sets the maximum number of bytes this allocation may relay in its lifetime. Pass 0 to disable bandwidth limiting. |
| `function` | [`expired`](#expired) `virtual` `const` `nodiscard` |  |
| `function` | [`deleted`](#deleted-1) `virtual` `const` `nodiscard` | Returns true if the allocation's deleted flag is set and or if the allocation has expired. |
| `function` | [`bandwidthLimit`](#bandwidthlimit) `virtual` `const` `nodiscard` |  |
| `function` | [`bandwidthUsed`](#bandwidthused) `virtual` `const` `nodiscard` |  |
| `function` | [`bandwidthRemaining`](#bandwidthremaining) `virtual` `const` `nodiscard` |  |
| `function` | [`timeRemaining`](#timeremaining) `virtual` `const` `nodiscard` |  |
| `function` | [`tuple`](#tuple) `virtual` |  |
| `function` | [`username`](#username-3) `virtual` `const` `nodiscard` |  |
| `function` | [`lifetime`](#lifetime-1) `virtual` `const` `nodiscard` |  |
| `function` | [`permissions`](#permissions) `virtual` `const` `nodiscard` |  |
| `function` | [`relayedAddress`](#relayedaddress-1) `virtual` `const` `nodiscard` |  |
| `function` | [`addPermission`](#addpermission-3) `virtual` | Adds a permission for `ip`, or refreshes the existing one. |
| `function` | [`addPermission`](#addpermission-4) `virtual` | Adds a permission for `address`, or refreshes the existing one. The port is ignored; TURN permissions are IP-only. |
| `function` | [`addPermissions`](#addpermissions) `virtual` | Adds (or refreshes) permissions for multiple IPs. |
| `function` | [`removePermission`](#removepermission-2) `virtual` | Removes the permission for `ip` if present. |
| `function` | [`removePermission`](#removepermission-3) `virtual` | Removes the permission for `address` if present. The port is ignored; TURN permissions are IP-only. |
| `function` | [`removeAllPermissions`](#removeallpermissions) `virtual` | Removes all permissions from the list. |
| `function` | [`removeExpiredPermissions`](#removeexpiredpermissions) `virtual` | Removes any permissions whose 5-minute lifetime has elapsed. |
| `function` | [`hasPermission`](#haspermission-2) `virtual` `nodiscard` | Checks whether a permission exists for `peerIP`. Local addresses (192.168.x.x and 127.x.x.x) are always permitted. |
| `function` | [`hasPermission`](#haspermission-3) `virtual` `nodiscard` | Checks whether a permission exists for `peerAddress`. The port is ignored; TURN permissions are IP-only. |
| `function` | [`print`](#print-11) `virtual` `const` `inline` |  |
| `variable` | [`_tuple`](#_tuple)  |  |
| `variable` | [`_username`](#_username-2)  |  |
| `variable` | [`_permissions`](#_permissions)  |  |
| `variable` | [`_lifetime`](#_lifetime)  |  |
| `variable` | [`_bandwidthLimit`](#_bandwidthlimit)  |  |
| `variable` | [`_bandwidthUsed`](#_bandwidthused)  |  |
| `variable` | [`_createdAt`](#_createdat)  |  |
| `variable` | [`_updatedAt`](#_updatedat)  |  |
| `variable` | [`_deleted`](#_deleted)  |  |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Client`](#client-3)  |  |
| `void` | [`start`](#start-13) `virtual` | Connects the socket to the TURN server and starts the allocation sequence. Permissions must be added via [addPermission()](#addpermission) before calling this. |
| `void` | [`stop`](#stop-10) `virtual` | Stops the timer, cancels pending transactions, and closes the socket. |
| `void` | [`sendAllocate`](#sendallocate) `virtual` | Sends an Allocate request to the server with the configured transport and lifetime. On first call the server will typically respond with a 401 challenge; the client re-sends with credentials automatically. |
| `void` | [`addPermission`](#addpermission) `virtual` | Adds multiple peer IP addresses to the permission list. |
| `void` | [`addPermission`](#addpermission-1) `virtual` `override` | Adds a single peer IP to the permission list, or refreshes it if already present. Permissions should be added before [start()](#start-13); they may also be added later, in which case a new CreatePermission request is required. |
| `void` | [`sendCreatePermission`](#sendcreatepermission) `virtual` | Sends a CreatePermission request for all IPs currently in the permission list. Called automatically after allocation succeeds and periodically by the timer to refresh expiring permissions. |
| `void` | [`sendChannelBind`](#sendchannelbind) `virtual` | Channel bindings (RFC 5766 Section 11) are intentionally not implemented. They are a bandwidth optimization that replaces STUN-framed Send/Data indications with a compact 4-byte ChannelData header. This only benefits high-throughput media relay scenarios; in practice, media flows directly via ICE/DTLS rather than through this TURN client's data path, so the optimization is not worth the complexity (channel number allocation, 10-minute binding refresh timers, ChannelData wire framing). Data relay uses [sendData()](#senddata) with Send Indications instead. |
| `void` | [`sendRefresh`](#sendrefresh) `virtual` | Sends a Refresh request to extend the allocation lifetime. Called automatically by the timer when roughly one-third of the lifetime remains. |
| `void` | [`sendData`](#senddata) `virtual` | Sends a Send Indication to relay `data` to `peerAddress` through the TURN server. If permissions are still being negotiated the indication is queued and flushed once CreatePermission succeeds. |
| `bool` | [`handleResponse`](#handleresponse) `virtual` | Dispatches an incoming STUN/TURN response to the appropriate handler. |
| `void` | [`handleAllocateResponse`](#handleallocateresponse) `virtual` | Processes a successful Allocate response; extracts mapped/relayed addresses and advances the state to Authorizing, then sends CreatePermission. |
| `void` | [`handleAllocateErrorResponse`](#handleallocateerrorresponse) `virtual` | Handles an Allocate error response; manages the 401 challenge/re-send flow and sets the client to Failed for unrecoverable errors. |
| `void` | [`handleCreatePermissionResponse`](#handlecreatepermissionresponse) `virtual` | Handles a successful CreatePermission response; flushes queued Send Indications and advances state to Success. |
| `void` | [`handleCreatePermissionErrorResponse`](#handlecreatepermissionerrorresponse) `virtual` | Handles a failed CreatePermission response; clears all permissions and sets the client to Failed. |
| `void` | [`handleRefreshResponse`](#handlerefreshresponse) `virtual` | Handles a Refresh response; updates the stored lifetime. |
| `void` | [`handleDataIndication`](#handledataindication) `virtual` | Handles an incoming Data Indication; extracts peer address and data and forwards to [ClientObserver::onRelayDataReceived()](#onrelaydatareceived). |
| `int` | [`transportProtocol`](#transportprotocol) `virtual` |  |
| `stun::Transaction::Ptr` | [`createTransaction`](#createtransaction) `virtual` | Creates a new STUN transaction, registers the progress callback, and adds it to the active transaction list. |
| `void` | [`authenticateRequest`](#authenticaterequest) `virtual` | Adds STUN long-term credential attributes (Username, Realm, Nonce, MessageIntegrity) to `request` if the realm has been received from the server. |
| `bool` | [`sendAuthenticatedTransaction`](#sendauthenticatedtransaction) `virtual` | Calls [authenticateRequest()](#authenticaterequest) then sends the transaction. |
| `stun::Transaction::Ptr` | [`removeTransaction`](#removetransaction) `virtual` | Removes a transaction from the active list. The [IntrusivePtr](base.md#intrusiveptr) keeps the object alive until the caller's copy is released. |
| `net::Address` | [`mappedAddress`](#mappedaddress) `const` `nodiscard` |  |
| `net::Address` | [`relayedAddress`](#relayedaddress) `virtual` `const` `nodiscard` `override` |  |
| `bool` | [`closed`](#closed-5) `const` `nodiscard` |  |
| `ClientObserver &` | [`observer`](#observer-1)  |  |
| `const Options &` | [`options`](#options-8) `const` `nodiscard` |  |
| `void` | [`addPermission`](#addpermission-2) `virtual` | Adds a permission for `address`, or refreshes the existing one. The port is ignored; TURN permissions are IP-only. |
| `bool` | [`hasPermission`](#haspermission) `virtual` `nodiscard` | Checks whether a permission exists for `peerIP`. Local addresses (192.168.x.x and 127.x.x.x) are always permitted. |
| `bool` | [`hasPermission`](#haspermission-1) `virtual` `nodiscard` | Checks whether a permission exists for `peerAddress`. The port is ignored; TURN permissions are IP-only. |
| `void` | [`removePermission`](#removepermission) `virtual` | Removes the permission for `ip` if present. |
| `void` | [`removePermission`](#removepermission-1) `virtual` | Removes the permission for `address` if present. The port is ignored; TURN permissions are IP-only. |

---

{#client-3}

#### Client

```cpp
Client(ClientObserver & observer, const Options & options, const net::Socket::Ptr & socket)
```

Defined in src/turn/include/icy/turn/client/client.h:161

#### Parameters
* `observer` Observer to receive lifecycle and data callbacks. 

* `options` [Configuration](base.md#configuration) for this client instance. 

* `socket` Underlying transport socket (TCP or UDP).

---

{#start-13}

#### start

`virtual`

```cpp
virtual void start()
```

Defined in src/turn/include/icy/turn/client/client.h:166

Connects the socket to the TURN server and starts the allocation sequence. Permissions must be added via [addPermission()](#addpermission) before calling this.

##### Reimplemented by

- [`start`](#start-15)

---

{#stop-10}

#### stop

`virtual`

```cpp
virtual void stop()
```

Defined in src/turn/include/icy/turn/client/client.h:169

Stops the timer, cancels pending transactions, and closes the socket.

##### Reimplemented by

- [`stop`](#stop-12)

---

{#sendallocate}

#### sendAllocate

`virtual`

```cpp
virtual void sendAllocate()
```

Defined in src/turn/include/icy/turn/client/client.h:175

Sends an Allocate request to the server with the configured transport and lifetime. On first call the server will typically respond with a 401 challenge; the client re-sends with credentials automatically.

---

{#addpermission}

#### addPermission

`virtual`

```cpp
virtual void addPermission(const IPList & peerIPs)
```

Defined in src/turn/include/icy/turn/client/client.h:179

Adds multiple peer IP addresses to the permission list. 
#### Parameters
* `peerIPs` List of IPv4 address strings to permit.

---

{#addpermission-1}

#### addPermission

`virtual` `override`

```cpp
virtual void addPermission(const std::string & ip) override
```

Defined in src/turn/include/icy/turn/client/client.h:186

Adds a single peer IP to the permission list, or refreshes it if already present. Permissions should be added before [start()](#start-13); they may also be added later, in which case a new CreatePermission request is required. 
#### Parameters
* `ip` IPv4 address string of the permitted peer.

##### Reimplements

- [`addPermission`](#addpermission-3)

---

{#sendcreatepermission}

#### sendCreatePermission

`virtual`

```cpp
virtual void sendCreatePermission()
```

Defined in src/turn/include/icy/turn/client/client.h:191

Sends a CreatePermission request for all IPs currently in the permission list. Called automatically after allocation succeeds and periodically by the timer to refresh expiring permissions.

---

{#sendchannelbind}

#### sendChannelBind

`virtual`

```cpp
virtual void sendChannelBind(const std::string & peerIP)
```

Defined in src/turn/include/icy/turn/client/client.h:202

Channel bindings (RFC 5766 Section 11) are intentionally not implemented. They are a bandwidth optimization that replaces STUN-framed Send/Data indications with a compact 4-byte ChannelData header. This only benefits high-throughput media relay scenarios; in practice, media flows directly via ICE/DTLS rather than through this TURN client's data path, so the optimization is not worth the complexity (channel number allocation, 10-minute binding refresh timers, ChannelData wire framing). Data relay uses [sendData()](#senddata) with Send Indications instead. 
#### Parameters
* `peerIP` Unused; always throws std::logic_error.

---

{#sendrefresh}

#### sendRefresh

`virtual`

```cpp
virtual void sendRefresh()
```

Defined in src/turn/include/icy/turn/client/client.h:207

Sends a Refresh request to extend the allocation lifetime. Called automatically by the timer when roughly one-third of the lifetime remains.

---

{#senddata}

#### sendData

`virtual`

```cpp
virtual void sendData(const char * data, size_t size, const net::Address & peerAddress)
```

Defined in src/turn/include/icy/turn/client/client.h:215

Sends a Send Indication to relay `data` to `peerAddress` through the TURN server. If permissions are still being negotiated the indication is queued and flushed once CreatePermission succeeds. 
#### Parameters
* `data` Pointer to the payload to relay. 

* `size` Number of bytes to relay. 

* `peerAddress` Destination peer address (must have an active permission).

##### Reimplemented by

- [`sendData`](#senddata-1)

---

{#handleresponse}

#### handleResponse

`virtual`

```cpp
virtual bool handleResponse(const stun::Message & response)
```

Defined in src/turn/include/icy/turn/client/client.h:220

Dispatches an incoming STUN/TURN response to the appropriate handler. 
#### Parameters
* `response` Received STUN message. 

#### Returns
true if the message was handled, false if it was unrecognised.

##### Reimplemented by

- [`handleResponse`](#handleresponse-1)

---

{#handleallocateresponse}

#### handleAllocateResponse

`virtual`

```cpp
virtual void handleAllocateResponse(const stun::Message & response)
```

Defined in src/turn/include/icy/turn/client/client.h:225

Processes a successful Allocate response; extracts mapped/relayed addresses and advances the state to Authorizing, then sends CreatePermission. 
#### Parameters
* `response` Allocate success response from the server.

---

{#handleallocateerrorresponse}

#### handleAllocateErrorResponse

`virtual`

```cpp
virtual void handleAllocateErrorResponse(const stun::Message & response)
```

Defined in src/turn/include/icy/turn/client/client.h:230

Handles an Allocate error response; manages the 401 challenge/re-send flow and sets the client to Failed for unrecoverable errors. 
#### Parameters
* `response` Allocate error response from the server.

---

{#handlecreatepermissionresponse}

#### handleCreatePermissionResponse

`virtual`

```cpp
virtual void handleCreatePermissionResponse(const stun::Message & response)
```

Defined in src/turn/include/icy/turn/client/client.h:235

Handles a successful CreatePermission response; flushes queued Send Indications and advances state to Success. 
#### Parameters
* `response` CreatePermission success response from the server.

---

{#handlecreatepermissionerrorresponse}

#### handleCreatePermissionErrorResponse

`virtual`

```cpp
virtual void handleCreatePermissionErrorResponse(const stun::Message & response)
```

Defined in src/turn/include/icy/turn/client/client.h:240

Handles a failed CreatePermission response; clears all permissions and sets the client to Failed. 
#### Parameters
* `response` CreatePermission error response from the server.

---

{#handlerefreshresponse}

#### handleRefreshResponse

`virtual`

```cpp
virtual void handleRefreshResponse(const stun::Message & response)
```

Defined in src/turn/include/icy/turn/client/client.h:244

Handles a Refresh response; updates the stored lifetime. 
#### Parameters
* `response` Refresh response from the server.

---

{#handledataindication}

#### handleDataIndication

`virtual`

```cpp
virtual void handleDataIndication(const stun::Message & response)
```

Defined in src/turn/include/icy/turn/client/client.h:249

Handles an incoming Data Indication; extracts peer address and data and forwards to [ClientObserver::onRelayDataReceived()](#onrelaydatareceived). 
#### Parameters
* `response` Data Indication message from the server.

---

{#transportprotocol}

#### transportProtocol

`virtual`

```cpp
virtual int transportProtocol()
```

Defined in src/turn/include/icy/turn/client/client.h:253

#### Returns
The IANA protocol number for the underlying transport (kProtocolUDP or kProtocolTCP).

##### Reimplemented by

- [`transportProtocol`](#transportprotocol-1)

---

{#createtransaction}

#### createTransaction

`virtual`

```cpp
virtual stun::Transaction::Ptr createTransaction(const net::Socket::Ptr & socket = nullptr)
```

Defined in src/turn/include/icy/turn/client/client.h:259

Creates a new STUN transaction, registers the progress callback, and adds it to the active transaction list. 
#### Parameters
* `socket` Optional override socket; uses the control socket if nullptr. 

#### Returns
[IntrusivePtr](base.md#intrusiveptr) to the new transaction.

---

{#authenticaterequest}

#### authenticateRequest

`virtual`

```cpp
virtual void authenticateRequest(stun::Message & request)
```

Defined in src/turn/include/icy/turn/client/client.h:264

Adds STUN long-term credential attributes (Username, Realm, Nonce, MessageIntegrity) to `request` if the realm has been received from the server. 
#### Parameters
* `request` Message to decorate with authentication attributes.

---

{#sendauthenticatedtransaction}

#### sendAuthenticatedTransaction

`virtual`

```cpp
virtual bool sendAuthenticatedTransaction(stun::Transaction * transaction)
```

Defined in src/turn/include/icy/turn/client/client.h:269

Calls [authenticateRequest()](#authenticaterequest) then sends the transaction. 
#### Parameters
* `transaction` Transaction whose request to authenticate and send. 

#### Returns
true if the send succeeded.

---

{#removetransaction}

#### removeTransaction

`virtual`

```cpp
virtual stun::Transaction::Ptr removeTransaction(stun::Transaction * transaction)
```

Defined in src/turn/include/icy/turn/client/client.h:275

Removes a transaction from the active list. The [IntrusivePtr](base.md#intrusiveptr) keeps the object alive until the caller's copy is released. 
#### Parameters
* `transaction` Transaction to remove. 

#### Returns
The removed Ptr, or nullptr if not found.

---

{#mappedaddress}

#### mappedAddress

`const` `nodiscard`

```cpp
[[nodiscard]] net::Address mappedAddress() const
```

Defined in src/turn/include/icy/turn/client/client.h:278

#### Returns
The server-reflexive (mapped) address assigned by the TURN server.

---

{#relayedaddress}

#### relayedAddress

`virtual` `const` `nodiscard` `override`

```cpp
[[nodiscard]] virtual net::Address relayedAddress() const override
```

Defined in src/turn/include/icy/turn/client/client.h:281

#### Returns
The relayed transport address assigned by the TURN server.

##### Reimplements

- [`relayedAddress`](#relayedaddress-1)

---

{#closed-5}

#### closed

`const` `nodiscard`

```cpp
[[nodiscard]] bool closed() const
```

Defined in src/turn/include/icy/turn/client/client.h:284

#### Returns
true when the client is in the None or Failed state.

---

{#observer-1}

#### observer

```cpp
ClientObserver & observer()
```

Defined in src/turn/include/icy/turn/client/client.h:287

#### Returns
Reference to the observer provided at construction.

---

{#options-8}

#### options

`const` `nodiscard`

```cpp
[[nodiscard]] const Options & options() const
```

Defined in src/turn/include/icy/turn/client/client.h:290

#### Returns
Reference to the immutable options struct.

---

{#addpermission-2}

#### addPermission

`virtual`

```cpp
virtual void addPermission(const net::Address & address)
```

Defined in src/turn/include/icy/turn/client/client.h:154

Adds a permission for `address`, or refreshes the existing one. The port is ignored; TURN permissions are IP-only.

##### Reimplements

- [`addPermission`](#addpermission-4)

---

{#haspermission}

#### hasPermission

`virtual` `nodiscard`

```cpp
[[nodiscard]] virtual bool hasPermission(const std::string & peerIP)
```

Defined in src/turn/include/icy/turn/client/client.h:155

Checks whether a permission exists for `peerIP`. Local addresses (192.168.x.x and 127.x.x.x) are always permitted. 
#### Parameters
* `peerIP` IPv4 address string to check. 

#### Returns
true if a valid (non-expired) permission exists.

##### Reimplements

- [`hasPermission`](#haspermission-2)

---

{#haspermission-1}

#### hasPermission

`virtual` `nodiscard`

```cpp
[[nodiscard]] virtual bool hasPermission(const net::Address & peerAddress)
```

Defined in src/turn/include/icy/turn/client/client.h:155

Checks whether a permission exists for `peerAddress`. The port is ignored; TURN permissions are IP-only.

##### Reimplements

- [`hasPermission`](#haspermission-3)

---

{#removepermission}

#### removePermission

`virtual`

```cpp
virtual void removePermission(const std::string & ip)
```

Defined in src/turn/include/icy/turn/client/client.h:156

Removes the permission for `ip` if present. 
#### Parameters
* `ip` IPv4 address string to remove.

##### Reimplements

- [`removePermission`](#removepermission-2)

---

{#removepermission-1}

#### removePermission

`virtual`

```cpp
virtual void removePermission(const net::Address & address)
```

Defined in src/turn/include/icy/turn/client/client.h:156

Removes the permission for `address` if present. The port is ignored; TURN permissions are IP-only.

##### Reimplements

- [`removePermission`](#removepermission-3)

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `ClientObserver &` | [`_observer`](#_observer-1)  |  |
| `Options` | [`_options`](#_options-3)  |  |
| `net::SocketEmitter` | [`_socket`](#_socket-3)  |  |
| `Timer` | [`_timer`](#_timer-1)  |  |
| `icy::Error` | [`_error`](#_error-5)  |  |
| `net::Address` | [`_mappedAddress`](#_mappedaddress)  |  |
| `net::Address` | [`_relayedAddress`](#_relayedaddress)  |  |
| `std::string` | [`_realm`](#_realm)  |  |
| `std::string` | [`_nonce`](#_nonce)  |  |
| `std::deque< stun::Message >` | [`_pendingIndications`](#_pendingindications)  | A list of queued Send indication packets awaiting server permissions. |
| `std::vector< stun::Transaction::Ptr >` | [`_transactions`](#_transactions)  | A list containing currently active transactions. |

---

{#_observer-1}

#### _observer

```cpp
ClientObserver & _observer
```

Defined in src/turn/include/icy/turn/client/client.h:324

---

{#_options-3}

#### _options

```cpp
Options _options
```

Defined in src/turn/include/icy/turn/client/client.h:325

---

{#_socket-3}

#### _socket

```cpp
net::SocketEmitter _socket
```

Defined in src/turn/include/icy/turn/client/client.h:326

---

{#_timer-1}

#### _timer

```cpp
Timer _timer
```

Defined in src/turn/include/icy/turn/client/client.h:327

---

{#_error-5}

#### _error

```cpp
icy::Error _error
```

Defined in src/turn/include/icy/turn/client/client.h:328

---

{#_mappedaddress}

#### _mappedAddress

```cpp
net::Address _mappedAddress
```

Defined in src/turn/include/icy/turn/client/client.h:329

---

{#_relayedaddress}

#### _relayedAddress

```cpp
net::Address _relayedAddress
```

Defined in src/turn/include/icy/turn/client/client.h:330

---

{#_realm}

#### _realm

```cpp
std::string _realm
```

Defined in src/turn/include/icy/turn/client/client.h:332

---

{#_nonce}

#### _nonce

```cpp
std::string _nonce
```

Defined in src/turn/include/icy/turn/client/client.h:333

---

{#_pendingindications}

#### _pendingIndications

```cpp
std::deque< stun::Message > _pendingIndications
```

Defined in src/turn/include/icy/turn/client/client.h:336

A list of queued Send indication packets awaiting server permissions.

---

{#_transactions}

#### _transactions

```cpp
std::vector< stun::Transaction::Ptr > _transactions
```

Defined in src/turn/include/icy/turn/client/client.h:339

A list containing currently active transactions.

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`setError`](#seterror-5) `virtual` | Sets the error field and transitions the client to the Failed state. |
| `bool` | [`onSocketConnect`](#onsocketconnect-5) `virtual` | Socket connect callback; starts the timer and sends the first Allocate request. |
| `bool` | [`onSocketRecv`](#onsocketrecv-7) `virtual` | Socket receive callback; parses STUN messages from the buffer and dispatches them. |
| `bool` | [`onSocketClose`](#onsocketclose-5) `virtual` | Socket close callback; shuts down the client and records the socket error. |
| `void` | [`onTransactionProgress`](#ontransactionprogress) `virtual` | STUN transaction state-change callback; handles Success and Failed outcomes. |
| `void` | [`onStateChange`](#onstatechange-1) `override` | Forwards state-change events to the observer. |
| `void` | [`onTimer`](#ontimer-1) `virtual` | Periodic timer callback; re-allocates on expiry or refreshes when lifetime is below one-third remaining. Also calls [ClientObserver::onTimer()](#ontimer-2). |

---

{#seterror-5}

#### setError

`virtual`

```cpp
virtual void setError(const icy::Error & error)
```

Defined in src/turn/include/icy/turn/client/client.h:295

Sets the error field and transitions the client to the Failed state. 
#### Parameters
* `error` [Error](base.md#error) descriptor.

---

{#onsocketconnect-5}

#### onSocketConnect

`virtual`

```cpp
virtual bool onSocketConnect(net::Socket & socket)
```

Defined in src/turn/include/icy/turn/client/client.h:299

Socket connect callback; starts the timer and sends the first Allocate request. 
#### Parameters
* `socket` The connected socket.

---

{#onsocketrecv-7}

#### onSocketRecv

`virtual`

```cpp
virtual bool onSocketRecv(net::Socket & socket, const MutableBuffer & buffer, const net::Address & peerAddress)
```

Defined in src/turn/include/icy/turn/client/client.h:305

Socket receive callback; parses STUN messages from the buffer and dispatches them. 
#### Parameters
* `socket` The receiving socket. 

* `buffer` Received data. 

* `peerAddress` Source address of the received data.

---

{#onsocketclose-5}

#### onSocketClose

`virtual`

```cpp
virtual bool onSocketClose(net::Socket & socket)
```

Defined in src/turn/include/icy/turn/client/client.h:309

Socket close callback; shuts down the client and records the socket error. 
#### Parameters
* `socket` The closed socket.

---

{#ontransactionprogress}

#### onTransactionProgress

`virtual`

```cpp
virtual void onTransactionProgress(void * sender, TransactionState & state, const TransactionState &)
```

Defined in src/turn/include/icy/turn/client/client.h:314

STUN transaction state-change callback; handles Success and Failed outcomes. 
#### Parameters
* `sender` Pointer to the [stun::Transaction](stun.md#transaction-2) that changed state. 

* `state` The new transaction state.

---

{#onstatechange-1}

#### onStateChange

`override`

```cpp
void onStateChange(ClientState & state, const ClientState & oldState) override
```

Defined in src/turn/include/icy/turn/client/client.h:317

Forwards state-change events to the observer.

---

{#ontimer-1}

#### onTimer

`virtual`

```cpp
virtual void onTimer()
```

Defined in src/turn/include/icy/turn/client/client.h:321

Periodic timer callback; re-allocates on expiry or refreshes when lifetime is below one-third remaining. Also calls [ClientObserver::onTimer()](#ontimer-2).

{#options-9}

## Options

```cpp
#include <icy/turn/client/client.h>
```

```cpp
struct Options
```

Defined in src/turn/include/icy/turn/client/client.h:129

[Configuration](base.md#configuration) options for the TURN client.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`software`](#software) | `variable` | Declared here |
| [`username`](#username-2) | `variable` | Declared here |
| [`password`](#password-2) | `variable` | Declared here |
| [`timeout`](#timeout-1) | `variable` | Declared here |
| [`lifetime`](#lifetime) | `variable` | Declared here |
| [`timerInterval`](#timerinterval) | `variable` | Declared here |
| [`serverAddr`](#serveraddr) | `variable` | Declared here |
| [`Options`](#options-10) | `function` | Declared here |

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`software`](#software)  |  |
| `std::string` | [`username`](#username-2)  |  |
| `std::string` | [`password`](#password-2)  |  |
| `long` | [`timeout`](#timeout-1)  |  |
| `std::int64_t` | [`lifetime`](#lifetime)  |  |
| `std::int64_t` | [`timerInterval`](#timerinterval)  |  |
| `net::Address` | [`serverAddr`](#serveraddr)  |  |

---

{#software}

#### software

```cpp
std::string software
```

Defined in src/turn/include/icy/turn/client/client.h:131

---

{#username-2}

#### username

```cpp
std::string username
```

Defined in src/turn/include/icy/turn/client/client.h:132

---

{#password-2}

#### password

```cpp
std::string password
```

Defined in src/turn/include/icy/turn/client/client.h:134

---

{#timeout-1}

#### timeout

```cpp
long timeout
```

Defined in src/turn/include/icy/turn/client/client.h:136

---

{#lifetime}

#### lifetime

```cpp
std::int64_t lifetime
```

Defined in src/turn/include/icy/turn/client/client.h:137

---

{#timerinterval}

#### timerInterval

```cpp
std::int64_t timerInterval
```

Defined in src/turn/include/icy/turn/client/client.h:138

---

{#serveraddr}

#### serverAddr

```cpp
net::Address serverAddr
```

Defined in src/turn/include/icy/turn/client/client.h:139

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Options`](#options-10) `inline` |  |

---

{#options-10}

#### Options

`inline`

```cpp
inline Options()
```

Defined in src/turn/include/icy/turn/client/client.h:140

{#fivetuple}

## FiveTuple

```cpp
#include <icy/turn/fivetuple.h>
```

```cpp
class FiveTuple
```

Defined in src/turn/include/icy/turn/fivetuple.h:69

TURN allocation identity tuple: client remote address, server local address, and transport protocol.

The TURN 5-tuple consists of a local address, a remote address, and the transport protocol used by the client to communicate with the server.

```cpp
                                                          +---------+
                                                          |         |
                                                          | External|
                                                        / | Client  |
                                                      //  |         |
                                                     /    |         |
                                                   //     +---------+
                                                  /
                                                //
                +-+                            /
                | |                           /
                | |                         //
 +---------+    | |          +---------+   /              +---------+
 |         |    |N|          |         | //               |         |
 | TURN    |    | |          |         |/                 | External|
 | Client  |----|A|----------|   TURN  |------------------| Client  |
 |         |    | |^        ^|  Server |^                ^|         |
 |         |    |T||        ||         ||                ||         |
 +---------+    | ||        |+---------+|                |+---------+
    ^           | ||        |           |                |
    |           | ||        |           |                |
    |           +-+|        |           |                |
    |              |        |           |                |
    |
               Internal     Internal    External         External
Client         Remote       Local       Local            Remote
Performing     Transport    Transport   Transport        Transport
Allocations    Address      Address     Address          Address

                   |          |            |                |
                   +-----+----+            +--------+-------+
                         |                          |
                         |                          |

                       Internal                External
                       5-Tuple                 5-tuple
```

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`operator<<`](#operator-25) | `friend` | Declared here |
| [`FiveTuple`](#fivetuple-1) | `function` | Declared here |
| [`FiveTuple`](#fivetuple-2) | `function` | Declared here |
| [`FiveTuple`](#fivetuple-3) | `function` | Declared here |
| [`remote`](#remote-2) | `function` | Declared here |
| [`local`](#local-2) | `function` | Declared here |
| [`transport`](#transport-4) | `function` | Declared here |
| [`remote`](#remote-3) | `function` | Declared here |
| [`local`](#local-3) | `function` | Declared here |
| [`transport`](#transport-5) | `function` | Declared here |
| [`operator==`](#operator-26) | `function` | Declared here |
| [`operator<`](#operator-27) | `function` | Declared here |
| [`toString`](#tostring-7) | `function` | Declared here |
| [`_remote`](#_remote-1) | `variable` | Declared here |
| [`_local`](#_local-1) | `variable` | Declared here |
| [`_transport`](#_transport) | `variable` | Declared here |

### Friends

| Name | Description |
|------|-------------|
| [`operator<<`](#operator-25) `inline` |  |

---

{#operator-25}

#### operator<<

`inline`

```cpp
friend inline std::ostream & operator<<(std::ostream & stream, const FiveTuple & tuple)
```

Defined in src/turn/include/icy/turn/fivetuple.h:118

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`FiveTuple`](#fivetuple-1)  | Constructs a default [FiveTuple](#fivetuple) with empty addresses and UDP transport. |
|  | [`FiveTuple`](#fivetuple-2)  | Constructs a [FiveTuple](#fivetuple) from explicit addresses and transport. |
|  | [`FiveTuple`](#fivetuple-3)  | Copy constructor. |
| `const net::Address &` | [`remote`](#remote-2) `const` `inline` `nodiscard` |  |
| `const net::Address &` | [`local`](#local-2) `const` `inline` `nodiscard` |  |
| `const net::TransportType &` | [`transport`](#transport-4) `const` `inline` `nodiscard` |  |
| `void` | [`remote`](#remote-3) `inline` | Sets the remote address. |
| `void` | [`local`](#local-3) `inline` | Sets the local address. |
| `void` | [`transport`](#transport-5) `inline` | Sets the transport protocol. |
| `bool` | [`operator==`](#operator-26) `const` | Equality comparison; all three components must match. |
| `bool` | [`operator<`](#operator-27) `const` | Less-than ordering based on remote then local port; used as std::map key. |
| `std::string` | [`toString`](#tostring-7) `const` `nodiscard` |  |

---

{#fivetuple-1}

#### FiveTuple

```cpp
FiveTuple()
```

Defined in src/turn/include/icy/turn/fivetuple.h:73

Constructs a default [FiveTuple](#fivetuple) with empty addresses and UDP transport.

---

{#fivetuple-2}

#### FiveTuple

```cpp
FiveTuple(const net::Address & remote, const net::Address & local, net::TransportType transport)
```

Defined in src/turn/include/icy/turn/fivetuple.h:79

Constructs a [FiveTuple](#fivetuple) from explicit addresses and transport. 
#### Parameters
* `remote` [Client](#client-2)'s remote transport address (as seen by the server). 

* `local` [Server](#server-6)'s local transport address. 

* `transport` Protocol in use ([net::UDP](net.md#namespaceicy_1_1net_1af6e0b9e4d8542b9f4db630e2523bc4b7ad002734ee233012c57a0ca3ed2bff53e) or [net::TCP](net.md#namespaceicy_1_1net_1af6e0b9e4d8542b9f4db630e2523bc4b7acd16ad02f430bf3259a04674e57f7105)).

---

{#fivetuple-3}

#### FiveTuple

```cpp
FiveTuple(const FiveTuple & r)
```

Defined in src/turn/include/icy/turn/fivetuple.h:83

Copy constructor.

---

{#remote-2}

#### remote

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline const net::Address & remote() const
```

Defined in src/turn/include/icy/turn/fivetuple.h:86

#### Returns
The remote (client-side) transport address.

---

{#local-2}

#### local

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline const net::Address & local() const
```

Defined in src/turn/include/icy/turn/fivetuple.h:89

#### Returns
The local (server-side) transport address.

---

{#transport-4}

#### transport

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline const net::TransportType & transport() const
```

Defined in src/turn/include/icy/turn/fivetuple.h:92

#### Returns
The transport protocol for this tuple.

---

{#remote-3}

#### remote

`inline`

```cpp
inline void remote(const net::Address & remote)
```

Defined in src/turn/include/icy/turn/fivetuple.h:96

Sets the remote address. 
#### Parameters
* `remote` New remote address.

---

{#local-3}

#### local

`inline`

```cpp
inline void local(const net::Address & local)
```

Defined in src/turn/include/icy/turn/fivetuple.h:100

Sets the local address. 
#### Parameters
* `local` New local address.

---

{#transport-5}

#### transport

`inline`

```cpp
inline void transport(const net::TransportType & transport)
```

Defined in src/turn/include/icy/turn/fivetuple.h:104

Sets the transport protocol. 
#### Parameters
* `transport` New transport type.

---

{#operator-26}

#### operator==

`const`

```cpp
bool operator==(const FiveTuple & r) const
```

Defined in src/turn/include/icy/turn/fivetuple.h:110

Equality comparison; all three components must match.

---

{#operator-27}

#### operator<

`const`

```cpp
bool operator<(const FiveTuple & r) const
```

Defined in src/turn/include/icy/turn/fivetuple.h:113

Less-than ordering based on remote then local port; used as std::map key.

---

{#tostring-7}

#### toString

`const` `nodiscard`

```cpp
[[nodiscard]] std::string toString() const
```

Defined in src/turn/include/icy/turn/fivetuple.h:116

#### Returns
A human-readable string of the form "FiveTuple[remote:local:transport]".

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `net::Address` | [`_remote`](#_remote-1)  |  |
| `net::Address` | [`_local`](#_local-1)  |  |
| `net::TransportType` | [`_transport`](#_transport)  |  |

---

{#_remote-1}

#### _remote

```cpp
net::Address _remote
```

Defined in src/turn/include/icy/turn/fivetuple.h:126

---

{#_local-1}

#### _local

```cpp
net::Address _local
```

Defined in src/turn/include/icy/turn/fivetuple.h:127

---

{#_transport}

#### _transport

```cpp
net::TransportType _transport
```

Defined in src/turn/include/icy/turn/fivetuple.h:128

{#iallocation}

## IAllocation

```cpp
#include <icy/turn/iallocation.h>
```

```cpp
class IAllocation
```

Defined in src/turn/include/icy/turn/iallocation.h:75

> **Subclassed by:** [`Client`](#client-2), [`ServerAllocation`](#serverallocation)

Base class for TURN relay allocations shared by client and server flows.

All TURN operations revolve around allocations, and all TURN messages are associated with an allocation. An allocation conceptually consists of:

* the relayed transport address;

* the 5-tuple: client's IP address, client's port, server IP address, server port, and transport protocol;

* the authentication information;

* the time-to-expiry;

* a list of permissions;

* a list of channel-to-peer bindings.

The relayed transport address is the transport address allocated by the server for communicating with peers, while the 5-tuple describes the communication path between the client and the server. On the client, the 5-tuple uses the client's host transport address; on the server, the 5-tuple uses the client's server-reflexive transport address.

Both the relayed transport address and the 5-tuple must be unique across all allocations, so either one can be used to uniquely identify the allocation.

The authentication information, such as username, password, realm, and nonce, is used to verify subsequent requests and compute the message integrity of responses. The username, realm, and nonce values are initially those used in the authenticated Allocate request that creates the allocation, though the server can change the nonce during the allocation lifetime using a 438 (Stale Nonce) reply. Rather than storing the password explicitly, a server may prefer to store the key value, which is an MD5 hash over the username, realm, and password as described by RFC 5389.

The time-to-expiry is the time left until the allocation expires. Each Allocate or Refresh transaction resets this timer, which then ticks down toward zero. By default, each Allocate or Refresh resets the lifetime to 600 seconds (10 minutes), but the client can request a different value in the Allocate and Refresh requests. Allocations can only be refreshed using Refresh; sending data to a peer does not refresh an allocation. When an allocation expires, the state data associated with it can be freed.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`operator<<`](#operator-28) | `friend` | Declared here |
| [`IAllocation`](#iallocation-1) | `function` | Declared here |
| [`IAllocation`](#iallocation-2) | `function` | Declared here |
| [`IAllocation`](#iallocation-3) | `function` | Declared here |
| [`updateUsage`](#updateusage) | `function` | Declared here |
| [`setLifetime`](#setlifetime) | `function` | Declared here |
| [`setBandwidthLimit`](#setbandwidthlimit) | `function` | Declared here |
| [`expired`](#expired) | `function` | Declared here |
| [`deleted`](#deleted-1) | `function` | Declared here |
| [`bandwidthLimit`](#bandwidthlimit) | `function` | Declared here |
| [`bandwidthUsed`](#bandwidthused) | `function` | Declared here |
| [`bandwidthRemaining`](#bandwidthremaining) | `function` | Declared here |
| [`timeRemaining`](#timeremaining) | `function` | Declared here |
| [`tuple`](#tuple) | `function` | Declared here |
| [`username`](#username-3) | `function` | Declared here |
| [`lifetime`](#lifetime-1) | `function` | Declared here |
| [`permissions`](#permissions) | `function` | Declared here |
| [`relayedAddress`](#relayedaddress-1) | `function` | Declared here |
| [`addPermission`](#addpermission-3) | `function` | Declared here |
| [`addPermission`](#addpermission-4) | `function` | Declared here |
| [`addPermissions`](#addpermissions) | `function` | Declared here |
| [`removePermission`](#removepermission-2) | `function` | Declared here |
| [`removePermission`](#removepermission-3) | `function` | Declared here |
| [`removeAllPermissions`](#removeallpermissions) | `function` | Declared here |
| [`removeExpiredPermissions`](#removeexpiredpermissions) | `function` | Declared here |
| [`hasPermission`](#haspermission-2) | `function` | Declared here |
| [`hasPermission`](#haspermission-3) | `function` | Declared here |
| [`print`](#print-11) | `function` | Declared here |
| [`_tuple`](#_tuple) | `variable` | Declared here |
| [`_username`](#_username-2) | `variable` | Declared here |
| [`_permissions`](#_permissions) | `variable` | Declared here |
| [`_lifetime`](#_lifetime) | `variable` | Declared here |
| [`_bandwidthLimit`](#_bandwidthlimit) | `variable` | Declared here |
| [`_bandwidthUsed`](#_bandwidthused) | `variable` | Declared here |
| [`_createdAt`](#_createdat) | `variable` | Declared here |
| [`_updatedAt`](#_updatedat) | `variable` | Declared here |
| [`_deleted`](#_deleted) | `variable` | Declared here |

### Friends

| Name | Description |
|------|-------------|
| [`operator<<`](#operator-28) `inline` |  |

---

{#operator-28}

#### operator<<

`inline`

```cpp
friend inline std::ostream & operator<<(std::ostream & stream, const IAllocation & alloc)
```

Defined in src/turn/include/icy/turn/iallocation.h:187

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`IAllocation`](#iallocation-1)  |  |
|  | [`IAllocation`](#iallocation-2)  | Deleted constructor. |
|  | [`IAllocation`](#iallocation-3)  | Deleted constructor. |
| `void` | [`updateUsage`](#updateusage) `virtual` | Updates the last-activity timestamp and accumulates bandwidth usage. Call this whenever data is relayed through the allocation. |
| `void` | [`setLifetime`](#setlifetime) `virtual` | Sets the allocation lifetime in seconds and resets the activity timestamp, effectively extending the expiry from the current moment. |
| `void` | [`setBandwidthLimit`](#setbandwidthlimit) `virtual` | Sets the maximum number of bytes this allocation may relay in its lifetime. Pass 0 to disable bandwidth limiting. |
| `bool` | [`expired`](#expired) `virtual` `const` `nodiscard` |  |
| `bool` | [`deleted`](#deleted-1) `virtual` `const` `nodiscard` | Returns true if the allocation's deleted flag is set and or if the allocation has expired. |
| `std::int64_t` | [`bandwidthLimit`](#bandwidthlimit) `virtual` `const` `nodiscard` |  |
| `std::int64_t` | [`bandwidthUsed`](#bandwidthused) `virtual` `const` `nodiscard` |  |
| `std::int64_t` | [`bandwidthRemaining`](#bandwidthremaining) `virtual` `const` `nodiscard` |  |
| `std::int64_t` | [`timeRemaining`](#timeremaining) `virtual` `const` `nodiscard` |  |
| `FiveTuple &` | [`tuple`](#tuple) `virtual` |  |
| `std::string` | [`username`](#username-3) `virtual` `const` `nodiscard` |  |
| `std::int64_t` | [`lifetime`](#lifetime-1) `virtual` `const` `nodiscard` |  |
| `PermissionList` | [`permissions`](#permissions) `virtual` `const` `nodiscard` |  |
| `net::Address` | [`relayedAddress`](#relayedaddress-1) `virtual` `const` `nodiscard` |  |
| `void` | [`addPermission`](#addpermission-3) `virtual` | Adds a permission for `ip`, or refreshes the existing one. |
| `void` | [`addPermission`](#addpermission-4) `virtual` | Adds a permission for `address`, or refreshes the existing one. The port is ignored; TURN permissions are IP-only. |
| `void` | [`addPermissions`](#addpermissions) `virtual` | Adds (or refreshes) permissions for multiple IPs. |
| `void` | [`removePermission`](#removepermission-2) `virtual` | Removes the permission for `ip` if present. |
| `void` | [`removePermission`](#removepermission-3) `virtual` | Removes the permission for `address` if present. The port is ignored; TURN permissions are IP-only. |
| `void` | [`removeAllPermissions`](#removeallpermissions) `virtual` | Removes all permissions from the list. |
| `void` | [`removeExpiredPermissions`](#removeexpiredpermissions) `virtual` | Removes any permissions whose 5-minute lifetime has elapsed. |
| `bool` | [`hasPermission`](#haspermission-2) `virtual` `nodiscard` | Checks whether a permission exists for `peerIP`. Local addresses (192.168.x.x and 127.x.x.x) are always permitted. |
| `bool` | [`hasPermission`](#haspermission-3) `virtual` `nodiscard` | Checks whether a permission exists for `peerAddress`. The port is ignored; TURN permissions are IP-only. |
| `void` | [`print`](#print-11) `virtual` `const` `inline` |  |

---

{#iallocation-1}

#### IAllocation

```cpp
IAllocation(const FiveTuple & tuple = FiveTuple(), const std::string & username = "", std::int64_t lifetime = kDefaultAllocationLifetime)
```

Defined in src/turn/include/icy/turn/iallocation.h:81

#### Parameters
* `tuple` The 5-tuple identifying this allocation. 

* `username` Authenticated username associated with this allocation. 

* `lifetime` Initial lifetime in seconds.

---

{#iallocation-2}

#### IAllocation

```cpp
IAllocation(const IAllocation &) = delete
```

Defined in src/turn/include/icy/turn/iallocation.h:86

Deleted constructor.

---

{#iallocation-3}

#### IAllocation

```cpp
IAllocation(IAllocation &&) = delete
```

Defined in src/turn/include/icy/turn/iallocation.h:88

Deleted constructor.

---

{#updateusage}

#### updateUsage

`virtual`

```cpp
virtual void updateUsage(std::int64_t numBytes = 0)
```

Defined in src/turn/include/icy/turn/iallocation.h:94

Updates the last-activity timestamp and accumulates bandwidth usage. Call this whenever data is relayed through the allocation. 
#### Parameters
* `numBytes` Number of bytes relayed (0 just refreshes the timestamp).

---

{#setlifetime}

#### setLifetime

`virtual`

```cpp
virtual void setLifetime(std::int64_t lifetime)
```

Defined in src/turn/include/icy/turn/iallocation.h:99

Sets the allocation lifetime in seconds and resets the activity timestamp, effectively extending the expiry from the current moment. 
#### Parameters
* `lifetime` New lifetime in seconds.

---

{#setbandwidthlimit}

#### setBandwidthLimit

`virtual`

```cpp
virtual void setBandwidthLimit(std::int64_t numBytes)
```

Defined in src/turn/include/icy/turn/iallocation.h:104

Sets the maximum number of bytes this allocation may relay in its lifetime. Pass 0 to disable bandwidth limiting. 
#### Parameters
* `numBytes` Bandwidth cap in bytes (0 = unlimited).

---

{#expired}

#### expired

`virtual` `const` `nodiscard`

```cpp
[[nodiscard]] virtual bool expired() const
```

Defined in src/turn/include/icy/turn/iallocation.h:108

#### Returns
true if the allocation's lifetime has elapsed or the bandwidth quota has been exhausted.

---

{#deleted-1}

#### deleted

`virtual` `const` `nodiscard`

```cpp
[[nodiscard]] virtual bool deleted() const
```

Defined in src/turn/include/icy/turn/iallocation.h:116

Returns true if the allocation's deleted flag is set and or if the allocation has expired.

This signifies that the allocation is ready to be destroyed via async garbage collection. See [Server::onTimer()](#ontimer-3) and [Client::onTimer()](#ontimer-1)

---

{#bandwidthlimit}

#### bandwidthLimit

`virtual` `const` `nodiscard`

```cpp
[[nodiscard]] virtual std::int64_t bandwidthLimit() const
```

Defined in src/turn/include/icy/turn/iallocation.h:119

#### Returns
The configured bandwidth limit in bytes (0 means unlimited).

---

{#bandwidthused}

#### bandwidthUsed

`virtual` `const` `nodiscard`

```cpp
[[nodiscard]] virtual std::int64_t bandwidthUsed() const
```

Defined in src/turn/include/icy/turn/iallocation.h:122

#### Returns
Total bytes transferred through this allocation since creation.

---

{#bandwidthremaining}

#### bandwidthRemaining

`virtual` `const` `nodiscard`

```cpp
[[nodiscard]] virtual std::int64_t bandwidthRemaining() const
```

Defined in src/turn/include/icy/turn/iallocation.h:126

#### Returns
Bytes remaining before the bandwidth quota is exhausted. Returns a large sentinel value when no limit is configured.

---

{#timeremaining}

#### timeRemaining

`virtual` `const` `nodiscard`

```cpp
[[nodiscard]] virtual std::int64_t timeRemaining() const
```

Defined in src/turn/include/icy/turn/iallocation.h:129

#### Returns
Seconds until the allocation expires (0 if already expired).

##### Reimplemented by

- [`timeRemaining`](#timeremaining-1)

---

{#tuple}

#### tuple

`virtual`

```cpp
virtual FiveTuple & tuple()
```

Defined in src/turn/include/icy/turn/iallocation.h:132

#### Returns
Reference to the 5-tuple identifying this allocation.

---

{#username-3}

#### username

`virtual` `const` `nodiscard`

```cpp
[[nodiscard]] virtual std::string username() const
```

Defined in src/turn/include/icy/turn/iallocation.h:135

#### Returns
The username associated with this allocation.

---

{#lifetime-1}

#### lifetime

`virtual` `const` `nodiscard`

```cpp
[[nodiscard]] virtual std::int64_t lifetime() const
```

Defined in src/turn/include/icy/turn/iallocation.h:138

#### Returns
The configured lifetime in seconds.

---

{#permissions}

#### permissions

`virtual` `const` `nodiscard`

```cpp
[[nodiscard]] virtual PermissionList permissions() const
```

Defined in src/turn/include/icy/turn/iallocation.h:141

#### Returns
A copy of the current permission list.

---

{#relayedaddress-1}

#### relayedAddress

`virtual` `const` `nodiscard`

```cpp
[[nodiscard]] virtual net::Address relayedAddress() const
```

Defined in src/turn/include/icy/turn/iallocation.h:144

#### Returns
The relay transport address assigned to this allocation.

##### Reimplemented by

- [`relayedAddress`](#relayedaddress)
- [`relayedAddress`](#relayedaddress-2)
- [`relayedAddress`](#relayedaddress-3)

---

{#addpermission-3}

#### addPermission

`virtual`

```cpp
virtual void addPermission(const std::string & ip)
```

Defined in src/turn/include/icy/turn/iallocation.h:148

Adds a permission for `ip`, or refreshes the existing one. 
#### Parameters
* `ip` IPv4 address string to permit.

##### Reimplemented by

- [`addPermission`](#addpermission-1)

---

{#addpermission-4}

#### addPermission

`virtual`

```cpp
virtual void addPermission(const net::Address & address)
```

Defined in src/turn/include/icy/turn/iallocation.h:152

Adds a permission for `address`, or refreshes the existing one. The port is ignored; TURN permissions are IP-only.

##### Reimplemented by

- [`addPermission`](#addpermission-2)

---

{#addpermissions}

#### addPermissions

`virtual`

```cpp
virtual void addPermissions(const IPList & ips)
```

Defined in src/turn/include/icy/turn/iallocation.h:156

Adds (or refreshes) permissions for multiple IPs. 
#### Parameters
* `ips` List of IPv4 address strings.

---

{#removepermission-2}

#### removePermission

`virtual`

```cpp
virtual void removePermission(const std::string & ip)
```

Defined in src/turn/include/icy/turn/iallocation.h:160

Removes the permission for `ip` if present. 
#### Parameters
* `ip` IPv4 address string to remove.

##### Reimplemented by

- [`removePermission`](#removepermission)

---

{#removepermission-3}

#### removePermission

`virtual`

```cpp
virtual void removePermission(const net::Address & address)
```

Defined in src/turn/include/icy/turn/iallocation.h:164

Removes the permission for `address` if present. The port is ignored; TURN permissions are IP-only.

##### Reimplemented by

- [`removePermission`](#removepermission-1)

---

{#removeallpermissions}

#### removeAllPermissions

`virtual`

```cpp
virtual void removeAllPermissions()
```

Defined in src/turn/include/icy/turn/iallocation.h:167

Removes all permissions from the list.

---

{#removeexpiredpermissions}

#### removeExpiredPermissions

`virtual`

```cpp
virtual void removeExpiredPermissions()
```

Defined in src/turn/include/icy/turn/iallocation.h:170

Removes any permissions whose 5-minute lifetime has elapsed.

---

{#haspermission-2}

#### hasPermission

`virtual` `nodiscard`

```cpp
[[nodiscard]] virtual bool hasPermission(const std::string & peerIP)
```

Defined in src/turn/include/icy/turn/iallocation.h:176

Checks whether a permission exists for `peerIP`. Local addresses (192.168.x.x and 127.x.x.x) are always permitted. 
#### Parameters
* `peerIP` IPv4 address string to check. 

#### Returns
true if a valid (non-expired) permission exists.

##### Reimplemented by

- [`hasPermission`](#haspermission)
- [`hasPermission`](#haspermission-4)

---

{#haspermission-3}

#### hasPermission

`virtual` `nodiscard`

```cpp
[[nodiscard]] virtual bool hasPermission(const net::Address & peerAddress)
```

Defined in src/turn/include/icy/turn/iallocation.h:180

Checks whether a permission exists for `peerAddress`. The port is ignored; TURN permissions are IP-only.

##### Reimplemented by

- [`hasPermission`](#haspermission-1)
- [`hasPermission`](#haspermission-5)

---

{#print-11}

#### print

`virtual` `const` `inline`

```cpp
virtual inline void print(std::ostream & os) const
```

Defined in src/turn/include/icy/turn/iallocation.h:182

##### Reimplemented by

- [`print`](#print-12)

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `FiveTuple` | [`_tuple`](#_tuple)  |  |
| `std::string` | [`_username`](#_username-2)  |  |
| `PermissionList` | [`_permissions`](#_permissions)  |  |
| `std::int64_t` | [`_lifetime`](#_lifetime)  |  |
| `std::int64_t` | [`_bandwidthLimit`](#_bandwidthlimit)  |  |
| `std::int64_t` | [`_bandwidthUsed`](#_bandwidthused)  |  |
| `time_t` | [`_createdAt`](#_createdat)  |  |
| `time_t` | [`_updatedAt`](#_updatedat)  |  |
| `bool` | [`_deleted`](#_deleted)  |  |

---

{#_tuple}

#### _tuple

```cpp
FiveTuple _tuple
```

Defined in src/turn/include/icy/turn/iallocation.h:195

---

{#_username-2}

#### _username

```cpp
std::string _username
```

Defined in src/turn/include/icy/turn/iallocation.h:196

---

{#_permissions}

#### _permissions

```cpp
PermissionList _permissions
```

Defined in src/turn/include/icy/turn/iallocation.h:197

---

{#_lifetime}

#### _lifetime

```cpp
std::int64_t _lifetime
```

Defined in src/turn/include/icy/turn/iallocation.h:198

---

{#_bandwidthlimit}

#### _bandwidthLimit

```cpp
std::int64_t _bandwidthLimit
```

Defined in src/turn/include/icy/turn/iallocation.h:199

---

{#_bandwidthused}

#### _bandwidthUsed

```cpp
std::int64_t _bandwidthUsed
```

Defined in src/turn/include/icy/turn/iallocation.h:200

---

{#_createdat}

#### _createdAt

```cpp
time_t _createdAt
```

Defined in src/turn/include/icy/turn/iallocation.h:201

---

{#_updatedat}

#### _updatedAt

```cpp
time_t _updatedAt
```

Defined in src/turn/include/icy/turn/iallocation.h:202

---

{#_deleted}

#### _deleted

```cpp
bool _deleted
```

Defined in src/turn/include/icy/turn/iallocation.h:203

{#permissionpolicy}

## PermissionPolicy

```cpp
#include <icy/turn/server/permissionpolicy.h>
```

```cpp
class PermissionPolicy
```

Defined in src/turn/include/icy/turn/server/permissionpolicy.h:25

Lightweight server-side TURN permission policy.

Keeps the explicit permission list fast and binary, while making server exceptions such as local-IP auto-grants explicit and auditable.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`PermissionPolicy`](#permissionpolicy-1) | `function` | Declared here |
| [`allowsExplicit`](#allowsexplicit) | `function` | Declared here |
| [`allowsImplicit`](#allowsimplicit) | `function` | Declared here |
| [`allows`](#allows) | `function` | Declared here |
| [`_enableLocalIPPermissions`](#_enablelocalippermissions) | `variable` | Declared here |
| [`isLocalAutoGrantAddress`](#islocalautograntaddress) | `function` | Declared here |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`PermissionPolicy`](#permissionpolicy-1) `inline` `explicit` `noexcept` |  |
| `bool` | [`allowsExplicit`](#allowsexplicit) `const` `nodiscard` `noexcept` |  |
| `bool` | [`allowsImplicit`](#allowsimplicit) `const` `nodiscard` `noexcept` |  |
| `bool` | [`allows`](#allows) `const` `nodiscard` `noexcept` |  |

---

{#permissionpolicy-1}

#### PermissionPolicy

`inline` `explicit` `noexcept`

```cpp
inline explicit PermissionPolicy(bool enableLocalIPPermissions = false) noexcept
```

Defined in src/turn/include/icy/turn/server/permissionpolicy.h:28

---

{#allowsexplicit}

#### allowsExplicit

`const` `nodiscard` `noexcept`

```cpp
[[nodiscard]] bool allowsExplicit(const PermissionList & permissions, const Permission::Key & peerKey) const noexcept
```

Defined in src/turn/include/icy/turn/server/permissionpolicy.h:33

---

{#allowsimplicit}

#### allowsImplicit

`const` `nodiscard` `noexcept`

```cpp
[[nodiscard]] bool allowsImplicit(const Permission::Key & peerKey) const noexcept
```

Defined in src/turn/include/icy/turn/server/permissionpolicy.h:35

---

{#allows}

#### allows

`const` `nodiscard` `noexcept`

```cpp
[[nodiscard]] bool allows(const PermissionList & permissions, const Permission::Key & peerKey) const noexcept
```

Defined in src/turn/include/icy/turn/server/permissionpolicy.h:36

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `bool` | [`_enableLocalIPPermissions`](#_enablelocalippermissions)  |  |

---

{#_enablelocalippermissions}

#### _enableLocalIPPermissions

```cpp
bool _enableLocalIPPermissions = false
```

Defined in src/turn/include/icy/turn/server/permissionpolicy.h:42

### Private Static Methods

| Return | Name | Description |
|--------|------|-------------|
| `bool` | [`isLocalAutoGrantAddress`](#islocalautograntaddress) `static` `nodiscard` `noexcept` |  |

---

{#islocalautograntaddress}

#### isLocalAutoGrantAddress

`static` `nodiscard` `noexcept`

```cpp
[[nodiscard]] static bool isLocalAutoGrantAddress(const Permission::Key & peerKey) noexcept
```

Defined in src/turn/include/icy/turn/server/permissionpolicy.h:40

{#request-12}

## Request

```cpp
#include <icy/turn/types.h>
```

```cpp
class Request
```

Defined in src/turn/include/icy/turn/types.h:61

> **Inherits:** [`Message`](stun.md#message-5)

A STUN message annotated with the transport type and source/destination addresses needed for server-side routing and response generation.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`transport`](#transport-6) | `variable` | Declared here |
| [`localAddress`](#localaddress) | `variable` | Declared here |
| [`remoteAddress`](#remoteaddress) | `variable` | Declared here |
| [`hash`](#hash) | `variable` | Declared here |
| [`Request`](#request-13) | `function` | Declared here |
| [`Message`](stun.md#message-6) | `function` | Inherited from [`Message`](stun.md#message-5) |
| [`Message`](stun.md#message-7) | `function` | Inherited from [`Message`](stun.md#message-5) |
| [`Message`](stun.md#message-8) | `function` | Inherited from [`Message`](stun.md#message-5) |
| [`Message`](stun.md#message-9) | `function` | Inherited from [`Message`](stun.md#message-5) |
| [`operator=`](stun.md#operator-30) | `function` | Inherited from [`Message`](stun.md#message-5) |
| [`operator=`](stun.md#operator-31) | `function` | Inherited from [`Message`](stun.md#message-5) |
| [`clone`](stun.md#clone-11) | `function` | Inherited from [`Message`](stun.md#message-5) |
| [`setClass`](stun.md#setclass) | `function` | Inherited from [`Message`](stun.md#message-5) |
| [`setMethod`](stun.md#setmethod-1) | `function` | Inherited from [`Message`](stun.md#message-5) |
| [`setTransactionID`](stun.md#settransactionid) | `function` | Inherited from [`Message`](stun.md#message-5) |
| [`classType`](stun.md#classtype) | `function` | Inherited from [`Message`](stun.md#message-5) |
| [`methodType`](stun.md#methodtype) | `function` | Inherited from [`Message`](stun.md#message-5) |
| [`transactionID`](stun.md#transactionid-2) | `function` | Inherited from [`Message`](stun.md#message-5) |
| [`size`](stun.md#size-6) | `function` | Inherited from [`Message`](stun.md#message-5) |
| [`methodString`](stun.md#methodstring) | `function` | Inherited from [`Message`](stun.md#message-5) |
| [`classString`](stun.md#classstring) | `function` | Inherited from [`Message`](stun.md#message-5) |
| [`errorString`](stun.md#errorstring) | `function` | Inherited from [`Message`](stun.md#message-5) |
| [`add`](stun.md#add-1) | `function` | Inherited from [`Message`](stun.md#message-5) |
| [`add`](stun.md#add-2) | `function` | Inherited from [`Message`](stun.md#message-5) |
| [`get`](stun.md#get-5) | `function` | Inherited from [`Message`](stun.md#message-5) |
| [`get`](stun.md#get-6) | `function` | Inherited from [`Message`](stun.md#message-5) |
| [`read`](stun.md#read-6) | `function` | Inherited from [`Message`](stun.md#message-5) |
| [`write`](stun.md#write-21) | `function` | Inherited from [`Message`](stun.md#message-5) |
| [`toString`](stun.md#tostring-8) | `function` | Inherited from [`Message`](stun.md#message-5) |
| [`print`](stun.md#print-13) | `function` | Inherited from [`Message`](stun.md#message-5) |
| [`className`](stun.md#classname-7) | `function` | Inherited from [`Message`](stun.md#message-5) |
| [`_class`](stun.md#_class-1) | `variable` | Inherited from [`Message`](stun.md#message-5) |
| [`_method`](stun.md#_method-1) | `variable` | Inherited from [`Message`](stun.md#message-5) |
| [`_size`](stun.md#_size-1) | `variable` | Inherited from [`Message`](stun.md#message-5) |
| [`_transactionID`](stun.md#_transactionid) | `variable` | Inherited from [`Message`](stun.md#message-5) |
| [`_attrs`](stun.md#_attrs) | `variable` | Inherited from [`Message`](stun.md#message-5) |
| [`MethodType`](stun.md#methodtype-1) | `enum` | Inherited from [`Message`](stun.md#message-5) |
| [`ClassType`](stun.md#classtype-1) | `enum` | Inherited from [`Message`](stun.md#message-5) |
| [`ErrorCodes`](stun.md#errorcodes) | `enum` | Inherited from [`Message`](stun.md#message-5) |
| [`computeBodySize`](stun.md#computebodysize) | `function` | Inherited from [`Message`](stun.md#message-5) |
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

### Inherited from [`Message`](stun.md#message-5)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`Message`](stun.md#message-6)  | Constructs a default message (Request class, Undefined method) with a randomly generated 12-byte transaction ID. |
| `function` | [`Message`](stun.md#message-7)  | Constructs a message with explicit class and method. |
| `function` | [`Message`](stun.md#message-8)  | Deep-copy constructor; clones all attributes. |
| `function` | [`Message`](stun.md#message-9) `noexcept` | Move constructor. |
| `function` | [`operator=`](stun.md#operator-30)  | Deep-copy assignment; clones all attributes from `that`. |
| `function` | [`operator=`](stun.md#operator-31) `noexcept` | Move assignment. |
| `function` | [`clone`](stun.md#clone-11) `virtual` `const` `override` |  |
| `function` | [`setClass`](stun.md#setclass)  | Sets the message class field. |
| `function` | [`setMethod`](stun.md#setmethod-1)  | Sets the message method field. |
| `function` | [`setTransactionID`](stun.md#settransactionid)  | Sets the 12-byte transaction ID. |
| `function` | [`classType`](stun.md#classtype) `const` `nodiscard` |  |
| `function` | [`methodType`](stun.md#methodtype) `const` `nodiscard` |  |
| `function` | [`transactionID`](stun.md#transactionid-2) `const` `inline` `nodiscard` |  |
| `function` | [`size`](stun.md#size-6) `virtual` `const` `inline` `nodiscard` `override` |  |
| `function` | [`methodString`](stun.md#methodstring) `const` `nodiscard` |  |
| `function` | [`classString`](stun.md#classstring) `const` `nodiscard` |  |
| `function` | [`errorString`](stun.md#errorstring) `const` `nodiscard` | Maps a numeric error code to its canonical string description. |
| `function` | [`add`](stun.md#add-1) `inline` | Constructs an attribute of type T in-place and appends it to the message. Returns a reference to the new attribute for further configuration. |
| `function` | [`add`](stun.md#add-2)  | Appends an attribute to the message, taking ownership via unique_ptr. |
| `function` | [`get`](stun.md#get-5) `const` `nodiscard` | Returns the Nth attribute of the given type, or nullptr if not found. |
| `function` | [`get`](stun.md#get-6) `const` `inline` `nodiscard` | Type-safe attribute accessor using the concrete attribute's TypeID. |
| `function` | [`read`](stun.md#read-6) `virtual` `override` | Parses a STUN/TURN packet from the given buffer. |
| `function` | [`write`](stun.md#write-21) `virtual` `const` `override` | Serialises this message into a STUN/TURN wire-format packet. |
| `function` | [`toString`](stun.md#tostring-8) `const` `nodiscard` |  |
| `function` | [`print`](stun.md#print-13) `virtual` `const` `override` | Writes the same representation as [toString()](stun.md#tostring-8) to the given stream. |
| `function` | [`className`](stun.md#classname-7) `virtual` `const` `inline` `override` | Returns the class name of this packet type for logging and diagnostics. |
| `variable` | [`_class`](stun.md#_class-1)  |  |
| `variable` | [`_method`](stun.md#_method-1)  |  |
| `variable` | [`_size`](stun.md#_size-1)  | Set by [read()](stun.md#read-6); [write()](stun.md#write-21) uses computeBodySize() instead. |
| `variable` | [`_transactionID`](stun.md#_transactionid)  |  |
| `variable` | [`_attrs`](stun.md#_attrs)  |  |
| `enum` | [`MethodType`](stun.md#methodtype-1)  |  |
| `enum` | [`ClassType`](stun.md#classtype-1)  |  |
| `enum` | [`ErrorCodes`](stun.md#errorcodes)  |  |
| `function` | [`computeBodySize`](stun.md#computebodysize) `const` `nodiscard` | Computes the wire body size from the current attribute list. |

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
| `net::TransportType` | [`transport`](#transport-6)  | Protocol on which the request arrived (UDP or TCP). |
| `net::Address` | [`localAddress`](#localaddress)  | [Server](#server-6)'s local address that received the request. |
| `net::Address` | [`remoteAddress`](#remoteaddress)  | [Client](#client-2)'s remote address (used to build the 5-tuple). |
| `std::string` | [`hash`](#hash)  | Pre-computed MessageIntegrity key for signing responses. |

---

{#transport-6}

#### transport

```cpp
net::TransportType transport
```

Defined in src/turn/include/icy/turn/types.h:64

Protocol on which the request arrived (UDP or TCP).

---

{#localaddress}

#### localAddress

```cpp
net::Address localAddress
```

Defined in src/turn/include/icy/turn/types.h:65

[Server](#server-6)'s local address that received the request.

---

{#remoteaddress}

#### remoteAddress

```cpp
net::Address remoteAddress
```

Defined in src/turn/include/icy/turn/types.h:66

[Client](#client-2)'s remote address (used to build the 5-tuple).

---

{#hash}

#### hash

```cpp
std::string hash
```

Defined in src/turn/include/icy/turn/types.h:67

Pre-computed MessageIntegrity key for signing responses.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Request`](#request-13) `inline` | Constructs a [Request](#request-12) by copying a parsed STUN message and annotating it with the transport context. |

---

{#request-13}

#### Request

`inline`

```cpp
inline Request(const stun::Message & message, net::TransportType transport, const net::Address & localAddress = net::Address(), const net::Address & remoteAddress = net::Address())
```

Defined in src/turn/include/icy/turn/types.h:75

Constructs a [Request](#request-12) by copying a parsed STUN message and annotating it with the transport context. 
#### Parameters
* `message` Parsed STUN message. 

* `transport` Transport protocol on which the message arrived. 

* `localAddress` Server-side local address. 

* `remoteAddress` Client-side remote address.

{#server-6}

## Server

```cpp
#include <icy/turn/server/server.h>
```

```cpp
class Server
```

Defined in src/turn/include/icy/turn/server/server.h:126

TURN server RFC 5766 / RFC 6062 implementation. Listens on UDP and/or TCP, authenticates requests via [ServerObserver](#serverobserver), and manages [ServerAllocation](#serverallocation) objects for each 5-tuple.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`Server`](#server-7) | `function` | Declared here |
| [`start`](#start-14) | `function` | Declared here |
| [`stop`](#stop-11) | `function` | Declared here |
| [`handleRequest`](#handlerequest) | `function` | Declared here |
| [`handleAuthorizedRequest`](#handleauthorizedrequest) | `function` | Declared here |
| [`handleBindingRequest`](#handlebindingrequest) | `function` | Declared here |
| [`handleAllocateRequest`](#handleallocaterequest) | `function` | Declared here |
| [`handleConnectionBindRequest`](#handleconnectionbindrequest) | `function` | Declared here |
| [`respond`](#respond) | `function` | Declared here |
| [`respondError`](#responderror) | `function` | Declared here |
| [`allocations`](#allocations) | `function` | Declared here |
| [`addAllocation`](#addallocation) | `function` | Declared here |
| [`removeAllocation`](#removeallocation) | `function` | Declared here |
| [`getAllocation`](#getallocation) | `function` | Declared here |
| [`getTCPAllocation`](#gettcpallocation) | `function` | Declared here |
| [`getTCPSocket`](#gettcpsocket) | `function` | Declared here |
| [`releaseTCPSocket`](#releasetcpsocket) | `function` | Declared here |
| [`observer`](#observer-2) | `function` | Declared here |
| [`options`](#options-11) | `function` | Declared here |
| [`udpSocket`](#udpsocket-4) | `function` | Declared here |
| [`tcpSocket`](#tcpsocket-4) | `function` | Declared here |
| [`timer`](#timer-1) | `function` | Declared here |
| [`onTCPAcceptConnection`](#ontcpacceptconnection) | `function` | Declared here |
| [`onTCPSocketClosed`](#ontcpsocketclosed) | `function` | Declared here |
| [`onSocketRecv`](#onsocketrecv-8) | `function` | Declared here |
| [`onTimer`](#ontimer-3) | `function` | Declared here |
| [`scheduleDeferredTCPSocketRelease`](#scheduledeferredtcpsocketrelease) | `function` | Declared here |
| [`drainReleasedTCPSockets`](#drainreleasedtcpsockets) | `function` | Declared here |
| [`_mutex`](#_mutex-13) | `variable` | Declared here |
| [`_observer`](#_observer-2) | `variable` | Declared here |
| [`_options`](#_options-4) | `variable` | Declared here |
| [`_udpSocket`](#_udpsocket) | `variable` | Declared here |
| [`_tcpSocket`](#_tcpsocket) | `variable` | Declared here |
| [`_tcpSockets`](#_tcpsockets) | `variable` | Declared here |
| [`_pendingReleasedTCPSockets`](#_pendingreleasedtcpsockets) | `variable` | Declared here |
| [`_tcpSocketReleaseScheduled`](#_tcpsocketreleasescheduled) | `variable` | Declared here |
| [`_allocations`](#_allocations) | `variable` | Declared here |
| [`_timer`](#_timer-2) | `variable` | Declared here |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Server`](#server-7)  |  |
| `void` | [`start`](#start-14) `virtual` | Binds and listens on the configured address, then starts the maintenance timer. |
| `void` | [`stop`](#stop-11) `virtual` | Stops the timer, destroys all allocations, and closes server sockets. |
| `void` | [`handleRequest`](#handlerequest)  | Routes an authenticated request to the appropriate handler based on state. Pending (Authenticating) requests are held until the observer calls back. |
| `void` | [`handleAuthorizedRequest`](#handleauthorizedrequest)  | Dispatches an already-authorized request to the specific method handler. |
| `void` | [`handleBindingRequest`](#handlebindingrequest)  | Handles a Binding request; responds with XOR-MAPPED-ADDRESS. |
| `void` | [`handleAllocateRequest`](#handleallocaterequest)  | Handles an Allocate request; creates a UDP or TCP [ServerAllocation](#serverallocation) and sends a success response with XOR-RELAYED-ADDRESS and LIFETIME. |
| `void` | [`handleConnectionBindRequest`](#handleconnectionbindrequest)  | Handles a ConnectionBind request by locating the [TCPAllocation](#tcpallocation) that owns the given CONNECTION-ID and delegating to it. |
| `void` | [`respond`](#respond)  | Sends a STUN response, signing it with MessageIntegrity if the request had a hash. Routes via UDP or TCP depending on request.transport. |
| `void` | [`respondError`](#responderror)  | Constructs and sends an error response with SOFTWARE, REALM, NONCE, and ERROR-CODE. |
| `std::map< FiveTuple, ServerAllocation * >` | [`allocations`](#allocations) `const` | Returns a snapshot copy of the allocation map for safe iteration. Returned raw pointers are valid only while the server holds the allocations. |
| `void` | [`addAllocation`](#addallocation)  | Transfers ownership of `alloc` to the server and notifies the observer. |
| `void` | [`removeAllocation`](#removeallocation)  | Removes `alloc` from the map and notifies the observer. Called automatically from the [ServerAllocation](#serverallocation) destructor. |
| `ServerAllocation *` | [`getAllocation`](#getallocation) `nodiscard` | Looks up an allocation by its 5-tuple. |
| `TCPAllocation *` | [`getTCPAllocation`](#gettcpallocation) `nodiscard` | Finds the [TCPAllocation](#tcpallocation) that owns a [TCPConnectionPair](#tcpconnectionpair) with the given connection ID. |
| `net::TCPSocket::Ptr` | [`getTCPSocket`](#gettcpsocket) `nodiscard` | Returns the accepted TCP socket whose peer address matches `remoteAddr`. |
| `void` | [`releaseTCPSocket`](#releasetcpsocket)  | Removes a TCP control socket from the server's socket list and unregisters callbacks. Called when the socket is handed off to a [TCPAllocation](#tcpallocation) (ConnectionBind). |
| `ServerObserver &` | [`observer`](#observer-2)  |  |
| `const ServerOptions &` | [`options`](#options-11) `const` `nodiscard` |  |
| `net::UDPSocket &` | [`udpSocket`](#udpsocket-4)  |  |
| `net::TCPSocket &` | [`tcpSocket`](#tcpsocket-4)  |  |
| `Timer &` | [`timer`](#timer-1)  |  |
| `void` | [`onTCPAcceptConnection`](#ontcpacceptconnection)  | Accept callback for the TCP listening socket; registers new connections for STUN message processing. |
| `bool` | [`onTCPSocketClosed`](#ontcpsocketclosed)  | Close callback for accepted TCP sockets; removes the socket from the list. |
| `bool` | [`onSocketRecv`](#onsocketrecv-8)  | Receive callback for both UDP and TCP sockets; parses STUN messages and calls [handleRequest()](#handlerequest) for each one. |
| `void` | [`onTimer`](#ontimer-3)  | Periodic maintenance callback; expires and removes stale allocations. |
| `void` | [`scheduleDeferredTCPSocketRelease`](#scheduledeferredtcpsocketrelease)  | Defers accepted TCP socket removal until after the active callback stack unwinds. |
| `void` | [`drainReleasedTCPSockets`](#drainreleasedtcpsockets)  |  |

---

{#server-7}

#### Server

```cpp
Server(ServerObserver & observer, const ServerOptions & options = ServerOptions())
```

Defined in src/turn/include/icy/turn/server/server.h:131

#### Parameters
* `observer` Observer used for authentication and allocation lifecycle events. 

* `options` [Server](#server-6) configuration; defaults to 0.0.0.0:3478 with TCP and UDP enabled.

---

{#start-14}

#### start

`virtual`

```cpp
virtual void start()
```

Defined in src/turn/include/icy/turn/server/server.h:135

Binds and listens on the configured address, then starts the maintenance timer.

---

{#stop-11}

#### stop

`virtual`

```cpp
virtual void stop()
```

Defined in src/turn/include/icy/turn/server/server.h:138

Stops the timer, destroys all allocations, and closes server sockets.

---

{#handlerequest}

#### handleRequest

```cpp
void handleRequest(Request & request, AuthenticationState state)
```

Defined in src/turn/include/icy/turn/server/server.h:144

Routes an authenticated request to the appropriate handler based on state. Pending (Authenticating) requests are held until the observer calls back. 
#### Parameters
* `request` Incoming STUN request. 

* `state` Result of the observer's authenticateRequest() call.

---

{#handleauthorizedrequest}

#### handleAuthorizedRequest

```cpp
void handleAuthorizedRequest(Request & request)
```

Defined in src/turn/include/icy/turn/server/server.h:148

Dispatches an already-authorized request to the specific method handler. 
#### Parameters
* `request` Authorized STUN request.

---

{#handlebindingrequest}

#### handleBindingRequest

```cpp
void handleBindingRequest(Request & request)
```

Defined in src/turn/include/icy/turn/server/server.h:152

Handles a Binding request; responds with XOR-MAPPED-ADDRESS. 
#### Parameters
* `request` Incoming Binding request.

---

{#handleallocaterequest}

#### handleAllocateRequest

```cpp
void handleAllocateRequest(Request & request)
```

Defined in src/turn/include/icy/turn/server/server.h:157

Handles an Allocate request; creates a UDP or TCP [ServerAllocation](#serverallocation) and sends a success response with XOR-RELAYED-ADDRESS and LIFETIME. 
#### Parameters
* `request` Incoming Allocate request.

---

{#handleconnectionbindrequest}

#### handleConnectionBindRequest

```cpp
void handleConnectionBindRequest(Request & request)
```

Defined in src/turn/include/icy/turn/server/server.h:162

Handles a ConnectionBind request by locating the [TCPAllocation](#tcpallocation) that owns the given CONNECTION-ID and delegating to it. 
#### Parameters
* `request` Incoming ConnectionBind request.

---

{#respond}

#### respond

```cpp
void respond(Request & request, stun::Message & response)
```

Defined in src/turn/include/icy/turn/server/server.h:168

Sends a STUN response, signing it with MessageIntegrity if the request had a hash. Routes via UDP or TCP depending on request.transport. 
#### Parameters
* `request` The original request (provides transport and remote address). 

* `response` The response message to send.

---

{#responderror}

#### respondError

```cpp
void respondError(Request & request, int errorCode, const char * errorDesc)
```

Defined in src/turn/include/icy/turn/server/server.h:174

Constructs and sends an error response with SOFTWARE, REALM, NONCE, and ERROR-CODE. 
#### Parameters
* `request` The original request. 

* `errorCode` STUN error code (e.g. 400, 401, 437). 

* `errorDesc` Human-readable error description string.

---

{#allocations}

#### allocations

`const`

```cpp
std::map< FiveTuple, ServerAllocation * > allocations() const
```

Defined in src/turn/include/icy/turn/server/server.h:179

Returns a snapshot copy of the allocation map for safe iteration. Returned raw pointers are valid only while the server holds the allocations. 
#### Returns
Map from [FiveTuple](#fivetuple) to raw [ServerAllocation](#serverallocation) pointers.

---

{#addallocation}

#### addAllocation

```cpp
void addAllocation(std::unique_ptr< ServerAllocation > alloc)
```

Defined in src/turn/include/icy/turn/server/server.h:183

Transfers ownership of `alloc` to the server and notifies the observer. 
#### Parameters
* `alloc` Newly constructed allocation to register.

---

{#removeallocation}

#### removeAllocation

```cpp
void removeAllocation(ServerAllocation * alloc)
```

Defined in src/turn/include/icy/turn/server/server.h:188

Removes `alloc` from the map and notifies the observer. Called automatically from the [ServerAllocation](#serverallocation) destructor. 
#### Parameters
* `alloc` Allocation being destroyed.

---

{#getallocation}

#### getAllocation

`nodiscard`

```cpp
[[nodiscard]] ServerAllocation * getAllocation(const FiveTuple & tuple)
```

Defined in src/turn/include/icy/turn/server/server.h:193

Looks up an allocation by its 5-tuple. 
#### Parameters
* `tuple` The 5-tuple to search for. 

#### Returns
Pointer to the matching allocation, or nullptr if not found.

---

{#gettcpallocation}

#### getTCPAllocation

`nodiscard`

```cpp
[[nodiscard]] TCPAllocation * getTCPAllocation(const uint32_t & connectionID)
```

Defined in src/turn/include/icy/turn/server/server.h:198

Finds the [TCPAllocation](#tcpallocation) that owns a [TCPConnectionPair](#tcpconnectionpair) with the given connection ID. 
#### Parameters
* `connectionID` TURN CONNECTION-ID to search for. 

#### Returns
Pointer to the owning [TCPAllocation](#tcpallocation), or nullptr if not found.

---

{#gettcpsocket}

#### getTCPSocket

`nodiscard`

```cpp
[[nodiscard]] net::TCPSocket::Ptr getTCPSocket(const net::Address & remoteAddr)
```

Defined in src/turn/include/icy/turn/server/server.h:203

Returns the accepted TCP socket whose peer address matches `remoteAddr`. 
#### Parameters
* `remoteAddr` Peer address to search for. 

#### Returns
Shared pointer to the socket, or empty if not found.

---

{#releasetcpsocket}

#### releaseTCPSocket

```cpp
void releaseTCPSocket(const net::Socket & socket)
```

Defined in src/turn/include/icy/turn/server/server.h:208

Removes a TCP control socket from the server's socket list and unregisters callbacks. Called when the socket is handed off to a [TCPAllocation](#tcpallocation) (ConnectionBind). 
#### Parameters
* `socket` The socket to release.

---

{#observer-2}

#### observer

```cpp
ServerObserver & observer()
```

Defined in src/turn/include/icy/turn/server/server.h:211

#### Returns
Reference to the observer provided at construction.

---

{#options-11}

#### options

`const` `nodiscard`

```cpp
[[nodiscard]] const ServerOptions & options() const
```

Defined in src/turn/include/icy/turn/server/server.h:214

#### Returns
Reference to the immutable options struct.

---

{#udpsocket-4}

#### udpSocket

```cpp
net::UDPSocket & udpSocket()
```

Defined in src/turn/include/icy/turn/server/server.h:217

#### Returns
Reference to the UDP server socket.

---

{#tcpsocket-4}

#### tcpSocket

```cpp
net::TCPSocket & tcpSocket()
```

Defined in src/turn/include/icy/turn/server/server.h:220

#### Returns
Reference to the TCP server listening socket.

---

{#timer-1}

#### timer

```cpp
Timer & timer()
```

Defined in src/turn/include/icy/turn/server/server.h:223

#### Returns
Reference to the maintenance timer.

---

{#ontcpacceptconnection}

#### onTCPAcceptConnection

```cpp
void onTCPAcceptConnection(const net::TCPSocket::Ptr & sock)
```

Defined in src/turn/include/icy/turn/server/server.h:228

Accept callback for the TCP listening socket; registers new connections for STUN message processing. 
#### Parameters
* `sock` Newly accepted TCP socket.

---

{#ontcpsocketclosed}

#### onTCPSocketClosed

```cpp
bool onTCPSocketClosed(net::Socket & socket)
```

Defined in src/turn/include/icy/turn/server/server.h:232

Close callback for accepted TCP sockets; removes the socket from the list. 
#### Parameters
* `socket` The closed socket.

---

{#onsocketrecv-8}

#### onSocketRecv

```cpp
bool onSocketRecv(net::Socket & socket, const MutableBuffer & buffer, const net::Address & peerAddress)
```

Defined in src/turn/include/icy/turn/server/server.h:239

Receive callback for both UDP and TCP sockets; parses STUN messages and calls [handleRequest()](#handlerequest) for each one. 
#### Parameters
* `socket` The receiving socket. 

* `buffer` Received data buffer. 

* `peerAddress` Source address of the data.

---

{#ontimer-3}

#### onTimer

```cpp
void onTimer()
```

Defined in src/turn/include/icy/turn/server/server.h:243

Periodic maintenance callback; expires and removes stale allocations.

---

{#scheduledeferredtcpsocketrelease}

#### scheduleDeferredTCPSocketRelease

```cpp
void scheduleDeferredTCPSocketRelease()
```

Defined in src/turn/include/icy/turn/server/server.h:246

Defers accepted TCP socket removal until after the active callback stack unwinds.

---

{#drainreleasedtcpsockets}

#### drainReleasedTCPSockets

```cpp
void drainReleasedTCPSockets()
```

Defined in src/turn/include/icy/turn/server/server.h:247

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::mutex` | [`_mutex`](#_mutex-13)  |  |
| `ServerObserver &` | [`_observer`](#_observer-2)  |  |
| `ServerOptions` | [`_options`](#_options-4)  |  |
| `net::SocketEmitter` | [`_udpSocket`](#_udpsocket)  |  |
| `net::SocketEmitter` | [`_tcpSocket`](#_tcpsocket)  |  |
| `std::vector< net::SocketEmitter >` | [`_tcpSockets`](#_tcpsockets)  |  |
| `std::unordered_set< const net::Socket * >` | [`_pendingReleasedTCPSockets`](#_pendingreleasedtcpsockets)  |  |
| `bool` | [`_tcpSocketReleaseScheduled`](#_tcpsocketreleasescheduled)  |  |
| `ServerAllocationMap` | [`_allocations`](#_allocations)  |  |
| `Timer` | [`_timer`](#_timer-2)  |  |

---

{#_mutex-13}

#### _mutex

```cpp
std::mutex _mutex
```

Defined in src/turn/include/icy/turn/server/server.h:250

---

{#_observer-2}

#### _observer

```cpp
ServerObserver & _observer
```

Defined in src/turn/include/icy/turn/server/server.h:251

---

{#_options-4}

#### _options

```cpp
ServerOptions _options
```

Defined in src/turn/include/icy/turn/server/server.h:252

---

{#_udpsocket}

#### _udpSocket

```cpp
net::SocketEmitter _udpSocket
```

Defined in src/turn/include/icy/turn/server/server.h:253

---

{#_tcpsocket}

#### _tcpSocket

```cpp
net::SocketEmitter _tcpSocket
```

Defined in src/turn/include/icy/turn/server/server.h:254

---

{#_tcpsockets}

#### _tcpSockets

```cpp
std::vector< net::SocketEmitter > _tcpSockets
```

Defined in src/turn/include/icy/turn/server/server.h:255

---

{#_pendingreleasedtcpsockets}

#### _pendingReleasedTCPSockets

```cpp
std::unordered_set< const net::Socket * > _pendingReleasedTCPSockets
```

Defined in src/turn/include/icy/turn/server/server.h:256

---

{#_tcpsocketreleasescheduled}

#### _tcpSocketReleaseScheduled

```cpp
bool _tcpSocketReleaseScheduled {false}
```

Defined in src/turn/include/icy/turn/server/server.h:257

---

{#_allocations}

#### _allocations

```cpp
ServerAllocationMap _allocations
```

Defined in src/turn/include/icy/turn/server/server.h:258

---

{#_timer-2}

#### _timer

```cpp
Timer _timer
```

Defined in src/turn/include/icy/turn/server/server.h:259

{#serverallocation}

## ServerAllocation

```cpp
#include <icy/turn/server/serverallocation.h>
```

```cpp
class ServerAllocation
```

Defined in src/turn/include/icy/turn/server/serverallocation.h:29

> **Inherits:** [`IAllocation`](#iallocation)
> **Subclassed by:** [`TCPAllocation`](#tcpallocation), [`UDPAllocation`](#udpallocation)

Server-side TURN allocation that tracks permissions, lifetime, and relay state. Handles Refresh and CreatePermission requests common to both UDP and TCP allocations. Subclassed by [UDPAllocation](#udpallocation) and [TCPAllocation](#tcpallocation) for transport-specific behaviour.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`Server`](#server-8) | `friend` | Declared here |
| [`ServerAllocation`](#serverallocation-1) | `function` | Declared here |
| [`handleRequest`](#handlerequest-1) | `function` | Declared here |
| [`handleRefreshRequest`](#handlerefreshrequest) | `function` | Declared here |
| [`handleCreatePermission`](#handlecreatepermission) | `function` | Declared here |
| [`onTimer`](#ontimer-4) | `function` | Declared here |
| [`timeRemaining`](#timeremaining-1) | `function` | Declared here |
| [`maxTimeRemaining`](#maxtimeremaining) | `function` | Declared here |
| [`hasPermission`](#haspermission-4) | `function` | Declared here |
| [`hasPermission`](#haspermission-5) | `function` | Declared here |
| [`server`](#server-9) | `function` | Declared here |
| [`print`](#print-12) | `function` | Declared here |
| [`refreshDeleteRequested`](#refreshdeleterequested) | `function` | Declared here |
| [`_maxLifetime`](#_maxlifetime) | `variable` | Declared here |
| [`_server`](#_server-1) | `variable` | Declared here |
| [`_permissionPolicy`](#_permissionpolicy) | `variable` | Declared here |
| [`_refreshDeleteRequested`](#_refreshdeleterequested) | `variable` | Declared here |
| [`ServerAllocation`](#serverallocation-2) | `function` | Declared here |
| [`ServerAllocation`](#serverallocation-3) | `function` | Declared here |
| [`operator<<`](#operator-28) | `friend` | Inherited from [`IAllocation`](#iallocation) |
| [`IAllocation`](#iallocation-1) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`IAllocation`](#iallocation-2) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`IAllocation`](#iallocation-3) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`updateUsage`](#updateusage) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`setLifetime`](#setlifetime) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`setBandwidthLimit`](#setbandwidthlimit) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`expired`](#expired) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`deleted`](#deleted-1) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`bandwidthLimit`](#bandwidthlimit) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`bandwidthUsed`](#bandwidthused) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`bandwidthRemaining`](#bandwidthremaining) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`timeRemaining`](#timeremaining) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`tuple`](#tuple) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`username`](#username-3) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`lifetime`](#lifetime-1) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`permissions`](#permissions) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`relayedAddress`](#relayedaddress-1) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`addPermission`](#addpermission-3) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`addPermission`](#addpermission-4) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`addPermissions`](#addpermissions) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`removePermission`](#removepermission-2) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`removePermission`](#removepermission-3) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`removeAllPermissions`](#removeallpermissions) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`removeExpiredPermissions`](#removeexpiredpermissions) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`hasPermission`](#haspermission-2) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`hasPermission`](#haspermission-3) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`print`](#print-11) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`_tuple`](#_tuple) | `variable` | Inherited from [`IAllocation`](#iallocation) |
| [`_username`](#_username-2) | `variable` | Inherited from [`IAllocation`](#iallocation) |
| [`_permissions`](#_permissions) | `variable` | Inherited from [`IAllocation`](#iallocation) |
| [`_lifetime`](#_lifetime) | `variable` | Inherited from [`IAllocation`](#iallocation) |
| [`_bandwidthLimit`](#_bandwidthlimit) | `variable` | Inherited from [`IAllocation`](#iallocation) |
| [`_bandwidthUsed`](#_bandwidthused) | `variable` | Inherited from [`IAllocation`](#iallocation) |
| [`_createdAt`](#_createdat) | `variable` | Inherited from [`IAllocation`](#iallocation) |
| [`_updatedAt`](#_updatedat) | `variable` | Inherited from [`IAllocation`](#iallocation) |
| [`_deleted`](#_deleted) | `variable` | Inherited from [`IAllocation`](#iallocation) |

### Inherited from [`IAllocation`](#iallocation)

| Kind | Name | Description |
|------|------|-------------|
| `friend` | [`operator<<`](#operator-28) `inline` |  |
| `function` | [`IAllocation`](#iallocation-1)  |  |
| `function` | [`IAllocation`](#iallocation-2)  | Deleted constructor. |
| `function` | [`IAllocation`](#iallocation-3)  | Deleted constructor. |
| `function` | [`updateUsage`](#updateusage) `virtual` | Updates the last-activity timestamp and accumulates bandwidth usage. Call this whenever data is relayed through the allocation. |
| `function` | [`setLifetime`](#setlifetime) `virtual` | Sets the allocation lifetime in seconds and resets the activity timestamp, effectively extending the expiry from the current moment. |
| `function` | [`setBandwidthLimit`](#setbandwidthlimit) `virtual` | Sets the maximum number of bytes this allocation may relay in its lifetime. Pass 0 to disable bandwidth limiting. |
| `function` | [`expired`](#expired) `virtual` `const` `nodiscard` |  |
| `function` | [`deleted`](#deleted-1) `virtual` `const` `nodiscard` | Returns true if the allocation's deleted flag is set and or if the allocation has expired. |
| `function` | [`bandwidthLimit`](#bandwidthlimit) `virtual` `const` `nodiscard` |  |
| `function` | [`bandwidthUsed`](#bandwidthused) `virtual` `const` `nodiscard` |  |
| `function` | [`bandwidthRemaining`](#bandwidthremaining) `virtual` `const` `nodiscard` |  |
| `function` | [`timeRemaining`](#timeremaining) `virtual` `const` `nodiscard` |  |
| `function` | [`tuple`](#tuple) `virtual` |  |
| `function` | [`username`](#username-3) `virtual` `const` `nodiscard` |  |
| `function` | [`lifetime`](#lifetime-1) `virtual` `const` `nodiscard` |  |
| `function` | [`permissions`](#permissions) `virtual` `const` `nodiscard` |  |
| `function` | [`relayedAddress`](#relayedaddress-1) `virtual` `const` `nodiscard` |  |
| `function` | [`addPermission`](#addpermission-3) `virtual` | Adds a permission for `ip`, or refreshes the existing one. |
| `function` | [`addPermission`](#addpermission-4) `virtual` | Adds a permission for `address`, or refreshes the existing one. The port is ignored; TURN permissions are IP-only. |
| `function` | [`addPermissions`](#addpermissions) `virtual` | Adds (or refreshes) permissions for multiple IPs. |
| `function` | [`removePermission`](#removepermission-2) `virtual` | Removes the permission for `ip` if present. |
| `function` | [`removePermission`](#removepermission-3) `virtual` | Removes the permission for `address` if present. The port is ignored; TURN permissions are IP-only. |
| `function` | [`removeAllPermissions`](#removeallpermissions) `virtual` | Removes all permissions from the list. |
| `function` | [`removeExpiredPermissions`](#removeexpiredpermissions) `virtual` | Removes any permissions whose 5-minute lifetime has elapsed. |
| `function` | [`hasPermission`](#haspermission-2) `virtual` `nodiscard` | Checks whether a permission exists for `peerIP`. Local addresses (192.168.x.x and 127.x.x.x) are always permitted. |
| `function` | [`hasPermission`](#haspermission-3) `virtual` `nodiscard` | Checks whether a permission exists for `peerAddress`. The port is ignored; TURN permissions are IP-only. |
| `function` | [`print`](#print-11) `virtual` `const` `inline` |  |
| `variable` | [`_tuple`](#_tuple)  |  |
| `variable` | [`_username`](#_username-2)  |  |
| `variable` | [`_permissions`](#_permissions)  |  |
| `variable` | [`_lifetime`](#_lifetime)  |  |
| `variable` | [`_bandwidthLimit`](#_bandwidthlimit)  |  |
| `variable` | [`_bandwidthUsed`](#_bandwidthused)  |  |
| `variable` | [`_createdAt`](#_createdat)  |  |
| `variable` | [`_updatedAt`](#_updatedat)  |  |
| `variable` | [`_deleted`](#_deleted)  |  |

### Friends

| Name | Description |
|------|-------------|
| [`Server`](#server-8)  |  |

---

{#server-8}

#### Server

```cpp
friend class Server
```

Defined in src/turn/include/icy/turn/server/serverallocation.h:84

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`ServerAllocation`](#serverallocation-1)  |  |
| `bool` | [`handleRequest`](#handlerequest-1) `virtual` | Dispatches incoming STUN requests to [handleCreatePermission()](#handlecreatepermission) or [handleRefreshRequest()](#handlerefreshrequest). Subclasses override to add transport-specific methods. |
| `void` | [`handleRefreshRequest`](#handlerefreshrequest) `virtual` | Processes a Refresh request; updates the lifetime or sets the delete flag if the requested lifetime is 0, then sends a success response. |
| `void` | [`handleCreatePermission`](#handlecreatepermission) `virtual` | Processes a CreatePermission request; installs or refreshes permissions for each XOR-PEER-ADDRESS attribute, then sends a success response. |
| `bool` | [`onTimer`](#ontimer-4) `nodiscard` | Periodic maintenance callback called by [Server::onTimer()](#ontimer-3). Removes expired permissions. Returns false to signal that this allocation should be destroyed. |
| `std::int64_t` | [`timeRemaining`](#timeremaining-1) `virtual` `const` `nodiscard` `override` |  |
| `std::int64_t` | [`maxTimeRemaining`](#maxtimeremaining) `const` `nodiscard` |  |
| `bool` | [`hasPermission`](#haspermission-4) `virtual` `nodiscard` `override` | Checks permission, auto-granting local IPs if enabled in server options. |
| `bool` | [`hasPermission`](#haspermission-5) `virtual` `nodiscard` `override` | Checks whether a permission exists for `peerAddress`. The port is ignored; TURN permissions are IP-only. |
| `Server &` | [`server`](#server-9) `virtual` |  |
| `void` | [`print`](#print-12) `virtual` `const` `override` | Writes a detailed diagnostic dump of this allocation to `os`. |
| `bool` | [`refreshDeleteRequested`](#refreshdeleterequested) `const` `inline` `nodiscard` | Returns true if the refresh request set lifetime to 0, signaling the parent [Server](#server-6) to delete this allocation. |

---

{#serverallocation-1}

#### ServerAllocation

```cpp
ServerAllocation(Server & server, const FiveTuple & tuple, const std::string & username, std::int64_t lifetime)
```

Defined in src/turn/include/icy/turn/server/serverallocation.h:36

#### Parameters
* `server` Owning TURN server. 

* `tuple` 5-tuple uniquely identifying this allocation. 

* `username` Authenticated username from the Allocate request. 

* `lifetime` Initial lifetime in seconds as negotiated with the client.

---

{#handlerequest-1}

#### handleRequest

`virtual`

```cpp
virtual bool handleRequest(Request & request)
```

Defined in src/turn/include/icy/turn/server/serverallocation.h:45

Dispatches incoming STUN requests to [handleCreatePermission()](#handlecreatepermission) or [handleRefreshRequest()](#handlerefreshrequest). Subclasses override to add transport-specific methods. 
#### Parameters
* `request` Incoming STUN request. 

#### Returns
true if the request was handled; false signals the server to respond with a 600 "Operation Not Supported" error.

##### Reimplemented by

- [`handleRequest`](#handlerequest-2)
- [`handleRequest`](#handlerequest-3)

---

{#handlerefreshrequest}

#### handleRefreshRequest

`virtual`

```cpp
virtual void handleRefreshRequest(Request & request)
```

Defined in src/turn/include/icy/turn/server/serverallocation.h:50

Processes a Refresh request; updates the lifetime or sets the delete flag if the requested lifetime is 0, then sends a success response. 
#### Parameters
* `request` Incoming Refresh request.

---

{#handlecreatepermission}

#### handleCreatePermission

`virtual`

```cpp
virtual void handleCreatePermission(Request & request)
```

Defined in src/turn/include/icy/turn/server/serverallocation.h:55

Processes a CreatePermission request; installs or refreshes permissions for each XOR-PEER-ADDRESS attribute, then sends a success response. 
#### Parameters
* `request` Incoming CreatePermission request.

---

{#ontimer-4}

#### onTimer

`nodiscard`

```cpp
[[nodiscard]] bool onTimer()
```

Defined in src/turn/include/icy/turn/server/serverallocation.h:61

Periodic maintenance callback called by [Server::onTimer()](#ontimer-3). Removes expired permissions. Returns false to signal that this allocation should be destroyed. 
#### Returns
true to keep the allocation alive; false to destroy it.

---

{#timeremaining-1}

#### timeRemaining

`virtual` `const` `nodiscard` `override`

```cpp
[[nodiscard]] virtual std::int64_t timeRemaining() const override
```

Defined in src/turn/include/icy/turn/server/serverallocation.h:64

#### Returns
The lesser of [IAllocation::timeRemaining()](#timeremaining) and [maxTimeRemaining()](#maxtimeremaining).

##### Reimplements

- [`timeRemaining`](#timeremaining)

---

{#maxtimeremaining}

#### maxTimeRemaining

`const` `nodiscard`

```cpp
[[nodiscard]] std::int64_t maxTimeRemaining() const
```

Defined in src/turn/include/icy/turn/server/serverallocation.h:67

#### Returns
Seconds remaining before the server-enforced maximum lifetime expires.

---

{#haspermission-4}

#### hasPermission

`virtual` `nodiscard` `override`

```cpp
[[nodiscard]] virtual bool hasPermission(const std::string & peerIP) override
```

Defined in src/turn/include/icy/turn/server/serverallocation.h:70

Checks permission, auto-granting local IPs if enabled in server options.

##### Reimplements

- [`hasPermission`](#haspermission-2)

---

{#haspermission-5}

#### hasPermission

`virtual` `nodiscard` `override`

```cpp
[[nodiscard]] virtual bool hasPermission(const net::Address & peerAddress) override
```

Defined in src/turn/include/icy/turn/server/serverallocation.h:71

Checks whether a permission exists for `peerAddress`. The port is ignored; TURN permissions are IP-only.

##### Reimplements

- [`hasPermission`](#haspermission-3)

---

{#server-9}

#### server

`virtual`

```cpp
virtual Server & server()
```

Defined in src/turn/include/icy/turn/server/serverallocation.h:74

#### Returns
Reference to the owning TURN server.

---

{#print-12}

#### print

`virtual` `const` `override`

```cpp
virtual void print(std::ostream & os) const override
```

Defined in src/turn/include/icy/turn/server/serverallocation.h:77

Writes a detailed diagnostic dump of this allocation to `os`.

##### Reimplements

- [`print`](#print-11)

---

{#refreshdeleterequested}

#### refreshDeleteRequested

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline bool refreshDeleteRequested() const
```

Defined in src/turn/include/icy/turn/server/serverallocation.h:81

Returns true if the refresh request set lifetime to 0, signaling the parent [Server](#server-6) to delete this allocation.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `uint32_t` | [`_maxLifetime`](#_maxlifetime)  |  |
| `Server &` | [`_server`](#_server-1)  |  |
| `PermissionPolicy` | [`_permissionPolicy`](#_permissionpolicy)  |  |
| `bool` | [`_refreshDeleteRequested`](#_refreshdeleterequested)  |  |

---

{#_maxlifetime}

#### _maxLifetime

```cpp
uint32_t _maxLifetime
```

Defined in src/turn/include/icy/turn/server/serverallocation.h:86

---

{#_server-1}

#### _server

```cpp
Server & _server
```

Defined in src/turn/include/icy/turn/server/serverallocation.h:87

---

{#_permissionpolicy}

#### _permissionPolicy

```cpp
PermissionPolicy _permissionPolicy
```

Defined in src/turn/include/icy/turn/server/serverallocation.h:88

---

{#_refreshdeleterequested}

#### _refreshDeleteRequested

```cpp
bool _refreshDeleteRequested = false
```

Defined in src/turn/include/icy/turn/server/serverallocation.h:89

### Private Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`ServerAllocation`](#serverallocation-2)  | NonCopyable and NonMovable. |
|  | [`ServerAllocation`](#serverallocation-3)  | Deleted constructor. |

---

{#serverallocation-2}

#### ServerAllocation

```cpp
ServerAllocation(const ServerAllocation &) = delete
```

Defined in src/turn/include/icy/turn/server/serverallocation.h:93

NonCopyable and NonMovable.

---

{#serverallocation-3}

#### ServerAllocation

```cpp
ServerAllocation(ServerAllocation &&) = delete
```

Defined in src/turn/include/icy/turn/server/serverallocation.h:95

Deleted constructor.

{#tcpallocation}

## TCPAllocation

```cpp
#include <icy/turn/server/tcpallocation.h>
```

```cpp
class TCPAllocation
```

Defined in src/turn/include/icy/turn/server/tcpallocation.h:36

> **Inherits:** [`ServerAllocation`](#serverallocation)

Server-side TCP TURN allocation with connection pair management per RFC 6062. Maintains a control connection for STUN signalling and an acceptor socket for incoming peer TCP connections. Each peer connection is tracked as a [TCPConnectionPair](#tcpconnectionpair) and bridged to the client after a successful ConnectionBind.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`TCPAllocation`](#tcpallocation-1) | `function` | Declared here |
| [`handleRequest`](#handlerequest-2) | `function` | Declared here |
| [`handleConnectRequest`](#handleconnectrequest) | `function` | Declared here |
| [`handleConnectionBindRequest`](#handleconnectionbindrequest-1) | `function` | Declared here |
| [`sendPeerConnectResponse`](#sendpeerconnectresponse) | `function` | Declared here |
| [`sendToControl`](#sendtocontrol) | `function` | Declared here |
| [`control`](#control) | `function` | Declared here |
| [`relayedAddress`](#relayedaddress-2) | `function` | Declared here |
| [`pairs`](#pairs) | `function` | Declared here |
| [`onTimer`](#ontimer-5) | `function` | Declared here |
| [`onPeerAccept`](#onpeeraccept) | `function` | Declared here |
| [`onControlClosed`](#oncontrolclosed) | `function` | Declared here |
| [`_control`](#_control) | `variable` | Declared here |
| [`_acceptor`](#_acceptor) | `variable` | Declared here |
| [`_pairs`](#_pairs) | `variable` | Declared here |
| [`Server`](#server-8) | `friend` | Inherited from [`ServerAllocation`](#serverallocation) |
| [`ServerAllocation`](#serverallocation-1) | `function` | Inherited from [`ServerAllocation`](#serverallocation) |
| [`handleRequest`](#handlerequest-1) | `function` | Inherited from [`ServerAllocation`](#serverallocation) |
| [`handleRefreshRequest`](#handlerefreshrequest) | `function` | Inherited from [`ServerAllocation`](#serverallocation) |
| [`handleCreatePermission`](#handlecreatepermission) | `function` | Inherited from [`ServerAllocation`](#serverallocation) |
| [`onTimer`](#ontimer-4) | `function` | Inherited from [`ServerAllocation`](#serverallocation) |
| [`timeRemaining`](#timeremaining-1) | `function` | Inherited from [`ServerAllocation`](#serverallocation) |
| [`maxTimeRemaining`](#maxtimeremaining) | `function` | Inherited from [`ServerAllocation`](#serverallocation) |
| [`hasPermission`](#haspermission-4) | `function` | Inherited from [`ServerAllocation`](#serverallocation) |
| [`hasPermission`](#haspermission-5) | `function` | Inherited from [`ServerAllocation`](#serverallocation) |
| [`server`](#server-9) | `function` | Inherited from [`ServerAllocation`](#serverallocation) |
| [`print`](#print-12) | `function` | Inherited from [`ServerAllocation`](#serverallocation) |
| [`refreshDeleteRequested`](#refreshdeleterequested) | `function` | Inherited from [`ServerAllocation`](#serverallocation) |
| [`_maxLifetime`](#_maxlifetime) | `variable` | Inherited from [`ServerAllocation`](#serverallocation) |
| [`_server`](#_server-1) | `variable` | Inherited from [`ServerAllocation`](#serverallocation) |
| [`_permissionPolicy`](#_permissionpolicy) | `variable` | Inherited from [`ServerAllocation`](#serverallocation) |
| [`_refreshDeleteRequested`](#_refreshdeleterequested) | `variable` | Inherited from [`ServerAllocation`](#serverallocation) |
| [`ServerAllocation`](#serverallocation-2) | `function` | Inherited from [`ServerAllocation`](#serverallocation) |
| [`ServerAllocation`](#serverallocation-3) | `function` | Inherited from [`ServerAllocation`](#serverallocation) |
| [`operator<<`](#operator-28) | `friend` | Inherited from [`IAllocation`](#iallocation) |
| [`IAllocation`](#iallocation-1) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`IAllocation`](#iallocation-2) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`IAllocation`](#iallocation-3) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`updateUsage`](#updateusage) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`setLifetime`](#setlifetime) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`setBandwidthLimit`](#setbandwidthlimit) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`expired`](#expired) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`deleted`](#deleted-1) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`bandwidthLimit`](#bandwidthlimit) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`bandwidthUsed`](#bandwidthused) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`bandwidthRemaining`](#bandwidthremaining) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`timeRemaining`](#timeremaining) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`tuple`](#tuple) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`username`](#username-3) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`lifetime`](#lifetime-1) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`permissions`](#permissions) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`relayedAddress`](#relayedaddress-1) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`addPermission`](#addpermission-3) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`addPermission`](#addpermission-4) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`addPermissions`](#addpermissions) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`removePermission`](#removepermission-2) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`removePermission`](#removepermission-3) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`removeAllPermissions`](#removeallpermissions) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`removeExpiredPermissions`](#removeexpiredpermissions) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`hasPermission`](#haspermission-2) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`hasPermission`](#haspermission-3) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`print`](#print-11) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`_tuple`](#_tuple) | `variable` | Inherited from [`IAllocation`](#iallocation) |
| [`_username`](#_username-2) | `variable` | Inherited from [`IAllocation`](#iallocation) |
| [`_permissions`](#_permissions) | `variable` | Inherited from [`IAllocation`](#iallocation) |
| [`_lifetime`](#_lifetime) | `variable` | Inherited from [`IAllocation`](#iallocation) |
| [`_bandwidthLimit`](#_bandwidthlimit) | `variable` | Inherited from [`IAllocation`](#iallocation) |
| [`_bandwidthUsed`](#_bandwidthused) | `variable` | Inherited from [`IAllocation`](#iallocation) |
| [`_createdAt`](#_createdat) | `variable` | Inherited from [`IAllocation`](#iallocation) |
| [`_updatedAt`](#_updatedat) | `variable` | Inherited from [`IAllocation`](#iallocation) |
| [`_deleted`](#_deleted) | `variable` | Inherited from [`IAllocation`](#iallocation) |

### Inherited from [`ServerAllocation`](#serverallocation)

| Kind | Name | Description |
|------|------|-------------|
| `friend` | [`Server`](#server-8)  |  |
| `function` | [`ServerAllocation`](#serverallocation-1)  |  |
| `function` | [`handleRequest`](#handlerequest-1) `virtual` | Dispatches incoming STUN requests to [handleCreatePermission()](#handlecreatepermission) or [handleRefreshRequest()](#handlerefreshrequest). Subclasses override to add transport-specific methods. |
| `function` | [`handleRefreshRequest`](#handlerefreshrequest) `virtual` | Processes a Refresh request; updates the lifetime or sets the delete flag if the requested lifetime is 0, then sends a success response. |
| `function` | [`handleCreatePermission`](#handlecreatepermission) `virtual` | Processes a CreatePermission request; installs or refreshes permissions for each XOR-PEER-ADDRESS attribute, then sends a success response. |
| `function` | [`onTimer`](#ontimer-4) `nodiscard` | Periodic maintenance callback called by [Server::onTimer()](#ontimer-3). Removes expired permissions. Returns false to signal that this allocation should be destroyed. |
| `function` | [`timeRemaining`](#timeremaining-1) `virtual` `const` `nodiscard` `override` |  |
| `function` | [`maxTimeRemaining`](#maxtimeremaining) `const` `nodiscard` |  |
| `function` | [`hasPermission`](#haspermission-4) `virtual` `nodiscard` `override` | Checks permission, auto-granting local IPs if enabled in server options. |
| `function` | [`hasPermission`](#haspermission-5) `virtual` `nodiscard` `override` | Checks whether a permission exists for `peerAddress`. The port is ignored; TURN permissions are IP-only. |
| `function` | [`server`](#server-9) `virtual` |  |
| `function` | [`print`](#print-12) `virtual` `const` `override` | Writes a detailed diagnostic dump of this allocation to `os`. |
| `function` | [`refreshDeleteRequested`](#refreshdeleterequested) `const` `inline` `nodiscard` | Returns true if the refresh request set lifetime to 0, signaling the parent [Server](#server-6) to delete this allocation. |
| `variable` | [`_maxLifetime`](#_maxlifetime)  |  |
| `variable` | [`_server`](#_server-1)  |  |
| `variable` | [`_permissionPolicy`](#_permissionpolicy)  |  |
| `variable` | [`_refreshDeleteRequested`](#_refreshdeleterequested)  |  |
| `function` | [`ServerAllocation`](#serverallocation-2)  | NonCopyable and NonMovable. |
| `function` | [`ServerAllocation`](#serverallocation-3)  | Deleted constructor. |

### Inherited from [`IAllocation`](#iallocation)

| Kind | Name | Description |
|------|------|-------------|
| `friend` | [`operator<<`](#operator-28) `inline` |  |
| `function` | [`IAllocation`](#iallocation-1)  |  |
| `function` | [`IAllocation`](#iallocation-2)  | Deleted constructor. |
| `function` | [`IAllocation`](#iallocation-3)  | Deleted constructor. |
| `function` | [`updateUsage`](#updateusage) `virtual` | Updates the last-activity timestamp and accumulates bandwidth usage. Call this whenever data is relayed through the allocation. |
| `function` | [`setLifetime`](#setlifetime) `virtual` | Sets the allocation lifetime in seconds and resets the activity timestamp, effectively extending the expiry from the current moment. |
| `function` | [`setBandwidthLimit`](#setbandwidthlimit) `virtual` | Sets the maximum number of bytes this allocation may relay in its lifetime. Pass 0 to disable bandwidth limiting. |
| `function` | [`expired`](#expired) `virtual` `const` `nodiscard` |  |
| `function` | [`deleted`](#deleted-1) `virtual` `const` `nodiscard` | Returns true if the allocation's deleted flag is set and or if the allocation has expired. |
| `function` | [`bandwidthLimit`](#bandwidthlimit) `virtual` `const` `nodiscard` |  |
| `function` | [`bandwidthUsed`](#bandwidthused) `virtual` `const` `nodiscard` |  |
| `function` | [`bandwidthRemaining`](#bandwidthremaining) `virtual` `const` `nodiscard` |  |
| `function` | [`timeRemaining`](#timeremaining) `virtual` `const` `nodiscard` |  |
| `function` | [`tuple`](#tuple) `virtual` |  |
| `function` | [`username`](#username-3) `virtual` `const` `nodiscard` |  |
| `function` | [`lifetime`](#lifetime-1) `virtual` `const` `nodiscard` |  |
| `function` | [`permissions`](#permissions) `virtual` `const` `nodiscard` |  |
| `function` | [`relayedAddress`](#relayedaddress-1) `virtual` `const` `nodiscard` |  |
| `function` | [`addPermission`](#addpermission-3) `virtual` | Adds a permission for `ip`, or refreshes the existing one. |
| `function` | [`addPermission`](#addpermission-4) `virtual` | Adds a permission for `address`, or refreshes the existing one. The port is ignored; TURN permissions are IP-only. |
| `function` | [`addPermissions`](#addpermissions) `virtual` | Adds (or refreshes) permissions for multiple IPs. |
| `function` | [`removePermission`](#removepermission-2) `virtual` | Removes the permission for `ip` if present. |
| `function` | [`removePermission`](#removepermission-3) `virtual` | Removes the permission for `address` if present. The port is ignored; TURN permissions are IP-only. |
| `function` | [`removeAllPermissions`](#removeallpermissions) `virtual` | Removes all permissions from the list. |
| `function` | [`removeExpiredPermissions`](#removeexpiredpermissions) `virtual` | Removes any permissions whose 5-minute lifetime has elapsed. |
| `function` | [`hasPermission`](#haspermission-2) `virtual` `nodiscard` | Checks whether a permission exists for `peerIP`. Local addresses (192.168.x.x and 127.x.x.x) are always permitted. |
| `function` | [`hasPermission`](#haspermission-3) `virtual` `nodiscard` | Checks whether a permission exists for `peerAddress`. The port is ignored; TURN permissions are IP-only. |
| `function` | [`print`](#print-11) `virtual` `const` `inline` |  |
| `variable` | [`_tuple`](#_tuple)  |  |
| `variable` | [`_username`](#_username-2)  |  |
| `variable` | [`_permissions`](#_permissions)  |  |
| `variable` | [`_lifetime`](#_lifetime)  |  |
| `variable` | [`_bandwidthLimit`](#_bandwidthlimit)  |  |
| `variable` | [`_bandwidthUsed`](#_bandwidthused)  |  |
| `variable` | [`_createdAt`](#_createdat)  |  |
| `variable` | [`_updatedAt`](#_updatedat)  |  |
| `variable` | [`_deleted`](#_deleted)  |  |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`TCPAllocation`](#tcpallocation-1)  |  |
| `bool` | [`handleRequest`](#handlerequest-2) `virtual` | Extends the base handler with Connect and ConnectionBind methods. |
| `void` | [`handleConnectRequest`](#handleconnectrequest)  | Handles a Connect request; creates a [TCPConnectionPair](#tcpconnectionpair) and initiates a TCP connection to the peer address carried in XOR-PEER-ADDRESS. |
| `void` | [`handleConnectionBindRequest`](#handleconnectionbindrequest-1)  | Handles a ConnectionBind request; associates the incoming TCP data socket with the pending [TCPConnectionPair](#tcpconnectionpair) and activates the relay pipe. |
| `void` | [`sendPeerConnectResponse`](#sendpeerconnectresponse)  | Sends a Connect success or failure response to the control connection. |
| `ssize_t` | [`sendToControl`](#sendtocontrol)  | Sends a STUN message to the client over the control TCP connection. |
| `net::TCPSocket &` | [`control`](#control)  |  |
| `net::Address` | [`relayedAddress`](#relayedaddress-2) `virtual` `const` `nodiscard` |  |
| `TCPConnectionPairMap &` | [`pairs`](#pairs)  |  |
| `bool` | [`onTimer`](#ontimer-5)  | Periodic timer callback; removes expired or pending-delete connection pairs, then calls the base class timer. |
| `void` | [`onPeerAccept`](#onpeeraccept)  | Accepts incoming peer sockets on the relay acceptor socket. Checks permissions and sends a ConnectionAttempt indication to the control. |
| `bool` | [`onControlClosed`](#oncontrolclosed)  | Called when the control connection closes; marks the allocation for deletion. |

---

{#tcpallocation-1}

#### TCPAllocation

```cpp
TCPAllocation(Server & server, const net::Socket::Ptr & control, const FiveTuple & tuple, const std::string & username, const uint32_t & lifetime)
```

Defined in src/turn/include/icy/turn/server/tcpallocation.h:44

#### Parameters
* `server` Owning TURN server. 

* `control` The accepted TCP socket used for STUN control messages. 

* `tuple` 5-tuple identifying this allocation. 

* `username` Authenticated username from the Allocate request. 

* `lifetime` Initial lifetime in seconds.

---

{#handlerequest-2}

#### handleRequest

`virtual`

```cpp
virtual bool handleRequest(Request & request)
```

Defined in src/turn/include/icy/turn/server/tcpallocation.h:52

Extends the base handler with Connect and ConnectionBind methods. 
#### Parameters
* `request` Incoming STUN request. 

#### Returns
true if handled.

##### Reimplements

- [`handleRequest`](#handlerequest-1)

---

{#handleconnectrequest}

#### handleConnectRequest

```cpp
void handleConnectRequest(Request & request)
```

Defined in src/turn/include/icy/turn/server/tcpallocation.h:57

Handles a Connect request; creates a [TCPConnectionPair](#tcpconnectionpair) and initiates a TCP connection to the peer address carried in XOR-PEER-ADDRESS. 
#### Parameters
* `request` Incoming Connect request.

---

{#handleconnectionbindrequest-1}

#### handleConnectionBindRequest

```cpp
void handleConnectionBindRequest(Request & request)
```

Defined in src/turn/include/icy/turn/server/tcpallocation.h:62

Handles a ConnectionBind request; associates the incoming TCP data socket with the pending [TCPConnectionPair](#tcpconnectionpair) and activates the relay pipe. 
#### Parameters
* `request` Incoming ConnectionBind request.

---

{#sendpeerconnectresponse}

#### sendPeerConnectResponse

```cpp
void sendPeerConnectResponse(TCPConnectionPair * pair, bool success)
```

Defined in src/turn/include/icy/turn/server/tcpallocation.h:67

Sends a Connect success or failure response to the control connection. 
#### Parameters
* `pair` The connection pair whose peer connection attempt completed. 

* `success` true if the peer TCP connection succeeded.

---

{#sendtocontrol}

#### sendToControl

```cpp
ssize_t sendToControl(stun::Message & message)
```

Defined in src/turn/include/icy/turn/server/tcpallocation.h:72

Sends a STUN message to the client over the control TCP connection. 
#### Parameters
* `message` Message to send. 

#### Returns
Number of bytes sent, or a negative value on error.

---

{#control}

#### control

```cpp
net::TCPSocket & control()
```

Defined in src/turn/include/icy/turn/server/tcpallocation.h:75

#### Returns
Reference to the TCP control socket.

---

{#relayedaddress-2}

#### relayedAddress

`virtual` `const` `nodiscard`

```cpp
[[nodiscard]] virtual net::Address relayedAddress() const
```

Defined in src/turn/include/icy/turn/server/tcpallocation.h:78

#### Returns
The listen address of the peer acceptor socket (used as the relayed address).

##### Reimplements

- [`relayedAddress`](#relayedaddress-1)

---

{#pairs}

#### pairs

```cpp
TCPConnectionPairMap & pairs()
```

Defined in src/turn/include/icy/turn/server/tcpallocation.h:81

#### Returns
Reference to the map of active TCPConnectionPairs keyed by connection ID.

---

{#ontimer-5}

#### onTimer

```cpp
bool onTimer()
```

Defined in src/turn/include/icy/turn/server/tcpallocation.h:86

Periodic timer callback; removes expired or pending-delete connection pairs, then calls the base class timer. 
#### Returns
false if the allocation should be destroyed.

---

{#onpeeraccept}

#### onPeerAccept

```cpp
void onPeerAccept(const net::TCPSocket::Ptr & sock)
```

Defined in src/turn/include/icy/turn/server/tcpallocation.h:91

Accepts incoming peer sockets on the relay acceptor socket. Checks permissions and sends a ConnectionAttempt indication to the control. 
#### Parameters
* `sock` Newly accepted peer socket.

---

{#oncontrolclosed}

#### onControlClosed

```cpp
bool onControlClosed(net::Socket & socket)
```

Defined in src/turn/include/icy/turn/server/tcpallocation.h:96

Called when the control connection closes; marks the allocation for deletion. 
#### Parameters
* `socket` The closed control socket. 

#### Returns
false (event propagation convention).

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `net::SocketEmitter` | [`_control`](#_control)  |  |
| `net::SocketEmitter` | [`_acceptor`](#_acceptor)  |  |
| `TCPConnectionPairMap` | [`_pairs`](#_pairs)  |  |

---

{#_control}

#### _control

```cpp
net::SocketEmitter _control
```

Defined in src/turn/include/icy/turn/server/tcpallocation.h:99

---

{#_acceptor}

#### _acceptor

```cpp
net::SocketEmitter _acceptor
```

Defined in src/turn/include/icy/turn/server/tcpallocation.h:100

---

{#_pairs}

#### _pairs

```cpp
TCPConnectionPairMap _pairs
```

Defined in src/turn/include/icy/turn/server/tcpallocation.h:101

{#tcpclient}

## TCPClient

```cpp
#include <icy/turn/client/tcpclient.h>
```

```cpp
class TCPClient
```

Defined in src/turn/include/icy/turn/client/tcpclient.h:87

> **Inherits:** [`Client`](#client-2)

TCP transport TURN client with relay connection management per RFC 6062. Uses a single TCP control connection for STUN signalling and opens additional TCP relay connections for each peer via the Connect / ConnectionBind flow.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`TCPClient`](#tcpclient-1) | `function` | Declared here |
| [`start`](#start-15) | `function` | Declared here |
| [`stop`](#stop-12) | `function` | Declared here |
| [`sendConnectRequest`](#sendconnectrequest) | `function` | Declared here |
| [`sendData`](#senddata-1) | `function` | Declared here |
| [`handleResponse`](#handleresponse-1) | `function` | Declared here |
| [`handleConnectResponse`](#handleconnectresponse) | `function` | Declared here |
| [`handleConnectErrorResponse`](#handleconnecterrorresponse) | `function` | Declared here |
| [`handleConnectionBindResponse`](#handleconnectionbindresponse) | `function` | Declared here |
| [`handleConnectionBindErrorResponse`](#handleconnectionbinderrorresponse) | `function` | Declared here |
| [`handleConnectionAttemptIndication`](#handleconnectionattemptindication) | `function` | Declared here |
| [`createAndBindConnection`](#createandbindconnection) | `function` | Declared here |
| [`onRelayConnectionConnect`](#onrelayconnectionconnect) | `function` | Declared here |
| [`onRelayDataReceived`](#onrelaydatareceived-1) | `function` | Declared here |
| [`onRelayConnectionError`](#onrelayconnectionerror) | `function` | Declared here |
| [`onRelayConnectionClosed`](#onrelayconnectionclosed) | `function` | Declared here |
| [`freeConnection`](#freeconnection) | `function` | Declared here |
| [`transportProtocol`](#transportprotocol-1) | `function` | Declared here |
| [`connections`](#connections) | `function` | Declared here |
| [`className`](#classname-6) | `function` | Declared here |
| [`_observer`](#_observer-3) | `variable` | Declared here |
| [`_connections`](#_connections-2) | `variable` | Declared here |
| [`Client`](#client-3) | `function` | Inherited from [`Client`](#client-2) |
| [`start`](#start-13) | `function` | Inherited from [`Client`](#client-2) |
| [`stop`](#stop-10) | `function` | Inherited from [`Client`](#client-2) |
| [`sendAllocate`](#sendallocate) | `function` | Inherited from [`Client`](#client-2) |
| [`addPermission`](#addpermission) | `function` | Inherited from [`Client`](#client-2) |
| [`addPermission`](#addpermission-1) | `function` | Inherited from [`Client`](#client-2) |
| [`sendCreatePermission`](#sendcreatepermission) | `function` | Inherited from [`Client`](#client-2) |
| [`sendChannelBind`](#sendchannelbind) | `function` | Inherited from [`Client`](#client-2) |
| [`sendRefresh`](#sendrefresh) | `function` | Inherited from [`Client`](#client-2) |
| [`sendData`](#senddata) | `function` | Inherited from [`Client`](#client-2) |
| [`handleResponse`](#handleresponse) | `function` | Inherited from [`Client`](#client-2) |
| [`handleAllocateResponse`](#handleallocateresponse) | `function` | Inherited from [`Client`](#client-2) |
| [`handleAllocateErrorResponse`](#handleallocateerrorresponse) | `function` | Inherited from [`Client`](#client-2) |
| [`handleCreatePermissionResponse`](#handlecreatepermissionresponse) | `function` | Inherited from [`Client`](#client-2) |
| [`handleCreatePermissionErrorResponse`](#handlecreatepermissionerrorresponse) | `function` | Inherited from [`Client`](#client-2) |
| [`handleRefreshResponse`](#handlerefreshresponse) | `function` | Inherited from [`Client`](#client-2) |
| [`handleDataIndication`](#handledataindication) | `function` | Inherited from [`Client`](#client-2) |
| [`transportProtocol`](#transportprotocol) | `function` | Inherited from [`Client`](#client-2) |
| [`createTransaction`](#createtransaction) | `function` | Inherited from [`Client`](#client-2) |
| [`authenticateRequest`](#authenticaterequest) | `function` | Inherited from [`Client`](#client-2) |
| [`sendAuthenticatedTransaction`](#sendauthenticatedtransaction) | `function` | Inherited from [`Client`](#client-2) |
| [`removeTransaction`](#removetransaction) | `function` | Inherited from [`Client`](#client-2) |
| [`mappedAddress`](#mappedaddress) | `function` | Inherited from [`Client`](#client-2) |
| [`relayedAddress`](#relayedaddress) | `function` | Inherited from [`Client`](#client-2) |
| [`closed`](#closed-5) | `function` | Inherited from [`Client`](#client-2) |
| [`observer`](#observer-1) | `function` | Inherited from [`Client`](#client-2) |
| [`options`](#options-8) | `function` | Inherited from [`Client`](#client-2) |
| [`addPermission`](#addpermission-2) | `function` | Inherited from [`Client`](#client-2) |
| [`hasPermission`](#haspermission) | `function` | Inherited from [`Client`](#client-2) |
| [`hasPermission`](#haspermission-1) | `function` | Inherited from [`Client`](#client-2) |
| [`removePermission`](#removepermission) | `function` | Inherited from [`Client`](#client-2) |
| [`removePermission`](#removepermission-1) | `function` | Inherited from [`Client`](#client-2) |
| [`_observer`](#_observer-1) | `variable` | Inherited from [`Client`](#client-2) |
| [`_options`](#_options-3) | `variable` | Inherited from [`Client`](#client-2) |
| [`_socket`](#_socket-3) | `variable` | Inherited from [`Client`](#client-2) |
| [`_timer`](#_timer-1) | `variable` | Inherited from [`Client`](#client-2) |
| [`_error`](#_error-5) | `variable` | Inherited from [`Client`](#client-2) |
| [`_mappedAddress`](#_mappedaddress) | `variable` | Inherited from [`Client`](#client-2) |
| [`_relayedAddress`](#_relayedaddress) | `variable` | Inherited from [`Client`](#client-2) |
| [`_realm`](#_realm) | `variable` | Inherited from [`Client`](#client-2) |
| [`_nonce`](#_nonce) | `variable` | Inherited from [`Client`](#client-2) |
| [`_pendingIndications`](#_pendingindications) | `variable` | Inherited from [`Client`](#client-2) |
| [`_transactions`](#_transactions) | `variable` | Inherited from [`Client`](#client-2) |
| [`setError`](#seterror-5) | `function` | Inherited from [`Client`](#client-2) |
| [`onSocketConnect`](#onsocketconnect-5) | `function` | Inherited from [`Client`](#client-2) |
| [`onSocketRecv`](#onsocketrecv-7) | `function` | Inherited from [`Client`](#client-2) |
| [`onSocketClose`](#onsocketclose-5) | `function` | Inherited from [`Client`](#client-2) |
| [`onTransactionProgress`](#ontransactionprogress) | `function` | Inherited from [`Client`](#client-2) |
| [`onStateChange`](#onstatechange-1) | `function` | Inherited from [`Client`](#client-2) |
| [`onTimer`](#ontimer-1) | `function` | Inherited from [`Client`](#client-2) |
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
| [`operator<<`](#operator-28) | `friend` | Inherited from [`IAllocation`](#iallocation) |
| [`IAllocation`](#iallocation-1) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`IAllocation`](#iallocation-2) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`IAllocation`](#iallocation-3) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`updateUsage`](#updateusage) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`setLifetime`](#setlifetime) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`setBandwidthLimit`](#setbandwidthlimit) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`expired`](#expired) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`deleted`](#deleted-1) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`bandwidthLimit`](#bandwidthlimit) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`bandwidthUsed`](#bandwidthused) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`bandwidthRemaining`](#bandwidthremaining) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`timeRemaining`](#timeremaining) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`tuple`](#tuple) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`username`](#username-3) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`lifetime`](#lifetime-1) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`permissions`](#permissions) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`relayedAddress`](#relayedaddress-1) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`addPermission`](#addpermission-3) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`addPermission`](#addpermission-4) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`addPermissions`](#addpermissions) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`removePermission`](#removepermission-2) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`removePermission`](#removepermission-3) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`removeAllPermissions`](#removeallpermissions) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`removeExpiredPermissions`](#removeexpiredpermissions) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`hasPermission`](#haspermission-2) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`hasPermission`](#haspermission-3) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`print`](#print-11) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`_tuple`](#_tuple) | `variable` | Inherited from [`IAllocation`](#iallocation) |
| [`_username`](#_username-2) | `variable` | Inherited from [`IAllocation`](#iallocation) |
| [`_permissions`](#_permissions) | `variable` | Inherited from [`IAllocation`](#iallocation) |
| [`_lifetime`](#_lifetime) | `variable` | Inherited from [`IAllocation`](#iallocation) |
| [`_bandwidthLimit`](#_bandwidthlimit) | `variable` | Inherited from [`IAllocation`](#iallocation) |
| [`_bandwidthUsed`](#_bandwidthused) | `variable` | Inherited from [`IAllocation`](#iallocation) |
| [`_createdAt`](#_createdat) | `variable` | Inherited from [`IAllocation`](#iallocation) |
| [`_updatedAt`](#_updatedat) | `variable` | Inherited from [`IAllocation`](#iallocation) |
| [`_deleted`](#_deleted) | `variable` | Inherited from [`IAllocation`](#iallocation) |

### Inherited from [`Client`](#client-2)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`Client`](#client-3)  |  |
| `function` | [`start`](#start-13) `virtual` | Connects the socket to the TURN server and starts the allocation sequence. Permissions must be added via [addPermission()](#addpermission) before calling this. |
| `function` | [`stop`](#stop-10) `virtual` | Stops the timer, cancels pending transactions, and closes the socket. |
| `function` | [`sendAllocate`](#sendallocate) `virtual` | Sends an Allocate request to the server with the configured transport and lifetime. On first call the server will typically respond with a 401 challenge; the client re-sends with credentials automatically. |
| `function` | [`addPermission`](#addpermission) `virtual` | Adds multiple peer IP addresses to the permission list. |
| `function` | [`addPermission`](#addpermission-1) `virtual` `override` | Adds a single peer IP to the permission list, or refreshes it if already present. Permissions should be added before [start()](#start-13); they may also be added later, in which case a new CreatePermission request is required. |
| `function` | [`sendCreatePermission`](#sendcreatepermission) `virtual` | Sends a CreatePermission request for all IPs currently in the permission list. Called automatically after allocation succeeds and periodically by the timer to refresh expiring permissions. |
| `function` | [`sendChannelBind`](#sendchannelbind) `virtual` | Channel bindings (RFC 5766 Section 11) are intentionally not implemented. They are a bandwidth optimization that replaces STUN-framed Send/Data indications with a compact 4-byte ChannelData header. This only benefits high-throughput media relay scenarios; in practice, media flows directly via ICE/DTLS rather than through this TURN client's data path, so the optimization is not worth the complexity (channel number allocation, 10-minute binding refresh timers, ChannelData wire framing). Data relay uses [sendData()](#senddata) with Send Indications instead. |
| `function` | [`sendRefresh`](#sendrefresh) `virtual` | Sends a Refresh request to extend the allocation lifetime. Called automatically by the timer when roughly one-third of the lifetime remains. |
| `function` | [`sendData`](#senddata) `virtual` | Sends a Send Indication to relay `data` to `peerAddress` through the TURN server. If permissions are still being negotiated the indication is queued and flushed once CreatePermission succeeds. |
| `function` | [`handleResponse`](#handleresponse) `virtual` | Dispatches an incoming STUN/TURN response to the appropriate handler. |
| `function` | [`handleAllocateResponse`](#handleallocateresponse) `virtual` | Processes a successful Allocate response; extracts mapped/relayed addresses and advances the state to Authorizing, then sends CreatePermission. |
| `function` | [`handleAllocateErrorResponse`](#handleallocateerrorresponse) `virtual` | Handles an Allocate error response; manages the 401 challenge/re-send flow and sets the client to Failed for unrecoverable errors. |
| `function` | [`handleCreatePermissionResponse`](#handlecreatepermissionresponse) `virtual` | Handles a successful CreatePermission response; flushes queued Send Indications and advances state to Success. |
| `function` | [`handleCreatePermissionErrorResponse`](#handlecreatepermissionerrorresponse) `virtual` | Handles a failed CreatePermission response; clears all permissions and sets the client to Failed. |
| `function` | [`handleRefreshResponse`](#handlerefreshresponse) `virtual` | Handles a Refresh response; updates the stored lifetime. |
| `function` | [`handleDataIndication`](#handledataindication) `virtual` | Handles an incoming Data Indication; extracts peer address and data and forwards to [ClientObserver::onRelayDataReceived()](#onrelaydatareceived). |
| `function` | [`transportProtocol`](#transportprotocol) `virtual` |  |
| `function` | [`createTransaction`](#createtransaction) `virtual` | Creates a new STUN transaction, registers the progress callback, and adds it to the active transaction list. |
| `function` | [`authenticateRequest`](#authenticaterequest) `virtual` | Adds STUN long-term credential attributes (Username, Realm, Nonce, MessageIntegrity) to `request` if the realm has been received from the server. |
| `function` | [`sendAuthenticatedTransaction`](#sendauthenticatedtransaction) `virtual` | Calls [authenticateRequest()](#authenticaterequest) then sends the transaction. |
| `function` | [`removeTransaction`](#removetransaction) `virtual` | Removes a transaction from the active list. The [IntrusivePtr](base.md#intrusiveptr) keeps the object alive until the caller's copy is released. |
| `function` | [`mappedAddress`](#mappedaddress) `const` `nodiscard` |  |
| `function` | [`relayedAddress`](#relayedaddress) `virtual` `const` `nodiscard` `override` |  |
| `function` | [`closed`](#closed-5) `const` `nodiscard` |  |
| `function` | [`observer`](#observer-1)  |  |
| `function` | [`options`](#options-8) `const` `nodiscard` |  |
| `function` | [`addPermission`](#addpermission-2) `virtual` | Adds a permission for `address`, or refreshes the existing one. The port is ignored; TURN permissions are IP-only. |
| `function` | [`hasPermission`](#haspermission) `virtual` `nodiscard` | Checks whether a permission exists for `peerIP`. Local addresses (192.168.x.x and 127.x.x.x) are always permitted. |
| `function` | [`hasPermission`](#haspermission-1) `virtual` `nodiscard` | Checks whether a permission exists for `peerAddress`. The port is ignored; TURN permissions are IP-only. |
| `function` | [`removePermission`](#removepermission) `virtual` | Removes the permission for `ip` if present. |
| `function` | [`removePermission`](#removepermission-1) `virtual` | Removes the permission for `address` if present. The port is ignored; TURN permissions are IP-only. |
| `variable` | [`_observer`](#_observer-1)  |  |
| `variable` | [`_options`](#_options-3)  |  |
| `variable` | [`_socket`](#_socket-3)  |  |
| `variable` | [`_timer`](#_timer-1)  |  |
| `variable` | [`_error`](#_error-5)  |  |
| `variable` | [`_mappedAddress`](#_mappedaddress)  |  |
| `variable` | [`_relayedAddress`](#_relayedaddress)  |  |
| `variable` | [`_realm`](#_realm)  |  |
| `variable` | [`_nonce`](#_nonce)  |  |
| `variable` | [`_pendingIndications`](#_pendingindications)  | A list of queued Send indication packets awaiting server permissions. |
| `variable` | [`_transactions`](#_transactions)  | A list containing currently active transactions. |
| `function` | [`setError`](#seterror-5) `virtual` | Sets the error field and transitions the client to the Failed state. |
| `function` | [`onSocketConnect`](#onsocketconnect-5) `virtual` | Socket connect callback; starts the timer and sends the first Allocate request. |
| `function` | [`onSocketRecv`](#onsocketrecv-7) `virtual` | Socket receive callback; parses STUN messages from the buffer and dispatches them. |
| `function` | [`onSocketClose`](#onsocketclose-5) `virtual` | Socket close callback; shuts down the client and records the socket error. |
| `function` | [`onTransactionProgress`](#ontransactionprogress) `virtual` | STUN transaction state-change callback; handles Success and Failed outcomes. |
| `function` | [`onStateChange`](#onstatechange-1) `override` | Forwards state-change events to the observer. |
| `function` | [`onTimer`](#ontimer-1) `virtual` | Periodic timer callback; re-allocates on expiry or refreshes when lifetime is below one-third remaining. Also calls [ClientObserver::onTimer()](#ontimer-2). |

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

### Inherited from [`IAllocation`](#iallocation)

| Kind | Name | Description |
|------|------|-------------|
| `friend` | [`operator<<`](#operator-28) `inline` |  |
| `function` | [`IAllocation`](#iallocation-1)  |  |
| `function` | [`IAllocation`](#iallocation-2)  | Deleted constructor. |
| `function` | [`IAllocation`](#iallocation-3)  | Deleted constructor. |
| `function` | [`updateUsage`](#updateusage) `virtual` | Updates the last-activity timestamp and accumulates bandwidth usage. Call this whenever data is relayed through the allocation. |
| `function` | [`setLifetime`](#setlifetime) `virtual` | Sets the allocation lifetime in seconds and resets the activity timestamp, effectively extending the expiry from the current moment. |
| `function` | [`setBandwidthLimit`](#setbandwidthlimit) `virtual` | Sets the maximum number of bytes this allocation may relay in its lifetime. Pass 0 to disable bandwidth limiting. |
| `function` | [`expired`](#expired) `virtual` `const` `nodiscard` |  |
| `function` | [`deleted`](#deleted-1) `virtual` `const` `nodiscard` | Returns true if the allocation's deleted flag is set and or if the allocation has expired. |
| `function` | [`bandwidthLimit`](#bandwidthlimit) `virtual` `const` `nodiscard` |  |
| `function` | [`bandwidthUsed`](#bandwidthused) `virtual` `const` `nodiscard` |  |
| `function` | [`bandwidthRemaining`](#bandwidthremaining) `virtual` `const` `nodiscard` |  |
| `function` | [`timeRemaining`](#timeremaining) `virtual` `const` `nodiscard` |  |
| `function` | [`tuple`](#tuple) `virtual` |  |
| `function` | [`username`](#username-3) `virtual` `const` `nodiscard` |  |
| `function` | [`lifetime`](#lifetime-1) `virtual` `const` `nodiscard` |  |
| `function` | [`permissions`](#permissions) `virtual` `const` `nodiscard` |  |
| `function` | [`relayedAddress`](#relayedaddress-1) `virtual` `const` `nodiscard` |  |
| `function` | [`addPermission`](#addpermission-3) `virtual` | Adds a permission for `ip`, or refreshes the existing one. |
| `function` | [`addPermission`](#addpermission-4) `virtual` | Adds a permission for `address`, or refreshes the existing one. The port is ignored; TURN permissions are IP-only. |
| `function` | [`addPermissions`](#addpermissions) `virtual` | Adds (or refreshes) permissions for multiple IPs. |
| `function` | [`removePermission`](#removepermission-2) `virtual` | Removes the permission for `ip` if present. |
| `function` | [`removePermission`](#removepermission-3) `virtual` | Removes the permission for `address` if present. The port is ignored; TURN permissions are IP-only. |
| `function` | [`removeAllPermissions`](#removeallpermissions) `virtual` | Removes all permissions from the list. |
| `function` | [`removeExpiredPermissions`](#removeexpiredpermissions) `virtual` | Removes any permissions whose 5-minute lifetime has elapsed. |
| `function` | [`hasPermission`](#haspermission-2) `virtual` `nodiscard` | Checks whether a permission exists for `peerIP`. Local addresses (192.168.x.x and 127.x.x.x) are always permitted. |
| `function` | [`hasPermission`](#haspermission-3) `virtual` `nodiscard` | Checks whether a permission exists for `peerAddress`. The port is ignored; TURN permissions are IP-only. |
| `function` | [`print`](#print-11) `virtual` `const` `inline` |  |
| `variable` | [`_tuple`](#_tuple)  |  |
| `variable` | [`_username`](#_username-2)  |  |
| `variable` | [`_permissions`](#_permissions)  |  |
| `variable` | [`_lifetime`](#_lifetime)  |  |
| `variable` | [`_bandwidthLimit`](#_bandwidthlimit)  |  |
| `variable` | [`_bandwidthUsed`](#_bandwidthused)  |  |
| `variable` | [`_createdAt`](#_createdat)  |  |
| `variable` | [`_updatedAt`](#_updatedat)  |  |
| `variable` | [`_deleted`](#_deleted)  |  |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`TCPClient`](#tcpclient-1)  |  |
| `void` | [`start`](#start-15) `virtual` `override` | Connects the socket to the TURN server and starts the allocation sequence. Permissions must be added via [addPermission()](#addpermission) before calling this. |
| `void` | [`stop`](#stop-12) `virtual` `override` | Stops the control connection, cancels all relay connections, and calls the base class [stop()](#stop-12). |
| `void` | [`sendConnectRequest`](#sendconnectrequest) `virtual` | Sends a Connect request to the server asking it to open a TCP connection to `peerAddress` on the client's behalf (RFC 6062 section 4.3). |
| `void` | [`sendData`](#senddata-1) `virtual` `override` | Sends raw data to `peerAddress` over the established relay connection. The peer must have an active permission and a bound relay connection. |
| `bool` | [`handleResponse`](#handleresponse-1) `virtual` `override` | Extends the base handler with Connect, ConnectionBind, and ConnectionAttempt responses. |
| `void` | [`handleConnectResponse`](#handleconnectresponse) `virtual` | Processes a successful Connect response; extracts the connection ID and calls [createAndBindConnection()](#createandbindconnection). |
| `void` | [`handleConnectErrorResponse`](#handleconnecterrorresponse) `virtual` | Processes a Connect error response; notifies the observer that binding failed. |
| `void` | [`handleConnectionBindResponse`](#handleconnectionbindresponse) `virtual` | Processes a successful ConnectionBind response; arms the relay socket for data transfer and notifies the observer. |
| `void` | [`handleConnectionBindErrorResponse`](#handleconnectionbinderrorresponse) `virtual` | Processes a ConnectionBind error response; frees the failed connection. |
| `void` | [`handleConnectionAttemptIndication`](#handleconnectionattemptindication) `virtual` | Processes a ConnectionAttempt indication from the server; optionally accepts the incoming peer connection by calling [createAndBindConnection()](#createandbindconnection). |
| `bool` | [`createAndBindConnection`](#createandbindconnection) `virtual` | Opens a new TCP relay socket to the TURN server and sends a ConnectionBind request carrying `connectionID`. |
| `bool` | [`onRelayConnectionConnect`](#onrelayconnectionconnect) `virtual` | Relay socket connect callback; sends the ConnectionBind request. |
| `bool` | [`onRelayDataReceived`](#onrelaydatareceived-1) `virtual` | Relay socket receive callback; forwards data to the observer. |
| `bool` | [`onRelayConnectionError`](#onrelayconnectionerror) `virtual` | Relay socket error callback; notifies the observer of the error. |
| `bool` | [`onRelayConnectionClosed`](#onrelayconnectionclosed) `virtual` | Relay socket close callback; notifies the observer and frees the connection. |
| `void` | [`freeConnection`](#freeconnection)  | Removes the relay connection for `peerAddress`, unregisters its callbacks, and deletes the associated [RelayConnectionBinding](#relayconnectionbinding). |
| `int` | [`transportProtocol`](#transportprotocol-1) `virtual` `override` |  |
| `ConnectionManager &` | [`connections`](#connections)  |  |
| `const char *` | [`className`](#classname-6) `virtual` `const` `inline` |  |

---

{#tcpclient-1}

#### TCPClient

```cpp
TCPClient(TCPClientObserver & observer, const Client::Options & options = Client::Options())
```

Defined in src/turn/include/icy/turn/client/tcpclient.h:92

#### Parameters
* `observer` Observer for TCP-specific relay events. 

* `options` [Client](#client-2) configuration; defaults to loopback server, 5-min lifetime.

---

{#start-15}

#### start

`virtual` `override`

```cpp
virtual void start() override
```

Defined in src/turn/include/icy/turn/client/tcpclient.h:96

Connects the socket to the TURN server and starts the allocation sequence. Permissions must be added via [addPermission()](#addpermission) before calling this.

##### Reimplements

- [`start`](#start-13)

---

{#stop-12}

#### stop

`virtual` `override`

```cpp
virtual void stop() override
```

Defined in src/turn/include/icy/turn/client/tcpclient.h:100

Stops the control connection, cancels all relay connections, and calls the base class [stop()](#stop-12).

##### Reimplements

- [`stop`](#stop-10)

---

{#sendconnectrequest}

#### sendConnectRequest

`virtual`

```cpp
virtual void sendConnectRequest(const net::Address & peerAddress)
```

Defined in src/turn/include/icy/turn/client/tcpclient.h:105

Sends a Connect request to the server asking it to open a TCP connection to `peerAddress` on the client's behalf (RFC 6062 section 4.3). 
#### Parameters
* `peerAddress` Target peer address.

---

{#senddata-1}

#### sendData

`virtual` `override`

```cpp
virtual void sendData(const char * data, size_t size, const net::Address & peerAddress) override
```

Defined in src/turn/include/icy/turn/client/tcpclient.h:112

Sends raw data to `peerAddress` over the established relay connection. The peer must have an active permission and a bound relay connection. 
#### Parameters
* `data` Payload bytes. 

* `size` Number of bytes to send. 

* `peerAddress` Destination peer (must have an entry in [connections()](#connections)).

##### Reimplements

- [`sendData`](#senddata)

---

{#handleresponse-1}

#### handleResponse

`virtual` `override`

```cpp
virtual bool handleResponse(const stun::Message & response) override
```

Defined in src/turn/include/icy/turn/client/tcpclient.h:118

Extends the base handler with Connect, ConnectionBind, and ConnectionAttempt responses. 
#### Parameters
* `response` Incoming STUN response. 

#### Returns
true if the message was handled.

##### Reimplements

- [`handleResponse`](#handleresponse)

---

{#handleconnectresponse}

#### handleConnectResponse

`virtual`

```cpp
virtual void handleConnectResponse(const stun::Message & response)
```

Defined in src/turn/include/icy/turn/client/tcpclient.h:123

Processes a successful Connect response; extracts the connection ID and calls [createAndBindConnection()](#createandbindconnection). 
#### Parameters
* `response` Connect success response.

---

{#handleconnecterrorresponse}

#### handleConnectErrorResponse

`virtual`

```cpp
virtual void handleConnectErrorResponse(const stun::Message & response)
```

Defined in src/turn/include/icy/turn/client/tcpclient.h:127

Processes a Connect error response; notifies the observer that binding failed. 
#### Parameters
* `response` Connect error response.

---

{#handleconnectionbindresponse}

#### handleConnectionBindResponse

`virtual`

```cpp
virtual void handleConnectionBindResponse(const stun::Message & response)
```

Defined in src/turn/include/icy/turn/client/tcpclient.h:132

Processes a successful ConnectionBind response; arms the relay socket for data transfer and notifies the observer. 
#### Parameters
* `response` ConnectionBind success response.

---

{#handleconnectionbinderrorresponse}

#### handleConnectionBindErrorResponse

`virtual`

```cpp
virtual void handleConnectionBindErrorResponse(const stun::Message & response)
```

Defined in src/turn/include/icy/turn/client/tcpclient.h:136

Processes a ConnectionBind error response; frees the failed connection. 
#### Parameters
* `response` ConnectionBind error response.

---

{#handleconnectionattemptindication}

#### handleConnectionAttemptIndication

`virtual`

```cpp
virtual void handleConnectionAttemptIndication(const stun::Message & response)
```

Defined in src/turn/include/icy/turn/client/tcpclient.h:141

Processes a ConnectionAttempt indication from the server; optionally accepts the incoming peer connection by calling [createAndBindConnection()](#createandbindconnection). 
#### Parameters
* `response` ConnectionAttempt indication message.

---

{#createandbindconnection}

#### createAndBindConnection

`virtual`

```cpp
virtual bool createAndBindConnection(uint32_t connectionID, const net::Address & peerAddress)
```

Defined in src/turn/include/icy/turn/client/tcpclient.h:148

Opens a new TCP relay socket to the TURN server and sends a ConnectionBind request carrying `connectionID`. 
#### Parameters
* `connectionID` TURN CONNECTION-ID from the server. 

* `peerAddress` Peer associated with this connection. 

#### Returns
true on success.

---

{#onrelayconnectionconnect}

#### onRelayConnectionConnect

`virtual`

```cpp
virtual bool onRelayConnectionConnect(net::Socket & socket)
```

Defined in src/turn/include/icy/turn/client/tcpclient.h:152

Relay socket connect callback; sends the ConnectionBind request. 
#### Parameters
* `socket` The relay socket that just connected.

---

{#onrelaydatareceived-1}

#### onRelayDataReceived

`virtual`

```cpp
virtual bool onRelayDataReceived(net::Socket & socket, const MutableBuffer & buffer, const net::Address & peerAddress)
```

Defined in src/turn/include/icy/turn/client/tcpclient.h:158

Relay socket receive callback; forwards data to the observer. 
#### Parameters
* `socket` The receiving relay socket. 

* `buffer` Received data. 

* `peerAddress` Source address (populated by the socket layer).

---

{#onrelayconnectionerror}

#### onRelayConnectionError

`virtual`

```cpp
virtual bool onRelayConnectionError(net::Socket & socket, const Error & error)
```

Defined in src/turn/include/icy/turn/client/tcpclient.h:163

Relay socket error callback; notifies the observer of the error. 
#### Parameters
* `socket` The affected relay socket. 

* `error` [Error](base.md#error) descriptor.

---

{#onrelayconnectionclosed}

#### onRelayConnectionClosed

`virtual`

```cpp
virtual bool onRelayConnectionClosed(net::Socket & socket)
```

Defined in src/turn/include/icy/turn/client/tcpclient.h:167

Relay socket close callback; notifies the observer and frees the connection. 
#### Parameters
* `socket` The closed relay socket.

---

{#freeconnection}

#### freeConnection

```cpp
void freeConnection(const net::Address & peerAddress)
```

Defined in src/turn/include/icy/turn/client/tcpclient.h:172

Removes the relay connection for `peerAddress`, unregisters its callbacks, and deletes the associated [RelayConnectionBinding](#relayconnectionbinding). 
#### Parameters
* `peerAddress` Peer whose connection to release.

---

{#transportprotocol-1}

#### transportProtocol

`virtual` `override`

```cpp
virtual int transportProtocol() override
```

Defined in src/turn/include/icy/turn/client/tcpclient.h:175

#### Returns
kProtocolTCP (6).

##### Reimplements

- [`transportProtocol`](#transportprotocol)

---

{#connections}

#### connections

```cpp
ConnectionManager & connections()
```

Defined in src/turn/include/icy/turn/client/tcpclient.h:178

#### Returns
Reference to the map of active relay connections keyed by peer address.

---

{#classname-6}

#### className

`virtual` `const` `inline`

```cpp
virtual inline const char * className() const
```

Defined in src/turn/include/icy/turn/client/tcpclient.h:180

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `TCPClientObserver &` | [`_observer`](#_observer-3)  |  |
| `ConnectionManager` | [`_connections`](#_connections-2)  |  |

---

{#_observer-3}

#### _observer

```cpp
TCPClientObserver & _observer
```

Defined in src/turn/include/icy/turn/client/tcpclient.h:183

---

{#_connections-2}

#### _connections

```cpp
ConnectionManager _connections
```

Defined in src/turn/include/icy/turn/client/tcpclient.h:184

{#tcpconnectionpair}

## TCPConnectionPair

```cpp
#include <icy/turn/server/tcpconnectionpair.h>
```

```cpp
class TCPConnectionPair
```

Defined in src/turn/include/icy/turn/server/tcpconnectionpair.h:35

> **Inherits:** [`RefCounted< TCPConnectionPair >`](base.md#refcounted)

Paired TCP connections forming a single TURN relay pipe between a client and a peer. Lifetime managed by [IntrusivePtr](base.md#intrusiveptr); the owning [TCPAllocation](#tcpallocation) holds the [Ptr](#ptr-15) in its pair map.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`allocation`](#allocation) | `variable` | Declared here |
| [`client`](#client-4) | `variable` | Declared here |
| [`peer`](#peer) | `variable` | Declared here |
| [`earlyPeerData`](#earlypeerdata) | `variable` | Declared here |
| [`connectionID`](#connectionid-1) | `variable` | Declared here |
| [`isDataConnection`](#isdataconnection) | `variable` | Declared here |
| [`pendingDelete`](#pendingdelete) | `variable` | Declared here |
| [`timeout`](#timeout-3) | `variable` | Declared here |
| [`transactionID`](#transactionid) | `variable` | Declared here |
| [`TCPConnectionPair`](#tcpconnectionpair-1) | `function` | Declared here |
| [`doPeerConnect`](#dopeerconnect) | `function` | Declared here |
| [`makeDataConnection`](#makedataconnection) | `function` | Declared here |
| [`setPeerSocket`](#setpeersocket) | `function` | Declared here |
| [`setClientSocket`](#setclientsocket) | `function` | Declared here |
| [`onPeerConnectSuccess`](#onpeerconnectsuccess) | `function` | Declared here |
| [`onPeerConnectError`](#onpeerconnecterror) | `function` | Declared here |
| [`onClientDataReceived`](#onclientdatareceived) | `function` | Declared here |
| [`onPeerDataReceived`](#onpeerdatareceived) | `function` | Declared here |
| [`onConnectionClosed`](#onconnectionclosed) | `function` | Declared here |
| [`startTimeout`](#starttimeout) | `function` | Declared here |
| [`requestDeletion`](#requestdeletion) | `function` | Declared here |
| [`expired`](#expired-1) | `function` | Declared here |
| [`Ptr`](#ptr-15) | `typedef` | Declared here |
| [`TCPConnectionPair`](#tcpconnectionpair-2) | `function` | Declared here |
| [`TCPConnectionPair`](#tcpconnectionpair-3) | `function` | Declared here |
| [`_refCount`](base.md#classicy_1_1RefCounted_1a9ad69b4e82cc4cce1d55780afea9e589) | `variable` | Inherited from [`RefCounted`](base.md#refcounted) |
| [`RefCounted`](base.md#classicy_1_1RefCounted_1a69db516c7fc776ec189af3cbd4c9804b) | `function` | Inherited from [`RefCounted`](base.md#refcounted) |
| [`RefCounted`](base.md#classicy_1_1RefCounted_1a4c715cce08cc8f065a0740716d828c51) | `function` | Inherited from [`RefCounted`](base.md#refcounted) |
| [`operator=`](base.md#classicy_1_1RefCounted_1a40bc82fe701cbdad57046c7b0ebd782b) | `function` | Inherited from [`RefCounted`](base.md#refcounted) |
| [`addRef`](base.md#classicy_1_1RefCounted_1a8d439708a2a8535bd0f98d58ba8b8950) | `function` | Inherited from [`RefCounted`](base.md#refcounted) |
| [`releaseRef`](base.md#classicy_1_1RefCounted_1a842da5db14ba73d10c95b5be072cbd1f) | `function` | Inherited from [`RefCounted`](base.md#refcounted) |
| [`refCount`](base.md#classicy_1_1RefCounted_1a0bd7a4e6db7b4582fcec1e632901ad2f) | `function` | Inherited from [`RefCounted`](base.md#refcounted) |
| [`~RefCounted`](base.md#classicy_1_1RefCounted_1a605be798c4be05f218ef0e84f5c5e909) | `function` | Inherited from [`RefCounted`](base.md#refcounted) |

### Inherited from [`RefCounted`](base.md#refcounted)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`_refCount`](base.md#classicy_1_1RefCounted_1a9ad69b4e82cc4cce1d55780afea9e589)  |  |
| `function` | [`RefCounted`](base.md#classicy_1_1RefCounted_1a69db516c7fc776ec189af3cbd4c9804b)  | Defaulted constructor. |
| `function` | [`RefCounted`](base.md#classicy_1_1RefCounted_1a4c715cce08cc8f065a0740716d828c51) `inline` `noexcept` |  |
| `function` | [`operator=`](base.md#classicy_1_1RefCounted_1a40bc82fe701cbdad57046c7b0ebd782b) `inline` `noexcept` |  |
| `function` | [`addRef`](base.md#classicy_1_1RefCounted_1a8d439708a2a8535bd0f98d58ba8b8950) `const` `inline` `noexcept` | Increments the reference count. Called by [IntrusivePtr](base.md#intrusiveptr) on acquisition. |
| `function` | [`releaseRef`](base.md#classicy_1_1RefCounted_1a842da5db14ba73d10c95b5be072cbd1f) `const` `inline` `noexcept` | Decrements the reference count. |
| `function` | [`refCount`](base.md#classicy_1_1RefCounted_1a0bd7a4e6db7b4582fcec1e632901ad2f) `const` `inline` `nodiscard` `noexcept` | Returns the current reference count. |
| `function` | [`~RefCounted`](base.md#classicy_1_1RefCounted_1a605be798c4be05f218ef0e84f5c5e909)  | Defaulted destructor. |

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `TCPAllocation &` | [`allocation`](#allocation)  |  |
| `net::SocketEmitter` | [`client`](#client-4)  |  |
| `net::SocketEmitter` | [`peer`](#peer)  |  |
| `Buffer` | [`earlyPeerData`](#earlypeerdata)  |  |
| `uint32_t` | [`connectionID`](#connectionid-1)  |  |
| `bool` | [`isDataConnection`](#isdataconnection)  |  |
| `bool` | [`pendingDelete`](#pendingdelete)  |  |
| `Timeout` | [`timeout`](#timeout-3)  |  |
| `stun::TransactionID` | [`transactionID`](#transactionid)  |  |

---

{#allocation}

#### allocation

```cpp
TCPAllocation & allocation
```

Defined in src/turn/include/icy/turn/server/tcpconnectionpair.h:67

---

{#client-4}

#### client

```cpp
net::SocketEmitter client
```

Defined in src/turn/include/icy/turn/server/tcpconnectionpair.h:69

---

{#peer}

#### peer

```cpp
net::SocketEmitter peer
```

Defined in src/turn/include/icy/turn/server/tcpconnectionpair.h:70

---

{#earlypeerdata}

#### earlyPeerData

```cpp
Buffer earlyPeerData
```

Defined in src/turn/include/icy/turn/server/tcpconnectionpair.h:72

---

{#connectionid-1}

#### connectionID

```cpp
uint32_t connectionID
```

Defined in src/turn/include/icy/turn/server/tcpconnectionpair.h:73

---

{#isdataconnection}

#### isDataConnection

```cpp
bool isDataConnection
```

Defined in src/turn/include/icy/turn/server/tcpconnectionpair.h:74

---

{#pendingdelete}

#### pendingDelete

```cpp
bool pendingDelete = false
```

Defined in src/turn/include/icy/turn/server/tcpconnectionpair.h:75

---

{#timeout-3}

#### timeout

```cpp
Timeout timeout
```

Defined in src/turn/include/icy/turn/server/tcpconnectionpair.h:76

---

{#transactionid}

#### transactionID

```cpp
stun::TransactionID transactionID
```

Defined in src/turn/include/icy/turn/server/tcpconnectionpair.h:77

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`TCPConnectionPair`](#tcpconnectionpair-1)  | Constructs a pair with a randomly assigned connection ID. The caller must add the pair to the allocation's pair map. |
| `bool` | [`doPeerConnect`](#dopeerconnect)  |  |
| `bool` | [`makeDataConnection`](#makedataconnection)  |  |
| `void` | [`setPeerSocket`](#setpeersocket)  |  |
| `void` | [`setClientSocket`](#setclientsocket)  |  |
| `bool` | [`onPeerConnectSuccess`](#onpeerconnectsuccess)  |  |
| `bool` | [`onPeerConnectError`](#onpeerconnecterror)  |  |
| `bool` | [`onClientDataReceived`](#onclientdatareceived)  |  |
| `bool` | [`onPeerDataReceived`](#onpeerdatareceived)  |  |
| `bool` | [`onConnectionClosed`](#onconnectionclosed)  |  |
| `void` | [`startTimeout`](#starttimeout)  |  |
| `void` | [`requestDeletion`](#requestdeletion)  |  |
| `bool` | [`expired`](#expired-1) `const` `nodiscard` |  |

---

{#tcpconnectionpair-1}

#### TCPConnectionPair

```cpp
TCPConnectionPair(TCPAllocation & allocation)
```

Defined in src/turn/include/icy/turn/server/tcpconnectionpair.h:43

Constructs a pair with a randomly assigned connection ID. The caller must add the pair to the allocation's pair map. 
#### Parameters
* `allocation` The [TCPAllocation](#tcpallocation) that owns this pair.

---

{#dopeerconnect}

#### doPeerConnect

```cpp
bool doPeerConnect(const net::Address & peerAddr)
```

Defined in src/turn/include/icy/turn/server/tcpconnectionpair.h:46

---

{#makedataconnection}

#### makeDataConnection

```cpp
bool makeDataConnection()
```

Defined in src/turn/include/icy/turn/server/tcpconnectionpair.h:47

---

{#setpeersocket}

#### setPeerSocket

```cpp
void setPeerSocket(const net::TCPSocket::Ptr & socket)
```

Defined in src/turn/include/icy/turn/server/tcpconnectionpair.h:49

---

{#setclientsocket}

#### setClientSocket

```cpp
void setClientSocket(const net::TCPSocket::Ptr & socket)
```

Defined in src/turn/include/icy/turn/server/tcpconnectionpair.h:50

---

{#onpeerconnectsuccess}

#### onPeerConnectSuccess

```cpp
bool onPeerConnectSuccess(net::Socket & socket)
```

Defined in src/turn/include/icy/turn/server/tcpconnectionpair.h:52

---

{#onpeerconnecterror}

#### onPeerConnectError

```cpp
bool onPeerConnectError(net::Socket & socket, const Error & error)
```

Defined in src/turn/include/icy/turn/server/tcpconnectionpair.h:53

---

{#onclientdatareceived}

#### onClientDataReceived

```cpp
bool onClientDataReceived(net::Socket & socket, const MutableBuffer & buffer, const net::Address & peerAddress)
```

Defined in src/turn/include/icy/turn/server/tcpconnectionpair.h:55

---

{#onpeerdatareceived}

#### onPeerDataReceived

```cpp
bool onPeerDataReceived(net::Socket & socket, const MutableBuffer & buffer, const net::Address & peerAddress)
```

Defined in src/turn/include/icy/turn/server/tcpconnectionpair.h:57

---

{#onconnectionclosed}

#### onConnectionClosed

```cpp
bool onConnectionClosed(net::Socket & socket)
```

Defined in src/turn/include/icy/turn/server/tcpconnectionpair.h:60

---

{#starttimeout}

#### startTimeout

```cpp
void startTimeout()
```

Defined in src/turn/include/icy/turn/server/tcpconnectionpair.h:62

---

{#requestdeletion}

#### requestDeletion

```cpp
void requestDeletion()
```

Defined in src/turn/include/icy/turn/server/tcpconnectionpair.h:63

---

{#expired-1}

#### expired

`const` `nodiscard`

```cpp
[[nodiscard]] bool expired() const
```

Defined in src/turn/include/icy/turn/server/tcpconnectionpair.h:65

### Public Types

| Name | Description |
|------|-------------|
| [`Ptr`](#ptr-15)  |  |

---

{#ptr-15}

#### Ptr

```cpp
using Ptr = IntrusivePtr< TCPConnectionPair >
```

Defined in src/turn/include/icy/turn/server/tcpconnectionpair.h:38

### Private Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`TCPConnectionPair`](#tcpconnectionpair-2)  | Deleted constructor. |
|  | [`TCPConnectionPair`](#tcpconnectionpair-3)  | Deleted constructor. |

---

{#tcpconnectionpair-2}

#### TCPConnectionPair

```cpp
TCPConnectionPair(const TCPConnectionPair &) = delete
```

Defined in src/turn/include/icy/turn/server/tcpconnectionpair.h:80

Deleted constructor.

---

{#tcpconnectionpair-3}

#### TCPConnectionPair

```cpp
TCPConnectionPair(TCPConnectionPair &&) = delete
```

Defined in src/turn/include/icy/turn/server/tcpconnectionpair.h:82

Deleted constructor.

{#udpallocation}

## UDPAllocation

```cpp
#include <icy/turn/server/udpallocation.h>
```

```cpp
class UDPAllocation
```

Defined in src/turn/include/icy/turn/server/udpallocation.h:31

> **Inherits:** [`ServerAllocation`](#serverallocation)

Server-side UDP TURN allocation that relays datagrams to permitted peers. Binds a dedicated UDP relay socket for peer communication and forwards incoming datagrams to the client as Data Indications.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`UDPAllocation`](#udpallocation-1) | `function` | Declared here |
| [`onPeerDataReceived`](#onpeerdatareceived-1) | `function` | Declared here |
| [`handleRequest`](#handlerequest-3) | `function` | Declared here |
| [`handleSendIndication`](#handlesendindication) | `function` | Declared here |
| [`send`](#send-16) | `function` | Declared here |
| [`relayedAddress`](#relayedaddress-3) | `function` | Declared here |
| [`_relaySocket`](#_relaysocket) | `variable` | Declared here |
| [`Server`](#server-8) | `friend` | Inherited from [`ServerAllocation`](#serverallocation) |
| [`ServerAllocation`](#serverallocation-1) | `function` | Inherited from [`ServerAllocation`](#serverallocation) |
| [`handleRequest`](#handlerequest-1) | `function` | Inherited from [`ServerAllocation`](#serverallocation) |
| [`handleRefreshRequest`](#handlerefreshrequest) | `function` | Inherited from [`ServerAllocation`](#serverallocation) |
| [`handleCreatePermission`](#handlecreatepermission) | `function` | Inherited from [`ServerAllocation`](#serverallocation) |
| [`onTimer`](#ontimer-4) | `function` | Inherited from [`ServerAllocation`](#serverallocation) |
| [`timeRemaining`](#timeremaining-1) | `function` | Inherited from [`ServerAllocation`](#serverallocation) |
| [`maxTimeRemaining`](#maxtimeremaining) | `function` | Inherited from [`ServerAllocation`](#serverallocation) |
| [`hasPermission`](#haspermission-4) | `function` | Inherited from [`ServerAllocation`](#serverallocation) |
| [`hasPermission`](#haspermission-5) | `function` | Inherited from [`ServerAllocation`](#serverallocation) |
| [`server`](#server-9) | `function` | Inherited from [`ServerAllocation`](#serverallocation) |
| [`print`](#print-12) | `function` | Inherited from [`ServerAllocation`](#serverallocation) |
| [`refreshDeleteRequested`](#refreshdeleterequested) | `function` | Inherited from [`ServerAllocation`](#serverallocation) |
| [`_maxLifetime`](#_maxlifetime) | `variable` | Inherited from [`ServerAllocation`](#serverallocation) |
| [`_server`](#_server-1) | `variable` | Inherited from [`ServerAllocation`](#serverallocation) |
| [`_permissionPolicy`](#_permissionpolicy) | `variable` | Inherited from [`ServerAllocation`](#serverallocation) |
| [`_refreshDeleteRequested`](#_refreshdeleterequested) | `variable` | Inherited from [`ServerAllocation`](#serverallocation) |
| [`ServerAllocation`](#serverallocation-2) | `function` | Inherited from [`ServerAllocation`](#serverallocation) |
| [`ServerAllocation`](#serverallocation-3) | `function` | Inherited from [`ServerAllocation`](#serverallocation) |
| [`operator<<`](#operator-28) | `friend` | Inherited from [`IAllocation`](#iallocation) |
| [`IAllocation`](#iallocation-1) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`IAllocation`](#iallocation-2) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`IAllocation`](#iallocation-3) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`updateUsage`](#updateusage) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`setLifetime`](#setlifetime) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`setBandwidthLimit`](#setbandwidthlimit) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`expired`](#expired) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`deleted`](#deleted-1) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`bandwidthLimit`](#bandwidthlimit) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`bandwidthUsed`](#bandwidthused) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`bandwidthRemaining`](#bandwidthremaining) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`timeRemaining`](#timeremaining) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`tuple`](#tuple) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`username`](#username-3) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`lifetime`](#lifetime-1) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`permissions`](#permissions) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`relayedAddress`](#relayedaddress-1) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`addPermission`](#addpermission-3) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`addPermission`](#addpermission-4) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`addPermissions`](#addpermissions) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`removePermission`](#removepermission-2) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`removePermission`](#removepermission-3) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`removeAllPermissions`](#removeallpermissions) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`removeExpiredPermissions`](#removeexpiredpermissions) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`hasPermission`](#haspermission-2) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`hasPermission`](#haspermission-3) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`print`](#print-11) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`_tuple`](#_tuple) | `variable` | Inherited from [`IAllocation`](#iallocation) |
| [`_username`](#_username-2) | `variable` | Inherited from [`IAllocation`](#iallocation) |
| [`_permissions`](#_permissions) | `variable` | Inherited from [`IAllocation`](#iallocation) |
| [`_lifetime`](#_lifetime) | `variable` | Inherited from [`IAllocation`](#iallocation) |
| [`_bandwidthLimit`](#_bandwidthlimit) | `variable` | Inherited from [`IAllocation`](#iallocation) |
| [`_bandwidthUsed`](#_bandwidthused) | `variable` | Inherited from [`IAllocation`](#iallocation) |
| [`_createdAt`](#_createdat) | `variable` | Inherited from [`IAllocation`](#iallocation) |
| [`_updatedAt`](#_updatedat) | `variable` | Inherited from [`IAllocation`](#iallocation) |
| [`_deleted`](#_deleted) | `variable` | Inherited from [`IAllocation`](#iallocation) |

### Inherited from [`ServerAllocation`](#serverallocation)

| Kind | Name | Description |
|------|------|-------------|
| `friend` | [`Server`](#server-8)  |  |
| `function` | [`ServerAllocation`](#serverallocation-1)  |  |
| `function` | [`handleRequest`](#handlerequest-1) `virtual` | Dispatches incoming STUN requests to [handleCreatePermission()](#handlecreatepermission) or [handleRefreshRequest()](#handlerefreshrequest). Subclasses override to add transport-specific methods. |
| `function` | [`handleRefreshRequest`](#handlerefreshrequest) `virtual` | Processes a Refresh request; updates the lifetime or sets the delete flag if the requested lifetime is 0, then sends a success response. |
| `function` | [`handleCreatePermission`](#handlecreatepermission) `virtual` | Processes a CreatePermission request; installs or refreshes permissions for each XOR-PEER-ADDRESS attribute, then sends a success response. |
| `function` | [`onTimer`](#ontimer-4) `nodiscard` | Periodic maintenance callback called by [Server::onTimer()](#ontimer-3). Removes expired permissions. Returns false to signal that this allocation should be destroyed. |
| `function` | [`timeRemaining`](#timeremaining-1) `virtual` `const` `nodiscard` `override` |  |
| `function` | [`maxTimeRemaining`](#maxtimeremaining) `const` `nodiscard` |  |
| `function` | [`hasPermission`](#haspermission-4) `virtual` `nodiscard` `override` | Checks permission, auto-granting local IPs if enabled in server options. |
| `function` | [`hasPermission`](#haspermission-5) `virtual` `nodiscard` `override` | Checks whether a permission exists for `peerAddress`. The port is ignored; TURN permissions are IP-only. |
| `function` | [`server`](#server-9) `virtual` |  |
| `function` | [`print`](#print-12) `virtual` `const` `override` | Writes a detailed diagnostic dump of this allocation to `os`. |
| `function` | [`refreshDeleteRequested`](#refreshdeleterequested) `const` `inline` `nodiscard` | Returns true if the refresh request set lifetime to 0, signaling the parent [Server](#server-6) to delete this allocation. |
| `variable` | [`_maxLifetime`](#_maxlifetime)  |  |
| `variable` | [`_server`](#_server-1)  |  |
| `variable` | [`_permissionPolicy`](#_permissionpolicy)  |  |
| `variable` | [`_refreshDeleteRequested`](#_refreshdeleterequested)  |  |
| `function` | [`ServerAllocation`](#serverallocation-2)  | NonCopyable and NonMovable. |
| `function` | [`ServerAllocation`](#serverallocation-3)  | Deleted constructor. |

### Inherited from [`IAllocation`](#iallocation)

| Kind | Name | Description |
|------|------|-------------|
| `friend` | [`operator<<`](#operator-28) `inline` |  |
| `function` | [`IAllocation`](#iallocation-1)  |  |
| `function` | [`IAllocation`](#iallocation-2)  | Deleted constructor. |
| `function` | [`IAllocation`](#iallocation-3)  | Deleted constructor. |
| `function` | [`updateUsage`](#updateusage) `virtual` | Updates the last-activity timestamp and accumulates bandwidth usage. Call this whenever data is relayed through the allocation. |
| `function` | [`setLifetime`](#setlifetime) `virtual` | Sets the allocation lifetime in seconds and resets the activity timestamp, effectively extending the expiry from the current moment. |
| `function` | [`setBandwidthLimit`](#setbandwidthlimit) `virtual` | Sets the maximum number of bytes this allocation may relay in its lifetime. Pass 0 to disable bandwidth limiting. |
| `function` | [`expired`](#expired) `virtual` `const` `nodiscard` |  |
| `function` | [`deleted`](#deleted-1) `virtual` `const` `nodiscard` | Returns true if the allocation's deleted flag is set and or if the allocation has expired. |
| `function` | [`bandwidthLimit`](#bandwidthlimit) `virtual` `const` `nodiscard` |  |
| `function` | [`bandwidthUsed`](#bandwidthused) `virtual` `const` `nodiscard` |  |
| `function` | [`bandwidthRemaining`](#bandwidthremaining) `virtual` `const` `nodiscard` |  |
| `function` | [`timeRemaining`](#timeremaining) `virtual` `const` `nodiscard` |  |
| `function` | [`tuple`](#tuple) `virtual` |  |
| `function` | [`username`](#username-3) `virtual` `const` `nodiscard` |  |
| `function` | [`lifetime`](#lifetime-1) `virtual` `const` `nodiscard` |  |
| `function` | [`permissions`](#permissions) `virtual` `const` `nodiscard` |  |
| `function` | [`relayedAddress`](#relayedaddress-1) `virtual` `const` `nodiscard` |  |
| `function` | [`addPermission`](#addpermission-3) `virtual` | Adds a permission for `ip`, or refreshes the existing one. |
| `function` | [`addPermission`](#addpermission-4) `virtual` | Adds a permission for `address`, or refreshes the existing one. The port is ignored; TURN permissions are IP-only. |
| `function` | [`addPermissions`](#addpermissions) `virtual` | Adds (or refreshes) permissions for multiple IPs. |
| `function` | [`removePermission`](#removepermission-2) `virtual` | Removes the permission for `ip` if present. |
| `function` | [`removePermission`](#removepermission-3) `virtual` | Removes the permission for `address` if present. The port is ignored; TURN permissions are IP-only. |
| `function` | [`removeAllPermissions`](#removeallpermissions) `virtual` | Removes all permissions from the list. |
| `function` | [`removeExpiredPermissions`](#removeexpiredpermissions) `virtual` | Removes any permissions whose 5-minute lifetime has elapsed. |
| `function` | [`hasPermission`](#haspermission-2) `virtual` `nodiscard` | Checks whether a permission exists for `peerIP`. Local addresses (192.168.x.x and 127.x.x.x) are always permitted. |
| `function` | [`hasPermission`](#haspermission-3) `virtual` `nodiscard` | Checks whether a permission exists for `peerAddress`. The port is ignored; TURN permissions are IP-only. |
| `function` | [`print`](#print-11) `virtual` `const` `inline` |  |
| `variable` | [`_tuple`](#_tuple)  |  |
| `variable` | [`_username`](#_username-2)  |  |
| `variable` | [`_permissions`](#_permissions)  |  |
| `variable` | [`_lifetime`](#_lifetime)  |  |
| `variable` | [`_bandwidthLimit`](#_bandwidthlimit)  |  |
| `variable` | [`_bandwidthUsed`](#_bandwidthused)  |  |
| `variable` | [`_createdAt`](#_createdat)  |  |
| `variable` | [`_updatedAt`](#_updatedat)  |  |
| `variable` | [`_deleted`](#_deleted)  |  |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`UDPAllocation`](#udpallocation-1)  | Binds the relay UDP socket and registers the peer receive callback. |
| `bool` | [`onPeerDataReceived`](#onpeerdatareceived-1)  | Receive callback for the relay UDP socket; validates permissions, updates bandwidth usage, and forwards data to the client as a Data Indication. |
| `bool` | [`handleRequest`](#handlerequest-3) `virtual` | Extends the base handler with Send Indication support. |
| `void` | [`handleSendIndication`](#handlesendindication)  | Handles a Send Indication from the client; validates permissions and relays the DATA attribute payload to the peer via the relay socket. Silently discards on permission or attribute errors. |
| `ssize_t` | [`send`](#send-16)  | Sends raw data to `peerAddress` via the relay UDP socket. Updates bandwidth usage and returns -1 if the allocation is expired. |
| `net::Address` | [`relayedAddress`](#relayedaddress-3) `virtual` `const` `nodiscard` |  |

---

{#udpallocation-1}

#### UDPAllocation

```cpp
UDPAllocation(Server & server, const FiveTuple & tuple, const std::string & username, const uint32_t & lifetime)
```

Defined in src/turn/include/icy/turn/server/udpallocation.h:39

Binds the relay UDP socket and registers the peer receive callback. 
#### Parameters
* `server` Owning TURN server. 

* `tuple` 5-tuple identifying this allocation. 

* `username` Authenticated username from the Allocate request. 

* `lifetime` Initial lifetime in seconds.

---

{#onpeerdatareceived-1}

#### onPeerDataReceived

```cpp
bool onPeerDataReceived(net::Socket & socket, const MutableBuffer & buffer, const net::Address & peerAddress)
```

Defined in src/turn/include/icy/turn/server/udpallocation.h:48

Receive callback for the relay UDP socket; validates permissions, updates bandwidth usage, and forwards data to the client as a Data Indication. 
#### Parameters
* `socket` The relay socket. 

* `buffer` Received datagram. 

* `peerAddress` Source address of the peer datagram.

---

{#handlerequest-3}

#### handleRequest

`virtual`

```cpp
virtual bool handleRequest(Request & request)
```

Defined in src/turn/include/icy/turn/server/udpallocation.h:54

Extends the base handler with Send Indication support. 
#### Parameters
* `request` Incoming STUN request or indication. 

#### Returns
true if handled.

##### Reimplements

- [`handleRequest`](#handlerequest-1)

---

{#handlesendindication}

#### handleSendIndication

```cpp
void handleSendIndication(Request & request)
```

Defined in src/turn/include/icy/turn/server/udpallocation.h:60

Handles a Send Indication from the client; validates permissions and relays the DATA attribute payload to the peer via the relay socket. Silently discards on permission or attribute errors. 
#### Parameters
* `request` Incoming Send Indication.

---

{#send-16}

#### send

```cpp
ssize_t send(const char * data, size_t size, const net::Address & peerAddress)
```

Defined in src/turn/include/icy/turn/server/udpallocation.h:68

Sends raw data to `peerAddress` via the relay UDP socket. Updates bandwidth usage and returns -1 if the allocation is expired. 
#### Parameters
* `data` Payload bytes to relay. 

* `size` Number of bytes. 

* `peerAddress` Destination peer address. 

#### Returns
Number of bytes sent, or -1 if the quota is exhausted.

---

{#relayedaddress-3}

#### relayedAddress

`virtual` `const` `nodiscard`

```cpp
[[nodiscard]] virtual net::Address relayedAddress() const
```

Defined in src/turn/include/icy/turn/server/udpallocation.h:72

#### Returns
The bound address of the relay UDP socket.

##### Reimplements

- [`relayedAddress`](#relayedaddress-1)

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `net::SocketEmitter` | [`_relaySocket`](#_relaysocket)  |  |

---

{#_relaysocket}

#### _relaySocket

```cpp
net::SocketEmitter _relaySocket
```

Defined in src/turn/include/icy/turn/server/udpallocation.h:75

{#udpclient}

## UDPClient

```cpp
#include <icy/turn/client/udpclient.h>
```

```cpp
class UDPClient
```

Defined in src/turn/include/icy/turn/client/udpclient.h:31

> **Inherits:** [`Client`](#client-2)

UDP transport TURN client for datagram-based relay allocations. Uses a single UDP socket for both STUN signalling and Send/Data Indications. All [Client](#client-2) methods apply; data is relayed via [sendData()](#senddata) using Send Indications.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`UDPClient`](#udpclient-1) | `function` | Declared here |
| [`Client`](#client-3) | `function` | Inherited from [`Client`](#client-2) |
| [`start`](#start-13) | `function` | Inherited from [`Client`](#client-2) |
| [`stop`](#stop-10) | `function` | Inherited from [`Client`](#client-2) |
| [`sendAllocate`](#sendallocate) | `function` | Inherited from [`Client`](#client-2) |
| [`addPermission`](#addpermission) | `function` | Inherited from [`Client`](#client-2) |
| [`addPermission`](#addpermission-1) | `function` | Inherited from [`Client`](#client-2) |
| [`sendCreatePermission`](#sendcreatepermission) | `function` | Inherited from [`Client`](#client-2) |
| [`sendChannelBind`](#sendchannelbind) | `function` | Inherited from [`Client`](#client-2) |
| [`sendRefresh`](#sendrefresh) | `function` | Inherited from [`Client`](#client-2) |
| [`sendData`](#senddata) | `function` | Inherited from [`Client`](#client-2) |
| [`handleResponse`](#handleresponse) | `function` | Inherited from [`Client`](#client-2) |
| [`handleAllocateResponse`](#handleallocateresponse) | `function` | Inherited from [`Client`](#client-2) |
| [`handleAllocateErrorResponse`](#handleallocateerrorresponse) | `function` | Inherited from [`Client`](#client-2) |
| [`handleCreatePermissionResponse`](#handlecreatepermissionresponse) | `function` | Inherited from [`Client`](#client-2) |
| [`handleCreatePermissionErrorResponse`](#handlecreatepermissionerrorresponse) | `function` | Inherited from [`Client`](#client-2) |
| [`handleRefreshResponse`](#handlerefreshresponse) | `function` | Inherited from [`Client`](#client-2) |
| [`handleDataIndication`](#handledataindication) | `function` | Inherited from [`Client`](#client-2) |
| [`transportProtocol`](#transportprotocol) | `function` | Inherited from [`Client`](#client-2) |
| [`createTransaction`](#createtransaction) | `function` | Inherited from [`Client`](#client-2) |
| [`authenticateRequest`](#authenticaterequest) | `function` | Inherited from [`Client`](#client-2) |
| [`sendAuthenticatedTransaction`](#sendauthenticatedtransaction) | `function` | Inherited from [`Client`](#client-2) |
| [`removeTransaction`](#removetransaction) | `function` | Inherited from [`Client`](#client-2) |
| [`mappedAddress`](#mappedaddress) | `function` | Inherited from [`Client`](#client-2) |
| [`relayedAddress`](#relayedaddress) | `function` | Inherited from [`Client`](#client-2) |
| [`closed`](#closed-5) | `function` | Inherited from [`Client`](#client-2) |
| [`observer`](#observer-1) | `function` | Inherited from [`Client`](#client-2) |
| [`options`](#options-8) | `function` | Inherited from [`Client`](#client-2) |
| [`addPermission`](#addpermission-2) | `function` | Inherited from [`Client`](#client-2) |
| [`hasPermission`](#haspermission) | `function` | Inherited from [`Client`](#client-2) |
| [`hasPermission`](#haspermission-1) | `function` | Inherited from [`Client`](#client-2) |
| [`removePermission`](#removepermission) | `function` | Inherited from [`Client`](#client-2) |
| [`removePermission`](#removepermission-1) | `function` | Inherited from [`Client`](#client-2) |
| [`_observer`](#_observer-1) | `variable` | Inherited from [`Client`](#client-2) |
| [`_options`](#_options-3) | `variable` | Inherited from [`Client`](#client-2) |
| [`_socket`](#_socket-3) | `variable` | Inherited from [`Client`](#client-2) |
| [`_timer`](#_timer-1) | `variable` | Inherited from [`Client`](#client-2) |
| [`_error`](#_error-5) | `variable` | Inherited from [`Client`](#client-2) |
| [`_mappedAddress`](#_mappedaddress) | `variable` | Inherited from [`Client`](#client-2) |
| [`_relayedAddress`](#_relayedaddress) | `variable` | Inherited from [`Client`](#client-2) |
| [`_realm`](#_realm) | `variable` | Inherited from [`Client`](#client-2) |
| [`_nonce`](#_nonce) | `variable` | Inherited from [`Client`](#client-2) |
| [`_pendingIndications`](#_pendingindications) | `variable` | Inherited from [`Client`](#client-2) |
| [`_transactions`](#_transactions) | `variable` | Inherited from [`Client`](#client-2) |
| [`setError`](#seterror-5) | `function` | Inherited from [`Client`](#client-2) |
| [`onSocketConnect`](#onsocketconnect-5) | `function` | Inherited from [`Client`](#client-2) |
| [`onSocketRecv`](#onsocketrecv-7) | `function` | Inherited from [`Client`](#client-2) |
| [`onSocketClose`](#onsocketclose-5) | `function` | Inherited from [`Client`](#client-2) |
| [`onTransactionProgress`](#ontransactionprogress) | `function` | Inherited from [`Client`](#client-2) |
| [`onStateChange`](#onstatechange-1) | `function` | Inherited from [`Client`](#client-2) |
| [`onTimer`](#ontimer-1) | `function` | Inherited from [`Client`](#client-2) |
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
| [`operator<<`](#operator-28) | `friend` | Inherited from [`IAllocation`](#iallocation) |
| [`IAllocation`](#iallocation-1) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`IAllocation`](#iallocation-2) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`IAllocation`](#iallocation-3) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`updateUsage`](#updateusage) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`setLifetime`](#setlifetime) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`setBandwidthLimit`](#setbandwidthlimit) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`expired`](#expired) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`deleted`](#deleted-1) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`bandwidthLimit`](#bandwidthlimit) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`bandwidthUsed`](#bandwidthused) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`bandwidthRemaining`](#bandwidthremaining) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`timeRemaining`](#timeremaining) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`tuple`](#tuple) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`username`](#username-3) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`lifetime`](#lifetime-1) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`permissions`](#permissions) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`relayedAddress`](#relayedaddress-1) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`addPermission`](#addpermission-3) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`addPermission`](#addpermission-4) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`addPermissions`](#addpermissions) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`removePermission`](#removepermission-2) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`removePermission`](#removepermission-3) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`removeAllPermissions`](#removeallpermissions) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`removeExpiredPermissions`](#removeexpiredpermissions) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`hasPermission`](#haspermission-2) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`hasPermission`](#haspermission-3) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`print`](#print-11) | `function` | Inherited from [`IAllocation`](#iallocation) |
| [`_tuple`](#_tuple) | `variable` | Inherited from [`IAllocation`](#iallocation) |
| [`_username`](#_username-2) | `variable` | Inherited from [`IAllocation`](#iallocation) |
| [`_permissions`](#_permissions) | `variable` | Inherited from [`IAllocation`](#iallocation) |
| [`_lifetime`](#_lifetime) | `variable` | Inherited from [`IAllocation`](#iallocation) |
| [`_bandwidthLimit`](#_bandwidthlimit) | `variable` | Inherited from [`IAllocation`](#iallocation) |
| [`_bandwidthUsed`](#_bandwidthused) | `variable` | Inherited from [`IAllocation`](#iallocation) |
| [`_createdAt`](#_createdat) | `variable` | Inherited from [`IAllocation`](#iallocation) |
| [`_updatedAt`](#_updatedat) | `variable` | Inherited from [`IAllocation`](#iallocation) |
| [`_deleted`](#_deleted) | `variable` | Inherited from [`IAllocation`](#iallocation) |

### Inherited from [`Client`](#client-2)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`Client`](#client-3)  |  |
| `function` | [`start`](#start-13) `virtual` | Connects the socket to the TURN server and starts the allocation sequence. Permissions must be added via [addPermission()](#addpermission) before calling this. |
| `function` | [`stop`](#stop-10) `virtual` | Stops the timer, cancels pending transactions, and closes the socket. |
| `function` | [`sendAllocate`](#sendallocate) `virtual` | Sends an Allocate request to the server with the configured transport and lifetime. On first call the server will typically respond with a 401 challenge; the client re-sends with credentials automatically. |
| `function` | [`addPermission`](#addpermission) `virtual` | Adds multiple peer IP addresses to the permission list. |
| `function` | [`addPermission`](#addpermission-1) `virtual` `override` | Adds a single peer IP to the permission list, or refreshes it if already present. Permissions should be added before [start()](#start-13); they may also be added later, in which case a new CreatePermission request is required. |
| `function` | [`sendCreatePermission`](#sendcreatepermission) `virtual` | Sends a CreatePermission request for all IPs currently in the permission list. Called automatically after allocation succeeds and periodically by the timer to refresh expiring permissions. |
| `function` | [`sendChannelBind`](#sendchannelbind) `virtual` | Channel bindings (RFC 5766 Section 11) are intentionally not implemented. They are a bandwidth optimization that replaces STUN-framed Send/Data indications with a compact 4-byte ChannelData header. This only benefits high-throughput media relay scenarios; in practice, media flows directly via ICE/DTLS rather than through this TURN client's data path, so the optimization is not worth the complexity (channel number allocation, 10-minute binding refresh timers, ChannelData wire framing). Data relay uses [sendData()](#senddata) with Send Indications instead. |
| `function` | [`sendRefresh`](#sendrefresh) `virtual` | Sends a Refresh request to extend the allocation lifetime. Called automatically by the timer when roughly one-third of the lifetime remains. |
| `function` | [`sendData`](#senddata) `virtual` | Sends a Send Indication to relay `data` to `peerAddress` through the TURN server. If permissions are still being negotiated the indication is queued and flushed once CreatePermission succeeds. |
| `function` | [`handleResponse`](#handleresponse) `virtual` | Dispatches an incoming STUN/TURN response to the appropriate handler. |
| `function` | [`handleAllocateResponse`](#handleallocateresponse) `virtual` | Processes a successful Allocate response; extracts mapped/relayed addresses and advances the state to Authorizing, then sends CreatePermission. |
| `function` | [`handleAllocateErrorResponse`](#handleallocateerrorresponse) `virtual` | Handles an Allocate error response; manages the 401 challenge/re-send flow and sets the client to Failed for unrecoverable errors. |
| `function` | [`handleCreatePermissionResponse`](#handlecreatepermissionresponse) `virtual` | Handles a successful CreatePermission response; flushes queued Send Indications and advances state to Success. |
| `function` | [`handleCreatePermissionErrorResponse`](#handlecreatepermissionerrorresponse) `virtual` | Handles a failed CreatePermission response; clears all permissions and sets the client to Failed. |
| `function` | [`handleRefreshResponse`](#handlerefreshresponse) `virtual` | Handles a Refresh response; updates the stored lifetime. |
| `function` | [`handleDataIndication`](#handledataindication) `virtual` | Handles an incoming Data Indication; extracts peer address and data and forwards to [ClientObserver::onRelayDataReceived()](#onrelaydatareceived). |
| `function` | [`transportProtocol`](#transportprotocol) `virtual` |  |
| `function` | [`createTransaction`](#createtransaction) `virtual` | Creates a new STUN transaction, registers the progress callback, and adds it to the active transaction list. |
| `function` | [`authenticateRequest`](#authenticaterequest) `virtual` | Adds STUN long-term credential attributes (Username, Realm, Nonce, MessageIntegrity) to `request` if the realm has been received from the server. |
| `function` | [`sendAuthenticatedTransaction`](#sendauthenticatedtransaction) `virtual` | Calls [authenticateRequest()](#authenticaterequest) then sends the transaction. |
| `function` | [`removeTransaction`](#removetransaction) `virtual` | Removes a transaction from the active list. The [IntrusivePtr](base.md#intrusiveptr) keeps the object alive until the caller's copy is released. |
| `function` | [`mappedAddress`](#mappedaddress) `const` `nodiscard` |  |
| `function` | [`relayedAddress`](#relayedaddress) `virtual` `const` `nodiscard` `override` |  |
| `function` | [`closed`](#closed-5) `const` `nodiscard` |  |
| `function` | [`observer`](#observer-1)  |  |
| `function` | [`options`](#options-8) `const` `nodiscard` |  |
| `function` | [`addPermission`](#addpermission-2) `virtual` | Adds a permission for `address`, or refreshes the existing one. The port is ignored; TURN permissions are IP-only. |
| `function` | [`hasPermission`](#haspermission) `virtual` `nodiscard` | Checks whether a permission exists for `peerIP`. Local addresses (192.168.x.x and 127.x.x.x) are always permitted. |
| `function` | [`hasPermission`](#haspermission-1) `virtual` `nodiscard` | Checks whether a permission exists for `peerAddress`. The port is ignored; TURN permissions are IP-only. |
| `function` | [`removePermission`](#removepermission) `virtual` | Removes the permission for `ip` if present. |
| `function` | [`removePermission`](#removepermission-1) `virtual` | Removes the permission for `address` if present. The port is ignored; TURN permissions are IP-only. |
| `variable` | [`_observer`](#_observer-1)  |  |
| `variable` | [`_options`](#_options-3)  |  |
| `variable` | [`_socket`](#_socket-3)  |  |
| `variable` | [`_timer`](#_timer-1)  |  |
| `variable` | [`_error`](#_error-5)  |  |
| `variable` | [`_mappedAddress`](#_mappedaddress)  |  |
| `variable` | [`_relayedAddress`](#_relayedaddress)  |  |
| `variable` | [`_realm`](#_realm)  |  |
| `variable` | [`_nonce`](#_nonce)  |  |
| `variable` | [`_pendingIndications`](#_pendingindications)  | A list of queued Send indication packets awaiting server permissions. |
| `variable` | [`_transactions`](#_transactions)  | A list containing currently active transactions. |
| `function` | [`setError`](#seterror-5) `virtual` | Sets the error field and transitions the client to the Failed state. |
| `function` | [`onSocketConnect`](#onsocketconnect-5) `virtual` | Socket connect callback; starts the timer and sends the first Allocate request. |
| `function` | [`onSocketRecv`](#onsocketrecv-7) `virtual` | Socket receive callback; parses STUN messages from the buffer and dispatches them. |
| `function` | [`onSocketClose`](#onsocketclose-5) `virtual` | Socket close callback; shuts down the client and records the socket error. |
| `function` | [`onTransactionProgress`](#ontransactionprogress) `virtual` | STUN transaction state-change callback; handles Success and Failed outcomes. |
| `function` | [`onStateChange`](#onstatechange-1) `override` | Forwards state-change events to the observer. |
| `function` | [`onTimer`](#ontimer-1) `virtual` | Periodic timer callback; re-allocates on expiry or refreshes when lifetime is below one-third remaining. Also calls [ClientObserver::onTimer()](#ontimer-2). |

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

### Inherited from [`IAllocation`](#iallocation)

| Kind | Name | Description |
|------|------|-------------|
| `friend` | [`operator<<`](#operator-28) `inline` |  |
| `function` | [`IAllocation`](#iallocation-1)  |  |
| `function` | [`IAllocation`](#iallocation-2)  | Deleted constructor. |
| `function` | [`IAllocation`](#iallocation-3)  | Deleted constructor. |
| `function` | [`updateUsage`](#updateusage) `virtual` | Updates the last-activity timestamp and accumulates bandwidth usage. Call this whenever data is relayed through the allocation. |
| `function` | [`setLifetime`](#setlifetime) `virtual` | Sets the allocation lifetime in seconds and resets the activity timestamp, effectively extending the expiry from the current moment. |
| `function` | [`setBandwidthLimit`](#setbandwidthlimit) `virtual` | Sets the maximum number of bytes this allocation may relay in its lifetime. Pass 0 to disable bandwidth limiting. |
| `function` | [`expired`](#expired) `virtual` `const` `nodiscard` |  |
| `function` | [`deleted`](#deleted-1) `virtual` `const` `nodiscard` | Returns true if the allocation's deleted flag is set and or if the allocation has expired. |
| `function` | [`bandwidthLimit`](#bandwidthlimit) `virtual` `const` `nodiscard` |  |
| `function` | [`bandwidthUsed`](#bandwidthused) `virtual` `const` `nodiscard` |  |
| `function` | [`bandwidthRemaining`](#bandwidthremaining) `virtual` `const` `nodiscard` |  |
| `function` | [`timeRemaining`](#timeremaining) `virtual` `const` `nodiscard` |  |
| `function` | [`tuple`](#tuple) `virtual` |  |
| `function` | [`username`](#username-3) `virtual` `const` `nodiscard` |  |
| `function` | [`lifetime`](#lifetime-1) `virtual` `const` `nodiscard` |  |
| `function` | [`permissions`](#permissions) `virtual` `const` `nodiscard` |  |
| `function` | [`relayedAddress`](#relayedaddress-1) `virtual` `const` `nodiscard` |  |
| `function` | [`addPermission`](#addpermission-3) `virtual` | Adds a permission for `ip`, or refreshes the existing one. |
| `function` | [`addPermission`](#addpermission-4) `virtual` | Adds a permission for `address`, or refreshes the existing one. The port is ignored; TURN permissions are IP-only. |
| `function` | [`addPermissions`](#addpermissions) `virtual` | Adds (or refreshes) permissions for multiple IPs. |
| `function` | [`removePermission`](#removepermission-2) `virtual` | Removes the permission for `ip` if present. |
| `function` | [`removePermission`](#removepermission-3) `virtual` | Removes the permission for `address` if present. The port is ignored; TURN permissions are IP-only. |
| `function` | [`removeAllPermissions`](#removeallpermissions) `virtual` | Removes all permissions from the list. |
| `function` | [`removeExpiredPermissions`](#removeexpiredpermissions) `virtual` | Removes any permissions whose 5-minute lifetime has elapsed. |
| `function` | [`hasPermission`](#haspermission-2) `virtual` `nodiscard` | Checks whether a permission exists for `peerIP`. Local addresses (192.168.x.x and 127.x.x.x) are always permitted. |
| `function` | [`hasPermission`](#haspermission-3) `virtual` `nodiscard` | Checks whether a permission exists for `peerAddress`. The port is ignored; TURN permissions are IP-only. |
| `function` | [`print`](#print-11) `virtual` `const` `inline` |  |
| `variable` | [`_tuple`](#_tuple)  |  |
| `variable` | [`_username`](#_username-2)  |  |
| `variable` | [`_permissions`](#_permissions)  |  |
| `variable` | [`_lifetime`](#_lifetime)  |  |
| `variable` | [`_bandwidthLimit`](#_bandwidthlimit)  |  |
| `variable` | [`_bandwidthUsed`](#_bandwidthused)  |  |
| `variable` | [`_createdAt`](#_createdat)  |  |
| `variable` | [`_updatedAt`](#_updatedat)  |  |
| `variable` | [`_deleted`](#_deleted)  |  |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`UDPClient`](#udpclient-1)  |  |

---

{#udpclient-1}

#### UDPClient

```cpp
UDPClient(ClientObserver & observer, const Options & options = Options())
```

Defined in src/turn/include/icy/turn/client/udpclient.h:36

#### Parameters
* `observer` Observer for allocation lifecycle and data relay events. 

* `options` [Client](#client-2) configuration; defaults to loopback server, 5-min lifetime.

{#clientobserver}

## ClientObserver

```cpp
#include <icy/turn/client/client.h>
```

```cpp
struct ClientObserver
```

Defined in src/turn/include/icy/turn/client/client.h:69

> **Subclassed by:** [`TCPClientObserver`](#tcpclientobserver)

Abstract observer interface for TURN client events. Callers subclass this to receive allocation lifecycle and data relay callbacks.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`onClientStateChange`](#onclientstatechange) | `function` | Declared here |
| [`onRelayDataReceived`](#onrelaydatareceived) | `function` | Declared here |
| [`onAllocationFailed`](#onallocationfailed) | `function` | Declared here |
| [`onAllocationDeleted`](#onallocationdeleted) | `function` | Declared here |
| [`onAllocationPermissionsCreated`](#onallocationpermissionscreated) | `function` | Declared here |
| [`onTransactionResponse`](#ontransactionresponse) | `function` | Declared here |
| [`onTimer`](#ontimer-2) | `function` | Declared here |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`onClientStateChange`](#onclientstatechange) `virtual` | Called whenever the client's state machine transitions to a new state. |
| `void` | [`onRelayDataReceived`](#onrelaydatareceived) `virtual` | Called when relayed data is received from a peer via a Data Indication. |
| `void` | [`onAllocationFailed`](#onallocationfailed) `virtual` `inline` | Called when the server rejects an Allocate request with an error. |
| `void` | [`onAllocationDeleted`](#onallocationdeleted) `virtual` `inline` | Called when the server confirms deletion of the allocation. |
| `void` | [`onAllocationPermissionsCreated`](#onallocationpermissionscreated) `virtual` `inline` | Called after a CreatePermission request succeeds. |
| `void` | [`onTransactionResponse`](#ontransactionresponse) `virtual` `inline` | All received transaction responses will be routed here after local processing so the observer can easily implement extra functionality. |
| `void` | [`onTimer`](#ontimer-2) `virtual` `inline` | Fires after the client's internal timer callback. Handy for performing extra async cleanup tasks. |

---

{#onclientstatechange}

#### onClientStateChange

`virtual`

```cpp
virtual void onClientStateChange(Client & client, ClientState & state, const ClientState & oldState)
```

Defined in src/turn/include/icy/turn/client/client.h:75

Called whenever the client's state machine transitions to a new state. 
#### Parameters
* `client` The client that changed state. 

* `state` The new state. 

* `oldState` The previous state.

---

{#onrelaydatareceived}

#### onRelayDataReceived

`virtual`

```cpp
virtual void onRelayDataReceived(Client & client, const char * data, size_t size, const net::Address & peerAddress)
```

Defined in src/turn/include/icy/turn/client/client.h:82

Called when relayed data is received from a peer via a Data Indication. 
#### Parameters
* `client` The owning client. 

* `data` Pointer to the received payload bytes. 

* `size` Number of bytes in `data`. 

* `peerAddress` Source address of the remote peer.

---

{#onallocationfailed}

#### onAllocationFailed

`virtual` `inline`

```cpp
virtual inline void onAllocationFailed(Client & client, int errorCode, const std::string & reason)
```

Defined in src/turn/include/icy/turn/client/client.h:88

Called when the server rejects an Allocate request with an error. 
#### Parameters
* `client` The owning client. 

* `errorCode` STUN error code (e.g. 401, 438). 

* `reason` Human-readable error reason string.

---

{#onallocationdeleted}

#### onAllocationDeleted

`virtual` `inline`

```cpp
virtual inline void onAllocationDeleted(Client & client, const stun::Transaction & transaction)
```

Defined in src/turn/include/icy/turn/client/client.h:93

Called when the server confirms deletion of the allocation. 
#### Parameters
* `client` The owning client. 

* `transaction` The completed Refresh transaction (lifetime == 0).

---

{#onallocationpermissionscreated}

#### onAllocationPermissionsCreated

`virtual` `inline`

```cpp
virtual inline void onAllocationPermissionsCreated(Client & client, const PermissionList & permissions)
```

Defined in src/turn/include/icy/turn/client/client.h:98

Called after a CreatePermission request succeeds. 
#### Parameters
* `client` The owning client. 

* `permissions` The full list of active permissions after this update.

---

{#ontransactionresponse}

#### onTransactionResponse

`virtual` `inline`

```cpp
virtual inline void onTransactionResponse(Client & client, const stun::Transaction & transaction)
```

Defined in src/turn/include/icy/turn/client/client.h:104

All received transaction responses will be routed here after local processing so the observer can easily implement extra functionality. 
#### Parameters
* `client` The owning client. 

* `transaction` The completed transaction (request + response accessible).

---

{#ontimer-2}

#### onTimer

`virtual` `inline`

```cpp
virtual inline void onTimer(Client & client)
```

Defined in src/turn/include/icy/turn/client/client.h:109

Fires after the client's internal timer callback. Handy for performing extra async cleanup tasks. 
#### Parameters
* `client` The owning client.

{#clientstate}

## ClientState

```cpp
#include <icy/turn/client/client.h>
```

```cpp
struct ClientState
```

Defined in src/turn/include/icy/turn/client/client.h:30

> **Inherits:** [`State`](base.md#state)

[State](base.md#state) machine states for the TURN client.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`toString`](#tostring-6) | `function` | Declared here |
| [`Type`](#type-13) | `enum` | Declared here |
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
| `std::string` | [`toString`](#tostring-6) `virtual` `const` `inline` |  |

---

{#tostring-6}

#### toString

`virtual` `const` `inline`

```cpp
virtual inline std::string toString() const
```

Defined in src/turn/include/icy/turn/client/client.h:43

#### Returns
Human-readable name for the current state.

##### Reimplements

- [`toString`](base.md#classicy_1_1State_1ab14617c2967319793f296006b3cb175c)

### Public Types

| Name | Description |
|------|-------------|
| [`Type`](#type-13)  |  |

---

{#type-13}

#### Type

```cpp
enum Type
```

Defined in src/turn/include/icy/turn/client/client.h:32

| Value | Description |
|-------|-------------|
| `None` |  |
| `Allocating` |  |
| `Authorizing` |  |
| `Success` |  |
| `Failed` |  |

{#permission}

## Permission

```cpp
#include <icy/turn/permission.h>
```

```cpp
struct Permission
```

Defined in src/turn/include/icy/turn/permission.h:37

A single TURN permission entry associating a peer IP with a 5-minute expiry timer. Per RFC 5766 section 8, permissions last exactly 300 seconds and must be refreshed via a new CreatePermission request before they expire.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`ip`](#ip) | `variable` | Declared here |
| [`key`](#key) | `variable` | Declared here |
| [`timeout`](#timeout-2) | `variable` | Declared here |
| [`Permission`](#permission-1) | `function` | Declared here |
| [`Permission`](#permission-2) | `function` | Declared here |
| [`refresh`](#refresh) | `function` | Declared here |
| [`operator==`](#operator-29) | `function` | Declared here |
| [`matches`](#matches) | `function` | Declared here |

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`ip`](#ip)  | IPv4 address string of the permitted peer. |
| `Key` | [`key`](#key)  | Binary IP key for allocation hot-path comparisons. |
| `Timeout` | [`timeout`](#timeout-2)  | Countdown timer; expires after PERMISSION_LIFETIME milliseconds. |

---

{#ip}

#### ip

```cpp
std::string ip
```

Defined in src/turn/include/icy/turn/permission.h:100

IPv4 address string of the permitted peer.

---

{#key}

#### key

```cpp
Key key
```

Defined in src/turn/include/icy/turn/permission.h:101

Binary IP key for allocation hot-path comparisons.

---

{#timeout-2}

#### timeout

```cpp
Timeout timeout
```

Defined in src/turn/include/icy/turn/permission.h:102

Countdown timer; expires after PERMISSION_LIFETIME milliseconds.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Permission`](#permission-1) `inline` | Constructs a permission for `ip` and immediately starts the expiry timer. |
|  | [`Permission`](#permission-2) `inline` | Constructs a permission from a binary socket address. |
| `void` | [`refresh`](#refresh) `inline` | Resets the expiry timer, extending the permission lifetime by another 300 seconds. |
| `bool` | [`operator==`](#operator-29) `const` `inline` | Equality comparison against an IP string. |
| `bool` | [`matches`](#matches) `const` `inline` `nodiscard` | Binary IP comparison used on the TURN relay hot path. |

---

{#permission-1}

#### Permission

`inline`

```cpp
inline Permission(const std::string & ip)
```

Defined in src/turn/include/icy/turn/permission.h:106

Constructs a permission for `ip` and immediately starts the expiry timer. 
#### Parameters
* `ip` IPv4 address string of the permitted peer.

---

{#permission-2}

#### Permission

`inline`

```cpp
inline Permission(const net::Address & address)
```

Defined in src/turn/include/icy/turn/permission.h:116

Constructs a permission from a binary socket address. 
#### Parameters
* `address` Peer IP address; port is ignored.

---

{#refresh}

#### refresh

`inline`

```cpp
inline void refresh()
```

Defined in src/turn/include/icy/turn/permission.h:125

Resets the expiry timer, extending the permission lifetime by another 300 seconds.

---

{#operator-29}

#### operator==

`const` `inline`

```cpp
inline bool operator==(std::string_view r) const
```

Defined in src/turn/include/icy/turn/permission.h:130

Equality comparison against an IP string. 
#### Parameters
* `r` IPv4 address string to compare against. 

#### Returns
true if this permission's IP matches `r`.

---

{#matches}

#### matches

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline bool matches(const net::Address & address) const
```

Defined in src/turn/include/icy/turn/permission.h:133

Binary IP comparison used on the TURN relay hot path.

{#key-1}

## Key

```cpp
#include <icy/turn/permission.h>
```

```cpp
struct Key
```

Defined in src/turn/include/icy/turn/permission.h:39

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`af`](#af-1) | `variable` | Declared here |
| [`bytes`](#bytes) | `variable` | Declared here |
| [`size`](#size-3) | `variable` | Declared here |
| [`valid`](#valid-8) | `function` | Declared here |
| [`matches`](#matches-1) | `function` | Declared here |
| [`matches`](#matches-2) | `function` | Declared here |
| [`fromAddress`](#fromaddress) | `function` | Declared here |
| [`fromIP`](#fromip) | `function` | Declared here |

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `int` | [`af`](#af-1)  |  |
| `std::array< uint8_t, 16 >` | [`bytes`](#bytes)  |  |
| `uint8_t` | [`size`](#size-3)  |  |

---

{#af-1}

#### af

```cpp
int af = AF_UNSPEC
```

Defined in src/turn/include/icy/turn/permission.h:41

---

{#bytes}

#### bytes

```cpp
std::array< uint8_t, 16 > bytes {}
```

Defined in src/turn/include/icy/turn/permission.h:42

---

{#size-3}

#### size

```cpp
uint8_t size = 0
```

Defined in src/turn/include/icy/turn/permission.h:43

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
| `bool` | [`valid`](#valid-8) `const` `inline` `nodiscard` |  |
| `bool` | [`matches`](#matches-1) `const` `inline` `nodiscard` |  |
| `bool` | [`matches`](#matches-2) `const` `inline` `nodiscard` |  |

---

{#valid-8}

#### valid

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline bool valid() const
```

Defined in src/turn/include/icy/turn/permission.h:45

---

{#matches-1}

#### matches

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline bool matches(const net::Address & address) const
```

Defined in src/turn/include/icy/turn/permission.h:81

---

{#matches-2}

#### matches

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline bool matches(const Key & other) const
```

Defined in src/turn/include/icy/turn/permission.h:90

### Public Static Methods

| Return | Name | Description |
|--------|------|-------------|
| `Key` | [`fromAddress`](#fromaddress) `static` `inline` |  |
| `Key` | [`fromIP`](#fromip) `static` `inline` |  |

---

{#fromaddress}

#### fromAddress

`static` `inline`

```cpp
static inline Key fromAddress(const net::Address & address)
```

Defined in src/turn/include/icy/turn/permission.h:50

---

{#fromip}

#### fromIP

`static` `inline`

```cpp
static inline Key fromIP(const std::string & ip)
```

Defined in src/turn/include/icy/turn/permission.h:72

{#relayconnectionbinding}

## RelayConnectionBinding

```cpp
#include <icy/turn/client/tcpclient.h>
```

```cpp
struct RelayConnectionBinding
```

Defined in src/turn/include/icy/turn/client/tcpclient.h:70

Binding metadata stored as socket opaque data during a ConnectionBind handshake. Associates a relay socket with its TURN connection ID and peer address.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`connectionID`](#connectionid) | `variable` | Declared here |
| [`peerAddress`](#peeraddress-5) | `variable` | Declared here |

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `uint32_t` | [`connectionID`](#connectionid)  | TURN CONNECTION-ID attribute value from the server. |
| `net::Address` | [`peerAddress`](#peeraddress-5)  | The peer address this binding targets. |

---

{#connectionid}

#### connectionID

```cpp
uint32_t connectionID
```

Defined in src/turn/include/icy/turn/client/tcpclient.h:72

TURN CONNECTION-ID attribute value from the server.

---

{#peeraddress-5}

#### peerAddress

```cpp
net::Address peerAddress
```

Defined in src/turn/include/icy/turn/client/tcpclient.h:73

The peer address this binding targets.

{#serverobserver}

## ServerObserver

```cpp
#include <icy/turn/server/server.h>
```

```cpp
struct ServerObserver
```

Defined in src/turn/include/icy/turn/server/server.h:96

Observer interface that the application must implement to participate in server-side allocation management and authentication.

The observer is responsible for enforcing per-user allocation quotas and bandwidth limits. Implementations may perform authentication synchronously (returning Authorized/NotAuthorized immediately) or asynchronously (returning Authenticating and calling handleRequest() again later).

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`onServerAllocationCreated`](#onserverallocationcreated) | `function` | Declared here |
| [`onServerAllocationRemoved`](#onserverallocationremoved) | `function` | Declared here |
| [`authenticateRequest`](#authenticaterequest-1) | `function` | Declared here |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`onServerAllocationCreated`](#onserverallocationcreated) `virtual` | Called after a new allocation is successfully created. |
| `void` | [`onServerAllocationRemoved`](#onserverallocationremoved) `virtual` | Called just before an allocation is destroyed (expired, deleted, or server stopped). |
| `AuthenticationState` | [`authenticateRequest`](#authenticaterequest-1) `virtual` | Authenticates an incoming STUN request using the long-term credential mechanism (RFC 5389 section 10.2). Return Authorized to proceed, NotAuthorized to reject with a 401, QuotaReached to reject with a 486, or Authenticating to defer until the result is available asynchronously. |

---

{#onserverallocationcreated}

#### onServerAllocationCreated

`virtual`

```cpp
virtual void onServerAllocationCreated(Server * server, IAllocation * alloc)
```

Defined in src/turn/include/icy/turn/server/server.h:101

Called after a new allocation is successfully created. 
#### Parameters
* `server` The server that owns the allocation. 

* `alloc` The newly created allocation (lifetime managed by the server).

---

{#onserverallocationremoved}

#### onServerAllocationRemoved

`virtual`

```cpp
virtual void onServerAllocationRemoved(Server * server, IAllocation * alloc)
```

Defined in src/turn/include/icy/turn/server/server.h:106

Called just before an allocation is destroyed (expired, deleted, or server stopped). 
#### Parameters
* `server` The server that owned the allocation. 

* `alloc` The allocation being removed; do not delete this pointer.

---

{#authenticaterequest-1}

#### authenticateRequest

`virtual`

```cpp
virtual AuthenticationState authenticateRequest(Server * server, Request & request)
```

Defined in src/turn/include/icy/turn/server/server.h:115

Authenticates an incoming STUN request using the long-term credential mechanism (RFC 5389 section 10.2). Return Authorized to proceed, NotAuthorized to reject with a 401, QuotaReached to reject with a 486, or Authenticating to defer until the result is available asynchronously. 
#### Parameters
* `server` The server receiving the request. 

* `request` The STUN request to authenticate. 

#### Returns
An [AuthenticationState](#authenticationstate) indicating how to proceed.

{#serveroptions}

## ServerOptions

```cpp
#include <icy/turn/server/server.h>
```

```cpp
struct ServerOptions
```

Defined in src/turn/include/icy/turn/server/server.h:53

[Configuration](base.md#configuration) options for the TURN server.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`software`](#software-1) | `variable` | Declared here |
| [`realm`](#realm) | `variable` | Declared here |
| [`allocationDefaultLifetime`](#allocationdefaultlifetime) | `variable` | Declared here |
| [`allocationMaxLifetime`](#allocationmaxlifetime) | `variable` | Declared here |
| [`allocationMaxPermissions`](#allocationmaxpermissions) | `variable` | Declared here |
| [`timerInterval`](#timerinterval-1) | `variable` | Declared here |
| [`earlyMediaBufferSize`](#earlymediabuffersize) | `variable` | Declared here |
| [`listenAddr`](#listenaddr) | `variable` | Declared here |
| [`externalIP`](#externalip) | `variable` | Declared here |
| [`enableTCP`](#enabletcp) | `variable` | Declared here |
| [`enableUDP`](#enableudp) | `variable` | Declared here |
| [`enableLocalIPPermissions`](#enablelocalippermissions) | `variable` | Declared here |
| [`ServerOptions`](#serveroptions-1) | `function` | Declared here |

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`software`](#software-1)  |  |
| `std::string` | [`realm`](#realm)  |  |
| `uint32_t` | [`allocationDefaultLifetime`](#allocationdefaultlifetime)  |  |
| `uint32_t` | [`allocationMaxLifetime`](#allocationmaxlifetime)  |  |
| `int` | [`allocationMaxPermissions`](#allocationmaxpermissions)  |  |
| `int` | [`timerInterval`](#timerinterval-1)  |  |
| `int` | [`earlyMediaBufferSize`](#earlymediabuffersize)  |  |
| `net::Address` | [`listenAddr`](#listenaddr)  | The TCP and UDP bind() address. |
| `std::string` | [`externalIP`](#externalip)  | The external public facing IP address of the server. |
| `bool` | [`enableTCP`](#enabletcp)  |  |
| `bool` | [`enableUDP`](#enableudp)  |  |
| `bool` | [`enableLocalIPPermissions`](#enablelocalippermissions)  | Auto-grant permissions for RFC 1918/loopback addresses. |

---

{#software-1}

#### software

```cpp
std::string software
```

Defined in src/turn/include/icy/turn/server/server.h:55

---

{#realm}

#### realm

```cpp
std::string realm
```

Defined in src/turn/include/icy/turn/server/server.h:56

---

{#allocationdefaultlifetime}

#### allocationDefaultLifetime

```cpp
uint32_t allocationDefaultLifetime
```

Defined in src/turn/include/icy/turn/server/server.h:58

---

{#allocationmaxlifetime}

#### allocationMaxLifetime

```cpp
uint32_t allocationMaxLifetime
```

Defined in src/turn/include/icy/turn/server/server.h:59

---

{#allocationmaxpermissions}

#### allocationMaxPermissions

```cpp
int allocationMaxPermissions
```

Defined in src/turn/include/icy/turn/server/server.h:60

---

{#timerinterval-1}

#### timerInterval

```cpp
int timerInterval
```

Defined in src/turn/include/icy/turn/server/server.h:61

---

{#earlymediabuffersize}

#### earlyMediaBufferSize

```cpp
int earlyMediaBufferSize
```

Defined in src/turn/include/icy/turn/server/server.h:62

---

{#listenaddr}

#### listenAddr

```cpp
net::Address listenAddr
```

Defined in src/turn/include/icy/turn/server/server.h:64

The TCP and UDP bind() address.

---

{#externalip}

#### externalIP

```cpp
std::string externalIP
```

Defined in src/turn/include/icy/turn/server/server.h:65

The external public facing IP address of the server.

---

{#enabletcp}

#### enableTCP

```cpp
bool enableTCP
```

Defined in src/turn/include/icy/turn/server/server.h:67

---

{#enableudp}

#### enableUDP

```cpp
bool enableUDP
```

Defined in src/turn/include/icy/turn/server/server.h:68

---

{#enablelocalippermissions}

#### enableLocalIPPermissions

```cpp
bool enableLocalIPPermissions
```

Defined in src/turn/include/icy/turn/server/server.h:69

Auto-grant permissions for RFC 1918/loopback addresses.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`ServerOptions`](#serveroptions-1) `inline` |  |

---

{#serveroptions-1}

#### ServerOptions

`inline`

```cpp
inline ServerOptions()
```

Defined in src/turn/include/icy/turn/server/server.h:71

{#tcpclientobserver}

## TCPClientObserver

```cpp
#include <icy/turn/client/tcpclient.h>
```

```cpp
struct TCPClientObserver
```

Defined in src/turn/include/icy/turn/client/tcpclient.h:31

> **Inherits:** [`ClientObserver`](#clientobserver)

Observer interface for TCP TURN client events (RFC 6062). Extends [ClientObserver](#clientobserver) with callbacks specific to TCP relay connections.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`onRelayConnectionCreated`](#onrelayconnectioncreated) | `function` | Declared here |
| [`onRelayConnectionError`](#onrelayconnectionerror-1) | `function` | Declared here |
| [`onRelayConnectionClosed`](#onrelayconnectionclosed-1) | `function` | Declared here |
| [`onRelayConnectionBindingFailed`](#onrelayconnectionbindingfailed) | `function` | Declared here |
| [`onPeerConnectionAttempt`](#onpeerconnectionattempt) | `function` | Declared here |
| [`onClientStateChange`](#onclientstatechange) | `function` | Inherited from [`ClientObserver`](#clientobserver) |
| [`onRelayDataReceived`](#onrelaydatareceived) | `function` | Inherited from [`ClientObserver`](#clientobserver) |
| [`onAllocationFailed`](#onallocationfailed) | `function` | Inherited from [`ClientObserver`](#clientobserver) |
| [`onAllocationDeleted`](#onallocationdeleted) | `function` | Inherited from [`ClientObserver`](#clientobserver) |
| [`onAllocationPermissionsCreated`](#onallocationpermissionscreated) | `function` | Inherited from [`ClientObserver`](#clientobserver) |
| [`onTransactionResponse`](#ontransactionresponse) | `function` | Inherited from [`ClientObserver`](#clientobserver) |
| [`onTimer`](#ontimer-2) | `function` | Inherited from [`ClientObserver`](#clientobserver) |

### Inherited from [`ClientObserver`](#clientobserver)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`onClientStateChange`](#onclientstatechange) `virtual` | Called whenever the client's state machine transitions to a new state. |
| `function` | [`onRelayDataReceived`](#onrelaydatareceived) `virtual` | Called when relayed data is received from a peer via a Data Indication. |
| `function` | [`onAllocationFailed`](#onallocationfailed) `virtual` `inline` | Called when the server rejects an Allocate request with an error. |
| `function` | [`onAllocationDeleted`](#onallocationdeleted) `virtual` `inline` | Called when the server confirms deletion of the allocation. |
| `function` | [`onAllocationPermissionsCreated`](#onallocationpermissionscreated) `virtual` `inline` | Called after a CreatePermission request succeeds. |
| `function` | [`onTransactionResponse`](#ontransactionresponse) `virtual` `inline` | All received transaction responses will be routed here after local processing so the observer can easily implement extra functionality. |
| `function` | [`onTimer`](#ontimer-2) `virtual` `inline` | Fires after the client's internal timer callback. Handy for performing extra async cleanup tasks. |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`onRelayConnectionCreated`](#onrelayconnectioncreated) `virtual` | Called when a ConnectionBind succeeds and the relay pipe is live. |
| `void` | [`onRelayConnectionError`](#onrelayconnectionerror-1) `virtual` `inline` | Called when an error occurs on a relay connection socket. |
| `void` | [`onRelayConnectionClosed`](#onrelayconnectionclosed-1) `virtual` | Called when a relay connection socket is closed. |
| `void` | [`onRelayConnectionBindingFailed`](#onrelayconnectionbindingfailed) `virtual` `inline` | Called when a Connect request (client-initiated) or a ConnectionBind handshake fails for the given peer. |
| `bool` | [`onPeerConnectionAttempt`](#onpeerconnectionattempt) `virtual` `inline` | Called when the server sends a ConnectionAttempt indication indicating that a remote peer wants to connect. Return true to accept the connection (proceeds with ConnectionBind), or false to reject it. |

---

{#onrelayconnectioncreated}

#### onRelayConnectionCreated

`virtual`

```cpp
virtual void onRelayConnectionCreated(TCPClient & client, const net::TCPSocket::Ptr & socket, const net::Address & peerAddress)
```

Defined in src/turn/include/icy/turn/client/tcpclient.h:38

Called when a ConnectionBind succeeds and the relay pipe is live. 
#### Parameters
* `client` The owning TCP client. 

* `socket` The TCP socket that is now connected to the TURN server and ready for direct data transfer to/from the peer. 

* `peerAddress` The remote peer address associated with this connection.

---

{#onrelayconnectionerror-1}

#### onRelayConnectionError

`virtual` `inline`

```cpp
virtual inline void onRelayConnectionError(TCPClient & client, const net::TCPSocket::Ptr & socket, const net::Address & peerAddress)
```

Defined in src/turn/include/icy/turn/client/tcpclient.h:44

Called when an error occurs on a relay connection socket. 
#### Parameters
* `client` The owning TCP client. 

* `socket` The affected socket. 

* `peerAddress` The remote peer address for this connection.

---

{#onrelayconnectionclosed-1}

#### onRelayConnectionClosed

`virtual`

```cpp
virtual void onRelayConnectionClosed(TCPClient & client, const net::TCPSocket::Ptr & socket, const net::Address & peerAddress)
```

Defined in src/turn/include/icy/turn/client/tcpclient.h:50

Called when a relay connection socket is closed. 
#### Parameters
* `client` The owning TCP client. 

* `socket` The closed socket. 

* `peerAddress` The remote peer address for this connection.

---

{#onrelayconnectionbindingfailed}

#### onRelayConnectionBindingFailed

`virtual` `inline`

```cpp
virtual inline void onRelayConnectionBindingFailed(TCPClient & client, const net::Address & peerAddress)
```

Defined in src/turn/include/icy/turn/client/tcpclient.h:56

Called when a Connect request (client-initiated) or a ConnectionBind handshake fails for the given peer. 
#### Parameters
* `client` The owning TCP client. 

* `peerAddress` The peer address whose binding failed.

---

{#onpeerconnectionattempt}

#### onPeerConnectionAttempt

`virtual` `inline`

```cpp
virtual inline bool onPeerConnectionAttempt(TCPClient & client, const net::Address & peerAddress)
```

Defined in src/turn/include/icy/turn/client/tcpclient.h:64

Called when the server sends a ConnectionAttempt indication indicating that a remote peer wants to connect. Return true to accept the connection (proceeds with ConnectionBind), or false to reject it. 
#### Parameters
* `client` The owning TCP client. 

* `peerAddress` The address of the connecting peer. 

#### Returns
true to accept and bind, false to ignore.

