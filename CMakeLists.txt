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
set(LibSourcey_SOURCE_DIR ${CMAKE_SOURCE_DIR})
set(LibSourcey_BUILD_DIR ${CMAKE_BINARY_DIR})
set(LibSourcey_INSTALL_DIR ${CMAKE_INSTALL_PREFIX})
set(LibSourcey_DEPENDENCIES_SOURCE_DIR "${LibSourcey_SOURCE_DIR}/deps")
set(LibSourcey_DEPENDENCIES_BUILD_DIR "${LibSourcey_INSTALL_DIR}/deps")
set(LibSourcey_DEPENDENCIES_INSTALL_DIR "${LibSourcey_INSTALL_DIR}/share/LibSourcey/deps")
#set(LibSourcey_INCLUDE_DIRS "") # Variable for all include dirs
#set(LibSourcey_LIBRARY_DIRS "") # Variable for all library dirs
#set(LibSourcey_DEBUG_LIBS "")   # Variable for all debug libraries
#set(LibSourcey_RELEASE_LIBS "") # Variable for all release libraries

# Set this for find_package to use our FindXXX.cmake files
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR})

# Append some common search paths for find_library
# to find libraries from these locations first
if(WIN32)
  set(CMAKE_SYSTEM_PREFIX_PATH ${CMAKE_SYSTEM_PREFIX_PATH}
      ${LibSourcey_DEPENDENCIES_SOURCE_DIR}
      D:/dev/apps/MinGW/msys/1.0/local
      D:/dev/lib/OpenCV2.3/build
      D:/dev/lib/wxWidgets-2.9.2/lib/vc_lib
      D:/dev/lib/ffmpeg/lib
      )
else()
    set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} 
      /lib 
      /usr/lib)
endif()
      #D:/dev/lib/OpenSSL-Win32/lib/VC/
      #D:/dev/lib/poco-1.4.1p1-all #/lib

# Append some common search paths for find_path
# to search these locations first
if(WIN32)
  set(CMAKE_SYSTEM_PREFIX_PATH ${CMAKE_SYSTEM_PREFIX_PATH}
      ${LibSourcey_DEPENDENCIES_SOURCE_DIR}
      D:/dev/apps/MinGW/msys/1.0/local
      D:/dev/lib/OpenCV2.3/build/include
      D:/dev/lib/wxWidgets-2.9.2
      D:/dev/lib/ffmpeg)
endif()

      #D:/dev/lib/OpenSSL-Win32/include
      #D:/dev/lib/poco-1.4.1p1-all
      
set(LibSourcey_ARCHITECTURE i386)
set(LibSourcey_BITNESS 32)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(LibSourcey_BITNESS 64)
  set(LibSourcey_ARCHITECTURE amd64)
endif()

include(LibSourceyIncludeDependency.cmake OPTIONAL)
include(LibSourceyIncludeModule.cmake OPTIONAL)


# ----------------------------------------------------------------------------
# Build deps libraries
#
# TODO: Move to /deps cmakelists.txt
# ----------------------------------------------------------------------------
#if(WIN32)
    set(BUILD_DEPENDENCIES_LIBS TRUE CACHE BOOL "Build 3rd party libraries")
#else()
#    set(BUILD_DEPENDENCIES_LIBS FALSE CACHE BOOL "Build 3rd party libraries")
#endif()
if(BUILD_DEPENDENCIES_LIBS)
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
  #set(LibSourcey_INCLUDE_DIRS ${LibSourcey_INCLUDE_DIRS} "${LibSourcey_DEPENDENCIES_SOURCE_DIR}/MemLeakDetect")
  add_subdirectory(${LibSourcey_DEPENDENCIES_SOURCE_DIR})
else()
  unset(WITH_PUGIXML CACHE)
  unset(WITH_RTAUDIO CACHE)
  unset(WITH_LIBSTROPHE CACHE)
  unset(WITH_JSONCPP CACHE)
  unset(WITH_OPENSSL CACHE)
endif()
  
# ============================================================================
# macro for looping sub directories
#
# Example:
#  subdirlist(SUBDIRS ${MY_CURRENT_DIR})
# ============================================================================
macro(subdirlist result curdir)
  file(GLOB children RELATIVE ${curdir} ${curdir}/*)
  set(dirlist "")
  foreach(child ${children})
    if(IS_DIRECTORY ${curdir}/${child})
        set(dirlist ${dirlist} ${child})
    endif()
  endforeach()
  set(${result} ${dirlist})
endmacro()


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
# Include third party libraries
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
set(BUILD_MODULES OFF CACHE BOOL "Build LibSourcey Modules")
if(BUILD_MODULES)  

  # Build sample apps
  # ---------------------------------------------------
  set(BUILD_MODULE_SAMPLES OFF CACHE BOOL "Build all samples")
  if(BUILD_MODULE_SAMPLES)
    include(LibSourceySample.cmake REQUIRED)
  endif()


  # Build tests
  # ---------------------------------------------------
  set(BUILD_MODULE_TESTS OFF CACHE BOOL "Build tests")
  if(BUILD_MODULE_TESTS)
    include(LibSourceyTest.cmake REQUIRED)
  endif()
  
endif()  

  
set(BUILD_APPLICATIONS OFF CACHE BOOL "Build LibSourcey Applications")
  

# ----------------------------------------------------------------------------
# Include the LibSourcey source tree
# ----------------------------------------------------------------------------
if(BUILD_MODULES OR BUILD_APPLICATIONS)  
  
  include(LibSourceyModule.cmake REQUIRED)
  subdirlist(subdirs "${CMAKE_SOURCE_DIR}/libs")

  foreach(module ${subdirs})
    message(STATUS "Including Module: ${module}")
    if (EXISTS "${CMAKE_SOURCE_DIR}/libs/${module}/CMakeLists.txt")      
      add_subdirectory("${CMAKE_SOURCE_DIR}/libs/${module}")
    endif()
  endforeach() 
endif()



#subdirlist(project_subdirs "${CMAKE_SOURCE_DIR}/projects")


#set(module_dirs ${MODULES} ${PROJECTS})  
#if(BUILD_MODULES)  
#
#  # Build modules:
#  # ---------------------------------------------------
#  foreach(module ${MODULES})
#    message(STATUS "Including Module: ${module}")
#    if (EXISTS "${CMAKE_SOURCE_DIR}/modules/${module}/CMakeLists.txt")
#      set(BUILD_MODULE_${module} ON CACHE BOOL "Build Module: ${module}")
#      mark_as_advanced(FORCE BUILD_MODULE_${module})
#      if(BUILD_MODULE_${module})      
#        add_subdirectory("${CMAKE_SOURCE_DIR}/modules/${module}")
#      endif()  
#    endif()
#  endforeach()  
#  
#else()
#
#  # Unset core module definitions:
#  # ---------------------------------------------------
#  foreach(module ${MODULES})
#    unset(BUILD_MODULE_${module} CACHE)
#  endforeach()  
#  
#endif()


# ----------------------------------------------------------------------------
# LibSourcey project applications
# ----------------------------------------------------------------------------
#set(BUILD_APPLICATIONS OFF CACHE BOOL "Build project applications")
#if(BUILD_APPLICATIONS)

#  # Include project folders:
#  # ---------------------------------------------------
#  foreach(project ${PROJECTS})
#    message(STATUS "Including Project: ${project}")
#    if (EXISTS "${CMAKE_SOURCE_DIR}/projects/${project}/CMakeLists.txt")      
#      set(BUILD_PROJECT_${project} ON CACHE BOOL "Build Project: ${project}")
#      mark_as_advanced(FORCE BUILD_PROJECT_${project})
#      if(BUILD_PROJECT_${project})      
#        add_subdirectory("${CMAKE_SOURCE_DIR}/projects/${project}")
#      endif()  
#    endif()
#  endforeach()  
#  
#else()

  # Hide project definitions:
  # ---------------------------------------------------
#  foreach(project ${PROJECTS})
#    unset(BUILD_PROJECT_${project} CACHE)
#  endforeach()  
#  
#endif()


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
set(PACKAGE_STRING "${PACKAGE} ${LibSourcey_VERSION}")
set(PACKAGE_TARNAME "${PACKAGE}")
set(PACKAGE_VERSION "${LibSourcey_VERSION}")

set(LibSourcey_CONFIG_FILE_INCLUDE_DIR "${CMAKE_BINARY_DIR}/" CACHE PATH "Where to create the platform-dependant Config.h")

message(STATUS "Parsing 'Config.h.cmake'")
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/Config.h.cmake" 
  "${LibSourcey_CONFIG_FILE_INCLUDE_DIR}/Config.h")
  
include_directories("${LibSourcey_CONFIG_FILE_INCLUDE_DIR}")


# ----------------------------------------------------------------------------
#  Uninstall target for "make uninstall"
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
