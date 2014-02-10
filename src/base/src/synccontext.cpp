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


#include "scy/synccontext.h"


namespace scy {


SyncContext::SyncContext(uv::Loop* loop) : 
	_handle(loop, new uv_async_t)
{
}


SyncContext::SyncContext(uv::Loop* loop, std::function<void()> target) : 
	_handle(loop, new uv_async_t)
{
	start(target);
}


SyncContext::SyncContext(uv::Loop* loop, std::function<void(void*)> target, void* arg) : 
	_handle(loop, new uv_async_t)
{
	start(target, arg);
}

	
SyncContext::~SyncContext()
{
	//assert(_handle.closed()); // must be dispose()d
	close();
}


void SyncContext::post()
{
	assert(!_handle.closed());
	uv_async_send(_handle.ptr<uv_async_t>());
}


void SyncContext::startAsync()
{
	assert(!_handle.active());	
	
	_handle.ptr()->data = new async::Runner::Context::ptr(pContext);
	int r = uv_async_init(_handle.loop(), _handle.ptr<uv_async_t>(), [](uv_async_t* req, int) {
		assert(req->data != nullptr); // catch late callbacks, may need to
		                              // make uv handle a context member
		auto ctx = reinterpret_cast<async::Runner::Context::ptr*>(req->data);
		if (ctx->get()->cancelled()) {
			delete ctx; // delete the context and free memory
			req->data = nullptr;
			return;
		}

		runAsync(ctx->get());		
	});

	if (r < 0) _handle.setAndThrowError("Cannot initialize async", r);		
}


void SyncContext::cancel()
{
	async::Runner::cancel();
}


void SyncContext::close()
{
	if (closed())
		return;
	cancel();
	post(); // post to wake up event loop
	_handle.close();
}


bool SyncContext::closed()
{
	return _handle.closed();
}

	
bool SyncContext::async() const
{
	return false;
}


uv::Handle& SyncContext::handle()
{
	return _handle;
}


} // namespace scy