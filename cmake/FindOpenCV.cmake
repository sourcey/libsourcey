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
#  OpenCV_LIBRARY_DIRS
# ----------------------------------------------------------------------

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
#set(OpenCV_INCLUDE_DIR OpenCV_INCLUDE_DIR-NOTFOUND)
if (NOT OpenCV_INCLUDE_DIR)
  find_path(OpenCV_INCLUDE_DIR 
    NAMES 
      opencv2/core/core.hpp
    DOC 
      "OpenCV Include Directory"
    PATHS
  	  /usr/local/include
      /usr/include
    )	
endif()

# ----------------------------------------------------------------------
# Determine OpenCV version
# ----------------------------------------------------------------------
# Set the version of the OpenCV library we are linking against.
# This is important for windows as the opencv libraries names include the vesion.

if(NOT OpenCV_VERSION AND OpenCV_INCLUDE_DIR) # AND NOT OpenCV_VERSION_FILE
  set(OpenCV_VERSION_FILE "${OpenCV_INCLUDE_DIR}/opencv2/core/version.hpp")
  if(EXISTS "${OpenCV_VERSION_FILE}")

    file(STRINGS "${OpenCV_VERSION_FILE}" OpenCV_VERSION_PARTS REGEX "#define CV_VERSION_[A-Z]+[ ]+[0-9]+" )

    string(REGEX REPLACE ".+CV_VERSION_EPOCH[ ]+([0-9]+).*" "\\1" OpenCV_VERSION_MAJOR "${OpenCV_VERSION_PARTS}")
    string(REGEX REPLACE ".+CV_VERSION_MAJOR[ ]+([0-9]+).*" "\\1" OpenCV_VERSION_MINOR "${OpenCV_VERSION_PARTS}")
    string(REGEX REPLACE ".+CV_VERSION_MINOR[ ]+([0-9]+).*" "\\1" OpenCV_VERSION_PATCH "${OpenCV_VERSION_PARTS}")
    string(REGEX REPLACE ".+CV_VERSION_REVISION[ ]+([0-9]+).*" "\\1" OpenCV_VERSION_TWEAK "${OpenCV_VERSION_PARTS}")

    if(OpenCV_VERSION_MAJOR AND OpenCV_VERSION_MINOR)
      set(OpenCV_VERSION "${OpenCV_VERSION_MAJOR}.${OpenCV_VERSION_MINOR}.${OpenCV_VERSION_PATCH}")
      if(OpenCV_VERSION_TWEAK GREATER 0)
        set(OpenCV_VERSION "${OpenCV_VERSION}.${OpenCV_VERSION_TWEAK}")
      endif()

      set(OpenCV_SOVERSION "${OpenCV_VERSION_MAJOR}.${OpenCV_VERSION_MINOR}")
      set(OpenCV_LIBVERSION "${OpenCV_VERSION_MAJOR}.${OpenCV_VERSION_MINOR}.${OpenCV_VERSION_PATCH}")
    endif()
  #else()
  #  set(OpenCV_VERSION "2.4.5" CACHE STRING "OpenCV library version.")    
  endif()
endif()

#if(NOT OpenCV_VERSION)
#  message(FATAL_ERROR "Cannot determine installed OpenCV version; please specify it manually. Version >= 2.4.5 supported.")
#endif()


# ----------------------------------------------------------------------
# Find component libraries
# ---------------------------------------------------------------------- 
# The reason for including via iteration rather than find_library
# is so we can remain version agnostic.
#set_module_notfound(OpenCV)
#set(OpenCV_FOUND 0)
if (NOT OpenCV_FOUND)
  set(OpenCV_FOUND 0)
  
  set(OpenCV_LIBRARY_DIRS OpenCV_LIBRARY_DIRS-NOTFOUND)
  find_path(OpenCV_LIBRARY_DIRS 
    NAMES       
      libopencv_core.a.${OpenCV_VERSION_MAJOR}.${OpenCV_VERSION_MINOR}.${OpenCV_VERSION_PATCH}      
      libopencv_core.so.${OpenCV_VERSION_MAJOR}.${OpenCV_VERSION_MINOR}.${OpenCV_VERSION_PATCH}     
      opencv_core${OpenCV_VERSION_MAJOR}${OpenCV_VERSION_MINOR}${OpenCV_VERSION_PATCH}.lib
      opencv_core${OpenCV_VERSION_MAJOR}${OpenCV_VERSION_MINOR}${OpenCV_VERSION_PATCH}d.lib
      libopencv_core.a     
      libopencv_core.so    
    DOC 
      "OpenCV Library Directory"    
    #PATH_SUFFIXES 
    #  Debug
    #  Release
    PATHS
      /usr/lib 
      /usr/lib/x86_64-linux-gnu
      /usr/lib/i386-linux-gnu
      /usr/lib64
      /usr/local/lib
      /usr/local/lib/x86_64-linux-gnu
      /usr/local/lib/i386-linux-gnu
      /usr/local/lib64
    )
    
  if(OpenCV_LIBRARY_DIRS)
    set(OpenCV_FOUND 1)    
    
    # Glob library files (wish CMake supported "*.{lib|so|a}" syntax :P)
    if (WIN32)
      file(GLOB_RECURSE libraries "${OpenCV_LIBRARY_DIRS}/*.lib") 
    else()
      if (BUILD_SHARED_LIBS)
        file(GLOB_RECURSE libraries "${OpenCV_LIBRARY_DIRS}/*.so")
      else()    
        file(GLOB_RECURSE libraries "${OpenCV_LIBRARY_DIRS}/*.a")
      endif()    
    endif()    
    
    # TODO: Include only OpenCV_FIND_COMPONENTS
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
  
    if(WIN32)
      # Include OpenCV shared libraries      
      if(NOT OpenCV_LINK_SHARED_LIBS)
        set(OpenCV_SHARED_LIBRARY_DIR "")
        if(IS_DIRECTORY "${OpenCV_INCLUDE_DIR}/../share/OpenCV/3rdparty/lib")
          set(OpenCV_SHARED_LIBRARY_DIR "${OpenCV_INCLUDE_DIR}/../share/OpenCV/3rdparty/lib")
        elseif(IS_DIRECTORY "${OpenCV_INCLUDE_DIR}/../3rdparty/lib")
          set(OpenCV_SHARED_LIBRARY_DIR "${OpenCV_INCLUDE_DIR}/../3rdparty/lib")
        endif()  
        if(OpenCV_SHARED_LIBRARY_DIR)       
          list(APPEND OpenCV_LIBRARY_DIRS ${OpenCV_SHARED_LIBRARY_DIR})       
          if(CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)  
            list(APPEND OpenCV_LIBRARIES debug zlibd.lib)
            list(APPEND OpenCV_LIBRARIES optimized zlib.lib)      
          else()  
            list(APPEND OpenCV_LIBRARIES zlib.lib)          
          endif()          
        endif() 
        list(APPEND OpenCV_LIBRARIES Vfw32.lib)   
      endif() 
      
      # LibSourcey requires HighGUI DirectShow to use multithreaded COM.
      # Just provide a warning for now, but we really should check the source file itself.
      status("IMPORTANT: OpenCV HighGUI DirectShow must be compiled with VI_COM_MULTI_THREADED defined.")
    endif()
  
    #message(STATUS "Found OpenCV libraries: ${OpenCV_LIBRARIES}")
    
    #set_module_found(OpenCV)
     
    # Cache the vars.
    set(OpenCV_INCLUDE_DIR  ${OpenCV_INCLUDE_DIR}  CACHE STRING   "The OpenCV include directory." FORCE)
    set(OpenCV_LIBRARY_DIRS ${OpenCV_LIBRARY_DIRS} CACHE STRING   "The OpenCV library directories." FORCE)
    set(OpenCV_LIBRARIES    ${OpenCV_LIBRARIES}    CACHE STRING   "The OpenCV libraries." FORCE)
    set(OpenCV_FOUND        ${OpenCV_FOUND}        CACHE BOOLEAN  "The OpenCV found status." FORCE)
        
    mark_as_advanced(OpenCV_INCLUDE_DIR
                     OpenCV_LIBRARY_DIRS
                     OpenCV_LIBRARIES
                     OpenCV_FOUND)       
  endif()
endif()

message("OpenCV_INCLUDE_DIR=${OpenCV_INCLUDE_DIR}")
message("OpenCV_LIBRARY_DIRS=${OpenCV_LIBRARY_DIRS}")
message("OpenCV_LIBRARIES=${OpenCV_LIBRARIES}")
message("OpenCV_VERSION_FILE=${OpenCV_VERSION_FILE}")
message("OpenCV_VERSION=${OpenCV_VERSION}")

if(NOT OpenCV_FOUND)
 if (OpenCV_FIND_REQUIRED)
    message(FATAL_ERROR 
      "OpenCV was not found; please specify the path manually. Version >= 2.4.5 supported.")
 endif()
endif()
