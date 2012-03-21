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


#include "Sourcey/SDP/O.h"
#include "Sourcey/Logger.h"


using namespace std;


namespace Sourcey {
namespace SDP { 


O::O(const string& src) : Line(Line::O, 2) 
{
	size_t len = src.length();
	if (src.substr(0, 2) != "o=") {
		Log("error") << "ERROR: SDP line does not start with <o=>:" << src << std::endl;
	}
	size_t i = 2;
	while (src[i] == ' ' && i < len)
		i++;
	
	while (src[i] != ' ' && i < len)
		_username += src[i++];

	while (src[i] == ' ' && i < len)
		i++;

	while (src[i] != ' ' && i < len)
		_sessionID += src[i++];

	while (src[i] == ' ' && i < len)
		i++;
	
	while (src[i] != ' ' && i < len)
		_version += src[i++];

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


O::O(const string& username, string sessionID, string version, string netType, string addressType, string address) : Line(Line::O, 2) {
	_username = username;
	_sessionID = sessionID;
	_version = version;
	_netType = netType;
	_addressType = addressType;
	_address = address;
}


O::~O() {
}


string O::username() {
	return _username;
}


void O::setUsername(const string& username) {
	_username = username;
}


string O::sessionId() {
	return _sessionID;
}


void O::setSessionId(const string& sessionID) {
	_sessionID = sessionID;
}


string O::version() {
	return _version;
}


void O::setVersion(const string& version) {
	_version = version;
}


string O::netType() {
	return _netType;
}


void O::setNetType(const string& netType) {
	_netType = netType;
}


string O::addrType() {
	return _addressType;
}


void O::setAddrType(const string& addressType) {
	_addressType = addressType;
}


string O::address() {
	return _address;
}


void O::setAddr(const string& address) {
	_address = address;
}


string O::toString() {
	string ret = "o=";
	if (_username.empty())
		ret += "-";
	else
		ret += _username;
	ret += " ";
	if (_sessionID.empty())
		ret += "-";
	else
		ret += _sessionID;
	ret += " ";
	if (_version.empty())
		ret += "-";
	else
		ret += _version;
	ret += " ";
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


} // namespace Sourcey
} // namespace SDP 