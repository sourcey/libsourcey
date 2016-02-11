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


#include "scy/mutex.h"
#include <cstdint>


namespace scy {


Mutex::Mutex()
{
    if (uv_mutex_init(&_mx) != 0)
        throw std::runtime_error("Mutex failed to initialize");
}


Mutex::~Mutex()
{
    uv_mutex_destroy(&_mx);
}


void Mutex::unlock()
{
    uv_mutex_unlock(&_mx);
}


void Mutex::lock()
{
    uv_mutex_lock(&_mx);
}


bool Mutex::tryLock()
{
    return uv_mutex_trylock(&_mx) == 0;
}


} // namespace scy
