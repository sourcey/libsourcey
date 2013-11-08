#include "MediaServer.h"

#include "Sourcey/Util/StreamManager.h"
#include "Sourcey/TURN/client/TCPClient.h"


namespace scy { 


//
// Relayed Streaming Client Allocation
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
		try	
		{		
			// Initiate the TRUN client allocation
			client.addPermission(peerIP);	
			client.addPermission("127.0.0.1"); // for proxy
			client.addPermission("192.168.1.1"); // for proxy
			client.initiate();
		} 
		catch (std::exception& exc) {
			errorL("RelayedStreamingAllocation", this) << "Error: " << exc.what() << std::endl;
			assert(0);
		}	
	}

	void terminate() 
	{
		debugL("RelayedStreamingAllocation", this) << "Terminating" << std::endl;	
		client.shutdown();

		// Free all managed packet streams
		streams.closeAll();
	}

protected:
	void onClientStateChange(turn::Client& client, turn::ClientState& state, const turn::ClientState&) 
	{
		debugL("RelayedStreamingAllocation", this) << "Relay state changed: " << state.toString() << std::endl;

		switch(state.id()) {
		case turn::ClientState::None:				
			break;
		case turn::ClientState::Allocating:				
			break;
		case turn::ClientState::Authorizing:
			break;
		case turn::ClientState::Success:
			AllocationCreated.emit(this, this->client);
			break;
		case turn::ClientState::Failed:
			//assert(0 && "Allocation failed");
			warnL("RelayedStreamingAllocation", this) << "Relay connection lost" << std::endl;
			break;
		//case turn::ClientState::Terminated:	
		//	break;
		}
	}
	
	void onRelayConnectionCreated(turn::TCPClient& client, const net::TCPSocket& socket, const net::Address& peerAddr) //UInt32 connectionID, 
	{
		debugL("RelayedStreamingAllocation", this) << "Connection created: " << peerAddr << std::endl;
		
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
			auto stream = new PacketStream(peerAddr.toString());
			
			// Setup the packet stream ensuring the audio capture isn't
			// destroyed with the stream, as it may be reused while the
			// allocation is active.
			MediaServer::setupPacketStream(*stream, options, false, true);
		
			// Feed the packet stream directly into the connection		
			stream->emitter += packetDelegate(reinterpret_cast<net::Socket*>(
				const_cast<net::TCPSocket*>(&socket)), &net::Socket::send);

			// Start the stream
			stream->start();	

			this->streams.addStream(stream);
		} 
		catch (std::exception& exc) {
			errorL("RelayedStreamingAllocation", this) << "Stream error: " << exc.what() << std::endl;
			assert(0);
		}
	}
		
	void onRelayConnectionClosed(turn::TCPClient& client, const net::TCPSocket& socket, const net::Address& peerAddress)
	{
		debugL("RelayedStreamingAllocation", this) << " Connection closed: " << peerAddress << std::endl;

		try	
		{	
			// Destroy the media stream for the closed connection (if any).
			//this->streams.free(peerAddress.toString());
			PacketStream* stream = streams.remove(peerAddress.toString());
			if (stream) {	
				stream->emitter += packetDelegate(reinterpret_cast<net::Socket*>(
					const_cast<net::TCPSocket*>(&socket)), &net::Socket::send);
				delete stream;
				//stream->destroy();
			}
		} 
		catch (std::exception& exc) {
			errorL("RelayedStreamingAllocation", this) << "Stream error: " << exc.what() << std::endl;
			assert(0);
		}
	}

	void onRelayDataReceived(turn::Client& client, const char* data, int size, const net::Address& peerAddr)
	{
		debugL("RelayedStreamingAllocation", this) << "Received data from peer: " << std::string(data, size) <<  ": " << peerAddr << std::endl;
	
		// If the remove peer is a web browser then the HTTP request sent 
		// to the relayed address will be the first thing we see here...
	}

	void onAllocationPermissionsCreated(turn::Client& client, const turn::PermissionList& permissions)
	{
		debugL("RelayedStreamingAllocation", this) << "Permissions created" << std::endl;
	}
};


//
// Relayed Streaming Connection Handler
//


class RelayedStreamingResponder: public http::ServerResponder
{
public:
	RelayedStreamingResponder(http::ServerConnection& connection, const StreamingOptions& options) : 		
		http::ServerResponder(connection), options(options), allocation(nullptr)
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
		co.serverAddr = net::Address("124.150.77.174", 3478); // "127.0.0.1"
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
		allocation->AllocationCreated -= delegate(this, &RelayedStreamingResponder::onAllocationCreated);
		std::string address(allocation->client.relayedAddress().toString());

		debugL("RelayedStreamingResponder", this) << "Allocation Created: " << address << std::endl;
					
		// Send the relay address response to the initiator		
		connection().response().set("Access-Control-Allow-Origin", "*");
		connection().socket().send(address.c_str(), address.length());
		connection().close();
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
	PacketStream    stream;
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
		stream.emitter += packetDelegate(this, &RelayedStreamingAllocation::onMediaEncoded);
		stream.start();		

		debugL("RelayedStreamingAllocation", this) << "Play Media: OK" << std::endl;
	}

	void stopMedia()
	{
		debugL("RelayedStreamingAllocation", this) << "Stop Media" << std::endl;
		stream.emitter -= packetDelegate(this, &RelayedStreamingAllocation::onMediaEncoded);
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
		catch (std::exception&/Exception&/ exc) {
			errorL("RelayedStreamingAllocation", this) << "^^^^^^^^^^^^^^^^^^^^^^^^ Send error: " << exc.what()/message()/ << std::endl;
			
			// TODO: Calling stream.stop() inside stream callback causing deadlock
			terminate();
		}
		debugL("RelayedStreamingAllocation", this) << "!$$$$$$$$$$$$$$ Sending Packet: OK: " << packet.size() << std::endl;
	}
	*/

	
		/*
		//stream.emitter += packetDelegate(this, &RelayedStreamingAllocation::onMediaEncoded);
		// Start the stream
		stream.emitter += packetDelegate(this, &RelayedStreamingAllocation::onMediaEncoded);
		stream.start();		
		stream.emitter -= packetDelegate(this, &RelayedStreamingAllocation::onMediaEncoded);
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