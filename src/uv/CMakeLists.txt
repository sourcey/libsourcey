ask_build_sourcey_module(uv)
if(BUILD_MODULES AND BUILD_MODULE_uv)

  # Include libuv dependency
  set(BUILD_TESTS OFF) # don't build libuv tests
  set(LIB_INSTALL_DIR "${LibSourcey_VENDOR_INSTALL_DIR}/lib")
  set(BIN_INSTALL_DIR "${LibSourcey_VENDOR_INSTALL_DIR}/bin")
  set(INCLUDE_INSTALL_DIR "${LibSourcey_VENDOR_INSTALL_DIR}/include")
  add_vendor_dependency(LIBUV libuv)

  include_directories(../base/include)
  #include_dependency(LibUV) #REQUIRED

  define_sourcey_module(uv)
endif()
