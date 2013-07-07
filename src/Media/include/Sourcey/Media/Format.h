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


#ifndef SOURCEY_MEDIA_Format_H
#define SOURCEY_MEDIA_Format_H


#include "Sourcey/Media/Codec.h"

#include <iostream>
#include <string>
#include <vector>


namespace scy {
namespace av {


struct Format 
	/// Defines a media container format which is available through the
	/// Format Registry for encoding/decoding. A format specifies preferred
	/// default values for each codec.
{	
	enum Type 
	{
		None		= 0,
		Video		= 1,	/// video only
		Audio		= 2,	/// audio only
		Multiplex	= 3		/// both video & audio
	};

	//
	// Base Format Variables
	//
	std::string name;		/// The display name of this format.
	std::string id;			/// The short name of this format.
	
	VideoCodec video;		/// The video codec.
	AudioCodec audio;		/// The audio codec.

	int priority;			/// The priority this format will be displayed on the list.

	//
	// Ctors/Dtors
	//
	Format();

	Format(const std::string& name, const char* id, 
		const VideoCodec& video, const AudioCodec& audio, int priority = 0);
		/// Multiplex format constructor
	
	Format(const std::string& name, const char* id, 
		const VideoCodec& video, int priority = 0);
		/// Video only format constructor
	
	Format(const std::string& name, const char* id, 
		const AudioCodec& audio, int priority = 0);
		/// Audio only format constructor

	Format(const Format& r);
		
	Type type() const;
		/// The format type.

	virtual std::string toString() const;
	virtual void print(std::ostream& ost);

	static bool preferable(const Format& first, const Format& second) {
		return first.priority > second.priority;
	}
};


typedef std::vector<Format> FormatList;
typedef std::vector<Format*> FormatPList;


} // namespace av
} // namespace scy


#endif




	/*
	enum ID 
	{
		Unknown		= 0,
		Raw			= 1,

		/// Video
		MP4			= 2,
		FLV			= 3,
		OGG			= 4,
		AVI			= 5,
		MJPEG		= 6,
		MPNG		= 7,

		/// Audio
		M4A			= 20,
		MP3			= 21
	};
	*/
	//
	/// Methods
	//	
	//virtual std::string extension() const;
		/// Returns the file extension for this format.
		/// The default implementation just transforms the 
		/// id string to lowercase.
		/// TODO: Consider removing or making a member variable

	//virtual std::string name() const;
		/// Returns a string representation of the Codec name.
		/// The default implementation uses idToName.
	

	//virtual std::string encoderName() const;
		/// Returns the encoder name for this format.
		/// The default implementation uses idToEncoderName.
		/// This function should return the short name
		/// for use with av_guess_format.
	//static ID toID(const std::string& name);	
	//static std::string idString(const char* id);
	//static std::string encoderName(const char* id);
	//static ID nameToID(const std::string& name);
	//static std::string idToName(const char* id);
	//static std::string idToEncoderName(const char* id);