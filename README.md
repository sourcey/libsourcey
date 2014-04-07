

# LibSourcey

Homepage
: [http://sourcey.com/libsourcey](http://sourcey.com/libsourcey)

Licence
: LGPL, <a href="/licence">Pro Licence</a>

Required Dependencies
: libuv, cmake, C++11 compiler

Optional Dependencies
: FFmpeg, OpenCV, WebRTC, OpenSSL, RtAudio, JsonCpp
  
LibSourcey is a collection of open source cross platform C++11 modules and classes which provide developers with a flexible high performance arsenal for the rapid development of real-time messaging and live media streaming applications. 

LibSourcey was been used in private production by Sourcey since 2005, and was open sourced in 2013 under the LGPL licence to promote development of real-time native and web applications.

Since version 0.92, C++11 and event-based asynchronous IO powered by Node.js's underlying libuv library are utilised throughout LibSourcey in to maximise performance and minimise concurrency reliance for building mission critical server side applications.

LibSourcey provides a simple and flexible method of capturing live audio/video streams (_OpenCV/FFmpeg_), processing, filtering and encoding them using any video format (_FFmpeg_), and broadcasting the result over the Internet (_libuv_). This is made possible by implementing a thin layer over the top of some brilliant open source projects, such as FFmpeg, OpenCV and libuv. The only required third-party dependency is libuv, and that is included in the local source and compiled automatically. All others dependencies are optional. 

LibSourcey has full support for the following protocols: **TCP**, **SSL**, **UDP**, **HTTP**, **JSON**, **XML**, **STUN**, **SDP**, **SocketIO**. 
Partial or non-standard support is implemented for the following protocols: **WebSockets**, **TURN**, **ICE**, **RTP**, and **XMPP**.
 
## Public Modules

The following modules are included in the public LibSourcey repository:
 
### Base 
_dependencies:_ libuv  
Re-usable utility classes and interfaces used throughout LibSourcey.

### Net
_dependencies:_ libuv, OpenSSL  
TCP, SSL and UDL socket implementation build on top of libuv architecture.

### HTTP
_dependencies:_ libuv, JsonCpp  
HTTP server and client stack including support for WebSockets, multipart streaming, and file transfers.        
    
### Media
_dependencies:_ libuv, OpenCV, FFmpeg, RtAudio  
Wrappers around FFmpeg and OpenCV for device capture, encoding, recording and streaming. The Media API makes extensive use of the PacketStream classes so that encoders, processors and packetisers can be dynamically added and removed from a media source.             

### STUN
<a href="http://tools.ietf.org/rfc/rfc5389">RFC 5389</a> implementation which includes support for ICE and TURN and TURN TCP messages. 

### TURN
_dependencies:_ libuv  
Server and client stack which supports both <a href="http://tools.ietf.org/rfc/rfc5766">RFC 5766 (Traversal Using Relays around NAT)</a> and <a href="http://tools.ietf.org/rfc/rfc6062">RFC 6062 (Traversal Using Relays around NAT Extensions for TCP Allocations)</a> specifications. 

### SDP
<a href="http://tools.ietf.org/rfc/rfc4566">RFC 4566</a> implementation which includes extra support for ICE headers. 
  
### SocketIO
_dependencies:_ libuv, JsonCpp  
SocketIO C++ client. Read more about <a href="http://socketIO">SocketIO</a>. 

### Symple
_dependencies:_ libuv, JsonCpp  
Client implementation of Sourcey's home grown real time messaging and presence protocol. <a href="/symple">More about Symple</a>. 

### Pacman
_dependencies:_ libuv, JsonCpp  
Pacman is an embeddable package manager which speaks JSON with the server. <a href="/pacman">More about Pacman</a>.    

### UVPP
_dependencies:_ libuv  
UVPP is a set of C++ wrappers for Joyent's fantastic libuv library. 

### JSON
_dependencies:_ JsonCpp  
Thin wrappers and helper functions for the JsonCpp library. 
  
## Private Modules

The following closed source modules are available.
Please contact us if you are interested in using any of them in your projects.

### ICE
_dependencies:_ libuv  
The ICE module is a complete implementation of <a href="http://tools.ietf.org/html/rfc5245">RFC 5245</a> (Interactive Connectivity Establishment) based on LibSourcey architecture.
ICE is a protocol for Network Address Translator (NAT) Traversal for Offer/Answer protocols.
This module is currently not open source. Please contact us if you are interested in using it.

### RTP
_dependencies:_ libuv  
Our RTP module is quite basic. At this point it only supports RTP and RTCP packetisation. RTCP session management still needs to implemented.
If anyone happens to make a project of this we would be very happy to improve our RTP module. 

### XML
_dependencies:_ pugixml  
Very thin wrappers around the pugixml XML library to better support LibSourcey architecture.
    
### XMPP
_dependencies:_ pugixml, libstrophe  
Our XMPP module includes a client with full Jingle session support. 
This module has been neglected for a while now in favor of other projects. 
Any bugfixes and improvements are welcome.  

## External Modules

The following LibSourcey modules are available in external repositories. 

### Anionu
_dependencies:_ libuv, OpevCV
The Anionu module includes a REST client interface for communicating with the Anionu public API.        

### ISpot
_dependencies:_ JsonCpp
ISpot is a complete C++ SDK and API for building <a href="http://anionu.com/spot">Spot</a> based applications and plug-ins. 
Spot is a part of <a href="http://anionu.com/spot">Anionu's</a> surveillance serivice.


## Install LibSourcey

<div class="panel callout radius">
If you are deploying server side on Ubuntu 12.04 then you're in luck, we have created `Capistrano` deploy recipies which make it a sinch to automate the LibSourcey build process including FFmpeg and OpenCV dependencies. Clone this [this repository](#) to download the scripts.
</div>

Install Git
 : * _Windows users_: Install [TortoiseGit](http://code.google.com/p/tortoisegit/), a convenient git front-end, which integrates right into Windows Explorer. MinGW users can use [msysgit]( from <a class="external" href="http://code.google.com/p/msysgit/downloads/list"></a>).
* _Linux users_: Install command-line git utility using your package manager, e.g. `apt-get install git` on Ubuntu and Debian. You can use <a href="http://www.syntevo.com/smartgithg/index.html" class="external">SmartGit</a> as a GUI client. SmartGit is cross-platform, btw.
* _Mac users_: If you installed Xcode (which you will need anyway) then you already have git. You can use [SourceTree](http://www.sourcetreeapp.com/), which is a very good GUI client.

Install CMake
 : CMake generates the LibSourcey project files so you can build on most platforms and compilers. CMake is also required to generate makefiles for OpenCV. [Download CMake](http://www.cmake.org/cmake/resources/software.html)

Install OpenSSL
 : * _Windows users_: download and install the [Windows OpenSSL binaries](http://slproweb.com/products/Win32OpenSSL.html).  
 * _Linux users_: Install the openssl package (if you don't already have it), ensuring the get the libssl-dev package which contains the developemnt headers: `apt-get install openssl libssl-dev`

Download LibSourcey
 : Clone the repository: `git clone https://github.com/sourcey/libsourcey.git`.  
   If you haven't got Git for some reason you can download and extract the [package archive](https://github.com/sourcey/libsourcey) from Github.

### Generating Project Files

Depending on your platform, you may choose run CMake from the command line, or use the CMake GUI.

#### CMake Command Line (Linux)

~~~ bash 
# dependencies: openssl libssl-dev cmake pkg-config 
# optional: libjack-jackd2-dev

git clone https://github.com/sourcey/libsourcey.git
cd libsourcey
mkdir -p build
cmake .. -DCMAKE_BUILD_TYPE=RELEASE # extra cmake commands here...
make
make install
~~~

#### CMake GUI (Windows/Mac)

If you use CMake GUI, execute "Configure" to do the initial configuration, then adjust any options, then press "Configure" again and then press "Generate".

#### CMake Options

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

### Building And Compiling

#### Compiling with Visual Studio
1. Generate solutions using CMake, as described above. Make sure, you chose the proper generator (32-bit or 64-bit)
1. Launch Visual Studio, locate and open the "LibSourcey.sln" solution file in your generated build folder (eg: `C:\LibSourcey\build\LibSourcey.sln`). Select "Debug" configuration, build the solution (Ctrl-Shift-B), and/or select "Release" and build it.
1. Add `{CMAKE_BINARY_DIR}\bin\Release`, `{CMAKE_BINARY_DIR}\bin\Debug` (containing "LibSourcey*.dll" and "LibSourcey*d.dll", respectively) to the system path (My Computer--[Right button click]->Properties->Advanced->Environment Variables->Path)

#### Compiling with GCC (MinGW, MSYS, Linux)

* Generate makefiles using CMake (choose "MinGW Makefiles" generator on Windows, "Unix Makefiles" on other OSes) as described above.
* Enter the output CMake directory (denoted as `{CMAKE_BINARY_DIR}` further) and type `make -j [optional_number_of_threads]` on Unix, or `mingw32-make`on for MinGW. Windows users can also run parallel build of LibSourcey. To do that, please, define "SHELL" environment variable (My Computer--[Right button click]->Properties->Advanced->Environment Variables->Path) and set it to "cmd.exe" (without quotes). Then you can run `mingw32-make -j` (note that "-j" must go without any numerical parameter).
* _Linux, MacOSX_ Then you can optionally run `sudo make install` (on Linux or MacOSX). Note, that if you are using CMake for your projects, it is not necessary to run `make install`. Just use LibSourcey from where you built it. This is actually the recommended approach, since it does not pollute system directories with potentially conflicting LibSourcey versions.
* _Windows_ If you built LibSourcey as dynamic libraries (DLLs), you may want to add {CMAKE_BINARY_DIR}\bin to the system path (My Computer--[Right button click]->Properties->Advanced->Environment Variables->Path) to let Windows find "scy*.dll" etc.)
  
#### Compile with Xcode
* Generate Xcode project using CMake, as described above.
* Launch Xcode, locate and open LibSourcey.xcodeproj. Select "Debug", build the BUILD_ALL target (Cmd-B), select "Release" and build it too.

## Examples

There is also plenty of examples available in the `samples` and `tests` folder of each module for you to sink your teeth into. Also, check out the [PacketStream API](http://sourcey.com/libsourcey-packetstream-api) article to get a feel for the LibSourcey way of doing things. 

## Contributing

1. [Fork LibSourcey on Github](https://github.com/sourcey/libsourcey)
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create new Pull Request

## Issues

If you find any bugs or issues please use the new [Github issue tracker](https://github.com/sourcey/libsourcey/issues).