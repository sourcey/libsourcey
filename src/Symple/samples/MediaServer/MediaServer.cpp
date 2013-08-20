#include "MediaServer.h"
#include "RelayResponder.h"
#include "SnapshotResponder.h"
#include "StreamingResponder.h"
#include "WebSocketResponder.h"

#include "Sourcey/Collection.h"
#include "Sourcey/Media/MediaFactory.h"
#include "Sourcey/Media/AVInputReader.h"
#include "Sourcey/Media/FLVMetadataInjector.h"
#include "Sourcey/Media/FormatRegistry.h"
#include "Sourcey/Media/AVPacketEncoder.h"

#include "Sourcey/HTTP/Packetizers.h"
#include "Sourcey/HTTP/Util.h"
#include "Sourcey/HTTP/Packetizers.h"
#include "Sourcey/Util/Base64PacketEncoder.h"


/*
// Detect Win32 memory Leaks
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


//
// HTTP Media Server
//


MediaServer::MediaServer(UInt16 port) :
	http::Server(port, new HTTPStreamingConnectionFactory(this))
{		
	debugL("MediaServer") << "Creating" << endl;
	
	// Register the media formats we will be using
	FormatRegistry& formats = MediaFactory::instance().formats();		
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


MediaServer::~MediaServer()
{		
	debugL("MediaServer") << "Destroying" << endl;
}


void MediaServer::setupPacketStream(PacketStream& stream, const StreamingOptions& options, bool freeCaptures, bool attachPacketizers) 
{	
	debugL("MediaServer") << "Setup Packet Stream" << endl;

	// Attach capture sources
	if (options.oformat.video.enabled) {
		assert(options.videoCapture);
		stream.attachSource(options.videoCapture, freeCaptures, true);
	}
	if (options.oformat.audio.enabled) {
		assert(options.audioCapture);
		stream.attachSource(options.audioCapture, freeCaptures, true);	
	}
								
	// Attach an async queue so we don't choke
	// the video capture while encoding.
	auto async = new AsyncPacketQueue(options.oformat.name == "MJPEG" ? 10 : 2048);
	stream.attach(async, 3, true);

	// Attach the video encoder				
	auto encoder = new av::AVPacketEncoder(options);
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
			auto base64 = new Base64PacketEncoder();
			stream.attach(base64, 9, true);
		}
		else
			throw Exception("Unsupported encoding method: " + options.encoding);
	}	
	else if (options.oformat.name == "FLV") {

		// Allow mid-stream flash client connection
		auto injector = new FLVMetadataInjector(options.oformat);
		stream.attach(injector, 10);
	}
		
	// Attach the HTTP output packetizer
	IPacketizer* packetizer = nullptr;
	if (options.packetizer.empty() ||
		options.packetizer == "none" ||
		options.packetizer == "None")
		;
		//packetizer = new http::StreamingAdapter("image/jpeg");	

	else if (options.packetizer == "chunked")
		packetizer = new http::ChunkedAdapter("image/jpeg");

	else if (options.packetizer == "multipart")
		packetizer = new http::MultipartAdapter("image/jpeg", options.encoding == "Base64");	// false, 			

	else throw Exception("Unsupported packetizer method: " + options.packetizer);

	if (packetizer)					
		stream.attach(packetizer, 10, true);
					
	// Attach a sync queue to synchronize output with the event loop
	auto sync = new SyncPacketQueue;
	stream.attach(sync, 15, true);
}


//
// HTTP Streaming Connection Factory
//


HTTPStreamingConnectionFactory::HTTPStreamingConnectionFactory(MediaServer* server) : 		
	_server(server)
{		
}		
	
http::ServerResponder* HTTPStreamingConnectionFactory::createResponder(http::ServerConnection& conn)
{
	try 
	{
		auto& request = conn.request();

		// Log incoming requests
		infoL("HTTPStreamingConnectionFactory")
			<< "Incoming connection from " << conn.socket().peerAddress() 
			<< ": Request:\n" << request << endl;
			
		// Parse streaming options from query
		StreamingOptions options(_server);
		NVCollection params;
		request.getURIParameters(params);
		FormatRegistry& formats = MediaFactory::instance().formats();

		// An exception will be thrown if no format was provided, 
		// or if the request format is not registered.
		options.oformat = formats.get(params.get("format", "MJPEG"));
		if (params.has("width"))		
			options.oformat.video.width = util::strtoi<UInt32>(params.get("width"));
		if (params.has("height"))	
			options.oformat.video.height = util::strtoi<UInt32>(params.get("height"));
		if (params.has("fps"))		
			options.oformat.video.fps = util::strtoi<UInt32>(params.get("fps"));
		if (params.has("quality"))	
			options.oformat.video.quality = util::strtoi<UInt32>(params.get("quality"));

		// Response encoding and packetizer options
		options.encoding = params.get("encoding", "");
		options.packetizer = params.get("packetizer", "");			
			
		// Video captures must be initialized in the main thread. 
		// See MediaFactory::loadVideo		
		av::Device dev;	
		auto& media = av::MediaFactory::instance();
		if (options.oformat.video.enabled) {
			media.devices().getDefaultVideoCaptureDevice(dev);
			options.videoCapture = media.createVideoCapture(dev.id);
			options.videoCapture->getEncoderFormat(options.iformat);	
		}
		if (options.oformat.audio.enabled) {
			media.devices().getDefaultAudioInputDevice(dev);
			options.audioCapture = media.createAudioCapture(dev.id, 
				options.oformat.audio.channels, 
				options.oformat.audio.sampleRate);
			options.audioCapture->getEncoderFormat(options.iformat);	
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
		if (request.getURI().find("/relay") == 0) {			
			return new RelayedStreamingResponder(conn, options);
		}

		// Handle HTTP snapshot requests
		if (request.getURI().find("/snapshot") == 0) {			
			return new SnapshotRequestHandler(conn, options);	
		}	
	}
	catch (std::exception&/*Exception&*/ exc)
	{
		errorL("StreamingRequestHandlerFactory") << "Request error: " << exc.what()/*message()*/ << endl;
	}
		
	errorL("StreamingRequestHandlerFactory") << "Bad Request" << endl;
	return new http::BadRequestHandler(conn);	
}


//
// HTTP Streaming Options
//


StreamingOptions::StreamingOptions(MediaServer* server, av::VideoCapture* videoCapture, av::AudioCapture* audioCapture) : 
	server(server), videoCapture(videoCapture), audioCapture(audioCapture) 
{
	debugL("StreamingOptions", this) << "Destroy" << endl;	
}

StreamingOptions::~StreamingOptions() 
{
	debugL("StreamingOptions", this) << "Destroy" << endl;			
}


} // namespace scy

	
static void onShutdown1(void* opaque)
{
	reinterpret_cast<MediaServer*>(opaque)->shutdown();
}


int main(int argc, char** argv)
{
	Logger::instance().add(new ConsoleChannel("debug", LTrace));		
	{
		// Pre-initialize video captures in the main thread	
		MediaFactory::instance().loadVideo();	

		// Start the application and server
		Application app;
		{			
			MediaServer server(SERVER_PORT);
			server.start();

			// Wait for Ctrl-C
			app.waitForShutdown(onShutdown1, &server);
		}
			
		// Free all pointers pending garbage collection
		//
		// Do this before shutting down the MediaFactory as
		// capture instances may be pending deletion and we 
		// need to dereference the implementation instances
		// so system devices can be properly released.
		GarbageCollector::instance().finalize();

		// Shutdown the media factory and release devices
		MediaFactory::instance().unloadVideo();		
		MediaFactory::shutdown();

		// Shutdown the garbage collector once and for all
		GarbageCollector::instance().shutdown();	
		
		// Finalize the application to free all memory
		// Note: 2 tiny mem leaks (964 bytes) are from OpenCV
		app.finalize();
	}
	Logger::shutdown();
	return 0;
}