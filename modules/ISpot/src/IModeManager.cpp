#include "Sourcey/Spot/IModeManager.h"
#include "Sourcey/Spot/IChannel.h"


using namespace std;
using namespace Poco;


namespace Sourcey {
namespace Spot {


IModeManager::IModeManager(IChannel& channel) :
	//IModule(channel.env()), 
	_channel(channel)
{
}


IModeManager::~IModeManager() 
{	
}


} } // namespace Sourcey::Spot