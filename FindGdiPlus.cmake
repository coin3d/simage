include(FindPackageHandleStandardArgs)

find_path(GDIPLUS_INCLUDE_DIRS NAMES gdiplus.h)

mark_as_advanced(GDIPLUS_INCLUDE_DIRS)

find_library(GDIPLUS_LIBRARIES NAMES gdiplus)

find_package_handle_standard_args(
	GdiPlus
	DEFAULT_MSG
	GDIPLUS_INCLUDE_DIRS
	GDIPLUS_LIBRARIES
)
