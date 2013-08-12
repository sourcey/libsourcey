include_dependency(Poco REQUIRED)
include_dependency(OpenSSL REQUIRED)
include_dependency(LibUV REQUIRED)
  
define_libsourcey_test(BaseTests Base UV)
add_subdirectory("plugin")