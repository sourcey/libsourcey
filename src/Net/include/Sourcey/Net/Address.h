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


#ifndef SOURCEY_Net_Address_H
#define SOURCEY_Net_Address_H


#include "Sourcey/Net/Types.h"


namespace scy {
namespace net {
	

class AddressBase;
class Address
	/// This class represents an internet (IP) endpoint/socket
	/// address. The address can belong either to the
	/// IPv4 or the IPv6 address family and consists of a
	/// host address and a port number.
{
public:
	enum Family
		/// Possible address families for IP addresses.
	{
		IPv4,
		IPv6
	};

	Address();
		/// Creates a wildcard (all zero) IPv4 Address.

	Address(const std::string& host, UInt16 port);
		/// Creates a Address from an IP address and a port number.
		///
		/// The IP address must either be a domain name, or it must
		/// be in dotted decimal (IPv4) or hex string (IPv6) format.

	Address(const Address& addr);
		/// Creates a Address by copying another one.

	Address(const struct sockaddr* addr, socklen_t length);
		/// Creates a Address from a native socket address.
	
	Address(const std::string& host, const std::string& port);
		/// Creates a Address from an IP address and a 
		/// service name or port number.
		///
		/// The IP address must either be a domain name, or it must
		/// be in dotted decimal (IPv4) or hex string (IPv6) format.
		///
		/// The given port must either be a decimal port number, or 
		/// a service name.

	explicit Address(const std::string& hostAndPort);
		/// Creates a Address from an IP address or host name and a
		/// port number/service name. Host name/address and port number must
		/// be separated by a colon. In case of an IPv6 address,
		/// the address part must be enclosed in brackets.
		///
		/// Examples:
		///     192.168.1.10:80
		///     [::ffff:192.168.1.120]:2040
		///     www.sourcey.com:8080

	~Address();
		/// Destroys the Address.

	Address& operator = (const Address& addr);
		/// Assigns another Address.

	void swap(Address& addr);
		/// Swaps the Address with another one.

	std::string host() const;
		/// Returns the host IP address.

	UInt16 port() const;
		/// Returns the port number.

	socklen_t length() const;
		/// Returns the length of the internal native socket address.

	const struct sockaddr* addr() const;
		/// Returns a pointer to the internal native socket address.

	int af() const;
		/// Returns the address family (AF_INET or AF_INET6) of the address.

	std::string toString() const;
		/// Returns a string representation of the address.

	Address::Family family() const;
		/// Returns the address family of the host's address.

	bool valid() const;
		/// Returns true when the port is set and the address is valid
		/// ie. not wildcard.
		
	static UInt16 resolveService(const std::string& service);

	static bool validateIP(const std::string& address);

	bool operator < (const Address& addr) const;
	bool operator == (const Address& addr) const;
	bool operator != (const Address& addr) const;
	
    friend std::ostream& operator << (std::ostream& stream, const Address& addr) 
	{
		stream << addr.toString();
		return stream;
    }

	enum
	{
		MAX_ADDRESS_LENGTH = 
#if defined(LibSourcey_HAVE_IPv6)
			sizeof(struct sockaddr_in6)
#else
			sizeof(struct sockaddr_in)
#endif
			/// Maximum length in bytes of a socket address.
	};

protected:
	void init(const std::string& host, UInt16 port);

private:
	AddressBase* _base;
};


} } // namespace scy::net


#endif // SOURCEY_Net_Address_H



// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
//
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.






	

/*
typedef sockaddr_in ip4_addr;
typedef sockaddr_in6 ip6_addr;


inline ip4_addr to_ip4_addr(const std::string& ip, int port) { return uv_ip4_addr(ip.c_str(), port); }
inline ip6_addr to_ip6_addr(const std::string& ip, int port) { return uv_ip6_addr(ip.c_str(), port); }


inline bool from_ip4_addr(ip4_addr* src, std::string& ip, int& port)
{
	char dest[16];
	if(uv_ip4_name(src, dest, 16) == 0)
	{
		ip = dest;
		port = static_cast<int>(ntohs(src->sin_port));
		return true;
	}
	return false;
}


inline bool from_ip6_addr(ip6_addr* src, std::string& ip, int& port)
{
	char dest[46];
	if(uv_ip6_name(src, dest, 46) == 0)
	{
		ip = dest;
		port = static_cast<int>(ntohs(src->sin6_port));
		return true;
	}
	return false;
}
class IPAddress;
*/
	/*
	Address(const IPAddress& host, UInt16 port);
		/// Creates a Address from an IP address and a port number.
		*/


	//IPAddress host() const;
		/// Returns the host IP address.
	/*
	//void init(const IPAddress& host, UInt16 port);
class Address//: public Poco::Net::Address
	/// Adds a few important methods which are missing from
	/// Poco's Address implementation.
{
public:
	Address() : 
	  Poco::Net::Address() {};
	Address(const Address& addr) : 
	  Poco::Net::Address(addr) {};
	Address(const Poco::Net::Address& addr) : 
	  Poco::Net::Address(addr) {};
	Address(const IP& host, unsigned short port) : 
	  Poco::Net::Address(host, port) {};
	Address(const std::string& host, unsigned short port) : 
	  Poco::Net::Address(host, port) {};
	Address(const std::string& host, const std::string& port) : 
	  Poco::Net::Address(host, port) {};	
	Address(const struct sockaddr* addr, int length) : 
	  Poco::Net::Address(addr, length) {};

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
	  */
	



/*
typedef Poco::std::stringAddress IP;


class Address: public Poco::Net::Address
	/// Adds a few important methods which are missing from
	/// Poco's Address implementation.
{
public:
	Address() : 
	  Poco::Net::Address() {};
	Address(const Address& addr) : 
	  Poco::Net::Address(addr) {};
	Address(const Poco::Net::Address& addr) : 
	  Poco::Net::Address(addr) {};
	Address(const IP& host, unsigned short port) : 
	  Poco::Net::Address(host, port) {};
	Address(const std::string& host, unsigned short port) : 
	  Poco::Net::Address(host, port) {};
	Address(const std::string& host, const std::string& port) : 
	  Poco::Net::Address(host, port) {};	
	Address(const struct sockaddr* addr, int length) : 
	  Poco::Net::Address(addr, length) {};

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
*/