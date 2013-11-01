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


#include "Sourcey/Sked/Trigger.h"
#include "Sourcey/Sked/Scheduler.h"
#include "Sourcey/Logger.h"
#include "Sourcey/DateTime.h"


using namespace std; 


namespace scy {
namespace sked {


Trigger::Trigger(const std::string& type, const std::string& name) :
	type(type),
	name(name),
	timesRun(0)
{
}

	
Int64 Trigger::remaining() 
{
	DateTime now;
	Timespan ts = scheduleAt - now;
	return ts.totalMilliseconds();
}

	
bool Trigger::timeout()
{
	DateTime now;
	return now >= scheduleAt;
}

	
bool Trigger::expired()
{
	return false;
}


void Trigger::serialize(json::Value& root)
{
	traceL() << "Serializing" << endl;	
	
	root["type"] = type;
	root["name"] = name;
	root["createdAt"] = DateTimeFormatter::format(createdAt, DateTimeFormat::ISO8601_FORMAT);
	root["scheduleAt"] = DateTimeFormatter::format(scheduleAt, DateTimeFormat::ISO8601_FORMAT);
	root["lastRunAt"] = DateTimeFormatter::format(lastRunAt, DateTimeFormat::ISO8601_FORMAT);
	root["timesRun"] = timesRun;
}


void Trigger::deserialize(json::Value& root)
{
	traceL() << "Deserializing" << endl;
	
	json::assertMember(root, "type");
	json::assertMember(root, "name");
	json::assertMember(root, "createdAt");
	json::assertMember(root, "scheduleAt");
	json::assertMember(root, "lastRunAt");
	json::assertMember(root, "timesRun");
	
	int tzd;
	type = root["type"].asString();
	name = root["name"].asString();
	createdAt = DateTimeParser::parse(DateTimeFormat::ISO8601_FORMAT, root["createdAt"].asString(), tzd);
	scheduleAt = DateTimeParser::parse(DateTimeFormat::ISO8601_FORMAT, root["scheduleAt"].asString(), tzd);
	lastRunAt = DateTimeParser::parse(DateTimeFormat::ISO8601_FORMAT, root["lastRunAt"].asString(), tzd);
	timesRun = root["timesRun"].asInt();
}


// ---------------------------------------------------------------------
OnceOnlyTrigger::OnceOnlyTrigger() :
	Trigger("OnceOnlyTrigger", "Once Only")
{
}


bool OnceOnlyTrigger::expired()
{ 
	return timesRun > 0; 
}


// ---------------------------------------------------------------------
IntervalTrigger::IntervalTrigger() :
	Trigger("IntervalTrigger", "Interval"),
	maxTimes(0)
{
}


void IntervalTrigger::update()
{
	scheduleAt += interval;
}


bool IntervalTrigger::expired()
{ 
	return maxTimes > 0 && timesRun >= maxTimes; 
}


void IntervalTrigger::serialize(json::Value& root)
{
	traceL() << "Serializing" << endl;	

	Trigger::serialize(root);
	
	root["interval"]["days"] = interval.days();
	root["interval"]["hours"] = interval.hours();
	root["interval"]["minutes"] = interval.minutes();
	root["interval"]["seconds"] = interval.seconds();
}


void IntervalTrigger::deserialize(json::Value& root)
{
	traceL() << "[IntervalTrigger] Deserializing" << endl;
	
	json::assertMember(root, "interval");
	json::assertMember(root["interval"], "days");
	json::assertMember(root["interval"], "hours");
	json::assertMember(root["interval"], "minutes");
	json::assertMember(root["interval"], "seconds");

	Trigger::deserialize(root);

	interval.assign(
		root["interval"]["days"].asInt(), 
		root["interval"]["hours"].asInt(), 
		root["interval"]["minutes"].asInt(), 
		root["interval"]["seconds"].asInt(), 0);
	
	if (!interval.totalSeconds())
		throw std::runtime_error("Interval trigger must have non zero interval.");
	
	DateTime now;	
	scheduleAt = now;
	scheduleAt += interval;
}



// ---------------------------------------------------------------------
DailyTrigger::DailyTrigger() :
	Trigger("DailyTrigger", "Daily")
{
}


void DailyTrigger::update() 
{
	DateTime now;
	DateTime next;
	DateTime prev(scheduleAt);
	Timespan day(1, 0, 0, 0, 0);
	bool initial = createdAt == scheduleAt;
		
	// Set next date as tomorrow if the schedule 
	// timeout is expired, or if we are setting up.
	if (!initial && now > scheduleAt)
		next += day;
		
	// Get the next day the task can run
	bool match = false;
	for (unsigned i = 0; i < 6; i++) {
		match = false;
		for (unsigned x = 0; x < daysExcluded.size(); x++) {
			if (daysExcluded[x] == next.dayOfWeek()) {
				match = true;

				// Increment the start date
				next += day;
			}
		}
		if (!match)
			break;
	}

	// Assign the next scheduled time
	scheduleAt.assign(
		next.year(),
		next.month(),
		next.day(),
		timeOfDay.hour(),
		timeOfDay.minute(),
		timeOfDay.second(),
		timeOfDay.millisecond(),
		timeOfDay.microsecond());
	
	/*
	traceL() << "[DailyTrigger] Updating: "
			<< "\n\tDayOfWeek: " << next.dayOfWeek()
			<< "\n\tNowTime: " << DateTimeFormatter::format(now, Poco::DateTimeFormat::ISO8601_FORMAT)
			<< "\n\tPrevTime: " << DateTimeFormatter::format(prev, Poco::DateTimeFormat::ISO8601_FORMAT)
			<< "\n\tNextTime: " << DateTimeFormatter::format(next, Poco::DateTimeFormat::ISO8601_FORMAT)
			<< "\n\tScheduleTime: " << DateTimeFormatter::format(scheduleAt, Poco::DateTimeFormat::ISO8601_FORMAT)
			<< endl;
			*/
}


} } // namespace scy::sked