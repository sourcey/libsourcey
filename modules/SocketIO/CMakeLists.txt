include("${CMAKE_SOURCE_DIR}/FindPoco.cmake")
include("${CMAKE_SOURCE_DIR}/FindOpenSSL.cmake")
include("${CMAKE_SOURCE_DIR}/LibSourceyFindJsonCpp.cmake")

define_libsourcey_module(SocketIO Base Net JSON)
