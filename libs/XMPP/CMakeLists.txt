include_dependency(Poco REQUIRED)
include_dependency(OpenSSL REQUIRED)
include_dependency(LibStrophe REQUIRED)
include_dependency(PugiXML REQUIRED)
include_dependency(FFmpeg REQUIRED)   
include_dependency(OpenCV REQUIRED)   
include_dependency(RtAudio REQUIRED)

define_libsourcey_module(XMPP Base Net XML Media RTP STUN TURN ICE SDP)
