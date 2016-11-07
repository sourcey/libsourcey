///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup sked
/// @{


#ifndef SCY_Sked_Scheduler_H
#define SCY_Sked_Scheduler_H


#include "scy/logger.h"
#include "scy/task.h"
#include "scy/json/iserializable.h"
#include "scy/sked/task.h"
#include "scy/sked/taskfactory.h"
#include "scy/singleton.h"

#include <vector>


namespace scy {
namespace sked {


/// The Scheduler manages and runs tasks
/// that need to be executed at specific times.
class Scheduler: public TaskRunner, public json::ISerializable
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

    /// Returns the default Scheduler singleton,
    /// although Scheduler instances may also be
    /// initialized individually.
    static Scheduler& getDefault();

    /// Returns the TaskFactory singleton.
    static sked::TaskFactory& factory();

protected:
    virtual void run();
    virtual void update();
};


// static std::string DepreciatedDateFormat = "%Y-%m-%d %H:%M:%S %Z";


} } // namespace scy::sked


#endif // SCY_Sked_Scheduler_H

/// @\}
