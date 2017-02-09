########################################################################
# CMake module for finding JsonCpp
#
# The following variables will be defined:
#
#  JSONCPP_FOUND
#  JSONCPP_INCLUDE_DIR
#  JSONCPP_LIBRARIES
#

find_path(JSONCPP_INCLUDE_DIR
  NAMES json/json.h
  PATHS ${LibSourcey_DIR}/vendor/jsoncpp
  NO_DEFAULT_PATH)

find_library(JSONCPP_LIBRARY_DEBUG
  NAMES jsoncppd libjsoncppd
  PATHS ${LibSourcey_BUILD_DIR}/vendor/jsoncpp
  PATH_SUFFIXES Debug
  NO_DEFAULT_PATH)

find_library(JSONCPP_LIBRARY_RELEASE
  NAMES jsoncpp libjsoncpp
  PATHS ${LibSourcey_BUILD_DIR}/vendor/jsoncpp
  PATH_SUFFIXES Release
  NO_DEFAULT_PATH)

include(${CMAKE_ROOT}/Modules/SelectLibraryConfigurations.cmake)
select_library_configurations(JSONCPP)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(JSONCPP DEFAULT_MSG JSONCPP_LIBRARIES JSONCPP_INCLUDE_DIR)
