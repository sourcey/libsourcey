//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is is distributed under a dual license that allows free, 
// open source use and closed source use under a standard commercial
// license.
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


#include "Sourcey/HTTP/Authenticator.h"
#include "Sourcey/Logger.h"
#include "Sourcey/CryptoProvider.h"
#include "Sourcey/HTTP/Util.h"

#include "Poco/Base64Decoder.h"
#include "Poco/Base64Encoder.h"
#include "Poco/String.h"
#include "Poco/Format.h"


using namespace std;
using namespace Poco;


namespace Sourcey { 
namespace HTTP {


// ---------------------------------------------------------------------
//
// Digest Authenticator
//
// ---------------------------------------------------------------------
//
//	Client requests uri:
//	-------------------------------------
//
//	GET / HTTP/1.1
//
//
//	Server requires authentication:
//	-------------------------------------
//
//	HTTP/1.1 401 Unauthorized
//	WWW-Authenticate: Digest realm = "The batcave",
//		qop = "auth",
//		nonce = "4993927ba6279",
//		opaque = "d8ea7aa61ha1693024c4cc3a516f49b3c"
//
//
//	Client authenticates:
//	-------------------------------------
//
//	GET / HTTP/1.1
//	Authorization: Digest username = Anionu_API_USERNAME, 
//		realm = "The batcave", 
//		nonce=49938e61ccaa4, 
//		uri = "/", 
//		response = "98ccab4542f284c00a79b5957baaff23", 
//		opaque = "d8ea7aa61ha1693024c4cc3a516f49b3c", 
//		qop=auth, nc=00000001, 
//		cnonce = "8d1b34edb475994b" 
//
// ---------------------------------------------------------------------
DigestAuthenticator::DigestAuthenticator(const string& realm, const string& version, bool usingRFC2617) : 
	Authenticator(),
	_realm(realm),
	_version(version),
	_usingRFC2617(usingRFC2617),
	_opaque(CryptoProvider::hash("md5", realm))
{
	Log("trace") << "[DigestAuthenticator] Creating" << endl;
}


DigestAuthenticator::~DigestAuthenticator() 
{
	Log("trace") << "[DigestAuthenticator] Destroying" << endl;
}


string DigestAuthenticator::parseHeaderSegment(const string& key) 
{
	string value = "";
	string::size_type start, end = 0;
	start = _lastRequest.find(" "+key+" = ",0);
	if (start != string::npos) {
		start += key.length()+2;
		end = _lastRequest.find(",", start);
		if (end == string::npos) end = _lastRequest.find("\r", start);
		if (end == string::npos) end = _lastRequest.find("\n", start);
		if (end == string::npos) 
			return "";
		value = _lastRequest.substr(start, end-start);
		replaceInPlace(value,"\"", "");
	}
	//Log("trace") << format("Parse: Key: %s, Value: %s, start: %d, end: %d", key, value, start, end));
	return value;
}


bool DigestAuthenticator::validateRequest(UserManager* authenticator, const string& request) 
{
	Log("trace") << "[DigestAuthenticator] Validating Request: " + request << endl;

	_lastRequest = request;
	string hash;
	string httpMethod = request.substr(0, request.find_first_of(' '));
	string username = parseHeaderSegment("username");
	string uri = parseHeaderSegment("uri");
	string nonce = parseHeaderSegment("nonce");
	string nc = parseHeaderSegment("nc");
	string cnonce = parseHeaderSegment("cnonce");
	string qop = parseHeaderSegment("qop");
	string response = parseHeaderSegment("response");

	/*
	Log("trace") << "Digest Authenticator:"
		<< "\n\tHTTP Method: " << httpMethod
		<< "\n\tUsername: " << username
		<< "\n\tURI: " << uri
		<< "\n\tNoonce: " << nonce
		<< "\n\tNc: " << nc
		<< "\n\tCNoonce: " << cnonce
		<< "\n\tQOP: " << qop 
		<< "\n\tResponse: " << response 
		<< endl;
		*/

	if (!httpMethod.size() || !username.size() || !uri.size() || !response.size())
		return false;

	// Get the user details for the supplied username from our session object
	const IUser* user = authenticator->get(username);
	if (!user) 
		return false;

	string ha1 = CryptoProvider::hash("md5", format("%s:%s:%s", username, _realm, user->password()));
	string ha2 = CryptoProvider::hash("md5", format("%s:%s", httpMethod, uri));

	if (_usingRFC2617 && qop.size()) {
		// Using advanced digest authentication
		// Hash = md5(HA1:nonce:nc:cnonce:qop:HA2) 
		hash = CryptoProvider::hash("md5", format("%s:%s:%s:%s:%s:%s", ha1, nonce, nc, cnonce, qop, ha2));
	} else {
		// Using standard digest authentication
		// Hash = md5(HA1:nonce:HA2) 
		hash = CryptoProvider::hash("md5", format("%s:%s:%s", ha1, nonce, ha2));
	}

	return hash == response;
}


string DigestAuthenticator::prepare401Header(const string& extra) 
{
	_noonce = CryptoProvider::generateRandomKey(32);
	if (_usingRFC2617) {
		return format(
			"%s 401 Unauthorized\r\n"
			"%s"
			"WWW-Authenticate: Digest realm=\"%s\", qop=\"auth\", nonce=\"%s\", opaque=\"%s\"\r\n\r\n",
			_version, extra, _realm, _noonce, _opaque
		);
	} else {
		return format(
			"%s 401 Unauthorized\r\n"
			"%s"
			"WWW-Authenticate: Digest realm=\"%s\", nonce=\"%s\"\r\n\r\n",
			_version, extra, _realm, _noonce
		);
	}
}


} } // namespace Sourcey::HTTP
