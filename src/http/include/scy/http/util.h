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


#ifndef SCY_HTTP_Utilities_h
#define SCY_HTTP_Utilities_h


#include "scy/base.h"
#include "scy/collection.h"



namespace scy {
namespace http {
	

std::string parseURI(const std::string& request);
	/// Parses the URI part from a HTTP request.

bool matchURL(const std::string& uri, const std::string& expression);
	/// Parses the URI against the expression.

bool matchURL(const std::string& uri, const std::string& expression);
	/// Parses the URI against the expression.

std::string parseCookieItem(const std::string& cookie, const std::string& item);
	/// Parses an item from a HTTP cookie.

bool splitURIParameters(const std::string& uri, NVCollection& out);
	/// Splits and adds the URI query parameters into the given collection.

void splitParameters(const std::string& s, std::string& value, NVCollection& parameters);
	/// Splits the given std::string into a value and a collection of parameters.
	/// Parameters are expected to be separated by semicolons.
	///
	/// Enclosing quotes of parameter values are removed.
	///
	/// For example, the std::string
	///   multipart/mixed; boundary="boundary-01234567"
	/// is split into the value
	///   multipart/mixed
	/// and the parameter
	///   boundary -> boundary-01234567

void splitParameters(const std::string::const_iterator& begin, const std::string::const_iterator& end, NVCollection& parameters);
	/// Splits the given std::string into a collection of parameters.
	/// Parameters are expected to be separated by semicolons.
	///
	/// Enclosing quotes of parameter values are removed.


} // namespace http
} // namespace scy


#endif // SCY_HTTP_Utilities_h




//std::string parseHeader(const std::string& request, const std::string& name);
	/// Parses a Header from a HTTP request.
//bool parseRestfulQuery(const std::string& request, std::vector<std::string>& out);
