ask_build_sourcey_module(UV)
if(BUILD_MODULES AND BUILD_MODULE_UV) 
  include_dependency(LibUV REQUIRED)
  define_sourcey_module(UV) # Base Net Media
endif()

  ##include_dependency(Poco REQUIRED)
  #include_dependency(OpenSSL REQUIRED)