#include "scy/socketio/client.h"
#include "scy/socketio/transaction.h"
#include "scy/net/sslmanager.h"
#include "scy/application.h"
#include "scy/util.h"


using namespace std;
using namespace scy;
using namespace scy::net;
using namespace scy::util;


/*
// Detect Memory Leaks
#ifdef _DEBUG
#include "MemLeakDetect/MemLeakDetect.h"
#include "MemLeakDetect/MemLeakDetect.cpp"
CMemLeakDetect memLeakDetect;
#endif
*/


namespace scy {
namespace sockio {

	
#define SERVER_HOST "localhost"
#define SERVER_PORT 88 //443
#define USE_SSL     0 //1


// ----------------------------------------------------------------------------
// SocketIO Client Test
//	
class Tests
{
	Application app;
	
public:
	Tests()
	{		
#if USE_SSL
		// Init SSL Context
		SSLContext::Ptr ptrContext = new SSLContext(SSLContext::CLIENT_USE, "", "", "",
			SSLContext::VERIFY_NONE, 9, true, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");	
		SSLManager::instance().initializeClient(ptrContext);

		sockio::SSLClient client(app.loop);
#else
		sockio::TCPClient client(app.loop);
#endif

		client.StateChange += delegate(this, &Tests::onClientStateChange);
		client.connect(SERVER_HOST, SERVER_PORT);
		
		app.run();
		
		// TODO: Benchmarks
		// TODO: Transaction tests

#if USE_SSL
	SSLManager::instance().shutdown();
#endif
		app.finalize();
	}		

	void onClientStateChange(void* sender, sockio::ClientState& state, const sockio::ClientState& oldState) 
	{
		sockio::Client* client = reinterpret_cast<sockio::Client*>(sender);	
		DebugL << "Connection state changed: " << state.toString() << ": " << client->socket().address() << endl;
		
		switch (state.id()) {
		case sockio::ClientState::Connecting:
			break;
		case sockio::ClientState::Connected: 
			DebugL << "Connected on " << client->socket().address() << endl;
			break;
		case sockio::ClientState::Online: 
			// TODO: Send message
			break;
		case sockio::ClientState::Disconnected: 
			break;
		}
	}
};


} } // namespace scy::sockio


int main(int argc, char** argv) 
{	
	Logger::instance().add(new ConsoleChannel("debug", LTrace));
	{
		scy::sockio::Tests run;
	}		
	Logger::shutdown();
	return 0;
}




	//(reactor, srvAddr);

	//Net::SSLSocket socket;
	//Net::SocketBase<Poco::Net::SecureStreamSocket, scy::Net::SSLTCP> socket;
	//Net::TCPStatefulSocket tcpSocket;
	//Net::SSLStatefulSocket sslSocket;


	  
		//,
		//tcpSocket(reactor),
		 //sslSocket(reactor)
		//Application app;		
		//Net::Reactor reactor;
		//net::Address srvAddr("127.0.0.1", 9443);
		
		/*
		//sockio::Socket socket(reactor, srvAddr);
		//socket.StateChange += delegate(this, &Tests::onClientStateChange);
		//socket.connect();
		
		Net::TCPStatefulSocket tcpSocket(reactor);
		tcpSocket.StateChange += delegate(this, &Tests::onClientStateChange);
		tcpSocket.connect(srvAddr);			

		Net::SSLStatefulSocket sslSocket(reactor);
		sslSocket.StateChange += delegate(this, &Tests::onClientStateChange);
		sslSocket.connect(srvAddr);
		*/


	/*
		//Log("debug") << "Connection State Changed: " << state.toString() << endl;
		//Net::StatefulSocketBase* socket = static_cast<Net::StatefulSocketBase*>(sender);	


	//SharedPtr<InvalidCertificateHandler> ptrCert = new ConsoleCertificateHandler(false); // ask the user via console
		"D:\\dev\\projects\\Sourcey\\share\\ssl\\symple.key", 
		"D:\\dev\\projects\\Sourcey\\share\\ssl\\symple.crt", 
		"D:\\dev\\projects\\Sourcey\\share\\ssl\\rootcert.pem", 
	
	// Note: we must create the passphrase handler prior Context 
	SharedPtr<InvalidCertificateHandler> ptrCert = new ConsoleCertificateHandler(false); // ask the user via console
	//Context::Ptr ptrContext = new Context(Context::CLIENT_USE, "", "", "rootcert.pem", Context::VERIFY_RELAXED, 9, false, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
	Context::Ptr ptrContext = new Context(Context::CLIENT_USE, "", "", "", Context::VERIFY_NONE, 9, true, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
	SSLManager::instance().initializeClient(0, ptrCert, ptrContext);
		
		
		
		
		
	
	void runConnectionTest(sockio::Socket& socket) 
	{
		Log("trace") << "[Tests:" << this << "] runConnectionTest" << std::endl;
		
		socket.StateChange += delegate(this, &Tests::onClientStateChange);
		socket.connect();

		//BroadcastPacket += PacketDelegate<Tests, RawPacket>(this, &Tests::onBroadcastPacket, 0);
		//RawPacket packet;
		//BroadcastPacket.send(this, packet);

		//util::pause();
		Log("trace") << "[Tests:" << this << "] runConnectionTest: END" << std::endl;

	}
		
		
		
		
		
		
		*/

    //  key: fs.readFileSync("D:\\dev\\projects\\Sourcey\\share\\ssl\\symple.key")
    //, cert: fs.readFileSync("D:\\dev\\projects\\Sourcey\\share\\ssl\\symple.crt")
	//Context::Ptr ptrContext = new Context(Context::CLIENT_USE, "", "", "", Context::VERIFY_NONE, 9, false, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");		



			/*
			{
				Log("debug") << "Connected to: " << sslSocket.peerAddress() << endl;
				//sslSocket.setBlocking(true);
				SecureStreamSocket& ss1(reinterpret_cast<SecureStreamSocket&>(sslSocket));

				http::Request request("GET", "/");
				stringstream ss;
				request.write(ss);	
				Log("trace") << "Request Headers: " << ss.str() << endl;
				ss1.sendBytes(ss.str().data(), ss.str().length());

				char buffer[256];
				int n = ss1.receiveBytes(buffer, sizeof(buffer));
				assert (n > 0);
				std::string response(buffer, n);
				Log("trace") << "Response: " << response << endl;
			}
				*/

/*
	void onConnected(void* sender) //, RawPacket& packet
	{
		Net::ISocket* socket = reinterpret_cast<Net::ISocket*>(sender);	
		//Net::SocketBase<Poco::Net::SecureStreamSocket, scy::Net::SSLTCP>* socket = reinterpret_cast<Net::SocketBase<Poco::Net::SecureStreamSocket, scy::Net::SSLTCP>*>(sender);	
		//Net::SocketBase<Poco::Net::SecureStreamSocket, scy::Net::SSLTCP>* socket = static_cast<Net::SocketBase<Poco::Net::SecureStreamSocket, scy::Net::SSLTCP>*>(sender);	
		
		//Net::ISocket* socket = static_cast<Net::ISocket*>(sender);	

		Log("debug") << "########################## [Test:" << this << "] onConnected: " << socket << endl;	
		//socket->detach(packetDelegate<EchoServer, RawPacket>(this, &EchoServer::onServerSocketRequestReceived));
		
				
		// SEND THE HTTP REQUEST
		Poco::Net::HTTPRequest request("GET", "/");
		stringstream ss;
		request.write(ss);	
		Log("trace") << "SRequest Headers: " << ss.str() << endl;
		//socket.sendBytes(ss.str().data(), ss.str().length());
		socket->send(ss.str().data(), ss.str().length());

		// echo the data back
		//socket->send(packet);
	}

	void onResponseReceived(void* sender, RawPacket& packet) 
	{
		Net::ISocket* socket = reinterpret_cast<Net::ISocket*>(sender);	

		Log("debug") << "########################## [Test:" << this << "] Packet Received: " << string((const char*)packet.data(), packet.size()) << ": " << socket->peerAddress() << endl;	
		//socket->detach(packetDelegate<EchoServer, RawPacket>(this, &EchoServer::onServerSocketRequestReceived));

		// echo the data back
		//socket->send(packet);
	}
	*/



	/*
	void testSourceySocketReactor()
	{
		
		Log("debug") << "testSocketReactor 1" << endl;
		//SocketReactor reactor;
		//Net::Reactor reactor;
		//SocketAcceptor<EchoServiceHandler> acceptor(ss, reactor);

		//SocketAddress sa("localhost", 9443);
		Log("debug") << "testSocketReactor 2" << endl;
		scy::Net::SourceySocketConnector connector(srvAddr, reactor); //<ClientServiceHandler>
		
		Log("debug") << "testSocketReactor 3" << endl;
		reactor.run();
		Log("debug") << "testSocketReactor 4" << endl;
		//std::string data(ClientServiceHandler::data());
		//assert (data.size() == 1024);
	}


	void testSocketReactor()
	{
		Log("debug") << "testSocketReactor" << endl;
		SocketAddress ssa;
		SecureServerSocket ss(ssa);
		
		Log("debug") << "testSocketReactor 1" << endl;
		SocketReactor reactor;
		//SocketAcceptor<EchoServiceHandler> acceptor(ss, reactor);

		SocketAddress sa("localhost", 9443);
		Log("debug") << "testSocketReactor 2" << endl;
		SocketConnector2 connector(sa, reactor); //<ClientServiceHandler>
		
		Log("debug") << "testSocketReactor 3" << endl;
		reactor.run();
		Log("debug") << "testSocketReactor 4" << endl;
		//std::string data(ClientServiceHandler::data());
		//assert (data.size() == 1024);
	}
	*/





		//
		// Stack Socket
		//
		//Net::SocketBase<Poco::Net::SecureStreamSocket, scy::Net::SSLTCP> ss11(reactor);
		//SecureStreamSocket& ss1(reinterpret_cast<SecureStreamSocket&>(ss11));
		
		//Net::SocketBase<Poco::Net::SecureStreamSocket, scy::Net::SSLTCP> ss1(reactor);
		//ss1.StateChange += delegate(this, &Tests::onClientStateChange);	
		//ss1.connect(srvAddr);	
		//Net::TCPStatefulSocket ss1(reactor);

		/*
		Net::SSLStatefulSocket ss1(reactor);
		//ss1 += packetDelegate(this, &Tests::onResponseReceived);
		ss1.StateChange += delegate(this, &Tests::onClientStateChange);	
		ss1.connect(srvAddr);		
		//SecureStreamSocket ss1(srvAddr);	

		socket.Connected += delegate(this, &Tests::onConnected);	
		socket += packetDelegate(this, &Tests::onResponseReceived);
		socket.connect(srvAddr);	
		*/
		
		/*
		reactor.run();			
		*/
		/*
		http::Request request("GET", "/");
		stringstream ss;
		request.write(ss);	
		Log("trace") << "Request Headers: " << ss.str() << endl;
		ss1.sendBytes(ss.str().data(), ss.str().length());

		char buffer[256];
		int n = ss1.receiveBytes(buffer, sizeof(buffer));
		assert (n > 0);
		std::string response(buffer, n);
		Log("trace") << "Response: " << response << endl;
		*/


		//runConnectionTest(socket);


		/*
		SocketAddress sa("localhost", srvAddr.port());
		SecureStreamSocket ss1(sa);
		//std::string data("hello, world");
		
		http::Request request("GET", "/");
		stringstream ss;
		request.write(ss);	
		Log("trace") << "Request Headers: " << ss.str() << endl;
		ss1.sendBytes(ss.str().data(), ss.str().length());

		//ss1.sendBytes(data.data(), (int) data.size());
		char buffer[256];
		int n = ss1.receiveBytes(buffer, sizeof(buffer));
		assert (n > 0);
		std::string response(buffer, n);
		Log("trace") << "Response: " << response << endl;

		//assert (std::string(buffer, n) == data);
		//assert (srv.currentConnections() == 1);
		//assert (srv.currentThreads() == 1);
		//assert (srv.queuedConnections() == 0);
		//assert (srv.totalConnections() == 1);
		//Thread::sleep(300);
		//assert (srv.currentConnections() == 0);

		//util::pause();

		ss1.close();
		
		//testSocketReactor();
		testSourceySocketReactor();
		*/










/*

namespace Poco {
namespace Net {

	
//template <class ServiceHandler>
class SourceySocketConnector
	/// This class implements the Connector part of the
	/// Acceptor-Connector design pattern.
	///
	/// The Acceptor-Connector pattern has been described in the book
	/// "Pattern Languages of Program Design 3", edited by Robert Martin,
	/// Frank Buschmann and Dirk Riehle (Addison Wesley, 1997).
	///
	/// The Acceptor-Connector design pattern decouples connection
	/// establishment and service initialization in a distributed system
	/// from the processing performed once a service is initialized.
	/// This decoupling is achieved with three components: Acceptors, 
	/// Connectors and Service Handlers.
	/// The Connector actively establishes a connection with a remote
	/// server socket (usually managed by an Acceptor) and initializes
	/// a Service Handler to manage the connection.
	///
	/// The SourceySocketConnector sets up a StreamSocket, initiates a non-blocking
	/// connect operation and registers itself for ReadableNotification, WritableNotification
	/// and ErrorNotification. ReadableNotification or WritableNotification denote the successful 
	/// establishment of the connection.
	///
	/// When the StreamSocket becomes readable or writeable, the SourceySocketConnector 
	/// creates a ServiceHandler to service the connection and unregisters
	/// itself.
	///
	/// In case of an error (ErrorNotification), the SourceySocketConnector unregisters itself
	/// and calls the onError() method, which can be overridden by subclasses
	/// to perform custom error handling.
	///
	/// The ServiceHandler class must provide a constructor that
	/// takes a StreamSocket and a scy::Net::Reactor as arguments,
	/// e.g.:
	///     MyServiceHandler(const StreamSocket& socket, ServiceReactor& reactor)
	///
	/// When the ServiceHandler is done, it must destroy itself.
	///
	/// Subclasses can override the createServiceHandler() factory method
	/// if special steps are necessary to create a ServiceHandler object.
{
public:		
	explicit SourceySocketConnector(SocketAddress& address) :
		_pReactor(0)
		/// Creates a SourceySocketConnector, using the given Socket.
	{
		_socket.connect(address);
		//_socket.connectNB(address);
	}

	SourceySocketConnector(SocketAddress& address, scy::Net::Reactor& reactor):
		_pReactor(0)
		/// Creates an acceptor, using the given ServerSocket.
		/// The SourceySocketConnector registers itself with the given scy::Net::Reactor.
	{
		_socket.connect(address);
		//_socket.connectNB(address);
		registerConnector(reactor);
	}

	virtual ~SourceySocketConnector()
		/// Destroys the SourceySocketConnector.
	{
		unregisterConnector();
	}
	
	virtual void registerConnector(scy::Net::Reactor& reactor)
		/// Registers the SourceySocketConnector with a scy::Net::Reactor.
		///
		/// A subclass can override this and, for example, also register
		/// an event handler for a timeout event.
		///
		/// The overriding method must call the baseclass implementation first.
	{
		_pReactor = &reactor;
		
		_pReactor->attach(_socket, reactorDelegate(this, &SourceySocketConnector::onReadable, SocketReadable));
		_pReactor->attach(_socket, reactorDelegate(this, &SourceySocketConnector::onWritable, SocketWritable));
		_pReactor->attach(_socket, reactorDelegate(this, &SourceySocketConnector::onError, SocketError));

		//_pReactor->addEventHandler(_socket, Poco::Observer<SourceySocketConnector, ReadableNotification>(*this, &SourceySocketConnector::onReadable));
		//_pReactor->addEventHandler(_socket, Poco::Observer<SourceySocketConnector, WritableNotification>(*this, &SourceySocketConnector::));
		//_pReactor->addEventHandler(_socket, Poco::Observer<SourceySocketConnector, ErrorNotification>(*this, &SourceySocketConnector::onError));
	}
	
	virtual void unregisterConnector()
		/// Unregisters the SourceySocketConnector.
		///
		/// A subclass can override this and, for example, also unregister
		/// its event handler for a timeout event.
		///
		/// The overriding method must call the baseclass implementation first.
	{
		if (_pReactor)
		{
			_pReactor->detach(_socket, reactorDelegate(this, &SourceySocketConnector::onReadable, SocketReadable));
			_pReactor->detach(_socket, reactorDelegate(this, &SourceySocketConnector::onWritable, SocketWritable));
			_pReactor->detach(_socket, reactorDelegate(this, &SourceySocketConnector::onError, SocketError));
			//_pReactor->removeEventHandler(_socket, Poco::Observer<SourceySocketConnector, ReadableNotification>(*this, &SourceySocketConnector::onReadable));
			//_pReactor->removeEventHandler(_socket, Poco::Observer<SourceySocketConnector, WritableNotification>(*this, &SourceySocketConnector::onWritable));
			//_pReactor->removeEventHandler(_socket, Poco::Observer<SourceySocketConnector, ErrorNotification>(*this, &SourceySocketConnector::onError));
		}
	}
	
	void onReadable() //ReadableNotification* pNotification)
	{
		Log("trace") << "######################### POCO onReadable: " << _socket.impl()->referenceCount() << endl;
				
		char buffer[256];
		int n = _socket.receiveBytes(buffer, sizeof(buffer));			
		std::string response(buffer, n);
		Log("trace") << "############################ RECV: " << response << endl;


		///pNotification->release();
		int err = _socket.impl()->socketError(); 
		if (err)
		{
			onError(err);
			unregisterConnector();
		}
		else
		{
			onConnect();
		}
	}
	
	void onWritable() //WritableNotification* pNotification)
	{
		Log("trace") << "######################### POCO onWritable: " << _socket.impl()->referenceCount() << endl;
		//pNotification->release();
		Log("trace") << "######################### POCO onWritable 1: " << _socket.impl()->referenceCount() << endl;
		
		//onConnect();
		//_socket.setBlocking(true);

		// SEND THE HTTP REQUEST
		http::Request request("GET", "/");
		stringstream ss;
		request.write(ss);	
		Log("trace") << "SRequest Headers: " << ss.str() << endl;
		_socket.sendBytes(ss.str().data(), ss.str().length());
		
		Log("trace") << "######################### POCO onWritable 2" << endl;
		
		char buffer[256];
		int n = _socket.receiveBytes(buffer, sizeof(buffer));			
		std::string response(buffer, n);
		Log("trace") << "############################ RECV: " << response << endl;
	}
	
	void onConnect()
	{
		//_socket.setBlocking(true);
		//createServiceHandler();
		//unregisterConnector();
		
		_pReactor->detach(_socket, reactorDelegate(this, &SourceySocketConnector::onWritable, SocketWritable));

		//onReadable();
	}
	
	void onError() //ErrorNotification* pNotification)
	{
		Log("trace") << "######################### POCO onError" << endl;

		//pNotification->release();
		onError(_socket.impl()->socketError());
		unregisterConnector();
	}
	
protected:
	//virtual ServiceHandler* createServiceHandler()
		/// Create and initialize a new ServiceHandler instance.
		///
		/// Subclasses can override this method.
	//{
	//	return new ServiceHandler(_socket, *_pReactor);
	//}

	virtual void onError(int errorCode)
		/// Called when the socket cannot be connected.
		///
		/// Subclasses can override this method.
	{
	}
	
	scy::Net::Reactor* reactor()
		/// Returns a pointer to the scy::Net::Reactor where
		/// this SourceySocketConnector is registered.
		///
		/// The pointer may be null.
	{
		return _pReactor;
	}
	
	Socket& socket()
		/// Returns a reference to the SourceySocketConnector's socket.
	{
		return _socket;
	}

private:
	SourceySocketConnector();
	SourceySocketConnector(const SourceySocketConnector&);
	SourceySocketConnector& operator = (const SourceySocketConnector&);
	
	SecureStreamSocket   _socket;
	scy::Net::Reactor* _pReactor;
};



//template <class ServiceHandler>
class SocketConnector2
	/// This class implements the Connector part of the
	/// Acceptor-Connector design pattern.
	///
	/// The Acceptor-Connector pattern has been described in the book
	/// "Pattern Languages of Program Design 3", edited by Robert Martin,
	/// Frank Buschmann and Dirk Riehle (Addison Wesley, 1997).
	///
	/// The Acceptor-Connector design pattern decouples connection
	/// establishment and service initialization in a distributed system
	/// from the processing performed once a service is initialized.
	/// This decoupling is achieved with three components: Acceptors, 
	/// Connectors and Service Handlers.
	/// The Connector actively establishes a connection with a remote
	/// server socket (usually managed by an Acceptor) and initializes
	/// a Service Handler to manage the connection.
	///
	/// The SocketConnector2 sets up a StreamSocket, initiates a non-blocking
	/// connect operation and registers itself for ReadableNotification, WritableNotification
	/// and ErrorNotification. ReadableNotification or WritableNotification denote the successful 
	/// establishment of the connection.
	///
	/// When the StreamSocket becomes readable or writeable, the SocketConnector2 
	/// creates a ServiceHandler to service the connection and unregisters
	/// itself.
	///
	/// In case of an error (ErrorNotification), the SocketConnector2 unregisters itself
	/// and calls the onError() method, which can be overridden by subclasses
	/// to perform custom error handling.
	///
	/// The ServiceHandler class must provide a constructor that
	/// takes a StreamSocket and a SocketReactor as arguments,
	/// e.g.:
	///     MyServiceHandler(const StreamSocket& socket, ServiceReactor& reactor)
	///
	/// When the ServiceHandler is done, it must destroy itself.
	///
	/// Subclasses can override the createServiceHandler() factory method
	/// if special steps are necessary to create a ServiceHandler object.
{
public:		
	explicit SocketConnector2(SocketAddress& address):
		_pReactor(0)
		/// Creates a SocketConnector2, using the given Socket.
	{
		_socket.connectNB(address);
	}

	SocketConnector2(SocketAddress& address, SocketReactor& reactor):
		_pReactor(0)
		/// Creates an acceptor, using the given ServerSocket.
		/// The SocketConnector2 registers itself with the given SocketReactor.
	{
		_socket.connectNB(address);
		registerConnector(reactor);
	}

	virtual ~SocketConnector2()
		/// Destroys the SocketConnector2.
	{
		unregisterConnector();
	}
	
	virtual void registerConnector(SocketReactor& reactor)
		/// Registers the SocketConnector2 with a SocketReactor.
		///
		/// A subclass can override this and, for example, also register
		/// an event handler for a timeout event.
		///
		/// The overriding method must call the baseclass implementation first.
	{
		_pReactor = &reactor;
		_pReactor->addEventHandler(_socket, Poco::Observer<SocketConnector2, ReadableNotification>(*this, &SocketConnector2::onReadable));
		_pReactor->addEventHandler(_socket, Poco::Observer<SocketConnector2, WritableNotification>(*this, &SocketConnector2::onWritable));
		_pReactor->addEventHandler(_socket, Poco::Observer<SocketConnector2, ErrorNotification>(*this, &SocketConnector2::onError));
	}
	
	virtual void unregisterConnector()
		/// Unregisters the SocketConnector2.
		///
		/// A subclass can override this and, for example, also unregister
		/// its event handler for a timeout event.
		///
		/// The overriding method must call the baseclass implementation first.
	{
		if (_pReactor)
		{
			_pReactor->removeEventHandler(_socket, Poco::Observer<SocketConnector2, ReadableNotification>(*this, &SocketConnector2::onReadable));
			_pReactor->removeEventHandler(_socket, Poco::Observer<SocketConnector2, WritableNotification>(*this, &SocketConnector2::onWritable));
			_pReactor->removeEventHandler(_socket, Poco::Observer<SocketConnector2, ErrorNotification>(*this, &SocketConnector2::onError));
		}
	}
	
	void onReadable(ReadableNotification* pNotification)
	{
		Log("trace") << "######################### POCO onReadable" << endl;

		pNotification->release();
				
		char buffer[32];
		int n = _socket.receiveBytes(buffer, sizeof(buffer));			
		std::string response(buffer, n);
		Log("trace") << "############################ RECV: " << response << endl;

		int err = _socket.impl()->socketError(); 
		if (err)
		{
			onError(err);
			unregisterConnector();
		}
		else
		{
			onConnect();
		}
	}
	
	void onWritable(WritableNotification* pNotification)
	{
		Log("trace") << "######################### POCO onWritable: " << _socket.impl()->referenceCount() << endl;

		pNotification->release();
		onConnect();

		// SEND THE HTTP REQUEST
		http::Request request("GET", "/");
		stringstream ss;
		request.write(ss);	
		Log("trace") << "SRequest Headers: " << ss.str() << endl;
		_socket.sendBytes(ss.str().data(), ss.str().length());
	}
	
	void onConnect()
	{
		_socket.setBlocking(true);
		//createServiceHandler();
		//unregisterConnector();
		
		_pReactor->removeEventHandler(_socket, Poco::Observer<SocketConnector2, WritableNotification>(*this, &SocketConnector2::onWritable));
	}
	
	void onError(ErrorNotification* pNotification)
	{
		Log("trace") << "######################### POCO onError" << endl;

		pNotification->release();
		onError(_socket.impl()->socketError());
		unregisterConnector();
	}
	
protected:
	//virtual ServiceHandler* createServiceHandler()
		/// Create and initialize a new ServiceHandler instance.
		///
		/// Subclasses can override this method.
	//{
	//	return new ServiceHandler(_socket, *_pReactor);
	//}

	virtual void onError(int errorCode)
		/// Called when the socket cannot be connected.
		///
		/// Subclasses can override this method.
	{
	}
	
	SocketReactor* reactor()
		/// Returns a pointer to the SocketReactor where
		/// this SocketConnector2 is registered.
		///
		/// The pointer may be null.
	{
		return _pReactor;
	}
	
	Socket& socket()
		/// Returns a reference to the SocketConnector2's socket.
	{
		return _socket;
	}

private:
	SocketConnector2();
	SocketConnector2(const SocketConnector2&);
	SocketConnector2& operator = (const SocketConnector2&);
	
	SecureStreamSocket   _socket;
	SocketReactor* _pReactor;
};


} } // namespace Poco::Net



namespace
{	
}
*/