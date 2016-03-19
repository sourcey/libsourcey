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


#include "scy/idler.h"
#include "scy/logger.h"


using std::endl;


namespace scy {


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
    int r = uv_idle_start(_handle.ptr<uv_idle_t>(), [](uv_idle_t* req) {
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
