ask_build_sourcey_module(base)
if(BUILD_MODULES OR (NOT BUILD_MODULES AND BUILD_MODULE_base))
  #include_dependency(OpenSSL REQUIRED)
  #include_dependency(LibUV REQUIRED)

  # Include inttypes.h for windows
  if (MSVC)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendor/msvc)
  endif()

  define_sourcey_module(base uv)
endif()
