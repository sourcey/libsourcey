add_subdirectory(deviceenumerator)

if(HAVE_FFMPEG AND HAVE_OPENCV)
  add_subdirectory(videosocket)
  if(BUILD_ALPHA)
    add_subdirectory(waveformoutput)
  endif()
endif()
