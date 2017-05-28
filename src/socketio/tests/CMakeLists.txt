# include_dependency(JsonCpp)
include_dependency(SSL REQUIRED)

set(socketiotests_EXCLUDE_CTEST TRUE)
define_libsourcey_test(socketiotests base crypto net http socketio json)
