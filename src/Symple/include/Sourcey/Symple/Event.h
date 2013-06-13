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


#ifndef SOURCEY_Symple_Event_H
#define SOURCEY_Symple_Event_H


#include "Sourcey/Symple/Message.h"


namespace Scy {
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
} // namespace Scy


#endif // SOURCEY_Symple_Event_H
