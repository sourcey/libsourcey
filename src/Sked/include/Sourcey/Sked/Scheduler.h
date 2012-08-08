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


#ifndef SOURCEY_Sked_Scheduler_H
#define SOURCEY_Sked_Scheduler_H


#include "Sourcey/Logger.h"
#include "Sourcey/Runner.h"
#include "Sourcey/JSON/ISerializable.h"
#include "Sourcey/Sked/Task.h"
#include "Sourcey/Sked/TaskFactory.h"

#include "Poco/Thread.h"
#include "Poco/Event.h"
#include "Poco/SingletonHolder.h"

#include <vector>


namespace Sourcey {
namespace Sked {


static const char* DateFormat = "%Y-%m-%d %H:%M:%S %Z";


class Scheduler: public Runner, public JSON::ISerializable
	/// The Scheduler manages and runs tasks 
	/// that need to be executed at specific times.
{
public:
	Scheduler();
	virtual ~Scheduler();

	virtual void schedule(Sked::Task* task);
	virtual void cancel(Sked::Task* task);
		
	virtual void serialize(JSON::Value& root);
	virtual void deserialize(JSON::Value& root);
	
    virtual void print(std::ostream& ost);

	static Scheduler& getDefault();
		/// Returns the default Scheduler singleton,  
		/// although Scheduler instances may also be
		/// initialized individually.
	
	static Sked::TaskFactory& factory();
		/// Returns the TaskFactory singleton.
	
	virtual const char* className() const { return "Scheduler"; }

protected:
	virtual void run();	
	virtual void update();
};


} } // namespace Sourcey::Sked


#endif // SOURCEY_Sked_Scheduler_H