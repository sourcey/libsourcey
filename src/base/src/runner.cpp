///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup base
/// @{


#include "scy/interface.h"
#include "scy/logger.h"
#include <memory>


namespace scy {


Runner::Runner() :
    _context(std::make_shared<Runner::Context>())
{
}


Runner::~Runner()
{
    // Always cancel so the async context can exit ASAP.
    cancel();
}


void Runner::setRepeating(bool flag)
{
    assert(!_context->running);
    _context->repeating = flag;
}


bool Runner::running() const
{
    return _context->running;
}


bool Runner::repeating() const
{
    return _context->repeating;
}


void Runner::cancel()
{
    _context->cancelled = true;
}


bool Runner::cancelled() const
{
    return _context->cancelled;
}


std::thread::id Runner::tid() const
{
    return _context->tid;
}


bool Runner::waitForExit(int timeout)
{
    assert(Thread::currentID() != tid());
    int times = 0;
    int interval = 10;
    while (!cancelled() || running()) {
        scy::sleep(interval);
        times++;
        if (timeout && ((times * interval) > timeout)) {
            assert(0 && "deadlock; calling inside thread scope?");
            return false;
        }
    }
    return true;
}


} // namespace scy


/// @\}
