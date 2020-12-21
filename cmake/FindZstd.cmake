# Find Zstandard compression algorithm headers and library
#
#  ZSTD_INCLUDE_DIRS - where to find zstd.h
#  ZSTD_LIBRARIES    - List of libraries when using Zstd
#  ZSTD_FOUND        - True if Zstd headers and libraries found

find_path(ZSTD_INCLUDE_DIR zstd.h HINTS ${ZSTD_ROOT})
find_library(ZSTD_LIBRARY NAMES ${ZSTD_NAMES} zstd HINTS ${ZSTD_ROOT})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  Zstd
  DEFAULT_MSG
  ZSTD_LIBRARY
  ZSTD_INCLUDE_DIR
)

if(ZSTD_FOUND)
  set(ZSTD_INCLUDE_DIRS ${ZSTD_INCLUDE_DIR})
  set(ZSTD_LIBRARIES ${ZSTD_LIBRARY})

  if(NOT TARGET Zstd::Zstd)
    add_library(Zstd::Zstd UNKNOWN IMPORTED)
    set_target_properties(Zstd::Zstd PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${ZSTD_INCLUDE_DIR}")
    set_property(TARGET Zstd::Zstd APPEND PROPERTY IMPORTED_LOCATION "${ZSTD_LIBRARY}")
  endif()
endif()

mark_as_advanced(ZSTD_INCLUDE_DIR ZSTD_LIBRARY)
