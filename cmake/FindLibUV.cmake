########################################################################
# CMake module for finding libuv
#
# The following variables will be defined:
#
#  LIBUV_FOUND
#  LIBUV_INCLUDE_DIR
#  LIBUV_LIBRARIES
#

find_path(LIBUV_INCLUDE_DIR
  NAMES uv.h
  PATHS
    ${LibSourcey_DIR}/vendor/libuv/include
    ${LibSourcey_DIR}/share/libsourcey/vendor/include
  NO_DEFAULT_PATH)

find_library(LIBUV_LIBRARY_DEBUG
  NAMES libuvd uvd
  PATHS
    ${LibSourcey_BUILD_DIR}/vendor/libuv
    ${LibSourcey_DIR}/share/libsourcey/vendor/lib
  PATH_SUFFIXES Debug
  NO_DEFAULT_PATH)

find_library(LIBUV_LIBRARY_RELEASE
  NAMES libuv uv
  PATHS
    ${LibSourcey_BUILD_DIR}/vendor/libuv
    ${LibSourcey_DIR}/share/libsourcey/vendor/lib
  PATH_SUFFIXES Release
  NO_DEFAULT_PATH)

include(${CMAKE_ROOT}/Modules/SelectLibraryConfigurations.cmake)
select_library_configurations(LIBUV)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LIBUV DEFAULT_MSG LIBUV_LIBRARIES LIBUV_INCLUDE_DIR)
