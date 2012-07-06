########################################################################
# Cmake module for finding LibStrophe
#
# The following variabled will be defined:
#
#  LibStrophe_FOUND
#  LibStrophe_INCLUDE_DIR
#  LibStrophe_LIBRARY
#


# ----------------------------------------------------------------------
# Find LibStrophe include path
# ----------------------------------------------------------------------
find_path(LibStrophe_INCLUDE_DIR strophe.h
  ${LibSourcey_DEPENDENCIES_SOURCE_DIR}/libstrophe
  /usr/local/include
  /usr/include
)


# ----------------------------------------------------------------------
# Find LibStrophe library
# ----------------------------------------------------------------------
if(WIN32 AND MSVC)
  
  find_library(LibStrophe_DEBUG_LIBRARY 
    NAMES 
      libstrophed
    PATHS 
      ${LibSourcey_DEPENDENCIES_BUILD_DIR}/libstrophe
      ${LibSourcey_DEPENDENCIES_INSTALL_DIR}/lib
      /usr/lib 
      /usr/local/lib
    )
    
  find_library(LibStrophe_RELEASE_LIBRARY 
    NAMES 
      libstrophe
    PATHS 
      ${LibSourcey_DEPENDENCIES_BUILD_DIR}/libstrophe
      ${LibSourcey_DEPENDENCIES_INSTALL_DIR}/lib
      /usr/lib 
      /usr/local/lib
    )
    
  if(LibStrophe_DEBUG_LIBRARY OR LibStrophe_RELEASE_LIBRARY)  
    if(CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)            
      if (LibStrophe_RELEASE_LIBRARY) 
        list(APPEND LibStrophe_LIBRARY "optimized" ${LibStrophe_RELEASE_LIBRARY})
      endif()
      if (LibStrophe_DEBUG_LIBRARY)
        list(APPEND LibStrophe_LIBRARY "debug" ${LibStrophe_DEBUG_LIBRARY})
      endif()
    else()    
      if (LibStrophe_RELEASE_LIBRARY) 
        list(APPEND LibStrophe_LIBRARY ${LibStrophe_RELEASE_LIBRARY})
      elseif (LibStrophe_DEBUG_LIBRARY) 
        list(APPEND LibStrophe_LIBRARY ${LibStrophe_DEBUG_LIBRARY})
      endif()
    endif()
    mark_as_advanced(LibStrophe_DEBUG_LIBRARY LibStrophe_RELEASE_LIBRARY)
  endif()

else()

  # TODO: LibStrophe lib names for various systems
  #set(LibStrophe_LIB_NAMES "libstrophe")
  
  find_library(LibStrophe_LIBRARY 
    NAMES 
      strophe #${LibStrophe_LIB_NAMES}
    PATHS 
      ${LibSourcey_DEPENDENCIES_BUILD_DIR}/libstrophe
      ${LibSourcey_DEPENDENCIES_INSTALL_DIR}/lib
      /usr/lib 
      /usr/local/lib
    )

endif()
  
  
#message("LibStrophe_LIBRARY=${LibStrophe_INCLUDE_DIR}")
#message("LibStrophe_SOURCE_DIR=${LibSourcey_DEPENDENCIES_SOURCE_DIR}/libstrophe/src")
#message("LibStrophe_INCLUDE_DIR=${LibStrophe_INCLUDE_DIR}")
  
if(LibStrophe_LIBRARY AND LibStrophe_INCLUDE_DIR)
  set(LibStrophe_FOUND 1)
  mark_as_advanced(LibStrophe_LIBRARY LibStrophe_INCLUDE_DIR)
else()
  set(LibStrophe_FOUND 0)
endif()
  
      
# ----------------------------------------------------------------------
# Display status
# ----------------------------------------------------------------------
if(NOT LibStrophe_FOUND)
   if(LibStrophe_FIND_REQUIRED)
      message(FATAL_ERROR "LibStrophe was not found. Please build dependencies first, or specify the path manually.")
   endif()
endif()
