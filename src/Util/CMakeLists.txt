ask_build_sourcey_module(Util)
if(BUILD_MODULES AND BUILD_MODULE_Util) 
  include_dependency(Poco REQUIRED)
  include_dependency(OpenSSL REQUIRED)
  include_dependency(JsonCpp REQUIRED)

  define_sourcey_module(Net Base HTTP JSON)
endif()
