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


#ifndef SOURCEY_StreamManager_H
#define SOURCEY_StreamManager_H


#include "Sourcey/EventfulManager.h"
#include "Sourcey/PacketStream.h"


namespace Scy { 


class StreamManager: protected EventfulManager<std::string, PacketStream>, public IPolymorphic
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


} // namespace Scy


#endif // SOURCEY_StreamManager_H