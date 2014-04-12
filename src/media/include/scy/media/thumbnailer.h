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


#ifndef SCY_MEDIA_Thumbnailer_H
#define SCY_MEDIA_Thumbnailer_H


#include "scy/base.h"
#include "scy/media/config.h"
#include "scy/media/avinputreader.h"

#include <iostream>
#include <string>
#include <list>


namespace scy {
namespace av {


struct Thumbnailer
{
	av::AVInputReader reader;
	av::VideoCodecEncoderContext encoder;
	std::string ifile;
	std::string ofile;
	double seek;
		
	Thumbnailer();
	~Thumbnailer() ;

	void init(const std::string& ifile, const std::string& ofile, int owidth = 0, int oheight = 0, double seek = 0.0);

	void grab() ;
		
	void onVideoPacket(void*, av::VideoPacket& packet);
	
	void saveFile(const std::string& path, const char* data, int size);
};


} // namespace av
} // namespace scy


#endif