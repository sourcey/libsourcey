#if(WITH_RTAUDIO AND NOT RTAUDIO_FOUND)
    add_subdirectory(RtAudio)
#endif()

#if(WITH_PUGIXML AND NOT PUGIXML_FOUND)
    add_subdirectory(pugixml)
#endif()

#if(WITH_JSONCPP AND NOT JSONCPP_FOUND)
    add_subdirectory(jsoncpp)
#endif()

#if(WITH_OPENSSL AND NOT OPENSSL_FOUND)    
#    add_subdirectory(openssl)
#endif()

#if(WITH_LIBSTROPHE AND NOT LIBSTROPHE_FOUND)
    add_subdirectory(libstrophe)
#endif()

#if(WITH_MEMLEAKDETECT AND NOT MEMLEAKDETECT_FOUND)
#    add_subdirectory(MemLeakDetect)
#endif()
