ask_build_sourcey_module(media)
if(BUILD_MODULES AND BUILD_MODULE_media) 
  
  if(WITH_RTAUDIO)
    list(APPEND LibSourcey_BUILD_DEPENDENCIES rtaudio)
    set(BUILD_TESTING OFF) # don't build rtaudio tests
    add_subdirectory(vendor/rtaudio)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendor/rtaudio)
    set(HAVE_RTAUDIO 1)
  endif()
  
  if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    find_library(AVFOUNDATION AVFoundation)
    # find_library(FOUNDATION Foundation)
    # find_library(QTKIT QTKit)
	
    # list(APPEND LibSourcey_BUILD_DEPENDENCIES ${FOUNDATION} ${AVFOUNDATION} ${QTKIT})
    list(APPEND LibSourcey_BUILD_DEPENDENCIES ${AVFOUNDATION})
  endif()

  include_dependency(FFmpeg)
  include_dependency(OpenCV) 
  #include_dependency(RtAudio REQUIRED)
  #include_dependency(LibUV REQUIRED)

  define_sourcey_module(media base net uv)
endif()
