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


#ifndef SOURCEY_MEDIA_IEncoder_H
#define SOURCEY_MEDIA_IEncoder_H


#include "Sourcey/Signal.h"
#include "Sourcey/IPacketProcessor.h"
#include "Sourcey/Stateful.h"
#include "Sourcey/Media/Format.h"


namespace Scy {
namespace Media {


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
		case None:			return "None";
		case Ready:			return "Ready";
		case Encoding:		return "Encoding";
		case Stopped:		return "Stopped";
		case Error:			return "Error";
		}
		return "undefined"; 
	};
};


struct EncoderOptions
{
	Format iformat;		// The input media format.
	Format oformat;		// The output media format.
	EncoderOptions(const Format& iformat = Format(), 
				   const Format& oformat = Format()) :
		iformat(iformat),
		oformat(oformat) {}
	EncoderOptions(const EncoderOptions& r) : 
		iformat(r.iformat),
		oformat(r.oformat) {}
	virtual ~EncoderOptions() {};
};
	

struct RecordingOptions: public EncoderOptions
{
	std::string ifile;	// The input file path.
	std::string ofile;	// The output file path.
	long duration;		// The millisecond duration 
						// of time to record.
	RecordingOptions(const Format& iformat = Format(),
					const Format& oformat = Format(),
					const std::string& ifile = "",
					const std::string& ofile = "",
					long duration = 0) :
		EncoderOptions(iformat, oformat),
		ifile(ifile),
		ofile(ofile),
		duration(duration) {}
	RecordingOptions(const EncoderOptions& r) :
		EncoderOptions(r.iformat, r.oformat) {}
	RecordingOptions(const RecordingOptions& r) : 
		EncoderOptions(r.iformat, r.oformat),
		ifile(r.ifile),
		ofile(r.ofile),
		duration(r.duration) {}
	virtual ~RecordingOptions() {};
};


class IEncoder: public StatefulSignal<EncoderState>
	/// This is the abstract class for all instantiations
	/// of the IEncoder template.
{
public:
	enum Type 
	{
		None		= 0,	// huh?
		Video		= 1,	// video only
		Audio		= 2,	// audio only
		Multiplex	= 3		// both video & audio
	};
	
	virtual void initialize() = 0;
	virtual void uninitialize() = 0;	
	
	virtual EncoderOptions& options() = 0;

	virtual bool isNone() const		{ return stateEquals(EncoderState::None); };
	virtual bool isReady() const	{ return stateEquals(EncoderState::Ready); };
	virtual bool isEncoding() const	{ return stateEquals(EncoderState::Encoding); };
	virtual bool isActive() const	{ return stateBetween(EncoderState::Ready, EncoderState::Encoding); };
	virtual bool isStopped() const	{ return stateEquals(EncoderState::Stopped); };
	virtual bool isError() const	{ return stateEquals(EncoderState::Error); };
};


class IPacketEncoder: public IEncoder, public IPacketProcessor
	/// This class extends the IEncoder interface to add
	/// PacketStream compatibility.
{
public:
	virtual void process(IPacket& packet) = 0;
		// Encodes the packet, and pushes it downstream.
};


} // namespace Media
} // namespace Scy


#endif // SOURCEY_MEDIA_IEncoder_H