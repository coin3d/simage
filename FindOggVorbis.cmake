include(FindPackageHandleStandardArgs)

find_path(OGG_INCLUDE_DIR ogg/ogg.h)
find_path(VORBIS_INCLUDE_DIR vorbis/vorbisfile.h)

mark_as_advanced(
	OGG_INCLUDE_DIR
	VORBIS_INCLUDE_DIR
)

set(
	OGGVORBIS_INCLUDE_DIRS
	${OGG_INCLUDE_DIR}
	${VORBIS_INCLUDE_DIR}
)

find_library(OGG_LIBRARY NAMES ogg)
find_library(VORBIS_LIBRARY NAMES vorbis)
find_library(VORBISFILE_LIBRARY NAMES vorbisfile)
find_library(VORBISENC_LIBRARY NAMES vorbisenc)

mark_as_advanced(
	OGG_LIBRARY
	VORBIS_LIBRARY
	VORBISFILE_LIBRARY
	VORBISENC_LIBRARY
)

set(
	OGGVORBIS_LIBRARIES
	${OGG_LIBRARY}
	${VORBIS_LIBRARY}
	${VORBISFILE_LIBRARY}
	${VORBISENC_LIBRARY}
)

find_package_handle_standard_args(
	OggVorbis
	DEFAULT_MSG
	OGGVORBIS_INCLUDE_DIRS
	OGGVORBIS_LIBRARIES
)
