ask_build_sourcey_module(sked)
if(BUILD_MODULES OR (NOT BUILD_MODULES AND BUILD_MODULE_sked)) 
  #include_dependency(LibUV) #REQUIRED
  #include_dependency(JsonCpp)

  define_sourcey_module(sked uv base json)
endif()
