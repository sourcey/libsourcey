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


#ifndef SOURCEY_HTTP_Utilities_h
#define SOURCEY_HTTP_Utilities_h


#include "Sourcey/Base.h"
#include "Poco/Net/NameValueCollection.h"


namespace Sourcey {
namespace Util {


std::string parseURI(const std::string& request);
	// Parses the URI from a HTTP request.

bool matchURI(const std::string& uri, const std::string& expression);
	// Parses the URI against the expression.

std::string parseHeader(const std::string& request, const std::string& name);
	// Parses a Header from a HTTP request.

std::string parseCookieItem(const std::string& cookie, const std::string& item);
	// Parses an item from a HTTP cookie.

//bool parseRestfulQuery(const std::string& request, StringList& out);

bool parseURIQuery(const std::string& uri, Poco::Net::NameValueCollection& out);


} // namespace Util
} // namespace Sourcey


#endif // SOURCEY_HTTP_Utilities_h