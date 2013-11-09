ask_build_sourcey_module(Media)
if(BUILD_MODULES AND BUILD_MODULE_Media) 

  include_dependency(OpenSSL REQUIRED)
  include_dependency(FFmpeg REQUIRED)  
  include_dependency(JsonCpp REQUIRED)   
  include_dependency(OpenCV REQUIRED)   
  include_dependency(RtAudio REQUIRED)
  include_dependency(LibUV REQUIRED)

  define_sourcey_module(Media Base Net UV)
endif()
