# LibSourcey

**Homepage**: [http://sourcey.com/libsourcey](http://sourcey.com/libsourcey)  
**Licence**: LGPL

[![Circle CI](https://circleci.com/gh/sourcey/libsourcey.svg?style=shield&circle-token=ab142562b19bb857de796d729aab28fa9df7682d)](https://circleci.com/gh/sourcey/libsourcey)

LibSourcey is a collection of open source cross platform C++11 modules and classes that provide developers with a flexible high performance arsenal for rapidly developing realtime messaging and media streaming applications. Think of it as the power of `libuv` combined with the ease and readability of the `stl` (C++ Standard Library).

<!-- For media streaming applications, LibSourcey provides a simple and flexible method of capturing live audio/video streams (_OpenCV/FFmpeg_), processing, filtering and encoding them using any video format (_FFmpeg_), and broadcasting the result over the Internet (_libuv_). This is made possible by implementing a thin layer over the top of some brilliant open source projects, such as FFmpeg, OpenCV and libuv. The only required third-party dependency is libuv, and that is included in the local source and compiled automatically. All others dependencies are optional. -->

<!-- Unfortunately documentation still a little sparse at this point, but we hope to change that in the near future. Until then, _use the source, Luke!_, and we welcome all community contributions to LibSourcey in order to promote the development of better real-time native and web applications. -->

## Features

* **Event-based IO** — Core modules are built on top of `libuv` (the underlying C library that powers Node.js) and use event-based asynchronous IO throughout to maximise performance and minimise concurrency reliance for building mission critical native and server side apps.

* **Cross platform** — The codebase is cross platform and should compile on any system with access to a modern C++11 compiler. We have successfully compiled and used LibSourcey on the following platforms: Linux(gcc), Mac(Xcode, gcc), Windows(msys, vs), Android and iOS.

* **Clean readable code** — Modern C++11 design principles have been used for clear and readable code. Libraries are modular for simple integration into your existing projects.

* **Networking layer** — The networking layer provides TCP, SSL and UDP socket implementations that combine the use of `libuv` for blazing fast networking, and `openssl` for security and encryption.

* **Media streaming and encoding** — The `av` library consists of thin wrappers around FFmpeg and OpenCV for media capture, encoding, recording, streaming, analysis and more. The media API makes full use of the PacketStream API so that encoders, processors and broadcasters can be dynamically added and removed from any media source - a great base for building a [media server](https://github.com/sourcey/libsourcey/tree/master/src/av/samples/mediaserver).  

* **Elegant PacketStream API** — The PacketStream API allows LibSourcey modules to pipe and process arbitrary data packets through a dynamic delegate chain. This dynamic method of layering packet processors makes it possible to develop complex data processing applications quickly and easily. Check out this writeup on how the PacketSteram API can be used to easily encode H.264 on the fly: http://sourcey.com/libsourcey-packetstream-api/

* **Realtime messaging** — LibSourcey aims to bridge the gap between desktop, mobile and web by providing performance oriented messaging solutions that work across all platforms.
    * **Socket.IO** — Socket.IO C++ client that supports the latest protocol revision 4 (>- 1.0). Read more about [Socket.IO](http://socket.io).
    * **Symple** — Sourcey's home grown realtime messaging protocol that works over the top of Socket.IO to provide rostering, presence and many other features necessary for building online games and chat applications. [More about Symple](<http://sourcey.com/symple).

* **WebRTC support** — WebRTC support allows you to build WebRTC native desktop and server side applications that inherit LibSourcey's realtime messaging and media capabilities. Two demo applications are included; one for streaming live webcam and microphone streams to the browser [here](https://github.com/sourcey/libsourcey/tree/master/src/webrtc/samples/webrtcstreamer), and one for multiplex recording live WebRTC streams from the browser on the server side [here](https://github.com/sourcey/libsourcey/tree/master/src/webrtc/samples/webrtcrecorder).

* **Web servers and clients** — A HTTP stack is included that supports HTTP servers, clients, WebSockets, media streaming, file transfers, and authentication. The HTTP parser is based on the super-fast C code used by `nginx`.

* **Full ICE stack** — The ICE module is a complete implementation of [RFC 5245 (Interactive Connectivity Establishment)](http://tools.ietf.org/html/rfc5245) based on LibSourcey architecture.
    * **STUN** — [RFC 5389](http://tools.ietf.org/rfc/rfc5389) implementation that includes support for ICE and TURN and TURN TCP messages.
    * **TURN** — Server and client stack that supports both [RFC 5766 (Traversal Using Relays around NAT)](http://tools.ietf.org/rfc/rfc5766) and [RFC 6062 (Traversal Using Relays around NAT Extensions for TCP Allocations)](http://tools.ietf.org/rfc/rfc6062) specifications.
    * **SDP** — [RFC 4566](http://tools.ietf.org/rfc/rfc4566) implementation that includes extra support for ICE headers.

## Dependencies

* **LibUV** (required, >= 1.8.0)
  Networking, filesystem and cross platform utilities
* **OpenSSL** (required, >= 1.0.1g)
  SSL networking layer and encryption
* **FFmpeg** (optional, >= 2.8.3)
  Media encoding and streaming
* **WebRTC** (optional)
  Peer-to-peer video conferencing
* **OpenCV** (optional, >= 3.0)
  Video capture and computer vision algorithms
  
<!--
* **RtAudio** (optional, >= 4.1.2)
Audio capture
-->

## Installation

LibSourcey uses `CMake` to build the project files for the compiler on your platform, and should build on any system with a modern C++11 compiler.

_Note:_ To compile LibSourcey with video and streaming capabilities enabled you should install the latest version of both `FFmpeg` and `OpenCV`, otherwise modules and features dependent on those libraries will be disabled by default.

### Installing on Linux

This guide is written for Ubuntu 14.04, although installation should be fairly portable across most flavours of Linux.

##### Install Dependencies

~~~ bash
sudo apt-get update
sudo apt-get install -y build-essential pkg-config git cmake openssl libssl-dev
~~~

<!--
jackd2 libjack-jackd2-dev
-->

<!--
**Install LibUV:**

~~~ bash
cd ~/tmp
git clone https://github.com/libuv/libuv.git
cd libuv
sh autogen.sh
./configure
make
# make check
sudo make install
~~~
-->

**Install FFmpeg (optional):**

FFmpeg is an optional but recommended dependency that's required to make use of LibSourcey's media encoding capabilities.

Most versions on Linux have FFmpeg development libraries that you can install directly from the package manager. If you're on a newer version of Ubuntu (>= 15.04) then you can install the required libraries as follows:

~~~ bash
sudo apt-get install --yes libavcodec-ffmpeg-dev libavdevice-ffmpeg-dev libavfilter-ffmpeg-dev libavformat-ffmpeg-dev libswresample-ffmpeg-dev libpostproc-ffmpeg-dev
~~~

Otherwise if you want to compile FFmpeg yourself you can follow the [official guide for installing FFmpeg](http://trac.ffmpeg.org/wiki/CompilationGuide) that works out of the box with LibSourcey.

<!--
**Install OpenCV (optional):**

OpenCV is an optional dependecy that's used by LibSourcey for it's video capture, video analysis and computer vision algorithms. Note that if you're compiling FFmpeg yourself (as above), then you should compile OpenCV with `WITH_FFMPEG=OFF` otherwise conflicting FFmpeg libraries may be installed on your system.

~~~ bash
cd ~/tmp
wget https://github.com/Itseez/opencv/archive/3.0.0.zip -O opencv-3.0.0.zip
unzip opencv-3.0.0.zip
cd opencv-3.0.0
mkdir build
cd build
cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D WITH_TBB=ON -D WITH_V4L=ON -D WITH_QT=ON -D WITH_OPENGL=ON -D WITH_FFMPEG=OFF ..
make -j $(nproc)
sudo make install
sudo /bin/bash -c 'echo "/usr/local/lib" > /etc/ld.so.conf.d/opencv.conf'
sudo ldconfig
~~~
-->

**Install WebRTC (optional):**

Installing WebRTC can be tricky and time consuming, but using the script below you should be able to get started without a hitch. Using the script is highly recommended as there are a few caveats to take note of when compiling WebRTC for use with LibSourcey:

1. RTTI must be enabled (the -no_rtti compiler flag must be removed)
2. The WebRTC static library must not contain JsonCpp (it conflicts with our newer internal version)
3. libstdc++ iterator debugging must be disabled or it conflicts with external dependencies compiled without it (ninja build must set `enable_iterator_debugging=false`)

~~~ bash
cd ~/tmp
git clone git@github.com:sourcey/webrtcbuilds.git
cd webrtcbuilds
./build.sh -l jsoncpp -e
~~~

##### Install LibSourcey

~~~ bash
cd ~/tmp
git clone https://github.com/sourcey/libsourcey.git
cd libsourcey
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=RELEASE # extra cmake commands here, see below for a full list...
make
sudo make install
~~~

All done!

[See here](#cmake-build-options) for a complete list of build options, and [here for examples](#examples).

### Installing on Apple (OS X)

This guide is for written for Mac users using [Homebrew](http://brew.sh).

##### Install Dependencies

<!-- # LibUV:
brew install --HEAD libuv -->

~~~ bash
# OpenSSL:
brew install openssl
brew link --force openssl

# CMake:
brew install cmake
~~~

Please see the [Linux instructions](#installing-on-linux) for other optional dependencies. <!-- such as FFmpeg and OpenCV -->

##### Install LibSourcey

~~~ bash
cd ~/tmp
git clone https://github.com/sourcey/libsourcey.git
cd libsourcey
mkdir build
cd build
cmake .. -D CMAKE_BUILD_TYPE=RELEASE # extra cmake commands here...
make
sudo make install
~~~

<!-- Install Git
 : Download the [latest Git installer package](http://code.google.com/p/git-osx-installer/downloads/list?can=3), double click on the installer to start the installation wizard. You’ll be prompted for your system password in order for the installer to complete.

Install CMake
 : CMake generates the LibSourcey project files so you can build on most platforms and compilers. [Download CMake](http://www.cmake.org/cmake/resources/software.html)

Install OpenSSL
 : If you don't already have OpenSSL development headers on your Mac, then please follow [this guide](http://www.opensource.apple.com/source/OpenSSL/OpenSSL-7.1/openssl/INSTALL?txt) to install them.

Download LibSourcey
 : Clone the repository: `git clone https://github.com/sourcey/libsourcey.git`.
   If you haven't got Git for some reason you can download and extract the [package archive](https://github.com/sourcey/libsourcey) from Github. -->

##### Generate Project Files

See linux instructions.

<!-- Open the CMake GUI and set the project directory to point to the LibSourcey root directory. Execute "Configure" to do the initial configuration, then adjust any options, then press "Configure" again and then press "Generate". -->

[See here](#cmake-build-options) for a complete list of build options, and [here for examples](#examples).

##### Compile with Xcode

<!-- * Generate Xcode project using CMake, as described above.
* Launch Xcode, locate and open libsourcey.xcodeproj. Select "Debug", build the BUILD_ALL target (Cmd-B), select "Release" and build it too. -->

### Installing on Windows

##### Install Dependencies

**Install Git**  

Install [TortoiseGit](http://code.google.com/p/tortoisegit/), a convenient git front-end that integrates right into Windows Explorer. MinGW users can use [msysgit](http://code.google.com/p/msysgit/downloads/list).  

**Install CMake**  

CMake generates the LibSourcey project files so you can build on most platforms and compilers. [Download CMake](http://www.cmake.org/cmake/resources/software.html)  

**Install OpenSSL**  

Download and install the [Windows OpenSSL binaries](http://slproweb.com/products/Win32OpenSSL.html).  

**Download LibSourcey**  

Clone the repository: `git clone https://github.com/sourcey/libsourcey.git`. If you haven't got Git for some reason you can download and extract the [package archive](https://github.com/sourcey/libsourcey) from Github.

##### Generate Project Files

Open the CMake GUI and set the project directory to point to the LibSourcey root directory. Execute "Configure" to do the initial configuration, then adjust any options, then press "Configure" again and then press "Generate".

[See here](#cmake-build-options) for a complete list of build options, and [here for examples](#examples).

##### Compile with Visual Studio

1. Generate solutions using CMake, as described above. Make sure, you chose the proper generator (32-bit or 64-bit)
2. Launch Visual Studio, locate and open the "libsourcey.sln" solution file in your generated build folder (eg: `C:\LibSourcey\build\libsourcey.sln`). Select "Debug" configuration, build the solution (Ctrl-Shift-B), and/or select "Release" and build it.
3. Add `{CMAKE_BINARY_DIR}\bin\Release`, `{CMAKE_BINARY_DIR}\bin\Debug` (containing "libscy*.dll" and "libscy*d.dll", respectively) to the system path (My Computer--[Right button click]->Properties->Advanced->Environment Variables->Path)

## CMake Build Options

The main build options you will want to configure are as follows:

* `BUILD_SHARED_LIBS`: Build shared libraries (.dll/.so) instead of static ones (.lib/.a). _default: NOT (WIN32 OR ANDROID OR IOS)_  
* `BUILD_DEPENDENCIES`: Build LibSourcey in source third-party dependencies. _default: ON_  
* `BUILD_MODULES`: Build LibSourcey modules. _default: ON_  
* `BUILD_MODULE_xxx`: Enable or disable a specific module replacing _xxx_ with the module name.  
* `BUILD_APPLICATIONS`: Build LibSourcey modules _default: ON_  
* `BUILD_APPLICATION_xxx`: Enable or disable a specific applications replacing _xxx_ with the module name.  
* `BUILD_MODULE_TESTS`: Build module test applications _default: ON_  
* `BUILD_MODULES_xxx`: Enable or disable a specific module replacing xxx with the module name.  
* `BUILD_MODULE_SAMPLES`: Build module sample applications _default: ON_  

If you are using third-party libraries is custom locations then make sure you update the CMake include paths: `CMAKE_SYSTEM_PREFIX_PATH` and `CMAKE_LIBRARY_PATH`.
The only third-party libraries that may need configuring if you're using them are FFmpeg, OpenCV and WebRTC.

For an exhaustive list of options check the `CMakeLists.txt` in the main directory.

## Examples

There is also plenty of examples available in the `samples` and `tests` folder of each module for you to cut your teeth on.

If you're interested in media streaming examples check out the `mediaserver` sample in the `av` module. Remember, you will need to compile LibSourcey with FFmpeg enabled to use it: https://github.com/sourcey/libsourcey/tree/master/src/av/samples/mediaserver

Two demo WebRTC applications are included; one for streaming live webcam and microphone streams to the browser [here](https://github.com/sourcey/libsourcey/tree/master/src/webrtc/samples/webrtcstreamer), and one for multiplex recording live WebRTC streams from the browser on the server side [here](https://github.com/sourcey/libsourcey/tree/master/src/webrtc/samples/webrtcrecorder).

For a redistributable C++ package manager, take a look at `pacm`, and specifically the `pacmconsole` application: http://sourcey.com/pacm/

Also, check out `pluga`, a simple C++ plugin API using LibSourcey: http://sourcey.com/pluga/

## Contributing

Pull Requests are always welcome, so if you make any improvements please feel free to float them back upstream :)

1. [Fork LibSourcey on Github](https://github.com/sourcey/libsourcey)
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create new Pull Request

## Issues

For bugs and issues please use the [Github issue tracker](https://github.com/sourcey/libsourcey/issues).
