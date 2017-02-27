ask_build_sourcey_module(json)
if(BUILD_MODULES OR (NOT BUILD_MODULES AND BUILD_MODULE_json))
  define_sourcey_module(json base uv)
endif()
