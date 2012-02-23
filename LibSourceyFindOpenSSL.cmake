############################### OpenSSL ################################

set(OPENSSL_INCLUDE_DIR "${LIBSOURCEY_SOURCE_DIR}/3rdparty/openssl/include" CACHE PATH "OpenSSL Include Location")       
set(OPENSSL_LIBRARY_DIR "${LIBSOURCEY_SOURCE_DIR}/3rdparty/openssl/lib")     
        
#set(OPENSSL_LIBRARY_DIR "${LIBSOURCEY_SOURCE_DIR}/3rdparty/openssl" CACHE PATH "OpenSSL Library Location")                 
#set(OPENSSL_INCLUDE_DIR "${OPENSSL_INCLUDE_DIR}/include")  

if(LINUX)
  # TODO
  set(OPENSSL_LIBRARY_DIR "${OPENSSL_LIBRARY_DIR}/MinGW")
elseif(WIN32)
  if(MSVC)
    set(OPENSSL_LIBRARY_DIR "${OPENSSL_LIBRARY_DIR}/VC")    
    if(NOT BUILD_SHARED_LIBS) 
      set(OPENSSL_LIBRARY_DIR "${OPENSSL_LIBRARY_DIR}/static")
    endif()
    
    # Set the library runtime SUFFIX for MSVC
    if(BUILD_WITH_STATIC_CRT)
      set(OPENSSL_LIB_SUFFIX MT)   
    else()  
      set(OPENSSL_LIB_SUFFIX MD)   
    endif()  
  else()
    set(OPENSSL_LIBRARY_DIR "${OPENSSL_LIBRARY_DIR}/MinGW")
  endif() 
endif()
            
set(OPENSSL_LIBRARY_DIR "${OPENSSL_LIBRARY_DIR}" CACHE PATH "OpenSSL Library Location")

set(OPENSSL_DEBUG_LIBS "ssleay32${OPENSSL_LIB_SUFFIX}d" 
                       "libeay32${OPENSSL_LIB_SUFFIX}d")
                    
set(OPENSSL_RELEASE_LIBS "ssleay32${OPENSSL_LIB_SUFFIX}" 
                         "libeay32${OPENSSL_LIB_SUFFIX}")

set(OPENSSL_FOUND 1)
      
include_directories(${OPENSSL_INCLUDE_DIR})  
link_directories(${OPENSSL_LIBRARY_DIR})
                      
set(LIBSOURCEY_INCLUDE_DIRS ${LIBSOURCEY_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR})
set(LIBSOURCEY_LIBRARY_DIRS ${LIBSOURCEY_LIBRARY_DIRS} ${OPENSSL_LIBRARY_DIR})
set(LIBSOURCEY_DEBUG_LIBS   ${LIBSOURCEY_DEBUG_LIBS}   ${OPENSSL_DEBUG_LIBS})    
set(LIBSOURCEY_RELEASE_LIBS ${LIBSOURCEY_RELEASE_LIBS} ${OPENSSL_RELEASE_LIBS})    