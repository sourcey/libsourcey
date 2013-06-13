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


#include "Sourcey/UV/AsyncLoop.h"

#include "Sourcey/Logger.h"


using namespace std;
//using namespace Poco;


namespace Sourcey {
namespace UV {


AsyncLoop::AsyncLoop(LoopHandle* loop) :
	_loop(loop), 
	_stop(false)
{
	Log("debug") << "[AsyncLoop:" << this << "] Creating" << endl;
	_thread.start(*this);
}


AsyncLoop::~AsyncLoop() 
{	
	Log("debug") << "[AsyncLoop:" << this << "] Destroying" << endl;
}


void AsyncLoop::run() 
{
	while (!_stop) {
		Log("debug") << "[AsyncLoop:" << this << "] Starting Loop" << endl;
		uv_run(_loop);
		//Sleep(100);
	}
}


LoopHandle* AsyncLoop::loop()
{
	return _loop;
}

	
/*
TCPContext* AsyncLoop::allocateTCPContext()
{
	TCPContext* socket = new TCPContext(_loop);	
	//uv_run(_loop);
	return socket;
}
*/



} } // namespace Sourcey::UV
