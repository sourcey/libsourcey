/* Name of package */
#define PACKAGE "${PACKAGE}"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "${PACKAGE_BUGREPORT}"

/* Define to the full name of this package. */
#define PACKAGE_NAME "${PACKAGE_NAME}"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "${PACKAGE_STRING}"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "${PACKAGE_TARNAME}"

/* Define to the version of this package. */
#define PACKAGE_VERSION "${PACKAGE_VERSION}"

/* Version number of package */
#define LIBSOURCEY_VERSION "${PACKAGE_VERSION}"

/* LibSourcey core modules */
#cmakedefine HAVE_LIBSOURCEY_Base
#cmakedefine HAVE_LIBSOURCEY_HTTP
#cmakedefine HAVE_LIBSOURCEY_JSON
#cmakedefine HAVE_LIBSOURCEY_Media
#cmakedefine HAVE_LIBSOURCEY_Net
#cmakedefine HAVE_LIBSOURCEY_Pacman
#cmakedefine HAVE_LIBSOURCEY_RTP
#cmakedefine HAVE_LIBSOURCEY_SDP
#cmakedefine HAVE_LIBSOURCEY_SocketIO
#cmakedefine HAVE_LIBSOURCEY_STUN
#cmakedefine HAVE_LIBSOURCEY_Symple
#cmakedefine HAVE_LIBSOURCEY_TURN
#cmakedefine HAVE_LIBSOURCEY_XML
#cmakedefine HAVE_LIBSOURCEY_XMPP

#cmakedefine HAVE_LIBSOURCEY_UV
#cmakedefine HAVE_LIBSOURCEY_RTSP
#cmakedefine HAVE_LIBSOURCEY_ICE

/* LibSourcey ext modules */
#cmakedefine HAVE_LIBSOURCEY_ISpot
#cmakedefine HAVE_LIBSOURCEY_Anionu

#cmakedefine Anionu_API_USERNAME
#cmakedefine Anionu_API_PASSWORD

/* Poco C++ library */
#cmakedefine HAVE_POCO

/* FFMpeg video library */
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
#cmakedefine  WORDS_BIGENDIAN
