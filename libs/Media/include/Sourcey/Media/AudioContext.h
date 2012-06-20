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


#ifndef SOURCEY_MEDIA_AudioContext_H
#define SOURCEY_MEDIA_AudioContext_H


#include "Sourcey/Media/Types.h"
#include "Sourcey/Media/Format.h"
//#include "Sourcey/Media/FPSCounter.h"
//#include "Sourcey/Media/IEncoder.h"

//#include "Poco/Mutex.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/fifo.h>
#include <libswscale/swscale.h>
}


namespace Sourcey {
namespace Media {


struct AudioContext
{
	AudioContext();
	virtual ~AudioContext();
	
	virtual void open();
	virtual void close();		
	virtual void reset();	
	
    // Internal data 
    AVStream*			stream;
    AVCodecContext*		codec;
	AVFrame*            frame;
    //AVCodec*			codec;
    //AVPacket*			packet;
	
    //int					bufferSize;
    //UInt8*				buffer;

	// The output frame size for encoding and decoding.
    //int					frameSize;

    //int				offset;

	// Exposed properties
	/*
    int bitRate;
    int sampleRate;
    int bitsPerSample;
    int channels;
	*/
	UInt64 frameNum;
    double				pts;
	
    std::string			error;
};


// ---------------------------------------------------------------------
//
struct AudioEncoderContext: public AudioContext
{
	AudioEncoderContext();
	virtual ~AudioEncoderContext();	
	
	virtual void open(AVFormatContext* oc);
	virtual void close();
	virtual void reset();	
	
	virtual bool encode(unsigned char* data, int size, AVPacket& opacket);
	virtual bool encode(AVPacket& ipacket, AVPacket& opacket);

	AudioCodec		iparams;
	AudioCodec		oparams;
};


// ---------------------------------------------------------------------
//
struct AudioDecoderContext: public AudioContext
{
	AudioDecoderContext();
	virtual ~AudioDecoderContext();
	
	virtual void open(AVFormatContext *ic, int streamID);
	virtual void close();
	virtual void reset();	
	
	virtual bool decode(UInt8* data, int size, AVPacket& opacket);
	virtual bool decode(AVPacket& ipacket, AVPacket& opacket);
		// Decodes a single frame from the provided packet.
		// Returns the size of the decoded frame. 
		// IMPORTANT: In order to ensure all data is decoded from the
		// input packet, this method should be looped until the input
		// packet size is 0.
		// Example:
		//	while (packet.size > 0) {
		//		decode(packet);
		//	}

    double duration;
    int width;	// Number of bits used to store a sample
    bool fp;	// Floating-point sample representation
};


} } // namespace Sourcey::Media


#endif	// SOURCEY_MEDIA_AudioContext_H

