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


#include "Sourcey/Router.h"
#include "Sourcey/Logger.h"
#include "assert.h"


using namespace std; 
using namespace Poco;


namespace Scy {


Router::Router(Runner& runner) :
	_task(new PacketQueue(*this, runner)),
	_runner(runner)
{	
	//_task->start();
	_runner.start(_task);
}


Router::~Router() 
{
	//_task->destroy();
	_runner.destroy(_task);  // The Runner will free the task pointer
}


void Router::send(const IPacket& packet)
{
	LogDebug() << "[Router] Queuing: " << packet.className() << endl;

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
	

} // namespace Scy