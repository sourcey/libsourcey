//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// LibSourcey is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//


#ifndef SCY_Base_Tests_H
#define SCY_Base_Tests_H


#include "scy/base.h"
#include "scy/test.h"
#include "scy/logger.h"
#include "scy/idler.h"
#include "scy/signal.h"
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

        uv::runDefaultLoop();
        // std::cout << "Test IPC: OK" << std::endl;

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

        uv::runDefaultLoop();
        // std::cout << "Ending" << std::endl;

        expect(numTimerTicks == wantTimerTicks);
    }

    void timerCallback(void* sender)
    {
        auto timer = reinterpret_cast<Timer*>(sender);
        // std::cout << "On timeout: " << timer->count() << std::endl;
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
            // args[2] = NULL;
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
        runner.cancel();
        // runner.close();
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
        PacketStream stream;
        // stream.setRunner(std::make_shared<Thread>());
        stream.attachSource(new MockThreadedPacketSource, true, true);
        // stream.attach(new AsyncPacketQueue, 0, true);
        stream.attach(new MockPacketProcessor, 1, true);
        // stream.attach(new SyncPacketQueue, 2, true);
        // stream.synchronizeOutput(uv::defaultLoop());
        stream.emitter += packetDelegate(this, &PacketStreamTest::onPacketStreamOutput);
        stream.start();

        // TODO: Test pause/resume functionality

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
        // PacketStream stream;
        // //stream.setRunner(std::make_shared<Thread>());
        // stream.attachSource(new MockThreadedPacketSource, true, true);
        // //stream.attach(new AsyncPacketQueue, 0, true);
        // stream.attach(new MockPacketProcessor, 1, true);
        // //stream.emitter += packetDelegate(this, &Tests::onPacketStreamOutput);
        // stream.start();
        //
        // // The second PacketStream receives packets from the first one
        // // and synchronizes output packets with the default event loop.
        // ChildStreams children;
        // children.s1 = new PacketStream;
        // //children.s1->setRunner(std::make_shared<Idler>()); // Use Idler
        // children.s1->attachSource(stream.emitter);
        // children.s1->attach(new AsyncPacketQueue, 0, true);
        // children.s1->attach(new SyncPacketQueue, 1, true);
        // children.s1->synchronizeOutput(uv::defaultLoop());
        // children.s1->emitter += packetDelegate(this, &MultiPacketStreamTest::onChildPacketStreamOutput);
        // children.s1->start();
        //
        // children.s2 = new PacketStream;
        // children.s2->attachSource(stream.emitter);
        // children.s2->attach(new AsyncPacketQueue, 0, true);
        // children.s2->synchronizeOutput(uv::defaultLoop());
        // children.s2->emitter += packetDelegate(this, &MultiPacketStreamTest::onChildPacketStreamOutput);
        // children.s2->start();
        //
        // children.s3 = new PacketStream;
        // children.s3->attachSource(stream.emitter);
        // children.s3->attach(new AsyncPacketQueue, 0, true);
        // children.s3->synchronizeOutput(uv::defaultLoop());
        // children.s3->emitter += packetDelegate(this, &MultiPacketStreamTest::onChildPacketStreamOutput);
        // children.s3->start();
        //
        // // app.waitForShutdown([](void* arg) {
        // //     auto streams = reinterpret_cast<ChildStreams*>(arg);
        // //     //streams->s1->attachSource(stream.emitter);
        // //     if (streams->s1) delete streams->s1;
        // //     if (streams->s2) delete streams->s2;
        // //     if (streams->s3) delete streams->s3;
        // // }, &children);
        //
        // uv::runDefaultLoop();
        //
        // if (children.s1) delete children.s1;
        // if (children.s2) delete children.s2;
        // if (children.s3) delete children.s3;
    }
};


} // namespace scy


#endif // SCY_Base_Tests_H
