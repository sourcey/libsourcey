ask_build_sourcey_module(util)
if(BUILD_MODULES AND BUILD_MODULE_util) 

  #include_dependency(OpenSSL REQUIRED)
  #include_dependency(JsonCpp)
  #include_dependency(LibUV REQUIRED)

  define_sourcey_module(util base uv json)
endif()
