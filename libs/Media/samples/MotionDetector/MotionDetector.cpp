#include "Sourcey/Base.h"
#include "Sourcey/PacketStream.h"
#include "Sourcey/Media/MediaFactory.h"
#include "Sourcey/Media/MotionDetector.h"


using namespace std;
using namespace Poco;
using namespace Sourcey;
using namespace Sourcey::Media;


// Detect Memory Leaks
/*
#ifdef _DEBUG
#include "MemLeakDetect/MemLeakDetect.h"
CMemLeakDetect memLeakDetect;
#endif<DataPacket>
*/


namespace Sourcey { 
namespace Media {	


// ----------------------------------------------------------------------------
//
// Motion Detector Stream
//
// ----------------------------------------------------------------------------
class MotionDetectorStream: public PacketStream
{
public:
	MotionDetectorStream()
	{	

		VideoCapture* video = MediaFactory::instance()->video.getCapture(0);		//new VideoCapture(0); //
		attach(video, false);

		Format format;
		AllocateOpenCVInputFormat(video, format);
		assert(format.video.width && format.video.height);
		
		MotionDetector::Options options;
		MotionDetector* detector = new MotionDetector(options);
		attach(detector, 3, true);	

		attach(packetDelegate(this, &MotionDetectorStream::onVideoEncoded));
	}

	virtual ~MotionDetectorStream() 
	{
	}

	void onVideoEncoded(void* sender, MatPacket& packet)
	{
		Log("trace") << "[MotionDetectorApp] Video Encoded" << endl;

		cv::imshow("Motion Image", *packet.mat);
		cv::waitKey(10);
	}
};


} } // namespace Sourcey::Media


int main(int argc, char** argv)
{
	Logger::instance().add(new ConsoleChannel("debug", TraceLevel));
	
	Media::MediaFactory::initialize();
	Media::MediaFactory::instance()->loadVideo();
	Media::MediaFactory::instance()->loadAudio();

	MotionDetectorStream test;
	test.start();
	/*
	MotionDetectorStream test1;
	test1.start();	
	MotionDetectorStream test2;
	test2.start();
	*/
	system("pause");

	return 0;
}
