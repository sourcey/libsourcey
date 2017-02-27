########################################################################
# CMake module for finding WEBRTC
#
# For compatability WebRTC must be build without jsoncpp, and must not define
# _GLIBCXX_DEBUG=1 or -fno-rtti
#
# The best way to build WebRTC is with the forked `webrtcbuilds` project here:
# https://github.com/sourcey/webrtcbuilds
#
# The following variables will be defined:
#
#  WEBRTC_FOUND
#  WEBRTC_INCLUDE_DIR
#  WEBRTC_LIBRARIES
#  WEBRTC_DEPENDENCIES
#

# unset(WEBRTC_INCLUDE_DIR)
# unset(WEBRTC_INCLUDE_DIR CACHE)
# unset(WEBRTC_LIBRARIES)
# unset(WEBRTC_LIBRARIES CACHE)
# unset(WEBRTC_LIBRARIES_DEBUG)
# unset(WEBRTC_LIBRARIES_DEBUG CACHE)
# unset(WEBRTC_LIBRARIES_RELEASE)
# unset(WEBRTC_LIBRARIES_RELEASE CACHE)

# Set required variables
set(WEBRTC_ROOT_DIR "" CACHE STRING "Where is the WebRTC root directory located?")
set(WEBRTC_BUILD_DIR_SUFFIX_DEBUG "out/Debug" CACHE STRING "What is the WebRTC debug build directory suffix?")
set(WEBRTC_BUILD_DIR_SUFFIX_RELEASE "out/Release" CACHE STRING "What is the WebRTC release build directory suffix?")

# set(_WEBRTC_DEPENDENCY_EXCLUDES "jsoncpp")


# ----------------------------------------------------------------------
# Find WEBRTC include path
# ----------------------------------------------------------------------
find_path(WEBRTC_INCLUDE_DIR
  NAMES
    webrtc/config.h
  PATHS
    ${WEBRTC_ROOT_DIR}
    $ENV{HOME}/tmp/webrtcbuilds/out/src
    $ENV{HOME}/sourcey/webrtcbuilds/out/src
)

# ----------------------------------------------------------------------
# Find WEBRTC libraries
# ----------------------------------------------------------------------
if(WEBRTC_INCLUDE_DIR)
  get_filename_component(debug_dir "${WEBRTC_ROOT_DIR}/${WEBRTC_BUILD_DIR_SUFFIX_DEBUG}" PATH)
  get_filename_component(release_dir "${WEBRTC_ROOT_DIR}/${WEBRTC_BUILD_DIR_SUFFIX_RELEASE}" PATH)

  # Attempt to find the monolithic library built with `webrtcbuilds`
  find_library_extended(WEBRTC
    NAMES webrtc_full
    PATHS_DEBUG ${debug_dir}
    PATHS_RELEASE ${release_dir}
  )

  # # Otherwise generate a library from available .o objects
  # if(NOT WEBRTC_LIBRARY)
  #   if(EXISTS ${_WEBRTC_BUILD_DIR_DEBUG})
  #     message("Generating WebRTC Debug library")
  #     set(lib_cmd "${LibSourcey_DIR}/cmake/webrtc_lib_generator.sh" -o ${_WEBRTC_BUILD_DIR_DEBUG} -x ${_WEBRTC_GENERATOR_EXCLUDES})
  #     execute_process(COMMAND ${lib_cmd})
  #   endif()
  #
  #   if(EXISTS ${_WEBRTC_BUILD_DIR_RELEASE})
  #     message("Generating WebRTC Release library")
  #     set(lib_cmd "${LibSourcey_DIR}/cmake/webrtc_lib_generator.sh" -o ${_WEBRTC_BUILD_DIR_RELEASE} -x ${_WEBRTC_GENERATOR_EXCLUDES})
  #     execute_process(COMMAND ${lib_cmd})
  #   endif()
  #
  #   # Attempt to find our library again...
  #   find_library_extended(WEBRTC
  #     NAMES webrtc_scy
  #     PATHS_DEBUG ${_WEBRTC_BUILD_DIR_DEBUG}
  #     PATHS_RELEASE ${_WEBRTC_BUILD_DIR_RELEASE}
  #   )
  # endif()

  # Otherwise fallback to library objects (slower and unreliable)
  if(NOT WEBRTC_LIBRARY)
    set(WEBRTC_LIBRARIES "")
    if(MSVC)
      set(lib_suffix "lib")
    else()
      set(lib_suffix "a")
    endif()

    # Debug
    if (EXISTS ${debug_dir})
      file(GLOB_RECURSE debug_libs "${debug_dir}/*.${lib_suffix}")
      foreach(lib ${debug_libs})
        # if(${lib} NOT MATCHES ${_WEBRTC_DEPENDENCY_EXCLUDES})
          list(APPEND WEBRTC_LIBRARIES_DEBUG ${lib})
        # endif()
      endforeach()
      foreach(lib ${WEBRTC_LIBRARIES_DEBUG})
        if(WIN32 AND (CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE))
          list(APPEND WEBRTC_LIBRARIES "debug" ${lib}) 
        else()
          list(APPEND WEBRTC_LIBRARIES ${lib})
        endif()
      endforeach()
    endif()

    # Release
    if (EXISTS ${release_dir})
      file(GLOB_RECURSE release_libs "${release_dir}/*.${lib_suffix}")
      foreach(lib ${release_libs})
        # if(${lib} NOT MATCHES ${_WEBRTC_DEPENDENCY_EXCLUDES})
          list(APPEND WEBRTC_LIBRARIES_RELEASE ${lib})
        # endif()
      endforeach()
      foreach(lib ${WEBRTC_LIBRARIES_RELEASE})
        if(WIN32 AND (CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE))
          list(APPEND WEBRTC_LIBRARIES "optimized" ${lib}) 
        else()
          list(APPEND WEBRTC_LIBRARIES ${lib})
        endif()
      endforeach()
    endif()
  endif()

  # Add required system libraries
  if(CMAKE_SYSTEM_NAME STREQUAL "Windows" AND MSVC)
    add_definitions(-DWEBRTC_WIN)
    set(WEBRTC_DEPENDENCIES Secur32.lib Winmm.lib msdmo.lib dmoguids.lib wmcodecdspuuid.lib) # strmbase.lib
  elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_definitions(-DWEBRTC_POSIX)
    set(WEBRTC_DEPENDENCIES -lX11 -lrt -lGLU -lGL)

    # Enable libstdc++ debugging if you build WebRTC with `enable_iterator_debugging=true`
    # set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_GLIBCXX_DEBUG=1")
  endif()

  # include(${CMAKE_ROOT}/Modules/SelectLibraryConfigurations.cmake)
  # select_library_configurations(WEBRTC)
  # message("WEBRTC_LIBRARIES: ${WEBRTC_LIBRARIES}")

  # HACK: WEBRTC_LIBRARIES and WEBRTC_DEPENDENCIES not propagating to parent scope
  # while the WEBRTC_DEBUG_LIBRARY and WEBRTC_RELEASE_LIBRARY vars are.
  # Setting PARENT_SCOPE fixes this solves theis issue for now.
  set(WEBRTC_LIBRARIES ${WEBRTC_LIBRARIES} PARENT_SCOPE)
  set(WEBRTC_DEPENDENCIES ${WEBRTC_DEPENDENCIES} PARENT_SCOPE)
endif()

# print_module_variables(WEBRTC)

# ----------------------------------------------------------------------
# Display status
# ----------------------------------------------------------------------
include(FindPackageHandleStandardArgs)
if(WIN32 AND (CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE))
  find_package_handle_standard_args(WEBRTC DEFAULT_MSG WEBRTC_LIBRARIES WEBRTC_INCLUDE_DIR)
else()
  find_package_handle_standard_args(WEBRTC DEFAULT_MSG WEBRTC_LIBRARY WEBRTC_INCLUDE_DIR)
endif()

mark_as_advanced(WEBRTC_LIBRARIES WEBRTC_LIBRARY WEBRTC_INCLUDE_DIR
                 WEBRTC_LIBRARIES_DEBUG WEBRTC_LIBRARIES_RELEASE
                 WEBRTC_BUILD_DIR_SUFFIX_DEBUG WEBRTC_BUILD_DIR_SUFFIX_RELEASE 
                 WEBRTC_DEPENDENCIES)