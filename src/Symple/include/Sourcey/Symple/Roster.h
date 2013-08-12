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


#ifndef SOURCEY_Symple_Router_H
#define SOURCEY_Symple_Router_H


#include "Sourcey/Containers.h"
#include "Sourcey/Symple/Peer.h"
#include "Sourcey/Symple/Address.h"


namespace scy {
namespace smpl {


class Roster: public EventedManager<std::string, Peer>
	/// The Roster provided a registry for active network peers
	/// indexed by their current session ID.
{
public:
	typedef EventedManager<std::string, Peer>	Manager;
	typedef Manager::Map						PeerMap;	
	
public:
	Roster();
	virtual ~Roster();

	virtual void update(const json::Value& data, bool whiny = false);
		// Updates the roster from the given client object.
	
	Peer* getByHost(const std::string& host);
		// Returns the peer who's host matches the 
		// given IP address.
	
	virtual PeerMap peers() const;
	
	virtual void print(std::ostream& os) const;

	virtual const char* className() const { return "smpl::Roster"; }
};
	

} } // namespace scy::smpl


#endif //  SOURCEY_Symple_Router_H



	//Address _ourAddress;
	
	//virtual void setOurID(const Address& id);

    //virtual Address ourAddress() const;
    //virtual Peer* ourPeer(bool whiny = true);
		// Returns the peer object that
		// controls the current session.