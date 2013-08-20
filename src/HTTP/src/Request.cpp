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


#include "Sourcey/HTTP/Request.h"
#include "Sourcey/HTTP/Util.h"

#include <assert.h>


using namespace std;


namespace scy { 
namespace http {


const std::string Method::Get       = "GET";
const std::string Method::Head      = "HEAD";
const std::string Method::Put       = "PUT";
const std::string Method::Post      = "POST";
const std::string Method::Options   = "OPTIONS";
const std::string Method::Delete    = "DELETE";
const std::string Method::Trace     = "TRACE";
const std::string Method::Connect   = "CONNECT";


Request::Request() :
	_method(Method::Get),
	_uri("/")
{
}

	
Request::Request(const std::string& version) :
	http::Message(version),
	_method(Method::Get),
	_uri("/")
{
}

	
Request::Request(const std::string& method, const std::string& uri) :
	_method(method),
	_uri(uri)
{
}


Request::Request(const std::string& method, const std::string& uri, const std::string& version) :
	http::Message(version),
	_method(method),
	_uri(uri)
{
}


Request::~Request()
{
}


void Request::setMethod(const std::string& method)
{
	_method = method;
}


void Request::setURI(const std::string& uri)
{
	_uri = uri;
}


void Request::setHost(const std::string& host)
{
	set("Host", host);
}

	
void Request::setHost(const std::string& host, UInt16 port)
{
	std::string value(host);
	if (port != 80 && port != 443) {
		value.append(":");
		value.append(util::itostr<UInt16>(port));
	}
	setHost(value);
}

	
const std::string& Request::getHost() const
{
	return get("Host");
}


const std::string& Request::getMethod() const
{
	return _method;
}


const std::string& Request::getURI() const
{
	return _uri;
}

void Request::setCookies(const NVCollection& cookies)
{
	std::string cookie;
	for (NVCollection::ConstIterator it = cookies.begin(); it != cookies.end(); ++it) {
		if (it != cookies.begin())
			cookie.append("; ");
		cookie.append(it->first);
		cookie.append("=");
		cookie.append(it->second);
	}
	add("Cookie", cookie);
}

	
void Request::getCookies(NVCollection& cookies) const
{
	NVCollection::ConstIterator it = find("Cookie");
	while (it != end() && util::icompare(it->first, "Cookie") == 0) {
		http::splitParameters(it->second.begin(), it->second.end(), cookies);
		++it;
	}
}

	
void Request::getURIParameters(NVCollection& params) const
{	
	http::splitURIParameters(getURI(), params);
}


bool Request::hasCredentials() const
{
	return has("Authorization");
}

	
void Request::getCredentials(std::string& scheme, std::string& authInfo) const
{
	getCredentials("Authorization", scheme, authInfo);
}

	
void Request::setCredentials(const std::string& scheme, const std::string& authInfo)
{
	setCredentials("Authorization", scheme, authInfo);
}


bool Request::hasProxyCredentials() const
{
	return has("Proxy-Authorization");
}

	
void Request::getProxyCredentials(std::string& scheme, std::string& authInfo) const
{
	getCredentials("Proxy-Authorization", scheme, authInfo);
}

	
void Request::setProxyCredentials(const std::string& scheme, const std::string& authInfo)
{
	setCredentials("Proxy-Authorization", scheme, authInfo);
}


void Request::write(std::ostream& ostr) const
{
	ostr << _method << " " << _uri << " " << getVersion() << "\r\n";
	http::Message::write(ostr);
	ostr << "\r\n";
}


void Request::getCredentials(const std::string& header, std::string& scheme, std::string& authInfo) const
{
	scheme.clear();
	authInfo.clear();
	if (has(header)) {
		const std::string& auth = get(header);
		std::string::const_iterator it  = auth.begin();
		std::string::const_iterator end = auth.end();
		while (it != end && ::isspace(*it)) ++it;
		while (it != end && !::isspace(*it)) scheme += *it++;
		while (it != end && ::isspace(*it)) ++it;
		while (it != end) authInfo += *it++;
	}
	else throw std::runtime_error("Request is not authenticated");
}

	
void Request::setCredentials(const std::string& header, const std::string& scheme, const std::string& authInfo)
{
	std::string auth(scheme);
	auth.append(" ");
	auth.append(authInfo);
	set(header, auth);
}


} } // namespace scy::http