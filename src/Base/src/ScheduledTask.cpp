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


#include "Sourcey/ScheduledTask.h"

#include "Poco/DateTimeParser.h"
#include "Poco/Format.h"


using namespace std; 
using namespace Poco; 


namespace Sourcey {
	

ScheduledTask::ScheduledTask()
{
	Log("trace") << "[ScheduledTask: " << this << "] Creating" << endl;
}

	
ScheduledTask::ScheduledTask(Runner& runner) : 
	TimerTask(runner)
{
	Log("trace") << "[ScheduledTask: " << this << "] Creating" << endl;
}


ScheduledTask::~ScheduledTask()
{
	Log("trace") << "[ScheduledTask: " << this << "] Destroying" << endl;
}


void ScheduledTask::schedule(const DateTime& time)
{
	Timestamp now;
	Timestamp::TimeDiff diff = time.timestamp() - now;
	setTimeout(diff / 1000);
}


void ScheduledTask::scheduleRepeated(const DateTime& time, const Timespan& interval)
{
	schedule(time);
	setInterval(interval.totalMilliseconds());
}


DateTime ScheduledTask::time() const
{
	FastMutex::ScopedLock lock(_mutex);	
	return _time;
}


} // namespace Sourcey



	//setTimeout(interval.totalMilliseconds());
    //DateTime now;
    //Timespan s = now += time;
    //Timespan s = time - now;	
	//Log("trace") << "[ScheduledTask: " << this << "] Time Now: " << DateTimeFormatter::format(now, DateTimeFormat::ISO8601_FORMAT) << endl;
	//Log("trace") << "[ScheduledTask: " << this << "] Time Trigger: " << DateTimeFormatter::format(time, DateTimeFormat::ISO8601_FORMAT) << endl;
	//Log("trace") << "[ScheduledTask: " << this << "] Timeout in " << (diff / 1000) << endl;
	//start();

    //DateTime now;
    //Timespan s = time - now;
	//start();


		/*
bool ScheduledTask::run()
{
	bool doTimeout = false;
	bool doDestroy = false;
	{
		Poco::FastMutex::ScopedLock lock(_mutex);
		if (_scheduleAt.expired()) {
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
void ScheduledTask::schedule(time_t time) 
{
	DateTime runAt(Timestamp::fromEpochTime(time));
	schedule(runAt);
}


void ScheduledTask::schedule(const string& time, const std::string& fmt) 
{
	int tzd;
	DateTime runAt(DateTimeParser::parse(fmt, time, tzd));
	schedule(runAt);
}
*/
/*
void ScheduledTask::serialize(JSON::Value& root)
{
	log("trace") << "Serializing" << endl;	
	
	root["time"] = DateTimeFormatter::format(time(), 
		DateTimeFormat::ISO8601_FORMAT);
}


void ScheduledTask::deserialize(JSON::Value& root)
{
	log("trace") << "Deserializing" << endl;

	if (!root.isMember("time"))
		throw Exception("A time member is required.");

	schedule(root["time"].asString());
}
*/
