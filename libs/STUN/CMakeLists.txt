ask_build_sourcey_module(STUN)
if(BUILD_MODULE_STUN) 
  include_dependency(Poco REQUIRED)
  include_dependency(OpenSSL REQUIRED)

  define_sourcey_module(STUN Base Net)
endif()