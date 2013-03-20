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


#include "Sourcey/SDP/Candidate.h"
#include "Sourcey/Logger.h"
//#include "Sourcey/Net/Address.h"
//#include <iostream>


using namespace std;


namespace Sourcey {
namespace SDP { 


Candidate::Candidate(const string& src) : 
	A(src) 
{
	// candidate-attribute   = "candidate" ":" foundation SP component-id SP
	//                         transport SP
	//                         priority SP
	//                         connection-address SP	;from RFC 4566
	//                         port						;port from RFC 4566
	//                         SP cand-type
	//                         [SP rel-address]
	//                         [SP rel-port]
	//                         *(SP extension-att-name SP
	//                              extension-att-value)
	//
	// Examples:
	// a=candidate:Hc0a80265 1 UDP 1694498815 192.168.2.101 16052 typ host
	// a=candidate:Sc0a80266 1 UDP 2130706431 1.1.1.1 16902 typ srflx raddr 192.168.2.101 rport 16052

	size_t len = src.length();
	if (src.substr(0, 11) != "a=candidate") {
		LogError() << "ERROR: Contact sdp line does not start with <a=candidate>" << std::endl;
	}
	size_t i = 12;
	while (src[i] == ' ' && i < len)
		i++;

	// foundation
	while (src[i] != ' ' && i < len)
		_foundation += src[i++];
	while (src[i] == ' ' && i < len)
		i++;

	// component-id
	string componentIDstr = "";
	while (src[i] != ' ' && i < len)
		componentIDstr += src[i++];
	_componentID = Util::atoi(componentIDstr.c_str());
	while (src[i] == ' ' && i < len)
		i++;

	// transport
	while (src[i] != ' ' && i < len)
		_transport += src[i++];
	while (src[i] == ' ' && i < len)
		i++;

	// priority
	string prioritystr = "";
	while (src[i] != ' ' && i < len)
		prioritystr += src[i++];
	_priority = Util::atoi(prioritystr.c_str());
	while (src[i] == ' ' && i < len)
		i++;

	// connection-address
	while (src[i] != ' ' && i < len)
		_address += src[i++];
	while (src[i] == ' ' && i < len)
		i++;

	// port
	string portstr = "";
	while (src[i] != ' ' && i < len)
		portstr += src[i++];
	_port = Util::atoi(portstr.c_str());
	while (src[i] == ' ' && i < len)
		i++;

	// skipping <typ>
	while (src[i] != ' ' && i < len)
		i++;
	while (src[i] == ' ' && i < len)
		i++;

	// candidateType
	while (src[i] != ' ' && 
		src[i] != '\r' && 
		src[i] != '\n' && i < len)
		_candidateType += src[i++];
	while (src[i] == ' ' && i < len)
		i++;

	if (_candidateType != "host") {

		// skipping <raddr>
		while (src[i] != ' ' && i < len)
			i++;
		while (src[i] == ' ' && i < len)
			i++;

		// rel-addr
		while (src[i] != ' ' && i < len)
			_relAddr += src[i++];
		while (src[i] == ' ' && i < len)
			i++;

		// skipping <rport>
		while (src[i] != ' ' && i < len)
			i++;
		while (src[i] == ' ' && i < len)
			i++;

		// rel-port
		string relportstr = "";
		while (src[i] != ' ' && i < len)
			relportstr += src[i++];
		_relPort = Util::atoi(relportstr.c_str());
		while (src[i] == ' ' && i < len)
			i++;
	}
}


Candidate::~Candidate() 
{	
}


string Candidate::foundation() const 
{
	return _foundation;
}


void Candidate::setFoundation(const string& foundation) 
{
	_foundation = foundation;
}


unsigned int Candidate::componentID() const 
{
	return _componentID;
}


void Candidate::setComponentID(unsigned int componentID) 
{
	_componentID = componentID;
}


string Candidate::transport() const 
{
	return _transport;
}


void Candidate::setTransport(const string& transport) 
{
	_transport = transport;
}


unsigned int Candidate::priority() const 
{
	return _priority;
}


void Candidate::setPriority(unsigned int priority) 
{
	_priority = priority;
}


string Candidate::address() const 
{
	return _address;
}


void Candidate::setAddr(const string& address) 
{
	_address = address;
}


unsigned int Candidate::port() const 
{
	return _port;
}


void Candidate::setPort(unsigned int port) 
{
	_port = port;
}


string Candidate::candidateType() const 
{
	return _candidateType;
}


void Candidate::setCandidateType(const string& candidateType) 
{
	_candidateType = candidateType;
}


string Candidate::relAddr() const 
{
	return _relAddr;
}


void Candidate::setRelAddr(const string& relAddr) 
{
	_relAddr = relAddr;
}


unsigned int Candidate::relPort() const 
{
	return _relPort;
}


void Candidate::setRelPort(unsigned int relPort) 
{
	_relPort = relPort;
}


string Candidate::toString() 
{
	string ret = "a=candidate:";
	if (_foundation.empty())
		ret += "-";
	else
		ret += _foundation;

	ret += " ";
	if (Util::itoa(_componentID).empty())
		ret += "0";
	else
		ret += Util::itoa(_componentID);

	ret += " ";
	if (_transport.empty())
		ret += "-";
	else
		ret += _transport;

	ret += " ";
	if (Util::itoa(_priority).empty())
		ret += "0";
	else
		ret += Util::itoa(_priority);

	ret += " ";
	if (_address.empty())
		ret += "0.0.0.0";
	else
		ret += _address;

	ret += " ";
	//if (Util::itoa(_port).empty())
	//	ret += "0";
	//else
	ret += Util::itoa(_port);

	
	ret += " typ ";
	if (_candidateType.empty())
		_candidateType += "-";
	else
		ret += _candidateType;


	if (_candidateType != "host") {

		// rel-addr
		ret += " raddr ";
		if (_relAddr.empty())
			ret += "0.0.0.0";
		else
			ret += _relAddr;

		// rel-port
		ret += " rport ";
		//if (Util::itoa(_relPort).empty())
		//	ret += "0";
		//else
			ret += Util::itoa(_relPort);
	}
	
	return ret;
}


} // namespace Sourcey
} // namespace SDP 