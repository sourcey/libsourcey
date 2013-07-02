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


#ifndef SOURCEY_HTTP_Response_H
#define SOURCEY_HTTP_Response_H


#include "Poco/Net/HTTPResponse.h"
#include <sstream>

	
namespace scy { 
namespace http {


class Response: public Poco::Net::HTTPResponse
{	
public:
	Response();
		/// Creates the Response with OK status.
		
	Response(HTTPStatus status, const std::string& reason);
		/// Creates the Response with the given status
		/// and reason phrase.

	Response(const std::string& version, HTTPStatus status, const std::string& reason);
		/// Creates the Response with the given version, status
		/// and reason phrase.
		
	Response(HTTPStatus status);
		/// Creates the Response with the given status
		/// an an appropriate reason phrase.

	Response(const std::string& version, HTTPStatus status);
		/// Creates the Response with the given version, status
		/// an an appropriate reason phrase.

	virtual ~Response();
		/// Destroys the Request.	
	
	std::stringstream body;
		/// Contains the response body data.

	std::string error;
		/// Contains an error message if the response failed.

	virtual bool success();
		/// Returns true if the HTTP response code was successful.
};


} } // namespace scy::http


#endif