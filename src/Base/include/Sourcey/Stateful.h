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

#include <string>
#include <iostream>


namespace scy {
	

class State 
{ 
public:
	typedef unsigned int ID;
	
	State(ID id = 0);
	virtual ~State() {};

	virtual ID id() const;
	virtual void set(ID id);
	virtual std::string message() const;
	virtual void setMessage(const std::string& message);
	
	virtual std::string str(ID id) const;
	virtual std::string toString() const;

	virtual bool between(ID lid, ID rid) const
	{ 	
		ID id = this->id();
		return id >= lid 
			&& id <= rid;
	}
		
	virtual bool equals(ID id) const
	{ 	
		return this->id() == id;
	}

	bool operator == (const State& r) 
	{ 
		return id() == r.id();
	}	
	
    friend std::ostream& operator << (std::ostream& os, const State& state) 
	{
		os << state.toString();
		return os;
    }

protected:
	ID _id;
	std::string _message;
};


//
// Mutex State
//


/// TODO: Use atomics
class MutexState: public State
{ 
public:
	MutexState(ID id = 0);
	MutexState(const MutexState& r) : State(r) {}
	virtual ~MutexState() {};

	virtual ID id() const { Mutex::ScopedLock lock(_mutex); return _id;	}
	virtual void set(ID id) { Mutex::ScopedLock lock(_mutex); _id = id; }
	virtual std::string message() const { Mutex::ScopedLock lock(_mutex); return _message; }
	virtual void setMessage(const std::string& message) { Mutex::ScopedLock lock(_mutex); _message = message; }

protected:
	mutable Mutex	_mutex;
};


//
// State Signal
//


class StateSignal: public MutexState
{ 
public:
	StateSignal(ID id = 0);
	StateSignal(const StateSignal& r);
	virtual ~StateSignal() {};
		
	virtual bool change(ID id);
	virtual bool canChange(ID id);	
	virtual void onChange(ID id, ID prev);
	
	/// Fired when the state changes to signal 
	/// the new and previous states.
	Signal2<const ID&, const ID&> Change;

protected:	
	virtual void set(ID id);
};


//
// Stateful Interface
//


/// This class implements a simple state machine.
/// T should be a derived State type.
template<typename T>
class Stateful
{
public:	
	Stateful()
	{ 	
	}
	
	virtual ~Stateful()
	{ 	
	}
	
	/// Fires when the state changes.
	Signal2<T&, const T&> StateChange;
	
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
	virtual const T state() const { return _state; }

protected:
	/// Override to handle pre state change logic.
	/// Return false to prevent state change.
	virtual bool canChangeState(unsigned int id) 
	{
		if (_state.id() != id) 
			return true;
		return false;
	}
	
	/// Override to handle post state change logic.
	virtual void onStateChange(T& /* state */, const T& /* oldState */) 
	{
	}
	
	/// Sets the state and sends the state signal if
	/// the state change was successful.
	virtual bool setState(void* sender, unsigned int id, const std::string& message = "") 
	{ 
		if (canChangeState(id)) {
			T oldState = _state;
			_state.set(id);	
			_state.setMessage(message);	
			onStateChange(_state, oldState);
			StateChange.emit(sender, _state, oldState);
			return true;
		}
		return false;
	}

	virtual void setStateMessage(const std::string& message) 
	{ 
		_state.setMessage(message);	
	}

protected:
	T _state;
};


} // namespace scy


#endif // SOURCEY_Stateful_H



	
	/*
			//T& state = this->state();
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
	*/
	//virtual void* self() = 0;
	//{ 
	//	return this;
	//}

		/*
		if (Stateful<T>::setState(id, message)) {
			StateChange.emit(sender, MutexStateful<T>::state(), oldState);
			return true;
		}
		return false;

		T oldState = MutexStateful<T>::state();
		if (Stateful<T>::setState(id, message)) {		
			StateChange.emit(sender, MutexStateful<T>::state(), oldState);
			return true;
		}
		return false;
		*/


/*
//
// Thread-Safe Stateful Interface
//


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
		Mutex::ScopedLock lock(_mutex);
		return Stateful<T>::_state; 
	}

	virtual T state() const 
	{ 
		Mutex::ScopedLock lock(_mutex);
		return Stateful<T>::_state; 
	}

protected:
	mutable Mutex	_mutex;
};



//
// Signaling Stateful Interface
//


template<typename T>
class Stateful: public MutexStateful<T>
	// This class adds a StateChange signal
	// to the thread safe implementation.
{
public:

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
*/