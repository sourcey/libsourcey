if(WITH_FFMPEG AND HAVE_FFMPEG AND WITH_OPENCV AND HAVE_OPENCV)
  #include_dependency(LibUV)
  # include_dependency(FFmpeg REQUIRED)

  define_libsourcey_test(mediatests uv base media)
endif()
