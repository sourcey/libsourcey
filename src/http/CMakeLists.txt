ask_build_sourcey_module(http)
if(BUILD_MODULES AND BUILD_MODULE_http) 
  
  include_dependency(LibUV REQUIRED)
  include_dependency(OpenSSL REQUIRED)
  include_dependency(HttpParser)
  
  # list(APPEND LibSourcey_BUILD_DEPENDENCIES http_parser)
  add_subdirectory(vendor/http_parser)
  # set(HAVE_HTTPPARSER 1)
  
  define_sourcey_module(http base net uv crypto) # util
endif()