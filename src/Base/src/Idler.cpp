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

/*
Idler::Idler(uv::Loop* loop) :
	ptr(loop, new uv_idle_t)
{
	uv_idle_init(loop, ptr.handle<uv_idle_t>());
	uv_unref(ptr.handle()); // don't increment main loop
}
	

void Idler::start(std::function<void()> target)
{		
	assert(!ptr.active());	
	callback.func = target;
	callback.self = this;
	ptr.handle()->data = &callback;
    int r = uv_idle_start(ptr.handle<uv_idle_t>(), [](uv_idle_t* req, int) {
		auto callback = reinterpret_cast<CallbackRef*>(req->data);
		callback->func(); // (callback->self);
		scy::sleep(1); // required or 100% CPU
	});
    if (r < 0)
		ptr.setAndThrowError("Cannot initialize idler", r);
}


void Idler::stop()
{
	uv_idle_stop(ptr.handle<uv_idle_t>());
}
*/





/*
Idler::Idler(uv::Loop* loop)

	
Idler::~Idler() 
{	
}
	

void Idler::start(std::function<void(T)> target)


void Idler::stop() 


void Idler::onIdle() 
{
	traceL("Idler", this) << "On idle" << endl;
}
*/

} // namespace scy




/*
namespace internal {

	UVEmptyStatusCallback(Idler, onIdle, uv_idle_t);

}
*/