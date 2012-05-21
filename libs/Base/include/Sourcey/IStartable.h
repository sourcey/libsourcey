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

#ifndef SOURCEY_IStartable_H
#define SOURCEY_IStartable_H


namespace Sourcey {

		
class IStartable
	/// Defines a generic interface for a
	/// class that can be started and stopped.
{
public:
	virtual void start() = 0;
	virtual void stop() = 0;
};


template <class TStartable>
class AsyncStartable: public TStartable, public Poco::Runnable
{
public:
	AsyncStartable() {};
	virtual ~AsyncStartable() {};

	virtual bool start() 
	{
		_thread.start(*this);
		return true;
	}
	
	virtual void stop()
	{
		TStartable::stop();
		_thread.join();
	}
	
protected:
	virtual void run()
	{	
		TStartable::start(); // must block
		delete this;
	}

protected:
	Poco::Thread _thread;
};

} // namespace Sourcey


#endif // SOURCEY_IStartable_H
