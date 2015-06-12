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


#include "scy/sked/scheduler.h"
#include "scy/logger.h"
#include "scy/platform.h"
#include "scy/datetime.h"
#include "scy/singleton.h"

#include <algorithm>
#include "assert.h"


using namespace std;


namespace scy {
namespace sked {


Scheduler::Scheduler()
{	
}


Scheduler::~Scheduler() 
{	
}


void Scheduler::schedule(sked::Task* task)
{
	TaskRunner::start(task);
	//_wakeUp.set();
}


void Scheduler::cancel(sked::Task* task) 
{
	TaskRunner::cancel(task);
	//_wakeUp.set();
}


void Scheduler::clear() 
{
	TaskRunner::clear();
	//_wakeUp.set();
}


void Scheduler::run() 
{
	//TraceLS(this) << "Running" << endl;
	//while (!_stopped) 
	//{	

		// Update and sort the task list bringing the
		// next scheduled task to the front of the list.
		// TODO: Call only after task run, and when the
		// initial task is nullptr.
		update();
		
		// TODO: Create a nextTask member so we don't 
		// need to call next() on each iteration
		sked::Task* task = reinterpret_cast<sked::Task*>(next());
		
		// Run the task
		if (task && 
			task->trigger().timeout()) {	
#if _DEBUG
			{
				DateTime now;
				Timespan remaining = task->trigger().scheduleAt - now;
				TraceLS(this) << "Waiting: "
					<< "\n\tPID: " << task
					<< "\n\tDays: " << remaining.days()
					<< "\n\tHours: " << remaining.totalHours()
					<< "\n\tMinutes: " << remaining.totalMinutes()
					<< "\n\tSeconds: " << remaining.totalSeconds()
					<< "\n\tMilliseconds: " << remaining.totalMilliseconds()
					<< "\n\tCurrentTime: " << DateTimeFormatter::format(now, DateTimeFormat::ISO8601_FORMAT)
					<< "\n\tScheduledAt: " << DateTimeFormatter::format(task->trigger().scheduleAt, DateTimeFormat::ISO8601_FORMAT)
					<< endl;
			}
#endif
			
			// Wait for the scheduled timeout
			//if (!task->trigger().timeout())
			//	_wakeUp.tryWait(static_cast<long>(task->trigger().remaining()));

			// The task list may have changed during the timeout
			// duration, or the current task deleted, so we need
			// to ensure that the next pending task matches the
			// current pending task.
			if (//task == next() &&
				//task->trigger().timeout() &&				
				task->beforeRun()) {	
#if _DEBUG						
				{
					DateTime now;
					TraceLS(this) << "Running: "
						<< "\n\tPID: " << task
						<< "\n\tCurrentTime: " << DateTimeFormatter::format(now, DateTimeFormat::ISO8601_FORMAT)
						<< "\n\tScheduledTime: " << DateTimeFormatter::format(task->trigger().scheduleAt, DateTimeFormat::ISO8601_FORMAT)
						<< endl;
				}
#else
				TraceLS(this) << "Running: " << task << endl;
#endif
				task->run();	
				if (task->afterRun())
					onRun(task);
				else {
					TraceLS(this) << "Destroy After Run: " << task << endl;
					task->_destroyed = true; //destroy();
				}
			}
			else
				TraceLS(this) << "Skipping Task: " << task << endl;
			
			// Destroy the task if needed
			if (task->destroyed()) {
				TraceLS(this) << "Destroy Task: " << task << endl;	
				assert(remove(task));
				delete task;
			}
			
			//TraceLS(this) << "Running: OK: " << task << endl;
		}

		// Go to sleep if we have no tasks
		//else {
		//	TraceLS(this) << "Sleeping" << endl;
		//	_wakeUp.wait();
		//	TraceLS(this) << "Waking up" << endl;
		//}

		// Prevent 100% CPU
		scy::sleep(5);

		// Dispatch the Idle signal
		// TODO: Send Idle complete iteration of all tasks, 
		// rather than after each task.
		//Idle.emit(this);
	//}
			
	//TraceLS(this) << "Shutdown" << endl;		
	//Shutdown.emit(this);
	//TraceLS(this) << "Exiting" << endl;
}


void Scheduler::update()
{
	Mutex::ScopedLock lock(_mutex);
	
	//TraceLS(this) << "Updating: " << _tasks.size() << endl;

	// Update and clean the task list
	auto it = _tasks.begin();
	while (it != _tasks.end()) {
		sked::Task* task = reinterpret_cast<sked::Task*>(*it);
		if (task->destroyed()) {
			it = _tasks.erase(it);
			onRemove(task);
			TraceLS(this) << "Destroy: " << task << endl;
			delete task;
		}
		else
			++it;
	}
	
	// Sort the task list to the next task to 
	// trigger is at the front of the queue.
	sort(_tasks.begin(), _tasks.end(), sked::Task::CompareTimeout);
}


void Scheduler::serialize(json::Value& root)
{
	TraceLS(this) << "Serializing" << endl;
	
	Mutex::ScopedLock lock(_mutex);
	for (auto it = _tasks.begin(); it != _tasks.end(); ++it) {
		sked::Task* task = reinterpret_cast<sked::Task*>(*it);
		TraceLS(this) << "Serializing: " << task << endl;
		json::Value& entry = root[root.size()];
		task->serialize(entry);
		task->trigger().serialize(entry["trigger"]);
	}
}


void Scheduler::deserialize(json::Value& root)
{
	TraceLS(this) << "Deserializing" << endl;
	
	for (auto it = root.begin(); it != root.end(); it++) {
		sked::Task* task = nullptr;
		sked::Trigger* trigger = nullptr;
		try {
			json::assertMember(*it, "trigger");
			task = factory().createTask((*it)["type"].asString());
			task->deserialize((*it));
			trigger = factory().createTrigger((*it)["trigger"]["type"].asString());
			trigger->deserialize((*it)["trigger"]);
			task->setTrigger(trigger);
			schedule(task);
		}
		catch (std::exception& exc) {
			if (task)
				delete task;
			if (trigger)
				delete trigger;
			ErrorLS(this) << "Deserialization Error: " << exc.what() << endl;
		}
	}
}


void Scheduler::print(std::ostream& ost)
{
	json::StyledWriter writer;
	json::Value data;
	serialize(data);
	ost << writer.write(data);
}


Scheduler& Scheduler::getDefault() 
{
	static Singleton<Scheduler> sh;
	return *sh.get();
}


TaskFactory& Scheduler::factory() 
{
	return TaskFactory::getDefault();
}


} } // namespace scy::sked
