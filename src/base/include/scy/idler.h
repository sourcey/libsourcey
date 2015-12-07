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


#ifndef SCY_Idler_H
#define SCY_Idler_H


#include "scy/uv/uvpp.h"
#include "scy/async.h"

#include <functional>


namespace scy {


class Idler: public async::Runner
{
public:
    Idler(uv::Loop* loop = uv::defaultLoop());
    Idler(uv::Loop* loop, std::function<void()> target);
    Idler(uv::Loop* loop, std::function<void(void*)> target, void* arg);
        // Create the idler context the given event loop and method.
    
    virtual ~Idler();
    
    uv::Handle& handle();

protected:    
    virtual void init();
    virtual void startAsync();
    virtual bool async() const;
    
    uv::Handle _handle;
};


} // namespace scy


#endif // SCY_Idler_H