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


#include "Sourcey/GC.h"
#include "Sourcey/Util.h"


using namespace std;


namespace scy {


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