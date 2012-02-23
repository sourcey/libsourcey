#ifndef ANIONU_SPOT_IConfiguration_H
#define ANIONU_SPOT_IConfiguration_H


#include "Sourcey/Signal.h"
#include "Sourcey/IConfiguration.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Symple/Form.h"


namespace Sourcey { 
namespace Spot {


class IConfiguration: public Sourcey::IConfiguration, public ILoggable
{
public:
	IConfiguration();
	virtual ~IConfiguration();

	Signal2<const std::string&, const std::string&> ConfigurationChanged;

	virtual const char* className() const { return "Configuration"; }

protected:
	virtual void setRaw(const std::string& key, const std::string& value);
		// The base method gets called after the config value is
		// updated to trigger the ConfigurationChanged callback.
};


} } // namespace Sourcey::Spot


#endif