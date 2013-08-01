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
	if (port != 80 && port != 443)
	{
		value.append(":");
		value.append(util::toString<UInt16>(port));
		//NumberFormatter::append(value, port);
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
	cookie.reserve(64);
	for (NVHash::ConstIterator it = cookies.begin(); it != cookies.end(); ++it)
	{
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
	while (it != end() && util::icompare(it->first, COOKIE) == 0)
	{
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


/*
void Request::read(std::istream& istr)
{
	static const int eof = std::char_traits<char>::eof();

	std::string method;
	std::string uri;
	std::string version;
	method.reserve(16);
	uri.reserve(64);
	version.reserve(16);
	int ch = istr.get();
	if (ch == eof) throw Exception("Message error: Cannot read empty message");
	while (::isspace(ch)) ch = istr.get();
	if (ch == eof) throw Exception("Message error: No HTTP request header");
	while (!::isspace(ch) && ch != eof && method.length() < MAX_METHOD_LENGTH) { method += (char) ch; ch = istr.get(); }
	if (!::isspace(ch)) throw Exception("Message error: HTTP request method invalid or too long");
	while (::isspace(ch)) ch = istr.get();
	while (!::isspace(ch) && ch != eof && uri.length() < MAX_URI_LENGTH) { uri += (char) ch; ch = istr.get(); }
	if (!::isspace(ch)) throw Exception("Message error: HTTP request URI invalid or too long");
	while (::isspace(ch)) ch = istr.get();
	while (!::isspace(ch) && ch != eof && version.length() < MAX_VERSION_LENGTH) { version += (char) ch; ch = istr.get(); }
	if (!::isspace(ch)) throw Exception("Message error: Invalid HTTP version string");
	while (ch != '\n' && ch != eof) { ch = istr.get(); }
	http::Message::read(istr);
	ch = istr.get();
	while (ch != '\n' && ch != eof) { ch = istr.get(); }
	setMethod(method);
	setURI(uri);
	setVersion(version);
}
*/


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





/*
Request::Request() : 
	http::Message(Message::HTTP_1_1)//, form(nil)
{
}


Request::Request(const string& version) : 
	http::Message(version)//, form(nil)
{
}


Request::Request(const string& method, const string& uri) : 
	http::Message(method, uri, Message::HTTP_1_1)//, form(nil)
{
}


Request::Request(const string& method, const string& uri, const string& version) : 
	http::Message(method, uri, version)//, form(nil)
{
}


Request::~Request()
{
	//if (form) delete form;
}


void Request::prepare()
{
	assert(!getMethod().empty());
	assert(!getURI().empty());

	string date = DateTimeFormatter::format(Timestamp(), DateTimeFormat::RFC822_FORMAT);
	set("Date", date);	
	set("User-Agent", "Sourcey C++ API");
	if (getMethod() == "POST" || 
		getMethod() == "PUT") {
		if (form) {
			form->prepareSubmit(*this);	
			form->write(body);
			streambuf* pbuf = body.rdbuf();
			long contentLength = (long)pbuf->pubseekoff(0, ios_base::end);
			assert(contentLength > 0);
			setContentLength(contentLength);
			setChunkedTransferEncoding(false);
			pbuf->pubseekpos(0);
		}
		else
			setContentLength(body.str().length());
	}
}


void Request::read(istream& istr)
{
	http::Message::read(istr);
	util::splitURIParameters(getURI(), _params);
}

			
const NVHash& Request::params() const
{	
	return _params;
}


bool Request::matches(const string& expression) const
{
	return util::matchURL(getURI(), expression);
}
*/