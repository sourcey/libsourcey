# include_dependency(SSL REQUIRED)
# include_dependency(LibUV)
# include_dependency(JsonCpp)
# include_dependency(HttpParser)

define_libsourcey_test(httptests uv base crypto net http)
