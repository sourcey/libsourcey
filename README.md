# LibSourcey

**Homepage**: [http://sourcey.com/libsourcey](http://sourcey.com/libsourcey)  
**Licence**: LGPL
  
LibSourcey is a collection of open source cross platform C++11 modules and classes that provide developers with a flexible high performance arsenal for the rapid development of real-time messaging and live media streaming applications. 

LibSourcey is built on Node.js's underlying libuv library and utilises event-based asynchronous IO in to maximise performance and minimise concurrency reliance for building mission critical server-side applications. Modern C++11 design principles have been adhered to throughout for clear and readable code, and straighforward integration into existing projects.

LibSourcey provides a simple and flexible method of capturing live audio/video streams (_OpenCV/FFmpeg_), processing, filtering and encoding them using any video format (_FFmpeg_), and broadcasting the result over the Internet (_libuv_). This is made possible by implementing a thin layer over the top of some brilliant open source projects, such as FFmpeg, OpenCV and libuv. The only required third-party dependency is libuv, and that is included in the local source and compiled automatically. All others dependencies are optional. 

LibSourcey provides full support for the following protocols: **TCP**, **SSL**, **UDP**, **HTTP**, **JSON**, **XML**, **STUN**, **SDP**, **SocketIO**.  
Partial support is provided for the following protocols: **WebSockets**, **TURN**, **ICE**, **RTP**, and **XMPP**.
 
## Dependencies

_Required_: libuv, CMake, C++11 compiler (GCC, Visual Studio, Xcode)  
_Optional_: FFmpeg (>= 2.8.3), OpenCV (>= 3.0), WebRTC, OpenSSL (>= 1.0.1g), RtAudio, JsonCpp

## Installation

### Installing on Linux

This guide has been written for Ubuntu 14.04, but installation is very simple and should be easily protable for most flavours of Linux.

First install the necessary dependencies:

~~~ bash 
sudo apt-get update

# required dependencies
apt-get install -y build-essential pkg-config git openssl libssl-dev cmake libjack-jackd2-dev

# optional dependencies
apt-get install -y opencv-dev
~~~

_Note:_ To compile LibSourcey with video and streaming capabilities enabled you should install the latest versions of both FFmpeg and OpenCV. 

If you need to install FFmpeg, then you can use the official FFmpeg installation guide which works out of the box with LibSourcey: http://trac.ffmpeg.org/wiki/CompilationGuide/Ubuntu

Now checkout and compile the source:

~~~ bash 
cd ~/tmp
git clone https://github.com/sourcey/libsourcey.git
cd libsourcey
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=RELEASE # extra cmake commands here...
make
sudo make install
~~~

Simple! [See here](#cmake-build-options) for a complete list of build options, and [here for examples](#examples).

### Installing on Windows

#### Install Dependencies

Install Git  
 : Install [TortoiseGit](http://code.google.com/p/tortoisegit/), a convenient git front-end that integrates right into Windows Explorer. MinGW users can use [msysgit]( from <a class="external" href="http://code.google.com/p/msysgit/downloads/list"></a>).  

Install CMake  
 : CMake generates the LibSourcey project files so you can build on most platforms and compilers. [Download CMake](http://www.cmake.org/cmake/resources/software.html)  

Install OpenSSL  
 : Download and install the [Windows OpenSSL binaries](http://slproweb.com/products/Win32OpenSSL.html).  

Download LibSourcey  
 : Clone the repository: `git clone https://github.com/sourcey/libsourcey.git`.  
   If you haven't got Git for some reason you can download and extract the [package archive](https://github.com/sourcey/libsourcey) from Github.

#### Generate Project Files

Open the CMake GUI and set the project directory to point to the LibSourcey root directory. Execute "Configure" to do the initial configuration, then adjust any options, then press "Configure" again and then press "Generate".

[See here](#cmake-build-options) for a complete list of build options, and [here for examples](#examples).

#### Compile with Visual Studio

1. Generate solutions using CMake, as described above. Make sure, you chose the proper generator (32-bit or 64-bit)
2. Launch Visual Studio, locate and open the "libsourcey.sln" solution file in your generated build folder (eg: `C:\LibSourcey\build\libsourcey.sln`). Select "Debug" configuration, build the solution (Ctrl-Shift-B), and/or select "Release" and build it.
3. Add `{CMAKE_BINARY_DIR}\bin\Release`, `{CMAKE_BINARY_DIR}\bin\Debug` (containing "libscy*.dll" and "libscy*d.dll", respectively) to the system path (My Computer--[Right button click]->Properties->Advanced->Environment Variables->Path)

### Installing on Apple (MacOS)

Install Git  
 : Download the [latest Git installer package](http://code.google.com/p/git-osx-installer/downloads/list?can=3), double click on the installer to start the installation wizard. You’ll be prompted for your system password in order for the installer to complete.

Install CMake  
 : CMake generates the LibSourcey project files so you can build on most platforms and compilers. [Download CMake](http://www.cmake.org/cmake/resources/software.html)

Install OpenSSL  
 : If you don't already have OpenSSL development headers on your Mac, then please follow [this guide](http://www.opensource.apple.com/source/OpenSSL/OpenSSL-7.1/openssl/INSTALL?txt) to install them.  

Download LibSourcey  
 : Clone the repository: `git clone https://github.com/sourcey/libsourcey.git`.  
   If you haven't got Git for some reason you can download and extract the [package archive](https://github.com/sourcey/libsourcey) from Github.

#### Generate Project Files

Open the CMake GUI and set the project directory to point to the LibSourcey root directory. Execute "Configure" to do the initial configuration, then adjust any options, then press "Configure" again and then press "Generate".

[See here](#cmake-build-options) for a complete list of build options, and [here for examples](#examples).

#### Compile with Xcode

* Generate Xcode project using CMake, as described above.
* Launch Xcode, locate and open libsourcey.xcodeproj. Select "Debug", build the BUILD_ALL target (Cmd-B), select "Release" and build it too.

### CMake Build Options

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

If you're interested in media streaming examples check out the `mediaserver` sample in the `symple` module. Remember, you will need to compile LibSourcey with OpenCV and FFmpeg enabled to use it: https://github.com/sourcey/libsourcey/tree/master/src/symple/samples/mediaserver

For an example of the LibSourcey way of doing things, check out this article about the PacketSteram API: http://sourcey.com/libsourcey-packetstream-api/

For a redistributable C++ package manager, take a look at `pacm`, and specifically the `pacmconsole` application: http://sourcey.com/pacm/

Also, check out `pluga`, a simple C++ plugin API using LibSourcey: http://sourcey.com/pluga/

## Core Modules

The following modules are included in the core LibSourcey repository:
 
#### Base 
_dependencies:_ libuv  
Re-usable utility classes and interfaces used throughout LibSourcey.

#### Net
_dependencies:_ libuv, OpenSSL  
TCP, SSL and UDL socket implementation build on top of libuv architecture.

#### HTTP
_dependencies:_ libuv, JsonCpp  
HTTP server and client stack including support for WebSockets, multipart streaming, and file transfers.  
    
#### Media
_dependencies:_ libuv, OpenCV, FFmpeg, RtAudio  
Wrappers around FFmpeg and OpenCV for device capture, encoding, recording and streaming. The Media API makes extensive use of the PacketStream classes so that encoders, processors and packetisers can be dynamically added and removed from a media source.  

#### UV
_dependencies:_ libuv  
The UV module is a set of C++ wrappers for Joyent's brilliant libuv library. 

#### STUN
[RFC 5389](http://tools.ietf.org/rfc/rfc5389) implementation which includes support for ICE and TURN and TURN TCP messages. 

#### TURN
_dependencies:_ libuv  
Server and client stack which supports both [RFC 5766 (Traversal Using Relays around NAT)](http://tools.ietf.org/rfc/rfc5766) and [RFC 6062 (Traversal Using Relays around NAT Extensions for TCP Allocations)](http://tools.ietf.org/rfc/rfc6062) specifications. 

#### SDP
[RFC 4566](http://tools.ietf.org/rfc/rfc4566) implementation which includes extra support for ICE headers. 
  
#### SocketIO
_dependencies:_ libuv, JsonCpp  
SocketIO C++ client. Read more about [SocketIO](http://socket.io). 

#### Symple
_dependencies:_ libuv, JsonCpp  
Client implementation of Sourcey's home grown real time messaging and presence protocol. [More about Symple](<http://sourcey.com/symple).    

#### JSON
_dependencies:_ JsonCpp  
Thin wrappers and helper functions for the JsonCpp library. 

## External Modules

The following LibSourcey modules are available in external repositories:

#### Pacm
_dependencies:_ libuv, JsonCpp  
Pacm is an embeddable package manager which speaks JSON with the server. [More about Pacm](http://sourcey.com/pacm). 

#### Pluga
Pluga is a simple C++ plugin system that's dead simple to use in your own projects. [More about Pluga](http://sourcey.com/pluga). 

#### Anionu SDK
_dependencies:_ libuv, OpevCV
The Anionu SDK includes a C++ API, tools, and client implementation for building [Spot](http://anionu.com/spot) plugins and applications that integrate with the [Anionu cloud surveillance serivice](https://anionu.com).

## Private Modules

The following closed source modules are available. Please contact us if you are interested in using any of them in your projects.

#### ICE
_dependencies:_ libuv  
The ICE module is a complete implementation of [RFC 5245 (Interactive Connectivity Establishment)](http://tools.ietf.org/html/rfc5245) based on LibSourcey architecture.
ICE is a protocol for Network Address Translator (NAT) Traversal for Offer/Answer protocols.
This module is currently not open source. Please contact us if you are interested in using it.

#### RTP
_dependencies:_ libuv  
Our RTP module is quite basic and at this point it only supports RTP and RTCP packetisation. RTCP session management still needs to implemented. If anyone happens to make a project of this we would be very happy to improve our RTP module. 

#### XML
_dependencies:_ pugixml  
Thin wrappers around the pugixml XML library to better support LibSourcey architecture.
    
#### XMPP
_dependencies:_ pugixml, libstrophe  
Our XMPP module includes a full client implementation with Jingle session support. 

## Contributing

All contributions are very welcome!

1. [Fork LibSourcey on Github](https://github.com/sourcey/libsourcey)
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create new Pull Request

## Issues

If you find any bugs or issues please use the [Github issue tracker](https://github.com/sourcey/libsourcey/issues).
