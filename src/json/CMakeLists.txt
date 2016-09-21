ask_build_sourcey_module(json)
if(BUILD_MODULES OR (NOT BUILD_MODULES AND BUILD_MODULE_json))

  add_vendor_dependency(JSONCPP jsoncpp)

  # add_subdirectory(vendor/jsoncpp)
  # include_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendor/jsoncpp)

  # include_dependency(JsonCpp) #ME
  # include_dependency(LibUV) #REQUIRED

  define_sourcey_module(json base uv)
endif()
