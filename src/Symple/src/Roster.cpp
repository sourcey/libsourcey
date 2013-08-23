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


#include "Sourcey/Symple/Roster.h"
#include "Sourcey/Logger.h"


using namespace std;



namespace scy {
namespace smpl {


Roster::Roster()
{	
	//traceL("Roster", this) << "Create" << endl;
}
	

Roster::~Roster() 
{
	//traceL("Roster", this) << "Destroy" << endl;
}


void Roster::update(const json::Value& data, bool whiny)
{
	if (data.isObject() &&
		data.isMember("id") && 
		data.isMember("user") && 
		data.isMember("name") && 
		data.isMember("type")) {
		traceL("Roster", this) << "Updating: " << json::stringify(data, true) << endl;
		std::string id = data["id"].asString();
		Peer* peer = get(id, false);
		if (!peer) {
			peer = new Peer(data);
			add(id, peer);
		} else
			static_cast<json::Value&>(*peer) = data;
	}
	else if (data.isArray()) {
		for (auto it = data.begin(); it != data.end(); it++) {
			update(*it, whiny);	
		}
	}
	else {
		std::string error("Bad presence data: " + json::stringify(data));
		errorL("Roster", this) << error << endl;	
		if (whiny)
			throw std::runtime_error(error);
	}
}


Peer* Roster::getByHost(const std::string& host)
{
	Mutex::ScopedLock lock(_mutex);
	for (PeerMap::const_iterator it = _map.begin(); it != _map.end(); ++it) {	
		if (it->second->host() == host)
			return it->second;
	}
	return NULL;
}


Roster::PeerMap Roster::peers() const 
{ 
	Mutex::ScopedLock lock(_mutex);
	return _map; 
}


void Roster::print(std::ostream& os) const
{
	Mutex::ScopedLock lock(_mutex);

	os << "Roster[";
	for (PeerMap::const_iterator it = _map.begin(); it != _map.end(); ++it) {	
		os << "\n\t" << it->second << ": " << it->first;
	}
	os << "\n]";
}


} } // namespace scy::smpl