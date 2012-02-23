#ifndef ANIONU_SPOT_IEnvironment_H
#define ANIONU_SPOT_IEnvironment_H


#include "Sourcey/Logger.h"


namespace Sourcey {


namespace Media {
class FormatRegistry;
}

namespace Spot {


class ISession;
class IConfiguration;
class ISynchronizer;
class IChannelManager;
class IModeRegistry;
class IStreamingManager;
class IStreamingSession;
class IMediaManager;


class IEnvironment 
{
public:
	IEnvironment() {};	
	virtual ~IEnvironment() {};
	
	virtual IConfiguration& config() /*const*/ = 0;
	virtual ISession& session() /*const*/ = 0;
	virtual ISynchronizer& synchronizer() /*const*/ = 0;
	virtual IChannelManager& channels() /*const*/ = 0;
	virtual IModeRegistry& modes() /*const*/ = 0;
	virtual IStreamingManager& streaming() /*const*/ = 0;
	virtual IMediaManager& media() /*const*/ = 0;
	virtual Logger& logger() /*const*/ = 0;

	virtual std::string version() const = 0;
	
	virtual void createEvent(const std::string& name, 
							 const std::string& message = "") = 0;
};


} } // namespace Sourcey::Spot


#endif // ANIONU_SPOT_IEnvironment_H