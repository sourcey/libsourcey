#ifndef ANIONU_SPOT_IMode_H
#define ANIONU_SPOT_IMode_H


#include "Sourcey/Base.h"
#include "Sourcey/Stateful.h"
#include "Sourcey/IRunnable.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Spot/IConfigurable.h"
#include "Sourcey/Symple/Form.h"

#include "Poco/Net/NameValueCollection.h"

#include <map>
#include <list>
#include <iostream>


namespace Sourcey {
namespace Spot {


class IMode;
class IChannel;
class IEnvironment;


class ModeConfiguration 
	/// ModeConfiguration enables modes to access config values at
	/// their channel specific namespace falling back to mode defaults.
	/// Default: channels.[name].modes.[name].[value]
	/// Root: modes.[name].[value]
{	
public:
	ModeConfiguration(IMode& mode);
		
	//void setup();

	std::string getString(const std::string& key, const std::string& defaultValue) const;
	int getInt(const std::string& key, int defaultValue) const;
	double getDouble(const std::string& key, double defaultValue) const;
	bool getBool(const std::string& key, bool defaultValue) const;

	void setString(const std::string& key, const std::string& value, bool useBase = false);
	void setInt(const std::string& key, int value, bool useBase = false);
	void setDouble(const std::string& key, double value, bool useBase = false);
	void setBool(const std::string& key, bool value, bool useBase = false);
	
	std::string getDefaultScope(const std::string& key) const;
	std::string getBaseScope(const std::string& key) const;
	std::string getScoped(const std::string& key, bool useBase = false) const;

	IMode& mode;
	std::string defaultScope;
	std::string baseScope;
};


typedef Poco::Net::NameValueCollection ModeOptions;
typedef std::map<std::string, IMode*> IModeMap;


struct ModeState: public StateT 
{
	enum Type
	{
		None = 0,
		Disabled,
		Enabled,
		Failed
	};

	std::string str(unsigned int id) const 
	{ 
		switch (id) {
		case None:		return "None";
		case Disabled:	return "Disabled";
		case Enabled:	return "Enabled";
		case Failed:	return "Failed";
		}
		return "undefined"; 
	};
};	


class IMode: public ILoggable, public IConfigurable, public StatefulSignal<ModeState>
	/// This class defines a specific operational mode for a
	/// Spot Channel.
{	
public:
	IMode(IEnvironment& env, IChannel& channel, const std::string& name);	
	virtual ~IMode();
	
	virtual void initialize();
	virtual void uninitialize();
		// If unrecoverable errors are enountered
		// during the initialization process, the
		// mode state should be set to Failed and
		// an Exception thrown.

	virtual void enable();
	virtual void disable();

	virtual std::string name() const		{ return _name; };
	virtual IEnvironment& env() 			{ return _env; };
	virtual IChannel& channel() 			{ return _channel; };
	virtual ModeConfiguration& config() 	{ return _config; };
	virtual ModeOptions& options()  		{ return _options; };
	
	virtual bool isDisabled() const { return stateEquals(ModeState::Disabled); };
	virtual bool isEnabled() const { return stateEquals(ModeState::Enabled); };
	virtual bool isFailed() const { return stateEquals(ModeState::Failed); };

	virtual void setData(const std::string& name, const std::string& value);
	virtual void removeVar(const std::string& name);
	virtual void clearData();
	virtual StringMap data() const { return _data; };
	
	Signal<const StringMap&> ModeDataChanged;
	
	virtual const char* className() const { return "Mode"; }

protected:
	IEnvironment&		_env;	
	IChannel&			_channel;
	ModeConfiguration	_config;
	ModeOptions			_options;
	std::string			_name;
	StringMap			_data;

	//friend class ModeConfiguration;
}; 


} } // namespace Sourcey::Spot


#endif // ANIONU_SPOT_IMode_H