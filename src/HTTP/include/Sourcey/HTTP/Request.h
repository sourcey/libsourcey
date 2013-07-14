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


#ifndef SOURCEY_HTTP_Request_H
#define SOURCEY_HTTP_Request_H


#include "Sourcey/Base.h"
#include "Sourcey/Manager.h"
#include "Poco/Net/HTTPRequest.h"
//#include "Poco/Net/HTMLForm.h"

#include <sstream>

	
namespace scy { 
namespace http {


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
			
	virtual const KVStore& params() const;
		/// Returns the request URI parameters.

	//Poco::Net::HTMLForm* form;
		/// An optional HTML form.

	std::ostringstream body;
		/// The HTTP request body data.
	
    friend std::ostream& operator << (std::ostream& stream, const Request& req) 
	{
		req.write(stream);
		return stream;
    }

protected:
	KVStore _params;

private:
	Request(const Request&) {}
		/// The copy constructor is private (Poco limitation). 
};


} } // namespace scy::http


#endif