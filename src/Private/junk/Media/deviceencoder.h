#ifndef DEVICE_ENCODER_H
#define DEVICE_ENCODER_H


#include "Sourcey/Media/Types.h"
#include "Sourcey/Media/ICapture.h"
#include "Sourcey/Media/IEncoder.h"
#include "Sourcey/Base/Logger.h"

#include "Poco/DateTimeFormatter.h"
#include "Poco/Timestamp.h"


namespace Sourcey {
namespace Media {


template <class EncoderT>
class CaptureEncoder: public EncoderT
	/// This class extends a Encoder object and encodes the output
	/// of a single Capture object.
{
public:
	CaptureEncoder(ICapture* capture, bool destroyOnStop = false) : 
		_capture(capture),
		_destroyOnStop(destroyOnStop)
	{
		Log("trace") << "[CaptureEncoder] Initializing..." << std::endl;
		assert(_capture);	
		setState(EncoderState::Idle);
	}


	virtual ~CaptureEncoder() 
	{
		Log("trace") << "[CaptureEncoder] Destroying..." << std::endl;

		// A call to stop() is required before destruction.
		assert(state().id() == EncoderState::Idle);
	}

	virtual void start(/*const ParamT& params*/) 
	{
		Log("trace") << "[CaptureEncoder] Starting..." << std::endl;
		try {
			assert(_capture);
			assert(isInitialized());
			
			_capture->start(Callback<CaptureEncoder, const MediaPacket, false>(this, &CaptureEncoder::onCapture));
			
			setState(EncoderState::Encoding);
		} 
		catch (Exception& exc) {
			Log("error") << "Encoder Error: " << exc.displayText() << std::endl;
			setState(EncoderState::Error);
			stop();
			exc.rethrow();
		}
	}

	virtual void stop() 
	{
		Log("trace") << "[CaptureEncoder] Stopping..." << std::endl;
		try {
			assert(_capture);
			assert(isInitialized());

			_capture->stop(Callback<CaptureEncoder, const MediaPacket, false>(this, &CaptureEncoder::onCapture));

			setState(EncoderState::Idle);
		} 
		catch (Exception& exc) {
			Log("error") << "Encoder Error: " << exc.displayText() << std::endl;
			setState(EncoderState::Error);
		}

		if (_destroyOnStop)
			delete this;
	}
	
	virtual void onCapture(const MediaPacket& packet)
	{
		try {
			if (!isEncoding())
				throw Exception("The encoder is not initilaized.");

			int size = encode((unsigned char*)packet.data, packet.size);
		} 
		catch (Exception& exc) {
			Log("error") << "Encoder Error: " << exc.displayText() << std::endl;
			setState(EncoderState::Error);
			stop();
		}
	}

	// Connect to PacketEncoded signal to receive encoded packets.

private:
	EncoderT	_encoder; 
	ICapture*	_capture;
	bool		_destroyOnStop;
};


class RawCaptureEncoder: public IEncoder
	/// This class extends a Encoder object and encodes the output
	/// of a single Capture object.
	///
	/// This is a convenience class for DataProvider's which want
	// to receive raw device output without encoding.
{
public:
	RawCaptureEncoder(ICapture* capture, bool destroyOnStop = false) : 
		_capture(capture),
		_destroyOnStop(destroyOnStop)
	{
		Log("trace") << "[RawCaptureEncoder] Initializing..." << std::endl;
		assert(_capture);	
	}


	virtual ~RawCaptureEncoder() 
	{
		Log("trace") << "[RawCaptureEncoder] Destroying..." << std::endl;

		// A call to stop() is required before destruction.
		assert(state().id() == EncoderState::None);
	}

	virtual void start() 
	{
		Log("trace") << "[RawCaptureEncoder] Starting..." << std::endl;
		try {
			assert(_capture);
			//assert(isInitialized());
			
			_capture->start(Callback<RawCaptureEncoder, const MediaPacket, false>(this, &RawCaptureEncoder::onCapture));
			
			setState(EncoderState::Encoding);
		} 
		catch (Exception& exc) {
			Log("error") << "Encoder Error: " << exc.displayText() << std::endl;
			setState(EncoderState::Error);
			stop();
			exc.rethrow();
		}
	}

	virtual void stop() 
	{
		Log("trace") << "[RawCaptureEncoder] Stopping..." << std::endl;
		try {
			assert(_capture);
			//assert(isInitialized());

			_capture->stop(Callback<RawCaptureEncoder, const MediaPacket, false>(this, &RawCaptureEncoder::onCapture));

			setState(EncoderState::None);
		} 
		catch (Exception& exc) {
			Log("error") << "Encoder Error: " << exc.displayText() << std::endl;
			setState(EncoderState::Error);
		}

		if (_destroyOnStop)
			delete this;
	}
	
	virtual void onCapture(const MediaPacket& packet)
	{		
		if (!isEncoding())
			throw Exception("The encoder is not initilaized.");

		// No encoding takes place, just relay the packet.
		//PacketEncoded.send(this, static_cast<Packet&>(packet));

		PacketEncoded.send(this, packet);
	}

	// Connect to PacketEncoded signal to receive encoded packets.

private:
	ICapture*	_capture;
	bool		_destroyOnStop;
};


} } // namespace Sourcey::Media


#endif // DEVICE_ENCODER_H