include_dependency(LibUV)
include_dependency(OpenSSL REQUIRED)

define_libsourcey_test(stuntests base uv stun crypto net)
