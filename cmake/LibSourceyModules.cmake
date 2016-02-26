#
### Macro: define_sourcey_module
#
# Defines a generic LibSourcey module.
#
macro(define_sourcey_module name)

  project(${name})

  # Include internal module dependencies
  #include_sourcey_modules(${ARGN})

  # Include library and header directories
  set_default_project_directories(${ARGN})

  # Add library source files
  file(GLOB_RECURSE lib_srcs "src/*.cpp")
  file(GLOB_RECURSE lib_hdrs "include/*.h")

  # Exclude platform dependent implementations
  set(lib_srcs_exclude "")
  set(lib_hdrs_exclude "")
  if(NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(lib_srcs_exclude ${lib_srcs_exclude} " *_win32.cpp")
    set(lib_hdrs_exclude ${lib_hdrs_exclude} " *_win32.h")
  endif()
  if(NOT CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    #message(FATAL_ERROR "${CMAKE_SYSTEM_NAME}")
    set(lib_srcs_exclude ${lib_srcs_exclude} " *_mac.cpp")
    set(lib_hdrs_exclude ${lib_hdrs_exclude} " *_mac.h")
  endif()
  if(NOT CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(lib_srcs_exclude ${lib_srcs_exclude} " *_linux.cpp")
    set(lib_hdrs_exclude ${lib_hdrs_exclude} " *_linux.h")
  endif()

  join("${lib_srcs_exclude}" "|" lib_srcs_exclude)
  join("${lib_hdrs_exclude}" "|" lib_hdrs_exclude)

  # Include Objective-C files on Apple
  if(APPLE)
	  file(GLOB_RECURSE lib_objectivec_srcs "*.mm")
	  set(lib_srcs ${lib_objectivec_srcs} ${lib_srcs})
  endif()

  string(REGEX REPLACE ${lib_srcs_exclude} "" lib_srcs "${lib_srcs}")
  string(REGEX REPLACE ${lib_hdrs_exclude} "" lib_hdrs "${lib_hdrs}")

  #if(name STREQUAL "media")
  #  message(FATAL_ERROR "${CMAKE_SYSTEM_NAME} ${lib_srcs}")
  #endif()

  source_group("Src" FILES ${lib_srcs})
  source_group("Include" FILES ${lib_hdrs})

  add_library(${name} ${lib_srcs} ${lib_hdrs})

  # Include linker dependencies
  set_default_project_dependencies(${name} ${ARGN})

  #  message(FATAL_ERROR "${CMAKE_SYSTEM_NAME} ${lib_srcs}")
  message(STATUS "Defining module ${name}")
  if(name STREQUAL "socketio")
    message(STATUS "    Dependencies: ${LibSourcey_BUILD_DEPENDENCIES}")
    message(STATUS "    Libraries: ${LibSourcey_INCLUDE_LIBRARIES}")
    # message(STATUS "    Library Dirs: ${LibSourcey_LIBRARY_DIRS}")
    message(STATUS "    Include Dirs: ${LibSourcey_INCLUDE_DIRS}")
  endif()

  if(NOT ANDROID)
    # Android SDK build scripts can include only .so files into final .apk
    # As result we should not set version properties for Android
    set_target_properties(${name} PROPERTIES
      VERSION ${LibSourcey_VERSION}
      SOVERSION ${LibSourcey_SOVERSION})
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
    INSTALL_NAME_DIR lib
    LINKER_LANGUAGE CXX)

  # Add install routine, unless lib is header only
  if (lib_srcs)
    install(TARGETS ${name}
      RUNTIME DESTINATION bin COMPONENT main
      LIBRARY DESTINATION lib COMPONENT main
      ARCHIVE DESTINATION lib COMPONENT main)
  endif()

  # Set HAVE_SOURCEY_XXX at parent scope for use in scyconfig.h
  set(HAVE_SOURCEY_${name} ON PARENT_SCOPE)

  # Build samples
  if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/samples)
    ask_build_sourcey_sample(${name})
    if(BUILD_MODULE_SAMPLES AND BUILD_SAMPLE_${name})
      add_subdirectory(samples)
    endif()
  endif()

  # Build tests
  if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tests)
    ask_build_sourcey_test(${name})
    if(BUILD_MODULE_TESTS AND BUILD_TEST_${name})
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

  # Include library and header directories
  set_default_project_directories(${ARGN})

  # Add source files
  file(GLOB lib_hdrs "*.h*")
  file(GLOB lib_srcs "*.cpp")

  source_group("Src" FILES ${lib_srcs})
  source_group("Include" FILES ${lib_hdrs})

  add_executable(${name} ${lib_srcs} ${lib_hdrs})

  # Include linker dependencies
  set_default_project_dependencies(${name} ${ARGN})

  message(STATUS "Defining module sample ${name}")
  #message(STATUS "    Libraries: ${LibSourcey_INCLUDE_LIBRARIES}")
  #message(STATUS "    Library Dirs: ${LibSourcey_LIBRARY_DIRS}")
  #message(STATUS "    Include Dirs: ${LibSourcey_INCLUDE_DIRS}")

  if(ENABLE_SOLUTION_FOLDERS)
    set_target_properties(${name} PROPERTIES FOLDER "samples")
  endif()
  set_target_properties(${name} PROPERTIES DEBUG_POSTFIX "d")

  install(TARGETS ${name} RUNTIME DESTINATION "samples/${name}" COMPONENT main)
endmacro()

#
### Macro: define_libsourcey_test
#
# Defines a generic LibSourcey test application.
#
macro(define_libsourcey_test name)

  project(${name})

  # Include internal module dependencies
  #include_sourcey_modules(${ARGN})

  # Include library and header directories
  set_default_project_directories(${ARGN})

  # Add source files
  file(GLOB lib_hdrs "*.h*")
  #file(GLOB lib_srcs "*.cpp")
  set(lib_srcs "${name}.cpp")

  source_group("Src" FILES ${lib_srcs})
  source_group("Include" FILES ${lib_hdrs})

  add_executable(${name} ${lib_srcs} ${lib_hdrs})

  # Include linker dependencies
  set_default_project_dependencies(${name} ${ARGN})

  message(STATUS "Defining module test ${name}")
  #message(STATUS "    Libraries: ${LibSourcey_INCLUDE_LIBRARIES}")
  #message(STATUS "    Library Dirs: ${LibSourcey_LIBRARY_DIRS}")
  #message(STATUS "    Include Dirs: ${LibSourcey_INCLUDE_DIRS}")

  if(ENABLE_SOLUTION_FOLDERS)
    set_target_properties(${name} PROPERTIES FOLDER "tests")
  endif()
  set_target_properties(${name} PROPERTIES DEBUG_POSTFIX "d")

  install(TARGETS ${name} RUNTIME DESTINATION "tests/${name}" COMPONENT main)
endmacro()


#
### Macro: define_libsourcey_library
#
# Defines a generic LibSourcey shared library.
#
macro(define_libsourcey_library name)

  project(${name})

  # Include internal module dependencies
  #include_sourcey_modules(${ARGN})

  # Include library and header directories
  set_default_project_directories(${ARGN})

  # Add source files
  file(GLOB lib_hdrs "*.h*")
  file(GLOB lib_srcs "*.cpp")

  add_definitions(-DSOURCEY_BUILD_SHARED)

  source_group("Src" FILES ${lib_srcs})
  source_group("Include" FILES ${lib_hdrs})

  add_library(${name} MODULE ${lib_srcs} ${lib_hdrs})

  # Include linker dependencies
  set_default_project_dependencies(${name} ${ARGN})

  message(STATUS "Defining module library ${name}")
  #message(STATUS "    Libraries: ${LibSourcey_INCLUDE_LIBRARIES}")
  #message(STATUS "    Library Dirs: ${LibSourcey_LIBRARY_DIRS}")
  #message(STATUS "    Include Dirs: ${LibSourcey_INCLUDE_DIRS}")

  if(ENABLE_SOLUTION_FOLDERS)
    set_target_properties(${name} PROPERTIES FOLDER "libraries")
  endif()
  set_target_properties(${name} PROPERTIES DEBUG_POSTFIX "d")

  install(TARGETS ${name} DESTINATION "bin/${name}" COMPONENT main)
endmacro()

#
### Macro: define_sourcey_application
#
# This template defines a Spot executable.
# Optional helper variables:
#   <name>_SUBSYSTEM_WINDOWS - Sets SUBSYSTEM:WINDOWS under WIN32
#   <name>_EXECUTABLE_NAME - The name of the executable file
#   <name>_DEBUG_POSTFIX - The output file name debum postfix
#
macro(define_sourcey_application name)

  project(${name})

  # Include internal module dependencies
  #include_sourcey_modules(${ARGN})

  # Include library and header directories
  set_default_project_directories(${ARGN})

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
  #include_directories(${LibSourcey_INCLUDE_DIRS})
  #  ${CMAKE_SOURCE_DIR}/projects/Anionu/ISpot/include
  #  ${CMAKE_SOURCE_DIR}/projects/AnionuPrivate/SpotImpl/include
  #link_directories(${LibSourcey_LIBRARY_DIRS})

  #status("  Linking application ${name}")
  #status("    Libraries:               ${LibSourcey_INCLUDE_LIBRARIES}")
  #status("    Library Dirs:            ${LibSourcey_LIBRARY_DIRS}")
  #status("    Include Dirs:            ${LibSourcey_INCLUDE_DIRS}")
  #status("    Dependencies:            ${LibSourcey_BUILD_DEPENDENCIES}")

  # Setting WIN32 sets SUBSYSTEM:WINDOWS
  if (${name}_SUBSYSTEM_WINDOWS)
    add_executable(${name} WIN32 ${lib_srcs} ${lib_hdrs})
  else()
    add_executable(${name} ${lib_srcs} ${lib_hdrs})
  endif()

  # Include linker dependencies
  set_default_project_dependencies(${name} ${ARGN})

  # Include dependent modules
  #foreach(module ${ARGN})
  #  include_sourcey_modules(${module})
  #  add_dependencies(${name} ${module})
  #endforeach()

  # KLUDGE: Include all thrid party dependencies for now
  #if (LibSourcey_BUILD_DEPENDENCIES)
  #  add_dependencies(${name} ${LibSourcey_BUILD_DEPENDENCIES})
  #endif()

  #if(MSVC)
  #  # Temporary workaround for "error LNK2026: module unsafe for SAFESEH image"
  #  # when compiling with certain externally compiled libraries with VS2012,
  #  # such as http://ffmpeg.zeranoe.com/builds/
  #  # This disables safe exception handling by default.
  #  if(${_MACHINE_ARCH_FLAG} MATCHES X86)
  #    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")
  #    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /SAFESEH:NO")
  #    set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /SAFESEH:NO")
  #  endif()
  #endif()

  # Add external dependencies and required libraries for linking.
  #target_link_libraries(${name} ${LibSourcey_INCLUDE_LIBRARIES})

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
    RUNTIME DESTINATION ${${name}_INSTALL_DESTINATION} COMPONENT main)

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
  include_directories("${CMAKE_CURRENT_SOURCE_DIR}" "${LibSourcey_INCLUDE_DIRS}")

  # Add library source files
  if (NOT ${name}_SOURCE_FILES)
    if (NOT ${name}_SOURCE_PATH)
      set(${name}_SOURCE_PATH "src/*.c*")
    endif()
    file(GLOB_RECURSE ${name}_SOURCE_FILES ${${name}_SOURCE_PATH})
    #message("${name}: Globbing source files: ${${name}_SOURCE_FILES}")
  endif()
  #message("${name}: Sourcey files: ${${name}_SOURCE_FILES}")

  # Add library header files
  if (NOT ${name}_HEADER_FILES)
    if (NOT ${name}_HEADER_PATH)
      set(${name}_HEADER_PATH "src/*.h*")
    endif()
    file(GLOB_RECURSE ${name}_HEADER_FILES ${${name}_HEADER_PATH})
  endif()

  source_group("Source" FILES ${${name}_SOURCE_FILES})
  source_group("Include" FILES ${${name}_HEADER_FILES})

  add_library(${name} ${LibSourcey_LIB_TYPE} ${${name}_SOURCE_FILES} ${${name}_HEADER_FILES})

  if (${name}_DEPENDENCIES)
    target_link_libraries(${name} ${${name}_DEPENDENCIES} ${CMAKE_DL_LIBS})
    add_dependencies(${name} ${${name}_DEPENDENCIES})
  endif()

  # Cache dependency directories for inclusion by modules and applications
  get_directory_property(lib_directories INCLUDE_DIRECTORIES)
  set(LibSourcey_INCLUDE_DIRS ${lib_directories} PARENT_SCOPE)
  set(LibSourcey_LINK_LIBRARIES ${LibSourcey_LINK_LIBRARIES} ${name} PARENT_SCOPE)
  set(LibSourcey_BUILD_DEPENDENCIES ${LibSourcey_BUILD_DEPENDENCIES} ${name} PARENT_SCOPE)

  # message(STATUS "- Linking dependency ${name} with libraries: ${LibSourcey_INCLUDE_LIBRARIES}")
  #message("${name}: Library Dirs: ${LibSourcey_LIBRARY_DIRS}")
  #message("${name}: Include Dirs: ${LibSourcey_INCLUDE_DIRS}")

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
  #if(CMAKE_C_FLAGS)
  #  set_target_properties(${name} PROPERTIES COMPILE_FLAGS ${CMAKE_C_FLAGS})
  #endif()

  # message("${name}: OUTPUT_NAME: ${name}_OUTPUT_NAME")
  # message("${name}: DEBUG_POSTFIX: ${name}_DEBUG_POSTFIX")
  # message("${name}: CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

  #set_target_properties(${name} PROPERTIES
  #  OUTPUT_NAME ${${name}_OUTPUT_NAME}
  #  DEBUG_POSTFIX ${${name}_DEBUG_POSTFIX})

  if (NOT INSTALL_DESTINATION)
    set(INSTALL_DESTINATION ${LibSourcey_DEPENDENCIES_INSTALL_DIR}/lib)
  endif()
  install(TARGETS ${name}
    RUNTIME DESTINATION ${INSTALL_DESTINATION} COMPONENT main
    ARCHIVE DESTINATION ${INSTALL_DESTINATION} COMPONENT main
    LIBRARY DESTINATION ${INSTALL_DESTINATION} COMPONENT main)

endmacro()
