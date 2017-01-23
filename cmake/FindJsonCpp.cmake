########################################################################
# CMake module for finding JsonCpp
#
# The following variables will be defined:
#
#  JSONCPP_FOUND
#  JSONCPP_INCLUDE_DIR
#  JSONCPP_LIBRARY
#

find_path(JSONCPP_INCLUDE_DIR
  NAMES json/json.h
  PATHS ${LibSourcey_DIR}/vendor/jsoncpp
  NO_DEFAULT_PATH)
  
find_library(JSONCPP_LIBRARY
  NAMES jsoncpp jsoncppd libjsoncpp
  PATHS ${LibSourcey_BUILD_DIR}/vendor/jsoncpp
  PATH_SUFFIXES Debug Release
  NO_DEFAULT_PATH)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(JSONCPP DEFAULT_MSG JSONCPP_LIBRARY JSONCPP_INCLUDE_DIR)
