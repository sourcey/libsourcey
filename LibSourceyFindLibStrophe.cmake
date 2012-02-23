############################### LibStrophe ################################

set(LIBSTROPHE_SOURCE_DIR "${LIBSOURCEY_SOURCE_DIR}/3rdparty/libstrophe" CACHE PATH "LibStrophe Source Location")
set(LIBSTROPHE_LIBRARY_DIR "${LIBSOURCEY_INSTALL_PREFIX}/share/LibSourcey/3rdparty/lib" CACHE PATH "LibStrophe (and expat) Library Location")
set(LIBSTROPHE_INCLUDE_DIRS "${LIBSTROPHE_SOURCE_DIR}"
                            "${LIBSTROPHE_SOURCE_DIR}/src"
                            "${LIBSTROPHE_SOURCE_DIR}/expat/lib")

if(WIN32 AND MSVC)
  if(BUILD_WITH_STATIC_CRT)
    set(EXPAT_LIB_SUFFIX MT)   
  else()  
    set(EXPAT_LIB_SUFFIX MD)   
  endif()  
endif()

set(LIBSTROPHE_DEBUG_LIBS "libexpat${EXPAT_LIB_SUFFIX}d" 
                          "libstrophed")
                    
set(LIBSTROPHE_RELEASE_LIBS "libexpat${EXPAT_LIB_SUFFIX}" 
                            "libstrophe")

#set(LIBSTROPHE_FOUND 1)     
      
include_directories(${LIBSTROPHE_INCLUDE_DIRS})  
link_directories(${LIBSTROPHE_LIBRARY_DIR})                     

set(LIBSOURCEY_INCLUDE_DIRS ${LIBSOURCEY_INCLUDE_DIRS} ${LIBSTROPHE_INCLUDE_DIRS})
set(LIBSOURCEY_LIBRARY_DIRS ${LIBSOURCEY_LIBRARY_DIRS} ${LIBSTROPHE_LIBRARY_DIR})
set(LIBSOURCEY_DEBUG_LIBS   ${LIBSOURCEY_DEBUG_LIBS}   ${LIBSTROPHE_DEBUG_LIBS})
set(LIBSOURCEY_RELEASE_LIBS ${LIBSOURCEY_RELEASE_LIBS} ${LIBSTROPHE_RELEASE_LIBS})    