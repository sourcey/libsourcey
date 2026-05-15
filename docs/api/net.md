{#netmodule}

# net

TCP/SSL/UDP networking, socket adapters, DNS resolution.

### Namespaces

| Name | Description |
|------|-------------|
| [`net`](#net) | [Socket](#socket-1) transports, adapters, and address helpers. |
| [`dns`](#dns) | DNS utilities. |

{#net}

# net

[Socket](#socket-1) transports, adapters, and address helpers.

### Classes

| Name | Description |
|------|-------------|
| [`Address`](#address) | Represents an IPv4 or IPv6 socket address with host and port. |
| [`PacketSocketEmitter`](#packetsocketemitter) | [Socket](#socket-1) adapter that emits received data as packets. |
| [`Socket`](#socket-1) | Base socket implementation from which all sockets derive. |
| [`SocketAdapter`](#socketadapter) | Abstract adapter interface for socket send/receive chains. |
| [`SocketEmitter`](#socketemitter) | [SocketAdapter](#socketadapter) that exposes socket events as signals. |
| [`SocketPacket`](#socketpacket) | Default packet type emitted by sockets. |
| [`SSLAdapter`](#ssladapter) | Manages the OpenSSL context and BIO buffers for an SSL socket connection. |
| [`SSLContext`](#sslcontext) | OpenSSL SSL_CTX wrapper for client and server TLS configuration. |
| [`SSLManager`](#sslmanager) | [Singleton](base.md#singleton) that owns the default client/server TLS contexts and related callbacks. |
| [`SSLSession`](#sslsession) | Cached SSL/TLS session wrapper used for client-side resumption. |
| [`SSLSocket`](#sslsocket) | SSL socket implementation. |
| [`TCPSocket`](#tcpsocket) | TCP socket implementation. |
| [`Transaction`](#transaction) | Request/response helper for packet types emitted from a socket. |
| [`UDPSocket`](#udpsocket) | UDP socket implementation. |
| [`VerificationErrorDetails`](#verificationerrordetails) | A utility class for certificate error handling. |
| [`PacketInfo`](#packetinfo) | Provides information about packets emitted from a socket. See [SocketPacket](#socketpacket). |

### Enumerations

| Name | Description |
|------|-------------|
| [`TransportType`](#transporttype)  | Transport protocol identifier used to distinguish socket types at runtime. |
| [`SocketMode`](#socketmode)  | Transport mode for socket adapters and accepted connections. |

---

{#transporttype}

#### TransportType

```cpp
enum TransportType
```

Transport protocol identifier used to distinguish socket types at runtime.

| Value | Description |
|-------|-------------|
| `UDP` | Unreliable datagram protocol. |
| `TCP` | Reliable stream protocol. |
| `SSLTCP` | TLS-encrypted TCP stream. |

---

{#socketmode}

#### SocketMode

```cpp
enum SocketMode
```

Transport mode for socket adapters and accepted connections.

| Value | Description |
|-------|-------------|
| `ServerSide` | Server-side adapter. |
| `ClientSide` | Client-side adapter. |

### Functions

| Return | Name | Description |
|--------|------|-------------|
| `std::shared_ptr< SocketT >` | [`makeSocket`](#makesocket) `inline` | Creates a socket of type SocketT wrapped in a shared_ptr. |
| `SSLContext::VerificationMode` | [`convertVerificationMode`](#convertverificationmode) `inline` | Non-case sensitive conversion of a string to a VerificationMode enum. If verMode is illegal an ArgumentException is thrown. |
| `std::string` | [`convertCertificateError`](#convertcertificateerror) `inline` | Converts an SSL certificate handling error code into an error message. |
| `std::string` | [`getLastError`](#getlasterror) `inline` | Returns the last error from the error stack. |
| `void` | [`clearErrorStack`](#clearerrorstack) `inline` | Clears the error stack. |
| `void` | [`getNetworkInterfaces`](#getnetworkinterfaces) `inline` | Populates `hosts` with all local network interface addresses. |
| `int` | [`getServerSocketSendBufSize`](#getserversocketsendbufsize)  | Returns the current send buffer size for a socket handle, in bytes. Passes val=0 to uv_send_buffer_size, which queries rather than sets the value. |
| `int` | [`getServerSocketRecvBufSize`](#getserversocketrecvbufsize)  | Returns the current receive buffer size for a socket handle, in bytes. Passes val=0 to uv_recv_buffer_size, which queries rather than sets the value. |
| `int` | [`setServerSocketSendBufSize`](#setserversocketsendbufsize)  | Sets the send buffer size for a socket handle. |
| `int` | [`setServerSocketRecvBufSize`](#setserversocketrecvbufsize)  | Sets the receive buffer size for a socket handle. |

---

{#makesocket}

#### makeSocket

`inline`

```cpp
template<class SocketT> inline std::shared_ptr< SocketT > makeSocket(uv::Loop * loop = uv::defaultLoop())
```

Creates a socket of type SocketT wrapped in a shared_ptr.

The socket is automatically destroyed when the last shared_ptr owner releases it. 
#### Parameters
* `SocketT` The concrete socket type to instantiate (e.g. [TCPSocket](#tcpsocket), [UDPSocket](#udpsocket)). 

#### Parameters
* `loop` Event loop to associate with the socket; defaults to the default loop. 

#### Returns
A shared_ptr owning the newly created socket.

---

{#convertverificationmode}

#### convertVerificationMode

`inline`

```cpp
inline SSLContext::VerificationMode convertVerificationMode(const std::string & vMode)
```

Non-case sensitive conversion of a string to a VerificationMode enum. If verMode is illegal an ArgumentException is thrown.

---

{#convertcertificateerror}

#### convertCertificateError

`inline`

```cpp
inline std::string convertCertificateError(long errCode)
```

Converts an SSL certificate handling error code into an error message.

---

{#getlasterror}

#### getLastError

`inline`

```cpp
inline std::string getLastError()
```

Returns the last error from the error stack.

---

{#clearerrorstack}

#### clearErrorStack

`inline`

```cpp
inline void clearErrorStack()
```

Clears the error stack.

---

{#getnetworkinterfaces}

#### getNetworkInterfaces

`inline`

```cpp
inline void getNetworkInterfaces(std::vector< net::Address > & hosts)
```

Populates `hosts` with all local network interface addresses.

Each entry is an IPv4 address constructed from the interface's address4 field. The results include loopback and any other active interfaces reported by libuv. 
#### Parameters
* `hosts` Vector to append the discovered addresses to.

---

{#getserversocketsendbufsize}

#### getServerSocketSendBufSize

```cpp
template<typename T> int getServerSocketSendBufSize(uv::Handle< T > & handle)
```

Returns the current send buffer size for a socket handle, in bytes. Passes val=0 to uv_send_buffer_size, which queries rather than sets the value. 
#### Parameters
* `T` The libuv handle type (e.g. uv_tcp_t, uv_udp_t). 

#### Parameters
* `handle` The socket handle to query. 

#### Returns
The send buffer size, or a libuv error code on failure.

---

{#getserversocketrecvbufsize}

#### getServerSocketRecvBufSize

```cpp
template<typename T> int getServerSocketRecvBufSize(uv::Handle< T > & handle)
```

Returns the current receive buffer size for a socket handle, in bytes. Passes val=0 to uv_recv_buffer_size, which queries rather than sets the value. 
#### Parameters
* `T` The libuv handle type (e.g. uv_tcp_t, uv_udp_t). 

#### Parameters
* `handle` The socket handle to query. 

#### Returns
The receive buffer size, or a libuv error code on failure.

---

{#setserversocketsendbufsize}

#### setServerSocketSendBufSize

```cpp
template<typename T> int setServerSocketSendBufSize(uv::Handle< T > & handle, int size)
```

Sets the send buffer size for a socket handle. 
#### Parameters
* `T` The libuv handle type (e.g. uv_tcp_t, uv_udp_t). 

#### Parameters
* `handle` The socket handle to configure. 

* `size` The desired send buffer size in bytes. 

#### Returns
0 on success, or a libuv error code on failure.

---

{#setserversocketrecvbufsize}

#### setServerSocketRecvBufSize

```cpp
template<typename T> int setServerSocketRecvBufSize(uv::Handle< T > & handle, int size)
```

Sets the receive buffer size for a socket handle. 
#### Parameters
* `T` The libuv handle type (e.g. uv_tcp_t, uv_udp_t). 

#### Parameters
* `handle` The socket handle to configure. 

* `size` The desired receive buffer size in bytes. 

#### Returns
0 on success, or a libuv error code on failure.

### Variables

| Return | Name | Description |
|--------|------|-------------|
| `int` | [`MAX_TCP_PACKET_SIZE`](#max_tcp_packet_size) `constexpr` | Maximum size of a single TCP receive buffer, in bytes. |
| `int` | [`MAX_UDP_PACKET_SIZE`](#max_udp_packet_size) `constexpr` | Maximum size of a single UDP datagram payload, in bytes. |

---

{#max_tcp_packet_size}

#### MAX_TCP_PACKET_SIZE

`constexpr`

```cpp
int MAX_TCP_PACKET_SIZE = 64 * 1024
```

Maximum size of a single TCP receive buffer, in bytes.

---

{#max_udp_packet_size}

#### MAX_UDP_PACKET_SIZE

`constexpr`

```cpp
int MAX_UDP_PACKET_SIZE = 1500
```

Maximum size of a single UDP datagram payload, in bytes.

{#address}

## Address

```cpp
#include <icy/net/address.h>
```

```cpp
class Address
```

Defined in src/net/include/icy/net/address.h:28

Represents an IPv4 or IPv6 socket address with host and port.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`operator<<`](#operator-4) | `friend` | Declared here |
| [`Address`](#address-1) | `function` | Declared here |
| [`Address`](#address-2) | `function` | Declared here |
| [`Address`](#address-3) | `function` | Declared here |
| [`Address`](#address-4) | `function` | Declared here |
| [`Address`](#address-5) | `function` | Declared here |
| [`Address`](#address-6) | `function` | Declared here |
| [`~Address`](#address-7) | `function` | Declared here |
| [`operator=`](#operator-5) | `function` | Declared here |
| [`swap`](#swap) | `function` | Declared here |
| [`host`](#host) | `function` | Declared here |
| [`port`](#port) | `function` | Declared here |
| [`length`](#length) | `function` | Declared here |
| [`addr`](#addr-1) | `function` | Declared here |
| [`af`](#af) | `function` | Declared here |
| [`toString`](#tostring-4) | `function` | Declared here |
| [`family`](#family) | `function` | Declared here |
| [`valid`](#valid) | `function` | Declared here |
| [`operator<`](#operator-6) | `function` | Declared here |
| [`operator==`](#operator-7) | `function` | Declared here |
| [`operator!=`](#operator-8) | `function` | Declared here |
| [`resolveService`](#resolveservice) | `function` | Declared here |
| [`validateIP`](#validateip) | `function` | Declared here |
| [`init`](#init-3) | `function` | Declared here |
| [`Family`](#family-1) | `enum` | Declared here |
| [`_base`](#_base) | `variable` | Declared here |

### Friends

| Name | Description |
|------|-------------|
| [`operator<<`](#operator-4) `inline` |  |

---

{#operator-4}

#### operator<<

`inline`

```cpp
friend inline std::ostream & operator<<(std::ostream & stream, const Address & addr)
```

Defined in src/net/include/icy/net/address.h:131

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Address`](#address-1)  | Creates a wildcard (all zero) IPv4 [Address](#address). |
|  | [`Address`](#address-2)  | Creates an [Address](#address) from an IP address and a port number. |
|  | [`Address`](#address-3)  | Creates an [Address](#address) by copying another one. |
|  | [`Address`](#address-4)  | Creates an [Address](#address) from a native socket address. |
|  | [`Address`](#address-5)  | Creates an [Address](#address) from an IP address and a service name or port number. |
|  | [`Address`](#address-6) `explicit` | Creates an [Address](#address) from an IP address or host name and a port number/service name. Host name/address and port number must be separated by a colon. In case of an IPv6 address, the address part must be enclosed in brackets. |
|  | [`~Address`](#address-7) `noexcept` | Destroys the [Address](#address). |
| `Address &` | [`operator=`](#operator-5)  | Assigns another [Address](#address). |
| `void` | [`swap`](#swap)  | Swaps the [Address](#address) with another one. |
| `std::string` | [`host`](#host) `const` | Returns the host IP address. |
| `uint16_t` | [`port`](#port) `const` | Returns the port number. |
| `socklen_t` | [`length`](#length) `const` | Returns the length of the internal native socket address. |
| `const struct sockaddr *` | [`addr`](#addr-1) `const` | Returns a pointer to the internal native socket address. |
| `int` | [`af`](#af) `const` | Returns the address family (AF_INET or AF_INET6) of the address. |
| `std::string` | [`toString`](#tostring-4) `const` | Returns a string representation of the address. |
| `Address::Family` | [`family`](#family) `const` | Returns the address family of the host's address. |
| `bool` | [`valid`](#valid) `const` | Returns true when the port is set and the address is valid ie. not wildcard. |
| `bool` | [`operator<`](#operator-6) `const` | Compares two addresses for ordering (by family then port). |
| `bool` | [`operator==`](#operator-7) `const` | Returns true if the host and port of both addresses are equal. |
| `bool` | [`operator!=`](#operator-8) `const` | Returns true if the host or port of the addresses differ. |

---

{#address-1}

#### Address

```cpp
Address()
```

Defined in src/net/include/icy/net/address.h:39

Creates a wildcard (all zero) IPv4 [Address](#address).

---

{#address-2}

#### Address

```cpp
Address(const std::string & host, uint16_t port)
```

Defined in src/net/include/icy/net/address.h:45

Creates an [Address](#address) from an IP address and a port number.

The IP address must either be a domain name, or it must be in dotted decimal (IPv4) or hex string (IPv6) format.

---

{#address-3}

#### Address

```cpp
Address(const Address & addr)
```

Defined in src/net/include/icy/net/address.h:48

Creates an [Address](#address) by copying another one.

---

{#address-4}

#### Address

```cpp
Address(const struct sockaddr * addr, socklen_t length)
```

Defined in src/net/include/icy/net/address.h:51

Creates an [Address](#address) from a native socket address.

---

{#address-5}

#### Address

```cpp
Address(const std::string & host, const std::string & port)
```

Defined in src/net/include/icy/net/address.h:61

Creates an [Address](#address) from an IP address and a service name or port number.

The IP address must either be a domain name, or it must be in dotted decimal (IPv4) or hex string (IPv6) format.

The given port must either be a decimal port number, or a service name.

---

{#address-6}

#### Address

`explicit`

```cpp
explicit Address(const std::string & hostAndPort)
```

Defined in src/net/include/icy/net/address.h:72

Creates an [Address](#address) from an IP address or host name and a port number/service name. Host name/address and port number must be separated by a colon. In case of an IPv6 address, the address part must be enclosed in brackets.

Examples: 192.168.1.10:80

0state.com:8080

---

{#address-7}

#### ~Address

`noexcept`

```cpp
~Address() noexcept
```

Defined in src/net/include/icy/net/address.h:75

Destroys the [Address](#address).

---

{#operator-5}

#### operator=

```cpp
Address & operator=(const Address & addr)
```

Defined in src/net/include/icy/net/address.h:78

Assigns another [Address](#address).

---

{#swap}

#### swap

```cpp
void swap(Address & addr)
```

Defined in src/net/include/icy/net/address.h:81

Swaps the [Address](#address) with another one.

---

{#host}

#### host

`const`

```cpp
std::string host() const
```

Defined in src/net/include/icy/net/address.h:84

Returns the host IP address.

---

{#port}

#### port

`const`

```cpp
uint16_t port() const
```

Defined in src/net/include/icy/net/address.h:87

Returns the port number.

---

{#length}

#### length

`const`

```cpp
socklen_t length() const
```

Defined in src/net/include/icy/net/address.h:90

Returns the length of the internal native socket address.

---

{#addr-1}

#### addr

`const`

```cpp
const struct sockaddr * addr() const
```

Defined in src/net/include/icy/net/address.h:93

Returns a pointer to the internal native socket address.

---

{#af}

#### af

`const`

```cpp
int af() const
```

Defined in src/net/include/icy/net/address.h:96

Returns the address family (AF_INET or AF_INET6) of the address.

---

{#tostring-4}

#### toString

`const`

```cpp
std::string toString() const
```

Defined in src/net/include/icy/net/address.h:99

Returns a string representation of the address.

---

{#family}

#### family

`const`

```cpp
Address::Family family() const
```

Defined in src/net/include/icy/net/address.h:102

Returns the address family of the host's address.

---

{#valid}

#### valid

`const`

```cpp
bool valid() const
```

Defined in src/net/include/icy/net/address.h:106

Returns true when the port is set and the address is valid ie. not wildcard.

---

{#operator-6}

#### operator<

`const`

```cpp
bool operator<(const Address & addr) const
```

Defined in src/net/include/icy/net/address.h:121

Compares two addresses for ordering (by family then port). 
#### Parameters
* `addr` The address to compare against. 

#### Returns
true if this address is less than `addr`.

---

{#operator-7}

#### operator==

`const`

```cpp
bool operator==(const Address & addr) const
```

Defined in src/net/include/icy/net/address.h:125

Returns true if the host and port of both addresses are equal. 
#### Parameters
* `addr` The address to compare against.

---

{#operator-8}

#### operator!=

`const`

```cpp
bool operator!=(const Address & addr) const
```

Defined in src/net/include/icy/net/address.h:129

Returns true if the host or port of the addresses differ. 
#### Parameters
* `addr` The address to compare against.

### Public Static Methods

| Return | Name | Description |
|--------|------|-------------|
| `uint16_t` | [`resolveService`](#resolveservice) `static` | Resolves a service name or decimal port string to a port number. |
| `bool` | [`validateIP`](#validateip) `static` | Returns true if the given string is a valid IPv4 or IPv6 address. |

---

{#resolveservice}

#### resolveService

`static`

```cpp
static uint16_t resolveService(const std::string & service)
```

Defined in src/net/include/icy/net/address.h:111

Resolves a service name or decimal port string to a port number. 
#### Parameters
* `service` Service name (e.g. "http") or decimal port string (e.g. "80"). 

#### Returns
The resolved port number in host byte order.

---

{#validateip}

#### validateIP

`static`

```cpp
static bool validateIP(std::string_view address)
```

Defined in src/net/include/icy/net/address.h:116

Returns true if the given string is a valid IPv4 or IPv6 address. 
#### Parameters
* `address` The string to validate. 

#### Returns
true if the address parses as a valid IP address, false otherwise.

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`init`](#init-3)  |  |

---

{#init-3}

#### init

```cpp
void init(const std::string & host, uint16_t port)
```

Defined in src/net/include/icy/net/address.h:138

### Public Types

| Name | Description |
|------|-------------|
| [`Family`](#family-1)  | Possible address families for IP addresses. |

---

{#family-1}

#### Family

```cpp
enum Family
```

Defined in src/net/include/icy/net/address.h:32

Possible address families for IP addresses.

| Value | Description |
|-------|-------------|
| `IPv4` |  |
| `IPv6` |  |

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::shared_ptr< AddressBase >` | [`_base`](#_base)  |  |

---

{#_base}

#### _base

```cpp
std::shared_ptr< AddressBase > _base
```

Defined in src/net/include/icy/net/address.h:141

{#packetsocketemitter}

## PacketSocketEmitter

```cpp
#include <icy/net/packetsocket.h>
```

```cpp
class PacketSocketEmitter
```

Defined in src/net/include/icy/net/packetsocket.h:32

> **Inherits:** [`SocketEmitter`](#socketemitter), [`Signal< void(IPacket &)>`](base.md#signal)
> **Subclassed by:** [`Transaction< Message >`](#transaction), [`Transaction< PacketT >`](#transaction)

[Socket](#socket-1) adapter that emits received data as packets.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`factory`](#factory) | `variable` | Declared here |
| [`PacketSocketEmitter`](#packetsocketemitter-1) | `function` | Declared here |
| [`onSocketRecv`](#onsocketrecv) | `function` | Declared here |
| [`onPacket`](#onpacket) | `function` | Declared here |
| [`Connect`](#connect-4) | `variable` | Inherited from [`SocketEmitter`](#socketemitter) |
| [`Recv`](#recv) | `variable` | Inherited from [`SocketEmitter`](#socketemitter) |
| [`Error`](#error-6) | `variable` | Inherited from [`SocketEmitter`](#socketemitter) |
| [`Close`](#close-13) | `variable` | Inherited from [`SocketEmitter`](#socketemitter) |
| [`impl`](#impl) | `variable` | Inherited from [`SocketEmitter`](#socketemitter) |
| [`SocketEmitter`](#socketemitter-1) | `function` | Inherited from [`SocketEmitter`](#socketemitter) |
| [`SocketEmitter`](#socketemitter-2) | `function` | Inherited from [`SocketEmitter`](#socketemitter) |
| [`~SocketEmitter`](#socketemitter-3) | `function` | Inherited from [`SocketEmitter`](#socketemitter) |
| [`addReceiver`](#addreceiver-1) | `function` | Inherited from [`SocketEmitter`](#socketemitter) |
| [`removeReceiver`](#removereceiver-1) | `function` | Inherited from [`SocketEmitter`](#socketemitter) |
| [`swap`](#swap-1) | `function` | Inherited from [`SocketEmitter`](#socketemitter) |
| [`as`](#as) | `function` | Inherited from [`SocketEmitter`](#socketemitter) |
| [`operator->`](#operator-9) | `function` | Inherited from [`SocketEmitter`](#socketemitter) |
| [`onSocketConnect`](#onsocketconnect-1) | `function` | Inherited from [`SocketEmitter`](#socketemitter) |
| [`onSocketRecv`](#onsocketrecv-2) | `function` | Inherited from [`SocketEmitter`](#socketemitter) |
| [`onSocketError`](#onsocketerror-1) | `function` | Inherited from [`SocketEmitter`](#socketemitter) |
| [`onSocketClose`](#onsocketclose-1) | `function` | Inherited from [`SocketEmitter`](#socketemitter) |
| [`priority`](#priority-1) | `variable` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`SocketAdapter`](#socketadapter-1) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`~SocketAdapter`](#socketadapter-2) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`send`](#send) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`send`](#send-1) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`sendOwned`](#sendowned-2) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`sendOwned`](#sendowned-3) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`sendPacket`](#sendpacket) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`sendPacket`](#sendpacket-1) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`sendPacket`](#sendpacket-2) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`setSender`](#setsender) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`sender`](#sender) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`addReceiver`](#addreceiver) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`removeReceiver`](#removereceiver) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`hasReceiver`](#hasreceiver) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`receivers`](#receivers) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`onSocketConnect`](#onsocketconnect) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`onSocketRecv`](#onsocketrecv-1) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`onSocketError`](#onsocketerror) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`onSocketClose`](#onsocketclose) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`_sender`](#_sender) | `variable` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`_receivers`](#_receivers) | `variable` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`_dirty`](#_dirty) | `variable` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`cleanupReceivers`](#cleanupreceivers) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |

### Inherited from [`SocketEmitter`](#socketemitter)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`Connect`](#connect-4)  | Signals that the socket is connected. |
| `variable` | [`Recv`](#recv)  | Signals when data is received by the socket. |
| `variable` | [`Error`](#error-6)  | Signals that the socket is closed in error. This signal will be sent just before the Closed signal. |
| `variable` | [`Close`](#close-13)  | Signals that the underlying socket is closed. |
| `variable` | [`impl`](#impl)  | Pointer to the underlying socket. Sent data will be proxied to this socket. |
| `function` | [`SocketEmitter`](#socketemitter-1)  | Creates the [SocketEmitter](#socketemitter) and optionally attaches it to a socket. If `socket` is provided, this emitter registers itself as a receiver. |
| `function` | [`SocketEmitter`](#socketemitter-2)  | Copy constructor; copies all signal connections and attaches to the same socket. |
| `function` | [`~SocketEmitter`](#socketemitter-3) `virtual` `noexcept` | Destroys the [SocketAdapter](#socketadapter). |
| `function` | [`addReceiver`](#addreceiver-1) `virtual` `override` | Attaches a [SocketAdapter](#socketadapter) as a receiver; wires it to all four socket signals. |
| `function` | [`removeReceiver`](#removereceiver-1) `virtual` `override` | Detaches a [SocketAdapter](#socketadapter) from all four socket signals. |
| `function` | [`swap`](#swap-1) `virtual` | Replaces the underlying socket with `socket`. |
| `function` | [`as`](#as) `inline` | Returns the underlying socket cast to type T, or nullptr if the cast fails. |
| `function` | [`operator->`](#operator-9) `const` `inline` | Returns a raw pointer to the underlying socket for direct method access. Follows shared_ptr semantics; the caller must not delete the returned pointer. |
| `function` | [`onSocketConnect`](#onsocketconnect-1) `virtual` `override` | Forwards the connect event to chained adapters, then fires the Connect signal. |
| `function` | [`onSocketRecv`](#onsocketrecv-2) `virtual` `override` | Forwards the recv event to chained adapters, then fires the Recv signal. |
| `function` | [`onSocketError`](#onsocketerror-1) `virtual` `override` | Forwards the error event to chained adapters, then fires the [Error](base.md#error) signal. |
| `function` | [`onSocketClose`](#onsocketclose-1) `virtual` `override` | Forwards the close event to chained adapters, then fires the Close signal. |

### Inherited from [`SocketAdapter`](#socketadapter)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`priority`](#priority-1)  | The priority of this adapter for STL sort operations. |
| `function` | [`SocketAdapter`](#socketadapter-1)  | Creates the [SocketAdapter](#socketadapter). |
| `function` | [`~SocketAdapter`](#socketadapter-2) `virtual` `noexcept` | Destroys the [SocketAdapter](#socketadapter). |
| `function` | [`send`](#send) `virtual` `nodiscard` | Sends the given data buffer to the connected peer. Returns the number of bytes sent or -1 on error. No exception will be thrown. For TCP sockets the given peer address must match the connected peer address. |
| `function` | [`send`](#send-1) `virtual` `nodiscard` |  |
| `function` | [`sendOwned`](#sendowned-2) `virtual` `nodiscard` | Sends an owned payload buffer to the connected peer. |
| `function` | [`sendOwned`](#sendowned-3) `virtual` `nodiscard` |  |
| `function` | [`sendPacket`](#sendpacket) `virtual` | Sends the given packet to the connected peer. Returns the number of bytes sent or -1 on error. No exception will be thrown. For TCP sockets the given peer address must match the connected peer address. |
| `function` | [`sendPacket`](#sendpacket-1) `virtual` |  |
| `function` | [`sendPacket`](#sendpacket-2) `virtual` | Sends the given packet to the connected peer. This method provides delegate compatibility, and unlike other send methods throws an exception if the underlying socket is closed. |
| `function` | [`setSender`](#setsender) `virtual` | Sets the pointer to the outgoing data adapter. Send methods proxy data to this adapter by default. |
| `function` | [`sender`](#sender)  | Returns the output [SocketAdapter](#socketadapter) pointer. |
| `function` | [`addReceiver`](#addreceiver) `virtual` | Sets the pointer to the incoming data adapter. Events proxy data to this adapter by default. |
| `function` | [`removeReceiver`](#removereceiver) `virtual` | Remove the given receiver. |
| `function` | [`hasReceiver`](#hasreceiver) `virtual` | Returns true if the given receiver is connected. |
| `function` | [`receivers`](#receivers)  | Returns all currently registered input [SocketAdapter](#socketadapter) pointers. Dead (removed) entries are excluded from the returned list. |
| `function` | [`onSocketConnect`](#onsocketconnect) `virtual` | Called when the socket establishes a connection. Forwards the event to all registered receivers in priority order. Override to intercept before the application sees the event. |
| `function` | [`onSocketRecv`](#onsocketrecv-1) `virtual` | Called when data is received from the socket. Forwards the event to all registered receivers in priority order. |
| `function` | [`onSocketError`](#onsocketerror) `virtual` | Called when the socket encounters an error. Forwards the event to all registered receivers in priority order. |
| `function` | [`onSocketClose`](#onsocketclose) `virtual` | Called when the socket is closed. Forwards the event to all registered receivers in priority order. |
| `variable` | [`_sender`](#_sender)  |  |
| `variable` | [`_receivers`](#_receivers)  |  |
| `variable` | [`_dirty`](#_dirty)  |  |
| `function` | [`cleanupReceivers`](#cleanupreceivers) `virtual` |  |

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `PacketFactory` | [`factory`](#factory)  | The packet factory. |

---

{#factory}

#### factory

```cpp
PacketFactory factory
```

Defined in src/net/include/icy/net/packetsocket.h:59

The packet factory.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`PacketSocketEmitter`](#packetsocketemitter-1)  | Creates the [PacketSocketEmitter](#packetsocketemitter) and attaches it to the given socket. |
| `bool` | [`onSocketRecv`](#onsocketrecv) `virtual` `override` | Parses raw received data into packets via the factory and forwards each parsed packet to [onPacket()](#onpacket). Returns true if propagation should stop. |
| `bool` | [`onPacket`](#onpacket) `virtual` | [Process](base.md#process) a parsed packet. Returns true to stop propagation. |

---

{#packetsocketemitter-1}

#### PacketSocketEmitter

```cpp
PacketSocketEmitter(const Socket::Ptr & socket = nullptr)
```

Defined in src/net/include/icy/net/packetsocket.h:43

Creates the [PacketSocketEmitter](#packetsocketemitter) and attaches it to the given socket.

The emitter should be assigned a higher priority than plain socket adapters so that packet parsing occurs before generic data callbacks. Packets are created and dispatched using the registered factory strategies; strategies with the highest priority are tried first. 
#### Parameters
* `socket` Optional socket to attach to immediately.

---

{#onsocketrecv}

#### onSocketRecv

`virtual` `override`

```cpp
virtual bool onSocketRecv(Socket & socket, const MutableBuffer & buffer, const Address & peerAddress) override
```

Defined in src/net/include/icy/net/packetsocket.h:53

Parses raw received data into packets via the factory and forwards each parsed packet to [onPacket()](#onpacket). Returns true if propagation should stop. 
#### Parameters
* `socket` The socket that received the data. 

* `buffer` The raw received data buffer. 

* `peerAddress` The sender's address. 

#### Returns
true if the event was consumed and should not propagate further.

##### Reimplements

- [`onSocketRecv`](#onsocketrecv-1)

---

{#onpacket}

#### onPacket

`virtual`

```cpp
virtual bool onPacket(IPacket & pkt)
```

Defined in src/net/include/icy/net/packetsocket.h:56

[Process](base.md#process) a parsed packet. Returns true to stop propagation.

##### Reimplemented by

- [`onPacket`](#onpacket-1)
- [`onPacket`](#onpacket-1)

{#socket-1}

## Socket

```cpp
#include <icy/net/socket.h>
```

```cpp
class Socket
```

Defined in src/net/include/icy/net/socket.h:49

> **Inherits:** [`SocketAdapter`](#socketadapter)
> **Subclassed by:** [`TCPSocket`](#tcpsocket), [`UDPSocket`](#udpsocket)

Base socket implementation from which all sockets derive.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`opaque`](#opaque) | `variable` | Declared here |
| [`Socket`](#socket-2) | `function` | Declared here |
| [`Socket`](#socket-3) | `function` | Declared here |
| [`Socket`](#socket-4) | `function` | Declared here |
| [`connect`](#connect-2) | `function` | Declared here |
| [`connect`](#connect-3) | `function` | Declared here |
| [`bind`](#bind) | `function` | Declared here |
| [`listen`](#listen) | `function` | Declared here |
| [`shutdown`](#shutdown-1) | `function` | Declared here |
| [`sendOwned`](#sendowned) | `function` | Declared here |
| [`sendOwned`](#sendowned-1) | `function` | Declared here |
| [`close`](#close-12) | `function` | Declared here |
| [`address`](#address-8) | `function` | Declared here |
| [`peerAddress`](#peeraddress-1) | `function` | Declared here |
| [`transport`](#transport) | `function` | Declared here |
| [`setError`](#seterror-1) | `function` | Declared here |
| [`error`](#error-5) | `function` | Declared here |
| [`closed`](#closed-1) | `function` | Declared here |
| [`loop`](#loop-3) | `function` | Declared here |
| [`_af`](#_af) | `variable` | Declared here |
| [`init`](#init-4) | `function` | Declared here |
| [`reset`](#reset-4) | `function` | Declared here |
| [`Ptr`](#ptr-4) | `typedef` | Declared here |
| [`Vec`](#vec) | `typedef` | Declared here |
| [`priority`](#priority-1) | `variable` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`SocketAdapter`](#socketadapter-1) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`~SocketAdapter`](#socketadapter-2) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`send`](#send) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`send`](#send-1) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`sendOwned`](#sendowned-2) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`sendOwned`](#sendowned-3) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`sendPacket`](#sendpacket) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`sendPacket`](#sendpacket-1) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`sendPacket`](#sendpacket-2) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`setSender`](#setsender) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`sender`](#sender) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`addReceiver`](#addreceiver) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`removeReceiver`](#removereceiver) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`hasReceiver`](#hasreceiver) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`receivers`](#receivers) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`onSocketConnect`](#onsocketconnect) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`onSocketRecv`](#onsocketrecv-1) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`onSocketError`](#onsocketerror) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`onSocketClose`](#onsocketclose) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`_sender`](#_sender) | `variable` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`_receivers`](#_receivers) | `variable` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`_dirty`](#_dirty) | `variable` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`cleanupReceivers`](#cleanupreceivers) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |

### Inherited from [`SocketAdapter`](#socketadapter)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`priority`](#priority-1)  | The priority of this adapter for STL sort operations. |
| `function` | [`SocketAdapter`](#socketadapter-1)  | Creates the [SocketAdapter](#socketadapter). |
| `function` | [`~SocketAdapter`](#socketadapter-2) `virtual` `noexcept` | Destroys the [SocketAdapter](#socketadapter). |
| `function` | [`send`](#send) `virtual` `nodiscard` | Sends the given data buffer to the connected peer. Returns the number of bytes sent or -1 on error. No exception will be thrown. For TCP sockets the given peer address must match the connected peer address. |
| `function` | [`send`](#send-1) `virtual` `nodiscard` |  |
| `function` | [`sendOwned`](#sendowned-2) `virtual` `nodiscard` | Sends an owned payload buffer to the connected peer. |
| `function` | [`sendOwned`](#sendowned-3) `virtual` `nodiscard` |  |
| `function` | [`sendPacket`](#sendpacket) `virtual` | Sends the given packet to the connected peer. Returns the number of bytes sent or -1 on error. No exception will be thrown. For TCP sockets the given peer address must match the connected peer address. |
| `function` | [`sendPacket`](#sendpacket-1) `virtual` |  |
| `function` | [`sendPacket`](#sendpacket-2) `virtual` | Sends the given packet to the connected peer. This method provides delegate compatibility, and unlike other send methods throws an exception if the underlying socket is closed. |
| `function` | [`setSender`](#setsender) `virtual` | Sets the pointer to the outgoing data adapter. Send methods proxy data to this adapter by default. |
| `function` | [`sender`](#sender)  | Returns the output [SocketAdapter](#socketadapter) pointer. |
| `function` | [`addReceiver`](#addreceiver) `virtual` | Sets the pointer to the incoming data adapter. Events proxy data to this adapter by default. |
| `function` | [`removeReceiver`](#removereceiver) `virtual` | Remove the given receiver. |
| `function` | [`hasReceiver`](#hasreceiver) `virtual` | Returns true if the given receiver is connected. |
| `function` | [`receivers`](#receivers)  | Returns all currently registered input [SocketAdapter](#socketadapter) pointers. Dead (removed) entries are excluded from the returned list. |
| `function` | [`onSocketConnect`](#onsocketconnect) `virtual` | Called when the socket establishes a connection. Forwards the event to all registered receivers in priority order. Override to intercept before the application sees the event. |
| `function` | [`onSocketRecv`](#onsocketrecv-1) `virtual` | Called when data is received from the socket. Forwards the event to all registered receivers in priority order. |
| `function` | [`onSocketError`](#onsocketerror) `virtual` | Called when the socket encounters an error. Forwards the event to all registered receivers in priority order. |
| `function` | [`onSocketClose`](#onsocketclose) `virtual` | Called when the socket is closed. Forwards the event to all registered receivers in priority order. |
| `variable` | [`_sender`](#_sender)  |  |
| `variable` | [`_receivers`](#_receivers)  |  |
| `variable` | [`_dirty`](#_dirty)  |  |
| `function` | [`cleanupReceivers`](#cleanupreceivers) `virtual` |  |

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::any` | [`opaque`](#opaque)  | Optional client data. |

---

{#opaque}

#### opaque

```cpp
std::any opaque
```

Defined in src/net/include/icy/net/socket.h:135

Optional client data.

The value is empty on initialization.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Socket`](#socket-2)  | Defaulted constructor. |
|  | [`Socket`](#socket-3)  | Deleted constructor. |
|  | [`Socket`](#socket-4)  | Deleted constructor. |
| `void` | [`connect`](#connect-2) `virtual` | Connects to the given peer IP address. |
| `void` | [`connect`](#connect-3) `virtual` | Resolves and connects to the given host address. |
| `void` | [`bind`](#bind) `virtual` | Bind a local address to the socket. The address may be IPv4 or IPv6 (if supported). |
| `void` | [`listen`](#listen) `virtual` `inline` | Listens the socket on the given address. |
| `bool` | [`shutdown`](#shutdown-1) `virtual` `inline` `nodiscard` | Sends the shutdown packet which should result is socket closure via callback. |
| `ssize_t` | [`sendOwned`](#sendowned) `virtual` `nodiscard` | Sends an owned payload buffer to the connected peer. |
| `ssize_t` | [`sendOwned`](#sendowned-1) `virtual` `nodiscard` |  |
| `void` | [`close`](#close-12) `virtual` | Closes the underlying socket. |
| `Address` | [`address`](#address-8) `virtual` `const` | The locally bound address. |
| `Address` | [`peerAddress`](#peeraddress-1) `virtual` `const` | The connected peer address. |
| `net::TransportType` | [`transport`](#transport) `virtual` `const` | The transport protocol: TCP, UDP or SSLTCP. |
| `void` | [`setError`](#seterror-1) `virtual` | Sets the socket error. |
| `const icy::Error &` | [`error`](#error-5) `virtual` `const` | Return the socket error if any. |
| `bool` | [`closed`](#closed-1) `virtual` `const` | Returns true if the native socket handle is closed. |
| `uv::Loop *` | [`loop`](#loop-3) `virtual` `const` | Returns the socket event loop. |

---

{#socket-2}

#### Socket

```cpp
Socket() = default
```

Defined in src/net/include/icy/net/socket.h:55

Defaulted constructor.

---

{#socket-3}

#### Socket

```cpp
Socket(const Socket &) = delete
```

Defined in src/net/include/icy/net/socket.h:58

Deleted constructor.

---

{#socket-4}

#### Socket

```cpp
Socket(Socket &&) = delete
```

Defined in src/net/include/icy/net/socket.h:60

Deleted constructor.

---

{#connect-2}

#### connect

`virtual`

```cpp
virtual void connect(const Address & address)
```

Defined in src/net/include/icy/net/socket.h:67

Connects to the given peer IP address.

Throws an exception if the address is malformed. Connection errors can be handled via the [Error](base.md#error) signal.

##### Reimplemented by

- [`connect`](#connect-5)
- [`connect`](#connect-7)
- [`connect`](#connect-9)

---

{#connect-3}

#### connect

`virtual`

```cpp
virtual void connect(std::string_view host, uint16_t port)
```

Defined in src/net/include/icy/net/socket.h:74

Resolves and connects to the given host address.

Throws an Exception if the host is malformed. Since the DNS callback is asynchronous implementations need to listen for the [Error](base.md#error) signal for handling connection errors.

##### Reimplemented by

- [`connect`](#connect-6)
- [`connect`](#connect-8)
- [`connect`](#connect-10)

---

{#bind}

#### bind

`virtual`

```cpp
virtual void bind(const Address & address, unsigned flags = 0)
```

Defined in src/net/include/icy/net/socket.h:80

Bind a local address to the socket. The address may be IPv4 or IPv6 (if supported).

Throws an Exception on error.

##### Reimplemented by

- [`bind`](#bind-1)
- [`bind`](#bind-2)
- [`bind`](#bind-3)

---

{#listen}

#### listen

`virtual` `inline`

```cpp
virtual inline void listen(int backlog = 64)
```

Defined in src/net/include/icy/net/socket.h:85

Listens the socket on the given address.

Throws an Exception on error.

##### Reimplemented by

- [`listen`](#listen-1)
- [`listen`](#listen-2)

---

{#shutdown-1}

#### shutdown

`virtual` `inline` `nodiscard`

```cpp
[[nodiscard]] virtual inline bool shutdown()
```

Defined in src/net/include/icy/net/socket.h:89

Sends the shutdown packet which should result is socket closure via callback.

##### Reimplemented by

- [`shutdown`](#shutdown-4)
- [`shutdown`](#shutdown-5)

---

{#sendowned}

#### sendOwned

`virtual` `nodiscard`

```cpp
[[nodiscard]] virtual ssize_t sendOwned(Buffer && buffer, int flags = 0)
```

Defined in src/net/include/icy/net/socket.h:95

Sends an owned payload buffer to the connected peer.

##### Reimplements

- [`sendOwned`](#sendowned-2)

##### Reimplemented by

- [`sendOwned`](#sendowned-4)
- [`sendOwned`](#sendowned-6)
- [`sendOwned`](#sendowned-8)

---

{#sendowned-1}

#### sendOwned

`virtual` `nodiscard`

```cpp
[[nodiscard]] virtual ssize_t sendOwned(Buffer && buffer, const Address & peerAddress, int flags = 0)
```

Defined in src/net/include/icy/net/socket.h:96

##### Reimplements

- [`sendOwned`](#sendowned-3)

##### Reimplemented by

- [`sendOwned`](#sendowned-5)
- [`sendOwned`](#sendowned-7)
- [`sendOwned`](#sendowned-9)

---

{#close-12}

#### close

`virtual`

```cpp
virtual void close()
```

Defined in src/net/include/icy/net/socket.h:99

Closes the underlying socket.

##### Reimplemented by

- [`close`](#close-14)
- [`close`](#close-15)
- [`close`](#close-16)

---

{#address-8}

#### address

`virtual` `const`

```cpp
virtual Address address() const
```

Defined in src/net/include/icy/net/socket.h:106

The locally bound address.

This function will not throw. A Wildcard 0.0.0.0:0 address is returned if the socket is closed or invalid.

##### Reimplemented by

- [`address`](#address-9)
- [`address`](#address-10)

---

{#peeraddress-1}

#### peerAddress

`virtual` `const`

```cpp
virtual Address peerAddress() const
```

Defined in src/net/include/icy/net/socket.h:113

The connected peer address.

This function will not throw. A Wildcard 0.0.0.0:0 address is returned if the socket is closed or invalid.

##### Reimplemented by

- [`peerAddress`](#peeraddress-2)
- [`peerAddress`](#peeraddress-4)

---

{#transport}

#### transport

`virtual` `const`

```cpp
virtual net::TransportType transport() const
```

Defined in src/net/include/icy/net/socket.h:116

The transport protocol: TCP, UDP or SSLTCP.

##### Reimplemented by

- [`transport`](#transport-1)
- [`transport`](#transport-2)
- [`transport`](#transport-3)

---

{#seterror-1}

#### setError

`virtual`

```cpp
virtual void setError(const icy::Error & err)
```

Defined in src/net/include/icy/net/socket.h:121

Sets the socket error.

Setting the error will result in socket closure.

##### Reimplemented by

- [`setError`](#seterror-2)
- [`setError`](#seterror-3)

---

{#error-5}

#### error

`virtual` `const`

```cpp
virtual const icy::Error & error() const
```

Defined in src/net/include/icy/net/socket.h:124

Return the socket error if any.

##### Reimplemented by

- [`error`](#error-7)
- [`error`](#error-8)

---

{#closed-1}

#### closed

`virtual` `const`

```cpp
virtual bool closed() const
```

Defined in src/net/include/icy/net/socket.h:127

Returns true if the native socket handle is closed.

##### Reimplemented by

- [`closed`](#closed-2)
- [`closed`](#closed-3)

---

{#loop-3}

#### loop

`virtual` `const`

```cpp
virtual uv::Loop * loop() const
```

Defined in src/net/include/icy/net/socket.h:130

Returns the socket event loop.

##### Reimplemented by

- [`loop`](#loop-4)
- [`loop`](#loop-5)

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `int` | [`_af`](#_af)  |  |

---

{#_af}

#### _af

```cpp
int _af {AF_UNSPEC}
```

Defined in src/net/include/icy/net/socket.h:144

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`init`](#init-4) `virtual` | Initializes the underlying socket context. |
| `void` | [`reset`](#reset-4) `virtual` | Resets the socket context for reuse. |

---

{#init-4}

#### init

`virtual`

```cpp
virtual void init()
```

Defined in src/net/include/icy/net/socket.h:139

Initializes the underlying socket context.

##### Reimplemented by

- [`init`](#init-5)
- [`init`](#init-6)

---

{#reset-4}

#### reset

`virtual`

```cpp
virtual void reset()
```

Defined in src/net/include/icy/net/socket.h:142

Resets the socket context for reuse.

##### Reimplemented by

- [`reset`](#reset-5)
- [`reset`](#reset-6)

### Public Types

| Name | Description |
|------|-------------|
| [`Ptr`](#ptr-4)  |  |
| [`Vec`](#vec)  |  |

---

{#ptr-4}

#### Ptr

```cpp
using Ptr = std::shared_ptr< Socket >
```

Defined in src/net/include/icy/net/socket.h:52

---

{#vec}

#### Vec

```cpp
using Vec = std::vector< Ptr >
```

Defined in src/net/include/icy/net/socket.h:53

{#socketadapter}

## SocketAdapter

```cpp
#include <icy/net/socketadapter.h>
```

```cpp
class SocketAdapter
```

Defined in src/net/include/icy/net/socketadapter.h:41

> **Subclassed by:** [`Connection`](http.md#connection-1), [`ConnectionAdapter`](http.md#connectionadapter-1), [`ConnectionStream`](http.md#connectionstream-1), [`Server`](http.md#server), [`Socket`](#socket-1), [`SocketEmitter`](#socketemitter)

Abstract adapter interface for socket send/receive chains.

[SocketAdapter](#socketadapter) is the abstract interface for all socket classes. A [SocketAdapter](#socketadapter) can also be attached to a [Socket](#socket-1) in order to override default [Socket](#socket-1) callbacks and behaviour, while still maintaining the default [Socket](#socket-1) interface (see Socket::setAdapter).

This class can also be extended to implement custom processing for received socket data before it is dispatched to the application (see [PacketSocketEmitter](#packetsocketemitter) and [Transaction](#transaction) classes).

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`priority`](#priority-1) | `variable` | Declared here |
| [`SocketAdapter`](#socketadapter-1) | `function` | Declared here |
| [`~SocketAdapter`](#socketadapter-2) | `function` | Declared here |
| [`send`](#send) | `function` | Declared here |
| [`send`](#send-1) | `function` | Declared here |
| [`sendOwned`](#sendowned-2) | `function` | Declared here |
| [`sendOwned`](#sendowned-3) | `function` | Declared here |
| [`sendPacket`](#sendpacket) | `function` | Declared here |
| [`sendPacket`](#sendpacket-1) | `function` | Declared here |
| [`sendPacket`](#sendpacket-2) | `function` | Declared here |
| [`setSender`](#setsender) | `function` | Declared here |
| [`sender`](#sender) | `function` | Declared here |
| [`addReceiver`](#addreceiver) | `function` | Declared here |
| [`removeReceiver`](#removereceiver) | `function` | Declared here |
| [`hasReceiver`](#hasreceiver) | `function` | Declared here |
| [`receivers`](#receivers) | `function` | Declared here |
| [`onSocketConnect`](#onsocketconnect) | `function` | Declared here |
| [`onSocketRecv`](#onsocketrecv-1) | `function` | Declared here |
| [`onSocketError`](#onsocketerror) | `function` | Declared here |
| [`onSocketClose`](#onsocketclose) | `function` | Declared here |
| [`_sender`](#_sender) | `variable` | Declared here |
| [`_receivers`](#_receivers) | `variable` | Declared here |
| [`_dirty`](#_dirty) | `variable` | Declared here |
| [`cleanupReceivers`](#cleanupreceivers) | `function` | Declared here |

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `int` | [`priority`](#priority-1)  | The priority of this adapter for STL sort operations. |

---

{#priority-1}

#### priority

```cpp
int priority = 0
```

Defined in src/net/include/icy/net/socketadapter.h:132

The priority of this adapter for STL sort operations.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`SocketAdapter`](#socketadapter-1)  | Creates the [SocketAdapter](#socketadapter). |
|  | [`~SocketAdapter`](#socketadapter-2) `virtual` `noexcept` | Destroys the [SocketAdapter](#socketadapter). |
| `ssize_t` | [`send`](#send) `virtual` `nodiscard` | Sends the given data buffer to the connected peer. Returns the number of bytes sent or -1 on error. No exception will be thrown. For TCP sockets the given peer address must match the connected peer address. |
| `ssize_t` | [`send`](#send-1) `virtual` `nodiscard` |  |
| `ssize_t` | [`sendOwned`](#sendowned-2) `virtual` `nodiscard` | Sends an owned payload buffer to the connected peer. |
| `ssize_t` | [`sendOwned`](#sendowned-3) `virtual` `nodiscard` |  |
| `ssize_t` | [`sendPacket`](#sendpacket) `virtual` | Sends the given packet to the connected peer. Returns the number of bytes sent or -1 on error. No exception will be thrown. For TCP sockets the given peer address must match the connected peer address. |
| `ssize_t` | [`sendPacket`](#sendpacket-1) `virtual` |  |
| `void` | [`sendPacket`](#sendpacket-2) `virtual` | Sends the given packet to the connected peer. This method provides delegate compatibility, and unlike other send methods throws an exception if the underlying socket is closed. |
| `void` | [`setSender`](#setsender) `virtual` | Sets the pointer to the outgoing data adapter. Send methods proxy data to this adapter by default. |
| `SocketAdapter *` | [`sender`](#sender)  | Returns the output [SocketAdapter](#socketadapter) pointer. |
| `void` | [`addReceiver`](#addreceiver) `virtual` | Sets the pointer to the incoming data adapter. Events proxy data to this adapter by default. |
| `void` | [`removeReceiver`](#removereceiver) `virtual` | Remove the given receiver. |
| `bool` | [`hasReceiver`](#hasreceiver) `virtual` | Returns true if the given receiver is connected. |
| `std::vector< SocketAdapter * >` | [`receivers`](#receivers)  | Returns all currently registered input [SocketAdapter](#socketadapter) pointers. Dead (removed) entries are excluded from the returned list. |
| `bool` | [`onSocketConnect`](#onsocketconnect) `virtual` | Called when the socket establishes a connection. Forwards the event to all registered receivers in priority order. Override to intercept before the application sees the event. |
| `bool` | [`onSocketRecv`](#onsocketrecv-1) `virtual` | Called when data is received from the socket. Forwards the event to all registered receivers in priority order. |
| `bool` | [`onSocketError`](#onsocketerror) `virtual` | Called when the socket encounters an error. Forwards the event to all registered receivers in priority order. |
| `bool` | [`onSocketClose`](#onsocketclose) `virtual` | Called when the socket is closed. Forwards the event to all registered receivers in priority order. |

---

{#socketadapter-1}

#### SocketAdapter

```cpp
SocketAdapter(SocketAdapter * sender = nullptr)
```

Defined in src/net/include/icy/net/socketadapter.h:45

Creates the [SocketAdapter](#socketadapter).

---

{#socketadapter-2}

#### ~SocketAdapter

`virtual` `noexcept`

```cpp
virtual ~SocketAdapter() noexcept
```

Defined in src/net/include/icy/net/socketadapter.h:48

Destroys the [SocketAdapter](#socketadapter).

---

{#send}

#### send

`virtual` `nodiscard`

```cpp
[[nodiscard]] virtual ssize_t send(const char * data, size_t len, int flags = 0)
```

Defined in src/net/include/icy/net/socketadapter.h:55

Sends the given data buffer to the connected peer. Returns the number of bytes sent or -1 on error. No exception will be thrown. For TCP sockets the given peer address must match the connected peer address.

##### Reimplemented by

- [`send`](http.md#send-10)
- [`send`](http.md#send-11)
- [`send`](http.md#send-12)
- [`send`](http.md#send-13)
- [`send`](http.md#send-14)
- [`send`](#send-2)
- [`send`](#send-4)
- [`send`](#send-7)

---

{#send-1}

#### send

`virtual` `nodiscard`

```cpp
[[nodiscard]] virtual ssize_t send(const char * data, size_t len, const Address & peerAddress, int flags = 0)
```

Defined in src/net/include/icy/net/socketadapter.h:56

##### Reimplemented by

- [`send`](http.md#send-15)
- [`send`](#send-3)
- [`send`](#send-5)
- [`send`](#send-8)

---

{#sendowned-2}

#### sendOwned

`virtual` `nodiscard`

```cpp
[[nodiscard]] virtual ssize_t sendOwned(Buffer && buffer, int flags = 0)
```

Defined in src/net/include/icy/net/socketadapter.h:62

Sends an owned payload buffer to the connected peer.

The buffer is moved through the adapter chain and retained by the transport layer until async write completion.

##### Reimplemented by

- [`sendOwned`](http.md#sendowned-10)
- [`sendOwned`](http.md#sendowned-11)
- [`sendOwned`](http.md#sendowned-12)
- [`sendOwned`](#sendowned)
- [`sendOwned`](#sendowned-4)
- [`sendOwned`](#sendowned-6)
- [`sendOwned`](#sendowned-8)

---

{#sendowned-3}

#### sendOwned

`virtual` `nodiscard`

```cpp
[[nodiscard]] virtual ssize_t sendOwned(Buffer && buffer, const Address & peerAddress, int flags = 0)
```

Defined in src/net/include/icy/net/socketadapter.h:63

##### Reimplemented by

- [`sendOwned`](http.md#sendowned-13)
- [`sendOwned`](#sendowned-1)
- [`sendOwned`](#sendowned-5)
- [`sendOwned`](#sendowned-7)
- [`sendOwned`](#sendowned-9)

---

{#sendpacket}

#### sendPacket

`virtual`

```cpp
virtual ssize_t sendPacket(const IPacket & packet, int flags = 0)
```

Defined in src/net/include/icy/net/socketadapter.h:70

Sends the given packet to the connected peer. Returns the number of bytes sent or -1 on error. No exception will be thrown. For TCP sockets the given peer address must match the connected peer address.

---

{#sendpacket-1}

#### sendPacket

`virtual`

```cpp
virtual ssize_t sendPacket(const IPacket & packet, const Address & peerAddress, int flags = 0)
```

Defined in src/net/include/icy/net/socketadapter.h:71

---

{#sendpacket-2}

#### sendPacket

`virtual`

```cpp
virtual void sendPacket(IPacket & packet)
```

Defined in src/net/include/icy/net/socketadapter.h:77

Sends the given packet to the connected peer. This method provides delegate compatibility, and unlike other send methods throws an exception if the underlying socket is closed.

---

{#setsender}

#### setSender

`virtual`

```cpp
virtual void setSender(SocketAdapter * adapter)
```

Defined in src/net/include/icy/net/socketadapter.h:81

Sets the pointer to the outgoing data adapter. Send methods proxy data to this adapter by default.

---

{#sender}

#### sender

```cpp
SocketAdapter * sender()
```

Defined in src/net/include/icy/net/socketadapter.h:84

Returns the output [SocketAdapter](#socketadapter) pointer.

---

{#addreceiver}

#### addReceiver

`virtual`

```cpp
virtual void addReceiver(SocketAdapter * adapter)
```

Defined in src/net/include/icy/net/socketadapter.h:88

Sets the pointer to the incoming data adapter. Events proxy data to this adapter by default.

##### Reimplemented by

- [`addReceiver`](#addreceiver-1)

---

{#removereceiver}

#### removeReceiver

`virtual`

```cpp
virtual void removeReceiver(SocketAdapter * adapter)
```

Defined in src/net/include/icy/net/socketadapter.h:94

Remove the given receiver.

By default this function does nothing unless the given receiver matches the current receiver.

##### Reimplemented by

- [`removeReceiver`](#removereceiver-1)

---

{#hasreceiver}

#### hasReceiver

`virtual`

```cpp
virtual bool hasReceiver(SocketAdapter * adapter)
```

Defined in src/net/include/icy/net/socketadapter.h:97

Returns true if the given receiver is connected.

---

{#receivers}

#### receivers

```cpp
std::vector< SocketAdapter * > receivers()
```

Defined in src/net/include/icy/net/socketadapter.h:101

Returns all currently registered input [SocketAdapter](#socketadapter) pointers. Dead (removed) entries are excluded from the returned list.

---

{#onsocketconnect}

#### onSocketConnect

`virtual`

```cpp
virtual bool onSocketConnect(Socket & socket)
```

Defined in src/net/include/icy/net/socketadapter.h:108

Called when the socket establishes a connection. Forwards the event to all registered receivers in priority order. Override to intercept before the application sees the event. 
#### Parameters
* `socket` The connected socket. 

#### Returns
true to stop propagation to subsequent receivers.

##### Reimplemented by

- [`onSocketConnect`](http.md#onsocketconnect-2)
- [`onSocketConnect`](http.md#onsocketconnect-3)
- [`onSocketConnect`](http.md#onsocketconnect-4)
- [`onSocketConnect`](#onsocketconnect-1)

---

{#onsocketrecv-1}

#### onSocketRecv

`virtual`

```cpp
virtual bool onSocketRecv(Socket & socket, const MutableBuffer & buffer, const Address & peerAddress)
```

Defined in src/net/include/icy/net/socketadapter.h:116

Called when data is received from the socket. Forwards the event to all registered receivers in priority order. 
#### Parameters
* `socket` The socket that received data. 

* `buffer` The received data buffer. 

* `peerAddress` [Address](#address) of the sender. 

#### Returns
true to stop propagation to subsequent receivers.

##### Reimplemented by

- [`onSocketRecv`](http.md#onsocketrecv-3)
- [`onSocketRecv`](http.md#onsocketrecv-4)
- [`onSocketRecv`](http.md#onsocketrecv-5)
- [`onSocketRecv`](http.md#onsocketrecv-6)
- [`onSocketRecv`](#onsocketrecv)
- [`onSocketRecv`](#onsocketrecv-2)

---

{#onsocketerror}

#### onSocketError

`virtual`

```cpp
virtual bool onSocketError(Socket & socket, const Error & error)
```

Defined in src/net/include/icy/net/socketadapter.h:123

Called when the socket encounters an error. Forwards the event to all registered receivers in priority order. 
#### Parameters
* `socket` The socket that encountered the error. 

* `error` [Error](base.md#error) details. 

#### Returns
true to stop propagation to subsequent receivers.

##### Reimplemented by

- [`onSocketError`](http.md#onsocketerror-2)
- [`onSocketError`](http.md#onsocketerror-3)
- [`onSocketError`](#onsocketerror-1)

---

{#onsocketclose}

#### onSocketClose

`virtual`

```cpp
virtual bool onSocketClose(Socket & socket)
```

Defined in src/net/include/icy/net/socketadapter.h:129

Called when the socket is closed. Forwards the event to all registered receivers in priority order. 
#### Parameters
* `socket` The socket that was closed. 

#### Returns
true to stop propagation to subsequent receivers.

##### Reimplemented by

- [`onSocketClose`](http.md#onsocketclose-2)
- [`onSocketClose`](http.md#onsocketclose-3)
- [`onSocketClose`](http.md#onsocketclose-4)
- [`onSocketClose`](#onsocketclose-1)

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `SocketAdapter *` | [`_sender`](#_sender)  |  |
| `std::vector< Ref >` | [`_receivers`](#_receivers)  |  |
| `bool` | [`_dirty`](#_dirty)  |  |

---

{#_sender}

#### _sender

```cpp
SocketAdapter * _sender
```

Defined in src/net/include/icy/net/socketadapter.h:144

---

{#_receivers}

#### _receivers

```cpp
std::vector< Ref > _receivers
```

Defined in src/net/include/icy/net/socketadapter.h:145

---

{#_dirty}

#### _dirty

```cpp
bool _dirty = false
```

Defined in src/net/include/icy/net/socketadapter.h:146

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`cleanupReceivers`](#cleanupreceivers) `virtual` |  |

---

{#cleanupreceivers}

#### cleanupReceivers

`virtual`

```cpp
virtual void cleanupReceivers()
```

Defined in src/net/include/icy/net/socketadapter.h:135

{#ref-1}

## Ref

```cpp
#include <icy/net/socketadapter.h>
```

```cpp
struct Ref
```

Defined in src/net/include/icy/net/socketadapter.h:138

Reference-counted handle to a [SocketAdapter](#socketadapter).

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`ptr`](#ptr-5) | `variable` | Declared here |
| [`alive`](#alive) | `variable` | Declared here |

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `SocketAdapter *` | [`ptr`](#ptr-5)  |  |
| `bool` | [`alive`](#alive)  |  |

---

{#ptr-5}

#### ptr

```cpp
SocketAdapter * ptr
```

Defined in src/net/include/icy/net/socketadapter.h:140

---

{#alive}

#### alive

```cpp
bool alive
```

Defined in src/net/include/icy/net/socketadapter.h:141

{#socketemitter}

## SocketEmitter

```cpp
#include <icy/net/socketemitter.h>
```

```cpp
class SocketEmitter
```

Defined in src/net/include/icy/net/socketemitter.h:32

> **Inherits:** [`SocketAdapter`](#socketadapter)
> **Subclassed by:** [`WebSocketAdapter`](http.md#websocketadapter-1), [`PacketSocketEmitter`](#packetsocketemitter)

[SocketAdapter](#socketadapter) that exposes socket events as signals.

Aside from adding a signal interface, the class wraps the underlying socket instance and is designed to be used much like a `std::unique_ptr` by overriding the `->` operator.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`Connect`](#connect-4) | `variable` | Declared here |
| [`Recv`](#recv) | `variable` | Declared here |
| [`Error`](#error-6) | `variable` | Declared here |
| [`Close`](#close-13) | `variable` | Declared here |
| [`impl`](#impl) | `variable` | Declared here |
| [`SocketEmitter`](#socketemitter-1) | `function` | Declared here |
| [`SocketEmitter`](#socketemitter-2) | `function` | Declared here |
| [`~SocketEmitter`](#socketemitter-3) | `function` | Declared here |
| [`addReceiver`](#addreceiver-1) | `function` | Declared here |
| [`removeReceiver`](#removereceiver-1) | `function` | Declared here |
| [`swap`](#swap-1) | `function` | Declared here |
| [`as`](#as) | `function` | Declared here |
| [`operator->`](#operator-9) | `function` | Declared here |
| [`onSocketConnect`](#onsocketconnect-1) | `function` | Declared here |
| [`onSocketRecv`](#onsocketrecv-2) | `function` | Declared here |
| [`onSocketError`](#onsocketerror-1) | `function` | Declared here |
| [`onSocketClose`](#onsocketclose-1) | `function` | Declared here |
| [`priority`](#priority-1) | `variable` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`SocketAdapter`](#socketadapter-1) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`~SocketAdapter`](#socketadapter-2) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`send`](#send) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`send`](#send-1) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`sendOwned`](#sendowned-2) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`sendOwned`](#sendowned-3) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`sendPacket`](#sendpacket) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`sendPacket`](#sendpacket-1) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`sendPacket`](#sendpacket-2) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`setSender`](#setsender) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`sender`](#sender) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`addReceiver`](#addreceiver) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`removeReceiver`](#removereceiver) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`hasReceiver`](#hasreceiver) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`receivers`](#receivers) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`onSocketConnect`](#onsocketconnect) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`onSocketRecv`](#onsocketrecv-1) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`onSocketError`](#onsocketerror) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`onSocketClose`](#onsocketclose) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`_sender`](#_sender) | `variable` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`_receivers`](#_receivers) | `variable` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`_dirty`](#_dirty) | `variable` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`cleanupReceivers`](#cleanupreceivers) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |

### Inherited from [`SocketAdapter`](#socketadapter)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`priority`](#priority-1)  | The priority of this adapter for STL sort operations. |
| `function` | [`SocketAdapter`](#socketadapter-1)  | Creates the [SocketAdapter](#socketadapter). |
| `function` | [`~SocketAdapter`](#socketadapter-2) `virtual` `noexcept` | Destroys the [SocketAdapter](#socketadapter). |
| `function` | [`send`](#send) `virtual` `nodiscard` | Sends the given data buffer to the connected peer. Returns the number of bytes sent or -1 on error. No exception will be thrown. For TCP sockets the given peer address must match the connected peer address. |
| `function` | [`send`](#send-1) `virtual` `nodiscard` |  |
| `function` | [`sendOwned`](#sendowned-2) `virtual` `nodiscard` | Sends an owned payload buffer to the connected peer. |
| `function` | [`sendOwned`](#sendowned-3) `virtual` `nodiscard` |  |
| `function` | [`sendPacket`](#sendpacket) `virtual` | Sends the given packet to the connected peer. Returns the number of bytes sent or -1 on error. No exception will be thrown. For TCP sockets the given peer address must match the connected peer address. |
| `function` | [`sendPacket`](#sendpacket-1) `virtual` |  |
| `function` | [`sendPacket`](#sendpacket-2) `virtual` | Sends the given packet to the connected peer. This method provides delegate compatibility, and unlike other send methods throws an exception if the underlying socket is closed. |
| `function` | [`setSender`](#setsender) `virtual` | Sets the pointer to the outgoing data adapter. Send methods proxy data to this adapter by default. |
| `function` | [`sender`](#sender)  | Returns the output [SocketAdapter](#socketadapter) pointer. |
| `function` | [`addReceiver`](#addreceiver) `virtual` | Sets the pointer to the incoming data adapter. Events proxy data to this adapter by default. |
| `function` | [`removeReceiver`](#removereceiver) `virtual` | Remove the given receiver. |
| `function` | [`hasReceiver`](#hasreceiver) `virtual` | Returns true if the given receiver is connected. |
| `function` | [`receivers`](#receivers)  | Returns all currently registered input [SocketAdapter](#socketadapter) pointers. Dead (removed) entries are excluded from the returned list. |
| `function` | [`onSocketConnect`](#onsocketconnect) `virtual` | Called when the socket establishes a connection. Forwards the event to all registered receivers in priority order. Override to intercept before the application sees the event. |
| `function` | [`onSocketRecv`](#onsocketrecv-1) `virtual` | Called when data is received from the socket. Forwards the event to all registered receivers in priority order. |
| `function` | [`onSocketError`](#onsocketerror) `virtual` | Called when the socket encounters an error. Forwards the event to all registered receivers in priority order. |
| `function` | [`onSocketClose`](#onsocketclose) `virtual` | Called when the socket is closed. Forwards the event to all registered receivers in priority order. |
| `variable` | [`_sender`](#_sender)  |  |
| `variable` | [`_receivers`](#_receivers)  |  |
| `variable` | [`_dirty`](#_dirty)  |  |
| `function` | [`cleanupReceivers`](#cleanupreceivers) `virtual` |  |

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `LocalSignal< bool(Socket &)>` | [`Connect`](#connect-4)  | Signals that the socket is connected. |
| `LocalSignal< bool(Socket &, const MutableBuffer &, const Address &)>` | [`Recv`](#recv)  | Signals when data is received by the socket. |
| `LocalSignal< bool(Socket &, const icy::Error &)>` | [`Error`](#error-6)  | Signals that the socket is closed in error. This signal will be sent just before the Closed signal. |
| `LocalSignal< bool(Socket &)>` | [`Close`](#close-13)  | Signals that the underlying socket is closed. |
| `Socket::Ptr` | [`impl`](#impl)  | Pointer to the underlying socket. Sent data will be proxied to this socket. |

---

{#connect-4}

#### Connect

```cpp
LocalSignal< bool(Socket &)> Connect
```

Defined in src/net/include/icy/net/socketemitter.h:48

Signals that the socket is connected.

---

{#recv}

#### Recv

```cpp
LocalSignal< bool(Socket &, const MutableBuffer &, const Address &)> Recv
```

Defined in src/net/include/icy/net/socketemitter.h:51

Signals when data is received by the socket.

---

{#error-6}

#### Error

```cpp
LocalSignal< bool(Socket &, const icy::Error &)> Error
```

Defined in src/net/include/icy/net/socketemitter.h:56

Signals that the socket is closed in error. This signal will be sent just before the Closed signal.

---

{#close-13}

#### Close

```cpp
LocalSignal< bool(Socket &)> Close
```

Defined in src/net/include/icy/net/socketemitter.h:59

Signals that the underlying socket is closed.

---

{#impl}

#### impl

```cpp
Socket::Ptr impl
```

Defined in src/net/include/icy/net/socketemitter.h:94

Pointer to the underlying socket. Sent data will be proxied to this socket.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`SocketEmitter`](#socketemitter-1)  | Creates the [SocketEmitter](#socketemitter) and optionally attaches it to a socket. If `socket` is provided, this emitter registers itself as a receiver. |
|  | [`SocketEmitter`](#socketemitter-2)  | Copy constructor; copies all signal connections and attaches to the same socket. |
|  | [`~SocketEmitter`](#socketemitter-3) `virtual` `noexcept` | Destroys the [SocketAdapter](#socketadapter). |
| `void` | [`addReceiver`](#addreceiver-1) `virtual` `override` | Attaches a [SocketAdapter](#socketadapter) as a receiver; wires it to all four socket signals. |
| `void` | [`removeReceiver`](#removereceiver-1) `virtual` `override` | Detaches a [SocketAdapter](#socketadapter) from all four socket signals. |
| `void` | [`swap`](#swap-1) `virtual` | Replaces the underlying socket with `socket`. |
| `T *` | [`as`](#as) `inline` | Returns the underlying socket cast to type T, or nullptr if the cast fails. |
| `Socket *` | [`operator->`](#operator-9) `const` `inline` | Returns a raw pointer to the underlying socket for direct method access. Follows shared_ptr semantics; the caller must not delete the returned pointer. |

---

{#socketemitter-1}

#### SocketEmitter

```cpp
SocketEmitter(const Socket::Ptr & socket = nullptr)
```

Defined in src/net/include/icy/net/socketemitter.h:38

Creates the [SocketEmitter](#socketemitter) and optionally attaches it to a socket. If `socket` is provided, this emitter registers itself as a receiver. 
#### Parameters
* `socket` Optional socket to attach to; pass nullptr to attach later via [swap()](#swap-1).

---

{#socketemitter-2}

#### SocketEmitter

```cpp
SocketEmitter(const SocketEmitter & that)
```

Defined in src/net/include/icy/net/socketemitter.h:42

Copy constructor; copies all signal connections and attaches to the same socket. 
#### Parameters
* `that` The [SocketEmitter](#socketemitter) to copy from.

---

{#socketemitter-3}

#### ~SocketEmitter

`virtual` `noexcept`

```cpp
virtual ~SocketEmitter() noexcept
```

Defined in src/net/include/icy/net/socketemitter.h:45

Destroys the [SocketAdapter](#socketadapter).

---

{#addreceiver-1}

#### addReceiver

`virtual` `override`

```cpp
virtual void addReceiver(SocketAdapter * adapter) override
```

Defined in src/net/include/icy/net/socketemitter.h:63

Attaches a [SocketAdapter](#socketadapter) as a receiver; wires it to all four socket signals. 
#### Parameters
* `adapter` The adapter to attach; its priority determines signal ordering.

##### Reimplements

- [`addReceiver`](#addreceiver)

---

{#removereceiver-1}

#### removeReceiver

`virtual` `override`

```cpp
virtual void removeReceiver(SocketAdapter * adapter) override
```

Defined in src/net/include/icy/net/socketemitter.h:67

Detaches a [SocketAdapter](#socketadapter) from all four socket signals. 
#### Parameters
* `adapter` The adapter to detach.

##### Reimplements

- [`removeReceiver`](#removereceiver)

---

{#swap-1}

#### swap

`virtual`

```cpp
virtual void swap(const Socket::Ptr & socket)
```

Defined in src/net/include/icy/net/socketemitter.h:73

Replaces the underlying socket with `socket`.

Throws std::logic_error if the emitter already has an attached socket. 
#### Parameters
* `socket` The new socket to attach.

---

{#as}

#### as

`inline`

```cpp
template<class T> inline T * as()
```

Defined in src/net/include/icy/net/socketemitter.h:79

Returns the underlying socket cast to type T, or nullptr if the cast fails. 
#### Parameters
* `T` Derived socket type to cast to. 

#### Returns
Pointer to the socket as T, or nullptr on type mismatch.

---

{#operator-9}

#### operator->

`const` `inline`

```cpp
inline Socket * operator->() const
```

Defined in src/net/include/icy/net/socketemitter.h:87

Returns a raw pointer to the underlying socket for direct method access. Follows shared_ptr semantics; the caller must not delete the returned pointer. 
#### Returns
Raw pointer to the socket (never null if a socket was attached).

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `bool` | [`onSocketConnect`](#onsocketconnect-1) `virtual` `override` | Forwards the connect event to chained adapters, then fires the Connect signal. |
| `bool` | [`onSocketRecv`](#onsocketrecv-2) `virtual` `override` | Forwards the recv event to chained adapters, then fires the Recv signal. |
| `bool` | [`onSocketError`](#onsocketerror-1) `virtual` `override` | Forwards the error event to chained adapters, then fires the [Error](base.md#error) signal. |
| `bool` | [`onSocketClose`](#onsocketclose-1) `virtual` `override` | Forwards the close event to chained adapters, then fires the Close signal. |

---

{#onsocketconnect-1}

#### onSocketConnect

`virtual` `override`

```cpp
virtual bool onSocketConnect(Socket & socket) override
```

Defined in src/net/include/icy/net/socketemitter.h:98

Forwards the connect event to chained adapters, then fires the Connect signal.

##### Reimplements

- [`onSocketConnect`](#onsocketconnect)

##### Reimplemented by

- [`onSocketConnect`](http.md#onsocketconnect-4)

---

{#onsocketrecv-2}

#### onSocketRecv

`virtual` `override`

```cpp
virtual bool onSocketRecv(Socket & socket, const MutableBuffer & buffer, const Address & peerAddress) override
```

Defined in src/net/include/icy/net/socketemitter.h:100

Forwards the recv event to chained adapters, then fires the Recv signal.

##### Reimplements

- [`onSocketRecv`](#onsocketrecv-1)

##### Reimplemented by

- [`onSocketRecv`](http.md#onsocketrecv-6)

---

{#onsocketerror-1}

#### onSocketError

`virtual` `override`

```cpp
virtual bool onSocketError(Socket & socket, const icy::Error & error) override
```

Defined in src/net/include/icy/net/socketemitter.h:102

Forwards the error event to chained adapters, then fires the [Error](base.md#error) signal.

##### Reimplements

- [`onSocketError`](#onsocketerror)

---

{#onsocketclose-1}

#### onSocketClose

`virtual` `override`

```cpp
virtual bool onSocketClose(Socket & socket) override
```

Defined in src/net/include/icy/net/socketemitter.h:104

Forwards the close event to chained adapters, then fires the Close signal.

##### Reimplements

- [`onSocketClose`](#onsocketclose)

##### Reimplemented by

- [`onSocketClose`](http.md#onsocketclose-4)

{#socketpacket}

## SocketPacket

```cpp
#include <icy/net/socket.h>
```

```cpp
class SocketPacket
```

Defined in src/net/include/icy/net/socket.h:200

> **Inherits:** [`RawPacket`](base.md#rawpacket)

Default packet type emitted by sockets.

[SocketPacket](#socketpacket) carries the remote peer address plus a borrowed view of the received byte buffer for zero-copy processing inside the receive callback.

The referenced packet buffer lifetime is only guaranteed for the duration of the receiver callback.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`SocketPacket`](#socketpacket-1) | `function` | Declared here |
| [`SocketPacket`](#socketpacket-2) | `function` | Declared here |
| [`packetInfo`](#packetinfo-3) | `function` | Declared here |
| [`print`](#print-6) | `function` | Declared here |
| [`clone`](#clone-6) | `function` | Declared here |
| [`read`](#read-1) | `function` | Declared here |
| [`write`](#write-1) | `function` | Declared here |
| [`className`](#classname-5) | `function` | Declared here |
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

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`SocketPacket`](#socketpacket-1) `inline` | Constructs a [SocketPacket](#socketpacket) wrapping the received buffer. |
|  | [`SocketPacket`](#socketpacket-2) `inline` | Copy constructor; shares the underlying buffer reference. |
| `PacketInfo *` | [`packetInfo`](#packetinfo-3) `const` `inline` | Returns the [PacketInfo](#packetinfo) for this socket packet. |
| `void` | [`print`](#print-6) `virtual` `const` `inline` | Prints a one-line description of the packet to `os`. |
| `std::unique_ptr< IPacket >` | [`clone`](#clone-6) `virtual` `const` `inline` | Returns a heap-allocated copy of this [SocketPacket](#socketpacket). |
| `ssize_t` | [`read`](#read-1) `virtual` `inline` | Not supported; always throws std::logic_error. |
| `void` | [`write`](#write-1) `virtual` `const` `inline` | Appends the packet payload to `buf`. |
| `const char *` | [`className`](#classname-5) `virtual` `const` `inline` |  |

---

{#socketpacket-1}

#### SocketPacket

`inline`

```cpp
inline SocketPacket(const Socket::Ptr & socket, const MutableBuffer & buffer, const Address & peerAddress)
```

Defined in src/net/include/icy/net/socket.h:210

Constructs a [SocketPacket](#socketpacket) wrapping the received buffer.

The buffer data pointer remains valid only for the duration of the enclosing receive callback; do not retain references beyond that scope. 
#### Parameters
* `socket` Shared pointer to the receiving socket. 

* `buffer` View of the raw received bytes. 

* `peerAddress` [Address](#address) of the remote sender.

---

{#socketpacket-2}

#### SocketPacket

`inline`

```cpp
inline SocketPacket(const SocketPacket & that)
```

Defined in src/net/include/icy/net/socket.h:218

Copy constructor; shares the underlying buffer reference. 
#### Parameters
* `that` Source [SocketPacket](#socketpacket) to copy.

---

{#packetinfo-3}

#### packetInfo

`const` `inline`

```cpp
inline PacketInfo * packetInfo() const
```

Defined in src/net/include/icy/net/socket.h:225

Returns the [PacketInfo](#packetinfo) for this socket packet. 
#### Returns
Pointer to the associated [PacketInfo](#packetinfo) (never null for a valid packet).

---

{#print-6}

#### print

`virtual` `const` `inline`

```cpp
virtual inline void print(std::ostream & os) const
```

Defined in src/net/include/icy/net/socket.h:234

Prints a one-line description of the packet to `os`. 
#### Parameters
* `os` Output stream to write to.

##### Reimplements

- [`print`](base.md#classicy_1_1IPacket_1acc8685ec343b3e9ed81687b8e8fa1acc)

---

{#clone-6}

#### clone

`virtual` `const` `inline`

```cpp
virtual inline std::unique_ptr< IPacket > clone() const
```

Defined in src/net/include/icy/net/socket.h:240

Returns a heap-allocated copy of this [SocketPacket](#socketpacket).

##### Reimplements

- [`clone`](base.md#classicy_1_1RawPacket_1a1ac09a8e23494bbd52939fedfe5a6a8c)

---

{#read-1}

#### read

`virtual` `inline`

```cpp
virtual inline ssize_t read(const ConstBuffer &)
```

Defined in src/net/include/icy/net/socket.h:243

Not supported; always throws std::logic_error.

##### Reimplements

- [`read`](base.md#classicy_1_1RawPacket_1a606a9688958cbc74f146d28c3042b769)

---

{#write-1}

#### write

`virtual` `const` `inline`

```cpp
virtual inline void write(Buffer & buf) const
```

Defined in src/net/include/icy/net/socket.h:251

Appends the packet payload to `buf`. 
#### Parameters
* `buf` Destination buffer to append raw bytes to.

##### Reimplements

- [`write`](base.md#classicy_1_1RawPacket_1a6864137826313bac337b46de7d8ae2f4)

---

{#classname-5}

#### className

`virtual` `const` `inline`

```cpp
virtual inline const char * className() const
```

Defined in src/net/include/icy/net/socket.h:258

#### Returns
The string "SocketPacket".

##### Reimplements

- [`className`](base.md#classicy_1_1RawPacket_1a41d94a78cbe6d6e7c074aa632deb6828)

{#ssladapter}

## SSLAdapter

```cpp
#include <icy/net/ssladapter.h>
```

```cpp
class SSLAdapter
```

Defined in src/net/include/icy/net/ssladapter.h:38

Manages the OpenSSL context and BIO buffers for an SSL socket connection.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`SSLSocket`](#net-sslsocket) | `friend` | Declared here |
| [`SSLAdapter`](#ssladapter-1) | `function` | Declared here |
| [`SSLAdapter`](#ssladapter-2) | `function` | Declared here |
| [`SSLAdapter`](#ssladapter-3) | `function` | Declared here |
| [`initClient`](#initclient) | `function` | Declared here |
| [`initServer`](#initserver) | `function` | Declared here |
| [`initialized`](#initialized-2) | `function` | Declared here |
| [`ready`](#ready) | `function` | Declared here |
| [`handshake`](#handshake) | `function` | Declared here |
| [`available`](#available-1) | `function` | Declared here |
| [`shutdown`](#shutdown-2) | `function` | Declared here |
| [`flush`](#flush-8) | `function` | Declared here |
| [`setHostname`](#sethostname) | `function` | Declared here |
| [`addIncomingData`](#addincomingdata) | `function` | Declared here |
| [`addOutgoingData`](#addoutgoingdata) | `function` | Declared here |
| [`addOutgoingData`](#addoutgoingdata-1) | `function` | Declared here |
| [`addOutgoingData`](#addoutgoingdata-2) | `function` | Declared here |
| [`_socket`](#_socket) | `variable` | Declared here |
| [`_ssl`](#_ssl) | `variable` | Declared here |
| [`_readBIO`](#_readbio) | `variable` | Declared here |
| [`_writeBIO`](#_writebio) | `variable` | Declared here |
| [`_bufferOut`](#_bufferout) | `variable` | Declared here |
| [`_hostname`](#_hostname) | `variable` | Declared here |
| [`handleError`](#handleerror) | `function` | Declared here |
| [`flushReadBIO`](#flushreadbio) | `function` | Declared here |
| [`flushWriteBIO`](#flushwritebio) | `function` | Declared here |

### Friends

| Name | Description |
|------|-------------|
| [`net::SSLSocket`](#net-sslsocket)  |  |

---

{#net-sslsocket}

#### net::SSLSocket

```cpp
friend class net::SSLSocket
```

Defined in src/net/include/icy/net/ssladapter.h:107

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`SSLAdapter`](#ssladapter-1)  | Constructs the [SSLAdapter](#ssladapter) and associates it with the given socket. The socket pointer must remain valid for the lifetime of this adapter. |
|  | [`SSLAdapter`](#ssladapter-2)  | Deleted constructor. |
|  | [`SSLAdapter`](#ssladapter-3)  | Deleted constructor. |
| `void` | [`initClient`](#initclient)  | Initializes the SSL context as a client. |
| `void` | [`initServer`](#initserver)  | Initializes the SSL context as a server. |
| `bool` | [`initialized`](#initialized-2) `const` | Returns true when SSL context has been initialized. |
| `bool` | [`ready`](#ready) `const` | Returns true when the handshake is complete. |
| `void` | [`handshake`](#handshake)  | Start/continue the SSL handshake process. |
| `int` | [`available`](#available-1) `const` | Returns the number of bytes available in the SSL buffer for immediate reading. |
| `void` | [`shutdown`](#shutdown-2)  | Issues an orderly SSL shutdown. |
| `void` | [`flush`](#flush-8)  | Flushes the SSL read/write buffers. |
| `void` | [`setHostname`](#sethostname)  | Set the expected peer hostname for certificate verification. Must be called before [initClient()](#initclient) to enable hostname verification. |
| `void` | [`addIncomingData`](#addincomingdata)  | Feeds encrypted data received from the network into the SSL read BIO. Triggers a flush, which drives the handshake or decrypts and delivers plaintext to the socket via onRecv(). |
| `void` | [`addOutgoingData`](#addoutgoingdata)  | Queues plaintext data for encryption and transmission. |
| `void` | [`addOutgoingData`](#addoutgoingdata-1)  | Queues plaintext data for encryption and transmission. |
| `void` | [`addOutgoingData`](#addoutgoingdata-2)  | Moves plaintext data into the pending write buffer when possible. |

---

{#ssladapter-1}

#### SSLAdapter

```cpp
SSLAdapter(net::SSLSocket * socket)
```

Defined in src/net/include/icy/net/ssladapter.h:44

Constructs the [SSLAdapter](#ssladapter) and associates it with the given socket. The socket pointer must remain valid for the lifetime of this adapter. 
#### Parameters
* `socket` The owning [SSLSocket](#sslsocket) that sends and receives raw data.

---

{#ssladapter-2}

#### SSLAdapter

```cpp
SSLAdapter(const SSLAdapter &) = delete
```

Defined in src/net/include/icy/net/ssladapter.h:47

Deleted constructor.

---

{#ssladapter-3}

#### SSLAdapter

```cpp
SSLAdapter(SSLAdapter &&) = delete
```

Defined in src/net/include/icy/net/ssladapter.h:49

Deleted constructor.

---

{#initclient}

#### initClient

```cpp
void initClient()
```

Defined in src/net/include/icy/net/ssladapter.h:53

Initializes the SSL context as a client.

---

{#initserver}

#### initServer

```cpp
void initServer()
```

Defined in src/net/include/icy/net/ssladapter.h:56

Initializes the SSL context as a server.

---

{#initialized-2}

#### initialized

`const`

```cpp
bool initialized() const
```

Defined in src/net/include/icy/net/ssladapter.h:59

Returns true when SSL context has been initialized.

---

{#ready}

#### ready

`const`

```cpp
bool ready() const
```

Defined in src/net/include/icy/net/ssladapter.h:62

Returns true when the handshake is complete.

---

{#handshake}

#### handshake

```cpp
void handshake()
```

Defined in src/net/include/icy/net/ssladapter.h:65

Start/continue the SSL handshake process.

---

{#available-1}

#### available

`const`

```cpp
int available() const
```

Defined in src/net/include/icy/net/ssladapter.h:69

Returns the number of bytes available in the SSL buffer for immediate reading.

---

{#shutdown-2}

#### shutdown

```cpp
void shutdown()
```

Defined in src/net/include/icy/net/ssladapter.h:72

Issues an orderly SSL shutdown.

---

{#flush-8}

#### flush

```cpp
void flush()
```

Defined in src/net/include/icy/net/ssladapter.h:75

Flushes the SSL read/write buffers.

---

{#sethostname}

#### setHostname

```cpp
void setHostname(std::string_view hostname)
```

Defined in src/net/include/icy/net/ssladapter.h:79

Set the expected peer hostname for certificate verification. Must be called before [initClient()](#initclient) to enable hostname verification.

---

{#addincomingdata}

#### addIncomingData

```cpp
void addIncomingData(const char * data, size_t len)
```

Defined in src/net/include/icy/net/ssladapter.h:86

Feeds encrypted data received from the network into the SSL read BIO. Triggers a flush, which drives the handshake or decrypts and delivers plaintext to the socket via onRecv(). 
#### Parameters
* `data` Pointer to the encrypted bytes. 

* `len` Number of bytes to feed.

---

{#addoutgoingdata}

#### addOutgoingData

```cpp
void addOutgoingData(std::string_view data)
```

Defined in src/net/include/icy/net/ssladapter.h:90

Queues plaintext data for encryption and transmission. 
#### Parameters
* `data` String view of the plaintext payload.

---

{#addoutgoingdata-1}

#### addOutgoingData

```cpp
void addOutgoingData(const char * data, size_t len)
```

Defined in src/net/include/icy/net/ssladapter.h:95

Queues plaintext data for encryption and transmission. 
#### Parameters
* `data` Pointer to the plaintext bytes. 

* `len` Number of bytes to queue.

---

{#addoutgoingdata-2}

#### addOutgoingData

```cpp
void addOutgoingData(Buffer && data)
```

Defined in src/net/include/icy/net/ssladapter.h:98

Moves plaintext data into the pending write buffer when possible.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `net::SSLSocket *` | [`_socket`](#_socket)  |  |
| `SSL *` | [`_ssl`](#_ssl)  |  |
| `BIO *` | [`_readBIO`](#_readbio)  | The incoming buffer we write encrypted SSL data into. |
| `BIO *` | [`_writeBIO`](#_writebio)  | The outgoing buffer we write to the socket. |
| `std::vector< char >` | [`_bufferOut`](#_bufferout)  | The outgoing payload to be encrypted and sent. |
| `std::string` | [`_hostname`](#_hostname)  | Expected peer hostname for verification. |

---

{#_socket}

#### _socket

```cpp
net::SSLSocket * _socket
```

Defined in src/net/include/icy/net/ssladapter.h:109

---

{#_ssl}

#### _ssl

```cpp
SSL * _ssl
```

Defined in src/net/include/icy/net/ssladapter.h:110

---

{#_readbio}

#### _readBIO

```cpp
BIO * _readBIO
```

Defined in src/net/include/icy/net/ssladapter.h:111

The incoming buffer we write encrypted SSL data into.

---

{#_writebio}

#### _writeBIO

```cpp
BIO * _writeBIO
```

Defined in src/net/include/icy/net/ssladapter.h:112

The outgoing buffer we write to the socket.

---

{#_bufferout}

#### _bufferOut

```cpp
std::vector< char > _bufferOut
```

Defined in src/net/include/icy/net/ssladapter.h:113

The outgoing payload to be encrypted and sent.

---

{#_hostname}

#### _hostname

```cpp
std::string _hostname
```

Defined in src/net/include/icy/net/ssladapter.h:114

Expected peer hostname for verification.

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`handleError`](#handleerror)  |  |
| `void` | [`flushReadBIO`](#flushreadbio)  |  |
| `void` | [`flushWriteBIO`](#flushwritebio)  |  |

---

{#handleerror}

#### handleError

```cpp
void handleError(int rc)
```

Defined in src/net/include/icy/net/ssladapter.h:101

---

{#flushreadbio}

#### flushReadBIO

```cpp
void flushReadBIO()
```

Defined in src/net/include/icy/net/ssladapter.h:103

---

{#flushwritebio}

#### flushWriteBIO

```cpp
void flushWriteBIO()
```

Defined in src/net/include/icy/net/ssladapter.h:104

{#sslcontext}

## SSLContext

```cpp
#include <icy/net/sslcontext.h>
```

```cpp
class SSLContext
```

Defined in src/net/include/icy/net/sslcontext.h:44

OpenSSL SSL_CTX wrapper for client and server TLS configuration.

This class encapsulates context information for an SSL server or client, such as the certificate verification mode and the location of certificates and private key files, as well as the list of supported ciphers.

The Context class is also used to control SSL session caching on the server and client side.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`SSLContext`](#sslcontext-1) | `function` | Declared here |
| [`SSLContext`](#sslcontext-2) | `function` | Declared here |
| [`~SSLContext`](#sslcontext-3) | `function` | Declared here |
| [`useCertificate`](#usecertificate) | `function` | Declared here |
| [`addChainCertificate`](#addchaincertificate) | `function` | Declared here |
| [`addVerificationCertificate`](#addverificationcertificate) | `function` | Declared here |
| [`sslContext`](#sslcontext-4) | `function` | Declared here |
| [`usage`](#usage) | `function` | Declared here |
| [`isForServerUse`](#isforserveruse) | `function` | Declared here |
| [`verificationMode`](#verificationmode) | `function` | Declared here |
| [`enableSessionCache`](#enablesessioncache) | `function` | Declared here |
| [`enableSessionCache`](#enablesessioncache-1) | `function` | Declared here |
| [`sessionCacheEnabled`](#sessioncacheenabled) | `function` | Declared here |
| [`setSessionCacheSize`](#setsessioncachesize) | `function` | Declared here |
| [`getSessionCacheSize`](#getsessioncachesize) | `function` | Declared here |
| [`setSessionTimeout`](#setsessiontimeout) | `function` | Declared here |
| [`getSessionTimeout`](#getsessiontimeout) | `function` | Declared here |
| [`flushSessionCache`](#flushsessioncache) | `function` | Declared here |
| [`disableStatelessSessionResumption`](#disablestatelesssessionresumption) | `function` | Declared here |
| [`setALPNProtocols`](#setalpnprotocols) | `function` | Declared here |
| [`SSLContext`](#sslcontext-5) | `function` | Declared here |
| [`SSLContext`](#sslcontext-6) | `function` | Declared here |
| [`enableSNI`](#enablesni) | `function` | Declared here |
| [`Usage`](#usage-1) | `enum` | Declared here |
| [`VerificationMode`](#verificationmode-1) | `enum` | Declared here |
| [`Ptr`](#ptr-6) | `typedef` | Declared here |
| [`_usage`](#_usage) | `variable` | Declared here |
| [`_mode`](#_mode) | `variable` | Declared here |
| [`_sslContext`](#_sslcontext) | `variable` | Declared here |
| [`_alpnWire`](#_alpnwire) | `variable` | Declared here |
| [`createSSLContext`](#createsslcontext) | `function` | Declared here |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`SSLContext`](#sslcontext-1)  | Creates a Context. |
|  | [`SSLContext`](#sslcontext-2)  | Creates a Context. |
|  | [`~SSLContext`](#sslcontext-3) `noexcept` | Destroys the Context. |
| `void` | [`useCertificate`](#usecertificate)  | Sets the certificate to be used by the Context. |
| `void` | [`addChainCertificate`](#addchaincertificate)  | Adds a certificate for certificate chain validation. |
| `void` | [`addVerificationCertificate`](#addverificationcertificate)  | Sets the private key to be used by the Context. |
| `SSL_CTX *` | [`sslContext`](#sslcontext-4) `const` `inline` | Returns the underlying OpenSSL SSL Context object. |
| `Usage` | [`usage`](#usage) `const` `inline` | Returns whether the context is for use by a client or by a server and whether TLSv1 is required. |
| `bool` | [`isForServerUse`](#isforserveruse) `const` `inline` | Returns true if the context is for use by a server. |
| `SSLContext::VerificationMode` | [`verificationMode`](#verificationmode) `const` `inline` | Returns the verification mode. |
| `void` | [`enableSessionCache`](#enablesessioncache)  | Enable or disable SSL/TLS session caching. For session caching to work, it must be enabled on the server, as well as on the client side. |
| `void` | [`enableSessionCache`](#enablesessioncache-1)  | Enables or disables SSL/TLS session caching on the server. For session caching to work, it must be enabled on the server, as well as on the client side. |
| `bool` | [`sessionCacheEnabled`](#sessioncacheenabled) `const` | Returns true if the session cache is enabled. |
| `void` | [`setSessionCacheSize`](#setsessioncachesize)  | Sets the maximum size of the server session cache, in number of sessions. The default size (according to OpenSSL documentation) is 1024*20, which may be too large for many applications, especially on embedded platforms with limited memory. |
| `size_t` | [`getSessionCacheSize`](#getsessioncachesize) `const` | Returns the current maximum size of the server session cache. |
| `void` | [`setSessionTimeout`](#setsessiontimeout)  | Sets the timeout (in seconds) of cached sessions on the server. A cached session will be removed from the cache if it has not been used for the given number of seconds. |
| `long` | [`getSessionTimeout`](#getsessiontimeout) `const` | Returns the timeout (in seconds) of cached sessions on the server. |
| `void` | [`flushSessionCache`](#flushsessioncache)  | Flushes the SSL session cache on the server. |
| `void` | [`disableStatelessSessionResumption`](#disablestatelesssessionresumption)  | Newer versions of OpenSSL support RFC 4507 tickets for stateless session resumption. |
| `void` | [`setALPNProtocols`](#setalpnprotocols)  | Set the ALPN protocols for negotiation. Protocols should be in preference order. Example: {"h2", "http/1.1"} |
|  | [`SSLContext`](#sslcontext-5)  | Deleted constructor. |
|  | [`SSLContext`](#sslcontext-6)  | Deleted constructor. |

---

{#sslcontext-1}

#### SSLContext

```cpp
SSLContext(Usage usage, const std::string & privateKeyFile, const std::string & certificateFile, const std::string & caLocation, VerificationMode verificationMode = VERIFY_RELAXED, int verificationDepth = 9, bool loadDefaultCAs = false, const std::string & cipherList = "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH")
```

Defined in src/net/include/icy/net/sslcontext.h:123

Creates a Context.

* usage specifies whether the context is used by a client or server.

* privateKeyFile contains the path to the private key file used for encryption. Can be empty if no private key file is used.

* certificateFile contains the path to the certificate file (in PEM format). If the private key and the certificate are stored in the same file, this can be empty if privateKeyFile is given.

* caLocation contains the path to the file or directory containing the CA/root certificates. Can be empty if the OpenSSL builtin CA certificates are used (see loadDefaultCAs).

* verificationMode specifies whether and how peer certificates are validated.

* verificationDepth sets the upper limit for verification chain sizes. Verification will fail if a certificate chain larger than this is encountered.

* loadDefaultCAs specifies whether the builtin CA certificates from OpenSSL are used.

* cipherList specifies the supported ciphers in OpenSSL notation.

Note: If the private key is protected by a passphrase, a PrivateKeyPassphraseHandler must have been setup with the [SSLManager](#sslmanager), or the [SSLManager](#sslmanager)'s PrivateKeyPassphraseRequired event must be handled.

---

{#sslcontext-2}

#### SSLContext

```cpp
SSLContext(Usage usage, const std::string & caLocation, VerificationMode verificationMode = VERIFY_RELAXED, int verificationDepth = 9, bool loadDefaultCAs = false, const std::string & cipherList = "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH")
```

Defined in src/net/include/icy/net/sslcontext.h:148

Creates a Context.

* usage specifies whether the context is used by a client or server.

* caLocation contains the path to the file or directory containing the CA/root certificates. Can be empty if the OpenSSL builtin CA certificates are used (see loadDefaultCAs).

* verificationMode specifies whether and how peer certificates are validated.

* verificationDepth sets the upper limit for verification chain sizes. Verification will fail if a certificate chain larger than this is encountered.

* loadDefaultCAs specifies whether the builtin CA certificates from OpenSSL are used.

* cipherList specifies the supported ciphers in OpenSSL notation.

Note that a private key and/or certificate must be specified with usePrivateKey()/useCertificate() before the Context can be used.

---

{#sslcontext-3}

#### ~SSLContext

`noexcept`

```cpp
~SSLContext() noexcept
```

Defined in src/net/include/icy/net/sslcontext.h:155

Destroys the Context.

---

{#usecertificate}

#### useCertificate

```cpp
void useCertificate(crypto::X509Certificate & certificate)
```

Defined in src/net/include/icy/net/sslcontext.h:165

Sets the certificate to be used by the Context.

To set-up a complete certificate chain, it might be necessary to call [addChainCertificate()](#addchaincertificate) to specify additional certificates.

Note that [useCertificate()](#usecertificate) must always be called before usePrivateKey().

---

{#addchaincertificate}

#### addChainCertificate

```cpp
void addChainCertificate(crypto::X509Certificate & certificate)
```

Defined in src/net/include/icy/net/sslcontext.h:168

Adds a certificate for certificate chain validation.

---

{#addverificationcertificate}

#### addVerificationCertificate

```cpp
void addVerificationCertificate(crypto::X509Certificate & certificate)
```

Defined in src/net/include/icy/net/sslcontext.h:184

Sets the private key to be used by the Context.

Note that [useCertificate()](#usecertificate) must always be called before usePrivateKey().

Note: If the private key is protected by a passphrase, a PrivateKeyPassphraseHandler must have been setup with the [SSLManager](#sslmanager), or the [SSLManager](#sslmanager)'s PrivateKeyPassphraseRequired event must be handled. Adds the given certificate to the list of trusted certificates that will be used for verification.

---

{#sslcontext-4}

#### sslContext

`const` `inline`

```cpp
inline SSL_CTX * sslContext() const
```

Defined in src/net/include/icy/net/sslcontext.h:187

Returns the underlying OpenSSL SSL Context object.

---

{#usage}

#### usage

`const` `inline`

```cpp
inline Usage usage() const
```

Defined in src/net/include/icy/net/sslcontext.h:191

Returns whether the context is for use by a client or by a server and whether TLSv1 is required.

---

{#isforserveruse}

#### isForServerUse

`const` `inline`

```cpp
inline bool isForServerUse() const
```

Defined in src/net/include/icy/net/sslcontext.h:194

Returns true if the context is for use by a server.

---

{#verificationmode}

#### verificationMode

`const` `inline`

```cpp
inline SSLContext::VerificationMode verificationMode() const
```

Defined in src/net/include/icy/net/sslcontext.h:197

Returns the verification mode.

---

{#enablesessioncache}

#### enableSessionCache

```cpp
void enableSessionCache(bool flag = true)
```

Defined in src/net/include/icy/net/sslcontext.h:208

Enable or disable SSL/TLS session caching. For session caching to work, it must be enabled on the server, as well as on the client side.

The default is disabled session caching.

To enable session caching on the server side, use the two-argument version of this method to specify a session ID context.

---

{#enablesessioncache-1}

#### enableSessionCache

```cpp
void enableSessionCache(bool flag, std::string_view sessionIdContext)
```

Defined in src/net/include/icy/net/sslcontext.h:225

Enables or disables SSL/TLS session caching on the server. For session caching to work, it must be enabled on the server, as well as on the client side.

SessionIdContext contains the application's unique session ID context, which becomes part of each session identifier generated by the server within this context. SessionIdContext can be an arbitrary sequence of bytes with a maximum length of SSL_MAX_SSL_SESSION_ID_LENGTH.

A non-empty sessionIdContext should be specified even if session caching is disabled to avoid problems with clients requesting to reuse a session (e.g. Firefox 3.6).

This method may only be called on SERVER_USE Context objects.

---

{#sessioncacheenabled}

#### sessionCacheEnabled

`const`

```cpp
bool sessionCacheEnabled() const
```

Defined in src/net/include/icy/net/sslcontext.h:228

Returns true if the session cache is enabled.

---

{#setsessioncachesize}

#### setSessionCacheSize

```cpp
void setSessionCacheSize(size_t size)
```

Defined in src/net/include/icy/net/sslcontext.h:238

Sets the maximum size of the server session cache, in number of sessions. The default size (according to OpenSSL documentation) is 1024*20, which may be too large for many applications, especially on embedded platforms with limited memory.

Specifying a size of 0 will set an unlimited cache size.

This method may only be called on SERVER_USE Context objects.

---

{#getsessioncachesize}

#### getSessionCacheSize

`const`

```cpp
size_t getSessionCacheSize() const
```

Defined in src/net/include/icy/net/sslcontext.h:243

Returns the current maximum size of the server session cache.

This method may only be called on SERVER_USE Context objects.

---

{#setsessiontimeout}

#### setSessionTimeout

```cpp
void setSessionTimeout(long seconds)
```

Defined in src/net/include/icy/net/sslcontext.h:250

Sets the timeout (in seconds) of cached sessions on the server. A cached session will be removed from the cache if it has not been used for the given number of seconds.

This method may only be called on SERVER_USE Context objects.

---

{#getsessiontimeout}

#### getSessionTimeout

`const`

```cpp
long getSessionTimeout() const
```

Defined in src/net/include/icy/net/sslcontext.h:255

Returns the timeout (in seconds) of cached sessions on the server.

This method may only be called on SERVER_USE Context objects.

---

{#flushsessioncache}

#### flushSessionCache

```cpp
void flushSessionCache()
```

Defined in src/net/include/icy/net/sslcontext.h:260

Flushes the SSL session cache on the server.

This method may only be called on SERVER_USE Context objects.

---

{#disablestatelesssessionresumption}

#### disableStatelessSessionResumption

```cpp
void disableStatelessSessionResumption()
```

Defined in src/net/include/icy/net/sslcontext.h:266

Newer versions of OpenSSL support RFC 4507 tickets for stateless session resumption.

The feature can be disabled by calling this method.

---

{#setalpnprotocols}

#### setALPNProtocols

```cpp
void setALPNProtocols(const std::vector< std::string > & protocols)
```

Defined in src/net/include/icy/net/sslcontext.h:271

Set the ALPN protocols for negotiation. Protocols should be in preference order. Example: {"h2", "http/1.1"}

---

{#sslcontext-5}

#### SSLContext

```cpp
SSLContext(const SSLContext &) = delete
```

Defined in src/net/include/icy/net/sslcontext.h:278

Deleted constructor.

---

{#sslcontext-6}

#### SSLContext

```cpp
SSLContext(SSLContext &&) = delete
```

Defined in src/net/include/icy/net/sslcontext.h:280

Deleted constructor.

### Public Static Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`enableSNI`](#enablesni) `static` | Enable SNI (Server Name Indication) for a specific SSL connection. The hostname is sent during the TLS handshake to allow the server to select the appropriate certificate. |

---

{#enablesni}

#### enableSNI

`static`

```cpp
static void enableSNI(SSL * ssl, const std::string & hostname)
```

Defined in src/net/include/icy/net/sslcontext.h:276

Enable SNI (Server Name Indication) for a specific SSL connection. The hostname is sent during the TLS handshake to allow the server to select the appropriate certificate.

### Public Types

| Name | Description |
|------|-------------|
| [`Usage`](#usage-1)  |  |
| [`VerificationMode`](#verificationmode-1)  |  |
| [`Ptr`](#ptr-6)  |  |

---

{#usage-1}

#### Usage

```cpp
enum Usage
```

Defined in src/net/include/icy/net/sslcontext.h:49

| Value | Description |
|-------|-------------|
| `CLIENT_USE` | Context is used by a client. |
| `SERVER_USE` | Context is used by a server. |
| `TLSV1_CLIENT_USE` | Context is used by a client requiring TLSv1. |
| `TLSV1_SERVER_USE` | Context is used by a server requiring TLSv2. |

---

{#verificationmode-1}

#### VerificationMode

```cpp
enum VerificationMode
```

Defined in src/net/include/icy/net/sslcontext.h:57

| Value | Description |
|-------|-------------|
| `VERIFY_NONE` | Server: The server will not send a client certificate request to the client, so the client will not send a certificate. |
| `VERIFY_RELAXED` | Server: The server sends a client certificate request to the client. The certificate returned (if any) is checked. If the verification process fails, the TLS/SSL handshake is immediately terminated with an alert message containing the reason for the verification failure. |
| `VERIFY_STRICT` | Server: If the client did not return a certificate, the TLS/SSL handshake is immediately terminated with a handshake failure alert. |
| `VERIFY_ONCE` | Server: Only request a client certificate on the initial TLS/SSL handshake. Do not ask for a client certificate again in case of a renegotiation. |

---

{#ptr-6}

#### Ptr

```cpp
using Ptr = std::shared_ptr< SSLContext >
```

Defined in src/net/include/icy/net/sslcontext.h:47

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `Usage` | [`_usage`](#_usage)  |  |
| `VerificationMode` | [`_mode`](#_mode)  |  |
| `SSL_CTX *` | [`_sslContext`](#_sslcontext)  |  |
| `std::vector< unsigned char >` | [`_alpnWire`](#_alpnwire)  | Wire-format ALPN protocols for server selection. |

---

{#_usage}

#### _usage

```cpp
Usage _usage
```

Defined in src/net/include/icy/net/sslcontext.h:287

---

{#_mode}

#### _mode

```cpp
VerificationMode _mode
```

Defined in src/net/include/icy/net/sslcontext.h:288

---

{#_sslcontext}

#### _sslContext

```cpp
SSL_CTX * _sslContext
```

Defined in src/net/include/icy/net/sslcontext.h:289

---

{#_alpnwire}

#### _alpnWire

```cpp
std::vector< unsigned char > _alpnWire
```

Defined in src/net/include/icy/net/sslcontext.h:290

Wire-format ALPN protocols for server selection.

### Private Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`createSSLContext`](#createsslcontext)  | Create a SSL_CTX object according to Context configuration. |

---

{#createsslcontext}

#### createSSLContext

```cpp
void createSSLContext()
```

Defined in src/net/include/icy/net/sslcontext.h:285

Create a SSL_CTX object according to Context configuration.

{#sslmanager}

## SSLManager

```cpp
#include <icy/net/sslmanager.h>
```

```cpp
class SSLManager
```

Defined in src/net/include/icy/net/sslmanager.h:31

[Singleton](base.md#singleton) that owns the default client/server TLS contexts and related callbacks.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`Singleton< SSLManager >`](#singletonsslmanager) | `friend` | Declared here |
| [`SSLContext`](#sslcontext-7) | `friend` | Declared here |
| [`ServerVerificationError`](#serververificationerror) | `variable` | Declared here |
| [`ClientVerificationError`](#clientverificationerror) | `variable` | Declared here |
| [`PrivateKeyPassphraseRequired`](#privatekeypassphraserequired) | `variable` | Declared here |
| [`initializeServer`](#initializeserver) | `function` | Declared here |
| [`initializeClient`](#initializeclient) | `function` | Declared here |
| [`defaultServerContext`](#defaultservercontext) | `function` | Declared here |
| [`defaultClientContext`](#defaultclientcontext) | `function` | Declared here |
| [`shutdown`](#shutdown-3) | `function` | Declared here |
| [`instance`](#instance-2) | `function` | Declared here |
| [`destroy`](#destroy) | `function` | Declared here |
| [`initNoVerifyClient`](#initnoverifyclient) | `function` | Declared here |
| [`initNoVerifyServer`](#initnoverifyserver) | `function` | Declared here |
| [`_defaultServerContext`](#_defaultservercontext) | `variable` | Declared here |
| [`_defaultClientContext`](#_defaultclientcontext) | `variable` | Declared here |
| [`_mutex`](#_mutex-6) | `variable` | Declared here |
| [`SSLManager`](#sslmanager-1) | `function` | Declared here |
| [`~SSLManager`](#sslmanager-2) | `function` | Declared here |
| [`SSLManager`](#sslmanager-3) | `function` | Declared here |
| [`SSLManager`](#sslmanager-4) | `function` | Declared here |
| [`verifyCallback`](#verifycallback) | `function` | Declared here |

### Friends

| Name | Description |
|------|-------------|
| [`Singleton< SSLManager >`](#singletonsslmanager)  |  |
| [`SSLContext`](#sslcontext-7)  |  |

---

{#singletonsslmanager}

#### Singleton< SSLManager >

```cpp
friend class Singleton< SSLManager >
```

Defined in src/net/include/icy/net/sslmanager.h:122

---

{#sslcontext-7}

#### SSLContext

```cpp
friend class SSLContext
```

Defined in src/net/include/icy/net/sslmanager.h:125

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `ThreadSignal< void(VerificationErrorDetails &)>` | [`ServerVerificationError`](#serververificationerror)  | Fired whenever a certificate verification error is detected by the server during a handshake. |
| `ThreadSignal< void(VerificationErrorDetails &)>` | [`ClientVerificationError`](#clientverificationerror)  | Fired whenever a certificate verification error is detected by the client during a handshake. |
| `ThreadSignal< void(std::string &)>` | [`PrivateKeyPassphraseRequired`](#privatekeypassphraserequired)  | Fired when an encrypted certificate or private key is loaded. Not setting the password in the event parameter will result in a failure to load the certificate. |

---

{#serververificationerror}

#### ServerVerificationError

```cpp
ThreadSignal< void(VerificationErrorDetails &)> ServerVerificationError
```

Defined in src/net/include/icy/net/sslmanager.h:49

Fired whenever a certificate verification error is detected by the server during a handshake.

---

{#clientverificationerror}

#### ClientVerificationError

```cpp
ThreadSignal< void(VerificationErrorDetails &)> ClientVerificationError
```

Defined in src/net/include/icy/net/sslmanager.h:53

Fired whenever a certificate verification error is detected by the client during a handshake.

---

{#privatekeypassphraserequired}

#### PrivateKeyPassphraseRequired

```cpp
ThreadSignal< void(std::string &)> PrivateKeyPassphraseRequired
```

Defined in src/net/include/icy/net/sslmanager.h:57

Fired when an encrypted certificate or private key is loaded. Not setting the password in the event parameter will result in a failure to load the certificate.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`initializeServer`](#initializeserver)  | Initializes the server side of the [SSLManager](#sslmanager) server-side [SSLContext](#sslcontext). |
| `void` | [`initializeClient`](#initializeclient)  | Initializes the client side of the [SSLManager](#sslmanager) with a default client-side [SSLContext](#sslcontext). |
| `SSLContext::Ptr` | [`defaultServerContext`](#defaultservercontext)  | Returns the default context used by the server if initialized. |
| `SSLContext::Ptr` | [`defaultClientContext`](#defaultclientcontext)  | Returns the default context used by the client if initialized. |
| `void` | [`shutdown`](#shutdown-3)  | Shuts down the [SSLManager](#sslmanager) and releases the default context objects. After a call to [shutdown()](#shutdown-3), the [SSLManager](#sslmanager) can no longer be used. |

---

{#initializeserver}

#### initializeServer

```cpp
void initializeServer(SSLContext::Ptr ptrContext)
```

Defined in src/net/include/icy/net/sslmanager.h:35

Initializes the server side of the [SSLManager](#sslmanager) server-side [SSLContext](#sslcontext).

---

{#initializeclient}

#### initializeClient

```cpp
void initializeClient(SSLContext::Ptr ptrContext)
```

Defined in src/net/include/icy/net/sslmanager.h:39

Initializes the client side of the [SSLManager](#sslmanager) with a default client-side [SSLContext](#sslcontext).

---

{#defaultservercontext}

#### defaultServerContext

```cpp
SSLContext::Ptr defaultServerContext()
```

Defined in src/net/include/icy/net/sslmanager.h:42

Returns the default context used by the server if initialized.

---

{#defaultclientcontext}

#### defaultClientContext

```cpp
SSLContext::Ptr defaultClientContext()
```

Defined in src/net/include/icy/net/sslmanager.h:45

Returns the default context used by the client if initialized.

---

{#shutdown-3}

#### shutdown

```cpp
void shutdown()
```

Defined in src/net/include/icy/net/sslmanager.h:66

Shuts down the [SSLManager](#sslmanager) and releases the default context objects. After a call to [shutdown()](#shutdown-3), the [SSLManager](#sslmanager) can no longer be used.

Normally, it's not necessary to call this method directly, as this will be called either by uninitializeSSL(), or when the [SSLManager](#sslmanager) instance is destroyed.

### Public Static Methods

| Return | Name | Description |
|--------|------|-------------|
| `SSLManager &` | [`instance`](#instance-2) `static` | Returns the instance of the [SSLManager](#sslmanager) singleton. |
| `void` | [`destroy`](#destroy) `static` | Shuts down and destroys the [SSLManager](#sslmanager) singleton instance. |
| `void` | [`initNoVerifyClient`](#initnoverifyclient) `static` | Initializes a default no-verify client context that's useful for testing. |
| `void` | [`initNoVerifyServer`](#initnoverifyserver) `static` | Initializes a default no-verify server context that's useful for testing. Optionally accepts private key and certificate file paths for server identity; if omitted, no certificate is loaded. |

---

{#instance-2}

#### instance

`static`

```cpp
static SSLManager & instance()
```

Defined in src/net/include/icy/net/sslmanager.h:69

Returns the instance of the [SSLManager](#sslmanager) singleton.

---

{#destroy}

#### destroy

`static`

```cpp
static void destroy()
```

Defined in src/net/include/icy/net/sslmanager.h:72

Shuts down and destroys the [SSLManager](#sslmanager) singleton instance.

---

{#initnoverifyclient}

#### initNoVerifyClient

`static`

```cpp
static void initNoVerifyClient()
```

Defined in src/net/include/icy/net/sslmanager.h:76

Initializes a default no-verify client context that's useful for testing.

---

{#initnoverifyserver}

#### initNoVerifyServer

`static`

```cpp
static void initNoVerifyServer(const std::string & privateKeyFile = "", const std::string & certificateFile = "")
```

Defined in src/net/include/icy/net/sslmanager.h:81

Initializes a default no-verify server context that's useful for testing. Optionally accepts private key and certificate file paths for server identity; if omitted, no certificate is loaded.

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `SSLContext::Ptr` | [`_defaultServerContext`](#_defaultservercontext)  |  |
| `SSLContext::Ptr` | [`_defaultClientContext`](#_defaultclientcontext)  |  |
| `std::mutex` | [`_mutex`](#_mutex-6)  |  |

---

{#_defaultservercontext}

#### _defaultServerContext

```cpp
SSLContext::Ptr _defaultServerContext
```

Defined in src/net/include/icy/net/sslmanager.h:120

---

{#_defaultclientcontext}

#### _defaultClientContext

```cpp
SSLContext::Ptr _defaultClientContext
```

Defined in src/net/include/icy/net/sslmanager.h:121

---

{#_mutex-6}

#### _mutex

```cpp
std::mutex _mutex
```

Defined in src/net/include/icy/net/sslmanager.h:122

### Private Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`SSLManager`](#sslmanager-1)  | Creates the [SSLManager](#sslmanager). |
|  | [`~SSLManager`](#sslmanager-2) `noexcept` | Destroys the [SSLManager](#sslmanager). |
|  | [`SSLManager`](#sslmanager-3)  | Deleted constructor. |
|  | [`SSLManager`](#sslmanager-4)  | Deleted constructor. |

---

{#sslmanager-1}

#### SSLManager

```cpp
SSLManager()
```

Defined in src/net/include/icy/net/sslmanager.h:105

Creates the [SSLManager](#sslmanager).

---

{#sslmanager-2}

#### ~SSLManager

`noexcept`

```cpp
~SSLManager() noexcept
```

Defined in src/net/include/icy/net/sslmanager.h:108

Destroys the [SSLManager](#sslmanager).

---

{#sslmanager-3}

#### SSLManager

```cpp
SSLManager(const SSLManager &) = delete
```

Defined in src/net/include/icy/net/sslmanager.h:110

Deleted constructor.

---

{#sslmanager-4}

#### SSLManager

```cpp
SSLManager(SSLManager &&) = delete
```

Defined in src/net/include/icy/net/sslmanager.h:112

Deleted constructor.

### Private Static Methods

| Return | Name | Description |
|--------|------|-------------|
| `int` | [`verifyCallback`](#verifycallback) `static` | The return value of this method defines how errors in verification are handled. Return 0 to terminate the handshake, or 1 to continue despite the error. |

---

{#verifycallback}

#### verifyCallback

`static`

```cpp
static int verifyCallback(bool server, int ok, X509_STORE_CTX * pStore)
```

Defined in src/net/include/icy/net/sslmanager.h:118

The return value of this method defines how errors in verification are handled. Return 0 to terminate the handshake, or 1 to continue despite the error.

{#sslsession}

## SSLSession

```cpp
#include <icy/net/sslsession.h>
```

```cpp
class SSLSession
```

Defined in src/net/include/icy/net/sslsession.h:31

Cached SSL/TLS session wrapper used for client-side resumption.

For session caching to work, a client must save the session object from an existing connection, if it wants to reuse it with a future connection.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`sslSession`](#sslsession-1) | `function` | Declared here |
| [`SSLSession`](#sslsession-2) | `function` | Declared here |
| [`~SSLSession`](#sslsession-3) | `function` | Declared here |
| [`SSLSession`](#sslsession-4) | `function` | Declared here |
| [`SSLSession`](#sslsession-5) | `function` | Declared here |
| [`SSLSession`](#sslsession-6) | `function` | Declared here |
| [`_ptr`](#_ptr) | `variable` | Declared here |
| [`Ptr`](#ptr-7) | `typedef` | Declared here |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
| `SSL_SESSION *` | [`sslSession`](#sslsession-1) `const` | Returns the stored OpenSSL SSL_SESSION object. |
|  | [`SSLSession`](#sslsession-2)  | Creates a new [SSLSession](#sslsession) wrapping the given OpenSSL session object. |
|  | [`~SSLSession`](#sslsession-3) `noexcept` | Destroys the Session. |
|  | [`SSLSession`](#sslsession-4)  | Constructs an empty [SSLSession](#sslsession) with a null session pointer. |
|  | [`SSLSession`](#sslsession-5)  | Deleted constructor. |
|  | [`SSLSession`](#sslsession-6)  | Deleted constructor. |

---

{#sslsession-1}

#### sslSession

`const`

```cpp
SSL_SESSION * sslSession() const
```

Defined in src/net/include/icy/net/sslsession.h:37

Returns the stored OpenSSL SSL_SESSION object.

---

{#sslsession-2}

#### SSLSession

```cpp
SSLSession(SSL_SESSION * ptr)
```

Defined in src/net/include/icy/net/sslsession.h:44

Creates a new [SSLSession](#sslsession) wrapping the given OpenSSL session object.

The SSL_SESSION's reference count is not incremented; the [SSLSession](#sslsession) takes ownership and will call SSL_SESSION_free() on destruction. 
#### Parameters
* `ptr` The OpenSSL session object to wrap.

---

{#sslsession-3}

#### ~SSLSession

`noexcept`

```cpp
~SSLSession() noexcept
```

Defined in src/net/include/icy/net/sslsession.h:50

Destroys the Session.

Calls SSL_SESSION_free() on the stored SSL_SESSION object.

---

{#sslsession-4}

#### SSLSession

```cpp
SSLSession()
```

Defined in src/net/include/icy/net/sslsession.h:53

Constructs an empty [SSLSession](#sslsession) with a null session pointer.

---

{#sslsession-5}

#### SSLSession

```cpp
SSLSession(const SSLSession &) = delete
```

Defined in src/net/include/icy/net/sslsession.h:55

Deleted constructor.

---

{#sslsession-6}

#### SSLSession

```cpp
SSLSession(SSLSession &&) = delete
```

Defined in src/net/include/icy/net/sslsession.h:57

Deleted constructor.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `SSL_SESSION *` | [`_ptr`](#_ptr)  |  |

---

{#_ptr}

#### _ptr

```cpp
SSL_SESSION * _ptr
```

Defined in src/net/include/icy/net/sslsession.h:61

### Public Types

| Name | Description |
|------|-------------|
| [`Ptr`](#ptr-7)  |  |

---

{#ptr-7}

#### Ptr

```cpp
using Ptr = std::shared_ptr< SSLSession >
```

Defined in src/net/include/icy/net/sslsession.h:34

{#sslsocket}

## SSLSocket

```cpp
#include <icy/net/sslsocket.h>
```

```cpp
class SSLSocket
```

Defined in src/net/include/icy/net/sslsocket.h:30

> **Inherits:** [`TCPSocket`](#tcpsocket)

SSL socket implementation.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`SSLAdapter`](#net-ssladapter) | `friend` | Declared here |
| [`SSLSocket`](#sslsocket-1) | `function` | Declared here |
| [`SSLSocket`](#sslsocket-2) | `function` | Declared here |
| [`SSLSocket`](#sslsocket-3) | `function` | Declared here |
| [`connect`](#connect-5) | `function` | Declared here |
| [`connect`](#connect-6) | `function` | Declared here |
| [`bind`](#bind-1) | `function` | Declared here |
| [`listen`](#listen-1) | `function` | Declared here |
| [`shutdown`](#shutdown-4) | `function` | Declared here |
| [`close`](#close-14) | `function` | Declared here |
| [`send`](#send-2) | `function` | Declared here |
| [`sendOwned`](#sendowned-4) | `function` | Declared here |
| [`send`](#send-3) | `function` | Declared here |
| [`sendOwned`](#sendowned-5) | `function` | Declared here |
| [`setHostname`](#sethostname-1) | `function` | Declared here |
| [`useContext`](#usecontext) | `function` | Declared here |
| [`context`](#context-4) | `function` | Declared here |
| [`useSession`](#usesession) | `function` | Declared here |
| [`currentSession`](#currentsession) | `function` | Declared here |
| [`sessionWasReused`](#sessionwasreused) | `function` | Declared here |
| [`available`](#available-2) | `function` | Declared here |
| [`peerCertificate`](#peercertificate) | `function` | Declared here |
| [`transport`](#transport-1) | `function` | Declared here |
| [`acceptConnection`](#acceptconnection) | `function` | Declared here |
| [`onConnect`](#onconnect) | `function` | Declared here |
| [`onRead`](#onread) | `function` | Declared here |
| [`_sslContext`](#_sslcontext-1) | `variable` | Declared here |
| [`_sslSession`](#_sslsession) | `variable` | Declared here |
| [`_sslAdapter`](#_ssladapter) | `variable` | Declared here |
| [`Ptr`](#ptr-8) | `typedef` | Declared here |
| [`Vec`](#vec-1) | `typedef` | Declared here |
| [`AcceptConnection`](#acceptconnection-1) | `variable` | Inherited from [`TCPSocket`](#tcpsocket) |
| [`TCPSocket`](#tcpsocket-1) | `function` | Inherited from [`TCPSocket`](#tcpsocket) |
| [`TCPSocket`](#tcpsocket-2) | `function` | Inherited from [`TCPSocket`](#tcpsocket) |
| [`TCPSocket`](#tcpsocket-3) | `function` | Inherited from [`TCPSocket`](#tcpsocket) |
| [`shutdown`](#shutdown-5) | `function` | Inherited from [`TCPSocket`](#tcpsocket) |
| [`close`](#close-15) | `function` | Inherited from [`TCPSocket`](#tcpsocket) |
| [`connect`](#connect-7) | `function` | Inherited from [`TCPSocket`](#tcpsocket) |
| [`connect`](#connect-8) | `function` | Inherited from [`TCPSocket`](#tcpsocket) |
| [`send`](#send-4) | `function` | Inherited from [`TCPSocket`](#tcpsocket) |
| [`sendOwned`](#sendowned-6) | `function` | Inherited from [`TCPSocket`](#tcpsocket) |
| [`send`](#send-5) | `function` | Inherited from [`TCPSocket`](#tcpsocket) |
| [`sendOwned`](#sendowned-7) | `function` | Inherited from [`TCPSocket`](#tcpsocket) |
| [`bind`](#bind-2) | `function` | Inherited from [`TCPSocket`](#tcpsocket) |
| [`listen`](#listen-2) | `function` | Inherited from [`TCPSocket`](#tcpsocket) |
| [`acceptConnection`](#acceptconnection-2) | `function` | Inherited from [`TCPSocket`](#tcpsocket) |
| [`setReusePort`](#setreuseport) | `function` | Inherited from [`TCPSocket`](#tcpsocket) |
| [`setNoDelay`](#setnodelay) | `function` | Inherited from [`TCPSocket`](#tcpsocket) |
| [`setKeepAlive`](#setkeepalive) | `function` | Inherited from [`TCPSocket`](#tcpsocket) |
| [`setSimultaneousAccepts`](#setsimultaneousaccepts) | `function` | Inherited from [`TCPSocket`](#tcpsocket) |
| [`setMode`](#setmode) | `function` | Inherited from [`TCPSocket`](#tcpsocket) |
| [`mode`](#mode) | `function` | Inherited from [`TCPSocket`](#tcpsocket) |
| [`setError`](#seterror-2) | `function` | Inherited from [`TCPSocket`](#tcpsocket) |
| [`error`](#error-7) | `function` | Inherited from [`TCPSocket`](#tcpsocket) |
| [`closed`](#closed-2) | `function` | Inherited from [`TCPSocket`](#tcpsocket) |
| [`address`](#address-9) | `function` | Inherited from [`TCPSocket`](#tcpsocket) |
| [`peerAddress`](#peeraddress-2) | `function` | Inherited from [`TCPSocket`](#tcpsocket) |
| [`transport`](#transport-2) | `function` | Inherited from [`TCPSocket`](#tcpsocket) |
| [`loop`](#loop-4) | `function` | Inherited from [`TCPSocket`](#tcpsocket) |
| [`onConnect`](#onconnect-1) | `function` | Inherited from [`TCPSocket`](#tcpsocket) |
| [`onRead`](#onread-1) | `function` | Inherited from [`TCPSocket`](#tcpsocket) |
| [`onRecv`](#onrecv) | `function` | Inherited from [`TCPSocket`](#tcpsocket) |
| [`onError`](#onerror-1) | `function` | Inherited from [`TCPSocket`](#tcpsocket) |
| [`onClose`](#onclose-1) | `function` | Inherited from [`TCPSocket`](#tcpsocket) |
| [`_mode`](#_mode-1) | `variable` | Inherited from [`TCPSocket`](#tcpsocket) |
| [`_peerAddress`](#_peeraddress) | `variable` | Inherited from [`TCPSocket`](#tcpsocket) |
| [`init`](#init-5) | `function` | Inherited from [`TCPSocket`](#tcpsocket) |
| [`reset`](#reset-5) | `function` | Inherited from [`TCPSocket`](#tcpsocket) |
| [`Ptr`](#ptr-9) | `typedef` | Inherited from [`TCPSocket`](#tcpsocket) |
| [`Vec`](#vec-2) | `typedef` | Inherited from [`TCPSocket`](#tcpsocket) |
| [`Handle`](base.md#classicy_1_1Stream_1a3cffeb0dacfbc1ced054bd40ba0d198a) | `typedef` | Inherited from [`Stream`](base.md#stream) |
| [`Read`](base.md#classicy_1_1Stream_1a97b040391e1972b8599599a101be2184) | `variable` | Inherited from [`Stream`](base.md#stream) |
| [`_buffer`](base.md#classicy_1_1Stream_1a947a340e14aaeaa8164d5d822a327857) | `variable` | Inherited from [`Stream`](base.md#stream) |
| [`_started`](base.md#classicy_1_1Stream_1a9db4e719d928ab7ee93cbe5672fa8271) | `variable` | Inherited from [`Stream`](base.md#stream) |
| [`_highWaterMark`](base.md#classicy_1_1Stream_1aa05d9882aaf5b99452c81ccb7b9effdf) | `variable` | Inherited from [`Stream`](base.md#stream) |
| [`_writeReqFree`](base.md#classicy_1_1Stream_1a37c46db727e34bf48851b3f2c7e40b0b) | `variable` | Inherited from [`Stream`](base.md#stream) |
| [`_ownedWriteReqFree`](base.md#classicy_1_1Stream_1abfe77906ad79a929cf4c9f3dac6d3365) | `variable` | Inherited from [`Stream`](base.md#stream) |
| [`Stream`](base.md#classicy_1_1Stream_1aa89bd769ee5af9812776575c114ced1a) | `function` | Inherited from [`Stream`](base.md#stream) |
| [`~Stream`](base.md#classicy_1_1Stream_1a846a88720d4484531aa275e9158e7e8a) | `function` | Inherited from [`Stream`](base.md#stream) |
| [`close`](base.md#classicy_1_1Stream_1a702473d53d40bc5f19b997a84817a341) | `function` | Inherited from [`Stream`](base.md#stream) |
| [`shutdown`](base.md#classicy_1_1Stream_1af4524ac354596db24f2b8de1fde027ba) | `function` | Inherited from [`Stream`](base.md#stream) |
| [`write`](base.md#classicy_1_1Stream_1a56926a6ac3ba433aed1414ffb1d20cf0) | `function` | Inherited from [`Stream`](base.md#stream) |
| [`writeOwned`](base.md#classicy_1_1Stream_1a2cd782ba47e07ad03ea83eb088780fa2) | `function` | Inherited from [`Stream`](base.md#stream) |
| [`setHighWaterMark`](base.md#classicy_1_1Stream_1af8bea25a99110f9ebdd4c61a70750961) | `function` | Inherited from [`Stream`](base.md#stream) |
| [`write`](base.md#classicy_1_1Stream_1a01d508b9541f4c4ff983be14887f8ee1) | `function` | Inherited from [`Stream`](base.md#stream) |
| [`stream`](base.md#classicy_1_1Stream_1a5d9205b236601db698897ada20795c1a) | `function` | Inherited from [`Stream`](base.md#stream) |
| [`readStart`](base.md#classicy_1_1Stream_1a56e0d361385f0b1a3795979c6de1b889) | `function` | Inherited from [`Stream`](base.md#stream) |
| [`readStop`](base.md#classicy_1_1Stream_1ae5ae85e023f0e245cbc56d0956baa74e) | `function` | Inherited from [`Stream`](base.md#stream) |
| [`onRead`](base.md#classicy_1_1Stream_1ad60923c284f29eae162e2d977ebebcab) | `function` | Inherited from [`Stream`](base.md#stream) |
| [`allocWriteReq`](base.md#classicy_1_1Stream_1a12dd6ceb997f9c5ebcdaae1baf203d8a) | `function` | Inherited from [`Stream`](base.md#stream) |
| [`freeWriteReq`](base.md#classicy_1_1Stream_1a2d8d36707eb1844afcd08557d2702e36) | `function` | Inherited from [`Stream`](base.md#stream) |
| [`allocOwnedWriteReq`](base.md#classicy_1_1Stream_1a64c87a4f11e5c9cbf4e9700b8d01bfb5) | `function` | Inherited from [`Stream`](base.md#stream) |
| [`freeOwnedWriteReq`](base.md#classicy_1_1Stream_1ace2be7e7601ac8c112180e2a7b9dd026) | `function` | Inherited from [`Stream`](base.md#stream) |
| [`canQueueWrite`](base.md#classicy_1_1Stream_1ae1087b59e3ef730967e0beaa7b3a9561) | `function` | Inherited from [`Stream`](base.md#stream) |
| [`handleRead`](base.md#classicy_1_1Stream_1ac74fea672c0d281f2a4f51bee6943b10) | `function` | Inherited from [`Stream`](base.md#stream) |
| [`allocReadBuffer`](base.md#classicy_1_1Stream_1a4b2854f2d85d98ea51f012f3b0b5c099) | `function` | Inherited from [`Stream`](base.md#stream) |
| [`Handle`](uv.md#handle-3) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`init`](uv.md#init-2) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`invoke`](uv.md#invoke) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`invokeOrThrow`](uv.md#invokeorthrow) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`close`](uv.md#close-11) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`ref`](uv.md#ref) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`unref`](uv.md#unref) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`initialized`](uv.md#initialized-1) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`active`](uv.md#active) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`closing`](uv.md#closing-1) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`closed`](uv.md#closed) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`error`](uv.md#error-4) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`setError`](uv.md#seterror) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`setUVError`](uv.md#setuverror) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`setAndThrowError`](uv.md#setandthrowerror) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`throwLastError`](uv.md#throwlasterror) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`loop`](uv.md#loop-2) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`reset`](uv.md#reset-3) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`get`](uv.md#get-2) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`tid`](uv.md#tid) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`context`](uv.md#context-3) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`setCloseCleanup`](uv.md#setclosecleanup) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`clearCloseCleanup`](uv.md#clearclosecleanup) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`assertThread`](uv.md#assertthread) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`_loop`](uv.md#_loop) | `variable` | Inherited from [`Handle`](uv.md#handle-2) |
| [`_context`](uv.md#_context) | `variable` | Inherited from [`Handle`](uv.md#handle-2) |
| [`_tid`](uv.md#_tid) | `variable` | Inherited from [`Handle`](uv.md#handle-2) |
| [`_error`](uv.md#_error-1) | `variable` | Inherited from [`Handle`](uv.md#handle-2) |
| [`onError`](uv.md#onerror) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`onClose`](uv.md#onclose) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`Handle`](uv.md#handle-4) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`Handle`](uv.md#handle-5) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`Type`](uv.md#type-7) | `typedef` | Inherited from [`Handle`](uv.md#handle-2) |
| [`opaque`](#opaque) | `variable` | Inherited from [`Socket`](#socket-1) |
| [`Socket`](#socket-2) | `function` | Inherited from [`Socket`](#socket-1) |
| [`Socket`](#socket-3) | `function` | Inherited from [`Socket`](#socket-1) |
| [`Socket`](#socket-4) | `function` | Inherited from [`Socket`](#socket-1) |
| [`connect`](#connect-2) | `function` | Inherited from [`Socket`](#socket-1) |
| [`connect`](#connect-3) | `function` | Inherited from [`Socket`](#socket-1) |
| [`bind`](#bind) | `function` | Inherited from [`Socket`](#socket-1) |
| [`listen`](#listen) | `function` | Inherited from [`Socket`](#socket-1) |
| [`shutdown`](#shutdown-1) | `function` | Inherited from [`Socket`](#socket-1) |
| [`sendOwned`](#sendowned) | `function` | Inherited from [`Socket`](#socket-1) |
| [`sendOwned`](#sendowned-1) | `function` | Inherited from [`Socket`](#socket-1) |
| [`close`](#close-12) | `function` | Inherited from [`Socket`](#socket-1) |
| [`address`](#address-8) | `function` | Inherited from [`Socket`](#socket-1) |
| [`peerAddress`](#peeraddress-1) | `function` | Inherited from [`Socket`](#socket-1) |
| [`transport`](#transport) | `function` | Inherited from [`Socket`](#socket-1) |
| [`setError`](#seterror-1) | `function` | Inherited from [`Socket`](#socket-1) |
| [`error`](#error-5) | `function` | Inherited from [`Socket`](#socket-1) |
| [`closed`](#closed-1) | `function` | Inherited from [`Socket`](#socket-1) |
| [`loop`](#loop-3) | `function` | Inherited from [`Socket`](#socket-1) |
| [`_af`](#_af) | `variable` | Inherited from [`Socket`](#socket-1) |
| [`init`](#init-4) | `function` | Inherited from [`Socket`](#socket-1) |
| [`reset`](#reset-4) | `function` | Inherited from [`Socket`](#socket-1) |
| [`Ptr`](#ptr-4) | `typedef` | Inherited from [`Socket`](#socket-1) |
| [`Vec`](#vec) | `typedef` | Inherited from [`Socket`](#socket-1) |
| [`priority`](#priority-1) | `variable` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`SocketAdapter`](#socketadapter-1) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`~SocketAdapter`](#socketadapter-2) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`send`](#send) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`send`](#send-1) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`sendOwned`](#sendowned-2) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`sendOwned`](#sendowned-3) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`sendPacket`](#sendpacket) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`sendPacket`](#sendpacket-1) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`sendPacket`](#sendpacket-2) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`setSender`](#setsender) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`sender`](#sender) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`addReceiver`](#addreceiver) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`removeReceiver`](#removereceiver) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`hasReceiver`](#hasreceiver) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`receivers`](#receivers) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`onSocketConnect`](#onsocketconnect) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`onSocketRecv`](#onsocketrecv-1) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`onSocketError`](#onsocketerror) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`onSocketClose`](#onsocketclose) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`_sender`](#_sender) | `variable` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`_receivers`](#_receivers) | `variable` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`_dirty`](#_dirty) | `variable` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`cleanupReceivers`](#cleanupreceivers) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |

### Inherited from [`TCPSocket`](#tcpsocket)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`AcceptConnection`](#acceptconnection-1)  | Fired when a new client connection is accepted; carries a shared_ptr to the new socket. |
| `function` | [`TCPSocket`](#tcpsocket-1)  | Constructs the [TCPSocket](#tcpsocket) and initializes the underlying libuv handle. |
| `function` | [`TCPSocket`](#tcpsocket-2)  | Deleted constructor. |
| `function` | [`TCPSocket`](#tcpsocket-3)  | Deleted constructor. |
| `function` | [`shutdown`](#shutdown-5) `virtual` `nodiscard` `override` | Sends a TCP shutdown request; the socket closes after the peer acknowledges. |
| `function` | [`close`](#close-15) `virtual` `override` | Closes the socket immediately, releasing all associated resources. |
| `function` | [`connect`](#connect-7) `virtual` `override` | Connects to `peerAddress` using a libuv connect request. On success, calls [onConnect()](#onconnect-1); on failure, calls [setUVError()](uv.md#setuverror). |
| `function` | [`connect`](#connect-8) `virtual` `override` | Resolves `host` via DNS (or maps "localhost"), then connects. |
| `function` | [`send`](#send-4) `virtual` `nodiscard` `override` | Writes `len` bytes to the connected peer. |
| `function` | [`sendOwned`](#sendowned-6) `virtual` `nodiscard` `override` | Sends an owned payload buffer to the connected peer. |
| `function` | [`send`](#send-5) `virtual` `nodiscard` `override` | Writes `len` bytes; `peerAddress` is ignored for TCP (connected stream). |
| `function` | [`sendOwned`](#sendowned-7) `virtual` `nodiscard` `override` |  |
| `function` | [`bind`](#bind-2) `virtual` `override` | Binds the socket to `address`. Resets and reinitializes the handle if the address family changes. |
| `function` | [`listen`](#listen-2) `virtual` `override` | Starts listening for incoming connections with the given backlog. |
| `function` | [`acceptConnection`](#acceptconnection-2) `virtual` | Accepts the next pending client connection and fires AcceptConnection. |
| `function` | [`setReusePort`](#setreuseport) `nodiscard` | Enables SO_REUSEPORT on Linux kernel >= 3.9 for multi-thread load balancing. Must be called after [bind()](#bind-2). No-op and returns false on unsupported platforms. |
| `function` | [`setNoDelay`](#setnodelay) `nodiscard` | Enables or disables TCP_NODELAY (Nagle's algorithm). |
| `function` | [`setKeepAlive`](#setkeepalive) `nodiscard` | Enables or disables TCP keep-alive probes. |
| `function` | [`setSimultaneousAccepts`](#setsimultaneousaccepts) `nodiscard` | Enables or disables simultaneous accepts on Windows. No-op and returns false on non-Windows platforms. |
| `function` | [`setMode`](#setmode)  | Sets the socket mode (ServerSide or ClientSide). |
| `function` | [`mode`](#mode) `const` | Returns the current socket mode. |
| `function` | [`setError`](#seterror-2) `virtual` `override` | Sets the socket error; ignores the call if an error is already recorded. Setting an error causes the socket to close. |
| `function` | [`error`](#error-7) `virtual` `const` `override` | Returns the current socket error, if any. |
| `function` | [`closed`](#closed-2) `virtual` `const` `override` | Returns true if the native socket handle is closed. |
| `function` | [`address`](#address-9) `virtual` `const` `override` | Returns the IP address and port number of the socket. A wildcard address is returned if the socket is not connected. |
| `function` | [`peerAddress`](#peeraddress-2) `virtual` `const` `override` | Returns the IP address and port number of the peer socket. A wildcard address is returned if the socket is not connected. |
| `function` | [`transport`](#transport-2) `virtual` `const` `override` | Returns the TCP transport protocol. |
| `function` | [`loop`](#loop-4) `virtual` `const` `override` | Returns the event loop associated with this socket. |
| `function` | [`onConnect`](#onconnect-1) `virtual` | Called by the stream layer when the TCP connection is established. |
| `function` | [`onRead`](#onread-1) `virtual` `override` | Called by the stream layer with raw received bytes; wraps them in a [MutableBuffer](base.md#mutablebuffer). |
| `function` | [`onRecv`](#onrecv) `virtual` | Dispatches a received buffer to all socket adapters via onSocketRecv. |
| `function` | [`onError`](#onerror-1) `virtual` `override` | Dispatches the error to adapters and closes the socket. |
| `function` | [`onClose`](#onclose-1) `virtual` `override` | Dispatches the close event to all socket adapters. |
| `variable` | [`_mode`](#_mode-1)  |  |
| `variable` | [`_peerAddress`](#_peeraddress)  | Cached peer address (avoids syscall per recv). |
| `function` | [`init`](#init-5) `virtual` `override` | Initializes the underlying socket context. |
| `function` | [`reset`](#reset-5) `virtual` `override` | Resets the socket context for reuse. |
| `typedef` | [`Ptr`](#ptr-9)  |  |
| `typedef` | [`Vec`](#vec-2)  |  |

### Inherited from [`Stream`](base.md#stream)

| Kind | Name | Description |
|------|------|-------------|
| `typedef` | [`Handle`](base.md#classicy_1_1Stream_1a3cffeb0dacfbc1ced054bd40ba0d198a)  |  |
| `variable` | [`Read`](base.md#classicy_1_1Stream_1a97b040391e1972b8599599a101be2184)  | Emitted when data has been received from the peer. |
| `variable` | [`_buffer`](base.md#classicy_1_1Stream_1a947a340e14aaeaa8164d5d822a327857)  |  |
| `variable` | [`_started`](base.md#classicy_1_1Stream_1a9db4e719d928ab7ee93cbe5672fa8271)  |  |
| `variable` | [`_highWaterMark`](base.md#classicy_1_1Stream_1aa05d9882aaf5b99452c81ccb7b9effdf)  | 16MB default write queue limit |
| `variable` | [`_writeReqFree`](base.md#classicy_1_1Stream_1a37c46db727e34bf48851b3f2c7e40b0b)  | Freelist for write requests. |
| `variable` | [`_ownedWriteReqFree`](base.md#classicy_1_1Stream_1abfe77906ad79a929cf4c9f3dac6d3365)  | Freelist for owned write requests. |
| `function` | [`Stream`](base.md#classicy_1_1Stream_1aa89bd769ee5af9812776575c114ced1a) `inline` | Construct the stream bound to `loop` with a 64 KiB read buffer. |
| `function` | [`~Stream`](base.md#classicy_1_1Stream_1a846a88720d4484531aa275e9158e7e8a) `virtual` `inline` | Destroy the stream, stopping reads and freeing pooled write requests. |
| `function` | [`close`](base.md#classicy_1_1Stream_1a702473d53d40bc5f19b997a84817a341) `virtual` `inline` `override` | Closes and resets the stream handle. This will close the active socket/pipe and destroy the handle. |
| `function` | [`shutdown`](base.md#classicy_1_1Stream_1af4524ac354596db24f2b8de1fde027ba) `inline` | Send a TCP/pipe shutdown request to the connected peer. |
| `function` | [`write`](base.md#classicy_1_1Stream_1a56926a6ac3ba433aed1414ffb1d20cf0) `inline` | Write `len` bytes from `data` to the stream. |
| `function` | [`writeOwned`](base.md#classicy_1_1Stream_1a2cd782ba47e07ad03ea83eb088780fa2) `inline` | Write an owned payload buffer to the stream. |
| `function` | [`setHighWaterMark`](base.md#classicy_1_1Stream_1af8bea25a99110f9ebdd4c61a70750961) `inline` | Set the high water mark for the write queue (default 16MB). When the write queue exceeds this size, [write()](base.md#classicy_1_1Stream_1a56926a6ac3ba433aed1414ffb1d20cf0) returns false. |
| `function` | [`write`](base.md#classicy_1_1Stream_1a01d508b9541f4c4ff983be14887f8ee1) `inline` | Write `len` bytes from `data` together with a stream handle over an IPC pipe (uses `uv_write2`). |
| `function` | [`stream`](base.md#classicy_1_1Stream_1a5d9205b236601db698897ada20795c1a) `inline` | Return the underlying `uv_stream_t` pointer cast from the native handle. |
| `function` | [`readStart`](base.md#classicy_1_1Stream_1a56e0d361385f0b1a3795979c6de1b889) `virtual` `inline` | Begin reading from the stream by registering libuv read callbacks. |
| `function` | [`readStop`](base.md#classicy_1_1Stream_1ae5ae85e023f0e245cbc56d0956baa74e) `virtual` `inline` | Stop reading from the stream. |
| `function` | [`onRead`](base.md#classicy_1_1Stream_1ad60923c284f29eae162e2d977ebebcab) `virtual` `inline` | Called by `[handleRead](base.md#classicy_1_1Stream_1ac74fea672c0d281f2a4f51bee6943b10)` when `len` bytes of `data` arrive. |
| `function` | [`allocWriteReq`](base.md#classicy_1_1Stream_1a12dd6ceb997f9c5ebcdaae1baf203d8a) `inline` | Return a `uv_write_t` from the freelist, or allocate a new one if the pool is empty. |
| `function` | [`freeWriteReq`](base.md#classicy_1_1Stream_1a2d8d36707eb1844afcd08557d2702e36) `inline` | Return `req` to the freelist, or delete it if the pool is at capacity. |
| `function` | [`allocOwnedWriteReq`](base.md#classicy_1_1Stream_1a64c87a4f11e5c9cbf4e9700b8d01bfb5) `inline` |  |
| `function` | [`freeOwnedWriteReq`](base.md#classicy_1_1Stream_1ace2be7e7601ac8c112180e2a7b9dd026) `inline` |  |
| `function` | [`canQueueWrite`](base.md#classicy_1_1Stream_1ae1087b59e3ef730967e0beaa7b3a9561) `inline` |  |
| `function` | [`handleRead`](base.md#classicy_1_1Stream_1ac74fea672c0d281f2a4f51bee6943b10) `static` `inline` | UV callbacks. |
| `function` | [`allocReadBuffer`](base.md#classicy_1_1Stream_1a4b2854f2d85d98ea51f012f3b0b5c099) `static` `inline` | libuv allocate-buffer callback. Provides the stream's internal buffer, growing it if libuv requests more space than the current allocation. |

### Inherited from [`Handle`](uv.md#handle-2)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`Handle`](uv.md#handle-3) `inline` | Construct the handle bound to the given event loop. |
| `function` | [`init`](uv.md#init-2) `inline` | Initialize the underlying libuv handle by calling `f` with the loop, the raw handle pointer, and any additional `args`. |
| `function` | [`invoke`](uv.md#invoke) `inline` | Invoke a libuv function `f` with `args` on the initialized handle. |
| `function` | [`invokeOrThrow`](uv.md#invokeorthrow) `inline` | Invoke a libuv function `f` with `args`, throwing on failure. |
| `function` | [`close`](uv.md#close-11) `virtual` `inline` | Close and destroy the handle. |
| `function` | [`ref`](uv.md#ref) `inline` | Re-reference the handle with the event loop after a previous `[unref()](uv.md#unref)`. |
| `function` | [`unref`](uv.md#unref) `inline` | Unreference the handle from the event loop. |
| `function` | [`initialized`](uv.md#initialized-1) `const` `inline` | Return `true` if the handle has been successfully initialized via `[init()](uv.md#init-2)`. |
| `function` | [`active`](uv.md#active) `virtual` `const` `inline` | Return `true` when the handle is active (libuv `uv_is_active`). |
| `function` | [`closing`](uv.md#closing-1) `virtual` `const` `inline` | Return `true` if `uv_close` has been called and the handle is awaiting its close callback (libuv `uv_is_closing`). |
| `function` | [`closed`](uv.md#closed) `virtual` `const` `inline` | Return `true` if the handle has been fully closed (context released). |
| `function` | [`error`](uv.md#error-4) `const` `inline` | Return the last error set on this handle, or a default-constructed `[Error](base.md#error)` if no error has occurred. |
| `function` | [`setError`](uv.md#seterror) `virtual` `inline` | Set the error state and invoke `[onError()](uv.md#onerror)`. |
| `function` | [`setUVError`](uv.md#setuverror) `inline` | Translate a libuv error code into an `[Error](base.md#error)` and call `[setError()](uv.md#seterror)`. |
| `function` | [`setAndThrowError`](uv.md#setandthrowerror) `inline` | Set the error state from a libuv error code and throw a `std::runtime_error`. |
| `function` | [`throwLastError`](uv.md#throwlasterror) `inline` | Throw a `std::runtime_error` if the handle currently holds an error. |
| `function` | [`loop`](uv.md#loop-2) `const` `inline` | Return the event loop this handle is bound to. |
| `function` | [`reset`](uv.md#reset-3) `inline` | Close the current handle (if open) and allocate a fresh `[Context](uv.md#context-1)`, leaving the handle ready to be re-initialized via `[init()](uv.md#init-2)`. |
| `function` | [`get`](uv.md#get-2) `const` `inline` | Return the raw libuv handle pointer cast to `[Handle](uv.md#handle-2)`. |
| `function` | [`tid`](uv.md#tid) `const` `inline` | Return the ID of the thread that constructed this handle. |
| `function` | [`context`](uv.md#context-3) `const` `inline` | Return the raw `[Context](uv.md#context-1)` that owns the libuv handle memory. |
| `function` | [`setCloseCleanup`](uv.md#setclosecleanup) `inline` |  |
| `function` | [`clearCloseCleanup`](uv.md#clearclosecleanup) `inline` |  |
| `function` | [`assertThread`](uv.md#assertthread) `const` `inline` | Throw `std::logic_error` if called from any thread other than the thread that constructed this handle. |
| `variable` | [`_loop`](uv.md#_loop)  |  |
| `variable` | [`_context`](uv.md#_context)  |  |
| `variable` | [`_tid`](uv.md#_tid)  |  |
| `variable` | [`_error`](uv.md#_error-1)  |  |
| `function` | [`onError`](uv.md#onerror) `virtual` `inline` | Called by `[setError()](uv.md#seterror)` after the error state has been updated. |
| `function` | [`onClose`](uv.md#onclose) `virtual` `inline` | Called by `[close()](uv.md#close-11)` after the context has been released. |
| `function` | [`Handle`](uv.md#handle-4)  | NonCopyable and NonMovable. |
| `function` | [`Handle`](uv.md#handle-5)  | Deleted constructor. |
| `typedef` | [`Type`](uv.md#type-7)  | Define the native handle type. |

### Inherited from [`Socket`](#socket-1)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`opaque`](#opaque)  | Optional client data. |
| `function` | [`Socket`](#socket-2)  | Defaulted constructor. |
| `function` | [`Socket`](#socket-3)  | Deleted constructor. |
| `function` | [`Socket`](#socket-4)  | Deleted constructor. |
| `function` | [`connect`](#connect-2) `virtual` | Connects to the given peer IP address. |
| `function` | [`connect`](#connect-3) `virtual` | Resolves and connects to the given host address. |
| `function` | [`bind`](#bind) `virtual` | Bind a local address to the socket. The address may be IPv4 or IPv6 (if supported). |
| `function` | [`listen`](#listen) `virtual` `inline` | Listens the socket on the given address. |
| `function` | [`shutdown`](#shutdown-1) `virtual` `inline` `nodiscard` | Sends the shutdown packet which should result is socket closure via callback. |
| `function` | [`sendOwned`](#sendowned) `virtual` `nodiscard` | Sends an owned payload buffer to the connected peer. |
| `function` | [`sendOwned`](#sendowned-1) `virtual` `nodiscard` |  |
| `function` | [`close`](#close-12) `virtual` | Closes the underlying socket. |
| `function` | [`address`](#address-8) `virtual` `const` | The locally bound address. |
| `function` | [`peerAddress`](#peeraddress-1) `virtual` `const` | The connected peer address. |
| `function` | [`transport`](#transport) `virtual` `const` | The transport protocol: TCP, UDP or SSLTCP. |
| `function` | [`setError`](#seterror-1) `virtual` | Sets the socket error. |
| `function` | [`error`](#error-5) `virtual` `const` | Return the socket error if any. |
| `function` | [`closed`](#closed-1) `virtual` `const` | Returns true if the native socket handle is closed. |
| `function` | [`loop`](#loop-3) `virtual` `const` | Returns the socket event loop. |
| `variable` | [`_af`](#_af)  |  |
| `function` | [`init`](#init-4) `virtual` | Initializes the underlying socket context. |
| `function` | [`reset`](#reset-4) `virtual` | Resets the socket context for reuse. |
| `typedef` | [`Ptr`](#ptr-4)  |  |
| `typedef` | [`Vec`](#vec)  |  |

### Inherited from [`SocketAdapter`](#socketadapter)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`priority`](#priority-1)  | The priority of this adapter for STL sort operations. |
| `function` | [`SocketAdapter`](#socketadapter-1)  | Creates the [SocketAdapter](#socketadapter). |
| `function` | [`~SocketAdapter`](#socketadapter-2) `virtual` `noexcept` | Destroys the [SocketAdapter](#socketadapter). |
| `function` | [`send`](#send) `virtual` `nodiscard` | Sends the given data buffer to the connected peer. Returns the number of bytes sent or -1 on error. No exception will be thrown. For TCP sockets the given peer address must match the connected peer address. |
| `function` | [`send`](#send-1) `virtual` `nodiscard` |  |
| `function` | [`sendOwned`](#sendowned-2) `virtual` `nodiscard` | Sends an owned payload buffer to the connected peer. |
| `function` | [`sendOwned`](#sendowned-3) `virtual` `nodiscard` |  |
| `function` | [`sendPacket`](#sendpacket) `virtual` | Sends the given packet to the connected peer. Returns the number of bytes sent or -1 on error. No exception will be thrown. For TCP sockets the given peer address must match the connected peer address. |
| `function` | [`sendPacket`](#sendpacket-1) `virtual` |  |
| `function` | [`sendPacket`](#sendpacket-2) `virtual` | Sends the given packet to the connected peer. This method provides delegate compatibility, and unlike other send methods throws an exception if the underlying socket is closed. |
| `function` | [`setSender`](#setsender) `virtual` | Sets the pointer to the outgoing data adapter. Send methods proxy data to this adapter by default. |
| `function` | [`sender`](#sender)  | Returns the output [SocketAdapter](#socketadapter) pointer. |
| `function` | [`addReceiver`](#addreceiver) `virtual` | Sets the pointer to the incoming data adapter. Events proxy data to this adapter by default. |
| `function` | [`removeReceiver`](#removereceiver) `virtual` | Remove the given receiver. |
| `function` | [`hasReceiver`](#hasreceiver) `virtual` | Returns true if the given receiver is connected. |
| `function` | [`receivers`](#receivers)  | Returns all currently registered input [SocketAdapter](#socketadapter) pointers. Dead (removed) entries are excluded from the returned list. |
| `function` | [`onSocketConnect`](#onsocketconnect) `virtual` | Called when the socket establishes a connection. Forwards the event to all registered receivers in priority order. Override to intercept before the application sees the event. |
| `function` | [`onSocketRecv`](#onsocketrecv-1) `virtual` | Called when data is received from the socket. Forwards the event to all registered receivers in priority order. |
| `function` | [`onSocketError`](#onsocketerror) `virtual` | Called when the socket encounters an error. Forwards the event to all registered receivers in priority order. |
| `function` | [`onSocketClose`](#onsocketclose) `virtual` | Called when the socket is closed. Forwards the event to all registered receivers in priority order. |
| `variable` | [`_sender`](#_sender)  |  |
| `variable` | [`_receivers`](#_receivers)  |  |
| `variable` | [`_dirty`](#_dirty)  |  |
| `function` | [`cleanupReceivers`](#cleanupreceivers) `virtual` |  |

### Friends

| Name | Description |
|------|-------------|
| [`net::SSLAdapter`](#net-ssladapter)  |  |

---

{#net-ssladapter}

#### net::SSLAdapter

```cpp
friend class net::SSLAdapter
```

Defined in src/net/include/icy/net/sslsocket.h:169

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`SSLSocket`](#sslsocket-1)  | Constructs an [SSLSocket](#sslsocket) that acquires its context from [SSLManager](#sslmanager) on first use. |
|  | [`SSLSocket`](#sslsocket-2)  | Constructs an [SSLSocket](#sslsocket) with an explicit SSL context. |
|  | [`SSLSocket`](#sslsocket-3)  | Constructs an [SSLSocket](#sslsocket) with an explicit context and a prior session for resumption. |
| `void` | [`connect`](#connect-5) `virtual` `override` | Initialize the [SSLSocket](#sslsocket) with the given [SSLContext](#sslcontext). |
| `void` | [`connect`](#connect-6) `virtual` `override` | Resolves `host` and initiates a secure connection. |
| `void` | [`bind`](#bind-1) `virtual` `override` | Binds the socket to `address` for server-side use. Throws std::logic_error if the context is not a server context. |
| `void` | [`listen`](#listen-1) `virtual` `override` | Starts listening for incoming connections. Throws std::logic_error if the context is not a server context. |
| `bool` | [`shutdown`](#shutdown-4) `virtual` `nodiscard` `override` | Shuts down the connection by attempting an orderly SSL shutdown, then actually shutting down the TCP connection. |
| `void` | [`close`](#close-14) `virtual` `override` | Closes the socket forcefully. |
| `ssize_t` | [`send`](#send-2) `virtual` `nodiscard` `override` | Encrypts and sends `len` bytes to the connected peer. |
| `ssize_t` | [`sendOwned`](#sendowned-4) `virtual` `nodiscard` `override` | Sends an owned payload buffer to the connected peer. |
| `ssize_t` | [`send`](#send-3) `virtual` `nodiscard` `override` | Encrypts and sends `len` bytes, ignoring `peerAddress` (TCP is connected). |
| `ssize_t` | [`sendOwned`](#sendowned-5) `virtual` `nodiscard` `override` |  |
| `void` | [`setHostname`](#sethostname-1)  | Set the expected peer hostname for certificate verification and SNI. Must be called before [connect()](#connect-5) to enable hostname verification. |
| `void` | [`useContext`](#usecontext)  | Use the given SSL context for this socket. |
| `SSLContext::Ptr` | [`context`](#context-4) `const` | Returns the SSL context used for this socket. |
| `void` | [`useSession`](#usesession)  | Sets the SSL session to use for the next connection. Setting a previously saved Session object is necessary to enable session caching. |
| `SSLSession::Ptr` | [`currentSession`](#currentsession)  | Returns the SSL session of the current connection, for reuse in a future connection (if session caching is enabled). |
| `bool` | [`sessionWasReused`](#sessionwasreused)  | Returns true if a reused session was negotiated during the handshake. |
| `int` | [`available`](#available-2) `const` | Returns the number of bytes available from the SSL buffer for immediate reading. |
| `X509 *` | [`peerCertificate`](#peercertificate) `const` | Returns the peer's X.509 certificate, or nullptr if no certificate was presented. |
| `net::TransportType` | [`transport`](#transport-1) `virtual` `const` `override` | Returns the SSLTCP transport protocol identifier. |
| `void` | [`acceptConnection`](#acceptconnection) `virtual` `override` | Accepts a pending client connection, initializes the server-side SSL context on the new socket, and fires the AcceptConnection signal. |
| `void` | [`onConnect`](#onconnect) `virtual` `override` | Called when the TCP connection is established; starts reading and initiates the client-side SSL handshake. |
| `void` | [`onRead`](#onread) `virtual` `override` | Feeds raw encrypted bytes from the network into the SSL adapter. Called by the stream layer when ciphertext arrives from the peer. |

---

{#sslsocket-1}

#### SSLSocket

```cpp
SSLSocket(uv::Loop * loop = uv::defaultLoop())
```

Defined in src/net/include/icy/net/sslsocket.h:38

Constructs an [SSLSocket](#sslsocket) that acquires its context from [SSLManager](#sslmanager) on first use. 
#### Parameters
* `loop` Event loop to use; defaults to the default loop.

---

{#sslsocket-2}

#### SSLSocket

```cpp
SSLSocket(SSLContext::Ptr sslContext, uv::Loop * loop = uv::defaultLoop())
```

Defined in src/net/include/icy/net/sslsocket.h:43

Constructs an [SSLSocket](#sslsocket) with an explicit SSL context. 
#### Parameters
* `sslContext` The SSL context to use for this connection. 

* `loop` Event loop to use; defaults to the default loop.

---

{#sslsocket-3}

#### SSLSocket

```cpp
SSLSocket(SSLContext::Ptr sslContext, SSLSession::Ptr session, uv::Loop * loop = uv::defaultLoop())
```

Defined in src/net/include/icy/net/sslsocket.h:49

Constructs an [SSLSocket](#sslsocket) with an explicit context and a prior session for resumption. 
#### Parameters
* `sslContext` The SSL context to use for this connection. 

* `session` A previously saved session to attempt resumption with. 

* `loop` Event loop to use; defaults to the default loop.

---

{#connect-5}

#### connect

`virtual` `override`

```cpp
virtual void connect(const Address & peerAddress) override
```

Defined in src/net/include/icy/net/sslsocket.h:61

Initialize the [SSLSocket](#sslsocket) with the given [SSLContext](#sslcontext).

Initiates a secure connection to the peer at the given address.

The SSL handshake begins automatically once the TCP connection is established. 
#### Parameters
* `peerAddress` The remote address to connect to.

##### Reimplements

- [`connect`](#connect-7)

---

{#connect-6}

#### connect

`virtual` `override`

```cpp
virtual void connect(std::string_view host, uint16_t port) override
```

Defined in src/net/include/icy/net/sslsocket.h:69

Resolves `host` and initiates a secure connection.

Sets the hostname on the SSL adapter for SNI and certificate verification before resolving and connecting. 
#### Parameters
* `host` Hostname or IP address string. 

* `port` Destination port.

##### Reimplements

- [`connect`](#connect-8)

---

{#bind-1}

#### bind

`virtual` `override`

```cpp
virtual void bind(const net::Address & address, unsigned flags = 0) override
```

Defined in src/net/include/icy/net/sslsocket.h:75

Binds the socket to `address` for server-side use. Throws std::logic_error if the context is not a server context. 
#### Parameters
* `address` Local address to bind to. 

* `flags` Optional bind flags (passed to uv_tcp_bind).

##### Reimplements

- [`bind`](#bind-2)

---

{#listen-1}

#### listen

`virtual` `override`

```cpp
virtual void listen(int backlog = 64) override
```

Defined in src/net/include/icy/net/sslsocket.h:80

Starts listening for incoming connections. Throws std::logic_error if the context is not a server context. 
#### Parameters
* `backlog` Maximum number of pending connections.

##### Reimplements

- [`listen`](#listen-2)

---

{#shutdown-4}

#### shutdown

`virtual` `nodiscard` `override`

```cpp
[[nodiscard]] virtual bool shutdown() override
```

Defined in src/net/include/icy/net/sslsocket.h:85

Shuts down the connection by attempting an orderly SSL shutdown, then actually shutting down the TCP connection.

##### Reimplements

- [`shutdown`](#shutdown-1)

---

{#close-14}

#### close

`virtual` `override`

```cpp
virtual void close() override
```

Defined in src/net/include/icy/net/sslsocket.h:88

Closes the socket forcefully.

##### Reimplements

- [`close`](#close-15)

---

{#send-2}

#### send

`virtual` `nodiscard` `override`

```cpp
[[nodiscard]] virtual ssize_t send(const char * data, size_t len, int flags = 0) override
```

Defined in src/net/include/icy/net/sslsocket.h:95

Encrypts and sends `len` bytes to the connected peer. 
#### Parameters
* `data` Pointer to the plaintext payload. 

* `len` Number of bytes to send. 

* `flags` Reserved; currently unused. 

#### Returns
Number of plaintext bytes accepted, or -1 on error.

##### Reimplements

- [`send`](#send-4)

---

{#sendowned-4}

#### sendOwned

`virtual` `nodiscard` `override`

```cpp
[[nodiscard]] virtual ssize_t sendOwned(Buffer && buffer, int flags = 0) override
```

Defined in src/net/include/icy/net/sslsocket.h:96

Sends an owned payload buffer to the connected peer.

##### Reimplements

- [`sendOwned`](#sendowned-6)

---

{#send-3}

#### send

`virtual` `nodiscard` `override`

```cpp
[[nodiscard]] virtual ssize_t send(const char * data, size_t len, const net::Address & peerAddress, int flags = 0) override
```

Defined in src/net/include/icy/net/sslsocket.h:104

Encrypts and sends `len` bytes, ignoring `peerAddress` (TCP is connected). 
#### Parameters
* `data` Pointer to the plaintext payload. 

* `len` Number of bytes to send. 

* `peerAddress` Ignored for SSL/TCP; present for interface conformance. 

* `flags` Reserved; currently unused. 

#### Returns
Number of plaintext bytes accepted, or -1 on error.

##### Reimplements

- [`send`](#send-5)

---

{#sendowned-5}

#### sendOwned

`virtual` `nodiscard` `override`

```cpp
[[nodiscard]] virtual ssize_t sendOwned(Buffer && buffer, const net::Address & peerAddress, int flags = 0) override
```

Defined in src/net/include/icy/net/sslsocket.h:106

##### Reimplements

- [`sendOwned`](#sendowned-7)

---

{#sethostname-1}

#### setHostname

```cpp
void setHostname(std::string_view hostname)
```

Defined in src/net/include/icy/net/sslsocket.h:111

Set the expected peer hostname for certificate verification and SNI. Must be called before [connect()](#connect-5) to enable hostname verification.

---

{#usecontext}

#### useContext

```cpp
void useContext(SSLContext::Ptr context)
```

Defined in src/net/include/icy/net/sslsocket.h:114

Use the given SSL context for this socket.

---

{#context-4}

#### context

`const`

```cpp
SSLContext::Ptr context() const
```

Defined in src/net/include/icy/net/sslsocket.h:117

Returns the SSL context used for this socket.

---

{#usesession}

#### useSession

```cpp
void useSession(SSLSession::Ptr session)
```

Defined in src/net/include/icy/net/sslsocket.h:127

Sets the SSL session to use for the next connection. Setting a previously saved Session object is necessary to enable session caching.

To remove the currently set session, a nullptr pointer can be given.

Must be called before [connect()](#connect-5) to be effective.

---

{#currentsession}

#### currentSession

```cpp
SSLSession::Ptr currentSession()
```

Defined in src/net/include/icy/net/sslsocket.h:134

Returns the SSL session of the current connection, for reuse in a future connection (if session caching is enabled).

If no connection is established, returns nullptr.

---

{#sessionwasreused}

#### sessionWasReused

```cpp
bool sessionWasReused()
```

Defined in src/net/include/icy/net/sslsocket.h:138

Returns true if a reused session was negotiated during the handshake.

---

{#available-2}

#### available

`const`

```cpp
int available() const
```

Defined in src/net/include/icy/net/sslsocket.h:142

Returns the number of bytes available from the SSL buffer for immediate reading.

---

{#peercertificate}

#### peerCertificate

`const`

```cpp
X509 * peerCertificate() const
```

Defined in src/net/include/icy/net/sslsocket.h:145

Returns the peer's X.509 certificate, or nullptr if no certificate was presented.

---

{#transport-1}

#### transport

`virtual` `const` `override`

```cpp
virtual net::TransportType transport() const override
```

Defined in src/net/include/icy/net/sslsocket.h:148

Returns the SSLTCP transport protocol identifier.

##### Reimplements

- [`transport`](#transport)

---

{#acceptconnection}

#### acceptConnection

`virtual` `override`

```cpp
virtual void acceptConnection() override
```

Defined in src/net/include/icy/net/sslsocket.h:152

Accepts a pending client connection, initializes the server-side SSL context on the new socket, and fires the AcceptConnection signal.

##### Reimplements

- [`acceptConnection`](#acceptconnection-2)

---

{#onconnect}

#### onConnect

`virtual` `override`

```cpp
virtual void onConnect() override
```

Defined in src/net/include/icy/net/sslsocket.h:156

Called when the TCP connection is established; starts reading and initiates the client-side SSL handshake.

##### Reimplements

- [`onConnect`](#onconnect-1)

---

{#onread}

#### onRead

`virtual` `override`

```cpp
virtual void onRead(const char * data, size_t len) override
```

Defined in src/net/include/icy/net/sslsocket.h:162

Feeds raw encrypted bytes from the network into the SSL adapter. Called by the stream layer when ciphertext arrives from the peer. 
#### Parameters
* `data` Pointer to the encrypted bytes. 

* `len` Number of bytes received.

##### Reimplements

- [`onRead`](#onread-1)

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `net::SSLContext::Ptr` | [`_sslContext`](#_sslcontext-1)  |  |
| `net::SSLSession::Ptr` | [`_sslSession`](#_sslsession)  |  |
| `net::SSLAdapter` | [`_sslAdapter`](#_ssladapter)  |  |

---

{#_sslcontext-1}

#### _sslContext

```cpp
net::SSLContext::Ptr _sslContext
```

Defined in src/net/include/icy/net/sslsocket.h:165

---

{#_sslsession}

#### _sslSession

```cpp
net::SSLSession::Ptr _sslSession
```

Defined in src/net/include/icy/net/sslsocket.h:166

---

{#_ssladapter}

#### _sslAdapter

```cpp
net::SSLAdapter _sslAdapter
```

Defined in src/net/include/icy/net/sslsocket.h:167

### Public Types

| Name | Description |
|------|-------------|
| [`Ptr`](#ptr-8)  |  |
| [`Vec`](#vec-1)  |  |

---

{#ptr-8}

#### Ptr

```cpp
using Ptr = std::shared_ptr< SSLSocket >
```

Defined in src/net/include/icy/net/sslsocket.h:33

---

{#vec-1}

#### Vec

```cpp
using Vec = std::vector< Ptr >
```

Defined in src/net/include/icy/net/sslsocket.h:34

{#tcpsocket}

## TCPSocket

```cpp
#include <icy/net/tcpsocket.h>
```

```cpp
class TCPSocket
```

Defined in src/net/include/icy/net/tcpsocket.h:30

> **Inherits:** [`Stream< uv_tcp_t >`](base.md#stream), [`Socket`](#socket-1)
> **Subclassed by:** [`SSLSocket`](#sslsocket)

TCP socket implementation.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`AcceptConnection`](#acceptconnection-1) | `variable` | Declared here |
| [`TCPSocket`](#tcpsocket-1) | `function` | Declared here |
| [`TCPSocket`](#tcpsocket-2) | `function` | Declared here |
| [`TCPSocket`](#tcpsocket-3) | `function` | Declared here |
| [`shutdown`](#shutdown-5) | `function` | Declared here |
| [`close`](#close-15) | `function` | Declared here |
| [`connect`](#connect-7) | `function` | Declared here |
| [`connect`](#connect-8) | `function` | Declared here |
| [`send`](#send-4) | `function` | Declared here |
| [`sendOwned`](#sendowned-6) | `function` | Declared here |
| [`send`](#send-5) | `function` | Declared here |
| [`sendOwned`](#sendowned-7) | `function` | Declared here |
| [`bind`](#bind-2) | `function` | Declared here |
| [`listen`](#listen-2) | `function` | Declared here |
| [`acceptConnection`](#acceptconnection-2) | `function` | Declared here |
| [`setReusePort`](#setreuseport) | `function` | Declared here |
| [`setNoDelay`](#setnodelay) | `function` | Declared here |
| [`setKeepAlive`](#setkeepalive) | `function` | Declared here |
| [`setSimultaneousAccepts`](#setsimultaneousaccepts) | `function` | Declared here |
| [`setMode`](#setmode) | `function` | Declared here |
| [`mode`](#mode) | `function` | Declared here |
| [`setError`](#seterror-2) | `function` | Declared here |
| [`error`](#error-7) | `function` | Declared here |
| [`closed`](#closed-2) | `function` | Declared here |
| [`address`](#address-9) | `function` | Declared here |
| [`peerAddress`](#peeraddress-2) | `function` | Declared here |
| [`transport`](#transport-2) | `function` | Declared here |
| [`loop`](#loop-4) | `function` | Declared here |
| [`onConnect`](#onconnect-1) | `function` | Declared here |
| [`onRead`](#onread-1) | `function` | Declared here |
| [`onRecv`](#onrecv) | `function` | Declared here |
| [`onError`](#onerror-1) | `function` | Declared here |
| [`onClose`](#onclose-1) | `function` | Declared here |
| [`_mode`](#_mode-1) | `variable` | Declared here |
| [`_peerAddress`](#_peeraddress) | `variable` | Declared here |
| [`init`](#init-5) | `function` | Declared here |
| [`reset`](#reset-5) | `function` | Declared here |
| [`Ptr`](#ptr-9) | `typedef` | Declared here |
| [`Vec`](#vec-2) | `typedef` | Declared here |
| [`Handle`](base.md#classicy_1_1Stream_1a3cffeb0dacfbc1ced054bd40ba0d198a) | `typedef` | Inherited from [`Stream`](base.md#stream) |
| [`Read`](base.md#classicy_1_1Stream_1a97b040391e1972b8599599a101be2184) | `variable` | Inherited from [`Stream`](base.md#stream) |
| [`_buffer`](base.md#classicy_1_1Stream_1a947a340e14aaeaa8164d5d822a327857) | `variable` | Inherited from [`Stream`](base.md#stream) |
| [`_started`](base.md#classicy_1_1Stream_1a9db4e719d928ab7ee93cbe5672fa8271) | `variable` | Inherited from [`Stream`](base.md#stream) |
| [`_highWaterMark`](base.md#classicy_1_1Stream_1aa05d9882aaf5b99452c81ccb7b9effdf) | `variable` | Inherited from [`Stream`](base.md#stream) |
| [`_writeReqFree`](base.md#classicy_1_1Stream_1a37c46db727e34bf48851b3f2c7e40b0b) | `variable` | Inherited from [`Stream`](base.md#stream) |
| [`_ownedWriteReqFree`](base.md#classicy_1_1Stream_1abfe77906ad79a929cf4c9f3dac6d3365) | `variable` | Inherited from [`Stream`](base.md#stream) |
| [`Stream`](base.md#classicy_1_1Stream_1aa89bd769ee5af9812776575c114ced1a) | `function` | Inherited from [`Stream`](base.md#stream) |
| [`~Stream`](base.md#classicy_1_1Stream_1a846a88720d4484531aa275e9158e7e8a) | `function` | Inherited from [`Stream`](base.md#stream) |
| [`close`](base.md#classicy_1_1Stream_1a702473d53d40bc5f19b997a84817a341) | `function` | Inherited from [`Stream`](base.md#stream) |
| [`shutdown`](base.md#classicy_1_1Stream_1af4524ac354596db24f2b8de1fde027ba) | `function` | Inherited from [`Stream`](base.md#stream) |
| [`write`](base.md#classicy_1_1Stream_1a56926a6ac3ba433aed1414ffb1d20cf0) | `function` | Inherited from [`Stream`](base.md#stream) |
| [`writeOwned`](base.md#classicy_1_1Stream_1a2cd782ba47e07ad03ea83eb088780fa2) | `function` | Inherited from [`Stream`](base.md#stream) |
| [`setHighWaterMark`](base.md#classicy_1_1Stream_1af8bea25a99110f9ebdd4c61a70750961) | `function` | Inherited from [`Stream`](base.md#stream) |
| [`write`](base.md#classicy_1_1Stream_1a01d508b9541f4c4ff983be14887f8ee1) | `function` | Inherited from [`Stream`](base.md#stream) |
| [`stream`](base.md#classicy_1_1Stream_1a5d9205b236601db698897ada20795c1a) | `function` | Inherited from [`Stream`](base.md#stream) |
| [`readStart`](base.md#classicy_1_1Stream_1a56e0d361385f0b1a3795979c6de1b889) | `function` | Inherited from [`Stream`](base.md#stream) |
| [`readStop`](base.md#classicy_1_1Stream_1ae5ae85e023f0e245cbc56d0956baa74e) | `function` | Inherited from [`Stream`](base.md#stream) |
| [`onRead`](base.md#classicy_1_1Stream_1ad60923c284f29eae162e2d977ebebcab) | `function` | Inherited from [`Stream`](base.md#stream) |
| [`allocWriteReq`](base.md#classicy_1_1Stream_1a12dd6ceb997f9c5ebcdaae1baf203d8a) | `function` | Inherited from [`Stream`](base.md#stream) |
| [`freeWriteReq`](base.md#classicy_1_1Stream_1a2d8d36707eb1844afcd08557d2702e36) | `function` | Inherited from [`Stream`](base.md#stream) |
| [`allocOwnedWriteReq`](base.md#classicy_1_1Stream_1a64c87a4f11e5c9cbf4e9700b8d01bfb5) | `function` | Inherited from [`Stream`](base.md#stream) |
| [`freeOwnedWriteReq`](base.md#classicy_1_1Stream_1ace2be7e7601ac8c112180e2a7b9dd026) | `function` | Inherited from [`Stream`](base.md#stream) |
| [`canQueueWrite`](base.md#classicy_1_1Stream_1ae1087b59e3ef730967e0beaa7b3a9561) | `function` | Inherited from [`Stream`](base.md#stream) |
| [`handleRead`](base.md#classicy_1_1Stream_1ac74fea672c0d281f2a4f51bee6943b10) | `function` | Inherited from [`Stream`](base.md#stream) |
| [`allocReadBuffer`](base.md#classicy_1_1Stream_1a4b2854f2d85d98ea51f012f3b0b5c099) | `function` | Inherited from [`Stream`](base.md#stream) |
| [`Handle`](uv.md#handle-3) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`init`](uv.md#init-2) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`invoke`](uv.md#invoke) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`invokeOrThrow`](uv.md#invokeorthrow) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`close`](uv.md#close-11) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`ref`](uv.md#ref) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`unref`](uv.md#unref) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`initialized`](uv.md#initialized-1) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`active`](uv.md#active) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`closing`](uv.md#closing-1) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`closed`](uv.md#closed) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`error`](uv.md#error-4) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`setError`](uv.md#seterror) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`setUVError`](uv.md#setuverror) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`setAndThrowError`](uv.md#setandthrowerror) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`throwLastError`](uv.md#throwlasterror) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`loop`](uv.md#loop-2) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`reset`](uv.md#reset-3) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`get`](uv.md#get-2) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`tid`](uv.md#tid) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`context`](uv.md#context-3) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`setCloseCleanup`](uv.md#setclosecleanup) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`clearCloseCleanup`](uv.md#clearclosecleanup) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`assertThread`](uv.md#assertthread) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`_loop`](uv.md#_loop) | `variable` | Inherited from [`Handle`](uv.md#handle-2) |
| [`_context`](uv.md#_context) | `variable` | Inherited from [`Handle`](uv.md#handle-2) |
| [`_tid`](uv.md#_tid) | `variable` | Inherited from [`Handle`](uv.md#handle-2) |
| [`_error`](uv.md#_error-1) | `variable` | Inherited from [`Handle`](uv.md#handle-2) |
| [`onError`](uv.md#onerror) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`onClose`](uv.md#onclose) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`Handle`](uv.md#handle-4) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`Handle`](uv.md#handle-5) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`Type`](uv.md#type-7) | `typedef` | Inherited from [`Handle`](uv.md#handle-2) |
| [`opaque`](#opaque) | `variable` | Inherited from [`Socket`](#socket-1) |
| [`Socket`](#socket-2) | `function` | Inherited from [`Socket`](#socket-1) |
| [`Socket`](#socket-3) | `function` | Inherited from [`Socket`](#socket-1) |
| [`Socket`](#socket-4) | `function` | Inherited from [`Socket`](#socket-1) |
| [`connect`](#connect-2) | `function` | Inherited from [`Socket`](#socket-1) |
| [`connect`](#connect-3) | `function` | Inherited from [`Socket`](#socket-1) |
| [`bind`](#bind) | `function` | Inherited from [`Socket`](#socket-1) |
| [`listen`](#listen) | `function` | Inherited from [`Socket`](#socket-1) |
| [`shutdown`](#shutdown-1) | `function` | Inherited from [`Socket`](#socket-1) |
| [`sendOwned`](#sendowned) | `function` | Inherited from [`Socket`](#socket-1) |
| [`sendOwned`](#sendowned-1) | `function` | Inherited from [`Socket`](#socket-1) |
| [`close`](#close-12) | `function` | Inherited from [`Socket`](#socket-1) |
| [`address`](#address-8) | `function` | Inherited from [`Socket`](#socket-1) |
| [`peerAddress`](#peeraddress-1) | `function` | Inherited from [`Socket`](#socket-1) |
| [`transport`](#transport) | `function` | Inherited from [`Socket`](#socket-1) |
| [`setError`](#seterror-1) | `function` | Inherited from [`Socket`](#socket-1) |
| [`error`](#error-5) | `function` | Inherited from [`Socket`](#socket-1) |
| [`closed`](#closed-1) | `function` | Inherited from [`Socket`](#socket-1) |
| [`loop`](#loop-3) | `function` | Inherited from [`Socket`](#socket-1) |
| [`_af`](#_af) | `variable` | Inherited from [`Socket`](#socket-1) |
| [`init`](#init-4) | `function` | Inherited from [`Socket`](#socket-1) |
| [`reset`](#reset-4) | `function` | Inherited from [`Socket`](#socket-1) |
| [`Ptr`](#ptr-4) | `typedef` | Inherited from [`Socket`](#socket-1) |
| [`Vec`](#vec) | `typedef` | Inherited from [`Socket`](#socket-1) |
| [`priority`](#priority-1) | `variable` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`SocketAdapter`](#socketadapter-1) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`~SocketAdapter`](#socketadapter-2) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`send`](#send) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`send`](#send-1) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`sendOwned`](#sendowned-2) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`sendOwned`](#sendowned-3) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`sendPacket`](#sendpacket) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`sendPacket`](#sendpacket-1) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`sendPacket`](#sendpacket-2) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`setSender`](#setsender) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`sender`](#sender) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`addReceiver`](#addreceiver) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`removeReceiver`](#removereceiver) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`hasReceiver`](#hasreceiver) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`receivers`](#receivers) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`onSocketConnect`](#onsocketconnect) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`onSocketRecv`](#onsocketrecv-1) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`onSocketError`](#onsocketerror) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`onSocketClose`](#onsocketclose) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`_sender`](#_sender) | `variable` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`_receivers`](#_receivers) | `variable` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`_dirty`](#_dirty) | `variable` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`cleanupReceivers`](#cleanupreceivers) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |

### Inherited from [`Stream`](base.md#stream)

| Kind | Name | Description |
|------|------|-------------|
| `typedef` | [`Handle`](base.md#classicy_1_1Stream_1a3cffeb0dacfbc1ced054bd40ba0d198a)  |  |
| `variable` | [`Read`](base.md#classicy_1_1Stream_1a97b040391e1972b8599599a101be2184)  | Emitted when data has been received from the peer. |
| `variable` | [`_buffer`](base.md#classicy_1_1Stream_1a947a340e14aaeaa8164d5d822a327857)  |  |
| `variable` | [`_started`](base.md#classicy_1_1Stream_1a9db4e719d928ab7ee93cbe5672fa8271)  |  |
| `variable` | [`_highWaterMark`](base.md#classicy_1_1Stream_1aa05d9882aaf5b99452c81ccb7b9effdf)  | 16MB default write queue limit |
| `variable` | [`_writeReqFree`](base.md#classicy_1_1Stream_1a37c46db727e34bf48851b3f2c7e40b0b)  | Freelist for write requests. |
| `variable` | [`_ownedWriteReqFree`](base.md#classicy_1_1Stream_1abfe77906ad79a929cf4c9f3dac6d3365)  | Freelist for owned write requests. |
| `function` | [`Stream`](base.md#classicy_1_1Stream_1aa89bd769ee5af9812776575c114ced1a) `inline` | Construct the stream bound to `loop` with a 64 KiB read buffer. |
| `function` | [`~Stream`](base.md#classicy_1_1Stream_1a846a88720d4484531aa275e9158e7e8a) `virtual` `inline` | Destroy the stream, stopping reads and freeing pooled write requests. |
| `function` | [`close`](base.md#classicy_1_1Stream_1a702473d53d40bc5f19b997a84817a341) `virtual` `inline` `override` | Closes and resets the stream handle. This will close the active socket/pipe and destroy the handle. |
| `function` | [`shutdown`](base.md#classicy_1_1Stream_1af4524ac354596db24f2b8de1fde027ba) `inline` | Send a TCP/pipe shutdown request to the connected peer. |
| `function` | [`write`](base.md#classicy_1_1Stream_1a56926a6ac3ba433aed1414ffb1d20cf0) `inline` | Write `len` bytes from `data` to the stream. |
| `function` | [`writeOwned`](base.md#classicy_1_1Stream_1a2cd782ba47e07ad03ea83eb088780fa2) `inline` | Write an owned payload buffer to the stream. |
| `function` | [`setHighWaterMark`](base.md#classicy_1_1Stream_1af8bea25a99110f9ebdd4c61a70750961) `inline` | Set the high water mark for the write queue (default 16MB). When the write queue exceeds this size, [write()](base.md#classicy_1_1Stream_1a56926a6ac3ba433aed1414ffb1d20cf0) returns false. |
| `function` | [`write`](base.md#classicy_1_1Stream_1a01d508b9541f4c4ff983be14887f8ee1) `inline` | Write `len` bytes from `data` together with a stream handle over an IPC pipe (uses `uv_write2`). |
| `function` | [`stream`](base.md#classicy_1_1Stream_1a5d9205b236601db698897ada20795c1a) `inline` | Return the underlying `uv_stream_t` pointer cast from the native handle. |
| `function` | [`readStart`](base.md#classicy_1_1Stream_1a56e0d361385f0b1a3795979c6de1b889) `virtual` `inline` | Begin reading from the stream by registering libuv read callbacks. |
| `function` | [`readStop`](base.md#classicy_1_1Stream_1ae5ae85e023f0e245cbc56d0956baa74e) `virtual` `inline` | Stop reading from the stream. |
| `function` | [`onRead`](base.md#classicy_1_1Stream_1ad60923c284f29eae162e2d977ebebcab) `virtual` `inline` | Called by `[handleRead](base.md#classicy_1_1Stream_1ac74fea672c0d281f2a4f51bee6943b10)` when `len` bytes of `data` arrive. |
| `function` | [`allocWriteReq`](base.md#classicy_1_1Stream_1a12dd6ceb997f9c5ebcdaae1baf203d8a) `inline` | Return a `uv_write_t` from the freelist, or allocate a new one if the pool is empty. |
| `function` | [`freeWriteReq`](base.md#classicy_1_1Stream_1a2d8d36707eb1844afcd08557d2702e36) `inline` | Return `req` to the freelist, or delete it if the pool is at capacity. |
| `function` | [`allocOwnedWriteReq`](base.md#classicy_1_1Stream_1a64c87a4f11e5c9cbf4e9700b8d01bfb5) `inline` |  |
| `function` | [`freeOwnedWriteReq`](base.md#classicy_1_1Stream_1ace2be7e7601ac8c112180e2a7b9dd026) `inline` |  |
| `function` | [`canQueueWrite`](base.md#classicy_1_1Stream_1ae1087b59e3ef730967e0beaa7b3a9561) `inline` |  |
| `function` | [`handleRead`](base.md#classicy_1_1Stream_1ac74fea672c0d281f2a4f51bee6943b10) `static` `inline` | UV callbacks. |
| `function` | [`allocReadBuffer`](base.md#classicy_1_1Stream_1a4b2854f2d85d98ea51f012f3b0b5c099) `static` `inline` | libuv allocate-buffer callback. Provides the stream's internal buffer, growing it if libuv requests more space than the current allocation. |

### Inherited from [`Handle`](uv.md#handle-2)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`Handle`](uv.md#handle-3) `inline` | Construct the handle bound to the given event loop. |
| `function` | [`init`](uv.md#init-2) `inline` | Initialize the underlying libuv handle by calling `f` with the loop, the raw handle pointer, and any additional `args`. |
| `function` | [`invoke`](uv.md#invoke) `inline` | Invoke a libuv function `f` with `args` on the initialized handle. |
| `function` | [`invokeOrThrow`](uv.md#invokeorthrow) `inline` | Invoke a libuv function `f` with `args`, throwing on failure. |
| `function` | [`close`](uv.md#close-11) `virtual` `inline` | Close and destroy the handle. |
| `function` | [`ref`](uv.md#ref) `inline` | Re-reference the handle with the event loop after a previous `[unref()](uv.md#unref)`. |
| `function` | [`unref`](uv.md#unref) `inline` | Unreference the handle from the event loop. |
| `function` | [`initialized`](uv.md#initialized-1) `const` `inline` | Return `true` if the handle has been successfully initialized via `[init()](uv.md#init-2)`. |
| `function` | [`active`](uv.md#active) `virtual` `const` `inline` | Return `true` when the handle is active (libuv `uv_is_active`). |
| `function` | [`closing`](uv.md#closing-1) `virtual` `const` `inline` | Return `true` if `uv_close` has been called and the handle is awaiting its close callback (libuv `uv_is_closing`). |
| `function` | [`closed`](uv.md#closed) `virtual` `const` `inline` | Return `true` if the handle has been fully closed (context released). |
| `function` | [`error`](uv.md#error-4) `const` `inline` | Return the last error set on this handle, or a default-constructed `[Error](base.md#error)` if no error has occurred. |
| `function` | [`setError`](uv.md#seterror) `virtual` `inline` | Set the error state and invoke `[onError()](uv.md#onerror)`. |
| `function` | [`setUVError`](uv.md#setuverror) `inline` | Translate a libuv error code into an `[Error](base.md#error)` and call `[setError()](uv.md#seterror)`. |
| `function` | [`setAndThrowError`](uv.md#setandthrowerror) `inline` | Set the error state from a libuv error code and throw a `std::runtime_error`. |
| `function` | [`throwLastError`](uv.md#throwlasterror) `inline` | Throw a `std::runtime_error` if the handle currently holds an error. |
| `function` | [`loop`](uv.md#loop-2) `const` `inline` | Return the event loop this handle is bound to. |
| `function` | [`reset`](uv.md#reset-3) `inline` | Close the current handle (if open) and allocate a fresh `[Context](uv.md#context-1)`, leaving the handle ready to be re-initialized via `[init()](uv.md#init-2)`. |
| `function` | [`get`](uv.md#get-2) `const` `inline` | Return the raw libuv handle pointer cast to `[Handle](uv.md#handle-2)`. |
| `function` | [`tid`](uv.md#tid) `const` `inline` | Return the ID of the thread that constructed this handle. |
| `function` | [`context`](uv.md#context-3) `const` `inline` | Return the raw `[Context](uv.md#context-1)` that owns the libuv handle memory. |
| `function` | [`setCloseCleanup`](uv.md#setclosecleanup) `inline` |  |
| `function` | [`clearCloseCleanup`](uv.md#clearclosecleanup) `inline` |  |
| `function` | [`assertThread`](uv.md#assertthread) `const` `inline` | Throw `std::logic_error` if called from any thread other than the thread that constructed this handle. |
| `variable` | [`_loop`](uv.md#_loop)  |  |
| `variable` | [`_context`](uv.md#_context)  |  |
| `variable` | [`_tid`](uv.md#_tid)  |  |
| `variable` | [`_error`](uv.md#_error-1)  |  |
| `function` | [`onError`](uv.md#onerror) `virtual` `inline` | Called by `[setError()](uv.md#seterror)` after the error state has been updated. |
| `function` | [`onClose`](uv.md#onclose) `virtual` `inline` | Called by `[close()](uv.md#close-11)` after the context has been released. |
| `function` | [`Handle`](uv.md#handle-4)  | NonCopyable and NonMovable. |
| `function` | [`Handle`](uv.md#handle-5)  | Deleted constructor. |
| `typedef` | [`Type`](uv.md#type-7)  | Define the native handle type. |

### Inherited from [`Socket`](#socket-1)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`opaque`](#opaque)  | Optional client data. |
| `function` | [`Socket`](#socket-2)  | Defaulted constructor. |
| `function` | [`Socket`](#socket-3)  | Deleted constructor. |
| `function` | [`Socket`](#socket-4)  | Deleted constructor. |
| `function` | [`connect`](#connect-2) `virtual` | Connects to the given peer IP address. |
| `function` | [`connect`](#connect-3) `virtual` | Resolves and connects to the given host address. |
| `function` | [`bind`](#bind) `virtual` | Bind a local address to the socket. The address may be IPv4 or IPv6 (if supported). |
| `function` | [`listen`](#listen) `virtual` `inline` | Listens the socket on the given address. |
| `function` | [`shutdown`](#shutdown-1) `virtual` `inline` `nodiscard` | Sends the shutdown packet which should result is socket closure via callback. |
| `function` | [`sendOwned`](#sendowned) `virtual` `nodiscard` | Sends an owned payload buffer to the connected peer. |
| `function` | [`sendOwned`](#sendowned-1) `virtual` `nodiscard` |  |
| `function` | [`close`](#close-12) `virtual` | Closes the underlying socket. |
| `function` | [`address`](#address-8) `virtual` `const` | The locally bound address. |
| `function` | [`peerAddress`](#peeraddress-1) `virtual` `const` | The connected peer address. |
| `function` | [`transport`](#transport) `virtual` `const` | The transport protocol: TCP, UDP or SSLTCP. |
| `function` | [`setError`](#seterror-1) `virtual` | Sets the socket error. |
| `function` | [`error`](#error-5) `virtual` `const` | Return the socket error if any. |
| `function` | [`closed`](#closed-1) `virtual` `const` | Returns true if the native socket handle is closed. |
| `function` | [`loop`](#loop-3) `virtual` `const` | Returns the socket event loop. |
| `variable` | [`_af`](#_af)  |  |
| `function` | [`init`](#init-4) `virtual` | Initializes the underlying socket context. |
| `function` | [`reset`](#reset-4) `virtual` | Resets the socket context for reuse. |
| `typedef` | [`Ptr`](#ptr-4)  |  |
| `typedef` | [`Vec`](#vec)  |  |

### Inherited from [`SocketAdapter`](#socketadapter)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`priority`](#priority-1)  | The priority of this adapter for STL sort operations. |
| `function` | [`SocketAdapter`](#socketadapter-1)  | Creates the [SocketAdapter](#socketadapter). |
| `function` | [`~SocketAdapter`](#socketadapter-2) `virtual` `noexcept` | Destroys the [SocketAdapter](#socketadapter). |
| `function` | [`send`](#send) `virtual` `nodiscard` | Sends the given data buffer to the connected peer. Returns the number of bytes sent or -1 on error. No exception will be thrown. For TCP sockets the given peer address must match the connected peer address. |
| `function` | [`send`](#send-1) `virtual` `nodiscard` |  |
| `function` | [`sendOwned`](#sendowned-2) `virtual` `nodiscard` | Sends an owned payload buffer to the connected peer. |
| `function` | [`sendOwned`](#sendowned-3) `virtual` `nodiscard` |  |
| `function` | [`sendPacket`](#sendpacket) `virtual` | Sends the given packet to the connected peer. Returns the number of bytes sent or -1 on error. No exception will be thrown. For TCP sockets the given peer address must match the connected peer address. |
| `function` | [`sendPacket`](#sendpacket-1) `virtual` |  |
| `function` | [`sendPacket`](#sendpacket-2) `virtual` | Sends the given packet to the connected peer. This method provides delegate compatibility, and unlike other send methods throws an exception if the underlying socket is closed. |
| `function` | [`setSender`](#setsender) `virtual` | Sets the pointer to the outgoing data adapter. Send methods proxy data to this adapter by default. |
| `function` | [`sender`](#sender)  | Returns the output [SocketAdapter](#socketadapter) pointer. |
| `function` | [`addReceiver`](#addreceiver) `virtual` | Sets the pointer to the incoming data adapter. Events proxy data to this adapter by default. |
| `function` | [`removeReceiver`](#removereceiver) `virtual` | Remove the given receiver. |
| `function` | [`hasReceiver`](#hasreceiver) `virtual` | Returns true if the given receiver is connected. |
| `function` | [`receivers`](#receivers)  | Returns all currently registered input [SocketAdapter](#socketadapter) pointers. Dead (removed) entries are excluded from the returned list. |
| `function` | [`onSocketConnect`](#onsocketconnect) `virtual` | Called when the socket establishes a connection. Forwards the event to all registered receivers in priority order. Override to intercept before the application sees the event. |
| `function` | [`onSocketRecv`](#onsocketrecv-1) `virtual` | Called when data is received from the socket. Forwards the event to all registered receivers in priority order. |
| `function` | [`onSocketError`](#onsocketerror) `virtual` | Called when the socket encounters an error. Forwards the event to all registered receivers in priority order. |
| `function` | [`onSocketClose`](#onsocketclose) `virtual` | Called when the socket is closed. Forwards the event to all registered receivers in priority order. |
| `variable` | [`_sender`](#_sender)  |  |
| `variable` | [`_receivers`](#_receivers)  |  |
| `variable` | [`_dirty`](#_dirty)  |  |
| `function` | [`cleanupReceivers`](#cleanupreceivers) `virtual` |  |

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `LocalSignal< void(const net::TCPSocket::Ptr &)>` | [`AcceptConnection`](#acceptconnection-1)  | Fired when a new client connection is accepted; carries a shared_ptr to the new socket. |

---

{#acceptconnection-1}

#### AcceptConnection

```cpp
LocalSignal< void(const net::TCPSocket::Ptr &)> AcceptConnection
```

Defined in src/net/include/icy/net/tcpsocket.h:149

Fired when a new client connection is accepted; carries a shared_ptr to the new socket.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`TCPSocket`](#tcpsocket-1)  | Constructs the [TCPSocket](#tcpsocket) and initializes the underlying libuv handle. |
|  | [`TCPSocket`](#tcpsocket-2)  | Deleted constructor. |
|  | [`TCPSocket`](#tcpsocket-3)  | Deleted constructor. |
| `bool` | [`shutdown`](#shutdown-5) `virtual` `nodiscard` `override` | Sends a TCP shutdown request; the socket closes after the peer acknowledges. |
| `void` | [`close`](#close-15) `virtual` `override` | Closes the socket immediately, releasing all associated resources. |
| `void` | [`connect`](#connect-7) `virtual` `override` | Connects to `peerAddress` using a libuv connect request. On success, calls [onConnect()](#onconnect-1); on failure, calls [setUVError()](uv.md#setuverror). |
| `void` | [`connect`](#connect-8) `virtual` `override` | Resolves `host` via DNS (or maps "localhost"), then connects. |
| `ssize_t` | [`send`](#send-4) `virtual` `nodiscard` `override` | Writes `len` bytes to the connected peer. |
| `ssize_t` | [`sendOwned`](#sendowned-6) `virtual` `nodiscard` `override` | Sends an owned payload buffer to the connected peer. |
| `ssize_t` | [`send`](#send-5) `virtual` `nodiscard` `override` | Writes `len` bytes; `peerAddress` is ignored for TCP (connected stream). |
| `ssize_t` | [`sendOwned`](#sendowned-7) `virtual` `nodiscard` `override` |  |
| `void` | [`bind`](#bind-2) `virtual` `override` | Binds the socket to `address`. Resets and reinitializes the handle if the address family changes. |
| `void` | [`listen`](#listen-2) `virtual` `override` | Starts listening for incoming connections with the given backlog. |
| `void` | [`acceptConnection`](#acceptconnection-2) `virtual` | Accepts the next pending client connection and fires AcceptConnection. |
| `bool` | [`setReusePort`](#setreuseport) `nodiscard` | Enables SO_REUSEPORT on Linux kernel >= 3.9 for multi-thread load balancing. Must be called after [bind()](#bind-2). No-op and returns false on unsupported platforms. |
| `bool` | [`setNoDelay`](#setnodelay) `nodiscard` | Enables or disables TCP_NODELAY (Nagle's algorithm). |
| `bool` | [`setKeepAlive`](#setkeepalive) `nodiscard` | Enables or disables TCP keep-alive probes. |
| `bool` | [`setSimultaneousAccepts`](#setsimultaneousaccepts) `nodiscard` | Enables or disables simultaneous accepts on Windows. No-op and returns false on non-Windows platforms. |
| `void` | [`setMode`](#setmode)  | Sets the socket mode (ServerSide or ClientSide). |
| `SocketMode` | [`mode`](#mode) `const` | Returns the current socket mode. |
| `void` | [`setError`](#seterror-2) `virtual` `override` | Sets the socket error; ignores the call if an error is already recorded. Setting an error causes the socket to close. |
| `const icy::Error &` | [`error`](#error-7) `virtual` `const` `override` | Returns the current socket error, if any. |
| `bool` | [`closed`](#closed-2) `virtual` `const` `override` | Returns true if the native socket handle is closed. |
| `net::Address` | [`address`](#address-9) `virtual` `const` `override` | Returns the IP address and port number of the socket. A wildcard address is returned if the socket is not connected. |
| `net::Address` | [`peerAddress`](#peeraddress-2) `virtual` `const` `override` | Returns the IP address and port number of the peer socket. A wildcard address is returned if the socket is not connected. |
| `net::TransportType` | [`transport`](#transport-2) `virtual` `const` `override` | Returns the TCP transport protocol. |
| `uv::Loop *` | [`loop`](#loop-4) `virtual` `const` `override` | Returns the event loop associated with this socket. |
| `void` | [`onConnect`](#onconnect-1) `virtual` | Called by the stream layer when the TCP connection is established. |
| `void` | [`onRead`](#onread-1) `virtual` `override` | Called by the stream layer with raw received bytes; wraps them in a [MutableBuffer](base.md#mutablebuffer). |
| `void` | [`onRecv`](#onrecv) `virtual` | Dispatches a received buffer to all socket adapters via onSocketRecv. |
| `void` | [`onError`](#onerror-1) `virtual` `override` | Dispatches the error to adapters and closes the socket. |
| `void` | [`onClose`](#onclose-1) `virtual` `override` | Dispatches the close event to all socket adapters. |

---

{#tcpsocket-1}

#### TCPSocket

```cpp
TCPSocket(uv::Loop * loop = uv::defaultLoop())
```

Defined in src/net/include/icy/net/tcpsocket.h:39

Constructs the [TCPSocket](#tcpsocket) and initializes the underlying libuv handle. 
#### Parameters
* `loop` Event loop to use; defaults to the default loop.

---

{#tcpsocket-2}

#### TCPSocket

```cpp
TCPSocket(const TCPSocket &) = delete
```

Defined in src/net/include/icy/net/tcpsocket.h:42

Deleted constructor.

---

{#tcpsocket-3}

#### TCPSocket

```cpp
TCPSocket(TCPSocket &&) = delete
```

Defined in src/net/include/icy/net/tcpsocket.h:44

Deleted constructor.

---

{#shutdown-5}

#### shutdown

`virtual` `nodiscard` `override`

```cpp
[[nodiscard]] virtual bool shutdown() override
```

Defined in src/net/include/icy/net/tcpsocket.h:49

Sends a TCP shutdown request; the socket closes after the peer acknowledges. 
#### Returns
true if the shutdown request was queued successfully.

##### Reimplements

- [`shutdown`](#shutdown-1)

---

{#close-15}

#### close

`virtual` `override`

```cpp
virtual void close() override
```

Defined in src/net/include/icy/net/tcpsocket.h:52

Closes the socket immediately, releasing all associated resources.

##### Reimplements

- [`close`](base.md#classicy_1_1Stream_1a702473d53d40bc5f19b997a84817a341)

##### Reimplemented by

- [`close`](#close-14)

---

{#connect-7}

#### connect

`virtual` `override`

```cpp
virtual void connect(const net::Address & peerAddress) override
```

Defined in src/net/include/icy/net/tcpsocket.h:57

Connects to `peerAddress` using a libuv connect request. On success, calls [onConnect()](#onconnect-1); on failure, calls [setUVError()](uv.md#setuverror). 
#### Parameters
* `peerAddress` The remote address to connect to.

##### Reimplements

- [`connect`](#connect-2)

##### Reimplemented by

- [`connect`](#connect-5)

---

{#connect-8}

#### connect

`virtual` `override`

```cpp
virtual void connect(std::string_view host, uint16_t port) override
```

Defined in src/net/include/icy/net/tcpsocket.h:62

Resolves `host` via DNS (or maps "localhost"), then connects. 
#### Parameters
* `host` Hostname or IP address string. 

* `port` Destination port.

##### Reimplements

- [`connect`](#connect-3)

##### Reimplemented by

- [`connect`](#connect-6)

---

{#send-4}

#### send

`virtual` `nodiscard` `override`

```cpp
[[nodiscard]] virtual ssize_t send(const char * data, size_t len, int flags = 0) override
```

Defined in src/net/include/icy/net/tcpsocket.h:69

Writes `len` bytes to the connected peer. 
#### Parameters
* `data` Pointer to the data to send. 

* `len` Number of bytes to send. 

* `flags` Reserved; currently unused. 

#### Returns
Number of bytes sent, or -1 on error.

##### Reimplements

- [`send`](#send)

##### Reimplemented by

- [`send`](#send-2)

---

{#sendowned-6}

#### sendOwned

`virtual` `nodiscard` `override`

```cpp
[[nodiscard]] virtual ssize_t sendOwned(Buffer && buffer, int flags = 0) override
```

Defined in src/net/include/icy/net/tcpsocket.h:70

Sends an owned payload buffer to the connected peer.

##### Reimplements

- [`sendOwned`](#sendowned)

##### Reimplemented by

- [`sendOwned`](#sendowned-4)

---

{#send-5}

#### send

`virtual` `nodiscard` `override`

```cpp
[[nodiscard]] virtual ssize_t send(const char * data, size_t len, const net::Address & peerAddress, int flags = 0) override
```

Defined in src/net/include/icy/net/tcpsocket.h:78

Writes `len` bytes; `peerAddress` is ignored for TCP (connected stream). 
#### Parameters
* `data` Pointer to the data to send. 

* `len` Number of bytes to send. 

* `peerAddress` Ignored; present for interface conformance. 

* `flags` Reserved; currently unused. 

#### Returns
Number of bytes sent, or -1 on error.

##### Reimplements

- [`send`](#send-1)

##### Reimplemented by

- [`send`](#send-3)

---

{#sendowned-7}

#### sendOwned

`virtual` `nodiscard` `override`

```cpp
[[nodiscard]] virtual ssize_t sendOwned(Buffer && buffer, const net::Address & peerAddress, int flags = 0) override
```

Defined in src/net/include/icy/net/tcpsocket.h:79

##### Reimplements

- [`sendOwned`](#sendowned-1)

##### Reimplemented by

- [`sendOwned`](#sendowned-5)

---

{#bind-2}

#### bind

`virtual` `override`

```cpp
virtual void bind(const net::Address & address, unsigned flags = 0) override
```

Defined in src/net/include/icy/net/tcpsocket.h:85

Binds the socket to `address`. Resets and reinitializes the handle if the address family changes. 
#### Parameters
* `address` Local address to bind to. 

* `flags` Optional bind flags (e.g. UV_TCP_IPV6ONLY is added automatically for IPv6).

##### Reimplements

- [`bind`](#bind)

##### Reimplemented by

- [`bind`](#bind-1)

---

{#listen-2}

#### listen

`virtual` `override`

```cpp
virtual void listen(int backlog = 64) override
```

Defined in src/net/include/icy/net/tcpsocket.h:89

Starts listening for incoming connections with the given backlog. 
#### Parameters
* `backlog` Maximum length of the pending connection queue.

##### Reimplements

- [`listen`](#listen)

##### Reimplemented by

- [`listen`](#listen-1)

---

{#acceptconnection-2}

#### acceptConnection

`virtual`

```cpp
virtual void acceptConnection()
```

Defined in src/net/include/icy/net/tcpsocket.h:92

Accepts the next pending client connection and fires AcceptConnection.

##### Reimplemented by

- [`acceptConnection`](#acceptconnection)

---

{#setreuseport}

#### setReusePort

`nodiscard`

```cpp
[[nodiscard]] bool setReusePort()
```

Defined in src/net/include/icy/net/tcpsocket.h:97

Enables SO_REUSEPORT on Linux kernel >= 3.9 for multi-thread load balancing. Must be called after [bind()](#bind-2). No-op and returns false on unsupported platforms. 
#### Returns
true if the socket option was set successfully.

---

{#setnodelay}

#### setNoDelay

`nodiscard`

```cpp
[[nodiscard]] bool setNoDelay(bool enable)
```

Defined in src/net/include/icy/net/tcpsocket.h:102

Enables or disables TCP_NODELAY (Nagle's algorithm). 
#### Parameters
* `enable` true to disable Nagle's algorithm (lower latency). 

#### Returns
true if the option was set successfully.

---

{#setkeepalive}

#### setKeepAlive

`nodiscard`

```cpp
[[nodiscard]] bool setKeepAlive(bool enable, int delay)
```

Defined in src/net/include/icy/net/tcpsocket.h:108

Enables or disables TCP keep-alive probes. 
#### Parameters
* `enable` true to enable keep-alive. 

* `delay` Initial delay in seconds before the first keep-alive probe. 

#### Returns
true if the option was set successfully.

---

{#setsimultaneousaccepts}

#### setSimultaneousAccepts

`nodiscard`

```cpp
[[nodiscard]] bool setSimultaneousAccepts(bool enable)
```

Defined in src/net/include/icy/net/tcpsocket.h:114

Enables or disables simultaneous accepts on Windows. No-op and returns false on non-Windows platforms. 
#### Parameters
* `enable` true to enable simultaneous accepts. 

#### Returns
true if the option was set successfully.

---

{#setmode}

#### setMode

```cpp
void setMode(SocketMode mode)
```

Defined in src/net/include/icy/net/tcpsocket.h:118

Sets the socket mode (ServerSide or ClientSide). 
#### Parameters
* `mode` The mode to assign.

---

{#mode}

#### mode

`const`

```cpp
SocketMode mode() const
```

Defined in src/net/include/icy/net/tcpsocket.h:121

Returns the current socket mode.

---

{#seterror-2}

#### setError

`virtual` `override`

```cpp
virtual void setError(const icy::Error & err) override
```

Defined in src/net/include/icy/net/tcpsocket.h:126

Sets the socket error; ignores the call if an error is already recorded. Setting an error causes the socket to close. 
#### Parameters
* `err` The error to record.

##### Reimplements

- [`setError`](uv.md#seterror)

---

{#error-7}

#### error

`virtual` `const` `override`

```cpp
virtual const icy::Error & error() const override
```

Defined in src/net/include/icy/net/tcpsocket.h:129

Returns the current socket error, if any.

##### Reimplements

- [`error`](#error-5)

---

{#closed-2}

#### closed

`virtual` `const` `override`

```cpp
virtual bool closed() const override
```

Defined in src/net/include/icy/net/tcpsocket.h:132

Returns true if the native socket handle is closed.

##### Reimplements

- [`closed`](uv.md#closed)

---

{#address-9}

#### address

`virtual` `const` `override`

```cpp
virtual net::Address address() const override
```

Defined in src/net/include/icy/net/tcpsocket.h:136

Returns the IP address and port number of the socket. A wildcard address is returned if the socket is not connected.

##### Reimplements

- [`address`](#address-8)

---

{#peeraddress-2}

#### peerAddress

`virtual` `const` `override`

```cpp
virtual net::Address peerAddress() const override
```

Defined in src/net/include/icy/net/tcpsocket.h:140

Returns the IP address and port number of the peer socket. A wildcard address is returned if the socket is not connected.

##### Reimplements

- [`peerAddress`](#peeraddress-1)

---

{#transport-2}

#### transport

`virtual` `const` `override`

```cpp
virtual net::TransportType transport() const override
```

Defined in src/net/include/icy/net/tcpsocket.h:143

Returns the TCP transport protocol.

##### Reimplements

- [`transport`](#transport)

---

{#loop-4}

#### loop

`virtual` `const` `override`

```cpp
virtual uv::Loop * loop() const override
```

Defined in src/net/include/icy/net/tcpsocket.h:146

Returns the event loop associated with this socket.

##### Reimplements

- [`loop`](#loop-3)

---

{#onconnect-1}

#### onConnect

`virtual`

```cpp
virtual void onConnect()
```

Defined in src/net/include/icy/net/tcpsocket.h:153

Called by the stream layer when the TCP connection is established.

##### Reimplemented by

- [`onConnect`](#onconnect)

---

{#onread-1}

#### onRead

`virtual` `override`

```cpp
virtual void onRead(const char * data, size_t len) override
```

Defined in src/net/include/icy/net/tcpsocket.h:158

Called by the stream layer with raw received bytes; wraps them in a [MutableBuffer](base.md#mutablebuffer). 
#### Parameters
* `data` Pointer to received bytes. 

* `len` Number of bytes received.

##### Reimplements

- [`onRead`](base.md#classicy_1_1Stream_1ad60923c284f29eae162e2d977ebebcab)

##### Reimplemented by

- [`onRead`](#onread)

---

{#onrecv}

#### onRecv

`virtual`

```cpp
virtual void onRecv(const MutableBuffer & buf)
```

Defined in src/net/include/icy/net/tcpsocket.h:162

Dispatches a received buffer to all socket adapters via onSocketRecv. 
#### Parameters
* `buf` The buffer containing the received data.

---

{#onerror-1}

#### onError

`virtual` `override`

```cpp
virtual void onError(const icy::Error & error) override
```

Defined in src/net/include/icy/net/tcpsocket.h:166

Dispatches the error to adapters and closes the socket. 
#### Parameters
* `error` The error that occurred.

##### Reimplements

- [`onError`](uv.md#onerror)

---

{#onclose-1}

#### onClose

`virtual` `override`

```cpp
virtual void onClose() override
```

Defined in src/net/include/icy/net/tcpsocket.h:169

Dispatches the close event to all socket adapters.

##### Reimplements

- [`onClose`](uv.md#onclose)

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `SocketMode` | [`_mode`](#_mode-1)  |  |
| `net::Address` | [`_peerAddress`](#_peeraddress)  | Cached peer address (avoids syscall per recv). |

---

{#_mode-1}

#### _mode

```cpp
SocketMode _mode
```

Defined in src/net/include/icy/net/tcpsocket.h:175

---

{#_peeraddress}

#### _peerAddress

```cpp
net::Address _peerAddress
```

Defined in src/net/include/icy/net/tcpsocket.h:176

Cached peer address (avoids syscall per recv).

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`init`](#init-5) `virtual` `override` | Initializes the underlying socket context. |
| `void` | [`reset`](#reset-5) `virtual` `override` | Resets the socket context for reuse. |

---

{#init-5}

#### init

`virtual` `override`

```cpp
virtual void init() override
```

Defined in src/net/include/icy/net/tcpsocket.h:172

Initializes the underlying socket context.

##### Reimplements

- [`init`](#init-4)

---

{#reset-5}

#### reset

`virtual` `override`

```cpp
virtual void reset() override
```

Defined in src/net/include/icy/net/tcpsocket.h:173

Resets the socket context for reuse.

##### Reimplements

- [`reset`](#reset-4)

### Public Types

| Name | Description |
|------|-------------|
| [`Ptr`](#ptr-9)  |  |
| [`Vec`](#vec-2)  |  |

---

{#ptr-9}

#### Ptr

```cpp
using Ptr = std::shared_ptr< TCPSocket >
```

Defined in src/net/include/icy/net/tcpsocket.h:34

---

{#vec-2}

#### Vec

```cpp
using Vec = std::vector< Ptr >
```

Defined in src/net/include/icy/net/tcpsocket.h:35

{#transaction}

## Transaction

```cpp
#include <icy/net/transaction.h>
```

```cpp
template<class PacketT>
class Transaction
```

Defined in src/net/include/icy/net/transaction.h:27

> **Inherits:** [`PacketTransaction< PacketT >`](base.md#packettransaction), [`PacketSocketEmitter`](#packetsocketemitter)

Request/response helper for packet types emitted from a socket.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`IntrusivePtr`](#icy-intrusiveptr) | `friend` | Declared here |
| [`Transaction`](#transaction-1) | `function` | Declared here |
| [`send`](#send-6) | `function` | Declared here |
| [`cancel`](#cancel) | `function` | Declared here |
| [`dispose`](#dispose) | `function` | Declared here |
| [`peerAddress`](#peeraddress-3) | `function` | Declared here |
| [`_peerAddress`](#_peeraddress-1) | `variable` | Declared here |
| [`onPacket`](#onpacket-1) | `function` | Declared here |
| [`onResponse`](#onresponse) | `function` | Declared here |
| [`checkResponse`](#checkresponse) | `function` | Declared here |
| [`BaseT`](#baset) | `typedef` | Declared here |
| [`Ptr`](base.md#classicy_1_1PacketTransaction_1a44ea6d9d47efb59eebdc71158c0158c5) | `typedef` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`IntrusivePtr`](base.md#classicy_1_1PacketTransaction_1a04a374ecab2499e81653198fd49ff15e) | `friend` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`_request`](base.md#classicy_1_1PacketTransaction_1a03e21f66e0f957926f03f37b48276dca) | `variable` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`_response`](base.md#classicy_1_1PacketTransaction_1a6322ae70f87db74c1d5c130b7fec7eb8) | `variable` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`_timer`](base.md#classicy_1_1PacketTransaction_1a1b02dbf0ac65fcad82c50a6867f81a79) | `variable` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`_retries`](base.md#classicy_1_1PacketTransaction_1a8c76a907486cae828c77f0302d20a844) | `variable` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`_attempts`](base.md#classicy_1_1PacketTransaction_1ace42ededd6279b9a3ce13bdfd6619c11) | `variable` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`_disposed`](base.md#classicy_1_1PacketTransaction_1a70d8cc5501fba3b9c26426af411241a4) | `variable` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`PacketTransaction`](base.md#classicy_1_1PacketTransaction_1a5b3c9258f951866005347123bdf7a7ff) | `function` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`PacketTransaction`](base.md#classicy_1_1PacketTransaction_1a711873135d73ab6cb3426db65e980196) | `function` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`send`](base.md#classicy_1_1PacketTransaction_1acf7a9bbcfff95999c0c227cb36f9cfd4) | `function` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`cancel`](base.md#classicy_1_1PacketTransaction_1ab7c1e492203b850fc9b5c1aa4f97c3ef) | `function` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`cancelled`](base.md#classicy_1_1PacketTransaction_1a24663172b10c70530b29c9dccdf5813d) | `function` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`dispose`](base.md#classicy_1_1PacketTransaction_1a59df328cafacdcea30f86ea38c4718ec) | `function` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`disposed`](base.md#classicy_1_1PacketTransaction_1a2a85d3be5eb4088c1f033c04c0a0342c) | `function` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`canResend`](base.md#classicy_1_1PacketTransaction_1aca2387c6623fb21b6345bf7a1e45612b) | `function` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`attempts`](base.md#classicy_1_1PacketTransaction_1a7c19c40febe7e9f6471a7ea5624c933e) | `function` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`retries`](base.md#classicy_1_1PacketTransaction_1a81bb9f59219ebd7399dfdbafe821dcb4) | `function` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`request`](base.md#classicy_1_1PacketTransaction_1a3a5ebb2afffd6d94e465a1db318161e5) | `function` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`request`](base.md#classicy_1_1PacketTransaction_1abe8ba9b6ee2c439944d84386204d5818) | `function` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`response`](base.md#classicy_1_1PacketTransaction_1a16cdc5ec77f02ab8213de87bb56f709f) | `function` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`response`](base.md#classicy_1_1PacketTransaction_1a50e68834b240b876dae1a7d1f84afb56) | `function` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`~PacketTransaction`](base.md#classicy_1_1PacketTransaction_1ac118e461ae9ba6fc8f3b69fe82fda134) | `function` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`onStateChange`](base.md#classicy_1_1PacketTransaction_1a14bf74aa47986f672b4f711a2f98e38d) | `function` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`handlePotentialResponse`](base.md#classicy_1_1PacketTransaction_1ad392bb447a02bcd8425614589966613a) | `function` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`checkResponse`](base.md#classicy_1_1PacketTransaction_1a36b55484206d69dff6375d61b5fb834b) | `function` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`onResponse`](base.md#classicy_1_1PacketTransaction_1a2ba8f73029cf89d03c1d641c8c3a42f3) | `function` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`onTimeout`](base.md#classicy_1_1PacketTransaction_1ad515edcc4134a741acea3f407409fc56) | `function` | Inherited from [`PacketTransaction`](base.md#packettransaction) |
| [`send`](base.md#send-9) | `function` | Inherited from [`Sendable`](base.md#sendable) |
| [`cancel`](base.md#cancel-2) | `function` | Inherited from [`Sendable`](base.md#sendable) |
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
| [`_refCount`](base.md#classicy_1_1RefCounted_1a9ad69b4e82cc4cce1d55780afea9e589) | `variable` | Inherited from [`RefCounted`](base.md#refcounted) |
| [`RefCounted`](base.md#classicy_1_1RefCounted_1a69db516c7fc776ec189af3cbd4c9804b) | `function` | Inherited from [`RefCounted`](base.md#refcounted) |
| [`RefCounted`](base.md#classicy_1_1RefCounted_1a4c715cce08cc8f065a0740716d828c51) | `function` | Inherited from [`RefCounted`](base.md#refcounted) |
| [`operator=`](base.md#classicy_1_1RefCounted_1a40bc82fe701cbdad57046c7b0ebd782b) | `function` | Inherited from [`RefCounted`](base.md#refcounted) |
| [`addRef`](base.md#classicy_1_1RefCounted_1a8d439708a2a8535bd0f98d58ba8b8950) | `function` | Inherited from [`RefCounted`](base.md#refcounted) |
| [`releaseRef`](base.md#classicy_1_1RefCounted_1a842da5db14ba73d10c95b5be072cbd1f) | `function` | Inherited from [`RefCounted`](base.md#refcounted) |
| [`refCount`](base.md#classicy_1_1RefCounted_1a0bd7a4e6db7b4582fcec1e632901ad2f) | `function` | Inherited from [`RefCounted`](base.md#refcounted) |
| [`~RefCounted`](base.md#classicy_1_1RefCounted_1a605be798c4be05f218ef0e84f5c5e909) | `function` | Inherited from [`RefCounted`](base.md#refcounted) |
| [`factory`](#factory) | `variable` | Inherited from [`PacketSocketEmitter`](#packetsocketemitter) |
| [`PacketSocketEmitter`](#packetsocketemitter-1) | `function` | Inherited from [`PacketSocketEmitter`](#packetsocketemitter) |
| [`onSocketRecv`](#onsocketrecv) | `function` | Inherited from [`PacketSocketEmitter`](#packetsocketemitter) |
| [`onPacket`](#onpacket) | `function` | Inherited from [`PacketSocketEmitter`](#packetsocketemitter) |
| [`Connect`](#connect-4) | `variable` | Inherited from [`SocketEmitter`](#socketemitter) |
| [`Recv`](#recv) | `variable` | Inherited from [`SocketEmitter`](#socketemitter) |
| [`Error`](#error-6) | `variable` | Inherited from [`SocketEmitter`](#socketemitter) |
| [`Close`](#close-13) | `variable` | Inherited from [`SocketEmitter`](#socketemitter) |
| [`impl`](#impl) | `variable` | Inherited from [`SocketEmitter`](#socketemitter) |
| [`SocketEmitter`](#socketemitter-1) | `function` | Inherited from [`SocketEmitter`](#socketemitter) |
| [`SocketEmitter`](#socketemitter-2) | `function` | Inherited from [`SocketEmitter`](#socketemitter) |
| [`~SocketEmitter`](#socketemitter-3) | `function` | Inherited from [`SocketEmitter`](#socketemitter) |
| [`addReceiver`](#addreceiver-1) | `function` | Inherited from [`SocketEmitter`](#socketemitter) |
| [`removeReceiver`](#removereceiver-1) | `function` | Inherited from [`SocketEmitter`](#socketemitter) |
| [`swap`](#swap-1) | `function` | Inherited from [`SocketEmitter`](#socketemitter) |
| [`as`](#as) | `function` | Inherited from [`SocketEmitter`](#socketemitter) |
| [`operator->`](#operator-9) | `function` | Inherited from [`SocketEmitter`](#socketemitter) |
| [`onSocketConnect`](#onsocketconnect-1) | `function` | Inherited from [`SocketEmitter`](#socketemitter) |
| [`onSocketRecv`](#onsocketrecv-2) | `function` | Inherited from [`SocketEmitter`](#socketemitter) |
| [`onSocketError`](#onsocketerror-1) | `function` | Inherited from [`SocketEmitter`](#socketemitter) |
| [`onSocketClose`](#onsocketclose-1) | `function` | Inherited from [`SocketEmitter`](#socketemitter) |
| [`priority`](#priority-1) | `variable` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`SocketAdapter`](#socketadapter-1) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`~SocketAdapter`](#socketadapter-2) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`send`](#send) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`send`](#send-1) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`sendOwned`](#sendowned-2) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`sendOwned`](#sendowned-3) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`sendPacket`](#sendpacket) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`sendPacket`](#sendpacket-1) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`sendPacket`](#sendpacket-2) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`setSender`](#setsender) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`sender`](#sender) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`addReceiver`](#addreceiver) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`removeReceiver`](#removereceiver) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`hasReceiver`](#hasreceiver) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`receivers`](#receivers) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`onSocketConnect`](#onsocketconnect) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`onSocketRecv`](#onsocketrecv-1) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`onSocketError`](#onsocketerror) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`onSocketClose`](#onsocketclose) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`_sender`](#_sender) | `variable` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`_receivers`](#_receivers) | `variable` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`_dirty`](#_dirty) | `variable` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`cleanupReceivers`](#cleanupreceivers) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |

### Inherited from [`PacketTransaction`](base.md#packettransaction)

| Kind | Name | Description |
|------|------|-------------|
| `typedef` | [`Ptr`](base.md#classicy_1_1PacketTransaction_1a44ea6d9d47efb59eebdc71158c0158c5)  |  |
| `friend` | [`IntrusivePtr`](base.md#classicy_1_1PacketTransaction_1a04a374ecab2499e81653198fd49ff15e)  |  |
| `variable` | [`_request`](base.md#classicy_1_1PacketTransaction_1a03e21f66e0f957926f03f37b48276dca)  |  |
| `variable` | [`_response`](base.md#classicy_1_1PacketTransaction_1a6322ae70f87db74c1d5c130b7fec7eb8)  |  |
| `variable` | [`_timer`](base.md#classicy_1_1PacketTransaction_1a1b02dbf0ac65fcad82c50a6867f81a79)  | The request timeout callback. |
| `variable` | [`_retries`](base.md#classicy_1_1PacketTransaction_1a8c76a907486cae828c77f0302d20a844)  | The maximum number of attempts before the transaction is considered failed. |
| `variable` | [`_attempts`](base.md#classicy_1_1PacketTransaction_1ace42ededd6279b9a3ce13bdfd6619c11)  | The number of times the transaction has been sent. |
| `variable` | [`_disposed`](base.md#classicy_1_1PacketTransaction_1a70d8cc5501fba3b9c26426af411241a4)  |  |
| `function` | [`PacketTransaction`](base.md#classicy_1_1PacketTransaction_1a5b3c9258f951866005347123bdf7a7ff) `inline` |  |
| `function` | [`PacketTransaction`](base.md#classicy_1_1PacketTransaction_1a711873135d73ab6cb3426db65e980196) `inline` |  |
| `function` | [`send`](base.md#classicy_1_1PacketTransaction_1acf7a9bbcfff95999c0c227cb36f9cfd4) `virtual` `inline` `override` | Starts the transaction timer and sends the request. Overriding classes should implement send logic here. |
| `function` | [`cancel`](base.md#classicy_1_1PacketTransaction_1ab7c1e492203b850fc9b5c1aa4f97c3ef) `virtual` `inline` `override` | Cancellation means that the agent will not retransmit the request, will not treat the lack of response to be a failure, but will wait the duration of the transaction timeout for a response. Transitions the transaction to the `Cancelled` state. |
| `function` | [`cancelled`](base.md#classicy_1_1PacketTransaction_1a24663172b10c70530b29c9dccdf5813d) `const` `inline` |  |
| `function` | [`dispose`](base.md#classicy_1_1PacketTransaction_1a59df328cafacdcea30f86ea38c4718ec) `virtual` `inline` | Stops the timer and unregisters callbacks. Does NOT delete the object; the [IntrusivePtr](base.md#intrusiveptr) destructor handles deletion when the last reference is released. Safe to call multiple times. |
| `function` | [`disposed`](base.md#classicy_1_1PacketTransaction_1a2a85d3be5eb4088c1f033c04c0a0342c) `const` `inline` `nodiscard` |  |
| `function` | [`canResend`](base.md#classicy_1_1PacketTransaction_1aca2387c6623fb21b6345bf7a1e45612b) `virtual` `inline` |  |
| `function` | [`attempts`](base.md#classicy_1_1PacketTransaction_1a7c19c40febe7e9f6471a7ea5624c933e) `const` `inline` |  |
| `function` | [`retries`](base.md#classicy_1_1PacketTransaction_1a81bb9f59219ebd7399dfdbafe821dcb4) `const` `inline` |  |
| `function` | [`request`](base.md#classicy_1_1PacketTransaction_1a3a5ebb2afffd6d94e465a1db318161e5) `inline` |  |
| `function` | [`request`](base.md#classicy_1_1PacketTransaction_1abe8ba9b6ee2c439944d84386204d5818) `const` `inline` |  |
| `function` | [`response`](base.md#classicy_1_1PacketTransaction_1a16cdc5ec77f02ab8213de87bb56f709f) `inline` |  |
| `function` | [`response`](base.md#classicy_1_1PacketTransaction_1a50e68834b240b876dae1a7d1f84afb56) `const` `inline` |  |
| `function` | [`~PacketTransaction`](base.md#classicy_1_1PacketTransaction_1ac118e461ae9ba6fc8f3b69fe82fda134) `virtual` `inline` |  |
| `function` | [`onStateChange`](base.md#classicy_1_1PacketTransaction_1a14bf74aa47986f672b4f711a2f98e38d) `virtual` `inline` `override` | Post state change hook. Calls [dispose()](base.md#classicy_1_1PacketTransaction_1a59df328cafacdcea30f86ea38c4718ec) on terminal states to stop the timer, but does not delete the object; [IntrusivePtr](base.md#intrusiveptr) handles that. |
| `function` | [`handlePotentialResponse`](base.md#classicy_1_1PacketTransaction_1ad392bb447a02bcd8425614589966613a) `virtual` `inline` | Processes a potential response candidate and updates the state accordingly. |
| `function` | [`checkResponse`](base.md#classicy_1_1PacketTransaction_1a36b55484206d69dff6375d61b5fb834b) `virtual` | Checks a potential response candidate and returns true on successful match. |
| `function` | [`onResponse`](base.md#classicy_1_1PacketTransaction_1a2ba8f73029cf89d03c1d641c8c3a42f3) `virtual` `inline` | Called when a successful response is received. |
| `function` | [`onTimeout`](base.md#classicy_1_1PacketTransaction_1ad515edcc4134a741acea3f407409fc56) `virtual` `inline` | Called by the timer when the transaction timeout elapses. Retransmits if retries remain, otherwise transitions to `Failed`. |

### Inherited from [`Sendable`](base.md#sendable)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`send`](base.md#send-9) `virtual` | Initiates the send operation. |
| `function` | [`cancel`](base.md#cancel-2) `virtual` | Cancels a pending send operation. |

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

### Inherited from [`PacketSocketEmitter`](#packetsocketemitter)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`factory`](#factory)  | The packet factory. |
| `function` | [`PacketSocketEmitter`](#packetsocketemitter-1)  | Creates the [PacketSocketEmitter](#packetsocketemitter) and attaches it to the given socket. |
| `function` | [`onSocketRecv`](#onsocketrecv) `virtual` `override` | Parses raw received data into packets via the factory and forwards each parsed packet to [onPacket()](#onpacket). Returns true if propagation should stop. |
| `function` | [`onPacket`](#onpacket) `virtual` | [Process](base.md#process) a parsed packet. Returns true to stop propagation. |

### Inherited from [`SocketEmitter`](#socketemitter)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`Connect`](#connect-4)  | Signals that the socket is connected. |
| `variable` | [`Recv`](#recv)  | Signals when data is received by the socket. |
| `variable` | [`Error`](#error-6)  | Signals that the socket is closed in error. This signal will be sent just before the Closed signal. |
| `variable` | [`Close`](#close-13)  | Signals that the underlying socket is closed. |
| `variable` | [`impl`](#impl)  | Pointer to the underlying socket. Sent data will be proxied to this socket. |
| `function` | [`SocketEmitter`](#socketemitter-1)  | Creates the [SocketEmitter](#socketemitter) and optionally attaches it to a socket. If `socket` is provided, this emitter registers itself as a receiver. |
| `function` | [`SocketEmitter`](#socketemitter-2)  | Copy constructor; copies all signal connections and attaches to the same socket. |
| `function` | [`~SocketEmitter`](#socketemitter-3) `virtual` `noexcept` | Destroys the [SocketAdapter](#socketadapter). |
| `function` | [`addReceiver`](#addreceiver-1) `virtual` `override` | Attaches a [SocketAdapter](#socketadapter) as a receiver; wires it to all four socket signals. |
| `function` | [`removeReceiver`](#removereceiver-1) `virtual` `override` | Detaches a [SocketAdapter](#socketadapter) from all four socket signals. |
| `function` | [`swap`](#swap-1) `virtual` | Replaces the underlying socket with `socket`. |
| `function` | [`as`](#as) `inline` | Returns the underlying socket cast to type T, or nullptr if the cast fails. |
| `function` | [`operator->`](#operator-9) `const` `inline` | Returns a raw pointer to the underlying socket for direct method access. Follows shared_ptr semantics; the caller must not delete the returned pointer. |
| `function` | [`onSocketConnect`](#onsocketconnect-1) `virtual` `override` | Forwards the connect event to chained adapters, then fires the Connect signal. |
| `function` | [`onSocketRecv`](#onsocketrecv-2) `virtual` `override` | Forwards the recv event to chained adapters, then fires the Recv signal. |
| `function` | [`onSocketError`](#onsocketerror-1) `virtual` `override` | Forwards the error event to chained adapters, then fires the [Error](base.md#error) signal. |
| `function` | [`onSocketClose`](#onsocketclose-1) `virtual` `override` | Forwards the close event to chained adapters, then fires the Close signal. |

### Inherited from [`SocketAdapter`](#socketadapter)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`priority`](#priority-1)  | The priority of this adapter for STL sort operations. |
| `function` | [`SocketAdapter`](#socketadapter-1)  | Creates the [SocketAdapter](#socketadapter). |
| `function` | [`~SocketAdapter`](#socketadapter-2) `virtual` `noexcept` | Destroys the [SocketAdapter](#socketadapter). |
| `function` | [`send`](#send) `virtual` `nodiscard` | Sends the given data buffer to the connected peer. Returns the number of bytes sent or -1 on error. No exception will be thrown. For TCP sockets the given peer address must match the connected peer address. |
| `function` | [`send`](#send-1) `virtual` `nodiscard` |  |
| `function` | [`sendOwned`](#sendowned-2) `virtual` `nodiscard` | Sends an owned payload buffer to the connected peer. |
| `function` | [`sendOwned`](#sendowned-3) `virtual` `nodiscard` |  |
| `function` | [`sendPacket`](#sendpacket) `virtual` | Sends the given packet to the connected peer. Returns the number of bytes sent or -1 on error. No exception will be thrown. For TCP sockets the given peer address must match the connected peer address. |
| `function` | [`sendPacket`](#sendpacket-1) `virtual` |  |
| `function` | [`sendPacket`](#sendpacket-2) `virtual` | Sends the given packet to the connected peer. This method provides delegate compatibility, and unlike other send methods throws an exception if the underlying socket is closed. |
| `function` | [`setSender`](#setsender) `virtual` | Sets the pointer to the outgoing data adapter. Send methods proxy data to this adapter by default. |
| `function` | [`sender`](#sender)  | Returns the output [SocketAdapter](#socketadapter) pointer. |
| `function` | [`addReceiver`](#addreceiver) `virtual` | Sets the pointer to the incoming data adapter. Events proxy data to this adapter by default. |
| `function` | [`removeReceiver`](#removereceiver) `virtual` | Remove the given receiver. |
| `function` | [`hasReceiver`](#hasreceiver) `virtual` | Returns true if the given receiver is connected. |
| `function` | [`receivers`](#receivers)  | Returns all currently registered input [SocketAdapter](#socketadapter) pointers. Dead (removed) entries are excluded from the returned list. |
| `function` | [`onSocketConnect`](#onsocketconnect) `virtual` | Called when the socket establishes a connection. Forwards the event to all registered receivers in priority order. Override to intercept before the application sees the event. |
| `function` | [`onSocketRecv`](#onsocketrecv-1) `virtual` | Called when data is received from the socket. Forwards the event to all registered receivers in priority order. |
| `function` | [`onSocketError`](#onsocketerror) `virtual` | Called when the socket encounters an error. Forwards the event to all registered receivers in priority order. |
| `function` | [`onSocketClose`](#onsocketclose) `virtual` | Called when the socket is closed. Forwards the event to all registered receivers in priority order. |
| `variable` | [`_sender`](#_sender)  |  |
| `variable` | [`_receivers`](#_receivers)  |  |
| `variable` | [`_dirty`](#_dirty)  |  |
| `function` | [`cleanupReceivers`](#cleanupreceivers) `virtual` |  |

### Friends

| Name | Description |
|------|-------------|
| [`icy::IntrusivePtr`](#icy-intrusiveptr)  |  |

---

{#icy-intrusiveptr}

#### icy::IntrusivePtr

```cpp
template<typename U> friend class icy::IntrusivePtr
```

Defined in src/net/include/icy/net/transaction.h:81

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Transaction`](#transaction-1) `inline` | Constructs a [Transaction](#transaction) on the given socket targeting `peerAddress`. |
| `bool` | [`send`](#send-6) `virtual` `inline` `override` | Sends the request packet to the peer address and starts the timeout timer. Sets state to Failed and returns false if the packet could not be sent. |
| `void` | [`cancel`](#cancel) `virtual` `inline` `override` | Cancels the transaction and stops the timeout timer. |
| `void` | [`dispose`](#dispose) `virtual` `inline` `override` | Stops the timer and unregisters callbacks. |
| `Address` | [`peerAddress`](#peeraddress-3) `const` `inline` | Returns the remote peer address used for this transaction. |

---

{#transaction-1}

#### Transaction

`inline`

```cpp
inline Transaction(const net::Socket::Ptr & socket, const Address & peerAddress, int timeout = 10000, int retries = 1)
```

Defined in src/net/include/icy/net/transaction.h:39

Constructs a [Transaction](#transaction) on the given socket targeting `peerAddress`. 
#### Parameters
* `socket` The socket to send/receive packets on. 

* `peerAddress` The remote address for the request and response matching. 

* `timeout` Milliseconds to wait for a response before failing. 

* `retries` Number of additional send attempts on timeout.

---

{#send-6}

#### send

`virtual` `inline` `override`

```cpp
virtual inline bool send() override
```

Defined in src/net/include/icy/net/transaction.h:51

Sends the request packet to the peer address and starts the timeout timer. Sets state to Failed and returns false if the packet could not be sent. 
#### Returns
true if the packet was sent and the timer started successfully.

##### Reimplements

- [`send`](base.md#classicy_1_1PacketTransaction_1acf7a9bbcfff95999c0c227cb36f9cfd4)

---

{#cancel}

#### cancel

`virtual` `inline` `override`

```cpp
virtual inline void cancel() override
```

Defined in src/net/include/icy/net/transaction.h:61

Cancels the transaction and stops the timeout timer.

##### Reimplements

- [`cancel`](base.md#classicy_1_1PacketTransaction_1ab7c1e492203b850fc9b5c1aa4f97c3ef)

---

{#dispose}

#### dispose

`virtual` `inline` `override`

```cpp
virtual inline void dispose() override
```

Defined in src/net/include/icy/net/transaction.h:68

Stops the timer and unregisters callbacks.

##### Reimplements

- [`dispose`](base.md#classicy_1_1PacketTransaction_1a59df328cafacdcea30f86ea38c4718ec)

---

{#peeraddress-3}

#### peerAddress

`const` `inline`

```cpp
inline Address peerAddress() const
```

Defined in src/net/include/icy/net/transaction.h:75

Returns the remote peer address used for this transaction.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `Address` | [`_peerAddress`](#_peeraddress-1)  |  |

---

{#_peeraddress-1}

#### _peerAddress

```cpp
Address _peerAddress
```

Defined in src/net/include/icy/net/transaction.h:118

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `bool` | [`onPacket`](#onpacket-1) `virtual` `inline` `override` | Checks whether `packet` is a matching response for the pending request. If it matches, the transaction completes; socket data propagation stops. |
| `void` | [`onResponse`](#onresponse) `virtual` `inline` `override` | Called when a confirmed response is received; emits the response via [PacketSignal](base.md#packetsignal). |
| `bool` | [`checkResponse`](#checkresponse) `virtual` `inline` `override` | Returns true if `packet` is a valid response for this transaction. |

---

{#onpacket-1}

#### onPacket

`virtual` `inline` `override`

```cpp
virtual inline bool onPacket(IPacket & packet) override
```

Defined in src/net/include/icy/net/transaction.h:89

Checks whether `packet` is a matching response for the pending request. If it matches, the transaction completes; socket data propagation stops. 
#### Parameters
* `packet` The received packet to test. 

#### Returns
true to stop further propagation of the socket data event.

##### Reimplements

- [`onPacket`](#onpacket)

---

{#onresponse}

#### onResponse

`virtual` `inline` `override`

```cpp
virtual inline void onResponse() override
```

Defined in src/net/include/icy/net/transaction.h:97

Called when a confirmed response is received; emits the response via [PacketSignal](base.md#packetsignal).

##### Reimplements

- [`onResponse`](base.md#classicy_1_1PacketTransaction_1a2ba8f73029cf89d03c1d641c8c3a42f3)

##### Reimplemented by

- [`onResponse`](stun.md#onresponse-1)

---

{#checkresponse}

#### checkResponse

`virtual` `inline` `override`

```cpp
virtual inline bool checkResponse(const PacketT & packet) override
```

Defined in src/net/include/icy/net/transaction.h:110

Returns true if `packet` is a valid response for this transaction.

The base implementation matches the local socket address against the [PacketInfo](#packetinfo) socket address and the stored peer address against the [PacketInfo](#packetinfo) peer address. Subclasses may override for stricter matching. 
#### Parameters
* `packet` The candidate response packet. 

#### Returns
true if the packet satisfies the response criteria.

##### Reimplements

- [`checkResponse`](base.md#classicy_1_1PacketTransaction_1a36b55484206d69dff6375d61b5fb834b)

### Public Types

| Name | Description |
|------|-------------|
| [`BaseT`](#baset)  |  |

---

{#baset}

#### BaseT

```cpp
using BaseT = PacketTransaction< PacketT >
```

Defined in src/net/include/icy/net/transaction.h:31

{#udpsocket}

## UDPSocket

```cpp
#include <icy/net/udpsocket.h>
```

```cpp
class UDPSocket
```

Defined in src/net/include/icy/net/udpsocket.h:28

> **Inherits:** [`Handle< uv_udp_t >`](uv.md#handle-2), [`Socket`](#socket-1)

UDP socket implementation.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`UDPSocket`](#udpsocket-1) | `function` | Declared here |
| [`UDPSocket`](#udpsocket-2) | `function` | Declared here |
| [`UDPSocket`](#udpsocket-3) | `function` | Declared here |
| [`connect`](#connect-9) | `function` | Declared here |
| [`connect`](#connect-10) | `function` | Declared here |
| [`close`](#close-16) | `function` | Declared here |
| [`bind`](#bind-3) | `function` | Declared here |
| [`send`](#send-7) | `function` | Declared here |
| [`sendOwned`](#sendowned-8) | `function` | Declared here |
| [`send`](#send-8) | `function` | Declared here |
| [`sendOwned`](#sendowned-9) | `function` | Declared here |
| [`setBroadcast`](#setbroadcast) | `function` | Declared here |
| [`setMulticastLoop`](#setmulticastloop) | `function` | Declared here |
| [`setMulticastTTL`](#setmulticastttl) | `function` | Declared here |
| [`address`](#address-10) | `function` | Declared here |
| [`peerAddress`](#peeraddress-4) | `function` | Declared here |
| [`transport`](#transport-3) | `function` | Declared here |
| [`setError`](#seterror-3) | `function` | Declared here |
| [`error`](#error-8) | `function` | Declared here |
| [`closed`](#closed-3) | `function` | Declared here |
| [`loop`](#loop-5) | `function` | Declared here |
| [`onRecv`](#onrecv-1) | `function` | Declared here |
| [`_peer`](#_peer) | `variable` | Declared here |
| [`_buffer`](#_buffer) | `variable` | Declared here |
| [`init`](#init-6) | `function` | Declared here |
| [`reset`](#reset-6) | `function` | Declared here |
| [`onError`](#onerror-2) | `function` | Declared here |
| [`onClose`](#onclose-2) | `function` | Declared here |
| [`recvStart`](#recvstart) | `function` | Declared here |
| [`recvStop`](#recvstop) | `function` | Declared here |
| [`Ptr`](#ptr-10) | `typedef` | Declared here |
| [`Vec`](#vec-3) | `typedef` | Declared here |
| [`Handle`](uv.md#handle-3) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`init`](uv.md#init-2) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`invoke`](uv.md#invoke) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`invokeOrThrow`](uv.md#invokeorthrow) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`close`](uv.md#close-11) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`ref`](uv.md#ref) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`unref`](uv.md#unref) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`initialized`](uv.md#initialized-1) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`active`](uv.md#active) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`closing`](uv.md#closing-1) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`closed`](uv.md#closed) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`error`](uv.md#error-4) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`setError`](uv.md#seterror) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`setUVError`](uv.md#setuverror) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`setAndThrowError`](uv.md#setandthrowerror) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`throwLastError`](uv.md#throwlasterror) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`loop`](uv.md#loop-2) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`reset`](uv.md#reset-3) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`get`](uv.md#get-2) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`tid`](uv.md#tid) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`context`](uv.md#context-3) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`setCloseCleanup`](uv.md#setclosecleanup) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`clearCloseCleanup`](uv.md#clearclosecleanup) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`assertThread`](uv.md#assertthread) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`_loop`](uv.md#_loop) | `variable` | Inherited from [`Handle`](uv.md#handle-2) |
| [`_context`](uv.md#_context) | `variable` | Inherited from [`Handle`](uv.md#handle-2) |
| [`_tid`](uv.md#_tid) | `variable` | Inherited from [`Handle`](uv.md#handle-2) |
| [`_error`](uv.md#_error-1) | `variable` | Inherited from [`Handle`](uv.md#handle-2) |
| [`onError`](uv.md#onerror) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`onClose`](uv.md#onclose) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`Handle`](uv.md#handle-4) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`Handle`](uv.md#handle-5) | `function` | Inherited from [`Handle`](uv.md#handle-2) |
| [`Type`](uv.md#type-7) | `typedef` | Inherited from [`Handle`](uv.md#handle-2) |
| [`opaque`](#opaque) | `variable` | Inherited from [`Socket`](#socket-1) |
| [`Socket`](#socket-2) | `function` | Inherited from [`Socket`](#socket-1) |
| [`Socket`](#socket-3) | `function` | Inherited from [`Socket`](#socket-1) |
| [`Socket`](#socket-4) | `function` | Inherited from [`Socket`](#socket-1) |
| [`connect`](#connect-2) | `function` | Inherited from [`Socket`](#socket-1) |
| [`connect`](#connect-3) | `function` | Inherited from [`Socket`](#socket-1) |
| [`bind`](#bind) | `function` | Inherited from [`Socket`](#socket-1) |
| [`listen`](#listen) | `function` | Inherited from [`Socket`](#socket-1) |
| [`shutdown`](#shutdown-1) | `function` | Inherited from [`Socket`](#socket-1) |
| [`sendOwned`](#sendowned) | `function` | Inherited from [`Socket`](#socket-1) |
| [`sendOwned`](#sendowned-1) | `function` | Inherited from [`Socket`](#socket-1) |
| [`close`](#close-12) | `function` | Inherited from [`Socket`](#socket-1) |
| [`address`](#address-8) | `function` | Inherited from [`Socket`](#socket-1) |
| [`peerAddress`](#peeraddress-1) | `function` | Inherited from [`Socket`](#socket-1) |
| [`transport`](#transport) | `function` | Inherited from [`Socket`](#socket-1) |
| [`setError`](#seterror-1) | `function` | Inherited from [`Socket`](#socket-1) |
| [`error`](#error-5) | `function` | Inherited from [`Socket`](#socket-1) |
| [`closed`](#closed-1) | `function` | Inherited from [`Socket`](#socket-1) |
| [`loop`](#loop-3) | `function` | Inherited from [`Socket`](#socket-1) |
| [`_af`](#_af) | `variable` | Inherited from [`Socket`](#socket-1) |
| [`init`](#init-4) | `function` | Inherited from [`Socket`](#socket-1) |
| [`reset`](#reset-4) | `function` | Inherited from [`Socket`](#socket-1) |
| [`Ptr`](#ptr-4) | `typedef` | Inherited from [`Socket`](#socket-1) |
| [`Vec`](#vec) | `typedef` | Inherited from [`Socket`](#socket-1) |
| [`priority`](#priority-1) | `variable` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`SocketAdapter`](#socketadapter-1) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`~SocketAdapter`](#socketadapter-2) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`send`](#send) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`send`](#send-1) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`sendOwned`](#sendowned-2) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`sendOwned`](#sendowned-3) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`sendPacket`](#sendpacket) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`sendPacket`](#sendpacket-1) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`sendPacket`](#sendpacket-2) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`setSender`](#setsender) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`sender`](#sender) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`addReceiver`](#addreceiver) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`removeReceiver`](#removereceiver) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`hasReceiver`](#hasreceiver) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`receivers`](#receivers) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`onSocketConnect`](#onsocketconnect) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`onSocketRecv`](#onsocketrecv-1) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`onSocketError`](#onsocketerror) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`onSocketClose`](#onsocketclose) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`_sender`](#_sender) | `variable` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`_receivers`](#_receivers) | `variable` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`_dirty`](#_dirty) | `variable` | Inherited from [`SocketAdapter`](#socketadapter) |
| [`cleanupReceivers`](#cleanupreceivers) | `function` | Inherited from [`SocketAdapter`](#socketadapter) |

### Inherited from [`Handle`](uv.md#handle-2)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`Handle`](uv.md#handle-3) `inline` | Construct the handle bound to the given event loop. |
| `function` | [`init`](uv.md#init-2) `inline` | Initialize the underlying libuv handle by calling `f` with the loop, the raw handle pointer, and any additional `args`. |
| `function` | [`invoke`](uv.md#invoke) `inline` | Invoke a libuv function `f` with `args` on the initialized handle. |
| `function` | [`invokeOrThrow`](uv.md#invokeorthrow) `inline` | Invoke a libuv function `f` with `args`, throwing on failure. |
| `function` | [`close`](uv.md#close-11) `virtual` `inline` | Close and destroy the handle. |
| `function` | [`ref`](uv.md#ref) `inline` | Re-reference the handle with the event loop after a previous `[unref()](uv.md#unref)`. |
| `function` | [`unref`](uv.md#unref) `inline` | Unreference the handle from the event loop. |
| `function` | [`initialized`](uv.md#initialized-1) `const` `inline` | Return `true` if the handle has been successfully initialized via `[init()](uv.md#init-2)`. |
| `function` | [`active`](uv.md#active) `virtual` `const` `inline` | Return `true` when the handle is active (libuv `uv_is_active`). |
| `function` | [`closing`](uv.md#closing-1) `virtual` `const` `inline` | Return `true` if `uv_close` has been called and the handle is awaiting its close callback (libuv `uv_is_closing`). |
| `function` | [`closed`](uv.md#closed) `virtual` `const` `inline` | Return `true` if the handle has been fully closed (context released). |
| `function` | [`error`](uv.md#error-4) `const` `inline` | Return the last error set on this handle, or a default-constructed `[Error](base.md#error)` if no error has occurred. |
| `function` | [`setError`](uv.md#seterror) `virtual` `inline` | Set the error state and invoke `[onError()](uv.md#onerror)`. |
| `function` | [`setUVError`](uv.md#setuverror) `inline` | Translate a libuv error code into an `[Error](base.md#error)` and call `[setError()](uv.md#seterror)`. |
| `function` | [`setAndThrowError`](uv.md#setandthrowerror) `inline` | Set the error state from a libuv error code and throw a `std::runtime_error`. |
| `function` | [`throwLastError`](uv.md#throwlasterror) `inline` | Throw a `std::runtime_error` if the handle currently holds an error. |
| `function` | [`loop`](uv.md#loop-2) `const` `inline` | Return the event loop this handle is bound to. |
| `function` | [`reset`](uv.md#reset-3) `inline` | Close the current handle (if open) and allocate a fresh `[Context](uv.md#context-1)`, leaving the handle ready to be re-initialized via `[init()](uv.md#init-2)`. |
| `function` | [`get`](uv.md#get-2) `const` `inline` | Return the raw libuv handle pointer cast to `[Handle](uv.md#handle-2)`. |
| `function` | [`tid`](uv.md#tid) `const` `inline` | Return the ID of the thread that constructed this handle. |
| `function` | [`context`](uv.md#context-3) `const` `inline` | Return the raw `[Context](uv.md#context-1)` that owns the libuv handle memory. |
| `function` | [`setCloseCleanup`](uv.md#setclosecleanup) `inline` |  |
| `function` | [`clearCloseCleanup`](uv.md#clearclosecleanup) `inline` |  |
| `function` | [`assertThread`](uv.md#assertthread) `const` `inline` | Throw `std::logic_error` if called from any thread other than the thread that constructed this handle. |
| `variable` | [`_loop`](uv.md#_loop)  |  |
| `variable` | [`_context`](uv.md#_context)  |  |
| `variable` | [`_tid`](uv.md#_tid)  |  |
| `variable` | [`_error`](uv.md#_error-1)  |  |
| `function` | [`onError`](uv.md#onerror) `virtual` `inline` | Called by `[setError()](uv.md#seterror)` after the error state has been updated. |
| `function` | [`onClose`](uv.md#onclose) `virtual` `inline` | Called by `[close()](uv.md#close-11)` after the context has been released. |
| `function` | [`Handle`](uv.md#handle-4)  | NonCopyable and NonMovable. |
| `function` | [`Handle`](uv.md#handle-5)  | Deleted constructor. |
| `typedef` | [`Type`](uv.md#type-7)  | Define the native handle type. |

### Inherited from [`Socket`](#socket-1)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`opaque`](#opaque)  | Optional client data. |
| `function` | [`Socket`](#socket-2)  | Defaulted constructor. |
| `function` | [`Socket`](#socket-3)  | Deleted constructor. |
| `function` | [`Socket`](#socket-4)  | Deleted constructor. |
| `function` | [`connect`](#connect-2) `virtual` | Connects to the given peer IP address. |
| `function` | [`connect`](#connect-3) `virtual` | Resolves and connects to the given host address. |
| `function` | [`bind`](#bind) `virtual` | Bind a local address to the socket. The address may be IPv4 or IPv6 (if supported). |
| `function` | [`listen`](#listen) `virtual` `inline` | Listens the socket on the given address. |
| `function` | [`shutdown`](#shutdown-1) `virtual` `inline` `nodiscard` | Sends the shutdown packet which should result is socket closure via callback. |
| `function` | [`sendOwned`](#sendowned) `virtual` `nodiscard` | Sends an owned payload buffer to the connected peer. |
| `function` | [`sendOwned`](#sendowned-1) `virtual` `nodiscard` |  |
| `function` | [`close`](#close-12) `virtual` | Closes the underlying socket. |
| `function` | [`address`](#address-8) `virtual` `const` | The locally bound address. |
| `function` | [`peerAddress`](#peeraddress-1) `virtual` `const` | The connected peer address. |
| `function` | [`transport`](#transport) `virtual` `const` | The transport protocol: TCP, UDP or SSLTCP. |
| `function` | [`setError`](#seterror-1) `virtual` | Sets the socket error. |
| `function` | [`error`](#error-5) `virtual` `const` | Return the socket error if any. |
| `function` | [`closed`](#closed-1) `virtual` `const` | Returns true if the native socket handle is closed. |
| `function` | [`loop`](#loop-3) `virtual` `const` | Returns the socket event loop. |
| `variable` | [`_af`](#_af)  |  |
| `function` | [`init`](#init-4) `virtual` | Initializes the underlying socket context. |
| `function` | [`reset`](#reset-4) `virtual` | Resets the socket context for reuse. |
| `typedef` | [`Ptr`](#ptr-4)  |  |
| `typedef` | [`Vec`](#vec)  |  |

### Inherited from [`SocketAdapter`](#socketadapter)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`priority`](#priority-1)  | The priority of this adapter for STL sort operations. |
| `function` | [`SocketAdapter`](#socketadapter-1)  | Creates the [SocketAdapter](#socketadapter). |
| `function` | [`~SocketAdapter`](#socketadapter-2) `virtual` `noexcept` | Destroys the [SocketAdapter](#socketadapter). |
| `function` | [`send`](#send) `virtual` `nodiscard` | Sends the given data buffer to the connected peer. Returns the number of bytes sent or -1 on error. No exception will be thrown. For TCP sockets the given peer address must match the connected peer address. |
| `function` | [`send`](#send-1) `virtual` `nodiscard` |  |
| `function` | [`sendOwned`](#sendowned-2) `virtual` `nodiscard` | Sends an owned payload buffer to the connected peer. |
| `function` | [`sendOwned`](#sendowned-3) `virtual` `nodiscard` |  |
| `function` | [`sendPacket`](#sendpacket) `virtual` | Sends the given packet to the connected peer. Returns the number of bytes sent or -1 on error. No exception will be thrown. For TCP sockets the given peer address must match the connected peer address. |
| `function` | [`sendPacket`](#sendpacket-1) `virtual` |  |
| `function` | [`sendPacket`](#sendpacket-2) `virtual` | Sends the given packet to the connected peer. This method provides delegate compatibility, and unlike other send methods throws an exception if the underlying socket is closed. |
| `function` | [`setSender`](#setsender) `virtual` | Sets the pointer to the outgoing data adapter. Send methods proxy data to this adapter by default. |
| `function` | [`sender`](#sender)  | Returns the output [SocketAdapter](#socketadapter) pointer. |
| `function` | [`addReceiver`](#addreceiver) `virtual` | Sets the pointer to the incoming data adapter. Events proxy data to this adapter by default. |
| `function` | [`removeReceiver`](#removereceiver) `virtual` | Remove the given receiver. |
| `function` | [`hasReceiver`](#hasreceiver) `virtual` | Returns true if the given receiver is connected. |
| `function` | [`receivers`](#receivers)  | Returns all currently registered input [SocketAdapter](#socketadapter) pointers. Dead (removed) entries are excluded from the returned list. |
| `function` | [`onSocketConnect`](#onsocketconnect) `virtual` | Called when the socket establishes a connection. Forwards the event to all registered receivers in priority order. Override to intercept before the application sees the event. |
| `function` | [`onSocketRecv`](#onsocketrecv-1) `virtual` | Called when data is received from the socket. Forwards the event to all registered receivers in priority order. |
| `function` | [`onSocketError`](#onsocketerror) `virtual` | Called when the socket encounters an error. Forwards the event to all registered receivers in priority order. |
| `function` | [`onSocketClose`](#onsocketclose) `virtual` | Called when the socket is closed. Forwards the event to all registered receivers in priority order. |
| `variable` | [`_sender`](#_sender)  |  |
| `variable` | [`_receivers`](#_receivers)  |  |
| `variable` | [`_dirty`](#_dirty)  |  |
| `function` | [`cleanupReceivers`](#cleanupreceivers) `virtual` |  |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`UDPSocket`](#udpsocket-1)  | Constructs the [UDPSocket](#udpsocket) and initializes the underlying libuv handle. |
|  | [`UDPSocket`](#udpsocket-2)  | Deleted constructor. |
|  | [`UDPSocket`](#udpsocket-3)  | Deleted constructor. |
| `void` | [`connect`](#connect-9) `virtual` `override` | Records the peer address and fires the Connect signal to mimic TCP socket behaviour. UDP is connectionless; this call does not send any data. |
| `void` | [`connect`](#connect-10) `virtual` `override` | Resolves `host` via DNS (or maps "localhost"), then calls connect(Address). |
| `void` | [`close`](#close-16) `virtual` `override` | Stops receiving and closes the underlying UDP handle. |
| `void` | [`bind`](#bind-3) `virtual` `override` | Binds the socket to `address` and starts the receive loop. |
| `ssize_t` | [`send`](#send-7) `virtual` `nodiscard` `override` | Sends `len` bytes to the previously connected peer address. Returns -1 if no peer address has been set. |
| `ssize_t` | [`sendOwned`](#sendowned-8) `virtual` `nodiscard` `override` | Sends an owned payload buffer to the connected peer. |
| `ssize_t` | [`send`](#send-8) `virtual` `nodiscard` `override` | Sends `len` bytes to `peerAddress`. Returns -1 if the socket is uninitialized or the address is not authorized. |
| `ssize_t` | [`sendOwned`](#sendowned-9) `virtual` `nodiscard` `override` |  |
| `bool` | [`setBroadcast`](#setbroadcast) `nodiscard` | Enables or disables UDP broadcast. |
| `bool` | [`setMulticastLoop`](#setmulticastloop) `nodiscard` | Enables or disables IP multicast loopback. |
| `bool` | [`setMulticastTTL`](#setmulticastttl) `nodiscard` | Sets the IP multicast time-to-live (hop limit). |
| `net::Address` | [`address`](#address-10) `virtual` `const` `override` | Returns the locally bound address, or a wildcard address if unbound. |
| `net::Address` | [`peerAddress`](#peeraddress-4) `virtual` `const` `override` | Returns the connected peer address set by [connect()](#connect-9), or a wildcard address if unconnected. |
| `net::TransportType` | [`transport`](#transport-3) `virtual` `const` `override` | Returns the UDP transport protocol. |
| `void` | [`setError`](#seterror-3) `virtual` `override` | Sets the socket error and triggers close. |
| `const icy::Error &` | [`error`](#error-8) `virtual` `const` `override` | Returns the current socket error, if any. |
| `bool` | [`closed`](#closed-3) `virtual` `const` `override` | Returns true if the native socket handle is closed. |
| `uv::Loop *` | [`loop`](#loop-5) `virtual` `const` `override` | Returns the event loop associated with this socket. |
| `void` | [`onRecv`](#onrecv-1) `virtual` | Dispatches a received datagram to all socket adapters via onSocketRecv. |

---

{#udpsocket-1}

#### UDPSocket

```cpp
UDPSocket(uv::Loop * loop = uv::defaultLoop())
```

Defined in src/net/include/icy/net/udpsocket.h:37

Constructs the [UDPSocket](#udpsocket) and initializes the underlying libuv handle. 
#### Parameters
* `loop` Event loop to use; defaults to the default loop.

---

{#udpsocket-2}

#### UDPSocket

```cpp
UDPSocket(const UDPSocket &) = delete
```

Defined in src/net/include/icy/net/udpsocket.h:40

Deleted constructor.

---

{#udpsocket-3}

#### UDPSocket

```cpp
UDPSocket(UDPSocket &&) = delete
```

Defined in src/net/include/icy/net/udpsocket.h:42

Deleted constructor.

---

{#connect-9}

#### connect

`virtual` `override`

```cpp
virtual void connect(const net::Address & peerAddress) override
```

Defined in src/net/include/icy/net/udpsocket.h:48

Records the peer address and fires the Connect signal to mimic TCP socket behaviour. UDP is connectionless; this call does not send any data. 
#### Parameters
* `peerAddress` The remote address to associate with this socket.

##### Reimplements

- [`connect`](#connect-2)

---

{#connect-10}

#### connect

`virtual` `override`

```cpp
virtual void connect(std::string_view host, uint16_t port) override
```

Defined in src/net/include/icy/net/udpsocket.h:53

Resolves `host` via DNS (or maps "localhost"), then calls connect(Address). 
#### Parameters
* `host` Hostname or IP address string. 

* `port` Destination port.

##### Reimplements

- [`connect`](#connect-3)

---

{#close-16}

#### close

`virtual` `override`

```cpp
virtual void close() override
```

Defined in src/net/include/icy/net/udpsocket.h:56

Stops receiving and closes the underlying UDP handle.

##### Reimplements

- [`close`](uv.md#close-11)

---

{#bind-3}

#### bind

`virtual` `override`

```cpp
virtual void bind(const net::Address & address, unsigned flags = 0) override
```

Defined in src/net/include/icy/net/udpsocket.h:61

Binds the socket to `address` and starts the receive loop. 
#### Parameters
* `address` Local address to bind to. 

* `flags` Optional bind flags (UV_UDP_IPV6ONLY is added automatically for IPv6).

##### Reimplements

- [`bind`](#bind)

---

{#send-7}

#### send

`virtual` `nodiscard` `override`

```cpp
[[nodiscard]] virtual ssize_t send(const char * data, size_t len, int flags = 0) override
```

Defined in src/net/include/icy/net/udpsocket.h:69

Sends `len` bytes to the previously connected peer address. Returns -1 if no peer address has been set. 
#### Parameters
* `data` Pointer to the data to send. 

* `len` Number of bytes to send. 

* `flags` Reserved; currently unused. 

#### Returns
Number of bytes accepted for sending, or -1 on error.

##### Reimplements

- [`send`](#send)

---

{#sendowned-8}

#### sendOwned

`virtual` `nodiscard` `override`

```cpp
[[nodiscard]] virtual ssize_t sendOwned(Buffer && buffer, int flags = 0) override
```

Defined in src/net/include/icy/net/udpsocket.h:70

Sends an owned payload buffer to the connected peer.

##### Reimplements

- [`sendOwned`](#sendowned)

---

{#send-8}

#### send

`virtual` `nodiscard` `override`

```cpp
[[nodiscard]] virtual ssize_t send(const char * data, size_t len, const net::Address & peerAddress, int flags = 0) override
```

Defined in src/net/include/icy/net/udpsocket.h:79

Sends `len` bytes to `peerAddress`. Returns -1 if the socket is uninitialized or the address is not authorized. 
#### Parameters
* `data` Pointer to the data to send. 

* `len` Number of bytes to send. 

* `peerAddress` Destination address; must match the connected peer if one is set. 

* `flags` Reserved; currently unused. 

#### Returns
Number of bytes accepted for sending, or -1 on error.

##### Reimplements

- [`send`](#send-1)

---

{#sendowned-9}

#### sendOwned

`virtual` `nodiscard` `override`

```cpp
[[nodiscard]] virtual ssize_t sendOwned(Buffer && buffer, const net::Address & peerAddress, int flags = 0) override
```

Defined in src/net/include/icy/net/udpsocket.h:81

##### Reimplements

- [`sendOwned`](#sendowned-1)

---

{#setbroadcast}

#### setBroadcast

`nodiscard`

```cpp
[[nodiscard]] bool setBroadcast(bool flag)
```

Defined in src/net/include/icy/net/udpsocket.h:87

Enables or disables UDP broadcast. 
#### Parameters
* `flag` true to enable broadcast. 

#### Returns
true if the option was set successfully.

---

{#setmulticastloop}

#### setMulticastLoop

`nodiscard`

```cpp
[[nodiscard]] bool setMulticastLoop(bool flag)
```

Defined in src/net/include/icy/net/udpsocket.h:92

Enables or disables IP multicast loopback. 
#### Parameters
* `flag` true to enable multicast loopback. 

#### Returns
true if the option was set successfully.

---

{#setmulticastttl}

#### setMulticastTTL

`nodiscard`

```cpp
[[nodiscard]] bool setMulticastTTL(int ttl)
```

Defined in src/net/include/icy/net/udpsocket.h:98

Sets the IP multicast time-to-live (hop limit). 
#### Parameters
* `ttl` Value in the range [1, 255]. 

#### Returns
true if the option was set successfully. 

#### Exceptions
* `std::invalid_argument` if `ttl` is out of range.

---

{#address-10}

#### address

`virtual` `const` `override`

```cpp
virtual net::Address address() const override
```

Defined in src/net/include/icy/net/udpsocket.h:101

Returns the locally bound address, or a wildcard address if unbound.

##### Reimplements

- [`address`](#address-8)

---

{#peeraddress-4}

#### peerAddress

`virtual` `const` `override`

```cpp
virtual net::Address peerAddress() const override
```

Defined in src/net/include/icy/net/udpsocket.h:104

Returns the connected peer address set by [connect()](#connect-9), or a wildcard address if unconnected.

##### Reimplements

- [`peerAddress`](#peeraddress-1)

---

{#transport-3}

#### transport

`virtual` `const` `override`

```cpp
virtual net::TransportType transport() const override
```

Defined in src/net/include/icy/net/udpsocket.h:107

Returns the UDP transport protocol.

##### Reimplements

- [`transport`](#transport)

---

{#seterror-3}

#### setError

`virtual` `override`

```cpp
virtual void setError(const icy::Error & err) override
```

Defined in src/net/include/icy/net/udpsocket.h:111

Sets the socket error and triggers close. 
#### Parameters
* `err` The error to record.

##### Reimplements

- [`setError`](uv.md#seterror)

---

{#error-8}

#### error

`virtual` `const` `override`

```cpp
virtual const icy::Error & error() const override
```

Defined in src/net/include/icy/net/udpsocket.h:114

Returns the current socket error, if any.

##### Reimplements

- [`error`](#error-5)

---

{#closed-3}

#### closed

`virtual` `const` `override`

```cpp
virtual bool closed() const override
```

Defined in src/net/include/icy/net/udpsocket.h:117

Returns true if the native socket handle is closed.

##### Reimplements

- [`closed`](uv.md#closed)

---

{#loop-5}

#### loop

`virtual` `const` `override`

```cpp
virtual uv::Loop * loop() const override
```

Defined in src/net/include/icy/net/udpsocket.h:120

Returns the event loop associated with this socket.

##### Reimplements

- [`loop`](#loop-3)

---

{#onrecv-1}

#### onRecv

`virtual`

```cpp
virtual void onRecv(const MutableBuffer & buf, const net::Address & address)
```

Defined in src/net/include/icy/net/udpsocket.h:125

Dispatches a received datagram to all socket adapters via onSocketRecv. 
#### Parameters
* `buf` [Buffer](base.md#buffer-2) containing the received datagram payload. 

* `address` [Address](#address) of the sender.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `net::Address` | [`_peer`](#_peer)  |  |
| `Buffer` | [`_buffer`](#_buffer)  |  |

---

{#_peer}

#### _peer

```cpp
net::Address _peer
```

Defined in src/net/include/icy/net/udpsocket.h:141

---

{#_buffer}

#### _buffer

```cpp
Buffer _buffer
```

Defined in src/net/include/icy/net/udpsocket.h:142

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`init`](#init-6) `virtual` `override` | Initializes the underlying socket context. |
| `void` | [`reset`](#reset-6) `virtual` `override` | Resets the socket context for reuse. |
| `void` | [`onError`](#onerror-2) `virtual` `override` | Called by `[setError()](#seterror-3)` after the error state has been updated. |
| `void` | [`onClose`](#onclose-2) `virtual` `override` | Called by `[close()](#close-16)` after the context has been released. |
| `bool` | [`recvStart`](#recvstart) `virtual` |  |
| `bool` | [`recvStop`](#recvstop) `virtual` |  |

---

{#init-6}

#### init

`virtual` `override`

```cpp
virtual void init() override
```

Defined in src/net/include/icy/net/udpsocket.h:128

Initializes the underlying socket context.

##### Reimplements

- [`init`](#init-4)

---

{#reset-6}

#### reset

`virtual` `override`

```cpp
virtual void reset() override
```

Defined in src/net/include/icy/net/udpsocket.h:129

Resets the socket context for reuse.

##### Reimplements

- [`reset`](#reset-4)

---

{#onerror-2}

#### onError

`virtual` `override`

```cpp
virtual void onError(const icy::Error & error) override
```

Defined in src/net/include/icy/net/udpsocket.h:131

Called by `[setError()](#seterror-3)` after the error state has been updated.

Override to react to errors. The default implementation is a no-op.

#### Parameters
* `error` The error that was set.

##### Reimplements

- [`onError`](uv.md#onerror)

---

{#onclose-2}

#### onClose

`virtual` `override`

```cpp
virtual void onClose() override
```

Defined in src/net/include/icy/net/udpsocket.h:132

Called by `[close()](#close-16)` after the context has been released.

Override to perform cleanup on handle closure. The default implementation is a no-op.

##### Reimplements

- [`onClose`](uv.md#onclose)

---

{#recvstart}

#### recvStart

`virtual`

```cpp
virtual bool recvStart()
```

Defined in src/net/include/icy/net/udpsocket.h:134

---

{#recvstop}

#### recvStop

`virtual`

```cpp
virtual bool recvStop()
```

Defined in src/net/include/icy/net/udpsocket.h:135

### Public Types

| Name | Description |
|------|-------------|
| [`Ptr`](#ptr-10)  |  |
| [`Vec`](#vec-3)  |  |

---

{#ptr-10}

#### Ptr

```cpp
using Ptr = std::shared_ptr< UDPSocket >
```

Defined in src/net/include/icy/net/udpsocket.h:32

---

{#vec-3}

#### Vec

```cpp
using Vec = std::vector< Ptr >
```

Defined in src/net/include/icy/net/udpsocket.h:33

{#verificationerrordetails}

## VerificationErrorDetails

```cpp
#include <icy/net/sslmanager.h>
```

```cpp
class VerificationErrorDetails
```

Defined in src/net/include/icy/net/sslmanager.h:134

A utility class for certificate error handling.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`VerificationErrorDetails`](#verificationerrordetails-1) | `function` | Declared here |
| [`~VerificationErrorDetails`](#verificationerrordetails-2) | `function` | Declared here |
| [`certificate`](#certificate) | `function` | Declared here |
| [`errorDepth`](#errordepth) | `function` | Declared here |
| [`errorNumber`](#errornumber) | `function` | Declared here |
| [`errorMessage`](#errormessage) | `function` | Declared here |
| [`setIgnoreError`](#setignoreerror) | `function` | Declared here |
| [`getIgnoreError`](#getignoreerror) | `function` | Declared here |
| [`_cert`](#_cert) | `variable` | Declared here |
| [`_errorDepth`](#_errordepth) | `variable` | Declared here |
| [`_errorNumber`](#_errornumber) | `variable` | Declared here |
| [`_errorMessage`](#_errormessage) | `variable` | Declared here |
| [`_ignoreError`](#_ignoreerror) | `variable` | Declared here |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`VerificationErrorDetails`](#verificationerrordetails-1)  | Creates the [VerificationErrorDetails](#verificationerrordetails). _ignoreError is per default set to false. |
|  | [`~VerificationErrorDetails`](#verificationerrordetails-2) `noexcept` | Destroys the [VerificationErrorDetails](#verificationerrordetails). |
| `const crypto::X509Certificate &` | [`certificate`](#certificate) `const` `inline` | Returns the certificate that caused the error. |
| `int` | [`errorDepth`](#errordepth) `const` `inline` | Returns the position of the certificate in the certificate chain. |
| `int` | [`errorNumber`](#errornumber) `const` `inline` | Returns the id of the error. |
| `const std::string &` | [`errorMessage`](#errormessage) `const` `inline` | Returns the textual presentation of the errorNumber. |
| `void` | [`setIgnoreError`](#setignoreerror) `inline` | setIgnoreError to true, if a verification error is judged non-fatal by the user. |
| `bool` | [`getIgnoreError`](#getignoreerror) `const` `inline` | returns the value of _ignoreError |

---

{#verificationerrordetails-1}

#### VerificationErrorDetails

```cpp
VerificationErrorDetails(const crypto::X509Certificate & cert, int errDepth, int errNum, const std::string & errMsg)
```

Defined in src/net/include/icy/net/sslmanager.h:139

Creates the [VerificationErrorDetails](#verificationerrordetails). _ignoreError is per default set to false.

---

{#verificationerrordetails-2}

#### ~VerificationErrorDetails

`noexcept`

```cpp
~VerificationErrorDetails() noexcept
```

Defined in src/net/include/icy/net/sslmanager.h:143

Destroys the [VerificationErrorDetails](#verificationerrordetails).

---

{#certificate}

#### certificate

`const` `inline`

```cpp
inline const crypto::X509Certificate & certificate() const
```

Defined in src/net/include/icy/net/sslmanager.h:146

Returns the certificate that caused the error.

---

{#errordepth}

#### errorDepth

`const` `inline`

```cpp
inline int errorDepth() const
```

Defined in src/net/include/icy/net/sslmanager.h:149

Returns the position of the certificate in the certificate chain.

---

{#errornumber}

#### errorNumber

`const` `inline`

```cpp
inline int errorNumber() const
```

Defined in src/net/include/icy/net/sslmanager.h:152

Returns the id of the error.

---

{#errormessage}

#### errorMessage

`const` `inline`

```cpp
inline const std::string & errorMessage() const
```

Defined in src/net/include/icy/net/sslmanager.h:155

Returns the textual presentation of the errorNumber.

---

{#setignoreerror}

#### setIgnoreError

`inline`

```cpp
inline void setIgnoreError(bool ignoreError)
```

Defined in src/net/include/icy/net/sslmanager.h:159

setIgnoreError to true, if a verification error is judged non-fatal by the user.

---

{#getignoreerror}

#### getIgnoreError

`const` `inline`

```cpp
inline bool getIgnoreError() const
```

Defined in src/net/include/icy/net/sslmanager.h:162

returns the value of _ignoreError

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `crypto::X509Certificate` | [`_cert`](#_cert)  |  |
| `int` | [`_errorDepth`](#_errordepth)  |  |
| `int` | [`_errorNumber`](#_errornumber)  |  |
| `std::string` | [`_errorMessage`](#_errormessage)  |  |
| `bool` | [`_ignoreError`](#_ignoreerror)  |  |

---

{#_cert}

#### _cert

```cpp
crypto::X509Certificate _cert
```

Defined in src/net/include/icy/net/sslmanager.h:165

---

{#_errordepth}

#### _errorDepth

```cpp
int _errorDepth
```

Defined in src/net/include/icy/net/sslmanager.h:166

---

{#_errornumber}

#### _errorNumber

```cpp
int _errorNumber
```

Defined in src/net/include/icy/net/sslmanager.h:167

---

{#_errormessage}

#### _errorMessage

```cpp
std::string _errorMessage
```

Defined in src/net/include/icy/net/sslmanager.h:168

---

{#_ignoreerror}

#### _ignoreError

```cpp
bool _ignoreError
```

Defined in src/net/include/icy/net/sslmanager.h:169

{#packetinfo}

## PacketInfo

```cpp
#include <icy/net/socket.h>
```

```cpp
struct PacketInfo
```

Defined in src/net/include/icy/net/socket.h:155

> **Inherits:** [`IPacketInfo`](base.md#ipacketinfo)

Provides information about packets emitted from a socket. See [SocketPacket](#socketpacket).

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`socket`](#socket) | `variable` | Declared here |
| [`peerAddress`](#peeraddress) | `variable` | Declared here |
| [`PacketInfo`](#packetinfo-1) | `function` | Declared here |
| [`PacketInfo`](#packetinfo-2) | `function` | Declared here |
| [`clone`](#clone-5) | `function` | Declared here |
| [`IPacketInfo`](base.md#structicy_1_1IPacketInfo_1acc94711631f475526138b8ead9859289) | `function` | Inherited from [`IPacketInfo`](base.md#ipacketinfo) |
| [`~IPacketInfo`](base.md#structicy_1_1IPacketInfo_1a9ca2452f67ad2bad882fd6644427cb3d) | `function` | Inherited from [`IPacketInfo`](base.md#ipacketinfo) |
| [`clone`](base.md#structicy_1_1IPacketInfo_1a77aed2ef78b6b18eaeb652bf9583f2ce) | `function` | Inherited from [`IPacketInfo`](base.md#ipacketinfo) |

### Inherited from [`IPacketInfo`](base.md#ipacketinfo)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`IPacketInfo`](base.md#structicy_1_1IPacketInfo_1acc94711631f475526138b8ead9859289)  | Defaulted constructor. |
| `function` | [`~IPacketInfo`](base.md#structicy_1_1IPacketInfo_1a9ca2452f67ad2bad882fd6644427cb3d) `virtual` | Defaulted destructor. |
| `function` | [`clone`](base.md#structicy_1_1IPacketInfo_1a77aed2ef78b6b18eaeb652bf9583f2ce) `virtual` `const` | Returns a heap-allocated deep copy of this info object. |

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `Socket::Ptr` | [`socket`](#socket)  | The source socket. |
| `Address` | [`peerAddress`](#peeraddress)  | The originating peer address. For TCP this will always be connected address. |

---

{#socket}

#### socket

```cpp
Socket::Ptr socket
```

Defined in src/net/include/icy/net/socket.h:158

The source socket.

---

{#peeraddress}

#### peerAddress

```cpp
Address peerAddress
```

Defined in src/net/include/icy/net/socket.h:162

The originating peer address. For TCP this will always be connected address.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`PacketInfo`](#packetinfo-1) `inline` | Constructs [PacketInfo](#packetinfo) with the originating socket and peer address. |
|  | [`PacketInfo`](#packetinfo-2) `inline` | Copy constructor. |
| `std::unique_ptr< IPacketInfo >` | [`clone`](#clone-5) `virtual` `const` `inline` | Returns a heap-allocated copy of this [PacketInfo](#packetinfo). |

---

{#packetinfo-1}

#### PacketInfo

`inline`

```cpp
inline PacketInfo(const Socket::Ptr & socket, const Address & peerAddress)
```

Defined in src/net/include/icy/net/socket.h:167

Constructs [PacketInfo](#packetinfo) with the originating socket and peer address. 
#### Parameters
* `socket` Shared pointer to the socket that received the packet. 

* `peerAddress` [Address](#address) of the remote peer that sent the packet.

---

{#packetinfo-2}

#### PacketInfo

`inline`

```cpp
inline PacketInfo(const PacketInfo & r)
```

Defined in src/net/include/icy/net/socket.h:175

Copy constructor. 
#### Parameters
* `r` Source [PacketInfo](#packetinfo) to copy from.

---

{#clone-5}

#### clone

`virtual` `const` `inline`

```cpp
virtual inline std::unique_ptr< IPacketInfo > clone() const
```

Defined in src/net/include/icy/net/socket.h:182

Returns a heap-allocated copy of this [PacketInfo](#packetinfo).

##### Reimplements

- [`clone`](base.md#structicy_1_1IPacketInfo_1a77aed2ef78b6b18eaeb652bf9583f2ce)

{#dns}

# dns

DNS utilities.

### Functions

| Return | Name | Description |
|--------|------|-------------|
| `auto` | [`resolve`](#resolve-1) `inline` | Resolves a hostname to a network address asynchronously. |

---

{#resolve-1}

#### resolve

`inline`

```cpp
inline auto resolve(const std::string & host, int port, std::function< void(int, const net::Address &)> callback, uv::Loop * loop = uv::defaultLoop())
```

Resolves a hostname to a network address asynchronously.

The callback is invoked on the event loop thread when resolution completes. On failure, the callback receives a non-zero status and an empty [Address](#address). 
#### Parameters
* `host` Hostname or IP address string to resolve. 

* `port` Port number to associate with the resolved address. 

* `callback` Invoked with (status, resolved [Address](#address)); status is 0 on success. 

* `loop` Event loop to use; defaults to the default loop. 

#### Returns
A request handle whose lifetime controls the in-flight DNS query.

