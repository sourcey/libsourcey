ask_build_sourcey_module(Util)
if(BUILD_MODULES AND BUILD_MODULE_Util) 
  #include_dependency(Poco REQUIRED)
  include_dependency(OpenSSL REQUIRED)
  include_dependency(JsonCpp REQUIRED)
  include_dependency(LibUV REQUIRED)

  define_sourcey_module(Util Base UV HTTP JSON)
endif()
