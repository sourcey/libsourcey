# include_dependency(JsonCpp)
# include_dependency(OpenSSL REQUIRED)

define_libsourcey_test(socketiotests base uv crypto net http socketio json)
