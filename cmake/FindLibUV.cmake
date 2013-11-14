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
find_path(LibUV_INCLUDE_DIR     
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

  add_definitions(-D_WIN32_WINNT=0x0600 -D_GNU_SOURCE)
  #set(LibUV_DEPENDENCIES advapi32.lib iphlpapi.lib psapi.lib shell32.lib ws2_32.lib)

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

  add_definitions(-D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -D_GNU_SOURCE)
  #set(LibUV_DEPENDENCIES "-lm")  
  #set(LibUV_DEPENDENCIES "m")
  #find_library(LIBM_LIBRARY NAMES m)  
  #if(LIBM_LIBRARY)
  #  list(APPEND FFmpeg_DEPENDENCIES ${LIBM_LIBRARY}) 
  #endif()

  if(APPLE)
    add_definitions(-D_DARWIN_USE_64_BIT_INODE=1)
  elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    #set(LibUV_DEPENDENCIES "-ldl -lrt ${LibUV_DEPENDENCIES}")
    #list(APPEND LibUV_DEPENDENCIES "-ldl")   
    #list(APPEND LibUV_DEPENDENCIES "-lrt")   
    #list(APPEND LibUV_DEPENDENCIES "dl")   
    #list(APPEND LibUV_DEPENDENCIES "rt")
    #find_library(LIBDL_LIBRARY NAMES dl)  
    #if(LIBDL_LIBRARY)
    #  list(APPEND LIBDL_DEPENDENCIES ${LIBDL_LIBRARY}) 
    #endif()
    #find_library(LIBRT_LIBRARY NAMES rt)  
    #if(LIBRT_LIBRARY)
    #  list(APPEND LIBDL_DEPENDENCIES ${LIBRT_LIBRARY}) 
    #endif()
  elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Android")
    #set(LibUV_DEPENDENCIES "-ldl ${LibUV_DEPENDENCIES}")
    #list(APPEND LibUV_DEPENDENCIES "-ldl")   
    #find_library(LIBDL_LIBRARY NAMES dl)  
    #if(LIBDL_LIBRARY)
    #  list(APPEND FFmpeg_DEPENDENCIES ${LIBDL_LIBRARY}) 
    #endif()
  else()
    message(FATAL_ERROR
      "Cannot include libuv on this system: ${CMAKE_SYSTEM_NAME}")
  endif()

  # TODO: Solaris, FreeBSD and others; see uv.gyp

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