#ifndef SCY_TestPlugin_H
#define SCY_TestPlugin_H


#include "TestPluginAPI.h"
#include "scy/packetstream.h"
#include "scy/logger.h"
#include <string>


class TestPlugin: public scy::IPlugin 
{
public:
	TestPlugin();
	virtual ~TestPlugin();
	
	virtual void setValue(const char *value);
		// Sets the internal string value.
	
	virtual const char* cValue() const;
		// Return the value as a const char* so we can compile
		// compatible plugins with any compiler.

	virtual std::string sValue() const;
		// NOTE: Careful when sharing std containers across process  
		// boundaries. ABI compatibility will break if the library
		// was built with a different compiler than the application
		// (or with different standard lib ports/versions).

	scy::PacketStream stream;
		// Testing...

protected:
	 std::string _value;
};


#endif