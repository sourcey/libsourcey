########################################################################
# Cmake module for finding minizip
#
# The following variabled will be defined:
#
#  Minizip_FOUND
#  Minizip_INCLUDE_DIR
#  Minizip_LIBRARY
#

# ----------------------------------------------------------------------
# Find minizip include path
# ----------------------------------------------------------------------
FIND_PATH(Minizip_INCLUDE_DIR     
  NAMES 
  	zip.h   
  PATHS
  	${LibSourcey_DEPENDENCIES_SOURCE_DIR}/zlib/contrib/minizip
  	/usr/local/include
  	/usr/include
)

# ----------------------------------------------------------------------
# Find minizip library
# ----------------------------------------------------------------------
if(WIN32 AND MSVC)
  
  find_library(Minizip_DEBUG_LIBRARY 
    NAMES 
      minizipd
    PATHS 
      ${LibSourcey_DEPENDENCIES_BUILD_DIR}/zlib/contrib/minizip
      ${LibSourcey_DEPENDENCIES_INSTALL_DIR}/lib
      /usr/lib 
      /usr/local/lib
    )
    
  find_library(Minizip_RELEASE_LIBRARY 
    NAMES 
      minizip
    PATHS 
      ${LibSourcey_DEPENDENCIES_BUILD_DIR}/zlib/contrib/minizip
      ${LibSourcey_DEPENDENCIES_INSTALL_DIR}/lib
      /usr/lib 
      /usr/local/lib
    )
    
  if(Minizip_DEBUG_LIBRARY OR Minizip_RELEASE_LIBRARY)  
    if(CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)            
      if (Minizip_RELEASE_LIBRARY) 
        list(APPEND Minizip_LIBRARY "optimized" ${Minizip_RELEASE_LIBRARY})
      endif()
      if (Minizip_DEBUG_LIBRARY)
        list(APPEND Minizip_LIBRARY "debug" ${Minizip_DEBUG_LIBRARY})
      endif()
    else()    
      if (Minizip_RELEASE_LIBRARY) 
        list(APPEND Minizip_LIBRARY ${Minizip_RELEASE_LIBRARY})
      elseif (Minizip_DEBUG_LIBRARY) 
        list(APPEND Minizip_LIBRARY ${Minizip_DEBUG_LIBRARY})
      endif()
    endif()  
    mark_as_advanced(Minizip_DEBUG_LIBRARY Minizip_RELEASE_LIBRARY)
  endif()

else()

  # TODO: minizip lib names for various systems
  #set(Minizip_LIB_NAMES "minizip")

  #if(CMAKE_COMPILER_IS_GNUCXX)
  #  # Get the GCC compiler version
  #  exec_program(${CMAKE_CXX_COMPILER}
  #               ARGS ${CMAKE_CXX_COMPILER_ARG1} -dumpversion
  #               OUTPUT_VARIABLE _gcc_COMPILER_VERSION
  #               OUTPUT_STRIP_TRAILING_WHITESPACE)
  #
  #  set(Minizip_LIB_NAMES ${Minizip_LIB_NAMES} libjson_linux-gcc-${_gcc_COMPILER_VERSION}_libmt.so)
  #endif()

  find_library(Minizip_LIBRARY 
    NAMES 
      minizip
      libminizip
    PATHS 
       ${LibSourcey_DEPENDENCIES_BUILD_DIR}/minizip
       ${LibSourcey_DEPENDENCIES_INSTALL_DIR}/lib
       /usr/lib 
       /usr/local/lib
    )

endif()

if(Minizip_LIBRARY AND Minizip_INCLUDE_DIR)
  set(Minizip_FOUND 1)
  mark_as_advanced(Minizip_LIBRARY Minizip_INCLUDE_DIR)
else()
  set(Minizip_FOUND 0)
endif()

#get_filename_component(Minizip_LIBRARY_DIR "${Minizip_LIBRARY}" PATH)
#get_filename_component(Minizip_LIBRARY "${Minizip_LIBRARY}" NAME)
    
# ----------------------------------------------------------------------
# Display status
# ----------------------------------------------------------------------
if(NOT Minizip_FOUND)
   if(Minizip_FIND_REQUIRED)
      message(FATAL_ERROR "minizip was not found. Please build dependencies first, or specify the path manually.")
   endif()
endif()