#
### Macro: define_sourcey_dependency
#
# This template defines a LibSourcey dependency.
# Optional helper variables:
#   <name>_OUTPUT_NAME - The name of the executable file
#   <name>_DEBUG_POSTFIX - The output file name debum postfix
#   <name>_SOURCE_PATH - The glob source path
#   <name>_HEADER_PATH - The glob header path
#   <name>_SOURCE_FILES - The glob source files
#   <name>_HEADER_FILES - The glob header files
#
macro(define_sourcey_dependency name)
            
  project(${name})
  
  # Add library source files
  if (NOT ${name}_SOURCE_FILES)  
    if (NOT ${name}_SOURCE_PATH)      
      set(${name}_SOURCE_PATH "src/*.c*")
    endif() 
    file(GLOB_RECURSE ${name}_SOURCE_FILES ${${name}_SOURCE_PATH})
    #message("${name}: Globbing Sourcey Files: ${${name}_SOURCE_FILES}")  
  endif() 
  #message("${name}: Sourcey Files: ${${name}_SOURCE_FILES}")  
  
  # Add library header files
  if (NOT ${name}_HEADER_FILES)
    if (NOT ${name}_HEADER_PATH)
      set(${name}_HEADER_PATH "src/*.h*")
    endif()   
    file(GLOB_RECURSE ${name}_HEADER_FILES ${${name}_HEADER_PATH})
  endif() 
  
  source_group("Source" FILES ${${name}_SOURCE_FILES})
  source_group("Include" FILES ${${name}_HEADER_FILES})
      
  add_library(${name} STATIC ${${name}_SOURCE_FILES} ${${name}_HEADER_FILES})
  
  if (${name}_DEPENDENCIES)
    add_dependencies(${name} ${${name}_DEPENDENCIES})
  endif()
  
  #message(STATUS "- Linking dependency ${name} with libraries: ${LibSourcey_INCLUDE_LIBRARIES}")    
  #message("${name}: Library Dirs: ${LibSourcey_LIBRARY_DIRS}")    
  #message("${name}: Include Dirs: ${LibSourcey_INCLUDE_DIRS}")

  if (NOT ${name}_OUTPUT_NAME)
    set(${name}_OUTPUT_NAME ${name})
  endif()
  if (NOT ${name}_DEBUG_POSTFIX)
    set(${name}_DEBUG_POSTFIX ${LibSourcey_DEBUG_POSTFIX})
  endif()
  if(ENABLE_SOLUTION_FOLDERS)
    set_target_properties(${name} PROPERTIES FOLDER "dependencies")
  endif()
  set_target_properties(${name} PROPERTIES
    OUTPUT_NAME ${${name}_OUTPUT_NAME}
    DEBUG_POSTFIX ${${name}_DEBUG_POSTFIX})
  
  if (NOT INSTALL_DESTINATION)
    set(INSTALL_DESTINATION ${LibSourcey_DEPENDENCIES_INSTALL_DIR}/lib) 
  endif()
  install(TARGETS ${name}
    ARCHIVE DESTINATION ${INSTALL_DESTINATION} COMPONENT main) 
        
endmacro()



  # Add external dependencies and required libraries for linking.
  #target_link_libraries(${name} ${LibSourcey_INCLUDE_LIBRARIES})   
  
    # Include all header and library directories
    #include_directories(${LibSourcey_INCLUDE_DIRS})
    #link_directories(${LibSourcey_LIBRARY_DIRS})   
  
    # Setting WIN32 sets SUBSYSTEM:WINDOWS
    #if (${name}_SUBSYSTEM_WINDOWS)
    #  add_executable(${name} WIN32 ${${name}_SOURCE_FILES} ${${name}_HEADER_FILES})
    #else()
    #  add_executable(${name} ${${name}_SOURCE_FILES} ${${name}_HEADER_FILES})
    #endif()
    
    # Include modules libraries first for the benefit of compilers
    # which require ordered link libraries.
    #foreach(module ${ARGN})
    #  include_sourcey_modules(${module})  
    #  add_dependencies(${name} ${module})
    #endforeach()