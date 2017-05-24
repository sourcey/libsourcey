#
### Macro: define_sourcey_module
#
# Defines a generic LibSourcey module.
#
macro(define_sourcey_module name)
  project(${name})

  # Add library source files
  file(GLOB_RECURSE lib_srcs "src/*.cpp")
  file(GLOB_RECURSE lib_hdrs "include/*.h")

  # Exclude platform dependent implementations
  set(lib_srcs_exclude_regex "")
  set(lib_hdrs_exclude_regex "")
  if(NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
    # set(lib_srcs_exclude_regex ${lib_srcs_exclude_regex} " *_win32.cpp")
    # set(lib_hdrs_exclude_regex ${lib_hdrs_exclude_regex} " *_win32.h")
    set(lib_srcs_exclude_regex ${lib_srcs_exclude_regex} ".*win32.*")
    set(lib_hdrs_exclude_regex ${lib_hdrs_exclude_regex} ".*win32.*")
  endif()
  if(NOT CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    # set(lib_srcs_exclude_regex ${lib_srcs_exclude_regex} " *_mac.cpp")
    # set(lib_hdrs_exclude_regex ${lib_hdrs_exclude_regex} " *_mac.h")
    set(lib_srcs_exclude_regex ${lib_srcs_exclude_regex} ".*apple.*")
    set(lib_hdrs_exclude_regex ${lib_hdrs_exclude_regex} ".*apple.*")
  endif()
  if(NOT CMAKE_SYSTEM_NAME STREQUAL "Linux")
    # set(lib_srcs_exclude_regex ${lib_srcs_exclude_regex} " *_linux.cpp")
    # set(lib_hdrs_exclude_regex ${lib_hdrs_exclude_regex} " *_linux.h")
    set(lib_srcs_exclude_regex ${lib_srcs_exclude_regex} ".*linux.*")
    set(lib_hdrs_exclude_regex ${lib_hdrs_exclude_regex} ".*linux.*")
  endif()

  join("${lib_srcs_exclude_regex}" "|" lib_srcs_exclude_regex)
  join("${lib_hdrs_exclude_regex}" "|" lib_hdrs_exclude_regex)

  # Include Objective-C files on Apple
  if(APPLE)
    file(GLOB_RECURSE lib_objectivec_srcs "src/*.mm")
    set(lib_srcs ${lib_objectivec_srcs} ${lib_srcs})
  endif()

  filter_list(lib_srcs "${lib_srcs}" ${lib_srcs_exclude_regex})
  filter_list(lib_hdrs "${lib_hdrs}" ${lib_hdrs_exclude_regex})

  list(APPEND LibSourcey_INCLUDE_DIRS ${dir}/include)

  # string(REGEX REPLACE ${lib_srcs_exclude_regex} "" lib_srcs "${lib_srcs}")
  # string(REGEX REPLACE ${lib_hdrs_exclude_regex} "" lib_hdrs "${lib_hdrs}")

  # if(name STREQUAL "media")
  #  message(FATAL_ERROR "${CMAKE_SYSTEM_NAME} ${lib_srcs}")
  # endif()

  source_group("Src" FILES ${lib_srcs})
  source_group("Include" FILES ${lib_hdrs})

  add_library(${name} ${lib_srcs} ${lib_hdrs})

  # Include library and header directories
  set_default_project_directories(${name} ${ARGN})

  # Include linker dependencies
  set_default_project_dependencies(${name} ${ARGN})

  message(STATUS "Including module ${name}")
  #message(STATUS "    Libraries: ${LibSourcey_INCLUDE_LIBRARIES}")
  #message(STATUS "    Library Dirs: ${LibSourcey_LIBRARY_DIRS}")
  #message(STATUS "    Include Dirs: ${LibSourcey_INCLUDE_DIRS}")
  #message(STATUS "    Dependencies: ${LibSourcey__VENDOR_INCLUDE_DIRS}")

  # Specify the library pretty name for shared EXPORT symbols.
  # Unless specified, default is `{Name}_EXPORTS` with uppercased
  # first letter for vanity, just because it looks better in code.
  # NOTE: We could be using `generate_export_header` with
  # CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS but doing it ourselves is cleaner.
  if (NOT ${name}_PRETTY_NAME)
    string(SUBSTRING ${name} 0 1 first_letter)
    string(TOUPPER ${first_letter} first_letter)
    string(REGEX REPLACE "^.(.*)" "${first_letter}\\1" name_capitalized "${name}")
    set(pretty_name ${name_capitalized})
  else()
    set(pretty_name ${${name}_PRETTY_NAME})
  endif()
  target_compile_definitions(${name} PRIVATE ${pretty_name}_EXPORTS)

  # include(GenerateExportHeader)
  # generate_export_header(${name}
  #         EXPORT_FILE_NAME "${CMAKE_BINARY_DIR}/${name}/exports.h"
  #         EXPORT_MACRO_NAME ${pretty_name}_EXPORT
  #         NO_EXPORT_MACRO_NAME ${pretty_name}_NO_EXPORT
  #         DEPRECATED_MACRO_NAME ${pretty_name}_DEPRECATED
  #         NO_DEPRECATED_MACRO_NAME ${pretty_name}_NO_DEPRECATED)

  if(NOT ANDROID)
    # Android SDK build scripts can include only .so files into final .apk
    # As result we should not set version properties for Android
    set_target_properties(${name} PROPERTIES
      VERSION "${LibSourcey_VERSION}"
      SOVERSION "${LibSourcey_SOVERSION}")
  endif()

  # Additional target properties
  if(ENABLE_SOLUTION_FOLDERS)
    set_target_properties(${name} PROPERTIES FOLDER "modules")
  endif()
  set_scy_libname(${name} lib_name)
  set_target_properties(${name} PROPERTIES
    OUTPUT_NAME ${lib_name}
    DEBUG_POSTFIX "${LibSourcey_DEBUG_POSTFIX}"
    #ARCHIVE_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}
    #RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
    #DEFINE_SYMBOL "${pretty_name}_EXPORTS"
    INSTALL_NAME_DIR lib
    LINKER_LANGUAGE CXX)

  # Add install routine, unless lib is header only
  if (lib_srcs)
    install(TARGETS ${name}
      RUNTIME DESTINATION bin COMPONENT libs
      LIBRARY DESTINATION lib COMPONENT libs
      ARCHIVE DESTINATION lib COMPONENT dev)
  endif()

  install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include"
    DESTINATION "${LibSourcey_INSTALL_DIR}"
    COMPONENT dev)

  # Set HAVE_SOURCEY_XXX at parent scope for use in libsourcey.h
  set(HAVE_SOURCEY_${name} ON PARENT_SCOPE)

  # set(LibSourcey_MODULE_LIBRARY_DIRS ${LibSourcey_MODULE_LIBRARY_DIRS} ${LibSourcey_BUILD_DIR}/src/${name} PARENT_SCOPE)
  set(LibSourcey_MODULE_INCLUDE_DIRS ${LibSourcey_MODULE_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)
  set(LibSourcey_MODULE_INCLUDE_LIBRARIES ${LibSourcey_MODULE_INCLUDE_LIBRARIES} ${name} PARENT_SCOPE)

  # Build samples
  if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/samples)
    ask_build_sourcey_sample(${name})
    if(BUILD_SAMPLES OR (NOT BUILD_SAMPLES AND BUILD_SAMPLES_${name}))
      add_subdirectory(samples)
    endif()
  endif()

  # Build tests
  if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tests)
    ask_build_sourcey_test(${name})
    if(BUILD_TESTS OR (NOT BUILD_TESTS AND BUILD_TESTS_${name}))
    # if(BUILD_TESTS AND BUILD_TESTS_${name})
      add_subdirectory(tests)
    endif()
  endif()
endmacro()


#
### Macro: define_sourcey_module_sample
#
# Defines a generic LibSourcey sample application.
#
macro(define_sourcey_module_sample name)

  project(${name})

  # Include internal module dependencies
  #include_sourcey_modules(${ARGN})

  # Add source files
  file(GLOB lib_hdrs "*.h*")
  file(GLOB lib_srcs "*.cpp")

  source_group("Src" FILES ${lib_srcs})
  source_group("Include" FILES ${lib_hdrs})

  message(STATUS "Including module sample ${name}")
  #message(STATUS "    Libraries: ${LibSourcey_INCLUDE_LIBRARIES}")
  #message(STATUS "    Library Dirs: ${LibSourcey_LIBRARY_DIRS}")
  #message(STATUS "    Include Dirs: ${LibSourcey_INCLUDE_DIRS}")

  add_executable(${name} ${lib_srcs} ${lib_hdrs})

  # Include library and header directories
  set_default_project_directories(${name} ${ARGN})

  # Include linker dependencies
  set_default_project_dependencies(${name} ${ARGN})

  if(ENABLE_SOLUTION_FOLDERS)
    set_target_properties(${name} PROPERTIES FOLDER "samples")
  endif()
  set_target_properties(${name} PROPERTIES DEBUG_POSTFIX "d")

  install(TARGETS ${name} RUNTIME DESTINATION "${LibSourcey_SHARED_INSTALL_DIR}/samples/${name}" COMPONENT samples)
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

  # Include library and header directories
  set_default_project_directories(${name} ${ARGN})

  # Include linker dependencies
  set_default_project_dependencies(${name} ${ARGN})

  message(STATUS "Including module test ${name}")
  # message(STATUS "    Libraries: ${LibSourcey_INCLUDE_LIBRARIES}")
  # message(STATUS "    Library Dirs: ${LibSourcey_LIBRARY_DIRS}")
  # message(STATUS "    Include Dirs: ${LibSourcey_INCLUDE_DIRS}")

  # Add ctest support
  if(NOT ${name}_EXCLUDE_CTEST)
    add_test(${name} ${name})
    #message(STATUS "Adding test ${name}")
  endif()

  if(ENABLE_SOLUTION_FOLDERS)
    set_target_properties(${name} PROPERTIES FOLDER "tests")
  endif()
  set_target_properties(${name} PROPERTIES DEBUG_POSTFIX "")

  install(TARGETS ${name} RUNTIME DESTINATION "${LibSourcey_SHARED_INSTALL_DIR}/tests/${name}" COMPONENT tests)
endmacro()


#
### Macro: define_libsourcey_library
#
# Defines a generic LibSourcey shared library.
#
macro(define_libsourcey_library name)
  project(${name})

  # Add source files
  file(GLOB lib_hdrs "*.h*")
  file(GLOB lib_srcs "*.cpp")

  add_definitions(-DSOURCEY_BUILD_SHARED)

  source_group("Src" FILES ${lib_srcs})
  source_group("Include" FILES ${lib_hdrs})

  add_library(${name} MODULE ${lib_srcs} ${lib_hdrs})

  # Include library and header directories
  set_default_project_directories(${name} ${ARGN})

  # Include linker dependencies
  set_default_project_dependencies(${name} ${ARGN})

  message(STATUS "Including module library ${name}")
  #message(STATUS "    Libraries: ${LibSourcey_INCLUDE_LIBRARIES}")
  #message(STATUS "    Library Dirs: ${LibSourcey_LIBRARY_DIRS}")
  #message(STATUS "    Include Dirs: ${LibSourcey_INCLUDE_DIRS}")

  if(ENABLE_SOLUTION_FOLDERS)
    set_target_properties(${name} PROPERTIES FOLDER "libraries")
  endif()
  set_target_properties(${name} PROPERTIES DEBUG_POSTFIX "d")

  install(TARGETS ${name} DESTINATION "bin/${name}" COMPONENT libs)
endmacro()


#
### Macro: define_sourcey_application
#
# This template defines a LibSourcey application.
# Optional helper variables:
#   <name>_SUBSYSTEM_WINDOWS - Sets SUBSYSTEM:WINDOWS under WIN32
#   <name>_EXECUTABLE_NAME - The name of the executable file
#   <name>_DEBUG_POSTFIX - The output file name debum postfix
#
macro(define_sourcey_application name)
  project(${name})

  # Add library source files
  file(GLOB_RECURSE lib_srcs "src/*.c*")
  file(GLOB_RECURSE lib_hdrs "src/*.h*")

  # If we build for windows systems, we also include the resource file
  # containing the manifest, icon and other resources
  if(WIN32 AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/${name}.rc")
    set(lib_srcs ${lib_srcs} src/${name}.rc)
  endif()

  source_group("Source" FILES ${lib_srcs})
  source_group("Include" FILES ${lib_hdrs})

  # Include all header and library directories
  # include_directories(${LibSourcey_INCLUDE_DIRS})
  #  ${CMAKE_SOURCE_DIR}/projects/Anionu/ISpot/include
  #  ${CMAKE_SOURCE_DIR}/projects/AnionuPrivate/SpotImpl/include
  # link_directories(${LibSourcey_LIBRARY_DIRS})

  status("Linking application ${name}")
  # status("    Libraries:               ${LibSourcey_INCLUDE_LIBRARIES}")
  # status("    Library Dirs:            ${LibSourcey_LIBRARY_DIRS}")
  # status("    Include Dirs:            ${LibSourcey_INCLUDE_DIRS}")
  # status("    Dependencies:            ${LibSourcey_BUILD_DEPENDENCIES}")

  # Setting WIN32 sets SUBSYSTEM:WINDOWS
  if (${name}_SUBSYSTEM_WINDOWS)
    add_executable(${name} WIN32 ${lib_srcs} ${lib_hdrs})
  else()
    add_executable(${name} ${lib_srcs} ${lib_hdrs})
  endif()

  # Include library and header directories
  set_default_project_directories(${name} ${ARGN})

  # Include linker dependencies
  set_default_project_dependencies(${name} ${ARGN})

  # Add external dependencies and required libraries for linking.
  # target_link_libraries(${name} ${LibSourcey_INCLUDE_LIBRARIES})

  if(ENABLE_SOLUTION_FOLDERS)
    set_target_properties(${name} PROPERTIES FOLDER "applications")
  endif()

  if (NOT ${name}_EXECUTABLE_NAME)
    set(${name}_EXECUTABLE_NAME ${name})
  endif()
  if (NOT ${name}_DEBUG_POSTFIX AND NOT ${name}_DEBUG_POSTFIX STREQUAL "")
    set(${name}_DEBUG_POSTFIX ${LibSourcey_DEBUG_POSTFIX})
  endif()
  set_target_properties(${name} PROPERTIES
    OUTPUT_NAME ${${name}_EXECUTABLE_NAME}
    DEBUG_POSTFIX "${${name}_DEBUG_POSTFIX}")

  if (NOT ${name}_INSTALL_DESTINATION)
    # Must add /bin for Linux
    # This will result in executables being installed to /usr/local/bin
    set(${name}_INSTALL_DESTINATION "bin")
  endif()
  install(TARGETS ${name}
    RUNTIME DESTINATION ${${name}_INSTALL_DESTINATION} COMPONENT apps)
endmacro()


#
### Macro: define_sourcey_dependency
#
# This template defines a LibSourcey dependency.
# Optional helper variables:
#   <name>_OUTPUT_NAME - The name of the executable file
#   <name>_DEBUG_POSTFIX - The output file name debug postfix
#   <name>_SOURCE_PATH - The glob source path
#   <name>_HEADER_PATH - The glob header path
#   <name>_SOURCE_FILES - The glob source files
#   <name>_HEADER_FILES - The glob header files
#
macro(define_sourcey_dependency name)
  project(${name})

  # Include current directory and existing dependency directories
  # include_directories("${CMAKE_CURRENT_SOURCE_DIR}") # "${LibSourcey_INCLUDE_DIRS}"

  # Add library source files
  if (NOT ${name}_SOURCE_FILES)
    if (NOT ${name}_SOURCE_PATH)
      set(${name}_SOURCE_PATH "src/*.c*")
    endif()
    file(GLOB_RECURSE ${name}_SOURCE_FILES ${${name}_SOURCE_PATH})
    #message("${name}: Globbing source files: ${${name}_SOURCE_FILES}")
  endif()
  #message("${name}: Source files: ${${name}_SOURCE_FILES}")

  # Add library header files
  if (NOT ${name}_HEADER_FILES)
    if (NOT ${name}_HEADER_PATH)
      set(${name}_HEADER_PATH "src/*.h*")
    endif()
    file(GLOB_RECURSE ${name}_HEADER_FILES ${${name}_HEADER_PATH})
  endif()

  source_group("Source" FILES ${${name}_SOURCE_FILES})
  source_group("Include" FILES ${${name}_HEADER_FILES})

  message(STATUS "- Linking dependency ${name} with libraries: ${LibSourcey_INCLUDE_LIBRARIES}")
  # message("${name}: Library Dirs: ${LibSourcey_LIBRARY_DIRS}")
  # message("${name}: Include Dirs: ${LibSourcey_INCLUDE_DIRS}")

  add_library(${name} ${LibSourcey_LIB_TYPE} ${${name}_SOURCE_FILES}) # ${${name}_HEADER_FILES}

  if (${name}_DEFINITIONS)
    target_compile_definitions(${name} PRIVATE ${${name}_DEFINITIONS})
  endif()
  if (${name}_FLAGS)
    target_compile_options(${name} PRIVATE ${${name}_FLAGS})
  endif()
  if (${name}_INCLUDE_DIRS)
    target_include_directories(${name} PRIVATE ${${name}_INCLUDE_DIRS})
  endif()
  if (${name}_DEPENDENCIES)
    target_link_libraries(${name} PRIVATE ${${name}_DEPENDENCIES} ${CMAKE_DL_LIBS})
    # add_dependencies(${name} ${${name}_DEPENDENCIES})
  endif()

  # Cache dependency directories for inclusion by modules and applications
  # get_directory_property(lib_directories INCLUDE_DIRECTORIES)
  # set(LibSourcey_VENDOR_INCLUDE_DIRS ${lib_directories})
  # set(LibSourcey_VENDOR_INCLUDE_DIRS ${lib_directories} PARENT_SCOPE)
  # set(LibSourcey_LINK_LIBRARIES ${LibSourcey_LINK_LIBRARIES} ${name} PARENT_SCOPE)
  # set(LibSourcey_BUILD_DEPENDENCIES ${LibSourcey_BUILD_DEPENDENCIES} ${name} PARENT_SCOPE)

  if (${name}_OUTPUT_NAME)
    set_target_properties(${name} PROPERTIES OUTPUT_NAME ${${name}_OUTPUT_NAME})
  endif()
  if (${name}_DEBUG_POSTFIX)
    set_target_properties(${name} PROPERTIES DEBUG_POSTFIX ${${name}_DEBUG_POSTFIX})
  elseif(LibSourcey_DEBUG_POSTFIX)
    set_target_properties(${name} PROPERTIES DEBUG_POSTFIX ${LibSourcey_DEBUG_POSTFIX})
  endif()
  if(ENABLE_SOLUTION_FOLDERS)
    set_target_properties(${name} PROPERTIES FOLDER "dependencies")
  endif()

  if (NOT INSTALL_DESTINATION)
    set(INSTALL_DESTINATION ${LibSourcey_VENDOR_INSTALL_DIR}/lib)
  endif()
  install(TARGETS ${name}
    RUNTIME DESTINATION ${INSTALL_DESTINATION} COMPONENT libs
    LIBRARY DESTINATION ${INSTALL_DESTINATION} COMPONENT libs
    ARCHIVE DESTINATION ${INSTALL_DESTINATION} COMPONENT dev)

  # install header include files
  install(FILES ${${name}_HEADER_FILES} DESTINATION ${LibSourcey_VENDOR_INSTALL_DIR}/include COMPONENT dev)
endmacro()
