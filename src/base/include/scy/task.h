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


#ifndef SCY_Task_H
#define SCY_Task_H


#include <cstdint>
#include "scy/interface.h"
#include "scy/mutex.h"


namespace scy {


#if 0
template <class async::RunnableT>
class ITask: public async::RunnableT
    // This class defines an asynchronous Task which is
    // managed by a TaskRunner.
{
public:
    virtual bool start() = 0;
    virtual bool cancel() = 0;
    virtual bool destroy() = 0;

    virtual bool cancelled() const = 0;
    virtual bool destroyed() const = 0;
    virtual bool repeating() const = 0;
        // Returns true if the task should be run once only

    virtual TaskRunner& runner();
        // Returns a reference to the affiliated TaskRunner or
        // throws an exception.

protected:
    Task& operator=(Task const&) = 0; // {}
    virtual ~Task() = 0;
        // CAUTION: The destructor should be private, but we
        // left it protected for implementational flexibility. The
        // reason being that if the derived task is programmatically
        // destroyed there is a chance that the TaskRunner will call
        // run() as a pure virtual method.

    virtual bool beforeRun();
        // Called by the TaskRunner to determine weather the task can
        // be run or not. It is safe to destroy() the task from
        // inside this method.
        // This method returns true by default.

    virtual void run() = 0;
        // Called by the TaskRunner to run the task.
        // Override this method to implement task logic.

protected:
    mutable Mutex    _mutex;

    bool _cancelled;
    bool _destroyed;
    bool _repeating;

    TaskRunner* _runner;

    friend class TaskRunner;
};
#endif


} // namespace scy


#endif // SCY_Task_H
