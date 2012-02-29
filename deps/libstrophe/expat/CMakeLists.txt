# ----------------------------------------------------------------------------
#  CMake file for expat. See root CMakeLists.txt
# ----------------------------------------------------------------------------
project(expat)

# List of C files:
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/lib")

# The .c files:
file(GLOB lib_srcs lib/*.c)
set(lib_hdrs lib/expat.h lib/expat_external.h)
	
# See xmlparse.c for config inclusion rules
if(WIN32)
    add_definitions(-DCOMPILED_FROM_DSP)  
elseif(APPLE)
    add_definitions(-DMACOS_CLASSIC)  
endif()


# ----------------------------------------------------------------------------
#  Define the library target:
# ----------------------------------------------------------------------------
set(the_target "expat")

add_library(${the_target} STATIC ${lib_srcs} ${lib_hdrs})

if(WIN32 AND MSVC)    
    if(BUILD_WITH_STATIC_CRT)
        set(output_name "lib${the_target}MT")   
    else()  
        set(output_name "lib${the_target}MD")   
    endif()  
else()
    set(output_name ${the_target}) 
endif()

set_target_properties(${the_target}
    PROPERTIES 
    OUTPUT_NAME "${output_name}"
    LINKER_LANGUAGE C
    DEBUG_POSTFIX "${LibSourcey_DEBUG_POSTFIX}"
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/deps/lib
    )
	
#if(ENABLE_SOLUTION_FOLDERS)
#	set_target_properties(${the_target} PROPERTIES FOLDER "deps")
#endif()	
    
#if(NOT BUILD_SHARED_LIBS)
    install(TARGETS ${the_target}
	    ARCHIVE DESTINATION share/LibSourcey/deps/lib COMPONENT main)
#endif()
