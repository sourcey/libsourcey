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


#ifndef SOURCEY_Stateful_H
#define SOURCEY_Stateful_H


#include "Sourcey/Signal.h"
#include "Sourcey/State.h"

#include <string>
#include <iostream>


namespace scy {


// ---------------------------------------------------------------------
template<typename T>
class Stateful
	// This class implements a simple state machine.
	// T should be a derived State.
	//
	// Use MutexStateful for thread safe implementation. 
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
	virtual bool canChangeState(unsigned int id) 
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
		if (canChangeState(id)) {
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
template<typename T>
class MutexStateful: public Stateful<T>
	// This class adds thread safety the base
	// Stateful implementation.
{
public:
	virtual bool stateEquals(unsigned int id) const
	{ 	
		return state().id() == id;
	}

	virtual bool stateBetween(unsigned int lid, unsigned int rid) const
	{ 	
		State::ID id = state().id();
		return id >= lid 
			&& id <= rid;
	}

	virtual T& state() 
	{ 
		ScopedLock lock(_mutex);
		return Stateful<T>::_state; 
	}

	virtual T state() const 
	{ 
		ScopedLock lock(_mutex);
		return Stateful<T>::_state; 
	}

protected:
	mutable Mutex	_mutex;
};


// ---------------------------------------------------------------------
template<typename T>
class StatefulSignal: public MutexStateful<T>
	// This class adds a StateChange signal
	// to the thread safe implementation.
{
public:
	Signal2<T&, const T&> StateChange;
		// Fired when the state changes to signal the
		// new and old states.

protected:
	virtual bool setState(void* sender, unsigned int id, const std::string& message = "") 
		// This method is used to send the state signal
		// after a successful state change.
	{ 
		T oldState = MutexStateful<T>::state();
		if (Stateful<T>::setState(id, message)) {
			StateChange.emit(sender, MutexStateful<T>::state(), oldState);
			return true;
		}
		return false;
	}
};


} // namespace scy


#endif // SOURCEY_Stateful_H