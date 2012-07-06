########################################################################
# Cmake module for finding JsonCpp
#
# The following variabled will be defined:
#
#  JsonCpp_FOUND
#  JsonCpp_INCLUDE_DIR
#  JsonCpp_LIBRARY
#

# ----------------------------------------------------------------------
# Find JsonCpp include path
# ----------------------------------------------------------------------
FIND_PATH(JsonCpp_INCLUDE_DIR     
  NAMES 
  	json/json.h   
  PATHS
  	${LibSourcey_DEPENDENCIES_SOURCE_DIR}/jsoncpp/include
  	/usr/local/include
  	/usr/include
)

# ----------------------------------------------------------------------
# Find JsonCpp library
# ----------------------------------------------------------------------
if(WIN32 AND MSVC)
  
  find_library(JsonCpp_DEBUG_LIBRARY 
    NAMES 
      jsoncppd
    PATHS 
      ${LibSourcey_DEPENDENCIES_BUILD_DIR}/jsoncpp
      ${LibSourcey_DEPENDENCIES_INSTALL_DIR}/lib
      /usr/lib 
      /usr/local/lib
    )
    
  find_library(JsonCpp_RELEASE_LIBRARY 
    NAMES 
      jsoncpp
    PATHS 
      ${LibSourcey_DEPENDENCIES_BUILD_DIR}/jsoncpp
      ${LibSourcey_DEPENDENCIES_INSTALL_DIR}/lib
      /usr/lib 
      /usr/local/lib
    )
    
  if(JsonCpp_DEBUG_LIBRARY OR JsonCpp_RELEASE_LIBRARY)  
    if(CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)            
      if (JsonCpp_RELEASE_LIBRARY) 
        list(APPEND JsonCpp_LIBRARY "optimized" ${JsonCpp_RELEASE_LIBRARY})
      endif()
      if (JsonCpp_DEBUG_LIBRARY)
        list(APPEND JsonCpp_LIBRARY "debug" ${JsonCpp_DEBUG_LIBRARY})
      endif()
    else()    
      if (JsonCpp_RELEASE_LIBRARY) 
        list(APPEND JsonCpp_LIBRARY ${JsonCpp_RELEASE_LIBRARY})
      elseif (JsonCpp_DEBUG_LIBRARY) 
        list(APPEND JsonCpp_LIBRARY ${JsonCpp_DEBUG_LIBRARY})
      endif()
    endif()  
    mark_as_advanced(JsonCpp_DEBUG_LIBRARY JsonCpp_RELEASE_LIBRARY)
  endif()

else()

  # TODO: jsoncpp lib names for various systems
  #set(JsonCpp_LIB_NAMES "jsoncpp")

  #if(CMAKE_COMPILER_IS_GNUCXX)
  #  # Get the GCC compiler version
  #  exec_program(${CMAKE_CXX_COMPILER}
  #               ARGS ${CMAKE_CXX_COMPILER_ARG1} -dumpversion
  #               OUTPUT_VARIABLE _gcc_COMPILER_VERSION
  #               OUTPUT_STRIP_TRAILING_WHITESPACE)
  #
  #  set(JsonCpp_LIB_NAMES ${JsonCpp_LIB_NAMES} libjson_linux-gcc-${_gcc_COMPILER_VERSION}_libmt.so)
  #endif()

  find_library(JsonCpp_LIBRARY 
    NAMES 
      jsoncpp
      libjsoncpp
    PATHS 
       ${LibSourcey_DEPENDENCIES_BUILD_DIR}/jsoncpp
       ${LibSourcey_DEPENDENCIES_INSTALL_DIR}/lib
       /usr/lib 
       /usr/local/lib
    )

endif()

if(JsonCpp_LIBRARY AND JsonCpp_INCLUDE_DIR)
  set(JsonCpp_FOUND 1)
  mark_as_advanced(JsonCpp_LIBRARY JsonCpp_INCLUDE_DIR)
else()
  set(JsonCpp_FOUND 0)
endif()

#get_filename_component(JsonCpp_LIBRARY_DIR "${JsonCpp_LIBRARY}" PATH)
#get_filename_component(JsonCpp_LIBRARY "${JsonCpp_LIBRARY}" NAME)
    
# ----------------------------------------------------------------------
# Display status
# ----------------------------------------------------------------------
if(NOT JsonCpp_FOUND)
   if(JsonCpp_FIND_REQUIRED)
      message(FATAL_ERROR "JsonCpp was not found. Please build dependencies first, or specify the path manually.")
   endif()
endif()