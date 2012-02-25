########################################################################
# Cmake module for finding RtAudio
#
# The following variabled will be defined:
#
#  RtAudio
#  RtAudio_INCLUDE_DIR
#  RtAudio_LIBRARY_DIR
#  RtAudio_LIBRARY
#


# ----------------------------------------------------------------------
# Find RtAudio include path
# ----------------------------------------------------------------------
find_path(RtAudio_INCLUDE_DIR RtAudio.h
  ${LIBSOURCEY_3RDPARTY_SOURCE_DIR}/RtAudio
  /usr/local/include
  /usr/include
)


# ----------------------------------------------------------------------
# Find RtAudio library
# ----------------------------------------------------------------------
FIND_LIBRARY(RtAudio_LIBRARY 
  NAMES 
    RtAudio.lib RtAudiod.lib
  PATHS 
    ${LIBSOURCEY_3RDPARTY_INSTALL_DIR}/lib
    /usr/lib 
    /usr/local/lib
  )

if(RtAudio_LIBRARY AND RtAudio_INCLUDE_DIR)
  set(RtAudio_FOUND 1)
else()
  set(RtAudio_FOUND 0)
endif()

get_filename_component(RtAudio_LIBRARY_DIR ${RtAudio_LIBRARY} PATH)
get_filename_component(RtAudio_LIBRARY ${RtAudio_LIBRARY} NAME)
  
      
# ----------------------------------------------------------------------
# Display status
# ----------------------------------------------------------------------
if(RtAudio_FOUND)
   if(NOT RtAudio_FIND_QUIETLY)
      message(STATUS "Found RtAudio: \n\tINC DIR: ${RtAudio_INCLUDE_DIR} \n\tLIB DIR: ${RtAudio_LIBRARY_DIR} \n\tLIBS: ${RtAudio_LIBRARY}")
   endif (NOT RtAudio_FIND_QUIETLY)
else()
   if(RtAudio_FIND_REQUIRED)
      message(FATAL_ERROR "RtAudio was not found.")
   endif()
endif()

#message(STATUS "LIBSOURCEY_3RDPARTY_BUILD_DIR: ${LIBSOURCEY_3RDPARTY_BUILD_DIR}")
#message(STATUS "LIBSOURCEY_3RDPARTY_SOURCE_DIR: ${LIBSOURCEY_3RDPARTY_SOURCE_DIR}")
#message(STATUS "RtAudio_INCLUDE_DIRS: ${RtAudio_INCLUDE_DIRS}")
#message(STATUS "RtAudio_LIBRARY: ${RtAudio_LIBRARY}")
 

# ----------------------------------------------------------------------
# Include and expose to LibSourcey
# ----------------------------------------------------------------------
if(RtAudio_FOUND)
  include_directories(${RtAudio_INCLUDE_DIR})  
  link_directories(${RtAudio_LIBRARY_DIR})
                        
  set(LIBSOURCEY_INCLUDE_DIRS ${LIBSOURCEY_INCLUDE_DIRS} ${RtAudio_INCLUDE_DIR})
  set(LIBSOURCEY_LIBRARY_DIRS ${LIBSOURCEY_LIBRARY_DIRS} ${RtAudio_LIBRARY_DIR})
  set(LIBSOURCEY_DEBUG_LIBS   ${LIBSOURCEY_DEBUG_LIBS}   ${RtAudio_LIBRARY})    
  set(LIBSOURCEY_RELEASE_LIBS ${LIBSOURCEY_RELEASE_LIBS} ${RtAudio_LIBRARY})
endif()