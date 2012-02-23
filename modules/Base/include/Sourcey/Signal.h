//
// This software is copyright by Sourcey <mail@sourcey.com> and is distributed under a dual license:
// Copyright (C) 2002 Sourcey
//
// Non-Commercial Use:
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// Commercial Use:
// Please contact mail@sourcey.com
//


#ifndef SOURCEY_Signal_H
#define SOURCEY_Signal_H


#include "Sourcey/Base.h"
#include "Sourcey/Delegate.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Util.h"
#include "Poco/Mutex.h"
#include <vector>
#include <assert.h>


namespace Sourcey {


template <class DelegateT, DelegateDefaultParams>
class SignalBase 
	/// This class implements a thread-safe signal which broadcasts
	/// data to multiple delegates.
{
public:
	typedef std::list<DelegateT*>				  DelegateList;
	typedef typename DelegateList::iterator       Iterator;
	typedef typename DelegateList::const_iterator ConstIterator;

	SignalBase() : _hasCancelled(false), _refCount(0)
	{
	}	

	virtual ~SignalBase() 
	{ 
		clear();
	}

	virtual void attach(const DelegateT& delegate) 
		/// Attaches a delegate to the signal. If the delegate is
		/// equal to an already existing one it will simply replace
		/// the existing delegate.
	{
		detach(delegate);
		Poco::FastMutex::ScopedLock lock(_mutex);
		_delegates.push_back(delegate.clone());
		_delegates.sort(DelegateT::ComparePrioroty); 
		_refCount++;
	}

	virtual void detach(const DelegateT& delegate) 
		/// Detaches a delegate from the signal.
		/// If the delegate is not found, the detach will be ignored.
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		for (Iterator it = _delegates.begin(); it != _delegates.end(); ++it) {
			if (delegate.equals(*it) && !(*it)->cancelled()) {	
				(*it)->cancel();
				_hasCancelled = true;
				_refCount--;
				return;
			}
		}
	}

	virtual void detachAll(const Void klass) 
		/// Detaches all delegates associated with the given instance.
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		for (Iterator it = _delegates.begin(); it != _delegates.end(); ++it) {
			if (klass == (*it)->object() && !(*it)->cancelled()) {	
				(*it)->cancel();
				_hasCancelled = true;
				_refCount--;
			}
		}
	}

	virtual void clear() 
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		Util::ClearList(_delegates);
		_refCount = 0;
	}

	virtual DelegateList delegates() const 
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		return _delegates;
	}
	
	virtual void obtain(DelegateList& active) 
		/// Obtains a list of active delegates, and deletes
		/// cancelled delegates.
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		Iterator it = _delegates.begin(); 
		while (it != _delegates.end()) {
			DelegateT* delegate = *it;
			if (delegate->cancelled()) {
				assert(_hasCancelled);
				delete delegate;
				it = _delegates.erase(it);
			}
			else {
				active.push_back(delegate);
				++it;
			}
		}

		_hasCancelled = false;
	}

	virtual int refCount() const 
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		return _refCount;
	}

	virtual void dispatch(Void sender) 
	{
		Void empty = 0;
		dispatch(sender, (P)empty, (P2)empty, (P3)empty, (P4)empty);
	}

	virtual void dispatch(Void sender, P arg) 
	{
		Void empty = 0;
		dispatch(sender, arg, (P2)empty, (P3)empty, (P4)empty);
	}

	virtual void dispatch(Void sender, P arg, P2 arg2) 
	{
		Void empty = 0;
		dispatch(sender, arg, arg2, (P3)empty, (P4)empty);
	}	

	virtual void dispatch(Void sender, P arg, P2 arg2, P3 arg3) 
	{
		Void empty = 0;
		dispatch(sender, arg, arg2, arg3, (P4)empty);
	}

	virtual void dispatch(Void sender, P arg, P2 arg2, P3 arg3, P4 arg4) 
	{
		DelegateList toNotify;
		obtain(toNotify);
		try {
			for (Iterator it = toNotify.begin(); it != toNotify.end(); ++it) {
				if ((*it)->accepts(sender, arg, arg2, arg3, arg4))
					(*it)->dispatch(sender, arg, arg2, arg3, arg4); 
			}
		}
		catch (StopPropagation&) {
		}
	}

	virtual void operator += (const DelegateT& delegate) { attach(delegate); }	
	virtual void operator -= (const DelegateT& delegate) { detach(delegate); }	
	virtual void operator -= (const Void klass) { detachAll(klass); }
		
protected:
	DelegateList _delegates;
	bool _hasCancelled;	
	int _refCount;

	mutable Poco::FastMutex	_mutex;
};


// -------------------------------------------------------------------
//
class NullSignal: public SignalBase<DelegateBase<>> {};


template <typename P>
class Signal: public SignalBase<DelegateBase<P>, P> {};


template <typename P, typename P2>
class Signal2: public SignalBase<DelegateBase<P, P2>, P, P2> {};


template <typename P, typename P2, typename P3>
class Signal3: public SignalBase<DelegateBase<P, P2, P3>, P, P2, P3> {};


template <typename P, typename P2, typename P3, typename P4>
class Signal4: public SignalBase<DelegateBase<P, P2, P3, P4>, P, P2, P3, P4> {};


} // namespace Sourcey


#endif // SOURCEY_Signal_H