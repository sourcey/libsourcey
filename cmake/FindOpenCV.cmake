# Try to find OpenCV library installation
# See http://sourceforge.net/projects/opencvlibrary/
#
# The following variables are optionally searched for defaults
#  OPENCV_FIND_COMPONENTS : find_package(OPENCV COMPONENTS ..)
#    compatible interface. typically  CV CXCORE CVAUX HIGHGUI CVCAM .. etc.
#
# The following are set after configuration is done:
#  OPENCV_FOUND
#  OPENCV_INCLUDE_DIR
#  OPENCV_LIBRARIES
#  OPENCV_LIBRARY_DIRS
# ----------------------------------------------------------------------

# ----------------------------------------------------------------------
# Default OpenCV components to include if COMPONENTS is undefined
# ----------------------------------------------------------------------
if(NOT OPENCV_FIND_COMPONENTS)
  set(OPENCV_FIND_COMPONENTS callib3d contrib core features2d flann gpu harrtraining_engine highgui imgproc legacy ml objdetect video)
endif()

# ----------------------------------------------------------------------
# Options
# ----------------------------------------------------------------------
set(OPENCV_LINK_SHARED_LIBS ${BUILD_SHARED_LIBS} CACHE BOOL "Link with shared OpenCV libraries (.dll/.so) instead of static ones (.lib/.a)")

# ----------------------------------------------------------------------
# Use pkg-config to find OpenCV if available
# ----------------------------------------------------------------------
find_package(PkgConfig QUIET)
if (PKG_CONFIG_FOUND)
  pkg_search_module(OPENCV opencv)

  # message("OPENCV_FOUND=${OPENCV_FOUND}")
  # message("OPENCV_LIBRARIES=${OPENCV_LIBRARIES}")
  # message("OPENCV_STATIC_LIBRARIES=${OPENCV_STATIC_LIBRARIES}")
  # message("OPENCV_INCLUDE_DIRS=${OPENCV_INCLUDE_DIRS}")
  # message("OPENCV_LIBRARY_DIRS=${OPENCV_LIBRARY_DIRS}")
  # message("OPENCV_DEFINITIONS=${OPENCV_DEFINITIONS}")
  # message("OPENCV_VERSION=${OPENCV_VERSION}")
endif()

# ----------------------------------------------------------------------
# Find component libraries
# ----------------------------------------------------------------------
#set_module_notfound(OPENCV)
if (NOT OPENCV_FOUND)
  set(OPENCV_FOUND 0)

  # Find OpenCV include directory
  # set(OPENCV_INCLUDE_DIR OPENCV_INCLUDE_DIR-NOTFOUND)
  find_path(OPENCV_INCLUDE_DIR
    NAMES
      opencv2/core/core.hpp
    DOC
      "OpenCV Include Directory"
    PATHS
      /usr/local/include
      /usr/include
    )

  # Find OpenCV library directory
  # set(OPENCV_LIBRARY_DIRS OPENCV_LIBRARY_DIRS-NOTFOUND)
  find_path(OPENCV_LIBRARY_DIRS
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

  if(OPENCV_LIBRARY_DIRS)
    set(OPENCV_FOUND 1)

    # Glob library files (wish CMake supported "*.{lib|so|a}" syntax :P)
    if (WIN32)
      file(GLOB_RECURSE libraries "${OPENCV_LIBRARY_DIRS}/*.lib")
    else()
      if (BUILD_SHARED_LIBS)
        file(GLOB_RECURSE libraries "${OPENCV_LIBRARY_DIRS}/*.so")
      else()
        file(GLOB_RECURSE libraries "${OPENCV_LIBRARY_DIRS}/*.a")
      endif()
    endif()

    # TODO: Include only OPENCV_FIND_COMPONENTS
    set(OPENCV_LIBRARIES "")
    foreach(lib ${libraries})
      get_filename_component(filename ${lib} NAME)
      if(${filename} MATCHES "opencv_")
        if(WIN32 AND (CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE))
          if (${filename} MATCHES "d.lib$")
            list(APPEND OPENCV_LIBRARIES "debug" ${filename})
          else()
            list(APPEND OPENCV_LIBRARIES "optimized" ${filename})
          endif()
        else()
          list(APPEND OPENCV_LIBRARIES ${filename})
        endif()
      endif()
    endforeach()

    if(WIN32)
      # Include OpenCV shared libraries
      if(NOT OPENCV_LINK_SHARED_LIBS)
        set(OPENCV_SHARED_LIBRARY_DIR "")
        if(IS_DIRECTORY "${OPENCV_INCLUDE_DIR}/../share/OpenCV/3rdparty/lib")
          set(OPENCV_SHARED_LIBRARY_DIR "${OPENCV_INCLUDE_DIR}/../share/OpenCV/3rdparty/lib")
        elseif(IS_DIRECTORY "${OPENCV_INCLUDE_DIR}/../3rdparty/lib")
          set(OPENCV_SHARED_LIBRARY_DIR "${OPENCV_INCLUDE_DIR}/../3rdparty/lib")
        endif()
        if(OPENCV_SHARED_LIBRARY_DIR)
          list(APPEND OPENCV_LIBRARY_DIRS ${OPENCV_SHARED_LIBRARY_DIR})
          if(CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)
            list(APPEND OPENCV_LIBRARIES debug zlibd.lib)
            list(APPEND OPENCV_LIBRARIES optimized zlib.lib)
          else()
            list(APPEND OPENCV_LIBRARIES zlib.lib)
          endif()
        endif()
        list(APPEND OPENCV_LIBRARIES Vfw32.lib)
      endif()

      # LibSourcey requires HighGUI DirectShow to use multithreaded COM.
      # Just provide a warning for now, but we really should check the source file itself.
      status("IMPORTANT: OpenCV HighGUI DirectShow must be compiled with VI_COM_MULTI_THREADED defined.")
    endif()

    #message(STATUS "Found OpenCV libraries: ${OPENCV_LIBRARIES}")

    #set_module_found(OPENCV)
  endif()
endif()

# Cache the vars.
set(OPENCV_INCLUDE_DIR  ${OPENCV_INCLUDE_DIR}  CACHE STRING   "The OpenCV include directory." FORCE)
set(OPENCV_LIBRARY_DIRS ${OPENCV_LIBRARY_DIRS} CACHE STRING   "The OpenCV library directories." FORCE)
set(OPENCV_LIBRARIES    ${OPENCV_LIBRARIES}    CACHE STRING   "The OpenCV libraries." FORCE)
set(OPENCV_FOUND        ${OPENCV_FOUND}        CACHE BOOLEAN  "The OpenCV found status." FORCE)

mark_as_advanced(OPENCV_INCLUDE_DIR
                 OPENCV_LIBRARY_DIRS
                 OPENCV_LIBRARIES
                 OPENCV_FOUND)

#message("OPENCV_INCLUDE_DIR=${OPENCV_INCLUDE_DIR}")
#message("OPENCV_LIBRARY_DIRS=${OPENCV_LIBRARY_DIRS}")
#message("OPENCV_LIBRARIES=${OPENCV_LIBRARIES}")
#message("OPENCV_VERSION_FILE=${OPENCV_VERSION_FILE}")
#message("OPENCV_VERSION=${OPENCV_VERSION}")

if(NOT OPENCV_FOUND)
 if (OPENCV_FIND_REQUIRED)
    message(FATAL_ERROR
      "OpenCV was not found, please specify the path manually. Version >= 3.0.0 supported.")
 endif()
endif()
