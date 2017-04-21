########################################################################
# CMake module for finding SSL library, either OpenSSL or BoringSSL
#

if (OPENSSL_IS_BORINGSSL)
  message(STATUS "Including BoringSSL")

  # KLUDGE: Dont include all WebRTC dependencies,
  # filter only the BoringSSL includes and libs.
  include_dependency(WebRTC ${ARGN})
  set(SSL_FOUND ${WEBRTC_FOUND})
else()
  message(STATUS "Including OpenSSL")
  include_dependency(OpenSSL ${ARGN})
  set(SSL_FOUND ${OPENSSL_FOUND})
endif()
