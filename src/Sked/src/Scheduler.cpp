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


#include "Sourcey/Sked/Scheduler.h"
#include "Sourcey/Logger.h"
#include "Poco/SingletonHolder.h"

#include <algorithm>
#include "assert.h"


using namespace std;
using namespace Poco;


namespace Scy {
namespace Sked {


Scheduler::Scheduler()
{	
}


Scheduler::~Scheduler() 
{	
}


void Scheduler::schedule(Sked::Task* task)
{
	Runner::start(task);
	_wakeUp.set();
}


void Scheduler::cancel(Sked::Task* task) 
{
	Runner::cancel(task);
	_wakeUp.set();
}


void Scheduler::clear() 
{
	Runner::clear();
	_wakeUp.set();
}


void Scheduler::run() 
{
	log("trace") << "Running" << endl;

	while (!_stop) 
	{	
		// Update and sort the task list bringing the
		// next scheduled task to the front of the list.
		update();
		
		Sked::Task* task = reinterpret_cast<Sked::Task*>(next());
		
		// Run the task
		if (task) {	
#if _DEBUG
			{
				Poco::DateTime now;
				Poco::Timespan remaining = task->trigger().scheduleAt - now;
				log("trace") << "Waiting: "
					<< "\n\tPID: " << task
					<< "\n\tDays: " << remaining.days()
					<< "\n\tHours: " << remaining.totalHours()
					<< "\n\tMinutes: " << remaining.totalMinutes()
					<< "\n\tSeconds: " << remaining.totalSeconds()
					<< "\n\tMilliseconds: " << remaining.totalMilliseconds()
					<< "\n\tCurrentTime: " << DateTimeFormatter::format(now, Sked::DateFormat)
					<< "\n\tScheduledAt: " << DateTimeFormatter::format(task->trigger().scheduleAt, Sked::DateFormat)
					<< endl;
			}
#endif
			
			// Wait for the scheduled timeout
			if (!task->trigger().timeout())
				_wakeUp.tryWait(static_cast<long>(task->trigger().remaining()));

			// The task list may have changed during the timeout
			// duration, or the current task deleted, so we need
			// to ensure that the next pending task matches the
			// current pending task.
			if (task == next() && 
				task->trigger().timeout() &&
				task->beforeRun()) {	
#if _DEBUG						
				{
					Poco::DateTime now;
					log("trace") << "Running: "
						<< "\n\tPID: " << task
						<< "\n\tCurrentTime: " << DateTimeFormatter::format(now, Sked::DateFormat)
						<< "\n\tScheduledTime: " << DateTimeFormatter::format(task->trigger().scheduleAt, Sked::DateFormat)
						<< endl;
				}
#else
				log("trace") << "Running: " << task << endl;
#endif
				task->run();	
				if (task->afterRun())
					onRun(task);
				else {
					log("trace") << "Destroying After Run: " << task << endl;
					task->_destroyed = true; //destroy();
				}
			}
			else
				log("trace") << "Skipping Task: " << task << endl;
			
			/*
			// Destroy the task if needed
			if (task->destroyed()) {
				log("trace") << "Destroying Task: " << task << endl;	
				assert(remove(task));
				delete task;
			}

			log("trace") << "Running: OK: " << task << endl;
			*/
		}

		// Go to sleep if we have no tasks
		else {
			log("trace") << "Sleeping" << endl;
			_wakeUp.wait();
			log("trace") << "Waking up" << endl;
		}

		// Gulp
		Thread::sleep(10);		

		// Dispatch the Idle signal
		// TODO: Send Idle complete iteration of all tasks, 
		// rather than after each task.
		Idle.emit(this);
	}
			
	log("trace") << "Shutdown" << endl;		
	Shutdown.emit(this);
	log("trace") << "Exiting" << endl;
}


void Scheduler::update()
{
	FastMutex::ScopedLock lock(_mutex);
	
	log("trace") << "Updating: " << _tasks.size() << endl;

	// Update and clean the task list
	TaskList::iterator it = _tasks.begin();
	while (it != _tasks.end()) {
		Sked::Task* task = reinterpret_cast<Sked::Task*>(*it);
		//assert(task);
		if (task->destroyed()) {
			it = _tasks.erase(it);
			onRemove(task);
			log("trace") << "Destroying: " << task << endl;
			delete task;
		}
		else {
			++it;
		}
	}
	
	// Sort the task list to the next task to 
	// trigger is at the front of the queue.
	sort(_tasks.begin(), _tasks.end(), Sked::Task::CompareTimeout);
}


void Scheduler::serialize(JSON::Value& root)
{
	log("trace") << "Serializing" << endl;
	
	FastMutex::ScopedLock lock(_mutex);
	for (TaskList::iterator it = _tasks.begin(); it != _tasks.end(); ++it) {
		Sked::Task* task = reinterpret_cast<Sked::Task*>(*it);
		log("trace") << "Serializing: " << task << endl;
		JSON::Value& entry = root[root.size()];
		task->serialize(entry);
		task->trigger().serialize(entry["trigger"]);
	}
}


void Scheduler::deserialize(JSON::Value& root)
{
	log("trace") << "Deserializing" << endl;
	
	for (JSON::ValueIterator it = root.begin(); it != root.end(); it++) {
		Sked::Task* task = NULL;
		Sked::Trigger* trigger = NULL;
		try {
			JSON::assertMember(*it, "trigger");
			task = factory().createTask((*it)["type"].asString());
			task->deserialize((*it));
			trigger = factory().createTrigger((*it)["trigger"]["type"].asString());
			trigger->deserialize((*it)["trigger"]);
			task->setTrigger(trigger);
			schedule(task);
		}
		catch (Exception& exc) {
			if (task)
				delete task;
			if (trigger)
				delete trigger;
			log("error") << "Deserialization Error: " << exc.displayText() << endl;
		}
	}
}


void Scheduler::print(ostream& ost)
{
	JSON::StyledWriter writer;
	JSON::Value data;
	serialize(data);
	ost << writer.write(data);
}


Scheduler& Scheduler::getDefault() 
{
	static SingletonHolder<Scheduler> sh;
	return *sh.get();
}


Sked::TaskFactory& Scheduler::factory() 
{
	return TaskFactory::getDefault();
}


} } // namespace Scy::Sked



		//Sked::Task* task = reinterpret_cast<Sked::Task*>(next());
		//Scy::Task* task = next(); //reinterpret_cast<Sked::Task*>(next());
				//continue;
			
			// Push the task back onto the end of the queue
			//else {
			//	log("trace") << "Replacing Task: " << task << endl;	
			//	FastMutex::ScopedLock lock(_mutex);
			//	_tasks.push_back(task);
			//}	
			//Sked::Trigger& trigger = reinterpret_cast<Sked::Task*>(task)->trigger();

	//if (_tasks.empty()) {
	//	root[(size_t)0];
	//	return;
	//}
	
	//JSON::Value& entry = root[(size_t)0];
		//entry = root[root.size()];

	
	//reinterpret_cast<Sked::Task*>(it->second)->serialize(root[root.size()]);
	//if ((*it).isObject() && 
	//	(*it).isMember(key))
	//	count++;
	//countNestedKeys(*it, key, count, depth);

	/*
	FastMutex::ScopedLock lock(_mutex);
	for (TaskList::const_iterator it = _tasks.begin(); it != _tasks.end(); ++it) {
		log("trace") << "Serializing: " << it->second << endl;
		reinterpret_cast<Sked::Task*>(it->second)->serialize(root[root.size()]);
	}
	*/

			/*
			// Update the next schedule time
			//task->trigger().update();

			if (task->trigger().update())
				++it;

			// Destroy the task if it is not repeatable
			else {
				log("trace") << "Clearing Redundant: " << task << endl;
				delete task;
				it = _tasks.erase(it);
			}
			*/

	
/*


Sked::TaskList Scheduler::tasks() const
{
	FastMutex::ScopedLock lock(_mutex);
	return _tasks;
}

void Scheduler::clear()
{
	FastMutex::ScopedLock lock(_mutex);			

	Sked::TaskList::iterator it = _tasks.begin();
	while (it != _tasks.end()) {
		Sked::Task* task = *it;
		log("trace") << "Clearing Task: " << task << endl;
		delete task;
		it = _tasks.erase(it);
	}
}
*/


	//:
	//_stop(false) 
	//_scheduleAt.start();
	//_thread.start(*this);

	//cout << "[Runner: " << this << "] Destroying" << endl;
	//_stop = true;
	//_wakeUp.set();
	//_thread.join();
	//clear();
	//ClearVector(_tasks);
	//cout << "[Runner: " << this << "] Destroying: OK" << endl;
 //, const Sked::TaskOptions& options
	
	// Attempt to stop any matching tasks
	//cancel(task);
	//{
	//	FastMutex::ScopedLock lock(_mutex);
	//	_tasks.push_back(task);	//.clone()
	//}
	//sort(_tasks.begin(), _tasks.end(), CompareTimeout);
	
	//log("trace") << "Started: " << task << endl;

	//_scheduleAt = _tasks.front()->scheduleAt();
	//update();
	//_wakeUp.set();

	/*
	log("trace") << "Stopping: " << task << endl;
	
	bool success = false;
	{
		FastMutex::ScopedLock lock(_mutex);
		for (Sked::TaskList::const_iterator it = _tasks.begin(); it != _tasks.end(); ++it) {
			if (*it == task) {
				log("trace") << "Stopped: " << *it << endl;
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
	FastMutex::ScopedLock lock(_mutex);
	return _scheduleAt;
}
*/





	/*
		// Sort all tasks before the first run
			//Task* scheduledTask = dynamic_cast<Sked::Task*>(task);
			//if (scheduledTask)

	Sked::Task* task;
	//Int64 timeout;
	while (!_stop) {

		// Obtain the next scheduled task
		{
			FastMutex::ScopedLock lock(_mutex);			
			task = _tasks.empty() ? NULL : _tasks.front();
		}

		// Wait for the scheduled timeout interval
		_wakeUp.tryWait(task ? task->trigger().remaining() : 60 * 1000);
		//scheduleAt().remaining()

		// Run the task if required
		if (task && !task->cancelled() && scheduleAt().expired()) {
				
			log("trace") << "Running: " << task << endl;
			//try {
			task->run();				
			//}
			//catch (Exception& exc) {
			//	log("error") << "Swallowing Exception: " << exc.displayText() << endl;
			//}
		}

		// Update and sort the task list
		update();
	}
	
	log("trace") << "Exiting" << endl;
	*/
	

			/*
			// If no tasks are available set the  
			// next timeout to 1 minute.
			if (!task) {
				_scheduleAt.reset();
				_scheduleAt.setDelay(60 * 1000);
				log("trace") << "No tasks" << endl;
			}

			// Otherwise update the scheduled timeout
			else
				_scheduleAt = task->scheduleAt();
			
			log("trace") << "Waiting for " << _scheduleAt.remaining() << endl;
				*/

				/*
				FastMutex::ScopedLock l(_mutex);			

				// Update and clean the task list
				Sked::TaskList::iterator it = _tasks.begin();
				while (it != _tasks.end()) {
					Sked::Task* task = *it;
					if (task->cancelled()) {
						log("trace") << "Clearing Cancelled: " << task << endl;
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

				for (Sked::TaskList::const_iterator it = tasks.begin(); it != tasks.end(); ++it) {
					Sked::Task* task = *it;
					if (!task->cancelled()) {
						log("trace") << "Running: " << task << endl;
						task->run();
					}	
				}
				
				//log("trace") << "Clearing Redundant Callbacks" << endl;
				*/

				//ScopedLockWithUnlock<Mutex> lock(_mutex);
				//lock.unlock();
				//bool hasRedundant = false;
					//if ((*it)->cancelled()) {
					//	hasRedundant = true;
					//	continue;
					//}
					
					/*
					log("trace") << "Printing Sorted Callbacks" << endl;
					Sked::TaskList::iterator it = _tasks.begin();
					while (it != _tasks.end()) {
						log("trace") << "Callback: " 
							<< (*it)->object() << ": " 
							<< (*it)->scheduleAt().remaining() << endl;
						++it;
					}
					*/

/*



void Scheduler::stopAll(const void* klass)
{
	FastMutex::ScopedLock lock(_mutex);
	for (Sked::TaskList::const_iterator it = _tasks.begin(); it != _tasks.end(); ++it) {
		if ((*it)->object() == klass) {
			log("trace") << "Stopped: " << (*it)->object() << endl;
			(*it)->cancel();
		}
	}
}


void Scheduler::reset(Sked::Task* task) 
{
	FastMutex::ScopedLock lock(_mutex);
	bool success = false;
	for (Sked::TaskList::const_iterator it = _tasks.begin(); it != _tasks.end(); ++it) {
		if (**it == task) {
			log("trace") << "Reset: " << (*it)->object() << endl;
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