# Module <!-- group --> `base`

The `base` module contains reusable cross platform tools and utilities.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`namespace `[``scy``](#namespacescy)    | 
`namespace `[``scy::async``](#namespacescy_1_1async)    | Classes for asynchronous programming.
`namespace `[``scy::base64``](#namespacescy_1_1base64)    | 
`namespace `[``scy::hex``](#namespacescy_1_1hex)    | 
`namespace `[``scy::basic``](#namespacescy_1_1basic)    | 
`namespace `[``scy::ipc``](#namespacescy_1_1ipc)    | Classes for inter-process communication.
`namespace `[``scy::deleter``](#namespacescy_1_1deleter)    | Deleter Functors.
`namespace `[``scy::test``](#namespacescy_1_1test)    | Modern unit testing classes.
`namespace `[``scy::util``](#namespacescy_1_1util)    | 
`class `[``scy::Application``](#classscy_1_1Application)    | 
`class `[``scy::async::Runnable``](#classscy_1_1async_1_1Runnable)    | 
`class `[``scy::async::Runner``](#classscy_1_1async_1_1Runner)    | 
`class `[``scy::async::Flag``](#classscy_1_1async_1_1Flag)    | 
`class `[``scy::async::Startable``](#classscy_1_1async_1_1Startable)    | 
`class `[``scy::async::Sendable``](#classscy_1_1async_1_1Sendable)    | 
`class `[``scy::MutableBuffer``](#classscy_1_1MutableBuffer)    | 
`class `[``scy::ConstBuffer``](#classscy_1_1ConstBuffer)    | 
`class `[``scy::BitReader``](#classscy_1_1BitReader)    | A [BitReader](#classscy_1_1BitReader) for reading binary streams.
`class `[``scy::BitWriter``](#classscy_1_1BitWriter)    | 
`class `[``scy::AbstractCollection``](#classscy_1_1AbstractCollection)    | 
`class `[``scy::PointerCollection``](#classscy_1_1PointerCollection)    | 
`class `[``scy::LiveCollection``](#classscy_1_1LiveCollection)    | 
`class `[``scy::KVCollection``](#classscy_1_1KVCollection)    | A reusable stack based unique key-value store for DRY coding.
`class `[``scy::NVCollection``](#classscy_1_1NVCollection)    | 
`class `[``scy::Configuration``](#classscy_1_1Configuration)    | 
`class `[``scy::ScopedConfiguration``](#classscy_1_1ScopedConfiguration)    | 
`class `[``scy::Timestamp``](#classscy_1_1Timestamp)    | 
`class `[``scy::Timespan``](#classscy_1_1Timespan)    | A class that represents time spans up to microsecond resolution.
`class `[``scy::DateTime``](#classscy_1_1DateTime)    | 
`class `[``scy::Timezone``](#classscy_1_1Timezone)    | This class provides information about the current timezone.
`class `[``scy::LocalDateTime``](#classscy_1_1LocalDateTime)    | 
`class `[``scy::DateTimeFormat``](#classscy_1_1DateTimeFormat)    | 
`class `[``scy::DateTimeFormatter``](#classscy_1_1DateTimeFormatter)    | 
`class `[``scy::DateTimeParser``](#classscy_1_1DateTimeParser)    | 
`class `[``scy::Stopwatch``](#classscy_1_1Stopwatch)    | 
`class `[``scy::Timeout``](#classscy_1_1Timeout)    | Simple millisecond timeout counter which expires after a given delay.
`class `[``scy::TimedToken``](#classscy_1_1TimedToken)    | A token that expires after the specified duration.
`class `[``scy::Delegate``](#classscy_1_1Delegate)    | 
`class `[``scy::Idler``](#classscy_1_1Idler)    | 
`class `[``scy::basic::Decoder``](#classscy_1_1basic_1_1Decoder)    | 
`class `[``scy::basic::Encoder``](#classscy_1_1basic_1_1Encoder)    | 
`class `[``scy::basic::Polymorphic``](#classscy_1_1basic_1_1Polymorphic)    | A base module class for C++ callback polymorphism.
`class `[``scy::ipc::Queue``](#classscy_1_1ipc_1_1Queue)    | 
`class `[``scy::ipc::SyncQueue``](#classscy_1_1ipc_1_1SyncQueue)    | 
`class `[``scy::LogWriter``](#classscy_1_1LogWriter)    | 
`class `[``scy::AsyncLogWriter``](#classscy_1_1AsyncLogWriter)    | 
`class `[``scy::Logger``](#classscy_1_1Logger)    | 
`class `[``scy::LogChannel``](#classscy_1_1LogChannel)    | 
`class `[``scy::ConsoleChannel``](#classscy_1_1ConsoleChannel)    | 
`class `[``scy::FileChannel``](#classscy_1_1FileChannel)    | 
`class `[``scy::RotatingFileChannel``](#classscy_1_1RotatingFileChannel)    | 
`class `[``scy::GarbageCollector``](#classscy_1_1GarbageCollector)    | Simple garbage collector for deferred pointer deletion.
`class `[``scy::ScopedPointer``](#classscy_1_1ScopedPointer)    | Scoped Pointer Classes.
`class `[``scy::ScopedRawPointer``](#classscy_1_1ScopedRawPointer)    | 
`class `[``scy::ScopedSharedPointer``](#classscy_1_1ScopedSharedPointer)    | 
`class `[``scy::SharedObject``](#classscy_1_1SharedObject)    | 
`class `[``scy::ScopedLock``](#classscy_1_1ScopedLock)    | 
`class `[``scy::Mutex``](#classscy_1_1Mutex)    | 
`class `[``scy::IPacket``](#classscy_1_1IPacket)    | 
`class `[``scy::FlagPacket``](#classscy_1_1FlagPacket)    | A simple flag packet for sending state flags along the packet stream.
`class `[``scy::RawPacket``](#classscy_1_1RawPacket)    | 
`class `[``scy::IPacketCreationStrategy``](#classscy_1_1IPacketCreationStrategy)    | 
`class `[``scy::PacketFactory``](#classscy_1_1PacketFactory)    | 
`class `[``scy::ThreadedStreamReader``](#classscy_1_1ThreadedStreamReader)    | 
`class `[``scy::StreamWriter``](#classscy_1_1StreamWriter)    | 
`class `[``scy::SyncPacketQueue``](#classscy_1_1SyncPacketQueue)    | 
`class `[``scy::AsyncPacketQueue``](#classscy_1_1AsyncPacketQueue)    | 
`class `[``scy::PacketStreamAdapter``](#classscy_1_1PacketStreamAdapter)    | 
`class `[``scy::PacketProcessor``](#classscy_1_1PacketProcessor)    | For 0.8.x compatibility.
`class `[``scy::PacketStream``](#classscy_1_1PacketStream)    | 
`class `[``scy::PacketTransaction``](#classscy_1_1PacketTransaction)    | 
`class `[``scy::Process``](#classscy_1_1Process)    | 
`class `[``scy::Queue``](#classscy_1_1Queue)    | Implements a thread-safe queue container.
`class `[``scy::RunnableQueue``](#classscy_1_1RunnableQueue)    | 
`class `[``scy::SyncQueue``](#classscy_1_1SyncQueue)    | 
`class `[``scy::AsyncQueue``](#classscy_1_1AsyncQueue)    | 
`class `[``scy::Random``](#classscy_1_1Random)    | 
`class `[``scy::StopPropagation``](#classscy_1_1StopPropagation)    | This exception is used to break out of a [Signal](#classscy_1_1Signal) callback scope.
`class `[``scy::SignalBase``](#classscy_1_1SignalBase)    | 
`class `[``scy::NullSignal``](#classscy_1_1NullSignal)    | 
`class `[``scy::Signal``](#classscy_1_1Signal)    | 
`class `[``scy::Signal2``](#classscy_1_1Signal2)    | 
`class `[``scy::Signal3``](#classscy_1_1Signal3)    | 
`class `[``scy::Signal4``](#classscy_1_1Signal4)    | 
`class `[``scy::Singleton``](#classscy_1_1Singleton)    | 
`class `[``scy::State``](#classscy_1_1State)    | 
`class `[``scy::MutexState``](#classscy_1_1MutexState)    | 
`class `[``scy::StateSignal``](#classscy_1_1StateSignal)    | 
`class `[``scy::Stateful``](#classscy_1_1Stateful)    | 
`class `[``scy::Stream``](#classscy_1_1Stream)    | 
`class `[``scy::SyncContext``](#classscy_1_1SyncContext)    | 
`class `[``scy::Task``](#classscy_1_1Task)    | 
`class `[``scy::TaskRunner``](#classscy_1_1TaskRunner)    | 
`class `[``scy::test::Test``](#classscy_1_1test_1_1Test)    | 
`class `[``scy::test::FunctionTest``](#classscy_1_1test_1_1FunctionTest)    | 
`class `[``scy::test::TestRunner``](#classscy_1_1test_1_1TestRunner)    | 
`class `[``scy::Thread``](#classscy_1_1Thread)    | 
`class `[``scy::AsyncStartable``](#classscy_1_1AsyncStartable)    | 
`class `[``scy::Timer``](#classscy_1_1Timer)    | 
`class `[``scy::TZInfo``](#classscy_1_1TZInfo)    | 
`struct `[``scy::OptionParser``](#structscy_1_1OptionParser)    | 
`struct `[``scy::async::Runner::Context``](#structscy_1_1async_1_1Runner_1_1Context)    | 
`struct `[``scy::base64::Encoder``](#structscy_1_1base64_1_1Encoder)    | 
`struct `[``scy::base64::Decoder``](#structscy_1_1base64_1_1Decoder)    | 
`struct `[``scy::Bitwise``](#structscy_1_1Bitwise)    | 
`struct `[``scy::NVCollection::ILT``](#structscy_1_1NVCollection_1_1ILT)    | 
`struct `[``scy::DelegateCallback``](#structscy_1_1DelegateCallback)    | 
`struct `[``scy::DelegateCallback< C, 0, true >``](#structscy_1_1DelegateCallback_3_01C_00_010_00_01true_01_4)    | 
`struct `[``scy::DelegateCallback< C, 0, false >``](#structscy_1_1DelegateCallback_3_01C_00_010_00_01false_01_4)    | 
`struct `[``scy::DelegateCallback< C, 1, true, P >``](#structscy_1_1DelegateCallback_3_01C_00_011_00_01true_00_01P_01_4)    | 
`struct `[``scy::DelegateCallback< C, 1, false, P >``](#structscy_1_1DelegateCallback_3_01C_00_011_00_01false_00_01P_01_4)    | 
`struct `[``scy::DelegateCallback< C, 2, true, P, P2 >``](#structscy_1_1DelegateCallback_3_01C_00_012_00_01true_00_01P_00_01P2_01_4)    | 
`struct `[``scy::DelegateCallback< C, 2, false, P, P2 >``](#structscy_1_1DelegateCallback_3_01C_00_012_00_01false_00_01P_00_01P2_01_4)    | 
`struct `[``scy::DelegateCallback< C, 3, true, P, P2, P3 >``](#structscy_1_1DelegateCallback_3_01C_00_013_00_01true_00_01P_00_01P2_00_01P3_01_4)    | 
`struct `[``scy::DelegateCallback< C, 3, false, P, P2, P3 >``](#structscy_1_1DelegateCallback_3_01C_00_013_00_01false_00_01P_00_01P2_00_01P3_01_4)    | 
`struct `[``scy::DelegateCallback< C, 4, true, P, P2, P3, P4 >``](#structscy_1_1DelegateCallback_3_01C_00_014_00_01true_00_01P_00_01P2_00_01P3_00_01P4_01_4)    | 
`struct `[``scy::DelegateCallback< C, 4, false, P, P2, P3, P4 >``](#structscy_1_1DelegateCallback_3_01C_00_014_00_01false_00_01P_00_01P2_00_01P3_00_01P4_01_4)    | 
`struct `[``scy::DelegateBase``](#structscy_1_1DelegateBase)    | 
`struct `[``scy::Error``](#structscy_1_1Error)    | 
`struct `[``scy::hex::Encoder``](#structscy_1_1hex_1_1Encoder)    | 
`struct `[``scy::hex::Decoder``](#structscy_1_1hex_1_1Decoder)    | 
`struct `[``scy::ipc::Action``](#structscy_1_1ipc_1_1Action)    | Default action type for executing synchronized callbacks.
`struct `[``scy::LogStream``](#structscy_1_1LogStream)    | 
`struct `[``scy::deleter::Deferred``](#structscy_1_1deleter_1_1Deferred)    | 
`struct `[``scy::deleter::Dispose``](#structscy_1_1deleter_1_1Dispose)    | 
`struct `[``scy::deleter::Array``](#structscy_1_1deleter_1_1Array)    | 
`struct `[``scy::IPacketInfo``](#structscy_1_1IPacketInfo)    | 
`struct `[``scy::PacketCreationStrategy``](#structscy_1_1PacketCreationStrategy)    | 
`struct `[``scy::PacketAdapterReference``](#structscy_1_1PacketAdapterReference)    | For 0.8.x compatibility.
`struct `[``scy::PacketStreamState``](#structscy_1_1PacketStreamState)    | 
`struct `[``scy::TransactionState``](#structscy_1_1TransactionState)    | 
`struct `[``scy::DelegateCallback< C, 8, false, PolymorphicT >``](#structscy_1_1DelegateCallback_3_01C_00_018_00_01false_00_01PolymorphicT_01_4)    | 
`struct `[``scy::DelegateCallback< C, 8, true, PolymorphicT >``](#structscy_1_1DelegateCallback_3_01C_00_018_00_01true_00_01PolymorphicT_01_4)    | 
`struct `[``scy::SharedLibrary``](#structscy_1_1SharedLibrary)    | 
`struct `[``scy::util::Version``](#structscy_1_1util_1_1Version)    | 
# namespace `scy`



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[``scy::AbstractCollection``](#classscy_1_1AbstractCollection)    | 
`class `[``scy::Application``](#classscy_1_1Application)    | 
`class `[``scy::AsyncLogWriter``](#classscy_1_1AsyncLogWriter)    | 
`class `[``scy::AsyncPacketQueue``](#classscy_1_1AsyncPacketQueue)    | 
`class `[``scy::AsyncQueue``](#classscy_1_1AsyncQueue)    | 
`class `[``scy::AsyncStartable``](#classscy_1_1AsyncStartable)    | 
`class `[``scy::BitReader``](#classscy_1_1BitReader)    | A [BitReader](#classscy_1_1BitReader) for reading binary streams.
`class `[``scy::BitWriter``](#classscy_1_1BitWriter)    | 
`class `[``scy::Configuration``](#classscy_1_1Configuration)    | 
`class `[``scy::ConsoleChannel``](#classscy_1_1ConsoleChannel)    | 
`class `[``scy::ConstBuffer``](#classscy_1_1ConstBuffer)    | 
`class `[``scy::DateTime``](#classscy_1_1DateTime)    | 
`class `[``scy::DateTimeFormat``](#classscy_1_1DateTimeFormat)    | 
`class `[``scy::DateTimeFormatter``](#classscy_1_1DateTimeFormatter)    | 
`class `[``scy::DateTimeParser``](#classscy_1_1DateTimeParser)    | 
`class `[``scy::Delegate``](#classscy_1_1Delegate)    | 
`class `[``scy::FileChannel``](#classscy_1_1FileChannel)    | 
`class `[``scy::FlagPacket``](#classscy_1_1FlagPacket)    | A simple flag packet for sending state flags along the packet stream.
`class `[``scy::GarbageCollector``](#classscy_1_1GarbageCollector)    | Simple garbage collector for deferred pointer deletion.
`class `[``scy::Idler``](#classscy_1_1Idler)    | 
`class `[``scy::IPacket``](#classscy_1_1IPacket)    | 
`class `[``scy::IPacketCreationStrategy``](#classscy_1_1IPacketCreationStrategy)    | 
`class `[``scy::KVCollection``](#classscy_1_1KVCollection)    | A reusable stack based unique key-value store for DRY coding.
`class `[``scy::LiveCollection``](#classscy_1_1LiveCollection)    | 
`class `[``scy::LocalDateTime``](#classscy_1_1LocalDateTime)    | 
`class `[``scy::LogChannel``](#classscy_1_1LogChannel)    | 
`class `[``scy::Logger``](#classscy_1_1Logger)    | 
`class `[``scy::LogWriter``](#classscy_1_1LogWriter)    | 
`class `[``scy::MutableBuffer``](#classscy_1_1MutableBuffer)    | 
`class `[``scy::Mutex``](#classscy_1_1Mutex)    | 
`class `[``scy::MutexState``](#classscy_1_1MutexState)    | 
`class `[``scy::NullSignal``](#classscy_1_1NullSignal)    | 
`class `[``scy::NVCollection``](#classscy_1_1NVCollection)    | 
`class `[``scy::PacketFactory``](#classscy_1_1PacketFactory)    | 
`class `[``scy::PacketProcessor``](#classscy_1_1PacketProcessor)    | For 0.8.x compatibility.
`class `[``scy::PacketStream``](#classscy_1_1PacketStream)    | 
`class `[``scy::PacketStreamAdapter``](#classscy_1_1PacketStreamAdapter)    | 
`class `[``scy::PacketTransaction``](#classscy_1_1PacketTransaction)    | 
`class `[``scy::PointerCollection``](#classscy_1_1PointerCollection)    | 
`class `[``scy::Process``](#classscy_1_1Process)    | 
`class `[``scy::Queue``](#classscy_1_1Queue)    | Implements a thread-safe queue container.
`class `[``scy::Random``](#classscy_1_1Random)    | 
`class `[``scy::RawPacket``](#classscy_1_1RawPacket)    | 
`class `[``scy::RotatingFileChannel``](#classscy_1_1RotatingFileChannel)    | 
`class `[``scy::RunnableQueue``](#classscy_1_1RunnableQueue)    | 
`class `[``scy::ScopedConfiguration``](#classscy_1_1ScopedConfiguration)    | 
`class `[``scy::ScopedLock``](#classscy_1_1ScopedLock)    | 
`class `[``scy::ScopedPointer``](#classscy_1_1ScopedPointer)    | Scoped Pointer Classes.
`class `[``scy::ScopedRawPointer``](#classscy_1_1ScopedRawPointer)    | 
`class `[``scy::ScopedSharedPointer``](#classscy_1_1ScopedSharedPointer)    | 
`class `[``scy::SharedObject``](#classscy_1_1SharedObject)    | 
`class `[``scy::Signal``](#classscy_1_1Signal)    | 
`class `[``scy::Signal2``](#classscy_1_1Signal2)    | 
`class `[``scy::Signal3``](#classscy_1_1Signal3)    | 
`class `[``scy::Signal4``](#classscy_1_1Signal4)    | 
`class `[``scy::SignalBase``](#classscy_1_1SignalBase)    | 
`class `[``scy::Singleton``](#classscy_1_1Singleton)    | 
`class `[``scy::State``](#classscy_1_1State)    | 
`class `[``scy::Stateful``](#classscy_1_1Stateful)    | 
`class `[``scy::StateSignal``](#classscy_1_1StateSignal)    | 
`class `[``scy::StopPropagation``](#classscy_1_1StopPropagation)    | This exception is used to break out of a [Signal](#classscy_1_1Signal) callback scope.
`class `[``scy::Stopwatch``](#classscy_1_1Stopwatch)    | 
`class `[``scy::Stream``](#classscy_1_1Stream)    | 
`class `[``scy::StreamWriter``](#classscy_1_1StreamWriter)    | 
`class `[``scy::SyncContext``](#classscy_1_1SyncContext)    | 
`class `[``scy::SyncPacketQueue``](#classscy_1_1SyncPacketQueue)    | 
`class `[``scy::SyncQueue``](#classscy_1_1SyncQueue)    | 
`class `[``scy::Task``](#classscy_1_1Task)    | 
`class `[``scy::TaskRunner``](#classscy_1_1TaskRunner)    | 
`class `[``scy::Thread``](#classscy_1_1Thread)    | 
`class `[``scy::ThreadedStreamReader``](#classscy_1_1ThreadedStreamReader)    | 
`class `[``scy::TimedToken``](#classscy_1_1TimedToken)    | A token that expires after the specified duration.
`class `[``scy::Timeout``](#classscy_1_1Timeout)    | Simple millisecond timeout counter which expires after a given delay.
`class `[``scy::Timer``](#classscy_1_1Timer)    | 
`class `[``scy::Timespan``](#classscy_1_1Timespan)    | A class that represents time spans up to microsecond resolution.
`class `[``scy::Timestamp``](#classscy_1_1Timestamp)    | 
`class `[``scy::Timezone``](#classscy_1_1Timezone)    | This class provides information about the current timezone.
`class `[``scy::TZInfo``](#classscy_1_1TZInfo)    | 
`struct `[``scy::Bitwise``](#structscy_1_1Bitwise)    | 
`struct `[``scy::DelegateBase``](#structscy_1_1DelegateBase)    | 
`struct `[``scy::DelegateCallback``](#structscy_1_1DelegateCallback)    | 
`struct `[``scy::DelegateCallback< C, 0, false >``](#structscy_1_1DelegateCallback_3_01C_00_010_00_01false_01_4)    | 
`struct `[``scy::DelegateCallback< C, 0, true >``](#structscy_1_1DelegateCallback_3_01C_00_010_00_01true_01_4)    | 
`struct `[``scy::DelegateCallback< C, 1, false, P >``](#structscy_1_1DelegateCallback_3_01C_00_011_00_01false_00_01P_01_4)    | 
`struct `[``scy::DelegateCallback< C, 1, true, P >``](#structscy_1_1DelegateCallback_3_01C_00_011_00_01true_00_01P_01_4)    | 
`struct `[``scy::DelegateCallback< C, 2, false, P, P2 >``](#structscy_1_1DelegateCallback_3_01C_00_012_00_01false_00_01P_00_01P2_01_4)    | 
`struct `[``scy::DelegateCallback< C, 2, true, P, P2 >``](#structscy_1_1DelegateCallback_3_01C_00_012_00_01true_00_01P_00_01P2_01_4)    | 
`struct `[``scy::DelegateCallback< C, 3, false, P, P2, P3 >``](#structscy_1_1DelegateCallback_3_01C_00_013_00_01false_00_01P_00_01P2_00_01P3_01_4)    | 
`struct `[``scy::DelegateCallback< C, 3, true, P, P2, P3 >``](#structscy_1_1DelegateCallback_3_01C_00_013_00_01true_00_01P_00_01P2_00_01P3_01_4)    | 
`struct `[``scy::DelegateCallback< C, 4, false, P, P2, P3, P4 >``](#structscy_1_1DelegateCallback_3_01C_00_014_00_01false_00_01P_00_01P2_00_01P3_00_01P4_01_4)    | 
`struct `[``scy::DelegateCallback< C, 4, true, P, P2, P3, P4 >``](#structscy_1_1DelegateCallback_3_01C_00_014_00_01true_00_01P_00_01P2_00_01P3_00_01P4_01_4)    | 
`struct `[``scy::DelegateCallback< C, 8, false, PolymorphicT >``](#structscy_1_1DelegateCallback_3_01C_00_018_00_01false_00_01PolymorphicT_01_4)    | 
`struct `[``scy::DelegateCallback< C, 8, true, PolymorphicT >``](#structscy_1_1DelegateCallback_3_01C_00_018_00_01true_00_01PolymorphicT_01_4)    | 
`struct `[``scy::Error``](#structscy_1_1Error)    | 
`struct `[``scy::IPacketInfo``](#structscy_1_1IPacketInfo)    | 
`struct `[``scy::LogStream``](#structscy_1_1LogStream)    | 
`struct `[``scy::OptionParser``](#structscy_1_1OptionParser)    | 
`struct `[``scy::PacketAdapterReference``](#structscy_1_1PacketAdapterReference)    | For 0.8.x compatibility.
`struct `[``scy::PacketCreationStrategy``](#structscy_1_1PacketCreationStrategy)    | 
`struct `[``scy::PacketStreamState``](#structscy_1_1PacketStreamState)    | 
`struct `[``scy::SharedLibrary``](#structscy_1_1SharedLibrary)    | 
`struct `[``scy::TransactionState``](#structscy_1_1TransactionState)    | 
# class `scy::AbstractCollection` 




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

#### `public inline  AbstractCollection()` 





#### `public inline virtual  ~AbstractCollection()` 





#### `public bool add(const TKey & key,TValue * item,bool whiny)` 





#### `public bool remove(const TValue * item)` 





#### `public TValue * remove(const TKey & key)` 





#### `public bool exists(const TKey & key) const` 





#### `public bool exists(const TValue * item) const` 





#### `public bool free(const TKey & key)` 





#### `public bool empty() const` 





#### `public int size() const` 





#### `public TValue * get(const TKey & key,bool whiny) const` 





#### `public void clear()` 





# class `scy::Application` 




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

#### `public uv::Loop * loop` 



The active event loop. May be assigned at construction, otherwise the default event loop is used.

#### `public  Application(uv::Loop * loop)` 





#### `public  ~Application()` 





#### `public void run()` 





#### `public void stop()` 





#### `public void finalize()` 





#### `public void bindShutdownSignal(std::function< void(void *)> callback,void * opaque)` 

Bind the shutdown signal.

Shutdown handling

#### `public void waitForShutdown(std::function< void(void *)> callback,void * opaque)` 

Bind the shutdown signal and run the main event loop.



#### `protected  Application(const `[`Application`](#classscy_1_1Application)` &)` 





#### `protected  Application(`[`Application`](#classscy_1_1Application)` &&)` 





#### `protected `[`Application`](#classscy_1_1Application)` & operator=(const `[`Application`](#classscy_1_1Application)` &)` 





#### `protected `[`Application`](#classscy_1_1Application)` & operator=(`[`Application`](#classscy_1_1Application)` &&)` 





# class `scy::AsyncLogWriter` 

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
`protected `[`Thread`](#classscy_1_1Thread)` _thread` | 
`protected std::deque< `[`LogStream`](#structscy_1_1LogStream)` * > _pending` | 
`protected mutable `[`Mutex`](#classscy_1_1Mutex)` _mutex` | 
`protected bool writeNext()` | 

## Members

#### `public  AsyncLogWriter()` 





#### `public virtual  ~AsyncLogWriter()` 





#### `public virtual void write(`[`LogStream`](#structscy_1_1LogStream)` * stream)` 

Queues the given log message stream.



#### `public void flush()` 

Flushes queued messages.



#### `public virtual void run()` 

Writes queued messages asynchronously.



#### `public void clear()` 

Clears all queued messages.



#### `protected `[`Thread`](#classscy_1_1Thread)` _thread` 





#### `protected std::deque< `[`LogStream`](#structscy_1_1LogStream)` * > _pending` 





#### `protected mutable `[`Mutex`](#classscy_1_1Mutex)` _mutex` 





#### `protected bool writeNext()` 





# class `scy::AsyncPacketQueue` 

```
class scy::AsyncPacketQueue
  : public scy::AsyncQueue< T >
  : public scy::PacketProcessor
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`PacketSignal`](#classscy_1_1SignalBase)` emitter` | 
`public inline  AsyncPacketQueue(int maxSize)` | 
`public inline virtual  ~AsyncPacketQueue()` | 
`public inline virtual void close()` | 
`public inline virtual void process(`[`IPacket`](#classscy_1_1IPacket)` & packet)` | 
`public inline virtual bool accepts(`[`IPacket`](#classscy_1_1IPacket)` &)` | 
`protected inline virtual void dispatch(T & packet)` | 
`protected inline virtual void onStreamStateChange(const `[`PacketStreamState`](#structscy_1_1PacketStreamState)` &)` | 

## Members

#### `public `[`PacketSignal`](#classscy_1_1SignalBase)` emitter` 





#### `public inline  AsyncPacketQueue(int maxSize)` 





#### `public inline virtual  ~AsyncPacketQueue()` 





#### `public inline virtual void close()` 





#### `public inline virtual void process(`[`IPacket`](#classscy_1_1IPacket)` & packet)` 



This method performs processing on the given packet and emits the result.

Note: If packet processing is async (the packet is not in the current thread scope) then packet data must be copied. Copied data can be freed directly aFter the async call to emit() the outgoing packet.

#### `public inline virtual bool accepts(`[`IPacket`](#classscy_1_1IPacket)` &)` 



This method ensures compatibility with the given packet type. Return false to reject the packet.

#### `protected inline virtual void dispatch(T & packet)` 





#### `protected inline virtual void onStreamStateChange(const `[`PacketStreamState`](#structscy_1_1PacketStreamState)` &)` 



Called by the [PacketStream](#classscy_1_1PacketStream) to notify when the internal [Stream](#classscy_1_1Stream) state changes. On receiving the Stopped state, it is the responsibility of the adapter to have ceased all outgoing packet transmission, especially in multi-thread scenarios.

# class `scy::AsyncQueue` 

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
`protected `[`Thread`](#classscy_1_1Thread)` _thread` | 
`protected inline virtual  ~AsyncQueue()` | 

## Members

#### `public inline  AsyncQueue(int limit)` 





#### `public inline virtual void cancel()` 





#### `protected `[`Thread`](#classscy_1_1Thread)` _thread` 





#### `protected inline virtual  ~AsyncQueue()` 





# class `scy::AsyncStartable` 

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
`protected `[`Thread`](#classscy_1_1Thread)` _thread` | 

## Members

#### `public inline  AsyncStartable()` 





#### `public inline virtual  ~AsyncStartable()` 





#### `public inline virtual bool start()` 





#### `public inline virtual void stop()` 





#### `protected `[`Thread`](#classscy_1_1Thread)` _thread` 





# class `scy::BitReader` 


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

#### `public  BitReader(const char * bytes,std::size_t size,`[`ByteOrder`](#group__base_1gaa934c0972c743983462716b01df7dc32)` order)` 





#### `public  BitReader(const Buffer & buf,`[`ByteOrder`](#group__base_1gaa934c0972c743983462716b01df7dc32)` order)` 





#### `public  BitReader(const `[`ConstBuffer`](#classscy_1_1ConstBuffer)` & pod,`[`ByteOrder`](#group__base_1gaa934c0972c743983462716b01df7dc32)` order)` 





#### `public  ~BitReader()` 





#### `public void get(char * val,std::size_t len)` 



Reads a value from the [BitReader](#classscy_1_1BitReader). Returns false if there isn't enough data left for the specified type. Throws a std::out_of_range exception if reading past the limit.

#### `public void get(std::string & val,std::size_t len)` 





#### `public void getU8(std::uint8_t & val)` 





#### `public void getU16(std::uint16_t & val)` 





#### `public void getU24(std::uint32_t & val)` 





#### `public void getU32(std::uint32_t & val)` 





#### `public void getU64(std::uint64_t & val)` 





#### `public const char peek()` 





#### `public const std::uint8_t peekU8()` 





#### `public const std::uint16_t peekU16()` 





#### `public const std::uint32_t peekU24()` 





#### `public const std::uint32_t peekU32()` 





#### `public const std::uint64_t peekU64()` 



Peeks data from the [BitReader](#classscy_1_1BitReader). -1 is returned if reading past boundary.

#### `public int skipToChar(char c)` 

String parsing methods.



#### `public int skipWhitespace()` 





#### `public int skipToNextLine()` 





#### `public int skipNextWord()` 





#### `public int readNextWord(std::string & val)` 





#### `public int readNextNumber(unsigned int & val)` 





#### `public int readLine(std::string & val)` 





#### `public int readToNext(std::string & val,char c)` 





#### `public void seek(std::size_t val)` 



Set position pointer to absolute position. Throws a std::out_of_range exception if the value exceeds the limit.

#### `public void skip(std::size_t size)` 



Set position pointer to relative position. Throws a std::out_of_range exception if the value exceeds the limit.

#### `public std::size_t limit() const` 

Returns the read limit.



#### `public inline std::size_t position() const` 

Returns the current read position.



#### `public std::size_t available() const` 

Returns the number of elements between the current position and the limit.



#### `public inline const char * begin() const` 





#### `public inline const char * current() const` 





#### `public inline `[`ByteOrder`](#group__base_1gaa934c0972c743983462716b01df7dc32)` order() const` 





#### `public std::string toString()` 





# class `scy::BitWriter` 




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

#### `public  BitWriter(char * bytes,std::size_t size,`[`ByteOrder`](#group__base_1gaa934c0972c743983462716b01df7dc32)` order)` 





#### `public  BitWriter(Buffer & buf,`[`ByteOrder`](#group__base_1gaa934c0972c743983462716b01df7dc32)` order)` 





#### `public  BitWriter(`[`MutableBuffer`](#classscy_1_1MutableBuffer)` & pod,`[`ByteOrder`](#group__base_1gaa934c0972c743983462716b01df7dc32)` order)` 





#### `public  ~BitWriter()` 





#### `public void put(const char * val,std::size_t len)` 



Append bytes to the buffer. Throws a std::out_of_range exception if reading past the limit.

#### `public void put(const std::string & val)` 





#### `public void putU8(std::uint8_t val)` 





#### `public void putU16(std::uint16_t val)` 





#### `public void putU24(std::uint32_t val)` 





#### `public void putU32(std::uint32_t val)` 





#### `public void putU64(std::uint64_t val)` 





#### `public bool update(const char * val,std::size_t len,std::size_t pos)` 



Update a byte range. Throws a std::out_of_range exception if reading past the limit.

#### `public bool update(const std::string & val,std::size_t pos)` 





#### `public bool updateU8(std::uint8_t val,std::size_t pos)` 





#### `public bool updateU16(std::uint16_t val,std::size_t pos)` 





#### `public bool updateU24(std::uint32_t val,std::size_t pos)` 





#### `public bool updateU32(std::uint32_t val,std::size_t pos)` 





#### `public bool updateU64(std::uint64_t val,std::size_t pos)` 





#### `public void seek(std::size_t val)` 



Set position pointer to absolute position. Throws a std::out_of_range exception if the value exceeds the limit.

#### `public void skip(std::size_t size)` 



Set position pointer to relative position. Throws a std::out_of_range exception if the value exceeds the limit.

#### `public std::size_t limit() const` 

Returns the write limit.



#### `public inline std::size_t position() const` 

Returns the current write position.



#### `public std::size_t available() const` 

Returns the number of elements between the current write position and the limit.



#### `public inline char * begin()` 





#### `public inline char * current()` 





#### `public inline const char * begin() const` 





#### `public inline const char * current() const` 





#### `public inline `[`ByteOrder`](#group__base_1gaa934c0972c743983462716b01df7dc32)` order() const` 

Returns written bytes as a string.



#### `public std::string toString()` 





# class `scy::Configuration` 




[Configuration](#classscy_1_1Configuration) is an abstract base class for managing /// different kinds of configuration storage back ends such as /// JSON, XML, or database.

Subclasses must override the [getRaw()](#group__base_1ga1246d954823215df6d932e78a08bc23b) and [setRaw()](#group__base_1gac6a4023c271c2357334275f15329310d) and methods.

This class is safe for multithreaded use.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`Signal2`](#classscy_1_1Signal2)`< const std::string &, const std::string & > PropertyChanged` | The Key and Value of the changed configuration property.
`public  Configuration()` | Creates the [Configuration](#classscy_1_1Configuration).
`public virtual  ~Configuration()` | Destroys the [Configuration](#classscy_1_1Configuration).
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

#### `public `[`Signal2`](#classscy_1_1Signal2)`< const std::string &, const std::string & > PropertyChanged` 

The Key and Value of the changed configuration property.



#### `public  Configuration()` 

Creates the [Configuration](#classscy_1_1Configuration).



#### `public virtual  ~Configuration()` 

Destroys the [Configuration](#classscy_1_1Configuration).



#### `public bool exists(const std::string & key) const` 

Returns true if the property with the given key exists.



#### `public std::string getString(const std::string & key) const` 



Returns the string value of the property with the given name. Throws a NotFoundException if the key does not exist.

#### `public std::string getString(const std::string & key,const std::string & defaultValue) const` 



If a property with the given key exists, returns the property's string value, otherwise returns the given default value.

#### `public std::string getRawString(const std::string & key) const` 



Returns the raw string value of the property with the given name. Throws a NotFoundException if the key does not exist. References to other properties are not expanded.

#### `public std::string getRawString(const std::string & key,const std::string & defaultValue) const` 



If a property with the given key exists, returns the property's raw string value, otherwise returns the given default value. References to other properties are not expanded.

#### `public int getInt(const std::string & key) const` 



Returns the int value of the property with the given name. Throws a NotFoundException if the key does not exist. Throws a SyntaxException if the property can not be converted to an int. Numbers starting with 0x are treated as hexadecimal.

#### `public int getInt(const std::string & key,int defaultValue) const` 



If a property with the given key exists, returns the property's int value, otherwise returns the given default value. Throws a SyntaxException if the property can not be converted to an int. Numbers starting with 0x are treated as hexadecimal.

#### `public std::int64_t getLargeInt(const std::string & key) const` 



Returns the int value of the property with the given name. Throws a NotFoundException if the key does not exist. Throws a SyntaxException if the property can not be converted to an int. Numbers starting with 0x are treated as hexadecimal.

#### `public std::int64_t getLargeInt(const std::string & key,std::int64_t defaultValue) const` 



If a property with the given key exists, returns the property's int value, otherwise returns the given default value. Throws a SyntaxException if the property can not be converted to an int. Numbers starting with 0x are treated as hexadecimal.

#### `public double getDouble(const std::string & key) const` 



Returns the double value of the property with the given name. Throws a NotFoundException if the key does not exist. Throws a SyntaxException if the property can not be converted to a double.

#### `public double getDouble(const std::string & key,double defaultValue) const` 



If a property with the given key exists, returns the property's double value, otherwise returns the given default value. Throws a SyntaxException if the property can not be converted to an double.

#### `public bool getBool(const std::string & key) const` 



Returns the double value of the property with the given name. Throws a NotFoundException if the key does not exist. Throws a SyntaxException if the property can not be converted to a double.

#### `public bool getBool(const std::string & key,bool defaultValue) const` 



If a property with the given key exists, returns the property's bool value, otherwise returns the given default value. Throws a SyntaxException if the property can not be converted to a boolean. The following string values can be converted into a boolean:

* numerical values: non zero becomes true, zero becomes false


* strings: true, yes, on become true, false, no, off become false Case does not matter.

#### `public void setString(const std::string & key,const std::string & value)` 



Sets the property with the given key to the given value. An already existing value for the key is overwritten.

#### `public void setInt(const std::string & key,int value)` 



Sets the property with the given key to the given value. An already existing value for the key is overwritten.

#### `public void setLargeInt(const std::string & key,std::int64_t value)` 



Sets the property with the given key to the given value. An already existing value for the key is overwritten.

#### `public void setDouble(const std::string & key,double value)` 



Sets the property with the given key to the given value. An already existing value for the key is overwritten.

#### `public void setBool(const std::string & key,bool value)` 



Sets the property with the given key to the given value. An already existing value for the key is overwritten.

#### `protected bool getRaw(const std::string & key,std::string & value) const` 



If the property with the given key exists, stores the property's value in value and returns true. Otherwise, returns false.

Must be overridden by subclasses.

#### `protected void setRaw(const std::string & key,const std::string & value)` 



Sets the property with the given key to the given value. An already existing value for the key is overwritten.

The implementation is responsible for emitting the PropertyChanged signal.

# class `scy::ConsoleChannel` 

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

#### `public  ConsoleChannel(const std::string & name,LogLevel level,const std::string & timeFormat)` 





#### `public inline virtual  ~ConsoleChannel()` 





#### `public virtual void write(const `[`LogStream`](#structscy_1_1LogStream)` & stream)` 





# class `scy::ConstBuffer` 




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

#### `public inline  ConstBuffer()` 

Construct an empty buffer.



#### `public inline  ConstBuffer(const void * data,std::size_t size)` 

Construct a buffer to represent the given memory range.



#### `public inline  ConstBuffer(const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & b)` 

Construct a non-modifiable buffer from a modifiable one.



#### `public inline const void * data() const` 





#### `public inline std::size_t size() const` 





#### `public inline const char * cstr() const` 

String methods.



#### `public inline std::string str() const` 





# class `scy::DateTime` 




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
`public  DateTime()` | Creates a [DateTime](#classscy_1_1DateTime) for the current date and time.
`public  DateTime(const `[`Timestamp`](#classscy_1_1Timestamp)` & timestamp)` | 
`public  DateTime(int year,int month,int day,int hour,int minute,int second,int millisecond,int microsecond)` | 
`public  DateTime(double julianDay)` | Creates a [DateTime](#classscy_1_1DateTime) for the given Julian day.
`public  DateTime(`[`Timestamp::UtcTimeVal`](#group__base_1gaec8ace0f25db0ce91c10b30270c3dc24)` utcTime,`[`Timestamp::TimeDiff`](#group__base_1ga9f06831e4e9b9cbc6f061cc61dc91fd0)` diff)` | 
`public  DateTime(const `[`DateTime`](#classscy_1_1DateTime)` & dateTime)` | Copy constructor. Creates the [DateTime](#classscy_1_1DateTime) from another one.
`public  ~DateTime()` | Destroys the [DateTime](#classscy_1_1DateTime).
`public `[`DateTime`](#classscy_1_1DateTime)` & operator=(const `[`DateTime`](#classscy_1_1DateTime)` & dateTime)` | Assigns another [DateTime](#classscy_1_1DateTime).
`public `[`DateTime`](#classscy_1_1DateTime)` & operator=(const `[`Timestamp`](#classscy_1_1Timestamp)` & timestamp)` | Assigns a [Timestamp](#classscy_1_1Timestamp).
`public `[`DateTime`](#classscy_1_1DateTime)` & operator=(double julianDay)` | Assigns a Julian day.
`public `[`DateTime`](#classscy_1_1DateTime)` & assign(int year,int month,int day,int hour,int minute,int second,int millisecond,int microseconds)` | 
`public void swap(`[`DateTime`](#classscy_1_1DateTime)` & dateTime)` | Swaps the [DateTime](#classscy_1_1DateTime) with another one.
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
`public inline `[`Timestamp`](#classscy_1_1Timestamp)` timestamp() const` | Returns the date and time expressed as a [Timestamp](#classscy_1_1Timestamp).
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

#### `public  DateTime()` 

Creates a [DateTime](#classscy_1_1DateTime) for the current date and time.



#### `public  DateTime(const `[`Timestamp`](#classscy_1_1Timestamp)` & timestamp)` 



Creates a [DateTime](#classscy_1_1DateTime) for the date and time given in a [Timestamp](#classscy_1_1Timestamp).

#### `public  DateTime(int year,int month,int day,int hour,int minute,int second,int millisecond,int microsecond)` 



Creates a [DateTime](#classscy_1_1DateTime) for the given Gregorian date and time.

* year is from 0 to 9999.


* month is from 1 to 12.


* day is from 1 to 31.


* hour is from 0 to 23.


* minute is from 0 to 59.


* second is from 0 to 59.


* millisecond is from 0 to 999.


* microsecond is from 0 to 999.

#### `public  DateTime(double julianDay)` 

Creates a [DateTime](#classscy_1_1DateTime) for the given Julian day.



#### `public  DateTime(`[`Timestamp::UtcTimeVal`](#group__base_1gaec8ace0f25db0ce91c10b30270c3dc24)` utcTime,`[`Timestamp::TimeDiff`](#group__base_1ga9f06831e4e9b9cbc6f061cc61dc91fd0)` diff)` 



Creates a [DateTime](#classscy_1_1DateTime) from an UtcTimeVal and a TimeDiff.

Mainly used internally by [DateTime](#classscy_1_1DateTime) and friends.

#### `public  DateTime(const `[`DateTime`](#classscy_1_1DateTime)` & dateTime)` 

Copy constructor. Creates the [DateTime](#classscy_1_1DateTime) from another one.



#### `public  ~DateTime()` 

Destroys the [DateTime](#classscy_1_1DateTime).



#### `public `[`DateTime`](#classscy_1_1DateTime)` & operator=(const `[`DateTime`](#classscy_1_1DateTime)` & dateTime)` 

Assigns another [DateTime](#classscy_1_1DateTime).



#### `public `[`DateTime`](#classscy_1_1DateTime)` & operator=(const `[`Timestamp`](#classscy_1_1Timestamp)` & timestamp)` 

Assigns a [Timestamp](#classscy_1_1Timestamp).



#### `public `[`DateTime`](#classscy_1_1DateTime)` & operator=(double julianDay)` 

Assigns a Julian day.



#### `public `[`DateTime`](#classscy_1_1DateTime)` & assign(int year,int month,int day,int hour,int minute,int second,int millisecond,int microseconds)` 



Assigns a Gregorian date and time.

* year is from 0 to 9999.


* month is from 1 to 12.


* day is from 1 to 31.


* hour is from 0 to 23.


* minute is from 0 to 59.


* second is from 0 to 59.


* millisecond is from 0 to 999.


* microsecond is from 0 to 999.

#### `public void swap(`[`DateTime`](#classscy_1_1DateTime)` & dateTime)` 

Swaps the [DateTime](#classscy_1_1DateTime) with another one.



#### `public inline int year() const` 

Returns the year.



#### `public inline int month() const` 

Returns the month (1 to 12).



#### `public int week(int firstDayOfWeek) const` 



Returns the week number within the year. FirstDayOfWeek should be either SUNDAY (0) or MONDAY (1). The returned week number will be from 0 to 53. Week number 1 is the week containing January 4. This is in accordance to ISO 8601.

The following example assumes that firstDayOfWeek is MONDAY. For 2005, which started on a Saturday, week 1 will be the week starting on Monday, January 3. January 1 and 2 will fall within week 0 (or the last week of the previous year).

For 2007, which starts on a Monday, week 1 will be the week startung on Monday, January 1. There will be no week 0 in 2007.

#### `public inline int day() const` 

Returns the day witin the month (1 to 31).



#### `public int dayOfWeek() const` 



Returns the weekday (0 to 6, where 0 = Sunday, 1 = Monday, ..., 6 = Saturday).

#### `public int dayOfYear() const` 



Returns the number of the day in the year. January 1 is 1, February 1 is 32, etc.

#### `public inline int hour() const` 

Returns the hour (0 to 23).



#### `public inline int hourAMPM() const` 

Returns the hour (0 to 12).



#### `public inline bool isAM() const` 

Returns true if hour < 12;.



#### `public inline bool isPM() const` 

Returns true if hour >= 12.



#### `public inline int minute() const` 

Returns the minute (0 to 59).



#### `public inline int second() const` 

Returns the second (0 to 59).



#### `public inline int millisecond() const` 

Returns the millisecond (0 to 999)



#### `public inline int microsecond() const` 

Returns the microsecond (0 to 999)



#### `public double julianDay() const` 

Returns the julian day for the date and time.



#### `public inline `[`Timestamp`](#classscy_1_1Timestamp)` timestamp() const` 

Returns the date and time expressed as a [Timestamp](#classscy_1_1Timestamp).



#### `public inline `[`Timestamp::UtcTimeVal`](#group__base_1gaec8ace0f25db0ce91c10b30270c3dc24)` utcTime() const` 



Returns the date and time expressed in UTC-based time. UTC base time is midnight, October 15, 1582. Resolution is 100 nanoseconds.

#### `public inline bool operator==(const `[`DateTime`](#classscy_1_1DateTime)` & dateTime) const` 





#### `public inline bool operator!=(const `[`DateTime`](#classscy_1_1DateTime)` & dateTime) const` 





#### `public inline bool operator<(const `[`DateTime`](#classscy_1_1DateTime)` & dateTime) const` 





#### `public inline bool operator<=(const `[`DateTime`](#classscy_1_1DateTime)` & dateTime) const` 





#### `public inline bool operator>(const `[`DateTime`](#classscy_1_1DateTime)` & dateTime) const` 





#### `public inline bool operator>=(const `[`DateTime`](#classscy_1_1DateTime)` & dateTime) const` 





#### `public `[`DateTime`](#classscy_1_1DateTime)` operator+(const `[`Timespan`](#classscy_1_1Timespan)` & span) const` 





#### `public `[`DateTime`](#classscy_1_1DateTime)` operator-(const `[`Timespan`](#classscy_1_1Timespan)` & span) const` 





#### `public `[`Timespan`](#classscy_1_1Timespan)` operator-(const `[`DateTime`](#classscy_1_1DateTime)` & dateTime) const` 





#### `public `[`DateTime`](#classscy_1_1DateTime)` & operator+=(const `[`Timespan`](#classscy_1_1Timespan)` & span)` 





#### `public `[`DateTime`](#classscy_1_1DateTime)` & operator-=(const `[`Timespan`](#classscy_1_1Timespan)` & span)` 





#### `public void makeUTC(int tzd)` 

Converts a local time into UTC, by applying the given time zone differential.



#### `public void makeLocal(int tzd)` 

Converts a UTC time into a local time, by applying the given time zone differential.



#### `protected void computeGregorian(double julianDay)` 



Computes the Gregorian date for the given Julian day. See [http://vsg.cape.com/~pbaum/date/injdimp.htm](http://vsg.cape.com/~pbaum/date/injdimp.htm), section 3.3.1 for the algorithm.

#### `protected void computeDaytime()` 

Extracts the daytime (hours, minutes, seconds, etc.) from the stored utcTime.



# class `scy::DateTimeFormat` 




Definition of date/time formats and various constants used by [DateTimeFormatter](#classscy_1_1DateTimeFormatter) and [DateTimeParser](#classscy_1_1DateTimeParser).

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------

## Members

# class `scy::DateTimeFormatter` 




This class converts dates and times into strings, supporting a variety of standard and custom formats.

There are two kind of static member functions: /// * format* functions return a std::string containing the formatted value.

* append* functions append the formatted value to an existing string.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------

## Members

# class `scy::DateTimeParser` 




This class provides a method for parsing dates and times from strings. All parsing methods do their best to parse a meaningful result, even from malformed input strings.

The returned [DateTime](#classscy_1_1DateTime) will always contain a time in the same timezone as the time in the string. Call [DateTime::makeUTC()](#group__base_1gaef85751cde47a0e909aeddae1775b805) /// with the timeZoneDifferential returned by [parse()](#group__base_1ga7249fe6e774b64ab401a6f3f97311f26) to convert the [DateTime](#classscy_1_1DateTime) to UTC.

Note: When parsing a time in 12-hour (AM/PM) format, the hour (h) must be parsed before the AM/PM designator (a, A), otherwise the AM/PM designator will be ignored.

See the [DateTimeFormatter](#classscy_1_1DateTimeFormatter) class for a list of supported format specifiers. In addition to the format specifiers supported by [DateTimeFormatter](#classscy_1_1DateTimeFormatter), an additional specifier is supported: r will parse a year given by either two or four digits. Years 69-00 are interpreted in the 20th century (1969-2000), years 01-68 in the 21th century (2001-2068).

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------

## Members

# class `scy::Delegate` 

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

#### `public inline  Delegate(C * object,Method method,int priority)` 





#### `public inline  Delegate(C * object,Method method,DataT filter,int priority)` 





#### `public inline  Delegate(const `[`Delegate`](#classscy_1_1Delegate)` & r)` 





#### `public inline virtual  ~Delegate()` 





#### `public inline BaseT * clone() const` 





#### `public inline void emit(void * sender,P arg,P2 arg2,P3 arg3,P4 arg4)` 





#### `public inline bool equals(const `[`DerivedT`](#structscy_1_1DelegateBase)` * r) const` 





#### `public inline void cancel()` 





#### `public inline bool cancelled() const` 





#### `public inline int priority() const` 





#### `public inline void * object() const` 





#### `protected int _priority` 





#### `protected bool _cancelled` 





#### `protected  Delegate()` 





# class `scy::FileChannel` 

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

#### `public  FileChannel(const std::string & name,const std::string & path,LogLevel level,const char * timeFormat)` 





#### `public virtual  ~FileChannel()` 





#### `public virtual void write(const `[`LogStream`](#structscy_1_1LogStream)` & stream)` 





#### `public void setPath(const std::string & path)` 





#### `public std::string path() const` 





#### `protected std::ofstream _fstream` 





#### `protected std::string _path` 





#### `protected virtual void open()` 





#### `protected virtual void close()` 





# class `scy::FlagPacket` 

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

#### `public inline  FlagPacket(unsigned flags)` 





#### `public inline virtual `[`IPacket`](#classscy_1_1IPacket)` * clone() const` 





#### `public inline  FlagPacket(const `[`FlagPacket`](#classscy_1_1FlagPacket)` & that)` 





#### `public inline virtual  ~FlagPacket()` 





#### `public inline virtual std::size_t read(const `[`ConstBuffer`](#classscy_1_1ConstBuffer)` &)` 



Read/parse to the packet from the given input buffer. The number of bytes read is returned.

#### `public inline virtual void write(Buffer &) const` 



Copy/generate to the packet given output buffer. The number of bytes written can be obtained from the buffer.

Todo: It may be prefferable to use our pod types here instead of buffer input, but the current codebase requires that the buffer be dynamically resizable for some protocols...

virtual std::size_t write(MutableBuffer&) const = 0;

#### `public inline virtual const char * className() const` 





# class `scy::GarbageCollector` 


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
`protected mutable `[`Mutex`](#classscy_1_1Mutex)` _mutex` | 
`protected std::vector< `[`ScopedPointer`](#classscy_1_1ScopedPointer)` * > _pending` | 
`protected std::vector< `[`ScopedPointer`](#classscy_1_1ScopedPointer)` * > _ready` | 
`protected `[`uv::Handle`](#classscy_1_1uv_1_1Handle)` _handle` | 
`protected bool _finalize` | 
`protected uv_thread_t _tid` | 
`protected void runAsync()` | 

## Members

#### `public  GarbageCollector()` 





#### `public  ~GarbageCollector()` 





#### `public template<class C>`  <br/>`inline void deleteLater(C * ptr)` 

Schedules a pointer for deferred deletion.



#### `public template<class C>`  <br/>`inline void deleteLater(std::shared_ptr< C > ptr)` 

Schedules a shared pointer for deferred deletion.



#### `public void finalize()` 



Frees all scheduled pointers now. This method must be called from the main thread while the event loop is inactive.

#### `public uv_thread_t tid()` 



Returns the TID of the garbage collector event loop thread. The garbage collector must be running.

#### `protected mutable `[`Mutex`](#classscy_1_1Mutex)` _mutex` 





#### `protected std::vector< `[`ScopedPointer`](#classscy_1_1ScopedPointer)` * > _pending` 





#### `protected std::vector< `[`ScopedPointer`](#classscy_1_1ScopedPointer)` * > _ready` 





#### `protected `[`uv::Handle`](#classscy_1_1uv_1_1Handle)` _handle` 





#### `protected bool _finalize` 





#### `protected uv_thread_t _tid` 





#### `protected void runAsync()` 





# class `scy::Idler` 

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
`protected `[`uv::Handle`](#classscy_1_1uv_1_1Handle)` _handle` | 
`protected virtual void init()` | 
`protected virtual void startAsync()` | Start the context from the control thread.
`protected virtual bool async() const` | 

## Members

#### `public  Idler(uv::Loop * loop)` 

Create the idler context the given event loop and method.



#### `public  Idler(uv::Loop * loop,std::function< void()> target)` 





#### `public  Idler(uv::Loop * loop,std::function< void(void *)> target,void * arg)` 





#### `public virtual  ~Idler()` 





#### `public `[`uv::Handle`](#classscy_1_1uv_1_1Handle)` & handle()` 





#### `protected `[`uv::Handle`](#classscy_1_1uv_1_1Handle)` _handle` 





#### `protected virtual void init()` 





#### `protected virtual void startAsync()` 

Start the context from the control thread.



#### `protected virtual bool async() const` 



Returns true if the implementation is thread-based, or false if it belongs to an event loop.

# class `scy::IPacket` 

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
`public `[`IPacketInfo`](#structscy_1_1IPacketInfo)` * info` | 
`public `[`Bitwise`](#structscy_1_1Bitwise)` flags` | Provides basic information about the packet.
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

#### `public void * source` 



Packet source pointer reference which enables processors along the signal chain can determine the packet origin. Often a subclass of PacketStreamSource.

#### `public void * opaque` 



Optional client data pointer. This pointer is not managed by the packet.

#### `public `[`IPacketInfo`](#structscy_1_1IPacketInfo)` * info` 



Optional extra information about the packet. This pointer is managed by the packet.

#### `public `[`Bitwise`](#structscy_1_1Bitwise)` flags` 

Provides basic information about the packet.



#### `public inline  IPacket(void * source,void * opaque,`[`IPacketInfo`](#structscy_1_1IPacketInfo)` * info,unsigned flags)` 





#### `public inline  IPacket(const `[`IPacket`](#classscy_1_1IPacket)` & r)` 





#### `public inline `[`IPacket`](#classscy_1_1IPacket)` & operator=(const `[`IPacket`](#classscy_1_1IPacket)` & r)` 





#### `public `[`IPacket`](#classscy_1_1IPacket)` * clone() const` 





#### `public inline virtual  ~IPacket()` 





#### `public std::size_t read(const `[`ConstBuffer`](#classscy_1_1ConstBuffer)` &)` 



Read/parse to the packet from the given input buffer. The number of bytes read is returned.

#### `public void write(Buffer &) const` 



Copy/generate to the packet given output buffer. The number of bytes written can be obtained from the buffer.

Todo: It may be prefferable to use our pod types here instead of buffer input, but the current codebase requires that the buffer be dynamically resizable for some protocols...

virtual std::size_t write(MutableBuffer&) const = 0;

#### `public inline virtual std::size_t size() const` 



The size of the packet in bytes.

This is the nember of bytes that will be written on a call to [write()](#group__base_1gabdcb765c58d74be25e190680bf7483d6), but may not be the number of bytes that will be consumed by [read()](#group__base_1ga0886a2812d86d64bbb47b50788cb791a).

#### `public inline virtual bool hasData() const` 





#### `public inline virtual char * data() const` 

The packet data pointer for buffered packets.



#### `public inline virtual const char * constData() const` 

The const packet data pointer for buffered packets.



#### `public const char * className() const` 





#### `public inline virtual void print(std::ostream & os) const` 





# class `scy::IPacketCreationStrategy` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  IPacketCreationStrategy()` | 
`public inline virtual  ~IPacketCreationStrategy()` | 
`public `[`IPacket`](#classscy_1_1IPacket)` * create(const `[`ConstBuffer`](#classscy_1_1ConstBuffer)` & buffer,std::size_t & nread) const` | 
`public int priority() const` | 

## Members

#### `public inline  IPacketCreationStrategy()` 





#### `public inline virtual  ~IPacketCreationStrategy()` 





#### `public `[`IPacket`](#classscy_1_1IPacket)` * create(const `[`ConstBuffer`](#classscy_1_1ConstBuffer)` & buffer,std::size_t & nread) const` 





#### `public int priority() const` 





# class `scy::KVCollection` 


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

#### `public inline  KVCollection()` 





#### `public inline virtual  ~KVCollection()` 





#### `public inline virtual bool add(const TKey & key,const TValue & item,bool update,bool whiny)` 





#### `public inline virtual TValue & get(const TKey & key)` 





#### `public inline virtual const TValue & get(const TKey & key,const TValue & defaultValue) const` 





#### `public inline virtual bool remove(const TKey & key)` 





#### `public inline virtual bool has(const TKey & key) const` 





#### `public inline virtual bool empty() const` 





#### `public inline virtual int size() const` 





#### `public inline virtual void clear()` 





#### `public inline virtual Map & map()` 





#### `protected Map _map` 





# class `scy::LiveCollection` 

```
class scy::LiveCollection
  : public scy::PointerCollection< TKey, TValue, TDeleter >
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`Signal`](#classscy_1_1Signal)`< TValue & > ItemAdded` | 
`public `[`Signal`](#classscy_1_1Signal)`< const TValue & > ItemRemoved` | 
`public inline virtual void onAdd(const TKey &,TValue * item)` | 
`public inline virtual void onRemove(const TKey &,TValue * item)` | 

## Members

#### `public `[`Signal`](#classscy_1_1Signal)`< TValue & > ItemAdded` 





#### `public `[`Signal`](#classscy_1_1Signal)`< const TValue & > ItemRemoved` 





#### `public inline virtual void onAdd(const TKey &,TValue * item)` 





#### `public inline virtual void onRemove(const TKey &,TValue * item)` 





# class `scy::LocalDateTime` 




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
`public  LocalDateTime(double julianDay)` | Creates a [LocalDateTime](#classscy_1_1LocalDateTime) for the given Julian day in the local time zone.
`public  LocalDateTime(int tzd,double julianDay)` | 
`public  LocalDateTime(const `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & dateTime)` | Copy constructor. Creates the [LocalDateTime](#classscy_1_1LocalDateTime) from another one.
`public  ~LocalDateTime()` | Destroys the [LocalDateTime](#classscy_1_1LocalDateTime).
`public `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & operator=(const `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & dateTime)` | Assigns another [LocalDateTime](#classscy_1_1LocalDateTime).
`public `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & operator=(const `[`Timestamp`](#classscy_1_1Timestamp)` & timestamp)` | Assigns a timestamp.
`public `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & operator=(double julianDay)` | Assigns a Julian day in the local time zone.
`public `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & assign(int year,int month,int day,int hour,int minute,int second,int millisecond,int microseconds)` | 
`public `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & assign(int tzd,int year,int month,int day,int hour,int minute,int second,int millisecond,int microseconds)` | 
`public `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & assign(int tzd,double julianDay)` | 
`public void swap(`[`LocalDateTime`](#classscy_1_1LocalDateTime)` & dateTime)` | Swaps the [LocalDateTime](#classscy_1_1LocalDateTime) with another one.
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
`public inline `[`Timestamp`](#classscy_1_1Timestamp)` timestamp() const` | Returns the date and time expressed as a [Timestamp](#classscy_1_1Timestamp).
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

#### `public  LocalDateTime()` 



Creates a [LocalDateTime](#classscy_1_1LocalDateTime) with the current date/time for the current time zone.

#### `public  LocalDateTime(int year,int month,int day,int hour,int minute,int second,int millisecond,int microsecond)` 



Creates a [LocalDateTime](#classscy_1_1LocalDateTime) for the given Gregorian local date and time.

* year is from 0 to 9999.


* month is from 1 to 12.


* day is from 1 to 31.


* hour is from 0 to 23.


* minute is from 0 to 59.


* second is from 0 to 59.


* millisecond is from 0 to 999.


* microsecond is from 0 to 999.

#### `public  LocalDateTime(int tzd,int year,int month,int day,int hour,int minute,int second,int millisecond,int microsecond)` 



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

#### `public  LocalDateTime(const `[`DateTime`](#classscy_1_1DateTime)` & dateTime)` 



Creates a [LocalDateTime](#classscy_1_1LocalDateTime) from the UTC time given in dateTime, using the time zone differential of the current time zone.

#### `public  LocalDateTime(int tzd,const `[`DateTime`](#classscy_1_1DateTime)` & dateTime)` 



@ deprecated Creates a [LocalDateTime](#classscy_1_1LocalDateTime) from the UTC time given in dateTime, using the given time zone differential. Adjusts dateTime for the given time zone differential.

#### `public  LocalDateTime(int tzd,const `[`DateTime`](#classscy_1_1DateTime)` & dateTime,bool adjust)` 



@ deprecated Creates a [LocalDateTime](#classscy_1_1LocalDateTime) from the UTC time given in dateTime, using the given time zone differential. If adjust is true, adjusts dateTime for the given time zone differential.

#### `public  LocalDateTime(double julianDay)` 

Creates a [LocalDateTime](#classscy_1_1LocalDateTime) for the given Julian day in the local time zone.



#### `public  LocalDateTime(int tzd,double julianDay)` 



@ deprecated Creates a [LocalDateTime](#classscy_1_1LocalDateTime) for the given Julian day in the time zone denoted by the time zone differential in tzd.

#### `public  LocalDateTime(const `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & dateTime)` 

Copy constructor. Creates the [LocalDateTime](#classscy_1_1LocalDateTime) from another one.



#### `public  ~LocalDateTime()` 

Destroys the [LocalDateTime](#classscy_1_1LocalDateTime).



#### `public `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & operator=(const `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & dateTime)` 

Assigns another [LocalDateTime](#classscy_1_1LocalDateTime).



#### `public `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & operator=(const `[`Timestamp`](#classscy_1_1Timestamp)` & timestamp)` 

Assigns a timestamp.



#### `public `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & operator=(double julianDay)` 

Assigns a Julian day in the local time zone.



#### `public `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & assign(int year,int month,int day,int hour,int minute,int second,int millisecond,int microseconds)` 



Assigns a Gregorian local date and time.

* year is from 0 to 9999.


* month is from 1 to 12.


* day is from 1 to 31.


* hour is from 0 to 23.


* minute is from 0 to 59.


* second is from 0 to 59.


* millisecond is from 0 to 999.


* microsecond is from 0 to 999.

#### `public `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & assign(int tzd,int year,int month,int day,int hour,int minute,int second,int millisecond,int microseconds)` 



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

#### `public `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & assign(int tzd,double julianDay)` 



@ deprecated Assigns a Julian day in the time zone denoted by the time zone differential in tzd.

#### `public void swap(`[`LocalDateTime`](#classscy_1_1LocalDateTime)` & dateTime)` 

Swaps the [LocalDateTime](#classscy_1_1LocalDateTime) with another one.



#### `public inline int year() const` 

Returns the year.



#### `public inline int month() const` 

Returns the month (1 to 12).



#### `public inline int week(int firstDayOfWeek) const` 



Returns the week number within the year. FirstDayOfWeek should be either SUNDAY (0) or MONDAY (1). The returned week number will be from 0 to 53. Week number 1 is the week containing January 4. This is in accordance to ISO 8601.

The following example assumes that firstDayOfWeek is MONDAY. For 2005, which started on a Saturday, week 1 will be the week starting on Monday, January 3. January 1 and 2 will fall within week 0 (or the last week of the previous year).

For 2007, which starts on a Monday, week 1 will be the week startung on Monday, January 1. There will be no week 0 in 2007.

#### `public inline int day() const` 

Returns the day witin the month (1 to 31).



#### `public inline int dayOfWeek() const` 



Returns the weekday (0 to 6, where 0 = Sunday, 1 = Monday, ..., 6 = Saturday).

#### `public inline int dayOfYear() const` 



Returns the number of the day in the year. January 1 is 1, February 1 is 32, etc.

#### `public inline int hour() const` 

Returns the hour (0 to 23).



#### `public inline int hourAMPM() const` 

Returns the hour (0 to 12).



#### `public inline bool isAM() const` 

Returns true if hour < 12;.



#### `public inline bool isPM() const` 

Returns true if hour >= 12.



#### `public inline int minute() const` 

Returns the minute (0 to 59).



#### `public inline int second() const` 

Returns the second (0 to 59).



#### `public inline int millisecond() const` 

Returns the millisecond (0 to 999)



#### `public inline int microsecond() const` 

Returns the microsecond (0 to 999)



#### `public inline double julianDay() const` 

Returns the julian day for the date.



#### `public inline int tzd() const` 

Returns the time zone differential.



#### `public `[`DateTime`](#classscy_1_1DateTime)` utc() const` 

Returns the UTC equivalent for the local date and time.



#### `public inline `[`Timestamp`](#classscy_1_1Timestamp)` timestamp() const` 

Returns the date and time expressed as a [Timestamp](#classscy_1_1Timestamp).



#### `public inline `[`Timestamp::UtcTimeVal`](#group__base_1gaec8ace0f25db0ce91c10b30270c3dc24)` utcTime() const` 

Returns the UTC equivalent for the local date and time.



#### `public bool operator==(const `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & dateTime) const` 





#### `public bool operator!=(const `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & dateTime) const` 





#### `public bool operator<(const `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & dateTime) const` 





#### `public bool operator<=(const `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & dateTime) const` 





#### `public bool operator>(const `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & dateTime) const` 





#### `public bool operator>=(const `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & dateTime) const` 





#### `public `[`LocalDateTime`](#classscy_1_1LocalDateTime)` operator+(const `[`Timespan`](#classscy_1_1Timespan)` & span) const` 





#### `public `[`LocalDateTime`](#classscy_1_1LocalDateTime)` operator-(const `[`Timespan`](#classscy_1_1Timespan)` & span) const` 





#### `public `[`Timespan`](#classscy_1_1Timespan)` operator-(const `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & dateTime) const` 





#### `public `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & operator+=(const `[`Timespan`](#classscy_1_1Timespan)` & span)` 





#### `public `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & operator-=(const `[`Timespan`](#classscy_1_1Timespan)` & span)` 





#### `protected  LocalDateTime(`[`Timestamp::UtcTimeVal`](#group__base_1gaec8ace0f25db0ce91c10b30270c3dc24)` utcTime,`[`Timestamp::TimeDiff`](#group__base_1ga9f06831e4e9b9cbc6f061cc61dc91fd0)` diff,int tzd)` 





#### `protected void determineTzd(bool adjust)` 



Recalculate the tzd based on the _dateTime member based on the current timezone using the Standard C runtime functions. If adjust is true, then [adjustForTzd()](#group__base_1gad0d104ecf1aa081650b681f2afe5718d) is called after the differential is calculated.

#### `protected inline void adjustForTzd()` 

Adjust the _dateTime member based on the _tzd member.



#### `protected std::time_t dstOffset(int & dstOffset) const` 

Determine the DST offset for the current date/time.



# class `scy::LogChannel` 






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

#### `public  LogChannel(const std::string & name,LogLevel level,const std::string & timeFormat)` 





#### `public inline virtual  ~LogChannel()` 





#### `public virtual void write(const `[`LogStream`](#structscy_1_1LogStream)` & stream)` 





#### `public virtual void write(const std::string & message,LogLevel level,const char * realm,const void * ptr)` 





#### `public virtual void format(const `[`LogStream`](#structscy_1_1LogStream)` & stream,std::ostream & ost)` 





#### `public inline std::string name() const` 





#### `public inline LogLevel level() const` 





#### `public inline std::string timeFormat() const` 





#### `public inline void setLevel(LogLevel level)` 





#### `public inline void setDateFormat(const std::string & format)` 





#### `public inline void setFilter(const std::string & filter)` 





#### `protected std::string _name` 





#### `protected LogLevel _level` 





#### `protected std::string _timeFormat` 





#### `protected std::string _filter` 





# class `scy::Logger` 






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
`protected mutable `[`Mutex`](#classscy_1_1Mutex)` _mutex` | 
`protected LogChannelMap _channels` | 
`protected `[`LogChannel`](#classscy_1_1LogChannel)` * _defaultChannel` | 
`protected `[`LogWriter`](#classscy_1_1LogWriter)` * _writer` | 
`protected  Logger(const `[`Logger`](#classscy_1_1Logger)` &)` | Non-copyable and non-movable.
`protected  Logger(`[`Logger`](#classscy_1_1Logger)` &&)` | 
`protected `[`Logger`](#classscy_1_1Logger)` & operator=(const `[`Logger`](#classscy_1_1Logger)` &)` | 
`protected `[`Logger`](#classscy_1_1Logger)` & operator=(`[`Logger`](#classscy_1_1Logger)` &&)` | 

## Members

#### `public  Logger()` 





#### `public  ~Logger()` 





#### `public void add(`[`LogChannel`](#classscy_1_1LogChannel)` * channel)` 

Adds the given log channel.



#### `public void remove(const std::string & name,bool freePointer)` 



Removes the given log channel by name, and optionally frees the pointer.

#### `public `[`LogChannel`](#classscy_1_1LogChannel)` * get(const std::string & name,bool whiny) const` 



Returns the specified log channel. Throws an exception if the channel doesn't exist.

#### `public void setDefault(const std::string & name)` 

Sets the default log to the specified log channel.



#### `public void setWriter(`[`LogWriter`](#classscy_1_1LogWriter)` * writer)` 

Sets the log writer instance.



#### `public `[`LogChannel`](#classscy_1_1LogChannel)` * getDefault() const` 



Returns the default log channel, or the nullptr channel if no default channel has been set.

#### `public void write(const `[`LogStream`](#structscy_1_1LogStream)` & stream)` 



Writes the given message to the default log channel. The message will be copied.

#### `public void write(`[`LogStream`](#structscy_1_1LogStream)` * stream)` 



Writes the given message to the default log channel. The stream pointer will be deleted when appropriate.

#### `public `[`LogStream`](#structscy_1_1LogStream)` & send(const char * level,const char * realm,const void * ptr,const char * channel) const` 



Sends to the default log using the given class instance. Recommend using write(LogStream&) to avoid copying data.

#### `protected mutable `[`Mutex`](#classscy_1_1Mutex)` _mutex` 





#### `protected LogChannelMap _channels` 





#### `protected `[`LogChannel`](#classscy_1_1LogChannel)` * _defaultChannel` 





#### `protected `[`LogWriter`](#classscy_1_1LogWriter)` * _writer` 





#### `protected  Logger(const `[`Logger`](#classscy_1_1Logger)` &)` 

Non-copyable and non-movable.



#### `protected  Logger(`[`Logger`](#classscy_1_1Logger)` &&)` 





#### `protected `[`Logger`](#classscy_1_1Logger)` & operator=(const `[`Logger`](#classscy_1_1Logger)` &)` 





#### `protected `[`Logger`](#classscy_1_1Logger)` & operator=(`[`Logger`](#classscy_1_1Logger)` &&)` 





# class `scy::LogWriter` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  LogWriter()` | 
`public virtual  ~LogWriter()` | 
`public virtual void write(`[`LogStream`](#structscy_1_1LogStream)` * stream)` | Writes the given log message stream.

## Members

#### `public  LogWriter()` 





#### `public virtual  ~LogWriter()` 





#### `public virtual void write(`[`LogStream`](#structscy_1_1LogStream)` * stream)` 

Writes the given log message stream.



# class `scy::MutableBuffer` 




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

#### `public inline  MutableBuffer()` 

Construct an empty buffer.



#### `public inline  MutableBuffer(void * data,std::size_t size)` 

Construct a buffer to represent the given memory range.



#### `public inline void * data() const` 





#### `public inline std::size_t size() const` 





#### `public inline char * cstr() const` 

String methods.



#### `public inline std::string str() const` 





# class `scy::Mutex` 




This class is a wrapper around uv_mutex_t.

A [Mutex](#classscy_1_1Mutex) (mutual exclusion) is a synchronization mechanism used to control access to a shared resource in a concurrent (multithreaded) scenario.

The [ScopedLock](#classscy_1_1ScopedLock) class is usually used to obtain a [Mutex](#classscy_1_1Mutex) lock, since it makes locking exception-safe.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Mutex()` | 
`public  ~Mutex()` | 
`public void lock()` | 
`public bool tryLock()` | 
`public void unlock()` | 

## Members

#### `public  Mutex()` 





#### `public  ~Mutex()` 





#### `public void lock()` 



Locks the mutex. Blocks if the mutex is held by another thread.

#### `public bool tryLock()` 



Tries to lock the mutex. Returns false if the mutex is already held by another thread. Returns true if the mutex was successfully locked.

#### `public void unlock()` 



Unlocks the mutex so that it can be acquired by other threads.

# class `scy::MutexState` 

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
`protected mutable `[`Mutex`](#classscy_1_1Mutex)` _mutex` | 

## Members

#### `public  MutexState(ID id)` 





#### `public inline  MutexState(const `[`MutexState`](#classscy_1_1MutexState)` & r)` 





#### `public inline virtual  ~MutexState()` 





#### `public inline virtual ID id() const` 





#### `public inline virtual void set(ID id)` 





#### `public inline virtual std::string message() const` 





#### `public inline virtual void setMessage(const std::string & message)` 





#### `protected mutable `[`Mutex`](#classscy_1_1Mutex)` _mutex` 





# class `scy::NullSignal` 

```
class scy::NullSignal
  : public scy::SignalBase< DelegateBase<> >
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------

## Members

# class `scy::NVCollection` 




A storage container for a name value collections. This collection can store multiple entries for each name, and it's getters are case-insensitive.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  NVCollection()` | 
`public inline  _map(nvc. _map) const` | 
`public inline virtual  ~NVCollection()` | 
`public inline `[`NVCollection`](#classscy_1_1NVCollection)` & operator=(const `[`NVCollection`](#classscy_1_1NVCollection)` & nvc)` | Assigns the name-value pairs of another [NVCollection](#classscy_1_1NVCollection) to this one.
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

#### `public inline  NVCollection()` 





#### `public inline  _map(nvc. _map) const` 





#### `public inline virtual  ~NVCollection()` 





#### `public inline `[`NVCollection`](#classscy_1_1NVCollection)` & operator=(const `[`NVCollection`](#classscy_1_1NVCollection)` & nvc)` 

Assigns the name-value pairs of another [NVCollection](#classscy_1_1NVCollection) to this one.



#### `public inline const std::string & operator[](const std::string & name) const` 



Returns the value of the (first) name-value pair with the given name.

Throws a NotFoundException if the name-value pair does not exist.

#### `public inline void set(const std::string & name,const std::string & value)` 

Sets the value of the (first) name-value pair with the given name.



#### `public inline void add(const std::string & name,const std::string & value)` 

Adds a new name-value pair with the given name and value.



#### `public inline const std::string & get(const std::string & name) const` 



Returns the value of the first name-value pair with the given name.

Throws a NotFoundException if the name-value pair does not exist.

#### `public inline const std::string & get(const std::string & name,const std::string & defaultValue) const` 



Returns the value of the first name-value pair with the given name. If no value with the given name has been found, the defaultValue is returned.

#### `public inline bool has(const std::string & name) const` 



Returns true if there is at least one name-value pair with the given name.

#### `public inline ConstIterator find(const std::string & name) const` 



Returns an iterator pointing to the first name-value pair with the given name.

#### `public inline ConstIterator begin() const` 



Returns an iterator pointing to the begin of the name-value pair collection.

#### `public inline ConstIterator end() const` 



Returns an iterator pointing to the end of the name-value pair collection.

#### `public inline bool empty() const` 

Returns true iff the header does not have any content.



#### `public inline int size() const` 



Returns the number of name-value pairs in the collection.

#### `public inline void erase(const std::string & name)` 

Removes all name-value pairs with the given name.



#### `public inline void clear()` 

Removes all name-value pairs and their values.



# class `scy::PacketFactory` 






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

#### `public inline  PacketFactory()` 





#### `public inline virtual  ~PacketFactory()` 





#### `public template<class PacketT>`  <br/>`inline void registerPacketType(int priority)` 





#### `public template<class PacketT>`  <br/>`inline void unregisterPacketType()` 





#### `public template<class StrategyT>`  <br/>`inline void registerStrategy(int priority)` 





#### `public template<class StrategyT>`  <br/>`inline void unregisterStrategy()` 





#### `public inline PacketCreationStrategyList & types()` 





#### `public inline PacketCreationStrategyList types() const` 





#### `public inline virtual bool onPacketCreated(`[`IPacket`](#classscy_1_1IPacket)` *)` 

returning false will stop packet propagation



#### `public inline virtual `[`IPacket`](#classscy_1_1IPacket)` * createPacket(const `[`ConstBuffer`](#classscy_1_1ConstBuffer)` & buffer,std::size_t & nread)` 





#### `protected PacketCreationStrategyList _types` 





# class `scy::PacketProcessor` 

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

#### `public inline  PacketProcessor(`[`PacketSignal`](#classscy_1_1SignalBase)` & emitter)` 





#### `public void process(`[`IPacket`](#classscy_1_1IPacket)` & packet)` 



This method performs processing on the given packet and emits the result.

Note: If packet processing is async (the packet is not in the current thread scope) then packet data must be copied. Copied data can be freed directly aFter the async call to emit() the outgoing packet.

#### `public inline virtual bool accepts(`[`IPacket`](#classscy_1_1IPacket)` &)` 



This method ensures compatibility with the given packet type. Return false to reject the packet.

#### `public inline virtual void operator<<(`[`IPacket`](#classscy_1_1IPacket)` & packet)` 

[Stream](#classscy_1_1Stream) operator alias for [process()](#group__base_1gad7adc3fd78dce41f0f96744dfe6d1d50)



# class `scy::PacketStream` 

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
`public `[`PacketSignal`](#classscy_1_1SignalBase)` emitter` | Signals to delegates on outgoing packets.
`public `[`Signal`](#classscy_1_1Signal)`< const std::exception_ptr & > Error` | 
`public `[`NullSignal`](#classscy_1_1NullSignal)` Close` | 
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
`public virtual bool closed() const` | Returns true when the stream is in the Closed or [Error](#structscy_1_1Error) state.
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
`public inline `[`PacketProcessor`](#classscy_1_1PacketProcessor)` * getProcessor(int order)` | Returns the [PacketProcessor](#classscy_1_1PacketProcessor) at the given position.
`protected mutable `[`Mutex`](#classscy_1_1Mutex)` _mutex` | 
`protected mutable `[`Mutex`](#classscy_1_1Mutex)` _procMutex` | 
`protected std::string _name` | 
`protected PacketAdapterVec _sources` | 
`protected PacketAdapterVec _processors` | 
`protected std::deque< `[`PacketStreamState`](#structscy_1_1PacketStreamState)` > _states` | 
`protected std::exception_ptr _error` | 
`protected bool _autoStart` | 
`protected bool _closeOnError` | 
`protected void * _clientData` | 
`protected void setup()` | Attach the source and processor delegate chain.
`protected void teardown()` | Detach the source and processor delegate chain.
`protected void emit(`[`IPacket`](#classscy_1_1IPacket)` & packet)` | 
`protected void attachSource(PacketAdapterReference::Ptr ref)` | 
`protected void attach(PacketAdapterReference::Ptr ref)` | 
`protected void process(`[`IPacket`](#classscy_1_1IPacket)` & packet)` | [Process](#classscy_1_1Process) incoming packets.
`protected void startSources()` | Start synchronized sources.
`protected void stopSources()` | Stop synchronized sources.
`protected void synchronizeStates()` | Synchronize queued states with adapters.
`protected virtual void onStateChange(`[`PacketStreamState`](#structscy_1_1PacketStreamState)` & state,const `[`PacketStreamState`](#structscy_1_1PacketStreamState)` & oldState)` | Override the [Stateful::onStateChange](#group__base_1ga3c88b4aa8e9343f9bdc1a8c2ae2a5452) method.
`protected bool hasQueuedState(PacketStreamState::ID state) const` | Returns true if the given state ID is queued.
`protected void assertCanModify()` | 

## Members

#### `public `[`PacketSignal`](#classscy_1_1SignalBase)` emitter` 

Signals to delegates on outgoing packets.



#### `public `[`Signal`](#classscy_1_1Signal)`< const std::exception_ptr & > Error` 



Signals that the [PacketStream](#classscy_1_1PacketStream) is in [Error](#structscy_1_1Error) state. If stream output is synchronized then the [Error](#structscy_1_1Error) signal will be sent from the synchronization context, otherwise it will be sent from the async processor context. See [synchronizeOutput()](#group__base_1ga27a797846a098946cf73d80726302a9e)

#### `public `[`NullSignal`](#classscy_1_1NullSignal)` Close` 



Signals that the [PacketStream](#classscy_1_1PacketStream) is in Close state. This signal is sent immediately via the [close()](#group__base_1gac0eec4eda5ff0571b46b4a39af9f7dcf) method, and as such will be sent from the calling thread context.

#### `public  PacketStream(const std::string & name)` 





#### `public virtual  ~PacketStream()` 





#### `public virtual void start()` 

Start the stream and synchronized sources.



#### `public virtual void stop()` 

Stop the stream and synchronized sources.



#### `public virtual void pause()` 

Pause the stream.



#### `public virtual void resume()` 

Resume the stream.



#### `public virtual void close()` 

Close the stream and transition the internal state to Closed.



#### `public virtual void reset()` 

Cleanup all managed stream adapters and reset the stream state.



#### `public virtual bool active() const` 

Returns true when the stream is in the Active state.



#### `public virtual bool stopped() const` 

Returns true when the stream is in the Stopping or Stopped state.



#### `public virtual bool closed() const` 

Returns true when the stream is in the Closed or [Error](#structscy_1_1Error) state.



#### `public virtual bool lock()` 



Sets the stream to locked state. In a locked state no new adapters can be added or removed from the stream until the stream is stopped.

#### `public virtual bool locked() const` 

Returns true is the stream is currently locked.



#### `public virtual void write(char * data,std::size_t len)` 

Writes data to the stream (nocopy).



#### `public virtual void write(const char * data,std::size_t len)` 

Writes data to the stream (copied).



#### `public virtual void write(`[`IPacket`](#classscy_1_1IPacket)` & packet)` 

Writes an incoming packet onto the stream.



#### `public virtual void attachSource(`[`PacketSignal`](#classscy_1_1SignalBase)` & source)` 



Attaches a source packet emitter to the stream. The source packet adapter can be another [PacketStream::emitter](#group__base_1ga3d4fa2d39446cd24ce5d8281834b70db).

#### `public virtual void attachSource(`[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` * source,bool freePointer,bool syncState)` 



Attaches a source packet emitter to the stream. If freePointer is true, the pointer will be deleted when the stream is closed. If syncState is true and the source is a basic::Stratable, then the source's [start()](#group__base_1gaa07075299f2271e58f78de3fd7e62b10)/stop() methods will be synchronized when calling [startSources()](#group__base_1gaeb9605f5d35a6aaa92bf427e0989827c)/stopSources().

#### `public template<class C>`  <br/>`inline void attachSource(std::shared_ptr< C > ptr,bool syncState)` 



Attaches a source packet emitter to the stream. This method enables compatibility with shared_ptr managed adapter instances.

#### `public virtual bool detachSource(`[`PacketSignal`](#classscy_1_1SignalBase)` & source)` 

Detaches the given source packet signal from the stream.



#### `public virtual bool detachSource(`[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` * source)` 



Detaches the given source packet adapter from the stream. Note: The pointer will be forgotten about, so if the freePointer flag set when calling [attachSource()](#group__base_1ga16adec5b00a33070556dd8531e1825c0) will have no effect.

#### `public virtual void attach(`[`PacketProcessor`](#classscy_1_1PacketProcessor)` * proc,int order,bool freePointer)` 



Attaches a packet processor to the stream. Order determines the position of the processor in the stream queue. If freePointer is true, the pointer will be deleted when the stream closes.

#### `public template<class C>`  <br/>`inline void attach(std::shared_ptr< C > ptr,bool syncState)` 



Attaches a packet processor to the stream. This method enables compatibility with shared_ptr managed adapter instances.

#### `public virtual bool detach(`[`PacketProcessor`](#classscy_1_1PacketProcessor)` * proc)` 



Detaches a packet processor from the stream. Note: The pointer will be forgotten about, so if the freePointer flag set when calling [attach()](#group__base_1gaeff06c27d0ed0174f7f19ff61b3ff2fe) will have no effect.

#### `public virtual void synchronizeOutput(uv::Loop * loop)` 

Synchronize stream output packets with the given event loop.



#### `public virtual void autoStart(bool flag)` 



Set the stream to auto start if inactive (default: false).

With this flag set the stream will automatically transition to Active state if the in either the None or Locaked state.

#### `public virtual void closeOnError(bool flag)` 



Set the stream to be closed on error (default: true).

With this flag set the stream will be automatically transitioned to Closed state from [Error](#structscy_1_1Error) state.

#### `public virtual void setClientData(void * data)` 

Accessors for the unmanaged client data pointer.



#### `public virtual void * clientData() const` 





#### `public const std::exception_ptr & error()` 

Returns the stream error (if any).



#### `public std::string name() const` 

Returns the name of the packet stream.



#### `public PacketAdapterVec adapters() const` 

Returns a combined list of all stream sources and processors.



#### `public PacketAdapterVec sources() const` 

Returns a list of all stream sources.



#### `public PacketAdapterVec processors() const` 

Returns a list of all stream processors.



#### `public bool waitForRunner()` 



Block the calling thread until all packets have been flushed, and internal states have been synchronized. This function is only useful after calling [stop()](#group__base_1ga5ace99af3f0ff1e93483a7f5dd5dca69) or [pause()](#group__base_1ga866ba120ec05be813d82a6e05827292e).

#### `public bool waitForStateSync(PacketStreamState::ID state)` 

Block the calling thread until the given state is synchronized.



#### `public int numSources() const` 





#### `public int numProcessors() const` 





#### `public int numAdapters() const` 





#### `public template<class AdapterT>`  <br/>`inline AdapterT * getSource(int index)` 





#### `public template<class AdapterT>`  <br/>`inline AdapterT * getProcessor(int index)` 





#### `public inline `[`PacketProcessor`](#classscy_1_1PacketProcessor)` * getProcessor(int order)` 

Returns the [PacketProcessor](#classscy_1_1PacketProcessor) at the given position.



#### `protected mutable `[`Mutex`](#classscy_1_1Mutex)` _mutex` 





#### `protected mutable `[`Mutex`](#classscy_1_1Mutex)` _procMutex` 





#### `protected std::string _name` 





#### `protected PacketAdapterVec _sources` 





#### `protected PacketAdapterVec _processors` 





#### `protected std::deque< `[`PacketStreamState`](#structscy_1_1PacketStreamState)` > _states` 





#### `protected std::exception_ptr _error` 





#### `protected bool _autoStart` 





#### `protected bool _closeOnError` 





#### `protected void * _clientData` 





#### `protected void setup()` 

Attach the source and processor delegate chain.



#### `protected void teardown()` 

Detach the source and processor delegate chain.



#### `protected void emit(`[`IPacket`](#classscy_1_1IPacket)` & packet)` 



Emit the final packet to listeners.

Synchronized signals such as Close and [Error](#structscy_1_1Error) are sent from this method. See [synchronizeOutput()](#group__base_1ga27a797846a098946cf73d80726302a9e)

#### `protected void attachSource(PacketAdapterReference::Ptr ref)` 





#### `protected void attach(PacketAdapterReference::Ptr ref)` 





#### `protected void process(`[`IPacket`](#classscy_1_1IPacket)` & packet)` 

[Process](#classscy_1_1Process) incoming packets.



#### `protected void startSources()` 

Start synchronized sources.



#### `protected void stopSources()` 

Stop synchronized sources.



#### `protected void synchronizeStates()` 

Synchronize queued states with adapters.



#### `protected virtual void onStateChange(`[`PacketStreamState`](#structscy_1_1PacketStreamState)` & state,const `[`PacketStreamState`](#structscy_1_1PacketStreamState)` & oldState)` 

Override the [Stateful::onStateChange](#group__base_1ga3c88b4aa8e9343f9bdc1a8c2ae2a5452) method.



#### `protected bool hasQueuedState(PacketStreamState::ID state) const` 

Returns true if the given state ID is queued.



#### `protected void assertCanModify()` 



Asserts that the stream can be modified, ie is not in the Locked, Stopping or Active states.

# class `scy::PacketStreamAdapter` 




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
`protected `[`PacketSignal`](#classscy_1_1SignalBase)` & _emitter` | 
`protected  PacketStreamAdapter(const `[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` &)` | 
`protected  PacketStreamAdapter(`[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` &&)` | 
`protected `[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` & operator=(const `[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` &)` | 
`protected `[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` & operator=(`[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` &&)` | 

## Members

#### `public  PacketStreamAdapter(`[`PacketSignal`](#classscy_1_1SignalBase)` & emitter)` 





#### `public inline virtual  ~PacketStreamAdapter()` 





#### `public virtual void emit(char * data,std::size_t len,unsigned flags)` 





#### `public virtual void emit(const char * data,std::size_t len,unsigned flags)` 





#### `public virtual void emit(const std::string & str,unsigned flags)` 





#### `public virtual void emit(unsigned flags)` 





#### `public virtual void emit(`[`IPacket`](#classscy_1_1IPacket)` & packet)` 





#### `public `[`PacketSignal`](#classscy_1_1SignalBase)` & getEmitter()` 

Returns a reference to the outgoing packet signal.



#### `public inline virtual void onStreamStateChange(const `[`PacketStreamState`](#structscy_1_1PacketStreamState)` &)` 



Called by the [PacketStream](#classscy_1_1PacketStream) to notify when the internal [Stream](#classscy_1_1Stream) state changes. On receiving the Stopped state, it is the responsibility of the adapter to have ceased all outgoing packet transmission, especially in multi-thread scenarios.

#### `protected `[`PacketSignal`](#classscy_1_1SignalBase)` & _emitter` 





#### `protected  PacketStreamAdapter(const `[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` &)` 





#### `protected  PacketStreamAdapter(`[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` &&)` 





#### `protected `[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` & operator=(const `[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` &)` 





#### `protected `[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` & operator=(`[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` &&)` 





# class `scy::PacketTransaction` 

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
`protected `[`Timer`](#classscy_1_1Timer)` _timer` | 
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

#### `public inline  PacketTransaction(long timeout,int retries,uv::Loop * loop)` 





#### `public inline  PacketTransaction(const PacketT & request,long timeout,int retries,uv::Loop * loop)` 





#### `public inline virtual bool send()` 



Starts the transaction timer and sends the request. Overriding classes should implement send logic here.

#### `public inline virtual void cancel()` 



Cancellation means that the agent will not retransmit the request, will not treat the lack of response to be a failure, but will wait the duration of the transaction timeout for a response.

#### `public inline bool cancelled() const` 





#### `public inline virtual void dispose()` 



Schedules the transaction for deferred deletion.

It is safe to call this function while the transaction is still active, providing the call is made from the same loop thread which the timer is running on.

Protected by the base implementation as this is called by the internal state machine.

#### `public inline virtual bool canResend()` 





#### `public inline int attempts() const` 





#### `public inline int retries() const` 





#### `public inline PacketT & request()` 





#### `public inline PacketT request() const` 





#### `public inline PacketT & response()` 





#### `public inline PacketT response() const` 





#### `protected PacketT _request` 





#### `protected PacketT _response` 





#### `protected `[`Timer`](#classscy_1_1Timer)` _timer` 





#### `protected int _timeout` 

The request timeout in milliseconds.



#### `protected int _retries` 

The maximum number of attempts before the transaction is considered failed.



#### `protected int _attempts` 

The number of times the transaction has been sent.



#### `protected bool _destroyed` 





#### `protected inline virtual  ~PacketTransaction()` 





#### `protected inline virtual void onStateChange(`[`TransactionState`](#structscy_1_1TransactionState)` & state,const `[`TransactionState`](#structscy_1_1TransactionState)` &)` 

Override to handle post state change logic.



#### `protected inline virtual bool handlePotentialResponse(const PacketT & packet)` 



Processes a potential response candidate and updates the state accordingly.

#### `protected bool checkResponse(const PacketT & packet)` 



Checks a potential response candidate and returns true on successful match.

#### `protected inline virtual void onResponse()` 

Called when a successful response is received.



#### `protected inline virtual void onTimeout(void *)` 





# class `scy::PointerCollection` 

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
`protected mutable `[`Mutex`](#classscy_1_1Mutex)` _mutex` | 

## Members

#### `public inline  PointerCollection()` 





#### `public inline virtual  ~PointerCollection()` 





#### `public inline virtual bool add(const TKey & key,TValue * item,bool whiny)` 





#### `public inline virtual void update(const TKey & key,TValue * item)` 





#### `public inline virtual TValue * get(const TKey & key,bool whiny) const` 





#### `public inline virtual bool free(const TKey & key)` 





#### `public inline virtual TValue * remove(const TKey & key)` 





#### `public inline virtual bool remove(const TValue * item)` 





#### `public inline virtual bool exists(const TKey & key) const` 





#### `public inline virtual bool exists(const TValue * item) const` 





#### `public inline virtual bool empty() const` 





#### `public inline virtual int size() const` 





#### `public inline virtual void clear()` 





#### `public inline virtual Map map() const` 





#### `public inline virtual Map & map()` 





#### `public inline virtual void onAdd(const TKey &,TValue *)` 





#### `public inline virtual void onRemove(const TKey &,TValue *)` 





#### `protected Map _map` 





#### `protected mutable `[`Mutex`](#classscy_1_1Mutex)` _mutex` 





# class `scy::Process` 

```
class scy::Process
  : public scy::uv::Handle
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::vector< std::string > args` | 
`public std::function< void(std::int64_t)> onexit` | Exit callback; returns the exit status.
`public ProcessOptions options` | [Process](#classscy_1_1Process) options.
`public  Process(uv::Loop * loop)` | 
`public void spawn()` | 
`public bool kill(int signum)` | Kills the process.
`public int pid() const` | Returns the process PID.
`protected uv_process_t _proc` | 

## Members

#### `public std::vector< std::string > args` 



Command line args. STL proxy for options.args

#### `public std::function< void(std::int64_t)> onexit` 

Exit callback; returns the exit status.



#### `public ProcessOptions options` 

[Process](#classscy_1_1Process) options.



#### `public  Process(uv::Loop * loop)` 





#### `public void spawn()` 



Spawns the process. Options must be properly set. Throws and exception on error.

#### `public bool kill(int signum)` 

Kills the process.



#### `public int pid() const` 

Returns the process PID.



#### `protected uv_process_t _proc` 





# class `scy::Queue` 


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

#### `public inline void push(const T & data)` 





#### `public inline bool empty() const` 





#### `public inline T & front()` 





#### `public inline T const  & front() const` 





#### `public inline T & back()` 





#### `public inline T const  & back() const` 





#### `public inline void pop()` 





#### `public template<typename Compare>`  <br/>`inline std::size_t sort()` 





#### `public inline std::size_t size()` 





#### `public inline std::deque< T > & queue()` 





# class `scy::Random` 




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

#### `public  Random(int stateSize)` 



Creates and initializes the PRNG. Specify either a state buffer size (8 to 256 bytes) or one of the Type values.

#### `public  ~Random()` 

Destroys the PRNG.



#### `public void seed(std::uint32_t seed)` 

Seeds the pseudo random generator with the given seed.



#### `public void seed()` 



Seeds the pseudo random generator with a random seed obtained from a RandomInputStream.

#### `public std::uint32_t next()` 

Returns the next 31-bit pseudo random number.



#### `public std::uint32_t next(std::uint32_t n)` 

Returns the next 31-bit pseudo random number modulo n.



#### `public char nextChar()` 

Returns the next pseudo random character.



#### `public bool nextBool()` 

Returns the next boolean pseudo random value.



#### `public float nextFloat()` 

Returns the next float pseudo random number between 0.0 and 1.0.



#### `public double nextDouble()` 

Returns the next double pseudo random number between 0.0 and 1.0.



#### `protected void initState(std::uint32_t seed,char * arg_state,std::int32_t n)` 





# class `scy::RawPacket` 

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

#### `public char * _data` 





#### `public std::size_t _size` 





#### `public bool _free` 





#### `public inline  RawPacket(char * data,std::size_t size,unsigned flags,void * source,void * opaque,`[`IPacketInfo`](#structscy_1_1IPacketInfo)` * info)` 





#### `public inline  RawPacket(const char * data,std::size_t size,unsigned flags,void * source,void * opaque,`[`IPacketInfo`](#structscy_1_1IPacketInfo)` * info)` 





#### `public inline  RawPacket(const `[`RawPacket`](#classscy_1_1RawPacket)` & that)` 





#### `public inline virtual  ~RawPacket()` 





#### `public inline virtual `[`IPacket`](#classscy_1_1IPacket)` * clone() const` 





#### `public inline virtual void setData(char * data,std::size_t size)` 





#### `public inline virtual void copyData(const char * data,std::size_t size)` 





#### `public inline virtual std::size_t read(const `[`ConstBuffer`](#classscy_1_1ConstBuffer)` &)` 



Read/parse to the packet from the given input buffer. The number of bytes read is returned.

#### `public inline virtual void write(Buffer &) const` 



Copy/generate to the packet given output buffer. The number of bytes written can be obtained from the buffer.

Todo: It may be prefferable to use our pod types here instead of buffer input, but the current codebase requires that the buffer be dynamically resizable for some protocols...

virtual std::size_t write(MutableBuffer&) const = 0;

#### `public inline virtual char * data() const` 

The packet data pointer for buffered packets.



#### `public inline virtual std::size_t size() const` 



The size of the packet in bytes.

This is the nember of bytes that will be written on a call to [write()](#group__base_1gaa237661815dfe9a2879a816a851b8329), but may not be the number of bytes that will be consumed by [read()](#group__base_1ga2fa8aec0d5020bba8911318dbadb6fd6).

#### `public inline virtual const char * className() const` 





#### `public inline virtual bool ownsBuffer() const` 





#### `public inline virtual void assignDataOwnership()` 





# class `scy::RotatingFileChannel` 

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

#### `public  RotatingFileChannel(const std::string & name,const std::string & dir,LogLevel level,const std::string & extension,int rotationInterval,const char * timeFormat)` 





#### `public virtual  ~RotatingFileChannel()` 





#### `public virtual void write(const `[`LogStream`](#structscy_1_1LogStream)` & stream)` 





#### `public virtual void rotate()` 





#### `public inline std::string dir() const` 





#### `public inline std::string filename() const` 





#### `public inline int rotationInterval() const` 





#### `public inline void setDir(const std::string & dir)` 





#### `public inline void setExtension(const std::string & ext)` 





#### `public inline void setRotationInterval(int interval)` 





#### `protected std::ofstream * _fstream` 





#### `protected std::string _dir` 





#### `protected std::string _filename` 





#### `protected std::string _extension` 





#### `protected int _rotationInterval` 

The log rotation interval in seconds.



#### `protected time_t _rotatedAt` 

The time the log was last rotated.



# class `scy::RunnableQueue` 

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
`protected mutable `[`Mutex`](#classscy_1_1Mutex)` _mutex` | queue_t _queue;
`protected  RunnableQueue(const `[`RunnableQueue`](#classscy_1_1RunnableQueue)` &)` | 
`protected `[`RunnableQueue`](#classscy_1_1RunnableQueue)` & operator=(const `[`RunnableQueue`](#classscy_1_1RunnableQueue)` &)` | 
`protected inline virtual T * popNext()` | 
`protected inline virtual bool dispatchNext()` | 

## Members

#### `public std::function< void(T &)> ondispatch` 

Must be set before the queue is running.

The default dispatch function.

#### `public inline  RunnableQueue(int limit,int timeout)` 





#### `public inline virtual  ~RunnableQueue()` 





#### `public inline virtual void push(T * item)` 

The queue takes ownership of the item pointer.



#### `public inline virtual void flush()` 





#### `public inline void clear()` 





#### `public inline virtual void run()` 



bool empty() /// { /// // Disabling mutex lock for bool check. //Mutex::ScopedLock lock(_mutex); /// return empty(); /// } If not timeout is set this method blocks until [cancel()](#group__base_1gadf150b3fdcef33cb5144119460c8729a) // is called, otherwise [runTimeout()](#group__base_1ga834803a488030d079458bc67563ce927) will be called. Pseudo protected for std::bind compatability.

#### `public inline virtual void runTimeout()` 



until the queue is empty or the timeout expires. Pseudo protected for std::bind compatability.

#### `public inline virtual void dispatch(T & item)` 





#### `public inline void setTimeout(int milliseconds)` 

int timeout() /// { /// Mutex::ScopedLock lock(_mutex); /// return _timeout; /// }



#### `protected int _limit` 





#### `protected int _timeout` 





#### `protected mutable `[`Mutex`](#classscy_1_1Mutex)` _mutex` 

queue_t _queue;



#### `protected  RunnableQueue(const `[`RunnableQueue`](#classscy_1_1RunnableQueue)` &)` 





#### `protected `[`RunnableQueue`](#classscy_1_1RunnableQueue)` & operator=(const `[`RunnableQueue`](#classscy_1_1RunnableQueue)` &)` 





#### `protected inline virtual T * popNext()` 





#### `protected inline virtual bool dispatchNext()` 





# class `scy::ScopedConfiguration` 




[ScopedConfiguration](#classscy_1_1ScopedConfiguration) provides multiple levels of configuration for a module. Multiple levels means that there is a module level scope, and a default scope. When a property is accessed, the module scope value will be used if available, otherwise the default scope value will be used.

Example scoping: Module: channels.[name].modes.[name].[value] Default: modes.[name].[value]

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`Configuration`](#classscy_1_1Configuration)` & config` | 
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

#### `public `[`Configuration`](#classscy_1_1Configuration)` & config` 





#### `public std::string currentScope` 





#### `public std::string defaultScope` 





#### `public  ScopedConfiguration(`[`Configuration`](#classscy_1_1Configuration)` & config,const std::string & currentScope,const std::string & defaultScope)` 





#### `public  ScopedConfiguration(const `[`ScopedConfiguration`](#classscy_1_1ScopedConfiguration)` & that)` 





#### `public std::string getString(const std::string & key,const std::string & defaultValue,bool forceDefaultScope) const` 





#### `public int getInt(const std::string & key,int defaultValue,bool forceDefaultScope) const` 





#### `public double getDouble(const std::string & key,double defaultValue,bool forceDefaultScope) const` 





#### `public bool getBool(const std::string & key,bool defaultValue,bool forceDefaultScope) const` 





#### `public void setString(const std::string & key,const std::string & value,bool defaultScope)` 





#### `public void setInt(const std::string & key,int value,bool defaultScope)` 





#### `public void setDouble(const std::string & key,double value,bool defaultScope)` 





#### `public void setBool(const std::string & key,bool value,bool defaultScope)` 





#### `public std::string getCurrentScope(const std::string & key) const` 





#### `public std::string getDafaultKey(const std::string & key) const` 





#### `public std::string getScopedKey(const std::string & key,bool defaultScope) const` 





# class `scy::ScopedLock` 




[ScopedLock](#classscy_1_1ScopedLock) simplifies thread synchronization with a [Mutex](#classscy_1_1Mutex) or similar lockable object. The given [Mutex](#classscy_1_1Mutex) is locked in the constructor, /// and unlocked it in the destructor. T can be any class with lock() and unlock() functions.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  explicit ScopedLock(T & m)` | 
`public inline  ~ScopedLock()` | 

## Members

#### `public inline  explicit ScopedLock(T & m)` 





#### `public inline  ~ScopedLock()` 





# class `scy::ScopedPointer` 


Scoped Pointer Classes.

[ScopedPointer](#classscy_1_1ScopedPointer) provides an interface for holding and ansynchronously deleting a pointer in various ways.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  ScopedPointer()` | 
`public inline virtual  ~ScopedPointer()` | 

## Members

#### `public inline  ScopedPointer()` 





#### `public inline virtual  ~ScopedPointer()` 





# class `scy::ScopedRawPointer` 

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

#### `public void * ptr` 





#### `public inline  ScopedRawPointer(void * p)` 





#### `public inline virtual  ~ScopedRawPointer()` 





# class `scy::ScopedSharedPointer` 

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

#### `public std::shared_ptr< T > ptr` 





#### `public inline  ScopedSharedPointer(std::shared_ptr< T > p)` 





#### `public inline virtual  ~ScopedSharedPointer()` 





# class `scy::SharedObject` 




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

#### `public inline  SharedObject(bool deferred)` 



Creates the [SharedObject](#classscy_1_1SharedObject) with an initial reference count of one.

#### `public inline void duplicate()` 

Increment the object's reference count.



#### `public inline void release()` 



Decrement the object's reference count and calls delete if the count reaches zero.

#### `public inline unsigned refCount() const` 





#### `protected std::atomic< unsigned > count` 





#### `protected bool deferred` 





#### `protected inline virtual void freeMemory()` 



Deletes the instance when the reference count reaches zero. This method can be overridden for different deletion strategies.

#### `protected inline virtual  ~SharedObject()` 



Destroys the [SharedObject](#classscy_1_1SharedObject). The destructor should never be called directly.

#### `protected  SharedObject(const `[`SharedObject`](#classscy_1_1SharedObject)` &)` 





#### `protected `[`SharedObject`](#classscy_1_1SharedObject)` & operator=(const `[`SharedObject`](#classscy_1_1SharedObject)` &)` 





# class `scy::Signal` 

```
class scy::Signal
  : public scy::SignalBase< DelegateBase< P >, P >
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------

## Members

# class `scy::Signal2` 

```
class scy::Signal2
  : public scy::SignalBase< DelegateBase< P, P2 >, P, P2 >
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------

## Members

# class `scy::Signal3` 

```
class scy::Signal3
  : public scy::SignalBase< DelegateBase< P, P2, P3 >, P, P2, P3 >
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------

## Members

# class `scy::Signal4` 

```
class scy::Signal4
  : public scy::SignalBase< DelegateBase< P, P2, P3, P4 >, P, P2, P3, P4 >
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------

## Members

# class `scy::SignalBase` 




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
`protected mutable `[`Mutex`](#classscy_1_1Mutex)` _mutex` | 

## Members

#### `public inline  SignalBase()` 





#### `public inline virtual  ~SignalBase()` 





#### `public inline void operator+=(const DelegateT & delegate)` 





#### `public inline void operator-=(const DelegateT & delegate)` 





#### `public inline void operator-=(const void * klass)` 





#### `public inline void attach(const DelegateT & delegate)` 



Attaches a delegate to the signal. If the delegate already exists it will overwrite the previous delegate.

#### `public inline bool detach(const DelegateT & delegate)` 



Detaches a delegate from the signal. Returns true if the delegate was detached, false otherwise.

#### `public inline void detach(const void * klass)` 

Detaches all delegates associated with the given class instance.



#### `public inline void cleanup()` 

Deletes cancelled delegates.



#### `public inline void obtain(DelegateList & active)` 



Retrieves a list of active delegates while simultaneously deleting any redundant delegates.

#### `public inline virtual void emit(void * sender)` 





#### `public inline virtual void emit(void * sender,P arg)` 





#### `public inline virtual void emit(void * sender,P arg,P2 arg2)` 





#### `public inline virtual void emit(void * sender,P arg,P2 arg2,P3 arg3)` 





#### `public inline virtual void emit(void * sender,P arg,P2 arg2,P3 arg3,P4 arg4)` 





#### `public inline void clear()` 





#### `public inline void enable(bool flag)` 





#### `public inline bool enabled() const` 





#### `public inline DelegateList delegates() const` 





#### `public inline int ndelegates() const` 



Returns the number of delegates connected to the signal. Use this instead of delegates().size() since the container is not updated in real time.

#### `protected DelegateList _delegates` 





#### `protected bool _enabled` 





#### `protected bool _dirty` 





#### `protected int _count` 





#### `protected mutable `[`Mutex`](#classscy_1_1Mutex)` _mutex` 





# class `scy::Singleton` 




This is a helper template class for managing singleton objects allocated on the heap.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  Singleton()` | Creates the [Singleton](#classscy_1_1Singleton) wrapper.
`public inline  ~Singleton()` | 
`public inline S * get()` | 
`public inline S * swap(S * newPtr)` | 
`public inline void destroy()` | Destroys the managed singleton instance.

## Members

#### `public inline  Singleton()` 

Creates the [Singleton](#classscy_1_1Singleton) wrapper.



#### `public inline  ~Singleton()` 



Destroys the [Singleton](#classscy_1_1Singleton) wrapper and the managed singleton instance it holds.

#### `public inline S * get()` 



Returns a pointer to the singleton object hold by the [Singleton](#classscy_1_1Singleton). The first call to get on a nullptr singleton will instantiate the singleton.

#### `public inline S * swap(S * newPtr)` 



Swaps the old pointer with the new one and returns the old instance.

#### `public inline void destroy()` 

Destroys the managed singleton instance.



# class `scy::State` 






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

#### `public  State(ID id,const std::string & message)` 





#### `public inline virtual  ~State()` 





#### `public virtual ID id() const` 





#### `public virtual void set(ID id)` 





#### `public virtual std::string message() const` 





#### `public virtual void setMessage(const std::string & message)` 





#### `public virtual std::string str(ID id) const` 





#### `public virtual std::string toString() const` 





#### `public inline virtual bool between(ID lid,ID rid) const` 





#### `public inline virtual bool equals(ID id) const` 





#### `public inline bool operator==(const `[`State`](#classscy_1_1State)` & r)` 





#### `public inline bool operator==(const State::ID & r)` 





#### `protected ID _id` 





#### `protected std::string _message` 





# class `scy::Stateful` 




This class implements a simple state machine. T should be a derived [State](#classscy_1_1State) type.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`Signal2`](#classscy_1_1Signal2)`< T &, const T & > StateChange` | Signals when the state changes.
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

#### `public `[`Signal2`](#classscy_1_1Signal2)`< T &, const T & > StateChange` 

Signals when the state changes.



#### `public inline  Stateful()` 





#### `public inline virtual  ~Stateful()` 





#### `public inline virtual bool stateEquals(unsigned int id) const` 





#### `public inline virtual bool stateBetween(unsigned int lid,unsigned int rid) const` 





#### `public inline virtual T & state()` 





#### `public inline virtual const T state() const` 





#### `protected T _state` 





#### `protected inline virtual bool beforeStateChange(const T & state)` 



Override to handle pre state change logic. Return false to prevent state change.

#### `protected inline virtual void onStateChange(T &,const T &)` 

Override to handle post state change logic.



#### `protected inline virtual bool setState(void * sender,unsigned int id,const std::string & message)` 



Sets the state and sends the state signal if the state change was successful.

#### `protected inline virtual bool setState(void * sender,const T & state)` 



Sets the state and sends the state signal if the state change was successful.

#### `protected inline virtual void setStateMessage(const std::string & message)` 





# class `scy::StateSignal` 

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

#### `public  StateSignal(ID id)` 





#### `public  StateSignal(const `[`StateSignal`](#classscy_1_1StateSignal)` & r)` 





#### `public inline virtual  ~StateSignal()` 





#### `public virtual bool change(ID id)` 





#### `public virtual bool canChange(ID id)` 





#### `public virtual void onChange(ID id,ID prev)` 





#### `protected virtual void set(ID id)` 



Fired when the state changes to signal the new and previous states.

# class `scy::StopPropagation` 

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

#### `public inline virtual  ~StopPropagation()` 





# class `scy::Stopwatch` 




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

#### `public  Stopwatch()` 





#### `public  ~Stopwatch()` 





#### `public void start()` 

Starts (or restarts) the stopwatch.



#### `public void stop()` 

Stops or pauses the stopwatch.



#### `public void reset()` 

Resets the stopwatch.



#### `public void restart()` 

Resets and starts the stopwatch.



#### `public `[`Timestamp::TimeDiff`](#group__base_1ga9f06831e4e9b9cbc6f061cc61dc91fd0)` elapsed() const` 



Returns the elapsed time in microseconds since the stopwatch started.

#### `public int elapsedSeconds() const` 



Returns the number of seconds elapsed since the stopwatch started.

#### `public int elapsedMilliseconds() const` 



Returns the number of milliseconds elapsed since the stopwatch started.

# class `scy::Stream` 

```
class scy::Stream
  : public scy::uv::Handle
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`Signal2`](#classscy_1_1Signal2)`< const char *, const int & > Read` | Signals when data can be read from the stream.
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

#### `public `[`Signal2`](#classscy_1_1Signal2)`< const char *, const int & > Read` 

Signals when data can be read from the stream.



#### `public inline  Stream(uv::Loop * loop,void * stream)` 





#### `public inline virtual void close()` 



Closes and resets the stream handle. This will close the active socket/pipe and destroy the uv_stream_t handle.

If the stream is already closed this call will have no side-effects.

#### `public inline bool shutdown()` 



Sends a shutdown packet to the connected peer. Returns true if the shutdown packet was sent.

#### `public inline bool write(const char * data,std::size_t len)` 



Writes data to the stream.

Returns false if the underlying socket is closed. This method does not throw an exception.

#### `public inline Buffer & buffer()` 





#### `public inline virtual bool closed() const` 

Returns true if the native socket handle is closed.



#### `protected Buffer _buffer` 





#### `protected inline virtual bool readStart()` 





#### `protected inline virtual bool readStop()` 





#### `protected inline virtual void onRead(const char * data,std::size_t len)` 





#### `protected inline virtual  ~Stream()` 





#### `protected inline virtual void * self()` 





# class `scy::StreamWriter` 

```
class scy::StreamWriter
  : public scy::PacketProcessor
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`PacketSignal`](#classscy_1_1SignalBase)` emitter` | 
`public inline  StreamWriter(std::ostream * stream)` | 
`public inline virtual  ~StreamWriter()` | 
`public inline virtual void process(`[`IPacket`](#classscy_1_1IPacket)` & packet)` | 
`public template<class StreamT>`  <br/>`inline StreamT & stream()` | 
`public inline std::ostream & stream()` | 
`protected std::ostream * _ostream` | 

## Members

#### `public `[`PacketSignal`](#classscy_1_1SignalBase)` emitter` 





#### `public inline  StreamWriter(std::ostream * stream)` 





#### `public inline virtual  ~StreamWriter()` 





#### `public inline virtual void process(`[`IPacket`](#classscy_1_1IPacket)` & packet)` 



This method performs processing on the given packet and emits the result.

Note: If packet processing is async (the packet is not in the current thread scope) then packet data must be copied. Copied data can be freed directly aFter the async call to emit() the outgoing packet.

#### `public template<class StreamT>`  <br/>`inline StreamT & stream()` 





#### `public inline std::ostream & stream()` 





#### `protected std::ostream * _ostream` 





# class `scy::SyncContext` 

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
`protected `[`uv::Handle`](#classscy_1_1uv_1_1Handle)` _handle` | 
`protected virtual void startAsync()` | Start the context from the control thread.
`protected virtual bool async() const` | 

## Members

#### `public  SyncContext(uv::Loop * loop)` 



Create the synchronization context the given event loop and method. The target method will be called from the event loop context.

#### `public  SyncContext(uv::Loop * loop,std::function< void()> target)` 





#### `public  SyncContext(uv::Loop * loop,std::function< void(void *)> target,void * arg)` 





#### `public virtual  ~SyncContext()` 





#### `public void post()` 



Send a synchronization request to the event loop. Call this each time you want the target method called synchronously. The synchronous method will be called on next iteration. This is not atomic, so do not expect a callback for every request.

#### `public virtual void cancel()` 





#### `public virtual void close()` 





#### `public virtual bool closed()` 





#### `public `[`uv::Handle`](#classscy_1_1uv_1_1Handle)` & handle()` 





#### `protected `[`uv::Handle`](#classscy_1_1uv_1_1Handle)` _handle` 





#### `protected virtual void startAsync()` 

Start the context from the control thread.



#### `protected virtual bool async() const` 



Returns true if the implementation is thread-based, or false if it belongs to an event loop.

# class `scy::SyncPacketQueue` 

```
class scy::SyncPacketQueue
  : public scy::SyncQueue< T >
  : public scy::PacketProcessor
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`PacketSignal`](#classscy_1_1SignalBase)` emitter` | 
`public inline  SyncPacketQueue(uv::Loop * loop,int maxSize)` | 
`public inline  SyncPacketQueue(int maxSize)` | 
`public inline virtual  ~SyncPacketQueue()` | 
`public inline virtual void process(`[`IPacket`](#classscy_1_1IPacket)` & packet)` | 
`public inline virtual bool accepts(`[`IPacket`](#classscy_1_1IPacket)` &)` | 
`protected inline virtual void dispatch(T & packet)` | 
`protected inline virtual void onStreamStateChange(const `[`PacketStreamState`](#structscy_1_1PacketStreamState)` &)` | 

## Members

#### `public `[`PacketSignal`](#classscy_1_1SignalBase)` emitter` 





#### `public inline  SyncPacketQueue(uv::Loop * loop,int maxSize)` 





#### `public inline  SyncPacketQueue(int maxSize)` 





#### `public inline virtual  ~SyncPacketQueue()` 





#### `public inline virtual void process(`[`IPacket`](#classscy_1_1IPacket)` & packet)` 



This method performs processing on the given packet and emits the result.

Note: If packet processing is async (the packet is not in the current thread scope) then packet data must be copied. Copied data can be freed directly aFter the async call to emit() the outgoing packet.

#### `public inline virtual bool accepts(`[`IPacket`](#classscy_1_1IPacket)` &)` 



This method ensures compatibility with the given packet type. Return false to reject the packet.

#### `protected inline virtual void dispatch(T & packet)` 



Emit should never be called after closure. Any late packets should have been dealt with and dropped by the [run()](#group__base_1ga372c52aa2d9b4c33f9b620dfb2c2af23) function.

#### `protected inline virtual void onStreamStateChange(const `[`PacketStreamState`](#structscy_1_1PacketStreamState)` &)` 



Called by the [PacketStream](#classscy_1_1PacketStream) to notify when the internal [Stream](#classscy_1_1Stream) state changes. On receiving the Stopped state, it is the responsibility of the adapter to have ceased all outgoing packet transmission, especially in multi-thread scenarios.

# class `scy::SyncQueue` 

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
`public inline virtual void push(T * item)` | Item pointers are now managed by the [SyncQueue](#classscy_1_1SyncQueue).
`public inline virtual void cancel()` | 
`public inline `[`SyncContext`](#classscy_1_1SyncContext)` & sync()` | 
`protected `[`SyncContext`](#classscy_1_1SyncContext)` _sync` | 

## Members

#### `public inline  SyncQueue(uv::Loop * loop,int limit,int timeout)` 





#### `public inline virtual  ~SyncQueue()` 

time for all callbacks to return.



#### `public inline virtual void push(T * item)` 

Item pointers are now managed by the [SyncQueue](#classscy_1_1SyncQueue).



#### `public inline virtual void cancel()` 





#### `public inline `[`SyncContext`](#classscy_1_1SyncContext)` & sync()` 





#### `protected `[`SyncContext`](#classscy_1_1SyncContext)` _sync` 





# class `scy::Task` 

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

#### `public  Task(bool repeat)` 





#### `public virtual void destroy()` 

Sets the task to destroyed state.



#### `public virtual bool destroyed() const` 

Signals that the task should be disposed of.



#### `public virtual bool repeating() const` 



Signals that the task's should be called repeatedly by the [TaskRunner](#classscy_1_1TaskRunner). If this returns false the task will be [cancelled()](#group__base_1ga7efc7639f668b72480625cc0f34f1eaa)

#### `public virtual std::uint32_t id() const` 

Unique task ID.



#### `protected std::uint32_t _id` 





#### `protected bool _repeating` 





#### `protected bool _destroyed` 





#### `protected  Task(const `[`Task`](#classscy_1_1Task)` & task)` 





#### `protected `[`Task`](#classscy_1_1Task)` & operator=(`[`Task`](#classscy_1_1Task)` const &)` 





#### `protected virtual  ~Task()` 

Should remain protected.



#### `protected void run()` 



Called by the [TaskRunner](#classscy_1_1TaskRunner) to run the task. Override this method to implement task action. Returning true means the true should be called again, and false will cause the task to be destroyed. The task will similarly be destroyed id [destroy()](#group__base_1gaecd8a85018f8f75ae802dcbf3749bdef) was called during the current task iteration.

# class `scy::TaskRunner` 

```
class scy::TaskRunner
  : public scy::async::Runnable
```  



The [TaskRunner](#classscy_1_1TaskRunner) is an asynchronous event loop in charge of running one or many tasks.

The [TaskRunner](#classscy_1_1TaskRunner) continually loops through each task in the task list calling the task's [run()](#group__base_1ga07a481d667f1a9c67e881eedcffc10a5) method.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`NullSignal`](#classscy_1_1NullSignal)` Idle` | Fires after completing an iteration of all tasks.
`public `[`NullSignal`](#classscy_1_1NullSignal)` Shutdown` | Signals when the [TaskRunner](#classscy_1_1TaskRunner) is shutting down.
`public  TaskRunner(async::Runner::Ptr runner)` | 
`public virtual  ~TaskRunner()` | 
`public virtual bool start(`[`Task`](#classscy_1_1Task)` * task)` | Starts a task, adding it if it doesn't exist.
`public virtual bool cancel(`[`Task`](#classscy_1_1Task)` * task)` | 
`public virtual bool destroy(`[`Task`](#classscy_1_1Task)` * task)` | Queues a task for destruction.
`public virtual bool exists(`[`Task`](#classscy_1_1Task)` * task) const` | Returns weather or not a task exists.
`public virtual `[`Task`](#classscy_1_1Task)` * get(std::uint32_t id) const` | 
`public virtual void setRunner(async::Runner::Ptr runner)` | 
`public inline virtual const char * className() const` | 
`protected mutable `[`Mutex`](#classscy_1_1Mutex)` _mutex` | 
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

#### `public `[`NullSignal`](#classscy_1_1NullSignal)` Idle` 

Fires after completing an iteration of all tasks.



#### `public `[`NullSignal`](#classscy_1_1NullSignal)` Shutdown` 

Signals when the [TaskRunner](#classscy_1_1TaskRunner) is shutting down.



#### `public  TaskRunner(async::Runner::Ptr runner)` 





#### `public virtual  ~TaskRunner()` 





#### `public virtual bool start(`[`Task`](#classscy_1_1Task)` * task)` 

Starts a task, adding it if it doesn't exist.



#### `public virtual bool cancel(`[`Task`](#classscy_1_1Task)` * task)` 



Cancels a task. The task reference will be managed the [TaskRunner](#classscy_1_1TaskRunner) until the task is destroyed.

#### `public virtual bool destroy(`[`Task`](#classscy_1_1Task)` * task)` 

Queues a task for destruction.



#### `public virtual bool exists(`[`Task`](#classscy_1_1Task)` * task) const` 

Returns weather or not a task exists.



#### `public virtual `[`Task`](#classscy_1_1Task)` * get(std::uint32_t id) const` 



Returns the task pointer matching the given ID, or nullptr if no task exists.

#### `public virtual void setRunner(async::Runner::Ptr runner)` 



Set the asynchronous context for packet processing. This may be a [Thread](#classscy_1_1Thread) or another derivative of Async. Must be set before the stream is activated.

#### `public inline virtual const char * className() const` 





#### `protected mutable `[`Mutex`](#classscy_1_1Mutex)` _mutex` 





#### `protected TaskList _tasks` 





#### `protected async::Runner::Ptr _runner` 





#### `protected virtual void run()` 

Called by the async context to run the next task.



#### `protected virtual bool add(`[`Task`](#classscy_1_1Task)` * task)` 

Adds a task to the runner.



#### `protected virtual bool remove(`[`Task`](#classscy_1_1Task)` * task)` 

Removes a task from the runner.



#### `protected virtual `[`Task`](#classscy_1_1Task)` * next() const` 

Returns the next task to be run.



#### `protected virtual void clear()` 

Destroys and clears all manages tasks.



#### `protected virtual void onAdd(`[`Task`](#classscy_1_1Task)` * task)` 

Called after a task is added.



#### `protected virtual void onStart(`[`Task`](#classscy_1_1Task)` * task)` 

Called after a task is started.



#### `protected virtual void onCancel(`[`Task`](#classscy_1_1Task)` * task)` 

Called after a task is cancelled.



#### `protected virtual void onRemove(`[`Task`](#classscy_1_1Task)` * task)` 

Called after a task is removed.



#### `protected virtual void onRun(`[`Task`](#classscy_1_1Task)` * task)` 

Called after a task has run.



# class `scy::Thread` 

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

#### `public  Thread()` 





#### `public  Thread(`[`async::Runnable`](#classscy_1_1async_1_1Runnable)` & target)` 





#### `public  Thread(std::function< void()> target)` 





#### `public  Thread(std::function< void(void *)> target,void * arg)` 





#### `public virtual  ~Thread()` 





#### `public void join()` 

Waits until the thread exits.



#### `public bool waitForExit(int timeout)` 



Waits until the thread exits. The thread should be cancelled beore calling this method. This method must be called from outside the current thread context or deadlock will ensue.

#### `public uv_thread_t id() const` 

Returns the native thread handle.



#### `protected uv_thread_t _handle` 





#### `protected  Thread(const `[`Thread`](#classscy_1_1Thread)` &)` 





#### `protected `[`Thread`](#classscy_1_1Thread)` & operator=(const `[`Thread`](#classscy_1_1Thread)` &)` 





#### `protected virtual bool async() const` 



Returns true if the implementation is thread-based, or false if it belongs to an event loop.

#### `protected virtual void startAsync()` 

Start the context from the control thread.



# class `scy::ThreadedStreamReader` 

```
class scy::ThreadedStreamReader
  : public scy::PacketStreamAdapter
  : public scy::async::Startable
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`PacketSignal`](#classscy_1_1SignalBase)` emitter` | 
`public inline  ThreadedStreamReader(std::istream * is)` | 
`public inline  ~ThreadedStreamReader()` | 
`public inline virtual void start()` | 
`public inline virtual void stop()` | 
`public template<class StreamT>`  <br/>`inline StreamT & stream()` | 
`public inline std::istream & stream()` | 
`protected `[`Thread`](#classscy_1_1Thread)` _runner` | 
`protected std::istream * _istream` | 

## Members

#### `public `[`PacketSignal`](#classscy_1_1SignalBase)` emitter` 





#### `public inline  ThreadedStreamReader(std::istream * is)` 





#### `public inline  ~ThreadedStreamReader()` 





#### `public inline virtual void start()` 





#### `public inline virtual void stop()` 





#### `public template<class StreamT>`  <br/>`inline StreamT & stream()` 





#### `public inline std::istream & stream()` 





#### `protected `[`Thread`](#classscy_1_1Thread)` _runner` 





#### `protected std::istream * _istream` 





# class `scy::TimedToken` 

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

#### `public  TimedToken(long duration)` 





#### `public  TimedToken(const std::string & id,long duration)` 





#### `public inline std::string id() const` 





#### `public inline bool operator==(const `[`TimedToken`](#classscy_1_1TimedToken)` & r) const` 





#### `public inline bool operator==(const std::string & r) const` 





#### `protected std::string _id` 





# class `scy::Timeout` 


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

#### `public  Timeout(long delay,bool autoStart)` 





#### `public  Timeout(const `[`Timeout`](#classscy_1_1Timeout)` & src)` 





#### `public  ~Timeout()` 





#### `public bool running() const` 





#### `public void start()` 





#### `public void stop()` 





#### `public void reset()` 





#### `public long remaining() const` 





#### `public bool expired() const` 





#### `public inline void setDelay(long delay)` 





#### `public inline clock_t startAt() const` 





#### `public inline long delay() const` 





#### `public `[`Timeout`](#classscy_1_1Timeout)` & operator=(const `[`Timeout`](#classscy_1_1Timeout)` & src)` 





#### `protected clock_t _startAt` 





#### `protected long _delay` 





# class `scy::Timer` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`NullSignal`](#classscy_1_1NullSignal)` Timeout` | 
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
`protected `[`uv::Handle`](#classscy_1_1uv_1_1Handle)` _handle` | 
`protected std::int64_t _timeout` | 
`protected std::int64_t _interval` | 
`protected std::int64_t _count` | 
`protected  Timer(const `[`Timer`](#classscy_1_1Timer)` &)` | 
`protected `[`Timer`](#classscy_1_1Timer)` & operator=(const `[`Timer`](#classscy_1_1Timer)` &)` | 
`protected virtual void init()` | 

## Members

#### `public `[`NullSignal`](#classscy_1_1NullSignal)` Timeout` 





#### `public  Timer(uv::Loop * loop)` 





#### `public virtual  ~Timer()` 





#### `public virtual void start(std::int64_t interval)` 





#### `public virtual void start(std::int64_t timeout,std::int64_t interval)` 



Starts the timer, an interval value of zero will only trigger once after timeout.

#### `public virtual void stop()` 

Stops the timer.



#### `public virtual void restart()` 



Restarts the timer, even if it hasn't been started yet. An interval or interval must be set or an exception will be thrown.

#### `public virtual void again()` 



Stop the timer, and if it is repeating restart it using the repeat value as the timeout. If the timer has never been started before it returns -1 and sets the error to UV_EINVAL.

#### `public virtual void setInterval(std::int64_t interval)` 



Set the repeat value. Note that if the repeat value is set from a timer callback it does not immediately take effect. If the timer was non-repeating before, it will have been stopped. If it was repeating, then the old repeat value will have been used to schedule the next timeout.

#### `public bool active() const` 





#### `public std::int64_t timeout() const` 





#### `public std::int64_t interval() const` 





#### `public std::int64_t count()` 





#### `public `[`uv::Handle`](#classscy_1_1uv_1_1Handle)` & handle()` 





#### `protected `[`uv::Handle`](#classscy_1_1uv_1_1Handle)` _handle` 





#### `protected std::int64_t _timeout` 





#### `protected std::int64_t _interval` 





#### `protected std::int64_t _count` 





#### `protected  Timer(const `[`Timer`](#classscy_1_1Timer)` &)` 





#### `protected `[`Timer`](#classscy_1_1Timer)` & operator=(const `[`Timer`](#classscy_1_1Timer)` &)` 





#### `protected virtual void init()` 





# class `scy::Timespan` 


A class that represents time spans up to microsecond resolution.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Timespan()` | Creates a zero [Timespan](#classscy_1_1Timespan).
`public  Timespan(TimeDiff microseconds)` | Creates a [Timespan](#classscy_1_1Timespan).
`public  Timespan(long seconds,long microseconds)` | 
`public  Timespan(int days,int hours,int minutes,int seconds,int microseconds)` | Creates a [Timespan](#classscy_1_1Timespan).
`public  Timespan(const `[`Timespan`](#classscy_1_1Timespan)` & timespan)` | Creates a [Timespan](#classscy_1_1Timespan) from another one.
`public  ~Timespan()` | Destroys the [Timespan](#classscy_1_1Timespan).
`public `[`Timespan`](#classscy_1_1Timespan)` & operator=(const `[`Timespan`](#classscy_1_1Timespan)` & timespan)` | Assignment operator.
`public `[`Timespan`](#classscy_1_1Timespan)` & operator=(TimeDiff microseconds)` | Assignment operator.
`public `[`Timespan`](#classscy_1_1Timespan)` & assign(int days,int hours,int minutes,int seconds,int microseconds)` | Assigns a new span.
`public `[`Timespan`](#classscy_1_1Timespan)` & assign(long seconds,long microseconds)` | 
`public void swap(`[`Timespan`](#classscy_1_1Timespan)` & timespan)` | Swaps the [Timespan](#classscy_1_1Timespan) with another one.
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

#### `public  Timespan()` 

Creates a zero [Timespan](#classscy_1_1Timespan).



#### `public  Timespan(TimeDiff microseconds)` 

Creates a [Timespan](#classscy_1_1Timespan).



#### `public  Timespan(long seconds,long microseconds)` 



Creates a [Timespan](#classscy_1_1Timespan). Useful for creating a [Timespan](#classscy_1_1Timespan) from a struct timeval.

#### `public  Timespan(int days,int hours,int minutes,int seconds,int microseconds)` 

Creates a [Timespan](#classscy_1_1Timespan).



#### `public  Timespan(const `[`Timespan`](#classscy_1_1Timespan)` & timespan)` 

Creates a [Timespan](#classscy_1_1Timespan) from another one.



#### `public  ~Timespan()` 

Destroys the [Timespan](#classscy_1_1Timespan).



#### `public `[`Timespan`](#classscy_1_1Timespan)` & operator=(const `[`Timespan`](#classscy_1_1Timespan)` & timespan)` 

Assignment operator.



#### `public `[`Timespan`](#classscy_1_1Timespan)` & operator=(TimeDiff microseconds)` 

Assignment operator.



#### `public `[`Timespan`](#classscy_1_1Timespan)` & assign(int days,int hours,int minutes,int seconds,int microseconds)` 

Assigns a new span.



#### `public `[`Timespan`](#classscy_1_1Timespan)` & assign(long seconds,long microseconds)` 



Assigns a new span. Useful for assigning from a struct timeval.

#### `public void swap(`[`Timespan`](#classscy_1_1Timespan)` & timespan)` 

Swaps the [Timespan](#classscy_1_1Timespan) with another one.



#### `public inline bool operator==(const `[`Timespan`](#classscy_1_1Timespan)` & ts) const` 





#### `public inline bool operator!=(const `[`Timespan`](#classscy_1_1Timespan)` & ts) const` 





#### `public inline bool operator>(const `[`Timespan`](#classscy_1_1Timespan)` & ts) const` 





#### `public inline bool operator>=(const `[`Timespan`](#classscy_1_1Timespan)` & ts) const` 





#### `public inline bool operator<(const `[`Timespan`](#classscy_1_1Timespan)` & ts) const` 





#### `public inline bool operator<=(const `[`Timespan`](#classscy_1_1Timespan)` & ts) const` 





#### `public inline bool operator==(TimeDiff microseconds) const` 





#### `public inline bool operator!=(TimeDiff microseconds) const` 





#### `public inline bool operator>(TimeDiff microseconds) const` 





#### `public inline bool operator>=(TimeDiff microseconds) const` 





#### `public inline bool operator<(TimeDiff microseconds) const` 





#### `public inline bool operator<=(TimeDiff microseconds) const` 





#### `public `[`Timespan`](#classscy_1_1Timespan)` operator+(const `[`Timespan`](#classscy_1_1Timespan)` & d) const` 





#### `public `[`Timespan`](#classscy_1_1Timespan)` operator-(const `[`Timespan`](#classscy_1_1Timespan)` & d) const` 





#### `public `[`Timespan`](#classscy_1_1Timespan)` & operator+=(const `[`Timespan`](#classscy_1_1Timespan)` & d)` 





#### `public `[`Timespan`](#classscy_1_1Timespan)` & operator-=(const `[`Timespan`](#classscy_1_1Timespan)` & d)` 





#### `public `[`Timespan`](#classscy_1_1Timespan)` operator+(TimeDiff microseconds) const` 





#### `public `[`Timespan`](#classscy_1_1Timespan)` operator-(TimeDiff microseconds) const` 





#### `public `[`Timespan`](#classscy_1_1Timespan)` & operator+=(TimeDiff microseconds)` 





#### `public `[`Timespan`](#classscy_1_1Timespan)` & operator-=(TimeDiff microseconds)` 





#### `public inline int days() const` 

Returns the number of days.



#### `public inline int hours() const` 

Returns the number of hours (0 to 23).



#### `public inline int totalHours() const` 

Returns the total number of hours.



#### `public inline int minutes() const` 

Returns the number of minutes (0 to 59).



#### `public inline int totalMinutes() const` 

Returns the total number of minutes.



#### `public inline int seconds() const` 

Returns the number of seconds (0 to 59).



#### `public inline int totalSeconds() const` 

Returns the total number of seconds.



#### `public inline int milliseconds() const` 

Returns the number of milliseconds (0 to 999).



#### `public inline TimeDiff totalMilliseconds() const` 

Returns the total number of milliseconds.



#### `public inline int microseconds() const` 



Returns the fractions of a millisecond in microseconds (0 to 999).

#### `public inline int useconds() const` 



Returns the fractions of a second in microseconds (0 to 999999).

#### `public inline TimeDiff totalMicroseconds() const` 

Returns the total number of microseconds.



# class `scy::Timestamp` 




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
`public void swap(`[`Timestamp`](#classscy_1_1Timestamp)` & timestamp)` | Swaps the [Timestamp](#classscy_1_1Timestamp) with another one.
`public void update()` | Updates the [Timestamp](#classscy_1_1Timestamp) with the current time.
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

#### `public  Timestamp()` 

Creates a timestamp with the current time.



#### `public  Timestamp(`[`TimeVal`](#group__base_1ga7da2b2da9cd1e096cbb146da027f17dc)` tv)` 

Creates a timestamp from the given time value.



#### `public  Timestamp(const `[`Timestamp`](#classscy_1_1Timestamp)` & other)` 

Copy constructor.



#### `public  ~Timestamp()` 

Destroys the timestamp.



#### `public `[`Timestamp`](#classscy_1_1Timestamp)` & operator=(const `[`Timestamp`](#classscy_1_1Timestamp)` & other)` 





#### `public `[`Timestamp`](#classscy_1_1Timestamp)` & operator=(`[`TimeVal`](#group__base_1ga7da2b2da9cd1e096cbb146da027f17dc)` tv)` 





#### `public void swap(`[`Timestamp`](#classscy_1_1Timestamp)` & timestamp)` 

Swaps the [Timestamp](#classscy_1_1Timestamp) with another one.



#### `public void update()` 

Updates the [Timestamp](#classscy_1_1Timestamp) with the current time.



#### `public inline bool operator==(const `[`Timestamp`](#classscy_1_1Timestamp)` & ts) const` 





#### `public inline bool operator!=(const `[`Timestamp`](#classscy_1_1Timestamp)` & ts) const` 





#### `public inline bool operator>(const `[`Timestamp`](#classscy_1_1Timestamp)` & ts) const` 





#### `public inline bool operator>=(const `[`Timestamp`](#classscy_1_1Timestamp)` & ts) const` 





#### `public inline bool operator<(const `[`Timestamp`](#classscy_1_1Timestamp)` & ts) const` 





#### `public inline bool operator<=(const `[`Timestamp`](#classscy_1_1Timestamp)` & ts) const` 





#### `public inline `[`Timestamp`](#classscy_1_1Timestamp)` operator+(`[`TimeDiff`](#group__base_1ga9f06831e4e9b9cbc6f061cc61dc91fd0)` d) const` 





#### `public inline `[`Timestamp`](#classscy_1_1Timestamp)` operator-(`[`TimeDiff`](#group__base_1ga9f06831e4e9b9cbc6f061cc61dc91fd0)` d) const` 





#### `public inline `[`TimeDiff`](#group__base_1ga9f06831e4e9b9cbc6f061cc61dc91fd0)` operator-(const `[`Timestamp`](#classscy_1_1Timestamp)` & ts) const` 





#### `public inline `[`Timestamp`](#classscy_1_1Timestamp)` & operator+=(`[`TimeDiff`](#group__base_1ga9f06831e4e9b9cbc6f061cc61dc91fd0)` d)` 





#### `public inline `[`Timestamp`](#classscy_1_1Timestamp)` & operator-=(`[`TimeDiff`](#group__base_1ga9f06831e4e9b9cbc6f061cc61dc91fd0)` d)` 





#### `public inline std::time_t epochTime() const` 



Returns the timestamp expressed in time_t. time_t base time is midnight, January 1, 1970. Resolution is one second.

#### `public inline `[`UtcTimeVal`](#group__base_1gaec8ace0f25db0ce91c10b30270c3dc24)` utcTime() const` 



Returns the timestamp expressed in UTC-based time. UTC base time is midnight, October 15, 1582. Resolution is 100 nanoseconds.

#### `public inline `[`TimeVal`](#group__base_1ga7da2b2da9cd1e096cbb146da027f17dc)` epochMicroseconds() const` 



Returns the timestamp expressed in microseconds since the Unix epoch, midnight, January 1, 1970.

#### `public inline `[`TimeDiff`](#group__base_1ga9f06831e4e9b9cbc6f061cc61dc91fd0)` elapsed() const` 



Returns the time elapsed since the time denoted by the timestamp. Equivalent to [Timestamp()](#group__base_1ga59eb0caf11a4b42805be30b575c64685) - *this.

#### `public inline bool isElapsed(`[`TimeDiff`](#group__base_1ga9f06831e4e9b9cbc6f061cc61dc91fd0)` interval) const` 



Returns true iff the given interval has passed since the time denoted by the timestamp.

# class `scy::Timezone` 


This class provides information about the current timezone.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------

## Members

# class `scy::TZInfo` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  TZInfo()` | 
`public inline int timeZone()` | 
`public inline const char * name(bool dst)` | 

## Members

#### `public inline  TZInfo()` 





#### `public inline int timeZone()` 





#### `public inline const char * name(bool dst)` 





# struct `scy::Bitwise` 






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

#### `public unsigned data` 





#### `public inline  Bitwise(unsigned flags)` 





#### `public inline virtual void reset()` 





#### `public inline virtual void set(unsigned flag)` 





#### `public inline virtual void add(unsigned flag)` 





#### `public inline virtual void remove(unsigned flag)` 





#### `public inline virtual void toggle(unsigned flag)` 





#### `public inline virtual bool has(unsigned flag) const` 





# struct `scy::DelegateBase` 




The abstract base for all instantiations of the [Delegate](#classscy_1_1Delegate) template classes.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public void * data` | 
`public inline  DelegateBase(DataT data)` | 
`public inline  DelegateBase(const `[`DelegateBase`](#structscy_1_1DelegateBase)` & r)` | 
`public inline virtual  ~DelegateBase()` | 

## Members

#### `public void * data` 





#### `public inline  DelegateBase(DataT data)` 





#### `public inline  DelegateBase(const `[`DelegateBase`](#structscy_1_1DelegateBase)` & r)` 





#### `public inline virtual  ~DelegateBase()` 





# struct `scy::DelegateCallback` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------

## Members

# struct `scy::DelegateCallback< C, 0, false >` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual void emit(void * sender,void *,void *,void *,void *) const` | 

## Members

#### `public inline virtual void emit(void * sender,void *,void *,void *,void *) const` 





# struct `scy::DelegateCallback< C, 0, true >` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual void emit(void * sender,void *,void *,void *,void *) const` | 

## Members

#### `public inline virtual void emit(void * sender,void *,void *,void *,void *) const` 





# struct `scy::DelegateCallback< C, 1, false, P >` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual void emit(void *,P arg,void *,void *,void *) const` | 

## Members

#### `public inline virtual void emit(void *,P arg,void *,void *,void *) const` 





# struct `scy::DelegateCallback< C, 1, true, P >` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual void emit(void * sender,P arg,void *,void *,void *) const` | 

## Members

#### `public inline virtual void emit(void * sender,P arg,void *,void *,void *) const` 





# struct `scy::DelegateCallback< C, 2, false, P, P2 >` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual void emit(void *,P arg,P2 arg2,void *,void *) const` | 

## Members

#### `public inline virtual void emit(void *,P arg,P2 arg2,void *,void *) const` 





# struct `scy::DelegateCallback< C, 2, true, P, P2 >` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual void emit(void * sender,P arg,P2 arg2,void *,void *) const` | 

## Members

#### `public inline virtual void emit(void * sender,P arg,P2 arg2,void *,void *) const` 





# struct `scy::DelegateCallback< C, 3, false, P, P2, P3 >` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual void emit(void *,P arg,P2 arg2,P3 arg3,void *) const` | 

## Members

#### `public inline virtual void emit(void *,P arg,P2 arg2,P3 arg3,void *) const` 





# struct `scy::DelegateCallback< C, 3, true, P, P2, P3 >` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual void emit(void * sender,P arg,P2 arg2,P3 arg3,void *) const` | 

## Members

#### `public inline virtual void emit(void * sender,P arg,P2 arg2,P3 arg3,void *) const` 





# struct `scy::DelegateCallback< C, 4, false, P, P2, P3, P4 >` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual void emit(void *,P arg,P2 arg2,P3 arg3,P4 arg4) const` | 

## Members

#### `public inline virtual void emit(void *,P arg,P2 arg2,P3 arg3,P4 arg4) const` 





# struct `scy::DelegateCallback< C, 4, true, P, P2, P3, P4 >` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual void emit(void * sender,P arg,P2 arg2,P3 arg3,P4 arg4) const` | 

## Members

#### `public inline virtual void emit(void * sender,P arg,P2 arg2,P3 arg3,P4 arg4) const` 





# struct `scy::DelegateCallback< C, 8, false, PolymorphicT >` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual DefineCallbackFields void emit(void *,`[`basic::Polymorphic`](#classscy_1_1basic_1_1Polymorphic)` & data,void *,void *,void *) const` | 

## Members

#### `public inline virtual DefineCallbackFields void emit(void *,`[`basic::Polymorphic`](#classscy_1_1basic_1_1Polymorphic)` & data,void *,void *,void *) const` 





# struct `scy::DelegateCallback< C, 8, true, PolymorphicT >` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual DefineCallbackFields void emit(void * sender,`[`basic::Polymorphic`](#classscy_1_1basic_1_1Polymorphic)` & data,void *,void *,void *) const` | 

## Members

#### `public inline virtual DefineCallbackFields void emit(void * sender,`[`basic::Polymorphic`](#classscy_1_1basic_1_1Polymorphic)` & data,void *,void *,void *) const` 





# struct `scy::Error` 






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

#### `public int errorno` 





#### `public std::string message` 





#### `public std::exception_ptr exception` 





#### `public inline  Error()` 





#### `public inline  Error(const std::string & msg)` 





#### `public inline  Error(const char * msg)` 





#### `public inline bool any() const` 





#### `public inline void reset()` 





#### `public inline void rethrow()` 





# struct `scy::IPacketInfo` 




An abstract interface for packet sources to provide extra information about packets.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  IPacketInfo()` | 
`public inline virtual  ~IPacketInfo()` | 
`public `[`IPacketInfo`](#structscy_1_1IPacketInfo)` * clone() const` | 

## Members

#### `public inline  IPacketInfo()` 





#### `public inline virtual  ~IPacketInfo()` 





#### `public `[`IPacketInfo`](#structscy_1_1IPacketInfo)` * clone() const` 





# struct `scy::LogStream` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public LogLevel level` | 
`public int line` | 
`public std::string realm` | 
`public std::string address` | 
`public std::ostringstream message` | 
`public std::time_t ts` | 
`public `[`LogChannel`](#classscy_1_1LogChannel)` * channel` | 
`public  LogStream(LogLevel level,const std::string & realm,int line,const void * ptr,const char * channel)` | 
`public  LogStream(LogLevel level,const std::string & realm,const std::string & address)` | 
`public  LogStream(const `[`LogStream`](#structscy_1_1LogStream)` & that)` | 
`public  ~LogStream()` | 
`public inline `[`LogStream`](#structscy_1_1LogStream)` & operator<<(const LogLevel data)` | 
`public inline `[`LogStream`](#structscy_1_1LogStream)` & operator<<(`[`LogChannel`](#classscy_1_1LogChannel)` * data)` | 
`public template<typename T>`  <br/>`inline `[`LogStream`](#structscy_1_1LogStream)` & operator<<(const T & data)` | 
`public inline `[`LogStream`](#structscy_1_1LogStream)` & operator<<(std::ostream &(*)(std::ostream &) f)` | 

## Members

#### `public LogLevel level` 





#### `public int line` 





#### `public std::string realm` 





#### `public std::string address` 





#### `public std::ostringstream message` 





#### `public std::time_t ts` 





#### `public `[`LogChannel`](#classscy_1_1LogChannel)` * channel` 





#### `public  LogStream(LogLevel level,const std::string & realm,int line,const void * ptr,const char * channel)` 





#### `public  LogStream(LogLevel level,const std::string & realm,const std::string & address)` 





#### `public  LogStream(const `[`LogStream`](#structscy_1_1LogStream)` & that)` 





#### `public  ~LogStream()` 





#### `public inline `[`LogStream`](#structscy_1_1LogStream)` & operator<<(const LogLevel data)` 





#### `public inline `[`LogStream`](#structscy_1_1LogStream)` & operator<<(`[`LogChannel`](#classscy_1_1LogChannel)` * data)` 





#### `public template<typename T>`  <br/>`inline `[`LogStream`](#structscy_1_1LogStream)` & operator<<(const T & data)` 





#### `public inline `[`LogStream`](#structscy_1_1LogStream)` & operator<<(std::ostream &(*)(std::ostream &) f)` 



Handle std::endl flags. This method flushes the log message and queues it for write. WARNING: After using std::endl to flush the message pointer should not be accessed.

# struct `scy::OptionParser` 






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

#### `public std::string exepath` 





#### `public OptionMap args` 





#### `public  OptionParser(int argc,char * argv,const char * delim)` 





#### `public inline bool has(const char * key)` 





#### `public inline std::string get(const char * key)` 





#### `public template<typename NumericType>`  <br/>`inline NumericType get(const char * key)` 





# struct `scy::PacketAdapterReference` 


For 0.8.x compatibility.

Provides a reference to a PacketSignal instance.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` * ptr` | 
`public `[`ScopedPointer`](#classscy_1_1ScopedPointer)` * deleter` | 
`public int order` | 
`public bool syncState` | 
`public inline  PacketAdapterReference(`[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` * ptr,`[`ScopedPointer`](#classscy_1_1ScopedPointer)` * deleter,int order,bool syncState)` | 
`public inline  ~PacketAdapterReference()` | 

## Members

#### `public `[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` * ptr` 





#### `public `[`ScopedPointer`](#classscy_1_1ScopedPointer)` * deleter` 





#### `public int order` 





#### `public bool syncState` 





#### `public inline  PacketAdapterReference(`[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` * ptr,`[`ScopedPointer`](#classscy_1_1ScopedPointer)` * deleter,int order,bool syncState)` 





#### `public inline  ~PacketAdapterReference()` 





# struct `scy::PacketCreationStrategy` 

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

#### `public inline  PacketCreationStrategy(int priority)` 





#### `public inline virtual  ~PacketCreationStrategy()` 





#### `public inline virtual `[`IPacket`](#classscy_1_1IPacket)` * create(const `[`ConstBuffer`](#classscy_1_1ConstBuffer)` & buffer,std::size_t & nread) const` 





#### `public inline virtual int priority() const` 





#### `protected int _priority` 





# struct `scy::PacketStreamState` 

```
struct scy::PacketStreamState
  : public scy::State
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual std::string str(unsigned int id) const` | 

## Members

#### `public inline virtual std::string str(unsigned int id) const` 





# struct `scy::SharedLibrary` 






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

#### `public inline bool open(const std::string & path)` 



Opens a shared library. The filename is in utf-8. Returns true on success and false on error. Call `SharedLibrary::error()` to get the error message.

#### `public inline void close()` 





#### `public inline bool sym(const char * name,void ** ptr)` 



Retrieves a data pointer from a dynamic library. It is legal for a symbol to map to nullptr. Returns 0 on success and -1 if the symbol was not found.

#### `public inline void setError(const std::string & prefix)` 





#### `public inline std::string error() const` 





#### `protected uv_lib_t _lib` 





#### `protected std::string _error` 





# struct `scy::TransactionState` 

```
struct scy::TransactionState
  : public scy::State
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual std::string str(unsigned int id) const` | 

## Members

#### `public inline virtual std::string str(unsigned int id) const` 





# namespace `scy::async`

Classes for asynchronous programming.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[``scy::async::Flag``](#classscy_1_1async_1_1Flag)    | 
`class `[``scy::async::Runnable``](#classscy_1_1async_1_1Runnable)    | 
`class `[``scy::async::Runner``](#classscy_1_1async_1_1Runner)    | 
`class `[``scy::async::Sendable``](#classscy_1_1async_1_1Sendable)    | 
`class `[``scy::async::Startable``](#classscy_1_1async_1_1Startable)    | 
# class `scy::async::Flag` 




A concurrent flag which can be used to request task cancellation.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  Flag()` | 
`public inline bool get() const` | 
`public inline void set(bool flag)` | 

## Members

#### `public inline  Flag()` 





#### `public inline bool get() const` 





#### `public inline void set(bool flag)` 





# class `scy::async::Runnable` 




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

#### `public inline  Runnable()` 





#### `public inline virtual  ~Runnable()` 





#### `public void run()` 

The run method will be called by the async context.



#### `public inline virtual void cancel(bool flag)` 



Cancel the current task. The [run()](#group__base_1gafa78ae78ab96f591e88e629ac4b7699d) method should return ASAP.

#### `public inline virtual bool cancelled() const` 

True when the task has been cancelled.



# class `scy::async::Runner` 




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
`public bool repeating() const` | Returns true if the [Runner](#classscy_1_1async_1_1Runner) is operating in repeating mode.
`public uv_thread_t tid() const` | Return the native thread ID.
`public void setRepeating(bool flag)` | 
`public bool async() const` | 
`protected Context::ptr pContext` | Shared pointer to the internal [Runner::Context](#structscy_1_1async_1_1Runner_1_1Context).
`protected void startAsync()` | Start the context from the control thread.
`protected  Runner(const `[`Runner`](#classscy_1_1async_1_1Runner)` &)` | 
`protected `[`Runner`](#classscy_1_1async_1_1Runner)` & operator=(const `[`Runner`](#classscy_1_1async_1_1Runner)` &)` | 

## Members

#### `public  Runner()` 





#### `public virtual  ~Runner()` 





#### `public virtual void start(`[`async::Runnable`](#classscy_1_1async_1_1Runnable)` & target)` 





#### `public virtual void start(std::function< void()> target)` 





#### `public virtual void start(std::function< void(void *)> target,void * arg)` 

Starts the thread with the given target.



#### `public bool started() const` 

Returns true if the async context has been started.



#### `public bool running() const` 

Returns true if the async context is currently running.



#### `public void cancel()` 

Cancels the async context.



#### `public bool cancelled() const` 



True when the task has been cancelled. It is up to the implementation to return at the earliest possible time.

#### `public bool repeating() const` 

Returns true if the [Runner](#classscy_1_1async_1_1Runner) is operating in repeating mode.



#### `public uv_thread_t tid() const` 

Return the native thread ID.



#### `public void setRepeating(bool flag)` 



This setting means the implementation should call the target function repeatedly until cancelled. The importance of this method to normalize the functionality of threadded and event loop driven [Runner](#classscy_1_1async_1_1Runner) models.

#### `public bool async() const` 



Returns true if the implementation is thread-based, or false if it belongs to an event loop.

#### `protected Context::ptr pContext` 

Shared pointer to the internal [Runner::Context](#structscy_1_1async_1_1Runner_1_1Context).



#### `protected void startAsync()` 

Start the context from the control thread.



#### `protected  Runner(const `[`Runner`](#classscy_1_1async_1_1Runner)` &)` 





#### `protected `[`Runner`](#classscy_1_1async_1_1Runner)` & operator=(const `[`Runner`](#classscy_1_1async_1_1Runner)` &)` 





# class `scy::async::Sendable` 




A generic interface for classes that can be sent and cancelled.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public bool send()` | 
`public inline virtual void cancel()` | 

## Members

#### `public bool send()` 





#### `public inline virtual void cancel()` 





# class `scy::async::Startable` 




A generic interface for a classes that can be started and stopped.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public void start()` | 
`public void stop()` | 

## Members

#### `public void start()` 





#### `public void stop()` 





# namespace `scy::base64`



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`struct `[``scy::base64::Decoder``](#structscy_1_1base64_1_1Decoder)    | 
`struct `[``scy::base64::Encoder``](#structscy_1_1base64_1_1Encoder)    | 
# struct `scy::base64::Decoder` 

```
struct scy::base64::Decoder
  : public scy::basic::Decoder
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public int _buffersize` | 
`public  _buffersize(buffersize)` | 

## Members

#### `public int _buffersize` 





#### `public  _buffersize(buffersize)` 





# struct `scy::base64::Encoder` 

```
struct scy::base64::Encoder
  : public scy::basic::Encoder
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public int _buffersize` | 
`public  _buffersize(buffersize)` | 

## Members

#### `public int _buffersize` 





#### `public  _buffersize(buffersize)` 





# namespace `scy::hex`



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`struct `[``scy::hex::Decoder``](#structscy_1_1hex_1_1Decoder)    | 
`struct `[``scy::hex::Encoder``](#structscy_1_1hex_1_1Encoder)    | 
# struct `scy::hex::Decoder` 

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

#### `public char lastbyte` 





#### `public inline  Decoder()` 





#### `public inline virtual  ~Decoder()` 





#### `public inline virtual std::size_t decode(const char * inbuf,std::size_t nread,char * outbuf)` 





#### `public inline virtual std::size_t finalize(char *)` 





#### `public inline bool readnext(const char * inbuf,std::size_t nread,std::size_t & rpos,char & c)` 





#### `public inline int nybble(const int n)` 





#### `public inline bool iswspace(const char c)` 





# struct `scy::hex::Encoder` 

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

#### `public int _linePos` 





#### `public int _lineLength` 





#### `public int _uppercase` 





#### `public inline  Encoder()` 





#### `public inline virtual std::size_t encode(const char * inbuf,std::size_t nread,char * outbuf)` 





#### `public inline virtual std::size_t finalize(char *)` 





#### `public inline void setUppercase(bool flag)` 





#### `public inline void setLineLength(int lineLength)` 





# namespace `scy::basic`



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[``scy::basic::Decoder``](#classscy_1_1basic_1_1Decoder)    | 
`class `[``scy::basic::Encoder``](#classscy_1_1basic_1_1Encoder)    | 
`class `[``scy::basic::Polymorphic``](#classscy_1_1basic_1_1Polymorphic)    | A base module class for C++ callback polymorphism.
# class `scy::basic::Decoder` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  Decoder()` | 
`public inline virtual  ~Decoder()` | 
`public std::size_t decode(const char * inbuf,std::size_t nread,char * outbuf)` | 
`public inline virtual std::size_t finalize(char *)` | 

## Members

#### `public inline  Decoder()` 





#### `public inline virtual  ~Decoder()` 





#### `public std::size_t decode(const char * inbuf,std::size_t nread,char * outbuf)` 





#### `public inline virtual std::size_t finalize(char *)` 





# class `scy::basic::Encoder` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  Encoder()` | 
`public inline virtual  ~Encoder()` | 
`public std::size_t encode(const char * inbuf,std::size_t nread,char * outbuf)` | 
`public inline virtual std::size_t finalize(char *)` | 

## Members

#### `public inline  Encoder()` 





#### `public inline virtual  ~Encoder()` 





#### `public std::size_t encode(const char * inbuf,std::size_t nread,char * outbuf)` 





#### `public inline virtual std::size_t finalize(char *)` 





# class `scy::basic::Polymorphic` 


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

#### `public inline virtual  ~Polymorphic()` 





#### `public template<class T>`  <br/>`inline bool is()` 





#### `public template<class T>`  <br/>`inline T * as(bool whiny)` 





#### `public `[`scy::LogStream`](#structscy_1_1LogStream)` & log(const char * level) const` 





#### `public const char * className() const` 





# namespace `scy::ipc`

Classes for inter-process communication.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[``scy::ipc::Queue``](#classscy_1_1ipc_1_1Queue)    | 
`class `[``scy::ipc::SyncQueue``](#classscy_1_1ipc_1_1SyncQueue)    | 
`struct `[``scy::ipc::Action``](#structscy_1_1ipc_1_1Action)    | Default action type for executing synchronized callbacks.
# class `scy::ipc::Queue` 




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
`protected mutable `[`Mutex`](#classscy_1_1Mutex)` _mutex` | 
`protected std::deque< TAction * > _actions` | 

## Members

#### `public inline  Queue()` 





#### `public inline virtual  ~Queue()` 





#### `public inline virtual void push(TAction * action)` 





#### `public inline virtual TAction * pop()` 





#### `public inline virtual void runSync()` 





#### `public inline virtual void close()` 





#### `public inline virtual void post()` 





#### `public inline void waitForSync()` 





#### `protected mutable `[`Mutex`](#classscy_1_1Mutex)` _mutex` 





#### `protected std::deque< TAction * > _actions` 





# class `scy::ipc::SyncQueue` 

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
`protected `[`SyncContext`](#classscy_1_1SyncContext)` _sync` | 

## Members

#### `public inline  SyncQueue(uv::Loop * loop)` 





#### `public inline virtual  ~SyncQueue()` 





#### `public inline virtual void close()` 





#### `public inline virtual void post()` 





#### `public inline virtual `[`SyncContext`](#classscy_1_1SyncContext)` & sync()` 





#### `protected `[`SyncContext`](#classscy_1_1SyncContext)` _sync` 





# struct `scy::ipc::Action` 


Default action type for executing synchronized callbacks.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public callback_t target` | 
`public void * arg` | 
`public std::string data` | 
`public inline  Action(callback_t target,void * arg,const std::string & data)` | 

## Members

#### `public callback_t target` 





#### `public void * arg` 





#### `public std::string data` 





#### `public inline  Action(callback_t target,void * arg,const std::string & data)` 





# namespace `scy::deleter`

Deleter Functors.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`struct `[``scy::deleter::Array``](#structscy_1_1deleter_1_1Array)    | 
`struct `[``scy::deleter::Deferred``](#structscy_1_1deleter_1_1Deferred)    | 
`struct `[``scy::deleter::Dispose``](#structscy_1_1deleter_1_1Dispose)    | 
# struct `scy::deleter::Array` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline void operator()(T * ptr)` | 

## Members

#### `public inline void operator()(T * ptr)` 





# struct `scy::deleter::Deferred` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline void operator()(T * ptr)` | 

## Members

#### `public inline void operator()(T * ptr)` 





# struct `scy::deleter::Dispose` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline void operator()(T * ptr)` | 

## Members

#### `public inline void operator()(T * ptr)` 





# namespace `scy::test`

Modern unit testing classes.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[``scy::test::FunctionTest``](#classscy_1_1test_1_1FunctionTest)    | 
`class `[``scy::test::Test``](#classscy_1_1test_1_1Test)    | 
`class `[``scy::test::TestRunner``](#classscy_1_1test_1_1TestRunner)    | 
# class `scy::test::FunctionTest` 

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
`protected inline virtual void run()` | Called by the [TestRunner](#classscy_1_1test_1_1TestRunner) to run the test.

## Members

#### `public voidfunc_t target` 





#### `public inline  FunctionTest(voidfunc_t target,const std::string & name)` 





#### `protected inline virtual  ~FunctionTest()` 





#### `protected inline virtual void run()` 

Called by the [TestRunner](#classscy_1_1test_1_1TestRunner) to run the test.



# class `scy::test::Test` 




This class is for implementing any kind async test that is compatible with a [TestRunner](#classscy_1_1test_1_1TestRunner).

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::string name` | The name of the test.
`public error_list_t errors` | A list of test errors.
`public double duration` | The test run duration for benchmarking.
`public  Test(const std::string & name)` | 
`public virtual  ~Test()` | Should remain protected.
`public void run()` | Called by the [TestRunner](#classscy_1_1test_1_1TestRunner) to run the test.
`public bool passed()` | Return true when the test passed without errors.
`protected  Test(const `[`Test`](#classscy_1_1test_1_1Test)` & test)` | 
`protected `[`Test`](#classscy_1_1test_1_1Test)` & operator=(`[`Test`](#classscy_1_1test_1_1Test)` const &)` | 

## Members

#### `public std::string name` 

The name of the test.



#### `public error_list_t errors` 

A list of test errors.



#### `public double duration` 

The test run duration for benchmarking.



#### `public  Test(const std::string & name)` 





#### `public virtual  ~Test()` 

Should remain protected.



#### `public void run()` 

Called by the [TestRunner](#classscy_1_1test_1_1TestRunner) to run the test.



#### `public bool passed()` 

Return true when the test passed without errors.



#### `protected  Test(const `[`Test`](#classscy_1_1test_1_1Test)` & test)` 





#### `protected `[`Test`](#classscy_1_1test_1_1Test)` & operator=(`[`Test`](#classscy_1_1test_1_1Test)` const &)` 





# class `scy::test::TestRunner` 




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
`public `[`Test`](#classscy_1_1test_1_1Test)` * current() const` | Return the currently active [Test](#classscy_1_1test_1_1Test) or nullptr.
`public test_list_t tests() const` | Return the list of tests.
`public error_map_t errors() const` | Return a map of tests and errors.
`public bool passed() const` | Return true if all tests passed.
`protected mutable `[`Mutex`](#classscy_1_1Mutex)` _mutex` | 
`protected std::list< `[`Test`](#classscy_1_1test_1_1Test)` * > _tests` | 
`protected `[`Test`](#classscy_1_1test_1_1Test)` * _current` | 

## Members

#### `public  TestRunner()` 





#### `public virtual  ~TestRunner()` 





#### `public void add(`[`Test`](#classscy_1_1test_1_1Test)` * test)` 

Add a test to the runner.



#### `public `[`Test`](#classscy_1_1test_1_1Test)` * get(const std::string & name) const` 



Return a pointer to the test matching the given name, or nullptr if no matching test exists.

#### `public void run()` 

Called by the async context to run the next test.



#### `public void clear()` 

Destroy and clears all managed tests.



#### `public `[`Test`](#classscy_1_1test_1_1Test)` * current() const` 

Return the currently active [Test](#classscy_1_1test_1_1Test) or nullptr.



#### `public test_list_t tests() const` 

Return the list of tests.



#### `public error_map_t errors() const` 

Return a map of tests and errors.



#### `public bool passed() const` 

Return true if all tests passed.



#### `protected mutable `[`Mutex`](#classscy_1_1Mutex)` _mutex` 





#### `protected std::list< `[`Test`](#classscy_1_1test_1_1Test)` * > _tests` 





#### `protected `[`Test`](#classscy_1_1test_1_1Test)` * _current` 





# namespace `scy::util`



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`struct `[``scy::util::Version``](#structscy_1_1util_1_1Version)    | 
# struct `scy::util::Version` 






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

#### `public int major` 





#### `public int minor` 





#### `public int revision` 





#### `public int build` 





#### `public inline  Version(const std::string & version)` 





#### `public inline bool operator<(const `[`Version`](#structscy_1_1util_1_1Version)` & other)` 





#### `public inline bool operator==(const `[`Version`](#structscy_1_1util_1_1Version)` & other)` 





# class `scy::Application` 




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

#### `public uv::Loop * loop` 



The active event loop. May be assigned at construction, otherwise the default event loop is used.

#### `public  Application(uv::Loop * loop)` 





#### `public  ~Application()` 





#### `public void run()` 





#### `public void stop()` 





#### `public void finalize()` 





#### `public void bindShutdownSignal(std::function< void(void *)> callback,void * opaque)` 

Bind the shutdown signal.

Shutdown handling

#### `public void waitForShutdown(std::function< void(void *)> callback,void * opaque)` 

Bind the shutdown signal and run the main event loop.



#### `protected  Application(const `[`Application`](#classscy_1_1Application)` &)` 





#### `protected  Application(`[`Application`](#classscy_1_1Application)` &&)` 





#### `protected `[`Application`](#classscy_1_1Application)` & operator=(const `[`Application`](#classscy_1_1Application)` &)` 





#### `protected `[`Application`](#classscy_1_1Application)` & operator=(`[`Application`](#classscy_1_1Application)` &&)` 





# class `scy::async::Runnable` 




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

#### `public inline  Runnable()` 





#### `public inline virtual  ~Runnable()` 





#### `public void run()` 

The run method will be called by the async context.



#### `public inline virtual void cancel(bool flag)` 



Cancel the current task. The [run()](#group__base_1gafa78ae78ab96f591e88e629ac4b7699d) method should return ASAP.

#### `public inline virtual bool cancelled() const` 

True when the task has been cancelled.



# class `scy::async::Runner` 




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
`public bool repeating() const` | Returns true if the [Runner](#classscy_1_1async_1_1Runner) is operating in repeating mode.
`public uv_thread_t tid() const` | Return the native thread ID.
`public void setRepeating(bool flag)` | 
`public bool async() const` | 
`protected Context::ptr pContext` | Shared pointer to the internal [Runner::Context](#structscy_1_1async_1_1Runner_1_1Context).
`protected void startAsync()` | Start the context from the control thread.
`protected  Runner(const `[`Runner`](#classscy_1_1async_1_1Runner)` &)` | 
`protected `[`Runner`](#classscy_1_1async_1_1Runner)` & operator=(const `[`Runner`](#classscy_1_1async_1_1Runner)` &)` | 

## Members

#### `public  Runner()` 





#### `public virtual  ~Runner()` 





#### `public virtual void start(`[`async::Runnable`](#classscy_1_1async_1_1Runnable)` & target)` 





#### `public virtual void start(std::function< void()> target)` 





#### `public virtual void start(std::function< void(void *)> target,void * arg)` 

Starts the thread with the given target.



#### `public bool started() const` 

Returns true if the async context has been started.



#### `public bool running() const` 

Returns true if the async context is currently running.



#### `public void cancel()` 

Cancels the async context.



#### `public bool cancelled() const` 



True when the task has been cancelled. It is up to the implementation to return at the earliest possible time.

#### `public bool repeating() const` 

Returns true if the [Runner](#classscy_1_1async_1_1Runner) is operating in repeating mode.



#### `public uv_thread_t tid() const` 

Return the native thread ID.



#### `public void setRepeating(bool flag)` 



This setting means the implementation should call the target function repeatedly until cancelled. The importance of this method to normalize the functionality of threadded and event loop driven [Runner](#classscy_1_1async_1_1Runner) models.

#### `public bool async() const` 



Returns true if the implementation is thread-based, or false if it belongs to an event loop.

#### `protected Context::ptr pContext` 

Shared pointer to the internal [Runner::Context](#structscy_1_1async_1_1Runner_1_1Context).



#### `protected void startAsync()` 

Start the context from the control thread.



#### `protected  Runner(const `[`Runner`](#classscy_1_1async_1_1Runner)` &)` 





#### `protected `[`Runner`](#classscy_1_1async_1_1Runner)` & operator=(const `[`Runner`](#classscy_1_1async_1_1Runner)` &)` 





# class `scy::async::Flag` 




A concurrent flag which can be used to request task cancellation.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  Flag()` | 
`public inline bool get() const` | 
`public inline void set(bool flag)` | 

## Members

#### `public inline  Flag()` 





#### `public inline bool get() const` 





#### `public inline void set(bool flag)` 





# class `scy::async::Startable` 




A generic interface for a classes that can be started and stopped.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public void start()` | 
`public void stop()` | 

## Members

#### `public void start()` 





#### `public void stop()` 





# class `scy::async::Sendable` 




A generic interface for classes that can be sent and cancelled.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public bool send()` | 
`public inline virtual void cancel()` | 

## Members

#### `public bool send()` 





#### `public inline virtual void cancel()` 





# class `scy::MutableBuffer` 




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

#### `public inline  MutableBuffer()` 

Construct an empty buffer.



#### `public inline  MutableBuffer(void * data,std::size_t size)` 

Construct a buffer to represent the given memory range.



#### `public inline void * data() const` 





#### `public inline std::size_t size() const` 





#### `public inline char * cstr() const` 

String methods.



#### `public inline std::string str() const` 





# class `scy::ConstBuffer` 




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

#### `public inline  ConstBuffer()` 

Construct an empty buffer.



#### `public inline  ConstBuffer(const void * data,std::size_t size)` 

Construct a buffer to represent the given memory range.



#### `public inline  ConstBuffer(const `[`MutableBuffer`](#classscy_1_1MutableBuffer)` & b)` 

Construct a non-modifiable buffer from a modifiable one.



#### `public inline const void * data() const` 





#### `public inline std::size_t size() const` 





#### `public inline const char * cstr() const` 

String methods.



#### `public inline std::string str() const` 





# class `scy::BitReader` 


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

#### `public  BitReader(const char * bytes,std::size_t size,`[`ByteOrder`](#group__base_1gaa934c0972c743983462716b01df7dc32)` order)` 





#### `public  BitReader(const Buffer & buf,`[`ByteOrder`](#group__base_1gaa934c0972c743983462716b01df7dc32)` order)` 





#### `public  BitReader(const `[`ConstBuffer`](#classscy_1_1ConstBuffer)` & pod,`[`ByteOrder`](#group__base_1gaa934c0972c743983462716b01df7dc32)` order)` 





#### `public  ~BitReader()` 





#### `public void get(char * val,std::size_t len)` 



Reads a value from the [BitReader](#classscy_1_1BitReader). Returns false if there isn't enough data left for the specified type. Throws a std::out_of_range exception if reading past the limit.

#### `public void get(std::string & val,std::size_t len)` 





#### `public void getU8(std::uint8_t & val)` 





#### `public void getU16(std::uint16_t & val)` 





#### `public void getU24(std::uint32_t & val)` 





#### `public void getU32(std::uint32_t & val)` 





#### `public void getU64(std::uint64_t & val)` 





#### `public const char peek()` 





#### `public const std::uint8_t peekU8()` 





#### `public const std::uint16_t peekU16()` 





#### `public const std::uint32_t peekU24()` 





#### `public const std::uint32_t peekU32()` 





#### `public const std::uint64_t peekU64()` 



Peeks data from the [BitReader](#classscy_1_1BitReader). -1 is returned if reading past boundary.

#### `public int skipToChar(char c)` 

String parsing methods.



#### `public int skipWhitespace()` 





#### `public int skipToNextLine()` 





#### `public int skipNextWord()` 





#### `public int readNextWord(std::string & val)` 





#### `public int readNextNumber(unsigned int & val)` 





#### `public int readLine(std::string & val)` 





#### `public int readToNext(std::string & val,char c)` 





#### `public void seek(std::size_t val)` 



Set position pointer to absolute position. Throws a std::out_of_range exception if the value exceeds the limit.

#### `public void skip(std::size_t size)` 



Set position pointer to relative position. Throws a std::out_of_range exception if the value exceeds the limit.

#### `public std::size_t limit() const` 

Returns the read limit.



#### `public inline std::size_t position() const` 

Returns the current read position.



#### `public std::size_t available() const` 

Returns the number of elements between the current position and the limit.



#### `public inline const char * begin() const` 





#### `public inline const char * current() const` 





#### `public inline `[`ByteOrder`](#group__base_1gaa934c0972c743983462716b01df7dc32)` order() const` 





#### `public std::string toString()` 





# class `scy::BitWriter` 




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

#### `public  BitWriter(char * bytes,std::size_t size,`[`ByteOrder`](#group__base_1gaa934c0972c743983462716b01df7dc32)` order)` 





#### `public  BitWriter(Buffer & buf,`[`ByteOrder`](#group__base_1gaa934c0972c743983462716b01df7dc32)` order)` 





#### `public  BitWriter(`[`MutableBuffer`](#classscy_1_1MutableBuffer)` & pod,`[`ByteOrder`](#group__base_1gaa934c0972c743983462716b01df7dc32)` order)` 





#### `public  ~BitWriter()` 





#### `public void put(const char * val,std::size_t len)` 



Append bytes to the buffer. Throws a std::out_of_range exception if reading past the limit.

#### `public void put(const std::string & val)` 





#### `public void putU8(std::uint8_t val)` 





#### `public void putU16(std::uint16_t val)` 





#### `public void putU24(std::uint32_t val)` 





#### `public void putU32(std::uint32_t val)` 





#### `public void putU64(std::uint64_t val)` 





#### `public bool update(const char * val,std::size_t len,std::size_t pos)` 



Update a byte range. Throws a std::out_of_range exception if reading past the limit.

#### `public bool update(const std::string & val,std::size_t pos)` 





#### `public bool updateU8(std::uint8_t val,std::size_t pos)` 





#### `public bool updateU16(std::uint16_t val,std::size_t pos)` 





#### `public bool updateU24(std::uint32_t val,std::size_t pos)` 





#### `public bool updateU32(std::uint32_t val,std::size_t pos)` 





#### `public bool updateU64(std::uint64_t val,std::size_t pos)` 





#### `public void seek(std::size_t val)` 



Set position pointer to absolute position. Throws a std::out_of_range exception if the value exceeds the limit.

#### `public void skip(std::size_t size)` 



Set position pointer to relative position. Throws a std::out_of_range exception if the value exceeds the limit.

#### `public std::size_t limit() const` 

Returns the write limit.



#### `public inline std::size_t position() const` 

Returns the current write position.



#### `public std::size_t available() const` 

Returns the number of elements between the current write position and the limit.



#### `public inline char * begin()` 





#### `public inline char * current()` 





#### `public inline const char * begin() const` 





#### `public inline const char * current() const` 





#### `public inline `[`ByteOrder`](#group__base_1gaa934c0972c743983462716b01df7dc32)` order() const` 

Returns written bytes as a string.



#### `public std::string toString()` 





# class `scy::AbstractCollection` 




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

#### `public inline  AbstractCollection()` 





#### `public inline virtual  ~AbstractCollection()` 





#### `public bool add(const TKey & key,TValue * item,bool whiny)` 





#### `public bool remove(const TValue * item)` 





#### `public TValue * remove(const TKey & key)` 





#### `public bool exists(const TKey & key) const` 





#### `public bool exists(const TValue * item) const` 





#### `public bool free(const TKey & key)` 





#### `public bool empty() const` 





#### `public int size() const` 





#### `public TValue * get(const TKey & key,bool whiny) const` 





#### `public void clear()` 





# class `scy::PointerCollection` 

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
`protected mutable `[`Mutex`](#classscy_1_1Mutex)` _mutex` | 

## Members

#### `public inline  PointerCollection()` 





#### `public inline virtual  ~PointerCollection()` 





#### `public inline virtual bool add(const TKey & key,TValue * item,bool whiny)` 





#### `public inline virtual void update(const TKey & key,TValue * item)` 





#### `public inline virtual TValue * get(const TKey & key,bool whiny) const` 





#### `public inline virtual bool free(const TKey & key)` 





#### `public inline virtual TValue * remove(const TKey & key)` 





#### `public inline virtual bool remove(const TValue * item)` 





#### `public inline virtual bool exists(const TKey & key) const` 





#### `public inline virtual bool exists(const TValue * item) const` 





#### `public inline virtual bool empty() const` 





#### `public inline virtual int size() const` 





#### `public inline virtual void clear()` 





#### `public inline virtual Map map() const` 





#### `public inline virtual Map & map()` 





#### `public inline virtual void onAdd(const TKey &,TValue *)` 





#### `public inline virtual void onRemove(const TKey &,TValue *)` 





#### `protected Map _map` 





#### `protected mutable `[`Mutex`](#classscy_1_1Mutex)` _mutex` 





# class `scy::LiveCollection` 

```
class scy::LiveCollection
  : public scy::PointerCollection< TKey, TValue, TDeleter >
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`Signal`](#classscy_1_1Signal)`< TValue & > ItemAdded` | 
`public `[`Signal`](#classscy_1_1Signal)`< const TValue & > ItemRemoved` | 
`public inline virtual void onAdd(const TKey &,TValue * item)` | 
`public inline virtual void onRemove(const TKey &,TValue * item)` | 

## Members

#### `public `[`Signal`](#classscy_1_1Signal)`< TValue & > ItemAdded` 





#### `public `[`Signal`](#classscy_1_1Signal)`< const TValue & > ItemRemoved` 





#### `public inline virtual void onAdd(const TKey &,TValue * item)` 





#### `public inline virtual void onRemove(const TKey &,TValue * item)` 





# class `scy::KVCollection` 


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

#### `public inline  KVCollection()` 





#### `public inline virtual  ~KVCollection()` 





#### `public inline virtual bool add(const TKey & key,const TValue & item,bool update,bool whiny)` 





#### `public inline virtual TValue & get(const TKey & key)` 





#### `public inline virtual const TValue & get(const TKey & key,const TValue & defaultValue) const` 





#### `public inline virtual bool remove(const TKey & key)` 





#### `public inline virtual bool has(const TKey & key) const` 





#### `public inline virtual bool empty() const` 





#### `public inline virtual int size() const` 





#### `public inline virtual void clear()` 





#### `public inline virtual Map & map()` 





#### `protected Map _map` 





# class `scy::NVCollection` 




A storage container for a name value collections. This collection can store multiple entries for each name, and it's getters are case-insensitive.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  NVCollection()` | 
`public inline  _map(nvc. _map) const` | 
`public inline virtual  ~NVCollection()` | 
`public inline `[`NVCollection`](#classscy_1_1NVCollection)` & operator=(const `[`NVCollection`](#classscy_1_1NVCollection)` & nvc)` | Assigns the name-value pairs of another [NVCollection](#classscy_1_1NVCollection) to this one.
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

#### `public inline  NVCollection()` 





#### `public inline  _map(nvc. _map) const` 





#### `public inline virtual  ~NVCollection()` 





#### `public inline `[`NVCollection`](#classscy_1_1NVCollection)` & operator=(const `[`NVCollection`](#classscy_1_1NVCollection)` & nvc)` 

Assigns the name-value pairs of another [NVCollection](#classscy_1_1NVCollection) to this one.



#### `public inline const std::string & operator[](const std::string & name) const` 



Returns the value of the (first) name-value pair with the given name.

Throws a NotFoundException if the name-value pair does not exist.

#### `public inline void set(const std::string & name,const std::string & value)` 

Sets the value of the (first) name-value pair with the given name.



#### `public inline void add(const std::string & name,const std::string & value)` 

Adds a new name-value pair with the given name and value.



#### `public inline const std::string & get(const std::string & name) const` 



Returns the value of the first name-value pair with the given name.

Throws a NotFoundException if the name-value pair does not exist.

#### `public inline const std::string & get(const std::string & name,const std::string & defaultValue) const` 



Returns the value of the first name-value pair with the given name. If no value with the given name has been found, the defaultValue is returned.

#### `public inline bool has(const std::string & name) const` 



Returns true if there is at least one name-value pair with the given name.

#### `public inline ConstIterator find(const std::string & name) const` 



Returns an iterator pointing to the first name-value pair with the given name.

#### `public inline ConstIterator begin() const` 



Returns an iterator pointing to the begin of the name-value pair collection.

#### `public inline ConstIterator end() const` 



Returns an iterator pointing to the end of the name-value pair collection.

#### `public inline bool empty() const` 

Returns true iff the header does not have any content.



#### `public inline int size() const` 



Returns the number of name-value pairs in the collection.

#### `public inline void erase(const std::string & name)` 

Removes all name-value pairs with the given name.



#### `public inline void clear()` 

Removes all name-value pairs and their values.



# class `scy::Configuration` 




[Configuration](#classscy_1_1Configuration) is an abstract base class for managing /// different kinds of configuration storage back ends such as /// JSON, XML, or database.

Subclasses must override the [getRaw()](#group__base_1ga1246d954823215df6d932e78a08bc23b) and [setRaw()](#group__base_1gac6a4023c271c2357334275f15329310d) and methods.

This class is safe for multithreaded use.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`Signal2`](#classscy_1_1Signal2)`< const std::string &, const std::string & > PropertyChanged` | The Key and Value of the changed configuration property.
`public  Configuration()` | Creates the [Configuration](#classscy_1_1Configuration).
`public virtual  ~Configuration()` | Destroys the [Configuration](#classscy_1_1Configuration).
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

#### `public `[`Signal2`](#classscy_1_1Signal2)`< const std::string &, const std::string & > PropertyChanged` 

The Key and Value of the changed configuration property.



#### `public  Configuration()` 

Creates the [Configuration](#classscy_1_1Configuration).



#### `public virtual  ~Configuration()` 

Destroys the [Configuration](#classscy_1_1Configuration).



#### `public bool exists(const std::string & key) const` 

Returns true if the property with the given key exists.



#### `public std::string getString(const std::string & key) const` 



Returns the string value of the property with the given name. Throws a NotFoundException if the key does not exist.

#### `public std::string getString(const std::string & key,const std::string & defaultValue) const` 



If a property with the given key exists, returns the property's string value, otherwise returns the given default value.

#### `public std::string getRawString(const std::string & key) const` 



Returns the raw string value of the property with the given name. Throws a NotFoundException if the key does not exist. References to other properties are not expanded.

#### `public std::string getRawString(const std::string & key,const std::string & defaultValue) const` 



If a property with the given key exists, returns the property's raw string value, otherwise returns the given default value. References to other properties are not expanded.

#### `public int getInt(const std::string & key) const` 



Returns the int value of the property with the given name. Throws a NotFoundException if the key does not exist. Throws a SyntaxException if the property can not be converted to an int. Numbers starting with 0x are treated as hexadecimal.

#### `public int getInt(const std::string & key,int defaultValue) const` 



If a property with the given key exists, returns the property's int value, otherwise returns the given default value. Throws a SyntaxException if the property can not be converted to an int. Numbers starting with 0x are treated as hexadecimal.

#### `public std::int64_t getLargeInt(const std::string & key) const` 



Returns the int value of the property with the given name. Throws a NotFoundException if the key does not exist. Throws a SyntaxException if the property can not be converted to an int. Numbers starting with 0x are treated as hexadecimal.

#### `public std::int64_t getLargeInt(const std::string & key,std::int64_t defaultValue) const` 



If a property with the given key exists, returns the property's int value, otherwise returns the given default value. Throws a SyntaxException if the property can not be converted to an int. Numbers starting with 0x are treated as hexadecimal.

#### `public double getDouble(const std::string & key) const` 



Returns the double value of the property with the given name. Throws a NotFoundException if the key does not exist. Throws a SyntaxException if the property can not be converted to a double.

#### `public double getDouble(const std::string & key,double defaultValue) const` 



If a property with the given key exists, returns the property's double value, otherwise returns the given default value. Throws a SyntaxException if the property can not be converted to an double.

#### `public bool getBool(const std::string & key) const` 



Returns the double value of the property with the given name. Throws a NotFoundException if the key does not exist. Throws a SyntaxException if the property can not be converted to a double.

#### `public bool getBool(const std::string & key,bool defaultValue) const` 



If a property with the given key exists, returns the property's bool value, otherwise returns the given default value. Throws a SyntaxException if the property can not be converted to a boolean. The following string values can be converted into a boolean:

* numerical values: non zero becomes true, zero becomes false


* strings: true, yes, on become true, false, no, off become false Case does not matter.

#### `public void setString(const std::string & key,const std::string & value)` 



Sets the property with the given key to the given value. An already existing value for the key is overwritten.

#### `public void setInt(const std::string & key,int value)` 



Sets the property with the given key to the given value. An already existing value for the key is overwritten.

#### `public void setLargeInt(const std::string & key,std::int64_t value)` 



Sets the property with the given key to the given value. An already existing value for the key is overwritten.

#### `public void setDouble(const std::string & key,double value)` 



Sets the property with the given key to the given value. An already existing value for the key is overwritten.

#### `public void setBool(const std::string & key,bool value)` 



Sets the property with the given key to the given value. An already existing value for the key is overwritten.

#### `protected bool getRaw(const std::string & key,std::string & value) const` 



If the property with the given key exists, stores the property's value in value and returns true. Otherwise, returns false.

Must be overridden by subclasses.

#### `protected void setRaw(const std::string & key,const std::string & value)` 



Sets the property with the given key to the given value. An already existing value for the key is overwritten.

The implementation is responsible for emitting the PropertyChanged signal.

# class `scy::ScopedConfiguration` 




[ScopedConfiguration](#classscy_1_1ScopedConfiguration) provides multiple levels of configuration for a module. Multiple levels means that there is a module level scope, and a default scope. When a property is accessed, the module scope value will be used if available, otherwise the default scope value will be used.

Example scoping: Module: channels.[name].modes.[name].[value] Default: modes.[name].[value]

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`Configuration`](#classscy_1_1Configuration)` & config` | 
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

#### `public `[`Configuration`](#classscy_1_1Configuration)` & config` 





#### `public std::string currentScope` 





#### `public std::string defaultScope` 





#### `public  ScopedConfiguration(`[`Configuration`](#classscy_1_1Configuration)` & config,const std::string & currentScope,const std::string & defaultScope)` 





#### `public  ScopedConfiguration(const `[`ScopedConfiguration`](#classscy_1_1ScopedConfiguration)` & that)` 





#### `public std::string getString(const std::string & key,const std::string & defaultValue,bool forceDefaultScope) const` 





#### `public int getInt(const std::string & key,int defaultValue,bool forceDefaultScope) const` 





#### `public double getDouble(const std::string & key,double defaultValue,bool forceDefaultScope) const` 





#### `public bool getBool(const std::string & key,bool defaultValue,bool forceDefaultScope) const` 





#### `public void setString(const std::string & key,const std::string & value,bool defaultScope)` 





#### `public void setInt(const std::string & key,int value,bool defaultScope)` 





#### `public void setDouble(const std::string & key,double value,bool defaultScope)` 





#### `public void setBool(const std::string & key,bool value,bool defaultScope)` 





#### `public std::string getCurrentScope(const std::string & key) const` 





#### `public std::string getDafaultKey(const std::string & key) const` 





#### `public std::string getScopedKey(const std::string & key,bool defaultScope) const` 





# class `scy::Timestamp` 




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
`public void swap(`[`Timestamp`](#classscy_1_1Timestamp)` & timestamp)` | Swaps the [Timestamp](#classscy_1_1Timestamp) with another one.
`public void update()` | Updates the [Timestamp](#classscy_1_1Timestamp) with the current time.
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

#### `public  Timestamp()` 

Creates a timestamp with the current time.



#### `public  Timestamp(`[`TimeVal`](#group__base_1ga7da2b2da9cd1e096cbb146da027f17dc)` tv)` 

Creates a timestamp from the given time value.



#### `public  Timestamp(const `[`Timestamp`](#classscy_1_1Timestamp)` & other)` 

Copy constructor.



#### `public  ~Timestamp()` 

Destroys the timestamp.



#### `public `[`Timestamp`](#classscy_1_1Timestamp)` & operator=(const `[`Timestamp`](#classscy_1_1Timestamp)` & other)` 





#### `public `[`Timestamp`](#classscy_1_1Timestamp)` & operator=(`[`TimeVal`](#group__base_1ga7da2b2da9cd1e096cbb146da027f17dc)` tv)` 





#### `public void swap(`[`Timestamp`](#classscy_1_1Timestamp)` & timestamp)` 

Swaps the [Timestamp](#classscy_1_1Timestamp) with another one.



#### `public void update()` 

Updates the [Timestamp](#classscy_1_1Timestamp) with the current time.



#### `public inline bool operator==(const `[`Timestamp`](#classscy_1_1Timestamp)` & ts) const` 





#### `public inline bool operator!=(const `[`Timestamp`](#classscy_1_1Timestamp)` & ts) const` 





#### `public inline bool operator>(const `[`Timestamp`](#classscy_1_1Timestamp)` & ts) const` 





#### `public inline bool operator>=(const `[`Timestamp`](#classscy_1_1Timestamp)` & ts) const` 





#### `public inline bool operator<(const `[`Timestamp`](#classscy_1_1Timestamp)` & ts) const` 





#### `public inline bool operator<=(const `[`Timestamp`](#classscy_1_1Timestamp)` & ts) const` 





#### `public inline `[`Timestamp`](#classscy_1_1Timestamp)` operator+(`[`TimeDiff`](#group__base_1ga9f06831e4e9b9cbc6f061cc61dc91fd0)` d) const` 





#### `public inline `[`Timestamp`](#classscy_1_1Timestamp)` operator-(`[`TimeDiff`](#group__base_1ga9f06831e4e9b9cbc6f061cc61dc91fd0)` d) const` 





#### `public inline `[`TimeDiff`](#group__base_1ga9f06831e4e9b9cbc6f061cc61dc91fd0)` operator-(const `[`Timestamp`](#classscy_1_1Timestamp)` & ts) const` 





#### `public inline `[`Timestamp`](#classscy_1_1Timestamp)` & operator+=(`[`TimeDiff`](#group__base_1ga9f06831e4e9b9cbc6f061cc61dc91fd0)` d)` 





#### `public inline `[`Timestamp`](#classscy_1_1Timestamp)` & operator-=(`[`TimeDiff`](#group__base_1ga9f06831e4e9b9cbc6f061cc61dc91fd0)` d)` 





#### `public inline std::time_t epochTime() const` 



Returns the timestamp expressed in time_t. time_t base time is midnight, January 1, 1970. Resolution is one second.

#### `public inline `[`UtcTimeVal`](#group__base_1gaec8ace0f25db0ce91c10b30270c3dc24)` utcTime() const` 



Returns the timestamp expressed in UTC-based time. UTC base time is midnight, October 15, 1582. Resolution is 100 nanoseconds.

#### `public inline `[`TimeVal`](#group__base_1ga7da2b2da9cd1e096cbb146da027f17dc)` epochMicroseconds() const` 



Returns the timestamp expressed in microseconds since the Unix epoch, midnight, January 1, 1970.

#### `public inline `[`TimeDiff`](#group__base_1ga9f06831e4e9b9cbc6f061cc61dc91fd0)` elapsed() const` 



Returns the time elapsed since the time denoted by the timestamp. Equivalent to [Timestamp()](#group__base_1ga59eb0caf11a4b42805be30b575c64685) - *this.

#### `public inline bool isElapsed(`[`TimeDiff`](#group__base_1ga9f06831e4e9b9cbc6f061cc61dc91fd0)` interval) const` 



Returns true iff the given interval has passed since the time denoted by the timestamp.

# class `scy::Timespan` 


A class that represents time spans up to microsecond resolution.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Timespan()` | Creates a zero [Timespan](#classscy_1_1Timespan).
`public  Timespan(TimeDiff microseconds)` | Creates a [Timespan](#classscy_1_1Timespan).
`public  Timespan(long seconds,long microseconds)` | 
`public  Timespan(int days,int hours,int minutes,int seconds,int microseconds)` | Creates a [Timespan](#classscy_1_1Timespan).
`public  Timespan(const `[`Timespan`](#classscy_1_1Timespan)` & timespan)` | Creates a [Timespan](#classscy_1_1Timespan) from another one.
`public  ~Timespan()` | Destroys the [Timespan](#classscy_1_1Timespan).
`public `[`Timespan`](#classscy_1_1Timespan)` & operator=(const `[`Timespan`](#classscy_1_1Timespan)` & timespan)` | Assignment operator.
`public `[`Timespan`](#classscy_1_1Timespan)` & operator=(TimeDiff microseconds)` | Assignment operator.
`public `[`Timespan`](#classscy_1_1Timespan)` & assign(int days,int hours,int minutes,int seconds,int microseconds)` | Assigns a new span.
`public `[`Timespan`](#classscy_1_1Timespan)` & assign(long seconds,long microseconds)` | 
`public void swap(`[`Timespan`](#classscy_1_1Timespan)` & timespan)` | Swaps the [Timespan](#classscy_1_1Timespan) with another one.
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

#### `public  Timespan()` 

Creates a zero [Timespan](#classscy_1_1Timespan).



#### `public  Timespan(TimeDiff microseconds)` 

Creates a [Timespan](#classscy_1_1Timespan).



#### `public  Timespan(long seconds,long microseconds)` 



Creates a [Timespan](#classscy_1_1Timespan). Useful for creating a [Timespan](#classscy_1_1Timespan) from a struct timeval.

#### `public  Timespan(int days,int hours,int minutes,int seconds,int microseconds)` 

Creates a [Timespan](#classscy_1_1Timespan).



#### `public  Timespan(const `[`Timespan`](#classscy_1_1Timespan)` & timespan)` 

Creates a [Timespan](#classscy_1_1Timespan) from another one.



#### `public  ~Timespan()` 

Destroys the [Timespan](#classscy_1_1Timespan).



#### `public `[`Timespan`](#classscy_1_1Timespan)` & operator=(const `[`Timespan`](#classscy_1_1Timespan)` & timespan)` 

Assignment operator.



#### `public `[`Timespan`](#classscy_1_1Timespan)` & operator=(TimeDiff microseconds)` 

Assignment operator.



#### `public `[`Timespan`](#classscy_1_1Timespan)` & assign(int days,int hours,int minutes,int seconds,int microseconds)` 

Assigns a new span.



#### `public `[`Timespan`](#classscy_1_1Timespan)` & assign(long seconds,long microseconds)` 



Assigns a new span. Useful for assigning from a struct timeval.

#### `public void swap(`[`Timespan`](#classscy_1_1Timespan)` & timespan)` 

Swaps the [Timespan](#classscy_1_1Timespan) with another one.



#### `public inline bool operator==(const `[`Timespan`](#classscy_1_1Timespan)` & ts) const` 





#### `public inline bool operator!=(const `[`Timespan`](#classscy_1_1Timespan)` & ts) const` 





#### `public inline bool operator>(const `[`Timespan`](#classscy_1_1Timespan)` & ts) const` 





#### `public inline bool operator>=(const `[`Timespan`](#classscy_1_1Timespan)` & ts) const` 





#### `public inline bool operator<(const `[`Timespan`](#classscy_1_1Timespan)` & ts) const` 





#### `public inline bool operator<=(const `[`Timespan`](#classscy_1_1Timespan)` & ts) const` 





#### `public inline bool operator==(TimeDiff microseconds) const` 





#### `public inline bool operator!=(TimeDiff microseconds) const` 





#### `public inline bool operator>(TimeDiff microseconds) const` 





#### `public inline bool operator>=(TimeDiff microseconds) const` 





#### `public inline bool operator<(TimeDiff microseconds) const` 





#### `public inline bool operator<=(TimeDiff microseconds) const` 





#### `public `[`Timespan`](#classscy_1_1Timespan)` operator+(const `[`Timespan`](#classscy_1_1Timespan)` & d) const` 





#### `public `[`Timespan`](#classscy_1_1Timespan)` operator-(const `[`Timespan`](#classscy_1_1Timespan)` & d) const` 





#### `public `[`Timespan`](#classscy_1_1Timespan)` & operator+=(const `[`Timespan`](#classscy_1_1Timespan)` & d)` 





#### `public `[`Timespan`](#classscy_1_1Timespan)` & operator-=(const `[`Timespan`](#classscy_1_1Timespan)` & d)` 





#### `public `[`Timespan`](#classscy_1_1Timespan)` operator+(TimeDiff microseconds) const` 





#### `public `[`Timespan`](#classscy_1_1Timespan)` operator-(TimeDiff microseconds) const` 





#### `public `[`Timespan`](#classscy_1_1Timespan)` & operator+=(TimeDiff microseconds)` 





#### `public `[`Timespan`](#classscy_1_1Timespan)` & operator-=(TimeDiff microseconds)` 





#### `public inline int days() const` 

Returns the number of days.



#### `public inline int hours() const` 

Returns the number of hours (0 to 23).



#### `public inline int totalHours() const` 

Returns the total number of hours.



#### `public inline int minutes() const` 

Returns the number of minutes (0 to 59).



#### `public inline int totalMinutes() const` 

Returns the total number of minutes.



#### `public inline int seconds() const` 

Returns the number of seconds (0 to 59).



#### `public inline int totalSeconds() const` 

Returns the total number of seconds.



#### `public inline int milliseconds() const` 

Returns the number of milliseconds (0 to 999).



#### `public inline TimeDiff totalMilliseconds() const` 

Returns the total number of milliseconds.



#### `public inline int microseconds() const` 



Returns the fractions of a millisecond in microseconds (0 to 999).

#### `public inline int useconds() const` 



Returns the fractions of a second in microseconds (0 to 999999).

#### `public inline TimeDiff totalMicroseconds() const` 

Returns the total number of microseconds.



# class `scy::DateTime` 




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
`public  DateTime()` | Creates a [DateTime](#classscy_1_1DateTime) for the current date and time.
`public  DateTime(const `[`Timestamp`](#classscy_1_1Timestamp)` & timestamp)` | 
`public  DateTime(int year,int month,int day,int hour,int minute,int second,int millisecond,int microsecond)` | 
`public  DateTime(double julianDay)` | Creates a [DateTime](#classscy_1_1DateTime) for the given Julian day.
`public  DateTime(`[`Timestamp::UtcTimeVal`](#group__base_1gaec8ace0f25db0ce91c10b30270c3dc24)` utcTime,`[`Timestamp::TimeDiff`](#group__base_1ga9f06831e4e9b9cbc6f061cc61dc91fd0)` diff)` | 
`public  DateTime(const `[`DateTime`](#classscy_1_1DateTime)` & dateTime)` | Copy constructor. Creates the [DateTime](#classscy_1_1DateTime) from another one.
`public  ~DateTime()` | Destroys the [DateTime](#classscy_1_1DateTime).
`public `[`DateTime`](#classscy_1_1DateTime)` & operator=(const `[`DateTime`](#classscy_1_1DateTime)` & dateTime)` | Assigns another [DateTime](#classscy_1_1DateTime).
`public `[`DateTime`](#classscy_1_1DateTime)` & operator=(const `[`Timestamp`](#classscy_1_1Timestamp)` & timestamp)` | Assigns a [Timestamp](#classscy_1_1Timestamp).
`public `[`DateTime`](#classscy_1_1DateTime)` & operator=(double julianDay)` | Assigns a Julian day.
`public `[`DateTime`](#classscy_1_1DateTime)` & assign(int year,int month,int day,int hour,int minute,int second,int millisecond,int microseconds)` | 
`public void swap(`[`DateTime`](#classscy_1_1DateTime)` & dateTime)` | Swaps the [DateTime](#classscy_1_1DateTime) with another one.
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
`public inline `[`Timestamp`](#classscy_1_1Timestamp)` timestamp() const` | Returns the date and time expressed as a [Timestamp](#classscy_1_1Timestamp).
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

#### `public  DateTime()` 

Creates a [DateTime](#classscy_1_1DateTime) for the current date and time.



#### `public  DateTime(const `[`Timestamp`](#classscy_1_1Timestamp)` & timestamp)` 



Creates a [DateTime](#classscy_1_1DateTime) for the date and time given in a [Timestamp](#classscy_1_1Timestamp).

#### `public  DateTime(int year,int month,int day,int hour,int minute,int second,int millisecond,int microsecond)` 



Creates a [DateTime](#classscy_1_1DateTime) for the given Gregorian date and time.

* year is from 0 to 9999.


* month is from 1 to 12.


* day is from 1 to 31.


* hour is from 0 to 23.


* minute is from 0 to 59.


* second is from 0 to 59.


* millisecond is from 0 to 999.


* microsecond is from 0 to 999.

#### `public  DateTime(double julianDay)` 

Creates a [DateTime](#classscy_1_1DateTime) for the given Julian day.



#### `public  DateTime(`[`Timestamp::UtcTimeVal`](#group__base_1gaec8ace0f25db0ce91c10b30270c3dc24)` utcTime,`[`Timestamp::TimeDiff`](#group__base_1ga9f06831e4e9b9cbc6f061cc61dc91fd0)` diff)` 



Creates a [DateTime](#classscy_1_1DateTime) from an UtcTimeVal and a TimeDiff.

Mainly used internally by [DateTime](#classscy_1_1DateTime) and friends.

#### `public  DateTime(const `[`DateTime`](#classscy_1_1DateTime)` & dateTime)` 

Copy constructor. Creates the [DateTime](#classscy_1_1DateTime) from another one.



#### `public  ~DateTime()` 

Destroys the [DateTime](#classscy_1_1DateTime).



#### `public `[`DateTime`](#classscy_1_1DateTime)` & operator=(const `[`DateTime`](#classscy_1_1DateTime)` & dateTime)` 

Assigns another [DateTime](#classscy_1_1DateTime).



#### `public `[`DateTime`](#classscy_1_1DateTime)` & operator=(const `[`Timestamp`](#classscy_1_1Timestamp)` & timestamp)` 

Assigns a [Timestamp](#classscy_1_1Timestamp).



#### `public `[`DateTime`](#classscy_1_1DateTime)` & operator=(double julianDay)` 

Assigns a Julian day.



#### `public `[`DateTime`](#classscy_1_1DateTime)` & assign(int year,int month,int day,int hour,int minute,int second,int millisecond,int microseconds)` 



Assigns a Gregorian date and time.

* year is from 0 to 9999.


* month is from 1 to 12.


* day is from 1 to 31.


* hour is from 0 to 23.


* minute is from 0 to 59.


* second is from 0 to 59.


* millisecond is from 0 to 999.


* microsecond is from 0 to 999.

#### `public void swap(`[`DateTime`](#classscy_1_1DateTime)` & dateTime)` 

Swaps the [DateTime](#classscy_1_1DateTime) with another one.



#### `public inline int year() const` 

Returns the year.



#### `public inline int month() const` 

Returns the month (1 to 12).



#### `public int week(int firstDayOfWeek) const` 



Returns the week number within the year. FirstDayOfWeek should be either SUNDAY (0) or MONDAY (1). The returned week number will be from 0 to 53. Week number 1 is the week containing January 4. This is in accordance to ISO 8601.

The following example assumes that firstDayOfWeek is MONDAY. For 2005, which started on a Saturday, week 1 will be the week starting on Monday, January 3. January 1 and 2 will fall within week 0 (or the last week of the previous year).

For 2007, which starts on a Monday, week 1 will be the week startung on Monday, January 1. There will be no week 0 in 2007.

#### `public inline int day() const` 

Returns the day witin the month (1 to 31).



#### `public int dayOfWeek() const` 



Returns the weekday (0 to 6, where 0 = Sunday, 1 = Monday, ..., 6 = Saturday).

#### `public int dayOfYear() const` 



Returns the number of the day in the year. January 1 is 1, February 1 is 32, etc.

#### `public inline int hour() const` 

Returns the hour (0 to 23).



#### `public inline int hourAMPM() const` 

Returns the hour (0 to 12).



#### `public inline bool isAM() const` 

Returns true if hour < 12;.



#### `public inline bool isPM() const` 

Returns true if hour >= 12.



#### `public inline int minute() const` 

Returns the minute (0 to 59).



#### `public inline int second() const` 

Returns the second (0 to 59).



#### `public inline int millisecond() const` 

Returns the millisecond (0 to 999)



#### `public inline int microsecond() const` 

Returns the microsecond (0 to 999)



#### `public double julianDay() const` 

Returns the julian day for the date and time.



#### `public inline `[`Timestamp`](#classscy_1_1Timestamp)` timestamp() const` 

Returns the date and time expressed as a [Timestamp](#classscy_1_1Timestamp).



#### `public inline `[`Timestamp::UtcTimeVal`](#group__base_1gaec8ace0f25db0ce91c10b30270c3dc24)` utcTime() const` 



Returns the date and time expressed in UTC-based time. UTC base time is midnight, October 15, 1582. Resolution is 100 nanoseconds.

#### `public inline bool operator==(const `[`DateTime`](#classscy_1_1DateTime)` & dateTime) const` 





#### `public inline bool operator!=(const `[`DateTime`](#classscy_1_1DateTime)` & dateTime) const` 





#### `public inline bool operator<(const `[`DateTime`](#classscy_1_1DateTime)` & dateTime) const` 





#### `public inline bool operator<=(const `[`DateTime`](#classscy_1_1DateTime)` & dateTime) const` 





#### `public inline bool operator>(const `[`DateTime`](#classscy_1_1DateTime)` & dateTime) const` 





#### `public inline bool operator>=(const `[`DateTime`](#classscy_1_1DateTime)` & dateTime) const` 





#### `public `[`DateTime`](#classscy_1_1DateTime)` operator+(const `[`Timespan`](#classscy_1_1Timespan)` & span) const` 





#### `public `[`DateTime`](#classscy_1_1DateTime)` operator-(const `[`Timespan`](#classscy_1_1Timespan)` & span) const` 





#### `public `[`Timespan`](#classscy_1_1Timespan)` operator-(const `[`DateTime`](#classscy_1_1DateTime)` & dateTime) const` 





#### `public `[`DateTime`](#classscy_1_1DateTime)` & operator+=(const `[`Timespan`](#classscy_1_1Timespan)` & span)` 





#### `public `[`DateTime`](#classscy_1_1DateTime)` & operator-=(const `[`Timespan`](#classscy_1_1Timespan)` & span)` 





#### `public void makeUTC(int tzd)` 

Converts a local time into UTC, by applying the given time zone differential.



#### `public void makeLocal(int tzd)` 

Converts a UTC time into a local time, by applying the given time zone differential.



#### `protected void computeGregorian(double julianDay)` 



Computes the Gregorian date for the given Julian day. See [http://vsg.cape.com/~pbaum/date/injdimp.htm](http://vsg.cape.com/~pbaum/date/injdimp.htm), section 3.3.1 for the algorithm.

#### `protected void computeDaytime()` 

Extracts the daytime (hours, minutes, seconds, etc.) from the stored utcTime.



# class `scy::Timezone` 


This class provides information about the current timezone.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------

## Members

# class `scy::LocalDateTime` 




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
`public  LocalDateTime(double julianDay)` | Creates a [LocalDateTime](#classscy_1_1LocalDateTime) for the given Julian day in the local time zone.
`public  LocalDateTime(int tzd,double julianDay)` | 
`public  LocalDateTime(const `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & dateTime)` | Copy constructor. Creates the [LocalDateTime](#classscy_1_1LocalDateTime) from another one.
`public  ~LocalDateTime()` | Destroys the [LocalDateTime](#classscy_1_1LocalDateTime).
`public `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & operator=(const `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & dateTime)` | Assigns another [LocalDateTime](#classscy_1_1LocalDateTime).
`public `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & operator=(const `[`Timestamp`](#classscy_1_1Timestamp)` & timestamp)` | Assigns a timestamp.
`public `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & operator=(double julianDay)` | Assigns a Julian day in the local time zone.
`public `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & assign(int year,int month,int day,int hour,int minute,int second,int millisecond,int microseconds)` | 
`public `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & assign(int tzd,int year,int month,int day,int hour,int minute,int second,int millisecond,int microseconds)` | 
`public `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & assign(int tzd,double julianDay)` | 
`public void swap(`[`LocalDateTime`](#classscy_1_1LocalDateTime)` & dateTime)` | Swaps the [LocalDateTime](#classscy_1_1LocalDateTime) with another one.
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
`public inline `[`Timestamp`](#classscy_1_1Timestamp)` timestamp() const` | Returns the date and time expressed as a [Timestamp](#classscy_1_1Timestamp).
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

#### `public  LocalDateTime()` 



Creates a [LocalDateTime](#classscy_1_1LocalDateTime) with the current date/time for the current time zone.

#### `public  LocalDateTime(int year,int month,int day,int hour,int minute,int second,int millisecond,int microsecond)` 



Creates a [LocalDateTime](#classscy_1_1LocalDateTime) for the given Gregorian local date and time.

* year is from 0 to 9999.


* month is from 1 to 12.


* day is from 1 to 31.


* hour is from 0 to 23.


* minute is from 0 to 59.


* second is from 0 to 59.


* millisecond is from 0 to 999.


* microsecond is from 0 to 999.

#### `public  LocalDateTime(int tzd,int year,int month,int day,int hour,int minute,int second,int millisecond,int microsecond)` 



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

#### `public  LocalDateTime(const `[`DateTime`](#classscy_1_1DateTime)` & dateTime)` 



Creates a [LocalDateTime](#classscy_1_1LocalDateTime) from the UTC time given in dateTime, using the time zone differential of the current time zone.

#### `public  LocalDateTime(int tzd,const `[`DateTime`](#classscy_1_1DateTime)` & dateTime)` 



@ deprecated Creates a [LocalDateTime](#classscy_1_1LocalDateTime) from the UTC time given in dateTime, using the given time zone differential. Adjusts dateTime for the given time zone differential.

#### `public  LocalDateTime(int tzd,const `[`DateTime`](#classscy_1_1DateTime)` & dateTime,bool adjust)` 



@ deprecated Creates a [LocalDateTime](#classscy_1_1LocalDateTime) from the UTC time given in dateTime, using the given time zone differential. If adjust is true, adjusts dateTime for the given time zone differential.

#### `public  LocalDateTime(double julianDay)` 

Creates a [LocalDateTime](#classscy_1_1LocalDateTime) for the given Julian day in the local time zone.



#### `public  LocalDateTime(int tzd,double julianDay)` 



@ deprecated Creates a [LocalDateTime](#classscy_1_1LocalDateTime) for the given Julian day in the time zone denoted by the time zone differential in tzd.

#### `public  LocalDateTime(const `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & dateTime)` 

Copy constructor. Creates the [LocalDateTime](#classscy_1_1LocalDateTime) from another one.



#### `public  ~LocalDateTime()` 

Destroys the [LocalDateTime](#classscy_1_1LocalDateTime).



#### `public `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & operator=(const `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & dateTime)` 

Assigns another [LocalDateTime](#classscy_1_1LocalDateTime).



#### `public `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & operator=(const `[`Timestamp`](#classscy_1_1Timestamp)` & timestamp)` 

Assigns a timestamp.



#### `public `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & operator=(double julianDay)` 

Assigns a Julian day in the local time zone.



#### `public `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & assign(int year,int month,int day,int hour,int minute,int second,int millisecond,int microseconds)` 



Assigns a Gregorian local date and time.

* year is from 0 to 9999.


* month is from 1 to 12.


* day is from 1 to 31.


* hour is from 0 to 23.


* minute is from 0 to 59.


* second is from 0 to 59.


* millisecond is from 0 to 999.


* microsecond is from 0 to 999.

#### `public `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & assign(int tzd,int year,int month,int day,int hour,int minute,int second,int millisecond,int microseconds)` 



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

#### `public `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & assign(int tzd,double julianDay)` 



@ deprecated Assigns a Julian day in the time zone denoted by the time zone differential in tzd.

#### `public void swap(`[`LocalDateTime`](#classscy_1_1LocalDateTime)` & dateTime)` 

Swaps the [LocalDateTime](#classscy_1_1LocalDateTime) with another one.



#### `public inline int year() const` 

Returns the year.



#### `public inline int month() const` 

Returns the month (1 to 12).



#### `public inline int week(int firstDayOfWeek) const` 



Returns the week number within the year. FirstDayOfWeek should be either SUNDAY (0) or MONDAY (1). The returned week number will be from 0 to 53. Week number 1 is the week containing January 4. This is in accordance to ISO 8601.

The following example assumes that firstDayOfWeek is MONDAY. For 2005, which started on a Saturday, week 1 will be the week starting on Monday, January 3. January 1 and 2 will fall within week 0 (or the last week of the previous year).

For 2007, which starts on a Monday, week 1 will be the week startung on Monday, January 1. There will be no week 0 in 2007.

#### `public inline int day() const` 

Returns the day witin the month (1 to 31).



#### `public inline int dayOfWeek() const` 



Returns the weekday (0 to 6, where 0 = Sunday, 1 = Monday, ..., 6 = Saturday).

#### `public inline int dayOfYear() const` 



Returns the number of the day in the year. January 1 is 1, February 1 is 32, etc.

#### `public inline int hour() const` 

Returns the hour (0 to 23).



#### `public inline int hourAMPM() const` 

Returns the hour (0 to 12).



#### `public inline bool isAM() const` 

Returns true if hour < 12;.



#### `public inline bool isPM() const` 

Returns true if hour >= 12.



#### `public inline int minute() const` 

Returns the minute (0 to 59).



#### `public inline int second() const` 

Returns the second (0 to 59).



#### `public inline int millisecond() const` 

Returns the millisecond (0 to 999)



#### `public inline int microsecond() const` 

Returns the microsecond (0 to 999)



#### `public inline double julianDay() const` 

Returns the julian day for the date.



#### `public inline int tzd() const` 

Returns the time zone differential.



#### `public `[`DateTime`](#classscy_1_1DateTime)` utc() const` 

Returns the UTC equivalent for the local date and time.



#### `public inline `[`Timestamp`](#classscy_1_1Timestamp)` timestamp() const` 

Returns the date and time expressed as a [Timestamp](#classscy_1_1Timestamp).



#### `public inline `[`Timestamp::UtcTimeVal`](#group__base_1gaec8ace0f25db0ce91c10b30270c3dc24)` utcTime() const` 

Returns the UTC equivalent for the local date and time.



#### `public bool operator==(const `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & dateTime) const` 





#### `public bool operator!=(const `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & dateTime) const` 





#### `public bool operator<(const `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & dateTime) const` 





#### `public bool operator<=(const `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & dateTime) const` 





#### `public bool operator>(const `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & dateTime) const` 





#### `public bool operator>=(const `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & dateTime) const` 





#### `public `[`LocalDateTime`](#classscy_1_1LocalDateTime)` operator+(const `[`Timespan`](#classscy_1_1Timespan)` & span) const` 





#### `public `[`LocalDateTime`](#classscy_1_1LocalDateTime)` operator-(const `[`Timespan`](#classscy_1_1Timespan)` & span) const` 





#### `public `[`Timespan`](#classscy_1_1Timespan)` operator-(const `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & dateTime) const` 





#### `public `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & operator+=(const `[`Timespan`](#classscy_1_1Timespan)` & span)` 





#### `public `[`LocalDateTime`](#classscy_1_1LocalDateTime)` & operator-=(const `[`Timespan`](#classscy_1_1Timespan)` & span)` 





#### `protected  LocalDateTime(`[`Timestamp::UtcTimeVal`](#group__base_1gaec8ace0f25db0ce91c10b30270c3dc24)` utcTime,`[`Timestamp::TimeDiff`](#group__base_1ga9f06831e4e9b9cbc6f061cc61dc91fd0)` diff,int tzd)` 





#### `protected void determineTzd(bool adjust)` 



Recalculate the tzd based on the _dateTime member based on the current timezone using the Standard C runtime functions. If adjust is true, then [adjustForTzd()](#group__base_1gad0d104ecf1aa081650b681f2afe5718d) is called after the differential is calculated.

#### `protected inline void adjustForTzd()` 

Adjust the _dateTime member based on the _tzd member.



#### `protected std::time_t dstOffset(int & dstOffset) const` 

Determine the DST offset for the current date/time.



# class `scy::DateTimeFormat` 




Definition of date/time formats and various constants used by [DateTimeFormatter](#classscy_1_1DateTimeFormatter) and [DateTimeParser](#classscy_1_1DateTimeParser).

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------

## Members

# class `scy::DateTimeFormatter` 




This class converts dates and times into strings, supporting a variety of standard and custom formats.

There are two kind of static member functions: /// * format* functions return a std::string containing the formatted value.

* append* functions append the formatted value to an existing string.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------

## Members

# class `scy::DateTimeParser` 




This class provides a method for parsing dates and times from strings. All parsing methods do their best to parse a meaningful result, even from malformed input strings.

The returned [DateTime](#classscy_1_1DateTime) will always contain a time in the same timezone as the time in the string. Call [DateTime::makeUTC()](#group__base_1gaef85751cde47a0e909aeddae1775b805) /// with the timeZoneDifferential returned by [parse()](#group__base_1ga7249fe6e774b64ab401a6f3f97311f26) to convert the [DateTime](#classscy_1_1DateTime) to UTC.

Note: When parsing a time in 12-hour (AM/PM) format, the hour (h) must be parsed before the AM/PM designator (a, A), otherwise the AM/PM designator will be ignored.

See the [DateTimeFormatter](#classscy_1_1DateTimeFormatter) class for a list of supported format specifiers. In addition to the format specifiers supported by [DateTimeFormatter](#classscy_1_1DateTimeFormatter), an additional specifier is supported: r will parse a year given by either two or four digits. Years 69-00 are interpreted in the 20th century (1969-2000), years 01-68 in the 21th century (2001-2068).

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------

## Members

# class `scy::Stopwatch` 




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

#### `public  Stopwatch()` 





#### `public  ~Stopwatch()` 





#### `public void start()` 

Starts (or restarts) the stopwatch.



#### `public void stop()` 

Stops or pauses the stopwatch.



#### `public void reset()` 

Resets the stopwatch.



#### `public void restart()` 

Resets and starts the stopwatch.



#### `public `[`Timestamp::TimeDiff`](#group__base_1ga9f06831e4e9b9cbc6f061cc61dc91fd0)` elapsed() const` 



Returns the elapsed time in microseconds since the stopwatch started.

#### `public int elapsedSeconds() const` 



Returns the number of seconds elapsed since the stopwatch started.

#### `public int elapsedMilliseconds() const` 



Returns the number of milliseconds elapsed since the stopwatch started.

# class `scy::Timeout` 


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

#### `public  Timeout(long delay,bool autoStart)` 





#### `public  Timeout(const `[`Timeout`](#classscy_1_1Timeout)` & src)` 





#### `public  ~Timeout()` 





#### `public bool running() const` 





#### `public void start()` 





#### `public void stop()` 





#### `public void reset()` 





#### `public long remaining() const` 





#### `public bool expired() const` 





#### `public inline void setDelay(long delay)` 





#### `public inline clock_t startAt() const` 





#### `public inline long delay() const` 





#### `public `[`Timeout`](#classscy_1_1Timeout)` & operator=(const `[`Timeout`](#classscy_1_1Timeout)` & src)` 





#### `protected clock_t _startAt` 





#### `protected long _delay` 





# class `scy::TimedToken` 

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

#### `public  TimedToken(long duration)` 





#### `public  TimedToken(const std::string & id,long duration)` 





#### `public inline std::string id() const` 





#### `public inline bool operator==(const `[`TimedToken`](#classscy_1_1TimedToken)` & r) const` 





#### `public inline bool operator==(const std::string & r) const` 





#### `protected std::string _id` 





# class `scy::Delegate` 

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

#### `public inline  Delegate(C * object,Method method,int priority)` 





#### `public inline  Delegate(C * object,Method method,DataT filter,int priority)` 





#### `public inline  Delegate(const `[`Delegate`](#classscy_1_1Delegate)` & r)` 





#### `public inline virtual  ~Delegate()` 





#### `public inline BaseT * clone() const` 





#### `public inline void emit(void * sender,P arg,P2 arg2,P3 arg3,P4 arg4)` 





#### `public inline bool equals(const `[`DerivedT`](#structscy_1_1DelegateBase)` * r) const` 





#### `public inline void cancel()` 





#### `public inline bool cancelled() const` 





#### `public inline int priority() const` 





#### `public inline void * object() const` 





#### `protected int _priority` 





#### `protected bool _cancelled` 





#### `protected  Delegate()` 





# class `scy::Idler` 

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
`protected `[`uv::Handle`](#classscy_1_1uv_1_1Handle)` _handle` | 
`protected virtual void init()` | 
`protected virtual void startAsync()` | Start the context from the control thread.
`protected virtual bool async() const` | 

## Members

#### `public  Idler(uv::Loop * loop)` 

Create the idler context the given event loop and method.



#### `public  Idler(uv::Loop * loop,std::function< void()> target)` 





#### `public  Idler(uv::Loop * loop,std::function< void(void *)> target,void * arg)` 





#### `public virtual  ~Idler()` 





#### `public `[`uv::Handle`](#classscy_1_1uv_1_1Handle)` & handle()` 





#### `protected `[`uv::Handle`](#classscy_1_1uv_1_1Handle)` _handle` 





#### `protected virtual void init()` 





#### `protected virtual void startAsync()` 

Start the context from the control thread.



#### `protected virtual bool async() const` 



Returns true if the implementation is thread-based, or false if it belongs to an event loop.

# class `scy::basic::Decoder` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  Decoder()` | 
`public inline virtual  ~Decoder()` | 
`public std::size_t decode(const char * inbuf,std::size_t nread,char * outbuf)` | 
`public inline virtual std::size_t finalize(char *)` | 

## Members

#### `public inline  Decoder()` 





#### `public inline virtual  ~Decoder()` 





#### `public std::size_t decode(const char * inbuf,std::size_t nread,char * outbuf)` 





#### `public inline virtual std::size_t finalize(char *)` 





# class `scy::basic::Encoder` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  Encoder()` | 
`public inline virtual  ~Encoder()` | 
`public std::size_t encode(const char * inbuf,std::size_t nread,char * outbuf)` | 
`public inline virtual std::size_t finalize(char *)` | 

## Members

#### `public inline  Encoder()` 





#### `public inline virtual  ~Encoder()` 





#### `public std::size_t encode(const char * inbuf,std::size_t nread,char * outbuf)` 





#### `public inline virtual std::size_t finalize(char *)` 





# class `scy::basic::Polymorphic` 


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

#### `public inline virtual  ~Polymorphic()` 





#### `public template<class T>`  <br/>`inline bool is()` 





#### `public template<class T>`  <br/>`inline T * as(bool whiny)` 





#### `public `[`scy::LogStream`](#structscy_1_1LogStream)` & log(const char * level) const` 





#### `public const char * className() const` 





# class `scy::ipc::Queue` 




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
`protected mutable `[`Mutex`](#classscy_1_1Mutex)` _mutex` | 
`protected std::deque< TAction * > _actions` | 

## Members

#### `public inline  Queue()` 





#### `public inline virtual  ~Queue()` 





#### `public inline virtual void push(TAction * action)` 





#### `public inline virtual TAction * pop()` 





#### `public inline virtual void runSync()` 





#### `public inline virtual void close()` 





#### `public inline virtual void post()` 





#### `public inline void waitForSync()` 





#### `protected mutable `[`Mutex`](#classscy_1_1Mutex)` _mutex` 





#### `protected std::deque< TAction * > _actions` 





# class `scy::ipc::SyncQueue` 

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
`protected `[`SyncContext`](#classscy_1_1SyncContext)` _sync` | 

## Members

#### `public inline  SyncQueue(uv::Loop * loop)` 





#### `public inline virtual  ~SyncQueue()` 





#### `public inline virtual void close()` 





#### `public inline virtual void post()` 





#### `public inline virtual `[`SyncContext`](#classscy_1_1SyncContext)` & sync()` 





#### `protected `[`SyncContext`](#classscy_1_1SyncContext)` _sync` 





# class `scy::LogWriter` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  LogWriter()` | 
`public virtual  ~LogWriter()` | 
`public virtual void write(`[`LogStream`](#structscy_1_1LogStream)` * stream)` | Writes the given log message stream.

## Members

#### `public  LogWriter()` 





#### `public virtual  ~LogWriter()` 





#### `public virtual void write(`[`LogStream`](#structscy_1_1LogStream)` * stream)` 

Writes the given log message stream.



# class `scy::AsyncLogWriter` 

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
`protected `[`Thread`](#classscy_1_1Thread)` _thread` | 
`protected std::deque< `[`LogStream`](#structscy_1_1LogStream)` * > _pending` | 
`protected mutable `[`Mutex`](#classscy_1_1Mutex)` _mutex` | 
`protected bool writeNext()` | 

## Members

#### `public  AsyncLogWriter()` 





#### `public virtual  ~AsyncLogWriter()` 





#### `public virtual void write(`[`LogStream`](#structscy_1_1LogStream)` * stream)` 

Queues the given log message stream.



#### `public void flush()` 

Flushes queued messages.



#### `public virtual void run()` 

Writes queued messages asynchronously.



#### `public void clear()` 

Clears all queued messages.



#### `protected `[`Thread`](#classscy_1_1Thread)` _thread` 





#### `protected std::deque< `[`LogStream`](#structscy_1_1LogStream)` * > _pending` 





#### `protected mutable `[`Mutex`](#classscy_1_1Mutex)` _mutex` 





#### `protected bool writeNext()` 





# class `scy::Logger` 






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
`protected mutable `[`Mutex`](#classscy_1_1Mutex)` _mutex` | 
`protected LogChannelMap _channels` | 
`protected `[`LogChannel`](#classscy_1_1LogChannel)` * _defaultChannel` | 
`protected `[`LogWriter`](#classscy_1_1LogWriter)` * _writer` | 
`protected  Logger(const `[`Logger`](#classscy_1_1Logger)` &)` | Non-copyable and non-movable.
`protected  Logger(`[`Logger`](#classscy_1_1Logger)` &&)` | 
`protected `[`Logger`](#classscy_1_1Logger)` & operator=(const `[`Logger`](#classscy_1_1Logger)` &)` | 
`protected `[`Logger`](#classscy_1_1Logger)` & operator=(`[`Logger`](#classscy_1_1Logger)` &&)` | 

## Members

#### `public  Logger()` 





#### `public  ~Logger()` 





#### `public void add(`[`LogChannel`](#classscy_1_1LogChannel)` * channel)` 

Adds the given log channel.



#### `public void remove(const std::string & name,bool freePointer)` 



Removes the given log channel by name, and optionally frees the pointer.

#### `public `[`LogChannel`](#classscy_1_1LogChannel)` * get(const std::string & name,bool whiny) const` 



Returns the specified log channel. Throws an exception if the channel doesn't exist.

#### `public void setDefault(const std::string & name)` 

Sets the default log to the specified log channel.



#### `public void setWriter(`[`LogWriter`](#classscy_1_1LogWriter)` * writer)` 

Sets the log writer instance.



#### `public `[`LogChannel`](#classscy_1_1LogChannel)` * getDefault() const` 



Returns the default log channel, or the nullptr channel if no default channel has been set.

#### `public void write(const `[`LogStream`](#structscy_1_1LogStream)` & stream)` 



Writes the given message to the default log channel. The message will be copied.

#### `public void write(`[`LogStream`](#structscy_1_1LogStream)` * stream)` 



Writes the given message to the default log channel. The stream pointer will be deleted when appropriate.

#### `public `[`LogStream`](#structscy_1_1LogStream)` & send(const char * level,const char * realm,const void * ptr,const char * channel) const` 



Sends to the default log using the given class instance. Recommend using write(LogStream&) to avoid copying data.

#### `protected mutable `[`Mutex`](#classscy_1_1Mutex)` _mutex` 





#### `protected LogChannelMap _channels` 





#### `protected `[`LogChannel`](#classscy_1_1LogChannel)` * _defaultChannel` 





#### `protected `[`LogWriter`](#classscy_1_1LogWriter)` * _writer` 





#### `protected  Logger(const `[`Logger`](#classscy_1_1Logger)` &)` 

Non-copyable and non-movable.



#### `protected  Logger(`[`Logger`](#classscy_1_1Logger)` &&)` 





#### `protected `[`Logger`](#classscy_1_1Logger)` & operator=(const `[`Logger`](#classscy_1_1Logger)` &)` 





#### `protected `[`Logger`](#classscy_1_1Logger)` & operator=(`[`Logger`](#classscy_1_1Logger)` &&)` 





# class `scy::LogChannel` 






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

#### `public  LogChannel(const std::string & name,LogLevel level,const std::string & timeFormat)` 





#### `public inline virtual  ~LogChannel()` 





#### `public virtual void write(const `[`LogStream`](#structscy_1_1LogStream)` & stream)` 





#### `public virtual void write(const std::string & message,LogLevel level,const char * realm,const void * ptr)` 





#### `public virtual void format(const `[`LogStream`](#structscy_1_1LogStream)` & stream,std::ostream & ost)` 





#### `public inline std::string name() const` 





#### `public inline LogLevel level() const` 





#### `public inline std::string timeFormat() const` 





#### `public inline void setLevel(LogLevel level)` 





#### `public inline void setDateFormat(const std::string & format)` 





#### `public inline void setFilter(const std::string & filter)` 





#### `protected std::string _name` 





#### `protected LogLevel _level` 





#### `protected std::string _timeFormat` 





#### `protected std::string _filter` 





# class `scy::ConsoleChannel` 

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

#### `public  ConsoleChannel(const std::string & name,LogLevel level,const std::string & timeFormat)` 





#### `public inline virtual  ~ConsoleChannel()` 





#### `public virtual void write(const `[`LogStream`](#structscy_1_1LogStream)` & stream)` 





# class `scy::FileChannel` 

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

#### `public  FileChannel(const std::string & name,const std::string & path,LogLevel level,const char * timeFormat)` 





#### `public virtual  ~FileChannel()` 





#### `public virtual void write(const `[`LogStream`](#structscy_1_1LogStream)` & stream)` 





#### `public void setPath(const std::string & path)` 





#### `public std::string path() const` 





#### `protected std::ofstream _fstream` 





#### `protected std::string _path` 





#### `protected virtual void open()` 





#### `protected virtual void close()` 





# class `scy::RotatingFileChannel` 

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

#### `public  RotatingFileChannel(const std::string & name,const std::string & dir,LogLevel level,const std::string & extension,int rotationInterval,const char * timeFormat)` 





#### `public virtual  ~RotatingFileChannel()` 





#### `public virtual void write(const `[`LogStream`](#structscy_1_1LogStream)` & stream)` 





#### `public virtual void rotate()` 





#### `public inline std::string dir() const` 





#### `public inline std::string filename() const` 





#### `public inline int rotationInterval() const` 





#### `public inline void setDir(const std::string & dir)` 





#### `public inline void setExtension(const std::string & ext)` 





#### `public inline void setRotationInterval(int interval)` 





#### `protected std::ofstream * _fstream` 





#### `protected std::string _dir` 





#### `protected std::string _filename` 





#### `protected std::string _extension` 





#### `protected int _rotationInterval` 

The log rotation interval in seconds.



#### `protected time_t _rotatedAt` 

The time the log was last rotated.



# class `scy::GarbageCollector` 


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
`protected mutable `[`Mutex`](#classscy_1_1Mutex)` _mutex` | 
`protected std::vector< `[`ScopedPointer`](#classscy_1_1ScopedPointer)` * > _pending` | 
`protected std::vector< `[`ScopedPointer`](#classscy_1_1ScopedPointer)` * > _ready` | 
`protected `[`uv::Handle`](#classscy_1_1uv_1_1Handle)` _handle` | 
`protected bool _finalize` | 
`protected uv_thread_t _tid` | 
`protected void runAsync()` | 

## Members

#### `public  GarbageCollector()` 





#### `public  ~GarbageCollector()` 





#### `public template<class C>`  <br/>`inline void deleteLater(C * ptr)` 

Schedules a pointer for deferred deletion.



#### `public template<class C>`  <br/>`inline void deleteLater(std::shared_ptr< C > ptr)` 

Schedules a shared pointer for deferred deletion.



#### `public void finalize()` 



Frees all scheduled pointers now. This method must be called from the main thread while the event loop is inactive.

#### `public uv_thread_t tid()` 



Returns the TID of the garbage collector event loop thread. The garbage collector must be running.

#### `protected mutable `[`Mutex`](#classscy_1_1Mutex)` _mutex` 





#### `protected std::vector< `[`ScopedPointer`](#classscy_1_1ScopedPointer)` * > _pending` 





#### `protected std::vector< `[`ScopedPointer`](#classscy_1_1ScopedPointer)` * > _ready` 





#### `protected `[`uv::Handle`](#classscy_1_1uv_1_1Handle)` _handle` 





#### `protected bool _finalize` 





#### `protected uv_thread_t _tid` 





#### `protected void runAsync()` 





# class `scy::ScopedPointer` 


Scoped Pointer Classes.

[ScopedPointer](#classscy_1_1ScopedPointer) provides an interface for holding and ansynchronously deleting a pointer in various ways.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  ScopedPointer()` | 
`public inline virtual  ~ScopedPointer()` | 

## Members

#### `public inline  ScopedPointer()` 





#### `public inline virtual  ~ScopedPointer()` 





# class `scy::ScopedRawPointer` 

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

#### `public void * ptr` 





#### `public inline  ScopedRawPointer(void * p)` 





#### `public inline virtual  ~ScopedRawPointer()` 





# class `scy::ScopedSharedPointer` 

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

#### `public std::shared_ptr< T > ptr` 





#### `public inline  ScopedSharedPointer(std::shared_ptr< T > p)` 





#### `public inline virtual  ~ScopedSharedPointer()` 





# class `scy::SharedObject` 




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

#### `public inline  SharedObject(bool deferred)` 



Creates the [SharedObject](#classscy_1_1SharedObject) with an initial reference count of one.

#### `public inline void duplicate()` 

Increment the object's reference count.



#### `public inline void release()` 



Decrement the object's reference count and calls delete if the count reaches zero.

#### `public inline unsigned refCount() const` 





#### `protected std::atomic< unsigned > count` 





#### `protected bool deferred` 





#### `protected inline virtual void freeMemory()` 



Deletes the instance when the reference count reaches zero. This method can be overridden for different deletion strategies.

#### `protected inline virtual  ~SharedObject()` 



Destroys the [SharedObject](#classscy_1_1SharedObject). The destructor should never be called directly.

#### `protected  SharedObject(const `[`SharedObject`](#classscy_1_1SharedObject)` &)` 





#### `protected `[`SharedObject`](#classscy_1_1SharedObject)` & operator=(const `[`SharedObject`](#classscy_1_1SharedObject)` &)` 





# class `scy::ScopedLock` 




[ScopedLock](#classscy_1_1ScopedLock) simplifies thread synchronization with a [Mutex](#classscy_1_1Mutex) or similar lockable object. The given [Mutex](#classscy_1_1Mutex) is locked in the constructor, /// and unlocked it in the destructor. T can be any class with lock() and unlock() functions.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  explicit ScopedLock(T & m)` | 
`public inline  ~ScopedLock()` | 

## Members

#### `public inline  explicit ScopedLock(T & m)` 





#### `public inline  ~ScopedLock()` 





# class `scy::Mutex` 




This class is a wrapper around uv_mutex_t.

A [Mutex](#classscy_1_1Mutex) (mutual exclusion) is a synchronization mechanism used to control access to a shared resource in a concurrent (multithreaded) scenario.

The [ScopedLock](#classscy_1_1ScopedLock) class is usually used to obtain a [Mutex](#classscy_1_1Mutex) lock, since it makes locking exception-safe.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Mutex()` | 
`public  ~Mutex()` | 
`public void lock()` | 
`public bool tryLock()` | 
`public void unlock()` | 

## Members

#### `public  Mutex()` 





#### `public  ~Mutex()` 





#### `public void lock()` 



Locks the mutex. Blocks if the mutex is held by another thread.

#### `public bool tryLock()` 



Tries to lock the mutex. Returns false if the mutex is already held by another thread. Returns true if the mutex was successfully locked.

#### `public void unlock()` 



Unlocks the mutex so that it can be acquired by other threads.

# class `scy::IPacket` 

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
`public `[`IPacketInfo`](#structscy_1_1IPacketInfo)` * info` | 
`public `[`Bitwise`](#structscy_1_1Bitwise)` flags` | Provides basic information about the packet.
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

#### `public void * source` 



Packet source pointer reference which enables processors along the signal chain can determine the packet origin. Often a subclass of PacketStreamSource.

#### `public void * opaque` 



Optional client data pointer. This pointer is not managed by the packet.

#### `public `[`IPacketInfo`](#structscy_1_1IPacketInfo)` * info` 



Optional extra information about the packet. This pointer is managed by the packet.

#### `public `[`Bitwise`](#structscy_1_1Bitwise)` flags` 

Provides basic information about the packet.



#### `public inline  IPacket(void * source,void * opaque,`[`IPacketInfo`](#structscy_1_1IPacketInfo)` * info,unsigned flags)` 





#### `public inline  IPacket(const `[`IPacket`](#classscy_1_1IPacket)` & r)` 





#### `public inline `[`IPacket`](#classscy_1_1IPacket)` & operator=(const `[`IPacket`](#classscy_1_1IPacket)` & r)` 





#### `public `[`IPacket`](#classscy_1_1IPacket)` * clone() const` 





#### `public inline virtual  ~IPacket()` 





#### `public std::size_t read(const `[`ConstBuffer`](#classscy_1_1ConstBuffer)` &)` 



Read/parse to the packet from the given input buffer. The number of bytes read is returned.

#### `public void write(Buffer &) const` 



Copy/generate to the packet given output buffer. The number of bytes written can be obtained from the buffer.

Todo: It may be prefferable to use our pod types here instead of buffer input, but the current codebase requires that the buffer be dynamically resizable for some protocols...

virtual std::size_t write(MutableBuffer&) const = 0;

#### `public inline virtual std::size_t size() const` 



The size of the packet in bytes.

This is the nember of bytes that will be written on a call to [write()](#group__base_1gabdcb765c58d74be25e190680bf7483d6), but may not be the number of bytes that will be consumed by [read()](#group__base_1ga0886a2812d86d64bbb47b50788cb791a).

#### `public inline virtual bool hasData() const` 





#### `public inline virtual char * data() const` 

The packet data pointer for buffered packets.



#### `public inline virtual const char * constData() const` 

The const packet data pointer for buffered packets.



#### `public const char * className() const` 





#### `public inline virtual void print(std::ostream & os) const` 





# class `scy::FlagPacket` 

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

#### `public inline  FlagPacket(unsigned flags)` 





#### `public inline virtual `[`IPacket`](#classscy_1_1IPacket)` * clone() const` 





#### `public inline  FlagPacket(const `[`FlagPacket`](#classscy_1_1FlagPacket)` & that)` 





#### `public inline virtual  ~FlagPacket()` 





#### `public inline virtual std::size_t read(const `[`ConstBuffer`](#classscy_1_1ConstBuffer)` &)` 



Read/parse to the packet from the given input buffer. The number of bytes read is returned.

#### `public inline virtual void write(Buffer &) const` 



Copy/generate to the packet given output buffer. The number of bytes written can be obtained from the buffer.

Todo: It may be prefferable to use our pod types here instead of buffer input, but the current codebase requires that the buffer be dynamically resizable for some protocols...

virtual std::size_t write(MutableBuffer&) const = 0;

#### `public inline virtual const char * className() const` 





# class `scy::RawPacket` 

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

#### `public char * _data` 





#### `public std::size_t _size` 





#### `public bool _free` 





#### `public inline  RawPacket(char * data,std::size_t size,unsigned flags,void * source,void * opaque,`[`IPacketInfo`](#structscy_1_1IPacketInfo)` * info)` 





#### `public inline  RawPacket(const char * data,std::size_t size,unsigned flags,void * source,void * opaque,`[`IPacketInfo`](#structscy_1_1IPacketInfo)` * info)` 





#### `public inline  RawPacket(const `[`RawPacket`](#classscy_1_1RawPacket)` & that)` 





#### `public inline virtual  ~RawPacket()` 





#### `public inline virtual `[`IPacket`](#classscy_1_1IPacket)` * clone() const` 





#### `public inline virtual void setData(char * data,std::size_t size)` 





#### `public inline virtual void copyData(const char * data,std::size_t size)` 





#### `public inline virtual std::size_t read(const `[`ConstBuffer`](#classscy_1_1ConstBuffer)` &)` 



Read/parse to the packet from the given input buffer. The number of bytes read is returned.

#### `public inline virtual void write(Buffer &) const` 



Copy/generate to the packet given output buffer. The number of bytes written can be obtained from the buffer.

Todo: It may be prefferable to use our pod types here instead of buffer input, but the current codebase requires that the buffer be dynamically resizable for some protocols...

virtual std::size_t write(MutableBuffer&) const = 0;

#### `public inline virtual char * data() const` 

The packet data pointer for buffered packets.



#### `public inline virtual std::size_t size() const` 



The size of the packet in bytes.

This is the nember of bytes that will be written on a call to [write()](#group__base_1gaa237661815dfe9a2879a816a851b8329), but may not be the number of bytes that will be consumed by [read()](#group__base_1ga2fa8aec0d5020bba8911318dbadb6fd6).

#### `public inline virtual const char * className() const` 





#### `public inline virtual bool ownsBuffer() const` 





#### `public inline virtual void assignDataOwnership()` 





# class `scy::IPacketCreationStrategy` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  IPacketCreationStrategy()` | 
`public inline virtual  ~IPacketCreationStrategy()` | 
`public `[`IPacket`](#classscy_1_1IPacket)` * create(const `[`ConstBuffer`](#classscy_1_1ConstBuffer)` & buffer,std::size_t & nread) const` | 
`public int priority() const` | 

## Members

#### `public inline  IPacketCreationStrategy()` 





#### `public inline virtual  ~IPacketCreationStrategy()` 





#### `public `[`IPacket`](#classscy_1_1IPacket)` * create(const `[`ConstBuffer`](#classscy_1_1ConstBuffer)` & buffer,std::size_t & nread) const` 





#### `public int priority() const` 





# class `scy::PacketFactory` 






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

#### `public inline  PacketFactory()` 





#### `public inline virtual  ~PacketFactory()` 





#### `public template<class PacketT>`  <br/>`inline void registerPacketType(int priority)` 





#### `public template<class PacketT>`  <br/>`inline void unregisterPacketType()` 





#### `public template<class StrategyT>`  <br/>`inline void registerStrategy(int priority)` 





#### `public template<class StrategyT>`  <br/>`inline void unregisterStrategy()` 





#### `public inline PacketCreationStrategyList & types()` 





#### `public inline PacketCreationStrategyList types() const` 





#### `public inline virtual bool onPacketCreated(`[`IPacket`](#classscy_1_1IPacket)` *)` 

returning false will stop packet propagation



#### `public inline virtual `[`IPacket`](#classscy_1_1IPacket)` * createPacket(const `[`ConstBuffer`](#classscy_1_1ConstBuffer)` & buffer,std::size_t & nread)` 





#### `protected PacketCreationStrategyList _types` 





# class `scy::ThreadedStreamReader` 

```
class scy::ThreadedStreamReader
  : public scy::PacketStreamAdapter
  : public scy::async::Startable
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`PacketSignal`](#classscy_1_1SignalBase)` emitter` | 
`public inline  ThreadedStreamReader(std::istream * is)` | 
`public inline  ~ThreadedStreamReader()` | 
`public inline virtual void start()` | 
`public inline virtual void stop()` | 
`public template<class StreamT>`  <br/>`inline StreamT & stream()` | 
`public inline std::istream & stream()` | 
`protected `[`Thread`](#classscy_1_1Thread)` _runner` | 
`protected std::istream * _istream` | 

## Members

#### `public `[`PacketSignal`](#classscy_1_1SignalBase)` emitter` 





#### `public inline  ThreadedStreamReader(std::istream * is)` 





#### `public inline  ~ThreadedStreamReader()` 





#### `public inline virtual void start()` 





#### `public inline virtual void stop()` 





#### `public template<class StreamT>`  <br/>`inline StreamT & stream()` 





#### `public inline std::istream & stream()` 





#### `protected `[`Thread`](#classscy_1_1Thread)` _runner` 





#### `protected std::istream * _istream` 





# class `scy::StreamWriter` 

```
class scy::StreamWriter
  : public scy::PacketProcessor
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`PacketSignal`](#classscy_1_1SignalBase)` emitter` | 
`public inline  StreamWriter(std::ostream * stream)` | 
`public inline virtual  ~StreamWriter()` | 
`public inline virtual void process(`[`IPacket`](#classscy_1_1IPacket)` & packet)` | 
`public template<class StreamT>`  <br/>`inline StreamT & stream()` | 
`public inline std::ostream & stream()` | 
`protected std::ostream * _ostream` | 

## Members

#### `public `[`PacketSignal`](#classscy_1_1SignalBase)` emitter` 





#### `public inline  StreamWriter(std::ostream * stream)` 





#### `public inline virtual  ~StreamWriter()` 





#### `public inline virtual void process(`[`IPacket`](#classscy_1_1IPacket)` & packet)` 



This method performs processing on the given packet and emits the result.

Note: If packet processing is async (the packet is not in the current thread scope) then packet data must be copied. Copied data can be freed directly aFter the async call to emit() the outgoing packet.

#### `public template<class StreamT>`  <br/>`inline StreamT & stream()` 





#### `public inline std::ostream & stream()` 





#### `protected std::ostream * _ostream` 





# class `scy::SyncPacketQueue` 

```
class scy::SyncPacketQueue
  : public scy::SyncQueue< T >
  : public scy::PacketProcessor
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`PacketSignal`](#classscy_1_1SignalBase)` emitter` | 
`public inline  SyncPacketQueue(uv::Loop * loop,int maxSize)` | 
`public inline  SyncPacketQueue(int maxSize)` | 
`public inline virtual  ~SyncPacketQueue()` | 
`public inline virtual void process(`[`IPacket`](#classscy_1_1IPacket)` & packet)` | 
`public inline virtual bool accepts(`[`IPacket`](#classscy_1_1IPacket)` &)` | 
`protected inline virtual void dispatch(T & packet)` | 
`protected inline virtual void onStreamStateChange(const `[`PacketStreamState`](#structscy_1_1PacketStreamState)` &)` | 

## Members

#### `public `[`PacketSignal`](#classscy_1_1SignalBase)` emitter` 





#### `public inline  SyncPacketQueue(uv::Loop * loop,int maxSize)` 





#### `public inline  SyncPacketQueue(int maxSize)` 





#### `public inline virtual  ~SyncPacketQueue()` 





#### `public inline virtual void process(`[`IPacket`](#classscy_1_1IPacket)` & packet)` 



This method performs processing on the given packet and emits the result.

Note: If packet processing is async (the packet is not in the current thread scope) then packet data must be copied. Copied data can be freed directly aFter the async call to emit() the outgoing packet.

#### `public inline virtual bool accepts(`[`IPacket`](#classscy_1_1IPacket)` &)` 



This method ensures compatibility with the given packet type. Return false to reject the packet.

#### `protected inline virtual void dispatch(T & packet)` 



Emit should never be called after closure. Any late packets should have been dealt with and dropped by the [run()](#group__base_1ga372c52aa2d9b4c33f9b620dfb2c2af23) function.

#### `protected inline virtual void onStreamStateChange(const `[`PacketStreamState`](#structscy_1_1PacketStreamState)` &)` 



Called by the [PacketStream](#classscy_1_1PacketStream) to notify when the internal [Stream](#classscy_1_1Stream) state changes. On receiving the Stopped state, it is the responsibility of the adapter to have ceased all outgoing packet transmission, especially in multi-thread scenarios.

# class `scy::AsyncPacketQueue` 

```
class scy::AsyncPacketQueue
  : public scy::AsyncQueue< T >
  : public scy::PacketProcessor
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`PacketSignal`](#classscy_1_1SignalBase)` emitter` | 
`public inline  AsyncPacketQueue(int maxSize)` | 
`public inline virtual  ~AsyncPacketQueue()` | 
`public inline virtual void close()` | 
`public inline virtual void process(`[`IPacket`](#classscy_1_1IPacket)` & packet)` | 
`public inline virtual bool accepts(`[`IPacket`](#classscy_1_1IPacket)` &)` | 
`protected inline virtual void dispatch(T & packet)` | 
`protected inline virtual void onStreamStateChange(const `[`PacketStreamState`](#structscy_1_1PacketStreamState)` &)` | 

## Members

#### `public `[`PacketSignal`](#classscy_1_1SignalBase)` emitter` 





#### `public inline  AsyncPacketQueue(int maxSize)` 





#### `public inline virtual  ~AsyncPacketQueue()` 





#### `public inline virtual void close()` 





#### `public inline virtual void process(`[`IPacket`](#classscy_1_1IPacket)` & packet)` 



This method performs processing on the given packet and emits the result.

Note: If packet processing is async (the packet is not in the current thread scope) then packet data must be copied. Copied data can be freed directly aFter the async call to emit() the outgoing packet.

#### `public inline virtual bool accepts(`[`IPacket`](#classscy_1_1IPacket)` &)` 



This method ensures compatibility with the given packet type. Return false to reject the packet.

#### `protected inline virtual void dispatch(T & packet)` 





#### `protected inline virtual void onStreamStateChange(const `[`PacketStreamState`](#structscy_1_1PacketStreamState)` &)` 



Called by the [PacketStream](#classscy_1_1PacketStream) to notify when the internal [Stream](#classscy_1_1Stream) state changes. On receiving the Stopped state, it is the responsibility of the adapter to have ceased all outgoing packet transmission, especially in multi-thread scenarios.

# class `scy::PacketStreamAdapter` 




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
`protected `[`PacketSignal`](#classscy_1_1SignalBase)` & _emitter` | 
`protected  PacketStreamAdapter(const `[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` &)` | 
`protected  PacketStreamAdapter(`[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` &&)` | 
`protected `[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` & operator=(const `[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` &)` | 
`protected `[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` & operator=(`[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` &&)` | 

## Members

#### `public  PacketStreamAdapter(`[`PacketSignal`](#classscy_1_1SignalBase)` & emitter)` 





#### `public inline virtual  ~PacketStreamAdapter()` 





#### `public virtual void emit(char * data,std::size_t len,unsigned flags)` 





#### `public virtual void emit(const char * data,std::size_t len,unsigned flags)` 





#### `public virtual void emit(const std::string & str,unsigned flags)` 





#### `public virtual void emit(unsigned flags)` 





#### `public virtual void emit(`[`IPacket`](#classscy_1_1IPacket)` & packet)` 





#### `public `[`PacketSignal`](#classscy_1_1SignalBase)` & getEmitter()` 

Returns a reference to the outgoing packet signal.



#### `public inline virtual void onStreamStateChange(const `[`PacketStreamState`](#structscy_1_1PacketStreamState)` &)` 



Called by the [PacketStream](#classscy_1_1PacketStream) to notify when the internal [Stream](#classscy_1_1Stream) state changes. On receiving the Stopped state, it is the responsibility of the adapter to have ceased all outgoing packet transmission, especially in multi-thread scenarios.

#### `protected `[`PacketSignal`](#classscy_1_1SignalBase)` & _emitter` 





#### `protected  PacketStreamAdapter(const `[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` &)` 





#### `protected  PacketStreamAdapter(`[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` &&)` 





#### `protected `[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` & operator=(const `[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` &)` 





#### `protected `[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` & operator=(`[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` &&)` 





# class `scy::PacketProcessor` 

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

#### `public inline  PacketProcessor(`[`PacketSignal`](#classscy_1_1SignalBase)` & emitter)` 





#### `public void process(`[`IPacket`](#classscy_1_1IPacket)` & packet)` 



This method performs processing on the given packet and emits the result.

Note: If packet processing is async (the packet is not in the current thread scope) then packet data must be copied. Copied data can be freed directly aFter the async call to emit() the outgoing packet.

#### `public inline virtual bool accepts(`[`IPacket`](#classscy_1_1IPacket)` &)` 



This method ensures compatibility with the given packet type. Return false to reject the packet.

#### `public inline virtual void operator<<(`[`IPacket`](#classscy_1_1IPacket)` & packet)` 

[Stream](#classscy_1_1Stream) operator alias for [process()](#group__base_1gad7adc3fd78dce41f0f96744dfe6d1d50)



# class `scy::PacketStream` 

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
`public `[`PacketSignal`](#classscy_1_1SignalBase)` emitter` | Signals to delegates on outgoing packets.
`public `[`Signal`](#classscy_1_1Signal)`< const std::exception_ptr & > Error` | 
`public `[`NullSignal`](#classscy_1_1NullSignal)` Close` | 
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
`public virtual bool closed() const` | Returns true when the stream is in the Closed or [Error](#structscy_1_1Error) state.
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
`public inline `[`PacketProcessor`](#classscy_1_1PacketProcessor)` * getProcessor(int order)` | Returns the [PacketProcessor](#classscy_1_1PacketProcessor) at the given position.
`protected mutable `[`Mutex`](#classscy_1_1Mutex)` _mutex` | 
`protected mutable `[`Mutex`](#classscy_1_1Mutex)` _procMutex` | 
`protected std::string _name` | 
`protected PacketAdapterVec _sources` | 
`protected PacketAdapterVec _processors` | 
`protected std::deque< `[`PacketStreamState`](#structscy_1_1PacketStreamState)` > _states` | 
`protected std::exception_ptr _error` | 
`protected bool _autoStart` | 
`protected bool _closeOnError` | 
`protected void * _clientData` | 
`protected void setup()` | Attach the source and processor delegate chain.
`protected void teardown()` | Detach the source and processor delegate chain.
`protected void emit(`[`IPacket`](#classscy_1_1IPacket)` & packet)` | 
`protected void attachSource(PacketAdapterReference::Ptr ref)` | 
`protected void attach(PacketAdapterReference::Ptr ref)` | 
`protected void process(`[`IPacket`](#classscy_1_1IPacket)` & packet)` | [Process](#classscy_1_1Process) incoming packets.
`protected void startSources()` | Start synchronized sources.
`protected void stopSources()` | Stop synchronized sources.
`protected void synchronizeStates()` | Synchronize queued states with adapters.
`protected virtual void onStateChange(`[`PacketStreamState`](#structscy_1_1PacketStreamState)` & state,const `[`PacketStreamState`](#structscy_1_1PacketStreamState)` & oldState)` | Override the [Stateful::onStateChange](#group__base_1ga3c88b4aa8e9343f9bdc1a8c2ae2a5452) method.
`protected bool hasQueuedState(PacketStreamState::ID state) const` | Returns true if the given state ID is queued.
`protected void assertCanModify()` | 

## Members

#### `public `[`PacketSignal`](#classscy_1_1SignalBase)` emitter` 

Signals to delegates on outgoing packets.



#### `public `[`Signal`](#classscy_1_1Signal)`< const std::exception_ptr & > Error` 



Signals that the [PacketStream](#classscy_1_1PacketStream) is in [Error](#structscy_1_1Error) state. If stream output is synchronized then the [Error](#structscy_1_1Error) signal will be sent from the synchronization context, otherwise it will be sent from the async processor context. See [synchronizeOutput()](#group__base_1ga27a797846a098946cf73d80726302a9e)

#### `public `[`NullSignal`](#classscy_1_1NullSignal)` Close` 



Signals that the [PacketStream](#classscy_1_1PacketStream) is in Close state. This signal is sent immediately via the [close()](#group__base_1gac0eec4eda5ff0571b46b4a39af9f7dcf) method, and as such will be sent from the calling thread context.

#### `public  PacketStream(const std::string & name)` 





#### `public virtual  ~PacketStream()` 





#### `public virtual void start()` 

Start the stream and synchronized sources.



#### `public virtual void stop()` 

Stop the stream and synchronized sources.



#### `public virtual void pause()` 

Pause the stream.



#### `public virtual void resume()` 

Resume the stream.



#### `public virtual void close()` 

Close the stream and transition the internal state to Closed.



#### `public virtual void reset()` 

Cleanup all managed stream adapters and reset the stream state.



#### `public virtual bool active() const` 

Returns true when the stream is in the Active state.



#### `public virtual bool stopped() const` 

Returns true when the stream is in the Stopping or Stopped state.



#### `public virtual bool closed() const` 

Returns true when the stream is in the Closed or [Error](#structscy_1_1Error) state.



#### `public virtual bool lock()` 



Sets the stream to locked state. In a locked state no new adapters can be added or removed from the stream until the stream is stopped.

#### `public virtual bool locked() const` 

Returns true is the stream is currently locked.



#### `public virtual void write(char * data,std::size_t len)` 

Writes data to the stream (nocopy).



#### `public virtual void write(const char * data,std::size_t len)` 

Writes data to the stream (copied).



#### `public virtual void write(`[`IPacket`](#classscy_1_1IPacket)` & packet)` 

Writes an incoming packet onto the stream.



#### `public virtual void attachSource(`[`PacketSignal`](#classscy_1_1SignalBase)` & source)` 



Attaches a source packet emitter to the stream. The source packet adapter can be another [PacketStream::emitter](#group__base_1ga3d4fa2d39446cd24ce5d8281834b70db).

#### `public virtual void attachSource(`[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` * source,bool freePointer,bool syncState)` 



Attaches a source packet emitter to the stream. If freePointer is true, the pointer will be deleted when the stream is closed. If syncState is true and the source is a basic::Stratable, then the source's [start()](#group__base_1gaa07075299f2271e58f78de3fd7e62b10)/stop() methods will be synchronized when calling [startSources()](#group__base_1gaeb9605f5d35a6aaa92bf427e0989827c)/stopSources().

#### `public template<class C>`  <br/>`inline void attachSource(std::shared_ptr< C > ptr,bool syncState)` 



Attaches a source packet emitter to the stream. This method enables compatibility with shared_ptr managed adapter instances.

#### `public virtual bool detachSource(`[`PacketSignal`](#classscy_1_1SignalBase)` & source)` 

Detaches the given source packet signal from the stream.



#### `public virtual bool detachSource(`[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` * source)` 



Detaches the given source packet adapter from the stream. Note: The pointer will be forgotten about, so if the freePointer flag set when calling [attachSource()](#group__base_1ga16adec5b00a33070556dd8531e1825c0) will have no effect.

#### `public virtual void attach(`[`PacketProcessor`](#classscy_1_1PacketProcessor)` * proc,int order,bool freePointer)` 



Attaches a packet processor to the stream. Order determines the position of the processor in the stream queue. If freePointer is true, the pointer will be deleted when the stream closes.

#### `public template<class C>`  <br/>`inline void attach(std::shared_ptr< C > ptr,bool syncState)` 



Attaches a packet processor to the stream. This method enables compatibility with shared_ptr managed adapter instances.

#### `public virtual bool detach(`[`PacketProcessor`](#classscy_1_1PacketProcessor)` * proc)` 



Detaches a packet processor from the stream. Note: The pointer will be forgotten about, so if the freePointer flag set when calling [attach()](#group__base_1gaeff06c27d0ed0174f7f19ff61b3ff2fe) will have no effect.

#### `public virtual void synchronizeOutput(uv::Loop * loop)` 

Synchronize stream output packets with the given event loop.



#### `public virtual void autoStart(bool flag)` 



Set the stream to auto start if inactive (default: false).

With this flag set the stream will automatically transition to Active state if the in either the None or Locaked state.

#### `public virtual void closeOnError(bool flag)` 



Set the stream to be closed on error (default: true).

With this flag set the stream will be automatically transitioned to Closed state from [Error](#structscy_1_1Error) state.

#### `public virtual void setClientData(void * data)` 

Accessors for the unmanaged client data pointer.



#### `public virtual void * clientData() const` 





#### `public const std::exception_ptr & error()` 

Returns the stream error (if any).



#### `public std::string name() const` 

Returns the name of the packet stream.



#### `public PacketAdapterVec adapters() const` 

Returns a combined list of all stream sources and processors.



#### `public PacketAdapterVec sources() const` 

Returns a list of all stream sources.



#### `public PacketAdapterVec processors() const` 

Returns a list of all stream processors.



#### `public bool waitForRunner()` 



Block the calling thread until all packets have been flushed, and internal states have been synchronized. This function is only useful after calling [stop()](#group__base_1ga5ace99af3f0ff1e93483a7f5dd5dca69) or [pause()](#group__base_1ga866ba120ec05be813d82a6e05827292e).

#### `public bool waitForStateSync(PacketStreamState::ID state)` 

Block the calling thread until the given state is synchronized.



#### `public int numSources() const` 





#### `public int numProcessors() const` 





#### `public int numAdapters() const` 





#### `public template<class AdapterT>`  <br/>`inline AdapterT * getSource(int index)` 





#### `public template<class AdapterT>`  <br/>`inline AdapterT * getProcessor(int index)` 





#### `public inline `[`PacketProcessor`](#classscy_1_1PacketProcessor)` * getProcessor(int order)` 

Returns the [PacketProcessor](#classscy_1_1PacketProcessor) at the given position.



#### `protected mutable `[`Mutex`](#classscy_1_1Mutex)` _mutex` 





#### `protected mutable `[`Mutex`](#classscy_1_1Mutex)` _procMutex` 





#### `protected std::string _name` 





#### `protected PacketAdapterVec _sources` 





#### `protected PacketAdapterVec _processors` 





#### `protected std::deque< `[`PacketStreamState`](#structscy_1_1PacketStreamState)` > _states` 





#### `protected std::exception_ptr _error` 





#### `protected bool _autoStart` 





#### `protected bool _closeOnError` 





#### `protected void * _clientData` 





#### `protected void setup()` 

Attach the source and processor delegate chain.



#### `protected void teardown()` 

Detach the source and processor delegate chain.



#### `protected void emit(`[`IPacket`](#classscy_1_1IPacket)` & packet)` 



Emit the final packet to listeners.

Synchronized signals such as Close and [Error](#structscy_1_1Error) are sent from this method. See [synchronizeOutput()](#group__base_1ga27a797846a098946cf73d80726302a9e)

#### `protected void attachSource(PacketAdapterReference::Ptr ref)` 





#### `protected void attach(PacketAdapterReference::Ptr ref)` 





#### `protected void process(`[`IPacket`](#classscy_1_1IPacket)` & packet)` 

[Process](#classscy_1_1Process) incoming packets.



#### `protected void startSources()` 

Start synchronized sources.



#### `protected void stopSources()` 

Stop synchronized sources.



#### `protected void synchronizeStates()` 

Synchronize queued states with adapters.



#### `protected virtual void onStateChange(`[`PacketStreamState`](#structscy_1_1PacketStreamState)` & state,const `[`PacketStreamState`](#structscy_1_1PacketStreamState)` & oldState)` 

Override the [Stateful::onStateChange](#group__base_1ga3c88b4aa8e9343f9bdc1a8c2ae2a5452) method.



#### `protected bool hasQueuedState(PacketStreamState::ID state) const` 

Returns true if the given state ID is queued.



#### `protected void assertCanModify()` 



Asserts that the stream can be modified, ie is not in the Locked, Stopping or Active states.

# class `scy::PacketTransaction` 

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
`protected `[`Timer`](#classscy_1_1Timer)` _timer` | 
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

#### `public inline  PacketTransaction(long timeout,int retries,uv::Loop * loop)` 





#### `public inline  PacketTransaction(const PacketT & request,long timeout,int retries,uv::Loop * loop)` 





#### `public inline virtual bool send()` 



Starts the transaction timer and sends the request. Overriding classes should implement send logic here.

#### `public inline virtual void cancel()` 



Cancellation means that the agent will not retransmit the request, will not treat the lack of response to be a failure, but will wait the duration of the transaction timeout for a response.

#### `public inline bool cancelled() const` 





#### `public inline virtual void dispose()` 



Schedules the transaction for deferred deletion.

It is safe to call this function while the transaction is still active, providing the call is made from the same loop thread which the timer is running on.

Protected by the base implementation as this is called by the internal state machine.

#### `public inline virtual bool canResend()` 





#### `public inline int attempts() const` 





#### `public inline int retries() const` 





#### `public inline PacketT & request()` 





#### `public inline PacketT request() const` 





#### `public inline PacketT & response()` 





#### `public inline PacketT response() const` 





#### `protected PacketT _request` 





#### `protected PacketT _response` 





#### `protected `[`Timer`](#classscy_1_1Timer)` _timer` 





#### `protected int _timeout` 

The request timeout in milliseconds.



#### `protected int _retries` 

The maximum number of attempts before the transaction is considered failed.



#### `protected int _attempts` 

The number of times the transaction has been sent.



#### `protected bool _destroyed` 





#### `protected inline virtual  ~PacketTransaction()` 





#### `protected inline virtual void onStateChange(`[`TransactionState`](#structscy_1_1TransactionState)` & state,const `[`TransactionState`](#structscy_1_1TransactionState)` &)` 

Override to handle post state change logic.



#### `protected inline virtual bool handlePotentialResponse(const PacketT & packet)` 



Processes a potential response candidate and updates the state accordingly.

#### `protected bool checkResponse(const PacketT & packet)` 



Checks a potential response candidate and returns true on successful match.

#### `protected inline virtual void onResponse()` 

Called when a successful response is received.



#### `protected inline virtual void onTimeout(void *)` 





# class `scy::Process` 

```
class scy::Process
  : public scy::uv::Handle
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::vector< std::string > args` | 
`public std::function< void(std::int64_t)> onexit` | Exit callback; returns the exit status.
`public ProcessOptions options` | [Process](#classscy_1_1Process) options.
`public  Process(uv::Loop * loop)` | 
`public void spawn()` | 
`public bool kill(int signum)` | Kills the process.
`public int pid() const` | Returns the process PID.
`protected uv_process_t _proc` | 

## Members

#### `public std::vector< std::string > args` 



Command line args. STL proxy for options.args

#### `public std::function< void(std::int64_t)> onexit` 

Exit callback; returns the exit status.



#### `public ProcessOptions options` 

[Process](#classscy_1_1Process) options.



#### `public  Process(uv::Loop * loop)` 





#### `public void spawn()` 



Spawns the process. Options must be properly set. Throws and exception on error.

#### `public bool kill(int signum)` 

Kills the process.



#### `public int pid() const` 

Returns the process PID.



#### `protected uv_process_t _proc` 





# class `scy::Queue` 


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

#### `public inline void push(const T & data)` 





#### `public inline bool empty() const` 





#### `public inline T & front()` 





#### `public inline T const  & front() const` 





#### `public inline T & back()` 





#### `public inline T const  & back() const` 





#### `public inline void pop()` 





#### `public template<typename Compare>`  <br/>`inline std::size_t sort()` 





#### `public inline std::size_t size()` 





#### `public inline std::deque< T > & queue()` 





# class `scy::RunnableQueue` 

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
`protected mutable `[`Mutex`](#classscy_1_1Mutex)` _mutex` | queue_t _queue;
`protected  RunnableQueue(const `[`RunnableQueue`](#classscy_1_1RunnableQueue)` &)` | 
`protected `[`RunnableQueue`](#classscy_1_1RunnableQueue)` & operator=(const `[`RunnableQueue`](#classscy_1_1RunnableQueue)` &)` | 
`protected inline virtual T * popNext()` | 
`protected inline virtual bool dispatchNext()` | 

## Members

#### `public std::function< void(T &)> ondispatch` 

Must be set before the queue is running.

The default dispatch function.

#### `public inline  RunnableQueue(int limit,int timeout)` 





#### `public inline virtual  ~RunnableQueue()` 





#### `public inline virtual void push(T * item)` 

The queue takes ownership of the item pointer.



#### `public inline virtual void flush()` 





#### `public inline void clear()` 





#### `public inline virtual void run()` 



bool empty() /// { /// // Disabling mutex lock for bool check. //Mutex::ScopedLock lock(_mutex); /// return empty(); /// } If not timeout is set this method blocks until [cancel()](#group__base_1gadf150b3fdcef33cb5144119460c8729a) // is called, otherwise [runTimeout()](#group__base_1ga834803a488030d079458bc67563ce927) will be called. Pseudo protected for std::bind compatability.

#### `public inline virtual void runTimeout()` 



until the queue is empty or the timeout expires. Pseudo protected for std::bind compatability.

#### `public inline virtual void dispatch(T & item)` 





#### `public inline void setTimeout(int milliseconds)` 

int timeout() /// { /// Mutex::ScopedLock lock(_mutex); /// return _timeout; /// }



#### `protected int _limit` 





#### `protected int _timeout` 





#### `protected mutable `[`Mutex`](#classscy_1_1Mutex)` _mutex` 

queue_t _queue;



#### `protected  RunnableQueue(const `[`RunnableQueue`](#classscy_1_1RunnableQueue)` &)` 





#### `protected `[`RunnableQueue`](#classscy_1_1RunnableQueue)` & operator=(const `[`RunnableQueue`](#classscy_1_1RunnableQueue)` &)` 





#### `protected inline virtual T * popNext()` 





#### `protected inline virtual bool dispatchNext()` 





# class `scy::SyncQueue` 

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
`public inline virtual void push(T * item)` | Item pointers are now managed by the [SyncQueue](#classscy_1_1SyncQueue).
`public inline virtual void cancel()` | 
`public inline `[`SyncContext`](#classscy_1_1SyncContext)` & sync()` | 
`protected `[`SyncContext`](#classscy_1_1SyncContext)` _sync` | 

## Members

#### `public inline  SyncQueue(uv::Loop * loop,int limit,int timeout)` 





#### `public inline virtual  ~SyncQueue()` 

time for all callbacks to return.



#### `public inline virtual void push(T * item)` 

Item pointers are now managed by the [SyncQueue](#classscy_1_1SyncQueue).



#### `public inline virtual void cancel()` 





#### `public inline `[`SyncContext`](#classscy_1_1SyncContext)` & sync()` 





#### `protected `[`SyncContext`](#classscy_1_1SyncContext)` _sync` 





# class `scy::AsyncQueue` 

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
`protected `[`Thread`](#classscy_1_1Thread)` _thread` | 
`protected inline virtual  ~AsyncQueue()` | 

## Members

#### `public inline  AsyncQueue(int limit)` 





#### `public inline virtual void cancel()` 





#### `protected `[`Thread`](#classscy_1_1Thread)` _thread` 





#### `protected inline virtual  ~AsyncQueue()` 





# class `scy::Random` 




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

#### `public  Random(int stateSize)` 



Creates and initializes the PRNG. Specify either a state buffer size (8 to 256 bytes) or one of the Type values.

#### `public  ~Random()` 

Destroys the PRNG.



#### `public void seed(std::uint32_t seed)` 

Seeds the pseudo random generator with the given seed.



#### `public void seed()` 



Seeds the pseudo random generator with a random seed obtained from a RandomInputStream.

#### `public std::uint32_t next()` 

Returns the next 31-bit pseudo random number.



#### `public std::uint32_t next(std::uint32_t n)` 

Returns the next 31-bit pseudo random number modulo n.



#### `public char nextChar()` 

Returns the next pseudo random character.



#### `public bool nextBool()` 

Returns the next boolean pseudo random value.



#### `public float nextFloat()` 

Returns the next float pseudo random number between 0.0 and 1.0.



#### `public double nextDouble()` 

Returns the next double pseudo random number between 0.0 and 1.0.



#### `protected void initState(std::uint32_t seed,char * arg_state,std::int32_t n)` 





# class `scy::StopPropagation` 

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

#### `public inline virtual  ~StopPropagation()` 





# class `scy::SignalBase` 




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
`protected mutable `[`Mutex`](#classscy_1_1Mutex)` _mutex` | 

## Members

#### `public inline  SignalBase()` 





#### `public inline virtual  ~SignalBase()` 





#### `public inline void operator+=(const DelegateT & delegate)` 





#### `public inline void operator-=(const DelegateT & delegate)` 





#### `public inline void operator-=(const void * klass)` 





#### `public inline void attach(const DelegateT & delegate)` 



Attaches a delegate to the signal. If the delegate already exists it will overwrite the previous delegate.

#### `public inline bool detach(const DelegateT & delegate)` 



Detaches a delegate from the signal. Returns true if the delegate was detached, false otherwise.

#### `public inline void detach(const void * klass)` 

Detaches all delegates associated with the given class instance.



#### `public inline void cleanup()` 

Deletes cancelled delegates.



#### `public inline void obtain(DelegateList & active)` 



Retrieves a list of active delegates while simultaneously deleting any redundant delegates.

#### `public inline virtual void emit(void * sender)` 





#### `public inline virtual void emit(void * sender,P arg)` 





#### `public inline virtual void emit(void * sender,P arg,P2 arg2)` 





#### `public inline virtual void emit(void * sender,P arg,P2 arg2,P3 arg3)` 





#### `public inline virtual void emit(void * sender,P arg,P2 arg2,P3 arg3,P4 arg4)` 





#### `public inline void clear()` 





#### `public inline void enable(bool flag)` 





#### `public inline bool enabled() const` 





#### `public inline DelegateList delegates() const` 





#### `public inline int ndelegates() const` 



Returns the number of delegates connected to the signal. Use this instead of delegates().size() since the container is not updated in real time.

#### `protected DelegateList _delegates` 





#### `protected bool _enabled` 





#### `protected bool _dirty` 





#### `protected int _count` 





#### `protected mutable `[`Mutex`](#classscy_1_1Mutex)` _mutex` 





# class `scy::NullSignal` 

```
class scy::NullSignal
  : public scy::SignalBase< DelegateBase<> >
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------

## Members

# class `scy::Signal` 

```
class scy::Signal
  : public scy::SignalBase< DelegateBase< P >, P >
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------

## Members

# class `scy::Signal2` 

```
class scy::Signal2
  : public scy::SignalBase< DelegateBase< P, P2 >, P, P2 >
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------

## Members

# class `scy::Signal3` 

```
class scy::Signal3
  : public scy::SignalBase< DelegateBase< P, P2, P3 >, P, P2, P3 >
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------

## Members

# class `scy::Signal4` 

```
class scy::Signal4
  : public scy::SignalBase< DelegateBase< P, P2, P3, P4 >, P, P2, P3, P4 >
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------

## Members

# class `scy::Singleton` 




This is a helper template class for managing singleton objects allocated on the heap.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  Singleton()` | Creates the [Singleton](#classscy_1_1Singleton) wrapper.
`public inline  ~Singleton()` | 
`public inline S * get()` | 
`public inline S * swap(S * newPtr)` | 
`public inline void destroy()` | Destroys the managed singleton instance.

## Members

#### `public inline  Singleton()` 

Creates the [Singleton](#classscy_1_1Singleton) wrapper.



#### `public inline  ~Singleton()` 



Destroys the [Singleton](#classscy_1_1Singleton) wrapper and the managed singleton instance it holds.

#### `public inline S * get()` 



Returns a pointer to the singleton object hold by the [Singleton](#classscy_1_1Singleton). The first call to get on a nullptr singleton will instantiate the singleton.

#### `public inline S * swap(S * newPtr)` 



Swaps the old pointer with the new one and returns the old instance.

#### `public inline void destroy()` 

Destroys the managed singleton instance.



# class `scy::State` 






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

#### `public  State(ID id,const std::string & message)` 





#### `public inline virtual  ~State()` 





#### `public virtual ID id() const` 





#### `public virtual void set(ID id)` 





#### `public virtual std::string message() const` 





#### `public virtual void setMessage(const std::string & message)` 





#### `public virtual std::string str(ID id) const` 





#### `public virtual std::string toString() const` 





#### `public inline virtual bool between(ID lid,ID rid) const` 





#### `public inline virtual bool equals(ID id) const` 





#### `public inline bool operator==(const `[`State`](#classscy_1_1State)` & r)` 





#### `public inline bool operator==(const State::ID & r)` 





#### `protected ID _id` 





#### `protected std::string _message` 





# class `scy::MutexState` 

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
`protected mutable `[`Mutex`](#classscy_1_1Mutex)` _mutex` | 

## Members

#### `public  MutexState(ID id)` 





#### `public inline  MutexState(const `[`MutexState`](#classscy_1_1MutexState)` & r)` 





#### `public inline virtual  ~MutexState()` 





#### `public inline virtual ID id() const` 





#### `public inline virtual void set(ID id)` 





#### `public inline virtual std::string message() const` 





#### `public inline virtual void setMessage(const std::string & message)` 





#### `protected mutable `[`Mutex`](#classscy_1_1Mutex)` _mutex` 





# class `scy::StateSignal` 

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

#### `public  StateSignal(ID id)` 





#### `public  StateSignal(const `[`StateSignal`](#classscy_1_1StateSignal)` & r)` 





#### `public inline virtual  ~StateSignal()` 





#### `public virtual bool change(ID id)` 





#### `public virtual bool canChange(ID id)` 





#### `public virtual void onChange(ID id,ID prev)` 





#### `protected virtual void set(ID id)` 



Fired when the state changes to signal the new and previous states.

# class `scy::Stateful` 




This class implements a simple state machine. T should be a derived [State](#classscy_1_1State) type.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`Signal2`](#classscy_1_1Signal2)`< T &, const T & > StateChange` | Signals when the state changes.
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

#### `public `[`Signal2`](#classscy_1_1Signal2)`< T &, const T & > StateChange` 

Signals when the state changes.



#### `public inline  Stateful()` 





#### `public inline virtual  ~Stateful()` 





#### `public inline virtual bool stateEquals(unsigned int id) const` 





#### `public inline virtual bool stateBetween(unsigned int lid,unsigned int rid) const` 





#### `public inline virtual T & state()` 





#### `public inline virtual const T state() const` 





#### `protected T _state` 





#### `protected inline virtual bool beforeStateChange(const T & state)` 



Override to handle pre state change logic. Return false to prevent state change.

#### `protected inline virtual void onStateChange(T &,const T &)` 

Override to handle post state change logic.



#### `protected inline virtual bool setState(void * sender,unsigned int id,const std::string & message)` 



Sets the state and sends the state signal if the state change was successful.

#### `protected inline virtual bool setState(void * sender,const T & state)` 



Sets the state and sends the state signal if the state change was successful.

#### `protected inline virtual void setStateMessage(const std::string & message)` 





# class `scy::Stream` 

```
class scy::Stream
  : public scy::uv::Handle
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`Signal2`](#classscy_1_1Signal2)`< const char *, const int & > Read` | Signals when data can be read from the stream.
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

#### `public `[`Signal2`](#classscy_1_1Signal2)`< const char *, const int & > Read` 

Signals when data can be read from the stream.



#### `public inline  Stream(uv::Loop * loop,void * stream)` 





#### `public inline virtual void close()` 



Closes and resets the stream handle. This will close the active socket/pipe and destroy the uv_stream_t handle.

If the stream is already closed this call will have no side-effects.

#### `public inline bool shutdown()` 



Sends a shutdown packet to the connected peer. Returns true if the shutdown packet was sent.

#### `public inline bool write(const char * data,std::size_t len)` 



Writes data to the stream.

Returns false if the underlying socket is closed. This method does not throw an exception.

#### `public inline Buffer & buffer()` 





#### `public inline virtual bool closed() const` 

Returns true if the native socket handle is closed.



#### `protected Buffer _buffer` 





#### `protected inline virtual bool readStart()` 





#### `protected inline virtual bool readStop()` 





#### `protected inline virtual void onRead(const char * data,std::size_t len)` 





#### `protected inline virtual  ~Stream()` 





#### `protected inline virtual void * self()` 





# class `scy::SyncContext` 

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
`protected `[`uv::Handle`](#classscy_1_1uv_1_1Handle)` _handle` | 
`protected virtual void startAsync()` | Start the context from the control thread.
`protected virtual bool async() const` | 

## Members

#### `public  SyncContext(uv::Loop * loop)` 



Create the synchronization context the given event loop and method. The target method will be called from the event loop context.

#### `public  SyncContext(uv::Loop * loop,std::function< void()> target)` 





#### `public  SyncContext(uv::Loop * loop,std::function< void(void *)> target,void * arg)` 





#### `public virtual  ~SyncContext()` 





#### `public void post()` 



Send a synchronization request to the event loop. Call this each time you want the target method called synchronously. The synchronous method will be called on next iteration. This is not atomic, so do not expect a callback for every request.

#### `public virtual void cancel()` 





#### `public virtual void close()` 





#### `public virtual bool closed()` 





#### `public `[`uv::Handle`](#classscy_1_1uv_1_1Handle)` & handle()` 





#### `protected `[`uv::Handle`](#classscy_1_1uv_1_1Handle)` _handle` 





#### `protected virtual void startAsync()` 

Start the context from the control thread.



#### `protected virtual bool async() const` 



Returns true if the implementation is thread-based, or false if it belongs to an event loop.

# class `scy::Task` 

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

#### `public  Task(bool repeat)` 





#### `public virtual void destroy()` 

Sets the task to destroyed state.



#### `public virtual bool destroyed() const` 

Signals that the task should be disposed of.



#### `public virtual bool repeating() const` 



Signals that the task's should be called repeatedly by the [TaskRunner](#classscy_1_1TaskRunner). If this returns false the task will be [cancelled()](#group__base_1ga7efc7639f668b72480625cc0f34f1eaa)

#### `public virtual std::uint32_t id() const` 

Unique task ID.



#### `protected std::uint32_t _id` 





#### `protected bool _repeating` 





#### `protected bool _destroyed` 





#### `protected  Task(const `[`Task`](#classscy_1_1Task)` & task)` 





#### `protected `[`Task`](#classscy_1_1Task)` & operator=(`[`Task`](#classscy_1_1Task)` const &)` 





#### `protected virtual  ~Task()` 

Should remain protected.



#### `protected void run()` 



Called by the [TaskRunner](#classscy_1_1TaskRunner) to run the task. Override this method to implement task action. Returning true means the true should be called again, and false will cause the task to be destroyed. The task will similarly be destroyed id [destroy()](#group__base_1gaecd8a85018f8f75ae802dcbf3749bdef) was called during the current task iteration.

# class `scy::TaskRunner` 

```
class scy::TaskRunner
  : public scy::async::Runnable
```  



The [TaskRunner](#classscy_1_1TaskRunner) is an asynchronous event loop in charge of running one or many tasks.

The [TaskRunner](#classscy_1_1TaskRunner) continually loops through each task in the task list calling the task's [run()](#group__base_1ga07a481d667f1a9c67e881eedcffc10a5) method.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`NullSignal`](#classscy_1_1NullSignal)` Idle` | Fires after completing an iteration of all tasks.
`public `[`NullSignal`](#classscy_1_1NullSignal)` Shutdown` | Signals when the [TaskRunner](#classscy_1_1TaskRunner) is shutting down.
`public  TaskRunner(async::Runner::Ptr runner)` | 
`public virtual  ~TaskRunner()` | 
`public virtual bool start(`[`Task`](#classscy_1_1Task)` * task)` | Starts a task, adding it if it doesn't exist.
`public virtual bool cancel(`[`Task`](#classscy_1_1Task)` * task)` | 
`public virtual bool destroy(`[`Task`](#classscy_1_1Task)` * task)` | Queues a task for destruction.
`public virtual bool exists(`[`Task`](#classscy_1_1Task)` * task) const` | Returns weather or not a task exists.
`public virtual `[`Task`](#classscy_1_1Task)` * get(std::uint32_t id) const` | 
`public virtual void setRunner(async::Runner::Ptr runner)` | 
`public inline virtual const char * className() const` | 
`protected mutable `[`Mutex`](#classscy_1_1Mutex)` _mutex` | 
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

#### `public `[`NullSignal`](#classscy_1_1NullSignal)` Idle` 

Fires after completing an iteration of all tasks.



#### `public `[`NullSignal`](#classscy_1_1NullSignal)` Shutdown` 

Signals when the [TaskRunner](#classscy_1_1TaskRunner) is shutting down.



#### `public  TaskRunner(async::Runner::Ptr runner)` 





#### `public virtual  ~TaskRunner()` 





#### `public virtual bool start(`[`Task`](#classscy_1_1Task)` * task)` 

Starts a task, adding it if it doesn't exist.



#### `public virtual bool cancel(`[`Task`](#classscy_1_1Task)` * task)` 



Cancels a task. The task reference will be managed the [TaskRunner](#classscy_1_1TaskRunner) until the task is destroyed.

#### `public virtual bool destroy(`[`Task`](#classscy_1_1Task)` * task)` 

Queues a task for destruction.



#### `public virtual bool exists(`[`Task`](#classscy_1_1Task)` * task) const` 

Returns weather or not a task exists.



#### `public virtual `[`Task`](#classscy_1_1Task)` * get(std::uint32_t id) const` 



Returns the task pointer matching the given ID, or nullptr if no task exists.

#### `public virtual void setRunner(async::Runner::Ptr runner)` 



Set the asynchronous context for packet processing. This may be a [Thread](#classscy_1_1Thread) or another derivative of Async. Must be set before the stream is activated.

#### `public inline virtual const char * className() const` 





#### `protected mutable `[`Mutex`](#classscy_1_1Mutex)` _mutex` 





#### `protected TaskList _tasks` 





#### `protected async::Runner::Ptr _runner` 





#### `protected virtual void run()` 

Called by the async context to run the next task.



#### `protected virtual bool add(`[`Task`](#classscy_1_1Task)` * task)` 

Adds a task to the runner.



#### `protected virtual bool remove(`[`Task`](#classscy_1_1Task)` * task)` 

Removes a task from the runner.



#### `protected virtual `[`Task`](#classscy_1_1Task)` * next() const` 

Returns the next task to be run.



#### `protected virtual void clear()` 

Destroys and clears all manages tasks.



#### `protected virtual void onAdd(`[`Task`](#classscy_1_1Task)` * task)` 

Called after a task is added.



#### `protected virtual void onStart(`[`Task`](#classscy_1_1Task)` * task)` 

Called after a task is started.



#### `protected virtual void onCancel(`[`Task`](#classscy_1_1Task)` * task)` 

Called after a task is cancelled.



#### `protected virtual void onRemove(`[`Task`](#classscy_1_1Task)` * task)` 

Called after a task is removed.



#### `protected virtual void onRun(`[`Task`](#classscy_1_1Task)` * task)` 

Called after a task has run.



# class `scy::test::Test` 




This class is for implementing any kind async test that is compatible with a [TestRunner](#classscy_1_1test_1_1TestRunner).

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::string name` | The name of the test.
`public error_list_t errors` | A list of test errors.
`public double duration` | The test run duration for benchmarking.
`public  Test(const std::string & name)` | 
`public virtual  ~Test()` | Should remain protected.
`public void run()` | Called by the [TestRunner](#classscy_1_1test_1_1TestRunner) to run the test.
`public bool passed()` | Return true when the test passed without errors.
`protected  Test(const `[`Test`](#classscy_1_1test_1_1Test)` & test)` | 
`protected `[`Test`](#classscy_1_1test_1_1Test)` & operator=(`[`Test`](#classscy_1_1test_1_1Test)` const &)` | 

## Members

#### `public std::string name` 

The name of the test.



#### `public error_list_t errors` 

A list of test errors.



#### `public double duration` 

The test run duration for benchmarking.



#### `public  Test(const std::string & name)` 





#### `public virtual  ~Test()` 

Should remain protected.



#### `public void run()` 

Called by the [TestRunner](#classscy_1_1test_1_1TestRunner) to run the test.



#### `public bool passed()` 

Return true when the test passed without errors.



#### `protected  Test(const `[`Test`](#classscy_1_1test_1_1Test)` & test)` 





#### `protected `[`Test`](#classscy_1_1test_1_1Test)` & operator=(`[`Test`](#classscy_1_1test_1_1Test)` const &)` 





# class `scy::test::FunctionTest` 

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
`protected inline virtual void run()` | Called by the [TestRunner](#classscy_1_1test_1_1TestRunner) to run the test.

## Members

#### `public voidfunc_t target` 





#### `public inline  FunctionTest(voidfunc_t target,const std::string & name)` 





#### `protected inline virtual  ~FunctionTest()` 





#### `protected inline virtual void run()` 

Called by the [TestRunner](#classscy_1_1test_1_1TestRunner) to run the test.



# class `scy::test::TestRunner` 




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
`public `[`Test`](#classscy_1_1test_1_1Test)` * current() const` | Return the currently active [Test](#classscy_1_1test_1_1Test) or nullptr.
`public test_list_t tests() const` | Return the list of tests.
`public error_map_t errors() const` | Return a map of tests and errors.
`public bool passed() const` | Return true if all tests passed.
`protected mutable `[`Mutex`](#classscy_1_1Mutex)` _mutex` | 
`protected std::list< `[`Test`](#classscy_1_1test_1_1Test)` * > _tests` | 
`protected `[`Test`](#classscy_1_1test_1_1Test)` * _current` | 

## Members

#### `public  TestRunner()` 





#### `public virtual  ~TestRunner()` 





#### `public void add(`[`Test`](#classscy_1_1test_1_1Test)` * test)` 

Add a test to the runner.



#### `public `[`Test`](#classscy_1_1test_1_1Test)` * get(const std::string & name) const` 



Return a pointer to the test matching the given name, or nullptr if no matching test exists.

#### `public void run()` 

Called by the async context to run the next test.



#### `public void clear()` 

Destroy and clears all managed tests.



#### `public `[`Test`](#classscy_1_1test_1_1Test)` * current() const` 

Return the currently active [Test](#classscy_1_1test_1_1Test) or nullptr.



#### `public test_list_t tests() const` 

Return the list of tests.



#### `public error_map_t errors() const` 

Return a map of tests and errors.



#### `public bool passed() const` 

Return true if all tests passed.



#### `protected mutable `[`Mutex`](#classscy_1_1Mutex)` _mutex` 





#### `protected std::list< `[`Test`](#classscy_1_1test_1_1Test)` * > _tests` 





#### `protected `[`Test`](#classscy_1_1test_1_1Test)` * _current` 





# class `scy::Thread` 

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

#### `public  Thread()` 





#### `public  Thread(`[`async::Runnable`](#classscy_1_1async_1_1Runnable)` & target)` 





#### `public  Thread(std::function< void()> target)` 





#### `public  Thread(std::function< void(void *)> target,void * arg)` 





#### `public virtual  ~Thread()` 





#### `public void join()` 

Waits until the thread exits.



#### `public bool waitForExit(int timeout)` 



Waits until the thread exits. The thread should be cancelled beore calling this method. This method must be called from outside the current thread context or deadlock will ensue.

#### `public uv_thread_t id() const` 

Returns the native thread handle.



#### `protected uv_thread_t _handle` 





#### `protected  Thread(const `[`Thread`](#classscy_1_1Thread)` &)` 





#### `protected `[`Thread`](#classscy_1_1Thread)` & operator=(const `[`Thread`](#classscy_1_1Thread)` &)` 





#### `protected virtual bool async() const` 



Returns true if the implementation is thread-based, or false if it belongs to an event loop.

#### `protected virtual void startAsync()` 

Start the context from the control thread.



# class `scy::AsyncStartable` 

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
`protected `[`Thread`](#classscy_1_1Thread)` _thread` | 

## Members

#### `public inline  AsyncStartable()` 





#### `public inline virtual  ~AsyncStartable()` 





#### `public inline virtual bool start()` 





#### `public inline virtual void stop()` 





#### `protected `[`Thread`](#classscy_1_1Thread)` _thread` 





# class `scy::Timer` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`NullSignal`](#classscy_1_1NullSignal)` Timeout` | 
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
`protected `[`uv::Handle`](#classscy_1_1uv_1_1Handle)` _handle` | 
`protected std::int64_t _timeout` | 
`protected std::int64_t _interval` | 
`protected std::int64_t _count` | 
`protected  Timer(const `[`Timer`](#classscy_1_1Timer)` &)` | 
`protected `[`Timer`](#classscy_1_1Timer)` & operator=(const `[`Timer`](#classscy_1_1Timer)` &)` | 
`protected virtual void init()` | 

## Members

#### `public `[`NullSignal`](#classscy_1_1NullSignal)` Timeout` 





#### `public  Timer(uv::Loop * loop)` 





#### `public virtual  ~Timer()` 





#### `public virtual void start(std::int64_t interval)` 





#### `public virtual void start(std::int64_t timeout,std::int64_t interval)` 



Starts the timer, an interval value of zero will only trigger once after timeout.

#### `public virtual void stop()` 

Stops the timer.



#### `public virtual void restart()` 



Restarts the timer, even if it hasn't been started yet. An interval or interval must be set or an exception will be thrown.

#### `public virtual void again()` 



Stop the timer, and if it is repeating restart it using the repeat value as the timeout. If the timer has never been started before it returns -1 and sets the error to UV_EINVAL.

#### `public virtual void setInterval(std::int64_t interval)` 



Set the repeat value. Note that if the repeat value is set from a timer callback it does not immediately take effect. If the timer was non-repeating before, it will have been stopped. If it was repeating, then the old repeat value will have been used to schedule the next timeout.

#### `public bool active() const` 





#### `public std::int64_t timeout() const` 





#### `public std::int64_t interval() const` 





#### `public std::int64_t count()` 





#### `public `[`uv::Handle`](#classscy_1_1uv_1_1Handle)` & handle()` 





#### `protected `[`uv::Handle`](#classscy_1_1uv_1_1Handle)` _handle` 





#### `protected std::int64_t _timeout` 





#### `protected std::int64_t _interval` 





#### `protected std::int64_t _count` 





#### `protected  Timer(const `[`Timer`](#classscy_1_1Timer)` &)` 





#### `protected `[`Timer`](#classscy_1_1Timer)` & operator=(const `[`Timer`](#classscy_1_1Timer)` &)` 





#### `protected virtual void init()` 





# class `scy::TZInfo` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  TZInfo()` | 
`public inline int timeZone()` | 
`public inline const char * name(bool dst)` | 

## Members

#### `public inline  TZInfo()` 





#### `public inline int timeZone()` 





#### `public inline const char * name(bool dst)` 





# struct `scy::OptionParser` 






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

#### `public std::string exepath` 





#### `public OptionMap args` 





#### `public  OptionParser(int argc,char * argv,const char * delim)` 





#### `public inline bool has(const char * key)` 





#### `public inline std::string get(const char * key)` 





#### `public template<typename NumericType>`  <br/>`inline NumericType get(const char * key)` 





# struct `scy::async::Runner::Context` 




The context which we send to the thread context. This allows us to garecefully handle late callbacks and avoid the need for deferred destruction of [Runner](#classscy_1_1async_1_1Runner) objects.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public uv_thread_t tid` | 
`public bool started` | 
`public bool running` | 
`public bool repeating` | 
`public std::atomic< bool > exit` | 
`public std::function< void()> target` | 
`public std::function< void(void *)> target1` | 
`public void * arg` | 
`public void * handle` | private implementation data
`public void cancel()` | 
`public bool cancelled() const` | 
`public inline void reset()` | 
`public inline  Context()` | 

## Members

#### `public uv_thread_t tid` 



Thread-safe POD members May be accessed at any time

#### `public bool started` 





#### `public bool running` 





#### `public bool repeating` 





#### `public std::atomic< bool > exit` 





#### `public std::function< void()> target` 



Non thread-safe members Should not be accessed once the [Runner](#classscy_1_1async_1_1Runner) is started

#### `public std::function< void(void *)> target1` 





#### `public void * arg` 





#### `public void * handle` 

private implementation data



#### `public void cancel()` 





#### `public bool cancelled() const` 





#### `public inline void reset()` 





#### `public inline  Context()` 





# struct `scy::base64::Encoder` 

```
struct scy::base64::Encoder
  : public scy::basic::Encoder
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public int _buffersize` | 
`public  _buffersize(buffersize)` | 

## Members

#### `public int _buffersize` 





#### `public  _buffersize(buffersize)` 





# struct `scy::base64::Decoder` 

```
struct scy::base64::Decoder
  : public scy::basic::Decoder
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public int _buffersize` | 
`public  _buffersize(buffersize)` | 

## Members

#### `public int _buffersize` 





#### `public  _buffersize(buffersize)` 





# struct `scy::Bitwise` 






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

#### `public unsigned data` 





#### `public inline  Bitwise(unsigned flags)` 





#### `public inline virtual void reset()` 





#### `public inline virtual void set(unsigned flag)` 





#### `public inline virtual void add(unsigned flag)` 





#### `public inline virtual void remove(unsigned flag)` 





#### `public inline virtual void toggle(unsigned flag)` 





#### `public inline virtual bool has(unsigned flag) const` 





# struct `scy::NVCollection::ILT` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline bool operator()(const std::string & s1,const std::string & s2) const` | 

## Members

#### `public inline bool operator()(const std::string & s1,const std::string & s2) const` 





# struct `scy::DelegateCallback` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------

## Members

# struct `scy::DelegateCallback< C, 0, true >` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual void emit(void * sender,void *,void *,void *,void *) const` | 

## Members

#### `public inline virtual void emit(void * sender,void *,void *,void *,void *) const` 





# struct `scy::DelegateCallback< C, 0, false >` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual void emit(void * sender,void *,void *,void *,void *) const` | 

## Members

#### `public inline virtual void emit(void * sender,void *,void *,void *,void *) const` 





# struct `scy::DelegateCallback< C, 1, true, P >` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual void emit(void * sender,P arg,void *,void *,void *) const` | 

## Members

#### `public inline virtual void emit(void * sender,P arg,void *,void *,void *) const` 





# struct `scy::DelegateCallback< C, 1, false, P >` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual void emit(void *,P arg,void *,void *,void *) const` | 

## Members

#### `public inline virtual void emit(void *,P arg,void *,void *,void *) const` 





# struct `scy::DelegateCallback< C, 2, true, P, P2 >` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual void emit(void * sender,P arg,P2 arg2,void *,void *) const` | 

## Members

#### `public inline virtual void emit(void * sender,P arg,P2 arg2,void *,void *) const` 





# struct `scy::DelegateCallback< C, 2, false, P, P2 >` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual void emit(void *,P arg,P2 arg2,void *,void *) const` | 

## Members

#### `public inline virtual void emit(void *,P arg,P2 arg2,void *,void *) const` 





# struct `scy::DelegateCallback< C, 3, true, P, P2, P3 >` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual void emit(void * sender,P arg,P2 arg2,P3 arg3,void *) const` | 

## Members

#### `public inline virtual void emit(void * sender,P arg,P2 arg2,P3 arg3,void *) const` 





# struct `scy::DelegateCallback< C, 3, false, P, P2, P3 >` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual void emit(void *,P arg,P2 arg2,P3 arg3,void *) const` | 

## Members

#### `public inline virtual void emit(void *,P arg,P2 arg2,P3 arg3,void *) const` 





# struct `scy::DelegateCallback< C, 4, true, P, P2, P3, P4 >` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual void emit(void * sender,P arg,P2 arg2,P3 arg3,P4 arg4) const` | 

## Members

#### `public inline virtual void emit(void * sender,P arg,P2 arg2,P3 arg3,P4 arg4) const` 





# struct `scy::DelegateCallback< C, 4, false, P, P2, P3, P4 >` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual void emit(void *,P arg,P2 arg2,P3 arg3,P4 arg4) const` | 

## Members

#### `public inline virtual void emit(void *,P arg,P2 arg2,P3 arg3,P4 arg4) const` 





# struct `scy::DelegateBase` 




The abstract base for all instantiations of the [Delegate](#classscy_1_1Delegate) template classes.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public void * data` | 
`public inline  DelegateBase(DataT data)` | 
`public inline  DelegateBase(const `[`DelegateBase`](#structscy_1_1DelegateBase)` & r)` | 
`public inline virtual  ~DelegateBase()` | 

## Members

#### `public void * data` 





#### `public inline  DelegateBase(DataT data)` 





#### `public inline  DelegateBase(const `[`DelegateBase`](#structscy_1_1DelegateBase)` & r)` 





#### `public inline virtual  ~DelegateBase()` 





# struct `scy::Error` 






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

#### `public int errorno` 





#### `public std::string message` 





#### `public std::exception_ptr exception` 





#### `public inline  Error()` 





#### `public inline  Error(const std::string & msg)` 





#### `public inline  Error(const char * msg)` 





#### `public inline bool any() const` 





#### `public inline void reset()` 





#### `public inline void rethrow()` 





# struct `scy::hex::Encoder` 

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

#### `public int _linePos` 





#### `public int _lineLength` 





#### `public int _uppercase` 





#### `public inline  Encoder()` 





#### `public inline virtual std::size_t encode(const char * inbuf,std::size_t nread,char * outbuf)` 





#### `public inline virtual std::size_t finalize(char *)` 





#### `public inline void setUppercase(bool flag)` 





#### `public inline void setLineLength(int lineLength)` 





# struct `scy::hex::Decoder` 

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

#### `public char lastbyte` 





#### `public inline  Decoder()` 





#### `public inline virtual  ~Decoder()` 





#### `public inline virtual std::size_t decode(const char * inbuf,std::size_t nread,char * outbuf)` 





#### `public inline virtual std::size_t finalize(char *)` 





#### `public inline bool readnext(const char * inbuf,std::size_t nread,std::size_t & rpos,char & c)` 





#### `public inline int nybble(const int n)` 





#### `public inline bool iswspace(const char c)` 





# struct `scy::ipc::Action` 


Default action type for executing synchronized callbacks.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public callback_t target` | 
`public void * arg` | 
`public std::string data` | 
`public inline  Action(callback_t target,void * arg,const std::string & data)` | 

## Members

#### `public callback_t target` 





#### `public void * arg` 





#### `public std::string data` 





#### `public inline  Action(callback_t target,void * arg,const std::string & data)` 





# struct `scy::LogStream` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public LogLevel level` | 
`public int line` | 
`public std::string realm` | 
`public std::string address` | 
`public std::ostringstream message` | 
`public std::time_t ts` | 
`public `[`LogChannel`](#classscy_1_1LogChannel)` * channel` | 
`public  LogStream(LogLevel level,const std::string & realm,int line,const void * ptr,const char * channel)` | 
`public  LogStream(LogLevel level,const std::string & realm,const std::string & address)` | 
`public  LogStream(const `[`LogStream`](#structscy_1_1LogStream)` & that)` | 
`public  ~LogStream()` | 
`public inline `[`LogStream`](#structscy_1_1LogStream)` & operator<<(const LogLevel data)` | 
`public inline `[`LogStream`](#structscy_1_1LogStream)` & operator<<(`[`LogChannel`](#classscy_1_1LogChannel)` * data)` | 
`public template<typename T>`  <br/>`inline `[`LogStream`](#structscy_1_1LogStream)` & operator<<(const T & data)` | 
`public inline `[`LogStream`](#structscy_1_1LogStream)` & operator<<(std::ostream &(*)(std::ostream &) f)` | 

## Members

#### `public LogLevel level` 





#### `public int line` 





#### `public std::string realm` 





#### `public std::string address` 





#### `public std::ostringstream message` 





#### `public std::time_t ts` 





#### `public `[`LogChannel`](#classscy_1_1LogChannel)` * channel` 





#### `public  LogStream(LogLevel level,const std::string & realm,int line,const void * ptr,const char * channel)` 





#### `public  LogStream(LogLevel level,const std::string & realm,const std::string & address)` 





#### `public  LogStream(const `[`LogStream`](#structscy_1_1LogStream)` & that)` 





#### `public  ~LogStream()` 





#### `public inline `[`LogStream`](#structscy_1_1LogStream)` & operator<<(const LogLevel data)` 





#### `public inline `[`LogStream`](#structscy_1_1LogStream)` & operator<<(`[`LogChannel`](#classscy_1_1LogChannel)` * data)` 





#### `public template<typename T>`  <br/>`inline `[`LogStream`](#structscy_1_1LogStream)` & operator<<(const T & data)` 





#### `public inline `[`LogStream`](#structscy_1_1LogStream)` & operator<<(std::ostream &(*)(std::ostream &) f)` 



Handle std::endl flags. This method flushes the log message and queues it for write. WARNING: After using std::endl to flush the message pointer should not be accessed.

# struct `scy::deleter::Deferred` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline void operator()(T * ptr)` | 

## Members

#### `public inline void operator()(T * ptr)` 





# struct `scy::deleter::Dispose` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline void operator()(T * ptr)` | 

## Members

#### `public inline void operator()(T * ptr)` 





# struct `scy::deleter::Array` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline void operator()(T * ptr)` | 

## Members

#### `public inline void operator()(T * ptr)` 





# struct `scy::IPacketInfo` 




An abstract interface for packet sources to provide extra information about packets.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  IPacketInfo()` | 
`public inline virtual  ~IPacketInfo()` | 
`public `[`IPacketInfo`](#structscy_1_1IPacketInfo)` * clone() const` | 

## Members

#### `public inline  IPacketInfo()` 





#### `public inline virtual  ~IPacketInfo()` 





#### `public `[`IPacketInfo`](#structscy_1_1IPacketInfo)` * clone() const` 





# struct `scy::PacketCreationStrategy` 

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

#### `public inline  PacketCreationStrategy(int priority)` 





#### `public inline virtual  ~PacketCreationStrategy()` 





#### `public inline virtual `[`IPacket`](#classscy_1_1IPacket)` * create(const `[`ConstBuffer`](#classscy_1_1ConstBuffer)` & buffer,std::size_t & nread) const` 





#### `public inline virtual int priority() const` 





#### `protected int _priority` 





# struct `scy::PacketAdapterReference` 


For 0.8.x compatibility.

Provides a reference to a PacketSignal instance.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` * ptr` | 
`public `[`ScopedPointer`](#classscy_1_1ScopedPointer)` * deleter` | 
`public int order` | 
`public bool syncState` | 
`public inline  PacketAdapterReference(`[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` * ptr,`[`ScopedPointer`](#classscy_1_1ScopedPointer)` * deleter,int order,bool syncState)` | 
`public inline  ~PacketAdapterReference()` | 

## Members

#### `public `[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` * ptr` 





#### `public `[`ScopedPointer`](#classscy_1_1ScopedPointer)` * deleter` 





#### `public int order` 





#### `public bool syncState` 





#### `public inline  PacketAdapterReference(`[`PacketStreamAdapter`](#classscy_1_1PacketStreamAdapter)` * ptr,`[`ScopedPointer`](#classscy_1_1ScopedPointer)` * deleter,int order,bool syncState)` 





#### `public inline  ~PacketAdapterReference()` 





# struct `scy::PacketStreamState` 

```
struct scy::PacketStreamState
  : public scy::State
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual std::string str(unsigned int id) const` | 

## Members

#### `public inline virtual std::string str(unsigned int id) const` 





# struct `scy::TransactionState` 

```
struct scy::TransactionState
  : public scy::State
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual std::string str(unsigned int id) const` | 

## Members

#### `public inline virtual std::string str(unsigned int id) const` 





# struct `scy::DelegateCallback< C, 8, false, PolymorphicT >` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual DefineCallbackFields void emit(void *,`[`basic::Polymorphic`](#classscy_1_1basic_1_1Polymorphic)` & data,void *,void *,void *) const` | 

## Members

#### `public inline virtual DefineCallbackFields void emit(void *,`[`basic::Polymorphic`](#classscy_1_1basic_1_1Polymorphic)` & data,void *,void *,void *) const` 





# struct `scy::DelegateCallback< C, 8, true, PolymorphicT >` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual DefineCallbackFields void emit(void * sender,`[`basic::Polymorphic`](#classscy_1_1basic_1_1Polymorphic)` & data,void *,void *,void *) const` | 

## Members

#### `public inline virtual DefineCallbackFields void emit(void * sender,`[`basic::Polymorphic`](#classscy_1_1basic_1_1Polymorphic)` & data,void *,void *,void *) const` 





# struct `scy::SharedLibrary` 






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

#### `public inline bool open(const std::string & path)` 



Opens a shared library. The filename is in utf-8. Returns true on success and false on error. Call `SharedLibrary::error()` to get the error message.

#### `public inline void close()` 





#### `public inline bool sym(const char * name,void ** ptr)` 



Retrieves a data pointer from a dynamic library. It is legal for a symbol to map to nullptr. Returns 0 on success and -1 if the symbol was not found.

#### `public inline void setError(const std::string & prefix)` 





#### `public inline std::string error() const` 





#### `protected uv_lib_t _lib` 





#### `protected std::string _error` 





# struct `scy::util::Version` 






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

#### `public int major` 





#### `public int minor` 





#### `public int revision` 





#### `public int build` 





#### `public inline  Version(const std::string & version)` 





#### `public inline bool operator<(const `[`Version`](#structscy_1_1util_1_1Version)` & other)` 





#### `public inline bool operator==(const `[`Version`](#structscy_1_1util_1_1Version)` & other)` 





