########################################################################
# Cmake module for finding HttpParser
#
# The following variabled will be defined:
#
#  HttpParser_FOUND
#  HttpParser_INCLUDE_DIR
#  HttpParser_LIBRARY
#

# ----------------------------------------------------------------------
# Find http_parser include path
# ----------------------------------------------------------------------
FIND_PATH(HttpParser_INCLUDE_DIR     
  NAMES 
  	uv.h   
  PATHS
  	${LibSourcey_DEPENDENCIES_SOURCE_DIR}/http_parser
  	/usr/local/include
  	/usr/include
)

# ----------------------------------------------------------------------
# Find http_parser library
# ----------------------------------------------------------------------
if(WIN32 AND MSVC)
  
  find_library(HttpParser_DEBUG_LIBRARY 
    NAMES 
      http_parserd
    PATHS 
      ${LibSourcey_DEPENDENCIES_BUILD_DIR}/http_parser
      ${LibSourcey_DEPENDENCIES_INSTALL_DIR}/lib
      /usr/lib 
      /usr/local/lib
    )
    
  find_library(HttpParser_RELEASE_LIBRARY 
    NAMES 
      http_parser
    PATHS 
      ${LibSourcey_DEPENDENCIES_BUILD_DIR}/http_parser
      ${LibSourcey_DEPENDENCIES_INSTALL_DIR}/lib
      /usr/lib 
      /usr/local/lib
    )
    
  if(HttpParser_DEBUG_LIBRARY OR HttpParser_RELEASE_LIBRARY)  
    if(CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)            
      if (HttpParser_RELEASE_LIBRARY) 
        list(APPEND HttpParser_LIBRARY "optimized" ${HttpParser_RELEASE_LIBRARY})
      endif()
      if (HttpParser_DEBUG_LIBRARY)
        list(APPEND HttpParser_LIBRARY "debug" ${HttpParser_DEBUG_LIBRARY})
      endif()
    else()    
      if (HttpParser_RELEASE_LIBRARY) 
        list(APPEND HttpParser_LIBRARY ${HttpParser_RELEASE_LIBRARY})
      elseif (HttpParser_DEBUG_LIBRARY) 
        list(APPEND HttpParser_LIBRARY ${HttpParser_DEBUG_LIBRARY})
      endif()
    endif()  
    mark_as_advanced(HttpParser_DEBUG_LIBRARY HttpParser_RELEASE_LIBRARY)
  endif()

else()

  find_library(HttpParser_LIBRARY 
    NAMES 
      http_parser
      libhttp_parser
    PATHS 
       ${LibSourcey_DEPENDENCIES_BUILD_DIR}/http_parser
       ${LibSourcey_DEPENDENCIES_INSTALL_DIR}/lib
       /usr/lib 
       /usr/local/lib
    )

endif()

if(HttpParser_LIBRARY AND HttpParser_INCLUDE_DIR)
  set(HttpParser_FOUND 1)
  mark_as_advanced(HttpParser_LIBRARY HttpParser_INCLUDE_DIR)
else()
  set(HttpParser_FOUND 0)
endif()