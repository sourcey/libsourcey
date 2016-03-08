ask_build_sourcey_application(sympleconsole)
if(BUILD_APPLICATIONS AND BUILD_APPLICATION_sympleconsole)
  set(sympleconsole_EXECUTABLE_NAME "symple")
  set(sympleconsole_DEBUG_POSTFIX "")

  define_sourcey_application(sympleconsole base uv crypto net http socketio symple json util)
endif()
