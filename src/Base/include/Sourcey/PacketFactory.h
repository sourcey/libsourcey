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


#ifndef SOURCEY_PacketFactory_H
#define SOURCEY_PacketFactory_H


#include "Sourcey/PacketEmitter.h"
#include "Sourcey/Buffer.h"

//#include "Poco/Thread.h"


namespace scy {

	
struct IPacketCreationStrategy
{
	virtual IPacket* create(Buffer& buffer) const = 0;	
	virtual int priority() const = 0; // 0 - 100

	static bool compareProiroty(const IPacketCreationStrategy* l, const IPacketCreationStrategy* r) {
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
		util::ClearVector(_types);
	}

	template <class PacketT>
	void registerPacketType(int priority) {
		unregisterPacketType<PacketT>(); // ensure unique values
		//Poco::FastMutex::ScopedLock lock(_mutex);
		_types.push_back(new PacketCreationStrategy<PacketT>(priority));
		sort(_types.begin(), _types.end(), IPacketCreationStrategy::compareProiroty);
	}

	template <class PacketT>
	void unregisterPacketType() {
		//Poco::FastMutex::ScopedLock lock(_mutex);		
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
		//Poco::FastMutex::ScopedLock lock(_mutex);
		_types.push_back(new StrategyT(priority));
		sort(_types.begin(), _types.end(), IPacketCreationStrategy::compareProiroty);
	}

	template <class StrategyT>
	void unregisterStrategy() {
		//Poco::FastMutex::ScopedLock lock(_mutex);		
		for (typename PacketCreationStrategyList::iterator it = _types.begin(); it != _types.end(); ++it) {
			if (dynamic_cast<StrategyT*>(*it) != 0) {
				delete *it;
				_types.erase(it);
				return;
			}
		}
	}

	PacketCreationStrategyList& types() {
		//Poco::FastMutex::ScopedLock lock(_mutex);		
		return _types;
	}

	PacketCreationStrategyList types() const {
		//Poco::FastMutex::ScopedLock lock(_mutex);		
		return _types;
	}

protected:
	PacketCreationStrategyList _types;
	//mutable Poco::FastMutex	_mutex;
};


// ---------------------------------------------------------------------
//
struct PacketFactory: public PacketRegistry
{
	virtual bool onPacketCreated(IPacket*) {
		// returning false will stop packet propagation
		return true;
	}

	virtual IPacket* createPacket(Buffer& buffer) {
		//Poco::FastMutex::ScopedLock lock(_mutex);
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
			buffer.position(offset);
		}
		return NULL;
	}
};


} // namespace scy


#endif // SOURCEY_PacketFactory_H