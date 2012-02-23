#ifndef ANIONU_SPOT_IConfigurable_H
#define ANIONU_SPOT_IConfigurable_H


#include "Sourcey/Base.h"
#include "Sourcey/Symple/Form.h"

#include <iostream>


namespace Sourcey {
namespace Spot {


class IConfigurable
{	
public:
	IConfigurable();	
	virtual ~IConfigurable();
	
	virtual bool isConfigurable() const;
		// This method is always called before generation methods 
		// to check hat the implementation provides configuration.
		// If false is returned createConfigForm() will never be
		// called.

	virtual bool hasParsableConfig(Symple::Form& form) const;
		// Checks if there is any parsable items for this class in
		// the provided form. If false is returned parseConfigForm()
		// will not be called.

	virtual void createHelp(std::ostream& ost);
	virtual void createConfigForm(Symple::Form& form, Symple::FormElement& element, bool useBase);
		// Generates the config form elements for this class. The
		// useBase variable is only applicable for classes which
		// use multi-scoped configuration such as IModes.

	virtual void parseConfigForm(Symple::Form& form, Symple::FormElement& element); //, StringMap& errors
}; 


} } // namespace Sourcey::Spot


#endif // ANIONU_SPOT_IConfigurable_H


//protected:
//	IConfiguration&	_config;
//	//std::string		_scope;
//, const std::string& scope IConfiguration& config
	//virtual std::string scope() const	{ return _scope; };
	//virtual IConfiguration& config() 	{ return _config; };

//class IConfigurable;
	//virtual IEnvironment& env() 			{ return _env; };
	//virtual ModeConfiguration& config() 	{ return _config; };
	//virtual ModeOptions& options()  		{ return _options; };

	//IEnvironment&		_env;	

	//ModeConfiguration	_config;
	//ModeOptions			_options;
	//std::string			_name;
	//StringMap			_data;

	//friend class ModeConfiguration;
//class IEnvironment;

/*


class IConfiguration;
class ModeConfiguration 
	/// ModeConfiguration enables modes to access config values at
	/// their config specific namespace falling back to mode defaults.
	/// Default: configs.[name].modes.[name].[value]
	/// Root: modes.[name].[value]
{	
public:
	ModeConfiguration(IConfigurable& mode);
		
	//void setup();

	std::string getString(const std::string& key, const std::string& defaultValue) const;
	int getInt(const std::string& key, int defaultValue) const;
	double getDouble(const std::string& key, double defaultValue) const;
	bool getBool(const std::string& key, bool defaultValue) const;

	void setString(const std::string& key, const std::string& value, bool useBase = false);
	void setInt(const std::string& key, int value, bool useBase = false);
	void setDouble(const std::string& key, double value, bool useBase = false);
	void setBool(const std::string& key, bool value, bool useBase = false);
	
	std::string getBase(const std::string& key);
	std::string getRoot(const std::string& key);
	std::string getPath(const std::string& key, bool useBase = false);

	IConfigurable& mode;
	std::string base;
	std::string root;
};


typedef Poco::Net::NameValueCollection ModeOptions;
typedef std::map<std::string, IConfigurable*> IConfigurableMap;


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
*/

	
	/*	
	virtual void initialize();
	virtual void uninitialize();

	virtual void start();
	virtual void stop();

	virtual bool isDisabled() const { return stateEquals(ModeState::Disabled); };
	virtual bool isEnabled() const { return stateEquals(ModeState::Enabled); };
	virtual bool isFailed() const { return stateEquals(ModeState::Failed); };

	virtual void setData(const std::string& name, const std::string& value);
	virtual void removeVar(const std::string& name);
	virtual void clearData();
	virtual StringMap data() const { return _data; };
	
	Signal<const StringMap&> ModeDataChanged;
	
	virtual const char* className() const { return "Mode"; }
	*/