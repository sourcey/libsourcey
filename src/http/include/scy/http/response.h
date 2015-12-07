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

#ifndef SCY_HTTP_Response_H
#define SCY_HTTP_Response_H


#include "scy/http/message.h"
#include "scy/http/cookie.h"
#include "scy/datetime.h"

#include <sstream>

    
namespace scy { 
namespace http {


enum class StatusCode
    /// HTTP Response Status Codes
{
    Continue = 100,
    SwitchingProtocols = 101,

    OK = 200,
    Created = 201,
    Accepted = 202,
    NonAuthoritative = 203,
    NoContent = 204,
    ResetContent = 205,
    PartialContent = 206,

    MultipleChoices = 300,
    MovedPermanently = 301,
    Found = 302,
    SeeOther = 303,
    NotModified = 304,
    UseProxy = 305,
    // SwitchProxy = 306, not used
    TemporaryRedirect = 307,

    BadRequest = 400,
    Unauthorized = 401,
    PaymentRequired = 402,
    Forbidden = 403,
    NotFound = 404,
    MethodNotAllowed = 405,
    NotAcceptable = 406,
    ProxyAuthRequired = 407,
    RequestTimeout = 408,
    Conflict = 409,
    Gone = 410,
    LengthRequired = 411,
    PreconditionFailed = 412,
    EntityTooLarge = 413,
    UriTooLong = 414,
    UnsupportedMediaType = 415,
    RangeNotSatisfiable = 416,
    ExpectationFailed = 417,

    InternalServerError = 500,
    NotImplemented = 501,
    BadGateway = 502,
    Unavailable = 503,
    GatewayTimeout = 504,
    VersionNotSupported = 505
};


class Response: public http::Message
    /// This class encapsulates an HTTP response message.
{
public:
    Response();
        // Creates the Response with OK status.
        
    Response(StatusCode status, const std::string& reason);
        // Creates the Response with the given status  and reason phrase.

    Response(const std::string& version, StatusCode status, const std::string& reason);
        // Creates the Response with the given version, status and reason phrase.
        
    Response(StatusCode status);
        // Creates the Response with the given status
        // an an appropriate reason phrase.

    Response(const std::string& version, StatusCode status);
        // Creates the Response with the given version, status
        // an an appropriate reason phrase.

    virtual ~Response();
        // Destroys the Response.

    void setStatus(StatusCode status);
        // Sets the HTTP status code.
        //
        // The reason phrase is set according to the status code.
        
    StatusCode getStatus() const;
        // Returns the HTTP status code.
        
    void setReason(const std::string& reason);
        // Sets the HTTP reason phrase.
        
    const std::string& getReason() const;
        // Returns the HTTP reason phrase.

    void setStatusAndReason(StatusCode status, const std::string& reason);
        // Sets the HTTP status code and reason phrase.

    void setDate(const Timestamp& dateTime);
        // Sets the Date header to the given date/time value.
        
    Timestamp getDate() const;
        // Returns the value of the Date header.

    void addCookie(const Cookie& cookie);
        // Adds the cookie to the response by
        // adding a Set-Cookie header.

    void getCookies(std::vector<Cookie>& cookies) const;
        // Returns a vector with all the cookies set in the response header.
        //
        // May throw an exception in case of a malformed Set-Cookie header.

    void write(std::ostream& ostr) const;
        /// Writes the HTTP response headers to the given output stream.

    virtual bool success() const;
        /// Returns true if the HTTP response code was successful (>= 400).
    
    friend std::ostream& operator << (std::ostream& stream, const Response& res) 
    {
        res.write(stream);
        return stream;
    }

private:    
    Response(const Response&);
    Response& operator = (const Response&);

    StatusCode  _status;
    std::string _reason;
};


const char* getStatusCodeReason(StatusCode status);


} } // namespace scy::http


#endif


//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//



    
    /*
    enum StatusCode
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
        /// UNUSED: 306
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
    
    static const std::string "Date";
    static const std::string "Set-Cookie";
    */
