ask_build_sourcey_module(Sked)
if(BUILD_MODULES AND BUILD_MODULE_Sked) 

  include_dependency(OpenSSL REQUIRED)
  include_dependency(JsonCpp REQUIRED)
  include_dependency(LibUV REQUIRED)

  define_sourcey_module(Sked Base UV HTTP JSON)
endif()
