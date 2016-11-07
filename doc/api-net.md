# Module <!-- group --> `net`



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`namespace `[``scy::net``](#namespacescy_1_1net)    | 
`class `[``scy::net::Address``](#classscy_1_1net_1_1Address)    | 
`class `[``scy::net::PacketSocketAdapter``](#classscy_1_1net_1_1PacketSocketAdapter)    | 
`class `[``scy::net::Socket``](#classscy_1_1net_1_1Socket)    | 
`class `[``scy::net::SocketPacket``](#classscy_1_1net_1_1SocketPacket)    | 
`class `[``scy::net::SocketAdapter``](#classscy_1_1net_1_1SocketAdapter)    | 
`class `[``scy::net::SSLAdapter``](#classscy_1_1net_1_1SSLAdapter)    | 
`class `[``scy::net::SSLContext``](#classscy_1_1net_1_1SSLContext)    | 
`class `[``scy::net::SSLManager``](#classscy_1_1net_1_1SSLManager)    | 
`class `[``scy::net::VerificationErrorDetails``](#classscy_1_1net_1_1VerificationErrorDetails)    | A utility class for certificate error handling.
`class `[``scy::net::SSLSession``](#classscy_1_1net_1_1SSLSession)    | 
`class `[``scy::net::SSLSocket``](#classscy_1_1net_1_1SSLSocket)    | 
`class `[``scy::net::TCPSocket``](#classscy_1_1net_1_1TCPSocket)    | 
`class `[``scy::net::Transaction``](#classscy_1_1net_1_1Transaction)    | 
`class `[``scy::net::UDPSocket``](#classscy_1_1net_1_1UDPSocket)    | 
`class `[``scy::net::AddressBase``](#classscy_1_1net_1_1AddressBase)    | 
`class `[``scy::net::IPv4AddressBase``](#classscy_1_1net_1_1IPv4AddressBase)    | 
`struct `[``scy::net::DNSResult``](#structscy_1_1net_1_1DNSResult)    | 
`struct `[``scy::net::PacketInfo``](#structscy_1_1net_1_1PacketInfo)    | 
# namespace `scy::net`



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[``scy::net::Address``](#classscy_1_1net_1_1Address)    | 
`class `[``scy::net::AddressBase``](#classscy_1_1net_1_1AddressBase)    | 
`class `[``scy::net::IPv4AddressBase``](#classscy_1_1net_1_1IPv4AddressBase)    | 
`class `[``scy::net::PacketSocketAdapter``](#classscy_1_1net_1_1PacketSocketAdapter)    | 
`class `[``scy::net::Socket``](#classscy_1_1net_1_1Socket)    | 
`class `[``scy::net::SocketAdapter``](#classscy_1_1net_1_1SocketAdapter)    | 
`class `[``scy::net::SocketPacket``](#classscy_1_1net_1_1SocketPacket)    | 
`class `[``scy::net::SSLAdapter``](#classscy_1_1net_1_1SSLAdapter)    | 
`class `[``scy::net::SSLContext``](#classscy_1_1net_1_1SSLContext)    | 
`class `[``scy::net::SSLManager``](#classscy_1_1net_1_1SSLManager)    | 
`class `[``scy::net::SSLSession``](#classscy_1_1net_1_1SSLSession)    | 
`class `[``scy::net::SSLSocket``](#classscy_1_1net_1_1SSLSocket)    | 
`class `[``scy::net::TCPSocket``](#classscy_1_1net_1_1TCPSocket)    | 
`class `[``scy::net::Transaction``](#classscy_1_1net_1_1Transaction)    | 
`class `[``scy::net::UDPSocket``](#classscy_1_1net_1_1UDPSocket)    | 
`class `[``scy::net::VerificationErrorDetails``](#classscy_1_1net_1_1VerificationErrorDetails)    | A utility class for certificate error handling.
`struct `[``scy::net::DNSResult``](#structscy_1_1net_1_1DNSResult)    | 
`struct `[``scy::net::PacketInfo``](#structscy_1_1net_1_1PacketInfo)    | 
# class `scy::net::Address` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Address()` | Creates a wildcard (all zero) IPv4 [Address](#classscy_1_1net_1_1Address).
`public  Address(const std::string & host,std::uint16_t port)` | 
`public  Address(const `[`Address`](#classscy_1_1net_1_1Address)` & addr)` | Creates a [Address](#classscy_1_1net_1_1Address) by copying another one.
`public  Address(const struct sockaddr * addr,socklen_t length)` | Creates a [Address](#classscy_1_1net_1_1Address) from a native socket address.
`public  Address(const std::string & host,const std::string & port)` | 
`public  explicit Address(const std::string & hostAndPort)` | 
`public  ~Address()` | Destroys the [Address](#classscy_1_1net_1_1Address).
`public `[`Address`](#classscy_1_1net_1_1Address)` & operator=(const `[`Address`](#classscy_1_1net_1_1Address)` & addr)` | Assigns another [Address](#classscy_1_1net_1_1Address).
`public void swap(`[`Address`](#classscy_1_1net_1_1Address)` & addr)` | Swaps the [Address](#classscy_1_1net_1_1Address) with another one.
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

#### `public  Address()` 

Creates a wildcard (all zero) IPv4 [Address](#classscy_1_1net_1_1Address).



#### `public  Address(const std::string & host,std::uint16_t port)` 



Creates a [Address](#classscy_1_1net_1_1Address) from an IP address and a port number.

The IP address must either be a domain name, or it must be in dotted decimal (IPv4) or hex string (IPv6) format.

#### `public  Address(const `[`Address`](#classscy_1_1net_1_1Address)` & addr)` 

Creates a [Address](#classscy_1_1net_1_1Address) by copying another one.



#### `public  Address(const struct sockaddr * addr,socklen_t length)` 

Creates a [Address](#classscy_1_1net_1_1Address) from a native socket address.



#### `public  Address(const std::string & host,const std::string & port)` 



Creates a [Address](#classscy_1_1net_1_1Address) from an IP address and a service name or port number.

The IP address must either be a domain name, or it must be in dotted decimal (IPv4) or hex string (IPv6) format.

The given port must either be a decimal port number, or a service name.

#### `public  explicit Address(const std::string & hostAndPort)` 



Creates a [Address](#classscy_1_1net_1_1Address) from an IP address or host name and a port number/service name. Host name/address and port number must be separated by a colon. In case of an IPv6 address, the address part must be enclosed in brackets.

Examples: 192.168.1.10:80

www.sourcey.com:8080

#### `public  ~Address()` 

Destroys the [Address](#classscy_1_1net_1_1Address).



#### `public `[`Address`](#classscy_1_1net_1_1Address)` & operator=(const `[`Address`](#classscy_1_1net_1_1Address)` & addr)` 

Assigns another [Address](#classscy_1_1net_1_1Address).



#### `public void swap(`[`Address`](#classscy_1_1net_1_1Address)` & addr)` 

Swaps the [Address](#classscy_1_1net_1_1Address) with another one.



#### `public std::string host() const` 

Returns the host IP address.



#### `public std::uint16_t port() const` 

Returns the port number.



#### `public socklen_t length() const` 

Returns the length of the internal native socket address.



#### `public const struct sockaddr * addr() const` 

Returns a pointer to the internal native socket address.



#### `public int af() const` 

Returns the address family (AF_INET or AF_INET6) of the address.



#### `public std::string toString() const` 

Returns a string representation of the address.



#### `public `[`Address::Family`](#group__net_1gaf91e77d155f2852974bbae5a2da34106)` family() const` 

Returns the address family of the host's address.



#### `public bool valid() const` 



Returns true when the port is set and the address is valid ie. not wildcard.

#### `public bool operator<(const `[`Address`](#classscy_1_1net_1_1Address)` & addr) const` 





#### `public bool operator==(const `[`Address`](#classscy_1_1net_1_1Address)` & addr) const` 





#### `public bool operator!=(const `[`Address`](#classscy_1_1net_1_1Address)` & addr) const` 





#### `protected void init(const std::string & host,std::uint16_t port)` 





# class `scy::net::AddressBase` 

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

#### `public std::string host() const` 





#### `public std::uint16_t port() const` 





#### `public `[`Address::Family`](#group__net_1gaf91e77d155f2852974bbae5a2da34106)` family() const` 





#### `public socklen_t length() const` 





#### `public const struct sockaddr * addr() const` 





#### `public int af() const` 





#### `protected inline  AddressBase()` 





#### `protected inline virtual  ~AddressBase()` 





# class `scy::net::IPv4AddressBase` 

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

#### `public inline  IPv4AddressBase()` 





#### `public inline  IPv4AddressBase(const struct sockaddr_in * addr)` 





#### `public inline  IPv4AddressBase(const void * addr,std::uint16_t port)` 





#### `public inline virtual std::string host() const` 





#### `public inline virtual std::uint16_t port() const` 





#### `public inline virtual `[`Address::Family`](#group__net_1gaf91e77d155f2852974bbae5a2da34106)` family() const` 





#### `public inline virtual socklen_t length() const` 





#### `public inline virtual const struct sockaddr * addr() const` 





#### `public inline virtual int af() const` 





# class `scy::net::PacketSocketAdapter` 

```
class scy::net::PacketSocketAdapter
  : public scy::net::SocketAdapter
  : public scy::SignalBase< PacketDelegateBase, IPacket & >
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public net::Socket::Ptr socket` | 
`public `[`PacketFactory`](#classscy_1_1PacketFactory)` factory` | 
`public  PacketSocketAdapter(const net::Socket::Ptr & socket)` | 
`public virtual void onSocketRecv(const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
`public virtual void onPacket(`[`IPacket`](#classscy_1_1IPacket)` & pkt)` | 

## Members

#### `public net::Socket::Ptr socket` 



Pointer to the underlying socket. Sent data will be proxied to this socket.

#### `public `[`PacketFactory`](#classscy_1_1PacketFactory)` factory` 





#### `public  PacketSocketAdapter(const net::Socket::Ptr & socket)` 



Creates the [PacketSocketAdapter](#classscy_1_1net_1_1PacketSocketAdapter) This class should have a higher priority than standard sockets so we can parse data packets first. Creates and dispatches a packet utilizing the available creation strategies. For best performance the most used strategies should have the highest priority.

#### `public virtual void onSocketRecv(const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`Address`](#classscy_1_1net_1_1Address)` & peerAddress)` 





#### `public virtual void onPacket(`[`IPacket`](#classscy_1_1IPacket)` & pkt)` 





# class `scy::net::Socket` 

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

#### `public  Socket()` 





#### `public virtual  ~Socket()` 





#### `public void connect(const `[`Address`](#classscy_1_1net_1_1Address)` & address)` 



Connects to the given peer IP address.

Throws an exception if the address is malformed. Connection errors can be handled via the [Error](#structscy_1_1Error) signal.

#### `public virtual void connect(const std::string & host,std::uint16_t port)` 



Resolves and connects to the given host address.

Throws an Exception if the host is malformed. Since the DNS callback is asynchronous implementations need to listen for the [Error](#structscy_1_1Error) signal for handling connection errors.

#### `public void bind(const `[`Address`](#classscy_1_1net_1_1Address)` & address,unsigned flags)` 



Bind a local address to the socket. The address may be IPv4 or IPv6 (if supported).

Throws an Exception on error.

#### `public inline virtual void listen(int backlog)` 



Listens the socket on the given address.

Throws an Exception on error.

#### `public inline virtual bool shutdown()` 



Sends the shutdown packet which should result is socket closure via callback.

#### `public void close()` 

Closes the underlying socket.



#### `public `[`Address`](#classscy_1_1net_1_1Address)` address() const` 



The locally bound address.

This function will not throw. A Wildcard 0.0.0.0:0 address is returned if the socket is closed or invalid.

#### `public `[`Address`](#classscy_1_1net_1_1Address)` peerAddress() const` 



The connected peer address.

This function will not throw. A Wildcard 0.0.0.0:0 address is returned if the socket is closed or invalid.

#### `public net::TransportType transport() const` 

The transport protocol: TCP, UDP or SSLTCP.



#### `public void setError(const `[`scy::Error`](#structscy_1_1Error)` & err)` 



Sets the socket error.

Setting the error will result in socket closure.

#### `public const `[`scy::Error`](#structscy_1_1Error)` & error() const` 

Return the socket error if any.



#### `public bool closed() const` 

Returns true if the native socket handle is closed.



#### `public uv::Loop * loop() const` 

Returns the socket event loop.



#### `protected void init()` 

Initializes the underlying socket context.



#### `protected inline virtual void reset()` 

Resets the socket context for reuse.



#### `protected inline virtual void * self()` 



Returns the derived instance pointer for casting [SocketAdapter](#classscy_1_1net_1_1SocketAdapter) signal callback sender arguments from void* to [Socket](#classscy_1_1net_1_1Socket). Note: This method must not be derived by subclasses or casting will fail for void* pointer callbacks.

# class `scy::net::SocketAdapter` 




[SocketAdapter](#classscy_1_1net_1_1SocketAdapter) is the abstract interface for all socket classes. A [SocketAdapter](#classscy_1_1net_1_1SocketAdapter) can also be attached to a [Socket](#classscy_1_1net_1_1Socket) in order to override default [Socket](#classscy_1_1net_1_1Socket) callbacks and behaviour, while still maintaining the default [Socket](#classscy_1_1net_1_1Socket) interface (see Socket::setAdapter).

This class also be extended to implement custom processing for received socket data before it is dispatched to the application (see [PacketSocketAdapter](#classscy_1_1net_1_1PacketSocketAdapter) and [Transaction](#classscy_1_1net_1_1Transaction) classes).

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public void * opaque` | 
`public `[`NullSignal`](#classscy_1_1NullSignal)` Connect` | Signals that the socket is connected.
`public `[`Signal2](#classscy_1_1Signal2)< const [MutableBuffer](#classscy_1_1MutableBuffer) &, const [Address`](#classscy_1_1net_1_1Address)` & > Recv` | Signals when data is received by the socket.
`public `[`Signal](#classscy_1_1Signal)< const [scy::Error`](#structscy_1_1Error)` & > Error` | 
`public `[`NullSignal`](#classscy_1_1NullSignal)` Close` | Signals that the underlying socket is closed.
`public  SocketAdapter(`[`SocketAdapter`](#classscy_1_1net_1_1SocketAdapter)` * sender,`[`SocketAdapter`](#classscy_1_1net_1_1SocketAdapter)` * receiver)` | Creates the [SocketAdapter](#classscy_1_1net_1_1SocketAdapter).
`public virtual  ~SocketAdapter()` | Destroys the [SocketAdapter](#classscy_1_1net_1_1SocketAdapter).
`public virtual int send(const char * data,std::size_t len,int flags)` | 
`public virtual int send(const char * data,std::size_t len,const `[`Address`](#classscy_1_1net_1_1Address)` & peerAddress,int flags)` | 
`public virtual int sendPacket(const `[`IPacket`](#classscy_1_1IPacket)` & packet,int flags)` | 
`public virtual int sendPacket(const `[`IPacket`](#classscy_1_1IPacket)` & packet,const `[`Address`](#classscy_1_1net_1_1Address)` & peerAddress,int flags)` | 
`public virtual void sendPacket(`[`IPacket`](#classscy_1_1IPacket)` & packet)` | 
`public virtual void onSocketConnect()` | 
`public virtual void onSocketRecv(const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
`public virtual void onSocketError(const `[`Error`](#structscy_1_1Error)` & error)` | 
`public virtual void onSocketClose()` | 
`public void setSender(`[`SocketAdapter`](#classscy_1_1net_1_1SocketAdapter)` * adapter,bool freeExisting)` | 
`public `[`SocketAdapter`](#classscy_1_1net_1_1SocketAdapter)` * sender()` | Returns the output [SocketAdapter](#classscy_1_1net_1_1SocketAdapter) pointer.
`public void addReceiver(`[`SocketAdapter`](#classscy_1_1net_1_1SocketAdapter)` * adapter,int priority)` | Adds an input [SocketAdapter](#classscy_1_1net_1_1SocketAdapter) for receiving socket callbacks.
`public void removeReceiver(`[`SocketAdapter`](#classscy_1_1net_1_1SocketAdapter)` * adapter)` | Removes an input [SocketAdapter](#classscy_1_1net_1_1SocketAdapter).
`protected `[`SocketAdapter`](#classscy_1_1net_1_1SocketAdapter)` * _sender` | 
`protected inline virtual void * self()` | 
`protected virtual void emitSocketConnect()` | 
`protected virtual void emitSocketRecv(const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
`protected virtual void emitSocketError(const `[`scy::Error`](#structscy_1_1Error)` & error)` | 
`protected virtual void emitSocketClose()` | 

## Members

#### `public void * opaque` 



Optional client data pointer.

The pointer is not initialized or managed by the socket base.

#### `public `[`NullSignal`](#classscy_1_1NullSignal)` Connect` 

Signals that the socket is connected.



#### `public `[`Signal2](#classscy_1_1Signal2)< const [MutableBuffer](#classscy_1_1MutableBuffer) &, const [Address`](#classscy_1_1net_1_1Address)` & > Recv` 

Signals when data is received by the socket.



#### `public `[`Signal](#classscy_1_1Signal)< const [scy::Error`](#structscy_1_1Error)` & > Error` 



Signals that the socket is closed in error. This signal will be sent just before the Closed signal.

#### `public `[`NullSignal`](#classscy_1_1NullSignal)` Close` 

Signals that the underlying socket is closed.



#### `public  SocketAdapter(`[`SocketAdapter`](#classscy_1_1net_1_1SocketAdapter)` * sender,`[`SocketAdapter`](#classscy_1_1net_1_1SocketAdapter)` * receiver)` 

Creates the [SocketAdapter](#classscy_1_1net_1_1SocketAdapter).



#### `public virtual  ~SocketAdapter()` 

Destroys the [SocketAdapter](#classscy_1_1net_1_1SocketAdapter).



#### `public virtual int send(const char * data,std::size_t len,int flags)` 



Sends the given data buffer to the connected peer. Returns the number of bytes sent or -1 on error. No exception will be thrown. For TCP sockets the given peer address must match the connected peer address.

#### `public virtual int send(const char * data,std::size_t len,const `[`Address`](#classscy_1_1net_1_1Address)` & peerAddress,int flags)` 





#### `public virtual int sendPacket(const `[`IPacket`](#classscy_1_1IPacket)` & packet,int flags)` 



Sends the given packet to the connected peer. Returns the number of bytes sent or -1 on error. No exception will be thrown. For TCP sockets the given peer address must match the connected peer address.

#### `public virtual int sendPacket(const `[`IPacket`](#classscy_1_1IPacket)` & packet,const `[`Address`](#classscy_1_1net_1_1Address)` & peerAddress,int flags)` 





#### `public virtual void sendPacket(`[`IPacket`](#classscy_1_1IPacket)` & packet)` 



Sends the given packet to the connected peer. This method provides delegate compatability, and unlike other send methods throws an exception if the underlying socket is closed.

#### `public virtual void onSocketConnect()` 



These virtual methods can be overridden as necessary to intercept socket events before they hit the application.

#### `public virtual void onSocketRecv(const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`Address`](#classscy_1_1net_1_1Address)` & peerAddress)` 





#### `public virtual void onSocketError(const `[`Error`](#structscy_1_1Error)` & error)` 





#### `public virtual void onSocketClose()` 





#### `public void setSender(`[`SocketAdapter`](#classscy_1_1net_1_1SocketAdapter)` * adapter,bool freeExisting)` 



A pointer to the adapter for handling outgoing data. Send methods proxy data to this adapter by default. Note that we only keep a simple pointer so as to avoid circular references preventing destruction.

#### `public `[`SocketAdapter`](#classscy_1_1net_1_1SocketAdapter)` * sender()` 

Returns the output [SocketAdapter](#classscy_1_1net_1_1SocketAdapter) pointer.



#### `public void addReceiver(`[`SocketAdapter`](#classscy_1_1net_1_1SocketAdapter)` * adapter,int priority)` 

Adds an input [SocketAdapter](#classscy_1_1net_1_1SocketAdapter) for receiving socket callbacks.



#### `public void removeReceiver(`[`SocketAdapter`](#classscy_1_1net_1_1SocketAdapter)` * adapter)` 

Removes an input [SocketAdapter](#classscy_1_1net_1_1SocketAdapter).



#### `protected `[`SocketAdapter`](#classscy_1_1net_1_1SocketAdapter)` * _sender` 





#### `protected inline virtual void * self()` 



Returns the polymorphic instance pointer for signal delegate callbacks.

#### `protected virtual void emitSocketConnect()` 





#### `protected virtual void emitSocketRecv(const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`Address`](#classscy_1_1net_1_1Address)` & peerAddress)` 





#### `protected virtual void emitSocketError(const `[`scy::Error`](#structscy_1_1Error)` & error)` 





#### `protected virtual void emitSocketClose()` 





# class `scy::net::SocketPacket` 

```
class scy::net::SocketPacket
  : public scy::RawPacket
```  



[SocketPacket](#classscy_1_1net_1_1SocketPacket) is the default packet type emitted by sockets. [SocketPacket](#classscy_1_1net_1_1SocketPacket) provides peer address information and a buffer reference for nocopy binary operations.

The referenced packet buffer lifetime is only guaranteed for the duration of the receiver callback.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`PacketInfo`](#structscy_1_1net_1_1PacketInfo)` * info` | [PacketInfo](#structscy_1_1net_1_1PacketInfo) pointer.
`public inline  SocketPacket(const Socket::Ptr & socket,const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
`public inline  SocketPacket(const `[`SocketPacket`](#classscy_1_1net_1_1SocketPacket)` & that)` | 
`public inline virtual  ~SocketPacket()` | 
`public inline virtual void print(std::ostream & os) const` | 
`public inline virtual `[`IPacket`](#classscy_1_1IPacket)` * clone() const` | 
`public inline virtual std::size_t read(const `[`ConstBuffer`](#classscy_1_1ConstBuffer)` &)` | 
`public inline virtual void write(Buffer &) const` | 
`public inline virtual const char * className() const` | 

## Members

#### `public `[`PacketInfo`](#structscy_1_1net_1_1PacketInfo)` * info` 

[PacketInfo](#structscy_1_1net_1_1PacketInfo) pointer.



#### `public inline  SocketPacket(const Socket::Ptr & socket,const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`Address`](#classscy_1_1net_1_1Address)` & peerAddress)` 





#### `public inline  SocketPacket(const `[`SocketPacket`](#classscy_1_1net_1_1SocketPacket)` & that)` 





#### `public inline virtual  ~SocketPacket()` 





#### `public inline virtual void print(std::ostream & os) const` 





#### `public inline virtual `[`IPacket`](#classscy_1_1IPacket)` * clone() const` 





#### `public inline virtual std::size_t read(const `[`ConstBuffer`](#classscy_1_1ConstBuffer)` &)` 



Read/parse to the packet from the given input buffer. The number of bytes read is returned.

#### `public inline virtual void write(Buffer &) const` 



Copy/generate to the packet given output buffer. The number of bytes written can be obtained from the buffer.

Todo: It may be prefferable to use our pod types here instead of buffer input, but the current codebase requires that the buffer be dynamically resizable for some protocols...

virtual std::size_t write(MutableBuffer&) const = 0;

#### `public inline virtual const char * className() const` 





# class `scy::net::SSLAdapter` 






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
`protected `[`net::SSLSocket`](#classscy_1_1net_1_1SSLSocket)` * _socket` | 
`protected SSL * _ssl` | 
`protected BIO * _readBIO` | The incoming buffer we write encrypted SSL data into.
`protected BIO * _writeBIO` | The outgoing buffer we write to the socket.
`protected std::vector< char > _bufferOut` | The outgoing payload to be encrypted and sent.
`protected void handleError(int rc)` | 
`protected void flushReadBIO()` | 
`protected void flushWriteBIO()` | 

## Members

#### `public  SSLAdapter(`[`net::SSLSocket`](#classscy_1_1net_1_1SSLSocket)` * socket)` 





#### `public  ~SSLAdapter()` 





#### `public void initClient()` 

Initializes the SSL context as a client.



#### `public void initServer()` 

Initializes the SSL context as a server.



#### `public bool initialized() const` 

Returns true when SSL context has been initialized.



#### `public bool ready() const` 

Returns true when the handshake is complete.



#### `public void handshake()` 

Start/continue the SSL handshake process.



#### `public int available() const` 



Returns the number of bytes available in the SSL buffer for immediate reading.

#### `public void shutdown()` 

Issues an orderly SSL shutdown.



#### `public void flush()` 

Flushes the SSL read/write buffers.



#### `public void addIncomingData(const char * data,std::size_t len)` 





#### `public void addOutgoingData(const std::string & data)` 





#### `public void addOutgoingData(const char * data,std::size_t len)` 





#### `protected `[`net::SSLSocket`](#classscy_1_1net_1_1SSLSocket)` * _socket` 





#### `protected SSL * _ssl` 





#### `protected BIO * _readBIO` 

The incoming buffer we write encrypted SSL data into.



#### `protected BIO * _writeBIO` 

The outgoing buffer we write to the socket.



#### `protected std::vector< char > _bufferOut` 

The outgoing payload to be encrypted and sent.



#### `protected void handleError(int rc)` 





#### `protected void flushReadBIO()` 





#### `protected void flushWriteBIO()` 





# class `scy::net::SSLContext` 

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

#### `public  SSLContext(`[`Usage`](#group__net_1ga2e13fff7d4c75c080e515a427bf15c71)` usage,const std::string & privateKeyFile,const std::string & certificateFile,const std::string & caLocation,`[`VerificationMode`](#group__net_1gafd7da63f7077511ce8d4da68f37d255e)` verificationMode,int verificationDepth,bool loadDefaultCAs,const std::string & cipherList)` 



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

#### `public  SSLContext(`[`Usage`](#group__net_1ga2e13fff7d4c75c080e515a427bf15c71)` usage,const std::string & caLocation,`[`VerificationMode`](#group__net_1gafd7da63f7077511ce8d4da68f37d255e)` verificationMode,int verificationDepth,bool loadDefaultCAs,const std::string & cipherList)` 



Creates a Context.



* usage specifies whether the context is used by a client or server.


* caLocation contains the path to the file or directory containing the CA/root certificates. Can be empty if the OpenSSL builtin CA certificates are used (see loadDefaultCAs).


* verificationMode specifies whether and how peer certificates are validated.


* verificationDepth sets the upper limit for verification chain sizes. Verification will fail if a certificate chain larger than this is encountered.


* loadDefaultCAs specifies weather the builtin CA certificates from OpenSSL are used.


* cipherList specifies the supported ciphers in OpenSSL notation.





Note that a private key and/or certificate must be specified with usePrivateKey()/useCertificate() before the Context can be used.

#### `public  ~SSLContext()` 

Destroys the Context.



#### `public void useCertificate(const `[`crypto::X509Certificate`](#classscy_1_1crypto_1_1X509Certificate)` & certificate)` 



Sets the certificate to be used by the Context.

To set-up a complete certificate chain, it might be necessary to call [addChainCertificate()](#group__net_1gac4fe7dff44de813dcdf7daf80ea48d5f) to specify additional certificates.

Note that [useCertificate()](#group__net_1ga840711ee078f8ee6432aaef8b50b48b9) must always be called before usePrivateKey().

#### `public void addChainCertificate(const `[`crypto::X509Certificate`](#classscy_1_1crypto_1_1X509Certificate)` & certificate)` 

Adds a certificate for certificate chain validation.



#### `public void addVerificationCertificate(const `[`crypto::X509Certificate`](#classscy_1_1crypto_1_1X509Certificate)` & certificate)` 



Sets the private key to be used by the Context.

Note that [useCertificate()](#group__net_1ga840711ee078f8ee6432aaef8b50b48b9) must always be called before usePrivateKey().

Note: If the private key is protected by a passphrase, a PrivateKeyPassphraseHandler must have been setup with the [SSLManager](#classscy_1_1net_1_1SSLManager), or the [SSLManager](#classscy_1_1net_1_1SSLManager)'s PrivateKeyPassphraseRequired event must be handled. Adds the given certificate to the list of trusted certificates that will be used for verification.

#### `public inline SSL_CTX * sslContext() const` 

Returns the underlying OpenSSL SSL Context object.



#### `public inline `[`Usage`](#group__net_1ga2e13fff7d4c75c080e515a427bf15c71)` usage() const` 



Returns whether the context is for use by a client or by a server and whether TLSv1 is required.

#### `public inline bool isForServerUse() const` 

Returns true if the context is for use by a server.



#### `public inline `[`SSLContext::VerificationMode`](#group__net_1gafd7da63f7077511ce8d4da68f37d255e)` verificationMode() const` 

Returns the verification mode.



#### `public void enableSessionCache(bool flag)` 



Enable or disable SSL/TLS session caching. For session caching to work, it must be enabled on the server, as well as on the client side.

The default is disabled session caching.

To enable session caching on the server side, use the two-argument version of this method to specify a session ID context.

#### `public void enableSessionCache(bool flag,const std::string & sessionIdContext)` 



Enables or disables SSL/TLS session caching on the server. For session caching to work, it must be enabled on the server, as well as on the client side.

SessionIdContext contains the application's unique session ID context, which becomes part of each session identifier generated by the server within this context. SessionIdContext can be an arbitrary sequence of bytes with a maximum length of SSL_MAX_SSL_SESSION_ID_LENGTH.

A non-empty sessionIdContext should be specified even if session caching is disabled to avoid problems with clients requesting to reuse a session (e.g. Firefox 3.6).

This method may only be called on SERVER_USE Context objects.

#### `public bool sessionCacheEnabled() const` 

Returns true if the session cache is enabled.



#### `public void setSessionCacheSize(std::size_t size)` 



Sets the maximum size of the server session cache, in number of sessions. The default size (according to OpenSSL documentation) is 1024*20, which may be too large for many applications, especially on embedded platforms with limited memory.

Specifying a size of 0 will set an unlimited cache size.

This method may only be called on SERVER_USE Context objects.

#### `public std::size_t getSessionCacheSize() const` 



Returns the current maximum size of the server session cache.

This method may only be called on SERVER_USE Context objects.

#### `public void setSessionTimeout(long seconds)` 



Sets the timeout (in seconds) of cached sessions on the server. A cached session will be removed from the cache if it has not been used for the given number of seconds.

This method may only be called on SERVER_USE Context objects.

#### `public long getSessionTimeout() const` 



Returns the timeout (in seconds) of cached sessions on the server.

This method may only be called on SERVER_USE Context objects.

#### `public void flushSessionCache()` 



Flushes the SSL session cache on the server.

This method may only be called on SERVER_USE Context objects.

#### `public void disableStatelessSessionResumption()` 



Newer versions of OpenSSL support RFC 4507 tickets for stateless session resumption.

The feature can be disabled by calling this method.

# class `scy::net::SSLManager` 




[SSLManager](#classscy_1_1net_1_1SSLManager) is a singleton for holding the default server/client Context and handling callbacks for certificate verification errors and private key passphrases.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`Signal](#classscy_1_1Signal)< [VerificationErrorDetails`](#classscy_1_1net_1_1VerificationErrorDetails)` & > ServerVerificationError` | Fired whenever a certificate verification error is detected by the server during a handshake.
`public `[`Signal](#classscy_1_1Signal)< [VerificationErrorDetails`](#classscy_1_1net_1_1VerificationErrorDetails)` & > ClientVerificationError` | Fired whenever a certificate verification error is detected by the client during a handshake.
`public `[`Signal`](#classscy_1_1Signal)`< std::string & > PrivateKeyPassphraseRequired` | 
`public void initializeServer(SSLContext::Ptr ptrContext)` | Initializes the server side of the [SSLManager](#classscy_1_1net_1_1SSLManager) server-side [SSLContext](#classscy_1_1net_1_1SSLContext).
`public void initializeClient(SSLContext::Ptr ptrContext)` | Initializes the client side of the [SSLManager](#classscy_1_1net_1_1SSLManager) with a default client-side [SSLContext](#classscy_1_1net_1_1SSLContext).
`public SSLContext::Ptr defaultServerContext()` | Returns the default Context used by the server if initialized.
`public SSLContext::Ptr defaultClientContext()` | Returns the default Context used by the client if initialized.
`public void shutdown()` | 

## Members

#### `public `[`Signal](#classscy_1_1Signal)< [VerificationErrorDetails`](#classscy_1_1net_1_1VerificationErrorDetails)` & > ServerVerificationError` 

Fired whenever a certificate verification error is detected by the server during a handshake.



#### `public `[`Signal](#classscy_1_1Signal)< [VerificationErrorDetails`](#classscy_1_1net_1_1VerificationErrorDetails)` & > ClientVerificationError` 

Fired whenever a certificate verification error is detected by the client during a handshake.



#### `public `[`Signal`](#classscy_1_1Signal)`< std::string & > PrivateKeyPassphraseRequired` 



Fired when a encrypted certificate is loaded. Not setting the password in the event parameter will result in a failure to load the certificate.

#### `public void initializeServer(SSLContext::Ptr ptrContext)` 

Initializes the server side of the [SSLManager](#classscy_1_1net_1_1SSLManager) server-side [SSLContext](#classscy_1_1net_1_1SSLContext).



#### `public void initializeClient(SSLContext::Ptr ptrContext)` 

Initializes the client side of the [SSLManager](#classscy_1_1net_1_1SSLManager) with a default client-side [SSLContext](#classscy_1_1net_1_1SSLContext).



#### `public SSLContext::Ptr defaultServerContext()` 

Returns the default Context used by the server if initialized.



#### `public SSLContext::Ptr defaultClientContext()` 

Returns the default Context used by the client if initialized.



#### `public void shutdown()` 



Shuts down the [SSLManager](#classscy_1_1net_1_1SSLManager) and releases the default Context objects. After a call to [shutdown()](#group__net_1ga4061d56eaf906bf79a0713310a3841c5), the [SSLManager](#classscy_1_1net_1_1SSLManager) can no longer be used.

Normally, it's not necessary to call this method directly, as this will be called either by uninitializeSSL(), or when the [SSLManager](#classscy_1_1net_1_1SSLManager) instance is destroyed.

# class `scy::net::SSLSession` 

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

#### `public SSL_SESSION * sslSession() const` 

Returns the stored OpenSSL SSL_SESSION object.



#### `public  SSLSession(SSL_SESSION * ptr)` 



Creates a new Session object, using the given SSL_SESSION object.

The SSL_SESSION's reference count is not changed.

#### `public  ~SSLSession()` 



Destroys the Session.

Calls SSL_SESSION_free() on the stored SSL_SESSION object.

#### `public  SSLSession()` 





#### `protected SSL_SESSION * _ptr` 





# class `scy::net::SSLSocket` 

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
`protected `[`net::SSLAdapter`](#classscy_1_1net_1_1SSLAdapter)` _sslAdapter` | 

## Members

#### `public  SSLSocket(uv::Loop * loop)` 





#### `public  SSLSocket(SSLContext::Ptr sslContext,uv::Loop * loop)` 





#### `public  SSLSocket(SSLContext::Ptr sslContext,SSLSession::Ptr session,uv::Loop * loop)` 





#### `public virtual  ~SSLSocket()` 





#### `public virtual bool shutdown()` 

Initialize the [SSLSocket](#classscy_1_1net_1_1SSLSocket) with the given [SSLContext](#classscy_1_1net_1_1SSLContext).

Initializes the socket and establishes a secure connection to the TCP server at the given address.

The SSL handshake is performed when the socket is connected. Shuts down the connection by attempting an orderly SSL shutdown, then actually shutting down the TCP connection.

#### `public virtual void close()` 

Closes the socket forcefully.



#### `public virtual int send(const char * data,std::size_t len,int flags)` 



Sends the given data buffer to the connected peer. Returns the number of bytes sent or -1 on error. No exception will be thrown. For TCP sockets the given peer address must match the connected peer address.

#### `public virtual int send(const char * data,std::size_t len,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress,int flags)` 





#### `public void useContext(SSLContext::Ptr context)` 

Use the given SSL context for this socket.



#### `public SSLContext::Ptr context() const` 

Returns the SSL context used for this socket.



#### `public void useSession(SSLSession::Ptr session)` 



Sets the SSL session to use for the next connection. Setting a previously saved Session object is necessary to enable session caching.

To remove the currently set session, a nullptr pointer can be given.

Must be called before [connect()](#group__net_1gac0e835a51a9e772b0d2f1e61cfddd87f) to be effective.

#### `public SSLSession::Ptr currentSession()` 



Returns the SSL session of the current connection, for reuse in a future connection (if session caching is enabled).

If no connection is established, returns nullptr.

#### `public bool sessionWasReused()` 



Returns true if a reused session was negotiated during the handshake.

#### `public int available() const` 



Returns the number of bytes available from the SSL buffer for immediate reading.

#### `public X509 * peerCertificate() const` 

Returns the peer's certificate.



#### `public virtual net::TransportType transport() const` 

The transport protocol: TCP, UDP or SSLTCP.



#### `public virtual void acceptConnection()` 





#### `public virtual void onConnect(uv_connect_t * handle,int status)` 





#### `public virtual void onRead(const char * data,std::size_t len)` 

Reads raw encrypted SSL data.



#### `protected net::SSLContext::Ptr _context` 

virtual bool readStart();



#### `protected net::SSLSession::Ptr _session` 





#### `protected `[`net::SSLAdapter`](#classscy_1_1net_1_1SSLAdapter)` _sslAdapter` 





# class `scy::net::TCPSocket` 

```
class scy::net::TCPSocket
  : public scy::Stream
  : public scy::net::Socket
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`Signal`](#classscy_1_1Signal)`< const net::TCPSocket::Ptr & > AcceptConnection` | 
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

#### `public `[`Signal`](#classscy_1_1Signal)`< const net::TCPSocket::Ptr & > AcceptConnection` 





#### `public  TCPSocket(uv::Loop * loop)` 





#### `public virtual  ~TCPSocket()` 





#### `public virtual bool shutdown()` 



Sends the shutdown packet which should result is socket closure via callback.

#### `public virtual void close()` 



Closes and resets the stream handle. This will close the active socket/pipe and destroy the uv_stream_t handle.

If the stream is already closed this call will have no side-effects.

#### `public virtual void connect(const `[`net::Address`](#classscy_1_1net_1_1Address)` & address)` 



Connects to the given peer IP address.

Throws an exception if the address is malformed. Connection errors can be handled via the [Error](#structscy_1_1Error) signal.

#### `public virtual int send(const char * data,std::size_t len,int flags)` 



Sends the given data buffer to the connected peer. Returns the number of bytes sent or -1 on error. No exception will be thrown. For TCP sockets the given peer address must match the connected peer address.

#### `public virtual int send(const char * data,std::size_t len,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress,int flags)` 





#### `public virtual void bind(const `[`net::Address`](#classscy_1_1net_1_1Address)` & address,unsigned flags)` 



Bind a local address to the socket. The address may be IPv4 or IPv6 (if supported).

Throws an Exception on error.

#### `public virtual void listen(int backlog)` 



Listens the socket on the given address.

Throws an Exception on error.

#### `public virtual void acceptConnection()` 





#### `public virtual void setNoDelay(bool enable)` 





#### `public virtual void setKeepAlive(int enable,unsigned int delay)` 





#### `public virtual uv::Loop * loop() const` 

Returns the socket event loop.



#### `public void setMode(`[`SocketMode`](#group__net_1gadfa90186218954a86b7d57d02c563da2)` mode)` 





#### `public const `[`SocketMode`](#group__net_1gadfa90186218954a86b7d57d02c563da2)` mode() const` 





#### `public virtual void setError(const `[`scy::Error`](#structscy_1_1Error)` & err)` 



Sets the socket error.

Setting the error will result in socket closure.

#### `public virtual const `[`scy::Error`](#structscy_1_1Error)` & error() const` 

Return the socket error if any.



#### `public virtual bool closed() const` 

Returns true if the native socket handle is closed.



#### `public virtual `[`net::Address`](#classscy_1_1net_1_1Address)` address() const` 



Returns the IP address and port number of the socket. A wildcard address is returned if the socket is not connected.

#### `public virtual `[`net::Address`](#classscy_1_1net_1_1Address)` peerAddress() const` 



Returns the IP address and port number of the peer socket. A wildcard address is returned if the socket is not connected.

#### `public virtual net::TransportType transport() const` 

Returns the TCP transport protocol.



#### `public virtual void onConnect(uv_connect_t * handle,int status)` 





#### `public virtual void onAcceptConnection(uv_stream_t * handle,int status)` 





#### `public virtual void onRead(const char * data,std::size_t len)` 





#### `public virtual void onRecv(const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buf)` 





#### `public virtual void onError(const `[`scy::Error`](#structscy_1_1Error)` &)` 



Override to handle errors. The error may be a UV error, or a custom error.

#### `public virtual void onClose()` 

Override to handle closure.



#### `protected uv_connect_t * _connectReq` 





#### `protected `[`SocketMode`](#group__net_1gadfa90186218954a86b7d57d02c563da2)` _mode` 





#### `protected virtual void init()` 

Initializes the underlying socket context.



# class `scy::net::Transaction` 

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
`protected `[`Address`](#classscy_1_1net_1_1Address)` _peerAddress` | 
`protected inline virtual  ~Transaction()` | 
`protected inline virtual void onPacket(`[`IPacket`](#classscy_1_1IPacket)` & packet)` | 
`protected inline virtual void onResponse()` | Called when a successful response match is received.
`protected inline virtual bool checkResponse(const PacketT & packet)` | 

## Members

#### `public inline  Transaction(const net::Socket::Ptr & socket,const `[`Address`](#classscy_1_1net_1_1Address)` & peerAddress,int timeout,int retries,uv::Loop * loop)` 





#### `public inline virtual bool send()` 



Starts the transaction timer and sends the request. Overriding classes should implement send logic here.

#### `public inline virtual void cancel()` 



Cancellation means that the agent will not retransmit the request, will not treat the lack of response to be a failure, but will wait the duration of the transaction timeout for a response.

#### `public inline virtual void dispose()` 



Schedules the transaction for deferred deletion.

It is safe to call this function while the transaction is still active, providing the call is made from the same loop thread which the timer is running on.

Protected by the base implementation as this is called by the internal state machine.

#### `public inline `[`Address`](#classscy_1_1net_1_1Address)` peerAddress() const` 





#### `protected `[`Address`](#classscy_1_1net_1_1Address)` _peerAddress` 





#### `protected inline virtual  ~Transaction()` 





#### `protected inline virtual void onPacket(`[`IPacket`](#classscy_1_1IPacket)` & packet)` 



Overrides the PacketSocketAdapter::onPacket callback for checking potential response candidates.

#### `protected inline virtual void onResponse()` 

Called when a successful response match is received.



#### `protected inline virtual bool checkResponse(const PacketT & packet)` 



Sub classes should derive this method to implement response checking logic. The base implementation only performs address matching.

# class `scy::net::UDPSocket` 

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
`protected `[`net::Address`](#classscy_1_1net_1_1Address)` _peer` | 
`protected Buffer _buffer` | 
`protected virtual void init()` | Initializes the underlying socket context.
`protected virtual bool recvStart()` | 
`protected virtual bool recvStop()` | 
`protected virtual void onError(const `[`scy::Error`](#structscy_1_1Error)` &)` | 
`protected virtual void onClose()` | Override to handle closure.

## Members

#### `public  UDPSocket(uv::Loop * loop)` 





#### `public virtual  ~UDPSocket()` 





#### `public virtual void connect(const `[`net::Address`](#classscy_1_1net_1_1Address)` & address)` 



Connects to the given peer IP address.

Throws an exception if the address is malformed. Connection errors can be handled via the [Error](#structscy_1_1Error) signal.

#### `public virtual void close()` 

Closes the underlying socket.



#### `public virtual void bind(const `[`net::Address`](#classscy_1_1net_1_1Address)` & address,unsigned flags)` 



Bind a local address to the socket. The address may be IPv4 or IPv6 (if supported).

Throws an Exception on error.

#### `public virtual int send(const char * data,std::size_t len,int flags)` 



Sends the given data buffer to the connected peer. Returns the number of bytes sent or -1 on error. No exception will be thrown. For TCP sockets the given peer address must match the connected peer address.

#### `public virtual int send(const char * data,std::size_t len,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress,int flags)` 





#### `public virtual bool setBroadcast(bool flag)` 





#### `public virtual bool setMulticastLoop(bool flag)` 





#### `public virtual bool setMulticastTTL(int ttl)` 





#### `public virtual `[`net::Address`](#classscy_1_1net_1_1Address)` address() const` 



The locally bound address.

This function will not throw. A Wildcard 0.0.0.0:0 address is returned if the socket is closed or invalid.

#### `public virtual `[`net::Address`](#classscy_1_1net_1_1Address)` peerAddress() const` 



The connected peer address.

This function will not throw. A Wildcard 0.0.0.0:0 address is returned if the socket is closed or invalid.

#### `public virtual net::TransportType transport() const` 

Returns the UDP transport protocol.



#### `public virtual void setError(const `[`scy::Error`](#structscy_1_1Error)` & err)` 



Sets the socket error.

Setting the error will result in socket closure.

#### `public virtual const `[`scy::Error`](#structscy_1_1Error)` & error() const` 

Return the socket error if any.



#### `public virtual bool closed() const` 



Returns true if the native socket handle is closed.

#### `public virtual uv::Loop * loop() const` 

Returns the socket event loop.



#### `public virtual void onRecv(const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buf,const `[`net::Address`](#classscy_1_1net_1_1Address)` & address)` 





#### `protected `[`net::Address`](#classscy_1_1net_1_1Address)` _peer` 





#### `protected Buffer _buffer` 





#### `protected virtual void init()` 

Initializes the underlying socket context.



#### `protected virtual bool recvStart()` 





#### `protected virtual bool recvStop()` 





#### `protected virtual void onError(const `[`scy::Error`](#structscy_1_1Error)` &)` 



Override to handle errors. The error may be a UV error, or a custom error.

#### `protected virtual void onClose()` 

Override to handle closure.



# class `scy::net::VerificationErrorDetails` 


A utility class for certificate error handling.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  VerificationErrorDetails(const `[`crypto::X509Certificate`](#classscy_1_1crypto_1_1X509Certificate)` & cert,int errDepth,int errNum,const std::string & errMsg)` | Creates the [VerificationErrorDetails](#classscy_1_1net_1_1VerificationErrorDetails). _ignoreError is per default set to false.
`public  ~VerificationErrorDetails()` | Destroys the [VerificationErrorDetails](#classscy_1_1net_1_1VerificationErrorDetails).
`public inline const `[`crypto::X509Certificate`](#classscy_1_1crypto_1_1X509Certificate)` & certificate() const` | Returns the certificate that caused the error.
`public inline int errorDepth() const` | Returns the position of the certificate in the certificate chain.
`public inline int errorNumber() const` | Returns the id of the error.
`public inline const std::string & errorMessage() const` | Returns the textual presentation of the errorNumber.
`public inline void setIgnoreError(bool ignoreError)` | setIgnoreError to true, if a verification error is judged non-fatal by the user.
`public inline bool getIgnoreError() const` | returns the value of _ignoreError

## Members

#### `public  VerificationErrorDetails(const `[`crypto::X509Certificate`](#classscy_1_1crypto_1_1X509Certificate)` & cert,int errDepth,int errNum,const std::string & errMsg)` 

Creates the [VerificationErrorDetails](#classscy_1_1net_1_1VerificationErrorDetails). _ignoreError is per default set to false.



#### `public  ~VerificationErrorDetails()` 

Destroys the [VerificationErrorDetails](#classscy_1_1net_1_1VerificationErrorDetails).



#### `public inline const `[`crypto::X509Certificate`](#classscy_1_1crypto_1_1X509Certificate)` & certificate() const` 

Returns the certificate that caused the error.



#### `public inline int errorDepth() const` 

Returns the position of the certificate in the certificate chain.



#### `public inline int errorNumber() const` 

Returns the id of the error.



#### `public inline const std::string & errorMessage() const` 

Returns the textual presentation of the errorNumber.



#### `public inline void setIgnoreError(bool ignoreError)` 

setIgnoreError to true, if a verification error is judged non-fatal by the user.



#### `public inline bool getIgnoreError() const` 

returns the value of _ignoreError



# struct `scy::net::DNSResult` 




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
`public `[`net::Address`](#classscy_1_1net_1_1Address)` addr` | The resolved address.
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

#### `public std::string host` 

The host to resolve.



#### `public std::uint16_t port` 

The host port to resolve.



#### `public `[`net::Address`](#classscy_1_1net_1_1Address)` addr` 

The resolved address.



#### `public struct addrinfo * info` 

The libuv uv_getaddrinfo result.



#### `public struct addrinfo * hints` 

libuv uv_getaddrinfo hints (optional)



#### `public std::function< void(const `[`DNSResult`](#structscy_1_1net_1_1DNSResult) &)`> callback` 

Result callback function.



#### `public void * opaque` 

Client data pointer.



#### `public enum scy::net::DNSResult::Status status` 





#### `public inline bool resolving() const` 





#### `public inline bool success() const` 





#### `public inline bool failed() const` 





#### `public inline bool complete() const` 





#### `public inline  DNSResult()` 





# struct `scy::net::PacketInfo` 

```
struct scy::net::PacketInfo
  : public scy::IPacketInfo
```  



Provides information about packets emitted from a socket. See [SocketPacket](#classscy_1_1net_1_1SocketPacket).

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public Socket::Ptr socket` | The source socket.
`public `[`Address`](#classscy_1_1net_1_1Address)` peerAddress` | 
`public inline  PacketInfo(const Socket::Ptr & socket,const `[`Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
`public inline  PacketInfo(const `[`PacketInfo`](#structscy_1_1net_1_1PacketInfo)` & r)` | 
`public inline virtual `[`IPacketInfo`](#structscy_1_1IPacketInfo)` * clone() const` | 
`public inline virtual  ~PacketInfo()` | 

## Members

#### `public Socket::Ptr socket` 

The source socket.



#### `public `[`Address`](#classscy_1_1net_1_1Address)` peerAddress` 



The originating peer address. For TCP this will always be connected address.

#### `public inline  PacketInfo(const Socket::Ptr & socket,const `[`Address`](#classscy_1_1net_1_1Address)` & peerAddress)` 





#### `public inline  PacketInfo(const `[`PacketInfo`](#structscy_1_1net_1_1PacketInfo)` & r)` 





#### `public inline virtual `[`IPacketInfo`](#structscy_1_1IPacketInfo)` * clone() const` 





#### `public inline virtual  ~PacketInfo()` 





# class `scy::net::Address` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Address()` | Creates a wildcard (all zero) IPv4 [Address](#classscy_1_1net_1_1Address).
`public  Address(const std::string & host,std::uint16_t port)` | 
`public  Address(const `[`Address`](#classscy_1_1net_1_1Address)` & addr)` | Creates a [Address](#classscy_1_1net_1_1Address) by copying another one.
`public  Address(const struct sockaddr * addr,socklen_t length)` | Creates a [Address](#classscy_1_1net_1_1Address) from a native socket address.
`public  Address(const std::string & host,const std::string & port)` | 
`public  explicit Address(const std::string & hostAndPort)` | 
`public  ~Address()` | Destroys the [Address](#classscy_1_1net_1_1Address).
`public `[`Address`](#classscy_1_1net_1_1Address)` & operator=(const `[`Address`](#classscy_1_1net_1_1Address)` & addr)` | Assigns another [Address](#classscy_1_1net_1_1Address).
`public void swap(`[`Address`](#classscy_1_1net_1_1Address)` & addr)` | Swaps the [Address](#classscy_1_1net_1_1Address) with another one.
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

#### `public  Address()` 

Creates a wildcard (all zero) IPv4 [Address](#classscy_1_1net_1_1Address).



#### `public  Address(const std::string & host,std::uint16_t port)` 



Creates a [Address](#classscy_1_1net_1_1Address) from an IP address and a port number.

The IP address must either be a domain name, or it must be in dotted decimal (IPv4) or hex string (IPv6) format.

#### `public  Address(const `[`Address`](#classscy_1_1net_1_1Address)` & addr)` 

Creates a [Address](#classscy_1_1net_1_1Address) by copying another one.



#### `public  Address(const struct sockaddr * addr,socklen_t length)` 

Creates a [Address](#classscy_1_1net_1_1Address) from a native socket address.



#### `public  Address(const std::string & host,const std::string & port)` 



Creates a [Address](#classscy_1_1net_1_1Address) from an IP address and a service name or port number.

The IP address must either be a domain name, or it must be in dotted decimal (IPv4) or hex string (IPv6) format.

The given port must either be a decimal port number, or a service name.

#### `public  explicit Address(const std::string & hostAndPort)` 



Creates a [Address](#classscy_1_1net_1_1Address) from an IP address or host name and a port number/service name. Host name/address and port number must be separated by a colon. In case of an IPv6 address, the address part must be enclosed in brackets.

Examples: 192.168.1.10:80

www.sourcey.com:8080

#### `public  ~Address()` 

Destroys the [Address](#classscy_1_1net_1_1Address).



#### `public `[`Address`](#classscy_1_1net_1_1Address)` & operator=(const `[`Address`](#classscy_1_1net_1_1Address)` & addr)` 

Assigns another [Address](#classscy_1_1net_1_1Address).



#### `public void swap(`[`Address`](#classscy_1_1net_1_1Address)` & addr)` 

Swaps the [Address](#classscy_1_1net_1_1Address) with another one.



#### `public std::string host() const` 

Returns the host IP address.



#### `public std::uint16_t port() const` 

Returns the port number.



#### `public socklen_t length() const` 

Returns the length of the internal native socket address.



#### `public const struct sockaddr * addr() const` 

Returns a pointer to the internal native socket address.



#### `public int af() const` 

Returns the address family (AF_INET or AF_INET6) of the address.



#### `public std::string toString() const` 

Returns a string representation of the address.



#### `public `[`Address::Family`](#group__net_1gaf91e77d155f2852974bbae5a2da34106)` family() const` 

Returns the address family of the host's address.



#### `public bool valid() const` 



Returns true when the port is set and the address is valid ie. not wildcard.

#### `public bool operator<(const `[`Address`](#classscy_1_1net_1_1Address)` & addr) const` 





#### `public bool operator==(const `[`Address`](#classscy_1_1net_1_1Address)` & addr) const` 





#### `public bool operator!=(const `[`Address`](#classscy_1_1net_1_1Address)` & addr) const` 





#### `protected void init(const std::string & host,std::uint16_t port)` 





# class `scy::net::PacketSocketAdapter` 

```
class scy::net::PacketSocketAdapter
  : public scy::net::SocketAdapter
  : public scy::SignalBase< PacketDelegateBase, IPacket & >
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public net::Socket::Ptr socket` | 
`public `[`PacketFactory`](#classscy_1_1PacketFactory)` factory` | 
`public  PacketSocketAdapter(const net::Socket::Ptr & socket)` | 
`public virtual void onSocketRecv(const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
`public virtual void onPacket(`[`IPacket`](#classscy_1_1IPacket)` & pkt)` | 

## Members

#### `public net::Socket::Ptr socket` 



Pointer to the underlying socket. Sent data will be proxied to this socket.

#### `public `[`PacketFactory`](#classscy_1_1PacketFactory)` factory` 





#### `public  PacketSocketAdapter(const net::Socket::Ptr & socket)` 



Creates the [PacketSocketAdapter](#classscy_1_1net_1_1PacketSocketAdapter) This class should have a higher priority than standard sockets so we can parse data packets first. Creates and dispatches a packet utilizing the available creation strategies. For best performance the most used strategies should have the highest priority.

#### `public virtual void onSocketRecv(const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`Address`](#classscy_1_1net_1_1Address)` & peerAddress)` 





#### `public virtual void onPacket(`[`IPacket`](#classscy_1_1IPacket)` & pkt)` 





# class `scy::net::Socket` 

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

#### `public  Socket()` 





#### `public virtual  ~Socket()` 





#### `public void connect(const `[`Address`](#classscy_1_1net_1_1Address)` & address)` 



Connects to the given peer IP address.

Throws an exception if the address is malformed. Connection errors can be handled via the [Error](#structscy_1_1Error) signal.

#### `public virtual void connect(const std::string & host,std::uint16_t port)` 



Resolves and connects to the given host address.

Throws an Exception if the host is malformed. Since the DNS callback is asynchronous implementations need to listen for the [Error](#structscy_1_1Error) signal for handling connection errors.

#### `public void bind(const `[`Address`](#classscy_1_1net_1_1Address)` & address,unsigned flags)` 



Bind a local address to the socket. The address may be IPv4 or IPv6 (if supported).

Throws an Exception on error.

#### `public inline virtual void listen(int backlog)` 



Listens the socket on the given address.

Throws an Exception on error.

#### `public inline virtual bool shutdown()` 



Sends the shutdown packet which should result is socket closure via callback.

#### `public void close()` 

Closes the underlying socket.



#### `public `[`Address`](#classscy_1_1net_1_1Address)` address() const` 



The locally bound address.

This function will not throw. A Wildcard 0.0.0.0:0 address is returned if the socket is closed or invalid.

#### `public `[`Address`](#classscy_1_1net_1_1Address)` peerAddress() const` 



The connected peer address.

This function will not throw. A Wildcard 0.0.0.0:0 address is returned if the socket is closed or invalid.

#### `public net::TransportType transport() const` 

The transport protocol: TCP, UDP or SSLTCP.



#### `public void setError(const `[`scy::Error`](#structscy_1_1Error)` & err)` 



Sets the socket error.

Setting the error will result in socket closure.

#### `public const `[`scy::Error`](#structscy_1_1Error)` & error() const` 

Return the socket error if any.



#### `public bool closed() const` 

Returns true if the native socket handle is closed.



#### `public uv::Loop * loop() const` 

Returns the socket event loop.



#### `protected void init()` 

Initializes the underlying socket context.



#### `protected inline virtual void reset()` 

Resets the socket context for reuse.



#### `protected inline virtual void * self()` 



Returns the derived instance pointer for casting [SocketAdapter](#classscy_1_1net_1_1SocketAdapter) signal callback sender arguments from void* to [Socket](#classscy_1_1net_1_1Socket). Note: This method must not be derived by subclasses or casting will fail for void* pointer callbacks.

# class `scy::net::SocketPacket` 

```
class scy::net::SocketPacket
  : public scy::RawPacket
```  



[SocketPacket](#classscy_1_1net_1_1SocketPacket) is the default packet type emitted by sockets. [SocketPacket](#classscy_1_1net_1_1SocketPacket) provides peer address information and a buffer reference for nocopy binary operations.

The referenced packet buffer lifetime is only guaranteed for the duration of the receiver callback.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`PacketInfo`](#structscy_1_1net_1_1PacketInfo)` * info` | [PacketInfo](#structscy_1_1net_1_1PacketInfo) pointer.
`public inline  SocketPacket(const Socket::Ptr & socket,const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
`public inline  SocketPacket(const `[`SocketPacket`](#classscy_1_1net_1_1SocketPacket)` & that)` | 
`public inline virtual  ~SocketPacket()` | 
`public inline virtual void print(std::ostream & os) const` | 
`public inline virtual `[`IPacket`](#classscy_1_1IPacket)` * clone() const` | 
`public inline virtual std::size_t read(const `[`ConstBuffer`](#classscy_1_1ConstBuffer)` &)` | 
`public inline virtual void write(Buffer &) const` | 
`public inline virtual const char * className() const` | 

## Members

#### `public `[`PacketInfo`](#structscy_1_1net_1_1PacketInfo)` * info` 

[PacketInfo](#structscy_1_1net_1_1PacketInfo) pointer.



#### `public inline  SocketPacket(const Socket::Ptr & socket,const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`Address`](#classscy_1_1net_1_1Address)` & peerAddress)` 





#### `public inline  SocketPacket(const `[`SocketPacket`](#classscy_1_1net_1_1SocketPacket)` & that)` 





#### `public inline virtual  ~SocketPacket()` 





#### `public inline virtual void print(std::ostream & os) const` 





#### `public inline virtual `[`IPacket`](#classscy_1_1IPacket)` * clone() const` 





#### `public inline virtual std::size_t read(const `[`ConstBuffer`](#classscy_1_1ConstBuffer)` &)` 



Read/parse to the packet from the given input buffer. The number of bytes read is returned.

#### `public inline virtual void write(Buffer &) const` 



Copy/generate to the packet given output buffer. The number of bytes written can be obtained from the buffer.

Todo: It may be prefferable to use our pod types here instead of buffer input, but the current codebase requires that the buffer be dynamically resizable for some protocols...

virtual std::size_t write(MutableBuffer&) const = 0;

#### `public inline virtual const char * className() const` 





# class `scy::net::SocketAdapter` 




[SocketAdapter](#classscy_1_1net_1_1SocketAdapter) is the abstract interface for all socket classes. A [SocketAdapter](#classscy_1_1net_1_1SocketAdapter) can also be attached to a [Socket](#classscy_1_1net_1_1Socket) in order to override default [Socket](#classscy_1_1net_1_1Socket) callbacks and behaviour, while still maintaining the default [Socket](#classscy_1_1net_1_1Socket) interface (see Socket::setAdapter).

This class also be extended to implement custom processing for received socket data before it is dispatched to the application (see [PacketSocketAdapter](#classscy_1_1net_1_1PacketSocketAdapter) and [Transaction](#classscy_1_1net_1_1Transaction) classes).

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public void * opaque` | 
`public `[`NullSignal`](#classscy_1_1NullSignal)` Connect` | Signals that the socket is connected.
`public `[`Signal2](#classscy_1_1Signal2)< const [MutableBuffer](#classscy_1_1MutableBuffer) &, const [Address`](#classscy_1_1net_1_1Address)` & > Recv` | Signals when data is received by the socket.
`public `[`Signal](#classscy_1_1Signal)< const [scy::Error`](#structscy_1_1Error)` & > Error` | 
`public `[`NullSignal`](#classscy_1_1NullSignal)` Close` | Signals that the underlying socket is closed.
`public  SocketAdapter(`[`SocketAdapter`](#classscy_1_1net_1_1SocketAdapter)` * sender,`[`SocketAdapter`](#classscy_1_1net_1_1SocketAdapter)` * receiver)` | Creates the [SocketAdapter](#classscy_1_1net_1_1SocketAdapter).
`public virtual  ~SocketAdapter()` | Destroys the [SocketAdapter](#classscy_1_1net_1_1SocketAdapter).
`public virtual int send(const char * data,std::size_t len,int flags)` | 
`public virtual int send(const char * data,std::size_t len,const `[`Address`](#classscy_1_1net_1_1Address)` & peerAddress,int flags)` | 
`public virtual int sendPacket(const `[`IPacket`](#classscy_1_1IPacket)` & packet,int flags)` | 
`public virtual int sendPacket(const `[`IPacket`](#classscy_1_1IPacket)` & packet,const `[`Address`](#classscy_1_1net_1_1Address)` & peerAddress,int flags)` | 
`public virtual void sendPacket(`[`IPacket`](#classscy_1_1IPacket)` & packet)` | 
`public virtual void onSocketConnect()` | 
`public virtual void onSocketRecv(const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
`public virtual void onSocketError(const `[`Error`](#structscy_1_1Error)` & error)` | 
`public virtual void onSocketClose()` | 
`public void setSender(`[`SocketAdapter`](#classscy_1_1net_1_1SocketAdapter)` * adapter,bool freeExisting)` | 
`public `[`SocketAdapter`](#classscy_1_1net_1_1SocketAdapter)` * sender()` | Returns the output [SocketAdapter](#classscy_1_1net_1_1SocketAdapter) pointer.
`public void addReceiver(`[`SocketAdapter`](#classscy_1_1net_1_1SocketAdapter)` * adapter,int priority)` | Adds an input [SocketAdapter](#classscy_1_1net_1_1SocketAdapter) for receiving socket callbacks.
`public void removeReceiver(`[`SocketAdapter`](#classscy_1_1net_1_1SocketAdapter)` * adapter)` | Removes an input [SocketAdapter](#classscy_1_1net_1_1SocketAdapter).
`protected `[`SocketAdapter`](#classscy_1_1net_1_1SocketAdapter)` * _sender` | 
`protected inline virtual void * self()` | 
`protected virtual void emitSocketConnect()` | 
`protected virtual void emitSocketRecv(const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
`protected virtual void emitSocketError(const `[`scy::Error`](#structscy_1_1Error)` & error)` | 
`protected virtual void emitSocketClose()` | 

## Members

#### `public void * opaque` 



Optional client data pointer.

The pointer is not initialized or managed by the socket base.

#### `public `[`NullSignal`](#classscy_1_1NullSignal)` Connect` 

Signals that the socket is connected.



#### `public `[`Signal2](#classscy_1_1Signal2)< const [MutableBuffer](#classscy_1_1MutableBuffer) &, const [Address`](#classscy_1_1net_1_1Address)` & > Recv` 

Signals when data is received by the socket.



#### `public `[`Signal](#classscy_1_1Signal)< const [scy::Error`](#structscy_1_1Error)` & > Error` 



Signals that the socket is closed in error. This signal will be sent just before the Closed signal.

#### `public `[`NullSignal`](#classscy_1_1NullSignal)` Close` 

Signals that the underlying socket is closed.



#### `public  SocketAdapter(`[`SocketAdapter`](#classscy_1_1net_1_1SocketAdapter)` * sender,`[`SocketAdapter`](#classscy_1_1net_1_1SocketAdapter)` * receiver)` 

Creates the [SocketAdapter](#classscy_1_1net_1_1SocketAdapter).



#### `public virtual  ~SocketAdapter()` 

Destroys the [SocketAdapter](#classscy_1_1net_1_1SocketAdapter).



#### `public virtual int send(const char * data,std::size_t len,int flags)` 



Sends the given data buffer to the connected peer. Returns the number of bytes sent or -1 on error. No exception will be thrown. For TCP sockets the given peer address must match the connected peer address.

#### `public virtual int send(const char * data,std::size_t len,const `[`Address`](#classscy_1_1net_1_1Address)` & peerAddress,int flags)` 





#### `public virtual int sendPacket(const `[`IPacket`](#classscy_1_1IPacket)` & packet,int flags)` 



Sends the given packet to the connected peer. Returns the number of bytes sent or -1 on error. No exception will be thrown. For TCP sockets the given peer address must match the connected peer address.

#### `public virtual int sendPacket(const `[`IPacket`](#classscy_1_1IPacket)` & packet,const `[`Address`](#classscy_1_1net_1_1Address)` & peerAddress,int flags)` 





#### `public virtual void sendPacket(`[`IPacket`](#classscy_1_1IPacket)` & packet)` 



Sends the given packet to the connected peer. This method provides delegate compatability, and unlike other send methods throws an exception if the underlying socket is closed.

#### `public virtual void onSocketConnect()` 



These virtual methods can be overridden as necessary to intercept socket events before they hit the application.

#### `public virtual void onSocketRecv(const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`Address`](#classscy_1_1net_1_1Address)` & peerAddress)` 





#### `public virtual void onSocketError(const `[`Error`](#structscy_1_1Error)` & error)` 





#### `public virtual void onSocketClose()` 





#### `public void setSender(`[`SocketAdapter`](#classscy_1_1net_1_1SocketAdapter)` * adapter,bool freeExisting)` 



A pointer to the adapter for handling outgoing data. Send methods proxy data to this adapter by default. Note that we only keep a simple pointer so as to avoid circular references preventing destruction.

#### `public `[`SocketAdapter`](#classscy_1_1net_1_1SocketAdapter)` * sender()` 

Returns the output [SocketAdapter](#classscy_1_1net_1_1SocketAdapter) pointer.



#### `public void addReceiver(`[`SocketAdapter`](#classscy_1_1net_1_1SocketAdapter)` * adapter,int priority)` 

Adds an input [SocketAdapter](#classscy_1_1net_1_1SocketAdapter) for receiving socket callbacks.



#### `public void removeReceiver(`[`SocketAdapter`](#classscy_1_1net_1_1SocketAdapter)` * adapter)` 

Removes an input [SocketAdapter](#classscy_1_1net_1_1SocketAdapter).



#### `protected `[`SocketAdapter`](#classscy_1_1net_1_1SocketAdapter)` * _sender` 





#### `protected inline virtual void * self()` 



Returns the polymorphic instance pointer for signal delegate callbacks.

#### `protected virtual void emitSocketConnect()` 





#### `protected virtual void emitSocketRecv(const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`Address`](#classscy_1_1net_1_1Address)` & peerAddress)` 





#### `protected virtual void emitSocketError(const `[`scy::Error`](#structscy_1_1Error)` & error)` 





#### `protected virtual void emitSocketClose()` 





# class `scy::net::SSLAdapter` 






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
`protected `[`net::SSLSocket`](#classscy_1_1net_1_1SSLSocket)` * _socket` | 
`protected SSL * _ssl` | 
`protected BIO * _readBIO` | The incoming buffer we write encrypted SSL data into.
`protected BIO * _writeBIO` | The outgoing buffer we write to the socket.
`protected std::vector< char > _bufferOut` | The outgoing payload to be encrypted and sent.
`protected void handleError(int rc)` | 
`protected void flushReadBIO()` | 
`protected void flushWriteBIO()` | 

## Members

#### `public  SSLAdapter(`[`net::SSLSocket`](#classscy_1_1net_1_1SSLSocket)` * socket)` 





#### `public  ~SSLAdapter()` 





#### `public void initClient()` 

Initializes the SSL context as a client.



#### `public void initServer()` 

Initializes the SSL context as a server.



#### `public bool initialized() const` 

Returns true when SSL context has been initialized.



#### `public bool ready() const` 

Returns true when the handshake is complete.



#### `public void handshake()` 

Start/continue the SSL handshake process.



#### `public int available() const` 



Returns the number of bytes available in the SSL buffer for immediate reading.

#### `public void shutdown()` 

Issues an orderly SSL shutdown.



#### `public void flush()` 

Flushes the SSL read/write buffers.



#### `public void addIncomingData(const char * data,std::size_t len)` 





#### `public void addOutgoingData(const std::string & data)` 





#### `public void addOutgoingData(const char * data,std::size_t len)` 





#### `protected `[`net::SSLSocket`](#classscy_1_1net_1_1SSLSocket)` * _socket` 





#### `protected SSL * _ssl` 





#### `protected BIO * _readBIO` 

The incoming buffer we write encrypted SSL data into.



#### `protected BIO * _writeBIO` 

The outgoing buffer we write to the socket.



#### `protected std::vector< char > _bufferOut` 

The outgoing payload to be encrypted and sent.



#### `protected void handleError(int rc)` 





#### `protected void flushReadBIO()` 





#### `protected void flushWriteBIO()` 





# class `scy::net::SSLContext` 

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

#### `public  SSLContext(`[`Usage`](#group__net_1ga2e13fff7d4c75c080e515a427bf15c71)` usage,const std::string & privateKeyFile,const std::string & certificateFile,const std::string & caLocation,`[`VerificationMode`](#group__net_1gafd7da63f7077511ce8d4da68f37d255e)` verificationMode,int verificationDepth,bool loadDefaultCAs,const std::string & cipherList)` 



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

#### `public  SSLContext(`[`Usage`](#group__net_1ga2e13fff7d4c75c080e515a427bf15c71)` usage,const std::string & caLocation,`[`VerificationMode`](#group__net_1gafd7da63f7077511ce8d4da68f37d255e)` verificationMode,int verificationDepth,bool loadDefaultCAs,const std::string & cipherList)` 



Creates a Context.



* usage specifies whether the context is used by a client or server.


* caLocation contains the path to the file or directory containing the CA/root certificates. Can be empty if the OpenSSL builtin CA certificates are used (see loadDefaultCAs).


* verificationMode specifies whether and how peer certificates are validated.


* verificationDepth sets the upper limit for verification chain sizes. Verification will fail if a certificate chain larger than this is encountered.


* loadDefaultCAs specifies weather the builtin CA certificates from OpenSSL are used.


* cipherList specifies the supported ciphers in OpenSSL notation.





Note that a private key and/or certificate must be specified with usePrivateKey()/useCertificate() before the Context can be used.

#### `public  ~SSLContext()` 

Destroys the Context.



#### `public void useCertificate(const `[`crypto::X509Certificate`](#classscy_1_1crypto_1_1X509Certificate)` & certificate)` 



Sets the certificate to be used by the Context.

To set-up a complete certificate chain, it might be necessary to call [addChainCertificate()](#group__net_1gac4fe7dff44de813dcdf7daf80ea48d5f) to specify additional certificates.

Note that [useCertificate()](#group__net_1ga840711ee078f8ee6432aaef8b50b48b9) must always be called before usePrivateKey().

#### `public void addChainCertificate(const `[`crypto::X509Certificate`](#classscy_1_1crypto_1_1X509Certificate)` & certificate)` 

Adds a certificate for certificate chain validation.



#### `public void addVerificationCertificate(const `[`crypto::X509Certificate`](#classscy_1_1crypto_1_1X509Certificate)` & certificate)` 



Sets the private key to be used by the Context.

Note that [useCertificate()](#group__net_1ga840711ee078f8ee6432aaef8b50b48b9) must always be called before usePrivateKey().

Note: If the private key is protected by a passphrase, a PrivateKeyPassphraseHandler must have been setup with the [SSLManager](#classscy_1_1net_1_1SSLManager), or the [SSLManager](#classscy_1_1net_1_1SSLManager)'s PrivateKeyPassphraseRequired event must be handled. Adds the given certificate to the list of trusted certificates that will be used for verification.

#### `public inline SSL_CTX * sslContext() const` 

Returns the underlying OpenSSL SSL Context object.



#### `public inline `[`Usage`](#group__net_1ga2e13fff7d4c75c080e515a427bf15c71)` usage() const` 



Returns whether the context is for use by a client or by a server and whether TLSv1 is required.

#### `public inline bool isForServerUse() const` 

Returns true if the context is for use by a server.



#### `public inline `[`SSLContext::VerificationMode`](#group__net_1gafd7da63f7077511ce8d4da68f37d255e)` verificationMode() const` 

Returns the verification mode.



#### `public void enableSessionCache(bool flag)` 



Enable or disable SSL/TLS session caching. For session caching to work, it must be enabled on the server, as well as on the client side.

The default is disabled session caching.

To enable session caching on the server side, use the two-argument version of this method to specify a session ID context.

#### `public void enableSessionCache(bool flag,const std::string & sessionIdContext)` 



Enables or disables SSL/TLS session caching on the server. For session caching to work, it must be enabled on the server, as well as on the client side.

SessionIdContext contains the application's unique session ID context, which becomes part of each session identifier generated by the server within this context. SessionIdContext can be an arbitrary sequence of bytes with a maximum length of SSL_MAX_SSL_SESSION_ID_LENGTH.

A non-empty sessionIdContext should be specified even if session caching is disabled to avoid problems with clients requesting to reuse a session (e.g. Firefox 3.6).

This method may only be called on SERVER_USE Context objects.

#### `public bool sessionCacheEnabled() const` 

Returns true if the session cache is enabled.



#### `public void setSessionCacheSize(std::size_t size)` 



Sets the maximum size of the server session cache, in number of sessions. The default size (according to OpenSSL documentation) is 1024*20, which may be too large for many applications, especially on embedded platforms with limited memory.

Specifying a size of 0 will set an unlimited cache size.

This method may only be called on SERVER_USE Context objects.

#### `public std::size_t getSessionCacheSize() const` 



Returns the current maximum size of the server session cache.

This method may only be called on SERVER_USE Context objects.

#### `public void setSessionTimeout(long seconds)` 



Sets the timeout (in seconds) of cached sessions on the server. A cached session will be removed from the cache if it has not been used for the given number of seconds.

This method may only be called on SERVER_USE Context objects.

#### `public long getSessionTimeout() const` 



Returns the timeout (in seconds) of cached sessions on the server.

This method may only be called on SERVER_USE Context objects.

#### `public void flushSessionCache()` 



Flushes the SSL session cache on the server.

This method may only be called on SERVER_USE Context objects.

#### `public void disableStatelessSessionResumption()` 



Newer versions of OpenSSL support RFC 4507 tickets for stateless session resumption.

The feature can be disabled by calling this method.

# class `scy::net::SSLManager` 




[SSLManager](#classscy_1_1net_1_1SSLManager) is a singleton for holding the default server/client Context and handling callbacks for certificate verification errors and private key passphrases.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`Signal](#classscy_1_1Signal)< [VerificationErrorDetails`](#classscy_1_1net_1_1VerificationErrorDetails)` & > ServerVerificationError` | Fired whenever a certificate verification error is detected by the server during a handshake.
`public `[`Signal](#classscy_1_1Signal)< [VerificationErrorDetails`](#classscy_1_1net_1_1VerificationErrorDetails)` & > ClientVerificationError` | Fired whenever a certificate verification error is detected by the client during a handshake.
`public `[`Signal`](#classscy_1_1Signal)`< std::string & > PrivateKeyPassphraseRequired` | 
`public void initializeServer(SSLContext::Ptr ptrContext)` | Initializes the server side of the [SSLManager](#classscy_1_1net_1_1SSLManager) server-side [SSLContext](#classscy_1_1net_1_1SSLContext).
`public void initializeClient(SSLContext::Ptr ptrContext)` | Initializes the client side of the [SSLManager](#classscy_1_1net_1_1SSLManager) with a default client-side [SSLContext](#classscy_1_1net_1_1SSLContext).
`public SSLContext::Ptr defaultServerContext()` | Returns the default Context used by the server if initialized.
`public SSLContext::Ptr defaultClientContext()` | Returns the default Context used by the client if initialized.
`public void shutdown()` | 

## Members

#### `public `[`Signal](#classscy_1_1Signal)< [VerificationErrorDetails`](#classscy_1_1net_1_1VerificationErrorDetails)` & > ServerVerificationError` 

Fired whenever a certificate verification error is detected by the server during a handshake.



#### `public `[`Signal](#classscy_1_1Signal)< [VerificationErrorDetails`](#classscy_1_1net_1_1VerificationErrorDetails)` & > ClientVerificationError` 

Fired whenever a certificate verification error is detected by the client during a handshake.



#### `public `[`Signal`](#classscy_1_1Signal)`< std::string & > PrivateKeyPassphraseRequired` 



Fired when a encrypted certificate is loaded. Not setting the password in the event parameter will result in a failure to load the certificate.

#### `public void initializeServer(SSLContext::Ptr ptrContext)` 

Initializes the server side of the [SSLManager](#classscy_1_1net_1_1SSLManager) server-side [SSLContext](#classscy_1_1net_1_1SSLContext).



#### `public void initializeClient(SSLContext::Ptr ptrContext)` 

Initializes the client side of the [SSLManager](#classscy_1_1net_1_1SSLManager) with a default client-side [SSLContext](#classscy_1_1net_1_1SSLContext).



#### `public SSLContext::Ptr defaultServerContext()` 

Returns the default Context used by the server if initialized.



#### `public SSLContext::Ptr defaultClientContext()` 

Returns the default Context used by the client if initialized.



#### `public void shutdown()` 



Shuts down the [SSLManager](#classscy_1_1net_1_1SSLManager) and releases the default Context objects. After a call to [shutdown()](#group__net_1ga4061d56eaf906bf79a0713310a3841c5), the [SSLManager](#classscy_1_1net_1_1SSLManager) can no longer be used.

Normally, it's not necessary to call this method directly, as this will be called either by uninitializeSSL(), or when the [SSLManager](#classscy_1_1net_1_1SSLManager) instance is destroyed.

# class `scy::net::VerificationErrorDetails` 


A utility class for certificate error handling.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  VerificationErrorDetails(const `[`crypto::X509Certificate`](#classscy_1_1crypto_1_1X509Certificate)` & cert,int errDepth,int errNum,const std::string & errMsg)` | Creates the [VerificationErrorDetails](#classscy_1_1net_1_1VerificationErrorDetails). _ignoreError is per default set to false.
`public  ~VerificationErrorDetails()` | Destroys the [VerificationErrorDetails](#classscy_1_1net_1_1VerificationErrorDetails).
`public inline const `[`crypto::X509Certificate`](#classscy_1_1crypto_1_1X509Certificate)` & certificate() const` | Returns the certificate that caused the error.
`public inline int errorDepth() const` | Returns the position of the certificate in the certificate chain.
`public inline int errorNumber() const` | Returns the id of the error.
`public inline const std::string & errorMessage() const` | Returns the textual presentation of the errorNumber.
`public inline void setIgnoreError(bool ignoreError)` | setIgnoreError to true, if a verification error is judged non-fatal by the user.
`public inline bool getIgnoreError() const` | returns the value of _ignoreError

## Members

#### `public  VerificationErrorDetails(const `[`crypto::X509Certificate`](#classscy_1_1crypto_1_1X509Certificate)` & cert,int errDepth,int errNum,const std::string & errMsg)` 

Creates the [VerificationErrorDetails](#classscy_1_1net_1_1VerificationErrorDetails). _ignoreError is per default set to false.



#### `public  ~VerificationErrorDetails()` 

Destroys the [VerificationErrorDetails](#classscy_1_1net_1_1VerificationErrorDetails).



#### `public inline const `[`crypto::X509Certificate`](#classscy_1_1crypto_1_1X509Certificate)` & certificate() const` 

Returns the certificate that caused the error.



#### `public inline int errorDepth() const` 

Returns the position of the certificate in the certificate chain.



#### `public inline int errorNumber() const` 

Returns the id of the error.



#### `public inline const std::string & errorMessage() const` 

Returns the textual presentation of the errorNumber.



#### `public inline void setIgnoreError(bool ignoreError)` 

setIgnoreError to true, if a verification error is judged non-fatal by the user.



#### `public inline bool getIgnoreError() const` 

returns the value of _ignoreError



# class `scy::net::SSLSession` 

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

#### `public SSL_SESSION * sslSession() const` 

Returns the stored OpenSSL SSL_SESSION object.



#### `public  SSLSession(SSL_SESSION * ptr)` 



Creates a new Session object, using the given SSL_SESSION object.

The SSL_SESSION's reference count is not changed.

#### `public  ~SSLSession()` 



Destroys the Session.

Calls SSL_SESSION_free() on the stored SSL_SESSION object.

#### `public  SSLSession()` 





#### `protected SSL_SESSION * _ptr` 





# class `scy::net::SSLSocket` 

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
`protected `[`net::SSLAdapter`](#classscy_1_1net_1_1SSLAdapter)` _sslAdapter` | 

## Members

#### `public  SSLSocket(uv::Loop * loop)` 





#### `public  SSLSocket(SSLContext::Ptr sslContext,uv::Loop * loop)` 





#### `public  SSLSocket(SSLContext::Ptr sslContext,SSLSession::Ptr session,uv::Loop * loop)` 





#### `public virtual  ~SSLSocket()` 





#### `public virtual bool shutdown()` 

Initialize the [SSLSocket](#classscy_1_1net_1_1SSLSocket) with the given [SSLContext](#classscy_1_1net_1_1SSLContext).

Initializes the socket and establishes a secure connection to the TCP server at the given address.

The SSL handshake is performed when the socket is connected. Shuts down the connection by attempting an orderly SSL shutdown, then actually shutting down the TCP connection.

#### `public virtual void close()` 

Closes the socket forcefully.



#### `public virtual int send(const char * data,std::size_t len,int flags)` 



Sends the given data buffer to the connected peer. Returns the number of bytes sent or -1 on error. No exception will be thrown. For TCP sockets the given peer address must match the connected peer address.

#### `public virtual int send(const char * data,std::size_t len,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress,int flags)` 





#### `public void useContext(SSLContext::Ptr context)` 

Use the given SSL context for this socket.



#### `public SSLContext::Ptr context() const` 

Returns the SSL context used for this socket.



#### `public void useSession(SSLSession::Ptr session)` 



Sets the SSL session to use for the next connection. Setting a previously saved Session object is necessary to enable session caching.

To remove the currently set session, a nullptr pointer can be given.

Must be called before [connect()](#group__net_1gac0e835a51a9e772b0d2f1e61cfddd87f) to be effective.

#### `public SSLSession::Ptr currentSession()` 



Returns the SSL session of the current connection, for reuse in a future connection (if session caching is enabled).

If no connection is established, returns nullptr.

#### `public bool sessionWasReused()` 



Returns true if a reused session was negotiated during the handshake.

#### `public int available() const` 



Returns the number of bytes available from the SSL buffer for immediate reading.

#### `public X509 * peerCertificate() const` 

Returns the peer's certificate.



#### `public virtual net::TransportType transport() const` 

The transport protocol: TCP, UDP or SSLTCP.



#### `public virtual void acceptConnection()` 





#### `public virtual void onConnect(uv_connect_t * handle,int status)` 





#### `public virtual void onRead(const char * data,std::size_t len)` 

Reads raw encrypted SSL data.



#### `protected net::SSLContext::Ptr _context` 

virtual bool readStart();



#### `protected net::SSLSession::Ptr _session` 





#### `protected `[`net::SSLAdapter`](#classscy_1_1net_1_1SSLAdapter)` _sslAdapter` 





# class `scy::net::TCPSocket` 

```
class scy::net::TCPSocket
  : public scy::Stream
  : public scy::net::Socket
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`Signal`](#classscy_1_1Signal)`< const net::TCPSocket::Ptr & > AcceptConnection` | 
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

#### `public `[`Signal`](#classscy_1_1Signal)`< const net::TCPSocket::Ptr & > AcceptConnection` 





#### `public  TCPSocket(uv::Loop * loop)` 





#### `public virtual  ~TCPSocket()` 





#### `public virtual bool shutdown()` 



Sends the shutdown packet which should result is socket closure via callback.

#### `public virtual void close()` 



Closes and resets the stream handle. This will close the active socket/pipe and destroy the uv_stream_t handle.

If the stream is already closed this call will have no side-effects.

#### `public virtual void connect(const `[`net::Address`](#classscy_1_1net_1_1Address)` & address)` 



Connects to the given peer IP address.

Throws an exception if the address is malformed. Connection errors can be handled via the [Error](#structscy_1_1Error) signal.

#### `public virtual int send(const char * data,std::size_t len,int flags)` 



Sends the given data buffer to the connected peer. Returns the number of bytes sent or -1 on error. No exception will be thrown. For TCP sockets the given peer address must match the connected peer address.

#### `public virtual int send(const char * data,std::size_t len,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress,int flags)` 





#### `public virtual void bind(const `[`net::Address`](#classscy_1_1net_1_1Address)` & address,unsigned flags)` 



Bind a local address to the socket. The address may be IPv4 or IPv6 (if supported).

Throws an Exception on error.

#### `public virtual void listen(int backlog)` 



Listens the socket on the given address.

Throws an Exception on error.

#### `public virtual void acceptConnection()` 





#### `public virtual void setNoDelay(bool enable)` 





#### `public virtual void setKeepAlive(int enable,unsigned int delay)` 





#### `public virtual uv::Loop * loop() const` 

Returns the socket event loop.



#### `public void setMode(`[`SocketMode`](#group__net_1gadfa90186218954a86b7d57d02c563da2)` mode)` 





#### `public const `[`SocketMode`](#group__net_1gadfa90186218954a86b7d57d02c563da2)` mode() const` 





#### `public virtual void setError(const `[`scy::Error`](#structscy_1_1Error)` & err)` 



Sets the socket error.

Setting the error will result in socket closure.

#### `public virtual const `[`scy::Error`](#structscy_1_1Error)` & error() const` 

Return the socket error if any.



#### `public virtual bool closed() const` 

Returns true if the native socket handle is closed.



#### `public virtual `[`net::Address`](#classscy_1_1net_1_1Address)` address() const` 



Returns the IP address and port number of the socket. A wildcard address is returned if the socket is not connected.

#### `public virtual `[`net::Address`](#classscy_1_1net_1_1Address)` peerAddress() const` 



Returns the IP address and port number of the peer socket. A wildcard address is returned if the socket is not connected.

#### `public virtual net::TransportType transport() const` 

Returns the TCP transport protocol.



#### `public virtual void onConnect(uv_connect_t * handle,int status)` 





#### `public virtual void onAcceptConnection(uv_stream_t * handle,int status)` 





#### `public virtual void onRead(const char * data,std::size_t len)` 





#### `public virtual void onRecv(const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buf)` 





#### `public virtual void onError(const `[`scy::Error`](#structscy_1_1Error)` &)` 



Override to handle errors. The error may be a UV error, or a custom error.

#### `public virtual void onClose()` 

Override to handle closure.



#### `protected uv_connect_t * _connectReq` 





#### `protected `[`SocketMode`](#group__net_1gadfa90186218954a86b7d57d02c563da2)` _mode` 





#### `protected virtual void init()` 

Initializes the underlying socket context.



# class `scy::net::Transaction` 

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
`protected `[`Address`](#classscy_1_1net_1_1Address)` _peerAddress` | 
`protected inline virtual  ~Transaction()` | 
`protected inline virtual void onPacket(`[`IPacket`](#classscy_1_1IPacket)` & packet)` | 
`protected inline virtual void onResponse()` | Called when a successful response match is received.
`protected inline virtual bool checkResponse(const PacketT & packet)` | 

## Members

#### `public inline  Transaction(const net::Socket::Ptr & socket,const `[`Address`](#classscy_1_1net_1_1Address)` & peerAddress,int timeout,int retries,uv::Loop * loop)` 





#### `public inline virtual bool send()` 



Starts the transaction timer and sends the request. Overriding classes should implement send logic here.

#### `public inline virtual void cancel()` 



Cancellation means that the agent will not retransmit the request, will not treat the lack of response to be a failure, but will wait the duration of the transaction timeout for a response.

#### `public inline virtual void dispose()` 



Schedules the transaction for deferred deletion.

It is safe to call this function while the transaction is still active, providing the call is made from the same loop thread which the timer is running on.

Protected by the base implementation as this is called by the internal state machine.

#### `public inline `[`Address`](#classscy_1_1net_1_1Address)` peerAddress() const` 





#### `protected `[`Address`](#classscy_1_1net_1_1Address)` _peerAddress` 





#### `protected inline virtual  ~Transaction()` 





#### `protected inline virtual void onPacket(`[`IPacket`](#classscy_1_1IPacket)` & packet)` 



Overrides the PacketSocketAdapter::onPacket callback for checking potential response candidates.

#### `protected inline virtual void onResponse()` 

Called when a successful response match is received.



#### `protected inline virtual bool checkResponse(const PacketT & packet)` 



Sub classes should derive this method to implement response checking logic. The base implementation only performs address matching.

# class `scy::net::UDPSocket` 

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
`protected `[`net::Address`](#classscy_1_1net_1_1Address)` _peer` | 
`protected Buffer _buffer` | 
`protected virtual void init()` | Initializes the underlying socket context.
`protected virtual bool recvStart()` | 
`protected virtual bool recvStop()` | 
`protected virtual void onError(const `[`scy::Error`](#structscy_1_1Error)` &)` | 
`protected virtual void onClose()` | Override to handle closure.

## Members

#### `public  UDPSocket(uv::Loop * loop)` 





#### `public virtual  ~UDPSocket()` 





#### `public virtual void connect(const `[`net::Address`](#classscy_1_1net_1_1Address)` & address)` 



Connects to the given peer IP address.

Throws an exception if the address is malformed. Connection errors can be handled via the [Error](#structscy_1_1Error) signal.

#### `public virtual void close()` 

Closes the underlying socket.



#### `public virtual void bind(const `[`net::Address`](#classscy_1_1net_1_1Address)` & address,unsigned flags)` 



Bind a local address to the socket. The address may be IPv4 or IPv6 (if supported).

Throws an Exception on error.

#### `public virtual int send(const char * data,std::size_t len,int flags)` 



Sends the given data buffer to the connected peer. Returns the number of bytes sent or -1 on error. No exception will be thrown. For TCP sockets the given peer address must match the connected peer address.

#### `public virtual int send(const char * data,std::size_t len,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress,int flags)` 





#### `public virtual bool setBroadcast(bool flag)` 





#### `public virtual bool setMulticastLoop(bool flag)` 





#### `public virtual bool setMulticastTTL(int ttl)` 





#### `public virtual `[`net::Address`](#classscy_1_1net_1_1Address)` address() const` 



The locally bound address.

This function will not throw. A Wildcard 0.0.0.0:0 address is returned if the socket is closed or invalid.

#### `public virtual `[`net::Address`](#classscy_1_1net_1_1Address)` peerAddress() const` 



The connected peer address.

This function will not throw. A Wildcard 0.0.0.0:0 address is returned if the socket is closed or invalid.

#### `public virtual net::TransportType transport() const` 

Returns the UDP transport protocol.



#### `public virtual void setError(const `[`scy::Error`](#structscy_1_1Error)` & err)` 



Sets the socket error.

Setting the error will result in socket closure.

#### `public virtual const `[`scy::Error`](#structscy_1_1Error)` & error() const` 

Return the socket error if any.



#### `public virtual bool closed() const` 



Returns true if the native socket handle is closed.

#### `public virtual uv::Loop * loop() const` 

Returns the socket event loop.



#### `public virtual void onRecv(const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buf,const `[`net::Address`](#classscy_1_1net_1_1Address)` & address)` 





#### `protected `[`net::Address`](#classscy_1_1net_1_1Address)` _peer` 





#### `protected Buffer _buffer` 





#### `protected virtual void init()` 

Initializes the underlying socket context.



#### `protected virtual bool recvStart()` 





#### `protected virtual bool recvStop()` 





#### `protected virtual void onError(const `[`scy::Error`](#structscy_1_1Error)` &)` 



Override to handle errors. The error may be a UV error, or a custom error.

#### `protected virtual void onClose()` 

Override to handle closure.



# class `scy::net::AddressBase` 

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

#### `public std::string host() const` 





#### `public std::uint16_t port() const` 





#### `public `[`Address::Family`](#group__net_1gaf91e77d155f2852974bbae5a2da34106)` family() const` 





#### `public socklen_t length() const` 





#### `public const struct sockaddr * addr() const` 





#### `public int af() const` 





#### `protected inline  AddressBase()` 





#### `protected inline virtual  ~AddressBase()` 





# class `scy::net::IPv4AddressBase` 

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

#### `public inline  IPv4AddressBase()` 





#### `public inline  IPv4AddressBase(const struct sockaddr_in * addr)` 





#### `public inline  IPv4AddressBase(const void * addr,std::uint16_t port)` 





#### `public inline virtual std::string host() const` 





#### `public inline virtual std::uint16_t port() const` 





#### `public inline virtual `[`Address::Family`](#group__net_1gaf91e77d155f2852974bbae5a2da34106)` family() const` 





#### `public inline virtual socklen_t length() const` 





#### `public inline virtual const struct sockaddr * addr() const` 





#### `public inline virtual int af() const` 





# struct `scy::net::DNSResult` 




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
`public `[`net::Address`](#classscy_1_1net_1_1Address)` addr` | The resolved address.
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

#### `public std::string host` 

The host to resolve.



#### `public std::uint16_t port` 

The host port to resolve.



#### `public `[`net::Address`](#classscy_1_1net_1_1Address)` addr` 

The resolved address.



#### `public struct addrinfo * info` 

The libuv uv_getaddrinfo result.



#### `public struct addrinfo * hints` 

libuv uv_getaddrinfo hints (optional)



#### `public std::function< void(const `[`DNSResult`](#structscy_1_1net_1_1DNSResult) &)`> callback` 

Result callback function.



#### `public void * opaque` 

Client data pointer.



#### `public enum scy::net::DNSResult::Status status` 





#### `public inline bool resolving() const` 





#### `public inline bool success() const` 





#### `public inline bool failed() const` 





#### `public inline bool complete() const` 





#### `public inline  DNSResult()` 





# struct `scy::net::PacketInfo` 

```
struct scy::net::PacketInfo
  : public scy::IPacketInfo
```  



Provides information about packets emitted from a socket. See [SocketPacket](#classscy_1_1net_1_1SocketPacket).

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public Socket::Ptr socket` | The source socket.
`public `[`Address`](#classscy_1_1net_1_1Address)` peerAddress` | 
`public inline  PacketInfo(const Socket::Ptr & socket,const `[`Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
`public inline  PacketInfo(const `[`PacketInfo`](#structscy_1_1net_1_1PacketInfo)` & r)` | 
`public inline virtual `[`IPacketInfo`](#structscy_1_1IPacketInfo)` * clone() const` | 
`public inline virtual  ~PacketInfo()` | 

## Members

#### `public Socket::Ptr socket` 

The source socket.



#### `public `[`Address`](#classscy_1_1net_1_1Address)` peerAddress` 



The originating peer address. For TCP this will always be connected address.

#### `public inline  PacketInfo(const Socket::Ptr & socket,const `[`Address`](#classscy_1_1net_1_1Address)` & peerAddress)` 





#### `public inline  PacketInfo(const `[`PacketInfo`](#structscy_1_1net_1_1PacketInfo)` & r)` 





#### `public inline virtual `[`IPacketInfo`](#structscy_1_1IPacketInfo)` * clone() const` 





#### `public inline virtual  ~PacketInfo()` 





