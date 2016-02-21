# ----------------------------------------------------------------------------
# CMake file for JsonCpp
# ----------------------------------------------------------------------------

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

set(jsoncpp_SOURCE_FILES jsoncpp.cpp)
set(jsoncpp_HEADER_FILES json/json.h)

define_sourcey_dependency(jsoncpp)