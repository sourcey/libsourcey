# Module <!-- group --> `turn`

The `turn` module contains TURN (rfc5766) client and server implementations.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`namespace `[`scy::turn`](#namespacescy_1_1turn)    | 
# namespace `scy::turn` {#namespacescy_1_1turn}



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
# class `scy::turn::Client` {#classscy_1_1turn_1_1Client}

```
class scy::turn::Client
  : public scy::Stateful< ClientState >
  : protected scy::turn::IAllocation
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Client(`[`ClientObserver`](#structscy_1_1turn_1_1ClientObserver)` & observer,const `[`Options`](#structscy_1_1turn_1_1Client_1_1Options)` & options)` | 
`public virtual  ~Client()` | 
`public virtual void initiate()` | Initiates the allocation sequence.
`public virtual void shutdown()` | Shutdown the client and destroy the active allocation.
`public virtual void sendAllocate()` | Sends the allocation request.
`public virtual void addPermission(const IPList & peerIPs)` | 
`public virtual void addPermission(const std::string & ip)` | 
`public virtual void sendCreatePermission()` | Sends a CreatePermission request including all hosts.
`public virtual void sendChannelBind(const std::string & peerIP)` | 
`public virtual void sendRefresh()` | 
`public virtual void sendData(const char * data,std::size_t size,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
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
`public virtual void onSocketRecv(void * sender,const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
`public virtual void onSocketConnect(void * sender)` | 
`public virtual void onSocketClose(void * sender)` | 
`public virtual void onTransactionProgress(void * sender,`[`TransactionState`](#structscy_1_1TransactionState)` & state,const `[`TransactionState`](#structscy_1_1TransactionState)` &)` | 
`public virtual void onStateChange(`[`ClientState`](#structscy_1_1turn_1_1ClientState)` & state,const `[`ClientState`](#structscy_1_1turn_1_1ClientState)` & oldState)` | 
`public virtual void onTimer(void *)` | 
`protected `[`ClientObserver`](api-turn.md#structscy_1_1turn_1_1ClientObserver)` & _observer` | 
`protected `[`Options`](api-turn.md#structscy_1_1turn_1_1Client_1_1Options)` _options` | 
`protected net::Socket::Ptr _socket` | 
`protected `[`Timer`](api-base.md#classscy_1_1Timer)` _timer` | 
`protected `[`net::Address`](api-net.md#classscy_1_1net_1_1Address)` _mappedAddress` | 
`protected `[`net::Address`](api-net.md#classscy_1_1net_1_1Address)` _relayedAddress` | 
`protected std::string _realm` | 
`protected std::string _nonce` | 
`protected std::deque< `[`stun::Message`](api-stun.md#classscy_1_1stun_1_1Message)` > _pendingIndications` | A list of queued Send indication packets awaiting server permissions.
`protected std::vector< `[`stun::Transaction`](api-stun.md#classscy_1_1stun_1_1Transaction)` * > _transactions` | A list containing currently active transactions.

## Members

#### `public  Client(`[`ClientObserver`](#structscy_1_1turn_1_1ClientObserver)` & observer,const `[`Options`](#structscy_1_1turn_1_1Client_1_1Options)` & options)` {#group__turn_1gaf0bd17dc752a17429d72778fb3e72d12}





#### `public virtual  ~Client()` {#group__turn_1ga7fc90ccb1863fddf1a63fae16ae9c371}





#### `public virtual void initiate()` {#group__turn_1gadf723a357d7bb4ad4162850e1689d3e3}

Initiates the allocation sequence.



#### `public virtual void shutdown()` {#group__turn_1ga1e0682ce8d32f7c62946ee7793c1181c}

Shutdown the client and destroy the active allocation.



#### `public virtual void sendAllocate()` {#group__turn_1ga7d568f600867e83b78f42f61d24e13ef}

Sends the allocation request.



#### `public virtual void addPermission(const IPList & peerIPs)` {#group__turn_1ga63590b39064f8a8f96adb89eee134d63}





#### `public virtual void addPermission(const std::string & ip)` {#group__turn_1gaba5057ecad2da5a048e67a51efc74d0f}



Peer permissions should be added/created before we kick off the allocation sequence, but may be added later.

#### `public virtual void sendCreatePermission()` {#group__turn_1ga8555bd85d39c6430f2f00f0f684d7628}

Sends a CreatePermission request including all hosts.

A CreatePermission request will be sent as soon as the Allocation is created, and at timer x intervals.

#### `public virtual void sendChannelBind(const std::string & peerIP)` {#group__turn_1ga8901df806b203e670fe925fe13dca81e}





#### `public virtual void sendRefresh()` {#group__turn_1gacd3057049693dd485726f0a14b1e5ee0}





#### `public virtual void sendData(const char * data,std::size_t size,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` {#group__turn_1ga648f0d38b95e4d7553a8c28a609b9ab2}





#### `public virtual bool handleResponse(const `[`stun::Message`](#classscy_1_1stun_1_1Message)` & response)` {#group__turn_1ga0f6ebc4b5830aaa97b69ca66f0999261}





#### `public virtual void handleAllocateResponse(const `[`stun::Message`](#classscy_1_1stun_1_1Message)` & response)` {#group__turn_1ga68bbf8a80eedb0ba14918aec893448a5}





#### `public virtual void handleAllocateErrorResponse(const `[`stun::Message`](#classscy_1_1stun_1_1Message)` & response)` {#group__turn_1ga12709199bdbf16d3c7dc0cd82e3a93ab}





#### `public virtual void handleCreatePermissionResponse(const `[`stun::Message`](#classscy_1_1stun_1_1Message)` & response)` {#group__turn_1gaa22352721ef5518a253a6594877a21eb}





#### `public virtual void handleCreatePermissionErrorResponse(const `[`stun::Message`](#classscy_1_1stun_1_1Message)` & response)` {#group__turn_1gabbd2963a3eb1021634c55abb70d39d63}





#### `public virtual void handleRefreshResponse(const `[`stun::Message`](#classscy_1_1stun_1_1Message)` & response)` {#group__turn_1ga144c651f56df26ffc92879219930e848}





#### `public virtual void handleDataIndication(const `[`stun::Message`](#classscy_1_1stun_1_1Message)` & response)` {#group__turn_1gaadd3916417580152b965c64c32cf73bb}





#### `public virtual int transportProtocol()` {#group__turn_1ga32c28b5280210a450c0e008904a7b6b2}





#### `public virtual `[`stun::Transaction`](#classscy_1_1stun_1_1Transaction)` * createTransaction(const net::Socket::Ptr & socket)` {#group__turn_1ga88dd79f35429039f830cd1fbfd7f3870}





#### `public virtual void authenticateRequest(`[`stun::Message`](#classscy_1_1stun_1_1Message)` & request)` {#group__turn_1ga7a7dac39cdf0e23586d6607ac65b06cb}





#### `public virtual bool sendAuthenticatedTransaction(`[`stun::Transaction`](#classscy_1_1stun_1_1Transaction)` * transaction)` {#group__turn_1ga44237aae8a9ad5ad3cc4cecc19881e53}





#### `public virtual bool removeTransaction(`[`stun::Transaction`](#classscy_1_1stun_1_1Transaction)` * transaction)` {#group__turn_1ga9f51f7e1bcbe90260db6c8e862927482}





#### `public `[`net::Address`](#classscy_1_1net_1_1Address)` mappedAddress() const` {#group__turn_1ga06d529cceb90e17ad9cdac3998cad80e}





#### `public virtual `[`net::Address`](#classscy_1_1net_1_1Address)` relayedAddress() const` {#group__turn_1ga5708eadcbe76c7c9bc3904d5942e07f5}





#### `public bool closed() const` {#group__turn_1gafd23179bb00b8ce9dca4aaa666ba8f97}





#### `public `[`ClientObserver`](#structscy_1_1turn_1_1ClientObserver)` & observer()` {#group__turn_1ga302010e3c53875c5d7dc468dd1113469}





#### `public `[`Options`](#structscy_1_1turn_1_1Client_1_1Options)` & options()` {#group__turn_1ga36514efc351b6cca0222702e137b27c9}





#### `public virtual void onSocketRecv(void * sender,const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` {#group__turn_1ga2d54a105093178fe90c9b8b0a4480380}





#### `public virtual void onSocketConnect(void * sender)` {#group__turn_1ga0a490e4cbc25e657cab472aaf9941a67}





#### `public virtual void onSocketClose(void * sender)` {#group__turn_1ga7abef610a195d068ba673eaae1fb1199}





#### `public virtual void onTransactionProgress(void * sender,`[`TransactionState`](#structscy_1_1TransactionState)` & state,const `[`TransactionState`](#structscy_1_1TransactionState)` &)` {#group__turn_1ga461a2a0c78dbde5294f0260cadf5410c}





#### `public virtual void onStateChange(`[`ClientState`](#structscy_1_1turn_1_1ClientState)` & state,const `[`ClientState`](#structscy_1_1turn_1_1ClientState)` & oldState)` {#group__turn_1ga35a6d685c1ddb38d29e1467278b7bedc}





#### `public virtual void onTimer(void *)` {#group__turn_1gaead5d8d574c5d1a1627119b232e72f4f}





#### `protected `[`ClientObserver`](api-turn.md#structscy_1_1turn_1_1ClientObserver)` & _observer` {#group__turn_1gab2775eacd7fff57764950d54e7383306}





#### `protected `[`Options`](api-turn.md#structscy_1_1turn_1_1Client_1_1Options)` _options` {#group__turn_1ga7baa7e34fb78a93ed627c6a3b36740ba}





#### `protected net::Socket::Ptr _socket` {#group__turn_1ga759bc9d6301faea20156634347bc9ac8}





#### `protected `[`Timer`](api-base.md#classscy_1_1Timer)` _timer` {#group__turn_1ga1704ecb1572601a2f2863ce70cc4fb97}





#### `protected `[`net::Address`](api-net.md#classscy_1_1net_1_1Address)` _mappedAddress` {#group__turn_1ga21a746dd56680daa4e14d19630ba4ecf}





#### `protected `[`net::Address`](api-net.md#classscy_1_1net_1_1Address)` _relayedAddress` {#group__turn_1ga1f3592858503ccb9efc59cad55ce3041}





#### `protected std::string _realm` {#group__turn_1ga819fcd60d15c7c555792ad91e34df15f}





#### `protected std::string _nonce` {#group__turn_1ga59aa3850af49ff0ea281fd6cb2168e14}





#### `protected std::deque< `[`stun::Message`](api-stun.md#classscy_1_1stun_1_1Message)` > _pendingIndications` {#group__turn_1ga269602d5f2c548df6a654ad9e7c17144}

A list of queued Send indication packets awaiting server permissions.



#### `protected std::vector< `[`stun::Transaction`](api-stun.md#classscy_1_1stun_1_1Transaction)` * > _transactions` {#group__turn_1ga9d80bebccf3f7cdf8d34d1246bf6fb02}

A list containing currently active transactions.



# class `scy::turn::FiveTuple` {#classscy_1_1turn_1_1FiveTuple}




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

#### `public  FiveTuple()` {#group__turn_1ga7b1939ff40fd399a6e235149f46c240d}





#### `public  FiveTuple(const `[`net::Address`](#classscy_1_1net_1_1Address)` & remote,const `[`net::Address`](#classscy_1_1net_1_1Address)` & local,net::TransportType transport)` {#group__turn_1ga2bf36ada2e32974976906cbef746713d}





#### `public  FiveTuple(const `[`FiveTuple`](#classscy_1_1turn_1_1FiveTuple)` & r)` {#group__turn_1ga9b05151cdd2cd10f1420c4a1bc60cd39}





#### `public inline const `[`net::Address`](#classscy_1_1net_1_1Address)` & remote() const` {#group__turn_1gae8d6e8e6029e31229dbf145b3cb1fa00}





#### `public inline const `[`net::Address`](#classscy_1_1net_1_1Address)` & local() const` {#group__turn_1ga260b892e8bb788cda22e253a88b6350f}





#### `public inline const net::TransportType & transport() const` {#group__turn_1gaa08337b9efd119d6d5a5eb7bc0449d89}





#### `public inline void remote(const `[`net::Address`](#classscy_1_1net_1_1Address)` & remote)` {#group__turn_1ga4cfdde49f1c93769f29f20486a023de8}





#### `public inline void local(const `[`net::Address`](#classscy_1_1net_1_1Address)` & local)` {#group__turn_1ga6a5743d49cf4a503f4e336a75e6ab1ec}





#### `public inline void transport(const net::TransportType & transport)` {#group__turn_1gaf92193a44ae9ae9eb7e490cc5d4ac889}





#### `public bool operator==(const `[`FiveTuple`](#classscy_1_1turn_1_1FiveTuple)` & r) const` {#group__turn_1ga68c363f7fec160aedbc1e43e842cf7ef}





#### `public bool operator<(const `[`FiveTuple`](#classscy_1_1turn_1_1FiveTuple)` & r) const` {#group__turn_1gaa8935b7a8df4ba5590d54571a244fb94}





#### `public std::string toString() const` {#group__turn_1ga6e72e84bfac22fd82a97d00651d56065}





# class `scy::turn::IAllocation` {#classscy_1_1turn_1_1IAllocation}




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
`protected `[`FiveTuple`](api-turn.md#classscy_1_1turn_1_1FiveTuple)` _tuple` | 
`protected std::string _username` | 
`protected PermissionList _permissions` | 
`protected std::int64_t _lifetime` | 
`protected std::int64_t _bandwidthLimit` | 
`protected std::int64_t _bandwidthUsed` | 
`protected time_t _createdAt` | 
`protected time_t _updatedAt` | 
`protected bool _deleted` | 

## Members

#### `public  IAllocation(const `[`FiveTuple`](#classscy_1_1turn_1_1FiveTuple)` & tuple,const std::string & username,std::int64_t lifetime)` {#group__turn_1ga27228ee7ea9b2d902844cba562482831}





#### `public virtual  ~IAllocation()` {#group__turn_1ga072f2bd3c27a11c556e95ca2f64a1b96}





#### `public virtual void updateUsage(std::int64_t numBytes)` {#group__turn_1gabe3a1c0062b6a83c84afb534016a2518}



Updates the allocation's internal timeout and bandwidth usage each time the allocation is used.

#### `public virtual void setLifetime(std::int64_t lifetime)` {#group__turn_1gac08a8899932878a7f5a0fdcc0b48a071}

Sets the lifetime of the allocation and resets the timeout.



#### `public virtual void setBandwidthLimit(std::int64_t numBytes)` {#group__turn_1ga8e2c86e075882c326d4c9bde49901a07}

Sets the bandwidth limit in bytes for this allocation.



#### `public virtual bool expired() const` {#group__turn_1ga853e61b93b00bf9298f2e6b63f09e175}



Returns true if the allocation is expired ie. is timed out or the bandwidth limit has been reached.

#### `public virtual bool deleted() const` {#group__turn_1ga9a7c145937ae53982675cef2f30bfb94}



Returns true if the allocation's deleted flag is set and or if the allocation has expired.

This signifies that the allocation is ready to be destroyed via async garbage collection. See Server::onTimer() and Client::onTimer()

#### `public virtual std::int64_t bandwidthLimit() const` {#group__turn_1gaa53520d7f1d391551df07362f251e12d}





#### `public virtual std::int64_t bandwidthUsed() const` {#group__turn_1ga83e1e4abe2f5f66d276efde8912f07fe}





#### `public virtual std::int64_t bandwidthRemaining() const` {#group__turn_1ga6fae3daac8a208fe57d3415e17f9efb3}





#### `public virtual std::int64_t timeRemaining() const` {#group__turn_1gae4af5f598634519896c21d05b2ae3dff}





#### `public virtual `[`FiveTuple`](#classscy_1_1turn_1_1FiveTuple)` & tuple()` {#group__turn_1ga1654e4d9fa04276e5f9e4bcf890bf744}





#### `public virtual std::string username() const` {#group__turn_1ga9c6bf9973f8f4f0dda5aa9db792ab7cd}





#### `public virtual std::int64_t lifetime() const` {#group__turn_1ga849c4d7c1ee031980c799b30896ea3d0}





#### `public virtual PermissionList permissions() const` {#group__turn_1gaa8d3ddee83af5feb781b95ef9453e4f1}





#### `public `[`net::Address`](#classscy_1_1net_1_1Address)` relayedAddress() const` {#group__turn_1ga5541d6593c96ed16b5b82d7ed5e77d7d}





#### `public virtual void addPermission(const std::string & ip)` {#group__turn_1ga30fd3724a345cc202bc2c147f2fe0a0f}





#### `public virtual void addPermissions(const IPList & ips)` {#group__turn_1gaa48e79d80a25a5652d6ce7d1a8b20fed}





#### `public virtual void removePermission(const std::string & ip)` {#group__turn_1ga9204e82ce85c16b73ae5ba216f9d12f3}





#### `public virtual void removeAllPermissions()` {#group__turn_1gac156f8c87e140b20655b276d4fb2b99f}





#### `public virtual void removeExpiredPermissions()` {#group__turn_1ga3b88ba0d8ef79bccb52cf4bdba1b6be9}





#### `public virtual bool hasPermission(const std::string & peerIP)` {#group__turn_1ga65e660c87fbe3a3ec5ff449a0b271c34}





#### `public inline virtual void print(std::ostream & os) const` {#group__turn_1ga0bcf2cb6675c36864ac804c12bff53c4}





#### `protected `[`FiveTuple`](api-turn.md#classscy_1_1turn_1_1FiveTuple)` _tuple` {#group__turn_1ga7c8695af0fa8d476fd2b3395c0e7acda}





#### `protected std::string _username` {#group__turn_1gaf6bbdd7265acb0086e3946281bcab790}





#### `protected PermissionList _permissions` {#group__turn_1ga8c38ed137db89cd1bee04ae3a94cf32a}





#### `protected std::int64_t _lifetime` {#group__turn_1ga4dcd6ce15037d29fd6d9b6a9a2ce470c}





#### `protected std::int64_t _bandwidthLimit` {#group__turn_1ga3a9549208c623572daf3e0fad84c17c1}





#### `protected std::int64_t _bandwidthUsed` {#group__turn_1ga3712c06d098b80de261f7476fa7cf445}





#### `protected time_t _createdAt` {#group__turn_1ga70a95a5e99046c782c602708ab305573}





#### `protected time_t _updatedAt` {#group__turn_1ga584485c991cbbc5a060b48b778d62b8e}





#### `protected bool _deleted` {#group__turn_1ga71472ac047f1522bf0bb6ac08f6e5c69}





# class `scy::turn::Request` {#classscy_1_1turn_1_1Request}

```
class scy::turn::Request
  : public scy::stun::Message
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public net::TransportType transport` | 
`public `[`net::Address`](api-net.md#classscy_1_1net_1_1Address)` localAddress` | 
`public `[`net::Address`](api-net.md#classscy_1_1net_1_1Address)` remoteAddress` | 
`public std::string hash` | 
`public inline  Request(const `[`stun::Message`](#classscy_1_1stun_1_1Message)` & message,net::TransportType transport,const `[`net::Address`](#classscy_1_1net_1_1Address)` & localAddress,const `[`net::Address`](#classscy_1_1net_1_1Address)` & remoteAddress)` | 

## Members

#### `public net::TransportType transport` {#group__turn_1gad73229b9a0ed2c9766a35e6464921d83}





#### `public `[`net::Address`](api-net.md#classscy_1_1net_1_1Address)` localAddress` {#group__turn_1gaa5eaa3335646687346a79d16bf44d500}





#### `public `[`net::Address`](api-net.md#classscy_1_1net_1_1Address)` remoteAddress` {#group__turn_1ga063c2acad8fd94c729479e6ad8b3b9f7}





#### `public std::string hash` {#group__turn_1ga220ca9d82c58f5ade3bf21ab77250590}





#### `public inline  Request(const `[`stun::Message`](#classscy_1_1stun_1_1Message)` & message,net::TransportType transport,const `[`net::Address`](#classscy_1_1net_1_1Address)` & localAddress,const `[`net::Address`](#classscy_1_1net_1_1Address)` & remoteAddress)` {#group__turn_1ga9449353e6e8218f409dc6cad8137bad1}





# class `scy::turn::Server` {#classscy_1_1turn_1_1Server}


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
`public `[`TCPAllocation`](#classscy_1_1turn_1_1TCPAllocation)` * getTCPAllocation(const std::uint32_t & connectionID)` | 
`public net::TCPSocket::Ptr getTCPSocket(const `[`net::Address`](#classscy_1_1net_1_1Address)` & remoteAddr)` | 
`public void releaseTCPSocket(`[`net::Socket`](#classscy_1_1net_1_1Socket)` * socket)` | 
`public `[`ServerObserver`](#structscy_1_1turn_1_1ServerObserver)` & observer()` | 
`public `[`ServerOptions`](#structscy_1_1turn_1_1ServerOptions)` & options()` | 
`public `[`net::UDPSocket`](#classscy_1_1net_1_1UDPSocket)` & udpSocket()` | 
`public `[`net::TCPSocket`](#classscy_1_1net_1_1TCPSocket)` & tcpSocket()` | 
`public `[`Timer`](#classscy_1_1Timer)` & timer()` | 
`public void onTCPAcceptConnection(void * sender,const net::TCPSocket::Ptr & sock)` | 
`public void onTCPSocketClosed(void * sender)` | 
`public void onSocketRecv(void * sender,const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
`public void onTimer(void *)` | 

## Members

#### `public  Server(`[`ServerObserver`](#structscy_1_1turn_1_1ServerObserver)` & observer,const `[`ServerOptions`](#structscy_1_1turn_1_1ServerOptions)` & options)` {#group__turn_1ga2a8d83c2d9447d113d890b3850ea57fc}





#### `public virtual  ~Server()` {#group__turn_1ga4abeb288eb73e7bed7699a4ed532564c}





#### `public virtual void start()` {#group__turn_1ga3f604371cc77d7849381505a586242c1}





#### `public virtual void stop()` {#group__turn_1gaf06ef5bbb67fe0d064273adf1bd47a64}





#### `public void handleRequest(`[`Request`](#classscy_1_1turn_1_1Request)` & request,AuthenticationState state)` {#group__turn_1gacb5bac1767407f17246c357bae94d8bc}





#### `public void handleAuthorizedRequest(`[`Request`](#classscy_1_1turn_1_1Request)` & request)` {#group__turn_1gaab030a0ce83ea7ce592ee288c8ad3e45}





#### `public void handleBindingRequest(`[`Request`](#classscy_1_1turn_1_1Request)` & request)` {#group__turn_1gaa5867d6f3c8126b0c1dd8daedf72455e}





#### `public void handleAllocateRequest(`[`Request`](#classscy_1_1turn_1_1Request)` & request)` {#group__turn_1ga3b400ef28f963bbd282b747299762d53}





#### `public void handleConnectionBindRequest(`[`Request`](#classscy_1_1turn_1_1Request)` & request)` {#group__turn_1ga9c45a88e010c2141510fdd7adbbaa331}





#### `public void respond(`[`Request`](#classscy_1_1turn_1_1Request)` & request,`[`stun::Message`](#classscy_1_1stun_1_1Message)` & response)` {#group__turn_1gafcae1c06fb5122cf021d731934c8725b}





#### `public void respondError(`[`Request`](#classscy_1_1turn_1_1Request)` & request,int errorCode,const char * errorDesc)` {#group__turn_1gadf23ef7b1ca2346b6b0ed57a865674aa}





#### `public ServerAllocationMap allocations() const` {#group__turn_1ga9c667d0d807f36e6a4cf6ab8168ea23f}





#### `public void addAllocation(`[`ServerAllocation`](#classscy_1_1turn_1_1ServerAllocation)` * alloc)` {#group__turn_1gade6e8dcd258a2b6d714cfb2f5ec2dc9e}





#### `public void removeAllocation(`[`ServerAllocation`](#classscy_1_1turn_1_1ServerAllocation)` * alloc)` {#group__turn_1gaef7c8d35bd764c75388dce31f63d3eae}





#### `public `[`ServerAllocation`](#classscy_1_1turn_1_1ServerAllocation)` * getAllocation(const `[`FiveTuple`](#classscy_1_1turn_1_1FiveTuple)` & tuple)` {#group__turn_1ga58132f7190c65ecad4f141eef0f3c971}





#### `public `[`TCPAllocation`](#classscy_1_1turn_1_1TCPAllocation)` * getTCPAllocation(const std::uint32_t & connectionID)` {#group__turn_1ga51542220240e9aba80104f8cbca693f5}





#### `public net::TCPSocket::Ptr getTCPSocket(const `[`net::Address`](#classscy_1_1net_1_1Address)` & remoteAddr)` {#group__turn_1ga67b8b43fd81875a89642793685adc25a}





#### `public void releaseTCPSocket(`[`net::Socket`](#classscy_1_1net_1_1Socket)` * socket)` {#group__turn_1ga3216c2d027b781ecda7f86892bdb5e51}





#### `public `[`ServerObserver`](#structscy_1_1turn_1_1ServerObserver)` & observer()` {#group__turn_1gae00e2da8460fa504d5c184a30153c808}





#### `public `[`ServerOptions`](#structscy_1_1turn_1_1ServerOptions)` & options()` {#group__turn_1gac50bc8f6103c1f019a729e1fd8ab600d}





#### `public `[`net::UDPSocket`](#classscy_1_1net_1_1UDPSocket)` & udpSocket()` {#group__turn_1gadc7036d9cd7736d8dddeb01a08344bfa}





#### `public `[`net::TCPSocket`](#classscy_1_1net_1_1TCPSocket)` & tcpSocket()` {#group__turn_1ga780655e9b7a19e7b227904361b88df4b}





#### `public `[`Timer`](#classscy_1_1Timer)` & timer()` {#group__turn_1ga52cc24e000f44454546ddfccdc85b197}





#### `public void onTCPAcceptConnection(void * sender,const net::TCPSocket::Ptr & sock)` {#group__turn_1ga13220d7ec6b46c142a2dc3fd268a12ac}





#### `public void onTCPSocketClosed(void * sender)` {#group__turn_1ga5c1d674d6d9f1e2af501310e8a7070d3}





#### `public void onSocketRecv(void * sender,const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` {#group__turn_1ga252bc8a22834cbe46196822035bedd26}





#### `public void onTimer(void *)` {#group__turn_1gabc9a40fbd2519961cc44968b93418d94}





# class `scy::turn::ServerAllocation` {#classscy_1_1turn_1_1ServerAllocation}

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
`protected std::uint32_t _maxLifetime` | 
`protected `[`Server`](api-turn.md#classscy_1_1turn_1_1Server)` & _server` | 
`protected virtual  ~ServerAllocation()` | 

## Members

#### `public  ServerAllocation(`[`Server`](#classscy_1_1turn_1_1Server)` & server,const `[`FiveTuple`](#classscy_1_1turn_1_1FiveTuple)` & tuple,const std::string & username,std::int64_t lifetime)` {#group__turn_1ga84047d7fa94573c8b4c9e90d7b2ca35e}





#### `public virtual bool handleRequest(`[`Request`](#classscy_1_1turn_1_1Request)` & request)` {#group__turn_1ga196225684548a5af62dbaf4536a40a6c}





#### `public virtual void handleRefreshRequest(`[`Request`](#classscy_1_1turn_1_1Request)` & request)` {#group__turn_1ga067a994e4f4b78f1208d946b526b7782}





#### `public virtual void handleCreatePermission(`[`Request`](#classscy_1_1turn_1_1Request)` & request)` {#group__turn_1ga123f3fce597db5b9303583eb95c830dc}





#### `public virtual bool onTimer()` {#group__turn_1gae360050350229d817fda8d6317531862}



Asynchronous timer callback for updating the allocation permissions and state etc. If this call returns false the allocation will be deleted.

#### `public virtual std::int64_t timeRemaining() const` {#group__turn_1ga86f851d76c345f440db48a3b8cc373ea}





#### `public virtual std::int64_t maxTimeRemaining() const` {#group__turn_1gac32ad9173f59a4ee7fff11ae498a96f6}





#### `public virtual `[`Server`](#classscy_1_1turn_1_1Server)` & server()` {#group__turn_1gac782e622e15da990801af994dbfa7bfa}





#### `public virtual void print(std::ostream & os) const` {#group__turn_1ga7e83857083889f67c5f29a754a34aa24}





#### `protected std::uint32_t _maxLifetime` {#group__turn_1gae73c4cc4970ad8e10355a1811006517f}





#### `protected `[`Server`](api-turn.md#classscy_1_1turn_1_1Server)` & _server` {#group__turn_1ga8c62f3c453e32f9c710ac172ad633aca}





#### `protected virtual  ~ServerAllocation()` {#group__turn_1ga5a72f2a758d972ead206cd52fcc47132}



IMPORTANT: The destructor should never be called directly as the allocation is deleted via the timer callback. See [onTimer()](#group__turn_1gae360050350229d817fda8d6317531862)

# class `scy::turn::TCPAllocation` {#classscy_1_1turn_1_1TCPAllocation}

```
class scy::turn::TCPAllocation
  : public scy::turn::ServerAllocation
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  TCPAllocation(`[`Server`](#classscy_1_1turn_1_1Server)` & server,const net::Socket::Ptr & control,const `[`FiveTuple`](#classscy_1_1turn_1_1FiveTuple)` & tuple,const std::string & username,const std::uint32_t & lifetime)` | 
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
`public void onPeerAccept(void * sender,const net::TCPSocket::Ptr & sock)` | Accepts incoming peer sockets for ConnectionBind requests.
`public void onControlClosed(void * sender)` | 
`protected net::TCPSocket::Ptr _control` | 
`protected net::TCPSocket::Ptr _acceptor` | 
`protected `[`TCPConnectionPairMap`](api-base.md#classscy_1_1PointerCollection)` _pairs` | 

## Members

#### `public  TCPAllocation(`[`Server`](#classscy_1_1turn_1_1Server)` & server,const net::Socket::Ptr & control,const `[`FiveTuple`](#classscy_1_1turn_1_1FiveTuple)` & tuple,const std::string & username,const std::uint32_t & lifetime)` {#group__turn_1ga9a16a49e42860e95a0beb1e82619d3bb}





#### `public virtual  ~TCPAllocation()` {#group__turn_1ga312379a655b3f96bcdb99e79fb689710}





#### `public virtual bool handleRequest(`[`Request`](#classscy_1_1turn_1_1Request)` & request)` {#group__turn_1ga6c454d8d13f6c5a006731cf8e5d9b517}





#### `public void handleConnectRequest(`[`Request`](#classscy_1_1turn_1_1Request)` & request)` {#group__turn_1ga581cc086264aaebb23867da082b69d08}





#### `public void handleConnectionBindRequest(`[`Request`](#classscy_1_1turn_1_1Request)` & request)` {#group__turn_1ga90d0b7dc91442cd9b165f0bbd597130e}





#### `public void sendPeerConnectResponse(`[`TCPConnectionPair`](#classscy_1_1turn_1_1TCPConnectionPair)` * pair,bool success)` {#group__turn_1gadfc50816e7433c9290781b9f906499ca}

Sends a Connect request response to control.



#### `public int sendToControl(`[`stun::Message`](#classscy_1_1stun_1_1Message)` & message)` {#group__turn_1ga938af5d30b8c6f5d61812f8a241a0a0f}





#### `public `[`net::TCPSocket`](#classscy_1_1net_1_1TCPSocket)` & control()` {#group__turn_1ga6b941622915a8607a4cedc7ee171437c}





#### `public virtual `[`net::Address`](#classscy_1_1net_1_1Address)` relayedAddress() const` {#group__turn_1ga7b3153537c0a9fd9453277443dcba25f}





#### `public `[`TCPConnectionPairMap`](#classscy_1_1PointerCollection)` & pairs()` {#group__turn_1ga883345d6de7c95b47330a0b0daaeb19b}





#### `public virtual bool onTimer()` {#group__turn_1ga263e247e447a7eaa989711bbd78a249a}



Asynchronous timer callback for updating the allocation permissions and state etc. If this call returns false the allocation will be deleted.

#### `public void onPeerAccept(void * sender,const net::TCPSocket::Ptr & sock)` {#group__turn_1ga347369134065f120d99727dd78251cf7}

Accepts incoming peer sockets for ConnectionBind requests.



#### `public void onControlClosed(void * sender)` {#group__turn_1gac2b5cc1ae3dc5e8de13f09a50197a61d}



Callback for handling controll connection destruction. The allocation will be deleted.

#### `protected net::TCPSocket::Ptr _control` {#group__turn_1gafc124d7747a829e534dcca617b52008c}





#### `protected net::TCPSocket::Ptr _acceptor` {#group__turn_1ga79913d5e43a6d80afc733ce9419c2216}





#### `protected `[`TCPConnectionPairMap`](api-base.md#classscy_1_1PointerCollection)` _pairs` {#group__turn_1ga865b233b3ce8e50c49c91c9fccc27920}





# class `scy::turn::TCPClient` {#classscy_1_1turn_1_1TCPClient}

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
`public virtual void sendData(const char * data,std::size_t size,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
`public virtual bool handleResponse(const `[`stun::Message`](#classscy_1_1stun_1_1Message)` & response)` | 
`public virtual void handleConnectResponse(const `[`stun::Message`](#classscy_1_1stun_1_1Message)` & response)` | 
`public virtual void handleConnectErrorResponse(const `[`stun::Message`](#classscy_1_1stun_1_1Message)` & response)` | 
`public virtual void handleConnectionBindResponse(const `[`stun::Message`](#classscy_1_1stun_1_1Message)` & response)` | 
`public virtual void handleConnectionBindErrorResponse(const `[`stun::Message`](#classscy_1_1stun_1_1Message)` & response)` | 
`public virtual void handleConnectionAttemptIndication(const `[`stun::Message`](#classscy_1_1stun_1_1Message)` & response)` | 
`public virtual bool createAndBindConnection(std::uint32_t connectionID,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
`public virtual void onRelayConnectionConnect(void * sender)` | 
`public virtual void onRelayDataReceived(void * sender,const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
`public virtual void onRelayConnectionError(void * sender,const `[`Error`](#structscy_1_1Error)` & error)` | 
`public virtual void onRelayConnectionClosed(void * sender)` | 
`public void freeConnection(const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
`public virtual int transportProtocol()` | 
`public `[`ConnectionManager`](#classscy_1_1KVCollection)` & connections()` | 
`public inline virtual const char * className() const` | 
`protected `[`TCPClientObserver`](api-turn.md#structscy_1_1turn_1_1TCPClientObserver)` & _observer` | 
`protected `[`ConnectionManager`](api-base.md#classscy_1_1KVCollection)` _connections` | 

## Members

#### `public  TCPClient(`[`TCPClientObserver`](#structscy_1_1turn_1_1TCPClientObserver)` & observer,const `[`Client::Options`](#structscy_1_1turn_1_1Client_1_1Options)` & options)` {#group__turn_1ga66a06f08c0fec14c755537b176dbe6fd}





#### `public virtual  ~TCPClient()` {#group__turn_1gaa8bf16affc9175efa5ec96b7638cdd75}





#### `public virtual void initiate()` {#group__turn_1ga21a24b1e8e50a72306595dde1e62b8c8}

Initiates the allocation sequence.



#### `public virtual void shutdown()` {#group__turn_1ga10311576b628961147df231abeea2e45}

Shutdown the client and destroy the active allocation.



#### `public virtual void sendConnectRequest(const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` {#group__turn_1gabc681709fb413701323ae55d99ef86c2}





#### `public virtual void sendData(const char * data,std::size_t size,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` {#group__turn_1ga908e8b5aaf0cf58ba95f112ffb2ed36c}





#### `public virtual bool handleResponse(const `[`stun::Message`](#classscy_1_1stun_1_1Message)` & response)` {#group__turn_1ga2d300174a7c09873df9bc36af3a66fe7}





#### `public virtual void handleConnectResponse(const `[`stun::Message`](#classscy_1_1stun_1_1Message)` & response)` {#group__turn_1gaab0f1e53575195633d0e150b7532bbb9}





#### `public virtual void handleConnectErrorResponse(const `[`stun::Message`](#classscy_1_1stun_1_1Message)` & response)` {#group__turn_1gaf5be5aa0c7f2a0a346dcdb5b1af90cb0}





#### `public virtual void handleConnectionBindResponse(const `[`stun::Message`](#classscy_1_1stun_1_1Message)` & response)` {#group__turn_1ga577618f2ce93bc9e8089f84037a48381}





#### `public virtual void handleConnectionBindErrorResponse(const `[`stun::Message`](#classscy_1_1stun_1_1Message)` & response)` {#group__turn_1gab8e9eb2b935c3af5ad0823c7f6e3d693}





#### `public virtual void handleConnectionAttemptIndication(const `[`stun::Message`](#classscy_1_1stun_1_1Message)` & response)` {#group__turn_1ga5cee29ca9f9c7ffc3d6580f5ca504f2f}





#### `public virtual bool createAndBindConnection(std::uint32_t connectionID,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` {#group__turn_1ga78a366200ccd828e0c102e987f7ff2c0}





#### `public virtual void onRelayConnectionConnect(void * sender)` {#group__turn_1ga419cd636742250d2b76b5cd614bc5fa0}





#### `public virtual void onRelayDataReceived(void * sender,const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` {#group__turn_1ga00407070ef0b93367cc25e3145679010}





#### `public virtual void onRelayConnectionError(void * sender,const `[`Error`](#structscy_1_1Error)` & error)` {#group__turn_1ga4e1250cc9283efdb37fa4b3dc2fe5891}





#### `public virtual void onRelayConnectionClosed(void * sender)` {#group__turn_1gaaaf9bf9afd40d724053537763944f8f8}





#### `public void freeConnection(const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` {#group__turn_1ga1b8eac8a9103d924fcac1b6284c0c584}





#### `public virtual int transportProtocol()` {#group__turn_1ga1fa35721f9353c21bf1e7b470c836883}





#### `public `[`ConnectionManager`](#classscy_1_1KVCollection)` & connections()` {#group__turn_1ga8080d44f1441c6315d3cfcbae375b979}





#### `public inline virtual const char * className() const` {#group__turn_1ga147134333aec71b72ce04bc6e1fb7c7a}





#### `protected `[`TCPClientObserver`](api-turn.md#structscy_1_1turn_1_1TCPClientObserver)` & _observer` {#group__turn_1ga07f4b77e0079d379c598ed2dd7fccb1c}





#### `protected `[`ConnectionManager`](api-base.md#classscy_1_1KVCollection)` _connections` {#group__turn_1ga35cdf1b381e1516ae904410722e6bbb1}





# class `scy::turn::TCPConnectionPair` {#classscy_1_1turn_1_1TCPConnectionPair}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`TCPAllocation`](api-turn.md#classscy_1_1turn_1_1TCPAllocation)` & allocation` | 
`public net::TCPSocket::Ptr client` | The client socket, nullptr to start.
`public net::TCPSocket::Ptr peer` | The client socket, nullptr to start.
`public Buffer earlyPeerData` | Stores early peer > client data.
`public std::uint32_t connectionID` | The unique connection ID.
`public bool isDataConnection` | True when p2p relay is flowing.
`public `[`Timeout`](api-base.md#classscy_1_1Timeout)` timeout` | The ConnectionBind request timeout counter.
`public stun::TransactionID transactionID` | 
`public  TCPConnectionPair(`[`TCPAllocation`](#classscy_1_1turn_1_1TCPAllocation)` & allocation)` | 
`public virtual  ~TCPConnectionPair()` | 
`public bool doPeerConnect(const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddr)` | 
`public bool makeDataConnection()` | 
`public void setPeerSocket(const net::TCPSocket::Ptr & socket)` | 
`public void setClientSocket(const net::TCPSocket::Ptr & socket)` | 
`public void onPeerConnectSuccess(void * sender)` | Connection success callback for Connect request.
`public void onPeerConnectError(void * sender,const `[`Error`](#structscy_1_1Error)` & error)` | Connection error callback for Connect request.
`public void onClientDataReceived(void * sender,const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
`public void onPeerDataReceived(void * sender,const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
`public void onConnectionClosed(void * sender)` | 
`public void startTimeout()` | 
`public bool expired() const` | Return true if the peer ConnectionBind request timed out.

## Members

#### `public `[`TCPAllocation`](api-turn.md#classscy_1_1turn_1_1TCPAllocation)` & allocation` {#group__turn_1ga5f8ce0ee36a5bc87f55d76a95d4ffdc6}





#### `public net::TCPSocket::Ptr client` {#group__turn_1gad7cde128f5226bdc2997b7408b3d5233}

The client socket, nullptr to start.



#### `public net::TCPSocket::Ptr peer` {#group__turn_1ga15f2914e072f15557542f8d169184abd}

The client socket, nullptr to start.



#### `public Buffer earlyPeerData` {#group__turn_1ga7d517bff868ad4aeb1a191abb6d440df}

Stores early peer > client data.



#### `public std::uint32_t connectionID` {#group__turn_1gaa37b31fa563a9a066a919e7db47c5a7f}

The unique connection ID.



#### `public bool isDataConnection` {#group__turn_1gaa0975b408c5100d385ab97ab54492441}

True when p2p relay is flowing.



#### `public `[`Timeout`](api-base.md#classscy_1_1Timeout)` timeout` {#group__turn_1gad2382d7f6eabb79e3ddd8b5ae7fe17f1}

The ConnectionBind request timeout counter.



#### `public stun::TransactionID transactionID` {#group__turn_1gaa13970f4c75250ead1dccdfb573c3af5}





#### `public  TCPConnectionPair(`[`TCPAllocation`](#classscy_1_1turn_1_1TCPAllocation)` & allocation)` {#group__turn_1gaef6a16730e04ad6935b74d7688d434ff}





#### `public virtual  ~TCPConnectionPair()` {#group__turn_1ga624ca19946f081cf19f75b0b185f88bd}





#### `public bool doPeerConnect(const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddr)` {#group__turn_1ga5fa2395970db2f91d0c39b2344293b4a}



Initiates an outgoing TCP connection to the peer for Connect requests.

#### `public bool makeDataConnection()` {#group__turn_1ga9e9fa7894dcb1ba7aead4a9ce89ad2a8}



Binds the client <> peer relay pipe once the ConnectionBind request is successfull.

#### `public void setPeerSocket(const net::TCPSocket::Ptr & socket)` {#group__turn_1gabb87674d00d60b47ec5799578c4303e8}





#### `public void setClientSocket(const net::TCPSocket::Ptr & socket)` {#group__turn_1gabe1aec80b35fe3f161fc38515f139b00}





#### `public void onPeerConnectSuccess(void * sender)` {#group__turn_1ga5a895c0a30cb5553cba6ed6acfdd20f0}

Connection success callback for Connect request.



#### `public void onPeerConnectError(void * sender,const `[`Error`](#structscy_1_1Error)` & error)` {#group__turn_1gaaab8def0bb4ba0a4c2e722f20612c13c}

Connection error callback for Connect request.



#### `public void onClientDataReceived(void * sender,const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` {#group__turn_1gae62cc0ddedaa29da681be6c53410e8c7}





#### `public void onPeerDataReceived(void * sender,const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` {#group__turn_1gaa9668c33061734e3cb9eac4bf06bf06f}





#### `public void onConnectionClosed(void * sender)` {#group__turn_1ga096266ce03908f4ff33a5139ccaf929e}



Callback for handing either client or peer connections which result in the destruction of the [TCPConnectionPair](#classscy_1_1turn_1_1TCPConnectionPair).

#### `public void startTimeout()` {#group__turn_1ga36352c6ddc19d8bdfef9968df7dcc165}



Starts the ConnectionBind request timeout.

If no ConnectionBind request associated with this peer data connection is received after 30 seconds, the peer data connection MUST be closed.

#### `public bool expired() const` {#group__turn_1gae31259009aac3c03ea440ec0a4bb8ff7}

Return true if the peer ConnectionBind request timed out.



# class `scy::turn::UDPAllocation` {#classscy_1_1turn_1_1UDPAllocation}

```
class scy::turn::UDPAllocation
  : public scy::turn::ServerAllocation
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  UDPAllocation(`[`Server`](#classscy_1_1turn_1_1Server)` & server,const `[`FiveTuple`](#classscy_1_1turn_1_1FiveTuple)` & tuple,const std::string & username,const std::uint32_t & lifetime)` | 
`public virtual  ~UDPAllocation()` | 
`public void onPeerDataReceived(void *,const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
`public virtual bool handleRequest(`[`Request`](#classscy_1_1turn_1_1Request)` & request)` | 
`public void handleSendIndication(`[`Request`](#classscy_1_1turn_1_1Request)` & request)` | 
`public int send(const char * data,std::size_t size,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
`public virtual `[`net::Address`](#classscy_1_1net_1_1Address)` relayedAddress() const` | 

## Members

#### `public  UDPAllocation(`[`Server`](#classscy_1_1turn_1_1Server)` & server,const `[`FiveTuple`](#classscy_1_1turn_1_1FiveTuple)` & tuple,const std::string & username,const std::uint32_t & lifetime)` {#group__turn_1ga76b5f52e7fda8cba6f99b878dfbc35aa}





#### `public virtual  ~UDPAllocation()` {#group__turn_1gacc331eb4c29175167d482de8fd682b2b}





#### `public void onPeerDataReceived(void *,const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & buffer,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` {#group__turn_1ga93fb7d907b32e74aac823857d9d47ce5}





#### `public virtual bool handleRequest(`[`Request`](#classscy_1_1turn_1_1Request)` & request)` {#group__turn_1gaadfa6b2dd671c0b9de943c687b4e507b}





#### `public void handleSendIndication(`[`Request`](#classscy_1_1turn_1_1Request)` & request)` {#group__turn_1ga96ad3bfc1e1c63895c320c92f14462ee}





#### `public int send(const char * data,std::size_t size,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` {#group__turn_1ga3855e22343debba0eddbe53aca4d29b8}





#### `public virtual `[`net::Address`](#classscy_1_1net_1_1Address)` relayedAddress() const` {#group__turn_1ga14647342910901c835819da6c0dfd7bc}





# class `scy::turn::UDPClient` {#classscy_1_1turn_1_1UDPClient}

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

#### `public  UDPClient(`[`ClientObserver`](#structscy_1_1turn_1_1ClientObserver)` & observer,const `[`Options`](#structscy_1_1turn_1_1Client_1_1Options)` & options)` {#group__turn_1gac81111283b7df471a1afe3409587ae20}





#### `public virtual  ~UDPClient()` {#group__turn_1gac46009fd65b1194565e6f5b2a25ed3d5}





# struct `scy::turn::ClientObserver` {#structscy_1_1turn_1_1ClientObserver}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public void onClientStateChange(`[`Client`](#classscy_1_1turn_1_1Client)` & client,`[`ClientState`](#structscy_1_1turn_1_1ClientState)` & state,const `[`ClientState`](#structscy_1_1turn_1_1ClientState)` & oldState)` | 
`public void onRelayDataReceived(`[`Client`](#classscy_1_1turn_1_1Client)` & client,const char * data,std::size_t size,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
`public inline virtual void onAllocationCreated(`[`Client`](#classscy_1_1turn_1_1Client)` & client,const `[`stun::Transaction`](#classscy_1_1stun_1_1Transaction)` & transaction)` | 
`public inline virtual void onAllocationFailed(`[`Client`](#classscy_1_1turn_1_1Client)` & client,int errorCode,const std::string & reason)` | 
`public inline virtual void onAllocationDeleted(`[`Client`](#classscy_1_1turn_1_1Client)` & client,const `[`stun::Transaction`](#classscy_1_1stun_1_1Transaction)` & transaction)` | 
`public inline virtual void onAllocationPermissionsCreated(`[`Client`](#classscy_1_1turn_1_1Client)` & client,const PermissionList & permissions)` | 
`public inline virtual void onTransactionResponse(`[`Client`](#classscy_1_1turn_1_1Client)` & client,const `[`stun::Transaction`](#classscy_1_1stun_1_1Transaction)` & transaction)` | 
`public inline virtual void onTimer(`[`Client`](#classscy_1_1turn_1_1Client)` & client)` | 

## Members

#### `public void onClientStateChange(`[`Client`](#classscy_1_1turn_1_1Client)` & client,`[`ClientState`](#structscy_1_1turn_1_1ClientState)` & state,const `[`ClientState`](#structscy_1_1turn_1_1ClientState)` & oldState)` {#group__turn_1ga2f8c99aadad178228ca800c56c66a53a}





#### `public void onRelayDataReceived(`[`Client`](#classscy_1_1turn_1_1Client)` & client,const char * data,std::size_t size,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` {#group__turn_1gaa41a2a0f96d8e0f4004c4c21cc454d30}





#### `public inline virtual void onAllocationCreated(`[`Client`](#classscy_1_1turn_1_1Client)` & client,const `[`stun::Transaction`](#classscy_1_1stun_1_1Transaction)` & transaction)` {#group__turn_1ga5e5286ad142553a2f683178435958b50}





#### `public inline virtual void onAllocationFailed(`[`Client`](#classscy_1_1turn_1_1Client)` & client,int errorCode,const std::string & reason)` {#group__turn_1gaf843439ada68af5d0a8c44b9e6f84cef}





#### `public inline virtual void onAllocationDeleted(`[`Client`](#classscy_1_1turn_1_1Client)` & client,const `[`stun::Transaction`](#classscy_1_1stun_1_1Transaction)` & transaction)` {#group__turn_1ga2fa9e57662dbf68959503085fa781d02}





#### `public inline virtual void onAllocationPermissionsCreated(`[`Client`](#classscy_1_1turn_1_1Client)` & client,const PermissionList & permissions)` {#group__turn_1ga26b4ea2e7a7f1a728218bda75b670c89}





#### `public inline virtual void onTransactionResponse(`[`Client`](#classscy_1_1turn_1_1Client)` & client,const `[`stun::Transaction`](#classscy_1_1stun_1_1Transaction)` & transaction)` {#group__turn_1ga0b96125a01852fdcd24ecdc7e9eba5af}



All received transaction responses will be routed here after local processing so the observer can easily implement extra functionality.

#### `public inline virtual void onTimer(`[`Client`](#classscy_1_1turn_1_1Client)` & client)` {#group__turn_1gac3b5b3eae27058463dc291d16883bbb6}



Fires after the client's internal timer callback. Handy for performing extra async cleanup tasks.

# struct `scy::turn::ClientState` {#structscy_1_1turn_1_1ClientState}

```
struct scy::turn::ClientState
  : public scy::State
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual std::string toString() const` | 

## Members

#### `public inline virtual std::string toString() const` {#group__turn_1ga64488300060951e76776bd292ee76701}





# struct `scy::turn::Permission` {#structscy_1_1turn_1_1Permission}


TURN permission for a user session.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::string ip` | 
`public `[`Timeout`](api-base.md#classscy_1_1Timeout)` timeout` | 
`public inline  Permission(const std::string & ip)` | 
`public inline void refresh()` | 
`public inline bool operator==(const std::string & r) const` | 

## Members

#### `public std::string ip` {#group__turn_1gac3d4b7db7da99f5df332df065c3bbeba}





#### `public `[`Timeout`](api-base.md#classscy_1_1Timeout)` timeout` {#group__turn_1ga7f9706e123e06bacdfc229e9f2d4b503}





#### `public inline  Permission(const std::string & ip)` {#group__turn_1ga1d84a2f95323c85990c9464c4a9d3256}





#### `public inline void refresh()` {#group__turn_1ga0b8b830b544ab68a84f3214563347080}





#### `public inline bool operator==(const std::string & r) const` {#group__turn_1gabf7857f22a589a833b5aa79391b57830}





# struct `scy::turn::RelayConnectionBinding` {#structscy_1_1turn_1_1RelayConnectionBinding}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::uint32_t connectionID` | 
`public `[`net::Address`](api-net.md#classscy_1_1net_1_1Address)` peerAddress` | 

## Members

#### `public std::uint32_t connectionID` {#group__turn_1gab19835c2c00cfdcac36e4c1b45ecd7a4}





#### `public `[`net::Address`](api-net.md#classscy_1_1net_1_1Address)` peerAddress` {#group__turn_1gab40590fa9feb9154fbd96678304cf5ce}





# struct `scy::turn::ServerObserver` {#structscy_1_1turn_1_1ServerObserver}




The [ServerObserver](#structscy_1_1turn_1_1ServerObserver) receives callbacks for and is responsible for managing allocation and bandwidth quotas, authentication methods and authentication.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public void onServerAllocationCreated(`[`Server`](#classscy_1_1turn_1_1Server)` * server,`[`IAllocation`](#classscy_1_1turn_1_1IAllocation)` * alloc)` | 
`public void onServerAllocationRemoved(`[`Server`](#classscy_1_1turn_1_1Server)` * server,`[`IAllocation`](#classscy_1_1turn_1_1IAllocation)` * alloc)` | 
`public AuthenticationState authenticateRequest(`[`Server`](#classscy_1_1turn_1_1Server)` * server,`[`Request`](#classscy_1_1turn_1_1Request)` & request)` | 

## Members

#### `public void onServerAllocationCreated(`[`Server`](#classscy_1_1turn_1_1Server)` * server,`[`IAllocation`](#classscy_1_1turn_1_1IAllocation)` * alloc)` {#group__turn_1gae40bc9446a48541dcbd436ed8fb6e1ff}





#### `public void onServerAllocationRemoved(`[`Server`](#classscy_1_1turn_1_1Server)` * server,`[`IAllocation`](#classscy_1_1turn_1_1IAllocation)` * alloc)` {#group__turn_1ga080fc643212385979ab6b66210bc15c1}





#### `public AuthenticationState authenticateRequest(`[`Server`](#classscy_1_1turn_1_1Server)` * server,`[`Request`](#classscy_1_1turn_1_1Request)` & request)` {#group__turn_1ga36ad126902d380f3b569e6a7c70837a1}



The observer class can implement authentication using the long-term credential mechanism of [RFC5389]. The class design is such that authentication can be preformed asynchronously against a remote database, or locally. The default implementation returns true to all requests.

To mitigate either intentional or unintentional denial-of-service attacks against the server by clients with valid usernames and passwords, it is RECOMMENDED that the server impose limits on both the number of allocations active at one time for a given username and on the amount of bandwidth those allocations can use. The server should reject new allocations that would exceed the limit on the allowed number of allocations active at one time with a 486 (Allocation Quota Exceeded) (see Section 6.2), and should discard application data traffic that exceeds the bandwidth quota.

# struct `scy::turn::ServerOptions` {#structscy_1_1turn_1_1ServerOptions}


[Configuration](#classscy_1_1Configuration) options for the TURN server.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::string software` | 
`public std::string realm` | 
`public std::uint32_t allocationDefaultLifetime` | 
`public std::uint32_t allocationMaxLifetime` | 
`public int allocationMaxPermissions` | 
`public int timerInterval` | 
`public int earlyMediaBufferSize` | 
`public `[`net::Address`](api-net.md#classscy_1_1net_1_1Address)` listenAddr` | The TCP and UDP bind() address.
`public std::string externalIP` | The external public facing IP address of the server.
`public bool enableTCP` | 
`public bool enableUDP` | 
`public inline  ServerOptions()` | 

## Members

#### `public std::string software` {#group__turn_1gac2291c280fe0980304bca18ab44f8731}





#### `public std::string realm` {#group__turn_1gaabb449fc0c3e896938868e9eba870ba4}





#### `public std::uint32_t allocationDefaultLifetime` {#group__turn_1ga168f2f362d7806d89da57251a7a674a5}





#### `public std::uint32_t allocationMaxLifetime` {#group__turn_1gad61aaa376c582dc8fc7e83de0a32b7ee}





#### `public int allocationMaxPermissions` {#group__turn_1gab877b8fafddc71ed3de320159ff614e5}





#### `public int timerInterval` {#group__turn_1ga30c595ee93bb3dfee3cd52faff6a4222}





#### `public int earlyMediaBufferSize` {#group__turn_1ga9122325d7162eb5b1f23888d63cd282d}





#### `public `[`net::Address`](api-net.md#classscy_1_1net_1_1Address)` listenAddr` {#group__turn_1gabd4aa0f66ce2fc218e80810f3c762d76}

The TCP and UDP bind() address.



#### `public std::string externalIP` {#group__turn_1ga88891ec27dede5dc6be7219ed26c9b2a}

The external public facing IP address of the server.



#### `public bool enableTCP` {#group__turn_1ga1d0c462bf8701333710f2f1d4eb0f18a}





#### `public bool enableUDP` {#group__turn_1gaf34f6a53ee1dd805c81af5556deecdb9}





#### `public inline  ServerOptions()` {#group__turn_1ga23b935569a27d3f3dcf0f78e8a5ca280}





# struct `scy::turn::TCPClientObserver` {#structscy_1_1turn_1_1TCPClientObserver}

```
struct scy::turn::TCPClientObserver
  : public scy::turn::ClientObserver
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual void onRelayConnectionCreated(`[`TCPClient`](#classscy_1_1turn_1_1TCPClient)` & client,const net::TCPSocket::Ptr & socket,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
`public inline virtual void onRelayConnectionError(`[`TCPClient`](#classscy_1_1turn_1_1TCPClient)` & client,const net::TCPSocket::Ptr & socket,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
`public inline virtual void onRelayConnectionClosed(`[`TCPClient`](#classscy_1_1turn_1_1TCPClient)` & client,const net::TCPSocket::Ptr & socket,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
`public inline virtual void onRelayConnectionBindingFailed(`[`TCPClient`](#classscy_1_1turn_1_1TCPClient)` & client,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 
`public inline virtual bool onPeerConnectionAttempt(`[`TCPClient`](#classscy_1_1turn_1_1TCPClient)` & client,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` | 

## Members

#### `public inline virtual void onRelayConnectionCreated(`[`TCPClient`](#classscy_1_1turn_1_1TCPClient)` & client,const net::TCPSocket::Ptr & socket,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` {#group__turn_1ga9b56050fd07b6ea19d811f3f7bf14e6a}





#### `public inline virtual void onRelayConnectionError(`[`TCPClient`](#classscy_1_1turn_1_1TCPClient)` & client,const net::TCPSocket::Ptr & socket,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` {#group__turn_1ga37d33d68190c4578c19b0bf376ad8f63}





#### `public inline virtual void onRelayConnectionClosed(`[`TCPClient`](#classscy_1_1turn_1_1TCPClient)` & client,const net::TCPSocket::Ptr & socket,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` {#group__turn_1ga2406cdb7cdb2c10fa0ee3737ef2d1eec}





#### `public inline virtual void onRelayConnectionBindingFailed(`[`TCPClient`](#classscy_1_1turn_1_1TCPClient)` & client,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` {#group__turn_1ga31a9a774ee2e1b13ca87e108bf6cf76e}





#### `public inline virtual bool onPeerConnectionAttempt(`[`TCPClient`](#classscy_1_1turn_1_1TCPClient)` & client,const `[`net::Address`](#classscy_1_1net_1_1Address)` & peerAddress)` {#group__turn_1gad6938fba6074bc0e1ca499cf90bb84ca}





