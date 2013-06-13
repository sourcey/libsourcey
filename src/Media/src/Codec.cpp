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


#include "Sourcey/Media/Codec.h"
#include <sstream>


using namespace std;


namespace Scy {
namespace Media {


// ---------------------------------------------------------------------
Codec::Codec() : 
	name("Unknown"), sampleRate(0), bitRate(0), quality(0), enabled(false) 
{
}

Codec::Codec(const string& name, int sampleRate, int bitRate, bool enabled) :
	name(name), sampleRate(sampleRate), bitRate(bitRate), quality(0), enabled(enabled) 
{
}


Codec::Codec(const string& name, const string& encoder, int sampleRate, int bitRate, bool enabled) :
	name(name), encoder(encoder), sampleRate(sampleRate), bitRate(bitRate), enabled(enabled) 
{
}


Codec::Codec(const Codec& r) :
	name(r.name),
	encoder(r.encoder),
	sampleRate(r.sampleRate),
	bitRate(r.bitRate),
	enabled(r.enabled) 
{
}


string Codec::toString() const 
{
	ostringstream os;
	os << "Codec[" << name << ":" << encoder << ":" << sampleRate << ":" << enabled << "]";
	return os.str();
}


void Codec::print(ostream& ost)
{
	ost << "Codec["		
		//<< "\n\tID: " << id
		<< "\n\tName: " << name
		<< "\n\tEncoder: " << encoder
		<< "\n\tSample Rate: " << sampleRate
		<< "\n\tBit Rate: " << bitRate
		<< "\n\tQuality: " << quality
		<< "\n\tEnabled: " << enabled	
		<< "]";
}


// ---------------------------------------------------------------------
AudioCodec::AudioCodec() : 
	Codec("Unknown", DEFAULT_AUDIO_SAMPLE_RATE, DEFAULT_AUDIO_BIT_RATE, false), 
	channels(DEFAULT_AUDIO_CHANNELS), sampleFmt(DEFAULT_AUDIO_SAMPLE_FMT) 
{
}


AudioCodec::AudioCodec(const string& name, int channels, int sampleRate, int bitRate, const char* sampleFmt) : 
	Codec(name, encoder, sampleRate, bitRate, true), channels(channels), sampleFmt(sampleFmt) 
{
}


AudioCodec::AudioCodec(const string& name, const string& encoder, int channels, int sampleRate, int bitRate, const char* sampleFmt) : 
	Codec(name, encoder, sampleRate, bitRate, true), channels(channels), sampleFmt(sampleFmt) 
{
}


AudioCodec::AudioCodec(const AudioCodec& r) :
	Codec(r.name, r.encoder, r.sampleRate, r.bitRate, r.enabled),
	channels(r.channels), sampleFmt(r.sampleFmt) 
{
}


string AudioCodec::toString() const 
{
	ostringstream os;
	os << "AudioCodec[" << name << ":" << encoder << ":" << sampleRate << ":" << bitRate
		<< ":" << channels << ":" << sampleFmt << ":" << enabled << "]";
	return os.str();
}


void AudioCodec::print(ostream& ost)
{
	ost << "AudioCodec["
		<< "\n\tName: " << name
		<< "\n\tEncoder: " << encoder
		<< "\n\tSample Rate: " << sampleRate
		<< "\n\tBit Rate: " << bitRate
		<< "\n\tChannels: " << channels
		<< "\n\tSample Fmt: " << sampleFmt
		<< "\n\tQuality: " << quality
		<< "\n\tEnabled: " << enabled	
		<< "]";
}


// ---------------------------------------------------------------------
VideoCodec::VideoCodec() : 
	Codec("Unknown", DEFAULT_VIDEO_SAMPLE_RATE, DEFAULT_VIDEO_BIT_RATE, false), 
	width(0), height(0), fps(0), pixelFmt(DEFAULT_VIDEO_PIXEL_FMT) 
{
}


VideoCodec::VideoCodec(const string& name, int width, int height, double fps, int sampleRate, int bitRate, const char* pixelFmt) : 
	Codec(name, sampleRate, bitRate, true), 
	width(width), height(height), fps(fps), pixelFmt(pixelFmt) 
{
}


VideoCodec::VideoCodec(const string& name, const string& encoder, int width, int height, double fps, int sampleRate, int bitRate, const char* pixelFmt) : 
	Codec(name, encoder, sampleRate, bitRate, true), 
	width(width), height(height), fps(fps), pixelFmt(pixelFmt) 
{
}


VideoCodec::VideoCodec(const VideoCodec& r) :
	Codec(r.name, r.encoder, r.sampleRate, r.bitRate, r.enabled),
	width(r.width), height(r.height), fps(r.fps), pixelFmt(r.pixelFmt)
{
}
	

string VideoCodec::toString() const 
{
	ostringstream os;
	os << "VideoCodec[" << name << ":" << encoder << ":" << width << ":" << height
		<< ":" << fps << ":" << pixelFmt << ":" << enabled << "]";
	return os.str();
}


void VideoCodec::print(ostream& ost)
{
	ost << "VideoCodec["
		<< "\n\tName: " << name
		<< "\n\tEncoder: " << encoder
		<< "\n\tSample Rate: " << sampleRate
		<< "\n\tBit Rate: " << bitRate
		<< "\n\tWidth: " << width
		<< "\n\tHeight: " << height
		<< "\n\tFPS: " << fps
		<< "\n\tPixel Format: " << pixelFmt
		<< "\n\tQuality: " << quality
		<< "\n\tEnabled: " << enabled	
		<< "]";
}


} // namespace Media 
} // namespace Scy



	/*id(Unknown), */
	
	
	
	// set the name if none was set by derived class.
	//if (name.empty()) 
	//	name = idString(id);
		//name = idString(static_cast<ID>(id));
	//id(r.id),
	
	/*
UInt32 Codec::toID(const string& type) 
{
	// TODO: Use FFmpeg is available?

	// video
	if (type == "Raw")
		return Codec::Raw;
	if (type == "H263")
		return Codec::H263;
	if (type == "H263p")
		return Codec::H263p;
	if (type == "H264")
		return Codec::H264;
	if (type == "MPEG1")
		return Codec::MPEG1;
	if (type == "MPEG2")
		return Codec::MPEG2;
	if (type == "MPEG4")
		return Codec::MPEG4;
	if (type == "MJPEG")
		return Codec::MJPEG;
	if (type == "JPEG")
		return Codec::MJPEG;
	if (type == "FLV")
		return Codec::FLV;

	// audio
	if (type == "PCM")
		return Codec::PCM;
	if (type == "MP2")
		return Codec::MP2;
	if (type == "MP3")
		return Codec::MP3;
	if (type == "AAC")
		return Codec::AAC;
	if (type == "AC3")
		return Codec::AC3;
	if (type == "Vorbis")
		return Codec::Vorbis;
	if (type == "FLAC")
		return Codec::FLAC;
	if (type == "NellyMoser")
		return Codec::NellyMoser;
	if (type == "Speex")
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
	// return avcodec_get_name(id);
}
*/


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