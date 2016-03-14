########################################################################
# CMake module for finding libuv
#
# The following variables will be defined:
#
#  LIBUV_FOUND
#  LIBUV_INCLUDE_DIR
#  LIBUV_LIBRARY
#

find_path(LIBUV_INCLUDE_DIR
  NAMES uv.h
  PATHS ${LibSourcey_SOURCE_DIR}/src/uv/vendor/libuv)
find_library(LIBUV_LIBRARY
  NAMES libuv
  PATHS ${LibSourcey_BUILD_DIR}/src/uv/vendor/libuv)

# libuv uv

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LIBUV DEFAULT_MSG LIBUV_LIBRARY LIBUV_INCLUDE_DIR)
