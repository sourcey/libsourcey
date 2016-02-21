ask_build_sourcey_module(archo)
if (BUILD_MODULES AND BUILD_MODULE_archo) 

  #include_dependency(OpenSSL REQUIRED)
  #include_dependency(LibUV REQUIRED)
  #include_dependency(ZLIB REQUIRED)
  #include_dependency(Minizip REQUIRED)

  add_subdirectory(vendor/zlib)
  add_subdirectory(vendor/minizip)

  define_sourcey_module(archo base uv)
endif ()
