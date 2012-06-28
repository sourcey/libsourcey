# ----------------------------------------------------------------------------
# Define the current LibSourcey version number
# ----------------------------------------------------------------------------
set(LibSourcey_VERSION_FILE "${CMAKE_SOURCE_DIR}/src/Base/include/Sourcey/Base.h")
if(NOT EXISTS "${LibSourcey_VERSION_FILE}")
  message(FATAL_ERROR "Cannot find base module headers.")
endif()

FILE(STRINGS "${LibSourcey_VERSION_FILE}" LibSourcey_VERSION_PARTS REGEX "#define SOURCEY_.+_VERSION[ ]+[0-9]+" )

string(REGEX REPLACE ".+SOURCEY_MAJOR_VERSION[ ]+([0-9]+).*" "\\1" LibSourcey_VERSION_MAJOR "${LibSourcey_VERSION_PARTS}")
string(REGEX REPLACE ".+SOURCEY_MINOR_VERSION[ ]+([0-9]+).*" "\\1" LibSourcey_VERSION_MINOR "${LibSourcey_VERSION_PARTS}")
string(REGEX REPLACE ".+SOURCEY_PATCH_VERSION[ ]+([0-9]+).*" "\\1" LibSourcey_VERSION_PATCH "${LibSourcey_VERSION_PARTS}")

set(LibSourcey_VERSION "${LibSourcey_VERSION_MAJOR}.${LibSourcey_VERSION_MINOR}.${LibSourcey_VERSION_PATCH}")
set(LibSourcey_SOVERSION "${LibSourcey_VERSION_MAJOR}.${LibSourcey_VERSION_MINOR}")

if(WIN32)
    # Postfix of DLLs:
    set(LibSourcey_DLLVERSION "${LibSourcey_VERSION_MAJOR}${LibSourcey_VERSION_MINOR}${LibSourcey_VERSION_PATCH}")
    set(LibSourcey_DEBUG_POSTFIX "d")
else()
    # Postfix of so's:
    set(LibSourcey_DLLVERSION "")
    set(LibSourcey_DEBUG_POSTFIX)
endif()