if(HAVE_FFMPEGV)
  # set(avtests_EXCLUDE_CTEST TRUE)
  define_libsourcey_test(avtests uv base av)
endif()
