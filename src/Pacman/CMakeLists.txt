ask_build_sourcey_module(Pacman)
if(BUILD_MODULE_Pacman) 
  include_dependency(Poco REQUIRED)
  include_dependency(OpenSSL REQUIRED)
  include_dependency(JsonCpp REQUIRED)

  define_sourcey_module(Pacman Base Net JSON HTTP)
endif()
