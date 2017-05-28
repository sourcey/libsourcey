ask_build_sourcey_module(sched)
if(BUILD_MODULES OR (NOT BUILD_MODULES AND BUILD_MODULE_sched))
  #include_dependency(LibUV) #REQUIRED
  #include_dependency(JsonCpp)

  define_sourcey_module(sched base json)
endif()
