//
// This software is copyright by Sourcey <mail@sourcey.com> and is distributed under a dual license:
// Copyright (C) 2002 Sourcey
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


#include "Sourcey/Router.h"
#include "Sourcey/Logger.h"
#include "assert.h"


using namespace std; 
using namespace Poco;


namespace Sourcey {


Router::Router(Runner& runner) :
	_task(new PacketQueue(*this, runner)),
	_runner(runner)
{	
	_task->start();
}


Router::~Router() 
{
	_task->destroy(); // The Runner will free the pointer
}


void Router::send(const IPacket& packet)
{
	Log("debug") << "[Router] Queuing: " << packet.className() << endl;

	Mutex::ScopedLock lock(_mutex);
	_task->push(packet.clone());
}


Runner& Router::runner() 
{ 
	Mutex::ScopedLock lock(_mutex);
	return _runner; 
}


Router& Router::operator >> (IPacket& packet) 
{
	send(packet);
	return *this;
}
	

} // namespace Sourcey