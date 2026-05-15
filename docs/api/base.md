{#basemodule}

# base

The `[Base module](#basemodule)` module contains reusable cross platform tools and utilities.

### Namespaces

| Name | Description |
|------|-------------|
| [`hex`](#hex) | Hexadecimal encoding and decoding helpers. |
| [`ipc`](#ipc) | Classes for inter-process communication. |
| [`test`](#test) | Modern unit testing framework. |
| [`time`](#time-3) | Classes and functions for handling time. |
| [`base64`](#base64) | Base64 encoding and decoding helpers. |
| [`deleter`](#deleter-2) | Deleter helpers for objects managed through custom destruction routines. |
| [`numeric`](#numeric) | Integer parsing and formatting helpers. |
| [`basic`](#basic) | Interface classes. |
| [`fs`](#fs) | Cross-platform filesystem path and file helpers. |
| [`util`](#util) | Miscellaneous string, parsing, and version utilities. |

### Classes

| Name | Description |
|------|-------------|
| [`Pipe`](#pipe) | Named pipe / stdio stream built on `uv_pipe_t`. |
| [`Idler`](#idler) | Asynchronous type that triggers callbacks when the event loop is idle. |
| [`Queue`](#queue) | Thread-safe queue container. |
| [`RunnableQueue`](#runnablequeue) | [Queue](#queue) of runnable tasks for sequential execution. |
| [`SyncQueue`](#syncqueue) | [SyncQueue](#syncqueue) extends [Synchronizer](#synchronizer) to implement a synchronized FIFO queue which receives T objects from any thread and synchronizes them for safe consumption by the associated event loop. |
| [`AsyncQueue`](#asyncqueue) | [AsyncQueue](#asyncqueue) is a thread-based queue which receives packets from any thread source and dispatches them asynchronously. |
| [`Timer`](#timer) | Asynchronous event based timer. |
| [`MutableBuffer`](#mutablebuffer) | The [MutableBuffer](#mutablebuffer) class provides a safe representation of a buffer that can be modified. It does not own the underlying data, and so is cheap to copy or assign. |
| [`ConstBuffer`](#constbuffer) | The [ConstBuffer](#constbuffer) class provides a safe representation of a buffer that cannot be modified. It does not own the underlying data, and so is cheap to copy or assign. |
| [`BitReader`](#bitreader) | Class for reading binary streams. |
| [`BitWriter`](#bitwriter) | Class for reading/writing binary streams. |
| [`DynamicBitWriter`](#dynamicbitwriter) | Class for reading/writing dynamically resizable binary streams. |
| [`LogWriter`](#logwriter) | Log output stream writer. |
| [`AsyncLogWriter`](#asynclogwriter) | [Thread](#thread) based log output stream writer. |
| [`Logger`](#logger) | [Logger](#logger) class. |
| [`LogChannel`](#logchannel) | Named log output channel with configurable severity level and formatting. |
| [`ConsoleChannel`](#consolechannel) | Log channel that writes formatted messages to standard output. |
| [`FileChannel`](#filechannel) | Log channel that writes formatted messages to a file. |
| [`RotatingFileChannel`](#rotatingfilechannel) | Log channel that writes to time-rotated log files. |
| [`RefCounted`](#refcounted) | Base class for intrusive reference counting. |
| [`IntrusivePtr`](#intrusiveptr) | Intrusive smart pointer for [RefCounted](#refcounted) objects. |
| [`Random`](#random) | [Random](#random) implements a pseudo random number generator (PRNG) using the Mersenne Twister algorithm (std::mt19937). |
| [`Runner`](#runner) | [Runner](#runner) is a virtual interface for implementing asynchronous objects such as threads and futures. |
| [`Signal< RT(Args...), MutexT >`](#signalrtargsmutext) | Thread-safe signal and slot implementation for callback-based event dispatch. |
| [`Stream`](#stream) | Basic stream type for sockets and pipes. |
| [`Thread`](#thread) | Platform-independent wrapper around an operating system thread. |
| [`Process`](#process) | Spawns and manages a child process with stdin/stdout/stderr pipes. |
| [`Timeout`](#timeout) | [Timeout](#timeout) counter which expires after a given delay. Delay is specified in milliseconds. |
| [`TimedToken`](#timedtoken) | Token that expires after the specified duration. |
| [`Timestamp`](#timestamp) | A [Timestamp](#timestamp) stores a monotonic* time value with (theoretical) microseconds resolution. Timestamps can be compared with each other and simple arithmetics are supported. |
| [`Timespan`](#timespan) | A class that represents time spans up to microsecond resolution. |
| [`DateTime`](#datetime) | This class represents an instant in time, expressed in years, months, days, hours, minutes, seconds and milliseconds based on the Gregorian calendar. The class is mainly useful for conversions between UTC, Julian day and Gregorian calendar dates. |
| [`Timezone`](#timezone) | This class provides information about the current timezone. |
| [`LocalDateTime`](#localdatetime) | This class represents an instant in local time (as opposed to UTC), expressed in years, months, days, hours, minutes, seconds and milliseconds based on the Gregorian calendar. |
| [`DateTimeFormat`](#datetimeformat) | Definition of date/time formats and various constants used by [DateTimeFormatter](#datetimeformatter) and [DateTimeParser](#datetimeparser). |
| [`DateTimeFormatter`](#datetimeformatter) | This class converts dates and times into strings, supporting a variety of standard and custom formats. |
| [`DateTimeParser`](#datetimeparser) | This class provides a method for parsing dates and times from strings. All parsing methods do their best to parse a meaningful result, even from malformed input strings. |
| [`Stopwatch`](#stopwatch) | A simple facility to measure time intervals with microsecond resolution. |
| [`ThreadedStreamReader`](#threadedstreamreader) | Threaded stream reader class. |
| [`StreamWriter`](#streamwriter) | Packet stream writer class. |
| [`State`](#state) | [State](#state) class for state machines. |
| [`Stateful`](#stateful) | [State](#state) machine implementation. |
| [`IRegistry`](#iregistry) | Abstract interface for object registries. |
| [`Singleton`](#singleton) | Helper template class for managing singleton objects allocated on the heap. |
| [`KeyedStore`](#keyedstore) | A keyed store of unique_ptr values with optional lifecycle hooks. Not thread-safe; designed for single-threaded event loop contexts. |
| [`KVCollection`](#kvcollection) | A keyed value store (values stored by copy, not pointer). |
| [`NVCollection`](#nvcollection) | A storage container for a name value collections. This collection can store multiple entries for each name, and it's getters are case-insensitive. |
| [`Application`](#application) | Main icey application class. |
| [`SyncPacketQueue`](#syncpacketqueue) | Synchronized packet queue for event loop integration. |
| [`AsyncPacketQueue`](#asyncpacketqueue) | Thread-based asynchronous packet dispatch queue. |
| [`RateLimiter`](#ratelimiter) | Token bucket rate limiter for throttling message send frequency. |
| [`PacketStreamAdapter`](#packetstreamadapter) | This class is a wrapper for integrating external classes with the a [PacketStream](#packetstream)'s data flow and state machine. |
| [`PacketProcessor`](#packetprocessor) | This class is a virtual interface for creating PacketStreamAdapters which process that and emit the [IPacket](#ipacket) type. |
| [`PacketStream`](#packetstream) | Processes and broadcasts IPackets through a configurable adapter graph. |
| [`Synchronizer`](#synchronizer) | [Synchronizer](#synchronizer) enables any thread to communicate with the associated event loop via synchronized callbacks. |
| [`TimedManager`](#timedmanager) | Timed pointer manager |
| [`IPacketCreationStrategy`](#ipacketcreationstrategy) | Abstract strategy for creating typed packets from raw buffer data. |
| [`PacketFactory`](#packetfactory) | Priority-ordered factory that creates typed packets from raw buffers using registered strategies. |
| [`StreamManager`](#streammanager) | Manages a named collection of [PacketStream](#packetstream) instances with lifecycle callbacks. |
| [`IDiagnostic`](#idiagnostic) | Abstract interface for diagnostic information providers. |
| [`AsyncDiagnostic`](#asyncdiagnostic) | Asynchronous diagnostic information collector. |
| [`DiagnosticManager`](#diagnosticmanager) | Registry and manager for diagnostic providers. |
| [`PacketTransaction`](#packettransaction) | Request/response transaction with timeout and retry logic. |
| [`Task`](#task) | Abstract base class for implementing asynchronous tasks. |
| [`TaskRunner`](#taskrunner) | [Runner](#runner) for tasks that inherit the `[Task](#task)` interface. |
| [`IPacket`](#ipacket) | The basic packet type which is passed around the icey system. [IPacket](#ipacket) can be extended for each protocol to enable polymorphic processing and callbacks using [PacketStream](#packetstream) and friends. |
| [`FlagPacket`](#flagpacket) | Packet for sending bitwise flags along the packet stream. |
| [`RawPacket`](#rawpacket) | [RawPacket](#rawpacket) is the default data packet type which consists of an optionally managed char pointer and a size value. |
| [`Configuration`](#configuration) | [Configuration](#configuration) is an abstract base class for managing different kinds of configuration storage back ends such as JSON, XML, or database. |
| [`ScopedConfiguration`](#scopedconfiguration) | [ScopedConfiguration](#scopedconfiguration) provides multiple levels of configuration for a module. Multiple levels means that there is a module level scope, and a default scope. When a property is accessed, the module scope value will be used if available, otherwise the default scope value will be used. |
| [`Error`](#error) | Basic error type. |
| [`LogStream`](#logstream) | No-op log record used when logging is compiled out. |
| [`NullSharedMutex`](#nullsharedmutex) | No-op mutex for single-threaded signal usage. When all signal operations occur on a single libuv event loop thread, the shared_mutex is unnecessary overhead. |
| [`Bitwise`](#bitwise) | Container for smart management of bitwise integer flags. |
| [`AbstractDelegate`](#abstractdelegate) | Abstract delegate interface. |
| [`FunctionDelegate`](#functiondelegate) | The `[FunctionDelegate](#functiondelegate)` contains a `std::function`. |
| [`ClassDelegate`](#classdelegate) | The `[ClassDelegate](#classdelegate)` contains a pointer to a class member. |
| [`ConstClassDelegate`](#constclassdelegate) | The `[ConstClassDelegate](#constclassdelegate)` contains a pointer to a `const` class member. |
| [`PolymorphicDelegate`](#polymorphicdelegate) | Polymorphic function delegate. |
| [`OptionParser`](#optionparser) | Command-line option parser. |
| [`ShutdownCmd`](#shutdowncmd) | Shutdown command packet for signalling process termination. |
| [`PacketAdapterReference`](#packetadapterreference) | Provides a reference to a [PacketStreamAdapter](#packetstreamadapter) with optional ownership. |
| [`PacketStreamState`](#packetstreamstate) | [State](#state) machine states for [PacketStream](#packetstream). |
| [`PacketCreationStrategy`](#packetcreationstrategy) | This template class implements an adapter that sits between an SignalBase and an object receiving notifications from it. |
| [`SharedLibrary`](#sharedlibrary) | Loads a shared library at runtime and resolves exported symbols. |
| [`DiagnosticState`](#diagnosticstate) | [State](#state) definitions for diagnostic providers. |
| [`TransactionState`](#transactionstate) | [State](#state) machine states for [PacketTransaction](#packettransaction). |
| [`IPacketInfo`](#ipacketinfo) | An abstract interface for packet sources to provide extra information about packets. |

### Enumerations

| Name | Description |
|------|-------------|
| [`Level`](#level)  | Logging severity levels used by [Logger](#logger) and [LogChannel](#logchannel). |
| [`ByteOrder`](#byteorder)  | Byte-order policy used when reading or writing multi-byte integers. |
| [`PacketRetention`](#packetretention)  | Describes how an adapter treats incoming packet lifetime beyond the current synchronous call chain. |
| [`PacketFlags`](#packetflags)  | Flags which determine how the packet is handled by the [PacketStream](#packetstream). |

---

{#level}

#### Level

```cpp
enum Level
```

Logging severity levels used by [Logger](#logger) and [LogChannel](#logchannel).

| Value | Description |
|-------|-------------|
| `Trace` | Most verbose diagnostic output. |
| `Debug` | Debug-only diagnostic output. |
| `Info` | Normal informational output. |
| `Warn` | Warning output for recoverable problems. |
| `Error` | [Error](#error) output for failed operations. |
| `Fatal` | Fatal output immediately before termination. |

---

{#byteorder}

#### ByteOrder

```cpp
enum ByteOrder
```

Byte-order policy used when reading or writing multi-byte integers.

| Value | Description |
|-------|-------------|
| `Network` | Default, use network byte order (big endian). |
| `Host` | Use the native order of the host. |

---

{#packetretention}

#### PacketRetention

```cpp
enum PacketRetention
```

Describes how an adapter treats incoming packet lifetime beyond the current synchronous call chain.

Borrowed adapters must finish using the packet before emit()/process() returns. Cloned and Retained adapters are explicit ownership boundaries: callers may mutate or free borrowed input only after such a boundary, or after the whole synchronous write/emit call has returned.

| Value | Description |
|-------|-------------|
| `Borrowed` | Packet is only used synchronously during the current emit/process call. |
| `Cloned` | Adapter makes its own copy before deferred or asynchronous use. |
| `Retained` | Adapter retains the original packet object beyond the current call chain. |

---

{#packetflags}

#### PacketFlags

```cpp
enum PacketFlags
```

Flags which determine how the packet is handled by the [PacketStream](#packetstream).

| Value | Description |
|-------|-------------|
| `NoModify` | The packet should not be modified by processors. |
| `Final` | The final packet in the stream. |

### Typedefs

| Return | Name | Description |
|--------|------|-------------|
| `std::vector< char >` | [`Buffer`](#buffer-2)  | Core buffer type. |
| `LogChannel` | [`NullChannel`](#nullchannel)  | Null log channel. |
| `Signal< void()>` | [`NullSignal`](#nullsignal)  | Zero-argument signal alias used for simple local event notifications. |
| `Signal< RT, std::shared_mutex >` | [`ThreadSignal`](#threadsignal)  | Cross-thread signal variant. |
| `Signal< RT, NullSharedMutex >` | [`LocalSignal`](#localsignal)  | Compatibility alias for the single-threaded fast path. |
| `uv_process_options_t` | [`ProcessOptions`](#processoptions)  | Raw `libuv` process spawn options passed through to `uv_spawn`. |
| `std::map< std::string, std::string >` | [`StringMap`](#stringmap)  | Generic string-to-string map used for headers, options, and environment-style metadata. |
| `std::vector< std::string >` | [`StringVec`](#stringvec)  | Generic string vector used for argument lists and ordered string collections. |
| `std::map< std::string, std::string >` | [`OptionMap`](#optionmap)  | Command Line Option Parser. |
| `Signal< void(IPacket &)>` | [`PacketSignal`](#packetsignal)  | [Signal](#signal) that broadcasts `[IPacket](#ipacket)` types. |
| `PacketStreamAdapter` | [`PacketSource`](#packetsource)  | For 0.8.x compatibility. |
| `PacketProcessor` | [`IPacketizer`](#ipacketizer)  | Compatibility alias for a packet processor that packetizes stream output. |
| `PacketProcessor` | [`IDepacketizer`](#idepacketizer)  | For 0.8.x compatibility. |
| `std::vector< PacketAdapterReference::Ptr >` | [`PacketAdapterVec`](#packetadaptervec)  | Ordered list of packet adapter references used for sources and processors. |
| `std::vector< PacketStream * >` | [`PacketStreamVec`](#packetstreamvec)  | Non-owning list of packet stream pointers used for graph traversal helpers. |
| `std::vector< PacketStream::Ptr >` | [`PacketStreamPtrVec`](#packetstreamptrvec)  | Owning list of packet stream handles retained across stream graphs. |
| `std::unique_ptr< IPacketCreationStrategy >` | [`PacketCreationStrategyPtr`](#packetcreationstrategyptr)  | Owning handle for one packet creation strategy. |
| `std::vector< PacketCreationStrategyPtr >` | [`PacketCreationStrategyList`](#packetcreationstrategylist)  | Ordered list of packet creation strategies consulted by a packet factory. |

---

{#buffer-2}

#### Buffer

```cpp
using Buffer = std::vector< char >
```

Core buffer type.

---

{#nullchannel}

#### NullChannel

```cpp
using NullChannel = LogChannel
```

Null log channel.

Redifine the base `[LogChannel](#logchannel)` as `[NullChannel](#nullchannel)` so it can be logically used as a disabled log channel.

---

{#nullsignal}

#### NullSignal

```cpp
using NullSignal = Signal< void()>
```

Zero-argument signal alias used for simple local event notifications.

---

{#threadsignal}

#### ThreadSignal

```cpp
using ThreadSignal = Signal< RT, std::shared_mutex >
```

Cross-thread signal variant.

---

{#localsignal}

#### LocalSignal

```cpp
using LocalSignal = Signal< RT, NullSharedMutex >
```

Compatibility alias for the single-threaded fast path.

---

{#processoptions}

#### ProcessOptions

```cpp
using ProcessOptions = uv_process_options_t
```

Raw `libuv` process spawn options passed through to `uv_spawn`.

---

{#stringmap}

#### StringMap

```cpp
using StringMap = std::map< std::string, std::string >
```

Generic string-to-string map used for headers, options, and environment-style metadata.

---

{#stringvec}

#### StringVec

```cpp
using StringVec = std::vector< std::string >
```

Generic string vector used for argument lists and ordered string collections.

---

{#optionmap}

#### OptionMap

```cpp
using OptionMap = std::map< std::string, std::string >
```

Command Line Option Parser.

---

{#packetsignal}

#### PacketSignal

```cpp
using PacketSignal = Signal< void(IPacket &)>
```

[Signal](#signal) that broadcasts `[IPacket](#ipacket)` types.

---

{#packetsource}

#### PacketSource

```cpp
using PacketSource = PacketStreamAdapter
```

For 0.8.x compatibility.

---

{#ipacketizer}

#### IPacketizer

```cpp
using IPacketizer = PacketProcessor
```

Compatibility alias for a packet processor that packetizes stream output.

---

{#idepacketizer}

#### IDepacketizer

```cpp
using IDepacketizer = PacketProcessor
```

For 0.8.x compatibility.

---

{#packetadaptervec}

#### PacketAdapterVec

```cpp
using PacketAdapterVec = std::vector< PacketAdapterReference::Ptr >
```

Ordered list of packet adapter references used for sources and processors.

---

{#packetstreamvec}

#### PacketStreamVec

```cpp
using PacketStreamVec = std::vector< PacketStream * >
```

Non-owning list of packet stream pointers used for graph traversal helpers.

---

{#packetstreamptrvec}

#### PacketStreamPtrVec

```cpp
using PacketStreamPtrVec = std::vector< PacketStream::Ptr >
```

Owning list of packet stream handles retained across stream graphs.

---

{#packetcreationstrategyptr}

#### PacketCreationStrategyPtr

```cpp
using PacketCreationStrategyPtr = std::unique_ptr< IPacketCreationStrategy >
```

Owning handle for one packet creation strategy.

---

{#packetcreationstrategylist}

#### PacketCreationStrategyList

```cpp
using PacketCreationStrategyList = std::vector< PacketCreationStrategyPtr >
```

Ordered list of packet creation strategies consulted by a packet factory.

### Functions

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`runOnce`](#runonce)  | Schedules func to run once at the beginning of the next event loop iteration. Uses a uv_prepare_t handle that self-destructs after the first invocation. |
| `std::string` | [`formatError`](#formaterror) `inline` | Formats a human-readable error string from a message and a libuv error code. If `err` is not `UV_UNKNOWN`, the libuv error description is appended after a colon. |
| `void` | [`throwError`](#throwerror) `inline` | Throws a `std::runtime_error` with a formatted error message. |
| `MutableBuffer` | [`mutableBuffer`](#mutablebuffer-1) `inline` | Creates a `[MutableBuffer](#mutablebuffer)` from an arbitrary pointer and size. |
| `MutableBuffer` | [`mutableBuffer`](#mutablebuffer-2) `inline` | Creates a `[MutableBuffer](#mutablebuffer)` from a `std::string`. |
| `MutableBuffer` | [`mutableBuffer`](#mutablebuffer-3) `inline` | Creates a `[MutableBuffer](#mutablebuffer)` from a const `std::string`. Casts away constness; use with care. |
| `MutableBuffer` | [`mutableBuffer`](#mutablebuffer-4) `inline` | Creates a `[MutableBuffer](#mutablebuffer)` from a const `std::vector`. Casts away constness; use with care. |
| `MutableBuffer` | [`mutableBuffer`](#mutablebuffer-5) `inline` | Creates a `[MutableBuffer](#mutablebuffer)` from a `[Buffer](#buffer-2)`. |
| `MutableBuffer` | [`mutableBuffer`](#mutablebuffer-6) `inline` | Creates a `[MutableBuffer](#mutablebuffer)` from a const `[Buffer](#buffer-2)`. Casts away constness; use with care. |
| `ConstBuffer` | [`constBuffer`](#constbuffer-1) `inline` | Creates a `[ConstBuffer](#constbuffer)` from an arbitrary pointer and size. |
| `ConstBuffer` | [`constBuffer`](#constbuffer-2) `inline` | Creates a `[ConstBuffer](#constbuffer)` from a `std::string`. |
| `ConstBuffer` | [`constBuffer`](#constbuffer-3) `inline` | Creates a `[ConstBuffer](#constbuffer)` from a `std::vector`. |
| `ConstBuffer` | [`constBuffer`](#constbuffer-4) `inline` `constexpr` | Creates a `[ConstBuffer](#constbuffer)` from a `[MutableBuffer](#mutablebuffer)`. |
| `ConstBuffer` | [`constBuffer`](#constbuffer-5) `inline` | Creates a `[ConstBuffer](#constbuffer)` from a `[Buffer](#buffer-2)`. |
| `ConstBuffer` | [`constBuffer`](#constbuffer-6) `inline` | Creates a `[ConstBuffer](#constbuffer)` from a const `[Buffer](#buffer-2)`. Casts away constness internally; use with care. |
| `PointerToPodType` | [`bufferCast`](#buffercast) `inline` `constexpr` | Casts a `[MutableBuffer](#mutablebuffer)` to a specified pointer-to-POD type. |
| `PointerToPodType` | [`bufferCast`](#buffercast-1) `inline` `constexpr` | Casts a `[ConstBuffer](#constbuffer)` to a specified pointer-to-POD type. |
| `Level` | [`getLevelFromString`](#getlevelfromstring) `inline` | Converts a log level string to its corresponding `[Level](#level)` enum value. Unrecognized strings default to `[Level::Trace](#namespaceicy_1aad3e81b3cd2daab89338dae9b5323f6badd4ec0ac4e58f7c32a01244ae91150b1)`. |
| `const char *` | [`getStringFromLevel`](#getstringfromlevel) `inline` | Converts a `[Level](#level)` enum value to its lowercase string representation. |
| `void` | [`logArgs`](#logargs)  | Write a single logging argument into the destination stream. |
| `void` | [`logArgs`](#logargs-1)  | Write multiple logging arguments into the destination stream in order. |
| `const char *` | [`str_end`](#str_end) `constexpr` | Return a pointer to the null terminator of a C string. |
| `bool` | [`str_slant`](#str_slant) `constexpr` | Return true if the C string contains a forward or back slash. |
| `const char *` | [`r_slant`](#r_slant) `constexpr` | Walk backward to the character after the last path separator. |
| `const char *` | [`_fileName`](#_filename) `constexpr` | Return the filename portion of a compile-time path string. |
| `std::string` | [`_methodName`](#_methodname) `inline` | Extract the class-qualified method name from a compiler pretty-function string. |
| `void` | [`deleteLater`](#deletelater) `inline` | Schedules deferred deletion of ptr on the next event loop iteration. This is essential for deleting objects that may still be referenced by pending libuv callbacks (e.g. socket adapters with in-flight I/O). Uses a self-cleaning uv_idle_t handle that fires once and then closes itself. |
| `IntrusivePtr< T >` | [`makeIntrusive`](#makeintrusive)  | Creates an [IntrusivePtr](#intrusiveptr) managing a newly heap-allocated T. Equivalent to std::make_shared. |
| `std::shared_ptr< internal::Slot< RT, Args... > >` | [`slot`](#slot)  | Creates a slot that binds a non-const class member function to an instance. |
| `std::shared_ptr< internal::Slot< RT, Args... > >` | [`slot`](#slot-1)  | Creates a slot that wraps a free (static) function pointer. |
| `void` | [`swap`](#swap-2) `inline` | Exchange two [DateTime](#datetime) values. |
| `void` | [`swap`](#swap-3) `inline` | Exchange two [LocalDateTime](#localdatetime) values. |
| `void` | [`swap`](#swap-4) `inline` | Exchange two [Timestamp](#timestamp) values. |
| `void` | [`swap`](#swap-5) `inline` | Exchange two [Timespan](#timespan) values. |
| `std::string` | [`getExePath`](#getexepath)  | Cross-platform utilities. |
| `std::string` | [`getCwd`](#getcwd)  | Return the current working directory. |
| `uint64_t` | [`getFreeMemory`](#getfreememory)  | Returns the current amount of free memory. |
| `uint64_t` | [`getTotalMemory`](#gettotalmemory)  | Returns the current amount of used memory. |
| `int` | [`numCpuCores`](#numcpucores)  | Returns the number of CPU cores. |
| `void` | [`sleep`](#sleep)  | Pause the current thread for the given ms duration. |
| `void` | [`pause`](#pause)  | Pause the current thread until enter is pressed. |
| `std::string` | [`getHostname`](#gethostname)  | Return the system hostname. |
| `std::string` | [`getEnv`](#getenv)  | Return an environment variable or the default value. |
| `bool` | [`getEnvBool`](#getenvbool)  | Return an environment variable boolean or the default value. The variable must be `1` or `true` for this function to return true. |
| `void` | [`set8`](#set8) `inline` | Writes a single byte at the given offset in memory. |
| `uint8_t` | [`get8`](#get8) `inline` | Reads a single byte at the given offset from memory. |
| `void` | [`setBE16`](#setbe16) `inline` | Writes a 16-bit value to memory in big-endian byte order. |
| `void` | [`setBE32`](#setbe32) `inline` | Writes a 32-bit value to memory in big-endian byte order. |
| `void` | [`setBE64`](#setbe64) `inline` | Writes a 64-bit value to memory in big-endian byte order. |
| `uint16_t` | [`getBE16`](#getbe16) `inline` | Reads a 16-bit big-endian value from memory. |
| `uint32_t` | [`getBE32`](#getbe32) `inline` | Reads a 32-bit big-endian value from memory. |
| `uint64_t` | [`getBE64`](#getbe64) `inline` | Reads a 64-bit big-endian value from memory. |
| `void` | [`setLE16`](#setle16) `inline` | Writes a 16-bit value to memory in little-endian byte order. |
| `void` | [`setLE32`](#setle32) `inline` | Writes a 32-bit value to memory in little-endian byte order. |
| `void` | [`setLE64`](#setle64) `inline` | Writes a 64-bit value to memory in little-endian byte order. |
| `uint16_t` | [`getLE16`](#getle16) `inline` | Reads a 16-bit little-endian value from memory. |
| `uint32_t` | [`getLE32`](#getle32) `inline` | Reads a 32-bit little-endian value from memory. |
| `uint64_t` | [`getLE64`](#getle64) `inline` | Reads a 64-bit little-endian value from memory. |
| `bool` | [`isBigEndian`](#isbigendian) `inline` | Returns true if the host CPU is big-endian. |
| `uint16_t` | [`hostToNetwork16`](#hosttonetwork16) `inline` | Converts a 16-bit value from host byte order to network (big-endian) byte order. |
| `uint32_t` | [`hostToNetwork32`](#hosttonetwork32) `inline` | Converts a 32-bit value from host byte order to network (big-endian) byte order. |
| `uint64_t` | [`hostToNetwork64`](#hosttonetwork64) `inline` | Converts a 64-bit value from host byte order to network (big-endian) byte order. |
| `uint16_t` | [`networkToHost16`](#networktohost16) `inline` | Converts a 16-bit value from network (big-endian) byte order to host byte order. |
| `uint32_t` | [`networkToHost32`](#networktohost32) `inline` | Converts a 32-bit value from network (big-endian) byte order to host byte order. |
| `uint64_t` | [`networkToHost64`](#networktohost64) `inline` | Converts a 64-bit value from network (big-endian) byte order to host byte order. |
| `void` | [`onShutdownSignal`](#onshutdownsignal) `inline` | Installs a SIGINT handler on the given event loop. When the signal fires, `callback` is invoked with `opaque` and the signal handle is closed. |
| `void` | [`waitForShutdown`](#waitforshutdown) `inline` | Installs a SIGINT handler and runs the event loop until shutdown. Equivalent to calling `[onShutdownSignal()](#onshutdownsignal)` then `uv_run()`. |
| `std::shared_ptr< internal::Slot< RT, IT & > >` | [`packetSlot`](#packetslot)  | Creates a signal slot that filters by packet subtype `PT` before invoking `method`. |
| `unsigned` | [`operator|`](#operator-10) `constexpr` | Combine [PacketFlags](#packetflags) values into a bitmask. |
| `RawPacket` | [`rawPacket`](#rawpacket-1) `inline` | Constructs a non-owning [RawPacket](#rawpacket) from a mutable buffer (borrowed pointer). |
| `RawPacket` | [`rawPacket`](#rawpacket-2) `inline` | Constructs an owning [RawPacket](#rawpacket) from a const buffer (data is copied). |
| `RawPacket` | [`rawPacket`](#rawpacket-3) `inline` | Constructs a non-owning [RawPacket](#rawpacket) from a raw mutable pointer (borrowed). |
| `RawPacket` | [`rawPacket`](#rawpacket-4) `inline` | Constructs an owning [RawPacket](#rawpacket) from a const char pointer (data is copied). |

---

{#runonce}

#### runOnce

```cpp
template<typename Function, typename... Args> void runOnce(uv::Loop * loop, Function && func, Args &&... args)
```

Schedules func to run once at the beginning of the next event loop iteration. Uses a uv_prepare_t handle that self-destructs after the first invocation. 
#### Parameters
* `loop` Event loop on which to schedule the callback. 

* `func` Callable to invoke on the next loop tick. 

* `args` Arguments forwarded to func.

---

{#formaterror}

#### formatError

`inline`

```cpp
inline std::string formatError(std::string_view message, int err = UV_UNKNOWN)
```

Formats a human-readable error string from a message and a libuv error code. If `err` is not `UV_UNKNOWN`, the libuv error description is appended after a colon. 
#### Parameters
* `message` Descriptive context for the error. 

* `err` libuv error code (e.g. from a failed `uv_*` call). Defaults to `UV_UNKNOWN`. 

#### Returns
Formatted error string.

---

{#throwerror}

#### throwError

`inline`

```cpp
inline void throwError(std::string_view message, int err = UV_UNKNOWN)
```

Throws a `std::runtime_error` with a formatted error message. 
#### Parameters
* `message` Descriptive context for the error. 

* `err` libuv error code to append. Defaults to `UV_UNKNOWN`.

---

{#mutablebuffer-1}

#### mutableBuffer

`inline`

```cpp
template<typename T> inline MutableBuffer mutableBuffer(T data, size_t size)
```

Creates a `[MutableBuffer](#mutablebuffer)` from an arbitrary pointer and size. 
#### Parameters
* `T` Pointer type; must be implicitly castable to `void*`. 

#### Parameters
* `data` Pointer to the start of the memory region. 

* `size` Number of bytes in the region. 

#### Returns
`[MutableBuffer](#mutablebuffer)` referencing the given memory.

---

{#mutablebuffer-2}

#### mutableBuffer

`inline`

```cpp
inline MutableBuffer mutableBuffer(std::string & str)
```

Creates a `[MutableBuffer](#mutablebuffer)` from a `std::string`. 
#### Parameters
* `str` Source string. Must remain valid while the buffer is in use. 

#### Returns
`[MutableBuffer](#mutablebuffer)` wrapping the string's internal storage.

---

{#mutablebuffer-3}

#### mutableBuffer

`inline`

```cpp
inline MutableBuffer mutableBuffer(const std::string & str)
```

Creates a `[MutableBuffer](#mutablebuffer)` from a const `std::string`. Casts away constness; use with care. 
#### Parameters
* `str` Source string. Must remain valid while the buffer is in use. 

#### Returns
`[MutableBuffer](#mutablebuffer)` wrapping the string's internal storage.

---

{#mutablebuffer-4}

#### mutableBuffer

`inline`

```cpp
template<typename T> inline MutableBuffer mutableBuffer(const std::vector< T > & vec)
```

Creates a `[MutableBuffer](#mutablebuffer)` from a const `std::vector`. Casts away constness; use with care. 
#### Parameters
* `T` Element type of the vector. 

#### Parameters
* `vec` Source vector. Must remain valid while the buffer is in use. 

#### Returns
`[MutableBuffer](#mutablebuffer)` wrapping the vector's internal storage.

---

{#mutablebuffer-5}

#### mutableBuffer

`inline`

```cpp
inline MutableBuffer mutableBuffer(Buffer & buf)
```

Creates a `[MutableBuffer](#mutablebuffer)` from a `[Buffer](#buffer-2)`. 
#### Parameters
* `buf` Source buffer. Must remain valid while the buffer is in use. 

#### Returns
`[MutableBuffer](#mutablebuffer)` wrapping the buffer's data.

---

{#mutablebuffer-6}

#### mutableBuffer

`inline`

```cpp
inline MutableBuffer mutableBuffer(const Buffer & buf)
```

Creates a `[MutableBuffer](#mutablebuffer)` from a const `[Buffer](#buffer-2)`. Casts away constness; use with care. 
#### Parameters
* `buf` Source buffer. Must remain valid while the buffer is in use. 

#### Returns
`[MutableBuffer](#mutablebuffer)` wrapping the buffer's data.

---

{#constbuffer-1}

#### constBuffer

`inline`

```cpp
template<typename T> inline ConstBuffer constBuffer(T data, size_t size)
```

Creates a `[ConstBuffer](#constbuffer)` from an arbitrary pointer and size. 
#### Parameters
* `T` Pointer type; must be implicitly castable to `const void*`. 

#### Parameters
* `data` Pointer to the start of the memory region. 

* `size` Number of bytes in the region. 

#### Returns
`[ConstBuffer](#constbuffer)` referencing the given memory.

---

{#constbuffer-2}

#### constBuffer

`inline`

```cpp
inline ConstBuffer constBuffer(const std::string & str)
```

Creates a `[ConstBuffer](#constbuffer)` from a `std::string`. 
#### Parameters
* `str` Source string. Must remain valid while the buffer is in use. 

#### Returns
`[ConstBuffer](#constbuffer)` wrapping the string's internal storage.

---

{#constbuffer-3}

#### constBuffer

`inline`

```cpp
template<typename T> inline ConstBuffer constBuffer(const std::vector< T > & vec)
```

Creates a `[ConstBuffer](#constbuffer)` from a `std::vector`. 
#### Parameters
* `T` Element type of the vector. 

#### Parameters
* `vec` Source vector. Must remain valid while the buffer is in use. 

#### Returns
`[ConstBuffer](#constbuffer)` wrapping the vector's internal storage.

---

{#constbuffer-4}

#### constBuffer

`inline` `constexpr`

```cpp
constexpr inline ConstBuffer constBuffer(const MutableBuffer & buf)
```

Creates a `[ConstBuffer](#constbuffer)` from a `[MutableBuffer](#mutablebuffer)`. 
#### Parameters
* `buf` Source mutable buffer. 

#### Returns
`[ConstBuffer](#constbuffer)` referencing the same memory region.

---

{#constbuffer-5}

#### constBuffer

`inline`

```cpp
template<typename T> inline ConstBuffer constBuffer(Buffer & buf)
```

Creates a `[ConstBuffer](#constbuffer)` from a `[Buffer](#buffer-2)`. 
#### Parameters
* `T` Unused; kept for overload symmetry. 

#### Parameters
* `buf` Source buffer. Must remain valid while the buffer is in use. 

#### Returns
`[ConstBuffer](#constbuffer)` wrapping the buffer's data.

---

{#constbuffer-6}

#### constBuffer

`inline`

```cpp
template<typename T> inline ConstBuffer constBuffer(const Buffer & buf)
```

Creates a `[ConstBuffer](#constbuffer)` from a const `[Buffer](#buffer-2)`. Casts away constness internally; use with care. 
#### Parameters
* `T` Unused; kept for overload symmetry. 

#### Parameters
* `buf` Source buffer. Must remain valid while the buffer is in use. 

#### Returns
`[ConstBuffer](#constbuffer)` wrapping the buffer's data.

---

{#buffercast}

#### bufferCast

`inline` `constexpr`

```cpp
template<typename PointerToPodType> constexpr inline PointerToPodType bufferCast(const MutableBuffer & b)
```

Casts a `[MutableBuffer](#mutablebuffer)` to a specified pointer-to-POD type. 
#### Parameters
* `PointerToPodType` Target pointer type (e.g. `char*`, `uint8_t*`). 

#### Parameters
* `b` Source mutable buffer. 

#### Returns
Pointer to the buffer's data, cast to `PointerToPodType`.

---

{#buffercast-1}

#### bufferCast

`inline` `constexpr`

```cpp
template<typename PointerToPodType> constexpr inline PointerToPodType bufferCast(const ConstBuffer & b)
```

Casts a `[ConstBuffer](#constbuffer)` to a specified pointer-to-POD type. 
#### Parameters
* `PointerToPodType` Target pointer type (e.g. `const char*`, `const uint8_t*`). 

#### Parameters
* `b` Source const buffer. 

#### Returns
Pointer to the buffer's data, cast to `PointerToPodType`.

---

{#getlevelfromstring}

#### getLevelFromString

`inline`

```cpp
inline Level getLevelFromString(const char * level)
```

Converts a log level string to its corresponding `[Level](#level)` enum value. Unrecognized strings default to `[Level::Trace](#namespaceicy_1aad3e81b3cd2daab89338dae9b5323f6badd4ec0ac4e58f7c32a01244ae91150b1)`. 
#### Parameters
* `level` Lowercase level string: "trace", "debug", "info", "warn", "error", or "fatal". 

#### Returns
The matching `[Level](#level)` enum value.

---

{#getstringfromlevel}

#### getStringFromLevel

`inline`

```cpp
inline const char * getStringFromLevel(Level level)
```

Converts a `[Level](#level)` enum value to its lowercase string representation. 
#### Parameters
* `level` The log level to convert. 

#### Returns
Lowercase C string: "trace", "debug", "info", "warn", "error", or "fatal".

---

{#logargs}

#### logArgs

```cpp
template<typename T> void logArgs(std::ostream & o, T && t)
```

Write a single logging argument into the destination stream. 
#### Parameters
* `o` Destination stream. 

* `t` Argument to append with operator<<.

---

{#logargs-1}

#### logArgs

```cpp
template<typename T, typename... Args> void logArgs(std::ostream & o, T && t, Args &&... args)
```

Write multiple logging arguments into the destination stream in order. 
#### Parameters
* `o` Destination stream. 

* `t` First argument to append. 

* `args` Remaining arguments to append recursively.

---

{#str_end}

#### str_end

`constexpr`

```cpp
constexpr const char * str_end(const char * str)
```

Return a pointer to the null terminator of a C string.

---

{#str_slant}

#### str_slant

`constexpr`

```cpp
constexpr bool str_slant(const char * str)
```

Return true if the C string contains a forward or back slash.

---

{#r_slant}

#### r_slant

`constexpr`

```cpp
constexpr const char * r_slant(const char * str)
```

Walk backward to the character after the last path separator.

---

{#_filename}

#### _fileName

`constexpr`

```cpp
constexpr const char * _fileName(const char * str)
```

Return the filename portion of a compile-time path string.

---

{#_methodname}

#### _methodName

`inline`

```cpp
inline std::string _methodName(std::string_view fsig)
```

Extract the class-qualified method name from a compiler pretty-function string.

---

{#deletelater}

#### deleteLater

`inline`

```cpp
template<typename T> inline void deleteLater(T * ptr, uv::Loop * loop)
```

Schedules deferred deletion of ptr on the next event loop iteration. This is essential for deleting objects that may still be referenced by pending libuv callbacks (e.g. socket adapters with in-flight I/O). Uses a self-cleaning uv_idle_t handle that fires once and then closes itself. 
#### Parameters
* `T` Type of the object to delete. 

#### Parameters
* `ptr` Object to delete. Does nothing if nullptr. 

* `loop` Event loop on which to schedule the deletion.

---

{#makeintrusive}

#### makeIntrusive

```cpp
template<typename T, typename... Args> IntrusivePtr< T > makeIntrusive(Args &&... args)
```

Creates an [IntrusivePtr](#intrusiveptr) managing a newly heap-allocated T. Equivalent to std::make_shared. 
#### Parameters
* `T` Type to construct; must inherit from [RefCounted<T>](#refcounted). 

#### Parameters
* `args` Arguments forwarded to T's constructor. 

#### Returns
[IntrusivePtr<T>](#intrusiveptr) owning the new object.

---

{#slot}

#### slot

```cpp
template<class Class, class RT, typename... Args> std::shared_ptr< internal::Slot< RT, Args... > > slot(Class * instance, RT(Class::*)(Args...) method, int id = -1, int priority = -1)
```

Creates a slot that binds a non-const class member function to an instance.

The returned `SlotPtr` can be passed to `Signal::attach()` or `operator+=`, and to `Signal::detach()` or `operator-=` to disconnect it later.

#### Parameters
* `Class` The class that owns the member function. 

* `RT` Return type of the member function. 

* `Args` Parameter types of the member function. 

#### Parameters
* `instance` Pointer to the object on which `method` will be called. 

* `method` Pointer to the non-const member function to bind. 

* `id` Explicit slot ID to assign; pass `-1` to auto-assign. 

* `priority` Higher values are called first; pass `-1` for default ordering. 

#### Returns
A `SlotPtr` ready to attach to a compatible `[Signal](#signal)`.

Creates a slot that binds a `const` class member function to an instance. Uses the same slot ID and priority rules as the non-const overload above.

---

{#slot-1}

#### slot

```cpp
template<class RT, typename... Args> std::shared_ptr< internal::Slot< RT, Args... > > slot(RT(*)(Args...) method, int id = -1, int priority = -1)
```

Creates a slot that wraps a free (static) function pointer.

#### Parameters
* `RT` Return type of the function. 

* `Args` Parameter types of the function. 

#### Parameters
* `method` Pointer to the free function to bind. 

* `id` Explicit slot ID to assign; pass `-1` to auto-assign. 

* `priority` Higher values are called first; pass `-1` for default ordering. 

#### Returns
A `SlotPtr` ready to attach to a compatible `[Signal](#signal)`.

---

{#swap-2}

#### swap

`inline`

```cpp
inline void swap(DateTime & d1, DateTime & d2)
```

Exchange two [DateTime](#datetime) values.

---

{#swap-3}

#### swap

`inline`

```cpp
inline void swap(LocalDateTime & d1, LocalDateTime & d2)
```

Exchange two [LocalDateTime](#localdatetime) values.

---

{#swap-4}

#### swap

`inline`

```cpp
inline void swap(Timestamp & s1, Timestamp & s2)
```

Exchange two [Timestamp](#timestamp) values.

---

{#swap-5}

#### swap

`inline`

```cpp
inline void swap(Timespan & s1, Timespan & s2)
```

Exchange two [Timespan](#timespan) values.

---

{#getexepath}

#### getExePath

```cpp
std::string getExePath()
```

Cross-platform utilities.

Returns the current executable path.

---

{#getcwd}

#### getCwd

```cpp
std::string getCwd()
```

Return the current working directory.

---

{#getfreememory}

#### getFreeMemory

```cpp
uint64_t getFreeMemory()
```

Returns the current amount of free memory.

---

{#gettotalmemory}

#### getTotalMemory

```cpp
uint64_t getTotalMemory()
```

Returns the current amount of used memory.

---

{#numcpucores}

#### numCpuCores

```cpp
int numCpuCores()
```

Returns the number of CPU cores.

---

{#sleep}

#### sleep

```cpp
void sleep(int ms)
```

Pause the current thread for the given ms duration.

---

{#pause}

#### pause

```cpp
void pause()
```

Pause the current thread until enter is pressed.

---

{#gethostname}

#### getHostname

```cpp
std::string getHostname()
```

Return the system hostname.

---

{#getenv}

#### getEnv

```cpp
std::string getEnv(std::string_view name, std::string_view defaultValue = "")
```

Return an environment variable or the default value.

---

{#getenvbool}

#### getEnvBool

```cpp
bool getEnvBool(std::string_view name)
```

Return an environment variable boolean or the default value. The variable must be `1` or `true` for this function to return true.

---

{#set8}

#### set8

`inline`

```cpp
inline void set8(void * memory, size_t offset, uint8_t v)
```

Writes a single byte at the given offset in memory. 
#### Parameters
* `memory` Pointer to the destination buffer. 

* `offset` Byte offset within the buffer. 

* `v` Value to write.

---

{#get8}

#### get8

`inline`

```cpp
inline uint8_t get8(const void * memory, size_t offset)
```

Reads a single byte at the given offset from memory. 
#### Parameters
* `memory` Pointer to the source buffer. 

* `offset` Byte offset within the buffer. 

#### Returns
The byte value at the specified offset.

---

{#setbe16}

#### setBE16

`inline`

```cpp
inline void setBE16(void * memory, uint16_t v)
```

Writes a 16-bit value to memory in big-endian byte order. 
#### Parameters
* `memory` Pointer to the destination buffer (must be at least 2 bytes). 

* `v` Value to write.

---

{#setbe32}

#### setBE32

`inline`

```cpp
inline void setBE32(void * memory, uint32_t v)
```

Writes a 32-bit value to memory in big-endian byte order. 
#### Parameters
* `memory` Pointer to the destination buffer (must be at least 4 bytes). 

* `v` Value to write.

---

{#setbe64}

#### setBE64

`inline`

```cpp
inline void setBE64(void * memory, uint64_t v)
```

Writes a 64-bit value to memory in big-endian byte order. 
#### Parameters
* `memory` Pointer to the destination buffer (must be at least 8 bytes). 

* `v` Value to write.

---

{#getbe16}

#### getBE16

`inline`

```cpp
inline uint16_t getBE16(const void * memory)
```

Reads a 16-bit big-endian value from memory. 
#### Parameters
* `memory` Pointer to the source buffer (must be at least 2 bytes). 

#### Returns
The 16-bit value in host byte order.

---

{#getbe32}

#### getBE32

`inline`

```cpp
inline uint32_t getBE32(const void * memory)
```

Reads a 32-bit big-endian value from memory. 
#### Parameters
* `memory` Pointer to the source buffer (must be at least 4 bytes). 

#### Returns
The 32-bit value in host byte order.

---

{#getbe64}

#### getBE64

`inline`

```cpp
inline uint64_t getBE64(const void * memory)
```

Reads a 64-bit big-endian value from memory. 
#### Parameters
* `memory` Pointer to the source buffer (must be at least 8 bytes). 

#### Returns
The 64-bit value in host byte order.

---

{#setle16}

#### setLE16

`inline`

```cpp
inline void setLE16(void * memory, uint16_t v)
```

Writes a 16-bit value to memory in little-endian byte order. 
#### Parameters
* `memory` Pointer to the destination buffer (must be at least 2 bytes). 

* `v` Value to write.

---

{#setle32}

#### setLE32

`inline`

```cpp
inline void setLE32(void * memory, uint32_t v)
```

Writes a 32-bit value to memory in little-endian byte order. 
#### Parameters
* `memory` Pointer to the destination buffer (must be at least 4 bytes). 

* `v` Value to write.

---

{#setle64}

#### setLE64

`inline`

```cpp
inline void setLE64(void * memory, uint64_t v)
```

Writes a 64-bit value to memory in little-endian byte order. 
#### Parameters
* `memory` Pointer to the destination buffer (must be at least 8 bytes). 

* `v` Value to write.

---

{#getle16}

#### getLE16

`inline`

```cpp
inline uint16_t getLE16(const void * memory)
```

Reads a 16-bit little-endian value from memory. 
#### Parameters
* `memory` Pointer to the source buffer (must be at least 2 bytes). 

#### Returns
The 16-bit value in host byte order.

---

{#getle32}

#### getLE32

`inline`

```cpp
inline uint32_t getLE32(const void * memory)
```

Reads a 32-bit little-endian value from memory. 
#### Parameters
* `memory` Pointer to the source buffer (must be at least 4 bytes). 

#### Returns
The 32-bit value in host byte order.

---

{#getle64}

#### getLE64

`inline`

```cpp
inline uint64_t getLE64(const void * memory)
```

Reads a 64-bit little-endian value from memory. 
#### Parameters
* `memory` Pointer to the source buffer (must be at least 8 bytes). 

#### Returns
The 64-bit value in host byte order.

---

{#isbigendian}

#### isBigEndian

`inline`

```cpp
inline bool isBigEndian()
```

Returns true if the host CPU is big-endian. 
#### Returns
true if the host byte order is big-endian, false if little-endian.

---

{#hosttonetwork16}

#### hostToNetwork16

`inline`

```cpp
inline uint16_t hostToNetwork16(uint16_t n)
```

Converts a 16-bit value from host byte order to network (big-endian) byte order. 
#### Parameters
* `n` Value in host byte order. 

#### Returns
Value in network byte order.

---

{#hosttonetwork32}

#### hostToNetwork32

`inline`

```cpp
inline uint32_t hostToNetwork32(uint32_t n)
```

Converts a 32-bit value from host byte order to network (big-endian) byte order. 
#### Parameters
* `n` Value in host byte order. 

#### Returns
Value in network byte order.

---

{#hosttonetwork64}

#### hostToNetwork64

`inline`

```cpp
inline uint64_t hostToNetwork64(uint64_t n)
```

Converts a 64-bit value from host byte order to network (big-endian) byte order. 
#### Parameters
* `n` Value in host byte order. 

#### Returns
Value in network byte order.

---

{#networktohost16}

#### networkToHost16

`inline`

```cpp
inline uint16_t networkToHost16(uint16_t n)
```

Converts a 16-bit value from network (big-endian) byte order to host byte order. 
#### Parameters
* `n` Value in network byte order. 

#### Returns
Value in host byte order.

---

{#networktohost32}

#### networkToHost32

`inline`

```cpp
inline uint32_t networkToHost32(uint32_t n)
```

Converts a 32-bit value from network (big-endian) byte order to host byte order. 
#### Parameters
* `n` Value in network byte order. 

#### Returns
Value in host byte order.

---

{#networktohost64}

#### networkToHost64

`inline`

```cpp
inline uint64_t networkToHost64(uint64_t n)
```

Converts a 64-bit value from network (big-endian) byte order to host byte order. 
#### Parameters
* `n` Value in network byte order. 

#### Returns
Value in host byte order.

---

{#onshutdownsignal}

#### onShutdownSignal

`inline`

```cpp
inline void onShutdownSignal(std::function< void(void *)> callback = nullptr, void * opaque = nullptr, uv::Loop * loop = uv::defaultLoop())
```

Installs a SIGINT handler on the given event loop. When the signal fires, `callback` is invoked with `opaque` and the signal handle is closed. 
#### Parameters
* `callback` Optional function called on SIGINT. 

* `opaque` Optional user data pointer passed to the callback. 

* `loop` Event loop to attach the signal watcher to.

---

{#waitforshutdown}

#### waitForShutdown

`inline`

```cpp
inline void waitForShutdown(std::function< void(void *)> callback = nullptr, void * opaque = nullptr, uv::Loop * loop = uv::defaultLoop())
```

Installs a SIGINT handler and runs the event loop until shutdown. Equivalent to calling `[onShutdownSignal()](#onshutdownsignal)` then `uv_run()`. 
#### Parameters
* `callback` Optional function called on SIGINT before the loop exits. 

* `opaque` Optional user data pointer passed to the callback. 

* `loop` Event loop to run.

---

{#packetslot}

#### packetSlot

```cpp
template<class Class, class RT, class PT, class IT = IPacket> std::shared_ptr< internal::Slot< RT, IT & > > packetSlot(Class * instance, RT(Class::*)(PT &) method, int id = -1, int priority = -1)
```

Creates a signal slot that filters by packet subtype `PT` before invoking `method`.

The returned slot is connected to a `[PacketSignal](#packetsignal)` (which broadcasts `[IPacket](#ipacket)&`). The slot performs a `dynamic_cast` on each received packet; if the cast succeeds, the listener method is called with the derived type `PT`. Non-matching packets are silently ignored.

#### Parameters
* `Class` Listener class type. 

* `RT` Return type of the listener method. 

* `PT` Derived packet type the listener expects (must derive from `IT`). 

* `IT` Base packet interface type; defaults to `[IPacket](#ipacket)`. 

#### Parameters
* `instance` Pointer to the listener object. 

* `method` Member function pointer on `Class` accepting a `PT&`. 

* `id` Optional slot identifier; -1 for automatic assignment. 

* `priority` Optional slot priority; higher values run first. 

#### Returns
A shared slot suitable for connecting to a `[PacketSignal](#packetsignal)`.

---

{#operator-10}

#### operator|

`constexpr`

```cpp
constexpr unsigned operator|(PacketFlags lhs, PacketFlags rhs)
```

Combine [PacketFlags](#packetflags) values into a bitmask.

---

{#rawpacket-1}

#### rawPacket

`inline`

```cpp
inline RawPacket rawPacket(const MutableBuffer & buf, unsigned flags = 0, std::unique_ptr< IPacketInfo > info = nullptr)
```

Constructs a non-owning [RawPacket](#rawpacket) from a mutable buffer (borrowed pointer).

---

{#rawpacket-2}

#### rawPacket

`inline`

```cpp
inline RawPacket rawPacket(const ConstBuffer & buf, unsigned flags = 0, std::unique_ptr< IPacketInfo > info = nullptr)
```

Constructs an owning [RawPacket](#rawpacket) from a const buffer (data is copied).

---

{#rawpacket-3}

#### rawPacket

`inline`

```cpp
inline RawPacket rawPacket(char * data = nullptr, size_t size = 0, unsigned flags = 0, std::unique_ptr< IPacketInfo > info = nullptr)
```

Constructs a non-owning [RawPacket](#rawpacket) from a raw mutable pointer (borrowed).

---

{#rawpacket-4}

#### rawPacket

`inline`

```cpp
inline RawPacket rawPacket(const char * data = nullptr, size_t size = 0, unsigned flags = 0, std::unique_ptr< IPacketInfo > info = nullptr)
```

Constructs an owning [RawPacket](#rawpacket) from a const char pointer (data is copied).

{#hex}

# hex

Hexadecimal encoding and decoding helpers.

### Classes

| Name | Description |
|------|-------------|
| [`Encoder`](#encoder-1) | Hex encoder. |
| [`Decoder`](#decoder) | Hex decoder. |

### Functions

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`encode`](#encode-14) `inline` | Converts the STL container to Hex. |

---

{#encode-14}

#### encode

`inline`

```cpp
template<typename T> inline std::string encode(const T & bytes)
```

Converts the STL container to Hex.

{#encoder-1}

## Encoder

```cpp
#include <icy/hex.h>
```

```cpp
struct Encoder
```

Defined in src/base/include/icy/hex.h:34

> **Inherits:** [`Encoder`](#encoder-5)

Hex encoder.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `int` | [`_linePos`](#_linepos)  |  |
| `int` | [`_lineLength`](#_linelength)  |  |
| `int` | [`_uppercase`](#_uppercase)  |  |

---

{#_linepos}

#### _linePos

```cpp
int _linePos
```

Defined in src/base/include/icy/hex.h:83

---

{#_linelength}

#### _lineLength

```cpp
int _lineLength
```

Defined in src/base/include/icy/hex.h:84

---

{#_uppercase}

#### _uppercase

```cpp
int _uppercase
```

Defined in src/base/include/icy/hex.h:85

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Encoder`](#encoder-2) `inline` |  |
| `ssize_t` | [`encode`](#encode-15) `virtual` `inline` `override` | Encodes binary input as lowercase hex characters, optionally inserting newlines every `[_lineLength](#_linelength)` output characters. |
| `ssize_t` | [`finalize`](#finalize) `virtual` `inline` `override` | No-op finalizer; hex encoding has no pending state. |
| `void` | [`setUppercase`](#setuppercase) `inline` | Controls whether encoded output uses uppercase hex digits (A-F) or lowercase (a-f). |
| `void` | [`setLineLength`](#setlinelength) `inline` | Sets the maximum number of output characters per line before a newline is inserted. Set to 0 to disable line wrapping. |

---

{#encoder-2}

#### Encoder

`inline`

```cpp
inline Encoder()
```

Defined in src/base/include/icy/hex.h:36

---

{#encode-15}

#### encode

`virtual` `inline` `override`

```cpp
virtual inline ssize_t encode(const char * inbuf, size_t nread, char * outbuf) override
```

Defined in src/base/include/icy/hex.h:49

Encodes binary input as lowercase hex characters, optionally inserting newlines every `[_lineLength](#_linelength)` output characters. 
#### Parameters
* `inbuf` Input buffer to encode. 

* `nread` Number of bytes to read from inbuf. 

* `outbuf` Destination buffer; must have capacity >= nread * 2 + nread/_lineLength + 1. 

#### Returns
Number of bytes written to outbuf.

---

{#finalize}

#### finalize

`virtual` `inline` `override`

```cpp
virtual inline ssize_t finalize(char *) override
```

Defined in src/base/include/icy/hex.h:72

No-op finalizer; hex encoding has no pending state. 
#### Returns
Always 0.

---

{#setuppercase}

#### setUppercase

`inline`

```cpp
inline void setUppercase(bool flag)
```

Defined in src/base/include/icy/hex.h:76

Controls whether encoded output uses uppercase hex digits (A-F) or lowercase (a-f). 
#### Parameters
* `flag` true for uppercase, false for lowercase.

---

{#setlinelength}

#### setLineLength

`inline`

```cpp
inline void setLineLength(int lineLength)
```

Defined in src/base/include/icy/hex.h:81

Sets the maximum number of output characters per line before a newline is inserted. Set to 0 to disable line wrapping. 
#### Parameters
* `lineLength` Characters per line.

{#decoder}

## Decoder

```cpp
#include <icy/hex.h>
```

```cpp
struct Decoder
```

Defined in src/base/include/icy/hex.h:111

> **Inherits:** [`Decoder`](#decoder-4)

Hex decoder.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `char` | [`lastbyte`](#lastbyte)  |  |

---

{#lastbyte}

#### lastbyte

```cpp
char lastbyte
```

Defined in src/base/include/icy/hex.h:201

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Decoder`](#decoder-1) `inline` |  |
| `ssize_t` | [`decode`](#decode-4) `virtual` `inline` `override` | Decodes hex-encoded input to binary. Whitespace in the input is ignored. A trailing unpaired nibble is buffered and prepended on the next call. |
| `ssize_t` | [`finalize`](#finalize-1) `virtual` `inline` `override` | No-op finalizer; hex decoding has no pending output state. |
| `bool` | [`readnext`](#readnext) `inline` | Reads the next non-whitespace character from inbuf, prepending any buffered lastbyte before consuming from the stream. |
| `int` | [`nybble`](#nybble) `inline` | Converts an ASCII hex character to its 4-bit integer value. |
| `bool` | [`iswspace`](#iswspace) `inline` | Returns true if c is an ASCII whitespace character (space, CR, tab, LF). |

---

{#decoder-1}

#### Decoder

`inline`

```cpp
inline Decoder()
```

Defined in src/base/include/icy/hex.h:113

---

{#decode-4}

#### decode

`virtual` `inline` `override`

```cpp
virtual inline ssize_t decode(const char * inbuf, size_t nread, char * outbuf) override
```

Defined in src/base/include/icy/hex.h:125

Decodes hex-encoded input to binary. Whitespace in the input is ignored. A trailing unpaired nibble is buffered and prepended on the next call. 
#### Parameters
* `inbuf` Hex-encoded input buffer. 

* `nread` Number of bytes to read from inbuf. 

* `outbuf` Destination buffer; must have capacity >= nread / 2. 

#### Returns
Number of decoded bytes written to outbuf.

---

{#finalize-1}

#### finalize

`virtual` `inline` `override`

```cpp
virtual inline ssize_t finalize(char *) override
```

Defined in src/base/include/icy/hex.h:152

No-op finalizer; hex decoding has no pending output state. 
#### Returns
Always 0.

---

{#readnext}

#### readnext

`inline`

```cpp
inline bool readnext(const char * inbuf, size_t nread, size_t & rpos, char & c)
```

Defined in src/base/include/icy/hex.h:161

Reads the next non-whitespace character from inbuf, prepending any buffered lastbyte before consuming from the stream. 
#### Parameters
* `inbuf` Input buffer. 

* `nread` Total bytes in inbuf. 

* `rpos` Current read position; advanced on each consumed byte. 

* `c` Output: the next non-whitespace character. 

#### Returns
true if more input remains after c was read, false otherwise.

---

{#nybble}

#### nybble

`inline`

```cpp
inline int nybble(const int n)
```

Defined in src/base/include/icy/hex.h:181

Converts an ASCII hex character to its 4-bit integer value. 
#### Parameters
* `n` ASCII character ('0'-'9', 'a'-'f', 'A'-'F'). 

#### Returns
Integer value in the range [0, 15]. 

#### Exceptions
* `std::runtime_error` if n is not a valid hex character.

---

{#iswspace}

#### iswspace

`inline`

```cpp
inline bool iswspace(const char c)
```

Defined in src/base/include/icy/hex.h:196

Returns true if c is an ASCII whitespace character (space, CR, tab, LF). 
#### Parameters
* `c` Character to test. 

#### Returns
true if c is whitespace.

{#ipc}

# ipc

Classes for inter-process communication.

### Classes

| Name | Description |
|------|-------------|
| [`Queue`](#queue-1) | IPC queue is for safely passing templated actions between threads and processes. |
| [`SyncQueue`](#syncqueue-1) | IPC synchronization queue is for passing templated actions between threads and the event loop we are synchronizing with. |
| [`Action`](#action) | Default action type for executing synchronized callbacks. |

### Typedefs

| Return | Name | Description |
|--------|------|-------------|
| `ipc::Queue< ipc::Action >` | [`ActionQueue`](#actionqueue)  |  |
| `ipc::SyncQueue< ipc::Action >` | [`ActionSyncQueue`](#actionsyncqueue)  |  |

---

{#actionqueue}

#### ActionQueue

```cpp
using ActionQueue = ipc::Queue< ipc::Action >
```

---

{#actionsyncqueue}

#### ActionSyncQueue

```cpp
using ActionSyncQueue = ipc::SyncQueue< ipc::Action >
```

{#queue-1}

## Queue

```cpp
#include <icy/ipc.h>
```

```cpp
template<typename TAction = ipc::Action>
class Queue
```

Defined in src/base/include/icy/ipc.h:57

IPC queue is for safely passing templated actions between threads and processes.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Queue`](#queue-2) `inline` |  |
| `void` | [`push`](#push-1) `virtual` `inline` | Pushes an action onto the queue and triggers a post notification. Takes ownership of action; the queue deletes it after execution. Thread-safe. |
| `TAction *` | [`pop`](#pop) `virtual` `inline` | Removes and returns the next action from the front of the queue. The caller takes ownership of the returned pointer. Thread-safe. |
| `void` | [`runSync`](#runsync) `virtual` `inline` | Drains the queue by invoking and deleting every pending action in order. Must be called from the thread that owns the event loop. |
| `void` | [`close`](#close-17) `virtual` `inline` | Closes the underlying notification handle. No-op in the base implementation. |
| `void` | [`post`](#post) `virtual` `inline` | Signals the event loop that new actions are available. No-op in the base implementation. |
| `void` | [`waitForSync`](#waitforsync) `inline` | Blocks the calling thread until the queue is empty or the timeout elapses. Polls every 10 ms. Logs a warning if the timeout is reached. |

---

{#queue-2}

#### Queue

`inline`

```cpp
inline Queue()
```

Defined in src/base/include/icy/ipc.h:60

---

{#push-1}

#### push

`virtual` `inline`

```cpp
virtual inline void push(TAction * action)
```

Defined in src/base/include/icy/ipc.h:68

Pushes an action onto the queue and triggers a post notification. Takes ownership of action; the queue deletes it after execution. Thread-safe. 
#### Parameters
* `action` Heap-allocated action to enqueue.

---

{#pop}

#### pop

`virtual` `inline`

```cpp
virtual inline TAction * pop()
```

Defined in src/base/include/icy/ipc.h:81

Removes and returns the next action from the front of the queue. The caller takes ownership of the returned pointer. Thread-safe. 
#### Returns
Pointer to the next action, or nullptr if the queue is empty.

---

{#runsync}

#### runSync

`virtual` `inline`

```cpp
virtual inline void runSync()
```

Defined in src/base/include/icy/ipc.h:93

Drains the queue by invoking and deleting every pending action in order. Must be called from the thread that owns the event loop.

---

{#close-17}

#### close

`virtual` `inline`

```cpp
virtual inline void close()
```

Defined in src/base/include/icy/ipc.h:103

Closes the underlying notification handle. No-op in the base implementation.

---

{#post}

#### post

`virtual` `inline`

```cpp
virtual inline void post()
```

Defined in src/base/include/icy/ipc.h:106

Signals the event loop that new actions are available. No-op in the base implementation.

---

{#waitforsync}

#### waitForSync

`inline`

```cpp
inline void waitForSync(std::chrono::milliseconds timeout = std::chrono::milliseconds(5000))
```

Defined in src/base/include/icy/ipc.h:111

Blocks the calling thread until the queue is empty or the timeout elapses. Polls every 10 ms. Logs a warning if the timeout is reached. 
#### Parameters
* `timeout` Maximum time to wait (default: 5000 ms).

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::mutex` | [`_mutex`](#_mutex-7)  |  |
| `std::deque< TAction * >` | [`_actions`](#_actions)  |  |

---

{#_mutex-7}

#### _mutex

```cpp
std::mutex _mutex
```

Defined in src/base/include/icy/ipc.h:127

---

{#_actions}

#### _actions

```cpp
std::deque< TAction * > _actions
```

Defined in src/base/include/icy/ipc.h:128

{#syncqueue-1}

## SyncQueue

```cpp
#include <icy/ipc.h>
```

```cpp
template<typename TAction = ipc::Action>
class SyncQueue
```

Defined in src/base/include/icy/ipc.h:137

> **Inherits:** [`Action >`](#queue-1)

IPC synchronization queue is for passing templated actions between threads and the event loop we are synchronizing with.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`SyncQueue`](#syncqueue-2) `inline` | Constructs a [SyncQueue](#syncqueue-1) bound to the given libuv event loop. |
| `void` | [`close`](#close-18) `virtual` `inline` | Closes the underlying [Synchronizer](#synchronizer) handle and stops loop wakeups. |
| `void` | [`post`](#post-1) `virtual` `inline` | Wakes up the event loop so pending actions are dispatched via [runSync()](#runsync). |
| `Synchronizer &` | [`sync`](#sync) `virtual` `inline` | Returns a reference to the internal [Synchronizer](#synchronizer). |

---

{#syncqueue-2}

#### SyncQueue

`inline`

```cpp
inline SyncQueue(uv::Loop * loop = uv::defaultLoop())
```

Defined in src/base/include/icy/ipc.h:142

Constructs a [SyncQueue](#syncqueue-1) bound to the given libuv event loop. 
#### Parameters
* `loop` Event loop to synchronize with (default: the process-wide default loop).

---

{#close-18}

#### close

`virtual` `inline`

```cpp
virtual inline void close()
```

Defined in src/base/include/icy/ipc.h:150

Closes the underlying [Synchronizer](#synchronizer) handle and stops loop wakeups.

---

{#post-1}

#### post

`virtual` `inline`

```cpp
virtual inline void post()
```

Defined in src/base/include/icy/ipc.h:153

Wakes up the event loop so pending actions are dispatched via [runSync()](#runsync).

---

{#sync}

#### sync

`virtual` `inline`

```cpp
virtual inline Synchronizer & sync()
```

Defined in src/base/include/icy/ipc.h:157

Returns a reference to the internal [Synchronizer](#synchronizer). 
#### Returns
Reference to the [Synchronizer](#synchronizer) used for loop wakeup.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `Synchronizer` | [`_sync`](#_sync)  |  |

---

{#_sync}

#### _sync

```cpp
Synchronizer _sync
```

Defined in src/base/include/icy/ipc.h:160

{#action}

## Action

```cpp
#include <icy/ipc.h>
```

```cpp
struct Action
```

Defined in src/base/include/icy/ipc.h:33

Default action type for executing synchronized callbacks.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `Callback` | [`target`](#target)  | The callable to invoke when the action is dispatched. |
| `void *` | [`arg`](#arg)  | Optional opaque pointer passed to the callback. |
| `std::string` | [`data`](#data)  | Optional string payload passed to the callback. |

---

{#target}

#### target

```cpp
Callback target
```

Defined in src/base/include/icy/ipc.h:37

The callable to invoke when the action is dispatched.

---

{#arg}

#### arg

```cpp
void * arg
```

Defined in src/base/include/icy/ipc.h:38

Optional opaque pointer passed to the callback.

---

{#data}

#### data

```cpp
std::string data
```

Defined in src/base/include/icy/ipc.h:39

Optional string payload passed to the callback.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Action`](#action-1) `inline` | Constructs an [Action](#action) with the given callback, optional argument, and optional data. |

---

{#action-1}

#### Action

`inline`

```cpp
inline Action(Callback target, void * arg = nullptr, const std::string & data = "")
```

Defined in src/base/include/icy/ipc.h:45

Constructs an [Action](#action) with the given callback, optional argument, and optional data. 
#### Parameters
* `target` [Callback](#callback-1) to invoke on dispatch. 

* `arg` Opaque pointer passed to the callback (default: nullptr). 

* `data` String payload passed to the callback (default: empty).

### Public Types

| Name | Description |
|------|-------------|
| [`Callback`](#callback-1)  |  |

---

{#callback-1}

#### Callback

```cpp
using Callback = std::function< void(const Action &)>
```

Defined in src/base/include/icy/ipc.h:35

{#test}

# test

Modern unit testing framework.

### Classes

| Name | Description |
|------|-------------|
| [`Test`](#test-1) | [Test](#test-1) wrapper class. |
| [`FunctionTest`](#functiontest) | [Test](#test-1) wrapper for standalone test functions. |
| [`TestRunner`](#testrunner-1) | [Test](#test-1) manager queue. |

### Typedefs

| Return | Name | Description |
|--------|------|-------------|
| `std::list< Test * >` | [`TestList`](#testlist)  |  |
| `std::list< std::string >` | [`SErrorist`](#serrorist)  |  |
| `std::map< Test *, SErrorist >` | [`ErrorMap`](#errormap)  |  |

---

{#testlist}

#### TestList

```cpp
using TestList = std::list< Test * >
```

---

{#serrorist}

#### SErrorist

```cpp
using SErrorist = std::list< std::string >
```

---

{#errormap}

#### ErrorMap

```cpp
using ErrorMap = std::map< Test *, SErrorist >
```

### Functions

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`init`](#init-7)  | Initialize the test environment. |
| `int` | [`finalize`](#finalize-2)  | Finalize the test environment. |
| `void` | [`runAll`](#runall)  | Run all tests. |
| `void` | [`describe`](#describe)  | Describe a test environment implemented by the given lambda function. |
| `void` | [`describe`](#describe-1)  | Describe a test environment implemented by the given test instance. |
| `void` | [`expectImpl`](#expectimpl)  | Expect asserts that a condition is true (use [expect()](#test_8h_1a92645105a4c87ac01db7587df58caca6) as defined below). |
| `bool` | [`waitFor`](#waitfor)  | Run the event loop until a condition is met or timeout expires. Returns true if the condition was satisfied, false on timeout. Useful for testing async operations that complete via libuv callbacks. |

---

{#init-7}

#### init

```cpp
void init()
```

Initialize the test environment.

---

{#finalize-2}

#### finalize

```cpp
int finalize()
```

Finalize the test environment.

Destroy the [TestRunner](#testrunner-1) singleton instance and return the exit code.

---

{#runall}

#### runAll

```cpp
void runAll()
```

Run all tests.

---

{#describe}

#### describe

```cpp
void describe(const std::string & name, std::function< void()> target)
```

Describe a test environment implemented by the given lambda function.

---

{#describe-1}

#### describe

```cpp
void describe(const std::string & name, Test * test)
```

Describe a test environment implemented by the given test instance.

---

{#expectimpl}

#### expectImpl

```cpp
void expectImpl(bool passed, const char * assert, const char * file, long line)
```

Expect asserts that a condition is true (use [expect()](#test_8h_1a92645105a4c87ac01db7587df58caca6) as defined below).

---

{#waitfor}

#### waitFor

```cpp
bool waitFor(std::function< bool()> condition, int timeoutMs = 3000)
```

Run the event loop until a condition is met or timeout expires. Returns true if the condition was satisfied, false on timeout. Useful for testing async operations that complete via libuv callbacks.

{#test-1}

## Test

```cpp
#include <icy/test.h>
```

```cpp
class Test
```

Defined in src/base/include/icy/test.h:81

> **Subclassed by:** [`FunctionTest`](#functiontest)

[Test](#test-1) wrapper class.

This class is for implementing any kind of unit test that can be executed by a `[TestRunner](#testrunner-1)`.

### Friends

| Name | Description |
|------|-------------|
| [`TestRunner`](#testrunner)  | Tests belong to a [TestRunner](#testrunner-1) instance. |

---

{#testrunner}

#### TestRunner

```cpp
friend class TestRunner
```

Defined in src/base/include/icy/test.h:110

Tests belong to a [TestRunner](#testrunner-1) instance.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`name`](#name-4)  | The name of the test. |
| `SErrorist` | [`errors`](#errors)  | A list of test errors. |
| `double` | [`duration`](#duration-1)  | The test run duration for benchmarking. |

---

{#name-4}

#### name

```cpp
std::string name
```

Defined in src/base/include/icy/test.h:97

The name of the test.

---

{#errors}

#### errors

```cpp
SErrorist errors
```

Defined in src/base/include/icy/test.h:100

A list of test errors.

---

{#duration-1}

#### duration

```cpp
double duration
```

Defined in src/base/include/icy/test.h:103

The test run duration for benchmarking.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Test`](#test-2)  |  |
|  | [`~Test`](#test-3) `virtual` | Should remain protected. |
| `void` | [`run`](#run-1) `virtual` | Called by the [TestRunner](#testrunner-1) to run the test. |
| `bool` | [`passed`](#passed)  | Return true when the test passed without errors. |

---

{#test-2}

#### Test

```cpp
Test(const std::string & name = "Unnamed Test")
```

Defined in src/base/include/icy/test.h:85

#### Parameters
* `name` Human-readable name displayed in test output.

---

{#test-3}

#### ~Test

`virtual`

```cpp
virtual ~Test()
```

Defined in src/base/include/icy/test.h:88

Should remain protected.

---

{#run-1}

#### run

`virtual`

```cpp
virtual void run()
```

Defined in src/base/include/icy/test.h:91

Called by the [TestRunner](#testrunner-1) to run the test.

---

{#passed}

#### passed

```cpp
bool passed()
```

Defined in src/base/include/icy/test.h:94

Return true when the test passed without errors.

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Test`](#test-4)  | Deleted constructor. |

---

{#test-4}

#### Test

```cpp
Test(const Test & test) = delete
```

Defined in src/base/include/icy/test.h:106

Deleted constructor.

{#functiontest}

## FunctionTest

```cpp
#include <icy/test.h>
```

```cpp
class FunctionTest
```

Defined in src/base/include/icy/test.h:115

> **Inherits:** [`Test`](#test-1)

[Test](#test-1) wrapper for standalone test functions.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::function< void()>` | [`target`](#target-1)  |  |

---

{#target-1}

#### target

```cpp
std::function< void()> target
```

Defined in src/base/include/icy/test.h:118

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`FunctionTest`](#functiontest-1) `inline` |  |

---

{#functiontest-1}

#### FunctionTest

`inline`

```cpp
inline FunctionTest(std::function< void()> target, const std::string & name = "Unnamed Test")
```

Defined in src/base/include/icy/test.h:122

#### Parameters
* `target` Lambda or function to execute as the test body. 

* `name` Human-readable test name.

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`run`](#run-2) `virtual` `inline` `override` | Called by the [TestRunner](#testrunner-1) to run the test. |

---

{#run-2}

#### run

`virtual` `inline` `override`

```cpp
virtual inline void run() override
```

Defined in src/base/include/icy/test.h:132

Called by the [TestRunner](#testrunner-1) to run the test.

{#testrunner-1}

## TestRunner

```cpp
#include <icy/test.h>
```

```cpp
class TestRunner
```

Defined in src/base/include/icy/test.h:148

[Test](#test-1) manager queue.

The `[TestRunner](#testrunner-1)` is a queue in charge of running one or many tests.

When `[run()](#run-3)` the `[TestRunner](#testrunner-1)` loops through each test in the list calling the test's `[run()](#run-3)` method.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`TestRunner`](#testrunner-2)  |  |
| `void` | [`add`](#add)  | Adds a test to the runner and prints its name to stdout. |
| `Test *` | [`get`](#get-3) `const` | Return a pointer to the test matching the given name, or nullptr if no matching test exists. |
| `void` | [`run`](#run-3)  | Runs all registered tests sequentially, printing results to stdout. |
| `void` | [`clear`](#clear-1)  | Destroy and clears all managed tests. |
| `Test *` | [`current`](#current) `const` | Return the currently active [Test](#test-1) or nullptr. |
| `TestList` | [`tests`](#tests) `const` | Return the list of tests. |
| `ErrorMap` | [`errors`](#errors-1) `const` | Return a map of tests and errors. |
| `bool` | [`passed`](#passed-1) `const` | Return true if all tests passed. |

---

{#testrunner-2}

#### TestRunner

```cpp
TestRunner()
```

Defined in src/base/include/icy/test.h:151

---

{#add}

#### add

```cpp
void add(Test * test)
```

Defined in src/base/include/icy/test.h:156

Adds a test to the runner and prints its name to stdout. 
#### Parameters
* `test` Non-null pointer to the test; the runner takes ownership.

---

{#get-3}

#### get

`const`

```cpp
Test * get(std::string_view name) const
```

Defined in src/base/include/icy/test.h:162

Return a pointer to the test matching the given name, or nullptr if no matching test exists. 
#### Parameters
* `name` [Test](#test-1) name to search for. 

#### Returns
Matching test pointer or nullptr.

---

{#run-3}

#### run

```cpp
void run()
```

Defined in src/base/include/icy/test.h:165

Runs all registered tests sequentially, printing results to stdout.

---

{#clear-1}

#### clear

```cpp
void clear()
```

Defined in src/base/include/icy/test.h:168

Destroy and clears all managed tests.

---

{#current}

#### current

`const`

```cpp
Test * current() const
```

Defined in src/base/include/icy/test.h:171

Return the currently active [Test](#test-1) or nullptr.

---

{#tests}

#### tests

`const`

```cpp
TestList tests() const
```

Defined in src/base/include/icy/test.h:174

Return the list of tests.

---

{#errors-1}

#### errors

`const`

```cpp
ErrorMap errors() const
```

Defined in src/base/include/icy/test.h:177

Return a map of tests and errors.

---

{#passed-1}

#### passed

`const`

```cpp
bool passed() const
```

Defined in src/base/include/icy/test.h:180

Return true if all tests passed.

### Public Static Methods

| Return | Name | Description |
|--------|------|-------------|
| `TestRunner &` | [`getDefault`](#getdefault-1) `static` | Return the default `[TestRunner](#testrunner-1)` singleton, although `[TestRunner](#testrunner-1)` instances may also be initialized individually. |

---

{#getdefault-1}

#### getDefault

`static`

```cpp
static TestRunner & getDefault()
```

Defined in src/base/include/icy/test.h:184

Return the default `[TestRunner](#testrunner-1)` singleton, although `[TestRunner](#testrunner-1)` instances may also be initialized individually.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::mutex` | [`_mutex`](#_mutex-8)  |  |
| `TestList` | [`_tests`](#_tests)  |  |
| `Test *` | [`_current`](#_current)  |  |

---

{#_mutex-8}

#### _mutex

```cpp
std::mutex _mutex
```

Defined in src/base/include/icy/test.h:187

---

{#_tests}

#### _tests

```cpp
TestList _tests
```

Defined in src/base/include/icy/test.h:188

---

{#_current}

#### _current

```cpp
Test * _current
```

Defined in src/base/include/icy/test.h:189

{#time-3}

# time

Classes and functions for handling time.

### Functions

| Return | Name | Description |
|--------|------|-------------|
| `std::time_t` | [`now`](#now)  | Returns the current wall-clock time as a UTC time_t (seconds since epoch). |
| `double` | [`clockSecs`](#clocksecs)  | Returns the elapsed process time in decimal seconds using a monotonic clock. |
| `std::string` | [`print`](#print-7)  | Formats a broken-down time value using the given strftime format string. |
| `std::string` | [`printLocal`](#printlocal)  | Formats the current local time using the given strftime format string. |
| `std::string` | [`printUTC`](#printutc)  | Formats the current UTC time using the given strftime format string. |
| `std::tm` | [`toLocal`](#tolocal)  | Converts a time_t value to a broken-down local time structure. Uses thread-safe native functions (localtime_r / localtime_s). |
| `std::tm` | [`toUTC`](#toutc)  | Converts a time_t value to a broken-down UTC time structure. Uses thread-safe native functions (gmtime_r / gmtime_s). |
| `std::string` | [`getLocal`](#getlocal)  | Returns the current local time as an ISO8601 formatted string. |
| `std::string` | [`getUTC`](#getutc)  | Returns the current UTC time as an ISO8601 formatted string. |
| `uint64_t` | [`hrtime`](#hrtime)  | Returns the current high-resolution monotonic time in nanoseconds. |

---

{#now}

#### now

```cpp
std::time_t now()
```

Returns the current wall-clock time as a UTC time_t (seconds since epoch). 
#### Returns
Current UTC time in seconds since the Unix epoch.

---

{#clocksecs}

#### clockSecs

```cpp
double clockSecs()
```

Returns the elapsed process time in decimal seconds using a monotonic clock. 
#### Returns
[Process](#process) time in seconds.

---

{#print-7}

#### print

```cpp
std::string print(const std::tm & dt, const char * fmt = ISO8601Format)
```

Formats a broken-down time value using the given strftime format string. 
#### Parameters
* `dt` Broken-down time to format. 

* `fmt` strftime format string (default: ISO8601Format). 

#### Returns
Formatted time string.

---

{#printlocal}

#### printLocal

```cpp
std::string printLocal(const char * fmt = ISO8601Format)
```

Formats the current local time using the given strftime format string. 
#### Parameters
* `fmt` strftime format string (default: ISO8601Format). 

#### Returns
Formatted local time string.

---

{#printutc}

#### printUTC

```cpp
std::string printUTC(const char * fmt = ISO8601Format)
```

Formats the current UTC time using the given strftime format string. 
#### Parameters
* `fmt` strftime format string (default: ISO8601Format). 

#### Returns
Formatted UTC time string.

---

{#tolocal}

#### toLocal

```cpp
std::tm toLocal(const std::time_t & time)
```

Converts a time_t value to a broken-down local time structure. Uses thread-safe native functions (localtime_r / localtime_s). 
#### Parameters
* `time` UTC time value to convert. 

#### Returns
Broken-down local time.

---

{#toutc}

#### toUTC

```cpp
std::tm toUTC(const std::time_t & time)
```

Converts a time_t value to a broken-down UTC time structure. Uses thread-safe native functions (gmtime_r / gmtime_s). 
#### Parameters
* `time` UTC time value to convert. 

#### Returns
Broken-down UTC time.

---

{#getlocal}

#### getLocal

```cpp
std::string getLocal()
```

Returns the current local time as an ISO8601 formatted string. 
#### Returns
ISO8601 local time string.

---

{#getutc}

#### getUTC

```cpp
std::string getUTC()
```

Returns the current UTC time as an ISO8601 formatted string. 
#### Returns
ISO8601 UTC time string.

---

{#hrtime}

#### hrtime

```cpp
uint64_t hrtime()
```

Returns the current high-resolution monotonic time in nanoseconds. 
#### Returns
Current time in nanoseconds (suitable for measuring intervals).

### Variables

| Return | Name | Description |
|--------|------|-------------|
| `const int64_t` | [`kNumMillisecsPerSec`](#knummillisecspersec) `static` | Constants for calculating time. |
| `const int64_t` | [`kNumMicrosecsPerSec`](#knummicrosecspersec) `static` |  |
| `const int64_t` | [`kNumNanosecsPerSec`](#knumnanosecspersec) `static` |  |
| `const int64_t` | [`kNumMicrosecsPerMillisec`](#knummicrosecspermillisec) `static` |  |
| `const int64_t` | [`kNumNanosecsPerMillisec`](#knumnanosecspermillisec) `static` |  |
| `const int64_t` | [`kNumNanosecsPerMicrosec`](#knumnanosecspermicrosec) `static` |  |
| `const char *` | [`ISO8601Format`](#iso8601format) `static` | The date/time format defined in the ISO 8601 standard. This is the default format used throughout the library for consistency. |

---

{#knummillisecspersec}

#### kNumMillisecsPerSec

`static`

```cpp
const int64_t kNumMillisecsPerSec = (1000)
```

Constants for calculating time.

---

{#knummicrosecspersec}

#### kNumMicrosecsPerSec

`static`

```cpp
const int64_t kNumMicrosecsPerSec = (1000000)
```

---

{#knumnanosecspersec}

#### kNumNanosecsPerSec

`static`

```cpp
const int64_t kNumNanosecsPerSec = (1000000000)
```

---

{#knummicrosecspermillisec}

#### kNumMicrosecsPerMillisec

`static`

```cpp
const int64_t kNumMicrosecsPerMillisec = kNumMicrosecsPerSec / kNumMillisecsPerSec
```

---

{#knumnanosecspermillisec}

#### kNumNanosecsPerMillisec

`static`

```cpp
const int64_t kNumNanosecsPerMillisec = kNumNanosecsPerSec / kNumMillisecsPerSec
```

---

{#knumnanosecspermicrosec}

#### kNumNanosecsPerMicrosec

`static`

```cpp
const int64_t kNumNanosecsPerMicrosec = kNumNanosecsPerSec / kNumMicrosecsPerSec
```

---

{#iso8601format}

#### ISO8601Format

`static`

```cpp
const char * ISO8601Format = "%Y-%m-%dT%H:%M:%SZ"
```

The date/time format defined in the ISO 8601 standard. This is the default format used throughout the library for consistency.

Examples: 2005-01-01T12:00:00+01:00 2005-01-01T11:00:00Z

{#base64}

# base64

Base64 encoding and decoding helpers.

### Classes

| Name | Description |
|------|-------------|
| [`Encoder`](#encoder-3) | Base64 encoder. |
| [`Decoder`](#decoder-2) | Base64 decoder. |

### Functions

| Return | Name | Description |
|--------|------|-------------|
| `size_t` | [`encodedBufferCapacity`](#encodedbuffercapacity) `inline` | Returns a safe temporary buffer size for encoding up to `inputSize` bytes. Includes padding/newline slack so callers can reuse the same buffer for finalize(). |
| `std::string` | [`encode`](#encode-16) `inline` | Encodes an STL byte container to a Base64 string. |
| `std::string` | [`decode`](#decode-5) `inline` | Decodes a Base64-encoded STL container to a binary string. |

---

{#encodedbuffercapacity}

#### encodedBufferCapacity

`inline`

```cpp
inline size_t encodedBufferCapacity(size_t inputSize, int lineLength = LINE_LENGTH)
```

Returns a safe temporary buffer size for encoding up to `inputSize` bytes. Includes padding/newline slack so callers can reuse the same buffer for finalize().

---

{#encode-16}

#### encode

`inline`

```cpp
template<typename T> inline std::string encode(const T & bytes, int lineLength = LINE_LENGTH)
```

Encodes an STL byte container to a Base64 string. 
#### Parameters
* `T` Container type with a `size()` method and contiguous `operator[]`. 

#### Parameters
* `bytes` Input data container. 

* `lineLength` Characters per line in the output (0 disables wrapping). 

#### Returns
Base64-encoded string.

---

{#decode-5}

#### decode

`inline`

```cpp
template<typename T> inline std::string decode(const T & bytes)
```

Decodes a Base64-encoded STL container to a binary string. 
#### Parameters
* `T` Container type with a `size()` method and contiguous `operator[]`. 

#### Parameters
* `bytes` Input Base64 data container. 

#### Returns
Decoded binary string.

### Variables

| Return | Name | Description |
|--------|------|-------------|
| `int` | [`BUFFER_SIZE`](#buffer_size) `constexpr` |  |
| `int` | [`LINE_LENGTH`](#line_length) `constexpr` |  |

---

{#buffer_size}

#### BUFFER_SIZE

`constexpr`

```cpp
int BUFFER_SIZE = 16384
```

---

{#line_length}

#### LINE_LENGTH

`constexpr`

```cpp
int LINE_LENGTH = 72
```

{#encoder-3}

## Encoder

```cpp
#include <icy/base64.h>
```

```cpp
struct Encoder
```

Defined in src/base/include/icy/base64.h:102

> **Inherits:** [`Encoder`](#encoder-5)

Base64 encoder.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `internal::encodestate` | [`_state`](#_state)  |  |
| `int` | [`_buffersize`](#_buffersize)  |  |

---

{#_state}

#### _state

```cpp
internal::encodestate _state
```

Defined in src/base/include/icy/base64.h:178

---

{#_buffersize}

#### _buffersize

```cpp
int _buffersize
```

Defined in src/base/include/icy/base64.h:179

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Encoder`](#encoder-4) `inline` |  |
| `void` | [`encode`](#encode-17) `inline` | Encodes the entire input stream and writes Base64 output to `ostrm`. Resets the encoder state after completion. |
| `void` | [`encode`](#encode-18) `inline` | Encodes a string to Base64 and appends the result to `out`. Resets the encoder state after completion. |
| `ssize_t` | [`encode`](#encode-19) `virtual` `inline` `override` | Encodes a raw buffer, writing Base64 characters to `outbuf`. May be called multiple times before calling `[finalize()](#finalize-3)`. |
| `ssize_t` | [`finalize`](#finalize-3) `virtual` `inline` `override` | Writes any pending padding and resets the encoder state. Must be called once after all `[encode()](#encode-17)` calls to flush the final block. |
| `void` | [`setLineLength`](#setlinelength-1) `inline` | Sets the line wrap length for encoded output (0 disables line wrapping). |

---

{#encoder-4}

#### Encoder

`inline`

```cpp
inline Encoder(int buffersize = BUFFER_SIZE)
```

Defined in src/base/include/icy/base64.h:105

#### Parameters
* `buffersize` Internal read buffer size in bytes.

---

{#encode-17}

#### encode

`inline`

```cpp
inline void encode(std::istream & istrm, std::ostream & ostrm)
```

Defined in src/base/include/icy/base64.h:115

Encodes the entire input stream and writes Base64 output to `ostrm`. Resets the encoder state after completion. 
#### Parameters
* `istrm` Source stream to encode. 

* `ostrm` Destination stream for Base64 output.

---

{#encode-18}

#### encode

`inline`

```cpp
inline void encode(const std::string & in, std::string & out)
```

Defined in src/base/include/icy/base64.h:141

Encodes a string to Base64 and appends the result to `out`. Resets the encoder state after completion. 
#### Parameters
* `in` Input string. 

* `out` Output string to which Base64 characters are appended.

---

{#encode-19}

#### encode

`virtual` `inline` `override`

```cpp
virtual inline ssize_t encode(const char * inbuf, size_t nread, char * outbuf) override
```

Defined in src/base/include/icy/base64.h:160

Encodes a raw buffer, writing Base64 characters to `outbuf`. May be called multiple times before calling `[finalize()](#finalize-3)`. 
#### Parameters
* `inbuf` Input binary data. 

* `nread` Number of bytes to encode. 

* `outbuf` Output buffer; must be at least `nread * 4 / 3 + 4` bytes. 

#### Returns
Number of Base64 characters written.

---

{#finalize-3}

#### finalize

`virtual` `inline` `override`

```cpp
virtual inline ssize_t finalize(char * outbuf) override
```

Defined in src/base/include/icy/base64.h:169

Writes any pending padding and resets the encoder state. Must be called once after all `[encode()](#encode-17)` calls to flush the final block. 
#### Parameters
* `outbuf` Output buffer; must be at least 5 bytes. 

#### Returns
Number of characters written.

---

{#setlinelength-1}

#### setLineLength

`inline`

```cpp
inline void setLineLength(int lineLength)
```

Defined in src/base/include/icy/base64.h:176

Sets the line wrap length for encoded output (0 disables line wrapping). 
#### Parameters
* `lineLength` Characters per line; use 0 to disable.

{#decoder-2}

## Decoder

```cpp
#include <icy/base64.h>
```

```cpp
struct Decoder
```

Defined in src/base/include/icy/base64.h:258

> **Inherits:** [`Decoder`](#decoder-4)

Base64 decoder.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `internal::decodestate` | [`_state`](#_state-1)  |  |
| `int` | [`_buffersize`](#_buffersize-1)  |  |

---

{#_state-1}

#### _state

```cpp
internal::decodestate _state
```

Defined in src/base/include/icy/base64.h:304

---

{#_buffersize-1}

#### _buffersize

```cpp
int _buffersize
```

Defined in src/base/include/icy/base64.h:305

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Decoder`](#decoder-3) `inline` |  |
| `ssize_t` | [`decode`](#decode-6) `inline` | Decodes a single Base64 character to its 6-bit value. |
| `ssize_t` | [`decode`](#decode-7) `virtual` `inline` `override` | Decodes a raw Base64 buffer into binary data. |
| `void` | [`decode`](#decode-8) `inline` | Decodes the entire input stream and writes binary output to `ostrm`. Resets the decoder state after completion. |

---

{#decoder-3}

#### Decoder

`inline`

```cpp
inline Decoder(int buffersize = BUFFER_SIZE)
```

Defined in src/base/include/icy/base64.h:261

#### Parameters
* `buffersize` Internal read buffer size in bytes.

---

{#decode-6}

#### decode

`inline`

```cpp
inline ssize_t decode(char value_in)
```

Defined in src/base/include/icy/base64.h:270

Decodes a single Base64 character to its 6-bit value. 
#### Parameters
* `value_in` Base64 character. 

#### Returns
Decoded 6-bit value, or a negative sentinel on invalid input.

---

{#decode-7}

#### decode

`virtual` `inline` `override`

```cpp
virtual inline ssize_t decode(const char * inbuf, size_t nread, char * outbuf) override
```

Defined in src/base/include/icy/base64.h:277

Decodes a raw Base64 buffer into binary data. 
#### Parameters
* `inbuf` Input Base64 characters. 

* `nread` Number of characters to decode. 

* `outbuf` Output buffer; must be at least `nread * 3 / 4` bytes. 

#### Returns
Number of binary bytes written.

---

{#decode-8}

#### decode

`inline`

```cpp
inline void decode(std::istream & istrm, std::ostream & ostrm)
```

Defined in src/base/include/icy/base64.h:286

Decodes the entire input stream and writes binary output to `ostrm`. Resets the decoder state after completion. 
#### Parameters
* `istrm` Source stream of Base64 data. 

* `ostrm` Destination stream for decoded binary output.

{#deleter-2}

# deleter

Deleter helpers for objects managed through custom destruction routines.

### Classes

| Name | Description |
|------|-------------|
| [`Dispose`](#dispose-1) | Deleter functor that calls dispose() on the pointer. Useful with std::unique_ptr for objects that require a dispose() call rather than direct deletion. |
| [`Array`](#array) | Deleter functor for array pointers. Calls delete[] on the pointer. |

{#dispose-1}

## Dispose

```cpp
#include <icy/memory.h>
```

```cpp
template<class T>
struct Dispose
```

Defined in src/base/include/icy/memory.h:220

Deleter functor that calls dispose() on the pointer. Useful with std::unique_ptr for objects that require a dispose() call rather than direct deletion.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`operator()`](#operator-11) `inline` | Calls ptr->dispose() if ptr is non-null. |

---

{#operator-11}

#### operator()

`inline`

```cpp
inline void operator()(T * ptr)
```

Defined in src/base/include/icy/memory.h:224

Calls ptr->dispose() if ptr is non-null. 
#### Parameters
* `ptr` Pointer to dispose; may be nullptr.

{#array}

## Array

```cpp
#include <icy/memory.h>
```

```cpp
template<class T>
struct Array
```

Defined in src/base/include/icy/memory.h:235

Deleter functor for array pointers. Calls delete[] on the pointer.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`operator()`](#operator-12) `inline` | Calls delete[] on ptr if non-null. |

---

{#operator-12}

#### operator()

`inline`

```cpp
inline void operator()(T * ptr)
```

Defined in src/base/include/icy/memory.h:239

Calls delete[] on ptr if non-null. 
#### Parameters
* `ptr` [Array](#array) pointer to delete; may be nullptr.

{#numeric}

# numeric

Integer parsing and formatting helpers.

### Functions

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`format`](#format-8)  | Formats an integer value in decimal notation. |
| `void` | [`format`](#format-9)  | Formats an integer value in decimal notation, right justified in a field having at least the specified width. |
| `void` | [`format0`](#format0)  | Formats an integer value in decimal notation, right justified and zero-padded in a field having at least the specified width. |
| `void` | [`formatHex`](#formathex)  | Formats an int value in hexadecimal notation. The value is treated as unsigned. |
| `void` | [`formatHex`](#formathex-1)  | Formats an int value in hexadecimal notation, right justified and zero-padded in a field having at least the specified width. The value is treated as unsigned. |
| `void` | [`format`](#format-10)  | Formats an unsigned int value in decimal notation. |
| `void` | [`format`](#format-11)  | Formats an unsigned int value in decimal notation, right justified in a field having at least the specified width. |
| `void` | [`format0`](#format0-1)  | Formats an unsigned int value in decimal notation, right justified and zero-padded in a field having at least the specified width. |
| `void` | [`formatHex`](#formathex-2)  | Formats an unsigned int value in hexadecimal notation. |
| `void` | [`formatHex`](#formathex-3)  | Formats an unsigned int value in hexadecimal notation, right justified and zero-padded in a field having at least the specified width. |
| `void` | [`format`](#format-12)  | Formats a long value in decimal notation. |
| `void` | [`format`](#format-13)  | Formats a long value in decimal notation, right justified in a field having at least the specified width. |
| `void` | [`format0`](#format0-2)  | Formats a long value in decimal notation, right justified and zero-padded in a field having at least the specified width. |
| `void` | [`formatHex`](#formathex-4)  | Formats a long value in hexadecimal notation. The value is treated as unsigned. |
| `void` | [`formatHex`](#formathex-5)  | Formats a long value in hexadecimal notation, right justified and zero-padded in a field having at least the specified width. The value is treated as unsigned. |
| `void` | [`format`](#format-14)  | Formats an unsigned long value in decimal notation. |
| `void` | [`format`](#format-15)  | Formats an unsigned long value in decimal notation, right justified in a field having at least the specified width. |
| `void` | [`format0`](#format0-3)  | Formats an unsigned long value in decimal notation, right justified and zero-padded in a field having at least the specified width. |
| `void` | [`formatHex`](#formathex-6)  | Formats an unsigned long value in hexadecimal notation. |
| `void` | [`formatHex`](#formathex-7)  | Formats an unsigned long value in hexadecimal notation, right justified and zero-padded in a field having at least the specified width. |
| `void` | [`format`](#format-16)  | Formats a 64-bit integer value in decimal notation. |
| `void` | [`format`](#format-17)  | Formats a 64-bit integer value in decimal notation, right justified in a field having at least the specified width. |
| `void` | [`format0`](#format0-4)  | Formats a 64-bit integer value in decimal notation, right justified and zero-padded in a field having at least the specified width. |
| `void` | [`formatHex`](#formathex-8)  | Formats a 64-bit integer value in hexadecimal notation. The value is treated as unsigned. |
| `void` | [`formatHex`](#formathex-9)  | Formats a 64-bit integer value in hexadecimal notation, right justified and zero-padded in a field having at least the specified width. The value is treated as unsigned. |
| `void` | [`format`](#format-18)  | Formats an unsigned 64-bit integer value in decimal notation. |
| `void` | [`format`](#format-19)  | Formats an unsigned 64-bit integer value in decimal notation, right justified in a field having at least the specified width. |
| `void` | [`format0`](#format0-5)  | Formats an unsigned 64-bit integer value in decimal notation, right justified and zero-padded in a field having at least the specified width. |
| `void` | [`formatHex`](#formathex-10)  | Formats an unsigned 64-bit integer value in hexadecimal notation. |
| `void` | [`formatHex`](#formathex-11)  | Formats an unsigned 64-bit integer value in hexadecimal notation, right justified and zero-padded in a field having at least the specified width. |

---

{#format-8}

#### format

```cpp
void format(std::string & str, int value)
```

Formats an integer value in decimal notation. 
#### Parameters
* `str` Output string to append to. 

* `value` Value to format.

---

{#format-9}

#### format

```cpp
void format(std::string & str, int value, int width)
```

Formats an integer value in decimal notation, right justified in a field having at least the specified width. 
#### Parameters
* `str` Output string to append to. 

* `value` Value to format. 

* `width` Minimum field width; padded with spaces on the left.

---

{#format0}

#### format0

```cpp
void format0(std::string & str, int value, int width)
```

Formats an integer value in decimal notation, right justified and zero-padded in a field having at least the specified width. 
#### Parameters
* `str` Output string to append to. 

* `value` Value to format. 

* `width` Minimum field width; padded with zeros on the left.

---

{#formathex}

#### formatHex

```cpp
void formatHex(std::string & str, int value)
```

Formats an int value in hexadecimal notation. The value is treated as unsigned. 
#### Parameters
* `str` Output string to append to. 

* `value` Value to format.

---

{#formathex-1}

#### formatHex

```cpp
void formatHex(std::string & str, int value, int width)
```

Formats an int value in hexadecimal notation, right justified and zero-padded in a field having at least the specified width. The value is treated as unsigned. 
#### Parameters
* `str` Output string to append to. 

* `value` Value to format. 

* `width` Minimum field width; padded with zeros on the left.

---

{#format-10}

#### format

```cpp
void format(std::string & str, unsigned value)
```

Formats an unsigned int value in decimal notation. 
#### Parameters
* `str` Output string to append to. 

* `value` Value to format.

---

{#format-11}

#### format

```cpp
void format(std::string & str, unsigned value, int width)
```

Formats an unsigned int value in decimal notation, right justified in a field having at least the specified width. 
#### Parameters
* `str` Output string to append to. 

* `value` Value to format. 

* `width` Minimum field width; padded with spaces on the left.

---

{#format0-1}

#### format0

```cpp
void format0(std::string & str, unsigned int value, int width)
```

Formats an unsigned int value in decimal notation, right justified and zero-padded in a field having at least the specified width. 
#### Parameters
* `str` Output string to append to. 

* `value` Value to format. 

* `width` Minimum field width; padded with zeros on the left.

---

{#formathex-2}

#### formatHex

```cpp
void formatHex(std::string & str, unsigned value)
```

Formats an unsigned int value in hexadecimal notation. 
#### Parameters
* `str` Output string to append to. 

* `value` Value to format.

---

{#formathex-3}

#### formatHex

```cpp
void formatHex(std::string & str, unsigned value, int width)
```

Formats an unsigned int value in hexadecimal notation, right justified and zero-padded in a field having at least the specified width. 
#### Parameters
* `str` Output string to append to. 

* `value` Value to format. 

* `width` Minimum field width; padded with zeros on the left.

---

{#format-12}

#### format

```cpp
void format(std::string & str, long value)
```

Formats a long value in decimal notation. 
#### Parameters
* `str` Output string to append to. 

* `value` Value to format.

---

{#format-13}

#### format

```cpp
void format(std::string & str, long value, int width)
```

Formats a long value in decimal notation, right justified in a field having at least the specified width. 
#### Parameters
* `str` Output string to append to. 

* `value` Value to format. 

* `width` Minimum field width; padded with spaces on the left.

---

{#format0-2}

#### format0

```cpp
void format0(std::string & str, long value, int width)
```

Formats a long value in decimal notation, right justified and zero-padded in a field having at least the specified width. 
#### Parameters
* `str` Output string to append to. 

* `value` Value to format. 

* `width` Minimum field width; padded with zeros on the left.

---

{#formathex-4}

#### formatHex

```cpp
void formatHex(std::string & str, long value)
```

Formats a long value in hexadecimal notation. The value is treated as unsigned. 
#### Parameters
* `str` Output string to append to. 

* `value` Value to format.

---

{#formathex-5}

#### formatHex

```cpp
void formatHex(std::string & str, long value, int width)
```

Formats a long value in hexadecimal notation, right justified and zero-padded in a field having at least the specified width. The value is treated as unsigned. 
#### Parameters
* `str` Output string to append to. 

* `value` Value to format. 

* `width` Minimum field width; padded with zeros on the left.

---

{#format-14}

#### format

```cpp
void format(std::string & str, unsigned long value)
```

Formats an unsigned long value in decimal notation. 
#### Parameters
* `str` Output string to append to. 

* `value` Value to format.

---

{#format-15}

#### format

```cpp
void format(std::string & str, unsigned long value, int width)
```

Formats an unsigned long value in decimal notation, right justified in a field having at least the specified width. 
#### Parameters
* `str` Output string to append to. 

* `value` Value to format. 

* `width` Minimum field width; padded with spaces on the left.

---

{#format0-3}

#### format0

```cpp
void format0(std::string & str, unsigned long value, int width)
```

Formats an unsigned long value in decimal notation, right justified and zero-padded in a field having at least the specified width. 
#### Parameters
* `str` Output string to append to. 

* `value` Value to format. 

* `width` Minimum field width; padded with zeros on the left.

---

{#formathex-6}

#### formatHex

```cpp
void formatHex(std::string & str, unsigned long value)
```

Formats an unsigned long value in hexadecimal notation. 
#### Parameters
* `str` Output string to append to. 

* `value` Value to format.

---

{#formathex-7}

#### formatHex

```cpp
void formatHex(std::string & str, unsigned long value, int width)
```

Formats an unsigned long value in hexadecimal notation, right justified and zero-padded in a field having at least the specified width. 
#### Parameters
* `str` Output string to append to. 

* `value` Value to format. 

* `width` Minimum field width; padded with zeros on the left.

---

{#format-16}

#### format

```cpp
void format(std::string & str, std::int64_t value)
```

Formats a 64-bit integer value in decimal notation. 
#### Parameters
* `str` Output string to append to. 

* `value` Value to format.

---

{#format-17}

#### format

```cpp
void format(std::string & str, std::int64_t value, int width)
```

Formats a 64-bit integer value in decimal notation, right justified in a field having at least the specified width. 
#### Parameters
* `str` Output string to append to. 

* `value` Value to format. 

* `width` Minimum field width; padded with spaces on the left.

---

{#format0-4}

#### format0

```cpp
void format0(std::string & str, std::int64_t value, int width)
```

Formats a 64-bit integer value in decimal notation, right justified and zero-padded in a field having at least the specified width. 
#### Parameters
* `str` Output string to append to. 

* `value` Value to format. 

* `width` Minimum field width; padded with zeros on the left.

---

{#formathex-8}

#### formatHex

```cpp
void formatHex(std::string & str, std::int64_t value)
```

Formats a 64-bit integer value in hexadecimal notation. The value is treated as unsigned. 
#### Parameters
* `str` Output string to append to. 

* `value` Value to format.

---

{#formathex-9}

#### formatHex

```cpp
void formatHex(std::string & str, std::int64_t value, int width)
```

Formats a 64-bit integer value in hexadecimal notation, right justified and zero-padded in a field having at least the specified width. The value is treated as unsigned. 
#### Parameters
* `str` Output string to append to. 

* `value` Value to format. 

* `width` Minimum field width; padded with zeros on the left.

---

{#format-18}

#### format

```cpp
void format(std::string & str, uint64_t value)
```

Formats an unsigned 64-bit integer value in decimal notation. 
#### Parameters
* `str` Output string to append to. 

* `value` Value to format.

---

{#format-19}

#### format

```cpp
void format(std::string & str, uint64_t value, int width)
```

Formats an unsigned 64-bit integer value in decimal notation, right justified in a field having at least the specified width. 
#### Parameters
* `str` Output string to append to. 

* `value` Value to format. 

* `width` Minimum field width; padded with spaces on the left.

---

{#format0-5}

#### format0

```cpp
void format0(std::string & str, uint64_t value, int width)
```

Formats an unsigned 64-bit integer value in decimal notation, right justified and zero-padded in a field having at least the specified width. 
#### Parameters
* `str` Output string to append to. 

* `value` Value to format. 

* `width` Minimum field width; padded with zeros on the left.

---

{#formathex-10}

#### formatHex

```cpp
void formatHex(std::string & str, uint64_t value)
```

Formats an unsigned 64-bit integer value in hexadecimal notation. 
#### Parameters
* `str` Output string to append to. 

* `value` Value to format.

---

{#formathex-11}

#### formatHex

```cpp
void formatHex(std::string & str, uint64_t value, int width)
```

Formats an unsigned 64-bit integer value in hexadecimal notation, right justified and zero-padded in a field having at least the specified width. 
#### Parameters
* `str` Output string to append to. 

* `value` Value to format. 

* `width` Minimum field width; padded with zeros on the left.

{#basic}

# basic

Interface classes.

### Classes

| Name | Description |
|------|-------------|
| [`Decoder`](#decoder-4) | Abstract interface for stream decoders. |
| [`Encoder`](#encoder-5) | Abstract interface for stream encoders. |
| [`Runnable`](#runnable) | Abstract interface for classes that can be run and cancelled. |
| [`Startable`](#startable) | Abstract interface for a classes that can be started and stopped. |
| [`Sendable`](#sendable) | Abstract interface for classes that can be sent and cancelled. |

{#decoder-4}

## Decoder

```cpp
#include <icy/interface.h>
```

```cpp
class Decoder
```

Defined in src/base/include/icy/interface.h:33

> **Subclassed by:** [`Decoder`](#decoder-2), [`Decoder`](#decoder)

Abstract interface for stream decoders.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Decoder`](#decoder-5)  | Defaulted constructor. |
| `ssize_t` | [`decode`](#decode-9) `virtual` | Decodes nread bytes from inbuf and writes decoded output to outbuf. |
| `ssize_t` | [`finalize`](#finalize-4) `virtual` `inline` | Flushes any buffered state and writes final output to outbuf. |

---

{#decoder-5}

#### Decoder

```cpp
Decoder() = default
```

Defined in src/base/include/icy/interface.h:36

Defaulted constructor.

---

{#decode-9}

#### decode

`virtual`

```cpp
virtual ssize_t decode(const char * inbuf, size_t nread, char * outbuf)
```

Defined in src/base/include/icy/interface.h:44

Decodes nread bytes from inbuf and writes decoded output to outbuf. 
#### Parameters
* `inbuf` Encoded input buffer. 

* `nread` Number of bytes to decode from inbuf. 

* `outbuf` Destination buffer for decoded output. 

#### Returns
Number of bytes written to outbuf, or -1 on error.

---

{#finalize-4}

#### finalize

`virtual` `inline`

```cpp
virtual inline ssize_t finalize(char * outbuf)
```

Defined in src/base/include/icy/interface.h:49

Flushes any buffered state and writes final output to outbuf. 
#### Parameters
* `outbuf` Destination buffer for any remaining output. 

#### Returns
Number of bytes written, or 0 if nothing to flush.

{#encoder-5}

## Encoder

```cpp
#include <icy/interface.h>
```

```cpp
class Encoder
```

Defined in src/base/include/icy/interface.h:54

> **Subclassed by:** [`Encoder`](#encoder-3), [`Encoder`](#encoder-1)

Abstract interface for stream encoders.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Encoder`](#encoder-6)  | Defaulted constructor. |
| `ssize_t` | [`encode`](#encode-20) `virtual` | Encodes nread bytes from inbuf and writes encoded output to outbuf. |
| `ssize_t` | [`finalize`](#finalize-5) `virtual` `inline` | Flushes any buffered state and writes final output to outbuf. |

---

{#encoder-6}

#### Encoder

```cpp
Encoder() = default
```

Defined in src/base/include/icy/interface.h:57

Defaulted constructor.

---

{#encode-20}

#### encode

`virtual`

```cpp
virtual ssize_t encode(const char * inbuf, size_t nread, char * outbuf)
```

Defined in src/base/include/icy/interface.h:65

Encodes nread bytes from inbuf and writes encoded output to outbuf. 
#### Parameters
* `inbuf` Raw input buffer to encode. 

* `nread` Number of bytes to encode from inbuf. 

* `outbuf` Destination buffer for encoded output. 

#### Returns
Number of bytes written to outbuf, or -1 on error.

---

{#finalize-5}

#### finalize

`virtual` `inline`

```cpp
virtual inline ssize_t finalize(char * outbuf)
```

Defined in src/base/include/icy/interface.h:70

Flushes any buffered state and writes final output to outbuf. 
#### Parameters
* `outbuf` Destination buffer for any remaining output. 

#### Returns
Number of bytes written, or 0 if nothing to flush.

{#runnable}

## Runnable

```cpp
#include <icy/interface.h>
```

```cpp
class Runnable
```

Defined in src/base/include/icy/interface.h:75

> **Subclassed by:** [`RunnableQueue< IPacket >`](#runnablequeue), [`PlanarAudioPacket >`](#runnablequeue), [`RunnableQueue< VisionFramePacket >`](#runnablequeue), [`AsyncDiagnostic`](#asyncdiagnostic), [`AsyncLogWriter`](#asynclogwriter), [`RunnableQueue< T >`](#runnablequeue), [`Task`](#task), [`TaskRunner`](#taskrunner), [`MediaCapture`](av.md#mediacapture), [`InstallTask`](pacm.md#installtask)

Abstract interface for classes that can be run and cancelled.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Runnable`](#runnable-1) `inline` |  |
| `void` | [`run`](#run-4) `virtual` | The run method will be called by the asynchronous context. |
| `void` | [`cancel`](#cancel-1) `virtual` `inline` | Cancel the current task. The [run()](#run-4) method should return ASAP. |
| `bool` | [`cancelled`](#cancelled) `virtual` `const` `inline` | Returns true when the task has been cancelled. |

---

{#runnable-1}

#### Runnable

`inline`

```cpp
inline Runnable()
```

Defined in src/base/include/icy/interface.h:78

---

{#run-4}

#### run

`virtual`

```cpp
virtual void run()
```

Defined in src/base/include/icy/interface.h:86

The run method will be called by the asynchronous context.

---

{#cancel-1}

#### cancel

`virtual` `inline`

```cpp
virtual inline void cancel(bool flag = true)
```

Defined in src/base/include/icy/interface.h:90

Cancel the current task. The [run()](#run-4) method should return ASAP.

---

{#cancelled}

#### cancelled

`virtual` `const` `inline`

```cpp
virtual inline bool cancelled() const
```

Defined in src/base/include/icy/interface.h:96

Returns true when the task has been cancelled.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::atomic< bool >` | [`exit`](#exit)  |  |

---

{#exit}

#### exit

```cpp
std::atomic< bool > exit
```

Defined in src/base/include/icy/interface.h:102

{#startable}

## Startable

```cpp
#include <icy/interface.h>
```

```cpp
class Startable
```

Defined in src/base/include/icy/interface.h:107

> **Subclassed by:** [`ThreadedStreamReader`](#threadedstreamreader), [`ICapture`](av.md#icapture), [`FormWriter`](http.md#formwriter)

Abstract interface for a classes that can be started and stopped.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`start`](#start-7) `virtual` | Starts the object (e.g. begins processing or listening). |
| `void` | [`stop`](#stop-6) `virtual` | Stops the object (e.g. halts processing or closes resources). |

---

{#start-7}

#### start

`virtual`

```cpp
virtual void start()
```

Defined in src/base/include/icy/interface.h:111

Starts the object (e.g. begins processing or listening).

---

{#stop-6}

#### stop

`virtual`

```cpp
virtual void stop()
```

Defined in src/base/include/icy/interface.h:114

Stops the object (e.g. halts processing or closes resources).

{#sendable}

## Sendable

```cpp
#include <icy/interface.h>
```

```cpp
class Sendable
```

Defined in src/base/include/icy/interface.h:119

> **Subclassed by:** [`PacketTransaction< Message >`](#packettransaction), [`PacketTransaction< PacketT >`](#packettransaction)

Abstract interface for classes that can be sent and cancelled.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
| `bool` | [`send`](#send-9) `virtual` | Initiates the send operation. |
| `void` | [`cancel`](#cancel-2) `virtual` | Cancels a pending send operation. |

---

{#send-9}

#### send

`virtual`

```cpp
virtual bool send()
```

Defined in src/base/include/icy/interface.h:124

Initiates the send operation. 
#### Returns
true if the send was dispatched successfully, false otherwise.

---

{#cancel-2}

#### cancel

`virtual`

```cpp
virtual void cancel()
```

Defined in src/base/include/icy/interface.h:127

Cancels a pending send operation.

{#fs}

# fs

Cross-platform filesystem path and file helpers.

### Functions

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`filename`](#filename)  | Returns the file name and extension part of the given path. |
| `std::string` | [`basename`](#basename)  | Returns the file name without its extension. |
| `std::string` | [`dirname`](#dirname)  | Returns the directory part of the path. |
| `std::string` | [`extname`](#extname)  | Returns the file extension part of the path. |
| `bool` | [`exists`](#exists-1)  | Returns true if the file or directory exists. |
| `bool` | [`isdir`](#isdir)  | Returns true if the path refers to a directory. |
| `std::int64_t` | [`filesize`](#filesize)  | Returns the size in bytes of the given file. |
| `void` | [`readdir`](#readdir)  | Populates `res` with the names of all entries in the given directory. |
| `void` | [`mkdir`](#mkdir)  | Creates a single directory. |
| `void` | [`mkdirr`](#mkdirr)  | Creates a directory and all missing parent directories. |
| `void` | [`rmdir`](#rmdir)  | Removes an empty directory. |
| `void` | [`unlink`](#unlink)  | Deletes a file. |
| `void` | [`rename`](#rename)  | Renames or moves the given file to the target path. |
| `void` | [`addsep`](#addsep)  | Appends the platform-specific path separator to `path` if not already present. |
| `void` | [`addnode`](#addnode)  | Appends a path node to `path`, inserting a separator if necessary. |
| `std::string` | [`makePath`](#makepath)  | Joins a base path and a node component into a single path string. |
| `std::string` | [`normalize`](#normalize)  | Normalizes a path by resolving `.` and `..` segments and converting separators to the native platform style. |
| `std::string` | [`transcode`](#transcode)  | Transcodes a path to the native platform format. On Windows with `ICY_UNICODE` defined, converts to the Windows-native wide-to-narrow format. On other platforms, returns the path unchanged. |
| `bool` | [`savefile`](#savefile)  | Writes `size` bytes from `data` to the file at `path`, creating or overwriting it. |

---

{#filename}

#### filename

```cpp
std::string filename(std::string_view path)
```

Returns the file name and extension part of the given path. 
#### Parameters
* `path` Filesystem path to parse. 

#### Returns
Filename component including extension (e.g. "file.txt").

---

{#basename}

#### basename

```cpp
std::string basename(std::string_view path)
```

Returns the file name without its extension. 
#### Parameters
* `path` Filesystem path to parse. 

#### Returns
Filename without the extension (e.g. "file").

---

{#dirname}

#### dirname

```cpp
std::string dirname(std::string_view path)
```

Returns the directory part of the path. 
#### Parameters
* `path` Filesystem path to parse. 

#### Returns
Directory component including trailing separator (e.g. "/usr/local/").

---

{#extname}

#### extname

```cpp
std::string extname(std::string_view path, bool includeDot = true)
```

Returns the file extension part of the path. 
#### Parameters
* `path` Filesystem path to parse. 

* `includeDot` If true (default), includes the leading dot (e.g. ".txt"); if false, the dot is stripped. 

#### Returns
Extension string, or empty if the path has no extension.

---

{#exists-1}

#### exists

```cpp
bool exists(std::string_view path)
```

Returns true if the file or directory exists. 
#### Parameters
* `path` Path to check. 

#### Returns
True if the path exists on the filesystem.

---

{#isdir}

#### isdir

```cpp
bool isdir(std::string_view path)
```

Returns true if the path refers to a directory. 
#### Parameters
* `path` Path to check. 

#### Returns
True if the path exists and is a directory.

---

{#filesize}

#### filesize

```cpp
std::int64_t filesize(std::string_view path)
```

Returns the size in bytes of the given file. 
#### Parameters
* `path` Path to the file. 

#### Returns
File size in bytes, or -1 if the file does not exist.

---

{#readdir}

#### readdir

```cpp
void readdir(std::string_view path, std::vector< std::string > & res)
```

Populates `res` with the names of all entries in the given directory. 
#### Parameters
* `path` Path to the directory to read. 

* `res` Vector to receive the list of entry names.

---

{#mkdir}

#### mkdir

```cpp
void mkdir(std::string_view path, int mode = 0755)
```

Creates a single directory. 
#### Parameters
* `path` Path of the directory to create. 

* `mode` Permission bits (default: 0755). Ignored on Windows.

---

{#mkdirr}

#### mkdirr

```cpp
void mkdirr(std::string_view path, int mode = 0755)
```

Creates a directory and all missing parent directories. 
#### Parameters
* `path` Path of the directory hierarchy to create. 

* `mode` Permission bits (default: 0755). Ignored on Windows.

---

{#rmdir}

#### rmdir

```cpp
void rmdir(std::string_view path)
```

Removes an empty directory. 
#### Parameters
* `path` Path of the directory to remove.

---

{#unlink}

#### unlink

```cpp
void unlink(std::string_view path)
```

Deletes a file. 
#### Parameters
* `path` Path of the file to delete.

---

{#rename}

#### rename

```cpp
void rename(std::string_view path, std::string_view target)
```

Renames or moves the given file to the target path. 
#### Parameters
* `path` Source file path. 

* `target` Destination file path.

---

{#addsep}

#### addsep

```cpp
void addsep(std::string & path)
```

Appends the platform-specific path separator to `path` if not already present. 
#### Parameters
* `path` Path string to modify in place.

---

{#addnode}

#### addnode

```cpp
void addnode(std::string & path, std::string_view node)
```

Appends a path node to `path`, inserting a separator if necessary. 
#### Parameters
* `path` Base path string to modify in place. 

* `node` Directory or file name component to append.

---

{#makepath}

#### makePath

```cpp
std::string makePath(std::string_view base, std::string_view node)
```

Joins a base path and a node component into a single path string. 
#### Parameters
* `base` Base directory path. 

* `node` Directory or file name component to append. 

#### Returns
Joined path string.

---

{#normalize}

#### normalize

```cpp
std::string normalize(std::string_view path)
```

Normalizes a path by resolving `.` and `..` segments and converting separators to the native platform style. 
#### Parameters
* `path` Path string to normalize. 

#### Returns
Normalized path string.

---

{#transcode}

#### transcode

```cpp
std::string transcode(std::string_view path)
```

Transcodes a path to the native platform format. On Windows with `ICY_UNICODE` defined, converts to the Windows-native wide-to-narrow format. On other platforms, returns the path unchanged. 
#### Parameters
* `path` Path string to transcode. 

#### Returns
Transcoded path string.

---

{#savefile}

#### savefile

```cpp
bool savefile(std::string_view path, const char * data, size_t size, bool whiny = false)
```

Writes `size` bytes from `data` to the file at `path`, creating or overwriting it. 
#### Parameters
* `path` Destination file path. Parent directories must already exist. 

* `data` Pointer to the data to write. 

* `size` Number of bytes to write. 

* `whiny` If true, throws a `std::runtime_error` on failure instead of returning false. 

#### Returns
True on success, false on failure (when `whiny` is false).

### Variables

| Return | Name | Description |
|--------|------|-------------|
| `const char *` | [`separator`](#separator)  | The platform specific path separator string: "/" on unix and "\\" on windows. |
| `const char` | [`delimiter`](#delimiter)  | The platform specific path separator character: '/' on unix and '\' on windows. |

---

{#separator}

#### separator

```cpp
const char * separator
```

The platform specific path separator string: "/" on unix and "\\" on windows.

---

{#delimiter}

#### delimiter

```cpp
const char delimiter
```

The platform specific path separator character: '/' on unix and '\' on windows.

{#util}

# util

Miscellaneous string, parsing, and version utilities.

### Classes

| Name | Description |
|------|-------------|
| [`Version`](#version) | Semantic version number with major, minor, and patch fields. |

### Functions

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`format`](#format-20)  | Printf-style string formatting for POD types. |
| `void` | [`toUnderscore`](#tounderscore)  | Replaces all non-alphanumeric characters in str with underscores and converts to lowercase. |
| `bool` | [`isNumber`](#isnumber)  | Returns true if str consists entirely of digit characters. |
| `bool` | [`endsWith`](#endswith)  | Returns true if str ends with the given suffix. |
| `void` | [`removeSpecialCharacters`](#removespecialcharacters)  | Replaces non-alphanumeric characters. Removes all non-alphanumeric characters from str in place. |
| `void` | [`replaceSpecialCharacters`](#replacespecialcharacters)  | Replaces all non-alphanumeric characters in str with `with` in place. |
| `bool` | [`tryParseHex`](#tryparsehex)  | Attempts to parse a hex string into an unsigned integer. |
| `unsigned` | [`parseHex`](#parsehex)  | Parses a hex string into an unsigned integer. |
| `std::string` | [`dumpbin`](#dumpbin)  | Formats the binary contents of data as a hex+ASCII dump string. |
| `bool` | [`compareVersion`](#compareversion)  | Compares two dot-separated version strings. |
| `bool` | [`matchNodes`](#matchnodes)  | Checks whether node matches xnode by splitting both on delim and comparing element-wise. |
| `bool` | [`matchNodes`](#matchnodes-1)  | Checks whether params matches xparams element-wise. |
| `std::string` | [`memAddress`](#memaddress)  | Returns the memory address of ptr as a hex string (e.g. "0x7f3a2b10c0"). |
| `std::string` | [`itostr`](#itostr)  | Converts an integer (or any stream-insertable type) to its string representation. |
| `T` | [`strtoi`](#strtoi)  | Parses a string into integer type T using std::istringstream. Returns 0 if parsing fails. Ensure T has sufficient range for the value. |
| `uint32_t` | [`randomNumber`](#randomnumber)  | Generates a 31-bit pseudo random number using the internal [Random](#random) instance. |
| `std::string` | [`randomString`](#randomstring)  | Generates a random alphanumeric string of the given length. |
| `std::string` | [`randomBinaryString`](#randombinarystring)  | Generates a random binary string of the given byte length. |
| `void` | [`split`](#split)  | Splits str on the delimiter string and appends tokens to elems. |
| `std::vector< std::string >` | [`split`](#split-1)  | Splits str on the delimiter string and returns the tokens as a vector. |
| `void` | [`split`](#split-2)  | Splits str on the delimiter character and appends tokens to elems. |
| `std::vector< std::string >` | [`split`](#split-3)  | Splits str on the delimiter character and returns the tokens as a vector. |
| `S &` | [`replaceInPlace`](#replaceinplace)  | Replace all occurrences of `from` in `str` with `to`, starting at position `start`. Modifies and returns `str` in place. `from` must not be empty. |
| `S &` | [`replaceInPlace`](#replaceinplace-1)  | Replace all occurrences of `from` in `str` with `to`, starting at position `start`. C-string overload. Modifies and returns `str` in place. |
| `S` | [`replace`](#replace)  | Replace all occurences of from (which must not be the empty string) in str with to, starting at position start. |
| `S` | [`replace`](#replace-1)  | Returns a copy of str with all occurrences of from replaced by to (C-string overload). |
| `S` | [`trimLeft`](#trimleft)  | Returns a copy of str with all leading whitespace removed. |
| `S &` | [`trimLeftInPlace`](#trimleftinplace)  | Removes all leading whitespace in str. |
| `S` | [`trimRight`](#trimright)  | Returns a copy of str with all trailing whitespace removed. |
| `S &` | [`trimRightInPlace`](#trimrightinplace)  | Removes all trailing whitespace in str. |
| `S` | [`trim`](#trim)  | Returns a copy of str with all leading and trailing whitespace removed. |
| `S &` | [`trimInPlace`](#triminplace)  | Removes all leading and trailing whitespace in str. |
| `S` | [`toUpper`](#toupper)  | Returns a copy of str containing all upper-case characters. |
| `S &` | [`toUpperInPlace`](#toupperinplace)  | Replaces all characters in str with their upper-case counterparts. |
| `S` | [`toLower`](#tolower)  | Returns a copy of str containing all lower-case characters. |
| `S &` | [`toLowerInPlace`](#tolowerinplace)  | Replaces all characters in str with their lower-case counterparts. |
| `int` | [`icompare`](#icompare) `inline` | Case-insensitive string comparison (locale-independent, ASCII only). |
| `std::streamsize` | [`copyStreamUnbuffered`](#copystreamunbuffered)  | Copies all bytes from istr to ostr one byte at a time (no internal buffer). |
| `std::streamsize` | [`copyStream`](#copystream)  | Copies all bytes from istr to ostr using an internal buffer. |
| `std::streamsize` | [`copyToString`](#copytostring)  | Reads all bytes from istr and appends them to str. |
| `void` | [`clearList`](#clearlist) `inline` | Delete all elements from a list of pointers. |
| `void` | [`clearDeque`](#cleardeque) `inline` | Delete all elements from a deque of pointers. |
| `void` | [`clearVector`](#clearvector) `inline` | Delete all elements from a vector of pointers. |
| `void` | [`clearMap`](#clearmap) `inline` | Delete all associated values from a map (not the key elements). |

---

{#format-20}

#### format

```cpp
std::string format(const char * fmt, ...)
```

Printf-style string formatting for POD types. 
#### Parameters
* `fmt` printf format string. 

* `...` Format arguments. 

#### Returns
Formatted string.

---

{#tounderscore}

#### toUnderscore

```cpp
void toUnderscore(std::string & str)
```

Replaces all non-alphanumeric characters in str with underscores and converts to lowercase. 
#### Parameters
* `str` String to transform in place.

---

{#isnumber}

#### isNumber

```cpp
bool isNumber(std::string_view str)
```

Returns true if str consists entirely of digit characters. 
#### Parameters
* `str` String to test. 

#### Returns
true if every character in str is a decimal digit.

---

{#endswith}

#### endsWith

```cpp
bool endsWith(std::string_view str, std::string_view suffix)
```

Returns true if str ends with the given suffix. 
#### Parameters
* `str` String to test. 

* `suffix` Suffix to look for. 

#### Returns
true if str ends with suffix.

---

{#removespecialcharacters}

#### removeSpecialCharacters

```cpp
void removeSpecialCharacters(std::string & str, bool allowSpaces = false)
```

Replaces non-alphanumeric characters. Removes all non-alphanumeric characters from str in place. 
#### Parameters
* `str` String to modify. 

* `allowSpaces` If true, ASCII spaces are preserved.

---

{#replacespecialcharacters}

#### replaceSpecialCharacters

```cpp
void replaceSpecialCharacters(std::string & str, char with = '_', bool allowSpaces = false)
```

Replaces all non-alphanumeric characters in str with `with` in place. 
#### Parameters
* `str` String to modify. 

* `with` Replacement character (default: '_'). 

* `allowSpaces` If true, ASCII spaces are preserved rather than replaced.

---

{#tryparsehex}

#### tryParseHex

```cpp
bool tryParseHex(std::string_view s, unsigned & value)
```

Attempts to parse a hex string into an unsigned integer. 
#### Parameters
* `s` Hex string (with or without 0x prefix). 

* `value` Output: parsed value on success. 

#### Returns
true if parsing succeeded, false otherwise.

---

{#parsehex}

#### parseHex

```cpp
unsigned parseHex(std::string_view s)
```

Parses a hex string into an unsigned integer. 
#### Parameters
* `s` Hex string (with or without 0x prefix). 

#### Returns
Parsed value. 

#### Exceptions
* `std::invalid_argument` if the string is not valid hex.

---

{#dumpbin}

#### dumpbin

```cpp
std::string dumpbin(const char * data, size_t len)
```

Formats the binary contents of data as a hex+ASCII dump string. 
#### Parameters
* `data` Pointer to the buffer to dump. 

* `len` Number of bytes to dump. 

#### Returns
Multi-line hex dump string.

---

{#compareversion}

#### compareVersion

```cpp
bool compareVersion(std::string_view l, std::string_view r)
```

Compares two dot-separated version strings. 
#### Parameters
* `l` Left (local) version string. 

* `r` Right (remote) version string. 

#### Returns
true if l is strictly greater than r, false if l is equal or less.

---

{#matchnodes}

#### matchNodes

```cpp
bool matchNodes(std::string_view node, std::string_view xnode, std::string_view delim = "\r\n")
```

Checks whether node matches xnode by splitting both on delim and comparing element-wise. 
#### Parameters
* `node` Node list string to test. 

* `xnode` Expected node list pattern. 

* `delim` Delimiter used to split both strings (default: "\r\n"). 

#### Returns
true if all elements of node match the corresponding elements of xnode.

---

{#matchnodes-1}

#### matchNodes

```cpp
bool matchNodes(const std::vector< std::string > & params, const std::vector< std::string > & xparams)
```

Checks whether params matches xparams element-wise. 
#### Parameters
* `params` Parameter list to test. 

* `xparams` Expected parameter list. 

#### Returns
true if every element of params matches the corresponding element of xparams.

---

{#memaddress}

#### memAddress

```cpp
std::string memAddress(const void * ptr)
```

Returns the memory address of ptr as a hex string (e.g. "0x7f3a2b10c0"). 
#### Parameters
* `ptr` Pointer whose address to format. 

#### Returns
Hex string representation of the pointer value.

---

{#itostr}

#### itostr

```cpp
template<typename T> std::string itostr(const T & t)
```

Converts an integer (or any stream-insertable type) to its string representation. 
#### Parameters
* `T` Type to convert; must support `operator<<` on std::ostream. 

#### Parameters
* `t` Value to convert. 

#### Returns
String representation of t.

---

{#strtoi}

#### strtoi

```cpp
template<typename T> T strtoi(std::string_view s)
```

Parses a string into integer type T using std::istringstream. Returns 0 if parsing fails. Ensure T has sufficient range for the value. 
#### Parameters
* `T` Target integer type. 

#### Parameters
* `s` String to parse. 

#### Returns
Parsed value, or 0 on failure.

---

{#randomnumber}

#### randomNumber

```cpp
uint32_t randomNumber()
```

Generates a 31-bit pseudo random number using the internal [Random](#random) instance. 
#### Returns
Pseudo random uint32_t value.

---

{#randomstring}

#### randomString

```cpp
std::string randomString(int size)
```

Generates a random alphanumeric string of the given length. 
#### Parameters
* `size` Number of characters to generate. 

#### Returns
[Random](#random) string of length size.

---

{#randombinarystring}

#### randomBinaryString

```cpp
std::string randomBinaryString(int size, bool doBase64 = false)
```

Generates a random binary string of the given byte length. 
#### Parameters
* `size` Number of random bytes to generate. 

* `doBase64` If true, returns the bytes as a base64-encoded string. 

#### Returns
[Random](#random) binary string, optionally base64-encoded.

---

{#split}

#### split

```cpp
void split(std::string_view str, std::string_view delim, std::vector< std::string > & elems, int limit = -1)
```

Splits str on the delimiter string and appends tokens to elems. 
#### Parameters
* `str` String to split. 

* `delim` Delimiter string. 

* `elems` Output vector; tokens are appended to it. 

* `limit` Maximum number of splits (-1 for unlimited).

---

{#split-1}

#### split

```cpp
std::vector< std::string > split(std::string_view str, std::string_view delim, int limit = -1)
```

Splits str on the delimiter string and returns the tokens as a vector. 
#### Parameters
* `str` String to split. 

* `delim` Delimiter string. 

* `limit` Maximum number of splits (-1 for unlimited). 

#### Returns
Vector of token strings.

---

{#split-2}

#### split

```cpp
void split(std::string_view str, char delim, std::vector< std::string > & elems, int limit = -1)
```

Splits str on the delimiter character and appends tokens to elems. 
#### Parameters
* `str` String to split. 

* `delim` Delimiter character. 

* `elems` Output vector; tokens are appended to it. 

* `limit` Maximum number of splits (-1 for unlimited).

---

{#split-3}

#### split

```cpp
std::vector< std::string > split(std::string_view str, char delim, int limit = -1)
```

Splits str on the delimiter character and returns the tokens as a vector. 
#### Parameters
* `str` String to split. 

* `delim` Delimiter character. 

* `limit` Maximum number of splits (-1 for unlimited). 

#### Returns
Vector of token strings.

---

{#replaceinplace}

#### replaceInPlace

```cpp
template<class S> S & replaceInPlace(S & str, const S & from, const S & to, typename S::size_type start = 0)
```

Replace all occurrences of `from` in `str` with `to`, starting at position `start`. Modifies and returns `str` in place. `from` must not be empty.

---

{#replaceinplace-1}

#### replaceInPlace

```cpp
template<class S> S & replaceInPlace(S & str, const typename S::value_type * from, const typename S::value_type * to, typename S::size_type start = 0)
```

Replace all occurrences of `from` in `str` with `to`, starting at position `start`. C-string overload. Modifies and returns `str` in place.

---

{#replace}

#### replace

```cpp
template<class S> S replace(const S & str, const S & from, const S & to, typename S::size_type start = 0)
```

Replace all occurences of from (which must not be the empty string) in str with to, starting at position start.

---

{#replace-1}

#### replace

```cpp
template<class S> S replace(const S & str, const typename S::value_type * from, const typename S::value_type * to, typename S::size_type start = 0)
```

Returns a copy of str with all occurrences of from replaced by to (C-string overload). 
#### Parameters
* `str` Source string. 

* `from` Substring to search for; must not be empty. 

* `to` Replacement string. 

* `start` Position in str at which to begin searching (default: 0). 

#### Returns
New string with all replacements applied.

---

{#trimleft}

#### trimLeft

```cpp
template<class S> S trimLeft(const S & str)
```

Returns a copy of str with all leading whitespace removed.

---

{#trimleftinplace}

#### trimLeftInPlace

```cpp
template<class S> S & trimLeftInPlace(S & str)
```

Removes all leading whitespace in str.

---

{#trimright}

#### trimRight

```cpp
template<class S> S trimRight(const S & str)
```

Returns a copy of str with all trailing whitespace removed.

---

{#trimrightinplace}

#### trimRightInPlace

```cpp
template<class S> S & trimRightInPlace(S & str)
```

Removes all trailing whitespace in str.

---

{#trim}

#### trim

```cpp
template<class S> S trim(const S & str)
```

Returns a copy of str with all leading and trailing whitespace removed.

---

{#triminplace}

#### trimInPlace

```cpp
template<class S> S & trimInPlace(S & str)
```

Removes all leading and trailing whitespace in str.

---

{#toupper}

#### toUpper

```cpp
template<class S> S toUpper(const S & str)
```

Returns a copy of str containing all upper-case characters.

---

{#toupperinplace}

#### toUpperInPlace

```cpp
template<class S> S & toUpperInPlace(S & str)
```

Replaces all characters in str with their upper-case counterparts.

---

{#tolower}

#### toLower

```cpp
template<class S> S toLower(const S & str)
```

Returns a copy of str containing all lower-case characters.

---

{#tolowerinplace}

#### toLowerInPlace

```cpp
template<class S> S & toLowerInPlace(S & str)
```

Replaces all characters in str with their lower-case counterparts.

---

{#icompare}

#### icompare

`inline`

```cpp
inline int icompare(std::string_view a, std::string_view b)
```

Case-insensitive string comparison (locale-independent, ASCII only). 
#### Parameters
* `a` First string. 

* `b` Second string. 

#### Returns
Negative if a < b, zero if a == b, positive if a > b.

---

{#copystreamunbuffered}

#### copyStreamUnbuffered

```cpp
std::streamsize copyStreamUnbuffered(std::istream & istr, std::ostream & ostr)
```

Copies all bytes from istr to ostr one byte at a time (no internal buffer). 
#### Parameters
* `istr` Source stream. 

* `ostr` Destination stream. 

#### Returns
Total number of bytes copied.

---

{#copystream}

#### copyStream

```cpp
std::streamsize copyStream(std::istream & istr, std::ostream & ostr, size_t bufferSize = 8192)
```

Copies all bytes from istr to ostr using an internal buffer. 
#### Parameters
* `istr` Source stream. 

* `ostr` Destination stream. 

* `bufferSize` Internal buffer size in bytes (default: 8192). 

#### Returns
Total number of bytes copied.

---

{#copytostring}

#### copyToString

```cpp
std::streamsize copyToString(std::istream & istr, std::string & str, size_t bufferSize = 8192)
```

Reads all bytes from istr and appends them to str. 
#### Parameters
* `istr` Source stream. 

* `str` Output string to append data to. 

* `bufferSize` Internal buffer size in bytes (default: 8192). 

#### Returns
Total number of bytes read.

---

{#clearlist}

#### clearList

`inline`

```cpp
template<typename Val> inline void clearList(std::list< Val * > & L)
```

Delete all elements from a list of pointers.

---

{#cleardeque}

#### clearDeque

`inline`

```cpp
template<typename Val> inline void clearDeque(std::deque< Val * > & D)
```

Delete all elements from a deque of pointers.

---

{#clearvector}

#### clearVector

`inline`

```cpp
template<typename Val> inline void clearVector(std::vector< Val * > & V)
```

Delete all elements from a vector of pointers.

---

{#clearmap}

#### clearMap

`inline`

```cpp
template<typename Key, typename Val> inline void clearMap(std::map< Key, Val * > & M)
```

Delete all associated values from a map (not the key elements).

{#version}

## Version

```cpp
#include <icy/util.h>
```

```cpp
struct Version
```

Defined in src/base/include/icy/util.h:491

Semantic version number with major, minor, and patch fields.

### Friends

| Name | Description |
|------|-------------|
| [`operator<<`](#operator-13) `inline` | Writes the version to a stream in "major.minor.revision.build" format. |

---

{#operator-13}

#### operator<<

`inline`

```cpp
friend inline std::ostream & operator<<(std::ostream & stream, const Version & ver)
```

Defined in src/base/include/icy/util.h:547

Writes the version to a stream in "major.minor.revision.build" format. 
#### Parameters
* `stream` Output stream. 

* `ver` [Version](#version) to format. 

#### Returns
Reference to stream.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `int` | [`major`](#major)  |  |
| `int` | [`minor`](#minor)  |  |
| `int` | [`revision`](#revision)  |  |
| `int` | [`build`](#build)  |  |

---

{#major}

#### major

```cpp
int major
```

Defined in src/base/include/icy/util.h:559

---

{#minor}

#### minor

```cpp
int minor
```

Defined in src/base/include/icy/util.h:559

---

{#revision}

#### revision

```cpp
int revision
```

Defined in src/base/include/icy/util.h:559

---

{#build}

#### build

```cpp
int build
```

Defined in src/base/include/icy/util.h:559

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Version`](#version-1) `inline` | Parses a dot-separated version string into up to four numeric fields. Unspecified fields default to 0. Examples: "1.2.3", "2.0", "3.7.8.0". |
| `bool` | [`operator<`](#operator-14) `inline` | Returns true if this version is strictly less than other. Compares fields in major, minor, revision, build order. |
| `bool` | [`operator==`](#operator-15) `const` `inline` | Returns true if all four version fields are equal. |

---

{#version-1}

#### Version

`inline`

```cpp
inline Version(std::string_view version)
```

Defined in src/base/include/icy/util.h:496

Parses a dot-separated version string into up to four numeric fields. Unspecified fields default to 0. Examples: "1.2.3", "2.0", "3.7.8.0". 
#### Parameters
* `version` Dot-separated version string.

---

{#operator-14}

#### operator<

`inline`

```cpp
inline bool operator<(const Version & other)
```

Defined in src/base/include/icy/util.h:521

Returns true if this version is strictly less than other. Compares fields in major, minor, revision, build order. 
#### Parameters
* `other` [Version](#version) to compare against. 

#### Returns
true if this < other.

---

{#operator-15}

#### operator==

`const` `inline`

```cpp
inline bool operator==(const Version & other) const
```

Defined in src/base/include/icy/util.h:537

Returns true if all four version fields are equal. 
#### Parameters
* `other` [Version](#version) to compare against. 

#### Returns
true if this == other.

{#pipe}

## Pipe

```cpp
#include <icy/pipe.h>
```

```cpp
class Pipe
```

Defined in src/base/include/icy/pipe.h:27

> **Inherits:** [`Stream< uv_pipe_t >`](#stream)

Named pipe / stdio stream built on `uv_pipe_t`.

Suitable for inter-process communication and for wrapping process stdio (stdin/stdout/stderr). IPC mode allows passing stream handles between processes over the pipe.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Pipe`](#pipe)  | Construct a `[Pipe](#pipe)` bound to `loop` without initializing the libuv handle. |
|  | [`~Pipe`](#pipe) `virtual` | Destroy the pipe, stopping reads and closing the handle. |
| `void` | [`init`](#init) `virtual` | Initialize the underlying `uv_pipe_t` handle. |
| `bool` | [`readStart`](#readstart) `virtual` `override` | Start reading from the pipe. |

---

{#pipe}

#### Pipe

```cpp
Pipe(uv::Loop * loop = uv::defaultLoop())
```

Defined in src/base/include/icy/pipe.h:35

Construct a `[Pipe](#pipe)` bound to `loop` without initializing the libuv handle.

Call `[init()](#classicy_1_1Pipe_1a09b32c70eceefd7d2339410b0863ad54)` before performing any I/O.

#### Parameters
* `loop` Event loop to associate with. Defaults to the process-wide default loop.

---

{#pipe}

#### ~Pipe

`virtual`

```cpp
virtual ~Pipe()
```

Defined in src/base/include/icy/pipe.h:38

Destroy the pipe, stopping reads and closing the handle.

---

{#init}

#### init

`virtual`

```cpp
virtual void init(bool ipc = false)
```

Defined in src/base/include/icy/pipe.h:46

Initialize the underlying `uv_pipe_t` handle.

Must be called before `[readStart()](#classicy_1_1Pipe_1aa346922ed574ccc39590f7dc3bae7885)` or any write operations.

#### Parameters
* `ipc` Set to `true` to enable IPC mode, which allows sending and receiving stream handles alongside data via `[write()](#classicy_1_1Stream_1a56926a6ac3ba433aed1414ffb1d20cf0)`.

---

{#readstart}

#### readStart

`virtual` `override`

```cpp
virtual bool readStart() override
```

Defined in src/base/include/icy/pipe.h:54

Start reading from the pipe.

Delegates to `[Stream](#stream)<uv_pipe_t>::[readStart()](#classicy_1_1Pipe_1aa346922ed574ccc39590f7dc3bae7885)`. Emits the `[Read](#classicy_1_1Stream_1a97b040391e1972b8599599a101be2184)` signal as data arrives.

#### Returns
`true` if `uv_read_start` was called successfully.

{#idler}

## Idler

```cpp
#include <icy/idler.h>
```

```cpp
class Idler
```

Defined in src/base/include/icy/idler.h:31

> **Inherits:** [`Runner`](#runner)

Asynchronous type that triggers callbacks when the event loop is idle.

This class inherits the `[Runner](#runner)` interface and may be used with any implementation that's powered by an asynchronous `[Runner](#runner)`.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Idler`](#idler)  | Create the idler bound to `loop` without starting it. |
|  | [`Idler`](#idler) `inline` `explicit` | Create and immediately start the idler on the default loop. |
|  | [`Idler`](#idler) `inline` `explicit` | Create and immediately start the idler on the given loop. |
| `void` | [`start`](#start) `inline` | Start the idler, invoking `func` (with `args`) on every idle iteration. |
| `void` | [`start`](#start) `virtual` `override` | Start the idler with a type-erased callback (implements `[Runner::start](#classicy_1_1Runner_1a5969e823f1ce0bdd8730d3108bf13fbd)`). |
| `uv::Handle< uv_idle_t > &` | [`handle`](#handle)  |  |

---

{#idler}

#### Idler

```cpp
Idler(uv::Loop * loop = uv::defaultLoop())
```

Defined in src/base/include/icy/idler.h:37

Create the idler bound to `loop` without starting it.

#### Parameters
* `loop` Event loop to associate with. Defaults to the process-wide default loop.

---

{#idler}

#### Idler

`inline` `explicit`

```cpp
template<typename Function, typename... Args> inline explicit Idler(Function && func, Args &&... args)
```

Defined in src/base/include/icy/idler.h:44

Create and immediately start the idler on the default loop.

#### Parameters
* `func` Callable invoked on every idle iteration. 

* `args` Arguments forwarded to `func`.

---

{#idler}

#### Idler

`inline` `explicit`

```cpp
template<typename Function, typename... Args> inline explicit Idler(uv::Loop * loop, Function && func, Args &&... args)
```

Defined in src/base/include/icy/idler.h:57

Create and immediately start the idler on the given loop.

#### Parameters
* `loop` Event loop to associate with. 

* `func` Callable invoked on every idle iteration. 

* `args` Arguments forwarded to `func`.

---

{#start}

#### start

`inline`

```cpp
template<typename Function, typename... Args> inline void start(Function && func, Args &&... args)
```

Defined in src/base/include/icy/idler.h:73

Start the idler, invoking `func` (with `args`) on every idle iteration.

The idler always runs in repeating mode; cancel via `[Runner::cancel()](#classicy_1_1Runner_1a53b3794fbd4ace235771a4e84f93f1e7)`. Throws `std::logic_error` if the handle is already active or the runner context reports it is still running.

#### Parameters
* `func` Callable invoked each time the event loop is idle. 

* `args` Arguments forwarded to `func` on each invocation.

---

{#start}

#### start

`virtual` `override`

```cpp
virtual void start(std::function< void()> func) override
```

Defined in src/base/include/icy/idler.h:118

Start the idler with a type-erased callback (implements `[Runner::start](#classicy_1_1Runner_1a5969e823f1ce0bdd8730d3108bf13fbd)`).

#### Parameters
* `func` Callback invoked on every idle iteration.

---

{#handle}

#### handle

```cpp
uv::Handle< uv_idle_t > & handle()
```

Defined in src/base/include/icy/idler.h:123

#### Returns
Reference to the underlying `uv_idle_t` handle wrapper.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `uv::Handle< uv_idle_t >` | [`_handle`](#_handle)  |  |

---

{#_handle}

#### _handle

```cpp
uv::Handle< uv_idle_t > _handle
```

Defined in src/base/include/icy/idler.h:132

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`init`](#init) `virtual` | Initialize the underlying `uv_idle_t` handle and unref it from the loop. |
| `bool` | [`async`](#async) `virtual` `const` `override` |  |

---

{#init}

#### init

`virtual`

```cpp
virtual void init()
```

Defined in src/base/include/icy/idler.h:127

Initialize the underlying `uv_idle_t` handle and unref it from the loop.

---

{#async}

#### async

`virtual` `const` `override`

```cpp
virtual bool async() const override
```

Defined in src/base/include/icy/idler.h:130

#### Returns
`false`; the idler is event-loop-driven, not thread-based.

{#queue}

## Queue

```cpp
#include <icy/queue.h>
```

```cpp
template<typename T>
class Queue
```

Defined in src/base/include/icy/queue.h:30

Thread-safe queue container.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`push`](#push) `inline` | Appends an item to the back of the queue (thread-safe). |
| `void` | [`push`](#push) `inline` | Appends an item to the back of the queue by move (thread-safe). |
| `bool` | [`empty`](#empty) `const` `inline` |  |
| `T` | [`front`](#front) `const` `inline` |  |
| `T` | [`back`](#back) `const` `inline` |  |
| `void` | [`pop`](#pop) `inline` | Removes the front item from the queue (thread-safe). |
| `void` | [`sort`](#sort) `inline` | Sorts all queued items using the given comparator (thread-safe). |
| `size_t` | [`size`](#size) `const` `inline` |  |
| `std::deque< T >` | [`queue`](#queue) `const` `inline` |  |

---

{#push}

#### push

`inline`

```cpp
inline void push(const T & data)
```

Defined in src/base/include/icy/queue.h:35

Appends an item to the back of the queue (thread-safe). 
#### Parameters
* `data` Item to enqueue.

---

{#push}

#### push

`inline`

```cpp
inline void push(T && data)
```

Defined in src/base/include/icy/queue.h:43

Appends an item to the back of the queue by move (thread-safe). 
#### Parameters
* `data` Item to enqueue.

---

{#empty}

#### empty

`const` `inline`

```cpp
inline bool empty() const
```

Defined in src/base/include/icy/queue.h:50

#### Returns
True if the queue contains no items.

---

{#front}

#### front

`const` `inline`

```cpp
inline T front() const
```

Defined in src/base/include/icy/queue.h:57

#### Returns
Copy of the front item.

---

{#back}

#### back

`const` `inline`

```cpp
inline T back() const
```

Defined in src/base/include/icy/queue.h:64

#### Returns
Copy of the back item.

---

{#pop}

#### pop

`inline`

```cpp
inline void pop()
```

Defined in src/base/include/icy/queue.h:71

Removes the front item from the queue (thread-safe).

---

{#sort}

#### sort

`inline`

```cpp
template<typename Compare> inline void sort()
```

Defined in src/base/include/icy/queue.h:80

Sorts all queued items using the given comparator (thread-safe). 
#### Parameters
* `Compare` Comparator type compatible with `std::sort`.

---

{#size}

#### size

`const` `inline`

```cpp
inline size_t size() const
```

Defined in src/base/include/icy/queue.h:87

#### Returns
Number of items currently in the queue (thread-safe).

---

{#queue}

#### queue

`const` `inline`

```cpp
inline std::deque< T > queue() const
```

Defined in src/base/include/icy/queue.h:94

#### Returns
Copy of the underlying deque.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::deque< T >` | [`_queue`](#_queue)  |  |
| `std::mutex` | [`_mutex`](#_mutex)  |  |

---

{#_queue}

#### _queue

```cpp
std::deque< T > _queue
```

Defined in src/base/include/icy/queue.h:101

---

{#_mutex}

#### _mutex

```cpp
std::mutex _mutex
```

Defined in src/base/include/icy/queue.h:102

{#runnablequeue}

## RunnableQueue

```cpp
#include <icy/queue.h>
```

```cpp
template<class T>
class RunnableQueue
```

Defined in src/base/include/icy/queue.h:113

> **Inherits:** [`Queue< T * >`](#queue), [`Runnable`](#runnable)
> **Subclassed by:** [`AsyncQueue< T >`](#asyncqueue), [`SyncQueue< T >`](#syncqueue)

[Queue](#queue) of runnable tasks for sequential execution.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::function< void(T &)>` | [`ondispatch`](#ondispatch)  | The default dispatch function. Must be set before the queue is running. |

---

{#ondispatch}

#### ondispatch

```cpp
std::function< void(T &)> ondispatch
```

Defined in src/base/include/icy/queue.h:119

The default dispatch function. Must be set before the queue is running.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`RunnableQueue`](#runnablequeue) `inline` |  |
| `void` | [`push`](#push) `virtual` `inline` | Push an item onto the queue. The queue takes ownership of the item pointer. |
| `void` | [`flush`](#flush) `virtual` `inline` | Flush all outgoing items. |
| `void` | [`clear`](#clear) `inline` |  |
| `void` | [`run`](#run) `virtual` `inline` `override` | Called asynchronously to dispatch queued items. If not timeout is set this method blocks until [cancel()](#cancel-1) is called, otherwise [runTimeout()](#classicy_1_1RunnableQueue_1a32e7d20cdbcd09993ba0c76830faf833) will be called. |
| `void` | [`runTimeout`](#runtimeout) `virtual` `inline` | Called asynchronously to dispatch queued items until the queue is empty or the timeout expires. Pseudo protected for std::bind compatability. |
| `void` | [`dispatch`](#dispatch) `virtual` `inline` | Dispatch a single item to listeners. |
| `int` | [`timeout`](#timeout) `inline` |  |
| `void` | [`setTimeout`](#settimeout) `inline` | Sets the dispatch timeout. Must only be called when the queue is empty. |
| `size_t` | [`dropped`](#dropped) `const` `inline` |  |

---

{#runnablequeue}

#### RunnableQueue

`inline`

```cpp
inline RunnableQueue(int limit = 2048, int timeout = 0)
```

Defined in src/base/include/icy/queue.h:123

#### Parameters
* `limit` Maximum number of queued items; oldest are purged when exceeded (0 = unlimited). 

* `timeout` Dispatch timeout in milliseconds; 0 means run until [cancel()](#cancel-1) is called.

---

{#push}

#### push

`virtual` `inline`

```cpp
virtual inline void push(T * item)
```

Defined in src/base/include/icy/queue.h:136

Push an item onto the queue. The queue takes ownership of the item pointer.

---

{#flush}

#### flush

`virtual` `inline`

```cpp
virtual inline void flush()
```

Defined in src/base/include/icy/queue.h:151

Flush all outgoing items.

---

{#clear}

#### clear

`inline`

```cpp
inline void clear()
```

Defined in src/base/include/icy/queue.h:169

---

{#run}

#### run

`virtual` `inline` `override`

```cpp
virtual inline void run() override
```

Defined in src/base/include/icy/queue.h:181

Called asynchronously to dispatch queued items. If not timeout is set this method blocks until [cancel()](#cancel-1) is called, otherwise [runTimeout()](#classicy_1_1RunnableQueue_1a32e7d20cdbcd09993ba0c76830faf833) will be called.

---

{#runtimeout}

#### runTimeout

`virtual` `inline`

```cpp
virtual inline void runTimeout()
```

Defined in src/base/include/icy/queue.h:196

Called asynchronously to dispatch queued items until the queue is empty or the timeout expires. Pseudo protected for std::bind compatability.

---

{#dispatch}

#### dispatch

`virtual` `inline`

```cpp
virtual inline void dispatch(T & item)
```

Defined in src/base/include/icy/queue.h:206

Dispatch a single item to listeners.

---

{#timeout}

#### timeout

`inline`

```cpp
inline int timeout()
```

Defined in src/base/include/icy/queue.h:213

#### Returns
Current dispatch timeout in milliseconds.

---

{#settimeout}

#### setTimeout

`inline`

```cpp
inline void setTimeout(int milliseconds)
```

Defined in src/base/include/icy/queue.h:222

Sets the dispatch timeout. Must only be called when the queue is empty. 
#### Parameters
* `milliseconds` New timeout in milliseconds. 

#### Exceptions
* `std::logic_error` if the queue is non-empty.

---

{#dropped}

#### dropped

`const` `inline`

```cpp
inline size_t dropped() const
```

Defined in src/base/include/icy/queue.h:231

#### Returns
Number of items purged because the queue limit was exceeded.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `int` | [`_limit`](#_limit)  |  |
| `int` | [`_timeout`](#_timeout)  |  |
| `size_t` | [`_dropped`](#_dropped)  |  |

---

{#_limit}

#### _limit

```cpp
int _limit
```

Defined in src/base/include/icy/queue.h:270

---

{#_timeout}

#### _timeout

```cpp
int _timeout
```

Defined in src/base/include/icy/queue.h:271

---

{#_dropped}

#### _dropped

```cpp
size_t _dropped = 0
```

Defined in src/base/include/icy/queue.h:272

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`RunnableQueue`](#runnablequeue)  | Deleted constructor. |
|  | [`RunnableQueue`](#runnablequeue)  | Deleted constructor. |
| `T *` | [`popNext`](#popnext) `virtual` `inline` | Pops the next waiting item. |
| `bool` | [`dispatchNext`](#dispatchnext) `virtual` `inline` | Pops and dispatches the next waiting item. |

---

{#runnablequeue}

#### RunnableQueue

```cpp
RunnableQueue(const RunnableQueue &) = delete
```

Defined in src/base/include/icy/queue.h:238

Deleted constructor.

---

{#runnablequeue}

#### RunnableQueue

```cpp
RunnableQueue(RunnableQueue &&) = delete
```

Defined in src/base/include/icy/queue.h:240

Deleted constructor.

---

{#popnext}

#### popNext

`virtual` `inline`

```cpp
virtual inline T * popNext()
```

Defined in src/base/include/icy/queue.h:244

Pops the next waiting item.

---

{#dispatchnext}

#### dispatchNext

`virtual` `inline`

```cpp
virtual inline bool dispatchNext()
```

Defined in src/base/include/icy/queue.h:259

Pops and dispatches the next waiting item.

{#syncqueue}

## SyncQueue

```cpp
#include <icy/queue.h>
```

```cpp
template<class T>
class SyncQueue
```

Defined in src/base/include/icy/queue.h:285

> **Inherits:** [`RunnableQueue< T >`](#runnablequeue)

[SyncQueue](#syncqueue) extends [Synchronizer](#synchronizer) to implement a synchronized FIFO queue which receives T objects from any thread and synchronizes them for safe consumption by the associated event loop.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`SyncQueue`](#syncqueue) `inline` |  |
|  | [`~SyncQueue`](#syncqueue) `virtual` `inline` | Destruction is deferred to allow enough time for all callbacks to return. |
| `void` | [`push`](#push) `virtual` `inline` `override` | Pushes an item onto the queue and wakes the event loop for dispatch. Ownership of `item` is transferred to the queue. |
| `void` | [`cancel`](#cancel) `virtual` `inline` `override` | Cancels the queue and its underlying synchronizer. |
| `Synchronizer &` | [`sync`](#sync) `inline` |  |

---

{#syncqueue}

#### SyncQueue

`inline`

```cpp
inline SyncQueue(uv::Loop * loop, int limit = 2048, int timeout = 20)
```

Defined in src/base/include/icy/queue.h:293

#### Parameters
* `loop` Event loop used by the internal `[Synchronizer](#synchronizer)`. 

* `limit` Maximum queued items; oldest are dropped when exceeded. 

* `timeout` Dispatch timeout in milliseconds passed to `[RunnableQueue](#runnablequeue)`.

---

{#syncqueue}

#### ~SyncQueue

`virtual` `inline`

```cpp
virtual inline ~SyncQueue()
```

Defined in src/base/include/icy/queue.h:301

Destruction is deferred to allow enough time for all callbacks to return.

---

{#push}

#### push

`virtual` `inline` `override`

```cpp
virtual inline void push(T * item) override
```

Defined in src/base/include/icy/queue.h:308

Pushes an item onto the queue and wakes the event loop for dispatch. Ownership of `item` is transferred to the queue. 
#### Parameters
* `item` Heap-allocated item to enqueue; the queue takes ownership.

---

{#cancel}

#### cancel

`virtual` `inline` `override`

```cpp
virtual inline void cancel(bool flag = true) override
```

Defined in src/base/include/icy/queue.h:316

Cancels the queue and its underlying synchronizer. 
#### Parameters
* `flag` True to cancel, false to un-cancel.

---

{#sync}

#### sync

`inline`

```cpp
inline Synchronizer & sync()
```

Defined in src/base/include/icy/queue.h:326

#### Returns
Reference to the underlying `[Synchronizer](#synchronizer)` handle.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `Synchronizer` | [`_sync`](#_sync)  |  |

---

{#_sync}

#### _sync

```cpp
Synchronizer _sync
```

Defined in src/base/include/icy/queue.h:329

### Public Types

| Name | Description |
|------|-------------|
| [`Queue`](#queue)  |  |

---

{#queue}

#### Queue

```cpp
using Queue = RunnableQueue< T >
```

Defined in src/base/include/icy/queue.h:288

{#asyncqueue}

## AsyncQueue

```cpp
#include <icy/queue.h>
```

```cpp
template<class T>
class AsyncQueue
```

Defined in src/base/include/icy/queue.h:347

> **Inherits:** [`RunnableQueue< T >`](#runnablequeue)
> **Subclassed by:** [`AsyncPacketQueue< PacketT >`](#asyncpacketqueue)

[AsyncQueue](#asyncqueue) is a thread-based queue which receives packets from any thread source and dispatches them asynchronously.

This queue is useful for deferring load from operation critical system devices before performing long running tasks.

The thread will call the [RunnableQueue](#runnablequeue)'s [run()](#classicy_1_1RunnableQueue_1a23ae7b15a81f939ea603d123d9aec2d5) method to constantly flush outgoing packets until [cancel()](#classicy_1_1AsyncQueue_1a23b99d0d77fb154c027fa717e7949840) is called.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`AsyncQueue`](#asyncqueue) `inline` |  |
| `void` | [`cancel`](#cancel) `virtual` `inline` `override` | Cancels the queue and joins the dispatch thread. |

---

{#asyncqueue}

#### AsyncQueue

`inline`

```cpp
inline AsyncQueue(int limit = 2048)
```

Defined in src/base/include/icy/queue.h:353

#### Parameters
* `limit` Maximum number of queued items before oldest are dropped.

---

{#cancel}

#### cancel

`virtual` `inline` `override`

```cpp
virtual inline void cancel(bool flag = true) override
```

Defined in src/base/include/icy/queue.h:361

Cancels the queue and joins the dispatch thread. 
#### Parameters
* `flag` True to cancel, false to un-cancel.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `Thread` | [`_thread`](#_thread)  |  |

---

{#_thread}

#### _thread

```cpp
Thread _thread
```

Defined in src/base/include/icy/queue.h:372

### Public Types

| Name | Description |
|------|-------------|
| [`Queue`](#queue)  |  |

---

{#queue}

#### Queue

```cpp
using Queue = RunnableQueue< T >
```

Defined in src/base/include/icy/queue.h:350

{#timer}

## Timer

```cpp
#include <icy/timer.h>
```

```cpp
class Timer
```

Defined in src/base/include/icy/timer.h:27

> **Inherits:** [`Runner`](#runner)

Asynchronous event based timer.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `NullSignal` | [`Timeout`](#timeout)  | [Signal](#signal) that gets triggered on timeout. |

---

{#timeout}

#### Timeout

```cpp
NullSignal Timeout
```

Defined in src/base/include/icy/timer.h:130

[Signal](#signal) that gets triggered on timeout.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Timer`](#timer)  | Create a timer without a timeout or interval; values must be set before `[start()](#classicy_1_1Timer_1a59e3275d17107212520b362cbf0cb3d5)`. |
|  | [`Timer`](#timer)  | Create a one-shot timer that fires after `timeout` milliseconds. |
|  | [`Timer`](#timer)  | Create a repeating timer with an initial delay and a repeat period. |
|  | [`~Timer`](#timer) `virtual` | Destructor. |
| `void` | [`start`](#start)  | Start the timer using the previously configured timeout and interval. |
| `void` | [`start`](#start) `virtual` `override` | Connect `func` to the `[Timeout](#timeout)` signal and start the timer. |
| `void` | [`stop`](#stop)  | Stop the timer. Resets the fire count to zero. |
| `void` | [`restart`](#restart)  | Restart the timer. |
| `void` | [`again`](#again)  | Reset the countdown without stopping and restarting the timer. |
| `void` | [`setTimeout`](#settimeout)  | Set the initial delay before the first timeout event. |
| `void` | [`setInterval`](#setinterval)  | Set the repeat interval (calls `uv_timer_set_repeat`). |
| `bool` | [`active`](#active) `const` |  |
| `std::int64_t` | [`timeout`](#timeout) `const` |  |
| `std::int64_t` | [`interval`](#interval) `const` |  |
| `std::int64_t` | [`count`](#count)  |  |
| `uv::Handle< uv_timer_t > &` | [`handle`](#handle)  |  |
| `bool` | [`async`](#async) `virtual` `const` `override` |  |

---

{#timer}

#### Timer

```cpp
Timer(uv::Loop * loop = uv::defaultLoop())
```

Defined in src/base/include/icy/timer.h:33

Create a timer without a timeout or interval; values must be set before `[start()](#classicy_1_1Timer_1a59e3275d17107212520b362cbf0cb3d5)`.

#### Parameters
* `loop` Event loop to associate with. Defaults to the process-wide default loop.

---

{#timer}

#### Timer

```cpp
Timer(std::int64_t timeout, uv::Loop * loop = uv::defaultLoop(), std::function< void()> func = nullptr)
```

Defined in src/base/include/icy/timer.h:44

Create a one-shot timer that fires after `timeout` milliseconds.

If `func` is provided the timer starts immediately; otherwise call `[start()](#classicy_1_1Timer_1a59e3275d17107212520b362cbf0cb3d5)` manually.

#### Parameters
* `timeout` Delay in milliseconds before the first (and only) fire. 

* `loop` Event loop to associate with. 

* `func` Optional callback connected to the `[Timeout](#timeout)` signal and used to start the timer immediately.

---

{#timer}

#### Timer

```cpp
Timer(std::int64_t timeout, std::int64_t interval, uv::Loop * loop = uv::defaultLoop(), std::function< void()> func = nullptr)
```

Defined in src/base/include/icy/timer.h:57

Create a repeating timer with an initial delay and a repeat period.

Fires once after `timeout` milliseconds, then repeatedly every `interval` milliseconds. If `func` is provided the timer starts immediately; otherwise call `[start()](#classicy_1_1Timer_1a59e3275d17107212520b362cbf0cb3d5)` manually.

#### Parameters
* `timeout` Initial delay in milliseconds. 

* `interval` Repeat period in milliseconds. 

* `loop` Event loop to associate with. 

* `func` Optional callback connected to the `[Timeout](#timeout)` signal and used to start the timer immediately.

---

{#timer}

#### ~Timer

`virtual`

```cpp
virtual ~Timer()
```

Defined in src/base/include/icy/timer.h:60

Destructor.

---

{#start}

#### start

```cpp
void start()
```

Defined in src/base/include/icy/timer.h:67

Start the timer using the previously configured timeout and interval.

Throws `std::logic_error` if neither a timeout nor an interval has been set, or if the handle has not been allocated. Has no effect if already active.

---

{#start}

#### start

`virtual` `override`

```cpp
virtual void start(std::function< void()> func) override
```

Defined in src/base/include/icy/timer.h:72

Connect `func` to the `[Timeout](#timeout)` signal and start the timer.

#### Parameters
* `func` Callback invoked on each timeout event.

---

{#stop}

#### stop

```cpp
void stop()
```

Defined in src/base/include/icy/timer.h:77

Stop the timer. Resets the fire count to zero.

Has no effect if the timer is not active.

---

{#restart}

#### restart

```cpp
void restart()
```

Defined in src/base/include/icy/timer.h:85

Restart the timer.

If the timer is not currently active, behaves like `[start()](#classicy_1_1Timer_1a59e3275d17107212520b362cbf0cb3d5)`. If it is active, calls `[again()](#classicy_1_1Timer_1a1479aec7fadd9fdc71a41ed6a51af1c8)` to reset the countdown using the repeat value. A timeout or interval must be set beforehand, otherwise `[start()](#classicy_1_1Timer_1a59e3275d17107212520b362cbf0cb3d5)` will throw `std::logic_error`.

---

{#again}

#### again

```cpp
void again()
```

Defined in src/base/include/icy/timer.h:93

Reset the countdown without stopping and restarting the timer.

For repeating timers this restarts the repeat interval from now. For non-repeating timers this has the same effect as stopping the timer. Throws on error if `uv_timer_again` fails (e.g. the timer was never started). Resets the fire count to zero.

---

{#settimeout}

#### setTimeout

```cpp
void setTimeout(std::int64_t timeout)
```

Defined in src/base/include/icy/timer.h:100

Set the initial delay before the first timeout event.

Has no effect if the timer is currently active.

#### Parameters
* `timeout` Delay in milliseconds.

---

{#setinterval}

#### setInterval

```cpp
void setInterval(std::int64_t interval)
```

Defined in src/base/include/icy/timer.h:109

Set the repeat interval (calls `uv_timer_set_repeat`).

Takes effect from the next timeout event. If set from within a timer callback on a repeating timer, the new value is used from the following iteration; on a non-repeating timer the timer will have already stopped.

#### Parameters
* `interval` Repeat period in milliseconds; 0 disables repeating.

---

{#active}

#### active

`const`

```cpp
bool active() const
```

Defined in src/base/include/icy/timer.h:112

#### Returns
`true` if the timer is currently counting down.

---

{#timeout}

#### timeout

`const`

```cpp
std::int64_t timeout() const
```

Defined in src/base/include/icy/timer.h:115

#### Returns
The configured initial timeout in milliseconds.

---

{#interval}

#### interval

`const`

```cpp
std::int64_t interval() const
```

Defined in src/base/include/icy/timer.h:118

#### Returns
The effective repeat interval reported by libuv, in milliseconds.

---

{#count}

#### count

```cpp
std::int64_t count()
```

Defined in src/base/include/icy/timer.h:121

#### Returns
The number of times the timer has fired since the last start or [again()](#classicy_1_1Timer_1a1479aec7fadd9fdc71a41ed6a51af1c8).

---

{#handle}

#### handle

```cpp
uv::Handle< uv_timer_t > & handle()
```

Defined in src/base/include/icy/timer.h:124

#### Returns
Reference to the underlying `uv_timer_t` handle wrapper.

---

{#async}

#### async

`virtual` `const` `override`

```cpp
virtual bool async() const override
```

Defined in src/base/include/icy/timer.h:127

#### Returns
`false`; the timer is event-loop-driven, not thread-based.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `uv::Handle< uv_timer_t >` | [`_handle`](#_handle)  |  |
| `std::int64_t` | [`_timeout`](#_timeout)  |  |
| `std::int64_t` | [`_interval`](#_interval)  |  |
| `std::int64_t` | [`_count`](#_count)  |  |

---

{#_handle}

#### _handle

```cpp
uv::Handle< uv_timer_t > _handle
```

Defined in src/base/include/icy/timer.h:140

---

{#_timeout}

#### _timeout

```cpp
std::int64_t _timeout
```

Defined in src/base/include/icy/timer.h:141

---

{#_interval}

#### _interval

```cpp
std::int64_t _interval
```

Defined in src/base/include/icy/timer.h:142

---

{#_count}

#### _count

```cpp
std::int64_t _count
```

Defined in src/base/include/icy/timer.h:143

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Timer`](#timer)  | Deleted constructor. |
|  | [`Timer`](#timer)  | Deleted constructor. |
| `void` | [`init`](#init)  |  |

---

{#timer}

#### Timer

```cpp
Timer(const Timer &) = delete
```

Defined in src/base/include/icy/timer.h:133

Deleted constructor.

---

{#timer}

#### Timer

```cpp
Timer(Timer &&) = delete
```

Defined in src/base/include/icy/timer.h:135

Deleted constructor.

---

{#init}

#### init

```cpp
void init()
```

Defined in src/base/include/icy/timer.h:138

{#mutablebuffer}

## MutableBuffer

```cpp
#include <icy/buffer.h>
```

```cpp
class MutableBuffer
```

Defined in src/base/include/icy/buffer.h:40

The [MutableBuffer](#mutablebuffer) class provides a safe representation of a buffer that can be modified. It does not own the underlying data, and so is cheap to copy or assign.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
| `constexpr` | [`MutableBuffer`](#mutablebuffer) `inline` `constexpr` | Construct an empty buffer. |
| `constexpr` | [`MutableBuffer`](#mutablebuffer) `inline` `constexpr` | Construct a buffer to represent the given memory range. |
| `void *` | [`data`](#data) `const` `inline` `nodiscard` `constexpr` |  |
| `size_t` | [`size`](#size) `const` `inline` `nodiscard` `constexpr` |  |
| `char *` | [`cstr`](#cstr) `const` `inline` `nodiscard` | Cast the buffer as a char pointer. |
| `std::string` | [`str`](#str) `const` `inline` | Returns the buffer as a string. |

---

{#mutablebuffer}

#### MutableBuffer

`inline` `constexpr`

```cpp
constexpr inline constexpr MutableBuffer()
```

Defined in src/base/include/icy/buffer.h:44

Construct an empty buffer.

---

{#mutablebuffer}

#### MutableBuffer

`inline` `constexpr`

```cpp
constexpr inline constexpr MutableBuffer(void * data, size_t size)
```

Defined in src/base/include/icy/buffer.h:51

Construct a buffer to represent the given memory range.

---

{#data}

#### data

`const` `inline` `nodiscard` `constexpr`

```cpp
[[nodiscard]] constexpr inline void * data() const
```

Defined in src/base/include/icy/buffer.h:57

---

{#size}

#### size

`const` `inline` `nodiscard` `constexpr`

```cpp
[[nodiscard]] constexpr inline size_t size() const
```

Defined in src/base/include/icy/buffer.h:58

---

{#cstr}

#### cstr

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline char * cstr() const
```

Defined in src/base/include/icy/buffer.h:61

Cast the buffer as a char pointer.

---

{#str}

#### str

`const` `inline`

```cpp
inline std::string str() const
```

Defined in src/base/include/icy/buffer.h:64

Returns the buffer as a string.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `void *` | [`_data`](#_data)  |  |
| `size_t` | [`_size`](#_size)  |  |

---

{#_data}

#### _data

```cpp
void * _data
```

Defined in src/base/include/icy/buffer.h:67

---

{#_size}

#### _size

```cpp
size_t _size
```

Defined in src/base/include/icy/buffer.h:68

{#constbuffer}

## ConstBuffer

```cpp
#include <icy/buffer.h>
```

```cpp
class ConstBuffer
```

Defined in src/base/include/icy/buffer.h:142

The [ConstBuffer](#constbuffer) class provides a safe representation of a buffer that cannot be modified. It does not own the underlying data, and so is cheap to copy or assign.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
| `constexpr` | [`ConstBuffer`](#constbuffer) `inline` `constexpr` | Construct an empty buffer. |
| `constexpr` | [`ConstBuffer`](#constbuffer) `inline` `constexpr` | Construct a buffer to represent the given memory range. |
| `constexpr` | [`ConstBuffer`](#constbuffer) `inline` `constexpr` | Construct a non-modifiable buffer from a modifiable one. |
| `const void *` | [`data`](#data) `const` `inline` `nodiscard` `constexpr` |  |
| `size_t` | [`size`](#size) `const` `inline` `nodiscard` `constexpr` |  |
| `const char *` | [`cstr`](#cstr) `const` `inline` `nodiscard` | Cast the buffer as a const char pointer. |
| `std::string` | [`str`](#str) `const` `inline` | Returns the buffer as a string. |

---

{#constbuffer}

#### ConstBuffer

`inline` `constexpr`

```cpp
constexpr inline constexpr ConstBuffer()
```

Defined in src/base/include/icy/buffer.h:146

Construct an empty buffer.

---

{#constbuffer}

#### ConstBuffer

`inline` `constexpr`

```cpp
constexpr inline constexpr ConstBuffer(const void * data, size_t size)
```

Defined in src/base/include/icy/buffer.h:153

Construct a buffer to represent the given memory range.

---

{#constbuffer}

#### ConstBuffer

`inline` `constexpr`

```cpp
constexpr inline constexpr ConstBuffer(const MutableBuffer & b)
```

Defined in src/base/include/icy/buffer.h:160

Construct a non-modifiable buffer from a modifiable one.

---

{#data}

#### data

`const` `inline` `nodiscard` `constexpr`

```cpp
[[nodiscard]] constexpr inline const void * data() const
```

Defined in src/base/include/icy/buffer.h:166

---

{#size}

#### size

`const` `inline` `nodiscard` `constexpr`

```cpp
[[nodiscard]] constexpr inline size_t size() const
```

Defined in src/base/include/icy/buffer.h:167

---

{#cstr}

#### cstr

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline const char * cstr() const
```

Defined in src/base/include/icy/buffer.h:170

Cast the buffer as a const char pointer.

---

{#str}

#### str

`const` `inline`

```cpp
inline std::string str() const
```

Defined in src/base/include/icy/buffer.h:173

Returns the buffer as a string.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `const void *` | [`_data`](#_data)  |  |
| `size_t` | [`_size`](#_size)  |  |

---

{#_data}

#### _data

```cpp
const void * _data
```

Defined in src/base/include/icy/buffer.h:176

---

{#_size}

#### _size

```cpp
size_t _size
```

Defined in src/base/include/icy/buffer.h:177

{#bitreader}

## BitReader

```cpp
#include <icy/buffer.h>
```

```cpp
class BitReader
```

Defined in src/base/include/icy/buffer.h:273

Class for reading binary streams.

### Friends

| Name | Description |
|------|-------------|
| [`operator<<`](#operator) `inline` |  |

---

{#operator}

#### operator<<

`inline`

```cpp
friend inline std::ostream & operator<<(std::ostream & stream, const BitReader & buf)
```

Defined in src/base/include/icy/buffer.h:423

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`BitReader`](#bitreader)  | Constructs a `[BitReader](#bitreader)` over a raw byte array. |
|  | [`BitReader`](#bitreader)  | Constructs a `[BitReader](#bitreader)` over a `[Buffer](#buffer-2)`. |
|  | [`BitReader`](#bitreader)  | Constructs a `[BitReader](#bitreader)` over a `[ConstBuffer](#constbuffer)`. |
| `void` | [`get`](#get)  | Reads a value from the [BitReader](#bitreader). Returns false if there isn't enough data left for the specified type. Throws a std::out_of_range exception if reading past the limit. Reads `len` raw bytes into `val`. Throws `std::out_of_range` if insufficient data remains. |
| `void` | [`get`](#get)  | Reads `len` bytes and appends them to `val`. Throws `std::out_of_range` if insufficient data remains. |
| `void` | [`getU8`](#getu8)  | Reads an unsigned 8-bit integer. Throws `std::out_of_range` if insufficient data remains. |
| `void` | [`getU16`](#getu16)  | Reads an unsigned 16-bit integer, applying byte-order conversion. Throws `std::out_of_range` if insufficient data remains. |
| `void` | [`getU24`](#getu24)  | Reads an unsigned 24-bit integer into a 32-bit variable, applying byte-order conversion. Throws `std::out_of_range` if insufficient data remains. |
| `void` | [`getU32`](#getu32)  | Reads an unsigned 32-bit integer, applying byte-order conversion. Throws `std::out_of_range` if insufficient data remains. |
| `void` | [`getU64`](#getu64)  | Reads an unsigned 64-bit integer, applying byte-order conversion. Throws `std::out_of_range` if insufficient data remains. |
| `char` | [`peek`](#peek)  | Peeks at the current byte without advancing the position. |
| `uint8_t` | [`peekU8`](#peeku8)  | Peeks at the current byte as a `uint8_t` without advancing the position. |
| `uint16_t` | [`peekU16`](#peeku16)  | Peeks at the next two bytes as a `uint16_t` without advancing the position. |
| `uint32_t` | [`peekU24`](#peeku24)  | Peeks at the next three bytes as a `uint32_t` without advancing the position. |
| `uint32_t` | [`peekU32`](#peeku32)  | Peeks at the next four bytes as a `uint32_t` without advancing the position. |
| `uint64_t` | [`peekU64`](#peeku64)  | Peeks data from the [BitReader](#bitreader). -1 is returned if reading past boundary. |
| `size_t` | [`skipToChar`](#skiptochar)  | Advances the position until the given character is found, stopping before it. |
| `size_t` | [`skipWhitespace`](#skipwhitespace)  | Advances the position past any leading space characters. |
| `size_t` | [`skipToNextLine`](#skiptonextline)  | Advances the position past the end of the current line (past the newline). |
| `size_t` | [`skipNextWord`](#skipnextword)  | Advances the position past the next whitespace-delimited word. |
| `size_t` | [`readNextWord`](#readnextword)  | Reads the next whitespace-delimited word into `val`. |
| `size_t` | [`readNextNumber`](#readnextnumber)  | Reads the next whitespace-delimited decimal number into `val`. |
| `size_t` | [`readLine`](#readline)  | Reads bytes up to (but not including) the next newline into `val`. |
| `size_t` | [`readToNext`](#readtonext)  | Reads bytes up to (but not including) the next occurrence of `c` into `val`. |
| `void` | [`seek`](#seek)  | Set position pointer to absolute position. Throws a std::out_of_range exception if the value exceeds the limit. |
| `void` | [`skip`](#skip)  | Set position pointer to relative position. Throws a std::out_of_range exception if the value exceeds the limit. |
| `size_t` | [`limit`](#limit) `const` `nodiscard` | Returns the read limit. |
| `size_t` | [`position`](#position) `const` `inline` `nodiscard` | Returns the current read position. |
| `size_t` | [`available`](#available) `const` `nodiscard` | Returns the number of elements between the current position and the limit. |
| `const char *` | [`begin`](#begin) `const` `inline` `nodiscard` | Returns a pointer to the start of the buffer. |
| `const char *` | [`current`](#current) `const` `inline` `nodiscard` | Returns a pointer to the current read position. |
| `ByteOrder` | [`order`](#order) `const` `inline` | Returns the byte order used for multi-byte integer reads. |
| `std::string` | [`toString`](#tostring)  | Returns the remaining unread bytes as a `std::string`. |

---

{#bitreader}

#### BitReader

```cpp
BitReader(const char * bytes, size_t size, ByteOrder order = ByteOrder::Network)
```

Defined in src/base/include/icy/buffer.h:280

Constructs a `[BitReader](#bitreader)` over a raw byte array. 
#### Parameters
* `bytes` Pointer to the start of the data. Must remain valid for the lifetime of the reader. 

* `size` Number of bytes available for reading. 

* `order` Byte order used when reading multi-byte integer types.

---

{#bitreader}

#### BitReader

```cpp
BitReader(const Buffer & buf, ByteOrder order = ByteOrder::Network)
```

Defined in src/base/include/icy/buffer.h:285

Constructs a `[BitReader](#bitreader)` over a `[Buffer](#buffer-2)`. 
#### Parameters
* `buf` Source buffer. Must remain valid for the lifetime of the reader. 

* `order` Byte order used when reading multi-byte integer types.

---

{#bitreader}

#### BitReader

```cpp
BitReader(const ConstBuffer & pod, ByteOrder order = ByteOrder::Network)
```

Defined in src/base/include/icy/buffer.h:290

Constructs a `[BitReader](#bitreader)` over a `[ConstBuffer](#constbuffer)`. 
#### Parameters
* `pod` Source const buffer. Must remain valid for the lifetime of the reader. 

* `order` Byte order used when reading multi-byte integer types.

---

{#get}

#### get

```cpp
void get(char * val, size_t len)
```

Defined in src/base/include/icy/buffer.h:299

Reads a value from the [BitReader](#bitreader). Returns false if there isn't enough data left for the specified type. Throws a std::out_of_range exception if reading past the limit. Reads `len` raw bytes into `val`. Throws `std::out_of_range` if insufficient data remains. 
#### Parameters
* `val` Destination buffer; must have capacity of at least `len` bytes. 

* `len` Number of bytes to read.

---

{#get}

#### get

```cpp
void get(std::string & val, size_t len)
```

Defined in src/base/include/icy/buffer.h:304

Reads `len` bytes and appends them to `val`. Throws `std::out_of_range` if insufficient data remains. 
#### Parameters
* `val` String to append the read bytes to. 

* `len` Number of bytes to read.

---

{#getu8}

#### getU8

```cpp
void getU8(uint8_t & val)
```

Defined in src/base/include/icy/buffer.h:308

Reads an unsigned 8-bit integer. Throws `std::out_of_range` if insufficient data remains. 
#### Parameters
* `val` Output parameter receiving the read value.

---

{#getu16}

#### getU16

```cpp
void getU16(uint16_t & val)
```

Defined in src/base/include/icy/buffer.h:312

Reads an unsigned 16-bit integer, applying byte-order conversion. Throws `std::out_of_range` if insufficient data remains. 
#### Parameters
* `val` Output parameter receiving the read value.

---

{#getu24}

#### getU24

```cpp
void getU24(uint32_t & val)
```

Defined in src/base/include/icy/buffer.h:316

Reads an unsigned 24-bit integer into a 32-bit variable, applying byte-order conversion. Throws `std::out_of_range` if insufficient data remains. 
#### Parameters
* `val` Output parameter receiving the read value.

---

{#getu32}

#### getU32

```cpp
void getU32(uint32_t & val)
```

Defined in src/base/include/icy/buffer.h:320

Reads an unsigned 32-bit integer, applying byte-order conversion. Throws `std::out_of_range` if insufficient data remains. 
#### Parameters
* `val` Output parameter receiving the read value.

---

{#getu64}

#### getU64

```cpp
void getU64(uint64_t & val)
```

Defined in src/base/include/icy/buffer.h:324

Reads an unsigned 64-bit integer, applying byte-order conversion. Throws `std::out_of_range` if insufficient data remains. 
#### Parameters
* `val` Output parameter receiving the read value.

---

{#peek}

#### peek

```cpp
char peek()
```

Defined in src/base/include/icy/buffer.h:328

Peeks at the current byte without advancing the position. 
#### Returns
Current byte as `char`, or 0 if at the end of the buffer.

---

{#peeku8}

#### peekU8

```cpp
uint8_t peekU8()
```

Defined in src/base/include/icy/buffer.h:332

Peeks at the current byte as a `uint8_t` without advancing the position. 
#### Returns
Current value, or 0 if at the end of the buffer.

---

{#peeku16}

#### peekU16

```cpp
uint16_t peekU16()
```

Defined in src/base/include/icy/buffer.h:336

Peeks at the next two bytes as a `uint16_t` without advancing the position. 
#### Returns
Current value with byte-order conversion applied, or 0 on failure.

---

{#peeku24}

#### peekU24

```cpp
uint32_t peekU24()
```

Defined in src/base/include/icy/buffer.h:340

Peeks at the next three bytes as a `uint32_t` without advancing the position. 
#### Returns
Current value with byte-order conversion applied, or 0 on failure.

---

{#peeku32}

#### peekU32

```cpp
uint32_t peekU32()
```

Defined in src/base/include/icy/buffer.h:344

Peeks at the next four bytes as a `uint32_t` without advancing the position. 
#### Returns
Current value with byte-order conversion applied, or 0 on failure.

---

{#peeku64}

#### peekU64

```cpp
uint64_t peekU64()
```

Defined in src/base/include/icy/buffer.h:348

Peeks data from the [BitReader](#bitreader). -1 is returned if reading past boundary.

---

{#skiptochar}

#### skipToChar

```cpp
size_t skipToChar(char c)
```

Defined in src/base/include/icy/buffer.h:357

Advances the position until the given character is found, stopping before it. 
#### Parameters
* `c` Character to search for. 

#### Returns
Number of bytes skipped.

---

{#skipwhitespace}

#### skipWhitespace

```cpp
size_t skipWhitespace()
```

Defined in src/base/include/icy/buffer.h:361

Advances the position past any leading space characters. 
#### Returns
Number of bytes skipped.

---

{#skiptonextline}

#### skipToNextLine

```cpp
size_t skipToNextLine()
```

Defined in src/base/include/icy/buffer.h:365

Advances the position past the end of the current line (past the newline). 
#### Returns
Number of bytes skipped including the newline character.

---

{#skipnextword}

#### skipNextWord

```cpp
size_t skipNextWord()
```

Defined in src/base/include/icy/buffer.h:369

Advances the position past the next whitespace-delimited word. 
#### Returns
Number of bytes skipped.

---

{#readnextword}

#### readNextWord

```cpp
size_t readNextWord(std::string & val)
```

Defined in src/base/include/icy/buffer.h:374

Reads the next whitespace-delimited word into `val`. 
#### Parameters
* `val` String to receive the word. 

#### Returns
Number of bytes consumed.

---

{#readnextnumber}

#### readNextNumber

```cpp
size_t readNextNumber(unsigned int & val)
```

Defined in src/base/include/icy/buffer.h:379

Reads the next whitespace-delimited decimal number into `val`. 
#### Parameters
* `val` Output parameter receiving the parsed unsigned integer. 

#### Returns
Number of bytes consumed.

---

{#readline}

#### readLine

```cpp
size_t readLine(std::string & val)
```

Defined in src/base/include/icy/buffer.h:384

Reads bytes up to (but not including) the next newline into `val`. 
#### Parameters
* `val` String to receive the line content. 

#### Returns
Number of bytes consumed including the newline.

---

{#readtonext}

#### readToNext

```cpp
size_t readToNext(std::string & val, char c)
```

Defined in src/base/include/icy/buffer.h:390

Reads bytes up to (but not including) the next occurrence of `c` into `val`. 
#### Parameters
* `val` String to receive the read bytes. 

* `c` Delimiter character to stop at. 

#### Returns
Number of bytes consumed.

---

{#seek}

#### seek

```cpp
void seek(size_t val)
```

Defined in src/base/include/icy/buffer.h:394

Set position pointer to absolute position. Throws a std::out_of_range exception if the value exceeds the limit.

---

{#skip}

#### skip

```cpp
void skip(size_t size)
```

Defined in src/base/include/icy/buffer.h:398

Set position pointer to relative position. Throws a std::out_of_range exception if the value exceeds the limit.

---

{#limit}

#### limit

`const` `nodiscard`

```cpp
[[nodiscard]] size_t limit() const
```

Defined in src/base/include/icy/buffer.h:401

Returns the read limit.

---

{#position}

#### position

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline size_t position() const
```

Defined in src/base/include/icy/buffer.h:404

Returns the current read position.

---

{#available}

#### available

`const` `nodiscard`

```cpp
[[nodiscard]] size_t available() const
```

Defined in src/base/include/icy/buffer.h:408

Returns the number of elements between the current position and the limit.

---

{#begin}

#### begin

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline const char * begin() const
```

Defined in src/base/include/icy/buffer.h:411

Returns a pointer to the start of the buffer.

---

{#current}

#### current

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline const char * current() const
```

Defined in src/base/include/icy/buffer.h:414

Returns a pointer to the current read position.

---

{#order}

#### order

`const` `inline`

```cpp
inline ByteOrder order() const
```

Defined in src/base/include/icy/buffer.h:417

Returns the byte order used for multi-byte integer reads.

---

{#tostring}

#### toString

```cpp
std::string toString()
```

Defined in src/base/include/icy/buffer.h:421

Returns the remaining unread bytes as a `std::string`. 
#### Returns
String containing bytes from the current position to the end.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `size_t` | [`_position`](#_position)  |  |
| `size_t` | [`_limit`](#_limit)  |  |
| `ByteOrder` | [`_order`](#_order)  |  |
| `const char *` | [`_bytes`](#_bytes)  |  |

---

{#_position}

#### _position

```cpp
size_t _position
```

Defined in src/base/include/icy/buffer.h:431

---

{#_limit}

#### _limit

```cpp
size_t _limit
```

Defined in src/base/include/icy/buffer.h:432

---

{#_order}

#### _order

```cpp
ByteOrder _order
```

Defined in src/base/include/icy/buffer.h:433

---

{#_bytes}

#### _bytes

```cpp
const char * _bytes
```

Defined in src/base/include/icy/buffer.h:434

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`init`](#init)  |  |

---

{#init}

#### init

```cpp
void init(const char * bytes, size_t size, ByteOrder order)
```

Defined in src/base/include/icy/buffer.h:429

{#bitwriter}

## BitWriter

```cpp
#include <icy/buffer.h>
```

```cpp
class BitWriter
```

Defined in src/base/include/icy/buffer.h:450

> **Subclassed by:** [`DynamicBitWriter`](#dynamicbitwriter)

Class for reading/writing binary streams.

Note that when using the constructor with the [Buffer](#buffer-2) reference as an argument, the writer will dynamically expand the given buffer when writing passed the buffer capacity. All other cases will throw a std::out_of_range error when writing past the buffer capacity.

### Friends

| Name | Description |
|------|-------------|
| [`operator<<`](#operator) `inline` |  |

---

{#operator}

#### operator<<

`inline`

```cpp
friend inline std::ostream & operator<<(std::ostream & stream, const BitWriter & wr)
```

Defined in src/base/include/icy/buffer.h:574

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`BitWriter`](#bitwriter)  | Constructs a `[BitWriter](#bitwriter)` over a raw byte array with a fixed capacity. |
|  | [`BitWriter`](#bitwriter)  | Constructs a `[BitWriter](#bitwriter)` backed by a `[Buffer](#buffer-2)`. Writes are bounded by the buffer's capacity; use `[DynamicBitWriter](#dynamicbitwriter)` for auto-resize. |
|  | [`BitWriter`](#bitwriter)  | Constructs a `[BitWriter](#bitwriter)` over a `[MutableBuffer](#mutablebuffer)` with a fixed capacity. |
| `void` | [`put`](#put) `virtual` | Append bytes to the buffer. Throws a `std::out_of_range` exception if reading past the limit. |
| `void` | [`put`](#put)  | Appends the contents of a string. Throws `std::out_of_range` if capacity is exceeded. |
| `void` | [`putU8`](#putu8)  | Appends an unsigned 8-bit integer. Throws `std::out_of_range` if capacity is exceeded. |
| `void` | [`putU16`](#putu16)  | Appends an unsigned 16-bit integer with byte-order conversion. Throws `std::out_of_range` if capacity is exceeded. |
| `void` | [`putU24`](#putu24)  | Appends the low 24 bits of a 32-bit integer with byte-order conversion. Throws `std::out_of_range` if capacity is exceeded. |
| `void` | [`putU32`](#putu32)  | Appends an unsigned 32-bit integer with byte-order conversion. Throws `std::out_of_range` if capacity is exceeded. |
| `void` | [`putU64`](#putu64)  | Appends an unsigned 64-bit integer with byte-order conversion. Throws `std::out_of_range` if capacity is exceeded. |
| `bool` | [`update`](#update) `virtual` | Update a byte range. Throws a `std::out_of_range` exception if reading past the limit. |
| `bool` | [`update`](#update)  | Overwrites a previously written string at the given absolute position. |
| `bool` | [`updateU8`](#updateu8)  | Overwrites a `uint8_t` at the given absolute position. |
| `bool` | [`updateU16`](#updateu16)  | Overwrites a `uint16_t` at the given absolute position, with byte-order conversion. |
| `bool` | [`updateU24`](#updateu24)  | Overwrites 3 bytes (low 24 bits of `val`) at the given absolute position, with byte-order conversion. |
| `bool` | [`updateU32`](#updateu32)  | Overwrites a `uint32_t` at the given absolute position, with byte-order conversion. |
| `bool` | [`updateU64`](#updateu64)  | Overwrites a `uint64_t` at the given absolute position, with byte-order conversion. |
| `void` | [`seek`](#seek)  | Set position pointer to absolute position. Throws a `std::out_of_range` exception if the value exceeds the limit. |
| `void` | [`skip`](#skip)  | Set position pointer to relative position. Throws a `std::out_of_range` exception if the value exceeds the limit. |
| `size_t` | [`limit`](#limit) `const` `nodiscard` | Returns the write limit. |
| `size_t` | [`position`](#position) `const` `inline` `nodiscard` | Returns the current write position. |
| `size_t` | [`available`](#available) `const` `nodiscard` | Returns the number of elements between the current write position and the limit. |
| `char *` | [`begin`](#begin) `inline` `nodiscard` | Returns a pointer to the start of the write buffer. |
| `char *` | [`current`](#current) `inline` `nodiscard` | Returns a pointer to the current write position. |
| `const char *` | [`begin`](#begin) `const` `inline` `nodiscard` | Returns a const pointer to the start of the write buffer. |
| `const char *` | [`current`](#current) `const` `inline` `nodiscard` | Returns a const pointer to the current write position. |
| `ByteOrder` | [`order`](#order) `const` `inline` | Returns the byte order used for multi-byte integer writes. |
| `std::string` | [`toString`](#tostring)  | Returns all bytes written so far as a `std::string`. |

---

{#bitwriter}

#### BitWriter

```cpp
BitWriter(char * bytes, size_t size, ByteOrder order = ByteOrder::Network)
```

Defined in src/base/include/icy/buffer.h:457

Constructs a `[BitWriter](#bitwriter)` over a raw byte array with a fixed capacity. 
#### Parameters
* `bytes` Pointer to the writable buffer. Must remain valid for the writer's lifetime. 

* `size` Capacity of the buffer in bytes. 

* `order` Byte order used for multi-byte integer writes.

---

{#bitwriter}

#### BitWriter

```cpp
BitWriter(Buffer & buf, ByteOrder order = ByteOrder::Network)
```

Defined in src/base/include/icy/buffer.h:463

Constructs a `[BitWriter](#bitwriter)` backed by a `[Buffer](#buffer-2)`. Writes are bounded by the buffer's capacity; use `[DynamicBitWriter](#dynamicbitwriter)` for auto-resize. 
#### Parameters
* `buf` Source buffer. Must remain valid for the writer's lifetime. 

* `order` Byte order used for multi-byte integer writes.

---

{#bitwriter}

#### BitWriter

```cpp
BitWriter(MutableBuffer & pod, ByteOrder order = ByteOrder::Network)
```

Defined in src/base/include/icy/buffer.h:468

Constructs a `[BitWriter](#bitwriter)` over a `[MutableBuffer](#mutablebuffer)` with a fixed capacity. 
#### Parameters
* `pod` Source mutable buffer. Must remain valid for the writer's lifetime. 

* `order` Byte order used for multi-byte integer writes.

---

{#put}

#### put

`virtual`

```cpp
virtual void put(const char * val, size_t len)
```

Defined in src/base/include/icy/buffer.h:473

Append bytes to the buffer. Throws a `std::out_of_range` exception if reading past the limit.

---

{#put}

#### put

```cpp
void put(const std::string & val)
```

Defined in src/base/include/icy/buffer.h:476

Appends the contents of a string. Throws `std::out_of_range` if capacity is exceeded. 
#### Parameters
* `val` String whose bytes are appended.

---

{#putu8}

#### putU8

```cpp
void putU8(uint8_t val)
```

Defined in src/base/include/icy/buffer.h:480

Appends an unsigned 8-bit integer. Throws `std::out_of_range` if capacity is exceeded. 
#### Parameters
* `val` Value to write.

---

{#putu16}

#### putU16

```cpp
void putU16(uint16_t val)
```

Defined in src/base/include/icy/buffer.h:484

Appends an unsigned 16-bit integer with byte-order conversion. Throws `std::out_of_range` if capacity is exceeded. 
#### Parameters
* `val` Value to write.

---

{#putu24}

#### putU24

```cpp
void putU24(uint32_t val)
```

Defined in src/base/include/icy/buffer.h:488

Appends the low 24 bits of a 32-bit integer with byte-order conversion. Throws `std::out_of_range` if capacity is exceeded. 
#### Parameters
* `val` Value to write (only the lower 3 bytes are written).

---

{#putu32}

#### putU32

```cpp
void putU32(uint32_t val)
```

Defined in src/base/include/icy/buffer.h:492

Appends an unsigned 32-bit integer with byte-order conversion. Throws `std::out_of_range` if capacity is exceeded. 
#### Parameters
* `val` Value to write.

---

{#putu64}

#### putU64

```cpp
void putU64(uint64_t val)
```

Defined in src/base/include/icy/buffer.h:496

Appends an unsigned 64-bit integer with byte-order conversion. Throws `std::out_of_range` if capacity is exceeded. 
#### Parameters
* `val` Value to write.

---

{#update}

#### update

`virtual`

```cpp
virtual bool update(const char * val, size_t len, size_t pos)
```

Defined in src/base/include/icy/buffer.h:500

Update a byte range. Throws a `std::out_of_range` exception if reading past the limit.

---

{#update}

#### update

```cpp
bool update(const std::string & val, size_t pos)
```

Defined in src/base/include/icy/buffer.h:505

Overwrites a previously written string at the given absolute position. 
#### Parameters
* `val` String to write. 

* `pos` Absolute byte offset to overwrite at. 

#### Returns
True on success, false if the range exceeds available space.

---

{#updateu8}

#### updateU8

```cpp
bool updateU8(uint8_t val, size_t pos)
```

Defined in src/base/include/icy/buffer.h:511

Overwrites a `uint8_t` at the given absolute position. 
#### Parameters
* `val` Value to write. 

* `pos` Absolute byte offset to overwrite at. 

#### Returns
True on success, false if the range exceeds available space.

---

{#updateu16}

#### updateU16

```cpp
bool updateU16(uint16_t val, size_t pos)
```

Defined in src/base/include/icy/buffer.h:517

Overwrites a `uint16_t` at the given absolute position, with byte-order conversion. 
#### Parameters
* `val` Value to write. 

* `pos` Absolute byte offset to overwrite at. 

#### Returns
True on success, false if the range exceeds available space.

---

{#updateu24}

#### updateU24

```cpp
bool updateU24(uint32_t val, size_t pos)
```

Defined in src/base/include/icy/buffer.h:523

Overwrites 3 bytes (low 24 bits of `val`) at the given absolute position, with byte-order conversion. 
#### Parameters
* `val` Value to write. 

* `pos` Absolute byte offset to overwrite at. 

#### Returns
True on success, false if the range exceeds available space.

---

{#updateu32}

#### updateU32

```cpp
bool updateU32(uint32_t val, size_t pos)
```

Defined in src/base/include/icy/buffer.h:529

Overwrites a `uint32_t` at the given absolute position, with byte-order conversion. 
#### Parameters
* `val` Value to write. 

* `pos` Absolute byte offset to overwrite at. 

#### Returns
True on success, false if the range exceeds available space.

---

{#updateu64}

#### updateU64

```cpp
bool updateU64(uint64_t val, size_t pos)
```

Defined in src/base/include/icy/buffer.h:535

Overwrites a `uint64_t` at the given absolute position, with byte-order conversion. 
#### Parameters
* `val` Value to write. 

* `pos` Absolute byte offset to overwrite at. 

#### Returns
True on success, false if the range exceeds available space.

---

{#seek}

#### seek

```cpp
void seek(size_t val)
```

Defined in src/base/include/icy/buffer.h:539

Set position pointer to absolute position. Throws a `std::out_of_range` exception if the value exceeds the limit.

---

{#skip}

#### skip

```cpp
void skip(size_t size)
```

Defined in src/base/include/icy/buffer.h:543

Set position pointer to relative position. Throws a `std::out_of_range` exception if the value exceeds the limit.

---

{#limit}

#### limit

`const` `nodiscard`

```cpp
[[nodiscard]] size_t limit() const
```

Defined in src/base/include/icy/buffer.h:546

Returns the write limit.

---

{#position}

#### position

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline size_t position() const
```

Defined in src/base/include/icy/buffer.h:549

Returns the current write position.

---

{#available}

#### available

`const` `nodiscard`

```cpp
[[nodiscard]] size_t available() const
```

Defined in src/base/include/icy/buffer.h:553

Returns the number of elements between the current write position and the limit.

---

{#begin}

#### begin

`inline` `nodiscard`

```cpp
[[nodiscard]] inline char * begin()
```

Defined in src/base/include/icy/buffer.h:556

Returns a pointer to the start of the write buffer.

---

{#current}

#### current

`inline` `nodiscard`

```cpp
[[nodiscard]] inline char * current()
```

Defined in src/base/include/icy/buffer.h:559

Returns a pointer to the current write position.

---

{#begin}

#### begin

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline const char * begin() const
```

Defined in src/base/include/icy/buffer.h:562

Returns a const pointer to the start of the write buffer.

---

{#current}

#### current

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline const char * current() const
```

Defined in src/base/include/icy/buffer.h:565

Returns a const pointer to the current write position.

---

{#order}

#### order

`const` `inline`

```cpp
inline ByteOrder order() const
```

Defined in src/base/include/icy/buffer.h:568

Returns the byte order used for multi-byte integer writes.

---

{#tostring}

#### toString

```cpp
std::string toString()
```

Defined in src/base/include/icy/buffer.h:572

Returns all bytes written so far as a `std::string`. 
#### Returns
String containing bytes from the start of the buffer up to the current position.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `size_t` | [`_position`](#_position)  |  |
| `size_t` | [`_limit`](#_limit)  |  |
| `ByteOrder` | [`_order`](#_order)  |  |
| `char *` | [`_bytes`](#_bytes)  |  |

---

{#_position}

#### _position

```cpp
size_t _position
```

Defined in src/base/include/icy/buffer.h:582

---

{#_limit}

#### _limit

```cpp
size_t _limit
```

Defined in src/base/include/icy/buffer.h:583

---

{#_order}

#### _order

```cpp
ByteOrder _order
```

Defined in src/base/include/icy/buffer.h:584

---

{#_bytes}

#### _bytes

```cpp
char * _bytes
```

Defined in src/base/include/icy/buffer.h:585

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`init`](#init) `virtual` |  |

---

{#init}

#### init

`virtual`

```cpp
virtual void init(char * bytes, size_t size, ByteOrder order)
```

Defined in src/base/include/icy/buffer.h:580

{#dynamicbitwriter}

## DynamicBitWriter

```cpp
#include <icy/buffer.h>
```

```cpp
class DynamicBitWriter
```

Defined in src/base/include/icy/buffer.h:596

> **Inherits:** [`BitWriter`](#bitwriter)

Class for reading/writing dynamically resizable binary streams.

Note that when using the constructor with the [Buffer](#buffer-2) reference as an argument, the writer will dynamically expand the given buffer when writing passed the buffer capacity. All other cases will throw a std::out_of_range error when writing past the buffer capacity.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`DynamicBitWriter`](#dynamicbitwriter)  | Constructs a `[DynamicBitWriter](#dynamicbitwriter)` backed by a dynamically resizable `[Buffer](#buffer-2)`. The buffer is expanded automatically as data is written. |
|  | [`DynamicBitWriter`](#dynamicbitwriter)  | Constructs a `[DynamicBitWriter](#dynamicbitwriter)` that inserts data starting at a specific iterator position. |
| `void` | [`put`](#put) `virtual` `override` | Append bytes to the buffer. Throws a `std::out_of_range` exception if reading past the limit. |
| `bool` | [`update`](#update) `virtual` `override` | Update a byte range. Throws a `std::out_of_range` exception if reading past the limit. |

---

{#dynamicbitwriter}

#### DynamicBitWriter

```cpp
DynamicBitWriter(Buffer & buf, ByteOrder order = ByteOrder::Network)
```

Defined in src/base/include/icy/buffer.h:603

Constructs a `[DynamicBitWriter](#dynamicbitwriter)` backed by a dynamically resizable `[Buffer](#buffer-2)`. The buffer is expanded automatically as data is written. 
#### Parameters
* `buf` [Buffer](#buffer-2) to write into; expanded as needed. 

* `order` Byte order used for multi-byte integer writes.

---

{#dynamicbitwriter}

#### DynamicBitWriter

```cpp
DynamicBitWriter(Buffer & buf, Buffer::iterator offset, ByteOrder order = ByteOrder::Network)
```

Defined in src/base/include/icy/buffer.h:609

Constructs a `[DynamicBitWriter](#dynamicbitwriter)` that inserts data starting at a specific iterator position. 
#### Parameters
* `buf` [Buffer](#buffer-2) to write into; expanded as needed. 

* `offset` Iterator into `buf` indicating the insertion start point. 

* `order` Byte order used for multi-byte integer writes.

---

{#put}

#### put

`virtual` `override`

```cpp
virtual void put(const char * val, size_t len) override
```

Defined in src/base/include/icy/buffer.h:614

Append bytes to the buffer. Throws a `std::out_of_range` exception if reading past the limit.

---

{#update}

#### update

`virtual` `override`

```cpp
virtual bool update(const char * val, size_t len, size_t pos) override
```

Defined in src/base/include/icy/buffer.h:618

Update a byte range. Throws a `std::out_of_range` exception if reading past the limit.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `Buffer &` | [`_buffer`](#_buffer)  |  |
| `size_t` | [`_offset`](#_offset)  |  |

---

{#_buffer}

#### _buffer

```cpp
Buffer & _buffer
```

Defined in src/base/include/icy/buffer.h:621

---

{#_offset}

#### _offset

```cpp
size_t _offset
```

Defined in src/base/include/icy/buffer.h:622

{#logwriter}

## LogWriter

```cpp
#include <icy/logger.h>
```

```cpp
class LogWriter
```

Defined in src/base/include/icy/logger.h:104

> **Subclassed by:** [`AsyncLogWriter`](#asynclogwriter)

Log output stream writer.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`LogWriter`](#logwriter)  |  |
| `void` | [`write`](#write) `virtual` | Writes the given log message stream. |

---

{#logwriter}

#### LogWriter

```cpp
LogWriter()
```

Defined in src/base/include/icy/logger.h:107

---

{#write}

#### write

`virtual`

```cpp
virtual void write(std::unique_ptr< LogStream > stream)
```

Defined in src/base/include/icy/logger.h:111

Writes the given log message stream.

{#asynclogwriter}

## AsyncLogWriter

```cpp
#include <icy/logger.h>
```

```cpp
class AsyncLogWriter
```

Defined in src/base/include/icy/logger.h:121

> **Inherits:** [`LogWriter`](#logwriter), [`Runnable`](#runnable)

[Thread](#thread) based log output stream writer.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`AsyncLogWriter`](#asynclogwriter)  |  |
| `void` | [`write`](#write) `virtual` `override` | Queues the given log message stream. |
| `void` | [`flush`](#flush)  | Flushes queued messages. |
| `void` | [`run`](#run) `virtual` `override` | Writes queued messages asynchronously. |
| `void` | [`clear`](#clear)  | Clears all queued messages. |

---

{#asynclogwriter}

#### AsyncLogWriter

```cpp
AsyncLogWriter()
```

Defined in src/base/include/icy/logger.h:125

---

{#write}

#### write

`virtual` `override`

```cpp
virtual void write(std::unique_ptr< LogStream > stream) override
```

Defined in src/base/include/icy/logger.h:129

Queues the given log message stream.

---

{#flush}

#### flush

```cpp
void flush()
```

Defined in src/base/include/icy/logger.h:132

Flushes queued messages.

---

{#run}

#### run

`virtual` `override`

```cpp
virtual void run() override
```

Defined in src/base/include/icy/logger.h:135

Writes queued messages asynchronously.

---

{#clear}

#### clear

```cpp
void clear()
```

Defined in src/base/include/icy/logger.h:138

Clears all queued messages.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `Thread` | [`_thread`](#_thread)  |  |
| `std::deque< std::unique_ptr< LogStream > >` | [`_pending`](#_pending)  |  |
| `std::mutex` | [`_mutex`](#_mutex)  |  |

---

{#_thread}

#### _thread

```cpp
Thread _thread
```

Defined in src/base/include/icy/logger.h:143

---

{#_pending}

#### _pending

```cpp
std::deque< std::unique_ptr< LogStream > > _pending
```

Defined in src/base/include/icy/logger.h:144

---

{#_mutex}

#### _mutex

```cpp
std::mutex _mutex
```

Defined in src/base/include/icy/logger.h:145

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `bool` | [`writeNext`](#writenext)  |  |

---

{#writenext}

#### writeNext

```cpp
bool writeNext()
```

Defined in src/base/include/icy/logger.h:141

{#logger}

## Logger

```cpp
#include <icy/logger.h>
```

```cpp
class Logger
```

Defined in src/base/include/icy/logger.h:155

[Logger](#logger) class.

### Friends

| Name | Description |
|------|-------------|
| [`Singleton< Logger >`](#singletonlogger)  |  |
| [`Thread`](#thread)  |  |

---

{#singletonlogger}

#### Singleton< Logger >

```cpp
friend class Singleton< Logger >
```

Defined in src/base/include/icy/logger.h:203

---

{#thread}

#### Thread

```cpp
friend class Thread
```

Defined in src/base/include/icy/logger.h:208

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Logger`](#logger)  |  |
| `void` | [`add`](#add)  | Adds the given log channel. Takes ownership. |
| `void` | [`remove`](#remove)  | Removes the given log channel by name. |
| `LogChannel *` | [`get`](#get) `const` | Returns the specified log channel. Throws an exception if the channel doesn't exist. |
| `void` | [`setDefault`](#setdefault)  | Sets the default log to the specified log channel. |
| `void` | [`setWriter`](#setwriter)  | Sets the log writer instance. Takes ownership. |
| `LogChannel *` | [`getDefault`](#getdefault) `const` | Returns the default log channel, or the nullptr channel if no default channel has been set. |
| `void` | [`write`](#write)  | Writes the given message to the default log channel. The message will be copied. |
| `void` | [`write`](#write)  | Writes the given message to the default log channel. |

---

{#logger}

#### Logger

```cpp
Logger()
```

Defined in src/base/include/icy/logger.h:158

---

{#add}

#### add

```cpp
void add(std::unique_ptr< LogChannel > channel)
```

Defined in src/base/include/icy/logger.h:172

Adds the given log channel. Takes ownership.

---

{#remove}

#### remove

```cpp
void remove(std::string_view name)
```

Defined in src/base/include/icy/logger.h:175

Removes the given log channel by name.

---

{#get}

#### get

`const`

```cpp
LogChannel * get(std::string_view name, bool whiny = true) const
```

Defined in src/base/include/icy/logger.h:179

Returns the specified log channel. Throws an exception if the channel doesn't exist.

---

{#setdefault}

#### setDefault

```cpp
void setDefault(std::string_view name)
```

Defined in src/base/include/icy/logger.h:182

Sets the default log to the specified log channel.

---

{#setwriter}

#### setWriter

```cpp
void setWriter(std::unique_ptr< LogWriter > writer)
```

Defined in src/base/include/icy/logger.h:185

Sets the log writer instance. Takes ownership.

---

{#getdefault}

#### getDefault

`const`

```cpp
LogChannel * getDefault() const
```

Defined in src/base/include/icy/logger.h:189

Returns the default log channel, or the nullptr channel if no default channel has been set.

---

{#write}

#### write

```cpp
void write(const LogStream & stream)
```

Defined in src/base/include/icy/logger.h:193

Writes the given message to the default log channel. The message will be copied.

---

{#write}

#### write

```cpp
void write(std::unique_ptr< LogStream > stream)
```

Defined in src/base/include/icy/logger.h:196

Writes the given message to the default log channel.

### Public Static Methods

| Return | Name | Description |
|--------|------|-------------|
| `Logger &` | [`instance`](#instance) `static` | Returns the default logger singleton. [Logger](#logger) instances may be created separately as needed. |
| `void` | [`setInstance`](#setinstance) `static` | Sets the default logger singleton instance. |
| `void` | [`destroy`](#destroy) `static` | Destroys the default logger singleton instance. |

---

{#instance}

#### instance

`static`

```cpp
static Logger & instance()
```

Defined in src/base/include/icy/logger.h:163

Returns the default logger singleton. [Logger](#logger) instances may be created separately as needed.

---

{#setinstance}

#### setInstance

`static`

```cpp
static void setInstance(Logger * logger, bool freeExisting = true)
```

Defined in src/base/include/icy/logger.h:166

Sets the default logger singleton instance.

---

{#destroy}

#### destroy

`static`

```cpp
static void destroy()
```

Defined in src/base/include/icy/logger.h:169

Destroys the default logger singleton instance.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::mutex` | [`_mutex`](#_mutex)  |  |
| `LogChannelMap` | [`_channels`](#_channels)  |  |
| `LogChannel *` | [`_defaultChannel`](#_defaultchannel)  |  |
| `std::unique_ptr< LogWriter >` | [`_writer`](#_writer)  |  |

---

{#_mutex}

#### _mutex

```cpp
std::mutex _mutex
```

Defined in src/base/include/icy/logger.h:210

---

{#_channels}

#### _channels

```cpp
LogChannelMap _channels
```

Defined in src/base/include/icy/logger.h:211

---

{#_defaultchannel}

#### _defaultChannel

```cpp
LogChannel * _defaultChannel
```

Defined in src/base/include/icy/logger.h:212

---

{#_writer}

#### _writer

```cpp
std::unique_ptr< LogWriter > _writer
```

Defined in src/base/include/icy/logger.h:213

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Logger`](#logger)  | NonCopyable and NonMovable. |
|  | [`Logger`](#logger)  | Deleted constructor. |

---

{#logger}

#### Logger

```cpp
Logger(const Logger &) = delete
```

Defined in src/base/include/icy/logger.h:200

NonCopyable and NonMovable.

---

{#logger}

#### Logger

```cpp
Logger(Logger &&) = delete
```

Defined in src/base/include/icy/logger.h:202

Deleted constructor.

{#logchannel}

## LogChannel

```cpp
#include <icy/logger.h>
```

```cpp
class LogChannel
```

Defined in src/base/include/icy/logger.h:346

> **Subclassed by:** [`ConsoleChannel`](#consolechannel), [`FileChannel`](#filechannel), [`RotatingFileChannel`](#rotatingfilechannel)

Named log output channel with configurable severity level and formatting.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`LogChannel`](#logchannel)  |  |
| `void` | [`write`](#write) `virtual` | Writes a log stream entry to this channel. |
| `void` | [`write`](#write) `virtual` | Writes a plain message to this channel. |
| `void` | [`format`](#format) `virtual` | Formats a log stream entry into the given output stream. |
| `std::string` | [`name`](#name) `const` `inline` | Returns the channel name. |
| `Level` | [`level`](#level) `const` `inline` | Returns the minimum severity level. |
| `std::string` | [`timeFormat`](#timeformat) `const` `inline` | Returns the timestamp format string. |
| `void` | [`setLevel`](#setlevel) `inline` | Sets the minimum severity level. |
| `void` | [`setTimeFormat`](#settimeformat) `inline` | Sets the timestamp format string. |
| `void` | [`setFilter`](#setfilter) `inline` | Sets a realm filter; only messages whose realm matches are written. |

---

{#logchannel}

#### LogChannel

```cpp
LogChannel(std::string name, Level level = Level::Debug, std::string timeFormat = "%H:%M:%S")
```

Defined in src/base/include/icy/logger.h:352

#### Parameters
* `name` Unique channel name. 

* `level` Minimum severity level; messages below this level are dropped. 

* `timeFormat` strftime-compatible format string for timestamps.

---

{#write}

#### write

`virtual`

```cpp
virtual void write(const LogStream & stream)
```

Defined in src/base/include/icy/logger.h:358

Writes a log stream entry to this channel. 
#### Parameters
* `stream` The log stream to write.

---

{#write}

#### write

`virtual`

```cpp
virtual void write(std::string message, Level level = Level::Debug, std::string realm = "")
```

Defined in src/base/include/icy/logger.h:364

Writes a plain message to this channel. 
#### Parameters
* `message` Log message text. 

* `level` Severity level for the message. 

* `realm` Optional source realm (e.g. file or class name).

---

{#format}

#### format

`virtual`

```cpp
virtual void format(const LogStream & stream, std::ostream & ost)
```

Defined in src/base/include/icy/logger.h:370

Formats a log stream entry into the given output stream. 
#### Parameters
* `stream` The log stream to format. 

* `ost` The output stream to write the formatted message into.

---

{#name}

#### name

`const` `inline`

```cpp
inline std::string name() const
```

Defined in src/base/include/icy/logger.h:373

Returns the channel name.

---

{#level}

#### level

`const` `inline`

```cpp
inline Level level() const
```

Defined in src/base/include/icy/logger.h:376

Returns the minimum severity level.

---

{#timeformat}

#### timeFormat

`const` `inline`

```cpp
inline std::string timeFormat() const
```

Defined in src/base/include/icy/logger.h:379

Returns the timestamp format string.

---

{#setlevel}

#### setLevel

`inline`

```cpp
inline void setLevel(Level level)
```

Defined in src/base/include/icy/logger.h:383

Sets the minimum severity level. 
#### Parameters
* `level` Messages below this level are dropped.

---

{#settimeformat}

#### setTimeFormat

`inline`

```cpp
inline void setTimeFormat(std::string format)
```

Defined in src/base/include/icy/logger.h:387

Sets the timestamp format string. 
#### Parameters
* `format` strftime-compatible format string.

---

{#setfilter}

#### setFilter

`inline`

```cpp
inline void setFilter(std::string filter)
```

Defined in src/base/include/icy/logger.h:391

Sets a realm filter; only messages whose realm matches are written. 
#### Parameters
* `filter` Realm substring or pattern to match against.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`_name`](#_name)  |  |
| `Level` | [`_level`](#_level)  |  |
| `std::string` | [`_timeFormat`](#_timeformat)  |  |
| `std::string` | [`_filter`](#_filter)  |  |

---

{#_name}

#### _name

```cpp
std::string _name
```

Defined in src/base/include/icy/logger.h:394

---

{#_level}

#### _level

```cpp
Level _level
```

Defined in src/base/include/icy/logger.h:395

---

{#_timeformat}

#### _timeFormat

```cpp
std::string _timeFormat
```

Defined in src/base/include/icy/logger.h:396

---

{#_filter}

#### _filter

```cpp
std::string _filter
```

Defined in src/base/include/icy/logger.h:397

{#consolechannel}

## ConsoleChannel

```cpp
#include <icy/logger.h>
```

```cpp
class ConsoleChannel
```

Defined in src/base/include/icy/logger.h:414

> **Inherits:** [`LogChannel`](#logchannel)

Log channel that writes formatted messages to standard output.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`ConsoleChannel`](#consolechannel)  |  |
| `void` | [`write`](#write) `virtual` `override` | Formats and writes the log stream entry to stdout. Messages below the channel level or filtered by realm are silently dropped. |

---

{#consolechannel}

#### ConsoleChannel

```cpp
ConsoleChannel(std::string name, Level level = Level::Debug, std::string timeFormat = "%H:%M:%S")
```

Defined in src/base/include/icy/logger.h:420

#### Parameters
* `name` Unique channel name. 

* `level` Minimum severity level; messages below this level are dropped. 

* `timeFormat` strftime-compatible format string for timestamps.

---

{#write}

#### write

`virtual` `override`

```cpp
virtual void write(const LogStream & stream) override
```

Defined in src/base/include/icy/logger.h:427

Formats and writes the log stream entry to stdout. Messages below the channel level or filtered by realm are silently dropped. 
#### Parameters
* `stream` The log stream to write.

{#filechannel}

## FileChannel

```cpp
#include <icy/logger.h>
```

```cpp
class FileChannel
```

Defined in src/base/include/icy/logger.h:437

> **Inherits:** [`LogChannel`](#logchannel)

Log channel that writes formatted messages to a file.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`FileChannel`](#filechannel)  |  |
| `void` | [`write`](#write) `virtual` `override` | Formats and writes the log stream entry to the file. Opens the file on first write if not already open. |
| `void` | [`setPath`](#setpath)  | Sets the file path and reopens the file stream. |
| `std::string` | [`path`](#path) `const` | Returns the current log file path. |

---

{#filechannel}

#### FileChannel

```cpp
FileChannel(std::string name, std::string path, Level level = Level::Debug, std::string timeFormat = "%H:%M:%S")
```

Defined in src/base/include/icy/logger.h:444

#### Parameters
* `name` Unique channel name. 

* `path` Path to the output log file. Directories are created if needed. 

* `level` Minimum severity level; messages below this level are dropped. 

* `timeFormat` strftime-compatible format string for timestamps.

---

{#write}

#### write

`virtual` `override`

```cpp
virtual void write(const LogStream & stream) override
```

Defined in src/base/include/icy/logger.h:452

Formats and writes the log stream entry to the file. Opens the file on first write if not already open. 
#### Parameters
* `stream` The log stream to write.

---

{#setpath}

#### setPath

```cpp
void setPath(const std::string & path)
```

Defined in src/base/include/icy/logger.h:456

Sets the file path and reopens the file stream. 
#### Parameters
* `path` Path to the new log file.

---

{#path}

#### path

`const`

```cpp
std::string path() const
```

Defined in src/base/include/icy/logger.h:460

Returns the current log file path. 
#### Returns
Absolute or relative path to the log file.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::ofstream` | [`_fstream`](#_fstream)  |  |
| `std::string` | [`_path`](#_path)  |  |

---

{#_fstream}

#### _fstream

```cpp
std::ofstream _fstream
```

Defined in src/base/include/icy/logger.h:467

---

{#_path}

#### _path

```cpp
std::string _path
```

Defined in src/base/include/icy/logger.h:468

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`open`](#open) `virtual` |  |
| `void` | [`close`](#close) `virtual` |  |

---

{#open}

#### open

`virtual`

```cpp
virtual void open()
```

Defined in src/base/include/icy/logger.h:463

---

{#close}

#### close

`virtual`

```cpp
virtual void close()
```

Defined in src/base/include/icy/logger.h:464

{#rotatingfilechannel}

## RotatingFileChannel

```cpp
#include <icy/logger.h>
```

```cpp
class RotatingFileChannel
```

Defined in src/base/include/icy/logger.h:478

> **Inherits:** [`LogChannel`](#logchannel)

Log channel that writes to time-rotated log files.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`RotatingFileChannel`](#rotatingfilechannel)  |  |
| `void` | [`write`](#write) `virtual` `override` | Formats and writes the log stream entry to the current log file. Rotates the file if the rotation interval has elapsed. |
| `void` | [`rotate`](#rotate) `virtual` | Closes the current log file and opens a new one with a timestamped filename. |
| `std::string` | [`dir`](#dir) `const` `inline` | Returns the directory where log files are written. |
| `std::string` | [`filename`](#filename) `const` `inline` | Returns the filename of the currently open log file. |
| `int` | [`rotationInterval`](#rotationinterval) `const` `inline` | Returns the rotation interval in seconds. |
| `void` | [`setDir`](#setdir) `inline` | Sets the output directory for rotated log files. |
| `void` | [`setExtension`](#setextension) `inline` | Sets the file extension for rotated log files. |
| `void` | [`setRotationInterval`](#setrotationinterval) `inline` | Sets the rotation interval. |

---

{#rotatingfilechannel}

#### RotatingFileChannel

```cpp
RotatingFileChannel(std::string name, std::string dir, Level level = Level::Debug, std::string extension = "log", int rotationInterval = 12 *3600, std::string timeFormat = "%H:%M:%S")
```

Defined in src/base/include/icy/logger.h:487

#### Parameters
* `name` Unique channel name. 

* `dir` Directory in which rotated log files are written. 

* `level` Minimum severity level; messages below this level are dropped. 

* `extension` File extension for log files (without leading dot). 

* `rotationInterval` Seconds between log rotations (default: 12 hours). 

* `timeFormat` strftime-compatible format string for timestamps.

---

{#write}

#### write

`virtual` `override`

```cpp
virtual void write(const LogStream & stream) override
```

Defined in src/base/include/icy/logger.h:498

Formats and writes the log stream entry to the current log file. Rotates the file if the rotation interval has elapsed. 
#### Parameters
* `stream` The log stream to write.

---

{#rotate}

#### rotate

`virtual`

```cpp
virtual void rotate()
```

Defined in src/base/include/icy/logger.h:501

Closes the current log file and opens a new one with a timestamped filename.

---

{#dir}

#### dir

`const` `inline`

```cpp
inline std::string dir() const
```

Defined in src/base/include/icy/logger.h:504

Returns the directory where log files are written.

---

{#filename}

#### filename

`const` `inline`

```cpp
inline std::string filename() const
```

Defined in src/base/include/icy/logger.h:507

Returns the filename of the currently open log file.

---

{#rotationinterval}

#### rotationInterval

`const` `inline`

```cpp
inline int rotationInterval() const
```

Defined in src/base/include/icy/logger.h:510

Returns the rotation interval in seconds.

---

{#setdir}

#### setDir

`inline`

```cpp
inline void setDir(std::string dir)
```

Defined in src/base/include/icy/logger.h:514

Sets the output directory for rotated log files. 
#### Parameters
* `dir` Target directory path.

---

{#setextension}

#### setExtension

`inline`

```cpp
inline void setExtension(std::string ext)
```

Defined in src/base/include/icy/logger.h:518

Sets the file extension for rotated log files. 
#### Parameters
* `ext` Extension without leading dot (e.g. "log").

---

{#setrotationinterval}

#### setRotationInterval

`inline`

```cpp
inline void setRotationInterval(int interval)
```

Defined in src/base/include/icy/logger.h:522

Sets the rotation interval. 
#### Parameters
* `interval` Number of seconds between rotations.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::unique_ptr< std::ofstream >` | [`_fstream`](#_fstream)  |  |
| `std::string` | [`_dir`](#_dir)  |  |
| `std::string` | [`_filename`](#_filename)  |  |
| `std::string` | [`_extension`](#_extension)  |  |
| `int` | [`_rotationInterval`](#_rotationinterval)  | The log rotation interval in seconds. |
| `time_t` | [`_rotatedAt`](#_rotatedat)  | The time the log was last rotated. |

---

{#_fstream}

#### _fstream

```cpp
std::unique_ptr< std::ofstream > _fstream
```

Defined in src/base/include/icy/logger.h:525

---

{#_dir}

#### _dir

```cpp
std::string _dir
```

Defined in src/base/include/icy/logger.h:526

---

{#_filename}

#### _filename

```cpp
std::string _filename
```

Defined in src/base/include/icy/logger.h:527

---

{#_extension}

#### _extension

```cpp
std::string _extension
```

Defined in src/base/include/icy/logger.h:528

---

{#_rotationinterval}

#### _rotationInterval

```cpp
int _rotationInterval
```

Defined in src/base/include/icy/logger.h:529

The log rotation interval in seconds.

---

{#_rotatedat}

#### _rotatedAt

```cpp
time_t _rotatedAt
```

Defined in src/base/include/icy/logger.h:530

The time the log was last rotated.

{#refcounted}

## RefCounted

```cpp
#include <icy/memory.h>
```

```cpp
template<typename T>
class RefCounted
```

Defined in src/base/include/icy/memory.h:57

Base class for intrusive reference counting.

Embeds the refcount in the object itself - no separate control block allocation, no atomic operations. Safe for single-threaded libuv loops.

Usage: inherit from RefCounted<YourClass>, then use [IntrusivePtr<YourClass>](#intrusiveptr) instead of std::shared_ptr<YourClass>.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`RefCounted`](#refcounted)  | Defaulted constructor. |
|  | [`RefCounted`](#refcounted) `inline` `noexcept` |  |
| `void` | [`addRef`](#addref) `const` `inline` `noexcept` | Increments the reference count. Called by [IntrusivePtr](#intrusiveptr) on acquisition. |
| `bool` | [`releaseRef`](#releaseref) `const` `inline` `noexcept` | Decrements the reference count. |
| `int` | [`refCount`](#refcount) `const` `inline` `nodiscard` `noexcept` | Returns the current reference count. |

---

{#refcounted}

#### RefCounted

```cpp
RefCounted() = default
```

Defined in src/base/include/icy/memory.h:60

Defaulted constructor.

---

{#refcounted}

#### RefCounted

`inline` `noexcept`

```cpp
inline RefCounted(const RefCounted &) noexcept
```

Defined in src/base/include/icy/memory.h:61

---

{#addref}

#### addRef

`const` `inline` `noexcept`

```cpp
inline void addRef() const noexcept
```

Defined in src/base/include/icy/memory.h:65

Increments the reference count. Called by [IntrusivePtr](#intrusiveptr) on acquisition.

---

{#releaseref}

#### releaseRef

`const` `inline` `noexcept`

```cpp
inline bool releaseRef() const noexcept
```

Defined in src/base/include/icy/memory.h:69

Decrements the reference count. 
#### Returns
true if the count reached zero (caller should delete the object).

---

{#refcount}

#### refCount

`const` `inline` `nodiscard` `noexcept`

```cpp
[[nodiscard]] inline int refCount() const noexcept
```

Defined in src/base/include/icy/memory.h:73

Returns the current reference count. 
#### Returns
Current reference count.

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `int` | [`_refCount`](#_refcount)  |  |

---

{#_refcount}

#### _refCount

```cpp
int _refCount = 0
```

Defined in src/base/include/icy/memory.h:79

{#intrusiveptr}

## IntrusivePtr

```cpp
#include <icy/memory.h>
```

```cpp
template<typename T>
class IntrusivePtr
```

Defined in src/base/include/icy/memory.h:90

Intrusive smart pointer for [RefCounted](#refcounted) objects.

Like std::shared_ptr but with zero allocation overhead:

* No separate control block (refcount is embedded in the object)

* Non-atomic refcount (safe for single-threaded libuv loops)

* Same API as shared_ptr for easy migration

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`IntrusivePtr`](#intrusiveptr) `inline` `noexcept` |  |
|  | [`IntrusivePtr`](#intrusiveptr) `inline` `noexcept` |  |
|  | [`IntrusivePtr`](#intrusiveptr) `inline` `explicit` `noexcept` |  |
|  | [`IntrusivePtr`](#intrusiveptr) `inline` `noexcept` |  |
|  | [`IntrusivePtr`](#intrusiveptr) `inline` `noexcept` |  |
|  | [`IntrusivePtr`](#intrusiveptr) `inline` `noexcept` |  |
|  | [`IntrusivePtr`](#intrusiveptr) `inline` `noexcept` |  |
| `void` | [`reset`](#reset) `inline` `noexcept` | Releases ownership of the current pointer, decrementing its refcount. The pointer is set to null. |
| `void` | [`reset`](#reset) `inline` `noexcept` | Releases the current pointer and takes ownership of p, incrementing its refcount. |
| `T *` | [`get`](#get) `const` `inline` `noexcept` | Returns the raw pointer without transferring ownership. |
| `T &` | [`operator*`](#operator) `const` `inline` `noexcept` | Dereferences the managed pointer. |
| `T *` | [`operator->`](#operator) `const` `inline` `noexcept` | Member access on the managed pointer. |
|  | [`operator bool`](#operatorbool) `const` `inline` `explicit` `noexcept` | Returns true if the pointer is non-null. |
| `void` | [`swap`](#swap) `inline` `noexcept` | Swaps the managed pointer with another [IntrusivePtr](#intrusiveptr). |
| `void` | [`detach`](#detach) `inline` `noexcept` | Release ownership without decrementing refcount. Used internally for move construction across types. |
| `bool` | [`operator==`](#operator) `const` `inline` `noexcept` |  |
| `bool` | [`operator!=`](#operator) `const` `inline` `noexcept` |  |
| `bool` | [`operator==`](#operator) `const` `inline` `noexcept` |  |
| `bool` | [`operator!=`](#operator) `const` `inline` `noexcept` |  |
| `bool` | [`operator<`](#operator) `const` `inline` `noexcept` |  |

---

{#intrusiveptr}

#### IntrusivePtr

`inline` `noexcept`

```cpp
inline IntrusivePtr() noexcept
```

Defined in src/base/include/icy/memory.h:93

---

{#intrusiveptr}

#### IntrusivePtr

`inline` `noexcept`

```cpp
inline IntrusivePtr(std::nullptr_t) noexcept
```

Defined in src/base/include/icy/memory.h:94

---

{#intrusiveptr}

#### IntrusivePtr

`inline` `explicit` `noexcept`

```cpp
inline explicit IntrusivePtr(T * p) noexcept
```

Defined in src/base/include/icy/memory.h:96

---

{#intrusiveptr}

#### IntrusivePtr

`inline` `noexcept`

```cpp
inline IntrusivePtr(const IntrusivePtr & r) noexcept
```

Defined in src/base/include/icy/memory.h:101

---

{#intrusiveptr}

#### IntrusivePtr

`inline` `noexcept`

```cpp
template<typename U> inline IntrusivePtr(const IntrusivePtr< U > & r) noexcept
```

Defined in src/base/include/icy/memory.h:107

---

{#intrusiveptr}

#### IntrusivePtr

`inline` `noexcept`

```cpp
inline IntrusivePtr(IntrusivePtr && r) noexcept
```

Defined in src/base/include/icy/memory.h:112

---

{#intrusiveptr}

#### IntrusivePtr

`inline` `noexcept`

```cpp
template<typename U> inline IntrusivePtr(IntrusivePtr< U > && r) noexcept
```

Defined in src/base/include/icy/memory.h:118

---

{#reset}

#### reset

`inline` `noexcept`

```cpp
inline void reset() noexcept
```

Defined in src/base/include/icy/memory.h:155

Releases ownership of the current pointer, decrementing its refcount. The pointer is set to null.

---

{#reset}

#### reset

`inline` `noexcept`

```cpp
inline void reset(T * p) noexcept
```

Defined in src/base/include/icy/memory.h:162

Releases the current pointer and takes ownership of p, incrementing its refcount. 
#### Parameters
* `p` New raw pointer to manage (may be nullptr).

---

{#get}

#### get

`const` `inline` `noexcept`

```cpp
inline T * get() const noexcept
```

Defined in src/base/include/icy/memory.h:169

Returns the raw pointer without transferring ownership. 
#### Returns
Raw pointer to the managed object, or nullptr.

---

{#operator}

#### operator*

`const` `inline` `noexcept`

```cpp
inline T & operator*() const noexcept
```

Defined in src/base/include/icy/memory.h:173

Dereferences the managed pointer. 
#### Returns
Reference to the managed object.

---

{#operator}

#### operator->

`const` `inline` `noexcept`

```cpp
inline T * operator->() const noexcept
```

Defined in src/base/include/icy/memory.h:177

Member access on the managed pointer. 
#### Returns
Raw pointer to the managed object.

---

{#operatorbool}

#### operator bool

`const` `inline` `explicit` `noexcept`

```cpp
inline explicit operator bool() const noexcept
```

Defined in src/base/include/icy/memory.h:180

Returns true if the pointer is non-null.

---

{#swap}

#### swap

`inline` `noexcept`

```cpp
inline void swap(IntrusivePtr & r) noexcept
```

Defined in src/base/include/icy/memory.h:184

Swaps the managed pointer with another [IntrusivePtr](#intrusiveptr). 
#### Parameters
* `r` The other [IntrusivePtr](#intrusiveptr) to swap with.

---

{#detach}

#### detach

`inline` `noexcept`

```cpp
inline void detach() noexcept
```

Defined in src/base/include/icy/memory.h:188

Release ownership without decrementing refcount. Used internally for move construction across types.

---

{#operator}

#### operator==

`const` `inline` `noexcept`

```cpp
inline bool operator==(const IntrusivePtr & r) const noexcept
```

Defined in src/base/include/icy/memory.h:190

---

{#operator}

#### operator!=

`const` `inline` `noexcept`

```cpp
inline bool operator!=(const IntrusivePtr & r) const noexcept
```

Defined in src/base/include/icy/memory.h:191

---

{#operator}

#### operator==

`const` `inline` `noexcept`

```cpp
inline bool operator==(std::nullptr_t) const noexcept
```

Defined in src/base/include/icy/memory.h:192

---

{#operator}

#### operator!=

`const` `inline` `noexcept`

```cpp
inline bool operator!=(std::nullptr_t) const noexcept
```

Defined in src/base/include/icy/memory.h:193

---

{#operator}

#### operator<

`const` `inline` `noexcept`

```cpp
inline bool operator<(const IntrusivePtr & r) const noexcept
```

Defined in src/base/include/icy/memory.h:194

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `T *` | [`_ptr`](#_ptr)  |  |

---

{#_ptr}

#### _ptr

```cpp
T * _ptr
```

Defined in src/base/include/icy/memory.h:197

{#random}

## Random

```cpp
#include <icy/random.h>
```

```cpp
class Random
```

Defined in src/base/include/icy/random.h:25

[Random](#random) implements a pseudo random number generator (PRNG) using the Mersenne Twister algorithm (std::mt19937).

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Random`](#random)  | Creates and initializes the PRNG. The stateSize parameter is accepted for API compatibility but is ignored; the engine always uses mt19937's fixed state size. |
|  | [`~Random`](#random)  | Destroys the PRNG. |
| `void` | [`seed`](#seed)  | Seeds the pseudo random generator with the given seed. |
| `void` | [`seed`](#seed)  | Seeds the pseudo random generator with entropy from std::random_device. |
| `uint32_t` | [`next`](#next)  | Returns the next pseudo random number from the mt19937 engine. |
| `uint32_t` | [`next`](#next)  | Returns the next pseudo random number in the range [0, n). |
| `char` | [`nextChar`](#nextchar)  | Returns the next pseudo random byte as a char. |
| `bool` | [`nextBool`](#nextbool)  | Returns the next pseudo random boolean value. |
| `float` | [`nextFloat`](#nextfloat)  | Returns the next pseudo random float in [0.0, 1.0]. |
| `double` | [`nextDouble`](#nextdouble)  | Returns the next pseudo random double in [0.0, 1.0]. |

---

{#random}

#### Random

```cpp
Random(int stateSize = 256)
```

Defined in src/base/include/icy/random.h:32

Creates and initializes the PRNG. The stateSize parameter is accepted for API compatibility but is ignored; the engine always uses mt19937's fixed state size. 
#### Parameters
* `stateSize` Ignored; present for API compatibility only.

---

{#random}

#### ~Random

```cpp
~Random()
```

Defined in src/base/include/icy/random.h:35

Destroys the PRNG.

---

{#seed}

#### seed

```cpp
void seed(uint32_t seed)
```

Defined in src/base/include/icy/random.h:39

Seeds the pseudo random generator with the given seed. 
#### Parameters
* `seed` 32-bit seed value.

---

{#seed}

#### seed

```cpp
void seed()
```

Defined in src/base/include/icy/random.h:42

Seeds the pseudo random generator with entropy from std::random_device.

---

{#next}

#### next

```cpp
uint32_t next()
```

Defined in src/base/include/icy/random.h:46

Returns the next pseudo random number from the mt19937 engine. 
#### Returns
Pseudo random uint32_t value.

---

{#next}

#### next

```cpp
uint32_t next(uint32_t n)
```

Defined in src/base/include/icy/random.h:51

Returns the next pseudo random number in the range [0, n). 
#### Parameters
* `n` Upper bound (exclusive). 

#### Returns
Pseudo random value in [0, n).

---

{#nextchar}

#### nextChar

```cpp
char nextChar()
```

Defined in src/base/include/icy/random.h:55

Returns the next pseudo random byte as a char. 
#### Returns
Pseudo random char value.

---

{#nextbool}

#### nextBool

```cpp
bool nextBool()
```

Defined in src/base/include/icy/random.h:59

Returns the next pseudo random boolean value. 
#### Returns
true or false with equal probability.

---

{#nextfloat}

#### nextFloat

```cpp
float nextFloat()
```

Defined in src/base/include/icy/random.h:63

Returns the next pseudo random float in [0.0, 1.0]. 
#### Returns
Pseudo random float value.

---

{#nextdouble}

#### nextDouble

```cpp
double nextDouble()
```

Defined in src/base/include/icy/random.h:67

Returns the next pseudo random double in [0.0, 1.0]. 
#### Returns
Pseudo random double value.

### Public Static Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`getSeed`](#getseed) `static` | Fills the buffer with cryptographically random bytes from std::random_device. |

---

{#getseed}

#### getSeed

`static`

```cpp
static void getSeed(char * seed, unsigned length)
```

Defined in src/base/include/icy/random.h:72

Fills the buffer with cryptographically random bytes from std::random_device. 
#### Parameters
* `seed` [Buffer](#buffer-2) to fill. 

* `length` Number of bytes to write into seed.

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::mt19937` | [`_engine`](#_engine)  |  |

---

{#_engine}

#### _engine

```cpp
std::mt19937 _engine
```

Defined in src/base/include/icy/random.h:75

{#runner}

## Runner

```cpp
#include <icy/runner.h>
```

```cpp
class Runner
```

Defined in src/base/include/icy/runner.h:34

> **Subclassed by:** [`Idler`](#idler), [`Synchronizer`](#synchronizer), [`Thread`](#thread), [`Timer`](#timer)

[Runner](#runner) is a virtual interface for implementing asynchronous objects such as threads and futures.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Runner`](#runner)  |  |
| `void` | [`start`](#start) `virtual` | Starts the asynchronous context with the given callback. The callback must remain valid for the lifetime of the `[Runner](#runner)`. |
| `bool` | [`running`](#running) `const` | Returns true if the async context is currently running. |
| `void` | [`cancel`](#cancel)  | Signals the async context to stop at the earliest opportunity. |
| `bool` | [`cancelled`](#cancelled) `const` | Returns true if the context has been cancelled. The implementation is responsible for exiting as soon as possible after cancellation. |
| `bool` | [`repeating`](#repeating) `const` | Returns true if the runner is in repeating mode. |
| `void` | [`setRepeating`](#setrepeating)  | Enables or disables repeating mode. When repeating, the target function is invoked repeatedly until `[cancel()](#classicy_1_1Runner_1a53b3794fbd4ace235771a4e84f93f1e7)` is called. This normalizes behaviour across thread-based and event-loop-based `[Runner](#runner)` implementations. Must be called before `[start()](#classicy_1_1Runner_1a5969e823f1ce0bdd8730d3108bf13fbd)`. |
| `bool` | [`async`](#async) `virtual` `const` | Returns true if the implementation is thread-based. |
| `std::thread::id` | [`tid`](#tid) `const` | Returns the native thread ID of the thread running the async context. |
| `bool` | [`waitForExit`](#waitforexit)  | Blocks until the async context exits or the timeout elapses. The context should be cancelled before calling this method. Must be called from outside the runner's own thread to avoid deadlock. |

---

{#runner}

#### Runner

```cpp
Runner()
```

Defined in src/base/include/icy/runner.h:37

---

{#start}

#### start

`virtual`

```cpp
virtual void start(std::function< void()> target)
```

Defined in src/base/include/icy/runner.h:43

Starts the asynchronous context with the given callback. The callback must remain valid for the lifetime of the `[Runner](#runner)`. 
#### Parameters
* `target` Callable to invoke when the context runs.

---

{#running}

#### running

`const`

```cpp
bool running() const
```

Defined in src/base/include/icy/runner.h:47

Returns true if the async context is currently running. 
#### Returns
True if the runner's context has been started and has not yet stopped.

---

{#cancel}

#### cancel

```cpp
void cancel()
```

Defined in src/base/include/icy/runner.h:50

Signals the async context to stop at the earliest opportunity.

---

{#cancelled}

#### cancelled

`const`

```cpp
bool cancelled() const
```

Defined in src/base/include/icy/runner.h:55

Returns true if the context has been cancelled. The implementation is responsible for exiting as soon as possible after cancellation. 
#### Returns
True if `[cancel()](#classicy_1_1Runner_1a53b3794fbd4ace235771a4e84f93f1e7)` has been called.

---

{#repeating}

#### repeating

`const`

```cpp
bool repeating() const
```

Defined in src/base/include/icy/runner.h:59

Returns true if the runner is in repeating mode. 
#### Returns
True if the target function is invoked in a loop until cancelled.

---

{#setrepeating}

#### setRepeating

```cpp
void setRepeating(bool flag)
```

Defined in src/base/include/icy/runner.h:66

Enables or disables repeating mode. When repeating, the target function is invoked repeatedly until `[cancel()](#classicy_1_1Runner_1a53b3794fbd4ace235771a4e84f93f1e7)` is called. This normalizes behaviour across thread-based and event-loop-based `[Runner](#runner)` implementations. Must be called before `[start()](#classicy_1_1Runner_1a5969e823f1ce0bdd8730d3108bf13fbd)`. 
#### Parameters
* `flag` True to enable repeating mode, false to run the target once.

---

{#async}

#### async

`virtual` `const`

```cpp
virtual bool async() const
```

Defined in src/base/include/icy/runner.h:70

Returns true if the implementation is thread-based. 
#### Returns
True for thread-backed runners, false for event-loop-driven runners.

---

{#tid}

#### tid

`const`

```cpp
std::thread::id tid() const
```

Defined in src/base/include/icy/runner.h:74

Returns the native thread ID of the thread running the async context. 
#### Returns
`std::thread::id` of the runner thread, or a default-constructed ID if not started.

---

{#waitforexit}

#### waitForExit

```cpp
bool waitForExit(int timeout = 5000)
```

Defined in src/base/include/icy/runner.h:81

Blocks until the async context exits or the timeout elapses. The context should be cancelled before calling this method. Must be called from outside the runner's own thread to avoid deadlock. 
#### Parameters
* `timeout` Maximum number of milliseconds to wait. Pass 0 to wait indefinitely. 

#### Returns
True if the context exited cleanly, false if the timeout was reached (throws instead).

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::shared_ptr< Context >` | [`_context`](#_context)  | Shared pointer to the internal [Context](#context). |

---

{#_context}

#### _context

```cpp
std::shared_ptr< Context > _context
```

Defined in src/base/include/icy/runner.h:117

Shared pointer to the internal [Context](#context).

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Runner`](#runner)  | NonCopyable and NonMovable. |
|  | [`Runner`](#runner)  | Deleted constructor. |

---

{#runner}

#### Runner

```cpp
Runner(const Runner &) = delete
```

Defined in src/base/include/icy/runner.h:111

NonCopyable and NonMovable.

---

{#runner}

#### Runner

```cpp
Runner(Runner &&) = delete
```

Defined in src/base/include/icy/runner.h:113

Deleted constructor.

{#signalrtargsmutext}

## Signal< RT(Args...), MutexT >

```cpp
#include <icy/signal.h>
```

```cpp
template<typename RT, typename... Args, typename MutexT>
class Signal< RT(Args...), MutexT >
```

Defined in src/base/include/icy/signal.h:142

Thread-safe signal and slot implementation for callback-based event dispatch.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
| `int` | [`attach`](#attach) `const` `inline` `nodiscard` | Connects a `lambda` or `std::function` to the signal. |
| `int` | [`attach`](#attach) `const` `inline` `nodiscard` | Connects a pre-constructed `[SlotPtr](#classicy_1_1Signal_3_01RT_07Args_8_8_8_08_00_01MutexT_01_4_1ad44dfcd3b949fddac73c8704d8754c3b)` to the signal. |
| `bool` | [`detach`](#detach) `const` `inline` | Detaches the slot with the given ID. |
| `bool` | [`detach`](#detach) `const` `inline` | Detaches all slots associated with the given instance pointer. |
| `bool` | [`detach`](#detach) `const` `inline` | Detaches the slot whose delegate compares equal to `other->delegate`. |
| `void` | [`detachAll`](#detachall) `const` `inline` | Detaches and destroys all currently attached slots. |
| `RT` | [`emit`](#emit) `virtual` `inline` | Emits the signal, invoking all live attached slots in priority order. |
| `std::vector< SlotPtr >` | [`slots`](#slots) `const` `inline` | Returns a snapshot copy of the current slot list. |
| `size_t` | [`nslots`](#nslots) `const` `inline` `nodiscard` | Returns the number of slots currently registered with this signal. |
| `int` | [`operator+=`](#operator) `inline` | Attaches a function; equivalent to `attach(func)`. |
| `int` | [`operator+=`](#operator) `inline` | Attaches a pre-constructed slot; equivalent to `attach(slot)`. |
| `bool` | [`operator-=`](#operator) `inline` | Detaches the slot with the given ID; equivalent to `detach(id)`. |
| `bool` | [`operator-=`](#operator) `inline` | Detaches all slots for the given instance; equivalent to `detach(instance)`. |
| `bool` | [`operator-=`](#operator) `inline` | Detaches the slot matching `[slot](#slot)`'s delegate; equivalent to `detach(slot)`. |
|  | [`Signal`](#signal)  | Defaulted constructor. |
|  | [`Signal`](#signal) `inline` | Copy constructor; copies the slot list and last-assigned ID from `r`. |
| `Signal &` | [`operator=`](#operator) `inline` | Copy assignment operator; copies the slot list and last-assigned ID from `r`. |

---

{#attach}

#### attach

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline int attach(Function const & func, void * instance = nullptr, int id = -1, int priority = -1) const
```

Defined in src/base/include/icy/signal.h:157

Connects a `lambda` or `std::function` to the signal.

#### Parameters
* `func` The callable to invoke when the signal is emitted. 

* `instance` Optional owner pointer used for instance-based detach; pass `nullptr` if not applicable. 

* `id` Explicit slot ID to assign; pass `-1` to auto-assign. 

* `priority` Higher values are called first; pass `-1` for default ordering. 

#### Returns
The assigned slot ID, which can be passed to `[detach()](#classicy_1_1Signal_3_01RT_07Args_8_8_8_08_00_01MutexT_01_4_1af559f1f6b044f249b3d7b04d80274bab)` to disconnect.

---

{#attach}

#### attach

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline int attach(SlotPtr slot) const
```

Defined in src/base/include/icy/signal.h:171

Connects a pre-constructed `[SlotPtr](#classicy_1_1Signal_3_01RT_07Args_8_8_8_08_00_01MutexT_01_4_1ad44dfcd3b949fddac73c8704d8754c3b)` to the signal.

Duplicate slots (matched by delegate equality) are removed before insertion. Slots are kept sorted in descending priority order after insertion.

#### Parameters
* `slot` The slot to attach. Must have a valid delegate. 

#### Returns
The assigned slot ID, which can be passed to `[detach()](#classicy_1_1Signal_3_01RT_07Args_8_8_8_08_00_01MutexT_01_4_1af559f1f6b044f249b3d7b04d80274bab)` to disconnect. 

#### Exceptions
* `std::logic_error` if `slot->id` is set explicitly and already in use.

---

{#detach}

#### detach

`const` `inline`

```cpp
inline bool detach(int id) const
```

Defined in src/base/include/icy/signal.h:202

Detaches the slot with the given ID.

Safe to call from within a slot's callback (the slot is marked dead before erasure).

#### Parameters
* `id` The slot ID returned by `[attach()](#classicy_1_1Signal_3_01RT_07Args_8_8_8_08_00_01MutexT_01_4_1ab117b8aacf13226bade149c94b6088ed)`. 

#### Returns
`true` if a slot was found and removed; `false` if the ID was not found.

---

{#detach}

#### detach

`const` `inline`

```cpp
inline bool detach(const void * instance) const
```

Defined in src/base/include/icy/signal.h:219

Detaches all slots associated with the given instance pointer.

Useful for bulk disconnect when an object is destroyed. Matches slots by their stored `instance` pointer, not by delegate equality.

#### Parameters
* `instance` The owner pointer used when the slots were attached. 

#### Returns
`true` if at least one slot was removed; `false` otherwise.

---

{#detach}

#### detach

`const` `inline`

```cpp
inline bool detach(SlotPtr other) const
```

Defined in src/base/include/icy/signal.h:236

Detaches the slot whose delegate compares equal to `other->delegate`.

Used by the `[slot()](#slot)` helper overloads and `operator-=` to disconnect a specific class-member or function binding by value.

#### Parameters
* `other` A slot whose delegate is compared against attached slots. 

#### Returns
`true` if a matching slot was found and removed; `false` otherwise.

---

{#detachall}

#### detachAll

`const` `inline`

```cpp
inline void detachAll() const
```

Defined in src/base/include/icy/signal.h:249

Detaches and destroys all currently attached slots.

Each slot is marked dead before removal. After this call `[nslots()](#classicy_1_1Signal_3_01RT_07Args_8_8_8_08_00_01MutexT_01_4_1ab987050a876fe4e983601eae08062587)` returns 0.

---

{#emit}

#### emit

`virtual` `inline`

```cpp
virtual inline RT emit(Args... args)
```

Defined in src/base/include/icy/signal.h:283

Emits the signal, invoking all live attached slots in priority order.

For `[Signal](#signal)<bool(...)>`: iterates slots and returns `true` as soon as any slot returns `true`, stopping further propagation. Returns `false` if no slot handled the event.

For `[Signal](#signal)<void(...)>`: calls every live slot unconditionally.

Local `[Signal](#signal)<...>` traverses the stable slot list directly without any snapshot allocation. `[ThreadSignal](#threadsignal)<...>` snapshots raw node pointers under a shared lock, then invokes delegates without holding the lock. Dead slots are swept after the outermost emission returns, allowing attach/detach inside callbacks without copying `shared_ptr`s on the hot path.

#### Parameters
* `args` Arguments forwarded to each connected slot. 

#### Returns
For `bool` return type: `true` if any slot handled the event, `false` otherwise. For `void` return type: nothing.

---

{#slots}

#### slots

`const` `inline`

```cpp
inline std::vector< SlotPtr > slots() const
```

Defined in src/base/include/icy/signal.h:354

Returns a snapshot copy of the current slot list.

The copy is taken under a shared lock, so it is safe to call concurrently with attach/detach operations. Only currently live slots are returned.

#### Returns
A vector of `[SlotPtr](#classicy_1_1Signal_3_01RT_07Args_8_8_8_08_00_01MutexT_01_4_1ad44dfcd3b949fddac73c8704d8754c3b)` representing all currently registered slots.

---

{#nslots}

#### nslots

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline size_t nslots() const
```

Defined in src/base/include/icy/signal.h:372

Returns the number of slots currently registered with this signal.

The count is taken under a shared lock and tracks only currently live slots. Dead-but-not-yet-swept nodes are excluded.

#### Returns
The number of entries in the internal slot list.

---

{#operator}

#### operator+=

`inline`

```cpp
inline int operator+=(Function const & func)
```

Defined in src/base/include/icy/signal.h:379

Attaches a function; equivalent to `attach(func)`.

#### Returns
Assigned slot ID.

---

{#operator}

#### operator+=

`inline`

```cpp
inline int operator+=(SlotPtr slot)
```

Defined in src/base/include/icy/signal.h:381

Attaches a pre-constructed slot; equivalent to `attach(slot)`.

#### Returns
Assigned slot ID.

---

{#operator}

#### operator-=

`inline`

```cpp
inline bool operator-=(int id)
```

Defined in src/base/include/icy/signal.h:383

Detaches the slot with the given ID; equivalent to `detach(id)`.

#### Returns
`true` if removed.

---

{#operator}

#### operator-=

`inline`

```cpp
inline bool operator-=(const void * instance)
```

Defined in src/base/include/icy/signal.h:385

Detaches all slots for the given instance; equivalent to `detach(instance)`.

#### Returns
`true` if any removed.

---

{#operator}

#### operator-=

`inline`

```cpp
inline bool operator-=(SlotPtr slot)
```

Defined in src/base/include/icy/signal.h:387

Detaches the slot matching `[slot](#slot)`'s delegate; equivalent to `detach(slot)`.

#### Returns
`true` if removed.

---

{#signal}

#### Signal

```cpp
Signal() = default
```

Defined in src/base/include/icy/signal.h:389

Defaulted constructor.

---

{#signal}

#### Signal

`inline`

```cpp
inline Signal(const Signal & r)
```

Defined in src/base/include/icy/signal.h:398

Copy constructor; copies the slot list and last-assigned ID from `r`. 
#### Parameters
* `r` The signal to copy from.

---

{#operator}

#### operator=

`inline`

```cpp
inline Signal & operator=(const Signal & r)
```

Defined in src/base/include/icy/signal.h:407

Copy assignment operator; copies the slot list and last-assigned ID from `r`. 
#### Parameters
* `r` The signal to copy from. 

#### Returns
Reference to this signal.

### Public Types

| Name | Description |
|------|-------------|
| [`Function`](#function)  |  |
| [`SlotPtr`](#slotptr)  |  |
| [`Slot`](#slot)  |  |
| [`SlotNode`](#slotnode)  |  |

---

{#function}

#### Function

```cpp
using Function = std::function< RT(Args...)>
```

Defined in src/base/include/icy/signal.h:145

---

{#slotptr}

#### SlotPtr

```cpp
using SlotPtr = std::shared_ptr< internal::Slot< RT, Args... > >
```

Defined in src/base/include/icy/signal.h:146

---

{#slot}

#### Slot

```cpp
using Slot = internal::Slot< RT, Args... >
```

Defined in src/base/include/icy/signal.h:147

---

{#slotnode}

#### SlotNode

```cpp
using SlotNode = internal::SlotNode< RT, Args... >
```

Defined in src/base/include/icy/signal.h:148

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `MutexT` | [`_mutex`](#_mutex)  |  |
| `SlotNode *` | [`_head`](#_head)  |  |
| `SlotNode *` | [`_tail`](#_tail)  |  |
| `size_t` | [`_liveCount`](#_livecount)  |  |
| `int` | [`_lastId`](#_lastid)  |  |
| `EmitDepth` | [`_emitDepth`](#_emitdepth)  |  |
| `SweepFlag` | [`_needsSweep`](#_needssweep)  |  |

---

{#_mutex}

#### _mutex

```cpp
MutexT _mutex
```

Defined in src/base/include/icy/signal.h:615

---

{#_head}

#### _head

```cpp
SlotNode * _head = nullptr
```

Defined in src/base/include/icy/signal.h:616

---

{#_tail}

#### _tail

```cpp
SlotNode * _tail = nullptr
```

Defined in src/base/include/icy/signal.h:617

---

{#_livecount}

#### _liveCount

```cpp
size_t _liveCount = 0
```

Defined in src/base/include/icy/signal.h:618

---

{#_lastid}

#### _lastId

```cpp
int _lastId = 0
```

Defined in src/base/include/icy/signal.h:619

---

{#_emitdepth}

#### _emitDepth

```cpp
EmitDepth _emitDepth {}
```

Defined in src/base/include/icy/signal.h:620

---

{#_needssweep}

#### _needsSweep

```cpp
SweepFlag _needsSweep {}
```

Defined in src/base/include/icy/signal.h:621

### Private Methods

| Return | Name | Description |
|--------|------|-------------|
| `size_t` | [`killMatchingLocked`](#killmatchinglocked) `const` `inline` |  |
| `void` | [`insertSlotLocked`](#insertslotlocked) `const` `inline` |  |
| `void` | [`appendSlotLocked`](#appendslotlocked) `const` `inline` |  |
| `void` | [`eraseNodeLocked`](#erasenodelocked) `const` `inline` |  |
| `size_t` | [`emitDepthLocked`](#emitdepthlocked) `const` `inline` |  |
| `void` | [`beginEmitLocked`](#beginemitlocked) `const` `inline` |  |
| `bool` | [`endEmitLocked`](#endemitlocked) `const` `inline` |  |
| `void` | [`requestSweepLocked`](#requestsweeplocked) `const` `inline` |  |
| `bool` | [`consumeSweepRequest`](#consumesweeprequest) `const` `inline` |  |
| `void` | [`sweepLocked`](#sweeplocked) `const` `inline` |  |
| `void` | [`finishEmit`](#finishemit) `inline` |  |
| `void` | [`clearNodesLocked`](#clearnodeslocked) `const` `inline` |  |
| `void` | [`resetEmitState`](#resetemitstate) `inline` |  |
| `std::pair< std::vector< SlotPtr >, int >` | [`snapshotState`](#snapshotstate) `const` `inline` |  |
| `void` | [`restoreState`](#restorestate) `inline` |  |

---

{#killmatchinglocked}

#### killMatchingLocked

`const` `inline`

```cpp
template<typename Matcher> inline size_t killMatchingLocked(Matcher && matcher, bool removeAll) const
```

Defined in src/base/include/icy/signal.h:422

---

{#insertslotlocked}

#### insertSlotLocked

`const` `inline`

```cpp
inline void insertSlotLocked(SlotPtr slot) const
```

Defined in src/base/include/icy/signal.h:448

---

{#appendslotlocked}

#### appendSlotLocked

`const` `inline`

```cpp
inline void appendSlotLocked(SlotPtr slot) const
```

Defined in src/base/include/icy/signal.h:475

---

{#erasenodelocked}

#### eraseNodeLocked

`const` `inline`

```cpp
inline void eraseNodeLocked(SlotNode * node) const
```

Defined in src/base/include/icy/signal.h:487

---

{#emitdepthlocked}

#### emitDepthLocked

`const` `inline`

```cpp
inline size_t emitDepthLocked() const
```

Defined in src/base/include/icy/signal.h:502

---

{#beginemitlocked}

#### beginEmitLocked

`const` `inline`

```cpp
inline void beginEmitLocked() const
```

Defined in src/base/include/icy/signal.h:510

---

{#endemitlocked}

#### endEmitLocked

`const` `inline`

```cpp
inline bool endEmitLocked() const
```

Defined in src/base/include/icy/signal.h:518

---

{#requestsweeplocked}

#### requestSweepLocked

`const` `inline`

```cpp
inline void requestSweepLocked() const
```

Defined in src/base/include/icy/signal.h:526

---

{#consumesweeprequest}

#### consumeSweepRequest

`const` `inline`

```cpp
inline bool consumeSweepRequest() const
```

Defined in src/base/include/icy/signal.h:534

---

{#sweeplocked}

#### sweepLocked

`const` `inline`

```cpp
inline void sweepLocked() const
```

Defined in src/base/include/icy/signal.h:545

---

{#finishemit}

#### finishEmit

`inline`

```cpp
inline void finishEmit()
```

Defined in src/base/include/icy/signal.h:555

---

{#clearnodeslocked}

#### clearNodesLocked

`const` `inline`

```cpp
inline void clearNodesLocked() const
```

Defined in src/base/include/icy/signal.h:570

---

{#resetemitstate}

#### resetEmitState

`inline`

```cpp
inline void resetEmitState()
```

Defined in src/base/include/icy/signal.h:583

---

{#snapshotstate}

#### snapshotState

`const` `inline`

```cpp
inline std::pair< std::vector< SlotPtr >, int > snapshotState() const
```

Defined in src/base/include/icy/signal.h:594

---

{#restorestate}

#### restoreState

`inline`

```cpp
inline void restoreState(std::vector< SlotPtr > snapshot, int lastId)
```

Defined in src/base/include/icy/signal.h:606

### Private Static Attributes

| Return | Name | Description |
|--------|------|-------------|
| `bool` | [`threadSafe`](#threadsafe) `static` `constexpr` |  |

---

{#threadsafe}

#### threadSafe

`static` `constexpr`

```cpp
bool threadSafe = !std::is_same_v<MutexT, >
```

Defined in src/base/include/icy/signal.h:417

{#stream}

## Stream

```cpp
#include <icy/stream.h>
```

```cpp
template<typename T>
class Stream
```

Defined in src/base/include/icy/stream.h:30

> **Inherits:** [`Handle< T >`](uv.md#handle-2)

Basic stream type for sockets and pipes.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `Signal< void(const char *, const int &)>` | [`Read`](#read)  | Emitted when data has been received from the peer. |

---

{#read}

#### Read

```cpp
Signal< void(const char *, const int &)> Read
```

Defined in src/base/include/icy/stream.h:210

Emitted when data has been received from the peer.

Slot signature: `void(const char* data, const int& len)`

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Stream`](#stream) `inline` | Construct the stream bound to `loop` with a 64 KiB read buffer. |
|  | [`~Stream`](#stream) `virtual` `inline` | Destroy the stream, stopping reads and freeing pooled write requests. |
| `void` | [`close`](#close) `virtual` `inline` `override` | Closes and resets the stream handle. This will close the active socket/pipe and destroy the handle. |
| `bool` | [`shutdown`](#shutdown) `inline` | Send a TCP/pipe shutdown request to the connected peer. |
| `bool` | [`write`](#write) `inline` | Write `len` bytes from `data` to the stream. |
| `bool` | [`writeOwned`](#writeowned) `inline` | Write an owned payload buffer to the stream. |
| `void` | [`setHighWaterMark`](#sethighwatermark) `inline` | Set the high water mark for the write queue (default 16MB). When the write queue exceeds this size, [write()](#classicy_1_1Stream_1a56926a6ac3ba433aed1414ffb1d20cf0) returns false. |
| `bool` | [`write`](#write) `inline` | Write `len` bytes from `data` together with a stream handle over an IPC pipe (uses `uv_write2`). |
| `uv_stream_t *` | [`stream`](#stream) `inline` | Return the underlying `uv_stream_t` pointer cast from the native handle. |

---

{#stream}

#### Stream

`inline`

```cpp
inline Stream(uv::Loop * loop = uv::defaultLoop())
```

Defined in src/base/include/icy/stream.h:47

Construct the stream bound to `loop` with a 64 KiB read buffer.

#### Parameters
* `loop` Event loop to associate this stream with.

---

{#stream}

#### ~Stream

`virtual` `inline`

```cpp
virtual inline ~Stream()
```

Defined in src/base/include/icy/stream.h:54

Destroy the stream, stopping reads and freeing pooled write requests.

---

{#close}

#### close

`virtual` `inline` `override`

```cpp
virtual inline void close() override
```

Defined in src/base/include/icy/stream.h:67

Closes and resets the stream handle. This will close the active socket/pipe and destroy the handle.

If the stream is already closed this call will have no side-effects.

---

{#shutdown}

#### shutdown

`inline`

```cpp
inline bool shutdown()
```

Defined in src/base/include/icy/stream.h:82

Send a TCP/pipe shutdown request to the connected peer.

Issues a half-close: no further writes will be accepted after this, but the stream remains open for reading until the peer also closes.

#### Returns
`true` if the shutdown request was submitted successfully; `false` if the stream is not active.

---

{#write}

#### write

`inline`

```cpp
inline bool write(const char * data, size_t len)
```

Defined in src/base/include/icy/stream.h:105

Write `len` bytes from `data` to the stream.

The write is non-blocking; data is buffered by libuv. Returns `false` without throwing if the stream is inactive, reads have not started, or the internal write queue exceeds the high-water mark.

#### Parameters
* `data` Pointer to the bytes to send. Must remain valid until the write completion callback fires. 

* `len` Number of bytes to send. 

#### Returns
`true` if the write was queued; `false` on backpressure or if the stream is not in a writable state.

---

{#writeowned}

#### writeOwned

`inline`

```cpp
inline bool writeOwned(Buffer && buffer)
```

Defined in src/base/include/icy/stream.h:140

Write an owned payload buffer to the stream.

The buffer is moved into the queued write request and retained until the libuv completion callback fires. Use this path whenever the caller does not naturally own storage beyond the current stack frame.

#### Parameters
* `buffer` Payload buffer moved into the async write request. 

#### Returns
`true` if the write was queued; `false` on backpressure or if the stream is not in a writable state.

---

{#sethighwatermark}

#### setHighWaterMark

`inline`

```cpp
inline void setHighWaterMark(size_t bytes)
```

Defined in src/base/include/icy/stream.h:166

Set the high water mark for the write queue (default 16MB). When the write queue exceeds this size, [write()](#classicy_1_1Stream_1a56926a6ac3ba433aed1414ffb1d20cf0) returns false.

---

{#write}

#### write

`inline`

```cpp
inline bool write(const char * data, size_t len, uv_stream_t * send)
```

Defined in src/base/include/icy/stream.h:178

Write `len` bytes from `data` together with a stream handle over an IPC pipe (uses `uv_write2`).

Only valid for named-pipe handles opened with IPC mode enabled. Throws `std::logic_error` if called on a non-IPC pipe.

#### Parameters
* `data` Bytes to send alongside the handle. 

* `len` Number of bytes to send. 

* `send` [Stream](#stream) handle to pass to the receiving process. 

#### Returns
`true` if the write was queued; `false` on error.

---

{#stream}

#### stream

`inline`

```cpp
inline uv_stream_t * stream()
```

Defined in src/base/include/icy/stream.h:202

Return the underlying `uv_stream_t` pointer cast from the native handle.

#### Returns
Pointer to the `uv_stream_t`, or `nullptr` if the handle is closed.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `Buffer` | [`_buffer`](#_buffer)  |  |
| `bool` | [`_started`](#_started)  |  |
| `size_t` | [`_highWaterMark`](#_highwatermark)  | 16MB default write queue limit |
| `std::vector< uv_write_t * >` | [`_writeReqFree`](#_writereqfree)  | Freelist for write requests. |
| `std::vector< OwnedWriteReq * >` | [`_ownedWriteReqFree`](#_ownedwritereqfree)  | Freelist for owned write requests. |

---

{#_buffer}

#### _buffer

```cpp
Buffer _buffer
```

Defined in src/base/include/icy/stream.h:374

---

{#_started}

#### _started

```cpp
bool _started {false}
```

Defined in src/base/include/icy/stream.h:375

---

{#_highwatermark}

#### _highWaterMark

```cpp
size_t _highWaterMark {16 * 1024 * 1024}
```

Defined in src/base/include/icy/stream.h:376

16MB default write queue limit

---

{#_writereqfree}

#### _writeReqFree

```cpp
std::vector< uv_write_t * > _writeReqFree
```

Defined in src/base/include/icy/stream.h:377

Freelist for write requests.

---

{#_ownedwritereqfree}

#### _ownedWriteReqFree

```cpp
std::vector< OwnedWriteReq * > _ownedWriteReqFree
```

Defined in src/base/include/icy/stream.h:378

Freelist for owned write requests.

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `bool` | [`readStart`](#readstart) `virtual` `inline` | Begin reading from the stream by registering libuv read callbacks. |
| `bool` | [`readStop`](#readstop) `virtual` `inline` | Stop reading from the stream. |
| `void` | [`onRead`](#onread) `virtual` `inline` | Called by `[handleRead](#classicy_1_1Stream_1ac74fea672c0d281f2a4f51bee6943b10)` when `len` bytes of `data` arrive. |
| `uv_write_t *` | [`allocWriteReq`](#allocwritereq) `inline` | Return a `uv_write_t` from the freelist, or allocate a new one if the pool is empty. |
| `void` | [`freeWriteReq`](#freewritereq) `inline` | Return `req` to the freelist, or delete it if the pool is at capacity. |
| `OwnedWriteReq *` | [`allocOwnedWriteReq`](#allocownedwritereq) `inline` |  |
| `void` | [`freeOwnedWriteReq`](#freeownedwritereq) `inline` |  |
| `bool` | [`canQueueWrite`](#canqueuewrite) `inline` |  |

---

{#readstart}

#### readStart

`virtual` `inline`

```cpp
virtual inline bool readStart()
```

Defined in src/base/include/icy/stream.h:219

Begin reading from the stream by registering libuv read callbacks.

Sets the stream's `data` pointer to `this` so callbacks can recover the C++ object. Has no effect and returns `false` if already started.

#### Returns
`true` if `uv_read_start` was called successfully.

---

{#readstop}

#### readStop

`virtual` `inline`

```cpp
virtual inline bool readStop()
```

Defined in src/base/include/icy/stream.h:236

Stop reading from the stream.

No further read callbacks will fire after this returns. Has no effect and returns `false` if not currently started.

#### Returns
`true` if `uv_read_stop` was called successfully.

---

{#onread}

#### onRead

`virtual` `inline`

```cpp
virtual inline void onRead(const char * data, size_t len)
```

Defined in src/base/include/icy/stream.h:253

Called by `[handleRead](#classicy_1_1Stream_1ac74fea672c0d281f2a4f51bee6943b10)` when `len` bytes of `data` arrive.

The default implementation emits the `[Read](#classicy_1_1Stream_1a97b040391e1972b8599599a101be2184)` signal. Override to intercept data before it reaches signal subscribers.

#### Parameters
* `data` Pointer into the read buffer; valid only for this call. 

* `len` Number of valid bytes in `data`.

---

{#allocwritereq}

#### allocWriteReq

`inline`

```cpp
inline uv_write_t * allocWriteReq()
```

Defined in src/base/include/icy/stream.h:318

Return a `uv_write_t` from the freelist, or allocate a new one if the pool is empty.

#### Returns
Pointer to an unused `uv_write_t`.

---

{#freewritereq}

#### freeWriteReq

`inline`

```cpp
inline void freeWriteReq(uv_write_t * req)
```

Defined in src/base/include/icy/stream.h:331

Return `req` to the freelist, or delete it if the pool is at capacity.

#### Parameters
* `req` Write request to recycle or free.

---

{#allocownedwritereq}

#### allocOwnedWriteReq

`inline`

```cpp
inline OwnedWriteReq * allocOwnedWriteReq()
```

Defined in src/base/include/icy/stream.h:340

---

{#freeownedwritereq}

#### freeOwnedWriteReq

`inline`

```cpp
inline void freeOwnedWriteReq(OwnedWriteReq * req)
```

Defined in src/base/include/icy/stream.h:350

---

{#canqueuewrite}

#### canQueueWrite

`inline`

```cpp
inline bool canQueueWrite(size_t len)
```

Defined in src/base/include/icy/stream.h:361

### Public Types

| Name | Description |
|------|-------------|
| [`Handle`](#handle)  |  |

---

{#handle}

#### Handle

```cpp
using Handle = uv::Handle< T >
```

Defined in src/base/include/icy/stream.h:33

{#thread}

## Thread

```cpp
#include <icy/thread.h>
```

```cpp
class Thread
```

Defined in src/base/include/icy/thread.h:32

> **Inherits:** [`Runner`](#runner)

Platform-independent wrapper around an operating system thread.

This class inherits the `[Runner](#runner)` interface and may be used with any implementation that's powered by an asynchronous `[Runner](#runner)`.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Thread`](#thread)  | Default constructor. |
|  | [`Thread`](#thread) `inline` `explicit` | Constructs a `[Thread](#thread)` and immediately starts it with the given function and arguments. |
|  | [`~Thread`](#thread) `virtual` | Destructor. |
| `void` | [`start`](#start) `inline` | Starts the thread with a variadic function and arguments. The thread is started immediately; the previous thread must have exited before calling again. |
| `void` | [`start`](#start) `virtual` `override` | Starts the thread with a `std::function` callback. Overrides `[Runner::start](#classicy_1_1Runner_1a5969e823f1ce0bdd8730d3108bf13fbd)`; delegates to the variadic `[start](#classicy_1_1Thread_1ad1f9f47ae525aa1c772441987306190f)` template. |
| `void` | [`join`](#join)  | Wait until the thread exits. |
| `std::thread::id` | [`id`](#id) `const` | Return the native thread handle. |

---

{#thread}

#### Thread

```cpp
Thread()
```

Defined in src/base/include/icy/thread.h:38

Default constructor.

---

{#thread}

#### Thread

`inline` `explicit`

```cpp
template<typename Function, typename... Args> inline explicit Thread(Function && func, Args &&... args)
```

Defined in src/base/include/icy/thread.h:46

Constructs a `[Thread](#thread)` and immediately starts it with the given function and arguments. 
#### Parameters
* `Function` Callable type. 

* `Args` Argument types forwarded to the function. 

#### Parameters
* `func` Callable to execute on the new thread. 

* `args` Arguments forwarded to `func`.

---

{#thread}

#### ~Thread

`virtual`

```cpp
virtual ~Thread()
```

Defined in src/base/include/icy/thread.h:54

Destructor.

---

{#start}

#### start

`inline`

```cpp
template<typename Function, typename... Args> inline void start(Function && func, Args &&... args)
```

Defined in src/base/include/icy/thread.h:63

Starts the thread with a variadic function and arguments. The thread is started immediately; the previous thread must have exited before calling again. 
#### Parameters
* `Function` Callable type. 

* `Args` Argument types forwarded to the function. 

#### Parameters
* `func` Callable to execute on the new thread. 

* `args` Arguments forwarded to `func`.

---

{#start}

#### start

`virtual` `override`

```cpp
virtual void start(std::function< void()> func) override
```

Defined in src/base/include/icy/thread.h:73

Starts the thread with a `std::function` callback. Overrides `[Runner::start](#classicy_1_1Runner_1a5969e823f1ce0bdd8730d3108bf13fbd)`; delegates to the variadic `[start](#classicy_1_1Thread_1ad1f9f47ae525aa1c772441987306190f)` template. 
#### Parameters
* `func` Callable to execute on the new thread.

---

{#join}

#### join

```cpp
void join()
```

Defined in src/base/include/icy/thread.h:76

Wait until the thread exits.

---

{#id}

#### id

`const`

```cpp
std::thread::id id() const
```

Defined in src/base/include/icy/thread.h:79

Return the native thread handle.

### Public Static Attributes

| Return | Name | Description |
|--------|------|-------------|
| `const std::thread::id` | [`mainID`](#mainid) `static` | Accessor for the main thread ID. |

---

{#mainid}

#### mainID

`static`

```cpp
const std::thread::id mainID
```

Defined in src/base/include/icy/thread.h:85

Accessor for the main thread ID.

### Public Static Methods

| Return | Name | Description |
|--------|------|-------------|
| `std::thread::id` | [`currentID`](#currentid) `static` | Return the native thread ID of the current thread. |

---

{#currentid}

#### currentID

`static`

```cpp
static std::thread::id currentID()
```

Defined in src/base/include/icy/thread.h:82

Return the native thread ID of the current thread.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::thread` | [`_thread`](#_thread)  |  |

---

{#_thread}

#### _thread

```cpp
std::thread _thread
```

Defined in src/base/include/icy/thread.h:96

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Thread`](#thread)  | NonCopyable and NonMovable. |
|  | [`Thread`](#thread)  | Deleted constructor. |
| `bool` | [`async`](#async) `virtual` `const` `override` | Returns true if the implementation is thread-based. |

---

{#thread}

#### Thread

```cpp
Thread(const Thread &) = delete
```

Defined in src/base/include/icy/thread.h:89

NonCopyable and NonMovable.

---

{#thread}

#### Thread

```cpp
Thread(Thread &&) = delete
```

Defined in src/base/include/icy/thread.h:91

Deleted constructor.

---

{#async}

#### async

`virtual` `const` `override`

```cpp
virtual bool async() const override
```

Defined in src/base/include/icy/thread.h:94

Returns true if the implementation is thread-based. 
#### Returns
True for thread-backed runners, false for event-loop-driven runners.

### Public Types

| Name | Description |
|------|-------------|
| [`Ptr`](#ptr)  |  |

---

{#ptr}

#### Ptr

```cpp
using Ptr = std::shared_ptr< Thread >
```

Defined in src/base/include/icy/thread.h:35

{#process}

## Process

```cpp
#include <icy/process.h>
```

```cpp
class Process
```

Defined in src/base/include/icy/process.h:32

Spawns and manages a child process with stdin/stdout/stderr pipes.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`file`](#file)  | Path to the program to execute. Convenience proxy for options.file. Must be set before `[spawn()](#classicy_1_1Process_1a1d4f466c7f2713460ee35954dc6663bc)` |
| `std::string` | [`cwd`](#cwd)  | Set the current working directory. Convenience proxy for options.cwd. Must be set before `[spawn()](#classicy_1_1Process_1a1d4f466c7f2713460ee35954dc6663bc)` |
| `std::vector< std::string >` | [`args`](#args)  | Command line arguments to pass to the process. Convenience proxy for options.args. Must be set before `[spawn()](#classicy_1_1Process_1a1d4f466c7f2713460ee35954dc6663bc)` |
| `std::vector< std::string >` | [`env`](#env)  | Environment variables for the process. Each entry should be in "KEY=VALUE" format. If empty, the child inherits the parent environment. Must be set before `[spawn()](#classicy_1_1Process_1a1d4f466c7f2713460ee35954dc6663bc)` |
| `std::function< void(std::string)>` | [`onstdout`](#onstdout)  | Stdout callback. Called when a line has been output from the process. |
| `std::function< void(std::string)>` | [`onstderr`](#onstderr)  | Stderr callback. Called when a line has been output on stderr. |
| `std::function< void(std::int64_t)>` | [`onexit`](#onexit)  | Exit callback. Called with process exit status code. |
| `ProcessOptions` | [`options`](#options)  | LibUV C options. Available for advanced use cases. |

---

{#file}

#### file

```cpp
std::string file
```

Defined in src/base/include/icy/process.h:55

Path to the program to execute. Convenience proxy for options.file. Must be set before `[spawn()](#classicy_1_1Process_1a1d4f466c7f2713460ee35954dc6663bc)`

---

{#cwd}

#### cwd

```cpp
std::string cwd
```

Defined in src/base/include/icy/process.h:60

Set the current working directory. Convenience proxy for options.cwd. Must be set before `[spawn()](#classicy_1_1Process_1a1d4f466c7f2713460ee35954dc6663bc)`

---

{#args}

#### args

```cpp
std::vector< std::string > args
```

Defined in src/base/include/icy/process.h:65

Command line arguments to pass to the process. Convenience proxy for options.args. Must be set before `[spawn()](#classicy_1_1Process_1a1d4f466c7f2713460ee35954dc6663bc)`

---

{#env}

#### env

```cpp
std::vector< std::string > env
```

Defined in src/base/include/icy/process.h:71

Environment variables for the process. Each entry should be in "KEY=VALUE" format. If empty, the child inherits the parent environment. Must be set before `[spawn()](#classicy_1_1Process_1a1d4f466c7f2713460ee35954dc6663bc)`

---

{#onstdout}

#### onstdout

```cpp
std::function< void(std::string)> onstdout
```

Defined in src/base/include/icy/process.h:97

Stdout callback. Called when a line has been output from the process.

---

{#onstderr}

#### onstderr

```cpp
std::function< void(std::string)> onstderr
```

Defined in src/base/include/icy/process.h:101

Stderr callback. Called when a line has been output on stderr.

---

{#onexit}

#### onexit

```cpp
std::function< void(std::int64_t)> onexit
```

Defined in src/base/include/icy/process.h:105

Exit callback. Called with process exit status code.

---

{#options}

#### options

```cpp
ProcessOptions options
```

Defined in src/base/include/icy/process.h:109

LibUV C options. Available for advanced use cases.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Process`](#process)  | Constructs a `[Process](#process)` attached to the given event loop. |
|  | [`Process`](#process)  | Constructs a `[Process](#process)` with initial command-line arguments. |
|  | [`~Process`](#process)  | Destructor. |
|  | [`Process`](#process)  | Deleted constructor. |
|  | [`Process`](#process)  | Deleted constructor. |
| `void` | [`spawn`](#spawn)  | Spawns the process. Options must be properly set. Throws an exception on error. |
| `bool` | [`kill`](#kill)  | Sends a signal to the process. |
| `int` | [`pid`](#pid) `const` | Returns the process PID, or 0 if not spawned. |
| `Pipe &` | [`in`](#in)  | Returns the stdin pipe. |
| `Pipe &` | [`out`](#out)  | Returns the stdout pipe. |
| `Pipe &` | [`err`](#err)  | Returns the stderr pipe. |

---

{#process}

#### Process

```cpp
Process(uv::Loop * loop = uv::defaultLoop())
```

Defined in src/base/include/icy/process.h:37

Constructs a `[Process](#process)` attached to the given event loop. 
#### Parameters
* `loop` Event loop to use for I/O and exit notifications. Defaults to the default loop.

---

{#process}

#### Process

```cpp
Process(std::initializer_list< std::string > args, uv::Loop * loop = uv::defaultLoop())
```

Defined in src/base/include/icy/process.h:42

Constructs a `[Process](#process)` with initial command-line arguments. 
#### Parameters
* `args` Initializer list of argument strings. The first element is typically the executable path. 

* `loop` Event loop to use for I/O and exit notifications. Defaults to the default loop.

---

{#process}

#### ~Process

```cpp
~Process()
```

Defined in src/base/include/icy/process.h:45

Destructor.

---

{#process}

#### Process

```cpp
Process(const Process &) = delete
```

Defined in src/base/include/icy/process.h:47

Deleted constructor.

---

{#process}

#### Process

```cpp
Process(Process &&) = delete
```

Defined in src/base/include/icy/process.h:49

Deleted constructor.

---

{#spawn}

#### spawn

```cpp
void spawn()
```

Defined in src/base/include/icy/process.h:76

Spawns the process. Options must be properly set. Throws an exception on error.

---

{#kill}

#### kill

```cpp
bool kill(int signum = SIGTERM)
```

Defined in src/base/include/icy/process.h:81

Sends a signal to the process. 
#### Parameters
* `signum` [Signal](#signal) number to send (default: `SIGTERM`). 

#### Returns
True if the signal was sent successfully, false if the process is not running or handle is invalid.

---

{#pid}

#### pid

`const`

```cpp
int pid() const
```

Defined in src/base/include/icy/process.h:84

Returns the process PID, or 0 if not spawned.

---

{#in}

#### in

```cpp
Pipe & in()
```

Defined in src/base/include/icy/process.h:87

Returns the stdin pipe.

---

{#out}

#### out

```cpp
Pipe & out()
```

Defined in src/base/include/icy/process.h:90

Returns the stdout pipe.

---

{#err}

#### err

```cpp
Pipe & err()
```

Defined in src/base/include/icy/process.h:93

Returns the stderr pipe.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `uv::Handle< uv_process_t >` | [`_handle`](#_handle)  |  |
| `Pipe` | [`_stdin`](#_stdin)  |  |
| `Pipe` | [`_stdout`](#_stdout)  |  |
| `Pipe` | [`_stderr`](#_stderr)  |  |
| `uv_stdio_container_t` | [`_stdio`](#_stdio)  |  |
| `std::vector< char * >` | [`_cargs`](#_cargs)  |  |
| `std::vector< char * >` | [`_cenv`](#_cenv)  |  |

---

{#_handle}

#### _handle

```cpp
uv::Handle< uv_process_t > _handle
```

Defined in src/base/include/icy/process.h:114

---

{#_stdin}

#### _stdin

```cpp
Pipe _stdin
```

Defined in src/base/include/icy/process.h:115

---

{#_stdout}

#### _stdout

```cpp
Pipe _stdout
```

Defined in src/base/include/icy/process.h:116

---

{#_stderr}

#### _stderr

```cpp
Pipe _stderr
```

Defined in src/base/include/icy/process.h:117

---

{#_stdio}

#### _stdio

```cpp
uv_stdio_container_t _stdio
```

Defined in src/base/include/icy/process.h:118

---

{#_cargs}

#### _cargs

```cpp
std::vector< char * > _cargs
```

Defined in src/base/include/icy/process.h:119

---

{#_cenv}

#### _cenv

```cpp
std::vector< char * > _cenv
```

Defined in src/base/include/icy/process.h:120

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`init`](#init)  |  |

---

{#init}

#### init

```cpp
void init()
```

Defined in src/base/include/icy/process.h:112

{#timeout}

## Timeout

```cpp
#include <icy/timeout.h>
```

```cpp
class Timeout
```

Defined in src/base/include/icy/timeout.h:32

> **Subclassed by:** [`TimedToken`](#timedtoken)

[Timeout](#timeout) counter which expires after a given delay. Delay is specified in milliseconds.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Timeout`](#timeout)  | Constructs a [Timeout](#timeout) with the given delay. |
|  | [`Timeout`](#timeout)  | Copy constructor. |
|  | [`Timeout`](#timeout) `noexcept` | Defaulted constructor. |
| `bool` | [`running`](#running) `const` | Returns true if the timer is currently running. |
| `void` | [`start`](#start)  | Starts (or restarts) the timer, recording the current time as the start point. |
| `void` | [`stop`](#stop)  | Stops the timer without resetting it. [expired()](#classicy_1_1Timeout_1a9f4b4b19851da30eca70e18fd369a2e7) will return false after this call. |
| `void` | [`reset`](#reset)  | Restarts the timer from now, equivalent to calling [start()](#classicy_1_1Timeout_1a53c636b9d861ad0fcc4051fdc2fb007e). |
| `long` | [`remaining`](#remaining) `const` | Returns the number of milliseconds remaining before expiry. Returns 0 if already expired, or the full delay if not running. |
| `bool` | [`expired`](#expired) `const` | Returns true if the timer is running and the delay has fully elapsed. |
| `void` | [`setDelay`](#setdelay) `inline` | Sets the expiry delay without restarting the timer. |
| `long` | [`delay`](#delay) `const` `inline` | Returns the configured delay in milliseconds. |
| `Timeout &` | [`operator=`](#operator)  | Copy assignment operator. |

---

{#timeout}

#### Timeout

```cpp
Timeout(long delay = 0, bool autoStart = false)
```

Defined in src/base/include/icy/timeout.h:38

Constructs a [Timeout](#timeout) with the given delay. 
#### Parameters
* `delay` Expiry duration in milliseconds (default: 0). 

* `autoStart` If true, starts the timer immediately on construction.

---

{#timeout}

#### Timeout

```cpp
Timeout(const Timeout & src)
```

Defined in src/base/include/icy/timeout.h:42

Copy constructor. 
#### Parameters
* `src` Source [Timeout](#timeout) to copy state from.

---

{#timeout}

#### Timeout

`noexcept`

```cpp
Timeout(Timeout && src) noexcept = default
```

Defined in src/base/include/icy/timeout.h:44

Defaulted constructor.

---

{#running}

#### running

`const`

```cpp
bool running() const
```

Defined in src/base/include/icy/timeout.h:49

Returns true if the timer is currently running. 
#### Returns
true if [start()](#classicy_1_1Timeout_1a53c636b9d861ad0fcc4051fdc2fb007e) has been called and [stop()](#classicy_1_1Timeout_1a3015db4e29b2d32b54d3816255b61cba) has not.

---

{#start}

#### start

```cpp
void start()
```

Defined in src/base/include/icy/timeout.h:52

Starts (or restarts) the timer, recording the current time as the start point.

---

{#stop}

#### stop

```cpp
void stop()
```

Defined in src/base/include/icy/timeout.h:55

Stops the timer without resetting it. [expired()](#classicy_1_1Timeout_1a9f4b4b19851da30eca70e18fd369a2e7) will return false after this call.

---

{#reset}

#### reset

```cpp
void reset()
```

Defined in src/base/include/icy/timeout.h:58

Restarts the timer from now, equivalent to calling [start()](#classicy_1_1Timeout_1a53c636b9d861ad0fcc4051fdc2fb007e).

---

{#remaining}

#### remaining

`const`

```cpp
long remaining() const
```

Defined in src/base/include/icy/timeout.h:63

Returns the number of milliseconds remaining before expiry. Returns 0 if already expired, or the full delay if not running. 
#### Returns
Milliseconds until expiry.

---

{#expired}

#### expired

`const`

```cpp
bool expired() const
```

Defined in src/base/include/icy/timeout.h:67

Returns true if the timer is running and the delay has fully elapsed. 
#### Returns
true if expired, false if stopped or not yet elapsed.

---

{#setdelay}

#### setDelay

`inline`

```cpp
inline void setDelay(long delay)
```

Defined in src/base/include/icy/timeout.h:71

Sets the expiry delay without restarting the timer. 
#### Parameters
* `delay` New delay in milliseconds.

---

{#delay}

#### delay

`const` `inline`

```cpp
inline long delay() const
```

Defined in src/base/include/icy/timeout.h:75

Returns the configured delay in milliseconds. 
#### Returns
Delay in milliseconds.

---

{#operator}

#### operator=

```cpp
Timeout & operator=(const Timeout & src)
```

Defined in src/base/include/icy/timeout.h:79

Copy assignment operator. 
#### Parameters
* `src` Source [Timeout](#timeout) to copy state from.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::chrono::steady_clock::time_point` | [`_startAt`](#_startat)  |  |
| `long` | [`_delay`](#_delay)  |  |
| `bool` | [`_running`](#_running)  |  |

---

{#_startat}

#### _startAt

```cpp
std::chrono::steady_clock::time_point _startAt
```

Defined in src/base/include/icy/timeout.h:83

---

{#_delay}

#### _delay

```cpp
long _delay
```

Defined in src/base/include/icy/timeout.h:84

---

{#_running}

#### _running

```cpp
bool _running
```

Defined in src/base/include/icy/timeout.h:85

{#timedtoken}

## TimedToken

```cpp
#include <icy/timeout.h>
```

```cpp
class TimedToken
```

Defined in src/base/include/icy/timeout.h:95

> **Inherits:** [`Timeout`](#timeout)

Token that expires after the specified duration.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`TimedToken`](#timedtoken) `explicit` | Constructs a [TimedToken](#timedtoken) with a randomly generated 32-character ID, started immediately with the given duration. |
|  | [`TimedToken`](#timedtoken) `explicit` | Constructs a [TimedToken](#timedtoken) with an explicit ID, started immediately. |
| `std::string` | [`id`](#id) `const` `inline` | Returns the token's identifier string. |
| `bool` | [`operator==`](#operator) `const` `inline` | Compares two tokens by ID. |
| `bool` | [`operator==`](#operator) `const` `inline` | Compares this token's ID against a string. |

---

{#timedtoken}

#### TimedToken

`explicit`

```cpp
explicit TimedToken(long duration = 1000)
```

Defined in src/base/include/icy/timeout.h:101

Constructs a [TimedToken](#timedtoken) with a randomly generated 32-character ID, started immediately with the given duration. 
#### Parameters
* `duration` Lifetime in milliseconds (default: 1000).

---

{#timedtoken}

#### TimedToken

`explicit`

```cpp
explicit TimedToken(const std::string & id, long duration = 1000)
```

Defined in src/base/include/icy/timeout.h:106

Constructs a [TimedToken](#timedtoken) with an explicit ID, started immediately. 
#### Parameters
* `id` Token identifier. 

* `duration` Lifetime in milliseconds (default: 1000).

---

{#id}

#### id

`const` `inline`

```cpp
inline std::string id() const
```

Defined in src/base/include/icy/timeout.h:110

Returns the token's identifier string. 
#### Returns
Token ID.

---

{#operator}

#### operator==

`const` `inline`

```cpp
inline bool operator==(const TimedToken & r) const
```

Defined in src/base/include/icy/timeout.h:113

Compares two tokens by ID.

---

{#operator}

#### operator==

`const` `inline`

```cpp
inline bool operator==(std::string_view r) const
```

Defined in src/base/include/icy/timeout.h:116

Compares this token's ID against a string.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`_id`](#_id)  |  |

---

{#_id}

#### _id

```cpp
std::string _id
```

Defined in src/base/include/icy/timeout.h:119

{#timestamp}

## Timestamp

```cpp
#include <icy/datetime.h>
```

```cpp
class Timestamp
```

Defined in src/base/include/icy/datetime.h:36

A [Timestamp](#timestamp) stores a monotonic* time value with (theoretical) microseconds resolution. Timestamps can be compared with each other and simple arithmetics are supported.

[*] Note that [Timestamp](#timestamp) values are only monotonic as long as the systems's clock is monotonic as well (and not, e.g. set back).

Timestamps are UTC (Coordinated Universal Time) based and thus independent of the timezone in effect on the system.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Timestamp`](#timestamp)  | Creates a timestamp with the current time. |
|  | [`Timestamp`](#timestamp)  | Creates a timestamp from the given time value. |
|  | [`Timestamp`](#timestamp)  | Copy constructor. |
|  | [`~Timestamp`](#timestamp)  | Destroys the timestamp. |
| `void` | [`swap`](#swap)  | Swaps the [Timestamp](#timestamp) with another one. |
| `void` | [`update`](#update)  | Updates the [Timestamp](#timestamp) with the current time. |
| `bool` | [`operator==`](#operator) `const` `inline` |  |
| `bool` | [`operator!=`](#operator) `const` `inline` |  |
| `bool` | [`operator>`](#operator) `const` `inline` |  |
| `bool` | [`operator>=`](#operator) `const` `inline` |  |
| `bool` | [`operator<`](#operator) `const` `inline` |  |
| `bool` | [`operator<=`](#operator) `const` `inline` |  |
| `Timestamp` | [`operator+`](#operator) `const` `inline` |  |
| `Timestamp` | [`operator-`](#operator) `const` `inline` |  |
| `TimeDiff` | [`operator-`](#operator) `const` `inline` |  |
| `Timestamp &` | [`operator+=`](#operator) `inline` |  |
| `Timestamp &` | [`operator-=`](#operator) `inline` |  |
| `std::time_t` | [`epochTime`](#epochtime) `const` `inline` | Returns the timestamp expressed in time_t. time_t base time is midnight, January 1, 1970. Resolution is one second. |
| `UtcTimeVal` | [`utcTime`](#utctime) `const` `inline` | Returns the timestamp expressed in UTC-based time. UTC base time is midnight, October 15, 1582. Resolution is 100 nanoseconds. |
| `TimeVal` | [`epochMicroseconds`](#epochmicroseconds) `const` `inline` | Returns the timestamp expressed in microseconds since the Unix epoch, midnight, January 1, 1970. |
| `TimeDiff` | [`elapsed`](#elapsed) `const` `inline` | Returns the time elapsed since the time denoted by the timestamp. Equivalent to [Timestamp()](#classicy_1_1Timestamp_1a772b4486bcf286983804ddcd6ff4291b) - *this. |
| `bool` | [`isElapsed`](#iselapsed) `const` `inline` | Returns true iff the given interval has passed since the time denoted by the timestamp. |

---

{#timestamp}

#### Timestamp

```cpp
Timestamp()
```

Defined in src/base/include/icy/datetime.h:44

Creates a timestamp with the current time.

---

{#timestamp}

#### Timestamp

```cpp
Timestamp(TimeVal tv)
```

Defined in src/base/include/icy/datetime.h:47

Creates a timestamp from the given time value.

---

{#timestamp}

#### Timestamp

```cpp
Timestamp(const Timestamp & other)
```

Defined in src/base/include/icy/datetime.h:50

Copy constructor.

---

{#timestamp}

#### ~Timestamp

```cpp
~Timestamp()
```

Defined in src/base/include/icy/datetime.h:53

Destroys the timestamp.

---

{#swap}

#### swap

```cpp
void swap(Timestamp & timestamp)
```

Defined in src/base/include/icy/datetime.h:59

Swaps the [Timestamp](#timestamp) with another one.

---

{#update}

#### update

```cpp
void update()
```

Defined in src/base/include/icy/datetime.h:62

Updates the [Timestamp](#timestamp) with the current time.

---

{#operator}

#### operator==

`const` `inline`

```cpp
inline bool operator==(const Timestamp & ts) const
```

Defined in src/base/include/icy/datetime.h:64

---

{#operator}

#### operator!=

`const` `inline`

```cpp
inline bool operator!=(const Timestamp & ts) const
```

Defined in src/base/include/icy/datetime.h:65

---

{#operator}

#### operator>

`const` `inline`

```cpp
inline bool operator>(const Timestamp & ts) const
```

Defined in src/base/include/icy/datetime.h:66

---

{#operator}

#### operator>=

`const` `inline`

```cpp
inline bool operator>=(const Timestamp & ts) const
```

Defined in src/base/include/icy/datetime.h:67

---

{#operator}

#### operator<

`const` `inline`

```cpp
inline bool operator<(const Timestamp & ts) const
```

Defined in src/base/include/icy/datetime.h:68

---

{#operator}

#### operator<=

`const` `inline`

```cpp
inline bool operator<=(const Timestamp & ts) const
```

Defined in src/base/include/icy/datetime.h:69

---

{#operator}

#### operator+

`const` `inline`

```cpp
inline Timestamp operator+(TimeDiff d) const
```

Defined in src/base/include/icy/datetime.h:71

---

{#operator}

#### operator-

`const` `inline`

```cpp
inline Timestamp operator-(TimeDiff d) const
```

Defined in src/base/include/icy/datetime.h:72

---

{#operator}

#### operator-

`const` `inline`

```cpp
inline TimeDiff operator-(const Timestamp & ts) const
```

Defined in src/base/include/icy/datetime.h:73

---

{#operator}

#### operator+=

`inline`

```cpp
inline Timestamp & operator+=(TimeDiff d)
```

Defined in src/base/include/icy/datetime.h:74

---

{#operator}

#### operator-=

`inline`

```cpp
inline Timestamp & operator-=(TimeDiff d)
```

Defined in src/base/include/icy/datetime.h:75

---

{#epochtime}

#### epochTime

`const` `inline`

```cpp
inline std::time_t epochTime() const
```

Defined in src/base/include/icy/datetime.h:80

Returns the timestamp expressed in time_t. time_t base time is midnight, January 1, 1970. Resolution is one second.

---

{#utctime}

#### utcTime

`const` `inline`

```cpp
inline UtcTimeVal utcTime() const
```

Defined in src/base/include/icy/datetime.h:85

Returns the timestamp expressed in UTC-based time. UTC base time is midnight, October 15, 1582. Resolution is 100 nanoseconds.

---

{#epochmicroseconds}

#### epochMicroseconds

`const` `inline`

```cpp
inline TimeVal epochMicroseconds() const
```

Defined in src/base/include/icy/datetime.h:89

Returns the timestamp expressed in microseconds since the Unix epoch, midnight, January 1, 1970.

---

{#elapsed}

#### elapsed

`const` `inline`

```cpp
inline TimeDiff elapsed() const
```

Defined in src/base/include/icy/datetime.h:93

Returns the time elapsed since the time denoted by the timestamp. Equivalent to [Timestamp()](#classicy_1_1Timestamp_1a772b4486bcf286983804ddcd6ff4291b) - *this.

---

{#iselapsed}

#### isElapsed

`const` `inline`

```cpp
inline bool isElapsed(TimeDiff interval) const
```

Defined in src/base/include/icy/datetime.h:97

Returns true iff the given interval has passed since the time denoted by the timestamp.

### Public Static Methods

| Return | Name | Description |
|--------|------|-------------|
| `Timestamp` | [`fromEpochTime`](#fromepochtime) `static` | Creates a timestamp from a std::time_t. |
| `Timestamp` | [`fromUtcTime`](#fromutctime) `static` | Creates a timestamp from a UTC time value. |
| `TimeVal` | [`resolution`](#resolution) `static` `inline` | Returns the resolution in units per second. Since the timestamp has microsecond resolution, the returned value is always 1000000. |

---

{#fromepochtime}

#### fromEpochTime

`static`

```cpp
static Timestamp fromEpochTime(std::time_t t)
```

Defined in src/base/include/icy/datetime.h:100

Creates a timestamp from a std::time_t.

---

{#fromutctime}

#### fromUtcTime

`static`

```cpp
static Timestamp fromUtcTime(UtcTimeVal val)
```

Defined in src/base/include/icy/datetime.h:103

Creates a timestamp from a UTC time value.

---

{#resolution}

#### resolution

`static` `inline`

```cpp
static inline TimeVal resolution()
```

Defined in src/base/include/icy/datetime.h:108

Returns the resolution in units per second. Since the timestamp has microsecond resolution, the returned value is always 1000000.

### Public Types

| Name | Description |
|------|-------------|
| [`TimeVal`](#timeval)  | monotonic UTC time value in microsecond resolution |
| [`UtcTimeVal`](#utctimeval)  | monotonic UTC time value in 100 nanosecond resolution |
| [`TimeDiff`](#timediff)  | difference between two timestamps in microseconds |

---

{#timeval}

#### TimeVal

```cpp
using TimeVal = std::int64_t
```

Defined in src/base/include/icy/datetime.h:39

monotonic UTC time value in microsecond resolution

---

{#utctimeval}

#### UtcTimeVal

```cpp
using UtcTimeVal = std::int64_t
```

Defined in src/base/include/icy/datetime.h:40

monotonic UTC time value in 100 nanosecond resolution

---

{#timediff}

#### TimeDiff

```cpp
using TimeDiff = std::int64_t
```

Defined in src/base/include/icy/datetime.h:41

difference between two timestamps in microseconds

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `TimeVal` | [`_ts`](#_ts)  |  |

---

{#_ts}

#### _ts

```cpp
TimeVal _ts
```

Defined in src/base/include/icy/datetime.h:111

{#timespan}

## Timespan

```cpp
#include <icy/datetime.h>
```

```cpp
class Timespan
```

Defined in src/base/include/icy/datetime.h:119

A class that represents time spans up to microsecond resolution.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Timespan`](#timespan)  | Creates a zero [Timespan](#timespan). |
|  | [`Timespan`](#timespan)  | Creates a [Timespan](#timespan). |
|  | [`Timespan`](#timespan)  | Creates a [Timespan](#timespan). Useful for creating a [Timespan](#timespan) from a struct timeval. |
|  | [`Timespan`](#timespan)  | Creates a [Timespan](#timespan). |
|  | [`Timespan`](#timespan)  | Creates a [Timespan](#timespan) from another one. |
|  | [`~Timespan`](#timespan)  | Destroys the [Timespan](#timespan). |
| `Timespan &` | [`operator=`](#operator)  | Assignment operator. |
| `Timespan &` | [`operator=`](#operator)  | Assignment operator. |
| `Timespan &` | [`assign`](#assign)  | Assigns a new span. |
| `Timespan &` | [`assign`](#assign)  | Assigns a new span. Useful for assigning from a struct timeval. |
| `void` | [`swap`](#swap)  | Swaps the [Timespan](#timespan) with another one. |
| `bool` | [`operator==`](#operator) `const` `inline` |  |
| `bool` | [`operator!=`](#operator) `const` `inline` |  |
| `bool` | [`operator>`](#operator) `const` `inline` |  |
| `bool` | [`operator>=`](#operator) `const` `inline` |  |
| `bool` | [`operator<`](#operator) `const` `inline` |  |
| `bool` | [`operator<=`](#operator) `const` `inline` |  |
| `bool` | [`operator==`](#operator) `const` `inline` |  |
| `bool` | [`operator!=`](#operator) `const` `inline` |  |
| `bool` | [`operator>`](#operator) `const` `inline` |  |
| `bool` | [`operator>=`](#operator) `const` `inline` |  |
| `bool` | [`operator<`](#operator) `const` `inline` |  |
| `bool` | [`operator<=`](#operator) `const` `inline` |  |
| `Timespan` | [`operator+`](#operator) `const` |  |
| `Timespan` | [`operator-`](#operator) `const` |  |
| `Timespan &` | [`operator+=`](#operator)  |  |
| `Timespan &` | [`operator-=`](#operator)  |  |
| `Timespan` | [`operator+`](#operator) `const` |  |
| `Timespan` | [`operator-`](#operator) `const` |  |
| `Timespan &` | [`operator+=`](#operator)  |  |
| `Timespan &` | [`operator-=`](#operator)  |  |
| `int` | [`days`](#days) `const` `inline` | Returns the number of days. |
| `int` | [`hours`](#hours) `const` `inline` | Returns the number of hours (0 to 23). |
| `int` | [`totalHours`](#totalhours) `const` `inline` | Returns the total number of hours. |
| `int` | [`minutes`](#minutes) `const` `inline` | Returns the number of minutes (0 to 59). |
| `int` | [`totalMinutes`](#totalminutes) `const` `inline` | Returns the total number of minutes. |
| `int` | [`seconds`](#seconds) `const` `inline` | Returns the number of seconds (0 to 59). |
| `int` | [`totalSeconds`](#totalseconds) `const` `inline` | Returns the total number of seconds. |
| `int` | [`milliseconds`](#milliseconds) `const` `inline` | Returns the number of milliseconds (0 to 999). |
| `TimeDiff` | [`totalMilliseconds`](#totalmilliseconds) `const` `inline` | Returns the total number of milliseconds. |
| `int` | [`microseconds`](#microseconds) `const` `inline` | Returns the fractions of a millisecond in microseconds (0 to 999). |
| `int` | [`useconds`](#useconds) `const` `inline` | Returns the fractions of a second in microseconds (0 to 999999). |
| `TimeDiff` | [`totalMicroseconds`](#totalmicroseconds) `const` `inline` | Returns the total number of microseconds. |

---

{#timespan}

#### Timespan

```cpp
Timespan()
```

Defined in src/base/include/icy/datetime.h:125

Creates a zero [Timespan](#timespan).

---

{#timespan}

#### Timespan

```cpp
Timespan(TimeDiff microseconds)
```

Defined in src/base/include/icy/datetime.h:128

Creates a [Timespan](#timespan).

---

{#timespan}

#### Timespan

```cpp
Timespan(long seconds, long microseconds)
```

Defined in src/base/include/icy/datetime.h:132

Creates a [Timespan](#timespan). Useful for creating a [Timespan](#timespan) from a struct timeval.

---

{#timespan}

#### Timespan

```cpp
Timespan(int days, int hours, int minutes, int seconds, int microseconds)
```

Defined in src/base/include/icy/datetime.h:135

Creates a [Timespan](#timespan).

---

{#timespan}

#### Timespan

```cpp
Timespan(const Timespan & timespan)
```

Defined in src/base/include/icy/datetime.h:138

Creates a [Timespan](#timespan) from another one.

---

{#timespan}

#### ~Timespan

```cpp
~Timespan()
```

Defined in src/base/include/icy/datetime.h:141

Destroys the [Timespan](#timespan).

---

{#operator}

#### operator=

```cpp
Timespan & operator=(const Timespan & timespan)
```

Defined in src/base/include/icy/datetime.h:144

Assignment operator.

---

{#operator}

#### operator=

```cpp
Timespan & operator=(TimeDiff microseconds)
```

Defined in src/base/include/icy/datetime.h:147

Assignment operator.

---

{#assign}

#### assign

```cpp
Timespan & assign(int days, int hours, int minutes, int seconds, int microseconds)
```

Defined in src/base/include/icy/datetime.h:150

Assigns a new span.

---

{#assign}

#### assign

```cpp
Timespan & assign(long seconds, long microseconds)
```

Defined in src/base/include/icy/datetime.h:154

Assigns a new span. Useful for assigning from a struct timeval.

---

{#swap}

#### swap

```cpp
void swap(Timespan & timespan)
```

Defined in src/base/include/icy/datetime.h:157

Swaps the [Timespan](#timespan) with another one.

---

{#operator}

#### operator==

`const` `inline`

```cpp
inline bool operator==(const Timespan & ts) const
```

Defined in src/base/include/icy/datetime.h:159

---

{#operator}

#### operator!=

`const` `inline`

```cpp
inline bool operator!=(const Timespan & ts) const
```

Defined in src/base/include/icy/datetime.h:160

---

{#operator}

#### operator>

`const` `inline`

```cpp
inline bool operator>(const Timespan & ts) const
```

Defined in src/base/include/icy/datetime.h:161

---

{#operator}

#### operator>=

`const` `inline`

```cpp
inline bool operator>=(const Timespan & ts) const
```

Defined in src/base/include/icy/datetime.h:162

---

{#operator}

#### operator<

`const` `inline`

```cpp
inline bool operator<(const Timespan & ts) const
```

Defined in src/base/include/icy/datetime.h:163

---

{#operator}

#### operator<=

`const` `inline`

```cpp
inline bool operator<=(const Timespan & ts) const
```

Defined in src/base/include/icy/datetime.h:164

---

{#operator}

#### operator==

`const` `inline`

```cpp
inline bool operator==(TimeDiff microseconds) const
```

Defined in src/base/include/icy/datetime.h:166

---

{#operator}

#### operator!=

`const` `inline`

```cpp
inline bool operator!=(TimeDiff microseconds) const
```

Defined in src/base/include/icy/datetime.h:167

---

{#operator}

#### operator>

`const` `inline`

```cpp
inline bool operator>(TimeDiff microseconds) const
```

Defined in src/base/include/icy/datetime.h:168

---

{#operator}

#### operator>=

`const` `inline`

```cpp
inline bool operator>=(TimeDiff microseconds) const
```

Defined in src/base/include/icy/datetime.h:169

---

{#operator}

#### operator<

`const` `inline`

```cpp
inline bool operator<(TimeDiff microseconds) const
```

Defined in src/base/include/icy/datetime.h:170

---

{#operator}

#### operator<=

`const` `inline`

```cpp
inline bool operator<=(TimeDiff microseconds) const
```

Defined in src/base/include/icy/datetime.h:171

---

{#operator}

#### operator+

`const`

```cpp
Timespan operator+(const Timespan & d) const
```

Defined in src/base/include/icy/datetime.h:173

---

{#operator}

#### operator-

`const`

```cpp
Timespan operator-(const Timespan & d) const
```

Defined in src/base/include/icy/datetime.h:174

---

{#operator}

#### operator+=

```cpp
Timespan & operator+=(const Timespan & d)
```

Defined in src/base/include/icy/datetime.h:175

---

{#operator}

#### operator-=

```cpp
Timespan & operator-=(const Timespan & d)
```

Defined in src/base/include/icy/datetime.h:176

---

{#operator}

#### operator+

`const`

```cpp
Timespan operator+(TimeDiff microseconds) const
```

Defined in src/base/include/icy/datetime.h:178

---

{#operator}

#### operator-

`const`

```cpp
Timespan operator-(TimeDiff microseconds) const
```

Defined in src/base/include/icy/datetime.h:179

---

{#operator}

#### operator+=

```cpp
Timespan & operator+=(TimeDiff microseconds)
```

Defined in src/base/include/icy/datetime.h:180

---

{#operator}

#### operator-=

```cpp
Timespan & operator-=(TimeDiff microseconds)
```

Defined in src/base/include/icy/datetime.h:181

---

{#days}

#### days

`const` `inline`

```cpp
inline int days() const
```

Defined in src/base/include/icy/datetime.h:184

Returns the number of days.

---

{#hours}

#### hours

`const` `inline`

```cpp
inline int hours() const
```

Defined in src/base/include/icy/datetime.h:187

Returns the number of hours (0 to 23).

---

{#totalhours}

#### totalHours

`const` `inline`

```cpp
inline int totalHours() const
```

Defined in src/base/include/icy/datetime.h:190

Returns the total number of hours.

---

{#minutes}

#### minutes

`const` `inline`

```cpp
inline int minutes() const
```

Defined in src/base/include/icy/datetime.h:193

Returns the number of minutes (0 to 59).

---

{#totalminutes}

#### totalMinutes

`const` `inline`

```cpp
inline int totalMinutes() const
```

Defined in src/base/include/icy/datetime.h:196

Returns the total number of minutes.

---

{#seconds}

#### seconds

`const` `inline`

```cpp
inline int seconds() const
```

Defined in src/base/include/icy/datetime.h:199

Returns the number of seconds (0 to 59).

---

{#totalseconds}

#### totalSeconds

`const` `inline`

```cpp
inline int totalSeconds() const
```

Defined in src/base/include/icy/datetime.h:202

Returns the total number of seconds.

---

{#milliseconds}

#### milliseconds

`const` `inline`

```cpp
inline int milliseconds() const
```

Defined in src/base/include/icy/datetime.h:205

Returns the number of milliseconds (0 to 999).

---

{#totalmilliseconds}

#### totalMilliseconds

`const` `inline`

```cpp
inline TimeDiff totalMilliseconds() const
```

Defined in src/base/include/icy/datetime.h:208

Returns the total number of milliseconds.

---

{#microseconds}

#### microseconds

`const` `inline`

```cpp
inline int microseconds() const
```

Defined in src/base/include/icy/datetime.h:212

Returns the fractions of a millisecond in microseconds (0 to 999).

---

{#useconds}

#### useconds

`const` `inline`

```cpp
inline int useconds() const
```

Defined in src/base/include/icy/datetime.h:216

Returns the fractions of a second in microseconds (0 to 999999).

---

{#totalmicroseconds}

#### totalMicroseconds

`const` `inline`

```cpp
inline TimeDiff totalMicroseconds() const
```

Defined in src/base/include/icy/datetime.h:219

Returns the total number of microseconds.

### Public Static Attributes

| Return | Name | Description |
|--------|------|-------------|
| `const TimeDiff` | [`MILLISECONDS`](#milliseconds) `static` | The number of microseconds in a millisecond. |
| `const TimeDiff` | [`SECONDS`](#seconds) `static` | The number of microseconds in a second. |
| `const TimeDiff` | [`MINUTES`](#minutes) `static` | The number of microseconds in a minute. |
| `const TimeDiff` | [`HOURS`](#hours) `static` | The number of microseconds in a hour. |
| `const TimeDiff` | [`DAYS`](#days) `static` | The number of microseconds in a day. |

---

{#milliseconds}

#### MILLISECONDS

`static`

```cpp
const TimeDiff MILLISECONDS
```

Defined in src/base/include/icy/datetime.h:221

The number of microseconds in a millisecond.

---

{#seconds}

#### SECONDS

`static`

```cpp
const TimeDiff SECONDS
```

Defined in src/base/include/icy/datetime.h:222

The number of microseconds in a second.

---

{#minutes}

#### MINUTES

`static`

```cpp
const TimeDiff MINUTES
```

Defined in src/base/include/icy/datetime.h:223

The number of microseconds in a minute.

---

{#hours}

#### HOURS

`static`

```cpp
const TimeDiff HOURS
```

Defined in src/base/include/icy/datetime.h:224

The number of microseconds in a hour.

---

{#days}

#### DAYS

`static`

```cpp
const TimeDiff DAYS
```

Defined in src/base/include/icy/datetime.h:225

The number of microseconds in a day.

### Public Types

| Name | Description |
|------|-------------|
| [`TimeDiff`](#timediff)  |  |

---

{#timediff}

#### TimeDiff

```cpp
using TimeDiff = Timestamp::TimeDiff
```

Defined in src/base/include/icy/datetime.h:122

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `TimeDiff` | [`_span`](#_span)  |  |

---

{#_span}

#### _span

```cpp
TimeDiff _span
```

Defined in src/base/include/icy/datetime.h:228

{#datetime}

## DateTime

```cpp
#include <icy/datetime.h>
```

```cpp
class DateTime
```

Defined in src/base/include/icy/datetime.h:265

This class represents an instant in time, expressed in years, months, days, hours, minutes, seconds and milliseconds based on the Gregorian calendar. The class is mainly useful for conversions between UTC, Julian day and Gregorian calendar dates.

The date and time stored in a [DateTime](#datetime) is always in UTC (Coordinated Universal Time) and thus independent of the timezone in effect on the system.

Conversion calculations are based on algorithms collected and described by Peter Baum at [http://vsg.cape.com/~pbaum/date/date0.htm](http://vsg.cape.com/~pbaum/date/date0.htm)

Internally, this class stores a date/time in two forms (UTC and broken down) for performance reasons. Only use this class for conversions between date/time representations. Use the [Timestamp](#timestamp) class for everything else.

Notes:

* Zero is a valid year (in accordance with ISO 8601 and astronomical year numbering)

* Year zero (0) is a leap year

* Negative years (years preceding 1 BC) are not supported

For more information, please see:

* [http://en.wikipedia.org/wiki/Gregorian_Calendar](http://en.wikipedia.org/wiki/Gregorian_Calendar)

* [http://en.wikipedia.org/wiki/Julian_day](http://en.wikipedia.org/wiki/Julian_day)

* [http://en.wikipedia.org/wiki/UTC](http://en.wikipedia.org/wiki/UTC)

* [http://en.wikipedia.org/wiki/ISO_8601](http://en.wikipedia.org/wiki/ISO_8601)

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`DateTime`](#datetime)  | Creates a [DateTime](#datetime) for the current date and time. |
|  | [`DateTime`](#datetime)  | Creates a [DateTime](#datetime) for the date and time given in a [Timestamp](#timestamp). |
|  | [`DateTime`](#datetime)  | Creates a [DateTime](#datetime) for the given Gregorian date and time. |
|  | [`DateTime`](#datetime)  | Creates a [DateTime](#datetime) for the given Julian day. |
|  | [`DateTime`](#datetime)  | Creates a [DateTime](#datetime) from an UtcTimeVal and a TimeDiff. |
|  | [`DateTime`](#datetime)  | Copy constructor. Creates the [DateTime](#datetime) from another one. |
|  | [`~DateTime`](#datetime)  | Destroys the [DateTime](#datetime). |
| `DateTime &` | [`operator=`](#operator)  | Assigns another [DateTime](#datetime). |
| `DateTime &` | [`operator=`](#operator)  | Assigns a [Timestamp](#timestamp). |
| `DateTime &` | [`operator=`](#operator)  | Assigns a Julian day. |
| `DateTime &` | [`assign`](#assign)  | Assigns a Gregorian date and time. |
| `void` | [`swap`](#swap)  | Swaps the [DateTime](#datetime) with another one. |
| `int` | [`year`](#year) `const` `inline` | Returns the year. |
| `int` | [`month`](#month) `const` `inline` | Returns the month (1 to 12). |
| `int` | [`week`](#week) `const` | Returns the week number within the year. FirstDayOfWeek should be either SUNDAY (0) or MONDAY (1). The returned week number will be from 0 to 53. Week number 1 is the week containing January 4. This is in accordance to ISO 8601. |
| `int` | [`day`](#day) `const` `inline` | Returns the day witin the month (1 to 31). |
| `int` | [`dayOfWeek`](#dayofweek) `const` | Returns the weekday (0 to 6, where 0 = Sunday, 1 = Monday, ..., 6 = Saturday). |
| `int` | [`dayOfYear`](#dayofyear) `const` | Returns the number of the day in the year. January 1 is 1, February 1 is 32, etc. |
| `int` | [`hour`](#hour) `const` `inline` | Returns the hour (0 to 23). |
| `int` | [`hourAMPM`](#hourampm) `const` `inline` | Returns the hour (0 to 12). |
| `bool` | [`isAM`](#isam) `const` `inline` | Returns true if hour < 12;. |
| `bool` | [`isPM`](#ispm) `const` `inline` | Returns true if hour >= 12. |
| `int` | [`minute`](#minute) `const` `inline` | Returns the minute (0 to 59). |
| `int` | [`second`](#second) `const` `inline` | Returns the second (0 to 59). |
| `int` | [`millisecond`](#millisecond) `const` `inline` | Returns the millisecond (0 to 999). |
| `int` | [`microsecond`](#microsecond) `const` `inline` | Returns the microsecond (0 to 999). |
| `double` | [`julianDay`](#julianday) `const` | Returns the julian day for the date and time. |
| `Timestamp` | [`timestamp`](#timestamp) `const` `inline` | Returns the date and time expressed as a [Timestamp](#timestamp). |
| `Timestamp::UtcTimeVal` | [`utcTime`](#utctime) `const` `inline` | Returns the date and time expressed in UTC-based time. UTC base time is midnight, October 15, 1582. Resolution is 100 nanoseconds. |
| `bool` | [`operator==`](#operator) `const` `inline` |  |
| `bool` | [`operator!=`](#operator) `const` `inline` |  |
| `bool` | [`operator<`](#operator) `const` `inline` |  |
| `bool` | [`operator<=`](#operator) `const` `inline` |  |
| `bool` | [`operator>`](#operator) `const` `inline` |  |
| `bool` | [`operator>=`](#operator) `const` `inline` |  |
| `DateTime` | [`operator+`](#operator) `const` |  |
| `DateTime` | [`operator-`](#operator) `const` |  |
| `Timespan` | [`operator-`](#operator) `const` |  |
| `DateTime &` | [`operator+=`](#operator)  |  |
| `DateTime &` | [`operator-=`](#operator)  |  |
| `void` | [`makeUTC`](#makeutc)  | Converts a local time into UTC, by applying the given time zone differential. |
| `void` | [`makeLocal`](#makelocal)  | Converts a UTC time into a local time, by applying the given time zone differential. |

---

{#datetime}

#### DateTime

```cpp
DateTime()
```

Defined in src/base/include/icy/datetime.h:298

Creates a [DateTime](#datetime) for the current date and time.

---

{#datetime}

#### DateTime

```cpp
DateTime(const Timestamp & timestamp)
```

Defined in src/base/include/icy/datetime.h:302

Creates a [DateTime](#datetime) for the date and time given in a [Timestamp](#timestamp).

---

{#datetime}

#### DateTime

```cpp
DateTime(int year, int month, int day, int hour = 0, int minute = 0, int second = 0, int millisecond = 0, int microsecond = 0)
```

Defined in src/base/include/icy/datetime.h:313

Creates a [DateTime](#datetime) for the given Gregorian date and time.

* year is from 0 to 9999.

* month is from 1 to 12.

* day is from 1 to 31.

* hour is from 0 to 23.

* minute is from 0 to 59.

* second is from 0 to 59.

* millisecond is from 0 to 999.

* microsecond is from 0 to 999.

---

{#datetime}

#### DateTime

```cpp
DateTime(double julianDay)
```

Defined in src/base/include/icy/datetime.h:317

Creates a [DateTime](#datetime) for the given Julian day.

---

{#datetime}

#### DateTime

```cpp
DateTime(Timestamp::UtcTimeVal utcTime, Timestamp::TimeDiff diff)
```

Defined in src/base/include/icy/datetime.h:322

Creates a [DateTime](#datetime) from an UtcTimeVal and a TimeDiff.

Mainly used internally by [DateTime](#datetime) and friends.

---

{#datetime}

#### DateTime

```cpp
DateTime(const DateTime & dateTime)
```

Defined in src/base/include/icy/datetime.h:325

Copy constructor. Creates the [DateTime](#datetime) from another one.

---

{#datetime}

#### ~DateTime

```cpp
~DateTime()
```

Defined in src/base/include/icy/datetime.h:328

Destroys the [DateTime](#datetime).

---

{#operator}

#### operator=

```cpp
DateTime & operator=(const DateTime & dateTime)
```

Defined in src/base/include/icy/datetime.h:331

Assigns another [DateTime](#datetime).

---

{#operator}

#### operator=

```cpp
DateTime & operator=(const Timestamp & timestamp)
```

Defined in src/base/include/icy/datetime.h:334

Assigns a [Timestamp](#timestamp).

---

{#operator}

#### operator=

```cpp
DateTime & operator=(double julianDay)
```

Defined in src/base/include/icy/datetime.h:337

Assigns a Julian day.

---

{#assign}

#### assign

```cpp
DateTime & assign(int year, int month, int day, int hour = 0, int minute = 0, int second = 0, int millisecond = 0, int microseconds = 0)
```

Defined in src/base/include/icy/datetime.h:348

Assigns a Gregorian date and time.

* year is from 0 to 9999.

* month is from 1 to 12.

* day is from 1 to 31.

* hour is from 0 to 23.

* minute is from 0 to 59.

* second is from 0 to 59.

* millisecond is from 0 to 999.

* microsecond is from 0 to 999.

---

{#swap}

#### swap

```cpp
void swap(DateTime & dateTime)
```

Defined in src/base/include/icy/datetime.h:352

Swaps the [DateTime](#datetime) with another one.

---

{#year}

#### year

`const` `inline`

```cpp
inline int year() const
```

Defined in src/base/include/icy/datetime.h:355

Returns the year.

---

{#month}

#### month

`const` `inline`

```cpp
inline int month() const
```

Defined in src/base/include/icy/datetime.h:358

Returns the month (1 to 12).

---

{#week}

#### week

`const`

```cpp
int week(int firstDayOfWeek = MONDAY) const
```

Defined in src/base/include/icy/datetime.h:374

Returns the week number within the year. FirstDayOfWeek should be either SUNDAY (0) or MONDAY (1). The returned week number will be from 0 to 53. Week number 1 is the week containing January 4. This is in accordance to ISO 8601.

The following example assumes that firstDayOfWeek is MONDAY. For 2005, which started on a Saturday, week 1 will be the week starting on Monday, January 3. January 1 and 2 will fall within week 0 (or the last week of the previous year).

For 2007, which starts on a Monday, week 1 will be the week startung on Monday, January 1. There will be no week 0 in 2007.

---

{#day}

#### day

`const` `inline`

```cpp
inline int day() const
```

Defined in src/base/include/icy/datetime.h:377

Returns the day witin the month (1 to 31).

---

{#dayofweek}

#### dayOfWeek

`const`

```cpp
int dayOfWeek() const
```

Defined in src/base/include/icy/datetime.h:381

Returns the weekday (0 to 6, where 0 = Sunday, 1 = Monday, ..., 6 = Saturday).

---

{#dayofyear}

#### dayOfYear

`const`

```cpp
int dayOfYear() const
```

Defined in src/base/include/icy/datetime.h:385

Returns the number of the day in the year. January 1 is 1, February 1 is 32, etc.

---

{#hour}

#### hour

`const` `inline`

```cpp
inline int hour() const
```

Defined in src/base/include/icy/datetime.h:388

Returns the hour (0 to 23).

---

{#hourampm}

#### hourAMPM

`const` `inline`

```cpp
inline int hourAMPM() const
```

Defined in src/base/include/icy/datetime.h:391

Returns the hour (0 to 12).

---

{#isam}

#### isAM

`const` `inline`

```cpp
inline bool isAM() const
```

Defined in src/base/include/icy/datetime.h:394

Returns true if hour < 12;.

---

{#ispm}

#### isPM

`const` `inline`

```cpp
inline bool isPM() const
```

Defined in src/base/include/icy/datetime.h:397

Returns true if hour >= 12.

---

{#minute}

#### minute

`const` `inline`

```cpp
inline int minute() const
```

Defined in src/base/include/icy/datetime.h:400

Returns the minute (0 to 59).

---

{#second}

#### second

`const` `inline`

```cpp
inline int second() const
```

Defined in src/base/include/icy/datetime.h:403

Returns the second (0 to 59).

---

{#millisecond}

#### millisecond

`const` `inline`

```cpp
inline int millisecond() const
```

Defined in src/base/include/icy/datetime.h:406

Returns the millisecond (0 to 999).

---

{#microsecond}

#### microsecond

`const` `inline`

```cpp
inline int microsecond() const
```

Defined in src/base/include/icy/datetime.h:409

Returns the microsecond (0 to 999).

---

{#julianday}

#### julianDay

`const`

```cpp
double julianDay() const
```

Defined in src/base/include/icy/datetime.h:412

Returns the julian day for the date and time.

---

{#timestamp}

#### timestamp

`const` `inline`

```cpp
inline Timestamp timestamp() const
```

Defined in src/base/include/icy/datetime.h:415

Returns the date and time expressed as a [Timestamp](#timestamp).

---

{#utctime}

#### utcTime

`const` `inline`

```cpp
inline Timestamp::UtcTimeVal utcTime() const
```

Defined in src/base/include/icy/datetime.h:420

Returns the date and time expressed in UTC-based time. UTC base time is midnight, October 15, 1582. Resolution is 100 nanoseconds.

---

{#operator}

#### operator==

`const` `inline`

```cpp
inline bool operator==(const DateTime & dateTime) const
```

Defined in src/base/include/icy/datetime.h:422

---

{#operator}

#### operator!=

`const` `inline`

```cpp
inline bool operator!=(const DateTime & dateTime) const
```

Defined in src/base/include/icy/datetime.h:423

---

{#operator}

#### operator<

`const` `inline`

```cpp
inline bool operator<(const DateTime & dateTime) const
```

Defined in src/base/include/icy/datetime.h:424

---

{#operator}

#### operator<=

`const` `inline`

```cpp
inline bool operator<=(const DateTime & dateTime) const
```

Defined in src/base/include/icy/datetime.h:425

---

{#operator}

#### operator>

`const` `inline`

```cpp
inline bool operator>(const DateTime & dateTime) const
```

Defined in src/base/include/icy/datetime.h:426

---

{#operator}

#### operator>=

`const` `inline`

```cpp
inline bool operator>=(const DateTime & dateTime) const
```

Defined in src/base/include/icy/datetime.h:427

---

{#operator}

#### operator+

`const`

```cpp
DateTime operator+(const Timespan & span) const
```

Defined in src/base/include/icy/datetime.h:429

---

{#operator}

#### operator-

`const`

```cpp
DateTime operator-(const Timespan & span) const
```

Defined in src/base/include/icy/datetime.h:430

---

{#operator}

#### operator-

`const`

```cpp
Timespan operator-(const DateTime & dateTime) const
```

Defined in src/base/include/icy/datetime.h:431

---

{#operator}

#### operator+=

```cpp
DateTime & operator+=(const Timespan & span)
```

Defined in src/base/include/icy/datetime.h:432

---

{#operator}

#### operator-=

```cpp
DateTime & operator-=(const Timespan & span)
```

Defined in src/base/include/icy/datetime.h:433

---

{#makeutc}

#### makeUTC

```cpp
void makeUTC(int tzd)
```

Defined in src/base/include/icy/datetime.h:437

Converts a local time into UTC, by applying the given time zone differential.

---

{#makelocal}

#### makeLocal

```cpp
void makeLocal(int tzd)
```

Defined in src/base/include/icy/datetime.h:441

Converts a UTC time into a local time, by applying the given time zone differential.

### Public Static Methods

| Return | Name | Description |
|--------|------|-------------|
| `bool` | [`isLeapYear`](#isleapyear) `static` `inline` | Returns true if the given year is a leap year; false otherwise. |
| `int` | [`daysOfMonth`](#daysofmonth) `static` | Returns the number of days in the given month and year. Month is from 1 to 12. |
| `bool` | [`isValid`](#isvalid) `static` | Checks if the given date and time is valid (all arguments are within a proper range). |

---

{#isleapyear}

#### isLeapYear

`static` `inline`

```cpp
static inline bool isLeapYear(int year)
```

Defined in src/base/include/icy/datetime.h:445

Returns true if the given year is a leap year; false otherwise.

---

{#daysofmonth}

#### daysOfMonth

`static`

```cpp
static int daysOfMonth(int year, int month)
```

Defined in src/base/include/icy/datetime.h:449

Returns the number of days in the given month and year. Month is from 1 to 12.

---

{#isvalid}

#### isValid

`static`

```cpp
static bool isValid(int year, int month, int day, int hour = 0, int minute = 0, int second = 0, int millisecond = 0, int microsecond = 0)
```

Defined in src/base/include/icy/datetime.h:455

Checks if the given date and time is valid (all arguments are within a proper range).

Returns true if all arguments are valid, false otherwise.

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`computeGregorian`](#computegregorian)  | Computes the Gregorian date for the given Julian day. See [http://vsg.cape.com/~pbaum/date/injdimp.htm](http://vsg.cape.com/~pbaum/date/injdimp.htm), section 3.3.1 for the algorithm. |
| `void` | [`computeDaytime`](#computedaytime)  | Extracts the daytime (hours, minutes, seconds, etc.) from the stored utcTime. |

---

{#computegregorian}

#### computeGregorian

```cpp
void computeGregorian(double julianDay)
```

Defined in src/base/include/icy/datetime.h:477

Computes the Gregorian date for the given Julian day. See [http://vsg.cape.com/~pbaum/date/injdimp.htm](http://vsg.cape.com/~pbaum/date/injdimp.htm), section 3.3.1 for the algorithm.

---

{#computedaytime}

#### computeDaytime

```cpp
void computeDaytime()
```

Defined in src/base/include/icy/datetime.h:481

Extracts the daytime (hours, minutes, seconds, etc.) from the stored utcTime.

### Public Types

| Name | Description |
|------|-------------|
| [`Months`](#months)  | Symbolic names for month numbers (1 to 12). |
| [`DaysOfWeek`](#daysofweek)  | Symbolic names for week day numbers (0 to 6). |

---

{#months}

#### Months

```cpp
enum Months
```

Defined in src/base/include/icy/datetime.h:269

Symbolic names for month numbers (1 to 12).

| Value | Description |
|-------|-------------|
| `JANUARY` |  |
| `FEBRUARY` |  |
| `MARCH` |  |
| `APRIL` |  |
| `MAY` |  |
| `JUNE` |  |
| `JULY` |  |
| `AUGUST` |  |
| `SEPTEMBER` |  |
| `OCTOBER` |  |
| `NOVEMBER` |  |
| `DECEMBER` |  |

---

{#daysofweek}

#### DaysOfWeek

```cpp
enum DaysOfWeek
```

Defined in src/base/include/icy/datetime.h:286

Symbolic names for week day numbers (0 to 6).

| Value | Description |
|-------|-------------|
| `SUNDAY` |  |
| `MONDAY` |  |
| `TUESDAY` |  |
| `WEDNESDAY` |  |
| `THURSDAY` |  |
| `FRIDAY` |  |
| `SATURDAY` |  |

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `Timestamp::UtcTimeVal` | [`_utcTime`](#_utctime)  |  |
| `short` | [`_year`](#_year)  |  |
| `short` | [`_month`](#_month)  |  |
| `short` | [`_day`](#_day)  |  |
| `short` | [`_hour`](#_hour)  |  |
| `short` | [`_minute`](#_minute)  |  |
| `short` | [`_second`](#_second)  |  |
| `short` | [`_millisecond`](#_millisecond)  |  |
| `short` | [`_microsecond`](#_microsecond)  |  |

---

{#_utctime}

#### _utcTime

```cpp
Timestamp::UtcTimeVal _utcTime
```

Defined in src/base/include/icy/datetime.h:488

---

{#_year}

#### _year

```cpp
short _year
```

Defined in src/base/include/icy/datetime.h:489

---

{#_month}

#### _month

```cpp
short _month
```

Defined in src/base/include/icy/datetime.h:490

---

{#_day}

#### _day

```cpp
short _day
```

Defined in src/base/include/icy/datetime.h:491

---

{#_hour}

#### _hour

```cpp
short _hour
```

Defined in src/base/include/icy/datetime.h:492

---

{#_minute}

#### _minute

```cpp
short _minute
```

Defined in src/base/include/icy/datetime.h:493

---

{#_second}

#### _second

```cpp
short _second
```

Defined in src/base/include/icy/datetime.h:494

---

{#_millisecond}

#### _millisecond

```cpp
short _millisecond
```

Defined in src/base/include/icy/datetime.h:495

---

{#_microsecond}

#### _microsecond

```cpp
short _microsecond
```

Defined in src/base/include/icy/datetime.h:496

### Private Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`checkLimit`](#checklimit)  | Utility functions used to correct the overflow in computeGregorian. |
| `void` | [`normalize`](#normalize)  |  |

---

{#checklimit}

#### checkLimit

```cpp
void checkLimit(short & lower, short & higher, short limit)
```

Defined in src/base/include/icy/datetime.h:485

Utility functions used to correct the overflow in computeGregorian.

---

{#normalize}

#### normalize

```cpp
void normalize()
```

Defined in src/base/include/icy/datetime.h:486

{#timezone}

## Timezone

```cpp
#include <icy/datetime.h>
```

```cpp
class Timezone
```

Defined in src/base/include/icy/datetime.h:641

This class provides information about the current timezone.

### Public Static Methods

| Return | Name | Description |
|--------|------|-------------|
| `int` | [`utcOffset`](#utcoffset) `static` | Returns the offset of local time to UTC, in seconds. local time = UTC + [utcOffset()](#classicy_1_1Timezone_1a170e81d324a7366aecc33909441f8e86) + [dst()](#classicy_1_1Timezone_1a63486f75f5a20b7c98bbb6509049a8ba). |
| `int` | [`dst`](#dst) `static` | Returns the daylight saving time offset in seconds if daylight saving time is in use. local time = UTC + [utcOffset()](#classicy_1_1Timezone_1a170e81d324a7366aecc33909441f8e86) + [dst()](#classicy_1_1Timezone_1a63486f75f5a20b7c98bbb6509049a8ba). |
| `bool` | [`isDst`](#isdst) `static` | Returns true if daylight saving time is in effect for the given time. Depending on the operating system platform this might only work reliably for certain date ranges, as the C library's localtime() function is used. |
| `int` | [`tzd`](#tzd) `static` | Returns the time zone differential for the current timezone. The timezone differential is computed as [utcOffset()](#classicy_1_1Timezone_1a170e81d324a7366aecc33909441f8e86) + [dst()](#classicy_1_1Timezone_1a63486f75f5a20b7c98bbb6509049a8ba) /// and is expressed in seconds. |
| `std::string` | [`name`](#name) `static` | Returns the timezone name currently in effect. |
| `std::string` | [`standardName`](#standardname) `static` | Returns the timezone name if not daylight saving time is in effect. |
| `std::string` | [`dstName`](#dstname) `static` | Returns the timezone name if daylight saving time is in effect. |

---

{#utcoffset}

#### utcOffset

`static`

```cpp
static int utcOffset()
```

Defined in src/base/include/icy/datetime.h:646

Returns the offset of local time to UTC, in seconds. local time = UTC + [utcOffset()](#classicy_1_1Timezone_1a170e81d324a7366aecc33909441f8e86) + [dst()](#classicy_1_1Timezone_1a63486f75f5a20b7c98bbb6509049a8ba).

---

{#dst}

#### dst

`static`

```cpp
static int dst()
```

Defined in src/base/include/icy/datetime.h:651

Returns the daylight saving time offset in seconds if daylight saving time is in use. local time = UTC + [utcOffset()](#classicy_1_1Timezone_1a170e81d324a7366aecc33909441f8e86) + [dst()](#classicy_1_1Timezone_1a63486f75f5a20b7c98bbb6509049a8ba).

---

{#isdst}

#### isDst

`static`

```cpp
static bool isDst(const Timestamp & timestamp)
```

Defined in src/base/include/icy/datetime.h:658

Returns true if daylight saving time is in effect for the given time. Depending on the operating system platform this might only work reliably for certain date ranges, as the C library's localtime() function is used.

---

{#tzd}

#### tzd

`static`

```cpp
static int tzd()
```

Defined in src/base/include/icy/datetime.h:663

Returns the time zone differential for the current timezone. The timezone differential is computed as [utcOffset()](#classicy_1_1Timezone_1a170e81d324a7366aecc33909441f8e86) + [dst()](#classicy_1_1Timezone_1a63486f75f5a20b7c98bbb6509049a8ba) /// and is expressed in seconds.

---

{#name}

#### name

`static`

```cpp
static std::string name()
```

Defined in src/base/include/icy/datetime.h:666

Returns the timezone name currently in effect.

---

{#standardname}

#### standardName

`static`

```cpp
static std::string standardName()
```

Defined in src/base/include/icy/datetime.h:669

Returns the timezone name if not daylight saving time is in effect.

---

{#dstname}

#### dstName

`static`

```cpp
static std::string dstName()
```

Defined in src/base/include/icy/datetime.h:672

Returns the timezone name if daylight saving time is in effect.

{#localdatetime}

## LocalDateTime

```cpp
#include <icy/datetime.h>
```

```cpp
class LocalDateTime
```

Defined in src/base/include/icy/datetime.h:703

This class represents an instant in local time (as opposed to UTC), expressed in years, months, days, hours, minutes, seconds and milliseconds based on the Gregorian calendar.

In addition to the date and time, the class also maintains a time zone differential, which denotes the difference in seconds from UTC to local time, i.e. UTC = local time - time zone differential.

Although [LocalDateTime](#localdatetime) supports relational and arithmetic operators, all date/time comparisons and date/time arithmetics should be done in UTC, using the [DateTime](#datetime) or [Timestamp](#timestamp) class for better performance. The relational operators normalize the dates/times involved to UTC before carrying out the comparison.

The time zone differential is based on the input date and time and current time zone. A number of constructors accept an explicit time zone differential parameter. These should not be used since daylight savings time processing is impossible since the time zone is unknown. Each of the constructors accepting a tzd parameter have been marked as deprecated and may be removed in a future revision.

### Friends

| Name | Description |
|------|-------------|
| [`DateTimeFormatter`](#datetimeformatter)  |  |
| [`DateTimeParser`](#datetimeparser)  |  |

---

{#datetimeformatter}

#### DateTimeFormatter

```cpp
friend class DateTimeFormatter
```

Defined in src/base/include/icy/datetime.h:919

---

{#datetimeparser}

#### DateTimeParser

```cpp
friend class DateTimeParser
```

Defined in src/base/include/icy/datetime.h:920

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`LocalDateTime`](#localdatetime)  | Creates a [LocalDateTime](#localdatetime) with the current date/time for the current time zone. |
|  | [`LocalDateTime`](#localdatetime)  | Creates a [LocalDateTime](#localdatetime) for the given Gregorian local date and time. |
|  | [`LocalDateTime`](#localdatetime)  | @ deprecated Creates a [LocalDateTime](#localdatetime) for the given Gregorian date and time in the time zone denoted by the time zone differential in tzd. |
|  | [`LocalDateTime`](#localdatetime)  | Creates a [LocalDateTime](#localdatetime) from the UTC time given in dateTime, using the time zone differential of the current time zone. |
|  | [`LocalDateTime`](#localdatetime)  | @ deprecated Creates a [LocalDateTime](#localdatetime) from the UTC time given in dateTime, using the given time zone differential. Adjusts dateTime for the given time zone differential. |
|  | [`LocalDateTime`](#localdatetime)  | @ deprecated Creates a [LocalDateTime](#localdatetime) from the UTC time given in dateTime, using the given time zone differential. If adjust is true, adjusts dateTime for the given time zone differential. |
|  | [`LocalDateTime`](#localdatetime)  | Creates a [LocalDateTime](#localdatetime) for the given Julian day in the local time zone. |
|  | [`LocalDateTime`](#localdatetime)  | @ deprecated Creates a [LocalDateTime](#localdatetime) for the given Julian day in the time zone denoted by the time zone differential in tzd. |
|  | [`LocalDateTime`](#localdatetime)  | Copy constructor. Creates the [LocalDateTime](#localdatetime) from another one. |
|  | [`~LocalDateTime`](#localdatetime)  | Destroys the [LocalDateTime](#localdatetime). |
| `LocalDateTime &` | [`operator=`](#operator)  | Assigns another [LocalDateTime](#localdatetime). |
| `LocalDateTime &` | [`operator=`](#operator)  | Assigns a timestamp. |
| `LocalDateTime &` | [`operator=`](#operator)  | Assigns a Julian day in the local time zone. |
| `LocalDateTime &` | [`assign`](#assign)  | Assigns a Gregorian local date and time. |
| `LocalDateTime &` | [`assign`](#assign)  | @ deprecated Assigns a Gregorian local date and time in the time zone denoted by the time zone differential in tzd. |
| `LocalDateTime &` | [`assign`](#assign)  | @ deprecated Assigns a Julian day in the time zone denoted by the time zone differential in tzd. |
| `void` | [`swap`](#swap)  | Swaps the [LocalDateTime](#localdatetime) with another one. |
| `int` | [`year`](#year) `const` `inline` | Returns the year. |
| `int` | [`month`](#month) `const` `inline` | Returns the month (1 to 12). |
| `int` | [`week`](#week) `const` `inline` | Returns the week number within the year. FirstDayOfWeek should be either SUNDAY (0) or MONDAY (1). The returned week number will be from 0 to 53. Week number 1 is the week containing January 4. This is in accordance to ISO 8601. |
| `int` | [`day`](#day) `const` `inline` | Returns the day witin the month (1 to 31). |
| `int` | [`dayOfWeek`](#dayofweek) `const` `inline` | Returns the weekday (0 to 6, where 0 = Sunday, 1 = Monday, ..., 6 = Saturday). |
| `int` | [`dayOfYear`](#dayofyear) `const` `inline` | Returns the number of the day in the year. January 1 is 1, February 1 is 32, etc. |
| `int` | [`hour`](#hour) `const` `inline` | Returns the hour (0 to 23). |
| `int` | [`hourAMPM`](#hourampm) `const` `inline` | Returns the hour (0 to 12). |
| `bool` | [`isAM`](#isam) `const` `inline` | Returns true if hour < 12;. |
| `bool` | [`isPM`](#ispm) `const` `inline` | Returns true if hour >= 12. |
| `int` | [`minute`](#minute) `const` `inline` | Returns the minute (0 to 59). |
| `int` | [`second`](#second) `const` `inline` | Returns the second (0 to 59). |
| `int` | [`millisecond`](#millisecond) `const` `inline` | Returns the millisecond (0 to 999). |
| `int` | [`microsecond`](#microsecond) `const` `inline` | Returns the microsecond (0 to 999). |
| `double` | [`julianDay`](#julianday) `const` `inline` | Returns the julian day for the date. |
| `int` | [`tzd`](#tzd) `const` `inline` | Returns the time zone differential. |
| `DateTime` | [`utc`](#utc) `const` | Returns the UTC equivalent for the local date and time. |
| `Timestamp` | [`timestamp`](#timestamp) `const` `inline` | Returns the date and time expressed as a [Timestamp](#timestamp). |
| `Timestamp::UtcTimeVal` | [`utcTime`](#utctime) `const` `inline` | Returns the UTC equivalent for the local date and time. |
| `bool` | [`operator==`](#operator) `const` |  |
| `bool` | [`operator!=`](#operator) `const` |  |
| `bool` | [`operator<`](#operator) `const` |  |
| `bool` | [`operator<=`](#operator) `const` |  |
| `bool` | [`operator>`](#operator) `const` |  |
| `bool` | [`operator>=`](#operator) `const` |  |
| `LocalDateTime` | [`operator+`](#operator) `const` |  |
| `LocalDateTime` | [`operator-`](#operator) `const` |  |
| `Timespan` | [`operator-`](#operator) `const` |  |
| `LocalDateTime &` | [`operator+=`](#operator)  |  |
| `LocalDateTime &` | [`operator-=`](#operator)  |  |

---

{#localdatetime}

#### LocalDateTime

```cpp
LocalDateTime()
```

Defined in src/base/include/icy/datetime.h:708

Creates a [LocalDateTime](#localdatetime) with the current date/time for the current time zone.

---

{#localdatetime}

#### LocalDateTime

```cpp
LocalDateTime(int year, int month, int day, int hour = 0, int minute = 0, int second = 0, int millisecond = 0, int microsecond = 0)
```

Defined in src/base/include/icy/datetime.h:719

Creates a [LocalDateTime](#localdatetime) for the given Gregorian local date and time.

* year is from 0 to 9999.

* month is from 1 to 12.

* day is from 1 to 31.

* hour is from 0 to 23.

* minute is from 0 to 59.

* second is from 0 to 59.

* millisecond is from 0 to 999.

* microsecond is from 0 to 999.

---

{#localdatetime}

#### LocalDateTime

```cpp
LocalDateTime(int tzd, int year, int month, int day, int hour, int minute, int second, int millisecond, int microsecond)
```

Defined in src/base/include/icy/datetime.h:734

@ deprecated Creates a [LocalDateTime](#localdatetime) for the given Gregorian date and time in the time zone denoted by the time zone differential in tzd.

* tzd is in seconds.

* year is from 0 to 9999.

* month is from 1 to 12.

* day is from 1 to 31.

* hour is from 0 to 23.

* minute is from 0 to 59.

* second is from 0 to 59.

* millisecond is from 0 to 999.

* microsecond is from 0 to 999.

---

{#localdatetime}

#### LocalDateTime

```cpp
LocalDateTime(const DateTime & dateTime)
```

Defined in src/base/include/icy/datetime.h:739

Creates a [LocalDateTime](#localdatetime) from the UTC time given in dateTime, using the time zone differential of the current time zone.

---

{#localdatetime}

#### LocalDateTime

```cpp
LocalDateTime(int tzd, const DateTime & dateTime)
```

Defined in src/base/include/icy/datetime.h:745

@ deprecated Creates a [LocalDateTime](#localdatetime) from the UTC time given in dateTime, using the given time zone differential. Adjusts dateTime for the given time zone differential.

---

{#localdatetime}

#### LocalDateTime

```cpp
LocalDateTime(int tzd, const DateTime & dateTime, bool adjust)
```

Defined in src/base/include/icy/datetime.h:752

@ deprecated Creates a [LocalDateTime](#localdatetime) from the UTC time given in dateTime, using the given time zone differential. If adjust is true, adjusts dateTime for the given time zone differential.

---

{#localdatetime}

#### LocalDateTime

```cpp
LocalDateTime(double julianDay)
```

Defined in src/base/include/icy/datetime.h:755

Creates a [LocalDateTime](#localdatetime) for the given Julian day in the local time zone.

---

{#localdatetime}

#### LocalDateTime

```cpp
LocalDateTime(int tzd, double julianDay)
```

Defined in src/base/include/icy/datetime.h:761

@ deprecated Creates a [LocalDateTime](#localdatetime) for the given Julian day in the time zone denoted by the time zone differential in tzd.

---

{#localdatetime}

#### LocalDateTime

```cpp
LocalDateTime(const LocalDateTime & dateTime)
```

Defined in src/base/include/icy/datetime.h:764

Copy constructor. Creates the [LocalDateTime](#localdatetime) from another one.

---

{#localdatetime}

#### ~LocalDateTime

```cpp
~LocalDateTime()
```

Defined in src/base/include/icy/datetime.h:767

Destroys the [LocalDateTime](#localdatetime).

---

{#operator}

#### operator=

```cpp
LocalDateTime & operator=(const LocalDateTime & dateTime)
```

Defined in src/base/include/icy/datetime.h:770

Assigns another [LocalDateTime](#localdatetime).

---

{#operator}

#### operator=

```cpp
LocalDateTime & operator=(const Timestamp & timestamp)
```

Defined in src/base/include/icy/datetime.h:773

Assigns a timestamp.

---

{#operator}

#### operator=

```cpp
LocalDateTime & operator=(double julianDay)
```

Defined in src/base/include/icy/datetime.h:776

Assigns a Julian day in the local time zone.

---

{#assign}

#### assign

```cpp
LocalDateTime & assign(int year, int month, int day, int hour = 0, int minute = 0, int second = 0, int millisecond = 0, int microseconds = 0)
```

Defined in src/base/include/icy/datetime.h:787

Assigns a Gregorian local date and time.

* year is from 0 to 9999.

* month is from 1 to 12.

* day is from 1 to 31.

* hour is from 0 to 23.

* minute is from 0 to 59.

* second is from 0 to 59.

* millisecond is from 0 to 999.

* microsecond is from 0 to 999.

---

{#assign}

#### assign

```cpp
LocalDateTime & assign(int tzd, int year, int month, int day, int hour, int minute, int second, int millisecond, int microseconds)
```

Defined in src/base/include/icy/datetime.h:803

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

---

{#assign}

#### assign

```cpp
LocalDateTime & assign(int tzd, double julianDay)
```

Defined in src/base/include/icy/datetime.h:810

@ deprecated Assigns a Julian day in the time zone denoted by the time zone differential in tzd.

---

{#swap}

#### swap

```cpp
void swap(LocalDateTime & dateTime)
```

Defined in src/base/include/icy/datetime.h:813

Swaps the [LocalDateTime](#localdatetime) with another one.

---

{#year}

#### year

`const` `inline`

```cpp
inline int year() const
```

Defined in src/base/include/icy/datetime.h:816

Returns the year.

---

{#month}

#### month

`const` `inline`

```cpp
inline int month() const
```

Defined in src/base/include/icy/datetime.h:819

Returns the month (1 to 12).

---

{#week}

#### week

`const` `inline`

```cpp
inline int week(int firstDayOfWeek = DateTime::MONDAY) const
```

Defined in src/base/include/icy/datetime.h:835

Returns the week number within the year. FirstDayOfWeek should be either SUNDAY (0) or MONDAY (1). The returned week number will be from 0 to 53. Week number 1 is the week containing January 4. This is in accordance to ISO 8601.

The following example assumes that firstDayOfWeek is MONDAY. For 2005, which started on a Saturday, week 1 will be the week starting on Monday, January 3. January 1 and 2 will fall within week 0 (or the last week of the previous year).

For 2007, which starts on a Monday, week 1 will be the week startung on Monday, January 1. There will be no week 0 in 2007.

---

{#day}

#### day

`const` `inline`

```cpp
inline int day() const
```

Defined in src/base/include/icy/datetime.h:838

Returns the day witin the month (1 to 31).

---

{#dayofweek}

#### dayOfWeek

`const` `inline`

```cpp
inline int dayOfWeek() const
```

Defined in src/base/include/icy/datetime.h:842

Returns the weekday (0 to 6, where 0 = Sunday, 1 = Monday, ..., 6 = Saturday).

---

{#dayofyear}

#### dayOfYear

`const` `inline`

```cpp
inline int dayOfYear() const
```

Defined in src/base/include/icy/datetime.h:846

Returns the number of the day in the year. January 1 is 1, February 1 is 32, etc.

---

{#hour}

#### hour

`const` `inline`

```cpp
inline int hour() const
```

Defined in src/base/include/icy/datetime.h:849

Returns the hour (0 to 23).

---

{#hourampm}

#### hourAMPM

`const` `inline`

```cpp
inline int hourAMPM() const
```

Defined in src/base/include/icy/datetime.h:852

Returns the hour (0 to 12).

---

{#isam}

#### isAM

`const` `inline`

```cpp
inline bool isAM() const
```

Defined in src/base/include/icy/datetime.h:855

Returns true if hour < 12;.

---

{#ispm}

#### isPM

`const` `inline`

```cpp
inline bool isPM() const
```

Defined in src/base/include/icy/datetime.h:858

Returns true if hour >= 12.

---

{#minute}

#### minute

`const` `inline`

```cpp
inline int minute() const
```

Defined in src/base/include/icy/datetime.h:861

Returns the minute (0 to 59).

---

{#second}

#### second

`const` `inline`

```cpp
inline int second() const
```

Defined in src/base/include/icy/datetime.h:864

Returns the second (0 to 59).

---

{#millisecond}

#### millisecond

`const` `inline`

```cpp
inline int millisecond() const
```

Defined in src/base/include/icy/datetime.h:867

Returns the millisecond (0 to 999).

---

{#microsecond}

#### microsecond

`const` `inline`

```cpp
inline int microsecond() const
```

Defined in src/base/include/icy/datetime.h:870

Returns the microsecond (0 to 999).

---

{#julianday}

#### julianDay

`const` `inline`

```cpp
inline double julianDay() const
```

Defined in src/base/include/icy/datetime.h:873

Returns the julian day for the date.

---

{#tzd}

#### tzd

`const` `inline`

```cpp
inline int tzd() const
```

Defined in src/base/include/icy/datetime.h:876

Returns the time zone differential.

---

{#utc}

#### utc

`const`

```cpp
DateTime utc() const
```

Defined in src/base/include/icy/datetime.h:879

Returns the UTC equivalent for the local date and time.

---

{#timestamp}

#### timestamp

`const` `inline`

```cpp
inline Timestamp timestamp() const
```

Defined in src/base/include/icy/datetime.h:882

Returns the date and time expressed as a [Timestamp](#timestamp).

---

{#utctime}

#### utcTime

`const` `inline`

```cpp
inline Timestamp::UtcTimeVal utcTime() const
```

Defined in src/base/include/icy/datetime.h:885

Returns the UTC equivalent for the local date and time.

---

{#operator}

#### operator==

`const`

```cpp
bool operator==(const LocalDateTime & dateTime) const
```

Defined in src/base/include/icy/datetime.h:887

---

{#operator}

#### operator!=

`const`

```cpp
bool operator!=(const LocalDateTime & dateTime) const
```

Defined in src/base/include/icy/datetime.h:888

---

{#operator}

#### operator<

`const`

```cpp
bool operator<(const LocalDateTime & dateTime) const
```

Defined in src/base/include/icy/datetime.h:889

---

{#operator}

#### operator<=

`const`

```cpp
bool operator<=(const LocalDateTime & dateTime) const
```

Defined in src/base/include/icy/datetime.h:890

---

{#operator}

#### operator>

`const`

```cpp
bool operator>(const LocalDateTime & dateTime) const
```

Defined in src/base/include/icy/datetime.h:891

---

{#operator}

#### operator>=

`const`

```cpp
bool operator>=(const LocalDateTime & dateTime) const
```

Defined in src/base/include/icy/datetime.h:892

---

{#operator}

#### operator+

`const`

```cpp
LocalDateTime operator+(const Timespan & span) const
```

Defined in src/base/include/icy/datetime.h:894

---

{#operator}

#### operator-

`const`

```cpp
LocalDateTime operator-(const Timespan & span) const
```

Defined in src/base/include/icy/datetime.h:895

---

{#operator}

#### operator-

`const`

```cpp
Timespan operator-(const LocalDateTime & dateTime) const
```

Defined in src/base/include/icy/datetime.h:896

---

{#operator}

#### operator+=

```cpp
LocalDateTime & operator+=(const Timespan & span)
```

Defined in src/base/include/icy/datetime.h:897

---

{#operator}

#### operator-=

```cpp
LocalDateTime & operator-=(const Timespan & span)
```

Defined in src/base/include/icy/datetime.h:898

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`LocalDateTime`](#localdatetime)  |  |
| `void` | [`determineTzd`](#determinetzd)  | Recalculate the tzd based on the _dateTime member based on the current timezone using the Standard C runtime functions. If adjust is true, then [adjustForTzd()](#classicy_1_1LocalDateTime_1a12b329ccf06bdc8c089785f46992faf4) is called after the differential is calculated. |
| `void` | [`adjustForTzd`](#adjustfortzd) `inline` | Adjust the _dateTime member based on the _tzd member. |
| `std::time_t` | [`dstOffset`](#dstoffset) `const` | Determine the DST offset for the current date/time. |

---

{#localdatetime}

#### LocalDateTime

```cpp
LocalDateTime(Timestamp::UtcTimeVal utcTime, Timestamp::TimeDiff diff, int tzd)
```

Defined in src/base/include/icy/datetime.h:901

---

{#determinetzd}

#### determineTzd

```cpp
void determineTzd(bool adjust = false)
```

Defined in src/base/include/icy/datetime.h:907

Recalculate the tzd based on the _dateTime member based on the current timezone using the Standard C runtime functions. If adjust is true, then [adjustForTzd()](#classicy_1_1LocalDateTime_1a12b329ccf06bdc8c089785f46992faf4) is called after the differential is calculated.

---

{#adjustfortzd}

#### adjustForTzd

`inline`

```cpp
inline void adjustForTzd()
```

Defined in src/base/include/icy/datetime.h:910

Adjust the _dateTime member based on the _tzd member.

---

{#dstoffset}

#### dstOffset

`const`

```cpp
std::time_t dstOffset(int & dstOffset) const
```

Defined in src/base/include/icy/datetime.h:913

Determine the DST offset for the current date/time.

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `DateTime` | [`_dateTime`](#_datetime)  |  |
| `int` | [`_tzd`](#_tzd)  |  |

---

{#_datetime}

#### _dateTime

```cpp
DateTime _dateTime
```

Defined in src/base/include/icy/datetime.h:916

---

{#_tzd}

#### _tzd

```cpp
int _tzd
```

Defined in src/base/include/icy/datetime.h:917

{#datetimeformat}

## DateTimeFormat

```cpp
#include <icy/datetime.h>
```

```cpp
class DateTimeFormat
```

Defined in src/base/include/icy/datetime.h:1055

Definition of date/time formats and various constants used by [DateTimeFormatter](#datetimeformatter) and [DateTimeParser](#datetimeparser).

### Public Static Attributes

| Return | Name | Description |
|--------|------|-------------|
| `const std::string` | [`ISO8601_FORMAT`](#iso8601_format) `static` | predefined date formats The date/time format defined in the ISO 8601 standard. |
| `const std::string` | [`ISO8601_FRAC_FORMAT`](#iso8601_frac_format) `static` | The date/time format defined in the ISO 8601 standard, with fractional seconds. |
| `const std::string` | [`RFC822_FORMAT`](#rfc822_format) `static` | The date/time format defined in RFC 822 (obsoleted by RFC 1123). |
| `const std::string` | [`RFC1123_FORMAT`](#rfc1123_format) `static` | The date/time format defined in RFC 1123 (obsoletes RFC 822). |
| `const std::string` | [`HTTP_FORMAT`](#http_format) `static` | The date/time format defined in the HTTP specification (RFC 2616), which is basically a variant of RFC 1036 with a zero-padded day field. |
| `const std::string` | [`RFC850_FORMAT`](#rfc850_format) `static` | The date/time format defined in RFC 850 (obsoleted by RFC 1036). |
| `const std::string` | [`RFC1036_FORMAT`](#rfc1036_format) `static` | The date/time format defined in RFC 1036 (obsoletes RFC 850). |
| `const std::string` | [`ASCTIME_FORMAT`](#asctime_format) `static` | The date/time format produced by the ANSI C asctime() function. |
| `const std::string` | [`SORTABLE_FORMAT`](#sortable_format) `static` | A simple, sortable date/time format. |
| `const std::string` | [`WEEKDAY_NAMES`](#weekday_names) `static` | names used by formatter and parser English names of week days (Sunday, Monday, Tuesday, ...). |
| `const std::string` | [`MONTH_NAMES`](#month_names) `static` | English names of months (January, February, ...). |

---

{#iso8601_format}

#### ISO8601_FORMAT

`static`

```cpp
const std::string ISO8601_FORMAT
```

Defined in src/base/include/icy/datetime.h:1064

predefined date formats The date/time format defined in the ISO 8601 standard.

Examples: 2005-01-01T12:00:00+01:00 2005-01-01T11:00:00Z

---

{#iso8601_frac_format}

#### ISO8601_FRAC_FORMAT

`static`

```cpp
const std::string ISO8601_FRAC_FORMAT
```

Defined in src/base/include/icy/datetime.h:1072

The date/time format defined in the ISO 8601 standard, with fractional seconds.

Examples: 2005-01-01T12:00:00.000000+01:00 2005-01-01T11:00:00.000000Z

---

{#rfc822_format}

#### RFC822_FORMAT

`static`

```cpp
const std::string RFC822_FORMAT
```

Defined in src/base/include/icy/datetime.h:1079

The date/time format defined in RFC 822 (obsoleted by RFC 1123).

Examples: Sat, 1 Jan 05 12:00:00 +0100 Sat, 1 Jan 05 11:00:00 GMT

---

{#rfc1123_format}

#### RFC1123_FORMAT

`static`

```cpp
const std::string RFC1123_FORMAT
```

Defined in src/base/include/icy/datetime.h:1086

The date/time format defined in RFC 1123 (obsoletes RFC 822).

Examples: Sat, 1 Jan 2005 12:00:00 +0100 Sat, 1 Jan 2005 11:00:00 GMT

---

{#http_format}

#### HTTP_FORMAT

`static`

```cpp
const std::string HTTP_FORMAT
```

Defined in src/base/include/icy/datetime.h:1094

The date/time format defined in the HTTP specification (RFC 2616), which is basically a variant of RFC 1036 with a zero-padded day field.

Examples: Sat, 01 Jan 2005 12:00:00 +0100 Sat, 01 Jan 2005 11:00:00 GMT

---

{#rfc850_format}

#### RFC850_FORMAT

`static`

```cpp
const std::string RFC850_FORMAT
```

Defined in src/base/include/icy/datetime.h:1101

The date/time format defined in RFC 850 (obsoleted by RFC 1036).

Examples: Saturday, 1-Jan-05 12:00:00 +0100 Saturday, 1-Jan-05 11:00:00 GMT

---

{#rfc1036_format}

#### RFC1036_FORMAT

`static`

```cpp
const std::string RFC1036_FORMAT
```

Defined in src/base/include/icy/datetime.h:1108

The date/time format defined in RFC 1036 (obsoletes RFC 850).

Examples: Saturday, 1 Jan 05 12:00:00 +0100 Saturday, 1 Jan 05 11:00:00 GMT

---

{#asctime_format}

#### ASCTIME_FORMAT

`static`

```cpp
const std::string ASCTIME_FORMAT
```

Defined in src/base/include/icy/datetime.h:1114

The date/time format produced by the ANSI C asctime() function.

Example: Sat Jan 1 12:00:00 2005

---

{#sortable_format}

#### SORTABLE_FORMAT

`static`

```cpp
const std::string SORTABLE_FORMAT
```

Defined in src/base/include/icy/datetime.h:1120

A simple, sortable date/time format.

Example: 2005-01-01 12:00:00

---

{#weekday_names}

#### WEEKDAY_NAMES

`static`

```cpp
const std::string WEEKDAY_NAMES
```

Defined in src/base/include/icy/datetime.h:1124

names used by formatter and parser English names of week days (Sunday, Monday, Tuesday, ...).

---

{#month_names}

#### MONTH_NAMES

`static`

```cpp
const std::string MONTH_NAMES
```

Defined in src/base/include/icy/datetime.h:1127

English names of months (January, February, ...).

{#datetimeformatter}

## DateTimeFormatter

```cpp
#include <icy/datetime.h>
```

```cpp
class DateTimeFormatter
```

Defined in src/base/include/icy/datetime.h:1142

This class converts dates and times into strings, supporting a variety of standard and custom formats.

There are two kind of static member functions:

* format* functions return a std::string containin the formatted value.

* append* functions append the formatted value to an existing string.

### Public Static Methods

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`format`](#format) `static` `inline` | Formats the given timestamp according to the given format. The format string is used as a template to format the date and is copied character by character except for the following special characters, which are replaced by the corresponding value. |
| `std::string` | [`format`](#format) `static` `inline` | Formats the given date and time according to the given format. See [format(const Timestamp&, std::string_view, int)](#classicy_1_1DateTimeFormatter_1a5e24b7cfd3f21050283962aa47df6487) for more information. |
| `std::string` | [`format`](#format) `static` `inline` | Formats the given local date and time according to the given format. See [format(const Timestamp&, std::string_view, int)](#classicy_1_1DateTimeFormatter_1a5e24b7cfd3f21050283962aa47df6487) for more information. |
| `std::string` | [`format`](#format) `static` `inline` | Formats the given timespan according to the given format. The format string is used as a template to format the date and is copied character by character except for the following special characters, which are replaced by the corresponding value. |
| `void` | [`append`](#append) `static` `inline` | Formats the given timestamp according to the given format and appends it to str. |
| `void` | [`append`](#append) `static` | Formats the given date and time according to the given format and appends it to str. |
| `void` | [`append`](#append) `static` | Formats the given local date and time according to the given format and appends it to str. |
| `void` | [`append`](#append) `static` | Formats the given timespan according to the given format and appends it to str. |
| `std::string` | [`tzdISO`](#tzdiso) `static` `inline` | Formats the given timezone differential in ISO format. If timeZoneDifferential is UTC, "Z" is returned, otherwise, +HH.MM (or -HH.MM) is returned. |
| `std::string` | [`tzdRFC`](#tzdrfc) `static` `inline` | Formats the given timezone differential in RFC format. If timeZoneDifferential is UTC, "GMT" is returned, otherwise ++HHMM (or -HHMM) is returned. |
| `void` | [`tzdISO`](#tzdiso) `static` | Formats the given timezone differential in ISO format and appends it to the given string. If timeZoneDifferential is UTC, "Z" is returned, otherwise, +HH.MM (or -HH.MM) is returned. |
| `void` | [`tzdRFC`](#tzdrfc) `static` | Formats the given timezone differential in RFC format and appends it to the given string. If timeZoneDifferential is UTC, "GMT" is returned, otherwise ++HHMM (or -HHMM) is returned. |

---

{#format}

#### format

`static` `inline`

```cpp
static inline std::string format(const Timestamp & timestamp, std::string_view fmt, int timeZoneDifferential = UTC)
```

Defined in src/base/include/icy/datetime.h:1183

Formats the given timestamp according to the given format. The format string is used as a template to format the date and is copied character by character except for the following special characters, which are replaced by the corresponding value.

* w - abbreviated weekday (Mon, Tue, ...)

* W - full weekday (Monday, Tuesday, ...)

* b - abbreviated month (Jan, Feb, ...)

* B - full month (January, February, ...)

* d - zero-padded day of month (01 .. 31)

* e - day of month (1 .. 31)

* f - space-padded day of month ( 1 .. 31)

* m - zero-padded month (01 .. 12)

* n - month (1 .. 12)

* o - space-padded month ( 1 .. 12)

* y - year without century (70)

* Y - year with century (1970)

* H - hour (00 .. 23)

* h - hour (00 .. 12)

* a - am/pm

* A - AM/PM

* M - minute (00 .. 59)

* S - second (00 .. 59)

* s - seconds and microseconds (equivalent to S.F)

* i - millisecond (000 .. 999)

* c - centisecond (0 .. 9)

* F - fractional seconds/microseconds (000000 - 999999)

* z - time zone differential in ISO 8601 format (Z or +NN.NN)

* Z - time zone differential in RFC format (GMT or +NNNN)

* %% - percent sign

Class [DateTimeFormat](#datetimeformat) defines format strings for various standard date/time formats.

---

{#format}

#### format

`static` `inline`

```cpp
static inline std::string format(const DateTime & dateTime, std::string_view fmt, int timeZoneDifferential = UTC)
```

Defined in src/base/include/icy/datetime.h:1190

Formats the given date and time according to the given format. See [format(const Timestamp&, std::string_view, int)](#classicy_1_1DateTimeFormatter_1a5e24b7cfd3f21050283962aa47df6487) for more information.

---

{#format}

#### format

`static` `inline`

```cpp
static inline std::string format(const LocalDateTime & dateTime, std::string_view fmt)
```

Defined in src/base/include/icy/datetime.h:1196

Formats the given local date and time according to the given format. See [format(const Timestamp&, std::string_view, int)](#classicy_1_1DateTimeFormatter_1a5e24b7cfd3f21050283962aa47df6487) for more information.

---

{#format}

#### format

`static` `inline`

```cpp
static inline std::string format(const Timespan & timespan, std::string_view fmt = "%dd %H:%M:%S.%i")
```

Defined in src/base/include/icy/datetime.h:1215

Formats the given timespan according to the given format. The format string is used as a template to format the date and is copied character by character except for the following special characters, which are replaced by the corresponding value.

* d - days

* H - hours (00 .. 23)

* h - total hours (0 .. n)

* M - minutes (00 .. 59)

* m - total minutes (0 .. n)

* S - seconds (00 .. 59)

* s - total seconds (0 .. n)

* i - milliseconds (000 .. 999)

* c - centisecond (0 .. 9)

* F - fractional seconds/microseconds (000000 - 999999)

* %% - percent sign

---

{#append}

#### append

`static` `inline`

```cpp
static inline void append(std::string & str, const Timestamp & timestamp, std::string_view fmt, int timeZoneDifferential = UTC)
```

Defined in src/base/include/icy/datetime.h:1222

Formats the given timestamp according to the given format and appends it to str.

See [format()](#classicy_1_1DateTimeFormatter_1a5e24b7cfd3f21050283962aa47df6487) for documentation of the formatting string.

---

{#append}

#### append

`static`

```cpp
static void append(std::string & str, const DateTime & dateTime, std::string_view fmt, int timeZoneDifferential = UTC)
```

Defined in src/base/include/icy/datetime.h:1229

Formats the given date and time according to the given format and appends it to str.

See [format()](#classicy_1_1DateTimeFormatter_1a5e24b7cfd3f21050283962aa47df6487) for documentation of the formatting string.

---

{#append}

#### append

`static`

```cpp
static void append(std::string & str, const LocalDateTime & dateTime, std::string_view fmt)
```

Defined in src/base/include/icy/datetime.h:1236

Formats the given local date and time according to the given format and appends it to str.

See [format()](#classicy_1_1DateTimeFormatter_1a5e24b7cfd3f21050283962aa47df6487) for documentation of the formatting string.

---

{#append}

#### append

`static`

```cpp
static void append(std::string & str, const Timespan & timespan, std::string_view fmt = "%dd %H:%M:%S.%i")
```

Defined in src/base/include/icy/datetime.h:1243

Formats the given timespan according to the given format and appends it to str.

See [format()](#classicy_1_1DateTimeFormatter_1a5e24b7cfd3f21050283962aa47df6487) for documentation of the formatting string.

---

{#tzdiso}

#### tzdISO

`static` `inline`

```cpp
static inline std::string tzdISO(int timeZoneDifferential)
```

Defined in src/base/include/icy/datetime.h:1249

Formats the given timezone differential in ISO format. If timeZoneDifferential is UTC, "Z" is returned, otherwise, +HH.MM (or -HH.MM) is returned.

---

{#tzdrfc}

#### tzdRFC

`static` `inline`

```cpp
static inline std::string tzdRFC(int timeZoneDifferential)
```

Defined in src/base/include/icy/datetime.h:1254

Formats the given timezone differential in RFC format. If timeZoneDifferential is UTC, "GMT" is returned, otherwise ++HHMM (or -HHMM) is returned.

---

{#tzdiso}

#### tzdISO

`static`

```cpp
static void tzdISO(std::string & str, int timeZoneDifferential)
```

Defined in src/base/include/icy/datetime.h:1260

Formats the given timezone differential in ISO format and appends it to the given string. If timeZoneDifferential is UTC, "Z" is returned, otherwise, +HH.MM (or -HH.MM) is returned.

---

{#tzdrfc}

#### tzdRFC

`static`

```cpp
static void tzdRFC(std::string & str, int timeZoneDifferential)
```

Defined in src/base/include/icy/datetime.h:1266

Formats the given timezone differential in RFC format and appends it to the given string. If timeZoneDifferential is UTC, "GMT" is returned, otherwise ++HHMM (or -HHMM) is returned.

### Public Types

| Name | Description |
|------|-------------|
| [`@324166125375040071131114040250204055235371364256`](#324166125375040071131114040250204055235371364256)  |  |

---

{#324166125375040071131114040250204055235371364256}

#### @324166125375040071131114040250204055235371364256

```cpp
enum @324166125375040071131114040250204055235371364256
```

Defined in src/base/include/icy/datetime.h:1145

| Value | Description |
|-------|-------------|
| `UTC` | Special value for timeZoneDifferential denoting UTC. |

{#datetimeparser}

## DateTimeParser

```cpp
#include <icy/datetime.h>
```

```cpp
class DateTimeParser
```

Defined in src/base/include/icy/datetime.h:1294

This class provides a method for parsing dates and times from strings. All parsing methods do their best to parse a meaningful result, even from malformed input strings.

The returned [DateTime](#datetime) will always contain a time in the same timezone as the time in the string. Call [DateTime::makeUTC()](#classicy_1_1DateTime_1afb1c31f9f9242ed9987d4c2481579ade) with the timeZoneDifferential returned by [parse()](#classicy_1_1DateTimeParser_1afaf62f609110897d879ec98f45c9752f) to convert the [DateTime](#datetime) to UTC.

Note: When parsing a time in 12-hour (AM/PM) format, the hour (h) must be parsed before the AM/PM designator (a, A), otherwise the AM/PM designator will be ignored.

See the [DateTimeFormatter](#datetimeformatter) class for a list of supported format specifiers. In addition to the format specifiers supported by [DateTimeFormatter](#datetimeformatter), an additional specifier is supported: r will parse a year given by either two or four digits. Years 69-00 are interpreted in the 20th century (1969-2000), years 01-68 in the 21th century (2001-2068).

### Public Static Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`parse`](#parse) `static` | Parses a date and time in the given format from the given string. Throws a SyntaxException if the string cannot be successfully parsed. Please see [DateTimeFormatter::format()](#classicy_1_1DateTimeFormatter_1a5e24b7cfd3f21050283962aa47df6487) for a description of the format string. Class [DateTimeFormat](#datetimeformat) defines format strings for various standard date/time formats. |
| `DateTime` | [`parse`](#parse) `static` | Parses a date and time in the given format from the given string. Throws a SyntaxException if the string cannot be successfully parsed. Please see [DateTimeFormatter::format()](#classicy_1_1DateTimeFormatter_1a5e24b7cfd3f21050283962aa47df6487) for a description of the format string. Class [DateTimeFormat](#datetimeformat) defines format strings for various standard date/time formats. |
| `bool` | [`tryParse`](#tryparse) `static` | Parses a date and time in the given format from the given string. Returns true if the string has been successfully parsed, false otherwise. Please see [DateTimeFormatter::format()](#classicy_1_1DateTimeFormatter_1a5e24b7cfd3f21050283962aa47df6487) for a description of the format string. Class [DateTimeFormat](#datetimeformat) defines format strings for various standard date/time formats. |
| `void` | [`parse`](#parse) `static` | Parses a date and time from the given dateTime string. Before parsing, the method examines the dateTime string for a known date/time format. Throws a SyntaxException if the string cannot be successfully parsed. Please see [DateTimeFormatter::format()](#classicy_1_1DateTimeFormatter_1a5e24b7cfd3f21050283962aa47df6487) for a description of the format string. Class [DateTimeFormat](#datetimeformat) defines format strings for various standard date/time formats. |
| `DateTime` | [`parse`](#parse) `static` | Parses a date and time from the given dateTime string. Before parsing, the method examines the dateTime string for a known date/time format. Please see [DateTimeFormatter::format()](#classicy_1_1DateTimeFormatter_1a5e24b7cfd3f21050283962aa47df6487) for a description of the format string. Class [DateTimeFormat](#datetimeformat) defines format strings for various standard date/time formats. |
| `bool` | [`tryParse`](#tryparse) `static` | Parses a date and time from the given dateTime string. Before parsing, the method examines the dateTime string for a known date/time format. Please see [DateTimeFormatter::format()](#classicy_1_1DateTimeFormatter_1a5e24b7cfd3f21050283962aa47df6487) for a description of the format string. Class [DateTimeFormat](#datetimeformat) defines format strings for various standard date/time formats. |
| `int` | [`parseMonth`](#parsemonth) `static` | Tries to interpret the given range as a month name. The range must be at least three characters long. Returns the month number (1 .. 12) if the month name is valid. Otherwise throws a SyntaxException. |
| `int` | [`parseDayOfWeek`](#parsedayofweek) `static` | Tries to interpret the given range as a weekday name. The range must be at least three characters long. Returns the weekday number (0 .. 6, where 0 = Synday, 1 = Monday, etc.) if the weekday name is valid. Otherwise throws a SyntaxException. |

---

{#parse}

#### parse

`static`

```cpp
static void parse(std::string_view fmt, std::string_view str, DateTime & dateTime, int & timeZoneDifferential)
```

Defined in src/base/include/icy/datetime.h:1303

Parses a date and time in the given format from the given string. Throws a SyntaxException if the string cannot be successfully parsed. Please see [DateTimeFormatter::format()](#classicy_1_1DateTimeFormatter_1a5e24b7cfd3f21050283962aa47df6487) for a description of the format string. Class [DateTimeFormat](#datetimeformat) defines format strings for various standard date/time formats.

---

{#parse}

#### parse

`static`

```cpp
static DateTime parse(std::string_view fmt, std::string_view str, int & timeZoneDifferential)
```

Defined in src/base/include/icy/datetime.h:1312

Parses a date and time in the given format from the given string. Throws a SyntaxException if the string cannot be successfully parsed. Please see [DateTimeFormatter::format()](#classicy_1_1DateTimeFormatter_1a5e24b7cfd3f21050283962aa47df6487) for a description of the format string. Class [DateTimeFormat](#datetimeformat) defines format strings for various standard date/time formats.

---

{#tryparse}

#### tryParse

`static`

```cpp
static bool tryParse(std::string_view fmt, std::string_view str, DateTime & dateTime, int & timeZoneDifferential)
```

Defined in src/base/include/icy/datetime.h:1322

Parses a date and time in the given format from the given string. Returns true if the string has been successfully parsed, false otherwise. Please see [DateTimeFormatter::format()](#classicy_1_1DateTimeFormatter_1a5e24b7cfd3f21050283962aa47df6487) for a description of the format string. Class [DateTimeFormat](#datetimeformat) defines format strings for various standard date/time formats.

---

{#parse}

#### parse

`static`

```cpp
static void parse(std::string_view str, DateTime & dateTime, int & timeZoneDifferential)
```

Defined in src/base/include/icy/datetime.h:1333

Parses a date and time from the given dateTime string. Before parsing, the method examines the dateTime string for a known date/time format. Throws a SyntaxException if the string cannot be successfully parsed. Please see [DateTimeFormatter::format()](#classicy_1_1DateTimeFormatter_1a5e24b7cfd3f21050283962aa47df6487) for a description of the format string. Class [DateTimeFormat](#datetimeformat) defines format strings for various standard date/time formats.

---

{#parse}

#### parse

`static`

```cpp
static DateTime parse(std::string_view str, int & timeZoneDifferential)
```

Defined in src/base/include/icy/datetime.h:1343

Parses a date and time from the given dateTime string. Before parsing, the method examines the dateTime string for a known date/time format. Please see [DateTimeFormatter::format()](#classicy_1_1DateTimeFormatter_1a5e24b7cfd3f21050283962aa47df6487) for a description of the format string. Class [DateTimeFormat](#datetimeformat) defines format strings for various standard date/time formats.

---

{#tryparse}

#### tryParse

`static`

```cpp
static bool tryParse(std::string_view str, DateTime & dateTime, int & timeZoneDifferential)
```

Defined in src/base/include/icy/datetime.h:1352

Parses a date and time from the given dateTime string. Before parsing, the method examines the dateTime string for a known date/time format. Please see [DateTimeFormatter::format()](#classicy_1_1DateTimeFormatter_1a5e24b7cfd3f21050283962aa47df6487) for a description of the format string. Class [DateTimeFormat](#datetimeformat) defines format strings for various standard date/time formats.

---

{#parsemonth}

#### parseMonth

`static`

```cpp
static int parseMonth(const char *& it, const char * end)
```

Defined in src/base/include/icy/datetime.h:1361

Tries to interpret the given range as a month name. The range must be at least three characters long. Returns the month number (1 .. 12) if the month name is valid. Otherwise throws a SyntaxException.

---

{#parsedayofweek}

#### parseDayOfWeek

`static`

```cpp
static int parseDayOfWeek(const char *& it, const char * end)
```

Defined in src/base/include/icy/datetime.h:1369

Tries to interpret the given range as a weekday name. The range must be at least three characters long. Returns the weekday number (0 .. 6, where 0 = Synday, 1 = Monday, etc.) if the weekday name is valid. Otherwise throws a SyntaxException.

{#stopwatch}

## Stopwatch

```cpp
#include <icy/datetime.h>
```

```cpp
class Stopwatch
```

Defined in src/base/include/icy/datetime.h:1733

A simple facility to measure time intervals with microsecond resolution.

The [Stopwatch](#stopwatch) uses the current system time, so if the system time changes the measured time will be incorrect.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Stopwatch`](#stopwatch)  |  |
| `void` | [`start`](#start)  | Starts (or restarts) the stopwatch. |
| `void` | [`stop`](#stop)  | Stops or pauses the stopwatch. |
| `void` | [`reset`](#reset)  | Resets the stopwatch. |
| `void` | [`restart`](#restart)  | Resets and starts the stopwatch. |
| `Timestamp::TimeDiff` | [`elapsed`](#elapsed) `const` | Returns the elapsed time in microseconds since the stopwatch started. |
| `int` | [`elapsedSeconds`](#elapsedseconds) `const` | Returns the number of seconds elapsed since the stopwatch started. |
| `int` | [`elapsedMilliseconds`](#elapsedmilliseconds) `const` | Returns the number of milliseconds elapsed since the stopwatch started. |

---

{#stopwatch}

#### Stopwatch

```cpp
Stopwatch()
```

Defined in src/base/include/icy/datetime.h:1736

---

{#start}

#### start

```cpp
void start()
```

Defined in src/base/include/icy/datetime.h:1740

Starts (or restarts) the stopwatch.

---

{#stop}

#### stop

```cpp
void stop()
```

Defined in src/base/include/icy/datetime.h:1743

Stops or pauses the stopwatch.

---

{#reset}

#### reset

```cpp
void reset()
```

Defined in src/base/include/icy/datetime.h:1746

Resets the stopwatch.

---

{#restart}

#### restart

```cpp
void restart()
```

Defined in src/base/include/icy/datetime.h:1749

Resets and starts the stopwatch.

---

{#elapsed}

#### elapsed

`const`

```cpp
Timestamp::TimeDiff elapsed() const
```

Defined in src/base/include/icy/datetime.h:1753

Returns the elapsed time in microseconds since the stopwatch started.

---

{#elapsedseconds}

#### elapsedSeconds

`const`

```cpp
int elapsedSeconds() const
```

Defined in src/base/include/icy/datetime.h:1757

Returns the number of seconds elapsed since the stopwatch started.

---

{#elapsedmilliseconds}

#### elapsedMilliseconds

`const`

```cpp
int elapsedMilliseconds() const
```

Defined in src/base/include/icy/datetime.h:1761

Returns the number of milliseconds elapsed since the stopwatch started.

### Public Static Methods

| Return | Name | Description |
|--------|------|-------------|
| `Timestamp::TimeVal` | [`resolution`](#resolution) `static` | Returns the resolution of the stopwatch. |

---

{#resolution}

#### resolution

`static`

```cpp
static Timestamp::TimeVal resolution()
```

Defined in src/base/include/icy/datetime.h:1764

Returns the resolution of the stopwatch.

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `Timestamp` | [`_start`](#_start)  |  |
| `Timestamp::TimeDiff` | [`_elapsed`](#_elapsed)  |  |
| `bool` | [`_running`](#_running)  |  |

---

{#_start}

#### _start

```cpp
Timestamp _start
```

Defined in src/base/include/icy/datetime.h:1770

---

{#_elapsed}

#### _elapsed

```cpp
Timestamp::TimeDiff _elapsed
```

Defined in src/base/include/icy/datetime.h:1771

---

{#_running}

#### _running

```cpp
bool _running
```

Defined in src/base/include/icy/datetime.h:1772

### Private Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Stopwatch`](#stopwatch)  | Deleted constructor. |

---

{#stopwatch}

#### Stopwatch

```cpp
Stopwatch(const Stopwatch &) = delete
```

Defined in src/base/include/icy/datetime.h:1767

Deleted constructor.

{#threadedstreamreader}

## ThreadedStreamReader

```cpp
#include <icy/packetio.h>
```

```cpp
class ThreadedStreamReader
```

Defined in src/base/include/icy/packetio.h:31

> **Inherits:** [`PacketStreamAdapter`](#packetstreamadapter), [`Startable`](#startable)

Threaded stream reader class.

This class can be connected to a `[PacketStream](#packetstream)` to read input from any class that derives from `std::istream`. It's most regularly used for reading input files.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `PacketSignal` | [`emitter`](#emitter)  |  |

---

{#emitter}

#### emitter

```cpp
PacketSignal emitter
```

Defined in src/base/include/icy/packetio.h:93

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`ThreadedStreamReader`](#threadedstreamreader) `inline` |  |
|  | [`~ThreadedStreamReader`](#threadedstreamreader) `inline` | Stops the reader thread and deletes the owned stream. |
| `void` | [`start`](#start) `virtual` `inline` `override` | Starts the reader thread; emits one line per iteration as a [RawPacket](#rawpacket). Emits a [FlagPacket](#flagpacket) with `[PacketFlags::Final](#namespaceicy_1a3d1e0d9028d45b9ec824bf4306047f18abeae421a14a34f831c113f61323d1ab3)` on EOF. |
| `void` | [`stop`](#stop) `virtual` `inline` `override` | Cancels the reader thread. |
| `StreamT &` | [`stream`](#stream) `inline` | Returns the internal stream cast to `StreamT`. |
| `std::istream &` | [`stream`](#stream) `inline` |  |

---

{#threadedstreamreader}

#### ThreadedStreamReader

`inline`

```cpp
inline ThreadedStreamReader(std::istream * is)
```

Defined in src/base/include/icy/packetio.h:36

#### Parameters
* `is` Input stream to read from; takes ownership.

---

{#threadedstreamreader}

#### ~ThreadedStreamReader

`inline`

```cpp
inline ~ThreadedStreamReader()
```

Defined in src/base/include/icy/packetio.h:44

Stops the reader thread and deletes the owned stream.

---

{#start}

#### start

`virtual` `inline` `override`

```cpp
virtual inline void start() override
```

Defined in src/base/include/icy/packetio.h:55

Starts the reader thread; emits one line per iteration as a [RawPacket](#rawpacket). Emits a [FlagPacket](#flagpacket) with `[PacketFlags::Final](#namespaceicy_1a3d1e0d9028d45b9ec824bf4306047f18abeae421a14a34f831c113f61323d1ab3)` on EOF.

---

{#stop}

#### stop

`virtual` `inline` `override`

```cpp
virtual inline void stop() override
```

Defined in src/base/include/icy/packetio.h:71

Cancels the reader thread.

---

{#stream}

#### stream

`inline`

```cpp
template<class StreamT> inline StreamT & stream()
```

Defined in src/base/include/icy/packetio.h:81

Returns the internal stream cast to `StreamT`. 
#### Parameters
* `StreamT` Target stream type derived from `std::istream`. 

#### Returns
Reference to the cast stream. 

#### Exceptions
* `std::runtime_error` if the cast fails.

---

{#stream}

#### stream

`inline`

```cpp
inline std::istream & stream()
```

Defined in src/base/include/icy/packetio.h:91

#### Returns
Reference to the underlying input stream.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `Thread` | [`_runner`](#_runner)  |  |
| `std::istream *` | [`_istream`](#_istream)  |  |

---

{#_runner}

#### _runner

```cpp
Thread _runner
```

Defined in src/base/include/icy/packetio.h:96

---

{#_istream}

#### _istream

```cpp
std::istream * _istream
```

Defined in src/base/include/icy/packetio.h:97

{#streamwriter}

## StreamWriter

```cpp
#include <icy/packetio.h>
```

```cpp
class StreamWriter
```

Defined in src/base/include/icy/packetio.h:108

> **Inherits:** [`PacketProcessor`](#packetprocessor)

Packet stream writer class.

This class can be connected to a `[PacketStream](#packetstream)` to write output to any class that derives from `std::ostream`. It's most regularly used for writing output files.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `PacketSignal` | [`emitter`](#emitter)  |  |

---

{#emitter}

#### emitter

```cpp
PacketSignal emitter
```

Defined in src/base/include/icy/packetio.h:188

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`StreamWriter`](#streamwriter) `inline` |  |
|  | [`~StreamWriter`](#streamwriter) `virtual` `inline` | Closes any open `std::ofstream` and deletes the owned stream. |
| `void` | [`process`](#process) `virtual` `inline` `override` | Serializes the packet via `write()`, flushes it to the output stream, then forwards the packet to the next processor. |
| `StreamT &` | [`stream`](#stream) `inline` | Returns the internal output stream cast to `StreamT`. |
| `void` | [`onStreamStateChange`](#onstreamstatechange) `virtual` `inline` `override` | Closes the output file on `Closed` or `[Error](#error)` stream state transitions. |
| `std::ostream &` | [`stream`](#stream) `inline` |  |

---

{#streamwriter}

#### StreamWriter

`inline`

```cpp
inline StreamWriter(std::ostream * stream)
```

Defined in src/base/include/icy/packetio.h:112

#### Parameters
* `stream` Output stream to write to; takes ownership.

---

{#streamwriter}

#### ~StreamWriter

`virtual` `inline`

```cpp
virtual inline ~StreamWriter()
```

Defined in src/base/include/icy/packetio.h:119

Closes any open `std::ofstream` and deletes the owned stream.

---

{#process}

#### process

`virtual` `inline` `override`

```cpp
virtual inline void process(IPacket & packet) override
```

Defined in src/base/include/icy/packetio.h:132

Serializes the packet via `write()`, flushes it to the output stream, then forwards the packet to the next processor. 
#### Parameters
* `packet` Incoming packet to process.

---

{#stream}

#### stream

`inline`

```cpp
template<class StreamT> inline StreamT & stream()
```

Defined in src/base/include/icy/packetio.h:148

Returns the internal output stream cast to `StreamT`. 
#### Parameters
* `StreamT` Target stream type derived from `std::ostream`. 

#### Returns
Reference to the cast stream. 

#### Exceptions
* `std::runtime_error` if the cast fails.

---

{#onstreamstatechange}

#### onStreamStateChange

`virtual` `inline` `override`

```cpp
virtual inline void onStreamStateChange(const PacketStreamState & state) override
```

Defined in src/base/include/icy/packetio.h:159

Closes the output file on `Closed` or `[Error](#error)` stream state transitions. 
#### Parameters
* `state` New stream state.

---

{#stream}

#### stream

`inline`

```cpp
inline std::ostream & stream()
```

Defined in src/base/include/icy/packetio.h:181

#### Returns
Reference to the underlying output stream. 

#### Exceptions
* `std::runtime_error` if the stream pointer is null.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::ostream *` | [`_ostream`](#_ostream)  |  |

---

{#_ostream}

#### _ostream

```cpp
std::ostream * _ostream
```

Defined in src/base/include/icy/packetio.h:191

{#state}

## State

```cpp
#include <icy/stateful.h>
```

```cpp
class State
```

Defined in src/base/include/icy/stateful.h:31

> **Subclassed by:** [`DiagnosticState`](#diagnosticstate), [`PacketStreamState`](#packetstreamstate), [`TransactionState`](#transactionstate), [`EncoderState`](av.md#encoderstate), [`InstallationState`](pacm.md#installationstate), [`ClientState`](symple.md#clientstate-1), [`ClientState`](turn.md#clientstate)

[State](#state) class for state machines.

This class defines the state for a state machine, and should be extended and passed to implementations of the `[Stateful](#stateful)` template.

For an example **See also**: [PacketStreamState](#packetstreamstate)

### Friends

| Name | Description |
|------|-------------|
| [`operator<<`](#operator) `inline` |  |

---

{#operator}

#### operator<<

`inline`

```cpp
friend inline std::ostream & operator<<(std::ostream & os, const State & state)
```

Defined in src/base/include/icy/stateful.h:80

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`State`](#state)  |  |
|  | [`State`](#state)  | Copy constructor. |
| `State &` | [`operator=`](#operator)  | Copy assignment. |
| `ID` | [`id`](#id) `const` | Returns the current state [ID](#classicy_1_1State_1af5e8f0a00984df441608f8bedaaecea3). |
| `void` | [`set`](#set)  | Sets the state [ID](#classicy_1_1State_1af5e8f0a00984df441608f8bedaaecea3). |
| `std::string` | [`str`](#str) `virtual` `const` | Returns a human-readable string for the given state [ID](#classicy_1_1State_1af5e8f0a00984df441608f8bedaaecea3). Override in derived classes to provide meaningful names. |
| `std::string` | [`toString`](#tostring) `virtual` `const` | Returns a human-readable string for the current state [ID](#classicy_1_1State_1af5e8f0a00984df441608f8bedaaecea3). |
| `bool` | [`equals`](#equals) `const` | Returns true if the current state [ID](#classicy_1_1State_1af5e8f0a00984df441608f8bedaaecea3) equals the given [ID](#classicy_1_1State_1af5e8f0a00984df441608f8bedaaecea3). |
| `bool` | [`between`](#between) `const` | Returns true if the current state [ID](#classicy_1_1State_1af5e8f0a00984df441608f8bedaaecea3) is in the inclusive range [lid, rid]. |
| `bool` | [`operator==`](#operator) `const` `inline` |  |
| `bool` | [`operator==`](#operator) `const` `inline` |  |

---

{#state}

#### State

```cpp
State(ID id = 0)
```

Defined in src/base/include/icy/stateful.h:37

#### Parameters
* `id` Initial state [ID](#classicy_1_1State_1af5e8f0a00984df441608f8bedaaecea3). Defaults to 0.

---

{#state}

#### State

```cpp
State(const State & that)
```

Defined in src/base/include/icy/stateful.h:41

Copy constructor. 
#### Parameters
* `that` [State](#state) to copy from.

---

{#operator}

#### operator=

```cpp
State & operator=(const State & that)
```

Defined in src/base/include/icy/stateful.h:45

Copy assignment. 
#### Parameters
* `that` [State](#state) to assign from.

---

{#id}

#### id

`const`

```cpp
ID id() const
```

Defined in src/base/include/icy/stateful.h:50

Returns the current state [ID](#classicy_1_1State_1af5e8f0a00984df441608f8bedaaecea3). 
#### Returns
Atomic state [ID](#classicy_1_1State_1af5e8f0a00984df441608f8bedaaecea3) value.

---

{#set}

#### set

```cpp
void set(ID id)
```

Defined in src/base/include/icy/stateful.h:54

Sets the state [ID](#classicy_1_1State_1af5e8f0a00984df441608f8bedaaecea3). 
#### Parameters
* `id` New state [ID](#classicy_1_1State_1af5e8f0a00984df441608f8bedaaecea3) to assign.

---

{#str}

#### str

`virtual` `const`

```cpp
virtual std::string str(ID id) const
```

Defined in src/base/include/icy/stateful.h:60

Returns a human-readable string for the given state [ID](#classicy_1_1State_1af5e8f0a00984df441608f8bedaaecea3). Override in derived classes to provide meaningful names. 
#### Parameters
* `id` [State](#state)[ID](#classicy_1_1State_1af5e8f0a00984df441608f8bedaaecea3) to convert. 

#### Returns
String representation of the state, or "undefined" by default.

---

{#tostring}

#### toString

`virtual` `const`

```cpp
virtual std::string toString() const
```

Defined in src/base/include/icy/stateful.h:64

Returns a human-readable string for the current state [ID](#classicy_1_1State_1af5e8f0a00984df441608f8bedaaecea3). 
#### Returns
Result of `str(id())`.

---

{#equals}

#### equals

`const`

```cpp
bool equals(ID id) const
```

Defined in src/base/include/icy/stateful.h:69

Returns true if the current state [ID](#classicy_1_1State_1af5e8f0a00984df441608f8bedaaecea3) equals the given [ID](#classicy_1_1State_1af5e8f0a00984df441608f8bedaaecea3). 
#### Parameters
* `id` [State](#state)[ID](#classicy_1_1State_1af5e8f0a00984df441608f8bedaaecea3) to compare against. 

#### Returns
True if IDs match.

---

{#between}

#### between

`const`

```cpp
bool between(ID lid, ID rid) const
```

Defined in src/base/include/icy/stateful.h:75

Returns true if the current state [ID](#classicy_1_1State_1af5e8f0a00984df441608f8bedaaecea3) is in the inclusive range [lid, rid]. 
#### Parameters
* `lid` Lower bound state [ID](#classicy_1_1State_1af5e8f0a00984df441608f8bedaaecea3). 

* `rid` Upper bound state [ID](#classicy_1_1State_1af5e8f0a00984df441608f8bedaaecea3). 

#### Returns
True if lid <= [id()](#classicy_1_1State_1ad1f3cf6094d92da45a8863ccd05a4dd2) <= rid.

---

{#operator}

#### operator==

`const` `inline`

```cpp
inline bool operator==(const State & that) const
```

Defined in src/base/include/icy/stateful.h:77

---

{#operator}

#### operator==

`const` `inline`

```cpp
inline bool operator==(const State::ID & that) const
```

Defined in src/base/include/icy/stateful.h:78

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::atomic< ID >` | [`_id`](#_id)  |  |

---

{#_id}

#### _id

```cpp
std::atomic< ID > _id
```

Defined in src/base/include/icy/stateful.h:87

### Public Types

| Name | Description |
|------|-------------|
| [`ID`](#id)  |  |

---

{#id}

#### ID

```cpp
using ID = uint32_t
```

Defined in src/base/include/icy/stateful.h:34

{#stateful}

## Stateful

```cpp
#include <icy/stateful.h>
```

```cpp
template<typename T>
class Stateful
```

Defined in src/base/include/icy/stateful.h:97

[State](#state) machine implementation.

For an example **See also**: [PacketStream](#packetstream)

#### Parameters
* `The` derived `[State](#state)` type.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `Signal< void(void *, T &, const T &)>` | [`StateChange`](#statechange)  | Signals when the state changes. |

---

{#statechange}

#### StateChange

```cpp
Signal< void(void *, T &, const T &)> StateChange
```

Defined in src/base/include/icy/stateful.h:130

Signals when the state changes.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Stateful`](#stateful) `inline` |  |
| `bool` | [`stateEquals`](#stateequals) `virtual` `const` `inline` | Returns true if the current state ID equals the given ID. |
| `bool` | [`stateBetween`](#statebetween) `virtual` `const` `inline` | Returns true if the current state ID is in the inclusive range [lid, rid]. |
| `T &` | [`state`](#state) `virtual` `inline` | Returns a mutable reference to the current state. |
| `const T` | [`state`](#state) `virtual` `const` `inline` | Returns a copy of the current state. |

---

{#stateful}

#### Stateful

`inline`

```cpp
inline Stateful()
```

Defined in src/base/include/icy/stateful.h:100

---

{#stateequals}

#### stateEquals

`virtual` `const` `inline`

```cpp
virtual inline bool stateEquals(typename T::ID id) const
```

Defined in src/base/include/icy/stateful.h:107

Returns true if the current state ID equals the given ID. 
#### Parameters
* `id` [State](#state) ID to compare against. 

#### Returns
True if the current state matches.

---

{#statebetween}

#### stateBetween

`virtual` `const` `inline`

```cpp
virtual inline bool stateBetween(typename T::ID lid, typename T::ID rid) const
```

Defined in src/base/include/icy/stateful.h:116

Returns true if the current state ID is in the inclusive range [lid, rid]. 
#### Parameters
* `lid` Lower bound state ID. 

* `rid` Upper bound state ID. 

#### Returns
True if lid <= state.id() <= rid.

---

{#state}

#### state

`virtual` `inline`

```cpp
virtual inline T & state()
```

Defined in src/base/include/icy/stateful.h:123

Returns a mutable reference to the current state. 
#### Returns
Reference to the internal state object.

---

{#state}

#### state

`virtual` `const` `inline`

```cpp
virtual inline const T state() const
```

Defined in src/base/include/icy/stateful.h:127

Returns a copy of the current state. 
#### Returns
Current state value.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `T` | [`_state`](#_state)  |  |

---

{#_state}

#### _state

```cpp
T _state
```

Defined in src/base/include/icy/stateful.h:170

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `bool` | [`beforeStateChange`](#beforestatechange) `virtual` `inline` | Override to handle pre state change logic. Return false to prevent state change. |
| `void` | [`onStateChange`](#onstatechange) `virtual` `inline` | Override to handle post state change logic. |
| `bool` | [`setState`](#setstate) `virtual` `inline` | Sets the state and sends the state signal if the state change was successful. |
| `bool` | [`setState`](#setstate) `virtual` `inline` | Sets the state and sends the state signal if the state change was successful. |

---

{#beforestatechange}

#### beforeStateChange

`virtual` `inline`

```cpp
virtual inline bool beforeStateChange(const T & state)
```

Defined in src/base/include/icy/stateful.h:135

Override to handle pre state change logic. Return false to prevent state change.

---

{#onstatechange}

#### onStateChange

`virtual` `inline`

```cpp
virtual inline void onStateChange(T &, const T &)
```

Defined in src/base/include/icy/stateful.h:143

Override to handle post state change logic.

---

{#setstate}

#### setState

`virtual` `inline`

```cpp
virtual inline bool setState(void * sender, typename T::ID id)
```

Defined in src/base/include/icy/stateful.h:147

Sets the state and sends the state signal if the state change was successful.

---

{#setstate}

#### setState

`virtual` `inline`

```cpp
virtual inline bool setState(void * sender, const T & state)
```

Defined in src/base/include/icy/stateful.h:156

Sets the state and sends the state signal if the state change was successful.

{#iregistry}

## IRegistry

```cpp
#include <icy/iregistry.h>
```

```cpp
template<class ItemT>
class IRegistry
```

Defined in src/base/include/icy/iregistry.h:25

Abstract interface for object registries.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `Signal< void(const std::string &)>` | [`TypeRegistered`](#typeregistered)  |  |
| `Signal< void(const std::string &)>` | [`TypeUnregistered`](#typeunregistered)  |  |

---

{#typeregistered}

#### TypeRegistered

```cpp
Signal< void(const std::string &)> TypeRegistered
```

Defined in src/base/include/icy/iregistry.h:77

---

{#typeunregistered}

#### TypeUnregistered

```cpp
Signal< void(const std::string &)> TypeUnregistered
```

Defined in src/base/include/icy/iregistry.h:78

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`IRegistry`](#iregistry)  | Defaulted constructor. |
| `ItemT *` | [`createInstance`](#createinstance) `virtual` `inline` | Creates and returns a new heap-allocated instance of the type registered under key s. |
| `void` | [`registerType`](#registertype) `inline` | Registers type T under the given key s. Emits TypeRegistered. Subsequent calls to createInstance(s) will return `new T()`. |
| `void` | [`unregisterType`](#unregistertype) `virtual` `inline` | Removes the type registered under key s. Emits TypeUnregistered. Does nothing if s is not registered. |
| `TypeMap` | [`types`](#types) `const` `inline` | Returns a copy of the current type map. |

---

{#iregistry}

#### IRegistry

```cpp
IRegistry() = default
```

Defined in src/base/include/icy/iregistry.h:36

Defaulted constructor.

---

{#createinstance}

#### createInstance

`virtual` `inline`

```cpp
virtual inline ItemT * createInstance(const std::string & s)
```

Defined in src/base/include/icy/iregistry.h:42

Creates and returns a new heap-allocated instance of the type registered under key s. 
#### Parameters
* `s` The registration key (type name). 

#### Returns
Pointer to the new instance, or nullptr if s is not registered.

---

{#registertype}

#### registerType

`inline`

```cpp
template<typename T> inline void registerType(const std::string & s)
```

Defined in src/base/include/icy/iregistry.h:55

Registers type T under the given key s. Emits TypeRegistered. Subsequent calls to createInstance(s) will return `new T()`. 
#### Parameters
* `T` Concrete type to register; must be default-constructible and derive from ItemT. 

#### Parameters
* `s` Registration key (type name).

---

{#unregistertype}

#### unregisterType

`virtual` `inline`

```cpp
virtual inline void unregisterType(const std::string & s)
```

Defined in src/base/include/icy/iregistry.h:64

Removes the type registered under key s. Emits TypeUnregistered. Does nothing if s is not registered. 
#### Parameters
* `s` Registration key to remove.

---

{#types}

#### types

`const` `inline`

```cpp
inline TypeMap types() const
```

Defined in src/base/include/icy/iregistry.h:75

Returns a copy of the current type map. 
#### Returns
Map of registration keys to factory function pointers.

### Public Types

| Name | Description |
|------|-------------|
| [`TypeMap`](#typemap)  |  |

---

{#typemap}

#### TypeMap

```cpp
using TypeMap = std::map< std::string, ItemT *(*)()>
```

Defined in src/base/include/icy/iregistry.h:34

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `TypeMap` | [`_types`](#_types)  |  |

---

{#_types}

#### _types

```cpp
TypeMap _types
```

Defined in src/base/include/icy/iregistry.h:81

### Private Static Methods

| Return | Name | Description |
|--------|------|-------------|
| `ItemT *` | [`createT`](#createt) `static` `inline` |  |

---

{#createt}

#### createT

`static` `inline`

```cpp
template<typename T> static inline ItemT * createT()
```

Defined in src/base/include/icy/iregistry.h:28

{#singleton}

## Singleton

```cpp
#include <icy/singleton.h>
```

```cpp
template<class S>
class Singleton
```

Defined in src/base/include/icy/singleton.h:24

Helper template class for managing singleton objects allocated on the heap.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Singleton`](#singleton)  | Defaulted constructor. |
| `S *` | [`get`](#get) `inline` | Returns a pointer to the managed singleton, instantiating it on first call. Thread-safe; protected by an internal mutex. |
| `S *` | [`swap`](#swap) `inline` | Replaces the managed singleton with newPtr and returns the previous instance. The caller takes ownership of the returned pointer. Thread-safe; protected by an internal mutex. |
| `void` | [`destroy`](#destroy) `inline` | Destroys the managed singleton instance and resets the internal pointer to null. Thread-safe; protected by an internal mutex. |

---

{#singleton}

#### Singleton

```cpp
Singleton() = default
```

Defined in src/base/include/icy/singleton.h:27

Defaulted constructor.

---

{#get}

#### get

`inline`

```cpp
inline S * get()
```

Defined in src/base/include/icy/singleton.h:34

Returns a pointer to the managed singleton, instantiating it on first call. Thread-safe; protected by an internal mutex. 
#### Returns
Pointer to the singleton instance (never null).

---

{#swap}

#### swap

`inline`

```cpp
inline S * swap(S * newPtr)
```

Defined in src/base/include/icy/singleton.h:47

Replaces the managed singleton with newPtr and returns the previous instance. The caller takes ownership of the returned pointer. Thread-safe; protected by an internal mutex. 
#### Parameters
* `newPtr` Replacement instance (may be nullptr). 

#### Returns
Previously managed pointer (caller must delete if non-null).

---

{#destroy}

#### destroy

`inline`

```cpp
inline void destroy()
```

Defined in src/base/include/icy/singleton.h:57

Destroys the managed singleton instance and resets the internal pointer to null. Thread-safe; protected by an internal mutex.

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::unique_ptr< S, Deleter >` | [`_ptr`](#_ptr)  |  |
| `std::mutex` | [`_m`](#_m)  |  |

---

{#_ptr}

#### _ptr

```cpp
std::unique_ptr< S, Deleter > _ptr
```

Defined in src/base/include/icy/singleton.h:70

---

{#_m}

#### _m

```cpp
std::mutex _m
```

Defined in src/base/include/icy/singleton.h:71

{#keyedstore}

## KeyedStore

```cpp
#include <icy/collection.h>
```

```cpp
template<class TKey, class TValue>
class KeyedStore
```

Defined in src/base/include/icy/collection.h:37

> **Subclassed by:** [`TimedManager< TKey, TValue >`](#timedmanager)

A keyed store of unique_ptr values with optional lifecycle hooks. Not thread-safe; designed for single-threaded event loop contexts.

Subclasses can override onAdd/onRemove for lifecycle reactions, and add [Signal](#signal) members for external observers.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`KeyedStore`](#keyedstore)  | Defaulted constructor. |
|  | [`KeyedStore`](#keyedstore)  | Deleted constructor. |
|  | [`KeyedStore`](#keyedstore)  | Defaulted constructor. |
| `TValue *` | [`get`](#get) `const` `inline` | Returns the item for `key`, or nullptr if not found. |
| `TValue &` | [`add`](#add) `inline` | Inserts a uniquely owned item. Returns a reference to the stored item. |
| `bool` | [`tryAdd`](#tryadd) `inline` | Inserts if absent; returns false on duplicate (never throws). |
| `TValue &` | [`put`](#put) `inline` | Inserts or replaces the item under `key`. |
| `bool` | [`erase`](#erase) `inline` | Removes and destroys the item under `key`. |
| `bool` | [`contains`](#contains) `const` `inline` |  |
| `bool` | [`empty`](#empty) `const` `inline` |  |
| `size_t` | [`size`](#size) `const` `inline` |  |
| `void` | [`clear`](#clear) `inline` |  |
| `Map &` | [`map`](#map) `inline` | Direct map access for iteration. |
| `const Map &` | [`map`](#map) `const` `inline` |  |
| `auto` | [`begin`](#begin) `inline` |  |
| `auto` | [`end`](#end) `inline` |  |
| `auto` | [`begin`](#begin) `const` `inline` |  |
| `auto` | [`end`](#end) `const` `inline` |  |

---

{#keyedstore}

#### KeyedStore

```cpp
KeyedStore() = default
```

Defined in src/base/include/icy/collection.h:42

Defaulted constructor.

---

{#keyedstore}

#### KeyedStore

```cpp
KeyedStore(const KeyedStore &) = delete
```

Defined in src/base/include/icy/collection.h:46

Deleted constructor.

---

{#keyedstore}

#### KeyedStore

```cpp
KeyedStore(KeyedStore &&) = default
```

Defined in src/base/include/icy/collection.h:48

Defaulted constructor.

---

{#get}

#### get

`const` `inline`

```cpp
inline TValue * get(const TKey & key) const
```

Defined in src/base/include/icy/collection.h:52

Returns the item for `key`, or nullptr if not found.

---

{#add}

#### add

`inline`

```cpp
inline TValue & add(const TKey & key, std::unique_ptr< TValue > item)
```

Defined in src/base/include/icy/collection.h:60

Inserts a uniquely owned item. Returns a reference to the stored item. 
#### Exceptions
* `std::runtime_error` if the key already exists.

---

{#tryadd}

#### tryAdd

`inline`

```cpp
inline bool tryAdd(const TKey & key, std::unique_ptr< TValue > item)
```

Defined in src/base/include/icy/collection.h:71

Inserts if absent; returns false on duplicate (never throws).

---

{#put}

#### put

`inline`

```cpp
inline TValue & put(const TKey & key, std::unique_ptr< TValue > item)
```

Defined in src/base/include/icy/collection.h:82

Inserts or replaces the item under `key`.

---

{#erase}

#### erase

`inline`

```cpp
inline bool erase(const TKey & key)
```

Defined in src/base/include/icy/collection.h:92

Removes and destroys the item under `key`. 
#### Returns
true if the item existed and was removed.

---

{#contains}

#### contains

`const` `inline`

```cpp
inline bool contains(const TKey & key) const
```

Defined in src/base/include/icy/collection.h:104

---

{#empty}

#### empty

`const` `inline`

```cpp
inline bool empty() const
```

Defined in src/base/include/icy/collection.h:105

---

{#size}

#### size

`const` `inline`

```cpp
inline size_t size() const
```

Defined in src/base/include/icy/collection.h:106

---

{#clear}

#### clear

`inline`

```cpp
inline void clear()
```

Defined in src/base/include/icy/collection.h:107

---

{#map}

#### map

`inline`

```cpp
inline Map & map()
```

Defined in src/base/include/icy/collection.h:110

Direct map access for iteration.

---

{#map}

#### map

`const` `inline`

```cpp
inline const Map & map() const
```

Defined in src/base/include/icy/collection.h:111

---

{#begin}

#### begin

`inline`

```cpp
inline auto begin()
```

Defined in src/base/include/icy/collection.h:114

---

{#end}

#### end

`inline`

```cpp
inline auto end()
```

Defined in src/base/include/icy/collection.h:115

---

{#begin}

#### begin

`const` `inline`

```cpp
inline auto begin() const
```

Defined in src/base/include/icy/collection.h:116

---

{#end}

#### end

`const` `inline`

```cpp
inline auto end() const
```

Defined in src/base/include/icy/collection.h:117

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `Map` | [`_map`](#_map)  |  |

---

{#_map}

#### _map

```cpp
Map _map
```

Defined in src/base/include/icy/collection.h:124

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`onAdd`](#onadd) `virtual` `inline` | Override for lifecycle reactions. |
| `void` | [`onRemove`](#onremove) `virtual` `inline` |  |

---

{#onadd}

#### onAdd

`virtual` `inline`

```cpp
virtual inline void onAdd(const TKey &, TValue *)
```

Defined in src/base/include/icy/collection.h:121

Override for lifecycle reactions.

---

{#onremove}

#### onRemove

`virtual` `inline`

```cpp
virtual inline void onRemove(const TKey &, TValue *)
```

Defined in src/base/include/icy/collection.h:122

### Public Types

| Name | Description |
|------|-------------|
| [`Map`](#map)  |  |

---

{#map}

#### Map

```cpp
using Map = std::map< TKey, std::unique_ptr< TValue > >
```

Defined in src/base/include/icy/collection.h:40

{#kvcollection}

## KVCollection

```cpp
#include <icy/collection.h>
```

```cpp
template<class TKey, class TValue>
class KVCollection
```

Defined in src/base/include/icy/collection.h:135

A keyed value store (values stored by copy, not pointer).

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`KVCollection`](#kvcollection)  | Defaulted constructor. |
| `bool` | [`add`](#add) `inline` | Inserts a value; returns false if key already exists. |
| `TValue &` | [`get`](#get) `inline` | Returns the value or throws. |
| `const TValue &` | [`get`](#get) `const` `inline` | Returns the value or defaultValue. |
| `bool` | [`remove`](#remove) `inline` |  |
| `bool` | [`has`](#has) `const` `inline` |  |
| `bool` | [`empty`](#empty) `const` `inline` |  |
| `size_t` | [`size`](#size) `const` `inline` |  |
| `void` | [`clear`](#clear) `inline` |  |
| `Map &` | [`map`](#map) `inline` |  |
| `const Map &` | [`map`](#map) `const` `inline` |  |

---

{#kvcollection}

#### KVCollection

```cpp
KVCollection() = default
```

Defined in src/base/include/icy/collection.h:140

Defaulted constructor.

---

{#add}

#### add

`inline`

```cpp
inline bool add(const TKey & key, const TValue & item)
```

Defined in src/base/include/icy/collection.h:144

Inserts a value; returns false if key already exists.

---

{#get}

#### get

`inline`

```cpp
inline TValue & get(const TKey & key)
```

Defined in src/base/include/icy/collection.h:151

Returns the value or throws.

---

{#get}

#### get

`const` `inline`

```cpp
inline const TValue & get(const TKey & key, const TValue & defaultValue) const
```

Defined in src/base/include/icy/collection.h:160

Returns the value or defaultValue.

---

{#remove}

#### remove

`inline`

```cpp
inline bool remove(const TKey & key)
```

Defined in src/base/include/icy/collection.h:166

---

{#has}

#### has

`const` `inline`

```cpp
inline bool has(const TKey & key) const
```

Defined in src/base/include/icy/collection.h:167

---

{#empty}

#### empty

`const` `inline`

```cpp
inline bool empty() const
```

Defined in src/base/include/icy/collection.h:168

---

{#size}

#### size

`const` `inline`

```cpp
inline size_t size() const
```

Defined in src/base/include/icy/collection.h:169

---

{#clear}

#### clear

`inline`

```cpp
inline void clear()
```

Defined in src/base/include/icy/collection.h:170

---

{#map}

#### map

`inline`

```cpp
inline Map & map()
```

Defined in src/base/include/icy/collection.h:171

---

{#map}

#### map

`const` `inline`

```cpp
inline const Map & map() const
```

Defined in src/base/include/icy/collection.h:172

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `Map` | [`_map`](#_map)  |  |

---

{#_map}

#### _map

```cpp
Map _map
```

Defined in src/base/include/icy/collection.h:175

### Public Types

| Name | Description |
|------|-------------|
| [`Map`](#map)  |  |

---

{#map}

#### Map

```cpp
using Map = std::map< TKey, TValue >
```

Defined in src/base/include/icy/collection.h:138

{#nvcollection}

## NVCollection

```cpp
#include <icy/collection.h>
```

```cpp
class NVCollection
```

Defined in src/base/include/icy/collection.h:187

> **Subclassed by:** [`FormWriter`](http.md#formwriter), [`Message`](http.md#message)

A storage container for a name value collections. This collection can store multiple entries for each name, and it's getters are case-insensitive.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`NVCollection`](#nvcollection) `inline` |  |
|  | [`NVCollection`](#nvcollection) `inline` |  |
|  | [`NVCollection`](#nvcollection) `inline` `noexcept` |  |
| `NVCollection &` | [`operator=`](#operator)  | Assigns the name-value pairs of another [NVCollection](#nvcollection) to this one. |
| `const std::string &` | [`operator[]`](#operator) `const` | Returns the value of the (first) name-value pair with the given name. |
| `void` | [`set`](#set)  | Sets the value of the (first) name-value pair with the given name. |
| `void` | [`add`](#add)  | Adds a new name-value pair with the given name and value. |
| `void` | [`add`](#add)  | Adds a new name-value pair using move semantics. |
| `const std::string &` | [`get`](#get) `const` | Returns the value of the first name-value pair with the given name. |
| `const std::string &` | [`get`](#get) `const` | Returns the value of the first name-value pair with the given name. If no value with the given name has been found, the defaultValue is returned. |
| `bool` | [`has`](#has) `const` | Returns true if there is at least one name-value pair with the given name. |
| `ConstIterator` | [`find`](#find) `const` | Returns an iterator pointing to the first name-value pair with the given name. |
| `ConstIterator` | [`begin`](#begin) `const` | Returns an iterator pointing to the begin of the name-value pair collection. |
| `ConstIterator` | [`end`](#end) `const` | Returns an iterator pointing to the end of the name-value pair collection. |
| `bool` | [`empty`](#empty) `const` | Returns true iff the header does not have any content. |
| `int` | [`size`](#size) `const` | Returns the number of name-value pairs in the collection. |
| `void` | [`erase`](#erase)  | Removes all name-value pairs with the given name. |
| `void` | [`clear`](#clear)  | Removes all name-value pairs and their values. |

---

{#nvcollection}

#### NVCollection

`inline`

```cpp
inline NVCollection()
```

Defined in src/base/include/icy/collection.h:194

---

{#nvcollection}

#### NVCollection

`inline`

```cpp
inline NVCollection(const NVCollection & nvc)
```

Defined in src/base/include/icy/collection.h:198

---

{#nvcollection}

#### NVCollection

`inline` `noexcept`

```cpp
inline NVCollection(NVCollection && nvc) noexcept
```

Defined in src/base/include/icy/collection.h:203

---

{#operator}

#### operator=

```cpp
NVCollection & operator=(const NVCollection & nvc)
```

Defined in src/base/include/icy/collection.h:213

Assigns the name-value pairs of another [NVCollection](#nvcollection) to this one.

---

{#operator}

#### operator[]

`const`

```cpp
const std::string & operator[](std::string_view name) const
```

Defined in src/base/include/icy/collection.h:219

Returns the value of the (first) name-value pair with the given name.

Throws a NotFoundException if the name-value pair does not exist.

---

{#set}

#### set

```cpp
void set(const std::string & name, const std::string & value)
```

Defined in src/base/include/icy/collection.h:222

Sets the value of the (first) name-value pair with the given name.

---

{#add}

#### add

```cpp
void add(const std::string & name, const std::string & value)
```

Defined in src/base/include/icy/collection.h:225

Adds a new name-value pair with the given name and value.

---

{#add}

#### add

```cpp
void add(std::string && name, std::string && value)
```

Defined in src/base/include/icy/collection.h:228

Adds a new name-value pair using move semantics.

---

{#get}

#### get

`const`

```cpp
const std::string & get(std::string_view name) const
```

Defined in src/base/include/icy/collection.h:233

Returns the value of the first name-value pair with the given name.

Throws a NotFoundException if the name-value pair does not exist.

---

{#get}

#### get

`const`

```cpp
const std::string & get(std::string_view name, const std::string & defaultValue) const
```

Defined in src/base/include/icy/collection.h:238

Returns the value of the first name-value pair with the given name. If no value with the given name has been found, the defaultValue is returned.

---

{#has}

#### has

`const`

```cpp
bool has(std::string_view name) const
```

Defined in src/base/include/icy/collection.h:242

Returns true if there is at least one name-value pair with the given name.

---

{#find}

#### find

`const`

```cpp
ConstIterator find(std::string_view name) const
```

Defined in src/base/include/icy/collection.h:246

Returns an iterator pointing to the first name-value pair with the given name.

---

{#begin}

#### begin

`const`

```cpp
ConstIterator begin() const
```

Defined in src/base/include/icy/collection.h:250

Returns an iterator pointing to the begin of the name-value pair collection.

---

{#end}

#### end

`const`

```cpp
ConstIterator end() const
```

Defined in src/base/include/icy/collection.h:254

Returns an iterator pointing to the end of the name-value pair collection.

---

{#empty}

#### empty

`const`

```cpp
bool empty() const
```

Defined in src/base/include/icy/collection.h:257

Returns true iff the header does not have any content.

---

{#size}

#### size

`const`

```cpp
int size() const
```

Defined in src/base/include/icy/collection.h:261

Returns the number of name-value pairs in the collection.

---

{#erase}

#### erase

```cpp
void erase(std::string_view name)
```

Defined in src/base/include/icy/collection.h:264

Removes all name-value pairs with the given name.

---

{#clear}

#### clear

```cpp
void clear()
```

Defined in src/base/include/icy/collection.h:267

Removes all name-value pairs and their values.

### Public Types

| Name | Description |
|------|-------------|
| [`Map`](#map)  |  |
| [`Iterator`](#iterator)  |  |
| [`ConstIterator`](#constiterator)  |  |

---

{#map}

#### Map

```cpp
using Map = std::vector< std::pair< std::string, std::string > >
```

Defined in src/base/include/icy/collection.h:190

---

{#iterator}

#### Iterator

```cpp
using Iterator = Map::iterator
```

Defined in src/base/include/icy/collection.h:191

---

{#constiterator}

#### ConstIterator

```cpp
using ConstIterator = Map::const_iterator
```

Defined in src/base/include/icy/collection.h:192

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `Map` | [`_map`](#_map)  |  |

---

{#_map}

#### _map

```cpp
Map _map
```

Defined in src/base/include/icy/collection.h:270

{#application}

## Application

```cpp
#include <icy/application.h>
```

```cpp
class Application
```

Defined in src/base/include/icy/application.h:36

Main icey application class.

This class exposes basic features required by most applications:

* Running the event loop

* Command line option parsing **See also**: [OptionParser](#optionparser)

* Shutdown signal (Ctrl-C) handling

* Garbage collection

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `uv::Loop *` | [`loop`](#loop)  | Active event loop. |

---

{#loop}

#### loop

```cpp
uv::Loop * loop
```

Defined in src/base/include/icy/application.h:53

Active event loop.

The event loop may be assigned on construction, otherwise the default event loop will be used.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Application`](#application)  | Constructor. |
|  | [`~Application`](#application)  | Destructor. |
| `void` | [`run`](#run)  | Run the application event loop. |
| `void` | [`stop`](#stop)  | Stop the application event loop. |
| `void` | [`finalize`](#finalize)  | Finalize and free any remaining pointers still held by the application event loop. |
| `void` | [`waitForShutdown`](#waitforshutdown)  | Bind the shutdown signal and run the main event loop. |
| `void` | [`bindShutdownSignal`](#bindshutdownsignal)  | Bind the shutdown signal. |

---

{#application}

#### Application

```cpp
Application(uv::Loop * loop = uv::defaultLoop())
```

Defined in src/base/include/icy/application.h:40

Constructor.

---

{#application}

#### ~Application

```cpp
~Application()
```

Defined in src/base/include/icy/application.h:43

Destructor.

---

{#run}

#### run

```cpp
void run()
```

Defined in src/base/include/icy/application.h:60

Run the application event loop.

---

{#stop}

#### stop

```cpp
void stop()
```

Defined in src/base/include/icy/application.h:63

Stop the application event loop.

---

{#finalize}

#### finalize

```cpp
void finalize()
```

Defined in src/base/include/icy/application.h:67

Finalize and free any remaining pointers still held by the application event loop.

---

{#waitforshutdown}

#### waitForShutdown

```cpp
void waitForShutdown(std::function< void(void *)> callback = nullptr, void * opaque = nullptr)
```

Defined in src/base/include/icy/application.h:74

Bind the shutdown signal and run the main event loop.

---

{#bindshutdownsignal}

#### bindShutdownSignal

```cpp
void bindShutdownSignal(std::function< void(void *)> callback = nullptr, void * opaque = nullptr)
```

Defined in src/base/include/icy/application.h:78

Bind the shutdown signal.

### Public Static Methods

| Return | Name | Description |
|--------|------|-------------|
| `Application &` | [`getDefault`](#getdefault) `static` | Returns the default [Application](#application) singleton, although [Application](#application) instances may be initialized individually. |

---

{#getdefault}

#### getDefault

`static`

```cpp
static Application & getDefault()
```

Defined in src/base/include/icy/application.h:47

Returns the default [Application](#application) singleton, although [Application](#application) instances may be initialized individually.

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Application`](#application)  | Deleted constructor. |
|  | [`Application`](#application)  | Deleted constructor. |

---

{#application}

#### Application

```cpp
Application(const Application &) = delete
```

Defined in src/base/include/icy/application.h:82

Deleted constructor.

---

{#application}

#### Application

```cpp
Application(Application &&) = delete
```

Defined in src/base/include/icy/application.h:84

Deleted constructor.

{#syncpacketqueue}

## SyncPacketQueue

```cpp
#include <icy/packetqueue.h>
```

```cpp
template<class T = IPacket>
class SyncPacketQueue
```

Defined in src/base/include/icy/packetqueue.h:31

> **Inherits:** [`SyncQueue< IPacket >`](#syncqueue), [`PacketProcessor`](#packetprocessor)

Synchronized packet queue for event loop integration.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `PacketSignal` | [`emitter`](#emitter)  |  |

---

{#emitter}

#### emitter

```cpp
PacketSignal emitter
```

Defined in src/base/include/icy/packetqueue.h:72

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`SyncPacketQueue`](#syncpacketqueue) `inline` |  |
|  | [`SyncPacketQueue`](#syncpacketqueue) `inline` | Uses the default libuv event loop. |
| `void` | [`process`](#process) `virtual` `inline` `override` | Clones the incoming packet and pushes it onto the queue for synchronized dispatch. This queue is therefore an explicit [PacketStream](#packetstream) ownership boundary. Drops the packet with a warning if the queue has been cancelled. |
| `bool` | [`accepts`](#accepts) `virtual` `inline` `override` | Returns true if the packet can be cast to type `T`. |
| `PacketRetention` | [`retention`](#retention) `virtual` `const` `inline` `nodiscard` `override` | Returns how this adapter treats incoming packet lifetime. Most adapters are synchronous and therefore only borrow the packet for the current call chain. Queue-style adapters override this to advertise that they clone before deferred use. Callers may treat the first adapter reporting Cloned or Retained as the explicit ownership boundary in the stream graph. |

---

{#syncpacketqueue}

#### SyncPacketQueue

`inline`

```cpp
inline SyncPacketQueue(uv::Loop * loop, int maxSize = 1024)
```

Defined in src/base/include/icy/packetqueue.h:40

#### Parameters
* `loop` Event loop to synchronize dispatch onto. 

* `maxSize` Maximum number of queued packets before oldest are dropped.

---

{#syncpacketqueue}

#### SyncPacketQueue

`inline`

```cpp
inline SyncPacketQueue(int maxSize = 1024)
```

Defined in src/base/include/icy/packetqueue.h:48

Uses the default libuv event loop. 
#### Parameters
* `maxSize` Maximum number of queued packets before oldest are dropped.

---

{#process}

#### process

`virtual` `inline` `override`

```cpp
virtual inline void process(IPacket & packet) override
```

Defined in src/base/include/icy/packetqueue.h:60

Clones the incoming packet and pushes it onto the queue for synchronized dispatch. This queue is therefore an explicit [PacketStream](#packetstream) ownership boundary. Drops the packet with a warning if the queue has been cancelled. 
#### Parameters
* `packet` Incoming packet to enqueue.

---

{#accepts}

#### accepts

`virtual` `inline` `override`

```cpp
virtual inline bool accepts(IPacket * packet) override
```

Defined in src/base/include/icy/packetqueue.h:65

Returns true if the packet can be cast to type `T`. 
#### Parameters
* `packet` Packet to test. 

#### Returns
True if `dynamic_cast<T*>(packet)` succeeds.

---

{#retention}

#### retention

`virtual` `const` `inline` `nodiscard` `override`

```cpp
[[nodiscard]] virtual inline PacketRetention retention() const override
```

Defined in src/base/include/icy/packetqueue.h:67

Returns how this adapter treats incoming packet lifetime. Most adapters are synchronous and therefore only borrow the packet for the current call chain. Queue-style adapters override this to advertise that they clone before deferred use. Callers may treat the first adapter reporting Cloned or Retained as the explicit ownership boundary in the stream graph.

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`dispatch`](#dispatch) `inline` `override` | Emits the packet to downstream processors from the event loop thread. |
| `void` | [`onStreamStateChange`](#onstreamstatechange) `virtual` `inline` `override` | Cancels the queue on `Closed` or `[Error](#error)` stream state transitions. |

---

{#dispatch}

#### dispatch

`inline` `override`

```cpp
inline void dispatch(T & packet) override
```

Defined in src/base/include/icy/packetqueue.h:77

Emits the packet to downstream processors from the event loop thread. 
#### Parameters
* `packet` Packet to dispatch.

---

{#onstreamstatechange}

#### onStreamStateChange

`virtual` `inline` `override`

```cpp
virtual inline void onStreamStateChange(const PacketStreamState &) override
```

Defined in src/base/include/icy/packetqueue.h:81

Cancels the queue on `Closed` or `[Error](#error)` stream state transitions. 
#### Parameters
* `state` New stream state.

### Public Types

| Name | Description |
|------|-------------|
| [`Queue`](#queue)  |  |
| [`Processor`](#processor)  |  |

---

{#queue}

#### Queue

```cpp
using Queue = SyncQueue< T >
```

Defined in src/base/include/icy/packetqueue.h:35

---

{#processor}

#### Processor

```cpp
using Processor = PacketProcessor
```

Defined in src/base/include/icy/packetqueue.h:36

{#asyncpacketqueue}

## AsyncPacketQueue

```cpp
#include <icy/packetqueue.h>
```

```cpp
template<class T = IPacket>
class AsyncPacketQueue
```

Defined in src/base/include/icy/packetqueue.h:145

> **Inherits:** [`AsyncQueue< IPacket >`](#asyncqueue), [`PacketProcessor`](#packetprocessor)

Thread-based asynchronous packet dispatch queue.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `PacketSignal` | [`emitter`](#emitter)  |  |

---

{#emitter}

#### emitter

```cpp
PacketSignal emitter
```

Defined in src/base/include/icy/packetqueue.h:180

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`AsyncPacketQueue`](#asyncpacketqueue) `inline` |  |
| `void` | [`close`](#close) `virtual` `inline` | Flushes remaining packets, cancels the queue, and joins the dispatch thread. |
| `void` | [`process`](#process) `virtual` `inline` `override` | Clones the incoming packet and pushes it onto the async queue. This queue is therefore an explicit [PacketStream](#packetstream) ownership boundary. Drops the packet with a warning if the queue has been cancelled. |
| `bool` | [`accepts`](#accepts) `virtual` `inline` `override` | Returns true if the packet can be cast to type `T`. |
| `PacketRetention` | [`retention`](#retention) `virtual` `const` `inline` `nodiscard` `override` | Returns how this adapter treats incoming packet lifetime. Most adapters are synchronous and therefore only borrow the packet for the current call chain. Queue-style adapters override this to advertise that they clone before deferred use. Callers may treat the first adapter reporting Cloned or Retained as the explicit ownership boundary in the stream graph. |

---

{#asyncpacketqueue}

#### AsyncPacketQueue

`inline`

```cpp
inline AsyncPacketQueue(int maxSize = 1024)
```

Defined in src/base/include/icy/packetqueue.h:153

#### Parameters
* `maxSize` Maximum number of queued packets before oldest are dropped.

---

{#close}

#### close

`virtual` `inline`

```cpp
virtual inline void close()
```

Defined in src/base/include/icy/packetqueue.h:162

Flushes remaining packets, cancels the queue, and joins the dispatch thread.

---

{#process}

#### process

`virtual` `inline` `override`

```cpp
virtual inline void process(IPacket & packet) override
```

Defined in src/base/include/icy/packetqueue.h:168

Clones the incoming packet and pushes it onto the async queue. This queue is therefore an explicit [PacketStream](#packetstream) ownership boundary. Drops the packet with a warning if the queue has been cancelled. 
#### Parameters
* `packet` Incoming packet to enqueue.

---

{#accepts}

#### accepts

`virtual` `inline` `override`

```cpp
virtual inline bool accepts(IPacket * packet) override
```

Defined in src/base/include/icy/packetqueue.h:173

Returns true if the packet can be cast to type `T`. 
#### Parameters
* `packet` Packet to test. 

#### Returns
True if `dynamic_cast<T*>(packet)` succeeds.

---

{#retention}

#### retention

`virtual` `const` `inline` `nodiscard` `override`

```cpp
[[nodiscard]] virtual inline PacketRetention retention() const override
```

Defined in src/base/include/icy/packetqueue.h:175

Returns how this adapter treats incoming packet lifetime. Most adapters are synchronous and therefore only borrow the packet for the current call chain. Queue-style adapters override this to advertise that they clone before deferred use. Callers may treat the first adapter reporting Cloned or Retained as the explicit ownership boundary in the stream graph.

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`dispatch`](#dispatch) `inline` `override` | Emits the packet to downstream processors from the async thread. |
| `void` | [`onStreamStateChange`](#onstreamstatechange) `virtual` `inline` `override` | Closes the queue on `[Error](#error)` or `Closed` stream state transitions. |

---

{#dispatch}

#### dispatch

`inline` `override`

```cpp
inline void dispatch(T & packet) override
```

Defined in src/base/include/icy/packetqueue.h:185

Emits the packet to downstream processors from the async thread. 
#### Parameters
* `packet` Packet to dispatch.

---

{#onstreamstatechange}

#### onStreamStateChange

`virtual` `inline` `override`

```cpp
virtual inline void onStreamStateChange(const PacketStreamState &) override
```

Defined in src/base/include/icy/packetqueue.h:189

Closes the queue on `[Error](#error)` or `Closed` stream state transitions. 
#### Parameters
* `state` New stream state.

### Public Types

| Name | Description |
|------|-------------|
| [`Queue`](#queue)  |  |
| [`Processor`](#processor)  |  |

---

{#queue}

#### Queue

```cpp
using Queue = AsyncQueue< T >
```

Defined in src/base/include/icy/packetqueue.h:149

---

{#processor}

#### Processor

```cpp
using Processor = PacketProcessor
```

Defined in src/base/include/icy/packetqueue.h:150

{#ratelimiter}

## RateLimiter

```cpp
#include <icy/ratelimiter.h>
```

```cpp
class RateLimiter
```

Defined in src/base/include/icy/ratelimiter.h:23

Token bucket rate limiter for throttling message send frequency.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `double` | [`rate`](#rate)  | How many messages. |
| `double` | [`seconds`](#seconds)  | Over how many seconds. |
| `double` | [`allowance`](#allowance)  | Remaining send allowance. |

---

{#rate}

#### rate

```cpp
double rate
```

Defined in src/base/include/icy/ratelimiter.h:26

How many messages.

---

{#seconds}

#### seconds

```cpp
double seconds
```

Defined in src/base/include/icy/ratelimiter.h:27

Over how many seconds.

---

{#allowance}

#### allowance

```cpp
double allowance
```

Defined in src/base/include/icy/ratelimiter.h:28

Remaining send allowance.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`RateLimiter`](#ratelimiter) `inline` | Constructs a token bucket limiter. |
| `bool` | [`canSend`](#cansend) `inline` | Returns true if a message may be sent without exceeding the rate limit. Replenishes the token bucket based on elapsed time since the last check, then consumes one token. Returns false if the bucket is empty. |

---

{#ratelimiter}

#### RateLimiter

`inline`

```cpp
inline RateLimiter(double rate = 5.0, double seconds = 6.0)
```

Defined in src/base/include/icy/ratelimiter.h:33

Constructs a token bucket limiter. 
#### Parameters
* `rate` Maximum number of messages permitted in the window. 

* `seconds` Duration of the replenishment window in seconds.

---

{#cansend}

#### canSend

`inline`

```cpp
inline bool canSend()
```

Defined in src/base/include/icy/ratelimiter.h:46

Returns true if a message may be sent without exceeding the rate limit. Replenishes the token bucket based on elapsed time since the last check, then consumes one token. Returns false if the bucket is empty. 
#### Returns
true if sending is allowed, false if the rate limit is exceeded.

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::chrono::steady_clock::time_point` | [`_lastCheck`](#_lastcheck)  |  |
| `bool` | [`_started`](#_started)  |  |

---

{#_lastcheck}

#### _lastCheck

```cpp
std::chrono::steady_clock::time_point _lastCheck
```

Defined in src/base/include/icy/ratelimiter.h:76

---

{#_started}

#### _started

```cpp
bool _started
```

Defined in src/base/include/icy/ratelimiter.h:77

{#packetstreamadapter}

## PacketStreamAdapter

```cpp
#include <icy/packetstream.h>
```

```cpp
class PacketStreamAdapter
```

Defined in src/base/include/icy/packetstream.h:42

> **Subclassed by:** [`PacketProcessor`](#packetprocessor), [`ThreadedStreamReader`](#threadedstreamreader), [`ICapture`](av.md#icapture), [`FormWriter`](http.md#formwriter), [`WebRtcTrackReceiver`](webrtc.md#webrtctrackreceiver)

This class is a wrapper for integrating external classes with the a [PacketStream](#packetstream)'s data flow and state machine.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`PacketStreamAdapter`](#packetstreamadapter)  | Construct the adapter, binding it to the given packet signal. |
| `void` | [`emit`](#emit) `virtual` | Emit a mutable raw buffer as a packet. |
| `void` | [`emit`](#emit) `virtual` | Emit a read-only raw buffer as a packet (data is copied internally). |
| `void` | [`emit`](#emit) `virtual` | Emit a string as a packet (data is copied internally). |
| `void` | [`emit`](#emit) `virtual` | Emit a flag-only packet carrying no payload data. |
| `void` | [`emit`](#emit) `virtual` | Emit an existing packet directly onto the outgoing signal. |
| `PacketSignal &` | [`getEmitter`](#getemitter)  | Returns a reference to the outgoing packet signal. |
| `PacketRetention` | [`retention`](#retention) `virtual` `const` `nodiscard` | Returns how this adapter treats incoming packet lifetime. Most adapters are synchronous and therefore only borrow the packet for the current call chain. Queue-style adapters override this to advertise that they clone before deferred use. Callers may treat the first adapter reporting Cloned or Retained as the explicit ownership boundary in the stream graph. |
| `void` | [`onStreamStateChange`](#onstreamstatechange) `virtual` `inline` | Called by the [PacketStream](#packetstream) to notify when the internal [Stream](#stream) state changes. On receiving the Stopped state, it is the responsibility of the adapter to have ceased all outgoing packet transmission, especially in multi-thread scenarios. |

---

{#packetstreamadapter}

#### PacketStreamAdapter

```cpp
PacketStreamAdapter(PacketSignal & emitter)
```

Defined in src/base/include/icy/packetstream.h:47

Construct the adapter, binding it to the given packet signal. 
#### Parameters
* `emitter` The outgoing packet signal owned by the subclass.

---

{#emit}

#### emit

`virtual`

```cpp
virtual void emit(char * data, size_t len, unsigned flags = 0)
```

Defined in src/base/include/icy/packetstream.h:54

Emit a mutable raw buffer as a packet. 
#### Parameters
* `data` Pointer to the buffer (not copied; caller retains ownership). 

* `len` Number of bytes in the buffer. 

* `flags` Optional packet flags (see [PacketFlags](#packetflags)).

---

{#emit}

#### emit

`virtual`

```cpp
virtual void emit(const char * data, size_t len, unsigned flags = 0)
```

Defined in src/base/include/icy/packetstream.h:60

Emit a read-only raw buffer as a packet (data is copied internally). 
#### Parameters
* `data` Pointer to the buffer. 

* `len` Number of bytes in the buffer. 

* `flags` Optional packet flags (see [PacketFlags](#packetflags)).

---

{#emit}

#### emit

`virtual`

```cpp
virtual void emit(const std::string & str, unsigned flags = 0)
```

Defined in src/base/include/icy/packetstream.h:65

Emit a string as a packet (data is copied internally). 
#### Parameters
* `str` String payload. 

* `flags` Optional packet flags (see [PacketFlags](#packetflags)).

---

{#emit}

#### emit

`virtual`

```cpp
virtual void emit(unsigned flags = 0)
```

Defined in src/base/include/icy/packetstream.h:69

Emit a flag-only packet carrying no payload data. 
#### Parameters
* `flags` Packet flags to embed in the emitted [FlagPacket](#flagpacket).

---

{#emit}

#### emit

`virtual`

```cpp
virtual void emit(IPacket & packet)
```

Defined in src/base/include/icy/packetstream.h:73

Emit an existing packet directly onto the outgoing signal. 
#### Parameters
* `packet` The packet to forward; must remain valid for the duration of the call.

---

{#getemitter}

#### getEmitter

```cpp
PacketSignal & getEmitter()
```

Defined in src/base/include/icy/packetstream.h:76

Returns a reference to the outgoing packet signal.

---

{#retention}

#### retention

`virtual` `const` `nodiscard`

```cpp
[[nodiscard]] virtual PacketRetention retention() const
```

Defined in src/base/include/icy/packetstream.h:84

Returns how this adapter treats incoming packet lifetime. Most adapters are synchronous and therefore only borrow the packet for the current call chain. Queue-style adapters override this to advertise that they clone before deferred use. Callers may treat the first adapter reporting Cloned or Retained as the explicit ownership boundary in the stream graph.

---

{#onstreamstatechange}

#### onStreamStateChange

`virtual` `inline`

```cpp
virtual inline void onStreamStateChange(const PacketStreamState &)
```

Defined in src/base/include/icy/packetstream.h:91

Called by the [PacketStream](#packetstream) to notify when the internal [Stream](#stream) state changes. On receiving the Stopped state, it is the responsibility of the adapter to have ceased all outgoing packet transmission, especially in multi-thread scenarios.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `PacketSignal &` | [`_emitter`](#_emitter)  |  |

---

{#_emitter}

#### _emitter

```cpp
PacketSignal & _emitter
```

Defined in src/base/include/icy/packetstream.h:100

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`PacketStreamAdapter`](#packetstreamadapter)  | NonCopyable and NonMovable. |
|  | [`PacketStreamAdapter`](#packetstreamadapter)  | Deleted constructor. |

---

{#packetstreamadapter}

#### PacketStreamAdapter

```cpp
PacketStreamAdapter(const PacketStreamAdapter &) = delete
```

Defined in src/base/include/icy/packetstream.h:95

NonCopyable and NonMovable.

---

{#packetstreamadapter}

#### PacketStreamAdapter

```cpp
PacketStreamAdapter(PacketStreamAdapter &&) = delete
```

Defined in src/base/include/icy/packetstream.h:97

Deleted constructor.

{#packetprocessor}

## PacketProcessor

```cpp
#include <icy/packetstream.h>
```

```cpp
class PacketProcessor
```

Defined in src/base/include/icy/packetstream.h:114

> **Inherits:** [`PacketStreamAdapter`](#packetstreamadapter)
> **Subclassed by:** [`AsyncPacketQueue< PacketT >`](#asyncpacketqueue), [`PlanarAudioPacket >`](#asyncpacketqueue), [`AsyncPacketQueue< VisionFramePacket >`](#asyncpacketqueue), [`AsyncPacketQueue< T >`](#asyncpacketqueue), [`StreamWriter`](#streamwriter), [`SyncPacketQueue< T >`](#syncpacketqueue), [`AudioPacketEncoder`](av.md#audiopacketencoder), [`FPSLimiter`](av.md#fpslimiter), [`MultiplexPacketEncoder`](av.md#multiplexpacketencoder), [`VideoPacketEncoder`](av.md#videopacketencoder), [`ChunkedAdapter`](http.md#chunkedadapter), [`MultipartAdapter`](http.md#multipartadapter), [`FrameNormalizer`](vision.md#framenormalizer), [`FrameSampler`](vision.md#framesampler), [`WebRtcTrackSender`](webrtc.md#webrtctracksender)

This class is a virtual interface for creating PacketStreamAdapters which process that and emit the [IPacket](#ipacket) type.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`PacketProcessor`](#packetprocessor) `inline` |  |
| `void` | [`process`](#process) `virtual` | This method performs processing on the given packet and emits the result. |
| `bool` | [`accepts`](#accepts) `virtual` `inline` | This method ensures compatibility with the given packet type. Return false to reject the packet. |
| `void` | [`operator<<`](#operator) `virtual` `inline` | [Stream](#stream) operator alias for [process()](#classicy_1_1PacketProcessor_1acd9fb1a5fa525c0a2d96971fe6c06294). |

---

{#packetprocessor}

#### PacketProcessor

`inline`

```cpp
inline PacketProcessor(PacketSignal & emitter)
```

Defined in src/base/include/icy/packetstream.h:117

---

{#process}

#### process

`virtual`

```cpp
virtual void process(IPacket & packet)
```

Defined in src/base/include/icy/packetstream.h:127

This method performs processing on the given packet and emits the result.

Processors that defer work asynchronously must either clone the packet or retain an owned equivalent. See [retention()](#classicy_1_1PacketStreamAdapter_1a37a1aa11e35a20a7c527efa78e99e928).

---

{#accepts}

#### accepts

`virtual` `inline`

```cpp
virtual inline bool accepts(IPacket *)
```

Defined in src/base/include/icy/packetstream.h:131

This method ensures compatibility with the given packet type. Return false to reject the packet.

---

{#operator}

#### operator<<

`virtual` `inline`

```cpp
virtual inline void operator<<(IPacket & packet)
```

Defined in src/base/include/icy/packetstream.h:134

[Stream](#stream) operator alias for [process()](#classicy_1_1PacketProcessor_1acd9fb1a5fa525c0a2d96971fe6c06294).

{#packetstream}

## PacketStream

```cpp
#include <icy/packetstream.h>
```

```cpp
class PacketStream
```

Defined in src/base/include/icy/packetstream.h:306

> **Inherits:** [`Stateful< PacketStreamState >`](#stateful)

Processes and broadcasts IPackets through a configurable adapter graph.

A [PacketStream](#packetstream) consists of one or many PacketSources, one or many PacketProcessors, and one or many delegate receivers.

This class enables the developer to setup a processor chain in order to perform arbitrary processing on data packets using interchangeable packet adapters, and pump the output to any delegate function or even another [PacketStream](#packetstream).

Note that [PacketStream](#packetstream) itself inherits from [PacketStreamAdapter](#packetstreamadapter), so a [PacketStream](#packetstream) can be the source of another [PacketStream](#packetstream).

All [PacketStream](#packetstream) methods are thread-safe, but once the stream is running you will not be able to attach or detach stream adapters.

In order to synchronize output packets with the application event loop take a look at the [SyncPacketQueue](#syncpacketqueue) class. For lengthy operations you can add an [AsyncPacketQueue](#asyncpacketqueue) to the start of the stream to defer processing from the [PacketSource](#packetsource) thread.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `PacketSignal` | [`emitter`](#emitter)  | Signals to delegates on outgoing packets. |
| `ThreadSignal< void(PacketStream &, const std::exception_ptr &)>` | [`Error`](#error)  | Signals that the [PacketStream](#packetstream) is in [Error](#error) state. If stream output is synchronized then the [Error](#error) signal will be sent from the synchronization context, otherwise it will be sent from the async processor context. See [synchronizeOutput()](#classicy_1_1PacketStream_1a246c1677eb9526056d99193e04d3be60) |
| `ThreadSignal< void(PacketStream &)>` | [`Close`](#close)  | Signals that the [PacketStream](#packetstream) is in Close state. This signal is sent immediately via the [close()](#classicy_1_1PacketStream_1a744ce42be8c86ada9cf8d8c85e080d37) method, and as such will be sent from the calling thread context. |

---

{#emitter}

#### emitter

```cpp
PacketSignal emitter
```

Defined in src/base/include/icy/packetstream.h:495

Signals to delegates on outgoing packets.

---

{#error}

#### Error

```cpp
ThreadSignal< void(PacketStream &, const std::exception_ptr &)> Error
```

Defined in src/base/include/icy/packetstream.h:501

Signals that the [PacketStream](#packetstream) is in [Error](#error) state. If stream output is synchronized then the [Error](#error) signal will be sent from the synchronization context, otherwise it will be sent from the async processor context. See [synchronizeOutput()](#classicy_1_1PacketStream_1a246c1677eb9526056d99193e04d3be60)

---

{#close}

#### Close

```cpp
ThreadSignal< void(PacketStream &)> Close
```

Defined in src/base/include/icy/packetstream.h:506

Signals that the [PacketStream](#packetstream) is in Close state. This signal is sent immediately via the [close()](#classicy_1_1PacketStream_1a744ce42be8c86ada9cf8d8c85e080d37) method, and as such will be sent from the calling thread context.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`PacketStream`](#packetstream)  | Construct a named packet stream. |
|  | [`~PacketStream`](#packetstream) `virtual` | Destroy the stream; calls [close()](#classicy_1_1PacketStream_1a744ce42be8c86ada9cf8d8c85e080d37) then [reset()](#classicy_1_1PacketStream_1a92bc04f29c224402edb681c55c648eb0) to release all adapters. |
|  | [`PacketStream`](#packetstream)  | Deleted constructor. |
|  | [`PacketStream`](#packetstream)  | Deleted constructor. |
| `void` | [`start`](#start) `virtual` | Start the stream and synchronized sources. |
| `void` | [`stop`](#stop) `virtual` | Stop the stream and synchronized sources. |
| `void` | [`pause`](#pause) `virtual` | Pause the stream. |
| `void` | [`resume`](#resume) `virtual` | Resume the stream. |
| `void` | [`close`](#close) `virtual` | Close the stream and transition the internal state to Closed. |
| `void` | [`reset`](#reset) `virtual` | Cleanup all managed stream adapters and reset the stream state. |
| `bool` | [`active`](#active) `virtual` `const` | Returns true when the stream is in the Active state. |
| `bool` | [`stopped`](#stopped) `virtual` `const` | Returns true when the stream is in the Stopping or Stopped state. |
| `bool` | [`closed`](#closed) `virtual` `const` | Returns true when the stream is in the Closed or [Error](#error) state. |
| `bool` | [`lock`](#lock) `virtual` | Sets the stream to locked state. In a locked state no new adapters can be added or removed from the stream until the stream is stopped. |
| `bool` | [`locked`](#locked) `virtual` `const` | Returns true is the stream is currently locked. |
| `void` | [`write`](#write) `virtual` | Write a mutable buffer into the stream without copying. The caller must keep the buffer alive until processing crosses a Cloned/Retained boundary or, if the graph is fully synchronous, until [write()](#classicy_1_1PacketStream_1a22f83c8eda142efab8d21ea04a88137a) returns. |
| `void` | [`write`](#write) `virtual` | Write a read-only buffer into the stream; data is copied immediately into an owning [RawPacket](#rawpacket) before any adapter sees it. |
| `void` | [`write`](#write) `virtual` | Write a packet directly into the processing chain. |
| `void` | [`attachSource`](#attachsource) `virtual` | Attach a bare packet signal as a stream source. The signal is wrapped in an unowned [PacketStreamAdapter](#packetstreamadapter) internally. Useful when the source is another [PacketStream::emitter](#classicy_1_1PacketStream_1adc148eb61c088baa5737dc299ecd9599). |
| `void` | [`attachSource`](#attachsource) `virtual` | Attach a [PacketStreamAdapter](#packetstreamadapter) as a source. Source adapters default to Borrowed retention unless overridden; they must not retain inbound packet storage asynchronously without reporting Cloned or Retained. |
| `void` | [`attachSource`](#attachsource) `inline` | Attach a shared_ptr-managed source adapter to the stream. The stream shares ownership; the adapter is kept alive at least until teardown. Throws std::runtime_error if `ptr` does not derive from [PacketStreamAdapter](#packetstreamadapter). |
| `bool` | [`detachSource`](#detachsource) `virtual` | Detach a source by its packet signal. Disconnects the signal from the stream's process slot and removes the adapter entry. |
| `bool` | [`detachSource`](#detachsource) `virtual` | Detach a source by its adapter pointer. Disconnects the adapter's emitter from the stream's process slot and removes the entry. |
| `void` | [`attach`](#attach) `virtual` | Attach a packet processor to the stream. Processors are executed in ascending order of their `order` value. Pass order = -1 to append at the end of the current processor list. Valid range is -1 to 101; values outside this range throw std::invalid_argument. Borrowed processors must finish with the packet before [process()](#classicy_1_1PacketStream_1ad5a93a94d2b8694b517438935c435397) returns. Queue/processors that defer work must report Cloned or Retained via retention() so upstream code has an explicit ownership boundary. |
| `void` | [`attach`](#attach) `inline` | Attach a shared_ptr-managed processor to the stream. The stream shares ownership; the processor is kept alive at least until teardown. Throws std::runtime_error if `ptr` does not derive from [PacketProcessor](#packetprocessor). |
| `bool` | [`detach`](#detach) `virtual` | Detach a packet processor from the stream. The processor's delegate connections are removed; ownership is released if held. |
| `void` | [`synchronizeOutput`](#synchronizeoutput) `virtual` | Synchronize stream output packets with a libuv event loop. Internally attaches a [SyncPacketQueue](#syncpacketqueue) at order 101 so that all packets emitted by the processor chain are dispatched from the loop thread rather than the source thread. Must be called before [start()](#classicy_1_1PacketStream_1ac6843f20698879acdaabd8110b440c55). |
| `void` | [`autoStart`](#autostart) `virtual` | Enable or disable auto-start behaviour (default: false). When enabled, the stream automatically transitions to Active state upon receiving the first packet while in the None or Locked state. Must be called before [start()](#classicy_1_1PacketStream_1ac6843f20698879acdaabd8110b440c55). |
| `void` | [`closeOnError`](#closeonerror) `virtual` | Enable or disable close-on-error behaviour (default: true). When enabled, an unhandled processor exception causes the stream to transition from [Error](#error) to Closed state automatically. |
| `const std::exception_ptr &` | [`error`](#error)  | Accessors for the unmanaged client data pointer. |
| `std::string` | [`name`](#name) `const` | Return the name assigned to this stream at construction. |
| `PacketAdapterVec` | [`adapters`](#adapters) `const` | Returns a combined list of all stream sources and processors. |
| `PacketAdapterVec` | [`sources`](#sources) `const` | Returns a list of all stream sources. |
| `PacketAdapterVec` | [`processors`](#processors) `const` | Returns a list of all stream processors. |
| `int` | [`numSources`](#numsources) `const` | Return the number of source adapters currently registered. |
| `int` | [`numProcessors`](#numprocessors) `const` | Return the number of processor adapters currently registered. |
| `int` | [`numAdapters`](#numadapters) `const` | Return the total number of adapters (sources + processors). |
| `AdapterT *` | [`getSource`](#getsource) `inline` | Return the nth source of type AdapterT, or nullptr if not found. Sources are searched in their registered order; only adapters that dynamic_cast successfully to AdapterT are counted. |
| `AdapterT *` | [`getProcessor`](#getprocessor) `inline` | Return the nth processor of type AdapterT, or nullptr if not found. Processors are searched in their registered order; only adapters that dynamic_cast successfully to AdapterT are counted. |
| `PacketProcessor *` | [`getProcessor`](#getprocessor) `inline` | Return the processor registered at a specific order value. Unlike the template overload, this searches by order rather than by type and index. |

---

{#packetstream}

#### PacketStream

```cpp
PacketStream(const std::string & name = "")
```

Defined in src/base/include/icy/packetstream.h:313

Construct a named packet stream. 
#### Parameters
* `name` Optional human-readable name used in log output.

---

{#packetstream}

#### ~PacketStream

`virtual`

```cpp
virtual ~PacketStream()
```

Defined in src/base/include/icy/packetstream.h:316

Destroy the stream; calls [close()](#classicy_1_1PacketStream_1a744ce42be8c86ada9cf8d8c85e080d37) then [reset()](#classicy_1_1PacketStream_1a92bc04f29c224402edb681c55c648eb0) to release all adapters.

---

{#packetstream}

#### PacketStream

```cpp
PacketStream(const PacketStream &) = delete
```

Defined in src/base/include/icy/packetstream.h:318

Deleted constructor.

---

{#packetstream}

#### PacketStream

```cpp
PacketStream(PacketStream &&) = delete
```

Defined in src/base/include/icy/packetstream.h:320

Deleted constructor.

---

{#start}

#### start

`virtual`

```cpp
virtual void start()
```

Defined in src/base/include/icy/packetstream.h:324

Start the stream and synchronized sources.

---

{#stop}

#### stop

`virtual`

```cpp
virtual void stop()
```

Defined in src/base/include/icy/packetstream.h:327

Stop the stream and synchronized sources.

---

{#pause}

#### pause

`virtual`

```cpp
virtual void pause()
```

Defined in src/base/include/icy/packetstream.h:330

Pause the stream.

---

{#resume}

#### resume

`virtual`

```cpp
virtual void resume()
```

Defined in src/base/include/icy/packetstream.h:333

Resume the stream.

---

{#close}

#### close

`virtual`

```cpp
virtual void close()
```

Defined in src/base/include/icy/packetstream.h:336

Close the stream and transition the internal state to Closed.

---

{#reset}

#### reset

`virtual`

```cpp
virtual void reset()
```

Defined in src/base/include/icy/packetstream.h:339

Cleanup all managed stream adapters and reset the stream state.

---

{#active}

#### active

`virtual` `const`

```cpp
virtual bool active() const
```

Defined in src/base/include/icy/packetstream.h:342

Returns true when the stream is in the Active state.

---

{#stopped}

#### stopped

`virtual` `const`

```cpp
virtual bool stopped() const
```

Defined in src/base/include/icy/packetstream.h:345

Returns true when the stream is in the Stopping or Stopped state.

---

{#closed}

#### closed

`virtual` `const`

```cpp
virtual bool closed() const
```

Defined in src/base/include/icy/packetstream.h:348

Returns true when the stream is in the Closed or [Error](#error) state.

---

{#lock}

#### lock

`virtual`

```cpp
virtual bool lock()
```

Defined in src/base/include/icy/packetstream.h:353

Sets the stream to locked state. In a locked state no new adapters can be added or removed from the stream until the stream is stopped.

---

{#locked}

#### locked

`virtual` `const`

```cpp
virtual bool locked() const
```

Defined in src/base/include/icy/packetstream.h:356

Returns true is the stream is currently locked.

---

{#write}

#### write

`virtual`

```cpp
virtual void write(char * data, size_t len)
```

Defined in src/base/include/icy/packetstream.h:364

Write a mutable buffer into the stream without copying. The caller must keep the buffer alive until processing crosses a Cloned/Retained boundary or, if the graph is fully synchronous, until [write()](#classicy_1_1PacketStream_1a22f83c8eda142efab8d21ea04a88137a) returns. 
#### Parameters
* `data` Pointer to the raw data buffer. 

* `len` Number of bytes to process.

---

{#write}

#### write

`virtual`

```cpp
virtual void write(const char * data, size_t len)
```

Defined in src/base/include/icy/packetstream.h:370

Write a read-only buffer into the stream; data is copied immediately into an owning [RawPacket](#rawpacket) before any adapter sees it. 
#### Parameters
* `data` Pointer to the raw data buffer. 

* `len` Number of bytes to process.

---

{#write}

#### write

`virtual`

```cpp
virtual void write(IPacket && packet)
```

Defined in src/base/include/icy/packetstream.h:374

Write a packet directly into the processing chain. 
#### Parameters
* `packet` Packet to process; moved into the stream.

---

{#attachsource}

#### attachSource

`virtual`

```cpp
virtual void attachSource(PacketSignal & source)
```

Defined in src/base/include/icy/packetstream.h:380

Attach a bare packet signal as a stream source. The signal is wrapped in an unowned [PacketStreamAdapter](#packetstreamadapter) internally. Useful when the source is another [PacketStream::emitter](#classicy_1_1PacketStream_1adc148eb61c088baa5737dc299ecd9599). 
#### Parameters
* `source` The packet signal to attach; must outlive the stream.

---

{#attachsource}

#### attachSource

`virtual`

```cpp
virtual void attachSource(PacketStreamAdapter * source, bool owned = true, bool syncState = false)
```

Defined in src/base/include/icy/packetstream.h:390

Attach a [PacketStreamAdapter](#packetstreamadapter) as a source. Source adapters default to Borrowed retention unless overridden; they must not retain inbound packet storage asynchronously without reporting Cloned or Retained. 
#### Parameters
* `source` The adapter to attach; must not be null. 

* `owned` If true the stream takes ownership and deletes the pointer on teardown. 

* `syncState` If true and `source` implements [basic::Startable](#startable), its [start()](#classicy_1_1PacketStream_1ac6843f20698879acdaabd8110b440c55)/stop() will be called by [startSources()](#classicy_1_1PacketStream_1a5e958f0d995288047462e89ade440084)/stopSources().

---

{#attachsource}

#### attachSource

`inline`

```cpp
template<class C> inline void attachSource(std::shared_ptr< C > ptr, bool syncState = false)
```

Defined in src/base/include/icy/packetstream.h:400

Attach a shared_ptr-managed source adapter to the stream. The stream shares ownership; the adapter is kept alive at least until teardown. Throws std::runtime_error if `ptr` does not derive from [PacketStreamAdapter](#packetstreamadapter). 
#### Parameters
* `C` Adapter type; must derive from [PacketStreamAdapter](#packetstreamadapter). 

#### Parameters
* `ptr` Shared pointer to the adapter instance. 

* `syncState` If true and `ptr` implements [basic::Startable](#startable), its [start()](#classicy_1_1PacketStream_1ac6843f20698879acdaabd8110b440c55)/stop() will be called by [startSources()](#classicy_1_1PacketStream_1a5e958f0d995288047462e89ade440084)/stopSources().

---

{#detachsource}

#### detachSource

`virtual`

```cpp
virtual bool detachSource(PacketSignal & source)
```

Defined in src/base/include/icy/packetstream.h:415

Detach a source by its packet signal. Disconnects the signal from the stream's process slot and removes the adapter entry. 
#### Parameters
* `source` The packet signal previously attached via [attachSource(PacketSignal&)](#classicy_1_1PacketStream_1ad11e84f0c24b4a75e53da886a3d7ca82). 

#### Returns
true if the source was found and removed, false otherwise.

---

{#detachsource}

#### detachSource

`virtual`

```cpp
virtual bool detachSource(PacketStreamAdapter * source)
```

Defined in src/base/include/icy/packetstream.h:421

Detach a source by its adapter pointer. Disconnects the adapter's emitter from the stream's process slot and removes the entry. 
#### Parameters
* `source` Pointer to the adapter previously attached. 

#### Returns
true if the source was found and removed, false otherwise.

---

{#attach}

#### attach

`virtual`

```cpp
virtual void attach(PacketProcessor * proc, int order = 0, bool owned = true)
```

Defined in src/base/include/icy/packetstream.h:433

Attach a packet processor to the stream. Processors are executed in ascending order of their `order` value. Pass order = -1 to append at the end of the current processor list. Valid range is -1 to 101; values outside this range throw std::invalid_argument. Borrowed processors must finish with the packet before [process()](#classicy_1_1PacketStream_1ad5a93a94d2b8694b517438935c435397) returns. Queue/processors that defer work must report Cloned or Retained via retention() so upstream code has an explicit ownership boundary. 
#### Parameters
* `proc` The processor to attach; must not be null. 

* `order` Position in the processing chain (lower runs first). 

* `owned` If true the stream takes ownership and deletes the pointer on teardown.

---

{#attach}

#### attach

`inline`

```cpp
template<class C> inline void attach(std::shared_ptr< C > ptr, int order = 0, bool syncState = false)
```

Defined in src/base/include/icy/packetstream.h:443

Attach a shared_ptr-managed processor to the stream. The stream shares ownership; the processor is kept alive at least until teardown. Throws std::runtime_error if `ptr` does not derive from [PacketProcessor](#packetprocessor). 
#### Parameters
* `C` Processor type; must derive from [PacketProcessor](#packetprocessor). 

#### Parameters
* `ptr` Shared pointer to the processor instance. 

* `order` Position in the processing chain (lower runs first). 

* `syncState` Reserved for future use; currently unused.

---

{#detach}

#### detach

`virtual`

```cpp
virtual bool detach(PacketProcessor * proc)
```

Defined in src/base/include/icy/packetstream.h:459

Detach a packet processor from the stream. The processor's delegate connections are removed; ownership is released if held. 
#### Parameters
* `proc` Pointer to the processor to remove. 

#### Returns
true if the processor was found and removed, false otherwise.

---

{#synchronizeoutput}

#### synchronizeOutput

`virtual`

```cpp
virtual void synchronizeOutput(uv::Loop * loop)
```

Defined in src/base/include/icy/packetstream.h:466

Synchronize stream output packets with a libuv event loop. Internally attaches a [SyncPacketQueue](#syncpacketqueue) at order 101 so that all packets emitted by the processor chain are dispatched from the loop thread rather than the source thread. Must be called before [start()](#classicy_1_1PacketStream_1ac6843f20698879acdaabd8110b440c55). 
#### Parameters
* `loop` The event loop to synchronize output onto; must not be null.

---

{#autostart}

#### autoStart

`virtual`

```cpp
virtual void autoStart(bool flag)
```

Defined in src/base/include/icy/packetstream.h:473

Enable or disable auto-start behaviour (default: false). When enabled, the stream automatically transitions to Active state upon receiving the first packet while in the None or Locked state. Must be called before [start()](#classicy_1_1PacketStream_1ac6843f20698879acdaabd8110b440c55). 
#### Parameters
* `flag` true to enable auto-start, false to disable.

---

{#closeonerror}

#### closeOnError

`virtual`

```cpp
virtual void closeOnError(bool flag)
```

Defined in src/base/include/icy/packetstream.h:479

Enable or disable close-on-error behaviour (default: true). When enabled, an unhandled processor exception causes the stream to transition from [Error](#error) to Closed state automatically. 
#### Parameters
* `flag` true to close the stream on error, false to remain in [Error](#error) state.

---

{#error}

#### error

```cpp
const std::exception_ptr & error()
```

Defined in src/base/include/icy/packetstream.h:488

Accessors for the unmanaged client data pointer.

Return the last captured exception, if the stream is in [Error](#error) state. The pointer is null when no error has occurred. 
#### Returns
A reference to the stored exception_ptr; empty if no error.

---

{#name}

#### name

`const`

```cpp
std::string name() const
```

Defined in src/base/include/icy/packetstream.h:492

Return the name assigned to this stream at construction. 
#### Returns
The stream name; empty string if none was provided.

---

{#adapters}

#### adapters

`const`

```cpp
PacketAdapterVec adapters() const
```

Defined in src/base/include/icy/packetstream.h:509

Returns a combined list of all stream sources and processors.

---

{#sources}

#### sources

`const`

```cpp
PacketAdapterVec sources() const
```

Defined in src/base/include/icy/packetstream.h:512

Returns a list of all stream sources.

---

{#processors}

#### processors

`const`

```cpp
PacketAdapterVec processors() const
```

Defined in src/base/include/icy/packetstream.h:515

Returns a list of all stream processors.

---

{#numsources}

#### numSources

`const`

```cpp
int numSources() const
```

Defined in src/base/include/icy/packetstream.h:519

Return the number of source adapters currently registered. 
#### Returns
Source count; thread-safe.

---

{#numprocessors}

#### numProcessors

`const`

```cpp
int numProcessors() const
```

Defined in src/base/include/icy/packetstream.h:523

Return the number of processor adapters currently registered. 
#### Returns
Processor count; thread-safe.

---

{#numadapters}

#### numAdapters

`const`

```cpp
int numAdapters() const
```

Defined in src/base/include/icy/packetstream.h:527

Return the total number of adapters (sources + processors). 
#### Returns
Combined adapter count; thread-safe.

---

{#getsource}

#### getSource

`inline`

```cpp
template<class AdapterT> inline AdapterT * getSource(int index = 0)
```

Defined in src/base/include/icy/packetstream.h:536

Return the nth source of type AdapterT, or nullptr if not found. Sources are searched in their registered order; only adapters that dynamic_cast successfully to AdapterT are counted. 
#### Parameters
* `AdapterT` Target type; must derive from [PacketStreamAdapter](#packetstreamadapter). 

#### Parameters
* `index` Zero-based index among matching sources (default 0). 

#### Returns
Pointer to the matching adapter, or nullptr.

---

{#getprocessor}

#### getProcessor

`inline`

```cpp
template<class AdapterT> inline AdapterT * getProcessor(int index = 0)
```

Defined in src/base/include/icy/packetstream.h:559

Return the nth processor of type AdapterT, or nullptr if not found. Processors are searched in their registered order; only adapters that dynamic_cast successfully to AdapterT are counted. 
#### Parameters
* `AdapterT` Target type; must derive from [PacketProcessor](#packetprocessor). 

#### Parameters
* `index` Zero-based index among matching processors (default 0). 

#### Returns
Pointer to the matching processor, or nullptr.

---

{#getprocessor}

#### getProcessor

`inline`

```cpp
inline PacketProcessor * getProcessor(int order = 0)
```

Defined in src/base/include/icy/packetstream.h:579

Return the processor registered at a specific order value. Unlike the template overload, this searches by order rather than by type and index. 
#### Parameters
* `order` The order value to match (default 0). 

#### Returns
Pointer to the matching processor, or nullptr if none registered at that order.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::mutex` | [`_mutex`](#_mutex)  |  |
| `std::mutex` | [`_procMutex`](#_procmutex)  |  |
| `std::string` | [`_name`](#_name)  |  |
| `PacketAdapterVec` | [`_sources`](#_sources)  |  |
| `PacketAdapterVec` | [`_processors`](#_processors)  |  |
| `std::deque< PacketStreamState >` | [`_states`](#_states)  |  |
| `std::exception_ptr` | [`_error`](#_error)  |  |
| `bool` | [`_autoStart`](#_autostart)  |  |
| `bool` | [`_closeOnError`](#_closeonerror)  |  |
| `bool` | [`_wired`](#_wired)  |  |

---

{#_mutex}

#### _mutex

```cpp
std::mutex _mutex
```

Defined in src/base/include/icy/packetstream.h:633

---

{#_procmutex}

#### _procMutex

```cpp
std::mutex _procMutex
```

Defined in src/base/include/icy/packetstream.h:634

---

{#_name}

#### _name

```cpp
std::string _name
```

Defined in src/base/include/icy/packetstream.h:635

---

{#_sources}

#### _sources

```cpp
PacketAdapterVec _sources
```

Defined in src/base/include/icy/packetstream.h:636

---

{#_processors}

#### _processors

```cpp
PacketAdapterVec _processors
```

Defined in src/base/include/icy/packetstream.h:637

---

{#_states}

#### _states

```cpp
std::deque< PacketStreamState > _states
```

Defined in src/base/include/icy/packetstream.h:638

---

{#_error}

#### _error

```cpp
std::exception_ptr _error
```

Defined in src/base/include/icy/packetstream.h:639

---

{#_autostart}

#### _autoStart

```cpp
bool _autoStart
```

Defined in src/base/include/icy/packetstream.h:640

---

{#_closeonerror}

#### _closeOnError

```cpp
bool _closeOnError
```

Defined in src/base/include/icy/packetstream.h:641

---

{#_wired}

#### _wired

```cpp
bool _wired
```

Defined in src/base/include/icy/packetstream.h:642

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`setup`](#setup)  | Attach the source and processor delegate chain. |
| `void` | [`teardown`](#teardown)  | Detach the source and processor delegate chain. |
| `void` | [`attachSource`](#attachsource)  |  |
| `void` | [`attach`](#attach)  |  |
| `void` | [`startSources`](#startsources)  | Start synchronized sources. |
| `void` | [`stopSources`](#stopsources)  | Stop synchronized sources. |
| `void` | [`process`](#process) `virtual` | [Process](#process) incoming packets. |
| `void` | [`emit`](#emit)  | Emit the final packet to listeners. |
| `void` | [`synchronizeStates`](#synchronizestates)  | Synchronize queued states with adapters. |
| `void` | [`onStateChange`](#onstatechange) `virtual` `override` | Override the [Stateful::onStateChange](#classicy_1_1Stateful_1a06e5572c313c3b54db932c922166079d) method. |
| `void` | [`assertCanModify`](#assertcanmodify)  | Returns true if the given state ID is queued. |
| `void` | [`handleException`](#handleexception)  | Handle an internal exception. |

---

{#setup}

#### setup

```cpp
void setup()
```

Defined in src/base/include/icy/packetstream.h:594

Attach the source and processor delegate chain.

---

{#teardown}

#### teardown

```cpp
void teardown()
```

Defined in src/base/include/icy/packetstream.h:597

Detach the source and processor delegate chain.

---

{#attachsource}

#### attachSource

```cpp
void attachSource(PacketAdapterReference::Ptr ref)
```

Defined in src/base/include/icy/packetstream.h:599

---

{#attach}

#### attach

```cpp
void attach(PacketAdapterReference::Ptr ref)
```

Defined in src/base/include/icy/packetstream.h:600

---

{#startsources}

#### startSources

```cpp
void startSources()
```

Defined in src/base/include/icy/packetstream.h:603

Start synchronized sources.

---

{#stopsources}

#### stopSources

```cpp
void stopSources()
```

Defined in src/base/include/icy/packetstream.h:606

Stop synchronized sources.

---

{#process}

#### process

`virtual`

```cpp
virtual void process(IPacket & packet)
```

Defined in src/base/include/icy/packetstream.h:609

[Process](#process) incoming packets.

---

{#emit}

#### emit

```cpp
void emit(IPacket & packet)
```

Defined in src/base/include/icy/packetstream.h:615

Emit the final packet to listeners.

Synchronized signals such as Close and [Error](#error) are sent from this method. See [synchronizeOutput()](#classicy_1_1PacketStream_1a246c1677eb9526056d99193e04d3be60)

---

{#synchronizestates}

#### synchronizeStates

```cpp
void synchronizeStates()
```

Defined in src/base/include/icy/packetstream.h:618

Synchronize queued states with adapters.

---

{#onstatechange}

#### onStateChange

`virtual` `override`

```cpp
virtual void onStateChange(PacketStreamState & state, const PacketStreamState & oldState) override
```

Defined in src/base/include/icy/packetstream.h:621

Override the [Stateful::onStateChange](#classicy_1_1Stateful_1a06e5572c313c3b54db932c922166079d) method.

---

{#assertcanmodify}

#### assertCanModify

```cpp
void assertCanModify()
```

Defined in src/base/include/icy/packetstream.h:628

Returns true if the given state ID is queued.

Asserts that the stream can be modified, ie is not in the Locked, Stopping or Active states.

---

{#handleexception}

#### handleException

```cpp
void handleException(std::exception & exc)
```

Defined in src/base/include/icy/packetstream.h:631

Handle an internal exception.

### Public Types

| Name | Description |
|------|-------------|
| [`Ptr`](#ptr)  |  |

---

{#ptr}

#### Ptr

```cpp
using Ptr = std::shared_ptr< PacketStream >
```

Defined in src/base/include/icy/packetstream.h:309

{#synchronizer}

## Synchronizer

```cpp
#include <icy/synchronizer.h>
```

```cpp
class Synchronizer
```

Defined in src/base/include/icy/synchronizer.h:38

> **Inherits:** [`Runner`](#runner)

[Synchronizer](#synchronizer) enables any thread to communicate with the associated event loop via synchronized callbacks.

This class inherits the `[Runner](#runner)` interface and may be used with any implementation that's powered by an asynchronous `[Runner](#runner)`.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Synchronizer`](#synchronizer)  | Creates a synchronizer attached to the given event loop without a callback. Call `[start()](#classicy_1_1Synchronizer_1a535a1605fc8902d814f9f5ab5e213a66)` separately to register the callback before using `[post()](#classicy_1_1Synchronizer_1a1400f8a4c294ba3d4a9d9d1887131abb)`. |
|  | [`Synchronizer`](#synchronizer)  | Creates and immediately starts a synchronizer with a single callback function. The target is invoked from the event loop context each time `[post()](#classicy_1_1Synchronizer_1a1400f8a4c294ba3d4a9d9d1887131abb)` is called. |
|  | [`Synchronizer`](#synchronizer) `inline` `explicit` | Creates and immediately starts a synchronizer with a variadic callback. |
|  | [`~Synchronizer`](#synchronizer) `virtual` | Destructor. |
| `void` | [`post`](#post)  | Send a synchronization request to the event loop. Call this each time you want the target method called synchronously. The synchronous method will be called on next iteration. This is not atomic, so do not expect a callback for every request. |
| `void` | [`start`](#start) `inline` | Starts the synchronizer with a variadic callback function. The callback is invoked from the event loop each time `[post()](#classicy_1_1Synchronizer_1a1400f8a4c294ba3d4a9d9d1887131abb)` is called. Throws `std::logic_error` if already running or if the handle is null. |
| `void` | [`start`](#start) `virtual` `override` | Starts the synchronizer with a `std::function` callback. Overrides `[Runner::start](#classicy_1_1Runner_1a5969e823f1ce0bdd8730d3108bf13fbd)`; delegates to the variadic `[start](#classicy_1_1Synchronizer_1a535a1605fc8902d814f9f5ab5e213a66)` template. |
| `void` | [`cancel`](#cancel) `virtual` | Cancels the synchronizer, signalling the associated callback to stop. A subsequent `[post()](#classicy_1_1Synchronizer_1a1400f8a4c294ba3d4a9d9d1887131abb)` is needed to wake up the event loop so it can process the cancellation. |
| `void` | [`close`](#close) `virtual` | Cancels the synchronizer and closes the underlying `uv_async_t` handle. Safe to call multiple times; no-op if already closed. |
| `uv::Handle< uv_async_t > &` | [`handle`](#handle)  | Returns a reference to the underlying libuv async handle. |

---

{#synchronizer}

#### Synchronizer

```cpp
Synchronizer(uv::Loop * loop)
```

Defined in src/base/include/icy/synchronizer.h:46

Creates a synchronizer attached to the given event loop without a callback. Call `[start()](#classicy_1_1Synchronizer_1a535a1605fc8902d814f9f5ab5e213a66)` separately to register the callback before using `[post()](#classicy_1_1Synchronizer_1a1400f8a4c294ba3d4a9d9d1887131abb)`. 
#### Parameters
* `loop` Event loop to attach the async handle to.

---

{#synchronizer}

#### Synchronizer

```cpp
Synchronizer(std::function< void()> target, uv::Loop * loop = uv::defaultLoop())
```

Defined in src/base/include/icy/synchronizer.h:52

Creates and immediately starts a synchronizer with a single callback function. The target is invoked from the event loop context each time `[post()](#classicy_1_1Synchronizer_1a1400f8a4c294ba3d4a9d9d1887131abb)` is called. 
#### Parameters
* `target` Callback to invoke on each event loop wakeup. 

* `loop` Event loop to attach the async handle to.

---

{#synchronizer}

#### Synchronizer

`inline` `explicit`

```cpp
template<typename Function, typename... Args> inline explicit Synchronizer(Function && func, Args &&... args, uv::Loop * loop = uv::defaultLoop())
```

Defined in src/base/include/icy/synchronizer.h:61

Creates and immediately starts a synchronizer with a variadic callback. 
#### Parameters
* `Function` Callable type. 

* `Args` Argument types forwarded to the function. 

#### Parameters
* `func` Callable to invoke on each event loop wakeup. 

* `args` Arguments forwarded to `func`. 

* `loop` Event loop to attach the async handle to.

---

{#synchronizer}

#### ~Synchronizer

`virtual`

```cpp
virtual ~Synchronizer()
```

Defined in src/base/include/icy/synchronizer.h:69

Destructor.

---

{#post}

#### post

```cpp
void post()
```

Defined in src/base/include/icy/synchronizer.h:75

Send a synchronization request to the event loop. Call this each time you want the target method called synchronously. The synchronous method will be called on next iteration. This is not atomic, so do not expect a callback for every request.

---

{#start}

#### start

`inline`

```cpp
template<typename Function, typename... Args> inline void start(Function && func, Args &&... args)
```

Defined in src/base/include/icy/synchronizer.h:85

Starts the synchronizer with a variadic callback function. The callback is invoked from the event loop each time `[post()](#classicy_1_1Synchronizer_1a1400f8a4c294ba3d4a9d9d1887131abb)` is called. Throws `std::logic_error` if already running or if the handle is null. 
#### Parameters
* `Function` Callable type. 

* `Args` Argument types forwarded to the function. 

#### Parameters
* `func` Callable to invoke on each event loop wakeup. 

* `args` Arguments forwarded to `func`.

---

{#start}

#### start

`virtual` `override`

```cpp
virtual void start(std::function< void()> func) override
```

Defined in src/base/include/icy/synchronizer.h:131

Starts the synchronizer with a `std::function` callback. Overrides `[Runner::start](#classicy_1_1Runner_1a5969e823f1ce0bdd8730d3108bf13fbd)`; delegates to the variadic `[start](#classicy_1_1Synchronizer_1a535a1605fc8902d814f9f5ab5e213a66)` template. 
#### Parameters
* `func` Callback invoked from the event loop on each `[post()](#classicy_1_1Synchronizer_1a1400f8a4c294ba3d4a9d9d1887131abb)` call.

---

{#cancel}

#### cancel

`virtual`

```cpp
virtual void cancel()
```

Defined in src/base/include/icy/synchronizer.h:135

Cancels the synchronizer, signalling the associated callback to stop. A subsequent `[post()](#classicy_1_1Synchronizer_1a1400f8a4c294ba3d4a9d9d1887131abb)` is needed to wake up the event loop so it can process the cancellation.

---

{#close}

#### close

`virtual`

```cpp
virtual void close()
```

Defined in src/base/include/icy/synchronizer.h:139

Cancels the synchronizer and closes the underlying `uv_async_t` handle. Safe to call multiple times; no-op if already closed.

---

{#handle}

#### handle

```cpp
uv::Handle< uv_async_t > & handle()
```

Defined in src/base/include/icy/synchronizer.h:143

Returns a reference to the underlying libuv async handle. 
#### Returns
Reference to the `[uv::Handle](uv.md#handle-2)<uv_async_t>`.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `uv::Handle< uv_async_t >` | [`_handle`](#_handle)  |  |

---

{#_handle}

#### _handle

```cpp
uv::Handle< uv_async_t > _handle
```

Defined in src/base/include/icy/synchronizer.h:148

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `bool` | [`async`](#async) `virtual` `const` `override` | Returns true if the implementation is thread-based. |

---

{#async}

#### async

`virtual` `const` `override`

```cpp
virtual bool async() const override
```

Defined in src/base/include/icy/synchronizer.h:146

Returns true if the implementation is thread-based. 
#### Returns
True for thread-backed runners, false for event-loop-driven runners.

{#timedmanager}

## TimedManager

```cpp
#include <icy/timedmanager.h>
```

```cpp
template<class TKey, class TValue>
class TimedManager
```

Defined in src/base/include/icy/timedmanager.h:29

> **Inherits:** [`KeyedStore< TKey, TValue >`](#keyedstore)

Timed pointer manager

Provides timed persistent data storage for class instances. TValue must implement the clone() method.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`TimedManager`](#timedmanager) `inline` | Constructs a [TimedManager](#timedmanager) and starts the internal expiry-check timer. |
| `void` | [`add`](#add) `inline` | Add an item which will expire (and be deleted) after the specified timeout value. If the timeout is 0 the item will be stored indefinitely. The [TimedManager](#timedmanager) assumes ownership of the given pointer. |
| `bool` | [`expires`](#expires) `virtual` `inline` | Update the item expiry timeout. |
| `bool` | [`expires`](#expires) `virtual` `inline` | Update the item expiry timeout. |
| `void` | [`clear`](#clear) `virtual` `inline` `override` | Removes all items and their associated timeouts. |

---

{#timedmanager}

#### TimedManager

`inline`

```cpp
inline TimedManager(uv::Loop * loop = uv::defaultLoop())
```

Defined in src/base/include/icy/timedmanager.h:37

Constructs a [TimedManager](#timedmanager) and starts the internal expiry-check timer. 
#### Parameters
* `loop` Event loop used by the internal timer (default: process-wide default loop).

---

{#add}

#### add

`inline`

```cpp
inline void add(const TKey & key, std::unique_ptr< TValue > item, long timeout = 0)
```

Defined in src/base/include/icy/timedmanager.h:51

Add an item which will expire (and be deleted) after the specified timeout value. If the timeout is 0 the item will be stored indefinitely. The [TimedManager](#timedmanager) assumes ownership of the given pointer.

---

{#expires}

#### expires

`virtual` `inline`

```cpp
virtual inline bool expires(const TKey & key, long timeout)
```

Defined in src/base/include/icy/timedmanager.h:60

Update the item expiry timeout.

---

{#expires}

#### expires

`virtual` `inline`

```cpp
virtual inline bool expires(TValue * item, long timeout)
```

Defined in src/base/include/icy/timedmanager.h:67

Update the item expiry timeout.

---

{#clear}

#### clear

`virtual` `inline` `override`

```cpp
virtual inline void clear() override
```

Defined in src/base/include/icy/timedmanager.h:74

Removes all items and their associated timeouts.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::mutex` | [`_tmutex`](#_tmutex)  |  |
| `TimeoutMap` | [`_timeouts`](#_timeouts)  |  |
| `Timer` | [`_timer`](#_timer)  |  |

---

{#_tmutex}

#### _tmutex

```cpp
std::mutex _tmutex
```

Defined in src/base/include/icy/timedmanager.h:153

---

{#_timeouts}

#### _timeouts

```cpp
TimeoutMap _timeouts
```

Defined in src/base/include/icy/timedmanager.h:154

---

{#_timer}

#### _timer

```cpp
Timer _timer
```

Defined in src/base/include/icy/timedmanager.h:155

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `bool` | [`setTimeout`](#settimeout) `virtual` `inline` | Sets or removes the expiry timeout for a specific item pointer. If timeout > 0, starts a countdown; if timeout == 0, removes any existing timeout. |
| `void` | [`onRemove`](#onremove) `virtual` `inline` `override` | Called when an item is removed from the collection. Erases the item's timeout entry and calls the base implementation. |
| `void` | [`onTimeout`](#ontimeout) `virtual` `inline` | Called when an item's timeout expires. Default implementation removes and deletes the item. |
| `void` | [`onTimerUpdate`](#ontimerupdate) `inline` | Internal timer callback; iterates all tracked timeouts and calls [onTimeout()](#classicy_1_1TimedManager_1a81f3927772f9e446aa04be7837215180) for any that have expired. |

---

{#settimeout}

#### setTimeout

`virtual` `inline`

```cpp
virtual inline bool setTimeout(TValue * item, long timeout)
```

Defined in src/base/include/icy/timedmanager.h:88

Sets or removes the expiry timeout for a specific item pointer. If timeout > 0, starts a countdown; if timeout == 0, removes any existing timeout. 
#### Parameters
* `item` Pointer to the managed item. 

* `timeout` Delay in milliseconds, or 0 to clear the timeout. 

#### Returns
true on success. 

#### Exceptions
* `std::logic_error` if item is nullptr.

---

{#onremove}

#### onRemove

`virtual` `inline` `override`

```cpp
virtual inline void onRemove(const TKey & key, TValue * item) override
```

Defined in src/base/include/icy/timedmanager.h:113

Called when an item is removed from the collection. Erases the item's timeout entry and calls the base implementation. 
#### Parameters
* `key` Key of the removed item. 

* `item` Pointer to the removed item.

---

{#ontimeout}

#### onTimeout

`virtual` `inline`

```cpp
virtual inline void onTimeout(TValue * item)
```

Defined in src/base/include/icy/timedmanager.h:126

Called when an item's timeout expires. Default implementation removes and deletes the item. 
#### Parameters
* `item` Pointer to the expired item.

---

{#ontimerupdate}

#### onTimerUpdate

`inline`

```cpp
inline void onTimerUpdate()
```

Defined in src/base/include/icy/timedmanager.h:139

Internal timer callback; iterates all tracked timeouts and calls [onTimeout()](#classicy_1_1TimedManager_1a81f3927772f9e446aa04be7837215180) for any that have expired.

### Public Types

| Name | Description |
|------|-------------|
| [`Base`](#base)  |  |
| [`TimeoutMap`](#timeoutmap)  |  |

---

{#base}

#### Base

```cpp
using Base = KeyedStore< TKey, TValue >
```

Defined in src/base/include/icy/timedmanager.h:32

---

{#timeoutmap}

#### TimeoutMap

```cpp
using TimeoutMap = std::map< TValue *, Timeout >
```

Defined in src/base/include/icy/timedmanager.h:33

{#ipacketcreationstrategy}

## IPacketCreationStrategy

```cpp
#include <icy/packetfactory.h>
```

```cpp
class IPacketCreationStrategy
```

Defined in src/base/include/icy/packetfactory.h:29

> **Subclassed by:** [`PacketCreationStrategy< PacketT >`](#packetcreationstrategy)

Abstract strategy for creating typed packets from raw buffer data.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`IPacketCreationStrategy`](#ipacketcreationstrategy)  | Defaulted constructor. |
| `IPacket *` | [`create`](#create) `virtual` `const` | Attempts to create a typed packet from the given buffer. |
| `int` | [`priority`](#priority) `virtual` `const` | Returns the dispatch priority of this strategy (0–100; higher runs first). |

---

{#ipacketcreationstrategy}

#### IPacketCreationStrategy

```cpp
IPacketCreationStrategy() = default
```

Defined in src/base/include/icy/packetfactory.h:32

Defaulted constructor.

---

{#create}

#### create

`virtual` `const`

```cpp
virtual IPacket * create(const ConstBuffer & buffer, size_t & nread) const
```

Defined in src/base/include/icy/packetfactory.h:38

Attempts to create a typed packet from the given buffer. 
#### Parameters
* `buffer` Raw input data. 

* `nread` Set to the number of bytes consumed on success, 0 otherwise. 

#### Returns
Newly allocated packet on success, nullptr if the buffer does not match.

---

{#priority}

#### priority

`virtual` `const`

```cpp
virtual int priority() const
```

Defined in src/base/include/icy/packetfactory.h:41

Returns the dispatch priority of this strategy (0–100; higher runs first).

{#packetfactory}

## PacketFactory

```cpp
#include <icy/packetfactory.h>
```

```cpp
class PacketFactory
```

Defined in src/base/include/icy/packetfactory.h:95

Priority-ordered factory that creates typed packets from raw buffers using registered strategies.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`PacketFactory`](#packetfactory)  | Defaulted constructor. |
|  | [`PacketFactory`](#packetfactory)  | Deleted constructor. |
|  | [`PacketFactory`](#packetfactory)  | Defaulted constructor. |
| `void` | [`registerPacketType`](#registerpackettype) `inline` | Registers a `[PacketCreationStrategy](#packetcreationstrategy)<PacketT>` at the given priority. Any previously registered strategy for `PacketT` is replaced. |
| `void` | [`unregisterPacketType`](#unregisterpackettype) `inline` | Removes the `[PacketCreationStrategy](#packetcreationstrategy)<PacketT>` from the factory, if present. |
| `void` | [`registerStrategy`](#registerstrategy) `inline` | Registers an arbitrary `[IPacketCreationStrategy](#ipacketcreationstrategy)` subclass at the given priority. Any previously registered instance of the same type is replaced. |
| `void` | [`unregisterStrategy`](#unregisterstrategy) `inline` | Removes the `StrategyT` instance from the factory, if present. |
| `PacketCreationStrategyList &` | [`types`](#types) `inline` |  |
| `const PacketCreationStrategyList &` | [`types`](#types) `const` `inline` |  |
| `bool` | [`onPacketCreated`](#onpacketcreated) `virtual` `inline` | Called after a packet is successfully created by a strategy. Override to apply filtering; return false to reject the packet (it will be deleted). |
| `IPacket *` | [`createPacket`](#createpacket) `virtual` `inline` | Iterates registered strategies in priority order and returns the first successfully created packet. |

---

{#packetfactory}

#### PacketFactory

```cpp
PacketFactory() = default
```

Defined in src/base/include/icy/packetfactory.h:98

Defaulted constructor.

---

{#packetfactory}

#### PacketFactory

```cpp
PacketFactory(const PacketFactory &) = delete
```

Defined in src/base/include/icy/packetfactory.h:99

Deleted constructor.

---

{#packetfactory}

#### PacketFactory

```cpp
PacketFactory(PacketFactory &&) = default
```

Defined in src/base/include/icy/packetfactory.h:101

Defaulted constructor.

---

{#registerpackettype}

#### registerPacketType

`inline`

```cpp
template<class PacketT> inline void registerPacketType(int priority)
```

Defined in src/base/include/icy/packetfactory.h:110

Registers a `[PacketCreationStrategy](#packetcreationstrategy)<PacketT>` at the given priority. Any previously registered strategy for `PacketT` is replaced. 
#### Parameters
* `PacketT` Packet type to register; must default-constructible with a `read()` method. 

#### Parameters
* `priority` Dispatch priority (0–100; higher runs first).

---

{#unregisterpackettype}

#### unregisterPacketType

`inline`

```cpp
template<class PacketT> inline void unregisterPacketType()
```

Defined in src/base/include/icy/packetfactory.h:120

Removes the `[PacketCreationStrategy](#packetcreationstrategy)<PacketT>` from the factory, if present. 
#### Parameters
* `PacketT` Packet type whose strategy should be removed.

---

{#registerstrategy}

#### registerStrategy

`inline`

```cpp
template<class StrategyT> inline void registerStrategy(int priority)
```

Defined in src/base/include/icy/packetfactory.h:134

Registers an arbitrary `[IPacketCreationStrategy](#ipacketcreationstrategy)` subclass at the given priority. Any previously registered instance of the same type is replaced. 
#### Parameters
* `StrategyT` Strategy type to instantiate; must accept a priority int in its constructor. 

#### Parameters
* `priority` Dispatch priority (0–100; higher runs first).

---

{#unregisterstrategy}

#### unregisterStrategy

`inline`

```cpp
template<class StrategyT> inline void unregisterStrategy()
```

Defined in src/base/include/icy/packetfactory.h:144

Removes the `StrategyT` instance from the factory, if present. 
#### Parameters
* `StrategyT` Strategy type to remove.

---

{#types}

#### types

`inline`

```cpp
inline PacketCreationStrategyList & types()
```

Defined in src/base/include/icy/packetfactory.h:154

#### Returns
Mutable reference to the ordered list of registered strategies.

---

{#types}

#### types

`const` `inline`

```cpp
inline const PacketCreationStrategyList & types() const
```

Defined in src/base/include/icy/packetfactory.h:160

#### Returns
Const reference to the ordered list of registered strategies.

---

{#onpacketcreated}

#### onPacketCreated

`virtual` `inline`

```cpp
virtual inline bool onPacketCreated(IPacket * packet)
```

Defined in src/base/include/icy/packetfactory.h:169

Called after a packet is successfully created by a strategy. Override to apply filtering; return false to reject the packet (it will be deleted). 
#### Parameters
* `packet` Newly created packet. 

#### Returns
True to accept the packet, false to discard it and try the next strategy.

---

{#createpacket}

#### createPacket

`virtual` `inline`

```cpp
virtual inline IPacket * createPacket(const ConstBuffer & buffer, size_t & nread)
```

Defined in src/base/include/icy/packetfactory.h:181

Iterates registered strategies in priority order and returns the first successfully created packet. 
#### Parameters
* `buffer` Raw input data. 

* `nread` Set to the number of bytes consumed on success. 

#### Returns
Newly allocated packet, or nullptr if no strategy matched. 

#### Exceptions
* `std::logic_error` if no packet types have been registered.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `PacketCreationStrategyList` | [`_types`](#_types)  |  |

---

{#_types}

#### _types

```cpp
PacketCreationStrategyList _types
```

Defined in src/base/include/icy/packetfactory.h:200

### Private Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`sortTypes`](#sorttypes) `inline` |  |

---

{#sorttypes}

#### sortTypes

`inline`

```cpp
inline void sortTypes()
```

Defined in src/base/include/icy/packetfactory.h:203

{#streammanager}

## StreamManager

```cpp
#include <icy/streammanager.h>
```

```cpp
class StreamManager
```

Defined in src/base/include/icy/streammanager.h:23

> **Inherits:** [`string, PacketStream >`](#keyedstore)

Manages a named collection of [PacketStream](#packetstream) instances with lifecycle callbacks.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`StreamManager`](#streammanager)  |  |
| `bool` | [`addStream`](#addstream)  |  |
| `bool` | [`addStream`](#addstream)  |  |
| `bool` | [`closeStream`](#closestream)  |  |
| `void` | [`closeAll`](#closeall)  |  |
| `PacketStream *` | [`getStream`](#getstream) `const` |  |
| `PacketStream *` | [`getDefaultStream`](#getdefaultstream) `const` |  |
| `void` | [`print`](#print) `const` |  |

---

{#streammanager}

#### StreamManager

```cpp
StreamManager()
```

Defined in src/base/include/icy/streammanager.h:26

---

{#addstream}

#### addStream

```cpp
bool addStream(PacketStream * stream, bool whiny = true)
```

Defined in src/base/include/icy/streammanager.h:29

---

{#addstream}

#### addStream

```cpp
bool addStream(std::unique_ptr< PacketStream > stream, bool whiny = true)
```

Defined in src/base/include/icy/streammanager.h:30

---

{#closestream}

#### closeStream

```cpp
bool closeStream(const std::string & name, bool whiny = true)
```

Defined in src/base/include/icy/streammanager.h:31

---

{#closeall}

#### closeAll

```cpp
void closeAll()
```

Defined in src/base/include/icy/streammanager.h:32

---

{#getstream}

#### getStream

`const`

```cpp
PacketStream * getStream(const std::string & name, bool whiny = true) const
```

Defined in src/base/include/icy/streammanager.h:34

---

{#getdefaultstream}

#### getDefaultStream

`const`

```cpp
PacketStream * getDefaultStream() const
```

Defined in src/base/include/icy/streammanager.h:35

---

{#print}

#### print

`const`

```cpp
void print(std::ostream & os) const
```

Defined in src/base/include/icy/streammanager.h:37

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`onAdd`](#onadd) `virtual` `override` | Override for lifecycle reactions. |
| `void` | [`onRemove`](#onremove) `virtual` `override` |  |
| `void` | [`onStreamStateChange`](#onstreamstatechange)  |  |
| `const char *` | [`className`](#classname) `virtual` `const` `inline` |  |

---

{#onadd}

#### onAdd

`virtual` `override`

```cpp
virtual void onAdd(const std::string &, PacketStream * stream) override
```

Defined in src/base/include/icy/streammanager.h:40

Override for lifecycle reactions.

---

{#onremove}

#### onRemove

`virtual` `override`

```cpp
virtual void onRemove(const std::string &, PacketStream * stream) override
```

Defined in src/base/include/icy/streammanager.h:41

---

{#onstreamstatechange}

#### onStreamStateChange

```cpp
void onStreamStateChange(void * sender, PacketStreamState & state, const PacketStreamState &)
```

Defined in src/base/include/icy/streammanager.h:43

---

{#classname}

#### className

`virtual` `const` `inline`

```cpp
virtual inline const char * className() const
```

Defined in src/base/include/icy/streammanager.h:45

{#idiagnostic}

## IDiagnostic

```cpp
#include <icy/diagnosticmanager.h>
```

```cpp
class IDiagnostic
```

Defined in src/base/include/icy/diagnosticmanager.h:64

> **Inherits:** [`Stateful< DiagnosticState >`](#stateful)
> **Subclassed by:** [`AsyncDiagnostic`](#asyncdiagnostic)

Abstract interface for diagnostic information providers.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`name`](#name)  | The name of the diagnostic. |
| `std::string` | [`description`](#description)  | The diagnostic description. |
| `std::vector< std::string >` | [`summary`](#summary)  | The diagnostic summary, maybe including troubleshooting information on failure. |
| `ThreadSignal< void(const std::string &)>` | [`SummaryUpdated`](#summaryupdated)  | Signals when a new text item is added to the summary. |

---

{#name}

#### name

```cpp
std::string name
```

Defined in src/base/include/icy/diagnosticmanager.h:70

The name of the diagnostic.

---

{#description}

#### description

```cpp
std::string description
```

Defined in src/base/include/icy/diagnosticmanager.h:71

The diagnostic description.

---

{#summary}

#### summary

```cpp
std::vector< std::string > summary
```

Defined in src/base/include/icy/diagnosticmanager.h:72

The diagnostic summary, maybe including troubleshooting information on failure.

---

{#summaryupdated}

#### SummaryUpdated

```cpp
ThreadSignal< void(const std::string &)> SummaryUpdated
```

Defined in src/base/include/icy/diagnosticmanager.h:95

Signals when a new text item is added to the summary.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`IDiagnostic`](#idiagnostic)  |  |
| `void` | [`check`](#check) `virtual` | Resets state to None and invokes [run()](#classicy_1_1IDiagnostic_1a052a9fcd71565081be080e9c285b7497) to perform the diagnostic check. |
| `void` | [`reset`](#reset) `virtual` | Clears the summary and resets state to None. |
| `bool` | [`complete`](#complete) `virtual` `const` | Returns true if the diagnostic has reached a terminal state (Passed or Failed). |
| `bool` | [`passed`](#passed) `virtual` `const` | Returns true if the diagnostic state is Passed. |
| `bool` | [`failed`](#failed) `virtual` `const` | Returns true if the diagnostic state is Failed. |

---

{#idiagnostic}

#### IDiagnostic

```cpp
IDiagnostic()
```

Defined in src/base/include/icy/diagnosticmanager.h:67

---

{#check}

#### check

`virtual`

```cpp
virtual void check()
```

Defined in src/base/include/icy/diagnosticmanager.h:76

Resets state to None and invokes [run()](#classicy_1_1IDiagnostic_1a052a9fcd71565081be080e9c285b7497) to perform the diagnostic check.

---

{#reset}

#### reset

`virtual`

```cpp
virtual void reset()
```

Defined in src/base/include/icy/diagnosticmanager.h:79

Clears the summary and resets state to None.

---

{#complete}

#### complete

`virtual` `const`

```cpp
virtual bool complete() const
```

Defined in src/base/include/icy/diagnosticmanager.h:83

Returns true if the diagnostic has reached a terminal state (Passed or Failed). 
#### Returns
true if complete.

---

{#passed}

#### passed

`virtual` `const`

```cpp
virtual bool passed() const
```

Defined in src/base/include/icy/diagnosticmanager.h:87

Returns true if the diagnostic state is Passed. 
#### Returns
true if passed.

---

{#failed}

#### failed

`virtual` `const`

```cpp
virtual bool failed() const
```

Defined in src/base/include/icy/diagnosticmanager.h:91

Returns true if the diagnostic state is Failed. 
#### Returns
true if failed.

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`run`](#run) `virtual` | Override to implement diagnostic logic. |
| `bool` | [`pass`](#pass) `virtual` | Transitions the state to Passed. |
| `bool` | [`fail`](#fail) `virtual` | Transitions the state to Failed. |
| `void` | [`addSummary`](#addsummary) `virtual` | Appends text to the summary list and emits SummaryUpdated. |

---

{#run}

#### run

`virtual`

```cpp
virtual void run()
```

Defined in src/base/include/icy/diagnosticmanager.h:102

Override to implement diagnostic logic.

The StateChange signal will dispatch diagnostic test results to delegates.

---

{#pass}

#### pass

`virtual`

```cpp
virtual bool pass()
```

Defined in src/base/include/icy/diagnosticmanager.h:106

Transitions the state to Passed. 
#### Returns
true if the state transition succeeded.

---

{#fail}

#### fail

`virtual`

```cpp
virtual bool fail()
```

Defined in src/base/include/icy/diagnosticmanager.h:110

Transitions the state to Failed. 
#### Returns
true if the state transition succeeded.

---

{#addsummary}

#### addSummary

`virtual`

```cpp
virtual void addSummary(const std::string & text)
```

Defined in src/base/include/icy/diagnosticmanager.h:114

Appends text to the summary list and emits SummaryUpdated. 
#### Parameters
* `text` Summary line to append.

{#asyncdiagnostic}

## AsyncDiagnostic

```cpp
#include <icy/diagnosticmanager.h>
```

```cpp
class AsyncDiagnostic
```

Defined in src/base/include/icy/diagnosticmanager.h:124

> **Inherits:** [`IDiagnostic`](#idiagnostic), [`Runnable`](#runnable)

Asynchronous diagnostic information collector.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`run`](#run) `virtual` `override` | Override to implement diagnostic logic. |
| `void` | [`check`](#check) `virtual` `inline` `override` | Resets the diagnostic and launches [run()](#classicy_1_1AsyncDiagnostic_1aff8234cc3095de10aea49dbb9cceb663) on a background thread. |

---

{#run}

#### run

`virtual` `override`

```cpp
virtual void run() override
```

Defined in src/base/include/icy/diagnosticmanager.h:130

Override to implement diagnostic logic.

The StateChange signal will dispatch diagnostic test results to delegates.

---

{#check}

#### check

`virtual` `inline` `override`

```cpp
virtual inline void check() override
```

Defined in src/base/include/icy/diagnosticmanager.h:133

Resets the diagnostic and launches [run()](#classicy_1_1AsyncDiagnostic_1aff8234cc3095de10aea49dbb9cceb663) on a background thread.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `Thread` | [`_thread`](#_thread)  |  |

---

{#_thread}

#### _thread

```cpp
Thread _thread
```

Defined in src/base/include/icy/diagnosticmanager.h:140

{#diagnosticmanager}

## DiagnosticManager

```cpp
#include <icy/diagnosticmanager.h>
```

```cpp
class DiagnosticManager
```

Defined in src/base/include/icy/diagnosticmanager.h:150

> **Inherits:** [`string, IDiagnostic >`](#keyedstore)

Registry and manager for diagnostic providers.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `NullSignal` | [`DiagnosticsComplete`](#diagnosticscomplete)  |  |

---

{#diagnosticscomplete}

#### DiagnosticsComplete

```cpp
NullSignal DiagnosticsComplete
```

Defined in src/base/include/icy/diagnosticmanager.h:169

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`DiagnosticManager`](#diagnosticmanager)  |  |
| `bool` | [`addDiagnostic`](#adddiagnostic)  | Adds a diagnostic, taking ownership. |
| `bool` | [`freeDiagnostic`](#freediagnostic)  | Removes and deletes the diagnostic registered under name. |
| `IDiagnostic *` | [`getDiagnostic`](#getdiagnostic) `const` | Returns the diagnostic or nullptr. |
| `void` | [`resetAll`](#resetall)  |  |
| `void` | [`checkAll`](#checkall)  |  |
| `bool` | [`allComplete`](#allcomplete) `const` |  |
| `void` | [`onDiagnosticStateChange`](#ondiagnosticstatechange)  |  |

---

{#diagnosticmanager}

#### DiagnosticManager

```cpp
DiagnosticManager()
```

Defined in src/base/include/icy/diagnosticmanager.h:153

---

{#adddiagnostic}

#### addDiagnostic

```cpp
bool addDiagnostic(std::unique_ptr< IDiagnostic > test)
```

Defined in src/base/include/icy/diagnosticmanager.h:157

Adds a diagnostic, taking ownership.

---

{#freediagnostic}

#### freeDiagnostic

```cpp
bool freeDiagnostic(const std::string & name)
```

Defined in src/base/include/icy/diagnosticmanager.h:160

Removes and deletes the diagnostic registered under name.

---

{#getdiagnostic}

#### getDiagnostic

`const`

```cpp
IDiagnostic * getDiagnostic(const std::string & name) const
```

Defined in src/base/include/icy/diagnosticmanager.h:163

Returns the diagnostic or nullptr.

---

{#resetall}

#### resetAll

```cpp
void resetAll()
```

Defined in src/base/include/icy/diagnosticmanager.h:165

---

{#checkall}

#### checkAll

```cpp
void checkAll()
```

Defined in src/base/include/icy/diagnosticmanager.h:166

---

{#allcomplete}

#### allComplete

`const`

```cpp
bool allComplete() const
```

Defined in src/base/include/icy/diagnosticmanager.h:167

---

{#ondiagnosticstatechange}

#### onDiagnosticStateChange

```cpp
void onDiagnosticStateChange(void *, DiagnosticState & state, const DiagnosticState &)
```

Defined in src/base/include/icy/diagnosticmanager.h:171

{#packettransaction}

## PacketTransaction

```cpp
#include <icy/packettransaction.h>
```

```cpp
template<class PacketT>
class PacketTransaction
```

Defined in src/base/include/icy/packettransaction.h:70

> **Inherits:** [`Sendable`](#sendable), [`Stateful< TransactionState >`](#stateful), [`RefCounted< PacketTransaction< PacketT > >`](#refcounted)
> **Subclassed by:** [`Transaction< PacketT >`](net.md#transaction)

Request/response transaction with timeout and retry logic.

Lifetime is managed by [IntrusivePtr](#intrusiveptr) via the [RefCounted](#refcounted) base class. The transaction is kept alive as long as at least one [IntrusivePtr](#intrusiveptr) references it. The owning client holds a [Ptr](#classicy_1_1PacketTransaction_1a44ea6d9d47efb59eebdc71158c0158c5) in its transaction list; callbacks should capture a [Ptr](#classicy_1_1PacketTransaction_1a44ea6d9d47efb59eebdc71158c0158c5) copy to prevent premature deletion.

When a terminal state (Success or Failed) is reached, the transaction cleans up its timer but does NOT delete itself. The [IntrusivePtr](#intrusiveptr) destructor handles deletion when the last reference is released.

### Friends

| Name | Description |
|------|-------------|
| [`IntrusivePtr`](#intrusiveptr)  |  |

---

{#intrusiveptr}

#### IntrusivePtr

```cpp
template<typename U> friend class IntrusivePtr
```

Defined in src/base/include/icy/packettransaction.h:165

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`PacketTransaction`](#packettransaction) `inline` |  |
|  | [`PacketTransaction`](#packettransaction) `inline` |  |
| `bool` | [`send`](#send) `virtual` `inline` `override` | Starts the transaction timer and sends the request. Overriding classes should implement send logic here. |
| `void` | [`cancel`](#cancel) `virtual` `inline` `override` | Cancellation means that the agent will not retransmit the request, will not treat the lack of response to be a failure, but will wait the duration of the transaction timeout for a response. Transitions the transaction to the `Cancelled` state. |
| `bool` | [`cancelled`](#cancelled) `const` `inline` |  |
| `void` | [`dispose`](#dispose) `virtual` `inline` | Stops the timer and unregisters callbacks. Does NOT delete the object; the [IntrusivePtr](#intrusiveptr) destructor handles deletion when the last reference is released. Safe to call multiple times. |
| `bool` | [`disposed`](#disposed) `const` `inline` `nodiscard` |  |
| `bool` | [`canResend`](#canresend) `virtual` `inline` |  |
| `int` | [`attempts`](#attempts) `const` `inline` |  |
| `int` | [`retries`](#retries) `const` `inline` |  |
| `PacketT &` | [`request`](#request) `inline` |  |
| `PacketT` | [`request`](#request) `const` `inline` |  |
| `PacketT &` | [`response`](#response) `inline` |  |
| `PacketT` | [`response`](#response) `const` `inline` |  |

---

{#packettransaction}

#### PacketTransaction

`inline`

```cpp
inline PacketTransaction(long timeout = 10000, int retries = 0, uv::Loop * loop = uv::defaultLoop())
```

Defined in src/base/include/icy/packettransaction.h:80

#### Parameters
* `timeout` Transaction timeout in milliseconds before failure or retry. 

* `retries` Maximum number of retransmissions (0 means one attempt only). 

* `loop` Event loop for the timeout timer.

---

{#packettransaction}

#### PacketTransaction

`inline`

```cpp
inline PacketTransaction(const PacketT & request, long timeout = 10000, int retries = 0, uv::Loop * loop = uv::defaultLoop())
```

Defined in src/base/include/icy/packettransaction.h:92

#### Parameters
* `request` Initial request packet to store and send. 

* `timeout` Transaction timeout in milliseconds before failure or retry. 

* `retries` Maximum number of retransmissions (0 means one attempt only). 

* `loop` Event loop for the timeout timer.

---

{#send}

#### send

`virtual` `inline` `override`

```cpp
virtual inline bool send() override
```

Defined in src/base/include/icy/packettransaction.h:103

Starts the transaction timer and sends the request. Overriding classes should implement send logic here.

---

{#cancel}

#### cancel

`virtual` `inline` `override`

```cpp
virtual inline void cancel() override
```

Defined in src/base/include/icy/packettransaction.h:122

Cancellation means that the agent will not retransmit the request, will not treat the lack of response to be a failure, but will wait the duration of the transaction timeout for a response. Transitions the transaction to the `Cancelled` state.

---

{#cancelled}

#### cancelled

`const` `inline`

```cpp
inline bool cancelled() const
```

Defined in src/base/include/icy/packettransaction.h:125

#### Returns
True if the transaction is in the `Cancelled` state.

---

{#dispose}

#### dispose

`virtual` `inline`

```cpp
virtual inline void dispose()
```

Defined in src/base/include/icy/packettransaction.h:130

Stops the timer and unregisters callbacks. Does NOT delete the object; the [IntrusivePtr](#intrusiveptr) destructor handles deletion when the last reference is released. Safe to call multiple times.

---

{#disposed}

#### disposed

`const` `inline` `nodiscard`

```cpp
[[nodiscard]] inline bool disposed() const
```

Defined in src/base/include/icy/packettransaction.h:141

#### Returns
True if [dispose()](#classicy_1_1PacketTransaction_1a59df328cafacdcea30f86ea38c4718ec) has been called.

---

{#canresend}

#### canResend

`virtual` `inline`

```cpp
virtual inline bool canResend()
```

Defined in src/base/include/icy/packettransaction.h:144

#### Returns
True if the transaction is not cancelled and has attempts remaining.

---

{#attempts}

#### attempts

`const` `inline`

```cpp
inline int attempts() const
```

Defined in src/base/include/icy/packettransaction.h:147

#### Returns
The number of times `[send()](#classicy_1_1PacketTransaction_1acf7a9bbcfff95999c0c227cb36f9cfd4)` has been called for this transaction.

---

{#retries}

#### retries

`const` `inline`

```cpp
inline int retries() const
```

Defined in src/base/include/icy/packettransaction.h:150

#### Returns
The configured maximum number of retransmissions.

---

{#request}

#### request

`inline`

```cpp
inline PacketT & request()
```

Defined in src/base/include/icy/packettransaction.h:153

#### Returns
Mutable reference to the outgoing request packet.

---

{#request}

#### request

`const` `inline`

```cpp
inline PacketT request() const
```

Defined in src/base/include/icy/packettransaction.h:156

#### Returns
Copy of the outgoing request packet.

---

{#response}

#### response

`inline`

```cpp
inline PacketT & response()
```

Defined in src/base/include/icy/packettransaction.h:159

#### Returns
Mutable reference to the received response packet.

---

{#response}

#### response

`const` `inline`

```cpp
inline PacketT response() const
```

Defined in src/base/include/icy/packettransaction.h:162

#### Returns
Copy of the received response packet.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `PacketT` | [`_request`](#_request)  |  |
| `PacketT` | [`_response`](#_response)  |  |
| `Timer` | [`_timer`](#_timer)  | The request timeout callback. |
| `int` | [`_retries`](#_retries)  | The maximum number of attempts before the transaction is considered failed. |
| `int` | [`_attempts`](#_attempts)  | The number of times the transaction has been sent. |
| `bool` | [`_disposed`](#_disposed)  |  |

---

{#_request}

#### _request

```cpp
PacketT _request
```

Defined in src/base/include/icy/packettransaction.h:219

---

{#_response}

#### _response

```cpp
PacketT _response
```

Defined in src/base/include/icy/packettransaction.h:220

---

{#_timer}

#### _timer

```cpp
Timer _timer
```

Defined in src/base/include/icy/packettransaction.h:221

The request timeout callback.

---

{#_retries}

#### _retries

```cpp
int _retries
```

Defined in src/base/include/icy/packettransaction.h:222

The maximum number of attempts before the transaction is considered failed.

---

{#_attempts}

#### _attempts

```cpp
int _attempts
```

Defined in src/base/include/icy/packettransaction.h:223

The number of times the transaction has been sent.

---

{#_disposed}

#### _disposed

```cpp
bool _disposed = false
```

Defined in src/base/include/icy/packettransaction.h:224

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`onStateChange`](#onstatechange) `virtual` `inline` `override` | Post state change hook. Calls [dispose()](#classicy_1_1PacketTransaction_1a59df328cafacdcea30f86ea38c4718ec) on terminal states to stop the timer, but does not delete the object; [IntrusivePtr](#intrusiveptr) handles that. |
| `bool` | [`handlePotentialResponse`](#handlepotentialresponse) `virtual` `inline` | Processes a potential response candidate and updates the state accordingly. |
| `bool` | [`checkResponse`](#checkresponse) `virtual` | Checks a potential response candidate and returns true on successful match. |
| `void` | [`onResponse`](#onresponse) `virtual` `inline` | Called when a successful response is received. |
| `void` | [`onTimeout`](#ontimeout) `virtual` `inline` | Called by the timer when the transaction timeout elapses. Retransmits if retries remain, otherwise transitions to `Failed`. |

---

{#onstatechange}

#### onStateChange

`virtual` `inline` `override`

```cpp
virtual inline void onStateChange(TransactionState & state, const TransactionState &) override
```

Defined in src/base/include/icy/packettransaction.h:174

Post state change hook. Calls [dispose()](#classicy_1_1PacketTransaction_1a59df328cafacdcea30f86ea38c4718ec) on terminal states to stop the timer, but does not delete the object; [IntrusivePtr](#intrusiveptr) handles that.

---

{#handlepotentialresponse}

#### handlePotentialResponse

`virtual` `inline`

```cpp
virtual inline bool handlePotentialResponse(const PacketT & packet)
```

Defined in src/base/include/icy/packettransaction.h:185

Processes a potential response candidate and updates the state accordingly.

---

{#checkresponse}

#### checkResponse

`virtual`

```cpp
virtual bool checkResponse(const PacketT & packet)
```

Defined in src/base/include/icy/packettransaction.h:198

Checks a potential response candidate and returns true on successful match.

---

{#onresponse}

#### onResponse

`virtual` `inline`

```cpp
virtual inline void onResponse()
```

Defined in src/base/include/icy/packettransaction.h:201

Called when a successful response is received.

---

{#ontimeout}

#### onTimeout

`virtual` `inline`

```cpp
virtual inline void onTimeout()
```

Defined in src/base/include/icy/packettransaction.h:208

Called by the timer when the transaction timeout elapses. Retransmits if retries remain, otherwise transitions to `Failed`.

### Public Types

| Name | Description |
|------|-------------|
| [`Ptr`](#ptr)  |  |

---

{#ptr}

#### Ptr

```cpp
using Ptr = IntrusivePtr< PacketTransaction< PacketT > >
```

Defined in src/base/include/icy/packettransaction.h:75

{#task}

## Task

```cpp
#include <icy/task.h>
```

```cpp
class Task
```

Defined in src/base/include/icy/task.h:32

> **Inherits:** [`Runnable`](#runnable)
> **Subclassed by:** [`Task`](sched.md#task-1)

Abstract base class for implementing asynchronous tasks.

Tasks are designed to be run by a [TaskRunner](#taskrunner).

### Friends

| Name | Description |
|------|-------------|
| [`TaskRunner`](#taskrunner)  | Tasks belong to a [TaskRunner](#taskrunner) instance. |

---

{#taskrunner}

#### TaskRunner

```cpp
friend class TaskRunner
```

Defined in src/base/include/icy/task.h:77

Tasks belong to a [TaskRunner](#taskrunner) instance.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Task`](#task)  |  |
| `void` | [`destroy`](#destroy) `virtual` | Sets the task to destroyed state. |
| `bool` | [`destroyed`](#destroyed) `virtual` `const` | Signals that the task should be disposed of. |
| `bool` | [`repeating`](#repeating) `virtual` `const` | Signals that the task should be called repeatedly by the [TaskRunner](#taskrunner). If this returns false the task will be cancelled. |
| `uint32_t` | [`id`](#id) `virtual` `const` | Unique task ID. |
|  | [`Task`](#task)  | Deleted constructor. |
|  | [`Task`](#task)  | Deleted constructor. |

---

{#task}

#### Task

```cpp
Task(bool repeat = false)
```

Defined in src/base/include/icy/task.h:37

#### Parameters
* `repeat` If true, the `[TaskRunner](#taskrunner)` will call `[run()](#classicy_1_1Task_1a27c42c43ba0ce7ea66f3210fdd27f3de)` repeatedly; if false, the task is cancelled after one execution.

---

{#destroy}

#### destroy

`virtual`

```cpp
virtual void destroy()
```

Defined in src/base/include/icy/task.h:40

Sets the task to destroyed state.

---

{#destroyed}

#### destroyed

`virtual` `const`

```cpp
virtual bool destroyed() const
```

Defined in src/base/include/icy/task.h:43

Signals that the task should be disposed of.

---

{#repeating}

#### repeating

`virtual` `const`

```cpp
virtual bool repeating() const
```

Defined in src/base/include/icy/task.h:48

Signals that the task should be called repeatedly by the [TaskRunner](#taskrunner). If this returns false the task will be cancelled.

---

{#id}

#### id

`virtual` `const`

```cpp
virtual uint32_t id() const
```

Defined in src/base/include/icy/task.h:51

Unique task ID.

---

{#task}

#### Task

```cpp
Task(const Task & task) = delete
```

Defined in src/base/include/icy/task.h:61

Deleted constructor.

---

{#task}

#### Task

```cpp
Task(Task &&) = delete
```

Defined in src/base/include/icy/task.h:63

Deleted constructor.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `uint32_t` | [`_id`](#_id)  |  |
| `bool` | [`_repeating`](#_repeating)  |  |
| `bool` | [`_destroyed`](#_destroyed)  |  |

---

{#_id}

#### _id

```cpp
uint32_t _id
```

Defined in src/base/include/icy/task.h:79

---

{#_repeating}

#### _repeating

```cpp
bool _repeating
```

Defined in src/base/include/icy/task.h:80

---

{#_destroyed}

#### _destroyed

```cpp
bool _destroyed
```

Defined in src/base/include/icy/task.h:81

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`run`](#run) `virtual` `override` | Called by the [TaskRunner](#taskrunner) to run the task. Override this method to implement task action. Returning true means the task should be called again, and false will cause the task to be destroyed. The task will similarly be destroyed if [destroy()](#classicy_1_1Task_1a639abe7ed51fd9294d6210e1d8264814) was called during the current task iteration. |

---

{#run}

#### run

`virtual` `override`

```cpp
virtual void run() override
```

Defined in src/base/include/icy/task.h:74

Called by the [TaskRunner](#taskrunner) to run the task. Override this method to implement task action. Returning true means the task should be called again, and false will cause the task to be destroyed. The task will similarly be destroyed if [destroy()](#classicy_1_1Task_1a639abe7ed51fd9294d6210e1d8264814) was called during the current task iteration.

{#taskrunner}

## TaskRunner

```cpp
#include <icy/task.h>
```

```cpp
class TaskRunner
```

Defined in src/base/include/icy/task.h:93

> **Inherits:** [`Runnable`](#runnable)
> **Subclassed by:** [`Scheduler`](sched.md#scheduler)

[Runner](#runner) for tasks that inherit the `[Task](#task)` interface.

The `[TaskRunner](#taskrunner)` continually loops through each task in the task list calling the task's `[run()](#classicy_1_1TaskRunner_1a99cef8fc7be815ddd9c151dd4d6985d2)` method.

The `[TaskRunner](#taskrunner)` is powered by an abstract `[Runner](#runner)` instance, which means that tasks can be executed in a thread or event loop context.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `NullSignal` | [`Idle`](#idle)  | Fires after completing an iteration of all tasks. |
| `NullSignal` | [`Shutdown`](#shutdown)  | Signals when the `[TaskRunner](#taskrunner)` is shutting down. |

---

{#idle}

#### Idle

```cpp
NullSignal Idle
```

Defined in src/base/include/icy/task.h:138

Fires after completing an iteration of all tasks.

---

{#shutdown}

#### Shutdown

```cpp
NullSignal Shutdown
```

Defined in src/base/include/icy/task.h:141

Signals when the `[TaskRunner](#taskrunner)` is shutting down.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`TaskRunner`](#taskrunner)  |  |
|  | [`TaskRunner`](#taskrunner)  | Deleted constructor. |
|  | [`TaskRunner`](#taskrunner)  | Deleted constructor. |
| `bool` | [`start`](#start) `virtual` | Starts a task, adding it if it doesn't exist. |
| `bool` | [`cancel`](#cancel) `virtual` | Cancels a task. |
| `bool` | [`destroy`](#destroy) `virtual` | Queues a task for destruction. |
| `bool` | [`exists`](#exists) `virtual` `const` | Returns whether a task exists. |
| `Task *` | [`get`](#get) `virtual` `const` | Returns the task pointer matching the given ID, or nullptr if no task exists. |
| `void` | [`setRunner`](#setrunner) `virtual` | Set the asynchronous context for packet processing. This may be a [Thread](#thread) or another derivative of Async. Must be set before the stream is activated. |
| `const char *` | [`className`](#classname) `virtual` `const` `inline` |  |
| `void` | [`cancel`](#cancel) `virtual` `inline` | Cancel the current task. The [run()](#classicy_1_1TaskRunner_1a99cef8fc7be815ddd9c151dd4d6985d2) method should return ASAP. |

---

{#taskrunner}

#### TaskRunner

```cpp
TaskRunner(std::shared_ptr< Runner > runner = nullptr)
```

Defined in src/base/include/icy/task.h:97

#### Parameters
* `runner` Async runner to drive task execution; defaults to a new `[Thread](#thread)`.

---

{#taskrunner}

#### TaskRunner

```cpp
TaskRunner(const TaskRunner &) = delete
```

Defined in src/base/include/icy/task.h:100

Deleted constructor.

---

{#taskrunner}

#### TaskRunner

```cpp
TaskRunner(TaskRunner &&) = delete
```

Defined in src/base/include/icy/task.h:102

Deleted constructor.

---

{#start}

#### start

`virtual`

```cpp
virtual bool start(Task * task)
```

Defined in src/base/include/icy/task.h:106

Starts a task, adding it if it doesn't exist.

---

{#cancel}

#### cancel

`virtual`

```cpp
virtual bool cancel(Task * task)
```

Defined in src/base/include/icy/task.h:114

Cancels a task.

The task reference will be managed by the [TaskRunner](#taskrunner) until the task is destroyed.

---

{#destroy}

#### destroy

`virtual`

```cpp
virtual bool destroy(Task * task)
```

Defined in src/base/include/icy/task.h:117

Queues a task for destruction.

---

{#exists}

#### exists

`virtual` `const`

```cpp
virtual bool exists(Task * task) const
```

Defined in src/base/include/icy/task.h:120

Returns whether a task exists.

---

{#get}

#### get

`virtual` `const`

```cpp
virtual Task * get(uint32_t id) const
```

Defined in src/base/include/icy/task.h:124

Returns the task pointer matching the given ID, or nullptr if no task exists.

---

{#setrunner}

#### setRunner

`virtual`

```cpp
virtual void setRunner(std::shared_ptr< Runner > runner)
```

Defined in src/base/include/icy/task.h:129

Set the asynchronous context for packet processing. This may be a [Thread](#thread) or another derivative of Async. Must be set before the stream is activated.

---

{#classname}

#### className

`virtual` `const` `inline`

```cpp
virtual inline const char * className() const
```

Defined in src/base/include/icy/task.h:143

---

{#cancel}

#### cancel

`virtual` `inline`

```cpp
virtual inline void cancel(bool flag = true)
```

Defined in src/base/include/icy/task.h:108

Cancel the current task. The [run()](#classicy_1_1TaskRunner_1a99cef8fc7be815ddd9c151dd4d6985d2) method should return ASAP.

### Public Static Methods

| Return | Name | Description |
|--------|------|-------------|
| `TaskRunner &` | [`getDefault`](#getdefault) `static` | Returns the default `[TaskRunner](#taskrunner)` singleton, although [TaskRunner](#taskrunner) instances may be initialized individually. The default runner should be kept for short running tasks such as timers in order to maintain performance. |

---

{#getdefault}

#### getDefault

`static`

```cpp
static TaskRunner & getDefault()
```

Defined in src/base/include/icy/task.h:135

Returns the default `[TaskRunner](#taskrunner)` singleton, although [TaskRunner](#taskrunner) instances may be initialized individually. The default runner should be kept for short running tasks such as timers in order to maintain performance.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::mutex` | [`_mutex`](#_mutex)  |  |
| `std::shared_ptr< Runner >` | [`_runner`](#_runner)  |  |
| `TaskList` | [`_tasks`](#_tasks)  |  |

---

{#_mutex}

#### _mutex

```cpp
std::mutex _mutex
```

Defined in src/base/include/icy/task.h:179

---

{#_runner}

#### _runner

```cpp
std::shared_ptr< Runner > _runner
```

Defined in src/base/include/icy/task.h:180

---

{#_tasks}

#### _tasks

```cpp
TaskList _tasks
```

Defined in src/base/include/icy/task.h:181

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`run`](#run) `virtual` `override` | Called by the async context to run the next task. |
| `bool` | [`add`](#add) `virtual` | Adds a task to the runner. |
| `bool` | [`remove`](#remove) `virtual` | Removes a task from the runner. |
| `Task *` | [`next`](#next) `virtual` `const` | Returns the next task to be run. |
| `void` | [`clear`](#clear) `virtual` | Destroys and clears all manages tasks. |
| `void` | [`onAdd`](#onadd) `virtual` | Called after a task is added. |
| `void` | [`onStart`](#onstart) `virtual` | Called after a task is started. |
| `void` | [`onCancel`](#oncancel) `virtual` | Called after a task is cancelled. |
| `void` | [`onRemove`](#onremove) `virtual` | Called after a task is removed. |
| `void` | [`onRun`](#onrun) `virtual` | Called after a task has run. |

---

{#run}

#### run

`virtual` `override`

```cpp
virtual void run() override
```

Defined in src/base/include/icy/task.h:147

Called by the async context to run the next task.

---

{#add}

#### add

`virtual`

```cpp
virtual bool add(Task * task)
```

Defined in src/base/include/icy/task.h:150

Adds a task to the runner.

---

{#remove}

#### remove

`virtual`

```cpp
virtual bool remove(Task * task)
```

Defined in src/base/include/icy/task.h:153

Removes a task from the runner.

---

{#next}

#### next

`virtual` `const`

```cpp
virtual Task * next() const
```

Defined in src/base/include/icy/task.h:156

Returns the next task to be run.

---

{#clear}

#### clear

`virtual`

```cpp
virtual void clear()
```

Defined in src/base/include/icy/task.h:159

Destroys and clears all manages tasks.

---

{#onadd}

#### onAdd

`virtual`

```cpp
virtual void onAdd(Task * task)
```

Defined in src/base/include/icy/task.h:162

Called after a task is added.

---

{#onstart}

#### onStart

`virtual`

```cpp
virtual void onStart(Task * task)
```

Defined in src/base/include/icy/task.h:165

Called after a task is started.

---

{#oncancel}

#### onCancel

`virtual`

```cpp
virtual void onCancel(Task * task)
```

Defined in src/base/include/icy/task.h:168

Called after a task is cancelled.

---

{#onremove}

#### onRemove

`virtual`

```cpp
virtual void onRemove(Task * task)
```

Defined in src/base/include/icy/task.h:171

Called after a task is removed.

---

{#onrun}

#### onRun

`virtual`

```cpp
virtual void onRun(Task * task)
```

Defined in src/base/include/icy/task.h:174

Called after a task has run.

{#ipacket}

## IPacket

```cpp
#include <icy/packet.h>
```

```cpp
class IPacket
```

Defined in src/base/include/icy/packet.h:47

> **Subclassed by:** [`FlagPacket`](#flagpacket), [`RawPacket`](#rawpacket), [`Message`](symple.md#message-10), [`Message`](stun.md#message-5)

The basic packet type which is passed around the icey system. [IPacket](#ipacket) can be extended for each protocol to enable polymorphic processing and callbacks using [PacketStream](#packetstream) and friends.

### Friends

| Name | Description |
|------|-------------|
| [`operator<<`](#operator) `inline` | [Stream](#stream) insertion operator; delegates to [print()](#classicy_1_1IPacket_1acc8685ec343b3e9ed81687b8e8fa1acc). |

---

{#operator}

#### operator<<

`inline`

```cpp
friend inline std::ostream & operator<<(std::ostream & stream, const IPacket & p)
```

Defined in src/base/include/icy/packet.h:137

[Stream](#stream) insertion operator; delegates to [print()](#classicy_1_1IPacket_1acc8685ec343b3e9ed81687b8e8fa1acc). 
#### Parameters
* `stream` Output stream. 

* `p` Packet to print. 

#### Returns
Reference to the stream.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::any` | [`opaque`](#opaque)  | Optional type-safe context data. Use std::any_cast to retrieve. Lifetime of the stored value is tied to the packet's lifetime. |
| `std::unique_ptr< IPacketInfo >` | [`info`](#info)  | Optional extra information about the packet. |
| `Bitwise` | [`flags`](#flags)  | Provides basic information about the packet. |

---

{#opaque}

#### opaque

```cpp
std::any opaque
```

Defined in src/base/include/icy/packet.h:86

Optional type-safe context data. Use std::any_cast to retrieve. Lifetime of the stored value is tied to the packet's lifetime.

---

{#info}

#### info

```cpp
std::unique_ptr< IPacketInfo > info
```

Defined in src/base/include/icy/packet.h:89

Optional extra information about the packet.

---

{#flags}

#### flags

```cpp
Bitwise flags
```

Defined in src/base/include/icy/packet.h:92

Provides basic information about the packet.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`IPacket`](#ipacket) `inline` |  |
|  | [`IPacket`](#ipacket) `inline` | Copy constructor; clones the info object if present. |
| `IPacket &` | [`operator=`](#operator) `inline` | Copy assignment; clones the info object if present. |
| `std::unique_ptr< IPacket >` | [`clone`](#clone) `virtual` `const` | Returns a heap-allocated deep copy of this packet. |
| `ssize_t` | [`read`](#read) `virtual` | Read/parse to the packet from the given input buffer. The number of bytes read is returned. |
| `void` | [`write`](#write) `virtual` `const` | Copy/generate to the packet given output buffer. The number of bytes written can be obtained from the buffer. |
| `size_t` | [`size`](#size) `virtual` `const` `inline` | The size of the packet in bytes. |
| `bool` | [`hasData`](#hasdata) `virtual` `const` `inline` | Returns true if the packet has a non-null data pointer. |
| `char *` | [`data`](#data) `virtual` `const` `inline` | The packet data pointer for buffered packets. |
| `const void *` | [`constData`](#constdata) `virtual` `const` `inline` | The const packet data pointer for buffered packets. |
| `const char *` | [`className`](#classname) `virtual` `const` | Returns the class name of this packet type for logging and diagnostics. |
| `void` | [`print`](#print) `virtual` `const` `inline` | Prints a human-readable representation to the given stream. |

---

{#ipacket}

#### IPacket

`inline`

```cpp
inline IPacket(std::unique_ptr< IPacketInfo > info = nullptr, unsigned flags = 0)
```

Defined in src/base/include/icy/packet.h:52

#### Parameters
* `info` Optional packet info; ownership transferred. 

* `flags` Initial bitwise flags.

---

{#ipacket}

#### IPacket

`inline`

```cpp
inline IPacket(const IPacket & r)
```

Defined in src/base/include/icy/packet.h:60

Copy constructor; clones the info object if present. 
#### Parameters
* `r` Source packet.

---

{#operator}

#### operator=

`inline`

```cpp
inline IPacket & operator=(const IPacket & r)
```

Defined in src/base/include/icy/packet.h:70

Copy assignment; clones the info object if present. 
#### Parameters
* `r` Source packet. 

#### Returns
Reference to this packet.

---

{#clone}

#### clone

`virtual` `const`

```cpp
virtual std::unique_ptr< IPacket > clone() const
```

Defined in src/base/include/icy/packet.h:80

Returns a heap-allocated deep copy of this packet. 
#### Returns
Owning pointer to the cloned packet.

---

{#read}

#### read

`virtual`

```cpp
virtual ssize_t read(const ConstBuffer &)
```

Defined in src/base/include/icy/packet.h:96

Read/parse to the packet from the given input buffer. The number of bytes read is returned.

---

{#write}

#### write

`virtual` `const`

```cpp
virtual void write(Buffer &) const
```

Defined in src/base/include/icy/packet.h:105

Copy/generate to the packet given output buffer. The number of bytes written can be obtained from the buffer.

Todo: It may be preferable to use our pod types here instead of buffer input, but the current codebase requires that the buffer be dynamically resizable for some protocols...

---

{#size}

#### size

`virtual` `const` `inline`

```cpp
virtual inline size_t size() const
```

Defined in src/base/include/icy/packet.h:112

The size of the packet in bytes.

This is the number of bytes that will be written on a call to [write()](#classicy_1_1IPacket_1a87b978fc87c58eb05e1c01ad1ca60f4c), but may not be the number of bytes that will be consumed by [read()](#classicy_1_1IPacket_1ad42e0c52a5092acb3dd1488928902c5b).

---

{#hasdata}

#### hasData

`virtual` `const` `inline`

```cpp
virtual inline bool hasData() const
```

Defined in src/base/include/icy/packet.h:115

Returns true if the packet has a non-null data pointer.

---

{#data}

#### data

`virtual` `const` `inline`

```cpp
virtual inline char * data() const
```

Defined in src/base/include/icy/packet.h:118

The packet data pointer for buffered packets.

---

{#constdata}

#### constData

`virtual` `const` `inline`

```cpp
virtual inline const void * constData() const
```

Defined in src/base/include/icy/packet.h:121

The const packet data pointer for buffered packets.

---

{#classname}

#### className

`virtual` `const`

```cpp
virtual const char * className() const
```

Defined in src/base/include/icy/packet.h:124

Returns the class name of this packet type for logging and diagnostics.

---

{#print}

#### print

`virtual` `const` `inline`

```cpp
virtual inline void print(std::ostream & os) const
```

Defined in src/base/include/icy/packet.h:128

Prints a human-readable representation to the given stream. 
#### Parameters
* `os` Output stream.

{#flagpacket}

## FlagPacket

```cpp
#include <icy/packet.h>
```

```cpp
class FlagPacket
```

Defined in src/base/include/icy/packet.h:146

> **Inherits:** [`IPacket`](#ipacket)

Packet for sending bitwise flags along the packet stream.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`FlagPacket`](#flagpacket) `inline` |  |
| `std::unique_ptr< IPacket >` | [`clone`](#clone) `virtual` `const` `inline` `override` |  |
|  | [`FlagPacket`](#flagpacket) `inline` |  |
| `ssize_t` | [`read`](#read) `virtual` `inline` `override` | No-op read; [FlagPacket](#flagpacket) carries no payload data. |
| `void` | [`write`](#write) `virtual` `const` `inline` `override` | No-op write; [FlagPacket](#flagpacket) carries no payload data. |
| `const char *` | [`className`](#classname) `virtual` `const` `inline` `override` | Returns the class name of this packet type for logging and diagnostics. |

---

{#flagpacket}

#### FlagPacket

`inline`

```cpp
inline FlagPacket(unsigned flags = 0)
```

Defined in src/base/include/icy/packet.h:150

#### Parameters
* `flags` [Bitwise](#bitwise) flags to carry in this packet.

---

{#clone}

#### clone

`virtual` `const` `inline` `override`

```cpp
virtual inline std::unique_ptr< IPacket > clone() const override
```

Defined in src/base/include/icy/packet.h:156

#### Returns
Owning pointer to a deep copy of this packet.

---

{#flagpacket}

#### FlagPacket

`inline`

```cpp
inline FlagPacket(const FlagPacket & that)
```

Defined in src/base/include/icy/packet.h:162

#### Parameters
* `that` Source packet to copy from.

---

{#read}

#### read

`virtual` `inline` `override`

```cpp
virtual inline ssize_t read(const ConstBuffer &) override
```

Defined in src/base/include/icy/packet.h:171

No-op read; [FlagPacket](#flagpacket) carries no payload data. 
#### Returns
Always returns true (1).

---

{#write}

#### write

`virtual` `const` `inline` `override`

```cpp
virtual inline void write(Buffer &) const override
```

Defined in src/base/include/icy/packet.h:174

No-op write; [FlagPacket](#flagpacket) carries no payload data.

---

{#classname}

#### className

`virtual` `const` `inline` `override`

```cpp
virtual inline const char * className() const override
```

Defined in src/base/include/icy/packet.h:176

Returns the class name of this packet type for logging and diagnostics.

{#rawpacket}

## RawPacket

```cpp
#include <icy/packet.h>
```

```cpp
class RawPacket
```

Defined in src/base/include/icy/packet.h:182

> **Inherits:** [`IPacket`](#ipacket)
> **Subclassed by:** [`MediaPacket`](av.md#mediapacket), [`SocketPacket`](net.md#socketpacket)

[RawPacket](#rawpacket) is the default data packet type which consists of an optionally managed char pointer and a size value.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`RawPacket`](#rawpacket) `inline` | Construct with borrowed (non-owning) buffer. |
|  | [`RawPacket`](#rawpacket) `inline` | Construct with const data (copied, owning). |
|  | [`RawPacket`](#rawpacket) `inline` | Copy constructor (always copies data). |
| `std::unique_ptr< IPacket >` | [`clone`](#clone) `virtual` `const` `inline` `override` |  |
| `void` | [`copyData`](#copydata) `virtual` `inline` | Copies data into an internally owned buffer, replacing any prior content. |
| `ssize_t` | [`read`](#read) `virtual` `inline` `override` | Reads from the buffer by copying its contents into an owned buffer. |
| `void` | [`write`](#write) `virtual` `const` `inline` `override` | Appends the packet data to the given output buffer. |
| `char *` | [`data`](#data) `virtual` `const` `inline` `override` |  |
| `size_t` | [`size`](#size) `virtual` `const` `inline` `override` |  |
| `const char *` | [`className`](#classname) `virtual` `const` `inline` `override` | Returns the class name of this packet type for logging and diagnostics. |
| `bool` | [`ownsBuffer`](#ownsbuffer) `const` `inline` |  |

---

{#rawpacket}

#### RawPacket

`inline`

```cpp
inline RawPacket(char * data = nullptr, size_t size = 0, unsigned flags = 0, std::unique_ptr< IPacketInfo > info = nullptr)
```

Defined in src/base/include/icy/packet.h:186

Construct with borrowed (non-owning) buffer.

---

{#rawpacket}

#### RawPacket

`inline`

```cpp
inline RawPacket(const char * data, size_t size = 0, unsigned flags = 0, std::unique_ptr< IPacketInfo > info = nullptr)
```

Defined in src/base/include/icy/packet.h:195

Construct with const data (copied, owning).

---

{#rawpacket}

#### RawPacket

`inline`

```cpp
inline RawPacket(const RawPacket & that)
```

Defined in src/base/include/icy/packet.h:205

Copy constructor (always copies data).

---

{#clone}

#### clone

`virtual` `const` `inline` `override`

```cpp
virtual inline std::unique_ptr< IPacket > clone() const override
```

Defined in src/base/include/icy/packet.h:216

#### Returns
Owning pointer to a deep copy of this packet (always copies data).

---

{#copydata}

#### copyData

`virtual` `inline`

```cpp
virtual inline void copyData(const void * data, size_t size)
```

Defined in src/base/include/icy/packet.h:224

Copies data into an internally owned buffer, replacing any prior content. 
#### Parameters
* `data` Source data pointer. 

* `size` Number of bytes to copy.

---

{#read}

#### read

`virtual` `inline` `override`

```cpp
virtual inline ssize_t read(const ConstBuffer & buf) override
```

Defined in src/base/include/icy/packet.h:237

Reads from the buffer by copying its contents into an owned buffer. 
#### Parameters
* `buf` Input buffer to read from. 

#### Returns
Number of bytes consumed (equal to buf.size()).

---

{#write}

#### write

`virtual` `const` `inline` `override`

```cpp
virtual inline void write(Buffer & buf) const override
```

Defined in src/base/include/icy/packet.h:245

Appends the packet data to the given output buffer. 
#### Parameters
* `buf` [Buffer](#buffer-2) to write into.

---

{#data}

#### data

`virtual` `const` `inline` `override`

```cpp
virtual inline char * data() const override
```

Defined in src/base/include/icy/packet.h:251

#### Returns
Mutable pointer to the raw packet data, or nullptr if empty.

---

{#size}

#### size

`virtual` `const` `inline` `override`

```cpp
virtual inline size_t size() const override
```

Defined in src/base/include/icy/packet.h:254

#### Returns
Size of the packet data in bytes.

---

{#classname}

#### className

`virtual` `const` `inline` `override`

```cpp
virtual inline const char * className() const override
```

Defined in src/base/include/icy/packet.h:256

Returns the class name of this packet type for logging and diagnostics.

---

{#ownsbuffer}

#### ownsBuffer

`const` `inline`

```cpp
inline bool ownsBuffer() const
```

Defined in src/base/include/icy/packet.h:259

#### Returns
True if this packet owns (manages) its data buffer.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `char *` | [`_data`](#_data)  |  |
| `size_t` | [`_size`](#_size)  |  |
| `std::unique_ptr< char[]>` | [`_owned`](#_owned)  |  |

---

{#_data}

#### _data

```cpp
char * _data
```

Defined in src/base/include/icy/packet.h:262

---

{#_size}

#### _size

```cpp
size_t _size
```

Defined in src/base/include/icy/packet.h:263

---

{#_owned}

#### _owned

```cpp
std::unique_ptr< char[]> _owned
```

Defined in src/base/include/icy/packet.h:264

{#configuration}

## Configuration

```cpp
#include <icy/configuration.h>
```

```cpp
class Configuration
```

Defined in src/base/include/icy/configuration.h:31

> **Subclassed by:** [`Configuration`](json.md#configuration-1)

[Configuration](#configuration) is an abstract base class for managing different kinds of configuration storage back ends such as JSON, XML, or database.

Subclasses must override the [getRaw()](#classicy_1_1Configuration_1a93755fec3296753ec88e909095026046) and [setRaw()](#classicy_1_1Configuration_1afe19c7376a327bb2cc117f7a1b1a3b40) and methods.

This class is safe for multithreaded use.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `ThreadSignal< void(const std::string &, const std::string &)>` | [`PropertyChanged`](#propertychanged)  | The Key and Value of the changed configuration property. |

---

{#propertychanged}

#### PropertyChanged

```cpp
ThreadSignal< void(const std::string &, const std::string &)> PropertyChanged
```

Defined in src/base/include/icy/configuration.h:138

The Key and Value of the changed configuration property.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Configuration`](#configuration)  | Creates the [Configuration](#configuration). |
|  | [`~Configuration`](#configuration) `virtual` | Destroys the [Configuration](#configuration). |
| `bool` | [`exists`](#exists) `const` | Returns true if the property with the given key exists. |
| `std::string` | [`getString`](#getstring) `const` | Returns the string value of the property with the given name. Throws a NotFoundException if the key does not exist. |
| `std::string` | [`getString`](#getstring) `const` | If a property with the given key exists, returns the property's string value, otherwise returns the given default value. |
| `std::string` | [`getRawString`](#getrawstring) `const` | Returns the raw string value of the property with the given name. Throws a NotFoundException if the key does not exist. References to other properties are not expanded. |
| `std::string` | [`getRawString`](#getrawstring) `const` | If a property with the given key exists, returns the property's raw string value, otherwise returns the given default value. References to other properties are not expanded. |
| `int` | [`getInt`](#getint) `const` | Returns the int value of the property with the given name. Throws a NotFoundException if the key does not exist. Throws a SyntaxException if the property can not be converted to an int. Numbers starting with 0x are treated as hexadecimal. |
| `int` | [`getInt`](#getint) `const` | If a property with the given key exists, returns the property's int value, otherwise returns the given default value. Throws a SyntaxException if the property can not be converted to an int. Numbers starting with 0x are treated as hexadecimal. |
| `std::int64_t` | [`getLargeInt`](#getlargeint) `const` | Returns the int value of the property with the given name. Throws a NotFoundException if the key does not exist. Throws a SyntaxException if the property can not be converted to an int. Numbers starting with 0x are treated as hexadecimal. |
| `std::int64_t` | [`getLargeInt`](#getlargeint) `const` | If a property with the given key exists, returns the property's int value, otherwise returns the given default value. Throws a SyntaxException if the property can not be converted to an int. Numbers starting with 0x are treated as hexadecimal. |
| `double` | [`getDouble`](#getdouble) `const` | Returns the double value of the property with the given name. Throws a NotFoundException if the key does not exist. Throws a SyntaxException if the property can not be converted to a double. |
| `double` | [`getDouble`](#getdouble) `const` | If a property with the given key exists, returns the property's double value, otherwise returns the given default value. Throws a SyntaxException if the property can not be converted to an double. |
| `bool` | [`getBool`](#getbool) `const` | Returns the double value of the property with the given name. Throws a NotFoundException if the key does not exist. Throws a SyntaxException if the property can not be converted to a double. |
| `bool` | [`getBool`](#getbool) `const` | If a property with the given key exists, returns the property's bool value, otherwise returns the given default value. Throws a SyntaxException if the property can not be converted to a boolean. The following string values can be converted into a boolean: |
| `void` | [`setString`](#setstring)  | Sets the property with the given key to the given value. An already existing value for the key is overwritten. |
| `void` | [`setInt`](#setint)  | Sets the property with the given key to the given value. An already existing value for the key is overwritten. |
| `void` | [`setLargeInt`](#setlargeint)  | Sets the property with the given key to the given value. An already existing value for the key is overwritten. |
| `void` | [`setDouble`](#setdouble)  | Sets the property with the given key to the given value. An already existing value for the key is overwritten. |
| `void` | [`setBool`](#setbool)  | Sets the property with the given key to the given value. An already existing value for the key is overwritten. |

---

{#configuration}

#### Configuration

```cpp
Configuration()
```

Defined in src/base/include/icy/configuration.h:35

Creates the [Configuration](#configuration).

---

{#configuration}

#### ~Configuration

`virtual`

```cpp
virtual ~Configuration()
```

Defined in src/base/include/icy/configuration.h:38

Destroys the [Configuration](#configuration).

---

{#exists}

#### exists

`const`

```cpp
bool exists(const std::string & key) const
```

Defined in src/base/include/icy/configuration.h:41

Returns true if the property with the given key exists.

---

{#getstring}

#### getString

`const`

```cpp
std::string getString(const std::string & key) const
```

Defined in src/base/include/icy/configuration.h:45

Returns the string value of the property with the given name. Throws a NotFoundException if the key does not exist.

---

{#getstring}

#### getString

`const`

```cpp
std::string getString(const std::string & key, const std::string & defaultValue) const
```

Defined in src/base/include/icy/configuration.h:49

If a property with the given key exists, returns the property's string value, otherwise returns the given default value.

---

{#getrawstring}

#### getRawString

`const`

```cpp
std::string getRawString(const std::string & key) const
```

Defined in src/base/include/icy/configuration.h:54

Returns the raw string value of the property with the given name. Throws a NotFoundException if the key does not exist. References to other properties are not expanded.

---

{#getrawstring}

#### getRawString

`const`

```cpp
std::string getRawString(const std::string & key, const std::string & defaultValue) const
```

Defined in src/base/include/icy/configuration.h:59

If a property with the given key exists, returns the property's raw string value, otherwise returns the given default value. References to other properties are not expanded.

---

{#getint}

#### getInt

`const`

```cpp
int getInt(const std::string & key) const
```

Defined in src/base/include/icy/configuration.h:66

Returns the int value of the property with the given name. Throws a NotFoundException if the key does not exist. Throws a SyntaxException if the property can not be converted to an int. Numbers starting with 0x are treated as hexadecimal.

---

{#getint}

#### getInt

`const`

```cpp
int getInt(const std::string & key, int defaultValue) const
```

Defined in src/base/include/icy/configuration.h:73

If a property with the given key exists, returns the property's int value, otherwise returns the given default value. Throws a SyntaxException if the property can not be converted to an int. Numbers starting with 0x are treated as hexadecimal.

---

{#getlargeint}

#### getLargeInt

`const`

```cpp
std::int64_t getLargeInt(const std::string & key) const
```

Defined in src/base/include/icy/configuration.h:80

Returns the int value of the property with the given name. Throws a NotFoundException if the key does not exist. Throws a SyntaxException if the property can not be converted to an int. Numbers starting with 0x are treated as hexadecimal.

---

{#getlargeint}

#### getLargeInt

`const`

```cpp
std::int64_t getLargeInt(const std::string & key, std::int64_t defaultValue) const
```

Defined in src/base/include/icy/configuration.h:88

If a property with the given key exists, returns the property's int value, otherwise returns the given default value. Throws a SyntaxException if the property can not be converted to an int. Numbers starting with 0x are treated as hexadecimal.

---

{#getdouble}

#### getDouble

`const`

```cpp
double getDouble(const std::string & key) const
```

Defined in src/base/include/icy/configuration.h:94

Returns the double value of the property with the given name. Throws a NotFoundException if the key does not exist. Throws a SyntaxException if the property can not be converted to a double.

---

{#getdouble}

#### getDouble

`const`

```cpp
double getDouble(const std::string & key, double defaultValue) const
```

Defined in src/base/include/icy/configuration.h:100

If a property with the given key exists, returns the property's double value, otherwise returns the given default value. Throws a SyntaxException if the property can not be converted to an double.

---

{#getbool}

#### getBool

`const`

```cpp
bool getBool(const std::string & key) const
```

Defined in src/base/include/icy/configuration.h:106

Returns the double value of the property with the given name. Throws a NotFoundException if the key does not exist. Throws a SyntaxException if the property can not be converted to a double.

---

{#getbool}

#### getBool

`const`

```cpp
bool getBool(const std::string & key, bool defaultValue) const
```

Defined in src/base/include/icy/configuration.h:115

If a property with the given key exists, returns the property's bool value, otherwise returns the given default value. Throws a SyntaxException if the property can not be converted to a boolean. The following string values can be converted into a boolean:

* numerical values: non zero becomes true, zero becomes false

* strings: true, yes, on become true, false, no, off become false Case does not matter.

---

{#setstring}

#### setString

```cpp
void setString(const std::string & key, const std::string & value)
```

Defined in src/base/include/icy/configuration.h:119

Sets the property with the given key to the given value. An already existing value for the key is overwritten.

---

{#setint}

#### setInt

```cpp
void setInt(const std::string & key, int value)
```

Defined in src/base/include/icy/configuration.h:123

Sets the property with the given key to the given value. An already existing value for the key is overwritten.

---

{#setlargeint}

#### setLargeInt

```cpp
void setLargeInt(const std::string & key, std::int64_t value)
```

Defined in src/base/include/icy/configuration.h:127

Sets the property with the given key to the given value. An already existing value for the key is overwritten.

---

{#setdouble}

#### setDouble

```cpp
void setDouble(const std::string & key, double value)
```

Defined in src/base/include/icy/configuration.h:131

Sets the property with the given key to the given value. An already existing value for the key is overwritten.

---

{#setbool}

#### setBool

```cpp
void setBool(const std::string & key, bool value)
```

Defined in src/base/include/icy/configuration.h:135

Sets the property with the given key to the given value. An already existing value for the key is overwritten.

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `bool` | [`getRaw`](#getraw) `virtual` `const` | If the property with the given key exists, stores the property's value in value and returns true. Otherwise, returns false. |
| `void` | [`setRaw`](#setraw) `virtual` | Sets the property with the given key to the given value. An already existing value for the key is overwritten. |
|  | [`Configuration`](#configuration)  | Deleted constructor. |
|  | [`Configuration`](#configuration)  | Deleted constructor. |

---

{#getraw}

#### getRaw

`virtual` `const`

```cpp
virtual bool getRaw(const std::string & key, std::string & value) const
```

Defined in src/base/include/icy/configuration.h:145

If the property with the given key exists, stores the property's value in value and returns true. Otherwise, returns false.

Must be overridden by subclasses.

---

{#setraw}

#### setRaw

`virtual`

```cpp
virtual void setRaw(const std::string & key, const std::string & value)
```

Defined in src/base/include/icy/configuration.h:151

Sets the property with the given key to the given value. An already existing value for the key is overwritten.

The implementation is responsible for emitting the PropertyChanged signal.

---

{#configuration}

#### Configuration

```cpp
Configuration(const Configuration &) = delete
```

Defined in src/base/include/icy/configuration.h:170

Deleted constructor.

---

{#configuration}

#### Configuration

```cpp
Configuration(Configuration &&) = delete
```

Defined in src/base/include/icy/configuration.h:172

Deleted constructor.

### Private Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::mutex` | [`_mutex`](#_mutex)  |  |

---

{#_mutex}

#### _mutex

```cpp
std::mutex _mutex
```

Defined in src/base/include/icy/configuration.h:176

{#scopedconfiguration}

## ScopedConfiguration

```cpp
#include <icy/configuration.h>
```

```cpp
class ScopedConfiguration
```

Defined in src/base/include/icy/configuration.h:195

[ScopedConfiguration](#scopedconfiguration) provides multiple levels of configuration for a module. Multiple levels means that there is a module level scope, and a default scope. When a property is accessed, the module scope value will be used if available, otherwise the default scope value will be used.

Example scoping: Module: channels.[name].modes.[name].[value] Default: modes.[name].[value]

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `Configuration &` | [`config`](#config)  |  |
| `std::string` | [`currentScope`](#currentscope)  |  |
| `std::string` | [`defaultScope`](#defaultscope)  |  |

---

{#config}

#### config

```cpp
Configuration & config
```

Defined in src/base/include/icy/configuration.h:278

---

{#currentscope}

#### currentScope

```cpp
std::string currentScope
```

Defined in src/base/include/icy/configuration.h:279

---

{#defaultscope}

#### defaultScope

```cpp
std::string defaultScope
```

Defined in src/base/include/icy/configuration.h:280

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`ScopedConfiguration`](#scopedconfiguration)  |  |
|  | [`ScopedConfiguration`](#scopedconfiguration)  |  |
| `std::string` | [`getString`](#getstring) `const` | Returns the string value, trying `[currentScope](#classicy_1_1ScopedConfiguration_1a3d8d7c8052515af4617962e0a790d703)` first then `[defaultScope](#classicy_1_1ScopedConfiguration_1a15e6f20291047fb072404269feffae68)`. |
| `int` | [`getInt`](#getint) `const` | Returns the int value, trying `[currentScope](#classicy_1_1ScopedConfiguration_1a3d8d7c8052515af4617962e0a790d703)` first then `[defaultScope](#classicy_1_1ScopedConfiguration_1a15e6f20291047fb072404269feffae68)`. |
| `double` | [`getDouble`](#getdouble) `const` | Returns the double value, trying `[currentScope](#classicy_1_1ScopedConfiguration_1a3d8d7c8052515af4617962e0a790d703)` first then `[defaultScope](#classicy_1_1ScopedConfiguration_1a15e6f20291047fb072404269feffae68)`. |
| `bool` | [`getBool`](#getbool) `const` | Returns the bool value, trying `[currentScope](#classicy_1_1ScopedConfiguration_1a3d8d7c8052515af4617962e0a790d703)` first then `[defaultScope](#classicy_1_1ScopedConfiguration_1a15e6f20291047fb072404269feffae68)`. |
| `void` | [`setString`](#setstring)  | Writes a string value under the scoped key. |
| `void` | [`setInt`](#setint)  | Writes an int value under the scoped key. |
| `void` | [`setDouble`](#setdouble)  | Writes a double value under the scoped key. |
| `void` | [`setBool`](#setbool)  | Writes a bool value under the scoped key. |
| `std::string` | [`getCurrentScope`](#getcurrentscope) `const` |  |
| `std::string` | [`getDafaultKey`](#getdafaultkey) `const` |  |
| `std::string` | [`getScopedKey`](#getscopedkey) `const` |  |

---

{#scopedconfiguration}

#### ScopedConfiguration

```cpp
ScopedConfiguration(Configuration & config, const std::string & currentScope, const std::string & defaultScope)
```

Defined in src/base/include/icy/configuration.h:201

#### Parameters
* `config` Backing configuration store. 

* `currentScope` Key prefix for the module-level scope. 

* `defaultScope` Key prefix for the default/fallback scope.

---

{#scopedconfiguration}

#### ScopedConfiguration

```cpp
ScopedConfiguration(const ScopedConfiguration & that)
```

Defined in src/base/include/icy/configuration.h:203

---

{#getstring}

#### getString

`const`

```cpp
std::string getString(const std::string & key, const std::string & defaultValue, bool forceDefaultScope = false) const
```

Defined in src/base/include/icy/configuration.h:210

Returns the string value, trying `[currentScope](#classicy_1_1ScopedConfiguration_1a3d8d7c8052515af4617962e0a790d703)` first then `[defaultScope](#classicy_1_1ScopedConfiguration_1a15e6f20291047fb072404269feffae68)`. 
#### Parameters
* `key` Property key (without scope prefix). 

* `defaultValue` Fallback when neither scope has the key. 

* `forceDefaultScope` If true, skips `[currentScope](#classicy_1_1ScopedConfiguration_1a3d8d7c8052515af4617962e0a790d703)` and reads from `[defaultScope](#classicy_1_1ScopedConfiguration_1a15e6f20291047fb072404269feffae68)` only. 

#### Returns
Property value or `defaultValue`.

---

{#getint}

#### getInt

`const`

```cpp
int getInt(const std::string & key, int defaultValue, bool forceDefaultScope = false) const
```

Defined in src/base/include/icy/configuration.h:219

Returns the int value, trying `[currentScope](#classicy_1_1ScopedConfiguration_1a3d8d7c8052515af4617962e0a790d703)` first then `[defaultScope](#classicy_1_1ScopedConfiguration_1a15e6f20291047fb072404269feffae68)`. 
#### Parameters
* `key` Property key (without scope prefix). 

* `defaultValue` Fallback when neither scope has the key. 

* `forceDefaultScope` If true, reads from `[defaultScope](#classicy_1_1ScopedConfiguration_1a15e6f20291047fb072404269feffae68)` only. 

#### Returns
Property value or `defaultValue`.

---

{#getdouble}

#### getDouble

`const`

```cpp
double getDouble(const std::string & key, double defaultValue, bool forceDefaultScope = false) const
```

Defined in src/base/include/icy/configuration.h:227

Returns the double value, trying `[currentScope](#classicy_1_1ScopedConfiguration_1a3d8d7c8052515af4617962e0a790d703)` first then `[defaultScope](#classicy_1_1ScopedConfiguration_1a15e6f20291047fb072404269feffae68)`. 
#### Parameters
* `key` Property key (without scope prefix). 

* `defaultValue` Fallback when neither scope has the key. 

* `forceDefaultScope` If true, reads from `[defaultScope](#classicy_1_1ScopedConfiguration_1a15e6f20291047fb072404269feffae68)` only. 

#### Returns
Property value or `defaultValue`.

---

{#getbool}

#### getBool

`const`

```cpp
bool getBool(const std::string & key, bool defaultValue, bool forceDefaultScope = false) const
```

Defined in src/base/include/icy/configuration.h:235

Returns the bool value, trying `[currentScope](#classicy_1_1ScopedConfiguration_1a3d8d7c8052515af4617962e0a790d703)` first then `[defaultScope](#classicy_1_1ScopedConfiguration_1a15e6f20291047fb072404269feffae68)`. 
#### Parameters
* `key` Property key (without scope prefix). 

* `defaultValue` Fallback when neither scope has the key. 

* `forceDefaultScope` If true, reads from `[defaultScope](#classicy_1_1ScopedConfiguration_1a15e6f20291047fb072404269feffae68)` only. 

#### Returns
Property value or `defaultValue`.

---

{#setstring}

#### setString

```cpp
void setString(const std::string & key, const std::string & value, bool defaultScope = false)
```

Defined in src/base/include/icy/configuration.h:242

Writes a string value under the scoped key. 
#### Parameters
* `key` Property key (without scope prefix). 

* `value` Value to store. 

* `defaultScope` If true, writes to `[defaultScope](#classicy_1_1ScopedConfiguration_1a15e6f20291047fb072404269feffae68)`; otherwise to `[currentScope](#classicy_1_1ScopedConfiguration_1a3d8d7c8052515af4617962e0a790d703)`.

---

{#setint}

#### setInt

```cpp
void setInt(const std::string & key, int value, bool defaultScope = false)
```

Defined in src/base/include/icy/configuration.h:249

Writes an int value under the scoped key. 
#### Parameters
* `key` Property key (without scope prefix). 

* `value` Value to store. 

* `defaultScope` If true, writes to `[defaultScope](#classicy_1_1ScopedConfiguration_1a15e6f20291047fb072404269feffae68)`; otherwise to `[currentScope](#classicy_1_1ScopedConfiguration_1a3d8d7c8052515af4617962e0a790d703)`.

---

{#setdouble}

#### setDouble

```cpp
void setDouble(const std::string & key, double value, bool defaultScope = false)
```

Defined in src/base/include/icy/configuration.h:255

Writes a double value under the scoped key. 
#### Parameters
* `key` Property key (without scope prefix). 

* `value` Value to store. 

* `defaultScope` If true, writes to `[defaultScope](#classicy_1_1ScopedConfiguration_1a15e6f20291047fb072404269feffae68)`; otherwise to `[currentScope](#classicy_1_1ScopedConfiguration_1a3d8d7c8052515af4617962e0a790d703)`.

---

{#setbool}

#### setBool

```cpp
void setBool(const std::string & key, bool value, bool defaultScope = false)
```

Defined in src/base/include/icy/configuration.h:262

Writes a bool value under the scoped key. 
#### Parameters
* `key` Property key (without scope prefix). 

* `value` Value to store. 

* `defaultScope` If true, writes to `[defaultScope](#classicy_1_1ScopedConfiguration_1a15e6f20291047fb072404269feffae68)`; otherwise to `[currentScope](#classicy_1_1ScopedConfiguration_1a3d8d7c8052515af4617962e0a790d703)`.

---

{#getcurrentscope}

#### getCurrentScope

`const`

```cpp
std::string getCurrentScope(const std::string & key) const
```

Defined in src/base/include/icy/configuration.h:266

#### Parameters
* `key` Property key (without scope prefix). 

#### Returns
Fully qualified key in `[currentScope](#classicy_1_1ScopedConfiguration_1a3d8d7c8052515af4617962e0a790d703)`.

---

{#getdafaultkey}

#### getDafaultKey

`const`

```cpp
std::string getDafaultKey(const std::string & key) const
```

Defined in src/base/include/icy/configuration.h:270

#### Parameters
* `key` Property key (without scope prefix). 

#### Returns
Fully qualified key in `[defaultScope](#classicy_1_1ScopedConfiguration_1a15e6f20291047fb072404269feffae68)`.

---

{#getscopedkey}

#### getScopedKey

`const`

```cpp
std::string getScopedKey(const std::string & key, bool defaultScope = false) const
```

Defined in src/base/include/icy/configuration.h:275

#### Parameters
* `key` Property key (without scope prefix). 

* `defaultScope` If true, returns the `[defaultScope](#classicy_1_1ScopedConfiguration_1a15e6f20291047fb072404269feffae68)` key; otherwise `[currentScope](#classicy_1_1ScopedConfiguration_1a3d8d7c8052515af4617962e0a790d703)` key. 

#### Returns
Fully qualified scoped key string.

{#error}

## Error

```cpp
#include <icy/error.h>
```

```cpp
struct Error
```

Defined in src/base/include/icy/error.h:32

Basic error type.

Errors contain an error code, message, and exception pointer.

### Friends

| Name | Description |
|------|-------------|
| [`operator<<`](#operator) `inline` |  |

---

{#operator}

#### operator<<

`inline`

```cpp
friend inline std::ostream & operator<<(std::ostream & stream, const Error & err)
```

Defined in src/base/include/icy/error.h:80

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `int` | [`err`](#err)  |  |
| `std::string` | [`message`](#message)  |  |
| `std::exception_ptr` | [`exception`](#exception)  |  |

---

{#err}

#### err

```cpp
int err
```

Defined in src/base/include/icy/error.h:34

---

{#message}

#### message

```cpp
std::string message
```

Defined in src/base/include/icy/error.h:35

---

{#exception}

#### exception

```cpp
std::exception_ptr exception
```

Defined in src/base/include/icy/error.h:36

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Error`](#error) `inline` | Default constructor; initializes all fields to a no-error state. |
|  | [`Error`](#error) `inline` | Constructs an error with the given message string. |
|  | [`Error`](#error) `inline` | Constructs an error with the given C string message. |
| `bool` | [`any`](#any) `const` `inline` | Returns true if any error condition is set (non-zero code, non-empty message, or exception). |
| `void` | [`reset`](#reset) `inline` | Clears all error fields, resetting to a no-error state. |
| `void` | [`rethrow`](#rethrow) `inline` | Re-throws the stored exception pointer if one is set. Has no effect if `[exception](#structicy_1_1Error_1a77a10c10703b1ad91bcd52ee5e05accf)` is null. |

---

{#error}

#### Error

`inline`

```cpp
inline Error()
```

Defined in src/base/include/icy/error.h:39

Default constructor; initializes all fields to a no-error state.

---

{#error}

#### Error

`inline`

```cpp
inline Error(const std::string & msg)
```

Defined in src/base/include/icy/error.h:43

Constructs an error with the given message string. 
#### Parameters
* `msg` Human-readable error description.

---

{#error}

#### Error

`inline`

```cpp
inline Error(const char * msg)
```

Defined in src/base/include/icy/error.h:51

Constructs an error with the given C string message. 
#### Parameters
* `msg` Human-readable error description.

---

{#any}

#### any

`const` `inline`

```cpp
inline bool any() const
```

Defined in src/base/include/icy/error.h:59

Returns true if any error condition is set (non-zero code, non-empty message, or exception). 
#### Returns
True if an error is present.

---

{#reset}

#### reset

`inline`

```cpp
inline void reset()
```

Defined in src/base/include/icy/error.h:65

Clears all error fields, resetting to a no-error state.

---

{#rethrow}

#### rethrow

`inline`

```cpp
inline void rethrow()
```

Defined in src/base/include/icy/error.h:74

Re-throws the stored exception pointer if one is set. Has no effect if `[exception](#structicy_1_1Error_1a77a10c10703b1ad91bcd52ee5e05accf)` is null.

{#logstream}

## LogStream

```cpp
#include <icy/logger.h>
```

```cpp
struct LogStream
```

Defined in src/base/include/icy/logger.h:314

No-op log record used when logging is compiled out.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`LogStream`](#logstream) `inline` |  |
|  | [`LogStream`](#logstream) `inline` |  |
| `void` | [`write`](#write) `inline` |  |
| `LogStream &` | [`operator<<`](#operator) `inline` |  |
| `LogStream &` | [`operator<<`](#operator) `inline` |  |

---

{#logstream}

#### LogStream

`inline`

```cpp
inline LogStream(Level level, std::string realm, int line, const char * channel = nullptr)
```

Defined in src/base/include/icy/logger.h:316

---

{#logstream}

#### LogStream

`inline`

```cpp
inline LogStream(const LogStream & that)
```

Defined in src/base/include/icy/logger.h:317

---

{#write}

#### write

`inline`

```cpp
template<typename... Args> inline void write(Args... args)
```

Defined in src/base/include/icy/logger.h:321

---

{#operator}

#### operator<<

`inline`

```cpp
template<typename T> inline LogStream & operator<<(const T &)
```

Defined in src/base/include/icy/logger.h:326

---

{#operator}

#### operator<<

`inline`

```cpp
inline LogStream & operator<<(std::ostream &(*)(std::ostream &) f)
```

Defined in src/base/include/icy/logger.h:331

{#nullsharedmutex}

## NullSharedMutex

```cpp
#include <icy/signal.h>
```

```cpp
struct NullSharedMutex
```

Defined in src/base/include/icy/signal.h:30

No-op mutex for single-threaded signal usage. When all signal operations occur on a single libuv event loop thread, the shared_mutex is unnecessary overhead.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`lock`](#lock) `inline` |  |
| `void` | [`unlock`](#unlock) `inline` |  |
| `void` | [`lock_shared`](#lock_shared) `inline` |  |
| `void` | [`unlock_shared`](#unlock_shared) `inline` |  |

---

{#lock}

#### lock

`inline`

```cpp
inline void lock()
```

Defined in src/base/include/icy/signal.h:32

---

{#unlock}

#### unlock

`inline`

```cpp
inline void unlock()
```

Defined in src/base/include/icy/signal.h:33

---

{#lock_shared}

#### lock_shared

`inline`

```cpp
inline void lock_shared()
```

Defined in src/base/include/icy/signal.h:34

---

{#unlock_shared}

#### unlock_shared

`inline`

```cpp
inline void unlock_shared()
```

Defined in src/base/include/icy/signal.h:35

{#bitwise}

## Bitwise

```cpp
#include <icy/bitwise.h>
```

```cpp
struct Bitwise
```

Defined in src/base/include/icy/bitwise.h:22

Container for smart management of bitwise integer flags.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `unsigned` | [`data`](#data)  | Backing storage for the flag bits. |

---

{#data}

#### data

```cpp
unsigned data
```

Defined in src/base/include/icy/bitwise.h:24

Backing storage for the flag bits.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Bitwise`](#bitwise) `inline` | Constructs a [Bitwise](#bitwise) with optional initial flags. |
| `void` | [`reset`](#reset) `virtual` `inline` | Clears all flags (sets data to 0). |
| `void` | [`set`](#set) `virtual` `inline` | Sets the given flag only if it is not already set. |
| `void` | [`add`](#add) `virtual` `inline` | Unconditionally sets (OR) the given flag bits. |
| `void` | [`remove`](#remove) `virtual` `inline` | Clears the given flag bits. |
| `void` | [`toggle`](#toggle) `virtual` `inline` | Toggles (XOR) the given flag bits. |
| `bool` | [`has`](#has) `virtual` `const` `inline` | Returns true if all bits in flag are set. |

---

{#bitwise}

#### Bitwise

`inline`

```cpp
inline Bitwise(unsigned flags = 0)
```

Defined in src/base/include/icy/bitwise.h:28

Constructs a [Bitwise](#bitwise) with optional initial flags. 
#### Parameters
* `flags` Initial flag value (default: 0).

---

{#reset}

#### reset

`virtual` `inline`

```cpp
virtual inline void reset()
```

Defined in src/base/include/icy/bitwise.h:34

Clears all flags (sets data to 0).

---

{#set}

#### set

`virtual` `inline`

```cpp
virtual inline void set(unsigned flag)
```

Defined in src/base/include/icy/bitwise.h:38

Sets the given flag only if it is not already set. 
#### Parameters
* `flag` The flag bit(s) to set.

---

{#add}

#### add

`virtual` `inline`

```cpp
virtual inline void add(unsigned flag)
```

Defined in src/base/include/icy/bitwise.h:46

Unconditionally sets (OR) the given flag bits. 
#### Parameters
* `flag` The flag bit(s) to add.

---

{#remove}

#### remove

`virtual` `inline`

```cpp
virtual inline void remove(unsigned flag)
```

Defined in src/base/include/icy/bitwise.h:50

Clears the given flag bits. 
#### Parameters
* `flag` The flag bit(s) to remove.

---

{#toggle}

#### toggle

`virtual` `inline`

```cpp
virtual inline void toggle(unsigned flag)
```

Defined in src/base/include/icy/bitwise.h:54

Toggles (XOR) the given flag bits. 
#### Parameters
* `flag` The flag bit(s) to toggle.

---

{#has}

#### has

`virtual` `const` `inline`

```cpp
virtual inline bool has(unsigned flag) const
```

Defined in src/base/include/icy/bitwise.h:59

Returns true if all bits in flag are set. 
#### Parameters
* `flag` The flag bit(s) to test. 

#### Returns
true if every bit in flag is present in data.

{#abstractdelegate}

## AbstractDelegate

```cpp
#include <icy/delegate.h>
```

```cpp
template<typename RT, typename... Args>
struct AbstractDelegate
```

Defined in src/base/include/icy/delegate.h:29

Abstract delegate interface.

The `Delegate` class contains a pointer to a function. This wrapper class is used instead of `std::function` since it is interchangable with fast delegates and also provides an equality operator for comparing the underlying function where supported.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
| `RT` | [`operator()`](#operator) `virtual` `const` | Invokes the underlying callable with the supplied arguments. |
| `bool` | [`operator==`](#operator) `virtual` `const` | Compares two delegates for equality (same target function and instance). |

---

{#operator}

#### operator()

`virtual` `const`

```cpp
virtual RT operator()(Args... args) const
```

Defined in src/base/include/icy/delegate.h:36

Invokes the underlying callable with the supplied arguments. 
#### Parameters
* `args` Arguments forwarded to the wrapped function. 

#### Returns
Result of the wrapped function call.

---

{#operator}

#### operator==

`virtual` `const`

```cpp
virtual bool operator==(const AbstractDelegate< RT, Args... > & that) const
```

Defined in src/base/include/icy/delegate.h:41

Compares two delegates for equality (same target function and instance). 
#### Parameters
* `that` Other delegate to compare against. 

#### Returns
True if both delegates refer to the same callable target.

{#functiondelegate}

## FunctionDelegate

```cpp
#include <icy/delegate.h>
```

```cpp
template<typename RT, typename... Args>
struct FunctionDelegate
```

Defined in src/base/include/icy/delegate.h:48

> **Inherits:** [`AbstractDelegate< RT, Args... >`](#abstractdelegate)

The `[FunctionDelegate](#functiondelegate)` contains a `std::function`.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::function< RT(Args...)>` | [`func`](#func)  |  |

---

{#func}

#### func

```cpp
std::function< RT(Args...)> func
```

Defined in src/base/include/icy/delegate.h:50

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`FunctionDelegate`](#functiondelegate) `inline` |  |
| `RT` | [`operator()`](#operator) `virtual` `const` `inline` `override` | Calls the wrapped `std::function`. |
| `bool` | [`operator==`](#operator) `virtual` `const` `inline` `override` | Always returns false; `std::function` targets cannot be compared for equality. |

---

{#functiondelegate}

#### FunctionDelegate

`inline`

```cpp
inline FunctionDelegate(std::function< RT(Args...)> func)
```

Defined in src/base/include/icy/delegate.h:53

#### Parameters
* `func` `std::function` to wrap.

---

{#operator}

#### operator()

`virtual` `const` `inline` `override`

```cpp
virtual inline RT operator()(Args... args) const override
```

Defined in src/base/include/icy/delegate.h:61

Calls the wrapped `std::function`. 
#### Parameters
* `args` Arguments forwarded to the function. 

#### Returns
Function result.

---

{#operator}

#### operator==

`virtual` `const` `inline` `override`

```cpp
virtual inline bool operator==(const AbstractDelegate< RT, Args... > &) const override
```

Defined in src/base/include/icy/delegate.h:67

Always returns false; `std::function` targets cannot be compared for equality.

{#classdelegate}

## ClassDelegate

```cpp
#include <icy/delegate.h>
```

```cpp
template<class Class, typename RT, typename... Args>
struct ClassDelegate
```

Defined in src/base/include/icy/delegate.h:79

> **Inherits:** [`AbstractDelegate< RT, Args... >`](#abstractdelegate)

The `[ClassDelegate](#classdelegate)` contains a pointer to a class member.

This class implements fast delegates and function comparison.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `Class *` | [`instance`](#instance)  |  |
| `RT(Class::*` | [`method`](#method)  |  |

---

{#instance}

#### instance

```cpp
Class * instance
```

Defined in src/base/include/icy/delegate.h:81

---

{#method}

#### method

```cpp
RT(Class::* method
```

Defined in src/base/include/icy/delegate.h:82

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`ClassDelegate`](#classdelegate) `inline` |  |
| `RT` | [`operator()`](#operator) `virtual` `const` `inline` `override` | Calls the member function on `[instance](#structicy_1_1ClassDelegate_1a1412fc98659c1148559e2cf42a2af610)`. |
| `bool` | [`operator==`](#operator) `virtual` `const` `inline` `override` |  |

---

{#classdelegate}

#### ClassDelegate

`inline`

```cpp
inline ClassDelegate(Class * instance, RT(Class::*)(Args...) method)
```

Defined in src/base/include/icy/delegate.h:86

#### Parameters
* `instance` Object on which to invoke the member function. 

* `method` Pointer to the member function to wrap.

---

{#operator}

#### operator()

`virtual` `const` `inline` `override`

```cpp
virtual inline RT operator()(Args... args) const override
```

Defined in src/base/include/icy/delegate.h:95

Calls the member function on `[instance](#structicy_1_1ClassDelegate_1a1412fc98659c1148559e2cf42a2af610)`. 
#### Parameters
* `args` Arguments forwarded to the method. 

#### Returns
Method result.

---

{#operator}

#### operator==

`virtual` `const` `inline` `override`

```cpp
virtual inline bool operator==(const AbstractDelegate< RT, Args... > & that) const override
```

Defined in src/base/include/icy/delegate.h:102

#### Parameters
* `that` Other delegate to compare. 

#### Returns
True if both delegates wrap the same instance/method pair.

{#constclassdelegate}

## ConstClassDelegate

```cpp
#include <icy/delegate.h>
```

```cpp
template<class Class, typename RT, typename... Args>
struct ConstClassDelegate
```

Defined in src/base/include/icy/delegate.h:116

> **Inherits:** [`AbstractDelegate< RT, Args... >`](#abstractdelegate)

The `[ConstClassDelegate](#constclassdelegate)` contains a pointer to a `const` class member.

This class implements fast delegates and function comparison.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `Class *` | [`instance`](#instance)  |  |
| `RT(Class::*` | [`method`](#method)  |  |

---

{#instance}

#### instance

```cpp
Class * instance
```

Defined in src/base/include/icy/delegate.h:118

---

{#method}

#### method

```cpp
RT(Class::* method
```

Defined in src/base/include/icy/delegate.h:119

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`ConstClassDelegate`](#constclassdelegate) `inline` |  |
| `RT` | [`operator()`](#operator) `virtual` `const` `inline` `override` | Calls the const member function on `[instance](#structicy_1_1ConstClassDelegate_1a5385d947b50390c33d9e7d6157b23c69)`. |
| `bool` | [`operator==`](#operator) `virtual` `const` `inline` `override` |  |

---

{#constclassdelegate}

#### ConstClassDelegate

`inline`

```cpp
inline ConstClassDelegate(Class * instance, RT(Class::*)(Args...) const method)
```

Defined in src/base/include/icy/delegate.h:123

#### Parameters
* `instance` Object on which to invoke the const member function. 

* `method` Pointer to the const member function to wrap.

---

{#operator}

#### operator()

`virtual` `const` `inline` `override`

```cpp
virtual inline RT operator()(Args... args) const override
```

Defined in src/base/include/icy/delegate.h:132

Calls the const member function on `[instance](#structicy_1_1ConstClassDelegate_1a5385d947b50390c33d9e7d6157b23c69)`. 
#### Parameters
* `args` Arguments forwarded to the method. 

#### Returns
Method result.

---

{#operator}

#### operator==

`virtual` `const` `inline` `override`

```cpp
virtual inline bool operator==(const AbstractDelegate< RT, Args... > & that) const override
```

Defined in src/base/include/icy/delegate.h:139

#### Parameters
* `that` Other delegate to compare. 

#### Returns
True if both delegates wrap the same instance/method pair.

{#polymorphicdelegate}

## PolymorphicDelegate

```cpp
#include <icy/delegate.h>
```

```cpp
template<class Class, typename RT, class PT, class IT>
struct PolymorphicDelegate
```

Defined in src/base/include/icy/delegate.h:154

> **Inherits:** [`AbstractDelegate< RT, IT & >`](#abstractdelegate)

Polymorphic function delegate.

Theis class contains a pointer to a class member that receices a derived subclass (`PT`) of the base type specified by the `IT` param.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `Class *` | [`instance`](#instance)  |  |
| `RT(Class::*` | [`method`](#method)  |  |

---

{#instance}

#### instance

```cpp
Class * instance
```

Defined in src/base/include/icy/delegate.h:156

---

{#method}

#### method

```cpp
RT(Class::* method
```

Defined in src/base/include/icy/delegate.h:157

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`PolymorphicDelegate`](#polymorphicdelegate) `inline` |  |
| `RT` | [`operator()`](#operator) `virtual` `const` `inline` `override` | Attempts to cast `object` to `PT`; invokes the method if successful. Returns a default-constructed `RT` if the cast fails (packet type mismatch). |
| `bool` | [`operator==`](#operator) `virtual` `const` `inline` `override` |  |

---

{#polymorphicdelegate}

#### PolymorphicDelegate

`inline`

```cpp
inline PolymorphicDelegate(Class * instance, RT(Class::*)(PT &) method)
```

Defined in src/base/include/icy/delegate.h:161

#### Parameters
* `instance` Object on which to invoke the member function. 

* `method` Pointer to the member function accepting a derived `PT&`.

---

{#operator}

#### operator()

`virtual` `const` `inline` `override`

```cpp
virtual inline RT operator()(IT & object) const override
```

Defined in src/base/include/icy/delegate.h:171

Attempts to cast `object` to `PT`; invokes the method if successful. Returns a default-constructed `RT` if the cast fails (packet type mismatch). 
#### Parameters
* `object` Base-type reference; cast to `PT` before dispatch. 

#### Returns
Method result, or default `RT` on cast failure.

---

{#operator}

#### operator==

`virtual` `const` `inline` `override`

```cpp
virtual inline bool operator==(const AbstractDelegate< RT, IT & > & that) const override
```

Defined in src/base/include/icy/delegate.h:181

#### Parameters
* `that` Other delegate to compare. 

#### Returns
True if both delegates wrap the same instance/method pair.

{#optionparser}

## OptionParser

```cpp
#include <icy/application.h>
```

```cpp
struct OptionParser
```

Defined in src/base/include/icy/application.h:100

Command-line option parser.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`exepath`](#exepath)  |  |
| `OptionMap` | [`args`](#args)  |  |

---

{#exepath}

#### exepath

```cpp
std::string exepath
```

Defined in src/base/include/icy/application.h:102

---

{#args}

#### args

```cpp
OptionMap args
```

Defined in src/base/include/icy/application.h:103

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`OptionParser`](#optionparser)  | Parses command-line arguments from `argc`/`argv`. |
| `bool` | [`has`](#has) `inline` | Returns true if the given option key was present on the command line. |
| `std::string` | [`get`](#get) `inline` | Returns the string value associated with the given option key. Returns an empty string if the key was not found. |
| `NumericType` | [`get`](#get) `inline` | Returns the value associated with the given option key, converted to a numeric type. |

---

{#optionparser}

#### OptionParser

```cpp
OptionParser(int argc, char * argv, const char * delim)
```

Defined in src/base/include/icy/application.h:109

Parses command-line arguments from `argc`/`argv`. 
#### Parameters
* `argc` Argument count from `main()`. 

* `argv` Argument vector from `main()`. 

* `delim` Prefix string that identifies an option key (e.g. "--").

---

{#has}

#### has

`inline`

```cpp
inline bool has(const char * key)
```

Defined in src/base/include/icy/application.h:114

Returns true if the given option key was present on the command line. 
#### Parameters
* `key` Option key to look up (without delimiter prefix). 

#### Returns
True if the key exists in the parsed argument map.

---

{#get}

#### get

`inline`

```cpp
inline std::string get(const char * key)
```

Defined in src/base/include/icy/application.h:120

Returns the string value associated with the given option key. Returns an empty string if the key was not found. 
#### Parameters
* `key` Option key to look up. 

#### Returns
Value string, or empty string if not present.

---

{#get}

#### get

`inline`

```cpp
template<typename NumericType> inline NumericType get(const char * key)
```

Defined in src/base/include/icy/application.h:133

Returns the value associated with the given option key, converted to a numeric type. 
#### Parameters
* `NumericType` Integral or floating-point type to convert the value to. 

#### Parameters
* `key` Option key to look up. 

#### Returns
Converted numeric value.

{#shutdowncmd}

## ShutdownCmd

```cpp
#include <icy/application.h>
```

```cpp
struct ShutdownCmd
```

Defined in src/base/include/icy/application.h:148

Shutdown command packet for signalling process termination.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `void *` | [`opaque`](#opaque)  |  |
| `std::function< void(void *)>` | [`callback`](#callback)  |  |

---

{#opaque}

#### opaque

```cpp
void * opaque
```

Defined in src/base/include/icy/application.h:150

---

{#callback}

#### callback

```cpp
std::function< void(void *)> callback
```

Defined in src/base/include/icy/application.h:151

{#packetadapterreference}

## PacketAdapterReference

```cpp
#include <icy/packetstream.h>
```

```cpp
struct PacketAdapterReference
```

Defined in src/base/include/icy/packetstream.h:148

Provides a reference to a [PacketStreamAdapter](#packetstreamadapter) with optional ownership.

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `PacketStreamAdapter *` | [`ptr`](#ptr)  |  |
| `std::shared_ptr< void >` | [`_prevent_deletion`](#_prevent_deletion)  |  |
| `int` | [`order`](#order)  |  |
| `bool` | [`syncState`](#syncstate)  |  |

---

{#ptr}

#### ptr

```cpp
PacketStreamAdapter * ptr
```

Defined in src/base/include/icy/packetstream.h:152

---

{#_prevent_deletion}

#### _prevent_deletion

```cpp
std::shared_ptr< void > _prevent_deletion
```

Defined in src/base/include/icy/packetstream.h:153

---

{#order}

#### order

```cpp
int order
```

Defined in src/base/include/icy/packetstream.h:154

---

{#syncstate}

#### syncState

```cpp
bool syncState
```

Defined in src/base/include/icy/packetstream.h:155

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`PacketAdapterReference`](#packetadapterreference) `inline` | Construct with raw pointer (non-owning). |
|  | [`PacketAdapterReference`](#packetadapterreference) `inline` | Construct with shared_ptr ownership. |

---

{#packetadapterreference}

#### PacketAdapterReference

`inline`

```cpp
inline PacketAdapterReference(PacketStreamAdapter * ptr = nullptr, int order = 0, bool syncState = false)
```

Defined in src/base/include/icy/packetstream.h:158

Construct with raw pointer (non-owning).

---

{#packetadapterreference}

#### PacketAdapterReference

`inline`

```cpp
template<class C> inline PacketAdapterReference(std::shared_ptr< C > owned, int order = 0, bool syncState = false)
```

Defined in src/base/include/icy/packetstream.h:168

Construct with shared_ptr ownership.

### Public Static Methods

| Return | Name | Description |
|--------|------|-------------|
| `bool` | [`compareOrder`](#compareorder) `static` `inline` | Comparator for sorting references by ascending order value. |

---

{#compareorder}

#### compareOrder

`static` `inline`

```cpp
static inline bool compareOrder(const PacketAdapterReference::Ptr & l, const PacketAdapterReference::Ptr & r)
```

Defined in src/base/include/icy/packetstream.h:181

Comparator for sorting references by ascending order value. 
#### Parameters
* `l` Left-hand reference. 

* `r` Right-hand reference. 

#### Returns
true if `l` should appear before `r` in the processor chain.

### Public Types

| Name | Description |
|------|-------------|
| [`Ptr`](#ptr)  |  |

---

{#ptr}

#### Ptr

```cpp
using Ptr = std::shared_ptr< PacketAdapterReference >
```

Defined in src/base/include/icy/packetstream.h:150

{#packetstreamstate}

## PacketStreamState

```cpp
#include <icy/packetstream.h>
```

```cpp
struct PacketStreamState
```

Defined in src/base/include/icy/packetstream.h:228

> **Inherits:** [`State`](#state)

[State](#state) machine states for [PacketStream](#packetstream).

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`str`](#str) `const` `inline` `override` |  |

---

{#str}

#### str

`const` `inline` `override`

```cpp
inline std::string str(unsigned int id) const override
```

Defined in src/base/include/icy/packetstream.h:254

### Public Static Attributes

| Return | Name | Description |
|--------|------|-------------|
| `unsigned int` | [`None`](#none) `static` `constexpr` |  |
| `unsigned int` | [`Locked`](#locked) `static` `constexpr` |  |
| `unsigned int` | [`Active`](#active) `static` `constexpr` |  |
| `unsigned int` | [`Paused`](#paused) `static` `constexpr` |  |
| `unsigned int` | [`Stopping`](#stopping) `static` `constexpr` |  |
| `unsigned int` | [`Stopped`](#stopped) `static` `constexpr` |  |
| `unsigned int` | [`Closed`](#closed) `static` `constexpr` |  |
| `unsigned int` | [`Error`](#error) `static` `constexpr` |  |

---

{#none}

#### None

`static` `constexpr`

```cpp
unsigned int None = static_cast<unsigned int>()
```

Defined in src/base/include/icy/packetstream.h:245

---

{#locked}

#### Locked

`static` `constexpr`

```cpp
unsigned int Locked = static_cast<unsigned int>()
```

Defined in src/base/include/icy/packetstream.h:246

---

{#active}

#### Active

`static` `constexpr`

```cpp
unsigned int Active = static_cast<unsigned int>()
```

Defined in src/base/include/icy/packetstream.h:247

---

{#paused}

#### Paused

`static` `constexpr`

```cpp
unsigned int Paused = static_cast<unsigned int>()
```

Defined in src/base/include/icy/packetstream.h:248

---

{#stopping}

#### Stopping

`static` `constexpr`

```cpp
unsigned int Stopping = static_cast<unsigned int>()
```

Defined in src/base/include/icy/packetstream.h:249

---

{#stopped}

#### Stopped

`static` `constexpr`

```cpp
unsigned int Stopped = static_cast<unsigned int>()
```

Defined in src/base/include/icy/packetstream.h:250

---

{#closed}

#### Closed

`static` `constexpr`

```cpp
unsigned int Closed = static_cast<unsigned int>()
```

Defined in src/base/include/icy/packetstream.h:251

---

{#error}

#### Error

`static` `constexpr`

```cpp
unsigned int Error = static_cast<unsigned int>()
```

Defined in src/base/include/icy/packetstream.h:252

### Public Types

| Name | Description |
|------|-------------|
| [`Type`](#type)  |  |

---

{#type}

#### Type

```cpp
enum Type
```

Defined in src/base/include/icy/packetstream.h:230

| Value | Description |
|-------|-------------|
| `None` |  |
| `Locked` |  |
| `Active` |  |
| `Paused` |  |
| `Stopping` |  |
| `Stopped` |  |
| `Closed` |  |
| `Error` |  |

{#packetcreationstrategy}

## PacketCreationStrategy

```cpp
#include <icy/packetfactory.h>
```

```cpp
template<class PacketT>
struct PacketCreationStrategy
```

Defined in src/base/include/icy/packetfactory.h:55

> **Inherits:** [`IPacketCreationStrategy`](#ipacketcreationstrategy)

This template class implements an adapter that sits between an SignalBase and an object receiving notifications from it.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`PacketCreationStrategy`](#packetcreationstrategy) `inline` |  |
| `IPacket *` | [`create`](#create) `virtual` `const` `inline` `override` | Attempts to default-construct a `PacketT`, calling its `read()` method. |
| `int` | [`priority`](#priority) `virtual` `const` `inline` `override` |  |

---

{#packetcreationstrategy}

#### PacketCreationStrategy

`inline`

```cpp
inline PacketCreationStrategy(int priority = 0)
```

Defined in src/base/include/icy/packetfactory.h:59

#### Parameters
* `priority` Dispatch priority in the range 0–100. 

#### Exceptions
* `std::logic_error` if priority exceeds 100.

---

{#create}

#### create

`virtual` `const` `inline` `override`

```cpp
virtual inline IPacket * create(const ConstBuffer & buffer, size_t & nread) const override
```

Defined in src/base/include/icy/packetfactory.h:70

Attempts to default-construct a `PacketT`, calling its `read()` method. 
#### Parameters
* `buffer` Raw input data. 

* `nread` Set to the number of bytes consumed when read() succeeds. 

#### Returns
Newly allocated `PacketT` on success, nullptr if read() returns 0.

---

{#priority}

#### priority

`virtual` `const` `inline` `override`

```cpp
virtual inline int priority() const override
```

Defined in src/base/include/icy/packetfactory.h:79

#### Returns
The priority value assigned at construction.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `int` | [`_priority`](#_priority)  |  |

---

{#_priority}

#### _priority

```cpp
int _priority
```

Defined in src/base/include/icy/packetfactory.h:85

{#sharedlibrary}

## SharedLibrary

```cpp
#include <icy/sharedlibrary.h>
```

```cpp
struct SharedLibrary
```

Defined in src/base/include/icy/sharedlibrary.h:24

Loads a shared library at runtime and resolves exported symbols.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
| `bool` | [`open`](#open) `inline` | Opens a shared library. The filename is in utf-8. Returns true on success and false on error. Call `[SharedLibrary::error()](#structicy_1_1SharedLibrary_1a884ec111fdba82e16e31feaaf65bd4fd)` to get the error message. |
| `void` | [`close`](#close) `inline` | Closes the shared library. |
| `bool` | [`sym`](#sym) `inline` | Retrieves a data pointer from a dynamic library. It is legal for a symbol to map to nullptr. Returns 0 on success and -1 if the symbol was not found. |
| `void` | [`setError`](#seterror) `inline` | Reads the last libuv dynamic-linker error, stores it in _error, and throws a std::runtime_error with the combined prefix and error message. |
| `std::string` | [`error`](#error) `const` `inline` | Returns the last error message recorded by [setError()](#structicy_1_1SharedLibrary_1a6ac5f2cc9cb2883df675da47cf844fbf). Empty if no error has occurred. |

---

{#open}

#### open

`inline`

```cpp
inline bool open(const std::string & path)
```

Defined in src/base/include/icy/sharedlibrary.h:29

Opens a shared library. The filename is in utf-8. Returns true on success and false on error. Call `[SharedLibrary::error()](#structicy_1_1SharedLibrary_1a884ec111fdba82e16e31feaaf65bd4fd)` to get the error message.

---

{#close}

#### close

`inline`

```cpp
inline void close()
```

Defined in src/base/include/icy/sharedlibrary.h:39

Closes the shared library.

---

{#sym}

#### sym

`inline`

```cpp
inline bool sym(const char * name, void ** ptr)
```

Defined in src/base/include/icy/sharedlibrary.h:47

Retrieves a data pointer from a dynamic library. It is legal for a symbol to map to nullptr. Returns 0 on success and -1 if the symbol was not found.

---

{#seterror}

#### setError

`inline`

```cpp
inline void setError(const std::string & prefix)
```

Defined in src/base/include/icy/sharedlibrary.h:60

Reads the last libuv dynamic-linker error, stores it in _error, and throws a std::runtime_error with the combined prefix and error message. 
#### Parameters
* `prefix` Human-readable context string prepended to the error detail. 

#### Exceptions
* `std::runtime_error` always.

---

{#error}

#### error

`const` `inline`

```cpp
inline std::string error() const
```

Defined in src/base/include/icy/sharedlibrary.h:72

Returns the last error message recorded by [setError()](#structicy_1_1SharedLibrary_1a6ac5f2cc9cb2883df675da47cf844fbf). Empty if no error has occurred. 
#### Returns
Last error string.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `uv_lib_t` | [`_lib`](#_lib)  |  |
| `std::string` | [`_error`](#_error)  |  |

---

{#_lib}

#### _lib

```cpp
uv_lib_t _lib
```

Defined in src/base/include/icy/sharedlibrary.h:75

---

{#_error}

#### _error

```cpp
std::string _error
```

Defined in src/base/include/icy/sharedlibrary.h:76

{#diagnosticstate}

## DiagnosticState

```cpp
#include <icy/diagnosticmanager.h>
```

```cpp
struct DiagnosticState
```

Defined in src/base/include/icy/diagnosticmanager.h:26

> **Inherits:** [`State`](#state)

[State](#state) definitions for diagnostic providers.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`str`](#str) `const` `inline` `override` | Returns a human-readable string for the given state [ID](#classicy_1_1State_1af5e8f0a00984df441608f8bedaaecea3). |

---

{#str}

#### str

`const` `inline` `override`

```cpp
inline std::string str(unsigned int id) const override
```

Defined in src/base/include/icy/diagnosticmanager.h:40

Returns a human-readable string for the given state [ID](#classicy_1_1State_1af5e8f0a00984df441608f8bedaaecea3). 
#### Parameters
* `id` A [DiagnosticState::Type](#structicy_1_1DiagnosticState_1ad709e58b6422f1c15b4278b86b8de065) value. 

#### Returns
[State](#state) name string. 

#### Exceptions
* `std::logic_error` for unknown IDs.

### Public Types

| Name | Description |
|------|-------------|
| [`Type`](#type)  |  |

---

{#type}

#### Type

```cpp
enum Type
```

Defined in src/base/include/icy/diagnosticmanager.h:28

| Value | Description |
|-------|-------------|
| `None` |  |
| `Checking` |  |
| `Passed` |  |
| `Failed` |  |

{#transactionstate}

## TransactionState

```cpp
#include <icy/packettransaction.h>
```

```cpp
struct TransactionState
```

Defined in src/base/include/icy/packettransaction.h:27

> **Inherits:** [`State`](#state)

[State](#state) machine states for [PacketTransaction](#packettransaction).

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`str`](#str) `const` `inline` |  |

---

{#str}

#### str

`const` `inline`

```cpp
inline std::string str(unsigned int id) const
```

Defined in src/base/include/icy/packettransaction.h:40

#### Parameters
* `id` A `[TransactionState::Type](#structicy_1_1TransactionState_1a3389dd308d8acd3cab977d1826298434)` value. 

#### Returns
Human-readable name of the state.

### Public Types

| Name | Description |
|------|-------------|
| [`Type`](#type)  |  |

---

{#type}

#### Type

```cpp
enum Type
```

Defined in src/base/include/icy/packettransaction.h:29

| Value | Description |
|-------|-------------|
| `Waiting` |  |
| `Running` |  |
| `Success` |  |
| `Cancelled` |  |
| `Failed` |  |

{#ipacketinfo}

## IPacketInfo

```cpp
#include <icy/packet.h>
```

```cpp
struct IPacketInfo
```

Defined in src/base/include/icy/packet.h:33

> **Subclassed by:** [`PacketInfo`](net.md#packetinfo)

An abstract interface for packet sources to provide extra information about packets.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`IPacketInfo`](#ipacketinfo)  | Defaulted constructor. |
| `std::unique_ptr< IPacketInfo >` | [`clone`](#clone) `virtual` `const` | Returns a heap-allocated deep copy of this info object. |

---

{#ipacketinfo}

#### IPacketInfo

```cpp
IPacketInfo() = default
```

Defined in src/base/include/icy/packet.h:35

Defaulted constructor.

---

{#clone}

#### clone

`virtual` `const`

```cpp
virtual std::unique_ptr< IPacketInfo > clone() const
```

Defined in src/base/include/icy/packet.h:40

Returns a heap-allocated deep copy of this info object. 
#### Returns
Owning pointer to the cloned instance.

