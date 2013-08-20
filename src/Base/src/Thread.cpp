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
#include <memory>


using namespace std;


namespace scy {
	

Thread::Thread() :
	_running(false),
	_id(0)
{
}


Thread::Thread(basic::Runnable& target) :
	_running(false),
	_id(0)
{
	start(target);
}


Thread::Thread(std::function<void()> target) :
	_running(false),
	_id(0)
{
	start(target);
}


Thread::Thread(std::function<void(void*)> target, void* opaque) :
	_running(false),
	_id(0)
{
	start(target, opaque);
}


Thread::~Thread()
{
}


namespace internal {
	struct AsyncMethod
	{
		Thread* thread;
		std::function<void()> target;
	};

	struct AsyncMethodArg
	{
		Thread* thread;
		std::function<void(void*)> target;
		void* opaque;
	};
}


void Thread::runAsync(void* arg, bool hasArg)
{
	Thread* self = nullptr;
	try {
		if (hasArg) {
			std::unique_ptr<internal::AsyncMethodArg> req(reinterpret_cast<internal::AsyncMethodArg*>(arg));
			self = req.get()->thread;
			self->_id = uv_thread_self();
			self->_running = true;
			req.get()->target(req.get()->opaque);
		}
		else {
			std::unique_ptr<internal::AsyncMethod> req(reinterpret_cast<internal::AsyncMethod*>(arg));
			self = req.get()->thread;
			self->_id = uv_thread_self();
			self->_running = true;
			req.get()->target();
		}
	}
	catch (std::exception& exc) {
		cerr << "thread error: " << exc.what() << endl;
#ifdef _DEBUG
		throw exc;
#endif
	}
	assert(self);
	if (self) {
		self->_id = 0;
		self->_running = false;
	}
}


void Thread::start(std::function<void()> target)
{
	Mutex::ScopedLock lock(_mutex);
	auto req = new internal::AsyncMethod;
	req->thread = this;
	req->target = target;	
	startC([](void* arg) {
		runAsync(arg, false);
	}, req);
}


void Thread::start(std::function<void(void*)> target, void* opaque)
{
	Mutex::ScopedLock lock(_mutex);
	auto req = new internal::AsyncMethodArg;
	req->thread = this;
	req->target = target;	
	req->opaque = opaque;
	startC([](void* arg) {
		runAsync(arg, true);
	}, req);
}


void Thread::start(basic::Runnable& target)
{
	Mutex::ScopedLock lock(_mutex);
	auto req = new internal::AsyncMethod;
	req->thread = this;
	req->target = std::bind(&basic::Runnable::run, &target);
	startC([](void* arg) {
		runAsync(arg, false);
	}, req);
}



void Thread::startC(basic::Callable target, void* opaque)
{
	Mutex::ScopedLock lock(_mutex);
	uv_thread_create(&_handle, target, opaque);
}


void Thread::join()
{
	uv_thread_join(&_handle);
}


unsigned long Thread::id() const
{
	Mutex::ScopedLock lock(_mutex);
	return _id;
}


unsigned long Thread::currentID()
{
	return uv_thread_self();
}


bool Thread::running() const
{
	Mutex::ScopedLock lock(_mutex);
	return _running;
}


} // namespace scy






/*
void Thread::start(basic::Runnable& target)
{
	Mutex::ScopedLock lock(_mutex);
	auto req = new internal::AsyncMethod;
	req->thread = this;
	req->target = std::bind(&basic::Runnable::run, &target);
	uv_thread_create(&_handle, runAsync, req);
}
std::string Thread::name() const
{
	Mutex::ScopedLock lock(_mutex);	
	return _name;
}
*/