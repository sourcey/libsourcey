if(HAVE_FFMPEG AND HAVE_OPENCV)
  # include_dependency(LibUV)
  # include_dependency(FFmpeg REQUIRED)

  define_libsourcey_test(mediatests base uv media)
endif()
