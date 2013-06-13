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


#ifndef SOURCEY_Symple_Address_H
#define SOURCEY_Symple_Address_H


#include "Sourcey/JSON/JSON.h"


namespace Scy {
namespace Symple {


class Address: public JSON::Value
	/// The Address structure contains identity information about
	/// an entity on the network.
{
public:
	Address();
	Address(const Address& r);
	Address(const JSON::Value& root);
		/// The provided JSON value should be an object containing
		/// the group, user, name and id string properties.
	virtual ~Address();
	
	std::string id() const;
	std::string user() const;
	std::string name() const;
	std::string group() const;
	
	void setID(const std::string& id);	
	void setUser(const std::string& user);
	void setName(const std::string& name);
	void setGroup(const std::string& group);
	
	bool valid() const;
	void print(std::ostream& os) const;
	
	bool operator == (const Address& r);
	
    friend std::ostream& operator << (std::ostream& os, const Address& addr)
	{
		addr.print(os);
		return os;
	}
	
	/*

	//std::string group;const std::string& id
	//std::string user;
	//std::string name;
	//std::string id;

	bool parse(const JSON::Value& root);
		id.print(os);
	std::string toString() const;
	bool operator == (std::string& r);

protected:
	JSON::Value* _root;
		/// The root pointer is just a reference
		/// to an externally managed JSON value,
		/// generally a Message instance.
	*/

};


} // namespace Symple 
} // namespace Scy


#endif // SOURCEY_Symple_Address_H
