# Try to find OpenCV library installation
# See http://sourceforge.net/projects/opencvlibrary/
#
# The following variables are optionally searched for defaults
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
# Options
# ----------------------------------------------------------------------
set(OpenCV_LINK_SHARED_LIBS TRUE CACHE BOOL "Link with shared OpenCV libraries (.dll/.so) instead of static ones (.lib/.a)")    


# ----------------------------------------------------------------------
# Find OpenCV include directory
# ----------------------------------------------------------------------
if (NOT OpenCV_INCLUDE_DIR)
  find_path(OpenCV_INCLUDE_DIR 
    NAMES 
      opencv2/core/core.hpp
    DOC 
      "OpenCV Include Directory"
    #PATHS
    #	/usr/local/include
    #	/usr/include
    )	
endif()


# ----------------------------------------------------------------------
# Determine OpenCV version
# ----------------------------------------------------------------------
# Set the version of the OpenCV library we are linking against.
# This is important for windows as the opencv libraries names include the vesion.

if(OpenCV_INCLUDE_DIR AND NOT OpenCV_VERSION_FILE)
  set(OpenCV_VERSION_FILE "${OpenCV_INCLUDE_DIR}/opencv2/core/version.hpp")
  if(EXISTS "${OpenCV_VERSION_FILE}")

    FILE(STRINGS "${OpenCV_VERSION_FILE}" OpenCV_VERSION_PARTS REGEX "#define CV_.+OR_VERSION[ ]+[0-9]+" )

    string(REGEX REPLACE ".+CV_MAJOR_VERSION[ ]+([0-9]+).*" "\\1" OpenCV_VERSION_MAJOR "${OpenCV_VERSION_PARTS}")
    string(REGEX REPLACE ".+CV_MINOR_VERSION[ ]+([0-9]+).*" "\\1" OpenCV_VERSION_MINOR "${OpenCV_VERSION_PARTS}")
    string(REGEX REPLACE ".+CV_SUBMINOR_VERSION[ ]+([0-9]+).*" "\\1" OpenCV_VERSION_PATCH "${OpenCV_VERSION_PARTS}")

    set(OpenCV_VERSION "${OpenCV_VERSION_MAJOR}.${OpenCV_VERSION_MINOR}.${OpenCV_VERSION_PATCH}")
    set(OpenCV_SOVERSION "${OpenCV_VERSION_MAJOR}.${OpenCV_VERSION_MINOR}")
    
  else()
    set(OpenCV_VERSION "2.4.1" CACHE STRING "OpenCV library version.")    
  endif()
endif()


#message("OpenCV_INCLUDE_DIR=${OpenCV_INCLUDE_DIR}")
#message("OpenCV_VERSION_FILE=${OpenCV_VERSION_FILE}")
#message("OpenCV_VERSION=${OpenCV_VERSION}")

# ----------------------------------------------------------------------
# Find component libraries
# ---------------------------------------------------------------------- 
# The reason for including via iteration rather than find_library
# is so we can remain version agnostic.
# TODO: Include only OpenCV_FIND_COMPONENTS
#set_module_notfound(OpenCV)
if (NOT OpenCV_FOUND)
  set(OpenCV_FOUND 0)
  
  find_path(OpenCV_LIBRARY_DIR 
    NAMES       
      libopencv_core.a.${OpenCV_VERSION_MAJOR}.${OpenCV_VERSION_MINOR}.${OpenCV_VERSION_PATCH}      
      libopencv_core.so.${OpenCV_VERSION_MAJOR}.${OpenCV_VERSION_MINOR}.${OpenCV_VERSION_PATCH}
      opencv_core${OpenCV_VERSION_MAJOR}${OpenCV_VERSION_MINOR}${OpenCV_VERSION_PATCH}.lib
      opencv_core${OpenCV_VERSION_MAJOR}${OpenCV_VERSION_MINOR}${OpenCV_VERSION_PATCH}d.lib
    DOC 
      "OpenCV Library Directory"    
    PATH_SUFFIXES 
      Debug
      Release
    PATHS
      /usr/lib 
      /usr/local/lib
    )

  #message("OpenCV_LIBRARY_NAME=${OpenCV_LIBRARY_DIR}")  
  #message("OpenCV_LIBRARY_DIR=libopencv_core.so.${OpenCV_VERSION_MAJOR}.${OpenCV_VERSION_MINOR}.${OpenCV_VERSION_PATCH}")
    
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
      if(${filename} MATCHES "opencv") 
        if(CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)  
          if (${filename} MATCHES "d.lib$")
            list(APPEND OpenCV_LIBRARIES "debug" ${filename})
          else()
            list(APPEND OpenCV_LIBRARIES "optimized" ${filename})
          endif()          
        else()  
          list(APPEND OpenCV_LIBRARIES ${filename})          
        endif()  
      endif()  
    endforeach()  
    
    #message(STATUS "Found OpenCV libraries: ${OpenCV_LIBRARIES}")
     
    # Cache the vars.
    set(OpenCV_INCLUDE_DIR  ${OpenCV_INCLUDE_DIR}  CACHE STRING   "The OpenCV include directory." FORCE)
    set(OpenCV_LIBRARY_DIR  ${OpenCV_LIBRARY_DIR}  CACHE STRING   "The OpenCV library directory." FORCE)
    set(OpenCV_LIBRARIES    ${OpenCV_LIBRARIES}    CACHE STRING   "The OpenCV libraries." FORCE)
    set(OpenCV_FOUND        ${OpenCV_FOUND}        CACHE BOOLEAN  "The OpenCV found status." FORCE)
        
    mark_as_advanced(OpenCV_INCLUDE_DIR
                     OpenCV_LIBRARY_DIR
                     OpenCV_LIBRARIES
                     OpenCV_FOUND)       
  endif()

  if(NOT OpenCV_FOUND)
     if (OpenCV_FIND_REQUIRED)
        message(FATAL_ERROR 
          "OpenCV was not found; please specify the path manually. Version >= 2.3 is supported.")
     endif()
  endif()
endif()