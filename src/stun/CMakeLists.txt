ask_build_sourcey_module(stun)
if(BUILD_MODULES OR (NOT BUILD_MODULES AND BUILD_MODULE_stun))
  # include_dependency(LibUV) #REQUIRED
  include_dependency(SSL REQUIRED)

  set(stun_PRETTY_NAME STUN)
  define_sourcey_module(stun base net crypto)
endif()
