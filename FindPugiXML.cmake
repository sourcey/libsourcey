########################################################################
# Cmake module for finding PugiXML
#
# The following variabled will be defined:
#
#  PugiXML
#  PugiXML_INCLUDE_DIR
#  PugiXML_LIBRARY
#


# ----------------------------------------------------------------------
# Find PugiXML include path
# ----------------------------------------------------------------------
find_path(PugiXML_INCLUDE_DIR pugixml.hpp
  ${LibSourcey_3RDPARTY_SOURCE_DIR}/pugixml/src
  /usr/local/include
  /usr/include
)


# ----------------------------------------------------------------------
# Find PugiXML library
# ----------------------------------------------------------------------
FIND_LIBRARY(PugiXML_LIBRARY 
  NAMES 
    pugixml.lib pugixmld.lib
  PATHS 
    ${LibSourcey_3RDPARTY_INSTALL_DIR}/lib
    /usr/lib 
    /usr/local/lib
  )

if(PugiXML_LIBRARY AND PugiXML_INCLUDE_DIR)
  set(PugiXML_FOUND 1)
else()
  set(PugiXML_FOUND 0)
endif()

#get_filename_component(PugiXML_LIBRARY_DIR ${PugiXML_LIBRARY} PATH)
#get_filename_component(PugiXML_LIBRARY ${PugiXML_LIBRARY} NAME)
  
      
# ----------------------------------------------------------------------
# Display status
# ----------------------------------------------------------------------
if(PugiXML_FOUND)
   if(NOT PugiXML_FIND_QUIETLY)
      message(STATUS "Found PugiXML: \n\tINC DIR: ${PugiXML_INCLUDE_DIR} \n\tLIB DIR: ${PugiXML_LIBRARY_DIR} \n\tLIBS: ${PugiXML_LIBRARY}")
   endif()
else()
   if(PugiXML_FIND_REQUIRED)
      message(FATAL_ERROR "PugiXML was not found.")
   endif()
endif()

#message(STATUS "LibSourcey_3RDPARTY_BUILD_DIR: ${LibSourcey_3RDPARTY_BUILD_DIR}")
#message(STATUS "LibSourcey_3RDPARTY_SOURCE_DIR: ${LibSourcey_3RDPARTY_SOURCE_DIR}")
#message(STATUS "PugiXML_INCLUDE_DIRS: ${PugiXML_INCLUDE_DIRS}")
#message(STATUS "PugiXML_LIBRARY: ${PugiXML_LIBRARY}")
 

# ----------------------------------------------------------------------
# Include and expose to LibSourcey
# ----------------------------------------------------------------------
if(PugiXML_FOUND)
  include_directories(${PugiXML_INCLUDE_DIR})  
  #link_directories(${PugiXML_LIBRARY_DIR})
                        
  set(LibSourcey_INCLUDE_DIRS ${LibSourcey_INCLUDE_DIRS} ${PugiXML_INCLUDE_DIR})
  #set(LibSourcey_LIBRARY_DIRS ${LibSourcey_LIBRARY_DIRS} ${PugiXML_LIBRARY_DIR})
  set(LibSourcey_DEBUG_LIBS   ${LibSourcey_DEBUG_LIBS}   ${PugiXML_LIBRARY})    
  set(LibSourcey_RELEASE_LIBS ${LibSourcey_RELEASE_LIBS} ${PugiXML_LIBRARY})
endif()