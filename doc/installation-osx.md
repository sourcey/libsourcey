# Installing on Apple (OS X)

This guide is for written for Mac users using [Homebrew](http://brew.sh).

## Install Dependencies

<!-- # LibUV:
brew install --HEAD libuv -->

~~~ bash
# OpenSSL:
brew install openssl
brew link --force openssl

# CMake:
brew install cmake
~~~

Please see the [Linux instructions](/installation-linux.md) for other optional dependencies, such as FFmpeg and WebRTC. <!-- such as FFmpeg and OpenCV -->

## Install LibSourcey

~~~ bash
cd ~/tmp
git clone https://github.com/sourcey/libsourcey.git
cd libsourcey
mkdir build
cd build
cmake .. -D CMAKE_BUILD_TYPE=RELEASE # extra cmake commands here, see below for a full list...
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

<!-- ## Generate Project Files

See linux instructions. -->

<!-- Open the CMake GUI and set the project directory to point to the LibSourcey root directory. Execute "Configure" to do the initial configuration, then adjust any options, then press "Configure" again and then press "Generate". -->

<!-- [See here](/installation.md#cmake-build-options) for a complete list of build options. -->

## Compile with Xcode

* Generate Xcode project using CMake, as described above.
* Launch Xcode, locate and open `libsourcey.xcodeproj`. Select "Debug", build the BUILD_ALL target (Cmd-B), select "Release" and build it too.
