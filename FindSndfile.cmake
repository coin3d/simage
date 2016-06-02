include(FindPackageHandleStandardArgs)

find_path(SNDFILE_INCLUDE_DIRS NAMES sndfile.h)

mark_as_advanced(SNDFILE_INCLUDE_DIRS)

find_library(SNDFILE_LIBRARIES NAMES sndfile)

find_package_handle_standard_args(
	Sndfile
	DEFAULT_MSG
	SNDFILE_INCLUDE_DIRS
	SNDFILE_LIBRARIES
)
