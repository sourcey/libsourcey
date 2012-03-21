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


#include "Sourcey/Media/Codec.h"
#include <sstream>


using namespace std;


namespace Sourcey {
namespace Media {


// ---------------------------------------------------------------------
//
Codec::Codec() : 
	id(Unknown), name("Unknown"), sampleRate(0), bitRate(0), quality(0), enabled(false) 
{
}

Codec::Codec(UInt32 id, const string& name, int sampleRate, int bitRate, bool enabled) : //const string& name, 
	id(id), name(name), sampleRate(sampleRate), bitRate(bitRate), quality(0), enabled(enabled) 
{
}


Codec::Codec(UInt32 id, int sampleRate, int bitRate, bool enabled) :
	id(id), sampleRate(sampleRate), bitRate(bitRate), enabled(enabled) 
{
	// set the name if none was set by derived class.
	if (name.empty()) 
		name = idString(static_cast<ID>(id));
}


Codec::Codec(const Codec& r) :
	id(r.id),
	name(r.name),
	sampleRate(r.sampleRate),
	bitRate(r.bitRate),
	enabled(r.enabled) 
{
}

	
Codec::ID Codec::toID(const string& type) 
{
	// video
	if (type =="Raw")
		return Codec::Raw;
	if (type =="H263")
		return Codec::H263;
	if (type =="H263p")
		return Codec::H263p;
	if (type =="H264")
		return Codec::H264;
	if (type =="MPEG1")
		return Codec::MPEG1;
	if (type =="MPEG2")
		return Codec::MPEG2;
	if (type =="MPEG4")
		return Codec::MPEG4;
	if (type =="MJPEG")
		return Codec::MJPEG;
	if (type =="JPEG")
		return Codec::MJPEG;
	if (type =="FLV")
		return Codec::FLV;

	// audio
	if (type =="PCM")
		return Codec::PCM;
	if (type =="MP2")
		return Codec::MP2;
	if (type =="MP3")
		return Codec::MP3;
	if (type =="AAC")
		return Codec::AAC;
	if (type =="AC3")
		return Codec::AC3;
	if (type =="Vorbis")
		return Codec::Vorbis;
	if (type =="FLAC")
		return Codec::FLAC;
	if (type =="NellyMoser")
		return Codec::NellyMoser;
	if (type =="Speex")
		return Codec::Speex;	

	return Codec::Unknown;
}


string Codec::idString(UInt32 id) 
{
	switch(id)
	{
		case Unknown:	return "Unknown";

		case Raw:		return "Raw";
		case H263:		return "H263";
		case H263p:		return "H263p";
		case H264:		return "H264";
		case MPEG1:		return "MPEG1";
		case MPEG2:		return "MPEG2";
		case MPEG4:		return "MPEG4";
		case MJPEG:		return "MJPEG";
		case FLV:		return "FLV";
			
		case PCM:		return "PCM";
		case MP2:		return "MP2";
		case MP3:		return "MP3";
		case AAC:		return "AAC";
		case AC3:		return "AC3";
		case Vorbis:	return "Vorbis";
		case FLAC:		return "FLAC";
		case NellyMoser:return "NellyMoser";
		case Speex:		return "Speex";
	}
	return "Unknown";
}


/*
string Codec::name() const
{
	return Codec::idString(id);
}


bool Codec::matches(UInt32 id, const string& name) const 
{
	return this->id == id && this->name == name;
}
*/


string Codec::toString() const 
{
	ostringstream os;
	os << "Codec[" << id << ":" << name << ":" << sampleRate << ":" << enabled << "]";
	return os.str();
}


void Codec::print(std::ostream& ost)
{
	ost << "Codec["		
		<< "\n\tID: " << id
		<< "\n\tName: " << name
		<< "\n\tSample Rate: " << sampleRate
		<< "\n\tBit Rate: " << bitRate
		<< "\n\tQuality: " << quality
		<< "\n\tEnabled: " << enabled	
		<< "]";
}


/*
	UInt32 id;
	std::string name;	// The display name for this codec. May or may not match ID string.
	int sampleRate;		// The sampling rate or RTP clock rate.
	int bitRate;		// The bit rate to encode at.
	int quality;		// Optional quality value, variable range depending on codec.
	bool enabled;		// Weather or not the codec is available for use.

string Codec::toSDP() const 
{
	ostringstream os;
	os << "a=rtpmap:" 
		<< id << " " 
		<< name << "/" 
		<< sampleRate 
		<< "\r\n";
	return os.str();
}
*/


// ---------------------------------------------------------------------
//

AudioCodec::AudioCodec() : 
	Codec(id, name, DEFAULT_AUDIO_SAMPLE_RATE, DEFAULT_AUDIO_BIT_RATE, false), channels(DEFAULT_AUDIO_CHANNELS) 
{
}


AudioCodec::AudioCodec(UInt32 id, const std::string& name, int channels, int sampleRate, int bitRate) : 
	Codec(id, name, sampleRate, bitRate, true), channels(channels) 
{
}


AudioCodec::AudioCodec(const AudioCodec& r) :
	Codec(r.id, r.name, r.sampleRate, r.bitRate, r.enabled),
	channels(r.channels)
{
}


string AudioCodec::toString() const 
{
	ostringstream os;
	os << "AudioCodec[" << id << ":" << name << ":" << sampleRate << ":" << bitRate
		<< ":" << channels << ":" << enabled << "]";
	return os.str();
}


void AudioCodec::print(std::ostream& ost)
{
	ost << "AudioCodec["		
		<< "\n\tID: " << id
		<< "\n\tName: " << name
		<< "\n\tSample Rate: " << sampleRate
		<< "\n\tBit Rate: " << bitRate
		<< "\n\tChannels: " << channels
		<< "\n\tQuality: " << quality
		<< "\n\tEnabled: " << enabled	
		<< "]";
}


// ---------------------------------------------------------------------
//
VideoCodec::VideoCodec() : 
	Codec(id, name, DEFAULT_VIDEO_SAMPLE_RATE, DEFAULT_VIDEO_BIT_RATE, false), 
	width(0), height(0), fps(0) 
{
}


VideoCodec::VideoCodec(UInt32 id, const string& name, int width, int height, double fps, int sampleRate, int bitRate) : 
	Codec(id, name, sampleRate, bitRate, true), width(width), height(height), fps(fps) 
{
}

VideoCodec::VideoCodec(const VideoCodec& r) :
	Codec(r.id, r.name, r.sampleRate, r.bitRate, r.enabled),
	width(r.width),
	height(r.height),
	fps(r.fps),
	pixfmt(r.pixfmt)
{
}
	

string VideoCodec::toString() const 
{
	ostringstream os;
	os << "VideoCodec[" << id << ":" << name << ":" << width << ":" << height
		<< ":" << fps << ":" << enabled << "]";
	return os.str();
}


void VideoCodec::print(std::ostream& ost)
{
	ost << "VideoCodec["		
		<< "\n\tID: " << id
		<< "\n\tName: " << name
		<< "\n\tSample Rate: " << sampleRate
		<< "\n\tBit Rate: " << bitRate
		<< "\n\tWidth: " << width
		<< "\n\tHeight: " << height
		<< "\n\tFPS: " << fps
		<< "\n\tPixel Format: " << pixfmt
		<< "\n\tQuality: " << quality
		<< "\n\tEnabled: " << enabled	
		<< "]";
}


} // namespace Media 
} // namespace Sourcey