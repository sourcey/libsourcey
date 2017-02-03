///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:    LGPL-2.1+
//
/// @addtogroup base
/// @{


#ifndef SCY_PacketQueue_H
#define SCY_PacketQueue_H


#include "scy/base.h"
#include "scy/packetstream.h"
#include "scy/queue.h"
#include "scy/synchronizer.h"


namespace scy {


//
// Synchronized Packet Queue
//


template <class T = IPacket>
class SCY_EXTERN SyncPacketQueue : public SyncQueue<T>, public PacketProcessor
{
public:
    typedef SyncQueue<T> Queue;
    typedef PacketProcessor Processor;

    SyncPacketQueue(uv::Loop* loop, int maxSize = 1024)
        : Queue(loop, maxSize)
        , Processor(this->emitter)
    {
    }

    SyncPacketQueue(int maxSize = 1024)
        : Queue(uv::defaultLoop(), maxSize)
        , Processor(this->emitter)
    {
    }

    virtual ~SyncPacketQueue() {}

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
    if (Queue::cancelled()) {
        WarnS(this) << "Process late packet" << std::endl;
        assert(0);
        return;
    }

    Queue::push(reinterpret_cast<T*>(packet.clone()));
}


/// Emit should never be called after closure.
/// Any late packets should have been dealt with
/// and dropped by the run() function.
template <class T> inline void SyncPacketQueue<T>::dispatch(T& packet)
{
    if (Queue::cancelled()) {
        WarnS(this) << "Dispatch late packet" << std::endl;
        assert(0);
        return;
    }

    Processor::emit(packet);
}


template <class T> inline bool SyncPacketQueue<T>::accepts(IPacket& packet)
{
    return dynamic_cast<T*>(&packet) != 0;
}


template <class T>
inline void
SyncPacketQueue<T>::onStreamStateChange(const PacketStreamState& state)
{
    TraceS(this) << "Stream state: " << state << std::endl;

    switch (state.id()) {
        // case PacketStreamState::None:
        // case PacketStreamState::Active:
        // case PacketStreamState::Resetting:
        // case PacketStreamState::Stopping:
        // case PacketStreamState::Stopped:
        case PacketStreamState::Closed:
        case PacketStreamState::Error:
            Queue::cancel();
            break;
    }
}


//
// Asynchronous Packet Queue
//


template <class T = IPacket>
class SCY_EXTERN AsyncPacketQueue : public AsyncQueue<T>, public PacketProcessor
{
public:
    typedef AsyncQueue<T> Queue;
    typedef PacketProcessor Processor;

    AsyncPacketQueue(int maxSize = 1024)
        : Queue(maxSize)
        , Processor(this->emitter)
    {
    }

    virtual ~AsyncPacketQueue() {}

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
    Queue::flush();
    assert(Queue::empty());
    Queue::cancel();
    Queue::_thread.join();
}


template <class T> inline void AsyncPacketQueue<T>::dispatch(T& packet)
{
    if (Queue::cancelled()) {
        WarnS(this) << "Dispatch late packet" << std::endl;
        assert(0);
        return;
    }

    Processor::emit(packet);
}


template <class T> inline void AsyncPacketQueue<T>::process(IPacket& packet)
{
    if (Queue::cancelled()) {
        WarnS(this) << "Process late packet" << std::endl;
        assert(0);
        return;
    }

    Queue::push(reinterpret_cast<T*>(packet.clone()));
}


template <class T> inline bool AsyncPacketQueue<T>::accepts(IPacket& packet)
{
    return dynamic_cast<T*>(&packet) != 0;
}


template <class T>
inline void
AsyncPacketQueue<T>::onStreamStateChange(const PacketStreamState& state)
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

            // case PacketStreamState::Resetting:
            // case PacketStreamState::None:
            // case PacketStreamState::Stopping:
    }
}

} // namespace scy


#endif // SCY_PacketQueue_H


/// @\}
