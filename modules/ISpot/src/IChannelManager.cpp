#include "Sourcey/Spot/IChannelManager.h"


using namespace std; 


namespace Sourcey {
namespace Spot {


IChannelManager::IChannelManager()
{	
}


IChannelManager::~IChannelManager() 
{
}


bool IChannelManager::addChannel(IChannel* channel) 
{
	assert(channel);
	assert(!channel->name().empty());
	
	log() << "Adding Channel: " << channel->name() << endl;	
	return Manager::add(channel->name(), channel);
}


bool IChannelManager::removeChannel(const string& name) 
{
	assert(!name.empty());

	log() << "Removing Channel: " << name << endl;		
	return Manager::free(name);
}


IChannel* IChannelManager::getChannel(const string& name) 
{
	return Manager::get(name);
}


IChannel* IChannelManager::getDafaultChannel()
{
	if (!_items.empty()) {
		IChannelMap::const_iterator it = _items.begin();
		return it->second;
	}

	return NULL;
}


IChannelManager::Map& IChannelManager::items() 
{ 
	Poco::FastMutex::ScopedLock lock(_mutex); 
	return _items;
}


	/*	
using namespace Poco;
	if (!_formats.empty()) {
		std::vector<Format>::iterator it = _formats.begin();
		return *it;
	}

IChannel* IChannelManager::get(const string& name, bool whiny) const 
{
	//FastMutex::ScopedLock lock(_mutex); 
	IChannelMap::const_iterator it = _items.find(name);	
	if (it != _items.end()) {
		return it->second;	 
	} else if (whiny) {
		throw NotFoundException("Invalid channel requested: " + name);
	}

	log("warn") << "Invalid channel requested: " << name << endl;
	return NULL;
}

void IChannelManager::remove(const string& name) 
{
	log() << "Removing: " << name << endl;	

	IChannelMap::iterator it = _items.find(name);	
	//assert(it != _items.end());
	if (it != _items.end()) {

		it->second->ChannelChanged -= delegate(this, &IChannelManager::onChannelChanged);
		delete it->second;	 
		_items.erase(it);

		string rName = name;
		ItemRemoved.dispatch(this, rName);
	}
}

	//log() << "[Spot::ManagedTManager] Removing: " << name << endl;	
		ManagedT* toRemove = get(name, false);
		if (!toRemove)
			return false;
	
		toRemove->ManagedTChanged -= delegate(this, &ManagedTManager::onManagedTChanged);

		ManagedTRemoved.dispatch(this, toRemove);

		return BasicManager<std::string, ManagedT>::remove(name);


bool IChannelManager::exists(const string& name) const 
{ 
	log() << "exists()" << endl;
	//FastMutex::ScopedLock lock(_mutex); 
	IChannelMap::const_iterator it = _items.find(name);	
	return it != _items.end();	 
}


IChannelMap IChannelManager::map() const 
{
	log() << "map()" << endl;
	//FastMutex::ScopedLock lock(_mutex); 
	return _items;
}

	*/


/*
string IChannelManager::toString() const 
{
	log() << "toString()" << endl;
	ostringstream ost;
	ost << "IChannelManager[";
	for (IChannelMap::const_iterator it = _items.begin(); it != _items.end(); ++it) {	
		ost << "\n\t" << it->first << ": " << it->second->toString();
	}
	ost << "\n]";
	return ost.str();
}


void IChannelManager::toXML(ostream& ost)
{	
	log() << "toXML()" << endl;
	//FastMutex::ScopedLock lock(_mutex); 
	ost << "<channels>";	
	for (IChannelMap::const_iterator it = _items.begin(); it != _items.end(); ++it) {	
		it->second->toXML(ost);
	}
	ost << "</channels>";
}
*/


} } // namespace Sourcey::Spot