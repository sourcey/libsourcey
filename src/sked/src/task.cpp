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


#include "scy/sked/task.h"
#include "scy/sked/scheduler.h"
#include "scy/datetime.h"


using namespace std; 


namespace scy {
namespace sked {
    

Task::Task(const std::string& type, const std::string& name) : 
    //scy::Task(true),        
    _type(type),
    _name(name),
    _scheduler(nullptr),
    _trigger(nullptr)
{
    TraceL << "Create" << endl;
}

    
Task::Task(sked::Scheduler& scheduler, const std::string& type, const std::string& name) : 
    //scy::Task(true),    
    //scy::Task(reinterpret_cast<Scheduler&>(scheduler), true, false),
    _type(type),
    _name(name),
    _scheduler(&scheduler),
    _trigger(nullptr)
{
    TraceL << "Create" << endl;
}


Task::~Task()
{
    TraceL << "Destroy" << endl;
}


/*
void Task::start()
{
    trigger(); // throw if trigger is nullptr
    scy::Task::start();
}
*/


void Task::serialize(json::Value& root)
{
    TraceL << "Serializing" << endl;    
    
    Mutex::ScopedLock lock(_mutex);
    
    root["id"] = _id;
    root["type"] = _type;
    root["name"] = _name;
}


void Task::deserialize(json::Value& root)
{
    TraceL << "Deserializing" << endl;
    
    Mutex::ScopedLock lock(_mutex);    
    
    json::assertMember(root, "id");
    json::assertMember(root, "type");
    json::assertMember(root, "name");
    
    _id = root["id"].asUInt();
    _type = root["type"].asString();
    _name = root["name"].asString();
}


bool Task::beforeRun()
{
    Mutex::ScopedLock lock(_mutex);    
    return _trigger && _trigger->timeout() && !_destroyed && !cancelled();
}


bool Task::afterRun()
{
    Mutex::ScopedLock lock(_mutex);
    DateTime now;
    _trigger->update();
    _trigger->timesRun++;
    _trigger->lastRunAt = now;
    return !_trigger->expired();
}


void Task::setTrigger(sked::Trigger* trigger)
{
    Mutex::ScopedLock lock(_mutex);    
    if (_trigger)
        delete _trigger;
    _trigger = trigger;
}


string Task::name() const
{
    Mutex::ScopedLock lock(_mutex);    
    return _name;
}


string Task::type() const
{
    Mutex::ScopedLock lock(_mutex);    
    return _type;
}


std::int64_t Task::remaining() const
{
    Mutex::ScopedLock lock(_mutex);    
    if (!_trigger)
        throw std::runtime_error("Tasks must be have a Trigger instance.");
    return _trigger->remaining();
}


sked::Trigger& Task::trigger()
{
    Mutex::ScopedLock lock(_mutex);    
    if (!_trigger)
        throw std::runtime_error("Tasks must have a Trigger instance.");
    return *_trigger;
}


sked::Scheduler& Task::scheduler()                         
{ 
    Mutex::ScopedLock lock(_mutex);    
    if (!_scheduler)
        throw std::runtime_error("Tasks must be started with a sked::Scheduler instance.");
    return *_scheduler;
}


} } // namespace scy::sked
