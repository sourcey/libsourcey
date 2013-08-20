#include_dependency(Poco REQUIRED)
include_dependency(OpenSSL REQUIRED)
include_dependency(FFmpeg REQUIRED)
include_dependency(OpenCV REQUIRED)
include_dependency(RtAudio REQUIRED)

define_sourcey_module(ICE Base Net Media RTP STUN TURN SDP)