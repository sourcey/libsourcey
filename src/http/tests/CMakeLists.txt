# include_dependency(OpenSSL REQUIRED)
# include_dependency(LibUV)
# include_dependency(JsonCpp)
# include_dependency(HttpParser)

define_libsourcey_test(httptests uv base crypto net http)
