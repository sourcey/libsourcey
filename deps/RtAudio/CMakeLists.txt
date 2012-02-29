# ----------------------------------------------------------------------------
#  CMake file for RtAudio. See root CMakeLists.txt
# ----------------------------------------------------------------------------
project(rtaudio)

# List of C++ files:
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# The .cpp files:
file(GLOB lib_srcs *.cpp)
file(GLOB lib_hdrs *.h)


# ----------------------------------------------------------------------------------
# Define the library target:
# ----------------------------------------------------------------------------------

set(the_target "rtaudio")

add_library(${the_target} STATIC ${lib_srcs} ${lib_hdrs})

set_target_properties(${the_target} PROPERTIES 
	OUTPUT_NAME "${the_target}"
	DEBUG_POSTFIX "${LibSourcey_DEBUG_POSTFIX}"
	ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/deps/lib)
	
#if(ENABLE_SOLUTION_FOLDERS)
#	set_target_properties(${the_target} PROPERTIES FOLDER "deps")
#endif()	
    
#if(NOT BUILD_SHARED_LIBS)
  install(TARGETS ${the_target}
    ARCHIVE DESTINATION share/LibSourcey/deps/lib COMPONENT main)
#endif()
