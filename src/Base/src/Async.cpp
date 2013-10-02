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


#include "Sourcey/Async.h"
#include "Sourcey/Logger.h"
#include <memory>


namespace scy {
namespace async {
	

Runner::Runner()
{
	pContext = std::make_shared<Runner::Context>();
}


Runner::~Runner()
{
	// Always call cancel so the async context can exit ASAP.
	cancel();
}


void Runner::runAsync(Context::ptr ctx)
{
	auto c = ctx.get();
	if (!c->cancelled()) 
	{
		try {
			c->running = true;
			if (c->target) {
				c->target();
			} else if (
				c->target1) {
				c->target1(c->arg);
			}
			else {
				// Ensure runAsync is not being hmmered by the
				// calling thread after cancelled and reset.
				assert(c->cancelled() && "no callback target");
				throw std::runtime_error("Async callback has no target");
			}
		}
		catch (std::exception& exc) {
			errorL("RunnerContext") << "Runner error: " << ctx << std::endl;	
	#ifdef _DEBUG
			//throw exc;
	#endif
		}
	}
	
	c->running = false;
	if (c->cancelled()) {
		// Once cancelled we release callback functions to allow freeing   
		// of memory allocated by std::shared_ptr managed pointers used to
		// std::bind the std::function.
		c->target = nullptr;
		c->target1 = nullptr;

		//c->reset();
		//c->cancel();
	}
}


void Runner::start(async::Runnable& target)
{
	if (started())
		throw std::runtime_error("Runner context already active");

	pContext->target = std::bind(&async::Runnable::run, &target);
	pContext->arg = nullptr;
	pContext->running = false;
	pContext->started = true;
	startAsync();
}


void Runner::start(std::function<void()> target)
{
	if (started())
		throw std::runtime_error("Runner context already active");

	pContext->target = target;	
	pContext->arg = nullptr;
	pContext->running = false;
	pContext->started = true;
	startAsync();
}


void Runner::start(std::function<void(void*)> target, void* arg)
{
	if (started())
		throw std::runtime_error("Runner context already active");
	
	pContext->target1 = target;	
	pContext->arg = arg;
	pContext->running = false;
	pContext->started = true;
	startAsync();
}


bool Runner::running() const
{
	return pContext->running;
}


bool Runner::started() const
{
	return pContext->started;
}


void Runner::cancel()
{
	pContext->cancel();
}


bool Runner::cancelled() const
{
	return pContext->cancelled();
}
	

//
// Runner Context
//
	

void Runner::Context::cancel()
{
	exit.store(true, std::memory_order_release);
}


bool Runner::Context::cancelled() const
{
	bool s = exit.load(std::memory_order_relaxed);
	if (s) std::atomic_thread_fence(std::memory_order_acquire);
	return s;
}


} } // namespace scy::basic



	//Async* self = nullptr;
		//std::unique_ptr<Runner::Context> req(reinterpret_cast<Runner::Context*>(arg));
		//self = req->self;
		//self->_running = true;


//void Runner::start(async::Runnable& target)
//{
//	//Mutex::ScopedLock lock(_mutex);
//	auto req = new Runner::Context;
//	req->thread = this;
//	req->target = std::bind(&async::Runnable::run, &target);
//	start([](void* arg) {
//		runAsync(arg, false);
//	}, req);
//	_started = true;
//}


/*

	
	//if (c->cancelled()) {
	//	std::cerr << "Runner exiting" << std::endl;
	//	if (c->onexit)
	//		c->onexit();
	//}

void Runner::onexit(std::function<void()> target)
{
	if (started())
		throw std::runtime_error("Runner context already active");

	pContext->onexit = target;
}

void Runner::start(basic::Callable target, void* opaque)
{
	//Mutex::ScopedLock lock(_mutex);
	//uv_thread_create(&_handle, target, opaque);
	//_started = true;
}
*/


//Runner::Async(async::Runnable& target) :
//	_running(false),
//	_started(false)
//{
//	start(target);
//}

/*


namespace internal {
	struct AsyncMethod
	{
		Async* self;
		std::function<void()> target;
		std::function<void(void*)> target1;
		void* opaque;
	};
}


	struct AsyncMethodArg
	{
		Async* thread;
		std::function<void(void*)> target;
	};
		//runAsync(arg, false);
		//auto req = reinterpret_cast<Runner::Context*>(arg);
		//req = req.get()->thread;
		//self->_id = uv_thread_self();, bool hasArg
*/
		/*
		if (hasArg) {
			std::unique_ptr<Runner::ContextArg> req(reinterpret_cast<Runner::ContextArg*>(arg));
			self = req.get()->thread;
			//self->_id = uv_thread_self();
			self->_running = true;
			req.get()->target(req.get()->opaque);
		}
		else {
			std::unique_ptr<Runner::Context> req(reinterpret_cast<Runner::Context*>(arg));
			req.get()->self->_running = true;
			req.get()->target1(req.get()->opaque);
			req.get()->self->reset();

		//}
		*/
