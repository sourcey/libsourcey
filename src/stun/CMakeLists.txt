ask_build_sourcey_module(stun)
if(BUILD_MODULES OR (NOT BUILD_MODULES AND BUILD_MODULE_stun))
  # include_dependency(LibUV) #REQUIRED
  #include_dependency(OpenSSL REQUIRED)

  define_sourcey_module(stun uv base net crypto)
endif()
