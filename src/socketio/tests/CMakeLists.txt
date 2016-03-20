# include_dependency(JsonCpp)
# include_dependency(OpenSSL REQUIRED)

define_libsourcey_test(socketiotests uv base crypto net http socketio json)
