# ----------------------------------------------------------------------------
#  CMake file for RtAudio.
# ----------------------------------------------------------------------------

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(rtaudio_SOURCE_PATH *.cpp)
set(rtaudio_HEADER_PATH *.h)

# TODO: Proper audio subsystem selection
#__UNIX_JACK__
#__LINUX_ALSA__
#__LINUX_OSS__
#__WINDOWS_ASIO__
#__WINDOWS_DS__
#__MACOSX_CORE__
#__RTAUDIO_DUMMY__

if(WIN32)
  add_definitions(-D__WINDOWS_DS__)
elseif(APPLE)
  add_definitions(-D__MACOSX_CORE__)
else()
  add_definitions(-D__UNIX_JACK__)
endif()

define_sourcey_dependency(rtaudio)