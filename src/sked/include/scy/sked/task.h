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


#ifndef SCY_Sked_Task_H
#define SCY_Sked_Task_H


#include "scy/taskrunner.h"
#include "scy/json/iserializable.h"
#include "scy/sked/trigger.h"


namespace scy {
namespace sked {


class Scheduler;


class Task: public scy::Task, public json::ISerializable
    /// This class extends the Task class to implement
    /// scheduling capabilities.
{
public:
    Task(const std::string& type = "", const std::string& name = "");
    Task(Scheduler& scheduler, const std::string& type, const std::string& name = "");
    
    //virtual void start();
        
    virtual void serialize(json::Value& root);
        // Serializes the task to JSON.

    virtual void deserialize(json::Value& root);
        // Deserializes the task from JSON.

    template<typename T>
    T* createTrigger() 
    {
        T* p = new T();
        setTrigger(p);
        return p;
    }
    void setTrigger(sked::Trigger* trigger);

    sked::Trigger& trigger();
        // Returns a reference to the associated 
        // sked::Trigger or throws an exception.
    
    Scheduler& scheduler();
        // Returns a reference to the associated 
        // Scheduler or throws an exception.
    
    Int64 remaining() const;
        // Returns the milliseconds remaining 
        // until the next scheduled timeout.
        // An sked::Trigger must be associated
        // or an exception will be thrown.
    
    std::string type() const;
    std::string name() const;
    void setName(const std::string& name);

protected:
    //Task& operator=(Task const&) {}
    virtual ~Task();
    
    virtual bool beforeRun();
    virtual void run() = 0;
    virtual bool afterRun();

    static bool CompareTimeout(const scy::Task* l, const scy::Task* r)
        // For stl::sort operations
    {
        return 
            reinterpret_cast<const Task*>(l)->remaining() <
            reinterpret_cast<const Task*>(r)->remaining();
    }

    friend class Scheduler;
    
    std::string _type;
    std::string _name;
    sked::Scheduler* _scheduler;
    sked::Trigger* _trigger;
    mutable Mutex _mutex;
};


typedef std::vector<sked::Task*> TaskList;
    

} } // namespace scy::sked


#endif // SCY_Sked_Task_H
