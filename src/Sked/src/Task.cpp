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


#include "Sourcey/Sked/Task.h"
#include "Sourcey/Sked/Scheduler.h"

#include "Poco/DateTimeParser.h"
#include "Poco/DateTime.h"
#include "Poco/Timespan.h"
#include "Poco/Format.h"


using namespace std; 
using namespace Poco; 


namespace scy {
namespace sked {
	

Task::Task(const string& type, const string& name) : 
	scy::Task(true),		
	_type(type),
	_name(name),
	_scheduler(NULL),
	_trigger(NULL)
{
	traceL() << "Creating" << endl;
}

	
Task::Task(sked::Scheduler& scheduler, const string& type, const string& name) : 
	scy::Task(reinterpret_cast<Scheduler&>(scheduler), true, false),
	_type(type),
	_name(name),
	_scheduler(NULL),
	_trigger(NULL)
{
	traceL() << "Creating" << endl;
}


Task::~Task()
{
	traceL() << "Destroying" << endl;
}


void Task::start()
{
	trigger(); // throw if trigger is NULL
	scy::Task::start();
}


void Task::serialize(JSON::Value& root)
{
	traceL() << "Serializing" << endl;	
	
	Poco::FastMutex::ScopedLock lock(_mutex);
	
	root["id"] = _id;
	root["type"] = _type;
	root["name"] = _name;
}


void Task::deserialize(JSON::Value& root)
{
	traceL() << "Deserializing" << endl;
	
	Poco::FastMutex::ScopedLock lock(_mutex);	
	
	JSON::assertMember(root, "id");
	JSON::assertMember(root, "type");
	JSON::assertMember(root, "name");
	
	_id = root["id"].asUInt();
	_type = root["type"].asString();
	_name = root["name"].asString();
}


bool Task::beforeRun()
{
	Poco::FastMutex::ScopedLock lock(_mutex);	
	return _trigger && _trigger->timeout() && !_destroyed && !_cancelled;
}


bool Task::afterRun()
{
	Poco::FastMutex::ScopedLock lock(_mutex);
	Poco::DateTime now;
	_trigger->update();
	_trigger->timesRun++;
	_trigger->lastRunAt = now;
	return !_trigger->expired();
}


void Task::setTrigger(sked::Trigger* trigger)
{
	Poco::FastMutex::ScopedLock lock(_mutex);	
	if (_trigger)
		delete _trigger;
	_trigger = trigger;
}


string Task::name() const
{
	Poco::FastMutex::ScopedLock lock(_mutex);	
	return _name;
}


string Task::type() const
{
	Poco::FastMutex::ScopedLock lock(_mutex);	
	return _type;
}


Int64 Task::remaining() const
{
	Poco::FastMutex::ScopedLock lock(_mutex);	
	if (!_trigger)
		throw Exception("Tasks must be associated with a sked::Trigger instance.");
	return _trigger->remaining();
}


sked::Trigger& Task::trigger()
{
	Poco::FastMutex::ScopedLock lock(_mutex);	
	if (!_trigger)
		throw Exception("Tasks must be associated with a sked::Trigger instance.");
	return *_trigger;
}


sked::Scheduler& Task::scheduler()						 
{ 
	Poco::FastMutex::ScopedLock lock(_mutex);	
	if (!_scheduler)
		throw Exception("Tasks must be started with a sked::Scheduler instance.");
	return *_scheduler;
}


} } // namespace scy::Sked




/*
void Task::setName(const string& name) 
{
	Poco::FastMutex::ScopedLock lock(_mutex);	
	_name = name;
}
*/

	//schedule(time);

	/*
	if (!root.isMember("time"))
		throw Exception("A time member is required.");
	
	int tzd;
	DateTime time(DateTimeParser::parse(sked::DateFormat, root["time"].asString(), tzd));
	schedule(time);
	*/

		//root["time"] = DateTimeFormatter::format(time(), 
		//	sked::DateFormat);

		//if (!root.isMember("time"))
		//	throw Exception("A time member is required.");
	
		//int tzd;
		//DateTime time(DateTimeParser::parse(sked::DateFormat, root["time"].asString(), tzd));
		//schedule(time);


	//if (!&trigger())
	//	throw Exception("A sked::Trigger is required to start the task.");


/*
void Task::schedule(const DateTime& time)
{
	Timestamp now;
	Timestamp::TimeDiff diff = time.timestamp() - now;
	setTimeout(diff / 1000);
}


void Task::scheduleRepeated(const DateTime& time, const Timespan& interval)
{
	schedule(time);
	setInterval(interval.totalMilliseconds());
}


DateTime Task::time() const
{
	FastMutex::ScopedLock lock(_mutex);	
	return _time;
}

*/




	//setTimeout(interval.totalMilliseconds());
    //DateTime now;
    //Timespan s = now += time;
    //Timespan s = time - now;	
	//traceL() << "[Task:" << this << "] Time Now: " << DateTimeFormatter::format(now, sked::DateFormat) << endl;
	//traceL() << "[Task:" << this << "] Time Trigger: " << DateTimeFormatter::format(time, sked::DateFormat) << endl;
	//traceL() << "[Task:" << this << "] Timeout in " << (diff / 1000) << endl;
	//start();

    //DateTime now;
    //Timespan s = time - now;
	//start();


		/*
bool Task::run()
{
	bool doTimeout = false;
	bool doDestroy = false;
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		if (_scheduleAt.timeout()) {
			doTimeout = true;
			if (_interval > 0) {
				_scheduleAt.setDelay(_interval);
				_scheduleAt.reset();
			}
			else doDestroy = true;
		}
	}
	if (doTimeout)
		onTimeout();
	if (doDestroy)
		destroy();
}
		*/



/*
void Task::schedule(time_t time) 
{
	DateTime runAt(Timestamp::fromEpochTime(time));
	schedule(runAt);
}


void Task::schedule(const string& time, const string& fmt) 
{
	int tzd;
	DateTime runAt(DateTimeParser::parse(fmt, time, tzd));
	schedule(runAt);
}
*/
/*
void Task::serialize(JSON::Value& root)
{
	traceL() << "Serializing" << endl;	
	
	root["time"] = DateTimeFormatter::format(time(), 
		sked::DateFormat);
}


void Task::deserialize(JSON::Value& root)
{
	traceL() << "Deserializing" << endl;

	if (!root.isMember("time"))
		throw Exception("A time member is required.");

	schedule(root["time"].asString());
}
*/
