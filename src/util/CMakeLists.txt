ask_build_sourcey_module(util)
if(BUILD_MODULES AND BUILD_MODULE_util)
  define_sourcey_module(util uv base json)
endif()
