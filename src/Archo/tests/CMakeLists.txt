#include_dependency(Poco REQUIRED)
include_dependency(OpenSSL REQUIRED)
include_dependency(JsonCpp REQUIRED)
  
define_libsourcey_test(ArchoTests Base UV Net HTTP JSON Archo)