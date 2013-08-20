#include_dependency(Poco REQUIRED)
include_dependency(OpenSSL REQUIRED)
include_dependency(JsonCpp REQUIRED)
  
define_libsourcey_test(CryptoTests Base UV Net HTTP JSON Crypto)