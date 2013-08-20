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

/*

#ifndef SOURCEY_AtomicOps_H
#define SOURCEY_AtomicOps_H


#include "Sourcey/Base.h"
#include "Sourcey/Mutex.h"


namespace scy {
namespace atomic {
	

// TODO: CAS routines and better compiler checking
// Will be depreciated when we can support std::atomic
	

//
// Apple
//
	
#if defined(_MACOSX) || defined(__APPLE__) || defined(__DARWIN__)
namespace { #include <libkern/OSAtomic.h> }

typedef int32_t type_t;

inline type_t increment(volatile type_t* p) { return OSAtomicIncrement32(volatile type_t* p); }
inline type_t decrement(volatile type_t* p) { return OSAtomicDecrement32(volatile type_t* p); }
inline type_t compareAndSwap(volatile type_t* p, type_t newVal, type_t oldVal) { return OSAtomicCompareAndSwap32(oldVal, newVal, p); }

//
// Windows
//

#elif defined(__WINDOWS__) || defined(WIN32)
#ifndef _WINDOWS_
#define WIN32_LEAN_AND_MEAN
namespace { #include <windows.h> }

#endif

typedef volatile LONG type_t;

inline type_t increment(volatile type_t* p) { return InterlockedIncrement(p); }
inline type_t decrement(volatile type_t* p) { return InterlockedDecrement(p); }
inline type_t compareAndSwap(volatile type_t* p, type_t newVal, type_t oldVal) { return InterlockedCompareExchange(p, newVal, oldVal); }


//
// Linux (gcc)
//

#elif (((__GNUC__ == 4) && (__GNUC_MINOR__ >= 2) || __GNUC__ > 4) && (defined(__x86_64__) || defined(__i386__)) ||
		(__GNUC__ == 4) && (__GNUC_MINOR__ >= 3) || __GNUC__ > 4))

typedef int type_t;

inline type_t increment(volatile type_t* p) { return __sync_add_and_fetch(p, 1L); }
inline type_t decrement(volatile type_t* p) { return __sync_sub_and_fetch(p, 1L); }
inline type_t compareAndSwap(volatile type_t* p, type_t newVal, type_t oldVal) { return __sync_val_compare_and_swap(p, newVal, oldVal); }


//
// Mutex Fallback
//

#else
namespace { #include "Sourcey/Mutex.h" }
#warning "Intrinsic atomic operations are not supported"

volatile int type_t;

#define ENABLE_MUTEX_FALLBACK 1
#ifdef ENABLE_ATOMIC_FALLBACK
static Mutex staticMutex;
#define ATOMIC_FALLBACK(p, op)  \
	staticMutex.lock();         \
	op(*p);                     \
	staticMutex.unlock();       \
	return p;
#else
#define ATOMIC_FALLBACK(p, op)  \
	return op(*p);
#endif

inline type_t increment(volatile long* p) { ATOMIC_FALLBACK(p, ++) }
inline type_t decrement(volatile long* p) { ATOMIC_FALLBACK(p, --) }
inline type_t compareAndSwap(volatile type_t* p, type_t newVal, type_t oldVal) { 
	Mutex::ScopedLock lock(&staticMutex);
    if (*p == oldVal) {
        *p = newVal;
        return oldVal;
    }
    else return newVal;
}


#endif


} } // namespace scy::atomic


#endif // SOURCEY_AtomicOps_H

*/