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


#include "Sourcey/Anionu/Authenticator.h"
#include "Sourcey/Logger.h"
#include "Sourcey/CryptoProvider.h"
#include "Sourcey/HTTP/Util.h"

#include "Poco/Base64Decoder.h"
#include "Poco/Base64Encoder.h"
#include "Poco/String.h"
#include "Poco/Format.h"


using namespace std;
using namespace Poco;
using namespace Scy;


namespace Anionu {


// ---------------------------------------------------------------------
// Anionu HTTP Authenticator
//
Authenticator::Authenticator(const string& realm, const string& version)
{
	_realm = realm;
	_opaque = CryptoProvider::hash("md5", _realm);
	_version = version;
}


Authenticator::~Authenticator() 
{
}


bool Authenticator::validateRequest(UserManager* authenticator, const string& request) 
{
	LogDebug() << "[Authenticator] Validating Request: " + request << endl;
		
	string httpMethod = request.substr(0, request.find_first_of(' '));
	string requestUri = Util::parseURI(request);	
	string authorization = Util::parseHeader(request, "Authorization");
	string clientUsername = Util::parseHeader(request, "Client-Username");
	string contentType = Util::parseHeader(request, "Content-Type");
	string date = Util::parseHeader(request, "Date");

	/*
	LogDebug() << "Anionu Validating Request:"
		<< "\n\tHTTP Method: " << httpMethod
		<< "\n\tURI: " << requestUri
		<< "\n\tAuthorization: " << authorization
		<< "\n\tClient-Username: " << clientUsername
		<< "\n\tContent-Type: " << contentType
		<< "\n\tDate: " << date
		<< endl;
		*/

	if (!httpMethod.size() || !requestUri.size() || !authorization.size())
		return false;
	
	replaceInPlace(authorization, "Anionu ", "");
	StringVec tokens = Util::split(authorization,":");
	if (tokens.size() != 2)
		return false;

	string username = tokens[0];
	string signature = tokens[1];
	if (!username.size() || !signature.size())
		return false;

	// Check the user exists and retrieve their password
	const IUser* user = authenticator->get(username);
	if (!user) 
		return false;

	string ourSignature = generateSignature(user->password(), httpMethod, requestUri, contentType, date);

	// Check equality
	return ourSignature == signature;
}


// Anionu API authentication header is computed as follows:
// Authorization: Anionu username:Base64(SHA256(access-key:http-method:request-uri:content-type:date))
string Authenticator::generateSignature(const string& password, const string& httpMethod, 
										const string& requestUri, const string& contentType, 
										const string& date)
{

	ostringstream ostr;
	Base64Encoder encoder(ostr);
	encoder << CryptoProvider::hash("sha1", 
		password +
		httpMethod +
		requestUri +
		contentType +
		date
	);
	encoder.close();
	
	/*
	LogDebug() << "Anionu Generating Signature:"
		<< "\n\tPassword: " << password
		<< "\n\tMethod: " << httpMethod
		<< "\n\tURI: " << requestUri
		<< "\n\tContent-Type: " << contentType
		<< "\n\tDate: " << date
		<< endl;
		*/
	return ostr.str();
}


// Anionu API authentication header is computed as follows:
// Anionu username:Base64(SHA256(access-key:Client-Username:http-method:request-uri:content-type:date))
string Authenticator::generateAuthHeader(const std::string& username, const string& password, /*const string& clientUsername,*/ const string& httpMethod, 
												 const string& requestUri, const string& contentType, const string& date)
{	
	return format(
		"Anionu %s:%s", 
		username, 
		generateSignature(password, /*clientUsername,*/ httpMethod, requestUri, contentType, date)
	);
}


string Authenticator::prepare401Header(const string& extra) 
{
	return format(
		"%s 401 Unauthorized\r\n"
		"%s"
		"WWW-Authenticate: Anionu realm=\"%s\"\r\n"
		"\r\n",
		_version,
		extra, 
		_realm
	);
}


} // namespace Anionu
