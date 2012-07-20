include_dependency(Poco REQUIRED)
include_dependency(OpenSSL REQUIRED)
include_dependency(FFmpeg REQUIRED)
include_dependency(OpenCV REQUIRED)
include_dependency(RtAudio REQUIRED)
include_dependency(JsonCpp REQUIRED)
  
define_libsourcey_test(MediaTests Base Media Net) #VideoSocket Anionu Base Net XML HTTP 
