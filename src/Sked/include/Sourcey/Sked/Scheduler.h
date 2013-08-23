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


#ifndef SOURCEY_Sked_Scheduler_H
#define SOURCEY_Sked_Scheduler_H


#include "Sourcey/Logger.h"
#include "Sourcey/Runner.h"
#include "Sourcey/JSON/ISerializable.h"
#include "Sourcey/Sked/Task.h"
#include "Sourcey/Sked/TaskFactory.h"

//
//#include "Poco/Event.h"
#include "Sourcey/Singleton.h"

#include <vector>


namespace scy {
namespace sked {


static const char* DeprecitatedDateFormat = "%Y-%m-%d %H:%M:%S %Z";


class Scheduler: public Runner, public json::ISerializable
	/// The Scheduler manages and runs tasks 
	/// that need to be executed at specific times.
{
public:
	Scheduler();
	virtual ~Scheduler();

	virtual void schedule(sked::Task* task);
	virtual void cancel(sked::Task* task);
	virtual void clear();
		
	virtual void serialize(json::Value& root);
	virtual void deserialize(json::Value& root);
	
    virtual void print(std::ostream& ost);

	static Scheduler& getDefault();
		// Returns the default Scheduler singleton,  
		// although Scheduler instances may also be
		// initialized individually.
	
	static sked::TaskFactory& factory();
		// Returns the TaskFactory singleton.
	
	virtual const char* className() const { return "Scheduler"; }

protected:
	virtual void onIdle();	
	virtual void update();
};


} } // namespace scy::sked


#endif // SOURCEY_Sked_Scheduler_H