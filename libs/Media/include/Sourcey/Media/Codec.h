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
	enum ID 
		/// The media ID values match their constituent CODEC_ID_XXX
		/// in the ffmpeg library.
	{		
		Unknown		= 0,
		
		// video
		Raw			= 14,
		H263		= 5,
		H263p		= 20,
		H264		= 28,
		MPEG1		= 1,
		MPEG2		= 2,
		MPEG4		= 13,
		MJPEG		= 8,
		FLV			= 22,
		
		// audio
		PCM			= 65536,
		MP2			= 86016,
		MP3			= 86017,
		AAC			= 86018,
		AC3			= 86019,
		Vorbis		= 86021,
		FLAC		= 86030,
		NellyMoser	= 86052,
		Speex		= 86054
	};

	//
	// Base Codec Variables
	//
	UInt32 id;
	std::string name;	// The display name for this codec. May or may not match ID string.
	int sampleRate;		// The sampling rate or RTP clock rate.
	int bitRate;		// The bit rate to encode at.
	int quality;		// Optional quality value, variable range depending on codec.
	bool enabled;		// Weather or not the codec is available for use.

	//
	// Ctors/Dtors
	//
	Codec();
	Codec(UInt32 id, const std::string& name, int sampleRate = 0, int bitRate = 0, bool enabled = true);
	Codec(UInt32 id, int sampleRate = 0, int bitRate = 0, bool enabled = true);
	Codec(const Codec& r);
		
	//
	// Methods
	//
	//bool matches(UInt32 id, const std::string& name) const;
	
	virtual std::string toString() const;
	virtual void print(std::ostream& ost);
	//virtual std::string toSDP() const;

	static Codec::ID toID(const std::string& type);	
	static std::string idString(UInt32 id);
};


// ---------------------------------------------------------------------
//
struct AudioCodec: public Codec 
{
	int channels;
	
	AudioCodec();
	AudioCodec(UInt32 id, const std::string& name, 
		int channels = DEFAULT_AUDIO_CHANNELS, 
		int sampleRate = DEFAULT_AUDIO_SAMPLE_RATE, 
		int bitRate = DEFAULT_AUDIO_BIT_RATE);
	AudioCodec(const AudioCodec& r);

	virtual std::string toString() const;
	virtual void print(std::ostream& ost);
};
	

// ---------------------------------------------------------------------
//
namespace PixelFormat 
	/// Specifies pixel formats for encoding/decoding video.
{
	enum ID 
	{
		YUV420P		= 0,	///< planar YUV 4:2:0, 12bpp, (1 Cr & Cb sample per 2x2 Y samples)
		YUVJ420P	= 12,	///< planar YUV 4:2:0, 12bpp, full scale (JPEG)
		YUYV422		= 1,	///< packed YUV 4:2:2, 16bpp, Y0 Cb Y1 Cr
		RGB24		= 2,    ///< packed RGB 8:8:8, 24bpp, RGBRGB...
		BGR24		= 3,	///< packed RGB 8:8:8, 24bpp, BGRBGR...
		GRAY8		= 8,	 ///<        Y        ,  8bpp
	};
	
	inline PixelFormat::ID toID(const std::string& type) 
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
}


struct VideoCodec: public Codec 
{
	int width;
	int height;
	double fps;
	PixelFormat::ID pixfmt;	
	
	VideoCodec();
	VideoCodec(UInt32 id, const std::string& name, 
		int width = 0, int height = 0, double fps = 20, 
		int bitRate = DEFAULT_VIDEO_BIT_RATE, 
		int sampleRate = DEFAULT_VIDEO_SAMPLE_RATE);		
	VideoCodec(const VideoCodec& r);

	virtual std::string toString() const;
	virtual void print(std::ostream& ost);
};


typedef std::list<Codec> CodecList;
typedef std::list<Codec*> CodecPList;


} // namespace Media
} // namespace Sourcey


#endif