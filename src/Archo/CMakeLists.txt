ask_build_sourcey_module(Archo)
if (BUILD_MODULES AND BUILD_MODULE_Archo) 

  #include_dependency(OpenSSL REQUIRED)
  include_dependency(LibUV REQUIRED)
  include_dependency(ZLIB REQUIRED)
  include_dependency(Minizip REQUIRED)

  define_sourcey_module(Archo Base UV)
endif ()
