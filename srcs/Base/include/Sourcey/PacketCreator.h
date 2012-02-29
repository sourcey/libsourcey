//
// This software is copyright by Sourcey <mail@sourcey.com> and is distributed under a dual license:
// Copyright (C) 2005 Sourcey
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


#ifndef SOURCEY_PacketCreator_H
#define SOURCEY_PacketCreator_H


#include "Sourcey/PacketDispatcher.h"
#include "Sourcey/Buffer.h"

#include "Poco/Thread.h"


namespace Sourcey {

	
struct IPacketCreationStrategy
{
	virtual IPacket* create(Buffer& buffer) const = 0;	
	virtual int priority() const = 0; // 0 - 100

	static bool CompareProiroty(const IPacketCreationStrategy* l, const IPacketCreationStrategy* r) {
		return l->priority() > r->priority();
	}
};


typedef std::vector<IPacketCreationStrategy*> PacketCreationStrategyList;


// ---------------------------------------------------------------------
//
template <class PacketT>
struct PacketCreationStrategy: public IPacketCreationStrategy
	/// This template class implements an adapter that sits between
	/// an SignalBase and an object receiving notifications from it.
{
	PacketCreationStrategy(int priority = 0) : 
		_priority(priority) {
		assert(_priority <= 100);
	}

	virtual IPacket* create(Buffer& buffer) const {
		PacketT* packet = new PacketT;
		if (packet->read(buffer))
			return packet;
		delete packet;
		return NULL;
	};

	virtual int priority() const { return _priority; };	

protected:
	int _priority;
};


// ---------------------------------------------------------------------
//
struct PacketRegistry
{
	PacketRegistry() {}
	PacketRegistry(const PacketRegistry& r) : 
		_types(r._types) {};		
	virtual ~PacketRegistry() {
		Util::ClearVector(_types);
	}

	template <class PacketT>
	void registerPacketType(int priority) {
		unregisterPacketType<PacketT>(); // ensure unique values
		Poco::FastMutex::ScopedLock lock(_mutex);
		_types.push_back(new PacketCreationStrategy<PacketT>(priority));
		sort(_types.begin(), _types.end(), IPacketCreationStrategy::CompareProiroty);
	}

	template <class PacketT>
	void unregisterPacketType() {
		Poco::FastMutex::ScopedLock lock(_mutex);		
		for (typename PacketCreationStrategyList::iterator it = _types.begin(); it != _types.end(); ++it) {
			if (dynamic_cast<PacketCreationStrategy<PacketT>*>(*it) != 0) {
				delete *it;
				_types.erase(it);
				return;
			}
		}
	}

	template <class StrategyT>
	void registerStrategy(int priority) {
		unregisterStrategy<StrategyT>(); // ensure unique values
		Poco::FastMutex::ScopedLock lock(_mutex);
		_types.push_back(new StrategyT(priority));
		sort(_types.begin(), _types.end(), IPacketCreationStrategy::CompareProiroty);
	}

	template <class StrategyT>
	void unregisterStrategy() {
		Poco::FastMutex::ScopedLock lock(_mutex);		
		for (typename PacketCreationStrategyList::iterator it = _types.begin(); it != _types.end(); ++it) {
			if (dynamic_cast<StrategyT*>(*it) != 0) {
				delete *it;
				_types.erase(it);
				return;
			}
		}
	}

protected:
	PacketCreationStrategyList _types;
	mutable Poco::FastMutex	_mutex;
};


// ---------------------------------------------------------------------
//
struct PacketCreator: public PacketRegistry
{
	virtual bool onPacketCreated(IPacket*) {
		// returning false will stop packet propagation
		return true;
	}

	virtual IPacket* createPacket(Buffer& buffer) {
		Poco::FastMutex::ScopedLock lock(_mutex);
		assert(!_types.empty() && "no packet types registered");
		size_t offset = buffer.position();
		for (unsigned i = 0; i < _types.size(); i++) {
			IPacket* packet = _types[i]->create(buffer);
			if (packet) {
				if (!onPacketCreated(packet)) {
					delete packet;
					continue;
				}
				return packet;
			}
			buffer.setPosition(offset);
		}
		return NULL;
	}
};


} // namespace Sourcey


#endif // SOURCEY_PacketCreator_H