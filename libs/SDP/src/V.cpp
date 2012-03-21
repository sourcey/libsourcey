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


#include "Sourcey/Logger.h"
#include "Sourcey/SDP/V.h"


using namespace std;



namespace Sourcey {
namespace SDP { 


V::V(const string& src) :
	Line(Line::V, 1) 
{
	size_t len = src.length();
	if (src.substr(0, 2) != "v=") {
		Log("error") << "ERROR: SDP line does not start with <v=>" << std::endl;
	}

	_version = Util::atoi(src.substr(2, src.length()).c_str());
}


V::V(unsigned int ver) :
	Line(Line::V, 1) 
{
	_version = ver;
}


unsigned int V::version() 
{
	return _version;
}


void V::setVersion(unsigned int ver) 
{
	_version = ver;
}


string V::toString() 
{
	return "v=" + Util::itoa(_version);
}


} // namespace SDP 
} // namespace Sourcey

