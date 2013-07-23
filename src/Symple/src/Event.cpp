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


#include "Sourcey/Symple/Event.h"
#include "Sourcey/Util.h"
////#include "Poco/DateTimeFormatter.h"
////#include "Poco/DateTimeFormat.h"


using namespace std;
//


namespace scy {
namespace smpl {


Event::Event() 
{
	setType("event");
	setTime(::time(0));
}


Event::Event(const Event& root) :
	Message(root)
{
	if (!isMember("type"))
		setType("event");
	if (!isMember("time"))
		setTime(::time(0));
}


Event::Event(const json::Value& root) :
	Message(root)
{
	if (!isMember("type"))
		setType("event");
	if (!isMember("time"))
		setTime(::time(0));
}


Event::~Event() 
{
}


bool Event::valid() const
{
	return Message::valid()
		&& isMember("name");
}


string Event::name() const 
{
	return get("name", "").asString();
}


/*
string Event::message() const 
{
	return get("message", "").asString();
}
*/


time_t Event::time() const 
{
	return static_cast<time_t>(get("time", 0).asDouble());
}


void Event::setName(const std::string& name)
{
	(*this)["name"] = name;
}


/*
void Event::setMessage(const string& message) 
{
	(*this)["message"] = message;
}
*/

	
void Event::setTime(time_t time) 
{
	(*this)["time"] = (double)time;
	//DateTimeFormatter::format(
	//	Timestamp::fromEpochTime(time), 
	//	DateTimeFormat::ISO8601_FORMAT);
}

	
/*
void Event::setTime(const string& time) 
{
	// no checking done
	(*this)["time"] = time;
}
*/


} // namespace symple 
} // namespace scy