ask_build_sourcey_module(util)
if(BUILD_MODULES OR (NOT BUILD_MODULES AND BUILD_MODULE_util))
  define_sourcey_module(util base json)
endif()
