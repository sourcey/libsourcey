ask_build_sourcey_module(http)
if(BUILD_MODULES AND BUILD_MODULE_http)

  add_vendor_dependency(HTTPPARSER http_parser)

  #list(APPEND LibSourcey_BUILD_DEPENDENCIES http_parser)
  #list(APPEND LibSourcey_BUILD_DEPENDENCIES http_parser PARENT_SCOPE)
  #set(LibSourcey_BUILD_DEPENDENCIES ${LibSourcey_BUILD_DEPENDENCIES} http_parser PARENT_SCOPE)
  #add_subdirectory(vendor/http_parser)
  #set(HAVE_HTTPPARSER 1)

  include_dependency(LibUV)
  include_dependency(OpenSSL REQUIRED)
#  include_dependency(HttpParser)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/vendor/http_parser)

  define_sourcey_module(http base net uv crypto) # util
endif()
