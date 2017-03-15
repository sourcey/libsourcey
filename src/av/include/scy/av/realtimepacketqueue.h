///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup av
/// @{


#ifndef SCY_RealtimePacketQueue_H
#define SCY_RealtimePacketQueue_H


#include "scy/av/types.h"
#include "scy/base.h"
#include "scy/packetqueue.h"


namespace scy {
namespace av {


/// This class emits media packets based on their realtime pts value.
template <class PacketT> 
class RealtimePacketQueue : public AsyncPacketQueue<PacketT>
{
public:
    typedef AsyncPacketQueue<PacketT> BaseQueue;

    RealtimePacketQueue(int maxSize = 1024)
        : BaseQueue(maxSize)
    {
    }

    virtual ~RealtimePacketQueue() 
    {
    }

    // Add an item to the queue
    virtual void push(PacketT* item)
    {
        assert(dynamic_cast<PacketT*>(item));
        BaseQueue::push(item);
        //BaseQueue::sort<MediaPacketTimeCompare>();

        //std::sort(_queue.begin(), _queue.end(), 
        //    [](const PacketT* a, const PacketT* b)
        //        { return a->time < b->time; }
        //);
    }

    // Return the current duration from stream start in microseconds
    int64_t realTime() 
    { 
        return (time::hrtime() - _startTime) / 1000; 
    }

protected:
    virtual PacketT* popNext()
    {
        if (BaseQueue::empty())
            return nullptr;

        auto next = BaseQueue::front();
        InfoS(this) << "@@@@@@@@@@@ next " << next->time << " < " << realTime() << std::endl;
        if (next->time > realTime())
            return nullptr;
        BaseQueue::pop();

        InfoS(this) << "################################## popNext: " << BaseQueue::size() << ": " << realTime()
            << " > " << next->time << std::endl;
        return next;
    }

    virtual void onStreamStateChange(const PacketStreamState& state)
    {
        TraceS(this) << "Stream state: " << state << std::endl;

        if (state.equals(PacketStreamState::Active)) {
            _startTime = time::hrtime();
        }

        BaseQueue::onStreamStateChange(state);
    }

    struct MediaPacketTimeCompare
    {
        bool operator()(const MediaPacket* a, const MediaPacket* b) {
            return a->time < b->time;
        }
    };

    int64_t _startTime;
};


} // namespace av
} // namespace scy


#endif // SCY_RealtimePacketQueue_H


/// @\}
