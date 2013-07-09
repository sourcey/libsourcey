#include "Sourcey/Runner.h"
#include "Sourcey/PacketStream.h"
#include "Sourcey/Media/VideoCapture.h"
#include "Sourcey/Media/AVInputReader.h"
#include "Sourcey/Media/AVEncoder.h"
#include "Sourcey/HTTP/Server.h"


using namespace std;
using namespace scy;


/*
// Detect Memory Leaks
#ifdef _DEBUG
#include "MemLeakDetect/MemLeakDetect.h"
#include "MemLeakDetect/MemLeakDetect.cpp"
CMemLeakDetect memLeakDetect;
#endif
*/


#define USE_AVDEVICE_CAPTURE 0


#if USE_AVDEVICE_CAPTURE
av::AVInputReader* gVideoCapture;
#else
av::VideoCapture* gVideoCapture;
#endif


namespace scy { 


class MPEGResponder: public http::ServerResponder
{	
	av::FPSCounter fpsCounter;
	PacketStream stream;

public:
	MPEGResponder(http::ServerConnection& conn) : 
		http::ServerResponder(conn)
	{
		debugL("MPEGResponder") << "Creating" << endl;

		// We will be sending our own headers
		conn.shouldSendHeaders(false);

		// Attach the video capture
		stream.attach(gVideoCapture, false);
		
		// Setup the encoder options
		av::RecordingOptions options;	
		options.oformat = av::Format("MJPEG", "mjpeg", av::VideoCodec(
			"MJPEG", "mjpeg", 400, 300, 25, 48000, 128000, "yuvj420p"));
		av::setVideoCaptureInputFormat(gVideoCapture, options.iformat);

		// Create and attach the encoder
		av::AVEncoder* encoder = new av::AVEncoder(options);
		encoder->initialize();
		stream.attach(encoder, 5, true);		
				
		// Create and attach the HTTP multipart packetizer
		http::MultipartPacketizer* packetizer = new http::MultipartPacketizer("image/jpeg", false);
		stream.attach(packetizer, 10);	
			
		// Start the stream
		stream += packetDelegate(this, &MPEGResponder::onVideoEncoded);
		stream.start();
	}

	~MPEGResponder()
	{
		debugL("MPEGResponder") << "Destroying" << endl;
	}

	void onPayload(const Buffer& body)
	{
		debugL("MPEGResponder") << "On recv payload: " << body << endl;

		// do something with data from peer
	}

	void onClose()
	{
		debugL("MPEGResponder") << "On close" << endl;
			
		stream -= packetDelegate(this, &MPEGResponder::onVideoEncoded);
		stream.stop();
	}

	void onVideoEncoded(void* sender, RawPacket& packet)
	{
		traceL("MPEGResponder") << "Sending packet: "
			<< packet.size() << ": " << fpsCounter.fps << endl;

		try {		
			connection().sendRaw(packet.data(), packet.size());
			fpsCounter.tick();		
		}
		catch (Exception& exc) {
			errorL("MPEGResponder") << "Error: " << exc.displayText() << endl;
			connection().close();
		}
	}
};


// -------------------------------------------------------------------
//
class StreamingResponderFactory: public http::ServerResponderFactory
{
public:
	http::ServerResponder* createResponder(http::ServerConnection& conn)
	{		
		return new MPEGResponder(conn);
	}
};


} // namespace scy

			
static void onKillSignal(void* opaque)
{
	reinterpret_cast<http::Server*>(opaque)->shutdown();
	assert(0);
}


int main(int argc, char** argv)
{
	Logger::instance().add(new ConsoleChannel("debug", TraceLevel));

#if USE_AVDEVICE_CAPTURE
	gVideoCapture = new av::AVInputReader();
	//gAVVideoCapture->openDevice(0);
	gVideoCapture->openFile(VIDEO_FILE_SOURCE);
	gVideoCapture->start();
#else
	// VideoCapture instances must be
	// instantiated in the main thread. 
	gVideoCapture = new av::VideoCapture(0);
#endif
	
	{
		Runner loop; 
		http::Server server(328, new StreamingResponderFactory);
		server.start();
		loop.waitForKill(onKillSignal, &server);
	}
	
	delete gVideoCapture;
	Logger::uninitialize();
	return 0;
}