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

  # Check for all possible components.      
  find_component(Foundation   Foundation  PocoFoundation  Poco/Foundation.h)
  find_component(CppUnit      CppUnit     PocoCppUnit     Poco/CppUnit/CppUnit.h)
  find_component(Net          Net         PocoNet         Poco/Net/Net.h)
  find_component(NetSSL       NetSSL      PocoNetSSL      Poco/Net/NetSSL.h)
  find_component(Crypto       Crypto      PocoCrypto      Poco/Crypto/Crypto.h)
  find_component(Util         Util        PocoUtil        Poco/Util/Util.h)
  find_component(XML          XML         PocoXML         Poco/XML/XML.h)
  find_component(Zip          Zip         PocoZip         Poco/Zip/Zip.h)
  find_component(Data         Data        PocoData        Poco/Data/Data.h)
  find_component(PageCompiler PageCompiler PocoPageCompiler Poco/PageCompiler/PageCompiler.h)
  
  # Set Poco as found. Any missing components will set this to false.
  set(Poco_FOUND 1)

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
    endif ()
  endforeach ()

  # Build the include path with duplicates removed.
  if (Poco_INCLUDE_DIRS)
    list(REMOVE_DUPLICATES Poco_INCLUDE_DIRS)
  endif ()
  if (Poco_LIBRARIES)
    list(REMOVE_DUPLICATES Poco_LIBRARIES)
  endif ()
  
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