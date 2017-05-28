ask_build_sourcey_module(base)
if(BUILD_MODULES OR (NOT BUILD_MODULES AND BUILD_MODULE_base))
  #include_dependency(SSL REQUIRED)
  #include_dependency(LibUV REQUIRED)

  define_sourcey_module(base)
endif()
