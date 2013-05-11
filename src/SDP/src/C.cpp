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


#include "Sourcey/SDP/C.h"
#include "Sourcey/Logger.h"
#include <iostream>


using namespace std;


namespace Scy {
namespace SDP { 


C::C(const string& src) : 
	Line(Line::C, 4) 
{
	size_t len = src.length();
	if (src.substr(0, 2) != "c=") {
		LogError() << "ERROR: Contact sdp line does not start with <c=>" << std::endl;
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
} // namespace Scy