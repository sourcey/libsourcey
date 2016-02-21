# include_dependency(OpenSSL REQUIRED)
# include_dependency(LibUV)
# include_dependency(JsonCpp)
# include_dependency(HttpParser)

define_libsourcey_test(httptests base uv crypto net http)
