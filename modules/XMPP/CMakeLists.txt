find_package(Poco REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(LibStrophe REQUIRED)
find_package(PugiXML REQUIRED)
find_package(FFmpeg REQUIRED)   
find_package(OpenCV REQUIRED)   
find_package(RtAudio REQUIRED)

define_libsourcey_module(XMPP Base Net XML Media RTP STUN TURN ICE SDP)
