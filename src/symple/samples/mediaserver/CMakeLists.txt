#include_dependency(Poco REQUIRED)
include_dependency(OpenSSL REQUIRED)
include_dependency(FFmpeg REQUIRED)
include_dependency(RtAudio REQUIRED)
include_dependency(OpenCV REQUIRED)
include_dependency(LibUV REQUIRED)
include_dependency(HttpParser REQUIRED)
  
define_sourcey_module_sample(mediaserver base uv media net http turn stun util) #Anionu
