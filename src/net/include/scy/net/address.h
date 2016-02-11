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


#ifndef SCY_Net_Address_H
#define SCY_Net_Address_H


#include "scy/net/types.h"


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

    Address(const std::string& host, std::uint16_t port);
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

    std::uint16_t port() const;
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
        
    static std::uint16_t resolveService(const std::string& service);

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
    void init(const std::string& host, std::uint16_t port);

private:
    AddressBase* _base;
};


} } // namespace scy::net


#endif // SCY_Net_Address_H



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