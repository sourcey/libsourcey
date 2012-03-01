ask_build_sourcey_module(SocketIO)
if(BUILD_MODULE_SocketIO) 
  include_dependency(Poco REQUIRED)
  include_dependency(OpenSSL REQUIRED)
  include_dependency(JsonCpp REQUIRED)

  define_sourcey_module(SocketIO Base Net JSON)
endif()