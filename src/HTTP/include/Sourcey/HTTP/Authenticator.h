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


#ifndef SOURCEY_HTTP_Authenticator_H
#define SOURCEY_HTTP_Authenticator_H


#include "Sourcey/HTTP/URL.h"
#include <string>


namespace scy {
namespace http {

	
class Request;
class Response;


class Authenticator
	/// This is a utility class for working with HTTP
	/// authentication (basic or digest) in http::Request objects.
	///
	/// Note: Do not forget to read the entire response stream from the 401 response
	/// before sending the authenticated request, otherwise there may be
	/// problems if a persistent connection is used.
{
public:
	Authenticator();
		// Creates an empty Authenticator object.

	Authenticator(const std::string& username, const std::string& password);
		// Creates an Authenticator object with the given username and password.

	~Authenticator();
		// Destroys the Authenticator.

	void fromUserInfo(const std::string& userInfo);
		// Parses username:password std::string and sets username and password of
		// the credentials object.
		// Throws SyntaxException on invalid user information.

	void fromURI(const http::URL& uri);
		// Extracts username and password from the given URI and sets username
		// and password of the credentials object.
		// Does nothing if URI has no user info part.

	void setUsername(const std::string& username);
		// Sets the username.

	const std::string& username() const;
		// Returns the username.

	void setPassword(const std::string& password);
		// Sets the password.

	const std::string& password() const;
		// Returns the password.

	void authenticate(http::Request& request, const http::Response& response);
		// Inspects WWW-Authenticate header of the response, initializes
		// the internal state (in case of digest authentication) and
		// adds required information to the given http::Request.
		//
		// Does nothing if there is no WWW-Authenticate header in the
		// http::Response.

	void updateAuthInfo(http::Request& request);
		// Updates internal state (in case of digest authentication) and
		// replaces authentication information in the request accordingly.

	void proxyAuthenticate(http::Request& request, const http::Response& response);
		// Inspects Proxy-Authenticate header of the response, initializes
		// the internal state (in case of digest authentication) and
		// adds required information to the given http::Request.
		//
		// Does nothing if there is no Proxy-Authenticate header in the
		// http::Response.

	void updateProxyAuthInfo(http::Request& request);
		// Updates internal state (in case of digest authentication) and
		// replaces proxy authentication information in the request accordingly.

private:
	Authenticator(const Authenticator&);
	Authenticator& operator = (const Authenticator&);
	
	std::string _username;
	std::string _password;
};


//
// Basic Authenticator (rfc2617)
//


class BasicAuthenticator
	/// This is a utility class for working with HTTP Basic
	/// Authentication in http::Request objects.
{
public:
	BasicAuthenticator();
		// Creates an empty BasicAuthenticator object.
		
	BasicAuthenticator(const std::string& username, const std::string& password);
		// Creates a BasicAuthenticator object with the given username and password.

	explicit BasicAuthenticator(const http::Request& request);
		// Creates a BasicAuthenticator object with the authentication information
		// from the given request.
		//
		// Throws a NotAuthenticatedException if the request does
		// not contain basic authentication information.

	explicit BasicAuthenticator(const std::string& authInfo);
		// Creates a BasicAuthenticator object with the authentication information
		// in the given std::string. The authentication information can be extracted
		// from a http::Request object by calling http::Request::getCredentials().

	~BasicAuthenticator();
		// Destroys the BasicAuthenticator.

	void setUsername(const std::string& username);
		// Sets the username.
		
	const std::string& username() const;
		// Returns the username.
		
	void setPassword(const std::string& password);
		// Sets the password.
		
	const std::string& password() const;
		// Returns the password.
		
	void authenticate(http::Request& request) const;
		// Adds authentication information to the given http::Request.

	void proxyAuthenticate(http::Request& request) const;
		// Adds proxy authentication information to the given http::Request.

protected:
	void parseAuthInfo(const std::string& authInfo);
		// Extracts username and password from Basic authentication info
		// by base64-decoding authInfo and splitting the resulting
		// std::string at the ':' delimiter.

private:
	BasicAuthenticator(const BasicAuthenticator&);
	BasicAuthenticator& operator = (const BasicAuthenticator&);
	
	std::string _username;
	std::string _password;
};


//
// Helpers
//


bool isBasicCredentials(const std::string& header);
	// Returns true if authentication header is for Basic authentication.

bool isDigestCredentials(const std::string& header);
	// Returns true if authentication header is for Digest authentication.

bool hasBasicCredentials(const http::Request& request);
	// Returns true if Authorization with Basic credentials header is present in the request.

bool hasDigestCredentials(const http::Request& request);
	// Returns true if Authorization with Digest credentials header is present in the request.

bool hasProxyBasicCredentials(const http::Request& request);
	// Returns true if Authorization with Basic credentials header is present in the request.

bool hasProxyDigestCredentials(const http::Request& request);
	// Returns true if Authorization with Digest credentials header is present in the request.

void extractCredentials(const std::string& userInfo, std::string& username, std::string& password);
	// Extracts username and password from user:password information std::string.

void extractCredentials(const http::URL& uri, std::string& username, std::string& password);
	// Extracts username and password from the given URI (e.g.: "http://user:pass@sample.com/secret").


} } // namespace scy::http


#endif // SOURCEY_HTTP_Authenticator_H


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