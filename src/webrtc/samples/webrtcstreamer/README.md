# WebRTC Native Video Recorder

This sample app showcases how to stream realtime video to the the browser using LibSourcey and WebRTC. Audio and video streams can be from realtime device captures or movie files encoded in any format that's supported by LibSourcey/FFmpeg.

## Installation

1. Compile LibSourcey with WebRTC support (`WITH_WEBRTC=ON`) and samples (`BUILD_SAMPLES_webrtc=ON`) enabled. See the main README for help: https://github.com/sourcey/libsourcey#installing-on-linux
2. Run the `webrtcstreamer` sample application from the `build` directory.
2. Install and run the Node.js client application:

~~~ bash
cd <libsourcey>/src/webrtc/samples/webrtcstreamer/client
npm install
node app
~~~

3. Finally, point your browser to: `http://localhost:4499`
4. Your should see your video and audio stream playing in the browser.

## Hacking

The source code is clear and concise, let it be your guide :)
