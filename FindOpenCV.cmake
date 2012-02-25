# - Try to find OpenCV library installation
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
# 
# 2004/05 Jan Woetzel, Friso, Daniel Grest 
# 2006/01 complete rewrite by Jan Woetzel
# 2006/09 2nd rewrite introducing ROOT_DIR and PATH_SUFFIXES 
#   to handle multiple installed versions gracefully by Jan Woetzel
#
# tested with:
# -OpenCV 0.97 (beta5a):  MSVS 7.1, gcc 3.3, gcc 4.1
# -OpenCV 0.99 (1.0rc1):  MSVS 7.1
#
# www.mip.informatik.uni-kiel.de/~jw
# --------------------------------


set(OpenCV_FOUND 0)


# ----------------------------------------------------------------------
# Default OpenCV components to include if COMPONENTS is undefined
# ----------------------------------------------------------------------
if(NOT OpenCV_FIND_COMPONENTS)
  set(OpenCV_FIND_COMPONENTS callib3d contrib core features2d flann gpu harrtraining_engine highgui imgproc legacy ml objdetect video)
endif()


# ----------------------------------------------------------------------
# Standard OpenCV installation paths
# ----------------------------------------------------------------------
set (OpenCV_POSSIBLE_ROOT_DIRS
  "${OpenCV_DIR}"
  "$ENV{OpenCV_ROOT}"
  "$ENV{OpenCV_HOME}"
  "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Intel(R) Open Source Computer Vision Library_is1;Inno Setup: App Path]"
  "$ENV{ProgramFiles}/OpenCV/build"
  /usr/local
  /usr
  )


# ----------------------------------------------------------------------
# Find the OpenCV root dir
# On windows this must be the OpenCV "build" dir
# ----------------------------------------------------------------------
if (NOT OpenCV_INCLUDE_DIR)
  find_path(OpenCV_DIR 
    NAMES 
      include/opencv2/core/core.hpp
      #build/include/opencv2/core/core.hpp
      #../build/opencv2/core/core.hpp
    PATHS
      ${OpenCV_POSSIBLE_ROOT_DIRS} 
    DOC 
      "OpenCV Install Location"
    )
    
  if (NOT OpenCV_DIR)
    message(FATAL_ERROR
      "Failed to locate OpenCV install path; please specify it manually. Version >= 2.3 is supported.")

    return() # Prevent further processing untill we have a root dir
  endif()
endif()


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
  foreach(directory ${OpenCV_INCDIR_SUFFIXES})
    set(directory "${OpenCV_DIR}/${directory}")
    if (IS_DIRECTORY ${directory})
      set(OpenCV_INCLUDE_DIR ${directory})
      break()
    endif()
  endforeach()
    
  if (NOT OpenCV_INCLUDE_DIR)
    message(FATAL_ERROR
      "Failed to locate OpenCV header include path; please specify it manually. Version >= 2.3 is supported.")
      
    return() # Prevent further processing untill we have an include dir
  endif()
endif()



# ----------------------------------------------------------------------
# Find component libraries
# ----------------------------------------------------------------------

# Library link dir suffixes appended to OpenCV_DIR 
set(OpenCV_LIBDIR_SUFFIXES
  lib
  )

# OpenCV 2.3 windows installations keep precompiled binaries
# in a sibling "build" folder adjacent to OpenCV_DIR
if (WIN32)
  set(OpenCV_WIN_LIBDIR_SUFFIX "")
  
  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(OpenCV_WIN_LIBDIR_SUFFIX "${OpenCV_WIN_LIBDIR_SUFFIX}/x64")
  else()
    set(OpenCV_WIN_LIBDIR_SUFFIX "${OpenCV_WIN_LIBDIR_SUFFIX}/x86")
  endif()

  if(MINGW)
    set(OpenCV_WIN_LIBDIR_SUFFIX "${OpenCV_WIN_LIBDIR_SUFFIX}/mingw")
  elseif(MSVC90)
    set(OpenCV_WIN_LIBDIR_SUFFIX "${OpenCV_WIN_LIBDIR_SUFFIX}/vc9")
  elseif(MSVC10)
    set(OpenCV_WIN_LIBDIR_SUFFIX "${OpenCV_WIN_LIBDIR_SUFFIX}/vc10")
  endif()
  
  #if(NOT BUILD_SHARED_LIBS) 
  #  set(OpenCV_WIN_LIBDIR_SUFFIX "${OpenCV_WIN_LIBDIR_SUFFIX}/staticlib")
  #else()
    set(OpenCV_WIN_LIBDIR_SUFFIX "${OpenCV_WIN_LIBDIR_SUFFIX}/lib")
  #endif()
      
  set(OpenCV_LIBDIR_SUFFIXES ${OpenCV_WIN_LIBDIR_SUFFIX} ${OpenCV_LIBDIR_SUFFIXES})

endif()

if (NOT OpenCV_LIBRARY_DIR)

  # Loop through OpenCV_LIBDIR_SUFFIXES to find the best one
  foreach(directory ${OpenCV_LIBDIR_SUFFIXES})
    set(directory "${OpenCV_DIR}/${directory}")
    if (IS_DIRECTORY ${directory})
    
      #message(STATUS "Found OpenCV library directory: ${directory}")
      
      # Set OpenCV_LIBRARY_DIR
      set(OpenCV_LIBRARY_DIR ${directory})
      
      # Glob library files (wish CMake supported "*.{lib|so|a}" syntax :P)
      if (WIN32)
        file(GLOB_RECURSE libraries "${directory}/*.lib") 
      else()
        file(GLOB_RECURSE libraries "${directory}/*.a")
      endif()
      
      foreach(lib ${libraries})
        #get_filename_component(filename ${lib} NAME_WE)
        list(APPEND OpenCV_LIBRARIES ${lib})          
        if (${lib} MATCHES "[0-9][d].*$")
          list(APPEND OpenCV_DEBUG_LIBRARIES ${lib})       
        else()
          list(APPEND OpenCV_RELEASE_LIBRARIES ${lib})     
        endif()
      endforeach()  
      
      # Our work here is done
      break()
         
    endif()
  endforeach()

  if (NOT OpenCV_LIBRARY_DIR)
    set(OpenCV_LIBRARY_DIR "" CACHE PATH "OpenCV Library Directory")
    message(FATAL_ERROR
      "Failed to locate OpenCV libraries; please specify the path manually. Version >= 2.3 is supported.")
      
    return() # Prevent further processing untill we have libraries
  endif()
endif()
  
  
set(OpenCV_FOUND 1)


# ----------------------------------------------------------------------
# Display status
# ----------------------------------------------------------------------
if (OpenCV_FOUND)
   if (NOT OpenCV_FIND_QUIETLY)
      message(STATUS "Found OpenCV: \n\tDir: ${OpenCV_DIR}\n\tDebug: ${OpenCV_DEBUG_LIBRARIES}\n\tRelease: ${OpenCV_RELEASE_LIBRARIES}")
   endif (NOT OpenCV_FIND_QUIETLY)
else ()
   if (OpenCV_FIND_REQUIRED)
      message(FATAL_ERROR "OpenCV was not found.")
   endif (OpenCV_FIND_REQUIRED)
endif()


# ----------------------------------------------------------------------
# Include and expose to LibSourcey
# ----------------------------------------------------------------------
if(OpenCV_FOUND)
  include_directories(${OpenCV_INCLUDE_DIR})  
  link_directories(${OpenCV_LIBRARY_DIR})
                        
  set(LIBSOURCEY_INCLUDE_DIRS ${LIBSOURCEY_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIR})
  set(LIBSOURCEY_LIBRARY_DIRS ${LIBSOURCEY_LIBRARY_DIRS} ${OpenCV_LIBRARY_DIR})
  set(LIBSOURCEY_DEBUG_LIBS   ${LIBSOURCEY_DEBUG_LIBS}   ${OpenCV_DEBUG_LIBRARIES})    
  set(LIBSOURCEY_RELEASE_LIBS ${LIBSOURCEY_RELEASE_LIBS} ${OpenCV_RELEASE_LIBRARIES})
endif()
