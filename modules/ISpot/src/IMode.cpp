#include "Sourcey/Spot/IMode.h"
#include "Sourcey/Spot/IChannel.h"
#include "Sourcey/Spot/IEnvironment.h"
#include "Sourcey/Spot/IConfiguration.h"
#include "Sourcey/Symple/Form.h"
#include "Sourcey/Logger.h"
#include "Poco/Format.h"


using namespace std;
using namespace Poco;


namespace Sourcey {
namespace Spot {


IMode::IMode(IEnvironment& env, IChannel& channel, const string& name) : 
	//IModule(channel.env()), 
	_env(env), 
	_channel(channel), 
	_name(name), 
	_config(*this)
{
	assert(!_name.empty());

	//
	// Default Scope: channels.[name].modes.[name].[value]
	//
	_config.defaultScope = format("channels.%s.modes.%s.", _channel.name(), _name);
	
	//
	// Base Scope: modes.[name].[value]
	//
	_config.baseScope = format("modes.%s.", _name);
}


IMode::~IMode()
{
}


void IMode::initialize()
{
}


void IMode::uninitialize()
{
}


void IMode::enable()
{
	setState(this, ModeState::Enabled);
}


void IMode::disable()
{
	setState(this, ModeState::Disabled);
}


void IMode::setData(const std::string& name, const std::string& value)
{
	_data[name] = value;
	ModeDataChanged.dispatch(this, _data);
}


void IMode::removeVar(const std::string& name) 
{
	StringMap::iterator it = _data.find(name);
	if (it != _data.end()) {
		_data.erase(it);
		ModeDataChanged.dispatch(this, _data);
	}
}


void IMode::clearData() 
{
	_data.clear();
	ModeDataChanged.dispatch(this, _data);
}


/*
void IMode::createConfigForm(Symple::Form& form, Symple::FormElement& element, bool useBase) 
{	
	XMPP::FormField field = form.addField("fixed");	
	field.addValue("No configuration options are available.");		
}


void IMode::parseConfigForm(Symple::Form& form, Symple::FormElement& element, StringMap& errors) 
{
}


void IMode::createHelp(ostream& s)
{
	s << _name << ": Sorry, no help is available.";
}
*/


// ---------------------------------------------------------------------
//
ModeConfiguration::ModeConfiguration(IMode& mode) :
	mode(mode)
{
}


string ModeConfiguration::getString(const string& key, const string& defaultValue) const 
{
	return mode.env().config().getString(getDefaultScope(key), 
		mode.env().config().getString(getBaseScope(key), defaultValue));
}


int ModeConfiguration::getInt(const string& key, int defaultValue) const 
{
	return mode.env().config().getInt(getDefaultScope(key), 
		mode.env().config().getInt(getBaseScope(key), defaultValue));
}


double ModeConfiguration::getDouble(const string& key, double defaultValue) const 
{
	return mode.env().config().getDouble(getDefaultScope(key), 
		mode.env().config().getDouble(getBaseScope(key), defaultValue));
}


bool ModeConfiguration::getBool(const string& key, bool defaultValue) const 
{
	return mode.env().config().getBool(getDefaultScope(key), 
		mode.env().config().getBool(getBaseScope(key), defaultValue));
}


void ModeConfiguration::setString(const string& key, const string& value, bool useBase) 
{
	mode.env().config().setString(getScoped(key, useBase), value);
}


void ModeConfiguration::setInt(const string& key, int value, bool useBase) 
{	
	mode.env().config().setInt(getScoped(key, useBase), value);
}


void ModeConfiguration::setDouble(const string& key, double value, bool useBase) 
{
	mode.env().config().setDouble(getScoped(key, useBase), value);
}


void ModeConfiguration::setBool(const string& key, bool value, bool useBase) 
{
	mode.env().config().setBool(getScoped(key, useBase), value);
}


string ModeConfiguration::getDefaultScope(const string& key) const
{	
	//if (defaultScope.empty())
	return defaultScope + key;
}


string ModeConfiguration::getBaseScope(const string& key) const
{
	//if (baseScope.empty())
	return baseScope + key;
}


string ModeConfiguration::getScoped(const string& key, bool useBase) const
{
	return useBase ? getBaseScope(key) : getDefaultScope(key);
}


} } // namespace Sourcey::Spot