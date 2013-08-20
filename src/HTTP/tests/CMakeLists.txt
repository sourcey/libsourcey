##include_dependency(Poco REQUIRED)
include_dependency(OpenSSL REQUIRED)
include_dependency(JsonCpp REQUIRED)
include_dependency(HttpParser REQUIRED)
include_dependency(LibUV REQUIRED)
  
define_libsourcey_test(HTTPTests Base UV Net Crypto HTTP)