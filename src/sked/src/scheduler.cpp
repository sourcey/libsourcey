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

		// Gulp
		//Thread::sleep(10);		
		

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



		//sked::Task* task = reinterpret_cast<sked::Task*>(next());
		//scy::Task* task = next(); //reinterpret_cast<sked::Task*>(next());
				//continue;
			
			// Push the task back onto the end of the queue
			//else {
			//	TraceLS(this) << "Replacing Task: " << task << endl;	
			//	Mutex::ScopedLock lock(_mutex);
			//	_tasks.push_back(task);
			//}	
			//sked::Trigger& trigger = reinterpret_cast<sked::Task*>(task)->trigger();

	//if (_tasks.empty()) {
	//	root[(size_t)0];
	//	return;
	//}
	
	//json::Value& entry = root[(size_t)0];
		//entry = root[root.size()];

	
	//reinterpret_cast<sked::Task*>(it->second)->serialize(root[root.size()]);
	//if ((*it).isObject() && 
	//	(*it).isMember(key))
	//	count++;
	//countNestedKeys(*it, key, count, depth);

	/*
	Mutex::ScopedLock lock(_mutex);
	for (auto it = _tasks.begin(); it != _tasks.end(); ++it) {
		TraceLS(this) << "Serializing: " << it->second << endl;
		reinterpret_cast<sked::Task*>(it->second)->serialize(root[root.size()]);
	}
	*/

			/*
			// Update the next schedule time
			//task->trigger().update();

			if (task->trigger().update())
				++it;

			// Destroy the task if it is not repeatable
			else {
				TraceLS(this) << "Clearing Redundant: " << task << endl;
				delete task;
				it = _tasks.erase(it);
			}
			*/

	
/*


sked::TaskList Scheduler::tasks() const
{
	Mutex::ScopedLock lock(_mutex);
	return _tasks;
}

void Scheduler::clear()
{
	Mutex::ScopedLock lock(_mutex);			

	sked::auto it = _tasks.begin();
	while (it != _tasks.end()) {
		sked::Task* task = *it;
		TraceLS(this) << "Clearing Task: " << task << endl;
		delete task;
		it = _tasks.erase(it);
	}
}
*/


	//:
	//_stop(false) 
	//_scheduleAt.start();
	//_thread.start(*this);

	//cout << "[TaskRunner: " << this << "] Destroying" << endl;
	//_stop = true;
	//_wakeUp.set();
	//_thread.join();
	//clear();
	//ClearVector(_tasks);
	//cout << "[TaskRunner: " << this << "] Destroying: OK" << endl;
 //, const sked::TaskOptions& options
	
	// Attempt to stop any matching tasks
	//cancel(task);
	//{
	//	Mutex::ScopedLock lock(_mutex);
	//	_tasks.push_back(task);	//.clone()
	//}
	//sort(_tasks.begin(), _tasks.end(), CompareTimeout);
	
	//TraceLS(this) << "Started: " << task << endl;

	//_scheduleAt = _tasks.front()->scheduleAt();
	//update();
	//_wakeUp.set();

	/*
	TraceLS(this) << "Stopping: " << task << endl;
	
	bool success = false;
	{
		Mutex::ScopedLock lock(_mutex);
		for (sked::auto it = _tasks.begin(); it != _tasks.end(); ++it) {
			if (*it == task) {
				TraceLS(this) << "Stopped: " << *it << endl;
				(*it)->cancel();
				success = true;
				break;
			}
		}
	}
	if (success) {
		//_scheduleAt = _tasks.front()->scheduleAt();
		update();
		_wakeUp.set();
	}
	*/


/*
Timeout Scheduler::scheduleAt() const
{
	Mutex::ScopedLock lock(_mutex);
	return _scheduleAt;
}
*/





	/*
		// Sort all tasks before the first run
			//Task* scheduledTask = dynamic_cast<sked::Task*>(task);
			//if (scheduledTask)

	sked::Task* task;
	//Int64 timeout;
	while (!_stop) {

		// Obtain the next scheduled task
		{
			Mutex::ScopedLock lock(_mutex);			
			task = _tasks.empty() ? nullptr : _tasks.front();
		}

		// Wait for the scheduled timeout interval
		_wakeUp.tryWait(task ? task->trigger().remaining() : 60 * 1000);
		//scheduleAt().remaining()

		// Run the task if required
		if (task && !task->cancelled() && scheduleAt().expired()) {
				
			TraceLS(this) << "Running: " << task << endl;
			//try {
			task->run();				
			//}
			//catch (std::except) {
			//	ErrorLS(this) << "Swallowing Exception: " << exc.what()/message() << endl;
			//}
		}

		// Update and sort the task list
		update();
	}
	
	TraceLS(this) << "Exiting" << endl;
	*/
	

			/*
			// If no tasks are available set the  
			// next timeout to 1 minute.
			if (!task) {
				_scheduleAt.reset();
				_scheduleAt.setDelay(60 * 1000);
				TraceLS(this) << "No tasks" << endl;
			}

			// Otherwise update the scheduled timeout
			else
				_scheduleAt = task->scheduleAt();
			
			TraceLS(this) << "Waiting for " << _scheduleAt.remaining() << endl;
				*/

				/*
				Mutex::ScopedLock l(_mutex);			

				// Update and clean the task list
				sked::auto it = _tasks.begin();
				while (it != _tasks.end()) {
					sked::Task* task = *it;
					if (task->cancelled()) {
						TraceLS(this) << "Clearing Cancelled: " << task << endl;
						//delete task;
						it = _tasks.erase(it);
					}
					else {
						// Update the next schedule time.
						task->trigger().update();
						++it;
					}
				}

				// Re-sort tasks and update our next task event.
				if (!_tasks.empty()) {
					sort(_tasks.begin(), _tasks.end(), CompareTimeout);
					//_scheduleAt = _tasks.front()->scheduleAt();
				}	
				*/

				/*
				//TaskList tasks(this->tasks());

				for (sked::auto it = tasks.begin(); it != tasks.end(); ++it) {
					sked::Task* task = *it;
					if (!task->cancelled()) {
						TraceLS(this) << "Running: " << task << endl;
						task->run();
					}	
				}
				
				//TraceLS(this) << "Clearing Redundant Callbacks" << endl;
				*/

				//Mutex::ScopedLockWithUnlock<Mutex> lock(_mutex);
				//lock.unlock();
				//bool hasRedundant = false;
					//if ((*it)->cancelled()) {
					//	hasRedundant = true;
					//	continue;
					//}
					
					/*
					TraceLS(this) << "Printing Sorted Callbacks" << endl;
					sked::auto it = _tasks.begin();
					while (it != _tasks.end()) {
						TraceLS(this) << "Callback: " 
							<< (*it)->object() << ": " 
							<< (*it)->scheduleAt().remaining() << endl;
						++it;
					}
					*/

/*



void Scheduler::stopAll(const void* klass)
{
	Mutex::ScopedLock lock(_mutex);
	for (sked::auto it = _tasks.begin(); it != _tasks.end(); ++it) {
		if ((*it)->object() == klass) {
			TraceLS(this) << "Stopped: " << (*it)->object() << endl;
			(*it)->cancel();
		}
	}
}


void Scheduler::reset(sked::Task* task) 
{
	Mutex::ScopedLock lock(_mutex);
	bool success = false;
	for (sked::auto it = _tasks.begin(); it != _tasks.end(); ++it) {
		if (**it == task) {
			TraceLS(this) << "Reset: " << (*it)->object() << endl;
			(*it)->scheduleAt().reset();
			success = true;
			break;
		}
	}
	if (success) {
		_scheduleAt = _tasks.front()->scheduleAt();
		_wakeUp.set();
	}
}
*/