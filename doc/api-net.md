# Module <!-- group --> `net`



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`namespace `[`scy::net`](#namespacescy_1_1net)    | 
# namespace `scy::net` {#namespacescy_1_1net}



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[`scy::net::Address`](#classscy_1_1net_1_1Address)    | 
`class `[`scy::net::AddressBase`](#classscy_1_1net_1_1AddressBase)    | 
`class `[`scy::net::IPv4AddressBase`](#classscy_1_1net_1_1IPv4AddressBase)    | 
`class `[`scy::net::PacketSocketAdapter`](#classscy_1_1net_1_1PacketSocketAdapter)    | 
`class `[`scy::net::Socket`](#classscy_1_1net_1_1Socket)    | 
`class `[`scy::net::SocketAdapter`](#classscy_1_1net_1_1SocketAdapter)    | 
`class `[`scy::net::SocketPacket`](#classscy_1_1net_1_1SocketPacket)    | 
`class `[`scy::net::SSLAdapter`](#classscy_1_1net_1_1SSLAdapter)    | 
`class `[`scy::net::SSLContext`](#classscy_1_1net_1_1SSLContext)    | 
`class `[`scy::net::SSLManager`](#classscy_1_1net_1_1SSLManager)    | 
`class `[`scy::net::SSLSession`](#classscy_1_1net_1_1SSLSession)    | 
`class `[`scy::net::SSLSocket`](#classscy_1_1net_1_1SSLSocket)    | 
`class `[`scy::net::TCPSocket`](#classscy_1_1net_1_1TCPSocket)    | 
`class `[`scy::net::Transaction`](#classscy_1_1net_1_1Transaction)    | 
`class `[`scy::net::UDPSocket`](#classscy_1_1net_1_1UDPSocket)    | 
`class `[`scy::net::VerificationErrorDetails`](#classscy_1_1net_1_1VerificationErrorDetails)    | A utility class for certificate error handling.
`struct `[`scy::net::DNSResult`](#structscy_1_1net_1_1DNSResult)    | 
`struct `[`scy::net::PacketInfo`](#structscy_1_1net_1_1PacketInfo)    | 
# class `scy::net::Address` {#classscy_1_1net_1_1Address}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Address()` | Creates a wildcard (all zero) IPv4 [Address](./doc/api-net.md#classscy_1_1net_1_1Address).
`public  Address(const std::string & host,std::uint16_t port)` | 
`public  Address(const `[`Address`](#classscy_1_1net_1_1Address)` & addr)` | Creates a [Address](./doc/api-net.md#classscy_1_1net_1_1Address) by copying another one.
`public  Address(const struct sockaddr * addr,socklen_t length)` | Creates a [Address](./doc/api-net.md#classscy_1_1net_1_1Address) from a native socket address.
`public  Address(const std::string & host,const std::string & port)` | 
`public  explicit Address(const std::string & hostAndPort)` | 
`public  ~Address()` | Destroys the [Address](./doc/api-net.md#classscy_1_1net_1_1Address).
`public `[`Address`](#classscy_1_1net_1_1Address)` & operator=(const `[`Address`](#classscy_1_1net_1_1Address)` & addr)` | Assigns another [Address](./doc/api-net.md#classscy_1_1net_1_1Address).
`public void swap(`[`Address`](#classscy_1_1net_1_1Address)` & addr)` | Swaps the [Address](./doc/api-net.md#classscy_1_1net_1_1Address) with another one.
`public std::string host() const` | Returns the host IP address.
`public std::uint16_t port() const` | Returns the port number.
`public socklen_t length() const` | Returns the length of the internal native socket address.
`public const struct sockaddr * addr() const` | Returns a pointer to the internal native socket address.
`public int af() const` | Returns the address family (AF_INET or AF_INET6) of the address.
`public std::string toString() const` | Returns a string representation of the address.
`public `[`Address::Family`](#group__net_1gaf91e77d155f2852974bbae5a2da34106)` family() const` | Returns the address family of the host's address.
`public bool valid() const` | 
`public bool operator<(const `[`Address`](#classscy_1_1net_1_1Address)` & addr) const` | 
`public bool operator==(const `[`Address`](#classscy_1_1net_1_1Address)` & addr) const` | 
`public bool operator!=(const `[`Address`](#classscy_1_1net_1_1Address)` & addr) const` | 
`protected void init(const std::string & host,std::uint16_t port)` | 

## Members

#### `public  Address()` {#group__net_1ga4376b5c4c7a0390b9148741c61331775}

Creates a wildcard (all zero) IPv4 [Address](#classscy_1_1net_1_1Address).



#### `public  Address(const std::string & host,std::uint16_t port)` {#group__net_1ga0357bf485934763f45de8149110f18f0}



Creates a [Address](#classscy_1_1net_1_1Address) from an IP address and a port number.

The IP address must either be a domain name, or it must be in dotted decimal (IPv4) or hex string (IPv6) format.

#### `public  Address(const `[`Address`](#classscy_1_1net_1_1Address)` & addr)` {#group__net_1ga1ebca1dc763be19d20ec69be97bbb3d7}

Creates a [Address](#classscy_1_1net_1_1Address) by copying another one.



#### `public  Address(const struct sockaddr * addr,socklen_t length)` {#group__net_1gac57cc836572c64291fe739745be8e25e}

Creates a [Address](#classscy_1_1net_1_1Address) from a native socket address.



#### `public  Address(const std::string & host,const std::string & port)` {#group__net_1gacde53f645ce818600709e240dbf510eb}



Creates a [Address](#classscy_1_1net_1_1Address) from an IP address and a service name or port number.

The IP address must either be a domain name, or it must be in dotted decimal (IPv4) or hex string (IPv6) format.

The given port must either be a decimal port number, or a service name.

#### `public  explicit Address(const std::string & hostAndPort)` {#group__net_1ga94e2bed4c55e369c97a2d229d2b591e6}



Creates a [Address](#classscy_1_1net_1_1Address) from an IP address or host name and a port number/service name. Host name/address and port number must be separated by a colon. In case of an IPv6 address, the address part must be enclosed in brackets.

Examples: 192.168.1.10:80

www.sourcey.com:8080

#### `public  ~Address()` {#group__net_1ga5b9e05ed6187c478f65d0d57ad4cdb35}

Destroys the [Address](#classscy_1_1net_1_1Address).



#### `public `[`Address`](#classscy_1_1net_1_1Address)` & operator=(const `[`Address`](#classscy_1_1net_1_1Address)` & addr)` {#group__net_1ga3b3ec108da40cf5fafd15cc21b4388bd}

Assigns another [Address](#classscy_1_1net_1_1Address).



#### `public void swap(`[`Address`](#classscy_1_1net_1_1Address)` & addr)` {#group__net_1ga877434e8e2b5a266d99f9f35be0882b0}

Swaps the [Address](#classscy_1_1net_1_1Address) with another one.



#### `public std::string host() const` {#group__net_1gad8fbd6353a1f03ef3e0346ef1be64800}

Returns the host IP address.



#### `public std::uint16_t port() const` {#group__net_1gaeb8da43e00e4da9ec8ed14b90a5b7e08}

Returns the port number.



#### `public socklen_t length() const` {#group__net_1ga474b01eea594bde98f3dca27ca6d09bd}

Returns the length of the internal native socket address.



#### `public const struct sockaddr * addr() const` {#group__net_1ga6ea740a58361a1e81ed6319d6529dcf4}

Returns a pointer to the internal native socket address.



#### `public int af() const` {#group__net_1gac1036c47a13a55df8cae09cf4ca82137}

Returns the address family (AF_INET or AF_INET6) of the address.



#### `public std::string toString() const` {#group__net_1gacc785d126d40fabb5af3e6f71c835084}

Returns a string representation of the address.



#### `public `[`Address::Family`](#group__net_1gaf91e77d155f2852974bbae5a2da34106)` family() const` {#group__net_1ga7683b700d696248f25f2a125a1e0f645}

Returns the address family of the host's address.



#### `public bool valid() const` {#group__net_1ga657136484ebe9fedce2ce01cb5c4497e}



Returns true when the port is set and the address is valid ie. not wildcard.

#### `public bool operator<(const `[`Address`](#classscy_1_1net_1_1Address)` & addr) const` {#group__net_1ga7a1cc2425e33d40740dd5d706de9ebd6}





#### `public bool operator==(const `[`Address`](#classscy_1_1net_1_1Address)` & addr) const` {#group__net_1gaa0f9e31b04a54f297c6b970ad8c6719f}





#### `public bool operator!=(const `[`Address`](#classscy_1_1net_1_1Address)` & addr) const` {#group__net_1gadf21121f417c9ef8ea3ebd441e6ee72e}





#### `protected void init(const std::string & host,std::uint16_t port)` {#group__net_1ga2ebe7684e4d8b21d137845bcd2a6cbad}





# class `scy::net::AddressBase` {#classscy_1_1net_1_1AddressBase}

```
class scy::net::AddressBase
  : public scy::SharedObject
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::string host() const` | 
`public std::uint16_t port() const` | 
`public `[`Address::Family`](#group__net_1gaf91e77d155f2852974bbae5a2da34106)` family() const` | 
`public socklen_t length() const` | 
`public const struct sockaddr * addr() const` | 
`public int af() const` | 
`protected inline  AddressBase()` | 
`protected inline virtual  ~AddressBase()` | 

## Members

#### `public std::string host() const` {#group__net_1ga867db5d11fa5c964830368ac1a4cbb7a}





#### `public std::uint16_t port() const` {#group__net_1gafc004855d7b74eb5bf4a6f3f6c7aa377}





#### `public `[`Address::Family`](#group__net_1gaf91e77d155f2852974bbae5a2da34106)` family() const` {#group__net_1ga018e56177bc5c8bf2b02bc88ec6efcfe}





#### `public socklen_t length() const` {#group__net_1ga9d32a44cd01e1e7e7e082f5c36aee5e1}





#### `public const struct sockaddr * addr() const` {#group__net_1gaf0ba03bb4f2288a5f3ae3c4a41dcf4ef}





#### `public int af() const` {#group__net_1ga69583a3ceab6d1839acb20d972d7fa7f}





#### `protected inline  AddressBase()` {#group__net_1ga0c4217e82452f7e70750ba1662802a67}





#### `protected inline virtual  ~AddressBase()` {#group__net_1gaee163e8131fa554005928e0941c8bf93}





# class `scy::net::IPv4AddressBase` {#classscy_1_1net_1_1IPv4AddressBase}

```
class scy::net::IPv4AddressBase
  : public scy::net::AddressBase
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  IPv4AddressBase()` | 
`public inline  IPv4AddressBase(const struct sockaddr_in * addr)` | 
`public inline  IPv4AddressBase(const void * addr,std::uint16_t port)` | 
`public inline virtual std::string host() const` | 
`public inline virtual std::uint16_t port() const` | 
`public inline virtual `[`Address::Family`](#group__net_1gaf91e77d155f2852974bbae5a2da34106)` family() const` | 
`public inline virtual socklen_t length() const` | 
`public inline virtual const struct sockaddr * addr() const` | 
`public inline virtual int af() const` | 

## Members

#### `public inline  IPv4AddressBase()` {#group__net_1ga48fc92d2cb95f0b16b6c28b77c7d04d9}





#### `public inline  IPv4AddressBase(const struct sockaddr_in * addr)` {#group__net_1ga9130e321cd339b3b9414929e1e6ff6d5}





#### `public inline  IPv4AddressBase(const void * addr,std::uint16_t port)` {#group__net_1ga9b375eae213526642eb776faad776a98}





#### `public inline virtual std::string host() const` {#group__net_1ga2d455659c2dd8150f39122a4b70c2459}





#### `public inline virtual std::uint16_t port() const` {#group__net_1ga665102389bfb8bb67224e80ee1d35c5c}





#### `public inline virtual `[`Address::Family`](#group__net_1gaf91e77d155f2852974bbae5a2da34106)` family() const` {#group__net_1ga8200f0c126c892638c7f0818b786a0c6}





#### `public inline virtual socklen_t length() const` {#group__net_1gae374547c5843e238cfdfaee8cdd82037}





#### `public inline virtual const struct sockaddr * addr() const` {#group__net_1ga08477510392162beb42e773960a6e139}





#### `public inline virtual int af() const` {#group__net_1ga3468fd918b7a508ed76fb12baf0491e4}





# class `scy::net::PacketSocketAdapter` {#classscy_1_1net_1_1PacketSocketAdapter}

```
class scy::net::PacketSocketAdapter
  : public scy::net::SocketAdapter
  : public scy::Signal< void(IPacket &)>
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public Socket::Ptr socket` | 
`public `[`PacketFactory`](./doc/api-base.md#classscy_1_1PacketFactory)` factory` | 
`public  PacketSocketAdapter(const Socket::Ptr & socket)` | 
`public virtual void onSocketRecv(`[`Socket`](#classscy_1_1net_1_1Socket)` & socket,const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
`public virtual void onPacket(`[`IPacket`](#classscy_1_1IPacket)` & pkt)` | 

## Members

#### `public Socket::Ptr socket` {#group__net_1ga9a80638bbd778f5b92b425fc0f659ba1}



Pointer to the underlying socket. Sent data will be proxied to this socket.

#### `public `[`PacketFactory`](./doc/api-base.md#classscy_1_1PacketFactory)` factory` {#group__net_1ga0b65ad8c24a9b68dd027ea2a33e83e2c}





#### `public  PacketSocketAdapter(const Socket::Ptr & socket)` {#group__net_1gaaffa3af9e2fb97f6401d52c0c920c2ce}



Creates the [PacketSocketAdapter](#classscy_1_1net_1_1PacketSocketAdapter) This class should have a higher priority than standard sockets so we can parse data packets first. Creates and dispatches a packet utilizing the available creation strategies. For best performance the most used strategies should have the highest priority.

#### `public virtual void onSocketRecv(`[`Socket`](#classscy_1_1net_1_1Socket)` & socket,const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`Address`](#classscy_1_1net_1_1Address)` & peerAddress)` {#group__net_1gadcd2cc1b5b293f84c573a3930cba63e6}





#### `public virtual void onPacket(`[`IPacket`](#classscy_1_1IPacket)` & pkt)` {#group__net_1ga615b06087799683f041ceb81f2813cdc}





# class `scy::net::Socket` {#classscy_1_1net_1_1Socket}

```
class scy::net::Socket
  : public scy::net::SocketAdapter
```  



[Socket](#classscy_1_1net_1_1Socket) is the base socket implementation from which all sockets derive.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Socket()` | 
`public virtual  ~Socket()` | 
`public void connect(const `[`Address`](#classscy_1_1net_1_1Address)` & address)` | 
`public virtual void connect(const std::string & host,std::uint16_t port)` | 
`public void bind(const `[`Address`](#classscy_1_1net_1_1Address)` & address,unsigned flags)` | 
`public inline virtual void listen(int backlog)` | 
`public inline virtual bool shutdown()` | 
`public void close()` | Closes the underlying socket.
`public `[`Address`](#classscy_1_1net_1_1Address)` address() const` | 
`public `[`Address`](#classscy_1_1net_1_1Address)` peerAddress() const` | 
`public net::TransportType transport() const` | The transport protocol: TCP, UDP or SSLTCP.
`public void setError(const `[`scy::Error`](#structscy_1_1Error)` & err)` | 
`public const `[`scy::Error`](#structscy_1_1Error)` & error() const` | Return the socket error if any.
`public bool closed() const` | Returns true if the native socket handle is closed.
`public uv::Loop * loop() const` | Returns the socket event loop.
`protected void init()` | Initializes the underlying socket context.
`protected inline virtual void reset()` | Resets the socket context for reuse.
`protected inline virtual void * self()` | 

## Members

#### `public  Socket()` {#group__net_1ga1e0540d7424900711dcfab2307d7c0bd}





#### `public virtual  ~Socket()` {#group__net_1ga2472af5142c47b8c6f030195262f6fe5}





#### `public void connect(const `[`Address`](#classscy_1_1net_1_1Address)` & address)` {#group__net_1ga26b4c04fb14fb521c3777966f5500814}



Connects to the given peer IP address.

Throws an exception if the address is malformed. Connection errors can be handled via the [Error](#structscy_1_1Error) signal.

#### `public virtual void connect(const std::string & host,std::uint16_t port)` {#group__net_1ga8a498220b9d7233892bc1e7f72433ed7}



Resolves and connects to the given host address.

Throws an Exception if the host is malformed. Since the DNS callback is asynchronous implementations need to listen for the [Error](#structscy_1_1Error) signal for handling connection errors.

#### `public void bind(const `[`Address`](#classscy_1_1net_1_1Address)` & address,unsigned flags)` {#group__net_1gad12fbab04fa447ab0104325d7062af03}



Bind a local address to the socket. The address may be IPv4 or IPv6 (if supported).

Throws an Exception on error.

#### `public inline virtual void listen(int backlog)` {#group__net_1ga3f113f66f6dcbb1fc2a2c25a86833eaa}



Listens the socket on the given address.

Throws an Exception on error.

#### `public inline virtual bool shutdown()` {#group__net_1ga68cca230e61d94ddcb530c8236b8b65a}



Sends the shutdown packet which should result is socket closure via callback.

#### `public void close()` {#group__net_1gaef3e9d763d40129224c70087c0b8660b}

Closes the underlying socket.



#### `public `[`Address`](#classscy_1_1net_1_1Address)` address() const` {#group__net_1gadb57bf71ea7475434e304193495a22f8}



The locally bound address.

This function will not throw. A Wildcard 0.0.0.0:0 address is returned if the socket is closed or invalid.

#### `public `[`Address`](#classscy_1_1net_1_1Address)` peerAddress() const` {#group__net_1gaa7aad8f6cdfc70fd2d993cc99e65f73c}



The connected peer address.

This function will not throw. A Wildcard 0.0.0.0:0 address is returned if the socket is closed or invalid.

#### `public net::TransportType transport() const` {#group__net_1ga1e1b0fbbf92bb3dd962783a8c0e94b3d}

The transport protocol: TCP, UDP or SSLTCP.



#### `public void setError(const `[`scy::Error`](#structscy_1_1Error)` & err)` {#group__net_1gaed1d2e8ca6eb5140528c044b675299b4}



Sets the socket error.

Setting the error will result in socket closure.

#### `public const `[`scy::Error`](#structscy_1_1Error)` & error() const` {#group__net_1ga6274cf1a3301f98601e781640e1247e0}

Return the socket error if any.



#### `public bool closed() const` {#group__net_1ga9bf8cf75403f7f833c37ed7fb16443a6}

Returns true if the native socket handle is closed.



#### `public uv::Loop * loop() const` {#group__net_1ga90e13e19c06edf614cb8dccb95a5e2ce}

Returns the socket event loop.



#### `protected void init()` {#group__net_1ga0b9b76689ad2ca6a398e909a5424c24b}

Initializes the underlying socket context.



#### `protected inline virtual void reset()` {#group__net_1ga684d297c0a9e10aff046e86261eb812f}

Resets the socket context for reuse.



#### `protected inline virtual void * self()` {#group__net_1gabd6d3aaefccf39f41d427c1e6ffb90d5}



Returns the derived instance pointer for casting [SocketAdapter](#classscy_1_1net_1_1SocketAdapter) signal callback sender arguments from void* to [Socket](#classscy_1_1net_1_1Socket). Note: This method must not be derived by subclasses or casting will fail for void* pointer callbacks.

# class `scy::net::SocketAdapter` {#classscy_1_1net_1_1SocketAdapter}




[SocketAdapter](#classscy_1_1net_1_1SocketAdapter) is the abstract interface for all socket classes. A [SocketAdapter](#classscy_1_1net_1_1SocketAdapter) can also be attached to a [Socket](#classscy_1_1net_1_1Socket) in order to override default [Socket](#classscy_1_1net_1_1Socket) callbacks and behaviour, while still maintaining the default [Socket](#classscy_1_1net_1_1Socket) interface (see Socket::setAdapter).

This class also be extended to implement custom processing for received socket data before it is dispatched to the application (see [PacketSocketAdapter](#classscy_1_1net_1_1PacketSocketAdapter) and [Transaction](#classscy_1_1net_1_1Transaction) classes).

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public void * opaque` | 
`public `[`Signal](#classscy_1_1Signal)< void([Socket`](#classscy_1_1net_1_1Socket) &)`> Connect` | Signals that the socket is connected.
`public `[`Signal](#classscy_1_1Signal)< void([Socket](#classscy_1_1net_1_1Socket) &, const [MutableBuffer](#classscy_1_1MutableBuffer) &, const [Address`](#classscy_1_1net_1_1Address) &)`> Recv` | Signals when data is received by the socket.
`public `[`Signal](#classscy_1_1Signal)< void([Socket](#classscy_1_1net_1_1Socket) &, const [scy::Error`](#structscy_1_1Error) &)`> Error` | 
`public `[`Signal](#classscy_1_1Signal)< void([Socket`](#classscy_1_1net_1_1Socket) &)`> Close` | Signals that the underlying socket is closed.
`public  SocketAdapter(`[`SocketAdapter`](#classscy_1_1net_1_1SocketAdapter)` * sender,`[`SocketAdapter`](#classscy_1_1net_1_1SocketAdapter)` * receiver)` | Creates the [SocketAdapter](./doc/api-net.md#classscy_1_1net_1_1SocketAdapter).
`public virtual  ~SocketAdapter()` | Destroys the [SocketAdapter](./doc/api-net.md#classscy_1_1net_1_1SocketAdapter).
`public virtual int send(const char * data,std::size_t len,int flags)` | 
`public virtual int send(const char * data,std::size_t len,const `[`Address`](#classscy_1_1net_1_1Address)` & peerAddress,int flags)` | 
`public virtual int sendPacket(const `[`IPacket`](#classscy_1_1IPacket)` & packet,int flags)` | 
`public virtual int sendPacket(const `[`IPacket`](#classscy_1_1IPacket)` & packet,const `[`Address`](#classscy_1_1net_1_1Address)` & peerAddress,int flags)` | 
`public virtual void sendPacket(`[`IPacket`](#classscy_1_1IPacket)` & packet)` | 
`public virtual void onSocketConnect(`[`Socket`](#classscy_1_1net_1_1Socket)` & socket)` | 
`public virtual void onSocketRecv(`[`Socket`](#classscy_1_1net_1_1Socket)` & socket,const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
`public virtual void onSocketError(`[`Socket`](#classscy_1_1net_1_1Socket)` & socket,const `[`Error`](#structscy_1_1Error)` & error)` | 
`public virtual void onSocketClose(`[`Socket`](#classscy_1_1net_1_1Socket)` & socket)` | 
`public void setSender(`[`SocketAdapter`](#classscy_1_1net_1_1SocketAdapter)` * adapter,bool freeExisting)` | 
`public `[`SocketAdapter`](#classscy_1_1net_1_1SocketAdapter)` * sender()` | Returns the output [SocketAdapter](./doc/api-net.md#classscy_1_1net_1_1SocketAdapter) pointer.
`public void addReceiver(`[`SocketAdapter`](#classscy_1_1net_1_1SocketAdapter)` * adapter,int priority)` | Adds an input [SocketAdapter](./doc/api-net.md#classscy_1_1net_1_1SocketAdapter) for receiving socket callbacks.
`public void removeReceiver(`[`SocketAdapter`](#classscy_1_1net_1_1SocketAdapter)` * adapter)` | Removes an input [SocketAdapter](./doc/api-net.md#classscy_1_1net_1_1SocketAdapter).
`protected `[`SocketAdapter`](./doc/api-net.md#classscy_1_1net_1_1SocketAdapter)` * _sender` | 
`protected inline virtual void * self()` | 
`protected virtual void emitSocketConnect(`[`Socket`](#classscy_1_1net_1_1Socket)` & socket)` | 
`protected virtual void emitSocketRecv(`[`Socket`](#classscy_1_1net_1_1Socket)` & socket,const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
`protected virtual void emitSocketError(`[`Socket`](#classscy_1_1net_1_1Socket)` & socket,const `[`scy::Error`](#structscy_1_1Error)` & error)` | 
`protected virtual void emitSocketClose(`[`Socket`](#classscy_1_1net_1_1Socket)` & socket)` | 

## Members

#### `public void * opaque` {#group__net_1gac9eea748326d1dcdc7f770d4e073856b}



Optional client data pointer.

The pointer is not initialized or managed by the socket base.

#### `public `[`Signal](#classscy_1_1Signal)< void([Socket`](#classscy_1_1net_1_1Socket) &)`> Connect` {#group__net_1gaf17d4734454424856c0b633bd9541088}

Signals that the socket is connected.



#### `public `[`Signal](#classscy_1_1Signal)< void([Socket](#classscy_1_1net_1_1Socket) &, const [MutableBuffer](#classscy_1_1MutableBuffer) &, const [Address`](#classscy_1_1net_1_1Address) &)`> Recv` {#group__net_1ga595ba3e14f06eed1f02f015a728c2d70}

Signals when data is received by the socket.



#### `public `[`Signal](#classscy_1_1Signal)< void([Socket](#classscy_1_1net_1_1Socket) &, const [scy::Error`](#structscy_1_1Error) &)`> Error` {#group__net_1gaf32bb1d5e97abc31fd973d49d6c04839}



Signals that the socket is closed in error. This signal will be sent just before the Closed signal.

#### `public `[`Signal](#classscy_1_1Signal)< void([Socket`](#classscy_1_1net_1_1Socket) &)`> Close` {#group__net_1gaf96db2932c4615e6498cc9feabb881fa}

Signals that the underlying socket is closed.



#### `public  SocketAdapter(`[`SocketAdapter`](#classscy_1_1net_1_1SocketAdapter)` * sender,`[`SocketAdapter`](#classscy_1_1net_1_1SocketAdapter)` * receiver)` {#group__net_1gaf9ced47f3c1aa08ccea3042997a2a320}

Creates the [SocketAdapter](#classscy_1_1net_1_1SocketAdapter).



#### `public virtual  ~SocketAdapter()` {#group__net_1gac06480990b5301cb185000fe8bf860e4}

Destroys the [SocketAdapter](#classscy_1_1net_1_1SocketAdapter).



#### `public virtual int send(const char * data,std::size_t len,int flags)` {#group__net_1ga6579b7708a46ad3789ac4c18dab6bff5}



Sends the given data buffer to the connected peer. Returns the number of bytes sent or -1 on error. No exception will be thrown. For TCP sockets the given peer address must match the connected peer address.

#### `public virtual int send(const char * data,std::size_t len,const `[`Address`](#classscy_1_1net_1_1Address)` & peerAddress,int flags)` {#group__net_1ga251a5c1729b84c4af927c42de671c1b8}





#### `public virtual int sendPacket(const `[`IPacket`](#classscy_1_1IPacket)` & packet,int flags)` {#group__net_1gaf684d330e51316add03edd55eed5860f}



Sends the given packet to the connected peer. Returns the number of bytes sent or -1 on error. No exception will be thrown. For TCP sockets the given peer address must match the connected peer address.

#### `public virtual int sendPacket(const `[`IPacket`](#classscy_1_1IPacket)` & packet,const `[`Address`](#classscy_1_1net_1_1Address)` & peerAddress,int flags)` {#group__net_1gaee7df0b9e237d6ab59c25711c3e1fb2a}





#### `public virtual void sendPacket(`[`IPacket`](#classscy_1_1IPacket)` & packet)` {#group__net_1ga6d7c711236a3570235353f634f9d1a77}



Sends the given packet to the connected peer. This method provides delegate compatability, and unlike other send methods throws an exception if the underlying socket is closed.

#### `public virtual void onSocketConnect(`[`Socket`](#classscy_1_1net_1_1Socket)` & socket)` {#group__net_1ga8dae1c179274b0571d1b5d8040f5afd8}



These virtual methods can be overridden as necessary to intercept socket events before they hit the application.

#### `public virtual void onSocketRecv(`[`Socket`](#classscy_1_1net_1_1Socket)` & socket,const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`Address`](#classscy_1_1net_1_1Address)` & peerAddress)` {#group__net_1ga48f8129547ba9eaac88094e059205529}





#### `public virtual void onSocketError(`[`Socket`](#classscy_1_1net_1_1Socket)` & socket,const `[`Error`](#structscy_1_1Error)` & error)` {#group__net_1ga5658d5b01a3ff4cbad6010190aefe3b7}





#### `public virtual void onSocketClose(`[`Socket`](#classscy_1_1net_1_1Socket)` & socket)` {#group__net_1gab5d5c13b628ba6a010785f447f9e4fdc}





#### `public void setSender(`[`SocketAdapter`](#classscy_1_1net_1_1SocketAdapter)` * adapter,bool freeExisting)` {#group__net_1gad39114f43b6983614b35c4a6363dba91}



A pointer to the adapter for handling outgoing data. Send methods proxy data to this adapter by default. Note that we only keep a simple pointer so as to avoid circular references preventing destruction.

#### `public `[`SocketAdapter`](#classscy_1_1net_1_1SocketAdapter)` * sender()` {#group__net_1ga036fe4e08fb6ef41c86613b801a84534}

Returns the output [SocketAdapter](#classscy_1_1net_1_1SocketAdapter) pointer.



#### `public void addReceiver(`[`SocketAdapter`](#classscy_1_1net_1_1SocketAdapter)` * adapter,int priority)` {#group__net_1ga551f61f940ced5938b585cefdd716c35}

Adds an input [SocketAdapter](#classscy_1_1net_1_1SocketAdapter) for receiving socket callbacks.



#### `public void removeReceiver(`[`SocketAdapter`](#classscy_1_1net_1_1SocketAdapter)` * adapter)` {#group__net_1ga523f50c94ac42d49d81efb8a4ff81629}

Removes an input [SocketAdapter](#classscy_1_1net_1_1SocketAdapter).



#### `protected `[`SocketAdapter`](./doc/api-net.md#classscy_1_1net_1_1SocketAdapter)` * _sender` {#group__net_1ga17dc4823b85f24d94b5127681006439f}





#### `protected inline virtual void * self()` {#group__net_1ga79c950fc80f816fe63a4e0fb055e9cf5}



Returns the polymorphic instance pointer for signal delegate callbacks.

#### `protected virtual void emitSocketConnect(`[`Socket`](#classscy_1_1net_1_1Socket)` & socket)` {#group__net_1ga966260cfba01b3ca969b832f31261dfc}





#### `protected virtual void emitSocketRecv(`[`Socket`](#classscy_1_1net_1_1Socket)` & socket,const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`Address`](#classscy_1_1net_1_1Address)` & peerAddress)` {#group__net_1gac4a4fe72a1b8485102de481b2544c7ef}





#### `protected virtual void emitSocketError(`[`Socket`](#classscy_1_1net_1_1Socket)` & socket,const `[`scy::Error`](#structscy_1_1Error)` & error)` {#group__net_1gaac26c63c41c7f302df628faf706f4233}





#### `protected virtual void emitSocketClose(`[`Socket`](#classscy_1_1net_1_1Socket)` & socket)` {#group__net_1gaa0adb99c95f2c8f5255f43739d46845c}





# class `scy::net::SocketPacket` {#classscy_1_1net_1_1SocketPacket}

```
class scy::net::SocketPacket
  : public scy::RawPacket
```  



[SocketPacket](#classscy_1_1net_1_1SocketPacket) is the default packet type emitted by sockets. [SocketPacket](#classscy_1_1net_1_1SocketPacket) provides peer address information and a buffer reference for nocopy binary operations.

The referenced packet buffer lifetime is only guaranteed for the duration of the receiver callback.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`PacketInfo`](./doc/api-net.md#structscy_1_1net_1_1PacketInfo)` * info` | [PacketInfo](./doc/api-net.md#structscy_1_1net_1_1PacketInfo) pointer.
`public inline  SocketPacket(const Socket::Ptr & socket,const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
`public inline  SocketPacket(const `[`SocketPacket`](#classscy_1_1net_1_1SocketPacket)` & that)` | 
`public inline virtual  ~SocketPacket()` | 
`public inline virtual void print(std::ostream & os) const` | 
`public inline virtual `[`IPacket`](#classscy_1_1IPacket)` * clone() const` | 
`public inline virtual std::size_t read(const `[`ConstBuffer`](#classscy_1_1ConstBuffer)` &)` | 
`public inline virtual void write(Buffer &) const` | 
`public inline virtual const char * className() const` | 

## Members

#### `public `[`PacketInfo`](./doc/api-net.md#structscy_1_1net_1_1PacketInfo)` * info` {#group__net_1ga80fa02b6b62e44c85ba7f4bf954cea78}

[PacketInfo](#structscy_1_1net_1_1PacketInfo) pointer.



#### `public inline  SocketPacket(const Socket::Ptr & socket,const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`Address`](#classscy_1_1net_1_1Address)` & peerAddress)` {#group__net_1gaec154054d8c54269655aa062d8fe1ffd}





#### `public inline  SocketPacket(const `[`SocketPacket`](#classscy_1_1net_1_1SocketPacket)` & that)` {#group__net_1gad327456520e77574cd3cb730ccc0a75a}





#### `public inline virtual  ~SocketPacket()` {#group__net_1ga3e883a20f382b845e90c1047e8ff03f1}





#### `public inline virtual void print(std::ostream & os) const` {#group__net_1ga319694029ed98f048cdaf68330e31807}





#### `public inline virtual `[`IPacket`](#classscy_1_1IPacket)` * clone() const` {#group__net_1ga9c40a9cbceb66a7c41285246b66a19d7}





#### `public inline virtual std::size_t read(const `[`ConstBuffer`](#classscy_1_1ConstBuffer)` &)` {#group__net_1gafe8c962183c6a1543ff6b13d88416d21}



Read/parse to the packet from the given input buffer. The number of bytes read is returned.

#### `public inline virtual void write(Buffer &) const` {#group__net_1gaac943003d23e0dbbedbf120a45ab3078}



Copy/generate to the packet given output buffer. The number of bytes written can be obtained from the buffer.

Todo: It may be prefferable to use our pod types here instead of buffer input, but the current codebase requires that the buffer be dynamically resizable for some protocols...

virtual std::size_t write(MutableBuffer&) const = 0;

#### `public inline virtual const char * className() const` {#group__net_1ga63397cc5b6177b8f5f23f5335191e39a}





# class `scy::net::SSLAdapter` {#classscy_1_1net_1_1SSLAdapter}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  SSLAdapter(`[`net::SSLSocket`](#classscy_1_1net_1_1SSLSocket)` * socket)` | 
`public  ~SSLAdapter()` | 
`public void initClient()` | Initializes the SSL context as a client.
`public void initServer()` | Initializes the SSL context as a server.
`public bool initialized() const` | Returns true when SSL context has been initialized.
`public bool ready() const` | Returns true when the handshake is complete.
`public void handshake()` | Start/continue the SSL handshake process.
`public int available() const` | 
`public void shutdown()` | Issues an orderly SSL shutdown.
`public void flush()` | Flushes the SSL read/write buffers.
`public void addIncomingData(const char * data,std::size_t len)` | 
`public void addOutgoingData(const std::string & data)` | 
`public void addOutgoingData(const char * data,std::size_t len)` | 
`protected `[`net::SSLSocket`](./doc/api-net.md#classscy_1_1net_1_1SSLSocket)` * _socket` | 
`protected SSL * _ssl` | 
`protected BIO * _readBIO` | The incoming buffer we write encrypted SSL data into.
`protected BIO * _writeBIO` | The outgoing buffer we write to the socket.
`protected std::vector< char > _bufferOut` | The outgoing payload to be encrypted and sent.
`protected void handleError(int rc)` | 
`protected void flushReadBIO()` | 
`protected void flushWriteBIO()` | 

## Members

#### `public  SSLAdapter(`[`net::SSLSocket`](#classscy_1_1net_1_1SSLSocket)` * socket)` {#group__net_1ga26cc38aebf1d0d1846ae6603ba2be64f}





#### `public  ~SSLAdapter()` {#group__net_1ga4639bd49bcad7899337f22ba0ec68084}





#### `public void initClient()` {#group__net_1gaebf891ed98b4c4f2c5edeccfd98cef03}

Initializes the SSL context as a client.



#### `public void initServer()` {#group__net_1ga707dd2a10f4541d9a38205083a60c08f}

Initializes the SSL context as a server.



#### `public bool initialized() const` {#group__net_1ga3051dc8e9a424368aeb91027d4b9d8be}

Returns true when SSL context has been initialized.



#### `public bool ready() const` {#group__net_1ga05e325d251acd31625116021c4abcf4a}

Returns true when the handshake is complete.



#### `public void handshake()` {#group__net_1ga16c72dfccd9cc787ca4d570179de7bb9}

Start/continue the SSL handshake process.



#### `public int available() const` {#group__net_1gaebf583d1844bb5eb37b6c7d94a7cdca1}



Returns the number of bytes available in the SSL buffer for immediate reading.

#### `public void shutdown()` {#group__net_1gaa2a689ba83da2bcc9d3b8b4b3bab303e}

Issues an orderly SSL shutdown.



#### `public void flush()` {#group__net_1gabf462204b841bac2edbc2d9c12d36d59}

Flushes the SSL read/write buffers.



#### `public void addIncomingData(const char * data,std::size_t len)` {#group__net_1gaf4ea86c962f0db54c50937c72e877e75}





#### `public void addOutgoingData(const std::string & data)` {#group__net_1ga80a755a8a5a1369ace2d8f95740767f7}





#### `public void addOutgoingData(const char * data,std::size_t len)` {#group__net_1ga1701e3f52e0f2de8eb99766c593f9768}





#### `protected `[`net::SSLSocket`](./doc/api-net.md#classscy_1_1net_1_1SSLSocket)` * _socket` {#group__net_1gae207495c52da9720625af1c1a8234ca0}





#### `protected SSL * _ssl` {#group__net_1gab767ae04b5ac979a1e2d0d337b4c4028}





#### `protected BIO * _readBIO` {#group__net_1ga0a542283ee6c57fcc584a1315cd0b9a7}

The incoming buffer we write encrypted SSL data into.



#### `protected BIO * _writeBIO` {#group__net_1ga7837fb89f6f46ab90a408290f308ee2a}

The outgoing buffer we write to the socket.



#### `protected std::vector< char > _bufferOut` {#group__net_1ga3c2e88ec5aa5631dedaf5179bf8dbfa0}

The outgoing payload to be encrypted and sent.



#### `protected void handleError(int rc)` {#group__net_1ga37281519a34b937cbf763e587e5675c4}





#### `protected void flushReadBIO()` {#group__net_1ga8c32288c3e5badc7985dcc0f58ac8336}





#### `protected void flushWriteBIO()` {#group__net_1ga5949d6d6339de68d4d38f1b4b07e3360}





# class `scy::net::SSLContext` {#classscy_1_1net_1_1SSLContext}

```
class scy::net::SSLContext
  : public scy::SharedObject
```  



This class encapsulates context information for an SSL server or client, such as the certificate verification mode and the location of certificates and private key files, as well as the list of supported ciphers.

The Context class is also used to control SSL session caching on the server and client side.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  SSLContext(`[`Usage`](#group__net_1ga2e13fff7d4c75c080e515a427bf15c71)` usage,const std::string & privateKeyFile,const std::string & certificateFile,const std::string & caLocation,`[`VerificationMode`](#group__net_1gafd7da63f7077511ce8d4da68f37d255e)` verificationMode,int verificationDepth,bool loadDefaultCAs,const std::string & cipherList)` | 
`public  SSLContext(`[`Usage`](#group__net_1ga2e13fff7d4c75c080e515a427bf15c71)` usage,const std::string & caLocation,`[`VerificationMode`](#group__net_1gafd7da63f7077511ce8d4da68f37d255e)` verificationMode,int verificationDepth,bool loadDefaultCAs,const std::string & cipherList)` | 
`public  ~SSLContext()` | Destroys the Context.
`public void useCertificate(const `[`crypto::X509Certificate`](#classscy_1_1crypto_1_1X509Certificate)` & certificate)` | 
`public void addChainCertificate(const `[`crypto::X509Certificate`](#classscy_1_1crypto_1_1X509Certificate)` & certificate)` | Adds a certificate for certificate chain validation.
`public void addVerificationCertificate(const `[`crypto::X509Certificate`](#classscy_1_1crypto_1_1X509Certificate)` & certificate)` | 
`public inline SSL_CTX * sslContext() const` | Returns the underlying OpenSSL SSL Context object.
`public inline `[`Usage`](#group__net_1ga2e13fff7d4c75c080e515a427bf15c71)` usage() const` | 
`public inline bool isForServerUse() const` | Returns true if the context is for use by a server.
`public inline `[`SSLContext::VerificationMode`](#group__net_1gafd7da63f7077511ce8d4da68f37d255e)` verificationMode() const` | Returns the verification mode.
`public void enableSessionCache(bool flag)` | 
`public void enableSessionCache(bool flag,const std::string & sessionIdContext)` | 
`public bool sessionCacheEnabled() const` | Returns true if the session cache is enabled.
`public void setSessionCacheSize(std::size_t size)` | 
`public std::size_t getSessionCacheSize() const` | 
`public void setSessionTimeout(long seconds)` | 
`public long getSessionTimeout() const` | 
`public void flushSessionCache()` | 
`public void disableStatelessSessionResumption()` | 

## Members

#### `public  SSLContext(`[`Usage`](#group__net_1ga2e13fff7d4c75c080e515a427bf15c71)` usage,const std::string & privateKeyFile,const std::string & certificateFile,const std::string & caLocation,`[`VerificationMode`](#group__net_1gafd7da63f7077511ce8d4da68f37d255e)` verificationMode,int verificationDepth,bool loadDefaultCAs,const std::string & cipherList)` {#group__net_1ga9bc5bae0a271799c171b37d3e6e13735}



Creates a Context.



* usage specifies whether the context is used by a client or server.


* privateKeyFile contains the path to the private key file used for encryption. Can be empty if no private key file is used.


* certificateFile contains the path to the certificate file (in PEM format). If the private key and the certificate are stored in the same file, this can be empty if privateKeyFile is given.


* caLocation contains the path to the file or directory containing the CA/root certificates. Can be empty if the OpenSSL builtin CA certificates are used (see loadDefaultCAs).


* verificationMode specifies whether and how peer certificates are validated.


* verificationDepth sets the upper limit for verification chain sizes. Verification will fail if a certificate chain larger than this is encountered.


* loadDefaultCAs specifies wheter the builtin CA certificates from OpenSSL are used.


* cipherList specifies the supported ciphers in OpenSSL notation.





Note: If the private key is protected by a passphrase, a PrivateKeyPassphraseHandler must have been setup with the [SSLManager](#classscy_1_1net_1_1SSLManager), or the [SSLManager](#classscy_1_1net_1_1SSLManager)'s PrivateKeyPassphraseRequired event must be handled.

#### `public  SSLContext(`[`Usage`](#group__net_1ga2e13fff7d4c75c080e515a427bf15c71)` usage,const std::string & caLocation,`[`VerificationMode`](#group__net_1gafd7da63f7077511ce8d4da68f37d255e)` verificationMode,int verificationDepth,bool loadDefaultCAs,const std::string & cipherList)` {#group__net_1ga9fe3fe5f358b95a71a49a42798ec36ce}



Creates a Context.



* usage specifies whether the context is used by a client or server.


* caLocation contains the path to the file or directory containing the CA/root certificates. Can be empty if the OpenSSL builtin CA certificates are used (see loadDefaultCAs).


* verificationMode specifies whether and how peer certificates are validated.


* verificationDepth sets the upper limit for verification chain sizes. Verification will fail if a certificate chain larger than this is encountered.


* loadDefaultCAs specifies weather the builtin CA certificates from OpenSSL are used.


* cipherList specifies the supported ciphers in OpenSSL notation.





Note that a private key and/or certificate must be specified with usePrivateKey()/useCertificate() before the Context can be used.

#### `public  ~SSLContext()` {#group__net_1gaf88baeb14fd0ee8d539e960f81142043}

Destroys the Context.



#### `public void useCertificate(const `[`crypto::X509Certificate`](#classscy_1_1crypto_1_1X509Certificate)` & certificate)` {#group__net_1ga840711ee078f8ee6432aaef8b50b48b9}



Sets the certificate to be used by the Context.

To set-up a complete certificate chain, it might be necessary to call [addChainCertificate()](#group__net_1gac4fe7dff44de813dcdf7daf80ea48d5f) to specify additional certificates.

Note that [useCertificate()](#group__net_1ga840711ee078f8ee6432aaef8b50b48b9) must always be called before usePrivateKey().

#### `public void addChainCertificate(const `[`crypto::X509Certificate`](#classscy_1_1crypto_1_1X509Certificate)` & certificate)` {#group__net_1gac4fe7dff44de813dcdf7daf80ea48d5f}

Adds a certificate for certificate chain validation.



#### `public void addVerificationCertificate(const `[`crypto::X509Certificate`](#classscy_1_1crypto_1_1X509Certificate)` & certificate)` {#group__net_1ga553376bb332952ef5c1829fb3600d554}



Sets the private key to be used by the Context.

Note that [useCertificate()](#group__net_1ga840711ee078f8ee6432aaef8b50b48b9) must always be called before usePrivateKey().

Note: If the private key is protected by a passphrase, a PrivateKeyPassphraseHandler must have been setup with the [SSLManager](#classscy_1_1net_1_1SSLManager), or the [SSLManager](#classscy_1_1net_1_1SSLManager)'s PrivateKeyPassphraseRequired event must be handled. Adds the given certificate to the list of trusted certificates that will be used for verification.

#### `public inline SSL_CTX * sslContext() const` {#group__net_1ga88c101c4f8f5b4fe3ff3a4e50ff7ccfd}

Returns the underlying OpenSSL SSL Context object.



#### `public inline `[`Usage`](#group__net_1ga2e13fff7d4c75c080e515a427bf15c71)` usage() const` {#group__net_1ga9b4562b3b554ae7b584a1cb2ab3825df}



Returns whether the context is for use by a client or by a server and whether TLSv1 is required.

#### `public inline bool isForServerUse() const` {#group__net_1ga5b018180ca35ce83c8d5efa9a33b3d35}

Returns true if the context is for use by a server.



#### `public inline `[`SSLContext::VerificationMode`](#group__net_1gafd7da63f7077511ce8d4da68f37d255e)` verificationMode() const` {#group__net_1ga134311a6874bd856a4ad24fb7605784f}

Returns the verification mode.



#### `public void enableSessionCache(bool flag)` {#group__net_1gabcb6479878cbc88959c819b4315d9d5b}



Enable or disable SSL/TLS session caching. For session caching to work, it must be enabled on the server, as well as on the client side.

The default is disabled session caching.

To enable session caching on the server side, use the two-argument version of this method to specify a session ID context.

#### `public void enableSessionCache(bool flag,const std::string & sessionIdContext)` {#group__net_1gafc6d7cc2fd93fb8e85f793e00b5d2926}



Enables or disables SSL/TLS session caching on the server. For session caching to work, it must be enabled on the server, as well as on the client side.

SessionIdContext contains the application's unique session ID context, which becomes part of each session identifier generated by the server within this context. SessionIdContext can be an arbitrary sequence of bytes with a maximum length of SSL_MAX_SSL_SESSION_ID_LENGTH.

A non-empty sessionIdContext should be specified even if session caching is disabled to avoid problems with clients requesting to reuse a session (e.g. Firefox 3.6).

This method may only be called on SERVER_USE Context objects.

#### `public bool sessionCacheEnabled() const` {#group__net_1ga660b36fa2986687c39716fbf170d5284}

Returns true if the session cache is enabled.



#### `public void setSessionCacheSize(std::size_t size)` {#group__net_1ga1dd19a0bb29d9bba51c2affbd606fbfd}



Sets the maximum size of the server session cache, in number of sessions. The default size (according to OpenSSL documentation) is 1024*20, which may be too large for many applications, especially on embedded platforms with limited memory.

Specifying a size of 0 will set an unlimited cache size.

This method may only be called on SERVER_USE Context objects.

#### `public std::size_t getSessionCacheSize() const` {#group__net_1ga7335dc5176f0a333918513fcbc0d08ba}



Returns the current maximum size of the server session cache.

This method may only be called on SERVER_USE Context objects.

#### `public void setSessionTimeout(long seconds)` {#group__net_1ga4f769feef9c5e604a5f66121e3618e14}



Sets the timeout (in seconds) of cached sessions on the server. A cached session will be removed from the cache if it has not been used for the given number of seconds.

This method may only be called on SERVER_USE Context objects.

#### `public long getSessionTimeout() const` {#group__net_1ga00d5531a1264b6dbb8079779ba864eca}



Returns the timeout (in seconds) of cached sessions on the server.

This method may only be called on SERVER_USE Context objects.

#### `public void flushSessionCache()` {#group__net_1ga9d5e8a57a06cdaa54a8c5782785848c8}



Flushes the SSL session cache on the server.

This method may only be called on SERVER_USE Context objects.

#### `public void disableStatelessSessionResumption()` {#group__net_1gae16a7eff1253b2924e5254066e151c6b}



Newer versions of OpenSSL support RFC 4507 tickets for stateless session resumption.

The feature can be disabled by calling this method.

# class `scy::net::SSLManager` {#classscy_1_1net_1_1SSLManager}




[SSLManager](#classscy_1_1net_1_1SSLManager) is a singleton for holding the default server/client Context and handling callbacks for certificate verification errors and private key passphrases.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`Signal](#classscy_1_1Signal)< void([VerificationErrorDetails`](#classscy_1_1net_1_1VerificationErrorDetails) &)`> ServerVerificationError` | 
`public `[`Signal](#classscy_1_1Signal)< void([VerificationErrorDetails`](#classscy_1_1net_1_1VerificationErrorDetails) &)`> ClientVerificationError` | 
`public `[`Signal`](#classscy_1_1Signal)< void(std::string &)`> PrivateKeyPassphraseRequired` | 
`public void initializeServer(SSLContext::Ptr ptrContext)` | Initializes the server side of the [SSLManager](#classscy_1_1net_1_1SSLManager) server-side [SSLContext](#classscy_1_1net_1_1SSLContext).
`public void initializeClient(SSLContext::Ptr ptrContext)` | 
`public SSLContext::Ptr defaultServerContext()` | Returns the default Context used by the server if initialized.
`public SSLContext::Ptr defaultClientContext()` | Returns the default Context used by the client if initialized.
`public void shutdown()` | 

## Members

#### `public `[`Signal](#classscy_1_1Signal)< void([VerificationErrorDetails`](#classscy_1_1net_1_1VerificationErrorDetails) &)`> ServerVerificationError` {#group__net_1ga5c8d1a58ad63cfe7fd8c6ef8da2bc7bc}



Fired whenever a certificate verification error is detected by the server during a handshake.

#### `public `[`Signal](#classscy_1_1Signal)< void([VerificationErrorDetails`](#classscy_1_1net_1_1VerificationErrorDetails) &)`> ClientVerificationError` {#group__net_1ga79cecc2be9b8293b79f9dc92b3ab967d}



Fired whenever a certificate verification error is detected by the client during a handshake.

#### `public `[`Signal`](#classscy_1_1Signal)< void(std::string &)`> PrivateKeyPassphraseRequired` {#group__net_1gac92a4d1a788ac605e000055f0c2d1f0b}



Fired when a encrypted certificate is loaded. Not setting the password in the event parameter will result in a failure to load the certificate.

#### `public void initializeServer(SSLContext::Ptr ptrContext)` {#group__net_1ga5476ccf3e346e493c3d053a21f3f2b29}

Initializes the server side of the [SSLManager](#classscy_1_1net_1_1SSLManager) server-side [SSLContext](#classscy_1_1net_1_1SSLContext).



#### `public void initializeClient(SSLContext::Ptr ptrContext)` {#group__net_1ga4472c80b8b9152947333589515621051}



Initializes the client side of the [SSLManager](#classscy_1_1net_1_1SSLManager) with a default client-side [SSLContext](#classscy_1_1net_1_1SSLContext).

#### `public SSLContext::Ptr defaultServerContext()` {#group__net_1ga5888698b042c4c4a16d69fc98cb0e582}

Returns the default Context used by the server if initialized.



#### `public SSLContext::Ptr defaultClientContext()` {#group__net_1gaf06768dcfa955ebdd5b9c3977682e6ea}

Returns the default Context used by the client if initialized.



#### `public void shutdown()` {#group__net_1ga4061d56eaf906bf79a0713310a3841c5}



Shuts down the [SSLManager](#classscy_1_1net_1_1SSLManager) and releases the default Context objects. After a call to [shutdown()](#group__net_1ga4061d56eaf906bf79a0713310a3841c5), the [SSLManager](#classscy_1_1net_1_1SSLManager) can no longer be used.

Normally, it's not necessary to call this method directly, as this will be called either by uninitializeSSL(), or when the [SSLManager](#classscy_1_1net_1_1SSLManager) instance is destroyed.

# class `scy::net::SSLSession` {#classscy_1_1net_1_1SSLSession}

```
class scy::net::SSLSession
  : public scy::SharedObject
```  



This class encapsulates a SSL session object used with session caching on the client side.

For session caching to work, a client must save the session object from an existing connection, if it wants to reuse it with a future connection.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public SSL_SESSION * sslSession() const` | Returns the stored OpenSSL SSL_SESSION object.
`public  SSLSession(SSL_SESSION * ptr)` | 
`public  ~SSLSession()` | 
`public  SSLSession()` | 
`protected SSL_SESSION * _ptr` | 

## Members

#### `public SSL_SESSION * sslSession() const` {#group__net_1ga6581d3ef3c9d2cb11707fcd0f1848b7e}

Returns the stored OpenSSL SSL_SESSION object.



#### `public  SSLSession(SSL_SESSION * ptr)` {#group__net_1ga432cb2d0fa33e99be752e2f67b05b24e}



Creates a new Session object, using the given SSL_SESSION object.

The SSL_SESSION's reference count is not changed.

#### `public  ~SSLSession()` {#group__net_1ga46203b77ac08b39a29c45bd4cb616f96}



Destroys the Session.

Calls SSL_SESSION_free() on the stored SSL_SESSION object.

#### `public  SSLSession()` {#group__net_1ga9e95112e1b50fd581979c940f7024655}





#### `protected SSL_SESSION * _ptr` {#group__net_1ga304e950ce3ef764e602a67033539de62}





# class `scy::net::SSLSocket` {#classscy_1_1net_1_1SSLSocket}

```
class scy::net::SSLSocket
  : public scy::net::TCPSocket
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  SSLSocket(uv::Loop * loop)` | 
`public  SSLSocket(SSLContext::Ptr sslContext,uv::Loop * loop)` | 
`public  SSLSocket(SSLContext::Ptr sslContext,SSLSession::Ptr session,uv::Loop * loop)` | 
`public virtual  ~SSLSocket()` | 
`public virtual bool shutdown()` | Initialize the [SSLSocket](#classscy_1_1net_1_1SSLSocket) with the given [SSLContext](#classscy_1_1net_1_1SSLContext).
`public virtual void close()` | Closes the socket forcefully.
`public virtual int send(const char * data,std::size_t len,int flags)` | 
`public virtual int send(const char * data,std::size_t len,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress,int flags)` | 
`public void useContext(SSLContext::Ptr context)` | Use the given SSL context for this socket.
`public SSLContext::Ptr context() const` | Returns the SSL context used for this socket.
`public void useSession(SSLSession::Ptr session)` | 
`public SSLSession::Ptr currentSession()` | 
`public bool sessionWasReused()` | 
`public int available() const` | 
`public X509 * peerCertificate() const` | Returns the peer's certificate.
`public virtual net::TransportType transport() const` | The transport protocol: TCP, UDP or SSLTCP.
`public virtual void acceptConnection()` | 
`public virtual void onConnect(uv_connect_t * handle,int status)` | 
`public virtual void onRead(const char * data,std::size_t len)` | Reads raw encrypted SSL data.
`protected net::SSLContext::Ptr _context` | virtual bool readStart();
`protected net::SSLSession::Ptr _session` | 
`protected `[`net::SSLAdapter`](./doc/api-net.md#classscy_1_1net_1_1SSLAdapter)` _sslAdapter` | 

## Members

#### `public  SSLSocket(uv::Loop * loop)` {#group__net_1ga621415db5b8ff309cec65201efc086bf}





#### `public  SSLSocket(SSLContext::Ptr sslContext,uv::Loop * loop)` {#group__net_1ga6ef56420a681a95cd42e066a842b1b63}





#### `public  SSLSocket(SSLContext::Ptr sslContext,SSLSession::Ptr session,uv::Loop * loop)` {#group__net_1ga0d27d1f822710a64478dffc4fad17f98}





#### `public virtual  ~SSLSocket()` {#group__net_1ga55a2e9ca93a9fcaddbf2bad40a57966b}





#### `public virtual bool shutdown()` {#group__net_1gaa97f188cc3c5dfdfc3611ce40563064a}

Initialize the [SSLSocket](#classscy_1_1net_1_1SSLSocket) with the given [SSLContext](#classscy_1_1net_1_1SSLContext).

Initializes the socket and establishes a secure connection to the TCP server at the given address.

The SSL handshake is performed when the socket is connected. Shuts down the connection by attempting an orderly SSL shutdown, then actually shutting down the TCP connection.

#### `public virtual void close()` {#group__net_1gaf6b9bef4bb08bf461d12f958dbde58db}

Closes the socket forcefully.



#### `public virtual int send(const char * data,std::size_t len,int flags)` {#group__net_1gac1be9e7ce350ee81628197c23e999e35}



Sends the given data buffer to the connected peer. Returns the number of bytes sent or -1 on error. No exception will be thrown. For TCP sockets the given peer address must match the connected peer address.

#### `public virtual int send(const char * data,std::size_t len,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress,int flags)` {#group__net_1ga2dd72032beceb23eea67c5c032b30741}





#### `public void useContext(SSLContext::Ptr context)` {#group__net_1ga25cdc10ce54de6edcf57b5612788e23d}

Use the given SSL context for this socket.



#### `public SSLContext::Ptr context() const` {#group__net_1gacd6970a43ee59539f56d29cc503f78ab}

Returns the SSL context used for this socket.



#### `public void useSession(SSLSession::Ptr session)` {#group__net_1ga73a2c547fb422f580065091121dcba3c}



Sets the SSL session to use for the next connection. Setting a previously saved Session object is necessary to enable session caching.

To remove the currently set session, a nullptr pointer can be given.

Must be called before [connect()](#group__net_1gac0e835a51a9e772b0d2f1e61cfddd87f) to be effective.

#### `public SSLSession::Ptr currentSession()` {#group__net_1gab2d4b703c76423909dd34f47f46e73fe}



Returns the SSL session of the current connection, for reuse in a future connection (if session caching is enabled).

If no connection is established, returns nullptr.

#### `public bool sessionWasReused()` {#group__net_1gab713d571c3f619b39d09686f6299ce14}



Returns true if a reused session was negotiated during the handshake.

#### `public int available() const` {#group__net_1gacbbee7806e51d77e9cbe79c104806f17}



Returns the number of bytes available from the SSL buffer for immediate reading.

#### `public X509 * peerCertificate() const` {#group__net_1ga6960af6e2f4f16eb05d7df38ab4502d1}

Returns the peer's certificate.



#### `public virtual net::TransportType transport() const` {#group__net_1ga021d710698207af5a0cd57d8b979e0bc}

The transport protocol: TCP, UDP or SSLTCP.



#### `public virtual void acceptConnection()` {#group__net_1gac3dbe26eb2ae4f87faac3260ed43f6ad}





#### `public virtual void onConnect(uv_connect_t * handle,int status)` {#group__net_1ga6540494e1ca223a0df4be4065ee761a2}





#### `public virtual void onRead(const char * data,std::size_t len)` {#group__net_1ga8ec4f05ae232bcc0bfee0a549b1e3ddb}

Reads raw encrypted SSL data.



#### `protected net::SSLContext::Ptr _context` {#group__net_1ga77085f0fa65a457889f2f561e3fec04c}

virtual bool readStart();



#### `protected net::SSLSession::Ptr _session` {#group__net_1gaad9d1562edfa5ee0a6e0b9e601068d04}





#### `protected `[`net::SSLAdapter`](./doc/api-net.md#classscy_1_1net_1_1SSLAdapter)` _sslAdapter` {#group__net_1ga6d18c18a5cf420456474c9e55736a1ac}





# class `scy::net::TCPSocket` {#classscy_1_1net_1_1TCPSocket}

```
class scy::net::TCPSocket
  : public scy::Stream
  : public scy::net::Socket
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`Signal`](#classscy_1_1Signal)< void(const net::TCPSocket::Ptr &)`> AcceptConnection` | 
`public  TCPSocket(uv::Loop * loop)` | 
`public virtual  ~TCPSocket()` | 
`public virtual bool shutdown()` | 
`public virtual void close()` | 
`public virtual void connect(const `[`net::Address`](#classscy_1_1net_1_1Address)` & address)` | 
`public virtual int send(const char * data,std::size_t len,int flags)` | 
`public virtual int send(const char * data,std::size_t len,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress,int flags)` | 
`public virtual void bind(const `[`net::Address`](#classscy_1_1net_1_1Address)` & address,unsigned flags)` | 
`public virtual void listen(int backlog)` | 
`public virtual void acceptConnection()` | 
`public virtual void setNoDelay(bool enable)` | 
`public virtual void setKeepAlive(int enable,unsigned int delay)` | 
`public virtual uv::Loop * loop() const` | Returns the socket event loop.
`public void setMode(`[`SocketMode`](#group__net_1gadfa90186218954a86b7d57d02c563da2)` mode)` | 
`public const `[`SocketMode`](#group__net_1gadfa90186218954a86b7d57d02c563da2)` mode() const` | 
`public virtual void setError(const `[`scy::Error`](#structscy_1_1Error)` & err)` | 
`public virtual const `[`scy::Error`](#structscy_1_1Error)` & error() const` | Return the socket error if any.
`public virtual bool closed() const` | Returns true if the native socket handle is closed.
`public virtual `[`net::Address`](#classscy_1_1net_1_1Address)` address() const` | 
`public virtual `[`net::Address`](#classscy_1_1net_1_1Address)` peerAddress() const` | 
`public virtual net::TransportType transport() const` | Returns the TCP transport protocol.
`public virtual void onConnect(uv_connect_t * handle,int status)` | 
`public virtual void onAcceptConnection(uv_stream_t * handle,int status)` | 
`public virtual void onRead(const char * data,std::size_t len)` | 
`public virtual void onRecv(const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buf)` | 
`public virtual void onError(const `[`scy::Error`](#structscy_1_1Error)` &)` | 
`public virtual void onClose()` | Override to handle closure.
`protected uv_connect_t * _connectReq` | 
`protected `[`SocketMode`](#group__net_1gadfa90186218954a86b7d57d02c563da2)` _mode` | 
`protected virtual void init()` | Initializes the underlying socket context.

## Members

#### `public `[`Signal`](#classscy_1_1Signal)< void(const net::TCPSocket::Ptr &)`> AcceptConnection` {#group__net_1ga22c24cb537c35dbdd0443b5b2d96b374}





#### `public  TCPSocket(uv::Loop * loop)` {#group__net_1gaec3b1c6ef0978a45b92a3941550d34cc}





#### `public virtual  ~TCPSocket()` {#group__net_1gaf96b67205dcf571df23d167589688858}





#### `public virtual bool shutdown()` {#group__net_1gaf4823d66f85cfa1b9fc59a786244c948}



Sends the shutdown packet which should result is socket closure via callback.

#### `public virtual void close()` {#group__net_1gaac7756a77cbaa32d75144cba9e6dfbb4}



Closes and resets the stream handle. This will close the active socket/pipe and destroy the uv_stream_t handle.

If the stream is already closed this call will have no side-effects.

#### `public virtual void connect(const `[`net::Address`](#classscy_1_1net_1_1Address)` & address)` {#group__net_1gac0e835a51a9e772b0d2f1e61cfddd87f}



Connects to the given peer IP address.

Throws an exception if the address is malformed. Connection errors can be handled via the [Error](#structscy_1_1Error) signal.

#### `public virtual int send(const char * data,std::size_t len,int flags)` {#group__net_1ga2feef69365e2505d4be657fdf730cc3a}



Sends the given data buffer to the connected peer. Returns the number of bytes sent or -1 on error. No exception will be thrown. For TCP sockets the given peer address must match the connected peer address.

#### `public virtual int send(const char * data,std::size_t len,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress,int flags)` {#group__net_1ga662e739eeef978a242611637820c5e8b}





#### `public virtual void bind(const `[`net::Address`](#classscy_1_1net_1_1Address)` & address,unsigned flags)` {#group__net_1ga98fd007d802a02377ee8e640d743c30e}



Bind a local address to the socket. The address may be IPv4 or IPv6 (if supported).

Throws an Exception on error.

#### `public virtual void listen(int backlog)` {#group__net_1ga927b6fdca8dbfefe96073fa8e56f19a8}



Listens the socket on the given address.

Throws an Exception on error.

#### `public virtual void acceptConnection()` {#group__net_1ga32b031a136c7083ed70b9586dbf4cb6c}





#### `public virtual void setNoDelay(bool enable)` {#group__net_1ga90240ecdc2eae0b2ab7a32d3531f82de}





#### `public virtual void setKeepAlive(int enable,unsigned int delay)` {#group__net_1gab57f4b56474b39687fad9cf12e21fa49}





#### `public virtual uv::Loop * loop() const` {#group__net_1gad4fa1374b259f711239bc6854244efbf}

Returns the socket event loop.



#### `public void setMode(`[`SocketMode`](#group__net_1gadfa90186218954a86b7d57d02c563da2)` mode)` {#group__net_1gaecf346cd0d3f3d3b4cd560b5776f84d9}





#### `public const `[`SocketMode`](#group__net_1gadfa90186218954a86b7d57d02c563da2)` mode() const` {#group__net_1ga3415e01ef65890d3e03c6c4bcb716cce}





#### `public virtual void setError(const `[`scy::Error`](#structscy_1_1Error)` & err)` {#group__net_1ga48fd2b9f1ca578a4d61d4f0752f25f0a}



Sets the socket error.

Setting the error will result in socket closure.

#### `public virtual const `[`scy::Error`](#structscy_1_1Error)` & error() const` {#group__net_1gafc8ebfbfc6639ee9dff3245959c57a68}

Return the socket error if any.



#### `public virtual bool closed() const` {#group__net_1ga2a058b90f0e9239452258edf61d498a2}

Returns true if the native socket handle is closed.



#### `public virtual `[`net::Address`](#classscy_1_1net_1_1Address)` address() const` {#group__net_1ga47415f4b17c63e60b569062ccfa71351}



Returns the IP address and port number of the socket. A wildcard address is returned if the socket is not connected.

#### `public virtual `[`net::Address`](#classscy_1_1net_1_1Address)` peerAddress() const` {#group__net_1ga50add914140cf8bd7d57cf4c3bc6910d}



Returns the IP address and port number of the peer socket. A wildcard address is returned if the socket is not connected.

#### `public virtual net::TransportType transport() const` {#group__net_1gaf4e4fa75db5e16d812c263175a64717f}

Returns the TCP transport protocol.



#### `public virtual void onConnect(uv_connect_t * handle,int status)` {#group__net_1ga71651156b12d81e75cff69bdad92e9e6}





#### `public virtual void onAcceptConnection(uv_stream_t * handle,int status)` {#group__net_1gaff9d9ba6cf6dc5945042469051449215}





#### `public virtual void onRead(const char * data,std::size_t len)` {#group__net_1ga1b20256ec1d6b1f9c8c6e78b9c389ef3}





#### `public virtual void onRecv(const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buf)` {#group__net_1ga5cddc3672cd38ebb92e2b699c68599bf}





#### `public virtual void onError(const `[`scy::Error`](#structscy_1_1Error)` &)` {#group__net_1ga86ba9910b2ace4ba9be023e289df39a7}



Override to handle errors. The error may be a UV error, or a custom error.

#### `public virtual void onClose()` {#group__net_1gae87cceecc61edcfb3118374074c0409f}

Override to handle closure.



#### `protected uv_connect_t * _connectReq` {#group__net_1ga23a4979e81f30c2271835ae798f6ae42}





#### `protected `[`SocketMode`](#group__net_1gadfa90186218954a86b7d57d02c563da2)` _mode` {#group__net_1ga27a244b308b0d1632172cd2a5ebeb8aa}





#### `protected virtual void init()` {#group__net_1gaa8038e51e1be9ff56713eb5451b37a4c}

Initializes the underlying socket context.



# class `scy::net::Transaction` {#classscy_1_1net_1_1Transaction}

```
class scy::net::Transaction
  : public scy::PacketTransaction< PacketT >
  : public scy::net::PacketSocketAdapter
```  



This class provides request/response functionality for [IPacket](#classscy_1_1IPacket) types emitted from a [Socket](#classscy_1_1net_1_1Socket).

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  Transaction(const net::Socket::Ptr & socket,const `[`Address`](#classscy_1_1net_1_1Address)` & peerAddress,int timeout,int retries,uv::Loop * loop)` | 
`public inline virtual bool send()` | 
`public inline virtual void cancel()` | 
`public inline virtual void dispose()` | 
`public inline `[`Address`](#classscy_1_1net_1_1Address)` peerAddress() const` | 
`protected `[`Address`](./doc/api-net.md#classscy_1_1net_1_1Address)` _peerAddress` | 
`protected inline virtual  ~Transaction()` | 
`protected inline virtual void onPacket(`[`IPacket`](#classscy_1_1IPacket)` & packet)` | 
`protected inline virtual void onResponse()` | Called when a successful response match is received.
`protected inline virtual bool checkResponse(const PacketT & packet)` | 

## Members

#### `public inline  Transaction(const net::Socket::Ptr & socket,const `[`Address`](#classscy_1_1net_1_1Address)` & peerAddress,int timeout,int retries,uv::Loop * loop)` {#group__net_1gad2318cd42a656c4dd61b43d58c5b8a78}





#### `public inline virtual bool send()` {#group__net_1ga2f21f3712f4ad494f839e93307b02fe6}



Starts the transaction timer and sends the request. Overriding classes should implement send logic here.

#### `public inline virtual void cancel()` {#group__net_1gaac5171efc1a325e378c3a6338bfd8dc6}



Cancellation means that the agent will not retransmit the request, will not treat the lack of response to be a failure, but will wait the duration of the transaction timeout for a response.

#### `public inline virtual void dispose()` {#group__net_1gad6b461043ebfa041d60618bdf80c3b04}



Schedules the transaction for deferred deletion.

It is safe to call this function while the transaction is still active, providing the call is made from the same loop thread which the timer is running on.

Protected by the base implementation as this is called by the internal state machine.

#### `public inline `[`Address`](#classscy_1_1net_1_1Address)` peerAddress() const` {#group__net_1ga4dbe158ad4bd2e7a844cfe8571623d71}





#### `protected `[`Address`](./doc/api-net.md#classscy_1_1net_1_1Address)` _peerAddress` {#group__net_1ga267322b198b169cb769dbc3b74432592}





#### `protected inline virtual  ~Transaction()` {#group__net_1gac5258572859ea693338670794b507639}





#### `protected inline virtual void onPacket(`[`IPacket`](#classscy_1_1IPacket)` & packet)` {#group__net_1ga1896f07c33d9df69e446d8bbce601570}



Overrides the PacketSocketAdapter::onPacket callback for checking potential response candidates.

#### `protected inline virtual void onResponse()` {#group__net_1ga7d30ff36061f3f3dec28f4aabbb464fb}

Called when a successful response match is received.



#### `protected inline virtual bool checkResponse(const PacketT & packet)` {#group__net_1gadcf5d65d88479ac9687fe1a1035a2b12}



Sub classes should derive this method to implement response checking logic. The base implementation only performs address matching.

# class `scy::net::UDPSocket` {#classscy_1_1net_1_1UDPSocket}

```
class scy::net::UDPSocket
  : public scy::net::Socket
  : public scy::uv::Handle
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  UDPSocket(uv::Loop * loop)` | 
`public virtual  ~UDPSocket()` | 
`public virtual void connect(const `[`net::Address`](#classscy_1_1net_1_1Address)` & address)` | 
`public virtual void close()` | Closes the underlying socket.
`public virtual void bind(const `[`net::Address`](#classscy_1_1net_1_1Address)` & address,unsigned flags)` | 
`public virtual int send(const char * data,std::size_t len,int flags)` | 
`public virtual int send(const char * data,std::size_t len,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress,int flags)` | 
`public virtual bool setBroadcast(bool flag)` | 
`public virtual bool setMulticastLoop(bool flag)` | 
`public virtual bool setMulticastTTL(int ttl)` | 
`public virtual `[`net::Address`](#classscy_1_1net_1_1Address)` address() const` | 
`public virtual `[`net::Address`](#classscy_1_1net_1_1Address)` peerAddress() const` | 
`public virtual net::TransportType transport() const` | Returns the UDP transport protocol.
`public virtual void setError(const `[`scy::Error`](#structscy_1_1Error)` & err)` | 
`public virtual const `[`scy::Error`](#structscy_1_1Error)` & error() const` | Return the socket error if any.
`public virtual bool closed() const` | 
`public virtual uv::Loop * loop() const` | Returns the socket event loop.
`public virtual void onRecv(const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buf,const `[`net::Address`](#classscy_1_1net_1_1Address)` & address)` | 
`protected `[`net::Address`](./doc/api-net.md#classscy_1_1net_1_1Address)` _peer` | 
`protected Buffer _buffer` | 
`protected virtual void init()` | Initializes the underlying socket context.
`protected virtual bool recvStart()` | 
`protected virtual bool recvStop()` | 
`protected virtual void onError(const `[`scy::Error`](#structscy_1_1Error)` &)` | 
`protected virtual void onClose()` | Override to handle closure.

## Members

#### `public  UDPSocket(uv::Loop * loop)` {#group__net_1gac832137d0bc5d3397f48186f4265c800}





#### `public virtual  ~UDPSocket()` {#group__net_1gac421df9fbeeb69142769802e9a80a178}





#### `public virtual void connect(const `[`net::Address`](#classscy_1_1net_1_1Address)` & address)` {#group__net_1ga797e9a6f6b3e6318ea4ac58dbed53363}



Connects to the given peer IP address.

Throws an exception if the address is malformed. Connection errors can be handled via the [Error](#structscy_1_1Error) signal.

#### `public virtual void close()` {#group__net_1ga03d1f5ad1acef55588876b42a1b1654f}

Closes the underlying socket.



#### `public virtual void bind(const `[`net::Address`](#classscy_1_1net_1_1Address)` & address,unsigned flags)` {#group__net_1gabcfe1a261c55d7af6bd3e4571f1b8bce}



Bind a local address to the socket. The address may be IPv4 or IPv6 (if supported).

Throws an Exception on error.

#### `public virtual int send(const char * data,std::size_t len,int flags)` {#group__net_1ga822cc0064d99017a5c630102cf1b73ce}



Sends the given data buffer to the connected peer. Returns the number of bytes sent or -1 on error. No exception will be thrown. For TCP sockets the given peer address must match the connected peer address.

#### `public virtual int send(const char * data,std::size_t len,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress,int flags)` {#group__net_1ga3f619561d73576ecbb3dd53f7529c0f8}





#### `public virtual bool setBroadcast(bool flag)` {#group__net_1ga0ee0352e997a45deff1805bfaa7dbe62}





#### `public virtual bool setMulticastLoop(bool flag)` {#group__net_1gad485a50948ead139d839ec2d53d83707}





#### `public virtual bool setMulticastTTL(int ttl)` {#group__net_1ga1b751c2f4af3dc831bde8ee1c08059d3}





#### `public virtual `[`net::Address`](#classscy_1_1net_1_1Address)` address() const` {#group__net_1ga5ae3170c0c25ae8f39406b4cec68688b}



The locally bound address.

This function will not throw. A Wildcard 0.0.0.0:0 address is returned if the socket is closed or invalid.

#### `public virtual `[`net::Address`](#classscy_1_1net_1_1Address)` peerAddress() const` {#group__net_1ga086f77292ff877b62ca1311dec863160}



The connected peer address.

This function will not throw. A Wildcard 0.0.0.0:0 address is returned if the socket is closed or invalid.

#### `public virtual net::TransportType transport() const` {#group__net_1ga9229386233c95cf9893f1e8319e8dc26}

Returns the UDP transport protocol.



#### `public virtual void setError(const `[`scy::Error`](#structscy_1_1Error)` & err)` {#group__net_1ga3c6527f692bc0af1bbe383c9c1a97e24}



Sets the socket error.

Setting the error will result in socket closure.

#### `public virtual const `[`scy::Error`](#structscy_1_1Error)` & error() const` {#group__net_1ga8272949cca4775a9e65b63d8810cd06d}

Return the socket error if any.



#### `public virtual bool closed() const` {#group__net_1ga65ad24dcdf36eccca4472ec086064e38}



Returns true if the native socket handle is closed.

#### `public virtual uv::Loop * loop() const` {#group__net_1gab68c745ae8810c4494654440604e77f4}

Returns the socket event loop.



#### `public virtual void onRecv(const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buf,const `[`net::Address`](#classscy_1_1net_1_1Address)` & address)` {#group__net_1ga1b142a3c18ca885e72d45aff466723a9}





#### `protected `[`net::Address`](./doc/api-net.md#classscy_1_1net_1_1Address)` _peer` {#group__net_1ga4b6e88e303adb6fc54a6d496e04d6d7e}





#### `protected Buffer _buffer` {#group__net_1gae4164f1c3392a3ef567cd255191afe78}





#### `protected virtual void init()` {#group__net_1gaa113650e5d196b1ecb3de255b4221e6b}

Initializes the underlying socket context.



#### `protected virtual bool recvStart()` {#group__net_1ga33d1b77ebd07eade126a299151ef5258}





#### `protected virtual bool recvStop()` {#group__net_1gaf16dae934756ee7a561a7cc965cde0bd}





#### `protected virtual void onError(const `[`scy::Error`](#structscy_1_1Error)` &)` {#group__net_1ga1e2104ecebc9baacadfbb5b4199e47dd}



Override to handle errors. The error may be a UV error, or a custom error.

#### `protected virtual void onClose()` {#group__net_1ga56ac720a98e90ab60908ff0ce1ad73fc}

Override to handle closure.



# class `scy::net::VerificationErrorDetails` {#classscy_1_1net_1_1VerificationErrorDetails}


A utility class for certificate error handling.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  VerificationErrorDetails(const `[`crypto::X509Certificate`](#classscy_1_1crypto_1_1X509Certificate)` & cert,int errDepth,int errNum,const std::string & errMsg)` | 
`public  ~VerificationErrorDetails()` | Destroys the [VerificationErrorDetails](./doc/api-net.md#classscy_1_1net_1_1VerificationErrorDetails).
`public inline const `[`crypto::X509Certificate`](#classscy_1_1crypto_1_1X509Certificate)` & certificate() const` | Returns the certificate that caused the error.
`public inline int errorDepth() const` | Returns the position of the certificate in the certificate chain.
`public inline int errorNumber() const` | Returns the id of the error.
`public inline const std::string & errorMessage() const` | Returns the textual presentation of the errorNumber.
`public inline void setIgnoreError(bool ignoreError)` | 
`public inline bool getIgnoreError() const` | returns the value of _ignoreError

## Members

#### `public  VerificationErrorDetails(const `[`crypto::X509Certificate`](#classscy_1_1crypto_1_1X509Certificate)` & cert,int errDepth,int errNum,const std::string & errMsg)` {#group__net_1ga6149cde284d053bbb8ea7f7d4febf260}



Creates the [VerificationErrorDetails](#classscy_1_1net_1_1VerificationErrorDetails). _ignoreError is per default set to false.

#### `public  ~VerificationErrorDetails()` {#group__net_1ga341d067a4369d88415b30f9d6acf9516}

Destroys the [VerificationErrorDetails](#classscy_1_1net_1_1VerificationErrorDetails).



#### `public inline const `[`crypto::X509Certificate`](#classscy_1_1crypto_1_1X509Certificate)` & certificate() const` {#group__net_1gaa61f1bc21123f77329fcc4c2e9c1b3a5}

Returns the certificate that caused the error.



#### `public inline int errorDepth() const` {#group__net_1ga7f64fe88500e3df99bd0bd7cd8d2d598}

Returns the position of the certificate in the certificate chain.



#### `public inline int errorNumber() const` {#group__net_1gac0130d1cbbca92575e2259000f792cef}

Returns the id of the error.



#### `public inline const std::string & errorMessage() const` {#group__net_1gaa13f824914ad36284036dcf6a07d6b2c}

Returns the textual presentation of the errorNumber.



#### `public inline void setIgnoreError(bool ignoreError)` {#group__net_1ga762c71561020eb2eae6842f5cfc6dcd4}



setIgnoreError to true, if a verification error is judged non-fatal by the user.

#### `public inline bool getIgnoreError() const` {#group__net_1ga682fc13cdc647b176d2e0d3ef3e783fb}

returns the value of _ignoreError



# struct `scy::net::DNSResult` {#structscy_1_1net_1_1DNSResult}




DNS Resolver

Example:


```cpp
static void onDNSResult(const net::DNSResult& result)
{
    do something with result
}
net::resolveDNS("google.com", 80, onDNSResult);
```

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::string host` | The host to resolve.
`public std::uint16_t port` | The host port to resolve.
`public `[`net::Address`](./doc/api-net.md#classscy_1_1net_1_1Address)` addr` | The resolved address.
`public struct addrinfo * info` | The libuv uv_getaddrinfo result.
`public struct addrinfo * hints` | libuv uv_getaddrinfo hints (optional)
`public std::function< void(const `[`DNSResult`](#structscy_1_1net_1_1DNSResult) &)`> callback` | Result callback function.
`public void * opaque` | Client data pointer.
`public enum scy::net::DNSResult::Status status` | 
`public inline bool resolving() const` | 
`public inline bool success() const` | 
`public inline bool failed() const` | 
`public inline bool complete() const` | 
`public inline  DNSResult()` | 

## Members

#### `public std::string host` {#group__net_1ga271fc30f8cbeba230aa2a05783d6abd7}

The host to resolve.



#### `public std::uint16_t port` {#group__net_1gad519cdc9e43410c0614d744bbf8a679e}

The host port to resolve.



#### `public `[`net::Address`](./doc/api-net.md#classscy_1_1net_1_1Address)` addr` {#group__net_1ga27835a425b986ef37186c1f9eb5673ef}

The resolved address.



#### `public struct addrinfo * info` {#group__net_1ga4c5a4b9d12163048c4ad5da015d206ff}

The libuv uv_getaddrinfo result.



#### `public struct addrinfo * hints` {#group__net_1ga84908c5b8a039957f4de1385a84406e5}

libuv uv_getaddrinfo hints (optional)



#### `public std::function< void(const `[`DNSResult`](#structscy_1_1net_1_1DNSResult) &)`> callback` {#group__net_1ga537922c7253d9ae3a9631772fc528f6a}

Result callback function.



#### `public void * opaque` {#group__net_1ga2bd2870ec3e9058a7f8cffec95dbadb6}

Client data pointer.



#### `public enum scy::net::DNSResult::Status status` {#group__net_1ga45cc2ae56f1719129314817c9f6c4ee5}





#### `public inline bool resolving() const` {#group__net_1gac0741abc0811be5577aa4b45c4721c97}





#### `public inline bool success() const` {#group__net_1ga0e9eb159f7ad2583b47104334fb08292}





#### `public inline bool failed() const` {#group__net_1gae4575bb020888883c2910c85157f2c5e}





#### `public inline bool complete() const` {#group__net_1ga01adfe1f3099122711931db932695572}





#### `public inline  DNSResult()` {#group__net_1gaf06f1832b28e43dc33f5394bb5e332fc}





# struct `scy::net::PacketInfo` {#structscy_1_1net_1_1PacketInfo}

```
struct scy::net::PacketInfo
  : public scy::IPacketInfo
```  



Provides information about packets emitted from a socket. See [SocketPacket](#classscy_1_1net_1_1SocketPacket).

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public Socket::Ptr socket` | The source socket.
`public `[`Address`](./doc/api-net.md#classscy_1_1net_1_1Address)` peerAddress` | 
`public inline  PacketInfo(const Socket::Ptr & socket,const `[`Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
`public inline  PacketInfo(const `[`PacketInfo`](#structscy_1_1net_1_1PacketInfo)` & r)` | 
`public inline virtual `[`IPacketInfo`](#structscy_1_1IPacketInfo)` * clone() const` | 
`public inline virtual  ~PacketInfo()` | 

## Members

#### `public Socket::Ptr socket` {#group__net_1ga679e93ce36b1f3bd031bbc54ea7939db}

The source socket.



#### `public `[`Address`](./doc/api-net.md#classscy_1_1net_1_1Address)` peerAddress` {#group__net_1ga41acad06a100d281506e6bf269b13799}



The originating peer address. For TCP this will always be connected address.

#### `public inline  PacketInfo(const Socket::Ptr & socket,const `[`Address`](#classscy_1_1net_1_1Address)` & peerAddress)` {#group__net_1gac4cee1c5178b1581eee877736e102755}





#### `public inline  PacketInfo(const `[`PacketInfo`](#structscy_1_1net_1_1PacketInfo)` & r)` {#group__net_1gae0b9f9bcc10f550dfea602f962092247}





#### `public inline virtual `[`IPacketInfo`](#structscy_1_1IPacketInfo)` * clone() const` {#group__net_1gad44f00df02b824a31b4afd69db788655}





#### `public inline virtual  ~PacketInfo()` {#group__net_1ga0b05488ca9b52e594d47a21678150869}





