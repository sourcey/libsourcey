############################### Poco ################################

#set(BUILD_WITH_SHARED_POCO ON CACHE BOOL "Build with Poco Shared Libraries")
set(POCO_SOURCE_DIR "${LIBSOURCEY_SOURCE_DIR}/3rdparty/poco" CACHE PATH "Poco Source Location")
set(POCO_LIBRARY_DIR "${POCO_SOURCE_DIR}/lib" CACHE PATH "Poco Library Location")
set(POCO_INCLUDE_DIRS "${POCO_SOURCE_DIR}/Foundation/include" 
                      "${POCO_SOURCE_DIR}/Crypto/include" 
                      "${POCO_SOURCE_DIR}/Net/include" 
                      "${POCO_SOURCE_DIR}/NetSSL_OpenSSL/include" 
                      "${POCO_SOURCE_DIR}/Util/include" 
                      "${POCO_SOURCE_DIR}/XML/include" 
                      "${POCO_SOURCE_DIR}/Zip/include")
   
if(WIN32 AND MSVC)
  if(BUILD_WITH_STATIC_CRT)
    set(POCO_LIB_SUFFIX'mt')
  endif()  
endif()

set(POCO_DEBUG_LIBS "PocoFoundation${POCO_LIB_POSTFIX}d" 
                    "PocoCrypto${POCO_LIB_POSTFIX}d" 
                    "PocoNet${POCO_LIB_POSTFIX}d" 
                    "PocoNetSSL${POCO_LIB_POSTFIX}d"
                    "PocoUtil${POCO_LIB_POSTFIX}d" 
                    "PocoXML${POCO_LIB_POSTFIX}d" 
                    "PocoZip${POCO_LIB_POSTFIX}d")
set(POCO_RELEASE_LIBS "PocoFoundation${POCO_LIB_POSTFIX}" 
                      "PocoCrypto${POCO_LIB_POSTFIX}" 
                      "PocoNet${POCO_LIB_POSTFIX}" 
                      "PocoNetSSL${POCO_LIB_POSTFIX}" 
                      "PocoUtil${POCO_LIB_POSTFIX}" 
                      "PocoXML${POCO_LIB_POSTFIX}"
                      "PocoZip${POCO_LIB_POSTFIX}")
      
include_directories(${POCO_INCLUDE_DIRS})  
link_directories(${POCO_LIBRARY_DIR})
                      
set(LIBSOURCEY_INCLUDE_DIRS ${LIBSOURCEY_INCLUDE_DIRS} ${POCO_INCLUDE_DIRS})
set(LIBSOURCEY_LIBRARY_DIRS ${LIBSOURCEY_LIBRARY_DIRS} ${POCO_LIBRARY_DIR})
set(LIBSOURCEY_DEBUG_LIBS   ${LIBSOURCEY_DEBUG_LIBS}   ${POCO_DEBUG_LIBS})   
set(LIBSOURCEY_RELEASE_LIBS ${LIBSOURCEY_RELEASE_LIBS} ${POCO_RELEASE_LIBS}) 
