#ifndef SOURCEY_TestPluginAPI_H
#define SOURCEY_TestPluginAPI_H


#include "Sourcey/Base.h"


namespace scy {


// This value is incremented whenever there are ABI
// breaking changes to LibSourcey or dependencies.
#define SOURCEY_PLUGIN_ABI_VERSION 1
	
#ifdef WIN32
# define SOURCEY_PLUGIN_EXPORT __declspec(dllexport)
#else
# define SOURCEY_PLUGIN_EXPORT // empty
#endif


class IPlugin 
{
public:
	IPlugin() {};
	virtual ~IPlugin() {};
};


SOURCEY_EXTERN typedef IPlugin* (*GetPluginFunc)();

struct PluginDetails {
	int abiVersion;
	const char* fileName;
	const char* className;
	const char* pluginName;
	const char* pluginVersion;
	GetPluginFunc initializeFunc;
};

#define SOURCEY_STANDARD_PLUGIN_STUFF \
	SOURCEY_PLUGIN_ABI_VERSION,       \
	__FILE__

// new classType;
#define SOURCEY_PLUGIN(classType, pluginName, pluginVersion) \
  extern "C" {		                                         \
      SOURCEY_PLUGIN_EXPORT scy::IPlugin* getPlugin()        \
      {                                                      \
		  static classType singleton;                        \
	      return &singleton;                                 \
      }                                                      \
      SOURCEY_PLUGIN_EXPORT scy::PluginDetails exports =        \
      {                                                      \
          SOURCEY_STANDARD_PLUGIN_STUFF,                     \
          #classType,                                        \
          pluginName,                                        \
          pluginVersion,                                     \
          getPlugin,                                         \
      };                                                     \
  }


} // namespace scy


#endif