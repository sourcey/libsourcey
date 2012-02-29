#
### Macro: include_libsourcey_modules
#
# Generic inclusion of LibSourcey module(s) into a project.
#
macro(include_libsourcey_modules)
  foreach(name ${ARGN})
    message("Including module: ${name}")
    
    if(IS_DIRECTORY "${CMAKE_SOURCE_DIR}/srcs/${name}/include")
      include_directories("${CMAKE_SOURCE_DIR}/srcs/${name}/include")    
    elseif(IS_DIRECTORY "${CMAKE_SOURCE_DIR}/srcs/Anionu/${name}/include")
      include_directories("${CMAKE_SOURCE_DIR}/srcs/Anionu/${name}/include")
    else()
       message("Unable to include ${name} headers")
    endif()
        
    set(lib_name "Sourcey${name}${LibSourcey_DLLVERSION}")
        
    # Create a Debug and a Release list for MSVC        
    if (MSVC)
      find_library(LibSourcey_RLS_${name} "${lib_name}" LibSourcey_LIBRARY_DIR)
      find_library(LibSourcey_DBG_${name} "${lib_name}d" LibSourcey_LIBRARY_DIR)
      if(CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)            
        if (LibSourcey_RLS_${name}) 
          list(APPEND LibSourcey_INCLUDE_LIBRARIES "optimized" ${LibSourcey_RLS_${name}})
        endif()
        if (LibSourcey_DBG_${name})
          list(APPEND LibSourcey_INCLUDE_LIBRARIES "debug" ${LibSourcey_DBG_${name}})
        endif()
      else()    
        if (LibSourcey_RLS_${name}) 
          list(APPEND LibSourcey_INCLUDE_LIBRARIES ${LibSourcey_RLS_${name}})
        endif()
      endif()
    else()
      find_library(LibSourcey_LIB_${name} "${lib_name}" LibSourcey_LIBRARY_DIR)
      if (LibSourcey_LIB_${name})
        # Prepend module libraries otherwise linking will fail
        # on compilers that require ordering of link libraries.
        set(LibSourcey_INCLUDE_LIBRARIES ${LibSourcey_LIB_${name}} ${LibSourcey_INCLUDE_LIBRARIES})
      endif()
    endif()
  endforeach()
endmacro()
        
    #list(APPEND LibSourcey_INCLUDE_LIBRARIES ${LibSourcey_LIB_${name}})
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