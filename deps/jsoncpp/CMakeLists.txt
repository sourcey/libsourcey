# ----------------------------------------------------------------------------
# CMake file for JsonCpp
# ----------------------------------------------------------------------------


include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
#set(LibSourcey_INCLUDE_DIRS ${LibSourcey_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE) #CACHE INTERNAL "") 

set(jsoncpp_SOURCE_FILES jsoncpp.cpp)
set(jsoncpp_HEADER_FILES json/json.h)

define_sourcey_dependency(jsoncpp)