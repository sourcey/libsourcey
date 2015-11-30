#include_dependency(OpenSSL REQUIRED)
include_dependency(JsonCpp REQUIRED)
include_dependency(HttpParser REQUIRED)
include_dependency(LibUV REQUIRED)
  
define_libsourcey_test(httptests base uv crypto net http)