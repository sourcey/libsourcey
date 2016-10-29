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


#ifndef SCY_RealtimePacketQueue_H
#define SCY_RealtimePacketQueue_H


#include "scy/base.h"
#include "scy/packetqueue.h"
// #include "scy/synccontext.h"
#include "scy/av/types.h"


namespace scy {
namespace av {


// struct MediaPacketTimeCompare
// {
//     bool operator()(const MediaPacket* a, const MediaPacket* b) {
//         return a->time > b->time;
//     }
// };


template <class PacketT>
class RealtimePacketQueue: public AsyncPacketQueue<PacketT>
    /// This class emits media packets based on their realtime pts value.
{
public:
    typedef std::shared_ptr<RealtimePacketQueue> ptr_t;
    typedef AsyncPacketQueue<PacketT> base_t;

    RealtimePacketQueue(int maxSize = 1024) :
        base_t(maxSize)
    {
    }

    virtual ~RealtimePacketQueue()
    {
    }

    std::int64_t realTime()
        // Return the current duration from stream start in microseconds
    {
        return (time::hrtime() - _startTime) / 1000;
    }

protected:
    virtual PacketT* popNext()
    {
        if (base_t::empty())
            return nullptr;

        auto next = base_t::front();
        // WarnS(this) << "popNext: " << base_t::size() << ": " << realTime() << " < " <<  next->time << std::endl;

        if (next->time > realTime())
            return nullptr;
        base_t::pop();

        WarnS(this) << "popNext: " << base_t::size() << ": " << realTime() << " > " <<  next->time << std::endl;
        return next;
    }

    virtual void onStreamStateChange(const PacketStreamState& state)
    {
        TraceS(this) << "Stream state: " << state << std::endl;

        if (state.equals(PacketStreamState::Active)) {
            _startTime = time::hrtime();
        }

        base_t::onStreamStateChange(state);
    }

    std::int64_t _startTime;
};


} } // namespace scy::av


#endif // SCY_RealtimePacketQueue_H
