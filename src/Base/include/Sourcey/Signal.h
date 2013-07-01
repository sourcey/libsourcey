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


#ifndef SOURCEY_Signal_H
#define SOURCEY_Signal_H


#include "Sourcey/Types.h"
#include "Sourcey/Delegate.h"
#include "Sourcey/Util.h"
#include "Poco/Mutex.h"
#include <vector>
#include <assert.h>


namespace scy {


template <class DelegateT, DelegateDefaultParams>
class SignalBase 
	/// This class implements a thread-safe signal which
	/// broadcasts data to multiple delegate listeners.
{
public:
	typedef std::list<DelegateT*>				  DelegateList;
	typedef typename DelegateList::iterator       Iterator;
	typedef typename DelegateList::const_iterator ConstIterator;

	SignalBase() : 
		_enabled(true), 
		_dirty(false), 
		_refCount(0)
	{
	}	

	virtual ~SignalBase() 
	{ 
		clear();
	}

	void operator += (const DelegateT& delegate) { attach(delegate); }	
	void operator -= (const DelegateT& delegate) { detach(delegate); }	
	void operator -= (const Void klass) { detach(klass); }

	void attach(const DelegateT& delegate) 
		/// Attaches a delegate to the signal. If the delegate is
		/// equal to an already existing one it will simply replace
		/// the existing delegate.
	{
		detach(delegate);
		//Poco::FastMutex::ScopedLock lock(_mutex);
		_delegates.push_back(delegate.clone());
		_delegates.sort(DelegateT::ComparePrioroty); 
		_refCount++;
	}

	bool detach(const DelegateT& delegate) 
		/// Detaches a delegate from the signal.
		/// Returns true if the delegate was detached, false otherwise.
	{
		bool res = false;
		{
			//Poco::FastMutex::ScopedLock lock(_mutex);
			for (Iterator it = _delegates.begin(); it != _delegates.end(); ++it) {
				if (delegate.equals(*it) && !(*it)->cancelled()) {	
					(*it)->cancel();
					_dirty = true;
					_refCount--;
					res = true;
				}
			}
			res = false;
		}
		cleanup();
		return res;
	}

	void detach(const Void klass) 
		/// Detaches all delegates associated with the given instance.
	{
		{
			//Poco::FastMutex::ScopedLock lock(_mutex);
			for (Iterator it = _delegates.begin(); it != _delegates.end(); ++it) {
				if (klass == (*it)->object() && !(*it)->cancelled()) {	
					(*it)->cancel();
					_dirty = true;
					_refCount--;
				}
			}
		}
		cleanup();
	}

	void cleanup() 
		/// Deletes cancelled delegates.
	{
		//Poco::FastMutex::ScopedLock lock(_mutex);
		if (_dirty) {
			Iterator it = _delegates.begin(); 
			while (it != _delegates.end()) {
				DelegateT* delegate = *it;
				if (delegate->cancelled()) {
					delete delegate;
					it = _delegates.erase(it);
				}
				else
					++it;
			}
		}
		_dirty = false;
	}

	void obtain(DelegateList& active) 
		/// Obtains a list of active delegates,
		/// and deletes any redundant delegates.
	{
		//Poco::FastMutex::ScopedLock lock(_mutex);
		Iterator it = _delegates.begin(); 
		while (it != _delegates.end()) {
			DelegateT* delegate = *it;
			if (delegate->cancelled()) {
				assert(_dirty);
				delete delegate;
				it = _delegates.erase(it);
			}
			else {
				if (_enabled) // skip if disabled
					active.push_back(delegate);
				++it;
			}
		}

		_dirty = false;
	}

	void clear() 
	{
		//Poco::FastMutex::ScopedLock lock(_mutex);
		util::ClearList(_delegates);
		_refCount = 0;
	}

	DelegateList delegates() const 
	{
		//Poco::FastMutex::ScopedLock lock(_mutex);
		return _delegates;
	}

	void enable(bool flag = true) 
	{
		//Poco::FastMutex::ScopedLock lock(_mutex);
		_enabled = flag;
	}

	bool enabled() 
	{
		//Poco::FastMutex::ScopedLock lock(_mutex);
		return _enabled;
	}
	
	int refCount() const 
	{
		//Poco::FastMutex::ScopedLock lock(_mutex);
		return _refCount;
	}

	void emit(Void sender) 
	{
		Void empty = 0;
		emit(sender, (P)empty, (P2)empty, (P3)empty, (P4)empty);
	}

	void emit(Void sender, P arg) 
	{
		Void empty = 0;
		emit(sender, arg, (P2)empty, (P3)empty, (P4)empty);
	}

	void emit(Void sender, P arg, P2 arg2) 
	{
		Void empty = 0;
		emit(sender, arg, arg2, (P3)empty, (P4)empty);
	}	

	void emit(Void sender, P arg, P2 arg2, P3 arg3) 
	{
		Void empty = 0;
		emit(sender, arg, arg2, arg3, (P4)empty);
	}

	void emit(Void sender, P arg, P2 arg2, P3 arg3, P4 arg4) 
	{
		DelegateList toNotify;
		obtain(toNotify);
		try {
			for (Iterator it = toNotify.begin(); it != toNotify.end(); ++it) {
				if ((*it)->accepts(sender, arg, arg2, arg3, arg4))
					(*it)->emit(sender, arg, arg2, arg3, arg4); 
			}
		}
		catch (StopPropagation&) {
		}
	}
		
protected:
	DelegateList _delegates;
	bool _enabled;	
	bool _dirty;
	int _refCount;

	//mutable Poco::FastMutex	_mutex;
};


// -------------------------------------------------------------------
//
struct StopPropagation: public std::exception
	// An exception used to break the current callback scope.
{
	virtual ~StopPropagation() throw() {};
};


// -------------------------------------------------------------------
//
class NullSignal: public SignalBase< 
	DelegateBase<> 
> {};


template <typename P>
class Signal: public SignalBase<DelegateBase<P>, P> {};


template <typename P, typename P2>
class Signal2: public SignalBase<DelegateBase<P, P2>, P, P2> {};


template <typename P, typename P2, typename P3>
class Signal3: public SignalBase<DelegateBase<P, P2, P3>, P, P2, P3> {};


template <typename P, typename P2, typename P3, typename P4>
class Signal4: public SignalBase<DelegateBase<P, P2, P3, P4>, P, P2, P3, P4> {};


} // namespace scy


#endif // SOURCEY_Signal_H