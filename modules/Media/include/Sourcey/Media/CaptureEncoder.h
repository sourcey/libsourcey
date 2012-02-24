//
// This software is copyright by Sourcey <mail@sourcey.com> and is distributed under a dual license:
// Copyright (C) 2005 Sourcey
//
// Non-Commercial Use:
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// Commercial Use:
// Please contact mail@sourcey.com
//


#ifndef DEVICE_ENCODER_H
#define DEVICE_ENCODER_H


#include "Sourcey/Media/Types.h"
#include "Sourcey/Media/ICapture.h"
#include "Sourcey/Media/IEncoder.h"
#include "Sourcey/Media/AudioEncoder.h"
#include "Sourcey/Media/VideoEncoder.h"
#include "Sourcey/IRunnable.h"
#include "Sourcey/Logger.h"

#include "Poco/DateTimeFormatter.h"
#include "Poco/Timestamp.h"


namespace Sourcey {
namespace Media {


template <class EncoderT>
class CaptureEncoder: public IRunnable, public EncoderT
	/// This class extends a Encoder object for encoding the output
	/// of an ICapture object.
	///
	/// EncoderT should be a VideoEncoder or AudioEncoder.
	///
	/// NOTE: This class is depreciated.
{
public:
	CaptureEncoder(ICapture* capture, const EncoderParams& params, bool destroyOnStop = false) : 
		EncoderT(params),
		_capture(capture),
		_destroyOnStop(destroyOnStop)
	{
		Log("debug") << "[CaptureEncoder] Creating" << std::endl;
		assert(_capture);	
	}

	virtual ~CaptureEncoder() 
	{
		Log("debug") << "[CaptureEncoder] Destroying" << std::endl;

		// A call to stop() is required before destruction.
		assert(state().id() == EncoderState::None);
	}

	virtual void start(/*const ParamT& params*/) 
	{
		Log("debug") << "[CaptureEncoder] Starting..." << std::endl;
		if (!EncoderT::isReady())
			EncoderT::initialize();

		try {
			assert(_capture);
			
			EncoderT::setState(this, EncoderState::Encoding);

			_capture->attach(polymorphicDelegate(this, &CaptureEncoder::onCapture));
			//_capture->start(Callback<CaptureEncoder, const MediaPacket, false>(this, &CaptureEncoder::onCapture));
			
		} 
		catch (Exception& exc) {
			Log("error") << "Encoder Error: " << exc.displayText() << std::endl;
			EncoderT::setState(this, EncoderState::Failed);
			stop();
			exc.rethrow();
		}
	}

	virtual void stop() 
	{
		Log("debug") << "[CaptureEncoder] Stopping..." << std::endl;
		try {
			assert(_capture);
			//assert(EncoderT::isReady());

			_capture->detach(polymorphicDelegate(this, &CaptureEncoder::onCapture));
			//_capture->stop(Callback<CaptureEncoder, const MediaPacket, false>(this, &CaptureEncoder::onCapture));
			
			EncoderT::setState(this, EncoderState::None);
		} 
		catch (Exception& exc) {
			Log("error") << "Encoder Error: " << exc.displayText() << std::endl;
			EncoderT::setState(this, EncoderState::Failed);
		}

		if (_destroyOnStop)
			delete this;
	}
	
	virtual void onCapture(void*, MediaPacket& packet)
	{
		try {
			if (!EncoderT::isEncoding())
				throw Exception("The encoder is not initialized.");

			int size = EncoderT::encode((unsigned char*)packet.data, packet.size);
		} 
		catch (Exception& exc) {
			Log("error") << "Encoder Error: " << exc.displayText() << std::endl;
			EncoderT::setState(this, EncoderState::Failed);
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
		Log("debug") << "[RawCaptureEncoder] Creating" << std::endl;
		assert(_capture);	
		EncoderT::setState(this, EncoderState::None);
	}


	virtual ~RawCaptureEncoder() 
	{
		Log("debug") << "[RawCaptureEncoder] Destroying" << std::endl;

		// A call to stop() is required before destruction.
		assert(state().id() == EncoderState::None);
	}

	virtual void start() 
	{
		Log("debug") << "[RawCaptureEncoder] Starting..." << std::endl;
		try {
			assert(_capture);
			//assert(isInitialized());
			
			_capture->start(Callback<RawCaptureEncoder, const MediaPacket, false>(this, &RawCaptureEncoder::onCapture));
			
			EncoderT::setState(this, EncoderState::Encoding);
		} 
		catch (Exception& exc) {
			Log("error") << "Encoder Error: " << exc.displayText() << std::endl;
			EncoderT::setState(this, EncoderState::Failed);
			stop();
			exc.rethrow();
		}
	}

	virtual void stop() 
	{
		Log("debug") << "[RawCaptureEncoder] Stopping..." << std::endl;
		try {
			assert(_capture);
			//assert(isInitialized());

			_capture->stop(Callback<RawCaptureEncoder, const MediaPacket, false>(this, &RawCaptureEncoder::onCapture));

			EncoderT::setState(this, EncoderState::None);
		} 
		catch (Exception& exc) {
			Log("error") << "Encoder Error: " << exc.displayText() << std::endl;
			EncoderT::setState(this, EncoderState::Failed);
		}

		if (_destroyOnStop)
			delete this;
	}
	
	virtual void onCapture(const MediaPacket& packet)
	{		
		if (!isEncoding())
			throw Exception("The encoder is not initilaized.");

		// No encoding takes place, just relay the packet.
		//PacketEncoded.dispatch(this, static_cast<Packet&>(packet));

		PacketEncoded.dispatch(this, packet);
	}

	// Connect to PacketEncoded signal to receive encoded packets.

private:
	ICapture*	_capture;
	bool		_destroyOnStop;
};
*/


} } // namespace Sourcey::Media


#endif // DEVICE_ENCODER_H