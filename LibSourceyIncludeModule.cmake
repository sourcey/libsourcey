# ----------------------------------------------------------------------
# This macro allows inclusion of LibSourcey module(s) into a project
# ----------------------------------------------------------------------
macro(include_libsourcey_modules)
  foreach(name ${ARGN})
    message("Including module: ${name}")
    
    if(IS_DIRECTORY "${CMAKE_SOURCE_DIR}/modules/${name}/include")
      include_directories("${CMAKE_SOURCE_DIR}/modules/${name}/include")    
    elseif(IS_DIRECTORY "${CMAKE_SOURCE_DIR}/projects/Anionu/${name}/include")
      include_directories("${CMAKE_SOURCE_DIR}/projects/Anionu/${name}/include")
    elseif(IS_DIRECTORY "${CMAKE_SOURCE_DIR}/projects/${name}/include")
      include_directories("${CMAKE_SOURCE_DIR}/projects/${name}/include")
    else()
       message("Unable to include ${name} headers")
    endif()
        
    set(lib_name "Sourcey${name}${LibSourcey_DLLVERSION}")
    
    if(MSVC) 
      if(CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)  
        list(APPEND LibSourcey_INCLUDE_LIBRARIES "debug" "${lib_name}d.lib")
        list(APPEND LibSourcey_INCLUDE_LIBRARIES "optimized" "${lib_name}.lib")      
      else()  
        list(APPEND LibSourcey_INCLUDE_LIBRARIES "${lib_name}.lib")          
      endif()     
    else()           
    endif()   
          
    #if(EXISTS "${LibSourcey_INSTALL_DIR}/lib/${lib_name}d.lib")
    #  list(APPEND LibSourcey_DEBUG_LIBS "${lib_name}d.lib")  
    #elseif(EXISTS "${LibSourcey_INSTALL_DIR}/lib/${lib_name}.lib")
    #  list(APPEND LibSourcey_RELEASE_LIBS "${lib_name}.lib")  
    #else()
    #   message("Unable to include ${name} library: ${lib_name}")
    #endif()
  endforeach()
endmacro()