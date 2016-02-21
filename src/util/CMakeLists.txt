ask_build_sourcey_module(util)
if(BUILD_MODULES AND BUILD_MODULE_util)

  include_dependency(LibUV)
  include_dependency(JsonCpp)

  define_sourcey_module(util base uv json)
endif()
