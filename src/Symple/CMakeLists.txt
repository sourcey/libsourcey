ask_build_sourcey_module(Symple)
if(BUILD_MODULES AND BUILD_MODULE_Symple) 
  include_dependency(Poco REQUIRED)
  include_dependency(OpenSSL REQUIRED)
  include_dependency(JsonCpp REQUIRED)

  define_sourcey_module(Symple Base Net JSON HTTP SocketIO)
endif()