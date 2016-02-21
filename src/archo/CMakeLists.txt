ask_build_sourcey_module(archo)
if (BUILD_MODULES AND BUILD_MODULE_archo)

  add_vendor_dependency(ZLIB zlib)
  add_vendor_dependency(MINIZIP minizip)
  #add_subdirectory(vendor/zlib)
  #add_subdirectory(vendor/minizip)

  include_dependency(LibUV) # REQUIRED)
  include_dependency(ZLIB)
  include_dependency(Minizip)

  define_sourcey_module(archo base uv)
endif ()
