ask_build_sourcey_module(Base)
if(BUILD_MODULE_Base) 
  include_dependency(Poco REQUIRED)
  include_dependency(OpenSSL REQUIRED)
  
  define_sourcey_module(Base)
endif()