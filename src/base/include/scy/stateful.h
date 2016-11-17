///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup base
/// @{


#ifndef SCY_Stateful_H
#define SCY_Stateful_H


#include "scy/signal.h"

#include <iostream>
#include <string>


namespace scy {


class State
{
public:
    typedef unsigned int ID;

    State(ID id= 0, const std::string& message= "");
    virtual ~State(){};

    virtual ID id() const;
    virtual void set(ID id);
    virtual std::string message() const;
    virtual void setMessage(const std::string& message);

    virtual std::string str(ID id) const;
    virtual std::string toString() const;

    virtual bool between(ID lid, ID rid) const
    {
        ID id= this->id();
        return id >= lid && id <= rid;
    }

    virtual bool equals(ID id) const { return this->id() == id; }

    bool operator==(const State& r) { return id() == r.id(); }

    bool operator==(const State::ID& r) { return id() == r; }

    friend std::ostream& operator<<(std::ostream& os, const State& state)
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

// TODO: Use atomics
class MutexState : public State
{
public:
    MutexState(ID id= 0);
    MutexState(const MutexState& r)
        : State(r)
    {
    }
    virtual ~MutexState(){};

    virtual ID id() const
    {
        Mutex::ScopedLock lock(_mutex);
        return _id;
    }
    virtual void set(ID id)
    {
        Mutex::ScopedLock lock(_mutex);
        _id= id;
    }
    virtual std::string message() const
    {
        Mutex::ScopedLock lock(_mutex);
        return _message;
    }
    virtual void setMessage(const std::string& message)
    {
        Mutex::ScopedLock lock(_mutex);
        _message= message;
    }

protected:
    mutable Mutex _mutex;
};


//
// State Signal
//


class StateSignal : public MutexState
{
public:
    StateSignal(ID id= 0);
    StateSignal(const StateSignal& r);
    virtual ~StateSignal(){};

    virtual bool change(ID id);
    virtual bool canChange(ID id);
    virtual void onChange(ID id, ID prev);

    /// Fired when the state changes to signal
    /// the new and previous states.
    // Signal2<const ID&, const ID&> Change;

protected:
    virtual void set(ID id);
};


//
// Stateful Interface
//


/// This class implements a simple state machine.
/// T should be a derived State type.
template <typename T> class Stateful
{
public:
    Stateful() {}

    virtual ~Stateful() {}

    /// Signals when the state changes.
    Signal<void(void*, T&, const T&)> StateChange;

    virtual bool stateEquals(unsigned int id) const
    {
        return _state.id() == id;
    }

    virtual bool stateBetween(unsigned int lid, unsigned int rid) const
    {
        return _state.id() >= lid && _state.id() <= rid;
    }

    virtual T& state() { return _state; }
    virtual const T state() const { return _state; }

protected:
    /// Override to handle pre state change logic.
    /// Return false to prevent state change.
    virtual bool beforeStateChange(const T& state)
    {
        if (_state == state)
            return false;
        return true;
    }

    /// Override to handle post state change logic.
    virtual void onStateChange(T& /*state*/, const T& /*oldState*/) {}

    /// Sets the state and sends the state signal if
    /// the state change was successful.
    virtual bool setState(void* sender, unsigned int id,
                          const std::string& message= "")
    {
        T state;
        state.set(id);
        state.setMessage(message);
        return setState(sender, state);
    }

    /// Sets the state and sends the state signal if
    /// the state change was successful.
    virtual bool setState(void* sender, const T& state)
    {
        if (beforeStateChange(state)) {
            T oldState= _state;
            _state= state;
            //_state.set(id);
            //_state.setMessage(message);
            onStateChange(_state, oldState);
            if (sender)
                StateChange.emit(sender, _state, oldState); // self(),
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


#endif // SCY_Stateful_H


/// @\}
