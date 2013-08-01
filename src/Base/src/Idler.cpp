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


#include "Sourcey/Idler.h"
#include "Sourcey/Logger.h"


using namespace std;


namespace scy {

	
Idler::Idler(uv::Loop& loop) :
	uv::Base(&loop, new uv_idle_t)
{
	traceL("Idler", this) << "Creating" << endl;

    uv_idle_init(&loop, handle<uv_idle_t>());

	// Idlers do not reference the main loop.
    uv_unref(handle());
}

	
Idler::~Idler() 
{	
	traceL("Idler", this) << "Destroying" << endl;
}


namespace internal {

	UVEmptyStatusCallback(Idler, onIdle, uv_idle_t);

}
	

void Idler::start() 
{		
	assert(!active());
    uv_idle_start(handle<uv_idle_t>(), internal::onIdle);
}


void Idler::stop() 
{
	uv_idle_stop(handle<uv_idle_t>());
}


void Idler::onIdle() 
{
	traceL("Idler", this) << "On idle" << endl;
}


} // namespace scy