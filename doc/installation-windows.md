# Installing on Windows

## Install Dependencies

### Install Git

[Download git-scm](https://git-scm.com/)
You can also use either the git command line or GUI client.  

### Install CMake

[Download CMake](https://cmake.org/download/)

CMake generates the LibSourcey project files so you can build on most platforms and compilers.
Here we use CMake to generate project for Visual Studio.

### Install OpenSSL

Download and install the [Windows OpenSSL binaries](http://slproweb.com/products/Win32OpenSSL.html).
Don't forget to set system path if you download `zip` file version, or if the OpenSSL Installer failed to set the properly:

* `<PATH_TO_OPENSSL_INSTALL_DIR>\bin`

You can set the `OPENSSL_ROOT_DIR` CMake variable to point to your OpenSSL installation directory and the build system will do the rest.

### Install FFmpeg

Download and install [FFmpeg for Windows](https://ffmpeg.zeranoe.com/builds/).
Notice that you need to download both `Shared` and `Dev` version.
Extract zip files and set them to head of your system's `PATH` (you can just add new item in Windows 10):

* `<PATH_TO_FFMPEG_SHARED_DIR>\bin`

Now you can set the `FFMPEG_ROOT_DIR` CMake variable to point to your FFmpeg dev installation directory and the build system will do the rest.

For example, if you want to build LibSourcey with FFmpeg enabled your CMake command might look something like this:

~~~ bash
cmake .. -DOPENSSL_ROOT_DIR=E:\dev\vendor\OpenSSL-Win64 -DWITH_FFMPEG=ON -DFFMPEG_ROOT_DIR=E:\dev\vendor\ffmpeg-3.2.2-win64-dev
~~~

### Download LibSourcey

Clone the repository: `git clone https://github.com/sourcey/libsourcey.git`. If you haven't got Git for some reason you can download and extract the [package archive](https://github.com/sourcey/libsourcey) from GitHub.

## Generate Project Files

Open the CMake GUI and set the project directory to point to the LibSourcey root directory. Execute "Configure" to do the initial configuration, then adjust any options, then press "Configure" again and then press "Generate".

[See here](/installation.md#cmake-build-options) for a complete list of build options.

## Compile with Visual Studio

1. Generate solutions using CMake, as described above. Make sure, you chose the proper generator (32-bit or 64-bit)
2. Launch Visual Studio, locate and open the `libsourcey.sln` solution file in your generated build folder (eg: `C:\LibSourcey\build\libsourcey.sln`). Select "Debug" configuration, build the solution (Ctrl-Shift-B), and/or select "Release" and build it.
3. Add `{CMAKE_BINARY_DIR}\bin\Release`, `{CMAKE_BINARY_DIR}\bin\Debug` (containing `libscy*.dll` and `libscy*d.dll`, respectively) to the system path (My Computer--[Right button click]->Properties->Advanced->Environment Variables->Path)
