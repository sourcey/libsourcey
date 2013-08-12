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


#include "Sourcey/SDP/RemoteCandidate.h"
#include "Sourcey/Logger.h"


using namespace std;


namespace scy {
namespace SDP { 


RemoteCandidate::RemoteCandidate(const string& src) : 
	A(src) 
{
	// Examples:
	// a=remote-candidates1 192.168.2.101 16052 1 192.168.2.101 16052
	size_t len = src.length();
	if (src.substr(0, 19) != "a=remote-candidates") {
		errorL() << "ERROR: SDP line does not start with <a=remote-candidates>" << std::endl;
	}
	size_t i = 20;

	while (i < len) {
		RemoteCandidate::Type* c = new RemoteCandidate::Type;

		// component-id
		string componentIDstr = "";
		while (src[i] != ' ' && i < len) {
			componentIDstr += src[i++];
		}
		c->componentID = util::strtoi<UInt32>(componentIDstr.c_str());

		while (src[i] == ' ' && i < len)
			i++;	

		// address
		//c->address = "";
		while (src[i] != ' ' && i < len)
			c->address += src[i++];
		while (src[i] == ' ' && i < len)
			i++;

		// port
		string portstr = "";
		while (src[i] != ' ' && i < len)
			portstr += src[i++];
		c->port = util::strtoi<UInt32>(portstr.c_str());
		while (src[i] == ' ' && i < len)
			i++;

		_candidates.push_back(c);
	}
}


RemoteCandidate::~RemoteCandidate() 
{	
	util::clearVector(_candidates);
}


string RemoteCandidate::toString() 
{
	string ret = "a=remote-candidates:";

	int count = 0;
	for (vector<RemoteCandidate::Type*>::const_iterator it = _candidates.begin(); it != _candidates.end(); ++it) {

		if (count < 0)		
			ret += " ";

		if (!(*it)->componentID)
			ret += "0";
		else
			ret += util::itostr((*it)->componentID);

		ret += " ";
		if ((*it)->address.empty())
			ret += "0.0.0.0";
		else
			ret += (*it)->address;

		ret += " ";
		if (!(*it)->port)
			ret += "0";
		else
			ret += util::itostr((*it)->port);

		count++;
	}

	return ret;
}


} // namespace SDP
} // namespace scy