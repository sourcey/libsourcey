# vim: ts=2 sw=2
# - Try to find the required ffmpeg components(default: AVFORMAT, AVUTIL, AVCODEC)
#
# Once done this will define
#  FFmpeg_FOUND         - System has the all required components.
#  FFmpeg_INCLUDE_DIRS  - Include directory necessary for using the required components headers.
#  FFmpeg_LIBRARIES     - Link these to use the required ffmpeg components.
#  FFmpeg_DEFINITIONS   - Compiler switches required for using the required ffmpeg components.
#
# For each of the components it will additionaly set.
#   - AVCODEC
#   - AVDEVICE
#   - AVFORMAT
#   - AVUTIL
#   - POSTPROCESS
#   - SWSCALE
#
# the following variables will be defined
#  <component>_FOUND        - System has <component>
#  <component>_INCLUDE_DIRS - Include directory necessary for using the <component> headers
#  <component>_LIBRARIES    - Link these to use <component>
#  <component>_DEFINITIONS  - Compiler switches required for using <component>
#  <component>_VERSION      - The components version
#
# Copyright (c) 2006, Matthias Kretz, <kretz@kde.org>
# Copyright (c) 2008, Alexander Neundorf, <neundorf@kde.org>
# Copyright (c) 2011, Michael Jansen, <kde@michael-jansen.biz>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

include(FindPackageHandleStandardArgs)

# The default components were taken from a survey over other FindFFmpeg.cmake files
if (NOT FFmpeg_FIND_COMPONENTS)
  set(FFmpeg_FIND_COMPONENTS AVCODEC AVFORMAT AVUTIL)
endif ()

#
### Macro: set_component_found
#
# Marks the given component as found if both *_LIBRARIES AND *_INCLUDE_DIRS is present.
#
macro(set_component_found _component )
  if (${_component}_LIBRARIES AND ${_component}_INCLUDE_DIRS)
     # message(STATUS "  - ${_component} found.")
    set(${_component}_FOUND TRUE)
  else ()
     # message(STATUS "  - ${_component} not found.")
  endif ()
endmacro()

#
### Macro: find_component
#
# Checks for the given component by invoking pkgconfig and then looking up the libraries and
# include directories.
#
macro(find_component _component _pkgconfig _library _header)

  if (NOT WIN32)
     # use pkg-config to get the directories and then use these values
     # in the FIND_PATH() and FIND_LIBRARY() calls
     find_package(PkgConfig)
     if (PKG_CONFIG_FOUND)
       pkg_check_modules(PC_${_component} ${_pkgconfig})
     endif ()
  endif (NOT WIN32)

  find_path(${_component}_INCLUDE_DIRS ${_header}
    HINTS
      ${PC_LIB${_component}_INCLUDEDIR}
      ${PC_LIB${_component}_INCLUDE_DIRS}
    PATH_SUFFIXES
      ffmpeg
  )

  find_library(${_component}_LIBRARIES NAMES ${_library}
      HINTS
      ${PC_LIB${_component}_LIBDIR}
      ${PC_LIB${_component}_LIBRARY_DIRS}
  )

  set(${_component}_DEFINITIONS  ${PC_${_component}_CFLAGS_OTHER} CACHE STRING "The ${_component} CFLAGS.")
  set(${_component}_VERSION      ${PC_${_component}_VERSION}      CACHE STRING "The ${_component} version number.")

  set_component_found(${_component})

  mark_as_advanced(
    ${_component}_INCLUDE_DIRS
    ${_component}_LIBRARIES
    ${_component}_DEFINITIONS
    ${_component}_VERSION)

endmacro()


# Check for cached results. If there are skip the costly part.
if (NOT FFmpeg_FOUND) #FFmpeg_LIBRARIES)

  # Check for all possible component.
  find_component(AVCODEC  libavcodec  avcodec  libavcodec/avcodec.h)
  find_component(AVFORMAT libavformat avformat libavformat/avformat.h)
  find_component(AVDEVICE libavdevice avdevice libavdevice/avdevice.h)
  find_component(AVUTIL   libavutil   avutil   libavutil/avutil.h)
  find_component(SWSCALE  libswscale  swscale  libswscale/swscale.h)
  find_component(POSTPROC libpostproc postproc libpostproc/postprocess.h)
  
  # Set FFmpeg as found. Any missing components will set this to false.
  set(FFmpeg_FOUND 1)

  # Check if the required components were found and add their stuff to the FFmpeg_* vars.
  foreach (_component ${FFmpeg_FIND_COMPONENTS})
    if (${_component}_FOUND)
      # message(STATUS "Required component ${_component} present.")
      set(FFmpeg_LIBRARIES   ${FFmpeg_LIBRARIES}   ${${_component}_LIBRARIES})
      set(FFmpeg_DEFINITIONS ${FFmpeg_DEFINITIONS} ${${_component}_DEFINITIONS})
      list(APPEND FFmpeg_INCLUDE_DIRS ${${_component}_INCLUDE_DIRS})
    else ()
      # message(STATUS "Required component ${_component} missing.")
      set(FFmpeg_FOUND 0)
    endif ()
  endforeach ()

  # Build the include path with duplicates removed.
  if (FFmpeg_INCLUDE_DIRS)
    list(REMOVE_DUPLICATES FFmpeg_INCLUDE_DIRS)
  endif ()

  # cache the vars.
  set(FFmpeg_INCLUDE_DIRS ${FFmpeg_INCLUDE_DIRS} CACHE STRING "The FFmpeg include directories." FORCE)
  set(FFmpeg_LIBRARIES    ${FFmpeg_LIBRARIES}    CACHE STRING "The FFmpeg libraries." FORCE)
  set(FFmpeg_DEFINITIONS  ${FFmpeg_DEFINITIONS}  CACHE STRING "The FFmpeg cflags." FORCE)

  mark_as_advanced(FFmpeg_INCLUDE_DIRS
                   FFmpeg_LIBRARIES
                   FFmpeg_DEFINITIONS)

endif ()

# Now set the noncached _FOUND vars for the components.
foreach (_component AVCODEC AVDEVICE AVFORMAT AVUTIL POSTPROCESS SWSCALE)
  set_component_found(${_component})
endforeach ()

# Compile the list of required vars
set(_FFmpeg_REQUIRED_VARS FFmpeg_LIBRARIES FFmpeg_INCLUDE_DIRS)
foreach (_component ${FFmpeg_FIND_COMPONENTS})
  list(APPEND _FFmpeg_REQUIRED_VARS ${_component}_LIBRARIES ${_component}_INCLUDE_DIRS)
endforeach ()

# Give a nice error message if some of the required vars are missing.
find_package_handle_standard_args(FFmpeg DEFAULT_MSG ${_FFmpeg_REQUIRED_VARS})


# ----------------------------------------------------------------------
# Display status
# ----------------------------------------------------------------------
if(FFmpeg_FOUND)
   if (NOT FFmpeg_FIND_QUIETLY)
      message(STATUS "Found FFmpeg: \n\tINC DIR: ${FFmpeg_INCLUDE_DIRS} \n\tLIBS: ${FFmpeg_LIBRARIES}")
   endif()
else()
   if(FFmpeg_FIND_REQUIRED)
      message(FATAL_ERROR "FFmpeg was not found.")
   endif()
endif()


#message(STATUS "FFmpeg_FOUND: ${FFmpeg_FOUND}")
#message(STATUS "FFmpeg_INCLUDE_DIRS: ${FFmpeg_INCLUDE_DIRS}")
#message(STATUS "FFmpeg_LIBRARIES: ${FFmpeg_LIBRARIES}")
#message(STATUS "FFmpeg_DEFINITIONS: ${FFmpeg_DEFINITIONS}")


# ----------------------------------------------------------------------
# Include and expose to LibSourcey
# ----------------------------------------------------------------------
if(FFmpeg_FOUND)
  include_directories(${FFmpeg_INCLUDE_DIRS})  
  link_directories(${FFmpeg_LIBRARY_DIR})
                        
  set(LIBSOURCEY_INCLUDE_DIRS ${LIBSOURCEY_INCLUDE_DIRS} ${FFmpeg_INCLUDE_DIRS})
  #set(LIBSOURCEY_LIBRARY_DIRS ${LIBSOURCEY_LIBRARY_DIRS} ${FFmpeg_LIBRARY_DIR})
  set(LIBSOURCEY_DEBUG_LIBS   ${LIBSOURCEY_DEBUG_LIBS}   ${FFmpeg_LIBRARIES})    
  set(LIBSOURCEY_RELEASE_LIBS ${LIBSOURCEY_RELEASE_LIBS} ${FFmpeg_LIBRARIES})
endif()