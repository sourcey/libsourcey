#include "Sourcey/Media/VideoCaptureEncoder.h"

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "Poco/DateTimeFormatter.h"
#include "Poco/Timestamp.h"

#include "assert.h"


using namespace std;
using namespace Poco;


namespace Sourcey {
namespace Media {


// ============================================================================
//
// Video Capture Encoder
//
// ============================================================================
VideoCaptureEncoder::VideoCaptureEncoder(VideoCapture* capture, const VideoParams& params, bool destroyOnStop) : 
	_capture(capture),
	_params(params), 
	_buffer(new unsigned char[MAX_VIDEO_PACKET_SIZE]),
	_destroyOnStop(destroyOnStop) 
{
	cout << "[VideoCaptureEncoder] Initializing..." << endl;
	assert(_capture);	
	setState(EncoderState::Idle);
}


VideoCaptureEncoder::VideoCaptureEncoder(bool destroyOnStop) : 
	_capture(NULL),
	//_buffer(new unsigned char[MAX_VIDEO_PACKET_SIZE]),
	_destroyOnStop(destroyOnStop) 
{
	cout << "[VideoCaptureEncoder] Initializing..." << endl;
	setState(EncoderState::Idle);
}


VideoCaptureEncoder::~VideoCaptureEncoder() 
{
	cout << "[VideoCaptureEncoder] Destroying..." << endl;

	// A call to stop() is required before destruction.
	assert(!_encoder.isInitialized());
}


void VideoCaptureEncoder::start() 
{
	cout << "[VideoCaptureEncoder] Starting..." << endl;
	try {
		assert(_capture);
		assert(_capture->width());
		assert(_capture->height());
		assert(!_encoder.isInitialized());

		_encoder.initEncodeContext(_params);

		VideoParams iParams;
		iParams.codec = Media::NoVideo;
		iParams.width = _capture->width();
		iParams.height = _capture->height();
		//iParams.pixfmt = VideoPixelFormat::YUV420P;
		//iParams.pixfmt = VideoPixelFormat::RGB24;
		iParams.pixfmt = Media::BGR24;
		_encoder.initScaleContext(iParams);
		
		_capture->start(VideoCallback<VideoCaptureEncoder>(this, &VideoCaptureEncoder::onVideoCapture));

		setState(EncoderState::Encoding);
	} 
	catch (Exception& exc) {
		cerr << "Encoder Error: " << exc.displayText() << endl;
		setState(EncoderState::Error);
		stop();
		throw exc;
	}
}

void VideoCaptureEncoder::stop() 
{
	cout << "[VideoCaptureEncoder] Stopping..." << endl;
	try {
		assert(_capture);
		assert(_encoder.isInitialized());

		_capture->stop(VideoCallback<VideoCaptureEncoder>(this, &VideoCaptureEncoder::onVideoCapture));

		_encoder.resetScaleContext();
		_encoder.resetEncodeContext();

		setState(EncoderState::Idle);
	} 
	catch (Exception& exc) {
		cerr << "Encoder Error: " << exc.displayText() << endl;
		setState(EncoderState::Error);
	}

	if (_destroyOnStop)
		delete this;
}
	

void VideoCaptureEncoder::onVideoCapture(const VideoPacket& packet) 
{
	try {
		if (!_encoder.isInitialized()) {
			throw Exception("The encoder is not initilaized.");
			return;
		}

		// Skip frames if we exceed the maximum FPS
		if (_counter.started()) {
			_counter.endFrame();
			if (_counter.fps > _params.fps) {
				return;
			}
		}
		_counter.startFrame();

		int size = _encoder.encode((unsigned char *)packet.data, packet.size, _buffer, MAX_VIDEO_PACKET_SIZE);

		VideoPacket encPacket;
		encPacket.data = (void*)_buffer;
		encPacket.size = size;
		PacketEncoded.send(this, encPacket);
	} 
	catch (Exception& exc) {
		cerr << "Encoder Error: " << exc.displayText() << endl;
		setState(EncoderState::Error);
		stop();
	}
}


} } // namespace Sourcey::Media