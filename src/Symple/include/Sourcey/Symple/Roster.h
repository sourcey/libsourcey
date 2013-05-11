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


#ifndef SOURCEY_Symple_Router_H
#define SOURCEY_Symple_Router_H


#include "Sourcey/EventfulManager.h"
#include "Sourcey/Symple/Peer.h"
#include "Sourcey/Symple/Address.h"


namespace Scy {
namespace Symple {


class Roster: public EventfulManager<std::string, Peer>
	/// The Roster provided a registry for active network peers
	/// indexed by their current session ID.
{
public:
	typedef EventfulManager<std::string, Peer>	Manager;
	typedef Manager::Map						PeerMap;	
	
public:
	Roster();
	virtual ~Roster();

	virtual void update(const JSON::Value& data, bool whiny = false);
		/// Updates the roster from the given client object.
	
	//virtual void setOurID(const Address& id);

    //virtual Address ourAddress() const;
    //virtual Peer* ourPeer(bool whiny = true);
		/// Returns the peer object that
		/// controls the current session.
	
	virtual PeerMap peers() const;
	
	virtual void print(std::ostream& os) const;

	virtual const char* className() const { return "Symple::Roster"; }

protected:

	//Address _ourAddress;
};
	

} } // namespace Scy::Symple


#endif //  SOURCEY_Symple_Router_H

