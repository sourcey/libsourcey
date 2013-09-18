ask_build_sourcey_module(Pacman)
if(BUILD_MODULES AND BUILD_MODULE_Pacman) 
  #include_dependency(Poco REQUIRED) # for ZipStream
  include_dependency(OpenSSL REQUIRED)
  include_dependency(JsonCpp REQUIRED)
  include_dependency(LibUV REQUIRED)
  include_dependency(HttpParser REQUIRED)
  include_dependency(ZLIB REQUIRED)
  include_dependency(Minizip REQUIRED)

  define_sourcey_module(Pacman Base UV Net JSON HTTP Archo)
endif()
