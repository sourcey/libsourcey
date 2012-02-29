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


#ifndef SOURCEY_ISendable_H
#define SOURCEY_ISendable_H


#include "Poco/Thread.h"


namespace Sourcey {

		
class ISendable
	/// Defines a generic interface for class
	/// that can be send and cancelled.
{
public:
	virtual bool send() = 0;
	virtual void cancel() = 0;
};


template <class TSendable>
class AsyncSendable: public TSendable, public Poco::Runnable
{
public:
	AsyncSendable() {};
	virtual ~AsyncSendable() {};

	virtual bool send() 
	{
		_thread.start(*this);
		return true;
	}
	
	virtual void cancel()
	{
		TSendable::cancel();
		_thread.join();
	}
	
protected:
	virtual void run()
	{	
		TSendable::send(); // must block
		delete this;
	}

protected:
	Poco::Thread _thread;
};


} // namespace Sourcey


#endif // SOURCEY_ISendable_H
