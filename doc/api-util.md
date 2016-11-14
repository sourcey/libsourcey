# Module <!-- group --> `util`

///

A simple message rate limiter based on the token bucket algorithm.

Provides timed persistent data storage for class instances. TValue must implement the clone() method.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`namespace `[`scy`](#namespacescy)    | 
# namespace `scy` {#namespacescy}



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[`scy::AsyncDiagnostic`](#classscy_1_1AsyncDiagnostic)    | 
`class `[`scy::Base64PacketEncoder`](#classscy_1_1Base64PacketEncoder)    | 
`class `[`scy::BasicUser`](#classscy_1_1BasicUser)    | 
`class `[`scy::DiagnosticManager`](#classscy_1_1DiagnosticManager)    | 
`class `[`scy::IDiagnostic`](#classscy_1_1IDiagnostic)    | 
`class `[`scy::IRegistry`](#classscy_1_1IRegistry)    | 
`class `[`scy::RateLimiter`](#classscy_1_1RateLimiter)    | 
`class `[`scy::StreamManager`](#classscy_1_1StreamManager)    | 
`class `[`scy::TimedManager`](#classscy_1_1TimedManager)    | 
`class `[`scy::UserManager`](#classscy_1_1UserManager)    | 
`struct `[`scy::DiagnosticState`](#structscy_1_1DiagnosticState)    | 
`struct `[`scy::IUser`](#structscy_1_1IUser)    | 
# class `scy::AsyncDiagnostic` {#classscy_1_1AsyncDiagnostic}

```
class scy::AsyncDiagnostic
  : public scy::IDiagnostic
  : public scy::async::Runnable
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual  ~AsyncDiagnostic()` | 
`public inline virtual void check()` | 
`protected `[`Thread`](api-base.md#classscy_1_1Thread)` _thread` | 

## Members

#### `public inline virtual  ~AsyncDiagnostic()` {#group__util_1ga505c905b8a86b311f10b1bc81f3dd3e9}





#### `public inline virtual void check()` {#group__util_1ga5a37929c47fe2098c4e8946e54a55cb5}





#### `protected `[`Thread`](api-base.md#classscy_1_1Thread)` _thread` {#group__util_1ga86a906cd8701f49e2f2065c9e8e3c1f3}





# class `scy::Base64PacketEncoder` {#classscy_1_1Base64PacketEncoder}

```
class scy::Base64PacketEncoder
  : public scy::PacketProcessor
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`PacketSignal`](api-base.md#classscy_1_1SignalBase)` emitter` | 
`public inline  Base64PacketEncoder()` | 
`public inline virtual void process(`[`IPacket`](#classscy_1_1IPacket)` & packet)` | 

## Members

#### `public `[`PacketSignal`](api-base.md#classscy_1_1SignalBase)` emitter` {#group__util_1ga2d3eededaa9fe67484a81fb7a346dc03}





#### `public inline  Base64PacketEncoder()` {#group__util_1ga7a4a4a092ea8e67ba50a511a57acaa0b}





#### `public inline virtual void process(`[`IPacket`](#classscy_1_1IPacket)` & packet)` {#group__util_1ga99b62b13142c23e2354869ef20cf810d}



This method performs processing on the given packet and emits the result.

Note: If packet processing is async (the packet is not in the current thread scope) then packet data must be copied. Copied data can be freed directly aFter the async call to emit() the outgoing packet.

# class `scy::BasicUser` {#classscy_1_1BasicUser}

```
class scy::BasicUser
  : public scy::IUser
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  BasicUser(const std::string & username,const std::string & password)` | 
`public inline virtual std::string username() const` | 
`public inline virtual std::string password() const` | 
`protected std::string _username` | 
`protected std::string _password` | 

## Members

#### `public inline  BasicUser(const std::string & username,const std::string & password)` {#group__util_1ga99d6a8ec8d148263b858607fc12e6915}





#### `public inline virtual std::string username() const` {#group__util_1gaa4d74cffed88430c95be50f4178a407c}





#### `public inline virtual std::string password() const` {#group__util_1gaf85d948cee382cb7a8fd63bce2230906}





#### `protected std::string _username` {#group__util_1ga984dc62d5f29dad4f46adb0a18709614}





#### `protected std::string _password` {#group__util_1ga6049b540334bbb0faa236a6a78646fb0}





# class `scy::DiagnosticManager` {#classscy_1_1DiagnosticManager}

```
class scy::DiagnosticManager
  : public scy::PointerCollection< TKey, TValue, TDeleter >
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`NullSignal`](api-base.md#classscy_1_1NullSignal)` DiagnosticsComplete` | 
`public  DiagnosticManager()` | 
`public virtual  ~DiagnosticManager()` | 
`public bool freeDiagnostic(const std::string & name)` | 
`public bool addDiagnostic(`[`IDiagnostic`](#classscy_1_1IDiagnostic)` * test)` | Adds a [IDiagnostic](api-util.md#classscy_1_1IDiagnostic) test instance.
`public virtual `[`IDiagnostic`](#classscy_1_1IDiagnostic)` * getDiagnostic(const std::string & name)` | 
`public virtual void resetAll()` | 
`public virtual void checkAll()` | 
`public virtual bool allComplete()` | 
`public virtual void onDiagnosticStateChange(void *,`[`DiagnosticState`](#structscy_1_1DiagnosticState)` & state,const `[`DiagnosticState`](#structscy_1_1DiagnosticState)` &)` | 

## Members

#### `public `[`NullSignal`](api-base.md#classscy_1_1NullSignal)` DiagnosticsComplete` {#group__util_1gae4bffe1d36c1087a6456bdc124740968}





#### `public  DiagnosticManager()` {#group__util_1ga1f7e1a71860acf1acf2ad00e658168b4}





#### `public virtual  ~DiagnosticManager()` {#group__util_1ga33969db867a5da02ba66af577888cce1}





#### `public bool freeDiagnostic(const std::string & name)` {#group__util_1gafcfa5828dcb06aa0da5d926026fba952}





#### `public bool addDiagnostic(`[`IDiagnostic`](#classscy_1_1IDiagnostic)` * test)` {#group__util_1ga965c646fe610d64eee38396d477e1527}

Adds a [IDiagnostic](#classscy_1_1IDiagnostic) test instance.



#### `public virtual `[`IDiagnostic`](#classscy_1_1IDiagnostic)` * getDiagnostic(const std::string & name)` {#group__util_1gac6ba64b207ffd86739f287a1efda9e73}



Returns the [IDiagnostic](#classscy_1_1IDiagnostic) instance or throws a NotFoundException exception.

#### `public virtual void resetAll()` {#group__util_1gaf46ebc8e54d7508f44dc5ad49c0e269f}





#### `public virtual void checkAll()` {#group__util_1ga32981bf8dbec2deef3f70427d48c2e54}



Runs all managed [IDiagnostic](#classscy_1_1IDiagnostic) tests. DiagnosticsComplete will be dispatched on completion.

#### `public virtual bool allComplete()` {#group__util_1gad077e09ee488e3639b0188c8667f2477}





#### `public virtual void onDiagnosticStateChange(void *,`[`DiagnosticState`](#structscy_1_1DiagnosticState)` & state,const `[`DiagnosticState`](#structscy_1_1DiagnosticState)` &)` {#group__util_1gae5adc21f9a476721730d164a13c8dcb3}





# class `scy::IDiagnostic` {#classscy_1_1IDiagnostic}

```
class scy::IDiagnostic
  : public scy::Stateful< DiagnosticState >
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::string name` | The name of the diagnostic.
`public std::string description` | The diagnostic description.
`public std::vector< std::string > summary` | 
`public `[`Signal`](api-base.md#classscy_1_1Signal)`< const std::string & > SummaryUpdated` | 
`public  IDiagnostic()` | 
`public virtual  ~IDiagnostic()` | 
`public virtual void check()` | 
`public virtual void reset()` | 
`public virtual bool complete() const` | 
`public virtual bool passed() const` | 
`public virtual bool failed() const` | 
`protected void run()` | Override to implement diagnostic logic.
`protected virtual bool pass()` | 
`protected virtual bool fail()` | 
`protected virtual void addSummary(const std::string & text)` | 

## Members

#### `public std::string name` {#group__util_1gaa69e4ded0fb7d0db9113ab8c318a043f}

The name of the diagnostic.



#### `public std::string description` {#group__util_1ga5a7dca63479b6fb69f62ed3c33919a11}

The diagnostic description.



#### `public std::vector< std::string > summary` {#group__util_1gac24ce2f6e0d721dc04d292d6b99e2beb}



The diagnostic summary, maybe including troubleshooting information on failure.

#### `public `[`Signal`](api-base.md#classscy_1_1Signal)`< const std::string & > SummaryUpdated` {#group__util_1ga0688716571fe932a387cbf0b09911483}



Signals when a new text item is added to the summary.

#### `public  IDiagnostic()` {#group__util_1gac8aca5149fda66a927334e1132c48890}





#### `public virtual  ~IDiagnostic()` {#group__util_1ga4ef2121b9fd45a42fda1b2aee0a9ea2c}





#### `public virtual void check()` {#group__util_1gabd4bf909e78503107e0604f11d0437f6}





#### `public virtual void reset()` {#group__util_1ga4824c0cc08d8b244a5b7a8916e50a136}





#### `public virtual bool complete() const` {#group__util_1ga23369ac4b799996b3b2911e05076e557}





#### `public virtual bool passed() const` {#group__util_1ga5ed635d12653d66026810386a4bfb7a1}





#### `public virtual bool failed() const` {#group__util_1ga31ceeef28f1b085ec2162002deb00d60}





#### `protected void run()` {#group__util_1ga36bfaa138a649addc6f0d89d4df5ea28}

Override to implement diagnostic logic.

The StateChange signal will dispatch diagnostic test results to delegates.

#### `protected virtual bool pass()` {#group__util_1ga673e4cb65e024a8ec9e123061689b112}





#### `protected virtual bool fail()` {#group__util_1ga05af62ad19e7683f04b2a0637ae0baed}





#### `protected virtual void addSummary(const std::string & text)` {#group__util_1ga64ac18ee28d24cec3f898123db0a62c8}





# class `scy::IRegistry` {#classscy_1_1IRegistry}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`Signal`](api-base.md#classscy_1_1Signal)`< const std::string & > TypeRegistered` | 
`public `[`Signal`](api-base.md#classscy_1_1Signal)`< const std::string & > TypeUnregistered` | 
`public inline  IRegistry()` | 
`public inline virtual  ~IRegistry()` | 
`public inline virtual ItemT * createInstance(const std::string & s)` | 
`public template<typename T>`  <br/>`inline void registerType(const std::string & s)` | 
`public inline virtual void unregisterType(const std::string & s)` | 
`public inline TypeMap types() const` | 

## Members

#### `public `[`Signal`](api-base.md#classscy_1_1Signal)`< const std::string & > TypeRegistered` {#group__util_1gacbce18cbab3d0de2deec13695a69ec6e}





#### `public `[`Signal`](api-base.md#classscy_1_1Signal)`< const std::string & > TypeUnregistered` {#group__util_1gab03caae1fb2324bb6520b2c924cffe04}





#### `public inline  IRegistry()` {#group__util_1ga8669f031838a467d3129960caa96e0b5}





#### `public inline virtual  ~IRegistry()` {#group__util_1ga9ce6d06a06e25983e03f7bd5a06716e3}





#### `public inline virtual ItemT * createInstance(const std::string & s)` {#group__util_1ga3892a5aee7e6f674d92ad5ba4b4db267}





#### `public template<typename T>`  <br/>`inline void registerType(const std::string & s)` {#group__util_1gae25f5528b11ce12d628e246eeb7e0e78}





#### `public inline virtual void unregisterType(const std::string & s)` {#group__util_1ga6501164e448376e278823594a9af0d93}





#### `public inline TypeMap types() const` {#group__util_1ga2b6c7be0bba65bcbe8131921459f0d3f}





# class `scy::RateLimiter` {#classscy_1_1RateLimiter}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public double rate` | How many messages.
`public double seconds` | Over how many seconds.
`public double allowance` | Remaining send allowance.
`public clock_t lastCheck` | Last time canSend() was called.
`public inline  RateLimiter(double rate,double seconds)` | 
`public inline bool canSend()` | 

## Members

#### `public double rate` {#group__util_1ga91c35e10dfcf207f17e62d2242708f26}

How many messages.



#### `public double seconds` {#group__util_1gac47e0bd0461efd33ba6d7e480eeb9059}

Over how many seconds.



#### `public double allowance` {#group__util_1ga40eb12dada557ee2fff2ad2e419ec92c}

Remaining send allowance.



#### `public clock_t lastCheck` {#group__util_1ga247fefbc3361b60286b4e431410ff98a}

Last time canSend() was called.



#### `public inline  RateLimiter(double rate,double seconds)` {#group__util_1gaba3e054e11999e0faf0d8be12e31855e}





#### `public inline bool canSend()` {#group__util_1gaa714f604007b21bc6435d3e518d68de4}





# class `scy::StreamManager` {#classscy_1_1StreamManager}

```
class scy::StreamManager
  : public scy::LiveCollection< PacketStream > >
  : public scy::basic::Polymorphic
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  StreamManager(bool freeClosedStreams)` | 
`public virtual  ~StreamManager()` | 
`public virtual bool addStream(`[`PacketStream`](#classscy_1_1PacketStream)` * stream,bool whiny)` | 
`public virtual bool closeStream(const std::string & name,bool whiny)` | 
`public virtual void closeAll()` | 
`public virtual `[`PacketStream`](#classscy_1_1PacketStream)` * getStream(const std::string & name,bool whiny)` | 
`public virtual `[`PacketStream`](#classscy_1_1PacketStream)` * getDafaultStream()` | Returns the first stream in the list, or NULL.
`public virtual Map streams() const` | 
`public virtual void print(std::ostream & os) const` | 
`protected bool _freeClosedStreams` | 
`protected virtual void onAdd(`[`PacketStream`](#classscy_1_1PacketStream)` * task)` | Called after a stream is added.
`protected virtual void onRemove(`[`PacketStream`](#classscy_1_1PacketStream)` * task)` | Called after a stream is removed.
`protected virtual void onStreamStateChange(void * sender,`[`PacketStreamState`](#structscy_1_1PacketStreamState)` & state,const `[`PacketStreamState`](#structscy_1_1PacketStreamState)` &)` | 
`protected inline virtual const char * className() const` | 

## Members

#### `public  StreamManager(bool freeClosedStreams)` {#group__util_1gad5602b2d25265c53656c86f7739ae7d6}





#### `public virtual  ~StreamManager()` {#group__util_1ga0204c0879b0c83ce9cd770ff14327669}





#### `public virtual bool addStream(`[`PacketStream`](#classscy_1_1PacketStream)` * stream,bool whiny)` {#group__util_1gaf630cdcb67fa136f37e4b8ae9f07cf05}





#### `public virtual bool closeStream(const std::string & name,bool whiny)` {#group__util_1ga4d9fe6b1424912270ee3669c093d6ac3}





#### `public virtual void closeAll()` {#group__util_1ga6a82d6d92fa742e14402400791ef12a0}





#### `public virtual `[`PacketStream`](#classscy_1_1PacketStream)` * getStream(const std::string & name,bool whiny)` {#group__util_1ga07cf385a3eb2fa5e1e4a4d236dbc6b3c}





#### `public virtual `[`PacketStream`](#classscy_1_1PacketStream)` * getDafaultStream()` {#group__util_1ga7cc76a987b8398d5c12cb01575bad6f0}

Returns the first stream in the list, or NULL.



#### `public virtual Map streams() const` {#group__util_1ga404d0fd5528693b0cc3effaf1c166392}





#### `public virtual void print(std::ostream & os) const` {#group__util_1gaf885fbf7d31d61dd1545862913c34e94}





#### `protected bool _freeClosedStreams` {#group__util_1gaddbce96aea755c169c60ecf93c378891}





#### `protected virtual void onAdd(`[`PacketStream`](#classscy_1_1PacketStream)` * task)` {#group__util_1ga8c8fa23ce1ede39afa3fbb868b95a9ab}

Called after a stream is added.



#### `protected virtual void onRemove(`[`PacketStream`](#classscy_1_1PacketStream)` * task)` {#group__util_1ga446dc5548a2201f9e6f60a5f487483b0}

Called after a stream is removed.



#### `protected virtual void onStreamStateChange(void * sender,`[`PacketStreamState`](#structscy_1_1PacketStreamState)` & state,const `[`PacketStreamState`](#structscy_1_1PacketStreamState)` &)` {#group__util_1ga7388ae78948b7e42d5ef65470d97022c}





#### `protected inline virtual const char * className() const` {#group__util_1ga40a27adac00840e0da6e344f5c5c05b4}





# class `scy::TimedManager` {#classscy_1_1TimedManager}

```
class scy::TimedManager
  : public scy::PointerCollection< TKey, TValue, TDeleter >
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  TimedManager(uv::Loop * loop)` | 
`public inline virtual  ~TimedManager()` | 
`public inline virtual void add(const TKey & key,TValue * item,long timeout)` | 
`public inline virtual bool expires(const TKey & key,long timeout)` | Update the item expiry timeout.
`public inline virtual bool expires(TValue * item,long timeout)` | Update the item expiry timeout.
`public inline virtual void clear()` | 
`protected mutable `[`Mutex`](api-thread.md#classscy_1_1Mutex)` _tmutex` | 
`protected TimeoutMap _timeouts` | 
`protected `[`Timer`](api-base.md#classscy_1_1Timer)` _timer` | 
`protected inline virtual bool setTimeout(TValue * item,long timeout)` | 
`protected inline virtual void onRemove(const TKey & key,TValue * item)` | 
`protected inline virtual void onTimeout(TValue * item)` | 
`protected inline void onTimerUpdate(void *)` | 

## Members

#### `public inline  TimedManager(uv::Loop * loop)` {#group__util_1gab895d284229d512d9d51193b138b0fe1}





#### `public inline virtual  ~TimedManager()` {#group__util_1gadab34766ebc90333370b198ec3a3e043}





#### `public inline virtual void add(const TKey & key,TValue * item,long timeout)` {#group__util_1ga49aabfeb877f828356eed48b2a494f74}



Add an item which will expire (and be deleted) after the specified timeout value. If the timeout is 0 the item will be stored indefinitely. The [TimedManager](#classscy_1_1TimedManager) assumes ownership of the given pointer.

#### `public inline virtual bool expires(const TKey & key,long timeout)` {#group__util_1ga8c23a5777851e796ff30dce5dc644972}

Update the item expiry timeout.



#### `public inline virtual bool expires(TValue * item,long timeout)` {#group__util_1ga08ed703cbdb67e04b43e4d3036c3f4d7}

Update the item expiry timeout.



#### `public inline virtual void clear()` {#group__util_1ga62616068a8fffa6176be947ee07b2db9}





#### `protected mutable `[`Mutex`](api-thread.md#classscy_1_1Mutex)` _tmutex` {#group__util_1ga4e858af645ee34e91e300b9f83536eeb}





#### `protected TimeoutMap _timeouts` {#group__util_1ga01952e22d328c5b1bd87a023a8bf2fd2}





#### `protected `[`Timer`](api-base.md#classscy_1_1Timer)` _timer` {#group__util_1ga3cb60024293e87939b487cfbeb4095ab}





#### `protected inline virtual bool setTimeout(TValue * item,long timeout)` {#group__util_1gab633d4bbc283611d2ddc0bac68fc759c}





#### `protected inline virtual void onRemove(const TKey & key,TValue * item)` {#group__util_1ga2db40f6ae23ae8456c8eb6ed6b7ca3fc}





#### `protected inline virtual void onTimeout(TValue * item)` {#group__util_1ga66465316a2fe303332a6afded5256895}





#### `protected inline void onTimerUpdate(void *)` {#group__util_1ga57d8c4cc4fd5d21d20d8f27615db3567}





# class `scy::UserManager` {#classscy_1_1UserManager}

```
class scy::UserManager
  : public scy::LiveCollection< std::string, IUser >
```  



> Deprecated: This class contains a list of users that have access on the system.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  UserManager()` | 
`public inline virtual  ~UserManager()` | 
`public inline virtual bool add(`[`IUser`](#structscy_1_1IUser)` * user)` | 

## Members

#### `public inline  UserManager()` {#group__util_1gaa85253bb14e3db42a41d1422c1630828}





#### `public inline virtual  ~UserManager()` {#group__util_1ga4cbc4a22d84e5c80835ef06daacd016a}





#### `public inline virtual bool add(`[`IUser`](#structscy_1_1IUser)` * user)` {#group__util_1ga7885b358f8dad3a9c1bfa26351919eb3}





# struct `scy::DiagnosticState` {#structscy_1_1DiagnosticState}

```
struct scy::DiagnosticState
  : public scy::State
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual std::string str(unsigned int id) const` | 

## Members

#### `public inline virtual std::string str(unsigned int id) const` {#group__util_1ga0ff5f446bcd0e549324821cbd482ff04}





# struct `scy::IUser` {#structscy_1_1IUser}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::string username() const` | 
`public std::string password() const` | 

## Members

#### `public std::string username() const` {#group__util_1gab1897a1d6cd4507762cb93b1a8144c3c}





#### `public std::string password() const` {#group__util_1gad90f6bab88fd34f0a1644c2ec43b5e5f}





