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


#ifndef SOURCEY_MEDIA_Codec_H
#define SOURCEY_MEDIA_Codec_H


#include "Sourcey/Base.h"
#include "Sourcey/Media/Config.h"

#include <iostream>
#include <string>
#include <list>


namespace Sourcey {
namespace Media {


// ---------------------------------------------------------------------
//
struct Codec 
	/// Specifies a codec for encoding/decoding media.
{	
	//
	// Base Codec Variables
	//
	std::string name;		// The display name for this codec.
	std::string encoder;	// The encoder name for FFmpeg.
	int sampleRate;			// The sampling rate or RTP clock rate.
	int bitRate;			// The bit rate to encode at.
	int quality;			// Optional quality value, variable range depending on codec.
	bool enabled;			// Weather or not the codec is available for use.

	//
	// Ctors/Dtors
	//
	Codec();
	Codec(/*UInt32 id, */const std::string& name, int sampleRate = 0, int bitRate = 0, bool enabled = true);
	Codec(/*UInt32 id, */const std::string& name, const std::string& encoder, int sampleRate = 0, int bitRate = 0, bool enabled = true);
	Codec(const Codec& r);
		
	//
	// Methods
	//
	virtual std::string toString() const;
	virtual void print(std::ostream& ost);
};


// ---------------------------------------------------------------------
//
struct SampleFormat 
	/// Common sample formats for audio encoding/decoding.
	/// The ID values match their constituent AV_SAMPLE_FMT_XXX
	/// values in the FFmpeg library.
{
	enum ID 
	{
		NONE = -1,
		U8,          
		S16,         
		S32,         
		FLT,         
		DBL,         

		U8P,         
		S16P,        
		S32P,        
		FLTP,        
		DBLP,        

		NB  
	};
};


// ---------------------------------------------------------------------
//
#define DEFAULT_AUDIO_SAMPLE_RATE	44100
#define DEFAULT_AUDIO_BIT_RATE		64000
#define DEFAULT_AUDIO_CHANNELS		2
#define DEFAULT_AUDIO_SAMPLE_FMT	SampleFormat::S16


struct AudioCodec: public Codec 
{	
	int channels;
	UInt32 sampleFmt;
	
	AudioCodec();
	AudioCodec(/*UInt32 id, */
		const std::string& name, 
		int channels = DEFAULT_AUDIO_CHANNELS, 
		int sampleRate = DEFAULT_AUDIO_SAMPLE_RATE, 
		int bitRate = DEFAULT_AUDIO_BIT_RATE,
		UInt32 sampleFmt = (UInt32)DEFAULT_AUDIO_SAMPLE_FMT);
	AudioCodec(/*UInt32 id, */
		const std::string& name, 
		const std::string& encoder, 
		int channels = DEFAULT_AUDIO_CHANNELS, 
		int sampleRate = DEFAULT_AUDIO_SAMPLE_RATE, 
		int bitRate = DEFAULT_AUDIO_BIT_RATE,
		UInt32 sampleFmt = (UInt32)DEFAULT_AUDIO_SAMPLE_FMT);
	AudioCodec(const AudioCodec& r);

	virtual std::string toString() const;
	virtual void print(std::ostream& ost);
};
	

// ---------------------------------------------------------------------
//
struct PixelFormat 
	/// Common pixel formats for video encoding/decoding .
	/// The ID values match their constituent AV_PIX_FMT_XXX
	/// values in the FFmpeg library.
{
	enum ID 
	{
		YUV420P		= 0,	///< planar YUV 4:2:0, 12bpp, (1 Cr & Cb sample per 2x2 Y samples)
		YUVJ420P	= 12,	///< planar YUV 4:2:0, 12bpp, full scale (JPEG)
		YUYV422		= 1,	///< packed YUV 4:2:2, 16bpp, Y0 Cb Y1 Cr
		RGB24		= 2,    ///< packed RGB 8:8:8, 24bpp, RGBRGB...
		BGR24		= 3,	///< packed RGB 8:8:8, 24bpp, BGRBGR...
		GRAY8		= 8,	///<        Y        ,  8bpp
	};

	inline ID toID(const std::string& type) 
	{
		if (type =="YUV420P")
			return PixelFormat::YUV420P;
		if (type =="YUVJ420P")
			return PixelFormat::YUVJ420P;
		if (type =="YUYV422")
			return PixelFormat::YUYV422;
		if (type =="RGB24")
			return PixelFormat::RGB24;
		if (type =="BGR24")
			return PixelFormat::BGR24;
		if (type =="GRAY8")
			return PixelFormat::GRAY8;
		return PixelFormat::YUV420P;
	}
};


// ---------------------------------------------------------------------
//
#define DEFAULT_VIDEO_SAMPLE_RATE	(384000)				// 128 – 384 kbit/s – business-oriented videoconferencing quality using video compression
#define DEFAULT_VIDEO_BIT_RATE		(9000)					// Default value for RTP	
#define DEFAULT_VIDEO_PIXEL_FMT		PixelFormat::YUV420P


struct VideoCodec: public Codec 
{
	int width;
	int height;
	double fps;
	UInt32 pixfmt;
	
	VideoCodec();
	VideoCodec(/*UInt32 id, */const std::string& name, 
		int width = 0, int height = 0, double fps = 20, 
		int bitRate = DEFAULT_VIDEO_BIT_RATE, 
		int sampleRate = DEFAULT_VIDEO_SAMPLE_RATE, 
		UInt32 pixfmt = (UInt32)DEFAULT_VIDEO_PIXEL_FMT);		
	VideoCodec(/*UInt32 id, */
		const std::string& name, 
		const std::string& encoder, 
		int width = 0, int height = 0, double fps = 20, 
		int bitRate = DEFAULT_VIDEO_BIT_RATE, 
		int sampleRate = DEFAULT_VIDEO_SAMPLE_RATE, 
		UInt32 pixfmt = (UInt32)DEFAULT_VIDEO_PIXEL_FMT);	
	VideoCodec(const VideoCodec& r);

	virtual std::string toString() const;
	virtual void print(std::ostream& ost);
};


typedef std::list<Codec> CodecList;
typedef std::list<Codec*> CodecPList;


} // namespace Media
} // namespace Sourcey


#endif



	/*
	enum ID 
		/// The codec ID values match their constituent
		/// AV_CODEC_ID_XXX values in the FFmpeg library.
	{		
		Unknown		= 0,
		
		// Video
		Raw			= 14,
		H263		= 5,
		H263p		= 20,
		H264		= 28,
		MPEG1		= 1,
		MPEG2		= 2,
		MPEG4		= 13,
		MJPEG		= 8,
		FLV			= 22,
		
		// Audio
		PCM			= 65536,
		MP2			= 86016,
		MP3			= 86017,
		AAC			= 86018,
		AC3			= 86019,
		Vorbis		= 86021,
		FLAC		= 86028,
		NellyMoser	= 86050,
		Speex		= 86052
	};
	*/
	//bool matches(/*UInt32 id, */const std::string& name) const;
	//virtual std::string toSDP() const;

	//static UInt32 toID(const std::string& type);	
	//static std::string idString(UInt32 id);
	
	//UInt32 id;			// The codec ID. Unsigned integer for easy casting.