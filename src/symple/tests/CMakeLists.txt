#include_dependency(Poco REQUIRED)
include_dependency(OpenSSL REQUIRED)
include_dependency(JsonCpp REQUIRED)
  
define_libsourcey_test(sympletests base uv crypto net http json socketio symple)