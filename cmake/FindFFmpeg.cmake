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
#   - AVFILTER
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

include(LibSourceyMacros)
include(FindPackageHandleStandardArgs)

# The default components were taken from a survey over other FindFFmpeg.cmake files
if (NOT FFmpeg_FIND_COMPONENTS)
  set(FFmpeg_FIND_COMPONENTS AVCODEC AVFORMAT AVUTIL SWSCALE) #AVFILTER
endif()


# Check for cached results. If there are skip the costly part.
if (NOT FFmpeg_FOUND) #FFmpeg_LIBRARIES
      
  # Check for all possible component.
  find_component(FFmpeg AVCODEC  libavcodec  avcodec  libavcodec/avcodec.h)
  find_component(FFmpeg AVFORMAT libavformat avformat libavformat/avformat.h)
  find_component(FFmpeg AVFILTER libavfilter avfilter libavfilter/avfilter.h)
  find_component(FFmpeg AVDEVICE libavdevice avdevice libavdevice/avdevice.h)
  find_component(FFmpeg AVUTIL   libavutil   avutil   libavutil/avutil.h)
  find_component(FFmpeg SWSCALE  libswscale  swscale  libswscale/swscale.h)
  find_component(FFmpeg POSTPROC libpostproc postproc libpostproc/postprocess.h)
  
  # Set FFmpeg as found. Any missing components will set this to false.
  set(FFmpeg_FOUND 1)

  # Check if the required components were found and add their stuff to the FFmpeg_* vars.
  foreach (_component ${FFmpeg_FIND_COMPONENTS})
    if (${_component}_FOUND)
      message(STATUS "Required component ${_component} present.")
      set(FFmpeg_LIBRARIES   ${FFmpeg_LIBRARIES}   ${${_component}_LIBRARIES})
      set(FFmpeg_DEFINITIONS ${FFmpeg_DEFINITIONS} ${${_component}_DEFINITIONS})
      list(APPEND FFmpeg_INCLUDE_DIRS ${${_component}_INCLUDE_DIRS})
    else ()
      message(STATUS "Required component ${_component} missing.")
      set(FFmpeg_FOUND 0)
    endif ()
  endforeach ()

  # Build the include path with duplicates removed.
  if (FFmpeg_INCLUDE_DIRS)
    list(REMOVE_DUPLICATES FFmpeg_INCLUDE_DIRS)
  endif ()

  # Cache the vars.
  set(FFmpeg_INCLUDE_DIRS ${FFmpeg_INCLUDE_DIRS} CACHE STRING   "The FFmpeg include directories." FORCE)
  set(FFmpeg_LIBRARIES    ${FFmpeg_LIBRARIES}    CACHE STRING   "The FFmpeg libraries." FORCE)
  set(FFmpeg_DEFINITIONS  ${FFmpeg_DEFINITIONS}  CACHE STRING   "The FFmpeg cflags." FORCE)
  set(FFmpeg_FOUND        ${FFmpeg_FOUND}        CACHE BOOLEAN  "The FFmpeg found status." FORCE)

  mark_as_advanced(FFmpeg_INCLUDE_DIRS
                   FFmpeg_LIBRARIES
                   FFmpeg_DEFINITIONS
                   FFmpeg_FOUND)

endif ()

# Now set the noncached _FOUND vars for the components.
foreach (_component AVCODEC AVDEVICE AVFORMAT AVUTIL POSTPROCESS SWSCALE) #AVFILTER
  set_component_found(${_component})
endforeach ()

# Compile the list of required vars
set(_FFmpeg_REQUIRED_VARS FFmpeg_LIBRARIES FFmpeg_INCLUDE_DIRS)
foreach (_component ${FFmpeg_FIND_COMPONENTS})
  list(APPEND _FFmpeg_REQUIRED_VARS ${_component}_LIBRARIES ${_component}_INCLUDE_DIRS)
endforeach ()

# Give a nice error message if some of the required vars are missing.
find_package_handle_standard_args(FFmpeg DEFAULT_MSG ${_FFmpeg_REQUIRED_VARS})