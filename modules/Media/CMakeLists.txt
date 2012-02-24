include("${CMAKE_SOURCE_DIR}/FindPoco.cmake")
include("${CMAKE_SOURCE_DIR}/FindOpenSSL.cmake")
include("${CMAKE_SOURCE_DIR}/FindFFMpeg.cmake")   
include("${CMAKE_SOURCE_DIR}/FindOpenCV.cmake")   
include("${CMAKE_SOURCE_DIR}/LibSourceyFindRtAudio.cmake")   

define_libsourcey_module(Media Base Net)
