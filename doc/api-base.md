# Module <!-- group --> `base`

The `base` module contains reusable cross platform tools and utilities.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`namespace `[`scy`](#namespacescy)    | 
`namespace `[`scy::async`](#namespacescy_1_1async)    | Classes for asynchronous programming.
`namespace `[`scy::base64`](#namespacescy_1_1base64)    | 
`namespace `[`scy::hex`](#namespacescy_1_1hex)    | 
`namespace `[`scy::basic`](#namespacescy_1_1basic)    | 
`namespace `[`scy::ipc`](#namespacescy_1_1ipc)    | Classes for inter-process communication.
`namespace `[`scy::deleter`](#namespacescy_1_1deleter)    | Deleter Functors.
`namespace `[`scy::test`](#namespacescy_1_1test)    | Modern unit testing classes.
`namespace `[`scy::util`](#namespacescy_1_1util)    | 
# namespace `scy` {#namespacescy}



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[`scy::AbstractCollection`](#classscy_1_1AbstractCollection)    | 
`class `[`scy::Application`](#classscy_1_1Application)    | 
`class `[`scy::AsyncLogWriter`](#classscy_1_1AsyncLogWriter)    | 
`class `[`scy::AsyncPacketQueue`](#classscy_1_1AsyncPacketQueue)    | 
`class `[`scy::AsyncQueue`](#classscy_1_1AsyncQueue)    | 
`class `[`scy::AsyncStartable`](#classscy_1_1AsyncStartable)    | 
`class `[`scy::BitReader`](#classscy_1_1BitReader)    | A [BitReader](#classscy_1_1BitReader) for reading binary streams.
`class `[`scy::BitWriter`](#classscy_1_1BitWriter)    | 
`class `[`scy::Configuration`](#classscy_1_1Configuration)    | 
`class `[`scy::ConsoleChannel`](#classscy_1_1ConsoleChannel)    | 
`class `[`scy::ConstBuffer`](#classscy_1_1ConstBuffer)    | 
`class `[`scy::DateTime`](#classscy_1_1DateTime)    | 
`class `[`scy::DateTimeFormat`](#classscy_1_1DateTimeFormat)    | 
`class `[`scy::DateTimeFormatter`](#classscy_1_1DateTimeFormatter)    | 
`class `[`scy::DateTimeParser`](#classscy_1_1DateTimeParser)    | 
`class `[`scy::Delegate`](#classscy_1_1Delegate)    | 
`class `[`scy::FileChannel`](#classscy_1_1FileChannel)    | 
`class `[`scy::FlagPacket`](#classscy_1_1FlagPacket)    | A simple flag packet for sending state flags along the packet stream.
`class `[`scy::GarbageCollector`](#classscy_1_1GarbageCollector)    | Simple garbage collector for deferred pointer deletion.
`class `[`scy::Idler`](#classscy_1_1Idler)    | 
`class `[`scy::IPacket`](#classscy_1_1IPacket)    | 
`class `[`scy::IPacketCreationStrategy`](#classscy_1_1IPacketCreationStrategy)    | 
`class `[`scy::KVCollection`](#classscy_1_1KVCollection)    | A reusable stack based unique key-value store for DRY coding.
`class `[`scy::LiveCollection`](#classscy_1_1LiveCollection)    | 
`class `[`scy::LocalDateTime`](#classscy_1_1LocalDateTime)    | 
`class `[`scy::LogChannel`](#classscy_1_1LogChannel)    | 
`class `[`scy::Logger`](#classscy_1_1Logger)    | 
`class `[`scy::LogWriter`](#classscy_1_1LogWriter)    | 
`class `[`scy::MutableBuffer`](#classscy_1_1MutableBuffer)    | 
`class `[`scy::MutexState`](#classscy_1_1MutexState)    | 
`class `[`scy::NullSignal`](#classscy_1_1NullSignal)    | 
`class `[`scy::NVCollection`](#classscy_1_1NVCollection)    | 
`class `[`scy::PacketFactory`](#classscy_1_1PacketFactory)    | 
`class `[`scy::PacketProcessor`](#classscy_1_1PacketProcessor)    | For 0.8.x compatibility.
`class `[`scy::PacketStream`](#classscy_1_1PacketStream)    | 
`class `[`scy::PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)    | 
`class `[`scy::PacketTransaction`](#classscy_1_1PacketTransaction)    | 
`class `[`scy::PointerCollection`](#classscy_1_1PointerCollection)    | 
`class `[`scy::Process`](#classscy_1_1Process)    | 
`class `[`scy::Queue`](#classscy_1_1Queue)    | Implements a thread-safe queue container.
`class `[`scy::Random`](#classscy_1_1Random)    | 
`class `[`scy::RawPacket`](#classscy_1_1RawPacket)    | 
`class `[`scy::RotatingFileChannel`](#classscy_1_1RotatingFileChannel)    | 
`class `[`scy::RunnableQueue`](#classscy_1_1RunnableQueue)    | 
`class `[`scy::ScopedConfiguration`](#classscy_1_1ScopedConfiguration)    | 
`class `[`scy::ScopedPointer`](#classscy_1_1ScopedPointer)    | Scoped Pointer Classes.
`class `[`scy::ScopedRawPointer`](#classscy_1_1ScopedRawPointer)    | 
`class `[`scy::ScopedSharedPointer`](#classscy_1_1ScopedSharedPointer)    | 
`class `[`scy::SharedObject`](#classscy_1_1SharedObject)    | 
`class `[`scy::Signal`](#classscy_1_1Signal)    | 
`class `[`scy::Signal2`](#classscy_1_1Signal2)    | 
`class `[`scy::Signal3`](#classscy_1_1Signal3)    | 
`class `[`scy::Signal4`](#classscy_1_1Signal4)    | 
`class `[`scy::SignalBase`](#classscy_1_1SignalBase)    | 
`class `[`scy::Singleton`](#classscy_1_1Singleton)    | 
`class `[`scy::State`](#classscy_1_1State)    | 
`class `[`scy::Stateful`](#classscy_1_1Stateful)    | 
`class `[`scy::StateSignal`](#classscy_1_1StateSignal)    | 
`class `[`scy::StopPropagation`](#classscy_1_1StopPropagation)    | This exception is used to break out of a [Signal](#classscy_1_1Signal) callback scope.
`class `[`scy::Stopwatch`](#classscy_1_1Stopwatch)    | 
`class `[`scy::Stream`](#classscy_1_1Stream)    | 
`class `[`scy::StreamWriter`](#classscy_1_1StreamWriter)    | 
`class `[`scy::SyncContext`](#classscy_1_1SyncContext)    | 
`class `[`scy::SyncPacketQueue`](#classscy_1_1SyncPacketQueue)    | 
`class `[`scy::SyncQueue`](#classscy_1_1SyncQueue)    | 
`class `[`scy::Task`](#classscy_1_1Task)    | 
`class `[`scy::TaskRunner`](#classscy_1_1TaskRunner)    | 
`class `[`scy::Thread`](#classscy_1_1Thread)    | 
`class `[`scy::ThreadedStreamReader`](#classscy_1_1ThreadedStreamReader)    | 
`class `[`scy::TimedToken`](#classscy_1_1TimedToken)    | A token that expires after the specified duration.
`class `[`scy::Timeout`](#classscy_1_1Timeout)    | Simple millisecond timeout counter which expires after a given delay.
`class `[`scy::Timer`](#classscy_1_1Timer)    | 
`class `[`scy::Timespan`](#classscy_1_1Timespan)    | A class that represents time spans up to microsecond resolution.
`class `[`scy::Timestamp`](#classscy_1_1Timestamp)    | 
`class `[`scy::Timezone`](#classscy_1_1Timezone)    | This class provides information about the current timezone.
`class `[`scy::TZInfo`](#classscy_1_1TZInfo)    | 
`struct `[`scy::Bitwise`](#structscy_1_1Bitwise)    | 
`struct `[`scy::DelegateBase`](#structscy_1_1DelegateBase)    | 
`struct `[`scy::DelegateCallback`](#structscy_1_1DelegateCallback)    | 
`struct `[`scy::DelegateCallback< C, 0, false >`](#structscy_1_1DelegateCallback_3_01C_00_010_00_01false_01_4)    | 
`struct `[`scy::DelegateCallback< C, 0, true >`](#structscy_1_1DelegateCallback_3_01C_00_010_00_01true_01_4)    | 
`struct `[`scy::DelegateCallback< C, 1, false, P >`](#structscy_1_1DelegateCallback_3_01C_00_011_00_01false_00_01P_01_4)    | 
`struct `[`scy::DelegateCallback< C, 1, true, P >`](#structscy_1_1DelegateCallback_3_01C_00_011_00_01true_00_01P_01_4)    | 
`struct `[`scy::DelegateCallback< C, 2, false, P, P2 >`](#structscy_1_1DelegateCallback_3_01C_00_012_00_01false_00_01P_00_01P2_01_4)    | 
`struct `[`scy::DelegateCallback< C, 2, true, P, P2 >`](#structscy_1_1DelegateCallback_3_01C_00_012_00_01true_00_01P_00_01P2_01_4)    | 
`struct `[`scy::DelegateCallback< C, 3, false, P, P2, P3 >`](#structscy_1_1DelegateCallback_3_01C_00_013_00_01false_00_01P_00_01P2_00_01P3_01_4)    | 
`struct `[`scy::DelegateCallback< C, 3, true, P, P2, P3 >`](#structscy_1_1DelegateCallback_3_01C_00_013_00_01true_00_01P_00_01P2_00_01P3_01_4)    | 
`struct `[`scy::DelegateCallback< C, 4, false, P, P2, P3, P4 >`](#structscy_1_1DelegateCallback_3_01C_00_014_00_01false_00_01P_00_01P2_00_01P3_00_01P4_01_4)    | 
`struct `[`scy::DelegateCallback< C, 4, true, P, P2, P3, P4 >`](#structscy_1_1DelegateCallback_3_01C_00_014_00_01true_00_01P_00_01P2_00_01P3_00_01P4_01_4)    | 
`struct `[`scy::DelegateCallback< C, 8, false, PolymorphicT >`](#structscy_1_1DelegateCallback_3_01C_00_018_00_01false_00_01PolymorphicT_01_4)    | 
`struct `[`scy::DelegateCallback< C, 8, true, PolymorphicT >`](#structscy_1_1DelegateCallback_3_01C_00_018_00_01true_00_01PolymorphicT_01_4)    | 
`struct `[`scy::Error`](#structscy_1_1Error)    | 
`struct `[`scy::IPacketInfo`](#structscy_1_1IPacketInfo)    | 
`struct `[`scy::LogStream`](#structscy_1_1LogStream)    | 
`struct `[`scy::OptionParser`](#structscy_1_1OptionParser)    | 
`struct `[`scy::PacketAdapterReference`](#structscy_1_1PacketAdapterReference)    | For 0.8.x compatibility.
`struct `[`scy::PacketCreationStrategy`](#structscy_1_1PacketCreationStrategy)    | 
`struct `[`scy::PacketStreamState`](#structscy_1_1PacketStreamState)    | 
`struct `[`scy::SharedLibrary`](#structscy_1_1SharedLibrary)    | 
`struct `[`scy::TransactionState`](#structscy_1_1TransactionState)    | 
# class `scy::AbstractCollection` {#classscy_1_1AbstractCollection}




[AbstractCollection](#classscy_1_1AbstractCollection) is an abstract interface for managing a key-value store of indexed pointers.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  AbstractCollection()` | 
`public inline virtual  ~AbstractCollection()` | 
`public bool add(const TKey & key,TValue * item,bool whiny)` | 
`public bool remove(const TValue * item)` | 
`public TValue * remove(const TKey & key)` | 
`public bool exists(const TKey & key) const` | 
`public bool exists(const TValue * item) const` | 
`public bool free(const TKey & key)` | 
`public bool empty() const` | 
`public int size() const` | 
`public TValue * get(const TKey & key,bool whiny) const` | 
`public void clear()` | 

## Members

#### `public inline  AbstractCollection()` {#group__base_1ga890f80336ad1fc7036ef91c7c4006723}





#### `public inline virtual  ~AbstractCollection()` {#group__base_1gadb44ca86f52d6229148fab898575ba5f}





#### `public bool add(const TKey & key,TValue * item,bool whiny)` {#group__base_1gaaee10ff4d5a246d2b62d3e1f4deb799d}





#### `public bool remove(const TValue * item)` {#group__base_1ga7688d5f1a92a829835e39144fa703383}





#### `public TValue * remove(const TKey & key)` {#group__base_1ga36c6ceb22481f952b070df77251f049f}





#### `public bool exists(const TKey & key) const` {#group__base_1ga862bf994fa6f95c49563528a1dc7f320}





#### `public bool exists(const TValue * item) const` {#group__base_1gaadc51ac08f83c7fd416067d31488fc4e}





#### `public bool free(const TKey & key)` {#group__base_1gad1fb3be3451f07f6a6c2841a71105657}





#### `public bool empty() const` {#group__base_1gaa145cb6f2956777b6da95c31750645b8}





#### `public int size() const` {#group__base_1gab188681872e88c925f49fe08141e1eed}





#### `public TValue * get(const TKey & key,bool whiny) const` {#group__base_1gaa64bd4169396cd4001662628c77a1d09}





#### `public void clear()` {#group__base_1gaff3c5cd97f75e7f56401aa18d13b39c3}





# class `scy::Application` {#classscy_1_1Application}




A simple event based application which runs until the internal event loop is terminated.

The [Application](#classscy_1_1Application) class provides shutdown handling (Ctrl-C).

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public uv::Loop * loop` | 
`public  Application(uv::Loop * loop)` | 
`public  ~Application()` | 
`public void run()` | 
`public void stop()` | 
`public void finalize()` | 
`public void bindShutdownSignal(std::function< void(void *)> callback,void * opaque)` | Bind the shutdown signal.
`public void waitForShutdown(std::function< void(void *)> callback,void * opaque)` | Bind the shutdown signal and run the main event loop.
`protected  Application(const `[`Application`](#classscy_1_1Application)` &)` | 
`protected  Application(`[`Application`](#classscy_1_1Application)` &&)` | 
`protected `[`Application`](#classscy_1_1Application)` & operator=(const `[`Application`](#classscy_1_1Application)` &)` | 
`protected `[`Application`](#classscy_1_1Application)` & operator=(`[`Application`](#classscy_1_1Application)` &&)` | 

## Members

#### `public uv::Loop * loop` {#group__base_1ga0cf54187707ea8b8f88324f045fbc797}



The active event loop. May be assigned at construction, otherwise the default event loop is used.

#### `public  Application(uv::Loop * loop)` {#group__base_1gadb808e26528e18856d2896a3ce7947ca}





#### `public  ~Application()` {#group__base_1ga0aafa241a2debe7409ae096420068d1e}





#### `public void run()` {#group__base_1gacf3f29800455e5d360a19b9c8fe66241}





#### `public void stop()` {#group__base_1ga830f616291457503049f9fc9d1a3f8ba}





#### `public void finalize()` {#group__base_1gae94cf187f38a9accf0abdffc868e700a}





#### `public void bindShutdownSignal(std::function< void(void *)> callback,void * opaque)` {#group__base_1ga80ca3367db09fde58ffec5b3b17ce960}

Bind the shutdown signal.

Shutdown handling

#### `public void waitForShutdown(std::function< void(void *)> callback,void * opaque)` {#group__base_1ga648ccd97eb0acd73cb4dc0b2b4ae030a}

Bind the shutdown signal and run the main event loop.



#### `protected  Application(const `[`Application`](#classscy_1_1Application)` &)` {#group__base_1gaf35e66236b350dc80aabc6c721d81459}





#### `protected  Application(`[`Application`](#classscy_1_1Application)` &&)` {#group__base_1ga733b14143f30c98d7e73815e05523f24}





#### `protected `[`Application`](#classscy_1_1Application)` & operator=(const `[`Application`](#classscy_1_1Application)` &)` {#group__base_1gacc683343ef14eb0899b29dfa5c75b820}





#### `protected `[`Application`](#classscy_1_1Application)` & operator=(`[`Application`](#classscy_1_1Application)` &&)` {#group__base_1gab9a81a8272daa858d1ddf5c294badb86}





# class `scy::AsyncLogWriter` {#classscy_1_1AsyncLogWriter}

```
class scy::AsyncLogWriter
  : public scy::LogWriter
  : public scy::async::Runnable
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  AsyncLogWriter()` | 
`public virtual  ~AsyncLogWriter()` | 
`public virtual void write(`[`LogStream`](#structscy_1_1LogStream)` * stream)` | Queues the given log message stream.
`public void flush()` | Flushes queued messages.
`public virtual void run()` | Writes queued messages asynchronously.
`public void clear()` | Clears all queued messages.
`protected `[`Thread`](api-base.md#classscy_1_1Thread)` _thread` | 
`protected std::deque< `[`LogStream`](api-base.md#structscy_1_1LogStream)` * > _pending` | 
`protected mutable `[`Mutex`](api-thread.md#classscy_1_1Mutex)` _mutex` | 
`protected bool writeNext()` | 

## Members

#### `public  AsyncLogWriter()` {#group__base_1ga0188ef05599b6dda991e15a125926a93}





#### `public virtual  ~AsyncLogWriter()` {#group__base_1ga94107b494835580b0e435f9c7ef870fe}





#### `public virtual void write(`[`LogStream`](#structscy_1_1LogStream)` * stream)` {#group__base_1ga2fd8247a9aff8ef55b2d5cb9ca9c6145}

Queues the given log message stream.



#### `public void flush()` {#group__base_1ga6af94a2ef68c7b0114f6d91684a2aca7}

Flushes queued messages.



#### `public virtual void run()` {#group__base_1gaa38a45dab3b692894a6d1753dadb7164}

Writes queued messages asynchronously.



#### `public void clear()` {#group__base_1gad918ab56f2652c0df2a0b4c74823fc18}

Clears all queued messages.



#### `protected `[`Thread`](api-base.md#classscy_1_1Thread)` _thread` {#group__base_1ga75dcded0c2370591afb489f4892b7b54}





#### `protected std::deque< `[`LogStream`](api-base.md#structscy_1_1LogStream)` * > _pending` {#group__base_1gaa09529c75e1bbe57025b72be87dfd41c}





#### `protected mutable `[`Mutex`](api-thread.md#classscy_1_1Mutex)` _mutex` {#group__base_1ga043a2de2b895ee38829901de0e1d6d43}





#### `protected bool writeNext()` {#group__base_1ga05d6ccb870b82f130810f3c9d002d9bb}





# class `scy::AsyncPacketQueue` {#classscy_1_1AsyncPacketQueue}

```
class scy::AsyncPacketQueue
  : public scy::AsyncQueue< T >
  : public scy::PacketProcessor
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`PacketSignal`](api-base.md#classscy_1_1SignalBase)` emitter` | 
`public inline  AsyncPacketQueue(int maxSize)` | 
`public inline virtual  ~AsyncPacketQueue()` | 
`public inline virtual void close()` | 
`public inline virtual void process(`[`IPacket`](#classscy_1_1IPacket)` & packet)` | 
`public inline virtual bool accepts(`[`IPacket`](#classscy_1_1IPacket)` &)` | 
`protected inline virtual void dispatch(T & packet)` | 
`protected inline virtual void onStreamStateChange(const `[`PacketStreamState`](#structscy_1_1PacketStreamState)` &)` | 

## Members

#### `public `[`PacketSignal`](api-base.md#classscy_1_1SignalBase)` emitter` {#group__base_1gaf462c1a658ea10a25f0159c3adf22054}





#### `public inline  AsyncPacketQueue(int maxSize)` {#group__base_1gab12334be9597666d9ccdbd8c1dc2b8b6}





#### `public inline virtual  ~AsyncPacketQueue()` {#group__base_1ga1d52cb92da8849a198e4c47aea73876c}





#### `public inline virtual void close()` {#group__base_1gab25027fe2c2a2cb00edf378a4e314ea0}





#### `public inline virtual void process(`[`IPacket`](#classscy_1_1IPacket)` & packet)` {#group__base_1ga25cdda1ce6348ca3da2f064a9ce7f078}



This method performs processing on the given packet and emits the result.

Note: If packet processing is async (the packet is not in the current thread scope) then packet data must be copied. Copied data can be freed directly aFter the async call to emit() the outgoing packet.

#### `public inline virtual bool accepts(`[`IPacket`](#classscy_1_1IPacket)` &)` {#group__base_1ga0ca37d7b236c0e856c839d54024b4347}



This method ensures compatibility with the given packet type. Return false to reject the packet.

#### `protected inline virtual void dispatch(T & packet)` {#group__base_1gaf6e78ca0279615319d33d7e6aa624e55}





#### `protected inline virtual void onStreamStateChange(const `[`PacketStreamState`](#structscy_1_1PacketStreamState)` &)` {#group__base_1gae72126b2b0f118975423119a3ddd6978}



Called by the [PacketStream](#classscy_1_1PacketStream) to notify when the internal [Stream](#classscy_1_1Stream) state changes. On receiving the Stopped state, it is the responsibility of the adapter to have ceased all outgoing packet transmission, especially in multi-thread scenarios.

# class `scy::AsyncQueue` {#classscy_1_1AsyncQueue}

```
class scy::AsyncQueue
  : public scy::RunnableQueue< T >
```  



[AsyncQueue](#classscy_1_1AsyncQueue) is a thread-based queue which receives packets from any thread source and dispatches them asynchronously.

This queue is useful for deferring load from operation critical system devices before performing long running tasks.

The thread will call the [RunnableQueue](#classscy_1_1RunnableQueue)'s [run()](#group__base_1ga372c52aa2d9b4c33f9b620dfb2c2af23) method to constantly flush outgoing packets until cancel() is called.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  AsyncQueue(int limit)` | 
`public inline virtual void cancel()` | 
`protected `[`Thread`](api-base.md#classscy_1_1Thread)` _thread` | 
`protected inline virtual  ~AsyncQueue()` | 

## Members

#### `public inline  AsyncQueue(int limit)` {#group__base_1ga1e59a5d7850526b3bb9226e707552a87}





#### `public inline virtual void cancel()` {#group__base_1ga0096428c7d684eb39f378784ee868fa1}





#### `protected `[`Thread`](api-base.md#classscy_1_1Thread)` _thread` {#group__base_1ga43d70d35f82a4c1286108cc14b6b8066}





#### `protected inline virtual  ~AsyncQueue()` {#group__base_1gad02ee332589a016e4864aba8e75b3194}





# class `scy::AsyncStartable` {#classscy_1_1AsyncStartable}

```
class scy::AsyncStartable
  : public TStartable
```  



Depreciated: This class is an invisible wrapper around a TStartable instance, which provides asynchronous access to the TStartable start() and stop() methods. TStartable is an instance of [async::Startable](#classscy_1_1async_1_1Startable).

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  AsyncStartable()` | 
`public inline virtual  ~AsyncStartable()` | 
`public inline virtual bool start()` | 
`public inline virtual void stop()` | 
`protected `[`Thread`](api-base.md#classscy_1_1Thread)` _thread` | 

## Members

#### `public inline  AsyncStartable()` {#group__base_1gab4fc448d9f475b84cb7277ead544f306}





#### `public inline virtual  ~AsyncStartable()` {#group__base_1ga3ab3aff431dfe7935bbeeaedd328dffb}





#### `public inline virtual bool start()` {#group__base_1ga08d56cb961c9e50feed8334e4870c518}





#### `public inline virtual void stop()` {#group__base_1ga3edf32524ceb24b9144f82a8a4544b96}





#### `protected `[`Thread`](api-base.md#classscy_1_1Thread)` _thread` {#group__base_1ga7745888e6f3cde6363e9eba4a1d60a4a}





# class `scy::BitReader` {#classscy_1_1BitReader}


A [BitReader](#classscy_1_1BitReader) for reading binary streams.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  BitReader(const char * bytes,std::size_t size,`[`ByteOrder`](#group__base_1gaa934c0972c743983462716b01df7dc32)` order)` | 
`public  BitReader(const Buffer & buf,`[`ByteOrder`](#group__base_1gaa934c0972c743983462716b01df7dc32)` order)` | 
`public  BitReader(const `[`ConstBuffer`](#classscy_1_1ConstBuffer)` & pod,`[`ByteOrder`](#group__base_1gaa934c0972c743983462716b01df7dc32)` order)` | 
`public  ~BitReader()` | 
`public void get(char * val,std::size_t len)` | 
`public void get(std::string & val,std::size_t len)` | 
`public void getU8(std::uint8_t & val)` | 
`public void getU16(std::uint16_t & val)` | 
`public void getU24(std::uint32_t & val)` | 
`public void getU32(std::uint32_t & val)` | 
`public void getU64(std::uint64_t & val)` | 
`public const char peek()` | 
`public const std::uint8_t peekU8()` | 
`public const std::uint16_t peekU16()` | 
`public const std::uint32_t peekU24()` | 
`public const std::uint32_t peekU32()` | 
`public const std::uint64_t peekU64()` | 
`public int skipToChar(char c)` | String parsing methods.
`public int skipWhitespace()` | 
`public int skipToNextLine()` | 
`public int skipNextWord()` | 
`public int readNextWord(std::string & val)` | 
`public int readNextNumber(unsigned int & val)` | 
`public int readLine(std::string & val)` | 
`public int readToNext(std::string & val,char c)` | 
`public void seek(std::size_t val)` | 
`public void skip(std::size_t size)` | 
`public std::size_t limit() const` | Returns the read limit.
`public inline std::size_t position() const` | Returns the current read position.
`public std::size_t available() const` | Returns the number of elements between the current position and the limit.
`public inline const char * begin() const` | 
`public inline const char * current() const` | 
`public inline `[`ByteOrder`](#group__base_1gaa934c0972c743983462716b01df7dc32)` order() const` | 
`public std::string toString()` | 

## Members

#### `public  BitReader(const char * bytes,std::size_t size,`[`ByteOrder`](#group__base_1gaa934c0972c743983462716b01df7dc32)` order)` {#group__base_1ga64318f8dc5f35cdb2a43a0b672c9d9a3}





#### `public  BitReader(const Buffer & buf,`[`ByteOrder`](#group__base_1gaa934c0972c743983462716b01df7dc32)` order)` {#group__base_1gabf0db9bf938b95829322f00935eb6092}





#### `public  BitReader(const `[`ConstBuffer`](#classscy_1_1ConstBuffer)` & pod,`[`ByteOrder`](#group__base_1gaa934c0972c743983462716b01df7dc32)` order)` {#group__base_1ga4d25c7558ec919c38a2aa2bb2d3d549e}





#### `public  ~BitReader()` {#group__base_1ga2b0a8e18dd144d85baaf44b4711ef57f}





#### `public void get(char * val,std::size_t len)` {#group__base_1ga7770f03636c30a7e17af0c5ac64c8bcc}



Reads a value from the [BitReader](#classscy_1_1BitReader). Returns false if there isn't enough data left for the specified type. Throws a std::out_of_range exception if reading past the limit.

#### `public void get(std::string & val,std::size_t len)` {#group__base_1ga2954c6c87eb7244cecd5b8314eb3625a}





#### `public void getU8(std::uint8_t & val)` {#group__base_1gaf5b3de17f8258a958073d8cc2c938de3}





#### `public void getU16(std::uint16_t & val)` {#group__base_1ga1dc004f64467d40fcf025b63bc65c202}





#### `public void getU24(std::uint32_t & val)` {#group__base_1gae6b5281ae3cf3ae424978e806f57c335}





#### `public void getU32(std::uint32_t & val)` {#group__base_1ga741621a7b85dc16f3309e97ff8ed4a9e}





#### `public void getU64(std::uint64_t & val)` {#group__base_1ga6d23ce44295dcc4bc542d0a538f3a0eb}





#### `public const char peek()` {#group__base_1gae207340c2735276d75374184da016fe2}





#### `public const std::uint8_t peekU8()` {#group__base_1gae01a1b1d2c2c047d8659c23d4107186d}





#### `public const std::uint16_t peekU16()` {#group__base_1ga874947c6791fbf214eeeb76f8dd11b7c}





#### `public const std::uint32_t peekU24()` {#group__base_1ga6338693b78a8e80b9a5f42e3cd1edb2b}





#### `public const std::uint32_t peekU32()` {#group__base_1ga89e278a8b785315f14842970f5093e80}





#### `public const std::uint64_t peekU64()` {#group__base_1ga6899a023f2a961f3eace4225cfc849b4}



Peeks data from the [BitReader](#classscy_1_1BitReader). -1 is returned if reading past boundary.

#### `public int skipToChar(char c)` {#group__base_1ga9e2bbcb89dd1c0fd6541674cd8b56f57}

String parsing methods.



#### `public int skipWhitespace()` {#group__base_1ga1862d129d963a272eedfc5867d7bcd30}





#### `public int skipToNextLine()` {#group__base_1ga96796b3f3697f2d61acbc4437e6d5933}





#### `public int skipNextWord()` {#group__base_1ga1c0c3c2b88a144a94fa34d503151bfd1}





#### `public int readNextWord(std::string & val)` {#group__base_1gaaf6ea2b3ec1981cb6ec75991f2c34021}





#### `public int readNextNumber(unsigned int & val)` {#group__base_1ga7543c2fa69bc107f8a5a969d2e6152f2}





#### `public int readLine(std::string & val)` {#group__base_1gad4b808eec398dc3b921aa628aab87ea5}





#### `public int readToNext(std::string & val,char c)` {#group__base_1ga6638395c18e4718b84a81ff314eba5ef}





#### `public void seek(std::size_t val)` {#group__base_1ga22b6e8894b5adc15ca4900b3e5754b56}



Set position pointer to absolute position. Throws a std::out_of_range exception if the value exceeds the limit.

#### `public void skip(std::size_t size)` {#group__base_1ga50baf199bca0c978b0d7850c0128121a}



Set position pointer to relative position. Throws a std::out_of_range exception if the value exceeds the limit.

#### `public std::size_t limit() const` {#group__base_1ga6d6ae24e35d7a9293a7baf12f75e3952}

Returns the read limit.



#### `public inline std::size_t position() const` {#group__base_1ga23d468ad98b19e114d78c9edff0d7f96}

Returns the current read position.



#### `public std::size_t available() const` {#group__base_1gab87ea4c096e614cdb3f5e4db340ca176}

Returns the number of elements between the current position and the limit.



#### `public inline const char * begin() const` {#group__base_1ga6ee4105efc2e18f5b31eceafd959c0d3}





#### `public inline const char * current() const` {#group__base_1ga5f8b74d1434e5300fe37ee60f6e394e1}





#### `public inline `[`ByteOrder`](#group__base_1gaa934c0972c743983462716b01df7dc32)` order() const` {#group__base_1ga82d4b93cc1c3c8a9bbfd0937b7f59aeb}





#### `public std::string toString()` {#group__base_1gac5342f8fbc0fa5e1c49615e530a9c5fe}





# class `scy::BitWriter` {#classscy_1_1BitWriter}




A [BitWriter](#classscy_1_1BitWriter) for reading/writing binary streams.

Note that when using the constructor with the Buffer reference as an argument, the writer will dynamically expand the given buffer when writing passed the buffer capacity. All other cases will throw a std::out_of_range error when writing past the buffer capacity.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  BitWriter(char * bytes,std::size_t size,`[`ByteOrder`](#group__base_1gaa934c0972c743983462716b01df7dc32)` order)` | 
`public  BitWriter(Buffer & buf,`[`ByteOrder`](#group__base_1gaa934c0972c743983462716b01df7dc32)` order)` | 
`public  BitWriter(`[`MutableBuffer`](#classscy_1_1MutableBuffer)` & pod,`[`ByteOrder`](#group__base_1gaa934c0972c743983462716b01df7dc32)` order)` | 
`public  ~BitWriter()` | 
`public void put(const char * val,std::size_t len)` | 
`public void put(const std::string & val)` | 
`public void putU8(std::uint8_t val)` | 
`public void putU16(std::uint16_t val)` | 
`public void putU24(std::uint32_t val)` | 
`public void putU32(std::uint32_t val)` | 
`public void putU64(std::uint64_t val)` | 
`public bool update(const char * val,std::size_t len,std::size_t pos)` | 
`public bool update(const std::string & val,std::size_t pos)` | 
`public bool updateU8(std::uint8_t val,std::size_t pos)` | 
`public bool updateU16(std::uint16_t val,std::size_t pos)` | 
`public bool updateU24(std::uint32_t val,std::size_t pos)` | 
`public bool updateU32(std::uint32_t val,std::size_t pos)` | 
`public bool updateU64(std::uint64_t val,std::size_t pos)` | 
`public void seek(std::size_t val)` | 
`public void skip(std::size_t size)` | 
`public std::size_t limit() const` | Returns the write limit.
`public inline std::size_t position() const` | Returns the current write position.
`public std::size_t available() const` | Returns the number of elements between the current write position and the limit.
`public inline char * begin()` | 
`public inline char * current()` | 
`public inline const char * begin() const` | 
`public inline const char * current() const` | 
`public inline `[`ByteOrder`](#group__base_1gaa934c0972c743983462716b01df7dc32)` order() const` | Returns written bytes as a string.
`public std::string toString()` | 

## Members

#### `public  BitWriter(char * bytes,std::size_t size,`[`ByteOrder`](#group__base_1gaa934c0972c743983462716b01df7dc32)` order)` {#group__base_1ga29083063f9b5dda2dd784b0b660bff89}





#### `public  BitWriter(Buffer & buf,`[`ByteOrder`](#group__base_1gaa934c0972c743983462716b01df7dc32)` order)` {#group__base_1ga9197ef38d9bcd9c796da1ef8ae70bb8c}





#### `public  BitWriter(`[`MutableBuffer`](#classscy_1_1MutableBuffer)` & pod,`[`ByteOrder`](#group__base_1gaa934c0972c743983462716b01df7dc32)` order)` {#group__base_1ga0d30b3f7a40f306e24b5f905c692356e}





#### `public  ~BitWriter()` {#group__base_1ga18cd74cab24e3aafc649ec3fa700611c}





#### `public void put(const char * val,std::size_t len)` {#group__base_1gaa6308699b53d3a728e44713e582fe3e4}



Append bytes to the buffer. Throws a std::out_of_range exception if reading past the limit.

#### `public void put(const std::string & val)` {#group__base_1ga74ae0af5b9c1456fe21e5d7d8dc8c0b0}





#### `public void putU8(std::uint8_t val)` {#group__base_1gae0ad248037bb190fd430d3b0a1891695}





#### `public void putU16(std::uint16_t val)` {#group__base_1ga5473ff9b1c760f81c8251f105115abde}





#### `public void putU24(std::uint32_t val)` {#group__base_1ga2528403ac77979a686130b2c49d6a9b5}





#### `public void putU32(std::uint32_t val)` {#group__base_1ga3efaec789a01d237156e23b4ea44affd}





#### `public void putU64(std::uint64_t val)` {#group__base_1gad4fb935091c3d07ee5190d35eb339391}





#### `public bool update(const char * val,std::size_t len,std::size_t pos)` {#group__base_1gaad17b6c0826093f91c3da558f97e1c5f}



Update a byte range. Throws a std::out_of_range exception if reading past the limit.

#### `public bool update(const std::string & val,std::size_t pos)` {#group__base_1ga48cc5a1a6525b7b93b8ff81c0cdd1b28}





#### `public bool updateU8(std::uint8_t val,std::size_t pos)` {#group__base_1ga98173917eba624120dcc595c15fcf759}





#### `public bool updateU16(std::uint16_t val,std::size_t pos)` {#group__base_1ga93badd5c68be44c516d73583f000e1b9}





#### `public bool updateU24(std::uint32_t val,std::size_t pos)` {#group__base_1ga89ff0ec975f28761388e3f1ef41009fc}





#### `public bool updateU32(std::uint32_t val,std::size_t pos)` {#group__base_1gac24f5e1d4cbb3c29845a1bb41579851c}





#### `public bool updateU64(std::uint64_t val,std::size_t pos)` {#group__base_1ga1280276f0692ba907df0e112284cb9c6}





#### `public void seek(std::size_t val)` {#group__base_1ga71cefd76e8c1ecd3c90e64fdbd5c545b}



Set position pointer to absolute position. Throws a std::out_of_range exception if the value exceeds the limit.

#### `public void skip(std::size_t size)` {#group__base_1ga31424089be7442cbd912675ccb55c5e0}



Set position pointer to relative position. Throws a std::out_of_range exception if the value exceeds the limit.

#### `public std::size_t limit() const` {#group__base_1ga94d894b090909ae66f8f5dd4b4abd075}

Returns the write limit.



#### `public inline std::size_t position() const` {#group__base_1ga73814b5b964501c56045f70815f94f4a}

Returns the current write position.



#### `public std::size_t available() const` {#group__base_1ga33ec3bf3c9676230958e5082248204bb}

Returns the number of elements between the current write position and the limit.



#### `public inline char * begin()` {#group__base_1ga5f4e9d6f4804cd95536a8aaa5191d79d}





#### `public inline char * current()` {#group__base_1gaeb5cd108a41063f5e67c3a437916cf5c}





#### `public inline const char * begin() const` {#group__base_1gabcde7c1a4fe040690437358bd44f88d3}





#### `public inline const char * current() const` {#group__base_1ga3126fd062b77270aecca62dfc36c62c2}





#### `public inline `[`ByteOrder`](#group__base_1gaa934c0972c743983462716b01df7dc32)` order() const` {#group__base_1gaf577c3165d6bea6a49e49f33e056b56a}

Returns written bytes as a string.



#### `public std::string toString()` {#group__base_1ga4d4c2a107262e4dc586917646c69ed31}





# class `scy::Configuration` {#classscy_1_1Configuration}




[Configuration](#classscy_1_1Configuration) is an abstract base class for managing /// different kinds of configuration storage back ends such as /// JSON, XML, or database.

Subclasses must override the [getRaw()](#group__base_1ga1246d954823215df6d932e78a08bc23b) and [setRaw()](#group__base_1gac6a4023c271c2357334275f15329310d) and methods.

This class is safe for multithreaded use.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`Signal2`](api-base.md#classscy_1_1Signal2)`< const std::string &, const std::string & > PropertyChanged` | The Key and Value of the changed configuration property.
`public  Configuration()` | Creates the [Configuration](api-base.md#classscy_1_1Configuration).
`public virtual  ~Configuration()` | Destroys the [Configuration](api-base.md#classscy_1_1Configuration).
`public bool exists(const std::string & key) const` | Returns true if the property with the given key exists.
`public std::string getString(const std::string & key) const` | 
`public std::string getString(const std::string & key,const std::string & defaultValue) const` | 
`public std::string getRawString(const std::string & key) const` | 
`public std::string getRawString(const std::string & key,const std::string & defaultValue) const` | 
`public int getInt(const std::string & key) const` | 
`public int getInt(const std::string & key,int defaultValue) const` | 
`public std::int64_t getLargeInt(const std::string & key) const` | 
`public std::int64_t getLargeInt(const std::string & key,std::int64_t defaultValue) const` | 
`public double getDouble(const std::string & key) const` | 
`public double getDouble(const std::string & key,double defaultValue) const` | 
`public bool getBool(const std::string & key) const` | 
`public bool getBool(const std::string & key,bool defaultValue) const` | 
`public void setString(const std::string & key,const std::string & value)` | 
`public void setInt(const std::string & key,int value)` | 
`public void setLargeInt(const std::string & key,std::int64_t value)` | 
`public void setDouble(const std::string & key,double value)` | 
`public void setBool(const std::string & key,bool value)` | 
`protected bool getRaw(const std::string & key,std::string & value) const` | 
`protected void setRaw(const std::string & key,const std::string & value)` | 

## Members

#### `public `[`Signal2`](api-base.md#classscy_1_1Signal2)`< const std::string &, const std::string & > PropertyChanged` {#group__base_1ga7cc246aa31150df064501e3df11e4976}

The Key and Value of the changed configuration property.



#### `public  Configuration()` {#group__base_1ga17ab598f09508375af5c81bcca2f7adf}

Creates the [Configuration](#classscy_1_1Configuration).



#### `public virtual  ~Configuration()` {#group__base_1ga92fedd22f2c662f1fd5ba513dedaad45}

Destroys the [Configuration](#classscy_1_1Configuration).



#### `public bool exists(const std::string & key) const` {#group__base_1ga46b7b29d78f0cf2939e9ffb3e561652c}

Returns true if the property with the given key exists.



#### `public std::string getString(const std::string & key) const` {#group__base_1ga05bb2078ab85e6d01e07bd0339e7f051}



Returns the string value of the property with the given name. Throws a NotFoundException if the key does not exist.

#### `public std::string getString(const std::string & key,const std::string & defaultValue) const` {#group__base_1ga759bde263c84572e2dca86974b79abe0}



If a property with the given key exists, returns the property's string value, otherwise returns the given default value.

#### `public std::string getRawString(const std::string & key) const` {#group__base_1ga32c5e46d389028da0c63b71059b261f1}



Returns the raw string value of the property with the given name. Throws a NotFoundException if the key does not exist. References to other properties are not expanded.

#### `public std::string getRawString(const std::string & key,const std::string & defaultValue) const` {#group__base_1ga39dea64c1f5a83216ed7f1de59fbc1ce}



If a property with the given key exists, returns the property's raw string value, otherwise returns the given default value. References to other properties are not expanded.

#### `public int getInt(const std::string & key) const` {#group__base_1gae4002dbb61ede2e81e518474ebc49057}



Returns the int value of the property with the given name. Throws a NotFoundException if the key does not exist. Throws a SyntaxException if the property can not be converted to an int. Numbers starting with 0x are treated as hexadecimal.

#### `public int getInt(const std::string & key,int defaultValue) const` {#group__base_1ga8b3ab025bfee6fdd2b9e83a75f892ca4}



If a property with the given key exists, returns the property's int value, otherwise returns the given default value. Throws a SyntaxException if the property can not be converted to an int. Numbers starting with 0x are treated as hexadecimal.

#### `public std::int64_t getLargeInt(const std::string & key) const` {#group__base_1gad069788603d90de791b983999d127f88}



Returns the int value of the property with the given name. Throws a NotFoundException if the key does not exist. Throws a SyntaxException if the property can not be converted to an int. Numbers starting with 0x are treated as hexadecimal.

#### `public std::int64_t getLargeInt(const std::string & key,std::int64_t defaultValue) const` {#group__base_1ga4048a586cef122d39c1553228c407524}



If a property with the given key exists, returns the property's int value, otherwise returns the given default value. Throws a SyntaxException if the property can not be converted to an int. Numbers starting with 0x are treated as hexadecimal.

#### `public double getDouble(const std::string & key) const` {#group__base_1gabb0a768d637e028d16bd14ddb3c6eeeb}



Returns the double value of the property with the given name. Throws a NotFoundException if the key does not exist. Throws a SyntaxException if the property can not be converted to a double.

#### `public double getDouble(const std::string & key,double defaultValue) const` {#group__base_1gaa570bb19020ae6f6d1afa8b5e099da84}



If a property with the given key exists, returns the property's double value, otherwise returns the given default value. Throws a SyntaxException if the property can not be converted to an double.

#### `public bool getBool(const std::string & key) const` {#group__base_1ga18f5f14dbe585841a2c97e3e6fabe923}



Returns the double value of the property with the given name. Throws a NotFoundException if the key does not exist. Throws a SyntaxException if the property can not be converted to a double.

#### `public bool getBool(const std::string & key,bool defaultValue) const` {#group__base_1gacf0956fc36e268cbc074e11ac8975b54}



If a property with the given key exists, returns the property's bool value, otherwise returns the given default value. Throws a SyntaxException if the property can not be converted to a boolean. The following string values can be converted into a boolean:

* numerical values: non zero becomes true, zero becomes false


* strings: true, yes, on become true, false, no, off become false Case does not matter.

#### `public void setString(const std::string & key,const std::string & value)` {#group__base_1gaa6619e41320857455ea04e4cf5dc5ada}



Sets the property with the given key to the given value. An already existing value for the key is overwritten.

#### `public void setInt(const std::string & key,int value)` {#group__base_1ga9668ea52c509d5ee335ca194402b0a8c}



Sets the property with the given key to the given value. An already existing value for the key is overwritten.

#### `public void setLargeInt(const std::string & key,std::int64_t value)` {#group__base_1ga82ff1ce7b02ab4989669913175970cd2}



Sets the property with the given key to the given value. An already existing value for the key is overwritten.

#### `public void setDouble(const std::string & key,double value)` {#group__base_1ga0c39abf4df23106c7b47221fe9cc3252}



Sets the property with the given key to the given value. An already existing value for the key is overwritten.

#### `public void setBool(const std::string & key,bool value)` {#group__base_1ga6130c066e06d2f3c8554160d216058bf}



Sets the property with the given key to the given value. An already existing value for the key is overwritten.

#### `protected bool getRaw(const std::string & key,std::string & value) const` {#group__base_1ga1246d954823215df6d932e78a08bc23b}



If the property with the given key exists, stores the property's value in value and returns true. Otherwise, returns false.

Must be overridden by subclasses.

#### `protected void setRaw(const std::string & key,const std::string & value)` {#group__base_1gac6a4023c271c2357334275f15329310d}



Sets the property with the given key to the given value. An already existing value for the key is overwritten.

The implementation is responsible for emitting the PropertyChanged signal.

# class `scy::ConsoleChannel` {#classscy_1_1ConsoleChannel}

```
class scy::ConsoleChannel
  : public scy::LogChannel
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  ConsoleChannel(const std::string & name,LogLevel level,const std::string & timeFormat)` | 
`public inline virtual  ~ConsoleChannel()` | 
`public virtual void write(const `[`LogStream`](#structscy_1_1LogStream)` & stream)` | 

## Members

#### `public  ConsoleChannel(const std::string & name,LogLevel level,const std::string & timeFormat)` {#group__base_1gaf5ca172eec6c9232405a4089a6248f25}





#### `public inline virtual  ~ConsoleChannel()` {#group__base_1gae5e41a6a0ff968f426c095793597a3ac}





#### `public virtual void write(const `[`LogStream`](#structscy_1_1LogStream)` & stream)` {#group__base_1ga480fd289733d1e75795041bdf8b7babd}





# class `scy::ConstBuffer` {#classscy_1_1ConstBuffer}




The [ConstBuffer](#classscy_1_1ConstBuffer) class provides a safe representation of a buffer that cannot be modified. It does not own the underlying data, and so is cheap to copy or assign.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  ConstBuffer()` | Construct an empty buffer.
`public inline  ConstBuffer(const void * data,std::size_t size)` | Construct a buffer to represent the given memory range.
`public inline  ConstBuffer(const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & b)` | Construct a non-modifiable buffer from a modifiable one.
`public inline const void * data() const` | 
`public inline std::size_t size() const` | 
`public inline const char * cstr() const` | String methods.
`public inline std::string str() const` | 

## Members

#### `public inline  ConstBuffer()` {#group__base_1ga9ee27b72f04bbf0a7012a2f56542f8ef}

Construct an empty buffer.



#### `public inline  ConstBuffer(const void * data,std::size_t size)` {#group__base_1gabb1b16cc640e5bb1647795bdea87a5d3}

Construct a buffer to represent the given memory range.



#### `public inline  ConstBuffer(const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & b)` {#group__base_1ga217c5af0a20a4a11cfa451a389997ad9}

Construct a non-modifiable buffer from a modifiable one.



#### `public inline const void * data() const` {#group__base_1ga0c19b0c976bb5ab3660cc1a79dd8bbae}





#### `public inline std::size_t size() const` {#group__base_1gaa77b0fded68a8438d633ecb9cb361850}





#### `public inline const char * cstr() const` {#group__base_1gadca6bc7c6eff265063d69d98e51405be}

String methods.



#### `public inline std::string str() const` {#group__base_1gab94b7a54497431189aeae02fe254c2c8}





# class `scy::DateTime` {#classscy_1_1DateTime}




This class represents an instant in time, expressed in years, months, days, hours, minutes, seconds and milliseconds based on the Gregorian calendar. The class is mainly useful for conversions between UTC, Julian day and Gregorian calendar dates.

The date and time stored in a [DateTime](#classscy_1_1DateTime) is always in UTC (Coordinated Universal Time) and thus independent of the timezone in effect on the system.

Conversion calculations are based on algorithms collected and described by Peter Baum at [http://vsg.cape.com/~pbaum/date/date0.htm](http://vsg.cape.com/~pbaum/date/date0.htm)

Internally, this class stores a date/time in two forms (UTC and broken down) for performance reasons. Only use this class for conversions between date/time representations. Use the [Timestamp](#classscy_1_1Timestamp) class for everything else.

Notes:

* Zero is a valid year (in accordance with ISO 8601 and astronomical year numbering)


* Year zero (0) is a leap year


* Negative years (years preceding 1 BC) are not supported





For more information, please see:

* [http://en.wikipedia.org/wiki/Gregorian_Calendar](http://en.wikipedia.org/wiki/Gregorian_Calendar)


* [http://en.wikipedia.org/wiki/Julian_day](http://en.wikipedia.org/wiki/Julian_day)


* [http://en.wikipedia.org/wiki/UTC](http://en.wikipedia.org/wiki/UTC)


* [http://en.wikipedia.org/wiki/ISO_8601](http://en.wikipedia.org/wiki/ISO_8601)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  DateTime()` | Creates a [DateTime](api-base.md#classscy_1_1DateTime) for the current date and time.
`public  DateTime(const `[`Timestamp`](#classscy_1_1Timestamp)` & timestamp)` | 
`public  DateTime(int year,int month,int day,int hour,int minute,int second,int millisecond,int microsecond)` | 
`public  DateTime(double julianDay)` | Creates a [DateTime](api-base.md#classscy_1_1DateTime) for the given Julian day.
`public  DateTime(`[`Timestamp::UtcTimeVal`](#group__base_1gaec8ace0f25db0ce91c10b30270c3dc24)` utcTime,`[`Timestamp::TimeDiff`](#group__base_1ga9f06831e4e9b9cbc6f061cc61dc91fd0)` diff)` | 
`public  DateTime(const `[`DateTime`](#classscy_1_1DateTime)` & dateTime)` | Copy constructor. Creates the [DateTime](api-base.md#classscy_1_1DateTime) from another one.
`public  ~DateTime()` | Destroys the [DateTime](api-base.md#classscy_1_1DateTime).
`public `[`DateTime`](#classscy_1_1DateTime)` & operator=(const `[`DateTime`](#classscy_1_1DateTime)` & dateTime)` | Assigns another [DateTime](api-base.md#classscy_1_1DateTime).
`public `[`DateTime`](#classscy_1_1DateTime)` & operator=(const `[`Timestamp`](#classscy_1_1Timestamp)` & timestamp)` | Assigns a [Timestamp](api-base.md#classscy_1_1Timestamp).
`public `[`DateTime`](#classscy_1_1DateTime)` & operator=(double julianDay)` | Assigns a Julian day.
`public `[`DateTime`](#classscy_1_1DateTime)` & assign(int year,int month,int day,int hour,int minute,int second,int millisecond,int microseconds)` | 
`public void swap(`[`DateTime`](#classscy_1_1DateTime)` & dateTime)` | Swaps the [DateTime](api-base.md#classscy_1_1DateTime) with another one.
`public inline int year() const` | Returns the year.
`public inline int month() const` | Returns the month (1 to 12).
`public int week(int firstDayOfWeek) const` | 
`public inline int day() const` | Returns the day witin the month (1 to 31).
`public int dayOfWeek() const` | 
`public int dayOfYear() const` | 
`public inline int hour() const` | Returns the hour (0 to 23).
`public inline int hourAMPM() const` | Returns the hour (0 to 12).
`public inline bool isAM() const` | Returns true if hour < 12;.
`public inline bool isPM() const` | Returns true if hour >= 12.
`public inline int minute() const` | Returns the minute (0 to 59).
`public inline int second() const` | Returns the second (0 to 59).
`public inline int millisecond() const` | Returns the millisecond (0 to 999)
`public inline int microsecond() const` | Returns the microsecond (0 to 999)
`public double julianDay() const` | Returns the julian day for the date and time.
`public inline `[`Timestamp`](#classscy_1_1Timestamp)` timestamp() const` | Returns the date and time expressed as a [Timestamp](api-base.md#classscy_1_1Timestamp).
`public inline `[`Timestamp::UtcTimeVal`](#group__base_1gaec8ace0f25db0ce91c10b30270c3dc24)` utcTime() const` | 
`public inline bool operator==(const `[`DateTime`](#classscy_1_1DateTime)` & dateTime) const` | 
`public inline bool operator!=(const `[`DateTime`](#classscy_1_1DateTime)` & dateTime) const` | 
`public inline bool operator<(const `[`DateTime`](#classscy_1_1DateTime)` & dateTime) const` | 
`public inline bool operator<=(const `[`DateTime`](#classscy_1_1DateTime)` & dateTime) const` | 
`public inline bool operator>(const `[`DateTime`](#classscy_1_1DateTime)` & dateTime) const` | 
`public inline bool operator>=(const `[`DateTime`](#classscy_1_1DateTime)` & dateTime) const` | 
`public `[`DateTime`](#classscy_1_1DateTime)` operator+(const `[`Timespan`](#classscy_1_1Timespan)` & span) const` | 
`public `[`DateTime`](#classscy_1_1DateTime)` operator-(const `[`Timespan`](#classscy_1_1Timespan)` & span) const` | 
`public `[`Timespan`](#classscy_1_1Timespan)` operator-(const `[`DateTime`](#classscy_1_1DateTime)` & dateTime) const` | 
`public `[`DateTime`](#classscy_1_1DateTime)` & operator+=(const `[`Timespan`](#classscy_1_1Timespan)` & span)` | 
`public `[`DateTime`](#classscy_1_1DateTime)` & operator-=(const `[`Timespan`](#classscy_1_1Timespan)` & span)` | 
`public void makeUTC(int tzd)` | Converts a local time into UTC, by applying the given time zone differential.
`public void makeLocal(int tzd)` | Converts a UTC time into a local time, by applying the given time zone differential.
`protected void computeGregorian(double julianDay)` | 
`protected void computeDaytime()` | Extracts the daytime (hours, minutes, seconds, etc.) from the stored utcTime.

## Members

#### `public  DateTime()` {#group__base_1gafc8db97ad1127e27ea8cc055d64bacdc}

Creates a [DateTime](#classscy_1_1DateTime) for the current date and time.



#### `public  DateTime(const `[`Timestamp`](#classscy_1_1Timestamp)` & timestamp)` {#group__base_1ga8757497c5f7df732cc6f84d2c31e997d}



Creates a [DateTime](#classscy_1_1DateTime) for the date and time given in a [Timestamp](#classscy_1_1Timestamp).

#### `public  DateTime(int year,int month,int day,int hour,int minute,int second,int millisecond,int microsecond)` {#group__base_1ga0723f5d0fb69a9f9f2231723b4712c2b}



Creates a [DateTime](#classscy_1_1DateTime) for the given Gregorian date and time.

* year is from 0 to 9999.


* month is from 1 to 12.


* day is from 1 to 31.


* hour is from 0 to 23.


* minute is from 0 to 59.


* second is from 0 to 59.


* millisecond is from 0 to 999.


* microsecond is from 0 to 999.

#### `public  DateTime(double julianDay)` {#group__base_1gaa8d974aeec2eaea35d48ac2860c6cfa6}

Creates a [DateTime](#classscy_1_1DateTime) for the given Julian day.



#### `public  DateTime(`[`Timestamp::UtcTimeVal`](#group__base_1gaec8ace0f25db0ce91c10b30270c3dc24)` utcTime,`[`Timestamp::TimeDiff`](#group__base_1ga9f06831e4e9b9cbc6f061cc61dc91fd0)` diff)` {#group__base_1gae8ca8fe73a37b81ea207f21115a3218a}



Creates a [DateTime](#classscy_1_1DateTime) from an UtcTimeVal and a TimeDiff.

Mainly used internally by [DateTime](#classscy_1_1DateTime) and friends.

#### `public  DateTime(const `[`DateTime`](#classscy_1_1DateTime)` & dateTime)` {#group__base_1gaa8c6129e0263af535cd47eace907e8da}

Copy constructor. Creates the [DateTime](#classscy_1_1DateTime) from another one.



#### `public  ~DateTime()` {#group__base_1gaab8ba2987dd09faf606927a8c5c8d57b}

Destroys the [DateTime](#classscy_1_1DateTime).



#### `public `[`DateTime`](#classscy_1_1DateTime)` & operator=(const `[`DateTime`](#classscy_1_1DateTime)` & dateTime)` {#group__base_1gaacf8c5cbf1e8b62928a397989844ca51}

Assigns another [DateTime](#classscy_1_1DateTime).



#### `public `[`DateTime`](#classscy_1_1DateTime)` & operator=(const `[`Timestamp`](#classscy_1_1Timestamp)` & timestamp)` {#group__base_1gae5bfb492a3b5469b672ab3c8c60ca2b4}

Assigns a [Timestamp](#classscy_1_1Timestamp).



#### `public `[`DateTime`](#classscy_1_1DateTime)` & operator=(double julianDay)` {#group__base_1ga40a6a5696a62415686d4ba0f8a36d52c}

Assigns a Julian day.



#### `public `[`DateTime`](#classscy_1_1DateTime)` & assign(int year,int month,int day,int hour,int minute,int second,int millisecond,int microseconds)` {#group__base_1ga60d82a180ce48c06261f324e8aa32615}



Assigns a Gregorian date and time.

* year is from 0 to 9999.


* month is from 1 to 12.


* day is from 1 to 31.


* hour is from 0 to 23.


* minute is from 0 to 59.


* second is from 0 to 59.


* millisecond is from 0 to 999.


* microsecond is from 0 to 999.

#### `public void swap(`[`DateTime`](#classscy_1_1DateTime)` & dateTime)` {#group__base_1ga2d52b10410733fc42b1d222b78b546f3}

Swaps the [DateTime](#classscy_1_1DateTime) with another one.



#### `public inline int year() const` {#group__base_1ga8ee24f2380b3a6f0728f1451cec5b846}

Returns the year.



#### `public inline int month() const` {#group__base_1ga36843329b8b306cb82a733b46f4d49c7}

Returns the month (1 to 12).



#### `public int week(int firstDayOfWeek) const` {#group__base_1gaf5ad099452341c3b5682a8f7352f8422}



Returns the week number within the year. FirstDayOfWeek should be either SUNDAY (0) or MONDAY (1). The returned week number will be from 0 to 53. Week number 1 is the week containing January 4. This is in accordance to ISO 8601.

The following example assumes that firstDayOfWeek is MONDAY. For 2005, which started on a Saturday, week 1 will be the week starting on Monday, January 3. January 1 and 2 will fall within week 0 (or the last week of the previous year).

For 2007, which starts on a Monday, week 1 will be the week startung on Monday, January 1. There will be no week 0 in 2007.

#### `public inline int day() const` {#group__base_1ga78029b135e06449cf535f2ae8c7ab497}

Returns the day witin the month (1 to 31).



#### `public int dayOfWeek() const` {#group__base_1ga2ea7a5f2314c97c367867fc550d61a5d}



Returns the weekday (0 to 6, where 0 = Sunday, 1 = Monday, ..., 6 = Saturday).

#### `public int dayOfYear() const` {#group__base_1gad9924403dc634f1e1ba699498d5c9c61}



Returns the number of the day in the year. January 1 is 1, February 1 is 32, etc.

#### `public inline int hour() const` {#group__base_1ga885ca6176737aff2dfa2443a10512c12}

Returns the hour (0 to 23).



#### `public inline int hourAMPM() const` {#group__base_1gad73305d05775a09b0fbd4c8db3c96923}

Returns the hour (0 to 12).



#### `public inline bool isAM() const` {#group__base_1gab9661f01a53099540cb71b23c0c1871d}

Returns true if hour < 12;.



#### `public inline bool isPM() const` {#group__base_1ga12d69cfbf155b6f294785aa0b9c49fe9}

Returns true if hour >= 12.



#### `public inline int minute() const` {#group__base_1gac124db7fe3f0fb9980087d09ed5abde6}

Returns the minute (0 to 59).



#### `public inline int second() const` {#group__base_1gab5062725edc5fb0dea2d9ea62dbdaa85}

Returns the second (0 to 59).



#### `public inline int millisecond() const` {#group__base_1gaa7bd963176ae54f61301fca30e02592a}

Returns the millisecond (0 to 999)



#### `public inline int microsecond() const` {#group__base_1gad4dbddbc0255cc26c9fd9dafaed353b9}

Returns the microsecond (0 to 999)



#### `public double julianDay() const` {#group__base_1ga48a04a535ae9290933994bfe0084b176}

Returns the julian day for the date and time.



#### `public inline `[`Timestamp`](#classscy_1_1Timestamp)` timestamp() const` {#group__base_1ga591c0712a8e1250820532510a54a49b3}

Returns the date and time expressed as a [Timestamp](#classscy_1_1Timestamp).



#### `public inline `[`Timestamp::UtcTimeVal`](#group__base_1gaec8ace0f25db0ce91c10b30270c3dc24)` utcTime() const` {#group__base_1gaa67be192392537cf1674d41f56704ab9}



Returns the date and time expressed in UTC-based time. UTC base time is midnight, October 15, 1582. Resolution is 100 nanoseconds.

#### `public inline bool operator==(const `[`DateTime`](#classscy_1_1DateTime)` & dateTime) const` {#group__base_1ga4262a88ba878383b2b4e0a1712b11649}





#### `public inline bool operator!=(const `[`DateTime`](#classscy_1_1DateTime)` & dateTime) const` {#group__base_1ga82a285c08ba0e9221127a4421211ba75}





#### `public inline bool operator<(const `[`DateTime`](#classscy_1_1DateTime)` & dateTime) const` {#group__base_1ga4968d25e134d2f9992866d797e27b7e3}





#### `public inline bool operator<=(const `[`DateTime`](#classscy_1_1DateTime)` & dateTime) const` {#group__base_1ga4bea2b52f94d03908e2b0e82c8f3bd29}





#### `public inline bool operator>(const `[`DateTime`](#classscy_1_1DateTime)` & dateTime) const` {#group__base_1ga35693ffc981a1608ed24246f1162ffa3}





#### `public inline bool operator>=(const `[`DateTime`](#classscy_1_1DateTime)` & dateTime) const` {#group__base_1ga2d2264fc51381c3f29b95e51433a96c1}





#### `public `[`DateTime`](#classscy_1_1DateTime)` operator+(const `[`Timespan`](#classscy_1_1Timespan)` & span) const` {#group__base_1gaed9c5e50f2543472847ca0675f6f87ca}





#### `public `[`DateTime`](#classscy_1_1DateTime)` operator-(const `[`Timespan`](#classscy_1_1Timespan)` & span) const` {#group__base_1ga46b10a0779e2e09a0be2630832412a47}





#### `public `[`Timespan`](#classscy_1_1Timespan)` operator-(const `[`DateTime`](#classscy_1_1DateTime)` & dateTime) const` {#group__base_1gaae40c16a2b2f6f2c63e93efe904d38dd}





#### `public `[`DateTime`](#classscy_1_1DateTime)` & operator+=(const `[`Timespan`](#classscy_1_1Timespan)` & span)` {#group__base_1gaa56672df917ae0e0e5a02f73a3f1e94a}





#### `public `[`DateTime`](#classscy_1_1DateTime)` & operator-=(const `[`Timespan`](#classscy_1_1Timespan)` & span)` {#group__base_1gadb7d7dc23a9eaa6e054f2383a21bb388}





#### `public void makeUTC(int tzd)` {#group__base_1gaef85751cde47a0e909aeddae1775b805}

Converts a local time into UTC, by applying the given time zone differential.



#### `public void makeLocal(int tzd)` {#group__base_1ga82ce62343d007567544418aed404dcd9}

Converts a UTC time into a local time, by applying the given time zone differential.



#### `protected void computeGregorian(double julianDay)` {#group__base_1ga6f59bebcde64cc028cdbcde61b7eb619}



Computes the Gregorian date for the given Julian day. See [http://vsg.cape.com/~pbaum/date/injdimp.htm](http://vsg.cape.com/~pbaum/date/injdimp.htm), section 3.3.1 for the algorithm.

#### `protected void computeDaytime()` {#group__base_1ga57cdba4dec7b53c448c56545d0c0a8fb}

Extracts the daytime (hours, minutes, seconds, etc.) from the stored utcTime.



# class `scy::DateTimeFormat` {#classscy_1_1DateTimeFormat}




Definition of date/time formats and various constants used by [DateTimeFormatter](#classscy_1_1DateTimeFormatter) and [DateTimeParser](#classscy_1_1DateTimeParser).

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------

## Members

# class `scy::DateTimeFormatter` {#classscy_1_1DateTimeFormatter}




This class converts dates and times into strings, supporting a variety of standard and custom formats.

There are two kind of static member functions: /// * format* functions return a std::string containing the formatted value.

* append* functions append the formatted value to an existing string.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------

## Members

# class `scy::DateTimeParser` {#classscy_1_1DateTimeParser}




This class provides a method for parsing dates and times from strings. All parsing methods do their best to parse a meaningful result, even from malformed input strings.

The returned [DateTime](#classscy_1_1DateTime) will always contain a time in the same timezone as the time in the string. Call [DateTime::makeUTC()](#group__base_1gaef85751cde47a0e909aeddae1775b805) /// with the timeZoneDifferential returned by [parse()](#group__base_1ga7249fe6e774b64ab401a6f3f97311f26) to convert the [DateTime](#classscy_1_1DateTime) to UTC.

Note: When parsing a time in 12-hour (AM/PM) format, the hour (h) must be parsed before the AM/PM designator (a, A), otherwise the AM/PM designator will be ignored.

See the [DateTimeFormatter](#classscy_1_1DateTimeFormatter) class for a list of supported format specifiers. In addition to the format specifiers supported by [DateTimeFormatter](#classscy_1_1DateTimeFormatter), an additional specifier is supported: r will parse a year given by either two or four digits. Years 69-00 are interpreted in the 20th century (1969-2000), years 01-68 in the 21th century (2001-2068).

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------

## Members

# class `scy::Delegate` {#classscy_1_1Delegate}

```
class scy::Delegate
  : public BaseT
  : public CallbackT
```  



This template class implements an adapter that sits between an [DelegateBase](#structscy_1_1DelegateBase) and an object receiving notifications from it.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  Delegate(C * object,Method method,int priority)` | 
`public inline  Delegate(C * object,Method method,DataT filter,int priority)` | 
`public inline  Delegate(const `[`Delegate`](#classscy_1_1Delegate)` & r)` | 
`public inline virtual  ~Delegate()` | 
`public inline BaseT * clone() const` | 
`public inline void emit(void * sender,P arg,P2 arg2,P3 arg3,P4 arg4)` | 
`public inline bool equals(const `[`DerivedT`](#structscy_1_1DelegateBase)` * r) const` | 
`public inline void cancel()` | 
`public inline bool cancelled() const` | 
`public inline int priority() const` | 
`public inline void * object() const` | 
`protected int _priority` | 
`protected bool _cancelled` | 
`protected  Delegate()` | 

## Members

#### `public inline  Delegate(C * object,Method method,int priority)` {#group__base_1gab8fe434bb87b6b0af083e6b2331c3a48}





#### `public inline  Delegate(C * object,Method method,DataT filter,int priority)` {#group__base_1ga1f6f4c4d6274b7afcae81b5d9dc8332f}





#### `public inline  Delegate(const `[`Delegate`](#classscy_1_1Delegate)` & r)` {#group__base_1ga33c8e032c356077eb029f90f0770b045}





#### `public inline virtual  ~Delegate()` {#group__base_1ga3bb9af8606a2d631c79c1ad34800f9d6}





#### `public inline BaseT * clone() const` {#group__base_1ga9bf5e7c29c855ed535c10f06db950f07}





#### `public inline void emit(void * sender,P arg,P2 arg2,P3 arg3,P4 arg4)` {#group__base_1ga14f53a982107849b9c71b2d720f0559e}





#### `public inline bool equals(const `[`DerivedT`](#structscy_1_1DelegateBase)` * r) const` {#group__base_1ga8118703891230eac10c0d9e4243d2a81}





#### `public inline void cancel()` {#group__base_1ga8b06fc982a682495cab824fc14f4dc4d}





#### `public inline bool cancelled() const` {#group__base_1ga49dfcd3ecd11317d902ec8c396719720}





#### `public inline int priority() const` {#group__base_1ga9c200a7f249213114e87e66c9e7f83ed}





#### `public inline void * object() const` {#group__base_1gaf6b4d37aff53862519fc4839105632b5}





#### `protected int _priority` {#group__base_1ga1cd6d173a1a436bf69f186acc86ff22e}





#### `protected bool _cancelled` {#group__base_1ga15434e40cf6c6e6a7a7857a75cf05794}





#### `protected  Delegate()` {#group__base_1gab4e199da6d67e1ab2fc447b94f556ded}





# class `scy::FileChannel` {#classscy_1_1FileChannel}

```
class scy::FileChannel
  : public scy::LogChannel
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  FileChannel(const std::string & name,const std::string & path,LogLevel level,const char * timeFormat)` | 
`public virtual  ~FileChannel()` | 
`public virtual void write(const `[`LogStream`](#structscy_1_1LogStream)` & stream)` | 
`public void setPath(const std::string & path)` | 
`public std::string path() const` | 
`protected std::ofstream _fstream` | 
`protected std::string _path` | 
`protected virtual void open()` | 
`protected virtual void close()` | 

## Members

#### `public  FileChannel(const std::string & name,const std::string & path,LogLevel level,const char * timeFormat)` {#group__base_1ga7257231cb76c22937760b751d12cb7de}





#### `public virtual  ~FileChannel()` {#group__base_1gaeb8131af118653a2b09bca46660fc2b1}





#### `public virtual void write(const `[`LogStream`](#structscy_1_1LogStream)` & stream)` {#group__base_1gab47bd04bc5248298db9e2c5bf53bc5d6}





#### `public void setPath(const std::string & path)` {#group__base_1ga2ae18223e682fc74ced9f079b9473f64}





#### `public std::string path() const` {#group__base_1ga4399a5666e22bab99beba14926b6db56}





#### `protected std::ofstream _fstream` {#group__base_1ga4f6cafce9cad0d449da791f4a8ff6416}





#### `protected std::string _path` {#group__base_1gaee5340f4b20b5361e05e84cb3eca9fd3}





#### `protected virtual void open()` {#group__base_1gac96f91229ba9e1d0566f6402aae8c294}





#### `protected virtual void close()` {#group__base_1ga69982cecf5699940fd17ef9852d88ea8}





# class `scy::FlagPacket` {#classscy_1_1FlagPacket}

```
class scy::FlagPacket
  : public scy::IPacket
```  

A simple flag packet for sending state flags along the packet stream.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  FlagPacket(unsigned flags)` | 
`public inline virtual `[`IPacket`](#classscy_1_1IPacket)` * clone() const` | 
`public inline  FlagPacket(const `[`FlagPacket`](#classscy_1_1FlagPacket)` & that)` | 
`public inline virtual  ~FlagPacket()` | 
`public inline virtual std::size_t read(const `[`ConstBuffer`](#classscy_1_1ConstBuffer)` &)` | 
`public inline virtual void write(Buffer &) const` | 
`public inline virtual const char * className() const` | 

## Members

#### `public inline  FlagPacket(unsigned flags)` {#group__base_1ga653e36ffe31957974caf6d74eac86ada}





#### `public inline virtual `[`IPacket`](#classscy_1_1IPacket)` * clone() const` {#group__base_1ga58a9b4e1cc6e2be401e99499519bde60}





#### `public inline  FlagPacket(const `[`FlagPacket`](#classscy_1_1FlagPacket)` & that)` {#group__base_1ga2fe2b6ce02c8780ccb958867d7b40f68}





#### `public inline virtual  ~FlagPacket()` {#group__base_1ga8da82044375fedaf8fe761ac0053a234}





#### `public inline virtual std::size_t read(const `[`ConstBuffer`](#classscy_1_1ConstBuffer)` &)` {#group__base_1ga83a6b0d0eebf9f0331b55c771c15caee}



Read/parse to the packet from the given input buffer. The number of bytes read is returned.

#### `public inline virtual void write(Buffer &) const` {#group__base_1ga95f01a2522cd11c4d92d78866f8f1386}



Copy/generate to the packet given output buffer. The number of bytes written can be obtained from the buffer.

Todo: It may be prefferable to use our pod types here instead of buffer input, but the current codebase requires that the buffer be dynamically resizable for some protocols...

virtual std::size_t write(MutableBuffer&) const = 0;

#### `public inline virtual const char * className() const` {#group__base_1ga1222400c48dd7d23fed666fbaa32d0e9}





# class `scy::GarbageCollector` {#classscy_1_1GarbageCollector}


Simple garbage collector for deferred pointer deletion.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  GarbageCollector()` | 
`public  ~GarbageCollector()` | 
`public template<class C>`  <br/>`inline void deleteLater(C * ptr)` | Schedules a pointer for deferred deletion.
`public template<class C>`  <br/>`inline void deleteLater(std::shared_ptr< C > ptr)` | Schedules a shared pointer for deferred deletion.
`public void finalize()` | 
`public uv_thread_t tid()` | 
`protected mutable `[`Mutex`](api-thread.md#classscy_1_1Mutex)` _mutex` | 
`protected std::vector< `[`ScopedPointer`](api-base.md#classscy_1_1ScopedPointer)` * > _pending` | 
`protected std::vector< `[`ScopedPointer`](api-base.md#classscy_1_1ScopedPointer)` * > _ready` | 
`protected `[`uv::Handle`](api-uv.md#classscy_1_1uv_1_1Handle)` _handle` | 
`protected bool _finalize` | 
`protected uv_thread_t _tid` | 
`protected void runAsync()` | 

## Members

#### `public  GarbageCollector()` {#group__base_1ga04e0caf7b61b22f3852975b101c39994}





#### `public  ~GarbageCollector()` {#group__base_1ga92874d6c0e6be122c5f59bfb54a034de}





#### `public template<class C>`  <br/>`inline void deleteLater(C * ptr)` {#group__base_1ga0cbf2dcb3911078614e03dcce33a2469}

Schedules a pointer for deferred deletion.



#### `public template<class C>`  <br/>`inline void deleteLater(std::shared_ptr< C > ptr)` {#group__base_1gac09d93a4f13e10e3a2d83c7661764284}

Schedules a shared pointer for deferred deletion.



#### `public void finalize()` {#group__base_1ga303cf004c714fc26cf693d7b97ed7fe9}



Frees all scheduled pointers now. This method must be called from the main thread while the event loop is inactive.

#### `public uv_thread_t tid()` {#group__base_1gaac5ea4199988e13870e7aba4faa18ad5}



Returns the TID of the garbage collector event loop thread. The garbage collector must be running.

#### `protected mutable `[`Mutex`](api-thread.md#classscy_1_1Mutex)` _mutex` {#group__base_1gacee5187f4d83b8d963e7f731fda5037a}





#### `protected std::vector< `[`ScopedPointer`](api-base.md#classscy_1_1ScopedPointer)` * > _pending` {#group__base_1gaa199630597b6523bc7903ef432780389}





#### `protected std::vector< `[`ScopedPointer`](api-base.md#classscy_1_1ScopedPointer)` * > _ready` {#group__base_1ga547d312fc9d30911593ff2139dac4486}





#### `protected `[`uv::Handle`](api-uv.md#classscy_1_1uv_1_1Handle)` _handle` {#group__base_1gabdb091600aa01b7a04295b6e6282dfb4}





#### `protected bool _finalize` {#group__base_1ga834187a02487cbcb4172d6d4f8d90ba8}





#### `protected uv_thread_t _tid` {#group__base_1gac01d7ae0110fef6d14f64a3570cd990d}





#### `protected void runAsync()` {#group__base_1gab999ad3be1e615244ae7bf35714ab4a7}





# class `scy::Idler` {#classscy_1_1Idler}

```
class scy::Idler
  : public scy::async::Runner
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Idler(uv::Loop * loop)` | Create the idler context the given event loop and method.
`public  Idler(uv::Loop * loop,std::function< void()> target)` | 
`public  Idler(uv::Loop * loop,std::function< void(void *)> target,void * arg)` | 
`public virtual  ~Idler()` | 
`public `[`uv::Handle`](#classscy_1_1uv_1_1Handle)` & handle()` | 
`protected `[`uv::Handle`](api-uv.md#classscy_1_1uv_1_1Handle)` _handle` | 
`protected virtual void init()` | 
`protected virtual void startAsync()` | Start the context from the control thread.
`protected virtual bool async() const` | 

## Members

#### `public  Idler(uv::Loop * loop)` {#group__base_1ga739bc010da58a894a3645ff96a4d952b}

Create the idler context the given event loop and method.



#### `public  Idler(uv::Loop * loop,std::function< void()> target)` {#group__base_1gac554ea4755d55c6cc5bf6360f614b9aa}





#### `public  Idler(uv::Loop * loop,std::function< void(void *)> target,void * arg)` {#group__base_1ga8631aef145650681bdfd3153443b802f}





#### `public virtual  ~Idler()` {#group__base_1gacaa644f9f9c9a9b8208abe678ab5535e}





#### `public `[`uv::Handle`](#classscy_1_1uv_1_1Handle)` & handle()` {#group__base_1gafddee059e7bc36a1f3397646321c652f}





#### `protected `[`uv::Handle`](api-uv.md#classscy_1_1uv_1_1Handle)` _handle` {#group__base_1gaedd4a0fb85911ee27eb0c1caa38fca80}





#### `protected virtual void init()` {#group__base_1ga331ca6dc143e20e5520d09d6db3e6f46}





#### `protected virtual void startAsync()` {#group__base_1ga934e4320fbd87a769f25d399ed3580c4}

Start the context from the control thread.



#### `protected virtual bool async() const` {#group__base_1ga96ac33176eb28672a996260623ff99cd}



Returns true if the implementation is thread-based, or false if it belongs to an event loop.

# class `scy::IPacket` {#classscy_1_1IPacket}

```
class scy::IPacket
  : public scy::basic::Polymorphic
```  



The basic packet type which is passed around the LibSourcey system. [IPacket](#classscy_1_1IPacket) can be extended for each protocol to enable polymorphic processing and callbacks using [PacketStream](#classscy_1_1PacketStream) and friends.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public void * source` | 
`public void * opaque` | 
`public `[`IPacketInfo`](api-base.md#structscy_1_1IPacketInfo)` * info` | 
`public `[`Bitwise`](api-base.md#structscy_1_1Bitwise)` flags` | Provides basic information about the packet.
`public inline  IPacket(void * source,void * opaque,`[`IPacketInfo`](#structscy_1_1IPacketInfo)` * info,unsigned flags)` | 
`public inline  IPacket(const `[`IPacket`](#classscy_1_1IPacket)` & r)` | 
`public inline `[`IPacket`](#classscy_1_1IPacket)` & operator=(const `[`IPacket`](#classscy_1_1IPacket)` & r)` | 
`public `[`IPacket`](#classscy_1_1IPacket)` * clone() const` | 
`public inline virtual  ~IPacket()` | 
`public std::size_t read(const `[`ConstBuffer`](#classscy_1_1ConstBuffer)` &)` | 
`public void write(Buffer &) const` | 
`public inline virtual std::size_t size() const` | 
`public inline virtual bool hasData() const` | 
`public inline virtual char * data() const` | The packet data pointer for buffered packets.
`public inline virtual const char * constData() const` | The const packet data pointer for buffered packets.
`public const char * className() const` | 
`public inline virtual void print(std::ostream & os) const` | 

## Members

#### `public void * source` {#group__base_1gad3f9d5b27d476a5e693dd752036e2aa9}



Packet source pointer reference which enables processors along the signal chain can determine the packet origin. Often a subclass of PacketStreamSource.

#### `public void * opaque` {#group__base_1gaf6b50b2656e3dfc60907b366b35fb1c0}



Optional client data pointer. This pointer is not managed by the packet.

#### `public `[`IPacketInfo`](api-base.md#structscy_1_1IPacketInfo)` * info` {#group__base_1gaa1d28787c49a63b8390561fbfe520ca0}



Optional extra information about the packet. This pointer is managed by the packet.

#### `public `[`Bitwise`](api-base.md#structscy_1_1Bitwise)` flags` {#group__base_1ga9eb92d88fc9705ff304ddd382c7588fa}

Provides basic information about the packet.



#### `public inline  IPacket(void * source,void * opaque,`[`IPacketInfo`](#structscy_1_1IPacketInfo)` * info,unsigned flags)` {#group__base_1ga407f72508b7ce034d083953665eb8780}





#### `public inline  IPacket(const `[`IPacket`](#classscy_1_1IPacket)` & r)` {#group__base_1gaadf363596a491bcfa114040d3e5f1ce5}





#### `public inline `[`IPacket`](#classscy_1_1IPacket)` & operator=(const `[`IPacket`](#classscy_1_1IPacket)` & r)` {#group__base_1ga117e052bf06015011564ebbb1dc771e1}





#### `public `[`IPacket`](#classscy_1_1IPacket)` * clone() const` {#group__base_1ga38148136d298669f0ecba4780c34cc10}





#### `public inline virtual  ~IPacket()` {#group__base_1ga82474cf5612f702064c02744fb3fd580}





#### `public std::size_t read(const `[`ConstBuffer`](#classscy_1_1ConstBuffer)` &)` {#group__base_1ga0886a2812d86d64bbb47b50788cb791a}



Read/parse to the packet from the given input buffer. The number of bytes read is returned.

#### `public void write(Buffer &) const` {#group__base_1gabdcb765c58d74be25e190680bf7483d6}



Copy/generate to the packet given output buffer. The number of bytes written can be obtained from the buffer.

Todo: It may be prefferable to use our pod types here instead of buffer input, but the current codebase requires that the buffer be dynamically resizable for some protocols...

virtual std::size_t write(MutableBuffer&) const = 0;

#### `public inline virtual std::size_t size() const` {#group__base_1ga49cbf8b6c2a0f4529f03e11311bd29f4}



The size of the packet in bytes.

This is the nember of bytes that will be written on a call to [write()](#group__base_1gabdcb765c58d74be25e190680bf7483d6), but may not be the number of bytes that will be consumed by [read()](#group__base_1ga0886a2812d86d64bbb47b50788cb791a).

#### `public inline virtual bool hasData() const` {#group__base_1ga4fc1159e6f71b7231f8de2220f6f0a83}





#### `public inline virtual char * data() const` {#group__base_1ga9d816749f7c3846f823c945282254359}

The packet data pointer for buffered packets.



#### `public inline virtual const char * constData() const` {#group__base_1ga6b073b2c2018bf15374380e236ddf4ca}

The const packet data pointer for buffered packets.



#### `public const char * className() const` {#group__base_1ga2f2115f313779ef2cb9d9525cc522e93}





#### `public inline virtual void print(std::ostream & os) const` {#group__base_1gad23b82ac26af7d46ca3828f1116ee9d7}





# class `scy::IPacketCreationStrategy` {#classscy_1_1IPacketCreationStrategy}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  IPacketCreationStrategy()` | 
`public inline virtual  ~IPacketCreationStrategy()` | 
`public `[`IPacket`](#classscy_1_1IPacket)` * create(const `[`ConstBuffer`](#classscy_1_1ConstBuffer)` & buffer,std::size_t & nread) const` | 
`public int priority() const` | 

## Members

#### `public inline  IPacketCreationStrategy()` {#group__base_1ga50f56327263cb8d8ee572496ea9a7a3b}





#### `public inline virtual  ~IPacketCreationStrategy()` {#group__base_1ga2c5055fba7c0c0dbd6fbb6d05a338811}





#### `public `[`IPacket`](#classscy_1_1IPacket)` * create(const `[`ConstBuffer`](#classscy_1_1ConstBuffer)` & buffer,std::size_t & nread) const` {#group__base_1ga4938009d616726597582d7e6f5cfb714}





#### `public int priority() const` {#group__base_1gaba808d34edb6272253418c30210f25ec}





# class `scy::KVCollection` {#classscy_1_1KVCollection}


A reusable stack based unique key-value store for DRY coding.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  KVCollection()` | 
`public inline virtual  ~KVCollection()` | 
`public inline virtual bool add(const TKey & key,const TValue & item,bool update,bool whiny)` | 
`public inline virtual TValue & get(const TKey & key)` | 
`public inline virtual const TValue & get(const TKey & key,const TValue & defaultValue) const` | 
`public inline virtual bool remove(const TKey & key)` | 
`public inline virtual bool has(const TKey & key) const` | 
`public inline virtual bool empty() const` | 
`public inline virtual int size() const` | 
`public inline virtual void clear()` | 
`public inline virtual Map & map()` | 
`protected Map _map` | 

## Members

#### `public inline  KVCollection()` {#group__base_1ga91a2efa7efe06f01164d4cf805381851}





#### `public inline virtual  ~KVCollection()` {#group__base_1gac9ac9c331dc55310741b1f3b87b5269a}





#### `public inline virtual bool add(const TKey & key,const TValue & item,bool update,bool whiny)` {#group__base_1gaee5cb65f2a4800e23984ac012fc50c73}





#### `public inline virtual TValue & get(const TKey & key)` {#group__base_1gaf0a54f73e168bc89a4d02147f7fe612c}





#### `public inline virtual const TValue & get(const TKey & key,const TValue & defaultValue) const` {#group__base_1gaa5e03985383fbb0811c900f6ffa01013}





#### `public inline virtual bool remove(const TKey & key)` {#group__base_1ga3fdb425c32ea35e45a5577955ec3bdd4}





#### `public inline virtual bool has(const TKey & key) const` {#group__base_1ga8b8c96f803ae6b0c701fd02e29104f66}





#### `public inline virtual bool empty() const` {#group__base_1ga6f9493de942d040964118ce00feb208a}





#### `public inline virtual int size() const` {#group__base_1ga3b4fe86a7036bc947b0fe2351b4a8786}





#### `public inline virtual void clear()` {#group__base_1gaeb411422a7704fd35374f0d931095e69}





#### `public inline virtual Map & map()` {#group__base_1ga680655a3535b0fffc8a62362863294b7}





#### `protected Map _map` {#group__base_1gaacf1b4202998f2ba32b06e6e340c1643}





# class `scy::LiveCollection` {#classscy_1_1LiveCollection}

```
class scy::LiveCollection
  : public scy::PointerCollection< TKey, TValue, TDeleter >
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`Signal`](api-base.md#classscy_1_1Signal)`< TValue & > ItemAdded` | 
`public `[`Signal`](api-base.md#classscy_1_1Signal)`< const TValue & > ItemRemoved` | 
`public inline virtual void onAdd(const TKey &,TValue * item)` | 
`public inline virtual void onRemove(const TKey &,TValue * item)` | 

## Members

#### `public `[`Signal`](api-base.md#classscy_1_1Signal)`< TValue & > ItemAdded` {#group__base_1ga2bce4b14de9a35658465803125cd90b9}





#### `public `[`Signal`](api-base.md#classscy_1_1Signal)`< const TValue & > ItemRemoved` {#group__base_1gaa5dc0ee0647127d7e19d4a677c5c4dc0}





#### `public inline virtual void onAdd(const TKey &,TValue * item)` {#group__base_1ga2f465a87236279b8fa466012cc1bcce8}





#### `public inline virtual void onRemove(const TKey &,TValue * item)` {#group__base_1ga6ebf9c21040f892591120b7f3172858d}





# class `scy::LocalDateTime` {#classscy_1_1LocalDateTime}




This class represents an instant in local time (as opposed to UTC), expressed in years, months, days, hours, minutes, seconds and milliseconds based on the Gregorian calendar.

In addition to the date and time, the class also maintains a time zone differential, which denotes the difference in seconds from UTC to local time, i.e. UTC = local time - time zone differential. Although [LocalDateTime](#classscy_1_1LocalDateTime) supports relational and arithmetic operators, all date/time comparisons and date/time arithmetics should be done in UTC, using the [DateTime](#classscy_1_1DateTime) or [Timestamp](#classscy_1_1Timestamp) class for better performance. The relational operators normalize the dates/times involved to UTC before carrying out the comparison.

The time zone differential is based on the input date and time and current time zone. A number of constructors accept an explicit time zone differential parameter. These should not be used since daylight savings time processing is impossible since the time zone is unknown. Each of the constructors accepting a tzd parameter have been marked as deprecated and may be removed in a future revision.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  LocalDateTime()` | 
`public  LocalDateTime(int year,int month,int day,int hour,int minute,int second,int millisecond,int microsecond)` | 
`public  LocalDateTime(int tzd,int year,int month,int day,int hour,int minute,int second,int millisecond,int microsecond)` | 
`public  LocalDateTime(const `[`DateTime`](#classscy_1_1DateTime)` & dateTime)` | 
`public  LocalDateTime(int tzd,const `[`DateTime`](#classscy_1_1DateTime)` & dateTime)` | 
`public  LocalDateTime(int tzd,const `[`DateTime`](#classscy_1_1DateTime)` & dateTime,bool adjust)` | 
`public  LocalDateTime(double julianDay)` | Creates a [LocalDateTime](api-base.md#classscy_1_1LocalDateTime) for the given Julian day in the local time zone.
`public  LocalDateTime(int tzd,double julianDay)` | 
`public  LocalDateTime(const `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & dateTime)` | Copy constructor. Creates the [LocalDateTime](api-base.md#classscy_1_1LocalDateTime) from another one.
`public  ~LocalDateTime()` | Destroys the [LocalDateTime](api-base.md#classscy_1_1LocalDateTime).
`public `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & operator=(const `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & dateTime)` | Assigns another [LocalDateTime](api-base.md#classscy_1_1LocalDateTime).
`public `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & operator=(const `[`Timestamp`](#classscy_1_1Timestamp)` & timestamp)` | Assigns a timestamp.
`public `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & operator=(double julianDay)` | Assigns a Julian day in the local time zone.
`public `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & assign(int year,int month,int day,int hour,int minute,int second,int millisecond,int microseconds)` | 
`public `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & assign(int tzd,int year,int month,int day,int hour,int minute,int second,int millisecond,int microseconds)` | 
`public `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & assign(int tzd,double julianDay)` | 
`public void swap(`[`LocalDateTime`](#classscy_1_1LocalDateTime)` & dateTime)` | Swaps the [LocalDateTime](api-base.md#classscy_1_1LocalDateTime) with another one.
`public inline int year() const` | Returns the year.
`public inline int month() const` | Returns the month (1 to 12).
`public inline int week(int firstDayOfWeek) const` | 
`public inline int day() const` | Returns the day witin the month (1 to 31).
`public inline int dayOfWeek() const` | 
`public inline int dayOfYear() const` | 
`public inline int hour() const` | Returns the hour (0 to 23).
`public inline int hourAMPM() const` | Returns the hour (0 to 12).
`public inline bool isAM() const` | Returns true if hour < 12;.
`public inline bool isPM() const` | Returns true if hour >= 12.
`public inline int minute() const` | Returns the minute (0 to 59).
`public inline int second() const` | Returns the second (0 to 59).
`public inline int millisecond() const` | Returns the millisecond (0 to 999)
`public inline int microsecond() const` | Returns the microsecond (0 to 999)
`public inline double julianDay() const` | Returns the julian day for the date.
`public inline int tzd() const` | Returns the time zone differential.
`public `[`DateTime`](#classscy_1_1DateTime)` utc() const` | Returns the UTC equivalent for the local date and time.
`public inline `[`Timestamp`](#classscy_1_1Timestamp)` timestamp() const` | Returns the date and time expressed as a [Timestamp](api-base.md#classscy_1_1Timestamp).
`public inline `[`Timestamp::UtcTimeVal`](#group__base_1gaec8ace0f25db0ce91c10b30270c3dc24)` utcTime() const` | Returns the UTC equivalent for the local date and time.
`public bool operator==(const `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & dateTime) const` | 
`public bool operator!=(const `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & dateTime) const` | 
`public bool operator<(const `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & dateTime) const` | 
`public bool operator<=(const `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & dateTime) const` | 
`public bool operator>(const `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & dateTime) const` | 
`public bool operator>=(const `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & dateTime) const` | 
`public `[`LocalDateTime`](#classscy_1_1LocalDateTime)` operator+(const `[`Timespan`](#classscy_1_1Timespan)` & span) const` | 
`public `[`LocalDateTime`](#classscy_1_1LocalDateTime)` operator-(const `[`Timespan`](#classscy_1_1Timespan)` & span) const` | 
`public `[`Timespan`](#classscy_1_1Timespan)` operator-(const `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & dateTime) const` | 
`public `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & operator+=(const `[`Timespan`](#classscy_1_1Timespan)` & span)` | 
`public `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & operator-=(const `[`Timespan`](#classscy_1_1Timespan)` & span)` | 
`protected  LocalDateTime(`[`Timestamp::UtcTimeVal`](#group__base_1gaec8ace0f25db0ce91c10b30270c3dc24)` utcTime,`[`Timestamp::TimeDiff`](#group__base_1ga9f06831e4e9b9cbc6f061cc61dc91fd0)` diff,int tzd)` | 
`protected void determineTzd(bool adjust)` | 
`protected inline void adjustForTzd()` | Adjust the _dateTime member based on the _tzd member.
`protected std::time_t dstOffset(int & dstOffset) const` | Determine the DST offset for the current date/time.

## Members

#### `public  LocalDateTime()` {#group__base_1gaf8f507cd5b10d8089d263b12e34e14fc}



Creates a [LocalDateTime](#classscy_1_1LocalDateTime) with the current date/time for the current time zone.

#### `public  LocalDateTime(int year,int month,int day,int hour,int minute,int second,int millisecond,int microsecond)` {#group__base_1gaa6b69743391798f50ba00facb3b344b8}



Creates a [LocalDateTime](#classscy_1_1LocalDateTime) for the given Gregorian local date and time.

* year is from 0 to 9999.


* month is from 1 to 12.


* day is from 1 to 31.


* hour is from 0 to 23.


* minute is from 0 to 59.


* second is from 0 to 59.


* millisecond is from 0 to 999.


* microsecond is from 0 to 999.

#### `public  LocalDateTime(int tzd,int year,int month,int day,int hour,int minute,int second,int millisecond,int microsecond)` {#group__base_1gac25cdf7cf37094a68feccb09da251a63}



@ deprecated Creates a [LocalDateTime](#classscy_1_1LocalDateTime) for the given Gregorian date and time in the time zone denoted by the time zone differential in tzd.

* tzd is in seconds.


* year is from 0 to 9999.


* month is from 1 to 12.


* day is from 1 to 31.


* hour is from 0 to 23.


* minute is from 0 to 59.


* second is from 0 to 59.


* millisecond is from 0 to 999.


* microsecond is from 0 to 999.

#### `public  LocalDateTime(const `[`DateTime`](#classscy_1_1DateTime)` & dateTime)` {#group__base_1ga8541afcbe851e21828977693c1130b5b}



Creates a [LocalDateTime](#classscy_1_1LocalDateTime) from the UTC time given in dateTime, using the time zone differential of the current time zone.

#### `public  LocalDateTime(int tzd,const `[`DateTime`](#classscy_1_1DateTime)` & dateTime)` {#group__base_1gab8d6a8ba8d4ce3c43821dc4d8561aa0d}



@ deprecated Creates a [LocalDateTime](#classscy_1_1LocalDateTime) from the UTC time given in dateTime, using the given time zone differential. Adjusts dateTime for the given time zone differential.

#### `public  LocalDateTime(int tzd,const `[`DateTime`](#classscy_1_1DateTime)` & dateTime,bool adjust)` {#group__base_1ga8aaa4e3e37cbac08e70d41e9eff399d0}



@ deprecated Creates a [LocalDateTime](#classscy_1_1LocalDateTime) from the UTC time given in dateTime, using the given time zone differential. If adjust is true, adjusts dateTime for the given time zone differential.

#### `public  LocalDateTime(double julianDay)` {#group__base_1gabe29c1922e6895d2d3b7aa1a01c5a40a}

Creates a [LocalDateTime](#classscy_1_1LocalDateTime) for the given Julian day in the local time zone.



#### `public  LocalDateTime(int tzd,double julianDay)` {#group__base_1ga20abd6e936deee4ce9d14af620c008d7}



@ deprecated Creates a [LocalDateTime](#classscy_1_1LocalDateTime) for the given Julian day in the time zone denoted by the time zone differential in tzd.

#### `public  LocalDateTime(const `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & dateTime)` {#group__base_1gae63da0ed0e40399b8986a776a3043053}

Copy constructor. Creates the [LocalDateTime](#classscy_1_1LocalDateTime) from another one.



#### `public  ~LocalDateTime()` {#group__base_1ga173cd6cde153c287ed6c83691329be4e}

Destroys the [LocalDateTime](#classscy_1_1LocalDateTime).



#### `public `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & operator=(const `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & dateTime)` {#group__base_1ga6e18acbc0937ed82dc958f9372504e33}

Assigns another [LocalDateTime](#classscy_1_1LocalDateTime).



#### `public `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & operator=(const `[`Timestamp`](#classscy_1_1Timestamp)` & timestamp)` {#group__base_1gafa3356c42a95457524c27c2be9a717fd}

Assigns a timestamp.



#### `public `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & operator=(double julianDay)` {#group__base_1ga344e356fccab4631853d9f5cd37de8c6}

Assigns a Julian day in the local time zone.



#### `public `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & assign(int year,int month,int day,int hour,int minute,int second,int millisecond,int microseconds)` {#group__base_1gaac91b6bebab01e9b755e5b72e9137107}



Assigns a Gregorian local date and time.

* year is from 0 to 9999.


* month is from 1 to 12.


* day is from 1 to 31.


* hour is from 0 to 23.


* minute is from 0 to 59.


* second is from 0 to 59.


* millisecond is from 0 to 999.


* microsecond is from 0 to 999.

#### `public `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & assign(int tzd,int year,int month,int day,int hour,int minute,int second,int millisecond,int microseconds)` {#group__base_1ga5a05f078a0a5b372ad43aad8f8505a77}



@ deprecated Assigns a Gregorian local date and time in the time zone denoted by the time zone differential in tzd.

* tzd is in seconds.


* year is from 0 to 9999.


* month is from 1 to 12.


* day is from 1 to 31.


* hour is from 0 to 23.


* minute is from 0 to 59.


* second is from 0 to 59.


* millisecond is from 0 to 999.


* microsecond is from 0 to 999.

#### `public `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & assign(int tzd,double julianDay)` {#group__base_1ga1ad03bada11f55c902dbec05d2fc20ba}



@ deprecated Assigns a Julian day in the time zone denoted by the time zone differential in tzd.

#### `public void swap(`[`LocalDateTime`](#classscy_1_1LocalDateTime)` & dateTime)` {#group__base_1ga6bc1c879e4d7b3b04a976c1161ecd5fc}

Swaps the [LocalDateTime](#classscy_1_1LocalDateTime) with another one.



#### `public inline int year() const` {#group__base_1ga8b67a920a92e22ea2b6d4e8a306566c9}

Returns the year.



#### `public inline int month() const` {#group__base_1ga8bb9094aac43c17269669b6422448932}

Returns the month (1 to 12).



#### `public inline int week(int firstDayOfWeek) const` {#group__base_1gaad9976490f29168f69b842eda06653cc}



Returns the week number within the year. FirstDayOfWeek should be either SUNDAY (0) or MONDAY (1). The returned week number will be from 0 to 53. Week number 1 is the week containing January 4. This is in accordance to ISO 8601.

The following example assumes that firstDayOfWeek is MONDAY. For 2005, which started on a Saturday, week 1 will be the week starting on Monday, January 3. January 1 and 2 will fall within week 0 (or the last week of the previous year).

For 2007, which starts on a Monday, week 1 will be the week startung on Monday, January 1. There will be no week 0 in 2007.

#### `public inline int day() const` {#group__base_1gadced33febfae85988662db9b42e56b02}

Returns the day witin the month (1 to 31).



#### `public inline int dayOfWeek() const` {#group__base_1gaf0b1cfda4f34b38a3f030a43cb935499}



Returns the weekday (0 to 6, where 0 = Sunday, 1 = Monday, ..., 6 = Saturday).

#### `public inline int dayOfYear() const` {#group__base_1ga4591a15d833c10a3a6ba391721bba9b6}



Returns the number of the day in the year. January 1 is 1, February 1 is 32, etc.

#### `public inline int hour() const` {#group__base_1ga03a3ac243fec29a048966cb9d3f2c84e}

Returns the hour (0 to 23).



#### `public inline int hourAMPM() const` {#group__base_1ga715ce88ba0720784ffb6d029be23929f}

Returns the hour (0 to 12).



#### `public inline bool isAM() const` {#group__base_1ga3dcd5082e83dfdf1fb448ca809abe23a}

Returns true if hour < 12;.



#### `public inline bool isPM() const` {#group__base_1gae32597d36b509e031d2c85e0f585cf33}

Returns true if hour >= 12.



#### `public inline int minute() const` {#group__base_1ga1dbfbad9d0753b0ff904994b486a34b3}

Returns the minute (0 to 59).



#### `public inline int second() const` {#group__base_1ga5767718a9fb2343d45d3b3c199a2a19e}

Returns the second (0 to 59).



#### `public inline int millisecond() const` {#group__base_1gaad844e44442901f267a5de9c79992374}

Returns the millisecond (0 to 999)



#### `public inline int microsecond() const` {#group__base_1ga23ed596d25ee1cd738810c7053e41ce9}

Returns the microsecond (0 to 999)



#### `public inline double julianDay() const` {#group__base_1gabe283094aa6c16646c54b712f299c040}

Returns the julian day for the date.



#### `public inline int tzd() const` {#group__base_1gaf4c12ad2150d60db15fae7b272eab06d}

Returns the time zone differential.



#### `public `[`DateTime`](#classscy_1_1DateTime)` utc() const` {#group__base_1ga416f298027e9fbb02fbbbb59280bce06}

Returns the UTC equivalent for the local date and time.



#### `public inline `[`Timestamp`](#classscy_1_1Timestamp)` timestamp() const` {#group__base_1gab031a9c8c582592f94857d03e9732c08}

Returns the date and time expressed as a [Timestamp](#classscy_1_1Timestamp).



#### `public inline `[`Timestamp::UtcTimeVal`](#group__base_1gaec8ace0f25db0ce91c10b30270c3dc24)` utcTime() const` {#group__base_1ga59f8e28c14dce3c650e5127f5aa9b304}

Returns the UTC equivalent for the local date and time.



#### `public bool operator==(const `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & dateTime) const` {#group__base_1ga33d47d51a7701e0b275500adf395df1a}





#### `public bool operator!=(const `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & dateTime) const` {#group__base_1gad10f335463ae82abbc089fc6194e90f7}





#### `public bool operator<(const `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & dateTime) const` {#group__base_1gaf569d4c7514648e90ebd3837ba4dde95}





#### `public bool operator<=(const `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & dateTime) const` {#group__base_1gad2528f970817d96c39323bad3dcdb259}





#### `public bool operator>(const `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & dateTime) const` {#group__base_1gae0861580c425aee3e4bba0c269e32d17}





#### `public bool operator>=(const `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & dateTime) const` {#group__base_1gac7cd53d72b4af340dae23afb3d727925}





#### `public `[`LocalDateTime`](#classscy_1_1LocalDateTime)` operator+(const `[`Timespan`](#classscy_1_1Timespan)` & span) const` {#group__base_1gacd5f92bb4a0cd7ded64e7fb3ffa6396e}





#### `public `[`LocalDateTime`](#classscy_1_1LocalDateTime)` operator-(const `[`Timespan`](#classscy_1_1Timespan)` & span) const` {#group__base_1gad7a1bbae9e178124fa09a1e2a5bc0571}





#### `public `[`Timespan`](#classscy_1_1Timespan)` operator-(const `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & dateTime) const` {#group__base_1ga216a8affb2ff0334d51f57ee0ca81dd7}





#### `public `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & operator+=(const `[`Timespan`](#classscy_1_1Timespan)` & span)` {#group__base_1ga8a0e1e554f15d276630225f97fd59961}





#### `public `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & operator-=(const `[`Timespan`](#classscy_1_1Timespan)` & span)` {#group__base_1ga11a3cfaef5f0d1ffb66f802646eb29a6}





#### `protected  LocalDateTime(`[`Timestamp::UtcTimeVal`](#group__base_1gaec8ace0f25db0ce91c10b30270c3dc24)` utcTime,`[`Timestamp::TimeDiff`](#group__base_1ga9f06831e4e9b9cbc6f061cc61dc91fd0)` diff,int tzd)` {#group__base_1ga1ab5b8d9c17a7ca56e8949c890d45004}





#### `protected void determineTzd(bool adjust)` {#group__base_1ga290ea5b1aee903968e51eed129b287f7}



Recalculate the tzd based on the _dateTime member based on the current timezone using the Standard C runtime functions. If adjust is true, then [adjustForTzd()](#group__base_1gad0d104ecf1aa081650b681f2afe5718d) is called after the differential is calculated.

#### `protected inline void adjustForTzd()` {#group__base_1gad0d104ecf1aa081650b681f2afe5718d}

Adjust the _dateTime member based on the _tzd member.



#### `protected std::time_t dstOffset(int & dstOffset) const` {#group__base_1ga764a663d49040c03389b398732cec64b}

Determine the DST offset for the current date/time.



# class `scy::LogChannel` {#classscy_1_1LogChannel}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  LogChannel(const std::string & name,LogLevel level,const std::string & timeFormat)` | 
`public inline virtual  ~LogChannel()` | 
`public virtual void write(const `[`LogStream`](#structscy_1_1LogStream)` & stream)` | 
`public virtual void write(const std::string & message,LogLevel level,const char * realm,const void * ptr)` | 
`public virtual void format(const `[`LogStream`](#structscy_1_1LogStream)` & stream,std::ostream & ost)` | 
`public inline std::string name() const` | 
`public inline LogLevel level() const` | 
`public inline std::string timeFormat() const` | 
`public inline void setLevel(LogLevel level)` | 
`public inline void setDateFormat(const std::string & format)` | 
`public inline void setFilter(const std::string & filter)` | 
`protected std::string _name` | 
`protected LogLevel _level` | 
`protected std::string _timeFormat` | 
`protected std::string _filter` | 

## Members

#### `public  LogChannel(const std::string & name,LogLevel level,const std::string & timeFormat)` {#group__base_1gac53c51be2549b572f4340d8df7278d32}





#### `public inline virtual  ~LogChannel()` {#group__base_1ga748adc0e12096b5b7d31d153544a9ee3}





#### `public virtual void write(const `[`LogStream`](#structscy_1_1LogStream)` & stream)` {#group__base_1gac8341cc041aa699d3e7ee7f41d37f568}





#### `public virtual void write(const std::string & message,LogLevel level,const char * realm,const void * ptr)` {#group__base_1ga43ccb5ce2a6f8a832ba58cb44a91f433}





#### `public virtual void format(const `[`LogStream`](#structscy_1_1LogStream)` & stream,std::ostream & ost)` {#group__base_1gafff619cedc1fa6a263b64f198b100307}





#### `public inline std::string name() const` {#group__base_1gafa0ee6ecc108c2f153760696afc335b8}





#### `public inline LogLevel level() const` {#group__base_1gaaca35187f512f05be346deefdffa921e}





#### `public inline std::string timeFormat() const` {#group__base_1gad20ec0d479792546f08423b0631cff9d}





#### `public inline void setLevel(LogLevel level)` {#group__base_1ga51948c961f81aba7a53a8efbf16967b1}





#### `public inline void setDateFormat(const std::string & format)` {#group__base_1ga964b4369b3752215f90034107a296937}





#### `public inline void setFilter(const std::string & filter)` {#group__base_1ga4079801e782f93617ff54214fab9d829}





#### `protected std::string _name` {#group__base_1ga36f066b53b2c1afe8928bfd882011fbb}





#### `protected LogLevel _level` {#group__base_1ga447af15ed8a40d0519159370d396dde7}





#### `protected std::string _timeFormat` {#group__base_1gaf59b0b2da184acb4d50f9bba96221d70}





#### `protected std::string _filter` {#group__base_1ga40e25f529270dfc4ec4a40b14e514a02}





# class `scy::Logger` {#classscy_1_1Logger}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Logger()` | 
`public  ~Logger()` | 
`public void add(`[`LogChannel`](#classscy_1_1LogChannel)` * channel)` | Adds the given log channel.
`public void remove(const std::string & name,bool freePointer)` | 
`public `[`LogChannel`](#classscy_1_1LogChannel)` * get(const std::string & name,bool whiny) const` | 
`public void setDefault(const std::string & name)` | Sets the default log to the specified log channel.
`public void setWriter(`[`LogWriter`](#classscy_1_1LogWriter)` * writer)` | Sets the log writer instance.
`public `[`LogChannel`](#classscy_1_1LogChannel)` * getDefault() const` | 
`public void write(const `[`LogStream`](#structscy_1_1LogStream)` & stream)` | 
`public void write(`[`LogStream`](#structscy_1_1LogStream)` * stream)` | 
`public `[`LogStream`](#structscy_1_1LogStream)` & send(const char * level,const char * realm,const void * ptr,const char * channel) const` | 
`protected mutable `[`Mutex`](api-thread.md#classscy_1_1Mutex)` _mutex` | 
`protected LogChannelMap _channels` | 
`protected `[`LogChannel`](api-base.md#classscy_1_1LogChannel)` * _defaultChannel` | 
`protected `[`LogWriter`](api-base.md#classscy_1_1LogWriter)` * _writer` | 
`protected  Logger(const `[`Logger`](#classscy_1_1Logger)` &)` | Non-copyable and non-movable.
`protected  Logger(`[`Logger`](#classscy_1_1Logger)` &&)` | 
`protected `[`Logger`](#classscy_1_1Logger)` & operator=(const `[`Logger`](#classscy_1_1Logger)` &)` | 
`protected `[`Logger`](#classscy_1_1Logger)` & operator=(`[`Logger`](#classscy_1_1Logger)` &&)` | 

## Members

#### `public  Logger()` {#group__base_1gaa58d26b288c4baf5595bbbcb71653b95}





#### `public  ~Logger()` {#group__base_1ga84c1d5e6cd7053bd476eca1eca9e8030}





#### `public void add(`[`LogChannel`](#classscy_1_1LogChannel)` * channel)` {#group__base_1ga699f1be44fef82dd80c3c08fb613a952}

Adds the given log channel.



#### `public void remove(const std::string & name,bool freePointer)` {#group__base_1gaca514dc2c889f2f18f37f73f9f0dbc43}



Removes the given log channel by name, and optionally frees the pointer.

#### `public `[`LogChannel`](#classscy_1_1LogChannel)` * get(const std::string & name,bool whiny) const` {#group__base_1ga91e6c5fd7fa7340056fe8ba8935c2641}



Returns the specified log channel. Throws an exception if the channel doesn't exist.

#### `public void setDefault(const std::string & name)` {#group__base_1ga7ec2b37dc4de80b4790a522f8c08f0a8}

Sets the default log to the specified log channel.



#### `public void setWriter(`[`LogWriter`](#classscy_1_1LogWriter)` * writer)` {#group__base_1ga743b69b00ec1d102d5a49907477bc8ab}

Sets the log writer instance.



#### `public `[`LogChannel`](#classscy_1_1LogChannel)` * getDefault() const` {#group__base_1ga5a48a75f4ee17d6bc3fa81b92331d64d}



Returns the default log channel, or the nullptr channel if no default channel has been set.

#### `public void write(const `[`LogStream`](#structscy_1_1LogStream)` & stream)` {#group__base_1gadea484eefd15977b355c26107cd325df}



Writes the given message to the default log channel. The message will be copied.

#### `public void write(`[`LogStream`](#structscy_1_1LogStream)` * stream)` {#group__base_1ga7e6221c3e20093be489c271a4363e600}



Writes the given message to the default log channel. The stream pointer will be deleted when appropriate.

#### `public `[`LogStream`](#structscy_1_1LogStream)` & send(const char * level,const char * realm,const void * ptr,const char * channel) const` {#group__base_1gaca8968560f32262038ab6f911503bee0}



Sends to the default log using the given class instance. Recommend using write(LogStream&) to avoid copying data.

#### `protected mutable `[`Mutex`](api-thread.md#classscy_1_1Mutex)` _mutex` {#group__base_1gab7cc378d72a27279827f0875a226251c}





#### `protected LogChannelMap _channels` {#group__base_1ga9477bfaa930adfd93c981708c72fff5f}





#### `protected `[`LogChannel`](api-base.md#classscy_1_1LogChannel)` * _defaultChannel` {#group__base_1ga0bc50bd2da990173ed15e251b7df2e9c}





#### `protected `[`LogWriter`](api-base.md#classscy_1_1LogWriter)` * _writer` {#group__base_1ga7faa46b191ca0f6f851ff759c93bbec4}





#### `protected  Logger(const `[`Logger`](#classscy_1_1Logger)` &)` {#group__base_1gacf89b6148e147b8b074e03217107c265}

Non-copyable and non-movable.



#### `protected  Logger(`[`Logger`](#classscy_1_1Logger)` &&)` {#group__base_1gaa7f7e21d2eea736305511f667641f729}





#### `protected `[`Logger`](#classscy_1_1Logger)` & operator=(const `[`Logger`](#classscy_1_1Logger)` &)` {#group__base_1ga6df04064715806676c20e744e9bf29d5}





#### `protected `[`Logger`](#classscy_1_1Logger)` & operator=(`[`Logger`](#classscy_1_1Logger)` &&)` {#group__base_1gad15f5a6b760f84ac89f7821bdd01bfb5}





# class `scy::LogWriter` {#classscy_1_1LogWriter}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  LogWriter()` | 
`public virtual  ~LogWriter()` | 
`public virtual void write(`[`LogStream`](#structscy_1_1LogStream)` * stream)` | Writes the given log message stream.

## Members

#### `public  LogWriter()` {#group__base_1gad880791856a3ee0c440304b8ad6aa0fd}





#### `public virtual  ~LogWriter()` {#group__base_1ga7059e18a3a0c69fe8333f2bbf9a9e6d0}





#### `public virtual void write(`[`LogStream`](#structscy_1_1LogStream)` * stream)` {#group__base_1ga6cba39980fed72f0ca1470bba8dcbda4}

Writes the given log message stream.



# class `scy::MutableBuffer` {#classscy_1_1MutableBuffer}




The [MutableBuffer](#classscy_1_1MutableBuffer) class provides a safe representation of a buffer that can be modified. It does not own the underlying data, and so is cheap to copy or assign.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  MutableBuffer()` | Construct an empty buffer.
`public inline  MutableBuffer(void * data,std::size_t size)` | Construct a buffer to represent the given memory range.
`public inline void * data() const` | 
`public inline std::size_t size() const` | 
`public inline char * cstr() const` | String methods.
`public inline std::string str() const` | 

## Members

#### `public inline  MutableBuffer()` {#group__base_1ga6305583f6d3eb711d61bdfbb0d3ccbea}

Construct an empty buffer.



#### `public inline  MutableBuffer(void * data,std::size_t size)` {#group__base_1ga547b55206376f5ec83ac9c96c90b3871}

Construct a buffer to represent the given memory range.



#### `public inline void * data() const` {#group__base_1ga1cdb57a6dd8985a70f97069e24a17c2e}





#### `public inline std::size_t size() const` {#group__base_1gaffc632ea4b4af2f5185c3364e18d128f}





#### `public inline char * cstr() const` {#group__base_1ga80e3a8715d5d79b28e37090b68c0031d}

String methods.



#### `public inline std::string str() const` {#group__base_1ga5ed8318956c72abfcd85d9f36643e94d}





# class `scy::MutexState` {#classscy_1_1MutexState}

```
class scy::MutexState
  : public scy::State
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  MutexState(ID id)` | 
`public inline  MutexState(const `[`MutexState`](#classscy_1_1MutexState)` & r)` | 
`public inline virtual  ~MutexState()` | 
`public inline virtual ID id() const` | 
`public inline virtual void set(ID id)` | 
`public inline virtual std::string message() const` | 
`public inline virtual void setMessage(const std::string & message)` | 
`protected mutable `[`Mutex`](api-thread.md#classscy_1_1Mutex)` _mutex` | 

## Members

#### `public  MutexState(ID id)` {#group__base_1gafdadcdc86293e4802c0ea3004841e980}





#### `public inline  MutexState(const `[`MutexState`](#classscy_1_1MutexState)` & r)` {#group__base_1ga60e792e091e7387d55b6b2087a8720c6}





#### `public inline virtual  ~MutexState()` {#group__base_1gac9cf7e2d1ad58640ce75d66eb06ee904}





#### `public inline virtual ID id() const` {#group__base_1ga8c19150df35a419c410f1f86e0c64f2c}





#### `public inline virtual void set(ID id)` {#group__base_1ga72fac3087dc1f9f82334faa2482db897}





#### `public inline virtual std::string message() const` {#group__base_1ga7c5cba9ece00a246448b014054c2c891}





#### `public inline virtual void setMessage(const std::string & message)` {#group__base_1ga5c3a86b27eac29fcd16f7ca92598d827}





#### `protected mutable `[`Mutex`](api-thread.md#classscy_1_1Mutex)` _mutex` {#group__base_1ga3c3eba92c1a058aa46cf9f58ac76c859}





# class `scy::NullSignal` {#classscy_1_1NullSignal}

```
class scy::NullSignal
  : public scy::SignalBase< DelegateBase<> >
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------

## Members

# class `scy::NVCollection` {#classscy_1_1NVCollection}




A storage container for a name value collections. This collection can store multiple entries for each name, and it's getters are case-insensitive.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  NVCollection()` | 
`public inline  NVCollection(const `[`NVCollection`](#classscy_1_1NVCollection)` & nvc)` | 
`public inline virtual  ~NVCollection()` | 
`public inline `[`NVCollection`](#classscy_1_1NVCollection)` & operator=(const `[`NVCollection`](#classscy_1_1NVCollection)` & nvc)` | Assigns the name-value pairs of another [NVCollection](api-base.md#classscy_1_1NVCollection) to this one.
`public inline const std::string & operator[](const std::string & name) const` | 
`public inline void set(const std::string & name,const std::string & value)` | Sets the value of the (first) name-value pair with the given name.
`public inline void add(const std::string & name,const std::string & value)` | Adds a new name-value pair with the given name and value.
`public inline const std::string & get(const std::string & name) const` | 
`public inline const std::string & get(const std::string & name,const std::string & defaultValue) const` | 
`public inline bool has(const std::string & name) const` | 
`public inline ConstIterator find(const std::string & name) const` | 
`public inline ConstIterator begin() const` | 
`public inline ConstIterator end() const` | 
`public inline bool empty() const` | Returns true iff the header does not have any content.
`public inline int size() const` | 
`public inline void erase(const std::string & name)` | Removes all name-value pairs with the given name.
`public inline void clear()` | Removes all name-value pairs and their values.

## Members

#### `public inline  NVCollection()` {#group__base_1ga41eb71d1735c8238b24dcda700c23d21}





#### `public inline  NVCollection(const `[`NVCollection`](#classscy_1_1NVCollection)` & nvc)` {#group__base_1gaca0d4928654fd5b79fd06612222080b9}





#### `public inline virtual  ~NVCollection()` {#group__base_1gaed8abfccd111eea9b5cabe59ef8aac46}





#### `public inline `[`NVCollection`](#classscy_1_1NVCollection)` & operator=(const `[`NVCollection`](#classscy_1_1NVCollection)` & nvc)` {#group__base_1ga6e9938e6b926c62d4ccbea0c7a623dec}

Assigns the name-value pairs of another [NVCollection](#classscy_1_1NVCollection) to this one.



#### `public inline const std::string & operator[](const std::string & name) const` {#group__base_1gab76fc12abf8a9f13ac81e32d9788cea3}



Returns the value of the (first) name-value pair with the given name.

Throws a NotFoundException if the name-value pair does not exist.

#### `public inline void set(const std::string & name,const std::string & value)` {#group__base_1ga36860b3fb325b24ed969668192d2d5ad}

Sets the value of the (first) name-value pair with the given name.



#### `public inline void add(const std::string & name,const std::string & value)` {#group__base_1gae0cbccd7da15dd37eed7631c853c2565}

Adds a new name-value pair with the given name and value.



#### `public inline const std::string & get(const std::string & name) const` {#group__base_1ga02db5c5230e78a2f8272b0380ac8fe2b}



Returns the value of the first name-value pair with the given name.

Throws a NotFoundException if the name-value pair does not exist.

#### `public inline const std::string & get(const std::string & name,const std::string & defaultValue) const` {#group__base_1ga5b609c360028a9ec67aee8fb1c6e543d}



Returns the value of the first name-value pair with the given name. If no value with the given name has been found, the defaultValue is returned.

#### `public inline bool has(const std::string & name) const` {#group__base_1ga01251074fc906db8ec5abaaaf084fdad}



Returns true if there is at least one name-value pair with the given name.

#### `public inline ConstIterator find(const std::string & name) const` {#group__base_1ga07ca18d979f83a19bcf526357e45d2e8}



Returns an iterator pointing to the first name-value pair with the given name.

#### `public inline ConstIterator begin() const` {#group__base_1gaef153806a4bf20b97530fcb8c0cb0c5f}



Returns an iterator pointing to the begin of the name-value pair collection.

#### `public inline ConstIterator end() const` {#group__base_1ga5de16e354315a6c291156deef03b3229}



Returns an iterator pointing to the end of the name-value pair collection.

#### `public inline bool empty() const` {#group__base_1gaf563c36905a2529054785a1734d46f67}

Returns true iff the header does not have any content.



#### `public inline int size() const` {#group__base_1gae1a0d58fc01bd230cf5497d8e5ae398f}



Returns the number of name-value pairs in the collection.

#### `public inline void erase(const std::string & name)` {#group__base_1gab295478be7ae8a444d0071c96f4108ff}

Removes all name-value pairs with the given name.



#### `public inline void clear()` {#group__base_1ga73d773429898c1c3ba526155b6cc0b90}

Removes all name-value pairs and their values.



# class `scy::PacketFactory` {#classscy_1_1PacketFactory}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  PacketFactory()` | 
`public inline virtual  ~PacketFactory()` | 
`public template<class PacketT>`  <br/>`inline void registerPacketType(int priority)` | 
`public template<class PacketT>`  <br/>`inline void unregisterPacketType()` | 
`public template<class StrategyT>`  <br/>`inline void registerStrategy(int priority)` | 
`public template<class StrategyT>`  <br/>`inline void unregisterStrategy()` | 
`public inline PacketCreationStrategyList & types()` | 
`public inline PacketCreationStrategyList types() const` | 
`public inline virtual bool onPacketCreated(`[`IPacket`](#classscy_1_1IPacket)` *)` | returning false will stop packet propagation
`public inline virtual `[`IPacket`](#classscy_1_1IPacket)` * createPacket(const `[`ConstBuffer`](#classscy_1_1ConstBuffer)` & buffer,std::size_t & nread)` | 
`protected PacketCreationStrategyList _types` | 

## Members

#### `public inline  PacketFactory()` {#group__base_1gac108d58054dccbf3736c7ecb2a968d58}





#### `public inline virtual  ~PacketFactory()` {#group__base_1ga803341e3fb9567e3017ad1774acda0fd}





#### `public template<class PacketT>`  <br/>`inline void registerPacketType(int priority)` {#group__base_1ga362ade3e31ecca2015cdb85cec6c9e4f}





#### `public template<class PacketT>`  <br/>`inline void unregisterPacketType()` {#group__base_1ga6e1ee51199e2b5241afb8f872120f48a}





#### `public template<class StrategyT>`  <br/>`inline void registerStrategy(int priority)` {#group__base_1gaec4ee2b41f920d44c5e3276992e16a48}





#### `public template<class StrategyT>`  <br/>`inline void unregisterStrategy()` {#group__base_1gabdbc79c8ee73048b8cff7ece2f465b30}





#### `public inline PacketCreationStrategyList & types()` {#group__base_1gac522f12ee6a7f707756ccf9079757e59}





#### `public inline PacketCreationStrategyList types() const` {#group__base_1ga78324767a673d825a34e6b6e02a29036}





#### `public inline virtual bool onPacketCreated(`[`IPacket`](#classscy_1_1IPacket)` *)` {#group__base_1ga86ec412fa2a46187ddf8154cab0afd5c}

returning false will stop packet propagation



#### `public inline virtual `[`IPacket`](#classscy_1_1IPacket)` * createPacket(const `[`ConstBuffer`](#classscy_1_1ConstBuffer)` & buffer,std::size_t & nread)` {#group__base_1gadc8df0d7fe90764a25642a9bde9b3b4d}





#### `protected PacketCreationStrategyList _types` {#group__base_1gac2bbd50d6da749659cc25265ba3b13e6}





# class `scy::PacketProcessor` {#classscy_1_1PacketProcessor}

```
class scy::PacketProcessor
  : public scy::PacketStreamAdapter
```  

For 0.8.x compatibility.

This class is a virtual interface for creating PacketStreamAdapters which process that and emit the [IPacket](#classscy_1_1IPacket) type.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  PacketProcessor(`[`PacketSignal`](#classscy_1_1SignalBase)` & emitter)` | 
`public void process(`[`IPacket`](#classscy_1_1IPacket)` & packet)` | 
`public inline virtual bool accepts(`[`IPacket`](#classscy_1_1IPacket)` &)` | 
`public inline virtual void operator<<(`[`IPacket`](#classscy_1_1IPacket)` & packet)` | [Stream](#classscy_1_1Stream) operator alias for [process()](#group__base_1gad7adc3fd78dce41f0f96744dfe6d1d50)

## Members

#### `public inline  PacketProcessor(`[`PacketSignal`](#classscy_1_1SignalBase)` & emitter)` {#group__base_1gaf3ab7df2c2416fdb3bd4e2b313c8b772}





#### `public void process(`[`IPacket`](#classscy_1_1IPacket)` & packet)` {#group__base_1gad7adc3fd78dce41f0f96744dfe6d1d50}



This method performs processing on the given packet and emits the result.

Note: If packet processing is async (the packet is not in the current thread scope) then packet data must be copied. Copied data can be freed directly aFter the async call to emit() the outgoing packet.

#### `public inline virtual bool accepts(`[`IPacket`](#classscy_1_1IPacket)` &)` {#group__base_1ga0b789237428851d3033e5cf3d6f6bfce}



This method ensures compatibility with the given packet type. Return false to reject the packet.

#### `public inline virtual void operator<<(`[`IPacket`](#classscy_1_1IPacket)` & packet)` {#group__base_1ga0b09928130bd267d58273ea85be1891a}

[Stream](#classscy_1_1Stream) operator alias for [process()](#group__base_1gad7adc3fd78dce41f0f96744dfe6d1d50)



# class `scy::PacketStream` {#classscy_1_1PacketStream}

```
class scy::PacketStream
  : public scy::Stateful< PacketStreamState >
```  



This class is used for processing and boradcasting IPackets in a flexible way. A [PacketStream](#classscy_1_1PacketStream) consists of one or many PacketSources, one or many PacketProcessors, and one or many delegate receivers.

This class enables the developer to setup a processor chain in order to perform arbitrary processing on data packets using interchangeable packet adapters, and pump the output to any delegate function,/// or even another [PacketStream](#classscy_1_1PacketStream).

Note that [PacketStream](#classscy_1_1PacketStream) itself inherits from [PacketStreamAdapter](#classscy_1_1PacketStreamAdapter),/// so a [PacketStream](#classscy_1_1PacketStream) be the source of another [PacketStream](#classscy_1_1PacketStream).

All [PacketStream](#classscy_1_1PacketStream) methods are thread-safe, but once the stream is running you will not be able to attach or detach stream adapters.

In order to synchronize output packets with the application event loop take a look at the [SyncPacketQueue](#classscy_1_1SyncPacketQueue) class. For lengthy operations you can add an [AsyncPacketQueue](#classscy_1_1AsyncPacketQueue) to the start of the stream to defer processing from the PacketSource thread.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`PacketSignal`](api-base.md#classscy_1_1SignalBase)` emitter` | Signals to delegates on outgoing packets.
`public `[`Signal`](api-base.md#classscy_1_1Signal)`< const std::exception_ptr & > Error` | 
`public `[`NullSignal`](api-base.md#classscy_1_1NullSignal)` Close` | 
`public  PacketStream(const std::string & name)` | 
`public virtual  ~PacketStream()` | 
`public virtual void start()` | Start the stream and synchronized sources.
`public virtual void stop()` | Stop the stream and synchronized sources.
`public virtual void pause()` | Pause the stream.
`public virtual void resume()` | Resume the stream.
`public virtual void close()` | Close the stream and transition the internal state to Closed.
`public virtual void reset()` | Cleanup all managed stream adapters and reset the stream state.
`public virtual bool active() const` | Returns true when the stream is in the Active state.
`public virtual bool stopped() const` | Returns true when the stream is in the Stopping or Stopped state.
`public virtual bool closed() const` | Returns true when the stream is in the Closed or [Error](api-base.md#structscy_1_1Error) state.
`public virtual bool lock()` | 
`public virtual bool locked() const` | Returns true is the stream is currently locked.
`public virtual void write(char * data,std::size_t len)` | Writes data to the stream (nocopy).
`public virtual void write(const char * data,std::size_t len)` | Writes data to the stream (copied).
`public virtual void write(`[`IPacket`](#classscy_1_1IPacket)` & packet)` | Writes an incoming packet onto the stream.
`public virtual void attachSource(`[`PacketSignal`](#classscy_1_1SignalBase)` & source)` | 
`public virtual void attachSource(`[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` * source,bool freePointer,bool syncState)` | 
`public template<class C>`  <br/>`inline void attachSource(std::shared_ptr< C > ptr,bool syncState)` | 
`public virtual bool detachSource(`[`PacketSignal`](#classscy_1_1SignalBase)` & source)` | Detaches the given source packet signal from the stream.
`public virtual bool detachSource(`[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` * source)` | 
`public virtual void attach(`[`PacketProcessor`](#classscy_1_1PacketProcessor)` * proc,int order,bool freePointer)` | 
`public template<class C>`  <br/>`inline void attach(std::shared_ptr< C > ptr,bool syncState)` | 
`public virtual bool detach(`[`PacketProcessor`](#classscy_1_1PacketProcessor)` * proc)` | 
`public virtual void synchronizeOutput(uv::Loop * loop)` | Synchronize stream output packets with the given event loop.
`public virtual void autoStart(bool flag)` | 
`public virtual void closeOnError(bool flag)` | 
`public virtual void setClientData(void * data)` | Accessors for the unmanaged client data pointer.
`public virtual void * clientData() const` | 
`public const std::exception_ptr & error()` | Returns the stream error (if any).
`public std::string name() const` | Returns the name of the packet stream.
`public PacketAdapterVec adapters() const` | Returns a combined list of all stream sources and processors.
`public PacketAdapterVec sources() const` | Returns a list of all stream sources.
`public PacketAdapterVec processors() const` | Returns a list of all stream processors.
`public bool waitForRunner()` | 
`public bool waitForStateSync(PacketStreamState::ID state)` | Block the calling thread until the given state is synchronized.
`public int numSources() const` | 
`public int numProcessors() const` | 
`public int numAdapters() const` | 
`public template<class AdapterT>`  <br/>`inline AdapterT * getSource(int index)` | 
`public template<class AdapterT>`  <br/>`inline AdapterT * getProcessor(int index)` | 
`public inline `[`PacketProcessor`](#classscy_1_1PacketProcessor)` * getProcessor(int order)` | Returns the [PacketProcessor](api-base.md#classscy_1_1PacketProcessor) at the given position.
`protected mutable `[`Mutex`](api-thread.md#classscy_1_1Mutex)` _mutex` | 
`protected mutable `[`Mutex`](api-thread.md#classscy_1_1Mutex)` _procMutex` | 
`protected std::string _name` | 
`protected PacketAdapterVec _sources` | 
`protected PacketAdapterVec _processors` | 
`protected std::deque< `[`PacketStreamState`](api-base.md#structscy_1_1PacketStreamState)` > _states` | 
`protected std::exception_ptr _error` | 
`protected bool _autoStart` | 
`protected bool _closeOnError` | 
`protected void * _clientData` | 
`protected void setup()` | Attach the source and processor delegate chain.
`protected void teardown()` | Detach the source and processor delegate chain.
`protected void emit(`[`IPacket`](#classscy_1_1IPacket)` & packet)` | 
`protected void attachSource(PacketAdapterReference::Ptr ref)` | 
`protected void attach(PacketAdapterReference::Ptr ref)` | 
`protected void process(`[`IPacket`](#classscy_1_1IPacket)` & packet)` | [Process](api-base.md#classscy_1_1Process) incoming packets.
`protected void startSources()` | Start synchronized sources.
`protected void stopSources()` | Stop synchronized sources.
`protected void synchronizeStates()` | Synchronize queued states with adapters.
`protected virtual void onStateChange(`[`PacketStreamState`](#structscy_1_1PacketStreamState)` & state,const `[`PacketStreamState`](#structscy_1_1PacketStreamState)` & oldState)` | Override the [Stateful::onStateChange](#group__base_1ga3c88b4aa8e9343f9bdc1a8c2ae2a5452) method.
`protected bool hasQueuedState(PacketStreamState::ID state) const` | Returns true if the given state ID is queued.
`protected void assertCanModify()` | 

## Members

#### `public `[`PacketSignal`](api-base.md#classscy_1_1SignalBase)` emitter` {#group__base_1ga3d4fa2d39446cd24ce5d8281834b70db}

Signals to delegates on outgoing packets.



#### `public `[`Signal`](api-base.md#classscy_1_1Signal)`< const std::exception_ptr & > Error` {#group__base_1ga2723c4adbc983393808117913b4aefe1}



Signals that the [PacketStream](#classscy_1_1PacketStream) is in [Error](#structscy_1_1Error) state. If stream output is synchronized then the [Error](#structscy_1_1Error) signal will be sent from the synchronization context, otherwise it will be sent from the async processor context. See [synchronizeOutput()](#group__base_1ga27a797846a098946cf73d80726302a9e)

#### `public `[`NullSignal`](api-base.md#classscy_1_1NullSignal)` Close` {#group__base_1gad79c7a7a6f86fc9e4a5d19198daab658}



Signals that the [PacketStream](#classscy_1_1PacketStream) is in Close state. This signal is sent immediately via the [close()](#group__base_1gac0eec4eda5ff0571b46b4a39af9f7dcf) method, and as such will be sent from the calling thread context.

#### `public  PacketStream(const std::string & name)` {#group__base_1ga7313ef8bb9b19b1b8cf88698e77cbb92}





#### `public virtual  ~PacketStream()` {#group__base_1ga2b6dffb019c4da42da8fea435623568e}





#### `public virtual void start()` {#group__base_1gaa07075299f2271e58f78de3fd7e62b10}

Start the stream and synchronized sources.



#### `public virtual void stop()` {#group__base_1ga5ace99af3f0ff1e93483a7f5dd5dca69}

Stop the stream and synchronized sources.



#### `public virtual void pause()` {#group__base_1ga866ba120ec05be813d82a6e05827292e}

Pause the stream.



#### `public virtual void resume()` {#group__base_1ga2e27223e238aa0174180abd379e3149c}

Resume the stream.



#### `public virtual void close()` {#group__base_1gac0eec4eda5ff0571b46b4a39af9f7dcf}

Close the stream and transition the internal state to Closed.



#### `public virtual void reset()` {#group__base_1gabf0b20944de791715e6891520fd6784b}

Cleanup all managed stream adapters and reset the stream state.



#### `public virtual bool active() const` {#group__base_1ga6f53641239172c34777c6fae3c96fc98}

Returns true when the stream is in the Active state.



#### `public virtual bool stopped() const` {#group__base_1gaa1485ea7e14fd0d929886604930a4092}

Returns true when the stream is in the Stopping or Stopped state.



#### `public virtual bool closed() const` {#group__base_1ga45d9514c5da12522e7e672f46f433d27}

Returns true when the stream is in the Closed or [Error](#structscy_1_1Error) state.



#### `public virtual bool lock()` {#group__base_1gaa49be3aec494af1412f80a0613d5dc77}



Sets the stream to locked state. In a locked state no new adapters can be added or removed from the stream until the stream is stopped.

#### `public virtual bool locked() const` {#group__base_1gad0ff7534821aebeda8731d5840cef8bb}

Returns true is the stream is currently locked.



#### `public virtual void write(char * data,std::size_t len)` {#group__base_1ga45245460b60507e554fcc3f37114fd1a}

Writes data to the stream (nocopy).



#### `public virtual void write(const char * data,std::size_t len)` {#group__base_1gaf564a3a9a138475d4683bd7daf796649}

Writes data to the stream (copied).



#### `public virtual void write(`[`IPacket`](#classscy_1_1IPacket)` & packet)` {#group__base_1ga739eea442d27d771be3a2917dfeed45f}

Writes an incoming packet onto the stream.



#### `public virtual void attachSource(`[`PacketSignal`](#classscy_1_1SignalBase)` & source)` {#group__base_1ga16adec5b00a33070556dd8531e1825c0}



Attaches a source packet emitter to the stream. The source packet adapter can be another [PacketStream::emitter](#group__base_1ga3d4fa2d39446cd24ce5d8281834b70db).

#### `public virtual void attachSource(`[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` * source,bool freePointer,bool syncState)` {#group__base_1gac341b1ecec898aab2c8be3629af1d846}



Attaches a source packet emitter to the stream. If freePointer is true, the pointer will be deleted when the stream is closed. If syncState is true and the source is a basic::Stratable, then the source's [start()](#group__base_1gaa07075299f2271e58f78de3fd7e62b10)/stop() methods will be synchronized when calling [startSources()](#group__base_1gaeb9605f5d35a6aaa92bf427e0989827c)/stopSources().

#### `public template<class C>`  <br/>`inline void attachSource(std::shared_ptr< C > ptr,bool syncState)` {#group__base_1gae6f1bb5accd371bd151cfbabc141b331}



Attaches a source packet emitter to the stream. This method enables compatibility with shared_ptr managed adapter instances.

#### `public virtual bool detachSource(`[`PacketSignal`](#classscy_1_1SignalBase)` & source)` {#group__base_1ga06e1aa0b843f233d6dc71ca293a24945}

Detaches the given source packet signal from the stream.



#### `public virtual bool detachSource(`[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` * source)` {#group__base_1ga4f77bef24858fbb7bf31b43899c6fd27}



Detaches the given source packet adapter from the stream. Note: The pointer will be forgotten about, so if the freePointer flag set when calling [attachSource()](#group__base_1ga16adec5b00a33070556dd8531e1825c0) will have no effect.

#### `public virtual void attach(`[`PacketProcessor`](#classscy_1_1PacketProcessor)` * proc,int order,bool freePointer)` {#group__base_1gaeff06c27d0ed0174f7f19ff61b3ff2fe}



Attaches a packet processor to the stream. Order determines the position of the processor in the stream queue. If freePointer is true, the pointer will be deleted when the stream closes.

#### `public template<class C>`  <br/>`inline void attach(std::shared_ptr< C > ptr,bool syncState)` {#group__base_1gad14b98beae412615b279039de334c85c}



Attaches a packet processor to the stream. This method enables compatibility with shared_ptr managed adapter instances.

#### `public virtual bool detach(`[`PacketProcessor`](#classscy_1_1PacketProcessor)` * proc)` {#group__base_1ga879c4077767fcbd5def56f4398202d63}



Detaches a packet processor from the stream. Note: The pointer will be forgotten about, so if the freePointer flag set when calling [attach()](#group__base_1gaeff06c27d0ed0174f7f19ff61b3ff2fe) will have no effect.

#### `public virtual void synchronizeOutput(uv::Loop * loop)` {#group__base_1ga27a797846a098946cf73d80726302a9e}

Synchronize stream output packets with the given event loop.



#### `public virtual void autoStart(bool flag)` {#group__base_1ga2042eee4cb00925265abeaef5cbb2588}



Set the stream to auto start if inactive (default: false).

With this flag set the stream will automatically transition to Active state if the in either the None or Locaked state.

#### `public virtual void closeOnError(bool flag)` {#group__base_1ga58995f68cf550280612199862a7dba84}



Set the stream to be closed on error (default: true).

With this flag set the stream will be automatically transitioned to Closed state from [Error](#structscy_1_1Error) state.

#### `public virtual void setClientData(void * data)` {#group__base_1ga471727de4be59f65acf21c4bfed4f1b0}

Accessors for the unmanaged client data pointer.



#### `public virtual void * clientData() const` {#group__base_1ga4677310b18eede321e35fc84e30bc8a3}





#### `public const std::exception_ptr & error()` {#group__base_1ga894d3a083f545aa5d4eb001819ce79f7}

Returns the stream error (if any).



#### `public std::string name() const` {#group__base_1gacf6a0e6b50acb9575a6cda69863c43f3}

Returns the name of the packet stream.



#### `public PacketAdapterVec adapters() const` {#group__base_1ga847517ada8ae4492b1fa54eb75fe7d0b}

Returns a combined list of all stream sources and processors.



#### `public PacketAdapterVec sources() const` {#group__base_1ga95efa9dc101b1ddd2a37f587931a45bc}

Returns a list of all stream sources.



#### `public PacketAdapterVec processors() const` {#group__base_1ga7c3ddf305de8bff9d02e00338f6a6792}

Returns a list of all stream processors.



#### `public bool waitForRunner()` {#group__base_1ga8b551f073319cdefab9409a4040f88a4}



Block the calling thread until all packets have been flushed, and internal states have been synchronized. This function is only useful after calling [stop()](#group__base_1ga5ace99af3f0ff1e93483a7f5dd5dca69) or [pause()](#group__base_1ga866ba120ec05be813d82a6e05827292e).

#### `public bool waitForStateSync(PacketStreamState::ID state)` {#group__base_1ga1faea001054f373f18efde4b7f9e496c}

Block the calling thread until the given state is synchronized.



#### `public int numSources() const` {#group__base_1ga846914577fbf526e0389ee36820865d8}





#### `public int numProcessors() const` {#group__base_1ga7e1d39ba8a8aca30b86c693937dde258}





#### `public int numAdapters() const` {#group__base_1gacd6972c47d4945735771841b16163a46}





#### `public template<class AdapterT>`  <br/>`inline AdapterT * getSource(int index)` {#group__base_1ga21bf4160a5eaec6a6eb74e1a23f5da0a}





#### `public template<class AdapterT>`  <br/>`inline AdapterT * getProcessor(int index)` {#group__base_1gae4db238f61fbd0cef936b9a523322f40}





#### `public inline `[`PacketProcessor`](#classscy_1_1PacketProcessor)` * getProcessor(int order)` {#group__base_1ga8e479f2307c4bc3158d7d53cfbaed036}

Returns the [PacketProcessor](#classscy_1_1PacketProcessor) at the given position.



#### `protected mutable `[`Mutex`](api-thread.md#classscy_1_1Mutex)` _mutex` {#group__base_1ga40b214c2a390ede8aafa541e05af73c9}





#### `protected mutable `[`Mutex`](api-thread.md#classscy_1_1Mutex)` _procMutex` {#group__base_1gac4ad9ab7d9abd4924b932c1d6b2502c9}





#### `protected std::string _name` {#group__base_1ga928ac73b404916c524349f82b837a251}





#### `protected PacketAdapterVec _sources` {#group__base_1ga884fa29fc53edcca06ff84997b48cfd5}





#### `protected PacketAdapterVec _processors` {#group__base_1ga06fe5e28961a3352db8f80138eb30564}





#### `protected std::deque< `[`PacketStreamState`](api-base.md#structscy_1_1PacketStreamState)` > _states` {#group__base_1ga6cdb9717a975c66542a88ce34586265b}





#### `protected std::exception_ptr _error` {#group__base_1gaabf4120335505da2f373b7afb64cf156}





#### `protected bool _autoStart` {#group__base_1gabc12f096a88953028521259790bae93f}





#### `protected bool _closeOnError` {#group__base_1ga255d1421e03dfac4aafbe763c759f85a}





#### `protected void * _clientData` {#group__base_1gadff41fcdd74a6207fb98311b2750c21c}





#### `protected void setup()` {#group__base_1ga2e71c28fac4871d262138e58e048c641}

Attach the source and processor delegate chain.



#### `protected void teardown()` {#group__base_1ga7529aa1efb6d052d08b1aa9d64988a03}

Detach the source and processor delegate chain.



#### `protected void emit(`[`IPacket`](#classscy_1_1IPacket)` & packet)` {#group__base_1ga2ec0154d38011167b59aa54a2cdddd98}



Emit the final packet to listeners.

Synchronized signals such as Close and [Error](#structscy_1_1Error) are sent from this method. See [synchronizeOutput()](#group__base_1ga27a797846a098946cf73d80726302a9e)

#### `protected void attachSource(PacketAdapterReference::Ptr ref)` {#group__base_1ga4187fed0c3f68c220a9bb2779481e2db}





#### `protected void attach(PacketAdapterReference::Ptr ref)` {#group__base_1ga6ab3b5c164ac0bab8137ae0e293b7f99}





#### `protected void process(`[`IPacket`](#classscy_1_1IPacket)` & packet)` {#group__base_1ga379f518fe7c1a50af177b738e0b28f17}

[Process](#classscy_1_1Process) incoming packets.



#### `protected void startSources()` {#group__base_1gaeb9605f5d35a6aaa92bf427e0989827c}

Start synchronized sources.



#### `protected void stopSources()` {#group__base_1ga73955535a0312cd3371c3317c1bfa1f0}

Stop synchronized sources.



#### `protected void synchronizeStates()` {#group__base_1ga4abe768801f8b39876834e45ead0ac1e}

Synchronize queued states with adapters.



#### `protected virtual void onStateChange(`[`PacketStreamState`](#structscy_1_1PacketStreamState)` & state,const `[`PacketStreamState`](#structscy_1_1PacketStreamState)` & oldState)` {#group__base_1gafba208f4b7aa3c493ea1eac2e571fa64}

Override the [Stateful::onStateChange](#group__base_1ga3c88b4aa8e9343f9bdc1a8c2ae2a5452) method.



#### `protected bool hasQueuedState(PacketStreamState::ID state) const` {#group__base_1ga8937a72ac835ea7c23a4ef478537ab74}

Returns true if the given state ID is queued.



#### `protected void assertCanModify()` {#group__base_1ga8222585fd3f575c67b6b28f1e7cb5217}



Asserts that the stream can be modified, ie is not in the Locked, Stopping or Active states.

# class `scy::PacketStreamAdapter` {#classscy_1_1PacketStreamAdapter}




This class is a wrapper for integrating external classes with the a [PacketStream](#classscy_1_1PacketStream)'s data flow and state machine.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  PacketStreamAdapter(`[`PacketSignal`](#classscy_1_1SignalBase)` & emitter)` | 
`public inline virtual  ~PacketStreamAdapter()` | 
`public virtual void emit(char * data,std::size_t len,unsigned flags)` | 
`public virtual void emit(const char * data,std::size_t len,unsigned flags)` | 
`public virtual void emit(const std::string & str,unsigned flags)` | 
`public virtual void emit(unsigned flags)` | 
`public virtual void emit(`[`IPacket`](#classscy_1_1IPacket)` & packet)` | 
`public `[`PacketSignal`](#classscy_1_1SignalBase)` & getEmitter()` | Returns a reference to the outgoing packet signal.
`public inline virtual void onStreamStateChange(const `[`PacketStreamState`](#structscy_1_1PacketStreamState)` &)` | 
`protected `[`PacketSignal`](api-base.md#classscy_1_1SignalBase)` & _emitter` | 
`protected  PacketStreamAdapter(const `[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` &)` | 
`protected  PacketStreamAdapter(`[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` &&)` | 
`protected `[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` & operator=(const `[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` &)` | 
`protected `[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` & operator=(`[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` &&)` | 

## Members

#### `public  PacketStreamAdapter(`[`PacketSignal`](#classscy_1_1SignalBase)` & emitter)` {#group__base_1ga33639caa7975d01430b01643da0f3ddb}





#### `public inline virtual  ~PacketStreamAdapter()` {#group__base_1ga6223fd3eac5802d6efa67b37515c7c3f}





#### `public virtual void emit(char * data,std::size_t len,unsigned flags)` {#group__base_1ga391365dfda25720e961d6cd809412bbf}





#### `public virtual void emit(const char * data,std::size_t len,unsigned flags)` {#group__base_1gafaea1217189ad7d83d1c3540466d21bf}





#### `public virtual void emit(const std::string & str,unsigned flags)` {#group__base_1gaefade7945aa72cd6b1fee74746fda713}





#### `public virtual void emit(unsigned flags)` {#group__base_1ga4f329880c10ea2492479ddd90df48b09}





#### `public virtual void emit(`[`IPacket`](#classscy_1_1IPacket)` & packet)` {#group__base_1ga3b302bbd1b8c8eb21d549eda77ab2e58}





#### `public `[`PacketSignal`](#classscy_1_1SignalBase)` & getEmitter()` {#group__base_1ga3c56e12afc6c4863d9f70258dfe6c2a5}

Returns a reference to the outgoing packet signal.



#### `public inline virtual void onStreamStateChange(const `[`PacketStreamState`](#structscy_1_1PacketStreamState)` &)` {#group__base_1gacaab737ce646207471c5f7bb3b6b794e}



Called by the [PacketStream](#classscy_1_1PacketStream) to notify when the internal [Stream](#classscy_1_1Stream) state changes. On receiving the Stopped state, it is the responsibility of the adapter to have ceased all outgoing packet transmission, especially in multi-thread scenarios.

#### `protected `[`PacketSignal`](api-base.md#classscy_1_1SignalBase)` & _emitter` {#group__base_1ga080cfd0707ca633ed93b77aa066c6e8b}





#### `protected  PacketStreamAdapter(const `[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` &)` {#group__base_1ga082704d9ef37c5cc5e267f95438053bb}





#### `protected  PacketStreamAdapter(`[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` &&)` {#group__base_1ga27b994fb11a53d66db06b6d29b66921c}





#### `protected `[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` & operator=(const `[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` &)` {#group__base_1gae8cdb3cddd4b5df3ad9f3cf4d55c287b}





#### `protected `[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` & operator=(`[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` &&)` {#group__base_1ga20a029ecd3bedeaa7c70f3ca4d2b3763}





# class `scy::PacketTransaction` {#classscy_1_1PacketTransaction}

```
class scy::PacketTransaction
  : public scy::async::Sendable
  : public scy::Stateful< TransactionState >
```  



This class provides request/response functionality for [IPacket](#classscy_1_1IPacket) types.

PacketTransactions are fire and forget. The object will be deleted after a successful response or a timeout.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  PacketTransaction(long timeout,int retries,uv::Loop * loop)` | 
`public inline  PacketTransaction(const PacketT & request,long timeout,int retries,uv::Loop * loop)` | 
`public inline virtual bool send()` | 
`public inline virtual void cancel()` | 
`public inline bool cancelled() const` | 
`public inline virtual void dispose()` | 
`public inline virtual bool canResend()` | 
`public inline int attempts() const` | 
`public inline int retries() const` | 
`public inline PacketT & request()` | 
`public inline PacketT request() const` | 
`public inline PacketT & response()` | 
`public inline PacketT response() const` | 
`protected PacketT _request` | 
`protected PacketT _response` | 
`protected `[`Timer`](api-base.md#classscy_1_1Timer)` _timer` | 
`protected int _timeout` | The request timeout in milliseconds.
`protected int _retries` | The maximum number of attempts before the transaction is considered failed.
`protected int _attempts` | The number of times the transaction has been sent.
`protected bool _destroyed` | 
`protected inline virtual  ~PacketTransaction()` | 
`protected inline virtual void onStateChange(`[`TransactionState`](#structscy_1_1TransactionState)` & state,const `[`TransactionState`](#structscy_1_1TransactionState)` &)` | Override to handle post state change logic.
`protected inline virtual bool handlePotentialResponse(const PacketT & packet)` | 
`protected bool checkResponse(const PacketT & packet)` | 
`protected inline virtual void onResponse()` | Called when a successful response is received.
`protected inline virtual void onTimeout(void *)` | 

## Members

#### `public inline  PacketTransaction(long timeout,int retries,uv::Loop * loop)` {#group__base_1gadb8e26b65b3a60f24db3a0e8183a66a3}





#### `public inline  PacketTransaction(const PacketT & request,long timeout,int retries,uv::Loop * loop)` {#group__base_1gaf6a92e72b87b249648ccd46fc433f09c}





#### `public inline virtual bool send()` {#group__base_1gad94b9acb3f91d3a32499b86340b72d2e}



Starts the transaction timer and sends the request. Overriding classes should implement send logic here.

#### `public inline virtual void cancel()` {#group__base_1ga49c75e9316f50f660872dd4b249c28ca}



Cancellation means that the agent will not retransmit the request, will not treat the lack of response to be a failure, but will wait the duration of the transaction timeout for a response.

#### `public inline bool cancelled() const` {#group__base_1gaa54ffbb88648bcc1bba5c768bf86b74f}





#### `public inline virtual void dispose()` {#group__base_1ga620cd3aada050a2cd54a9ea34400c847}



Schedules the transaction for deferred deletion.

It is safe to call this function while the transaction is still active, providing the call is made from the same loop thread which the timer is running on.

Protected by the base implementation as this is called by the internal state machine.

#### `public inline virtual bool canResend()` {#group__base_1ga08ff3ee8b0be3e16a84699c44ea6a2a6}





#### `public inline int attempts() const` {#group__base_1ga4f6506c8e89b3d125fd7c08464c63688}





#### `public inline int retries() const` {#group__base_1ga34cb6bd2af610a3d8c8639a47d0713ef}





#### `public inline PacketT & request()` {#group__base_1gaa5c37750d66412b497c9130b5ad58cc2}





#### `public inline PacketT request() const` {#group__base_1ga44b16598078348c741c8903dfda2b848}





#### `public inline PacketT & response()` {#group__base_1gaa086ddf7d95b1866e89721c0c3ef0e5c}





#### `public inline PacketT response() const` {#group__base_1gaafd5b5bae5829fffac6d6f2bf81204bb}





#### `protected PacketT _request` {#group__base_1ga062276bf6ecdca9a64f8b89b6ba8e268}





#### `protected PacketT _response` {#group__base_1ga4c36f743181e84dedf6b71cd6acb5c04}





#### `protected `[`Timer`](api-base.md#classscy_1_1Timer)` _timer` {#group__base_1ga56cf3ef1cde1ed234a81adbc534a4d0c}





#### `protected int _timeout` {#group__base_1gaaf3c2bc4f0365bf13dbf7abad6ffb364}

The request timeout in milliseconds.



#### `protected int _retries` {#group__base_1ga3c72d7ce16d3b6defff73f7fb572ed2b}

The maximum number of attempts before the transaction is considered failed.



#### `protected int _attempts` {#group__base_1ga15a0177855becd97f06df6f4b24d59a9}

The number of times the transaction has been sent.



#### `protected bool _destroyed` {#group__base_1gae22c00abf59f34d7274f98bbd8e140f0}





#### `protected inline virtual  ~PacketTransaction()` {#group__base_1gac52c12fac214bb76b7b61c510672ed55}





#### `protected inline virtual void onStateChange(`[`TransactionState`](#structscy_1_1TransactionState)` & state,const `[`TransactionState`](#structscy_1_1TransactionState)` &)` {#group__base_1ga4c4e38262bc381d348c8c0f9b349c349}

Override to handle post state change logic.



#### `protected inline virtual bool handlePotentialResponse(const PacketT & packet)` {#group__base_1ga5111866a3ab35fdf98dab7b21a8cbb3d}



Processes a potential response candidate and updates the state accordingly.

#### `protected bool checkResponse(const PacketT & packet)` {#group__base_1ga2c0fb5cd9cd4f64f9600d04f2fc5cb34}



Checks a potential response candidate and returns true on successful match.

#### `protected inline virtual void onResponse()` {#group__base_1ga738b44f24205411c26bcb0ffe8ce6a80}

Called when a successful response is received.



#### `protected inline virtual void onTimeout(void *)` {#group__base_1ga587ddc82d2f9e3b6cf93b50c538190a5}





# class `scy::PointerCollection` {#classscy_1_1PointerCollection}

```
class scy::PointerCollection
  : public scy::AbstractCollection< TKey, TValue >
```  



This collection is used to maintain an map of any pointer type indexed by key value in a thread-safe way.

This class allows for custom memory handling of managed pointers via the TDeleter argument.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  PointerCollection()` | 
`public inline virtual  ~PointerCollection()` | 
`public inline virtual bool add(const TKey & key,TValue * item,bool whiny)` | 
`public inline virtual void update(const TKey & key,TValue * item)` | 
`public inline virtual TValue * get(const TKey & key,bool whiny) const` | 
`public inline virtual bool free(const TKey & key)` | 
`public inline virtual TValue * remove(const TKey & key)` | 
`public inline virtual bool remove(const TValue * item)` | 
`public inline virtual bool exists(const TKey & key) const` | 
`public inline virtual bool exists(const TValue * item) const` | 
`public inline virtual bool empty() const` | 
`public inline virtual int size() const` | 
`public inline virtual void clear()` | 
`public inline virtual Map map() const` | 
`public inline virtual Map & map()` | 
`public inline virtual void onAdd(const TKey &,TValue *)` | 
`public inline virtual void onRemove(const TKey &,TValue *)` | 
`protected Map _map` | 
`protected mutable `[`Mutex`](api-thread.md#classscy_1_1Mutex)` _mutex` | 

## Members

#### `public inline  PointerCollection()` {#group__base_1gab6664b90a66034c9edb47fa91235066f}





#### `public inline virtual  ~PointerCollection()` {#group__base_1ga07477833f5eb5db63304f827cee3efe0}





#### `public inline virtual bool add(const TKey & key,TValue * item,bool whiny)` {#group__base_1ga0c43d64e850210d46b9c281c9d4ebe1f}





#### `public inline virtual void update(const TKey & key,TValue * item)` {#group__base_1ga17e9fede03abe85799099d5b775dc2bd}





#### `public inline virtual TValue * get(const TKey & key,bool whiny) const` {#group__base_1ga4e8c9b02bb96b88d4627f66a4b14cc4b}





#### `public inline virtual bool free(const TKey & key)` {#group__base_1ga0145aa78faced91e4933db0fb8b90a18}





#### `public inline virtual TValue * remove(const TKey & key)` {#group__base_1gabc07ed8b47ac07e08b8aab4e56cf4596}





#### `public inline virtual bool remove(const TValue * item)` {#group__base_1ga0c681ea0dbdfa00b3a079e23b2d73d76}





#### `public inline virtual bool exists(const TKey & key) const` {#group__base_1ga5f465ace9a53dd924f294db6ae4bb970}





#### `public inline virtual bool exists(const TValue * item) const` {#group__base_1gaea838b8303433b6d2172f28acd750db3}





#### `public inline virtual bool empty() const` {#group__base_1ga24fb234ef9aff118711b47decc810f4d}





#### `public inline virtual int size() const` {#group__base_1ga25057e5ade72fd6a36006af9efe92582}





#### `public inline virtual void clear()` {#group__base_1ga7d7be8fe190371c7dfea3ea6de4d8571}





#### `public inline virtual Map map() const` {#group__base_1ga1ca85a3045a479bfd55dd6c405fb776e}





#### `public inline virtual Map & map()` {#group__base_1ga6c735e73584aa8707ea0f72e2c856ca9}





#### `public inline virtual void onAdd(const TKey &,TValue *)` {#group__base_1ga83228911febde3a7d64d2a75a1e30b27}





#### `public inline virtual void onRemove(const TKey &,TValue *)` {#group__base_1ga8d08d2001bf86f88a21624eb41e24fc8}





#### `protected Map _map` {#group__base_1gad5e5a4ff0a791488d3bba1da9c231a25}





#### `protected mutable `[`Mutex`](api-thread.md#classscy_1_1Mutex)` _mutex` {#group__base_1gadf9570794ed77aa862bd1f5744a1d015}





# class `scy::Process` {#classscy_1_1Process}

```
class scy::Process
  : public scy::uv::Handle
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::vector< std::string > args` | 
`public std::function< void(std::int64_t)> onexit` | Exit callback; returns the exit status.
`public ProcessOptions options` | [Process](api-base.md#classscy_1_1Process) options.
`public  Process(uv::Loop * loop)` | 
`public void spawn()` | 
`public bool kill(int signum)` | Kills the process.
`public int pid() const` | Returns the process PID.
`protected uv_process_t _proc` | 

## Members

#### `public std::vector< std::string > args` {#group__base_1gab717c10f12861cb06e94570f6e3cc955}



Command line args. STL proxy for options.args

#### `public std::function< void(std::int64_t)> onexit` {#group__base_1gacc4d49ab33dc77b331248ada30fd576e}

Exit callback; returns the exit status.



#### `public ProcessOptions options` {#group__base_1ga8fa7a36e401ff6b282c6a65cad8461ef}

[Process](#classscy_1_1Process) options.



#### `public  Process(uv::Loop * loop)` {#group__base_1ga21555aabd6288aeb0e2b7ef13dbc40ce}





#### `public void spawn()` {#group__base_1ga2b8587f848160ac9f84743a27cc28151}



Spawns the process. Options must be properly set. Throws and exception on error.

#### `public bool kill(int signum)` {#group__base_1ga941b00cc79b1e3e4ab3130a2406a6daf}

Kills the process.



#### `public int pid() const` {#group__base_1gab6b488740e07c049371ccdbac53ab50c}

Returns the process PID.



#### `protected uv_process_t _proc` {#group__base_1ga06c8830e4a84fb51e800aa4f34a69f29}





# class `scy::Queue` {#classscy_1_1Queue}


Implements a thread-safe queue container.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline void push(const T & data)` | 
`public inline bool empty() const` | 
`public inline T & front()` | 
`public inline T const  & front() const` | 
`public inline T & back()` | 
`public inline T const  & back() const` | 
`public inline void pop()` | 
`public template<typename Compare>`  <br/>`inline std::size_t sort()` | 
`public inline std::size_t size()` | 
`public inline std::deque< T > & queue()` | 

## Members

#### `public inline void push(const T & data)` {#group__base_1ga80ab35a22c083c80a6a7c717c2174c6a}





#### `public inline bool empty() const` {#group__base_1ga1bad28bab5f78677918cc73ceff03312}





#### `public inline T & front()` {#group__base_1ga28aac02ee039bf0b0d6e73ec329e1c2a}





#### `public inline T const  & front() const` {#group__base_1gaaaff769eea9a33aaacc54726d5752afb}





#### `public inline T & back()` {#group__base_1gab0c806356fdd175f09d234487f1ec712}





#### `public inline T const  & back() const` {#group__base_1gae9e07cbdbcf020e6ebbb592be692a7c2}





#### `public inline void pop()` {#group__base_1gaca35447c62090eb344640f15d75fd0b3}





#### `public template<typename Compare>`  <br/>`inline std::size_t sort()` {#group__base_1gaa9d9ff38f5d9174f86beab5e16123d62}





#### `public inline std::size_t size()` {#group__base_1gadac9317cc8af507fc8ca9a2886d5b1c8}





#### `public inline std::deque< T > & queue()` {#group__base_1ga12348bb136266eb41269c1ec64e50d27}





# class `scy::Random` {#classscy_1_1Random}




[Random](#classscy_1_1Random) implements a pseudo random number generator (PRNG). The PRNG is a nonlinear additive feedback random number generator using 256 bytes of state information and a period of up to 2^69.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Random(int stateSize)` | 
`public  ~Random()` | Destroys the PRNG.
`public void seed(std::uint32_t seed)` | Seeds the pseudo random generator with the given seed.
`public void seed()` | 
`public std::uint32_t next()` | Returns the next 31-bit pseudo random number.
`public std::uint32_t next(std::uint32_t n)` | Returns the next 31-bit pseudo random number modulo n.
`public char nextChar()` | Returns the next pseudo random character.
`public bool nextBool()` | Returns the next boolean pseudo random value.
`public float nextFloat()` | Returns the next float pseudo random number between 0.0 and 1.0.
`public double nextDouble()` | Returns the next double pseudo random number between 0.0 and 1.0.
`protected void initState(std::uint32_t seed,char * arg_state,std::int32_t n)` | 

## Members

#### `public  Random(int stateSize)` {#group__base_1ga658f3a6766a2f0207f378aef3e6a5f79}



Creates and initializes the PRNG. Specify either a state buffer size (8 to 256 bytes) or one of the Type values.

#### `public  ~Random()` {#group__base_1gae5781de4f82f83eb1d90b6a44a3d6a8f}

Destroys the PRNG.



#### `public void seed(std::uint32_t seed)` {#group__base_1ga79096dc8df35bb9668821e00d6455913}

Seeds the pseudo random generator with the given seed.



#### `public void seed()` {#group__base_1ga088b5bbefdc56704ad059271355d0087}



Seeds the pseudo random generator with a random seed obtained from a RandomInputStream.

#### `public std::uint32_t next()` {#group__base_1gaaaf027711ff5492bff5e8211ead1644f}

Returns the next 31-bit pseudo random number.



#### `public std::uint32_t next(std::uint32_t n)` {#group__base_1gad42ee913f8a588a6cd52035e63eed4a1}

Returns the next 31-bit pseudo random number modulo n.



#### `public char nextChar()` {#group__base_1ga539b6e8002927588265e941ff166d6ae}

Returns the next pseudo random character.



#### `public bool nextBool()` {#group__base_1gadc7c49f9a915ca4c8af9527bceefbffb}

Returns the next boolean pseudo random value.



#### `public float nextFloat()` {#group__base_1gac6ac62ca9dfb0e6566c3b8078a661032}

Returns the next float pseudo random number between 0.0 and 1.0.



#### `public double nextDouble()` {#group__base_1gadba7f18dddf46b7b34deeddf57fa5f74}

Returns the next double pseudo random number between 0.0 and 1.0.



#### `protected void initState(std::uint32_t seed,char * arg_state,std::int32_t n)` {#group__base_1ga4f27ef33e2c8d3e81d863c1762277590}





# class `scy::RawPacket` {#classscy_1_1RawPacket}

```
class scy::RawPacket
  : public scy::IPacket
```  



[RawPacket](#classscy_1_1RawPacket) is the default data packet type which consists of an optionally managed char pointer and a size value.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public char * _data` | 
`public std::size_t _size` | 
`public bool _free` | 
`public inline  RawPacket(char * data,std::size_t size,unsigned flags,void * source,void * opaque,`[`IPacketInfo`](#structscy_1_1IPacketInfo)` * info)` | 
`public inline  RawPacket(const char * data,std::size_t size,unsigned flags,void * source,void * opaque,`[`IPacketInfo`](#structscy_1_1IPacketInfo)` * info)` | 
`public inline  RawPacket(const `[`RawPacket`](#classscy_1_1RawPacket)` & that)` | 
`public inline virtual  ~RawPacket()` | 
`public inline virtual `[`IPacket`](#classscy_1_1IPacket)` * clone() const` | 
`public inline virtual void setData(char * data,std::size_t size)` | 
`public inline virtual void copyData(const char * data,std::size_t size)` | 
`public inline virtual std::size_t read(const `[`ConstBuffer`](#classscy_1_1ConstBuffer)` &)` | 
`public inline virtual void write(Buffer &) const` | 
`public inline virtual char * data() const` | The packet data pointer for buffered packets.
`public inline virtual std::size_t size() const` | 
`public inline virtual const char * className() const` | 
`public inline virtual bool ownsBuffer() const` | 
`public inline virtual void assignDataOwnership()` | 

## Members

#### `public char * _data` {#group__base_1ga846efb5bb786079552a8d91617ddf1d2}





#### `public std::size_t _size` {#group__base_1gad554fe982165c90f2c5cbd662237387d}





#### `public bool _free` {#group__base_1gaf2b753b24a1f34e84f9324d8050637fa}





#### `public inline  RawPacket(char * data,std::size_t size,unsigned flags,void * source,void * opaque,`[`IPacketInfo`](#structscy_1_1IPacketInfo)` * info)` {#group__base_1gac872d25ed18d8e4b3f21cd1d3c4a41b5}





#### `public inline  RawPacket(const char * data,std::size_t size,unsigned flags,void * source,void * opaque,`[`IPacketInfo`](#structscy_1_1IPacketInfo)` * info)` {#group__base_1ga26696b10a18d81c86daa70a2d5caac52}





#### `public inline  RawPacket(const `[`RawPacket`](#classscy_1_1RawPacket)` & that)` {#group__base_1ga8e345ba779f5ac06df86e2a6c7ec2ead}





#### `public inline virtual  ~RawPacket()` {#group__base_1ga1b58e7531cfebd845c24a180c1960da6}





#### `public inline virtual `[`IPacket`](#classscy_1_1IPacket)` * clone() const` {#group__base_1ga5d60f447c22ae8aeb649b0c59fe37578}





#### `public inline virtual void setData(char * data,std::size_t size)` {#group__base_1ga4e8ef4b13b41e2d72b6883ddab9bb9bf}





#### `public inline virtual void copyData(const char * data,std::size_t size)` {#group__base_1ga3df9ae6bb576cdc226c9e0fa22c2332a}





#### `public inline virtual std::size_t read(const `[`ConstBuffer`](#classscy_1_1ConstBuffer)` &)` {#group__base_1ga2fa8aec0d5020bba8911318dbadb6fd6}



Read/parse to the packet from the given input buffer. The number of bytes read is returned.

#### `public inline virtual void write(Buffer &) const` {#group__base_1gaa237661815dfe9a2879a816a851b8329}



Copy/generate to the packet given output buffer. The number of bytes written can be obtained from the buffer.

Todo: It may be prefferable to use our pod types here instead of buffer input, but the current codebase requires that the buffer be dynamically resizable for some protocols...

virtual std::size_t write(MutableBuffer&) const = 0;

#### `public inline virtual char * data() const` {#group__base_1ga1f2fa00a46e10f5c4ad7a082a8c39de0}

The packet data pointer for buffered packets.



#### `public inline virtual std::size_t size() const` {#group__base_1gad9f4526bf05bd934160e51b472ca7c50}



The size of the packet in bytes.

This is the nember of bytes that will be written on a call to [write()](#group__base_1gaa237661815dfe9a2879a816a851b8329), but may not be the number of bytes that will be consumed by [read()](#group__base_1ga2fa8aec0d5020bba8911318dbadb6fd6).

#### `public inline virtual const char * className() const` {#group__base_1gab5af246c37d39fc1010249f5fae58af5}





#### `public inline virtual bool ownsBuffer() const` {#group__base_1ga233325fe1e69fcfd13527e3791dbeeaa}





#### `public inline virtual void assignDataOwnership()` {#group__base_1ga6dd9c4f9e018952872cfd3d483e9eaac}





# class `scy::RotatingFileChannel` {#classscy_1_1RotatingFileChannel}

```
class scy::RotatingFileChannel
  : public scy::LogChannel
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  RotatingFileChannel(const std::string & name,const std::string & dir,LogLevel level,const std::string & extension,int rotationInterval,const char * timeFormat)` | 
`public virtual  ~RotatingFileChannel()` | 
`public virtual void write(const `[`LogStream`](#structscy_1_1LogStream)` & stream)` | 
`public virtual void rotate()` | 
`public inline std::string dir() const` | 
`public inline std::string filename() const` | 
`public inline int rotationInterval() const` | 
`public inline void setDir(const std::string & dir)` | 
`public inline void setExtension(const std::string & ext)` | 
`public inline void setRotationInterval(int interval)` | 
`protected std::ofstream * _fstream` | 
`protected std::string _dir` | 
`protected std::string _filename` | 
`protected std::string _extension` | 
`protected int _rotationInterval` | The log rotation interval in seconds.
`protected time_t _rotatedAt` | The time the log was last rotated.

## Members

#### `public  RotatingFileChannel(const std::string & name,const std::string & dir,LogLevel level,const std::string & extension,int rotationInterval,const char * timeFormat)` {#group__base_1ga47df1e3f9faf07db6b64c33ebc39e1d1}





#### `public virtual  ~RotatingFileChannel()` {#group__base_1gaac06718485f99a815fea1794918add66}





#### `public virtual void write(const `[`LogStream`](#structscy_1_1LogStream)` & stream)` {#group__base_1ga2cb30a31d04c959dc4de628f6fbad4fd}





#### `public virtual void rotate()` {#group__base_1ga6987d2d74c5bf0d806b910262dafc18b}





#### `public inline std::string dir() const` {#group__base_1gad734c262397421ebe099d6e721cc1825}





#### `public inline std::string filename() const` {#group__base_1gadd1ee0bd5e6a7b8e3c81dcb7d599e0a7}





#### `public inline int rotationInterval() const` {#group__base_1ga9507632f6b9471837ebcf1220ce9bee6}





#### `public inline void setDir(const std::string & dir)` {#group__base_1gac09e2028292bf1384f7f3c59bc79e4ea}





#### `public inline void setExtension(const std::string & ext)` {#group__base_1gada9e4f92369bf6b0e6085ef06f49a4a5}





#### `public inline void setRotationInterval(int interval)` {#group__base_1gadeb5c1ec6bed59029009039ebe623ef0}





#### `protected std::ofstream * _fstream` {#group__base_1ga063cadebdf2bae52c7d39ef0451d75d2}





#### `protected std::string _dir` {#group__base_1gaccead79b2337cea1e03927f09cc6eb2f}





#### `protected std::string _filename` {#group__base_1ga997dcf71593d8450e25f5f33b1e41dd4}





#### `protected std::string _extension` {#group__base_1gacbb348e159c2da3ce607710957a39bfb}





#### `protected int _rotationInterval` {#group__base_1ga6fcee513ef9e7648ca8b83691fcf8fe6}

The log rotation interval in seconds.



#### `protected time_t _rotatedAt` {#group__base_1gad6355b7ebfeeb0605c24c4934d45e912}

The time the log was last rotated.



# class `scy::RunnableQueue` {#classscy_1_1RunnableQueue}

```
class scy::RunnableQueue
  : public scy::Queue< T *>
  : public scy::async::Runnable
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::function< void(T &)> ondispatch` | Must be set before the queue is running.
`public inline  RunnableQueue(int limit,int timeout)` | 
`public inline virtual  ~RunnableQueue()` | 
`public inline virtual void push(T * item)` | The queue takes ownership of the item pointer.
`public inline virtual void flush()` | 
`public inline void clear()` | 
`public inline virtual void run()` | 
`public inline virtual void runTimeout()` | 
`public inline virtual void dispatch(T & item)` | 
`public inline void setTimeout(int milliseconds)` | int timeout() /// { /// Mutex::ScopedLock lock(_mutex); /// return _timeout; /// }
`protected int _limit` | 
`protected int _timeout` | 
`protected mutable `[`Mutex`](api-thread.md#classscy_1_1Mutex)` _mutex` | queue_t _queue;
`protected  RunnableQueue(const `[`RunnableQueue`](#classscy_1_1RunnableQueue)` &)` | 
`protected `[`RunnableQueue`](#classscy_1_1RunnableQueue)` & operator=(const `[`RunnableQueue`](#classscy_1_1RunnableQueue)` &)` | 
`protected inline virtual T * popNext()` | 
`protected inline virtual bool dispatchNext()` | 

## Members

#### `public std::function< void(T &)> ondispatch` {#group__base_1gaffe98a4f91bf2bc8131b59cb94006c9b}

Must be set before the queue is running.

The default dispatch function.

#### `public inline  RunnableQueue(int limit,int timeout)` {#group__base_1ga04ba6cebdaf72214752e3cb1ce867461}





#### `public inline virtual  ~RunnableQueue()` {#group__base_1ga52f804f6571e308e1dcba1c948969c23}





#### `public inline virtual void push(T * item)` {#group__base_1gac870c5b8456c3509e144517d055757dd}

The queue takes ownership of the item pointer.



#### `public inline virtual void flush()` {#group__base_1gaacc8fe709fdfef37c09b0a4b4223435e}





#### `public inline void clear()` {#group__base_1ga5a36945ff1d15379b4ae6de4046fb05c}





#### `public inline virtual void run()` {#group__base_1ga372c52aa2d9b4c33f9b620dfb2c2af23}



bool empty() /// { /// // Disabling mutex lock for bool check. //Mutex::ScopedLock lock(_mutex); /// return empty(); /// } If not timeout is set this method blocks until [cancel()](#group__base_1gadf150b3fdcef33cb5144119460c8729a) // is called, otherwise [runTimeout()](#group__base_1ga834803a488030d079458bc67563ce927) will be called. Pseudo protected for std::bind compatability.

#### `public inline virtual void runTimeout()` {#group__base_1ga834803a488030d079458bc67563ce927}



until the queue is empty or the timeout expires. Pseudo protected for std::bind compatability.

#### `public inline virtual void dispatch(T & item)` {#group__base_1gac8b4857c7505c81466387b3dc29c39e0}





#### `public inline void setTimeout(int milliseconds)` {#group__base_1gad0dfa024d9a58c4d98191c2fa52b8778}

int timeout() /// { /// Mutex::ScopedLock lock(_mutex); /// return _timeout; /// }



#### `protected int _limit` {#group__base_1gac4231be4ccc33b05adef9052c6b70938}





#### `protected int _timeout` {#group__base_1ga21cf39b5892920945baa2b2fff5c14e4}





#### `protected mutable `[`Mutex`](api-thread.md#classscy_1_1Mutex)` _mutex` {#group__base_1ga5b6ef24633663c2660805b302dd82a1e}

queue_t _queue;



#### `protected  RunnableQueue(const `[`RunnableQueue`](#classscy_1_1RunnableQueue)` &)` {#group__base_1ga8d373b558f3e907e9fd79358254ee885}





#### `protected `[`RunnableQueue`](#classscy_1_1RunnableQueue)` & operator=(const `[`RunnableQueue`](#classscy_1_1RunnableQueue)` &)` {#group__base_1ga689cc65219b7014e802121f774d63374}





#### `protected inline virtual T * popNext()` {#group__base_1gaf2abac57361a8aa7ed5ec0232ad7d31e}





#### `protected inline virtual bool dispatchNext()` {#group__base_1ga94ba51c70d311ed6ac2e8310de616e85}





# class `scy::ScopedConfiguration` {#classscy_1_1ScopedConfiguration}




[ScopedConfiguration](#classscy_1_1ScopedConfiguration) provides multiple levels of configuration for a module. Multiple levels means that there is a module level scope, and a default scope. When a property is accessed, the module scope value will be used if available, otherwise the default scope value will be used.

Example scoping: Module: channels.[name].modes.[name].[value] Default: modes.[name].[value]

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`Configuration`](api-base.md#classscy_1_1Configuration)` & config` | 
`public std::string currentScope` | 
`public std::string defaultScope` | 
`public  ScopedConfiguration(`[`Configuration`](#classscy_1_1Configuration)` & config,const std::string & currentScope,const std::string & defaultScope)` | 
`public  ScopedConfiguration(const `[`ScopedConfiguration`](#classscy_1_1ScopedConfiguration)` & that)` | 
`public std::string getString(const std::string & key,const std::string & defaultValue,bool forceDefaultScope) const` | 
`public int getInt(const std::string & key,int defaultValue,bool forceDefaultScope) const` | 
`public double getDouble(const std::string & key,double defaultValue,bool forceDefaultScope) const` | 
`public bool getBool(const std::string & key,bool defaultValue,bool forceDefaultScope) const` | 
`public void setString(const std::string & key,const std::string & value,bool defaultScope)` | 
`public void setInt(const std::string & key,int value,bool defaultScope)` | 
`public void setDouble(const std::string & key,double value,bool defaultScope)` | 
`public void setBool(const std::string & key,bool value,bool defaultScope)` | 
`public std::string getCurrentScope(const std::string & key) const` | 
`public std::string getDafaultKey(const std::string & key) const` | 
`public std::string getScopedKey(const std::string & key,bool defaultScope) const` | 

## Members

#### `public `[`Configuration`](api-base.md#classscy_1_1Configuration)` & config` {#group__base_1gab789db5bbe6952f38128f6a477b6cb45}





#### `public std::string currentScope` {#group__base_1gae55030c57f6402acba43d7d3ea38a415}





#### `public std::string defaultScope` {#group__base_1ga77d05f9ce766ee3f0a84a313ae83426b}





#### `public  ScopedConfiguration(`[`Configuration`](#classscy_1_1Configuration)` & config,const std::string & currentScope,const std::string & defaultScope)` {#group__base_1gab5e9573cc6a94c6b942aeb8fa75982bf}





#### `public  ScopedConfiguration(const `[`ScopedConfiguration`](#classscy_1_1ScopedConfiguration)` & that)` {#group__base_1ga2d9a45d573b97ec8d2fa4c5496bdedfa}





#### `public std::string getString(const std::string & key,const std::string & defaultValue,bool forceDefaultScope) const` {#group__base_1ga815efb31af29d03a283bd511d73865e1}





#### `public int getInt(const std::string & key,int defaultValue,bool forceDefaultScope) const` {#group__base_1ga054a3db8ef6052a2c244dfbb13309b9f}





#### `public double getDouble(const std::string & key,double defaultValue,bool forceDefaultScope) const` {#group__base_1ga2b3857cbbca439b3b825ce7f521fa061}





#### `public bool getBool(const std::string & key,bool defaultValue,bool forceDefaultScope) const` {#group__base_1ga7fe7def68b732c44e7c3835b89cc53dd}





#### `public void setString(const std::string & key,const std::string & value,bool defaultScope)` {#group__base_1ga4d0ed2a990ce5ab5b28c2ba2e06b13ad}





#### `public void setInt(const std::string & key,int value,bool defaultScope)` {#group__base_1ga94aac2d52d9eb5689797986c40edfabf}





#### `public void setDouble(const std::string & key,double value,bool defaultScope)` {#group__base_1ga72aa7eab9ef3496e461b5593b5edb8bf}





#### `public void setBool(const std::string & key,bool value,bool defaultScope)` {#group__base_1ga018ae27a4b9d4d2344df5723b1b01635}





#### `public std::string getCurrentScope(const std::string & key) const` {#group__base_1ga36f148de5018eff36e52c0c990b039dc}





#### `public std::string getDafaultKey(const std::string & key) const` {#group__base_1gac6effdc628b5f325c4585d0a06322894}





#### `public std::string getScopedKey(const std::string & key,bool defaultScope) const` {#group__base_1ga1b6e232dfd5b13c1eaff4408adc03b77}





# class `scy::ScopedPointer` {#classscy_1_1ScopedPointer}


Scoped Pointer Classes.

[ScopedPointer](#classscy_1_1ScopedPointer) provides an interface for holding and ansynchronously deleting a pointer in various ways.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  ScopedPointer()` | 
`public inline virtual  ~ScopedPointer()` | 

## Members

#### `public inline  ScopedPointer()` {#group__base_1ga91c4b85e5b3e5aef8667135f72db49bb}





#### `public inline virtual  ~ScopedPointer()` {#group__base_1ga0d9c3fc0d84dbf614447c01efa73bfb2}





# class `scy::ScopedRawPointer` {#classscy_1_1ScopedRawPointer}

```
class scy::ScopedRawPointer
  : public scy::ScopedPointer
```  



[ScopedRawPointer](#classscy_1_1ScopedRawPointer) implements the [ScopedPointer](#classscy_1_1ScopedPointer) interface to provide a method for deleting a raw pointer.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public void * ptr` | 
`public inline  ScopedRawPointer(void * p)` | 
`public inline virtual  ~ScopedRawPointer()` | 

## Members

#### `public void * ptr` {#group__base_1ga1c1e1af42525637a66a6ba5237380180}





#### `public inline  ScopedRawPointer(void * p)` {#group__base_1ga7e78ba90ac5895640c048e6bb0af5715}





#### `public inline virtual  ~ScopedRawPointer()` {#group__base_1ga0ab9d969f4befddc779357ae39dadb5d}





# class `scy::ScopedSharedPointer` {#classscy_1_1ScopedSharedPointer}

```
class scy::ScopedSharedPointer
  : public scy::ScopedPointer
```  



[ScopedSharedPointer](#classscy_1_1ScopedSharedPointer) implements the [ScopedPointer](#classscy_1_1ScopedPointer) interface to provide deferred deletion for shared_ptr managed pointers. Note that this class does not guarantee deletion of the managed pointer; all it does is copy the shared_ptr and release it when the [ScopedSharedPointer](#classscy_1_1ScopedSharedPointer) instance is deleted, which makes it useful for certain asyncronous scenarios.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::shared_ptr< T > ptr` | 
`public inline  ScopedSharedPointer(std::shared_ptr< T > p)` | 
`public inline virtual  ~ScopedSharedPointer()` | 

## Members

#### `public std::shared_ptr< T > ptr` {#group__base_1ga1e8741571abc89aa54884818bc077a57}





#### `public inline  ScopedSharedPointer(std::shared_ptr< T > p)` {#group__base_1ga742eade6fb3cb96566341745db050929}





#### `public inline virtual  ~ScopedSharedPointer()` {#group__base_1gad20697d1548f246dc3f9ab41b25b957a}





# class `scy::SharedObject` {#classscy_1_1SharedObject}




[SharedObject](#classscy_1_1SharedObject) is the base class for objects that employ reference counting based garbage collection.

Reference-counted objects inhibit construction by copying and assignment.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  SharedObject(bool deferred)` | 
`public inline void duplicate()` | Increment the object's reference count.
`public inline void release()` | 
`public inline unsigned refCount() const` | 
`protected std::atomic< unsigned > count` | 
`protected bool deferred` | 
`protected inline virtual void freeMemory()` | 
`protected inline virtual  ~SharedObject()` | 
`protected  SharedObject(const `[`SharedObject`](#classscy_1_1SharedObject)` &)` | 
`protected `[`SharedObject`](#classscy_1_1SharedObject)` & operator=(const `[`SharedObject`](#classscy_1_1SharedObject)` &)` | 

## Members

#### `public inline  SharedObject(bool deferred)` {#group__base_1gad63000b286982adf5356ccaa2912baf0}



Creates the [SharedObject](#classscy_1_1SharedObject) with an initial reference count of one.

#### `public inline void duplicate()` {#group__base_1ga498863f3b3f388a0796e1384f02dd4da}

Increment the object's reference count.



#### `public inline void release()` {#group__base_1gaab015702390ec5dafe91aacf80802834}



Decrement the object's reference count and calls delete if the count reaches zero.

#### `public inline unsigned refCount() const` {#group__base_1ga62553b1de80941caa9ab215e913e24b4}





#### `protected std::atomic< unsigned > count` {#group__base_1ga2080a97cd38d8d0711c2d9bffa4c3122}





#### `protected bool deferred` {#group__base_1gaeead0edd6c0087307c87f77f98d12a6d}





#### `protected inline virtual void freeMemory()` {#group__base_1ga137409f9aa0bacb5a46beb3058f26fde}



Deletes the instance when the reference count reaches zero. This method can be overridden for different deletion strategies.

#### `protected inline virtual  ~SharedObject()` {#group__base_1ga46ce2d135da598ebe6756b6f978f82b1}



Destroys the [SharedObject](#classscy_1_1SharedObject). The destructor should never be called directly.

#### `protected  SharedObject(const `[`SharedObject`](#classscy_1_1SharedObject)` &)` {#group__base_1gaf7716413141daa5cc28241add916b873}





#### `protected `[`SharedObject`](#classscy_1_1SharedObject)` & operator=(const `[`SharedObject`](#classscy_1_1SharedObject)` &)` {#group__base_1gaa138467c168935ff93c72b5c61ee1ae2}





# class `scy::Signal` {#classscy_1_1Signal}

```
class scy::Signal
  : public scy::SignalBase< DelegateBase< P >, P >
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------

## Members

# class `scy::Signal2` {#classscy_1_1Signal2}

```
class scy::Signal2
  : public scy::SignalBase< DelegateBase< P, P2 >, P, P2 >
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------

## Members

# class `scy::Signal3` {#classscy_1_1Signal3}

```
class scy::Signal3
  : public scy::SignalBase< DelegateBase< P, P2, P3 >, P, P2, P3 >
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------

## Members

# class `scy::Signal4` {#classscy_1_1Signal4}

```
class scy::Signal4
  : public scy::SignalBase< DelegateBase< P, P2, P3, P4 >, P, P2, P3, P4 >
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------

## Members

# class `scy::SignalBase` {#classscy_1_1SignalBase}




This class implements a thread-safe signal which broadcasts arbitrary data to multiple receiver delegates.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  SignalBase()` | 
`public inline virtual  ~SignalBase()` | 
`public inline void operator+=(const DelegateT & delegate)` | 
`public inline void operator-=(const DelegateT & delegate)` | 
`public inline void operator-=(const void * klass)` | 
`public inline void attach(const DelegateT & delegate)` | 
`public inline bool detach(const DelegateT & delegate)` | 
`public inline void detach(const void * klass)` | Detaches all delegates associated with the given class instance.
`public inline void cleanup()` | Deletes cancelled delegates.
`public inline void obtain(DelegateList & active)` | 
`public inline virtual void emit(void * sender)` | 
`public inline virtual void emit(void * sender,P arg)` | 
`public inline virtual void emit(void * sender,P arg,P2 arg2)` | 
`public inline virtual void emit(void * sender,P arg,P2 arg2,P3 arg3)` | 
`public inline virtual void emit(void * sender,P arg,P2 arg2,P3 arg3,P4 arg4)` | 
`public inline void clear()` | 
`public inline void enable(bool flag)` | 
`public inline bool enabled() const` | 
`public inline DelegateList delegates() const` | 
`public inline int ndelegates() const` | 
`protected DelegateList _delegates` | 
`protected bool _enabled` | 
`protected bool _dirty` | 
`protected int _count` | 
`protected mutable `[`Mutex`](api-thread.md#classscy_1_1Mutex)` _mutex` | 

## Members

#### `public inline  SignalBase()` {#group__base_1gaf59fa7dc8dd8aa11f0a3b52cd6a70442}





#### `public inline virtual  ~SignalBase()` {#group__base_1gaf1d5c65bde4637359b62230b2b366210}





#### `public inline void operator+=(const DelegateT & delegate)` {#group__base_1ga7e10d74e636632a19975570c4b198f4f}





#### `public inline void operator-=(const DelegateT & delegate)` {#group__base_1ga3612e81c0441de63c64c4a109b783a68}





#### `public inline void operator-=(const void * klass)` {#group__base_1ga6550c3f81bea0d4a0cd0aa8162705dfa}





#### `public inline void attach(const DelegateT & delegate)` {#group__base_1ga38e483d2d752c5c6999d55a8a0aa07c3}



Attaches a delegate to the signal. If the delegate already exists it will overwrite the previous delegate.

#### `public inline bool detach(const DelegateT & delegate)` {#group__base_1ga1cd2c449fd0ecfa89dbc56174a6139c1}



Detaches a delegate from the signal. Returns true if the delegate was detached, false otherwise.

#### `public inline void detach(const void * klass)` {#group__base_1ga781e801f45b7679e767f5977d52d5262}

Detaches all delegates associated with the given class instance.



#### `public inline void cleanup()` {#group__base_1gab5007309eccbd4c7faa6b41420fc10da}

Deletes cancelled delegates.



#### `public inline void obtain(DelegateList & active)` {#group__base_1gabf0d710eb0ad8b6cc5c52fb2245c3a59}



Retrieves a list of active delegates while simultaneously deleting any redundant delegates.

#### `public inline virtual void emit(void * sender)` {#group__base_1ga0e1385097955d44c3df2330f7b44074f}





#### `public inline virtual void emit(void * sender,P arg)` {#group__base_1ga18ddc31b671b097c84b433894657b0d8}





#### `public inline virtual void emit(void * sender,P arg,P2 arg2)` {#group__base_1gafa1225e73367a3671e614f9c83afdd87}





#### `public inline virtual void emit(void * sender,P arg,P2 arg2,P3 arg3)` {#group__base_1gaae055444fca242b18f030950458e354c}





#### `public inline virtual void emit(void * sender,P arg,P2 arg2,P3 arg3,P4 arg4)` {#group__base_1ga1239308100dbb8410e0df88f54af0455}





#### `public inline void clear()` {#group__base_1ga5b8b00d4f6789e62162e391150e6a2e4}





#### `public inline void enable(bool flag)` {#group__base_1ga636d9306d7c507c754732a7a71450cc6}





#### `public inline bool enabled() const` {#group__base_1ga81035ff58a89ba5a887843856a0c211f}





#### `public inline DelegateList delegates() const` {#group__base_1ga312d51d3f8f2afa5f1702273ae5191fc}





#### `public inline int ndelegates() const` {#group__base_1ga778ea872541e59894fdba9d89864b9d5}



Returns the number of delegates connected to the signal. Use this instead of delegates().size() since the container is not updated in real time.

#### `protected DelegateList _delegates` {#group__base_1gad533551492e7e01158abffa68837a248}





#### `protected bool _enabled` {#group__base_1ga29d24530639efd2101c32fdd3f3a41e8}





#### `protected bool _dirty` {#group__base_1gaa470afedc3badb45c39fbc8f74b58e34}





#### `protected int _count` {#group__base_1gac6f8ca597b88ce1b7b6e117b2eb5c5b1}





#### `protected mutable `[`Mutex`](api-thread.md#classscy_1_1Mutex)` _mutex` {#group__base_1ga77737ea2ca1576cbdcd29f1f668313ff}





# class `scy::Singleton` {#classscy_1_1Singleton}




This is a helper template class for managing singleton objects allocated on the heap.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  Singleton()` | Creates the [Singleton](api-base.md#classscy_1_1Singleton) wrapper.
`public inline  ~Singleton()` | 
`public inline S * get()` | 
`public inline S * swap(S * newPtr)` | 
`public inline void destroy()` | Destroys the managed singleton instance.

## Members

#### `public inline  Singleton()` {#group__base_1gac8b29b8a152c5199b23419d52f7bb266}

Creates the [Singleton](#classscy_1_1Singleton) wrapper.



#### `public inline  ~Singleton()` {#group__base_1ga6c3200daffcfad751bd8a1ea01d6f77d}



Destroys the [Singleton](#classscy_1_1Singleton) wrapper and the managed singleton instance it holds.

#### `public inline S * get()` {#group__base_1ga4a7ed440457031001625e0f1e61ad00d}



Returns a pointer to the singleton object hold by the [Singleton](#classscy_1_1Singleton). The first call to get on a nullptr singleton will instantiate the singleton.

#### `public inline S * swap(S * newPtr)` {#group__base_1gaf4547efe2aefa7f8539bbe0f438eef09}



Swaps the old pointer with the new one and returns the old instance.

#### `public inline void destroy()` {#group__base_1ga30845630ba9bebeb153d5519f46e9aef}

Destroys the managed singleton instance.



# class `scy::State` {#classscy_1_1State}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  State(ID id,const std::string & message)` | 
`public inline virtual  ~State()` | 
`public virtual ID id() const` | 
`public virtual void set(ID id)` | 
`public virtual std::string message() const` | 
`public virtual void setMessage(const std::string & message)` | 
`public virtual std::string str(ID id) const` | 
`public virtual std::string toString() const` | 
`public inline virtual bool between(ID lid,ID rid) const` | 
`public inline virtual bool equals(ID id) const` | 
`public inline bool operator==(const `[`State`](#classscy_1_1State)` & r)` | 
`public inline bool operator==(const State::ID & r)` | 
`protected ID _id` | 
`protected std::string _message` | 

## Members

#### `public  State(ID id,const std::string & message)` {#group__base_1ga933419189a1b0305b206ce9de1543968}





#### `public inline virtual  ~State()` {#group__base_1gae0add030bbe46fa8bf0b1d7ef12cfbbe}





#### `public virtual ID id() const` {#group__base_1gad5b4dd779ec160d101eb3f780c24d04f}





#### `public virtual void set(ID id)` {#group__base_1gac7b685fc2715f9cac21d14310fbd08c6}





#### `public virtual std::string message() const` {#group__base_1ga01fce872e43a692a437c315cfe8c773a}





#### `public virtual void setMessage(const std::string & message)` {#group__base_1gaf01895f15ffa2a810d2f6642315e6e48}





#### `public virtual std::string str(ID id) const` {#group__base_1gaf08ad3e3c5e32fddda195ff37115278b}





#### `public virtual std::string toString() const` {#group__base_1ga328e6a3b702375b0ebcf56dafceadda0}





#### `public inline virtual bool between(ID lid,ID rid) const` {#group__base_1ga114fe3d98c151eae036d15890396b0c1}





#### `public inline virtual bool equals(ID id) const` {#group__base_1gacd96cfd8cce44f2ed96c1eb0ba011b1d}





#### `public inline bool operator==(const `[`State`](#classscy_1_1State)` & r)` {#group__base_1ga469436328d286064474afd0e3ebc06ce}





#### `public inline bool operator==(const State::ID & r)` {#group__base_1ga7112f7a2efb2ceff501b830fa5c3814d}





#### `protected ID _id` {#group__base_1gad0836ca88d5caafb2dcece58f3a57fd1}





#### `protected std::string _message` {#group__base_1gace82c13f902422a67cb2741fef409db4}





# class `scy::Stateful` {#classscy_1_1Stateful}




This class implements a simple state machine. T should be a derived [State](#classscy_1_1State) type.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`Signal2`](api-base.md#classscy_1_1Signal2)`< T &, const T & > StateChange` | Signals when the state changes.
`public inline  Stateful()` | 
`public inline virtual  ~Stateful()` | 
`public inline virtual bool stateEquals(unsigned int id) const` | 
`public inline virtual bool stateBetween(unsigned int lid,unsigned int rid) const` | 
`public inline virtual T & state()` | 
`public inline virtual const T state() const` | 
`protected T _state` | 
`protected inline virtual bool beforeStateChange(const T & state)` | 
`protected inline virtual void onStateChange(T &,const T &)` | Override to handle post state change logic.
`protected inline virtual bool setState(void * sender,unsigned int id,const std::string & message)` | 
`protected inline virtual bool setState(void * sender,const T & state)` | 
`protected inline virtual void setStateMessage(const std::string & message)` | 

## Members

#### `public `[`Signal2`](api-base.md#classscy_1_1Signal2)`< T &, const T & > StateChange` {#group__base_1ga87c05d98e9c7ed230e65617895a12ab1}

Signals when the state changes.



#### `public inline  Stateful()` {#group__base_1ga17f56a2225eb139ebf730715844ae093}





#### `public inline virtual  ~Stateful()` {#group__base_1ga88e8a5a4d0be0a7c8b4bcf8283e8118a}





#### `public inline virtual bool stateEquals(unsigned int id) const` {#group__base_1ga55370a83a7d614570298ef6124380d95}





#### `public inline virtual bool stateBetween(unsigned int lid,unsigned int rid) const` {#group__base_1ga46ee7c073892ac21a6c7078dfd8dc49f}





#### `public inline virtual T & state()` {#group__base_1ga5f049a3a48933e1dae7c7906ddc8e2f6}





#### `public inline virtual const T state() const` {#group__base_1gaf74b1907ca291f0fe0288e7fed013a33}





#### `protected T _state` {#group__base_1gaa772242e3f30125cb5006f757f6d9021}





#### `protected inline virtual bool beforeStateChange(const T & state)` {#group__base_1gaa8d284c63cb789cd53c36b06ff3bcc6f}



Override to handle pre state change logic. Return false to prevent state change.

#### `protected inline virtual void onStateChange(T &,const T &)` {#group__base_1ga3c88b4aa8e9343f9bdc1a8c2ae2a5452}

Override to handle post state change logic.



#### `protected inline virtual bool setState(void * sender,unsigned int id,const std::string & message)` {#group__base_1ga537abda617b5b77adb025767afab22d0}



Sets the state and sends the state signal if the state change was successful.

#### `protected inline virtual bool setState(void * sender,const T & state)` {#group__base_1gab22136dc42f0b6352249274fd13d4ef0}



Sets the state and sends the state signal if the state change was successful.

#### `protected inline virtual void setStateMessage(const std::string & message)` {#group__base_1ga8154fd0e0aa70810133a552336789419}





# class `scy::StateSignal` {#classscy_1_1StateSignal}

```
class scy::StateSignal
  : public scy::MutexState
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  StateSignal(ID id)` | 
`public  StateSignal(const `[`StateSignal`](#classscy_1_1StateSignal)` & r)` | 
`public inline virtual  ~StateSignal()` | 
`public virtual bool change(ID id)` | 
`public virtual bool canChange(ID id)` | 
`public virtual void onChange(ID id,ID prev)` | 
`protected virtual void set(ID id)` | 

## Members

#### `public  StateSignal(ID id)` {#group__base_1gadc8be2f6a8166a68aa9d722a1e17d1c7}





#### `public  StateSignal(const `[`StateSignal`](#classscy_1_1StateSignal)` & r)` {#group__base_1gae58288fc312a61242e4e28e1684a0227}





#### `public inline virtual  ~StateSignal()` {#group__base_1gacfc6193d36f3f0df310e8cc7f30b6b67}





#### `public virtual bool change(ID id)` {#group__base_1ga65674be69d27612cdf28164c67e19f30}





#### `public virtual bool canChange(ID id)` {#group__base_1ga002e735fa1210617b9504087fd945bdc}





#### `public virtual void onChange(ID id,ID prev)` {#group__base_1gae032a097bde94676bfa38ba222eb507f}





#### `protected virtual void set(ID id)` {#group__base_1ga3ca25f82efc82cf2113a046f5f0d16dc}



Fired when the state changes to signal the new and previous states.

# class `scy::StopPropagation` {#classscy_1_1StopPropagation}

```
class scy::StopPropagation
  : public exception
```  

This exception is used to break out of a [Signal](#classscy_1_1Signal) callback scope.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual  ~StopPropagation()` | 

## Members

#### `public inline virtual  ~StopPropagation()` {#group__base_1ga4b88cfd276388776d7c3384b64bdb39d}





# class `scy::Stopwatch` {#classscy_1_1Stopwatch}




A simple facility to measure time intervals with microsecond resolution.

The [Stopwatch](#classscy_1_1Stopwatch) uses the current system time, so if the system time changes the measured time will be incorrect.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Stopwatch()` | 
`public  ~Stopwatch()` | 
`public void start()` | Starts (or restarts) the stopwatch.
`public void stop()` | Stops or pauses the stopwatch.
`public void reset()` | Resets the stopwatch.
`public void restart()` | Resets and starts the stopwatch.
`public `[`Timestamp::TimeDiff`](#group__base_1ga9f06831e4e9b9cbc6f061cc61dc91fd0)` elapsed() const` | 
`public int elapsedSeconds() const` | 
`public int elapsedMilliseconds() const` | 

## Members

#### `public  Stopwatch()` {#group__base_1gac4e0a026e16191af82dd820b98d10dfb}





#### `public  ~Stopwatch()` {#group__base_1ga8c4cf8243072ce31339f003a06a7f2e6}





#### `public void start()` {#group__base_1ga668b373cd0428065e574eb0f8bf591a5}

Starts (or restarts) the stopwatch.



#### `public void stop()` {#group__base_1ga4843077bf122dca1078f9fa84c5a9b84}

Stops or pauses the stopwatch.



#### `public void reset()` {#group__base_1ga8d1261800ad12511d76821ea78c76611}

Resets the stopwatch.



#### `public void restart()` {#group__base_1ga67dc5bd64beea0d85f8a691b0bb2f7fb}

Resets and starts the stopwatch.



#### `public `[`Timestamp::TimeDiff`](#group__base_1ga9f06831e4e9b9cbc6f061cc61dc91fd0)` elapsed() const` {#group__base_1ga2faaf087859f8cd8443a9b0dc61e2fbb}



Returns the elapsed time in microseconds since the stopwatch started.

#### `public int elapsedSeconds() const` {#group__base_1ga004d96123d0dd9f3ddb14e1c6d5ea9a3}



Returns the number of seconds elapsed since the stopwatch started.

#### `public int elapsedMilliseconds() const` {#group__base_1ga1a1d93858e58200b50061d20d3f8f0e6}



Returns the number of milliseconds elapsed since the stopwatch started.

# class `scy::Stream` {#classscy_1_1Stream}

```
class scy::Stream
  : public scy::uv::Handle
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`Signal2`](api-base.md#classscy_1_1Signal2)`< const char *, const int & > Read` | Signals when data can be read from the stream.
`public inline  Stream(uv::Loop * loop,void * stream)` | 
`public inline virtual void close()` | 
`public inline bool shutdown()` | 
`public inline bool write(const char * data,std::size_t len)` | 
`public inline Buffer & buffer()` | 
`public inline virtual bool closed() const` | Returns true if the native socket handle is closed.
`protected Buffer _buffer` | 
`protected inline virtual bool readStart()` | 
`protected inline virtual bool readStop()` | 
`protected inline virtual void onRead(const char * data,std::size_t len)` | 
`protected inline virtual  ~Stream()` | 
`protected inline virtual void * self()` | 

## Members

#### `public `[`Signal2`](api-base.md#classscy_1_1Signal2)`< const char *, const int & > Read` {#group__base_1ga44340acfa09938bfef60507153be4ee3}

Signals when data can be read from the stream.



#### `public inline  Stream(uv::Loop * loop,void * stream)` {#group__base_1ga6d99cf399c543f315ce98fbc01de605e}





#### `public inline virtual void close()` {#group__base_1ga9a71a3a51d802d91b9f9febe495f5a52}



Closes and resets the stream handle. This will close the active socket/pipe and destroy the uv_stream_t handle.

If the stream is already closed this call will have no side-effects.

#### `public inline bool shutdown()` {#group__base_1ga7d66d130113efb136ad2a0ff8a1f5355}



Sends a shutdown packet to the connected peer. Returns true if the shutdown packet was sent.

#### `public inline bool write(const char * data,std::size_t len)` {#group__base_1gabb8e84063aee8ca47d6a0201a54edb75}



Writes data to the stream.

Returns false if the underlying socket is closed. This method does not throw an exception.

#### `public inline Buffer & buffer()` {#group__base_1ga6e2a0a0bd7c06572b3d7e69d2d7c504b}





#### `public inline virtual bool closed() const` {#group__base_1gae03ec2cec706c0e1495feabc59491ad9}

Returns true if the native socket handle is closed.



#### `protected Buffer _buffer` {#group__base_1ga95ac82efa2bdd682ac490c84e77c1861}





#### `protected inline virtual bool readStart()` {#group__base_1gaa9690cf7d34b6dcfc3ff3821311747d7}





#### `protected inline virtual bool readStop()` {#group__base_1ga6f4a63c87b48df6db07afe8b9a2abdfd}





#### `protected inline virtual void onRead(const char * data,std::size_t len)` {#group__base_1gaa38c150bdde634b05adc119c502c262f}





#### `protected inline virtual  ~Stream()` {#group__base_1ga124f57b420370c5891a76bb8d050cfe8}





#### `protected inline virtual void * self()` {#group__base_1ga5199b21fa4be2c0b2e6f160f1c85d617}





# class `scy::StreamWriter` {#classscy_1_1StreamWriter}

```
class scy::StreamWriter
  : public scy::PacketProcessor
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`PacketSignal`](api-base.md#classscy_1_1SignalBase)` emitter` | 
`public inline  StreamWriter(std::ostream * stream)` | 
`public inline virtual  ~StreamWriter()` | 
`public inline virtual void process(`[`IPacket`](#classscy_1_1IPacket)` & packet)` | 
`public template<class StreamT>`  <br/>`inline StreamT & stream()` | 
`public inline std::ostream & stream()` | 
`protected std::ostream * _ostream` | 

## Members

#### `public `[`PacketSignal`](api-base.md#classscy_1_1SignalBase)` emitter` {#group__base_1ga1996908eb8e6301955ff4df97c5a22a7}





#### `public inline  StreamWriter(std::ostream * stream)` {#group__base_1gad15868ece5ffa0a63ae71ab555b0d43f}





#### `public inline virtual  ~StreamWriter()` {#group__base_1ga0bdd60e6b4e56dcde24d2b7f372d55e2}





#### `public inline virtual void process(`[`IPacket`](#classscy_1_1IPacket)` & packet)` {#group__base_1ga941680e41f4597378f2c41a9ac036afd}



This method performs processing on the given packet and emits the result.

Note: If packet processing is async (the packet is not in the current thread scope) then packet data must be copied. Copied data can be freed directly aFter the async call to emit() the outgoing packet.

#### `public template<class StreamT>`  <br/>`inline StreamT & stream()` {#group__base_1ga632e161d32fea07fd9cd40f6062d4b3a}





#### `public inline std::ostream & stream()` {#group__base_1ga82bde741508d5245a589923de03647ba}





#### `protected std::ostream * _ostream` {#group__base_1ga2e9fe16cc8bec4c2ab33673eda06f4ff}





# class `scy::SyncContext` {#classscy_1_1SyncContext}

```
class scy::SyncContext
  : public scy::async::Runner
```  



[SyncContext](#classscy_1_1SyncContext) enables any thread to communicate with the associated event loop via synchronized callbacks.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  SyncContext(uv::Loop * loop)` | 
`public  SyncContext(uv::Loop * loop,std::function< void()> target)` | 
`public  SyncContext(uv::Loop * loop,std::function< void(void *)> target,void * arg)` | 
`public virtual  ~SyncContext()` | 
`public void post()` | 
`public virtual void cancel()` | 
`public virtual void close()` | 
`public virtual bool closed()` | 
`public `[`uv::Handle`](#classscy_1_1uv_1_1Handle)` & handle()` | 
`protected `[`uv::Handle`](api-uv.md#classscy_1_1uv_1_1Handle)` _handle` | 
`protected virtual void startAsync()` | Start the context from the control thread.
`protected virtual bool async() const` | 

## Members

#### `public  SyncContext(uv::Loop * loop)` {#group__base_1ga7f538b8fe81e9c6aa2996504dd16dffc}



Create the synchronization context the given event loop and method. The target method will be called from the event loop context.

#### `public  SyncContext(uv::Loop * loop,std::function< void()> target)` {#group__base_1gaa0b8424721da617ed39faa568640576b}





#### `public  SyncContext(uv::Loop * loop,std::function< void(void *)> target,void * arg)` {#group__base_1ga34fb9ffaf1dec0d466af03a94822cda7}





#### `public virtual  ~SyncContext()` {#group__base_1ga5f9b854709966bb8d5296551c09ea9aa}





#### `public void post()` {#group__base_1gaa48fafd2575f13d63100b61ed4f05f5d}



Send a synchronization request to the event loop. Call this each time you want the target method called synchronously. The synchronous method will be called on next iteration. This is not atomic, so do not expect a callback for every request.

#### `public virtual void cancel()` {#group__base_1ga255de9fabd75a256ad2f8c041783f3d4}





#### `public virtual void close()` {#group__base_1gac719de34d111dc84c7ef92cb883e1579}





#### `public virtual bool closed()` {#group__base_1gac79fcba6dad7a7c4d886d6b333a714a0}





#### `public `[`uv::Handle`](#classscy_1_1uv_1_1Handle)` & handle()` {#group__base_1gad884084257f521d83c42d2e5fc61f635}





#### `protected `[`uv::Handle`](api-uv.md#classscy_1_1uv_1_1Handle)` _handle` {#group__base_1ga83a319d3ca515ba644f59d0c470950c9}





#### `protected virtual void startAsync()` {#group__base_1ga090abce165e3934146312722d36648be}

Start the context from the control thread.



#### `protected virtual bool async() const` {#group__base_1ga7a996e343ddb26f3615f9735629f54c9}



Returns true if the implementation is thread-based, or false if it belongs to an event loop.

# class `scy::SyncPacketQueue` {#classscy_1_1SyncPacketQueue}

```
class scy::SyncPacketQueue
  : public scy::SyncQueue< T >
  : public scy::PacketProcessor
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`PacketSignal`](api-base.md#classscy_1_1SignalBase)` emitter` | 
`public inline  SyncPacketQueue(uv::Loop * loop,int maxSize)` | 
`public inline  SyncPacketQueue(int maxSize)` | 
`public inline virtual  ~SyncPacketQueue()` | 
`public inline virtual void process(`[`IPacket`](#classscy_1_1IPacket)` & packet)` | 
`public inline virtual bool accepts(`[`IPacket`](#classscy_1_1IPacket)` &)` | 
`protected inline virtual void dispatch(T & packet)` | 
`protected inline virtual void onStreamStateChange(const `[`PacketStreamState`](#structscy_1_1PacketStreamState)` &)` | 

## Members

#### `public `[`PacketSignal`](api-base.md#classscy_1_1SignalBase)` emitter` {#group__base_1ga9cb2e51e85e32907283c98c62fc5967d}





#### `public inline  SyncPacketQueue(uv::Loop * loop,int maxSize)` {#group__base_1gae6d88a8cc455a3fcc463e302a4fa1b1f}





#### `public inline  SyncPacketQueue(int maxSize)` {#group__base_1ga10b57a26379bac19ff0f4c48b367ab18}





#### `public inline virtual  ~SyncPacketQueue()` {#group__base_1gaaddebcff191f315fa84dd988bb9f9f64}





#### `public inline virtual void process(`[`IPacket`](#classscy_1_1IPacket)` & packet)` {#group__base_1ga080f412e9e91140f1e77d903b75999e8}



This method performs processing on the given packet and emits the result.

Note: If packet processing is async (the packet is not in the current thread scope) then packet data must be copied. Copied data can be freed directly aFter the async call to emit() the outgoing packet.

#### `public inline virtual bool accepts(`[`IPacket`](#classscy_1_1IPacket)` &)` {#group__base_1ga66e1123be201dc3a4ec39763e435ef10}



This method ensures compatibility with the given packet type. Return false to reject the packet.

#### `protected inline virtual void dispatch(T & packet)` {#group__base_1ga11c2c12c52dd842cd37d83ea8f05147c}



Emit should never be called after closure. Any late packets should have been dealt with and dropped by the [run()](#group__base_1ga372c52aa2d9b4c33f9b620dfb2c2af23) function.

#### `protected inline virtual void onStreamStateChange(const `[`PacketStreamState`](#structscy_1_1PacketStreamState)` &)` {#group__base_1ga37f27e601fe6cd6e73c5b3da83b7634d}



Called by the [PacketStream](#classscy_1_1PacketStream) to notify when the internal [Stream](#classscy_1_1Stream) state changes. On receiving the Stopped state, it is the responsibility of the adapter to have ceased all outgoing packet transmission, especially in multi-thread scenarios.

# class `scy::SyncQueue` {#classscy_1_1SyncQueue}

```
class scy::SyncQueue
  : public scy::RunnableQueue< T >
```  



[SyncQueue](#classscy_1_1SyncQueue) extends [SyncContext](#classscy_1_1SyncContext) to implement a synchronized FIFO queue which receives T objects from any thread and synchronizes them for safe consumption by the associated event loop.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  SyncQueue(uv::Loop * loop,int limit,int timeout)` | 
`public inline virtual  ~SyncQueue()` | time for all callbacks to return.
`public inline virtual void push(T * item)` | Item pointers are now managed by the [SyncQueue](api-base.md#classscy_1_1SyncQueue).
`public inline virtual void cancel()` | 
`public inline `[`SyncContext`](#classscy_1_1SyncContext)` & sync()` | 
`protected `[`SyncContext`](api-base.md#classscy_1_1SyncContext)` _sync` | 

## Members

#### `public inline  SyncQueue(uv::Loop * loop,int limit,int timeout)` {#group__base_1gae1cf6a31580baa5e4455d193ba3c0b54}





#### `public inline virtual  ~SyncQueue()` {#group__base_1ga9539ee4bbac1c32eb8c315a15ee0c9b3}

time for all callbacks to return.



#### `public inline virtual void push(T * item)` {#group__base_1ga807c4a3d7d9adf4c15fd9b78121ca830}

Item pointers are now managed by the [SyncQueue](#classscy_1_1SyncQueue).



#### `public inline virtual void cancel()` {#group__base_1gafd7fe7eff78132084172f272e76c37b7}





#### `public inline `[`SyncContext`](#classscy_1_1SyncContext)` & sync()` {#group__base_1gaa41a7c0c9525c7d4885011d5d1298b25}





#### `protected `[`SyncContext`](api-base.md#classscy_1_1SyncContext)` _sync` {#group__base_1ga2c448333acaf9de512e8487deb4b7eab}





# class `scy::Task` {#classscy_1_1Task}

```
class scy::Task
  : public scy::async::Runnable
```  



This class is for implementing any kind async task that is compatible with a [TaskRunner](#classscy_1_1TaskRunner).

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Task(bool repeat)` | 
`public virtual void destroy()` | Sets the task to destroyed state.
`public virtual bool destroyed() const` | Signals that the task should be disposed of.
`public virtual bool repeating() const` | 
`public virtual std::uint32_t id() const` | Unique task ID.
`protected std::uint32_t _id` | 
`protected bool _repeating` | 
`protected bool _destroyed` | 
`protected  Task(const `[`Task`](#classscy_1_1Task)` & task)` | 
`protected `[`Task`](#classscy_1_1Task)` & operator=(`[`Task`](#classscy_1_1Task)` const &)` | 
`protected virtual  ~Task()` | Should remain protected.
`protected void run()` | 

## Members

#### `public  Task(bool repeat)` {#group__base_1ga785465536aec43f179f47c7439e44aa8}





#### `public virtual void destroy()` {#group__base_1gaecd8a85018f8f75ae802dcbf3749bdef}

Sets the task to destroyed state.



#### `public virtual bool destroyed() const` {#group__base_1ga7a28372139954b0ff0c96f99be0418f2}

Signals that the task should be disposed of.



#### `public virtual bool repeating() const` {#group__base_1gaed5e6c02f8118ab86c7ff620ee2ddf48}



Signals that the task's should be called repeatedly by the [TaskRunner](#classscy_1_1TaskRunner). If this returns false the task will be [cancelled()](#group__base_1ga7efc7639f668b72480625cc0f34f1eaa)

#### `public virtual std::uint32_t id() const` {#group__base_1gac94a3e12533ef624fbd605102ccaf06d}

Unique task ID.



#### `protected std::uint32_t _id` {#group__base_1gad53fa7ea0b36b2a023521f3e94c22d2d}





#### `protected bool _repeating` {#group__base_1gae5fcfd4789448c078bbc04a24b00849a}





#### `protected bool _destroyed` {#group__base_1ga930e56a65dfd577c40fe7d30f3018e68}





#### `protected  Task(const `[`Task`](#classscy_1_1Task)` & task)` {#group__base_1ga663f4dcc05db829e8cb1335ad821f4c4}





#### `protected `[`Task`](#classscy_1_1Task)` & operator=(`[`Task`](#classscy_1_1Task)` const &)` {#group__base_1ga6419797a6b2f05ccac8c3c6b5bcf18b3}





#### `protected virtual  ~Task()` {#group__base_1gac05de8a5d56fb224bee75277714de893}

Should remain protected.



#### `protected void run()` {#group__base_1ga0bfb09779b8f6aa9c5e3ce7dd7e4cfe3}



Called by the [TaskRunner](#classscy_1_1TaskRunner) to run the task. Override this method to implement task action. Returning true means the true should be called again, and false will cause the task to be destroyed. The task will similarly be destroyed id [destroy()](#group__base_1gaecd8a85018f8f75ae802dcbf3749bdef) was called during the current task iteration.

# class `scy::TaskRunner` {#classscy_1_1TaskRunner}

```
class scy::TaskRunner
  : public scy::async::Runnable
```  



The [TaskRunner](#classscy_1_1TaskRunner) is an asynchronous event loop in charge of running one or many tasks.

The [TaskRunner](#classscy_1_1TaskRunner) continually loops through each task in the task list calling the task's [run()](#group__base_1ga07a481d667f1a9c67e881eedcffc10a5) method.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`NullSignal`](api-base.md#classscy_1_1NullSignal)` Idle` | Fires after completing an iteration of all tasks.
`public `[`NullSignal`](api-base.md#classscy_1_1NullSignal)` Shutdown` | Signals when the [TaskRunner](api-base.md#classscy_1_1TaskRunner) is shutting down.
`public  TaskRunner(async::Runner::Ptr runner)` | 
`public virtual  ~TaskRunner()` | 
`public virtual bool start(`[`Task`](#classscy_1_1Task)` * task)` | Starts a task, adding it if it doesn't exist.
`public virtual bool cancel(`[`Task`](#classscy_1_1Task)` * task)` | 
`public virtual bool destroy(`[`Task`](#classscy_1_1Task)` * task)` | Queues a task for destruction.
`public virtual bool exists(`[`Task`](#classscy_1_1Task)` * task) const` | Returns weather or not a task exists.
`public virtual `[`Task`](#classscy_1_1Task)` * get(std::uint32_t id) const` | 
`public virtual void setRunner(async::Runner::Ptr runner)` | 
`public inline virtual const char * className() const` | 
`protected mutable `[`Mutex`](api-thread.md#classscy_1_1Mutex)` _mutex` | 
`protected TaskList _tasks` | 
`protected async::Runner::Ptr _runner` | 
`protected virtual void run()` | Called by the async context to run the next task.
`protected virtual bool add(`[`Task`](#classscy_1_1Task)` * task)` | Adds a task to the runner.
`protected virtual bool remove(`[`Task`](#classscy_1_1Task)` * task)` | Removes a task from the runner.
`protected virtual `[`Task`](#classscy_1_1Task)` * next() const` | Returns the next task to be run.
`protected virtual void clear()` | Destroys and clears all manages tasks.
`protected virtual void onAdd(`[`Task`](#classscy_1_1Task)` * task)` | Called after a task is added.
`protected virtual void onStart(`[`Task`](#classscy_1_1Task)` * task)` | Called after a task is started.
`protected virtual void onCancel(`[`Task`](#classscy_1_1Task)` * task)` | Called after a task is cancelled.
`protected virtual void onRemove(`[`Task`](#classscy_1_1Task)` * task)` | Called after a task is removed.
`protected virtual void onRun(`[`Task`](#classscy_1_1Task)` * task)` | Called after a task has run.

## Members

#### `public `[`NullSignal`](api-base.md#classscy_1_1NullSignal)` Idle` {#group__base_1gabf058e5fccd90bd4c65e22a5bc542ddf}

Fires after completing an iteration of all tasks.



#### `public `[`NullSignal`](api-base.md#classscy_1_1NullSignal)` Shutdown` {#group__base_1gaf3afae0333fb5b248c320f07a876c682}

Signals when the [TaskRunner](#classscy_1_1TaskRunner) is shutting down.



#### `public  TaskRunner(async::Runner::Ptr runner)` {#group__base_1ga4d3db14bcb1309bee6156ff635526285}





#### `public virtual  ~TaskRunner()` {#group__base_1ga7f694f4681bee437fffd58ad6e21f09e}





#### `public virtual bool start(`[`Task`](#classscy_1_1Task)` * task)` {#group__base_1ga3f30dc3d3e1f68ada87091a2f1ca73d1}

Starts a task, adding it if it doesn't exist.



#### `public virtual bool cancel(`[`Task`](#classscy_1_1Task)` * task)` {#group__base_1ga429b24f441d2f86b7dcddc6764359d49}



Cancels a task. The task reference will be managed the [TaskRunner](#classscy_1_1TaskRunner) until the task is destroyed.

#### `public virtual bool destroy(`[`Task`](#classscy_1_1Task)` * task)` {#group__base_1ga44547fd3858f512ea07c71676059ca7e}

Queues a task for destruction.



#### `public virtual bool exists(`[`Task`](#classscy_1_1Task)` * task) const` {#group__base_1ga6f2cdcb41929fb914cdf288e1a00358e}

Returns weather or not a task exists.



#### `public virtual `[`Task`](#classscy_1_1Task)` * get(std::uint32_t id) const` {#group__base_1ga765c6cdfb0a656a2a22c99fa5218d0b2}



Returns the task pointer matching the given ID, or nullptr if no task exists.

#### `public virtual void setRunner(async::Runner::Ptr runner)` {#group__base_1ga0b9fa91d1edef4ff2237d8a552deb674}



Set the asynchronous context for packet processing. This may be a [Thread](#classscy_1_1Thread) or another derivative of Async. Must be set before the stream is activated.

#### `public inline virtual const char * className() const` {#group__base_1gae39b99240b62df5815fd611d56cdb7c1}





#### `protected mutable `[`Mutex`](api-thread.md#classscy_1_1Mutex)` _mutex` {#group__base_1gaad6506a080eff59878e0bfe7aa1f3264}





#### `protected TaskList _tasks` {#group__base_1ga0d19424a7b9506240e72da3a45a4b08c}





#### `protected async::Runner::Ptr _runner` {#group__base_1gaf3a70151c37d321bd1b155d7461d0ff2}





#### `protected virtual void run()` {#group__base_1ga07a481d667f1a9c67e881eedcffc10a5}

Called by the async context to run the next task.



#### `protected virtual bool add(`[`Task`](#classscy_1_1Task)` * task)` {#group__base_1ga3cf9ff9159b170aa7e413bf0b0d0703e}

Adds a task to the runner.



#### `protected virtual bool remove(`[`Task`](#classscy_1_1Task)` * task)` {#group__base_1gaca2341833adec925d50db8a5e128f679}

Removes a task from the runner.



#### `protected virtual `[`Task`](#classscy_1_1Task)` * next() const` {#group__base_1ga5c318e730939bf4b63072ebcdac5e9ee}

Returns the next task to be run.



#### `protected virtual void clear()` {#group__base_1gaa92a25f1d62e16bc40642688f569d5bd}

Destroys and clears all manages tasks.



#### `protected virtual void onAdd(`[`Task`](#classscy_1_1Task)` * task)` {#group__base_1gad6fc274ed81e757dba96db8f24fb2420}

Called after a task is added.



#### `protected virtual void onStart(`[`Task`](#classscy_1_1Task)` * task)` {#group__base_1ga2178d212993fd101fbe839afaaa0f79f}

Called after a task is started.



#### `protected virtual void onCancel(`[`Task`](#classscy_1_1Task)` * task)` {#group__base_1gab28a8ded043ae47ecace8c79512ec31a}

Called after a task is cancelled.



#### `protected virtual void onRemove(`[`Task`](#classscy_1_1Task)` * task)` {#group__base_1ga3c19020b93c4abaf5006c67a8b1ae474}

Called after a task is removed.



#### `protected virtual void onRun(`[`Task`](#classscy_1_1Task)` * task)` {#group__base_1ga0851ff27ae76aeb33d8d4f289844aa61}

Called after a task has run.



# class `scy::Thread` {#classscy_1_1Thread}

```
class scy::Thread
  : public scy::async::Runner
```  



This class implements a platform-independent wrapper around an operating system thread.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Thread()` | 
`public  Thread(`[`async::Runnable`](#classscy_1_1async_1_1Runnable)` & target)` | 
`public  Thread(std::function< void()> target)` | 
`public  Thread(std::function< void(void *)> target,void * arg)` | 
`public virtual  ~Thread()` | 
`public void join()` | Waits until the thread exits.
`public bool waitForExit(int timeout)` | 
`public uv_thread_t id() const` | Returns the native thread handle.
`protected uv_thread_t _handle` | 
`protected  Thread(const `[`Thread`](#classscy_1_1Thread)` &)` | 
`protected `[`Thread`](#classscy_1_1Thread)` & operator=(const `[`Thread`](#classscy_1_1Thread)` &)` | 
`protected virtual bool async() const` | 
`protected virtual void startAsync()` | Start the context from the control thread.

## Members

#### `public  Thread()` {#group__base_1ga54fd807dcf46e13b7740713f24487816}





#### `public  Thread(`[`async::Runnable`](#classscy_1_1async_1_1Runnable)` & target)` {#group__base_1gab70d8a5d65f0a2f0c077123041e02d8c}





#### `public  Thread(std::function< void()> target)` {#group__base_1gab5b31ddaa4aac8464c14ff69a30ff8e0}





#### `public  Thread(std::function< void(void *)> target,void * arg)` {#group__base_1gae77a1fdf6ad50532ade119cfad1fff28}





#### `public virtual  ~Thread()` {#group__base_1gaf8d1a393d2f0130b0e8d3e634f50c125}





#### `public void join()` {#group__base_1gad5b13af2dd59884809566c8f95a99f16}

Waits until the thread exits.



#### `public bool waitForExit(int timeout)` {#group__base_1ga60b2ef4860d9cb68ec601d95a35b38b5}



Waits until the thread exits. The thread should be cancelled beore calling this method. This method must be called from outside the current thread context or deadlock will ensue.

#### `public uv_thread_t id() const` {#group__base_1gad3ac929d47e0dcadc5f106473d11a148}

Returns the native thread handle.



#### `protected uv_thread_t _handle` {#group__base_1ga4f306e5416331b9b4131adfc0f45c908}





#### `protected  Thread(const `[`Thread`](#classscy_1_1Thread)` &)` {#group__base_1gab16fbe0e53ba88c12055f346b13aa83d}





#### `protected `[`Thread`](#classscy_1_1Thread)` & operator=(const `[`Thread`](#classscy_1_1Thread)` &)` {#group__base_1gab22acc3ad4419d060be247ac23548bab}





#### `protected virtual bool async() const` {#group__base_1gac9ec059f9ffbd29d9790a7f95c5c5d21}



Returns true if the implementation is thread-based, or false if it belongs to an event loop.

#### `protected virtual void startAsync()` {#group__base_1gaf0bc2b6aec6123dc914966507326c61d}

Start the context from the control thread.



# class `scy::ThreadedStreamReader` {#classscy_1_1ThreadedStreamReader}

```
class scy::ThreadedStreamReader
  : public scy::PacketStreamAdapter
  : public scy::async::Startable
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`PacketSignal`](api-base.md#classscy_1_1SignalBase)` emitter` | 
`public inline  ThreadedStreamReader(std::istream * is)` | 
`public inline  ~ThreadedStreamReader()` | 
`public inline virtual void start()` | 
`public inline virtual void stop()` | 
`public template<class StreamT>`  <br/>`inline StreamT & stream()` | 
`public inline std::istream & stream()` | 
`protected `[`Thread`](api-base.md#classscy_1_1Thread)` _runner` | 
`protected std::istream * _istream` | 

## Members

#### `public `[`PacketSignal`](api-base.md#classscy_1_1SignalBase)` emitter` {#group__base_1ga567cb288d73e8913f61ba6d76008b22f}





#### `public inline  ThreadedStreamReader(std::istream * is)` {#group__base_1ga145262d7cc0e8082313771c78d719a93}





#### `public inline  ~ThreadedStreamReader()` {#group__base_1ga39fe2eb7ff89764e1bdea0dd6c391b09}





#### `public inline virtual void start()` {#group__base_1gaac4e2b03913105c0365cf653243737ae}





#### `public inline virtual void stop()` {#group__base_1ga42178b04aeaa27a342cb45850d66f548}





#### `public template<class StreamT>`  <br/>`inline StreamT & stream()` {#group__base_1gafa9dc55fffa942040a7c784933056dd0}





#### `public inline std::istream & stream()` {#group__base_1ga429cb36fa436bf79457789adca51c964}





#### `protected `[`Thread`](api-base.md#classscy_1_1Thread)` _runner` {#group__base_1ga209b1d5e46545ea7ed88bc0f0492f22d}





#### `protected std::istream * _istream` {#group__base_1gaf859c1452a438a01f7faf1771f4f8463}





# class `scy::TimedToken` {#classscy_1_1TimedToken}

```
class scy::TimedToken
  : public scy::Timeout
```  

A token that expires after the specified duration.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  TimedToken(long duration)` | 
`public  TimedToken(const std::string & id,long duration)` | 
`public inline std::string id() const` | 
`public inline bool operator==(const `[`TimedToken`](#classscy_1_1TimedToken)` & r) const` | 
`public inline bool operator==(const std::string & r) const` | 
`protected std::string _id` | 

## Members

#### `public  TimedToken(long duration)` {#group__base_1ga3172961c6e661451b64bc60c935c2e30}





#### `public  TimedToken(const std::string & id,long duration)` {#group__base_1ga00d0a09459c0d5b2de18de73ccf1eda7}





#### `public inline std::string id() const` {#group__base_1ga468f28c2d15d97b85ed6687bec10ce37}





#### `public inline bool operator==(const `[`TimedToken`](#classscy_1_1TimedToken)` & r) const` {#group__base_1ga36b8cadb6d6ccc57b725b530d8f92f47}





#### `public inline bool operator==(const std::string & r) const` {#group__base_1ga44476c978974979e4cb079b8562ce724}





#### `protected std::string _id` {#group__base_1ga7d90971c34ef4d63f00a5153c74cc1c3}





# class `scy::Timeout` {#classscy_1_1Timeout}


Simple millisecond timeout counter which expires after a given delay.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Timeout(long delay,bool autoStart)` | 
`public  Timeout(const `[`Timeout`](#classscy_1_1Timeout)` & src)` | 
`public  ~Timeout()` | 
`public bool running() const` | 
`public void start()` | 
`public void stop()` | 
`public void reset()` | 
`public long remaining() const` | 
`public bool expired() const` | 
`public inline void setDelay(long delay)` | 
`public inline clock_t startAt() const` | 
`public inline long delay() const` | 
`public `[`Timeout`](#classscy_1_1Timeout)` & operator=(const `[`Timeout`](#classscy_1_1Timeout)` & src)` | 
`protected clock_t _startAt` | 
`protected long _delay` | 

## Members

#### `public  Timeout(long delay,bool autoStart)` {#group__base_1ga7a0c9d99bcece690d28b3568a532c8bb}





#### `public  Timeout(const `[`Timeout`](#classscy_1_1Timeout)` & src)` {#group__base_1gafd9ba1a97e117625f6c87121c5b1bb53}





#### `public  ~Timeout()` {#group__base_1ga96b82e3575fe7a5d18d46f9e409e3883}





#### `public bool running() const` {#group__base_1ga451634c2144d1adda5a9301dcef6c118}





#### `public void start()` {#group__base_1ga4de922b575fa908ce89b978f26ef4059}





#### `public void stop()` {#group__base_1gafb82ee057268b469694dc2aa8c72bc36}





#### `public void reset()` {#group__base_1gaddc18501ce642ff8724d15fc41939ac8}





#### `public long remaining() const` {#group__base_1gae7eafa18334cdd1def62b1cd9504a5ed}





#### `public bool expired() const` {#group__base_1ga434bc816acdc562bd4434b853d2252f9}





#### `public inline void setDelay(long delay)` {#group__base_1ga29f93b01c146d755a6f46bb1f6ec8cee}





#### `public inline clock_t startAt() const` {#group__base_1ga82eac31b0527400086bbe0c2414ad9b5}





#### `public inline long delay() const` {#group__base_1ga5d32e6ba4780efe685496b6d6ef95c62}





#### `public `[`Timeout`](#classscy_1_1Timeout)` & operator=(const `[`Timeout`](#classscy_1_1Timeout)` & src)` {#group__base_1ga3f4396dde53c6f38869c40b1a1ee1074}





#### `protected clock_t _startAt` {#group__base_1gac8610fab1717accf992e597d12a904c1}





#### `protected long _delay` {#group__base_1ga94fe2854e36942ecf3abff5a1f19769e}





# class `scy::Timer` {#classscy_1_1Timer}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`NullSignal`](api-base.md#classscy_1_1NullSignal)` Timeout` | 
`public  Timer(uv::Loop * loop)` | 
`public virtual  ~Timer()` | 
`public virtual void start(std::int64_t interval)` | 
`public virtual void start(std::int64_t timeout,std::int64_t interval)` | 
`public virtual void stop()` | Stops the timer.
`public virtual void restart()` | 
`public virtual void again()` | 
`public virtual void setInterval(std::int64_t interval)` | 
`public bool active() const` | 
`public std::int64_t timeout() const` | 
`public std::int64_t interval() const` | 
`public std::int64_t count()` | 
`public `[`uv::Handle`](#classscy_1_1uv_1_1Handle)` & handle()` | 
`protected `[`uv::Handle`](api-uv.md#classscy_1_1uv_1_1Handle)` _handle` | 
`protected std::int64_t _timeout` | 
`protected std::int64_t _interval` | 
`protected std::int64_t _count` | 
`protected  Timer(const `[`Timer`](#classscy_1_1Timer)` &)` | 
`protected `[`Timer`](#classscy_1_1Timer)` & operator=(const `[`Timer`](#classscy_1_1Timer)` &)` | 
`protected virtual void init()` | 

## Members

#### `public `[`NullSignal`](api-base.md#classscy_1_1NullSignal)` Timeout` {#group__base_1ga70ab283148e5fb200fe84f75ea9ea138}





#### `public  Timer(uv::Loop * loop)` {#group__base_1ga1fae10ff8c5f7967f91cde97229af7f9}





#### `public virtual  ~Timer()` {#group__base_1ga64a91b7e8c9c4145058cf2088b3221a4}





#### `public virtual void start(std::int64_t interval)` {#group__base_1gaabdada653a2a7661e97ddc1e0be3cd7d}





#### `public virtual void start(std::int64_t timeout,std::int64_t interval)` {#group__base_1gae2db39250cb09183994756c0474b5e2b}



Starts the timer, an interval value of zero will only trigger once after timeout.

#### `public virtual void stop()` {#group__base_1ga13edfe3f0b4efd439cf640377624b5be}

Stops the timer.



#### `public virtual void restart()` {#group__base_1ga0e9afbcacb03261ee441b0631e2dccf1}



Restarts the timer, even if it hasn't been started yet. An interval or interval must be set or an exception will be thrown.

#### `public virtual void again()` {#group__base_1gab91426f1cb86e09423775e6ee59a2b8e}



Stop the timer, and if it is repeating restart it using the repeat value as the timeout. If the timer has never been started before it returns -1 and sets the error to UV_EINVAL.

#### `public virtual void setInterval(std::int64_t interval)` {#group__base_1gab2ba3bfa336276158867c017b39f24e4}



Set the repeat value. Note that if the repeat value is set from a timer callback it does not immediately take effect. If the timer was non-repeating before, it will have been stopped. If it was repeating, then the old repeat value will have been used to schedule the next timeout.

#### `public bool active() const` {#group__base_1ga3bc8f6a117d6982ea125f7d1f26ad67f}





#### `public std::int64_t timeout() const` {#group__base_1ga731fc5f29c10ef7237494b5fef7063d5}





#### `public std::int64_t interval() const` {#group__base_1gae80ec21a3b77a186e8781179e478a623}





#### `public std::int64_t count()` {#group__base_1ga3e4978ac6eb64b151c670a68980ff11d}





#### `public `[`uv::Handle`](#classscy_1_1uv_1_1Handle)` & handle()` {#group__base_1gaa1283adfa01c58a35b0622062e0567bd}





#### `protected `[`uv::Handle`](api-uv.md#classscy_1_1uv_1_1Handle)` _handle` {#group__base_1ga28d2ed75952cde3696854ce23f11fcc1}





#### `protected std::int64_t _timeout` {#group__base_1ga146dcd71f59664feead6e84c971c6b9a}





#### `protected std::int64_t _interval` {#group__base_1ga0a84a02412fc3e3fe9fed3ce4ff8b7fa}





#### `protected std::int64_t _count` {#group__base_1gad7d0505734bb3683560456c3a5fea546}





#### `protected  Timer(const `[`Timer`](#classscy_1_1Timer)` &)` {#group__base_1ga1be94900103cc25cffe8732c32e37ae9}





#### `protected `[`Timer`](#classscy_1_1Timer)` & operator=(const `[`Timer`](#classscy_1_1Timer)` &)` {#group__base_1gadc0cd9fc2bc156413a4c2adcba15c7e1}





#### `protected virtual void init()` {#group__base_1ga4d7d19830efea92f0944f972caba2fdb}





# class `scy::Timespan` {#classscy_1_1Timespan}


A class that represents time spans up to microsecond resolution.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Timespan()` | Creates a zero [Timespan](api-base.md#classscy_1_1Timespan).
`public  Timespan(TimeDiff microseconds)` | Creates a [Timespan](api-base.md#classscy_1_1Timespan).
`public  Timespan(long seconds,long microseconds)` | 
`public  Timespan(int days,int hours,int minutes,int seconds,int microseconds)` | Creates a [Timespan](api-base.md#classscy_1_1Timespan).
`public  Timespan(const `[`Timespan`](#classscy_1_1Timespan)` & timespan)` | Creates a [Timespan](api-base.md#classscy_1_1Timespan) from another one.
`public  ~Timespan()` | Destroys the [Timespan](api-base.md#classscy_1_1Timespan).
`public `[`Timespan`](#classscy_1_1Timespan)` & operator=(const `[`Timespan`](#classscy_1_1Timespan)` & timespan)` | Assignment operator.
`public `[`Timespan`](#classscy_1_1Timespan)` & operator=(TimeDiff microseconds)` | Assignment operator.
`public `[`Timespan`](#classscy_1_1Timespan)` & assign(int days,int hours,int minutes,int seconds,int microseconds)` | Assigns a new span.
`public `[`Timespan`](#classscy_1_1Timespan)` & assign(long seconds,long microseconds)` | 
`public void swap(`[`Timespan`](#classscy_1_1Timespan)` & timespan)` | Swaps the [Timespan](api-base.md#classscy_1_1Timespan) with another one.
`public inline bool operator==(const `[`Timespan`](#classscy_1_1Timespan)` & ts) const` | 
`public inline bool operator!=(const `[`Timespan`](#classscy_1_1Timespan)` & ts) const` | 
`public inline bool operator>(const `[`Timespan`](#classscy_1_1Timespan)` & ts) const` | 
`public inline bool operator>=(const `[`Timespan`](#classscy_1_1Timespan)` & ts) const` | 
`public inline bool operator<(const `[`Timespan`](#classscy_1_1Timespan)` & ts) const` | 
`public inline bool operator<=(const `[`Timespan`](#classscy_1_1Timespan)` & ts) const` | 
`public inline bool operator==(TimeDiff microseconds) const` | 
`public inline bool operator!=(TimeDiff microseconds) const` | 
`public inline bool operator>(TimeDiff microseconds) const` | 
`public inline bool operator>=(TimeDiff microseconds) const` | 
`public inline bool operator<(TimeDiff microseconds) const` | 
`public inline bool operator<=(TimeDiff microseconds) const` | 
`public `[`Timespan`](#classscy_1_1Timespan)` operator+(const `[`Timespan`](#classscy_1_1Timespan)` & d) const` | 
`public `[`Timespan`](#classscy_1_1Timespan)` operator-(const `[`Timespan`](#classscy_1_1Timespan)` & d) const` | 
`public `[`Timespan`](#classscy_1_1Timespan)` & operator+=(const `[`Timespan`](#classscy_1_1Timespan)` & d)` | 
`public `[`Timespan`](#classscy_1_1Timespan)` & operator-=(const `[`Timespan`](#classscy_1_1Timespan)` & d)` | 
`public `[`Timespan`](#classscy_1_1Timespan)` operator+(TimeDiff microseconds) const` | 
`public `[`Timespan`](#classscy_1_1Timespan)` operator-(TimeDiff microseconds) const` | 
`public `[`Timespan`](#classscy_1_1Timespan)` & operator+=(TimeDiff microseconds)` | 
`public `[`Timespan`](#classscy_1_1Timespan)` & operator-=(TimeDiff microseconds)` | 
`public inline int days() const` | Returns the number of days.
`public inline int hours() const` | Returns the number of hours (0 to 23).
`public inline int totalHours() const` | Returns the total number of hours.
`public inline int minutes() const` | Returns the number of minutes (0 to 59).
`public inline int totalMinutes() const` | Returns the total number of minutes.
`public inline int seconds() const` | Returns the number of seconds (0 to 59).
`public inline int totalSeconds() const` | Returns the total number of seconds.
`public inline int milliseconds() const` | Returns the number of milliseconds (0 to 999).
`public inline TimeDiff totalMilliseconds() const` | Returns the total number of milliseconds.
`public inline int microseconds() const` | 
`public inline int useconds() const` | 
`public inline TimeDiff totalMicroseconds() const` | Returns the total number of microseconds.

## Members

#### `public  Timespan()` {#group__base_1ga183ab023a03182a6b62287f31ba4667c}

Creates a zero [Timespan](#classscy_1_1Timespan).



#### `public  Timespan(TimeDiff microseconds)` {#group__base_1ga36bd62906099ad73b6411bbee9df4687}

Creates a [Timespan](#classscy_1_1Timespan).



#### `public  Timespan(long seconds,long microseconds)` {#group__base_1ga9281640ae0cfdf1af90bf219e3ab2502}



Creates a [Timespan](#classscy_1_1Timespan). Useful for creating a [Timespan](#classscy_1_1Timespan) from a struct timeval.

#### `public  Timespan(int days,int hours,int minutes,int seconds,int microseconds)` {#group__base_1gad7c52f4c3f97829b1a2184819c6fdb78}

Creates a [Timespan](#classscy_1_1Timespan).



#### `public  Timespan(const `[`Timespan`](#classscy_1_1Timespan)` & timespan)` {#group__base_1gad644dad5d9fcc72c753fb207aa0f7799}

Creates a [Timespan](#classscy_1_1Timespan) from another one.



#### `public  ~Timespan()` {#group__base_1ga58f2185a33f9adf2043381a216e7e3d0}

Destroys the [Timespan](#classscy_1_1Timespan).



#### `public `[`Timespan`](#classscy_1_1Timespan)` & operator=(const `[`Timespan`](#classscy_1_1Timespan)` & timespan)` {#group__base_1gacb09eb0c421d3ee1567221bbcbfa1686}

Assignment operator.



#### `public `[`Timespan`](#classscy_1_1Timespan)` & operator=(TimeDiff microseconds)` {#group__base_1ga53a89a4c0d074d07a69651f9b1390ca2}

Assignment operator.



#### `public `[`Timespan`](#classscy_1_1Timespan)` & assign(int days,int hours,int minutes,int seconds,int microseconds)` {#group__base_1ga8aeb84fe300f38a39ec0ee7cd4a2c463}

Assigns a new span.



#### `public `[`Timespan`](#classscy_1_1Timespan)` & assign(long seconds,long microseconds)` {#group__base_1ga326e06380b6c271143dd53261061e0a7}



Assigns a new span. Useful for assigning from a struct timeval.

#### `public void swap(`[`Timespan`](#classscy_1_1Timespan)` & timespan)` {#group__base_1ga3289402362d3f09413c7e27c8a5f84a7}

Swaps the [Timespan](#classscy_1_1Timespan) with another one.



#### `public inline bool operator==(const `[`Timespan`](#classscy_1_1Timespan)` & ts) const` {#group__base_1ga6901eaae07320f3bbf4cbcbdcba58788}





#### `public inline bool operator!=(const `[`Timespan`](#classscy_1_1Timespan)` & ts) const` {#group__base_1gac44da14b266ce4dbf80779330cb7090d}





#### `public inline bool operator>(const `[`Timespan`](#classscy_1_1Timespan)` & ts) const` {#group__base_1ga7add4d0610dc0b197739f9b3d68dc39f}





#### `public inline bool operator>=(const `[`Timespan`](#classscy_1_1Timespan)` & ts) const` {#group__base_1gad19debbff7b0f7cad21fab77e589e982}





#### `public inline bool operator<(const `[`Timespan`](#classscy_1_1Timespan)` & ts) const` {#group__base_1ga11f56482aeb54a4a1e787750d8e1bd5d}





#### `public inline bool operator<=(const `[`Timespan`](#classscy_1_1Timespan)` & ts) const` {#group__base_1gaf862e58fa751b510a38233b11aa2c658}





#### `public inline bool operator==(TimeDiff microseconds) const` {#group__base_1ga337882f65d1c794d461573d928877ad9}





#### `public inline bool operator!=(TimeDiff microseconds) const` {#group__base_1gab46f76d0e8a8ed7a150a20ffeb02d337}





#### `public inline bool operator>(TimeDiff microseconds) const` {#group__base_1ga3c356ed1db34e4a04abfdf8dae5d0a7f}





#### `public inline bool operator>=(TimeDiff microseconds) const` {#group__base_1ga6a10617e981175a0672fc9532edf5fd6}





#### `public inline bool operator<(TimeDiff microseconds) const` {#group__base_1ga57fa8c22b3b06fa8884b1dae53b2974d}





#### `public inline bool operator<=(TimeDiff microseconds) const` {#group__base_1gaa9c1dfaca2cac4fe2da24ca6e835e808}





#### `public `[`Timespan`](#classscy_1_1Timespan)` operator+(const `[`Timespan`](#classscy_1_1Timespan)` & d) const` {#group__base_1gacdafb69117cce505b5c3290eff82e231}





#### `public `[`Timespan`](#classscy_1_1Timespan)` operator-(const `[`Timespan`](#classscy_1_1Timespan)` & d) const` {#group__base_1ga036be956f49e4ee5b3c7974c49c84689}





#### `public `[`Timespan`](#classscy_1_1Timespan)` & operator+=(const `[`Timespan`](#classscy_1_1Timespan)` & d)` {#group__base_1gac6abe30a87d689470bce966fe2bca4f1}





#### `public `[`Timespan`](#classscy_1_1Timespan)` & operator-=(const `[`Timespan`](#classscy_1_1Timespan)` & d)` {#group__base_1gab466c189ee0189435fda807fce532333}





#### `public `[`Timespan`](#classscy_1_1Timespan)` operator+(TimeDiff microseconds) const` {#group__base_1gae336ef6426d9886f13f62e3e0326a92b}





#### `public `[`Timespan`](#classscy_1_1Timespan)` operator-(TimeDiff microseconds) const` {#group__base_1ga96a28c1241b0c065e3c213ab4cc44cf4}





#### `public `[`Timespan`](#classscy_1_1Timespan)` & operator+=(TimeDiff microseconds)` {#group__base_1gadfd1a60c223de14e723fbb03e15f157f}





#### `public `[`Timespan`](#classscy_1_1Timespan)` & operator-=(TimeDiff microseconds)` {#group__base_1ga476bf78a0dea511c9dbe6c99ec86ca6c}





#### `public inline int days() const` {#group__base_1gab39bf74d96f285b9502b0411321c2c40}

Returns the number of days.



#### `public inline int hours() const` {#group__base_1ga6887a573816f2fdc9019e1e42b15a1fa}

Returns the number of hours (0 to 23).



#### `public inline int totalHours() const` {#group__base_1ga4c78c96a19b68218bc1ec9056870dd48}

Returns the total number of hours.



#### `public inline int minutes() const` {#group__base_1ga9b4fa5de6b469773c139d3992c368580}

Returns the number of minutes (0 to 59).



#### `public inline int totalMinutes() const` {#group__base_1ga8dc72898fece5e76f4e89de4a60483ca}

Returns the total number of minutes.



#### `public inline int seconds() const` {#group__base_1ga6a1f8b3aaadca7e4092b76e860e11473}

Returns the number of seconds (0 to 59).



#### `public inline int totalSeconds() const` {#group__base_1ga2d936ac7dd1d8c6e3b445992c08fb0da}

Returns the total number of seconds.



#### `public inline int milliseconds() const` {#group__base_1ga2d17d492ef454811b4487c210f6f4fa4}

Returns the number of milliseconds (0 to 999).



#### `public inline TimeDiff totalMilliseconds() const` {#group__base_1ga5301028400cc25b3ff5f1f34e85e3612}

Returns the total number of milliseconds.



#### `public inline int microseconds() const` {#group__base_1ga038a3be32de4ebe87b3fe3676a327a04}



Returns the fractions of a millisecond in microseconds (0 to 999).

#### `public inline int useconds() const` {#group__base_1ga4dbdae0aa5912a70c8f3497560fce318}



Returns the fractions of a second in microseconds (0 to 999999).

#### `public inline TimeDiff totalMicroseconds() const` {#group__base_1ga9fe3ab13665735f41ab6fd8ed8ae477b}

Returns the total number of microseconds.



# class `scy::Timestamp` {#classscy_1_1Timestamp}




A [Timestamp](#classscy_1_1Timestamp) stores a monotonic* time value with (theoretical) microseconds resolution. Timestamps can be compared with each other and simple arithmetics are supported.

[*] Note that [Timestamp](#classscy_1_1Timestamp) values are only monotonic as long as the systems's clock is monotonic as well (and not, e.g. set back).

Timestamps are UTC (Coordinated Universal Time) based and thus independent of the timezone in effect on the system.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Timestamp()` | Creates a timestamp with the current time.
`public  Timestamp(`[`TimeVal`](#group__base_1ga7da2b2da9cd1e096cbb146da027f17dc)` tv)` | Creates a timestamp from the given time value.
`public  Timestamp(const `[`Timestamp`](#classscy_1_1Timestamp)` & other)` | Copy constructor.
`public  ~Timestamp()` | Destroys the timestamp.
`public `[`Timestamp`](#classscy_1_1Timestamp)` & operator=(const `[`Timestamp`](#classscy_1_1Timestamp)` & other)` | 
`public `[`Timestamp`](#classscy_1_1Timestamp)` & operator=(`[`TimeVal`](#group__base_1ga7da2b2da9cd1e096cbb146da027f17dc)` tv)` | 
`public void swap(`[`Timestamp`](#classscy_1_1Timestamp)` & timestamp)` | Swaps the [Timestamp](api-base.md#classscy_1_1Timestamp) with another one.
`public void update()` | Updates the [Timestamp](api-base.md#classscy_1_1Timestamp) with the current time.
`public inline bool operator==(const `[`Timestamp`](#classscy_1_1Timestamp)` & ts) const` | 
`public inline bool operator!=(const `[`Timestamp`](#classscy_1_1Timestamp)` & ts) const` | 
`public inline bool operator>(const `[`Timestamp`](#classscy_1_1Timestamp)` & ts) const` | 
`public inline bool operator>=(const `[`Timestamp`](#classscy_1_1Timestamp)` & ts) const` | 
`public inline bool operator<(const `[`Timestamp`](#classscy_1_1Timestamp)` & ts) const` | 
`public inline bool operator<=(const `[`Timestamp`](#classscy_1_1Timestamp)` & ts) const` | 
`public inline `[`Timestamp`](#classscy_1_1Timestamp)` operator+(`[`TimeDiff`](#group__base_1ga9f06831e4e9b9cbc6f061cc61dc91fd0)` d) const` | 
`public inline `[`Timestamp`](#classscy_1_1Timestamp)` operator-(`[`TimeDiff`](#group__base_1ga9f06831e4e9b9cbc6f061cc61dc91fd0)` d) const` | 
`public inline `[`TimeDiff`](#group__base_1ga9f06831e4e9b9cbc6f061cc61dc91fd0)` operator-(const `[`Timestamp`](#classscy_1_1Timestamp)` & ts) const` | 
`public inline `[`Timestamp`](#classscy_1_1Timestamp)` & operator+=(`[`TimeDiff`](#group__base_1ga9f06831e4e9b9cbc6f061cc61dc91fd0)` d)` | 
`public inline `[`Timestamp`](#classscy_1_1Timestamp)` & operator-=(`[`TimeDiff`](#group__base_1ga9f06831e4e9b9cbc6f061cc61dc91fd0)` d)` | 
`public inline std::time_t epochTime() const` | 
`public inline `[`UtcTimeVal`](#group__base_1gaec8ace0f25db0ce91c10b30270c3dc24)` utcTime() const` | 
`public inline `[`TimeVal`](#group__base_1ga7da2b2da9cd1e096cbb146da027f17dc)` epochMicroseconds() const` | 
`public inline `[`TimeDiff`](#group__base_1ga9f06831e4e9b9cbc6f061cc61dc91fd0)` elapsed() const` | 
`public inline bool isElapsed(`[`TimeDiff`](#group__base_1ga9f06831e4e9b9cbc6f061cc61dc91fd0)` interval) const` | 

## Members

#### `public  Timestamp()` {#group__base_1ga59eb0caf11a4b42805be30b575c64685}

Creates a timestamp with the current time.



#### `public  Timestamp(`[`TimeVal`](#group__base_1ga7da2b2da9cd1e096cbb146da027f17dc)` tv)` {#group__base_1ga634322242b4d434f27d0b549e6a2d02a}

Creates a timestamp from the given time value.



#### `public  Timestamp(const `[`Timestamp`](#classscy_1_1Timestamp)` & other)` {#group__base_1gaee8a093ba4e7d09969d6e9d66d39c98c}

Copy constructor.



#### `public  ~Timestamp()` {#group__base_1ga9454ae9b51eef4ddfd6a5fc517d5fb89}

Destroys the timestamp.



#### `public `[`Timestamp`](#classscy_1_1Timestamp)` & operator=(const `[`Timestamp`](#classscy_1_1Timestamp)` & other)` {#group__base_1gaa6f065d2bafe45160d5830ec7f3fea03}





#### `public `[`Timestamp`](#classscy_1_1Timestamp)` & operator=(`[`TimeVal`](#group__base_1ga7da2b2da9cd1e096cbb146da027f17dc)` tv)` {#group__base_1gad270942977ff386b8295e92d32a4247a}





#### `public void swap(`[`Timestamp`](#classscy_1_1Timestamp)` & timestamp)` {#group__base_1ga051b185aa50b85119422c8069b31f9b1}

Swaps the [Timestamp](#classscy_1_1Timestamp) with another one.



#### `public void update()` {#group__base_1gae851ba7fcabd33ddf4b69029b93e7da1}

Updates the [Timestamp](#classscy_1_1Timestamp) with the current time.



#### `public inline bool operator==(const `[`Timestamp`](#classscy_1_1Timestamp)` & ts) const` {#group__base_1gaaff90e64401ed08703d4bab0b09bc5bd}





#### `public inline bool operator!=(const `[`Timestamp`](#classscy_1_1Timestamp)` & ts) const` {#group__base_1ga70b120698ef0ca3429d5e6035e03ab51}





#### `public inline bool operator>(const `[`Timestamp`](#classscy_1_1Timestamp)` & ts) const` {#group__base_1gacdc27b968a6f92a38dfa29f4ce0639ca}





#### `public inline bool operator>=(const `[`Timestamp`](#classscy_1_1Timestamp)` & ts) const` {#group__base_1gae4ddc243de02e4d233f681d9df760111}





#### `public inline bool operator<(const `[`Timestamp`](#classscy_1_1Timestamp)` & ts) const` {#group__base_1gae1ae154f8dc65bb9966411801d514f88}





#### `public inline bool operator<=(const `[`Timestamp`](#classscy_1_1Timestamp)` & ts) const` {#group__base_1ga18146b0b77bdc63ad3eaee69a460e38b}





#### `public inline `[`Timestamp`](#classscy_1_1Timestamp)` operator+(`[`TimeDiff`](#group__base_1ga9f06831e4e9b9cbc6f061cc61dc91fd0)` d) const` {#group__base_1gaae2bf2fef867c4856c03637c4eaa6916}





#### `public inline `[`Timestamp`](#classscy_1_1Timestamp)` operator-(`[`TimeDiff`](#group__base_1ga9f06831e4e9b9cbc6f061cc61dc91fd0)` d) const` {#group__base_1gafb9644c6db3560126efd053dfca80f7f}





#### `public inline `[`TimeDiff`](#group__base_1ga9f06831e4e9b9cbc6f061cc61dc91fd0)` operator-(const `[`Timestamp`](#classscy_1_1Timestamp)` & ts) const` {#group__base_1gaf904d00bddad7f6dd1ad05c7fd8592ee}





#### `public inline `[`Timestamp`](#classscy_1_1Timestamp)` & operator+=(`[`TimeDiff`](#group__base_1ga9f06831e4e9b9cbc6f061cc61dc91fd0)` d)` {#group__base_1gab575836d90b63b74515022efa909b261}





#### `public inline `[`Timestamp`](#classscy_1_1Timestamp)` & operator-=(`[`TimeDiff`](#group__base_1ga9f06831e4e9b9cbc6f061cc61dc91fd0)` d)` {#group__base_1ga4a845176e12d2116809c4c7b9b3fd940}





#### `public inline std::time_t epochTime() const` {#group__base_1ga018ebc0e980ee7e467da6238010a4313}



Returns the timestamp expressed in time_t. time_t base time is midnight, January 1, 1970. Resolution is one second.

#### `public inline `[`UtcTimeVal`](#group__base_1gaec8ace0f25db0ce91c10b30270c3dc24)` utcTime() const` {#group__base_1gac3ec372fc5bc417e7d38a7849d3bd173}



Returns the timestamp expressed in UTC-based time. UTC base time is midnight, October 15, 1582. Resolution is 100 nanoseconds.

#### `public inline `[`TimeVal`](#group__base_1ga7da2b2da9cd1e096cbb146da027f17dc)` epochMicroseconds() const` {#group__base_1ga9ed38faa45afc53468343bc1c89c6000}



Returns the timestamp expressed in microseconds since the Unix epoch, midnight, January 1, 1970.

#### `public inline `[`TimeDiff`](#group__base_1ga9f06831e4e9b9cbc6f061cc61dc91fd0)` elapsed() const` {#group__base_1gaee70f8a34b9712e25310f2bb642305b4}



Returns the time elapsed since the time denoted by the timestamp. Equivalent to [Timestamp()](#group__base_1ga59eb0caf11a4b42805be30b575c64685) - *this.

#### `public inline bool isElapsed(`[`TimeDiff`](#group__base_1ga9f06831e4e9b9cbc6f061cc61dc91fd0)` interval) const` {#group__base_1gadf63758d952b2ee7aa4e00238e89e950}



Returns true iff the given interval has passed since the time denoted by the timestamp.

# class `scy::Timezone` {#classscy_1_1Timezone}


This class provides information about the current timezone.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------

## Members

# class `scy::TZInfo` {#classscy_1_1TZInfo}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  TZInfo()` | 
`public inline int timeZone()` | 
`public inline const char * name(bool dst)` | 

## Members

#### `public inline  TZInfo()` {#group__base_1ga6491a626255da59420f4e79f135bec79}





#### `public inline int timeZone()` {#group__base_1ga2a6e305f50e4018b7a4f8cd97a4f579e}





#### `public inline const char * name(bool dst)` {#group__base_1ga8d7417517337cc59948fd5a9ccac25a7}





# struct `scy::Bitwise` {#structscy_1_1Bitwise}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public unsigned data` | 
`public inline  Bitwise(unsigned flags)` | 
`public inline virtual void reset()` | 
`public inline virtual void set(unsigned flag)` | 
`public inline virtual void add(unsigned flag)` | 
`public inline virtual void remove(unsigned flag)` | 
`public inline virtual void toggle(unsigned flag)` | 
`public inline virtual bool has(unsigned flag) const` | 

## Members

#### `public unsigned data` {#group__base_1ga561f756550e5240f615043453f77912b}





#### `public inline  Bitwise(unsigned flags)` {#group__base_1gab17fe0dc48af357d997ba8bcce089def}





#### `public inline virtual void reset()` {#group__base_1ga61f01af4d27436a26ef0c336f9d0dd23}





#### `public inline virtual void set(unsigned flag)` {#group__base_1ga73020121e07e2201909bf45030ef28c9}





#### `public inline virtual void add(unsigned flag)` {#group__base_1ga3ff03ce689fc75d83c0fd12e25f4e489}





#### `public inline virtual void remove(unsigned flag)` {#group__base_1ga8fbd08d9008b713faa32830f3f164075}





#### `public inline virtual void toggle(unsigned flag)` {#group__base_1ga34695eee0b63a6118ef4af65f9dbf081}





#### `public inline virtual bool has(unsigned flag) const` {#group__base_1ga84fb5bee9780f9ed4f6417adf378a039}





# struct `scy::DelegateBase` {#structscy_1_1DelegateBase}




The abstract base for all instantiations of the [Delegate](#classscy_1_1Delegate) template classes.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public void * data` | 
`public inline  DelegateBase(DataT data)` | 
`public inline  DelegateBase(const `[`DelegateBase`](#structscy_1_1DelegateBase)` & r)` | 
`public inline virtual  ~DelegateBase()` | 

## Members

#### `public void * data` {#group__base_1ga1b13b62fdacf39658c42fa1906e64cf6}





#### `public inline  DelegateBase(DataT data)` {#group__base_1gad572668d65141668a27d5e87a81bdb3c}





#### `public inline  DelegateBase(const `[`DelegateBase`](#structscy_1_1DelegateBase)` & r)` {#group__base_1ga228ce4c702fe131aa131bb2fb30e7d27}





#### `public inline virtual  ~DelegateBase()` {#group__base_1ga74b06c87e214b19d4c25a81a703fbff6}





# struct `scy::DelegateCallback` {#structscy_1_1DelegateCallback}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------

## Members

# struct `scy::DelegateCallback< C, 0, false >` {#structscy_1_1DelegateCallback_3_01C_00_010_00_01false_01_4}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual void emit(void * sender,void *,void *,void *,void *) const` | 

## Members

#### `public inline virtual void emit(void * sender,void *,void *,void *,void *) const` {#group__base_1ga43ffa9615285b7c85bbaeaa9b7fc875f}





# struct `scy::DelegateCallback< C, 0, true >` {#structscy_1_1DelegateCallback_3_01C_00_010_00_01true_01_4}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual void emit(void * sender,void *,void *,void *,void *) const` | 

## Members

#### `public inline virtual void emit(void * sender,void *,void *,void *,void *) const` {#group__base_1ga0e71e422423be34abcb368cab200bfdc}





# struct `scy::DelegateCallback< C, 1, false, P >` {#structscy_1_1DelegateCallback_3_01C_00_011_00_01false_00_01P_01_4}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual void emit(void *,P arg,void *,void *,void *) const` | 

## Members

#### `public inline virtual void emit(void *,P arg,void *,void *,void *) const` {#group__base_1gad6c32cc9256c77540a2bcdfb6f18bd8a}





# struct `scy::DelegateCallback< C, 1, true, P >` {#structscy_1_1DelegateCallback_3_01C_00_011_00_01true_00_01P_01_4}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual void emit(void * sender,P arg,void *,void *,void *) const` | 

## Members

#### `public inline virtual void emit(void * sender,P arg,void *,void *,void *) const` {#group__base_1ga8fa88106ac1a31826440f152570787f6}





# struct `scy::DelegateCallback< C, 2, false, P, P2 >` {#structscy_1_1DelegateCallback_3_01C_00_012_00_01false_00_01P_00_01P2_01_4}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual void emit(void *,P arg,P2 arg2,void *,void *) const` | 

## Members

#### `public inline virtual void emit(void *,P arg,P2 arg2,void *,void *) const` {#group__base_1gafd22870c7ad8f20f5c7c6d67341c15ae}





# struct `scy::DelegateCallback< C, 2, true, P, P2 >` {#structscy_1_1DelegateCallback_3_01C_00_012_00_01true_00_01P_00_01P2_01_4}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual void emit(void * sender,P arg,P2 arg2,void *,void *) const` | 

## Members

#### `public inline virtual void emit(void * sender,P arg,P2 arg2,void *,void *) const` {#group__base_1gac5ad828bfb0a76222d92b564cef42e06}





# struct `scy::DelegateCallback< C, 3, false, P, P2, P3 >` {#structscy_1_1DelegateCallback_3_01C_00_013_00_01false_00_01P_00_01P2_00_01P3_01_4}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual void emit(void *,P arg,P2 arg2,P3 arg3,void *) const` | 

## Members

#### `public inline virtual void emit(void *,P arg,P2 arg2,P3 arg3,void *) const` {#group__base_1ga167499a98081b0a9268cbd962023f0a0}





# struct `scy::DelegateCallback< C, 3, true, P, P2, P3 >` {#structscy_1_1DelegateCallback_3_01C_00_013_00_01true_00_01P_00_01P2_00_01P3_01_4}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual void emit(void * sender,P arg,P2 arg2,P3 arg3,void *) const` | 

## Members

#### `public inline virtual void emit(void * sender,P arg,P2 arg2,P3 arg3,void *) const` {#group__base_1ga1bf55048047ead3e3986773028872d09}





# struct `scy::DelegateCallback< C, 4, false, P, P2, P3, P4 >` {#structscy_1_1DelegateCallback_3_01C_00_014_00_01false_00_01P_00_01P2_00_01P3_00_01P4_01_4}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual void emit(void *,P arg,P2 arg2,P3 arg3,P4 arg4) const` | 

## Members

#### `public inline virtual void emit(void *,P arg,P2 arg2,P3 arg3,P4 arg4) const` {#group__base_1ga386e5d9048abdc6c504ee1253a210224}





# struct `scy::DelegateCallback< C, 4, true, P, P2, P3, P4 >` {#structscy_1_1DelegateCallback_3_01C_00_014_00_01true_00_01P_00_01P2_00_01P3_00_01P4_01_4}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual void emit(void * sender,P arg,P2 arg2,P3 arg3,P4 arg4) const` | 

## Members

#### `public inline virtual void emit(void * sender,P arg,P2 arg2,P3 arg3,P4 arg4) const` {#group__base_1ga83e5c853574ee2dfc008a08ccd95bee9}





# struct `scy::DelegateCallback< C, 8, false, PolymorphicT >` {#structscy_1_1DelegateCallback_3_01C_00_018_00_01false_00_01PolymorphicT_01_4}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual DefineCallbackFields void emit(void *,`[`basic::Polymorphic`](#classscy_1_1basic_1_1Polymorphic)` & data,void *,void *,void *) const` | 

## Members

#### `public inline virtual DefineCallbackFields void emit(void *,`[`basic::Polymorphic`](#classscy_1_1basic_1_1Polymorphic)` & data,void *,void *,void *) const` {#group__base_1gaf640d5053772ac2780374815deea6a9b}





# struct `scy::DelegateCallback< C, 8, true, PolymorphicT >` {#structscy_1_1DelegateCallback_3_01C_00_018_00_01true_00_01PolymorphicT_01_4}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual DefineCallbackFields void emit(void * sender,`[`basic::Polymorphic`](#classscy_1_1basic_1_1Polymorphic)` & data,void *,void *,void *) const` | 

## Members

#### `public inline virtual DefineCallbackFields void emit(void * sender,`[`basic::Polymorphic`](#classscy_1_1basic_1_1Polymorphic)` & data,void *,void *,void *) const` {#group__base_1gae472abfa68c1f1920d2344bb0dfa896a}





# struct `scy::Error` {#structscy_1_1Error}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public int errorno` | 
`public std::string message` | 
`public std::exception_ptr exception` | 
`public inline  Error()` | 
`public inline  Error(const std::string & msg)` | 
`public inline  Error(const char * msg)` | 
`public inline bool any() const` | 
`public inline void reset()` | 
`public inline void rethrow()` | 

## Members

#### `public int errorno` {#group__base_1ga192eee018e1e6d3be5a3aa5544b54ce3}





#### `public std::string message` {#group__base_1ga2c262d00d0cfd95c2d7326a6907269da}





#### `public std::exception_ptr exception` {#group__base_1gafa6e4615e88233c1ec0d1f0cb56b05f7}





#### `public inline  Error()` {#group__base_1gaab8c612dc5997bbf6b57397ec1a0e6d9}





#### `public inline  Error(const std::string & msg)` {#group__base_1ga75e04b5142b2bcec58bf418dd15253c7}





#### `public inline  Error(const char * msg)` {#group__base_1gab773389bc3179a4587bb57fba06addbe}





#### `public inline bool any() const` {#group__base_1ga6acffca8b048905bbc39b40a9f6a589c}





#### `public inline void reset()` {#group__base_1ga55e9f6b538f9526c20ccafe8df691f7d}





#### `public inline void rethrow()` {#group__base_1gabe0a98baa3810d0e0ed301c4ce44b3b1}





# struct `scy::IPacketInfo` {#structscy_1_1IPacketInfo}




An abstract interface for packet sources to provide extra information about packets.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  IPacketInfo()` | 
`public inline virtual  ~IPacketInfo()` | 
`public `[`IPacketInfo`](#structscy_1_1IPacketInfo)` * clone() const` | 

## Members

#### `public inline  IPacketInfo()` {#group__base_1gaca04fa38dbe8065e4807fef815da5cce}





#### `public inline virtual  ~IPacketInfo()` {#group__base_1ga8723fca6fde92c8e898c95f6ce936d16}





#### `public `[`IPacketInfo`](#structscy_1_1IPacketInfo)` * clone() const` {#group__base_1gac1849ccc9dc43d4d78e19437030a1bf0}





# struct `scy::LogStream` {#structscy_1_1LogStream}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public LogLevel level` | 
`public int line` | 
`public std::string realm` | 
`public std::string address` | 
`public std::ostringstream message` | 
`public std::time_t ts` | 
`public `[`LogChannel`](api-base.md#classscy_1_1LogChannel)` * channel` | 
`public  LogStream(LogLevel level,const std::string & realm,int line,const void * ptr,const char * channel)` | 
`public  LogStream(LogLevel level,const std::string & realm,const std::string & address)` | 
`public  LogStream(const `[`LogStream`](#structscy_1_1LogStream)` & that)` | 
`public  ~LogStream()` | 
`public inline `[`LogStream`](#structscy_1_1LogStream)` & operator<<(const LogLevel data)` | 
`public inline `[`LogStream`](#structscy_1_1LogStream)` & operator<<(`[`LogChannel`](#classscy_1_1LogChannel)` * data)` | 
`public template<typename T>`  <br/>`inline `[`LogStream`](#structscy_1_1LogStream)` & operator<<(const T & data)` | 
`public inline `[`LogStream`](#structscy_1_1LogStream)` & operator<<(std::ostream &(*)(std::ostream &) f)` | 

## Members

#### `public LogLevel level` {#group__base_1ga73f3f956a419da4225b19026d2e9a7e0}





#### `public int line` {#group__base_1ga417a122fd717a6d69e274fa417c7edda}





#### `public std::string realm` {#group__base_1ga8c22e0461abfd723edba37db20ff5999}





#### `public std::string address` {#group__base_1ga4eaa7a30c6f5c418652e759939700920}





#### `public std::ostringstream message` {#group__base_1ga12b091f0034ac12630df196735b2c8cc}





#### `public std::time_t ts` {#group__base_1ga4ef889027fb73f66c36c6cde4e21c255}





#### `public `[`LogChannel`](api-base.md#classscy_1_1LogChannel)` * channel` {#group__base_1gab30b5106bdb89ddddcd5c51011e5965e}





#### `public  LogStream(LogLevel level,const std::string & realm,int line,const void * ptr,const char * channel)` {#group__base_1ga87cc09db3ccb2b4cd7fb3d9af1423e07}





#### `public  LogStream(LogLevel level,const std::string & realm,const std::string & address)` {#group__base_1ga438a939767642dd9c9be284143f2bc18}





#### `public  LogStream(const `[`LogStream`](#structscy_1_1LogStream)` & that)` {#group__base_1ga234f0a55d2faddf5b97dca2ae579c23b}





#### `public  ~LogStream()` {#group__base_1gab14eceb8999d7cc3199635447ade4297}





#### `public inline `[`LogStream`](#structscy_1_1LogStream)` & operator<<(const LogLevel data)` {#group__base_1gaf46ff14bd44d66132367910c61549a55}





#### `public inline `[`LogStream`](#structscy_1_1LogStream)` & operator<<(`[`LogChannel`](#classscy_1_1LogChannel)` * data)` {#group__base_1ga8cad827da49076e479eb9e1544056df4}





#### `public template<typename T>`  <br/>`inline `[`LogStream`](#structscy_1_1LogStream)` & operator<<(const T & data)` {#group__base_1ga6f95a78048e9f7d52189c39f6b4db842}





#### `public inline `[`LogStream`](#structscy_1_1LogStream)` & operator<<(std::ostream &(*)(std::ostream &) f)` {#group__base_1ga372aad7ed9ef23916ba7ae94acaad14a}



Handle std::endl flags. This method flushes the log message and queues it for write. WARNING: After using std::endl to flush the message pointer should not be accessed.

# struct `scy::OptionParser` {#structscy_1_1OptionParser}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::string exepath` | 
`public OptionMap args` | 
`public  OptionParser(int argc,char * argv,const char * delim)` | 
`public inline bool has(const char * key)` | 
`public inline std::string get(const char * key)` | 
`public template<typename NumericType>`  <br/>`inline NumericType get(const char * key)` | 

## Members

#### `public std::string exepath` {#group__base_1gacf94798c87c7e8a3d9c0fee9f0b9b021}





#### `public OptionMap args` {#group__base_1ga3f72252b7f830d90de66dcefa2372585}





#### `public  OptionParser(int argc,char * argv,const char * delim)` {#group__base_1gacfaf92525d5f46c8c4165b4e9687a01e}





#### `public inline bool has(const char * key)` {#group__base_1gadce3a04bb7896063238940156056e18e}





#### `public inline std::string get(const char * key)` {#group__base_1ga975b86adb62cff3250a769a9e63d2d7c}





#### `public template<typename NumericType>`  <br/>`inline NumericType get(const char * key)` {#group__base_1gadc8dc1da69c345f00ee501b8b163da3b}





# struct `scy::PacketAdapterReference` {#structscy_1_1PacketAdapterReference}


For 0.8.x compatibility.

Provides a reference to a PacketSignal instance.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`PacketStreamAdapter`](api-base.md#classscy_1_1PacketStreamAdapter)` * ptr` | 
`public `[`ScopedPointer`](api-base.md#classscy_1_1ScopedPointer)` * deleter` | 
`public int order` | 
`public bool syncState` | 
`public inline  PacketAdapterReference(`[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` * ptr,`[`ScopedPointer`](#classscy_1_1ScopedPointer)` * deleter,int order,bool syncState)` | 
`public inline  ~PacketAdapterReference()` | 

## Members

#### `public `[`PacketStreamAdapter`](api-base.md#classscy_1_1PacketStreamAdapter)` * ptr` {#group__base_1ga91ef0542de1409ffa2a4a55d735abc99}





#### `public `[`ScopedPointer`](api-base.md#classscy_1_1ScopedPointer)` * deleter` {#group__base_1ga6e45237650bc2f174d331b0010f9a636}





#### `public int order` {#group__base_1ga1ede9883ce7a84245fd2ed7b6428ff1d}





#### `public bool syncState` {#group__base_1ga67b82c55538ee173a9305b0dad953f3f}





#### `public inline  PacketAdapterReference(`[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` * ptr,`[`ScopedPointer`](#classscy_1_1ScopedPointer)` * deleter,int order,bool syncState)` {#group__base_1gaa86cda4ece5d3b3d922775d80f70b0c5}





#### `public inline  ~PacketAdapterReference()` {#group__base_1gac6089625a26f4d7a2b17c4ef43dbb052}





# struct `scy::PacketCreationStrategy` {#structscy_1_1PacketCreationStrategy}

```
struct scy::PacketCreationStrategy
  : public scy::IPacketCreationStrategy
```  



This template class implements an adapter that sits between an [SignalBase](#classscy_1_1SignalBase) and an object receiving notifications from it.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  PacketCreationStrategy(int priority)` | 
`public inline virtual  ~PacketCreationStrategy()` | 
`public inline virtual `[`IPacket`](#classscy_1_1IPacket)` * create(const `[`ConstBuffer`](#classscy_1_1ConstBuffer)` & buffer,std::size_t & nread) const` | 
`public inline virtual int priority() const` | 
`protected int _priority` | 

## Members

#### `public inline  PacketCreationStrategy(int priority)` {#group__base_1gaf5895e8457d4d23df0a521026546ca73}





#### `public inline virtual  ~PacketCreationStrategy()` {#group__base_1gafad0e19b9e69c85139949c859da8ac1f}





#### `public inline virtual `[`IPacket`](#classscy_1_1IPacket)` * create(const `[`ConstBuffer`](#classscy_1_1ConstBuffer)` & buffer,std::size_t & nread) const` {#group__base_1gac6a286bb4fc399aba4d44a41a104a40e}





#### `public inline virtual int priority() const` {#group__base_1ga4f64d063389f2d1fdebfe0d1f84786a0}





#### `protected int _priority` {#group__base_1ga192649e00c46e3d9ce641e448f207ea9}





# struct `scy::PacketStreamState` {#structscy_1_1PacketStreamState}

```
struct scy::PacketStreamState
  : public scy::State
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual std::string str(unsigned int id) const` | 

## Members

#### `public inline virtual std::string str(unsigned int id) const` {#group__base_1gaaee1b46215fbe13176a0de4c4b926cc4}





# struct `scy::SharedLibrary` {#structscy_1_1SharedLibrary}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline bool open(const std::string & path)` | 
`public inline void close()` | 
`public inline bool sym(const char * name,void ** ptr)` | 
`public inline void setError(const std::string & prefix)` | 
`public inline std::string error() const` | 
`protected uv_lib_t _lib` | 
`protected std::string _error` | 

## Members

#### `public inline bool open(const std::string & path)` {#group__base_1ga21dfcbd8f0adda4d0bebd1a317570783}



Opens a shared library. The filename is in utf-8. Returns true on success and false on error. Call `SharedLibrary::error()` to get the error message.

#### `public inline void close()` {#group__base_1gac139b2afe33befeb2079d60327729f26}





#### `public inline bool sym(const char * name,void ** ptr)` {#group__base_1ga8028a3c607dfd1df3f5cb3614bd00a8b}



Retrieves a data pointer from a dynamic library. It is legal for a symbol to map to nullptr. Returns 0 on success and -1 if the symbol was not found.

#### `public inline void setError(const std::string & prefix)` {#group__base_1gaa72dbffb419561d3330b00086846d43b}





#### `public inline std::string error() const` {#group__base_1gad238435837ffd636d54e7b18973c64a9}





#### `protected uv_lib_t _lib` {#group__base_1ga69563904f0fb50a897c217c372110244}





#### `protected std::string _error` {#group__base_1gaa920397bfdf4e812eef6fcd8d03c4455}





# struct `scy::TransactionState` {#structscy_1_1TransactionState}

```
struct scy::TransactionState
  : public scy::State
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual std::string str(unsigned int id) const` | 

## Members

#### `public inline virtual std::string str(unsigned int id) const` {#group__base_1ga3bc0a991c145f88c468ef75cf3be51be}





# namespace `scy::async` {#namespacescy_1_1async}

Classes for asynchronous programming.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[`scy::async::Flag`](#classscy_1_1async_1_1Flag)    | 
`class `[`scy::async::Runnable`](#classscy_1_1async_1_1Runnable)    | 
`class `[`scy::async::Runner`](#classscy_1_1async_1_1Runner)    | 
`class `[`scy::async::Sendable`](#classscy_1_1async_1_1Sendable)    | 
`class `[`scy::async::Startable`](#classscy_1_1async_1_1Startable)    | 
# class `scy::async::Flag` {#classscy_1_1async_1_1Flag}




A concurrent flag which can be used to request task cancellation.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  Flag()` | 
`public inline bool get() const` | 
`public inline void set(bool flag)` | 

## Members

#### `public inline  Flag()` {#group__base_1ga1eff0601643d0e7ea73cee8defefd067}





#### `public inline bool get() const` {#group__base_1ga86270328f4c3dd1ef93566d76c043843}





#### `public inline void set(bool flag)` {#group__base_1gae10f7c92a104de02340f79b50ad3a05c}





# class `scy::async::Runnable` {#classscy_1_1async_1_1Runnable}




A generic interface for classes that can be run and cancelled.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  Runnable()` | 
`public inline virtual  ~Runnable()` | 
`public void run()` | The run method will be called by the async context.
`public inline virtual void cancel(bool flag)` | 
`public inline virtual bool cancelled() const` | True when the task has been cancelled.

## Members

#### `public inline  Runnable()` {#group__base_1ga518654b813972ce9dcb0d7227b473c4d}





#### `public inline virtual  ~Runnable()` {#group__base_1gaeb5fc7cb8b6c40070d1403e319f17880}





#### `public void run()` {#group__base_1gafa78ae78ab96f591e88e629ac4b7699d}

The run method will be called by the async context.



#### `public inline virtual void cancel(bool flag)` {#group__base_1gadf150b3fdcef33cb5144119460c8729a}



Cancel the current task. The [run()](#group__base_1gafa78ae78ab96f591e88e629ac4b7699d) method should return ASAP.

#### `public inline virtual bool cancelled() const` {#group__base_1ga7efc7639f668b72480625cc0f34f1eaa}

True when the task has been cancelled.



# class `scy::async::Runner` {#classscy_1_1async_1_1Runner}




[Runner](#classscy_1_1async_1_1Runner) is a virtual interface for implementing asynchronous objects such as threads and futures.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Runner()` | 
`public virtual  ~Runner()` | 
`public virtual void start(`[`async::Runnable`](#classscy_1_1async_1_1Runnable)` & target)` | 
`public virtual void start(std::function< void()> target)` | 
`public virtual void start(std::function< void(void *)> target,void * arg)` | Starts the thread with the given target.
`public bool started() const` | Returns true if the async context has been started.
`public bool running() const` | Returns true if the async context is currently running.
`public void cancel()` | Cancels the async context.
`public bool cancelled() const` | 
`public bool repeating() const` | Returns true if the [Runner](api-base.md#classscy_1_1async_1_1Runner) is operating in repeating mode.
`public uv_thread_t tid() const` | Return the native thread ID.
`public void setRepeating(bool flag)` | 
`public bool async() const` | 
`protected Context::ptr pContext` | Shared pointer to the internal [Runner::Context](api-base.md#structscy_1_1async_1_1Runner_1_1Context).
`protected void startAsync()` | Start the context from the control thread.
`protected  Runner(const `[`Runner`](#classscy_1_1async_1_1Runner)` &)` | 
`protected `[`Runner`](#classscy_1_1async_1_1Runner)` & operator=(const `[`Runner`](#classscy_1_1async_1_1Runner)` &)` | 

## Members

#### `public  Runner()` {#group__base_1ga181039d16f63c0a216937f3779d8ed43}





#### `public virtual  ~Runner()` {#group__base_1ga9b0ecbe08364f576d896c500b303cb78}





#### `public virtual void start(`[`async::Runnable`](#classscy_1_1async_1_1Runnable)` & target)` {#group__base_1ga55d314b43171fb16e24700322e801167}





#### `public virtual void start(std::function< void()> target)` {#group__base_1gad1ef20c4e1757073af31c3a0005171bc}





#### `public virtual void start(std::function< void(void *)> target,void * arg)` {#group__base_1ga5d6d4917ea3e916e43228225523cb2ae}

Starts the thread with the given target.



#### `public bool started() const` {#group__base_1ga6d9f41b24062c9e23adbf2bcb18c2ea7}

Returns true if the async context has been started.



#### `public bool running() const` {#group__base_1gae69cae4a092e02ee69f22c30bbb4fb8c}

Returns true if the async context is currently running.



#### `public void cancel()` {#group__base_1gaa2fae5ff617d30c3922deb2935815899}

Cancels the async context.



#### `public bool cancelled() const` {#group__base_1gad8e31342c8c3eb0c5fe2bf64c5fdd5d7}



True when the task has been cancelled. It is up to the implementation to return at the earliest possible time.

#### `public bool repeating() const` {#group__base_1gabfb62eab63b3a3caa3617fce3adf7d94}

Returns true if the [Runner](#classscy_1_1async_1_1Runner) is operating in repeating mode.



#### `public uv_thread_t tid() const` {#group__base_1ga68aeff8aa5b34520a2d3c838b8169f4f}

Return the native thread ID.



#### `public void setRepeating(bool flag)` {#group__base_1gad2c90faf9f1f6ce22b84b4fd1b0c75d0}



This setting means the implementation should call the target function repeatedly until cancelled. The importance of this method to normalize the functionality of threadded and event loop driven [Runner](#classscy_1_1async_1_1Runner) models.

#### `public bool async() const` {#group__base_1gaf3d43124da5c0b33433a5464f4961b45}



Returns true if the implementation is thread-based, or false if it belongs to an event loop.

#### `protected Context::ptr pContext` {#group__base_1ga5282f8e4193b4ebf4ffa2044348fc8de}

Shared pointer to the internal [Runner::Context](#structscy_1_1async_1_1Runner_1_1Context).



#### `protected void startAsync()` {#group__base_1ga30c1909db971c286bfdad8a4095decc2}

Start the context from the control thread.



#### `protected  Runner(const `[`Runner`](#classscy_1_1async_1_1Runner)` &)` {#group__base_1gaeb97a8c9cbe19809c76f01d3a42d54ca}





#### `protected `[`Runner`](#classscy_1_1async_1_1Runner)` & operator=(const `[`Runner`](#classscy_1_1async_1_1Runner)` &)` {#group__base_1gab9686d6beebfacea6eb308b957a1e6de}





# class `scy::async::Sendable` {#classscy_1_1async_1_1Sendable}




A generic interface for classes that can be sent and cancelled.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public bool send()` | 
`public inline virtual void cancel()` | 

## Members

#### `public bool send()` {#group__base_1ga08d904c4a44d5a0abde56081f7f02e7f}





#### `public inline virtual void cancel()` {#group__base_1gac52fd6569c020b8765245e77acbab811}





# class `scy::async::Startable` {#classscy_1_1async_1_1Startable}




A generic interface for a classes that can be started and stopped.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public void start()` | 
`public void stop()` | 

## Members

#### `public void start()` {#group__base_1gad05d7ed937d0144a6879e92cddf16e53}





#### `public void stop()` {#group__base_1gab309a7fe4c27e1ffc63a976cab20773d}





# namespace `scy::base64` {#namespacescy_1_1base64}



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`struct `[`scy::base64::Decoder`](#structscy_1_1base64_1_1Decoder)    | 
`struct `[`scy::base64::Encoder`](#structscy_1_1base64_1_1Encoder)    | 
# struct `scy::base64::Decoder` {#structscy_1_1base64_1_1Decoder}

```
struct scy::base64::Decoder
  : public scy::basic::Decoder
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public internal::decodestate _state` | 
`public int _buffersize` | 
`public inline  Decoder(int buffersize)` | 
`public inline int decode(char value_in)` | 
`public inline virtual std::size_t decode(const char * inbuf,std::size_t nread,char * outbuf)` | 
`public inline void decode(std::istream & istrm,std::ostream & ostrm)` | 

## Members

#### `public internal::decodestate _state` {#group__base_1ga852ccfc6b149bdbf6158bd830a00fb8f}





#### `public int _buffersize` {#group__base_1gae44073b77993e4a6a1461d75960aaa61}





#### `public inline  Decoder(int buffersize)` {#group__base_1ga3a55fbfacbfe7fb8ea87b7788d02f7b7}





#### `public inline int decode(char value_in)` {#group__base_1gaad048ac9b2ead706549fd6af65be6c74}





#### `public inline virtual std::size_t decode(const char * inbuf,std::size_t nread,char * outbuf)` {#group__base_1gaac8713c57da775e3cfd2f9685ab67880}





#### `public inline void decode(std::istream & istrm,std::ostream & ostrm)` {#group__base_1gaee095eb1f717a873862271efdf8b78f5}





# struct `scy::base64::Encoder` {#structscy_1_1base64_1_1Encoder}

```
struct scy::base64::Encoder
  : public scy::basic::Encoder
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public internal::encodestate _state` | 
`public int _buffersize` | 
`public inline  Encoder(int buffersize)` | 
`public inline void encode(std::istream & istrm,std::ostream & ostrm)` | 
`public inline void encode(const std::string & in,std::string & out)` | 
`public inline virtual std::size_t encode(const char * inbuf,std::size_t nread,char * outbuf)` | 
`public inline virtual std::size_t finalize(char * outbuf)` | 
`public inline void setLineLength(int lineLength)` | 

## Members

#### `public internal::encodestate _state` {#group__base_1gad85db3c5637d5b339034826bdbc0e786}





#### `public int _buffersize` {#group__base_1ga759e0716429cb47740e4a65ab28acc6b}





#### `public inline  Encoder(int buffersize)` {#group__base_1ga0b4bbf5fe617990a4044d61a5aa4d9b5}





#### `public inline void encode(std::istream & istrm,std::ostream & ostrm)` {#group__base_1ga8cd9623054dd2d6eebee7561feeeaec5}





#### `public inline void encode(const std::string & in,std::string & out)` {#group__base_1ga169108c69ec1b416d1bdc1a21eb2114d}





#### `public inline virtual std::size_t encode(const char * inbuf,std::size_t nread,char * outbuf)` {#group__base_1gae44d73f1c4e83f770ebeaa4c1c5dd1a0}





#### `public inline virtual std::size_t finalize(char * outbuf)` {#group__base_1ga1dec1d8bf6adec5c7bcf26bd580b4f12}





#### `public inline void setLineLength(int lineLength)` {#group__base_1gaa95988d443bebd458b3c2e421a46d799}





# namespace `scy::hex` {#namespacescy_1_1hex}



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`struct `[`scy::hex::Decoder`](#structscy_1_1hex_1_1Decoder)    | 
`struct `[`scy::hex::Encoder`](#structscy_1_1hex_1_1Encoder)    | 
# struct `scy::hex::Decoder` {#structscy_1_1hex_1_1Decoder}

```
struct scy::hex::Decoder
  : public scy::basic::Decoder
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public char lastbyte` | 
`public inline  Decoder()` | 
`public inline virtual  ~Decoder()` | 
`public inline virtual std::size_t decode(const char * inbuf,std::size_t nread,char * outbuf)` | 
`public inline virtual std::size_t finalize(char *)` | 
`public inline bool readnext(const char * inbuf,std::size_t nread,std::size_t & rpos,char & c)` | 
`public inline int nybble(const int n)` | 
`public inline bool iswspace(const char c)` | 

## Members

#### `public char lastbyte` {#group__base_1gacf494fcbc21eb2ff254940ad66e3da68}





#### `public inline  Decoder()` {#group__base_1ga5998a95cdfedea8b8ab0a02065f0a624}





#### `public inline virtual  ~Decoder()` {#group__base_1ga9d6d431cf4d0739cf304afcbc2962b2d}





#### `public inline virtual std::size_t decode(const char * inbuf,std::size_t nread,char * outbuf)` {#group__base_1ga4c3695cf7297281abaf36837e6cfdb87}





#### `public inline virtual std::size_t finalize(char *)` {#group__base_1ga45d9c67415aa90fc99978b291ce80534}





#### `public inline bool readnext(const char * inbuf,std::size_t nread,std::size_t & rpos,char & c)` {#group__base_1gac078559becf81eb563f13ac16ca7e154}





#### `public inline int nybble(const int n)` {#group__base_1gad58c78f2177b39304b7e910d56eac83d}





#### `public inline bool iswspace(const char c)` {#group__base_1ga19c548e3b173984aba506a7f8a837d6f}





# struct `scy::hex::Encoder` {#structscy_1_1hex_1_1Encoder}

```
struct scy::hex::Encoder
  : public scy::basic::Encoder
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public int _linePos` | 
`public int _lineLength` | 
`public int _uppercase` | 
`public inline  Encoder()` | 
`public inline virtual std::size_t encode(const char * inbuf,std::size_t nread,char * outbuf)` | 
`public inline virtual std::size_t finalize(char *)` | 
`public inline void setUppercase(bool flag)` | 
`public inline void setLineLength(int lineLength)` | 

## Members

#### `public int _linePos` {#group__base_1ga288899d48739dcf0378d6b9f11dfe58f}





#### `public int _lineLength` {#group__base_1ga064c41e567fdbeb17a86c98f2bd278ff}





#### `public int _uppercase` {#group__base_1ga4c6c6ece9aee62f7edbe81a4462b6f9f}





#### `public inline  Encoder()` {#group__base_1gaa0343ef0f07c144b4a50748750004aee}





#### `public inline virtual std::size_t encode(const char * inbuf,std::size_t nread,char * outbuf)` {#group__base_1ga0df35033404692b03e7eea1b923fd2e1}





#### `public inline virtual std::size_t finalize(char *)` {#group__base_1ga9cb276db2215687bbd6cbba396d25afd}





#### `public inline void setUppercase(bool flag)` {#group__base_1gaf3135b7edfac6b752a05686c193e2495}





#### `public inline void setLineLength(int lineLength)` {#group__base_1ga9c23084ff0fc0b186777ca003e30deda}





# namespace `scy::basic` {#namespacescy_1_1basic}



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[`scy::basic::Decoder`](#classscy_1_1basic_1_1Decoder)    | 
`class `[`scy::basic::Encoder`](#classscy_1_1basic_1_1Encoder)    | 
`class `[`scy::basic::Polymorphic`](#classscy_1_1basic_1_1Polymorphic)    | A base module class for C++ callback polymorphism.
# class `scy::basic::Decoder` {#classscy_1_1basic_1_1Decoder}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  Decoder()` | 
`public inline virtual  ~Decoder()` | 
`public std::size_t decode(const char * inbuf,std::size_t nread,char * outbuf)` | 
`public inline virtual std::size_t finalize(char *)` | 

## Members

#### `public inline  Decoder()` {#group__base_1ga7c9e45bb92ec807fc950726dd26ffc24}





#### `public inline virtual  ~Decoder()` {#group__base_1gabec8b063336f82af27d9d581e06ffa78}





#### `public std::size_t decode(const char * inbuf,std::size_t nread,char * outbuf)` {#group__base_1gad2dab5333fc1f1d485eaa4eb0cb83ac4}





#### `public inline virtual std::size_t finalize(char *)` {#group__base_1gae53e9c0df6acc234d2b1869344ff5e4c}





# class `scy::basic::Encoder` {#classscy_1_1basic_1_1Encoder}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  Encoder()` | 
`public inline virtual  ~Encoder()` | 
`public std::size_t encode(const char * inbuf,std::size_t nread,char * outbuf)` | 
`public inline virtual std::size_t finalize(char *)` | 

## Members

#### `public inline  Encoder()` {#group__base_1ga3c2b847f4a2fada5473a453f7cacae9c}





#### `public inline virtual  ~Encoder()` {#group__base_1gad716d841761d7c5a860249d08d260cb8}





#### `public std::size_t encode(const char * inbuf,std::size_t nread,char * outbuf)` {#group__base_1ga1d9c0570e3098ba42789e83ca1874bde}





#### `public inline virtual std::size_t finalize(char *)` {#group__base_1ga7445728d5e04fdcb7e2d92beda4c8c4a}





# class `scy::basic::Polymorphic` {#classscy_1_1basic_1_1Polymorphic}


A base module class for C++ callback polymorphism.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual  ~Polymorphic()` | 
`public template<class T>`  <br/>`inline bool is()` | 
`public template<class T>`  <br/>`inline T * as(bool whiny)` | 
`public `[`scy::LogStream`](#structscy_1_1LogStream)` & log(const char * level) const` | 
`public const char * className() const` | 

## Members

#### `public inline virtual  ~Polymorphic()` {#group__base_1gadd2e064e08321befa2791a86c728284b}





#### `public template<class T>`  <br/>`inline bool is()` {#group__base_1ga8a806c748396e94182adddf4581f1319}





#### `public template<class T>`  <br/>`inline T * as(bool whiny)` {#group__base_1ga04c451f467024606700e4ad8588383b7}





#### `public `[`scy::LogStream`](#structscy_1_1LogStream)` & log(const char * level) const` {#group__base_1gaa6975647c67722fa4e537b6ccca2d5e8}





#### `public const char * className() const` {#group__base_1gad57198d84a39e99e2acd7d7436af925d}





# namespace `scy::ipc` {#namespacescy_1_1ipc}

Classes for inter-process communication.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[`scy::ipc::Queue`](#classscy_1_1ipc_1_1Queue)    | 
`class `[`scy::ipc::SyncQueue`](#classscy_1_1ipc_1_1SyncQueue)    | 
`struct `[`scy::ipc::Action`](#structscy_1_1ipc_1_1Action)    | Default action type for executing synchronized callbacks.
# class `scy::ipc::Queue` {#classscy_1_1ipc_1_1Queue}




IPC queue is for safely passing templated actions between threads and processes.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  Queue()` | 
`public inline virtual  ~Queue()` | 
`public inline virtual void push(TAction * action)` | 
`public inline virtual TAction * pop()` | 
`public inline virtual void runSync()` | 
`public inline virtual void close()` | 
`public inline virtual void post()` | 
`public inline void waitForSync()` | 
`protected mutable `[`Mutex`](api-thread.md#classscy_1_1Mutex)` _mutex` | 
`protected std::deque< TAction * > _actions` | 

## Members

#### `public inline  Queue()` {#group__base_1gac93ef0838e9baed8333908c8d65e7b8d}





#### `public inline virtual  ~Queue()` {#group__base_1ga9546d7f27045e6717afa2125dfafd053}





#### `public inline virtual void push(TAction * action)` {#group__base_1ga9c786634a41240b2c27b2bf3de496f9c}





#### `public inline virtual TAction * pop()` {#group__base_1gafdb3fb63903e8014198738138ad10fc4}





#### `public inline virtual void runSync()` {#group__base_1ga1e0208d28978fee4cb039205a0fbc020}





#### `public inline virtual void close()` {#group__base_1ga0efb103e4e82dc199cba8c3908183ea4}





#### `public inline virtual void post()` {#group__base_1ga3bb872c395705ae78b72cfafc188e1fd}





#### `public inline void waitForSync()` {#group__base_1ga394493f310e54d4bb33265cb840fefdf}





#### `protected mutable `[`Mutex`](api-thread.md#classscy_1_1Mutex)` _mutex` {#group__base_1gad45c34ad8611c7694110e41e2b2a1756}





#### `protected std::deque< TAction * > _actions` {#group__base_1ga238c51fb2dd7bf29152e4863d13fa1e5}





# class `scy::ipc::SyncQueue` {#classscy_1_1ipc_1_1SyncQueue}

```
class scy::ipc::SyncQueue
  : public scy::ipc::Queue< TAction >
```  



IPC synchronization queue is for passing templated actions between threads and the event loop we are synchronizing with.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  SyncQueue(uv::Loop * loop)` | 
`public inline virtual  ~SyncQueue()` | 
`public inline virtual void close()` | 
`public inline virtual void post()` | 
`public inline virtual `[`SyncContext`](#classscy_1_1SyncContext)` & sync()` | 
`protected `[`SyncContext`](api-base.md#classscy_1_1SyncContext)` _sync` | 

## Members

#### `public inline  SyncQueue(uv::Loop * loop)` {#group__base_1ga5d75b88fd48fd9b14a1300775471fcbb}





#### `public inline virtual  ~SyncQueue()` {#group__base_1gae3f23ea3eb42ed63fbb0850d43805178}





#### `public inline virtual void close()` {#group__base_1ga8546519d8077f5c05a52d9f2b926b233}





#### `public inline virtual void post()` {#group__base_1gabbb1f498c837051e29c0553ed4127db8}





#### `public inline virtual `[`SyncContext`](#classscy_1_1SyncContext)` & sync()` {#group__base_1ga1bf9d5d39b886274a18c14d0e96eeeca}





#### `protected `[`SyncContext`](api-base.md#classscy_1_1SyncContext)` _sync` {#group__base_1gac1bd3d9add3c68968054787288c53f47}





# struct `scy::ipc::Action` {#structscy_1_1ipc_1_1Action}


Default action type for executing synchronized callbacks.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public callback_t target` | 
`public void * arg` | 
`public std::string data` | 
`public inline  Action(callback_t target,void * arg,const std::string & data)` | 

## Members

#### `public callback_t target` {#group__base_1gad17a0f714b4d6127fb7d1dc3501edaf7}





#### `public void * arg` {#group__base_1ga7bfadcacf4497ab86260402b2f4129e7}





#### `public std::string data` {#group__base_1gaaf3ce54c7827ef8b2958a6caa8f9fbab}





#### `public inline  Action(callback_t target,void * arg,const std::string & data)` {#group__base_1ga71fe1afc55cdab44dff6ddbacb9065b1}





# namespace `scy::deleter` {#namespacescy_1_1deleter}

Deleter Functors.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`struct `[`scy::deleter::Array`](#structscy_1_1deleter_1_1Array)    | 
`struct `[`scy::deleter::Deferred`](#structscy_1_1deleter_1_1Deferred)    | 
`struct `[`scy::deleter::Dispose`](#structscy_1_1deleter_1_1Dispose)    | 
# struct `scy::deleter::Array` {#structscy_1_1deleter_1_1Array}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline void operator()(T * ptr)` | 

## Members

#### `public inline void operator()(T * ptr)` {#group__base_1ga2b9a2ee44aca9d8171c8188d1caa5e53}





# struct `scy::deleter::Deferred` {#structscy_1_1deleter_1_1Deferred}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline void operator()(T * ptr)` | 

## Members

#### `public inline void operator()(T * ptr)` {#group__base_1ga50265aec599f8850e428709b3ba77ca8}





# struct `scy::deleter::Dispose` {#structscy_1_1deleter_1_1Dispose}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline void operator()(T * ptr)` | 

## Members

#### `public inline void operator()(T * ptr)` {#group__base_1ga83955b9220bdccdbaac1ca7923c3ad65}





# namespace `scy::test` {#namespacescy_1_1test}

Modern unit testing classes.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[`scy::test::FunctionTest`](#classscy_1_1test_1_1FunctionTest)    | 
`class `[`scy::test::Test`](#classscy_1_1test_1_1Test)    | 
`class `[`scy::test::TestRunner`](#classscy_1_1test_1_1TestRunner)    | 
# class `scy::test::FunctionTest` {#classscy_1_1test_1_1FunctionTest}

```
class scy::test::FunctionTest
  : public scy::test::Test
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public voidfunc_t target` | 
`public inline  FunctionTest(voidfunc_t target,const std::string & name)` | 
`protected inline virtual  ~FunctionTest()` | 
`protected inline virtual void run()` | Called by the [TestRunner](api-base.md#classscy_1_1test_1_1TestRunner) to run the test.

## Members

#### `public voidfunc_t target` {#group__base_1ga80f8d710c844c7d7ad7629c2e970113b}





#### `public inline  FunctionTest(voidfunc_t target,const std::string & name)` {#group__base_1ga54a462e27f52346fd94636bb2457bf72}





#### `protected inline virtual  ~FunctionTest()` {#group__base_1gacc32a5204646832c258c30892449531e}





#### `protected inline virtual void run()` {#group__base_1ga5d3c89ca30abab0fe4020db97ed6e513}

Called by the [TestRunner](#classscy_1_1test_1_1TestRunner) to run the test.



# class `scy::test::Test` {#classscy_1_1test_1_1Test}




This class is for implementing any kind async test that is compatible with a [TestRunner](#classscy_1_1test_1_1TestRunner).

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::string name` | The name of the test.
`public error_list_t errors` | A list of test errors.
`public double duration` | The test run duration for benchmarking.
`public  Test(const std::string & name)` | 
`public virtual  ~Test()` | Should remain protected.
`public void run()` | Called by the [TestRunner](api-base.md#classscy_1_1test_1_1TestRunner) to run the test.
`public bool passed()` | Return true when the test passed without errors.
`protected  Test(const `[`Test`](#classscy_1_1test_1_1Test)` & test)` | 
`protected `[`Test`](#classscy_1_1test_1_1Test)` & operator=(`[`Test`](#classscy_1_1test_1_1Test)` const &)` | 

## Members

#### `public std::string name` {#group__base_1ga05c0192e474a337da0822200d2ad8084}

The name of the test.



#### `public error_list_t errors` {#group__base_1gaa25c01102de7399acdc3181445cb3073}

A list of test errors.



#### `public double duration` {#group__base_1gac56aeb8b24a0ad4d6ac4d5c8f8f2a260}

The test run duration for benchmarking.



#### `public  Test(const std::string & name)` {#group__base_1ga77ec963c31149717e725fb25a0d9c3e2}





#### `public virtual  ~Test()` {#group__base_1ga190692773ad0ccef364dbfcd5a10b2a8}

Should remain protected.



#### `public void run()` {#group__base_1ga4b2e88e8a32def45844c9fdcf1b4bc5e}

Called by the [TestRunner](#classscy_1_1test_1_1TestRunner) to run the test.



#### `public bool passed()` {#group__base_1ga7136d60a9c295927a26d1eee662a9112}

Return true when the test passed without errors.



#### `protected  Test(const `[`Test`](#classscy_1_1test_1_1Test)` & test)` {#group__base_1ga6635c765383ab00f182e7bfcc03a2fa5}





#### `protected `[`Test`](#classscy_1_1test_1_1Test)` & operator=(`[`Test`](#classscy_1_1test_1_1Test)` const &)` {#group__base_1gaad1bf3675df71fac8cf5f3260bd6b991}





# class `scy::test::TestRunner` {#classscy_1_1test_1_1TestRunner}




The [TestRunner](#classscy_1_1test_1_1TestRunner) is a queue in charge of running one or many tests.

The [TestRunner](#classscy_1_1test_1_1TestRunner) continually loops through each test in the test list calling the test's [run()](#group__base_1gae237ec747f0a785fb2514154a46022b3) method.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  TestRunner()` | 
`public virtual  ~TestRunner()` | 
`public void add(`[`Test`](#classscy_1_1test_1_1Test)` * test)` | Add a test to the runner.
`public `[`Test`](#classscy_1_1test_1_1Test)` * get(const std::string & name) const` | 
`public void run()` | Called by the async context to run the next test.
`public void clear()` | Destroy and clears all managed tests.
`public `[`Test`](#classscy_1_1test_1_1Test)` * current() const` | Return the currently active [Test](api-base.md#classscy_1_1test_1_1Test) or nullptr.
`public test_list_t tests() const` | Return the list of tests.
`public error_map_t errors() const` | Return a map of tests and errors.
`public bool passed() const` | Return true if all tests passed.
`protected mutable `[`Mutex`](api-thread.md#classscy_1_1Mutex)` _mutex` | 
`protected std::list< `[`Test`](api-base.md#classscy_1_1test_1_1Test)` * > _tests` | 
`protected `[`Test`](api-base.md#classscy_1_1test_1_1Test)` * _current` | 

## Members

#### `public  TestRunner()` {#group__base_1gaa969f22aa934715fccb037572de9d51b}





#### `public virtual  ~TestRunner()` {#group__base_1ga816353a412bed8f169ecfaf82b34cd8b}





#### `public void add(`[`Test`](#classscy_1_1test_1_1Test)` * test)` {#group__base_1ga576996da07db6bf6de177586530c352c}

Add a test to the runner.



#### `public `[`Test`](#classscy_1_1test_1_1Test)` * get(const std::string & name) const` {#group__base_1gae6eae9609b3a4b78e8508e930e924220}



Return a pointer to the test matching the given name, or nullptr if no matching test exists.

#### `public void run()` {#group__base_1gae237ec747f0a785fb2514154a46022b3}

Called by the async context to run the next test.



#### `public void clear()` {#group__base_1ga6dd3a9bc14a7c4223d8811bc11651f82}

Destroy and clears all managed tests.



#### `public `[`Test`](#classscy_1_1test_1_1Test)` * current() const` {#group__base_1gad5960442871ffed66bf6d14a159ec29f}

Return the currently active [Test](#classscy_1_1test_1_1Test) or nullptr.



#### `public test_list_t tests() const` {#group__base_1ga82580d13c7625f7ceb0fe07dba1a9da4}

Return the list of tests.



#### `public error_map_t errors() const` {#group__base_1ga5fd610a35291b0b13e39d9f0024cc745}

Return a map of tests and errors.



#### `public bool passed() const` {#group__base_1gaa899250db70a6f7d3855a58ac46b9385}

Return true if all tests passed.



#### `protected mutable `[`Mutex`](api-thread.md#classscy_1_1Mutex)` _mutex` {#group__base_1ga99eb18cc606b0187d055e7faf20e9b95}





#### `protected std::list< `[`Test`](api-base.md#classscy_1_1test_1_1Test)` * > _tests` {#group__base_1ga96216808dfb4a1e3af39abb3933c728c}





#### `protected `[`Test`](api-base.md#classscy_1_1test_1_1Test)` * _current` {#group__base_1ga3e5f349d8600497eca5acacd6e853ad7}





# namespace `scy::util` {#namespacescy_1_1util}



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`struct `[`scy::util::Version`](#structscy_1_1util_1_1Version)    | 
# struct `scy::util::Version` {#structscy_1_1util_1_1Version}






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public int major` | 
`public int minor` | 
`public int revision` | 
`public int build` | 
`public inline  Version(const std::string & version)` | 
`public inline bool operator<(const `[`Version`](#structscy_1_1util_1_1Version)` & other)` | 
`public inline bool operator==(const `[`Version`](#structscy_1_1util_1_1Version)` & other)` | 

## Members

#### `public int major` {#group__base_1ga05e06561c82da53755f84c1a0e52282b}





#### `public int minor` {#group__base_1ga51722ff9365a52c346ee5a2b500ef719}





#### `public int revision` {#group__base_1ga9fd57e2361545ed40ea4c7a19240d10b}





#### `public int build` {#group__base_1gad20634ae741953afa2129e82bfdbdd8a}





#### `public inline  Version(const std::string & version)` {#group__base_1gabefe33fadbd31c287b84c33a34c0cdb4}





#### `public inline bool operator<(const `[`Version`](#structscy_1_1util_1_1Version)` & other)` {#group__base_1gac7dbfa8df99679fc2fc161523dc89d50}





#### `public inline bool operator==(const `[`Version`](#structscy_1_1util_1_1Version)` & other)` {#group__base_1ga11b1e27149c742ffb0b9c6b3fe2405a9}





