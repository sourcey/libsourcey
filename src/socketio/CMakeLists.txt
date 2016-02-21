ask_build_sourcey_module(socketio)
if(BUILD_MODULES AND BUILD_MODULE_socketio) 
  include_dependency(LibUV REQUIRED)
  include_dependency(OpenSSL REQUIRED)
  include_dependency(JsonCpp)
  include_dependency(HttpParser)

  define_sourcey_module(socketio base uv crypto net http json)
endif()