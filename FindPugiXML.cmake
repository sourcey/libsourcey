########################################################################
# Cmake module for finding PugiXML
#
# The following variabled will be defined:
#
#  PugiXML_FOUND
#  PugiXML_INCLUDE_DIR
#  PugiXML_LIBRARY
#


# ----------------------------------------------------------------------
# Find PugiXML include path
# ----------------------------------------------------------------------
find_path(PugiXML_INCLUDE_DIR pugixml.hpp
  ${LibSourcey_DEPENDENCIES_SOURCE_DIR}/pugixml/src
  /usr/local/include
  /usr/include
)


# ----------------------------------------------------------------------
# Find PugiXML library
# ----------------------------------------------------------------------
if(WIN32 AND MSVC)
  
  find_library(PugiXML_DEBUG_LIBRARY 
    NAMES 
      pugixmld
    PATHS 
      ${LibSourcey_DEPENDENCIES_INSTALL_DIR}/lib
      /usr/lib 
      /usr/local/lib
    )
    
  find_library(PugiXML_RELEASE_LIBRARY 
    NAMES 
      pugixml
    PATHS 
      ${LibSourcey_DEPENDENCIES_INSTALL_DIR}/lib
      /usr/lib 
      /usr/local/lib
    )
    
  if(PugiXML_DEBUG_LIBRARY OR PugiXML_RELEASE_LIBRARY)  
    if(CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)            
      if (PugiXML_RELEASE_LIBRARY) 
        list(APPEND PugiXML_LIBRARY "optimized" ${PugiXML_RELEASE_LIBRARY})
      endif()
      if (PugiXML_DEBUG_LIBRARY)
        list(APPEND PugiXML_LIBRARY "debug" ${PugiXML_DEBUG_LIBRARY})
      endif()
    else()    
      if (PugiXML_RELEASE_LIBRARY) 
        list(APPEND PugiXML_LIBRARY ${PugiXML_RELEASE_LIBRARY})
      elseif (PugiXML_DEBUG_LIBRARY) 
        list(APPEND PugiXML_LIBRARY ${PugiXML_DEBUG_LIBRARY})
      endif()
    endif()
    mark_as_advanced(PugiXML_DEBUG_LIBRARY PugiXML_RELEASE_LIBRARY)
  endif()

else()

  # TODO: PugiXML lib names for various systems
  set(PugiXML_LIB_NAMES "pugixml")
  
  find_library(PugiXML_LIBRARY 
    NAMES 
      ${PugiXML_LIB_NAMES}
    PATHS 
      ${LibSourcey_DEPENDENCIES_INSTALL_DIR}/lib
      /usr/lib 
      /usr/local/lib
    )

endif()

if(PugiXML_LIBRARY AND PugiXML_INCLUDE_DIR)
  set(PugiXML_FOUND 1)
  mark_as_advanced(PugiXML_LIBRARY PugiXML_INCLUDE_DIR)
else()
  set(PugiXML_FOUND 0)
endif()
  
      
# ----------------------------------------------------------------------
# Display status
# ----------------------------------------------------------------------
if(NOT PugiXML_FOUND)
   if(PugiXML_FIND_REQUIRED)
      message(FATAL_ERROR "PugiXML was not found. Please build dependencies first, or specify the path manually.")
   endif()
endif()