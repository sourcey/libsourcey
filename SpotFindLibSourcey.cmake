############################### LibSourcey ################################

set(LibSourcey_SOURCE_DIR "${CMAKE_SOURCE_DIR}/../LibSourcey" CACHE PATH "LibSourcey Source Location")

if(WIN32)
    set(LibSourcey_INSTALL_PREFIX "${LibSourcey_SOURCE_DIR}/build/install" CACHE PATH "LibSourcey Install Location")
else()
    set(LibSourcey_INSTALL_PREFIX "/usr/local" CACHE PATH "LibSourcey Install Location")
endif()

set(LibSourcey_VERSION "0.8.0" CACHE PATH "LibSourcey Version")                           

string(REGEX MATCHALL "[0-9]" LibSourcey_VERSION_PARTS "${LibSourcey_VERSION}")

list(GET LibSourcey_VERSION_PARTS 0 LibSourcey_VERSION_MAJOR)
list(GET LibSourcey_VERSION_PARTS 1 LibSourcey_VERSION_MINOR)
list(GET LibSourcey_VERSION_PARTS 2 LibSourcey_VERSION_PATCH)

#set(LibSourcey_SOVERSION "${LibSourcey_VERSION_MAJOR}.${LibSourcey_VERSION_MINOR}")
#
#if(WIN32)
#    # Postfix of DLLs:
#    set(LibSourcey_VERSION_SUFFIX "${LibSourcey_VERSION_MAJOR}${LibSourcey_VERSION_MINOR}${LibSourcey_VERSION_PATCH}")
#    set(LibSourcey_DEBUG_POSTFIX d)
#else()
#    # Postfix of so's:
#    set(LibSourcey_VERSION_SUFFIX "")
#    set(LibSourcey_DEBUG_POSTFIX)
#endif()

set(LibSourcey_VERSION_SUFFIX "${LibSourcey_VERSION_MAJOR}${LibSourcey_VERSION_MINOR}${LibSourcey_VERSION_PATCH}")  
                         
set(LibSourcey_LIBRARY_DIRS "${LibSourcey_INSTALL_PREFIX}/lib" 
                            "${LibSourcey_INSTALL_PREFIX}/share/LibSourcey/3rdparty/lib") 
                            
foreach(m Anionu Base ICE ISpot Media Net HTTP RTP SDP SpotImpl STUN TURN XML XMPP Pacman JSON SocketIO Symple)
  set(LibSourcey_INCLUDE_DIRS ${LibSourcey_INCLUDE_DIRS} "${LibSourcey_SOURCE_DIR}/${m}/include")
  set(LibSourcey_DEBUG_LIBS ${LibSourcey_DEBUG_LIBS} "Sourcey${m}${LibSourcey_VERSION_SUFFIX}d")
  set(LibSourcey_RELEASE_LIBS ${LibSourcey_RELEASE_LIBS} "Sourcey${m}${LibSourcey_VERSION_SUFFIX}")
endforeach()
      
include_directories(${LibSourcey_INCLUDE_DIRS})  
link_directories(${LibSourcey_LIBRARY_DIRS})
                      
set(LibSourcey_INCLUDE_DIRS ${LibSourcey_INCLUDE_DIRS} ${LibSourcey_INCLUDE_DIRS})
set(LibSourcey_LIBRARY_DIRS ${LibSourcey_LIBRARY_DIRS} ${LibSourcey_LIBRARY_DIR})
set(LibSourcey_DEBUG_LIBS   ${LibSourcey_DEBUG_LIBS}   ${LibSourcey_DEBUG_LIBS})   
set(LibSourcey_RELEASE_LIBS ${LibSourcey_RELEASE_LIBS} ${LibSourcey_RELEASE_LIBS}) 
