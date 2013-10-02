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


#include "Sourcey/Interface.h"
#include "Sourcey/Logger.h"
#include <memory>


namespace scy {

//
// Polymorphic
//


LogStream& basic::Polymorphic::log(const char* level) const 
{ 
	return printL(level, className(), this);
}

} // namespace scy



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
