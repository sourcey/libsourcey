############################### wxWidgets ################################

set(WXWIDGETS_SOURCE_PATH "${LIBSOURCEY_SOURCE_DIR}/3rdparty/wxWidgets" CACHE PATH "wxWidgets Install Location")
set(WXWIDGETS_INCLUDE_PATH "${WXWIDGETS_SOURCE_PATH}/include")
set(WXWIDGETS_LIBRARY_DIR "${WXWIDGETS_SOURCE_PATH}/lib")

# TODO: More compiler types for wxWidgets
if(MSVC)
  set(LIBSOURCEY_INCLUDE_DIRS ${LIBSOURCEY_INCLUDE_DIRS} "${WXWIDGETS_INCLUDE_PATH}/msvc")
  set(WXWIDGETS_LIBRARY_DIR "${WXWIDGETS_LIBRARY_DIR}/vc_lib")
else()
  message("wxWidgets libs not included. Please setup for ${CMAKE_SYSTEM_PROCESSOR}")
endif()

set(WXWIDGETS_LIBRARY_DIR ${WXWIDGETS_LIBRARY_DIR} CACHE PATH "wxWidgets Library Location")

set(WXWIDGETS_DEBUG_LIBS "wxbase29ud" 
                         "wxbase29ud_net" 
                         "wxbase29ud_xml" 
                         "wxmsw29ud_core" 
                         "wxmsw29ud_adv" 
                         "wxmsw29ud_core" 
                         "wxmsw29ud_core")

set(WXWIDGETS_RELEASE_LIBS "wxbase29u" 
                           "wxbase29u_net" 
                           "wxbase29u_xml" 
                           "wxmsw29u_core" 
                           "wxmsw29u_adv" 
                           "wxmsw29u_core" 
                           "wxmsw29u_core")
      
include_directories(${WXWIDGETS_INCLUDE_PATH})  
link_directories(${WXWIDGETS_LIBRARY_DIR})
                      
set(LIBSOURCEY_INCLUDE_DIRS ${LIBSOURCEY_INCLUDE_DIRS} ${WXWIDGETS_INCLUDE_PATH})
set(LIBSOURCEY_LIBRARY_DIRS ${LIBSOURCEY_LIBRARY_DIRS} ${WXWIDGETS_LIBRARY_DIR})
set(LIBSOURCEY_DEBUG_LIBS   ${LIBSOURCEY_DEBUG_LIBS}   ${WXWIDGETS_DEBUG_LIBS})    
set(LIBSOURCEY_RELEASE_LIBS ${LIBSOURCEY_RELEASE_LIBS} ${WXWIDGETS_RELEASE_LIBS})    