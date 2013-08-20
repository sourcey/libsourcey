#include "Sourcey/Base.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Runner.h"
#include "Sourcey/Idler.h"
#include "Sourcey/Signal.h"
#include "Sourcey/Buffer.h"
#include "Sourcey/Platform.h"
#include "Sourcey/Collection.h"
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


namespace
{
	class TestObj
	{
	public:
		TestObj(): _rc(1)
		{
			++_count;
		}
				
		void duplicate()
		{
			++_rc;
		}
		
		void release()
		{
			if (--_rc == 0)
				delete this;
		}
		
		int rc() const
		{
			return _rc;
		}
		
		static int count()
		{
			return _count;
		}
		
	protected:
		~TestObj()
		{
			--_count;
		}
		
	private:
		int _rc;
		static int _count;
	};
	
	int TestObj::_count = 0;
}


namespace scy {
	

class Tests
{
	Application app;

public:
	Tests()
	{	
		//testBuffer();
		//testHandle();
		//testNVCollection();
		//runFSTest();
		//runPluginTest();
		//runLoggerTest();
		//runPlatformTests();
		//runExceptionTest();
		//runScheduler::TaskTest();
		//testTimer();
		//testIdler();
		
		//runPacketStreamTests();
		//runPacketSignalTest();
		//runSocketTests();
		//runGarbageCollectorTests();
		//runSignalReceivers();
		
		scy::pause();
	}
	
	/*
	/*
	void testGZStream()
	{	

		std::string path("gzstream.tgz");
		std::string dest("gzstream");

		assert(fs::exists("gzstream.tgz"));

		// check alternate way of opening file
		igzstream in2;
		in2.open(path.c_str());
		if (!in2.good()) {
			std::cerr << "ERROR: Opening file `" << path << "' failed.\n";
			assert(0);
		}
		in2.close();
		if (!in2.good()) {
			std::cerr << "ERROR: Closing file `" << path << "' failed.\n";
			assert(0);
		}

		// now use the shorter way with the constructor to open the same file
		igzstream in(path.c_str());
		if (!in.good()) {
			std::cerr << "ERROR: Opening file `" << path << "' failed.\n";
			assert(0);
		}
		std::ofstream out(dest.c_str());
		if (!out.good()) {
			std::cerr << "ERROR: Opening file `" << dest << "' failed.\n";
			assert(0);
		}

		char c;
		while (in.get(c))
			out << c;
		in.close();
		out.close();
		if (!in.eof()) {
			std::cerr << "ERROR: Reading file `" << path << "' failed.\n";
			assert(0);
		}
		if (!out.good()) {
			std::cerr << "ERROR: Writing file `" << dest << "' failed.\n";
			assert(0);
		}
	}
	*/
	
	void testBuffer()
	{
		ByteOrder orders[2] = { ByteOrder::Host,
								ByteOrder::Network };
		for (size_t i = 0; i < 2; i++) {
			Buffer buffer(1024);
			BitReader reader(buffer, orders[i]);
			BitWriter writer(buffer, orders[i]);
			assert(orders[i] == reader.order());
			assert(orders[i] == writer.order());

			// Write and read UInt8.
			UInt8 wu8 = 1;
			writer.putU8(wu8);
			UInt8 ru8;
			reader.getU8(ru8);
			assert(wu8 == ru8);
			assert(writer.position() == 1);
			assert(reader.position() == 1);		

			// Write and read UInt16.
			UInt16 wu16 = (1 << 8) + 1;
			writer.putU16(wu16);
			UInt16 ru16;
			reader.getU16(ru16);
			assert(wu16 == ru16);
			assert(writer.position() == 3);
			assert(reader.position() == 3);
		
			// Write and read UInt24.
			UInt32 wu24 = (3 << 16) + (2 << 8) + 1;
			writer.putU24(wu24);
			UInt32 ru24;
			reader.getU24(ru24);
			assert(wu24 == ru24);
			assert(writer.position() == 6);
			assert(reader.position() == 6);
		
			// Write and read UInt32.
			UInt32 wu32 = (4 << 24) + (3 << 16) + (2 << 8) + 1;
			writer.putU32(wu32);
			UInt32 ru32;
			reader.getU32(ru32);
			assert(wu32 == ru32);
			assert(writer.position() == 10);
			assert(reader.position() == 10);
		
			// Write and read UInt64.
			UInt32 another32 = (8 << 24) + (7 << 16) + (6 << 8) + 5;
			UInt64 wu64 = (static_cast<UInt64>(another32) << 32) + wu32;
			writer.putU64(wu64);
			UInt64 ru64;
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
	// Handle Test
	//
	void testHandle()
	{
		{
			Handle<TestObj> ptr = new TestObj;
			assert (ptr->rc() == 1);
			Handle<TestObj> ptr2 = ptr;
			assert (ptr->rc() == 2);
			ptr2 = new TestObj;
			assert (ptr->rc() == 1);
			Handle<TestObj> ptr3;
			ptr3 = ptr2;
			assert (ptr2->rc() == 2);
			ptr3 = new TestObj;
			assert (ptr2->rc() == 1);
			ptr3 = ptr2;
			assert (ptr2->rc() == 2);
			assert (TestObj::count() > 0);
		}
		assert (TestObj::count() == 0);
	}


	// ============================================================================
	// Collection Test
	//
	void testNVCollection()
	{
		NVCollection nvc;
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
	
		NVCollection::ConstIterator it = nvc.find("Name3");
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
	}

	// ============================================================================
	// Plugin Test
	//
	typedef int (*GimmeFiveFunc)();

	void runPluginTest() 
	{
		debugL("PluginTest") << "Starting" << endl;
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
		catch (std::exception/*Exception*/& exc)
		{
			errorL("PluginTest") << "Error: " << exc << endl;
			assert(0);
		}
		
		cout << "Ending" << endl;
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
	void runLoggerTest() 
	{
		// Test default synchronous writer
		Logger::instance().setWriter(new LogWriter);		
		clock_t start = clock();
		for (unsigned i = 0; i < 1000; i++) 
			cout << "Test message: " << i << endl;
		cout << "#### synchronous test completed after: " << (clock() - start) << endl;
		
		// Test asynchronous writer (approx 10x faster)
		Logger::instance().setWriter(new AsyncLogWriter);		
		start = clock();
		for (unsigned i = 0; i < 1000; i++) 
			cout << "Test message: " << i << endl;
		cout << "#### asynchronous test completed after: " << (clock() - start) << endl;
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
			assert(std::string(exc.what())/*message()*/ == "IO error");
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
	
	void testTimer() 
	{
		cout << "Starting" << endl;
		Timer timer;
		timer.Timeout += delegate(this, &Tests::timerCallback);
		timer.start(10, 10);

		timerRestarted = false;
		
		uv_ref(timer.handle()); // timers do not reference the loop
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
		idler.start(std::bind(&Tests::idlerCallback, this));

		idlerTicks = 0;
		uv_ref(idler.ptr.handle()); // idlers do not reference the loop
		runLoop();
	}

	void idlerCallback()
	{
		cout << "On idle" << endl;
		if (++idlerTicks == numTimerTicks) {
			idler.stop(); // event loop will be released
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
	// Version String Comparison
	//	
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