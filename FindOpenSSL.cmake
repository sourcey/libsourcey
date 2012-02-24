# from wzdftpd
# via http://fresh.t-systems-sfr.com/unix/src/privat2/wzdftpd-0.8.2.tar.gz:a/wzdftpd-0.8.2/cmake/FindOpenSSL.cmake
# - Try to find the OpenSSL encryption library
# Once done this will define
#
#  OPENSSL_FOUND - system has the OpenSSL library
#  OPENSSL_INCLUDE_DIR - the OpenSSL include directory
#  OPENSSL_LIBRARIES - The libraries needed to use OpenSSL  
          
set(OPENSSL_DIR "${LIBSOURCEY_SOURCE_DIR}/3rdparty/openssl" CACHE PATH "OpenSSL install location")  
#set(OPENSSL_INCLUDE_DIR "${OPENSSL_DIR}/include/")  
          
if (OPENSSL_INCLUDE_DIR AND OPENSSL_LIBRARIES)

  # in cache already
  SET(OPENSSL_FOUND TRUE)

else (OPENSSL_INCLUDE_DIR AND OPENSSL_LIBRARIES)

  FIND_PATH(OPENSSL_INCLUDE_DIR openssl/ssl.h
     /usr/include/
     /usr/local/include/
     $ENV{ProgramFiles}/OpenSSL/include/
     $ENV{SystemDrive}/OpenSSL/include/
     ${OPENSSL_DIR}/include/
  )  

  if(WIN32 AND MSVC)
      # /MD and /MDd are the standard values - if somone wants to use
      # others, the libnames have to change here too
      # see http://www.openssl.org/support/faq.html#PROG2 for their meaning

      FIND_LIBRARY(SSL_EAY_DEBUG NAMES ssleay32MDd
        PATHS
        $ENV{ProgramFiles}/OpenSSL/lib/VC/
        $ENV{SystemDrive}/OpenSSL/lib/VC/
        ${OPENSSL_DIR}/lib/VC/
      )
      FIND_LIBRARY(SSL_EAY_RELEASE NAMES ssleay32MD
        PATHS
        $ENV{ProgramFiles}/OpenSSL/lib/VC/
        $ENV{SystemDrive}/OpenSSL/lib/VC/
        ${OPENSSL_DIR}/lib/VC/
      )
      FIND_LIBRARY(LIB_EAY_DEBUG NAMES libeay32MDd
        PATHS
        $ENV{ProgramFiles}/OpenSSL/lib/VC/
        $ENV{SystemDrive}/OpenSSL/lib/VC/
        ${OPENSSL_DIR}/lib/VC/
      )
      FIND_LIBRARY(LIB_EAY_RELEASE NAMES libeay32MD
        PATHS
        $ENV{ProgramFiles}/OpenSSL/lib/VC/
        $ENV{SystemDrive}/OpenSSL/lib/VC/
        ${OPENSSL_DIR}/lib/VC/
      )

      IF(MSVC_IDE)
        IF(SSL_EAY_DEBUG AND SSL_EAY_RELEASE)
          SET(OPENSSL_LIBRARIES optimized ${SSL_EAY_RELEASE} ${LIB_EAY_RELEASE} debug ${SSL_EAY_DEBUG} ${LIB_EAY_DEBUG})
        ELSE(SSL_EAY_DEBUG AND SSL_EAY_RELEASE)
          MESSAGE(FATAL_ERROR "Could not find the debug and release version of openssl")   
        ENDIF(SSL_EAY_DEBUG AND SSL_EAY_RELEASE)
      ELSE(MSVC_IDE)
        STRING(TOLOWER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_TOLOWER)
        IF(CMAKE_BUILD_TYPE_TOLOWER MATCHES debug)
          SET(OPENSSL_LIBRARIES ${SSL_EAY_DEBUG} ${LIB_EAY_DEBUG})
        ELSE(CMAKE_BUILD_TYPE_TOLOWER MATCHES debug)
          SET(OPENSSL_LIBRARIES ${SSL_EAY_RELEASE} ${LIB_EAY_RELEASE})
        ENDIF(CMAKE_BUILD_TYPE_TOLOWER MATCHES debug)
      ENDIF(MSVC_IDE)
      MARK_AS_ADVANCED(SSL_EAY_DEBUG SSL_EAY_RELEASE LIB_EAY_DEBUG LIB_EAY_RELEASE)
  else(WIN32 AND MSVC)

   FIND_LIBRARY(OPENSSL_LIBRARIES NAMES ssl ssleay32 ssleay32MD libeay32 libeay32MD
      PATHS
      /usr/lib
      /usr/local/lib
   )
   FIND_LIBRARY(CRYPTO_LIBRARIES crypto
      PATHS
      /usr/lib
      /usr/local/lib
   )

  endif(WIN32 AND MSVC)

  if (OPENSSL_INCLUDE_DIR AND OPENSSL_LIBRARIES)
     set(OPENSSL_FOUND TRUE)
  endif (OPENSSL_INCLUDE_DIR AND OPENSSL_LIBRARIES)

  if (CRYPTO_LIBRARIES)
     set(CRYPTO_FOUND TRUE)
     message(STATUS "Found libcrypto: ${CRYPTO_LIBRARIES}")
  endif (CRYPTO_LIBRARIES)

  if (OPENSSL_FOUND)
     if (NOT OpenSSL_FIND_QUIETLY)
        message(STATUS "Found OpenSSL: ${OPENSSL_LIBRARIES}")
     endif (NOT OpenSSL_FIND_QUIETLY)
  else (OPENSSL_FOUND)
     if (OpenSSL_FIND_REQUIRED)
        message(FATAL_ERROR "Could NOT find OpenSSL")
     endif (OpenSSL_FIND_REQUIRED)
  endif (OPENSSL_FOUND)

  MARK_AS_ADVANCED(OPENSSL_INCLUDE_DIR OPENSSL_LIBRARIES)

endif (OPENSSL_INCLUDE_DIR AND OPENSSL_LIBRARIES)

# MESSAGE( "${OPENSSL_FOUND}" )
# MESSAGE( "${OPENSSL_INCLUDE_DIR}" )
# MESSAGE( "${OPENSSL_LIBRARY_DIR}" )
# MESSAGE( "OPENSSL_LIBRARIES}" )

# Link and include
include_directories(${OPENSSL_INCLUDE_DIR})  
link_directories(${OPENSSL_LIBRARY_DIR})
                      
set(LIBSOURCEY_INCLUDE_DIRS ${LIBSOURCEY_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR})
set(LIBSOURCEY_LIBRARY_DIRS ${LIBSOURCEY_LIBRARY_DIRS} ${OPENSSL_LIBRARY_DIR})
set(LIBSOURCEY_DEBUG_LIBS   ${LIBSOURCEY_DEBUG_LIBS}   ${OPENSSL_LIBRARIES})    
set(LIBSOURCEY_RELEASE_LIBS ${LIBSOURCEY_RELEASE_LIBS} ${OPENSSL_LIBRARIES})