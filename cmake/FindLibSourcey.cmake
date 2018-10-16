# - Try to find the required LibSourcey components (default: base uv crypto net util)
#
# Once done this will define
#  LibSourcey_FOUND         - System has the all required components.
#  LibSourcey_INCLUDE_DIRS  - Include directory necessary for using the required components headers.
#  LibSourcey_LIBRARY_DIRS  - Library directories necessary for using the required components.
#  LibSourcey_LIBRARIES     - Link these to use the required components.
#  LibSourcey_DEFINITIONS   - Compiler switches required for using the required components.
#
# the following variables will be defined
#  LibSourcey_<component>_FOUND        - System has <component>
#  LibSourcey_<component>_INCLUDE_DIRS - Include directories necessary for using the <component> headers
#  LibSourcey_<component>_LIBRARY_DIRS - Library directories necessary for using the <component>
#  LibSourcey_<component>_LIBRARIES    - Link these to use <component>
#  LibSourcey_<component>_DEFINITIONS  - Compiler switches required for using <component>
#  LibSourcey_<component>_VERSION      - The components version

# Set required variables
set(LibSourcey_ROOT_DIR "" CACHE STRING "Where is the LibSourcey root directory located?")
set(LibSourcey_INCLUDE_DIR "${LibSourcey_ROOT_DIR}/src" CACHE STRING "Where are the LibSourcey headers (.h) located?")
set(LibSourcey_LIBRARY_DIR "${LibSourcey_ROOT_DIR}/build" CACHE STRING "Where are the LibSourcey libraries (.dll/.so) located?") #/src

# Set path variables (used in .cmake files)
set(LibSourcey_DIR ${LibSourcey_ROOT_DIR})
set(LibSourcey_SOURCE_DIR ${LibSourcey_DIR}/src)
set(LibSourcey_BUILD_DIR ${LibSourcey_DIR}/build)
# set(LibSourcey_DIR ${LibSourcey_ROOT_DIR} PARENT_SCOPE)
# set(LibSourcey_SOURCE_DIR ${LibSourcey_DIR}/src PARENT_SCOPE)
# set(LibSourcey_BUILD_DIR ${LibSourcey_DIR}/build PARENT_SCOPE)

# message("LibSourcey_SOURCE_DIR=${LibSourcey_SOURCE_DIR}")
# message("LibSourcey_ROOT_DIR=${LibSourcey_ROOT_DIR}")
# message("LibSourcey_LIBRARY_DIR=${LibSourcey_ROOT_DIR}")
# message("LibSourcey_INCLUDE_DIR=${LibSourcey_INCLUDE_DIR}")

include(CMakeHelpers REQUIRED)
include(CMakeFindExtensions REQUIRED)

# The default components to find
if (NOT LibSourcey_FIND_COMPONENTS)
  set(LibSourcey_FIND_COMPONENTS base uv crypto net util)
endif()

# Set a list of all available modules
set(LibSourcey_ALL_MODULES
  archo
  av
  base
  crypto
  http
  json
  net
  pacm
  pluga
  sked
  socketio
  stun
  symple
  turn
  util
  webrtc
)

# Check for cached results. If there are then skip the costly part.
# set_module_notfound(LibSourcey)
if (NOT LibSourcey_FOUND)
  if(WIN32 AND MSVC)
    set(LibSourcey_MULTI_CONFIGURATION TRUE)
  endif()

  # Set search path suffixes
  foreach(component ${LibSourcey_FIND_COMPONENTS})
    list(APPEND LibSourcey_INCLUDE_SUFFIXES ${component}/include)
    list(APPEND LibSourcey_LIBRARY_SUFFIXES ${component})
    list(APPEND LibSourcey_LIBRARY_SUFFIXES ${component}/Release)
    list(APPEND LibSourcey_LIBRARY_SUFFIXES ${component}/Debug)
  endforeach()

  # Check for all available components
  find_component(LibSourcey pacm     pacm     scy_pacm     scy/pacm/config.h)
  find_component(LibSourcey pluga    pluga    scy_pluga    scy/pluga/config.h)
  find_component(LibSourcey sked     sked     scy_sked     scy/sked/scheduler.h)
  find_component(LibSourcey webrtc   webrtc   scy_webrtc   scy/webrtc/webrtc.h)
  find_component(LibSourcey turn     turn     scy_turn     scy/turn/turn.h)
  find_component(LibSourcey stun     stun     scy_stun     scy/stun/stun.h)
  find_component(LibSourcey symple   symple   scy_symple   scy/symple/client.h)
  find_component(LibSourcey socketio socketio scy_socketio scy/socketio/client.h)
  find_component(LibSourcey av       av       scy_av       scy/av/ffmpeg.h)
  find_component(LibSourcey util     util     scy_util     scy/util/ratelimiter.h)
  find_component(LibSourcey http     http     scy_http     scy/http/server.h)
  find_component(LibSourcey net      net      scy_net      scy/net/socket.h)
  find_component(LibSourcey archo    archo    scy_archo    scy/archo/zipfile.h)
  find_component(LibSourcey crypto   crypto   scy_crypto   scy/crypto/crypto.h)
  find_component(LibSourcey json     json     scy_json     scy/json/json.h)
  find_component(LibSourcey base     base     scy_base     scy/base.h)

  # Include the dir with libsourcey.h
  list(APPEND LibSourcey_INCLUDE_DIRS ${LibSourcey_LIBRARY_DIR})

  # Include dependency headers
  list(APPEND LibSourcey_INCLUDE_DIRS
    ${LibSourcey_DIR}/share/libsourcey/vendor/include
    ${LibSourcey_DIR}/vendor/zlib
    ${LibSourcey_DIR}/vendor/zlib
    ${LibSourcey_DIR}/vendor/minizip
    # ${LibSourcey_DIR}/vendor/rtaudio
    # ${LibSourcey_DIR}/vendor/rtaudio/include
    ${LibSourcey_DIR}/vendor/libuv/include
    ${LibSourcey_DIR}/vendor/http_parser
    # ${LibSourcey_DIR}/vendor/jsoncpp
    ${LibSourcey_DIR}/vendor/json/src)

  # Set LibSourcey as found or not
  # print_module_variables(LibSourcey)
  set_module_found(LibSourcey)
endif()

# Include dependencies if found
if (LibSourcey_FOUND)

	# Add required system libraries
	if(MSVC)
	  set(LibSourcey_DEPENDENCIES advapi32.lib iphlpapi.lib psapi.lib shell32.lib ws2_32.lib dsound.lib winmm.lib strmiids.lib)
	elseif(APPLE)
	  set(LibSourcey_DEPENDENCIES -ldl)
	elseif(UNIX)
	  set(LibSourcey_DEPENDENCIES -lm -lrt -ldl)
	endif()

	# Third party dependencies
  find_package(Threads REQUIRED)
  find_package(LibUV REQUIRED)
  find_package(HttpParser REQUIRED)
  # find_package(SSL REQUIRED)

  list(APPEND LibSourcey_INCLUDE_DIRS ${LIBUV_INCLUDE_DIR} ${HTTPPARSER_INCLUDE_DIR})
  list(APPEND LibSourcey_DEPENDENCIES ${LIBUV_LIBRARIES} ${HTTPPARSER_LIBRARIES})

	# This is necessary on Linux as well as Threads package
	if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
	endif()
endif()
