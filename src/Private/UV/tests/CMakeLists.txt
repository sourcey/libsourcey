#include_dependency(Poco REQUIRED)
include_dependency(OpenSSL REQUIRED)
set(CMAKE_CXX_STANDARD_LIBRARIES ${CMAKE_CXX_STANDARD_LIBRARIES} advapi32.lib iphlpapi.lib psapi.lib ws2_32.lib shell32.lib)  
  
define_libsourcey_test(UVTests Base Net UV)  
#define_libsourcey_test(MediaTests Base Media Net) #VideoSocket Anionu Base Net XML HTTP 
