#include "TestPlugin.h"

#include <iostream>

	
//
// Base Test Plugin
// 


SCY_PLUGIN(TestPlugin, "Test Plugin", "0.1.1")


TestPlugin::TestPlugin()
{
	std::cout << "TestPlugin: Instantiating" << std::endl;
}


TestPlugin::~TestPlugin() 
{
	std::cout << "TestPlugin: Destroying" << std::endl;
}


void TestPlugin::setValue(const char *value)
{
	std::cout << "TestPlugin: Setting value: " << value << std::endl;
	_value = value;
}


std::string TestPlugin::sValue() const 
{ 
	return _value;
}


const char* TestPlugin::cValue() const 
{ 
	return _value.c_str();
}



//
// Export Methods
// 


extern "C" SCY_PLUGIN_EXPORT int gimmeFive();


int gimmeFive()
{
	scy::errorL("TestPlugin") << "gimmeFive" << std::endl;
	return 5;
}