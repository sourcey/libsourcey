# this is template for a LibSourcey module 
macro(define_libsourcey_module name)
            
    project(${name})
    
    #set(name "${name}")

    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
        
    foreach(d ${ARGN})
        include_directories("${CMAKE_SOURCE_DIR}/${d}/include")
    endforeach()

    # Add library source files
    file(GLOB_RECURSE lib_srcs "src/*.cpp")
    file(GLOB_RECURSE lib_int_hdrs "src/*.h*")
    
    source_group("Src" FILES ${lib_srcs} ${lib_int_hdrs})
    
    # Add library header files
    if(${name} MATCHES "Base")
      set(lib_hdrs_path "")
    elseif(${name} MATCHES "Spot")
      set(lib_hdrs_path "Spot/")
    else()
      set(lib_hdrs_path "${name}/")
    endif()
    file(GLOB_RECURSE lib_hdrs "${CMAKE_CURRENT_SOURCE_DIR}/include/Sourcey/${lib_hdrs_path}*.h*")
    source_group("Include" FILES ${lib_hdrs})

    add_library(${name} ${lib_srcs} ${lib_hdrs} ${lib_int_hdrs})

    # For dynamic link numbering convenions
    if(NOT ANDROID)
        # Android SDK build scripts can include only .so files into final .apk
        # As result we should not set version properties for Android
        set_target_properties(${name} PROPERTIES
            VERSION ${LIBSOURCEY_VERSION}
            SOVERSION ${LIBSOURCEY_SOVERSION})
    endif()

    set_target_properties(${name} PROPERTIES OUTPUT_NAME "Sourcey${name}${LIBSOURCEY_DLLVERSION}" LINKER_LANGUAGE CXX)
    
    #add_dependencies(${name} ${LIBSOURCEY_DEBUG_LIBS} ${LIBSOURCEY_RELEASE_LIBS} ${ARGN})

    # Additional target properties
    set_target_properties(${name} PROPERTIES
        DEBUG_POSTFIX "${LIBSOURCEY_DEBUG_POSTFIX}"
        ARCHIVE_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}
        RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
        INSTALL_NAME_DIR lib)
      
    # Add dependencies and required libraries for linking
    foreach(lib IN LISTS LIBSOURCEY_DEBUG_LIBS)
        target_link_libraries(${name} debug ${lib})
    endforeach()
    foreach(lib IN LISTS LIBSOURCEY_RELEASE_LIBS)
        target_link_libraries(${name} optimized ${lib})
    endforeach()

    install(TARGETS ${name}
        RUNTIME DESTINATION bin COMPONENT main
        LIBRARY DESTINATION lib COMPONENT main
        ARCHIVE DESTINATION lib COMPONENT main)        
    
    # ----------------------------------------------------------------------------
    #   Build Samples
    # ----------------------------------------------------------------------------
    if(BUILD_SAMPLES AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/samples)
        add_subdirectory(samples)
    endif() 
    
    # ----------------------------------------------------------------------------
    #   Build Tests
    # ----------------------------------------------------------------------------
    if(BUILD_TESTS AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tests)
        add_subdirectory(tests)
    endif()    
        
endmacro()
