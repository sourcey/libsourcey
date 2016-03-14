include_dependency(LibUV)
include_dependency(HttpParser)
include_dependency(RtAudio)
include_dependency(OpenSSL REQUIRED)
include_dependency(FFmpeg REQUIRED)
include_dependency(OpenCV REQUIRED)

define_sourcey_module_sample(mediaserver uv base media net http turn stun util)
