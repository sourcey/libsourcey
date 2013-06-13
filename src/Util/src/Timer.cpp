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


#include "Sourcey/Util/Timer.h"
#include "Sourcey/Logger.h"
#include "Poco/SingletonHolder.h"

#include <algorithm>
#include "assert.h"


using namespace std;
using namespace Poco;


namespace Scy {


Timer::Timer() :
	_stop(false) 
{	
	_scheduleAt.start();
	_thread.start(*this);
}


Timer::~Timer() 
{	
	cout << "[Timer:" << this << "] Destroying" << endl;
	_stop = true;
	_wakeUp.set();
	_thread.join();
	Util::ClearVector(_callbacks);	
	cout << "[Timer:" << this << "] Destroying: OK" << endl;
}


void Timer::start(const ITimerCallback& callback) 
{
	// Attempt to stop any matching callbacks
	stop(callback);

	FastMutex::ScopedLock lock(_mutex);
	_callbacks.push_back(callback.clone());	
	sort(_callbacks.begin(), _callbacks.end(), CompareTimeout);
	
	log("trace") << "Started: " << callback.object() << endl;

	_scheduleAt = _callbacks.front()->scheduleAt();
	_wakeUp.set();
}


void Timer::stop(const ITimerCallback& callback) 
{
	log("trace") << "Stopping: " << callback.object() << endl;

	FastMutex::ScopedLock lock(_mutex);
	bool success = false;
	for (TimerCallbackList::const_iterator it = _callbacks.begin(); it != _callbacks.end(); ++it) {
		if (**it == callback) {
			log("trace") << "Stopped: " << (*it)->object() << endl;
			(*it)->cancel();
			success = true;
			break;
		}
	}
	if (success) {
		_scheduleAt = _callbacks.front()->scheduleAt();
		_wakeUp.set();
	}
}


void Timer::stopAll(const void* klass)
{
	FastMutex::ScopedLock lock(_mutex);
	for (TimerCallbackList::const_iterator it = _callbacks.begin(); it != _callbacks.end(); ++it) {
		if ((*it)->object() == klass) {
			log("trace") << "Stopped: " << (*it)->object() << endl;
			(*it)->cancel();
		}
	}
}


void Timer::reset(const ITimerCallback& callback) 
{
	FastMutex::ScopedLock lock(_mutex);
	bool success = false;
	for (TimerCallbackList::const_iterator it = _callbacks.begin(); it != _callbacks.end(); ++it) {
		if (**it == callback) {
			log("trace") << "Reset: " << (*it)->object() << endl;
			(*it)->scheduleAt().reset();
			success = true;
			break;
		}
	}
	if (success) {
		_scheduleAt = _callbacks.front()->scheduleAt();
		_wakeUp.set();
	}
}


void Timer::run() 
{
	log("trace") << "Running" << endl;

	while (!_stop) {	
		//try {
			log("trace") << "Waiting for " << _scheduleAt.remaining() << endl;

			_wakeUp.tryWait(_scheduleAt.remaining());
			if (_scheduleAt.expired()) {

				log("trace") << "Timeout" << endl;
				//ScopedLockWithUnlock<Mutex> lock(_mutex);
				//TimerCallbackList callbacks(_callbacks);
				//lock.unlock();
				TimerCallbackList callbacks(this->callbacks());

				// If callbacks are empty and we made it here the last 
				// of our previous callbacks has been cleared. We can
				// restart the timer.
				if (callbacks.empty()) {
					_scheduleAt.reset();
					_scheduleAt.setDelay(60 * 1000);
					continue;
				}

				bool hasRedundant = false;
				for (TimerCallbackList::const_iterator it = callbacks.begin(); it != callbacks.end(); ++it) {
					if ((*it)->cancelled()) {
						hasRedundant = true;
					}
					else if ((*it)->ready()) {
						log("trace") << "Invoking: " << *it << endl;
						(*it)->invoke();
					}	
				}

				// Post processing...
				FastMutex::ScopedLock alock(_mutex);				
				if (hasRedundant) {
					//log("trace") << "Clearing Redundant Callbacks" << endl;
					TimerCallbackList::iterator it = _callbacks.begin();
					while (it != _callbacks.end()) {
						if ((*it)->cancelled()) {
							log("trace") << "Clearing Cancelled: " << (*it)->object() << endl;
							delete *it;
							it = _callbacks.erase(it);
						}
						else
							++it;
					}
				}

				// Re-sort callbacks and update our next callback event.
				if (!_callbacks.empty()) {
					sort(_callbacks.begin(), _callbacks.end(), CompareTimeout);
					_scheduleAt = _callbacks.front()->scheduleAt();
					
					/*
					log("trace") << "Printing Sorted Callbacks" << endl;
					TimerCallbackList::iterator it = _callbacks.begin();
					while (it != _callbacks.end()) {
						log("trace") << "Callback: " 
							<< (*it)->object() << ": " 
							<< (*it)->scheduleAt().remaining() << endl;
						++it;
					}
					*/
				}	
			}
		//}
		//catch (Exception& exc) {
		//	log("error") << "Swallowing Exception: " << exc.displayText() << endl;
		//}
	}
	
	log("trace") << "Exiting" << endl;
}


TimerCallbackList Timer::callbacks() const
{
	FastMutex::ScopedLock lock(_mutex);
	return _callbacks;
}


Timer& Timer::getDefault() 
{
	static SingletonHolder<Timer> sh;
	return *sh.get();
}


} // namespace Scy
