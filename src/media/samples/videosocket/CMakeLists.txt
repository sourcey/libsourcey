# include_dependency(LibUV)
# include_dependency(HttpParser)
# include_dependency(RtAudio)
# include_dependency(JsonCpp)
# include_dependency(OpenSSL REQUIRED)
# include_dependency(FFmpeg REQUIRED)
# include_dependency(OpenCV REQUIRED)

define_sourcey_module_sample(videosocket base uv crypto net http media util)
