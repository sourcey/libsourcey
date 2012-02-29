#
### Macro: define_libsourcey_module
#
# Defines a generic LibSourcey module library.
#
macro(define_libsourcey_module name)       
    
  set(BUILD_MODULE_${name} ON CACHE BOOL "Build LibSourcey module: ${name}")
  if(BUILD_MODULE_${name}) 
    mark_as_advanced(FORCE BUILD_MODULE_${name})  
           
    project(${name})

    # Add library source files
    file(GLOB_RECURSE lib_srcs "src/*.c*")
    file(GLOB_RECURSE lib_hdrs "src/*.h*")
    file(GLOB_RECURSE lib_int_hdrs "include/*.h*")
    
    source_group("Src" FILES ${lib_srcs})
    source_group("Include" FILES ${lib_hdrs}  ${lib_int_hdrs})

    add_library(${name} ${lib_srcs} ${lib_hdrs} ${lib_int_hdrs})     
    
    # Include dependent modules
    foreach(module ${ARGN})
      include_libsourcey_modules(${module})  
      add_dependencies(${name} ${module})
    endforeach()  

    # Include external dependencies
    target_link_libraries(${name} ${LibSourcey_INCLUDE_LIBRARIES})
    add_dependencies(${name} ${LibSourcey_INCLUDE_LIBRARIES})   
    
    # Include library and header directories
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")  
    include_directories(${LibSourcey_INCLUDE_DIRS})
    link_directories(${LibSourcey_LIBRARY_DIRS})
    
    #message(STATUS "LibSourcey_INCLUDE_DIRS: ${LibSourcey_INCLUDE_DIRS}")
    #message(STATUS "LibSourcey_LIBRARY_DIRS: ${LibSourcey_LIBRARY_DIRS}")
    #message(STATUS "LibSourcey_INCLUDE_LIBRARIES: ${LibSourcey_INCLUDE_LIBRARIES}")

    if(NOT ANDROID)
      # Android SDK build scripts can include only .so files into final .apk
      # As result we should not set version properties for Android
      set_target_properties(${name} PROPERTIES
        VERSION ${LibSourcey_VERSION}
        SOVERSION ${LibSourcey_SOVERSION})
    endif()

    # Additional target properties
    set_target_properties(${name} PROPERTIES
      OUTPUT_NAME "Sourcey${name}${LibSourcey_DLLVERSION}" 
      DEBUG_POSTFIX "${LibSourcey_DEBUG_POSTFIX}"
      #ARCHIVE_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}
      #RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
      INSTALL_NAME_DIR lib
      LINKER_LANGUAGE CXX)
      
    install(TARGETS ${name}
      RUNTIME DESTINATION bin COMPONENT main
      LIBRARY DESTINATION lib COMPONENT main
      ARCHIVE DESTINATION lib COMPONENT main)        
    
    # ----------------------------------------------------------------------
    # Build Samples
    # ----------------------------------------------------------------------
    if(BUILD_MODULE_SAMPLES AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/samples)
      add_subdirectory(samples)
    endif() 
    
    # ----------------------------------------------------------------------
    # Build Tests
    # ----------------------------------------------------------------------
    if(BUILD_MODULE_TESTS AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tests)
      add_subdirectory(tests)
    endif()    
  
  endif()
        
endmacro()


  #list(REMOVE_DUPLICATES LibSourcey_DEBUG_LIBS)
  #list(REMOVE_DUPLICATES LibSourcey_RELEASE_LIBS)
  #foreach(lib IN LISTS LibSourcey_DEBUG_LIBS)
  #  target_link_libraries(${name} debug ${lib})
  #endforeach()
  #foreach(lib IN LISTS LibSourcey_RELEASE_LIBS)
  #  target_link_libraries(${name} optimized ${lib})
  #endforeach()
    
  #foreach(module ${ARGN})
  #  set(lib_name "Sourcey${module}${LibSourcey_DLLVERSION}")
  #  if (IS_DIRECTORY "${CMAKE_SOURCE_DIR}/modules/${module}/include")
  #    include_directories("${CMAKE_SOURCE_DIR}/modules/${module}/include")
  #  elseif(IS_DIRECTORY "${CMAKE_SOURCE_DIR}/projects/${module}/include")
  #    include_directories("${CMAKE_SOURCE_DIR}/projects/${module}/include")
  #  else()
  #     message("Unable to include ${module} in ${name}")
  #  endif()
  #endforeach()
