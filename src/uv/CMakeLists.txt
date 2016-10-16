ask_build_sourcey_module(uv)
if(BUILD_MODULES OR (NOT BUILD_MODULES AND BUILD_MODULE_uv))
  include_directories(../base/include)
  #include_dependency(LibUV REQUIRED)

  define_sourcey_module(uv)
endif()
