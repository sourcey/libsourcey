ask_build_sourcey_module(Net)
if(BUILD_MODULES AND BUILD_MODULE_Net) 
  include_dependency(Poco REQUIRED)
  include_dependency(OpenSSL REQUIRED)
  include_dependency(LibUV REQUIRED)

  define_sourcey_module(Net Base UV)
endif()
