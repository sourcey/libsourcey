//
// This software is copyright by Sourcey <mail@sourcey.com> and is distributed under a dual license:
// Copyright (C) 2005 Sourcey
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


#ifndef SOURCEY_HTTP_Response_H
#define SOURCEY_HTTP_Response_H


#include "Poco/Net/HTTPResponse.h"
#include <sstream>

	
namespace Sourcey { 
namespace HTTP {


class Response: public Poco::Net::HTTPResponse
{	
public:
	Response();
	virtual ~Response();
		/// Destroys the Request.	
	
	std::stringstream body;
		/// Contains the respose body data.

	std::string error;
		/// Contains an error message if the response failed.

	virtual bool success();
		/// Returns true if the HTTP response code was successful.
};


} } // namespace Sourcey::HTTP


#endif