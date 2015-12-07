ask_build_sourcey_module(json)
if(BUILD_MODULES AND BUILD_MODULE_json) 

  #include_dependency(JsonCpp)
  #include_dependency(LibUV REQUIRED)

  define_sourcey_module(json base uv)
endif()
