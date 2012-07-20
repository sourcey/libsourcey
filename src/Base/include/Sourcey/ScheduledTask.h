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


#ifndef SOURCEY_ScheduledTask_H
#define SOURCEY_ScheduledTask_H


#include "Sourcey/TimerTask.h"


namespace Sourcey {


class ScheduledTask: public TimerTask
	/// This class extends the TimerTask to add scheduling
	/// features to the timer.
{
public:
	ScheduledTask();
	ScheduledTask(Runner& runner);
	
	void schedule(const Poco::DateTime& time);
		/// Schedules the task to run once at the given time.

	void scheduleRepeated(const Poco::DateTime& time, const Poco::Timespan& interval);
		/// Scheduled the task to run once at the given time,
		/// and then repeatedly after the given interval has
		/// elapsed.
	
	Poco::DateTime time() const;
		/// Returns the time the task will be run.

protected:
	ScheduledTask& operator=(ScheduledTask const&) {}
	virtual ~ScheduledTask();

	Poco::DateTime _time;	
};
	

} // namespace Sourcey


#endif // SOURCEY_ScheduledTask_H

	


//#include "Sourcey/Spot/IModule.h"
//#include "Sourcey/JSON/ISerializable.h"

	//ScheduledTask(Runner& runner, long timeout = 0, long interval = 0);	
//long timeout = 0, long interval = 0
//protected:
	//Poco::DateTime time();
		/// Returns the time the task will be run.

	//virtual const char* className() const { return "ScheduledTask"; }

	/*

	/// Override onTimeout
protected:	
	virtual void onTimeout() = 0;
		/// Performs task processing when the timer fires.
*/
	//mutable Poco::FastMutex	_mutex;	
	//Poco::DateTime _time;
	//Poco::Timestamp _scheduleAt;
	//bool _repeat;

	
	//virtual void run() = 0;
	//virtual bool valid() = 0;

	
	
	//virtual void serialize(JSON::Value& root);
	//virtual void deserialize(JSON::Value& root);
	//void schedule(std::time_t time);
	//void schedule(const std::string& time, const std::string& fmt = Poco::DateTimeFormat::ISO8601_FORMAT);
		/// Sets the time the task will be run.