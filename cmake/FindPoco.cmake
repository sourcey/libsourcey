# vim: ts=2 sw=2
# - Try to find the required ffmpeg components(default: Zip Net NetSSL Crypto Util XML Foundation)
#
# Once done this will define
#  Poco_FOUND         - System has the all required components.
#  Poco_INCLUDE_DIRS  - Include directory necessary for using the required components headers.
#  Poco_LIBRARIES     - Link these to use the required ffmpeg components.
#  Poco_DEFINITIONS   - Compiler switches required for using the required ffmpeg components.
#
# For each of the components it will additionaly set.
#   - Foundation
#   - CppUnit
#   - Net
#   - NetSSL
#   - Crypto
#   - Util
#   - XML
#   - Zip
#   - Data
#   - PageCompiler
#
# the following variables will be defined
#  <component>_FOUND        - System has <component>
#  <component>_INCLUDE_DIRS - Include directory necessary for using the <component> headers
#  <component>_LIBRARIES    - Link these to use <component>
#  <component>_DEFINITIONS  - Compiler switches required for using <component>
#  <component>_VERSION      - The components version
#
# Copyright (c) 2006, Matthias Kretz, <kretz@kde.org>
# Copyright (c) 2008, Alexander Neundorf, <neundorf@kde.org>
# Copyright (c) 2011, Michael Jansen, <kde@michael-jansen.biz>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
  
include(LibSourceyMacros)
include(FindPackageHandleStandardArgs)

# The default components were taken from a survey over other FindPoco.cmake files
if (NOT Poco_FIND_COMPONENTS)
  set(Poco_FIND_COMPONENTS Zip Net NetSSL Crypto Util XML Foundation)
endif()

# Check for cached results. If there are skip the costly part.
if (NOT Poco_FOUND) # Poco_LIBRARIES

  # Set the library suffix for our build type
  set(Poco_LINK_SHARED_LIBS TRUE CACHE BOOL "Link with shared Poco libraries (.dll/.so) instead of static ones (.lib/.a)")    
  set(Poco_LIB_SUFFIX "")
  #set(Poco_LIB_EXT "")
  if(WIN32) #  AND MSVC    
    add_definitions(-DPOCO_NO_AUTOMATIC_LIBS)
    if(Poco_LINK_SHARED_LIBS)
      add_definitions(-DPOCO_DLL)
    else()
      add_definitions(-DPOCO_STATIC)
      if(BUILD_WITH_STATIC_CRT)
        set(Poco_LIB_SUFFIX "mt")
      else()
        set(Poco_LIB_SUFFIX "md")
      endif()    
    endif()
  endif()

  # Set search path suffixes for windows
  if(WIN32)
    set(Poco_PATH_SUFFIXES
      /CppUnit/include
      /Data/include
      /Foundation/include
      /Crypto/include
      /Net/include
      /NetSSL_OpenSSL/include
      /PageCompiler/include
      /Util/include
      /XML/include
      /Zip/include)
  endif()
      
  # Check for all possible components.      
  find_multi_component(Poco Foundation   Foundation  PocoFoundation${Poco_LIB_SUFFIX}  Poco/Foundation.h)
  find_multi_component(Poco CppUnit      CppUnit     PocoCppUnit${Poco_LIB_SUFFIX}     Poco/CppUnit/CppUnit.h)
  find_multi_component(Poco Net          Net         PocoNet${Poco_LIB_SUFFIX}         Poco/Net/Net.h)
  find_multi_component(Poco NetSSL       NetSSL      PocoNetSSL${Poco_LIB_SUFFIX}      Poco/Net/NetSSL.h)
  find_multi_component(Poco Crypto       Crypto      PocoCrypto${Poco_LIB_SUFFIX}      Poco/Crypto/Crypto.h)
  find_multi_component(Poco Util         Util        PocoUtil${Poco_LIB_SUFFIX}        Poco/Util/Util.h)
  find_multi_component(Poco XML          XML         PocoXML${Poco_LIB_SUFFIX}         Poco/XML/XML.h)
  find_multi_component(Poco Zip          Zip         PocoZip${Poco_LIB_SUFFIX}         Poco/Zip/Zip.h)
  find_multi_component(Poco Data         Data        PocoData${Poco_LIB_SUFFIX}        Poco/Data/Data.h)
  find_multi_component(Poco PageCompiler PageCompiler PocoPageCompiler Poco/PageCompiler/PageCompiler.h)  
  
  # Set Poco as found. Any missing required components will set this to false.
  set(Poco_FOUND 1)
  set(Poco_INCLUDE_DIRS "")
  set(Poco_LIBRARIES "")

  # Check if the required components were found and add their stuff to the Poco_* vars.
  foreach (_component ${Poco_FIND_COMPONENTS})
    if (${_component}_FOUND)
      # message(STATUS "Required component ${_component} present.")
      set(Poco_LIBRARIES   ${Poco_LIBRARIES}   ${${_component}_LIBRARIES})
      set(Poco_DEFINITIONS ${Poco_DEFINITIONS} ${${_component}_DEFINITIONS})
      list(APPEND Poco_INCLUDE_DIRS ${${_component}_INCLUDE_DIRS})
    else ()
      # message(ERROR "Required component ${_component} missing.")
      set(Poco_FOUND 0)
    endif()
  endforeach()
  
  # If we are on windows find_component will not be able to resolve the include paths
  # so we need to add them manually
  
  
#  find_path(Poco_DIR 
#    NAMES 
#      Foundation/include/Poco/Foundation.h 
#    PATH_SUFFIXES 
#      ${Poco_DIR_SUFFIXES} 
#    PATHS
#      ${Poco_POSSIBLE_ROOT_DIRS}
#    DOC 
#      "The ${Poco_INCLUDE_PATH_DESCRIPTION}"
#  )

  # Build the include path with duplicates removed.
  #if (Poco_INCLUDE_DIRS)
  #  list(REMOVE_DUPLICATES Poco_INCLUDE_DIRS)
  #endif ()
  #if (Poco_LIBRARIES)
  #  list(REMOVE_DUPLICATES Poco_LIBRARIES)
  #endif ()
  
  # Stop Poco for undefining windows stuff
  add_definitions(-DPOCO_NO_UNWINDOWS)

  # cache the vars.
  set(Poco_INCLUDE_DIRS ${Poco_INCLUDE_DIRS} CACHE STRING "The Poco include directories." FORCE)
  set(Poco_LIBRARIES    ${Poco_LIBRARIES}    CACHE STRING "The Poco libraries." FORCE)
  set(Poco_DEFINITIONS  ${Poco_DEFINITIONS}  CACHE STRING "The Poco cflags." FORCE)

  mark_as_advanced(Poco_INCLUDE_DIRS
                   Poco_LIBRARIES
                   Poco_DEFINITIONS)

endif ()

# Now set the noncached _FOUND vars for the components.
#Zip Net NetSSL Crypto Util XML Foundation
foreach (_component Net NetSSL Crypto Util XML Foundation Zip Data CppUnit PageCompiler)
  set_component_found(${_component})
endforeach ()
  
# Compile the list of required vars
set(_Poco_REQUIRED_VARS Poco_LIBRARIES Poco_INCLUDE_DIRS)
foreach (_component ${Poco_FIND_COMPONENTS})
  list(APPEND _Poco_REQUIRED_VARS ${_component}_LIBRARIES ${_component}_INCLUDE_DIRS)
endforeach ()

# Give a nice error message if some of the required vars are missing.
find_package_handle_standard_args(Poco DEFAULT_MSG ${_Poco_REQUIRED_VARS})