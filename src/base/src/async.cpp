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


#include "scy/async.h"
#include "scy/logger.h"
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


void Runner::runAsync(Context* c)
{
    // std::cout << "Runner::runAsync" << std::endl;
    c->running = true;
    try {
        if (!c->cancelled()) {
            if (!c->tid)
                c->tid = uv_thread_self();
            if (c->target) {
                //assert((!c->cancelled()));
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
    }
    catch (std::exception& exc) {
        ErrorL << "Runner error: " << exc.what() << std::endl;
#ifdef _DEBUG
        throw exc;
#endif
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


void Runner::setRepeating(bool flag)
{
    assert(!pContext->started);
    pContext->repeating = flag;
}


bool Runner::running() const
{
    return pContext->running;
}


bool Runner::started() const
{
    return pContext->started;
}


bool Runner::repeating() const
{
    return pContext->repeating;
}


void Runner::cancel()
{
    pContext->cancel();
}


bool Runner::cancelled() const
{
    return pContext->cancelled();
}


uv_thread_t Runner::tid() const
{
    return pContext->tid;
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
