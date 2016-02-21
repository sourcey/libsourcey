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
# Use pkg-config to find OpenCV is available
# ----------------------------------------------------------------------
find_package(PkgConfig QUIET)
if (PKG_CONFIG_FOUND)
  pkg_search_module(OpenCV opencv)

  # message("OpenCV_FOUND=${OpenCV_FOUND}")
  # message("OpenCV_LIBRARIES=${OpenCV_LIBRARIES}")
  # message("OpenCV_STATIC_LIBRARIES=${OpenCV_STATIC_LIBRARIES}")
  # message("OpenCV_INCLUDE_DIRS=${OpenCV_INCLUDE_DIRS}")
  # message("OpenCV_LIBRARY_DIRS=${OpenCV_LIBRARY_DIRS}")
  # message("OpenCV_DEFINITIONS=${OpenCV_DEFINITIONS}")
  # message("OpenCV_VERSION=${OpenCV_VERSION}")
endif()

# ----------------------------------------------------------------------
# Find component libraries
# ----------------------------------------------------------------------
#set_module_notfound(OpenCV)
if (NOT OpenCV_FOUND)
  set(OpenCV_FOUND 0)

  # Find OpenCV include directory
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

  # Find OpenCV library directory
  #set(OpenCV_LIBRARY_DIRS OpenCV_LIBRARY_DIRS-NOTFOUND)
  find_path(OpenCV_LIBRARY_DIRS
    NAMES
      opencv_core
    DOC
      "OpenCV Library Directory"
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
      if(${filename} MATCHES "opencv_")
        if(WIN32 AND (CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE))
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
  endif()
endif()

# Cache the vars.
set(OpenCV_INCLUDE_DIR  ${OpenCV_INCLUDE_DIR}  CACHE STRING   "The OpenCV include directory." FORCE)
set(OpenCV_LIBRARY_DIRS ${OpenCV_LIBRARY_DIRS} CACHE STRING   "The OpenCV library directories." FORCE)
set(OpenCV_LIBRARIES    ${OpenCV_LIBRARIES}    CACHE STRING   "The OpenCV libraries." FORCE)
set(OpenCV_FOUND        ${OpenCV_FOUND}        CACHE BOOLEAN  "The OpenCV found status." FORCE)

mark_as_advanced(OpenCV_INCLUDE_DIR
                 OpenCV_LIBRARY_DIRS
                 OpenCV_LIBRARIES
                 OpenCV_FOUND)

#message("OpenCV_INCLUDE_DIR=${OpenCV_INCLUDE_DIR}")
#message("OpenCV_LIBRARY_DIRS=${OpenCV_LIBRARY_DIRS}")
#message("OpenCV_LIBRARIES=${OpenCV_LIBRARIES}")
#message("OpenCV_VERSION_FILE=${OpenCV_VERSION_FILE}")
#message("OpenCV_VERSION=${OpenCV_VERSION}")

if(NOT OpenCV_FOUND)
 if (OpenCV_FIND_REQUIRED)
    message(FATAL_ERROR
      "OpenCV was not found, please specify the path manually. Version >= 3.0.0 supported.")
 endif()
endif()
