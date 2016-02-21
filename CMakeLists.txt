# ============================================================================
# Root CMake file for LibSourcey
# ============================================================================

cmake_minimum_required(VERSION 2.8.10)

# This _must_ go before project(LibSourcey) in order to work
if(WIN32)
  set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Installation Directory")
else()
  set(CMAKE_INSTALL_PREFIX "/usr/local" CACHE PATH "Installation Directory")
endif()

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${CMAKE_CONFIGURATION_TYPES})

project(LibSourcey)

add_definitions(-DUNICODE -D_UNICODE)

# Set warning level to W3
if(CMAKE_BUILD_TOOL MATCHES "(msdev|devenv|nmake)")
  add_definitions(/W3)
endif()

if(APPLE)
  # Remove the “MACOSX_RPATH is not specified” warning
  # set(CMAKE_MACOSX_RPATH 1)
  
  # Silence CMake warnings by adopting modern behavior for MACOSX_RPATH on newer
  # versions of CMake
  if(POLICY CMP0042)
    cmake_policy(SET CMP0042 NEW)
  endif()
  set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}")
endif()

# Tell CMake where to locate our .cmake files
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

# Include required cmake files
include(LibSourceyUtilities REQUIRED)
include(LibSourceyIncludes REQUIRED)
include(LibSourceyDependency REQUIRED)
include(LibSourceyApplication REQUIRED)
include(LibSourceyModule REQUIRED)
include(LibSourceyDetectCXXCompiler REQUIRED)
include(LibSourceyVersion REQUIRED)

# ----------------------------------------------------------------------------
# LibSourcey build components
# ----------------------------------------------------------------------------
set_option(BUILD_SHARED_LIBS          "Build shared libraries (.dll/.so) instead of static ones (.lib/.a)" NOT (WIN32 OR ANDROID OR IOS))
set_option(BUILD_DEPENDENCIES         "Build third party dependencies"                           ON)
set_option(BUILD_MODULES              "Build LibSourcey modules"                                 ON)
set_option(BUILD_APPLICATIONS         "Build LibSourcey applications"                            ON)
set_option(BUILD_MODULE_TESTS         "Build module test applications?"                          ON)
set_option(BUILD_MODULE_SAMPLES       "Build module sample applications?"                        ON)
set_option(BUILD_WITH_DEBUG_INFO      "Include debug info into debug libs"                       ON)
set_option(BUILD_WITH_STATIC_CRT      "Enables statically linked CRT for statically linked libraries" OFF)
set_option(BUILD_ALPHA                "Build alpha development modules"                          OFF)

# ----------------------------------------------------------------------------
# LibSourcey build options
# ----------------------------------------------------------------------------
set_option(ENABLE_SOLUTION_FOLDERS    "Solution folder in Visual Studio or in other IDEs"        MSVC_IDE IF (CMAKE_VERSION VERSION_GREATER "2.8.0") )
set_option(ENABLE_PROFILING           "Enable profiling in the GCC compiler (Add flags: -g -pg)" OFF  IF CMAKE_COMPILER_IS_GNUCXX )
set_option(ENABLE_OMIT_FRAME_POINTER  "Enable -fomit-frame-pointer for GCC"                      ON   IF CMAKE_COMPILER_IS_GNUCXX )
set_option(ENABLE_POWERPC             "Enable PowerPC for GCC"                                   ON   IF (CMAKE_COMPILER_IS_GNUCXX AND CMAKE_SYSTEM_PROCESSOR MATCHES powerpc.*) )
set_option(ENABLE_FAST_MATH           "Enable -ffast-math (not recommended for GCC 4.6.x)"       OFF  IF (CMAKE_COMPILER_IS_GNUCXX AND (X86 OR X86_64)) )
set_option(ENABLE_SSE                 "Enable SSE instructions"                                  ON   IF (MSVC OR CMAKE_COMPILER_IS_GNUCXX AND (X86 OR X86_64)) )
set_option(ENABLE_SSE2                "Enable SSE2 instructions"                                 ON   IF (MSVC OR CMAKE_COMPILER_IS_GNUCXX AND (X86 OR X86_64)) )
set_option(ENABLE_SSE3                "Enable SSE3 instructions"                                 OFF  IF (CV_ICC OR CMAKE_COMPILER_IS_GNUCXX AND (X86 OR X86_64)) )
set_option(ENABLE_SSSE3               "Enable SSSE3 instructions"                                OFF  IF (CMAKE_COMPILER_IS_GNUCXX AND (X86 OR X86_64)) )
set_option(ENABLE_SSE41               "Enable SSE4.1 instructions"                               OFF  IF (CV_ICC OR CMAKE_COMPILER_IS_GNUCXX AND (X86 OR X86_64)) )
set_option(ENABLE_SSE42               "Enable SSE4.2 instructions"                               OFF  IF (CMAKE_COMPILER_IS_GNUCXX AND (X86 OR X86_64)) )
set_option(ENABLE_NOISY_WARNINGS      "Show all warnings even if they are too noisy"             OFF )
set_option(LibSourcey_WARNINGS_ARE_ERRORS "Treat warnings as errors"                             OFF )

# ----------------------------------------------------------------------------
# LibSourcey internal options
# ----------------------------------------------------------------------------
set(LibSourcey_INCLUDE_DIRS           "") # CACHE INTERNAL "Global include dirs" FORCE)
set(LibSourcey_LIBRARY_DIRS           "") # CACHE INTERNAL "Global include library dirs" FORCE)
set(LibSourcey_INCLUDE_LIBRARIES      "") # CACHE INTERNAL "Global include libraries" FORCE)

set(LibSourcey_BUILD_DEPENDENCIES     "" CACHE INTERNAL "Dependencies to build" FORCE)
set(LibSourcey_BUILD_MODULES          "" CACHE INTERNAL "Modules to build" FORCE)
set(LibSourcey_BUILD_SAMPLES          "" CACHE INTERNAL "Samples to build" FORCE)
set(LibSourcey_BUILD_TESTS            "" CACHE INTERNAL "Tests to build" FORCE)
set(LibSourcey_BUILD_APPLICATIONS     "" CACHE INTERNAL "Applications to build" FORCE)

# ----------------------------------------------------------------------------
# LibSourcey Build paths
# ----------------------------------------------------------------------------
set(LibSourcey_DIR ${CMAKE_SOURCE_DIR})
set(LibSourcey_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(LibSourcey_BUILD_DIR ${CMAKE_BINARY_DIR})
set(LibSourcey_INSTALL_DIR ${CMAKE_INSTALL_PREFIX})
# set(LibSourcey_DEPENDENCIES_SOURCE_DIR "${LibSourcey_DIR}/deps")
set(LibSourcey_DEPENDENCIES_BUILD_DIR "${LibSourcey_BUILD_DIR}/deps")
set(LibSourcey_DEPENDENCIES_INSTALL_DIR "${LibSourcey_INSTALL_DIR}/share/libscy/deps")

# # Append some common search paths for find_library and find_path
# # to find libraries from these locations first
# if(WIN32)
#   # # set(CMAKE_FIND_LIBRARY_PREFIXES "" "lib")
#   # # set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib" ".a") # ".dll"
#   # set(CMAKE_SYSTEM_PREFIX_PATH ${CMAKE_SYSTEM_PREFIX_PATH}
#   #   # ${LibSourcey_SOURCE_DIR}/anionu-abi/openssl/lib/
#   #   # ${LibSourcey_SOURCE_DIR}/anionu-abi/ffmpeg/lib/
#   #   # ${LibSourcey_SOURCE_DIR}/anionu-abi/opencv/lib/
#   #   ${LibSourcey_DEPENDENCIES_SOURCE_DIR}/lib
#   #   )
# else()
#   # # set(CMAKE_FIND_LIBRARY_PREFIXES "lib")
#   # # set(CMAKE_FIND_LIBRARY_SUFFIXES ".so" ".a")
#   # set(CMAKE_LIBRARY_PATH
#   #   #${LibSourcey_BUILD_DIR}/lib
#   #   #/usr/local/share/libscy/deps/lib
#   #   #/home/deploy/build/lib
#   #   # /usr/local/lib
#   #   # /usr/local/lib/x86_64-linux-gnu
#   #   # /usr/local/lib/i386-linux-gnu
#   #   # /usr/local/lib64
#   #   # /usr/lib
#   #   # /usr/lib/x86_64-linux-gnu
#   #   # /usr/lib/i386-linux-gnu
#   #   /usr/lib64
#   #   ${CMAKE_LIBRARY_PATH})
#   #
#   # set(CMAKE_SYSTEM_PREFIX_PATH
#   #   #/usr/local/share/libscy/deps/include
#   #   #/home/deploy/build
#   #   #/home/deploy/build/include
#   #   ${CMAKE_SYSTEM_PREFIX_PATH})
#   #
#   # # Temp fix for "/usr/bin/ld: cannot find -lavcodec"
#   # # link_directories(/home/deploy/build/lib)
# endif()

# ----------------------------------------------------------------------------
# Solution folders:
# ----------------------------------------------------------------------------
if(ENABLE_SOLUTION_FOLDERS)
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
  set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMakeTargets")
endif()

# ----------------------------------------------------------------------------
# Use statically or dynamically linked CRT?
# Default: dynamic
# ----------------------------------------------------------------------------
if(MSVC)
  include(LibSourceyCRTLinkage REQUIRED)
endif()

# ----------------------------------------------------------------------------
# LibSourcey compiler and linker options
# ----------------------------------------------------------------------------
include(LibSourceyCompilerOptions REQUIRED)

# ----------------------------------------------------------------------------
# Set the location of our platform-dependant LibSourceyConfig.h
# ----------------------------------------------------------------------------
set(LibSourcey_CONFIG_FILE_INCLUDE_DIR "${CMAKE_BINARY_DIR}/" CACHE PATH "Where to create the platform-dependant LibSourceyConfig.h")
include_directories("${LibSourcey_CONFIG_FILE_INCLUDE_DIR}")

# Set the LibSourcey_LIB_TYPE variable for add_library
set(LibSourcey_LIB_TYPE STATIC)
if(BUILD_SHARED_LIBS)
  set(LibSourcey_LIB_TYPE SHARED)
endif()

# ============================================================================
# Include Dependencies, Modules and Applications
#
# LibSourcey automatically includes all directories inside the ./src folder.
# Libraries in the LibSourcey source tree are broken up into three types:
#
# Dependency:
#     A third party library required by a LibSourcey Modue or Application.
#     Dependencies may be external or internal. External dependencies reside
#     outside on the source tree, while internal dependencies generally reside
#     in the ./deps folder. All dependencies must be built, installed, found
#     and included by the build system before Modules and Application can be
#     built.
#
# Module:
#     A static or dynamic library which extends the LibSourcey core, and is
#     included by Applications based on LibSourcey architecture. Modules must
#     be built and installed before Applications.
#
# Application:
#     A standalone application. These can be built once all Dependencies and
#     Modules have been built and installed.
#
# ============================================================================

# ----------------------------------------------------------------------------
# Include standard and defult libraries
# ----------------------------------------------------------------------------
if(MSVC)
  # CMAKE_CXX_STANDARD_LIBRARIES must be a string for MSVC
  set(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} advapi32.lib iphlpapi.lib psapi.lib shell32.lib ws2_32.lib dsound.lib winmm.lib strmiids.lib")
elseif(MSYS)
  set(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} -lws2_32 -liphlpapi") #${LibSourcey_INCLUDE_LIBRARIES}
elseif(APPLE)
  set(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} -ldl") # -lm -lz -llibc -lglibc #${LibSourcey_INCLUDE_LIBRARIES}
elseif(UNIX)
  set(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} -lm -lz -ldl -lrt") # -lpulse-simple -lpulse -ljack -llibc -lglibc #${LibSourcey_INCLUDE_LIBRARIES}
endif()

if(MSVC)
  # Temporary workaround for "error LNK2026: module unsafe for SAFESEH image"
  # when compiling with certain externally compiled libraries with VS2012,
  # such as http://ffmpeg.zeranoe.com/builds/
  # This disables safe exception handling by default.
  if(${_MACHINE_ARCH_FLAG} MATCHES X86)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /SAFESEH:NO")
    set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /SAFESEH:NO")
  endif()
endif()


# ----------------------------------------------------------------------------
# Include third party dependencies
# ----------------------------------------------------------------------------

# Prefind external dependencies so we can set defaults
find_package(OpenCV)
find_package(FFmpeg)

#set_option(WITH_LIBUV           "Include LibUV support"                ON)
#set_option(WITH_HTTPPARSER      "Include HttpParser support"           ON)
set_option(WITH_JSONCPP         "Include JsonCpp support"              ON)
set_option(WITH_ZLIB            "Include zlib support"                 ON)
set_option(WITH_OPENSSL         "Include OpenSSL support"              ON)
set_option(WITH_RTAUDIO         "Include RtAudio support"              ON)
set_option(WITH_FFMPEG          "Include FFmpeg support"               ON IF (FFmpeg_FOUND))
set_option(WITH_OPENCV          "Include OpenCV support"               ON IF (OpenCV_FOUND))
set_option(WITH_WEBRTC          "Include WebRTC support"               OFF)
set_option(WITH_WXWIDGETS       "Include wxWidgets support"            OFF)

# Include dependencies build dir for cmake generated config files
list(APPEND LibSourcey_INCLUDE_DIRS ${LibSourcey_DEPENDENCIES_BUILD_DIR})

#set(LibSourcey_INCLUDE_DIRS ${LibSourcey_INCLUDE_DIRS} ${LibSourcey_DEPENDENCIES_BUILD_DIR})
#list(APPEND LibSourcey_INCLUDE_DIRS ${LibSourcey_DEPENDENCIES_SOURCE_DIR})
#set(LibSourcey_DEPENDENCIES_SOURCE_DIR "${LibSourcey_DIR}/deps")
#set(LibSourcey_DEPENDENCIES_BUILD_DIR "${LibSourcey_BUILD_DIR}/deps")
#set(LibSourcey_DEPENDENCIES_INSTALL_DIR "${LibSourcey_INSTALL_DIR}/share/libscy/deps")
#link_directories("${LibSourcey_DEPENDENCIES_INSTALL_DIR}")

#if(BUILD_DEPENDENCIES)
#  add_subdirectory(${LibSourcey_DEPENDENCIES_SOURCE_DIR})
#endif()

# Local dependencies
#if(WITH_LIBUV)
  #include_dependency(LibUV REQUIRED)
  # list(APPEND LibSourcey_BUILD_DEPENDENCIES libuv)
  # add_subdirectory(deps/libuv)
  # set(HAVE_LIBUV 1)
#endif()

# #if(WITH_HTTPPARSER)
#   #include_dependency(HttpParser REQUIRED)
#   list(APPEND LibSourcey_BUILD_DEPENDENCIES http_parser)
#   add_subdirectory(deps/http_parser)
#   set(HAVE_HTTPPARSER 1)
# #endif()

# if(WITH_RTAUDIO)
#   #include_dependency(RtAudio REQUIRED)
#   set(BUILD_TESTING OFF)
#   list(APPEND LibSourcey_BUILD_DEPENDENCIES rtaudio)
#   add_subdirectory(deps/rtaudio)
#   set(HAVE_RTAUDIO 1)
#   include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/rtaudio)
# endif()

# if(WITH_JSONCPP)
#   #include_dependency(JsonCpp REQUIRED)
#   set(HAVE_JSONCPP 1)
# endif()
#
# if(WITH_ZLIB)
#   #include_dependency(ZLIB REQUIRED)
#   #include_dependency(Minizip REQUIRED)
#   #list(APPEND LibSourcey_BUILD_DEPENDENCIES zlib)
#   #list(APPEND LibSourcey_BUILD_DEPENDENCIES minizip)
#   set(HAVE_ZLIB 1)
# endif()

# External dependencies
include_dependency(LibUV REQUIRED)

if(WITH_OPENSSL)
  include_dependency(OpenSSL REQUIRED)
endif()

if(WITH_FFMPEG)
  include_dependency(FFmpeg REQUIRED)
endif()

if(WITH_OPENCV)
  include_dependency(OpenCV REQUIRED)
endif()

if(WITH_WEBRTC)
  include_dependency(WebRTC REQUIRED)
endif()

if(WITH_WXWIDGETS)
  # TODO: specify library option
  include_dependency(wxWidgets REQUIRED core base adv)
endif()

# Remove any duplicates from our lists
# if(LibSourcey_LINK_LIBRARIES)
#   list(REMOVE_DUPLICATES LibSourcey_LINK_LIBRARIES)
# endif()
# if(LibSourcey_BUILD_DEPENDENCIES)
#   list(REMOVE_DUPLICATES LibSourcey_BUILD_DEPENDENCIES)
# endif()

# ----------------------------------------------------------------------------
# Include the LibSourcey source tree
# ----------------------------------------------------------------------------
if(BUILD_MODULES OR BUILD_APPLICATIONS)
  subdirlist(subdirs "${CMAKE_SOURCE_DIR}/src")
  foreach(name ${subdirs})
    # This variable is so modules can set HAVE_LibSourcey_XXX for our
    # LibSourceyConfig.h inside the child scope. See include_sourcey_modules()
    # TODO: Need to refactor since modules may be nested.
    set(HAVE_LibSourcey_${name} 0)
    set(dir "${CMAKE_SOURCE_DIR}/src/${name}")
    if (EXISTS "${dir}/CMakeLists.txt")
      add_subdirectory(${dir})
    endif()
  endforeach()
endif()

# ----------------------------------------------------------------------------
# Build our LibSourceyConfig.h file
#
# A directory will be created for each platform so the "LibSourceyConfig.h" file is
# not overwritten if cmake generates code in the same path.
# ----------------------------------------------------------------------------
add_definitions(-DHAVE_CONFIG_H)

# Variables for LibSourceyConfig.h.cmake
set(PACKAGE "LibSourcey")
set(PACKAGE_BUGREPORT "https://github.com/sourcey/LibSourcey/issues")
set(PACKAGE_NAME "LibSourcey")
set(PACKAGE_STRING "${PACKAGE} ${LibSourcey_VERSION}")
set(PACKAGE_TARNAME "${PACKAGE}")
set(PACKAGE_VERSION "${LibSourcey_VERSION}")

status("Parsing 'LibSourceyConfig.h.cmake'")
configure_file(
  "${CMAKE_SOURCE_DIR}/cmake/LibSourceyConfig.h.cmake"
  "${LibSourcey_CONFIG_FILE_INCLUDE_DIR}/scyconfig.h")

# ----------------------------------------------------------------------------
#  Uninstall target for "make uninstall"
# ----------------------------------------------------------------------------
if(UNIX)
  CONFIGURE_FILE(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

  ADD_CUSTOM_TARGET(uninstall
    "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
endif()

# ----------------------------------------------------------------------------
#  Summary:
# ----------------------------------------------------------------------------

# Build platform
status("")
status("  Platform:")
status("    Host:"             ${CMAKE_HOST_SYSTEM_NAME} ${CMAKE_HOST_SYSTEM_VERSION} ${CMAKE_HOST_SYSTEM_PROCESSOR})
if(CMAKE_CROSSCOMPILING)
  status("    Target:"           ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_VERSION} ${CMAKE_SYSTEM_PROCESSOR})
endif()
status("    CMake:"            ${CMAKE_VERSION})
status("    CMake generator:"  ${CMAKE_GENERATOR})
status("    CMake build tool:" ${CMAKE_BUILD_TOOL})
if(MSVC)
  status("    MSVC:"           ${MSVC_VERSION})
endif()
if(CMAKE_GENERATOR MATCHES Xcode)
  status("    Xcode:"          ${XCODE_VERSION})
endif()
if(NOT CMAKE_GENERATOR MATCHES "Xcode|Visual Studio")
  status("    Configuration:"  ${CMAKE_BUILD_TYPE})
endif()

# C/C++ options
status("")
status("  C/C++:")
status("    Built as dynamic libs?:" BUILD_SHARED_LIBS THEN YES ELSE NO)
status("    C++ Compiler:"           CMAKE_COMPILER_IS_GNUCXX THEN "${CMAKE_CXX_COMPILER} (ver ${CMAKE_GCC_REGEX_VERSION})" ELSE "${CMAKE_CXX_COMPILER}" )
status("    C++ flags (Release):"    ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE})
status("    C++ flags (Debug):"      ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG})

status("    C Compiler:"             ${CMAKE_C_COMPILER})
status("    C flags (Release):"      ${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_RELEASE})
status("    C flags (Debug):"        ${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_DEBUG})
if(WIN32)
  status("    Linker flags (Release):" ${CMAKE_EXE_LINKER_FLAGS} ${CMAKE_EXE_LINKER_FLAGS_RELEASE})
  status("    Linker flags (Debug):"   ${CMAKE_EXE_LINKER_FLAGS} ${CMAKE_EXE_LINKER_FLAGS_DEBUG})
else()
  status("    Linker flags (Release):" ${CMAKE_SHARED_LINKER_FLAGS} ${CMAKE_SHARED_LINKER_FLAGS_RELEASE})
  status("    Linker flags (Debug):"   ${CMAKE_SHARED_LINKER_FLAGS} ${CMAKE_SHARED_LINKER_FLAGS_DEBUG})
endif()

# Modules
status("")
status("  Build Components:")
status("")
status("     Dependencies:               ${LibSourcey_BUILD_DEPENDENCIES}")
status("     Modules:                    ${LibSourcey_BUILD_MODULES}")
status("     Applications:               ${LibSourcey_BUILD_APPLICATIONS}")

# Dependencies
status("")
status("  Other third-party libraries:")
status("")
#status("    Use LibUV:"         HAVE_LIBUV           THEN YES ELSE NO)
#status("    Use HttpParser:"    HAVE_HTTPPARSER      THEN YES ELSE NO)
status("    Use OpenSSL:"       HAVE_OPENSSL         THEN YES ELSE NO)
status("    Use FFmpeg:"        HAVE_FFMPEG          THEN YES ELSE NO)
status("    Use OpenCV:"        HAVE_OPENCV          THEN "YES (ver ${OpenCV_VERSION})" ELSE NO)
status("    Use WebRTC:"        HAVE_WEBRTC          THEN YES ELSE NO)
status("    Use RtAudio:"       HAVE_RTAUDIO         THEN YES ELSE NO)
status("    Use JsonCpp:"       HAVE_JSONCPP         THEN YES ELSE NO)
status("    Use wxWidgets:"     HAVE_WXWIDGETS       THEN YES ELSE NO)
status("    Use zlib:"          HAVE_ZLIB            THEN YES ELSE NO)

# Includes
# status("")
# status("  Linker paths and libraries:")
# status("")
# status("    Include dirs:                ${LibSourcey_INCLUDE_DIRS}")
# status("    Include libs:                ${LibSourcey_INCLUDE_LIBRARIES}")
# status("    Library dirs:                ${LibSourcey_LIBRARY_DIRS}")
# status("    Standard libs:               ${CMAKE_CXX_STANDARD_LIBRARIES}")

# Auxiliary
status("")
status("    Install path:                ${CMAKE_INSTALL_PREFIX}")
status("")
status("    LibSourceyConfig.h is in:    ${LibSourcey_CONFIG_FILE_INCLUDE_DIR}")
status("-----------------------------------------------------------------")
status("")

# Warn in the case of in-source build
if("${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")
    message(WARNING "The source directory is the same as binary directory. \"make clean\" may damage the source tree")
endif()
