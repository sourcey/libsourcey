#include "basetests.h"

using std::cout;
using std::cerr;
using std::endl;
using namespace scy;
using namespace scy::test;


int main(int argc, char** argv)
{
    // Logger::instance().add(new ConsoleChannel("debug", Level::Trace));
    test::init();

    // =========================================================================
    // Garbage Collector
    //
    describe("garbage collector", []() {
        GarbageCollector gc;
        auto loop = uv::defaultLoop();
        auto loop1 = uv::createLoop();

        // delete running idler on default loop
        auto idler = new Idler(loop, []() {});
        idler->handle().ref();
        gc.deleteLater(idler, loop);

        // delete running idler on loop1
        auto idler1 = new Idler(loop1, []() {});
        idler1->handle().ref();
        gc.deleteLater(idler1, loop1);

        // run the default loop which will return
        // once the idler is deleted
        uv::runLoop();

        // finalize deletion of all pointers
        gc.finalize();

        expect(loop->active_handles == 0);
        expect(loop1->active_handles == 0);

        // free the created loop1
        uv::closeLoop(loop1);
    });


    // =========================================================================
    // Signal Benchmarks
    //
    describe("signal class member benchmark", []() {
        Signal<void(uint64_t&)> signal;
        SignalCounter counter;
        signal += slot(&counter, &SignalCounter::increment);
        const uint64_t benchstart = time::hrtime();
        uint64_t i, value = 0;
        for (i = 0; i < 999999; i++) {
            signal.emit(value);
        }
        const uint64_t benchdone = time::hrtime();
        expect(value == i);

        std::cout << "signal class member benchmark: "
            << ((benchdone - benchstart) * 1.0 / i) << "ns "
            << "per emission (sz=" << sizeof(signal) << ")"
            << std::endl;
    });

    describe("signal const class member benchmark", []() {
        Signal<void(uint64_t&)> signal;
        SignalCounter counter;
        signal += slot(&counter, &SignalCounter::incrementConst);
        const uint64_t benchstart = time::hrtime();
        uint64_t i, value = 0;
        for (i = 0; i < 999999; i++) {
            signal.emit(value);
        }
        const uint64_t benchdone = time::hrtime();
        expect(value == i);

        std::cout << "signal const class member benchmark: "
            << ((benchdone - benchstart) * 1.0 / i) << "ns "
            << "per emission (sz=" << sizeof(signal) << ")" << std::endl;
    });

    describe("signal static member benchmark", []() {
        Signal<void(uint64_t&)> signal;
        signal += slot(&SignalCounter::incrementStatic);
        const uint64_t benchstart = time::hrtime();
        uint64_t i, value = 0;
        for (i = 0; i < 999999; i++) {
            signal.emit(value);
        }
        const uint64_t benchdone = time::hrtime();
        expect(value == i);

        std::cout << "signal static member benchmark: "
            << ((benchdone - benchstart) * 1.0 / i) << "ns "
            << "per emission (sz=" << sizeof(signal) << ")"
            << std::endl;
    });

    describe("signal free function benchmark", []() {
        Signal<void(uint64_t&)> signal;
        signal += signalIncrementFree;
        const uint64_t benchstart = time::hrtime();
        uint64_t i, value = 0;
        for (i = 0; i < 999999; i++) {
            signal.emit(value);
        }
        const uint64_t benchdone = time::hrtime();
        expect(value == i);

        std::cout << "signal free function benchmark: "
            << ((benchdone - benchstart) * 1.0 / i) << "ns "
            << "per emission (sz=" << sizeof(signal) << ")"
            << std::endl;
    });


    // =========================================================================
    // Buffer
    //
    describe("buffer", []() {
        ByteOrder orders[2] = { ByteOrder::Host, ByteOrder::Network };
        for (size_t i = 0; i < 2; i++) {
            Buffer buffer(1024);
            BitReader reader(buffer, orders[i]);
            BitWriter writer(buffer, orders[i]);
            expect(orders[i] == reader.order());
            expect(orders[i] == writer.order());

            // Write and read uint8_t.
            uint8_t wu8 = 1;
            writer.putU8(wu8);
            uint8_t ru8;
            reader.getU8(ru8);
            expect(writer.position() == 1);
            expect(reader.position() == 1);
            expect(wu8 == ru8);

            // Write and read uint16_t.
            uint16_t wu16 = (1 << 8) + 1;
            writer.putU16(wu16);
            uint16_t ru16;
            reader.getU16(ru16);
            expect(wu16 == ru16);
            expect(writer.position() == 3);
            expect(reader.position() == 3);

            // Write and read UInt24.
            uint32_t wu24 = (3 << 16) + (2 << 8) + 1;
            writer.putU24(wu24);
            uint32_t ru24;
            reader.getU24(ru24);
            expect(wu24 == ru24);
            expect(writer.position() == 6);
            expect(reader.position() == 6);

            // Write and read uint32_t.
            uint32_t wu32 = (4 << 24) + (3 << 16) + (2 << 8) + 1;
            writer.putU32(wu32);
            uint32_t ru32;
            reader.getU32(ru32);
            expect(wu32 == ru32);
            expect(writer.position() == 10);
            expect(reader.position() == 10);

            // Write and read uint64_t.
            uint32_t another32 = (8 << 24) + (7 << 16) + (6 << 8) + 5;
            uint64_t wu64 = (static_cast<uint64_t>(another32) << 32) + wu32;
            writer.putU64(wu64);
            uint64_t ru64;
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

    describe("buffer move offset", []() {
        Buffer buffer(1000);

        BitWriter writer(buffer.data(), buffer.size());
        expect(writer.position() == 0);
        expect(writer.limit() == buffer.size());

        // Move the write cursor around.
        writer.seek(100);
        expect(writer.position() == 100);
        writer.seek(900);
        expect(writer.position() == 900);
        writer.seek(500);
        expect(writer.position() == 500);

        // The cursor can be set to past the end of the buffer,
        // but can't perform any write operations there.
        try {
            writer.seek(1200);
            expect(0 && "must throw");
        }
        catch (std::out_of_range& exc) {
        }
    });

    describe("buffer write chunk", []() {
        const std::string kInputData = "abcdefghijklmnopqrstuvwxyz";
        Buffer output(kInputData.size());
        BitWriter writer(output.data(), output.size());

        writer.put(kInputData.data(), kInputData.size());
        expect(writer.position() == output.size());

        // Compare input and output data, converting the latter to
        // a string for clarity of error messages.
        expect(std::string(output.begin(), output.end()) == kInputData);
    });


    describe("dynamic resizable buffer", []() {
        std::string write_string("hello");
        std::string write_string1("world");

        ByteOrder orders[2] = { ByteOrder::Host, ByteOrder::Network };
        for (size_t i = 0; i < 2; i++) {
            Buffer buffer;
            DynamicBitWriter writer(buffer, orders[i]);
            writer.put(write_string.c_str(), write_string.size());
            writer.put(write_string1.c_str(), write_string1.size());

            BitReader reader(buffer, orders[i]);
            std::string read_string;
            reader.get(read_string, write_string.size() + write_string1.size());

            expect((write_string + write_string1) == read_string);
            expect(writer.position() == 10);
            expect(reader.position() == 10);
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

        expect((v1 == "value3" && v2 == "value31") || (v1 == "value31" && v2 == "value3"));

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
        // cout << "executable path: " << path << endl;
        expect(fs::exists(path));

        std::string junkPath(path + "junkname.huh");
        // cout << "junk path: " << junkPath << endl;
        expect(!fs::exists(junkPath));

        std::string dir(fs::dirname(path));
        // cout << "dir name: " << dir << endl;
        expect(fs::exists(dir));
        expect(fs::exists(dir + "/"));
        expect(fs::exists(dir + "\\"));
        expect(fs::dirname("file.a") == ".");
        expect(fs::dirname("/some/file.a") == "/some");
        expect(fs::dirname("/some/dir") == "/some");
        expect(fs::dirname("/some/dir/") == "/some/dir");

        std::string test(fs::dirname(dir));
        fs::addnode(test, "tests");
        expect(test == dir);
    });

    // =========================================================================
    // Logger

    describe("logger", []() {
        Logger& logger = Logger::instance();

        // Test default synchronous writer
        logger.setWriter(new LogWriter);
        clock_t start = clock();
        for (unsigned i = 0; i < 1000; i++)
            LTrace("sync log ", i)
        cout << "logger: synchronous test completed after: " << (clock() - start) << "ms" << endl;

        // Test default synchronous var args writer
        start = clock();
        for (unsigned i = 0; i < 1000; i++)
            LTrace("sync log", i)
        cout << "logger: synchronous var args test completed after: " << (clock() - start) << "ms" << endl;

        // Test asynchronous writer (approx 10x faster with console output)
        logger.setWriter(new AsyncLogWriter);
        start = clock();
        for (unsigned i = 0; i < 1000; i++)
            LTrace("async log ", i)
        cout << "logger: asynchronous test completed after: " << (clock() - start) << "ms" << endl;

         //// Test function logging
         //start = clock();
         //for (unsigned i = 0; i < 1000; i++)
         //    LTrace("test: ", i)
         //cout << "logger: asynchronous function logging completed after: "
         //     << (clock() - start) << endl;

         //// Test function and mem address logging
         //start = clock();
         //for (unsigned i = 0; i < 1000; i++)
         //    LTrace("test: ", i)
         //cout << "logger: asynchronous function and mem address logging completed after: "
         //     << (clock() - start) << endl;

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
            // std::cout << "on idle: " << counter << std::endl;
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
        uv::runLoop();

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
        uv::runLoop();

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


    // =========================================================================
    // Process
    //
    describe("process", []() {
        bool gotStdout = false, gotExit = false;
        Process proc{ "ping", "-c 4", "sourcey.com" };
        proc.onstdout = [&](std::string line) {
            std::cout << "process stdout: " << line << std::endl;
            gotStdout = true;
            proc.kill();
        };
        proc.onexit = [&](int64_t status) {
            std::cout << "process exit: " << status << std::endl;
            gotExit = true;
        };
        proc.spawn();
        uv::runLoop();
        expect(gotStdout);
        expect(gotExit);

        // TODO: test stdin pipes and verify file contents
    });


    // Define class based tests
    describe("signal", new SignalTest);
    describe("ipc", new IpcTest);
    describe("timer", new TimerTest);
    describe("packet stream", new PacketStreamTest);
    describe("packet stream file io", new PacketStreamIOTest);
    // describe("multi packet stream", new MultiPacketStreamTest);

    test::runAll();

    return test::finalize();
}
