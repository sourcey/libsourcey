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

project(LibSourcey)

add_definitions(-DUNICODE -D_UNICODE)


# ----------------------------------------------------------------------------
# Current version number:
# ----------------------------------------------------------------------------
set(LIBSOURCEY_VERSION "0.8.0")

string(REGEX MATCHALL "[0-9]" LIBSOURCEY_VERSION_PARTS "${LIBSOURCEY_VERSION}")

list(GET LIBSOURCEY_VERSION_PARTS 0 LIBSOURCEY_VERSION_MAJOR)
list(GET LIBSOURCEY_VERSION_PARTS 1 LIBSOURCEY_VERSION_MINOR)
list(GET LIBSOURCEY_VERSION_PARTS 2 LIBSOURCEY_VERSION_PATCH)

set(LIBSOURCEY_SOVERSION "${LIBSOURCEY_VERSION_MAJOR}.${LIBSOURCEY_VERSION_MINOR}")

if(WIN32)
    # Postfix of DLLs:
    set(LIBSOURCEY_DLLVERSION "${LIBSOURCEY_VERSION_MAJOR}${LIBSOURCEY_VERSION_MINOR}${LIBSOURCEY_VERSION_PATCH}")
    set(LIBSOURCEY_DEBUG_POSTFIX d)
else()
    # Postfix of so's:
    set(LIBSOURCEY_DLLVERSION "")
    set(LIBSOURCEY_DEBUG_POSTFIX)
endif()


# ----------------------------------------------------------------------------
# Build static or dynamic libs?
# Default: static libraries
# ----------------------------------------------------------------------------
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries (.dll/.so) instead of static ones (.lib/.a)")


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

endif(MSVC)


# ============================================================================
# Build paths
# ============================================================================
set(LIBSOURCEY_SOURCE_DIR ${CMAKE_SOURCE_DIR})
set(LIBSOURCEY_BUILD_DIR ${CMAKE_BINARY_DIR})
set(LIBSOURCEY_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
set(LIBSOURCEY_3RDPARTY_SOURCE_DIR "${LIBSOURCEY_SOURCE_DIR}/3rdparty")
set(LIBSOURCEY_3RDPARTY_BUILD_DIR "${LIBSOURCEY_INSTALL_PREFIX}/3rdparty")
set(LIBSOURCEY_INCLUDE_DIRS "") # Variable for all include dirs
set(LIBSOURCEY_LIBRARY_DIRS "") # Variable for all library dirs
set(LIBSOURCEY_DEBUG_LIBS "")   # Variable for all debug libraries
set(LIBSOURCEY_RELEASE_LIBS "") # Variable for all release libraries


# ----------------------------------------------------------------------------
# Build 3rdparty libraries
# ----------------------------------------------------------------------------
if(WIN32)
    set(BUILD_3RDPARTY_LIBS TRUE CACHE BOOL "Build 3rd party libraries")
else()
    set(BUILD_3RDPARTY_LIBS FALSE CACHE BOOL "Build 3rd party libraries")
endif()
if(BUILD_3RDPARTY_LIBS)
  # TODO: Search for library and conditional build
  # depending on XXX_FOUND or HAVE_XXX variable  
  set(WITH_PUGIXML ON CACHE BOOL "Build with PugiXML")
  set(WITH_RTAUDIO ON CACHE BOOL "Build with RtAudio")
  set(WITH_LIBSTROPHE ON CACHE BOOL "Build with LibStrophe")
  set(WITH_JSONCPP ON CACHE BOOL "Build with JsonCpp")
  set(WITH_OPENSSL ON CACHE BOOL "Build with OpenSSL")
  #set(WITH_POCO ON CACHE BOOL "Build with Poco")
  #set(WITH_FFMPEG ON CACHE BOOL "Build with FFmpeg")
  #set(WITH_OPENCV ON CACHE BOOL "Build with OpenCV")
  #set(WITH_WXWIDGETS ON CACHE BOOL "Build with wxWidgets")
  #set(WITH_LIBUV ON CACHE BOOL "Build with Libuv")
  #set(LIBSOURCEY_INCLUDE_DIRS ${LIBSOURCEY_INCLUDE_DIRS} "${LIBSOURCEY_SOURCE_DIR}/3rdparty/MemLeakDetect")
  add_subdirectory(${LIBSOURCEY_3RDPARTY_SOURCE_DIR})
else()
  unset(WITH_PUGIXML CACHE)
  unset(WITH_RTAUDIO CACHE)
  unset(WITH_LIBSTROPHE CACHE)
  unset(WITH_JSONCPP CACHE)
  unset(WITH_OPENSSL CACHE)
endif()


# ============================================================================
# Macro for looping sub directories
#
# Example:
#  SUBDIRLIST(SUBDIRS ${MY_CURRENT_DIR})
# ============================================================================
MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
        SET(dirlist ${dirlist} ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()


# ============================================================================
# Include libraries, modules and projects
# ============================================================================

# ----------------------------------------------------------------------------
# Additional Libraries
# ----------------------------------------------------------------------------
if(WIN32)
  set(LIBSOURCEY_DEBUG_LIBS ${LIBSOURCEY_DEBUG_LIBS} ws2_32)  
  set(LIBSOURCEY_DEBUG_LIBS ${LIBSOURCEY_DEBUG_LIBS} dsound)  
  set(LIBSOURCEY_RELEASE_LIBS ${LIBSOURCEY_RELEASE_LIBS} ws2_32)  
  set(LIBSOURCEY_RELEASE_LIBS ${LIBSOURCEY_RELEASE_LIBS} dsound) 
endif()


# ----------------------------------------------------------------------------
# Include third party libraries
# ----------------------------------------------------------------------------

# Include our third party source directory
#set(LIBSOURCEY_INCLUDE_DIRS ${LIBSOURCEY_INCLUDE_DIRS} "${LIBSOURCEY_INCLUDE_DIRS}/3rdparty")
# Include our third party install directory
#set(LIBSOURCEY_INCLUDE_DIRS ${LIBSOURCEY_INCLUDE_DIRS} "${LIBSOURCEY_INSTALL_PREFIX}/3rdparty")
#include_directories("${LIBSOURCEY_INCLUDE_DIRS}")

# Include our third party library install directory
link_directories("${LIBSOURCEY_INSTALL_PREFIX}/lib")


# ----------------------------------------------------------------------------
# Module includes
# ----------------------------------------------------------------------------
include(LibSourceyModule.cmake REQUIRED)

#set(USING_POCO OFF)
#message("INIT: ${USING_POCO}")
  
  
# ----------------------------------------------------------------------------
# LibSourcey modules
# ----------------------------------------------------------------------------
set(BUILD_MODULES ON CACHE BOOL "Build core modules")
SUBDIRLIST(MODULES "${CMAKE_SOURCE_DIR}/modules")
if(BUILD_MODULES)

  
  # Build modules sample apps:
  # ---------------------------------------------------
  set(BUILD_MODULE_SAMPLES OFF CACHE BOOL "Build all samples")
  if(BUILD_MODULE_SAMPLES)
    include(LibSourceySample.cmake REQUIRED)
  endif()
  

  # Build modules tests:
  # ---------------------------------------------------
  set(BUILD_MODULE_TESTS OFF CACHE BOOL "Build tests")
  if(BUILD_MODULE_TESTS)
    include(LibSourceyTest.cmake REQUIRED)
  endif()  
  

  # Build modules:
  # ---------------------------------------------------
  FOREACH(module ${MODULES})
    message(STATUS "Including Module: ${module}")
    if (EXISTS "${CMAKE_SOURCE_DIR}/modules/${module}/CMakeLists.txt")
      set(BUILD_MODULE_${module} ON CACHE BOOL "Build Module: ${module}")
      mark_as_advanced(FORCE BUILD_MODULE_${module})
      if(BUILD_MODULE_${module})      
        ADD_SUBDIRECTORY("${CMAKE_SOURCE_DIR}/modules/${module}")
      endif()  
    endif()
  ENDFOREACH()  
  
else()
  
  message(STATUS "Not building modules")

  # Hide module definitions:
  # ---------------------------------------------------
  unset(BUILD_MODULE_SAMPLES CACHE)
  unset(BUILD_MODULE_TESTS CACHE)  
  FOREACH(module ${MODULES})
    unset(BUILD_MODULE_${module} CACHE)
  ENDFOREACH()  
  
endif()


# ----------------------------------------------------------------------------
# LibSourcey projects
# ----------------------------------------------------------------------------
set(BUILD_PROJECTS ON CACHE BOOL "Build projects")
SUBDIRLIST(PROJECTS "${CMAKE_SOURCE_DIR}/projects")
if(BUILD_PROJECTS)

  # Include project folders:
  # ---------------------------------------------------
  FOREACH(project ${PROJECTS})
    message(STATUS "Including Project: ${project}")
    if (EXISTS "${CMAKE_SOURCE_DIR}/projects/${project}/CMakeLists.txt")      
      set(BUILD_PROJECT_${project} ON CACHE BOOL "Build Project: ${project}")
      mark_as_advanced(FORCE BUILD_PROJECT_${project})
      if(BUILD_PROJECT_${project})      
        ADD_SUBDIRECTORY("${CMAKE_SOURCE_DIR}/projects/${project}")
      endif()  
    endif()
  ENDFOREACH()  
  
else()

  # Hide project definitions:
  # ---------------------------------------------------
  FOREACH(project ${PROJECTS})
    unset(BUILD_PROJECT_${project} CACHE)
  ENDFOREACH()  
  
endif()


# ----------------------------------------------------------------------------
# Build config file
#
# A directory will be created for each platform so the "Config.h" file is
# not overwritten if cmake generates code in the same path.
# ----------------------------------------------------------------------------
add_definitions(-DHAVE_CONFIG_H)

# Variables for Config.h.cmake
set(PACKAGE "LibSourcey")
set(PACKAGE_BUGREPORT "https://github.com/sourcey/LibSourcey/issues")
set(PACKAGE_NAME "LibSourcey")
set(PACKAGE_STRING "${PACKAGE} ${LIBSOURCEY_VERSION}")
set(PACKAGE_TARNAME "${PACKAGE}")
set(PACKAGE_VERSION "${LIBSOURCEY_VERSION}")

set(LIBSOURCEY_CONFIG_FILE_INCLUDE_DIR "${CMAKE_BINARY_DIR}/" CACHE PATH "Where to create the platform-dependant Config.h")

message(STATUS "Parsing 'Config.h.cmake'")
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/Config.h.cmake" 
  "${LIBSOURCEY_CONFIG_FILE_INCLUDE_DIR}/Config.h")
  
include_directories("${LIBSOURCEY_CONFIG_FILE_INCLUDE_DIR}")


# ----------------------------------------------------------------------------
#  Uninstall target, for "make uninstall"
# ----------------------------------------------------------------------------
if(UNIX)
  CONFIGURE_FILE(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

  ADD_CUSTOM_TARGET(uninstall
    "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
endif()



# ----------------------------------------------------------------------------
#  Summary:
# ----------------------------------------------------------------------------
message(STATUS "")
message(STATUS "General configuration for LibSourcey ${LIBSOURCEY_VERSION} =====================================")
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
message(STATUS "    Config.h is in:             ${LIBSOURCEY_CONFIG_FILE_INCLUDE_DIR}")
message(STATUS "-----------------------------------------------------------------")
message(STATUS "")

# Warn in the case of in-source build
if("${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")
    message(WARNING "The source directory is the same as binary directory. \"make clean\" may damage the source tree")
endif()


#----------------------------------------------------------------------------
#  Generate the LibSourceyConfig.cmake file for unix
#     installation in CMAKE_INSTALL_PREFIX
#----------------------------------------------------------------------------
