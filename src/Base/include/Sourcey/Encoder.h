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
// This file uses the public domain libb64 library: http://libb64.sourceforge.net/
//


#ifndef SOURCEY_Encoder_H
#define SOURCEY_Encoder_H


#include "Sourcey/Types.h"


namespace scy {
	

class Encoder 
{	
public:
	Encoder() {}
	virtual ~Encoder() {}
	virtual std::size_t encode(const char* inbuf, std::size_t nread, char* outbuf) = 0;
	virtual std::size_t finalize(char* outbuf) { return 0; }
};
	

} // namespace scy


#endif // SOURCEY_Encoder_H
