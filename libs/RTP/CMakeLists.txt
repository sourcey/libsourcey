ask_build_sourcey_module(RTP)
if(BUILD_MODULE_RTP) 
  include_dependency(Poco REQUIRED)
  include_dependency(OpenSSL REQUIRED)

  define_sourcey_module(RTP Base Net Media)
endif()