# LibSourcey

> C++ Networking Evolved

[![Circle CI](https://circleci.com/gh/sourcey/libsourcey.svg?style=shield&circle-token=ab142562b19bb857de796d729aab28fa9df7682d)](https://circleci.com/gh/sourcey/libsourcey)
[![Doxygen](http://sourcey.com/images/doxygen.svg)](http://sourcey.com/libsourcey)

<!-- <img style="float:left;width:70px;margin-right:15px;margin-bottom:10px" src="http://sourcey.com/images/logos/libsourcey-80x80.png"> -->
LibSourcey is a collection of cross platform C++11 modules and classes that provide developers with an arsenal for rapidly developing high performance network based p2p and media streaming applications. Think of it as the power and performance of `libuv` combined with the features of `FFmpeg`, `OpenCV` and `WebRTC`, all integrated with the ease and readability of the `stl` (C++ Standard Library).

* **Documentation**: [http://sourcey.com/libsourcey](http://sourcey.com/libsourcey)  
* **Repository**: [https://github.com/sourcey/libsourcey](https://github.com/sourcey/libsourcey)  
* **Licence**: [LGPL-2.1+](/LICENSE.md)

## Features

* **Event-based IO** — Core modules are built on top of `libuv` (the underlying C library that powers `nodejs`) and use event-based asynchronous IO throughout to maximize performance and minimize concurrency reliance for building mission critical native and server side apps.

* **Cross platform** — The codebase is cross platform and should compile on any system with access to a modern C++11 compiler.

* **Modular libraries** — Libraries are modular for easy integration into your existing projects, so you can just "include what you need" without incurring extra incumbent bloat.

* **Well tested** — Core modules are well tested with unit tests and stress tested daily in production.

* **Clean and readable code** — Modern C++ design principles have been adhered to throughout for clean and readable code.

* **Networking layer** — A solid network layer is absolutely essential. The networking layer contains TCP, SSL and UDP socket implementations that combine `libuv` under the hood for blazing fast networking, and `openssl` for security and encryption.

* **Media streaming and encoding** — The `av` library consists of thin wrappers around `FFmpeg` and `OpenCV` for media capture, encoding, recording, streaming, analysis and more.

* **Web servers and clients** — A HTTP stack is provided that includes servers, clients, WebSockets, media streaming, file transfers, and authentication. The HTTP parser is based on the super-fast C code used by `nginx`.

* **Realtime messaging** — LibSourcey aims to bridge the gap between desktop, mobile and web by providing performance oriented messaging solutions that work across all platforms.
    * **Socket.IO** — Socket.IO C++ client that supports the latest protocol revision 4 (>= 1.0). Read more about [Socket.IO](http://socket.io).
    * **Symple** — Sourcey's home grown realtime messaging protocol that works over the top of Socket.IO to provide rostering, presence and many other features necessary for building online games and chat applications. [More about Symple](<http://sourcey.com/symple).

* **WebRTC support** — WebRTC native support allows you to build p2p desktop and server side applications that inherit LibSourcey's realtime messaging and media capabilities. Take a look at the [examples](examples.md) for how to stream live webcam and microphone streams to the browser, and also how to record live WebRTC streams on the server side.

<!--
* **Full ICE stack** — The ICE module is a complete implementation of [RFC 5245 (Interactive Connectivity Establishment)](http://tools.ietf.org/html/rfc5245) based on LibSourcey architecture.
    * **ICE** — [RFC 5245](http://tools.ietf.org/rfc/rfc5245) ICE agent implementation.
    * **STUN** — [RFC 5389](http://tools.ietf.org/rfc/rfc5389) implementation that includes support for ICE and TURN and TURN TCP messages.
    * **TURN** — Server and client stack that supports both [RFC 5766 (Traversal Using Relays around NAT)](http://tools.ietf.org/rfc/rfc5766) and [RFC 6062 (Traversal Using Relays around NAT Extensions for TCP Allocations)](http://tools.ietf.org/rfc/rfc6062) specifications.s
    * **SDP** — [RFC 4566](http://tools.ietf.org/rfc/rfc4566) implementation that includes extra support for ICE headers.
-->

## Examples

What better way to get acquainted with a new library then with some tasty code examples.

#### HTTP echo server

Lets start with the classic HTTP echo server, which looks something like this:

~~~cpp
http::Server srv(net::Address("0.0.0.0", 1337));
srv.Connection += [](http::ServerConnection::Ptr conn) {
    conn->Payload += [](http::ServerConnection& conn, const MutableBuffer& buffer) {
        conn.send(bufferCast<const char*>(buffer), buffer.size());
        conn.close();
    };
};
srv.start();
~~~

Pretty neat right? Its bloody fast too, especially on Linux kernel 3.9 or newer where its optimized to use of kernel level multicore socket load balancing. Don't just take our work for it though, check out some benchmarks using `wrk`:

**LibSourcey `httpechoserver`**

~~~bash
$ wrk -d10s --timeout 2s http://localhost:1337
Running 10s test @ http://localhost:1337
  2 threads and 10 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   265.76us  472.62us  12.42ms   97.51%
    Req/Sec    18.84k     1.26k   21.57k    74.50%
  375060 requests in 10.01s, 20.39MB read
Requests/sec:  37461.50
Transfer/sec:      2.04MB
~~~

**Nodejs echo server**

~~~bash
$ wrk -d10s --timeout 2s http://localhost:1337
Running 10s test @ http://localhost:1337
  2 threads and 10 connections
  Thread Stats   Avg      Stdev     Max   +/- Stdev
    Latency   502.70us  715.11us  14.83ms   97.90%
    Req/Sec    11.69k     1.46k   14.52k    70.50%
  232667 requests in 10.01s, 21.97MB read
Requests/sec:  23236.33
Transfer/sec:      2.19MB
~~~

As you can see the `httpechoserver` is almost twice as fast as the `nodejs` echo server, which is a massive performance gain over one of the leading web server implementations. Check the `httpechoserver` sample for the full code, including the `nodejs` echo server we used for profiling.

#### Processes

Interacting with system processes doesn't have to be painful.
The following code will run the `ping sourcey.com` and with `stdio` and exit callbacks:

~~~cpp
Process proc({ "ping", "sourcey.com" });
proc.onstdout = [](std::string line)
{
    // handle process output
};
proc.onexit = [](std::int64_t status)
{
    // handle process exit
};
proc.spawn();

// write some random data to the stdin pipe
proc.in() << "random data"
~~~

#### PacketStream

A good starting point is the `PacketStream`, which lets you create a dynamic delegate chain for piping, processing and outputting arbitrary data packets. This method of layering packet processors and dynamic functionality makes it possible to develop complex data processing applications quickly and easily.

For example, this is how you would capture a live webcam stream, encode it into H.264, and broadcast it in realtime over the internet:

~~~cpp
// Create a PacketStream to pass packets from the
// input device captures -> encoder -> socket
PacketStream stream;

// Setup the encoder options
av::EncoderOptions options;
options.oformat = av::Format("MP4", "mp4",
    av::VideoCodec("H.264", "libx264", 400, 300, 25, 48000, 128000, "yuv420p"),
    av::AudioCodec("AAC", "libfdk_aac", 2, 44100, 64000, "s16"));

// Create a device manager instance to enumerate system devices
av::DeviceManager devman;
av::Device device;

// Create and attach the default video capture
av::VideoCapture::Ptr video;
if (devman.getDefaultCamera(device)) {
    video.open(device.id, 640, 480, 30);
    video.getEncoderFormat(options.iformat);
    stream.attachSource(video, true);
}

// Create and attach the default audio capture
av::AudioCapture::Ptr audio;
if (devman.getDefaultMicrophone(device)) {
    audio.open(device.id, 2, 44100);
    audio.getEncoderFormat(options.iformat);
    stream.attachSource(audio, true);
}

// Create and attach the multiplex encoder
av::MultiplexPacketEncoder::Ptr encoder(options);
stream.attach(encoder);

// Attach the output net::Socket instance (instantiated elsewhere)
// to broadcast encoded packets over the network
stream.attach(socket);

// Start the stream
// This method call will start the device captures and begin
// pumping encoded media into the output socket
stream.start();
~~~

There are plenty more demos and sample code to play with over on the [examples](https://github.com/sourcey/libsourcey/blob/master/doc/examples.md) page.

## Installation

See the platform independent [installation guides](https://github.com/sourcey/libsourcey/blob/master/doc/installation.md).

## Contributors

A BIG thanks to everyone who has contributed to the development of LibSourcey so far:

* Kam Low ([@auscaster](https://github.com/auscaster)) — Primary developer
* Yury Shulbn ([@yuryshubin](https://github.com/yuryshubin)) — iOS build toolchain and platform fixes
* Vinci Xu ([@VinciShark](https://github.com/VinciShark)) — Windows documentation, testing and updates
* Michael Fig ([@michael-fig](https://github.com/michael-fig)) — Fixed compiler flags to build without FFmpeg
* Hyunuk Kim ([@surinkim](https://github.com/surinkim)) — Fixed `std::codecvt` unicode character conversion on Windows
* Damian Zelim ([@ZelimDamian](https://github.com/ZelimDamian)) — Fixed compiler flags for OS X build
* Norm Ovenseri ([@normano](https://github.com/normano)) — Added verbose logging output to build system
* Alexey ([@deilos](https://github.com/deilos)) — Fixed cross-platform FFmpeg build script

## Contributing

Pull Requests are always welcome, so if you make any improvements please feel free to float them back upstream :)

1. [Fork LibSourcey on Github](https://github.com/sourcey/libsourcey)
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create new Pull Request

<!--
## Issues

For bugs and issues please use the [Github issue tracker](https://github.com/sourcey/libsourcey/issues).
-->
