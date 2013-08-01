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


#ifndef SOURCEY_Interfaces_H
#define SOURCEY_Interfaces_H


 // depreciate


namespace scy {
namespace abstract {

		
class Runnable
	// A generic interface for classes
	// that can be run and cancelled.
{
public:
	virtual bool run() = 0;
		// The run method will be called repeatedly by the  
		// managing context until this method return false.

	virtual void cancel() {};
		// Cancels the current task.
};

		
class Startable
	// A generic interface for a classes
	// that can be started and stopped.
{
public:
	virtual void start() = 0;
	virtual void stop() = 0;
};

		
class Sendable
	// A generic interface for classes
	// that can be sent and cancelled.
{
public:
	virtual bool send() = 0;
	virtual void cancel() {};
};

	
/*	
class Emitter
	// A generic interface for a classes
	// that emit data packets.
	// Generally associated with a PacketStream.
{
public:	
	void emit(char* data, size_t len, void* opaque);

	void emit(const char* data, size_t len, void* opaque);

	void emit(const std::string& str, void* opaque);
};


template <class TSendable>
class AsyncSendable: public TSendable, public abstract::Runnable
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
	virtual bool run()
	{	
		TSendable::send(); // must block
		delete this;
	}

protected:
	Thread _thread;
};
*/
	 

} } // namespace scy::abstract


#endif // SOURCEY_Interfaces_H
