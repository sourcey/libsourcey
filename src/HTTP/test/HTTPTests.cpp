/*
#include "Poco/Net/NetException.h"
#include "Poco/Net/SocketAcceptor.h"
#include "Poco/Net/SocketReactor.h"

#include "Sourcey/Base/Logger.h"
#include "Sourcey/Base/Timer.h"
#include "Sourcey/Base/TaskRunner.h"
#include "Sourcey/Base/Signal.h"
#include "Sourcey/Base/PacketStream.h"
#include "Sourcey/Net/UDPSocket.h"
#include "Sourcey/STUN/Transaction.h"


//#include "Sourcey/Base/TCPService.h"
//#include "Sourcey/Base/TCPClient.h"
#include "Sourcey/Base/Callback.h"
//#include "Sourcey/Base/Authenticator.h"
#include "Sourcey/STUN/Transaction.h"
//#include "Sourcey/Media/MJPEGPacketizer.h"
#include "Sourcey/Media/AudioCapture.h"
#include "Sourcey/Media/AudioEncoder.h"
#include "Sourcey/Media/VideoEncoder.h"
#include "Sourcey/Media/CaptureEncoder.h"
#include "Sourcey/RTP/Packet.h"
//#include "Sourcey/RTP/DataProvider.h"
//#include "Sourcey/RTP/DePacketizer.h"
#include "Sourcey/Media/VideoCapture.h"
*/

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <assert.h>


using namespace std;
//using namespace Poco;
//using namespace Poco::Net;


// Detect Memory Leaks
#ifdef _DEBUG
#include "MemLeakDetect/MemLeakDetect.h"
CMemLeakDetect memLeakDetect;
#endif


namespace Sourcey {

	
class Tests
{
public:
	Tests()
	{	
		//assert(CompareVersion("3.7.8.0", "3.7.8") == false);
		//assert(CompareVersion("3.7.9", "3.7.8") == true);
		//assert(CompareVersion("1.7.9", "3.7.8") == false);
		//runTimerTests();
		//runPacketStreamTests();
		//runPacketSignalReceivers();
		//runSocketTests();
		//runSTUNTests();
		//runGarbageCollectorTests();
		//runSignalReceivers();
	}


	/*
	// ----------------------------------------------------------------------------
	//
	// Packet Stream Tests
	//
	// ----------------------------------------------------------------------------	
	void onBroadcastPacket(void* sender, DataPacket& packet) //IPacketSource& source, , PacketStream*& stream
	{
		Log("trace") << "[Tests:" << this << "] On Packet: " << packet.className() << std::endl;
	}

	PacketSignal BroadcastPacket; // test polymorphic packet signal
	
	void runPacketSignalReceivers() 
	{
		Log("trace") << "[Tests:" << this << "] runPacketSignalReceivers" << std::endl;
		BroadcastPacket += PacketDelegate<Tests, DataPacket>(this, &Tests::onBroadcastPacket, 0);
		DataPacket packet;
		BroadcastPacket.send(this, packet);
		system("pause");
		Log("trace") << "[Tests:" << this << "] runPacketSignalReceivers: END" << std::endl;

	}


	// ----------------------------------------------------------------------------
	//
	// Packet Stream Tests
	//
	// ----------------------------------------------------------------------------	
	class SimplePacketProcessor: public IPacketProcessor
	{
		void process(IPacket& packet) 
		{
			Log("debug") << "SimplePacketProcessor: Processing: " << packet.className() << std::endl;
			
			BroadcastPacket.send(this, packet);
		}
	};
	
	void onPacketStreamPacket(void* sender, IPacket& packet) 
	{
		Log("trace") << "[Tests:" << this << "] On Packet: " << packet.className() << std::endl;
	}

	void runPacketStreamTests() 
	{
		PacketStream stream;
		stream.addReceiver(packetDelegate(this, &Tests::onPacketStreamPacket, 0));

		// init source
		//Media::VideoCapture cap(0);
		//stream.attach(&cap);
		
		// init proc
		//SimplePacketProcessor proc;
		//stream.attach(&proc);	

		// run!		
		//cap.start();
		system("pause");
		//cap.stop();
	}

	// ----------------------------------------------------------------------------
	//
	// STUN Tests
	//
	// ----------------------------------------------------------------------------	
	void runSTUNTests() {
		STUN::Message request;

		// PRIORITY
		STUN::ICEPriority* priorityAttr = new STUN::ICEPriority();
		priorityAttr->setValue(888888);
		request.add(priorityAttr);

		// USE-CANDIDATE
		request.add(new STUN::ICEUseCandidate);
		
		Buffer buf;
		request.write(buf);
		
		STUN::Message response;
		response.read(buf);

		Log("debug") << "STUN Message: " << response.toString() << std::endl;

		system("pause");
	}

	// ----------------------------------------------------------------------------
	//
	// Garbage Collector Tests
	//
	// ----------------------------------------------------------------------------	
	void runGarbageCollectorTests() {
		
		//for (unsigned i = 0; i < 100; i++) { 
			char* ptr = new char[1000];
		
			Poco::Thread* ptr1 = new Poco::Thread;
		
			TaskRunner::getDefault().deleteLater<char*>(ptr);
			//TaskRunner::getDefault().deleteLater<Poco::Thread>(ptr1);
		//}

		system("pause");
	}

	// ----------------------------------------------------------------------------
	//
	// Socket Tests
	//
	// ----------------------------------------------------------------------------
	struct SocketThread: public Poco::Runnable 
	{		
		// ----------------------------------------------------------------------------
		//
		struct SocketReceiver
		{
			UDPSocket socket;

			SocketReceiver(const string& bindIP = "127.0.0.1", short bindPort = 0)
			{
				socket.addPacketType<STUN::Message>(1);
				socket.bind(SocketAddress(bindIP, bindPort), true);
				socket.addReceiver(packetDelegate(this, &SocketReceiver::onDataPacketReceived, 0));
				socket.addReceiver(packetDelegate(this, &SocketReceiver::onSTUNPacketReceived, 100));
				Log("debug") << "SocketReceiver: Starting: PORT: " << socket.localAddr().toString() << std::endl;
			}

			~SocketReceiver()
			{
				Log("debug") << "SocketReceiver: Destroying" << std::endl;	
				socket.removeReceiver(packetDelegate(this, &SocketReceiver::onDataPacketReceived));
				socket.removeReceiver(packetDelegate(this, &SocketReceiver::onSTUNPacketReceived));
				socket.close();
			}
			
			void onDataPacketReceived(void* sender, DataPacket& packet)
			{
				Log("trace") << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
				Log("trace") << "[Tests:" << this << "] On Packet Received: " << packet.className() << std::endl;
			}
			
			void onSTUNPacketReceived(void* sender, STUN::Message& packet)
			{
				Log("trace") << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
				Log("trace") << "[Tests:" << this << "] On STUN Packet Received: " << packet.className() << std::endl;
			}
		};
		
		SocketThread(const std::string& name) : _name(name) {
			_thread.start(*this);
		}
	
		void run() 	{
			// start and destroy multiple receivers
			//for (unsigned i = 0; i < 100; i++) { //0000
			//	Log("debug") << "SocketThread: " << _name << std::endl;
			//	//new SocketReceiver;
			//	SocketReceiver test;
			//	//UDPSocket sock;
			//}				

			SocketReceiver receiver;	
			UDPSocket sendSocket;
			for (int i = 0; i < 1000; i++) {
				// 1 in 10 packets is a STUN message
				if (i % 10) {
					
					Log("debug") << "SocketSender: STUN: >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << std::endl;	

					STUN::Message request;
					request.setType(STUN::Message::Refresh);

					sendSocket.send(request, receiver.socket.localAddr());
				}

				else {
					Log("debug") << "SocketSender: DATA: !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;	

					unsigned char* data = new unsigned char[100];
					DataPacket packet(data, 100);
					sendSocket.send(packet, receiver.socket.localAddr());
					delete data;
				}
			}
		}
	
		Poco::Thread		_thread;
		std::string			_name;
	};

	void runSocketTests() {
		SocketThread run("Thread");
		//new SocketThread::SocketTest("localhost", 3000);
		new SocketThread::SocketTest("127.0.0.1", 3000);
		new SocketThread::SocketTest("192.168.1.64", 3000);
		SocketThread run("Thread");
		SocketThread run1("Thread1");
		SocketThread run2("Thread2");
		SocketThread run3("Thread3");
		SocketThread run4("Thread4");
		system("pause");
	}

	
	// ----------------------------------------------------------------------------
	//
	// Timer Tests
	//
	// ----------------------------------------------------------------------------
	struct TimerThread: public Poco::Runnable 
	{				
		struct TimerTest
		{
			TimerTest(int timeout = 1000)
			{
				Timer::getDefault().start(TimerCallback<TimerTest>(this, &TimerTest::onTimer, timeout)); //, 1000, 5
				//Timer::getDefault().stop(TimerCallback<TimerTest>(this, &TimerTest::onTimer));
				Log("debug") << "TimerTest: Starting" << std::endl;	
			}

			~TimerTest()
			{
				Log("debug") << "TimerTest: Destroying" << std::endl;	
			}

			void onTimer(TimerCallback<TimerTest>& timer)
			{
				Log("debug") << "TimerTest: Callback" << std::endl;	
				Timer::getDefault().stop(TimerCallback<TimerTest>(this, &TimerTest::onTimer));
				delete this;
			}
		};

		TimerThread(const std::string& name) : _name(name) {
			_thread.start(*this);
		}
	
		void run() 	{
			for (unsigned i = 0; i < 100; i++) {
				Log("debug") << "TimerThread: " << _name << std::endl;
				new TimerTest;
			}
		}
	
		Poco::Thread		_thread;
		std::string			_name;
	};
	 
	void runTimerTests() {
		new TimerThread::TimerTest(3000);
		new TimerThread::TimerTest(3000);
		system("pause");
	}

	

	
	// ----------------------------------------------------------------------------
	//
	// Signal Tests
	//
	// ----------------------------------------------------------------------------		
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
			Log("debug") << "SignalReceiver: Starting" << std::endl;
			klass.TestSignal += delegate(this, &SignalReceiver::onSignal);
		}

		~SignalReceiver()
		{
			Log("debug") << "SignalReceiver: Destroying" << std::endl;	
			klass.TestSignal -= delegate(this, &SignalReceiver::onSignal);
		}

		void onSignal(void*, int& value)
		{
			Log("debug") << "SignalReceiver: Callback: " << value << std::endl;	
		}
	};
	 
	void runSignalReceivers() {
		{
			SignalBroadcaster broadcaster; //("Thread1");
			{
				SignalReceiver receiver(broadcaster);
			}
		}
		system("pause");
	}
		*/
};


} // namespace Sourcey


int main(int argc, char** argv) 
{	
	//Sourcey::Logger::instance().add(new Sourcey::ConsoleChannel("debug", Sourcey::TraceLevel));
	{

		Sourcey::Tests app;
		//app.run(argc, argv);
		//Sourcey::TURN::RelayServer app;
		//app.run(); //argc, argv
	}
	
	//Sourcey::Logger::uninitialize();
	system("pause");
	return 0;
}


	/*
	Buffer	_xmlBuffer;
	Timeout	_readTimeout;
	void buildXML(const string& data)
	{
		Log("trace") << "[XMPPConnectionHandler] Buffer length: " << _xmlBuffer.length() << endl;
		Log("trace") << "[XMPPConnectionHandler] Buffer position: " << _xmlBuffer.position() << endl;

		// Clear the current read buffer if it is expired.
		if (_xmlBuffer.length() > 0 &&
			_readTimeout.expired()) {
			Log("trace") << "[XMPPConnectionHandler] Resetting Expired Read Buffer" << endl;
			_readTimeout.reset();
			_xmlBuffer.clear();
		}

		// Resize the read buffer as necessary.
		int totalLength = data.length() + _xmlBuffer.length();
		if (_xmlBuffer.capacity() < totalLength) {
			Log("trace") << "[XMPPConnectionHandler] Resizing Read Buffer: " << totalLength << endl;
			_xmlBuffer.resize(totalLength);
		}

		int size = data.length();
			//socket().receiveBytes(_xmlBuffer.data(), socket().available());
		//_xmlBuffer.writeString(data);
		memcpy(_xmlBuffer.bytes() + _xmlBuffer.length(), data.data(), data.length());	
		_xmlBuffer.length(_xmlBuffer.length() + data.length());	
		//_xmlBuffer += _xmlBuffer.position() + size;

		try {
			//Log("error") << "[XMPPConnectionHandler] Creating XML Doc: " << _xmlBuffer.length() << ": " << string(_xmlBuffer.bytes(), _xmlBuffer.length()) << endl;
			
						if (_xmlBuffer.length() > 100) {
							Log("error") << "[XMPPConnectionHandler] Creating XML Doc First 100: " << _xmlBuffer.length() << ": " << string(_xmlBuffer.bytes(), 100) << endl;
							Log("error") << "[XMPPConnectionHandler] Creating XML Doc Lase 100: " << _xmlBuffer.length() << ": " << string(_xmlBuffer.bytes()[_xmlBuffer.length() - 100], 100) << endl;
						}

			XML::Document doc;
			pugi::xml_parse_result result = doc.load(_xmlBuffer.bytes());
			if (!result) {
				// We may have a fragmented XML request, start the read timer
				// if it is not running.							
				if (!_readTimeout.running()) {
					_readTimeout.start();
				}

				throw Exception(result.description()); 
			}
						
			Log("error") << "[XMPPConnectionHandler] Creating XMPP Stanza: " << doc.first_child().name() << endl;
						
			doc.print(cout);
		}
		catch (Exception& exc)
		{
			Log("error") << "[XMPPConnectionHandler] Bad XMPP Stanza: " << exc.displayText() << endl;
		}

		//return true;
	}

	void runBuildXMLString()
	{
		//xmlStream = "";
		_readTimeout.setDelay(100);

		string data = "<data xmlns=\"heyyy\">"
			"grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1"
			"grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1"
			"grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1"
			"grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1"
			"grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1grrrrrrrrrrrrrrrrrrrrr1";
		buildXML(data);

		//Sleep(1000);

		data = "2grrrrrrrrrrrrrrrrrrrrr3";
		buildXML(data);

		data = "4grrrrrrrrrrrrrrrrrrrrr</data>";
		buildXML(data);
	}

class Tests//: public Poco::Util::ServerApplication//, public STUN::TransactionHandler, public PacketHandler<Tests, STUN::Message>
{
public:
	//virtual void onSTUNMessage(STUN::Message* packet) {
	//	Log("debug") << "_s2: onSTUNMessage: " << std::endl;
	//};
	//	Log("debug") << "_s2: onPacket: " << std::endl;
	//};
	//virtual void onDataPacketReceived(PacketSocket* socket, DataPacket* packet, const SocketAddress& localAddr, const SocketAddress& remoteAddr) {
	//	Log("debug") << "_s2: onDataPacketReceived: Data: " << std::endl;	
	//}
	virtual void onSTUNPacketReceived1(PacketSocket* socket, STUN::Message* message, const SocketAddress& localAddr, const SocketAddress& remoteAddr) {
		Log("debug") << "_s1: onDataPacketReceived: STUN: " << std::endl;	
	}

	virtual void onSTUNPacketReceived2(PacketSocket* socket, STUN::Message* message, const SocketAddress& localAddr, const SocketAddress& remoteAddr) {
		Log("debug") << "_s2: onSTUNPacketReceived2: STUN: " << std::endl;	
		
		//Buffer buf;
		//message->write(buf);
		//_s2->send(buf.bytes(), buf.length(), _s1->localAddr());
	}


	// --------------------------------------------------
	*/



	/*
	virtual void onDataPacketReceived(PacketSocket* socket, Packet* packet, const SocketAddress& localAddr, const SocketAddress& remoteAddr) {
		Log("debug") << "_s2: onDataPacketReceived: " << std::endl;		
		//Log("debug") << "_s2: " << packet->name() << std::endl;
		
		STUN::Message* message = dynamic_cast<STUN::Message*>(packet);

		Buffer buf;
		//STUN::Message response;
		message->write(buf);
		_s2->send(buf.bytes(), buf.length(), _s1->localAddr());
	};
	*/

	/*
	virtual void onSTUNTransactionComplete(STUN::Transaction* transaction, PacketSocket* socket) {
		Log("debug") << "s1: STUNTransactionComplete" << std::endl;

		Buffer buf;
		STUN::Message response;
		response.write(buf);
		_s1->send(response, _s2->localAddr());
	};

	virtual void onSTUNDataReceived(Buffer* buffer) {
	};

	virtual bool onTimeout() {
		Log("debug") << "onTimeout: " << endl;
		//Timer::getDefault().stop(TimerCallback<Tests>(this, &Tests::onTimeout));
		return false;
	};

	virtual void onTimer() { //TimerNotification* nf) {
		Log("debug") << " onTimer: " << endl; // << nf->
	};
	virtual void onSTUNTransactionTimeout(STUN::Transaction* transaction) {
		Log("debug") << "s1: STUNTransactionTimeout" << std::endl;
	};
	virtual void onSTUNTransactionStateChanged(void* transaction, STUN::TransactionState& state) {
		Log("debug") << "s1: onSTUNTransactionStateChanged: " << state.str() << std::endl;

		Buffer buf;
		STUN::Message response;
		response.write(buf);
		_s1->send(response, _s2->localAddr());
	};

	virtual void onDataPacketReceived1(PacketSocket* socket, const DataPacket& packet, const SocketAddress& localAddr, const SocketAddress& remoteAddr) {
		Log("debug") << "_s1: onDataPacketReceived1" << std::endl;	
	}

	virtual void onDataPacketReceived2(PacketSocket* socket, const DataPacket& packet, const SocketAddress& localAddr, const SocketAddress& remoteAddr) {
		Log("debug") << "_s2: onDataPacketReceived1" << std::endl;	
		
		//Buffer buf;
		//message->write(buf);
		//_s2->send(buf.bytes(), buf.length(), _s1->localAddr());
	}

	void onRTPPacket(void*,Packet& packet) {
		Log("debug") << "############# onRTPPacket: " << &packet << std::endl;
	};

	virtual void onSignal(void* sender, DataPacket& data) {
		Log("debug") << "onSignal: " << &data << std::endl;
		//TestSignal -= delegate(this, &Tests::onSignal, 100); //<Polymorphic&>
	};

	virtual void onSignal2(void* sender, DataPacket& packet, const User*& user) {
		Log("debug") << "onSignal2: packet.size: " << packet.size << std::endl;
		packet.size = 5000;
		user = new BasicUser("grrr", "fuck");
		Log("debug") << "onSignal2: user: " << user << std::endl;
	};

	virtual void onSignal3(void* sender, Polymorphic& data, Polymorphic& data2, Polymorphic& data3) {
		Log("debug") << "onSignal3: " << &data3 << std::endl;
	};


	//void onVideoEncoded(void* sender, Sourcey::Packet& packet) 
	void onVideoEncoded1(Sourcey::Packet& packet) 
	{
		cout << "VideoFrameEncoded: " << &packet  << endl;	
		Buffer buf;
		packet.write(buf);
		//_s1->send(packet, SocketAddress("127.0.0.1", 8880));
	}

	Signal<DataPacket&> TestSignal;
	Signal2<DataPacket&, const User*&> TestSignal2;
	Signal3<Polymorphic&, Polymorphic&, Polymorphic&> TestSignal3;

	virtual void onDataPacketReceived(PacketSocket* socket, const DataPacket& packet, const SocketAddress& localAddr, const SocketAddress& remoteAddr) {
		Log("debug") << "TCP: onDataPacketReceived: " << std::endl;	
	}
	*/

		/*
	Tests() {
		const vector<string>& args
		Packet* src = new Packet((unsigned char*)"test", 4, 1, 111111, 999999);
		cout << "src: " << string(src->getBytes(), src->size()) << endl;

		Packet* dest = Packet::readPacket((unsigned char*)src->getBytes(), src->size());
		cout << "dest: " << string(dest->getBytes(), dest->size()) << endl;


		Notifier::getDefault().addObserver(Observer<Tests, TimeoutNotification>(this, &Tests::onTimeout));
		Notifier::getDefault().addObserver(Observer<Tests, TimerNotification>(this, &Tests::onTimer));
		//Notifier::getDefault().removeObserver(Observer<Tests, TimeoutNotification>(this, &Tests::onTimeout));

		Notifier::getDefault().enqueue(new TimeoutNotification(this), 1000);
		//ProtocolFactory::getDefault().registerProtocol<STUN::STUNProtocol>(new STUN::STUNProtocol(), 100);
		Notifier::getDefault().enqueue(new TimerNotification(1000, this), 1000);
		//ProtocolFactory::getDefault().registerProtocol<STUN::STUNProtocol>(new STUN::STUNProtocol(), 100);

		Timer::getDefault().start(TimerCallback<Tests>(this, &Tests::onTimeout, 1000, 1000));
		Timer::getDefault().start(TimerCallback<Tests>(this, &Tests::onTimeout, 2000, 1000));
		Timer::getDefault().start(TimerCallback<Tests>(this, &Tests::onTimeout, 3000, 1000));
		//Timer::getDefault().start(TimerCallback<Tests>(this, &Tests::onTimeout));
		//Timer::getDefault().start(TimerCallback<Tests>(this, &Tests::onTimeout));
		DataProvider<Media::MJPEGPacketizer<Packet>> provider = DataProvider<Media::MJPEGPacketizer<Packet>>(NULL);
		provider += callback(this, &Tests::onRTPPacket);

		DataProvider<Media::MJPEGPacketizer<Packet>>* pprovider = new DataProvider<Media::MJPEGPacketizer<Packet>>(NULL);
		//RTP::DataProvider<RTP::Packet>* provider = new RTP::DataProvider<RTP::Packet>(NULL, NULL, "initiator-video-stream", "video");
		*pprovider += callback(this, &Tests::onRTPPacket);

		try	{
		*/	

			//TestSignal += delegate<Tests, Polymorphic>(this, &Tests::onSignal, 100);
			//TestSignal += delegate(this, &Tests::onSignal, 500); //<Polymorphic&>
			//TestSignal += delegate(this, &Tests::onSignal, 100); //<Polymorphic&>
			//TestSignal.send(this, DataPacket());

			//TCPService* svc = new TCPService(new Poco::Net::TCPServerConnectionFactory(), 8880);
			//svc->
			//Poco::Net::ServerSocket ss(SocketAddress("127.0.0.1", 8880));
			//Poco::Net::SocketAcceptor<EchoServiceHandler> acceptor(ss, Reactor::getDefault());

			//TCPClient* tcp = new TCPClient(SocketAddress("127.0.0.1", 8880));
			//tcp->connect(SocketAddress("127.0.0.1", 8880));
			//tcp->addHandler(PacketHandler<Tests, DataPacket>(this, &Tests::onDataPacketReceived));
			//tcp->send("test", 4);
			//tcp->send("test", 4);
			//Reactor::getDefault().run();

			/*
			TestSignal2 += delegate(this, &Tests::onSignal2, 100);
			
			const User* user1 = NULL; //new BasicUser("grrr", "fuck");
			DataPacket packet;
			TestSignal2.send(this, packet, user1);
			Log("debug") << "packet.size: " << packet.size << std::endl;
			cout << "user1: " << user1 << endl;
			assert(user1);

			TestSignal3 += delegate(this, &Tests::onSignal3, 100);
			TestSignal3.send(this, DataPacket(), DataPacket(), DataPacket());
			*/
			/*

			_s1 = new UDPSocket();
			_s1->bind(SocketAddress("127.0.0.1", 8880));
			//_s1->addHandler(PacketHandler<Tests, STUN::Message>(this, &Tests::onSTUNPacketReceived1, 100));
			_s1->addHandler(PacketHandler<Tests, DataPacket>(this, &Tests::onDataPacketReceived1, 100));
			
			//UDPSocket* _s2 = new UDPSocket();
			//_s2->bind(SocketAddress("127.0.0.1", 9990));
			//_s2->addHandler(PacketHandler<Tests, STUN::Message>(this, &Tests::onSTUNPacketReceived2, 100));
			//_s2->addHandler(PacketHandler<Tests, DataPacket>(this, &Tests::onDataPacketReceived2, 100));
		
			for (;;) {
				char* temp = new char[1500];
				//DataPacket packet((unsigned char*)temp, 1500);
				//_s1->send(packet, SocketAddress("127.0.0.1", 8880));
				_s1->send(temp, 1500, SocketAddress("127.0.0.1", 8880));
				delete temp;
			}
			Media::VideoCapture cap("test.avi", true);
			Media::CaptureEncoder<Media::VideoEncoder> enc(&cap);

			Media::VideoParams oParams(Media::MJPEG, 400, 300, 10);
			enc.initEncodeContext(oParams);
			Media::VideoParams iParams(Media::NoVideo, cap.width(), cap.height(), 0, Media::BGR24);
			enc.initScaleContext(iParams);

			enc.PacketEncoded += delegate(this, &Tests::onVideoEncoded);	
			enc.start();
			system("pause");
			enc.stop();

			RTP::DataProvider<Sourcey::Packet> provider(&enc, 
				new RTP::VideoCodec(26, "JPEG", 90000, 400, 300, 1, 0), "Test", "video", "RTP/AVP");			
			provider += callback(this, &Tests::onVideoEncoded1);

			provider.start();
			system("pause");
			provider.stop();
			*/

			//Packet* src = new Packet((unsigned char*)"test", 4, 1, 111111, 999999);

			/*
			//_s1->send(src->getBytes(), src->size(), _s2->localAddr());
			STUN::Transaction* transaction = new STUN::Transaction(//this, 
				_s1, _s2->localAddr(), 2000);
			transaction->StateChanged += delegate(this, &Tests::onSTUNTransactionStateChanged);	
			transaction->request.setType(STUN::Message::Binding);
			transaction->send();
		
			Buffer buf;
			STUN::Message request;
			request.setType(STUN::Message::Binding);
			request.setTransactionID(CryptoProvider::generateRandomKey(16));
			request.write(buf);
			_s1->send(buf.bytes(), buf.length(), _s2->localAddr());

		}
		catch (Poco::Net::NetException& exc) {
			Log("debug") << "TESTS Net: " << exc.displayText() << std::endl;
		}
		catch (Poco::Exception& exc) {
			Log("debug") << "TESTS: " << exc.displayText() << std::endl;
		}

		system("pause");
	}

	UDPSocket* _s1;
	//UDPSocket* _s2;
};
			*/