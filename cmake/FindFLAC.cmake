# Find FLAC includes and libraries
#
#  FLAC_INCLUDE_DIRS - where to find FLAC/all.h
#  FLAC_LIBRARIES    - List of libraries when using FLAC
#  FLAC_FOUND        - True if FLAC includes and libraries found

find_path(FLAC_INCLUDE_DIR FLAC/all.h HINTS ${FLAC_ROOT})
find_library(FLAC_LIBRARY NAMES ${FLAC_NAMES} FLAC HINTS ${FLAC_ROOT})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  FLAC
  DEFAULT_MSG
  FLAC_LIBRARY
  FLAC_INCLUDE_DIR
)

if(FLAC_FOUND)
  set(FLAC_INCLUDE_DIRS ${FLAC_INCLUDE_DIR})
  set(FLAC_LIBRARIES ${FLAC_LIBRARY})

  if(NOT TARGET FLAC::FLAC)
    add_library(FLAC::FLAC UNKNOWN IMPORTED)
    set_target_properties(FLAC::FLAC PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${FLAC_INCLUDE_DIR}")
    set_property(TARGET FLAC::FLAC APPEND PROPERTY IMPORTED_LOCATION "${FLAC_LIBRARY}")
  endif()
endif()

mark_as_advanced(FLAC_INCLUDE_DIR FLAC_LIBRARY)
