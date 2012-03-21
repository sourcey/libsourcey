#include "Sourcey/Runner.h"
#include "Sourcey/PacketStream.h"
#include "Sourcey/Util.h"
#include "Sourcey/Net/TCPService.h"
#include "Sourcey/Media/CaptureFactory.h"
#include "Sourcey/Media/AudioCapture.h"
#include "Sourcey/Media/AVEncoder.h"
#include "Sourcey/Media/FLVMetadataInjector.h"

//#include "Sourcey/HTTP/MultipartPacketizer.h"


/*
#include "Sourcey/Net/Util.h"
#include "Sourcey/HTTP/Util.h"
*/
#include "Sourcey/HTTP/MultipartPacketizer.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Media/ImageEncoder.h"

#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/TCPServer.h"
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


using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace Sourcey;
using namespace Sourcey::Net;
using namespace Sourcey::Media;
using namespace Sourcey::Util;
//using namespace Sourcey::HTTP;


// Detect Memory Leaks
/*
#ifdef _DEBUG
#include "MemLeakDetect/MemLeakDetect.h"
CMemLeakDetect memLeakDetect;
#endif<DataPacket>
*/

VideoCapture* videoCapture;
AudioCapture* audioCapture;
//bool stop = false;


namespace Sourcey { 
namespace Media {	


// ----------------------------------------------------------------------------
//
// Media Connection
//
// ----------------------------------------------------------------------------
class MediaConnection: public TCPServerConnection
{
public:
	MediaConnection(const StreamSocket& sock, const Media::RecorderParams& params) : 
		TCPServerConnection(sock), params(params), encoder(NULL)
	{		
		Log("trace") << "[MediaConnection] Creating" << std::endl;
		//stop = false;
	}

	~MediaConnection() 
	{
		Log("trace") << "[MediaConnection] Destroying" << std::endl;
	}
		
	void run()
	{
		try
		{
			Log("trace") << "[MediaConnection] Running" << std::endl;				

			//params.iformat.video.pixfmt = (Sourcey::Media::PixelFormat::ID)PIX_FMT_GRAY8; //PIX_FMT_BGR8; //PIX_FMT_BGR32 // PIX_FMT_BGR32
			//MotionDetector* detector = new MotionDetector();
			//detector->setVideoCapture(videoCapture);
			//stream.attach(detector, true);		
			//stream.attach(new SurveillanceMJPEGPacketizer(*detector), 20, true);	
			
			//stream.attach(videoCapture, false);
			stream.attach(videoCapture, false);
		
			// init encoder				
			encoder = new AVEncoder();
			encoder->setParams(params);
			encoder->initialize();
			stream.attach(encoder, 5, true);				
				
			if (params.oformat.label == "MJPEG") {
				HTTP::MultipartPacketizer* packetizer = new HTTP::MultipartPacketizer("image/jpeg");
				stream.attach(packetizer, 10);
			}
			else if (params.oformat.label == "FLV") {
				FLVMetadataInjector* injector = new FLVMetadataInjector(params.oformat);
				stream.attach(injector, 10);
			}

			// start the stream
			stream += packetDelegate(this, &MediaConnection::onVideoEncoded);
			stream.start();
			
			_stop.wait();
			//while (!stop)
			//{
			//	Thread::sleep(50);
			//}
			Log("trace") << "[MediaConnection] #### Stopped" << std::endl;
			
			stream -= packetDelegate(this, &MediaConnection::onVideoEncoded);
			stream.stop();
			
		}
		catch (Poco::Exception& exc)
		{
			Log("error") << "[MediaConnection] #### Error: " << exc.displayText() << std::endl;
		}

		Log("trace") << "[MediaConnection] #### Exiting" << std::endl;
	}

	void onVideoEncoded(void* sender, DataPacket& packet)
	{
		Log("trace") << "[MediaConnection] Sending Packet: " << packet.size() << ": " << fpsCounter.fps << std::endl;
		fpsCounter.tick();

		try
		{		
			socket().sendBytes(packet.data(), packet.size());
		}
		catch (Poco::Exception& exc)
		{
			Log("error") << "[MediaConnection] Error: " << exc.displayText() << std::endl;
			_stop.set();
			//stop = true;
		}
	}
	
	Media::RecorderParams params;
	Poco::Event _stop;
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
	TCPServerConnection* MediaConnectionFactory::createConnection(const StreamSocket& socket) 
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
				Log("trace") << "@@@@@@@@@@@@@@@@@@@@@@@ [MediaConnectionFactory] Sending Flash Policy File Request" << endl;
				return new FlashPolicyRequestHandler(sock);		
			}
			
			Media::RecorderParams params;
			//params.ofile = "enctest.mp4";
			//params.stopAt = time(0) + 3;
			AllocateOpenCVInputFormat(videoCapture, params.iformat);	

			if ((request.find("mjpeg") != string::npos))
				params.oformat = Format("MJPEG", Format::MJPEG, VideoCodec(Codec::MJPEG, "MJPEG", 640, 480, 25));
			
			else if ((request.find("flv") != string::npos))
				params.oformat = Format("FLV", Format::FLV, VideoCodec(Codec::FLV, "FLV", 640, 480, 100));
			
			else 
				throw Exception("No format specified");

			//params.oformat = Format("FLV", Format::FLV, VideoCodec(Codec::H264, "H264", 400, 300, 25));	
			//params.oformat = Format("FLV", Format::FLV, VideoCodec(Codec::FLV, "FLV", 640, 480, 100));	
			//params.oformat = Format("FLV", Format::FLV, VideoCodec(Codec::FLV, "FLV", 320, 240, 15));	
			//params.oformat = Format("FLV", Format::FLV, VideoCodec(Codec::H264, "H264", 400, 300, 25));	
			//params.oformat = Format("MP4", Format::MP4, VideoCodec(Codec::H264, "H264", 400, 300, 25));	

			return new MediaConnection(socket, params);
			
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
	MediaServer::MediaServer(unsigned short port) :
		TCPService(new MediaConnectionFactory(), port)
	{
	}

	MediaServer::~MediaServer()
	{
	}
};



} } // namespace Sourcey::Media



int main(int argc, char** argv)
{
	Logger::instance().add(new ConsoleChannel("verbose", TraceLevel));

	videoCapture = NULL;
	audioCapture = NULL;

	try
	{
		videoCapture = new VideoCapture(0, true);
		//audioCapture = new AudioCapture(0, 2, 44100);
		//audioCapture = new AudioCapture(0, 1, 16000);	
		audioCapture = new AudioCapture(0, 1, 11025);			
	}
	catch (Poco::Exception& exc)
	{
		Log("trace") << "Media Tests: FAILED: " << exc.displayText() << std::endl;
	}	
	
	MediaServer srv(328);
	srv.start();
		system("pause");

	/*
	{
		StreamTest test;
		test.run();
		system("pause");
	}
	{
		StreamTest test1;
		test1.run();
		system("pause");
	}
	{
		StreamTest test2;
		test2.run();
		system("pause");
	}


	ServerSocket svs(328);
	TCPServer srv(new TCPServerConnectionFactoryImpl<MediaConnection>(), svs);
	srv.start();
	system("pause");
	stop = true;
	system("pause");


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
	*/

	system("pause");

	return 0;
}





/*
class StreamTest
{
public:
	StreamTest() : 
		waitForEnd(false), encoder(NULL) //stop(false), 
	{		
	}

	~StreamTest() 
	{
		Log("trace") << "[StreamTest] DESTROYING!!!!!!!!!!!!!" << std::endl;
		stream.stop();
		//delete encoder;
	}

		
	void run()
	{
		try
		{
			Log("trace") << "[StreamTest] Running" << std::endl;

			Media::RecorderParams params;
			//params.ofile = "enctest.mp4";
			//params.stopAt = time(0) + 3;
			AllocateOpenCVInputFormat(videoCapture, params.iformat);	
			
			params.oformat = Format("FLV", Format::FLV, 
				VideoCodec(Codec::FLV, "FLV", 320, 240, 15),
				//AudioCodec(Codec::NellyMoser, "NellyMoser", 1, 11025),
				AudioCodec(Codec::Speex, "Speex", 1, 16000)//,
				//AudioCodec(Codec::Speex, "Speex", 2, 44100)
				);	
			params.oformat = Format("FLV", Format::FLV,
				//VideoCodec(Codec::MPEG4, "MPEG4", 640, 480, 25), 
				//VideoCodec(Codec::H264, "H264", 640, 480, 25), 
				VideoCodec(Codec::FLV, "FLV", 640, 480, 25), 
				//AudioCodec(Codec::NellyMoser, "NellyMoser", 1, 11025)
				//AudioCodec(Codec::Speex, "Speex", 2, 44100)
				//AudioCodec(Codec::MP3, "MP3", 2, 44100)
				//AudioCodec(Codec::AAC, "AAC", 2, 44100)
				AudioCodec(Codec::AAC, "AAC", 1, 11025)
			);
			params.oformat = Format("MJPEG", Format::MJPEG, VideoCodec(Codec::MJPEG, "MJPEG", 640, 480, 25));
			//params.oformat = Format("FLV", Format::FLV, VideoCodec(Codec::H264, "H264", 400, 300, 25));	
			//params.oformat = Format("FLV", Format::FLV, VideoCodec(Codec::FLV, "FLV", 640, 480, 100));	
			//params.oformat = Format("FLV", Format::FLV, VideoCodec(Codec::FLV, "FLV", 320, 240, 15));	
			//params.oformat = Format("FLV", Format::FLV, VideoCodec(Codec::H264, "H264", 400, 300, 25));	
			//params.oformat = Format("MP4", Format::MP4, VideoCodec(Codec::H264, "H264", 400, 300, 25));					
			
			//videoCapture += videoDelegate(this, &StreamTest::onVideoEncoded);

			//params.iformat.video.pixfmt = (Sourcey::Media::PixelFormat::ID)PIX_FMT_GRAY8; //PIX_FMT_BGR8; //PIX_FMT_BGR32 // PIX_FMT_BGR32
			//MotionDetector* detector = new MotionDetector();
			//detector->setVideoCapture(videoCapture);
			//stream.attach(detector, true);		
			
			stream.attach(audioCapture, false);
			stream.attach(videoCapture, false);
				
			vector<int> cvparams = vector<int>(2);
			cvparams[0] = CV_IMWRITE_JPEG_QUALITY;
			cvparams[1] = 95;	// default 95 [0-100]
			//encoder = new ImageEncoder(session->params(), params);

			stream.attach(new ImageEncoder(params, cvparams), 20, true);	
			//stream.attach(videoCapture, false);
			
			//stream += packetDelegate(this, &StreamTest::onVideoPacket);
			//stream -= packetDelegate(this, &StreamTest::onVideoPacket);
			//stream += videoDelegate(this, &StreamTest::onVideoPacket);
			//stream -= videoDelegate(this, &StreamTest::onVideoPacket);
		
			// init encoder				
			encoder = new AVEncoder();
			encoder->setParams(params);
			encoder->initialize();
			stream.attach(encoder, 5, true);				
				
			//HTTP::MultipartPacketizer* packetizer = new HTTP::MultipartPacketizer("image/jpeg");
			//stream.attach(packetizer, 10);

			//FLVMetadataInjector* injector = new FLVMetadataInjector(params.oformat);
			//stream.attach(injector);

			// start the stream
			stream.start();
				
			stream += packetDelegate(this, &StreamTest::onVideoEncoded);

			//while (!stop)
			//{
			//	Thread::sleep(50);
			//}
				
			//stream -= packetDelegate(this, &StreamTest::onVideoEncoded);
			
			Log("trace") << "[MediaConnection] ENDING!!!!!!!!!!!!!" << std::endl;
		}
		catch (Poco::Exception& exc)
		{
			Log("trace") << "[MediaConnection] ERROR: " << exc.displayText() << std::endl;
		}
		Log("trace") << "[MediaConnection] Ending ###############################" << std::endl;
	}

	void onVideoEncoded(void* sender, DataPacket& packet)
	{
		Log("trace") << "[MediaConnection] Video Encoded" << std::endl;

		fpsCounter.tick();

		//Log("trace") << "[MediaConnection] Sending Packet: " << packet.size << ": " << fpsCounter.fps << std::endl;
		//Log("trace") << "[MediaConnection] Sending Packet: " << string((const char*)packet.data, 50) << std::endl;
		
		StreamSocket& ss = socket();
		if (fpsCounter.frames == 50) {
			//stream.detach(videoCapture);
			//waitForEnd = true;
			//stop = true;
			//stream.start();
			stream.stop();
			//encoder->uninitialize();
		}

		try
		{		
			ss.sendBytes(packet.data, packet.size);
		}
		catch (Poco::Exception& exc)
		{
			Log("trace") << "[MediaConnection] " << exc.displayText() << std::endl;
			stop = true;
		}

		//if (waitForEnd)
		//	stop = true;
	}
		
	bool waitForEnd;
	AVEncoder* encoder;
	PacketStream stream;
	FPSCounter fpsCounter;
};
		*/
