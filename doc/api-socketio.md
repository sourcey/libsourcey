# Module <!-- group --> `socketio`



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`namespace `[`scy::sockio`](#namespacescy_1_1sockio)    | 
`struct `[`scy::sockio::Client::Options`](#structscy_1_1sockio_1_1Client_1_1Options)    | 
# namespace `scy::sockio` 



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[`scy::sockio::Client`](#classscy_1_1sockio_1_1Client)    | 
`class `[`scy::sockio::Packet`](#classscy_1_1sockio_1_1Packet)    | 
`class `[`scy::sockio::SSLClient`](#classscy_1_1sockio_1_1SSLClient)    | 
`class `[`scy::sockio::TCPClient`](#classscy_1_1sockio_1_1TCPClient)    | 
`struct `[`scy::sockio::ClientState`](#structscy_1_1sockio_1_1ClientState)    | 
`struct `[`scy::sockio::Transaction`](#structscy_1_1sockio_1_1Transaction)    | 
# class `scy::sockio::Client` 

```
class scy::sockio::Client
  : public scy::Stateful< ClientState >
  : public scy::net::SocketAdapter
  : public scy::Signal< void(IPacket &)>
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Client(const net::Socket::Ptr & socket,const `[`Options`](#structscy_1_1sockio_1_1Client_1_1Options)` & options)` | Client(const net::Socket::Ptr& socket);.
`public virtual  ~Client()` | 
`public virtual void connect()` | 
`public virtual void close()` | 
`public virtual int send(const std::string & message,bool ack)` | Send a default message packet.
`public virtual int send(const json::value & message,bool ack)` | 
`public virtual int send(const std::string & event,const char * message,bool ack)` | Send an event packet.
`public virtual int send(const std::string & event,const std::string & message,bool ack)` | 
`public virtual int send(const std::string & event,const json::value & message,bool ack)` | 
`public virtual int send(const `[`sockio::Packet`](#classscy_1_1sockio_1_1Packet)` & packet)` | Send the given packet.
`public `[`Transaction`](#structscy_1_1sockio_1_1Transaction)` * createTransaction(const `[`sockio::Packet`](#classscy_1_1sockio_1_1Packet)` & request,long timeout)` | Create a packet transaction.
`public `[`Client::Options`](#structscy_1_1sockio_1_1Client_1_1Options)` & options()` | Return a reference to the client options object.
`public `[`http::ws::WebSocket`](#classscy_1_1http_1_1ws_1_1WebSocket)` & ws()` | Return the underlying WebSocket instance.
`public std::string sessionID() const` | Return the current session ID assigned by the server.
`public `[`scy::Error`](#structscy_1_1Error)` error() const` | Return the error object (if any).
`public bool isOnline() const` | Return true if the client is is Online state.
`public bool reconnecting() const` | Return true if currently reconnecting.
`public bool wasOnline() const` | 
`public inline virtual const char * className() const` | 
`protected `[`Timer`](./doc/api-base.md#classscy_1_1Timer)` _pingTimer` | 
`protected `[`Timer`](./doc/api-base.md#classscy_1_1Timer)` _pingTimeoutTimer` | 
`protected `[`Timer`](./doc/api-base.md#classscy_1_1Timer)` _reconnectTimer` | 
`protected `[`scy::Error`](./doc/api-base.md#structscy_1_1Error)` _error` | 
`protected std::string _sessionID` | 
`protected `[`Client::Options`](./doc/api-socketio.md#structscy_1_1sockio_1_1Client_1_1Options)` _options` | 
`protected `[`http::ws::WebSocket`](./doc/api-http.md#classscy_1_1http_1_1ws_1_1WebSocket)` _ws` | 
`protected int _pingTimeout` | 
`protected int _pingInterval` | 
`protected bool _reconnecting` | 
`protected bool _wasOnline` | 
`protected virtual void setError(const `[`scy::Error`](#structscy_1_1Error)` & error)` | 
`protected virtual void reset()` | Reset variables and timers at the beginning and end of each session.
`protected virtual void onConnect()` | 
`protected virtual void onOnline()` | 
`protected virtual void onClose()` | 
`protected virtual void onPacket(`[`sockio::Packet`](#classscy_1_1sockio_1_1Packet)` & packet)` | 
`protected virtual void onHandshake(`[`sockio::Packet`](#classscy_1_1sockio_1_1Packet)` & packet)` | 
`protected virtual void onMessage(`[`sockio::Packet`](#classscy_1_1sockio_1_1Packet)` & packet)` | 
`protected virtual void onSocketConnect(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket)` | 
`protected virtual void onSocketRecv(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket,const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
`protected virtual void onSocketError(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket,const `[`scy::Error`](#structscy_1_1Error)` & error)` | 
`protected virtual void onSocketClose(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket)` | 
`protected virtual void onPingTimer()` | 
`protected virtual void onPingTimeoutTimer()` | 
`protected virtual void startReconnectTimer()` | 
`protected virtual void stopReconnectTimer()` | 
`protected virtual void onReconnectTimer()` | 
`protected virtual int sendPing()` | 
`protected virtual void onPong()` | 

## Members

#### `public  Client(const net::Socket::Ptr & socket,const `[`Options`](#structscy_1_1sockio_1_1Client_1_1Options)` & options)` 

Client(const net::Socket::Ptr& socket);.



#### `public virtual  ~Client()` 





#### `public virtual void connect()` 





#### `public virtual void close()` 





#### `public virtual int send(const std::string & message,bool ack)` 

Send a default message packet.



#### `public virtual int send(const json::value & message,bool ack)` 





#### `public virtual int send(const std::string & event,const char * message,bool ack)` 

Send an event packet.



#### `public virtual int send(const std::string & event,const std::string & message,bool ack)` 





#### `public virtual int send(const std::string & event,const json::value & message,bool ack)` 





#### `public virtual int send(const `[`sockio::Packet`](#classscy_1_1sockio_1_1Packet)` & packet)` 

Send the given packet.



#### `public `[`Transaction`](#structscy_1_1sockio_1_1Transaction)` * createTransaction(const `[`sockio::Packet`](#classscy_1_1sockio_1_1Packet)` & request,long timeout)` 

Create a packet transaction.



#### `public `[`Client::Options`](#structscy_1_1sockio_1_1Client_1_1Options)` & options()` 

Return a reference to the client options object.



#### `public `[`http::ws::WebSocket`](#classscy_1_1http_1_1ws_1_1WebSocket)` & ws()` 

Return the underlying WebSocket instance.



#### `public std::string sessionID() const` 

Return the current session ID assigned by the server.



#### `public `[`scy::Error`](#structscy_1_1Error)` error() const` 

Return the error object (if any).



#### `public bool isOnline() const` 

Return true if the client is is Online state.



#### `public bool reconnecting() const` 

Return true if currently reconnecting.



#### `public bool wasOnline() const` 



Return true if the client was previously in the Online state. Useful for delegates handling the [Error](#structscy_1_1Error) state.

#### `public inline virtual const char * className() const` 





#### `protected `[`Timer`](./doc/api-base.md#classscy_1_1Timer)` _pingTimer` 





#### `protected `[`Timer`](./doc/api-base.md#classscy_1_1Timer)` _pingTimeoutTimer` 





#### `protected `[`Timer`](./doc/api-base.md#classscy_1_1Timer)` _reconnectTimer` 





#### `protected `[`scy::Error`](./doc/api-base.md#structscy_1_1Error)` _error` 





#### `protected std::string _sessionID` 





#### `protected `[`Client::Options`](./doc/api-socketio.md#structscy_1_1sockio_1_1Client_1_1Options)` _options` 





#### `protected `[`http::ws::WebSocket`](./doc/api-http.md#classscy_1_1http_1_1ws_1_1WebSocket)` _ws` 





#### `protected int _pingTimeout` 





#### `protected int _pingInterval` 





#### `protected bool _reconnecting` 





#### `protected bool _wasOnline` 





#### `protected virtual void setError(const `[`scy::Error`](#structscy_1_1Error)` & error)` 





#### `protected virtual void reset()` 

Reset variables and timers at the beginning and end of each session.



#### `protected virtual void onConnect()` 





#### `protected virtual void onOnline()` 





#### `protected virtual void onClose()` 





#### `protected virtual void onPacket(`[`sockio::Packet`](#classscy_1_1sockio_1_1Packet)` & packet)` 





#### `protected virtual void onHandshake(`[`sockio::Packet`](#classscy_1_1sockio_1_1Packet)` & packet)` 





#### `protected virtual void onMessage(`[`sockio::Packet`](#classscy_1_1sockio_1_1Packet)` & packet)` 





#### `protected virtual void onSocketConnect(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket)` 



These virtual methods can be overridden as necessary to intercept socket events before they hit the application.

#### `protected virtual void onSocketRecv(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket,const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` 





#### `protected virtual void onSocketError(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket,const `[`scy::Error`](#structscy_1_1Error)` & error)` 





#### `protected virtual void onSocketClose(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket)` 





#### `protected virtual void onPingTimer()` 





#### `protected virtual void onPingTimeoutTimer()` 





#### `protected virtual void startReconnectTimer()` 





#### `protected virtual void stopReconnectTimer()` 





#### `protected virtual void onReconnectTimer()` 





#### `protected virtual int sendPing()` 





#### `protected virtual void onPong()` 





# class `scy::sockio::Packet` 

```
class scy::sockio::Packet
  : public scy::IPacket
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Packet(Frame frame,Type type,int id,const std::string & nsp,const std::string & event,const std::string & message,bool ack)` | Default contructor.
`public  Packet(Type type,const std::string & message,bool ack)` | General contructor.
`public  Packet(const std::string & message,bool ack)` | Message contructor.
`public  Packet(const json::value & message,bool ack)` | JSON contructor.
`public  Packet(const std::string & event,const std::string & message,bool ack)` | Event contructor.
`public  Packet(const std::string & event,const json::value & message,bool ack)` | Event JSON contructor.
`public  Packet(const `[`Packet`](#classscy_1_1sockio_1_1Packet)` & r)` | 
`public `[`Packet`](#classscy_1_1sockio_1_1Packet)` & operator=(const `[`Packet`](#classscy_1_1sockio_1_1Packet)` & r)` | 
`public virtual  ~Packet()` | 
`public virtual `[`IPacket`](#classscy_1_1IPacket)` * clone() const` | 
`public Frame frame() const` | 
`public Type type() const` | 
`public int id() const` | 
`public std::string nsp() const` | 
`public std::string message() const` | 
`public json::value json() const` | 
`public void setID(int id)` | 
`public void setNamespace(const std::string & nsp)` | 
`public void setMessage(const std::string & message)` | 
`public void setAck(bool flag)` | 
`public virtual ssize_t read(const `[`ConstBuffer`](#classscy_1_1ConstBuffer)` &)` | 
`public virtual void write(`[`Buffer`](#group__base_1gab10b36b080deb583ed703fca6b63ffdd)` &) const` | 
`public virtual size_t size() const` | 
`public bool valid() const` | 
`public std::string frameString() const` | 
`public std::string typeString() const` | 
`public std::string toString() const` | 
`public virtual void print(std::ostream & os) const` | 
`public inline virtual const char * className() const` | 
`protected Frame _frame` | 
`protected Type _type` | 
`protected int _id` | 
`protected std::string _nsp` | 
`protected std::string _event` | 
`protected std::string _message` | 
`protected bool _ack` | 
`protected size_t _size` | 

## Members

#### `public  Packet(Frame frame,Type type,int id,const std::string & nsp,const std::string & event,const std::string & message,bool ack)` 

Default contructor.



#### `public  Packet(Type type,const std::string & message,bool ack)` 

General contructor.



#### `public  Packet(const std::string & message,bool ack)` 

Message contructor.



#### `public  Packet(const json::value & message,bool ack)` 

JSON contructor.



#### `public  Packet(const std::string & event,const std::string & message,bool ack)` 

Event contructor.



#### `public  Packet(const std::string & event,const json::value & message,bool ack)` 

Event JSON contructor.



#### `public  Packet(const `[`Packet`](#classscy_1_1sockio_1_1Packet)` & r)` 





#### `public `[`Packet`](#classscy_1_1sockio_1_1Packet)` & operator=(const `[`Packet`](#classscy_1_1sockio_1_1Packet)` & r)` 





#### `public virtual  ~Packet()` 





#### `public virtual `[`IPacket`](#classscy_1_1IPacket)` * clone() const` 





#### `public Frame frame() const` 





#### `public Type type() const` 





#### `public int id() const` 





#### `public std::string nsp() const` 





#### `public std::string message() const` 





#### `public json::value json() const` 





#### `public void setID(int id)` 





#### `public void setNamespace(const std::string & nsp)` 





#### `public void setMessage(const std::string & message)` 





#### `public void setAck(bool flag)` 





#### `public virtual ssize_t read(const `[`ConstBuffer`](#classscy_1_1ConstBuffer)` &)` 



Read/parse to the packet from the given input buffer. The number of bytes read is returned.

#### `public virtual void write(`[`Buffer`](#group__base_1gab10b36b080deb583ed703fca6b63ffdd)` &) const` 



Copy/generate to the packet given output buffer. The number of bytes written can be obtained from the buffer.

Todo: It may be prefferable to use our pod types here instead of buffer input, but the current codebase requires that the buffer be dynamically resizable for some protocols...

#### `public virtual size_t size() const` 



The size of the packet in bytes.

This is the nember of bytes that will be written on a call to [write()](#group__socketio_1gad8bcc194e62922929788a39be680775c), but may not be the number of bytes that will be consumed by [read()](#group__socketio_1ga2b911c9a18008b42c74ab85015ddecb9).

#### `public bool valid() const` 





#### `public std::string frameString() const` 





#### `public std::string typeString() const` 





#### `public std::string toString() const` 





#### `public virtual void print(std::ostream & os) const` 





#### `public inline virtual const char * className() const` 





#### `protected Frame _frame` 





#### `protected Type _type` 





#### `protected int _id` 





#### `protected std::string _nsp` 





#### `protected std::string _event` 





#### `protected std::string _message` 





#### `protected bool _ack` 





#### `protected size_t _size` 





# class `scy::sockio::SSLClient` 

```
class scy::sockio::SSLClient
  : public scy::sockio::Client
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  SSLClient(const `[`Client::Options`](#structscy_1_1sockio_1_1Client_1_1Options)` & options,uv::Loop * loop)` | 

## Members

#### `public  SSLClient(const `[`Client::Options`](#structscy_1_1sockio_1_1Client_1_1Options)` & options,uv::Loop * loop)` 





# class `scy::sockio::TCPClient` 

```
class scy::sockio::TCPClient
  : public scy::sockio::Client
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  TCPClient(const `[`Client::Options`](#structscy_1_1sockio_1_1Client_1_1Options)` & options,uv::Loop * loop)` | 

## Members

#### `public  TCPClient(const `[`Client::Options`](#structscy_1_1sockio_1_1Client_1_1Options)` & options,uv::Loop * loop)` 





# struct `scy::sockio::ClientState` 

```
struct scy::sockio::ClientState
  : public scy::State
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline std::string str(unsigned int id) const` | 

## Members

#### `public inline std::string str(unsigned int id) const` 





# struct `scy::sockio::Transaction` 

```
struct scy::sockio::Transaction
  : public scy::PacketTransaction< sockio::Packet >
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`sockio::Client`](./doc/api-socketio.md#classscy_1_1sockio_1_1Client)` & client` | 
`public  Transaction(`[`sockio::Client`](#classscy_1_1sockio_1_1Client)` & client,long timeout)` | 
`public  Transaction(`[`sockio::Client`](#classscy_1_1sockio_1_1Client)` & client,const `[`sockio::Packet`](#classscy_1_1sockio_1_1Packet)` & request,long timeout)` | 
`public virtual bool send()` | 
`public virtual bool checkResponse(const `[`sockio::Packet`](#classscy_1_1sockio_1_1Packet)` & packet)` | 
`public virtual void onPotentialResponse(`[`sockio::Packet`](#classscy_1_1sockio_1_1Packet)` & packet)` | 
`public virtual void onResponse()` | Called when a successful response is received.
`public inline virtual const char * className() const` | 
`protected virtual  ~Transaction()` | 

## Members

#### `public `[`sockio::Client`](./doc/api-socketio.md#classscy_1_1sockio_1_1Client)` & client` 





#### `public  Transaction(`[`sockio::Client`](#classscy_1_1sockio_1_1Client)` & client,long timeout)` 





#### `public  Transaction(`[`sockio::Client`](#classscy_1_1sockio_1_1Client)` & client,const `[`sockio::Packet`](#classscy_1_1sockio_1_1Packet)` & request,long timeout)` 





#### `public virtual bool send()` 



Starts the transaction timer and sends the request. Overriding classes should implement send logic here.

#### `public virtual bool checkResponse(const `[`sockio::Packet`](#classscy_1_1sockio_1_1Packet)` & packet)` 



Checks a potential response candidate and returns true on successful match.

#### `public virtual void onPotentialResponse(`[`sockio::Packet`](#classscy_1_1sockio_1_1Packet)` & packet)` 





#### `public virtual void onResponse()` 

Called when a successful response is received.



#### `public inline virtual const char * className() const` 





#### `protected virtual  ~Transaction()` 





# struct `scy::sockio::Client::Options` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::string host` | 
`public uint16_t port` | 
`public bool reconnection` | Weather or not to reconnect if disconnected from the server.
`public int reconnectAttempts` | 
`public int reconnectDelay` | 
`public inline  Options()` | 

## Members

#### `public std::string host` 





#### `public uint16_t port` 





#### `public bool reconnection` 

Weather or not to reconnect if disconnected from the server.



#### `public int reconnectAttempts` 



The number of times to attempt to reconnect if disconnected from the server. (0 = unlimited)

#### `public int reconnectDelay` 





#### `public inline  Options()` 





