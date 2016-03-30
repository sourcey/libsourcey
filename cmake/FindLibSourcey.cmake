# Try to find LibSourcey library installation
# See http://sourcey.com/libsourcey
#
# The following variables are optionally searched for defaults
#  LIBSOURCEY_FIND_COMPONENTS : find_package(LIBSOURCEY COMPONENTS ..)
#    compatible interface. typically archo base crypto http .. etc.
#
# The following are set after configuration is done:
#  LIBSOURCEY_FOUND
#  LIBSOURCEY_INCLUDE_DIRS
#  LIBSOURCEY_LIBRARIES
#  LIBSOURCEY_LIBRARY_DIRS
# ----------------------------------------------------------------------

# ----------------------------------------------------------------------
# Default LibSourcey components to include if COMPONENTS is undefined
# ----------------------------------------------------------------------
if(NOT LIBSOURCEY_FIND_COMPONENTS)
  set(LIBSOURCEY_FIND_COMPONENTS archo base crypto http json media net sked socketio stun symple turn util uv)
endif()

# ----------------------------------------------------------------------
# Options
# ----------------------------------------------------------------------
set(LIBSOURCEY_INCLUDE_DIRS ${CMAKE_INSTALL_PREFIX}/include CACHE PATH   "The LibSourcey include directory." FORCE)
set(LIBSOURCEY_LIBRARY_DIRS ${CMAKE_INSTALL_PREFIX}/lib     CACHE PATH   "The LibSourcey library directories." FORCE)
set(LIBSOURCEY_LIBRARIES    ""                              CACHE STRING "The LibSourcey libraries." FORCE)
set(LIBSOURCEY_LINK_SHARED_LIBS TRUE                        CACHE BOOL   "Link with shared LibSourcey libraries (.dll/.so) instead of static ones (.lib/.a)")

# ----------------------------------------------------------------------
# Find component libraries
# ----------------------------------------------------------------------
find_package(PkgConfig QUIET)
if (PKG_CONFIG_FOUND)
  # Use pkg-config to find LibSourcey if available
  pkg_search_module(LIBSOURCEY libsourcey)
else()

  # Find LibSourcey library directory
  # set(LIBSOURCEY_LIBRARY_DIR LIBSOURCEY_LIBRARY_DIR-NOTFOUND)
  find_path(LIBSOURCEY_LIBRARY_DIR
    NAMES
      scy_base
    PATHS
      /usr/local/lib
      /usr/lib
    DOC
      "LibSourcey Library Directory")

  # Find LibSourcey include directory
  # set(LIBSOURCEY_INCLUDE_DIR LIBSOURCEY_INCLUDE_DIR-NOTFOUND)
  find_path(LIBSOURCEY_INCLUDE_DIR
    NAMES
      scy/base.h
    PATHS
  	  /usr/local/include
      /usr/include
    DOC
      "LibSourcey Include Directory")

  # Include LibSourcey component libraries
  foreach(component ${LIBSOURCEY_FIND_COMPONENTS})
    if(CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)
      list(APPEND LIBSOURCEY_LIBRARIES debug scy_${component})
      list(APPEND LIBSOURCEY_LIBRARIES optimized scy_${component})
    else()
      list(APPEND LIBSOURCEY_LIBRARIES scy_${component})
    endif()
  endforeach()
endif()

# Include LibSourcey vendor libraries and includes
if(LIBSOURCEY_LIBRARY_DIRS)
  set(LIBSOURCEY_VENDOR_LIBRARY_DIR ${LIBSOURCEY_LIBRARY_DIR}/../share/scy/vendor/lib)
  set(LIBSOURCEY_VENDOR_INCLUDE_DIR ${LIBSOURCEY_INCLUDE_DIR}/../share/scy/vendor/include)

  if (WIN32)
    file(GLOB_RECURSE libraries "${LIBSOURCEY_VENDOR_LIBRARY_DIR}/*.lib")
  else()
    if (LIBSOURCEY_LINK_SHARED_LIBS)
      file(GLOB_RECURSE libraries "${LIBSOURCEY_VENDOR_LIBRARY_DIR}/*.so")
    else()
      file(GLOB_RECURSE libraries "${LIBSOURCEY_VENDOR_LIBRARY_DIR}/*.a")
    endif()
  endif()

  # Include LibSourcey vendor libraries
  set(LIBSOURCEY_LIBRARIES "")
  foreach(lib ${libraries})
    get_filename_component(filename ${lib} NAME)
    if(CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)
      list(APPEND LIBSOURCEY_LIBRARIES debug ${filename})
      list(APPEND LIBSOURCEY_LIBRARIES optimized ${filename})
    else()
      list(APPEND LIBSOURCEY_LIBRARIES ${filename})
    endif()
  endforeach()
endif()

# message("LIBSOURCEY_FOUND=${LIBSOURCEY_FOUND}")
# message("LIBSOURCEY_LIBRARIES=${LIBSOURCEY_LIBRARIES}")
# message("LIBSOURCEY_STATIC_LIBRARIES=${LIBSOURCEY_STATIC_LIBRARIES}")
# message("LIBSOURCEY_INCLUDE_DIRS=${LIBSOURCEY_INCLUDE_DIRS}")
# message("LIBSOURCEY_LIBRARY_DIRS=${LIBSOURCEY_LIBRARY_DIRS}")
# message("LIBSOURCEY_DEFINITIONS=${LIBSOURCEY_DEFINITIONS}")
# message("LIBSOURCEY_VERSION=${LIBSOURCEY_VERSION}")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LIBSOURCEY DEFAULT_MSG
                                  LIBSOURCEY_FOUND
                                  LIBSOURCEY_LIBRARIES
                                  LIBSOURCEY_INCLUDE_DIRS
                                  LIBSOURCEY_LIBRARY_DIRS)
