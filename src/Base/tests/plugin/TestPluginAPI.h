#ifndef SCY_TestPluginAPI_H
#define SCY_TestPluginAPI_H


#include "scy/base.h"


namespace scy {


// This value is incremented whenever there are ABI
// breaking changes to LibSourcey or dependencies.
#define SCY_PLUGIN_ABI_VERSION 1
	
#ifdef WIN32
# define SCY_PLUGIN_EXPORT __declspec(dllexport)
#else
# define SCY_PLUGIN_EXPORT // empty
#endif


class IPlugin 
{
public:
	IPlugin() {};
	virtual ~IPlugin() {};
};


SCY_EXTERN typedef IPlugin* (*GetPluginFunc)();

struct PluginDetails {
	int abiVersion;
	const char* fileName;
	const char* className;
	const char* pluginName;
	const char* pluginVersion;
	GetPluginFunc initializeFunc;
};

#define SCY_STANDARD_PLUGIN_STUFF \
	SCY_PLUGIN_ABI_VERSION,       \
	__FILE__

// new classType;
#define SCY_PLUGIN(classType, pluginName, pluginVersion) \
  extern "C" {		                                         \
      SCY_PLUGIN_EXPORT scy::IPlugin* getPlugin()        \
      {                                                      \
		  static classType singleton;                        \
	      return &singleton;                                 \
      }                                                      \
      SCY_PLUGIN_EXPORT scy::PluginDetails exports =        \
      {                                                      \
          SCY_STANDARD_PLUGIN_STUFF,                     \
          #classType,                                        \
          pluginName,                                        \
          pluginVersion,                                     \
          getPlugin,                                         \
      };                                                     \
  }


} // namespace scy


#endif