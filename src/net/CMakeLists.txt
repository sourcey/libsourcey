ask_build_sourcey_module(net)
if(BUILD_MODULES OR (NOT BUILD_MODULES AND BUILD_MODULE_net))
  include_dependency(SSL REQUIRED)

  define_sourcey_module(net base crypto)
endif()
