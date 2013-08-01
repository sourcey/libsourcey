#include "Sourcey/Base.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Runner.h"
#include "Sourcey/Signal.h"
#include "Sourcey/Platform.h"
#include "Sourcey/Application.h"
#include "Sourcey/PacketStream.h"
#include "Sourcey/Timer.h"
#include "Sourcey/Util.h"

#include <assert.h>


using namespace std;
using namespace scy;


/*
// Detect Memory Leaks
#ifdef _DEBUG
#include "MemLeakDetect/MemLeakDetect.h"
CMemLeakDetect memLeakDetect;
#endif
*/


namespace scy {


/*
class Logger1 {
public:
	Logger1() 
	{
	}

	AsyncQueue queue;
}
*/
	

class Tests
{
	Application app;

public:
	Tests()
	{	
		runLoggerTest();
		//runPlatformTests();
		//runExceptionTest();
		//runTimerTest();
		//runScheduler::TaskTest();
		//runTimerTest();
		//runPacketStreamTests();
		//runPacketSignalTest();
		//runSocketTests();
		//runGarbageCollectorTests();
		//runSignalReceivers();
		
		scy::pause();
	}

	// ============================================================================
	// Platform Test
	//
	void runPlatformTests() 
	{
		debugL("PlatformTests") << "Executable Path: " << scy::getExePath() << endl;
		debugL("PlatformTests") << "Current Working Directory: " << scy::getCWD() << endl;
	}

	// ============================================================================
	// Logger Test
	//
	void runLoggerTest() 
	{
		clock_t start = clock();
		for (unsigned i = 0; i < 1000; i++) { 
			debugL("LoggerTest") << "Test message: " << i << endl;
		}

		runLoop();

		debugL("LoggerTest") << "#################### Completed after: " << (clock() - start) << endl;
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
			debugL("ExceptionTests") << "Message: " << exc << endl;
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
			debugL("ExceptionTests") << "Message: " << exc << endl;
			assert(exc.message() == "IO error");
		}
		catch (Exception&)
		{
			assert(0 && "bad cast");
		}
	}
	
	// ============================================================================
	// Timer Test
	//
	const static int numTimerTicks = 5;
	bool timerRestarted;

	void runTimerTest() 
	{
		traceL("TimerTest") << "Starting" << endl;
		Timer timer;
		timer.Timeout += delegate(this, &Tests::onOnTimerTimeout);
		timer.start(10, 10);

		timerRestarted = false;
		
		uv_ref(timer.handle()); // timers do not reference the loop
		runLoop();
		traceL("TimerTest") << "Ending" << endl;
	}

	void onOnTimerTimeout(void* sender)
	{
		Timer* timer = reinterpret_cast<Timer*>(sender);
		traceL("TimerTest") << "On timeout: " << timer->count() << endl;
		if (timer->count() == numTimerTicks) {
			if (!timerRestarted) {
				timerRestarted = true;
				timer->restart(); // restart once, count returns to 0
			}
			else
				timer->stop(); // event loop will be released
		}
	}

	/*
	// ============================================================================
	// Scheduled Task Tests
	//
	struct TestScheduler::Task: public Scheduler::Task
	{
		TestScheduler::Task(Runner& runner) : 
			Scheduler::Task(runner) {}

		void run() 
		{
			Log("debug") << "TestScheduler::Task: Running" << endl;
			ready.set();	
		}
	};

	void runScheduler::TaskTest() 
	{
		Log("trace") << "Running Scheduled Task Test" << endl;

		// Schedule to fire 1 second from now.
		{
			TestScheduler::Task* task = new TestScheduler::Task(runner);
			Poco::DateTime dt;
			Poco::Timespan ts(1, 0);
			dt += ts;
			task->schedule(dt);
			task->start();
			ready.wait();
		}
		
		// Schedule to fire once now, and twice at 1 second intervals.
		{
			TestScheduler::Task* task = new TestScheduler::Task(runner);
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
		//util::pause();
	}


	// ============================================================================
	// Packet Signal Tests
	//
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
		BroadcastPacket.emit(this, packet);
		//util::pause();
		Log("trace") << "Running Packet Signal Test: END" << endl;
	}

	
	// ============================================================================
	// Packet Stream Tests
	//
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
		//util::pause();
		//cap.stop();
	}
	

	// ============================================================================
	// Garbage Collector Tests
	//
	void runGarbageCollectorTests() {
		Log("trace") << "Running Garbage Collector Test" << endl;
		
		//for (unsigned i = 0; i < 100; i++) { 
			char* ptr = new char[1000];
		
			Poco::Thread* ptr1 = new Poco::Thread;
		
			TaskRunner::getDefault().deleteLater<char*>(ptr);
			//TaskRunner::getDefault().deleteLater<Poco::Thread>(ptr1);
		//}

		//util::pause();
		Log("trace") << "Running Garbage Collector Test: END" << endl;
	}
	
	// ============================================================================
	// Timer Task Tests
	//
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
		//util::pause();
		Log("trace") << "Running Timer Task Test: END" << endl;
	}
	*/
	

	
	// ============================================================================
	// Signal Tests
	//
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
		//util::pause();
	}


	/*
	// ============================================================================
	//
	// Version String Comparison
	//
	// ============================================================================		
	void runVersionStringComparison() 
	{
		//assert(util::CompareVersion("3.7.8.0", "3.7.8") == false);
		//assert(util::CompareVersion("3.7.9", "3.7.8") == true);
		//assert(util::CompareVersion("1.7.9", "3.7.8") == false);
	}
	*/

	void runLoop() {
		debugL("Tests") << "#################### Running" << endl;
		app.run();
		debugL("Tests") << "#################### Ended" << endl;
	}

	void runCleanup() {
		debugL("Tests") << "#################### Finalizing" << endl;
		app.cleanup();
		debugL("Tests") << "#################### Exiting" << endl;
	}
	
};


} // namespace scy


int main(int argc, char** argv) 
{	
	Logger::instance().add(new ConsoleChannel("Test", LTrace));
	{
		scy::Tests app;
	}	
	Logger::shutdown();
	//util::pause();
	return 0;
}