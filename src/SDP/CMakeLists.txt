ask_build_sourcey_module(SDP)
if(BUILD_MODULE_SDP) 
  include_dependency(Poco REQUIRED)
  include_dependency(OpenSSL REQUIRED)

  define_sourcey_module(SDP Base Net)
endif()
