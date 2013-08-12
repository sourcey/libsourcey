#include "Sourcey/Base.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Runner.h"
#include "Sourcey/Signal.h"
#include "Sourcey/Platform.h"
#include "Sourcey/Containers.h"
#include "Sourcey/Application.h"
#include "Sourcey/PacketStream.h"
#include "Sourcey/SharedLibrary.h"
#include "Sourcey/FileSystem.h"
#include "Sourcey/Timer.h"
#include "Sourcey/Util.h"

#include "plugin/TestPlugin.h"

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
	

class Tests
{
	Application app;

public:
	Tests()
	{	
		runNVHashTest();
		//runFSTest();
		//runPluginTest();
		//runLoggerTest();
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

	void runNVHashTest()
	{
		NVHash nvc;
	
		nvc.add("Connection", "value31");
		nvc.add("Authorization", "value31");
		nvc.add("Content-Type", "value31");
		nvc.add("Connection", "value31");
		nvc.add("Authorization", "value31");
		nvc.add("Content-Type", "value31");
		/*
		assert (nvc.empty());
		assert (nvc.size() == 0);
	
		nvc.set("name", "value");
		assert (!nvc.empty());
		assert (nvc["name"] == "value");
		assert (nvc["Name"] == "value");
	
		nvc.set("name2", "value2");
		assert (nvc.get("name2") == "value2");
		assert (nvc.get("NAME2") == "value2");
	
		assert (nvc.size() == 2);
	
		try
		{
			std::string value = nvc.get("name3");
			assert(0 && "not found - must throw");
		}
		catch (NotFoundException&)
		{
		}

		try
		{
			std::string value = nvc["name3"];
			assert(0 && "not found - must throw");
		}
		catch (NotFoundException&)
		{
		}
	
		assert (nvc.get("name", "default") == "value");
		assert (nvc.get("name3", "default") == "default");

		assert (nvc.has("name"));
		assert (nvc.has("name2"));
		assert (!nvc.has("name3"));	
	
		nvc.add("name3", "value3");
		assert (nvc.get("name3") == "value3");
	
		nvc.add("name3", "value31");
		
		nvc.add("Connection", "value31");
	
		NVHash::ConstIterator it = nvc.find("Name3");
		assert (it != nvc.end());
		std::string v1 = it->second;
		assert (it->first == "name3");
		++it;
		assert (it != nvc.end());
		std::string v2 = it->second;
		assert (it->first == "name3");
	
		assert ((v1 == "value3" && v2 == "value31") || (v1 == "value31" && v2 == "value3"));
	
		nvc.erase("name3");
		assert (!nvc.has("name3"));
		assert (nvc.find("name3") == nvc.end());
	
		it = nvc.begin();
		assert (it != nvc.end());
		++it;
		assert (it != nvc.end());
		++it;
		assert (it == nvc.end());
	
		nvc.clear();
		assert (nvc.empty());
	
		assert (nvc.size() == 0);
		*/
	}


	// ============================================================================
	// FileSystem Test
	//
	void runFSTest() 
	{
		std::string path(scy::getExePath());
		debugL("FileSystemTest") << "Executable path: " << path << endl;

		//std::string junkPath(path + "junkname.huh");
		//debugL("FileSystemTest") << "Junk path: " << junkPath << endl;

		std::string dir(fs::dirname(path));
		debugL("FileSystemTest") << "Dir name: " << dir << endl;

		std::vector<std::string> res;
		assert(fs::readdir(dir, res));
		for (unsigned i = 0; i < res.size(); i++) 
			debugL("FileSystemTest") << "Enumerating file: " << res[i] << endl;

		
	}

	// ============================================================================
	// Plugin Test
	//
	typedef int (*GimmeFiveFunc)();

	void runPluginTest() 
	{
		debugL("PluginTest") << "Starting" << endl;
		// TODO: Use getExePath
		string path("D:/dev/projects/Sourcey/LibSourcey/build/install/libs/TestPlugin/TestPlugind.dll");
		
		try
		{
			//
			// Load the shared library
			SharedLibrary lib;
			lib.open(path, true);
			
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
			if (info->abiVersion != SOURCEY_PLUGIN_ABI_VERSION)
				throw Exception(util::format("Module version mismatch. Expected %s, got %s.", SOURCEY_PLUGIN_ABI_VERSION, info->abiVersion));
			
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
		catch (Exception& exc)
		{
			errorL("PluginTest") << "Error: " << exc << endl;
			assert(0);
		}
		
		cout << "Ending" << endl;
		debugL("PluginTest") << "Ending" << endl;
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
		// Test default synchronous writer
		Logger::instance().setWriter(new LogWriter);		
		clock_t start = clock();
		for (unsigned i = 0; i < 1000; i++) 
			debugL("LoggerTest") << "Test message: " << i << endl;
		debugL("LoggerTest") << "#################### Synchronous test completed after: " << (clock() - start) << endl;
		
		// Test asynchronous writer (10x faster)
		Logger::instance().setWriter(new AsyncLogWriter);		
		start = clock();
		for (unsigned i = 0; i < 1000; i++) 
			debugL("LoggerTest") << "Test message: " << i << endl;
		debugL("LoggerTest") << "#################### Asynchronous test completed after: " << (clock() - start) << endl;
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
			debugL() << "TestScheduler::Task: Running" << endl;
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
			debugL() << "SimplePacketProcessor: Processing: " << packet.className() << endl;
			
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
			debugL() << "SignalReceiver: Starting" << endl;
			klass.TestSignal += delegate(this, &SignalReceiver::onSignal);
		}

		~SignalReceiver()
		{
			debugL() << "SignalReceiver: Destroying" << endl;	
			klass.TestSignal -= delegate(this, &SignalReceiver::onSignal);
		}

		void onSignal(void*, int& value)
		{
			debugL() << "SignalReceiver: Callback: " << value << endl;	
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
	*/


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
		app.finalize();
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