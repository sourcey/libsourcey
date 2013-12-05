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


#include "scy/stateful.h"


using std::endl;


namespace scy {


State::State(State::ID id, const std::string& message) : 
	_id(id), _message(message)
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


std::string State::message() const 
{ 
	return _message;
}


void State::setMessage(const std::string& message) 
{
	_message = message; 
}
	

std::string State::str(State::ID id) const 
{ 
	// Must be overridden...
	(void)id; 
	return "undefined"; 
}


std::string State::toString() const 
{ 
	return str(id()); 
}


//
// Mutex State
//


MutexState::MutexState(State::ID id) : 
	State(id)
{ 
}


//
// State Signal
//


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
	//Change.emit(this, id, prev);
}


void StateSignal::set(State::ID id) 
{ 
	MutexState::set(id); 
}


} // namespace scy