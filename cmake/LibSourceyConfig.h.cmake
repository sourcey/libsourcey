/* Name of package */
#define SOURCEY_PACKAGE "${PACKAGE}"

/* Define to the address where bug reports for this package should be sent. */
#define SOURCEY_PACKAGE_BUGREPORT "${PACKAGE_BUGREPORT}"

/* Define to the full name of this package. */
#define SOURCEY_PACKAGE_NAME "${PACKAGE_NAME}"

/* Define to the full name and version of this package. */
#define SOURCEY_PACKAGE_STRING "${PACKAGE_STRING}"

/* Define to the one symbol short name of this package. */
#define SOURCEY_PACKAGE_TARNAME "${PACKAGE_TARNAME}"

/* Define to the version of this package. */
#define SOURCEY_PACKAGE_VERSION "${PACKAGE_VERSION}"

/* Version number of package */
#cmakedefine SOURCEY_BUILD_SHARED "${BUILD_SHARED_LIBS}" 

/* LibSourcey core modules
# cmakedefine HAVE_SOURCEY_Base
# cmakedefine HAVE_SOURCEY_HTTP
# cmakedefine HAVE_SOURCEY_JSON
# cmakedefine HAVE_SOURCEY_Media
# cmakedefine HAVE_SOURCEY_Net
# cmakedefine HAVE_SOURCEY_Pacman
# cmakedefine HAVE_SOURCEY_RTP
# cmakedefine HAVE_SOURCEY_SDP
# cmakedefine HAVE_SOURCEY_SocketIO
# cmakedefine HAVE_SOURCEY_STUN
# cmakedefine HAVE_SOURCEY_Symple
# cmakedefine HAVE_SOURCEY_TURN
# cmakedefine HAVE_SOURCEY_XML
# cmakedefine HAVE_SOURCEY_XMPP

# cmakedefine HAVE_SOURCEY_UV
# cmakedefine HAVE_SOURCEY_RTSP
# cmakedefine HAVE_SOURCEY_ICE */

/* LibSourcey ext modules
# cmakedefine HAVE_SOURCEY_ISpot
# cmakedefine HAVE_SOURCEY_Anionu */

/* Anionu API Credentials */
#define Anionu_API_USERNAME "${Anionu_API_USERNAME}"
#define Anionu_API_PASSWORD "${Anionu_API_PASSWORD}"

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

/* LibUV library */
#cmakedefine HAVE_LIBUV

/* Define to 1 if you have the <inttypes.h> header file. */
#cmakedefine HAVE_INTTYPES_H 1

/* Define to 1 if your processor stores words with the most significant byte
   first (like Motorola and SPARC, unlike Intel and VAX). */
#cmakedefine WORDS_BIGENDIAN
