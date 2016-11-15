#include "basetests.h"


using std::cout;
using std::cerr;
using std::endl;
using namespace scy;
using namespace scy::test;


int main(int argc, char** argv)
{
    // test::initialize();

    // =========================================================================
    // Signal V2 Tests
    //
    describe("v2 polymorphic signal", new PolymorphicSignalTests);

    // describe("v2 signal", new BasicSignalTests);
    // describe("v2 signal collector vector", new TestCollectorVector);
    // describe("v2 signal collector until0", new TestCollectorUntil0);
    // describe("v2 signal collector while0", new TestCollectorWhile0);
    //
    // describe("v2 async signal", []() {
    //     typedef v2::AsyncSignal < std::string(const std::string & , std::string & ,
    //             std::string, double, int) >
    //         StringTestSignal;
    //     std::string btag = "B";
    //     // empty emission
    //     StringTestSignal sig_string_test;
    //     StringTestSignal::Emission * emi =
    //         sig_string_test.emission("", btag, "", 0.0, 0);
    //     expect(emi->has_value() == false);
    //     expect(emi->pending() == false);
    //     expect(emi->dispatch() == false);
    //     expect(emi->done() == true);
    //     delete emi;
    //     // add simple handler
    //     auto lambda1 = [](const std::string & a, std::string & b, std::string c,
    //         double d, long l) -> std::string {
    //         return "1" + a + b + c + util::format("%.0f%d", d, l);
    //     };
    //     sig_string_test() += (lambda1);
    //     // emission with handler
    //     emi = sig_string_test.emission("A", btag, "C", -3.0, 9);
    //     expect(emi->has_value() == false);
    //     expect(emi->done() == false);
    //     expect(emi->pending() == true);
    //     expect(emi->has_value() == false);
    //     expect(emi->dispatch() == true); // value unreaped
    //     expect(emi->has_value() == true);
    //     expect(emi->get_value() == "1ABC-39"); // fetch and validate value
    //     expect(emi->has_value() == false);
    //     expect(emi->pending() == false);
    //     expect(emi->done() == true);
    //     expect(emi->dispatch() == false);
    //     delete emi;
    //     // add async handler with delay
    //     Mutex handler2_lock;
    //     auto lambda2 = [ & handler2_lock](
    //         const std::string & a, std::string & b, std::string c, double d,
    //             int i) -> std::future < std::string > { // need to turn (std::string &b) into
    //         // (std::string b) copy for deferred
    //         // execution
    //         auto lambda = [ & handler2_lock](const std::string & a, std::string b,
    //             std::string c, double d,
    //             int i) // -> std::string
    //         {
    //             std::this_thread::sleep_for(
    //                 std::chrono::milliseconds(1)); // help race detection
    //             handler2_lock.lock();
    //             handler2_lock.unlock(); // force proper synchronization
    //             return "2" + a + b + c + util::format("%.0f%d", d, i);
    //         };
    //         return std::async(std::launch::async, lambda, a, b, c, d,
    //             i); // execute in seperate thread
    //     };
    //     sig_string_test().connect_future(lambda2);
    //     // emission with handler + async handler
    //     emi = sig_string_test.emission("a", btag, "c", -5, 7);
    //     expect(!emi->done() && !emi->has_value() && emi->pending());
    //     expect(emi->dispatch() == true);
    //     expect(!emi->done());
    //     expect(emi->has_value() == true); // first handler result
    //     expect(emi->get_value() == "1aBc-57"); // fetch and validate value
    //     expect(!emi->done() && !emi->has_value() && emi->pending());
    //     handler2_lock.lock(); // block handler2
    //     expect(emi->dispatch() == true); // second handler started
    //     expect(!emi->done());
    //     expect(emi->has_value() == false); // asynchronous execution blocked
    //     if (0)
    //         emi->get_value(); // test deadlock...
    //     handler2_lock.unlock(); // unblock handler2
    //     bool saved_pending = emi->pending();
    //     while (emi->has_value() == false) // waiting for async handler2 completion
    //     {
    //         expect(saved_pending == false);
    //         scy::sleep(1); // allow for async handler2 completion
    //         saved_pending = emi->pending();
    //     }
    //     expect(emi->pending() == true); // pending, since has_value() == true
    //     expect(emi->get_value() == "2aBc-57"); // synchronize, fetch and validate value
    //     expect(!emi->has_value() && !emi->pending() && emi->done());
    //     delete emi;
    //     // add handler with promise
    //     std::promise < std::string > result3;
    //     auto lambda3 = [ & result3](const std::string & a, std::string & b, std::string c,
    //             double d, int i) // -> std::future<std::string>
    //         {
    //             return result3.get_future();
    //         };
    //     sig_string_test().connect_future(lambda3);
    //     // emission with handler + async handler + promise handler
    //     emi = sig_string_test.emission("_", btag, "_", -7, -6);
    //     expect(!emi->done() && !emi->has_value() && emi->pending());
    //     expect(emi->dispatch() && !emi->done() && emi->has_value());
    //     expect(emi->get_value() == "1_B_-7-6"); // fetch and validate value
    //     expect(!emi->done() && !emi->has_value() && emi->pending());
    //     expect(emi->dispatch() && !emi->done());
    //     expect(emi->get_value() == "2_B_-7-6"); // synchronize with handler2, fetch and validate value
    //     expect(!emi->done() && !emi->has_value() && emi->pending());
    //     expect(emi->dispatch() && !emi->done());
    //     expect(emi->has_value() == false); // promise remains unset
    //     result3.set_value("future");
    //     expect(emi->has_value() == true); // promise set, future resolved
    //     expect(emi->get_value() == "future"); // fetch and validate future
    //     expect(!emi->has_value() && !emi->pending() && emi->done());
    //     delete emi;
    //     emi = nullptr;
    //     // parallel and partial emissions
    //     StringTestSignal::Emission * emi1 =
    //         sig_string_test.emission("(1)", btag, "x", -1, -1);
    //     StringTestSignal::Emission * emi2 =
    //         sig_string_test.emission("(2)", btag, "x", -1, -1);
    //     StringTestSignal::Emission * emi3 =
    //         sig_string_test.emission("(3)", btag, "x", -1, -1);
    //     expect(emi1 -> dispatch() && emi2 -> dispatch() && emi3 -> dispatch());
    //     expect(emi1 -> get_value() == "1(1)Bx-1-1" &&
    //         emi2 -> get_value() == "1(2)Bx-1-1" &&
    //         emi3 -> get_value() == "1(3)Bx-1-1");
    //     delete emi1; // aborting emission
    //     expect(emi2 -> dispatch() && emi3 -> dispatch());
    //     expect(emi2 -> get_value() == "2(2)Bx-1-1" &&
    //         emi3 -> get_value() == "2(3)Bx-1-1");
    //     delete emi3;
    //     if (0)
    //         expect(emi2 -> dispatch()); // avoid future_already_retrieved exception
    //     delete emi2;
    // });
    //
    // describe("signal benchmark: AsyncSignal", []() {
    //     typedef v2::AsyncSignal < void(void * , std::uint64_t) > TestSignal;
    //     TestSignal sig_increment;
    //     sig_increment() += test_counter_add2; {
    //         TestSignal::Emission * tmp = sig_increment.emission(nullptr, 1);
    //         delete tmp;
    //     }
    //     const std::uint64_t start_counter = TestCounter::get();
    //     const std::uint64_t benchstart = time::hrtime();
    //     std::uint64_t i;
    //     for (i = 0; i < 999999; i++) {
    //         TestSignal::Emission * emi = sig_increment.emission(nullptr, 1);
    //         emi->dispatch();
    //         emi->get_value();
    //         delete emi;
    //     }
    //     const std::uint64_t benchdone = time::hrtime();
    //     const std::uint64_t end_counter = TestCounter::get();
    //     expect(end_counter - start_counter == i);
    //
    //     std::cout << util::format("SignalBench: AsyncSignal: %fns per emission (sz=%u)", size_t
    //     (benchdone - benchstart) * 1.0 / size_t (i),
    //               sizeof (sig_increment)) << std::endl;
    // });
    //
    // describe("signal benchmark: v2::Signal", []() {
    //     v2::Signal < void(void * , std::uint64_t) > sig_increment;
    //     sig_increment() += test_counter_add2;
    //     const std::uint64_t start_counter = TestCounter::get();
    //     const std::uint64_t benchstart = time::hrtime();
    //     std::uint64_t i;
    //     for (i = 0; i < 999999; i++) {
    //         sig_increment.emit(nullptr, 1);
    //     }
    //     const std::uint64_t benchdone = time::hrtime();
    //     const std::uint64_t end_counter = TestCounter::get();
    //     expect(end_counter - start_counter == i);
    //
    //     std::cout << util::format("signal benchmark: v2::Signal: %fns per emission (sz=%u)",
    //         size_t (benchdone - benchstart) * 1.0 / size_t (i), sizeof (sig_increment)) << std::endl;
    // });
    //
    // describe("signal benchmark: callback loop", []() {
    //     void( * counter_increment)(void * , std::uint64_t) = test_counter_add2;
    //     const std::uint64_t start_counter = TestCounter::get();
    //     const std::uint64_t benchstart = time::hrtime();
    //     std::uint64_t i;
    //     for (i = 0; i < 999999; i++) {
    //         counter_increment(nullptr, 1);
    //     }
    //     const std::uint64_t benchdone = time::hrtime();
    //     const std::uint64_t end_counter = TestCounter::get();
    //     expect(end_counter - start_counter == i);
    //
    //     std::cout << util::format("signal benchmark: callback loop: %fns per round",
    //         size_t (benchdone - benchstart) * 1.0 / size_t (i)) << std::endl;
    // });

    // // =========================================================================
    // // Buffer
    // //
    // describe("buffer", []() {
    //     ByteOrder orders[2] = { ByteOrder::Host,
    //                             ByteOrder::Network };
    //     for (std::size_t i = 0; i < 2; i++) {
    //         Buffer buffer(1024);
    //         BitReader reader(buffer, orders[i]);
    //         BitWriter writer(buffer, orders[i]);
    //         expect(orders[i] == reader.order());
    //         expect(orders[i] == writer.order());
    //
    //         // Write and read std::uint8_t.
    //         std::uint8_t wu8 = 1;
    //         writer.putU8(wu8);
    //         std::uint8_t ru8;
    //         reader.getU8(ru8);
    //         expect(writer.position() == 1);
    //         expect(reader.position() == 1);
    //         expect(wu8 == ru8);
    //
    //         // Write and read std::uint16_t.
    //         std::uint16_t wu16 = (1 << 8) + 1;
    //         writer.putU16(wu16);
    //         std::uint16_t ru16;
    //         reader.getU16(ru16);
    //         expect(wu16 == ru16);
    //         expect(writer.position() == 3);
    //         expect(reader.position() == 3);
    //
    //         // Write and read UInt24.
    //         std::uint32_t wu24 = (3 << 16) + (2 << 8) + 1;
    //         writer.putU24(wu24);
    //         std::uint32_t ru24;
    //         reader.getU24(ru24);
    //         expect(wu24 == ru24);
    //         expect(writer.position() == 6);
    //         expect(reader.position() == 6);
    //
    //         // Write and read std::uint32_t.
    //         std::uint32_t wu32 = (4 << 24) + (3 << 16) + (2 << 8) + 1;
    //         writer.putU32(wu32);
    //         std::uint32_t ru32;
    //         reader.getU32(ru32);
    //         expect(wu32 == ru32);
    //         expect(writer.position() == 10);
    //         expect(reader.position() == 10);
    //
    //         // Write and read std::uint64_t.
    //         std::uint32_t another32 = (8 << 24) + (7 << 16) + (6 << 8) + 5;
    //         std::uint64_t wu64 = (static_cast<std::uint64_t>(another32) << 32) + wu32;
    //         writer.putU64(wu64);
    //         std::uint64_t ru64;
    //         reader.getU64(ru64);
    //         expect(wu64 == ru64);
    //         expect(writer.position() == 18);
    //         expect(reader.position() == 18);
    //
    //         // Write and read string.
    //         std::string write_string("hello");
    //         writer.put(write_string);
    //         std::string read_string;
    //         reader.get(read_string, write_string.size());
    //         expect(write_string == read_string);
    //         expect(writer.position() == 23);
    //         expect(reader.position() == 23);
    //
    //         // Write and read bytes
    //         char write_bytes[] = "foo";
    //         writer.put(write_bytes, 3);
    //         char read_bytes[3];
    //         reader.get(read_bytes, 3);
    //         for (int i = 0; i < 3; ++i) {
    //           expect(write_bytes[i] == read_bytes[i]);
    //         }
    //         expect(writer.position() == 26);
    //         expect(reader.position() == 26);
    //
    //         // TODO: Test overflow
    //         // try {
    //         //     reader.getU8(ru8);
    //         //     expect(0 && "must throw");
    //         // }
    //         // catch (std::out_of_range& exc) {
    //         // }
    //     }
    // });
    //
    //
    // // =========================================================================
    // // Collection
    // //
    // describe("collection", []() {
    //     NVCollection nvc;
    //     expect(nvc.empty());
    //     expect(nvc.size() == 0);
    //
    //     nvc.set("name", "value");
    //     expect(!nvc.empty());
    //     expect(nvc["name"] == "value");
    //     expect(nvc["Name"] == "value");
    //
    //     nvc.set("name2", "value2");
    //     expect(nvc.get("name2") == "value2");
    //     expect(nvc.get("NAME2") == "value2");
    //
    //     expect(nvc.size() == 2);
    //
    //     try
    //     {
    //         std::string value = nvc.get("name3");
    //         expect(0 && "not found - must throw");
    //     }
    //     catch (std::exception&)
    //     {
    //     }
    //
    //     try
    //     {
    //         std::string value = nvc["name3"];
    //         expect(0 && "not found - must throw");
    //     }
    //     catch (std::exception&)
    //     {
    //     }
    //
    //     expect(nvc.get("name", "default") == "value");
    //     expect(nvc.get("name3", "default") == "default");
    //
    //     expect(nvc.has("name"));
    //     expect(nvc.has("name2"));
    //     expect(!nvc.has("name3"));
    //
    //     nvc.add("name3", "value3");
    //     expect(nvc.get("name3") == "value3");
    //
    //     nvc.add("name3", "value31");
    //
    //     nvc.add("Connection", "value31");
    //
    //     NVCollection::ConstIterator it = nvc.find("Name3");
    //     expect(it != nvc.end());
    //     std::string v1 = it->second;
    //     expect(it->first == "name3");
    //     ++it;
    //     expect(it != nvc.end());
    //     std::string v2 = it->second;
    //     expect(it->first == "name3");
    //
    //     expect((v1 == "value3" && v2 == "value31") || (v1 == "value31" && v2 == "value3"));
    //
    //     nvc.erase("name3");
    //     expect(!nvc.has("name3"));
    //     expect(nvc.find("name3") == nvc.end());
    //
    //     it = nvc.begin();
    //     expect(it != nvc.end());
    //     ++it;
    //     expect(it != nvc.end());
    //     // ++it;
    //     // expect(it == nvc.end());
    //
    //     nvc.clear();
    //     expect(nvc.empty());
    //
    //     expect(nvc.size() == 0);
    // });
    //
    //
    // // =========================================================================
    // // Filesystem
    // //
    // describe("filesystem", []() {
    //     std::string path(scy::getExePath());
    //     cout << "Executable path: " << path << endl;
    //     expect(fs::exists(path));
    //
    //     std::string junkPath(path + "junkname.huh");
    //     cout << "Junk path: " << junkPath << endl;
    //     expect(!fs::exists(junkPath));
    //
    //     std::string dir(fs::dirname(path));
    //     cout << "Dir name: " << dir << endl;
    //     expect(fs::exists(dir));
    //     expect(fs::exists(dir + "/"));
    //     expect(fs::exists(dir + "\\"));
    //     expect(fs::dirname(dir) == dir);
    //     // expect(fs::dirname(dir + "/") == dir);
    //     // expect(fs::dirname(dir + "\\") == dir);
    // });
    //
    //
    // // =========================================================================
    // // Logger
    // //
    // describe("logger", []() {
    //     Logger& logger = Logger::instance();
    //
    //     // Test default synchronous writer
    //     logger.setWriter(new LogWriter);
    //     clock_t start = clock();
    //     for (unsigned i = 0; i < 1000; i++)
    //         TraceL << "test: " << i << endl;
    //     cout << "logger: synchronous test completed after: " << (clock() - start) << endl;
    //
    //     // Test asynchronous writer (approx 10x faster)
    //     logger.setWriter(new AsyncLogWriter);
    //     start = clock();
    //     for (unsigned i = 0; i < 1000; i++)
    //         TraceL << "test: " << i << endl;
    //     cout << "logger: asynchronous test completed after: " << (clock() - start) << endl;
    //
    //     // // Test function logging
    //     // start = clock();
    //     // for (unsigned i = 0; i < 1000; i++)
    //     //     TraceS(this) << "test: " << i << endl;
    //     // cout << "logger: asynchronous function logging completed after: " << (clock() - start) << endl;
    //
    //     // // Test function and mem address logging
    //     // start = clock();
    //     // for (unsigned i = 0; i < 1000; i++)
    //     //     TraceS(this) << "test: " << i << endl;
    //     // cout << "logger: asynchronous function and mem address logging completed after: " << (clock() - start) << endl;
    //
    //     logger.setWriter(nullptr);
    //
    //     // TODO: Test log filtering
    //     // logger.getDefault()->setFilter("scy::*");
    //
    //     // Destory the current Logger instance to ensure no crash
    //     Logger::destroy();
    // });
    //
    //
    // // =========================================================================
    // // Platform
    // //
    // describe("platform", []() {
    //     // TODO: expand test coverage
    //
    //     expect(!scy::getExePath().empty());
    //     expect(!scy::getCwd().empty());
    //
    //     // cout << "executable path: " << scy::getExePath() << endl;
    //     // cout << "current working directory: " << scy::getCwd() << endl;
    // });
    //
    //
    // // =========================================================================
    // // Version String Comparison
    // //
    // describe("version string comparison", []() {
    //     expect((util::Version("3.7.8.0") == util::Version("3.7.8.0")) == true);
    //     expect((util::Version("3.7.8.0") == util::Version("3.7.8")) == true);
    //     expect((util::Version("3.7.8.0") < util::Version("3.7.8")) == false);
    //     expect((util::Version("3.7.9") < util::Version("3.7.8")) == false);
    //     // expect((util::Version("3") < util::Version("3.7.9")) == true);
    //     expect((util::Version("1.7.9") < util::Version("3.1")) == true);
    //
    //     // cout << "Printing version (3.7.8.0): " << util::Version("3.7.8.0") << endl;
    // });
    //
    //
    // // =========================================================================
    // // Thread
    // //
    // describe("thread", []() {
    //     bool ran = false;
    //
    //     Thread async([](void* arg) {
    //         auto ran = reinterpret_cast<bool*>(arg);
    //         scy::sleep(1); // prevent race condition
    //         *ran = true;
    //     }, &ran);
    //
    //     expect(async.started() == true);
    //
    //     async.join();
    //
    //     expect(ran == true);
    //     expect(async.running() == false);
    //     expect(async.started() == false);
    // });
    //
    // // Define class based tests
    // describe("signal", new SignalTest);
    // describe("ipc", new IpcTest);
    // describe("timer", new TimerTest);
    // describe("idler", new IdlerTest);
    // describe("packet stream", new PacketStreamTest);
    // describe("packet stream file io", new PacketStreamIOTest);
    // // describe("multi packet stream", new MultiPacketStreamTest);

    test::runAll();

    return test::finalize();
}


// class Tests
// {
// public:
//     Application& app;
//
//     Tests(Application& app) : app(app)
//     {
//
// #if 0
//         testLogger();
//         testVersionStringComparisonTest();
//         testSignal();
//         runFSTest();
//         testBuffer();
//         testNVCollection();
//         runPluginTest();
//         runPlatformTests();
//         runExceptionTest();
//         runSchedulerTaskTest();
//         testTimer();
//         testIdler();
//         testSyncDelegate();
//         testProcess();
//         testRunner();
//         testThread();
//         testIPC();
//
//         testSyncQueue();
//         testPacketStream();
//         testMultiPacketStream();
//         runPacketSignalTest();
//         runSocketTests();
//         runGarbageCollectorTests();
//         runSignalReceivers();
//         testMultiPacketStream();
// #endif
//     }
//
//     // =========================================================================
//     // Sync Delegate
//     //
//     NullSignal SyncText;
//
//     void testSyncDelegate()
//     {
//         SyncText += syncDelegate(this, &Tests::onSyncSignal);
//
//         Thread async([](void* arg) {
//             cout << "Sending Sync Callback" << endl;
//
//             auto self = reinterpret_cast<Tests*>(arg);
//             self->SyncText.emit(self);
//         }, this);
//
//         scy::sleep(50); // wait for thread
//         expect(!SyncText.delegates().empty());
//
//         uv::runDefaultLoop();
//     }
//
//     void onSyncSignal(void* sender)
//     {
//         // This method is called inside the event loop context.
//
//         expect(sender == this);
//         cout << "Received Sync Callback" << endl;
//
//         // Remove the delegate
//         SyncText -= syncDelegate(this, &Tests::onSyncSignal);
//
//         // Cleanup now to remove the redundant delegate,
//         // and dereference the event loop.
//         SyncText.cleanup();
//         expect(SyncText.delegates().empty());
//     }
//
// #if 0
//     // =========================================================================
//     // Plugin Test
//     //
//     typedef int (*GimmeFiveFunc)();
//
//     void runPluginTest()
//     {
//         DebugL << "Starting" << endl;
//         // TODO: Use getExePath
//         std::string path("D:/dev/projects/Sourcey/LibSourcey/build/install/libs/TestPlugin/TestPlugind.dll");
//
//         try
//         {
//             //
//             // Load the shared library
//             SharedLibrary lib;
//             lib.open(path);
//
//             //
//             // Get plugin descriptor and exports
//             PluginDetails* info;
//             lib.sym("exports", reinterpret_cast<void**>(&info));
//             cout << "Plugin Info: "
//                 << "\n\tAPIVersion: " << info->abiVersion
//                 << "\n\tFileName: " << info->fileName
//                 << "\n\tClassName: " << info->className
//                 << "\n\tPluginName: " << info->pluginName
//                 << "\n\tPluginVersion: " << info->pluginVersion
//                 << endl;
//
//             //
//             // Version checking
//             if (info->abiVersion != SCY_PLUGIN_ABI_VERSION)
//                 throw std::runtime_error(util::format("Module version mismatch. Expected %s, got %s.", SCY_PLUGIN_ABI_VERSION, info->abiVersion));
//
//             //
//             // Instantiate the plugin
//             TestPlugin* plugin = reinterpret_cast<TestPlugin*>(info->initializeFunc());
//
//             //
//             // Run test methods
//             //plugin->setValue("abracadabra");
//             //expect(plugin->sValue() == "abracadabra");
//
//             //
//             // Call a C function
//             GimmeFiveFunc gimmeFive;
//             lib.sym("gimmeFive", reinterpret_cast<void**>(&gimmeFive));
//             expect(gimmeFive() == 5);
//
//             //
//             // Cleanup and close the library
//             cout << "Cleanup" << endl;
//             //delete plugin;
//             cout << "Cleanup 1" << endl;
//             lib.close();
//             cout << "Cleanup 2" << endl;
//         }
//         catch (std::exception& exc)
//         {
//             ErrorL << "Error: " << exc.what() << endl;
//             expect(0);
//         }
//
//         cout << "Ending" << endl;
//     }
// #endif
//
//     // =========================================================================
//     // Packet Signal Tests
//     //
//     PacketSignal BroadcastPacket;
//
//     void onBroadcastPacket(void* sender, DataPacket& packet)
//     {
//         TraceL << "On Packet: " << packet.className() << endl;
//     }
//
//     void runPacketSignalTest()
//     {
//         TraceL << "Running Packet Signal Test" << endl;
//         BroadcastPacket += packetDelegate(this, &Tests::onBroadcastPacket, 0);
//         DataPacket packet;
//         BroadcastPacket.emit(this, packet);
//         //util::pause();
//         TraceL << "Running Packet Signal Test: END" << endl;
//     }
//
//
//     // =========================================================================
//     // Garbage Collector Tests
//     //
//     void runGarbageCollectorTests() {
//         TraceL << "Running Garbage Collector Test" << endl;
//
//         //for (unsigned i = 0; i < 100; i++) {
//             char* ptr = new char[1000];
//
//             Poco::Thread* ptr1 = new Poco::Thread;
//
//             TaskRunner::getDefault().deleteLater<char*>(ptr);
//             //TaskRunner::getDefault().deleteLater<Poco::Thread>(ptr1);
//         //}
//
//         //util::pause();
//         TraceL << "Running Garbage Collector Test: END" << endl;
//     }
//
//     // =========================================================================
//     // Timer Task Tests
//     //
//     void onTimerTask(void* sender)
//     {
//         TraceL << "Timer Task Timout" << endl;
//         ready.set();
//     }
//
//     void runTimerTaskTest()
//     {
//         TraceL << "Running Timer Task Test" << endl;
//         TimerTask* task = new TimerTask(runner, 1000, 1000);
//         task->Timeout += sdelegate(this, &Tests::onTimerTask);
//         task->start();
//         ready.wait();
//         ready.wait();
//         task->destroy();
//         //util::pause();
//         TraceL << "Running Timer Task Test: END" << endl;
//     }
//
//
//     // =========================================================================
//     // Signal Tests
//     //
//     struct SignalBroadcaster
//     {
//         SignalBroadcaster() {}
//         ~SignalBroadcaster() {}
//
//         Signal<int&>    TestSignal;
//     };
//
//     struct SignalReceiver
//     {
//         SignalBroadcaster& klass;
//         SignalReceiver(SignalBroadcaster& klass) : klass(klass)
//         {
//             DebugL << "SignalReceiver: Starting" << endl;
//             klass.TestSignal += sdelegate(this, &SignalReceiver::onSignal);
//         }
//
//         ~SignalReceiver()
//         {
//             DebugL << "SignalReceiver: Destroying" << endl;
//             klass.TestSignal -= sdelegate(this, &SignalReceiver::onSignal);
//         }
//
//         void onSignal(void*, int& value)
//         {
//             DebugL << "SignalReceiver: Callback: " << value << endl;
//         }
//     };
//
//     void runSignalReceivers() {
//         {
//             SignalBroadcaster broadcaster; //("Thread1");
//             {
//                 SignalReceiver receiver(broadcaster);
//             }
//         }
//         //util::pause();
//     }
//
//     // =========================================================================
//     // Exception Test
//     //
//     void runExceptionTest()
//     {
//         try
//         {
//             throw FileException("That's not a file!");
//             expect(0 && "must throw");
//         }
//         catch (FileException& exc)
//         {
//             cout << "Message: " << exc << endl;
//         }
//         catch (Exception&)
//         {
//             expect(0 && "bad cast");
//         }
//
//         try
//         {
//             throw IOException();
//             expect(0 && "must throw");
//         }
//         catch (IOException& exc)
//         {
//             cout << "Message: " << exc << endl;
//             expect(std::string(exc.what()) == "IO error");
//         }
//         catch (Exception&)
//         {
//             expect(0 && "bad cast");
//         }
//     }
//
//     void runLoop() {
//         DebugL << "################logger: Running" << endl;
//         app.run();
//         DebugL << "################logger: Ended" << endl;
//     }
//
//     void runCleanup() {
//         DebugL << "################logger: Finalizing" << endl;
//         app.finalize();
//         DebugL << "################logger: Exiting" << endl;
//     }
//
// };
