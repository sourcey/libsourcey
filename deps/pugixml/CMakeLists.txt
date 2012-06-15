# ----------------------------------------------------------------------------
#  CMake file for pugixml. See root CMakeLists.txt
#
# ----------------------------------------------------------------------------
project(pugixml)

# List of C++ files:

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src")

# The .cpp files:
file(GLOB lib_srcs src/*.cpp)
file(GLOB lib_hdrs src/*.hpp)

# ----------------------------------------------------------------------------------
# 				Define the library target:
# ----------------------------------------------------------------------------------

set(the_target "pugixml")

add_library(${the_target} STATIC ${lib_srcs} ${lib_hdrs})

#if(MSVC)
#    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W3")
#endif()

#if(UNIX)
#     if(CMAKE_COMPILER_IS_GNUCXX OR CV_ICC)
#       set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
#     endif()
#endif()

#if(CMAKE_COMPILER_IS_GNUCXX)
#    set_source_files_properties(jcdctmgr.c PROPERTIES COMPILE_FLAGS "-O1")
#endif()

set_target_properties(${the_target}
	PROPERTIES OUTPUT_NAME "${the_target}"
	DEBUG_POSTFIX "${LibSourcey_DEBUG_POSTFIX}"
	ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/deps/lib
	)
	
#if(ENABLE_SOLUTION_FOLDERS)
#	set_target_properties(${the_target} PROPERTIES FOLDER "deps")
#endif()	
    
#if(NOT BUILD_SHARED_LIBS)
  install(TARGETS ${the_target}
    ARCHIVE DESTINATION ${LibSourcey_DEPENDENCIES_INSTALL_DIR}/lib COMPONENT main)
#endif()
