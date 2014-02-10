########################################################################
# Cmake module for finding WebRTC
#
# The following variabled will be defined:
#
#  WebRTC_FOUND
#  WebRTC_INCLUDE_DIR
#  WebRTC_LIBRARY
#  WebRTC_DEPENDENCIES
#

# ----------------------------------------------------------------------
# Find WebRTC include path
# ----------------------------------------------------------------------
find_path(WebRTC_INCLUDE_DIR     
  NAMES 
  	webrtc/config.h   
  PATHS
  	D:/dev/lib/webrtc/trunk
)

      
# ----------------------------------------------------------------------
# Find WebRTC libraries
# ----------------------------------------------------------------------
set(WebRTC_DEBUG_PATHS D:/dev/lib/webrtc/trunk/build/Debug/lib)
set(WebRTC_RELEASE_PATHS D:/dev/lib/webrtc/trunk/build/Debug/lib)

# webrtc
sourcey_find_library(WebRTC
  NAMES webrtc_utility
  DEBUG_PATHS ${WebRTC_DEBUG_PATHS}
  RELEASE_PATHS ${WebRTC_RELEASE_PATHS})

if(WebRTC_LIBRARY AND WebRTC_INCLUDE_DIR)

  # TODO: Exclude jsoncp and test libs

  if(WIN32 AND MSVC)    
    if(CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)     
      if (WebRTC_DEBUG_LIBRARY)
        string(REGEX REPLACE "debug;" "" WebRTC_DEBUG_LIBRARY_DIR ${WebRTC_DEBUG_LIBRARY})      
        get_filename_component(debug_dir ${WebRTC_DEBUG_LIBRARY_DIR} PATH)       
        file(GLOB_RECURSE debug_libs "${debug_dir}/*.lib") 
        foreach(lib ${debug_libs})
          list(APPEND WebRTC_DEPENDENCIES "debug" ${lib})
        endforeach() 
        
        # For some reason libyuv.lib is in a different location
        list(APPEND WebRTC_DEPENDENCIES "debug" ${debug_dir}/../libyuv.lib)
      endif()  
      if (WebRTC_RELEASE_LIBRARY)
        string(REGEX REPLACE "optimized;" "" WebRTC_RELEASE_LIBRARY_DIR ${WebRTC_RELEASE_LIBRARY})
        get_filename_component(release_dir ${WebRTC_RELEASE_LIBRARY_DIR} PATH) 
        file(GLOB_RECURSE release_libs "${release_dir}/*.lib") 
        foreach(lib ${release_libs})
          list(APPEND WebRTC_DEPENDENCIES "optimized" ${lib})
        endforeach()
        
        # For some reason libyuv.lib is in a different location
        list(APPEND WebRTC_DEPENDENCIES "optimized" ${release_dir}/../libyuv.lib)
      endif()  
    else()  
      get_filename_component(lib_dir ${WebRTC_LIBRARY} PATH) 
      file(GLOB_RECURSE lib_dir "${lib_dir}/*.lib") 
      foreach(lib ${lib_dir})
        list(APPEND WebRTC_DEPENDENCIES ${lib})
      endforeach()         
      list(APPEND WebRTC_DEPENDENCIES ${lib_dir}/../libyuv.lib)
    endif()  
    
    # Add required system libraries for win32
    set(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} Secur32.lib Winmm.lib msdmo.lib dmoguids.lib wmcodecdspuuid.lib") # strmbase.lib
  else()
      get_filename_component(lib_dir ${WebRTC_LIBRARY} PATH) 
      file(GLOB_RECURSE lib_dir "${lib_dir}/*.a") 
      foreach(lib ${lib_dir})
        list(APPEND WebRTC_DEPENDENCIES ${lib})
      endforeach()    
      list(APPEND WebRTC_DEPENDENCIES ${lib_dir}/../libyuv.a)
  endif() 
  
  set(WebRTC_FOUND 1)
  mark_as_advanced(WebRTC_LIBRARY WebRTC_INCLUDE_DIR)
else()
  set(WebRTC_FOUND 0)
endif()
    
# ----------------------------------------------------------------------
# Display status
# ----------------------------------------------------------------------
if(NOT WebRTC_FOUND)
   if(WebRTC_FIND_REQUIRED)
      message(FATAL_ERROR "WebRTC was not found. Please specify the path manually.")
   endif()
endif()


  # i420
  #sourcey_find_library(WebRTC_i420
  #  NAMES webrtc_i420    
  #  DEBUG_PATHS ${WebRTC_DEBUG_PATHS}
  #  RELEASE_PATHS ${WebRTC_RELEASE_PATHS})
  #if(WebRTC_i420_LIBRARY)
  #  list(APPEND WebRTC_DEPENDENCIES ${WebRTC_i420_LIBRARY})
  #endif()

  # opus
  #sourcey_find_library(WebRTC_opus
  #  NAMES webrtc_opus    
  #  DEBUG_PATHS ${WebRTC_DEBUG_PATHS}
  #  RELEASE_PATHS ${WebRTC_RELEASE_PATHS})
  #if(WebRTC_opus_LIBRARY)
  #  list(APPEND WebRTC_DEPENDENCIES ${WebRTC_opus_LIBRARY})
  #endif()
  
  # utility
  #sourcey_find_library(WebRTC_utility
  #  NAMES webrtc_utility    
  #  DEBUG_PATHS ${WebRTC_DEBUG_PATHS}
  #  RELEASE_PATHS ${WebRTC_RELEASE_PATHS})
  #if(WebRTC_utility_LIBRARY)
  #  list(APPEND WebRTC_DEPENDENCIES ${WebRTC_utility_LIBRARY})
  #endif()
    
  # video_coding
  #sourcey_find_library(WebRTC_video_coding
  #  NAMES webrtc_video_coding    
  #  DEBUG_PATHS ${WebRTC_DEBUG_PATHS}
  #  RELEASE_PATHS ${WebRTC_RELEASE_PATHS})
  #if(WebRTC_video_coding_LIBRARY)
  #  list(APPEND WebRTC_DEPENDENCIES ${WebRTC_video_coding_LIBRARY})
  #endif()
    
  # vp8
  #sourcey_find_library(WebRTC_vp8
  #  NAMES webrtc_vp8    
  #  DEBUG_PATHS ${WebRTC_DEBUG_PATHS}
  #  RELEASE_PATHS ${WebRTC_RELEASE_PATHS})
  #if(WebRTC_vp8_LIBRARY)
  #  list(APPEND WebRTC_DEPENDENCIES ${WebRTC_vp8_LIBRARY})
  #endif()
    
  # libjingle
  #sourcey_find_library(WebRTC_libjingle
  #  NAMES libjingle
  #  DEBUG_PATHS ${WebRTC_DEBUG_PATHS}
  #  RELEASE_PATHS ${WebRTC_RELEASE_PATHS})
  #if(WebRTC_libjingle_LIBRARY)
  #  list(APPEND WebRTC_DEPENDENCIES ${WebRTC_libjingle_LIBRARY})
  #endif()
    
  # libjingle_p2p
  #sourcey_find_library(WebRTC_libjingle_p2p
  #  NAMES libjingle_p2p
  #  DEBUG_PATHS ${WebRTC_DEBUG_PATHS}
  #  RELEASE_PATHS ${WebRTC_RELEASE_PATHS})
  #if(WebRTC_libjingle_p2p_LIBRARY)
  #  list(APPEND WebRTC_DEPENDENCIES ${WebRTC_libjingle_p2p_LIBRARY})
  #endif()
    
  # libjingle_peerconnection
  #sourcey_find_library(WebRTC_libjingle_peerconnection
  #  NAMES libjingle_peerconnection
  #  DEBUG_PATHS ${WebRTC_DEBUG_PATHS}
  #  RELEASE_PATHS ${WebRTC_RELEASE_PATHS})
  #if(WebRTC_libjingle_peerconnection_LIBRARY)
  #  list(APPEND WebRTC_DEPENDENCIES ${WebRTC_libjingle_peerconnection_LIBRARY})
  #endif()