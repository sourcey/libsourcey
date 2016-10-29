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


#ifndef SCY_PacketQueue_H
#define SCY_PacketQueue_H


#include "scy/base.h"
#include "scy/queue.h"
#include "scy/packetstream.h"
#include "scy/synccontext.h"


namespace scy {


//
// Synchronized Packet Queue
//


template <class T = IPacket>
class SyncPacketQueue: public SyncQueue<T>, public PacketProcessor
{
public:
    typedef SyncQueue<T> base_t;
    typedef PacketProcessor proc_t;

    SyncPacketQueue(uv::Loop* loop, int maxSize = 1024) :
        base_t(loop, maxSize),
        proc_t(this->emitter)
    {
    }

    SyncPacketQueue(int maxSize = 1024) :
        base_t(uv::defaultLoop(), maxSize),
        proc_t(this->emitter)
    {
    }

    virtual ~SyncPacketQueue()
    {
    }

    virtual void process(IPacket& packet);
    virtual bool accepts(IPacket& packet);

    PacketSignal emitter;

protected:
    virtual void dispatch(T& packet);

    virtual void onStreamStateChange(const PacketStreamState&);
};



// SyncPacketQueue::SyncPacketQueue(uv::Loop* loop, int maxSize) :
//     SyncQueue<IPacket>(loop, maxSize),
//     PacketProcessor(this->emitter)
// {
//     TraceS(this) << "Create" << std::endl;
// }
//
//
// SyncPacketQueue::SyncPacketQueue(int maxSize) :
//     SyncQueue<IPacket>(uv::defaultLoop(), maxSize),
//     PacketProcessor(this->emitter)
// {
//     TraceS(this) << "Create" << std::endl;
// }
//
//
// SyncPacketQueue::~SyncPacketQueue()
// {
//     TraceS(this) << "Destroy" << std::endl;
// }


template <class T> inline void SyncPacketQueue<T>::process(IPacket& packet)
{
    if (base_t::cancelled()) {
        WarnS(this) << "Process late packet" << std::endl;
        assert(0);
        return;
    }

    base_t::push(reinterpret_cast<T*>(packet.clone()));
}


template <class T> inline void SyncPacketQueue<T>::dispatch(T& packet)
{
    // Emit should never be called after closure.
    // Any late packets should have been dealt with
    // and dropped by the run() function.
    if (base_t::cancelled()) {
        WarnS(this) << "Dispatch late packet" << std::endl;
        assert(0);
        return;
    }

    proc_t::emit(packet);
}


template <class T> inline bool SyncPacketQueue<T>::accepts(IPacket& packet)
{
    return dynamic_cast<T*>(&packet) != 0;
}


template <class T> inline void SyncPacketQueue<T>::onStreamStateChange(const PacketStreamState& state)
{
    TraceS(this) << "Stream state: " << state << std::endl;

    switch (state.id()) {
    //case PacketStreamState::None:
    //case PacketStreamState::Active:
    //case PacketStreamState::Resetting:
    //case PacketStreamState::Stopping:
    //case PacketStreamState::Stopped:
    case PacketStreamState::Closed:
    case PacketStreamState::Error:
        base_t::cancel();
        break;
    }
}


//
// Asynchronous Packet Queue
//


template <class T = IPacket>
class AsyncPacketQueue: public AsyncQueue<T>, public PacketProcessor
{
public:
    typedef AsyncQueue<T> base_t;
    typedef PacketProcessor proc_t;

    AsyncPacketQueue(int maxSize = 1024) :
        base_t(maxSize),
        proc_t(this->emitter)
    {
    }

    virtual ~AsyncPacketQueue()
    {
    }

    virtual void close();

    virtual void process(IPacket& packet);
    virtual bool accepts(IPacket& packet);

    PacketSignal emitter;

protected:
    virtual void dispatch(T& packet);

    virtual void onStreamStateChange(const PacketStreamState&);
};


template <class T> inline void AsyncPacketQueue<T>::close()
{
    // Flush queued items, some protocols can't afford dropped packets
    base_t::flush();
    assert(base_t::empty());
    base_t::cancel();
    base_t::_thread.join();
}


template <class T> inline void AsyncPacketQueue<T>::dispatch(T& packet)
{
    if (base_t::cancelled()) {
        WarnS(this) << "Dispatch late packet" << std::endl;
        assert(0);
        return;
    }

    proc_t::emit(packet);
}


template <class T> inline void AsyncPacketQueue<T>::process(IPacket& packet)
{
    if (base_t::cancelled()) {
        WarnS(this) << "Process late packet" << std::endl;
        assert(0);
        return;
    }

    base_t::push(reinterpret_cast<T*>(packet.clone()));
}


template <class T> inline bool AsyncPacketQueue<T>::accepts(IPacket& packet)
{
    return dynamic_cast<T*>(&packet) != 0;
}


template <class T> inline void AsyncPacketQueue<T>::onStreamStateChange(const PacketStreamState& state)
{
    TraceS(this) << "Stream state: " << state << std::endl;

    switch (state.id()) {
    case PacketStreamState::Active:
        break;

    case PacketStreamState::Stopped:
        break;

    case PacketStreamState::Error:
    case PacketStreamState::Closed:
        close();
        break;

    //case PacketStreamState::Resetting:
    //case PacketStreamState::None:
    //case PacketStreamState::Stopping:
    }
}

} // namespace scy


#endif // SCY_PacketQueue_H
