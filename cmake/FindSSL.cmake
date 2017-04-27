########################################################################
# CMake module for finding SSL library, either OpenSSL or BoringSSL
#

if (OPENSSL_IS_BORINGSSL)
  # KLUDGE: Dont include all WebRTC dependencies,
  # filter only the BoringSSL includes and libs.
  include_dependency(WebRTC ${ARGN})
  set(SSL_FOUND ${WEBRTC_FOUND})
  message(STATUS "Including BoringSSL ${WEBRTC_FOUND}")
else()
  include_dependency(OpenSSL ${ARGN})
  set(SSL_FOUND ${OPENSSL_FOUND})
    message(STATUS "Including OpenSSL ${OPENSSL_FOUND}")
endif()
