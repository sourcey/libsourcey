/* Name of package */
#define SCY_PACKAGE "${PACKAGE}"

/* Define to the address where bug reports for this package should be sent. */
#define SCY_PACKAGE_BUGREPORT "${PACKAGE_BUGREPORT}"

/* Define to the full name of this package. */
#define SCY_PACKAGE_NAME "${PACKAGE_NAME}"

/* Define to the full name and version of this package. */
#define SCY_PACKAGE_STRING "${PACKAGE_STRING}"

/* Define to the one symbol short name of this package. */
#define SCY_PACKAGE_TARNAME "${PACKAGE_TARNAME}"

/* Define to the version of this package. */
#define SCY_PACKAGE_VERSION "${PACKAGE_VERSION}"

/* Define the installation path. */
#define SCY_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}"

/* Version number of package */
#cmakedefine SCY_BUILD_SHARED "${BUILD_SHARED_LIBS}" 

/* LibSourcey modules
# cmakedefine HAVE_SCY_base
# cmakedefine HAVE_SCY_http
# cmakedefine HAVE_SCY_json
# cmakedefine HAVE_SCY_media
# cmakedefine HAVE_SCY_net
# cmakedefine HAVE_SCY_rtp
# cmakedefine HAVE_SCY_sdp
# cmakedefine HAVE_SCY_socketio
# cmakedefine HAVE_SCY_stun
# cmakedefine HAVE_SCY_symple
# cmakedefine HAVE_SCY_turn
# cmakedefine HAVE_SCY_xml
# cmakedefine HAVE_SCY_xmpp
# cmakedefine HAVE_SCY_uv
# cmakedefine HAVE_SCY_rtsp
# cmakedefine HAVE_SCY_ice
# cmakedefine HAVE_SCY_pacm
# cmakedefine HAVE_SCY_anionu
# cmakedefine HAVE_SCY_spotapi */

/* LibUV library */
#cmakedefine HAVE_LIBUV

/* FFmpeg video library */
#cmakedefine HAVE_FFMPEG

/* OpenCV library */
#cmakedefine HAVE_OPENCV

/* OpenSSL library */
#cmakedefine HAVE_OPENSSL

/* PugiXML library */
#cmakedefine HAVE_PUGIXML

/* RtAudio library */
#cmakedefine HAVE_RTAUDIO

/* LibStrophe library */
#cmakedefine HAVE_LIBSTROPHE

/* Define to 1 if you have the <inttypes.h> header file. */
#cmakedefine HAVE_INTTYPES_H 1

/* Define to 1 if your processor stores words with the most significant byte
   first (like Motorola and SPARC, unlike Intel and VAX). */
#cmakedefine WORDS_BIGENDIAN
