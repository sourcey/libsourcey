# WebRTC Native Video Capturer

This sample demonstrates how to stream audio and video capture devices from a native app to the browser.

## Installation

1. Compile LibSourcey with WebRTC support (`WITH_WEBRTC=ON`) and samples (`BUILD_SAMPLES_webrtc=ON`) enabled. See the main README for help: https://github.com/sourcey/libsourcey#installing-on-linux
2. Run the `webrtccapturer` sample application from the `build` directory.
2. Install and run the Node.js client application:

~~~ bash
cd <libsourcey>/src/webrtc/samples/webrtccapturer/client
npm install
node app
~~~

3. Finally, point your browser to: `http://localhost:4499`

## Hacking

The source code is clear and concise, let it be your guide :)
