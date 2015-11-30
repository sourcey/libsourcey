include_dependency(OpenSSL REQUIRED)
include_dependency(LibUV REQUIRED)
  
define_libsourcey_test(turnclienttest base net stun turn uv util)
