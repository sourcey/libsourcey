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
  ${LibSourcey_3RDPARTY_SOURCE_DIR}/RtAudio
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
    ${LibSourcey_3RDPARTY_INSTALL_DIR}/lib
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
if(NOT RtAudio_FOUND)
   if(RtAudio_FIND_REQUIRED)
      message(FATAL_ERROR "RtAudio was not found.")
   endif()
endif()