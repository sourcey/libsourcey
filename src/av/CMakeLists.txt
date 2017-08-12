ask_build_sourcey_module(av)
if(BUILD_MODULES OR (NOT BUILD_MODULES AND BUILD_MODULE_av))

  # RtAudio is now deprecated
  # if(WITH_RTAUDIO)
  #   set(BUILD_TESTING OFF) # don't build rtaudio tests
  #   add_vendor_dependency(RTAUDIO rtaudio)
  # endif()

  # if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  #   find_library(AVFOUNDATION AVFoundation)
  #   list(APPEND LibSourcey_BUILD_DEPENDENCIES ${AVFOUNDATION})
  # endif()

  include_dependency(FFmpeg)
  # include_dependency(OpenCV)

  set(av_PRETTY_NAME AV)
  define_sourcey_module(av base)
endif()
