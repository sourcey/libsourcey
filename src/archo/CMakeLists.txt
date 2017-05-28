ask_build_sourcey_module(archo)
if(BUILD_MODULES OR (NOT BUILD_MODULES AND BUILD_MODULE_archo))
  set(INSTALL_BIN_DIR "${LibSourcey_VENDOR_INSTALL_DIR}/bin")
  set(INSTALL_LIB_DIR "${LibSourcey_VENDOR_INSTALL_DIR}/lib")
  set(INSTALL_INC_DIR "${LibSourcey_VENDOR_INSTALL_DIR}/include")
  set(INSTALL_MAN_DIR "${LibSourcey_VENDOR_INSTALL_DIR}/share/man")
  set(INSTALL_PKGCONFIG_DIR "${LibSourcey_VENDOR_INSTALL_DIR}/share/pkgconfig")
  
  #add_subdirectory(vendor/zlib)
  #add_subdirectory(vendor/minizip)

  #include_dependency(LibUV) # REQUIRED)
  #include_dependency(ZLIB)
  #include_dependency(Minizip)

  define_sourcey_module(archo base)
endif ()
