ask_build_sourcey_module(symple)
if(BUILD_MODULES OR (NOT BUILD_MODULES AND BUILD_MODULE_symple))
  #include_dependency(LibUV)
  include_dependency(SSL REQUIRED)
  #include_dependency(JsonCpp)
  #include_dependency(HttpParser)

  # FIXME: Including OpenCV dependency here for MediaServer
  # sample although it is not directly required by Symple.
  # include_dependency for samples does not include the
  # dependency libraries properly.
  #include_dependency(OpenCV REQUIRED)
  #include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../http/vendor/http_parser)

  define_sourcey_module(symple base crypto net http socketio json util)
endif()
