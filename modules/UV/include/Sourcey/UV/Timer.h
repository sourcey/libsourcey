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


#ifndef SOURCEY_UV_Timer_H
#define SOURCEY_UV_Timer_H


#include "Sourcey/UV/UVPP.h"
#include "Sourcey/Types.h"
#include "Sourcey/Signal.h"


namespace Sourcey {
namespace UV {


class Timer: public UV::Interface
	/// Wraps libev's ev_timer watcher. Used to get woken up at a specified time
	/// in the future.
{
public:
	Timer(LoopHandle* loop = DefaultLoop);
	virtual ~Timer();

	virtual bool start(Int64 timeout, Int64 interval = 0);
		// Start the timer, an interval value of zero will only trigger once after
		// timeout.

	virtual bool stop();

	virtual bool again();
		// Stop the timer, and if it is repeating restart it using the repeat value
		// as the timeout. If the timer has never been started before it returns -1
		// and sets the error to UV_EINVAL.

	virtual void setInterval(Int64 interval);
		// Set the repeat value. Note that if the repeat value is set from a timer
		// callback it does not immediately take effect. If the timer was non-repeating
		// before, it will have been stopped. If it was repeating, then the old repeat
		// value will have been used to schedule the next timeout.

	virtual Int64 getInterval();
	
	Int64 count();

	void timerCallback(int status);
	
	Signal<Int64> OnTimeout;	

protected:	
	void updateState();
		
	TimerHandle	_handle;
	Int64		_count;
	bool		_active;
};


//
// UV Callbacks
//

UVStatusCallback(Timer, timerCallback, UV::TimerHandle);


} } // namespace Sourcey::UV


#endif // SOURCEY_UV_Timer_H
