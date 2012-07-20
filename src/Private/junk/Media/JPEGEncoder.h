#ifndef MEDIA_MJPEG_ENCODER_H
#define MEDIA_MJPEG_ENCODER_H


#include "Sourcey/Media/Types.h"
#include "Sourcey/Media/VideoCapture.h"
#include "Sourcey/Media/IEncoder.h"
#include "Sourcey/Base/Logger.h"

#include "Poco/DateTimeFormatter.h"
#include "Poco/Timestamp.h"


namespace Sourcey {
namespace Media {


class JPEGEncoder: public IPacketEncoder
	/// This class extends a Encoder object and encodes the
	/// output of a single VideoCapture object into JPEG frames.
	///
	/// This is class relies solely on OpenCV conversion so it
	/// does not require FFMpeg.
{
public:
	JPEGEncoder(VideoCapture* capture, const VideoCodec& params, bool destroyOnStop = false) : 
		_capture(capture),
		_params(params),
		_destroyOnStop(destroyOnStop)
	{
		Log("trace") << "[JPEGEncoder] Creating" << std::endl;
		assert(_capture);	
		setState(EncoderState::None);
	}

	virtual ~JPEGEncoder() 
	{
		Log("trace") << "[JPEGEncoder] Destroying" << std::endl;

		// A call to stop() is required before destruction.
		assert(state().id() == EncoderState::None);
	}

	virtual void start() 
	{
		Log("trace") << "[JPEGEncoder] Starting..." << std::endl;
		try {
			assert(_capture);			
			//_capture->start(VideoCallback<JPEGEncoder>(this, &JPEGEncoder::onCapture, NULL, _params.fps)); //
			
			setState(EncoderState::Ready);
		} 
		catch (Exception& exc) {
			Log("error") << "Encoder Error: " << exc.displayText() << std::endl;
			setState(EncoderState::Failed);
			stop();
			exc.rethrow();
		}
	}

	virtual void stop() 
	{
		Log("trace") << "[JPEGEncoder] Stopping..." << std::endl;
		try {
			assert(_capture);
			//_capture->stop(VideoCallback<JPEGEncoder>(this, &JPEGEncoder::onCapture));

			setState(EncoderState::None);
		} 
		catch (Exception& exc) {
			Log("error") << "Encoder Error: " << exc.displayText() << std::endl;
			setState(EncoderState::Failed);
		}

		if (_destroyOnStop)
			delete this;
	}	

	virtual void onCapture(const VideoPacket& packet)
	{		
		Log("trace") << "[JPEGEncoder] onCapture..." << std::endl;

		if (!isReady())
			throw Exception("The encoder is not initilaized.");

		if (!packet.mat->empty()) {	
			CvMat *jpeg;
			int params[3] = {0};
			params[0] = CV_IMWRITE_JPEG_QUALITY;
			params[1] = _params.quality; // default: 80% quality
			if (_params.width != packet.mat->cols || 
				_params.height != packet.mat->rows) {
				cv::Mat resized;
				cv::resize(*packet.mat, resized, cv::Size(_params.width, _params.height));
				jpeg = cvEncodeImage(".jpg", &(IplImage)resized, params);
			} else
				jpeg = cvEncodeImage(".jpg", &packet.mat->operator IplImage(), params);
			if (jpeg->data.s) {
				DataPacket opacket;
				opacket.data = jpeg->data.ptr;
				opacket.size = jpeg->width;	
				broadcast(opacket);
			}
			cvReleaseMat(&jpeg);
		}
	}

	// Connect to PacketEncoded signal to receive encoded packets.

private:
	VideoCapture*	_capture;
	VideoCodec		_params;
	bool			_destroyOnStop;
};


} } // namespace Sourcey::Media


#endif // MEDIA_MJPEG_ENCODER_H