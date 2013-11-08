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


#include "Sourcey/InterProc.h"


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

	/*
	_handle.ptr()->data = &pContext;
	int r = uv_async_init(_handle.loop(), _handle.ptr<uv_async_t>(), [](uv_async_t* req, int) {
		const auto& ptr = *reinterpret_cast<const async::Runner::Context::ptr*>(req->data);
		errorL("SyncContext") << "runAsync: " << ptr << std::endl;	
		//if (ptr->cancelled())
		//	uv_idle_stop(ptr);
		runAsync(ptr.get());
	});
	*/

	if (r < 0) _handle.setAndThrowError("Cannot initialize async", r);		
}


void SyncContext::cancel()
{
	async::Runner::cancel();
}


void SyncContext::close()
{
	cancel();
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


//
// Idler
//


Idler::Idler(uv::Loop* loop) : 
	_handle(loop, new uv_async_t)
{
	init();
}


Idler::Idler(uv::Loop* loop, std::function<void()> target) : 
	_handle(loop, new uv_async_t)
{
	init();
	start(target);
}


Idler::Idler(uv::Loop* loop, std::function<void(void*)> target, void* arg) : 
	_handle(loop, new uv_async_t)
{
	init();
	start(target, arg);
}

	
Idler::~Idler()
{
	//assert(_handle.closed()); // must be dispose()d
}


void Idler::init()
{	
	pContext->repeating = true;
	pContext->handle = _handle.ptr<uv_idle_t>();
	uv_idle_init(_handle.loop(), _handle.ptr<uv_idle_t>());
	_handle.unref(); // unref by default
}


void Idler::startAsync()
{
	assert(!_handle.closed()); // close() must not have been called
	
	_handle.ptr()->data = new async::Runner::Context::ptr(pContext);
	int r = uv_idle_start(_handle.ptr<uv_idle_t>(), [](uv_idle_t* req, int) {
		auto ctx = reinterpret_cast<async::Runner::Context::ptr*>(req->data);
		runAsync(ctx->get());
		if (ctx->get()->handle && ctx->get()->cancelled()) {
			uv_idle_stop(reinterpret_cast<uv_idle_t*>(ctx->get()->handle));
			delete ctx; // delete the context and free memory
		}
		//scy::sleep(1); // prevent 100% idle CPU
		               // TODO: uv_thread_yield when available
	});

	if (r < 0) _handle.setAndThrowError("Cannot initialize idler", r);		
}


uv::Handle& Idler::handle()
{
	return _handle;
}

	
bool Idler::async() const
{
	return false;
}


} // namespace scy





/*
void Idler::cancel()
{
	traceL("Idler") << "cancel: " << pContext << std::endl;	
	//cancel();
	uv_idle_stop(_handle.ptr<uv_idle_t>());
}


void Idler::restart()
{
	startAsync();
}


void Idler::close()
{
	cancel();
	_handle.close();
}


bool Idler::closed()
{
	return _handle.closed();
}
*/
	
/*

	
void Idler::dispose()
{
	close();

	// Defer deletion since we need to wait 
	// for all libuv callbacks to return.
	deleteLater<Idler>(this);
}

void SyncContext::dispose()
{
	// TODO: uv_close should be called from the main thread,
	// parhaps we need to leave this until destruction and
	// set a separate flag when dispose is called.
	close();

	// Defer deletion since we need to wait 
	// for all libuv callbacks to return.
	deleteLater<SyncContext>(this);
}
*/

    //int r = uv_idle_start(_handle.ptr<uv_idle_t>(), [](uv_idle_t* req, int) {
	//	auto callback = reinterpret_cast<CallbackRef*>(req->data);
	//	callback->func(); // (callback->self);
	//	// scy::sleep(1); // required or 100% CPU
	//});