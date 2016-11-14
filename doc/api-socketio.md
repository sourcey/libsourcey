# Module <!-- group --> `socketio`



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`namespace `[`scy::sockio`](#namespacescy_1_1sockio)    | 
# namespace `scy::sockio` {#namespacescy_1_1sockio}



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[`scy::sockio::Client`](#classscy_1_1sockio_1_1Client)    | 
`class `[`scy::sockio::Packet`](#classscy_1_1sockio_1_1Packet)    | 
`class `[`scy::sockio::SSLClient`](#classscy_1_1sockio_1_1SSLClient)    | 
`class `[`scy::sockio::TCPClient`](#classscy_1_1sockio_1_1TCPClient)    | 
`struct `[`scy::sockio::ClientState`](#structscy_1_1sockio_1_1ClientState)    | 
`struct `[`scy::sockio::Transaction`](#structscy_1_1sockio_1_1Transaction)    | 
# class `scy::sockio::Client` {#classscy_1_1sockio_1_1Client}

```
class scy::sockio::Client
  : public scy::Stateful< ClientState >
  : public scy::net::SocketAdapter
  : public scy::SignalBase< PacketDelegateBase, IPacket & >
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Client(const net::Socket::Ptr & socket,const `[`Options`](#structscy_1_1sockio_1_1Client_1_1Options)` & options)` | Client(const net::Socket::Ptr& socket);.
`public virtual  ~Client()` | 
`public virtual void connect()` | virtual void connect(const std::string& host, std::uint16_t port);
`public virtual void close()` | 
`public virtual int send(const std::string & message,bool ack)` | Send a default message packet.
`public virtual int send(const json::Value & message,bool ack)` | 
`public virtual int send(const std::string & event,const char * message,bool ack)` | Send an event packet.
`public virtual int send(const std::string & event,const std::string & message,bool ack)` | 
`public virtual int send(const std::string & event,const json::Value & message,bool ack)` | 
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
`protected `[`Timer`](api-base.md#classscy_1_1Timer)` _pingTimer` | 
`protected `[`Timer`](api-base.md#classscy_1_1Timer)` _pingTimeoutTimer` | 
`protected `[`Timer`](api-base.md#classscy_1_1Timer)` _reconnectTimer` | 
`protected `[`scy::Error`](api-base.md#structscy_1_1Error)` _error` | 
`protected std::string _sessionID` | 
`protected `[`Client::Options`](api-socketio.md#structscy_1_1sockio_1_1Client_1_1Options)` _options` | 
`protected `[`http::ws::WebSocket`](api-http.md#classscy_1_1http_1_1ws_1_1WebSocket)` _ws` | 
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
`protected virtual void onSocketConnect()` | 
`protected virtual void onSocketRecv(const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
`protected virtual void onSocketError(const `[`scy::Error`](#structscy_1_1Error)` & error)` | 
`protected virtual void onSocketClose()` | 
`protected virtual void onPingTimer()` | 
`protected virtual void onPingTimeoutTimer()` | 
`protected virtual void startReconnectTimer()` | 
`protected virtual void stopReconnectTimer()` | 
`protected virtual void onReconnectTimer()` | 
`protected virtual int sendPing()` | 
`protected virtual void onPong()` | 

## Members

#### `public  Client(const net::Socket::Ptr & socket,const `[`Options`](#structscy_1_1sockio_1_1Client_1_1Options)` & options)` {#group__socketio_1gaf3bab48935a43adc1d1b950fca539b4b}

Client(const net::Socket::Ptr& socket);.



#### `public virtual  ~Client()` {#group__socketio_1ga4e03c96fccd0c3921a6f52d8474075a0}





#### `public virtual void connect()` {#group__socketio_1ga4cae0ae6454ca86d3f8e621855f04711}

virtual void connect(const std::string& host, std::uint16_t port);



#### `public virtual void close()` {#group__socketio_1gac7749ec4aef50ac8e6e11bdb85eae9b7}





#### `public virtual int send(const std::string & message,bool ack)` {#group__socketio_1ga508d0e1b286770420acddbb0c49ddfa9}

Send a default message packet.



#### `public virtual int send(const json::Value & message,bool ack)` {#group__socketio_1gaffa817822eee598ac8f6edbba44e9302}





#### `public virtual int send(const std::string & event,const char * message,bool ack)` {#group__socketio_1ga987dbb65a29b9da9a6a3940ad04a2117}

Send an event packet.



#### `public virtual int send(const std::string & event,const std::string & message,bool ack)` {#group__socketio_1gabbec9041687e54f59c789b480310c89d}





#### `public virtual int send(const std::string & event,const json::Value & message,bool ack)` {#group__socketio_1ga32a6d4d67162af92f863993d36ed9d48}





#### `public virtual int send(const `[`sockio::Packet`](#classscy_1_1sockio_1_1Packet)` & packet)` {#group__socketio_1ga7b79fb1718fe20de7497203098dd51b2}

Send the given packet.



#### `public `[`Transaction`](#structscy_1_1sockio_1_1Transaction)` * createTransaction(const `[`sockio::Packet`](#classscy_1_1sockio_1_1Packet)` & request,long timeout)` {#group__socketio_1gadcd60b1fd7f4e10d8c7b1f48786e5d97}

Create a packet transaction.



#### `public `[`Client::Options`](#structscy_1_1sockio_1_1Client_1_1Options)` & options()` {#group__socketio_1ga1d56cbc044e9cbd203fe8337f1b3aef2}

Return a reference to the client options object.



#### `public `[`http::ws::WebSocket`](#classscy_1_1http_1_1ws_1_1WebSocket)` & ws()` {#group__socketio_1gaac4475f8dce94d6b015effb16dba3621}

Return the underlying WebSocket instance.



#### `public std::string sessionID() const` {#group__socketio_1gab8f39e912113bd9cb05335616df9ca9f}

Return the current session ID assigned by the server.



#### `public `[`scy::Error`](#structscy_1_1Error)` error() const` {#group__socketio_1ga902ed628006040ee1d0e899b940d5007}

Return the error object (if any).



#### `public bool isOnline() const` {#group__socketio_1gaf3d50835f1bdb0c4b991524602fe9fd8}

Return true if the client is is Online state.



#### `public bool reconnecting() const` {#group__socketio_1ga4ee4ec2bfe49171d926531513198d5dc}

Return true if currently reconnecting.



#### `public bool wasOnline() const` {#group__socketio_1gaa9f0577de19a49e5a9fef0c830dd0b81}



Return true if the client was previously in the Online state. Useful for delegates handling the [Error](#structscy_1_1Error) state.

#### `public inline virtual const char * className() const` {#group__socketio_1ga89bbcae860f354466e5517964370f6ad}





#### `protected `[`Timer`](api-base.md#classscy_1_1Timer)` _pingTimer` {#group__socketio_1ga416988803a0fc305de3962455e7b0b29}





#### `protected `[`Timer`](api-base.md#classscy_1_1Timer)` _pingTimeoutTimer` {#group__socketio_1ga0793683d8d28021add17a90913f81479}





#### `protected `[`Timer`](api-base.md#classscy_1_1Timer)` _reconnectTimer` {#group__socketio_1gab5956064197e289fef9f3b5d095d20de}





#### `protected `[`scy::Error`](api-base.md#structscy_1_1Error)` _error` {#group__socketio_1gadd9c82f068158700490b50da7a6ba560}





#### `protected std::string _sessionID` {#group__socketio_1ga00c395bd0813515d9faf3eabd4df78f2}





#### `protected `[`Client::Options`](api-socketio.md#structscy_1_1sockio_1_1Client_1_1Options)` _options` {#group__socketio_1ga5c5a2501f0bdea875cd3822f9e0f98ce}





#### `protected `[`http::ws::WebSocket`](api-http.md#classscy_1_1http_1_1ws_1_1WebSocket)` _ws` {#group__socketio_1ga440dc05a637acef15f4a9d35da1a26f2}





#### `protected int _pingTimeout` {#group__socketio_1ga4a754e67ebf67dd0d771bfeb550993c6}





#### `protected int _pingInterval` {#group__socketio_1gaa2f34c122fd6a70f83046affadffe78b}





#### `protected bool _reconnecting` {#group__socketio_1gad4c182fcc58b7c23e746a1cb09496fbe}





#### `protected bool _wasOnline` {#group__socketio_1gafa85defc2c39cc8e9f653ece5ef9aefb}





#### `protected virtual void setError(const `[`scy::Error`](#structscy_1_1Error)` & error)` {#group__socketio_1gaf6195f3ff79e0044b2fc31ea23e01d3a}





#### `protected virtual void reset()` {#group__socketio_1ga9134950e317ce24f5a0c974cbedfd6f8}

Reset variables and timers at the beginning and end of each session.



#### `protected virtual void onConnect()` {#group__socketio_1ga8b2c8ada557eb4ccd443e85e57ff4322}





#### `protected virtual void onOnline()` {#group__socketio_1gad68140a980a49d58e7380d96197972cf}





#### `protected virtual void onClose()` {#group__socketio_1gaceddb423e21d34765f822aad6058cca8}





#### `protected virtual void onPacket(`[`sockio::Packet`](#classscy_1_1sockio_1_1Packet)` & packet)` {#group__socketio_1ga01da124277a760455a7a7875d82edec6}





#### `protected virtual void onHandshake(`[`sockio::Packet`](#classscy_1_1sockio_1_1Packet)` & packet)` {#group__socketio_1gab22a95163fa927739ea1811da5d9ec7e}





#### `protected virtual void onMessage(`[`sockio::Packet`](#classscy_1_1sockio_1_1Packet)` & packet)` {#group__socketio_1gaa9979d800bf8dd1176def89649b50e05}





#### `protected virtual void onSocketConnect()` {#group__socketio_1ga6aa005f194d3e945fd87962fd4811d34}



These virtual methods can be overridden as necessary to intercept socket events before they hit the application.

#### `protected virtual void onSocketRecv(const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` {#group__socketio_1gaac63d76f879ffcde98ff81f36e48cfcb}





#### `protected virtual void onSocketError(const `[`scy::Error`](#structscy_1_1Error)` & error)` {#group__socketio_1ga7bb57992f58af7b05bad32e5de10c730}





#### `protected virtual void onSocketClose()` {#group__socketio_1ga89ed5f3741c27265219bda1cd82fb44f}





#### `protected virtual void onPingTimer()` {#group__socketio_1ga514fa32ddc1de62a5678b64075a20889}





#### `protected virtual void onPingTimeoutTimer()` {#group__socketio_1gae899cd49123f75ae6483fc8cce8522b6}





#### `protected virtual void startReconnectTimer()` {#group__socketio_1ga861e10613f7afc68208b3255f1d777a6}





#### `protected virtual void stopReconnectTimer()` {#group__socketio_1gabea3a8460fba3dd2d7f3328aa01c7488}





#### `protected virtual void onReconnectTimer()` {#group__socketio_1gae46cfa6314a89ba94d2f6e31d029b474}





#### `protected virtual int sendPing()` {#group__socketio_1gac3651e340dae54a87e327a586a8d457a}





#### `protected virtual void onPong()` {#group__socketio_1ga5e3aee9f2d75784cdab41cf4ac214f80}





# class `scy::sockio::Packet` {#classscy_1_1sockio_1_1Packet}

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
`public  Packet(const json::Value & message,bool ack)` | JSON contructor.
`public  Packet(const std::string & event,const std::string & message,bool ack)` | Event contructor.
`public  Packet(const std::string & event,const json::Value & message,bool ack)` | Event JSON contructor.
`public  Packet(const `[`Packet`](#classscy_1_1sockio_1_1Packet)` & r)` | 
`public `[`Packet`](#classscy_1_1sockio_1_1Packet)` & operator=(const `[`Packet`](#classscy_1_1sockio_1_1Packet)` & r)` | 
`public virtual  ~Packet()` | 
`public virtual `[`IPacket`](#classscy_1_1IPacket)` * clone() const` | 
`public Frame frame() const` | 
`public Type type() const` | 
`public int id() const` | 
`public std::string nsp() const` | 
`public std::string message() const` | 
`public json::Value json() const` | 
`public void setID(int id)` | 
`public void setNamespace(const std::string & nsp)` | 
`public void setMessage(const std::string & message)` | 
`public void setAck(bool flag)` | 
`public virtual std::size_t read(const `[`ConstBuffer`](#classscy_1_1ConstBuffer)` &)` | 
`public virtual void write(Buffer &) const` | 
`public virtual std::size_t size() const` | 
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
`protected std::size_t _size` | 

## Members

#### `public  Packet(Frame frame,Type type,int id,const std::string & nsp,const std::string & event,const std::string & message,bool ack)` {#group__socketio_1ga44aa1f29292b9b7173f781f2284f68e9}

Default contructor.



#### `public  Packet(Type type,const std::string & message,bool ack)` {#group__socketio_1ga675b28c25bed57cbcf586f2e11df9fd0}

General contructor.



#### `public  Packet(const std::string & message,bool ack)` {#group__socketio_1ga1f49e2cafcac76149e4e620ad3745911}

Message contructor.



#### `public  Packet(const json::Value & message,bool ack)` {#group__socketio_1gadf5c11c6607ce6d9198313dc97570c67}

JSON contructor.



#### `public  Packet(const std::string & event,const std::string & message,bool ack)` {#group__socketio_1gab5ed33e585ea579cdfabb1cc0c3862ca}

Event contructor.



#### `public  Packet(const std::string & event,const json::Value & message,bool ack)` {#group__socketio_1gadf00255cc75f3df980606508f0c719c5}

Event JSON contructor.



#### `public  Packet(const `[`Packet`](#classscy_1_1sockio_1_1Packet)` & r)` {#group__socketio_1ga449b726f5f330c10b8d675f1dddb68c0}





#### `public `[`Packet`](#classscy_1_1sockio_1_1Packet)` & operator=(const `[`Packet`](#classscy_1_1sockio_1_1Packet)` & r)` {#group__socketio_1gad70159cc94352ec492bc331351901bf1}





#### `public virtual  ~Packet()` {#group__socketio_1gab9cbbf7163eb215c143bf3d557f01adb}





#### `public virtual `[`IPacket`](#classscy_1_1IPacket)` * clone() const` {#group__socketio_1ga8d68411ae7a9c9bef636dcef4420009d}





#### `public Frame frame() const` {#group__socketio_1gab8d8da781b310ecc8740cbefd4ccde95}





#### `public Type type() const` {#group__socketio_1gad25c4befad513753e9f94a292524d78b}





#### `public int id() const` {#group__socketio_1gaa876eb8dc0e8d4f736d53e80a5857f6f}





#### `public std::string nsp() const` {#group__socketio_1ga6391fa748eb7dce648ac50e34bc4d378}





#### `public std::string message() const` {#group__socketio_1gae5f0cff0a9d5b8d09814e145d5b0b6b4}





#### `public json::Value json() const` {#group__socketio_1gac1d9f1bbed120d97988af363e0061c58}





#### `public void setID(int id)` {#group__socketio_1ga53879f05bcdc78a461bdac94143b4ee5}





#### `public void setNamespace(const std::string & nsp)` {#group__socketio_1gab46a71b0d0d3b1e6fd565c5868834985}





#### `public void setMessage(const std::string & message)` {#group__socketio_1gabac8aab538510674d60221b0262b15a6}





#### `public void setAck(bool flag)` {#group__socketio_1ga5f686903d075ae98dff5669e6aec531e}





#### `public virtual std::size_t read(const `[`ConstBuffer`](#classscy_1_1ConstBuffer)` &)` {#group__socketio_1ga081246038126d83d077764781899a723}



Read/parse to the packet from the given input buffer. The number of bytes read is returned.

#### `public virtual void write(Buffer &) const` {#group__socketio_1gad8bcc194e62922929788a39be680775c}



Copy/generate to the packet given output buffer. The number of bytes written can be obtained from the buffer.

Todo: It may be prefferable to use our pod types here instead of buffer input, but the current codebase requires that the buffer be dynamically resizable for some protocols...

virtual std::size_t write(MutableBuffer&) const = 0;

#### `public virtual std::size_t size() const` {#group__socketio_1ga283b7ff23ec9537c629a51e2f6229ffd}



The size of the packet in bytes.

This is the nember of bytes that will be written on a call to [write()](#group__socketio_1gad8bcc194e62922929788a39be680775c), but may not be the number of bytes that will be consumed by [read()](#group__socketio_1ga081246038126d83d077764781899a723).

#### `public bool valid() const` {#group__socketio_1gae3dcaa80ffcc3cff7ab73e7760d0286c}





#### `public std::string frameString() const` {#group__socketio_1ga3ef4816dc3959657072000b521e55b23}





#### `public std::string typeString() const` {#group__socketio_1ga5e1f13ee6e5c733be731a9a2835e65b3}





#### `public std::string toString() const` {#group__socketio_1ga4689c6ada5ef5838eaf8def23686c867}





#### `public virtual void print(std::ostream & os) const` {#group__socketio_1gaf38306882f7c10744954aecbc8cb93ad}





#### `public inline virtual const char * className() const` {#group__socketio_1ga6259516dbbe673b202f316f0704169df}





#### `protected Frame _frame` {#group__socketio_1ga9b83a0852c496507297a7a4fa628d986}





#### `protected Type _type` {#group__socketio_1ga0901545fc4a417dcb5ce82626a8bca0b}





#### `protected int _id` {#group__socketio_1gafbff43d4fd108fc4b8db7f0eea78e18d}





#### `protected std::string _nsp` {#group__socketio_1gaa148339029b0d207dd2cf385175cc039}





#### `protected std::string _event` {#group__socketio_1ga01a154e6f19480d779b89f42dbcc00a8}





#### `protected std::string _message` {#group__socketio_1gaa966093c5e0ac9e0509b8169ea6f125d}





#### `protected bool _ack` {#group__socketio_1ga117282ced74d06cf3285be7c8eecdc58}





#### `protected std::size_t _size` {#group__socketio_1ga7bb35917f5d7ab2995895f2d8be28766}





# class `scy::sockio::SSLClient` {#classscy_1_1sockio_1_1SSLClient}

```
class scy::sockio::SSLClient
  : public scy::sockio::Client
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  SSLClient(const `[`Client::Options`](#structscy_1_1sockio_1_1Client_1_1Options)` & options,uv::Loop * loop)` | 

## Members

#### `public  SSLClient(const `[`Client::Options`](#structscy_1_1sockio_1_1Client_1_1Options)` & options,uv::Loop * loop)` {#group__socketio_1ga9d4ec4f9600af18b563d571d26ab16de}





# class `scy::sockio::TCPClient` {#classscy_1_1sockio_1_1TCPClient}

```
class scy::sockio::TCPClient
  : public scy::sockio::Client
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  TCPClient(const `[`Client::Options`](#structscy_1_1sockio_1_1Client_1_1Options)` & options,uv::Loop * loop)` | 

## Members

#### `public  TCPClient(const `[`Client::Options`](#structscy_1_1sockio_1_1Client_1_1Options)` & options,uv::Loop * loop)` {#group__socketio_1gadea62402cdf6ec3065b44f7d9b505592}





# struct `scy::sockio::ClientState` {#structscy_1_1sockio_1_1ClientState}

```
struct scy::sockio::ClientState
  : public scy::State
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual std::string str(unsigned int id) const` | 

## Members

#### `public inline virtual std::string str(unsigned int id) const` {#group__socketio_1ga25bc5821495add59ba1d4f45796b42c8}





# struct `scy::sockio::Transaction` {#structscy_1_1sockio_1_1Transaction}

```
struct scy::sockio::Transaction
  : public scy::PacketTransaction< sockio::Packet >
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`sockio::Client`](api-socketio.md#classscy_1_1sockio_1_1Client)` & client` | 
`public  Transaction(`[`sockio::Client`](#classscy_1_1sockio_1_1Client)` & client,long timeout)` | 
`public  Transaction(`[`sockio::Client`](#classscy_1_1sockio_1_1Client)` & client,const `[`sockio::Packet`](#classscy_1_1sockio_1_1Packet)` & request,long timeout)` | 
`public virtual bool send()` | 
`public virtual bool checkResponse(const `[`sockio::Packet`](#classscy_1_1sockio_1_1Packet)` & packet)` | 
`public virtual void onPotentialResponse(void *,`[`sockio::Packet`](#classscy_1_1sockio_1_1Packet)` & packet)` | 
`public virtual void onResponse()` | Called when a successful response is received.
`public inline virtual const char * className() const` | 
`protected virtual  ~Transaction()` | 

## Members

#### `public `[`sockio::Client`](api-socketio.md#classscy_1_1sockio_1_1Client)` & client` {#group__socketio_1ga96048abc4d5da1b2c1255e1199979cfe}





#### `public  Transaction(`[`sockio::Client`](#classscy_1_1sockio_1_1Client)` & client,long timeout)` {#group__socketio_1ga9f8e74df2b6ed494bd0549e067015628}





#### `public  Transaction(`[`sockio::Client`](#classscy_1_1sockio_1_1Client)` & client,const `[`sockio::Packet`](#classscy_1_1sockio_1_1Packet)` & request,long timeout)` {#group__socketio_1ga96ea4c7a2e2086a628d9a5cb5a1e5f31}





#### `public virtual bool send()` {#group__socketio_1gaceaa4ef7048b73ad0e762fda18f9f6a1}



Starts the transaction timer and sends the request. Overriding classes should implement send logic here.

#### `public virtual bool checkResponse(const `[`sockio::Packet`](#classscy_1_1sockio_1_1Packet)` & packet)` {#group__socketio_1ga334fa421742d7f91eca8b68e488f2c45}



Checks a potential response candidate and returns true on successful match.

#### `public virtual void onPotentialResponse(void *,`[`sockio::Packet`](#classscy_1_1sockio_1_1Packet)` & packet)` {#group__socketio_1ga22b7351290bd8e667eef1ebac40481ce}





#### `public virtual void onResponse()` {#group__socketio_1ga0653a7b7d0e1080a9421d9705453aeda}

Called when a successful response is received.



#### `public inline virtual const char * className() const` {#group__socketio_1ga7eda37071ff487047ba47023aff1f79a}





#### `protected virtual  ~Transaction()` {#group__socketio_1gadb5419dcd87210b93fb4512602fe0759}





