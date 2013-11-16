ask_build_sourcey_module(symple)
if(BUILD_MODULES AND BUILD_MODULE_symple) 

  #include_dependency(OpenSSL REQUIRED)
  #include_dependency(JsonCpp REQUIRED)
  #include_dependency(LibUV REQUIRED)
  #include_dependency(HttpParser REQUIRED)
  
  # FIXME: Including OpenCV dependency here for MediaServer 
  # sample although it is not directly required by Symple.
  # include_dependency for samples does not include the 
  # dependency libraries properly.
  #include_dependency(OpenCV REQUIRED)

  define_sourcey_module(symple base uv crypto net http socketio json util)
endif()