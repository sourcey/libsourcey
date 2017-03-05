#include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

set(http_parser_SOURCE_FILES http_parser.c)
set(http_parser_HEADER_FILES http_parser.h)

define_sourcey_dependency(http_parser)
