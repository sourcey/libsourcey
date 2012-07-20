#include "Sourcey/Base.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Runner.h"
#include "Sourcey/Signal.h"
#include "Sourcey/PacketStream.h"
#include "Sourcey/ScheduledTask.h"
#include "Sourcey/Timer.h"
#include "Sourcey/Util.h"

#include "Poco/NamedEvent.h"

/*
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
*/
#include <assert.h>


using namespace std;
using namespace Poco;
using namespace Sourcey;


/*
// Detect Memory Leaks
#ifdef _DEBUG
#include "MemLeakDetect/MemLeakDetect.h"
CMemLeakDetect memLeakDetect;
#endif
*/


namespace Sourcey {
	

static NamedEvent ready("TestEvent");
	

class Tests
{
	//Poco::Event ready;
	Runner runner;

public:
	Tests()
	{	
		//runTimerTaskTest();
		runScheduledTaskTest();
		//runTimerTest();
		//runPacketStreamTests();
		//runPacketSignalTest();
		//runSocketTests();
		//runGarbageCollectorTests();
		//runSignalReceivers();
		
		Util::pause();
	}
	
	// ---------------------------------------------------------------------
	//
	// Timer Task Tests
	//
	// ---------------------------------------------------------------------	
	void onTimerTask(void* sender)
	{
		Log("trace") << "Timer Task Timout" << endl;
		ready.set();
	}

	void runTimerTaskTest() 
	{
		Log("trace") << "Running Timer Task Test" << endl;
		TimerTask* task = new TimerTask(runner, 1000, 1000);
		task->Timeout += delegate(this, &Tests::onTimerTask);
		task->start();
		ready.wait();
		ready.wait();
		task->destroy();
		//Util::pause();
		Log("trace") << "Running Timer Task Test: END" << endl;
	}
	

	// ---------------------------------------------------------------------
	//
	// Scheduled Task Tests
	//
	// ---------------------------------------------------------------------	
	struct TestScheduledTask: public ScheduledTask
	{
		TestScheduledTask(Runner& runner) : 
			ScheduledTask(runner) {}

		void run() 
		{
			Log("debug") << "TestScheduledTask: Running" << endl;
			ready.set();	
		}
	};

	void runScheduledTaskTest() 
	{
		Log("trace") << "Running Scheduled Task Test" << endl;

		// Schedule to fire 1 second from now.
		{
			TestScheduledTask* task = new TestScheduledTask(runner);
			Poco::DateTime dt;
			Poco::Timespan ts(1, 0);
			dt += ts;
			task->schedule(dt);
			task->start();
			ready.wait();
		}
		
		// Schedule to fire once now, and twice at 1 second intervals.
		{
			TestScheduledTask* task = new TestScheduledTask(runner);
			Poco::DateTime dt;
			Poco::Timespan ts(1, 0);
			task->scheduleRepeated(dt, ts);
			task->start();
			ready.wait();
			ready.wait();
			ready.wait();		
			task->destroy();
		}
		
		Log("trace") << "Running Scheduled Task Test: END" << endl;
		Util::pause();
	}


	// ---------------------------------------------------------------------
	//
	// Packet Signal Tests
	//
	// ---------------------------------------------------------------------	
	/*
	PacketSignal BroadcastPacket;

	void onBroadcastPacket(void* sender, DataPacket& packet)
	{
		Log("trace") << "On Packet: " << packet.className() << endl;
	}
	
	void runPacketSignalTest() 
	{
		Log("trace") << "Running Packet Signal Test" << endl;
		BroadcastPacket += packetDelegate(this, &Tests::onBroadcastPacket, 0);
		DataPacket packet;
		BroadcastPacket.dispatch(this, packet);
		Util::pause();
		Log("trace") << "Running Packet Signal Test: END" << endl;
	}
	*/

	
	// ---------------------------------------------------------------------
	//
	// Packet Stream Tests
	//
	// ---------------------------------------------------------------------	
	/*
	class SimplePacketProcessor: public IPacketProcessor
	{
		void process(IPacket& packet) 
		{
			Log("debug") << "SimplePacketProcessor: Processing: " << packet.className() << endl;
			
			BroadcastPacket.send(this, packet);
		}
	};
	
	void onPacketStreamPacket(void* sender, IPacket& packet) 
	{
		Log("trace") << "On Packet: " << packet.className() << endl;
	}

	void runPacketStreamTests() 
	{
		PacketStream stream;
		stream.attach(packetDelegate(this, &Tests::onPacketStreamPacket, 0));

		// init source
		//Media::VideoCapture cap(0);
		//stream.attach(&cap);
		
		// init proc
		//SimplePacketProcessor proc;
		//stream.attach(&proc);	

		// run!		
		//cap.start();
		Util::pause();
		//cap.stop();
	}
	*/
	

	// ---------------------------------------------------------------------
	//
	// Garbage Collector Tests
	//
	// ---------------------------------------------------------------------	
	/*
	void runGarbageCollectorTests() {
		Log("trace") << "Running Garbage Collector Test" << endl;
		
		//for (unsigned i = 0; i < 100; i++) { 
			char* ptr = new char[1000];
		
			Poco::Thread* ptr1 = new Poco::Thread;
		
			TaskRunner::getDefault().deleteLater<char*>(ptr);
			//TaskRunner::getDefault().deleteLater<Poco::Thread>(ptr1);
		//}

		Util::pause();
		Log("trace") << "Running Garbage Collector Test: END" << endl;
	}
	*/
	
	
	// ---------------------------------------------------------------------
	//
	// Timer Tests
	//
	// ---------------------------------------------------------------------

	/*
	struct TimerThread: public Poco::Runnable 
	{				
		TimerThread(const string& name) : _name(name) {
			_thread.start(*this);
		}
	
		void run() 	{
			for (unsigned i = 0; i < 100; i++) {
				Log("debug") << "TimerThread: " << _name << endl;
				new TimerTest;
			}
		}
	
		Poco::Thread		_thread;
		string			_name;
	};
	*/
	struct TimerTest
	{
		string name;
		int iteration;
		int iterations;
		time_t startAt;
		//Timeout validator;
		//int timeout;
		//int interval;

		TimerTest(const string& name, int timeout = 1000/*, int interval = 1000*/, int iterations = 5) : 
			name(name), startAt(Util::getTime()), iteration(0), iterations(iterations)//, timeout(timeout), interval(interval)
		{
			Log("debug") << "[TimerTest:" << name << "] Creating" << endl;	
			Timer::getDefault().start(TimerCallback<TimerTest>(this, &TimerTest::onTimer, timeout, timeout));
			//validator.start(timeout * iterations);
		}

		~TimerTest()
		{
			Log("debug") << "[TimerTest:" << name << "] Destroying" << endl;	
			Timer::getDefault().stop(TimerCallback<TimerTest>(this, &TimerTest::onTimer));
		}

		void onTimer(TimerCallback<TimerTest>& timer)
		{
			iteration++;
			if (iteration < iterations) {
				Log("debug") << "[TimerTest:" << name << "] Callback"
					<< "\n\tElapsed: " << Util::getTime() - startAt
					<< "\n\tInterval: " << timer.periodicInterval()
					<< "\n\tIteration: " << iteration
					<< endl;
			}
			else {
				Log("debug") << "[TimerTest:" << name << "] Complete #####################################"
					<< "\n\tTotal Time: " << Util::getTime() - startAt
					<< "\n\tCorrect Time: " << timer.periodicInterval() * iterations
					<< "\n\tIterations: " << iterations
					<< endl;			
				delete this;
			}
		}
	};
	 
	void runTimerTest() {
		Log("trace") << "Running Timer Test" << endl;
		new TimerTest("Periodic", 1000, 3);
		new TimerTest("Single", 3000, 1);
		new TimerTest("Single 1", 5000, 1);
		new TimerTest("Periodic 1", 1000, 5);
		new TimerTest("Single 2", 5000, 1);
		new TimerTest("Periodic 2", 1000, 5);
		Util::pause();
		Log("trace") << "Running Timer Test: END" << endl;
	}

	

	
	// ---------------------------------------------------------------------
	//
	// Signal Tests
	//
	// ---------------------------------------------------------------------		
	struct SignalBroadcaster
	{
		SignalBroadcaster() {}
		~SignalBroadcaster() {}
	
		Signal<int&>	TestSignal;
	};

	struct SignalReceiver
	{
		SignalBroadcaster& klass;
		SignalReceiver(SignalBroadcaster& klass) : klass(klass)
		{
			Log("debug") << "SignalReceiver: Starting" << endl;
			klass.TestSignal += delegate(this, &SignalReceiver::onSignal);
		}

		~SignalReceiver()
		{
			Log("debug") << "SignalReceiver: Destroying" << endl;	
			klass.TestSignal -= delegate(this, &SignalReceiver::onSignal);
		}

		void onSignal(void*, int& value)
		{
			Log("debug") << "SignalReceiver: Callback: " << value << endl;	
		}
	};
	 
	void runSignalReceivers() {
		{
			SignalBroadcaster broadcaster; //("Thread1");
			{
				SignalReceiver receiver(broadcaster);
			}
		}
		Util::pause();
	}


	/*
	// ---------------------------------------------------------------------
	//
	// Version String Comparison
	//
	// ---------------------------------------------------------------------		
	void runVersionStringComparison() 
	{
		//assert(Util::CompareVersion("3.7.8.0", "3.7.8") == false);
		//assert(Util::CompareVersion("3.7.9", "3.7.8") == true);
		//assert(Util::CompareVersion("1.7.9", "3.7.8") == false);
	}
	*/
};


} // namespace Sourcey


int main(int argc, char** argv) 
{	
	Logger::instance().add(new ConsoleChannel("Test", TraceLevel));
	{
		Sourcey::Tests app;
	}	
	Sourcey::Logger::uninitialize();
	Util::pause();
	return 0;
}