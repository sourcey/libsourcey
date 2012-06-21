#
### Macro: subdirlist
#
# Returns a list of subdirectories.
#
macro(subdirlist result curdir)
  file(GLOB children RELATIVE ${curdir} ${curdir}/*)
  set(dirlist "")
  foreach(child ${children})
    if(IS_DIRECTORY ${curdir}/${child})
        set(dirlist ${dirlist} ${child})
    endif()
  endforeach()
  set(${result} ${dirlist})
endmacro()


#
### Macro: join
#
# Joins a string array.
# Example:
#   SET( letters "" "\;a" b c "d\;d" )
#   JOIN("${letters}" ":" output)
#   MESSAGE("${output}") # :;a:b:c:d;d
#
function(JOIN VALUES GLUE OUTPUT)
  string (REPLACE ";" "${GLUE}" _TMP_STR "${VALUES}")
  set (${OUTPUT} "${_TMP_STR}" PARENT_SCOPE)
endfunction()


#
### Macro: list_length
#
# Example:
# SET(MYLIST hello world foo bar)
# LIST_LENGTH(length ${MYLIST})
# MESSAGE("length: ${length}")
#
MACRO(LIST_LENGTH var)
  SET(entries)
  FOREACH(e ${ARGN})
    SET(entries "${entries}.")
  ENDFOREACH(e)
  STRING(LENGTH ${entries} ${var})
ENDMACRO(LIST_LENGTH)


#
### Macro: list_append_new
#
# Appends items from the source list to the given target list
# if they are not already contained within the target list
# in string form.
#
function(list_append_new target source)  
  
  if (NOT ${source}) 
    #message("alist_append_unique: no source array ${source}: ${${source}}")    
    return()
  endif()

  #message("list_append_unique:BEFORE=${target}=${${target}}")  
  #message("======= alist_append_unique:target=${target}a")    
  #message("alist_append_unique:source=${source}a") 
  #message("alist_append_unique:target1=${${target}}a")    
  #message("alist_append_unique:source1=${${source}}a")  

  if (NOT ${target})       
    set(${target} ${${source}} PARENT_SCOPE)
    #message("alist_append_unique: target array is empty, setting from source")  
  else()    

    join("${${target}}" ":" target_str)  
    join("${${source}}" ":" source_str)
    if (NOT ${target_str} MATCHES ${source_str}) 
      #  #list(APPEND target ${source})      
      #message("alist_append_unique: apending ${source_str} to ${target_str}")  
      set(${target} ${${target}} ${${source}} PARENT_SCOPE)
    else()     	
      #message("alist_append_unique: skipping because of match: ${target_str} contains ${source_str}")  
    endif() 
  endif() 

  #message("list_append_unique:RESULT=${target}=${${target}}")  

  #message("list_append_unique:target_str=${target_str}")    
  #message("list_append_unique:source_str=${source_str}")  
  #set(${target} ${${target}} ${${source}})
  #set(${target} ${${source}})

  #if (NOT target_str)       
  #  set(target ${${target}} ${${source}} PARENT_SCOPE)
  #  return()  
  #endif() 

endfunction()


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

  include(FindPackageHandleStandardArgs)
  
  set(${module}_FOUND FALSE PARENT_SCOPE)

  #if (NOT ${module}_LIBRARIES OR NOT ${module}_INCLUDE_DIRS)  
  #  return()
  #endif()
 
  # Check that the required components were found.   
  #if (${module}_FIND_COMPONENTS)
  #  foreach (component ${${module}_FIND_COMPONENTS})
  #    if (${module}_${component}_FOUND)
  #      message(STATUS "Required ${module} component ${component} present.")
  #    else ()
  #      message(STATUS "Required ${module} component ${component} missing.")
  #      return()
  #    endif ()
  #  endforeach ()
  #endif()  

  # TODO: Honour this setting
  #if (${module}_FIND_REQUIRED)

  # Compile the list of required vars
  set(_${module}_REQUIRED_VARS ${module}_LIBRARIES ${module}_INCLUDE_DIRS)
  foreach (_component ${${module}_FIND_COMPONENTS})
    list(APPEND _${module}_REQUIRED_VARS ${module}_${_component}_LIBRARIES ${module}_${_component}_INCLUDE_DIRS)
  endforeach ()

  # Give a nice error message if some of the required vars are missing.
  find_package_handle_standard_args(${module} DEFAULT_MSG ${_${module}_REQUIRED_VARS})

  #endif() 

  # Cache the vars.
  #set(${module}_INCLUDE_DIRS ${${module}_INCLUDE_DIRS} CACHE STRING   "The ${module} include directories." FORCE)
  #set(${module}_LIBRARY_DIRS ${${module}_LIBRARY_DIRS} CACHE STRING   "The ${module} library directories." FORCE)
  #set(${module}_LIBRARIES    ${${module}_LIBRARIES}    CACHE STRING   "The ${module} libraries." FORCE)
  #set(${module}_FOUND        ${${module}_FOUND}        CACHE BOOLEAN  "The ${module} found status." FORCE)  

  set(${module}_INCLUDE_DIRS ${${module}_INCLUDE_DIRS} PARENT_SCOPE)
  set(${module}_LIBRARY_DIRS ${${module}_LIBRARY_DIRS} PARENT_SCOPE)
  set(${module}_LIBRARIES    ${${module}_LIBRARIES}    PARENT_SCOPE)
  set(${module}_FOUND        ${${module}_FOUND}        PARENT_SCOPE)

  mark_as_advanced(${module}_INCLUDE_DIRS
                   ${module}_LIBRARY_DIRS
                   ${module}_LIBRARIES
                   ${module}_DEFINITIONS
                   ${module}_FOUND)

  set(${module}_FOUND TRUE PARENT_SCOPE)

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
    message(STATUS "  - ${module} ${component} found.")
    set(${ALIAS_FOUND} TRUE PARENT_SCOPE)
  
    # Add component vars to the perant module lists
    list_append_new(${module}_INCLUDE_DIRS ${ALIAS_INCLUDE_DIRS})    
    list_append_new(${module}_LIBRARY_DIRS ${ALIAS_LIBRARY_DIRS})   
    list_append_new(${module}_LIBRARIES    ${ALIAS_LIBRARIES})    
    list_append_new(${module}_DEFINITIONS  ${ALIAS_DEFINITIONS})

    #if (${${module}_INCLUDE_DIRS} NOT MATCHES ${${ALIAS_INCLUDE_DIRS}}) 
    #	list(APPEND ${module}_INCLUDE_DIRS ${${ALIAS_INCLUDE_DIRS}})    
    #endif ()             
    #if (${${module}_LIBRARY_DIRS} NOT MATCHES ${${ALIAS_LIBRARY_DIRS}}) 
    #	list(APPEND ${module}_LIBRARY_DIRS ${${ALIAS_LIBRARY_DIRS}})   
    #endif ()           
    #if (${${module}_LIBRARIES} NOT MATCHES ${${ALIAS_LIBRARIES}}) 
    #  list(APPEND ${module}_LIBRARIES    ${${ALIAS_LIBRARIES}})  
    #endif ()            
    #if (${${module}_DEFINITIONS} NOT MATCHES ${${ALIAS_DEFINITIONS}})    
    #  list(APPEND ${module}_DEFINITIONS  ${${ALIAS_DEFINITIONS}}) 
    #endif ()   
    #set(${module}_INCLUDE_DIRS ${${module}_INCLUDE_DIRS} ${${ALIAS_INCLUDE_DIRS}} PARENT_SCOPE)      
    #set(${module}_LIBRARY_DIRS ${${module}_LIBRARY_DIRS} ${${ALIAS_LIBRARY_DIRS}} PARENT_SCOPE)  
    #set(${module}_LIBRARIES    ${${module}_LIBRARIES}    ${${ALIAS_LIBRARIES}} PARENT_SCOPE)        
    #set(${module}_LIBRARIES    ${${module}_LIBRARIES}    ${${ALIAS_LIBRARIES}})      
    #set(${module}_DEFINITIONS  ${${module}_DEFINITIONS} ${${ALIAS_DEFINITIONS}} PARENT_SCOPE)   

    #message("Find Component Paths=${module}:${component}:${library}:${header}")
    #message("${ALIAS_INCLUDE_DIRS}=${${ALIAS_INCLUDE_DIRS}}")  
    #message("${ALIAS_RELEASE_LIBRARIES}=${${ALIAS_RELEASE_LIBRARIES}}")  
    #message("${ALIAS_DEBUG_LIBRARIES}=${${ALIAS_DEBUG_LIBRARIES}}")  
    #message("${ALIAS_LIBRARIES}=${${ALIAS_LIBRARIES}}")
    #message("${module}_INCLUDE_DIRS=${${module}_INCLUDE_DIRS}") 



    #message("${module}_LIBRARIES=${${module}_LIBRARIES}")         
    #message("${ALIAS_LIBRARIES}=${${ALIAS_LIBRARIES}}")  
    #message("${ALIAS_INCLUDE_DIRS}=${${ALIAS_INCLUDE_DIRS}}")      
    #message("${module}_INCLUDE_DIRS=${${module}_INCLUDE_DIRS}")  


    mark_as_advanced(
      ${ALIAS_INCLUDE_DIRS}
      ${ALIAS_LIBRARY_DIRS})

  else ()
     message(STATUS "  - ${module} ${component} not found.")
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

  # message(STATUS "  - ${module} ${component} setting notfound.")
  set(${module}_FOUND FALSE PARENT_SCOPE)
  
  if (CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)  
    #set(${module}_RELEASE_LIBRARIES "" PARENT_SCOPE)
    #set(${module}_DEBUG_LIBRARIES "" PARENT_SCOPE)
    #set(${module}_LIBRARIES "" PARENT_SCOPE)
  else()    
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

  # message(STATUS "  - ${module} ${component} setting notfound.")
  set(${ALIAS_FOUND} FALSE PARENT_SCOPE)
  
  if (CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)  
    #set(${ALIAS_RELEASE_LIBRARIES} ${ALIAS_RELEASE_LIBRARIES}-NOTFOUND PARENT_SCOPE)
    #set(${ALIAS_DEBUG_LIBRARIES} ${ALIAS_DEBUG_LIBRARIES}-NOTFOUND PARENT_SCOPE)
    #set(${ALIAS_LIBRARIES} "") #${module}_${component}_LIBRARIES-NOTFOUND PARENT_SCOPE)
  else()
    #set(${ALIAS_LIBRARIES} ${ALIAS_LIBRARIES}-NOTFOUND PARENT_SCOPE)
  endif()  
    
endmacro()


#
### Macro: find_paths
#
# Finds the given component library and include paths.
#
macro(find_component_paths module component library header)

  set_component_alias(${module} ${component})  
  #set_component_notfound(${module} ${component})

  find_path(${ALIAS_INCLUDE_DIRS} ${header}
    #HINTS
    #  ${${ALIAS_INCLUDE_DIRS}}    
    PATH_SUFFIXES
      ${${module}_PATH_SUFFIXES}
  )

  # Create a Debug and a Release list for multi configuration builds        
  if (CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)
    find_library(${ALIAS_RELEASE_LIBRARIES} 
      NAMES
        lib${library}.a         
        lib${library}.socd ..
        ${library}.lib
        ${library}
      #HINTS
      #  ${${ALIAS_LIBRARY_DIRS}}
    ) 
    find_library(${ALIAS_DEBUG_LIBRARIES} 
      NAMES
        lib${library}d.a       
        lib${library}d.so
        ${library}d.lib
        ${library}d
      #HINTS
      #  ${${ALIAS_LIBRARY_DIRS}}
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
        lib${library}.so     
        lib${library}.a 
        ${library}.lib
        ${library}
      #HINTS
      #  ${${ALIAS_LIBRARY_DIRS}}
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
	 
  #message("Find Component=${module}:${component}:${pkgconfig}:${library}:${header}")

  set_component_alias(${module} ${component})

  # Use pkg-config to obtain directories for
  # the FIND_PATH() and find_library() calls.
  find_package(PkgConfig)
  if (PKG_CONFIG_FOUND)
    set(PKG_ALIAS                   PKG_${component})
    pkg_check_modules(${PKG_ALIAS}  ${pkgconfig})
    set(${ALIAS}_FOUND              ${${PKG_ALIAS}_FOUND})
    set(${ALIAS}_LIBRARIES          ${${PKG_ALIAS}_LIBRARIES})
    set(${ALIAS}_INCLUDE_DIRS       ${${PKG_ALIAS}_INCLUDE_DIRS})
    set(${ALIAS}_LIBRARY_DIRS       ${${PKG_ALIAS}_LIBRARY_DIRS})
    set(${ALIAS}_DEFINITIONS        ${${PKG_ALIAS}_CFLAGS_OTHER})        
    set(${ALIAS}_VERSION            ${${PKG_ALIAS}_VERSION})
  endif()  
   
  #message("${ALIAS_LIBRARIES}=${${ALIAS_LIBRARIES}}")  
  #message("${ALIAS_INCLUDE_DIRS}=${${ALIAS_INCLUDE_DIRS}}")  

  if(NOT ${ALIAS_FOUND})
    find_component_paths(${module} ${component} ${library} ${header})    
  else()   
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

  # FIXME

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
  