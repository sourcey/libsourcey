########################################################################
# Cmake module for finding libuv
#
# The following variabled will be defined:
#
#  libuv_FOUND
#  libuv_INCLUDE_DIR
#  libuv_LIBRARY
#

# ----------------------------------------------------------------------
# Find libuv include path
# ----------------------------------------------------------------------
FIND_PATH(libuv_INCLUDE_DIR     
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
  
  find_library(libuv_DEBUG_LIBRARY 
    NAMES 
      libuvd
    PATHS 
      ${LibSourcey_DEPENDENCIES_BUILD_DIR}/libuv
      ${LibSourcey_DEPENDENCIES_INSTALL_DIR}/lib
      /usr/lib 
      /usr/local/lib
    )
    
  find_library(libuv_RELEASE_LIBRARY 
    NAMES 
      libuv
    PATHS 
      ${LibSourcey_DEPENDENCIES_BUILD_DIR}/libuv
      ${LibSourcey_DEPENDENCIES_INSTALL_DIR}/lib
      /usr/lib 
      /usr/local/lib
    )
    
  if(libuv_DEBUG_LIBRARY OR libuv_RELEASE_LIBRARY)  
    if(CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)            
      if (libuv_RELEASE_LIBRARY) 
        list(APPEND libuv_LIBRARY "optimized" ${libuv_RELEASE_LIBRARY})
      endif()
      if (libuv_DEBUG_LIBRARY)
        list(APPEND libuv_LIBRARY "debug" ${libuv_DEBUG_LIBRARY})
      endif()
    else()    
      if (libuv_RELEASE_LIBRARY) 
        list(APPEND libuv_LIBRARY ${libuv_RELEASE_LIBRARY})
      elseif (libuv_DEBUG_LIBRARY) 
        list(APPEND libuv_LIBRARY ${libuv_DEBUG_LIBRARY})
      endif()
    endif()  
    mark_as_advanced(libuv_DEBUG_LIBRARY libuv_RELEASE_LIBRARY)
  endif()

else()

  # TODO: libuv lib names for various systems
  #set(libuv_LIB_NAMES "libuv")

  #if(CMAKE_COMPILER_IS_GNUCXX)
  #  # Get the GCC compiler version
  #  exec_program(${CMAKE_CXX_COMPILER}
  #               ARGS ${CMAKE_CXX_COMPILER_ARG1} -dumpversion
  #               OUTPUT_VARIABLE _gcc_COMPILER_VERSION
  #               OUTPUT_STRIP_TRAILING_WHITESPACE)
  #
  #  set(libuv_LIB_NAMES ${libuv_LIB_NAMES} libjson_linux-gcc-${_gcc_COMPILER_VERSION}_libmt.so)
  #endif()

  find_library(libuv_LIBRARY 
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

if(libuv_LIBRARY AND libuv_INCLUDE_DIR)
  set(libuv_FOUND 1)
  mark_as_advanced(libuv_LIBRARY libuv_INCLUDE_DIR)
else()
  set(libuv_FOUND 0)
endif()

#get_filename_component(libuv_LIBRARY_DIR "${libuv_LIBRARY}" PATH)
#get_filename_component(libuv_LIBRARY "${libuv_LIBRARY}" NAME)
    
# ----------------------------------------------------------------------
# Display status
# ----------------------------------------------------------------------
if(NOT libuv_FOUND)
   if(libuv_FIND_REQUIRED)
      message(FATAL_ERROR "libuv was not found. Please build dependencies first, or specify the path manually.")
   endif()
endif()