include_directories("${LibSourcey_VENDOR_BUILD_DIR}/zlib") # for zconf.h
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../zlib") # for zlib.h

# Include source files:
set(minizip_SOURCE_FILES
  ioapi.c
  mztools.c
  unzip.c
  zip.c)

set(minizip_HEADER_FILES
  crypt.h
  ioapi.h
  mztools.h
  unzip.h
  zip.h)

if(WIN32)
  # iowin32.h iowin32.c
  set(minizip_SOURCE_FILES ${minizip_SOURCE_FILES} iowin32.c)
endif()

set(minizip_DEPENDENCIES zlib)

define_sourcey_dependency(minizip)
