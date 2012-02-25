# ----------------------------------------------------------------------
# This macro defines a generic LibSourcey module 
# ----------------------------------------------------------------------
macro(define_libsourcey_module name)
            
  project(${name})

  # Add library source files
  file(GLOB_RECURSE lib_srcs "src/*.cpp")
  file(GLOB_RECURSE lib_hdrs "src/*.h*")
  file(GLOB_RECURSE lib_int_hdrs "include/*.h*")
  
  source_group("Src" FILES ${lib_srcs})
  source_group("Include" FILES ${lib_hdrs}  ${lib_int_hdrs})
  
  include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
  include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include/Sourcey/${name}")
  
  foreach(module ${ARGN})
    if (IS_DIRECTORY "${CMAKE_SOURCE_DIR}/modules/${module}/include")
      include_directories("${CMAKE_SOURCE_DIR}/modules/${module}/include")
    elseif(IS_DIRECTORY "${CMAKE_SOURCE_DIR}/projects/${module}/include")
      include_directories("${CMAKE_SOURCE_DIR}/projects/${module}/include")
    else()
       message("Unable to include ${module} in ${name}")
    endif()
  endforeach()

  add_library(${name} ${lib_srcs} ${lib_hdrs} ${lib_int_hdrs})

  if(NOT ANDROID)
    # Android SDK build scripts can include only .so files into final .apk
    # As result we should not set version properties for Android
    set_target_properties(${name} PROPERTIES
      VERSION ${LIBSOURCEY_VERSION}
      SOVERSION ${LIBSOURCEY_SOVERSION})
  endif()

  # Additional target properties
  set_target_properties(${name} PROPERTIES
    OUTPUT_NAME "Sourcey${name}${LIBSOURCEY_DLLVERSION}" 
    DEBUG_POSTFIX "${LIBSOURCEY_DEBUG_POSTFIX}"
    ARCHIVE_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}
    RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
    INSTALL_NAME_DIR lib
    LINKER_LANGUAGE CXX)

  # Add dependencies and required libraries for linking
  #foreach(lib IN LISTS LIBSOURCEY_DEBUG_LIBS)
  #  target_link_libraries(${name} debug ${lib})
  #endforeach()
  #foreach(lib IN LISTS LIBSOURCEY_RELEASE_LIBS)
  #  target_link_libraries(${name} optimized ${lib})
  #endforeach()
  #add_dependencies(${name} ${LIBSOURCEY_DEBUG_LIBS} ${LIBSOURCEY_RELEASE_LIBS} ${ARGN})

  install(TARGETS ${name}
    RUNTIME DESTINATION bin COMPONENT main
    LIBRARY DESTINATION lib COMPONENT main
    ARCHIVE DESTINATION lib COMPONENT main)        
  
  # ----------------------------------------------------------------------
  #   Build Samples
  # ----------------------------------------------------------------------
  if(BUILD_MODULE_SAMPLES AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/samples)
    add_subdirectory(samples)
  endif() 
  
  # ----------------------------------------------------------------------
  #   Build Tests
  # ----------------------------------------------------------------------
  if(BUILD_MODULE_TESTS AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tests)
    add_subdirectory(tests)
  endif()    
        
endmacro()
