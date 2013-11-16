ask_build_sourcey_module(media)
if(BUILD_MODULES AND BUILD_MODULE_media) 

  #include_dependency(OpenSSL REQUIRED)
  #include_dependency(FFmpeg REQUIRED)  
  #include_dependency(JsonCpp REQUIRED)   
  #include_dependency(OpenCV REQUIRED)   
  #include_dependency(RtAudio REQUIRED)
  #include_dependency(LibUV REQUIRED)

  define_sourcey_module(media base net uv)
endif()
