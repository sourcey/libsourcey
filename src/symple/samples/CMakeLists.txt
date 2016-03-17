if(WITH_FFMPEG AND HAVE_FFMPEG AND WITH_OPENCV AND HAVE_OPENCV)
  add_subdirectory(mediaserver)
  #add_subdirectory(webrtcstreamer)
endif()
