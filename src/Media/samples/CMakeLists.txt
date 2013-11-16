# ----------------------------------------------------------------------------
#  CMake file for samples. See root CMakeLists.txt 
#
# ----------------------------------------------------------------------------

add_subdirectory(deviceenumerator)
add_subdirectory(videosocket)
if(BUILD_ALPHA)
  #add_subdirectory(waveformoutput)
endif()
