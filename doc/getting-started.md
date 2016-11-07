# Getting Started

First and foremost, welcome to LibSourcey — it's great to have you along for the ride! We've done our best to make the docs as clear as possible to save you time, so if something's missing or changed, or newer versions are available then please let us know or submit a [Push Request](/README.md#contributing) with your updates.

Before going any further please follow the [installation guide](/installation.md) to compiled and install LibSourcey on your system.

Once LibSourcey is installed you a number of choices how you include the libraries in your own project:

### 1) Using `pkg-config` (Linux only)

Using `pkg-config` is the simplest and easiest method to include LibSourcey libraries and headers in your project. After running `make install` the `pkg-config` script is located in `/usr/local/lib/pkgconfig/libsourcey.pc` by default.

Using the default structure you can include Libsourcey in your project like so (using `gcc`):

~~~ bash
gcc -o test test.cpp $(pkg-config --libs --cflags libsourcey)
~~~

### 2) Using CMake

If you're already using CMake in your own project then including LibSourcey with CMake is highly recommended. Once you have built and installed LibSourcey you can include all the libraries and headers by adding the following lines to your `CMakeLists.txt`:

~~~
# The LibSourcey root directory must be set
set(LibSourcey_ROOT_DIR "" CACHE STRING "Where is the LibSourcey root directory located?")
find_package_handle_standard_args(${module} DEFAULT_MSG LibSourcey_ROOT_DIR)

# Tell CMake where to locate the LibSourcey .cmake files
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${LibSourcey_ROOT_DIR}/cmake)
set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} ${LibSourcey_ROOT_DIR}/cmake)

# Include LibSourcey with specified modules (example: uv base net crypto http)
find_package(LibSourcey COMPONENTS uv base net crypto http REQUIRED)
~~~

### 3) Extending the build system

The third option is to include the LibSourcey CMake build system in your own project. When you choose this option you will be building LibSourcey modules along side your own project as dependencies, and will also inherit the LibSorucey build system (which is pretty awesome). [See here](/installation.md#cmake-build-options) for a complete list of build options.

This method is only recommended for advanced users and if you want your project to be tightly coupled with LibSourcey.

Below is an example of a full `CMakeLists.txt` file that you can use to include LibSourcey in your build tree:

~~~
cmake_minimum_required(VERSION 2.8.10)
project(exampleapp)

# Set the source directory and search locations as required for your project
set(sourcedir src)
set(sourcefiles ${sourcedir}/*.cpp)
set(headerfiles ${sourcedir}/*.h)

# Include LibSourcey
include("../libsourcey/LibSourcey.cmake")
include_directories(${Libourcey_INCLUDE_DIRS})

link_libraries(${LibSourcey_LIBRARIES})
include_directories(${sourcedir} ${LibSourcey_INCLUDE_DIRS})

# Glob your sources and headers
# Be sure to modify the search paths according to your project structure
file(GLOB_RECURSE sources ${sourcefiles})
file(GLOB_RECURSE headers ${headerfiles})

add_executable(exampleapp ${sources} ${headers})
install(TARGETS exampleapp)
~~~

## Next Steps

Once LibSourcey is included in your project you're ready to start using the API. Check out the [code examples](/examples.md), and [API reference](/api-base.md) to start cutting code.
