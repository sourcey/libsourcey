# LibSourcey

**Homepage**: [http://sourcey.com/libsourcey](http://sourcey.com/libsourcey)  
**Licence**: LGPL
  
LibSourcey is a collection of open source cross platform C++11 modules and classes which provide developers with a flexible high performance arsenal for the rapid development of real-time messaging and live media streaming applications. 

LibSourcey was been used in private production by Sourcey since 2005, and was open sourced in 2013 under the LGPL licence to promote development of real-time native and web applications.

Since version 0.92, C++11 and event-based asynchronous IO powered by Node.js's underlying libuv library are utilised throughout LibSourcey in to maximise performance and minimise concurrency reliance for building mission critical server-side applications.

LibSourcey provides a simple and flexible method of capturing live audio/video streams (_OpenCV/FFmpeg_), processing, filtering and encoding them using any video format (_FFmpeg_), and broadcasting the result over the Internet (_libuv_). This is made possible by implementing a thin layer over the top of some brilliant open source projects, such as FFmpeg, OpenCV and libuv. The only required third-party dependency is libuv, and that is included in the local source and compiled automatically. All others dependencies are optional. 

LibSourcey has full support for the following protocols: **TCP**, **SSL**, **UDP**, **HTTP**, **JSON**, **XML**, **STUN**, **SDP**, **SocketIO**. 
Partial or non-standard support is implemented for the following protocols: **WebSockets**, **TURN**, **ICE**, **RTP**, and **XMPP**.
 
## Dependencies

_Required_: libuv, CMake, C++11 compiler  
_Optional_: FFmpeg (>= 2.8.3), OpenCV (>= 3.0), WebRTC, OpenSSL (> 1.0.1g), RtAudio, JsonCpp
 
## Public Modules

The following modules are included in the public LibSourcey repository:
 
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

#### UVPP
_dependencies:_ libuv  
UVPP is a set of C++ wrappers for Joyent's fantastic libuv library. 

#### JSON
_dependencies:_ JsonCpp  
Thin wrappers and helper functions for the JsonCpp library. 
  
## Private Modules

The following closed source modules are available.
Please contact us if you are interested in using any of them in your projects.

#### ICE
_dependencies:_ libuv  
The ICE module is a complete implementation of [RFC 5245 (Interactive Connectivity Establishment)](http://tools.ietf.org/html/rfc5245) based on LibSourcey architecture.
ICE is a protocol for Network Address Translator (NAT) Traversal for Offer/Answer protocols.
This module is currently not open source. Please contact us if you are interested in using it.

#### RTP
_dependencies:_ libuv  
Our RTP module is quite basic. At this point it only supports RTP and RTCP packetisation. RTCP session management still needs to implemented.
If anyone happens to make a project of this we would be very happy to improve our RTP module. 

#### XML
_dependencies:_ pugixml  
Very thin wrappers around the pugixml XML library to better support LibSourcey architecture.
    
#### XMPP
_dependencies:_ pugixml, libstrophe  
Our XMPP module includes a client with full Jingle session support. 
This module has been neglected for a while now in favor of other projects. 
Any bugfixes and improvements are welcome.  

## External Modules

The following LibSourcey modules are available in external repositories. 

#### Anionu SDK
_dependencies:_ libuv, OpevCV
The Anionu SDK includes a C++ API, tools, and client implementation for building [Spot](http://anionu.com/spot) plugins and applications that integrate with the [Anionu cloud surveillance serivice](https://anionu.com).

#### Pacm
_dependencies:_ libuv, JsonCpp  
Pacm is an embeddable package manager which speaks JSON with the server. [More about Pacm](http://sourcey.com/pacm). 

## Installation

### Linux

This guide has been written for Ubuntu 14.04, but should be easily protable for most flavours of Linux.

First install the necessary dependencies:

~~~ bash 
sudo apt-get update

# required dependencies
apt-get install -y build-essential pkg-config git openssl libssl-dev cmake libjack-jackd2-dev

# optional dependencies
apt-get install -y opencv-dev
~~~

_Note_: To compile LibSourcey with video and streaming capabilities enabled you should install the latest versions of both FFmpeg and OpenCV. 

If you don't have FFmpeg development headers installed, then you can use the official FFmpeg installation guide which works out of the box with LibSourcey: http://trac.ffmpeg.org/wiki/CompilationGuide/Ubuntu

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

Simple! For a complete list of build options [see here](#cmake-build-options).

### Windows

#### Install Dependencies

**Install Git**  
 : Install [TortoiseGit](http://code.google.com/p/tortoisegit/), a convenient git front-end that integrates right into Windows Explorer. MinGW users can use [msysgit]( from <a class="external" href="http://code.google.com/p/msysgit/downloads/list"></a>).  

**Install CMake**  
CMake generates the LibSourcey project files so you can build on most platforms and compilers. [Download CMake](http://www.cmake.org/cmake/resources/software.html)  

**Install OpenSSL**  
Download and install the [Windows OpenSSL binaries](http://slproweb.com/products/Win32OpenSSL.html).  

** Download LibSourcey**  
Clone the repository: `git clone https://github.com/sourcey/libsourcey.git`. If you haven't got Git for some reason you can download and extract the [package archive](https://github.com/sourcey/libsourcey) from Github.

#### Generate Project Files

Open the CMake GUI and set the project directory to point to the LibSourcey root directory. Execute "Configure" to do the initial configuration, then adjust any options, then press "Configure" again and then press "Generate".

#### Compile With Visual Studio

1. Generate solutions using CMake, as described above. Make sure, you chose the proper generator (32-bit or 64-bit)
2. Launch Visual Studio, locate and open the "libsourcey.sln" solution file in your generated build folder (eg: `C:\LibSourcey\build\libsourcey.sln`). Select "Debug" configuration, build the solution (Ctrl-Shift-B), and/or select "Release" and build it.
3. Add `{CMAKE_BINARY_DIR}\bin\Release`, `{CMAKE_BINARY_DIR}\bin\Debug` (containing "libscy*.dll" and "libscy*d.dll", respectively) to the system path (My Computer--[Right button click]->Properties->Advanced->Environment Variables->Path)

### Apple (MacOS)

**Install Git**  
Download the [latest Git installer package](http://code.google.com/p/git-osx-installer/downloads/list?can=3), double click on the installer to start the installation wizard. You’ll be prompted for your system password in order for the installer to complete.

**Install CMake**  
CMake generates the LibSourcey project files so you can build on most platforms and compilers. [Download CMake](http://www.cmake.org/cmake/resources/software.html)

**Install OpenSSL**  
If you don't already have OpenSSL development headers on your Mac, then please follow [this guide](http://www.opensource.apple.com/source/OpenSSL/OpenSSL-7.1/openssl/INSTALL?txt) to install them.  

Download LibSourcey**  
Clone the repository: `git clone https://github.com/sourcey/libsourcey.git`. If you haven't got Git for some reason you can download and extract the [package archive](https://github.com/sourcey/libsourcey) from Github.

#### Generate Project Files

Open the CMake GUI and set the project directory to point to the LibSourcey root directory. Execute "Configure" to do the initial configuration, then adjust any options, then press "Configure" again and then press "Generate".

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
* `BUILD_MODULE_TESTS`: Build module test applications _default: OFF_  
* `BUILD_MODULES_xxx`: Enable or disable a specific module replacing xxx with the module name.  
* `BUILD_MODULE_SAMPLES`: Build module sample applications _default: OFF_  

If you are using third-party libraries is custom locations then make sure you update the CMake include paths: `CMAKE_SYSTEM_PREFIX_PATH` and `CMAKE_LIBRARY_PATH`.
The only third-party libraries that may need configuring if you're using them are FFmpeg, OpenCV and WebRTC.

For an exhaustive list of options check the `CMakeLists.txt` in the main directory. 

## Examples

There is also plenty of examples available in the `samples` and `tests` folder of each module for you to cut your teeth on.

## Contributing

1. [Fork LibSourcey on Github](https://github.com/sourcey/libsourcey)
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create new Pull Request

## Issues

If you find any bugs or issues please use the [Github issue tracker](https://github.com/sourcey/libsourcey/issues).
