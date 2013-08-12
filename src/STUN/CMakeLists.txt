ask_build_sourcey_module(STUN)
if(BUILD_MODULES AND BUILD_MODULE_STUN) 
  include_dependency(Poco REQUIRED)
  include_dependency(OpenSSL REQUIRED)
  include_dependency(LibUV REQUIRED)

  define_sourcey_module(STUN Base Net Crypto UV)
endif()