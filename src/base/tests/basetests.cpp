#include "basetests.h"


using std::cout;
using std::cerr;
using std::endl;
using namespace scy;
using namespace scy::test;


int main(int argc, char** argv)
{
    Logger::instance().add(new ConsoleChannel("debug", LTrace));
    test::initialize();


    // =========================================================================
    // Signal Benchmarks
    //
    describe("signal class member benchmark", []() {
        Signal<void(std::uint64_t&)> signal;
        SignalCounter counter;
        signal += slot(&counter, &SignalCounter::increment);
        const std::uint64_t benchstart = time::hrtime();
        std::uint64_t i, value = 0;
        for (i = 0; i < 999999; i++) {
            signal.emit(value);
        }
        const std::uint64_t benchdone = time::hrtime();
        expect(value == i);

        std::cout << "signal class member benchmark: "
            << ((benchdone - benchstart) * 1.0 / i) << "ns "
            << "per emission (sz=" << sizeof (signal) << ")"
            << std::endl;
    });

    describe("signal const class member benchmark", []() {
        Signal<void(std::uint64_t&)> signal;
        SignalCounter counter;
        signal += slot(&counter, &SignalCounter::incrementConst);
        const std::uint64_t benchstart = time::hrtime();
        std::uint64_t i, value = 0;
        for (i = 0; i < 999999; i++) {
            signal.emit(value);
        }
        const std::uint64_t benchdone = time::hrtime();
        expect(value == i);

        std::cout << "signal const class member benchmark: "
                  << ((benchdone - benchstart) * 1.0 / i) << "ns "
                  << "per emission (sz=" << sizeof(signal) << ")" << std::endl;
    });

    describe("signal static member benchmark", []() {
        Signal<void(std::uint64_t&)> signal;
        // SignalCounter counter;
        signal += slot(&SignalCounter::incrementStatic);
        // signal += &SignalCounter::incrementStatic;
        const std::uint64_t benchstart = time::hrtime();
        std::uint64_t i, value = 0;
        for (i = 0; i < 999999; i++) {
            signal.emit(value);
        }
        const std::uint64_t benchdone = time::hrtime();
        expect(value == i);

        std::cout << "signal static member benchmark: "
            << ((benchdone - benchstart) * 1.0 / i) << "ns "
            << "per emission (sz=" << sizeof (signal) << ")"
            << std::endl;
    });

    describe("signal free function benchmark", []() {
        Signal<void(std::uint64_t&)> signal;
        signal += signalIncrementFree;
        const std::uint64_t benchstart = time::hrtime();
        std::uint64_t i, value = 0;
        for (i = 0; i < 999999; i++) {
            signal.emit(value);
        }
        const std::uint64_t benchdone = time::hrtime();
        expect(value == i);

        std::cout << "signal free function benchmark: "
            << ((benchdone - benchstart) * 1.0 / i) << "ns "
            << "per emission (sz=" << sizeof (signal) << ")"
            << std::endl;
    });

    // =========================================================================
    // Buffer
    //
    describe("buffer", []() {
        ByteOrder orders[2] = { ByteOrder::Host,
                                ByteOrder::Network };
        for (std::size_t i = 0; i < 2; i++) {
            Buffer buffer(1024);
            BitReader reader(buffer, orders[i]);
            BitWriter writer(buffer, orders[i]);
            expect(orders[i] == reader.order());
            expect(orders[i] == writer.order());

            // Write and read std::uint8_t.
            std::uint8_t wu8 = 1;
            writer.putU8(wu8);
            std::uint8_t ru8;
            reader.getU8(ru8);
            expect(writer.position() == 1);
            expect(reader.position() == 1);
            expect(wu8 == ru8);

            // Write and read std::uint16_t.
            std::uint16_t wu16 = (1 << 8) + 1;
            writer.putU16(wu16);
            std::uint16_t ru16;
            reader.getU16(ru16);
            expect(wu16 == ru16);
            expect(writer.position() == 3);
            expect(reader.position() == 3);

            // Write and read UInt24.
            std::uint32_t wu24 = (3 << 16) + (2 << 8) + 1;
            writer.putU24(wu24);
            std::uint32_t ru24;
            reader.getU24(ru24);
            expect(wu24 == ru24);
            expect(writer.position() == 6);
            expect(reader.position() == 6);

            // Write and read std::uint32_t.
            std::uint32_t wu32 = (4 << 24) + (3 << 16) + (2 << 8) + 1;
            writer.putU32(wu32);
            std::uint32_t ru32;
            reader.getU32(ru32);
            expect(wu32 == ru32);
            expect(writer.position() == 10);
            expect(reader.position() == 10);

            // Write and read std::uint64_t.
            std::uint32_t another32 = (8 << 24) + (7 << 16) + (6 << 8) + 5;
            std::uint64_t wu64 = (static_cast<std::uint64_t>(another32) <<
            32) + wu32;
            writer.putU64(wu64);
            std::uint64_t ru64;
            reader.getU64(ru64);
            expect(wu64 == ru64);
            expect(writer.position() == 18);
            expect(reader.position() == 18);

            // Write and read string.
            std::string write_string("hello");
            writer.put(write_string);
            std::string read_string;
            reader.get(read_string, write_string.size());
            expect(write_string == read_string);
            expect(writer.position() == 23);
            expect(reader.position() == 23);

            // Write and read bytes
            char write_bytes[] = "foo";
            writer.put(write_bytes, 3);
            char read_bytes[3];
            reader.get(read_bytes, 3);
            for (int x = 0; x < 3; ++x) {
              expect(write_bytes[x] == read_bytes[x]);
            }
            expect(writer.position() == 26);
            expect(reader.position() == 26);

            // TODO: Test overflow
            // try {
            //     reader.getU8(ru8);
            //     expect(0 && "must throw");
            // }
            // catch (std::out_of_range& exc) {
            // }
        }
    });


    // =========================================================================
    // Collection
    //
    describe("collection", []() {
        NVCollection nvc;
        expect(nvc.empty());
        expect(nvc.size() == 0);

        nvc.set("name", "value");
        expect(!nvc.empty());
        expect(nvc["name"] == "value");
        expect(nvc["Name"] == "value");

        nvc.set("name2", "value2");
        expect(nvc.get("name2") == "value2");
        expect(nvc.get("NAME2") == "value2");

        expect(nvc.size() == 2);

        try {
            std::string value = nvc.get("name3");
            expect(0 && "not found - must throw");
        }
        catch (std::exception&) {
        }

        try {
            std::string value = nvc["name3"];
            expect(0 && "not found - must throw");
        }
        catch (std::exception&) {
        }

        expect(nvc.get("name", "default") == "value");
        expect(nvc.get("name3", "default") == "default");

        expect(nvc.has("name"));
        expect(nvc.has("name2"));
        expect(!nvc.has("name3"));

        nvc.add("name3", "value3");
        expect(nvc.get("name3") == "value3");

        nvc.add("name3", "value31");

        nvc.add("Connection", "value31");

        NVCollection::ConstIterator it = nvc.find("Name3");
        expect(it != nvc.end());
        std::string v1 = it->second;
        expect(it->first == "name3");
        ++it;
        expect(it != nvc.end());
        std::string v2 = it->second;
        expect(it->first == "name3");

        expect((v1 == "value3" && v2 == "value31") || (v1 == "value31" && v2
        == "value3"));

        nvc.erase("name3");
        expect(!nvc.has("name3"));
        expect(nvc.find("name3") == nvc.end());

        it = nvc.begin();
        expect(it != nvc.end());
        ++it;
        expect(it != nvc.end());
        // ++it;
        // expect(it == nvc.end());

        nvc.clear();
        expect(nvc.empty());

        expect(nvc.size() == 0);
    });


    // =========================================================================
    // Filesystem
    //
    describe("filesystem", []() {
        std::string path(scy::getExePath());
        cout << "Executable path: " << path << endl;
        expect(fs::exists(path));

        std::string junkPath(path + "junkname.huh");
        cout << "Junk path: " << junkPath << endl;
        expect(!fs::exists(junkPath));

        std::string dir(fs::dirname(path));
        cout << "Dir name: " << dir << endl;
        expect(fs::exists(dir));
        expect(fs::exists(dir + "/"));
        expect(fs::exists(dir + "\\"));
        expect(fs::dirname(dir) == dir);
        // expect(fs::dirname(dir + "/") == dir);
        // expect(fs::dirname(dir + "\\") == dir);
    });


    // =========================================================================
    // Logger
    //
    describe("logger", []() {
        Logger& logger = Logger::instance();

        // Test default synchronous writer
        logger.setWriter(new LogWriter);
        clock_t start = clock();
        for (unsigned i = 0; i < 1000; i++)
            TraceL << "test: " << i << endl;
        cout << "logger: synchronous test completed after: " << (clock() -
        start) << endl;

        // Test asynchronous writer (approx 10x faster)
        logger.setWriter(new AsyncLogWriter);
        start = clock();
        for (unsigned i = 0; i < 1000; i++)
            TraceL << "test: " << i << endl;
        cout << "logger: asynchronous test completed after: " << (clock() -
        start) << endl;

        // // Test function logging
        // start = clock();
        // for (unsigned i = 0; i < 1000; i++)
        //     TraceS(this) << "test: " << i << endl;
        // cout << "logger: asynchronous function logging completed after: "
        //      << (clock() - start) << endl;
        //
        // // Test function and mem address logging
        // start = clock();
        // for (unsigned i = 0; i < 1000; i++)
        //     TraceS(this) << "test: " << i << endl;
        // cout << "logger: asynchronous function and mem address logging completed after: "
        //      << (clock() - start) << endl;

        logger.setWriter(nullptr);

        // TODO: Test log filtering
        // logger.getDefault()->setFilter("scy::*");

        // Destory the current Logger instance to ensure no crash
        Logger::destroy();
    });


    // =========================================================================
    // Platform
    //
    describe("platform", []() {
        // TODO: expand test coverage

        expect(!scy::getExePath().empty());
        expect(!scy::getCwd().empty());

        // cout << "executable path: " << scy::getExePath() << endl;
        // cout << "current working directory: " << scy::getCwd() << endl;
    });


    // =========================================================================
    // Version String Comparison
    //
    describe("version string comparison", []() {
        expect((util::Version("3.7.8.0") == util::Version("3.7.8.0")) == true);
        expect((util::Version("3.7.8.0") == util::Version("3.7.8")) == true);
        expect((util::Version("3.7.8.0") < util::Version("3.7.8")) == false);
        expect((util::Version("3.7.9") < util::Version("3.7.8")) == false);
        // expect((util::Version("3") < util::Version("3.7.9")) == true);
        expect((util::Version("1.7.9") < util::Version("3.1")) == true);

        cout << "Printing version (3.7.8.0): " << util::Version("3.7.8.0") << endl;
    });


    // =========================================================================
    // Idler
    //
    describe("idler", []() {
        int counter = 0;
        bool ran = false;

        // Create the idler with a lambda
		Idler idler;
        idler.start([&]() {
            std::cout << "On idle: " << counter << std::endl;
            if (++counter == 10) {
                ran = true;
                expect(idler.running() == true);
                idler.cancel();
                expect(idler.cancelled() == true);
            }
        });

        // Make the idler reference the event loop
        idler.handle().ref();

        // Run the loop
        uv::runDefaultLoop();

        // Check variables
        expect(ran == true);
        expect(counter == 10);
        expect(idler.running() == false);

        // Reinitialize the idler with veradic args
        ran = false;
        idler.start([](Idler* idler, bool* ran) {
            *ran = true;
            expect(idler->running() == true);
            idler->cancel();
        }, &idler, &ran);

        // Run the loop again
        uv::runDefaultLoop();

        // Check variables
        expect(ran == true);
        expect(idler.running() == false);

        // Bind to class member
        // idler.start(std::bind(&IdlerTest::idlerCallback, this));
    });


    // =========================================================================
    // Thread
    //
    describe("thread", []() {
        bool ran = false;

    		Thread t1;
    		t1.start([&]() {
            ran = true;
            expect(t1.running() == true);
        });
        t1.join();
        expect(ran == true);
        expect(t1.running() == false);

        // Reuse the same thread container
        ran = false;
        t1.start([&]() {
            ran = true;
            expect(t1.running() == true);
        });
        t1.join();
        expect(ran == true);
        expect(t1.running() == false);
    });

    // Define class based tests
    describe("signal", new SignalTest);
    describe("ipc", new IpcTest);
    describe("timer", new TimerTest);
    describe("packet stream", new PacketStreamTest);
    describe("packet stream file io", new PacketStreamIOTest);
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
//     //
//     =========================================================================
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
//             self->SyncText.emit(/*self*/);
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
//     //
//     =========================================================================
//     // Plugin Test
//     //
//     typedef int (*GimmeFiveFunc)();
//
//     void runPluginTest()
//     {
//         DebugL << "Starting" << endl;
//         // TODO: Use getExePath
//         std::string
//         path("D:/dev/projects/Sourcey/LibSourcey/build/install/libs/TestPlugin/TestPlugind.dll");
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
//                 throw std::runtime_error(util::format("Module version
//                 mismatch. Expected %s, got %s.", SCY_PLUGIN_ABI_VERSION,
//                 info->abiVersion));
//
//             //
//             // Instantiate the plugin
//             TestPlugin* plugin =
//             reinterpret_cast<TestPlugin*>(info->initializeFunc());
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
//     //
//     =========================================================================
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
//         BroadcastPacket += packetSlot(this, &Tests::onBroadcastPacket, 0);
//         DataPacket packet;
//         BroadcastPacket.emit(/*this, */packet);
//         //util::pause();
//         TraceL << "Running Packet Signal Test: END" << endl;
//     }
//
//
//     //
//     =========================================================================
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
//     //
//     =========================================================================
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
//     //
//     =========================================================================
//     // Signal Tests
//     //
//     struct SignalBroadcaster
//     {
//         SignalBroadcaster() {}
//         ~SignalBroadcaster() {}
//
//         Signal<void(std::uint64_t&)>    TestSignal;
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
//     //
//     =========================================================================
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
