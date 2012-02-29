# - Find the Poco includes and libraries.
# The following variables are set if Poco is found.  If Poco is not
# found, Poco_FOUND is set to false.
#  Poco_FOUND        - True when the Poco include directory is found.
#  Poco_INCLUDE_DIRS - the path to where the poco include files are.
#  Poco_LIBRARY_DIR - The path to where the poco library files are.
#  Poco_BINARY_DIR - The path to where the poco dlls are.
#  Poco_LIBRARIES - list of all libs from requested components.

# ----------------------------------------------------------------------
# If you have installed Poco in a non-standard location.
# Then you have three options.
# In the following comments, it is assumed that <Your Path>
# points to the root directory of the include directory of Poco. e.g
# If you have put poco in C:\development\Poco then <Your Path> is
# "C:/development/Poco" and in this directory there will be two
# directories called "include" and "lib".
# 1) After CMake runs, set Poco_DIR to <Your Path>/poco<-version>
# 2) Use CMAKE_INCLUDE_PATH to set a path to <Your Path>/poco<-version>. This will allow find_path()
#    to locate Poco_DIR by utilizing the PATH_SUFFIXES option. e.g.
#    set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "<Your Path>/include")
# 3) Set an environment variable called ${Poco_ROOT} that points to the root of where you have
#    installed Poco, e.g. <Your Path>. It is assumed that there is at least a subdirectory called
#    Foundation/include/Poco in this path.
#
# Note:
#  1) If you are just using the poco headers, then you do not need to use
#     Poco_LIBRARY_DIR in your CMakeLists.txt file.
#  2) If Poco has not been installed, then when setting Poco_LIBRARY_DIR
#     the script will look for /lib first and, if this fails, then for /stage/lib.
#
# Usage:
# In your CMakeLists.txt file do something like this:
# ...
# # Poco
# find_package(Poco COMPONENTS XML Net Data...)
# ...
# INCLUDE_DIRECTORIES(${Poco_INCLUDE_DIRS})
# LINK_DIRECTORIES(${Poco_LIBRARY_DIR})
#
# In Windows, we make the assumption that, if the Poco files are installed, the default directory
# will be C:\poco or C:\Program Files\Poco or C:\Programme\Poco.


set(Poco_INCLUDE_PATH_DESCRIPTION "top-level directory containing the poco include directories. E.g /usr/local/include/ or c:\\poco\\include\\poco-1.3.2")
set(Poco_DIR_MESSAGE "Set the Poco_DIR cmake cache entry to the ${Poco_INCLUDE_PATH_DESCRIPTION}")
set(Poco_LIBRARY_PATH_DESCRIPTION "top-level directory containing the poco libraries.")
set(Poco_LIBRARY_DIR_MESSAGE "Set the Poco_LIBRARY_DIR cmake cache entry to the ${Poco_LIBRARY_PATH_DESCRIPTION}")


# ----------------------------------------------------------------------
# Default Poco components to include if COMPONENTS is undefined
# ----------------------------------------------------------------------
if(NOT Poco_FIND_COMPONENTS)
  set(Poco_FIND_COMPONENTS Crypto Foundation Net NetSSL Zip) # CppUnit PageCompiler Util XML NetSSL_Poco
endif()


# ----------------------------------------------------------------------
# Standard OpenCV installation paths
# ----------------------------------------------------------------------
set(Poco_POSSIBLE_ROOT_DIRS $ENV{Poco_ROOT})
if(Poco_POSSIBLE_ROOT_DIRS)
  FILE(TO_CMAKE_PATH ${Poco_POSSIBLE_ROOT_DIRS} Poco_POSSIBLE_ROOT_DIRS)
endif(Poco_POSSIBLE_ROOT_DIRS)

if(WIN32)
  set(Poco_POSSIBLE_ROOT_DIRS
    ${Poco_POSSIBLE_ROOT_DIRS}
    C:/poco
    D:/poco
    "C:/Program Files/poco"
    "C:/Programme/poco"
    "D:/Program Files/poco"
    "D:/Programme/poco"
  )
endif()

set(Poco_LIBRARY_DIR_SUFFIXES lib)
if(WIN32 AND CMAKE_SIZEOF_VOID_P EQUAL 8)
  list(APPEND Poco_LIBRARY_DIR_SUFFIXES lib64)
endif()

# Set the library suffix for our build type
set(Poco_LINK_SHARED_LIBS TRUE CACHE BOOL "Link with shared Poco libraries (.dll/.so) instead of static ones (.lib/.a)")    
set(Poco_LIB_SUFFIX "")
#set(Poco_LIB_EXT "")
if(WIN32) #  AND MSVC
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
#else()
#  if(Poco_LINK_SHARED_LIBS)
#    set(Poco_LIB_EXT ".so")
#  else()
#    set(Poco_LIB_EXT ".a")
#  endif()
endif()


# ----------------------------------------------------------------------
# Find Poco installation
# ----------------------------------------------------------------------
find_path(Poco_DIR 
  NAMES 
    Foundation/include/Poco/Foundation.h 
  PATH_SUFFIXES 
    ${Poco_DIR_SUFFIXES} 
  PATHS
    ${Poco_POSSIBLE_ROOT_DIRS}
  DOC 
    "The ${Poco_INCLUDE_PATH_DESCRIPTION}"
)

if(NOT Poco_DIR)
  # Look for standard unix include paths
  find_path(Poco_DIR Poco/Poco.h DOC "The ${Poco_INCLUDE_PATH_DESCRIPTION}")
endif()

# Assume we didn't find it.
set(Poco_FOUND 0)

# Now try to get the include and library path.
if(Poco_DIR)
  if(EXISTS "${Poco_DIR}/Foundation")
    set(Poco_INCLUDE_DIRS
      ${Poco_DIR}/CppUnit/include
      ${Poco_DIR}/Data/include
      ${Poco_DIR}/Foundation/include
      ${Poco_DIR}/Crypto/include
      ${Poco_DIR}/Net/include
      ${Poco_DIR}/NetSSL_OpenSSL/include
      ${Poco_DIR}/PageCompiler/include
      ${Poco_DIR}/Util/include
      ${Poco_DIR}/XML/include
      ${Poco_DIR}/Zip/include)
    set(Poco_FOUND 1)
  elseif(EXISTS "${Poco_DIR}/Poco/Poco.h")
    set(Poco_INCLUDE_DIRS ${Poco_DIR})
    set(Poco_FOUND 1)
  endif()
    
  unset(Poco_LIBRARY_DIR CACHE)
  unset(Poco_LIB_Foundation CACHE)
  unset(Poco_BINARY_DIR CACHE)
    
  if(NOT Poco_LIBRARY_DIR)

    #find_library(Poco_LIB_Foundation 
    #  NAMES 
    #    PocoFoundation.lib #"PocoFoundation${Poco_LIB_SUFFIX}.lib"
    #  PATHS
    #    ${Poco_DIR}/lib
      #  ${Poco_POSSIBLE_ROOT_DIRS}
      #PATH_SUFFIXES 
      #  ${Poco_LIBRARY_DIR_SUFFIXES} 
    #  PATH_SUFFIXES lib
    #  DOC 
    #    "The ${Poco_LIBRARY_PATH_DESCRIPTION}"
    #)
    #PATH_SUFFIXES lib 
    #PATHS D:/dev/lib/poco-1.4.1p1-all
    #PocoFoundation.lib 
    SET(Poco_LIB_Foundation Poco_LIB_Foundation-NOTFOUND)
    FIND_LIBRARY(Poco_LIB_Foundation NAMES PocoFoundation
      PATHS D:/dev/apps/MinGW/msys/1.0/local/lib #${Poco_DIR}
      DOC "The ${Poco_LIBRARY_PATH_DESCRIPTION}"
    )

    set(Poco_LIBRARY_DIR "" CACHE PATH Poco_LIBARARY_PATH_DESCRIPTION)
    
    get_filename_component(Poco_LIBRARY_DIR ${Poco_LIB_Foundation} PATH)
    
    # Look for the poco binary path.
    if(Poco_LIBRARY_DIR AND Poco_LIB_Foundation)
      set(Poco_BINARY_DIR ${Poco_DIR})
      if(Poco_BINARY_DIR AND EXISTS "${Poco_BINARY_DIR}/bin")
        set(Poco_BINARY_DIR ${Poco_BINARY_DIR}/bin)
      endif()
    endif()    
    
    message("Poco_DIR=${Poco_DIR}")
    message("Poco_LIBBBBBBBBR=PocoFoundation${Poco_LIB_SUFFIX}")
    message("Poco_LIBRARY_DIR=${Poco_LIBRARY_DIR}")
    message("Poco_BINARY_DIR=${Poco_BINARY_DIR}")
    message("Poco_POSSIBLE_ROOT_DIRS=${Poco_POSSIBLE_ROOT_DIRS}")
    message("Poco_LIBRARY_DIR_SUFFIXES=${Poco_LIBRARY_DIR_SUFFIXES}")
    message("Poco_LIB_Foundation=${Poco_LIB_Foundation}")    
    
    # Look poco libraries.
    set(Poco_LIBRARIES)
    if(Poco_LIB_Foundation)
      #set(Poco_COMPONENTS "Foundation${DBG}")
      foreach(component ${Poco_FIND_COMPONENTS})
      
        #if(CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)
        #  set(OPENSSL_LIBRARIES
        #    optimized ${SSL_EAY_RELEASE} debug ${SSL_EAY_DEBUG}
        #    optimized ${LIB_EAY_RELEASE} debug ${LIB_EAY_DEBUG}
        #    )
        #    
        #else()
        #  set( OPENSSL_LIBRARIES ${SSL_EAY_RELEASE} ${LIB_EAY_RELEASE} )
        #endif()

        # Create a Debug and a Release list for windows        
        if (MSVC)
          find_library(Poco_RLS_${component} NAMES "Poco${component}${Poco_LIB_SUFFIX}" PATHS Poco_LIBRARY_DIR)
          find_library(Poco_DBG_${component} NAMES "Poco${component}${Poco_LIB_SUFFIX}d" PATHS Poco_LIBRARY_DIR)
          if(CMAKE_CONFIGURATION_TYPES OR CMAKE_BUILD_TYPE)            
            if (Poco_RLS_${component}) 
              list(APPEND Poco_LIBRARIES "optimized" ${Poco_RLS_${component}})
            endif()
            if (Poco_DBG_${component})
              list(APPEND Poco_LIBRARIES "debug" ${Poco_DBG_${component}})
            endif()
          else()    
            if (Poco_RLS_${component}) 
              list(APPEND Poco_LIBRARIES ${Poco_RLS_${component}})
            endif()
          endif()
        else()
          message("component=${Poco_LIB_${component}}")
          unset(Poco_LIB_${component} CACHE)
          message("component=${Poco_LIB_${component}}")
          SET(Poco_LIB_${component} Poco_LIB_${component}-NOTFOUND) # FORCE
          find_library(Poco_LIB_${component} 
            NAMES 
              libPoco${component}${Poco_LIB_SUFFIX}.a 
              Poco${component}${Poco_LIB_SUFFIX}.lib # Adding this to top as DLL is found before lib!
            #Poco${component}${Poco_LIB_SUFFIX} 
            
            PATHS D:/dev/apps/MinGW/msys/1.0/local/lib #${Poco_DIR}
            
            Poco_LIBRARY_DIR
            )
          if (Poco_LIB_${component})
            list(APPEND Poco_LIBRARIES ${Poco_LIB_${component}}) #"Poco${component}${Poco_LIB_SUFFIX}"
          endif()
        endif()
      endforeach()
      
      #list(REMOVE_DUPLICATES Poco_LIBRARIES)
      
    endif(Poco_LIB_Foundation)
  endif(NOT Poco_LIBRARY_DIR)
endif(Poco_DIR)


#message(STATUS "Poco_LIBRARIES=${Poco_LIBRARIES}")
#message(STATUS "Poco_LIBRARY_DIRS=${Poco_LIBRARY_DIRS}")
#message(STATUS "Poco_POSSIBLE_ROOT_DIRS=${Poco_POSSIBLE_ROOT_DIRS}")
#message(STATUS "Poco_DIR=${Poco_DIR}")
