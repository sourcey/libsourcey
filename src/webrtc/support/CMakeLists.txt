if(TARGET webrtc_support)
  return()
endif()

if(NOT TARGET webrtc)
  message(FATAL_ERROR "webrtc_support requires the webrtc target")
endif()

if(NOT TARGET symple)
  message(FATAL_ERROR "webrtc_support requires the symple target")
endif()

if(NOT TARGET graft)
  message(FATAL_ERROR "webrtc_support requires the graft target")
endif()

file(GLOB_RECURSE _srcs "src/*.cpp")
file(GLOB_RECURSE _hdrs "include/*.h")
set(_pipeline_capi_src "${CMAKE_CURRENT_SOURCE_DIR}/src/pipelineapi.cpp")
list(REMOVE_ITEM _srcs "${_pipeline_capi_src}")

add_library(webrtc_support STATIC ${_srcs} ${_hdrs})
add_library(icey::webrtc_support ALIAS webrtc_support)

target_include_directories(webrtc_support
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(webrtc_support
  PUBLIC base graft json symple webrtc
)

add_library(pipeline_capi SHARED ${_pipeline_capi_src})
add_library(icey::pipeline_capi ALIAS pipeline_capi)

target_include_directories(pipeline_capi
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../graft/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(pipeline_capi
  PRIVATE webrtc_support
)

set_target_properties(pipeline_capi PROPERTIES
  OUTPUT_NAME icy_pipeline_capi
  DEBUG_POSTFIX "${icey_DEBUG_POSTFIX}"
)

if(ENABLE_SOLUTION_FOLDERS)
  set_target_properties(webrtc_support PROPERTIES FOLDER "support")
  set_target_properties(pipeline_capi PROPERTIES FOLDER "support")
endif()

# Only export when webrtc itself is exported (system deps mode).
# Vendored libdatachannel creates export set conflicts.
if(USE_SYSTEM_DEPS)
  install(TARGETS webrtc_support
    EXPORT iceyTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT dev
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT libs
  )

  install(TARGETS pipeline_capi
    EXPORT iceyTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT libs
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT libs
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT dev
  )
else()
  install(TARGETS pipeline_capi
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT libs
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT libs
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT dev
  )
endif()

install(DIRECTORY include/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  COMPONENT dev
  FILES_MATCHING PATTERN "*.h"
)
