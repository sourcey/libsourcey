#include "scy/base.h"
#include "scy/logger.h"
#include "scy/idler.h"
#include "scy/signal.h"
#include "scy/buffer.h"
#include "scy/platform.h"
#include "scy/collection.h"
#include "scy/application.h"
#include "scy/packetstream.h"
#include "scy/packetqueue.h"
#include "scy/sharedlibrary.h"
#include "scy/filesystem.h"
#include "scy/process.h"
#include "scy/timer.h"
#include "scy/ipc.h"
#include "scy/util.h"

#include <assert.h>


using std::cout;
using std::cerr;
using std::endl;
using namespace scy;


namespace scy {


class Tests
{
public:
    Application& app;

    Tests(Application& app) : app(app)
    {

#if 0
        testLogger();
        testVersionStringComparison();
        testSignal();
        runFSTest();
        testBuffer();
        testNVCollection();
        runPluginTest();
        runPlatformTests();
        runExceptionTest();
        runSchedulerTaskTest();
        testTimer();
        testIdler();
        testSyncDelegate();
        testProcess();
        testRunner();
        testThread();

        testSyncQueue();
        testPacketStream();
        testMultiPacketStream();
        runPacketSignalTest();
        runSocketTests();
        runGarbageCollectorTests();
        runSignalReceivers();
        testMultiPacketStream();
#endif
        testIPC();

        //scy::pause();
    }

    void testBuffer()
    {
        ByteOrder orders[2] = { ByteOrder::Host,
                                ByteOrder::Network };
        for (std::size_t i = 0; i < 2; i++) {
            Buffer buffer(1024);
            BitReader reader(buffer, orders[i]);
            BitWriter writer(buffer, orders[i]);
            assert(orders[i] == reader.order());
            assert(orders[i] == writer.order());

            // Write and read std::uint8_t.
            std::uint8_t wu8 = 1;
            writer.putU8(wu8);
            std::uint8_t ru8;
            reader.getU8(ru8);
            assert(wu8 == ru8);
            assert(writer.position() == 1);
            assert(reader.position() == 1);

            // Write and read std::uint16_t.
            std::uint16_t wu16 = (1 << 8) + 1;
            writer.putU16(wu16);
            std::uint16_t ru16;
            reader.getU16(ru16);
            assert(wu16 == ru16);
            assert(writer.position() == 3);
            assert(reader.position() == 3);

            // Write and read UInt24.
            std::uint32_t wu24 = (3 << 16) + (2 << 8) + 1;
            writer.putU24(wu24);
            std::uint32_t ru24;
            reader.getU24(ru24);
            assert(wu24 == ru24);
            assert(writer.position() == 6);
            assert(reader.position() == 6);

            // Write and read std::uint32_t.
            std::uint32_t wu32 = (4 << 24) + (3 << 16) + (2 << 8) + 1;
            writer.putU32(wu32);
            std::uint32_t ru32;
            reader.getU32(ru32);
            assert(wu32 == ru32);
            assert(writer.position() == 10);
            assert(reader.position() == 10);

            // Write and read std::uint64_t.
            std::uint32_t another32 = (8 << 24) + (7 << 16) + (6 << 8) + 5;
            std::uint64_t wu64 = (static_cast<std::uint64_t>(another32) << 32) + wu32;
            writer.putU64(wu64);
            std::uint64_t ru64;
            reader.getU64(ru64);
            assert(wu64 == ru64);
            assert(writer.position() == 18);
            assert(reader.position() == 18);

            // Write and read string.
            std::string write_string("hello");
            writer.put(write_string);
            std::string read_string;
            reader.get(read_string, write_string.size());
            assert(write_string == read_string);
            assert(writer.position() == 23);
            assert(reader.position() == 23);

            // Write and read bytes
            char write_bytes[] = "foo";
            writer.put(write_bytes, 3);
            char read_bytes[3];
            reader.get(read_bytes, 3);
            for (int i = 0; i < 3; ++i) {
              assert(write_bytes[i] == read_bytes[i]);
            }
            assert(writer.position() == 26);
            assert(reader.position() == 26);

            // TODO: Test overflow

            /*
            try {
                reader.getU8(ru8);
                assert(0 && "must throw");
            }
            catch (std::out_of_range& exc) {
            }
            */
        }
    }


    // ============================================================================
    // Signal Test
    //
    Signal<int&> TestSignal;

    void testSignal()
    {
        int val = 0;
        TestSignal += sdelegate(this, &Tests::testSignalCallback);
        TestSignal += delegate(this, &Tests::testSignalCallbackNoSender);
        TestSignal.emit(this, val);
        assert(val == 2);
    }

    void testSignalCallback(void* sender, int& val)
    {
        assert(sender == this);
        val++;
    }

    void testSignalCallbackNoSender(int& val)
    {
        val++;
    }


    // ============================================================================
    // Collection Test
    //
    void testNVCollection()
    {
        NVCollection nvc;
        assert(nvc.empty());
        assert(nvc.size() == 0);

        nvc.set("name", "value");
        assert(!nvc.empty());
        assert(nvc["name"] == "value");
        assert(nvc["Name"] == "value");

        nvc.set("name2", "value2");
        assert(nvc.get("name2") == "value2");
        assert(nvc.get("NAME2") == "value2");

        assert(nvc.size() == 2);

        try
        {
            std::string value = nvc.get("name3");
            assert(0 && "not found - must throw");
        }
        catch (std::exception&)
        {
        }

        try
        {
            std::string value = nvc["name3"];
            assert(0 && "not found - must throw");
        }
        catch (std::exception&)
        {
        }

        assert(nvc.get("name", "default") == "value");
        assert(nvc.get("name3", "default") == "default");

        assert(nvc.has("name"));
        assert(nvc.has("name2"));
        assert(!nvc.has("name3"));

        nvc.add("name3", "value3");
        assert(nvc.get("name3") == "value3");

        nvc.add("name3", "value31");

        nvc.add("Connection", "value31");

        NVCollection::ConstIterator it = nvc.find("Name3");
        assert(it != nvc.end());
        std::string v1 = it->second;
        assert(it->first == "name3");
        ++it;
        assert(it != nvc.end());
        std::string v2 = it->second;
        assert(it->first == "name3");

        assert((v1 == "value3" && v2 == "value31") || (v1 == "value31" && v2 == "value3"));

        nvc.erase("name3");
        assert(!nvc.has("name3"));
        assert(nvc.find("name3") == nvc.end());

        it = nvc.begin();
        assert(it != nvc.end());
        ++it;
        assert(it != nvc.end());
        ++it;
        assert(it == nvc.end());

        nvc.clear();
        assert(nvc.empty());

        assert(nvc.size() == 0);
    }


    // ============================================================================
    // FileSystem Test
    //
    void runFSTest()
    {
        std::string path(scy::getExePath());
        DebugL << "Executable path: " << path << endl;
        assert(fs::exists(path));

        std::string junkPath(path + "junkname.huh");
        DebugL << "Junk path: " << junkPath << endl;
        assert(!fs::exists(junkPath));

        std::string dir(fs::dirname(path));
        DebugL << "Dir name: " << dir << endl;
        assert(fs::exists(dir));
        assert(fs::exists(dir + "/"));
        assert(fs::exists(dir + "\\"));
        assert(fs::dirname(dir) == dir);
        assert(fs::dirname(dir + "/") == dir);
        assert(fs::dirname(dir + "\\") == dir);
    }


    // ============================================================================
    // Platform Test
    //
    void runPlatformTests()
    {
        cout << "executable path: " << scy::getExePath() << endl;
        cout << "current working directory: " << scy::getCwd() << endl;
    }

    // ============================================================================
    // Logger Test
    //
    void testLogger()
    {
        Logger& logger = Logger::instance();

        // Test default synchronous writer
        logger.setWriter(new LogWriter);
        clock_t start = clock();
        for (unsigned i = 0; i < 1000; i++)
            TraceL << "Test message: " << i << endl;
        cout << "#### synchronous test completed after: " << (clock() - start) << endl;

        // Test asynchronous writer (approx 10x faster)
        logger.setWriter(new AsyncLogWriter);
        start = clock();
        for (unsigned i = 0; i < 1000; i++)
            TraceL << "Test message: " << i << endl;
        cout << "#### asynchronous test completed after: " << (clock() - start) << endl;

        // Test function logging
        start = clock();
        for (unsigned i = 0; i < 1000; i++)
            TraceS(this) << "Test message: " << i << endl;
        cout << "#### asynchronous function logging completed after: " << (clock() - start) << endl;

        // Test function and mem address logging
        start = clock();
        for (unsigned i = 0; i < 1000; i++)
            TraceS(this) << "Test message: " << i << endl;
        cout << "#### asynchronous function and mem address logging completed after: " << (clock() - start) << endl;


        logger.getDefault()->setFilter("scy::*");
    }


    // ============================================================================
    // Process Test
    //
    void testProcess()
    {
        try
        {
            Process proc;

            char* args[3];
            args[0] = "C:/Windows/notepad.exe";
            args[1] = "runspot";
            args[2] = NULL;

            proc.options.args = args;
            proc.options.file = args[0];
            proc.onexit = std::bind(&Tests::processExit, this, std::placeholders::_1);
            proc.spawn();

            runLoop();
        }
        catch (std::exception& exc)
        {
            cerr << "Process error: " << exc.what() << endl;
            assert(0);
        }
    }

    void processExit(std::int64_t exitStatus)
    {
        cout << "On process exit: " << exitStatus << endl;
    }


    // ============================================================================
    // Thread Tests
    //
    bool threadRan;

    void testThread()
    {
        threadRan = false;
        Thread async([](void* arg) {
            auto self = reinterpret_cast<Tests*>(arg);
            self->threadRan = true;
        }, this);

        while (!threadRan) {
            scy::sleep(10); // wait for thread
        }

        cout << "Thread Ran" << endl;
        assert(async.started());
        assert(!async.running());
    }


    /*
    // ============================================================================
    // Sync Delegate
    //
    NullSignal SyncText;

    void testSyncDelegate()
    {
        SyncText += syncDelegate(this, &Tests::onSyncSignal);

        Thread async([](void* arg) {
            cout << "Sending Sync Callback" << endl;

            auto self = reinterpret_cast<Tests*>(arg);
            self->SyncText.emit(self);
        }, this);

        scy::sleep(50); // wait for thread
        assert(!SyncText.delegates().empty());

        runLoop();
    }

    void onSyncSignal(void* sender)
    {
        // This method is called inside the event loop context.

        assert(sender == this);
        cout << "Received Sync Callback" << endl;

        // Remove the delegate
        SyncText -= syncDelegate(this, &Tests::onSyncSignal);

        // Cleanup now to remove the redundant delegate,
        // and dereference the event loop.
        SyncText.cleanup();
        assert(SyncText.delegates().empty());
    }
    */


    // ============================================================================
    // Timer Test
    //
    const static int numTimerTicks = 5;
    bool timerRestarted;

    void testTimer()
    {
        cout << "Starting" << endl;
        Timer timer;
        //timer.Timeout += sdelegate(this, &Tests::timerCallback);
        timer.start(10, 10);

        timerRestarted = false;

        runLoop();
        cout << "Ending" << endl;
    }

    void timerCallback(void* sender)
    {
        auto timer = reinterpret_cast<Timer*>(sender);
        cout << "On timeout: " << timer->count() << endl;
        if (timer->count() == numTimerTicks) {
            if (!timerRestarted) {
                timerRestarted = true;
                timer->restart(); // restart once, count returns to 0
            }
            else
                timer->stop(); // event loop will be released
        }
    }

    // ============================================================================
    // Idler Test
    //
    const static int wantIdlerTicks = 5;
    int idlerTicks;
    Idler idler;

    void testIdler()
    {
        idlerTicks = 0;
        idler.start(std::bind(&Tests::idlerCallback, this));
        runLoop();
    }

    void idlerCallback()
    {
        cout << "On idle" << endl;
        if (++idlerTicks == numTimerTicks) {
            idler.cancel(); // event loop will be released
        }
    }


    // ============================================================================
    // IPC Test
    //
    const static int want_x_ipc_callbacks = 5;
    int num_ipc_callbacks;

    void testIPC()
    {
        cout << "Test IPC" << endl;
        num_ipc_callbacks = 0;
        ipc::SyncQueue<> ipc;
        ipc.push(new ipc::Action(std::bind(&Tests::ipcCallback, this, std::placeholders::_1), &ipc, "test1"));
        ipc.push(new ipc::Action(std::bind(&Tests::ipcCallback, this, std::placeholders::_1), &ipc, "test2"));
        ipc.push(new ipc::Action(std::bind(&Tests::ipcCallback, this, std::placeholders::_1), &ipc, "test3"));
        ipc.push(new ipc::Action(std::bind(&Tests::ipcCallback, this, std::placeholders::_1), &ipc, "test4"));
        ipc.push(new ipc::Action(std::bind(&Tests::ipcCallback, this, std::placeholders::_1), &ipc, "test5"));
        runLoop();
        cout << "Test IPC: OK" << endl;
    }

    void ipcCallback(const ipc::Action& action)
    {
        cout << "Got IPC callback: " << action.data << endl;
        if (++num_ipc_callbacks == want_x_ipc_callbacks)
            reinterpret_cast<ipc::Queue<>*>(action.arg)->close();
    }

    // ============================================================================
    // SyncQueue Test
    //
    void testSyncQueue()
    {
        runLoop();
    }

    // ============================================================================
    // Packet Stream Tests
    //
    struct TestPacketSource: public PacketSource, public async::Startable
    {
        Thread runner;
        //Idler runner;
        PacketSignal emitter;

        TestPacketSource() :
            PacketSource(emitter)
        {
            runner.setRepeating(true);
        }

        void start()
        {
            DebugS(this) << "Start" << endl;
            runner.start([](void* arg) {
                auto self = reinterpret_cast<TestPacketSource*>(arg);
                DebugL << "Emitting" << endl;
                RawPacket p("hello", 5);
                self->emitter.emit(self, p);
            }, this);
        }

        void stop()
        {
            DebugS(this) << "Stop" << endl;
            runner.cancel();
            //runner.close();
            DebugS(this) << "Stop: OK" << endl;
        }
    };

    struct TestPacketProcessor: public PacketProcessor
    {
        PacketSignal emitter;

        TestPacketProcessor() :
            PacketProcessor(emitter)
        {
        }

        void process(IPacket& packet)
        {
            DebugS(this) << "Process: " << packet.className() << endl;
            emit(packet);
        }
    };

    void onPacketStreamOutput(void* sender, IPacket& packet)
    {
        DebugS(this) << ">>>>>>>>>>> On packet: " << packet.className() << endl;
    }

    void testPacketStream()
    {
        PacketStream stream;
        //stream.setRunner(std::make_shared<Thread>());
        stream.attachSource(new TestPacketSource, true, true);
        //stream.attach(new AsyncPacketQueue, 0, true);
        stream.attach(new TestPacketProcessor, 1, true);
        //stream.attach(new SyncPacketQueue, 2, true);
        stream.synchronizeOutput(uv::defaultLoop());
        //stream.emitter += packetDelegate(this, &Tests::onPacketStreamOutput);
        stream.start();

        // TODO: Test pause/resume functionality

        app.waitForShutdown([](void* arg) {
            auto stream = reinterpret_cast<PacketStream*>(arg);
            DebugL << "########## Shutdown" << endl;
            stream->close();
            //reinterpret_cast<TestPacketSource*>(stream->base().sources()[0].ptr)->stop();
            DebugL << "########## Shutdown: After" << endl;
        }, &stream);

        DebugL << "########## Exiting" << endl;
        stream.close();
    }

    void onChildPacketStreamOutput(void* sender, IPacket& packet)
    {
        DebugS(this) << ">>>>>>>>>>> On child packet: " << packet.className() << endl;
    }

    struct ChildStreams
    {
        PacketStream* s1;
        PacketStream* s2;
        PacketStream* s3;
    };


    void testMultiPacketStream()
    {
        PacketStream stream;
        //stream.setRunner(std::make_shared<Thread>());
        stream.attachSource(new TestPacketSource, true, true);
        //stream.attach(new AsyncPacketQueue, 0, true);
        stream.attach(new TestPacketProcessor, 1, true);
        //stream.emitter += packetDelegate(this, &Tests::onPacketStreamOutput);
        stream.start();

        // The second PacketStream receives packets from the first one
        // and synchronizes output packets with the default event loop.
        ChildStreams children;
        children.s1 = new PacketStream;
        //children.s1->setRunner(std::make_shared<Idler>()); // Use Idler
        children.s1->attachSource(stream.emitter);
        children.s1->attach(new AsyncPacketQueue, 0, true);
        children.s1->attach(new SyncPacketQueue, 1, true);
        children.s1->synchronizeOutput(uv::defaultLoop());
        //children.s1->emitter += packetDelegate(this, &Tests::onChildPacketStreamOutput);
        children.s1->start();

        children.s2 = new PacketStream;
        children.s2->attachSource(stream.emitter);
        children.s2->attach(new AsyncPacketQueue, 0, true);
        children.s2->synchronizeOutput(uv::defaultLoop());
        //children.s2->emitter += packetDelegate(this, &Tests::onChildPacketStreamOutput);
        children.s2->start();

        children.s3 = new PacketStream;
        children.s3->attachSource(stream.emitter);
        children.s3->attach(new AsyncPacketQueue, 0, true);
        //children.s3->synchronizeOutput(uv::defaultLoop());
        //children.s3->emitter += packetDelegate(this, &Tests::onChildPacketStreamOutput);
        children.s3->start();

        app.waitForShutdown([](void* arg) {
            auto streams = reinterpret_cast<ChildStreams*>(arg);
            //streams->s1->attachSource(stream.emitter);
            if (streams->s1) delete streams->s1;
            if (streams->s2) delete streams->s2;
            if (streams->s3) delete streams->s3;
            TraceL << "DESTROYED *********************************************************" << endl;
        }, &children);

        TraceS(this) << "ENDING *********************************************************" << endl;
    }


    /*
#if 0
    // ============================================================================
    // Plugin Test
    //
    typedef int (*GimmeFiveFunc)();

    void runPluginTest()
    {
        DebugL << "Starting" << endl;
        // TODO: Use getExePath
        std::string path("D:/dev/projects/Sourcey/LibSourcey/build/install/libs/TestPlugin/TestPlugind.dll");

        try
        {
            //
            // Load the shared library
            SharedLibrary lib;
            lib.open(path);

            //
            // Get plugin descriptor and exports
            PluginDetails* info;
            lib.sym("exports", reinterpret_cast<void**>(&info));
            cout << "Plugin Info: "
                << "\n\tAPIVersion: " << info->abiVersion
                << "\n\tFileName: " << info->fileName
                << "\n\tClassName: " << info->className
                << "\n\tPluginName: " << info->pluginName
                << "\n\tPluginVersion: " << info->pluginVersion
                << endl;

            //
            // Version checking
            if (info->abiVersion != SCY_PLUGIN_ABI_VERSION)
                throw std::runtime_error(util::format("Module version mismatch. Expected %s, got %s.", SCY_PLUGIN_ABI_VERSION, info->abiVersion));

            //
            // Instantiate the plugin
            TestPlugin* plugin = reinterpret_cast<TestPlugin*>(info->initializeFunc());

            //
            // Run test methods
            //plugin->setValue("abracadabra");
            //assert(plugin->sValue() == "abracadabra");

            //
            // Call a C function
            GimmeFiveFunc gimmeFive;
            lib.sym("gimmeFive", reinterpret_cast<void**>(&gimmeFive));
            assert(gimmeFive() == 5);

            //
            // Cleanup and close the library
            cout << "Cleanup" << endl;
            //delete plugin;
            cout << "Cleanup 1" << endl;
            lib.close();
            cout << "Cleanup 2" << endl;
        }
        catch (std::exception& exc)
        {
            ErrorL << "Error: " << exc.what() << endl;
            assert(0);
        }

        cout << "Ending" << endl;
    }
#endif

    // ============================================================================
    // Packet Signal Tests
    //
    PacketSignal BroadcastPacket;

    void onBroadcastPacket(void* sender, DataPacket& packet)
    {
        TraceL << "On Packet: " << packet.className() << endl;
    }

    void runPacketSignalTest()
    {
        TraceL << "Running Packet Signal Test" << endl;
        BroadcastPacket += packetDelegate(this, &Tests::onBroadcastPacket, 0);
        DataPacket packet;
        BroadcastPacket.emit(this, packet);
        //util::pause();
        TraceL << "Running Packet Signal Test: END" << endl;
    }


    // ============================================================================
    // Garbage Collector Tests
    //
    void runGarbageCollectorTests() {
        TraceL << "Running Garbage Collector Test" << endl;

        //for (unsigned i = 0; i < 100; i++) {
            char* ptr = new char[1000];

            Poco::Thread* ptr1 = new Poco::Thread;

            TaskRunner::getDefault().deleteLater<char*>(ptr);
            //TaskRunner::getDefault().deleteLater<Poco::Thread>(ptr1);
        //}

        //util::pause();
        TraceL << "Running Garbage Collector Test: END" << endl;
    }

    // ============================================================================
    // Timer Task Tests
    //
    void onTimerTask(void* sender)
    {
        TraceL << "Timer Task Timout" << endl;
        ready.set();
    }

    void runTimerTaskTest()
    {
        TraceL << "Running Timer Task Test" << endl;
        TimerTask* task = new TimerTask(runner, 1000, 1000);
        task->Timeout += sdelegate(this, &Tests::onTimerTask);
        task->start();
        ready.wait();
        ready.wait();
        task->destroy();
        //util::pause();
        TraceL << "Running Timer Task Test: END" << endl;
    }


    // ============================================================================
    // Signal Tests
    //
    struct SignalBroadcaster
    {
        SignalBroadcaster() {}
        ~SignalBroadcaster() {}

        Signal<int&>    TestSignal;
    };

    struct SignalReceiver
    {
        SignalBroadcaster& klass;
        SignalReceiver(SignalBroadcaster& klass) : klass(klass)
        {
            DebugL << "SignalReceiver: Starting" << endl;
            klass.TestSignal += sdelegate(this, &SignalReceiver::onSignal);
        }

        ~SignalReceiver()
        {
            DebugL << "SignalReceiver: Destroying" << endl;
            klass.TestSignal -= sdelegate(this, &SignalReceiver::onSignal);
        }

        void onSignal(void*, int& value)
        {
            DebugL << "SignalReceiver: Callback: " << value << endl;
        }
    };

    void runSignalReceivers() {
        {
            SignalBroadcaster broadcaster; //("Thread1");
            {
                SignalReceiver receiver(broadcaster);
            }
        }
        //util::pause();
    }

    // ============================================================================
    // Exception Test
    //
    void runExceptionTest()
    {
        try
        {
            throw FileException("That's not a file!");
            assert(0 && "must throw");
        }
        catch (FileException& exc)
        {
            cout << "Message: " << exc << endl;
        }
        catch (Exception&)
        {
            assert(0 && "bad cast");
        }

        try
        {
            throw IOException();
            assert(0 && "must throw");
        }
        catch (IOException& exc)
        {
            cout << "Message: " << exc << endl;
            assert(std::string(exc.what()) == "IO error");
        }
        catch (Exception&)
        {
            assert(0 && "bad cast");
        }
    }
    */

    // ============================================================================
    // Version String Comparison
    //
    void testVersionStringComparison()
    {
        assert((util::Version("3.7.8.0") == util::Version("3.7.8.0")) == true);
        assert((util::Version("3.7.8.0") == util::Version("3.7.8")) == true);
        assert((util::Version("3.7.8.0") < util::Version("3.7.8")) == false);
        assert((util::Version("3.7.9") < util::Version("3.7.8")) == false);
        assert((util::Version("3") < util::Version("3.7.9")) == true);
        assert((util::Version("1.7.9") < util::Version("3.1")) == true);

        cout << "Printing version (3.7.8.0): " << util::Version("3.7.8.0") << endl;
    }

    void runLoop() {
        DebugL << "#################### Running" << endl;
        app.run();
        DebugL << "#################### Ended" << endl;
    }

    void runCleanup() {
        DebugL << "#################### Finalizing" << endl;
        app.finalize();
        DebugL << "#################### Exiting" << endl;
    }

};


} // namespace scy


int main(int argc, char** argv)
{
    Logger::instance().add(new ConsoleChannel("debug", LTrace));
    //Logger::instance().setWriter(new AsyncLogWriter);

    {
#ifdef _MSC_VER
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

        // Create the test application
        Application app;

        // Initialize the GarbageCollector in the main thread
        GarbageCollector::instance();

        // Run tests
        {
            scy::Tests run(app);
        }

        // Wait for user intervention before finalizing
        scy::pause();

        // Finalize the application to free all memory
        app.finalize();
    }

    // Cleanup singleton instances
    GarbageCollector::destroy();
    Logger::destroy();
    return 0;
}
