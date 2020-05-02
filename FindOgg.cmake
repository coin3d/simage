include(FindPackageHandleStandardArgs)

find_path(OGG_INCLUDE_DIR ogg/ogg.h)

mark_as_advanced(OGG_INCLUDE_DIR)

set(OGG_INCLUDE_DIRS ${OGG_INCLUDE_DIR})

find_library(OGG_LIBRARY NAMES ogg)

mark_as_advanced(OGG_LIBRARY)

set(OGG_LIBRARIES ${OGG_LIBRARY})

find_package_handle_standard_args(
	Ogg
	DEFAULT_MSG
	OGG_INCLUDE_DIRS
	OGG_LIBRARIES
)

if(OGG_FOUND AND NOT TARGET Ogg::Ogg)
	add_library(Ogg::Ogg UNKNOWN IMPORTED)
	set_target_properties(Ogg::Ogg PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${OGG_INCLUDE_DIR}")
	set_property(TARGET Ogg::Ogg APPEND PROPERTY IMPORTED_LOCATION "${OGG_LIBRARY}")
endif()
