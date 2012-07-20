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


#ifndef SOURCEY_StreamManager_H
#define SOURCEY_StreamManager_H


#include "Sourcey/EventfulManager.h"
#include "Sourcey/PacketStream.h"


namespace Sourcey { 


class StreamManager: protected EventfulManager<std::string, PacketStream>, public ILoggable
{
public:
	typedef EventfulManager<std::string, PacketStream>	Manager;
	typedef Manager::Base::Map							Map;

public:
	StreamManager(bool freeClosedStreams = true);
	virtual ~StreamManager();

	virtual PacketStream* getStream(const std::string& name, bool whiny = false);
		/// Returns the PacketStream instance or throws a
		/// NotFoundException exception.

    virtual PacketStream* getDafaultStream();
		/// Returns the first stream in the list.

	virtual void onStreamStateChange(void* sender, PacketStreamState& state, const PacketStreamState&);	

	virtual Map streams() const;
	
	virtual void print(std::ostream& os) const;

	virtual const char* className() const { return "StreamManager"; }

protected:
	virtual bool addStream(PacketStream* stream, bool whiny = false);
	virtual bool closeStream(const std::string& name, bool whiny = false);	
	virtual void closeAll();	

	bool _freeClosedStreams;
};


} // namespace Sourcey


#endif // SOURCEY_StreamManager_H