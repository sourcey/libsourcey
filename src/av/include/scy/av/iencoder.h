///
//
// LibSourcey
// Copyright (c) 2005, Sourcey <http://sourcey.com>
//
// SPDX-License-Identifier:	LGPL-2.1+
//
/// @addtogroup av
/// @{


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
    long duration;       // duration of time to record in nanoseconds.
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

/// This is the abstract class for all encoders.
class IEncoder: public Stateful<EncoderState>
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


typedef IEncoder IPacketEncoder;/// 0.8.x compatibility

/*/// This class extends the IEncoder interface to add
/// PacketStream compatibility.
class IPacketEncoder: public IEncoder, public PacketProcessor
{
public:    
    /// Encodes the packet, and pushes it downstream.
    virtual void process(IPacket& packet) = 0;

};
*/


} // namespace av
} // namespace scy


#endif // SCY_AV_IEncoder_H

/// @\}
