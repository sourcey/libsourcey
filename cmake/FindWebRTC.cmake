########################################################################
# CMake module for finding WEBRTC
#
# IMPORTANT: This script only supports WebRTC built with the webrtcbuilds
# project: https://github.com/vsimon/webrtcbuilds
#
# The following variables will be defined:
#
#  WEBRTC_FOUND
#  WEBRTC_INCLUDE_DIR
#  WEBRTC_LIBRARY
#  WEBRTC_DEPENDENCIES
#

set(_WEBRTC_ROOT_HINTS /home/kam/sourcey/webrtcbuilds/out/src)
set(_WEBRTC_SUFFIX_DEBUG out/Debug)
set(_WEBRTC_SUFFIX_RELEASE out/Release)
# set(_WEBRTC_DEPENDENCY_EXCLUDES " ") #protobuf_full|third_party|_proto|rtc_task_queue|.so

unset(WEBRTC_INCLUDE_DIR)
unset(WEBRTC_INCLUDE_DIR CACHE)
unset(WEBRTC_LIBRARY)
unset(WEBRTC_LIBRARY CACHE)
unset(WEBRTC_LIBRARY_DEBUG)
unset(WEBRTC_LIBRARY_DEBUG CACHE)
unset(WEBRTC_LIBRARY_RELEASE)
unset(WEBRTC_LIBRARY_RELEASE CACHE)

# ----------------------------------------------------------------------
# Find WEBRTC include path
# ----------------------------------------------------------------------
# unset(WEBRTC_INCLUDE_DIR CACHE)
find_path(WEBRTC_INCLUDE_DIR
  NAMES
    webrtc/config.h
  PATHS
    ${_WEBRTC_ROOT_HINTS}
)

# ----------------------------------------------------------------------
# Find WEBRTC libraries
# ----------------------------------------------------------------------
if(WIN32 AND MSVC)
  set(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} Secur32.lib Winmm.lib msdmo.lib dmoguids.lib wmcodecdspuuid.lib") # strmbase.lib
endif()

sourcey_find_library(WEBRTC
  NAMES webrtc_full
  PATHS_DEBUG ${WEBRTC_INCLUDE_DIR}/${_WEBRTC_SUFFIX_DEBUG}
  PATHS_RELEASE ${WEBRTC_INCLUDE_DIR}/${_WEBRTC_SUFFIX_RELEASE}
)

# DEPRECATED: Old script stopped working with the introduction of WebRTC GN
# build system. Use webrtcbuilds instead.
# if(WEBRTC_INCLUDE_DIR)
#   if(WIN32 AND MSVC)
#     set(lib_suffix "lib")
#
#     # Add required system libraries for win32
#     set(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} Secur32.lib Winmm.lib msdmo.lib dmoguids.lib wmcodecdspuuid.lib") # strmbase.lib
#   else()
#     set(lib_suffix "a")
#   endif()
#
#   find_path(WEBRTC_LIBRARY_DEBUG_BUILD_SCRIPT
#     NAMES build.ninja
#     PATHS ${WEBRTC_INCLUDE_DIR}${_WEBRTC_SUFFIX_DEBUG}
#   )
#   if (WEBRTC_LIBRARY_DEBUG_BUILD_SCRIPT)
#     get_filename_component(debug_dir ${WEBRTC_LIBRARY_DEBUG_BUILD_SCRIPT} PATH)
#     file(GLOB_RECURSE debug_libs "${debug_dir}/*.${lib_suffix}")
#     foreach(lib ${debug_libs})
#       if(NOT ${lib} MATCHES ${_WEBRTC_DEPENDENCY_EXCLUDES})
#         list(APPEND WEBRTC_LIBRARY_DEBUG ${lib})
#       endif()
#     endforeach()
#   endif()
#
#   find_path(WEBRTC_LIBRARY_RELEASE_BUILD_SCRIPT
#     NAMES build.ninja
#     PATHS ${WEBRTC_INCLUDE_DIR}${_WEBRTC_SUFFIX_RELEASE}
#   )
#   if (WEBRTC_LIBRARY_RELEASE_BUILD_SCRIPT)
#     get_filename_component(release_dir ${WEBRTC_LIBRARY_RELEASE_BUILD_SCRIPT} PATH)
#     file(GLOB_RECURSE release_libs "${release_dir}/*.${lib_suffix}")
#     foreach(lib ${release_libs})
#       if(NOT ${lib} MATCHES ${_WEBRTC_DEPENDENCY_EXCLUDES})
#         list(APPEND WEBRTC_LIBRARY_RELEASE ${lib})
#       endif()
#     endforeach()
#   endif()
#
#   mark_as_advanced(WEBRTC_LIBRARY WEBRTC_INCLUDE_DIR
#     WEBRTC_LIBRARY_DEBUG WEBRTC_LIBRARY_RELEASE
#     WEBRTC_LIBRARY_DEBUG_BUILD_SCRIPT WEBRTC_LIBRARY_RELEASE_BUILD_SCRIPT)
#
#   include(${CMAKE_ROOT}/Modules/SelectLibraryConfigurations.cmake)
#   select_library_configurations(WEBRTC)
#
#   # print_module_variables(WEBRTC)
# endif()

# ----------------------------------------------------------------------
# Display status
# ----------------------------------------------------------------------
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(WEBRTC DEFAULT_MSG WEBRTC_LIBRARY WEBRTC_INCLUDE_DIR)

# if(NOT WEBRTC_FOUND)
#   if(WEBRTC_FIND_REQUIRED)
#     message(FATAL_ERROR "WEBRTC was not found. Please specify the path manually.")
#   endif()
# endif()


# i420
#sourcey_find_library(WEBRTC_i420
#  NAMES webrtc_i420
#  PATHS_DEBUG ${WEBRTC_PATHS_DEBUG}
#  PATHS_RELEASE ${WEBRTC_PATHS_RELEASE})
#if(WEBRTC_i420_LIBRARY)
#  list(APPEND WEBRTC_DEPENDENCIES ${WEBRTC_i420_LIBRARY})
#endif()

# opus
#sourcey_find_library(WEBRTC_opus
#  NAMES webrtc_opus
#  PATHS_DEBUG ${WEBRTC_PATHS_DEBUG}
#  PATHS_RELEASE ${WEBRTC_PATHS_RELEASE})
#if(WEBRTC_opus_LIBRARY)
#  list(APPEND WEBRTC_DEPENDENCIES ${WEBRTC_opus_LIBRARY})
#endif()

# utility
#sourcey_find_library(WEBRTC_utility
#  NAMES webrtc_utility
#  PATHS_DEBUG ${WEBRTC_PATHS_DEBUG}
#  PATHS_RELEASE ${WEBRTC_PATHS_RELEASE})
#if(WEBRTC_utility_LIBRARY)
#  list(APPEND WEBRTC_DEPENDENCIES ${WEBRTC_utility_LIBRARY})
#endif()

# video_coding
#sourcey_find_library(WEBRTC_video_coding
#  NAMES webrtc_video_coding
#  PATHS_DEBUG ${WEBRTC_PATHS_DEBUG}
#  PATHS_RELEASE ${WEBRTC_PATHS_RELEASE})
#if(WEBRTC_video_coding_LIBRARY)
#  list(APPEND WEBRTC_DEPENDENCIES ${WEBRTC_video_coding_LIBRARY})
#endif()

# vp8
#sourcey_find_library(WEBRTC_vp8
#  NAMES webrtc_vp8
#  PATHS_DEBUG ${WEBRTC_PATHS_DEBUG}
#  PATHS_RELEASE ${WEBRTC_PATHS_RELEASE})
#if(WEBRTC_vp8_LIBRARY)
#  list(APPEND WEBRTC_DEPENDENCIES ${WEBRTC_vp8_LIBRARY})
#endif()

# libjingle
#sourcey_find_library(WEBRTC_libjingle
#  NAMES libjingle
#  PATHS_DEBUG ${WEBRTC_PATHS_DEBUG}
#  PATHS_RELEASE ${WEBRTC_PATHS_RELEASE})
#if(WEBRTC_libjingle_LIBRARY)
#  list(APPEND WEBRTC_DEPENDENCIES ${WEBRTC_libjingle_LIBRARY})
#endif()

# libjingle_p2p
#sourcey_find_library(WEBRTC_libjingle_p2p
#  NAMES libjingle_p2p
#  PATHS_DEBUG ${WEBRTC_PATHS_DEBUG}
#  PATHS_RELEASE ${WEBRTC_PATHS_RELEASE})
#if(WEBRTC_libjingle_p2p_LIBRARY)
#  list(APPEND WEBRTC_DEPENDENCIES ${WEBRTC_libjingle_p2p_LIBRARY})
#endif()

# libjingle_peerconnection
#sourcey_find_library(WEBRTC_libjingle_peerconnection
#  NAMES libjingle_peerconnection
#  PATHS_DEBUG ${WEBRTC_PATHS_DEBUG}
#  PATHS_RELEASE ${WEBRTC_PATHS_RELEASE})
#if(WEBRTC_libjingle_peerconnection_LIBRARY)
#  list(APPEND WEBRTC_DEPENDENCIES ${WEBRTC_libjingle_peerconnection_LIBRARY})
#endif()
