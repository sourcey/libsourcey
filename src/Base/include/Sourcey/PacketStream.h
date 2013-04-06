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


#ifndef SOURCEY_PacketStream_H
#define SOURCEY_PacketStream_H


#include "Sourcey/Logger.h"
#include "Sourcey/Stateful.h"
#include "Sourcey/IStartable.h"
#include "Sourcey/PacketDispatcher.h"
#include "Sourcey/IPacketProcessor.h"

#include "Poco/Mutex.h"
#include "Poco/Event.h"


namespace Sourcey {


struct PacketAdapterReference
	/// Provides a reference to a PacketDispatcher instance.
{
	PacketDispatcher* ptr;
	int order;
	bool freePointer;	
	bool syncState;

	PacketAdapterReference(PacketDispatcher* ptr = NULL, int order = 0, 
		bool freePointer = true, bool syncState = false) : 
		ptr(ptr), order(order), freePointer(freePointer), 
		syncState(syncState) {}
		
	static bool CompareOrder(const PacketAdapterReference& l, const PacketAdapterReference& r) 
	{
		return l.order < r.order;
	}
};


typedef std::vector<PacketAdapterReference> PacketAdapterList;


struct PacketStreamState: public State 
{
	enum Type 
	{
		None = 0,
		Running,
		Stopped,
		Resetting,
		Closing,
		Closed,
		Failed,
	};

	std::string str(unsigned int id) const 
	{ 
		switch(id) {
		case None:				return "None";
		case Running:			return "Running";
		case Stopped:			return "Stopped";
		case Resetting:			return "Resetting";
		case Closing:			return "Closing";
		case Closed:			return "Closed";
		case Failed:			return "Failed";
		default:				assert(false);
		}
		return "undefined"; 
	}
};


class PacketStream: public PacketDispatcher, public StatefulSignal<PacketStreamState>, public IStartable
	/// This class provides an interface for processing packets.
	/// A packet stream consists of a single PacketDispatcher,
	/// one or many IPacketProcessor instances, and one or many 
	/// callback receivers.
{ 
public:	
	PacketStream(const std::string& name = ""); 
	virtual ~PacketStream();
	
	virtual void start();
	virtual void stop();

	virtual void reset();
		/// Resets the internal state of all packet dispatchers
		/// in the stream. This is useful for correcting timestamp 
		/// and metadata generation in some cases.

	virtual void close();
		/// Closes the stream and transitions the internal state
		/// to Disconnected. This method is called by the destructor.
	
	virtual void attach(PacketDispatcher* source, bool freePointer = true, bool syncState = false);
	virtual void detach(PacketDispatcher* source);

	virtual void attach(IPacketProcessor* proc, int order = 0, bool freePointer = true);
	virtual void detach(IPacketProcessor* proc);
	
	virtual void attach(const PacketDelegateBase& delegate);
	virtual bool detach(const PacketDelegateBase& delegate);
	
	virtual std::string name() const;
		/// Returns the name of the stream.
	
	virtual bool isRunning() const;
		/// Returns true is the stream is in the Running state.
	
	virtual bool waitForReady();
		/// Locks until the current process queue iteration is complete.
		/// For for safe access after calling reset() or stop().

	virtual int numSources() const;
	virtual int numProcessors() const;
	virtual int numAdapters() const;

	PacketAdapterList adapters() const;
	
	virtual void setClientData(void* data);
	virtual void* clientData() const;
		/// Provides the ability to associate a non managed
		/// arbitrary client data pointer with the stream.
	
	template <class AdapterT>
	AdapterT* getSource(int index = 0)
	{
		int x = 0;
		for (unsigned i = 0; i < _sources.size(); i++) {
			AdapterT* source = dynamic_cast<AdapterT*>(_sources[i].ptr);
			if (source) {
				if (index == x)
					return source;
				else x++;
			}
		}
		return NULL;
	}

	template <class AdapterT>
	AdapterT* getProcessor(int index = 0)
	{
		int x = 0;
		for (unsigned i = 0; i < _processors.size(); i++) {
			AdapterT* processor = dynamic_cast<AdapterT*>(_processors[i].ptr);
			if (processor) {
				if (index == x)
					return processor;
				else x++;
			}
		}
		return NULL;
	}

protected:
	virtual void detachAll();
		/// Detaches all stream adapters, and frees 
		/// pointers if the adapters are managed.
	
	virtual void onSourcePacket(void* sender, IPacket& packet);
	virtual void onDispatchPacket(void* sender, IPacket& packet);
	virtual void onStateChange(PacketStreamState& state, const PacketStreamState& oldState);

protected:
	mutable Poco::FastMutex	_mutex;

	std::string _name;
	Poco::Event _ready;
	PacketAdapterList _sources;
	PacketAdapterList _processors;
	void* _clientData;
};


} // namespace Sourcey


#endif // SOURCEY_PacketStream_H