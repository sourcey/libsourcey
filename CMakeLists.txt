# ============================================================================
# Root CMake file for LibSourcey
# ============================================================================

cmake_minimum_required(VERSION 2.8)

# This _must_ go before project(LibSourcey) in order to work
if(WIN32)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Installation Directory")
else()
    set(CMAKE_INSTALL_PREFIX "/usr/local" CACHE PATH "Installation Directory")
endif()

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)
if(DEFINED CMAKE_BUILD_TYPE AND CMAKE_VERSION VERSION_GREATER "2.8")
  set_property(7 CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${CMAKE_CONFIGURATION_TYPES}7 )
endif()

project(LibSourcey)

add_definitions(-DUNICODE -D_UNICODE)

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
set_option(BUILD_MODULE_SAMPLES       "Build module sample applications?"                        OFF)
set_option(BUILD_MODULE_TESTS         "Build module test applications?"                          OFF)
set_option(BUILD_WITH_DEBUG_INFO      "Include debug info into debug libs"                       ON)
set_option(BUILD_WITH_DEBUG_INFO      "Include debug info into debug libs (not MSCV only)"       ON )
set_option(BUILD_WITH_STATIC_CRT      "Enables staticaly linked CRT for staticaly linked libraries" OFF)

# ----------------------------------------------------------------------------
# OpenCV build options
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
# Build paths
# ----------------------------------------------------------------------------
set(LibSourcey_DIR ${CMAKE_SOURCE_DIR})
set(LibSourcey_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(LibSourcey_BUILD_DIR ${CMAKE_BINARY_DIR})
set(LibSourcey_INSTALL_DIR ${CMAKE_INSTALL_PREFIX})
set(LibSourcey_DEPENDENCIES_SOURCE_DIR "${LibSourcey_DIR}/deps")
set(LibSourcey_DEPENDENCIES_BUILD_DIR "${LibSourcey_INSTALL_DIR}/deps")
set(LibSourcey_DEPENDENCIES_INSTALL_DIR "${LibSourcey_INSTALL_DIR}/share/LibSourcey/deps")
set(LibSourcey_INCLUDE_DIRS "")         # Contains all include dirs
set(LibSourcey_LIBRARY_DIRS "")         # Contains all include library dirs
set(LibSourcey_INCLUDE_LIBRARIES "")    # Contains all include libraries
set(LibSourcey_BUILD_DEPENDENCIES "")   # List of all dependencies to build
set(LibSourcey_BUILD_MODULES "")        # List of all modules to build
set(LibSourcey_BUILD_APPLICATIONS "")   # List of all apps to build

# Append some common search paths for find_library
# to find libraries from these locations first
if(WIN32)
  set(CMAKE_SYSTEM_PREFIX_PATH ${CMAKE_SYSTEM_PREFIX_PATH}
    D:/dev/lib/OpenCV2.4.1/build/install/lib
    D:/dev/lib/wxWidgets-2.9.4/lib/vc_lib
    D:/dev/lib/ffmpeg-1.0.1-gpl/lib
    D:/dev/lib/OpenSSL-Win32/lib/VC
    D:/dev/lib/poco-1.4.1p1-all/lib 
    ${LibSourcey_DEPENDENCIES_SOURCE_DIR}/lib
    )
    #D:/dev/lib/OpenCV2.3/build/x86/vc10/lib
    #D:/dev/lib/OpenCV2.4/build/lib/Debug
    #D:/dev/lib/OpenCV2.4/build/install/lib
    #D:/dev/lib/OpenCV2.4.1/build/x86/vc10/lib
    #D:/dev/lib/OpenCV2.4.1/build/x86/vc10/lib
    #${LibSourcey_DEPENDENCIES_SOURCE_DIR}/ffmpeg/bin
    #D:/dev/lib/ffmpeg/lib
    #D:/dev/apps/MinGW/msys/1.0/local
    #D:/dev/projects/Sourcey/LibSourcey/src/anionu/deps/ffmpeg/bin
else()
  set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} 
    /lib 
    /usr/lib)
endif()

# Append some common search paths for find_path
# to search these locations first
if(WIN32)
  set(CMAKE_SYSTEM_PREFIX_PATH ${CMAKE_SYSTEM_PREFIX_PATH}
    D:/dev/lib/OpenCV2.4.1/build/include
    D:/dev/lib/wxWidgets-2.9.4
    D:/dev/lib/ffmpeg-1.0.1-gpl/include
    D:/dev/lib/OpenSSL-Win32/include
    D:/dev/lib/poco-1.4.1p1-all
    ${LibSourcey_DEPENDENCIES_SOURCE_DIR}
    )
    #D:/dev/lib/OpenCV2.3/build/include
    #D:/dev/lib/OpenCV2.4/build/install/include
    #${LibSourcey_DEPENDENCIES_SOURCE_DIR}/ffmpeg
    #D:/dev/lib/ffmpeg
    #D:/dev/apps/MinGW/msys/1.0/local
    #D:/dev/projects/Sourcey/LibSourcey/src/anionu/deps/ffmpeg/include
endif()

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
# Set the location of our platform-dependant Config.h
# ----------------------------------------------------------------------------
set(LibSourcey_CONFIG_FILE_INCLUDE_DIR "${CMAKE_BINARY_DIR}/" CACHE PATH "Where to create the platform-dependant Config.h")
include_directories("${LibSourcey_CONFIG_FILE_INCLUDE_DIR}")


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
# Include standard Libraries
# ----------------------------------------------------------------------------
if(MSVC)
  set(CMAKE_CXX_STANDARD_LIBRARIES ${CMAKE_CXX_STANDARD_LIBRARIES} ws2_32.lib dsound.lib winmm.lib strmiids.lib)  
elseif(MSYS)
  set(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} -lws2_32 -liphlpapi")
elseif(LINUX)
  set(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} -llibc -lglibc")
endif()

# ----------------------------------------------------------------------------
# Include and third party dependencies
# ----------------------------------------------------------------------------

# Define known dependencies for our Config.h
# These variables will be modified by include_dependency()
set(HAVE_PUGIXML 0)
set(HAVE_RTAUDIO 0)
set(HAVE_LIBSTROPHE 0)
set(HAVE_JSONCPP 0)
set(HAVE_OPENSSL 0)
set(HAVE_POCO 0)
set(HAVE_FFMPEG 0)
set(HAVE_OPENCV 0)
set(HAVE_WXWIDGETS 0)
set(HAVE_LIBUV 0)
  
if(BUILD_DEPENDENCIES)
  # TODO: Search for library and conditional build
  # depending on XXX_FOUND or HAVE_XXX variable  
  set(WITH_PUGIXML ON CACHE BOOL "Build with PugiXML")
  set(WITH_RTAUDIO ON CACHE BOOL "Build with RtAudio")
  set(WITH_LIBSTROPHE ON CACHE BOOL "Build with LibStrophe")
  set(WITH_JSONCPP ON CACHE BOOL "Build with JsonCpp")
  set(LibSourcey_INCLUDE_DIRS ${LibSourcey_INCLUDE_DIRS} "${LibSourcey_DEPENDENCIES_SOURCE_DIR}/MemLeakDetect")
  add_subdirectory(${LibSourcey_DEPENDENCIES_SOURCE_DIR})
else()
  unset(WITH_PUGIXML CACHE)
  unset(WITH_RTAUDIO CACHE)
  unset(WITH_LIBSTROPHE CACHE)
  unset(WITH_JSONCPP CACHE)
endif()

# Include our dependencies source directory
list(APPEND LibSourcey_INCLUDE_DIRS ${LibSourcey_DEPENDENCIES_SOURCE_DIR}) 
if (MSVC)
  list(APPEND LibSourcey_INCLUDE_DIRS ${LibSourcey_DEPENDENCIES_SOURCE_DIR}/msvc) 
endif()
include_directories("${LibSourcey_INCLUDE_DIRS}")
#link_directories("${LibSourcey_INSTALL_DIR}/lib")
  
# ----------------------------------------------------------------------------
# Include the LibSourcey source tree
# ----------------------------------------------------------------------------
if(BUILD_MODULES OR BUILD_APPLICATIONS)    
  subdirlist(subdirs "${CMAKE_SOURCE_DIR}/src")
  foreach(name ${subdirs})
    # This variable is so modules can set HAVE_LibSourcey_XXX for our
    # Config.h inside the child scope. See include_sourcey_modules()
    set(HAVE_LibSourcey_${name} 0)
    set(dir "${CMAKE_SOURCE_DIR}/src/${name}")
    if (EXISTS "${dir}/CMakeLists.txt")
      add_subdirectory(${dir})
    endif()
  endforeach() 
endif()

# ----------------------------------------------------------------------------
# Build our Config.h file
#
# A directory will be created for each platform so the "Config.h" file is
# not overwritten if cmake generates code in the same path.
# ----------------------------------------------------------------------------
add_definitions(-DHAVE_CONFIG_H)

# Variables for Config.h.cmake
set(PACKAGE "LibSourcey")
set(PACKAGE_BUGREPORT "https://github.com/sourcey/LibSourcey/issues")
set(PACKAGE_NAME "LibSourcey")
set(PACKAGE_STRING "${PACKAGE} ${LibSourcey_VERSION}")
set(PACKAGE_TARNAME "${PACKAGE}")
set(PACKAGE_VERSION "${LibSourcey_VERSION}")

status("Parsing 'Config.h.cmake'")
configure_file(
  "${CMAKE_SOURCE_DIR}/cmake/Config.h.cmake" 
  "${LibSourcey_CONFIG_FILE_INCLUDE_DIR}/Config.h")
  
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
status("    Dependencies:               ${LibSourcey_BUILD_DEPENDENCIES}")
status("    Modules:                    ${LibSourcey_BUILD_MODULES}")
status("    Applications:               ${LibSourcey_BUILD_APPLICATIONS}")
  
# Auxiliary
status("")
status("    Install path:               ${CMAKE_INSTALL_PREFIX}")
status("")
status("    Config.h is in:             ${LibSourcey_CONFIG_FILE_INCLUDE_DIR}")
status("-----------------------------------------------------------------")
status("")

# Warn in the case of in-source build
if("${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")
    message(WARNING "The source directory is the same as binary directory. \"make clean\" may damage the source tree")
endif()











#foreach(m ${LibSourcey_BUILD_MODULES})
  
#endforeach()
  #set(__mdeps "")
  #foreach(d ${LibSourcey_MODULE_${m}_DEPS})
  #  if(d MATCHES "^opencv_" AND NOT HAVE_${d})
  #    list(APPEND __mdeps ${d})
  #  endif()
  #endforeach()
  #list(APPEND LibSourcey_MODULES_DISABLED_AUTO_ST "${m}(deps: ${__mdeps})")


#string(REPLACE "opencv_" "" LibSourcey_MODULES_BUILD_ST          "${LibSourcey_MODULES_BUILD}")
#string(REPLACE "opencv_" "" LibSourcey_MODULES_DISABLED_USER_ST  "${LibSourcey_MODULES_DISABLED_USER}")
#string(REPLACE "opencv_" "" LibSourcey_MODULES_DISABLED_FORCE_ST "${LibSourcey_MODULES_DISABLED_FORCE}")
#set(LibSourcey_MODULES_DISABLED_AUTO_ST "")




# ----------------------------------------------------------------------------
# Use statically or dynamically linked CRT?
# Default: dynamic
# ----------------------------------------------------------------------------
#if(WIN32 AND NOT BUILD_SHARED_LIBS)
#  option (BUILD_WITH_STATIC_CRT "Enables use of staticaly linked CRT" OFF)
#endif()
    


# ----------------------------------------------------------------------------
# Set bitness
# ----------------------------------------------------------------------------  
#set(LibSourcey_ARCHITECTURE i386)
#set(LibSourcey_BITNESS 32)
#if(CMAKE_SIZEOF_VOID_P EQUAL 8)
#  set(LibSourcey_BITNESS 64)
#  set(LibSourcey_ARCHITECTURE amd64)
#endif()
