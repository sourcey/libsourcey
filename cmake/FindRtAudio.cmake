########################################################################
# CMake module for finding RtAudio
#
# The following variables will be defined:
#
#  RTAUDIO_FOUND
#  RTAUDIO_INCLUDE_DIR
#  RTAUDIO_LIBRARY
#

find_path(RTAUDIO_INCLUDE_DIR
	NAMES RtAudio.h
	PATHS ${LibSourcey_SOURCE_DIR}/media/vendor/rtaudio)

find_library(RTAUDIO_LIBRARY
	NAMES rtaudio librtaudio
    PATHS ${LibSourcey_BUILD_DIR}/src/media/vendor/rtaudio)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(RTAUDIO DEFAULT_MSG RTAUDIO_LIBRARY RTAUDIO_INCLUDE_DIR)
