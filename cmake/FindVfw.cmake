# - Find Video For Windows (VFW)
# Find the VFW includes and libraries
#
#  VFW_INCLUDE_DIRS - where to find vfw.h header
#  VFW_LIBRARIES    - List of libraries when using VFW
#  VFW_FOUND        - True if VFW was found

if(WIN32)
  if(MINGW)
    # e.g. CMAKE_CXX_COMPILER: C:/msys64/mingw64/bin/g++.exe
    get_filename_component(MINGW_PATH ${CMAKE_CXX_COMPILER} PATH)
    # e.g. MINGW_PATH: C:/msys64/mingw64/bin
    get_filename_component(MINGW_PATH ${MINGW_PATH} PATH)
    # e.g. MINGW_PATH: C:/msys64/mingw64
    if("${MINGW_PATH}" MATCHES ".*mingw64$")
      set(MINGW_HINTS "${MINGW_PATH}/x86_64-w64-mingw32")
    elseif("${MINGW_PATH}" MATCHES ".*mingw32$")
      set(MINGW_HINTS "${MINGW_PATH}/i686-w64-mingw32")
    endif()
  endif()

  find_path(VFW_INCLUDE_DIR
    NAMES
      Vfw.h vfw.h
    HINTS
      "${MINGW_HINTS}"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Microsoft SDKs\\Windows\\v7.0;InstallationFolder]"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Microsoft SDKs\\Windows\\v7.0A;InstallationFolder]"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Microsoft SDKs\\Windows\\v7.1;InstallationFolder]"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Microsoft SDKs\\Windows\\v7.1A;InstallationFolder]"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Microsoft SDKs\\Windows\\v8.0;InstallationFolder]"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Microsoft SDKs\\Windows\\v8.0A;InstallationFolder]"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Microsoft SDKs\\Windows\\v8.1;InstallationFolder]"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Microsoft SDKs\\Windows\\v8.1A;InstallationFolder]"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows Kits\\Installed Roots;KitsRoot]"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows Kits\\Installed Roots;KitsRoot81]"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows Kits\\Installed Roots;KitsRoot10]"
    PATH_SUFFIXES
      include
      Include
      Include/um
      Include/shared
  )

  if(MINGW)
    find_library(VFW_LIBRARY
      NAMES
        libvfw32 vfw32
      HINTS
        "${MINGW_HINTS}"
      PATH_SUFFIXES
        lib
    )
  else()
    set(VFW_LIBRARY vfw32)
  endif()
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  Vfw
  DEFAULT_MSG
  VFW_LIBRARY
  VFW_INCLUDE_DIR
)

if(VFW_FOUND)
  set(VFW_LIBRARIES ${VFW_LIBRARY})
  set(VFW_INCLUDE_DIRS ${VFW_INCLUDE_DIR})
endif()

mark_as_advanced(VFW_INCLUDE_DIRS VFW_LIBRARIES)
