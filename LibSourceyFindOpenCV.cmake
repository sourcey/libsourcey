############################### OpenCV ################################

set(OPENCV_BUILD_PATH "${LIBSOURCEY_SOURCE_DIR}/3rdparty/opencv" CACHE PATH "OpenCV Install Location")
set(OPENCV_INCLUDE_PATH "${OPENCV_BUILD_PATH}/include")

if(${CMAKE_SYSTEM_PROCESSOR} MATCHES amd64*|x86_64* OR MSVC64)
  set(OPENCV_LIBRARY_DIR "${OPENCV_BUILD_PATH}/x64")
elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES x86*|i386*|i686*)
  set(OPENCV_LIBRARY_DIR "${OPENCV_BUILD_PATH}/x86")
endif()

# TODO: More compiler types for OpenCV
if(MINGW)
  set(OPENCV_LIBRARY_DIR "${OPENCV_LIBRARY_DIR}/mingw")
elseif(MSVC80)
  set(OPENCV_LIBRARY_DIR "${OPENCV_LIBRARY_DIR}/vc8")
elseif(MSVC90)
  set(OPENCV_LIBRARY_DIR "${OPENCV_LIBRARY_DIR}/vc9")
elseif(MSVC10)
  set(OPENCV_LIBRARY_DIR "${OPENCV_LIBRARY_DIR}/vc10")
endif()
                    
#if(NOT BUILD_SHARED_LIBS AND IS_DIRECTORY "${OPENCV_LIBRARY_DIR}/staticlib") 
#  set(OPENCV_LIBRARY_DIR "${OPENCV_LIBRARY_DIR}/staticlib")
#else()
  set(OPENCV_LIBRARY_DIR "${OPENCV_LIBRARY_DIR}/lib")
#endif()

set(OPENCV_LIBRARY_DIR ${OPENCV_LIBRARY_DIR} CACHE PATH "OpenCV Library Location")

set(OPENCV_DEBUG_LIBS "opencv_core230d" 
                      "opencv_imgproc230d" 
                      "opencv_highgui230d" 
                      "opencv_video230d")

set(OPENCV_RELEASE_LIBS "opencv_core230" 
                        "opencv_imgproc230" 
                        "opencv_highgui230" 
                        "opencv_video230")
      
include_directories(${OPENCV_INCLUDE_PATH})  
link_directories(${OPENCV_LIBRARY_DIR})
                      
set(LIBSOURCEY_INCLUDE_DIRS ${LIBSOURCEY_INCLUDE_DIRS} ${OPENCV_INCLUDE_PATH})
set(LIBSOURCEY_LIBRARY_DIRS ${LIBSOURCEY_LIBRARY_DIRS} ${OPENCV_LIBRARY_DIR})
set(LIBSOURCEY_DEBUG_LIBS   ${LIBSOURCEY_DEBUG_LIBS}   ${OPENCV_DEBUG_LIBS})    
set(LIBSOURCEY_RELEASE_LIBS ${LIBSOURCEY_RELEASE_LIBS} ${OPENCV_RELEASE_LIBS})    