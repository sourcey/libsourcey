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
//#include "Sourcey/HTTP/Authenticator.h"
#include "Sourcey/HTTP/Util.h"

#include <assert.h>


using namespace std;


namespace scy { 
namespace http {

const std::string Request::HTTP_GET            = "GET";
const std::string Request::HTTP_HEAD           = "HEAD";
const std::string Request::HTTP_PUT            = "PUT";
const std::string Request::HTTP_POST           = "POST";
const std::string Request::HTTP_OPTIONS        = "OPTIONS";
const std::string Request::HTTP_DELETE         = "DELETE";
const std::string Request::HTTP_TRACE          = "TRACE";
const std::string Request::HTTP_CONNECT        = "CONNECT";
const std::string Request::HOST                = "Host";
const std::string Request::COOKIE              = "Cookie";
const std::string Request::AUTHORIZATION       = "Authorization";
const std::string Request::PROXY_AUTHORIZATION = "Proxy-Authorization";


Request::Request():
	_method(HTTP_GET),
	_uri("/")
{
}

	
Request::Request(const std::string& version):
	http::Message(version),
	_method(HTTP_GET),
	_uri("/")
{
}

	
Request::Request(const std::string& method, const std::string& uri):
	_method(method),
	_uri(uri)
{
}


Request::Request(const std::string& method, const std::string& uri, const std::string& version):
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
	set(HOST, host);
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
	return get(HOST);
}


const std::string& Request::getMethod() const
{
	return _method;
}


const std::string& Request::getURI() const
{
	return _uri;
}

void Request::setCookies(const NVHash& cookies)
{
	std::string cookie;
	for (NVHash::ConstIterator it = cookies.begin(); it != cookies.end(); ++it) {
		if (it != cookies.begin())
			cookie.append("; ");
		cookie.append(it->first);
		cookie.append("=");
		cookie.append(it->second);
	}
	add(COOKIE, cookie);
}

	
void Request::getCookies(NVHash& cookies) const
{
	NVHash::ConstIterator it = find(COOKIE);
	while (it != end() && util::icompare(it->first, COOKIE) == 0) {
		util::splitParameters(it->second.begin(), it->second.end(), cookies);
		++it;
	}
}

	
void Request::getURIParameters(NVHash& params) const
{	
	util::splitURIParameters(getURI(), params);
}


bool Request::hasCredentials() const
{
	return has(AUTHORIZATION);
}

	
void Request::getCredentials(std::string& scheme, std::string& authInfo) const
{
	getCredentials(AUTHORIZATION, scheme, authInfo);
}

	
void Request::setCredentials(const std::string& scheme, const std::string& authInfo)
{
	setCredentials(AUTHORIZATION, scheme, authInfo);
}


bool Request::hasProxyCredentials() const
{
	return has(PROXY_AUTHORIZATION);
}

	
void Request::getProxyCredentials(std::string& scheme, std::string& authInfo) const
{
	getCredentials(PROXY_AUTHORIZATION, scheme, authInfo);
}

	
void Request::setProxyCredentials(const std::string& scheme, const std::string& authInfo)
{
	setCredentials(PROXY_AUTHORIZATION, scheme, authInfo);
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
	if (has(header))
	{
		const std::string& auth = get(header);
		std::string::const_iterator it  = auth.begin();
		std::string::const_iterator end = auth.end();
		while (it != end && ::isspace(*it)) ++it;
		while (it != end && !::isspace(*it)) scheme += *it++;
		while (it != end && ::isspace(*it)) ++it;
		while (it != end) authInfo += *it++;
	}
	else throw Exception("Request is not authenticated");
}

	
void Request::setCredentials(const std::string& header, const std::string& scheme, const std::string& authInfo)
{
	std::string auth(scheme);
	auth.append(" ");
	auth.append(authInfo);
	set(header, auth);
}


} } // namespace scy::http