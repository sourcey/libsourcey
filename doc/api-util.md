# Module <!-- group --> `util`

A simple message rate limiter based on the token bucket algorithm.

///

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`namespace `[`scy`](#namespacescy)    | 
# namespace `scy` 



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
# class `scy::AsyncDiagnostic` 

```
class scy::AsyncDiagnostic
  : public scy::IDiagnostic
  : public scy::basic::Runnable
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual  ~AsyncDiagnostic()` | 
`public void run()` | The run method will be called by the asynchronous context.
`public inline virtual void check()` | 
`protected `[`Thread`](./doc/api-base.md#classscy_1_1Thread)` _thread` | 

## Members

#### `public inline virtual  ~AsyncDiagnostic()` 





#### `public void run()` 

The run method will be called by the asynchronous context.



#### `public inline virtual void check()` 





#### `protected `[`Thread`](./doc/api-base.md#classscy_1_1Thread)` _thread` 





# class `scy::Base64PacketEncoder` 

```
class scy::Base64PacketEncoder
  : public scy::PacketProcessor
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`PacketSignal`](#group__base_1ga3ffb5bda6133fac97f3bed2b567b9b6a)` emitter` | 
`public inline  Base64PacketEncoder()` | 
`public inline virtual void process(`[`IPacket`](#classscy_1_1IPacket)` & packet)` | 

## Members

#### `public `[`PacketSignal`](#group__base_1ga3ffb5bda6133fac97f3bed2b567b9b6a)` emitter` 





#### `public inline  Base64PacketEncoder()` 





#### `public inline virtual void process(`[`IPacket`](#classscy_1_1IPacket)` & packet)` 



This method performs processing on the given packet and emits the result.

Note: If packet processing is async (the packet is not in the current thread scope) then packet data must be copied. Copied data can be freed directly aFter the async call to emit() the outgoing packet.

# class `scy::BasicUser` 

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

#### `public inline  BasicUser(const std::string & username,const std::string & password)` 





#### `public inline virtual std::string username() const` 





#### `public inline virtual std::string password() const` 





#### `protected std::string _username` 





#### `protected std::string _password` 





# class `scy::DiagnosticManager` 

```
class scy::DiagnosticManager
  : public scy::PointerCollection< TKey, TValue, TDeleter >
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`NullSignal`](./doc/api-base.md#classscy_1_1Signal)` DiagnosticsComplete` | 
`public  DiagnosticManager()` | 
`public virtual  ~DiagnosticManager()` | 
`public bool freeDiagnostic(const std::string & name)` | 
`public bool addDiagnostic(`[`IDiagnostic`](#classscy_1_1IDiagnostic)` * test)` | Adds a [IDiagnostic](./doc/api-util.md#classscy_1_1IDiagnostic) test instance.
`public virtual `[`IDiagnostic`](#classscy_1_1IDiagnostic)` * getDiagnostic(const std::string & name)` | 
`public virtual void resetAll()` | 
`public virtual void checkAll()` | 
`public virtual bool allComplete()` | 
`public virtual void onDiagnosticStateChange(void *,`[`DiagnosticState`](#structscy_1_1DiagnosticState)` & state,const `[`DiagnosticState`](#structscy_1_1DiagnosticState)` &)` | 

## Members

#### `public `[`NullSignal`](./doc/api-base.md#classscy_1_1Signal)` DiagnosticsComplete` 





#### `public  DiagnosticManager()` 





#### `public virtual  ~DiagnosticManager()` 





#### `public bool freeDiagnostic(const std::string & name)` 





#### `public bool addDiagnostic(`[`IDiagnostic`](#classscy_1_1IDiagnostic)` * test)` 

Adds a [IDiagnostic](#classscy_1_1IDiagnostic) test instance.



#### `public virtual `[`IDiagnostic`](#classscy_1_1IDiagnostic)` * getDiagnostic(const std::string & name)` 



Returns the [IDiagnostic](#classscy_1_1IDiagnostic) instance or throws a NotFoundException exception.

#### `public virtual void resetAll()` 





#### `public virtual void checkAll()` 



Runs all managed [IDiagnostic](#classscy_1_1IDiagnostic) tests. DiagnosticsComplete will be dispatched on completion.

#### `public virtual bool allComplete()` 





#### `public virtual void onDiagnosticStateChange(void *,`[`DiagnosticState`](#structscy_1_1DiagnosticState)` & state,const `[`DiagnosticState`](#structscy_1_1DiagnosticState)` &)` 





# class `scy::IDiagnostic` 

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
`public `[`Signal`](#classscy_1_1Signal)< void(const std::string &)`> SummaryUpdated` | 
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

#### `public std::string name` 

The name of the diagnostic.



#### `public std::string description` 

The diagnostic description.



#### `public std::vector< std::string > summary` 



The diagnostic summary, maybe including troubleshooting information on failure.

#### `public `[`Signal`](#classscy_1_1Signal)< void(const std::string &)`> SummaryUpdated` 



Signals when a new text item is added to the summary.

#### `public  IDiagnostic()` 





#### `public virtual  ~IDiagnostic()` 





#### `public virtual void check()` 





#### `public virtual void reset()` 





#### `public virtual bool complete() const` 





#### `public virtual bool passed() const` 





#### `public virtual bool failed() const` 





#### `protected void run()` 

Override to implement diagnostic logic.

The StateChange signal will dispatch diagnostic test results to delegates.

#### `protected virtual bool pass()` 





#### `protected virtual bool fail()` 





#### `protected virtual void addSummary(const std::string & text)` 





# class `scy::IRegistry` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`Signal`](#classscy_1_1Signal)< void(const std::string &)`> TypeRegistered` | 
`public `[`Signal`](#classscy_1_1Signal)< void(const std::string &)`> TypeUnregistered` | 
`public inline  IRegistry()` | 
`public inline virtual  ~IRegistry()` | 
`public inline virtual ItemT * createInstance(const std::string & s)` | 
`public template<typename T>`  <br/>`inline void registerType(const std::string & s)` | 
`public inline virtual void unregisterType(const std::string & s)` | 
`public inline TypeMap types() const` | 

## Members

#### `public `[`Signal`](#classscy_1_1Signal)< void(const std::string &)`> TypeRegistered` 





#### `public `[`Signal`](#classscy_1_1Signal)< void(const std::string &)`> TypeUnregistered` 





#### `public inline  IRegistry()` 





#### `public inline virtual  ~IRegistry()` 





#### `public inline virtual ItemT * createInstance(const std::string & s)` 





#### `public template<typename T>`  <br/>`inline void registerType(const std::string & s)` 





#### `public inline virtual void unregisterType(const std::string & s)` 





#### `public inline TypeMap types() const` 





# class `scy::RateLimiter` 






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

#### `public double rate` 

How many messages.



#### `public double seconds` 

Over how many seconds.



#### `public double allowance` 

Remaining send allowance.



#### `public clock_t lastCheck` 

Last time canSend() was called.



#### `public inline  RateLimiter(double rate,double seconds)` 





#### `public inline bool canSend()` 





# class `scy::StreamManager` 

```
class scy::StreamManager
  : public scy::LiveCollection< PacketStream > >
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

#### `public  StreamManager(bool freeClosedStreams)` 





#### `public virtual  ~StreamManager()` 





#### `public virtual bool addStream(`[`PacketStream`](#classscy_1_1PacketStream)` * stream,bool whiny)` 





#### `public virtual bool closeStream(const std::string & name,bool whiny)` 





#### `public virtual void closeAll()` 





#### `public virtual `[`PacketStream`](#classscy_1_1PacketStream)` * getStream(const std::string & name,bool whiny)` 





#### `public virtual `[`PacketStream`](#classscy_1_1PacketStream)` * getDafaultStream()` 

Returns the first stream in the list, or NULL.



#### `public virtual Map streams() const` 





#### `public virtual void print(std::ostream & os) const` 





#### `protected bool _freeClosedStreams` 





#### `protected virtual void onAdd(`[`PacketStream`](#classscy_1_1PacketStream)` * task)` 

Called after a stream is added.



#### `protected virtual void onRemove(`[`PacketStream`](#classscy_1_1PacketStream)` * task)` 

Called after a stream is removed.



#### `protected virtual void onStreamStateChange(void * sender,`[`PacketStreamState`](#structscy_1_1PacketStreamState)` & state,const `[`PacketStreamState`](#structscy_1_1PacketStreamState)` &)` 





#### `protected inline virtual const char * className() const` 





# class `scy::TimedManager` 

```
class scy::TimedManager
  : public scy::PointerCollection< TKey, TValue, TDeleter >
```  



Timed pointer manager

Provides timed persistent data storage for class instances. TValue must implement the clone() method.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  TimedManager(uv::Loop * loop)` | 
`public inline virtual  ~TimedManager()` | 
`public inline virtual void add(const TKey & key,TValue * item,long timeout)` | 
`public inline virtual bool expires(const TKey & key,long timeout)` | Update the item expiry timeout.
`public inline virtual bool expires(TValue * item,long timeout)` | Update the item expiry timeout.
`public inline virtual void clear()` | 
`protected mutable std::mutex _tmutex` | 
`protected TimeoutMap _timeouts` | 
`protected `[`Timer`](./doc/api-base.md#classscy_1_1Timer)` _timer` | 
`protected inline virtual bool setTimeout(TValue * item,long timeout)` | 
`protected inline virtual void onRemove(const TKey & key,TValue * item)` | 
`protected inline virtual void onTimeout(TValue * item)` | 
`protected inline void onTimerUpdate()` | 

## Members

#### `public inline  TimedManager(uv::Loop * loop)` 





#### `public inline virtual  ~TimedManager()` 





#### `public inline virtual void add(const TKey & key,TValue * item,long timeout)` 



Add an item which will expire (and be deleted) after the specified timeout value. If the timeout is 0 the item will be stored indefinitely. The [TimedManager](#classscy_1_1TimedManager) assumes ownership of the given pointer.

#### `public inline virtual bool expires(const TKey & key,long timeout)` 

Update the item expiry timeout.



#### `public inline virtual bool expires(TValue * item,long timeout)` 

Update the item expiry timeout.



#### `public inline virtual void clear()` 





#### `protected mutable std::mutex _tmutex` 





#### `protected TimeoutMap _timeouts` 





#### `protected `[`Timer`](./doc/api-base.md#classscy_1_1Timer)` _timer` 





#### `protected inline virtual bool setTimeout(TValue * item,long timeout)` 





#### `protected inline virtual void onRemove(const TKey & key,TValue * item)` 





#### `protected inline virtual void onTimeout(TValue * item)` 





#### `protected inline void onTimerUpdate()` 





# class `scy::UserManager` 

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

#### `public inline  UserManager()` 





#### `public inline virtual  ~UserManager()` 





#### `public inline virtual bool add(`[`IUser`](#structscy_1_1IUser)` * user)` 





# struct `scy::DiagnosticState` 

```
struct scy::DiagnosticState
  : public scy::State
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline std::string str(unsigned int id) const` | 

## Members

#### `public inline std::string str(unsigned int id) const` 





# struct `scy::IUser` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::string username() const` | 
`public std::string password() const` | 

## Members

#### `public std::string username() const` 





#### `public std::string password() const` 





