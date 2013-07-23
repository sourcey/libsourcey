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


#ifndef SOURCEY_Symple_Peer_H
#define SOURCEY_Symple_Peer_H


#include "Sourcey/JSON/JSON.h"
#include "Sourcey/Symple/Address.h"


namespace scy {
namespace smpl {


class Peer: public json::Value
	/// This class represents a peer on the network.
	/// A Peer object may also contain arbitrary data set by
	/// the client to share with other peers on the network.
	/// @see Address for further methods and basic accessors.
{
public:		
	Peer();
	Peer(const Peer& r);
	Peer(const json::Value& r);
	virtual ~Peer();

	Address address() const;
	
	std::string id() const;
	std::string user() const;
	std::string name() const;
	std::string group() const;
	std::string type() const;
	std::string host() const;
	
	void setID(const std::string& id);	
	void setUser(const std::string& user);
	void setName(const std::string& name);
	void setGroup(const std::string& group);
	void setType(const std::string& type);
	void setHost(const std::string& host);

	virtual bool valid();	
	
	void print(std::ostream& os) const;
	
    friend std::ostream& operator << (std::ostream& os, const Peer& peer)
	{
		peer.print(os);
		return os;
	}
	
	virtual const char* className() const { return "smpl::Peer"; }
};


} // namespace symple 
} // namespace scy


#endif // SOURCEY_Symple_Peer_H
