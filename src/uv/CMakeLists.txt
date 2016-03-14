ask_build_sourcey_module(uv)
if(BUILD_MODULES AND BUILD_MODULE_uv)

  set(BUILD_TESTS OFF) # don't build libuv tests
  add_vendor_dependency(LIBUV libuv)

  #if(WITH_LIBUV)
  #  list(APPEND LibSourcey_BUILD_DEPENDENCIES libuv)
  #  set(BUILD_TESTS OFF) # don't build tests
  #  add_subdirectory(vendor/libuv)
  #  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendor/libuv)
  #  set(HAVE_LIBUV 1)
  #endif()

  #include_dependency(LibUV) #REQUIRED

  define_sourcey_module(uv)
endif()
