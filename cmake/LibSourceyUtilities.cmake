#
### Function: set_scy_libname
#
# Converts a CMake list to a string containing elements separated by spaces
#
function(set_scy_libname module_name output_var)
  set(temp_name)
  if(WIN32)
    # Postfix of DLLs:
    string(TOLOWER "scy_${module_name}" temp_name) #_${LibSourcey_DLLVERSION}
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
# This should be called before include_dependency and
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
# This should be called before include_dependency and
# define_sourcey_test for each test.
#
macro(ask_build_sourcey_test name)
  if(BUILD_TESTS)
    set(BUILD_TESTS_${name} ON CACHE BOOL "Build LibSourcey test: ${name}")
  endif()
  if(BUILD_TESTS_${name})
    #message(STATUS "Building module test: ${name}")
    set(LibSourcey_BUILD_TESTS ${LibSourcey_BUILD_TESTS} ${name} CACHE INTERNAL "")
  endif()
  mark_as_advanced(FORCE BUILD_TESTS_${name})
endmacro()


#
### Macro: ask_build_sourcey_sample
#
# Optionally build a LibSourcey sample.
# This should be called before  include_dependency and
# define_sourcey_sample for each sample.
#
macro(ask_build_sourcey_sample name)
  if(BUILD_SAMPLES)
    set(BUILD_SAMPLES_${name} ON CACHE BOOL "Build LibSourcey sample: ${name}")
  endif()
  if(BUILD_SAMPLES_${name})
    #message(STATUS "Building module sample: ${name}")
    set(LibSourcey_BUILD_SAMPLES ${LibSourcey_BUILD_SAMPLES} ${name} CACHE INTERNAL "")
  endif()
  mark_as_advanced(FORCE BUILD_SAMPLES_${name})
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


#
### Macro: start_track_variables
#
# Start to track variables for change or adding.
# Note that variables starting with underscore are ignored.
macro(start_track_variables)
    get_cmake_property(_fnvtps_cache_vars CACHE_VARIABLES)
    get_cmake_property(_fnvtps_old_vars VARIABLES)
    
    foreach(_i ${_fnvtps_old_vars})
        if (NOT "x${_i}" MATCHES "^x_.*$")
            list(FIND _fnvtps_cache_vars ${_i} _fnvtps_is_in_cache)
            if(${_fnvtps_is_in_cache} EQUAL -1)
                set("_fnvtps_old${_i}" ${${_i}})
                #message(STATUS "_fnvtps_old${_i} = ${_fnvtps_old${_i}}")
            endif()
        endif()
    endforeach()
endmacro()


#
### Macro: forward_changed_variables_to_parent_scope
#
# forward_changed_variables_to_parent_scope([exclusions])
# Forwards variables that was added/changed since last call to start_track_variables() to the parent scope.
# Note that variables starting with underscore are ignored.
macro(forward_changed_variables_to_parent_scope)
    get_cmake_property(_fnvtps_cache_vars CACHE_VARIABLES)
    get_cmake_property(_fnvtps_vars VARIABLES)
    set(_fnvtps_cache_vars ${_fnvtps_cache_vars} ${ARGN})
    
    foreach(_i ${_fnvtps_vars})
        if (NOT "x${_i}" MATCHES "^x_.*$")
            list(FIND _fnvtps_cache_vars ${_i} _fnvtps_is_in_cache)
            
            if (${_fnvtps_is_in_cache} EQUAL -1)
                list(FIND _fnvtps_old_vars ${_i} _fnvtps_is_old)
                
                if(${_fnvtps_is_old} EQUAL -1 OR NOT "${${_i}}" STREQUAL "${_fnvtps_old${_i}}")
                    set(${_i} ${${_i}} PARENT_SCOPE)
                    #message(STATUS "forwarded var ${_i}")
                endif()
            endif()
        endif()
    endforeach()
endmacro()