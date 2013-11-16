#include_dependency(Poco REQUIRED)
include_dependency(OpenSSL REQUIRED)
include_dependency(LibUV REQUIRED)
  
define_libsourcey_test(stuntests base uv stun crypto net)