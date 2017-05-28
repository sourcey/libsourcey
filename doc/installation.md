## Installation

LibSourcey uses `CMake` to generate the project files for your compiler, and the codebase is cross platform so it should compile on any system with a modern C++14 compiler. LibSourcey has been successfully compiled and used on the following platforms:

* Linux (`gcc` >= 5.0)
* OS X (`Xcode`, `gcc`)
* Windows (`msys`, `vs`)
* Android
* iOS

**Note**: If using `gcc` then version >= 5.0 is required because older versions have a bug and that doesn't accept parameters pack in lambdas. See here: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=55914

Please refer to the platform specific installation guides:

* [Linux](/installation-linux.md)
* [OS X](/installation-osx.md)
* [Windows](/installation-windows.md)

## Dependencies

<!-- * **LibUV** (>= 1.8.0, required, internal)  
  Networking, filesystem and cross platform utilities -->
* **OpenSSL** (>= 1.0.1g, required, external)  
  Required by the `net` and `crypto` modules for SSL networking, encryption and cryptography
* **FFmpeg** (>= 2.8.3, optional, external)  
  Required to compile media related encoding, decoding, device and capture classes in the `av` module
* **WebRTC** (optional, external)  
  Peer-to-peer video conferencing
* **OpenCV** (>= 3.0, optional, external)  
  Video capture and computer vision algorithms
<!-- * **RtAudio** (>= 4.1.2, optional, internal)
Audio capture -->

## CMake Build Options

The main build options you will want to configure are as follows:

* `BUILD_SHARED_LIBS`: Build shared libraries (.dll/.so) instead of static ones (.lib/.a). _default: NOT (WIN32 OR ANDROID OR IOS)_  
* `BUILD_DEPENDENCIES`: Build LibSourcey in source third-party dependencies. _default: ON_  
* `BUILD_MODULES`: Build LibSourcey modules. _default: ON_  
* `BUILD_MODULE_xxx`: Enable or disable a specific module replacing `xxx` with the module name.  
* `BUILD_APPLICATIONS`: Build LibSourcey modules _default: ON_  
* `BUILD_APPLICATION_xxx`: Enable or disable a specific applications replacing `xxx` with the module name.  
* `BUILD_TESTS`: Build module test applications _default: ON_  
* `BUILD_TEST_xxx`: Enable or disable a specific test replacing `xxx` with the module name.
* `BUILD_SAMPLES`: Build module test applications _default: ON_  
* `BUILD_SAMPLE_xxx`: Enable or disable a specific test replacing `xxx` with the module name.
* `BUILD_MODULE_SAMPLES`: Build module sample applications _default: ON_
* `ENABLE_LOGGING`: Enable internal debug logging _default: OFF_

If you are using third-party libraries is custom locations then make sure you update the CMake include paths: `CMAKE_SYSTEM_PREFIX_PATH` and `CMAKE_LIBRARY_PATH`.

The only third-party libraries that may need configuring if you're using them are FFmpeg, OpenCV and WebRTC.

For an exhaustive list of options check the `CMakeLists.txt` in the main directory.

## Building Packages

LibSourcey can be packaged into `deb`, `rpm`, `tar.gz`, `zip` and many other formats using CPack:

~~~ bash
cd libsourcey/build
cmake .. -DCPACK_GENERATOR=DEB # other build commands here
sudo cpack ..
~~~

The output package will be in the `build` folder.

Check the [CPack documentation](https://cmake.org/Wiki/CMake:CPackPackageGenerators) for a full list of supported package generators.

## CMake Build Shortcuts

Below are a number of build shortcuts that can be used to build LibSourcey with different features enabled:

~~~ bash
# Default debug build
cmake .. -DCMAKE_BUILD_TYPE=DEBUG -DBUILD_SHARED_LIBS=OFF \
         -DBUILD_MODULES=ON -DBUILD_APPLICATIONS=ON \
         -DBUILD_SAMPLES=ON -DBUILD_TESTS=ON \
         -DWITH_FFMPEG=ON -DWITH_WEBRTC=OFF

# All modules (selective)
cmake .. -DCMAKE_BUILD_TYPE=DEBUG -DBUILD_SHARED_LIBS=OFF \
         -DBUILD_APPLICATIONS=OFF -DBUILD_MODULES=OFF \
         -DBUILD_SAMPLES=ON -DBUILD_TESTS=ON \
         -DWITH_FFMPEG=ON -DWITH_OPENCV=OFF \
         -DWITH_WEBRTC=OFF -DBUILD_MODULE_archo=ON \
         -DBUILD_MODULE_base=ON -DBUILD_MODULE_crypto=ON \
         -DBUILD_MODULE_http=ON -DBUILD_MODULE_json=ON \
         -DBUILD_MODULE_av=ON -DBUILD_MODULE_net=ON \
         -DBUILD_MODULE_pacm=ON -DBUILD_MODULE_pluga=ON \
         -DBUILD_MODULE_sked=ON -DBUILD_MODULE_socketio=ON \
         -DBUILD_MODULE_stun=ON -DBUILD_MODULE_symple=ON \
         -DBUILD_MODULE_turn=ON -DBUILD_MODULE_util=ON \
         -DBUILD_MODULE_webrtc=ON

# Minimum build (uv, base)
cmake .. -DCMAKE_BUILD_TYPE=DEBUG -DBUILD_MODULES=OFF \
         -DBUILD_MODULE_base=ON

# Media build
cmake .. -DCMAKE_BUILD_TYPE=DEBUG -DBUILD_SHARED_LIBS=OFF \
         -DBUILD_APPLICATIONS=OFF -DBUILD_MODULES=OFF \
         -DBUILD_SAMPLES=ON -DBUILD_TESTS=ON \
         -DWITH_FFMPEG=ON -DWITH_OPENCV=OFF \
         -DBUILD_MODULE_av=ON -DBUILD_MODULE_base=ON


# WebRTC build (with Symple)
cmake .. -DCMAKE_BUILD_TYPE=DEBUG -DBUILD_SHARED_LIBS=OFF \
         -DBUILD_MODULES=OFF -DBUILD_APPLICATIONS=OFF \
         -DBUILD_SAMPLES=OFF -DBUILD_TESTS=OFF \
         -DWITH_WEBRTC=ON -DWITH_FFMPEG=ON \
         -DBUILD_MODULE_base=ON -DBUILD_MODULE_crypto=ON \
         -DBUILD_MODULE_http=ON -DBUILD_MODULE_json=ON \
         -DBUILD_MODULE_av=ON -DBUILD_MODULE_net=ON \
         -DBUILD_MODULE_socketio=ON -DBUILD_MODULE_symple=ON \
         -DBUILD_MODULE_util=ON -DBUILD_MODULE_webrtc=ON

# Symple build (no media)
cmake .. -DCMAKE_BUILD_TYPE=DEBUG -DBUILD_SHARED_LIBS=OFF \
         -DBUILD_APPLICATIONS=ON -DBUILD_MODULES=OFF \
         -DBUILD_SAMPLES=ON -DBUILD_TESTS=ON \
         -DWITH_FFMPEG=OFF -DWITH_OPENCV=OFF \
         -DBUILD_MODULE_archo=ON -DBUILD_MODULE_base=ON \
         -DBUILD_MODULE_crypto=ON -DBUILD_MODULE_http=ON \
         -DBUILD_MODULE_json=ON -DBUILD_MODULE_net=ON \
         -DBUILD_MODULE_socketio=ON -DBUILD_MODULE_symple=ON \
         -DBUILD_MODULE_util=ON

# Symple build (with media)
cmake .. -DCMAKE_BUILD_TYPE=DEBUG -DBUILD_SHARED_LIBS=OFF \
         -DBUILD_APPLICATIONS=ON -DBUILD_MODULES=OFF \
         -DBUILD_SAMPLES=ON -DBUILD_TESTS=ON \
         -DWITH_FFMPEG=ON -DWITH_OPENCV=OFF \
         -DBUILD_MODULE_archo=ON -DBUILD_MODULE_base=ON \
         -DBUILD_MODULE_crypto=ON -DBUILD_MODULE_http=ON \
         -DBUILD_MODULE_json=ON -DBUILD_MODULE_av=ON \
         -DBUILD_MODULE_net=ON -DBUILD_MODULE_socketio=ON \
         -DBUILD_MODULE_symple=ON -DBUILD_MODULE_util=ON


# HTTP build
cmake .. -DCMAKE_BUILD_TYPE=RELEASE -DBUILD_SHARED_LIBS=OFF \
         -DBUILD_APPLICATIONS=ON -DBUILD_MODULES=OFF \
         -DBUILD_SAMPLES=ON -DBUILD_TESTS=ON \
         -DBUILD_MODULE_archo=ON -DBUILD_MODULE_base=ON \
         -DBUILD_MODULE_crypto=ON -DBUILD_MODULE_http=ON \
         -DBUILD_MODULE_json=ON -DBUILD_MODULE_net=ON \
         -DBUILD_MODULE_util=ON

# STUN/TURN build
cmake .. -DCMAKE_BUILD_TYPE=DEBUG -DBUILD_SHARED_LIBS=OFF \
         -DBUILD_APPLICATIONS=ON -DBUILD_MODULES=OFF \
         -DBUILD_MODULE_base=ON -DBUILD_MODULE_crypto=ON \
         -DBUILD_MODULE_net=ON -DBUILD_MODULE_stun=ON \
         -DBUILD_MODULE_turn=ON -DBUILD_MODULE_util=OFF

~~~
