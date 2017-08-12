#
### Macro: add_vendor_dependency
#
macro(add_vendor_dependency name libname)
  set(libdir ${LibSourcey_VENDOR_SOURCE_DIR}/${libname})

  add_subdirectory(${libdir} ${LibSourcey_VENDOR_BUILD_DIR}/${libname})

  # Include current directory and existing dependency directories
  # include_directories(${libdir})

  # Cache dependency directories for inclusion by modules and applications
  # get_directory_property(lib_directories INCLUDE_DIRECTORIES)
  set(LibSourcey_INCLUDE_DIRS ${libdir})
  set(LibSourcey_LINK_LIBRARIES ${LibSourcey_LINK_LIBRARIES} ${libname})
  set(LibSourcey_BUILD_DEPENDENCIES ${LibSourcey_BUILD_DEPENDENCIES} ${libname})
  set(HAVE_${name} TRUE)

  set(LibSourcey_INCLUDE_DIRS ${LibSourcey_INCLUDE_DIRS} CACHE INTERNAL "")
  set(LibSourcey_LINK_LIBRARIES ${LibSourcey_LINK_LIBRARIES} CACHE INTERNAL "")
  set(LibSourcey_BUILD_DEPENDENCIES ${LibSourcey_BUILD_DEPENDENCIES} CACHE INTERNAL "")
  set(HAVE_${name} TRUE CACHE INTERNAL "")

  # get_directory_property(hasParent PARENT_DIRECTORY)
  # if(hasParent)
  #   set(LibSourcey_INCLUDE_DIRS ${libdir} PARENT_SCOPE)
  #   set(LibSourcey_LINK_LIBRARIES ${LibSourcey_LINK_LIBRARIES} ${libname} PARENT_SCOPE)
  #   set(LibSourcey_BUILD_DEPENDENCIES ${LibSourcey_BUILD_DEPENDENCIES} ${libname} PARENT_SCOPE)

  #   set(HAVE_${name} TRUE PARENT_SCOPE)
  # endif()
endmacro()


#
### Function: find_dependency
#
# Finds a 3rd party dependency and sets the XXX_FOUND and HAVE_XXX variables.
#
function(find_dependency name)

  # Set the variable scope
  set(var_root ${name})
  string(TOUPPER ${var_root} var_root_upper)

  # If the module is disabled then continue
  if(DEFINED WITH_${var_root_upper} AND NOT WITH_${var_root_upper})
    return()
  endif()

  # start_track_variables()
  find_package(${name} ${ARGN})
  # forward_changed_variables_to_parent_scope() #(bla_bla);

  # Try to find standard uppercase variable accessor
  set(lib_found 0)
  if(${var_root_upper}_FOUND)
    set(var_root ${var_root_upper})
    set(lib_found 1)
  # else()
  #   if(${var_root}_FOUND)
  #   set(lib_found 1)
  # endif()
  endif()

  # Exit message on failure
  if (NOT ${var_root}_FOUND)

    # NOTE: find_package does not always honour REQUIRED flag, so make
    # sure the build is cancelled if the dependency wasnt found.
    if (${ARGN} MATCHES REQUIRED)
      message(FATAL_ERROR "Failed to include dependency: ${name}. Please build and install dependencies before using CMake.")
    else()
      message("Failed to include dependency: ${name}. Please build and install dependencies before using CMake.")
      return()
    endif()
  endif()

  # Set a HAVE_XXX variable at global scope for the libsourcey.h config sile
  set(HAVE_${var_root_upper} TRUE CACHE INTERNAL "")
  set(${var_root_upper}_FOUND TRUE CACHE INTERNAL "")

  # HACK: This is also called in `include_dependency`, but for some reason
  # valiables set by `find_package`, such as `OPENSSL_LIBRARIES` set by 
  # are not propagating to the outside `include_dependency` function scope.
  # This behaviour was noted with cmake 3.5.2 when calling FindSSL.cmake.
  # Expose normal and uppercase variants to LibSourcey
  string(TOUPPER ${name} name_upper)
  add_dependency_build_variables(${name})
  add_dependency_build_variables(${name_upper})

  # This is required for multi-level finders such as FindSSL.cmake
  # get_directory_property(hasParent PARENT_DIRECTORY)
  # if(hasParent)
  #   set(HAVE_${var_root_upper} TRUE PARENT_SCOPE)
  #   set(${var_root_upper}_FOUND TRUE PARENT_SCOPE)
  # endif()
endfunction()


#
### Macro: include_dependency
#
# Includes a 3rd party dependency into the LibSourcey solution.
#
function(include_dependency name)
  message(STATUS "Including dependency: ${name}")
  find_dependency(${name} ${ARGN})

  # Expose normal and uppercase variants to LibSourcey
  string(TOUPPER ${name} name_upper)
  add_dependency_build_variables(${name})
  add_dependency_build_variables(${name_upper})
endfunction()


#
### Macro: add_dependency_build_variables
#
# Adds a dependency variables the LibSourcey build variables.
#
macro(add_dependency_build_variables name)
  # print_module_variables(${name})

  # TODO: Remove the use of global LibSourcey_XXX variables and use
  # set_target_xxx function to set dependencies on a target specific basis.

  if(${name}_INCLUDE_DIR)
    # message(STATUS "- Found ${name} Inc Dir: ${${name}_INCLUDE_DIR}")
    #include_directories(${${name}_INCLUDE_DIR})
    list(APPEND LibSourcey_INCLUDE_DIRS ${${name}_INCLUDE_DIR})
    set(LibSourcey_INCLUDE_DIRS ${LibSourcey_INCLUDE_DIRS} ${${name}_INCLUDE_DIR})
  endif()
  if(${name}_INCLUDE_DIRS)
    # message(STATUS "- Found ${name} Inc Dirs: ${${name}_INCLUDE_DIRS}")
    #include_directories(${${name}_INCLUDE_DIRS})
    list(APPEND LibSourcey_INCLUDE_DIRS ${${name}_INCLUDE_DIRS})
    set(LibSourcey_INCLUDE_DIRS ${LibSourcey_INCLUDE_DIRS} ${${name}_INCLUDE_DIRS})
  endif()
  if(${name}_LIBRARY_DIR)
    #message(STATUS "- Found ${name} Lib Dir: ${${name}_LIBRARY_DIR}")
    #link_directories(${${name}_LIBRARY_DIR})
    list(APPEND LibSourcey_LIBRARY_DIRS ${${name}_LIBRARY_DIR})
    set(LibSourcey_LIBRARY_DIRS ${LibSourcey_LIBRARY_DIRS} ${${name}_LIBRARY_DIR})
  endif()
  if(${name}_LIBRARY_DIRS)
    #message(STATUS "- Found ${name} Lib Dirs: ${${name}_LIBRARY_DIRS}")
    #link_directories(${${name}_LIBRARY_DIRS})
    list(APPEND LibSourcey_LIBRARY_DIRS ${${name}_LIBRARY_DIRS})
    set(LibSourcey_LIBRARY_DIRS ${LibSourcey_LIBRARY_DIRS} ${${name}_LIBRARY_DIRS})
  endif()
  if(${name}_LIBRARY)
    # message(STATUS "- Found dependency lib ${name}: ${${name}_LIBRARY}")
    list(APPEND LibSourcey_INCLUDE_LIBRARIES ${${name}_LIBRARY})
    set(LibSourcey_INCLUDE_LIBRARIES ${LibSourcey_INCLUDE_LIBRARIES} ${${name}_LIBRARY})
  endif()
  if(${name}_LIBRARIES)
    # message(STATUS "- Found dependency libs ${name}: ${${name}_LIBRARIES}")
    list(APPEND LibSourcey_INCLUDE_LIBRARIES ${${name}_LIBRARIES})
    set(LibSourcey_INCLUDE_LIBRARIES ${LibSourcey_INCLUDE_LIBRARIES} ${${name}_LIBRARIES})
  endif()
  if(${name}_DEPENDENCIES)
    # message(STATUS "- Found external dependency ${name}: ${${name}_DEPENDENCIES}")
    list(APPEND LibSourcey_INCLUDE_LIBRARIES ${${name}_DEPENDENCIES})
    set(LibSourcey_INCLUDE_LIBRARIES ${LibSourcey_INCLUDE_LIBRARIES} ${${name}_DEPENDENCIES})
  endif()

  # if(LibSourcey_INCLUDE_DIRS)
  #   list(REMOVE_DUPLICATES LibSourcey_INCLUDE_DIRS)
  # endif()
  # if(LibSourcey_LIBRARY_DIRS)
  #  list(REMOVE_DUPLICATES LibSourcey_LIBRARY_DIRS)
  # endif()
  # if(LibSourcey_INCLUDE_LIBRARIES)
  #   list(REMOVE_DUPLICATES LibSourcey_INCLUDE_LIBRARIES)
  # endif()
  # if(LibSourcey_BUILD_DEPENDENCIES)
  #   list(REMOVE_DUPLICATES LibSourcey_BUILD_DEPENDENCIES)
  # endif()

  set(LibSourcey_LIBRARY_DIRS ${LibSourcey_LIBRARY_DIRS} CACHE INTERNAL "")
  set(LibSourcey_INCLUDE_DIRS ${LibSourcey_INCLUDE_DIRS} CACHE INTERNAL "")
  set(LibSourcey_INCLUDE_LIBRARIES ${LibSourcey_INCLUDE_LIBRARIES} CACHE INTERNAL "")

  set(LibSourcey_LIBRARY_DIRS ${LibSourcey_LIBRARY_DIRS} PARENT_SCOPE)
  set(LibSourcey_INCLUDE_DIRS ${LibSourcey_INCLUDE_DIRS} PARENT_SCOPE)
  set(LibSourcey_INCLUDE_LIBRARIES ${LibSourcey_INCLUDE_LIBRARIES} PARENT_SCOPE)
endmacro()


#
### Macro: set_default_project_directories
#
# Set the default header and library directories for a LibSourcey project.
#
macro(set_default_project_directories name)
  # set(MODULE_${name})
  foreach(module ${ARGN})
    list(APPEND LibSourcey_LIBRARY_DIRS ${LibSourcey_BUILD_DIR}/${module})
    list(APPEND LibSourcey_INCLUDE_DIRS ${LibSourcey_SOURCE_DIR}/${module}/include)
  endforeach()

  target_include_directories(${name} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${LibSourcey_INCLUDE_DIRS}
    ${LibSourcey_VENDOR_INCLUDE_DIRS}
    ${CMAKE_BINARY_DIR}
  )

  # include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
  # include_directories(${LibSourcey_INCLUDE_DIRS})
  # include_directories(${LibSourcey_VENDOR_INCLUDE_DIRS}) # include all internal vendor dirs
  # include_directories(${CMAKE_BINARY_DIR})
  link_directories(${LibSourcey_LIBRARY_DIRS})
endmacro()


#
### Macro: set_default_project_dependencies
#
# Set the default dependencies for a LibSourcey project.
#
macro(set_default_project_dependencies name)
  # foreach(dep ${LibSourcey_BUILD_DEPENDENCIES})
  #   add_dependencies(${name} ${dep})
  # endforeach()

  # Include dependent modules
  foreach(module ${ARGN})
    add_dependencies(${name} ${module})
  endforeach()

  # Include all linker libraries
  set(${name}_LIBRARIES ${ARGN})
  list(APPEND ${name}_LIBRARIES ${LibSourcey_INCLUDE_LIBRARIES})
  list(APPEND ${name}_LIBRARIES ${LibSourcey_BUILD_DEPENDENCIES})

  # Allow exclusion of specific libraries
  if (${name}_EXCLUDE_LIBRARIES)
    foreach(lib ${${name}_LIBRARIES})
      if(${lib} MATCHES ${${name}_EXCLUDE_LIBRARIES})
        list(REMOVE_ITEM ${name}_LIBRARIES ${lib})
      endif()
    endforeach()
  endif()

  #message(STATUS "${name} link libraries: ${LibSourcey_INCLUDE_LIBRARIES}")
  target_link_libraries(${name} ${${name}_LIBRARIES})
endmacro()


#
### Macro: include_sourcey_modules
#
# Includes dependent LibSourcey module(s) into a project.
#
#macro(include_sourcey_modules)
#  foreach(name ${ARGN})
#  #message(STATUS "Including module: ${name}")
#
#  set_scy_libname(${name} lib_name)
#
#  # Include the build library directory.
#  list(APPEND LibSourcey_LIBRARY_DIRS ${LibSourcey_BUILD_DIR}/src/${name})
#
#  # Include the module headers.
#  # These may be located in the "src/" root directory or in a sub directory.
#  set(HAVE_SOURCEY_${name} 0)
#  if(IS_DIRECTORY "${LibSourcey_SOURCE_DIR}/${name}/include")
#    #include_directories("${LibSourcey_SOURCE_DIR}/${name}/include")
#    list(APPEND LibSourcey_INCLUDE_DIRS ${LibSourcey_SOURCE_DIR}/${name}/include)
#    set(HAVE_SOURCEY_${name} 1)
#  else()
#    subdirlist(subdirs "${LibSourcey_SOURCE_DIR}")
#    foreach(dir ${subdirs})
#    set(dir "${LibSourcey_SOURCE_DIR}/${dir}/${name}/include")
#    if(IS_DIRECTORY ${dir})
#      #include_directories(${dir})
#      list(APPEND LibSourcey_INCLUDE_DIRS ${dir})
#      set(HAVE_SOURCEY_${name} 1)
#    endif()
#    endforeach()
#  endif()
#
#  if (NOT HAVE_SOURCEY_${name})
#    message(ERROR "Unable to include dependent LibSourcey module ${name}. The build may fail.")
#  endif()
#
#  # Create a Debug and a Release list for MSVC
#  if (MSVC)
#
#    # Find the module giving priority to the build output folder, and install folder second.
#    # This way we don't have to install updated modules before building applications
#    # simplifying the build process.
#
#    # Always reset the module folders.
#    set(LibSourcey_${name}_RELEASE LibSourcey_${name}_RELEASE-NOTFOUND)
#    #set(LibSourcey_${name}_RELEASE LibSourcey_${name}_RELEASE-NOTFOUND PARENT_SCOPE)
#    set(LibSourcey_${name}_DEBUG LibSourcey_${name}_DEBUG-NOTFOUND)
#    #set(LibSourcey_${name}_DEBUG LibSourcey_${name}_DEBUG-NOTFOUND PARENT_SCOPE)
#
#    # Since CMake doesn't give priority to the PATHS parameter, we need to search twice:
#    # once using NO_DEFAULT_PATH, and once using default values.
#    # TODO: Better handle nested module build dirs, or have all modules build to
#    # intermediate directory for easy searching.
#    find_library(LibSourcey_${name}_RELEASE "${lib_name}"
#    PATHS
#      ${LibSourcey_BUILD_DIR}/src/${name}
#      ${LibSourcey_BUILD_DIR}/src/anionu-sdk/${name}
#      ${LibSourcey_BUILD_DIR}/src/anionu-private/${name}
#    PATH_SUFFIXES Release
#    NO_DEFAULT_PATH)
#    find_library(LibSourcey_${name}_DEBUG "${lib_name}d"
#    PATHS
#      ${LibSourcey_BUILD_DIR}/src/${name}
#      ${LibSourcey_BUILD_DIR}/src/anionu-sdk/${name}
#      ${LibSourcey_BUILD_DIR}/src/anionu-private/${name}
#    PATH_SUFFIXES Debug
#    NO_DEFAULT_PATH)
#
#    # Search the module install folder if none was located.
#    find_library(LibSourcey_${name}_RELEASE "${lib_name}")
#    find_library(LibSourcey_${name}_DEBUG "${lib_name}d")
#
#    if(CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)
#    if (LibSourcey_${name}_RELEASE)
#      list(APPEND LibSourcey_INCLUDE_LIBRARIES "optimized" ${LibSourcey_${name}_RELEASE})
#      mark_as_advanced(LibSourcey_${name}_RELEASE)
#    endif()
#    if (LibSourcey_${name}_DEBUG)
#      list(APPEND LibSourcey_INCLUDE_LIBRARIES "debug" ${LibSourcey_${name}_DEBUG})
#      mark_as_advanced(LibSourcey_${name}_DEBUG)
#    endif()
#    else()
#    if (LibSourcey_${name}_RELEASE)
#      list(APPEND LibSourcey_INCLUDE_LIBRARIES ${LibSourcey_${name}_RELEASE})
#      mark_as_advanced(LibSourcey_${name}_RELEASE)
#    endif()
#    if (LibSourcey_${name}_DEBUG)
#      mark_as_advanced(LibSourcey_${name}_DEBUG)
#    endif()
#    endif()
#  else()
#    # Find the module giving preference to the build output folder.
#    #find_library(LibSourcey_${name} "${lib_name}"
#    #  PATHS ${LibSourcey_BUILD_DIR}/lib/${name}
#    #  NO_DEFAULT_PATH)
#    find_library(LibSourcey_${name} "${lib_name}")
#    if (LibSourcey_${name})
#    # Prepend module libraries otherwise linking may fail
#    # on compilers that require ordering of link libraries.
#    set(LibSourcey_INCLUDE_LIBRARIES ${LibSourcey_${name}} ${LibSourcey_INCLUDE_LIBRARIES})
#    mark_as_advanced(LibSourcey_${name})
#    else()
#    endif()
#  endif()
#  endforeach()
#endmacro()

# #
# ### Macro: set_component_alias
# #
# # Sets the current module component alias variables.
# #
# macro(set_component_alias module component)
#   set(ALIAS                   ${module}_${component})
#   set(ALIAS_FOUND             ${ALIAS}_FOUND)
#   set(ALIAS_LIBRARIES         ${ALIAS}_LIBRARIES)
#   set(ALIAS_RELEASE_LIBRARIES ${ALIAS}_RELEASE_LIBRARIES)
#   set(ALIAS_DEBUG_LIBRARIES   ${ALIAS}_DEBUG_LIBRARIES)
#   set(ALIAS_INCLUDE_DIRS      ${ALIAS}_INCLUDE_DIRS)
#   set(ALIAS_LIBRARY_DIRS      ${ALIAS}_LIBRARY_DIRS)
#   set(ALIAS_DEFINITIONS       ${ALIAS}_CFLAGS_OTHER)
#   set(ALIAS_VERSION           ${ALIAS}_VERSION)
# endmacro()
#
#
# #
# ### Macro: set_module_found
# #
# # Marks the given module as found if all required components are present.
# #
# macro(set_module_found module)
#   set(${module}_FOUND FALSE)
#   # set(${module}_FOUND FALSE PARENT_SCOPE)
#
#   # Compile the list of required vars
#   set(_${module}_REQUIRED_VARS ${module}_LIBRARIES) # ${module}_INCLUDE_DIRS
#   foreach (component ${${module}_FIND_COMPONENTS})
#     # NOTE: Not including XXX_INCLUDE_DIRS as required var since it may be empty
#     list(APPEND _${module}_REQUIRED_VARS ${module}_${component}_LIBRARIES) # ${module}_${component}_INCLUDE_DIRS
#     if (NOT ${module}_${component}_FOUND)
#       message(FATAL_ERROR "Required ${module} component ${component} missing. Please recompile ${module} with ${component} enabled.")
#     #else()
#       # message(STATUS "  - Required ${module} component ${component} found.")
#     endif()
#   endforeach()
#
#   # Cache the vars.
#   set(${module}_INCLUDE_DIRS   ${${module}_INCLUDE_DIRS} CACHE STRING   "The ${module} include directories." FORCE)
#   set(${module}_LIBRARY_DIRS   ${${module}_LIBRARY_DIRS} CACHE STRING   "The ${module} library directories." FORCE)
#   set(${module}_LIBRARIES      ${${module}_LIBRARIES}    CACHE STRING   "The ${module} libraries." FORCE)
#   set(${module}_FOUND          ${${module}_FOUND}        CACHE BOOLEAN  "The ${module} found status." FORCE)
#   # set(${module}_INCLUDE_DIRS ${${module}_INCLUDE_DIRS} PARENT_SCOPE)
#   # set(${module}_LIBRARY_DIRS ${${module}_LIBRARY_DIRS} PARENT_SCOPE)
#   # set(${module}_LIBRARIES    ${${module}_LIBRARIES}  PARENT_SCOPE)
#   # set(${module}_FOUND        ${${module}_FOUND}    PARENT_SCOPE)
#
#   # Ensure required variables have been set, or fail in error.
#   # NOTE: Disabling find_package_handle_standard_args check since it's always
#   # returning false even when variables are set. Maybe an internal CMake issue?
#   # if (${module}_FIND_REQUIRED)
#   #
#   #   # Give a nice error message if some of the required vars are missing.
#   #   include(FindPackageHandleStandardArgs)
#   #   find_package_handle_standard_args(${module} DEFAULT_MSG ${_${module}_REQUIRED_VARS})
#   # endif()
#
#   # Set the module as found.
#   if (${module}_LIBRARIES)
#     set(${module}_FOUND TRUE)
#     # set(${module}_FOUND TRUE PARENT_SCOPE)
#   else()
#     message("Failed to locate ${module}. Please specify paths manually.")
#   endif()
#
#   mark_as_advanced(${module}_INCLUDE_DIRS
#            ${module}_LIBRARY_DIRS
#            ${module}_LIBRARIES
#            ${module}_DEFINITIONS
#            ${module}_FOUND)
# endmacro()
#
#
# #
# ### Macro: set_component_found
# #
# # Marks the given component as found if both *_LIBRARIES AND *_INCLUDE_DIRS is present.
# #
# macro(set_component_found module component)
#   set_component_alias(${module} ${component})
#
#   # message("${ALIAS_LIBRARIES}=${${ALIAS_LIBRARIES}}")
#   # message("${ALIAS_INCLUDE_DIRS}=${${ALIAS_INCLUDE_DIRS}}")
#   # message("${ALIAS_LIBRARY_DIRS}=${${ALIAS_LIBRARY_DIRS}}")
#
#   #if (${module}_${component}_LIBRARIES AND ${module}_${component}_INCLUDE_DIRS)
#   if (${ALIAS_LIBRARIES}) # AND ${ALIAS_INCLUDE_DIRS} (XXX_INCLUDE_DIRS may be empty)
#     # message(STATUS "  - ${module} ${component} found.")
#     set(${ALIAS_FOUND} TRUE)
#     # set(${ALIAS_FOUND} TRUE PARENT_SCOPE)
#
#     # Add component vars to the parent module lists
#     append_unique_list(${module}_INCLUDE_DIRS ${ALIAS_INCLUDE_DIRS})
#     append_unique_list(${module}_LIBRARY_DIRS ${ALIAS_LIBRARY_DIRS})
#     append_unique_list(${module}_LIBRARIES  ${ALIAS_LIBRARIES})
#     append_unique_list(${module}_DEFINITIONS  ${ALIAS_DEFINITIONS})
#
#     # set(${module}_INCLUDE_DIRS ${${module}_INCLUDE_DIRS} PARENT_SCOPE)
#     # set(${module}_LIBRARY_DIRS ${${module}_LIBRARY_DIRS} PARENT_SCOPE)
#     # set(${module}_LIBRARIES  ${${module}_LIBRARIES}  PARENT_SCOPE)
#     # set(${module}_DEFINITIONS  ${${module}_DEFINITIONS}  PARENT_SCOPE)
#
#     # message("Find Component Paths=${module}:${component}:${library}:${header}")
#     # message("${ALIAS_INCLUDE_DIRS}=${${ALIAS_INCLUDE_DIRS}}")
#     # message("${ALIAS_RELEASE_LIBRARIES}=${${ALIAS_RELEASE_LIBRARIES}}")
#     # message("${ALIAS_DEBUG_LIBRARIES}=${${ALIAS_DEBUG_LIBRARIES}}")
#     # message("${ALIAS_LIBRARIES}=${${ALIAS_LIBRARIES}}")
#     # message("${module}_INCLUDE_DIRS=${${module}_INCLUDE_DIRS}")
#     # message("${module}_LIBRARIES=${${module}_LIBRARIES}")
#
#     # Only mark as advanced when found
#     mark_as_advanced(
#       ${ALIAS_INCLUDE_DIRS}
#       ${ALIAS_LIBRARY_DIRS})
#
#   else()
#     # NOTE: an error message will be displayed in set_module_found if the module is REQUIRED
#     # message(STATUS "  - ${module} ${component} not found.")
#   endif()
#
#   set(HAVE_${ALIAS} ${${ALIAS_FOUND}})
#
#   get_directory_property(hasParent PARENT_DIRECTORY)
#   if(hasParent)
#     set(HAVE_${ALIAS} ${${ALIAS_FOUND}} PARENT_SCOPE)
#   endif()
#
#   mark_as_advanced(
#       ${ALIAS_FOUND}
#       ${ALIAS_DEBUG_LIBRARIES}
#       ${ALIAS_RELEASE_LIBRARIES}
#       ${ALIAS_LIBRARIES}
#       ${ALIAS_DEFINITIONS}
#       ${ALIAS_VERSION})
# endmacro()
#
#
# #
# ### Macro: set_module_notfound
# #
# # Marks the given component as not found, and resets the cache for find_path and find_library results.
# #
# macro(set_module_notfound module)
#   #message(STATUS "  - Setting ${module} not found.")
#   set(${module}_FOUND FALSE)
#   #set(${module}_FOUND FALSE PARENT_SCOPE)
#
#   if (CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)
#     set(${module}_RELEASE_LIBRARIES "")
#     set(${module}_DEBUG_LIBRARIES "")
#     set(${module}_LIBRARIES "")
#     #set(${module}_RELEASE_LIBRARIES "" PARENT_SCOPE)
#     #set(${module}_DEBUG_LIBRARIES "" PARENT_SCOPE)
#     #set(${module}_LIBRARIES "" PARENT_SCOPE)
#   else()
#     set(${module}_LIBRARIES ${ALIAS_LIBRARIES}-NOTFOUND)
#     #set(${module}_LIBRARIES ${ALIAS_LIBRARIES}-NOTFOUND PARENT_SCOPE)
#   endif()
#
# endmacro()
#
#
# #
# ### Macro: set_component_notfound
# #
# # Marks the given component as not found, and resets the cache for find_path and find_library results.
# #
# macro(set_component_notfound module component)
#   set_component_alias(${module} ${component})
#
#   #message(STATUS "  - Setting ${module} ${component} not found.")
#   set(${ALIAS_FOUND} FALSE)
#   #set(${ALIAS_FOUND} FALSE PARENT_SCOPE)
#
#   if (${module}_MULTI_CONFIGURATION AND (CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE))
#     set(${ALIAS_RELEASE_LIBRARIES} ${ALIAS_RELEASE_LIBRARIES}-NOTFOUND)
#     set(${ALIAS_DEBUG_LIBRARIES} ${ALIAS_DEBUG_LIBRARIES}-NOTFOUND)
#     set(${ALIAS_LIBRARIES} "") #${module}_${component}_LIBRARIES-NOTFOUND)
#     #set(${ALIAS_RELEASE_LIBRARIES} ${ALIAS_RELEASE_LIBRARIES}-NOTFOUND PARENT_SCOPE)
#     #set(${ALIAS_DEBUG_LIBRARIES} ${ALIAS_DEBUG_LIBRARIES}-NOTFOUND PARENT_SCOPE)
#     #set(${ALIAS_LIBRARIES} "") #${module}_${component}_LIBRARIES-NOTFOUND PARENT_SCOPE)
#   else()
#     set(${ALIAS_LIBRARIES} ${ALIAS_LIBRARIES}-NOTFOUND)
#     #set(${ALIAS_LIBRARIES} ${ALIAS_LIBRARIES}-NOTFOUND PARENT_SCOPE)
#   endif()
#
# endmacro()
#
#
# #
# ### Macro: find_component_paths
# #
# # Finds the given component library and include paths.
# #
# macro(find_component_paths module component library header)
#   # message(STATUS "Find Component Paths=${module}:${component}:${library}:${header}")
#
#   # Reset alias namespace (force recheck)
#   # set_component_alias(${module} ${component})
#
#   # Reset search paths (force recheck)
#   # set_component_notfound(${module} ${component})
#
#   find_path(${ALIAS_INCLUDE_DIRS} ${header}
#     PATHS
#       ${${module}_INCLUDE_DIR}   # try find from root module include dir
#       ${${module}_INCLUDE_HINTS} # try find from root module include hints
#     PATH_SUFFIXES
#       ${${module}_INCLUDE_SUFFIXES} # try find from root module include suffixes
#   )
#
#   # Create a Debug and a Release list for multi configuration builds.
#   # NOTE: <module>_CONFIGURATION_TYPES must be set to use this.
#   if (${module}_MULTI_CONFIGURATION AND (CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE))
#     find_library(${ALIAS_RELEASE_LIBRARIES}
#       NAMES
#         ${library}
#       PATHS
#         ${${ALIAS_LIBRARY_DIRS}}
#         ${${module}_LIBRARY_DIR}
#         ${${module}_LIBRARY_HINTS}
#       PATH_SUFFIXES
#         ${${module}_LIBRARY_SUFFIXES}
#     )
#     find_library(${ALIAS_DEBUG_LIBRARIES}
#       NAMES
#         ${library}d
#       PATHS
#         ${${ALIAS_LIBRARY_DIRS}}
#         ${${module}_LIBRARY_DIR}
#         ${${module}_LIBRARY_HINTS}
#       PATH_SUFFIXES
#         ${${module}_LIBRARY_SUFFIXES}
#     )
#     # if (${ALIAS_RELEASE_LIBRARIES})
#     #   list(APPEND ${ALIAS_LIBRARIES} "optimized" ${${ALIAS_RELEASE_LIBRARIES}})
#     # endif()
#     # if (${ALIAS_DEBUG_LIBRARIES})
#     #   list(APPEND ${ALIAS_LIBRARIES} "debug" ${${ALIAS_DEBUG_LIBRARIES}})
#     # endif()
#     include(${CMAKE_ROOT}/Modules/SelectLibraryConfigurations.cmake)
#     select_library_configurations(${ALIAS})
#   else()
#     find_library(${ALIAS_LIBRARIES}
#       NAMES
#         ${library}
#       PATHS
#         ${${ALIAS_LIBRARY_DIRS}}
#         ${${module}_LIBRARY_DIR}
#         ${${module}_LIBRARY_HINTS}
#       PATH_SUFFIXES
#         ${${module}_LIBRARY_SUFFIXES}
#     )
#   endif()
#
#   set_component_found(${module} ${component})
# endmacro()
#
#
# #
# ### Macro: find_component
# #
# # Checks for the given component by invoking pkgconfig and then looking up the
# # libraries and include directories.
# #
# macro(find_component module component pkgconfig library header)
#   # message("Find Component=${module}:${component}:${pkgconfig}:${library}:${header}")
#
#   # Reset component alias values (force recheck)
#   set_component_alias(${module} ${component})
#
#   # Use pkg-config to obtain directories for the find_path() and find_library() calls.
#   find_package(PkgConfig QUIET)
#   if (PKG_CONFIG_FOUND)
#     pkg_search_module(${ALIAS} ${pkgconfig} QUIET)
#     # message(STATUS "Find Component PkgConfig=${ALIAS}:${${ALIAS}_FOUND}:${${ALIAS}_LIBRARIES}:${${ALIAS}_INCLUDE_DIRS}:${${ALIAS}_LIBRARY_DIRS}:${${ALIAS}_LIBDIR}:${${ALIAS}_INCLUDEDIR}")
#   endif()
#
#   # message(STATUS "${ALIAS_FOUND}=${${ALIAS_FOUND}}")
#   # message(STATUS "${ALIAS_LIBRARIES}=${${ALIAS_LIBRARIES}}")
#   # message(STATUS "${ALIAS_INCLUDE_DIRS}=${${ALIAS_INCLUDE_DIRS}}")
#
#   if(NOT ${ALIAS_FOUND})
#     # message(STATUS "  - ${module} ${component} pkg-config not found, searching...")
#     find_component_paths(${module} ${component} ${library} ${header})
#   else()
#     # message(STATUS "  - ${module} ${component} pkg-config found.")
#     set_component_found(${module} ${component})
#   endif()
# endmacro()
