ask_build_sourcey_application(webrtcstreamer)
if(HAVE_WEBRTC AND WITH_OPENCV AND BUILD_APPLICATIONS AND BUILD_APPLICATION_webrtcstreamer)
  include_dependency(WebRTC REQUIRED)
  include_dependency(OpenCV REQUIRED)


  define_sourcey_application(webrtcstreamer uv base crypto media net http util json socketio symple webrtc)
endif()
