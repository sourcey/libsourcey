///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//


#ifndef SCY_Base_Tests_H
#define SCY_Base_Tests_H


#include "scy/base.h"
#include "scy/test.h"
#include "scy/application.h"
#include "scy/buffer.h"
#include "scy/datetime.h"
#include "scy/collection.h"
#include "scy/filesystem.h"
#include "scy/idler.h"
#include "scy/ipc.h"
#include "scy/logger.h"
#include "scy/packetio.h"
#include "scy/packetqueue.h"
#include "scy/packetstream.h"
#include "scy/platform.h"
#include "scy/process.h"
#include "scy/sharedlibrary.h"
#include "scy/signal.h"
#include "scy/time.h"
#include "scy/timer.h"
#include "scy/thread.h"
#include "scy/util.h"


using std::cout;
using std::cerr;
using std::endl;
using scy::test::Test;


namespace scy {


// =============================================================================
// IPC Test
//
class IpcTest : public Test
{
    int want_x_ipc_callbacks;
    int num_ipc_callbacks;

    void run()
    {
        // std::cout << "Test IPC" << std::endl;

        want_x_ipc_callbacks = 5;
        num_ipc_callbacks = 0;

        ipc::SyncQueue<> ipc;
        ipc.push(new ipc::Action(
            std::bind(&IpcTest::ipcCallback, this, std::placeholders::_1), &ipc, "test1"));
        ipc.push(new ipc::Action(
            std::bind(&IpcTest::ipcCallback, this, std::placeholders::_1), &ipc, "test2"));
        ipc.push(new ipc::Action(
            std::bind(&IpcTest::ipcCallback, this, std::placeholders::_1), &ipc, "test3"));
        ipc.push(new ipc::Action(
            std::bind(&IpcTest::ipcCallback, this, std::placeholders::_1), &ipc, "test4"));
        ipc.push(new ipc::Action(
            std::bind(&IpcTest::ipcCallback, this, std::placeholders::_1), &ipc,  "test5"));

        // std::cout << "Test IPC: OK" << std::endl;
        uv::runLoop();

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
class TimerTest : public Test
{
    void run()
    {
        std::cout << "Starting" << std::endl;

        int numTimerTicks = 0;
        int wantTimerTicks = 10;
        bool timerRestarted = false;

        Timer timer(10, 10);
        timer.start([&]() {
            // std::cout << "On timeout: " << timer->count() << std::endl;

            numTimerTicks++;
            if (timer.count() == wantTimerTicks / 2) {
                if (!timerRestarted) {
                    timerRestarted = true;
                    timer.restart(); // restart once, count returns to 0
                } else {
                    timer.handle().unref();
                    timer.stop(); // event loop will be released
                }
            }
        });
        timer.handle().ref();
        // timer.Timeout += [&]() {
        //     // std::cout << "On timeout: " << timer->count() << std::endl;
        //
        //     numTimerTicks++;
        //     if (timer.count() == wantTimerTicks / 2) {
        //         if (!timerRestarted) {
        //             timerRestarted = true;
        //             timer.restart(); // restart once, count returns to 0
        //         } else {
        //             timer.handle().unref();
        //             timer.stop(); // event loop will be released
        //         }
        //     }
        // };

        // std::cout << "Ending" << std::endl;
        uv::runLoop();

        expect(numTimerTicks == wantTimerTicks);
    }
};


// =============================================================================
// Idler Test
//
// class IdlerTest : public Test
// {
//     int wantIdlerTicks;
//     int numIdlerTicks;
//     Idler idler;
//
//     void run()
//     {
//         wantIdlerTicks = 5;
//         numIdlerTicks = 0;
//
//         idler.start(std::bind(&IdlerTest::idlerCallback, this));
//         idler.handle().ref();
//
//         uv::runLoop();
//
//         expect(numIdlerTicks == wantIdlerTicks);
//     }
//
//     void idlerCallback()
//     {
//         // std::cout << "On idle" << std::endl;
//         if (++numIdlerTicks == wantIdlerTicks) {
//             idler.handle().unref();
//             idler.cancel(); // event loop will be released
//         }
//     }
// };


// =============================================================================
// Signal Test
//
// class SignalTest: public Test
// {
//     Signal<void(uint64_t&)> TestSignal;
//
//     void run()
//     {
//         int val = 0;
//         TestSignal += sdelegate(this, &SignalTest::intCallback);
//         TestSignal += slot(this, &SignalTest::intCallbackNoSender);
//         TestSignal.emit(/*this, */val);
//         expect(val == 2);
//
//         val = -1;
//         TestSignal.clear();
//         TestSignal += slot(this, &SignalTest::priorityOne, 1); // last
//         TestSignal += slot(this, &SignalTest::priorityZero, 0); // second
//         TestSignal += slot(this, &SignalTest::priorityMinusOne, -1); // first
//         TestSignal.emit(/*this, */val);
//     }
//
//     void intCallback(void* sender, int& val)
//     {
//         expect(sender == this);
//         val++;
//     }
//
//     void intCallbackNoSender(int& val)
//     {
//         val++;
//     }
//
//     void priorityMinusOne(int& val)
//     {
//         expect(val++ == -1);
//     }
//
//     void priorityZero(int& val)
//     {
//         expect(val++ == 0);
//     }
//
//     void priorityOne(int& val)
//     {
//         expect(val++ == 1);
//     }
// };


struct SignalCounter
{
    void increment(uint64_t& val) { val++; }

    void incrementConst(uint64_t& val) const { val++; }

    static void incrementStatic(uint64_t& val) { val++; }
};


void signalIncrementFree(uint64_t& val)
{
    val++;
}


bool signalHandlerC(const char* sl, size_t ln)
{
    // std::cout << "signalHandlerC: " << sl << ln << std::endl;
    return false;
}


class SignalTest: public Test
{
    struct Foo
    {
        int timesCalled = 0;

        bool signalHandlerA(const char* sl, size_t ln)
        {
            // std::cout << "signalHandlerA: " << sl << ln << std::endl;
            timesCalled++;
            return true;
        }

        bool signalHandlerB(const char* sl, size_t ln) const
        {
            // std::cout << "signalHandlerB: " << sl << ln << std::endl;
            return true;
        }
    };

    void run()
    {
        Foo foo;
        Signal<bool(const char*, size_t)> signal;

        int refid1, refid2, refid3, refid4, refid5;

        // Attach a lambda function
        refid1 = signal.attach([&](const char* arg1, size_t arg2) {
            // std::cout << "lambda: " << arg1 << arg2 << std::endl;

            // Detach slots inside callback scope
            expect(signal.nslots() == 4);
            signal.detach(refid1);
            signal.detach(refid2);
            expect(signal.nslots() == 2);
            return true;
        });
        expect(refid1 == 1);

        // Attach a lambda function via += operator
        refid2 = signal += [&](const char* arg1, size_t arg2) {
            // std::cout << "lambda 2: " << arg1 << arg2 << std::endl;
            return true;
        };
        expect(refid2 == 2);

        // Attach and disconnect a class member slot
        refid3 = signal += slot(&foo, &Foo::signalHandlerA);
        expect(refid3 == 3);
        expect(signal.nslots() == 3);
        bool detached = signal -= slot(&foo, &Foo::signalHandlerA);
        expect(detached == true);
        expect(signal.nslots() == 2);

        // Attach and disconnect a const class member slot
        refid4 = signal += slot(&foo, &Foo::signalHandlerB, 100, -1);
        expect(refid4 == 100);

        // Attach a static function via += operator
        refid5 = signal += signalHandlerC;
        expect(refid5 == 4);

        signal.emit("the answer to life the universe and everything is", 42);
    }
};


#if 0 // TODO: Write cross platfrom process test with file fixtures
// =============================================================================
// Process
//
class ProcessTest : public Test
{
    void run()
    {
        try {
            Process proc;

            char* args[3];
            args[0] = "C:/Windows/notepad.exe";
            args[1] = "runspot";
            args[2] = nullptr_t;

            proc.options.args = args;
            proc.options.file = args[0];
            proc.onexit = std::bind(&Tests::processExit, this, std::placeholders::_1);
            proc.spawn();

            uv::runLoop();
        try {
        } catch (std::exception& exc) {
            std::cerr << "Process error: " << exc.what() << std::endl;
            expect(0);
        }
    }

    void processExit(std::int64_t exitStatus)
    {
        std::cout << "On process exit: " << exitStatus << std::endl;
    }
};
#endif


// =============================================================================
// Packet Stream
//
struct MockThreadedPacketSource : public PacketSource, public basic::Startable
{
    Thread runner;
    PacketSignal emitter;

    MockThreadedPacketSource()
        : PacketSource(emitter)
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
            self->emitter.emit(/*self, */ p);
            // std::cout << "Emitting 2" << std::endl;
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

struct MockPacketProcessor : public PacketProcessor
{
    PacketSignal emitter;

    MockPacketProcessor()
        : PacketProcessor(emitter)
    {
    }

    void process(IPacket& packet)
    {
        // std::cout << "Process: " << packet.className() << std::endl;
        emitter.emit(packet);
    }
};

class PacketStreamTest : public Test
{
    int numPackets;

    void onPacketStreamOutput(IPacket& packet)
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

        stream.emitter += slot(this, &PacketStreamTest::onPacketStreamOutput);
        // stream.emitter += packetSlot(this,
        // &PacketStreamTest::onPacketStreamOutput);
        // stream.emitter.attach<PacketStreamTest,
        // &PacketStreamTest::onPacketStreamOutput>(this);

        stream.start();

        // TODO: Test pause/resume functionality
        // Run the thread for 100ms
        scy::sleep(100);

        stream.close();

        expect(numPackets > 0);
    }
};

static std::string RANDOM_CONTENT = "r@ndom";

struct PacketStreamIOTest : public Test
{
    int numPackets;

    PacketStreamIOTest()
    {
        fs::savefile("input.txt", RANDOM_CONTENT.c_str(),
                     RANDOM_CONTENT.length(), true);
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

class MultiPacketStreamTest : public Test
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
        // //stream.emitter += packetSlot(this, &Tests::onPacketStreamOutput);
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
        // children.s1->emitter += packetSlot(this,
        // &MultiPacketStreamTest::onChildPacketStreamOutput);
        // children.s1->start();
        ///
        /// children.s2 = new PacketStream;
        // children.s2->attachSource(stream.emitter);
        // children.s2->attach(new AsyncPacketQueue, 0, true);
        // children.s2->synchronizeOutput(uv::defaultLoop());
        // children.s2->emitter += packetSlot(this,
        // &MultiPacketStreamTest::onChildPacketStreamOutput);
        // children.s2->start();
        ///
        /// children.s3 = new PacketStream;
        // children.s3->attachSource(stream.emitter);
        // children.s3->attach(new AsyncPacketQueue, 0, true);
        // children.s3->synchronizeOutput(uv::defaultLoop());
        // children.s3->emitter += packetSlot(this,
        // &MultiPacketStreamTest::onChildPacketStreamOutput);
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
        /// uv::runLoop();
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
