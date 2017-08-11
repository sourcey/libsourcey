ask_build_sourcey_module(av)
if(BUILD_MODULES OR (NOT BUILD_MODULES AND BUILD_MODULE_av))

  # RtAudio is now deprecated
  # if(WITH_RTAUDIO)
  #   set(BUILD_TESTING OFF) # don't build rtaudio tests
  #   add_vendor_dependency(RTAUDIO rtaudio)
  # endif()

  include_dependency(FFmpeg)
  # include_dependency(OpenCV)

  set(av_PRETTY_NAME AV)
  define_sourcey_module(av base)
endif()
