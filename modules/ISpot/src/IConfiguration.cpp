#include "Sourcey/Spot/IConfiguration.h"


using namespace std;
using namespace Poco;


namespace Sourcey { 
namespace Spot {


IConfiguration::IConfiguration(/*IEnvironment& env*/) //:
	//IModule(env)
{	
}


IConfiguration::~IConfiguration()
{
}


void IConfiguration::setRaw(const string& key, const string& value)
{	
	//log() << "Set Raw: " << key << ": " << value << endl;
	ConfigurationChanged.dispatch(this, key);
}


} } // namespace Sourcey::Spot