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
  include_directories(${LibSourcey_INCLUDE_DIRS})
  #  ${CMAKE_SOURCE_DIR}/projects/Anionu/ISpot/include
  #  ${CMAKE_SOURCE_DIR}/projects/AnionuPrivate/SpotImpl/include
  link_directories(${LibSourcey_LIBRARY_DIRS})

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

  # Include dependent modules
  foreach(module ${ARGN})
    include_sourcey_modules(${module})
    add_dependencies(${name} ${module})
  endforeach()

  # KLUDGE: Include all thrid party dependencies for now
  if (LibSourcey_BUILD_DEPENDENCIES)
    add_dependencies(${name} ${LibSourcey_BUILD_DEPENDENCIES})
  endif()

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
  target_link_libraries(${name} ${LibSourcey_INCLUDE_LIBRARIES})

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
