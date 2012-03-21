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


#ifndef SOURCEY_Symple_Event_H
#define SOURCEY_Symple_Event_H


#include "Sourcey/Symple/Message.h"


namespace Sourcey {
namespace Symple {


class Event: public Message
{
public:	
	Event();
	Event(const JSON::Value& root);
	Event(const Event& root);
	virtual ~Event();
	
	bool valid() const;
		
	std::string name() const;
	//std::string message() const;
	std::time_t time() const;
		
	void setName(const std::string& name);
	//void setMessage(const std::string& message);
	void setTime(std::time_t time);
	//void setTime(const std::string& time);
		/// Sets the time from a UTC time string.
};


} // namespace Symple 
} // namespace Sourcey


#endif // SOURCEY_Symple_Event_H
