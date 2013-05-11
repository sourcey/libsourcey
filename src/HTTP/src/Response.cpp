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


#include "Sourcey/HTTP/Response.h"


using namespace std;
using namespace Poco;
using namespace Poco::Net;


namespace Scy { 
namespace HTTP {
	

Response::Response()
{
}

	
Response::Response(HTTPStatus status, const std::string& reason) :
	HTTPResponse(status, reason)
{
}


	
Response::Response(const std::string& version, HTTPStatus status, const std::string& reason) :
	HTTPResponse(version, status, reason)
{
}

	
Response::Response(HTTPStatus status) :
	HTTPResponse(status)
{
}


Response::Response(const std::string& version, HTTPStatus status) :
	HTTPResponse(version, status)
{
}


Response::~Response() 
{
}
	

bool Response::success() 
{
	return getStatus() < 400;
}


} } // namespace Scy::HTTP