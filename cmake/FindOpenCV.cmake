# Try to find OpenCV library installation
# See http://sourceforge.net/projects/opencvlibrary/
#
# The following variables are optionally searched for defaults
#  OpenCV_DIR:            Base directory of OpenCv tree to use.
#  OpenCV_FIND_COMPONENTS : find_package(OpenCV COMPONENTS ..) 
#    compatible interface. typically  CV CXCORE CVAUX HIGHGUI CVCAM .. etc.
#
# The following are set after configuration is done: 
#  OpenCV_FOUND
#  OpenCV_INCLUDE_DIR
#  OpenCV_LIBRARIES
#  OpenCV_LIBRARY_DIR
# --------------------------------


# ----------------------------------------------------------------------
# Default OpenCV components to include if COMPONENTS is undefined
# ----------------------------------------------------------------------
if(NOT OpenCV_FIND_COMPONENTS)
  set(OpenCV_FIND_COMPONENTS callib3d contrib core features2d flann gpu harrtraining_engine highgui imgproc legacy ml objdetect video)
endif()


# ----------------------------------------------------------------------
# Standard OpenCV installation paths
# ----------------------------------------------------------------------
#set (OpenCV_POSSIBLE_ROOT_DIRS
#  "${OpenCV_DIR}"
#  "$ENV{OpenCV_ROOT}"
#  "$ENV{OpenCV_HOME}"
#  "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Intel(R) Open Source Computer Vision Library_is1;Inno Setup: App Path]"
#  "$ENV{ProgramFiles}/OpenCV/build"
#  /usr/local
#  /usr
#  )


# ----------------------------------------------------------------------
# Find the OpenCV root dir
# On windows this must be the OpenCV "build" dir
# ----------------------------------------------------------------------
#if (NOT OpenCV_DIR)
#  find_path(OpenCV_DIR 
#    NAMES 
#      include/opencv2/core/core.hpp
#    PATHS
#      ${OpenCV_POSSIBLE_ROOT_DIRS} 
#    DOC 
#      "OpenCV Install Location"
#    )
#    
#  if (NOT OpenCV_DIR)
#    message(FATAL_ERROR
#      "Failed to locate OpenCV install path; please specify it manually. Version >= 2.3 is supported.")
#
#    return() # Prevent further processing untill we have a root dir
#  endif()
#  mark_as_advanced(FORCE OpenCV_DIR)  
#endif()


# ----------------------------------------------------------------------
# Find OpenCV include directory
# ----------------------------------------------------------------------

# Header include dir suffixes appended to OpenCV_DIR
set(OpenCV_INCDIR_SUFFIXES
  include
  build/include
  ../build/include
  )

if (NOT OpenCV_INCLUDE_DIR)
  find_path(OpenCV_INCLUDE_DIR 
    NAMES 
      #include/opencv2/core/core.hpp
      opencv2/core/core.hpp
    #PATHS
    #  ${OpenCV_POSSIBLE_ROOT_DIRS} 
    DOC 
      "OpenCV Include Directory"
    )
  #foreach(directory ${OpenCV_INCDIR_SUFFIXES})
  #  set(directory "${OpenCV_DIR}/${directory}")
  #  if (IS_DIRECTORY ${directory})
  #    set(OpenCV_INCLUDE_DIR ${directory})
  #    break()
  #  endif()
  #endforeach()
endif()


# ----------------------------------------------------------------------
# Find component libraries
# ----------------------------------------------------------------------

set(OpenCV_LINK_SHARED_LIBS TRUE CACHE BOOL "Link with shared OpenCV libraries (.dll/.so) instead of static ones (.lib/.a)")    

# Library link dir suffixes appended to OpenCV_DIR 
#set(OpenCV_LIBDIR_SUFFIXES
#  lib
#  )

# OpenCV 2.3 windows installations keep precompiled binaries
# in a sibling "build" folder adjacent to OpenCV_DIR
#if (WIN32)
#  set(OpenCV_WIN_LIBDIR_SUFFIX "")
#  
#  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
#    set(OpenCV_WIN_LIBDIR_SUFFIX "${OpenCV_WIN_LIBDIR_SUFFIX}/x64")
#  else()
#    set(OpenCV_WIN_LIBDIR_SUFFIX "${OpenCV_WIN_LIBDIR_SUFFIX}/x86")
#  endif()
#
#  if(MINGW)
#    set(OpenCV_WIN_LIBDIR_SUFFIX "${OpenCV_WIN_LIBDIR_SUFFIX}/mingw")
#  elseif(MSVC90)
#    set(OpenCV_WIN_LIBDIR_SUFFIX "${OpenCV_WIN_LIBDIR_SUFFIX}/vc9")
#  elseif(MSVC10)
#    set(OpenCV_WIN_LIBDIR_SUFFIX "${OpenCV_WIN_LIBDIR_SUFFIX}/vc10")
#  endif()
#  
#  if(NOT OpenCV_LINK_SHARED_LIBS)
#    set(OpenCV_WIN_LIBDIR_SUFFIX "${OpenCV_WIN_LIBDIR_SUFFIX}/staticlib")
#  else()
#    set(OpenCV_WIN_LIBDIR_SUFFIX "${OpenCV_WIN_LIBDIR_SUFFIX}/lib")
#  endif()
#      
#  set(OpenCV_LIBDIR_SUFFIXES ${OpenCV_WIN_LIBDIR_SUFFIX} ${OpenCV_LIBDIR_SUFFIXES})
#endif()

set(OpenCV_LIBRARY_VERSION "2.4.0" CACHE STRING "OpenCV library version.")    
string(REGEX MATCHALL "[0-9]" OpenCV_LIBRARY_VERSION_PARTS "${OpenCV_LIBRARY_VERSION}")

list(GET OpenCV_LIBRARY_VERSION_PARTS 0 OpenCV_VERSION_MAJOR)
list(GET OpenCV_LIBRARY_VERSION_PARTS 1 OpenCV_VERSION_MINOR)
list(GET OpenCV_LIBRARY_VERSION_PARTS 2 OpenCV_VERSION_PATCH)

    
# The reason for including via iteration rather than find_library
# is so we can remain version agnostic.
# TODO: Include only specified modules
# TODO: Check for specified version
# TODO: Optimize the following, it will probably break on some systems
if (NOT OpenCV_FOUND) #OpenCV_INCLUDE_DIR AND NOT OpenCV_LIBRARY_DIR)
  set(OpenCV_FOUND 0)
  
  find_path(OpenCV_LIBRARY_DIR 
    NAMES 
      opencv_core${OpenCV_VERSION_MAJOR}${OpenCV_VERSION_MINOR}${OpenCV_VERSION_PATCH}.a
      opencv_core${OpenCV_VERSION_MAJOR}${OpenCV_VERSION_MINOR}${OpenCV_VERSION_PATCH}.lib
      opencv_core${OpenCV_VERSION_MAJOR}${OpenCV_VERSION_MINOR}${OpenCV_VERSION_PATCH}d.lib
    DOC 
      "OpenCV Library Directory"
    )    
  
  #message(STATUS "Searching for OpenCV ${OpenCV_VERSION_MAJOR}${OpenCV_VERSION_MINOR}${OpenCV_VERSION_PATCH}")
  #message(STATUS "Found OpenCV library directory: ${OpenCV_LIBRARY_DIR}")
  # Loop through OpenCV_LIBDIR_SUFFIXES to find the best one
  #foreach(directory ${OpenCV_LIBDIR_SUFFIXES})
  #  set(directory "${OpenCV_DIR}/${directory}")
  #  if (IS_DIRECTORY ${directory})
                   
      # Our work here is done
      #break()
  #endforeach()
  
  
  if(OpenCV_LIBRARY_DIR)
    set(OpenCV_FOUND 1)    
    
    # Glob library files (wish CMake supported "*.{lib|so|a}" syntax :P)
    if (WIN32)
      file(GLOB_RECURSE libraries "${OpenCV_LIBRARY_DIR}/*.lib") 
    else()
      file(GLOB_RECURSE libraries "${OpenCV_LIBRARY_DIR}/*.a")
    endif()
    
    set(OpenCV_LIBRARIES "")
    foreach(lib ${libraries})
      get_filename_component(filename ${lib} NAME)        
      if(CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)  
        if (${filename} MATCHES "d.lib$")
          list(APPEND OpenCV_LIBRARIES "debug" ${filename})
        else()
          list(APPEND OpenCV_LIBRARIES "optimized" ${filename})
        endif()          
      else()  
        list(APPEND OpenCV_LIBRARIES ${filename})          
      endif()  
    endforeach()  
    
    message(STATUS "Found OpenCV libraries: ${OpenCV_LIBRARIES}")
       
  endif()
     
  # Cache the vars.
  set(OpenCV_INCLUDE_DIR  ${OpenCV_INCLUDE_DIR}  CACHE STRING   "The OpenCV include directory." FORCE)
  set(OpenCV_LIBRARY_DIR  ${OpenCV_LIBRARY_DIR}  CACHE STRING   "The OpenCV library directory." FORCE)
  set(OpenCV_LIBRARIES    ${OpenCV_LIBRARIES}    CACHE STRING   "The OpenCV libraries." FORCE)
  set(OpenCV_FOUND        ${OpenCV_FOUND}        CACHE BOOLEAN  "The OpenCV found status." FORCE)
      
  mark_as_advanced(OpenCV_INCLUDE_DIR
                   OpenCV_LIBRARY_DIR
                   OpenCV_LIBRARIES
                   OpenCV_FOUND)

  if(NOT OpenCV_FOUND)
     if (OpenCV_FIND_REQUIRED)
        message(FATAL_ERROR 
          "OpenCV was not found; please specify the path manually. Version >= 2.3 is supported.")
     endif()
  endif()
endif()