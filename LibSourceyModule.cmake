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

  add_library(${name} ${lib_srcs} ${lib_hdrs} ${lib_int_hdrs})     
  
  # Include modules first for the benefit of compilers which
  # require ordered link libraries.
  foreach(module ${ARGN})
    include_libsourcey_modules(${module})  
    add_dependencies(${name} ${module})
  endforeach()  

  # Add external dependencies and required libraries for linking.
  target_link_libraries(${name} ${LibSourcey_INCLUDE_LIBRARIES})
  add_dependencies(${name} ${LibSourcey_INCLUDE_LIBRARIES})   
  
  include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")  
  message(STATUS "LibSourcey_INCLUDE_DIRS: ${LibSourcey_INCLUDE_DIRS}")
  include_directories(${LibSourcey_INCLUDE_DIRS})
  link_directories(${LibSourcey_LIBRARY_DIRS})

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
