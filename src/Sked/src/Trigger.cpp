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


#include "Sourcey/Sked/Trigger.h"
#include "Sourcey/Sked/Scheduler.h"
#include "Sourcey/Logger.h"

#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeParser.h"
#include "Poco/DateTime.h"
#include "Poco/Timespan.h"
#include "Poco/Format.h"


using namespace std; 
using namespace Poco; 


namespace Sourcey {
namespace Sked {


Trigger::Trigger(const string& type, const string& name) :
	type(type),
	name(name),
	timesRun(0)
{
}

	
Int64 Trigger::remaining() 
{
	Poco::DateTime now;
	Poco::Timespan ts = scheduleAt - now;
	return ts.totalMilliseconds();
}

	
bool Trigger::timeout()
{
	Poco::DateTime now;
	return now >= scheduleAt;
}

	
bool Trigger::expired()
{
	return false;
}


void Trigger::serialize(JSON::Value& root)
{
	Log("trace") << "Serializing" << endl;	
	
	root["type"] = type;
	root["name"] = name;
	root["createdAt"] = DateTimeFormatter::format(createdAt, Sked::DateFormat);
	root["scheduleAt"] = DateTimeFormatter::format(scheduleAt, Sked::DateFormat);
	root["lastRunAt"] = DateTimeFormatter::format(lastRunAt, Sked::DateFormat);
	root["timesRun"] = timesRun;
}


void Trigger::deserialize(JSON::Value& root)
{
	Log("trace") << "Deserializing" << endl;
	
	JSON::assertMember(root, "type");
	JSON::assertMember(root, "name");
	JSON::assertMember(root, "createdAt");
	JSON::assertMember(root, "scheduleAt");
	JSON::assertMember(root, "lastRunAt");
	JSON::assertMember(root, "timesRun");
	
	int tzd;
	type = root["type"].asString();
	name = root["name"].asString();
	createdAt = DateTimeParser::parse(Sked::DateFormat, root["createdAt"].asString(), tzd);
	scheduleAt = DateTimeParser::parse(Sked::DateFormat, root["scheduleAt"].asString(), tzd);
	lastRunAt = DateTimeParser::parse(Sked::DateFormat, root["lastRunAt"].asString(), tzd);
	timesRun = root["timesRun"].asInt();
}


// ---------------------------------------------------------------------
//
OnceOnlyTrigger::OnceOnlyTrigger() :
	Trigger("OnceOnlyTrigger", "Once Only")
{
}


bool OnceOnlyTrigger::expired()
{ 
	return timesRun > 0; 
}


// ---------------------------------------------------------------------
//
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


void IntervalTrigger::serialize(JSON::Value& root)
{
	Log("trace") << "Serializing" << endl;	

	Trigger::serialize(root);
	
	root["interval"]["days"] = interval.days();
	root["interval"]["hours"] = interval.hours();
	root["interval"]["minutes"] = interval.minutes();
	root["interval"]["seconds"] = interval.seconds();
}


void IntervalTrigger::deserialize(JSON::Value& root)
{
	Log("trace") << "[IntervalTrigger] Deserializing" << endl;
	
	JSON::assertMember(root, "interval");
	JSON::assertMember(root["interval"], "days");
	JSON::assertMember(root["interval"], "hours");
	JSON::assertMember(root["interval"], "minutes");
	JSON::assertMember(root["interval"], "seconds");

	Trigger::deserialize(root);

	interval.assign(
		root["interval"]["days"].asInt(), 
		root["interval"]["hours"].asInt(), 
		root["interval"]["minutes"].asInt(), 
		root["interval"]["seconds"].asInt(), 0);
	
	if (!interval.totalSeconds())
		throw Exception("Interval trigger must have non zero interval.");
	
	Poco::DateTime now;	
	scheduleAt = now;
	scheduleAt += interval;
}



// ---------------------------------------------------------------------
//
DailyTrigger::DailyTrigger() :
	Trigger("DailyTrigger", "Daily")
{
}


void DailyTrigger::update() 
{
	Poco::DateTime now;
	Poco::DateTime next;
	Poco::DateTime prev(scheduleAt);
	Poco::Timespan day(1, 0, 0, 0, 0);
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
	Log("trace") << "[DailyTrigger] Updating: "
			<< "\n\tDayOfWeek: " << next.dayOfWeek()
			<< "\n\tNowTime: " << Poco::DateTimeFormatter::format(now, Poco::Sked::DateFormat)
			<< "\n\tPrevTime: " << Poco::DateTimeFormatter::format(prev, Poco::Sked::DateFormat)
			<< "\n\tNextTime: " << Poco::DateTimeFormatter::format(next, Poco::Sked::DateFormat)
			<< "\n\tScheduleTime: " << Poco::DateTimeFormatter::format(scheduleAt, Poco::Sked::DateFormat)
			<< endl;
			*/
}


} } // namespace Sourcey::Sked