
# Include source files:
set(minizip_SOURCE_FILES 
  ioapi.c
  mztools.c
  iowin32.c
  unzip.c
  zip.c)

set(minizip_HEADER_FILES 
  crypt.h
  ioapi.h
  mztools.h
  iowin32.h
  unzip.h
  zip.h)

define_sourcey_dependency(minizip)