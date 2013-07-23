/*
#include "Sourcey/Logger.h"
#include "Sourcey/Application.h"
#include "Sourcey/PacketStream.h"
#include "Sourcey/SyncPacketStream.h"
#include "Sourcey/Crypto.h"
#include "Sourcey/Util.h"

#include "Sourcey/HTTP/Server.h"
#include "Sourcey/HTTP/WebSocket.h"

#include "Sourcey/Media/MediaFactory.h"
#include "Sourcey/Media/AVEncoder.h"
#include "Sourcey/Media/AVInputReader.h"
#include "Sourcey/Media/FLVMetadataInjector.h"
#include "Sourcey/Media/FormatRegistry.h"

#include "Sourcey/HTTP/Util.h"
#include "Sourcey/HTTP/Packetizers.h"
#include "Sourcey/Util/Base64PacketEncoder.h"

#include "Sourcey/Containers.h"

#include <string>
#include <vector>
#include <assert.h>
#include <conio.h>

// Detect Win32 memory Leaks - leak free!
#ifdef _DEBUG
#include "MemLeakDetect/MemLeakDetect.h"
#include "MemLeakDetect/MemLeakDetect.cpp"
CMemLeakDetect memLeakDetect;
#endif
	

using namespace std;
using namespace scy;
using namespace scy::av;
	//Application app; //runner, 

*/

#include "MediaServer.h"

#include "Sourcey/Util/StreamManager.h"
#include "Sourcey/TURN/client/TCPClient.h"


namespace scy { 


// ----------------------------------------------------------------------------
// Releayed Streaming Client Allocation
//
class RelayedStreamingAllocation: public turn::TCPClientObserver
{
public:
	turn::TCPClient client;
	std::string	peerIP;		
	StreamManager streams;
	StreamingOptions options;	
	int	frameNumber;
	bool connected;

	Signal<turn::Client&> AllocationCreated;
	Signal2<turn::Client&, const net::Address&> ConnectionCreated;

	RelayedStreamingAllocation(const StreamingOptions& options, const turn::Client::Options& clientOptions, const std::string& peerIP) : 
		client(*this, clientOptions), options(options), peerIP(peerIP), frameNumber(0), connected(false)
	{
	}

	virtual ~RelayedStreamingAllocation() 
	{ 
		terminate();
		assert(streams.empty());

		// NOTE: We are responsible for deleting the audioCapture.
		if (options.audioCapture)
			delete options.audioCapture;
	}

	void initiate() 
	{
		debugL("RelayedStreamingAllocation", this) << "Initiating" << std::endl;		
		//terminate();
		try	
		{		
			// Initiate the TRUN client allocation
			debugL("RelayedStreamingAllocation", this) << "Adding Persission" << std::endl;		
			client.addPermission(peerIP);	
			debugL("RelayedStreamingAllocation", this) << "Initiating Client" << std::endl;		
			client.initiate();
		} 
		catch (Exception& exc) {
			errorL("RelayedStreamingAllocation", this) << "Error: " << exc.message() << std::endl;
			assert(0);
		}
		debugL("RelayedStreamingAllocation", this) << "Initiating: OK" << std::endl;	
	}

	void terminate() 
	{
		debugL("RelayedStreamingAllocation", this) << "Terminating" << std::endl;	
		client.terminate();

		// Free all managed packet streams
		streams.closeAll();
	}

protected:
	void onClientStateChange(turn::Client& client, turn::ClientState& state, const turn::ClientState&) 
	{
		debugL("RelayedStreamingAllocation", this) << "Relay State Changed: " << state.toString() << std::endl;

		switch(state.id()) {
		case turn::ClientState::Waiting:				
			break;
		case turn::ClientState::Allocating:				
			break;
		case turn::ClientState::Authorizing:
			break;
		case turn::ClientState::Success:
			AllocationCreated.emit(this, this->client);
			break;
		case turn::ClientState::Failed:
			assert(0 && "Allocation failed");
			break;
		case turn::ClientState::Terminated:	
			break;
		}
	}
	
	void onRelayConnectionCreated(turn::TCPClient& client, const net::TCPSocket& socket, const net::Address& peerAddr) //UInt32 connectionID, 
	{
		debugL("RelayedStreamingAllocation", this) << "################# Connection created: " << peerAddr << std::endl;
		
		// Just allow one stream for now
		if (this->streams.size() == 1) {
			debugL("RelayedStreamingAllocation", this) << "Rejecting connection" << std::endl;
			return;
		}

		try	
		{	
			// Notify the outside application
			ConnectionCreated.emit(this, client, peerAddr);

			// Create an output media stream for the new connection
			SyncPacketStream* stream = new SyncPacketStream(peerAddr.toString());
			
			// Setup the packet stream ensuring the audio capture isn't
			// destroyed with the stream, as it may be reused while the
			// allocation is active.
			MediaServer::setupPacketStream(*stream, options, false, true);
		
			// Feed the packet stream directly into the connection		
			stream->attach(packetDelegate(reinterpret_cast<net::Socket*>(
				const_cast<net::TCPSocket*>(&socket)), &net::Socket::send));

			// Start the stream
			stream->start();	

			this->streams.addStream(stream);
		} 
		catch (Exception& exc) {
			errorL("RelayedStreamingAllocation", this) << "Stream Error: " << exc.message() << std::endl;
			assert(0);
		}

		debugL("RelayedStreamingAllocation", this) << "Connection Created: OK: " << peerAddr << std::endl;
	}
		
	void onRelayConnectionClosed(turn::TCPClient& client, const net::TCPSocket& socket, const net::Address& peerAddress)
	{
		debugL("RelayedStreamingAllocation", this) << "!!!!!!!!!!!!!!!!!!!!!!!! Connection Closed: " << peerAddress << std::endl;

		try	
		{	
			// Destroy the media stream for the closed connection (if any).
			//this->streams.free(peerAddress.toString());
			PacketStream* stream = streams.remove(peerAddress.toString());
			if (stream) {	
				stream->detach(packetDelegate(reinterpret_cast<net::Socket*>(
					const_cast<net::TCPSocket*>(&socket)), &net::Socket::send));
				//delete stream;
				stream->destroy();
			}
		} 
		catch (Exception& exc) {
			errorL("RelayedStreamingAllocation", this) << "Stream Error: " << exc.message() << std::endl;
			assert(0);
		}
	}

	void onRelayDataReceived(turn::Client& client, const char* data, int size, const net::Address& peerAddr)
	{
		debugL("RelayedStreamingAllocation", this) << "Received data from peer: " << std::string(data, size) <<  ": " << peerAddr << std::endl;
	
		// If the remore peer is a web browser then the HTTP request sent 
		// to the relayed address will be the first thing we see here...
	}

	void onAllocationPermissionsCreated(turn::Client& client, const turn::PermissionList& permissions)
	{
		debugL("RelayedStreamingAllocation", this) << "Permissions Created" << std::endl;
	}
};


// ----------------------------------------------------------------------------
// Releayed Streaming Connection Handler
//
class RelayedStreamingResponder: public http::ServerResponder
{
public:
	RelayedStreamingResponder(http::ServerConnection& connection, const StreamingOptions& options) : 		
		http::ServerResponder(connection), options(options), allocation(NULL)//, allocation(allocation)//, response(response)
	{		
	}

	virtual ~RelayedStreamingResponder() 
	{
	}
		
	void onRequest(http::Request& request, http::Response& response)
	{
		debugL("RelayedStreamingResponder", this) << "Running: " 
			<< "\n\tOutput Format: " << options.oformat.name
			<< "\n\tOutput Encoding: " << options.encoding
			<< "\n\tOutput Packetizer: " << options.packetizer
			<< std::endl;

		turn::Client::Options co;
		co.serverAddr = net::Address("127.0.0.1", 3478); // "173.230.150.125"
		co.lifetime  = 120 * 1000;	// 2 minutes
		co.timeout = 10 * 1000;
		co.timerInterval = 3 * 1000;
		co.username = Anionu_API_USERNAME;
		co.password = Anionu_API_PASSWORD;

		allocation = new RelayedStreamingAllocation(options, co, connection().socket().peerAddress().host());
		allocation->AllocationCreated += delegate(this, &RelayedStreamingResponder::onAllocationCreated);
		allocation->initiate();
	}
	
	void onAllocationCreated(void* sender, turn::Client& client)
	{
		debugL("RelayedStreamingResponder", this) << "Allocation Created" << std::endl;

		allocation->AllocationCreated -= delegate(this, &RelayedStreamingResponder::onAllocationCreated);
					
		// Send the relay address response to the initiator		
		connection().response().set("Access-Control-Allow-Origin", "*");
		connection().response().body << allocation->client.relayedAddress();
		connection().send();
	}
	
	RelayedStreamingAllocation* allocation;
	StreamingOptions options;
	av::FPSCounter fpsCounter;
};


} // namespace scy



		//debugL("RelayedStreamingResponder", this) << "Allocation Created 1" << std::endl;		
		//stopSignal.set();
		//debugL("RelayedStreamingResponder", this) << "Allocation Created 2" << std::endl;

		//assert(this->response);
		//this->response->send() 
		//	<< allocation->client.relayedAddress()
		//	<< std::flush;
		
		//this->response = &response;
				
		//std::string peerIP("127.0.0.1");
		//debugL("RelayedStreamingResponder", this) << "Waiting" << std::endl;	
		//stopSignal.wait();
		//debugL("RelayedStreamingResponder", this) << "Stopped" << std::endl;	

		// TODO: The allocation will outlive this handler instance
		// Manage allocation via MediaService?

		//debugL("RelayedStreamingResponder", this) << "Exiting" << std::endl;


/*
	//http::ServerResponder* response;
	//Poco::Event stopSignal;
class RelayedMediaStream 
{
public:
	SyncPacketStream    stream;
};
*/

/* 
*/


	/*	
		//client.terminate(); 
		//stopMedia();
	//PacketStream    stream;

	void playMedia() 
	{
		debugL("RelayedStreamingAllocation", this) << "Play Media" << std::endl;

		// Create the packet stream
		//debugL("RelayedStreamingAllocation", this) << "Setup Packet Stream" << std::endl;	
		//setupPacketStream(stream, options);
		//debugL("RelayedStreamingAllocation", this) << "Setup Packet Stream: OK" << std::endl;	

		// Start the stream
		stream += packetDelegate(this, &RelayedStreamingAllocation::onMediaEncoded);
		stream.start();		

		debugL("RelayedStreamingAllocation", this) << "Play Media: OK" << std::endl;
	}

	void stopMedia()
	{
		debugL("RelayedStreamingAllocation", this) << "Stop Media" << std::endl;
		stream -= packetDelegate(this, &RelayedStreamingAllocation::onMediaEncoded);
		stream.close();	
		debugL("RelayedStreamingAllocation", this) << "Stop Media: OK" << std::endl;
	}
	*/

	/*
	void onMediaEncoded(void* sender, RawPacket& packet)
	{
		debugL("RelayedStreamingAllocation", this) << "$$$$$$$$$$$$$$ Sending Packet: " << packet.size() << string((const char*)packet.data(), 100)  << std::endl;
		
		//assert(currentPeerAddr.valid());
		try {
			// Send the media to our peer
			//client.sendData((const char*)packet.data(), packet.size(), currentPeerAddr);
			//ostringstream oss;
			//oss << "Packet: " << frameNumber++ << std::endl;			
			//client.sendData(oss.str().data(), oss.str().length(), currentPeerAddr);
		}
		catch (Exception& exc) {
			errorL("RelayedStreamingAllocation", this) << "^^^^^^^^^^^^^^^^^^^^^^^^ Send Error: " << exc.message() << std::endl;
			
			// TODO: Calling stream.stop() inside stream callback causing deadlock
			terminate();
		}
		debugL("RelayedStreamingAllocation", this) << "!$$$$$$$$$$$$$$ Sending Packet: OK: " << packet.size() << std::endl;
	}
	*/

	
		/*
		//stream += packetDelegate(this, &RelayedStreamingAllocation::onMediaEncoded);
		// Start the stream
		stream += packetDelegate(this, &RelayedStreamingAllocation::onMediaEncoded);
		stream.start();		
		stream -= packetDelegate(this, &RelayedStreamingAllocation::onMediaEncoded);
		stream.close();	

		//currentPeerAddr = peerAddr; // Current peer
		//connected = true;
		
		// TODO: Restart packet stream without deleting managed sources
		//stopMedia();
		//playMedia();
		*/
	/*
	void onRelayConnectionState(turn::TCPClient& client, net::SocketBase*, 
		Net::SocketState& state, const Net::SocketState& oldState) 
	{
		debugL("RelayedStreamingAllocation", this) << "@@@@@@@@@@@@@@@@@@@@@ Connection State: " << state.toString() << std::endl;
	}

		//assert(currentPeerAddr.valid());
		//assert(connected);
		//playMedia();
	*/