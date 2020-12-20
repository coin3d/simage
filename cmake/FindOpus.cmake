# Find Opus includes and libraries
#
#  OPUS_INCLUDE_DIRS - where to find opus/opus.h
#  OPUS_LIBRARIES    - List of libraries when using Opus
#  OPUS_FOUND        - True if Opus includes and libraries found

find_path(OPUS_INCLUDE_DIR opus/opus.h HINTS ${OPUS_ROOT})
find_library(OPUS_LIBRARY NAMES ${OPUS_NAMES} opus HINTS ${OPUS_ROOT})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  Opus
  DEFAULT_MSG
  OPUS_LIBRARY
  OPUS_INCLUDE_DIR
)

if(OPUS_FOUND)
  set(OPUS_INCLUDE_DIRS ${OPUS_INCLUDE_DIR})
  set(OPUS_LIBRARIES ${OPUS_LIBRARY})

  if(NOT TARGET Opus::Opus)
    add_library(Opus::Opus UNKNOWN IMPORTED)
    set_target_properties(Opus::Opus PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${OPUS_INCLUDE_DIR}")
    set_property(TARGET Opus::Opus APPEND PROPERTY IMPORTED_LOCATION "${OPUS_LIBRARY}")
  endif()
endif()

mark_as_advanced(OPUS_INCLUDE_DIR OPUS_LIBRARY)
