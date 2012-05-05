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

project(LibSourcey)

add_definitions(-DUNICODE -D_UNICODE)


# ----------------------------------------------------------------------------
# Current version number:
# ----------------------------------------------------------------------------
set(LibSourcey_VERSION "0.8.0")

string(REGEX MATCHALL "[0-9]" LibSourcey_VERSION_PARTS "${LibSourcey_VERSION}")

list(GET LibSourcey_VERSION_PARTS 0 LibSourcey_VERSION_MAJOR)
list(GET LibSourcey_VERSION_PARTS 1 LibSourcey_VERSION_MINOR)
list(GET LibSourcey_VERSION_PARTS 2 LibSourcey_VERSION_PATCH)

set(LibSourcey_SOVERSION "${LibSourcey_VERSION_MAJOR}.${LibSourcey_VERSION_MINOR}")

if(WIN32)
    # Postfix of DLLs:
    set(LibSourcey_DLLVERSION "${LibSourcey_VERSION_MAJOR}${LibSourcey_VERSION_MINOR}${LibSourcey_VERSION_PATCH}")
    set(LibSourcey_DEBUG_POSTFIX "d")
else()
    # Postfix of so's:
    set(LibSourcey_DLLVERSION "")
    set(LibSourcey_DEBUG_POSTFIX)
endif()


# ----------------------------------------------------------------------------
# Build static or dynamic libs?
# Default: static libraries
# ----------------------------------------------------------------------------
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries (.dll/.so) instead of static ones (.lib/.a)")


# ----------------------------------------------------------------------------
# Build sample applications
# Default: off
# ----------------------------------------------------------------------------
set(BUILD_MODULE_SAMPLES OFF CACHE BOOL "Build module sample applications?")


# ----------------------------------------------------------------------------
# Build test applications
# Default: off
# ----------------------------------------------------------------------------
set(BUILD_MODULE_TESTS OFF CACHE BOOL "Build module test applications?")


# ----------------------------------------------------------------------------
# Include debug info into debug libs?
# Default: yes
# ----------------------------------------------------------------------------
set(BUILD_WITH_DEBUG_INFO ON CACHE BOOL "Include debug info into debug libs")


# ----------------------------------------------------------------------------
# Use statically or dynamically linked CRT?
# Default: dynamic
# ----------------------------------------------------------------------------
if(WIN32 AND NOT BUILD_SHARED_LIBS)
  option (BUILD_WITH_STATIC_CRT "Enables use of staticaly linked CRT" OFF)
endif()
    
if(MSVC)
    if(BUILD_WITH_STATIC_CRT)
        foreach(flag_var
                CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
                CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO
                CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
                CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
           if(${flag_var} MATCHES "/MD")
              string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
           endif(${flag_var} MATCHES "/MD")

           if(${flag_var} MATCHES "/MDd")
              string(REGEX REPLACE "/MDd" "/MTd" ${flag_var} "${${flag_var}}")
           endif(${flag_var} MATCHES "/MDd")
        endforeach(flag_var)

        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:atlthunk.lib /NODEFAULTLIB:msvcrt.lib /NODEFAULTLIB:msvcrtd.lib")

        set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /NODEFAULTLIB:libcmt.lib")
        set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /NODEFAULTLIB:libcmtd.lib")
    else(BUILD_WITH_STATIC_CRT)
        foreach(flag_var
                CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
                CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO
                CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
                CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
           if(${flag_var} MATCHES "/MT")
              string(REGEX REPLACE "/MT" "/MD" ${flag_var} "${${flag_var}}")
           endif(${flag_var} MATCHES "/MT")

           if(${flag_var} MATCHES "/MTd")
              string(REGEX REPLACE "/MTd" "/MDd" ${flag_var} "${${flag_var}}")
           endif(${flag_var} MATCHES "/MTd")
        endforeach(flag_var)

    endif(BUILD_WITH_STATIC_CRT)

    if(NOT BUILD_WITH_DEBUG_INFO)
        string(REPLACE "/debug" "" CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG}")
        string(REPLACE "/DEBUG" "" CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG}")
        string(REPLACE "/INCREMENTAL:YES" "/INCREMENTAL:NO" CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG}")
        string(REPLACE "/INCREMENTAL " "/INCREMENTAL:NO" CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG}")

        string(REPLACE "/debug" "" CMAKE_MODULE_LINKER_FLAGS_DEBUG "${CMAKE_MODULE_LINKER_FLAGS_DEBUG}")
        string(REPLACE "/DEBUG" "" CMAKE_MODULE_LINKER_FLAGS_DEBUG "${CMAKE_MODULE_LINKER_FLAGS_DEBUG}")
        string(REPLACE "/INCREMENTAL:YES" "/INCREMENTAL:NO" CMAKE_MODULE_LINKER_FLAGS_DEBUG "${CMAKE_MODULE_LINKER_FLAGS_DEBUG}")
        string(REPLACE "/INCREMENTAL " "/INCREMENTAL:NO" CMAKE_MODULE_LINKER_FLAGS_DEBUG "${CMAKE_MODULE_LINKER_FLAGS_DEBUG}")

        string(REPLACE "/debug" "" CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG}")
        string(REPLACE "/DEBUG" "" CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG}")
        string(REPLACE "/INCREMENTAL:YES" "/INCREMENTAL:NO" CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG}")
        string(REPLACE "/INCREMENTAL " "/INCREMENTAL:NO" CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG}")

        string(REPLACE "/Zi" "" CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
        string(REPLACE "/Zi" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
    endif()

endif()


# ----------------------------------------------------------------------------
# Set the location of our platform-dependant Config.h
# ----------------------------------------------------------------------------
set(LibSourcey_CONFIG_FILE_INCLUDE_DIR "${CMAKE_BINARY_DIR}/" CACHE PATH "Where to create the platform-dependant Config.h")
include_directories("${LibSourcey_CONFIG_FILE_INCLUDE_DIR}")


# ============================================================================
# Build paths
# ============================================================================
set(LibSourcey_SOURCE_DIR ${CMAKE_SOURCE_DIR})
set(LibSourcey_BUILD_DIR ${CMAKE_BINARY_DIR})
set(LibSourcey_INSTALL_DIR ${CMAKE_INSTALL_PREFIX})
set(LibSourcey_DEPENDENCIES_SOURCE_DIR "${LibSourcey_SOURCE_DIR}/deps")
set(LibSourcey_DEPENDENCIES_BUILD_DIR "${LibSourcey_INSTALL_DIR}/deps")
set(LibSourcey_DEPENDENCIES_INSTALL_DIR "${LibSourcey_INSTALL_DIR}/share/LibSourcey/deps")
set(LibSourcey_INCLUDE_DIRS "") # Contains all include dirs
set(LibSourcey_LIBRARY_DIRS "") # Contains all include library dirs
set(LibSourcey_INCLUDE_LIBRARIES "") # Contains all include libraries

# Tell CMake where to locate our .cmake files
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

# Append some common search paths for find_library
# to find libraries from these locations first
if(WIN32)
  set(CMAKE_SYSTEM_PREFIX_PATH ${CMAKE_SYSTEM_PREFIX_PATH}
    ${LibSourcey_DEPENDENCIES_SOURCE_DIR}/lib
    D:/dev/lib/OpenCV2.3/build
    D:/dev/lib/wxWidgets-2.9.2/lib/vc_lib
    #D:/dev/lib/ffmpeg/lib
    D:/dev/lib/ffmpeg-2012-04-16/lib
    D:/dev/lib/OpenSSL-Win32/lib/VC
    D:/dev/lib/poco-1.4.1p1-all/lib 
    #D:/dev/apps/MinGW/msys/1.0/local
    )
else()
  set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} 
    /lib 
    /usr/lib)
endif()

# Append some common search paths for find_path
# to search these locations first
if(WIN32)
  set(CMAKE_SYSTEM_PREFIX_PATH ${CMAKE_SYSTEM_PREFIX_PATH}
    ${LibSourcey_DEPENDENCIES_SOURCE_DIR}
    D:/dev/lib/OpenCV2.3/build/include
    D:/dev/lib/wxWidgets-2.9.2
    #D:/dev/lib/ffmpeg
    D:/dev/lib/ffmpeg-2012-04-16/include
    D:/dev/lib/OpenSSL-Win32/include
    D:/dev/lib/poco-1.4.1p1-all
    #D:/dev/apps/MinGW/msys/1.0/local
    )
endif()
  #set(LibSourcey_INCLUDE_DIRS ${LibSourcey_INCLUDE_DIRS} "${LibSourcey_DEPENDENCIES_SOURCE_DIR}/MemLeakDetect")
      
set(LibSourcey_ARCHITECTURE i386)
set(LibSourcey_BITNESS 32)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(LibSourcey_BITNESS 64)
  set(LibSourcey_ARCHITECTURE amd64)
endif()

include(LibSourceyMacros REQUIRED)
include(LibSourceyModule REQUIRED)
include(LibSourceyDependency OPTIONAL)


# ----------------------------------------------------------------------------
# Include and build third party dependencies
# ----------------------------------------------------------------------------
set(BUILD_DEPENDENCIES TRUE CACHE BOOL "Build third party dependencies")

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
  #set(LibSourcey_INCLUDE_DIRS ${LibSourcey_INCLUDE_DIRS} "${LibSourcey_DEPENDENCIES_SOURCE_DIR}/MemLeakDetect")
  add_subdirectory(${LibSourcey_DEPENDENCIES_SOURCE_DIR})
else()
  unset(WITH_PUGIXML CACHE)
  unset(WITH_RTAUDIO CACHE)
  unset(WITH_LIBSTROPHE CACHE)
  unset(WITH_JSONCPP CACHE)
endif()


# ============================================================================
# Include libraries, modules and projects
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
# Include third party dependencies
# ----------------------------------------------------------------------------

# Include our third party source directory
list(APPEND LibSourcey_INCLUDE_DIRS ${LibSourcey_DEPENDENCIES_SOURCE_DIR}) 
if (MSVC)
  list(APPEND LibSourcey_INCLUDE_DIRS ${LibSourcey_DEPENDENCIES_SOURCE_DIR}/msvc) 
endif()
include_directories("${LibSourcey_INCLUDE_DIRS}")
link_directories("${LibSourcey_INSTALL_DIR}/lib")

 
# ----------------------------------------------------------------------------
# Include LibSourcey modules and projects
#
# LibSourcey automatically includes all directories inside core and projects. 
# Libraries in the LibSourcey source tree are broken up into two types:
#
#   Dependency:
#     A third party library required by a LibSourcey Modue or Application.  
#     Dependencies may be external or internal. External dependencies reside
#     outside on the source tree, while internal dependencies generally reside
#     in the deps folder. All dependencies must be built, installed, found
#     and included by the build system before Modules and Application can be
#     built. 
#
#   Module: 
#     A static or dynamic library which extends the LibSourcey core, and is
#     included by Applications based on LibSourcey architecture. Modules must
#     be built and installed before Applications.
#
#   Application: 
#     A standalone application. These must be built once all Dependencies and
#     Modules have been built and installed.
#
# ----------------------------------------------------------------------------
set(BUILD_MODULES OFF CACHE BOOL "Build LibSourcey modules")  
set(BUILD_APPLICATIONS OFF CACHE BOOL "Build LibSourcey applications")
  

# ----------------------------------------------------------------------------
# Include the LibSourcey source tree
# ----------------------------------------------------------------------------
if(BUILD_MODULES OR BUILD_APPLICATIONS)    
  subdirlist(subdirs "${CMAKE_SOURCE_DIR}/libs")
  foreach(name ${subdirs})
    # This variable is so modules can set HAVE_LIBSOURCEY_XXX for our
    # Config.h inside the child scope. See include_sourcey_modules()
    set(HAVE_LIBSOURCEY_${name} 0)
    set(dir "${CMAKE_SOURCE_DIR}/libs/${name}")
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

message(STATUS "Parsing 'Config.h.cmake'")
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
message(STATUS "")
message(STATUS "General configuration for LibSourcey ${LibSourcey_VERSION} =====================================")
message(STATUS "")
message(STATUS "    Built as dynamic libs?:     ${BUILD_SHARED_LIBS}")
message(STATUS "    Compiler:                   ${CMAKE_COMPILER}")
message(STATUS "    C++ flags (Release):        ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE}")
message(STATUS "    C++ flags (Debug):          ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG}")
if(WIN32)
message(STATUS "    Linker flags (Release):     ${CMAKE_EXE_LINKER_FLAGS} ${CMAKE_EXE_LINKER_FLAGS_RELEASE}")
message(STATUS "    Linker flags (Debug):       ${CMAKE_EXE_LINKER_FLAGS} ${CMAKE_EXE_LINKER_FLAGS_DEBUG}")
else()
message(STATUS "    Linker flags (Release):     ${CMAKE_SHARED_LINKER_FLAGS} ${CMAKE_SHARED_LINKER_FLAGS_RELEASE}")
message(STATUS "    Linker flags (Debug):       ${CMAKE_SHARED_LINKER_FLAGS} ${CMAKE_SHARED_LINKER_FLAGS_DEBUG}")
endif()

message(STATUS "")
message(STATUS "    Install path:               ${CMAKE_INSTALL_PREFIX}")
message(STATUS "")
message(STATUS "    Config.h is in:             ${LibSourcey_CONFIG_FILE_INCLUDE_DIR}")
message(STATUS "-----------------------------------------------------------------")
message(STATUS "")

# Warn in the case of in-source build
if("${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")
    message(WARNING "The source directory is the same as binary directory. \"make clean\" may damage the source tree")
endif()
