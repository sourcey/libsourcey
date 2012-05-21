//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is is distributed under a dual license that allows free, 
// open source use and closed source use under a standard commercial
// license.
//
// Non-Commercial Use:
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// Commercial Use:
// Please contact mail@sourcey.com
//


#ifndef SOURCEY_MEDIA_IEncoder_H
#define SOURCEY_MEDIA_IEncoder_H


#include "Sourcey/Signal.h"
#include "Sourcey/IPacketProcessor.h"
#include "Sourcey/Stateful.h"
#include "Sourcey/Media/Format.h"


namespace Sourcey {
namespace Media {


struct EncoderState: public StateT 
{
	enum Type 
	{
		None = 0,
		Ready,
		Encoding,
		Stopped,
		Failed
	};

	std::string str(unsigned int id) const 
	{ 	
		switch(id) {
		case None:			return "None";
		case Ready:			return "Ready";
		case Encoding:		return "Encoding";
		case Stopped:		return "Stopped";
		case Failed:		return "Failed";
		}
		return "undefined"; 
	};
};


struct EncoderParams
{
	Format iformat;		// The input media format.
	Format oformat;		// The output media format.
	EncoderParams(const Format& iformat = Format(), 
				  const Format& oformat = Format()) :
		iformat(iformat),
		oformat(oformat) {}
	EncoderParams(const EncoderParams& r) : 
		iformat(r.iformat),
		oformat(r.oformat) {}
	virtual ~EncoderParams() {};
};
	

struct RecorderParams: public EncoderParams
{
	std::string ifile;	// The input file path.
	std::string ofile;	// The output file path.
	time_t stopAt;		// Stop at a certain time.
	RecorderParams(const Format& iformat = Format(),
					  const Format& oformat = Format(),
					  const std::string& ifile = "",
					  const std::string& ofile = "",
					  time_t stopAt = 0) :
		EncoderParams(iformat, oformat),
		ifile(ifile),
		ofile(ofile),
		stopAt(stopAt) {}
	RecorderParams(const EncoderParams& r) :
		EncoderParams(r.iformat, r.oformat) {}
	RecorderParams(const RecorderParams& r) : 
		EncoderParams(r.iformat, r.oformat),
		ifile(r.ifile),
		ofile(r.ofile),
		stopAt(r.stopAt) {}
	virtual ~RecorderParams() {};
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
	
	virtual EncoderParams& params() = 0;

	virtual bool isNone() const		{ return stateEquals(EncoderState::None); };
	virtual bool isReady() const	{ return stateBetween(EncoderState::Ready, EncoderState::Encoding); };
	virtual bool isEncoding() const	{ return stateEquals(EncoderState::Encoding); };
	virtual bool isStopped() const	{ return stateEquals(EncoderState::Stopped); };
	virtual bool isFailed() const	{ return stateEquals(EncoderState::Failed); };
};


class IPacketEncoder: public IEncoder, public IPacketProcessor
	/// This is the abstract class for all instantiations
	/// of the IPacketEncoder template.
{
public:
	virtual void process(IPacket& packet) = 0;
		// Encodes the packet, and pushes it on downstream.
};


} // namespace Media
} // namespace Sourcey


#endif // SOURCEY_MEDIA_IEncoder_H