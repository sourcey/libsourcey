#include "Sourcey/Logger.h"
#include "Sourcey/Runner.h"
#include "Sourcey/PacketStream.h"
#include "Sourcey/Util.h"
#include "Sourcey/Media/VideoContext.h"
#include "Sourcey/Media/MediaFactory.h"
#include "Sourcey/Media/AVEncoder.h"
#include "Sourcey/Media/AVInputReader.h"
#include "Sourcey/Media/FLVMetadataInjector.h"
#include "Sourcey/Media/FormatRegistry.h"
#include "Sourcey/Net/TCPService.h"
#include "Sourcey/HTTP/MultipartPacketizer.h"
#include "Sourcey/HTTP/StreamingPacketizer.h"
#include "Sourcey/HTTP/ChunkedPacketizer.h"
#include "Sourcey/HTTP/Request.h"
#include "Sourcey/Util/Base64PacketEncoder.h"
#include "Poco/SingletonHolder.h"

#include <string>
#include <vector>
#include <assert.h>
#include <conio.h>


using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace Scy;
using namespace Scy::Net;
using namespace Scy::Media;
using namespace Scy::Util;


/*
// Detect Memory Leaks
#ifdef _DEBUG
#include "MemLeakDetect/MemLeakDetect.h"
#include "MemLeakDetect/MemLeakDetect.cpp"
CMemLeakDetect memLeakDetect;
#endif
*/


namespace Scy { 
namespace Media {	


// ----------------------------------------------------------------------------
// Media Connection
//
class MediaConnection: public TCPServerConnection
{
public:
	MediaConnection(const StreamSocket& sock, const RecordingOptions& options, 
		VideoCapture* videoCapture = NULL, AudioCapture* audioCapture = NULL) : 
		TCPServerConnection(sock), options(options), encoder(NULL), 
		videoCapture(videoCapture), audioCapture(audioCapture)
	{		
		LogTrace("MediaConnection") << "Creating" << endl;
	}

	~MediaConnection() 
	{
		LogTrace("MediaConnection") << "Destroying" << endl;
	}
		
	void run()
	{
		try
		{
			// Attach media captures
			if (options.oformat.video.enabled && videoCapture)
				stream.attach(videoCapture, false);
			if (options.oformat.audio.enabled && audioCapture)
				stream.attach(audioCapture, false);	

			// Init and attach media encoder				
			encoder = new AVEncoder(options);
			encoder->initialize();
			stream.attach(encoder, 5, true);			
				
			// Add format specific packetizers
			if ((options.oformat.name.find("base64-mxhr") != string::npos)) {
				
				// Base64 encode the MJPEG stream
				Base64PacketEncoder* encoder = new Base64PacketEncoder();
				stream.attach(encoder, 9, true);
				
				// And attach a HTTP streaming packetizer
				//HTTP::StreamingPacketizer* packetizer = new HTTP::StreamingPacketizer("image/jpeg");
				HTTP::ChunkedPacketizer* packetizer = new HTTP::ChunkedPacketizer("image/jpeg");
				stream.attach(packetizer, 10, true);			
			}
			else if ((options.oformat.name.find("mjpeg") != string::npos)) {
				HTTP::MultipartPacketizer* packetizer = new HTTP::MultipartPacketizer("image/jpeg", false);
				stream.attach(packetizer, 10, true);
			}	
			else if ((options.oformat.name.find("flv") != string::npos)) {

				// Allow mid-stream flash client connection
				FLVMetadataInjector* injector = new FLVMetadataInjector(options.oformat);
				stream.attach(injector, 10);
			}	

			// Start the stream
			stream += packetDelegate(this, &MediaConnection::onVideoEncoded);
			stream.start();			

			// Wait for stop signal
			LogTrace("MediaConnection") << "#### Running: Waiting for Stop" << endl;				
			_stop.wait();
			LogTrace("MediaConnection") << "#### Stopped" << endl;
			
			stream -= packetDelegate(this, &MediaConnection::onVideoEncoded);
			stream.stop();		
		}
		catch (Poco::Exception& exc)
		{
			LogError("MediaConnection") << "#### Error: " << exc.displayText() << endl;
		}

		LogTrace("MediaConnection") << "#### Exiting" << endl;
	}

	void onVideoEncoded(void* sender, DataPacket& packet)
	{
		LogTrace("MediaConnection") << "Sending Packet: "
		 << packet.size() << ": " << fpsCounter.fps << endl;
		try
		{					
			socket().sendBytes(packet.data(), packet.size());
			fpsCounter.tick();		
		}
		catch (Poco::Exception& exc)
		{
			LogError("MediaConnection") << "Error: " << exc.displayText() << endl;
			_stop.set();
		}
	}
	
	RecordingOptions options;
	VideoCapture* videoCapture;
	AudioCapture* audioCapture;
	Poco::Event _stop;
	std::ofstream _file;
	AVEncoder* encoder;
	PacketStream stream;
	FPSCounter fpsCounter;
};


// ---------------------------------------------------------------------
//
class SnapshotConnection: public TCPServerConnection
{
public:
	SnapshotConnection(const StreamSocket& sock, VideoCapture* videoCapture = NULL, int width = 480, int height = 320) : 
		TCPServerConnection(sock), videoCapture(videoCapture), width(width), height(height)
	{		
		LogTrace("SnapshotConnection") << "Creating: " 
			<< "\n\tWidth: " << width
			<< "\n\tHeight: " << height
			<< endl;
	}

	~SnapshotConnection() 
	{
		LogTrace("SnapshotConnection") << "Destroying" << endl;
	}

	virtual void run()
	{
		cv::Mat frame;
		videoCapture->getFrame(frame, width, height);
	
		vector<unsigned char> buffer;
		vector<int> param = vector<int>(2);
		param[0] = CV_IMWRITE_JPEG_QUALITY;
		param[1] = 95; // default(95) 0-100

		cv::imencode(".jpg", frame, buffer, param);

		LogDebug("SnapshotConnection") << "Taking Snapshot Image: " 
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
		socket().sendBytes((const char*)data, buffer.size());
	}
	
	int width;
	int height;
	VideoCapture* videoCapture;
};


// ----------------------------------------------------------------------------
// HTTP Connection Factory
//
class MediaConnectionFactory: public Poco::Net::TCPServerConnectionFactory
{
public:
	Media::FormatRegistry formatRegistry;

	MediaConnectionFactory() 
	{
		formatRegistry.registerFormat(Format("mp3-44100", "mp3", 
			AudioCodec("MP3", "libmp3lame", 2, 44100, 128000, "s16p")));	
			// Adobe Flash Player requires that audio files be 16bit and have a sample rate of 44.1khz.
			// Flash Player can handle MP3 files encoded at 32kbps, 48kbps, 56kbps, 64kbps, 128kbps, 160kbps or 256kbps.
			// NOTE: 128000 works fine for 44100, but 64000 is broken!
			
		formatRegistry.registerFormat(Format("mp3-8000", "mp3", 
			AudioCodec("MP3", "libmp3lame", 1, 8000, 64000)));
			
		formatRegistry.registerFormat(Format("mp3-311000", "mp3", 
			AudioCodec("MP3", "libmp3lame", 1, 11000, 16000)));

		formatRegistry.registerFormat(Format("mp3-48000", "mp3", 
			AudioCodec("MP3", "libmp3lame", 2, 48000, 128000, "s16p")));
 						
		formatRegistry.registerFormat(Format("flv", "flv", 
			VideoCodec("FLV", "flv", 320, 240)));	//, 6, 9000, 64000

		formatRegistry.registerFormat(Format("flv-speex-16000", "flv", 
			VideoCodec("FLV", "flv", 320, 240),
			AudioCodec("Speex", "libspeex", 1, 16000)));	

		formatRegistry.registerFormat(Format("speex-16000", "flv", 
			AudioCodec("Speex", "libspeex", 1, 16000)));//, 16800

		formatRegistry.registerFormat(Format("mjpeg", "mjpeg", 
			VideoCodec("MJPEG", "mjpeg", 640, 480, 25, 48000, 128000, "yuvj420p")));

		formatRegistry.registerFormat(Format("mjpeg-base64-mxhr", "mjpeg", 
			VideoCodec("MJPEG", "mjpeg", 196, 128, 25, 48000, 128000, "yuvj420p")));
	}
	

	TCPServerConnection* createConnection(const StreamSocket& socket) 
	{  
		LogTrace("MediaConnectionFactory") << "Creating Connection" << endl;
	
		StreamSocket sock(socket);
		try 
		{
			// Wait until we have some data in the sock read buffer
			// then parse and redirect accordingly.
			char buffer[4096];
			sock.setReceiveTimeout(Timespan(2,0));
			int size = sock.receiveBytes(buffer, sizeof(buffer));
			string requestData(buffer, size);

			//
			// Flash policy-file-requests
			if ((requestData.find("policy-file-request") != string::npos) ||
				(requestData.find("crossdomain") != string::npos)) {
				LogTrace("MediaConnectionFactory") << "Sending Flash Crossdomain Policy" << endl;
				return new FlashPolicyRequestHandler(sock);
			}

			// Log genuine incoming connections to our production log.
			// Disabling for now because service may be hammered for 
			// snapshots for pseudo MJPEG stream.
			LogTrace("MediaConnectionFactory")
				<< "Incoming HTTP connection: " << sock.peerAddress().toString() 
				<< "\n\tRequest: " << requestData
				<< endl;

			//HTTP::Request* request = new 
			HTTP::Request request;
			istringstream ist(requestData);
			request.read(ist);

			/*
			1 Connection + disconnect
			-----------------------------------------------------------
			Total 5 Memory Leaks: 1048 bytes Total Alocations 9402

						
			2 Connections + disconnect
			-----------------------------------------------------------
			Total 5 Memory Leaks: 1048 bytes Total Alocations 16775
			*/

			if ((request.matches("/streaming"))) {
			
				RecordingOptions options;
						
				// An exception will be thrown if no format was provided, 
				// or if the request format is not registered.
				options.oformat = formatRegistry.get(request.params().get("format"));

				if (request.params().has("width"))		
					options.oformat.video.width = Util::fromString<UInt32>(request.params().get("width"));
				if (request.params().has("height"))	
					options.oformat.video.height = Util::fromString<UInt32>(request.params().get("height"));
				if (request.params().has("fps"))		
					options.oformat.video.fps = Util::fromString<UInt32>(request.params().get("fps"));
				if (request.params().has("quality"))	
					options.oformat.video.quality = Util::fromString<UInt32>(request.params().get("quality"));

				// Video captures should always be instantiated
				// in the main thread. See MediaFactory::loadVideo
				VideoCapture* videoCapture = NULL;
				AudioCapture* audioCapture = NULL;
				if (options.oformat.video.enabled) {
					videoCapture = MediaFactory::instance()->getVideoCapture(0);
					AllocateOpenCVInputFormat(videoCapture, options.iformat);	
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
						AllocateOpenCVInputFormat(gVideoCapture, options.iformat);	
			
					if (gAudioCapture && options.oformat.audio.enabled)
						AllocateRtAudioInputFormat(gAudioCapture, options.iformat);
				}
#endif			
			
				LogTrace("MediaConnectionFactory") << "Creating Media Connection" << endl;
				//if (request)
				return new MediaConnection(socket, options, videoCapture, audioCapture);	
			}
			else if ((request.matches("/snapshot"))) {				
				VideoCapture* videoCapture = MediaFactory::instance()->getVideoCapture(0);
				int width = Scy::Util::fromString<int>(request.params().get("width", Scy::Util::toString(videoCapture->width() ? videoCapture->width() : 480)));
				int height = Scy::Util::fromString<int>(request.params().get("height", Scy::Util::toString(videoCapture->height() ? videoCapture->height() : 320)));
				return new SnapshotConnection(socket, videoCapture, width, height);	
			}
		}
		catch (Exception& exc)
		{
			LogError("MediaConnectionFactory") << "Bad Request: " << exc.displayText() << endl;
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
		TCPService(new MediaConnectionFactory(), port)
	{		
		LogTrace("MediaServer") << "Creating" << endl;
	}

	~MediaServer()
	{		
		LogTrace("MediaServer") << "Destroying" << endl;
	}
};


} } // namespace Scy::Media


int main(int argc, char** argv)
{
	Logger::instance().add(new ConsoleChannel("debug", TraceLevel));
				
	MediaFactory::initialize();

	// Pre initialize video captures in the main thread.
	//MediaFactory::instance()->loadVideo(VideoCapture::SyncWithDelegates);

#if 0
	//gAVVideoCapture = new AVInputReader();
	//gAVVideoCapture->openDevice(0);
	//gAVVideoCapture->openFile("big_buck_bunny.mp4");
	//gAVVideoCapture->start();
	
	if (!gAVVideoCapture) {
		gCurrentFormat.video.quality = 100;
		if (gCurrentFormat.video.enabled) {
			gVideoCapture = MediaFactory::instance()->getVideoCapture(0);
		}
		if (gCurrentFormat.audio.enabled) {
			gAudioCapture = MediaFactory::instance()->createAudioCapture(0, 
				gCurrentFormat.audio.channels, 
				gCurrentFormat.audio.sampleRate); 
		}
	}
#endif
	
	{
		MediaServer srv(328);
		srv.start();
	
		char o = 0;
		while (o != 'Q') {	
			cout << 
				"COMMANDS:\n\n"
				"  Q	Quit.\n\n";
		
			o = toupper(getch());
		}
	}
	
	LogTrace("Application") << "Freeing Video Captures" << endl;
	MediaFactory::instance()->unloadVideo();
	MediaFactory::uninitialize();
	LogTrace("Application") << "Exiting" << endl;
	Logger::uninitialize();
	return 0;
}





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

