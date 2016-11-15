///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//


#ifndef SCY_Base_Tests_H
#define SCY_Base_Tests_H


#include "scy/base.h"
#include "scy/test.h"
#include "scy/logger.h"
#include "scy/idler.h"
#include "scy/signal.h"
#include "scy/signal2.h"
#include "scy/buffer.h"
#include "scy/platform.h"
#include "scy/collection.h"
#include "scy/application.h"
#include "scy/packetstream.h"
#include "scy/packetqueue.h"
#include "scy/packetio.h"
#include "scy/sharedlibrary.h"
#include "scy/filesystem.h"
#include "scy/process.h"
#include "scy/timer.h"
#include "scy/time.h"
#include "scy/ipc.h"
#include "scy/util.h"


using std::cout;
using std::cerr;
using std::endl;
using scy::test::Test;


namespace scy {


// =============================================================================
// IPC Test
//
class IpcTest: public Test
{
    int want_x_ipc_callbacks;
    int num_ipc_callbacks;

    void run() {
        // std::cout << "Test IPC" << std::endl;

        want_x_ipc_callbacks = 5;
        num_ipc_callbacks = 0;

        ipc::SyncQueue<> ipc;
        ipc.push(new ipc::Action(std::bind(&IpcTest::ipcCallback, this, std::placeholders::_1), &ipc, "test1"));
        ipc.push(new ipc::Action(std::bind(&IpcTest::ipcCallback, this, std::placeholders::_1), &ipc, "test2"));
        ipc.push(new ipc::Action(std::bind(&IpcTest::ipcCallback, this, std::placeholders::_1), &ipc, "test3"));
        ipc.push(new ipc::Action(std::bind(&IpcTest::ipcCallback, this, std::placeholders::_1), &ipc, "test4"));
        ipc.push(new ipc::Action(std::bind(&IpcTest::ipcCallback, this, std::placeholders::_1), &ipc, "test5"));

        // std::cout << "Test IPC: OK" << std::endl;
        uv::runDefaultLoop();

        expect(num_ipc_callbacks == want_x_ipc_callbacks);
    }

    void ipcCallback(const ipc::Action& action)
    {
        // std::cout << "Got IPC callback: " << action.data << std::endl;
        if (++num_ipc_callbacks == want_x_ipc_callbacks)
            reinterpret_cast<ipc::Queue<>*>(action.arg)->close();
    }
};


// =============================================================================
// Timer Test
//
class TimerTest: public Test
{
    int numTimerTicks;
    int wantTimerTicks;
    bool timerRestarted;

    void run()
    {
        std::cout << "Starting" << std::endl;

        numTimerTicks = 0;
        wantTimerTicks = 10;
        timerRestarted = false;

        Timer timer;
        timer.Timeout += sdelegate(this, &TimerTest::timerCallback);
        timer.start(10, 10);
        timer.handle().ref();

        // std::cout << "Ending" << std::endl;
        uv::runDefaultLoop();

        expect(numTimerTicks == wantTimerTicks);
    }

    void timerCallback(void* sender)
    {
        // std::cout << "On timeout: " << timer->count() << std::endl;
        auto timer = reinterpret_cast<Timer*>(sender);

        numTimerTicks++;
        if (timer->count() == wantTimerTicks / 2) {
            if (!timerRestarted) {
                timerRestarted = true;
                timer->restart(); // restart once, count returns to 0
            }
            else {
                timer->handle().unref();
                timer->stop(); // event loop will be released
            }
        }
    }
};


// =============================================================================
// Idler Test
//
class IdlerTest: public Test
{
    int wantIdlerTicks;
    int numIdlerTicks;
    Idler idler;

    void run()
    {
        wantIdlerTicks = 5;
        numIdlerTicks = 0;

        idler.start(std::bind(&IdlerTest::idlerCallback, this));
        idler.handle().ref();

        uv::runDefaultLoop();

        expect(numIdlerTicks == wantIdlerTicks);
    }

    void idlerCallback()
    {
        // std::cout << "On idle" << std::endl;
        if (++numIdlerTicks == wantIdlerTicks) {
            idler.handle().unref();
            idler.cancel(); // event loop will be released
        }
    }
};


// =============================================================================
// Signal Test
//
class SignalTest: public Test
{
    Signal<int&> TestSignal;

    void run()
    {
        int val = 0;
        TestSignal += sdelegate(this, &SignalTest::intCallback);
        TestSignal += delegate(this, &SignalTest::intCallbackNoSender);
        TestSignal.emit(this, val);
        expect(val == 2);

        val = -1;
        TestSignal.clear();
        TestSignal += delegate(this, &SignalTest::priorityOne, 1); // last
        TestSignal += delegate(this, &SignalTest::priorityZero, 0); // second
        TestSignal += delegate(this, &SignalTest::priorityMinusOne, -1); // first
        TestSignal.emit(this, val);
    }

    void intCallback(void* sender, int& val)
    {
        expect(sender == this);
        val++;
    }

    void intCallbackNoSender(int& val)
    {
        val++;
    }

    void priorityMinusOne(int& val)
    {
        expect(val++ == -1);
    }

    void priorityZero(int& val)
    {
        expect(val++ == 0);
    }

    void priorityOne(int& val)
    {
        expect(val++ == 1);
    }
};


// =============================================================================
// Signal V2 Test
//
class PolymorphicSignalTests: public Test {
    int numPackets = 0;

    void onPacket(IPacket& packet)
    {
        std::cout << "On packet: " << packet.className() << std::endl;
        numPackets++;
    }

    void onPacketWithSender(void*, IPacket& packet)
    {
        std::cout << "On packet sender: " << packet.className() << std::endl;
        numPackets++;
    }

public:
    void run() {
        v2::Signal < void(IPacket&) > sig1;
        v2::Signal < void(void*, IPacket&) > sig2;

        sig1() += v2::slot(this, &PolymorphicSignalTests::onPacket);
        sig2() += v2::slot(this, &PolymorphicSignalTests::onPacketWithSender);

        RawPacket p("hello", 5);
        sig1.emit(p);
        sig2.emit(this, p);

        expect(numPackets == 2);
    }
};


class BasicSignalTests: public Test {
    static std::string accu;
    struct Foo {
        char foo_bool(float f, int i, std::string s) {
            accu += util::format("Foo: %.2f\n", f + i + s.size());
            return true;
        }
    };
    static char float_callback(float f, int, std::string) {
        accu += util::format("float: %.2f\n", f);
        return 0;
    }

public:
    void run() {
        accu = "";
        v2::Signal < char(float, int, std::string) > sig1;
        size_t id1 = sig1() += float_callback;
        size_t id2 = sig1() += [](float, int i, std::string) {
            accu += util::format("int: %d\n", i);
            return 0;
        };
        size_t id3 = sig1() += [](float, int,
            const std::string & s) {
            accu += util::format("string: %s\n", s.c_str());
            return 0;
        };
        sig1.emit(.3, 4, "huhu");
        bool success;
        success = sig1() -= id1;
        expect(success == true);
        success = sig1() -= id1;
        expect(success == false);
        success = sig1() -= id2;
        expect(success == true);
        success = sig1() -= id3;
        expect(success == true);
        success = sig1() -= id3;
        expect(success == false);
        success = sig1() -= id2;
        expect(success == false);
        Foo foo;
        sig1() += v2::slot(foo, & Foo::foo_bool);
        sig1() += v2::slot( & foo, & Foo::foo_bool);
        sig1.emit(.5, 1, "12");

        v2::Signal < void(std::string, int) > sig2;
        sig2() += [](std::string msg, int) {
            accu += util::format("msg: %s", msg.c_str());
        };
        sig2() += [](std::string, int d) {
            accu += util::format(" *%d*\n", d);
        };
        sig2.emit("in sig2", 17);

        accu += "DONE";

        const char * expected = "float: 0.30\n"
        "int: 4\n"
        "string: huhu\n"
        "Foo: 3.50\n"
        "Foo: 3.50\n"
        "msg: in sig2 *17*\n"
        "DONE";
        expect(accu == expected);
        std::cout << accu << std::endl;
    }
};
std::string BasicSignalTests::accu;


class TestCollectorVector: public Test {
    static int handler1() {
        return 1;
    }
    static int handler42() {
        return 42;
    }
    static int handler777() {
        return 777;
    }
public:
    void run() {
        v2::Signal < int(), v2::CollectorVector < int >> sig_vector;
        sig_vector() += handler777;
        sig_vector() += handler42;
        sig_vector() += handler1;
        sig_vector() += handler42;
        sig_vector() += handler777;
        std::vector < int > results = sig_vector.emit();
        // const int reference_array[] = { 777, 42, 1, 42, 777, };
        // const std::vector<int> reference = vector_from_array (reference_array);
        const std::vector < int > reference = {
            777,
            42,
            1,
            42,
            777,
        };
        expect(results == reference);
    }
};


class TestCollectorUntil0: public Test {
    bool check1, check2;
    bool handler_true() {
        check1 = true;
        return true;
    }
    bool handler_false() {
        check2 = true;
        return false;
    }
    bool handler_abort() {
        abort();
    }
public:
    TestCollectorUntil0(): check1(0), check2(0) {}

    void run() {
        TestCollectorUntil0 self;
        v2::Signal < bool(), v2::CollectorUntil0 < bool >> sig_until0;
        sig_until0() += v2::slot(self, & TestCollectorUntil0::handler_true);
        sig_until0() += v2::slot(self, & TestCollectorUntil0::handler_false);
        sig_until0() += v2::slot(self, & TestCollectorUntil0::handler_abort);
        expect(!self.check1 && !self.check2);
        const bool result = sig_until0.emit();
        expect(!result && self.check1 && self.check2);
    }
};


class TestCollectorWhile0: public Test {
    bool check1, check2;
    bool handler_0() {
        check1 = true;
        return false;
    }
    bool handler_1() {
        check2 = true;
        return true;
    }
    bool handler_abort() {
        abort();
    }
public:
    TestCollectorWhile0(): check1(0), check2(0) {}

    void run() {
        TestCollectorWhile0 self;
        v2::Signal < bool(), v2::CollectorWhile0 < bool >> sig_while0;
        sig_while0() += v2::slot(self, & TestCollectorWhile0::handler_0);
        sig_while0() += v2::slot(self, & TestCollectorWhile0::handler_1);
        sig_while0() += v2::slot(self, & TestCollectorWhile0::handler_abort);
        expect(!self.check1 && !self.check2);
        const bool result = sig_while0.emit();
        expect(result == true && self.check1 && self.check2);
    }
};


// TestCounter for Signal benchmark tests
struct TestCounter {
    static std::uint64_t get();
    static void set(std::uint64_t);
    static void add(std::uint64_t);
    static void add2(void * , std::uint64_t);
};

namespace { // Anon
    void( * test_counter_add2)(void * , std::uint64_t) =
        TestCounter::add2; // external symbol to prevent easy inlining
    static std::uint64_t test_counter_var = 0;
} // Anon

std::uint64_t TestCounter::get() {
    return test_counter_var;
}

void TestCounter::set(std::uint64_t v) {
    test_counter_var = v;
}

void TestCounter::add(std::uint64_t v) {
    test_counter_var += v;
}

void TestCounter::add2(void*, std::uint64_t v) {
    test_counter_var += v;
}


// =============================================================================
// Process
//
class ProcessTest: public Test
{
    void run()
    {
        try
        {
            Process proc;

            // TODO: Write cross platfrom process test

            // char* args[3];
            // args[0] = "C:/Windows/notepad.exe";
            // args[1] = "runspot";
            // args[2] = nullptr_t;
            //
            // proc.options.args = args;
            // proc.options.file = args[0];
            // proc.onexit = std::bind(&Tests::processExit, this, std::placeholders::_1);
            // proc.spawn();

            uv::runDefaultLoop();
        }
        catch (std::exception& exc)
        {
            std::cerr << "Process error: " << exc.what() << std::endl;
            expect(0);
        }
    }

    void processExit(std::int64_t exitStatus)
    {
        std::cout << "On process exit: " << exitStatus << std::endl;
    }
};


// =============================================================================
// Packet Stream
//
struct MockThreadedPacketSource: public PacketSource, public async::Startable
{
    Thread runner;
    PacketSignal emitter;

    MockThreadedPacketSource() :
        PacketSource(emitter)
    {
        runner.setRepeating(true);
    }

    void start()
    {
        std::cout << "Start" << std::endl;
        runner.start([](void* arg) {
            auto self = reinterpret_cast<MockThreadedPacketSource*>(arg);
            // std::cout << "Emitting" << std::endl;
            RawPacket p("hello", 5);
            self->emitter.emit(self, p);
        }, this);
    }

    void stop()
    {
        // std::cout << "Stop" << std::endl;
        // runner.close();
        runner.cancel();
        // std::cout << "Stop: OK" << std::endl;
    }
};

struct MockPacketProcessor: public PacketProcessor
{
    PacketSignal emitter;

    MockPacketProcessor() :
        PacketProcessor(emitter)
    {
    }

    void process(IPacket& packet)
    {
        // std::cout << "Process: " << packet.className() << std::endl;
        emit(packet);
    }
};

class PacketStreamTest: public Test
{
    int numPackets;

    void onPacketStreamOutput(void* sender, IPacket& packet)
    {
        // std::cout << "On packet: " << packet.className() << std::endl;
        numPackets++;
    }

    void run()
    {
        numPackets = 0;
        // stream.setRunner(std::make_shared<Thread>());
        PacketStream stream;
        // stream.attach(new AsyncPacketQueue, 0, true);
        stream.attachSource(new MockThreadedPacketSource, true, true);
        // stream.attach(new SyncPacketQueue, 2, true);
        // stream.synchronizeOutput(uv::defaultLoop());
        stream.attach(new MockPacketProcessor, 1, true);

        stream.emitter += packetDelegate(this, &PacketStreamTest::onPacketStreamOutput);
        stream.start();
    /// TODO: Test pause/resume functionality
        // Run the thread for 100ms
        scy::sleep(100);

        stream.close();

        expect(numPackets > 0);
    }
};

static std::string RANDOM_CONTENT = "r@ndom";

struct PacketStreamIOTest: public Test
{

    int numPackets;

    PacketStreamIOTest()
    {
        fs::savefile("input.txt", RANDOM_CONTENT.c_str(), RANDOM_CONTENT.length(), true);
    }

    ~PacketStreamIOTest()
    {
        fs::unlink("input.txt");
        fs::unlink("output.txt");
    }

    void run()
    {
        numPackets = 0;
        PacketStream stream;
        stream.attachSource(new ThreadedStreamReader(new std::ifstream("input.txt")), true, true);
        stream.attach(new StreamWriter(new std::ofstream("output.txt")), 1, true);
        stream.start();
        // Run the thread for 100ms
        scy::sleep(100);

        stream.close();

        // Verify result
        std::string result;
        std::ifstream ofile("output.txt");
        util::copyToString(ofile, result);
        expect(result == RANDOM_CONTENT);
    }
};

class MultiPacketStreamTest: public Test
{
    void onChildPacketStreamOutput(void* sender, IPacket& packet)
    {
        // std::cout << "On child packet: " << packet.className() << std::endl;
    }

    struct ChildStreams
    {
        PacketStream* s1;
        PacketStream* s2;
        PacketStream* s3;
    };

    void run()
    {
        /// //stream.setRunner(std::make_shared<Thread>());
        // stream.attachSource(new MockThreadedPacketSource, true, true);
        // //stream.attach(new AsyncPacketQueue, 0, true);
        // stream.attach(new MockPacketProcessor, 1, true);
        // //stream.emitter += packetDelegate(this, &Tests::onPacketStreamOutput);
        // stream.start();
        ///
        /// // The second PacketStream receives packets from the first one
        /// // and synchronizes output packets with the default event loop.
        /// ChildStreams children;
        // children.s1 = new PacketStream;
        // //children.s1->setRunner(std::make_shared<Idler>()); // Use Idler
        /// children.s1->attachSource(stream.emitter);
        // children.s1->attach(new AsyncPacketQueue, 0, true);
        // children.s1->attach(new SyncPacketQueue, 1, true);
        // children.s1->synchronizeOutput(uv::defaultLoop());
        // children.s1->emitter += packetDelegate(this, &MultiPacketStreamTest::onChildPacketStreamOutput);
        // children.s1->start();
        ///
        /// children.s2 = new PacketStream;
        // children.s2->attachSource(stream.emitter);
        // children.s2->attach(new AsyncPacketQueue, 0, true);
        // children.s2->synchronizeOutput(uv::defaultLoop());
        // children.s2->emitter += packetDelegate(this, &MultiPacketStreamTest::onChildPacketStreamOutput);
        // children.s2->start();
        ///
        /// children.s3 = new PacketStream;
        // children.s3->attachSource(stream.emitter);
        // children.s3->attach(new AsyncPacketQueue, 0, true);
        // children.s3->synchronizeOutput(uv::defaultLoop());
        // children.s3->emitter += packetDelegate(this, &MultiPacketStreamTest::onChildPacketStreamOutput);
        // children.s3->start();
        ///
        /// // app.waitForShutdown([](void* arg) {
        /// //     auto streams = reinterpret_cast<ChildStreams*>(arg);
        // //     //streams->s1->attachSource(stream.emitter);
        // //     if (streams->s1) delete streams->s1;
        // //     if (streams->s2) delete streams->s2;
        // //     if (streams->s3) delete streams->s3;
        // // }, &children);
        ///
        /// uv::runDefaultLoop();
        ///
        /// if (children.s1) delete children.s1;
        // if (children.s2) delete children.s2;
        // if (children.s3) delete children.s3;
        // PacketStream stream;
    }
};


} // namespace scy


#endif // SCY_Base_Tests_H

/// @\}
