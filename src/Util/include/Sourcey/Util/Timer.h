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


#ifndef SOURCEY_Timer_H
#define SOURCEY_Timer_H


#include "Sourcey/Logger.h"
#include "Sourcey/Timeout.h"
#include "Sourcey/IPolymorphic.h"

#include "Poco/Thread.h"
#include "Poco/Event.h"

#include <vector>


namespace scy {


class ITimerCallback 
	/// The abstract class for all instantiations of the
	/// TimerCallback template classes.
{
public:
	virtual ITimerCallback* clone() const = 0;

	virtual bool ready() const = 0;
	virtual void invoke() = 0;
	virtual void cancel() = 0;
	virtual bool cancelled() = 0;
	virtual long startInterval() const = 0;
	virtual long periodicInterval() const = 0;
	virtual void setStartInterval(long interval) = 0;
	virtual void setPeriodicInterval(long interval) = 0;
	virtual Timeout scheduleAt() const = 0;
	virtual void* object() const = 0;

	virtual bool operator == (const ITimerCallback& r) const = 0;
};


template <class C>
class TimerCallback: public ITimerCallback 
	/// This template class implements an adapter that sits between
	/// a Timer and an object receiving callbacks from it.
{
public:
	typedef void (C::*Callback)(TimerCallback&);

	TimerCallback(C* object, 
			      Callback method,
				  long startInterval = 0,
				  long periodicInterval = 0,
				  void* opaque = NULL) : 
		_object(object), 
		_method(method), 
		_startInterval(startInterval), 
		_periodicInterval(periodicInterval), 
		_scheduleAt(startInterval), 
		_cancelled(false),
		_opaque(opaque){
		_scheduleAt.start();
	}

	TimerCallback(const TimerCallback& r) : 
		_object(r._object), 
		_method(r._method), 
		_startInterval(r._startInterval), 
		_periodicInterval(r._periodicInterval), 
		_scheduleAt(r._scheduleAt), 
		_cancelled(r._cancelled), 
		_opaque(r._opaque){}
	
	virtual ITimerCallback* clone() const {
		return new TimerCallback(*this);
	}

	virtual bool ready() const {
		return _scheduleAt.expired();
	}
	
	virtual void invoke() {
		if (cancelled())
			return;
		if (_periodicInterval > 0) {
			_scheduleAt.setDelay(_periodicInterval);
			_scheduleAt.reset();
		}
		else cancel();
		(_object->*_method)(*this);
	}

	virtual void cancel() { _cancelled = true; _scheduleAt.stop(); };
	virtual bool cancelled() { return _cancelled; };
	virtual long startInterval() const { return _startInterval;	}
	virtual long periodicInterval() const { return _periodicInterval; }
	virtual void setStartInterval(long interval) { _startInterval = interval; }
	virtual void setPeriodicInterval(long interval) { _periodicInterval = interval; }
	virtual Timeout scheduleAt() const { return _scheduleAt; }
	virtual void* object() const { return _object; }
	virtual void* opaque() const { return _opaque; }
	
	virtual bool operator == (const ITimerCallback& r) const { 
		const TimerCallback* c = dynamic_cast<const TimerCallback*>(&r);
		return c && 
			   c->_object == _object && 
			   c->_method == _method && 
			   // If the comparing start and periodic intervals are 0
			   // they will not be compared.
			   (c->_startInterval == 0 || c->_startInterval == _startInterval) && 
			   (c->_periodicInterval == 0 ||  c->_periodicInterval == _periodicInterval) &&
			   // If client data is set then match against it too.
			   (!c->_opaque || c->_opaque == _opaque);
	}
	
protected:
	TimerCallback();

	C*       _object;
	Callback _method;
	long	 _startInterval;
	long	 _periodicInterval;
	bool	 _cancelled;
	Timeout  _scheduleAt;
	void*    _opaque;
};


typedef std::vector<ITimerCallback*> TimerCallbackList;


class Timer: public Poco::Runnable, public IPolymorphic
	/// The Timer is an asynchronous delayed notification sender
	/// responsible for invoking callbacks of notifications meeting 
	/// specific criteria.
	///
	/// NOTE: This class is depreciated in favor of TimerTask.
{
public:
	Timer();
	virtual ~Timer();

	void start(const ITimerCallback& callback);
	void stop(const ITimerCallback& callback);
	void stopAll(const void* klass);
	void reset(const ITimerCallback& callback);

	static Timer& getDefault();
		/// Returns the default Timer singleton, although Timer
		/// instances may also be initialized individually.

	TimerCallbackList callbacks() const;
	
	virtual const char* className() const { return "Timer"; }

protected:
	void run();

	static bool CompareTimeout(const ITimerCallback* l, const ITimerCallback* r)
		/// For stl::sort operations
	{
		return l->scheduleAt().remaining() < r->scheduleAt().remaining();
	}

protected:	 	
	mutable Poco::FastMutex	_mutex;
	TimerCallbackList	_callbacks;
	Poco::Thread		_thread;
	Poco::Event			_wakeUp;
	Timeout				_scheduleAt;
	bool				_stop;
};


} // namespace scy


#endif // SOURCEY_Timer_H