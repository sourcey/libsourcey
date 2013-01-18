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


#ifndef SOURCEY_Sked_Trigger_H
#define SOURCEY_Sked_Trigger_H


//#include "Sourcey/Task.h"
#include "Sourcey/JSON/ISerializable.h"

#include "Poco/DateTime.h"
#include "Poco/Timespan.h"

	
namespace Sourcey {
namespace Sked {


enum DaysOfTheWeek
	/// Days of the week
{
    Sunday = 0,
    Monday = 1,
    Tuesday = 2,
    Wednesday = 3,
    Thursday = 4,
    Friday = 5,
    Saturday = 6
};


enum MonthOfTheYeay
	/// Months of the year
{
    January = 0,
    February = 1,
    March = 2,
    April = 3,
    May = 4,
    June = 5,
    July = 6,
    August = 7,
    September = 8,
    October = 9,
    November = 10,
    December = 11
};
          

// ---------------------------------------------------------------------
//
struct Trigger: public JSON::ISerializable
{
	Trigger(const std::string& type = "", const std::string& name = "");

	virtual void update() = 0;
		/// Updates the scheduleAt value to the
		/// next scheduled time.
		/// Return false here to destroy the task.
	
	virtual Int64 remaining();
		/// Returns the milliseconds remaining 
		/// until the next scheduled timeout.
	
	virtual bool timeout();
		/// Returns true if the task is ready to
		/// be run, false otherwise.
	
	virtual bool expired();
		/// Returns true if the task is expired
		/// and should be destroyed.
		/// Returns false by default.
	
	virtual void serialize(JSON::Value& root);
	virtual void deserialize(JSON::Value& root);

	std::string type;
		/// The type of this trigger class.

	std::string name;
		/// The display name of this trigger class.
	
	int timesRun;
		/// The number of times the task has run
		/// since creation;
	
	Poco::DateTime createdAt;
		/// The time the task was created.

	Poco::DateTime scheduleAt;
		/// The time the task is scheduled to run.

	Poco::DateTime lastRunAt;
		/// The time the task was last run.
};


// ---------------------------------------------------------------------
//
struct OnceOnlyTrigger: public Trigger
{
	OnceOnlyTrigger();

	virtual void update() {
		// Nothing to do since scheduleAt contains 
		// the correct date and we run once only.
	}

	virtual bool expired();
};


// ---------------------------------------------------------------------
//
struct IntervalTrigger: public Trigger
{
	IntervalTrigger();

	virtual void update();
	virtual bool expired();
	
	virtual void serialize(JSON::Value& root);
	virtual void deserialize(JSON::Value& root);

	Poco::Timespan interval;
		/// This value represents the interval to wait
		/// before running the task.

	int maxTimes;
		/// The maximum number of times the task will
		/// be run before it is destroyed.
		/// 0 for no effect.
};


// ---------------------------------------------------------------------
//
struct DailyTrigger: public Trigger
{
	DailyTrigger();

	virtual void update();

	Poco::DateTime timeOfDay;
		/// This value represents the time of day the
		/// task will trigger.
		/// The date part of the timestamp is redundant.

	std::vector<DaysOfTheWeek> daysExcluded;
		/// Days of the week can be excluded by adding
		/// the appropriate bit flag here. 
};
	

} } // namespace Sourcey::Sked


#endif // SOURCEY_Sked_Trigger_H