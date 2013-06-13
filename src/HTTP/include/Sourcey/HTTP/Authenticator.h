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


#include "Sourcey/Util/UserManager.h"
#include <string>


namespace Scy { 
namespace HTTP {


// ---------------------------------------------------------------------
// Authenticator
//
class Authenticator
{
public:
	virtual ~Authenticator() {};

public:
	virtual bool validateRequest(UserManager* authenticator, const std::string& request) = 0;
	virtual std::string prepare401Header(const std::string& extra = "") = 0;
};


// ---------------------------------------------------------------------
// Digest Authenticator
//
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


} } // namespace Scy::HTTP


#endif // SOURCEY_HTTP_Authenticator_H