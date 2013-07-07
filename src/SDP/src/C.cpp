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


#include "Sourcey/SDP/C.h"
#include "Sourcey/Logger.h"
#include <iostream>


using namespace std;


namespace scy {
namespace SDP { 


C::C(const string& src) : 
	Line(Line::C, 4) 
{
	size_t len = src.length();
	if (src.substr(0, 2) != "c=") {
		errorL() << "ERROR: Contact sdp line does not start with <c=>" << std::endl;
	}
	size_t i = 2;
	while (src[i] == ' ' && i < len)
		i++;

	while (src[i] != ' ' && i < len)
		_netType += src[i++];
	while (src[i] == ' ' && i < len)
		i++;

	while (src[i] != ' ' && i < len)
		_addressType += src[i++];
	while (src[i] == ' ' && i < len)
		i++;

	while (src[i] != ' ' && i < len)
		_address += src[i++];

}


C::C(const string& netType, string addressType, string address) : Line(Line::C, 4) 
{
	_netType = netType;
	_addressType = addressType;
	_address = address;
}


C::~C() 
{
}


string C::netType() const 
{
	return _netType;
}


void C::setNetType(const string& netType) 
{
	_netType = netType;
}


string C::addrType() const 
{
	return _addressType;
}


void C::setAddrType(const string& addressType) 
{
	_addressType = addressType;
}


string C::address() const 
{
	return _address;
}


void C::setAddr(const string& a) 
{
	_address = a;
}


string C::toString() 
{
	string ret = "c=";
	if (_netType.empty())
		ret += "-";
	else
		ret += _netType;
	ret += " ";
	if (_addressType.empty())
		ret += "-";
	else
		ret += _addressType;
	ret += " ";
	if (_address.empty())
		ret += "-";
	else
		ret += _address;
	return ret;
}


} // namespace SDP 
} // namespace scy