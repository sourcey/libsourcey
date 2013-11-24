# Install script for directory: /home/deploy/src/libsourcey

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/home/deploy/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "RELEASE")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  INCLUDE("/home/deploy/src/libsourcey/build/deps/libuv/cmake_install.cmake")
  INCLUDE("/home/deploy/src/libsourcey/build/deps/RtAudio/cmake_install.cmake")
  INCLUDE("/home/deploy/src/libsourcey/build/deps/jsoncpp/cmake_install.cmake")
  INCLUDE("/home/deploy/src/libsourcey/build/deps/zlib/cmake_install.cmake")
  INCLUDE("/home/deploy/src/libsourcey/build/deps/minizip/cmake_install.cmake")
  INCLUDE("/home/deploy/src/libsourcey/build/deps/http_parser/cmake_install.cmake")
  INCLUDE("/home/deploy/src/libsourcey/build/src/media/cmake_install.cmake")
  INCLUDE("/home/deploy/src/libsourcey/build/src/json/cmake_install.cmake")
  INCLUDE("/home/deploy/src/libsourcey/build/src/anionu-private/cmake_install.cmake")
  INCLUDE("/home/deploy/src/libsourcey/build/src/socketio/cmake_install.cmake")
  INCLUDE("/home/deploy/src/libsourcey/build/src/symple/cmake_install.cmake")
  INCLUDE("/home/deploy/src/libsourcey/build/src/sked/cmake_install.cmake")
  INCLUDE("/home/deploy/src/libsourcey/build/src/pacman/cmake_install.cmake")
  INCLUDE("/home/deploy/src/libsourcey/build/src/stun/cmake_install.cmake")
  INCLUDE("/home/deploy/src/libsourcey/build/src/crypto/cmake_install.cmake")
  INCLUDE("/home/deploy/src/libsourcey/build/src/base/cmake_install.cmake")
  INCLUDE("/home/deploy/src/libsourcey/build/src/net/cmake_install.cmake")
  INCLUDE("/home/deploy/src/libsourcey/build/src/util/cmake_install.cmake")
  INCLUDE("/home/deploy/src/libsourcey/build/src/uv/cmake_install.cmake")
  INCLUDE("/home/deploy/src/libsourcey/build/src/http/cmake_install.cmake")
  INCLUDE("/home/deploy/src/libsourcey/build/src/anionu-sdk/cmake_install.cmake")
  INCLUDE("/home/deploy/src/libsourcey/build/src/archo/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)

IF(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
ELSE(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
ENDIF(CMAKE_INSTALL_COMPONENT)

FILE(WRITE "/home/deploy/src/libsourcey/build/${CMAKE_INSTALL_MANIFEST}" "")
FOREACH(file ${CMAKE_INSTALL_MANIFEST_FILES})
  FILE(APPEND "/home/deploy/src/libsourcey/build/${CMAKE_INSTALL_MANIFEST}" "${file}\n")
ENDFOREACH(file)
