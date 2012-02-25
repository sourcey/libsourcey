# -*- cmake -*-

# - Find JsonCpp
# Find the JsonCpp includes and library
# This module defines
#  JsonCpp_INCLUDE_DIR, where to find json.h, etc.
#  JsonCpp_LIBRARY, the libraries needed to use JsonCpp.
#  JsonCpp_FOUND, If false, do not try to use JsonCpp.
#  also defined, but not for general use are
#  JsonCpp_LIBRARY, where to find the JsonCpp library.


# ----------------------------------------------------------------------
# Find include path
# ----------------------------------------------------------------------
FIND_PATH(JsonCpp_INCLUDE_DIR json/json.h
  ${LIBSOURCEY_3RDPARTY_SOURCE_DIR}/jsoncpp/include
  /usr/local/include
  /usr/include
)


# ----------------------------------------------------------------------
# Find library
# ----------------------------------------------------------------------
if(WIN32 AND MSVC)
  
  FIND_LIBRARY(JsonCpp_DEBUG_LIBRARY 
    NAMES 
      jsoncppd
    PATHS 
      ${LIBSOURCEY_3RDPARTY_INSTALL_DIR}/lib
      /usr/lib 
      /usr/local/lib
    )
  FIND_LIBRARY(JsonCpp_RELEASE_LIBRARY 
    NAMES 
      jsoncpp
    PATHS 
      ${LIBSOURCEY_3RDPARTY_INSTALL_DIR}/lib
      /usr/lib 
      /usr/local/lib
    )
    
  IF(JsonCpp_DEBUG_LIBRARY OR JsonCpp_RELEASE_LIBRARY)
    IF(MSVC_IDE)
      SET(JsonCpp_LIBRARY "")
      IF(JsonCpp_DEBUG_LIBRARY)
        SET(JsonCpp_LIBRARY ${JsonCpp_LIBRARY} debug ${JsonCpp_DEBUG_LIBRARY})
      ENDIF()
      IF(JsonCpp_RELEASE_LIBRARY)     
        SET(JsonCpp_LIBRARY ${JsonCpp_LIBRARY} optimized ${JsonCpp_RELEASE_LIBRARY})
      ENDIF()
    ELSE(MSVC_IDE)
      STRING(TOLOWER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_TOLOWER)
      IF(CMAKE_BUILD_TYPE_TOLOWER MATCHES debug)
        SET(JsonCpp_LIBRARY ${JsonCpp_DEBUG_LIBRARY})
      ELSE()
        SET(JsonCpp_LIBRARY ${JsonCpp_RELEASE_LIBRARY})
      ENDIF()
    ENDIF(MSVC_IDE)
    #mark_as_advanced(JsonCpp_DEBUG_LIBRARY JsonCpp_RELEASE_LIBRARY)
  ENDIF()

ELSE()

  # TODO: jsoncpp lib names for various systems
  SET(JsonCpp_LIB_NAMES "jsoncpp")

  if (CMAKE_COMPILER_IS_GNUCXX)
    # Get the GCC compiler version
    execute_process(${CMAKE_CXX_COMPILER}
                    ARGS ${CMAKE_CXX_COMPILER_ARG1} -dumpversion
                    OUTPUT_VARIABLE _gcc_COMPILER_VERSION
                    OUTPUT_STRIP_TRAILING_WHITESPACE)

    SET(JsonCpp_LIB_NAMES ${JsonCpp_LIB_NAMES} libjson_linux-gcc-${_gcc_COMPILER_VERSION}_libmt.so)
  endif()

  FIND_LIBRARY(JsonCpp_LIBRARY 
    NAMES 
      ${JsonCpp_LIB_NAMES}
    PATHS 
      ${LIBSOURCEY_3RDPARTY_INSTALL_DIR}/lib
      /usr/lib 
      /usr/local/lib
    )

ENDIF()

IF(JsonCpp_LIBRARY AND JsonCpp_INCLUDE_DIR)
  SET(JsonCpp_FOUND 1)
ELSE()
  SET(JsonCpp_FOUND 0)
ENDIF()

get_filename_component(JsonCpp_LIBRARY_DIR ${JsonCpp_LIBRARY} PATH)
get_filename_component(JsonCpp_LIBRARY ${JsonCpp_LIBRARY} NAME)
  
  
# ----------------------------------------------------------------------
# Display status
# ----------------------------------------------------------------------
if(JsonCpp_FOUND)
   if(NOT JsonCpp_FIND_QUIETLY)
      message(STATUS "Found JsonCpp: \n\tDir: ${JsonCpp_INCLUDE_DIR} \n\tLibs: ${JsonCpp_LIBRARY}")
   endif (NOT JsonCpp_FIND_QUIETLY)
else()
   if(JsonCpp_FIND_REQUIRED)
      message(FATAL_ERROR "JsonCpp was not found.")
   endif()
endif()


# ----------------------------------------------------------------------
# Include and expose to LibSourcey
# ----------------------------------------------------------------------
if(JsonCpp_FOUND) 
#message(STATUS "LIBSOURCEY_3RDPARTY_BUILD_DIR: ${LIBSOURCEY_3RDPARTY_BUILD_DIR}")
#message(STATUS "LIBSOURCEY_3RDPARTY_SOURCE_DIR: ${LIBSOURCEY_3RDPARTY_SOURCE_DIR}")
message(STATUS "JsonCpp_INCLUDE_DIR: ${JsonCpp_INCLUDE_DIR}")
message(STATUS "JsonCpp_LIBRARY: ${JsonCpp_LIBRARY}")
message(STATUS "JsonCpp_FOUND: ${JsonCpp_FOUND}")

  include_directories(${JsonCpp_INCLUDE_DIR})  
  link_directories(${JsonCpp_LIBRARY_DIR})
                        
  set(LIBSOURCEY_INCLUDE_DIRS ${LIBSOURCEY_INCLUDE_DIRS} ${JsonCpp_INCLUDE_DIR})
  set(LIBSOURCEY_LIBRARY_DIRS ${LIBSOURCEY_LIBRARY_DIRS} ${JsonCpp_LIBRARY_DIR})
  if(JsonCpp_DEBUG_LIBRARY) 
    set(LIBSOURCEY_DEBUG_LIBS   ${LIBSOURCEY_DEBUG_LIBS}   ${JsonCpp_DEBUG_LIBRARY})
  endif()
  if(JsonCpp_RELEASE_LIBRARY) 
    set(LIBSOURCEY_RELEASE_LIBS ${LIBSOURCEY_RELEASE_LIBS} ${JsonCpp_RELEASE_LIBRARY})
  endif()
endif()