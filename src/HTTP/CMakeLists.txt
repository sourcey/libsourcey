ask_build_sourcey_module(http)
if(BUILD_MODULES AND BUILD_MODULE_http) 
  ##include_dependency(Poco REQUIRED)
  #include_dependency(OpenSSL REQUIRED) 
  #include_dependency(LibUV REQUIRED)
  #include_dependency(HttpParser REQUIRED)

  define_sourcey_module(http base net uv crypto) # util
endif()