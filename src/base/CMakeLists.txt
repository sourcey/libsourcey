ask_build_sourcey_module(base)
if(BUILD_MODULES AND BUILD_MODULE_base) 
  #include_dependency(OpenSSL REQUIRED)
  #include_dependency(LibUV REQUIRED)
  
  define_sourcey_module(base uv)
endif()