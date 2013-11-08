# https://c9.io/creationix/moonslice-luv/workspace/luv/libuv/CMakeLists.txt

include_directories(src include include/uv-private)

# Common source files
set(libuv_COMMON_SOURCE_FILES 
  include/uv.h
  include/uv-private/tree.h
  src/fs-poll.c
  src/inet.c
  src/queue.h
  src/uv-common.c
  src/uv-common.h
  src/version.c)

if(MSVC)
  add_definitions(-D_WIN32_WINNT=0x0600 -D_GNU_SOURCE)
  set(libuv_DEPENDENCIES advapi32.lib iphlpapi.lib psapi.lib shell32.lib ws2_32.lib)

  # Windows source files
  set(libuv_SOURCE_FILES ${libuv_COMMON_SOURCE_FILES} 
    include/uv-private/uv-win.h
    src/win/async.c
    src/win/atomicops-inl.h
    src/win/core.c
    src/win/dl.c
    src/win/error.c
    src/win/fs.c
    src/win/fs-event.c
    src/win/getaddrinfo.c
    src/win/handle.c
    src/win/handle-inl.h
    src/win/internal.h
    src/win/loop-watcher.c
    src/win/pipe.c
    src/win/thread.c
    src/win/poll.c
    src/win/process.c
    src/win/process-stdio.c
    src/win/req.c
    src/win/req-inl.h
    src/win/signal.c
    src/win/stream.c
    src/win/stream-inl.h
    src/win/tcp.c
    src/win/tty.c
    src/win/threadpool.c
    src/win/timer.c
    src/win/udp.c
    src/win/util.c
    src/win/winapi.c
    src/win/winapi.h
    src/win/winsock.c
    src/win/winsock.h)

else()
  add_definitions(-D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -D_GNU_SOURCE)
  set(libuv_DEPENDENCIES "-lm")

  # Posix source files
  set(libuv_POSIX_SOURCE_FILES    
    include/uv-private/uv-unix.h
    include/uv-private/uv-linux.h
    include/uv-private/uv-sunos.h
    include/uv-private/uv-darwin.h
    include/uv-private/uv-bsd.h
    src/unix/async.c    
    src/unix/core.c    
    src/unix/dl.c     
    src/unix/fs.c    
    src/unix/getaddrinfo.c    
    src/unix/internal.h    
    src/unix/loop.c    
    src/unix/loop-watcher.c    
    src/unix/pipe.c    
    src/unix/poll.c    
    src/unix/process.c    
    src/unix/signal.c    
    src/unix/stream.c    
    src/unix/tcp.c    
    src/unix/thread.c    
    src/unix/threadpool.c    
    src/unix/timer.c    
    src/unix/tty.c    
    src/unix/udp.c)

  # For linux mac and android; see uv.gyp
  set(libuv_PROCTILE_SOURCE_FILE src/unix/proctitle.c)   

  if(APPLE)
    add_definitions(-D_DARWIN_USE_64_BIT_INODE=1)
    set(libuv_SOURCE_FILES 
      ${libuv_COMMON_SOURCE_FILES}      
      ${libuv_POSIX_SOURCE_FILES}    
      ${libuv_PROCTILE_SOURCE_FILE} 
      src/unix/darwin.c
      src/unix/fsevents.c
      src/unix/darwin-proctitle.c)

  elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(libuv_DEPENDENCIES "-ldl -lrt ${libuv_DEPENDENCIES}")
    set(libuv_SOURCE_FILES 
      ${libuv_COMMON_SOURCE_FILES}      
      ${libuv_POSIX_SOURCE_FILES} 
      ${libuv_PROCTILE_SOURCE_FILE} 
      src/unix/linux-core.c
      src/unix/linux-inotify.c
      src/unix/linux-syscalls.c      
      src/unix/linux-syscalls.h)

  elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Android")    
    set(libuv_DEPENDENCIES "-ldl ${libuv_DEPENDENCIES}")
    set(libuv_SOURCE_FILES 
      ${libuv_COMMON_SOURCE_FILES}      
      ${libuv_POSIX_SOURCE_FILES} 
      ${libuv_PROCTILE_SOURCE_FILE} 
      src/unix/linux-core.c
      src/unix/linux-inotify.c
      src/unix/linux-syscalls.c      
      src/unix/linux-syscalls.h
      src/unix/pthread-fixes.c)
  else()
    message(FATAL_ERROR
      "Cannot build libuv on this system: ${CMAKE_SYSTEM_NAME}")
  endif()

  # TODO: Solaris, FreeBSD and others; see uv.gyp
 
endif()

# Include LibUV dependencies in LibSourcey build
set(LibSourcey_INCLUDE_LIBRARIES ${libuv_DEPENDENCIES} ${LibSourcey_INCLUDE_LIBRARIES} CACHE INTERNAL "") 

# Define the project
define_sourcey_dependency(libuv)
