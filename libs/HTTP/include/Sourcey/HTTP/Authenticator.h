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


#ifndef SOURCEY_HTTP_Authenticator_H
#define SOURCEY_HTTP_Authenticator_H


#include "Sourcey/UserManager.h"
#include <string>


namespace Sourcey { 
namespace HTTP {


// ---------------------------------------------------------------------
//
// Authenticator
//
// ---------------------------------------------------------------------
class Authenticator
{
public:
	Authenticator();
	virtual ~Authenticator();

public:
	virtual bool validateRequest(UserManager* authenticator, const std::string& request) = 0;
	virtual std::string prepare401Header(const std::string& extra = "") = 0;
};


// ---------------------------------------------------------------------
//
// Anionu Authenticator
//
// ---------------------------------------------------------------------
class AnionuAuthenticator: public Authenticator
{
public:
	AnionuAuthenticator(const std::string& realm = "Spot", const std::string& version = "HTTP/1.1");
	virtual ~AnionuAuthenticator();

public:
	std::string prepare401Header(const std::string& extra = "");
	bool validateRequest(UserManager* authenticator, const std::string& request);	
	static std::string generateSignature(const std::string& password,  const std::string& requestUri, 
									     const std::string& httpMethod, const std::string& contentType, 
										 const std::string& date);
	static std::string generateAuthHeader(const std::string& username, const std::string& password, 
										  const std::string& requestUri, const std::string& httpMethod, 
										  const std::string& contentType, const std::string& date);

protected:
	std::string _protocol;
	std::string _version;
	std::string _realm;
	std::string _noonce;
	std::string _opaque;
};


// ---------------------------------------------------------------------
//
// Digest Authenticator
//
// ---------------------------------------------------------------------
class DigestAuthenticator: public Authenticator
{
public:
	DigestAuthenticator(const std::string& realm = "Spot", const std::string& version = "HTTP/1.1", bool usingRFC2617 = false);
	virtual ~DigestAuthenticator();

public:
	std::string prepare401Header(const std::string& extra = "");
	std::string parseHeaderSegment(const std::string& key);
	bool validateRequest(UserManager* authenticator, const std::string& request);
	std::string version() { return _version; };

protected:
	std::string _lastRequest;
	std::string _protocol;
	std::string _version;
	std::string _realm;
	std::string _noonce;
	std::string _opaque;
	bool _usingRFC2617;
};


} } // namespace Sourcey::HTTP


#endif // SOURCEY_HTTP_Authenticator_H