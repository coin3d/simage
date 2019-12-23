#.rst:
# FindSndfile
# --------
#
# Find the sndfile library (libsndfile)
#
# Imported targets
# ^^^^^^^^^^^^^^^^
#
# This module defines the following :prop_tgt:`IMPORTED` targets:
#
# ``sndfile::sndfile``
#   The sndfile library, if found.
#
# Result variables
# ^^^^^^^^^^^^^^^^
#
# This module will set the following variables in your project:
#
# ``SNDFILE_FOUND``
#   If false, do not try to use sndfile.
# ``SNDFILE_INCLUDE_DIRS``
#   where to find sndfile.h, etc.
# ``SNDFILE_LIBRARIES``
#   the libraries needed to use sndfile.
#
# Cache variables
# ^^^^^^^^^^^^^^^
#
# The following cache variables may also be set:
#
# ``SNDFILE_INCLUDE_DIRS``
#   where to find sndfile.h, etc.
# ``SNDFILE_LIBRARY_RELEASE``
#   where to find the sndfile library (optimized).
# ``SNDFILE_LIBRARY_DEBUG``
#   where to find the sndfile library (debug).

if(NOT SNDFILE_ROOT)
  set(SNDFILE_ROOT $ENV{SNDFILE_ROOT})
endif()
if(SNDFILE_ROOT)
  set(_SNDFILE_SEARCH_OPTS NO_DEFAULT_PATH)
else()
  set(_SNDFILE_SEARCH_OPTS)
endif()

find_path(SNDFILE_INCLUDE_DIR NAMES sndfile.h HINTS ${SNDFILE_ROOT} PATH_SUFFIXES include include/sndfile ${_SNDFILE_SEARCH_OPTS})

set(sndfile_names ${SNDFILE_NAMES} sndfile sndfile-1 libsndfile libsndfile-1)
foreach(_name ${sndfile_names})
  list(APPEND sndfile_names_debug "${_name}d")
endforeach()

if(NOT SNDFILE_LIBRARY)
  find_library(SNDFILE_LIBRARY_RELEASE NAMES ${sndfile_names} HINTS ${SNDFILE_ROOT} PATH_SUFFIXES lib ${_SNDFILE_SEARCH_OPTS})
  find_library(SNDFILE_LIBRARY_DEBUG NAMES ${sndfile_names_debug} HINTS ${SNDFILE_ROOT} PATH_SUFFIXES lib ${_SNDFILE_SEARCH_OPTS})
  include(SelectLibraryConfigurations)
  select_library_configurations(SNDFILE)
  mark_as_advanced(SNDFILE_LIBRARY_RELEASE SNDFILE_LIBRARY_DEBUG)
endif()
unset(sndfile_names)
unset(sndfile_names_debug)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  Sndfile
  DEFAULT_MSG
  SNDFILE_LIBRARY
  SNDFILE_INCLUDE_DIR
)

if(SNDFILE_FOUND)
  set(SNDFILE_LIBRARIES ${SNDFILE_LIBRARY})
  set(SNDFILE_INCLUDE_DIRS "${SNDFILE_INCLUDE_DIR}")

  if(NOT TARGET sndfile::sndfile)
    add_library(sndfile::sndfile UNKNOWN IMPORTED)
    if(SNDFILE_INCLUDE_DIRS)
      set_target_properties(sndfile::sndfile PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${SNDFILE_INCLUDE_DIRS}")
    endif()
    if(EXISTS "${SNDFILE_LIBRARY}")
      set_target_properties(sndfile::sndfile PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES "C"
        IMPORTED_LOCATION "${SNDFILE_LIBRARY}")
    endif()
    if(EXISTS "${SNDFILE_LIBRARY_RELEASE}")
      set_property(TARGET sndfile::sndfile APPEND PROPERTY
        IMPORTED_CONFIGURATIONS RELEASE)
      set_target_properties(sndfile::sndfile PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
        IMPORTED_LOCATION_RELEASE "${SNDFILE_LIBRARY_RELEASE}")
    endif()
    if(EXISTS "${SNDFILE_LIBRARY_DEBUG}")
      set_property(TARGET sndfile::sndfile APPEND PROPERTY
        IMPORTED_CONFIGURATIONS DEBUG)
      set_target_properties(sndfile::sndfile PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
        IMPORTED_LOCATION_DEBUG "${SNDFILE_LIBRARY_DEBUG}")
    endif()
  endif()
endif()

mark_as_advanced(SNDFILE_LIBRARY SNDFILE_INCLUDE_DIR)
