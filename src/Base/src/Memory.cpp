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


#include "Sourcey/Memory.h"
#include "Sourcey/Util.h"


using namespace std;


namespace scy {


static Singleton<GarbageCollector> singleton;
static const int GCTimerDelay = 2400;
	

GarbageCollector::GarbageCollector() : 
	_handle(uv::defaultLoop(), new uv_timer_t), 
	_finalize(false), 
	_tid(0)
{
	traceL("GarbageCollector", this) << "Create" << std::endl;

	_handle.ptr()->data = this;
	uv_timer_init(_handle.loop(), _handle.ptr<uv_timer_t>());		
	uv_timer_start(_handle.ptr<uv_timer_t>(), GarbageCollector::onTimer, GCTimerDelay, GCTimerDelay);
	uv_unref(_handle.ptr());
}

	
GarbageCollector::~GarbageCollector()
{
	traceL("GarbageCollector", this) << "Destroy: "
			<< "\n\tReady: " << _ready.size() 
			<< "\n\tPending: " << _pending.size()
			<< "\n\tFinalize: " << _finalize
			<< std::endl;

	// The queue should be empty on shutdown if finalized correctly.
	assert(_ready.empty() && _pending.empty());
}


void GarbageCollector::finalize()
{
	traceL("GarbageCollector", this) << "Finalize" << std::endl;	
	
	// Ensure the loop is not running and that the 
	// calling thread is the main thread.
	_handle.assertTID();
	//assert(_handle.loop()->active_handles <= 1); 
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

	traceL("GarbageCollector", this) << "Finalize: OK" << std::endl;
}


void GarbageCollector::runAsync()
{
	std::vector<DeferredDeleter*> deletable;
	{
		Mutex::ScopedLock lock(_mutex);

		if (!_ready.empty() || !_pending.empty()) {
			traceL("GarbageCollector", this) << "Deleting: "
				<< "\n\tReady: " << _ready.size() 
				<< "\n\tPending: " << _pending.size()
				<< "\n\tFinalize: " << _finalize
				<< std::endl;

			// Delete waiting pointers
			//util::clearVector(_ready);
			deletable = _ready;
			_ready.clear();

			// Swap pending pointers to the ready queue
			_ready.swap(_pending);
		}

		if (_finalize && _ready.empty() && _pending.empty()) {
			// Stop and close the timer handle.
			// This should cause the loop to return after 
			// uv_close has been called on the timer handle.
			uv_timer_stop(_handle.ptr<uv_timer_t>());
			_handle.close();

			traceL("GarbageCollector") << "Finalization complete" << std::endl;
		}

		if (!_tid) { _tid = uv_thread_self(); }	
	}	
	
	// Delete pointers
	util::clearVector(deletable);
}


void GarbageCollector::onTimer(uv_timer_t* handle, int)
{
	static_cast<GarbageCollector*>(handle->data)->runAsync();
}
	

unsigned long GarbageCollector::tid()
{
	return _tid;
}
	

void GarbageCollector::shutdown()
{
	singleton.destroy();
}


GarbageCollector& GarbageCollector::instance() 
{
	return *singleton.get();
}


} // namespace scy::uv