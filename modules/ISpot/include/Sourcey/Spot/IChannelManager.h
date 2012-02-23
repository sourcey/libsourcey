#ifndef ANIONU_SPOT_IChannelManager_H
#define ANIONU_SPOT_IChannelManager_H


#include "Sourcey/EventfulManager.h"
#include "Sourcey/Spot/IChannel.h"
#include "Sourcey/Logger.h"


namespace Sourcey { 
namespace Spot {


class IChannelManager: protected EventfulManager<std::string, IChannel>, public ILoggable
{
public:
	typedef EventfulManager<std::string, IChannel>	Manager;
	typedef Manager::Map							Map;
	
public:
	IChannelManager();
	virtual ~IChannelManager();

	virtual IChannel* getChannel(const std::string& name);
		// Returns the IChannel instance or throws a
		// NotFoundException exception.

    virtual IChannel* getDafaultChannel();
		// Returns the first channel in the list.

	Map& items();
	
	virtual const char* className() const { return "ChannelManager"; }

protected:
	bool addChannel(IChannel* channel);
	bool removeChannel(const std::string& name);	
};


} } // namespace Sourcey::Spot


#endif // ANIONU_SPOT_IChannelManager_H