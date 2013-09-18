ask_build_sourcey_module(Crypto)
if(BUILD_MODULES AND BUILD_MODULE_Crypto) 
  #include_dependency(Poco REQUIRED)
  include_dependency(OpenSSL REQUIRED)
  include_dependency(LibUV REQUIRED)

  define_sourcey_module(Crypto Base UV)
endif()
