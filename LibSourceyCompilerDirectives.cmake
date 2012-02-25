# ----------------------------------------------------------------------
#  Root CMake file for LibSourcey
#
#    From the off-tree build directory, invoke:
#      $ cmake <PATH_TO_LIBSOURCEY_ROOT>
#
# ----------------------------------------------------------------------

set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS true)

# Add these standard paths to the search paths for FIND_LIBRARY
# to find libraries from these locations first
if(UNIX)
    set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} /lib /usr/lib)
endif()

# This _must_ go before PROJECT(LibSourcey) in order to work
if(WIN32)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Installation Directory")
else()
    set(CMAKE_INSTALL_PREFIX "/usr/local" CACHE PATH "Installation Directory")
endif()

set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)
set(CMAKE_C_FLAGS_MINSIZEREL "" CACHE INTERNAL "" FORCE)
set(CMAKE_C_FLAGS_RELWITHDEBINFO "" CACHE INTERNAL "" FORCE)
set(CMAKE_CXX_FLAGS_MINSIZEREL "" CACHE INTERNAL "" FORCE)
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "" CACHE INTERNAL "" FORCE)
set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "" CACHE INTERNAL "" FORCE)
set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "" CACHE INTERNAL "" FORCE)
set(CMAKE_MODULE_LINKER_FLAGS_MINSIZEREL "" CACHE INTERNAL "" FORCE)
set(CMAKE_MODULE_LINKER_FLAGS_RELWITHDEBINFO "" CACHE INTERNAL "" FORCE)
set(CMAKE_SHARED_LINKER_FLAGS_MINSIZEREL "" CACHE INTERNAL "" FORCE)
set(CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO "" CACHE INTERNAL "" FORCE)
set(CMAKE_VERBOSE OFF CACHE BOOL "Verbose mode")

if(WIN32)
    if(${CMAKE_GENERATOR} MATCHES "(MinGW)|(MSYS)")
        set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG" CACHE STRING "")
    endif()
endif()

if(CMAKE_VERBOSE)
    set(CMAKE_VERBOSE_MAKEFILE 1)
endif()



project(LibSourcey)

cmake_minimum_required(VERSION 2.8)

if(MSVC)
    set(CMAKE_USE_RELATIVE_PATHS ON CACHE INTERNAL "" FORCE)
endif()

add_definitions(-DUNICODE -D_UNICODE)


# --------------------------------------------------------------
# Indicate CMake 2.7 and above that we don't want to mix relative
#  and absolute paths in linker lib lists.
# Run "cmake --help-policy CMP0003" for more information.
# --------------------------------------------------------------
if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif()

# ----------------------------------------------------------------------
# Build static or dynamic libs?
# Default: dynamic libraries
# ----------------------------------------------------------------------
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries (.dll/.so) instead of static ones (.lib/.a)")

# ----------------------------------------------------------------------
# Include debug info into debug libs?
# Default: yes
# ----------------------------------------------------------------------
set(BUILD_WITH_DEBUG_INFO ON CACHE BOOL "Include debug info into debug libs")

# ----------------------------------------------------------------------
#  Current version number:
# ----------------------------------------------------------------------
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

# ----------------------------------------------------------------------
# Use statically or dynamically linked CRT?
# Default: dynamic
# ----------------------------------------------------------------------
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


# ----------------------------------------------------------------------
#  Variables for config.h.cmake
# ----------------------------------------------------------------------
set(PACKAGE "LibSourcey")
#set(PACKAGE_BUGREPORT "libsourceylibrary-devel@lists.sourceforge.net")
set(PACKAGE_NAME "LibSourcey")
set(PACKAGE_STRING "${PACKAGE} ${LIBSOURCEY_VERSION}")
set(PACKAGE_TARNAME "${PACKAGE}")
set(PACKAGE_VERSION "${LIBSOURCEY_VERSION}")


# ----------------------------------------------------------------------
# Detect Microsoft compiler:
# ----------------------------------------------------------------------
if(CMAKE_CL_64)
    set(MSVC64 1)
endif()


# ----------------------------------------------------------------------
# Detect GNU version:
# ----------------------------------------------------------------------
if(CMAKE_COMPILER_IS_GNUCXX)
    execute_process(COMMAND ${CMAKE_CXX_COMPILER} --version
                  OUTPUT_VARIABLE CMAKE_LIBSOURCEY_GCC_VERSION_FULL
                  OUTPUT_STRIP_TRAILING_WHITESPACE)

    execute_process(COMMAND ${CMAKE_CXX_COMPILER} -v
                  ERROR_VARIABLE CMAKE_LIBSOURCEY_GCC_INFO_FULL
                  OUTPUT_STRIP_TRAILING_WHITESPACE)

    # Typical output in CMAKE_LIBSOURCEY_GCC_VERSION_FULL: "c+//0 (whatever) 4.2.3 (...)"
    #  Look for the version number
    string(REGEX MATCH "[0-9].[0-9].[0-9]" CMAKE_GCC_REGEX_VERSION "${CMAKE_LIBSOURCEY_GCC_VERSION_FULL}")

    # Split the three parts:
    string(REGEX MATCHALL "[0-9]" CMAKE_LIBSOURCEY_GCC_VERSIONS "${CMAKE_GCC_REGEX_VERSION}")

    list(GET CMAKE_LIBSOURCEY_GCC_VERSIONS 0 CMAKE_LIBSOURCEY_GCC_VERSION_MAJOR)
    list(GET CMAKE_LIBSOURCEY_GCC_VERSIONS 1 CMAKE_LIBSOURCEY_GCC_VERSION_MINOR)

    set(CMAKE_LIBSOURCEY_GCC_VERSION ${CMAKE_LIBSOURCEY_GCC_VERSION_MAJOR}${CMAKE_LIBSOURCEY_GCC_VERSION_MINOR})
    math(EXPR CMAKE_LIBSOURCEY_GCC_VERSION_NUM "${CMAKE_LIBSOURCEY_GCC_VERSION_MAJOR}*100 + ${CMAKE_LIBSOURCEY_GCC_VERSION_MINOR}")
    message(STATUS "Detected version of GNU GCC: ${CMAKE_LIBSOURCEY_GCC_VERSION} (${CMAKE_LIBSOURCEY_GCC_VERSION_NUM})")

    if(WIN32)
        execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpmachine
                  OUTPUT_VARIABLE CMAKE_LIBSOURCEY_GCC_TARGET_MACHINE
                  OUTPUT_STRIP_TRAILING_WHITESPACE)
        if(CMAKE_LIBSOURCEY_GCC_TARGET_MACHINE MATCHES "64")
            set(MINGW64 1)
        endif()
    endif()

endif()


# ----------------------------------------------------------------------
#       CHECK FOR SYSTEM LIBRARIES, OPTIONS, ETC..
# ----------------------------------------------------------------------

# Build/install (or not) some apps:
# ===================================================
set(BUILD_EXAMPLES OFF CACHE BOOL "Build all examples")

# Build tests:
# ===================================================
set(BUILD_TESTS ON CACHE BOOL "Build tests")

# Build 3rdparty libraries under unix
# ===================================================
if(WIN32)
    set(LIBSOURCEY_BUILD_3RDPARTY_LIBS TRUE  CACHE BOOL "Build 3rd party libraries")
else()
    set(LIBSOURCEY_BUILD_3RDPARTY_LIBS FALSE CACHE BOOL "Build 3rd party libraries")
endif()

include(LibSourceyModule.cmake REQUIRED)

if(UNIX)
    include(LibSourceyFindPkgConfig.cmake OPTIONAL)
    include(CheckFunctionExists)
    include(CheckIncludeFile)
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
    set(ENABLE_PROFILING OFF CACHE BOOL "Enable profiling in the GCC compiler (Add flags: -g -pg)")
    set(USE_OMIT_FRAME_POINTER ON CACHE BOOL "Enable -fomit-frame-pointer for GCC")

    if(${CMAKE_SYSTEM_PROCESSOR} MATCHES amd64*|x86_64*)
        set(X86_64 1)
    endif()

    if(${CMAKE_SYSTEM_PROCESSOR} MATCHES i686*|i386*|x86*)
        set(X86 1)
    endif()

    if(${CMAKE_SYSTEM_PROCESSOR} MATCHES powerpc*)
        set(ENABLE_POWERPC ON CACHE BOOL "Enable PowerPC for GCC")
    endif()

    if(X86 OR X86_64)
        # enable everything, since the available set of instructions is checked at runtime
        set(USE_FAST_MATH ON CACHE BOOL "Enable -ffast-math")
        set(ENABLE_SSE ON CACHE BOOL "Enable SSE instructions")
        set(ENABLE_SSE2 ON CACHE BOOL "Enable SSE2 instructions")
        set(ENABLE_SSE3 OFF CACHE BOOL "Enable SSE3 instructions")
        set(ENABLE_SSSE3 OFF CACHE BOOL "Enable SSSE3 instructions")
        set(ENABLE_SSE41 OFF CACHE BOOL "Enable SSE4.1 instructions")
        set(ENABLE_SSE42 OFF CACHE BOOL "Enable SSE4.2 instructions")
    endif()
endif()

if(MSVC)
    set(ENABLE_SSE ON CACHE BOOL "Enable SSE instructions for MSVC")
    set(ENABLE_SSE2 ON CACHE BOOL "Enable SSE2 instructions for MSVC")
    if(CMAKE_C_COMPILER MATCHES "icc")
        set(ENABLE_SSE3 OFF CACHE BOOL "Enable SSE3 instructions for ICC")
        set(ENABLE_SSE4_1 OFF CACHE BOOL "Enable SSE4.1 instructions for ICC")
    endif()
endif()


# ---------------------------------------------------------------------------
# Include third party libraries:
# ---------------------------------------------------------------------------
set(LIBSOURCEY_SOURCE_DIR ${CMAKE_SOURCE_DIR})
set(LIBSOURCEY_BUILD_DIR ${CMAKE_BINARY_DIR})

set(WITH_POCO ON CACHE BOOL "Build with Poco")
if(WITH_POCO)
    include(LibSourceyFindPoco.cmake)   
endif()

set(WITH_FFMPEG ON CACHE BOOL "Build with FFmpeg")
if(WITH_FFMPEG)
    include(LibSourceyFindFFmpeg.cmake)
endif()

set(WITH_OPENCV ON CACHE BOOL "Build with OpenCV")
if(WITH_OPENCV)
    include(LibSourceyFindOpenCV.cmake)
endif()

set(WITH_OPENSSL ON CACHE BOOL "Build with OpenSSL")
if(WITH_OPENSSL)
    include(LibSourceyFindOpenSSL.cmake)
endif()

set(WITH_PUGIXML ON CACHE BOOL "Build with PugiXML")
if(WITH_PUGIXML)
    include(LibSourceyFindPugiXML.cmake)
endif()

set(WITH_RTAUDIO ON CACHE BOOL "Build with RtAudio")
if(WITH_RTAUDIO)
    include(LibSourceyFindRtAudio.cmake)
endif()

set(WITH_LIBSTROPHE ON CACHE BOOL "Build with LibStrophe")
if(WITH_LIBSTROPHE)
    include(LibSourceyFindLibStrophe.cmake)
endif()


# ---------------------------------------------------------------------------
# Additional Libraries:
# ---------------------------------------------------------------------------
if(WIN32)
    set(LIBSOURCEY_DEBUG_LIBS ${LIBSOURCEY_DEBUG_LIBS} ws2_32)  
    set(LIBSOURCEY_DEBUG_LIBS ${LIBSOURCEY_DEBUG_LIBS} dsound)  
    set(LIBSOURCEY_RELEASE_LIBS ${LIBSOURCEY_RELEASE_LIBS} ws2_32)  
    set(LIBSOURCEY_RELEASE_LIBS ${LIBSOURCEY_RELEASE_LIBS} dsound) 
endif()


# ----------------------------------------------------------------------
#                   UPDATE CONFIG FILES & SCRIPTS:
#
#  CONFIGURE_FILE(InputFile OutputFile [COPYONLY] [ESCAPE_QUOTES] [@ONLY])
# If @ONLY is specified, only variables of the form @VAR@ will be
#  replaces and ${VAR} will be ignored.
#
#  A directory will be created for each platform so the "config.h" file is
#   not overwritten if cmake generates code in the same path.
# ----------------------------------------------------------------------
add_definitions(-DHAVE_CONFIG_H)

set(LIBSOURCEY_CONFIG_FILE_INCLUDE_DIR "${CMAKE_BINARY_DIR}/" CACHE PATH "Where to create the platform-dependant config.h")

message(STATUS "Parsing 'config.h.cmake'")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake" "${LIBSOURCEY_CONFIG_FILE_INCLUDE_DIR}/config.h")
include_directories("${LIBSOURCEY_CONFIG_FILE_INCLUDE_DIR}")


# ----------------------------------------------------------------------
#           Set the maximum level of warnings:
# ----------------------------------------------------------------------
# Should be set to true for development
set(LIBSOURCEY_WARNINGS_ARE_ERRORS OFF CACHE BOOL "Treat warnings as errors")

set(EXTRA_C_FLAGS "")
set(EXTRA_C_FLAGS_RELEASE "")
set(EXTRA_C_FLAGS_DEBUG "")
set(EXTRA_EXE_LINKER_FLAGS "")
set(EXTRA_EXE_LINKER_FLAGS_RELEASE "")
set(EXTRA_EXE_LINKER_FLAGS_DEBUG "")

if(MSVC)
    set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} /D _CRT_SECURE_NO_DEPRECATE /D _CRT_NONSTDC_NO_DEPRECATE /D _SCL_SECURE_NO_WARNINGS")
    # 64-bit portability warnings, in MSVC8
    if(MSVC80)
        set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} /Wp64")

    endif()
    #if(MSVC90)
    #    set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} /D _BIND_TO_CURRENT_CRT_VERSION=1 /D _BIND_TO_CURRENT_VCLIBS_VERSION=1")
    #endif()

    if(BUILD_WITH_DEBUG_INFO)
        set(EXTRA_EXE_LINKER_FLAGS_RELEASE "${EXTRA_EXE_LINKER_FLAGS_RELEASE} /debug")
    endif()

    # Remove unreferenced functions: function level linking
    set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} /Gy")
    if(BUILD_WITH_DEBUG_INFO)
       set(EXTRA_C_FLAGS_RELEASE "${EXTRA_C_FLAGS_RELEASE} /Zi")
    endif()
endif()

if(CMAKE_COMPILER_IS_GNUCXX)

    # High level of warnings.
    set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -Wall")

    if(BUILD_WITH_STATIC_CRT)
        set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -static-libstdc++")
    endif()

    # The -Wno-long-long is required in 64bit systems when including sytem headers.
    if(${CMAKE_SYSTEM_PROCESSOR} MATCHES x86_64*)
    set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -Wno-long-long")
    endif()
    if(${CMAKE_SYSTEM_PROCESSOR} MATCHES amd64*)
    set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -Wno-long-long")
    endif()

    # We need pthread's
    if(UNIX)
      if(NOT ANDROID)
        set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -pthread")
      endif()
    endif()

    if(LIBSOURCEY_WARNINGS_ARE_ERRORS)
        set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -Werror")
    endif()

    if(X86)
        if(NOT MINGW64)
            if(NOT X86_64)
                if(NOT APPLE)
                    set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -march=i686")
                endif()
            endif()
        endif()
    endif()

    # Other optimizations
    if(USE_OMIT_FRAME_POINTER)
        set(EXTRA_C_FLAGS_RELEASE "${EXTRA_C_FLAGS_RELEASE} -fomit-frame-pointer")
    endif()
    if(USE_FAST_MATH)
        set(EXTRA_C_FLAGS_RELEASE "${EXTRA_C_FLAGS_RELEASE} -ffast-math")
    endif()
    if(ENABLE_POWERPC)
        set(EXTRA_C_FLAGS_RELEASE "${EXTRA_C_FLAGS_RELEASE} -mcpu=G3 -mtune=G5")
    endif()
    if(ENABLE_SSE)
        set(EXTRA_C_FLAGS_RELEASE "${EXTRA_C_FLAGS_RELEASE} -msse")
    endif()
    if(ENABLE_SSE2)
        set(EXTRA_C_FLAGS_RELEASE "${EXTRA_C_FLAGS_RELEASE} -msse2")
    endif()
    # SSE3 and further should be disabled under MingW because it generates compiler errors
    if(NOT MINGW)
        if(ENABLE_SSE3)
            set(EXTRA_C_FLAGS_RELEASE "${EXTRA_C_FLAGS_RELEASE} -msse3")
        endif()

        if(${CMAKE_LIBSOURCEY_GCC_VERSION_NUM} GREATER 402)
            set(HAVE_GCC43_OR_NEWER 1)
        endif()
        if(${CMAKE_LIBSOURCEY_GCC_VERSION_NUM} GREATER 401)
            set(HAVE_GCC42_OR_NEWER 1)
        endif()

        if(HAVE_GCC43_OR_NEWER OR APPLE)
            if(ENABLE_SSSE3)
                set(EXTRA_C_FLAGS_RELEASE "${EXTRA_C_FLAGS_RELEASE} -mssse3")
            endif()
            if(HAVE_GCC42_OR_NEWER)
                if(ENABLE_SSE41)
                    set(EXTRA_C_FLAGS_RELEASE "${EXTRA_C_FLAGS_RELEASE} -msse4.1")
                endif()
                if(ENABLE_SSE42)
                    set(EXTRA_C_FLAGS_RELEASE "${EXTRA_C_FLAGS_RELEASE} -msse4.2")
                endif()
            endif()
        endif()
    endif()

    if(X86 OR X86_64)
        if(NOT APPLE)
            if(${CMAKE_SIZEOF_VOID_P} EQUAL 4)
                set(EXTRA_C_FLAGS_RELEASE "${EXTRA_C_FLAGS_RELEASE} -mfpmath=387")
            endif()
        endif()
    endif()

    # Profiling?
    if(ENABLE_PROFILING)
        set(EXTRA_C_FLAGS_RELEASE "${EXTRA_C_FLAGS_RELEASE} -pg -g")
    else()
        # Remove unreferenced functions: function level linking
        if(NOT APPLE)
            set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -ffunction-sections")
        endif()
    endif()

    set(EXTRA_C_FLAGS_RELEASE "${EXTRA_C_FLAGS_RELEASE} -DNDEBUG")
    set(EXTRA_C_FLAGS_DEBUG "${EXTRA_C_FLAGS_DEBUG} -O0 -DDEBUG -D_DEBUG")

    if(BUILD_WITH_DEBUG_INFO)
       set(EXTRA_C_FLAGS_DEBUG "${EXTRA_C_FLAGS_DEBUG} -ggdb3")
    endif()
endif()

if(MSVC)
    # 64-bit MSVC compiler uses SSE/SSE2 by default
    if(NOT MSVC64)
       if(ENABLE_SSE)
          set(EXTRA_C_FLAGS_RELEASE "${EXTRA_C_FLAGS_RELEASE} /arch:SSE")
       endif()
       if(ENABLE_SSE2)
          set(EXTRA_C_FLAGS_RELEASE "${EXTRA_C_FLAGS_RELEASE} /arch:SSE2")
       endif()
    endif()
    if(ENABLE_SSE3)
       set(EXTRA_C_FLAGS_RELEASE "${EXTRA_C_FLAGS_RELEASE} /arch:SSE3")
    endif()
    if(ENABLE_SSE4_1)
       set(EXTRA_C_FLAGS_RELEASE "${EXTRA_C_FLAGS_RELEASE} /arch:SSE4.1")
    endif()
    if(ENABLE_SSE OR ENABLE_SSE2 OR ENABLE_SSE3 OR ENABLE_SSE4_1)
       set(EXTRA_C_FLAGS_RELEASE "${EXTRA_C_FLAGS_RELEASE} /Oi")
    endif()
endif()

# Extra link libs if the user selects building static libs:
if(NOT BUILD_SHARED_LIBS)
    if(CMAKE_COMPILER_IS_GNUCXX)
        set(LIBSOURCEY_DEBUG_LIBS ${LIBSOURCEY_DEBUG_LIBS} stdc++)
        set(LIBSOURCEY_EXTRA_C_FLAGS "${LIBSOURCEY_EXTRA_C_FLAGS} -fPIC")
    endif()
endif()


# Add user supplied extra options (optimization, etc...)
# ==========================================================
set(LIBSOURCEY_EXTRA_C_FLAGS "" CACHE STRING "Extra compiler options")
set(LIBSOURCEY_EXTRA_C_FLAGS_RELEASE "" CACHE STRING "Extra compiler options for Release build")
set(LIBSOURCEY_EXTRA_C_FLAGS_DEBUG "" CACHE STRING "Extra compiler options for Debug build")
set(LIBSOURCEY_EXTRA_EXE_LINKER_FLAGS "" CACHE STRING "Extra linker flags" FORCE)
set(LIBSOURCEY_EXTRA_EXE_LINKER_FLAGS_RELEASE "" CACHE STRING "Extra linker flags for Release build" FORCE)
set(LIBSOURCEY_EXTRA_EXE_LINKER_FLAGS_DEBUG "" CACHE STRING "Extra linker flags for Debug build" FORCE)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_C_FLAGS} ${LIBSOURCEY_EXTRA_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_C_FLAGS} ${LIBSOURCEY_EXTRA_C_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${EXTRA_C_FLAGS_RELEASE} ${LIBSOURCEY_EXTRA_C_FLAGS_RELEASE}")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${EXTRA_C_FLAGS_RELEASE} ${LIBSOURCEY_EXTRA_C_FLAGS_RELEASE}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${EXTRA_C_FLAGS_DEBUG} ${LIBSOURCEY_EXTRA_C_FLAGS_DEBUG}")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${EXTRA_C_FLAGS_DEBUG} ${LIBSOURCEY_EXTRA_C_FLAGS_DEBUG}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${EXTRA_EXE_LINKER_FLAGS} ${LIBSOURCEY_EXTRA_EXE_LINKER_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} ${EXTRA_EXE_LINKER_FLAGS_RELEASE} ${LIBSOURCEY_EXTRA_EXE_LINKER_FLAGS_RELEASE}")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} ${EXTRA_EXE_LINKER_FLAGS_DEBUG} ${LIBSOURCEY_EXTRA_EXE_LINKER_FLAGS_DEBUG}")

# In case of Makefiles if the user does not setup CMAKE_BUILD_TYPE, assume it's Release:
if(${CMAKE_GENERATOR} MATCHES ".*Makefiles")
    if("${CMAKE_BUILD_TYPE}" STREQUAL "")
        set(CMAKE_BUILD_TYPE Release)
    endif()
endif()

if(WIN32 AND MSVC)
    # avoid warnings from MSVC about overriding the /W* option
    # we replace /W3 with /W4 only for C++ files,
    # since all the 3rd-party libraries LibSourcey uses are in C,
    # and we do not care about their warnings.
    string(REPLACE "/W3" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    string(REPLACE "/W3" "/W4" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
    string(REPLACE "/W3" "/W4" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")

    # allow extern "C" functions throw exceptions
    string(REPLACE "/EHsc" "/EHsc-" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
    string(REPLACE "/EHsc" "/EHsc-" CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
    string(REPLACE "/EHsc" "/EHsc-" CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
    string(REPLACE "/EHsc" "/EHsc-" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    string(REPLACE "/EHsc" "/EHsc-" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
    string(REPLACE "/EHsc" "/EHsc-" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")

    string(REPLACE "/Zm1000" " " CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    string(REPLACE "/Zm1000" " " CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
endif()

if("${CMAKE_CONFIGURE_LDFLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${CMAKE_CONFIGURE_LDFLAGS}")
endif("${CMAKE_CONFIGURE_LDFLAGS}")


# ----------------------------------------------------------------------
#   Uninstall target, for "make uninstall"
# ----------------------------------------------------------------------
if(UNIX)
  CONFIGURE_FILE(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

  ADD_CUSTOM_TARGET(uninstall
    "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
endif()


#-----------------------------------
# Subdirectories:
#-----------------------------------
if(LIBSOURCEY_BUILD_3RDPARTY_LIBS)
    add_subdirectory(3rdparty)
endif()

if(EXISTS ${CMAKE_SOURCE_DIR}/Base)
    add_subdirectory(Base)
endif()

if(EXISTS ${CMAKE_SOURCE_DIR}/Net)
    add_subdirectory(Net)
endif()

if(EXISTS ${CMAKE_SOURCE_DIR}/Media)
    add_subdirectory(Media)
endif()

if(EXISTS ${CMAKE_SOURCE_DIR}/XML)
    add_subdirectory(XML)
endif()

if(EXISTS ${CMAKE_SOURCE_DIR}/XMPP)
    add_subdirectory(XMPP)
endif()

if(EXISTS ${CMAKE_SOURCE_DIR}/RTP)
    add_subdirectory(RTP)
endif()

if(EXISTS ${CMAKE_SOURCE_DIR}/STUN)
    add_subdirectory(STUN)
endif()

if(EXISTS ${CMAKE_SOURCE_DIR}/TURN)
    add_subdirectory(TURN)
endif()

if(EXISTS ${CMAKE_SOURCE_DIR}/SDP)
    add_subdirectory(SDP)
endif()

if(EXISTS ${CMAKE_SOURCE_DIR}/ICE)
    add_subdirectory(ICE)
endif()

if(EXISTS ${CMAKE_SOURCE_DIR}/ISpot)
    add_subdirectory(ISpot)
endif()

if(EXISTS ${CMAKE_SOURCE_DIR}/SpotImpl)
    add_subdirectory(SpotImpl)
endif()


# ----------------------------------------------------------------------
#   Summary:
# ----------------------------------------------------------------------
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
message(STATUS "    config.h is in:             ${LIBSOURCEY_CONFIG_FILE_INCLUDE_DIR}")
message(STATUS "-----------------------------------------------------------------")
message(STATUS "")

# Warn in the case of in-source build
if("${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")
    message(WARNING "The source directory is the same as binary directory. \"make clean\" may damage the source tree")
endif()


#----------------------------------------------------------------------------
#   Generate the LibSourceyConfig.cmake file for unix
#      installation in CMAKE_INSTALL_PREFIX
#----------------------------------------------------------------------------
