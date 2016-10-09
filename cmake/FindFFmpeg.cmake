# - Try to find the required ffmpeg components(default: AVFORMAT, AVUTIL, AVCODEC)
#
# Once done this will define
#  FFMPEG_FOUND         - System has the all required components.
#  FFMPEG_INCLUDE_DIRS  - Include directory necessary for using the required components headers.
#  FFMPEG_LIBRARIES     - Link these to use the required ffmpeg components.
#  FFMPEG_DEFINITIONS   - Compiler switches required for using the required ffmpeg components.
#
# For each of the components it will additionaly set.
#   - AVCODEC
#   - AVFORMAT
#   - AVFILTER
#   - AVUTIL
#   - AVDEVICE
#   - SWSCALE
#   - SWRESAMPLE
#   - POSTPROC
#
# the following variables will be defined
#  <component>_FOUND        - System has <component>
#  <component>_INCLUDE_DIRS - Include directory necessary for using the <component> headers
#  <component>_LIBRARIES    - Link these to use <component>
#  <component>_DEFINITIONS  - Compiler switches required for using <component>
#  <component>_VERSION      - The components version

# The default components were taken from a survey over other FindFFmpeg.cmake files
if (NOT FFMPEG_FIND_COMPONENTS)
  set(FFMPEG_FIND_COMPONENTS AVCODEC AVFORMAT AVUTIL SWSCALE SWRESAMPLE)
endif()

# Check for cached results. If there are skip the costly part.
set_module_notfound(FFMPEG)
if (NOT FFMPEG_FOUND)


  # The FFmpeg compilation guide stores files in an unusual location,
  # so let's support that out of the box
  # http://trac.ffmpeg.org/wiki/CompilationGuide/Ubuntu
  set(CMAKE_LIBRARY_PATH
    $ENV{HOME}/tmp/ffmpeg_build/lib
    $ENV{HOME}/ffmpeg_build/lib
    /tmp/ffmpeg_build/lib
    ${CMAKE_LIBRARY_PATH})
  set(CMAKE_SYSTEM_PREFIX_PATH
    $ENV{HOME}/tmp/ffmpeg_build/include
    $ENV{HOME}/ffmpeg_build/include
    /tmp/ffmpeg_build/include
    ${CMAKE_SYSTEM_PREFIX_PATH})

  # Check for all components
  find_component(FFMPEG SWRESAMPLE libswresample swresample libswresample/swresample.h)
  find_component(FFMPEG AVFORMAT   libavformat   avformat   libavformat/avformat.h)
  find_component(FFMPEG AVCODEC    libavcodec    avcodec    libavcodec/avcodec.h)
  find_component(FFMPEG SWSCALE    libswscale    swscale    libswscale/swscale.h)
  find_component(FFMPEG AVUTIL     libavutil     avutil     libavutil/avutil.h)
  find_component(FFMPEG AVFILTER   libavfilter   avfilter   libavfilter/avfilter.h)
  find_component(FFMPEG AVDEVICE   libavdevice   avdevice   libavdevice/avdevice.h)
  find_component(FFMPEG POSTPROC   libpostproc   postproc   libpostproc/postprocess.h)

  # Set FFmpeg as found or not
  set_module_found(FFMPEG ${FFMPEG_FIND_REQUIRED})

  # Include some dependencies required when linking to FFmpeg static build.
  # NOTE: These libraries are required is compiling FFmpeg with default build
  # specified in https://trac.ffmpeg.org/wiki/CompilationGuide/Ubuntu

  find_library(BZ2_LIBRARY NAMES bz2)
  if(BZ2_LIBRARY)
    list(APPEND FFMPEG_DEPENDENCIES ${BZ2_LIBRARY})
  endif()

  find_library(LZMA_LIBRARY NAMES lzma)
  if(LZMA_LIBRARY)
    list(APPEND FFMPEG_DEPENDENCIES ${LZMA_LIBRARY})
  endif()

  find_library(LIBVPX_LIBRARY NAMES vpx)
  if(LIBVPX_LIBRARY)
    list(APPEND FFMPEG_DEPENDENCIES ${LIBVPX_LIBRARY})
  endif()

  find_library(LIBX264_LIBRARY NAMES x264)
  if(LIBX264_LIBRARY)
    list(APPEND FFMPEG_DEPENDENCIES ${LIBX264_LIBRARY})
  endif()

  find_library(LIBX265_LIBRARY NAMES x265)
  if(LIBX265_LIBRARY)
    list(APPEND FFMPEG_DEPENDENCIES ${LIBX265_LIBRARY})
  endif()

  find_library(LIBVA_LIBRARY NAMES va)
  if(LIBVA_LIBRARY)
    list(APPEND FFMPEG_DEPENDENCIES ${LIBVA_LIBRARY})
  endif()

  find_library(LIBFDKAAC_LIBRARY NAMES fdk-aac)
  if(LIBFDKAAC_LIBRARY)
    list(APPEND FFMPEG_DEPENDENCIES ${LIBFDKAAC_LIBRARY})
  endif()

  find_library(LIBMP3LAME_LIBRARY NAMES mp3lame)
  if(LIBMP3LAME_LIBRARY)
    list(APPEND FFMPEG_DEPENDENCIES ${LIBMP3LAME_LIBRARY})
  endif()

  #set(LIBVORBIS_LIBRARY LIBVORBIS_LIBRARY-NOTFOUND)
  find_library(LIBVORBIS_LIBRARY NAMES vorbis)
  if(LIBVORBIS_LIBRARY)
    list(APPEND FFMPEG_DEPENDENCIES ${LIBVORBIS_LIBRARY})
  endif()

  #set(LIBVORBISRNC_LIBRARY LIBVORBISRNC_LIBRARY-NOTFOUND)
  find_library(LIBVORBISRNC_LIBRARY NAMES vorbisenc)
  if(LIBVORBISRNC_LIBRARY)
    list(APPEND FFMPEG_DEPENDENCIES ${LIBVORBISRNC_LIBRARY})
  endif()

  #set(LIBTHEORA_LIBRARY LIBTHEORA_LIBRARY-NOTFOUND)
  find_library(LIBTHEORA_LIBRARY NAMES theora)
  if(LIBTHEORA_LIBRARY)
    list(APPEND FFMPEG_DEPENDENCIES ${LIBTHEORA_LIBRARY})
  endif()

  #set(LIBOGG_LIBRARY LIBOGG_LIBRARY-NOTFOUND)
  find_library(LIBOGG_LIBRARY NAMES ogg)
  if(LIBOGG_LIBRARY)
    list(APPEND FFMPEG_DEPENDENCIES ${LIBOGG_LIBRARY})
  endif()

  find_library(LIBOPUS_LIBRARY NAMES opus)
  if(LIBOPUS_LIBRARY)
    list(APPEND FFMPEG_DEPENDENCIES ${LIBOPUS_LIBRARY})
  endif()

  print_module_variables(FFMPEG)
endif()

# Cache the vars.
# set(FFMPEG_INCLUDE_DIRS ${FFMPEG_INCLUDE_DIRS} CACHE STRING   "The FFmpeg include directories." FORCE)
# set(FFMPEG_LIBRARIES    ${FFMPEG_LIBRARIES}    CACHE STRING   "The FFmpeg libraries." FORCE)
# set(FFMPEG_DEFINITIONS  ${FFMPEG_DEFINITIONS}  CACHE STRING   "The FFmpeg cflags." FORCE)
# set(FFMPEG_FOUND        ${FFMPEG_FOUND}        CACHE BOOLEAN  "The FFmpeg found status." FORCE)

# Check that the required components were found.
# set(FFMPEG_FOUND 1)
# foreach (_component ${FFMPEG_FIND_COMPONENTS})
#  if (FFMPEG_${_component}_FOUND)
#    message(STATUS "Required component ${_component} present.")
#  else ()
#    message(STATUS "Required component ${_component} missing.")
#    set(FFMPEG_FOUND 0)
#  endif ()
# endforeach ()

# Build the include path with duplicates removed.
# if (FFMPEG_INCLUDE_DIRS)
#  list(REMOVE_DUPLICATES FFMPEG_INCLUDE_DIRS)
# endif ()

# Now set the noncached _FOUND vars for the components.
# foreach (_component AVCODEC AVDEVICE AVFORMAT AVUTIL POSTPROCESS SWSCALE) #AVFILTER
#  set_component_found(${_component})
# endforeach ()

# Compile the list of required vars
# set(_FFMPEG_REQUIRED_VARS FFMPEG_LIBRARIES FFMPEG_INCLUDE_DIRS)
# foreach (_component ${FFMPEG_FIND_COMPONENTS})
#   list(APPEND _FFMPEG_REQUIRED_VARS ${_component}_LIBRARIES ${_component}_INCLUDE_DIRS)
# endforeach ()

# Give a nice error message if some of the required vars are missing.
# include(FindPackageHandleStandardArgs)
# find_package_handle_standard_args(FFMPEG DEFAULT_MSG ${_FFMPEG_REQUIRED_VARS})
#
# mark_as_advanced(FFMPEG_INCLUDE_DIRS
#                  FFMPEG_LIBRARIES
#                  FFMPEG_DEFINITIONS
#                  FFMPEG_FOUND)
