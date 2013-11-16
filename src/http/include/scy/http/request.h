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


#ifndef SCY_Request_H
#define SCY_Request_H


#include "scy/base.h"
#include "scy/collection.h"
#include "scy/http/message.h"

#include <sstream>

	
namespace scy { 
namespace http {
	

struct Method
	/// HTTP request methods
{
	static const std::string Get;
	static const std::string Head;
	static const std::string Put;
	static const std::string Post;
	static const std::string Options;
	static const std::string Delete;
	static const std::string Trace;
	static const std::string Connect;
};

	
class Request: public http::Message
	/// This class encapsulates an HTTP request message.
	///
	/// In addition to the properties common to all HTTP messages, 
	/// a HTTP request has a method (e.g. GET, HEAD, POST, etc.) and
	/// a request URI.
{
public:
	Request();
		// Creates a GET / HTTP/1.1 HTTP request.
		
	Request(const std::string& version);
		// Creates a GET / HTTP/1.x request with
		// the given version (HTTP/1.0 or HTTP/1.1).
		
	Request(const std::string& method, const std::string& uri);
		// Creates a HTTP/1.0 request with the given method and URI.

	Request(const std::string& method, const std::string& uri, const std::string& version);
		// Creates a HTTP request with the given method, URI and version.

	virtual ~Request();
		// Destroys the Request.

	void setMethod(const std::string& method);
		// Sets the method.

	const std::string& getMethod() const;
		// Returns the method.

	void setURI(const std::string& uri);
		// Sets the request URI.
		
	const std::string& getURI() const;
		// Returns the request URI.
		
	void setHost(const std::string& host);
		// Sets the value of the Host header field.
		
	void setHost(const std::string& host, UInt16 port);
		// Sets the value of the Host header field.
		//
		// If the given port number is a non-standard
		// port number (other than 80 or 443), it is
		// included in the Host header field.
		
	const std::string& getHost() const;
		// Returns the value of the Host header field.
		//
		// Throws a NotFoundException if the request
		// does not have a Host header field.

	void setCookies(const NVCollection& cookies);
		// Adds a Cookie header with the names and
		// values from cookies.
		
	void getCookies(NVCollection& cookies) const;
		// Fills cookies with the cookies extracted
		// from the Cookie headers in the request.
			
	void getURIParameters(NVCollection& params) const;
		// Returns the request URI parameters.

	bool hasCredentials() const;
		// Returns true if the request contains authentication
		// information in the form of an Authorization header.
		
	void getCredentials(std::string& scheme, std::string& authInfo) const;
		// Returns the authentication scheme and additional authentication
		// information contained in this request.
		//
		// Throws a std::exception if no authentication information
		// is contained in the request.
		
	void setCredentials(const std::string& scheme, const std::string& authInfo);
		// Sets the authentication scheme and information for
		// this request.

	bool hasProxyCredentials() const;
		// Returns true if the request contains proxy authentication
		// information in the form of an Proxy-Authorization header.
		
	void getProxyCredentials(std::string& scheme, std::string& authInfo) const;
		// Returns the proxy authentication scheme and additional proxy authentication
		// information contained in this request.
		//
		// Throws a std::exception if no proxy authentication information
		// is contained in the request.
		
	void setProxyCredentials(const std::string& scheme, const std::string& authInfo);
		// Sets the proxy authentication scheme and information for this request.

	void write(std::ostream& ostr) const;
		// Writes the HTTP request to the given output stream.
	
    friend std::ostream& operator << (std::ostream& stream, const Request& req) 
	{
		req.write(stream);
		return stream;
    }

protected:
	void getCredentials(const std::string& header, std::string& scheme, std::string& authInfo) const;
		// Returns the authentication scheme and additional authentication
		// information contained in the given header of request.
		//
		// Throws a NotAuthenticatedException if no authentication information
		// is contained in the request.
		
	void setCredentials(const std::string& header, const std::string& scheme, const std::string& authInfo);
		// Writes the authentication scheme and information for
		// this request to the given header.

private:
	std::string _method;
	std::string _uri;
};


} } // namespace scy::http


#endif