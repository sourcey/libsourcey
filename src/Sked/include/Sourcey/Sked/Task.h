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


#ifndef SOURCEY_Sked_Task_H
#define SOURCEY_Sked_Task_H


#include "Sourcey/Task.h"
#include "Sourcey/JSON/ISerializable.h"
#include "Sourcey/Sked/Trigger.h"


namespace Sourcey {
namespace Sked {


class Scheduler;


class Task: public Sourcey::Task, public JSON::ISerializable
	/// This class extends the Task class to implement
	/// scheduling capabilities.
{
public:
	Task(const std::string& type, const std::string& name = "");
	Task(Scheduler& scheduler, const std::string& type, const std::string& name = "");
	
	virtual void start();
		
	virtual void serialize(JSON::Value& root);
		/// Serializes the task to JSON.

	virtual void deserialize(JSON::Value& root);
		/// Deserializes the task from JSON.

	template<typename T>
	T* createTrigger() 
	{
		T* p = new T();
		setTrigger(p);
		return p;
	}
	void setTrigger(Sked::Trigger* trigger);

	Sked::Trigger& trigger();
		/// Returns a reference to the associated 
		/// Sked::Trigger or throws an exception.
	
	Scheduler& scheduler();
		/// Returns a reference to the associated 
		/// Scheduler or throws an exception.
	
	Int64 remaining() const;
		/// Returns the milliseconds remaining 
		/// until the next scheduled timeout.
		/// An Sked::Trigger must be associated
		/// or an exception will be thrown.
	
	std::string type() const;
	std::string name() const;
	void setName(const std::string& name);

	virtual const char* className() const { return "Task"; }

protected:
	Task& operator=(Task const&) {}
	virtual ~Task();
	
	virtual bool beforeRun();
	virtual void run() = 0;
	virtual bool afterRun();

	static bool CompareTimeout(const Sourcey::Task* l, const Sourcey::Task* r)
		/// For stl::sort operations
	{
		return 
			reinterpret_cast<const Task*>(l)->remaining() <
			reinterpret_cast<const Task*>(r)->remaining();
	}
	
	Sked::Scheduler* _scheduler;
	Sked::Trigger*   _trigger;
	std::string      _type;
	std::string      _name;

	friend class Scheduler;
};


typedef std::vector<Sked::Task*> TaskList;
	

} } // namespace Sourcey::Sked


#endif // SOURCEY_Sked_Task_H

	


	//Flags daysExcluded;
	
		//Poco::Timespan ts = now - scheduleAt;
	/*
	virtual void setTimeout(Poco::DateTime& time) {
		Poco::DateTime now;
		Poco::Timespan ts = now - time;
		timeout.setDelay(ts.totalMilliseconds());
	}
	
	Poco::DateTime createdAt;
	Poco::DateTime scheduleAt;
	*/
		
		//int nextDay;
		//nextDay = now.dayOfWeek();
		//setTimeout(timeOfDay);
	//Timeout timeout;

	//virtual void setTimeout(Poco::DateTime& time) {
	//{
	//	Poco::Timestamp now;
	//	return scheduleAt > now;
	//}
		
		//Timestamp now;
		//Timespan ts = now - scheduleAt;
		//timeout.setDelay(ts / 1000);
		
		//Poco::DateTime now;
			
		//while (true) {	
		//}



		//if ()
		//Poco::DateTime scheduleAt;



		//Poco::DateTime now;
		//Poco::Timespan ts = now - scheduleAt;
		//timeout.setDelay(ts / 1000);
	
	//now;
	
		//scheduleAt.timestamp();
		//Poco::Timestamp now;
		//return scheduleAt > now;
		//Poco::Timestamp now
	//Timeout timeout;


//#include "Sourcey/Spot/IModule.h"
//#include "Sourcey/JSON/ISerializable.h"

	//Task(Scheduler& runner, long timeout = 0, long interval = 0);	
//long timeout = 0, long interval = 0
//protected:
	//Poco::Timestamp time();
		/// Returns the time the task will be run.

	//virtual const char* className() const { return "Task"; }

	/*

	/// Override onTimeout
protected:	
	virtual void onTimeout() = 0;
		/// Performs task processing when the timer fires.
*/
	//mutable Poco::FastMutex	_mutex;	
	//Poco::Timestamp _time;
	//Poco::Timestamp _scheduleAt;
	//bool _repeat;

	
	//virtual void run() = 0;
	//virtual bool valid() = 0;

	
	
	//virtual void serialize(JSON::Value& root);
	//virtual void deserialize(JSON::Value& root);
	//void schedule(std::time_t time);
	//void schedule(const std::string& time, const std::string& fmt = Poco::TimestampFormat::ISO8601_FORMAT);
		/// Sets the time the task will be run.