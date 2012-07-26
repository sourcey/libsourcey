include_dependency(Poco REQUIRED)
include_dependency(OpenSSL REQUIRED)
  
define_libsourcey_test(TURNClientTest Base Net STUN TURN Util)
