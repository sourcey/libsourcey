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
  if(NOT WIN32)
    set(lib_srcs_exclude ${lib_srcs_exclude} " *_win32.cpp")
    set(lib_hdrs_exclude ${lib_hdrs_exclude} " *_win32.h")
  endif()
  if(NOT UNIX)
    set(lib_srcs_exclude ${lib_srcs_exclude} " *_linux.cpp")
    set(lib_hdrs_exclude ${lib_hdrs_exclude} " *_linux.h")
  endif()
  if(NOT APPLE)
    set(lib_srcs_exclude ${lib_srcs_exclude} " *_mac.cpp")
    set(lib_hdrs_exclude ${lib_hdrs_exclude} " *_mac.h")
  endif()
  JOIN("${lib_srcs_exclude}" "|" lib_srcs_exclude)
  JOIN("${lib_hdrs_exclude}" "|" lib_hdrs_exclude)

  #STRING(CHOP ${lib_srcs_exclude} 1 lib_srcs_exclude)
  #STRING(CHOP ${lib_hdrs_exclude} 1 lib_hdrs_exclude)

  #string(REGEX REPLACE " *_WIN32.cpp| *_LINUX.cpp| *_UNIX.cpp| *_MAC.cpp| *_NULL.cpp" "" lib_srcs "${lib_srcs}")
  #string(REGEX REPLACE " *_WIN32.h| *_LINUX.h| *_UNIX.h| *_MAC.h| *_FAKE.h" "" lib_hdrs "${lib_hdrs}")
  #message(${lib_srcs_exclude} )
  #message(${lib_hdrs_exclude} )
  string(REGEX REPLACE ${lib_srcs_exclude} "" lib_srcs "${lib_srcs}")
  string(REGEX REPLACE ${lib_hdrs_exclude} "" lib_hdrs "${lib_hdrs}")

  source_group("Src" FILES ${lib_srcs})
  source_group("Include" FILES ${lib_hdrs})

  add_library(${name} ${lib_srcs} ${lib_hdrs})

  # Include linker dependencies
  set_default_project_dependencies(${name} ${ARGN})

  #message(STATUS "Defining module ${name}:")
  #message(STATUS "    Libraries: ${LibSourcey_INCLUDE_LIBRARIES}")
  #message(STATUS "    Library Dirs: ${LibSourcey_LIBRARY_DIRS}")
  #message(STATUS "    Include Dirs: ${LibSourcey_INCLUDE_DIRS}")

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

  #message(STATUS "Defining module sample ${name}:")
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

  #message(STATUS "Defining module test ${name}:")
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

  #message(STATUS "Defining module library ${name}:")
  #message(STATUS "    Libraries: ${LibSourcey_INCLUDE_LIBRARIES}")
  #message(STATUS "    Library Dirs: ${LibSourcey_LIBRARY_DIRS}")
  #message(STATUS "    Include Dirs: ${LibSourcey_INCLUDE_DIRS}")

  if(ENABLE_SOLUTION_FOLDERS)
    set_target_properties(${name} PROPERTIES FOLDER "libraries")
  endif()
  set_target_properties(${name} PROPERTIES DEBUG_POSTFIX "d")

  install(TARGETS ${name} DESTINATION "bin/${name}" COMPONENT main)
endmacro()
