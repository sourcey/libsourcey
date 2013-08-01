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


#ifndef SOURCEY_AtomicOps_H
#define SOURCEY_AtomicOps_H


#include "Sourcey/Base.h"
#include "Sourcey/Mutex.h"


namespace scy {
	

// TODO: CAS routines and better compiler checking
// Will be depreciated when we can support std::atomic


#if defined(_MACOSX) || defined(__APPLE__) || defined(__DARWIN__)
	namespace { #include <libkern/OSAtomic.h> }
	inline long atomicIncrement(volatile long* p) { return OSatomicIncrement32((volatile int32_t*)p); }
	inline long atomicDecrement(volatile long* p) { return OSatomicDecrement32((volatile int32_t*)p); }
#elif defined(__WINDOWS__) || defined(WIN32)
	#ifndef _WINDOWS_
	#define WIN32_LEAN_AND_MEAN
	namespace { #include <windows.h> }
	#endif
	#if defined(_MSC_VER) && _MSC_VER <= 1200
	inline long atomicIncrement(volatile long* p) { return InterlockedIncrement((long*)p); }
	inline long atomicDecrement(volatile long* p) { return InterlockedDecrement((long*)p); }
	#else
	inline long atomicIncrement(volatile long* p) { return InterlockedIncrement(p); }
	inline long atomicDecrement(volatile long* p) { return InterlockedDecrement(p); }
	#endif
#elif defined(__ANDROID__) || defined(__linux__)
	inline long atomicIncrement(volatile long* p) { return __sync_add_and_fetch(p, 1L); }
	inline long atomicDecrement(volatile long* p) { return __sync_sub_and_fetch(p, 1L); }
#else
	namespace { #include "Sourcey/Mutex.h" }
	#define MUTEX_FALLBACK(p, op)   \
		static Mutex atomicMutex;  \
		atomicMutex.lock();        \
		op(*p);                     \
		atomicMutex.unlock();      \
		return p;                   \
	inline long atomicIncrement(volatile long* p) { MUTEX_FALLBACK(p, ++) }
	inline long atomicDecrement(volatile long* p) { MUTEX_FALLBACK(p, --) }
#endif


} // namespace scy


#endif // SOURCEY_AtomicOps_H
