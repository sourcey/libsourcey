///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup base
/// @{


#include "scy/thread.h"
#include "scy/logger.h"
#include "scy/platform.h"
#include <assert.h>
#include <memory>


using std::endl;


namespace scy {


const std::thread::id Thread::mainID = std::this_thread::get_id();


Thread::Thread() :
    // Call the default constructor explicitly to
    // underline the fact that it does get called
    _thread()
{
}


Thread::~Thread()
{
    cancel();
    if (_thread.joinable())
        _thread.join();
}


void Thread::start(std::function<void()> target)
{
    start<std::function<void()>>(std::forward<std::function<void()>>(target));
}


void Thread::join()
{
    assert(this->tid() != Thread::currentID());
    // assert(this->cancelled()); // should probably be cancelled
    _thread.join();
    assert(!this->running());
}


std::thread::id Thread::currentID()
{
    return std::this_thread::get_id(); //uv_thread_self();
}


bool Thread::async() const
{
    return true;
}


} // namespace scy


/// @\}
