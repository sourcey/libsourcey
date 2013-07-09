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
using namespace Poco;


namespace scy {
namespace smple {


Roster::Roster()
{	
	//traceL("Roster", this) << "Creating" << endl;
}
	

Roster::~Roster() 
{
	//traceL("Roster", this) << "Destroying" << endl;
}


void Roster::update(const json::Value& data, bool whiny)
{
	if (data.isObject() &&
		data.isMember("id") && 
		data.isMember("user") && 
		data.isMember("name") && 
		data.isMember("type")) {
		traceL("Roster", this) << "Updating: " << json::stringify(data, true) << endl;
		string id = data["id"].asString();
		Peer* peer = get(id, false);
		if (!peer) {
			peer = new Peer(data);
			add(id, peer);
		} else
			static_cast<json::Value&>(*peer) = data;
	}
	else if (data.isArray()) {
		for (json::ValueIterator it = data.begin(); it != data.end(); it++) {
			update(*it, whiny);	
		}
	}
	else {
		string error("Bad presence data: " + json::stringify(data));
		errorL("Roster", this) << error << endl;	
		if (whiny)
			throw Exception(error);
	}
}


Peer* Roster::getByHost(const string& host)
{
	FastMutex::ScopedLock lock(_mutex);
	for (PeerMap::const_iterator it = _store.begin(); it != _store.end(); ++it) {	
		if (it->second->host() == host)
			return it->second;
	}
	return NULL;
}


Roster::PeerMap Roster::peers() const 
{ 
	FastMutex::ScopedLock lock(_mutex);
	return _store; 
}


void Roster::print(ostream& os) const
{
	FastMutex::ScopedLock lock(_mutex);

	os << "Roster[";
	for (PeerMap::const_iterator it = _store.begin(); it != _store.end(); ++it) {	
		os << "\n\t" << it->second << ": " << it->first;
	}
	os << "\n]";
}


} } // namespace scy::smple