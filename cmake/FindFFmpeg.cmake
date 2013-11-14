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
# TODO: Implement this...
if (NOT FFmpeg_FIND_COMPONENTS)
  set(FFmpeg_FIND_COMPONENTS AVCODEC AVFORMAT AVUTIL SWSCALE SWRESAMPLE)
endif()

# Check for cached results. If there are skip the costly part.
set_module_notfound(FFmpeg)
if (NOT FFmpeg_FOUND)
  
  # These folders represent the default install location for
  # our Capsitrano deploy scripts.
  set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} 
    /home/deploy/ffmpeg_build)

  set(CMAKE_SYSTEM_PREFIX_PATH ${CMAKE_SYSTEM_PREFIX_PATH}
    /home/deploy/ffmpeg_build
    /home/deploy/ffmpeg_build/lib    
    /home/deploy/ffmpeg_build/include)

  # Check for all components
  find_component(FFmpeg AVCODEC    libavcodec    avcodec    libavcodec/avcodec.h)
  find_component(FFmpeg AVFORMAT   libavformat   avformat   libavformat/avformat.h)
  #find_component(FFmpeg AVFILTER   libavfilter   avfilter   libavfilter/avfilter.h)
  find_component(FFmpeg AVUTIL     libavutil     avutil     libavutil/avutil.h)
  #find_component(FFmpeg AVDEVICE   libavdevice   avdevice   libavdevice/avdevice.h)
  find_component(FFmpeg SWSCALE    libswscale    swscale    libswscale/swscale.h)
  find_component(FFmpeg SWRESAMPLE libswresample swresample libswresample/swresample.h)
  #find_component(FFmpeg POSTPROC   libpostproc   postproc   libpostproc/postprocess.h)
  
  # Set FFmpeg as found or not
  set_module_found(FFmpeg)

  # Include FFmpeg dependencies if available
  #find_library(LIBDL_LIBRARY NAMES dl)  
  #if(LIBDL_LIBRARY)
  #  message("################################# LIBDL_LIBRARY=${LIBDL_LIBRARY}")   
  #  list(APPEND FFmpeg_DEPENDENCIES ${LIBDL_LIBRARY}) 
  #endif()

  find_library(LIBVPX_LIBRARY NAMES vpx)  
  if(LIBVPX_LIBRARY)
    list(APPEND FFmpeg_DEPENDENCIES ${LIBVPX_LIBRARY}) 
  endif()

  find_library(LIBX264_LIBRARY NAMES x264)  
  if(LIBX264_LIBRARY)
    list(APPEND FFmpeg_DEPENDENCIES ${LIBX264_LIBRARY}) 
  endif()

  find_library(LIBVA_LIBRARY NAMES va)  
  if(LIBVA_LIBRARY)
    list(APPEND FFmpeg_DEPENDENCIES ${LIBVA_LIBRARY}) 
  endif()

  find_library(LIBFDKAAC_LIBRARY NAMES fdk-aac)  
  if(LIBFDKAAC_LIBRARY)
    list(APPEND FFmpeg_DEPENDENCIES ${LIBFDKAAC_LIBRARY}) 
  endif()

  find_library(LIBMP3LAME_LIBRARY NAMES mp3lame)  
  if(LIBMP3LAME_LIBRARY)
    list(APPEND FFmpeg_DEPENDENCIES ${LIBMP3LAME_LIBRARY}) 
  endif()

  set(LIBVORBIS_LIBRARY LIBVORBIS_LIBRARY-NOTFOUND)
  find_library(LIBVORBIS_LIBRARY NAMES vorbis)  
  if(LIBVORBIS_LIBRARY)
   # message("################################# LIBVORBIS_LIBRARY=${LIBVORBIS_LIBRARY}")   
    list(APPEND FFmpeg_DEPENDENCIES ${LIBVORBIS_LIBRARY}) 
  endif()

  set(LIBVORBISRNC_LIBRARY LIBVORBISRNC_LIBRARY-NOTFOUND)
  find_library(LIBVORBISRNC_LIBRARY NAMES vorbisenc)  
  if(LIBVORBISRNC_LIBRARY)
    #message("################################# LIBVORBISRNC_LIBRARY=${LIBVORBISRNC_LIBRARY}")   
    list(APPEND FFmpeg_DEPENDENCIES ${LIBVORBISRNC_LIBRARY}) 
  endif()

  set(LIBTHEORA_LIBRARY LIBTHEORA_LIBRARY-NOTFOUND)
  find_library(LIBTHEORA_LIBRARY NAMES theora)  
  if(LIBTHEORA_LIBRARY)
    list(APPEND FFmpeg_DEPENDENCIES ${LIBTHEORA_LIBRARY}) 
  endif()

  set(LIBOGG_LIBRARY LIBOGG_LIBRARY-NOTFOUND)
  find_library(LIBOGG_LIBRARY NAMES ogg)  
  if(LIBOGG_LIBRARY)
    list(APPEND FFmpeg_DEPENDENCIES ${LIBOGG_LIBRARY}) 
  endif()

  find_library(LIBOPUS_LIBRARY NAMES opus)  
  if(LIBOPUS_LIBRARY)
    list(APPEND FFmpeg_DEPENDENCIES ${LIBOPUS_LIBRARY}) 
  endif()

  #message("FFmpeg_LIBRARIES=${FFmpeg_LIBRARIES}")     
  #message("FFmpeg_INCLUDE_DIRS=${FFmpeg_INCLUDE_DIRS}")      
  #message("FFmpeg_INCLUDE_DIRS=${FFmpeg_INCLUDE_DIRS}")  
  #message("LIBVPX_LIBRARY=${LIBVPX_LIBRARY}")  

endif ()


# Cache the vars.
#set(FFmpeg_INCLUDE_DIRS ${FFmpeg_INCLUDE_DIRS} CACHE STRING   "The FFmpeg include directories." FORCE)
#set(FFmpeg_LIBRARIES    ${FFmpeg_LIBRARIES}    CACHE STRING   "The FFmpeg libraries." FORCE)
#set(FFmpeg_DEFINITIONS  ${FFmpeg_DEFINITIONS}  CACHE STRING   "The FFmpeg cflags." FORCE)
#set(FFmpeg_FOUND        ${FFmpeg_FOUND}        CACHE BOOLEAN  "The FFmpeg found status." FORCE)


# Check that the required components were found.    
#set(FFmpeg_FOUND 1)
#foreach (_component ${FFmpeg_FIND_COMPONENTS})
#  if (FFmpeg_${_component}_FOUND)
#    message(STATUS "Required component ${_component} present.")
#  else ()
#    message(STATUS "Required component ${_component} missing.")
#    set(FFmpeg_FOUND 0)
#  endif ()
#endforeach ()

# Build the include path with duplicates removed.
#if (FFmpeg_INCLUDE_DIRS)
#  list(REMOVE_DUPLICATES FFmpeg_INCLUDE_DIRS)
#endif ()

#mark_as_advanced(FFmpeg_INCLUDE_DIRS
#                 FFmpeg_LIBRARIES
#                 FFmpeg_DEFINITIONS
#                 FFmpeg_FOUND)

# Now set the noncached _FOUND vars for the components.
#foreach (_component AVCODEC AVDEVICE AVFORMAT AVUTIL POSTPROCESS SWSCALE) #AVFILTER
#  set_component_found(${_component})
#endforeach ()

# Compile the list of required vars
#set(_FFmpeg_REQUIRED_VARS FFmpeg_LIBRARIES FFmpeg_INCLUDE_DIRS)
#foreach (_component ${FFmpeg_FIND_COMPONENTS})
#  list(APPEND _FFmpeg_REQUIRED_VARS ${_component}_LIBRARIES ${_component}_INCLUDE_DIRS)
#endforeach ()

# Give a nice error message if some of the required vars are missing.
#find_package_handle_standard_args(FFmpeg DEFAULT_MSG ${_FFmpeg_REQUIRED_VARS})