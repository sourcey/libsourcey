# Module <!-- group --> `turn`

The `turn` module contains TURN (rfc5766) client and server implementations.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`namespace `[`scy::turn`](#namespacescy_1_1turn)    | 
`struct `[`scy::turn::Client::Options`](#structscy_1_1turn_1_1Client_1_1Options)    | 
# namespace `scy::turn` 



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[`scy::turn::Client`](#classscy_1_1turn_1_1Client)    | 
`class `[`scy::turn::FiveTuple`](#classscy_1_1turn_1_1FiveTuple)    | 
`class `[`scy::turn::IAllocation`](#classscy_1_1turn_1_1IAllocation)    | 
`class `[`scy::turn::Request`](#classscy_1_1turn_1_1Request)    | 
`class `[`scy::turn::Server`](#classscy_1_1turn_1_1Server)    | TURN server rfc5766 implementation.
`class `[`scy::turn::ServerAllocation`](#classscy_1_1turn_1_1ServerAllocation)    | 
`class `[`scy::turn::TCPAllocation`](#classscy_1_1turn_1_1TCPAllocation)    | 
`class `[`scy::turn::TCPClient`](#classscy_1_1turn_1_1TCPClient)    | 
`class `[`scy::turn::TCPConnectionPair`](#classscy_1_1turn_1_1TCPConnectionPair)    | 
`class `[`scy::turn::UDPAllocation`](#classscy_1_1turn_1_1UDPAllocation)    | 
`class `[`scy::turn::UDPClient`](#classscy_1_1turn_1_1UDPClient)    | 
`struct `[`scy::turn::ClientObserver`](#structscy_1_1turn_1_1ClientObserver)    | 
`struct `[`scy::turn::ClientState`](#structscy_1_1turn_1_1ClientState)    | 
`struct `[`scy::turn::Permission`](#structscy_1_1turn_1_1Permission)    | TURN permission for a user session.
`struct `[`scy::turn::RelayConnectionBinding`](#structscy_1_1turn_1_1RelayConnectionBinding)    | 
`struct `[`scy::turn::ServerObserver`](#structscy_1_1turn_1_1ServerObserver)    | 
`struct `[`scy::turn::ServerOptions`](#structscy_1_1turn_1_1ServerOptions)    | [Configuration](#classscy_1_1Configuration) options for the TURN server.
`struct `[`scy::turn::TCPClientObserver`](#structscy_1_1turn_1_1TCPClientObserver)    | 
# class `scy::turn::Client` 

```
class scy::turn::Client
  : public scy::Stateful< ClientState >
  : protected scy::turn::IAllocation
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Client(`[`ClientObserver`](#structscy_1_1turn_1_1ClientObserver)` & observer,const `[`Options`](#structscy_1_1turn_1_1Client_1_1Options)` & options,const net::Socket::Ptr & socket)` | 
`public virtual  ~Client()` | 
`public virtual void initiate()` | Initiates the allocation sequence.
`public virtual void shutdown()` | Shutdown the client and destroy the active allocation.
`public virtual void sendAllocate()` | Sends the allocation request.
`public virtual void addPermission(const IPList & peerIPs)` | 
`public virtual void addPermission(const std::string & ip)` | 
`public virtual void sendCreatePermission()` | Sends a CreatePermission request including all hosts.
`public virtual void sendChannelBind(const std::string & peerIP)` | 
`public virtual void sendRefresh()` | 
`public virtual void sendData(const char * data,size_t size,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
`public virtual bool handleResponse(const `[`stun::Message`](#classscy_1_1stun_1_1Message)` & response)` | 
`public virtual void handleAllocateResponse(const `[`stun::Message`](#classscy_1_1stun_1_1Message)` & response)` | 
`public virtual void handleAllocateErrorResponse(const `[`stun::Message`](#classscy_1_1stun_1_1Message)` & response)` | 
`public virtual void handleCreatePermissionResponse(const `[`stun::Message`](#classscy_1_1stun_1_1Message)` & response)` | 
`public virtual void handleCreatePermissionErrorResponse(const `[`stun::Message`](#classscy_1_1stun_1_1Message)` & response)` | 
`public virtual void handleRefreshResponse(const `[`stun::Message`](#classscy_1_1stun_1_1Message)` & response)` | 
`public virtual void handleDataIndication(const `[`stun::Message`](#classscy_1_1stun_1_1Message)` & response)` | 
`public virtual int transportProtocol()` | 
`public virtual `[`stun::Transaction`](#classscy_1_1stun_1_1Transaction)` * createTransaction(const net::Socket::Ptr & socket)` | 
`public virtual void authenticateRequest(`[`stun::Message`](#classscy_1_1stun_1_1Message)` & request)` | 
`public virtual bool sendAuthenticatedTransaction(`[`stun::Transaction`](#classscy_1_1stun_1_1Transaction)` * transaction)` | 
`public virtual bool removeTransaction(`[`stun::Transaction`](#classscy_1_1stun_1_1Transaction)` * transaction)` | 
`public `[`net::Address`](#classscy_1_1net_1_1Address)` mappedAddress() const` | 
`public virtual `[`net::Address`](#classscy_1_1net_1_1Address)` relayedAddress() const` | 
`public bool closed() const` | 
`public `[`ClientObserver`](#structscy_1_1turn_1_1ClientObserver)` & observer()` | 
`public `[`Options`](#structscy_1_1turn_1_1Client_1_1Options)` & options()` | 
`protected `[`ClientObserver`](./doc/api-turn.md#structscy_1_1turn_1_1ClientObserver)` & _observer` | 
`protected `[`Options`](./doc/api-turn.md#structscy_1_1turn_1_1Client_1_1Options)` _options` | 
`protected `[`net::SocketEmitter`](./doc/api-net.md#classscy_1_1net_1_1SocketEmitter)` _socket` | 
`protected `[`Timer`](./doc/api-base.md#classscy_1_1Timer)` _timer` | 
`protected `[`scy::Error`](./doc/api-base.md#structscy_1_1Error)` _error` | 
`protected `[`net::Address`](./doc/api-net.md#classscy_1_1net_1_1Address)` _mappedAddress` | 
`protected `[`net::Address`](./doc/api-net.md#classscy_1_1net_1_1Address)` _relayedAddress` | 
`protected std::string _realm` | 
`protected std::string _nonce` | 
`protected std::deque< `[`stun::Message`](./doc/api-stun.md#classscy_1_1stun_1_1Message)` > _pendingIndications` | A list of queued Send indication packets awaiting server permissions.
`protected std::vector< `[`stun::Transaction`](./doc/api-stun.md#classscy_1_1stun_1_1Transaction)` * > _transactions` | A list containing currently active transactions.
`protected virtual void setError(const `[`scy::Error`](#structscy_1_1Error)` & error)` | 
`protected virtual void onSocketConnect(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket)` | 
`protected virtual void onSocketRecv(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket,const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
`protected virtual void onSocketClose(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket)` | 
`protected virtual void onTransactionProgress(void * sender,`[`TransactionState`](#structscy_1_1TransactionState)` & state,const `[`TransactionState`](#structscy_1_1TransactionState)` &)` | 
`protected virtual void onStateChange(`[`ClientState`](#structscy_1_1turn_1_1ClientState)` & state,const `[`ClientState`](#structscy_1_1turn_1_1ClientState)` & oldState)` | 
`protected virtual void onTimer()` | 

## Members

#### `public  Client(`[`ClientObserver`](#structscy_1_1turn_1_1ClientObserver)` & observer,const `[`Options`](#structscy_1_1turn_1_1Client_1_1Options)` & options,const net::Socket::Ptr & socket)` 





#### `public virtual  ~Client()` 





#### `public virtual void initiate()` 

Initiates the allocation sequence.



#### `public virtual void shutdown()` 

Shutdown the client and destroy the active allocation.



#### `public virtual void sendAllocate()` 

Sends the allocation request.



#### `public virtual void addPermission(const IPList & peerIPs)` 





#### `public virtual void addPermission(const std::string & ip)` 



Peer permissions should be added/created before we kick off the allocation sequence, but may be added later.

#### `public virtual void sendCreatePermission()` 

Sends a CreatePermission request including all hosts.

A CreatePermission request will be sent as soon as the Allocation is created, and at timer x intervals.

#### `public virtual void sendChannelBind(const std::string & peerIP)` 





#### `public virtual void sendRefresh()` 





#### `public virtual void sendData(const char * data,size_t size,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` 





#### `public virtual bool handleResponse(const `[`stun::Message`](#classscy_1_1stun_1_1Message)` & response)` 





#### `public virtual void handleAllocateResponse(const `[`stun::Message`](#classscy_1_1stun_1_1Message)` & response)` 





#### `public virtual void handleAllocateErrorResponse(const `[`stun::Message`](#classscy_1_1stun_1_1Message)` & response)` 





#### `public virtual void handleCreatePermissionResponse(const `[`stun::Message`](#classscy_1_1stun_1_1Message)` & response)` 





#### `public virtual void handleCreatePermissionErrorResponse(const `[`stun::Message`](#classscy_1_1stun_1_1Message)` & response)` 





#### `public virtual void handleRefreshResponse(const `[`stun::Message`](#classscy_1_1stun_1_1Message)` & response)` 





#### `public virtual void handleDataIndication(const `[`stun::Message`](#classscy_1_1stun_1_1Message)` & response)` 





#### `public virtual int transportProtocol()` 





#### `public virtual `[`stun::Transaction`](#classscy_1_1stun_1_1Transaction)` * createTransaction(const net::Socket::Ptr & socket)` 





#### `public virtual void authenticateRequest(`[`stun::Message`](#classscy_1_1stun_1_1Message)` & request)` 





#### `public virtual bool sendAuthenticatedTransaction(`[`stun::Transaction`](#classscy_1_1stun_1_1Transaction)` * transaction)` 





#### `public virtual bool removeTransaction(`[`stun::Transaction`](#classscy_1_1stun_1_1Transaction)` * transaction)` 





#### `public `[`net::Address`](#classscy_1_1net_1_1Address)` mappedAddress() const` 





#### `public virtual `[`net::Address`](#classscy_1_1net_1_1Address)` relayedAddress() const` 





#### `public bool closed() const` 





#### `public `[`ClientObserver`](#structscy_1_1turn_1_1ClientObserver)` & observer()` 





#### `public `[`Options`](#structscy_1_1turn_1_1Client_1_1Options)` & options()` 





#### `protected `[`ClientObserver`](./doc/api-turn.md#structscy_1_1turn_1_1ClientObserver)` & _observer` 





#### `protected `[`Options`](./doc/api-turn.md#structscy_1_1turn_1_1Client_1_1Options)` _options` 





#### `protected `[`net::SocketEmitter`](./doc/api-net.md#classscy_1_1net_1_1SocketEmitter)` _socket` 





#### `protected `[`Timer`](./doc/api-base.md#classscy_1_1Timer)` _timer` 





#### `protected `[`scy::Error`](./doc/api-base.md#structscy_1_1Error)` _error` 





#### `protected `[`net::Address`](./doc/api-net.md#classscy_1_1net_1_1Address)` _mappedAddress` 





#### `protected `[`net::Address`](./doc/api-net.md#classscy_1_1net_1_1Address)` _relayedAddress` 





#### `protected std::string _realm` 





#### `protected std::string _nonce` 





#### `protected std::deque< `[`stun::Message`](./doc/api-stun.md#classscy_1_1stun_1_1Message)` > _pendingIndications` 

A list of queued Send indication packets awaiting server permissions.



#### `protected std::vector< `[`stun::Transaction`](./doc/api-stun.md#classscy_1_1stun_1_1Transaction)` * > _transactions` 

A list containing currently active transactions.



#### `protected virtual void setError(const `[`scy::Error`](#structscy_1_1Error)` & error)` 





#### `protected virtual void onSocketConnect(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket)` 





#### `protected virtual void onSocketRecv(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket,const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` 





#### `protected virtual void onSocketClose(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket)` 





#### `protected virtual void onTransactionProgress(void * sender,`[`TransactionState`](#structscy_1_1TransactionState)` & state,const `[`TransactionState`](#structscy_1_1TransactionState)` &)` 





#### `protected virtual void onStateChange(`[`ClientState`](#structscy_1_1turn_1_1ClientState)` & state,const `[`ClientState`](#structscy_1_1turn_1_1ClientState)` & oldState)` 





#### `protected virtual void onTimer()` 





# class `scy::turn::FiveTuple` 




The 5-TUPLE consists of a local, a remote address, and the transport protocol used by the client to communicate with the server.                                                           +---------+
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

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  FiveTuple()` | 
`public  FiveTuple(const `[`net::Address`](#classscy_1_1net_1_1Address)` & remote,const `[`net::Address`](#classscy_1_1net_1_1Address)` & local,net::TransportType transport)` | 
`public  FiveTuple(const `[`FiveTuple`](#classscy_1_1turn_1_1FiveTuple)` & r)` | 
`public inline const `[`net::Address`](#classscy_1_1net_1_1Address)` & remote() const` | 
`public inline const `[`net::Address`](#classscy_1_1net_1_1Address)` & local() const` | 
`public inline const net::TransportType & transport() const` | 
`public inline void remote(const `[`net::Address`](#classscy_1_1net_1_1Address)` & remote)` | 
`public inline void local(const `[`net::Address`](#classscy_1_1net_1_1Address)` & local)` | 
`public inline void transport(const net::TransportType & transport)` | 
`public bool operator==(const `[`FiveTuple`](#classscy_1_1turn_1_1FiveTuple)` & r) const` | 
`public bool operator<(const `[`FiveTuple`](#classscy_1_1turn_1_1FiveTuple)` & r) const` | 
`public std::string toString() const` | 

## Members

#### `public  FiveTuple()` 





#### `public  FiveTuple(const `[`net::Address`](#classscy_1_1net_1_1Address)` & remote,const `[`net::Address`](#classscy_1_1net_1_1Address)` & local,net::TransportType transport)` 





#### `public  FiveTuple(const `[`FiveTuple`](#classscy_1_1turn_1_1FiveTuple)` & r)` 





#### `public inline const `[`net::Address`](#classscy_1_1net_1_1Address)` & remote() const` 





#### `public inline const `[`net::Address`](#classscy_1_1net_1_1Address)` & local() const` 





#### `public inline const net::TransportType & transport() const` 





#### `public inline void remote(const `[`net::Address`](#classscy_1_1net_1_1Address)` & remote)` 





#### `public inline void local(const `[`net::Address`](#classscy_1_1net_1_1Address)` & local)` 





#### `public inline void transport(const net::TransportType & transport)` 





#### `public bool operator==(const `[`FiveTuple`](#classscy_1_1turn_1_1FiveTuple)` & r) const` 





#### `public bool operator<(const `[`FiveTuple`](#classscy_1_1turn_1_1FiveTuple)` & r) const` 





#### `public std::string toString() const` 





# class `scy::turn::IAllocation` 




All TURN operations revolve around allocations, and all TURN messages are associated with an allocation. An allocation conceptually consists of the following state data:



* the relayed transport address;


* the 5-tuple: (client's IP address, client's port, server IP address, server port, transport protocol);


* the authentication information;


* the time-to-expiry;





a list of permissions;

* a list of channel to peer bindings.





The relayed transport address is the transport address allocated by the server for communicating with peers, while the 5-tuple describes the communication path between the client and the server. On the client, the 5-tuple uses the client's host transport address; on the server, the 5-tuple uses the client's server-reflexive transport address.

Both the relayed transport address and the 5-tuple MUST be unique across all allocations, so either one can be used to uniquely identify the allocation. The authentication information (e.g., username, password, realm, and nonce) is used to both verify subsequent requests and to compute the message integrity of responses. The username, realm, and nonce values are initially those used in the authenticated Allocate request that creates the allocation, though the server can change the nonce value during the lifetime of the allocation using a 438 (Stale Nonce) reply. Note that, rather than storing the password explicitly, for security reasons, it may be desirable for the server to store the key value, which is an MD5 hash over the username, realm, and password (see [RFC5389]).

The time-to-expiry is the time in seconds left until the allocation expires. Each Allocate or Refresh transaction sets this timer, which then ticks down towards 0. By default, each Allocate or Refresh transaction resets this timer to the default lifetime value of 600 seconds (10 minutes), but the client can request a different value in the Allocate and Refresh request. Allocations can only be refreshed using the Refresh request; sending data to a peer does not refresh an allocation. When an allocation expires, the state data associated with the allocation can be freed.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  IAllocation(const `[`FiveTuple`](#classscy_1_1turn_1_1FiveTuple)` & tuple,const std::string & username,std::int64_t lifetime)` | 
`public virtual  ~IAllocation()` | 
`public virtual void updateUsage(std::int64_t numBytes)` | 
`public virtual void setLifetime(std::int64_t lifetime)` | Sets the lifetime of the allocation and resets the timeout.
`public virtual void setBandwidthLimit(std::int64_t numBytes)` | Sets the bandwidth limit in bytes for this allocation.
`public virtual bool expired() const` | 
`public virtual bool deleted() const` | 
`public virtual std::int64_t bandwidthLimit() const` | 
`public virtual std::int64_t bandwidthUsed() const` | 
`public virtual std::int64_t bandwidthRemaining() const` | 
`public virtual std::int64_t timeRemaining() const` | 
`public virtual `[`FiveTuple`](#classscy_1_1turn_1_1FiveTuple)` & tuple()` | 
`public virtual std::string username() const` | 
`public virtual std::int64_t lifetime() const` | 
`public virtual PermissionList permissions() const` | 
`public `[`net::Address`](#classscy_1_1net_1_1Address)` relayedAddress() const` | 
`public virtual void addPermission(const std::string & ip)` | 
`public virtual void addPermissions(const IPList & ips)` | 
`public virtual void removePermission(const std::string & ip)` | 
`public virtual void removeAllPermissions()` | 
`public virtual void removeExpiredPermissions()` | 
`public virtual bool hasPermission(const std::string & peerIP)` | 
`public inline virtual void print(std::ostream & os) const` | 
`protected `[`FiveTuple`](./doc/api-turn.md#classscy_1_1turn_1_1FiveTuple)` _tuple` | 
`protected std::string _username` | 
`protected PermissionList _permissions` | 
`protected std::int64_t _lifetime` | 
`protected std::int64_t _bandwidthLimit` | 
`protected std::int64_t _bandwidthUsed` | 
`protected time_t _createdAt` | 
`protected time_t _updatedAt` | 
`protected bool _deleted` | 

## Members

#### `public  IAllocation(const `[`FiveTuple`](#classscy_1_1turn_1_1FiveTuple)` & tuple,const std::string & username,std::int64_t lifetime)` 





#### `public virtual  ~IAllocation()` 





#### `public virtual void updateUsage(std::int64_t numBytes)` 



Updates the allocation's internal timeout and bandwidth usage each time the allocation is used.

#### `public virtual void setLifetime(std::int64_t lifetime)` 

Sets the lifetime of the allocation and resets the timeout.



#### `public virtual void setBandwidthLimit(std::int64_t numBytes)` 

Sets the bandwidth limit in bytes for this allocation.



#### `public virtual bool expired() const` 



Returns true if the allocation is expired ie. is timed out or the bandwidth limit has been reached.

#### `public virtual bool deleted() const` 



Returns true if the allocation's deleted flag is set and or if the allocation has expired.

This signifies that the allocation is ready to be destroyed via async garbage collection. See Server::onTimer() and Client::onTimer()

#### `public virtual std::int64_t bandwidthLimit() const` 





#### `public virtual std::int64_t bandwidthUsed() const` 





#### `public virtual std::int64_t bandwidthRemaining() const` 





#### `public virtual std::int64_t timeRemaining() const` 





#### `public virtual `[`FiveTuple`](#classscy_1_1turn_1_1FiveTuple)` & tuple()` 





#### `public virtual std::string username() const` 





#### `public virtual std::int64_t lifetime() const` 





#### `public virtual PermissionList permissions() const` 





#### `public `[`net::Address`](#classscy_1_1net_1_1Address)` relayedAddress() const` 





#### `public virtual void addPermission(const std::string & ip)` 





#### `public virtual void addPermissions(const IPList & ips)` 





#### `public virtual void removePermission(const std::string & ip)` 





#### `public virtual void removeAllPermissions()` 





#### `public virtual void removeExpiredPermissions()` 





#### `public virtual bool hasPermission(const std::string & peerIP)` 





#### `public inline virtual void print(std::ostream & os) const` 





#### `protected `[`FiveTuple`](./doc/api-turn.md#classscy_1_1turn_1_1FiveTuple)` _tuple` 





#### `protected std::string _username` 





#### `protected PermissionList _permissions` 





#### `protected std::int64_t _lifetime` 





#### `protected std::int64_t _bandwidthLimit` 





#### `protected std::int64_t _bandwidthUsed` 





#### `protected time_t _createdAt` 





#### `protected time_t _updatedAt` 





#### `protected bool _deleted` 





# class `scy::turn::Request` 

```
class scy::turn::Request
  : public scy::stun::Message
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public net::TransportType transport` | 
`public `[`net::Address`](./doc/api-net.md#classscy_1_1net_1_1Address)` localAddress` | 
`public `[`net::Address`](./doc/api-net.md#classscy_1_1net_1_1Address)` remoteAddress` | 
`public std::string hash` | 
`public inline  Request(const `[`stun::Message`](#classscy_1_1stun_1_1Message)` & message,net::TransportType transport,const `[`net::Address`](#classscy_1_1net_1_1Address)` & localAddress,const `[`net::Address`](#classscy_1_1net_1_1Address)` & remoteAddress)` | 

## Members

#### `public net::TransportType transport` 





#### `public `[`net::Address`](./doc/api-net.md#classscy_1_1net_1_1Address)` localAddress` 





#### `public `[`net::Address`](./doc/api-net.md#classscy_1_1net_1_1Address)` remoteAddress` 





#### `public std::string hash` 





#### `public inline  Request(const `[`stun::Message`](#classscy_1_1stun_1_1Message)` & message,net::TransportType transport,const `[`net::Address`](#classscy_1_1net_1_1Address)` & localAddress,const `[`net::Address`](#classscy_1_1net_1_1Address)` & remoteAddress)` 





# class `scy::turn::Server` 


TURN server rfc5766 implementation.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Server(`[`ServerObserver`](#structscy_1_1turn_1_1ServerObserver)` & observer,const `[`ServerOptions`](#structscy_1_1turn_1_1ServerOptions)` & options)` | 
`public virtual  ~Server()` | 
`public virtual void start()` | 
`public virtual void stop()` | 
`public void handleRequest(`[`Request`](#classscy_1_1turn_1_1Request)` & request,AuthenticationState state)` | 
`public void handleAuthorizedRequest(`[`Request`](#classscy_1_1turn_1_1Request)` & request)` | 
`public void handleBindingRequest(`[`Request`](#classscy_1_1turn_1_1Request)` & request)` | 
`public void handleAllocateRequest(`[`Request`](#classscy_1_1turn_1_1Request)` & request)` | 
`public void handleConnectionBindRequest(`[`Request`](#classscy_1_1turn_1_1Request)` & request)` | 
`public void respond(`[`Request`](#classscy_1_1turn_1_1Request)` & request,`[`stun::Message`](#classscy_1_1stun_1_1Message)` & response)` | 
`public void respondError(`[`Request`](#classscy_1_1turn_1_1Request)` & request,int errorCode,const char * errorDesc)` | 
`public ServerAllocationMap allocations() const` | 
`public void addAllocation(`[`ServerAllocation`](#classscy_1_1turn_1_1ServerAllocation)` * alloc)` | 
`public void removeAllocation(`[`ServerAllocation`](#classscy_1_1turn_1_1ServerAllocation)` * alloc)` | 
`public `[`ServerAllocation`](#classscy_1_1turn_1_1ServerAllocation)` * getAllocation(const `[`FiveTuple`](#classscy_1_1turn_1_1FiveTuple)` & tuple)` | 
`public `[`TCPAllocation`](#classscy_1_1turn_1_1TCPAllocation)` * getTCPAllocation(const uint32_t & connectionID)` | 
`public net::TCPSocket::Ptr getTCPSocket(const `[`net::Address`](#classscy_1_1net_1_1Address)` & remoteAddr)` | 
`public void releaseTCPSocket(const `[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket)` | 
`public `[`ServerObserver`](#structscy_1_1turn_1_1ServerObserver)` & observer()` | 
`public `[`ServerOptions`](#structscy_1_1turn_1_1ServerOptions)` & options()` | 
`public `[`net::UDPSocket`](#classscy_1_1net_1_1UDPSocket)` & udpSocket()` | 
`public `[`net::TCPSocket`](#classscy_1_1net_1_1TCPSocket)` & tcpSocket()` | 
`public `[`Timer`](#classscy_1_1Timer)` & timer()` | 
`public void onTCPAcceptConnection(const net::TCPSocket::Ptr & sock)` | 
`public void onTCPSocketClosed(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket)` | 
`public void onSocketRecv(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket,const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
`public void onTimer()` | 

## Members

#### `public  Server(`[`ServerObserver`](#structscy_1_1turn_1_1ServerObserver)` & observer,const `[`ServerOptions`](#structscy_1_1turn_1_1ServerOptions)` & options)` 





#### `public virtual  ~Server()` 





#### `public virtual void start()` 





#### `public virtual void stop()` 





#### `public void handleRequest(`[`Request`](#classscy_1_1turn_1_1Request)` & request,AuthenticationState state)` 





#### `public void handleAuthorizedRequest(`[`Request`](#classscy_1_1turn_1_1Request)` & request)` 





#### `public void handleBindingRequest(`[`Request`](#classscy_1_1turn_1_1Request)` & request)` 





#### `public void handleAllocateRequest(`[`Request`](#classscy_1_1turn_1_1Request)` & request)` 





#### `public void handleConnectionBindRequest(`[`Request`](#classscy_1_1turn_1_1Request)` & request)` 





#### `public void respond(`[`Request`](#classscy_1_1turn_1_1Request)` & request,`[`stun::Message`](#classscy_1_1stun_1_1Message)` & response)` 





#### `public void respondError(`[`Request`](#classscy_1_1turn_1_1Request)` & request,int errorCode,const char * errorDesc)` 





#### `public ServerAllocationMap allocations() const` 





#### `public void addAllocation(`[`ServerAllocation`](#classscy_1_1turn_1_1ServerAllocation)` * alloc)` 





#### `public void removeAllocation(`[`ServerAllocation`](#classscy_1_1turn_1_1ServerAllocation)` * alloc)` 





#### `public `[`ServerAllocation`](#classscy_1_1turn_1_1ServerAllocation)` * getAllocation(const `[`FiveTuple`](#classscy_1_1turn_1_1FiveTuple)` & tuple)` 





#### `public `[`TCPAllocation`](#classscy_1_1turn_1_1TCPAllocation)` * getTCPAllocation(const uint32_t & connectionID)` 





#### `public net::TCPSocket::Ptr getTCPSocket(const `[`net::Address`](#classscy_1_1net_1_1Address)` & remoteAddr)` 





#### `public void releaseTCPSocket(const `[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket)` 





#### `public `[`ServerObserver`](#structscy_1_1turn_1_1ServerObserver)` & observer()` 





#### `public `[`ServerOptions`](#structscy_1_1turn_1_1ServerOptions)` & options()` 





#### `public `[`net::UDPSocket`](#classscy_1_1net_1_1UDPSocket)` & udpSocket()` 





#### `public `[`net::TCPSocket`](#classscy_1_1net_1_1TCPSocket)` & tcpSocket()` 





#### `public `[`Timer`](#classscy_1_1Timer)` & timer()` 





#### `public void onTCPAcceptConnection(const net::TCPSocket::Ptr & sock)` 





#### `public void onTCPSocketClosed(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket)` 





#### `public void onSocketRecv(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket,const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` 





#### `public void onTimer()` 





# class `scy::turn::ServerAllocation` 

```
class scy::turn::ServerAllocation
  : public scy::turn::IAllocation
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  ServerAllocation(`[`Server`](#classscy_1_1turn_1_1Server)` & server,const `[`FiveTuple`](#classscy_1_1turn_1_1FiveTuple)` & tuple,const std::string & username,std::int64_t lifetime)` | 
`public virtual bool handleRequest(`[`Request`](#classscy_1_1turn_1_1Request)` & request)` | 
`public virtual void handleRefreshRequest(`[`Request`](#classscy_1_1turn_1_1Request)` & request)` | 
`public virtual void handleCreatePermission(`[`Request`](#classscy_1_1turn_1_1Request)` & request)` | 
`public virtual bool onTimer()` | 
`public virtual std::int64_t timeRemaining() const` | 
`public virtual std::int64_t maxTimeRemaining() const` | 
`public virtual `[`Server`](#classscy_1_1turn_1_1Server)` & server()` | 
`public virtual void print(std::ostream & os) const` | 
`protected uint32_t _maxLifetime` | 
`protected `[`Server`](./doc/api-turn.md#classscy_1_1turn_1_1Server)` & _server` | 
`protected virtual  ~ServerAllocation()` | 

## Members

#### `public  ServerAllocation(`[`Server`](#classscy_1_1turn_1_1Server)` & server,const `[`FiveTuple`](#classscy_1_1turn_1_1FiveTuple)` & tuple,const std::string & username,std::int64_t lifetime)` 





#### `public virtual bool handleRequest(`[`Request`](#classscy_1_1turn_1_1Request)` & request)` 





#### `public virtual void handleRefreshRequest(`[`Request`](#classscy_1_1turn_1_1Request)` & request)` 





#### `public virtual void handleCreatePermission(`[`Request`](#classscy_1_1turn_1_1Request)` & request)` 





#### `public virtual bool onTimer()` 



Asynchronous timer callback for updating the allocation permissions and state etc. If this call returns false the allocation will be deleted.

#### `public virtual std::int64_t timeRemaining() const` 





#### `public virtual std::int64_t maxTimeRemaining() const` 





#### `public virtual `[`Server`](#classscy_1_1turn_1_1Server)` & server()` 





#### `public virtual void print(std::ostream & os) const` 





#### `protected uint32_t _maxLifetime` 





#### `protected `[`Server`](./doc/api-turn.md#classscy_1_1turn_1_1Server)` & _server` 





#### `protected virtual  ~ServerAllocation()` 



IMPORTANT: The destructor should never be called directly as the allocation is deleted via the timer callback. See [onTimer()](#group__turn_1gae360050350229d817fda8d6317531862)

# class `scy::turn::TCPAllocation` 

```
class scy::turn::TCPAllocation
  : public scy::turn::ServerAllocation
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  TCPAllocation(`[`Server`](#classscy_1_1turn_1_1Server)` & server,const net::Socket::Ptr & control,const `[`FiveTuple`](#classscy_1_1turn_1_1FiveTuple)` & tuple,const std::string & username,const uint32_t & lifetime)` | 
`public virtual  ~TCPAllocation()` | 
`public virtual bool handleRequest(`[`Request`](#classscy_1_1turn_1_1Request)` & request)` | 
`public void handleConnectRequest(`[`Request`](#classscy_1_1turn_1_1Request)` & request)` | 
`public void handleConnectionBindRequest(`[`Request`](#classscy_1_1turn_1_1Request)` & request)` | 
`public void sendPeerConnectResponse(`[`TCPConnectionPair`](#classscy_1_1turn_1_1TCPConnectionPair)` * pair,bool success)` | Sends a Connect request response to control.
`public int sendToControl(`[`stun::Message`](#classscy_1_1stun_1_1Message)` & message)` | 
`public `[`net::TCPSocket`](#classscy_1_1net_1_1TCPSocket)` & control()` | 
`public virtual `[`net::Address`](#classscy_1_1net_1_1Address)` relayedAddress() const` | 
`public `[`TCPConnectionPairMap`](#classscy_1_1PointerCollection)` & pairs()` | 
`public virtual bool onTimer()` | 
`public void onPeerAccept(const net::TCPSocket::Ptr & sock)` | Accepts incoming peer sockets for ConnectionBind requests.
`public void onControlClosed(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket)` | 
`protected `[`net::SocketEmitter`](./doc/api-net.md#classscy_1_1net_1_1SocketEmitter)` _control` | 
`protected `[`net::SocketEmitter`](./doc/api-net.md#classscy_1_1net_1_1SocketEmitter)` _acceptor` | 
`protected `[`TCPConnectionPairMap`](./doc/api-base.md#classscy_1_1PointerCollection)` _pairs` | 

## Members

#### `public  TCPAllocation(`[`Server`](#classscy_1_1turn_1_1Server)` & server,const net::Socket::Ptr & control,const `[`FiveTuple`](#classscy_1_1turn_1_1FiveTuple)` & tuple,const std::string & username,const uint32_t & lifetime)` 





#### `public virtual  ~TCPAllocation()` 





#### `public virtual bool handleRequest(`[`Request`](#classscy_1_1turn_1_1Request)` & request)` 





#### `public void handleConnectRequest(`[`Request`](#classscy_1_1turn_1_1Request)` & request)` 





#### `public void handleConnectionBindRequest(`[`Request`](#classscy_1_1turn_1_1Request)` & request)` 





#### `public void sendPeerConnectResponse(`[`TCPConnectionPair`](#classscy_1_1turn_1_1TCPConnectionPair)` * pair,bool success)` 

Sends a Connect request response to control.



#### `public int sendToControl(`[`stun::Message`](#classscy_1_1stun_1_1Message)` & message)` 





#### `public `[`net::TCPSocket`](#classscy_1_1net_1_1TCPSocket)` & control()` 





#### `public virtual `[`net::Address`](#classscy_1_1net_1_1Address)` relayedAddress() const` 





#### `public `[`TCPConnectionPairMap`](#classscy_1_1PointerCollection)` & pairs()` 





#### `public virtual bool onTimer()` 



Asynchronous timer callback for updating the allocation permissions and state etc. If this call returns false the allocation will be deleted.

#### `public void onPeerAccept(const net::TCPSocket::Ptr & sock)` 

Accepts incoming peer sockets for ConnectionBind requests.



#### `public void onControlClosed(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket)` 



Callback for handling controll connection destruction. The allocation will be deleted.

#### `protected `[`net::SocketEmitter`](./doc/api-net.md#classscy_1_1net_1_1SocketEmitter)` _control` 





#### `protected `[`net::SocketEmitter`](./doc/api-net.md#classscy_1_1net_1_1SocketEmitter)` _acceptor` 





#### `protected `[`TCPConnectionPairMap`](./doc/api-base.md#classscy_1_1PointerCollection)` _pairs` 





# class `scy::turn::TCPClient` 

```
class scy::turn::TCPClient
  : public scy::turn::Client
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  TCPClient(`[`TCPClientObserver`](#structscy_1_1turn_1_1TCPClientObserver)` & observer,const `[`Client::Options`](#structscy_1_1turn_1_1Client_1_1Options)` & options)` | 
`public virtual  ~TCPClient()` | 
`public virtual void initiate()` | Initiates the allocation sequence.
`public virtual void shutdown()` | Shutdown the client and destroy the active allocation.
`public virtual void sendConnectRequest(const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
`public virtual void sendData(const char * data,size_t size,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
`public virtual bool handleResponse(const `[`stun::Message`](#classscy_1_1stun_1_1Message)` & response)` | 
`public virtual void handleConnectResponse(const `[`stun::Message`](#classscy_1_1stun_1_1Message)` & response)` | 
`public virtual void handleConnectErrorResponse(const `[`stun::Message`](#classscy_1_1stun_1_1Message)` & response)` | 
`public virtual void handleConnectionBindResponse(const `[`stun::Message`](#classscy_1_1stun_1_1Message)` & response)` | 
`public virtual void handleConnectionBindErrorResponse(const `[`stun::Message`](#classscy_1_1stun_1_1Message)` & response)` | 
`public virtual void handleConnectionAttemptIndication(const `[`stun::Message`](#classscy_1_1stun_1_1Message)` & response)` | 
`public virtual bool createAndBindConnection(uint32_t connectionID,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
`public virtual void onRelayConnectionConnect(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket)` | 
`public virtual void onRelayDataReceived(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket,const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
`public virtual void onRelayConnectionError(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket,const `[`Error`](#structscy_1_1Error)` & error)` | 
`public virtual void onRelayConnectionClosed(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket)` | 
`public void freeConnection(const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
`public virtual int transportProtocol()` | 
`public `[`ConnectionManager`](#classscy_1_1KVCollection)` & connections()` | 
`public inline virtual const char * className() const` | 
`protected `[`TCPClientObserver`](./doc/api-turn.md#structscy_1_1turn_1_1TCPClientObserver)` & _observer` | 
`protected `[`ConnectionManager`](./doc/api-base.md#classscy_1_1KVCollection)` _connections` | 

## Members

#### `public  TCPClient(`[`TCPClientObserver`](#structscy_1_1turn_1_1TCPClientObserver)` & observer,const `[`Client::Options`](#structscy_1_1turn_1_1Client_1_1Options)` & options)` 





#### `public virtual  ~TCPClient()` 





#### `public virtual void initiate()` 

Initiates the allocation sequence.



#### `public virtual void shutdown()` 

Shutdown the client and destroy the active allocation.



#### `public virtual void sendConnectRequest(const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` 





#### `public virtual void sendData(const char * data,size_t size,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` 





#### `public virtual bool handleResponse(const `[`stun::Message`](#classscy_1_1stun_1_1Message)` & response)` 





#### `public virtual void handleConnectResponse(const `[`stun::Message`](#classscy_1_1stun_1_1Message)` & response)` 





#### `public virtual void handleConnectErrorResponse(const `[`stun::Message`](#classscy_1_1stun_1_1Message)` & response)` 





#### `public virtual void handleConnectionBindResponse(const `[`stun::Message`](#classscy_1_1stun_1_1Message)` & response)` 





#### `public virtual void handleConnectionBindErrorResponse(const `[`stun::Message`](#classscy_1_1stun_1_1Message)` & response)` 





#### `public virtual void handleConnectionAttemptIndication(const `[`stun::Message`](#classscy_1_1stun_1_1Message)` & response)` 





#### `public virtual bool createAndBindConnection(uint32_t connectionID,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` 





#### `public virtual void onRelayConnectionConnect(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket)` 





#### `public virtual void onRelayDataReceived(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket,const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` 





#### `public virtual void onRelayConnectionError(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket,const `[`Error`](#structscy_1_1Error)` & error)` 





#### `public virtual void onRelayConnectionClosed(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket)` 





#### `public void freeConnection(const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` 





#### `public virtual int transportProtocol()` 





#### `public `[`ConnectionManager`](#classscy_1_1KVCollection)` & connections()` 





#### `public inline virtual const char * className() const` 





#### `protected `[`TCPClientObserver`](./doc/api-turn.md#structscy_1_1turn_1_1TCPClientObserver)` & _observer` 





#### `protected `[`ConnectionManager`](./doc/api-base.md#classscy_1_1KVCollection)` _connections` 





# class `scy::turn::TCPConnectionPair` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`TCPAllocation`](./doc/api-turn.md#classscy_1_1turn_1_1TCPAllocation)` & allocation` | 
`public `[`net::SocketEmitter`](./doc/api-net.md#classscy_1_1net_1_1SocketEmitter)` client` | The client socket, nullptr to start.
`public `[`net::SocketEmitter`](./doc/api-net.md#classscy_1_1net_1_1SocketEmitter)` peer` | The client socket, nullptr to start.
`public `[`Buffer`](#group__base_1gab10b36b080deb583ed703fca6b63ffdd)` earlyPeerData` | Stores early peer > client data.
`public uint32_t connectionID` | The unique connection ID.
`public bool isDataConnection` | True when p2p relay is flowing.
`public `[`Timeout`](./doc/api-base.md#classscy_1_1Timeout)` timeout` | The ConnectionBind request timeout counter.
`public stun::TransactionID transactionID` | 
`public  TCPConnectionPair(`[`TCPAllocation`](#classscy_1_1turn_1_1TCPAllocation)` & allocation)` | 
`public virtual  ~TCPConnectionPair()` | 
`public bool doPeerConnect(const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddr)` | 
`public bool makeDataConnection()` | 
`public void setPeerSocket(const net::TCPSocket::Ptr & socket)` | 
`public void setClientSocket(const net::TCPSocket::Ptr & socket)` | 
`public void onPeerConnectSuccess(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket)` | Connection success callback for Connect request.
`public void onPeerConnectError(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket,const `[`Error`](#structscy_1_1Error)` & error)` | Connection error callback for Connect request.
`public void onClientDataReceived(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket,const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
`public void onPeerDataReceived(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket,const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
`public void onConnectionClosed(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket)` | 
`public void startTimeout()` | 
`public bool expired() const` | Return true if the peer `ConnectionBind` request timed out.

## Members

#### `public `[`TCPAllocation`](./doc/api-turn.md#classscy_1_1turn_1_1TCPAllocation)` & allocation` 





#### `public `[`net::SocketEmitter`](./doc/api-net.md#classscy_1_1net_1_1SocketEmitter)` client` 

The client socket, nullptr to start.



#### `public `[`net::SocketEmitter`](./doc/api-net.md#classscy_1_1net_1_1SocketEmitter)` peer` 

The client socket, nullptr to start.



#### `public `[`Buffer`](#group__base_1gab10b36b080deb583ed703fca6b63ffdd)` earlyPeerData` 

Stores early peer > client data.



#### `public uint32_t connectionID` 

The unique connection ID.



#### `public bool isDataConnection` 

True when p2p relay is flowing.



#### `public `[`Timeout`](./doc/api-base.md#classscy_1_1Timeout)` timeout` 

The ConnectionBind request timeout counter.



#### `public stun::TransactionID transactionID` 





#### `public  TCPConnectionPair(`[`TCPAllocation`](#classscy_1_1turn_1_1TCPAllocation)` & allocation)` 





#### `public virtual  ~TCPConnectionPair()` 





#### `public bool doPeerConnect(const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddr)` 



Initiates an outgoing TCP connection to the peer for Connect requests.

#### `public bool makeDataConnection()` 



Binds the client <> peer relay pipe once the ConnectionBind request is successfull.

#### `public void setPeerSocket(const net::TCPSocket::Ptr & socket)` 





#### `public void setClientSocket(const net::TCPSocket::Ptr & socket)` 





#### `public void onPeerConnectSuccess(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket)` 

Connection success callback for Connect request.



#### `public void onPeerConnectError(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket,const `[`Error`](#structscy_1_1Error)` & error)` 

Connection error callback for Connect request.



#### `public void onClientDataReceived(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket,const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` 





#### `public void onPeerDataReceived(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket,const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` 





#### `public void onConnectionClosed(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket)` 



Callback for handing either client or peer connections which result in the destruction of the [TCPConnectionPair](#classscy_1_1turn_1_1TCPConnectionPair).

#### `public void startTimeout()` 



Starts the ConnectionBind request timeout.

If no ConnectionBind request associated with this peer data connection is received after 30 seconds, the peer data connection MUST be closed.

#### `public bool expired() const` 

Return true if the peer `ConnectionBind` request timed out.



# class `scy::turn::UDPAllocation` 

```
class scy::turn::UDPAllocation
  : public scy::turn::ServerAllocation
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  UDPAllocation(`[`Server`](#classscy_1_1turn_1_1Server)` & server,const `[`FiveTuple`](#classscy_1_1turn_1_1FiveTuple)` & tuple,const std::string & username,const uint32_t & lifetime)` | 
`public virtual  ~UDPAllocation()` | 
`public void onPeerDataReceived(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket,const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
`public virtual bool handleRequest(`[`Request`](#classscy_1_1turn_1_1Request)` & request)` | 
`public void handleSendIndication(`[`Request`](#classscy_1_1turn_1_1Request)` & request)` | 
`public ssize_t send(const char * data,size_t size,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
`public virtual `[`net::Address`](#classscy_1_1net_1_1Address)` relayedAddress() const` | 

## Members

#### `public  UDPAllocation(`[`Server`](#classscy_1_1turn_1_1Server)` & server,const `[`FiveTuple`](#classscy_1_1turn_1_1FiveTuple)` & tuple,const std::string & username,const uint32_t & lifetime)` 





#### `public virtual  ~UDPAllocation()` 





#### `public void onPeerDataReceived(`[`net::Socket`](#classscy_1_1net_1_1Socket)` & socket,const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` 





#### `public virtual bool handleRequest(`[`Request`](#classscy_1_1turn_1_1Request)` & request)` 





#### `public void handleSendIndication(`[`Request`](#classscy_1_1turn_1_1Request)` & request)` 





#### `public ssize_t send(const char * data,size_t size,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` 





#### `public virtual `[`net::Address`](#classscy_1_1net_1_1Address)` relayedAddress() const` 





# class `scy::turn::UDPClient` 

```
class scy::turn::UDPClient
  : public scy::turn::Client
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  UDPClient(`[`ClientObserver`](#structscy_1_1turn_1_1ClientObserver)` & observer,const `[`Options`](#structscy_1_1turn_1_1Client_1_1Options)` & options)` | 
`public virtual  ~UDPClient()` | 

## Members

#### `public  UDPClient(`[`ClientObserver`](#structscy_1_1turn_1_1ClientObserver)` & observer,const `[`Options`](#structscy_1_1turn_1_1Client_1_1Options)` & options)` 





#### `public virtual  ~UDPClient()` 





# struct `scy::turn::ClientObserver` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public void onClientStateChange(`[`Client`](#classscy_1_1turn_1_1Client)` & client,`[`ClientState`](#structscy_1_1turn_1_1ClientState)` & state,const `[`ClientState`](#structscy_1_1turn_1_1ClientState)` & oldState)` | 
`public void onRelayDataReceived(`[`Client`](#classscy_1_1turn_1_1Client)` & client,const char * data,size_t size,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
`public inline virtual void onAllocationFailed(`[`Client`](#classscy_1_1turn_1_1Client)` & client,int errorCode,const std::string & reason)` | 
`public inline virtual void onAllocationDeleted(`[`Client`](#classscy_1_1turn_1_1Client)` & client,const `[`stun::Transaction`](#classscy_1_1stun_1_1Transaction)` & transaction)` | 
`public inline virtual void onAllocationPermissionsCreated(`[`Client`](#classscy_1_1turn_1_1Client)` & client,const PermissionList & permissions)` | 
`public inline virtual void onTransactionResponse(`[`Client`](#classscy_1_1turn_1_1Client)` & client,const `[`stun::Transaction`](#classscy_1_1stun_1_1Transaction)` & transaction)` | 
`public inline virtual void onTimer(`[`Client`](#classscy_1_1turn_1_1Client)` & client)` | 

## Members

#### `public void onClientStateChange(`[`Client`](#classscy_1_1turn_1_1Client)` & client,`[`ClientState`](#structscy_1_1turn_1_1ClientState)` & state,const `[`ClientState`](#structscy_1_1turn_1_1ClientState)` & oldState)` 





#### `public void onRelayDataReceived(`[`Client`](#classscy_1_1turn_1_1Client)` & client,const char * data,size_t size,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` 





#### `public inline virtual void onAllocationFailed(`[`Client`](#classscy_1_1turn_1_1Client)` & client,int errorCode,const std::string & reason)` 





#### `public inline virtual void onAllocationDeleted(`[`Client`](#classscy_1_1turn_1_1Client)` & client,const `[`stun::Transaction`](#classscy_1_1stun_1_1Transaction)` & transaction)` 





#### `public inline virtual void onAllocationPermissionsCreated(`[`Client`](#classscy_1_1turn_1_1Client)` & client,const PermissionList & permissions)` 





#### `public inline virtual void onTransactionResponse(`[`Client`](#classscy_1_1turn_1_1Client)` & client,const `[`stun::Transaction`](#classscy_1_1stun_1_1Transaction)` & transaction)` 



All received transaction responses will be routed here after local processing so the observer can easily implement extra functionality.

#### `public inline virtual void onTimer(`[`Client`](#classscy_1_1turn_1_1Client)` & client)` 



Fires after the client's internal timer callback. Handy for performing extra async cleanup tasks.

# struct `scy::turn::ClientState` 

```
struct scy::turn::ClientState
  : public scy::State
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual std::string toString() const` | 

## Members

#### `public inline virtual std::string toString() const` 





# struct `scy::turn::Permission` 


TURN permission for a user session.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::string ip` | 
`public `[`Timeout`](./doc/api-base.md#classscy_1_1Timeout)` timeout` | 
`public inline  Permission(const std::string & ip)` | 
`public inline void refresh()` | 
`public inline bool operator==(const std::string & r) const` | 

## Members

#### `public std::string ip` 





#### `public `[`Timeout`](./doc/api-base.md#classscy_1_1Timeout)` timeout` 





#### `public inline  Permission(const std::string & ip)` 





#### `public inline void refresh()` 





#### `public inline bool operator==(const std::string & r) const` 





# struct `scy::turn::RelayConnectionBinding` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public uint32_t connectionID` | 
`public `[`net::Address`](./doc/api-net.md#classscy_1_1net_1_1Address)` peerAddress` | 

## Members

#### `public uint32_t connectionID` 





#### `public `[`net::Address`](./doc/api-net.md#classscy_1_1net_1_1Address)` peerAddress` 





# struct `scy::turn::ServerObserver` 




The [ServerObserver](#structscy_1_1turn_1_1ServerObserver) receives callbacks for and is responsible for managing allocation and bandwidth quotas, authentication methods and authentication.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public void onServerAllocationCreated(`[`Server`](#classscy_1_1turn_1_1Server)` * server,`[`IAllocation`](#classscy_1_1turn_1_1IAllocation)` * alloc)` | 
`public void onServerAllocationRemoved(`[`Server`](#classscy_1_1turn_1_1Server)` * server,`[`IAllocation`](#classscy_1_1turn_1_1IAllocation)` * alloc)` | 
`public AuthenticationState authenticateRequest(`[`Server`](#classscy_1_1turn_1_1Server)` * server,`[`Request`](#classscy_1_1turn_1_1Request)` & request)` | 

## Members

#### `public void onServerAllocationCreated(`[`Server`](#classscy_1_1turn_1_1Server)` * server,`[`IAllocation`](#classscy_1_1turn_1_1IAllocation)` * alloc)` 





#### `public void onServerAllocationRemoved(`[`Server`](#classscy_1_1turn_1_1Server)` * server,`[`IAllocation`](#classscy_1_1turn_1_1IAllocation)` * alloc)` 





#### `public AuthenticationState authenticateRequest(`[`Server`](#classscy_1_1turn_1_1Server)` * server,`[`Request`](#classscy_1_1turn_1_1Request)` & request)` 



The observer class can implement authentication using the long-term credential mechanism of [RFC5389]. The class design is such that authentication can be preformed asynchronously against a remote database, or locally. The default implementation returns true to all requests.

To mitigate either intentional or unintentional denial-of-service attacks against the server by clients with valid usernames and passwords, it is RECOMMENDED that the server impose limits on both the number of allocations active at one time for a given username and on the amount of bandwidth those allocations can use. The server should reject new allocations that would exceed the limit on the allowed number of allocations active at one time with a 486 (Allocation Quota Exceeded) (see Section 6.2), and should discard application data traffic that exceeds the bandwidth quota.

# struct `scy::turn::ServerOptions` 


[Configuration](#classscy_1_1Configuration) options for the TURN server.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::string software` | 
`public std::string realm` | 
`public uint32_t allocationDefaultLifetime` | 
`public uint32_t allocationMaxLifetime` | 
`public int allocationMaxPermissions` | 
`public int timerInterval` | 
`public int earlyMediaBufferSize` | 
`public `[`net::Address`](./doc/api-net.md#classscy_1_1net_1_1Address)` listenAddr` | The TCP and UDP bind() address.
`public std::string externalIP` | The external public facing IP address of the server.
`public bool enableTCP` | 
`public bool enableUDP` | 
`public inline  ServerOptions()` | 

## Members

#### `public std::string software` 





#### `public std::string realm` 





#### `public uint32_t allocationDefaultLifetime` 





#### `public uint32_t allocationMaxLifetime` 





#### `public int allocationMaxPermissions` 





#### `public int timerInterval` 





#### `public int earlyMediaBufferSize` 





#### `public `[`net::Address`](./doc/api-net.md#classscy_1_1net_1_1Address)` listenAddr` 

The TCP and UDP bind() address.



#### `public std::string externalIP` 

The external public facing IP address of the server.



#### `public bool enableTCP` 





#### `public bool enableUDP` 





#### `public inline  ServerOptions()` 





# struct `scy::turn::TCPClientObserver` 

```
struct scy::turn::TCPClientObserver
  : public scy::turn::ClientObserver
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public void onRelayConnectionCreated(`[`TCPClient`](#classscy_1_1turn_1_1TCPClient)` & client,const net::TCPSocket::Ptr & socket,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
`public inline virtual void onRelayConnectionError(`[`TCPClient`](#classscy_1_1turn_1_1TCPClient)` & client,const net::TCPSocket::Ptr & socket,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
`public void onRelayConnectionClosed(`[`TCPClient`](#classscy_1_1turn_1_1TCPClient)` & client,const net::TCPSocket::Ptr & socket,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
`public inline virtual void onRelayConnectionBindingFailed(`[`TCPClient`](#classscy_1_1turn_1_1TCPClient)` & client,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
`public inline virtual bool onPeerConnectionAttempt(`[`TCPClient`](#classscy_1_1turn_1_1TCPClient)` & client,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 

## Members

#### `public void onRelayConnectionCreated(`[`TCPClient`](#classscy_1_1turn_1_1TCPClient)` & client,const net::TCPSocket::Ptr & socket,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` 





#### `public inline virtual void onRelayConnectionError(`[`TCPClient`](#classscy_1_1turn_1_1TCPClient)` & client,const net::TCPSocket::Ptr & socket,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` 





#### `public void onRelayConnectionClosed(`[`TCPClient`](#classscy_1_1turn_1_1TCPClient)` & client,const net::TCPSocket::Ptr & socket,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` 





#### `public inline virtual void onRelayConnectionBindingFailed(`[`TCPClient`](#classscy_1_1turn_1_1TCPClient)` & client,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` 





#### `public inline virtual bool onPeerConnectionAttempt(`[`TCPClient`](#classscy_1_1turn_1_1TCPClient)` & client,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` 





# struct `scy::turn::Client::Options` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::string software` | 
`public std::string username` | 
`public std::string password` | 
`public long timeout` | 
`public std::int64_t lifetime` | 
`public std::int64_t timerInterval` | 
`public `[`net::Address`](./doc/api-net.md#classscy_1_1net_1_1Address)` serverAddr` | 
`public inline  Options()` | 

## Members

#### `public std::string software` 





#### `public std::string username` 





#### `public std::string password` 





#### `public long timeout` 





#### `public std::int64_t lifetime` 





#### `public std::int64_t timerInterval` 





#### `public `[`net::Address`](./doc/api-net.md#classscy_1_1net_1_1Address)` serverAddr` 





#### `public inline  Options()` 





