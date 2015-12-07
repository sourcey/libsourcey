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


#ifndef SCY_Thread_H
#define SCY_Thread_H


#include "scy/uv/uvpp.h"
#include "scy/mutex.h"
#include "scy/platform.h"
#include "scy/async.h"


namespace scy {
    

class Thread: public async::Runner
    /// This class implements a platform-independent
    /// wrapper around an operating system thread.
{
public:    
    typedef std::shared_ptr<Thread> ptr;

    Thread();
    Thread(async::Runnable& target);    
    Thread(std::function<void()> target);    
    Thread(std::function<void(void*)> target, void* arg);
    virtual ~Thread();
    
    void join();
        // Waits until the thread exits.
    
    bool waitForExit(int timeout = 5000);
        // Waits until the thread exits.
        // The thread should be cancelled beore calling this method.
        // This method must be called from outside the current thread
        // context or deadlock will ensue.
     
    unsigned long id() const;
        // Returns the native thread ID.
    
    static unsigned long currentID();
         // Returns the native thread ID of the current thread.

    static const unsigned long mainID;

protected:
    Thread(const Thread&);
    Thread& operator = (const Thread&);
    
    virtual bool async() const;
    virtual void startAsync(); 

    uv_thread_t _handle;
};


//
// Runner Startable
//


template <class TStartable>
class AsyncStartable: public TStartable
    /// Depreciated: This class is an invisible wrapper around a TStartable instance,
    /// which provides asynchronous access to the TStartable start() and
    /// stop() methods. TStartable is an instance of async::Startable.
{
public:
    AsyncStartable() {};
    virtual ~AsyncStartable() {};

    static void runAsync(void* arg) {
        try {
            // Call the blocking start() function once only
            static_cast<TStartable*>(arg)->start();
        }
        catch (std::exception& exc) {
            // errorL("AsyncStartable") << exc.what() << std::endl;
#ifdef _DEBUG
            throw exc;
#endif
        }
    }

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
    Thread _thread;
};


} // namespace scy


#endif