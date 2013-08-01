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


#include "Sourcey/Thread.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Platform.h"
#include "assert.h"


using namespace std;


namespace scy {
	

Thread::Thread() :
	_running(false),
	_id(0)
{
}

Thread::Thread(const std::string& name) :
	_running(false),
	_name(name),
	_id(0)
{
}


Thread::~Thread()
{
}


namespace internal {
	struct ThreadReq
	{
		Thread* thread;
		abstract::Runnable* target;
	};
}


void Thread::runAsync(void* arg)
{
	traceL("Thread") << "Running" << endl;
	internal::ThreadReq* req = reinterpret_cast<internal::ThreadReq*>(arg);
	try {
		req->thread->_id = uv_thread_self();
		req->thread->_running = true;
		
		while (req->target->run()) {	
			scy::sleep(5);
		}
	}
	catch (std::exception& exc) {
		traceL("Thread") << "Error:" << exc.what() << endl;
#ifdef _DEBUG
		throw exc;
#endif
	}
	req->thread->_id = 0;
	req->thread->_running = false;
	delete req;
	traceL("Thread") << "Ending" << endl;
}


void Thread::start(abstract::Runnable& target)
{
	ScopedLock lock(_mutex);
	internal::ThreadReq* req = new internal::ThreadReq;
	req->thread = this;
	req->target = &target;
	uv_thread_create(&_handle, runAsync, req);
}


void Thread::start(Callable target, void* opaque)
{
	ScopedLock lock(_mutex);
	uv_thread_create(&_handle, target, opaque);
}


void Thread::join()
{
	ScopedLock lock(_mutex);
	uv_thread_join(&_handle);
}


unsigned long Thread::id() const
{
	ScopedLock lock(_mutex);
	return _id;
}


std::string Thread::name() const
{
	ScopedLock lock(_mutex);	
	return _name;
}


unsigned long Thread::currentID() const
{
	return uv_thread_self();
}


bool Thread::running() const
{
	ScopedLock lock(_mutex);
	return _running;
}



} // namespace scy