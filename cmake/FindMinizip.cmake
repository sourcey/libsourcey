########################################################################
# CMake module for finding minizip
#
# The following variables will be defined:
#
#  MINIZIP_FOUND
#  MINIZIP_INCLUDE_DIR
#  MINIZIP_LIBRARY
#

find_path(MINIZIP_INCLUDE_DIR
  NAMES zip.h
  PATHS ${LibSourcey_SOURCE_DIR}/archo/vendor/minizip
  NO_DEFAULT_PATH)

find_library(MINIZIP_LIBRARY
  NAMES minizip libminizip
    PATHS ${LibSourcey_BUILD_DIR}/src/archo/vendor/minizip
    NO_DEFAULT_PATH)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MINIZIP DEFAULT_MSG MINIZIP_LIBRARY MINIZIP_INCLUDE_DIR)
