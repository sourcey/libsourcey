########################################################################
# CMake module for finding WEBRTC
#
# For compatability WebRTC must be build without jsoncpp, and must not define
# _GLIBCXX_DEBUG=1 or -fno-rtti
#
# The best way to build WebRTC is with the forked `webrtcbuilds` project:
# https://github.com/sourcey/webrtcbuilds
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
# set(_WEBRTC_GENERATOR_EXCLUDES jsoncpp|unittest|examples|main.o)


# unset(WEBRTC_INCLUDE_DIR)
# unset(WEBRTC_INCLUDE_DIR CACHE)
# unset(WEBRTC_LIBRARY)
# unset(WEBRTC_LIBRARY CACHE)
# unset(WEBRTC_LIBRARY_DEBUG)
# unset(WEBRTC_LIBRARY_DEBUG CACHE)
# unset(WEBRTC_LIBRARY_RELEASE)
# unset(WEBRTC_LIBRARY_RELEASE CACHE)

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
if(WEBRTC_INCLUDE_DIR)

  # Attempt to find our own monolythic library
  sourcey_find_library(WEBRTC
    NAMES webrtc_full
    PATHS_DEBUG ${WEBRTC_INCLUDE_DIR}/${_WEBRTC_SUFFIX_DEBUG} #${WEBRTC_LIBRARY_DIR_DEBUG}
    PATHS_RELEASE ${WEBRTC_INCLUDE_DIR}/${_WEBRTC_SUFFIX_RELEASE} #${WEBRTC_LIBRARY_DIR_RELEASE}
  )

  # find_path(WEBRTC_LIBRARY_DIR_DEBUG
  #   NAMES build.ninja
  #   PATHS ${WEBRTC_INCLUDE_DIR}/${_WEBRTC_SUFFIX_DEBUG}
  # )
  #
  # find_path(WEBRTC_LIBRARY_DIR_RELEASE
  #   NAMES build.ninja
  #   PATHS ${WEBRTC_INCLUDE_DIR}/${_WEBRTC_SUFFIX_RELEASE}
  # )
  #
  # # Otherwise generate a library from available .o objects
  # if(NOT WEBRTC_LIBRARY)
  #   if(EXISTS ${WEBRTC_LIBRARY_DIR_DEBUG})
  #     message("Generating WebRTC Debug library")
  #     set(lib_cmd "${LibSourcey_DIR}/cmake/webrtc_lib_generator.sh" -o ${WEBRTC_LIBRARY_DIR_DEBUG} -x ${_WEBRTC_GENERATOR_EXCLUDES})
  #     execute_process(COMMAND ${lib_cmd})
  #   endif()
  #
  #   if(EXISTS ${WEBRTC_LIBRARY_DIR_RELEASE})
  #     message("Generating WebRTC Release library")
  #     set(lib_cmd "${LibSourcey_DIR}/cmake/webrtc_lib_generator.sh" -o ${WEBRTC_LIBRARY_DIR_RELEASE} -x ${_WEBRTC_GENERATOR_EXCLUDES})
  #     execute_process(COMMAND ${lib_cmd})
  #   endif()
  #
  #   # Attempt to find our library again...
  #   sourcey_find_library(WEBRTC
  #     NAMES webrtc_scy
  #     PATHS_DEBUG ${WEBRTC_LIBRARY_DIR_DEBUG}
  #     PATHS_RELEASE ${WEBRTC_LIBRARY_DIR_RELEASE}
  #   )
  # endif()

  # # Otherwise fallback to .o objects (slower)
  # if(NOT WEBRTC_INCLUDE_DIR)
  #   find_path(WEBRTC_LIBRARY_DIR_DEBUG
  #     NAMES build.ninja
  #     PATHS ${WEBRTC_INCLUDE_DIR}/${_WEBRTC_SUFFIX_DEBUG}
  #   )
  #   if (WEBRTC_LIBRARY_DIR_DEBUG)
  #     get_filename_component(debug_dir ${WEBRTC_LIBRARY_DIR_DEBUG}/ PATH)
  #     file(GLOB_RECURSE debug_libs "${debug_dir}/*.o") #${lib_suffix}
  #     foreach(lib ${debug_libs})
  #       if(${lib} MATCHES ${_WEBRTC_DEPENDENCY_EXCLUDES}) #NOT
  #         list(APPEND WEBRTC_LIBRARY_DEBUG ${lib})
  #       endif()
  #     endforeach()
  #
  #     # foreach(lib ${_WEBRTC_DEPENDENCY_EXCLUDES})
  #     #   list(APPEND WEBRTC_LIBRARY_DEBUG "${WEBRTC_LIBRARY_DIR_DEBUG}/${lib}.${lib_suffix}")
  #     # endforeach()
  #   endif()
  #
  #   find_path(WEBRTC_LIBRARY_DIR_RELEASE
  #     NAMES build.ninja
  #     PATHS ${WEBRTC_INCLUDE_DIR}/${_WEBRTC_SUFFIX_RELEASE}
  #   )
  #   if (WEBRTC_LIBRARY_DIR_RELEASE)
  #     get_filename_component(release_dir ${WEBRTC_LIBRARY_DIR_RELEASE} PATH)
  #     file(GLOB_RECURSE release_libs "${release_dir}/*.o") #${lib_suffix}
  #     foreach(lib ${release_libs})
  #       if(${lib} MATCHES ${_WEBRTC_DEPENDENCY_EXCLUDES}) #NOT
  #         list(APPEND WEBRTC_LIBRARY_RELEASE ${lib})
  #       endif()
  #     endforeach()
  #
  #     # foreach(lib ${_WEBRTC_DEPENDENCY_EXCLUDES})
  #     #   list(APPEND WEBRTC_LIBRARY_RELEASE "${WEBRTC_LIBRARY_DIR_DEBUG}/${lib}.${lib_suffix}")
  #     # endforeach()
  #   endif()
  # endif()

  # Add required system libraries
  if(CMAKE_SYSTEM_NAME STREQUAL "Windows" AND MSVC)
    add_definitions(-DWEBRTC_WINDOWS)
    set(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} Secur32.lib Winmm.lib msdmo.lib dmoguids.lib wmcodecdspuuid.lib") # strmbase.lib
  elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_definitions(-DWEBRTC_POSIX)

    # Enable libstdc++ debugging if you build WebRTC with `enable_iterator_debugging=true`
    # set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_GLIBCXX_DEBUG=1")
    set(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} -lX11 -lrt -lGLU -lGL")
  endif()

  include(${CMAKE_ROOT}/Modules/SelectLibraryConfigurations.cmake)
  select_library_configurations(WEBRTC)
endif()

# print_module_variables(WEBRTC)

# ----------------------------------------------------------------------
# Display status
# ----------------------------------------------------------------------
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(WEBRTC DEFAULT_MSG WEBRTC_LIBRARY WEBRTC_INCLUDE_DIR)

mark_as_advanced(WEBRTC_LIBRARY WEBRTC_INCLUDE_DIR
  WEBRTC_LIBRARY_DEBUG WEBRTC_LIBRARY_RELEASE
  WEBRTC_LIBRARY_DIR_DEBUG WEBRTC_LIBRARY_DIR_RELEASE)
