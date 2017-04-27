# CMake module to build libuv FOR LiubSourcey.
# Based heavily off of libuv's Makefile.am, but using CMake paradigms.
# Works with libuv 1.11.1 dev

# Create configuration containers
set(libuv_FLAGS) # Compiler flags
set(libuv_DEFINITIONS) # Compiler definitions
set(libuv_INCLUDE_DIRS) # Include directories
set(libuv_SOURCE_FILES) # Source files
set(libuv_HEADER_FILES) # Headers to install
set(libuv_DEPENDENCIES) # Library dependencies

# Utility function which will only add compiler flags if supported
include(CheckCCompilerFlag)
function(add_flag_if_supported container flag)
  CHECK_C_COMPILER_FLAG(${flag} _flag_supported)
  if(_flag_supported)
    # NOTE: Can't use list(APPEND...) here because it doesn't support
    # PARENT_SCOPE:
    #   http://stackoverflow.com/a/10032337
    set(${container} ${${container}} ${flag} PARENT_SCOPE)
  endif()
endfunction()

# Common configuration
if(BUILD_SHARED_LIBS)
  list(APPEND libuv_DEFINITIONS BUILDING_UV_SHARED)
endif()
list(APPEND libuv_INCLUDE_DIRS include src)
list(APPEND libuv_SOURCE_FILES
  src/fs-poll.c
  src/inet.c
  src/queue.h
  src/threadpool.c
  src/uv-common.c
  src/uv-common.h
  src/version.c
)

list(APPEND libuv_HEADER_FILES
  include/uv.h
  include/uv-errno.h
  include/uv-threadpool.h
  include/uv-version.h
)

# Handle system-specific flags/sources
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  list(APPEND libuv_DEFINITIONS
    WIN32_LEAN_AND_MEAN
    _WIN32_WINNT=0x0600
  )
  list(APPEND libuv_INCLUDE_DIRS
    src/win
  )
  list(APPEND libuv_SOURCE_FILES
    src/win/async.c
    src/win/atomicops-inl.h
    src/win/core.c
    src/win/detect-wakeup.c
    src/win/dl.c
    src/win/error.c
    src/win/fs-event.c
    src/win/fs.c
    src/win/getaddrinfo.c
    src/win/getnameinfo.c
    src/win/handle.c
    src/win/handle-inl.h
    src/win/internal.h
    src/win/loop-watcher.c
    src/win/pipe.c
    src/win/poll.c
    src/win/process-stdio.c
    src/win/process.c
    src/win/req.c
    src/win/req-inl.h
    src/win/signal.c
    src/win/stream.c
    src/win/stream-inl.h
    src/win/tcp.c
    src/win/thread.c
    src/win/timer.c
    src/win/tty.c
    src/win/udp.c
    src/win/util.c
    src/win/winapi.c
    src/win/winapi.h
    src/win/winsock.c
    src/win/winsock.h
  )
  list(APPEND libuv_DEPENDENCIES
    ws2_32
    psapi
    iphlpapi
    shell32
    userenv
  )
  list(APPEND libuv_HEADER_FILES
    include/uv-win.h
    include/tree.h
  )
  if((MSVC AND MSVC_VERSION VERSION_LESS 1600) OR MINGW)
    list(APPEND libuv_HEADER_FILES
      include/stdint-msvc2008.h
    )
  endif()

else() # Non-Windows

  # These flags are geared to match the libuv autotools flags, but -g is
  # excluded since it is better-controlled by CMAKE_BUILD_TYPE
  add_flag_if_supported(libuv_FLAGS -fvisibility=hidden)
  add_flag_if_supported(libuv_FLAGS -pedantic)
  add_flag_if_supported(libuv_FLAGS -Wall)
  add_flag_if_supported(libuv_FLAGS -Wextra)
  add_flag_if_supported(libuv_FLAGS -Wno-unused-parameter)
  if(NOT CMAKE_COMPILER_IS_GNUCC)
    add_flag_if_supported(libuv_FLAGS -std=gnu89)
  endif()
  list(APPEND libuv_DEFINITIONS
    _LARGEFILE_SOURCE
    _FILE_OFFSET_BITS=64
  )
  list(APPEND libuv_INCLUDE_DIRS
    src/unix
  )
  list(APPEND libuv_SOURCE_FILES
    src/unix/async.c
    src/unix/atomic-ops.h
    src/unix/core.c
    src/unix/dl.c
    src/unix/fs.c
    src/unix/getaddrinfo.c
    src/unix/getnameinfo.c
    src/unix/internal.h
    src/unix/loop-watcher.c
    src/unix/loop.c
    src/unix/pipe.c
    src/unix/poll.c
    src/unix/process.c
    src/unix/signal.c
    src/unix/spinlock.h
    src/unix/stream.c
    src/unix/tcp.c
    src/unix/thread.c
    src/unix/timer.c
    src/unix/tty.c
    src/unix/udp.c
  )

  find_package(Threads REQUIRED)
  list(APPEND libuv_DEPENDENCIES ${CMAKE_THREAD_LIBS_INIT} dl)
  list(APPEND libuv_HEADER_FILES
    include/uv-unix.h
  )
  if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    list(APPEND libuv_DEFINITIONS
      _DARWIN_USE_64_BIT_INODE=1
      _DARWIN_UNLIMITED_SELECT=1
    )
    list(APPEND libuv_SOURCE_FILES
      src/unix/darwin.c
      src/unix/darwin-proctitle.c
      src/unix/fsevents.c
      src/unix/kqueue.c
      src/unix/proctitle.c
      src/unix/pthread-barrier.c
    )
    list(APPEND libuv_HEADER_FILES
      include/uv-darwin.h
      include/pthread-barrier.h
    )
  elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    list(APPEND libuv_DEFINITIONS
      _GNU_SOURCE
    )
    list(APPEND libuv_SOURCE_FILES
      src/unix/linux-core.c
      src/unix/linux-inotify.c
      src/unix/linux-syscalls.c
      src/unix/proctitle.c
    )
    list(APPEND libuv_HEADER_FILES
      include/uv-linux.h
    )
  elseif(CMAKE_SYSTEM_NAME STREQUAL "DragonFly")
    list(APPEND libuv_HEADER_FILES
      include/uv-bsd.h
    )
  elseif(CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
    list(APPEND libuv_SOURCE_FILES
      src/unix/kqueue.c
      src/unix/freebsd.c
    )
    list(APPEND libuv_HEADER_FILES
      include/uv-bsd.h
    )
  elseif(CMAKE_SYSTEM_NAME STREQUAL "NetBSD")
    list(APPEND libuv_SOURCE_FILES
      src/unix/kqueue.c
      src/unix/netbsd.c
    )
    list(APPEND libuv_HEADER_FILES
      include/uv-bsd.h
    )
  elseif(CMAKE_SYSTEM_NAME STREQUAL "OpenBSD")
    list(APPEND libuv_SOURCE_FILES
      src/unix/kqueue.c
      src/unix/openbsd.c
    )
    list(APPEND libuv_HEADER_FILES
      include/uv-bsd.h
    )
  elseif(CMAKE_SYSTEM_NAME STREQUAL "AIX")
    list(APPEND libuv_DEFINITIONS
      _ALL_SOURCE
      _XOPEN_SOURCE=500
      _LINUX_SOURCE_COMPAT
    )
    list(APPEND libuv_SOURCE_FILES
      src/unix/aix.c
    )
  elseif(CMAKE_SYSTEM_NAME STREQUAL "SunOS")
    add_flag_if_supported(libuv_FLAGS -pthread)
    list(APPEND libuv_DEFINITIONS
      __EXTENSIONS__
      _XOPEN_SOURCE=500
    )
    list(APPEND libuv_SOURCE_FILES
      src/unix/sunos.c
    )
    list(APPEND libuv_HEADER_FILES
      include/uv-sunos.h
    )
  elseif(DEFINED ANDROID_NDK)
    list(APPEND libuv_SOURCE_FILES
      src/unix/android-ifaddrs.c
      src/unix/pthread-fixes.c
      src/unix/pthread-barrier.c
    )
    list(APPEND libuv_HEADER_FILES
      include/android-ifaddrs.h
      include/pthread-fixes.h
    )
  endif()
endif()

define_sourcey_dependency(libuv)
