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

/*
static Singleton<GarbageCollector> singleton;
	

GarbageCollector::GarbageCollector() : 
	uv::Handle(&uv::defaultLoop(), new uv_timer_t)
{
	traceL("GarbageCollector", this) << "Create" << std::endl;

	uv_timer_init(loop(), handle<uv_timer_t>());		
	uv_timer_start(handle<uv_timer_t>(), GarbageCollector::onTimer, 2400, 2400);
	uv_unref(handle());
}

	
GarbageCollector::~GarbageCollector()
{
	close();
}


void GarbageCollector::finalize()
{
	Mutex::ScopedLock lock(_mutex);
	util::clearVector(_ready);
#ifdef _DEBUG
	// Pending items will not be freed in release 
	// mode since it may result in a crash
	//util::clearVector(_pending);
#endif
}

	
void GarbageCollector::close()
{
	finalize();
	uv::Handle::close();
}

	
void GarbageCollector::shutdown()
{
	traceL("GarbageCollector") << "shutdown" << std::endl;

	singleton.get()->close();
	singleton.destroy();
}


void GarbageCollector::onTimer()
{
	Mutex::ScopedLock lock(_mutex);

	if (_ready.empty() && 
		_pending.empty())
		return;

	traceL("GarbageCollector", this) << "Delete queue: "
		<< "\n\tReady: " << _ready.size() 
		<< "\n\tPending: " << _pending.size()
		<< std::endl;

	// Delete waiting pointers
	util::clearVector(_ready);

	// Swap pending pointers to the ready queue
	_ready.swap(_pending);
	assert(_pending.empty());
}


GarbageCollector& GarbageCollector::instance() 
{
	return *singleton.get();
}
*/


static Singleton<GarbageCollector> singleton;
	

GarbageCollector::GarbageCollector() : 
	uv::Handle(&uv::defaultLoop(), new uv_timer_t)
{
	traceL("GarbageCollector", this) << "Create" << std::endl;

	uv_timer_init(loop(), handle<uv_timer_t>());		
	uv_timer_start(handle<uv_timer_t>(), GarbageCollector::onTimer, 2400, 2400);
	uv_unref(handle());
}

	
GarbageCollector::~GarbageCollector()
{
	close();
}


void GarbageCollector::finalize()
{
	Mutex::ScopedLock lock(_mutex);
	util::clearVector(_ready);
#ifdef _DEBUG
	// Pending items will not be freed in release 
	// mode since it may result in a crash
	//util::clearVector(_pending);
#endif
}

	
void GarbageCollector::close()
{
	finalize();
	uv::Handle::close();
}

	
void GarbageCollector::shutdown()
{
	traceL("GarbageCollector") << "shutdown" << std::endl;

	singleton.get()->close();
	singleton.destroy();
}


void GarbageCollector::onTimer()
{
	Mutex::ScopedLock lock(_mutex);

	if (_ready.empty() && 
		_pending.empty())
		return;

	traceL("GarbageCollector", this) << "Delete queue: "
		<< "\n\tReady: " << _ready.size() 
		<< "\n\tPending: " << _pending.size()
		<< std::endl;

	// Delete waiting pointers
	util::clearVector(_ready);

	// Swap pending pointers to the ready queue
	_ready.swap(_pending);
	assert(_pending.empty());
}


GarbageCollector& GarbageCollector::instance() 
{
	return *singleton.get();
}



} // namespace scy::uv



/*
void Timer::close()
{
	traceL("Timer", this) << "Closing: " << _handle << endl;
	Base::close();
}
*/


/*
void Timer::updateState()
{
	_count = 0;
	bool wasActive = _active;
	_active = !!uv_is_active((uv_handle_t*) handle<uv_timer_t>());
		
	traceL("Timer", this) << "Update State: " << _active << endl;
	if (!wasActive && _active) {
		// If our state is changing from inactive to active, we
		// increase the loop's reference count.
		// TODO: reenable
		uv_ref(handle());
		uv_unref(handle());
	} 
	else if (wasActive && !_active) {
		// If our state is changing from active to inactive, we
		// decrease the loop's reference count.
		// TODO: reenable
		uv_unref(handle());
	}
}
*/