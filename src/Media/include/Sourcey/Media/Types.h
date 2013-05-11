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


#ifndef SOURCEY_MEDIA_Types_H
#define SOURCEY_MEDIA_Types_H


#include "Sourcey/Media/Config.h"
#include "Sourcey/IPacket.h"

//#include <opencv/cv.h>
//#include <opencv/highgui.h>


namespace Scy {
namespace Media {
	

struct MediaPacket: public DataPacket 
{
	double time;

	MediaPacket(unsigned char* data = NULL,
				int size = 0,
				double time = 0) :
		DataPacket(data, size),
		time(time) {};

	MediaPacket(const MediaPacket& r) : 
		DataPacket(r), 
		time(r.time)
	{
	}
		
	virtual ~MediaPacket() {};

	virtual IPacket* clone() const {
		return new MediaPacket(*this);
	}

	virtual const char* className() const { return "MediaPacket"; }
}; 


struct VideoPacket: public MediaPacket 
{
	int width;
	int height;

	VideoPacket(unsigned char* data = NULL,
				int size = 0,
				int width = 0,
				int height = 0,
				double time = 0) : //(double)clock() / CLOCKS_PER_SEC
		MediaPacket(data, size, time),
		width(width),
		height(height) {};

	VideoPacket(const VideoPacket& r) : 
		MediaPacket(r), 
		width(r.width), 
		height(r.height)
	{
	}

	virtual ~VideoPacket() {};

	virtual IPacket* clone() const {
		return new VideoPacket(*this);
	}	

	virtual const char* className() const { return "VideoPacket"; }
}; 


struct AudioPacket: public MediaPacket 
{
	AudioPacket(unsigned char* data = NULL,
				int size = 0,
				double time = 0) : //(double)clock() / CLOCKS_PER_SEC
		MediaPacket(data, size, time) {};

	AudioPacket(const AudioPacket& r) : 
		MediaPacket(r)
	{
	}

	virtual ~AudioPacket() {};

	virtual IPacket* clone() const {
		return new AudioPacket(*this);
	}	

	virtual const char* className() const { return "AudioPacket"; }
};


/*
template <class C>
static Callback<C, const MediaPacket, false> MediaCallback(C* object, void (C::*Method)(const MediaPacket&)) 
	/// Defines a callback for media packets.
{
	return Callback<C, const MediaPacket, false>(object, Method);
};

	
namespace Format 
	/// Specifies the available encoding/decoding media container formats.
{
}


struct VideoCodec 
{
	bool enabled;				// Signifies weather video is being used or not.
	Codec::ID codec;
	int width;
	int height;
	double fps;	
	PIX_FMT_ID pixelFmt;		// The input pixel format 
	int bitRate;
	int quality;				// For JPEG creation
	VideoCodec(Codec::ID codec,
				int width = 400,
				int height = 300,
				double fps = 25,
				PIX_FMT_ID pixelFmt = PIX_FMT_BGR24,
				int bitRate = 200 * 1024,
				int quality = 80) : 
		enabled(true),
		codec(codec),
		width(width),
		height(height),
		fps(fps),
		pixelFmt(pixelFmt),
		bitRate(bitRate),
		quality(quality) {}
	VideoCodec() : 
		enabled(false),
		codec(Codec::Unknown),
		width(400),
		height(300),
		fps(25),
		pixelFmt(PIX_FMT_BGR24),
		bitRate(200 * 1024),
		quality(80) {}
	VideoCodec(const VideoCodec& r) : 
		enabled(r.enabled),
		codec(r.codec), 
		width(r.width), 
		height(r.height), 
		fps(r.fps),  
		pixelFmt(r.pixelFmt), 
		bitRate(r.bitRate),
		quality(r.quality) {}
	VideoCodec* clone() { return new VideoCodec(*this); }
    VideoCodec& operator= (const VideoCodec& r) {
        if (this != &r) {
			enabled = r.enabled; 
			codec = r.codec; 
			width = r.width; 
			height = r.height; 
			fps = r.fps;  
			pixelFmt = r.pixelFmt; 
			bitRate = r.bitRate;
			quality = r.quality;
        }
        return *this;
    }
	std::string toString() const {			
		std::ostringstream ss;
		ss  << "VideoCodec["
			<< " Codec:" << codec
			<< " Width:" << width
			<< " Height:" << height
			<< " FPS:" << fps
			<< " Quality:" << quality
			<< " Pixel Format:" << pixelFmt
			<< " Bit Rate:" << bitRate
			<< " Enabled:" << enabled
			<< "]";
		return ss.str();
	}
};


struct AudioCodec 
{
	bool enabled;	// Signifies weather audio is being used or not.
	int bitRate;
	int channels;
	int sampleRate;	
	Codec::ID codec;
	AudioCodec(Codec::ID codec,
				int bitRate = 64000,
				int channels = 2,
				int sampleRate = 44100) :
		enabled(true),
		codec(codec),
		bitRate(bitRate),
		channels(channels),
		sampleRate(sampleRate) {}
	AudioCodec() :
		enabled(false),
		codec(Codec::Unknown),
		bitRate(64000),
		channels(2),
		sampleRate(44100) {}
	AudioCodec(const AudioCodec& r) : 
		enabled(r.enabled),
		codec(r.codec), 
		bitRate(r.bitRate), 
		channels(r.channels), 
		sampleRate(r.sampleRate) {}
	AudioCodec* clone() { return new AudioCodec(*this); }
    AudioCodec& operator= (const AudioCodec& r) {
        if (this != &r) {
			enabled = r.enabled; 
			codec = r.codec; 
			bitRate = r.bitRate;
			channels = r.channels; 
			sampleRate = r.sampleRate; 
        }
        return *this;
    }
	std::string toString() const {			
		std::ostringstream ss;
		ss  << "AudioCodec["
			<< " Codec:" << codec
			<< " Bit Rate:" << bitRate
			<< " Channels:" << channels
			<< " Sample Rate:" << sampleRate
			<< " Enabled:" << enabled
			<< "]";
		return ss.str();
	}
};


struct Format 
{
	VideoCodec video;
	AudioCodec audio;
	Format(const VideoCodec& video = VideoCodec(),
		        const AudioCodec& audio = AudioCodec()) :
		video(video),
		audio(audio) {}
	Format(const AudioCodec& audio) :
		audio(audio) {}
	Format(const Format& r) : 
		video(r.video), 
		audio(r.audio) {}
};
*/


} } // namespace Scy::Media


#endif // SOURCEY_MEDIA_Types_H






/*
inline const char* getFFmpegStringFromCodec::ID(UInt32 id) 
{
	switch(id)
	{
      case AAC:		return "aac"; break;
      case AC3:		return "ac3"; break;
      case MP3:		return "mp3"; break;
      case MP2:		return "mp2"; break;
      case Vorbis:	return "vorbis"; break;
      case FLAC:	return "flac"; break;
      //case WMA:		return "wma"; break;
      case PCM:		return "pcm"; break; // TODO: Should be sle or some such...
	}
	return "none";
}


std::ostream& operator<<( std::ostream& os, const Codec::ID& id )
{
   switch( id )
   {
      case AAC: os << "AAC"; break;
      case AC3: os << "AC3"; break;
      case MP3: os << "MP3"; break;
      case MP2: os << "MP2"; break;
      case Vorbis: os << "Vorbis"; break;
      case FLAC: os << "FLAC"; break;
      case WMA: os << "WMA"; break;
      case PCM: os << "PCM"; break;
   }
}
*/


/*
std::ostream& operator<<( std::ostream& os, const Codec::ID& id )
{
   switch( id )
   {
      case None: os << "None"; break;
      case Raw: os << "Raw"; break;
      case H263: os << "H263"; break;
      case H263p: os << "H263p"; break;
      case H264: os << "H264"; break;
      case MPEG1: os << "MPEG1"; break;
      case MPEG2: os << "MPEG2"; break;
      case MPEG4: os << "MPEG4"; break;
      case FLV: os << "FLV"; break;
   }
}
*/


/*
inline const char* getFFmpegStringFromCodec::ID(UInt32 id) 
{
	switch(id)
	{
      case Raw:		return "raw"; break;
      case H263:	return "h263"; break;
      case H263p:	return "h263p"; break;
      case H264:	return "h264"; break;
      case MPEG1:	return "mpeg"; break;
      case MPEG2:	return "mpeg2video"; break;
      case MPEG4:	return "mpeg4"; break;
      case MJPEG:	return "mjpeg"; break;
      case FLV:		return "flv"; break;
	}
	return "none";
}
*/
