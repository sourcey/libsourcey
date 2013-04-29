#include "Sourcey/Logger.h"
#include "Sourcey/Runner.h"
#include "Sourcey/PacketStream.h"
#include "Sourcey/Util.h"
#include "Sourcey/Media/VideoContext.h"
#include "Sourcey/Media/MediaFactory.h"
#include "Sourcey/Media/MotionDetector.h"
#include "Sourcey/Media/AVEncoder.h"
#include "Sourcey/Media/AVInputReader.h"
#include "Sourcey/Media/FLVMetadataInjector.h"
#include "Sourcey/Net/TCPService.h"
#include "Sourcey/HTTP/MultipartPacketizer.h"
#include "Sourcey/HTTP/StreamingPacketizer.h"
#include "Sourcey/Util/Base64PacketEncoder.h"

#include <string>
#include <vector>
#include <assert.h>
#include <conio.h>


using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace Sourcey;
using namespace Sourcey::Net;
using namespace Sourcey::Media;
using namespace Sourcey::Util;


/*
// Detect Memory Leaks
#ifdef _DEBUG
#include "MemLeakDetect/MemLeakDetect.h"
#include "MemLeakDetect/MemLeakDetect.cpp"
CMemLeakDetect memLeakDetect;
#endif
*/


namespace Sourcey { 
namespace Media {	

		
// ----------------------------------------------------------------------------
//
// Media Formats
//-------		
Format MP344100 = Format("MP3", "mp3", 
	VideoCodec(),
	AudioCodec("MP3", "libmp3lame", 2, 44100, 128000, "s16p"));	
	// Adobe Flash Player requires that audio files be 16bit and have a sample rate of 44.1khz.
	// Flash Player can handle MP3 files encoded at 32kbps, 48kbps, 56kbps, 64kbps, 128kbps, 160kbps or 256kbps.
	// NOTE: 128000 works fine for 44100, but 64000 is broken!
			
Format MP38000  = Format("MP3", "mp3", 
	VideoCodec(),
	AudioCodec("MP3", "libmp3lame", 1, 8000, 64000)); //)
			
Format MP311000  = Format("MP3", "mp3", 
	VideoCodec(),
	AudioCodec("MP3", "libmp3lame", 1, 11000, 16000));

//16 kbps, 11 khz, Mono, MP3

Format MP348000 = Format("MP3", "mp3", 
	VideoCodec(),
	AudioCodec("MP3", "libmp3lame", 2, 48000, 128000, "s16p"));	//, "s16p")
 						
Format FLVNoAudio = Format("FLV", "flv", 
	VideoCodec("FLV", "flv", 320, 240));	//, 6, 9000, 64000

Format FLVSpeex16000 = Format("FLV", "flv", 
	VideoCodec("FLV", "flv", 320, 240),
	AudioCodec("Speex", "libspeex", 1, 16000));	

Format FLVSpeex16000NoVideo = Format("FLV", "flv", 
	VideoCodec(),
	AudioCodec("Speex", "libspeex", 1, 16000));	//, 16800


Format MJPEG = Format("MJPEG", "mjpeg", 
	VideoCodec("MJPEG", "mjpeg", 640, 480, 25, 48000, 128000, "yuvj420p"));

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
Format currentFormat = FLVNoAudio; //MJPEG; //FLVNoAudio; //FLVSpeex16000; //FLVSpeex16000NoVideo; //MJPEG; //H264AAC; //MP38000; //MP38000; //MP38000; //MP344100; //MP344100; //AAC44100; //FLVNoAudio; //FLVNellyMoser11025NoVideo; //FLVSpeex16000NoVideo; //FLVNellyMoser11025NoVideo; //FLVH264NoAudio; //MP344100; //FLVNellyMoser11025NoVideo; //FLVH264NoAudio; //FLVNellyMoser11025NoVideo; //FLVNellyMoser11025NoVideo; //FLVNellyMoser11025NoVideo; FLVNellyMoser11025NoVideo; //MP344100; 
AVInputReader* avVideoCapture = NULL;
VideoCapture* videoCapture = NULL;
AudioCapture* audioCapture = NULL;


// ----------------------------------------------------------------------------
//
// Media Connection
//-------
class MediaConnection: public TCPServerConnection
{
public:
	MediaConnection(const StreamSocket& sock, const RecorderOptions& options, 
		VideoCapture* videoCapture = NULL, AudioCapture* audioCapture = NULL) : 
		TCPServerConnection(sock), options(options), encoder(NULL), 
		videoCapture(videoCapture), audioCapture(audioCapture)
	{		
		LogTrace() << "[MediaConnection] Creating" << endl;
	}

	~MediaConnection() 
	{
		LogTrace() << "[MediaConnection] Destroying" << endl;
	}
		
	void run()
	{
		try
		{
			LogTrace() << "[MediaConnection] Running" << endl;		
						
			if (avVideoCapture) {
				stream.attach(avVideoCapture, false);
				Media::InitVideoCodecFromContext(avVideoCapture->video()->ctx, options.iformat.video);
			}

			else {
				if (options.oformat.video.enabled && videoCapture) {
					stream.attach(videoCapture, false);
				}
				if (options.oformat.audio.enabled && audioCapture) {
					stream.attach(audioCapture, false);	
				}
			}

			//options.iformat.video.pixelFmt = "grey"; 
			//MotionDetector* detector = new MotionDetector();
			//stream.attach(detector, 3, true);

			// init encoder				
			encoder = new AVEncoder(options);
			encoder->initialize();
			stream.attach(encoder, 5, true);				
			
			if (options.oformat.label == "MJPEG") {
				
				// We can base64 encode MJPEG streams
				//Base64PacketEncoder* encoder = new Base64PacketEncoder();
				//stream.attach(encoder, 9, true);
				
				// HTTP streaming is also an option
				//HTTP::StreamingPacketizer* packetizer = new HTTP::StreamingPacketizer("image/jpeg");
				//stream.attach(packetizer, 10, true);

				HTTP::MultipartPacketizer* packetizer = new HTTP::MultipartPacketizer("image/jpeg", false);
				stream.attach(packetizer, 10, true);
			}
			else if (options.oformat.label == "FLV") {
				FLVMetadataInjector* injector = new FLVMetadataInjector(options.oformat);
				stream.attach(injector, 10);
			}

			//_file.open("vidtest.flv", ios::out | ios::binary);		

			// start the stream
			stream += packetDelegate(this, &MediaConnection::onVideoEncoded);
			stream.start();			
			
			_stop.wait();
			LogTrace() << "[MediaConnection] #### Stopped" << endl;
			
			stream -= packetDelegate(this, &MediaConnection::onVideoEncoded);
			stream.stop();
			
			_file.close();			
		}
		catch (Poco::Exception& exc)
		{
			Log("error") << "[MediaConnection] #### Error: " << exc.displayText() << endl;
		}

		LogTrace() << "[MediaConnection] #### Exiting" << endl;
	}

	void onVideoEncoded(void* sender, DataPacket& packet)
	{
		//LogTrace() << "[MediaConnection] Sending Packet: " << packet.size() << ": " << fpsCounter.fps << ": " << string((const char*)packet.data(), min(300, (int)packet.size())) << endl;
		fpsCounter.tick();
		
	//LogTrace() << "Sending Packet: " 
	//	<< size << ": " << string(data, min(50, packet.size())) << endl;	
		try
		{					
			//_file.write((const char*)packet.data(), packet.size());
			socket().sendBytes(packet.data(), packet.size());
		}
		catch (Poco::Exception& exc)
		{
			Log("error") << "[MediaConnection] Error: " << exc.displayText() << endl;
			_stop.set();
			//stop = true;
		}
	}
	
	RecorderOptions options;
	VideoCapture* videoCapture;
	AudioCapture* audioCapture;
	Poco::Event _stop;
	std::ofstream _file;
	AVEncoder* encoder;
	PacketStream stream;
	FPSCounter fpsCounter;
};


// ----------------------------------------------------------------------------
//
// HTTP Connection Factory
//-------
class MediaConnectionFactory: public Poco::Net::TCPServerConnectionFactory
{
public:
	TCPServerConnection* createConnection(const StreamSocket& socket) 
	{  
		LogTrace() << "[MediaConnectionFactory] Creating Connection" << endl;
	
		StreamSocket sock(socket);

		try 
		{
			// Wait until we have some data in the sock read buffer
			// then parse and redirect accordingly.
			char buffer[4096];
			sock.setReceiveTimeout(Timespan(2,0));
			int size = sock.receiveBytes(buffer, sizeof(buffer));
			string request(buffer, size);

			LogTrace() << "HTTP Connection:\n"
				<< "\n\tClient Address: " << sock.peerAddress().toString()
				<< "\n\tRequest: " << request
				<< endl;

			//
			// Flash policy-file-request
			//
			if ((request.find("policy-file-request") != string::npos) ||
				(request.find("crossdomain") != string::npos)) {
				LogTrace() << "[MediaConnectionFactory] Sending Flash Crossdomain Policy" << endl;
				return new FlashPolicyRequestHandler(sock);
			}
			
			RecorderOptions options;
			options.oformat = currentFormat;
			//options.ofile = "test1.flv";
			//options.stopAt = time(0) + 3;
									
			if (avVideoCapture) {
				InitVideoCodecFromContext(avVideoCapture->video()->ctx, options.iformat.video);
			}

			else {
				if (videoCapture && options.oformat.video.enabled)
					AllocateOpenCVInputFormat(videoCapture, options.iformat);	
			
				if (audioCapture && options.oformat.audio.enabled)
					AllocateRtAudioInputFormat(audioCapture, options.iformat);
			}
			
#if 0
			if ((request.find("mjpeg") != string::npos))
				options.oformat = MJPEG;
			
			else if ((request.find("flv") != string::npos))
				options.oformat = FLVSpeex16000; //MP344100; //FLVSpeex16000NoVideo; //FLVNoAudio; //FLVNellyMoser11025NoVideo; //FLVSpeex16000NoVideo; //FLVNoAudio; //FLVH264NoAudio; //FLVSpeex16000NoVideo; //FLVNoAudio; //FLVSpeex16000NoVideo; //FLVH264AAC; //FLVNellyMoser11025; //FLVAAC; //FLVSpeex16000; //FLVMP3; //NoVideo; //FLVNellyMoser11025; //NoVideo; //FLVMP3; //FLVSpeex16000NoVideo; //FLVSpeex16000; //FLVNoAudio; //
			
			else if ((request.find("mp3") != string::npos))
				options.oformat = MP344100;		
			
			else 
				throw Exception("No format specified");

			// Creating the video capture inside this thread is causing errors
			VideoCapture* videoCapture = NULL;
			AudioCapture* audioCapture = NULL;
			if (options.oformat.video.enabled) {
				videoCapture = MediaFactory::instance()->createVideoCapture(0);
				AllocateOpenCVInputFormat(videoCapture, options.iformat);	
			}
			if (options.oformat.audio.enabled)
				audioCapture = MediaFactory::instance()->createAudioCapture(0, 
					options.oformat.audio.channels, 
					options.oformat.audio.sampleRate);
#endif

			return new MediaConnection(socket, options, videoCapture, audioCapture);			
		}
		catch (Exception& exc)
		{
			Log("error") << exc.displayText() << endl;
		}

		return new BadRequestHandler(sock);
	}
};


// ----------------------------------------------------------------------------
//
// HTTP Service
//-------
class MediaServer: public TCPService
{
public:
	MediaServer(unsigned short port) :
		TCPService(new MediaConnectionFactory(), port)
	{
	}

	~MediaServer()
	{
	}
};


} } // namespace Sourcey::Media


int main(int argc, char** argv)
{
	Logger::instance().add(new ConsoleChannel("debug", TraceLevel));
				
	MediaFactory::initialize();

	//avVideoCapture = new AVInputReader();
	//avVideoCapture->openDevice(0);
	//avVideoCapture->openFile("big_buck_bunny.mp4");
	//avVideoCapture->start();
	
	if (!avVideoCapture) {
		//MediaFactory::instance()->loadVideo();	
		currentFormat.video.quality = 100;
		if (currentFormat.video.enabled) {
			videoCapture = MediaFactory::instance()->getVideoCapture(0);
		}
		if (currentFormat.audio.enabled) {
			audioCapture = MediaFactory::instance()->createAudioCapture(0, 
				currentFormat.audio.channels, 
				currentFormat.audio.sampleRate); 
		}
	}
	
	MediaServer srv(328);
	srv.start();
	
	char o = 0;
	while (o != 'Q') {	
		cout << 
			"COMMANDS:\n\n"
			"  Q	Quit.\n\n";
		
		o = toupper(getch());
	}
	return 0;
}