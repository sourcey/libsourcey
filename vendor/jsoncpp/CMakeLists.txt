# ----------------------------------------------------------------------------
# CMake file for JsonCpp
# ----------------------------------------------------------------------------

# include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

set(jsoncpp_SOURCE_FILES jsoncpp.cpp)

install(DIRECTORY json DESTINATION ${LibSourcey_VENDOR_INSTALL_DIR}/include)

define_sourcey_dependency(jsoncpp)
