//
// This software is copyright by Sourcey <mail@sourcey.com> and is distributed under a dual license:
// Copyright (C) 2005 Sourcey
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


#ifndef SOURCEY_Stateful_H
#define SOURCEY_Stateful_H


#include "Sourcey/Signal.h"

#include <string>
#include <iostream>


namespace Sourcey {


struct StateT 
{ 
	StateT(unsigned int id = 0) : _id(id) {}
	StateT(const StateT& r) : _id(r._id), _message(r._message) {}
	virtual std::string str(unsigned int id) const { (void)id; return "undefined"; }
	virtual std::string toString() const { return str(_id); }
	virtual unsigned int id() const { return _id; }
	virtual void set(unsigned int id) { _id = id; }
	virtual std::string message() const { return _message; }
	virtual void setMessage(const std::string& message) { _message = message; }
	bool operator == (const StateT& r) { return _id == r._id; }	
	
    friend std::ostream& operator << (std::ostream& os, const StateT& state) 
	{
		os << state.toString();
		return os;
    }

protected:
	unsigned int _id;
	std::string _message;
};


// ---------------------------------------------------------------------
//
template<typename T>
class Stateful
	/// This class implements a simple state machine.
	/// T should be a derived StateT.
	///
	/// This class is not inherently thread safe. 
	/// Synchronization is left to the implementation.
{
public:
	virtual bool stateEquals(unsigned int id) const
	{ 	
		return _state.id() == id;
	}

	virtual bool stateBetween(unsigned int lid, unsigned int rid) const
	{ 	
		return _state.id() >= lid 
			&& _state.id() <= rid;
	}

	virtual T& state() { return _state; }
	virtual T state() const { return _state; }

protected:
	virtual bool canTransitionState(unsigned int id) 
	{
		if (state().id() != id) 
			return true;
		return false;
	}
	
	virtual void onStateChange(T&, const T&) 
	{
	}

	virtual bool setState(unsigned int id, const std::string& message = "") 
	{ 	
		if (canTransitionState(id)) {
			T& state = this->state();
			T oldState = state;
			state.set(id);	
			state.setMessage(message);	
			onStateChange(state, oldState);
			return true;
		}
		return false;
	}

	virtual void setStateMessage(const std::string& message) 
	{ 
		state().setMessage(message);	
	}

protected:
	T _state;
};


// ---------------------------------------------------------------------
//
template<typename T>
class MutexStateful: public Stateful<T>
	/// This class adds thread safety the base
	/// Stateful implementation.
{
public:
	virtual bool stateEquals(unsigned int id) const
	{ 	
		return state().id() == id;
	}

	virtual bool stateBetween(unsigned int lid, unsigned int rid) const
	{ 	
		int id = state().id();
		return id >= lid 
			&& id <= rid;
	}

	virtual T& state() 
	{ 
		Poco::FastMutex::ScopedLock lock(_mutex);
		return Stateful<T>::_state; 
	}

	virtual T state() const 
	{ 
		Poco::FastMutex::ScopedLock lock(_mutex);
		return Stateful<T>::_state; 
	}

protected:
	mutable Poco::FastMutex	_mutex;
};


// ---------------------------------------------------------------------
//
template<typename T>
class StatefulSignal: public MutexStateful<T>
	/// This class adds a StateChange signal
	/// to the base implementation.
{
public:
	Signal2<T&, const T&> StateChange;
		/// Fired when the state changes to signal the
		/// new and old states.

protected:
	virtual bool setState(void* sender, unsigned int id, const std::string& message = "") 
		/// This method is used to send the state signal
		/// after a successful state change.
	{ 
		T oldState = MutexStateful<T>::state();
		if (Stateful<T>::setState(id, message)) {
			StateChange.dispatch(sender, MutexStateful<T>::state(), oldState);
			return true;
		}
		return false;
	}
};


} // namespace Sourcey


#endif // SOURCEY_Stateful_H