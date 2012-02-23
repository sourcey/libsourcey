//
// This software is copyright by Sourcey <mail@sourcey.com> and is distributed under a dual license:
// Copyright (C) 2002 Sourcey
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


#ifndef SOURCEY_HTTP_Request_H
#define SOURCEY_HTTP_Request_H


#include "Sourcey/Base.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTMLForm.h"

#include <sstream>

	
namespace Sourcey { 
namespace HTTP {


class Request: public Poco::Net::HTTPRequest
{
public:
	Request();
		/// Creates a GET / HTTP/1.1 HTTP request.
		
	Request(const std::string& version);
		/// Creates a GET / HTTP/1.x request with
		/// the given version (HTTP/1.0 or HTTP/1.1).
		
	Request(const std::string& method, const std::string& uri);
		/// Creates a HTTP/1.1 request with the given method and URI.

	Request(const std::string& method, const std::string& uri, const std::string& version);
		/// Creates a HTTP request with the given method, URI and version.

	virtual ~Request();
		/// Destroys the Request.	

	virtual void read(std::istream& istr);
		/// Reads the HTTP request from the
		/// given input stream.

	virtual void prepare();
		/// MUST be called after setting all information and
		/// credentials before sending the request.
	
	virtual bool matches(const std::string& expression) const;
		/// Matches the URI against the given expression.
			
	virtual const NameValueCollection& params() const;
		/// Returns the request URI parameters.

	Poco::Net::HTMLForm* form;
		/// An optional HTML form.

	std::stringstream body;
		/// The HTTP request data.

protected:
	NameValueCollection _params;

private:
	Request(Request& r) {}
		/// The copy constructor is private (Poco limitation). 
};


} } // namespace Sourcey::HTTP


#endif