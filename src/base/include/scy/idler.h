///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup base
/// @{


#ifndef SCY_Idler_H
#define SCY_Idler_H


#include "scy/async.h"
#include "scy/uv/uvpp.h"

#include <functional>


namespace scy {


class Idler : public async::Runner
{
public:
    /// Create the idler context the given event loop and method.
    Idler(uv::Loop* loop = uv::defaultLoop());
    Idler(uv::Loop* loop, std::function<void()> target);
    Idler(uv::Loop* loop, std::function<void(void*)> target, void* arg);

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


/// @\}
