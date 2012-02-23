############################### JsonCpp ################################

set(JSONCPP_SOURCE_DIR "${LIBSOURCEY_SOURCE_DIR}/3rdparty/jsoncpp/include" CACHE PATH "JsonCpp Source Location")
set(JSONCPP_LIBRARY_DIR "${LIBSOURCEY_INSTALL_PREFIX}/share/LibSourcey/3rdparty/lib" CACHE PATH "JsonCpp Library Location")

set(JSONCPP_DEBUG_LIBS "jsoncppd")  
set(JSONCPP_RELEASE_LIBS "jsoncpp")

#set(JSONCPP_FOUND 1)
      
include_directories(${JSONCPP_SOURCE_DIR})  
link_directories(${JSONCPP_LIBRARY_DIR})
                        
set(LIBSOURCEY_INCLUDE_DIRS ${LIBSOURCEY_INCLUDE_DIRS} ${JSONCPP_SOURCE_DIR})
set(LIBSOURCEY_LIBRARY_DIRS ${LIBSOURCEY_LIBRARY_DIRS} ${JSONCPP_LIBRARY_DIR})
set(LIBSOURCEY_DEBUG_LIBS   ${LIBSOURCEY_DEBUG_LIBS}   ${JSONCPP_DEBUG_LIBS})    
set(LIBSOURCEY_RELEASE_LIBS ${LIBSOURCEY_RELEASE_LIBS} ${JSONCPP_RELEASE_LIBS})