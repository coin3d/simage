# Find vorbis includes and libraries
#
#  VORBIS_INCLUDE_DIRS - where to find vorbis/vorbisfile.h
#  VORBIS_LIBRARIES    - List of libraries when using vorbis
#  VORBIS_FOUND        - True if vorbis includes and libraries found

find_path(VORBIS_INCLUDE_DIR vorbis/vorbisfile.h HINTS ${VORBIS_ROOT})
find_library(VORBIS_LIBRARY NAMES ${VORBIS_NAMES} vorbis HINTS ${VORBIS_ROOT})
find_library(VORBIS_VORBISFILE_LIBRARY NAMES ${VORBISFILE_NAMES} vorbisfile HINTS ${VORBIS_ROOT})
find_library(VORBIS_VORBISENC_LIBRARY NAMES ${VORBISENC_NAMES} vorbisenc HINTS ${VORBIS_ROOT})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  Vorbis
  DEFAULT_MSG
  VORBIS_LIBRARY
  VORBIS_INCLUDE_DIR
)

if(VORBIS_FOUND)
  set(VORBIS_INCLUDE_DIRS ${VORBIS_INCLUDE_DIR})
  set(VORBIS_LIBRARIES ${VORBIS_LIBRARY} ${VORBIS_VORBISFILE_LIBRARY} ${VORBIS_VORBISENC_LIBRARY})

  if(NOT TARGET Vorbis::Vorbis)
    add_library(Vorbis::Vorbis UNKNOWN IMPORTED)
    set_target_properties(Vorbis::Vorbis PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${VORBIS_INCLUDE_DIR}")
    set_property(TARGET Vorbis::Vorbis APPEND PROPERTY IMPORTED_LOCATION "${VORBIS_LIBRARY}")
  endif()

  if(NOT TARGET Vorbis::VorbisFile)
    add_library(Vorbis::VorbisFile UNKNOWN IMPORTED)
    set_target_properties(Vorbis::VorbisFile PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${VORBIS_INCLUDE_DIR}")
    set_property(TARGET Vorbis::VorbisFile APPEND PROPERTY IMPORTED_LOCATION "${VORBIS_VORBISFILE_LIBRARY}")
  endif()

  if(NOT TARGET Vorbis::VorbisEnc)
    add_library(Vorbis::VorbisEnc UNKNOWN IMPORTED)
    set_target_properties(Vorbis::VorbisEnc PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${VORBIS_INCLUDE_DIR}")
    set_property(TARGET Vorbis::VorbisEnc APPEND PROPERTY IMPORTED_LOCATION "${VORBIS_VORBISENC_LIBRARY}")
  endif()
endif()

mark_as_advanced(VORBIS_INCLUDE_DIR VORBIS_LIBRARY VORBIS_VORBISFILE_LIBRARY VORBIS_VORBISENC_LIBRARY)
