# include_dependency(SSL REQUIRED)
# include_dependency(FFmpeg REQUIRED)
# include_dependency(OpenCV REQUIRED)
# include_dependency(RtAudio REQUIRED)
# include_dependency(JsonCpp)
# include_dependency(LibUV REQUIRED)
  
define_sourcey_module_sample(turnmediaprovider base net turn http uv media)
