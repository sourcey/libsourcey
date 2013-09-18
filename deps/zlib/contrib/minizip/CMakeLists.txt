include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../") # for zlib.h

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
  set(minizip_SOURCE_FILES ${minizip_SOURCE_FILES} iowin32.c)
endif()

# iowin32.h iowin32.c

define_sourcey_dependency(minizip)