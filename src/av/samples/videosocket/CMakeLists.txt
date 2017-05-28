#include_dependency(LibUV)
#include_dependency(HttpParser)
#include_dependency(JsonCpp)
#include_dependency(SSL REQUIRED)
include_dependency(FFmpeg REQUIRED)
include_dependency(OpenCV REQUIRED)

define_sourcey_module_sample(videosocket base crypto net http av util)
