//
// This software is copyright by Sourcey <mail@sourcey.com> and is distributed under a dual license:
// Copyright (C) 2005 Sourcey
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


#include "Sourcey/Timer.h"
#include "Sourcey/Logger.h"
#include "Poco/SingletonHolder.h"
#include "assert.h"


using namespace std;
using namespace Poco;


namespace Sourcey {


Timer::Timer() :
	_stop(false) 
{	
	_scheduleAt.start();
	_thread.start(*this);
}


Timer::~Timer() 
{	
	cout << "[Timer: " << this << "] Drestroying" << endl;
	_stop = true;
	_wakeUp.set();
	_thread.join();
	Util::ClearVector(_callbacks);	
}


void Timer::start(const ITimerCallback& callback) 
{
	// Attempt to stop any matching callbacks
	stop(callback);

	Mutex::ScopedLock lock(_mutex);
	_callbacks.push_back(callback.clone());	
	sort(_callbacks.begin(), _callbacks.end(), CompareTimeout);
	
	Log("trace") << "[Timer:" << this << "] Started: " << callback.object() << endl;

	_scheduleAt = _callbacks.front()->scheduleAt();
	_wakeUp.set();
}


void Timer::stop(const ITimerCallback& callback) 
{
	Mutex::ScopedLock lock(_mutex);
	bool success = false;
	for (TimerCallbackList::const_iterator it = _callbacks.begin(); it != _callbacks.end(); ++it) {
		if (**it == callback) {
			Log("trace") << "[Timer:" << this << "] Stopped: " << (*it)->object() << endl;
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
	Mutex::ScopedLock lock(_mutex);
	for (TimerCallbackList::const_iterator it = _callbacks.begin(); it != _callbacks.end(); ++it) {
		if ((*it)->object() == klass) {
			Log("trace") << "[Timer:" << this << "] Stopped: " << (*it)->object() << endl;
			(*it)->cancel();
		}
	}
}


void Timer::reset(const ITimerCallback& callback) 
{
	Mutex::ScopedLock lock(_mutex);
	bool success = false;
	for (TimerCallbackList::const_iterator it = _callbacks.begin(); it != _callbacks.end(); ++it) {
		if (**it == callback) {
			Log("trace") << "[Timer:" << this << "] Reset: " << (*it)->object() << endl;
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
	Log("trace") << "[Timer:" << this << "] Running" << endl;

	while (!_stop) {	
		try {
			//Log("trace") << "[Timer:" << this << "] Waiting for " << _scheduleAt.remaining() << endl;

			_wakeUp.tryWait(_scheduleAt.remaining());
			if (_scheduleAt.expired()) {

				//Log("trace") << "[Timer:" << this << "] Timeout" << endl;
				ScopedLockWithUnlock<Mutex> lock(_mutex);
				TimerCallbackList toNotify(_callbacks);
				lock.unlock();

				// If callbacks are empty and we made it here the last 
				// of our previous callbacks has been cleared. We can
				// restart the timer.
				if (toNotify.empty()) {
					_scheduleAt.reset();
					_scheduleAt.setDelay(60 * 1000);
					continue;
				}

				bool hasRedundant = false;
				for (TimerCallbackList::const_iterator it = toNotify.begin(); it != toNotify.end(); ++it) {
					if ((*it)->cancelled()) {
						hasRedundant = true;
					}
					else if ((*it)->ready()) {
						Log("trace") << "[Timer:" << this << "] Invoking: " << *it << endl;
						(*it)->invoke();
					}	
				}

				// Post processing...
				Mutex::ScopedLock alock(_mutex);				
				if (hasRedundant) {
					//Log("trace") << "[Timer:" << this << "] Clearing Redundant Callbacks" << endl;
					TimerCallbackList::iterator it = _callbacks.begin();
					while (it != _callbacks.end()) {
						if ((*it)->cancelled()) {
							Log("trace") << "[Timer:" << this << "] Clearing Cancelled: " << (*it)->object() << endl;
							delete *it;
							it = _callbacks.erase(it);
						}
						else
							++it;
					}
				}

				// Re-sort callbacks and update our next callback event.
				if (!_callbacks.empty()) {
					_scheduleAt = _callbacks.front()->scheduleAt();
					sort(_callbacks.begin(), _callbacks.end(), CompareTimeout);
				}	
			}
		}
		catch (Exception& exc) {
			Log("error") << "[Timer:" << this << "] Swallowing Exception: " << exc.displayText() << endl;
		}
	}

	Log("trace") << "[Timer:" << this << "] Exiting" << endl;
}


Timer& Timer::getDefault() 
{
	static SingletonHolder<Timer> sh;
	return *sh.get();
}


} // namespace Sourcey
