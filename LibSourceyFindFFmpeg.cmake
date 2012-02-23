############################### FFmpeg ################################
           
set(FFMPEG_INCLUDE_DIR "${LIBSOURCEY_SOURCE_DIR}/3rdparty/ffmpeg" CACHE PATH "FFmpeg Include Location")

if(WIN32 AND MSVC)     
  set(FFMPEG_LIBRARY_DIR "${FFMPEG_INCLUDE_DIR}/bin")
  
  # Our third party FFmpeg folder includes inttypes.h for MSVC    
  set(FFMPEG_INCLUDE_DIR ${FFMPEG_INCLUDE_DIR} "${FFMPEG_INCLUDE_DIR}/msvc")
else()    
  set(FFMPEG_LIBRARY_DIR "${FFMPEG_INCLUDE_DIR}/lib")
  set(FFMPEG_LIBRARY_PREFIX "lib") 
endif()                 
                        
set(FFMPEG_LIBRARY_DIR "${FFMPEG_LIBRARY_DIR}" CACHE PATH "FFmpeg Library Location")  
                        
set(FFMPEG_LIBS "${FFMPEG_LIBRARY_PREFIX}avcodec" 
                "${FFMPEG_LIBRARY_PREFIX}avformat" 
                "${FFMPEG_LIBRARY_PREFIX}avutil" 
                "${FFMPEG_LIBRARY_PREFIX}swresample" 
                "${FFMPEG_LIBRARY_PREFIX}swscale")

set(FFMPEG_FOUND 1)
        
include_directories(${FFMPEG_INCLUDE_DIR})  
link_directories(${FFMPEG_LIBRARY_DIR})
                      
set(LIBSOURCEY_INCLUDE_DIRS ${LIBSOURCEY_INCLUDE_DIRS} ${FFMPEG_INCLUDE_DIR})
set(LIBSOURCEY_LIBRARY_DIRS ${LIBSOURCEY_LIBRARY_DIRS} ${FFMPEG_LIBRARY_DIR})
set(LIBSOURCEY_DEBUG_LIBS   ${LIBSOURCEY_DEBUG_LIBS}   ${FFMPEG_LIBS})   
set(LIBSOURCEY_RELEASE_LIBS ${LIBSOURCEY_DEBUG_LIBS}   ${FFMPEG_LIBS})
