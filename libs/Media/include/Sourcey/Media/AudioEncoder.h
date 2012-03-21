//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is is distributed under a dual license that allows free, 
// open source use and closed source use under a standard commercial
// license.
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


#ifndef SOURCEY_MEDIA_AudioEncoder_H
#define SOURCEY_MEDIA_AudioEncoder_H


#include "Sourcey/Media/Types.h"
#include "Sourcey/Media/IEncoder.h"

#include "Poco/Mutex.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/fifo.h>
#include <libswscale/swscale.h>
}


namespace Sourcey {
namespace Media {


class AudioEncoder: public IPacketEncoder
	/// This class implements an audio encoder which 
	/// depends on libavcodec/libavformat.
	///
	/// NOTE: Depreciated in favor of AVEncoder.
{
public:		
	AudioEncoder(const EncoderParams& params = EncoderParams());
	virtual ~AudioEncoder();

	virtual void initialize();
	virtual void uninitialize();
	virtual void cleanup();

	virtual bool isReady();
	
	virtual bool accepts(IPacket& packet);
	virtual void process(IPacket& packet);

    virtual int encode(unsigned char *input, int inputSize/*, unsigned char *output, unsigned int outputSize*/);

    virtual AVCodecContext* getEncoderContext() const { return _encoderContext; }
	
    virtual EncoderParams& params() { return _params; }	
    virtual double getNow() const { return _now; }

private:
	static Poco::FastMutex _mutex;	// Mutex locker to protect avcodec_open/close().
	//AudioCodec		_iCodec;
	//AudioCodec		_oCodec;	
	EncoderParams	_params;
	AVFifoBuffer*	_fifo;		
	UInt8*			_fifoBuffer;
	int				_outSize;
	UInt8*			_outBuffer;
    AVCodecContext*	_encoderContext;
    double			_now;
};


} } // namespace Sourcey::Media


#endif	// SOURCEY_MEDIA_AudioEncoder_H

