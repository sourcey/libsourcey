# include_dependency(LibUV)
# include_dependency(HttpParser)
include_dependency(SSL REQUIRED)
include_dependency(FFmpeg REQUIRED)
# include_dependency(RtAudio)
# include_dependency(OpenCV REQUIRED)


define_sourcey_module_sample(mediaserver base av net http turn stun util)
