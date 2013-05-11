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


#ifndef SOURCEY_SOCKET_ADDRESS_H
#define SOURCEY_SOCKET_ADDRESS_H


#include "Sourcey/Base.h"
#include "Poco/Net/SocketAddress.h"

#include <vector>
#include "assert.h"


namespace Scy {
namespace Net {


typedef Poco::Net::IPAddress IP;


class Address: public Poco::Net::SocketAddress
	/// Adds a few important methods which are missing from
	/// Poco's Address implementation.
{
public:
	Address() : 
	  Poco::Net::SocketAddress() {};
	Address(const Address& addr) : 
	  Poco::Net::SocketAddress(addr) {};
	Address(const Poco::Net::SocketAddress& addr) : 
	  Poco::Net::SocketAddress(addr) {};
	Address(const IP& host, unsigned short port) : 
	  Poco::Net::SocketAddress(host, port) {};
	Address(const std::string& host, unsigned short port) : 
	  Poco::Net::SocketAddress(host, port) {};
	Address(const std::string& host, const std::string& port) : 
	  Poco::Net::SocketAddress(host, port) {};	
	Address(const struct sockaddr* addr, int length) : 
	  Poco::Net::SocketAddress(addr, length) {};

	void setIP(const IP& host) 
	{
		setIP(host.toString());
	}

	void setPort(unsigned short port) 
	{
		init(host(), port);
	}

	void setIP(const std::string& host) 
	{
		init(host, port());
	}

    //int stunType() {
	//	return af() == AF_INET ? 0x01 : 0x02;
	//}

    const sockaddr_in* addr4() 
	{
		assert(af() == AF_INET);
		return reinterpret_cast<const struct sockaddr_in*>(addr());
	}

    const sockaddr_in6* addr6()	
	{
		assert(af() == AF_INET6);
		return reinterpret_cast<const struct sockaddr_in6*>(addr());
	}

	bool valid() const 
	{
		return port() != 0;
	}

	bool operator == (const Address& r) const 
	{
		if (af() == r.af() &&
			host() == r.host() &&
			port() == r.port())
			return true;
		return false;
	}

	bool operator < (const Address& r) const 
	{
		if (port() < r.port())
			return true;
		return false;
	}
	
    friend std::ostream& operator << (std::ostream& stream, const Address& addr) 
	{
		stream << addr.toString();
		return stream;
    }
};


typedef std::vector<Address*> AddressList;


} } // namespace Scy::Net


#endif // SOURCEY_SOCKET_ADDRESS_H
