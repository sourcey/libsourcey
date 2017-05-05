########################################################################
# CMake module for finding HTTPPARSER
#
# The following variables will be defined:
#
#  HTTPPARSER_FOUND
#  HTTPPARSER_INCLUDE_DIR
#  HTTPPARSER_LIBRARIES
#

find_path(HTTPPARSER_INCLUDE_DIR
  NAMES http_parser.h
  PATHS ${LibSourcey_DIR}/vendor/http_parser
  PATHS ${LibSourcey_DIR}/share/libsourcey/vendor/include
  NO_DEFAULT_PATH)

find_library(HTTPPARSER_LIBRARY_DEBUG
  NAMES http_parserd libhttp_parserd
  PATHS
    ${LibSourcey_BUILD_DIR}/vendor/http_parser
    ${LibSourcey_DIR}/share/libsourcey/vendor/lib
  PATH_SUFFIXES Debug
  NO_DEFAULT_PATH)

find_library(HTTPPARSER_LIBRARY_RELEASE
  NAMES http_parser libhttp_parser
  PATHS
    ${LibSourcey_BUILD_DIR}/vendor/http_parser
    ${LibSourcey_DIR}/share/libsourcey/vendor/lib
  PATH_SUFFIXES Release
  NO_DEFAULT_PATH)

include(${CMAKE_ROOT}/Modules/SelectLibraryConfigurations.cmake)
select_library_configurations(HTTPPARSER)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(HTTPPARSER DEFAULT_MSG HTTPPARSER_LIBRARIES HTTPPARSER_INCLUDE_DIR)
