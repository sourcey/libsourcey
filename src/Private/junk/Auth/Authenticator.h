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


#ifndef ANIONU_Authenticator_H
#define ANIONU_Authenticator_H


#include "Sourcey/HTTP/Authenticator.h"
#include "Sourcey/Util/UserManager.h"
#include <string>


namespace Anionu {


// ---------------------------------------------------------------------
// Anionu Authenticator
//
class Authenticator: public Scy::HTTP::Authenticator
{
public:
	Authenticator(const std::string& realm = "Spot", const std::string& version = "HTTP/1.1");
	virtual ~Authenticator();

public:
	std::string prepare401Header(const std::string& extra = "");
	bool validateRequest(Scy::UserManager* authenticator, const std::string& request);	
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


} // namespace Anionu


#endif // ANIONU_Authenticator_H