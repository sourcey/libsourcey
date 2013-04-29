//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is is distributed under a dual license that allows free, 
// open source use and closed source use under a standard commercial
// license.
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


#include "Sourcey/State.h"


using namespace std;


namespace Sourcey {


State::State(State::ID id) : 
	_id(id)
{ 
}


unsigned int State::id() const 
{ 
	return _id;
}


void State::set(State::ID id) 
{ 
	_id = id; 
}


string State::message() const 
{ 
	return _message;
}


void State::setMessage(const string& message) 
{
	_message = message; 
}
	

string State::str(State::ID id) const 
{ 
	// Must be overridden...
	(void)id; 
	return "undefined"; 
}


string State::toString() const 
{ 
	return str(id()); 
}


// ---------------------------------------------------------------------
MutexState::MutexState(State::ID id) : 
	State(id)
{ 
}


// ---------------------------------------------------------------------
StateSignal::StateSignal(State::ID id) : 
	MutexState(id)
{ 
}


bool StateSignal::change(State::ID id) 
{ 
	if (canChange(id)) {
		unsigned int old = this->id();
		MutexState::set(id);
		onChange(id, old);
		return true;
	}
	return false;
}

	
bool StateSignal::canChange(State::ID id) 
{
	// Can be overridden
	if (this->id() != id) 
		return true;
	return false;
}


void StateSignal::onChange(ID id, ID prev) 
{
	// Can be overridden
	Change.emit(this, id, prev);
}


void StateSignal::set(State::ID id) 
{ 
	MutexState::set(id); 
}


} // namespace Sourcey




/*
#include "Sourcey/Logger.h"
#include "Sourcey/Runner.h"
#include "Sourcey/CryptoProvider.h"

#include <assert.h>
*/
		//T& state = this->state();
		//T oldState = state;
		//state.setMessage(message);