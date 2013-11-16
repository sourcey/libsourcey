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


#include "scy/thread.h"
#include "scy/logger.h"
#include "scy/platform.h"
#include "assert.h"
#include <memory>


using std::endl;


namespace scy {


Thread::Thread()
{
}


Thread::Thread(async::Runnable& target)
{
	start(target);
}


Thread::Thread(std::function<void()> target)
{
	start(target);
}


Thread::Thread(std::function<void(void*)> target, void* arg)
{
	start(target, arg);
}


Thread::~Thread()
{
}


void Thread::startAsync()
{
	int r = uv_thread_create(&_handle, [](void* arg) {
		auto& ptr = *reinterpret_cast<Runner::Context::ptr*>(arg);
		do {
			runAsync(ptr.get());
			scy::sleep(1); // TODO: uv_thread_yield when available
		} while (ptr->repeating && !ptr->cancelled());
		delete &ptr;
	}, new Runner::Context::ptr(pContext));
	if (r < 0) throw std::runtime_error("System error: Cannot initialize thread");	
}


void Thread::join()
{
	uv_thread_join(&_handle);
	//reset();
}


unsigned long Thread::currentID()
{
	return uv_thread_self();
}

	
bool Thread::async() const
{
	return true;
}


} // namespace scy





//void Thread::reset()
//{
	//_id = 0;
	//_running = false;
	//_started = false;
//}

/*
async::Runner::async::Runner(std::function<void()> target) :
	_running(false),
	_started(false)
{
	start(target);
}


async::Runner::async::Runner(std::function<void(void*)> target, void* arg) :
	_running(false),
	_started(false)
{
	start(target, arg);
}


async::Runner::async::Runner(async::Runnable& target) :
	_running(false),
	_started(false)
{
	start(target);
}
*/
/*
void async::Runner::reset()
{
	_running = false;
	_started = false;
}
*/
	
	//auto req = new async::Runner::Context;
	//Mutex::ScopedLock lock(_mutex);
	/*[](void* arg) {
		try {
			std::unique_ptr<async::Runner::Context> req(reinterpret_cast<async::Runner::Context*>(arg));
			req.get()->running = true;
			req.get()->target1(req.get()->arg);
			req.get()->self->reset();
		}
		catch (std::exception& exc) {
			std::cerr << "Runner error: " << exc.what() << std::endl;
#ifdef _DEBUG
			throw exc;
#endif
		}
	}, req*/


/*
void Thread::start(basic::Callable target, void* arg)
{
	Mutex::ScopedLock lock(_mutex);
	uv_thread_create(&_handle, target, arg);
}
*/


/*
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
		void* arg;
	};
}


void Thread::runAsync(void* arg, bool hasArg)
{
	Thread* self = nullptr;
	try {
		if (hasArg) {
			std::unique_ptr<internal::AsyncMethodArg> req(reinterpret_cast<internal::AsyncMethodArg*>(arg));
			self = req.get()->thread;
			id = uv_thread_self();
			running = true;
			req.get()->target(req.get()->arg);
		}
		else {
			std::unique_ptr<internal::AsyncMethod> req(reinterpret_cast<internal::AsyncMethod*>(arg));
			self = req.get()->thread;
			id = uv_thread_self();
			running = true;
			req.get()->target();
		}
	}
	catch (std::exception& exc) {
		cerr << "Thread error: " << exc.what() << endl;
#ifdef _DEBUG
		throw exc;
#endif
	}
	assert(self);
	if (self) {
		self->reset();
	}
}


void Thread::start(std::function<void()> target)
{
	Mutex::ScopedLock lock(_mutex);
	auto req = new internal::AsyncMethod;
	req->thread = this;
	req->target = target;	
	start([](void* arg) {
		runAsync(arg, false);
	}, req);
	_started = true;
}


void Thread::start(std::function<void(void*)> target, void* arg)
{
	Mutex::ScopedLock lock(_mutex);
	auto req = new internal::AsyncMethodArg;
	req->thread = this;
	req->target = target;	
	req->arg = arg;
	start([](void* arg) {
		runAsync(arg, true);
	}, req);
	_started = true;
}


void Thread::start(async::Runnable& target)
{
	Mutex::ScopedLock lock(_mutex);
	auto req = new internal::AsyncMethod;
	req->thread = this;
	req->target = std::bind(&async::Runnable::run, &target);
	start([](void* arg) {
		runAsync(arg, false);
	}, req);
	_started = true;
}
	_started = true;
*/


/*


bool Thread::running() const
{
	Mutex::ScopedLock lock(_mutex);
	return _running;
}


bool Thread::started() const
{
	Mutex::ScopedLock lock(_mutex);
	return _started;
	_running = false;
	_started = false;
}

void Thread::start(async::Runnable& target)
{
	Mutex::ScopedLock lock(_mutex);
	auto req = new internal::AsyncMethod;
	req->thread = this;
	req->target = std::bind(&async::Runnable::run, &target);
	uv_thread_create(&_handle, runAsync, req);
}
std::string Thread::name() const
{
	Mutex::ScopedLock lock(_mutex);	
	return _name;
}
*/