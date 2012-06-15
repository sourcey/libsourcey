# ----------------------------------------------------------------------------
#  CMake file for libstrophe. See root CMakeLists.txt
#
# ----------------------------------------------------------------------------
project(libstrophe)

# Include directories:
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src")

# Include source files:
set(lib_srcs src/auth.c src/conn.c src/ctx.c 
  src/event.c src/handler.c src/hash.c 
  src/jid.c src/md5.c src/sasl.c src/sha1.c 
  src/snprintf.c src/sock.c src/stanza.c src/thread.c 
  src/tls_openssl.c src/util.c src/parser.c src/tls_openssl.c)
file(GLOB lib_hdrs *.h src/*.h)

if(MSVC)
  add_definitions(-DXML_STATIC)
endif()

# ----------------------------------------------------------------------------------
# Library dependencies:
# ----------------------------------------------------------------------------------
  
# Compile expat with libstrophe. 
# No need to maintain an extra library.
if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/expat")
  include_directories("${CMAKE_CURRENT_SOURCE_DIR}/expat/lib")
  #include_directories("${CMAKE_CURRENT_SOURCE_DIR}/lib")

  # Include source files:
  file(GLOB expat_lib_srcs expat/lib/*.c)
  set(expat_lib_hdrs expat/lib/expat.h expat/lib/expat_external.h)
    
  # See xmlparse.c for config inclusion rules
  if(WIN32)
      add_definitions(-DCOMPILED_FROM_DSP)  
  elseif(APPLE)
      add_definitions(-DMACOS_CLASSIC)  
  endif()
  add_definitions(-DHAVE_MEMMOVE)  
endif()  
  
# Include OpenSSL
find_package(OpenSSL REQUIRED)

# Cmake OPENSSL find module uses old style uppercase variable scope.
# Check for both in case this changes in the future.
if(OPENSSL_INCLUDE_DIR)
  include_directories(${OPENSSL_INCLUDE_DIR})
elseif(OpenSSL_INCLUDE_DIR)
  include_directories(${OpenSSL_INCLUDE_DIR})
endif()



# ----------------------------------------------------------------------------------
# Define the library target:
# ----------------------------------------------------------------------------------

set(the_target "libstrophe")

add_library(${the_target} STATIC ${lib_srcs} ${lib_hdrs} ${expat_lib_srcs} ${expat_lib_hdrs})

set_target_properties(${the_target}
  PROPERTIES 
  PREFIX ""
  OUTPUT_NAME "${the_target}"
  DEBUG_POSTFIX "${LibSourcey_DEBUG_POSTFIX}"
  ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/deps/lib
  LINKER_LANGUAGE C)	
	
#if(ENABLE_SOLUTION_FOLDERS)
# set_target_properties(${the_target} PROPERTIES FOLDER "deps")
#endif()	
    
#if(NOT BUILD_SHARED_LIBS)
  install(TARGETS ${the_target}
    ARCHIVE DESTINATION ${LibSourcey_DEPENDENCIES_INSTALL_DIR}/lib COMPONENT main)
#endif()
