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
macro(LIST_LENGTH var)
  set(entries)
  foreach(e ${ARGN})
    set(entries "${entries}.")
  endforeach(e)
  string(LENGTH ${entries} ${var})
endmacro()


#
### Macro: append_unique_list
#
# Appends items from the source list to the given target list
# if they are not already contained within the target list
# in flattened string form.
#
function(append_unique_list target source)    
  if (NOT ${source}) 
    return()
  endif()
  if (NOT ${target})       
    set(${target} ${${source}} PARENT_SCOPE)
  else()
    join("${${target}}" ":" target_str)  
    join("${${source}}" ":" source_str)
    if (NOT ${target_str} MATCHES ${source_str}) 
      set(${target} ${${target}} ${${source}} PARENT_SCOPE)
    endif() 
  endif() 
endfunction()


#
### Macro: set_option
#
# Provides an option that the user can optionally select.
# Can accept condition to control when option is available for user.
# Usage:
#   option(<option_variable> "help string describing the option" <initial value or boolean expression> [IF <condition>])
macro(set_option variable description value)
  set(__value ${value})
  set(__condition "")
  set(__varname "__value")
  foreach(arg ${ARGN})
    if(arg STREQUAL "IF" OR arg STREQUAL "if")
      set(__varname "__condition")
    else()
      list(APPEND ${__varname} ${arg})
    endif()
  endforeach()
  unset(__varname)
  if("${__condition}" STREQUAL "")
    set(__condition 2 GREATER 1)
  endif()

  if(${__condition})
    if("${__value}" MATCHES ";")
      if(${__value})
        option(${variable} "${description}" ON)
      else()
        option(${variable} "${description}" OFF)
      endif()
    elseif(DEFINED ${__value})
      if(${__value})
        option(${variable} "${description}" ON)
      else()
        option(${variable} "${description}" OFF)
      endif()
    else()
      option(${variable} "${description}" ${__value})
    endif()
  else()
    unset(${variable} CACHE)
  endif()
  unset(__condition)
  unset(__value)
endmacro()

#
### Function: status
#
# Status report function.
# Automatically align right column and selects text based on condition.
# Usage:
#   status(<text>)
#   status(<heading> <value1> [<value2> ...])
#   status(<heading> <condition> THEN <text for TRUE> ELSE <text for FALSE> )
function(status text)
  set(status_cond)
  set(status_then)
  set(status_else)

  set(status_current_name "cond")
  foreach(arg ${ARGN})
    if(arg STREQUAL "THEN")
      set(status_current_name "then")
    elseif(arg STREQUAL "ELSE")
      set(status_current_name "else")
    else()
      list(APPEND status_${status_current_name} ${arg})
    endif()
  endforeach()

  if(DEFINED status_cond)
    set(status_placeholder_length 32)
    string(RANDOM LENGTH ${status_placeholder_length} ALPHABET " " status_placeholder)
    string(LENGTH "${text}" status_text_length)
    if(status_text_length LESS status_placeholder_length)
      string(SUBSTRING "${text}${status_placeholder}" 0 ${status_placeholder_length} status_text)
    elseif(DEFINED status_then OR DEFINED status_else)
      message(STATUS "${text}")
      set(status_text "${status_placeholder}")
    else()
      set(status_text "${text}")
    endif()

    if(DEFINED status_then OR DEFINED status_else)
      if(${status_cond})
        string(REPLACE ";" " " status_then "${status_then}")
        string(REGEX REPLACE "^[ \t]+" "" status_then "${status_then}")
        message(STATUS "${status_text} ${status_then}")
      else()
        string(REPLACE ";" " " status_else "${status_else}")
        string(REGEX REPLACE "^[ \t]+" "" status_else "${status_else}")
        message(STATUS "${status_text} ${status_else}")
      endif()
    else()
      string(REPLACE ";" " " status_cond "${status_cond}")
      string(REGEX REPLACE "^[ \t]+" "" status_cond "${status_cond}")
      message(STATUS "${status_text} ${status_cond}")
    endif()
  else()
    message(STATUS "${text}")
  endif()
endfunction()


# Converts a CMake list to a string containing elements separated by spaces
function(set_scy_libname module_name output_var)  
  set(temp_name)
  if(WIN32)
    # Postfix of DLLs:
    string(TOLOWER "scy_${module_name}_${LibSourcey_DLLVERSION}" temp_name)
  else()
    # Postfix of so's:
    string(TOLOWER "scy_${module_name}" temp_name)
  endif()
  set(${output_var} "${temp_name}" PARENT_SCOPE)
endfunction()

#
### Macro: ask_build_sourcey_dependency
#
# Optionally build a LibSourcey dependency.
#
macro(ask_build_sourcey_dependency name)
  if(BUILD_DEPENDENCIES)
    set(BUILD_DEPENDENCY_${name} ON CACHE BOOL "Build LibSourcey dependency: ${name}")   
  endif()    
  if(BUILD_DEPENDENCY_${name})    
    set(LibSourcey_BUILD_DEPENDENCIES ${LibSourcey_BUILD_DEPENDENCIES} ${name} CACHE INTERNAL "")
    #message(STATUS "Building dependency: ${name}")
  endif()    
  mark_as_advanced(FORCE BUILD_DEPENDENCY_${name})  
endmacro()


#
### Macro: ask_build_sourcey_module
#
# Optionally build a LibSourcey module.
# This should be called before  include_dependency and
# define_sourcey_module for each module.
#
macro(ask_build_sourcey_module name)
  if(BUILD_MODULES)
    set(BUILD_MODULE_${name} ON CACHE BOOL "Build LibSourcey module: ${name}")   
  endif()    
  if(BUILD_MODULE_${name})
    #message(STATUS "Building module: ${name}")   
    set(LibSourcey_BUILD_MODULES ${LibSourcey_BUILD_MODULES} ${name} CACHE INTERNAL "") 
    endif()    
  mark_as_advanced(FORCE BUILD_MODULE_${name})  
endmacro()


#
### Macro: ask_build_sourcey_test
#
# Optionally build a LibSourcey test.
# This should be called before  include_dependency and
# define_sourcey_test for each test.
#
macro(ask_build_sourcey_test name)
  if(BUILD_MODULE_TESTS)
    set(BUILD_TEST_${name} ON CACHE BOOL "Build LibSourcey test: ${name}")   
  endif()    
  if(BUILD_TEST_${name})
    #message(STATUS "Building module test: ${name}")  
    set(LibSourcey_BUILD_TESTS ${LibSourcey_BUILD_TESTS} ${name} CACHE INTERNAL "")   
  endif()    
  mark_as_advanced(FORCE BUILD_TEST_${name})  
endmacro()

#
### Macro: ask_build_sourcey_sample
#
# Optionally build a LibSourcey sample.
# This should be called before  include_dependency and
# define_sourcey_sample for each sample.
#
macro(ask_build_sourcey_sample name)
  if(BUILD_MODULE_SAMPLES)
    set(BUILD_SAMPLE_${name} ON CACHE BOOL "Build LibSourcey sample: ${name}")   
  endif()    
  if(BUILD_SAMPLE_${name})
    #message(STATUS "Building module sample: ${name}")  
    set(LibSourcey_BUILD_SAMPLES ${LibSourcey_BUILD_SAMPLES} ${name} CACHE INTERNAL "") 
  endif()    
  mark_as_advanced(FORCE BUILD_SAMPLE_${name})  
endmacro()


#
### Macro: ask_build_sourcey_application
#
# Optionally build a LibSourcey application.
# This should be called before  include_dependency for each application.
#
macro(ask_build_sourcey_application name)
  if(BUILD_APPLICATIONS)
    set(BUILD_APPLICATION_${name} ON CACHE BOOL "Build LibSourcey application: ${name}")
  endif()  
  if(BUILD_APPLICATION_${name})
    #message(STATUS "Building application: ${name}")  
    set(LibSourcey_BUILD_APPLICATIONS ${LibSourcey_BUILD_APPLICATIONS} ${name} CACHE INTERNAL "") 
  endif()    
  mark_as_advanced(FORCE BUILD_APPLICATION_${name})  
endmacro()