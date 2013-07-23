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


//////#include "Poco/Net/Response.h"
#include "Sourcey/HTTP/Message.h"
#include "Sourcey/HTTP/Cookie.h"
#include "Sourcey/DateTime.h"

#include <sstream>

	
namespace scy { 
namespace http {


//class Cookie;


class Response: public http::Message
	/// This class encapsulates an HTTP response
	/// message.
	///
	/// In addition to the properties common to
	/// all HTTP messages, a HTTP response has
	/// status code and a reason phrase.
{
public:
	enum HTTPStatus
	{
		HTTP_CONTINUE                        = 100,
		HTTP_SWITCHING_PROTOCOLS             = 101,
		HTTP_OK                              = 200,
		HTTP_CREATED                         = 201,
		HTTP_ACCEPTED                        = 202,
		HTTP_NONAUTHORITATIVE                = 203,
		HTTP_NO_CONTENT                      = 204,
		HTTP_RESET_CONTENT                   = 205,
		HTTP_PARTIAL_CONTENT                 = 206,
		HTTP_MULTIPLE_CHOICES                = 300,
		HTTP_MOVED_PERMANENTLY               = 301,
		HTTP_FOUND                           = 302,
		HTTP_SEE_OTHER                       = 303,
		HTTP_NOT_MODIFIED                    = 304,
		HTTP_USEPROXY                        = 305,
		// UNUSED: 306
		HTTP_TEMPORARY_REDIRECT              = 307,
		HTTP_BAD_REQUEST                     = 400,
		HTTP_UNAUTHORIZED                    = 401,
		HTTP_PAYMENT_REQUIRED                = 402,
		HTTP_FORBIDDEN                       = 403,
		HTTP_NOT_FOUND                       = 404,
		HTTP_METHOD_NOT_ALLOWED              = 405,
		HTTP_NOT_ACCEPTABLE                  = 406,
		HTTP_PROXY_AUTHENTICATION_REQUIRED   = 407,
		HTTP_REQUEST_TIMEOUT                 = 408,
		HTTP_CONFLICT                        = 409,
		HTTP_GONE                            = 410,
		HTTP_LENGTH_REQUIRED                 = 411,
		HTTP_PRECONDITION_FAILED             = 412,
		HTTP_REQUESTENTITYTOOLARGE           = 413,
		HTTP_REQUESTURITOOLONG               = 414,
		HTTP_UNSUPPORTEDMEDIATYPE            = 415,
		HTTP_REQUESTED_RANGE_NOT_SATISFIABLE = 416,
		HTTP_EXPECTATION_FAILED              = 417,
		HTTP_INTERNAL_SERVER_ERROR           = 500,
		HTTP_NOT_IMPLEMENTED                 = 501,
		HTTP_BAD_GATEWAY                     = 502,
		HTTP_SERVICE_UNAVAILABLE             = 503,
		HTTP_GATEWAY_TIMEOUT                 = 504,
		HTTP_VERSION_NOT_SUPPORTED           = 505
	};

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
		/// Destroys the Response.

	void setStatus(HTTPStatus status);
		/// Sets the HTTP status code.
		///
		/// Does not change the reason phrase.
		
	HTTPStatus getStatus() const;
		/// Returns the HTTP status code.
		
	void setStatus(const std::string& status);
		/// Sets the HTTP status code.
		///
		/// The string must contain a valid
		/// HTTP numerical status code.
		
	void setReason(const std::string& reason);
		/// Sets the HTTP reason phrase.
		
	const std::string& getReason() const;
		/// Returns the HTTP reason phrase.

	void setStatusAndReason(HTTPStatus status, const std::string& reason);
		/// Sets the HTTP status code and reason phrase.
		
	void setStatusAndReason(HTTPStatus status);
		/// Sets the HTTP status code and reason phrase.
		///
		/// The reason phrase is set according to the status code.

	void setDate(const Timestamp& dateTime);
		/// Sets the Date header to the given date/time value.
		
	Timestamp getDate() const;
		/// Returns the value of the Date header.

	void addCookie(const Cookie& cookie);
		/// Adds the cookie to the response by
		/// adding a Set-Cookie header.

	void getCookies(std::vector<Cookie>& cookies) const;
		/// Returns a vector with all the cookies
		/// set in the response header.
		///
		/// May throw an exception in case of a malformed
		/// Set-Cookie header.

	void write(std::ostream& ostr) const;
		/// Writes the HTTP response to the given
		/// output stream.

	virtual bool success() const;
		/// Returns true if the HTTP response code was successful.
	
	static const std::string& getReasonForStatus(HTTPStatus status);
		/// Returns an appropriate reason phrase
		/// for the given status code.
	
    friend std::ostream& operator << (std::ostream& stream, const Response& res) 
	{
		res.write(stream);
		return stream;
    }

	static const std::string HTTP_REASON_CONTINUE;
	static const std::string HTTP_REASON_SWITCHING_PROTOCOLS;
	static const std::string HTTP_REASON_OK;
	static const std::string HTTP_REASON_CREATED;
	static const std::string HTTP_REASON_ACCEPTED;
	static const std::string HTTP_REASON_NONAUTHORITATIVE;
	static const std::string HTTP_REASON_NO_CONTENT;
	static const std::string HTTP_REASON_RESET_CONTENT;
	static const std::string HTTP_REASON_PARTIAL_CONTENT;
	static const std::string HTTP_REASON_MULTIPLE_CHOICES;
	static const std::string HTTP_REASON_MOVED_PERMANENTLY;
	static const std::string HTTP_REASON_FOUND;
	static const std::string HTTP_REASON_SEE_OTHER;
	static const std::string HTTP_REASON_NOT_MODIFIED;
	static const std::string HTTP_REASON_USEPROXY;
	static const std::string HTTP_REASON_TEMPORARY_REDIRECT;
	static const std::string HTTP_REASON_BAD_REQUEST;
	static const std::string HTTP_REASON_UNAUTHORIZED;
	static const std::string HTTP_REASON_PAYMENT_REQUIRED;
	static const std::string HTTP_REASON_FORBIDDEN;
	static const std::string HTTP_REASON_NOT_FOUND;
	static const std::string HTTP_REASON_METHOD_NOT_ALLOWED;
	static const std::string HTTP_REASON_NOT_ACCEPTABLE;
	static const std::string HTTP_REASON_PROXY_AUTHENTICATION_REQUIRED;
	static const std::string HTTP_REASON_REQUEST_TIMEOUT;
	static const std::string HTTP_REASON_CONFLICT;
	static const std::string HTTP_REASON_GONE;
	static const std::string HTTP_REASON_LENGTH_REQUIRED;
	static const std::string HTTP_REASON_PRECONDITION_FAILED;
	static const std::string HTTP_REASON_REQUESTENTITYTOOLARGE;
	static const std::string HTTP_REASON_REQUESTURITOOLONG;
	static const std::string HTTP_REASON_UNSUPPORTEDMEDIATYPE;
	static const std::string HTTP_REASON_REQUESTED_RANGE_NOT_SATISFIABLE;
	static const std::string HTTP_REASON_EXPECTATION_FAILED;
	static const std::string HTTP_REASON_INTERNAL_SERVER_ERROR;
	static const std::string HTTP_REASON_NOT_IMPLEMENTED;
	static const std::string HTTP_REASON_BAD_GATEWAY;
	static const std::string HTTP_REASON_SERVICE_UNAVAILABLE;
	static const std::string HTTP_REASON_GATEWAY_TIMEOUT;
	static const std::string HTTP_REASON_VERSION_NOT_SUPPORTED;
	static const std::string HTTP_REASON_UNKNOWN;
	
	static const std::string DATE;
	static const std::string SET_COOKIE;

private:
	enum Limits
	{
		MAX_VERSION_LENGTH = 8,
		MAX_STATUS_LENGTH  = 3,
		MAX_REASON_LENGTH  = 512
	};
	
	HTTPStatus  _status;
	std::string _reason;
	
	Response(const Response&);
	Response& operator = (const Response&);
};



	/*
class Response: public http::Message
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

	std::string error;
		/// Contains an error message if the response failed.
};
*/


} } // namespace scy::http


#endif