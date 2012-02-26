# This is template for a LibSourcey sample
macro(define_libsourcey_sample name)
            
    project(${name})    

    # Add example source files
    file(GLOB lib_hdrs "*.h*")
    file(GLOB lib_srcs "*.cpp")
    
    source_group("Src" FILES ${lib_srcs})
    source_group("Include" FILES ${lib_hdrs})

    add_executable(${name} ${lib_srcs} ${lib_hdrs})
     
    set_target_properties(${name} PROPERTIES DEBUG_POSTFIX "d")
        
    foreach(lib ${ARGN})
        target_link_libraries(${name} debug "Sourcey${lib}${LibSourcey_DLLVERSION}d")
        target_link_libraries(${name} optimized "Sourcey${lib}${LibSourcey_DLLVERSION}")
    endforeach()
    foreach(lib IN LISTS LibSourcey_DEBUG_LIBS)
        target_link_libraries(${name} debug ${lib})
    endforeach()
    foreach(lib IN LISTS LibSourcey_RELEASE_LIBS)
        target_link_libraries(${name} optimized ${lib})
    endforeach()

    install(TARGETS ${name} RUNTIME DESTINATION "samples/${name}" COMPONENT main) 
        
endmacro()
