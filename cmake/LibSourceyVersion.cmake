# ----------------------------------------------------------------------------
# Define the current LibSourcey version number
# ----------------------------------------------------------------------------
set(LibSourcey_VERSION_FILE "${LibSourcey_DIR}/VERSION")
if(NOT EXISTS "${LibSourcey_VERSION_FILE}")
  message(FATAL_ERROR "Cannot find VERSION file.")
endif()

file(STRINGS "${LibSourcey_VERSION_FILE}" LibSourcey_VERSION)

string(REPLACE "." ";" VERSION_LIST ${LibSourcey_VERSION})
list(GET VERSION_LIST 0 LibSourcey_VERSION_MAJOR)
list(GET VERSION_LIST 1 LibSourcey_VERSION_MINOR)
list(GET VERSION_LIST 2 LibSourcey_VERSION_PATCH)

if(WIN32)
  # Postfix of DLLs:
  set(LibSourcey_DLLVERSION "${LibSourcey_VERSION_MAJOR}${LibSourcey_VERSION_MINOR}${LibSourcey_VERSION_PATCH}")
  set(LibSourcey_DEBUG_POSTFIX "d")
else()
  # Postfix of so's:
	set(LibSourcey_SOVERSION "${LibSourcey_VERSION_MAJOR}.${LibSourcey_VERSION_MINOR}")
  set(LibSourcey_DEBUG_POSTFIX "")
endif()
