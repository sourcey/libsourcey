#
### Macro: include_dependency
#
# Inclusion of 3rd party dependency into a LibSourcey project.
#
macro(include_dependency name)
  #message(STATUS "Including dependency: ${name}")
  
  find_package(${name} ${ARGN})  
    
  # Determine the variable scope
  set(var_root ${name})  
  set(lib_found 0)
  string(TOUPPER ${var_root} var_root_upper)
  if(${var_root}_FOUND)
    set(lib_found 1)
  else()
    # Try to use old style uppercase variable accessor
    if(${var_root_upper}_FOUND)
      set(var_root ${var_root_upper})  
      set(lib_found 1)
    endif()
  endif()  
  
  # Exit message on failure
  if (NOT ${var_root}_FOUND)
    message("Failed to include dependency: ${name}. Please build LibSourcey dependencies first by enabling BUILD_DEPENDENCIES and disabling BUILD_MODULES and BUILD_APPLICATIONS") 
    return()
  endif()
  
  # Set a HAVE_XXX variable at parent scope for our Config.h
  set(HAVE_${var_root_upper} TRUE PARENT_SCOPE)      
  # Expose to LibSourcey
  if(${var_root}_INCLUDE_DIR)
    #message(STATUS "- Found ${name} Inc Dir: ${${var_root}_INCLUDE_DIR}")
    #include_directories(${${var_root}_INCLUDE_DIR})
    list(APPEND LibSourcey_INCLUDE_DIRS ${${var_root}_INCLUDE_DIR})
  endif()
  if(${var_root}_INCLUDE_DIRS)
    #message(STATUS "- Found ${name} Inc Dirs: ${${var_root}_INCLUDE_DIRS}")
    #include_directories(${${var_root}_INCLUDE_DIRS})
    list(APPEND LibSourcey_INCLUDE_DIRS ${${var_root}_INCLUDE_DIRS})    
  endif()
  if(${var_root}_LIBRARY_DIR)
    #message(STATUS "- Found ${name} Lib Dir: ${${var_root}_LIBRARY_DIR}")
    #link_directories(${${var_root}_LIBRARY_DIR})
    list(APPEND LibSourcey_LIBRARY_DIRS ${${var_root}_LIBRARY_DIR})
    #list(REMOVE_DUPLICATES LibSourcey_LIBRARY_DIRS)
  endif()
  if(${var_root}_LIBRARY_DIRS)
    #message(STATUS "- Found ${name} Lib Dirs: ${${var_root}_LIBRARY_DIRS}")
    #link_directories(${${var_root}_LIBRARY_DIRS})
    list(APPEND LibSourcey_LIBRARY_DIRS ${${var_root}_LIBRARY_DIRS})
    #list(REMOVE_DUPLICATES LibSourcey_LIBRARY_DIRS)
  endif()
  if(${var_root}_LIBRARY)
    #message(STATUS "- Found ${name} Lib: ${${var_root}_LIBRARY}")
    list(APPEND LibSourcey_INCLUDE_LIBRARIES ${${var_root}_LIBRARY})
  endif()
  if(${var_root}_LIBRARIES)
    #message(STATUS "- Found ${name} Libs: ${${var_root}_LIBRARIES}")
    list(APPEND LibSourcey_INCLUDE_LIBRARIES ${${var_root}_LIBRARIES})
  endif()
endmacro()


#
### Macro: include_sourcey_modules
#
# Inclusion of LibSourcey module(s) into a project.
#
macro(include_sourcey_modules)
  foreach(name ${ARGN})
    # message(STATUS "Including module: ${name}")
    
    # Include the module headers. 
    # These may be located in the "src/" root directory,
    # or in a sub directory.
    set(HAVE_SOURCEY_${name} 0)
    if(IS_DIRECTORY "${LibSourcey_SOURCE_DIR}/${name}/include")
      include_directories("${LibSourcey_SOURCE_DIR}/${name}/include")   
      set(HAVE_SOURCEY_${name} 1)
    else()       
      subdirlist(subdirs "${LibSourcey_SOURCE_DIR}")
      foreach(dir ${subdirs})
        set(dir "${LibSourcey_SOURCE_DIR}/${dir}/${name}/include")
        if(IS_DIRECTORY ${dir})
          include_directories(${dir})
          set(HAVE_SOURCEY_${name} 1)
        endif()
      endforeach()       
    endif()
    
    if (NOT HAVE_SOURCEY_${name})
       message(ERROR "Unable to include dependent LibSourcey module ${name}. The build may fail.")
    endif()
        
    set(lib_name "Sourcey${name}${LibSourcey_DLLVERSION}")
    
    # Create a Debug and a Release list for MSVC        
    if (MSVC)    
      
      # Find the module giving priority to the build output folder, and install folder second.
      # This way we don't have to install updated modules before building applications
      # simplifying the build process. 
    
      # Always reset the module folders since.
      set(LibSourcey_${name}_RELEASE LibSourcey_${name}_RELEASE-NOTFOUND)
      set(LibSourcey_${name}_RELEASE LibSourcey_${name}_RELEASE-NOTFOUND PARENT_SCOPE)
      set(LibSourcey_${name}_DEBUG LibSourcey_${name}_DEBUG-NOTFOUND)
      set(LibSourcey_${name}_DEBUG LibSourcey_${name}_DEBUG-NOTFOUND PARENT_SCOPE)  
      
    
      # Since CMake doesn't give priority to the PATHS parameter, we need to search twice:
      # once using NO_DEFAULT_PATH, and once using default values.         
      find_library(LibSourcey_${name}_RELEASE "${lib_name}" 
        PATHS ${LibSourcey_BUILD_DIR}/src/${name}
        PATH_SUFFIXES Release
        NO_DEFAULT_PATH)
      find_library(LibSourcey_${name}_DEBUG "${lib_name}d" 
        PATHS ${LibSourcey_BUILD_DIR}/src/${name}
        PATH_SUFFIXES Debug
        NO_DEFAULT_PATH)        
      
      # Search the module install folder if none was located.
      find_library(LibSourcey_${name}_RELEASE "${lib_name}")
      find_library(LibSourcey_${name}_DEBUG "${lib_name}d")
      
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
      # Find the module giving preference to the build output folder.
      find_library(LibSourcey_${name} "${lib_name}" 
        PATHS ${LibSourcey_BUILD_DIR}/src/${name}
        NO_DEFAULT_PATH)
      find_library(LibSourcey_${name} "${lib_name}")
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
### Macro: set_component_alias
#
# Sets the current alias variables.
#
macro(set_component_alias module component)
  set(ALIAS                    ${module}_${component})
  set(ALIAS_FOUND              ${ALIAS}_FOUND)
  set(ALIAS_LIBRARIES          ${ALIAS}_LIBRARIES)
  set(ALIAS_RELEASE_LIBRARIES  ${ALIAS}_RELEASE_LIBRARIES)
  set(ALIAS_DEBUG_LIBRARIES    ${ALIAS}_DEBUG_LIBRARIES)
  set(ALIAS_INCLUDE_DIRS       ${ALIAS}_INCLUDE_DIRS)
  set(ALIAS_LIBRARY_DIRS       ${ALIAS}_LIBRARY_DIRS)
  set(ALIAS_DEFINITIONS        ${ALIAS}_CFLAGS_OTHER)        
  set(ALIAS_VERSION            ${ALIAS}_VERSION)
endmacro()


#
### Macro: set_module_found
#
# Marks the given module as found all component are present.
#
macro(set_module_found module) 
  
  set(${module}_FOUND FALSE)
  set(${module}_FOUND FALSE PARENT_SCOPE)

  # Compile the list of required vars
  set(_${module}_REQUIRED_VARS ${module}_LIBRARIES ${module}_INCLUDE_DIRS)
  foreach (component ${${module}_FIND_COMPONENTS})
    list(APPEND _${module}_REQUIRED_VARS ${module}_${component}_LIBRARIES ${module}_${component}_INCLUDE_DIRS)
    #if (${module}_${component}_FOUND)
    #  message(STATUS "Required ${module} component ${component} present.")
    #else ()
    #  message(STATUS "Required ${module} component ${component} missing.")
    #endif ()
  endforeach ()

  # Cache the vars.
  set(${module}_INCLUDE_DIRS ${${module}_INCLUDE_DIRS} CACHE STRING   "The ${module} include directories." FORCE)
  set(${module}_LIBRARY_DIRS ${${module}_LIBRARY_DIRS} CACHE STRING   "The ${module} library directories." FORCE)
  set(${module}_LIBRARIES    ${${module}_LIBRARIES}    CACHE STRING   "The ${module} libraries." FORCE)
  set(${module}_FOUND        ${${module}_FOUND}        CACHE BOOLEAN  "The ${module} found status." FORCE)  
  set(${module}_INCLUDE_DIRS ${${module}_INCLUDE_DIRS} PARENT_SCOPE)
  set(${module}_LIBRARY_DIRS ${${module}_LIBRARY_DIRS} PARENT_SCOPE)
  set(${module}_LIBRARIES    ${${module}_LIBRARIES}    PARENT_SCOPE)
  set(${module}_FOUND        ${${module}_FOUND}        PARENT_SCOPE)

  if (${module}_FIND_REQUIRED)
    # Give a nice error message if some of the required vars are missing.
    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(${module} DEFAULT_MSG ${_${module}_REQUIRED_VARS})
  else() 
    message("Failed to locate ${module}. Please specify paths manually.")  
  endif() 

  set(${module}_FOUND TRUE)
  set(${module}_FOUND TRUE PARENT_SCOPE)

  mark_as_advanced(${module}_INCLUDE_DIRS
                   ${module}_LIBRARY_DIRS
                   ${module}_LIBRARIES
                   ${module}_DEFINITIONS
                   ${module}_FOUND)
  
  #message("Module Found=${module}")    

endmacro()


#
### Macro: set_component_found
#
# Marks the given component as found if both *_LIBRARIES AND *_INCLUDE_DIRS is present.
#
macro(set_component_found module component)  

  set_component_alias(${module} ${component})

  if (${module}_${component}_LIBRARIES AND ${module}_${component}_INCLUDE_DIRS)
    # message(STATUS "  - ${module} ${component} found.")
    set(${ALIAS_FOUND} TRUE PARENT_SCOPE)
  
    # Add component vars to the perant module lists
    append_unique_list(${module}_INCLUDE_DIRS ${ALIAS_INCLUDE_DIRS})    
    append_unique_list(${module}_LIBRARY_DIRS ${ALIAS_LIBRARY_DIRS})   
    append_unique_list(${module}_LIBRARIES    ${ALIAS_LIBRARIES})    
    append_unique_list(${module}_DEFINITIONS  ${ALIAS_DEFINITIONS})

    set(${module}_INCLUDE_DIRS ${${module}_INCLUDE_DIRS} PARENT_SCOPE)      
    set(${module}_LIBRARY_DIRS ${${module}_LIBRARY_DIRS} PARENT_SCOPE)  
    set(${module}_LIBRARIES    ${${module}_LIBRARIES}    PARENT_SCOPE)           
    set(${module}_DEFINITIONS  ${${module}_DEFINITIONS}  PARENT_SCOPE)   
    
    #message("Find Component Paths=${module}:${component}:${library}:${header}")
    #message("${ALIAS_INCLUDE_DIRS}=${${ALIAS_INCLUDE_DIRS}}")  
    #message("${ALIAS_RELEASE_LIBRARIES}=${${ALIAS_RELEASE_LIBRARIES}}")  
    #message("${ALIAS_DEBUG_LIBRARIES}=${${ALIAS_DEBUG_LIBRARIES}}")  
    #message("${ALIAS_LIBRARIES}=${${ALIAS_LIBRARIES}}")
    #message("${module}_INCLUDE_DIRS=${${module}_INCLUDE_DIRS}") 
    #message("${module}_LIBRARIES=${${module}_LIBRARIES}")     

    mark_as_advanced(
      ${ALIAS_INCLUDE_DIRS}
      ${ALIAS_LIBRARY_DIRS})

  else ()
     #message(STATUS "  - ${module} ${component} not found.")
  endif ()

  mark_as_advanced(
    ${ALIAS_DEBUG_LIBRARIES}
    ${ALIAS_RELEASE_LIBRARIES}
    ${ALIAS_LIBRARIES}
    ${ALIAS_DEFINITIONS}
    ${ALIAS_VERSION})

endmacro()


#
### Macro: set_module_notfound
#
# Marks the given component as not found, and resets the cache for find_path and find_library results.
#
macro(set_module_notfound module)

  # message(STATUS "  - ${module} ${component} setting not found.")
  set(${module}_FOUND FALSE)
  set(${module}_FOUND FALSE PARENT_SCOPE)
  
  if (CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)  
    set(${module}_RELEASE_LIBRARIES "")
    set(${module}_DEBUG_LIBRARIES "")
    set(${module}_LIBRARIES "")
    set(${module}_RELEASE_LIBRARIES "" PARENT_SCOPE)
    set(${module}_DEBUG_LIBRARIES "" PARENT_SCOPE)
    set(${module}_LIBRARIES "" PARENT_SCOPE)
  else()    
    set(${module}_LIBRARIES ${ALIAS_LIBRARIES}-NOTFOUND)
    #set(${module}_LIBRARIES ${ALIAS_LIBRARIES}-NOTFOUND PARENT_SCOPE)
  endif()  
    
endmacro()


#
### Macro: set_component_notfound
#
# Marks the given component as not found, and resets the cache for find_path and find_library results.
#
macro(set_component_notfound module component)

  set_component_alias(${module} ${component})

  # message(STATUS "  - ${module} ${component} setting not found.")
  set(${ALIAS_FOUND} FALSE)
  set(${ALIAS_FOUND} FALSE PARENT_SCOPE)
  
  if (${module}_MULTI_CONFIGURATION AND (CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE))
    set(${ALIAS_RELEASE_LIBRARIES} ${ALIAS_RELEASE_LIBRARIES}-NOTFOUND)
    set(${ALIAS_DEBUG_LIBRARIES} ${ALIAS_DEBUG_LIBRARIES}-NOTFOUND)
    set(${ALIAS_LIBRARIES} "") #${module}_${component}_LIBRARIES-NOTFOUND)
    #set(${ALIAS_RELEASE_LIBRARIES} ${ALIAS_RELEASE_LIBRARIES}-NOTFOUND PARENT_SCOPE)
    #set(${ALIAS_DEBUG_LIBRARIES} ${ALIAS_DEBUG_LIBRARIES}-NOTFOUND PARENT_SCOPE)
    #set(${ALIAS_LIBRARIES} "") #${module}_${component}_LIBRARIES-NOTFOUND PARENT_SCOPE)
  else()
    set(${ALIAS_LIBRARIES} ${ALIAS_LIBRARIES}-NOTFOUND)
    #set(${ALIAS_LIBRARIES} ${ALIAS_LIBRARIES}-NOTFOUND PARENT_SCOPE)
  endif()  
    
endmacro()


#
### Macro: find_paths
#
# Finds the given component library and include paths.
#
macro(find_component_paths module component library header)  
  # message(STATUS "Find Component Paths=${module}:${component}:${library}:${header}")
  
  # Reset alias namespace (force recheck)
  #set_component_alias(${module} ${component})  

  # Reset search paths (force recheck)
  #set_component_notfound(${module} ${component})

  find_path(${ALIAS_INCLUDE_DIRS} ${header}
    #HINTS
    #  ${${ALIAS_INCLUDE_DIRS}}    
    PATH_SUFFIXES
      ${${module}_PATH_SUFFIXES}
  )

  # Create a Debug and a Release list for multi configuration builds.
  # NOTE: <module>_CONFIGURATION_TYPES must be set to use this.
  if (${module}_MULTI_CONFIGURATION AND (CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE))  
    find_library(${ALIAS_RELEASE_LIBRARIES} 
      NAMES
        ${library}     
        #lib${library} 
        #lib${library}.so
        #${library}.lib
        #${library}
      #HINTS
      PATHS
        ${${ALIAS_LIBRARY_DIRS}}
    )
    find_library(${ALIAS_DEBUG_LIBRARIES} 
      NAMES
        ${library}d
        #lib${library}d.a       
        #lib${library}d.so
        #${library}d.lib
      #HINTS
      PATHS
        ${${ALIAS_LIBRARY_DIRS}}
    )     
    if (${ALIAS_RELEASE_LIBRARIES})   
      list(APPEND ${ALIAS_LIBRARIES} "optimized" ${${ALIAS_RELEASE_LIBRARIES}})
    endif()
    if (${ALIAS_DEBUG_LIBRARIES})
      list(APPEND ${ALIAS_LIBRARIES} "debug" ${${ALIAS_DEBUG_LIBRARIES}})
    endif()
  else()
    find_library(${ALIAS_LIBRARIES}
      NAMES # setting in order might help overcome find_library bugs :/
        #lib${library}.so     
        #lib${library}.a 
        #${library}.lib
        ${library}
      #HINTS
      PATHS
        ${${ALIAS_LIBRARY_DIRS}}
    )
  endif()  

  set_component_found(${module} ${component})
endmacro()


#
### Macro: find_component
#
# Checks for the given component by invoking pkgconfig and then looking up the libraries and
# include directories.
#
macro(find_component module component pkgconfig library header)	 
  # message("Find Component=${module}:${component}:${pkgconfig}:${library}:${header}")

  # Reset component alias values (force recheck)
  set_component_alias(${module} ${component})

  # Use pkg-config to obtain directories for
  # the FIND_PATH() and find_library() calls.
  find_package(PkgConfig)
  if (PKG_CONFIG_FOUND)
    #set(PKG_ALIAS                   PKG_${component})
    #pkg_check_modules(${PKG_ALIAS}  ${pkgconfig})
    #set(${ALIAS}_FOUND              ${${PKG_ALIAS}_FOUND})
    #set(${ALIAS}_LIBRARIES          ${${PKG_ALIAS}_LIBRARIES})
    #set(${ALIAS}_INCLUDE_DIRS       ${${PKG_ALIAS}_INCLUDE_DIRS})
    #set(${ALIAS}_LIBRARY_DIRS       ${${PKG_ALIAS}_LIBRARY_DIRS})
    #set(${ALIAS}_DEFINITIONS        ${${PKG_ALIAS}_CFLAGS_OTHER})        
    #set(${ALIAS}_VERSION            ${${PKG_ALIAS}_VERSION})

    pkg_search_module(${ALIAS} ${pkgconfig})
    message(STATUS "Find Component PkgConfig=${ALIAS}:${${ALIAS}_FOUND}:${${ALIAS}_LIBRARIES}:${${ALIAS}_INCLUDE_DIRS}:${${ALIAS}_LIBRARY_DIRS}:${${ALIAS}_LIBDIR}:${${ALIAS}_INCLUDEDIR}")
  endif()  
   
  #message(STATUS "${ALIAS_LIBRARIES}=${${ALIAS_LIBRARIES}}")  
  #message(STATUS "${ALIAS_INCLUDE_DIRS}=${${ALIAS_INCLUDE_DIRS}}")  

  if(NOT ${ALIAS_FOUND})
    message("${module} ${component} package not found, searching...")
    find_component_paths(${module} ${component} ${library} ${header})    
  else()      
    message("${module} ${component} package found.")
    set_component_found(${module} ${component})
  endif()

endmacro()


#
### Macro: find_multi_component
#
# Checks for the given multi configuration component by invoking pkgconfig and then looking up the
# libraries and include directories.
# Extra helper variables may be set to assist finding libraries:
#   ${module}_PATH_SUFFIXES
#
macro(find_multi_component module component pkgconfig library header)

  # FIXME: currently broken, need to update API

  if (NOT WIN32)
     # use pkg-config to get the directories and then use these values
     # in the FIND_PATH() and find_library() calls
     find_package(PkgConfig)
     if (PKG_CONFIG_FOUND)
       pkg_check_modules(PC_${component} ${pkgconfig})
     endif()
  endif()

  find_path(${component}_INCLUDE_DIRS ${header}
    HINTS
      ${PC_LIB${component}_INCLUDEDIR}
      ${PC_LIB${component}_INCLUDE_DIRS}
    PATH_SUFFIXES
      ${${module}_PATH_SUFFIXES}
  )

  # Create a Debug and a Release list for multi configuration builds        
  if (CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)
    #set(${ALIAS_RELEASE_LIBRARIES} ${ALIAS_RELEASE_LIBRARIES}-NOTFOUND)
    #set(${ALIAS_DEBUG_LIBRARIES} ${ALIAS_DEBUG_LIBRARIES}-NOTFOUND)
    #set(${ALIAS_LIBRARIES})
    find_library(${ALIAS_RELEASE_LIBRARIES} 
      NAMES
        lib${library}.a 
        ${library}.lib
        ${library}
      HINTS
        ${PC_LIB${component}_LIBDIR}
        ${PC_LIB${component}_LIBRARY_DIRS}
      PATH_SUFFIXES 
        lib
        bin
    ) 
    find_library(${ALIAS_DEBUG_LIBRARIES} 
      NAMES
        lib${library}d.a 
        ${library}d.lib
        ${library}d
      HINTS
        ${PC_LIB${component}_LIBDIR}
        ${PC_LIB${component}_LIBRARY_DIRS}
      PATH_SUFFIXES 
        lib
        bin
    )     
    if (${ALIAS_RELEASE_LIBRARIES}) 
      list(APPEND ${ALIAS_LIBRARIES} "optimized" ${${ALIAS_RELEASE_LIBRARIES}})
    endif()
    if (${ALIAS_DEBUG_LIBRARIES})
      list(APPEND ${ALIAS_LIBRARIES} "debug" ${${ALIAS_DEBUG_LIBRARIES}})
    endif()  
    
    #message("R-----${component}=${${ALIAS_LIBRARIES}}")
    #message("D-----${component}=${${ALIAS_LIBRARIES}}")
  else()
    #set(${ALIAS_LIBRARIES})
    find_library(${ALIAS_LIBRARIES} 
      NAMES # setting in order might help overcome find_library bugs :/
        #lib${library}.a 
        #${library}.lib
        ${library}
      HINTS
        ${PC_LIB${component}_LIBDIR}
        ${PC_LIB${component}_LIBRARY_DIRS}
      PATH_SUFFIXES 
        lib
        bin
    )  
  endif()        
  
  #message("find_component =Searching for: ${library}")
  #message("find_component module=${${module}_PATH_SUFFIXES}")
  #message("find_component _PATH_SUFFIXES=${${module}_PATH_SUFFIXES}")
  #message("find_component _INCLUDE_DIRS=${${component}_INCLUDE_DIRS}")
  #message("find_component =lib${library}.a")
  #message("find_component =${${component}_INCLUDE_DIRS}")
  #message("find_component _INCLUDE_DIRS=${${component}_INCLUDE_DIRS}")
  #message("find_component _LIBRARIES=${${ALIAS_LIBRARIES}}")
  #message("${${component}_INCLUDE_DIRS}/lib")
  #message("${PC_LIB${component}_LIBDIR}")
  #message("${PC_LIB${component}_LIBRARY_DIRS}")
  #message("${header}")
  #message("${library}")

  set(${component}_DEFINITIONS  ${PC_${component}_CFLAGS_OTHER} CACHE STRING "The ${component} CFLAGS.")
  set(${component}_VERSION      ${PC_${component}_VERSION}      CACHE STRING "The ${component} version number.")

  set_component_found(${component})

  mark_as_advanced(
    ${component}_INCLUDE_DIRS
    ${ALIAS_LIBRARIES}
    ${ALIAS_DEBUG_LIBRARIES}
    ${ALIAS_RELEASE_LIBRARIES}
    ${component}_DEFINITIONS
    ${component}_VERSION)

endmacro()





         #   if(${ALIAS_FOUND})

 #   # Add results to the perant module lists    
 #   list(APPEND ${module}_INCLUDE_DIRS ${${ALIAS_INCLUDE_DIRS}})              
 #   list(APPEND ${module}_LIBRARY_DIRS ${${ALIAS_LIBRARY_DIRS}})        
  #  list(APPEND ${module}_LIBRARIES    ${${ALIAS_LIBRARIES}})          
 #   list(APPEND ${module}_DEFINITIONS  ${${ALIAS_DEFINITIONS}})       

  #  mark_as_advanced(
  #    ${${ALIAS_INCLUDE_DIRS}}
 #     ${${ALIAS_LIBRARY_DIRS}})
#
 # endif()  


  #if (NOT WIN32)
  #endif()    
  
  #message("find_component _PATH_SUFFIXES=${{module}_PATH_SUFFIXES}")
  #message("find_component _INCLUDE_DIRS=${${component}_INCLUDE_DIRS}")
  #message("find_component =lib${library}.a")
  #message("find_component =${${component}_INCLUDE_DIRS}")
  #message("find_component _LIBRARIES=${${ALIAS_LIBRARIES}}")  
  #message("find_component _PC=${PC_${component}}")  
  #message("find_component _PCg=${pkgconfig}")
  #message("${${component}_INCLUDE_DIRS}/lib")
  #message("${PC_LIB${component}_LIBDIR}")
  #message("${PC_LIB${component}_LIBRARY_DIRS}")
  #message("${header}")
  #message("${library}")

  # TODO: Update this to use ALIAS
  #set(${component}_DEFINITIONS  ${${ALIAS_CFLAGS_OTHER}} CACHE STRING "${module} ${component} CFLAGS.")
  #set(${component}_VERSION      ${${ALIAS_VERSION}}      CACHE STRING "${module} ${component} version number.")



        #pkg_checkmodules(PC_${component} ${pkgconfig})  
  	 #message("ALIAS LIBRARY_DIRS=${PC_${component}_LIBRARY_DIRS}")  	  	 
        #message("ALIAS_INCLUDE_DIRS=${PC_${component}_INCLUDE_DIRS}")   	  	 
        #message("ALIAS_LIBRARIES=${PC_${ALIAS_LIBRARIES}}")   
        #message("ALIAS_CFLAGS_OTHER=${PC_${component}_CFLAGS_OTHER}") 
        #message("ALIAS_VERSION=${PC_${component}_VERSION}")
	 #message("ALIAS_LIBRARIES=${${ALIAS_LIBRARIES}}")
	  #set(${define} 1)
          #foreach(P "${ALIAS_INCLUDE_DIRS}")
          #  if(${P})
          #    list(APPEND ${module}_INCLUDE_DIRS ${${P}})
          #    #message("ALIAS_INCLUDE_DIRS=${${P}}")
          #  endif()
          #endforeach()

          #foreach(P "${ALIAS LIBRARY_DIRS}")
          #  if(${P})
          #    list(APPEND ${module}_LIBRARY_DIRS ${${P}})
          #  endif()
          #endforeach()      
	   #message("ALIAS_LIBRARIES=${${ALIAS_LIBRARIES}}")


#    # Create a Debug and a Release list for multi configuration builds        
#  if (CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)
#    find_library(${ALIAS_RELEASE_LIBRARIES} 
#      NAMES
#        lib${library}.a 
#        ${library}.lib
#        ${library}
#      HINTS
#        ${PC_LIB${component}_LIBDIR}
#        ${PC_LIB${component}_LIBRARY_DIRS}
#      PATH_SUFFIXES 
#        lib
#        bin
#    ) 
#    find_library(${ALIAS_DEBUG_LIBRARIES} 
#      NAMES
#        lib${library}d.a 
#        ${library}d.lib
#        ${library}d
#      HINTS
#        ${PC_LIB${component}_LIBDIR}
#        ${PC_LIB${component}_LIBRARY_DIRS}
#      PATH_SUFFIXES 
#        lib
#        bin
#    ) 
#    #message("R-----${${ALIAS_RELEASE_LIBRARIES}}=${${ALIAS_LIBRARIES}}")
#    #message("D-----${${ALIAS_DEBUG_LIBRARIES}}=${${ALIAS_LIBRARIES}}")
#    
#    if (${ALIAS_RELEASE_LIBRARIES}) 
#      list(APPEND ${ALIAS_LIBRARIES} "optimized" ${${ALIAS_RELEASE_LIBRARIES}})
#    endif()
#    if (${ALIAS_DEBUG_LIBRARIES})
#      list(APPEND ${ALIAS_LIBRARIES} "debug" ${${ALIAS_DEBUG_LIBRARIES}})
#    endif()  
#  else()
#    find_library(${ALIAS_LIBRARIES} 
#      NAMES # setting in order might help overcome find_library bugs :/
#        lib${library}.a 
#        ${library}.lib
#        ${library}
#      HINTS
#        ${PC_LIB${component}_LIBDIR}
#        ${PC_LIB${component}_LIBRARY_DIRS}
#      PATH_SUFFIXES 
#        lib
#        bin
#    )  
#  endif()  
 