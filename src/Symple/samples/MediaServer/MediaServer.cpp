#include "Sourcey/Logger.h"
#include "Sourcey/Runner.h"
#include "Sourcey/PacketStream.h"
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
#include "Sourcey/Util/StreamManager.h"

#include "Sourcey/TURN/client/TCPClient.h"
#include "Poco/Net/NameValueCollection.h"

#include <string>
#include <vector>
#include <assert.h>
#include <conio.h>


/*
// Detect Win32 memory Leaks - leak free!
#ifdef _DEBUG
#include "MemLeakDetect/MemLeakDetect.h"
#include "MemLeakDetect/MemLeakDetect.cpp"
CMemLeakDetect memLeakDetect;
#endif
*/


using namespace std;
using namespace scy;
using namespace scy::av;


#define SERVER_PORT 328


namespace scy { 

	
class MediaServer;


struct StreamingOptions: public av::RecordingOptions
{	
	std::string packetizer;		// HTTP response packetizer [chunked, multipart]
	std::string encoding;		// The packet content encoding method [Base64, ...]
		
	MediaServer* server;		// Media server instance
	VideoCapture* videoCapture; // Video capture instance
	AudioCapture* audioCapture; // Audio capture instance

	StreamingOptions(MediaServer* server = NULL, VideoCapture* videoCapture = NULL, AudioCapture* audioCapture = NULL) : 
		server(server), videoCapture(videoCapture), audioCapture(audioCapture) {}
};


void setupPacketStream(PacketStream& stream, const StreamingOptions& options, bool freeAudio = true) 
{	
	debugL() << "Setup Packet Stream" << endl;

	// Attach media captures
	if (options.oformat.video.enabled) {
		assert(options.videoCapture);
		stream.attach(options.videoCapture, 0, false);
	}
	if (options.oformat.audio.enabled) {
		assert(options.audioCapture);
		stream.attach(options.audioCapture, 0, freeAudio);	
	}

	// Create and attach media encoder				
	AVEncoder* encoder = new AVEncoder(options);
	encoder->initialize();
	stream.attach(encoder, 5, true);		
				
	// Add format specific packetizers
	if (options.oformat.name == "MJPEG") {	

		// Base64 encode the MJPEG stream for old browsers
		if (options.encoding.empty() ||
			options.encoding == "none" ||
			options.encoding == "None") {
			// no default encoding
		}
		else if (options.encoding == "Base64") {
			Base64PacketEncoder* base64 = new Base64PacketEncoder();
			stream.attach(base64, 9, true);
		}
		else
			throw Exception("Unsupported encoding method: " + options.encoding);
				
		// Attach an HTTP output packetizer
		IPacketizer* packetizer = NULL;
		if (options.packetizer.empty() ||
			options.packetizer == "none" ||
			options.packetizer == "None")
			packetizer = new http::StreamingPacketizer("image/jpeg");	

		else if (options.packetizer == "chunked")
			packetizer = new http::ChunkedPacketizer("image/jpeg");

		else if (options.packetizer == "multipart")
			packetizer = new http::MultipartPacketizer("image/jpeg", false, options.encoding == "Base64");				

		else throw Exception("Unsupported packetizer method: " + options.packetizer);

		if (packetizer)					
			stream.attach(packetizer, 10, true);	
	}	
	else if (options.oformat.name == "FLV") {

		// Allow mid-stream flash client connection
		FLVMetadataInjector* injector = new FLVMetadataInjector(options.oformat);
		stream.attach(injector, 10);
	}
	
	debugL() << "Setup Packet Stream: OK" << endl;
}


// ----------------------------------------------------------------------------
//
class StreamingRequestHandler: public http::ServerResponder
{
public:
	StreamingRequestHandler(http::ServerConnection& connection, const StreamingOptions& options) :
		http::ServerResponder(connection), options(options)
	{		
	}

	~StreamingRequestHandler() 
	{
	}
		
	void onRequest(http::Request& request, http::Response& response) 
	{
		debugL("HTTPStreamingConnectionHandler") << "Running: " 
			<< "\n\tOutput Format: " << options.oformat.name
			<< "\n\tOutput Encoding: " << options.encoding
			<< "\n\tOutput Packetizer: " << options.packetizer
			<< endl;

		// Create the packet stream
		setupPacketStream(stream, options);

		// Start the stream
		stream += packetDelegate(this, &StreamingRequestHandler::onVideoEncoded);
		stream.start();
	}

	void onClose()
	{
		stream -= packetDelegate(this, &StreamingRequestHandler::onVideoEncoded);
		stream.stop();
	}

	void onVideoEncoded(void* sender, RawPacket& packet)
	{
		debugL("StreamingRequestHandler") << "Sending Packet: " 
			<< packet.size() << ": " << fpsCounter.fps << endl;

		try {	
			connection().sendRaw((const char*)packet.data(), packet.size());
			fpsCounter.tick();		
		}
		catch (Exception& exc) {
			errorL("StreamingRequestHandler") << "Error: " << exc.displayText() << endl;
			connection().close();
		}
	}
	
	PacketStream stream;
	StreamingOptions options;
	FPSCounter fpsCounter;
};


// ----------------------------------------------------------------------------
// Releayed Streaming Client Allocation
//
/*
class RelayedMediaStream 
{
public:
	PacketStream    stream;
};
*/

/* 
class RelayedStreamingAllocation: public turn::TCPClientObserver
{
public:
	Runner			runner;
	// Net::Reactor	reactor;

	net::IP			peerIP;	
	turn::TCPClient client;
	
	StreamManager	streams;
	StreamingOptions options;	
	int				frameNumber;
	bool			connected;

	Signal<turn::Client&> AllocationCreated;
	Signal2<turn::Client&, const net::Address&> ConnectionCreated;

	RelayedStreamingAllocation(const StreamingOptions& options, const turn::Client::Options& clientOptions, const net::IP& peerIP) : 
		client(*this, reactor, runner, clientOptions), options(options), peerIP(peerIP), frameNumber(0), connected(false)
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
		debugL("RelayedStreamingAllocation", this) << "Initiating" << endl;		
		//terminate();
		try	
		{		
			// Initiate the TRUN client allocation
			debugL("RelayedStreamingAllocation", this) << "Adding Persission" << endl;		
			client.addPermission(peerIP);	
			debugL("RelayedStreamingAllocation", this) << "Initiating Client" << endl;		
			client.initiate();
		} 
		catch (Exception& exc) {
			errorL()  << "[RelayedStreamingAllocation: " << this << "] Error: " << exc.displayText() << std::endl;
			assert(0);
		}
		debugL("RelayedStreamingAllocation", this) << "Initiating: OK" << endl;	
	}

	void terminate() 
	{
		debugL("RelayedStreamingAllocation", this) << "Terminating" << endl;	
		client.terminate();

		// Free all managed packet streams
		streams.closeAll();
	}

protected:
	void onRelayStateChange(turn::Client& client, turn::ClientState& state, const turn::ClientState&) 
	{
		debugL("RelayedStreamingAllocation", this) << "Relay State Changed: " << state.toString() << endl;

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
	
	void onClientConnectionCreated(turn::TCPClient& client, net::SocketBase* socket, const net::Address& peerAddr) //UInt32 connectionID, 
	{
		debugL("RelayedStreamingAllocation", this) << "################# Connection Created: " << peerAddr << endl;
		
		// Just allow one stream for now
		if (this->streams.size() == 1) {
			debugL("RelayedStreamingAllocation", this) << "@@@@@@@@@@@@@@@@@@@@@@@@@@ Rejecting Connection" << endl;
			return;
		}

		try	
		{	
			// Notify the outside application
			ConnectionCreated.emit(this, client, peerAddr);

			// Create an output media stream for the new connection
			PacketStream* stream = new PacketStream(peerAddr.toString());
			
			// Setup the packet stream ensuring the audio capture isn't
			// destroyed with the stream, as it may be reused while the
			// allocation is active.
			setupPacketStream(*stream, options, false);
		
			// Feed the packet stream directly into the connection		
			stream->attach(packetDelegate(socket, &net::SocketBase::send));

			// Start the stream
			stream->start();	

			this->streams.addStream(stream);
		} 
		catch (Exception& exc) {
			errorL()  << "[RelayedStreamingAllocation: " << this << "] Stream Error: " << exc.displayText() << std::endl;
			assert(0);
		}

		debugL("RelayedStreamingAllocation", this) << "Connection Created: OK: " << peerAddr << endl;
	}
		
	void onClientConnectionClosed(turn::TCPClient& client, net::SocketBase* socket, const net::Address& peerAddress)
	{
		debugL("RelayedStreamingAllocation", this) << "!!!!!!!!!!!!!!!!!!!!!!!! Connection Closed: " << peerAddress << endl;

		try	
		{	
			// Destroy the media stream for the closed connection (if any).
			//this->streams.free(peerAddress.toString());
			PacketStream* stream = streams.remove(peerAddress.toString());
			if (stream) {
				// Feed the packet stream directly into the connection		
				stream->detach(packetDelegate(socket, &net::SocketBase::send));
				//delete stream;
			}
		} 
		catch (Exception& exc) {
			errorL()  << "[RelayedStreamingAllocation: " << this << "] Stream Error: " << exc.displayText() << std::endl;
			assert(0);
		}
	}

	void onRelayedData(turn::Client& client, const char* data, int size, const net::Address& peerAddr)
	{
		debugL("RelayedStreamingAllocation", this) << "Received data from peer: " << string(data, size) <<  ": " << peerAddr << endl;
	
		// The remore peer is a web browser, the HTTP request sent 
		// to the relayed address will be the first thing we see...
	}

	void onAllocationPermissionsCreated(turn::Client& client, const turn::PermissionList& permissions)
	{
		debugL("RelayedStreamingAllocation", this) << "Permissions Created" << endl;
	}
};
*/


	/*	
		//client.terminate(); 
		//stopMedia();
	//PacketStream    stream;

	void playMedia() 
	{
		debugL("RelayedStreamingAllocation", this) << "Play Media" << endl;

		// Create the packet stream
		//debugL("RelayedStreamingAllocation", this) << "Setup Packet Stream" << endl;	
		//setupPacketStream(stream, options);
		//debugL("RelayedStreamingAllocation", this) << "Setup Packet Stream: OK" << endl;	

		// Start the stream
		stream += packetDelegate(this, &RelayedStreamingAllocation::onMediaEncoded);
		stream.start();		

		debugL("RelayedStreamingAllocation", this) << "Play Media: OK" << endl;
	}

	void stopMedia()
	{
		debugL("RelayedStreamingAllocation", this) << "Stop Media" << endl;
		stream -= packetDelegate(this, &RelayedStreamingAllocation::onMediaEncoded);
		stream.close();	
		debugL("RelayedStreamingAllocation", this) << "Stop Media: OK" << endl;
	}
	*/

	/*
	void onMediaEncoded(void* sender, RawPacket& packet)
	{
		debugL("RelayedStreamingAllocation", this) << "$$$$$$$$$$$$$$ Sending Packet: " << packet.size() << string((const char*)packet.data(), 100)  << endl;
		
		//assert(currentPeerAddr.valid());
		try {
			// Send the media to our peer
			//client.sendData((const char*)packet.data(), packet.size(), currentPeerAddr);
			//ostringstream oss;
			//oss << "Packet: " << frameNumber++ << endl;			
			//client.sendData(oss.str().data(), oss.str().length(), currentPeerAddr);
		}
		catch (Exception& exc) {
			errorL("RelayedStreamingAllocation", this) << "^^^^^^^^^^^^^^^^^^^^^^^^ Send Error: " << exc.displayText() << endl;
			
			// TODO: Calling stream.stop() inside stream callback causing deadlock
			terminate();
		}
		debugL("RelayedStreamingAllocation", this) << "!$$$$$$$$$$$$$$ Sending Packet: OK: " << packet.size() << endl;
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
	void onClientConnectionState(turn::TCPClient& client, net::SocketBase*, 
		Net::SocketState& state, const Net::SocketState& oldState) 
	{
		debugL("RelayedStreamingAllocation", this) << "@@@@@@@@@@@@@@@@@@@@@ Connection State: " << state.toString() << endl;
	}

		//assert(currentPeerAddr.valid());
		//assert(connected);
		//playMedia();


// ----------------------------------------------------------------------------
// Releayed Streaming Connection Handler
//
class RelayedStreamingRequestHandler: public http::ServerResponder
{
public:
	RelayedStreamingRequestHandler(const StreamingOptions& options) : 		
		options(options), allocation(allocation), response(response)
	{		
	}

	~RelayedStreamingRequestHandler() 
	{
	}
		
	void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
	{
		debugL("RelayedStreamingRequestHandler") << "Running: " 
			<< "\n\tOutput Format: " << options.oformat.name
			<< "\n\tOutput Encoding: " << options.encoding
			<< "\n\tOutput Packetizer: " << options.packetizer
			<< endl;
		
		response.set("Access-Control-Allow-Origin", "*");
		this->response = &response;
				
		//net::IP peerIP("127.0.0.1");
		turn::Client::Options co;
		co.serverAddr = net::Address("127.0.0.1", 3478); // "173.230.150.125"
		co.lifetime  = 120 * 1000;	// 2 minutes
		co.timeout = 10 * 1000;
		co.timerInterval = 3 * 1000;
		co.username = Anionu_API_USERNAME;
		co.password = Anionu_API_PASSWORD;

		allocation = new RelayedStreamingAllocation(options, co, request.clientAddress().host());
		allocation->AllocationCreated += delegate(this, &RelayedStreamingRequestHandler::onAllocationCreated);
		allocation->initiate();
		
		debugL("RelayedStreamingRequestHandler", this) << "Waiting" << endl;	
		stopSignal.wait();
		debugL("RelayedStreamingRequestHandler", this) << "Stopped" << endl;	
		
		allocation->AllocationCreated -= delegate(this, &RelayedStreamingRequestHandler::onAllocationCreated);

		// TODO: The allocation will outlive this handler instance
		// Manage allocation via MediaService?

		debugL("RelayedStreamingRequestHandler", this) << "Exiting" << endl;
	}
	
	void onAllocationCreated(void* sender, turn::Client& client)
	{
		debugL("RelayedStreamingRequestHandler", this) << "Allocation Created" << endl;
					
		// Write the relay address	
		assert(this->response);
		this->response->send() 
			<< allocation->client.relayedAddress()
			<< std::flush;

		debugL("RelayedStreamingRequestHandler", this) << "Allocation Created 1" << endl;		
		stopSignal.set();
		debugL("RelayedStreamingRequestHandler", this) << "Allocation Created 2" << endl;
	}
	
	//RelayedStreamingAllocation* allocation;
	Poco::Net::HTTPServerResponse* response;
	StreamingOptions options;
	Poco::Event stopSignal;
	FPSCounter fpsCounter;
};
		//assert(ourMediaProvider);
		//socket().sendBytes(data.data(), data.size());			
		//Buffer(data.data(), data.size());
		//string data = allocation->client.relayedAddress();
	//Poco::Net::StreamSocket socket;
	//VideoCapture* videoCapture;
	//AudioCapture* audioCapture;
	*/


// ---------------------------------------------------------------------
//
class SnapshotRequestHandler: public http::ServerResponder
{
public:
	SnapshotRequestHandler(http::ServerConnection& connection, VideoCapture* videoCapture = NULL, int width = 480, int height = 320) : 
		http::ServerResponder(connection), videoCapture(videoCapture), width(width), height(height)
	{		
	}

	~SnapshotRequestHandler() 
	{
	}
			
	void onRequest(http::Request& request, http::Response& response) 
	{
		debugL("SnapshotRequestHandler") << "Running" << endl;

		cv::Mat frame;
		videoCapture->getFrame(frame, width, height);
		
		vector<unsigned char> buffer;
		vector<int> param = vector<int>(2);
		param[0] = CV_IMWRITE_JPEG_QUALITY;
		param[1] = 95; // default(95) 0-100
		cv::imencode(".jpg", frame, buffer, param);

		debugL("SnapshotRequestHandler") << "Taking Snapshot Image: " 
			<< "\n\tWidth: " << frame.cols 
			<< "\n\tHeight: " << frame.rows 
			<< "\n\tCapture Width: " << videoCapture->width()
			<< "\n\tCapture Height: " << videoCapture->height()
			<< "\n\tType: " << frame.type()
			<< "\n\tInput Size: " << frame.total() 
			<< "\n\tOutput Size: " << buffer.size()
			<< endl;

		unsigned char* data = new unsigned char[buffer.size()];
		copy(buffer.begin(), buffer.end(), data);
		connection().sendRaw((const char*)data, buffer.size());
		delete data;
		connection().close();
	}
	
	int width;
	int height;
	VideoCapture* videoCapture;
};


// ----------------------------------------------------------------------------
//
class WebSocketRequestHandler: public http::ServerResponder
{
public:
	WebSocketRequestHandler(http::ServerConnection& connection, const StreamingOptions& options) : 		
		http::ServerResponder(connection), options(options)
	{	
		debugL("WebSocketRequestHandler") << "Create" << endl;

		// Create the packet stream
		setupPacketStream(stream, options);

		// Start the stream
		stream += packetDelegate(this, &WebSocketRequestHandler::onVideoEncoded);
		stream.start();	
	}

	~WebSocketRequestHandler() 
	{
		debugL("WebSocketRequestHandler") << "Destroy" << endl;			
	}
		
	void onClose() 
	{
		debugL("WebSocketRequestHandler") << "On close" << endl;

		stream.stop();	
		stream -= packetDelegate(this, &WebSocketRequestHandler::onVideoEncoded);
	}
	
	void onVideoEncoded(void* sender, RawPacket& packet)
	{
		debugL("WebSocketRequestHandler") << "Sending Packet: "
			<< packet.size() << ": " << fpsCounter.fps << endl;
		try
		{	
			connection().sendRaw(packet.data(), packet.size(), http::WebSocket::FRAME_BINARY);
			fpsCounter.tick();		
		}
		catch (Exception& exc)
		{
			errorL("StreamingRequestHandler") << "Error: " << exc.displayText() << endl;
			connection().close();
		}
	}
	
	PacketStream stream;
	StreamingOptions options;
	FPSCounter fpsCounter;
};


// ----------------------------------------------------------------------------
// HTTP Streaming Connection Factory
//
class HTTPStreamingConnectionFactory: public http::ServerResponderFactory
{
public:
	HTTPStreamingConnectionFactory(MediaServer* server) : 		
		_server(server)
	{		
	}		
	
	http::ServerResponder* createResponder(http::ServerConnection& conn)
	{
		try 
		{
			http::Request& request = conn.request();

			// Log incoming requests
			infoL("HTTPStreamingConnectionFactory")
				<< "Incoming connection from " << conn.socket().peerAddress() 
				<< ": Request:\n" << request << endl;
			
			// Parse streaming options from query
			StreamingOptions options(_server);
			Poco::Net::NameValueCollection params;
			util::parseURIQuery(request.getURI(), params);
			FormatRegistry& formats = MediaFactory::instance()->formats();				

			// An exception will be thrown if no format was provided, 
			// or if the request format is not registered.
			options.oformat = formats.get(params.get("format", "MJPEG"));
			if (params.has("width"))		
				options.oformat.video.width = util::fromString<UInt32>(params.get("width"));
			if (params.has("height"))	
				options.oformat.video.height = util::fromString<UInt32>(params.get("height"));
			if (params.has("fps"))		
				options.oformat.video.fps = util::fromString<UInt32>(params.get("fps"));
			if (params.has("quality"))	
				options.oformat.video.quality = util::fromString<UInt32>(params.get("quality"));

			// Response encoding and packetizer options
			options.encoding = params.get("encoding", "");
			options.packetizer = params.get("packetizer", "");
			
			// Video captures must be initialized in the main thread. 
			// See MediaFactory::loadVideo			
			VideoCapture* videoCapture = NULL;
			AudioCapture* audioCapture = NULL;
			MediaFactory* mediaFactory = av::MediaFactory::instance();
			if (options.oformat.video.enabled) {
				av::Device video;
				mediaFactory->devices().getDefaultVideoCaptureDevice(video);
				options.videoCapture = mediaFactory->getVideoCapture(video.id);
				av::setVideoCaptureInputFormat(options.videoCapture, options.iformat);	
			}
			if (options.oformat.audio.enabled) {
				av::Device audio;
				mediaFactory->devices().getDefaultAudioInputDevice(audio);
				options.audioCapture = mediaFactory->createAudioCapture(audio.id, 
					options.oformat.audio.channels, 
					options.oformat.audio.sampleRate);
			}		
			
			// Handle websocket connections
			if (request.has("Sec-WebSocket-Key") ||
				request.getURI().find("/websocket") == 0) {		
				return new WebSocketRequestHandler(conn, options);
			}	

			// Handle HTTP streaming
			if (request.getURI().find("/streaming") == 0) {			
				return new StreamingRequestHandler(conn, options);
			}

			// Handle relayed media requests
			//if (request.getURI().find("/relay") == 0) {			
			//	return new RelayedStreamingRequestHandler(conn, options);
			//}

			// Handle HTTP snapshot requests
			if (request.getURI().find("/snapshot") == 0) {			
				return new SnapshotRequestHandler(conn, videoCapture, options.oformat.video.width, options.oformat.video.height);	
			}	
		}
		catch (Exception& exc)
		{
			errorL("StreamingRequestHandlerFactory") << "Request Error: " << exc.displayText() << endl;
		}
		
		errorL("StreamingRequestHandlerFactory") << "Bad Request" << endl;
		return new http::BadRequestHandler(conn);	
	}

	MediaServer* _server;
};
	

// ----------------------------------------------------------------------------
// HTTP Media Server
//
class MediaServer: public http::Server
{
public:
	MediaServer(UInt16 port) :
		http::Server(port, new HTTPStreamingConnectionFactory(this))
	{		
		debugL("MediaServer") << "Creating" << endl;
		
		// Pre-initialize video captures in the main thread	
		MediaFactory::initialize();
		MediaFactory::instance()->loadVideo();

		// Register the media formats we will be using
		FormatRegistry& formats = MediaFactory::instance()->formats();		
		formats.registerFormat(Format("MP3", "mp3",
			AudioCodec("MP3", "libmp3lame", 2, 44100, 128000, "s16p")));	
			// Adobe Flash Player requires that audio files be 16bit and have a sample rate of 44.1khz.
			// Flash Player can handle MP3 files encoded at 32kbps, 48kbps, 56kbps, 64kbps, 128kbps, 160kbps or 256kbps.
			// NOTE: 128000 works fine for 44100, but 64000 is borked!
 						
		formats.registerFormat(Format("FLV", "flv", 
			VideoCodec("FLV", "flv", 320, 240)));

		formats.registerFormat(Format("FLV-Speex", "flv",
			VideoCodec("FLV", "flv", 320, 240),
			AudioCodec("Speex", "libspeex", 1, 16000)));	

		formats.registerFormat(Format("Speex", "flv",
			AudioCodec("Speex", "libspeex", 1, 16000)));

		formats.registerFormat(Format("MJPEG", "mjpeg", 
			VideoCodec("MJPEG", "mjpeg", 480, 320, 20)));

		// TODO: Add h264 and newer audio formats when time permits
	}

	~MediaServer()
	{		
		debugL("MediaServer") << "Destroying" << endl;
	
		MediaFactory::instance()->unloadVideo();
		MediaFactory::uninitialize();
	}
};


} // namespace scy

			
static void onKillSignal(void* opaque)
{
	reinterpret_cast<MediaServer*>(opaque)->shutdown();
	assert(0);
}


int main(int argc, char** argv)
{
	Logger::instance().add(new ConsoleChannel("debug", TraceLevel));	
	{	
		Runner loop;
		MediaServer server(SERVER_PORT);
		server.start();
		loop.waitForKill(onKillSignal, &server);
	}
	Logger::uninitialize();
	return 0;
}



/*
	
		// TODO: Handle input commands..
		char o = 0;
		while (o != 'Q') {	
			cout << 
				"COMMANDS:\n\n"
				"  Q	Quit.\n\n";
		
			o = toupper(getch());
		}
//#include "Sourcey/Net/TCPService.h"
#include "Sourcey/HTTP/MultipartPacketizer.h"
#include "Sourcey/HTTP/StreamingPacketizer.h"
#include "Sourcey/HTTP/ChunkedPacketizer.h"
*/
/*
//#include "Poco/HTTP/WebSocket.h"
//#include "Poco/Net/WebSocketImpl.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerRequestImpl.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/NetException.h"
*/

		/*
		// Detach the socket for direct access
		//socket = static_cast<Poco::Net::HTTPServerRequestImpl&>(request).detachSocket();
		try
		{	

		// Wait for stop signal
		debugL("StreamingRequestHandler") << "Waiting" << endl;				
		stopSignal.wait();
		debugL("StreamingRequestHandler") << "Stopped" << endl;	
			

			socket.close();
		}
		catch (Exception& exc)
		{
			errorL("StreamingRequestHandler") << "Error: " << exc.displayText() << endl;
		}

		debugL("StreamingRequestHandler") << "Exiting" << endl;
		*/


	/*
	
	//av::Device video;
	//av::Device audio;
	//MediaFactory::instance()->devices().getDefaultVideoCaptureDevice(video);
	//MediaFactory::instance()->devices().getDefaultAudioInputDevice(audio);
		//Poco::Net::ServerSocket socket(328);
		//server.addConnectionHook(new http::FlashPolicyConnectionHook);	

#if 0
			if (gAVVideoCapture) {
				InitVideoCodecFromContext(gAVVideoCapture->video()->ctx, options.iformat.video);					
			}
			else {
				if (gVideoCapture && options.oformat.video.enabled)
					av::setVideoCaptureInputForma(gVideoCapture, options.iformat);	
			
				if (gAudioCapture && options.oformat.audio.enabled)
					AllocateRtAudioInputFormat(gAudioCapture, options.iformat);
			}
#endif	
	void onRequest(http::Request& request, http::Response& response) 
	{
		debugL("WebSocketRequestHandler") << "Running" << endl;

		try
		{			
			try
			{	
				http::WebSocket ws(request, response);	
				webSocket = &ws;

				// Wait for stop signal
				debugL("WebSocketRequestHandler") << "Waiting" << endl;				
				stopSignal.wait();
				debugL("WebSocketRequestHandler") << "Stopped" << endl;	
			}
			catch (Exception& exc)
			{
				errorL("WebSocketRequestHandler") << "Error: " << exc.displayText() << endl;
			}
			
			debugL("WebSocketRequestHandler") << "Stopped" << endl;
		}
		catch (http::WebSocketException& exc)
		{
			errorL("WebSocketRequestHandler") << "Error: " << exc.code() << ": " << exc.displayText() << endl;			
			switch (exc.code())
			{
			case http::WebSocket::WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION:
				response.set("Sec-WebSocket-Version", http::WebSocket::WEBSOCKET_VERSION);
				// fallthrough
			case http::WebSocket::WS_ERR_NO_HANDSHAKE:
			case http::WebSocket::WS_ERR_HANDSHAKE_NO_VERSION:
			case http::WebSocket::WS_ERR_HANDSHAKE_NO_KEY:
				response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
				response.setContentLength(0);
				response.send();
				break;
			}
		}
		
		debugL("WebSocketRequestHandler") << "Exiting" << endl;
	}
	void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
	{
	//VideoCapture* videoCapture;
	//AudioCapture* audioCapture;
	//http::WebSocket* webSocket;
	//Poco::Event stopSignal;
	*/



/*//, 16800 //-16000 //-16000	//, 6, 9000, 64000
 //, 48000, 128000, "yuvj420p"
	
			
	//formats.registerFormat(Format("mp3-8000", "mp3", 
	//	AudioCodec("MP3", "libmp3lame", 1, 8000, 64000)));
			
	//formats.registerFormat(Format("mp3-311000", "mp3", 
	//	AudioCodec("MP3", "libmp3lame", 1, 11000, 16000)));

	//formats.registerFormat(Format("mp3-48000", "mp3", 
	//	AudioCodec("MP3", "libmp3lame", 2, 48000, 128000, "s16p")));

	//formats.registerFormat(Format("MJPEG-base64-mxhr", "mjpeg", 
	//	VideoCodec("MJPEG", "mjpeg", 196, 128, 25, 48000, 128000, "yuvj420p")));
	
		//Poco::Net::ServerSocket ss(328);
		//http::Server server(new HTTPStreamingConnectionFactory, ss, new Poco::Net::HTTPServerParams);

// ---------------------------------------------------------------------
//
class FlashPolicyConnectionHook: public http::ServerConnectionHook
{
public:
	Poco::Net::TCPServerConnection* createConnection(const Poco::Net::StreamSocket& socket, const std::string& rawRequest)
	{		
		try 
		{			
			if (rawRequest.find("policy-file-request") != string::npos) {
				debugL("StreamingRequestHandlerFactory") << "Sending Flash Crossdomain XMLSocket Policy" << endl;
				return new Net::FlashPolicyRequestHandler(socket, false);
			}
			else if (rawRequest.find("crossdomain.xml") != string::npos) {
				debugL("StreamingRequestHandlerFactory") << "Sending Flash Crossdomain HTTP Policy" << endl;
				return new Net::FlashPolicyRequestHandler(socket, true);
			}			
		}
		catch (Exception& exc)
		{
			errorL("ServerConnectionHook") << "Bad Request: " << exc.displayText() << endl;
		}	
		return NULL;
	};
};
*/


/*	

			// Handle flash policy file requests (because we have to)
			if (request.getURI().find("/crossdomain.xml") == 0) {
				return new FlashPolicyRequestHandler1();
			}
// ----------------------------------------------------------------------------
// Flash Policy Connection
//
class FlashPolicyRequestHandler1: public http::ServerResponder
{
public:
	void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
	{
		response.set("Content-Type", "text/x-cross-domain-policy");
		response.set("X-Permitted-Cross-Domain-Policies", "all");	
		string policy("<?xml version=\"1.0\"?><cross-domain-policy><allow-access-from domain=\"*\" to-ports=\"*\" /></cross-domain-policy>");
				
		//if (request.getChunkedTransferEncoding())
		//	response.setChunkedTransferEncoding(true);
		//else if (request.getContentLength() != HTTPMessage::UNKNOWN_CONTENT_LENGTH)
		//	response.setContentLength(request.getContentLength());
		//response.setContentType(request.getContentType());
		response.setContentLength(policy.length()); //\r\n
						
		//std::istream& istr = request.stream();
		std::ostream& ostr = response.send();
		ostr << policy;
		ostr.flush();
		//std::streamsize n = StreamCopier::copyStream(istr, ostr);

		//response.setContentLength(policy.length());\r\n
		//
		// < allow-access-from domain="*" secure="false" to-ports="*" >  + 1
		//response.sendBuffer(policy.data(), policy.length()  + 1); // << policy << std::flush;
		//response.fl
		//response.s
		//request.

		debugL() << "Sending policy file response: " << policy << endl;
	}
};
*/
	
	/*



// ----------------------------------------------------------------------------
// HTTP Connection Factory
//
class StreamingRequestHandlerFactory: public http::ServerResponderFactory
{
public:
	Poco::Net::TCPServerConnection* createConnection(const Poco::Net::StreamSocket& socket) 
	{  
		debugL("StreamingRequestHandlerFactory") << "Creating Connection" << endl;
	
		Poco::Net::StreamSocket sock(socket);
		try 
		{
			// Wait until we have some data in the sock read buffer
			// then parse and redirect accordingly.
			char buffer[4096];
			sock.setReceiveTimeout(Poco::Timespan(2,0));
			int size = sock.receiveBytes(buffer, sizeof(buffer));
			string rawRequest(buffer, size);

			// Log genuine incoming connections to our production log.
			// Disabling for now because service may be hammered for 
			// snapshots for pseudo MJPEG stream.
			debugL("StreamingRequestHandlerFactory")
				<< "Incoming HTTP connection: " << sock.peerAddress() 
				<< "\n\tRequest: " << rawRequest
				<< endl;

			//
			// Flash policy-file-requests
			if ((rawRequest.find("policy-file-request") != string::npos) ||
				(rawRequest.find("crossdomain") != string::npos)) {
				debugL("StreamingRequestHandlerFactory") << "Sending Flash Crossdomain Policy" << endl;
				return new Net::FlashPolicyRequestHandler(sock);
			}

			//assert(0);

		}
		catch (Exception& exc)
		{
			errorL("StreamingRequestHandlerFactory") << "Bad Request: " << exc.displayText() << endl;
		}

		return new Net::NullRequestHandler(sock);
	}
};


		// Create a WebSocket server on 329
		//Poco::Net::ServerSocket ss(328);
		//Poco::Net::HTTPServer server(new HTTPStreamingConnectionFactory, ss, new Poco::Net::HTTPServerParams);
		//server.start();
		
		//MediaServer ms(328);
		//ms.start();
		

	std::string encoding;
	//std::string peer;		// Peer ID of the initiating party
	//std::string channel;	// The channel we are streaming from
	//std::string token;		// The session's unique identification token
	//std::string transport;	// The transport protocol [UDP, TCP, TLS]
	//std::string protocol;	// The packet protocol [Raw, HTTP, RTP/AVP]
	//std::string mime;		// The HTTP packet content type [None, image/jpeg, multipart/x-mixed-replace, ...]
	//int timeout;			// The lifetime after disconnection timeout value

	//bool disableLAN;		// Disable LAN candidates
	//bool disableHost;		// Disable Host candidates
	//bool disableRelay;		// Disable Relay candidates

	bool valid() const 
	{
		return //!peer.empty()
			//&& !channel.empty()
			//&& !token.empty() &&
			(transport == "TCP" 
			||  transport == "SSL" 
			||  transport == "UDP") 
			&& (oformat.video.enabled 
			||  oformat.audio.enabled);
	}//,
			//mime(mime),
			//timeout(timeout),
			//disableLAN(false),
			//disableHost(false),
			//disableRelay(false) 
			//peer(peer),
			//channel(channel),
			//transport(transport),
			//protocol(protocol),//,
		//const std::string& peer = "",
		//const std::string& channel = "",
		//const std::string& mime = "None",
		//int timeout = 20000
		//const std::string& transport = "TCP",
		//const std::string& protocol = "Raw",
	*/


//inline createMediaStream() 
		//http::ServerResponder(sock), options(options), encoder(NULL), 
		//const StreamSocket& sock, const StreamingOptions& options, 
				
			/*
			//StreamingOptions options;
			// An exception will be thrown if no format was provided, 
			// or if the request format is not registered.
			Poco::Net::NameValueCollection params;
			util::parseURIQuery(request.getURI(), params);
			FormatRegistry& formats = MediaFactory::instance()->formats();	
			options.oformat = formats.get(params.get("format"));
			if (params.has("width"))		
				options.oformat.video.width = util::fromString<UInt32>(params.get("width"));
			if (params.has("height"))	
				options.oformat.video.height = util::fromString<UInt32>(params.get("height"));
			if (params.has("fps"))		
				options.oformat.video.fps = util::fromString<UInt32>(params.get("fps"));
			if (params.has("quality"))	
				options.oformat.video.quality = util::fromString<UInt32>(params.get("quality"));
				*/
	
			/*
			// Wait until we have some data in the sock read buffer
			// then parse and redirect accordingly.
			char buffer[4096];
			sock.setReceiveTimeout(Timespan(2,0));
			int size = sock.receiveBytes(buffer, sizeof(buffer));
			string rawRequest(buffer, size);

			//
			// Flash policy-file-requests
			if ((rawRequest.find("policy-file-request") != string::npos) ||
				(rawRequest.find("crossdomain") != string::npos)) {
				debugL("StreamingRequestHandlerFactory") << "Sending Flash Crossdomain Policy" << endl;
				return new FlashPolicyRequestHandler1(sock);
			}

			// Log genuine incoming connections to our production log.
			// Disabling for now because service may be hammered for 
			// snapshots for pseudo MJPEG stream.
			debugL("StreamingRequestHandlerFactory")
				<< "Incoming HTTP connection: " << request.clientAddress() 
				<< "\n\tRequest: " << rawRequest
				<< endl;

			//http::Request* request = new 
			http::Request request;
			istringstream ist(rawRequest);
			request.read(ist);
			*/

	//StreamingOptions options;
	//AVEncoder* encoder;
	//PacketStream stream;

		// Create the main MediaServer server on 328
		//MediaServer srv(328);
		//srv.start();


						
				/*
				StreamingOptions options;
				// An exception will be thrown if no format was provided, 
				// or if the request format is not registered.
				FormatRegistry& formats = MediaFactory::instance()->formats();	
				options.oformat = formats.get("mjpeg");

				// Video captures should always be instantiated
				// in the main thread. See MediaFactory::loadVideo
				VideoCapture* videoCapture = NULL;
				AudioCapture* audioCapture = NULL;
				if (options.oformat.video.enabled) {
					videoCapture = MediaFactory::instance()->getVideoCapture(0);
					av::setVideoCaptureInputForma(videoCapture, options.iformat);	
				}
				if (options.oformat.audio.enabled)
					audioCapture = MediaFactory::instance()->createAudioCapture(0, 
						options.oformat.audio.channels, 
						options.oformat.audio.sampleRate);
						*/


	/*
				//VideoCapture* videoCapture = MediaFactory::instance()->getVideoCapture(0);
				//int width = scy::util::fromString<int>(request.params().get("width", scy::util::toString(videoCapture->width() ? videoCapture->width() : 480)));
				//int height = scy::util::fromString<int>(request.params().get("height", scy::util::toString(videoCapture->height() ? videoCapture->height() : 320)));

// ----------------------------------------------------------------------------
// HTTP Connection Factory
//
class StreamingRequestHandlerFactory: public http::ServerResponderFactory
{
public:
	TCPServerConnection* createConnection(const StreamSocket& socket) 
	{  
		debugL("StreamingRequestHandlerFactory") << "Creating Connection" << endl;
	
		StreamSocket sock(socket);
		try 
		{
			// Wait until we have some data in the sock read buffer
			// then parse and redirect accordingly.
			char buffer[4096];
			sock.setReceiveTimeout(Timespan(2,0));
			int size = sock.receiveBytes(buffer, sizeof(buffer));
			string rawRequest(buffer, size);

			//
			// Flash policy-file-requests
			if ((rawRequest.find("policy-file-request") != string::npos) ||
				(rawRequest.find("crossdomain") != string::npos)) {
				debugL("StreamingRequestHandlerFactory") << "Sending Flash Crossdomain Policy" << endl;
				return new FlashPolicyRequestHandler1(sock);
			}

			// Log genuine incoming connections to our production log.
			// Disabling for now because service may be hammered for 
			// snapshots for pseudo MJPEG stream.
			debugL("StreamingRequestHandlerFactory")
				<< "Incoming HTTP connection: " << sock.peerAddress() 
				<< "\n\tRequest: " << rawRequest
				<< endl;

			//http::Request* request = new 
			http::Request request;
			istringstream ist(rawRequest);
			request.read(ist);

			if ((request.matches("/streaming"))) {
			
				StreamingOptions options;
						
				// An exception will be thrown if no format was provided, 
				// or if the request format is not registered.
				FormatRegistry& formats = MediaFactory::instance()->formats();	
				options.oformat = formats.get(request.params().get("format"));

				if (request.params().has("width"))		
					options.oformat.video.width = util::fromString<UInt32>(request.params().get("width"));
				if (request.params().has("height"))	
					options.oformat.video.height = util::fromString<UInt32>(request.params().get("height"));
				if (request.params().has("fps"))		
					options.oformat.video.fps = util::fromString<UInt32>(request.params().get("fps"));
				if (request.params().has("quality"))	
					options.oformat.video.quality = util::fromString<UInt32>(request.params().get("quality"));

				// Video captures should always be instantiated
				// in the main thread. See MediaFactory::loadVideo
				VideoCapture* videoCapture = NULL;
				AudioCapture* audioCapture = NULL;
				if (options.oformat.video.enabled) {
					videoCapture = MediaFactory::instance()->getVideoCapture(0);
					av::setVideoCaptureInputForma(videoCapture, options.iformat);	
				}
				if (options.oformat.audio.enabled)
					audioCapture = MediaFactory::instance()->createAudioCapture(0, 
						options.oformat.audio.channels, 
						options.oformat.audio.sampleRate);

#if 0
				if (gAVVideoCapture) {
					InitVideoCodecFromContext(gAVVideoCapture->video()->ctx, options.iformat.video);					
				}
				else {
					if (gVideoCapture && options.oformat.video.enabled)
						av::setVideoCaptureInputForma(gVideoCapture, options.iformat);	
			
					if (gAudioCapture && options.oformat.audio.enabled)
						AllocateRtAudioInputFormat(gAudioCapture, options.iformat);
				}
#endif			
			
				debugL("StreamingRequestHandlerFactory") << "Creating Media Connection" << endl;
				//if (request)
				return new StreamingRequestHandler(socket, options, videoCapture, audioCapture);	
			}
			else if ((request.matches("/snapshot"))) {				
				VideoCapture* videoCapture = MediaFactory::instance()->getVideoCapture(0);
				int width = scy::util::fromString<int>(request.params().get("width", scy::util::toString(videoCapture->width() ? videoCapture->width() : 480)));
				int height = scy::util::fromString<int>(request.params().get("height", scy::util::toString(videoCapture->height() ? videoCapture->height() : 320)));
				return new SnapshotRequestHandler(socket, videoCapture, width, height);	
			}
		}
		catch (Exception& exc)
		{
			errorL("StreamingRequestHandlerFactory") << "Bad Request: " << exc.displayText() << endl;
		}

		return new NullRequestHandler(sock);
	}
};
	

// ----------------------------------------------------------------------------
// HTTP Service
//
class MediaServer: public TCPService
{
public:
	MediaServer(unsigned short port) :
		TCPService(new StreamingRequestHandlerFactory(), port)
	{		
		debugL("MediaServer") << "Creating" << endl;
	}

	~MediaServer()
	{		
		debugL("MediaServer") << "Destroying" << endl;
	}
};
		*/

/*
Format FLVNellyMoser11025 = Format("FLV", "flv", 
	VideoCodec("FLV", "flv", 320, 240, 20),
	AudioCodec("Nellymoser", "nellymoser", 1, 11025));	
			
Format FLVNellyMoser11025NoVideo = Format("FLV", "flv", 
	VideoCodec(),
	AudioCodec("Nellymoser", "nellymoser", 1, 11025, 64000, (UInt32)SampleFormat::FLT));	//
	// Supports float only
			
Format FLVH264NoAudio = Format("FLVH264", "flv", 
	VideoCodec("H264", "libx264", 320, 240, 30));	
			
//Format FLVSpeex8000NoVideo = Format("FLV", "flv", 
//	VideoCodec(),
//	AudioCodec("Speex", "libspeex", 1, 8000));	//, 16800
			
Format FLVMP3 = Format("FLV", "flv", 
	VideoCodec("FLV", "flv", 320, 240, 20),
	AudioCodec("MP3", "libmp3lame", 2, 44100, 64000, "s16p"));	

Format AAC44100 = Format("MP4", "mp4",
	//VideoCodec("MPEG4", "mpeg4", 640, 480, 25), 
	//VideoCodec("H264", "libx264", 320, 240, 25),
	AudioCodec("AAC", "aac", 2, 44100));
			
Format FLVAAC = Format("FLV", "flv", 
	VideoCodec("FLV", "flv", 320, 240, 25),
	AudioCodec("AAC", "aac", 2, 44100));	

Format FLVH264AAC = Format("FLV", "flv",
	//VideoCodec("MPEG4", "mpeg4", 640, 480, 25), 
	VideoCodec("H264", "libx264", 320, 240, 25),
	AudioCodec("AAC", "aac", 2, 44100));

Format MPEG4AAC = Format("FLV", "flv",
	VideoCodec("MPEG4", "mpeg4", 640, 480, 25),
	AudioCodec("AAC", "aac", 2, 44100));

Format H264AAC = Format("H264AAC", "flv",
	VideoCodec("H264", "libx264", 640, 480, 25),
	AudioCodec("AAC", "aac", 2, 44100));

Format MP4 = Format("MP4", "mp4",
	VideoCodec("MPEG4", "mpeg4", 320, 240, 25),
	AudioCodec("AC3", "ac3_fixed", 2, 44100));
	*/


// Global for now
 //MJPEG; //FLVNoAudio; //FLVSpeex16000; //Speex16000; //MJPEG; //H264AAC; //MP38000; //MP38000; //MP38000; //MP344100; //MP344100; //AAC44100; //FLVNoAudio; //FLVNellyMoser11025NoVideo; //Speex16000; //FLVNellyMoser11025NoVideo; //FLVH264NoAudio; //MP344100; //FLVNellyMoser11025NoVideo; //FLVH264NoAudio; //FLVNellyMoser11025NoVideo; //FLVNellyMoser11025NoVideo; //FLVNellyMoser11025NoVideo; FLVNellyMoser11025NoVideo; //MP344100; 
//Format gCurrentFormat = FLVNoAudio;
//AVInputReader* gAVVideoCapture = NULL;
//VideoCapture* gVideoCapture = NULL;
//AudioCapture* gAudioCapture = NULL;


			//options.oformat = gCurrentFormat;
			//options.ofile = "test1.flv";
			//options.stopAt = time(0) + 3;	
			
			/*

			//MP344100; //Speex16000; //FLVNoAudio; //Speex16000; 
			//FLVH264AAC; //FLVNellyMoser11025; //FLVAAC; //FLVSpeex16000; //FLVMP3; 
			//NoVideo; //FLVNellyMoser11025; //NoVideo; //FLVMP3; //Speex16000; //FLVSpeex16000; //FLVNoAudio; //
			
			if ((request->matches("/mjpeg-base64-mxhr")))
				options.oformat = MJPEGBase64MXHR;
			
			else if ((request->matches("/mjpeg")))
				options.oformat = MJPEG;		
			
			//else if ((request.find("/flv-h264") != string::npos))
			//	options.oformat = FLVH264NoAudio;
			
			else if ((request->matches("/flv-speex-16000")))
				options.oformat = FLVSpeex16000; 
			
			else if ((request->matches("/flv")))
				options.oformat = FLVNoAudio;
			
			else if ((request->matches("/mp3-44100")))
				options.oformat = MP344100;		
			
			else if ((request->matches("/speex-11025")))
				options.oformat = Speex16000;

			//else if ((request.find("/nellymoser-11025") != string::npos))
			//	options.oformat = FLVNellyMoser11025NoVideo;

			else 
				throw Exception("No format specified");
				*/

