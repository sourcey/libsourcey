///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup http
/// @{

#include "scy/http/response.h"
#include "scy/datetime.h"
#include "scy/http/util.h"

using std::endl;

namespace scy {
namespace http {

Response::Response()
    : _status(StatusCode::OK)
    , _reason(getStatusCodeReason(StatusCode::OK))
{
}

Response::Response(StatusCode status, const std::string& reason)
    : _status(status)
    , _reason(reason)
{
}

Response::Response(const std::string& version, StatusCode status,
                   const std::string& reason)
    : http::Message(version)
    , _status(status)
    , _reason(reason)
{
}

Response::Response(StatusCode status)
    : _status(status)
    , _reason(getStatusCodeReason(status))
{
}

Response::Response(const std::string& version, StatusCode status)
    : http::Message(version)
    , _status(status)
    , _reason(getStatusCodeReason(status))
{
}

Response::~Response()
{
}

void Response::setStatus(StatusCode status)
{
    _status = status;
    _reason = getStatusCodeReason(status);
}

void Response::setReason(const std::string& reason)
{
    _reason = reason;
}

void Response::setStatusAndReason(StatusCode status, const std::string& reason)
{
    _status = status;
    _reason = reason;
}

void Response::setDate(const Timestamp& dateTime)
{
    set("Date",
        DateTimeFormatter::format(dateTime, DateTimeFormat::HTTP_FORMAT));
}

Timestamp Response::getDate() const
{
    const std::string& dateTime = get("Date");
    int tzd;
    return DateTimeParser::parse(dateTime, tzd).timestamp();
}

void Response::addCookie(const Cookie& cookie)
{
    add("Set-Cookie", cookie.toString());
}

void Response::getCookies(std::vector<Cookie>& cookies) const
{
    cookies.clear();
    NVCollection::ConstIterator it = find("Set-Cookie");
    while (it != end() && util::icompare(it->first, "Set-Cookie") == 0) {
        NVCollection nvc;
        http::splitParameters(it->second.begin(), it->second.end(), nvc);
        cookies.push_back(Cookie(nvc));
        ++it;
    }
}

void Response::write(std::ostream& ostr) const
{
    ostr << getVersion() << " " << static_cast<int>(_status) << " " << _reason
         << "\r\n";
    http::Message::write(ostr);
    ostr << "\r\n";
}

bool Response::success() const
{
    return getStatus() < StatusCode::BadRequest; // < 400
}

StatusCode Response::getStatus() const
{
    return _status;
}

const std::string& Response::getReason() const
{
    return _reason;
}

const char* getStatusCodeReason(StatusCode status)
{
    switch (status) {
        case StatusCode::Continue:
            return "Continue";
        case StatusCode::SwitchingProtocols:
            return "Switching Protocols";

        case StatusCode::OK:
            return "OK";
        case StatusCode::Created:
            return "Created";
        case StatusCode::Accepted:
            return "Accepted";
        case StatusCode::NonAuthoritative:
            return "Non-Authoritative Information";
        case StatusCode::NoContent:
            return "No Content";
        case StatusCode::ResetContent:
            return "Reset Content";
        case StatusCode::PartialContent:
            return "Partial Content";

        // 300 range: redirects
        case StatusCode::MultipleChoices:
            return "Multiple Choices";
        case StatusCode::MovedPermanently:
            return "Moved Permanently";
        case StatusCode::Found:
            return "Found";
        case StatusCode::SeeOther:
            return "See Other";
        case StatusCode::NotModified:
            return "Not Modified";
        case StatusCode::UseProxy:
            return "Use Proxy";
        case StatusCode::TemporaryRedirect:
            return "OK";

        // 400 range: client errors
        case StatusCode::BadRequest:
            return "Bad Request";
        case StatusCode::Unauthorized:
            return "Unauthorized";
        case StatusCode::PaymentRequired:
            return "Payment Required";
        case StatusCode::Forbidden:
            return "Forbidden";
        case StatusCode::NotFound:
            return "Not Found";
        case StatusCode::MethodNotAllowed:
            return "Method Not Allowed";
        case StatusCode::NotAcceptable:
            return "Not Acceptable";
        case StatusCode::ProxyAuthRequired:
            return "Proxy Authentication Required";
        case StatusCode::RequestTimeout:
            return "Request Time-out";
        case StatusCode::Conflict:
            return "Conflict";
        case StatusCode::Gone:
            return "Gone";
        case StatusCode::LengthRequired:
            return "Length Required";
        case StatusCode::PreconditionFailed:
            return "Precondition Failed";
        case StatusCode::EntityTooLarge:
            return "Request Entity Too Large";
        case StatusCode::UriTooLong:
            return "Request-URI Too Large";
        case StatusCode::UnsupportedMediaType:
            return "Unsupported Media Type";
        case StatusCode::RangeNotSatisfiable:
            return "Requested range not satisfiable";
        case StatusCode::ExpectationFailed:
            return "Expectation Failed";
        case StatusCode::UpgradeRequired:
            return "Upgrade Required";

        // 500 range: server errors
        case StatusCode::InternalServerError:
            return "Internal Server Error";
        case StatusCode::NotImplemented:
            return "Not Implemented";
        case StatusCode::BadGateway:
            return "Bad Gateway";
        case StatusCode::Unavailable:
            return "Service Unavailable";
        case StatusCode::GatewayTimeout:
            return "Gateway Time-out";
        case StatusCode::VersionNotSupported:
            return "Version Not Supported";
    }
    assert(0);
    return "Unknown";
}

} // namespace http
} // namespace scy

/// @\}

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
