ask_build_sourcey_module(crypto)
if(BUILD_MODULES AND BUILD_MODULE_crypto) 

  #include_dependency(OpenSSL REQUIRED)
  #include_dependency(LibUV REQUIRED)

  define_sourcey_module(crypto base uv)
endif()
