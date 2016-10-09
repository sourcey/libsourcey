ask_build_sourcey_application(webrtcrecorder)
if(HAVE_WEBRTC AND HAVE_OPENCV AND BUILD_APPLICATIONS AND BUILD_APPLICATION_webrtcrecorder)
  include_dependency(FFmpeg REQUIRED)
  include_dependency(WebRTC REQUIRED)
  #include_dependency(OpenCV REQUIRED)

  define_sourcey_application(webrtcrecorder uv base crypto av net http util json socketio symple webrtc)
endif()
