#
### Macro: define_sourcey_dependency
#
# This template defines a LibSourcey dependency.
# Optional helper variables:
#   <name>_OUTPUT_NAME - The name of the executable file
#   <name>_DEBUG_POSTFIX - The output file name debug postfix
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
    #message("${name}: Globbing source files: ${${name}_SOURCE_FILES}")  
  endif() 
  #message("${name}: Sourcey files: ${${name}_SOURCE_FILES}")  
  
  # Add library header files
  if (NOT ${name}_HEADER_FILES)
    if (NOT ${name}_HEADER_PATH)
      set(${name}_HEADER_PATH "src/*.h*")
    endif()   
    file(GLOB_RECURSE ${name}_HEADER_FILES ${${name}_HEADER_PATH})
  endif() 
  
  source_group("Source" FILES ${${name}_SOURCE_FILES})
  source_group("Include" FILES ${${name}_HEADER_FILES})
      
  add_library(${name} ${LibSourcey_LIB_TYPE} ${${name}_SOURCE_FILES} ${${name}_HEADER_FILES})
  
  if (${name}_DEPENDENCIES)
    add_dependencies(${name} ${${name}_DEPENDENCIES})
  endif()
  
  # Include current directory and existing dependency directories
  include_directories("${CMAKE_CURRENT_SOURCE_DIR}" "${LibSourcey_INCLUDE_DIRS}")
  
  # Cache dependency directories for inclusion by modules and applications
  get_directory_property(lib_directories INCLUDE_DIRECTORIES)
  set(LibSourcey_INCLUDE_DIRS ${lib_directories} PARENT_SCOPE)
  set(LibSourcey_INCLUDE_LIBRARIES ${LibSourcey_INCLUDE_LIBRARIES} ${name} PARENT_SCOPE)
  
  #message(STATUS "- Linking dependency ${name} with libraries: ${LibSourcey_INCLUDE_LIBRARIES}")    
  #message("${name}: Library Dirs: ${LibSourcey_LIBRARY_DIRS}")    
  #message("${name}: Include Dirs: ${LibSourcey_INCLUDE_DIRS}")  

  if (${name}_OUTPUT_NAME)  	
    set_target_properties(${name} PROPERTIES OUTPUT_NAME ${${name}_OUTPUT_NAME})
  endif()
  if (${name}_DEBUG_POSTFIX)
    set_target_properties(${name} PROPERTIES DEBUG_POSTFIX ${${name}_DEBUG_POSTFIX})
  elseif(LibSourcey_DEBUG_POSTFIX)    
    set_target_properties(${name} PROPERTIES DEBUG_POSTFIX ${LibSourcey_DEBUG_POSTFIX})
  endif()
  if(ENABLE_SOLUTION_FOLDERS)
    set_target_properties(${name} PROPERTIES FOLDER "dependencies")
  endif()
  #if(CMAKE_C_FLAGS)
  #  set_target_properties(${name} PROPERTIES COMPILE_FLAGS ${CMAKE_C_FLAGS})
  #endif()

  # message("${name}: OUTPUT_NAME: ${name}_OUTPUT_NAME")
  # message("${name}: DEBUG_POSTFIX: ${name}_DEBUG_POSTFIX")  
  # message("${name}: CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

  #set_target_properties(${name} PROPERTIES
  #  OUTPUT_NAME ${${name}_OUTPUT_NAME}
  #  DEBUG_POSTFIX ${${name}_DEBUG_POSTFIX})
  
  if (NOT INSTALL_DESTINATION)
    set(INSTALL_DESTINATION ${LibSourcey_DEPENDENCIES_INSTALL_DIR}/lib) 
  endif()
  install(TARGETS ${name}
    RUNTIME DESTINATION ${INSTALL_DESTINATION} COMPONENT main   
    ARCHIVE DESTINATION ${INSTALL_DESTINATION} COMPONENT main
    LIBRARY DESTINATION ${INSTALL_DESTINATION} COMPONENT main) 
        
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