ask_build_sourcey_module(XML)
if(BUILD_MODULE_XML) 
  include_dependency(Poco REQUIRED)
  include_dependency(OpenSSL REQUIRED)
  include_dependency(PugiXML REQUIRED)

  define_sourcey_module(XML Base)
endif()
