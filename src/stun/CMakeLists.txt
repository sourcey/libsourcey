ask_build_sourcey_module(stun)
if(BUILD_MODULES AND BUILD_MODULE_stun) 

  #include_dependency(OpenSSL REQUIRED)
  #include_dependency(LibUV REQUIRED)

  define_sourcey_module(stun base uv crypto net)
endif()