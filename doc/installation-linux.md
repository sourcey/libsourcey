# Installing on Linux

This guide is written for Ubuntu 14.04, although installation should be fairly portable across most flavors of Linux.

## Install Dependencies

Before doing anything make sure you have the core dependencies installed:

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

### Install FFmpeg (optional)

FFmpeg is an optional but recommended dependency that's required to make use of LibSourcey's media encoding capabilities.

Most versions on Linux have FFmpeg development libraries that you can install directly from the package manager. If you're on a newer version of Ubuntu (>= 15.04) then you can install the required libraries as follows:

~~~ bash
sudo apt-get install --yes libavcodec-ffmpeg-dev libavdevice-ffmpeg-dev libavfilter-ffmpeg-dev libavformat-ffmpeg-dev libswresample-ffmpeg-dev libpostproc-ffmpeg-dev
~~~

Otherwise if you want to compile FFmpeg yourself you can follow the [official guide for installing FFmpeg](http://trac.ffmpeg.org/wiki/CompilationGuide) that works out of the box with LibSourcey without configuration.

If FFmpeg is installed in a non-standard location you can enable expose it to the CMake build system like so:

~~~ bash
cmake .. -DWITH_FFMPEG=ON -DFFMPEG_ROOT_DIR=<PATH_TO_FFMPEG_BUILD_DIRECTORY>
~~~

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

### Install WebRTC (optional)

<!-- Installing WebRTC can be tricky and time consuming, but using the script below you should be able to get started without a hitch. Using the script is highly recommended as there are a copuple of caveats to take note of when compiling WebRTC for use with LibSourcey:

1. RTTI must be enabled (the `-no_rtti` compiler flag must be removed)
2. libstdc++ iterator debugging must be disabled or it conflicts with external dependencies compiled without it (ninja build must set `enable_iterator_debugging=false`) -->

The easiest way to get started with WebRTC is to download the precompiled static libraries and headers available here: https://sourcey.com/precompiled-webrtc-libraries/

Alternatively if you want to compile WebRTC yourself you can use our automated build scripts, which can be found here: https://github.com/sourcey/webrtc-builds

~~~ bash
cd ~/tmp
git clone git@github.com:sourcey/webrtc-builds.git
cd webrtc-builds
./build.sh
~~~

Once installed enable and add the WebRTC root directory to your build command like so:

~~~ bash
cmake .. -DWITH_WEBRTC=ON -DWEBRTC_ROOT_DIR=<PATH_TO_WEBRTC_ROOT_DIRECTORY>
~~~

### Install LibSourcey

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

[See here](/installation.md#cmake-build-options) for a complete list of build options.
