//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// LibSourcey is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//


#ifndef SOURCEY_MEDIA_CaptureEncoder_H
#define SOURCEY_MEDIA_CaptureEncoder_H


#include "Sourcey/Media/Types.h"
#include "Sourcey/Media/ICapture.h"
#include "Sourcey/Media/IEncoder.h"
#include "Sourcey/Media/AudioEncoder.h"
#include "Sourcey/Media/VideoEncoder.h"
#include "Sourcey/Interface.h"
#include "Sourcey/Logger.h"
#include "Sourcey/DateTime.h"

//#include "Poco/Timestamp.h"


namespace scy {
namespace av {


template <class EncoderT>
class CaptureEncoder: public basic::Startable, public EncoderT
	/// This class extends a Encoder object for encoding the output
	/// of an ICapture object.
	///
	/// EncoderT should be a VideoEncoder or AudioEncoder.
	///
	/// Note: This class is depreciated.
{
public:
	CaptureEncoder(ICapture* capture, const EncoderOptions& options, bool destroyOnStop = false) : 
		EncoderT(params),
		_capture(capture),
		_destroyOnStop(destroyOnStop)
	{
		traceL() << "[CaptureEncoder] Creating" << std::endl;
		assert(_capture);	
	}

	virtual ~CaptureEncoder() 
	{
		traceL() << "[CaptureEncoder] Destroying" << std::endl;

		// A call to stop() is required before destruction.
		assert(EncoderT::state().id() == EncoderState::None);
	}

	virtual void start(/*const ParamT& params*/) 
	{
		traceL() << "[CaptureEncoder] Starting..." << std::endl;
		if (!EncoderT::isReady())
			EncoderT::initialize();

		try {
			assert(_capture);
			
			EncoderT::setState(this, EncoderState::Encoding);

			_capture->attach(polymorphicDelegate(this, &CaptureEncoder::onCapture));
			//_capture->start(Callback<CaptureEncoder, const MediaPacket, false>(this, &CaptureEncoder::onCapture));
			
		} 
		catch (std::exception& exc) {
			errorL() << "Encoder Error: " << exc.what() << std::endl;
			EncoderT::setState(this, EncoderState::Error);
			stop();
			throw exc;/*exc.rethrow();*/
		}
	}

	virtual void stop() 
	{
		traceL() << "[CaptureEncoder] Stopping..." << std::endl;
		try {
			assert(_capture);
			//assert(EncoderT::isReady());

			_capture->detach(polymorphicDelegate(this, &CaptureEncoder::onCapture));
			//_capture->stop(Callback<CaptureEncoder, const MediaPacket, false>(this, &CaptureEncoder::onCapture));
			
			EncoderT::setState(this, EncoderState::None);
		} 
		catch (std::exception& exc) {
			errorL() << "Encoder Error: " << exc.what() << std::endl;
			EncoderT::setState(this, EncoderState::Error);
		}

		if (_destroyOnStop)
			delete this;
	}
	
	virtual void onCapture(void*, MediaPacket& packet)
	{
		try {
			if (!EncoderT::isEncoding())
				throw std::runtime_error("The encoder is not initialized.");

			int size = EncoderT::encode((unsigned char*)packet.data, packet.size);
		} 
		catch (std::exception& exc) {
			errorL() << "Encoder Error: " << exc.what() << std::endl;
			EncoderT::setState(this, EncoderState::Error);
			stop();
		}
	}

	// Connect to PacketEncoded signal to receive encoded packets.

private:
	EncoderT	_encoder; 
	ICapture*	_capture;
	bool		_destroyOnStop;
};


/*
class RawCaptureEncoder: public IEncoder
	/// This class extends a Encoder object and encodes the output
	/// of a single Capture object.
	///
	/// This is a convenience class for Provider's which want
	// to receive raw device output without encoding.
{
public:
	RawCaptureEncoder(ICapture* capture, bool destroyOnStop = false) : 
		_capture(capture),
		_destroyOnStop(destroyOnStop)
	{
		traceL() << "[RawCaptureEncoder] Creating" << std::endl;
		assert(_capture);	
		EncoderT::setState(this, EncoderState::None);
	}


	virtual ~RawCaptureEncoder() 
	{
		traceL() << "[RawCaptureEncoder] Destroying" << std::endl;

		// A call to stop() is required before destruction.
		assert(state().id() == EncoderState::None);
	}

	virtual void start() 
	{
		traceL() << "[RawCaptureEncoder] Starting..." << std::endl;
		try {
			assert(_capture);
			//assert(isInitialized());
			
			_capture->start(Callback<RawCaptureEncoder, const MediaPacket, false>(this, &RawCaptureEncoder::onCapture));
			
			EncoderT::setState(this, EncoderState::Encoding);
		} 
		catch (std::exception& exc) {
			errorL() << "Encoder Error: " << exc.what() << std::endl;
			EncoderT::setState(this, EncoderState::Error);
			stop();
			throw exc;/*exc.rethrow();*/
		}
	}

	virtual void stop() 
	{
		traceL() << "[RawCaptureEncoder] Stopping..." << std::endl;
		try {
			assert(_capture);
			//assert(isInitialized());

			_capture->stop(Callback<RawCaptureEncoder, const MediaPacket, false>(this, &RawCaptureEncoder::onCapture));

			EncoderT::setState(this, EncoderState::None);
		} 
		catch (std::exception& exc) {
			errorL() << "Encoder Error: " << exc.what() << std::endl;
			EncoderT::setState(this, EncoderState::Error);
		}

		if (_destroyOnStop)
			delete this;
	}
	
	virtual void onCapture(const MediaPacket& packet)
	{		
		if (!isEncoding())
			throw std::runtime_error("The encoder is not initilaized.");

		// No encoding takes place, just relay the packet.
		//PacketEncoded.emit(this, static_cast<Packet&>(packet));

		PacketEncoded.emit(packet);
	}

	// Connect to PacketEncoded signal to receive encoded packets.

private:
	ICapture*	_capture;
	bool		_destroyOnStop;
};
*/


} } // namespace scy::av


#endif // SOURCEY_MEDIA_CaptureEncoder_H