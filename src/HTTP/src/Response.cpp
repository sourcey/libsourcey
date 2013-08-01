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


#include "Sourcey/HTTP/Response.h"
#include "Sourcey/HTTP/Util.h"
#include "Sourcey/DateTime.h"

#include "Poco/NumberFormatter.h"
#include "Poco/NumberParser.h"
#include "Poco/Ascii.h"
#include "Poco/String.h"


using namespace std;


namespace scy { 
namespace http {


const std::string Response::HTTP_REASON_CONTINUE                        = "Continue";
const std::string Response::HTTP_REASON_SWITCHING_PROTOCOLS             = "Switching Protocols";
const std::string Response::HTTP_REASON_OK                              = "OK";
const std::string Response::HTTP_REASON_CREATED                         = "Created";
const std::string Response::HTTP_REASON_ACCEPTED                        = "Accepted";
const std::string Response::HTTP_REASON_NONAUTHORITATIVE                = "Non-Authoritative Information";
const std::string Response::HTTP_REASON_NO_CONTENT                      = "No Content";
const std::string Response::HTTP_REASON_RESET_CONTENT                   = "Reset Content";
const std::string Response::HTTP_REASON_PARTIAL_CONTENT                 = "Partial Content";
const std::string Response::HTTP_REASON_MULTIPLE_CHOICES                = "Multiple Choices";
const std::string Response::HTTP_REASON_MOVED_PERMANENTLY               = "Moved Permanently";
const std::string Response::HTTP_REASON_FOUND                           = "Found";
const std::string Response::HTTP_REASON_SEE_OTHER                       = "See Other";
const std::string Response::HTTP_REASON_NOT_MODIFIED                    = "Not Modified";
const std::string Response::HTTP_REASON_USEPROXY                        = "Use Proxy";
const std::string Response::HTTP_REASON_TEMPORARY_REDIRECT              = "Temporary Redirect";
const std::string Response::HTTP_REASON_BAD_REQUEST                     = "Bad Request";
const std::string Response::HTTP_REASON_UNAUTHORIZED                    = "Unauthorized";
const std::string Response::HTTP_REASON_PAYMENT_REQUIRED                = "Payment Required";
const std::string Response::HTTP_REASON_FORBIDDEN                       = "Forbidden";
const std::string Response::HTTP_REASON_NOT_FOUND                       = "Not Found";
const std::string Response::HTTP_REASON_METHOD_NOT_ALLOWED              = "Method Not Allowed";
const std::string Response::HTTP_REASON_NOT_ACCEPTABLE                  = "Not Acceptable";
const std::string Response::HTTP_REASON_PROXY_AUTHENTICATION_REQUIRED   = "Proxy Authentication Required";
const std::string Response::HTTP_REASON_REQUEST_TIMEOUT                 = "Request Time-out";
const std::string Response::HTTP_REASON_CONFLICT                        = "Conflict";
const std::string Response::HTTP_REASON_GONE                            = "Gone";
const std::string Response::HTTP_REASON_LENGTH_REQUIRED                 = "Length Required";
const std::string Response::HTTP_REASON_PRECONDITION_FAILED             = "Precondition Failed";
const std::string Response::HTTP_REASON_REQUESTENTITYTOOLARGE           = "Request Entity Too Large";
const std::string Response::HTTP_REASON_REQUESTURITOOLONG               = "Request-URI Too Large";
const std::string Response::HTTP_REASON_UNSUPPORTEDMEDIATYPE            = "Unsupported Media Type";
const std::string Response::HTTP_REASON_REQUESTED_RANGE_NOT_SATISFIABLE = "Requested Range Not Satisfiable";
const std::string Response::HTTP_REASON_EXPECTATION_FAILED              = "Expectation Failed";
const std::string Response::HTTP_REASON_INTERNAL_SERVER_ERROR           = "Internal Server Error";
const std::string Response::HTTP_REASON_NOT_IMPLEMENTED                 = "Not Implemented";
const std::string Response::HTTP_REASON_BAD_GATEWAY                     = "Bad Gateway";
const std::string Response::HTTP_REASON_SERVICE_UNAVAILABLE             = "Service Unavailable";
const std::string Response::HTTP_REASON_GATEWAY_TIMEOUT                 = "Gateway Time-out";
const std::string Response::HTTP_REASON_VERSION_NOT_SUPPORTED           = "HTTP Version not supported";
const std::string Response::HTTP_REASON_UNKNOWN                         = "???";
const std::string Response::DATE       = "Date";
const std::string Response::SET_COOKIE = "Set-Cookie";


Response::Response():
	_status(HTTP_OK),
	_reason(getReasonForStatus(HTTP_OK))
{
}

	
Response::Response(HTTPStatus status, const std::string& reason):
	_status(status),
	_reason(reason)
{
}


	
Response::Response(const std::string& version, HTTPStatus status, const std::string& reason):
	http::Message(version),
	_status(status),
	_reason(reason)
{
}

	
Response::Response(HTTPStatus status):
	_status(status),
	_reason(getReasonForStatus(status))
{
}


Response::Response(const std::string& version, HTTPStatus status):
	http::Message(version),
	_status(status),
	_reason(getReasonForStatus(status))
{
}


Response::~Response()
{
}


void Response::setStatus(HTTPStatus status)
{
	_status = status;
}


void Response::setStatus(const std::string& status)
{
	setStatus((HTTPStatus) Poco::NumberParser::parse(status));
}
	
	
void Response::setReason(const std::string& reason)
{
	_reason = reason;
}


void Response::setStatusAndReason(HTTPStatus status, const std::string& reason)
{
	_status = status;
	_reason = reason;
}

	
void Response::setStatusAndReason(HTTPStatus status)
{
	setStatusAndReason(status, getReasonForStatus(status));
}


void Response::setDate(const Timestamp& dateTime)
{
	set(DATE, DateTimeFormatter::format(dateTime, DateTimeFormat::HTTP_FORMAT));
}

	
Timestamp Response::getDate() const
{
	const std::string& dateTime = get(DATE);
	int tzd;
	return DateTimeParser::parse(dateTime, tzd).timestamp();
}


void Response::addCookie(const Cookie& cookie)
{
	add(SET_COOKIE, cookie.toString());
}


void Response::getCookies(std::vector<Cookie>& cookies) const
{
	cookies.clear();
	NVHash::ConstIterator it = find(SET_COOKIE);
	while (it != end() && util::icompare(it->first, SET_COOKIE) == 0)
	{
		NVHash nvc;
		util::splitParameters(it->second.begin(), it->second.end(), nvc);
		cookies.push_back(Cookie(nvc));
		++it;
	}
}


void Response::write(std::ostream& ostr) const
{
	ostr << getVersion() << " " << static_cast<int>(_status) << " " << _reason << "\r\n";
	http::Message::write(ostr);
	ostr << "\r\n";
}
	

bool Response::success()  const
{
	return getStatus() < 400;
}


Response::HTTPStatus Response::getStatus() const
{
	return _status;
}


const std::string& Response::getReason() const
{
	return _reason;
}


const std::string& Response::getReasonForStatus(HTTPStatus status)
{
	switch (status)
	{
	case HTTP_CONTINUE: 
		return HTTP_REASON_CONTINUE;
	case HTTP_SWITCHING_PROTOCOLS: 
		return HTTP_REASON_SWITCHING_PROTOCOLS;
	case HTTP_OK: 
		return HTTP_REASON_OK;
	case HTTP_CREATED: 
		return HTTP_REASON_CREATED;
	case HTTP_ACCEPTED: 
		return HTTP_REASON_ACCEPTED;
	case HTTP_NONAUTHORITATIVE:	
		return HTTP_REASON_NONAUTHORITATIVE;
	case HTTP_NO_CONTENT: 
		return HTTP_REASON_NO_CONTENT;
	case HTTP_RESET_CONTENT: 
		return HTTP_REASON_RESET_CONTENT;
	case HTTP_PARTIAL_CONTENT: 
		return HTTP_REASON_PARTIAL_CONTENT;
	case HTTP_MULTIPLE_CHOICES: 
		return HTTP_REASON_MULTIPLE_CHOICES;
	case HTTP_MOVED_PERMANENTLY: 
		return HTTP_REASON_MOVED_PERMANENTLY;
	case HTTP_FOUND: 
		return HTTP_REASON_FOUND;
	case HTTP_SEE_OTHER: 
		return HTTP_REASON_SEE_OTHER;
	case HTTP_NOT_MODIFIED: 
		return HTTP_REASON_NOT_MODIFIED;
	case HTTP_USEPROXY: 
		return HTTP_REASON_USEPROXY;
	case HTTP_TEMPORARY_REDIRECT: 
		return HTTP_REASON_TEMPORARY_REDIRECT;
	case HTTP_BAD_REQUEST: 
		return HTTP_REASON_BAD_REQUEST;
	case HTTP_UNAUTHORIZED: 
		return HTTP_REASON_UNAUTHORIZED;
	case HTTP_PAYMENT_REQUIRED: 
		return HTTP_REASON_PAYMENT_REQUIRED;
	case HTTP_FORBIDDEN: 
		return HTTP_REASON_FORBIDDEN;
	case HTTP_NOT_FOUND: 
		return HTTP_REASON_NOT_FOUND;
	case HTTP_METHOD_NOT_ALLOWED:
		return HTTP_REASON_METHOD_NOT_ALLOWED;
	case HTTP_NOT_ACCEPTABLE: 
		return HTTP_REASON_NOT_ACCEPTABLE;
	case HTTP_PROXY_AUTHENTICATION_REQUIRED: 
		return HTTP_REASON_PROXY_AUTHENTICATION_REQUIRED;
	case HTTP_REQUEST_TIMEOUT: 
		return HTTP_REASON_REQUEST_TIMEOUT;
	case HTTP_CONFLICT: 
		return HTTP_REASON_CONFLICT;
	case HTTP_GONE: 
		return HTTP_REASON_GONE;
	case HTTP_LENGTH_REQUIRED: 
		return HTTP_REASON_LENGTH_REQUIRED;
	case HTTP_PRECONDITION_FAILED: 
		return HTTP_REASON_PRECONDITION_FAILED;
	case HTTP_REQUESTENTITYTOOLARGE: 
		return HTTP_REASON_REQUESTENTITYTOOLARGE;
	case HTTP_REQUESTURITOOLONG: 
		return HTTP_REASON_REQUESTURITOOLONG;
	case HTTP_UNSUPPORTEDMEDIATYPE: 
		return HTTP_REASON_UNSUPPORTEDMEDIATYPE;
	case HTTP_REQUESTED_RANGE_NOT_SATISFIABLE: 
		return HTTP_REASON_REQUESTED_RANGE_NOT_SATISFIABLE;
	case HTTP_EXPECTATION_FAILED: 
		return HTTP_REASON_EXPECTATION_FAILED;
	case HTTP_INTERNAL_SERVER_ERROR: 
		return HTTP_REASON_INTERNAL_SERVER_ERROR;
	case HTTP_NOT_IMPLEMENTED: 
		return HTTP_REASON_NOT_IMPLEMENTED;
	case HTTP_BAD_GATEWAY: 
		return HTTP_REASON_BAD_GATEWAY;
	case HTTP_SERVICE_UNAVAILABLE:
		return HTTP_REASON_SERVICE_UNAVAILABLE;
	case HTTP_GATEWAY_TIMEOUT: 
		return HTTP_REASON_GATEWAY_TIMEOUT;
	case HTTP_VERSION_NOT_SUPPORTED: 
		return HTTP_REASON_VERSION_NOT_SUPPORTED;
	default: 
		return HTTP_REASON_UNKNOWN;
	}
}



/*
Response::Response()
{
}

	
Response::Response(HTTPStatus status, const std::string& reason) :
	Response(status, reason)
{
}


	
Response::Response(const std::string& version, HTTPStatus status, const std::string& reason) :
	Response(version, status, reason)
{
}

	
Response::Response(HTTPStatus status) :
	Response(status)
{
}


Response::Response(const std::string& version, HTTPStatus status) :
	Response(version, status)
{
}


Response::~Response() 
{
}
*/


} } // namespace scy::http