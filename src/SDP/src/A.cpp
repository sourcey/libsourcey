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


#include "Sourcey/SDP/A.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Util.h"
#include <iostream>
#include <assert.h>


using namespace std;


namespace Scy {
namespace SDP { 


A::A(const string& src) : 
	Line(Line::A, 10) 
{
	assert(src.substr(0, 2) == "a=");
	string attr(src.substr(2, src.length()-2));
	Util::trim(attr);
	_attributes = attr;
}


A::~A() 
{
}


string A::attributes() 
{
	return _attributes;
}


void A::setAttributes(const string& attr) 
{
	_attributes =  attr;
}


string A::toString() 
{
	return "a=" + _attributes;
}


string A::attributeType() 
{
	size_t pos;	
	if ((pos = _attributes.find(":")) == string::npos)
		return "property";
	return _attributes.substr(0, pos);
}


string A::attributeValue() 
{
	if (attributeType() == "property")
		return _attributes;
	int pos = (int)_attributes.find(":");
	if (_attributes.length() <= pos + 1) {
		LogError() << "Invalid a field in SDP packet" << endl;
		return "";
	}
	return _attributes.substr( pos + 1, _attributes.length() - 2);
}


} // namespace Scy
} // namespace SDP 


