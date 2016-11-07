## Examples

There are plenty of examples available in the `samples` and `tests` folder of each module for you to cut your teeth on.

* [WebRTC Recorder](https://github.com/sourcey/libsourcey/tree/master/src/webrtc/samples/webrtcrecorder)
  The WebRTC native multiplex recorder lets you record live WebRTC streams from the browser on the server side.

* [WebRTC Streamer](https://github.com/sourcey/libsourcey/tree/master/src/webrtc/samples/webrtcstreamer)
  The WebRTC Streamer lets you stream any video source ie. webcam and microphone or video file from the server to the browser.

* [Media Server](https://github.com/sourcey/libsourcey/tree/master/src/av/samples/mediaserver)
  Check out the `mediaserver` sample in the `av` module for how to stream media over HTTP, WebSocket and TURN protocols. You will need to compile LibSourcey with FFmpeg enabled to use it.

* [Device Recorder](https://github.com/sourcey/libsourcey/tree/master/src/av/samples/devicerecorder)
  The `devicerecorder` sample lets you multiplex record live webcam and microphone capture streams in any video format supported by FFmpeg. Remember, you will need to compile LibSourcey with FFmpeg enabled to use it.

* [Device Enumerator](https://github.com/sourcey/libsourcey/tree/master/src/av/samples/deviceenumerator)
  The `deviceenumerator` sample outputs all the available input and output devices on the system.

* [HTTP Echo Server](https://github.com/sourcey/libsourcey/tree/master/src/http/samples/httpechoserver)
  Where would we be without the good ol' HTTP echo server? Works for HTTP, HTTPS and WebSocket connections.

* [Socket Echo Server](https://github.com/sourcey/libsourcey/tree/master/src/http/samples/httpechoserver)
  The socket echo server echoes raw data packets over TCP and UPD socket connections.

* [Package Manager](https://github.com/sourcey/pacm/tree/master/apps/pacmconsole)
  Take a look at `pacm` for a redistributable C++ package manager built on LibSourcey, and specifically the `pacmconsole` application. http://sourcey.com/pacm/

* [TURN Server](https://github.com/sourcey/libsourcey/tree/master/src/turn/samples/turnserver)
  The TURN server provides a functional RFC 5766 server implementation.
