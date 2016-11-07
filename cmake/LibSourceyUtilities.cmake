#
### Function: set_scy_libname
#
# Converts a CMake list to a string containing elements separated by spaces
#
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
