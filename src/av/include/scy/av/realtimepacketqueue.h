///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <https://sourcey.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @addtogroup av
/// @{


#ifndef SCY_RealtimePacketQueue_H
#define SCY_RealtimePacketQueue_H


#include "scy/av/packet.h"
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
    virtual void push(PacketT* item) override
    {
        BaseQueue::push(item);
        BaseQueue::template sort<MediaPacketTimeCompare>();
    }

    // Return the current duration from stream start in microseconds
    int64_t realTime()
    {
        return (time::hrtime() - _startTime) / 1000;
    }

protected:
    virtual PacketT* popNext() override
    {
        if (BaseQueue::empty())
            return nullptr;

        auto next = BaseQueue::front();
        if (next->time > realTime())
            return nullptr;
        BaseQueue::pop();

        STrace << "Pop next: " << BaseQueue::size() << ": "
            << realTime() << " > " << next->time << std::endl;
        return next;
    }

    virtual void onStreamStateChange(const PacketStreamState& state) override
    {
        LTrace("Stream state changed: ", state)

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
