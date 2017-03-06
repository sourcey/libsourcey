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

Download and install the 1.0.x branch [Windows OpenSSL binaries](http://slproweb.com/products/Win32OpenSSL.html).
Don't forget to set system path if you download `zip` file version, or if the OpenSSL Installer failed to set the properly:

* `<PATH_TO_OPENSSL_INSTALL_DIR>\bin`

You can set the `OPENSSL_ROOT_DIR` CMake variable to point to your OpenSSL installation directory and the build system will do the rest.


### Install FFmpeg (optional)

Download and install [FFmpeg for Windows](https://ffmpeg.zeranoe.com/builds/).
Notice that you need to download both `Shared` and `Dev` version.
Extract zip files and set them to head of your system's `PATH` (you can just add new item in Windows 10):

* `<PATH_TO_FFMPEG_SHARED_DIR>\bin`

Now you can set the `FFMPEG_ROOT_DIR` CMake variable to point to your FFmpeg dev installation directory and the build system will do the rest.

For example, if you want to build LibSourcey with FFmpeg enabled your CMake command might look something like this:

~~~ bash
cmake .. -DOPENSSL_ROOT_DIR=E:\dev\vendor\OpenSSL-Win64 -DWITH_FFMPEG=ON -DFFMPEG_ROOT_DIR=E:\dev\vendor\ffmpeg-3.2.2-win64-dev
~~~


### Install WebRTC (optional)

The official install guide is here: https://webrtc.org/native-code/development/
Alternatively, follow the guide below to install WebRTC on your Windows system without pain. 

##### Download dependencies

1.  Install dependencies

  Install Visual Studio 2015 Update 2 or later - Community Edition should work if its license is appropriate for you. Use the Custom Install option and select:

    * Visual C++, which will select three sub-categories including MFC
    * Universal Windows Apps Development Tools > Tools
    * Universal Windows Apps Development Tools > Windows 10 SDK (10.0.10586)

2.  Install the Chromium depot tolls

  1. Download [depot_tools.zip](https://storage.googleapis.com/chrome-infra/depot_tools.zip) and decompress it.
  2. Add depot_tools to the end of your PATH:

    * With Administrator access:

      * `Control Panel > System and Security > System > Advanced system settings`
      * Modify the PATH system variable to include depot_tools

    * Without Administrator access:

      * `Control Panel > User Accounts > User Accounts > Change my environment variables`
      * Add a PATH user variable: `%PATH%;C:\path\to\depot_tools`

  3. Run `gclient` from the cmd shell (Run as Administrator). The first time it is run, it will install its own copy of svn and other tools.
  4. Run following commands to set necessary environment variables:
  
    ~~~bash
    set DEPOT_TOOLS_WIN_TOOLCHAIN=0
    ~~~

#### Download the source code

1.  Create a working directory, enter it, and run fetch webrtc:

  ~~~bash
  mkdir webrtc-checkout
  cd webrtc-checkout
  fetch --nohooks webrtc
  ~~~

2.  Optionally you can choose the stable release rather than the most recent release by enter:

  ~~~bash
  cd src
  git branch -r
  git checkout <branch_name>
  ~~~
  Example: `git checkout branch-heads/57`

3.  Download the code
  ~~~bash
  gclient sync
  ~~~

#### Building WebRTC library

1.  After downloading the code, you can start building the WebRTC library (standing in `src/`).

  ~~~bash
  gn gen out/Debug --args="is_debug=true rtc_include_tests=false"
  gn gen out/Release --args="is_debug=false rtc_include_tests=false"
  ~~~
  **Note:** _Please run this command with Python2.x, Python3 is currently not supported._
  
  Then compile with:

  Debug:
  ~~~bash
  ninja -C out/Debug
  ~~~
  Release:
  ~~~bash
  ninja -C out/Release
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
