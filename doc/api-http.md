# Module <!-- group --> `http`



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`namespace `[`scy::http`](#namespacescy_1_1http)    | 
`namespace `[`scy::http::ws`](#namespacescy_1_1http_1_1ws)    | 
`struct `[`scy::http::FormWriter::Part`](#structscy_1_1http_1_1FormWriter_1_1Part)    | 
# namespace `scy::http` {#namespacescy_1_1http}



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[`scy::http::Authenticator`](#classscy_1_1http_1_1Authenticator)    | 
`class `[`scy::http::BadRequestHandler`](#classscy_1_1http_1_1BadRequestHandler)    | 
`class `[`scy::http::BasicAuthenticator`](#classscy_1_1http_1_1BasicAuthenticator)    | 
`class `[`scy::http::ChunkedAdapter`](#classscy_1_1http_1_1ChunkedAdapter)    | 
`class `[`scy::http::Client`](#classscy_1_1http_1_1Client)    | 
`class `[`scy::http::ClientAdapter`](#classscy_1_1http_1_1ClientAdapter)    | 
`class `[`scy::http::ClientConnection`](#classscy_1_1http_1_1ClientConnection)    | 
`class `[`scy::http::Connection`](#classscy_1_1http_1_1Connection)    | 
`class `[`scy::http::ConnectionAdapter`](#classscy_1_1http_1_1ConnectionAdapter)    | Default HTTP socket adapter for reading and writing HTTP messages.
`class `[`scy::http::Cookie`](#classscy_1_1http_1_1Cookie)    | 
`class `[`scy::http::FilePart`](#classscy_1_1http_1_1FilePart)    | An implementation of [FilePart](#classscy_1_1http_1_1FilePart) for plain files.
`class `[`scy::http::FormPart`](#classscy_1_1http_1_1FormPart)    | An implementation of [FormPart](#classscy_1_1http_1_1FormPart).
`class `[`scy::http::FormWriter`](#classscy_1_1http_1_1FormWriter)    | 
`class `[`scy::http::Message`](#classscy_1_1http_1_1Message)    | 
`class `[`scy::http::MultipartAdapter`](#classscy_1_1http_1_1MultipartAdapter)    | 
`class `[`scy::http::Parser`](#classscy_1_1http_1_1Parser)    | 
`class `[`scy::http::ParserObserver`](#classscy_1_1http_1_1ParserObserver)    | 
`class `[`scy::http::ProgressSignal`](#classscy_1_1http_1_1ProgressSignal)    | 
`class `[`scy::http::Request`](#classscy_1_1http_1_1Request)    | 
`class `[`scy::http::Response`](#classscy_1_1http_1_1Response)    | This class encapsulates an HTTP response message.
`class `[`scy::http::Server`](#classscy_1_1http_1_1Server)    | 
`class `[`scy::http::ServerAdapter`](#classscy_1_1http_1_1ServerAdapter)    | 
`class `[`scy::http::ServerConnection`](#classscy_1_1http_1_1ServerConnection)    | 
`class `[`scy::http::ServerResponder`](#classscy_1_1http_1_1ServerResponder)    | 
`class `[`scy::http::ServerResponderFactory`](#classscy_1_1http_1_1ServerResponderFactory)    | 
`class `[`scy::http::StringPart`](#classscy_1_1http_1_1StringPart)    | An implementation of [StringPart](#classscy_1_1http_1_1StringPart) for plain files.
`class `[`scy::http::URL`](#classscy_1_1http_1_1URL)    | 
`struct `[`scy::http::Method`](#structscy_1_1http_1_1Method)    | HTTP request methods.
`struct `[`scy::http::ParserError`](#structscy_1_1http_1_1ParserError)    | 
# class `scy::http::Authenticator` {#classscy_1_1http_1_1Authenticator}




This is a utility class for working with HTTP authentication (basic or digest) in [http::Request](http::Request) objects.

Note: Do not forget to read the entire response stream from the 401 response before sending the authenticated request, otherwise there may be problems if a persistent connection is used.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Authenticator()` | Creates an empty [Authenticator](./doc/api-http.md#classscy_1_1http_1_1Authenticator) object.
`public  Authenticator(const std::string & username,const std::string & password)` | Creates an [Authenticator](./doc/api-http.md#classscy_1_1http_1_1Authenticator) object with the given username and password.
`public  ~Authenticator()` | Destroys the [Authenticator](./doc/api-http.md#classscy_1_1http_1_1Authenticator).
`public void fromUserInfo(const std::string & userInfo)` | 
`public void fromURI(const `[`http::URL`](#classscy_1_1http_1_1URL)` & uri)` | 
`public inline void setUsername(const std::string & username)` | Sets the username.
`public inline const std::string & username() const` | Returns the username.
`public inline void setPassword(const std::string & password)` | Sets the password.
`public inline const std::string & password() const` | Returns the password.
`public void authenticate(`[`http::Request`](#classscy_1_1http_1_1Request)` & request,const `[`http::Response`](#classscy_1_1http_1_1Response)` & response)` | 
`public void updateAuthInfo(`[`http::Request`](#classscy_1_1http_1_1Request)` & request)` | 
`public void proxyAuthenticate(`[`http::Request`](#classscy_1_1http_1_1Request)` & request,const `[`http::Response`](#classscy_1_1http_1_1Response)` & response)` | 
`public void updateProxyAuthInfo(`[`http::Request`](#classscy_1_1http_1_1Request)` & request)` | 

## Members

#### `public  Authenticator()` {#group__http_1ga6214df1c0d604a6925a4ff5341a2570e}

Creates an empty [Authenticator](#classscy_1_1http_1_1Authenticator) object.



#### `public  Authenticator(const std::string & username,const std::string & password)` {#group__http_1gaab5bbd50fb1b1d53a321ba47b294e2c8}

Creates an [Authenticator](#classscy_1_1http_1_1Authenticator) object with the given username and password.



#### `public  ~Authenticator()` {#group__http_1gaed3172ab7db55f51397c6230f32dea08}

Destroys the [Authenticator](#classscy_1_1http_1_1Authenticator).



#### `public void fromUserInfo(const std::string & userInfo)` {#group__http_1gad1ee6dffdb0fdd9a8276425ea40fafca}



Parses username:password std::string and sets username and password of the credentials object. Throws SyntaxException on invalid user information.

#### `public void fromURI(const `[`http::URL`](#classscy_1_1http_1_1URL)` & uri)` {#group__http_1ga12f51f7f097ca4e3cfc5d434e754a01d}



Extracts username and password from the given URI and sets username and password of the credentials object. Does nothing if URI has no user info part.

#### `public inline void setUsername(const std::string & username)` {#group__http_1ga7944c4a74c6a4e5dbd3b2c2c1f252fc6}

Sets the username.



#### `public inline const std::string & username() const` {#group__http_1ga365097004eb19ff11797471edaba7a11}

Returns the username.



#### `public inline void setPassword(const std::string & password)` {#group__http_1ga4699ccc9f01adab04a81a389b3367e80}

Sets the password.



#### `public inline const std::string & password() const` {#group__http_1ga7eeb4f10098784269c96fc6a3146fac8}

Returns the password.



#### `public void authenticate(`[`http::Request`](#classscy_1_1http_1_1Request)` & request,const `[`http::Response`](#classscy_1_1http_1_1Response)` & response)` {#group__http_1ga653edb8897e31620c3f51e3b3241dd27}



Inspects WWW-Authenticate header of the response, initializes the internal state (in case of digest authentication) and adds required information to the given [http::Request](http::Request).

Does nothing if there is no WWW-Authenticate header in the [http::Response](http::Response).

#### `public void updateAuthInfo(`[`http::Request`](#classscy_1_1http_1_1Request)` & request)` {#group__http_1ga518fc10b748ab74fa1d907b16f623802}



Updates internal state (in case of digest authentication) and replaces authentication information in the request accordingly.

#### `public void proxyAuthenticate(`[`http::Request`](#classscy_1_1http_1_1Request)` & request,const `[`http::Response`](#classscy_1_1http_1_1Response)` & response)` {#group__http_1gaf3d04876711434a0ec46b0ed92063415}



Inspects Proxy-Authenticate header of the response, initializes the internal state (in case of digest authentication) and adds required information to the given [http::Request](http::Request).

Does nothing if there is no Proxy-Authenticate header in the [http::Response](http::Response).

#### `public void updateProxyAuthInfo(`[`http::Request`](#classscy_1_1http_1_1Request)` & request)` {#group__http_1ga312f80e580ffd52a53e00df22105c0d0}



Updates internal state (in case of digest authentication) and replaces proxy authentication information in the request accordingly.

# class `scy::http::BadRequestHandler` {#classscy_1_1http_1_1BadRequestHandler}

```
class scy::http::BadRequestHandler
  : public scy::http::ServerResponder
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  BadRequestHandler(`[`ServerConnection`](#classscy_1_1http_1_1ServerConnection)` & connection)` | 
`public inline virtual void onRequest(`[`Request`](#classscy_1_1http_1_1Request)` &,`[`Response`](#classscy_1_1http_1_1Response)` & response)` | 

## Members

#### `public inline  BadRequestHandler(`[`ServerConnection`](#classscy_1_1http_1_1ServerConnection)` & connection)` {#group__http_1ga1f07a6c70915272567c9ab3f9faca592}





#### `public inline virtual void onRequest(`[`Request`](#classscy_1_1http_1_1Request)` &,`[`Response`](#classscy_1_1http_1_1Response)` & response)` {#group__http_1gad28cbb6882e223ce278d659d2e980e59}





# class `scy::http::BasicAuthenticator` {#classscy_1_1http_1_1BasicAuthenticator}




This is a utility class for working with HTTP Basic Authentication in [http::Request](http::Request) objects.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  BasicAuthenticator()` | Creates an empty [BasicAuthenticator](./doc/api-http.md#classscy_1_1http_1_1BasicAuthenticator) object.
`public  BasicAuthenticator(const std::string & username,const std::string & password)` | 
`public  explicit BasicAuthenticator(const `[`http::Request`](#classscy_1_1http_1_1Request)` & request)` | 
`public  explicit BasicAuthenticator(const std::string & authInfo)` | 
`public  ~BasicAuthenticator()` | Destroys the [BasicAuthenticator](./doc/api-http.md#classscy_1_1http_1_1BasicAuthenticator).
`public void setUsername(const std::string & username)` | Sets the username.
`public const std::string & username() const` | Returns the username.
`public void setPassword(const std::string & password)` | Sets the password.
`public const std::string & password() const` | Returns the password.
`public void authenticate(`[`http::Request`](#classscy_1_1http_1_1Request)` & request) const` | Adds authentication information to the given [http::Request](http::Request).
`public void proxyAuthenticate(`[`http::Request`](#classscy_1_1http_1_1Request)` & request) const` | Adds proxy authentication information to the given [http::Request](http::Request).
`protected void parseAuthInfo(const std::string & authInfo)` | 

## Members

#### `public  BasicAuthenticator()` {#group__http_1ga4e2fd63defaafd17ae03fb754c2da8c1}

Creates an empty [BasicAuthenticator](#classscy_1_1http_1_1BasicAuthenticator) object.



#### `public  BasicAuthenticator(const std::string & username,const std::string & password)` {#group__http_1gad6de1b1584d452ed7e06bedc87ca1021}



Creates a [BasicAuthenticator](#classscy_1_1http_1_1BasicAuthenticator) object with the given username and password.

#### `public  explicit BasicAuthenticator(const `[`http::Request`](#classscy_1_1http_1_1Request)` & request)` {#group__http_1gaf36d893fa18c895ad0b2fe1f8316de10}



Creates a [BasicAuthenticator](#classscy_1_1http_1_1BasicAuthenticator) object with the authentication information from the given request.

Throws a NotAuthenticatedException if the request does not contain basic authentication information.

#### `public  explicit BasicAuthenticator(const std::string & authInfo)` {#group__http_1gae38dcdedcd5cdcd1c3ffd5fe28a7b1a2}



Creates a [BasicAuthenticator](#classscy_1_1http_1_1BasicAuthenticator) object with the authentication information in the given std::string. The authentication information can be extracted from a [http::Request](http::Request) object by calling [http::Request::getCredentials()](http::Request::getCredentials()).

#### `public  ~BasicAuthenticator()` {#group__http_1ga5f94ae51fc1c7c593b84a03fef64bde4}

Destroys the [BasicAuthenticator](#classscy_1_1http_1_1BasicAuthenticator).



#### `public void setUsername(const std::string & username)` {#group__http_1ga25effc506bb6d0904b86bd4d20add8c7}

Sets the username.



#### `public const std::string & username() const` {#group__http_1ga674869333bed1c65b2e38c7700a98a1e}

Returns the username.



#### `public void setPassword(const std::string & password)` {#group__http_1gabc1db19a45f0501e8ca2b29355c14fe0}

Sets the password.



#### `public const std::string & password() const` {#group__http_1ga1ed5b0686db8c1701f515e1a41b3d323}

Returns the password.



#### `public void authenticate(`[`http::Request`](#classscy_1_1http_1_1Request)` & request) const` {#group__http_1gaaae8a1c5cadebe43bb512bac3974c0bc}

Adds authentication information to the given [http::Request](http::Request).



#### `public void proxyAuthenticate(`[`http::Request`](#classscy_1_1http_1_1Request)` & request) const` {#group__http_1ga4e342080880e1ce658f95e4bf4c6fb23}

Adds proxy authentication information to the given [http::Request](http::Request).



#### `protected void parseAuthInfo(const std::string & authInfo)` {#group__http_1ga2e24fc651b06a3d5f24acc93c13039dd}



Extracts username and password from Basic authentication info by base64-decoding authInfo and splitting the resulting std::string at the ':' delimiter.

# class `scy::http::ChunkedAdapter` {#classscy_1_1http_1_1ChunkedAdapter}

```
class scy::http::ChunkedAdapter
  : public scy::PacketProcessor
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`Connection`](./doc/api-http.md#classscy_1_1http_1_1Connection)` * connection` | 
`public std::string contentType` | 
`public std::string frameSeparator` | 
`public bool initial` | 
`public bool nocopy` | 
`public `[`PacketSignal`](#group__base_1ga3ffb5bda6133fac97f3bed2b567b9b6a)` emitter` | 
`public inline  ChunkedAdapter(`[`Connection`](#classscy_1_1http_1_1Connection)` * connection,const std::string & frameSeparator,bool nocopy)` | 
`public inline  ChunkedAdapter(const std::string & contentType,const std::string & frameSeparator,bool nocopy)` | 
`public inline virtual  ~ChunkedAdapter()` | 
`public inline virtual void emitHeader()` | 
`public inline virtual void process(`[`IPacket`](#classscy_1_1IPacket)` & packet)` | 

## Members

#### `public `[`Connection`](./doc/api-http.md#classscy_1_1http_1_1Connection)` * connection` {#group__http_1ga41a3be6fd0d95c90588e24662aecdbcb}





#### `public std::string contentType` {#group__http_1ga42c0bc9dd9c269c4cb07f4312757e27b}





#### `public std::string frameSeparator` {#group__http_1ga1ab13fa89e2582407844aeff3c47b3a9}





#### `public bool initial` {#group__http_1ga8c694d7668b8ac9062561d57f764c8df}





#### `public bool nocopy` {#group__http_1ga35f74aabaea9e37fb8b90f8880fa9e15}





#### `public `[`PacketSignal`](#group__base_1ga3ffb5bda6133fac97f3bed2b567b9b6a)` emitter` {#group__http_1gabcc47b70a4402dcdf6544143881fed7d}





#### `public inline  ChunkedAdapter(`[`Connection`](#classscy_1_1http_1_1Connection)` * connection,const std::string & frameSeparator,bool nocopy)` {#group__http_1gab7f7116d8fe9d1ce35a2be8657d414f5}





#### `public inline  ChunkedAdapter(const std::string & contentType,const std::string & frameSeparator,bool nocopy)` {#group__http_1ga0992d011faef2ac9b957753eccb9523f}





#### `public inline virtual  ~ChunkedAdapter()` {#group__http_1gabf8b0cdc814e1c8f0c8f98d8afdf1397}





#### `public inline virtual void emitHeader()` {#group__http_1ga994d502823366aaef2833945c6f9d555}



Sets HTTP headers for the initial response. This method must not include the final carriage return.

#### `public inline virtual void process(`[`IPacket`](#classscy_1_1IPacket)` & packet)` {#group__http_1ga86c1844ce976a579b704074f8cea506a}



This method performs processing on the given packet and emits the result.

Note: If packet processing is async (the packet is not in the current thread scope) then packet data must be copied. Copied data can be freed directly aFter the async call to emit() the outgoing packet.

# class `scy::http::Client` {#classscy_1_1http_1_1Client}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`NullSignal`](./doc/api-base.md#classscy_1_1Signal)` Shutdown` | 
`public  Client()` | 
`public virtual  ~Client()` | 
`public void shutdown()` | Shutdown the [Client](./doc/api-http.md#classscy_1_1http_1_1Client) and close all connections.
`public template<class ConnectionT>`  <br/>`inline ClientConnection::Ptr createConnectionT(const `[`URL`](#classscy_1_1http_1_1URL)` & url,uv::Loop * loop)` | 
`public inline ClientConnection::Ptr createConnection(const `[`URL`](#classscy_1_1http_1_1URL)` & url,uv::Loop * loop)` | 
`public virtual void addConnection(ClientConnection::Ptr conn)` | 
`public virtual void removeConnection(`[`ClientConnection`](#classscy_1_1http_1_1ClientConnection)` * conn)` | 
`protected ClientConnectionPtrVec _connections` | 
`protected void onConnectionClose(`[`Connection`](#classscy_1_1http_1_1Connection)` & conn)` | 

## Members

#### `public `[`NullSignal`](./doc/api-base.md#classscy_1_1Signal)` Shutdown` {#group__http_1ga49e55ffb75fdb1ce32bcf826e01c3833}





#### `public  Client()` {#group__http_1ga440874fa8ebff5267d09bf6d51263704}





#### `public virtual  ~Client()` {#group__http_1ga77bf65808c00dc4fc81a581d3df19fb4}





#### `public void shutdown()` {#group__http_1ga7e14004363f3b40a14f62b178216abc8}

Shutdown the [Client](#classscy_1_1http_1_1Client) and close all connections.



#### `public template<class ConnectionT>`  <br/>`inline ClientConnection::Ptr createConnectionT(const `[`URL`](#classscy_1_1http_1_1URL)` & url,uv::Loop * loop)` {#group__http_1gabe89c3317e8d4c4583a8a329db3d78f4}





#### `public inline ClientConnection::Ptr createConnection(const `[`URL`](#classscy_1_1http_1_1URL)` & url,uv::Loop * loop)` {#group__http_1gab62f7d47a8d31d870945643c45dfad5c}





#### `public virtual void addConnection(ClientConnection::Ptr conn)` {#group__http_1ga267093bbb5030726e5af6f6a1f1ee085}





#### `public virtual void removeConnection(`[`ClientConnection`](#classscy_1_1http_1_1ClientConnection)` * conn)` {#group__http_1gae4a59dc7b0e3338edca92474f4057db2}





#### `protected ClientConnectionPtrVec _connections` {#group__http_1gaddcbaf554b35e8c00d96a2e12a5cd980}





#### `protected void onConnectionClose(`[`Connection`](#classscy_1_1http_1_1Connection)` & conn)` {#group__http_1gaf084fc0713c300f05031fe1bad9cac49}





# class `scy::http::ClientAdapter` {#classscy_1_1http_1_1ClientAdapter}

```
class scy::http::ClientAdapter
  : public scy::http::ConnectionAdapter
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  ClientAdapter(`[`ClientConnection`](#classscy_1_1http_1_1ClientConnection)` & connection)` | 

## Members

#### `public inline  ClientAdapter(`[`ClientConnection`](#classscy_1_1http_1_1ClientConnection)` & connection)` {#group__http_1gaa03c80de6f24df1aea954ce1b835212b}





# class `scy::http::ClientConnection` {#classscy_1_1http_1_1ClientConnection}

```
class scy::http::ClientConnection
  : public scy::http::Connection
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public void * opaque` | Optional unmanaged client data pointer.
`public `[`NullSignal`](./doc/api-base.md#classscy_1_1Signal)` Connect` | Status signals.
`public `[`Signal](#classscy_1_1Signal)< void([Response`](#classscy_1_1http_1_1Response) &)`> Headers` | Signals when the response HTTP header has been received.
`public `[`Signal](#classscy_1_1Signal)< void(const [MutableBuffer`](#classscy_1_1MutableBuffer) &)`> Payload` | Signals when raw data is received.
`public `[`Signal](#classscy_1_1Signal)< void(const [Response`](#classscy_1_1http_1_1Response) &)`> Complete` | Signals when the HTTP transaction is complete.
`public  ClientConnection(const `[`URL`](#classscy_1_1http_1_1URL)` & url,const net::Socket::Ptr & socket)` | Create a standalone connection with the given host.
`public virtual  ~ClientConnection()` | 
`public virtual void send()` | 
`public virtual void send(`[`http::Request`](#classscy_1_1http_1_1Request)` & req)` | 
`public virtual int send(const char * data,std::size_t len,int flags)` | 
`public virtual void close()` | Forcefully closes the HTTP connection.
`public virtual void setReadStream(std::ostream * os)` | 
`public template<class StreamT>`  <br/>`inline StreamT & readStream()` | Return the cast read stream pointer or nullptr.
`public virtual void onHeaders()` | Internal callbacks.
`public virtual void onPayload(const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer)` | 
`public virtual void onMessage()` | 
`public virtual void onComplete()` | 
`public virtual void onClose()` | 
`protected `[`URL`](./doc/api-http.md#classscy_1_1http_1_1URL)` _url` | 
`protected std::vector< std::string > _outgoingBuffer` | 
`protected bool _connect` | 
`protected bool _active` | 
`protected bool _complete` | 
`protected virtual void connect()` | 
`protected `[`http::Client`](#classscy_1_1http_1_1Client)` * client()` | 
`protected virtual `[`http::Message`](#classscy_1_1http_1_1Message)` * incomingHeader()` | 
`protected virtual `[`http::Message`](#classscy_1_1http_1_1Message)` * outgoingHeader()` | 
`protected virtual void onSocketConnect(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket)` | 
`protected void onHostResolved(void *,const `[`net::DNSResult`](#structscy_1_1net_1_1DNSResult)` & result)` | 

## Members

#### `public void * opaque` {#group__http_1ga10f216a8863c0fba6aeda758e921e01d}

Optional unmanaged client data pointer.



#### `public `[`NullSignal`](./doc/api-base.md#classscy_1_1Signal)` Connect` {#group__http_1ga8818169eb311460d7c88ccbe4f64e1da}

Status signals.

Signals when the client socket is connected and data can flow

#### `public `[`Signal](#classscy_1_1Signal)< void([Response`](#classscy_1_1http_1_1Response) &)`> Headers` {#group__http_1ga7a53d8d574783ef7863640c44c42784e}

Signals when the response HTTP header has been received.



#### `public `[`Signal](#classscy_1_1Signal)< void(const [MutableBuffer`](#classscy_1_1MutableBuffer) &)`> Payload` {#group__http_1gaf1db56f2d086c0c21ad31cc27a82c4bb}

Signals when raw data is received.



#### `public `[`Signal](#classscy_1_1Signal)< void(const [Response`](#classscy_1_1http_1_1Response) &)`> Complete` {#group__http_1gac8c65bf400e2ab26443afd5e100b19ef}

Signals when the HTTP transaction is complete.



#### `public  ClientConnection(const `[`URL`](#classscy_1_1http_1_1URL)` & url,const net::Socket::Ptr & socket)` {#group__http_1ga655dfc98e9fe5974db50c5e2b3c4861d}

Create a standalone connection with the given host.



#### `public virtual  ~ClientConnection()` {#group__http_1gaa6c0850500c46ca9889b77911eefa7f1}





#### `public virtual void send()` {#group__http_1ga4129126653843e0ee325ecac057e58fd}



Send the HTTP request.

Calls [connect()](#group__http_1ga3fe2e8385f1720882a418cbd6aa59766) internally if the socket is not already connecting or connected. The actual request will be sent when the socket is connected.

#### `public virtual void send(`[`http::Request`](#classscy_1_1http_1_1Request)` & req)` {#group__http_1gaae487a3b6b943ed1feb988dd5779161c}



Send the given HTTP request. The given request will overwrite the internal HTTP request object.

Calls [connect()](#group__http_1ga3fe2e8385f1720882a418cbd6aa59766) internally if the socket is not already connecting or connected. The actual request will be sent when the socket is connected.

#### `public virtual int send(const char * data,std::size_t len,int flags)` {#group__http_1ga0391c2cbf6f6e30388c0169461b4c4c7}



Send raw data to the peer. Calls [send()](#group__http_1ga4129126653843e0ee325ecac057e58fd) internally.

Throws an exception if the socket is not already or connected.

#### `public virtual void close()` {#group__http_1gab3966dace415a2d8850117fd665e9da2}

Forcefully closes the HTTP connection.



#### `public virtual void setReadStream(std::ostream * os)` {#group__http_1gacffa26d7470d96c6a43e04db03e3ac68}



Set the output stream for writing response data to. The stream pointer is managed internally, and will be freed along with the connection.

#### `public template<class StreamT>`  <br/>`inline StreamT & readStream()` {#group__http_1ga554ae0d57e01ad34dede5bfc4bc372db}

Return the cast read stream pointer or nullptr.



#### `public virtual void onHeaders()` {#group__http_1gad4a7417ccb56097b0469675b79c3ca7f}

Internal callbacks.



#### `public virtual void onPayload(const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer)` {#group__http_1gac1ba202df43a460dbaec2698f1480cea}





#### `public virtual void onMessage()` {#group__http_1gac107ada2b847156e9e61429a6b5a338b}





#### `public virtual void onComplete()` {#group__http_1gabd632fb7185b6299bd8fd391e2497b39}





#### `public virtual void onClose()` {#group__http_1ga9455d776e71602a16cfed3d45988a26e}





#### `protected `[`URL`](./doc/api-http.md#classscy_1_1http_1_1URL)` _url` {#group__http_1ga3f89a543b05fb16639adcc46729b8ee2}





#### `protected std::vector< std::string > _outgoingBuffer` {#group__http_1ga3af86cb30ab9373b79b1aa662b490ad3}





#### `protected bool _connect` {#group__http_1gacd6448a0d862e8f11eac3b4772be8470}





#### `protected bool _active` {#group__http_1ga1899b1899cf44e3f861562295b548e59}





#### `protected bool _complete` {#group__http_1ga592202659ff77bbf122085a786df4ff6}





#### `protected virtual void connect()` {#group__http_1ga3fe2e8385f1720882a418cbd6aa59766}



Connects to the server endpoint. All sent data is buffered until the connection is made.

#### `protected `[`http::Client`](#classscy_1_1http_1_1Client)` * client()` {#group__http_1ga0e9db5b252d791a5e280ca40bec65a45}





#### `protected virtual `[`http::Message`](#classscy_1_1http_1_1Message)` * incomingHeader()` {#group__http_1ga0dd5e775baa390c4b0d846ee76bc5c5f}





#### `protected virtual `[`http::Message`](#classscy_1_1http_1_1Message)` * outgoingHeader()` {#group__http_1gaf4c6c89221eec5820ff9b6a37fd65ac0}





#### `protected virtual void onSocketConnect(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket)` {#group__http_1gaf90d5fe1830521ae3053ef6672843f71}



These virtual methods can be overridden as necessary to intercept socket events before they hit the application.

#### `protected void onHostResolved(void *,const `[`net::DNSResult`](#structscy_1_1net_1_1DNSResult)` & result)` {#group__http_1gaad98cafb4b2f59638356635d6be6f856}





# class `scy::http::Connection` {#classscy_1_1http_1_1Connection}

```
class scy::http::Connection
  : public scy::net::SocketAdapter
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`PacketStream`](./doc/api-base.md#classscy_1_1PacketStream)` Outgoing` | 
`public `[`PacketStream`](./doc/api-base.md#classscy_1_1PacketStream)` Incoming` | 
`public `[`ProgressSignal`](./doc/api-http.md#classscy_1_1http_1_1ProgressSignal)` IncomingProgress` | Fired on download progress.
`public `[`ProgressSignal`](./doc/api-http.md#classscy_1_1http_1_1ProgressSignal)` OutgoingProgress` | Fired on upload progress.
`public `[`Signal](#classscy_1_1Signal)< void([Connection`](#classscy_1_1http_1_1Connection) &)`> Close` | 
`public  Connection(const net::Socket::Ptr & socket)` | 
`public virtual  ~Connection()` | 
`public virtual int send(const char * data,std::size_t len,int flags)` | Send raw data to the peer.
`public virtual int sendHeader()` | Send the outdoing HTTP header.
`public virtual void close()` | 
`public bool closed() const` | Return true if the connection is closed.
`public `[`scy::Error`](#structscy_1_1Error)` error() const` | Return the error object if any.
`public void onHeaders()` | 
`public inline virtual void onPayload(const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` &)` | 
`public void onMessage()` | 
`public virtual void onClose()` | 
`public bool shouldSendHeader() const` | 
`public void shouldSendHeader(bool flag)` | Set true to prevent auto-sending HTTP headers.
`public void replaceAdapter(`[`net::SocketAdapter`](#classscy_1_1net_1_1SocketAdapter)` * adapter)` | 
`public net::Socket::Ptr & socket()` | Return the underlying socket pointer.
`public `[`Request`](#classscy_1_1http_1_1Request)` & request()` | The HTTP request headers.
`public `[`Response`](#classscy_1_1http_1_1Response)` & response()` | The HTTP response headers.
`public `[`http::Message`](#classscy_1_1http_1_1Message)` * incomingHeader()` | 
`public `[`http::Message`](#classscy_1_1http_1_1Message)` * outgoingHeader()` | 
`protected net::Socket::Ptr _socket` | 
`protected `[`SocketAdapter`](#group__net_1gaf9ced47f3c1aa08ccea3042997a2a320)` * _adapter` | 
`protected `[`Request`](./doc/api-http.md#classscy_1_1http_1_1Request)` _request` | 
`protected `[`Response`](./doc/api-http.md#classscy_1_1http_1_1Response)` _response` | 
`protected `[`scy::Error`](./doc/api-base.md#structscy_1_1Error)` _error` | 
`protected bool _closed` | 
`protected bool _shouldSendHeader` | 
`protected virtual void onSocketConnect(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket)` | 
`protected virtual void onSocketRecv(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket,const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
`protected virtual void onSocketError(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket,const `[`scy::Error`](#structscy_1_1Error)` & error)` | 
`protected virtual void onSocketClose(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket)` | 
`protected virtual void setError(const `[`scy::Error`](#structscy_1_1Error)` & err)` | 

## Members

#### `public `[`PacketStream`](./doc/api-base.md#classscy_1_1PacketStream)` Outgoing` {#group__http_1ga55b968b2e008641e8dbc5f6a4ac5507c}



The Outgoing stream is responsible for packetizing raw application data into the agreed upon HTTP format and sending it to the peer.

#### `public `[`PacketStream`](./doc/api-base.md#classscy_1_1PacketStream)` Incoming` {#group__http_1gafc445ad6bf4dbb79edfff267fef6fb19}



The Incoming stream emits incoming HTTP packets for processing by the application.

This is useful for example when writing incoming data to a file.

#### `public `[`ProgressSignal`](./doc/api-http.md#classscy_1_1http_1_1ProgressSignal)` IncomingProgress` {#group__http_1ga867abecb125b3505e8c1987d170890a4}

Fired on download progress.



#### `public `[`ProgressSignal`](./doc/api-http.md#classscy_1_1http_1_1ProgressSignal)` OutgoingProgress` {#group__http_1ga901259e61a69f7f248382e8e18472d99}

Fired on upload progress.



#### `public `[`Signal](#classscy_1_1Signal)< void([Connection`](#classscy_1_1http_1_1Connection) &)`> Close` {#group__http_1ga07721462a2bf8648c8378a264abc01e2}





#### `public  Connection(const net::Socket::Ptr & socket)` {#group__http_1ga7c28dbc580b5887ee2f53e04ae741807}





#### `public virtual  ~Connection()` {#group__http_1ga34b5926baf06240530f2194af8865071}





#### `public virtual int send(const char * data,std::size_t len,int flags)` {#group__http_1gaec9b91946254087216e12b0e321062d6}

Send raw data to the peer.



#### `public virtual int sendHeader()` {#group__http_1ga19087b52625b1f56d273f6c23a1aed82}

Send the outdoing HTTP header.



#### `public virtual void close()` {#group__http_1ga7c40813c43b7e09e0b52ec8f33378dc9}



Close the connection and schedule the object for deferred deletion.

#### `public bool closed() const` {#group__http_1gaeaf613f9c94f3e93b981b4929f5a2406}

Return true if the connection is closed.



#### `public `[`scy::Error`](#structscy_1_1Error)` error() const` {#group__http_1gae097a4b0be0ce96f55adb7f099eb21d4}

Return the error object if any.



#### `public void onHeaders()` {#group__http_1ga336fbc30418a29d18531050fab48823a}



Return true if the server did not give us a proper response within the allotted time. bool expired() const;

#### `public inline virtual void onPayload(const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` &)` {#group__http_1ga2bb0e762757545d0e38acba07aae6f65}





#### `public void onMessage()` {#group__http_1ga1bfc9ced21781297c7ed58944ed9367f}





#### `public virtual void onClose()` {#group__http_1ga044ce830989b118cc3d73faa5851df2f}





#### `public bool shouldSendHeader() const` {#group__http_1ga38cfd15a7f82a369eaebddf5e4d5bc0c}





#### `public void shouldSendHeader(bool flag)` {#group__http_1gad8e7071bd3cd8162ddfa739ed4a30f94}

Set true to prevent auto-sending HTTP headers.



#### `public void replaceAdapter(`[`net::SocketAdapter`](#classscy_1_1net_1_1SocketAdapter)` * adapter)` {#group__http_1ga0eb7c6200dd3c7e6d78a96f0100b1d9c}



Assign the new [ConnectionAdapter](#classscy_1_1http_1_1ConnectionAdapter) and setup the chain The flow is: [Connection](#classscy_1_1http_1_1Connection) <-> [ConnectionAdapter](#classscy_1_1http_1_1ConnectionAdapter) <-> Socket

#### `public net::Socket::Ptr & socket()` {#group__http_1ga37609ef570181ce95d34c0c7ba594080}

Return the underlying socket pointer.



#### `public `[`Request`](#classscy_1_1http_1_1Request)` & request()` {#group__http_1ga43cc676959b5eb284d9f67a911bbca36}

The HTTP request headers.



#### `public `[`Response`](#classscy_1_1http_1_1Response)` & response()` {#group__http_1ga69d78cf331ae34a8f57a226f4c78d6f6}

The HTTP response headers.



#### `public `[`http::Message`](#classscy_1_1http_1_1Message)` * incomingHeader()` {#group__http_1ga8626a625ce1415857cdd638d1520b7c9}





#### `public `[`http::Message`](#classscy_1_1http_1_1Message)` * outgoingHeader()` {#group__http_1ga414cac25b0961ad9013e0178b7a540d4}





#### `protected net::Socket::Ptr _socket` {#group__http_1ga627e783aef97f4b64ce8ae96524b15c2}





#### `protected `[`SocketAdapter`](#group__net_1gaf9ced47f3c1aa08ccea3042997a2a320)` * _adapter` {#group__http_1ga324c822d903de2d82577769c4a7a69ab}





#### `protected `[`Request`](./doc/api-http.md#classscy_1_1http_1_1Request)` _request` {#group__http_1gaa8acc765dd7da86268d4534053be3497}





#### `protected `[`Response`](./doc/api-http.md#classscy_1_1http_1_1Response)` _response` {#group__http_1ga6bce4856f6f0f6daba2ae58e620dd392}





#### `protected `[`scy::Error`](./doc/api-base.md#structscy_1_1Error)` _error` {#group__http_1ga65dddb1c2b352b6e129457c70f762b7c}





#### `protected bool _closed` {#group__http_1ga930e46e3b07a1919b617a6e037959e99}





#### `protected bool _shouldSendHeader` {#group__http_1ga4895da546cdb23fbd3fd8be4fe2f6bb9}





#### `protected virtual void onSocketConnect(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket)` {#group__http_1gaa75f67bb69ac9ace3e6715abec3ce4af}



These virtual methods can be overridden as necessary to intercept socket events before they hit the application.

#### `protected virtual void onSocketRecv(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket,const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` {#group__http_1gab91f2cbe3aceb285e67469af0893135f}





#### `protected virtual void onSocketError(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket,const `[`scy::Error`](#structscy_1_1Error)` & error)` {#group__http_1gab7e5a89654355ad1679ed933f5aab469}





#### `protected virtual void onSocketClose(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket)` {#group__http_1gaea7315f5a4619a851a23c652b8db23d8}





#### `protected virtual void setError(const `[`scy::Error`](#structscy_1_1Error)` & err)` {#group__http_1gaebeaf7cb7d32b02026a0f608d4ffd4a9}



Set the internal error. Note: Setting the error does not `[close()](#group__http_1ga7c40813c43b7e09e0b52ec8f33378dc9)` the connection.

# class `scy::http::ConnectionAdapter` {#classscy_1_1http_1_1ConnectionAdapter}

```
class scy::http::ConnectionAdapter
  : public scy::http::ParserObserver
  : public scy::net::SocketAdapter
```  

Default HTTP socket adapter for reading and writing HTTP messages.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  ConnectionAdapter(`[`Connection`](#classscy_1_1http_1_1Connection)` & connection,http_parser_type type)` | 
`public virtual  ~ConnectionAdapter()` | 
`public virtual int send(const char * data,std::size_t len,int flags)` | 
`public `[`Parser`](#classscy_1_1http_1_1Parser)` & parser()` | 
`public `[`Connection`](#classscy_1_1http_1_1Connection)` & connection()` | 
`protected `[`Connection`](./doc/api-http.md#classscy_1_1http_1_1Connection)` & _connection` | 
`protected `[`Parser`](./doc/api-http.md#classscy_1_1http_1_1Parser)` _parser` | 
`protected virtual void onSocketRecv(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket,const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
`protected virtual void onParserHeader(const std::string & name,const std::string & value)` | 
`protected virtual void onParserHeadersEnd()` | 
`protected virtual void onParserChunk(const char * buf,std::size_t len)` | 
`protected virtual void onParserError(const `[`ParserError`](#structscy_1_1http_1_1ParserError)` & err)` | 
`protected virtual void onParserEnd()` | 

## Members

#### `public  ConnectionAdapter(`[`Connection`](#classscy_1_1http_1_1Connection)` & connection,http_parser_type type)` {#group__http_1ga42139e22afe1b82bb885ca75520d6fa5}





#### `public virtual  ~ConnectionAdapter()` {#group__http_1gadf323289c48fe19dddc2bd2b9eab9329}





#### `public virtual int send(const char * data,std::size_t len,int flags)` {#group__http_1ga51203b93b52a7202788261871d542b3c}



Sends the given data buffer to the connected peer. Returns the number of bytes sent or -1 on error. No exception will be thrown. For TCP sockets the given peer address must match the connected peer address.

#### `public `[`Parser`](#classscy_1_1http_1_1Parser)` & parser()` {#group__http_1ga1a4df38a8eb53687fbdf13177dd9cc1c}





#### `public `[`Connection`](#classscy_1_1http_1_1Connection)` & connection()` {#group__http_1ga3ae51326364bc15abe4bcf1802c1df18}





#### `protected `[`Connection`](./doc/api-http.md#classscy_1_1http_1_1Connection)` & _connection` {#group__http_1gafbaca6a50ce7e935a7a3ad9b0f12c173}





#### `protected `[`Parser`](./doc/api-http.md#classscy_1_1http_1_1Parser)` _parser` {#group__http_1ga02352190d724739060c896e0acc743c0}





#### `protected virtual void onSocketRecv(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket,const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` {#group__http_1gaf675696c310d06f65e3110c206b1991c}





#### `protected virtual void onParserHeader(const std::string & name,const std::string & value)` {#group__http_1ga611f889d794434f91f6b36c60a995bc5}





#### `protected virtual void onParserHeadersEnd()` {#group__http_1gadab35d703139caf9abb81d04af6eb362}





#### `protected virtual void onParserChunk(const char * buf,std::size_t len)` {#group__http_1gaf38a937e8a2e7fc2f1277876d7e21ffb}





#### `protected virtual void onParserError(const `[`ParserError`](#structscy_1_1http_1_1ParserError)` & err)` {#group__http_1ga76152ce88609722824f4cbddd5299d18}





#### `protected virtual void onParserEnd()` {#group__http_1gac45acb1992c50b24b438f830f4e8a245}





# class `scy::http::Cookie` {#classscy_1_1http_1_1Cookie}




This class represents a HTTP [Cookie](#classscy_1_1http_1_1Cookie).

A cookie is a small amount of information sent by a Web server to a Web browser, saved by the browser, and later sent back to the server. A cookie's value can uniquely identify a client, so cookies are commonly used for session management.

A cookie has a name, a single value, and optional attributes such as a comment, path and domain qualifiers, a maximum age, and a version number.

This class supports both the Version 0 (by Netscape) and Version 1 (by RFC 2109) cookie specifications. By default, cookies are created using Version 0 to ensure the best interoperability.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Cookie()` | Creates an empty [Cookie](./doc/api-http.md#classscy_1_1http_1_1Cookie).
`public  explicit Cookie(const std::string & name)` | 
`public  explicit Cookie(const `[`NVCollection`](#classscy_1_1NVCollection)` & nvc)` | Creates a cookie from the given [NVCollection](./doc/api-base.md#classscy_1_1NVCollection).
`public  Cookie(const std::string & name,const std::string & value)` | 
`public  Cookie(const `[`Cookie`](#classscy_1_1http_1_1Cookie)` & cookie)` | Creates the [Cookie](./doc/api-http.md#classscy_1_1http_1_1Cookie) by copying another one.
`public  ~Cookie()` | Destroys the [Cookie](./doc/api-http.md#classscy_1_1http_1_1Cookie).
`public `[`Cookie`](#classscy_1_1http_1_1Cookie)` & operator=(const `[`Cookie`](#classscy_1_1http_1_1Cookie)` & cookie)` | Assigns a cookie.
`public void setVersion(int version)` | 
`public inline int getVersion() const` | 
`public void setName(const std::string & name)` | Sets the name of the cookie.
`public inline const std::string & getName() const` | Returns the name of the cookie.
`public void setValue(const std::string & value)` | 
`public inline const std::string & getValue() const` | Returns the value of the cookie.
`public void setComment(const std::string & comment)` | 
`public inline const std::string & getComment() const` | Returns the comment for the cookie.
`public void setDomain(const std::string & domain)` | Sets the domain for the cookie.
`public inline const std::string & getDomain() const` | Returns the domain for the cookie.
`public void setPath(const std::string & path)` | Sets the path for the cookie.
`public inline const std::string & getPath() const` | Returns the path for the cookie.
`public void setSecure(bool secure)` | 
`public inline bool getSecure() const` | 
`public void setMaxAge(int maxAge)` | 
`public inline int getMaxAge() const` | 
`public void setHttpOnly(bool flag)` | Sets the HttpOnly flag for the cookie.
`public inline bool getHttpOnly() const` | Returns true if the cookie's HttpOnly flag is set.
`public std::string toString() const` | 

## Members

#### `public  Cookie()` {#group__http_1ga7fe027bf50ad3e074e3394cee3da9f08}

Creates an empty [Cookie](#classscy_1_1http_1_1Cookie).



#### `public  explicit Cookie(const std::string & name)` {#group__http_1ga027e2a2f4f74f8a9908e4558c7749912}



Creates a cookie with the given name. The cookie never expires.

#### `public  explicit Cookie(const `[`NVCollection`](#classscy_1_1NVCollection)` & nvc)` {#group__http_1ga691e1a1ad74b1dcb68045335491965b5}

Creates a cookie from the given [NVCollection](#classscy_1_1NVCollection).



#### `public  Cookie(const std::string & name,const std::string & value)` {#group__http_1gaa2944b0e82cf14e9001cb33706aa1197}



Creates a cookie with the given name and value. The cookie never expires.

Note: If value contains whitespace or non-alphanumeric characters, the value should be escaped by calling [escape()](#group__http_1ga5dc9fb3d11c40a91b69c562135a4a5f8) before passing it to the constructor.

#### `public  Cookie(const `[`Cookie`](#classscy_1_1http_1_1Cookie)` & cookie)` {#group__http_1ga12f322ffcbc6b333f022ea1c646a89c9}

Creates the [Cookie](#classscy_1_1http_1_1Cookie) by copying another one.



#### `public  ~Cookie()` {#group__http_1gaa1237673ec2466ff08022b689b3149ab}

Destroys the [Cookie](#classscy_1_1http_1_1Cookie).



#### `public `[`Cookie`](#classscy_1_1http_1_1Cookie)` & operator=(const `[`Cookie`](#classscy_1_1http_1_1Cookie)` & cookie)` {#group__http_1gaf79ee72cc2c039956c3b383b51ba67f6}

Assigns a cookie.



#### `public void setVersion(int version)` {#group__http_1ga0503c149157a6c1f7d3c2d4232e91eff}



Sets the version of the cookie.

Version must be either 0 (denoting a Netscape cookie) or 1 (denoting a RFC 2109 cookie).

#### `public inline int getVersion() const` {#group__http_1ga6bba92249ab2db70324e8563e944de61}



Returns the version of the cookie, which is either 0 or 1.

#### `public void setName(const std::string & name)` {#group__http_1gad8867c1d8c54497acc3b251b9086f73d}

Sets the name of the cookie.



#### `public inline const std::string & getName() const` {#group__http_1ga19a1d78ead5a8f7bd2e857dcaeb01afe}

Returns the name of the cookie.



#### `public void setValue(const std::string & value)` {#group__http_1gae819d97814ae5c7b225e5678215a8c93}



Sets the value of the cookie.

According to the cookie specification, the size of the value should not exceed 4 Kbytes.

Note: If value contains whitespace or non-alphanumeric characters, the value should be escaped by calling [escape()](#group__http_1ga5dc9fb3d11c40a91b69c562135a4a5f8) prior to passing it to [setName()](#group__http_1gad8867c1d8c54497acc3b251b9086f73d).

#### `public inline const std::string & getValue() const` {#group__http_1ga2ce5ad4ace4ef72042b5d66ba1073e52}

Returns the value of the cookie.



#### `public void setComment(const std::string & comment)` {#group__http_1ga2ab87e18725de24a714047755a571624}



Sets the comment for the cookie.

Comments are only supported for version 1 cookies.

#### `public inline const std::string & getComment() const` {#group__http_1gada9b4df4051c57243a739984aa91941e}

Returns the comment for the cookie.



#### `public void setDomain(const std::string & domain)` {#group__http_1gada05d2d453ce8bd5ce9b98fe78344af5}

Sets the domain for the cookie.



#### `public inline const std::string & getDomain() const` {#group__http_1ga0b19831c3a5651e2b168445dbb324d09}

Returns the domain for the cookie.



#### `public void setPath(const std::string & path)` {#group__http_1gac36b773a25ed2a55dfd327218d42d2d1}

Sets the path for the cookie.



#### `public inline const std::string & getPath() const` {#group__http_1ga15927613e58d7b0020941fb0f26560ad}

Returns the path for the cookie.



#### `public void setSecure(bool secure)` {#group__http_1ga6f7cc92be1f67f178283bdf898f93cee}



Sets the value of the secure flag for the cookie.

#### `public inline bool getSecure() const` {#group__http_1gaf1af920b2dc5cc4d2e12d12ab7221b2f}



Returns the value of the secure flag for the cookie.

#### `public void setMaxAge(int maxAge)` {#group__http_1ga9cbe3e7dcb2b1ddfd5b5ef015dc3d04d}



Sets the maximum age in seconds for the cookie.

A value of -1 causes the cookie to never expire on the client.

A value of 0 deletes the cookie on the client.

#### `public inline int getMaxAge() const` {#group__http_1gacf9466fafc045014b7a8645b9e02d0ed}



Returns the maximum age in seconds for the cookie.

#### `public void setHttpOnly(bool flag)` {#group__http_1gabe8656c52f52eaf3efe62df8144bfe72}

Sets the HttpOnly flag for the cookie.



#### `public inline bool getHttpOnly() const` {#group__http_1ga1ed3b24d77125d5162c70c744912005c}

Returns true if the cookie's HttpOnly flag is set.



#### `public std::string toString() const` {#group__http_1ga687bf0e8583f850b75df212a4daf25a2}



Returns a std::string representation of the cookie, suitable for use in a Set-Cookie header.

# class `scy::http::FilePart` {#classscy_1_1http_1_1FilePart}

```
class scy::http::FilePart
  : public scy::http::FormPart
```  

An implementation of [FilePart](#classscy_1_1http_1_1FilePart) for plain files.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  FilePart(const std::string & path)` | 
`public  FilePart(const std::string & path,const std::string & contentType)` | 
`public  FilePart(const std::string & path,const std::string & filename,const std::string & contentType)` | 
`public virtual  ~FilePart()` | Destroys the [FilePart](./doc/api-http.md#classscy_1_1http_1_1FilePart).
`public virtual void open()` | 
`public virtual void reset()` | Reset the internal state and write position to the start.
`public virtual bool writeChunk(`[`FormWriter`](#classscy_1_1http_1_1FormWriter)` & writer)` | 
`public virtual void write(`[`FormWriter`](#classscy_1_1http_1_1FormWriter)` & writer)` | Writes the form data to the given HTTP client connection.
`public virtual void write(std::ostream & ostr)` | Writes the form data to the given output stream.
`public const std::string & filename() const` | Returns the filename portion of the path.
`public std::ifstream & stream()` | Returns the file input stream.
`public virtual std::uint64_t length() const` | Returns the length of the current part.
`protected std::string _path` | 
`protected std::string _filename` | 
`protected std::ifstream _istr` | 
`protected std::uint64_t _fileSize` | 

## Members

#### `public  FilePart(const std::string & path)` {#group__http_1ga3965f033a1a66974801bf1ecb2a65e43}



Creates the [FilePart](#classscy_1_1http_1_1FilePart) for the given path.

The MIME type is set to application/octet-stream.

Throws an FileException if the file cannot be opened.

#### `public  FilePart(const std::string & path,const std::string & contentType)` {#group__http_1ga0543f767567ba948d3f02b3de6020011}



Creates the [FilePart](#classscy_1_1http_1_1FilePart) for the given path and MIME type.

Throws an FileException if the file cannot be opened.

#### `public  FilePart(const std::string & path,const std::string & filename,const std::string & contentType)` {#group__http_1ga18dea4dba60832a3b9ab08fd1fc0a6fe}



Creates the [FilePart](#classscy_1_1http_1_1FilePart) for the given path and MIME type. The given filename is used as part filename (see [filename()](#group__http_1gad2765df66b173afe804630d0294b57d2)) only.

Throws an FileException if the file cannot be opened.

#### `public virtual  ~FilePart()` {#group__http_1gad1cba409e3ebbb99a8d90a70eaf68938}

Destroys the [FilePart](#classscy_1_1http_1_1FilePart).



#### `public virtual void open()` {#group__http_1ga3254f30104cd2cba62847b430ba1a6b7}



Opens the file.

Throws an FileException if the file cannot be opened.

#### `public virtual void reset()` {#group__http_1ga8e47a18cf77f0ddfbbdb95e4be54502d}

Reset the internal state and write position to the start.



#### `public virtual bool writeChunk(`[`FormWriter`](#classscy_1_1http_1_1FormWriter)` & writer)` {#group__http_1ga79720d0fc2bd52c655162db91b154605}



Writes a form data chunk to the given HTTP client connection. Returns true if there is more data to be written.

#### `public virtual void write(`[`FormWriter`](#classscy_1_1http_1_1FormWriter)` & writer)` {#group__http_1ga4aed2e1707e335c71c14a974aeaad47e}

Writes the form data to the given HTTP client connection.



#### `public virtual void write(std::ostream & ostr)` {#group__http_1gac83ece1e9f7236b20b7c17fe885ab6d9}

Writes the form data to the given output stream.



#### `public const std::string & filename() const` {#group__http_1gad2765df66b173afe804630d0294b57d2}

Returns the filename portion of the path.



#### `public std::ifstream & stream()` {#group__http_1ga8ef4a43e06b5e9aecd8e008fe06bbd41}

Returns the file input stream.



#### `public virtual std::uint64_t length() const` {#group__http_1ga1392d431731dd43b412607bc7b6f291d}

Returns the length of the current part.



#### `protected std::string _path` {#group__http_1gac84b6cb214c304f0685e3db164c5e9d4}





#### `protected std::string _filename` {#group__http_1ga5a022ac797b9194b540ce9e5128f54d4}





#### `protected std::ifstream _istr` {#group__http_1gac44819c6c5d495ad370e0773ba15f341}





#### `protected std::uint64_t _fileSize` {#group__http_1ga254530c7676713788891aaa2ec8f5525}





# class `scy::http::FormPart` {#classscy_1_1http_1_1FormPart}


An implementation of [FormPart](#classscy_1_1http_1_1FormPart).



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  FormPart(const std::string & contentType)` | Creates the [FormPart](./doc/api-http.md#classscy_1_1http_1_1FormPart) with the given MIME type.
`public virtual  ~FormPart()` | Destroys the [FormPart](./doc/api-http.md#classscy_1_1http_1_1FormPart).
`public virtual void reset()` | Reset the internal state and write position to the start.
`public bool writeChunk(`[`FormWriter`](#classscy_1_1http_1_1FormWriter)` & writer)` | 
`public void write(`[`FormWriter`](#classscy_1_1http_1_1FormWriter)` & writer)` | Writes the form data to the given HTTP client connection.
`public void write(std::ostream & ostr)` | Writes the form data to the given output stream.
`public `[`NVCollection`](#classscy_1_1NVCollection)` & headers()` | 
`public virtual bool initialWrite() const` | Returns true if this is the initial write.
`public const std::string & contentType() const` | Returns the MIME type for this part or attachment.
`public std::uint64_t length() const` | Returns the length of the current part.
`protected std::string _contentType` | 
`protected std::uint64_t _length` | 
`protected `[`NVCollection`](./doc/api-base.md#classscy_1_1NVCollection)` _headers` | 
`protected bool _initialWrite` | 

## Members

#### `public  FormPart(const std::string & contentType)` {#group__http_1gaddfd1e7e0464b40e773a806e1d30ec4f}

Creates the [FormPart](#classscy_1_1http_1_1FormPart) with the given MIME type.



#### `public virtual  ~FormPart()` {#group__http_1ga1771d5947b6f1c8d4595f70d6d2ba79a}

Destroys the [FormPart](#classscy_1_1http_1_1FormPart).



#### `public virtual void reset()` {#group__http_1gac1b2db21062b5770195b6217da2ee7f6}

Reset the internal state and write position to the start.



#### `public bool writeChunk(`[`FormWriter`](#classscy_1_1http_1_1FormWriter)` & writer)` {#group__http_1ga10b82f2f86dbd9dd27fdd3321d4d09f0}



Writes a form data chunk to the given HTTP client connection. Returns true if there is more data to be written.

#### `public void write(`[`FormWriter`](#classscy_1_1http_1_1FormWriter)` & writer)` {#group__http_1gafcc4b878645c0b9b1a25986827f8002a}

Writes the form data to the given HTTP client connection.



#### `public void write(std::ostream & ostr)` {#group__http_1ga9364dc83c243a4334a3157f7d7856bca}

Writes the form data to the given output stream.



#### `public `[`NVCollection`](#classscy_1_1NVCollection)` & headers()` {#group__http_1ga16a42c613efc74d90591aad48992c5f2}



Returns a [NVCollection](#classscy_1_1NVCollection) containing additional header fields for the part.

#### `public virtual bool initialWrite() const` {#group__http_1ga0aeef9590266d57540c1c381f81bc3d9}

Returns true if this is the initial write.



#### `public const std::string & contentType() const` {#group__http_1ga5b33a860f6532933b853ef1bb79b53fe}

Returns the MIME type for this part or attachment.



#### `public std::uint64_t length() const` {#group__http_1gab6d94f98f5220257bdb129e705a81227}

Returns the length of the current part.



#### `protected std::string _contentType` {#group__http_1ga99b49cdefb7941eaa59014a6ffac990c}





#### `protected std::uint64_t _length` {#group__http_1ga0adee93ca1e2cbf2f6518336836d07cd}





#### `protected `[`NVCollection`](./doc/api-base.md#classscy_1_1NVCollection)` _headers` {#group__http_1ga1610eaa082d3154399710dba0bc4b1e4}





#### `protected bool _initialWrite` {#group__http_1ga4e0d100858d98915b243591998133d4a}





# class `scy::http::FormWriter` {#classscy_1_1http_1_1FormWriter}

```
class scy::http::FormWriter
  : public scy::NVCollection
  : public scy::PacketStreamAdapter
  : public scy::async::Startable
```  



[FormWriter](#classscy_1_1http_1_1FormWriter) is a HTTP client connection adapter for writing HTML forms.

This class runs in its own thread so as not to block the event loop while uploading big files. Class members are not synchronized hence they should not be accessed while the form is sending, not that there would be any reason to do so.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`PacketSignal`](#group__base_1ga3ffb5bda6133fac97f3bed2b567b9b6a)` emitter` | The outgoing packet emitter.
`public virtual  ~FormWriter()` | Destroys the [FormWriter](./doc/api-http.md#classscy_1_1http_1_1FormWriter).
`public void addPart(const std::string & name,`[`FormPart`](#classscy_1_1http_1_1FormPart)` * part)` | 
`public virtual void start()` | Starts the sending thread.
`public virtual void stop()` | Stops the sending thread.
`public bool complete() const` | Returns true if the request is complete.
`public bool cancelled() const` | Returns true if the request is cancelled.
`public void prepareSubmit()` | 
`public std::uint64_t calculateMultipartContentLength()` | 
`public void writeUrl(std::ostream & ostr)` | 
`public void writeMultipartChunk()` | 
`public void writeAsync()` | 
`public void setEncoding(const std::string & encoding)` | 
`public const std::string & encoding() const` | Returns the encoding used for posting the form.
`public void setBoundary(const std::string & boundary)` | 
`public const std::string & boundary() const` | Returns the MIME boundary used for writing multipart form data.
`public `[`ClientConnection`](#classscy_1_1http_1_1ClientConnection)` & connection()` | The associated HTTP client connection.
`protected `[`ClientConnection`](./doc/api-http.md#classscy_1_1http_1_1ClientConnection)` & _connection` | 
`protected async::Runner::Ptr _runner` | 
`protected std::string _encoding` | 
`protected std::string _boundary` | 
`protected PartQueue _parts` | 
`protected std::uint64_t _filesLength` | 
`protected int _writeState` | 
`protected bool _initial` | 
`protected bool _complete` | 
`protected  FormWriter(`[`ClientConnection`](#classscy_1_1http_1_1ClientConnection)` & conn,async::Runner::Ptr runner,const std::string & encoding)` | 
`protected  FormWriter(const `[`FormWriter`](#classscy_1_1http_1_1FormWriter)` &)` | 
`protected `[`FormWriter`](#classscy_1_1http_1_1FormWriter)` & operator=(const `[`FormWriter`](#classscy_1_1http_1_1FormWriter)` &)` | 
`protected void writePartHeader(const `[`NVCollection`](#classscy_1_1NVCollection)` & header,std::ostream & ostr)` | 
`protected void writeEnd(std::ostream & ostr)` | Writes the final boundary std::string to the output stream.
`protected virtual void updateProgress(int nread)` | 

## Members

#### `public `[`PacketSignal`](#group__base_1ga3ffb5bda6133fac97f3bed2b567b9b6a)` emitter` {#group__http_1gab2466a31197f14952d9591b1d5c4a1ad}

The outgoing packet emitter.



#### `public virtual  ~FormWriter()` {#group__http_1gab41f26d58a5eceed4d3dca809b2fffd6}

Destroys the [FormWriter](#classscy_1_1http_1_1FormWriter).



#### `public void addPart(const std::string & name,`[`FormPart`](#classscy_1_1http_1_1FormPart)` * part)` {#group__http_1gaa9e9d1c89a622c3411d899249e66c95f}



Adds an part/attachment (file upload) to the form.

The form takes ownership of the [FilePart](#classscy_1_1http_1_1FilePart) and deletes it when it is no longer needed. The part will only be sent if the encoding set for the form is "multipart/form-data"

#### `public virtual void start()` {#group__http_1gaa5f7896cd6fdc03332ca24ce61a21cc9}

Starts the sending thread.



#### `public virtual void stop()` {#group__http_1gada956df3e4d829e6f8089bcb8131f4b6}

Stops the sending thread.



#### `public bool complete() const` {#group__http_1gac09b25eb5461de108430eec710d68675}

Returns true if the request is complete.



#### `public bool cancelled() const` {#group__http_1ga9f19254603100e55352b7550d76d5c73}

Returns true if the request is cancelled.



#### `public void prepareSubmit()` {#group__http_1gaa5769359cbcdf8addf6da8dc3f45d283}



Prepares the outgoing HTTP request object for submitting the form.

If the request method is GET, the encoded form is appended to the request URI as query std::string. Otherwise (the method is POST), the form's content type is set to the form's encoding. The form's parameters must be written to the request body separately, with a call to write. If the request's HTTP version is HTTP/1.0:

* persistent connections are disabled


* the content transfer encoding is set to identity encoding Otherwise, if the request's HTTP version is HTTP/1.1:


* the request's persistent connection state is left unchanged


* the content transfer encoding is set to chunked

#### `public std::uint64_t calculateMultipartContentLength()` {#group__http_1ga7a66709247bfcd6a314e73224847b1dc}



Processes the entire stream and calculates the content length. Not used for chunked encoding.

#### `public void writeUrl(std::ostream & ostr)` {#group__http_1gae991f982b59ea236144c4afc93a1f212}



Writes "application/x-www-form-urlencoded" encoded data to the client connection.

#### `public void writeMultipartChunk()` {#group__http_1ga52a768c3b4f38dd4d626f9d3f0e69c9f}



Writes the next multipart "multipart/form-data" encoded to the client connection. This method is non-blocking, // and is suitable for use with the event loop.

#### `public void writeAsync()` {#group__http_1ga316ac15771f371e398cd9897cf983675}



Called asynchronously by the Runner to write the next message chunk. If "multipart/form-data" the next multipart chunk will be written. If "application/x-www-form-urlencoded" the entire message will be written. The complete flag will be set when the entire request has been written.

#### `public void setEncoding(const std::string & encoding)` {#group__http_1gaff90cfef483b9111d1ecd987744e0371}



Sets the encoding used for posting the form.

Encoding must be either "application/x-www-form-urlencoded" (which is the default) or "multipart/form-data".

#### `public const std::string & encoding() const` {#group__http_1ga3d3d1bd0b01df47b5e02abe86cccd03b}

Returns the encoding used for posting the form.



#### `public void setBoundary(const std::string & boundary)` {#group__http_1ga385dfb4313fd4b63c90326edb3d8bf2a}



Sets the boundary to use for separating form parts. Must be set before [prepareSubmit()](#group__http_1gaa5769359cbcdf8addf6da8dc3f45d283) is called.

#### `public const std::string & boundary() const` {#group__http_1ga08c75f60ff2b194f6eca60124cd5ac7c}

Returns the MIME boundary used for writing multipart form data.



#### `public `[`ClientConnection`](#classscy_1_1http_1_1ClientConnection)` & connection()` {#group__http_1ga7817a64e1777aa5245a474d91ef58eb8}

The associated HTTP client connection.



#### `protected `[`ClientConnection`](./doc/api-http.md#classscy_1_1http_1_1ClientConnection)` & _connection` {#group__http_1ga9f79b8365fab747d1e1cee3cea0e2ac4}





#### `protected async::Runner::Ptr _runner` {#group__http_1ga200dc5bf9bf9516a2432fe9156f79be2}





#### `protected std::string _encoding` {#group__http_1gaffbac44d4b4e31504c159879ad4aeeea}





#### `protected std::string _boundary` {#group__http_1ga6bdb766483b5dc3ec566ba5dd8c62d0a}





#### `protected PartQueue _parts` {#group__http_1ga203d49a219f8dd1db20b68d5c8f7f762}





#### `protected std::uint64_t _filesLength` {#group__http_1ga4c0228d905f84b5886ff3b431b51e518}





#### `protected int _writeState` {#group__http_1ga6d4721907f8d77d40d79b97a522c2ac0}





#### `protected bool _initial` {#group__http_1gae9379de89bcbc2827acc2e98834b592f}





#### `protected bool _complete` {#group__http_1ga0937540b2e3ebfa1b33cdb955384cbb3}





#### `protected  FormWriter(`[`ClientConnection`](#classscy_1_1http_1_1ClientConnection)` & conn,async::Runner::Ptr runner,const std::string & encoding)` {#group__http_1ga61b4df3b3df0a8e5edee5724ec669ca8}



Creates the [FormWriter](#classscy_1_1http_1_1FormWriter) that uses the given encoding.

Encoding must be either "application/x-www-form-urlencoded" (which is the default) or "multipart/form-data".

#### `protected  FormWriter(const `[`FormWriter`](#classscy_1_1http_1_1FormWriter)` &)` {#group__http_1ga791e16afce05540110610e8edfb699f3}





#### `protected `[`FormWriter`](#classscy_1_1http_1_1FormWriter)` & operator=(const `[`FormWriter`](#classscy_1_1http_1_1FormWriter)` &)` {#group__http_1ga46a15de69fb7025e499e9700c0c54f8b}





#### `protected void writePartHeader(const `[`NVCollection`](#classscy_1_1NVCollection)` & header,std::ostream & ostr)` {#group__http_1ga284dfa7aaa9df8733b3478c50f7cc1aa}



Writes the message boundary std::string, followed by the message header to the output stream.

#### `protected void writeEnd(std::ostream & ostr)` {#group__http_1gae821aa326c6156c6ab81809b75bfdff9}

Writes the final boundary std::string to the output stream.



#### `protected virtual void updateProgress(int nread)` {#group__http_1ga8d47d8f0ecc8dfe4cab1ce61c90a091f}



Updates the upload progress via the associated [ClientConnection](#classscy_1_1http_1_1ClientConnection) object.

# class `scy::http::Message` {#classscy_1_1http_1_1Message}

```
class scy::http::Message
  : public scy::NVCollection
```  



The base class for [Request](#classscy_1_1http_1_1Request) and [Response](#classscy_1_1http_1_1Response).

Defines the common properties of all HTTP messages. These are version, content length, content type and transfer encoding.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public void setVersion(const std::string & version)` | Sets the HTTP version for this message.
`public const std::string & getVersion() const` | Returns the HTTP version for this message.
`public void setContentLength(std::uint64_t length)` | 
`public std::uint64_t getContentLength() const` | 
`public bool hasContentLength() const` | Returns true if a Content-Length header is present.
`public void setTransferEncoding(const std::string & transferEncoding)` | 
`public const std::string & getTransferEncoding() const` | 
`public void setChunkedTransferEncoding(bool flag)` | 
`public bool isChunkedTransferEncoding() const` | 
`public void setContentType(const std::string & contentType)` | 
`public const std::string & getContentType() const` | 
`public void setKeepAlive(bool keepAlive)` | 
`public bool getKeepAlive() const` | 
`public virtual void write(std::ostream & ostr) const` | 
`protected  Message()` | Creates the [Message](./doc/api-http.md#classscy_1_1http_1_1Message) with version HTTP/1.0.
`protected  Message(const std::string & version)` | 
`protected virtual  ~Message()` | Destroys the [Message](./doc/api-http.md#classscy_1_1http_1_1Message).

## Members

#### `public void setVersion(const std::string & version)` {#group__http_1ga723cfacd1de2e59acd576eb63010d6fc}

Sets the HTTP version for this message.



#### `public const std::string & getVersion() const` {#group__http_1gaeaf5238f777cf31201cb8bc342283221}

Returns the HTTP version for this message.



#### `public void setContentLength(std::uint64_t length)` {#group__http_1ga896f5e886a4657a765b4dad60b6619b9}



Sets the Content-Length header.

If length is UNKNOWN_CONTENT_LENGTH, removes the Content-Length header.

#### `public std::uint64_t getContentLength() const` {#group__http_1ga8dbbb7e2e4317e75a6bf7877fd95e2df}



Returns the content length for this message, which may be UNKNOWN_CONTENT_LENGTH if no Content-Length header is present.

#### `public bool hasContentLength() const` {#group__http_1gad90dd4a412c227b05b071b26c71aaf35}

Returns true if a Content-Length header is present.



#### `public void setTransferEncoding(const std::string & transferEncoding)` {#group__http_1ga44b4439cc2b005a82ad7d118a53e0e0b}



Sets the transfer encoding for this message.

The value should be either IDENTITY_TRANSFER_CODING or CHUNKED_TRANSFER_CODING.

#### `public const std::string & getTransferEncoding() const` {#group__http_1ga886838e78abe992b994489cd5809285f}



Returns the transfer encoding used for this message.

Normally, this is the value of the Transfer-Encoding header field. If no such field is present, returns IDENTITY_TRANSFER_CODING.

#### `public void setChunkedTransferEncoding(bool flag)` {#group__http_1ga1093aa082297c1b15f018f7cda41096e}



If flag is true, sets the Transfer-Encoding header to chunked. Otherwise, removes the Transfer-Encoding header.

#### `public bool isChunkedTransferEncoding() const` {#group__http_1gac8677cc8b307cf7172a3299116acad30}



Returns true if the Transfer-Encoding header is set and its value is chunked.

#### `public void setContentType(const std::string & contentType)` {#group__http_1ga0bc22314e98367be2ef4a72ddc626b94}



Sets the content type for this message.

Specify NO_CONTENT_TYPE to remove the Content-Type header.

#### `public const std::string & getContentType() const` {#group__http_1gaab4a46e024093c712047548b6297f5f1}



Returns the content type for this message.

If no Content-Type header is present, returns UNKNOWN_CONTENT_TYPE.

#### `public void setKeepAlive(bool keepAlive)` {#group__http_1ga093099a4721005e4705804f0f9aa096e}



Sets the value of the [Connection](#classscy_1_1http_1_1Connection) header field.

The value is set to "Keep-Alive" if keepAlive is true, or to "Close" otherwise.

#### `public bool getKeepAlive() const` {#group__http_1ga12e6a3de621471dbf9fa8576902f972c}



Returns true if

* the message has a [Connection](#classscy_1_1http_1_1Connection) header field and its value is "Keep-Alive"


* the message is a HTTP/1.1 message and not [Connection](#classscy_1_1http_1_1Connection) header is set Returns false otherwise.

#### `public virtual void write(std::ostream & ostr) const` {#group__http_1ga9ebc2305bb2173419345a1d2e21efd19}



Writes the message header to the given output stream.

The format is one name-value pair per line, with name and value separated by a colon and lines delimited by a carriage return and a linefeed character. See RFC 2822 for details.

#### `protected  Message()` {#group__http_1ga928fa708b89d544c3b6f5039b787bb7e}

Creates the [Message](#classscy_1_1http_1_1Message) with version HTTP/1.0.



#### `protected  Message(const std::string & version)` {#group__http_1gabcb10a4c47593f94c9431c0f21cbf114}



Creates the [Message](#classscy_1_1http_1_1Message) and sets the version.

#### `protected virtual  ~Message()` {#group__http_1ga71ef669503290a7f386ee2dc91e09b53}

Destroys the [Message](#classscy_1_1http_1_1Message).



# class `scy::http::MultipartAdapter` {#classscy_1_1http_1_1MultipartAdapter}

```
class scy::http::MultipartAdapter
  : public scy::PacketProcessor
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`Connection`](./doc/api-http.md#classscy_1_1http_1_1Connection)` * connection` | 
`public std::string contentType` | 
`public bool isBase64` | 
`public bool initial` | 
`public `[`PacketSignal`](#group__base_1ga3ffb5bda6133fac97f3bed2b567b9b6a)` emitter` | 
`public inline  MultipartAdapter(`[`Connection`](#classscy_1_1http_1_1Connection)` * connection,bool base64)` | 
`public inline  MultipartAdapter(const std::string & contentType,bool base64)` | 
`public inline virtual  ~MultipartAdapter()` | 
`public inline virtual void emitHeader()` | 
`public inline virtual void emitChunkHeader()` | Sets HTTP header for the current chunk.
`public inline virtual void process(`[`IPacket`](#classscy_1_1IPacket)` & packet)` | 

## Members

#### `public `[`Connection`](./doc/api-http.md#classscy_1_1http_1_1Connection)` * connection` {#group__http_1ga48eac4fef04680408a5952fd69fdd91c}





#### `public std::string contentType` {#group__http_1ga28482bfd82b764ab36ebba40465d6bd8}





#### `public bool isBase64` {#group__http_1ga9d95a8cdd1e6a1d164aeaba94b986c3f}





#### `public bool initial` {#group__http_1ga7c8957e88432dbaad3f86ae512c3e23e}





#### `public `[`PacketSignal`](#group__base_1ga3ffb5bda6133fac97f3bed2b567b9b6a)` emitter` {#group__http_1ga73a88cc955c203e10227dc1e72319fc6}





#### `public inline  MultipartAdapter(`[`Connection`](#classscy_1_1http_1_1Connection)` * connection,bool base64)` {#group__http_1ga11ebba058c8a5e8ad5a2a54db802555e}





#### `public inline  MultipartAdapter(const std::string & contentType,bool base64)` {#group__http_1ga3a4c616e3dbe183d1ba9f08043a47987}





#### `public inline virtual  ~MultipartAdapter()` {#group__http_1gaa60e5cd062c7768f7636a851f5154076}





#### `public inline virtual void emitHeader()` {#group__http_1ga5c48095fcace4608150c12d3e918a4b7}





#### `public inline virtual void emitChunkHeader()` {#group__http_1ga98b86bd888dc6d41962ab88c46d3729d}

Sets HTTP header for the current chunk.



#### `public inline virtual void process(`[`IPacket`](#classscy_1_1IPacket)` & packet)` {#group__http_1gaafa137c5fc3cfa81a0ed7bb3de2ba3c4}



This method performs processing on the given packet and emits the result.

Note: If packet processing is async (the packet is not in the current thread scope) then packet data must be copied. Copied data can be freed directly aFter the async call to emit() the outgoing packet.

# class `scy::http::Parser` {#classscy_1_1http_1_1Parser}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`ParserObserver`](./doc/api-http.md#classscy_1_1http_1_1ParserObserver)` * _observer` | 
`public `[`http::Request`](./doc/api-http.md#classscy_1_1http_1_1Request)` * _request` | 
`public `[`http::Response`](./doc/api-http.md#classscy_1_1http_1_1Response)` * _response` | 
`public `[`http::Message`](./doc/api-http.md#classscy_1_1http_1_1Message)` * _message` | 
`public http_parser _parser` | 
`public http_parser_settings _settings` | 
`public bool _wasHeaderValue` | 
`public std::string _lastHeaderField` | 
`public std::string _lastHeaderValue` | 
`public bool _complete` | 
`public `[`ParserError`](./doc/api-http.md#structscy_1_1http_1_1ParserError)` * _error` | 
`public  Parser(`[`http::Response`](#classscy_1_1http_1_1Response)` * response)` | 
`public  Parser(`[`http::Request`](#classscy_1_1http_1_1Request)` * request)` | 
`public  Parser(http_parser_type type)` | 
`public  ~Parser()` | 
`public void init(http_parser_type type)` | 
`public std::size_t parse(const char * data,std::size_t length)` | 
`public void reset()` | 
`public bool complete() const` | 
`public void setParserError(const std::string & message)` | 
`public void setRequest(`[`http::Request`](#classscy_1_1http_1_1Request)` * request)` | 
`public void setResponse(`[`http::Response`](#classscy_1_1http_1_1Response)` * response)` | 
`public void setObserver(`[`ParserObserver`](#classscy_1_1http_1_1ParserObserver)` * observer)` | 
`public `[`http::Message`](#classscy_1_1http_1_1Message)` * message()` | 
`public `[`ParserObserver`](#classscy_1_1http_1_1ParserObserver)` * observer() const` | 
`public bool upgrade() const` | 
`public bool shouldKeepAlive() const` | 
`public void onURL(const std::string & value)` | 
`public void onHeader(const std::string & name,const std::string & value)` | 
`public void onHeadersEnd()` | 
`public void onBody(const char * buf,std::size_t len)` | 
`public void onMessageEnd()` | 
`public void onError(const `[`ParserError`](#structscy_1_1http_1_1ParserError)` & err)` | 

## Members

#### `public `[`ParserObserver`](./doc/api-http.md#classscy_1_1http_1_1ParserObserver)` * _observer` {#group__http_1gab42977a9639b8058d82de95625466afc}





#### `public `[`http::Request`](./doc/api-http.md#classscy_1_1http_1_1Request)` * _request` {#group__http_1ga4981cb205d6bb8d818da9f83335894ec}





#### `public `[`http::Response`](./doc/api-http.md#classscy_1_1http_1_1Response)` * _response` {#group__http_1ga65ae85b18c21ebe24f566e625e1fa649}





#### `public `[`http::Message`](./doc/api-http.md#classscy_1_1http_1_1Message)` * _message` {#group__http_1gad783cae492f8af491ac50f84ed46a0ba}





#### `public http_parser _parser` {#group__http_1ga284061d4f96863a6ddb4811374426ac2}





#### `public http_parser_settings _settings` {#group__http_1gac371969ab5620341f41eea646d868e2d}





#### `public bool _wasHeaderValue` {#group__http_1ga1355086b7cec9416fe9bc0769d252053}





#### `public std::string _lastHeaderField` {#group__http_1gaeeac236dee38ff52b5582052dcd32564}





#### `public std::string _lastHeaderValue` {#group__http_1gad5a5ae35c21488224635872e273967bb}





#### `public bool _complete` {#group__http_1ga3f868ba7ed8140a65dcc88d30d149440}





#### `public `[`ParserError`](./doc/api-http.md#structscy_1_1http_1_1ParserError)` * _error` {#group__http_1gad8036bbc353e3caa83656131716a8416}





#### `public  Parser(`[`http::Response`](#classscy_1_1http_1_1Response)` * response)` {#group__http_1gaa8e33ac970406b210115f11bf311b264}





#### `public  Parser(`[`http::Request`](#classscy_1_1http_1_1Request)` * request)` {#group__http_1ga3ec4c7513082d1c024cbbb9bb3643ed8}





#### `public  Parser(http_parser_type type)` {#group__http_1gaaaecb96651b9bde4227f97e430b8d879}





#### `public  ~Parser()` {#group__http_1ga5a5601800541bf19f61a711a95c3eb3d}





#### `public void init(http_parser_type type)` {#group__http_1gaa380c3223b4ccbf364e871125d9961c6}





#### `public std::size_t parse(const char * data,std::size_t length)` {#group__http_1ga4d753b4f04a42fc7255fbd5407cda13d}





#### `public void reset()` {#group__http_1ga78f9760a7041e1abf6c18b7bfe45f372}



Feed data read from socket into the http_parser.

Returns true of the message is complete, false if incomplete. Reset the parser state for a new message

#### `public bool complete() const` {#group__http_1ga5d34867367516a57d5c7b66419ef5f2d}



Returns true if parsing is complete, either in success or error.

#### `public void setParserError(const std::string & message)` {#group__http_1gaeced99c347e8b8347b82206d8c119f04}





#### `public void setRequest(`[`http::Request`](#classscy_1_1http_1_1Request)` * request)` {#group__http_1ga88ddd77c698950e4f1dc67808e680da8}





#### `public void setResponse(`[`http::Response`](#classscy_1_1http_1_1Response)` * response)` {#group__http_1gad6aed0467f8fcdbd9b9a2caa885401ac}





#### `public void setObserver(`[`ParserObserver`](#classscy_1_1http_1_1ParserObserver)` * observer)` {#group__http_1gae102cf86a6376a3cdaab320bf5eaefae}





#### `public `[`http::Message`](#classscy_1_1http_1_1Message)` * message()` {#group__http_1ga4fee2c58d459db5eb9d51d19ba903543}





#### `public `[`ParserObserver`](#classscy_1_1http_1_1ParserObserver)` * observer() const` {#group__http_1gab3c0294c7cf94c797f36232579e73fac}





#### `public bool upgrade() const` {#group__http_1ga1c61be572464f18b0859c5642617b4ff}





#### `public bool shouldKeepAlive() const` {#group__http_1gac47e23cf6fe3234ea85646f6c258994a}





#### `public void onURL(const std::string & value)` {#group__http_1gaa5cb6c93db76ffce3859f110ef859cba}





#### `public void onHeader(const std::string & name,const std::string & value)` {#group__http_1ga39c3e8ebe19b3fc465ab97450c357a86}





#### `public void onHeadersEnd()` {#group__http_1ga2f638fec24c8fd048b9c18c9455fe5d5}





#### `public void onBody(const char * buf,std::size_t len)` {#group__http_1ga6ea5c0d3fd5cae8e7cf0c988eab06737}





#### `public void onMessageEnd()` {#group__http_1gaabdf6e5c001616ca452b8ca11731314a}





#### `public void onError(const `[`ParserError`](#structscy_1_1http_1_1ParserError)` & err)` {#group__http_1gaeae9b93e88e113e34680cc0b42bbeeb9}





# class `scy::http::ParserObserver` {#classscy_1_1http_1_1ParserObserver}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public void onParserHeader(const std::string & name,const std::string & value)` | 
`public void onParserHeadersEnd()` | 
`public void onParserChunk(const char * data,std::size_t len)` | 
`public void onParserEnd()` | 
`public void onParserError(const `[`ParserError`](#structscy_1_1http_1_1ParserError)` & err)` | 

## Members

#### `public void onParserHeader(const std::string & name,const std::string & value)` {#group__http_1ga510985b7152fedee52ebb93488faf6c7}





#### `public void onParserHeadersEnd()` {#group__http_1ga3d918d5b06428e5ec16df51092b63700}





#### `public void onParserChunk(const char * data,std::size_t len)` {#group__http_1ga1681b1149f3972c22ee7de171ddd08d0}





#### `public void onParserEnd()` {#group__http_1gae2b96db1a35fd47c09a4d1b9c2eca40c}





#### `public void onParserError(const `[`ParserError`](#structscy_1_1http_1_1ParserError)` & err)` {#group__http_1ga190424099d659692bb12d156f6a86627}





# class `scy::http::ProgressSignal` {#classscy_1_1http_1_1ProgressSignal}

```
class scy::http::ProgressSignal
  : public scy::Signal< void(const double &)>
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public void * sender` | 
`public std::uint64_t current` | 
`public std::uint64_t total` | 
`public inline  ProgressSignal()` | 
`public inline double progress() const` | 
`public inline void update(int nread)` | 

## Members

#### `public void * sender` {#group__http_1gaac7fe5b5998e92c2ebfc4be61490f7ba}





#### `public std::uint64_t current` {#group__http_1ga4d50dd8d1b7710ca51e34140f2db5b15}





#### `public std::uint64_t total` {#group__http_1ga71f4563f91ea8df9a27b65d67f3fe2ac}





#### `public inline  ProgressSignal()` {#group__http_1ga096bf69d4c5dfdb71eb5e16c84b14d91}





#### `public inline double progress() const` {#group__http_1ga312d627b4aa2afa7922d352c21803c04}





#### `public inline void update(int nread)` {#group__http_1gac3373e381f6b191a3e88d7b449ab7b94}





# class `scy::http::Request` {#classscy_1_1http_1_1Request}

```
class scy::http::Request
  : public scy::http::Message
```  



This class encapsulates an HTTP request message.

In addition to the properties common to all HTTP messages, a HTTP request has a method (e.g. GET, HEAD, POST, etc.) and a request URI.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Request()` | Creates a GET / HTTP/1.1 HTTP request.
`public  Request(const std::string & version)` | 
`public  Request(const std::string & method,const std::string & uri)` | Creates a HTTP/1.0 request with the given method and URI.
`public  Request(const std::string & method,const std::string & uri,const std::string & version)` | Creates a HTTP request with the given method, URI and version.
`public virtual  ~Request()` | Destroys the [Request](./doc/api-http.md#classscy_1_1http_1_1Request).
`public void setMethod(const std::string & method)` | Sets the method.
`public const std::string & getMethod() const` | Returns the method.
`public void setURI(const std::string & uri)` | Sets the request URI.
`public const std::string & getURI() const` | Returns the request URI.
`public void setHost(const std::string & host)` | Sets the value of the Host header field.
`public void setHost(const std::string & host,std::uint16_t port)` | 
`public const std::string & getHost() const` | 
`public void setCookies(const `[`NVCollection`](#classscy_1_1NVCollection)` & cookies)` | 
`public void getCookies(`[`NVCollection`](#classscy_1_1NVCollection)` & cookies) const` | 
`public void getURIParameters(`[`NVCollection`](#classscy_1_1NVCollection)` & params) const` | Returns the request URI parameters.
`public bool hasCredentials() const` | 
`public void getCredentials(std::string & scheme,std::string & authInfo) const` | 
`public void setCredentials(const std::string & scheme,const std::string & authInfo)` | 
`public bool hasProxyCredentials() const` | 
`public void getProxyCredentials(std::string & scheme,std::string & authInfo) const` | 
`public void setProxyCredentials(const std::string & scheme,const std::string & authInfo)` | Sets the proxy authentication scheme and information for this request.
`public virtual void write(std::ostream & ostr) const` | Writes the HTTP request to the given output stream.
`protected void getCredentials(const std::string & header,std::string & scheme,std::string & authInfo) const` | 
`protected void setCredentials(const std::string & header,const std::string & scheme,const std::string & authInfo)` | 

## Members

#### `public  Request()` {#group__http_1gad2ce3628c6611035ce0aef0d1c426d11}

Creates a GET / HTTP/1.1 HTTP request.



#### `public  Request(const std::string & version)` {#group__http_1ga80211c782607bc90f2f131cf42268b08}



Creates a GET / HTTP/1.x request with the given version (HTTP/1.0 or HTTP/1.1).

#### `public  Request(const std::string & method,const std::string & uri)` {#group__http_1gacc3e9d5ec5cf6dd2dae22ab388b46f7b}

Creates a HTTP/1.0 request with the given method and URI.



#### `public  Request(const std::string & method,const std::string & uri,const std::string & version)` {#group__http_1ga3cbd5cedd395cb94008fe3f7b76d6a6b}

Creates a HTTP request with the given method, URI and version.



#### `public virtual  ~Request()` {#group__http_1ga5a0f84be907807cec979ba0f16ea05d0}

Destroys the [Request](#classscy_1_1http_1_1Request).



#### `public void setMethod(const std::string & method)` {#group__http_1ga903bfa0b58cd56ee270b0740696e429a}

Sets the method.



#### `public const std::string & getMethod() const` {#group__http_1ga7bbc530cd6fd5963d605678cb39f690c}

Returns the method.



#### `public void setURI(const std::string & uri)` {#group__http_1ga56b72d7b2214832b2e6fa363a2488a03}

Sets the request URI.



#### `public const std::string & getURI() const` {#group__http_1gaeae46584e5ffd76b18e5d8c26e966ddf}

Returns the request URI.



#### `public void setHost(const std::string & host)` {#group__http_1gaa589b8c926d37f43c4bb0e82ecdca0f3}

Sets the value of the Host header field.



#### `public void setHost(const std::string & host,std::uint16_t port)` {#group__http_1ga885a17965b531cd110f5e73ac89a701f}



Sets the value of the Host header field.

If the given port number is a non-standard port number (other than 80 or 443), it is included in the Host header field.

#### `public const std::string & getHost() const` {#group__http_1ga8657ba5d0627331bca43ccbc51cab24f}



Returns the value of the Host header field.

Throws a NotFoundException if the request does not have a Host header field.

#### `public void setCookies(const `[`NVCollection`](#classscy_1_1NVCollection)` & cookies)` {#group__http_1gaeb9f90191eef1b9858e95ecc5b64d7f7}



Adds a [Cookie](#classscy_1_1http_1_1Cookie) header with the names and values from cookies.

#### `public void getCookies(`[`NVCollection`](#classscy_1_1NVCollection)` & cookies) const` {#group__http_1gae2282f1d55a4a0e7e22cccc4ec0608c1}



Fills cookies with the cookies extracted from the [Cookie](#classscy_1_1http_1_1Cookie) headers in the request.

#### `public void getURIParameters(`[`NVCollection`](#classscy_1_1NVCollection)` & params) const` {#group__http_1ga58c2d881e36b934b69cd9e97d2d30fc9}

Returns the request URI parameters.



#### `public bool hasCredentials() const` {#group__http_1ga827d504a689a7a136670496996b79911}



Returns true if the request contains authentication information in the form of an Authorization header.

#### `public void getCredentials(std::string & scheme,std::string & authInfo) const` {#group__http_1ga3e3e20aa8b039c0376be64bad867b836}



Returns the authentication scheme and additional authentication information contained in this request.

Throws a std::exception if no authentication information is contained in the request.

#### `public void setCredentials(const std::string & scheme,const std::string & authInfo)` {#group__http_1ga3d250b6ef7029e86627fe4a5e3519f0c}



Sets the authentication scheme and information for this request.

#### `public bool hasProxyCredentials() const` {#group__http_1ga8dfbc037a7cf934202abe6c4043c633e}



Returns true if the request contains proxy authentication information in the form of an Proxy-Authorization header.

#### `public void getProxyCredentials(std::string & scheme,std::string & authInfo) const` {#group__http_1ga296b8e6cbb8e437bb615b8696c3e79b0}



Returns the proxy authentication scheme and additional proxy authentication information contained in this request.

Throws a std::exception if no proxy authentication information is contained in the request.

#### `public void setProxyCredentials(const std::string & scheme,const std::string & authInfo)` {#group__http_1ga68cc14b25555c4e31aa5e738acc1af03}

Sets the proxy authentication scheme and information for this request.



#### `public virtual void write(std::ostream & ostr) const` {#group__http_1gada337261a2a71406c679ef09d1512508}

Writes the HTTP request to the given output stream.



#### `protected void getCredentials(const std::string & header,std::string & scheme,std::string & authInfo) const` {#group__http_1gaba6e2033b8d589d71c264b2469a97c20}



Returns the authentication scheme and additional authentication information contained in the given header of request.

Throws a NotAuthenticatedException if no authentication information is contained in the request.

#### `protected void setCredentials(const std::string & header,const std::string & scheme,const std::string & authInfo)` {#group__http_1gaa5f997f647ac3e1d6dff0a9ef5e450bd}



Writes the authentication scheme and information for this request to the given header.

# class `scy::http::Response` {#classscy_1_1http_1_1Response}

```
class scy::http::Response
  : public scy::http::Message
```  

This class encapsulates an HTTP response message.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Response()` | typedef std::shared_ptr<Response> Ptr;
`public  Response(`[`StatusCode`](#group__http_1ga557c5bc8f0990c42f31cbaca9b756f2b)` status,const std::string & reason)` | Creates the [Response](./doc/api-http.md#classscy_1_1http_1_1Response) with the given status and reason phrase.
`public  Response(const std::string & version,`[`StatusCode`](#group__http_1ga557c5bc8f0990c42f31cbaca9b756f2b)` status,const std::string & reason)` | Creates the [Response](./doc/api-http.md#classscy_1_1http_1_1Response) with the given version, status and reason phrase.
`public  Response(`[`StatusCode`](#group__http_1ga557c5bc8f0990c42f31cbaca9b756f2b)` status)` | 
`public  Response(const std::string & version,`[`StatusCode`](#group__http_1ga557c5bc8f0990c42f31cbaca9b756f2b)` status)` | 
`public virtual  ~Response()` | Destroys the [Response](./doc/api-http.md#classscy_1_1http_1_1Response).
`public void setStatus(`[`StatusCode`](#group__http_1ga557c5bc8f0990c42f31cbaca9b756f2b)` status)` | 
`public `[`StatusCode`](#group__http_1ga557c5bc8f0990c42f31cbaca9b756f2b)` getStatus() const` | Returns the HTTP status code.
`public void setReason(const std::string & reason)` | Sets the HTTP reason phrase.
`public const std::string & getReason() const` | Returns the HTTP reason phrase.
`public void setStatusAndReason(`[`StatusCode`](#group__http_1ga557c5bc8f0990c42f31cbaca9b756f2b)` status,const std::string & reason)` | Sets the HTTP status code and reason phrase.
`public void setDate(const `[`Timestamp`](#classscy_1_1Timestamp)` & dateTime)` | Sets the Date header to the given date/time value.
`public `[`Timestamp`](#classscy_1_1Timestamp)` getDate() const` | Returns the value of the Date header.
`public void addCookie(const `[`Cookie`](#classscy_1_1http_1_1Cookie)` & cookie)` | 
`public void getCookies(std::vector< `[`Cookie`](#classscy_1_1http_1_1Cookie)` > & cookies) const` | 
`public virtual void write(std::ostream & ostr) const` | Writes the HTTP response headers to the given output stream.
`public virtual bool success() const` | Returns true if the HTTP response code was successful (>= 400).

## Members

#### `public  Response()` {#group__http_1ga6dcc7957a768d368de855de1832b0c7f}

typedef std::shared_ptr<Response> Ptr;

Creates the [Response](#classscy_1_1http_1_1Response) with OK status.

#### `public  Response(`[`StatusCode`](#group__http_1ga557c5bc8f0990c42f31cbaca9b756f2b)` status,const std::string & reason)` {#group__http_1gaa6cfa32e947c8f7b935299ec14908fc4}

Creates the [Response](#classscy_1_1http_1_1Response) with the given status and reason phrase.



#### `public  Response(const std::string & version,`[`StatusCode`](#group__http_1ga557c5bc8f0990c42f31cbaca9b756f2b)` status,const std::string & reason)` {#group__http_1gafad75c415532c75c39557562a0d6b1e9}

Creates the [Response](#classscy_1_1http_1_1Response) with the given version, status and reason phrase.



#### `public  Response(`[`StatusCode`](#group__http_1ga557c5bc8f0990c42f31cbaca9b756f2b)` status)` {#group__http_1ga68ef0a2401a81b38a71b8a8e9568f2ef}



Creates the [Response](#classscy_1_1http_1_1Response) with the given status an an appropriate reason phrase.

#### `public  Response(const std::string & version,`[`StatusCode`](#group__http_1ga557c5bc8f0990c42f31cbaca9b756f2b)` status)` {#group__http_1gaef92509c09ac639c5cbf4a7ca306da4e}



Creates the [Response](#classscy_1_1http_1_1Response) with the given version, status an an appropriate reason phrase.

#### `public virtual  ~Response()` {#group__http_1ga4ca65ee2c06c00f1c37d6efc2a6b8a94}

Destroys the [Response](#classscy_1_1http_1_1Response).



#### `public void setStatus(`[`StatusCode`](#group__http_1ga557c5bc8f0990c42f31cbaca9b756f2b)` status)` {#group__http_1ga81446407dccadd7d04bfe7146b40a626}



Sets the HTTP status code.

The reason phrase is set according to the status code.

#### `public `[`StatusCode`](#group__http_1ga557c5bc8f0990c42f31cbaca9b756f2b)` getStatus() const` {#group__http_1ga31cb1724821d5ae5637e6528f7b36744}

Returns the HTTP status code.



#### `public void setReason(const std::string & reason)` {#group__http_1ga0afdb0e96ca64dd1ac2371951979419c}

Sets the HTTP reason phrase.



#### `public const std::string & getReason() const` {#group__http_1ga16648e0457003c4f294e726bbb96bc63}

Returns the HTTP reason phrase.



#### `public void setStatusAndReason(`[`StatusCode`](#group__http_1ga557c5bc8f0990c42f31cbaca9b756f2b)` status,const std::string & reason)` {#group__http_1ga31f9b365c9554051f015a48dd5767c74}

Sets the HTTP status code and reason phrase.



#### `public void setDate(const `[`Timestamp`](#classscy_1_1Timestamp)` & dateTime)` {#group__http_1ga1b94d074683a9943553de3a65418248a}

Sets the Date header to the given date/time value.



#### `public `[`Timestamp`](#classscy_1_1Timestamp)` getDate() const` {#group__http_1ga9fe6d805ac9fc0ad932f38c47493e9a1}

Returns the value of the Date header.



#### `public void addCookie(const `[`Cookie`](#classscy_1_1http_1_1Cookie)` & cookie)` {#group__http_1ga3cb3ac30eaf1374441d2d34619104cae}



Adds the cookie to the response by adding a Set-Cookie header.

#### `public void getCookies(std::vector< `[`Cookie`](#classscy_1_1http_1_1Cookie)` > & cookies) const` {#group__http_1gad31dde264fc5343a15e4e441707e82da}



Returns a vector with all the cookies set in the response header.

May throw an exception in case of a malformed Set-Cookie header.

#### `public virtual void write(std::ostream & ostr) const` {#group__http_1ga64096ef947b515ad0dd23a4b8cd2d929}

Writes the HTTP response headers to the given output stream.



#### `public virtual bool success() const` {#group__http_1ga903707bc08142cd176229837609931ef}

Returns true if the HTTP response code was successful (>= 400).



# class `scy::http::Server` {#classscy_1_1http_1_1Server}




DISCLAIMER: This HTTP server is not standards compliant. It was created to be a fast (nocopy where possible) solution for streaming video to web browsers.

TODO:

* SSL [Server](#classscy_1_1http_1_1Server)


* Enable responders (controllers?) to be instantiated via registered routes.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public net::TCPSocket::Ptr socket` | 
`public `[`ServerResponderFactory`](./doc/api-http.md#classscy_1_1http_1_1ServerResponderFactory)` * factory` | 
`public ServerConnectionList connections` | 
`public `[`net::Address`](./doc/api-net.md#classscy_1_1net_1_1Address)` address` | 
`public `[`NullSignal`](./doc/api-base.md#classscy_1_1Signal)` Shutdown` | 
`public  Server(short port,`[`ServerResponderFactory`](#classscy_1_1http_1_1ServerResponderFactory)` * factory)` | 
`public virtual  ~Server()` | 
`public void start()` | 
`public void shutdown()` | 
`public std::uint16_t port()` | 
`protected ServerConnection::Ptr createConnection(const net::Socket::Ptr & sock)` | 
`protected `[`ServerResponder`](#classscy_1_1http_1_1ServerResponder)` * createResponder(`[`ServerConnection`](#classscy_1_1http_1_1ServerConnection)` & conn)` | 
`protected virtual void addConnection(ServerConnection::Ptr conn)` | 
`protected virtual void removeConnection(`[`ServerConnection`](#classscy_1_1http_1_1ServerConnection)` * conn)` | 
`protected void onSocketAccept(const net::TCPSocket::Ptr & socket)` | 
`protected void onSocketClose(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket)` | 
`protected void onConnectionClose(`[`Connection`](#classscy_1_1http_1_1Connection)` & socket)` | 

## Members

#### `public net::TCPSocket::Ptr socket` {#group__http_1ga70808298d34765c717fab4fdd6c59e71}





#### `public `[`ServerResponderFactory`](./doc/api-http.md#classscy_1_1http_1_1ServerResponderFactory)` * factory` {#group__http_1ga144741437940605b8d5dd3f70644e09c}





#### `public ServerConnectionList connections` {#group__http_1ga0bf963cb60281e44b891b54f78d3bc58}





#### `public `[`net::Address`](./doc/api-net.md#classscy_1_1net_1_1Address)` address` {#group__http_1gaddbdac8f55bfe0c23ce967fd1574e259}





#### `public `[`NullSignal`](./doc/api-base.md#classscy_1_1Signal)` Shutdown` {#group__http_1ga7cf8f08eb3f78b6f9b2f220ee579ec43}





#### `public  Server(short port,`[`ServerResponderFactory`](#classscy_1_1http_1_1ServerResponderFactory)` * factory)` {#group__http_1ga1dc23ff23f8625961a38e5de0a0891de}





#### `public virtual  ~Server()` {#group__http_1ga6390681787294a3d75419f0aa3407ef9}





#### `public void start()` {#group__http_1gacd3378a005d8cdf83199810b3adbea12}





#### `public void shutdown()` {#group__http_1gafea310fcd0a89235dce2ad8182832d8a}





#### `public std::uint16_t port()` {#group__http_1ga87aa0c45573c742790771e186db160d8}





#### `protected ServerConnection::Ptr createConnection(const net::Socket::Ptr & sock)` {#group__http_1ga88332f85602b6ce8ab55a3ebb261b8f8}





#### `protected `[`ServerResponder`](#classscy_1_1http_1_1ServerResponder)` * createResponder(`[`ServerConnection`](#classscy_1_1http_1_1ServerConnection)` & conn)` {#group__http_1gae78a1aaaa86149a459a05f3abdb514a1}





#### `protected virtual void addConnection(ServerConnection::Ptr conn)` {#group__http_1gaaeb1f152226d3839d888ef7989beb804}





#### `protected virtual void removeConnection(`[`ServerConnection`](#classscy_1_1http_1_1ServerConnection)` * conn)` {#group__http_1gab5c36f488887719faf3696753c06c6bf}





#### `protected void onSocketAccept(const net::TCPSocket::Ptr & socket)` {#group__http_1ga53de55e2e742d9e5206e22ac3c6f7d89}





#### `protected void onSocketClose(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket)` {#group__http_1gaf63edf7f8001f9ebbbb10f089166f983}





#### `protected void onConnectionClose(`[`Connection`](#classscy_1_1http_1_1Connection)` & socket)` {#group__http_1gaed2fb3b9898de149eab8643426af3258}





# class `scy::http::ServerAdapter` {#classscy_1_1http_1_1ServerAdapter}

```
class scy::http::ServerAdapter
  : public scy::http::ConnectionAdapter
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  ServerAdapter(`[`ServerConnection`](#classscy_1_1http_1_1ServerConnection)` & connection)` | 

## Members

#### `public inline  ServerAdapter(`[`ServerConnection`](#classscy_1_1http_1_1ServerConnection)` & connection)` {#group__http_1ga1dd2090bc9ee3bb6f44528d30cd47535}





# class `scy::http::ServerConnection` {#classscy_1_1http_1_1ServerConnection}

```
class scy::http::ServerConnection
  : public scy::http::Connection
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  ServerConnection(`[`Server`](#classscy_1_1http_1_1Server)` & server,net::Socket::Ptr socket)` | 
`public virtual  ~ServerConnection()` | 
`public virtual void close()` | Sends the HTTP response.
`protected `[`Server`](./doc/api-http.md#classscy_1_1http_1_1Server)` & _server` | 
`protected `[`ServerResponder`](./doc/api-http.md#classscy_1_1http_1_1ServerResponder)` * _responder` | 
`protected bool _upgrade` | 
`protected bool _requestComplete` | 
`protected virtual void onHeaders()` | 
`protected virtual void onPayload(const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer)` | 
`protected virtual void onMessage()` | 
`protected virtual void onClose()` | 
`protected `[`Server`](#classscy_1_1http_1_1Server)` & server()` | 
`protected virtual `[`http::Message`](#classscy_1_1http_1_1Message)` * incomingHeader()` | 
`protected virtual `[`http::Message`](#classscy_1_1http_1_1Message)` * outgoingHeader()` | 

## Members

#### `public  ServerConnection(`[`Server`](#classscy_1_1http_1_1Server)` & server,net::Socket::Ptr socket)` {#group__http_1ga95968f58192083a8a72a91d53c3ef1f5}





#### `public virtual  ~ServerConnection()` {#group__http_1gaf91054fe5b7bc11f492a1680e7b76ea8}





#### `public virtual void close()` {#group__http_1ga6b446f21d99cca93a303e09553df09ec}

Sends the HTTP response.

Closes the HTTP connection

#### `protected `[`Server`](./doc/api-http.md#classscy_1_1http_1_1Server)` & _server` {#group__http_1gaad54c14cb297b6ff031f35e08fc5d054}





#### `protected `[`ServerResponder`](./doc/api-http.md#classscy_1_1http_1_1ServerResponder)` * _responder` {#group__http_1gad1b0f70d5b87251947942913d9aa4deb}





#### `protected bool _upgrade` {#group__http_1gae2be8a0f2cb44470ded70990fe02ca96}





#### `protected bool _requestComplete` {#group__http_1ga9e748f2a6b7a1bbb191f17dfbcb82f75}





#### `protected virtual void onHeaders()` {#group__http_1ga8a50a23a9bf7b969e3c40d6507871bae}



Return true if the server did not give us a proper response within the allotted time. bool expired() const;

#### `protected virtual void onPayload(const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer)` {#group__http_1gaa9547a5ed6e9dcc0d585a125a4d9a0c7}





#### `protected virtual void onMessage()` {#group__http_1gaf913bbe90e17f1ca696c0d3e186f6517}





#### `protected virtual void onClose()` {#group__http_1ga3155050806ae33b9c370e0b28c3e3a1f}





#### `protected `[`Server`](#classscy_1_1http_1_1Server)` & server()` {#group__http_1ga232cf7d7739008405d2505457b728d58}





#### `protected virtual `[`http::Message`](#classscy_1_1http_1_1Message)` * incomingHeader()` {#group__http_1ga489493745b61f7ecba896933eec840b5}





#### `protected virtual `[`http::Message`](#classscy_1_1http_1_1Message)` * outgoingHeader()` {#group__http_1gabcb92bba2cb7a96e61cd8c38d62795f3}





# class `scy::http::ServerResponder` {#classscy_1_1http_1_1ServerResponder}




The abstract base class for HTTP ServerResponders created by HTTP [Server](#classscy_1_1http_1_1Server).

Derived classes must override the handleRequest() method.

A new HTTPServerResponder object will be created for each new HTTP request that is received by the HTTP [Server](#classscy_1_1http_1_1Server).

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  ServerResponder(`[`ServerConnection`](#classscy_1_1http_1_1ServerConnection)` & connection)` | 
`public inline virtual  ~ServerResponder()` | 
`public inline virtual void onHeaders(`[`Request`](#classscy_1_1http_1_1Request)` &)` | 
`public inline virtual void onPayload(const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` &)` | 
`public inline virtual void onRequest(`[`Request`](#classscy_1_1http_1_1Request)` &,`[`Response`](#classscy_1_1http_1_1Response)` &)` | 
`public inline virtual void onClose()` | 
`public inline `[`ServerConnection`](#classscy_1_1http_1_1ServerConnection)` & connection()` | 
`public inline `[`Request`](#classscy_1_1http_1_1Request)` & request()` | 
`public inline `[`Response`](#classscy_1_1http_1_1Response)` & response()` | 
`protected `[`ServerConnection`](./doc/api-http.md#classscy_1_1http_1_1ServerConnection)` & _connection` | 

## Members

#### `public inline  ServerResponder(`[`ServerConnection`](#classscy_1_1http_1_1ServerConnection)` & connection)` {#group__http_1ga4719a885f46da4d09406cab15a27b193}





#### `public inline virtual  ~ServerResponder()` {#group__http_1ga0e2b52bb16fd085eb0e1295c66db1a31}





#### `public inline virtual void onHeaders(`[`Request`](#classscy_1_1http_1_1Request)` &)` {#group__http_1ga30fbead9dbcc0e21ca43d4a520b8f1d1}





#### `public inline virtual void onPayload(const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` &)` {#group__http_1ga2eba11973f346b45b68fd65986077bd2}





#### `public inline virtual void onRequest(`[`Request`](#classscy_1_1http_1_1Request)` &,`[`Response`](#classscy_1_1http_1_1Response)` &)` {#group__http_1gab39d5ba1fd5ef06743e972bf125acc4d}





#### `public inline virtual void onClose()` {#group__http_1ga2eeff60bc5f765bf66a3ca55d2646a48}





#### `public inline `[`ServerConnection`](#classscy_1_1http_1_1ServerConnection)` & connection()` {#group__http_1ga6c6af0d65968b631b4e4f6a2258d677c}





#### `public inline `[`Request`](#classscy_1_1http_1_1Request)` & request()` {#group__http_1ga0992082586057556ba9ce5518e726c7e}





#### `public inline `[`Response`](#classscy_1_1http_1_1Response)` & response()` {#group__http_1gaa8c010ebf55ca723c56c6034b4a6f58e}





#### `protected `[`ServerConnection`](./doc/api-http.md#classscy_1_1http_1_1ServerConnection)` & _connection` {#group__http_1ga23b4b87c41034983a2116b90ed31c0b3}





# class `scy::http::ServerResponderFactory` {#classscy_1_1http_1_1ServerResponderFactory}




This implementation of a [ServerResponderFactory](#classscy_1_1http_1_1ServerResponderFactory) is used by HTTPServer to create [ServerResponder](#classscy_1_1http_1_1ServerResponder) objects.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  ServerResponderFactory()` | 
`public inline virtual  ~ServerResponderFactory()` | 
`public `[`ServerResponder`](#classscy_1_1http_1_1ServerResponder)` * createResponder(`[`ServerConnection`](#classscy_1_1http_1_1ServerConnection)` & connection)` | 

## Members

#### `public inline  ServerResponderFactory()` {#group__http_1ga85068533df3d2e9879d4ccd9a6d881f5}





#### `public inline virtual  ~ServerResponderFactory()` {#group__http_1ga3727975eced25114e50e82b2a8213ea4}





#### `public `[`ServerResponder`](#classscy_1_1http_1_1ServerResponder)` * createResponder(`[`ServerConnection`](#classscy_1_1http_1_1ServerConnection)` & connection)` {#group__http_1ga861a7c189ceb70396ba44a512c1e40f4}



Factory method for instantiating the [ServerResponder](#classscy_1_1http_1_1ServerResponder) instance using the given [ServerConnection](#classscy_1_1http_1_1ServerConnection).

# class `scy::http::StringPart` {#classscy_1_1http_1_1StringPart}

```
class scy::http::StringPart
  : public scy::http::FormPart
```  

An implementation of [StringPart](#classscy_1_1http_1_1StringPart) for plain files.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  StringPart(const std::string & path)` | Creates the [StringPart](./doc/api-http.md#classscy_1_1http_1_1StringPart) for the given string.
`public  StringPart(const std::string & data,const std::string & contentType)` | Creates the [StringPart](./doc/api-http.md#classscy_1_1http_1_1StringPart) for the given string and MIME type.
`public virtual  ~StringPart()` | Destroys the [StringPart](./doc/api-http.md#classscy_1_1http_1_1StringPart).
`public virtual bool writeChunk(`[`FormWriter`](#classscy_1_1http_1_1FormWriter)` & writer)` | 
`public virtual void write(`[`FormWriter`](#classscy_1_1http_1_1FormWriter)` & writer)` | Writes the form data to the given HTTP client connection.
`public virtual void write(std::ostream & ostr)` | Writes the form data to the given output stream.
`public virtual std::uint64_t length() const` | Returns the length of the current part.
`protected std::string _data` | 

## Members

#### `public  StringPart(const std::string & path)` {#group__http_1ga0b5884a2d863ceb7cce47c351d4de3cf}

Creates the [StringPart](#classscy_1_1http_1_1StringPart) for the given string.



#### `public  StringPart(const std::string & data,const std::string & contentType)` {#group__http_1ga54d19c7cbabf1a1052dcdbb9c7bce61c}

Creates the [StringPart](#classscy_1_1http_1_1StringPart) for the given string and MIME type.



#### `public virtual  ~StringPart()` {#group__http_1ga377cfef62f1d4c5088d4dc61b9a2689e}

Destroys the [StringPart](#classscy_1_1http_1_1StringPart).



#### `public virtual bool writeChunk(`[`FormWriter`](#classscy_1_1http_1_1FormWriter)` & writer)` {#group__http_1ga1a8a1a3f648f0edb715cf5ac42275f14}



Writes a form data chunk to the given HTTP client connection. Returns true if there is more data to be written.

#### `public virtual void write(`[`FormWriter`](#classscy_1_1http_1_1FormWriter)` & writer)` {#group__http_1ga73600ffb13f790118e3ae97ca028f88d}

Writes the form data to the given HTTP client connection.



#### `public virtual void write(std::ostream & ostr)` {#group__http_1ga346d908a6b89c22a370ae88c616d48a1}

Writes the form data to the given output stream.



#### `public virtual std::uint64_t length() const` {#group__http_1ga3b556f40c37ca98d630e4c8be3b09a50}

Returns the length of the current part.



#### `protected std::string _data` {#group__http_1ga96f0a59bb5b72c46ec183f3e490b9071}





# class `scy::http::URL` {#classscy_1_1http_1_1URL}




An RFC 3986 based [URL](#classscy_1_1http_1_1URL) which uses an external c library to do the heavy lifting. Constructors and assignment operators will throw a SyntaxException if the [URL](#classscy_1_1http_1_1URL) is invalid.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  URL()` | 
`public  URL(const char * url)` | 
`public  URL(const std::string & url)` | 
`public  URL(const std::string & scheme,const std::string & authority)` | 
`public  URL(const std::string & scheme,const std::string & authority,const std::string & pathEtc)` | 
`public  URL(const std::string & scheme,const std::string & authority,const std::string & path,const std::string & query,const std::string & fragment)` | 
`public  ~URL()` | 
`public `[`URL`](#classscy_1_1http_1_1URL)` & operator=(const `[`URL`](#classscy_1_1http_1_1URL)` & uri)` | 
`public `[`URL`](#classscy_1_1http_1_1URL)` & operator=(const std::string & uri)` | 
`public `[`URL`](#classscy_1_1http_1_1URL)` & operator=(const char * uri)` | 
`public bool parse(const std::string & url,bool whiny)` | 
`public std::string scheme() const` | 
`public std::string userInfo() const` | 
`public std::string host() const` | 
`public std::uint16_t port() const` | 
`public std::string authority() const` | 
`public std::string path() const` | 
`public std::string pathEtc() const` | 
`public std::string query() const` | 
`public std::string fragment() const` | 
`public bool hasSchema() const` | 
`public bool hasUserInfo() const` | 
`public bool hasHost() const` | 
`public bool hasPort() const` | 
`public bool hasPath() const` | 
`public bool hasQuery() const` | 
`public bool hasFragment() const` | 
`public bool valid() const` | 
`public std::string str() const` | 
`protected http_parser_url _parser` | 
`protected std::string _buf` | 

## Members

#### `public  URL()` {#group__http_1ga0012ce3eef95070f554cac035e705e7c}





#### `public  URL(const char * url)` {#group__http_1ga9de97b22ce4f4af0459d8e9d7aaa8497}





#### `public  URL(const std::string & url)` {#group__http_1gac4d2bd2403c5f9cab0b66194a542f855}





#### `public  URL(const std::string & scheme,const std::string & authority)` {#group__http_1gae12d25102f9ec2eb7092d226237c08bd}





#### `public  URL(const std::string & scheme,const std::string & authority,const std::string & pathEtc)` {#group__http_1ga6830dee9798a13b656e8e34411d7e4ae}





#### `public  URL(const std::string & scheme,const std::string & authority,const std::string & path,const std::string & query,const std::string & fragment)` {#group__http_1ga67f3316095f007ee0e121bacb2c018ad}





#### `public  ~URL()` {#group__http_1ga0c834ba842f2f8f344d8117c06e8715c}





#### `public `[`URL`](#classscy_1_1http_1_1URL)` & operator=(const `[`URL`](#classscy_1_1http_1_1URL)` & uri)` {#group__http_1gaeff1d481a38a0954989bf182742eadea}





#### `public `[`URL`](#classscy_1_1http_1_1URL)` & operator=(const std::string & uri)` {#group__http_1gafb9d3e584511e3b861901d01095e445d}





#### `public `[`URL`](#classscy_1_1http_1_1URL)` & operator=(const char * uri)` {#group__http_1ga3730a1d5b734bcd14c13215857558472}





#### `public bool parse(const std::string & url,bool whiny)` {#group__http_1ga71cd693e288402f730de66eb26052c7a}



Parses and assigns an URI from the given std::string. Throws a SyntaxException if whiny is set and the given url is invalid.

#### `public std::string scheme() const` {#group__http_1ga13034b88cea824826c548354561a852a}





#### `public std::string userInfo() const` {#group__http_1ga0b0f24c4d86c9277604faf68aeb6badb}





#### `public std::string host() const` {#group__http_1ga4758b58c52c2856d2840169cf9e0786a}





#### `public std::uint16_t port() const` {#group__http_1gabb58d9cb2abc0e52caa547ebebdf1765}





#### `public std::string authority() const` {#group__http_1ga2fc7a6437283ab979476f0b772aa1cf0}





#### `public std::string path() const` {#group__http_1gaf31b015725319620264f05991b4d8b05}





#### `public std::string pathEtc() const` {#group__http_1gacf28e36ca3cf0c3fa013fb59b91c15ed}





#### `public std::string query() const` {#group__http_1ga0d6148d4f756d4c50105e8a960b61165}





#### `public std::string fragment() const` {#group__http_1gac19a19dbc857d249c3053813c91a1197}





#### `public bool hasSchema() const` {#group__http_1ga3d640b9dd8a7b44e4dd089c0416386a4}





#### `public bool hasUserInfo() const` {#group__http_1ga90997ee80f17e40d5cfe020b4cbb71f8}





#### `public bool hasHost() const` {#group__http_1ga36a7164574eb94971d7e77fdca25fbf9}





#### `public bool hasPort() const` {#group__http_1gaa101f8ff29ed88ee08c6c67bdadc25b4}





#### `public bool hasPath() const` {#group__http_1ga561e6340aec22a24ed878cbbfd9efba4}





#### `public bool hasQuery() const` {#group__http_1ga735e5e96b0c5b77ea26f40f8c1c124b1}





#### `public bool hasFragment() const` {#group__http_1gaacf35655cd015c098d99134053a3cad4}





#### `public bool valid() const` {#group__http_1ga84294885e92d117d63cf9aff519b8a44}





#### `public std::string str() const` {#group__http_1ga8778b74bf16d66a0fac0f83b250e0c0b}





#### `protected http_parser_url _parser` {#group__http_1gaf084881c64e6bc00e6560e9cb5e95290}





#### `protected std::string _buf` {#group__http_1ga3399c457c9e15e3f4f52b1e9028b7e1e}





# struct `scy::http::Method` {#structscy_1_1http_1_1Method}


HTTP request methods.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------

## Members

# struct `scy::http::ParserError` {#structscy_1_1http_1_1ParserError}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public http_errno code` | 
`public std::string message` | 

## Members

#### `public http_errno code` {#group__http_1ga7da040c95cfb1e1e59e021d02ebfcc98}





#### `public std::string message` {#group__http_1ga4d7fe6ac82086d3c47c36c7b11ee7979}





# namespace `scy::http::ws` {#namespacescy_1_1http_1_1ws}



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[`scy::http::ws::ConnectionAdapter`](#classscy_1_1http_1_1ws_1_1ConnectionAdapter)    | [WebSocket](#classscy_1_1http_1_1ws_1_1WebSocket) class which belongs to a HTTP [Connection](#classscy_1_1http_1_1Connection).
`class `[`scy::http::ws::WebSocket`](#classscy_1_1http_1_1ws_1_1WebSocket)    | Standalone [WebSocket](#classscy_1_1http_1_1ws_1_1WebSocket) class.
`class `[`scy::http::ws::WebSocketAdapter`](#classscy_1_1http_1_1ws_1_1WebSocketAdapter)    | 
`class `[`scy::http::ws::WebSocketFramer`](#classscy_1_1http_1_1ws_1_1WebSocketFramer)    | 
# class `scy::http::ws::ConnectionAdapter` {#classscy_1_1http_1_1ws_1_1ConnectionAdapter}

```
class scy::http::ws::ConnectionAdapter
  : public scy::http::ws::WebSocketAdapter
```  

[WebSocket](#classscy_1_1http_1_1ws_1_1WebSocket) class which belongs to a HTTP [Connection](#classscy_1_1http_1_1Connection).



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  ConnectionAdapter(`[`Connection`](#classscy_1_1http_1_1Connection)` & connection,`[`ws::Mode`](#group__http_1gaa9edb02676fb90927487e1de2afa287e)` mode)` | 
`public virtual  ~ConnectionAdapter()` | 
`public virtual void onHandshakeComplete()` | virtual int sendHeader();
`protected `[`Connection`](./doc/api-http.md#classscy_1_1http_1_1Connection)` & _connection` | 

## Members

#### `public  ConnectionAdapter(`[`Connection`](#classscy_1_1http_1_1Connection)` & connection,`[`ws::Mode`](#group__http_1gaa9edb02676fb90927487e1de2afa287e)` mode)` {#group__http_1gadd1285467fc4868fc4909bb91915768d}





#### `public virtual  ~ConnectionAdapter()` {#group__http_1gaee2000f846c1e426dceedd1c30a7c3be}





#### `public virtual void onHandshakeComplete()` {#group__http_1ga4da5f2517b482a40b7033c8931208039}

virtual int sendHeader();



#### `protected `[`Connection`](./doc/api-http.md#classscy_1_1http_1_1Connection)` & _connection` {#group__http_1gaa2280f7bfbdcade89c6df528b8360a33}





# class `scy::http::ws::WebSocket` {#classscy_1_1http_1_1ws_1_1WebSocket}

```
class scy::http::ws::WebSocket
  : public scy::http::ws::WebSocketAdapter
```  

Standalone [WebSocket](#classscy_1_1http_1_1ws_1_1WebSocket) class.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  WebSocket(const net::Socket::Ptr & socket)` | 
`public virtual  ~WebSocket()` | 
`public `[`http::Request`](#classscy_1_1http_1_1Request)` & request()` | 
`public `[`http::Response`](#classscy_1_1http_1_1Response)` & response()` | 
`protected `[`http::Request`](./doc/api-http.md#classscy_1_1http_1_1Request)` _request` | 
`protected `[`http::Response`](./doc/api-http.md#classscy_1_1http_1_1Response)` _response` | 

## Members

#### `public  WebSocket(const net::Socket::Ptr & socket)` {#group__http_1ga3c94305f34fd0937cc51457412672070}



Creates the [WebSocket](#classscy_1_1http_1_1ws_1_1WebSocket) with the given Socket. The Socket should be a TCPSocket or a SSLSocket, depending on the protocol used (ws or wss).

#### `public virtual  ~WebSocket()` {#group__http_1ga44166903755fb387c3cfd02ef8f4037b}





#### `public `[`http::Request`](#classscy_1_1http_1_1Request)` & request()` {#group__http_1gab2306735b2e76616d02e802cd46187b6}





#### `public `[`http::Response`](#classscy_1_1http_1_1Response)` & response()` {#group__http_1ga59013d9020baa540a2015b0aa84aa2b3}





#### `protected `[`http::Request`](./doc/api-http.md#classscy_1_1http_1_1Request)` _request` {#group__http_1gabe420b6263836453b99dd0b333f80da7}





#### `protected `[`http::Response`](./doc/api-http.md#classscy_1_1http_1_1Response)` _response` {#group__http_1ga54257ef6c009b7d41e6849a3211fd0fc}





# class `scy::http::ws::WebSocketAdapter` {#classscy_1_1http_1_1ws_1_1WebSocketAdapter}

```
class scy::http::ws::WebSocketAdapter
  : public scy::net::SocketAdapter
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public net::Socket::Ptr socket` | 
`public  WebSocketAdapter(const net::Socket::Ptr & socket,`[`ws::Mode`](#group__http_1gaa9edb02676fb90927487e1de2afa287e)` mode,`[`http::Request`](#classscy_1_1http_1_1Request)` & request,`[`http::Response`](#classscy_1_1http_1_1Response)` & response)` | 
`public virtual int send(const char * data,std::size_t len,int flags)` | 
`public virtual int send(const char * data,std::size_t len,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddr,int flags)` | 
`public virtual bool shutdown(std::uint16_t statusCode,const std::string & statusMessage)` | 
`public virtual void sendClientRequest()` | [Client](./doc/api-http.md#classscy_1_1http_1_1Client) side.
`public virtual void handleClientResponse(const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddr)` | 
`public virtual void handleServerRequest(const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddr)` | [Server](./doc/api-http.md#classscy_1_1http_1_1Server) side.
`public virtual void onSocketConnect(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket)` | 
`public virtual void onSocketRecv(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket,const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
`public virtual void onSocketClose(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket)` | 
`public virtual void onHandshakeComplete()` | 
`protected `[`WebSocketFramer`](./doc/api-http.md#classscy_1_1http_1_1ws_1_1WebSocketFramer)` framer` | 
`protected `[`http::Request`](./doc/api-http.md#classscy_1_1http_1_1Request)` & _request` | 
`protected `[`http::Response`](./doc/api-http.md#classscy_1_1http_1_1Response)` & _response` | 
`protected virtual  ~WebSocketAdapter()` | 

## Members

#### `public net::Socket::Ptr socket` {#group__http_1ga194468c2c928bbac6d1519cc43693fce}



Pointer to the underlying socket. Sent data will be proxied to this socket.

#### `public  WebSocketAdapter(const net::Socket::Ptr & socket,`[`ws::Mode`](#group__http_1gaa9edb02676fb90927487e1de2afa287e)` mode,`[`http::Request`](#classscy_1_1http_1_1Request)` & request,`[`http::Response`](#classscy_1_1http_1_1Response)` & response)` {#group__http_1ga0c90407e7aa55a076ce25b0e7bd45945}





#### `public virtual int send(const char * data,std::size_t len,int flags)` {#group__http_1ga0c29806b89ede1de7e0b46712e5a17a9}



Sends the given data buffer to the connected peer. Returns the number of bytes sent or -1 on error. No exception will be thrown. For TCP sockets the given peer address must match the connected peer address.

#### `public virtual int send(const char * data,std::size_t len,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddr,int flags)` {#group__http_1ga89ae271288746b9534538ac0e65c7e28}





#### `public virtual bool shutdown(std::uint16_t statusCode,const std::string & statusMessage)` {#group__http_1gaf675ce63f6ad0920ce21ebedeb5a35f0}





#### `public virtual void sendClientRequest()` {#group__http_1ga5b2297be10b0728276e95325cb49dd87}

[Client](#classscy_1_1http_1_1Client) side.



#### `public virtual void handleClientResponse(const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddr)` {#group__http_1ga0a4e2c205c5d0c64f27677751cf5acfc}





#### `public virtual void handleServerRequest(const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddr)` {#group__http_1ga2df61cf3b5dfb720cff8d61c88eebbc5}

[Server](#classscy_1_1http_1_1Server) side.



#### `public virtual void onSocketConnect(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket)` {#group__http_1ga0134364a09e37e939ff44f7d3aa7ea39}



These virtual methods can be overridden as necessary to intercept socket events before they hit the application.

#### `public virtual void onSocketRecv(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket,const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` {#group__http_1gae617802894aa5c20d0e636bbdf15d09e}





#### `public virtual void onSocketClose(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket)` {#group__http_1ga02a8a98e9741e90a5ef0906e1380205b}





#### `public virtual void onHandshakeComplete()` {#group__http_1ga3725543bf358559cd80d180adfffbb6a}





#### `protected `[`WebSocketFramer`](./doc/api-http.md#classscy_1_1http_1_1ws_1_1WebSocketFramer)` framer` {#group__http_1ga5b9dd45fd977d97d4bed67c41d85337c}





#### `protected `[`http::Request`](./doc/api-http.md#classscy_1_1http_1_1Request)` & _request` {#group__http_1ga38efeb718ff37412f94f07e69abe837f}





#### `protected `[`http::Response`](./doc/api-http.md#classscy_1_1http_1_1Response)` & _response` {#group__http_1ga68e67b65349a7f1a1c9c916185192368}





#### `protected virtual  ~WebSocketAdapter()` {#group__http_1ga05aba918e9afe39069fadda8cf41a2b9}





# class `scy::http::ws::WebSocketFramer` {#classscy_1_1http_1_1ws_1_1WebSocketFramer}




This class implements a [WebSocket](#classscy_1_1http_1_1ws_1_1WebSocket) parser according to the [WebSocket](#classscy_1_1http_1_1ws_1_1WebSocket) protocol described in RFC 6455.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  WebSocketFramer(`[`ws::Mode`](#group__http_1gaa9edb02676fb90927487e1de2afa287e)` mode)` | Creates a Socket using the given Socket.
`public virtual  ~WebSocketFramer()` | 
`public virtual std::size_t writeFrame(const char * data,std::size_t len,int flags,`[`BitWriter`](#classscy_1_1BitWriter)` & frame)` | Writes a [WebSocket](./doc/api-http.md#classscy_1_1http_1_1ws_1_1WebSocket) protocol frame from the given data.
`public virtual std::uint64_t readFrame(`[`BitReader`](#classscy_1_1BitReader)` & frame,char *& payload)` | 
`public bool handshakeComplete() const` | 
`public void acceptServerRequest(`[`http::Request`](#classscy_1_1http_1_1Request)` & request,`[`http::Response`](#classscy_1_1http_1_1Response)` & response)` | 
`public void createClientHandshakeRequest(`[`http::Request`](#classscy_1_1http_1_1Request)` & request)` | Appends the WS hanshake HTTP request hearers.
`public bool checkClientHandshakeResponse(`[`http::Response`](#classscy_1_1http_1_1Response)` & response)` | 
`public void completeClientHandshake(`[`http::Response`](#classscy_1_1http_1_1Response)` & response)` | Verifies the handshake response or thrown and exception.
`protected int frameFlags() const` | 
`protected bool mustMaskPayload() const` | 
`protected `[`ws::Mode`](#group__http_1gaa9edb02676fb90927487e1de2afa287e)` mode() const` | 

## Members

#### `public  WebSocketFramer(`[`ws::Mode`](#group__http_1gaa9edb02676fb90927487e1de2afa287e)` mode)` {#group__http_1ga1d91b9caac96874c2655910321dea79c}

Creates a Socket using the given Socket.



#### `public virtual  ~WebSocketFramer()` {#group__http_1ga7537c2cae222878058b613e1c476435e}





#### `public virtual std::size_t writeFrame(const char * data,std::size_t len,int flags,`[`BitWriter`](#classscy_1_1BitWriter)` & frame)` {#group__http_1ga33423fa89193d26b9d18d08ec62906a4}

Writes a [WebSocket](#classscy_1_1http_1_1ws_1_1WebSocket) protocol frame from the given data.



#### `public virtual std::uint64_t readFrame(`[`BitReader`](#classscy_1_1BitReader)` & frame,char *& payload)` {#group__http_1ga60fc4167796de84fb0718cef38a8cd45}



Reads a single [WebSocket](#classscy_1_1http_1_1ws_1_1WebSocket) frame from the given buffer (frame).

The actual payload length is returned, and the beginning of the payload buffer will be assigned in the second (payload) argument. No data is copied.

If the frame is invalid or too big an exception will be thrown. Return true when the handshake has completed successfully.

#### `public bool handshakeComplete() const` {#group__http_1ga9b1b58854f05b235baaecb0091c2ba3d}





#### `public void acceptServerRequest(`[`http::Request`](#classscy_1_1http_1_1Request)` & request,`[`http::Response`](#classscy_1_1http_1_1Response)` & response)` {#group__http_1ga6f6011b7baafbacc771dde625af0e31c}





#### `public void createClientHandshakeRequest(`[`http::Request`](#classscy_1_1http_1_1Request)` & request)` {#group__http_1ga894342a4b8bfd9ce4ebd661971f1cd66}

Appends the WS hanshake HTTP request hearers.

[Client](#classscy_1_1http_1_1Client) side Sends the initial WS handshake HTTP request. void sendHandshakeRequest();

#### `public bool checkClientHandshakeResponse(`[`http::Response`](#classscy_1_1http_1_1Response)` & response)` {#group__http_1gafb02df6ce83694ec969f3c7a1075c92e}



Checks the veracity the HTTP handshake response. Returns true on success, false if the request should be resent (in case of authentication), or throws on error.

#### `public void completeClientHandshake(`[`http::Response`](#classscy_1_1http_1_1Response)` & response)` {#group__http_1ga2c39e1fedb1dfdb494854ce07fcf458e}

Verifies the handshake response or thrown and exception.



#### `protected int frameFlags() const` {#group__http_1ga347aac0790ae49e9436df83f61a116cd}



Returns the frame flags of the most recently received frame. Set by [readFrame()](#group__http_1ga60fc4167796de84fb0718cef38a8cd45)

#### `protected bool mustMaskPayload() const` {#group__http_1ga81204f5e3fa61bc5439cc8f483bfc38e}



Returns true if the payload must be masked. Used by [writeFrame()](#group__http_1ga33423fa89193d26b9d18d08ec62906a4)

#### `protected `[`ws::Mode`](#group__http_1gaa9edb02676fb90927487e1de2afa287e)` mode() const` {#group__http_1ga7f7b3086af7abd333e0d1403d9e1f046}





# struct `scy::http::FormWriter::Part` {#structscy_1_1http_1_1FormWriter_1_1Part}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::string name` | 
`public `[`FormPart`](./doc/api-http.md#classscy_1_1http_1_1FormPart)` * part` | 

## Members

#### `public std::string name` {#group__http_1ga59be331e36d604c25ef7d780cc814e85}





#### `public `[`FormPart`](./doc/api-http.md#classscy_1_1http_1_1FormPart)` * part` {#group__http_1gac1cd7abd9bb03fd415b1667a90af74c5}





