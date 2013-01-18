#include "Sourcey/Logger.h"
#include "Sourcey/Runner.h"
#include "Sourcey/PacketStream.h"
#include "Sourcey/Util.h"
#include "Sourcey/Media/MediaFactory.h"
#include "Sourcey/Media/MotionDetector.h"
#include "Sourcey/Media/AVEncoder.h"
#include "Sourcey/Media/FLVMetadataInjector.h"
#include "Sourcey/Net/TCPService.h"
#include "Sourcey/HTTP/MultipartPacketizer.h"


/*
#include "Sourcey/Media/AudioCapture.h"
#include "Sourcey/Net/Util.h"
#include "Sourcey/HTTP/Util.h"
#include "Sourcey/HTTP/MultipartPacketizer.h"
#include "Sourcey/Media/ImageEncoder.h"
//using namespace Sourcey::HTTP;
//VideoCapture* videoCapture;
//AudioCapture* audioCapture;
//bool stop = false;

#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/SocketAcceptor.h"
#include "Poco/Net/TCPServerConnection.h"
#include "Poco/Net/TCPServerConnectionFactory.h"
#include "Poco/Net/TCPServerParams.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/format.h"
#include <string>
#include <vector>
#include <assert.h>

#include <conio.h>
*/



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
//
// ----------------------------------------------------------------------------		
Format MP344100 = Format("MP3", Format::MP3, 
	VideoCodec(),
	AudioCodec("MP3", "libmp3lame", 2, 44100, 128000, (UInt32)SampleFormat::S16P));	
	// Adobe Flash Player requires that audio files be 16bit and have a sample rate of 44.1khz.
	// Flash Player can handle MP3 files encoded at 32kbps, 48kbps, 56kbps, 64kbps, 128kbps, 160kbps or 256kbps.
	// NOTE: 128000 works fine for 44100, but 64000 is broken!
			
Format MP38000  = Format("MP3", Format::MP3, 
	VideoCodec(),
	AudioCodec("MP3", "libmp3lame", 1, 8000, 64000)); //)
			
Format MP311000  = Format("MP3", Format::MP3, 
	VideoCodec(),
	AudioCodec("MP3", "libmp3lame", 1, 11000, 16000));

//16 kbps, 11 khz, Mono, MP3

Format MP348000 = Format("MP3", Format::MP3, 
	VideoCodec(),
	AudioCodec("MP3", "libmp3lame", 2, 48000, 128000, (UInt32)SampleFormat::S16P));	//, (UInt32)SampleFormat::S16P)
 						
Format FLVNoAudio = Format("FLV", Format::FLV, 
	VideoCodec("FLV", "flv", 320, 240));	//, 6, 9000, 64000

Format FLVSpeex16000 = Format("FLV", Format::FLV, 
	VideoCodec("FLV", "flv", 320, 240),
	AudioCodec("Speex", "libspeex", 1, 16000));	

Format FLVSpeex16000NoVideo = Format("FLV", Format::FLV, 
	VideoCodec(),
	AudioCodec("Speex", "libspeex", 1, 16000));	//, 16800


Format MJPEG = Format("MJPEG", Format::MJPEG, 
	VideoCodec("MJPEG", "mjpeg", 640, 480, 25));

/*
Format FLVNellyMoser11025 = Format("FLV", Format::FLV, 
	VideoCodec("FLV", "flv", 320, 240, 20),
	AudioCodec("Nellymoser", "nellymoser", 1, 11025));	
			
Format FLVNellyMoser11025NoVideo = Format("FLV", Format::FLV, 
	VideoCodec(),
	AudioCodec("Nellymoser", "nellymoser", 1, 11025, 64000, (UInt32)SampleFormat::FLT));	//
	// Supports float only
			
Format FLVH264NoAudio = Format("FLVH264", Format::FLV, 
	VideoCodec("H264", "libx264", 320, 240, 30));	
			
//Format FLVSpeex8000NoVideo = Format("FLV", Format::FLV, 
//	VideoCodec(),
//	AudioCodec("Speex", "libspeex", 1, 8000));	//, 16800
			
Format FLVMP3 = Format("FLV", Format::FLV, 
	VideoCodec("FLV", "flv", 320, 240, 20),
	AudioCodec("MP3", "libmp3lame", 2, 44100, 64000, (UInt32)SampleFormat::S16P));	

Format AAC44100 = Format("MP4", Format::MP4,
	//VideoCodec("MPEG4", "mpeg4", 640, 480, 25), 
	//VideoCodec("H264", "libx264", 320, 240, 25),
	AudioCodec("AAC", "aac", 2, 44100));
			
Format FLVAAC = Format("FLV", Format::FLV, 
	VideoCodec("FLV", "flv", 320, 240, 25),
	AudioCodec("AAC", "aac", 2, 44100));	

Format FLVH264AAC = Format("FLV", Format::FLV,
	//VideoCodec("MPEG4", "mpeg4", 640, 480, 25), 
	VideoCodec("H264", "libx264", 320, 240, 25),
	AudioCodec("AAC", "aac", 2, 44100));

Format MPEG4AAC = Format("FLV", Format::FLV,
	VideoCodec("MPEG4", "mpeg4", 640, 480, 25),
	AudioCodec("AAC", "aac", 2, 44100));

Format H264AAC = Format("H264AAC", Format::FLV,
	VideoCodec("H264", "libx264", 640, 480, 25),
	AudioCodec("AAC", "aac", 2, 44100));

Format MP4 = Format("MP4", Format::MP4,
	VideoCodec("MPEG4", "mpeg4", 320, 240, 25),
	AudioCodec("AC3", "ac3_fixed", 2, 44100));
	*/


// Global for now
Format currentFormat = MJPEG; //FLVSpeex16000; //FLVSpeex16000NoVideo; //MJPEG; //H264AAC; //MP38000; //MP38000; //MP38000; //MP344100; //MP344100; //AAC44100; //FLVNoAudio; //FLVNellyMoser11025NoVideo; //FLVSpeex16000NoVideo; //FLVNellyMoser11025NoVideo; //FLVH264NoAudio; //MP344100; //FLVNellyMoser11025NoVideo; //FLVH264NoAudio; //FLVNellyMoser11025NoVideo; //FLVNellyMoser11025NoVideo; //FLVNellyMoser11025NoVideo; FLVNellyMoser11025NoVideo; //MP344100; 
VideoCapture* videoCapture = NULL;
AudioCapture* audioCapture = NULL;


// ----------------------------------------------------------------------------
//
// Media Connection
//
// ----------------------------------------------------------------------------
class MediaConnection: public TCPServerConnection
{
public:
	MediaConnection(const StreamSocket& sock, const RecorderOptions& options, 
		VideoCapture* videoCapture = NULL, AudioCapture* audioCapture = NULL) : 
		TCPServerConnection(sock), options(options), encoder(NULL), 
		videoCapture(videoCapture), audioCapture(audioCapture)
	{		
		Log("trace") << "[MediaConnection] Creating" << endl;
	}

	~MediaConnection() 
	{
		Log("trace") << "[MediaConnection] Destroying" << endl;
	}
		
	void run()
	{
		try
		{
			Log("trace") << "[MediaConnection] Running" << endl;
						
			//options.oformat.video.enabled = false;
			if (options.oformat.video.enabled && videoCapture)
				stream.attach(videoCapture, false);
			//options.oformat.audio.enabled = false;
			if (options.oformat.audio.enabled && audioCapture)
				stream.attach(audioCapture, false);	
				//stream.attach(audioCapture, true);			

			//options.iformat.video.pixfmt = (Sourcey::::PixelFormat)PIX_FMT_GRAY8; 
			//MotionDetector* detector = new MotionDetector();
			//stream.attach(detector, 3, true);			
		
			//options.oformat.video.bitRate = 10000000;

			// init encoder				
			encoder = new AVEncoder(options);
			encoder->initialize();
			stream.attach(encoder, 5, true);				
			
			/*
			if (options.oformat.label == "MJPEG") {
				HTTP::MultipartPacketizer* packetizer = new HTTP::MultipartPacketizer("image/jpeg", false);
				stream.attach(packetizer, 10, true);
			}
			else if (options.oformat.label == "FLV") {
				FLVMetadataInjector* injector = new FLVMetadataInjector(options.oformat);
				stream.attach(injector, 10);
			}
			*/
			
			_file.open("vidtest.flv", ios::out | ios::binary);	

			// start the stream
			stream += packetDelegate(this, &MediaConnection::onVideoEncoded);
			stream.start();
			
			
			_stop.wait();
			Log("trace") << "[MediaConnection] #### Stopped" << endl;
			
			stream -= packetDelegate(this, &MediaConnection::onVideoEncoded);
			stream.stop();

			
			_file.close();
			
		}
		catch (Poco::Exception& exc)
		{
			Log("error") << "[MediaConnection] #### Error: " << exc.displayText() << endl;
		}

		Log("trace") << "[MediaConnection] #### Exiting" << endl;
	}

	void onVideoEncoded(void* sender, DataPacket& packet)
	{
		Log("trace") << "[MediaConnection] Sending Packet: " << packet.size() << ": " << fpsCounter.fps << endl;
		fpsCounter.tick();

		try
		{					
			_file.write((const char*)packet.data(), packet.size());
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
//
// ----------------------------------------------------------------------------
class MediaConnectionFactory: public Poco::Net::TCPServerConnectionFactory
{
public:
	TCPServerConnection* createConnection(const StreamSocket& socket) 
	{  
		Log("trace") << "[MediaConnectionFactory] Creating Connection" << endl;
	
		StreamSocket sock(socket);

		try 
		{
			// Wait until we have some data in the sock read buffer
			// then parse and redirect accordingly.
			char buffer[4096];
			sock.setReceiveTimeout(Timespan(2,0));
			int size = sock.receiveBytes(buffer, sizeof(buffer));
			string request(buffer, size);

			Log("trace") << "HTTP Connection:\n"
				<< "\n\tClient Address: " << sock.peerAddress().toString()
				<< "\n\tRequest: " << request
				<< endl;

			//
			// Flash policy-file-request
			//
			if ((request.find("policy-file-request") != string::npos) ||
				(request.find("crossdomain") != string::npos)) {
				Log("trace") << "[MediaConnectionFactory] Sending Flash Crossdomain Policy" << endl;
				return new FlashPolicyRequestHandler(sock);
			}
			
			RecorderOptions options;
			options.oformat = currentFormat;
			//options.ofile = "test1.flv";
			//options.stopAt = time(0) + 3;

			/*
			if ((request.find("mjpeg") != string::npos))
				options.oformat = MJPEG;
			
			else if ((request.find("flv") != string::npos))
				options.oformat = FLVNellyMoser11025NoVideo; //MP344100; //FLVSpeex16000NoVideo; //FLVNoAudio; //FLVNellyMoser11025NoVideo; //FLVSpeex16000NoVideo; //FLVNoAudio; //FLVH264NoAudio; //FLVSpeex16000NoVideo; //FLVNoAudio; //FLVSpeex16000NoVideo; //FLVH264AAC; //FLVNellyMoser11025; //FLVAAC; //FLVSpeex16000; //FLVMP3; //NoVideo; //FLVNellyMoser11025; //NoVideo; //FLVMP3; //FLVSpeex16000NoVideo; //FLVSpeex16000; //FLVNoAudio; //
			
			else if ((request.find("mp3") != string::npos))
				options.oformat = MP344100;		
			
			else 
				throw Exception("No format specified");
				*/
			
			cout << "HEREEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE: " << options.oformat.toString() << endl;
			
			if (options.oformat.video.enabled)
				AllocateOpenCVInputFormat(videoCapture, options.iformat);	
			
			if (options.oformat.audio.enabled)
				AllocateRtAudioInputFormat(audioCapture, options.iformat);

			/*
			VideoCapture* videoCapture = NULL;
			AudioCapture* audioCapture = NULL;
			if (options.oformat.video.enabled) {
				videoCapture = MediaFactory::instance()->video.getCapture(0);
				AllocateOpenCVInputFormat(videoCapture, options.iformat);	
			}
			if (options.oformat.audio.enabled)
				audioCapture = MediaFactory::instance()->audio.getCapture(0, 
					options.oformat.audio.channels, 
					options.oformat.audio.sampleRate);
					*/

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
//
// ----------------------------------------------------------------------------
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



// ----------------------------------------------------------------------------
//
// Stream Test
//
// ----------------------------------------------------------------------------

class StreamTest
{
public:
	StreamTest() : 
		waitForEnd(false), encoder(NULL) //stop(false), 
	{		
	}

	~StreamTest() 
	{
		Log("trace") << "[StreamTest] Destroying" << endl;
		//stream.stop();
		//delete encoder;
	}
			
	void run()
	{
		try
		{
			Log("trace") << "[StreamTest] Running" << endl;			

			RecorderOptions options;
			options.oformat = currentFormat;
			options.ofile = "enctest4.mp4";
			//options.stopAt = time(0) + 3;
			//AllocateOpenCVInputFormat(videoCapture, options.iformat);	
			//options.oformat = MP344100; //MP4; //FLVSpeex16000; //MP344100; //FLVMP3; //
			
			//_file.open("enctest1.mp3", ios::out | ios::binary);	
			

			/*			


			Log("trace") << "[StreamTest] Running: " << options.oformat.audio.sampleFmt << endl;	
			Log("trace") << "[StreamTest] Running: " << AV_SAMPLE_FMT_S16P << endl;	
	
			assert((AVSampleFormat)options.oformat.audio.sampleFmt == AV_SAMPLE_FMT_S16P);

			options.oformat = Format("FLV", Format::FLV, 
				VideoCodec("FLV", "flv", 320, 240, 20),
				//AudioCodec("Nellymoser", "nellymoser", 1, 11025)
				//AudioCodec("Speex", "libspeex", 1, 16000)//,
				AudioCodec("MP3", "libmp3lame", 2, 44100)
				);
				audioCapture = MediaFactory::instance()->audio.getCapture(0, 1, 16000);
			VideoCapture* videoCapture = NULL;
			AudioCapture* audioCapture = NULL;
				*/
			

			if (options.oformat.video.enabled) {
				
				Log("trace") << "[StreamTest] Opening Video Capture" << endl;	

				//videoCapture = MediaFactory::instance()->video.getCapture(0);
				//AllocateOpenCVInputFormat(videoCapture, options.iformat);	
				AllocateOpenCVInputFormat(videoCapture, options.iformat);
				assert(options.iformat.video.width);
				stream.attach(videoCapture, false);
			}
			//options.oformat.audio.enabled = false;
			if (options.oformat.audio.enabled) {
				Log("trace") << "[StreamTest] Opening Video Capture" << endl;	

				//audioCapture = MediaFactory::instance()->audio.getCapture(0, 
				//	options.oformat.audio.channels, 
				//	options.oformat.audio.sampleRate);
				AllocateRtAudioInputFormat(audioCapture, options.iformat);
				stream.attach(audioCapture, true);
			}

			/*
			options.oformat = Format("FLV", Format::FLV,
				//VideoCodec("MPEG4", "mpeg4", 640, 480, 25), 
				//VideoCodec("H264", "libx264", 640, 480, 25), 
				VideoCodec("FLV", "flv", 640, 480, 25), 
				//AudioCodec("Nellymoser", "nellymoser", 1, 11025)
				//AudioCodec("Speex", "libspeex", 2, 44100)
				//AudioCodec("MP3", "libmp3lame", 2, 44100)
				//AudioCodec("AAC", "aac", 2, 44100)
				AudioCodec("AAC", "aac", 1, 11025)
			);
			options.oformat = Format("MJPEG", Format::MJPEG, VideoCodec("MJPEG", "mjpeg", 640, 480, 25));
			//options.oformat = Format("FLV", Format::FLV, VideoCodec("H264", "libx264", 400, 300, 25));	
			//options.oformat = Format("FLV", Format::FLV, VideoCodec("FLV", "flv", 640, 480, 100));	
			//options.oformat = Format("FLV", Format::FLV, VideoCodec("FLV", "flv", 320, 240, 15));	
			//options.oformat = Format("FLV", Format::FLV, VideoCodec("H264", "libx264", 400, 300, 25));	
			//options.oformat = Format("MP4", Format::MP4, VideoCodec("H264", "libx264", 400, 300, 25));
			*/
			
			//videoCapture += videoDelegate(this, &StreamTest::onVideoEncoded);

			//options.iformat.video.pixfmt = (Sourcey::::PixelFormat)PIX_FMT_GRAY8; //PIX_FMT_BGR8; //PIX_FMT_BGR32 // PIX_FMT_BGR32
			//MotionDetector* detector = new MotionDetector();
			//detector->setVideoCapture(videoCapture);
			//stream.attach(detector, true);		
			
				
			//vector<int> cvoptions = vector<int>(2);
			//cvoptions[0] = CV_IMWRITE_JPEG_QUALITY;
			//cvoptions[1] = 95;	// default 95 [0-100]
			//encoder = new ImageEncoder(session->options(), options);

			//stream.attach(new ImageEncoder(options, cvoptions), 20, true);	
			//stream.attach(videoCapture, false);
			
			//stream += packetDelegate(this, &StreamTest::onVideoPacket);
			//stream -= packetDelegate(this, &StreamTest::onVideoPacket);
			//stream += videoDelegate(this, &StreamTest::onVideoPacket);
			//stream -= videoDelegate(this, &StreamTest::onVideoPacket);
		
			// init encoder				
			encoder = new AVEncoder(options);
			encoder->initialize();
			stream.attach(encoder, 5, true);				
				
			//HTTP::MultipartPacketizer* packetizer = new HTTP::MultipartPacketizer("image/jpeg");
			//stream.attach(packetizer, 10);

			//FLVMetadataInjector* injector = new FLVMetadataInjector(options.oformat);
			//stream.attach(injector);
			
			// attach delegates
			stream += packetDelegate(this, &StreamTest::onVideoEncoded);

			// start the stream
			stream.start();
				
			Util::pause();
	
			_file.close();
			//while (!stop)
			//{
			//	Thread::sleep(50);
			//}
				
			//stream -= packetDelegate(this, &StreamTest::onVideoEncoded);
			
			Log("trace") << "[MediaConnection] ENDING!!!!!!!!!!!!!" << endl;
		}
		catch (Poco::Exception& exc)
		{
			Log("trace") << "[MediaConnection] ERROR: " << exc.displayText() << endl;
		}
		Log("trace") << "[MediaConnection] Ending ###############################" << endl;
	}

	void onVideoEncoded(void* sender, DataPacket& packet)
	{
		Log("trace") << "[MediaConnection] Video Encoded" << endl;

		fpsCounter.tick();
		
		_file.write((const char*)packet.data(), packet.size());		
		/*
		//Log("trace") << "[MediaConnection] Sending Packet: " << packet.size << ": " << fpsCounter.fps << endl;
		//Log("trace") << "[MediaConnection] Sending Packet: " << string((const char*)packet.data, 50) << endl;
		
		if (fpsCounter.frames == 50) {
			//stream.detach(videoCapture);
			//waitForEnd = true;
			//stop = true;
			//stream.start();
			stream.stop();
			//encoder->uninitialize();
		}
		
		StreamSocket& ss = socket();
		try
		{		
			ss.sendBytes(packet.data, packet.size);
		}
		catch (Poco::Exception& exc)
		{
			Log("trace") << "[MediaConnection] " << exc.displayText() << endl;
			stop = true;
		}
		*/

		//if (waitForEnd)
		//	stop = true;
	}
		
	bool waitForEnd;
	AVEncoder* encoder;
	PacketStream stream;
	FPSCounter fpsCounter;
	std::ofstream _file;	
};


} } // namespace Sourcey::Media



int main(int argc, char** argv)
{
	Logger::instance().add(new ConsoleChannel("debug", TraceLevel));
	
	Log("trace") << "HTTP Connection:\n"
		<< "\n\CODEC_ID_MP2: " << CODEC_ID_MP2
		<< "\n\CODEC_ID_MP3: " << CODEC_ID_MP3
		<< "\n\CODEC_ID_AAC: " << CODEC_ID_AAC
		<< "\n\CODEC_ID_AC3: " << CODEC_ID_AC3
		<< "\n\CODEC_ID_FLAC: " << CODEC_ID_FLAC
		<< "\n\CODEC_ID_VORBIS: " << CODEC_ID_VORBIS
		<< "\n\CODEC_ID_NELLYMOSER: " << CODEC_ID_NELLYMOSER
		<< "\n\CODEC_ID_SPEEX: " << CODEC_ID_SPEEX
		<< "\n\CODEC_ID_H263: " << CODEC_ID_H263
		<< "\n\CODEC_ID_H263P: " << CODEC_ID_H263P
		<< "\n\CODEC_ID_H264: " << CODEC_ID_H264
		//<< "\n\CODEC_ID_MPEG: " << CODEC_ID_MPEG
		//<< "\n\CODEC_ID_MPEG2: " << CODEC_ID_MPEG2
		<< "\n\CODEC_ID_MPEG4: " << CODEC_ID_MPEG4
		<< "\n\CODEC_ID_MJPEG: " << CODEC_ID_MJPEG
		<< "\n\CODEC_ID_FLV1: " << CODEC_ID_FLV1
		<< endl;
	
	/*
	
	MP2			= 86016,
	MP3			= 86017,
	AAC			= 86018,
	AC3			= 86019,
	Vorbis		= 86021,
	FLAC		= 86030,

	Raw			= 14,
	H263		= 5,
	H263p		= 20,
	H264		= 28,
	MPEG1		= 1,
	MPEG2		= 2,
	MPEG4		= 13,
	MJPEG		= 8,
	FLV			= 22,
		
	// audio
	PCM			= 65536,
	MP2			= 86016,
	MP3			= 86017,
	AAC			= 86018,
	AC3			= 86019,
	Vorbis		= 86021,
	FLAC		= 86030,
	NellyMoser	= 86052,
	Speex		= 86054

	videoCapture = NULL;
	audioCapture = NULL;	
	
static const RtAudioFormat RTAUDIO_SINT8 = 0x1;    // 8-bit signed integer.
static const RtAudioFormat RTAUDIO_SINT16 = 0x2;   // 16-bit signed integer.
static const RtAudioFormat RTAUDIO_SINT24 = 0x4;   // Lower 3 bytes of 32-bit signed integer.
static const RtAudioFormat RTAUDIO_SINT32 = 0x8;   // 32-bit signed integer.
static const RtAudioFormat RTAUDIO_FLOAT32 = 0x10; // Normalized between plus/minus 1.0.
static const RtAudioFormat RTAUDIO_FLOAT64 = 0x20; // Normalized between plus/minus 1.0.

typedef char AUDIO_DATA;
#define AUDIO_FORMAT RTAUDIO_SINT8

typedef signed short AUDIO_DATA;
#define AUDIO_FORMAT RTAUDIO_SINT16

typedef signed long AUDIO_DATA;
#define AUDIO_FORMAT RTAUDIO_SINT24

typedef signed long AUDIO_DATA;
#define AUDIO_FORMAT RTAUDIO_SINT32

typedef float AUDIO_DATA;
#define AUDIO_FORMAT RTAUDIO_FLOAT32
			Log("trace") << "[MediaConnection] ENDING!!!!!!!!!!!!!" << sizeof(char) << endl;
			Log("trace") << "[MediaConnection] ENDING!!!!!!!!!!!!!" << sizeof(signed short) << endl;
			Log("trace") << "[MediaConnection] ENDING!!!!!!!!!!!!!" << sizeof(signed long) << endl;
			Log("trace") << "[MediaConnection] ENDING!!!!!!!!!!!!!" << sizeof(signed long) << endl;
			Log("trace") << "[MediaConnection] ENDING!!!!!!!!!!!!!" << sizeof(float) << endl;
			Log("trace") << "[MediaConnection] ENDING!!!!!!!!!!!!!" << sizeof(double) << endl;
	
	assert(sizeof(signed short) > 20);
	*/			
	

	MediaFactory::initialize();
	MediaFactory::instance()->loadVideo();
	MediaFactory::instance()->loadAudio();
	
	videoCapture = MediaFactory::instance()->video.getCapture(0);
	audioCapture = MediaFactory::instance()->audio.getCapture(0, 
		currentFormat.audio.channels, 
		currentFormat.audio.sampleRate); //, RTAUDIO_FLOAT32
	//audioCapture = MediaFactory::instance()->audio.getCapture(0, 1, 11025, RTAUDIO_FLOAT32);
	//audioCapture = MediaFactory::instance()->audio.getCapture(0, 2, 44100);
	//audioCapture = MediaFactory::instance()->audio.getCapture(0, 1, 16000);
	//audioCapture->start();
	//, RTAUDIO_FLOAT32, RTAUDIO_FLOAT64
	
	/*
	{
		StreamTest test;
		test.run();
		Util::pause();
	}	
	*/
	{
		MediaServer srv(328);
		srv.start();
		Util::pause();
	}

	MediaFactory::uninitialize();

	/*	
	MediaServer srv(328);
	srv.start();
	Util::pause();
	{
		StreamTest test;
		test.run();
		Util::pause();
	}
	{
		StreamTest test1;
		test1.run();
		Util::pause();
	}
	{
		StreamTest test2;
		test2.run();
		Util::pause();
	}


	ServerSocket svs(328);
	TCPServer srv(new TCPServerConnectionFactoryImpl<MediaConnection>(), svs);
	srv.start();
	Util::pause();
	stop = true;
	Util::pause();


	srv.stop();
	api.setCredentials(Anionu_API_USERNAME, Anionu_API_PASSWORD);
	//api.loadServices();

	string service("GetAccount");
	
	char o = 0;
	while (o != 'Q') 
	{	
		cout << 
			"COMMANDS:\n\n"
			"  A	Set Target Service.\n"
			"  C	Call Basic Service.\n"
			"  U	Call Upload Asset Service.\n"
			"  L	List Services.\n"
			//"  K	List Remote Packages.\n"
			//"  R	Reload Service List.\n"
			//"  I	Install Packages.\n"
			//"  U	Uninstall Packages.\n"
			//"  D	Update All Packages.\n"
			"  Q	Quit.\n\n";
		
		o = toupper(getch());
		
		// Set Target Service
		if (o == 'A') {	
			cout << "Enter target service name: " << endl;
			getline(cin, service);
		}

		// Call Basic Service
		else if (o == 'C') {
			assert(!service.empty());
			APITransaction* transaction = api.call(service);
			transaction->APITransactionComplete += delegate(&api, &VideoSocket::onAPITransactionComplete);
			transaction->start();	
		} 

		// Call Upload Asset Service
		else if (o == 'U') {

			Path file("D:/test.avi");
			
			HTMLForm* form = new HTMLForm(HTMLForm::ENCODING_MULTIPART);
			form->set("asset[type]", "Video");
			form->addPart("asset[file]", new FilePartSource(file.toString()));

			APITransaction* transaction = api.upload("UploadAsset", form); //file.toString(), 
			transaction->APITransactionComplete += delegate(&api, &VideoSocket::onAPITransactionComplete);
			transaction->start();
		} 

		// List Services
		else if (o == 'L') {
			api.services().print(cout);
		} 

		// Reload Service List
		else if (o == 'R') {
			api.loadServices();
		} 
	}

	Util::pause();

	*/
	return 0;
}





/*
		*/
