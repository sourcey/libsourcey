#include_dependency(Poco REQUIRED)
include_dependency(OpenSSL REQUIRED)
include_dependency(JsonCpp REQUIRED)
  
define_libsourcey_test(SympleTests Base UV Crypto Net HTTP JSON SocketIO Symple)