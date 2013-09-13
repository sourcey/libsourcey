include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include/uv-private")

# Include source files:
set(libuv_SOURCE_FILES 
  src/fs-poll.c
  src/inet.c
  src/queue.h
  src/uv-common.c
  src/uv-common.h
  src/version.c)

set(libuv_HEADER_FILES 
  include/uv.h
  include/uv-private/tree.h)

if (WIN32)
  set(libuv_SOURCE_FILES ${libuv_SOURCE_FILES}
    src/win/async.c
    src/win/atomicops-inl.h
    src/win/core.c
    src/win/dl.c
    src/win/error.c
    src/win/fs.c
    src/win/fs-event.c
    src/win/getaddrinfo.c
    src/win/handle.c
    src/win/loop-watcher.c
    src/win/pipe.c
    src/win/thread.c
    src/win/poll.c
    src/win/process.c
    src/win/process-stdio.c
    src/win/req.c
    src/win/signal.c
    src/win/stream.c
    src/win/tcp.c
    src/win/tty.c
    src/win/threadpool.c
    src/win/timer.c
    src/win/udp.c
    src/win/util.c
    src/win/winapi.c
    src/win/winsock.c)

  set(libuv_HEADER_FILES ${libuv_HEADER_FILES}  
    include/uv-private/uv-win.h
    src/win/internal.h
    src/win/handle-inl.h
    src/win/req-inl.h
    src/win/stream-inl.h
    src/win/winapi.h
    src/win/winsock.h)

else() # POSIX
  set(libuv_SOURCE_FILES ${libuv_SOURCE_FILES}
    src/unix/async.c    
    src/unix/core.c    
    src/unix/dl.c    
    src/unix/error.c    
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

  set(libuv_HEADER_FILES ${libuv_HEADER_FILES}  
    include/uv-private/uv-unix.h
    include/uv-private/uv-linux.h
    include/uv-private/uv-sunos.h
    include/uv-private/uv-darwin.h
    include/uv-private/uv-bsd.h)
endif()


if(MSVC)
  add_definitions(-D_WIN32_WINNT=0x0600 -D_GNU_SOURCE)
endif()

set(LibUV_DEPENDENCIES advapi32.lib iphlpapi.lib psapi.lib ws2_32.lib shell32.lib)
#set(CMAKE_CXX_STANDARD_LIBRARIES ${CMAKE_CXX_STANDARD_LIBRARIES} advapi32.lib iphlpapi.lib psapi.lib ws2_32.lib shell32.lib)  

define_sourcey_dependency(libuv)