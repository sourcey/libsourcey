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


#ifndef SOURCEY_IStartable_H
#define SOURCEY_IStartable_H


namespace scy {

		
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

} // namespace scy


#endif // SOURCEY_IStartable_H
