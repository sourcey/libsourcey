ask_build_sourcey_module(Symple)
if(BUILD_MODULES AND BUILD_MODULE_Symple) 
  #include_dependency(Poco REQUIRED)
  include_dependency(OpenSSL REQUIRED)
  include_dependency(JsonCpp REQUIRED)
  include_dependency(LibUV REQUIRED)
  include_dependency(HttpParser REQUIRED)
  
  # FIXME: Including OpenCV dependency here for MediaServer 
  # sample although it is not directly required by Symple.
  # include_dependency for samples does not include the 
  # dependency libraries properly.
  include_dependency(OpenCV REQUIRED)

  define_sourcey_module(Symple Base UV Crypto Net HTTP SocketIO JSON Util)
endif()