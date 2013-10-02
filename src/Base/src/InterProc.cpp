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
	ptr(loop, new uv_async_t)
{
}


SyncContext::SyncContext(uv::Loop* loop, std::function<void()> target) : 
	ptr(loop, new uv_async_t)
{
	start(target);
}


SyncContext::SyncContext(uv::Loop* loop, std::function<void(void*)> target, void* arg) : 
	ptr(loop, new uv_async_t)
{
	start(target, arg);
}

	
SyncContext::~SyncContext()
{
	//assert(ptr.closed()); // must be dispose()d
}


void SyncContext::post()
{
	assert(!ptr.closed());
	uv_async_send(ptr.handle<uv_async_t>());
}


void SyncContext::startAsync()
{
	assert(!ptr.active());	

	ptr.handle()->data = &pContext;
	int r = uv_async_init(ptr.loop(), ptr.handle<uv_async_t>(), [](uv_async_t* req, int) {
		runAsync(*reinterpret_cast<const async::Runner::Context::ptr*>(req->data));
	});

	if (r < 0) ptr.setAndThrowError("Cannot initialize async", r);		
}


void SyncContext::close()
{
	ptr.close();
}


bool SyncContext::closed()
{
	return ptr.closed();
}

	
bool SyncContext::synced() const
{
	return true;
}


//
// Idler
//


Idler::Idler(uv::Loop* loop) : 
	ptr(loop, new uv_async_t)
{
	init();
}


Idler::Idler(uv::Loop* loop, std::function<void()> target) : 
	ptr(loop, new uv_async_t)
{
	init();
	start(target);
}


Idler::Idler(uv::Loop* loop, std::function<void(void*)> target, void* arg) : 
	ptr(loop, new uv_async_t)
{
	init();
	start(target, arg);
}

	
Idler::~Idler()
{
	//assert(ptr.closed()); // must be dispose()d
}


void Idler::init()
{	
	uv_idle_init(ptr.loop(), ptr.handle<uv_idle_t>());
}


void Idler::unref()
{	
	assert(ptr.active()); // should only call once after init()
	uv_unref(ptr.handle()); // don't increment main loop
	assert(!ptr.active());
}


/*
void Idler::cancel()
{
	traceL("Idler") << "cancel: " << pContext << std::endl;	
	//cancel();
	uv_idle_stop(ptr.handle<uv_idle_t>());
}
*/


void Idler::startAsync()
{
	assert(!ptr.closed()); // close() must not have been called
	
	pContext->opaque = ptr.handle<uv_idle_t>();
	ptr.handle()->data = &pContext;
	int r = uv_idle_start(ptr.handle<uv_idle_t>(), [](uv_idle_t* req, int) {
		const auto& ptr = *reinterpret_cast<const async::Runner::Context::ptr*>(req->data);
		runAsync(ptr);
		if (ptr->cancelled() && ptr->opaque)
			uv_idle_stop(reinterpret_cast<uv_idle_t*>(ptr->opaque));
		scy::sleep(1); // required or 100% idle CPU
	});

	if (r < 0) ptr.setAndThrowError("Cannot initialize idler", r);		
}


void Idler::restart()
{
	startAsync();
}


void Idler::close()
{
	cancel();
	ptr.close();
}


bool Idler::closed()
{
	return ptr.closed();
}

	
bool Idler::synced() const
{
	return true;
}


} // namespace scy



	
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

    //int r = uv_idle_start(ptr.handle<uv_idle_t>(), [](uv_idle_t* req, int) {
	//	auto callback = reinterpret_cast<CallbackRef*>(req->data);
	//	callback->func(); // (callback->self);
	//	scy::sleep(1); // required or 100% CPU
	//});