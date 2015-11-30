if(HAVE_FFMPEG AND HAVE_OPENCV)
 include_dependency(FFmpeg REQUIRED)
 include_dependency(LibUV REQUIRED)
    
  define_libsourcey_test(mediatests base uv media) 
endif()
