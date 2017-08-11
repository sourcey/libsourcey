# ============================================================================
# Root CMakeLists.txt file for LibSourcey
# ============================================================================

# CMake 2.8.10 or newer for other platforms.
# This version is available via Ubuntu's package manager for the 14.04 LTS.
cmake_minimum_required(VERSION 2.8.10)

# This must go before `project(LibSourcey)` in order to work.
if(WIN32)
  # CMake 3.4 or newer is required on windoes to make use of the
  # CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS value for shares releases.
  # cmake_minimum_required(VERSION 3.4)
  # set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
  set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Installation Directory")
else()
  # set(CMAKE_INSTALL_PREFIX "/usr/local" CACHE PATH "Installation Directory")
endif()

# set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)
# set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${CMAKE_CONFIGURATION_TYPES})

project(LibSourcey)

add_definitions(-DUNICODE -D_UNICODE)

# set(CMAKE_MACOSX_RPATH 1)

# Set warning level to W3
if(CMAKE_BUILD_TOOL MATCHES "(msdev|devenv|nmake)")
  add_definitions(/W3)
endif()

# Remove a couple of annoying CMake warnings
# cmake_policy(SET CMP0054 OLD)
# CMP0042


include(LibSourcey.cmake REQUIRED)
# include(cmake/LibSourceyVersion.cmake)

# ----------------------------------------------------------------------------
#  Package configuration
# ----------------------------------------------------------------------------

include(cmake/LibSourceyPackage.cmake)

# ----------------------------------------------------------------------------
#  Add Tests target
# ----------------------------------------------------------------------------
# if (BUILD_TESTS)
  set(CMAKE_CTEST_COMMAND ctest -V)
  add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})
# endif()

# ----------------------------------------------------------------------------
#  Add Doxygen documentation target
# ----------------------------------------------------------------------------
find_package(Doxygen)
if(DOXYGEN_FOUND)
  configure_file(
		${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile 
		${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
  add_custom_target(doc
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen" VERBATIM)
endif()

# ----------------------------------------------------------------------------
#  Uninstall target for "make uninstall"
# ----------------------------------------------------------------------------
if(UNIX)
  configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

  add_custom_target(uninstall
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
status("    C++ Compiler:"           CMAKE_COMPILER_IS_GNUCXX THEN "${CMAKE_CXX_COMPILER} (ver ${CMAKE_CXX_COMPILER_VERSION})" ELSE "${CMAKE_CXX_COMPILER}" )
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
status("     Samples:                    ${LibSourcey_BUILD_SAMPLES}")
status("     Tests:                      ${LibSourcey_BUILD_TESTS}")

# Dependencies
status("")
status("  Other third-party libraries:")
status("")
if (OPENSSL_IS_BORINGSSL)
  status("    Use BoringSSL:"     HAVE_BORINGSSL       THEN YES ELSE NO)
else()
  status("    Use OpenSSL:"       HAVE_OPENSSL         THEN YES ELSE NO)
endif()
status("    Use FFmpeg:"        HAVE_FFMPEG          THEN YES ELSE NO)
status("    Use OpenCV:"        HAVE_OPENCV          THEN "YES (ver ${OPENCV_VERSION})" ELSE NO)
status("    Use WebRTC:"        HAVE_WEBRTC          THEN YES ELSE NO)
status("    Use Poco:"          HAVE_POCO            THEN YES ELSE NO)
status("    Use wxWidgets:"     HAVE_WXWIDGETS       THEN YES ELSE NO)
# status("    Use LibUV:"         HAVE_LIBUV           THEN YES ELSE NO)
# status("    Use HttpParser:"    HAVE_HTTPPARSER      THEN YES ELSE NO)
# status("    Use RtAudio:"       HAVE_RTAUDIO         THEN YES ELSE NO)
# status("    Use JsonCpp:"       HAVE_JSONCPP         THEN YES ELSE NO)
# status("    Use zlib:"          HAVE_ZLIB            THEN YES ELSE NO)

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
status("    Install path:                ${LibSourcey_INSTALL_DIR}")
status("")
status("    libsourcey.h is in:          ${LibSourcey_INSTALL_DIR}/include/libsourcey.h")
status("-----------------------------------------------------------------")
status("")

# Warn in the case of in-source build
if("${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")
  message(WARNING "The source directory is the same as binary directory. \"make clean\" may damage the source tree")
endif()

# This is a proper place for make iOS build happy. It cannot be moved to the top
if(IOS)
  set(SDKROOT "iphoneos")
  set(CMAKE_OSX_SYSROOT ${SDKROOT})
endif()
