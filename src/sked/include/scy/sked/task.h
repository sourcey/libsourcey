///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup sked
/// @{


#ifndef SCY_Sked_Task_H
#define SCY_Sked_Task_H


#include "scy/json/iserializable.h"
#include "scy/sked/trigger.h"
#include "scy/task.h"


namespace scy {
namespace sked {


class Scheduler;


/// This class extends the Task class to implement
/// scheduling capabilities.
class Task : public scy::Task, public json::ISerializable
{
public:
    Task(const std::string& type= "", const std::string& name= "");
    Task(Scheduler& scheduler, const std::string& type,
         const std::string& name= "");

    // virtual void start();

    /// Serializes the task to JSON.
    virtual void serialize(json::Value& root);

    /// Deserializes the task from JSON.
    virtual void deserialize(json::Value& root);


    template <typename T> T* createTrigger()
    {
        T* p= new T();
        setTrigger(p);
        return p;
    }
    void setTrigger(sked::Trigger* trigger);

    /// Returns a reference to the associated
    /// sked::Trigger or throws an exception.
    sked::Trigger& trigger();

    /// Returns a reference to the associated
    /// Scheduler or throws an exception.
    Scheduler& scheduler();

    /// Returns the milliseconds remaining
    /// until the next scheduled timeout.
    /// An sked::Trigger must be associated
    /// or an exception will be thrown.
    std::int64_t remaining() const;

    std::string type() const;
    std::string name() const;
    void setName(const std::string& name);

protected:
    // Task& operator=(Task const&) {}
    virtual ~Task();

    virtual bool beforeRun();
    virtual void run()= 0;
    virtual bool afterRun();

    // For stl::sort operations
    static bool CompareTimeout(const scy::Task* l, const scy::Task* r)
    {
        return reinterpret_cast<const Task*>(l)->remaining() <
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


} // namespace sked
} // namespace scy


#endif // SCY_Sked_Task_H


/// @\}
