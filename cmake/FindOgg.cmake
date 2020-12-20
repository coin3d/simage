# Find ogg includes and libraries
#
#  OGG_INCLUDE_DIRS - where to find ogg.h
#  OGG_LIBRARIES    - List of libraries when using ogg
#  OGG_FOUND        - True if ogg includes and libraries found

find_path(OGG_INCLUDE_DIR ogg/ogg.h HINTS ${OGG_ROOT})
find_library(OGG_LIBRARY NAMES ${OGG_NAMES} ogg ogg_static libogg libogg_static HINTS ${OGG_ROOT})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  Ogg
  DEFAULT_MSG
  OGG_LIBRARY
  OGG_INCLUDE_DIR
)

if(OGG_FOUND)
  set(OGG_INCLUDE_DIRS ${OGG_INCLUDE_DIR})
  set(OGG_LIBRARIES ${OGG_LIBRARY})

  if(NOT TARGET Ogg::Ogg)
    add_library(Ogg::Ogg UNKNOWN IMPORTED)
    set_target_properties(Ogg::Ogg PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${OGG_INCLUDE_DIR}")
    set_property(TARGET Ogg::Ogg APPEND PROPERTY IMPORTED_LOCATION "${OGG_LIBRARY}")
  endif()
endif()

mark_as_advanced(OGG_INCLUDE_DIR OGG_LIBRARY)
