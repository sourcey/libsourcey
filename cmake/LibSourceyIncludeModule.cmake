#
### Macro: include_modules
#
# Generic inclusion of LibSourcey module(s) into a project.
#
macro(include_modules)
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
