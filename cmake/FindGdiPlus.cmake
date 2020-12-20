# Find the GDI+ includes and libraries
#
#  GDIPLUS_INCLUDE_DIRS - where to find gdiplus.h header
#  GDIPLUS_LIBRARIES    - List of libraries when using GDI+
#  GDIPLUS_FOUND        - True if GDI+ was found

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

  find_path(GDIPLUS_INCLUDE_DIR
    NAMES
      GdiPlus.h gdiplus.h
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
    find_library(GDIPLUS_LIBRARY
      NAMES
        libgdiplus gdiplus
      HINTS
        "${MINGW_HINTS}"
      PATH_SUFFIXES
        lib
    )
  else()
    set(GDIPLUS_LIBRARY gdiplus)
  endif()
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  GdiPlus
  DEFAULT_MSG
  GDIPLUS_LIBRARY
  GDIPLUS_INCLUDE_DIR
)

if(GDIPLUS_FOUND)
  set(GDIPLUS_LIBRARIES ${GDIPLUS_LIBRARY})
  set(GDIPLUS_INCLUDE_DIRS ${GDIPLUS_INCLUDE_DIR})
endif()

mark_as_advanced(GDIPLUS_INCLUDE_DIRS GDIPLUS_LIBRARIES)
