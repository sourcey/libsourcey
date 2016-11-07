# include_dependency(JsonCpp)
include_dependency(OpenSSL REQUIRED)

set(socketiotests_EXCLUDE_CTEST TRUE)
define_libsourcey_test(socketiotests uv base crypto net http socketio json)
