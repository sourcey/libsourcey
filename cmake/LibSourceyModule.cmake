#
### Macro: ask_build_sourcey_module
#
# Optionally build a LibSourcey module.
# This should be called before include_dependency and
# define_sourcey_module for each module.
#
macro(ask_build_sourcey_module name)
  if(BUILD_MODULES)
    set(BUILD_MODULE_${name} ON CACHE BOOL "Build LibSourcey module: ${name}")
    if(BUILD_MODULE_${name}) 
      mark_as_advanced(FORCE BUILD_MODULE_${name})  
    endif()  
  endif()  
endmacro()


#
### Macro: include_sourcey_modules
#
# Generic inclusion of LibSourcey module(s) into a project.
#
macro(include_sourcey_modules)
  foreach(name ${ARGN})
    # message(STATUS "Including module: ${name}")
    
    # Include the module headers. 
    # These may be located in the "libs/" root directory,
    # or in a sub directory.
    set(HAVE_LIBSOURCEY_${name} 0)
    if(IS_DIRECTORY "${CMAKE_SOURCE_DIR}/libs/${name}/include")
      include_directories("${CMAKE_SOURCE_DIR}/libs/${name}/include")   
      set(HAVE_LIBSOURCEY_${name} 1)
    else()       
      subdirlist(subdirs "${CMAKE_SOURCE_DIR}/libs")
      foreach(dir ${subdirs})
        set(dir "${CMAKE_SOURCE_DIR}/libs/${dir}/${name}/include")
        if(IS_DIRECTORY ${dir})
          include_directories(${dir})
          set(HAVE_LIBSOURCEY_${name} 1)
        endif()
      endforeach()       
    endif()
    
    if (NOT HAVE_LIBSOURCEY_${name})
       message(FATAL_ERROR "Unable to include dependent LibSourcey module: ${name}")
    endif()
        
    set(lib_name "Sourcey${name}${LibSourcey_DLLVERSION}")
        
    # Create a Debug and a Release list for MSVC        
    if (MSVC)
      find_library(LibSourcey_${name}_RELEASE "${lib_name}" LibSourcey_LIBRARY_DIR)
      find_library(LibSourcey_${name}_DEBUG "${lib_name}d" LibSourcey_LIBRARY_DIR)
      if(CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)            
        if (LibSourcey_${name}_RELEASE) 
          list(APPEND LibSourcey_INCLUDE_LIBRARIES "optimized" ${LibSourcey_${name}_RELEASE})
          mark_as_advanced(LibSourcey_${name}_RELEASE)
        endif()
        if (LibSourcey_${name}_DEBUG)
          list(APPEND LibSourcey_INCLUDE_LIBRARIES "debug" ${LibSourcey_${name}_DEBUG})
          mark_as_advanced(LibSourcey_${name}_DEBUG)
        endif()
      else()    
        if (LibSourcey_${name}_RELEASE) 
          list(APPEND LibSourcey_INCLUDE_LIBRARIES ${LibSourcey_${name}_RELEASE})
          mark_as_advanced(LibSourcey_${name}_RELEASE)
        endif()
        if (LibSourcey_${name}_DEBUG)
          mark_as_advanced(LibSourcey_${name}_DEBUG)
        endif()
      endif()      
    else()
      find_library(LibSourcey_${name} "${lib_name}" LibSourcey_LIBRARY_DIR)
      if (LibSourcey_${name})
        # Prepend module libraries otherwise linking will fail
        # on compilers that require ordering of link libraries.
        set(LibSourcey_INCLUDE_LIBRARIES ${LibSourcey_${name}} ${LibSourcey_INCLUDE_LIBRARIES})
        mark_as_advanced(LibSourcey_${name})
      endif()
    endif()
  endforeach()
endmacro()


#
### Macro: define_sourcey_module
#
# Defines a generic LibSourcey module.
#
macro(define_sourcey_module name)       
    
  project(${name})

  # Add library source files
  file(GLOB_RECURSE lib_srcs "src/*.c*")
  file(GLOB_RECURSE lib_hdrs "include/*.h*")
  
  # Exclude platform dependent implementations
  set(lib_srcs_exclude "")
  set(lib_hdrs_exclude "")
  if(NOT WIN32)
    set(lib_srcs_exclude ${lib_srcs_exclude} " *_WIN32.cpp")  
    set(lib_hdrs_exclude ${lib_hdrs_exclude} " *_WIN32.h")  
  endif()  
  if(NOT LINUX)
    set(lib_srcs_exclude ${lib_srcs_exclude} " *_LINUX.cpp")  
    set(lib_hdrs_exclude ${lib_hdrs_exclude} " *_LINUX.h")  
  endif()  
  if(NOT APPLE)
    set(lib_srcs_exclude ${lib_srcs_exclude} " *_MAC.cpp")  
    set(lib_hdrs_exclude ${lib_hdrs_exclude} " *_MAC.h")  
  endif()
  JOIN("${lib_srcs_exclude}" "|" lib_srcs_exclude)
  JOIN("${lib_hdrs_exclude}" "|" lib_hdrs_exclude)
  
  #STRING(CHOP ${lib_srcs_exclude} 1 lib_srcs_exclude) 
  #STRING(CHOP ${lib_hdrs_exclude} 1 lib_hdrs_exclude) 
  
  #string(REGEX REPLACE " *_WIN32.cpp| *_LINUX.cpp| *_UNIX.cpp| *_MAC.cpp| *_NULL.cpp" "" lib_srcs "${lib_srcs}")
  #string(REGEX REPLACE " *_WIN32.h| *_LINUX.h| *_UNIX.h| *_MAC.h| *_FAKE.h" "" lib_hdrs "${lib_hdrs}")
  #message(${lib_srcs_exclude} )
  #message(${lib_hdrs_exclude} )
  string(REGEX REPLACE ${lib_srcs_exclude} "" lib_srcs "${lib_srcs}")
  string(REGEX REPLACE ${lib_hdrs_exclude} "" lib_hdrs "${lib_hdrs}")
  
  source_group("Src" FILES ${lib_srcs})
  source_group("Include" FILES ${lib_hdrs})

  add_library(${name} ${lib_srcs} ${lib_hdrs})     
      
  # Set HAVE_LIBSOURCEY_XXX at parent scope for inclusion
  # into our Config.h
  set(HAVE_LIBSOURCEY_${name} ON PARENT_SCOPE)
    
  # Include dependent modules
  foreach(module ${ARGN})
    include_sourcey_modules(${module})  
    add_dependencies(${name} ${module})
  endforeach()  

  # Include external dependencies
  target_link_libraries(${name} ${LibSourcey_INCLUDE_LIBRARIES})
  add_dependencies(${name} ${LibSourcey_INCLUDE_LIBRARIES}) # requires complete recompile on some systems
  
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
  
  # Build samples
  if(BUILD_MODULE_SAMPLES AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/samples)
    add_subdirectory(samples)
  endif() 
  
  # Build tests
  if(BUILD_MODULE_TESTS AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tests)
    add_subdirectory(tests)
  endif()    
       
endmacro()


#
### Macro: define_sourcey_module_sample
#
# Defines a generic LibSourcey sample application.
#
macro(define_sourcey_module_sample name)
            
    project(${name})    

    # Add example source files
    file(GLOB lib_hdrs "*.h*")
    file(GLOB lib_srcs "*.cpp")
    
    source_group("Src" FILES ${lib_srcs})
    source_group("Include" FILES ${lib_hdrs})
  
    add_executable(${name} ${lib_srcs} ${lib_hdrs})

    # Include dependent modules
    foreach(module ${ARGN})
      include_sourcey_modules(${module})  
      #add_dependencies(${name} ${module})
    endforeach()  

    # Include external dependencies
    target_link_libraries(${name} ${LibSourcey_INCLUDE_LIBRARIES})
    add_dependencies(${name} ${LibSourcey_INCLUDE_LIBRARIES}) # requires complete recompile on some systems

    # Include library and header directories
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")  
    include_directories(${LibSourcey_INCLUDE_DIRS})
    link_directories(${LibSourcey_LIBRARY_DIRS})  
     
    set_target_properties(${name} PROPERTIES DEBUG_POSTFIX "d")
        
    #foreach(lib ${ARGN})
    #    target_link_libraries(${name} debug "Sourcey${lib}${LibSourcey_DLLVERSION}d")
    #    target_link_libraries(${name} optimized "Sourcey${lib}${LibSourcey_DLLVERSION}")
    #endforeach()
    #foreach(lib IN LISTS LibSourcey_DEBUG_LIBS)
    #    target_link_libraries(${name} debug ${lib})
    #endforeach()
    #foreach(lib IN LISTS LibSourcey_RELEASE_LIBS)
    #    target_link_libraries(${name} optimized ${lib})
    #endforeach()

    install(TARGETS ${name} RUNTIME DESTINATION "samples/${name}" COMPONENT main) 
        
endmacro()

#
### Macro: define_libsourcey_test
#
# Defines a generic LibSourcey test application.
#
macro(define_libsourcey_test name)
                    
    project(${name})    

    # Add source files
    file(GLOB lib_hdrs "*.h*")
    file(GLOB lib_srcs "*.cpp")
    
    source_group("Src" FILES ${lib_srcs})
    source_group("Include" FILES ${lib_hdrs})
  
    add_executable(${name} ${lib_srcs} ${lib_hdrs})

    # Include dependent modules
    foreach(module ${ARGN})
      include_sourcey_modules(${module})  
      #add_dependencies(${name} ${module})
    endforeach()  

    # Include external dependencies
    target_link_libraries(${name} ${LibSourcey_INCLUDE_LIBRARIES})
    add_dependencies(${name} ${LibSourcey_INCLUDE_LIBRARIES}) # requires complete recompile on some systems

    # Include library and header directories
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")  
    include_directories(${LibSourcey_INCLUDE_DIRS})
    link_directories(${LibSourcey_LIBRARY_DIRS})  
     
    set_target_properties(${name} PROPERTIES DEBUG_POSTFIX "d")

    install(TARGETS ${name} RUNTIME DESTINATION "tests/${name}" COMPONENT main) 
    
endmacro()


            
    #project(${name})    

    # Add example source files
    #file(GLOB lib_hdrs "*.h*")
    #file(GLOB lib_srcs "*.cpp")
    
    #source_group("Src" FILES ${lib_srcs})
    #source_group("Include" FILES ${lib_hdrs})

    #add_executable(${name} ${lib_srcs} ${lib_hdrs})
     
    #set_target_properties(${name} PROPERTIES DEBUG_POSTFIX "d")
        
    #foreach(lib ${ARGN})
    #    target_link_libraries(${name} debug "Sourcey${lib}${LibSourcey_DLLVERSION}d")
    #    target_link_libraries(${name} optimized "Sourcey${lib}${LibSourcey_DLLVERSION}")
    #endforeach()
    #foreach(lib IN LISTS LibSourcey_DEBUG_LIBS)
    #    target_link_libraries(${name} debug ${lib})
    #endforeach()
    #foreach(lib IN LISTS LibSourcey_RELEASE_LIBS)
    #    target_link_libraries(${name} optimized ${lib})
    #endforeach()

    #install(TARGETS ${name} RUNTIME DESTINATION "tests/${name}" COMPONENT main) 




        
    
      #if(IS_DIRECTORY "${CMAKE_SOURCE_DIR}/libs/Anionu/${name}/include")        
      #message(STATUS "Including Module: ${module}")
      #if (EXISTS "${CMAKE_SOURCE_DIR}/libs/${module}/CMakeLists.txt")      
      #  add_subdirectory("${CMAKE_SOURCE_DIR}/libs/${module}")
      #endif()
    #list(APPEND LibSourcey_INCLUDE_LIBRARIES ${LibSourcey_${name}})
    #if(MSVC) 
    #  if(CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)  
    #    list(APPEND LibSourcey_INCLUDE_LIBRARIES "debug" "${lib_name}d.lib")
    #    list(APPEND LibSourcey_INCLUDE_LIBRARIES "optimized" "${lib_name}.lib")      
    #  else()  
    #    list(APPEND LibSourcey_INCLUDE_LIBRARIES "${lib_name}.lib")          
    #  endif()     
    #else()           
    #endif()   
          
    #if(EXISTS "${LibSourcey_INSTALL_DIR}/lib/${lib_name}d.lib")
    #  list(APPEND LibSourcey_DEBUG_LIBS "${lib_name}d.lib")  
    #elseif(EXISTS "${LibSourcey_INSTALL_DIR}/lib/${lib_name}.lib")
    #  list(APPEND LibSourcey_RELEASE_LIBS "${lib_name}.lib")  
    #else()
    #   message("Unable to include ${name} library: ${lib_name}")
    #endif()
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
