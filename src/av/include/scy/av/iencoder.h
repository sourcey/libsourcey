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


#ifndef SCY_AV_IEncoder_H
#define SCY_AV_IEncoder_H


#include "scy/signal.h"
#include "scy/packetstream.h"
#include "scy/stateful.h"
#include "scy/av/format.h"


namespace scy {
namespace av {


struct EncoderState: public State
{
    enum Type
    {
        None = 0,
        Ready,
        Encoding,
        Stopped,
        Error
    };

    std::string str(unsigned int id) const
    {
        switch(id) {
        case None:            return "None";
        case Ready:            return "Ready";
        case Encoding:        return "Encoding";
        case Stopped:        return "Stopped";
        case Error:            return "Error";
        }
        return "undefined";
    };
};


struct EncoderOptions
{
    Format iformat;      // input media format.
    Format oformat;      // output media format.
    std::string ifile;   // input file path.
    std::string ofile;   // output file path.
    long duration;       // duration of time to record in milliseconds.
    EncoderOptions(const Format& iformat = Format(),
                   const Format& oformat = Format(),
                   const std::string& ifile = "",
                   const std::string& ofile = "",
                   long duration = 0) :
        iformat(iformat),
        oformat(oformat),
        ifile(ifile),
        ofile(ofile),
        duration(duration) {}
    virtual ~EncoderOptions() {};
};


class IEncoder: public Stateful<EncoderState>
    /// This is the abstract class for all encoders.
{
public:
    enum Type
    {
        None        = 0,    // huh?
        Video       = 1,    // video only
        Audio       = 2,    // audio only
        Multiplex   = 3     // both video & audio
    };

    virtual void initialize() = 0;
    virtual void uninitialize() = 0;

    virtual EncoderOptions& options() = 0;

    virtual bool isNone() const        { return stateEquals(EncoderState::None); };
    virtual bool isReady() const    { return stateEquals(EncoderState::Ready); };
    virtual bool isEncoding() const    { return stateEquals(EncoderState::Encoding); };
    virtual bool isActive() const    { return stateBetween(EncoderState::Ready, EncoderState::Encoding); };
    virtual bool isStopped() const    { return stateEquals(EncoderState::Stopped); };
    virtual bool isError() const    { return stateEquals(EncoderState::Error); };
};


typedef IEncoder IPacketEncoder;
    /// 0.8.x compatibility

/*
class IPacketEncoder: public IEncoder, public PacketProcessor
    /// This class extends the IEncoder interface to add
    /// PacketStream compatibility.
{
public:
    virtual void process(IPacket& packet) = 0;
        // Encodes the packet, and pushes it downstream.
};
*/


} // namespace av
} // namespace scy


#endif // SCY_AV_IEncoder_H
