# ----------------------------------------------------------------------------
#  CMake file for samples. See root CMakeLists.txt 
#
# ----------------------------------------------------------------------------

add_subdirectory(DeviceEnumerator)
add_subdirectory(VideoSocket)
if(BUILD_ALPHA)
  #add_subdirectory(WaveformOutput)
endif()
