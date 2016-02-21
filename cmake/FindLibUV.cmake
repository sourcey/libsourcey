########################################################################
# CMake module for finding libuv
#
# The following variables will be defined:
#
#  LIBUV_FOUND
#  LIBUV_INCLUDE_DIR
#  LIBUV_LIBRARY
#

find_path(LIBUV_INCLUDE_DIR NAMES uv.h)
find_library(LIBUV_LIBRARY NAMES uv libuv)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LIBUV DEFAULT_MSG LIBUV_LIBRARY LIBUV_INCLUDE_DIR)