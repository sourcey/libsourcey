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


#ifndef SOURCEY_TURN_ServerAllocation_H
#define SOURCEY_TURN_ServerAllocation_H


#include "Sourcey/TURN/IAllocation.h"
#include "Sourcey/TURN/FiveTuple.h"


namespace Scy {
namespace TURN {


class Server;


class ServerAllocation: public IAllocation
{
public:
	ServerAllocation(Server& server, 
					 const FiveTuple& tuple, 
					 const std::string& username, 
					 UInt32 lifetime);
	virtual ~ServerAllocation();
		/// IMPORTANT: The destructor should never be called directly.
		/// The allocation is always deleted via the timer callback
		/// @see onTimer()
	
	virtual UInt32 timeRemaining(); 
	
	virtual bool handleRequest(const Request& request);	
	virtual void handleRefreshRequest(const Request& request);	
	virtual void handleCreatePermission(const Request& request);
	
	virtual bool onTimer();
		/// Asynchronous timer callback for updating the allocation
		/// permissions and state etc.
		/// If this call returns false the allocation will be deleted.

	virtual Server& server(); 

	virtual const char* className() const { return "ServerAllocation"; };

protected:
	Server&	_server;
	UInt32	_maxLifetime;
};


typedef std::map<FiveTuple, ServerAllocation*> ServerAllocationMap;


} } // namespace Scy::TURN


#endif // SOURCEY_TURN_ServerAllocation_H