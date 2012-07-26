ask_build_sourcey_module(HTTP)
if(BUILD_MODULES AND BUILD_MODULE_HTTP) 
  include_dependency(Poco REQUIRED)
  include_dependency(OpenSSL REQUIRED)

  define_sourcey_module(HTTP Base Net Util)
endif()