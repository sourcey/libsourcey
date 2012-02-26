# from wzdftpd
# via http://fresh.t-systems-sfr.com/unix/src/privat2/wzdftpd-0.8.2.tar.gz:a/wzdftpd-0.8.2/cmake/FindOpenSSL.cmake
# - Try to find the OpenSSL encryption library
# Once done this will define
#
#  OpenSSL_FOUND - system has the OpenSSL library
#  OpenSSL_INCLUDE_DIR - the OpenSSL include directory
#  OpenSSL_LIBRARIES - The libraries needed to use OpenSSL  
          
          
if (OpenSSL_INCLUDE_DIR AND OpenSSL_LIBRARIES)

  # in cache already
  SET(OpenSSL_FOUND 1)

else ()

  find_path(OpenSSL_INCLUDE_DIR openssl/ssl.h
     /usr/include/
     /usr/local/include/
     $ENV{ProgramFiles}/OpenSSL/include/
     $ENV{SystemDrive}/OpenSSL/include/
     ${OpenSSL_DIR}/include/
  )  

  if(WIN32 AND MSVC)
      # /MD and /MDd are the standard values - if somone wants to use
      # others, the libnames have to change here too
      # see http://www.openssl.org/support/faq.html#PROG2 for their meaning

      find_library(SSL_EAY_DEBUG NAMES ssleay32MDd
        PATHS
          $ENV{ProgramFiles}/OpenSSL/lib/VC/
          $ENV{SystemDrive}/OpenSSL/lib/VC/
          ${OpenSSL_DIR}/lib/VC/
      )
      find_library(SSL_EAY_RELEASE NAMES ssleay32MD
        PATHS
          $ENV{ProgramFiles}/OpenSSL/lib/VC/
          $ENV{SystemDrive}/OpenSSL/lib/VC/
          ${OpenSSL_DIR}/lib/VC/
      )
      find_library(LIB_EAY_DEBUG NAMES libeay32MDd
        PATHS
          $ENV{ProgramFiles}/OpenSSL/lib/VC/
          $ENV{SystemDrive}/OpenSSL/lib/VC/
          ${OpenSSL_DIR}/lib/VC/
      )
      find_library(LIB_EAY_RELEASE NAMES libeay32MD
        PATHS
          $ENV{ProgramFiles}/OpenSSL/lib/VC/
          $ENV{SystemDrive}/OpenSSL/lib/VC/
          ${OpenSSL_DIR}/lib/VC/
      )

      IF(MSVC_IDE)
        IF(SSL_EAY_DEBUG AND SSL_EAY_RELEASE)
          SET(OpenSSL_LIBRARIES optimized ${SSL_EAY_RELEASE} ${LIB_EAY_RELEASE} debug ${SSL_EAY_DEBUG} ${LIB_EAY_DEBUG})
        ELSE(SSL_EAY_DEBUG AND SSL_EAY_RELEASE)
          MESSAGE(FATAL_ERROR "Could not find the debug and release version of openssl")   
        ENDIF(SSL_EAY_DEBUG AND SSL_EAY_RELEASE)
      ELSE(MSVC_IDE)
        STRING(TOLOWER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_TOLOWER)
        IF(CMAKE_BUILD_TYPE_TOLOWER MATCHES debug)
          SET(OpenSSL_LIBRARIES ${SSL_EAY_DEBUG} ${LIB_EAY_DEBUG})
        ELSE(CMAKE_BUILD_TYPE_TOLOWER MATCHES debug)
          SET(OpenSSL_LIBRARIES ${SSL_EAY_RELEASE} ${LIB_EAY_RELEASE})
        ENDIF(CMAKE_BUILD_TYPE_TOLOWER MATCHES debug)
      ENDIF(MSVC_IDE)
      mark_as_advanced(SSL_EAY_DEBUG SSL_EAY_RELEASE LIB_EAY_DEBUG LIB_EAY_RELEASE)
  else(WIN32 AND MSVC)

   find_library(OpenSSL_LIBRARIES NAMES ssl ssleay32 ssleay32MD libeay32 libeay32MD
      PATHS
        /usr/lib
        /usr/local/lib
   )
   find_library(CRYPTO_LIBRARIES crypto
      PATHS
        /usr/lib
        /usr/local/lib
   )

  endif(WIN32 AND MSVC)

  if (OpenSSL_INCLUDE_DIR AND OpenSSL_LIBRARIES)
     set(OpenSSL_FOUND 1)
  endif (OpenSSL_INCLUDE_DIR AND OpenSSL_LIBRARIES)

  if (CRYPTO_LIBRARIES)
     set(CRYPTO_FOUND 1)
     message(STATUS "Found libcrypto: ${CRYPTO_LIBRARIES}")
  endif()

  mark_as_advanced(OpenSSL_INCLUDE_DIR OpenSSL_LIBRARIES)

endif()

  
#MESSAGE( "OpenSSL_LIBRARIES: ${OpenSSL_FOUND}" )
#MESSAGE( "OpenSSL_LIBRARIES: ${OpenSSL_INCLUDE_DIR}" )
#MESSAGE( "OpenSSL_LIBRARIES: ${OpenSSL_LIBRARY_DIR}" )
#MESSAGE( "OpenSSL_LIBRARIES: ${OpenSSL_LIBRARIES}" )


# ----------------------------------------------------------------------
# Display status
# ----------------------------------------------------------------------
if(OpenSSL_FOUND)
   if(NOT OpenSSL_FIND_QUIETLY)
      message(STATUS "Found OpenSSL: \n\tDIR: ${OpenSSL_INCLUDE_DIR} \n\tLIBS: ${OpenSSL_LIBRARIES}")
   endif()
else()
   if(OpenSSL_FIND_REQUIRED)
      message(FATAL_ERROR "OpenSSL was not found.")
   endif()
endif()


# ----------------------------------------------------------------------
# Include and expose to LibSourcey
# ----------------------------------------------------------------------
if(OpenSSL_FOUND)
  include_directories(${OpenSSL_INCLUDE_DIR})  
  link_directories(${OpenSSL_LIBRARY_DIR})
                        
  set(LibSourcey_INCLUDE_DIRS ${LibSourcey_INCLUDE_DIRS} ${OpenSSL_INCLUDE_DIR})
  set(LibSourcey_LIBRARY_DIRS ${LibSourcey_LIBRARY_DIRS} ${OpenSSL_LIBRARY_DIR})
  set(LibSourcey_RELEASE_LIBS ${LibSourcey_RELEASE_LIBS} ${SSL_EAY_RELEASE} ${LIB_EAY_RELEASE})
  set(LibSourcey_DEBUG_LIBS   ${LibSourcey_DEBUG_LIBS}   ${SSL_EAY_DEBUG} ${LIB_EAY_DEBUG})
endif()