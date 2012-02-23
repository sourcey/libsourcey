#ifndef ANIONU_SPOT_IPlugin_H
#define ANIONU_SPOT_IPlugin_H


#include "Sourcey/Stateful.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Spot/IEnvironment.h"
#include "Sourcey/Spot/IConfigurable.h"

#include <string>


namespace Sourcey {
namespace Spot {

	
//class IEnvironment;


struct PluginState: public StateT 
{
	enum Type
	{
		None = 0,
		Disabled,
		Enabled,
		Error
	};

	std::string str(unsigned int id) const 
	{ 
		switch (id) {
		case None:		return "None";
		case Disabled:	return "Disabled";
		case Enabled:	return "Enabled";
		case Error:		return "Error";
		}
		return "undefined"; 
	};
};	


class IPlugin: public IConfigurable, public Stateful<PluginState> //public ILoggable, 
{
public:
	IPlugin();
	virtual ~IPlugin();
	
	virtual void initialize();
	virtual void uninitialize();
		// If unrecoverable errors are enountered during
		// the initialization process, the internal state
		// should be set to Error, and an Exception thrown.

	virtual void enable() = 0;
	virtual void disable() = 0;
	
	virtual std::string name() const = 0;
	virtual std::string title() const = 0;
	virtual std::string author() const = 0;
	virtual std::string version() const = 0;

	virtual IEnvironment& env() const;
	virtual void setEnvironment(IEnvironment* env);
		// An IEnvironment instance will be set by the 
		// application before plugin initialization.

	virtual std::string path() const;
	virtual void setPath(const std::string& path);
		// The plugin's system path will be set by the
		// application before plugin initialization.
		
	virtual LogStream log(const char* level = "debug") const;
	virtual void printLog(LogStream& ost) const;

	virtual bool isDisabled() const { return stateEquals(PluginState::Disabled); };
	virtual bool isEnabled() const { return stateEquals(PluginState::Enabled); };
	virtual bool isError() const { return stateEquals(PluginState::Error); };

protected: 
	IEnvironment* _env;
	std::string _path;
};


} } // namespace Sourcey::Spot


#endif // ANIONU_SPOT_IPlugin_H


	/*//, public IRunnable
	enum State 
	{
		None = 0,
		//Initialized,
		Disabled,
		Enabled,
		Error,
	}; : _state(None)
	*/
	//State _state;

	//virtual State state() const { return _state; };
	//virtual void setState(const State& state) { _state = state; };
	
	//virtual bool isInitialized() const { return _state == Initialized; };
	//virtual bool isEnabled() const { return _state == Enabled; };
	//virtual bool isError() const { return _state == Error; };