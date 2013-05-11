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


#ifndef SOURCEY_UserManager_H
#define SOURCEY_UserManager_H


#include "Sourcey/EventfulManager.h"
#include <map>


namespace Scy {


struct IUser 
{
	virtual std::string username() const = 0;
	virtual std::string password() const = 0;
};


class BasicUser: public IUser 
{
public:
	BasicUser(const std::string& username, 
			  const std::string& password = "") :
		_username(username), 
		_password(password) {}
	
	std::string username() const { return _username; }
	std::string password() const { return _password; }

protected:
	std::string _username;
	std::string _password;
};


typedef std::map<std::string, IUser*> IUserMap;


class UserManager: public EventfulManager<std::string, IUser>
	/// This class contains a list of users that have access
	/// on the system.
	///
	/// NOTE: This class is depreciated.
{
public:
	typedef EventfulManager<std::string, IUser>	Manager;
	typedef Manager::Map						Map;

public:
	UserManager() {};
	virtual ~UserManager() {};

	virtual bool add(IUser* user) {
		return Manager::add(user->username(), user);
	};
};


} // namespace Scy


#endif // SOURCEY_UserManager_H
