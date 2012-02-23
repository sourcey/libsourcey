#include "Sourcey/Spot/IPlugin.h"
#include "Sourcey/Spot/IEnvironment.h"
#include "Sourcey/Spot/IEnvironment.h"


using namespace std;
using namespace Poco;


namespace Sourcey {
namespace Spot {


IPlugin::IPlugin() : 
	_env(NULL)
{
}


IPlugin::~IPlugin() 
{	
}


void IPlugin::initialize()
{
}


void IPlugin::uninitialize()
{
}


IEnvironment& IPlugin::env() const 
{ 
	return *_env; 
}

	
void IPlugin::setEnvironment(IEnvironment* env) 
{ 
	_env = env; 
}


std::string IPlugin::path() const 
{ 
	return _path;
}


void IPlugin::setPath(const std::string& path) 
{ 
	_path = path; 
}


LogStream IPlugin::log(const char* level) const 
{ 
	// The Environment instance will be NULL until
	// after the constructor has been called.
	if (_env == NULL)
		return LogStream();

	LogStream stream(_env->logger().send(level));
	printLog(stream);
	return stream;
}


void IPlugin::printLog(LogStream& ost) const
{
	ost << "["
		<< name()
		<< ":"
		<< this
		<< "] ";
}


} } // namespace Sourcey::Spot