# CMake module to build libuv.  Based heavily off of libuv's Makefile.am, but
# using CMake paradigms.


# Set minimum CMake version
# NOTE: Currently determined by use of target_compile_options
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)


# Silence CMake warnings by adopting modern behavior for MACOSX_RPATH on newer
# versions of CMake
IF(POLICY CMP0042)
    CMAKE_POLICY(SET CMP0042 NEW)
ENDIF()


# Create the project
PROJECT(libuv)


# Set the version
SET(LIBUV_VERSION 1.8.0)


# Set up options
OPTION(BUILD_SHARED_LIBS "Build shared libraries (use OFF for static)" ON)
OPTION(BUILD_TESTS "Enable test building" ON)
SET(BIN_INSTALL_DIR bin CACHE PATH "Install prefix subdirectory for binaries")
SET(LIB_INSTALL_DIR lib CACHE PATH "Install prefix subdirectory for libraries")
SET(INCLUDE_INSTALL_DIR include CACHE PATH
    "Install prefix subdirectory for headers")
OPTION(ENABLE_SOVERSION "Enable shared library versioning and symlinks" OFF)


# Create configuration containers
SET(UV_FLAGS) # Compiler flags
SET(UV_DEFINITIONS) # Compiler definitions
SET(UV_INCLUDE_PATHS) # Include directories
SET(UV_SOURCES) # Source files
SET(UV_DEPENDENCIES) # Library dependencies
SET(UV_INSTALL_HEADERS) # Headers to install


# Utility function which will only add compiler flags if supported
INCLUDE(CheckCCompilerFlag)
FUNCTION(ADD_FLAG_IF_SUPPORTED container flag)
    CHECK_C_COMPILER_FLAG(${flag} _flag_supported)
    IF(_flag_supported)
        # NOTE: Can't use LIST(APPEND...) here because it doesn't support
        # PARENT_SCOPE:
        #   http://stackoverflow.com/a/10032337
        SET(${container} ${${container}} ${flag} PARENT_SCOPE)
    ENDIF()
ENDFUNCTION()


# Common configuration
IF(BUILD_SHARED_LIBS)
    LIST(APPEND UV_DEFINITIONS
        BUILDING_UV_SHARED
    )
ENDIF()
LIST(APPEND UV_INCLUDE_PATHS
    include
    src)
LIST(APPEND UV_SOURCES
    src/fs-poll.c
    src/inet.c
    src/threadpool.c
    src/uv-common.c
    src/version.c
)
LIST(APPEND UV_INSTALL_HEADERS
    include/uv.h
    include/uv-errno.h
    include/uv-threadpool.h
    include/uv-version.h
)


# Handle system-specific flags/sources
IF(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    LIST(APPEND UV_DEFINITIONS
        WIN32_LEAN_AND_MEAN
        _WIN32_WINNT=0x0600
    )
    LIST(APPEND UV_INCLUDE_PATHS
        src/win
    )
    LIST(APPEND UV_SOURCES
        src/win/async.c
        src/win/core.c
        src/win/dl.c
        src/win/error.c
        src/win/fs-event.c
        src/win/fs.c
        src/win/getaddrinfo.c
        src/win/getnameinfo.c
        src/win/handle.c
        src/win/loop-watcher.c
        src/win/pipe.c
        src/win/poll.c
        src/win/process-stdio.c
        src/win/process.c
        src/win/req.c
        src/win/signal.c
        src/win/stream.c
        src/win/tcp.c
        src/win/thread.c
        src/win/timer.c
        src/win/tty.c
        src/win/udp.c
        src/win/util.c
        src/win/winapi.c
        src/win/winsock.c
    )
    LIST(APPEND UV_DEPENDENCIES
        ws2_32
        psapi
        iphlpapi
        shell32
        userenv
    )
    LIST(APPEND UV_INSTALL_HEADERS
        include/uv-win.h
        include/tree.h
    )
    IF((MSVC AND MSVC_VERSION VERSION_LESS 1600) OR MINGW)
        LIST(APPEND UV_INSTALL_HEADERS
            include/stdint-msvc2008.h
        )
    ENDIF()
ELSE()
    # These flags are geared to match the libuv autotools flags, but -g is
    # excluded since it is better-controlled by CMAKE_BUILD_TYPE
    ADD_FLAG_IF_SUPPORTED(UV_FLAGS -fvisibility=hidden)
    ADD_FLAG_IF_SUPPORTED(UV_FLAGS -std=gnu89)
    ADD_FLAG_IF_SUPPORTED(UV_FLAGS -pedantic)
    ADD_FLAG_IF_SUPPORTED(UV_FLAGS -Wall)
    ADD_FLAG_IF_SUPPORTED(UV_FLAGS -Wextra)
    ADD_FLAG_IF_SUPPORTED(UV_FLAGS -Wno-unused-parameter)
    LIST(APPEND UV_DEFINITIONS
        _LARGEFILE_SOURCE
        _FILE_OFFSET_BITS=64
    )
    LIST(APPEND UV_INCLUDE_PATHS
        src/unix
    )
    LIST(APPEND UV_SOURCES
        src/unix/async.c
        src/unix/core.c
        src/unix/dl.c
        src/unix/fs.c
        src/unix/getaddrinfo.c
        src/unix/getnameinfo.c
        src/unix/loop-watcher.c
        src/unix/loop.c
        src/unix/pipe.c
        src/unix/poll.c
        src/unix/process.c
        src/unix/signal.c
        src/unix/stream.c
        src/unix/tcp.c
        src/unix/thread.c
        src/unix/timer.c
        src/unix/tty.c
        src/unix/udp.c
    )
    FIND_PACKAGE(Threads REQUIRED)
    LIST(APPEND UV_DEPENDENCIES
        ${CMAKE_THREAD_LIBS_INIT}
        dl
    )
    LIST(APPEND UV_INSTALL_HEADERS
        include/uv-unix.h
    )
    IF(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        LIST(APPEND UV_DEFINITIONS
            _DARWIN_USE_64_BIT_INODE=1
            _DARWIN_UNLIMITED_SELECT=1
        )
        LIST(APPEND UV_SOURCES
            src/unix/darwin.c
            src/unix/darwin-proctitle.c
            src/unix/fsevents.c
            src/unix/kqueue.c
            src/unix/proctitle.c
        )
        LIST(APPEND UV_INSTALL_HEADERS
            include/uv-darwin.h
        )
    ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        LIST(APPEND UV_DEFINITIONS
            _GNU_SOURCE
        )
        LIST(APPEND UV_SOURCES
            src/unix/linux-core.c
            src/unix/linux-inotify.c
            src/unix/linux-syscalls.c
            src/unix/proctitle.c
        )
        LIST(APPEND UV_INSTALL_HEADERS
            include/uv-linux.h
        )
    ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "DragonFly")
        LIST(APPEND UV_INSTALL_HEADERS
            include/uv-bsd.h
        )
    ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
        LIST(APPEND UV_SOURCES
            src/unix/kqueue.c
            src/unix/freebsd.c
        )
        LIST(APPEND UV_INSTALL_HEADERS
            include/uv-bsd.h
        )
    ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "NetBSD")
        LIST(APPEND UV_SOURCES
            src/unix/kqueue.c
            src/unix/netbsd.c
        )
        LIST(APPEND UV_INSTALL_HEADERS
            include/uv-bsd.h
        )
    ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "OpenBSD")
        LIST(APPEND UV_SOURCES
            src/unix/kqueue.c
            src/unix/openbsd.c
        )
        LIST(APPEND UV_INSTALL_HEADERS
            include/uv-bsd.h
        )
    ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "AIX")
        LIST(APPEND UV_DEFINITIONS
            _ALL_SOURCE
            _XOPEN_SOURCE=500
            _LINUX_SOURCE_COMPAT
        )
        LIST(APPEND UV_SOURCES
            src/unix/aix.c
        )
    ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "SunOS")
        ADD_FLAG_IF_SUPPORTED(UV_FLAGS -pthread)
        LIST(APPEND UV_DEFINITIONS
            __EXTENSIONS__
            _XOPEN_SOURCE=500
        )
        LIST(APPEND UV_SOURCES
            src/unix/sunos.c
        )
        LIST(APPEND UV_INSTALL_HEADERS
            include/uv-sunos.h
        )
    ELSEIF(DEFINED ANDROID_NDK)
        LIST(APPEND UV_SOURCES
            src/unix/android-ifaddrs.c
            src/unix/pthread-fixes.c
        )
        LIST(APPEND UV_INSTALL_HEADERS
            include/android-ifaddrs.h
            include/pthread-fixes.h
        )
    ENDIF()
ENDIF()


# Create the library
ADD_LIBRARY(libuv ${UV_SOURCES})
TARGET_COMPILE_DEFINITIONS(libuv PRIVATE ${UV_DEFINITIONS})
TARGET_COMPILE_OPTIONS(libuv PRIVATE ${UV_FLAGS})
TARGET_INCLUDE_DIRECTORIES(libuv PRIVATE ${UV_INCLUDE_PATHS})
TARGET_LINK_LIBRARIES(libuv PRIVATE ${UV_DEPENDENCIES})
IF(ENABLE_SOVERSION)
    SET_TARGET_PROPERTIES(libuv PROPERTIES SOVERSION ${LIBUV_VERSION})
ENDIF()


# Install commands
INSTALL(TARGETS libuv
    RUNTIME DESTINATION "${BIN_INSTALL_DIR}"
    LIBRARY DESTINATION "${LIB_INSTALL_DIR}"
    ARCHIVE DESTINATION "${LIB_INSTALL_DIR}"
)
INSTALL(FILES ${UV_INSTALL_HEADERS} DESTINATION "${INCLUDE_INSTALL_DIR}")


# Create tests if requested
IF(BUILD_TESTS)
    # Enable testing
    ENABLE_TESTING()

    # Set up sources
    SET(TEST_SOURCES
        test/blackhole-server.c
        test/dns-server.c
        test/echo-server.c
        test/run-tests.c
        test/runner.c
        test/test-active.c
        test/test-async.c
        test/test-async-null-cb.c
        test/test-barrier.c
        test/test-callback-order.c
        test/test-callback-stack.c
        test/test-close-fd.c
        test/test-close-order.c
        test/test-condvar.c
        test/test-connection-fail.c
        test/test-cwd-and-chdir.c
        test/test-default-loop-close.c
        test/test-delayed-accept.c
        test/test-dlerror.c
        test/test-embed.c
        test/test-emfile.c
        test/test-error.c
        test/test-fail-always.c
        test/test-fs-event.c
        test/test-fs-poll.c
        test/test-fs.c
        test/test-get-currentexe.c
        test/test-get-loadavg.c
        test/test-get-memory.c
        test/test-getaddrinfo.c
        test/test-getnameinfo.c
        test/test-getsockname.c
        test/test-handle-fileno.c
        test/test-homedir.c
        test/test-hrtime.c
        test/test-idle.c
        test/test-ip4-addr.c
        test/test-ip6-addr.c
        test/test-ipc-send-recv.c
        test/test-ipc.c
        test/test-loop-handles.c
        test/test-loop-alive.c
        test/test-loop-close.c
        test/test-loop-stop.c
        test/test-loop-time.c
        test/test-loop-configure.c
        test/test-multiple-listen.c
        test/test-mutexes.c
        test/test-osx-select.c
        test/test-pass-always.c
        test/test-ping-pong.c
        test/test-pipe-bind-error.c
        test/test-pipe-connect-error.c
        test/test-pipe-connect-multiple.c
        test/test-pipe-connect-prepare.c
        test/test-pipe-getsockname.c
        test/test-pipe-pending-instances.c
        test/test-pipe-sendmsg.c
        test/test-pipe-server-close.c
        test/test-pipe-close-stdout-read-stdin.c
        test/test-pipe-set-non-blocking.c
        test/test-platform-output.c
        test/test-poll-close.c
        test/test-poll-close-doesnt-corrupt-stack.c
        test/test-poll-closesocket.c
        test/test-poll.c
        test/test-process-title.c
        test/test-ref.c
        test/test-run-nowait.c
        test/test-run-once.c
        test/test-semaphore.c
        test/test-shutdown-close.c
        test/test-shutdown-eof.c
        test/test-shutdown-twice.c
        test/test-signal-multiple-loops.c
        test/test-signal.c
        test/test-socket-buffer-size.c
        test/test-spawn.c
        test/test-stdio-over-pipes.c
        test/test-tcp-bind-error.c
        test/test-tcp-bind6-error.c
        test/test-tcp-close-accept.c
        test/test-tcp-close-while-connecting.c
        test/test-tcp-close.c
        test/test-tcp-create-socket-early.c
        test/test-tcp-connect-error-after-write.c
        test/test-tcp-connect-error.c
        test/test-tcp-connect-timeout.c
        test/test-tcp-connect6-error.c
        test/test-tcp-flags.c
        test/test-tcp-open.c
        test/test-tcp-read-stop.c
        test/test-tcp-shutdown-after-write.c
        test/test-tcp-unexpected-read.c
        test/test-tcp-oob.c
        test/test-tcp-write-to-half-open-connection.c
        test/test-tcp-write-after-connect.c
        test/test-tcp-writealot.c
        test/test-tcp-write-fail.c
        test/test-tcp-try-write.c
        test/test-tcp-write-queue-order.c
        test/test-thread-equal.c
        test/test-thread.c
        test/test-threadpool-cancel.c
        test/test-threadpool.c
        test/test-timer-again.c
        test/test-timer-from-check.c
        test/test-timer.c
        test/test-tty.c
        test/test-udp-bind.c
        test/test-udp-create-socket-early.c
        test/test-udp-dgram-too-big.c
        test/test-udp-ipv6.c
        test/test-udp-multicast-interface.c
        test/test-udp-multicast-interface6.c
        test/test-udp-multicast-join.c
        test/test-udp-multicast-join6.c
        test/test-udp-multicast-ttl.c
        test/test-udp-open.c
        test/test-udp-options.c
        test/test-udp-send-and-recv.c
        test/test-udp-send-immediate.c
        test/test-udp-send-unreachable.c
        test/test-udp-try-send.c
        test/test-walk-handles.c
        test/test-watcher-cross-stop.c
    )
    IF(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        LIST(APPEND TEST_SOURCES
            test/runner-win.c
        )
    ELSE()
        LIST(APPEND TEST_SOURCES
            test/runner-unix.c
        )
    ENDIF()

    # Create the test runner
    ADD_EXECUTABLE(run-tests ${TEST_SOURCES})
    TARGET_COMPILE_DEFINITIONS(run-tests PRIVATE ${UV_DEFINITIONS})
    TARGET_COMPILE_OPTIONS(run-tests PRIVATE ${UV_FLAGS})
    TARGET_INCLUDE_DIRECTORIES(run-tests PRIVATE ${UV_INCLUDE_PATHS})
    # NOTE: Some of the tests, specifically on Windows, use platform functions
    # which require dependencies beyond just libuv, so we have to link to them
    # as well.  In general, programs and libraries using libuv ONLY need to link
    # against libuv, not these other dependencies.  The tests just happen to
    # have dependencies byeond libuv.
    TARGET_LINK_LIBRARIES(run-tests PRIVATE uv ${UV_DEPENDENCIES})

    # HACK: Some of the test runner's tests use internal libuv functions which
    # are not exported or visible through headers.  When using a static library,
    # the test runner can declare prototypes for these functions and when it
    # links everything will be happy.  This approach doesn't work for shared
    # libraries though (at least on Windows) because the symbols won't be found
    # in the .lib/.dll, even if their code is there, unless they are explicitly
    # exported.  So adding this definition in the case of shared libraries
    # causes those tests to be skipped.  Seems like maybe the libuv authors will
    # fix this in the future.  It doesn't seem like this is necessary on POSIX
    # systems, since symbols are exported by default, but libuv uses it on all
    # platforms where shared libraries are being used, so let's follow their
    # lead.
    IF(BUILD_SHARED_LIBS)
        TARGET_COMPILE_DEFINITIONS(run-tests PRIVATE USING_UV_SHARED=1)
    ENDIF()

    # Copy test fixtures when building tester
    ADD_CUSTOM_COMMAND(TARGET run-tests PRE_BUILD
        COMMAND
        "${CMAKE_COMMAND}" -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/test/fixtures"
        "${CMAKE_CURRENT_BINARY_DIR}/test/fixtures"
    )

    # Add a check target
    ADD_TEST(tests run-tests)
ENDIF()
