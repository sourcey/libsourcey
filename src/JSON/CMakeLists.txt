ask_build_sourcey_module(JSON)
if(BUILD_MODULES AND BUILD_MODULE_JSON) 
  include_dependency(Poco REQUIRED)
  include_dependency(JsonCpp REQUIRED)
  include_dependency(LibUV REQUIRED)

  define_sourcey_module(JSON Base UV)
endif()
