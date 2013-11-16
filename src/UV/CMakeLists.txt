ask_build_sourcey_module(uv)
if(BUILD_MODULES AND BUILD_MODULE_uv) 
  #include_dependency(LibUV REQUIRED)
  define_sourcey_module(uv) # Base Net Media
endif()

  ##include_dependency(Poco REQUIRED)
  #include_dependency(OpenSSL REQUIRED)