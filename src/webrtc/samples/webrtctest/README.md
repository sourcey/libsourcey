# WebRTC Native Video Recorder

This sample app showcases how to record live WebRTC video streams on the server side using LibSourcey. Audio and video streams are recorded and multiplexed in MP4 format (H.264/ACC) by default, but any format supported by LibSourcey/FFmpeg can be used.

## Installation

1. Compile LibSourcey with WebRTC support (`WITH_WEBRTC=ON`) and samples (`BUILD_SAMPLES_webrtc=ON`) enabled. See the main README for help: https://github.com/sourcey/libsourcey#installing-on-linux
2. Run the `webrtcrecorder` sample application from the `build` directory.
2. Install and run the Node.js client application:

~~~ bash
cd <libsourcey>/src/webrtc/samples/webrtcrecorder/client
npm install
node app
~~~

3. Finally, point your browser to: `http://localhost:4499`
4. Your WebRTC stream will be recorded as `webrtcrecorder.mp4`.

## Hacking

The source code is clear and concise, let it be your guide :)
