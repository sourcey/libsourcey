#include_dependency(Poco REQUIRED)
include_dependency(OpenSSL REQUIRED)
include_dependency(LibUV REQUIRED)

define_sourcey_module_sample(turnserver base net stun turn uv)
