ask_build_sourcey_module(TURN)
if(BUILD_MODULE_TURN) 
  include_dependency(Poco REQUIRED)
  include_dependency(OpenSSL REQUIRED)

  define_sourcey_module(TURN Base Net STUN)
endif()
