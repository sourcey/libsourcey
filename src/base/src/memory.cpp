///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup base
/// @{


#include "scy/memory.h"
#include "scy/util.h"


using std::endl;


namespace scy {


static Singleton<GarbageCollector> singleton;
static const int GCTimerDelay = 100;


GarbageCollector::GarbageCollector()
    : _tid(std::this_thread::get_id())
{
    LTrace("Create")
}


GarbageCollector::~GarbageCollector()
{
    //if (!_finalize)
    //    finalize();
    finalize();

    util::clearVector(_cleaners);
}



GarbageCollector::Cleaner* GarbageCollector::getCleaner(uv::Loop* loop)
{
    std::lock_guard<std::mutex> guard(_mutex);

    for (auto cleaner : _cleaners) {
        if (loop == cleaner->_loop)
            return cleaner;
    }

    auto clnr = new Cleaner(loop);
    _cleaners.push_back(clnr);
    return clnr;
}


void GarbageCollector::finalize()
{
    // LTrace("Finalize")

    std::lock_guard<std::mutex> guard(_mutex);

    assert(_tid == std::this_thread::get_id());
    for (auto cleaner : _cleaners) {
        if (!cleaner->_finalize)
            cleaner->finalize();
    }

    //// Ensure the loop is not running and that the
    //// calling thread is the main thread.
    //_handle.assertThread();
    //// assert(_handle.loop()->active_handles <= 1);
    //assert(!_handle.closed());
    //assert(!_finalize);
    //_finalize = true;

    //// Run the loop until managed pointers have been deleted,
    //// and the internal timer has also been deleted.
    //uv_timer_set_repeat(_handle.ptr<uv_timer_t>(), 1);
    //uv_ref(_handle.ptr());
    //uv_run(_handle.loop(), UV_RUN_DEFAULT);

    //LTrace("Finalize: OK")
}


std::thread::id GarbageCollector::tid()
{
    return _tid;
}


void GarbageCollector::destroy()
{
    singleton.destroy();
}


GarbageCollector& GarbageCollector::instance()
{
    return *singleton.get();
}


//
// Garbage Collector Cleaner
//


GarbageCollector::Cleaner::Cleaner(uv::Loop* loop)
    : _timer(GCTimerDelay, GCTimerDelay, loop,
        std::bind(&GarbageCollector::Cleaner::work, this))
    , _loop(loop)
    , _finalize(false)
{
    LTrace("Create: ", loop)
}


GarbageCollector::Cleaner::~Cleaner()
{
    if (!_finalize)
        finalize();

    // The queue should be empty on shutdown if finalized correctly.
    assert(_pending.empty());
    assert(_ready.empty());

    //util::clearVector(_ready);
    //util::clearVector(_pending);
}


void GarbageCollector::Cleaner::finalize()
{
    assert(!_finalize && "finalizing twice");
    _finalize = true;

    // Ensure previous calls to uv_stop don't prevent cleanup.
    //_loop->stop_flag = 0;

    assert(_timer.handle().initialized());
    _timer.setInterval(1);
    _timer.handle().ref(); // ref until complete
    uv_run(_loop, UV_RUN_DEFAULT);

    assert(_pending.empty());
    assert(_ready.empty());
}


void GarbageCollector::Cleaner::work()
{
    std::vector<ScopedPointer*> deletable;
    {
        std::lock_guard<std::mutex> guard(_mutex);
        if (!_ready.empty() || !_pending.empty()) {
            // STrace << "Deleting: "
            //     << "ready=" << _ready.size() << ", "
            //     << "pending=" << _pending.size() << std::endl;

            // Delete waiting pointers
            deletable = _ready;
            _ready.clear();

            // Swap pending pointers to the ready queue
            _ready.swap(_pending);
        }
    }

    // Delete pointers
    util::clearVector(deletable);

    // Handle finalization
    if (_finalize) {
        std::lock_guard<std::mutex> guard(_mutex);
        if (_ready.empty() && _pending.empty()) {
            // Stop the timer handle allowing the finalize() method to return.
            _timer.stop();

            // LTrace("Finalization complete: ", _loop->active_handles)
#ifdef _DEBUG
            // Print active handles, there should only be 1 left (our timer)
            uv_walk(_loop, [](uv_handle_t* handle, void* /* arg */) {
                // LDebug("Active handle: ", handle, ": ", handle->type)
            }, nullptr);
            // assert(_handle.loop()->active_handles <= 1);
#endif
        }
    }
}


} // namespace scy


/// @\}
