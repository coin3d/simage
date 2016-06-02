include(FindPackageHandleStandardArgs)

find_path(VFW_INCLUDE_DIRS NAMES vfw.h)

mark_as_advanced(VFW_INCLUDE_DIRS)

find_library(VFW_LIBRARIES NAMES vfw32)

find_package_handle_standard_args(
	Vfw
	DEFAULT_MSG
	VFW_INCLUDE_DIRS
	VFW_LIBRARIES
)
