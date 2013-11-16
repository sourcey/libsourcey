ask_build_sourcey_module(sked)
if(BUILD_MODULES AND BUILD_MODULE_sked) 

  #include_dependency(OpenSSL REQUIRED)
  #include_dependency(JsonCpp REQUIRED)
  #include_dependency(LibUV REQUIRED)

  define_sourcey_module(sked base uv http json)
endif()
