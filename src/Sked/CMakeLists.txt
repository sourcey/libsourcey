ask_build_sourcey_module(Sked)
if(BUILD_MODULES AND BUILD_MODULE_Sked) 
  include_dependency(Poco REQUIRED)
  include_dependency(OpenSSL REQUIRED)
  include_dependency(JsonCpp REQUIRED)

  define_sourcey_module(Sked Base HTTP JSON)
endif()
