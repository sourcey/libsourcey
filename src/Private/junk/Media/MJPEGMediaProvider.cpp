#include "Sourcey/Media/MJPEGMediaProvider.h"

#include "Poco/Net/HTTPResponse.h"


using namespace std;
using namespace Poco;



namespace Sourcey {
namespace Media {


// ============================================================================
//
// Audio/Video Media Provider
//
// ============================================================================
MJPEGMediaProvider::MJPEGMediaProvider(Media::VideoCapture* capture, const VideoParams& params, bool destroyOnStop) : 
	_capture(capture),
	_params(params),
	_encoder(NULL),
	_destroyOnStop(destroyOnStop) {
	cout << "MJPEGMediaProvider::MJPEGMediaProvider()" << endl;
}


MJPEGMediaProvider::~MJPEGMediaProvider() {
	cout << "MJPEGMediaProvider::~MJPEGMediaProvider()" << endl;
}


void MJPEGMediaProvider::start() {
	_params.codec = Media::MJPEG;
	_params.pixfmt = Media::YUVJ420P;	

	cout << "MJPEG Provider Initializing:"		
		<< "\n\tPID: " << this
		<< "\n\tCodec: " << _params.codec
		<< "\n\tWidth: " << _params.width
		<< "\n\tHeight: " << _params.height
		<< "\n\tFPS: " << _params.fps
		//<< "\n\tQuality: " << _quality
		<< endl;

	assert(!_encoder);
	assert(_params.width);
	assert(_params.height);
	assert(_params.fps);
	//assert(_quality);

	try {
		_encoder = new Media::VideoCaptureEncoder(_capture, _params);
		_encoder->VideoEncoded += delegate(this, &MJPEGMediaProvider::onVideoReady);	

		// Send the initial response header
		// TODO: Remove HTTPResponse dependency
		ostringstream ss;
		Net::HTTPResponse response;
		response.set("Server", "Spot");
		response.set("Expires", "0");
		response.set("Pragma", "no-cache");
		response.set("Cache-Control", "no-store");
		response.set("Content-Type", "multipart/x-mixed-replace; boundary=--eof");
		response.write(ss);	
		Packet packet((unsigned char *)ss.str().data(), ss.str().length());
		MediaPacketReady.send(this, packet);

		// Start the encoder	
		_encoder->start();
		//_capture->start(VideoCallback<MJPEGMediaProvider>(*this, &MJPEGMediaProvider::onVideoCapture));

	}
	catch (Exception& exc) {
		cerr << "[MJPEGMediaProvider] Exception: " << exc.displayText() << endl;
		sendError(exc.displayText());
	}
}


void MJPEGMediaProvider::stop() {
	// Stop the encoder
	if (_encoder) {
		_encoder->stop();	
		delete _encoder;
		_encoder = NULL;
	}
}


void MJPEGMediaProvider::onVideoReady(void* sender, Media::VideoPacket& packet) {
	try {
		if (packet.size) {			
			string header = format(
				"--eof\r\n"
				"Content-Type: image/jpeg\r\n"
				"Content-Length: %d\r\n"
				"\r\n",
				packet.size);

			assert(packet.size + header.size() < MAX_VIDEO_PACKET_SIZE);
			memmove(packet.data + header.size(), packet.data, packet.size + header.size());	
			memcpy(packet.data, header.data(), header.size());
			packet.size = packet.size + header.size();
			MediaPacketReady.send(this, packet);
		}
	}
	catch (Exception& exc) {
		cerr << "Exception: " << exc.displayText() << endl;
		stop();
	}
}


void MJPEGMediaProvider::sendError(const std::string& err) {
	ostringstream ss;
	Net::HTTPResponse response;
	response.setStatus(Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
	response.write(ss);	
	ss << err;
	Packet packet((unsigned char *)ss.str().data(), ss.str().length());
	MediaPacketReady.send(this, packet);
}


} } // namespace Sourcey::Media