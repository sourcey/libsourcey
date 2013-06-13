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


#include "Sourcey/ICE/Candidate.h"
#include "Sourcey/Util.h"
#include <iostream>


//using namespace Poco;
//using namespace Poco::Net;
using namespace std;
using namespace Sourcey;


namespace Sourcey {
namespace ICE { 


Candidate::Candidate() : 
	_type(Candidate::Host), 
	_transportAddr(Net::Address()), 
	_componentID(0), 
	_transport(Candidate::Udp),
	_priority(0)
{
}


Candidate::Candidate(Type type, 
					 int componentID, 
					 Transport transport, 
					 const Net::Address& transportAddr) : 
	_type(type), 
	_componentID(componentID), 
	_transport(transport),
	_transportAddr(transportAddr), 
	_priority(0)
{
}


Candidate::Candidate(const Candidate& src) 
{
	_type = src.type();
	_transportAddr = src.transportAddr();
	_foundation = src.foundation();
	_componentID = src.componentID();
	_transport = src.transport();
	_priority = src.priority();
}


Candidate& Candidate::operator = (const Candidate& src) 
{
	_type = src.type();
	_transportAddr = src.transportAddr();
	_foundation = src.foundation();
	_componentID = src.componentID();
	_transport = src.transport();
	_priority = src.priority();
	return *this;
}


Candidate::~Candidate() 
{
}


std::string Candidate::typeToString(UInt32 type) 
{
	switch (type) {
	case Candidate::Host:
		return "host";
	case Candidate::ServerReflexive:
		return "srflx";
	case Candidate::PeerReflexive:
		return "prflx";
	case Candidate::Relayed:
		return "relay";
	}
	return "unknown";
}


std::string Candidate::typeString() const 
{	
	return typeToString(_type);
}


std::string Candidate::transportToString(UInt32 transport) 
{
	switch (transport) {
	case Candidate::Udp:
		return "UDP";
	case Candidate::TcpSo:
		return "TCP-SO";
	case Candidate::TcpAct:
		return "TCP-ACT";
	case Candidate::TcpPass:
		return "TCP-PASS";
	case Candidate::TlsSo:
		return "TLS-SO";
	case Candidate::TlsAct:
		return "TLS-ACT";
	case Candidate::TlsPass:
		return "TLS-PASS";
	}
	return "unknown";
}


Candidate::Type Candidate::stringToType(const std::string& type) 
{
   if (type == "host")
      return Candidate::Host;
   else if (type == "srflx")
      return Candidate::ServerReflexive;
   else if (type == "prflx")
      return Candidate::PeerReflexive;
   else if (type == "relay")
      return Candidate::Relayed;
   return Candidate::NoType;
}


Candidate::Transport Candidate::stringToTransport(const std::string& transport) 
{
   if (transport == "UDP")
      return Candidate::Udp;
   else if (transport == "TCP-SO")
      return Candidate::TcpSo;
   else if (transport == "TCP-ACT")
      return Candidate::TcpAct;
   else if (transport == "TCP-PASS")
      return Candidate::TcpPass;
   else if (transport == "TLS-SO")
      return Candidate::TlsSo;
   else if (transport == "TLS-ACT")
      return Candidate::TlsAct;
   else if (transport == "TLS-PASS")
      return Candidate::TlsPass;
   return Candidate::NoTransport;
}


std::string Candidate::transportString() const 
{
	return transportToString(_transport);
}


bool Candidate::valid() const 
{
	return _foundation.size() &&
		_componentID != 0 &&
		_transport != 0 &&
		_priority != 0 &&
		_type != 0 &&
		transportIP() != "0.0.0.0" &&
		transportPort() != 0;
}


std::string Candidate::toString() const 
{
    std::stringstream ost;
	ost << "Candidate["
		<< typeString() << ":"
		<< transportString() << ":"
		<< transportIP() << ":"
		<< transportPort() << ":"
		<< foundation() << ":"
		<< priority() << ":"
		<< componentID()
		<< "]";
	return ost.str();
	return "";
}


void Candidate::printLog(std::ostream& ost) const 
{
	ost << "["
		<< className()
		<< ":"
		<< this
		<< ":"
		<< _transportAddr.toString()
		<< "] ";
}


// Compares similarity (for foundation comparison)
bool Candidate::operator ^= (const Candidate& c) const 
{
	return 
		type() == c.type() && 
		transportIP() == c.transportIP() && 
		transport() == c.transport();
}


// Compares equality
bool Candidate::operator == (const Candidate& c) const 
{
	return
		transportAddr() == c.transportAddr() && 
		componentID()== c.componentID() && 
		transport() == c.transport();
}


bool Candidate::operator != (const Candidate& c) const 
{
	return !(*this == c);
}


} // namespace Sourcey
} // namespace ICE 