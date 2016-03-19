########################################################################
# CMake module for finding HTTPPARSER
#
# The following variables will be defined:
#
#  HTTPPARSER_FOUND
#  HTTPPARSER_INCLUDE_DIR
#  HTTPPARSER_LIBRARY
#

find_path(HTTPPARSER_INCLUDE_DIR
	NAMES http_parser.h
	PATHS ${LibSourcey_SOURCE_DIR}/http/vendor/http_parser
  NO_DEFAULT_PATH)

find_library(HTTPPARSER_LIBRARY
	NAMES http_parser libhttp_parser
  PATHS ${LibSourcey_BUILD_DIR}/src/http/vendor/http_parser
  NO_DEFAULT_PATH)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(HTTPPARSER DEFAULT_MSG HTTPPARSER_LIBRARY HTTPPARSER_INCLUDE_DIR)
