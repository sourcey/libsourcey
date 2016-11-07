///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup base
/// @{


#include "scy/memory.h"
#include "scy/util.h"


using std::endl;


namespace scy {


static Singleton<GarbageCollector> singleton;
static const int GCTimerDelay = 2400;


GarbageCollector::GarbageCollector() :
    _handle(uv::defaultLoop(), new uv_timer_t),
    _finalize(false),
    _tid(0)
{
    TraceL << "Create" << std::endl;

    _handle.ptr()->data = this;
    uv_timer_init(_handle.loop(), _handle.ptr<uv_timer_t>());
    uv_timer_start(_handle.ptr<uv_timer_t>(), GarbageCollector::onTimer, GCTimerDelay, GCTimerDelay);
    uv_unref(_handle.ptr());
}


GarbageCollector::~GarbageCollector()
{
    TraceL << "Destroy: "
        << "ready=" << _ready.size() << ", "
        << "pending=" << _pending.size() << ", "
        << "finalize=" << _finalize
        << std::endl;

    // NOTE: Calling finalize() here is dangerous since more handles may be
    // queued to the garbage collector during finalization causing a deadlock.
    // Because of this finalize() should always be called before the application
    // exists and the garbage collector singleton is destroyed.
    if (!_finalize)
        finalize();

    // The queue should be empty on shutdown if finalized correctly.
    assert(_pending.empty());
    assert(_ready.empty());
}


void GarbageCollector::finalize()
{
    TraceL << "Finalize" << std::endl;

    // Ensure the loop is not running and that the
    // calling thread is the main thread.
    _handle.assertThread();
    // assert(_handle.loop()->active_handles <= 1);
    assert(!_handle.closed());
    assert(!_finalize);
    _finalize = true;

    // Make sure uv_stop doesn't prevent cleanup.
    _handle.loop()->stop_flag = 0;

    // Run the loop until managed pointers have been deleted,
    // and the internal timer has also been deleted.
    uv_timer_set_repeat(_handle.ptr<uv_timer_t>(), 1);
    uv_ref(_handle.ptr());
    uv_run(_handle.loop(), UV_RUN_DEFAULT);

    assert(_pending.empty());
    assert(_ready.empty());

    TraceL << "Finalize: OK" << std::endl;
}


void onPrintHandle(uv_handle_t* handle, void* /* arg */)
{
    DebugL << "Active handle: " << handle << ": " << handle->type << std::endl;
}


void GarbageCollector::runAsync()
{
    std::vector<ScopedPointer*> deletable;
    {
        Mutex::ScopedLock lock(_mutex);
        if (!_tid) { _tid = uv_thread_self(); }
        if (!_ready.empty() || !_pending.empty()) {
            TraceL << "Deleting: "
                << "ready=" << _ready.size() << ", "
                << "pending=" << _pending.size()
                << std::endl;

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
        Mutex::ScopedLock lock(_mutex);
        if (_ready.empty() && _pending.empty()) {
            // Stop and close the timer handle.
            // This should cause the loop to return after
            // uv_close has been called on the timer handle.
            uv_timer_stop(_handle.ptr<uv_timer_t>());
            //_handle.close();

            TraceL << "Finalization complete: " << _handle.loop()->active_handles << std::endl;

#ifdef _DEBUG
            // Print active handles, there should only be 1 left
            uv_walk(_handle.loop(), onPrintHandle, nullptr);
            //assert(_handle.loop()->active_handles <= 1);
#endif
        }
    }
}


void GarbageCollector::onTimer(uv_timer_t* handle)
{
    static_cast<GarbageCollector*>(handle->data)->runAsync();
}


uv_thread_t GarbageCollector::tid()
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


} // namespace scy

/// @\}
