ask_build_sourcey_module(media)
if(BUILD_MODULES AND BUILD_MODULE_media)

  #if(WITH_RTAUDIO)
    set(BUILD_TESTING OFF) # don't build rtaudio tests
    add_vendor_dependency(RTAUDIO rtaudio)

    #list(APPEND LibSourcey_BUILD_DEPENDENCIES rtaudio)
    #add_subdirectory(vendor/rtaudio)
    #include_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendor/rtaudio)
    #set(HAVE_RTAUDIO 1)
    #set(HAVE_RTAUDIO 1 PARENT_SCOPE)
  #endif()

  if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    find_library(AVFOUNDATION AVFoundation)
    # find_library(FOUNDATION Foundation)
    # find_library(QTKIT QTKit)

    list(APPEND LibSourcey_BUILD_DEPENDENCIES ${AVFOUNDATION}) # ${AVFOUNDATION} ${QTKIT}
  endif()

  include_dependency(FFmpeg)
  include_dependency(OpenCV)
  #message(FATAL_ERROR "${CMAKE_SYSTEM_NAME}")
  #include_dependency(RtAudio)
  #include_dependency(LibUV REQUIRED)

  define_sourcey_module(media base net uv)
endif()
