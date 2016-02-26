ask_build_sourcey_module(net)
if(BUILD_MODULES AND BUILD_MODULE_net)
  #include_dependency(OpenSSL REQUIRED)
  #include_dependency(LibUV REQUIRED)

  define_sourcey_module(net base crypto uv)

  include_directories(${LIBUV_INCLUDE_DIR})
endif()
