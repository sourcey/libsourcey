///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup base
/// @{

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

/// @\}
