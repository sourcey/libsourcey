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


#ifndef SOURCEY_TURN_ServerAllocation_H
#define SOURCEY_TURN_ServerAllocation_H


#include "Sourcey/TURN/IAllocation.h"
#include "Sourcey/TURN/FiveTuple.h"


namespace Sourcey {
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

	virtual const char* className() const { return "ServerAllocation"; };

protected:
	Server&	_server;
	UInt32	_maxLifetime;
};


typedef std::map<FiveTuple, ServerAllocation*> ServerAllocationMap;


} } // namespace Sourcey::TURN


#endif // SOURCEY_TURN_ServerAllocation_H