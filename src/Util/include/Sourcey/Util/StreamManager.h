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


#include "Sourcey/Manager.h"
#include "Sourcey/PacketStream.h"


namespace scy { 
	

typedef EventedManager<
	std::string, PacketStream, DestroyMethodDeleter<PacketStream>
> StreamManagerBase;


class StreamManager: public StreamManagerBase, public IPolymorphic
{
public:
	typedef StreamManagerBase Manager;
	typedef Manager::Map	  Map;

public:
	StreamManager(bool freeClosedStreams = true);
	virtual ~StreamManager();

	virtual bool addStream(PacketStream* stream, bool whiny = true);	
	virtual bool closeStream(const std::string& name, bool whiny = true);	
	virtual void closeAll();	
	virtual PacketStream* getStream(const std::string& name, bool whiny = true);
    virtual PacketStream* getDafaultStream();
		/// Returns the first stream in the list, or NULL.

	virtual Map streams() const;

	virtual void print(std::ostream& os) const;

	virtual const char* className() const { return "StreamManager"; }

protected:	
	virtual void onAdd(PacketStream* task);
		/// Called after a stream is added.

	virtual void onRemove(PacketStream* task);
		/// Called after a stream is removed.

	virtual void onStreamStateChange(void* sender, PacketStreamState& state, const PacketStreamState&);	
	
protected:	
	bool _freeClosedStreams;
};


} // namespace scy


#endif // SOURCEY_StreamManager_H