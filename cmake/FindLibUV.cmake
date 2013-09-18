########################################################################
# Cmake module for finding libuv
#
# The following variabled will be defined:
#
#  LibUV_FOUND
#  LibUV_INCLUDE_DIR
#  LibUV_LIBRARY
#

# ----------------------------------------------------------------------
# Find libuv include path
# ----------------------------------------------------------------------
FIND_PATH(LibUV_INCLUDE_DIR     
  NAMES 
  	uv.h   
  PATHS
  	${LibSourcey_DEPENDENCIES_SOURCE_DIR}/libuv/include
  	/usr/local/include
  	/usr/include
)

# ----------------------------------------------------------------------
# Find libuv library
# ----------------------------------------------------------------------
if(WIN32 AND MSVC)
  
  find_library(LibUV_DEBUG_LIBRARY 
    NAMES 
      libuvd
    PATHS 
      ${LibSourcey_DEPENDENCIES_BUILD_DIR}/libuv
      ${LibSourcey_DEPENDENCIES_INSTALL_DIR}/lib
      /usr/lib 
      /usr/local/lib
    )
    
  find_library(LibUV_RELEASE_LIBRARY 
    NAMES 
      libuv
    PATHS 
      ${LibSourcey_DEPENDENCIES_BUILD_DIR}/libuv
      ${LibSourcey_DEPENDENCIES_INSTALL_DIR}/lib
      /usr/lib 
      /usr/local/lib
    )
    
  if(LibUV_DEBUG_LIBRARY OR LibUV_RELEASE_LIBRARY)  
    if(CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)            
      if (LibUV_RELEASE_LIBRARY) 
        list(APPEND LibUV_LIBRARY "optimized" ${LibUV_RELEASE_LIBRARY})
      endif()
      if (LibUV_DEBUG_LIBRARY)
        list(APPEND LibUV_LIBRARY "debug" ${LibUV_DEBUG_LIBRARY})
      endif()
    else()    
      if (LibUV_RELEASE_LIBRARY) 
        list(APPEND LibUV_LIBRARY ${LibUV_RELEASE_LIBRARY})
      elseif (LibUV_DEBUG_LIBRARY) 
        list(APPEND LibUV_LIBRARY ${LibUV_DEBUG_LIBRARY})
      endif()
    endif()  
    mark_as_advanced(LibUV_DEBUG_LIBRARY LibUV_RELEASE_LIBRARY)
  endif()

else()

  # TODO: libuv lib names for various systems
  #set(LibUV_LIB_NAMES "libuv")

  #if(CMAKE_COMPILER_IS_GNUCXX)
  #  # Get the GCC compiler version
  #  exec_program(${CMAKE_CXX_COMPILER}
  #               ARGS ${CMAKE_CXX_COMPILER_ARG1} -dumpversion
  #               OUTPUT_VARIABLE _gcc_COMPILER_VERSION
  #               OUTPUT_STRIP_TRAILING_WHITESPACE)
  #
  #  set(LibUV_LIB_NAMES ${LibUV_LIB_NAMES} libjson_linux-gcc-${_gcc_COMPILER_VERSION}_libmt.so)
  #endif()

  find_library(LibUV_LIBRARY 
    NAMES 
      libuv
      liblibuv
    PATHS 
       ${LibSourcey_DEPENDENCIES_BUILD_DIR}/libuv
       ${LibSourcey_DEPENDENCIES_INSTALL_DIR}/lib
       /usr/lib 
       /usr/local/lib
    )

endif()

if(LibUV_LIBRARY AND LibUV_INCLUDE_DIR)
  set(LibUV_FOUND 1)
  mark_as_advanced(LibUV_LIBRARY LibUV_INCLUDE_DIR)
else()
  set(LibUV_FOUND 0)
endif()

#get_filename_component(LibUV_LIBRARY_DIR "${LibUV_LIBRARY}" PATH)
#get_filename_component(LibUV_LIBRARY "${LibUV_LIBRARY}" NAME)
    
# ----------------------------------------------------------------------
# Display status
# ----------------------------------------------------------------------
if(NOT LibUV_FOUND)
   if(LibUV_FIND_REQUIRED)
      message(FATAL_ERROR "libuv was not found. Please build dependencies first, or specify the path manually.")
   endif()
endif()