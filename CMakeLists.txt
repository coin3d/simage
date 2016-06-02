cmake_minimum_required(VERSION 2.8.11)

project(simage)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

include(CheckCXXSourceCompiles)
include(CheckFunctionExists)
include(CheckIncludeFiles)
include(CheckLibraryExists)
include(CheckStructHasMember)
include(CheckSymbolExists)
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

option(SIMAGE_BUILD_SHARED_LIBS "Build shared libraries" ON)
option(USE_QT5 "Prefer Qt5 over Qt4 if available" ON)

if(USE_QT5)
	find_package(Qt5 COMPONENTS Core QUIET)
endif()

if(Qt5_FOUND)
	set(QT_LIBRARIES ${Qt5Core_LIBRARIES})
else()
	set(QT_USE_IMPORTED_TARGETS ON)
	find_package(Qt4 COMPONENTS QtCore)
	include(${QT_USE_FILE})
endif()

find_package(GIF)
find_package(Jasper)
find_package(JPEG)
find_package(PNG)
find_package(QuickTime)
find_package(TIFF)

find_package(GdiPlus)
find_package(OggVorbis)
find_package(Sndfile)
find_package(Vfw)

check_cxx_source_compiles("
	#include <CoreFoundation/CoreFoundation.h>
	#include <ApplicationServices/ApplicationServices.h>
	int main() {
		CFStringRef cfname = CFStringCreateWithCString(kCFAllocatorDefault, \"filename.png\", kCFStringEncodingUTF8);
		CFURLRef texture_url = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, cfname, kCFURLPOSIXPathStyle, false);
		CGImageSourceRef image_source = CGImageSourceCreateWithURL(texture_url, NULL);
		return 0;
	}
" CGIMAGE_FOUND)

if(VFW_FOUND)
	option(SIMAGE_AVIENC_SUPPORT "Enable support for avi encoding (Win32 only)" ON)
else()
	set(SIMAGE_AVIENC_SUPPORT OFF)
endif()

if(CGIMAGE_FOUND)
	option(SIMAGE_CGIMAGE_SUPPORT "Use CGImage on OS X to load/save images" ON)
else()
	set(SIMAGE_CGIMAGE_SUPPORT OFF)
endif()

option(SIMAGE_EPS_SUPPORT "Enable support for encapsulated postscript export" ON)

if(GDIPLUS_FOUND)
	option(SIMAGE_GDIPLUS_SUPPORT "Use GDI+ on Windows to load/save images" ON)
else()
	set(SIMAGE_GDIPLUS_SUPPORT OFF)
endif()

if(SNDFILE_FOUND)
	option(SIMAGE_LIBSNDFILE_SUPPORT "" ON)
else()
	set(SIMAGE_LIBSNDFILE_SUPPORT OFF)
endif()

option(SIMAGE_MPEG2ENC_SUPPORT "Enable support for mpeg2 encoding" ON)

if(OGGVORBIS_FOUND)
	option(SIMAGE_OGGVORBIS_SUPPORT "" ON)
else()
	set(SIMAGE_OGGVORBIS_SUPPORT OFF)
endif()

option(SIMAGE_PIC_SUPPORT "Enable support for PIC images" ON)

if(QTFOUND_FOUND)
	option(SIMAGE_QIMAGE_SUPPORT "Use Qt's QImage to load/save images" ON)
else()
	set(SIMAGE_QIMAGE_SUPPORT OFF)
endif()

if(QUICKTIME_FOUND)
	option(SIMAGE_QUICKTIME_SUPPORT "Use Quicktime load/save images" ON)
else()
	set(SIMAGE_QUICKTIME_SUPPORT OFF)
endif()

option(SIMAGE_RGB_SUPPORT "Enable support for RGB images" ON)
option(SIMAGE_TGA_SUPPORT "Enable support for Targa images" ON)
option(SIMAGE_XWD_SUPPORT "Enable support for XWD images" ON)

set(SIMAGE_MAJOR_VERSION "1")
set(SIMAGE_MINOR_VERSION "7")
set(SIMAGE_MICRO_VERSION "1")
set(SIMAGE_BETA_VERSION "a")
set(SIMAGE_VERSION "${SIMAGE_MAJOR_VERSION}.${SIMAGE_MINOR_VERSION}.${SIMAGE_MICRO_VERSION}${SIMAGE_BETA_VERSION}")
set(VERSION "${SIMAGE_VERSION}")

if(GDIPLUS_FOUND)
	set(CMAKE_REQUIRED_INCLUDES ${GDIPLUS_INCLUDE_DIRS})
	set(CMAKE_REQUIRED_LIBRARIES ${GDIPLUS_LIBRARIES})
endif()

check_include_files(dlfcn.h HAVE_DLFCN_H)
check_cxx_source_compiles("
	#include <windows.h>
	#if defined(_MSC_VER) && (_MSC_VER == 1200) && !defined(ULONG_PTR)
	#define ULONG_PTR ULONG
	#endif
	#include <gdiplus.h>
	int main() { return 0; }
" HAVE_GDIPLUS_H)
check_cxx_source_compiles("
	#include <windows.h>
	#if defined(_MSC_VER) && (_MSC_VER == 1200) && !defined(ULONG_PTR)
	#define ULONG_PTR ULONG
	#endif
	#include <gdiplus.h>
	int main() {
		Gdiplus::Bitmap * bitmap = NULL;
		Gdiplus::BitmapData bitmapData;
		Gdiplus::Rect rect(0, 0, 100, 100);
		Gdiplus::Status result = bitmap->LockBits(&rect, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &bitmapData);
		return 0;
	}
" HAVE_GDIPLUS_LOCKBITS_RECTARG_POINTER)
set(HAVE_GIFLIB ${GIF_FOUND})
check_include_files(guile/gh.h HAVE_GUILE_GH_H)
check_include_files(inttypes.h HAVE_INTTYPES_H)
set(HAVE_JASPER ${JASPER_FOUND})
set(HAVE_JPEGLIB ${JPG_FOUND})
check_include_files(libguile.h HAVE_LIBGUILE_H)
check_include_files(memory.h HAVE_MEMORY_H)
set(HAVE_PNGLIB ${PNG_FOUND})
check_include_files(stdint.h HAVE_STDINT_H)
check_include_files(stdlib.h HAVE_STDLIB_H)
check_include_files(strings.h HAVE_STRINGS_H)
check_include_files(string.h HAVE_STRING_H)
check_include_files(sys/stat.h HAVE_SYS_STAT_H)
check_include_files(sys/types.h HAVE_SYS_TYPES_H)
set(HAVE_TIFFLIB ${TIFF_FOUND})
check_include_files(unistd.h HAVE_UNISTD_H)
set(HAVE_VFW ${VFW_FOUND})
check_include_files(windows.h HAVE_WINDOWS_H)
set(PACKAGE "simage")
set(PACKAGE_BUGREPORT "coin-support@coin3d.org")
set(PACKAGE_NAME "simage")
set(PACKAGE_STRING "simage ${SIMAGE_VERSION}")
set(PACKAGE_TARNAME "simage")
set(PACKAGE_URL "https://bitbucket.org/Coin3D/simage")
set(PACKAGE_VERSION "${SIMAGE_VERSION}")
check_include_files("assert.h;ctype.h;errno.h;float.h;limits.h;locale.h;math.h;setjmp.h;signal.h;stdarg.h;stddef.h;stdio.h;stdlib.h;string.h;time.h" STDC_HEADERS)
set(X_DISPLAY_MISSING ON)

configure_file(config.h.cmake.in config.h)

set(
	HDRS
	include/avi_encode.h
	include/simage.h
	include/simage_eps.h
	include/simage_jasper.h
	include/simage_libsndfile.h
	include/simage_oggvorbis.h
	include/simage_pic.h
	include/simage_private.h
	include/simage_rgb.h
	include/simage_xwd.h
)

set(
	SRCS
	src/avi_encode.c
	src/movie.c
	src/params.c
	src/resize.c
	src/simage.c
	src/simage_avi.c
	src/simage_eps.c
	src/simage_jasper.c
	src/simage_libsndfile.c
	src/simage_oggvorbis_reader.c
	src/simage_pic.c
	src/simage_rgb.c
	src/simage_write.c
	src/simage_xwd.c
	src/simage12.c
	src/simage13.c
	src/stream.c
)

if(SIMAGE_CGIMAGE_SUPPORT)
	list(APPEND HDRS include/simage_cgimage.h)
	list(APPEND SRCS src/simage_cgimage.c)
endif()

if(SIMAGE_GDIPLUS_SUPPORT)
	list(APPEND HDRS include/simage_gdiplus.h)
	list(APPEND SRCS src/simage_gdiplus.cpp)
endif()

if(SIMAGE_MPEG2ENC_SUPPORT)
	list(
		APPEND
		HDRS
		mpeg2enc/api.h
		mpeg2enc/global.h
		mpeg2enc/mpeg2enc.h
		mpeg2enc/putseq.h
		mpeg2enc/vlc.h
	)
	list(
		APPEND
		SRCS
		mpeg2enc/api.c
		mpeg2enc/conform.c
		mpeg2enc/fdctref.c
		mpeg2enc/idct.c
		mpeg2enc/motion.c
		mpeg2enc/predict.c
		mpeg2enc/putbits.c
		mpeg2enc/puthdr.c
		mpeg2enc/putmpg.c
		mpeg2enc/putpic.c
		mpeg2enc/putseq.c
		mpeg2enc/putvlc.c
		mpeg2enc/quantize.c
		mpeg2enc/ratectl.c
		mpeg2enc/readpic.c
		mpeg2enc/stats.c
		mpeg2enc/transfrm.c
		mpeg2enc/writepic.c
	)
endif()

if(SIMAGE_QIMAGE_SUPPORT)
	list(APPEND HDRS include/simage_qimage.h)
	list(APPEND SRCS src/simage_qimage.cpp)
endif()

if(SIMAGE_QUICKTIME_SUPPORT)
	list(APPEND HDRS include/simage_quicktime.h)
	list(APPEND SRCS src/simage_quicktime.c)
else()
	list(
		APPEND
		HDRS
		include/simage_gif.h
		include/simage_jpeg.h
		include/simage_png.h
		include/simage_tga.h
		include/simage_tiff.h
	)
	list(
		APPEND
		SRCS
		src/simage_gif.c
		src/simage_jpeg.c
		src/simage_png.c
		src/simage_tga.c
		src/simage_tiff.c
	)
endif()

if(SIMAGE_BUILD_SHARED_LIBS)
	add_library(simage SHARED ${HDRS} ${COMMON_HDRS} ${SRCS} ${COMMON_SRCS} ${MOC_SRCS})
	target_compile_definitions(simage PRIVATE -DSIMAGE_MAKE_DLL INTERFACE -DSIMAGE_DLL)
else()
	add_library(simage STATIC ${HDRS} ${COMMON_HDRS} ${SRCS} ${COMMON_SRCS} ${MOC_SRCS})
	target_compile_definitions(simage INTERFACE -DSIMAGE_NOT_DLL)
endif()

set_target_properties(simage PROPERTIES DEBUG_POSTFIX d VERSION ${VERSION})

target_compile_definitions(simage PRIVATE -DHAVE_CONFIG_H -DSIMAGE_INTERNAL)

if(CMAKE_BUILD_TYPE MATCHES "[Dd][Ee][Bb][Uu][Gg]")
	target_compile_definitions(simage PRIVATE -DSIMAGE_DEBUG=1)
else()
	target_compile_definitions(simage PRIVATE -DSIMAGE_DEBUG=0)
endif()

target_include_directories(
	simage
	PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>
)

if(GDIPLUS_FOUND)
	target_link_libraries(simage gdiplus)
endif()

if(GIF_FOUND)
	target_include_directories(simage PUBLIC ${GIF_INCLUDE_DIR})
	target_link_libraries(simage ${GIF_LIBRARIES})
endif()

if(JASPER_FOUND)
	target_include_directories(simage PUBLIC ${JASPER_INCLUDE_DIR})
	target_link_libraries(simage ${JASPER_LIBRARIES})
endif()

if(JPEG_FOUND)
	target_include_directories(simage PUBLIC ${JPEG_INCLUDE_DIR})
	target_link_libraries(simage ${JPEG_LIBRARIES})
endif()

if(OGGVORBIS_FOUND)
	target_include_directories(simage PUBLIC ${OGGVORBIS_INCLUDE_DIRS})
	target_link_libraries(simage ${OGGVORBIS_LIBRARIES})
endif()

if(PNG_FOUND)
	target_include_directories(simage PUBLIC ${PNG_INCLUDE_DIR})
	target_link_libraries(simage ${PNG_LIBRARIES})
endif()

if(QT_FOUND)
	target_link_libraries(simage ${QT_LIBRARIES})
endif()

if(QUICKTIME_FOUND)
	target_include_directories(simage PUBLIC ${QUICKTIME_INCLUDE_DIR})
	target_link_libraries(simage ${QUICKTIME_LIBRARIES})
endif()

if(SNDFILE_FOUND)
	target_include_directories(simage PUBLIC ${SNDFILE_INCLUDE_DIRS})
	target_link_libraries(simage ${SNDFILE_LIBRARIES})
endif()

if(TIFF_FOUND)
	target_include_directories(simage PUBLIC ${TIFF_INCLUDE_DIR})
	target_link_libraries(simage ${TIFF_LIBRARY})
endif()

if(VFW_FOUND)
	target_link_libraries(simage vfw32)
endif()

install(FILES include/simage.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT development)

install(
	TARGETS	simage
	EXPORT simage
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT development
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT runtime NAMELINK_SKIP
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT runtime
)

if(SIMAGE_BUILD_SHARED_LIBS)
	install(
		TARGETS	simage
		EXPORT simage
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT development NAMELINK_ONLY
	)
endif()

if(MSVC AND SIMAGE_BUILD_SHARED_LIBS AND ${CMAKE_MAJOR_VERSION} GREATER 2 AND ${CMAKE_MINOR_VERSION} GREATER 0)
	install(FILES $<TARGET_PDB_FILE:simage> DESTINATION ${CMAKE_INSTALL_BINDIR} CONFIGURATIONS Debug RelWithDebInfo COMPONENT debug)
endif()

configure_package_config_file(
  simage-config.cmake.in simage-config.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/simage-${SIMAGE_VERSION}
)

install(
	FILES ${CMAKE_CURRENT_BINARY_DIR}/simage-config.cmake
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/simage-${SIMAGE_VERSION}
	COMPONENT development
)

write_basic_package_version_file(
	${CMAKE_CURRENT_BINARY_DIR}/simage-config-version.cmake
	VERSION ${VERSION}
	COMPATIBILITY ExactVersion
)

install(
	FILES ${CMAKE_CURRENT_BINARY_DIR}/simage-config-version.cmake
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/simage-${SIMAGE_VERSION}
	COMPONENT development
)

install(
	EXPORT simage
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/simage-${SIMAGE_VERSION}
	NAMESPACE simage::
	FILE simage-export.cmake
	COMPONENT development
)

enable_testing()

add_executable(loaders tests/loaders.c)

if(SIMAGE_BUILD_SHARED_LIBS)
	target_compile_definitions(loaders PRIVATE -DSIMAGE_DLL)
else()
	target_compile_definitions(loaders PRIVATE -DSIMAGE_NOT_DLL)
endif()

target_include_directories(loaders PRIVATE ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(loaders simage)

if(UNIX)
	target_link_libraries(loaders m)
endif()

add_test(
	loaders
	${CMAKE_CURRENT_BINARY_DIR}/loaders
	${CMAKE_CURRENT_SOURCE_DIR}/tests/img.gif
	${CMAKE_CURRENT_SOURCE_DIR}/tests/img.jpg
	${CMAKE_CURRENT_SOURCE_DIR}/tests/img.png
	${CMAKE_CURRENT_SOURCE_DIR}/tests/img.rgb
	${CMAKE_CURRENT_SOURCE_DIR}/tests/img.tga
	${CMAKE_CURRENT_SOURCE_DIR}/tests/img.tif
)

set(CPACK_COMPONENT_DEVELOPMENT_DEPENDS runtime)
set(CPACK_DEB_COMPONENT_INSTALL ON)
set(CPACK_DEBIAN_DEVELOPMENT_PACKAGE_DEPENDS "lib${PACKAGE_TARNAME}")
set(CPACK_DEBIAN_DEVELOPMENT_PACKAGE_NAME "lib${PACKAGE_TARNAME}-dev")
set(CPACK_DEBIAN_DEVELOPMENT_PACKAGE_SECTION "libdevel")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE ${PACKAGE_URL})
set(CPACK_DEBIAN_PACKAGE_NAME "lib${PACKAGE_TARNAME}")
set(CPACK_DEBIAN_PACKAGE_SECTION "devel")
set(CPACK_DEBIAN_RUNTIME_PACKAGE_NAME "lib${PACKAGE_TARNAME}")
set(CPACK_DEBIAN_RUNTIME_PACKAGE_SECTION "libs")
set(CPACK_NSIS_PACKAGE_NAME ${PACKAGE_STRING})
set(CPACK_NSIS_URL_INFO_ABOUT ${PACKAGE_URL})
set(CPACK_PACKAGE_CONTACT ${PACKAGE_BUGREPORT})
set(CPACK_PACKAGE_DISPLAY_NAME ${PACKAGE_STRING})
set(CPACK_PACKAGE_INSTALL_DIRECTORY "${PACKAGE_TARNAME}-${PACKAGE_VERSION}")
set(CPACK_PACKAGE_NAME ${PACKAGE_TARNAME})
set(CPACK_PACKAGE_VERSION ${VERSION})
set(CPACK_PACKAGE_VERSION_MAJOR ${SOQT_MAJOR_VERSION})
set(CPACK_PACKAGE_VERSION_MINOR ${SOQT_MINOR_VERSION})
set(CPACK_PACKAGE_VERSION_PATCH ${SOQT_MICRO_VERSION})
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/COPYING)
set(CPACK_RPM_COMPONENT_INSTALL ON)
set(CPACK_RPM_development_PACKAGE_NAME "${PACKAGE_NAME}-devel")
set(CPACK_RPM_development_PACKAGE_REQUIRES "${PACKAGE_NAME}")
set(CPACK_RPM_PACKAGE_GROUP "Development/Libraries")
set(CPACK_RPM_PACKAGE_LICENSE "BSD")
set(CPACK_RPM_PACKAGE_NAME ${PACKAGE_TARNAME})
set(CPACK_RPM_PACKAGE_URL ${PACKAGE_URL})
set(CPACK_RPM_runtime_PACKAGE_NAME "${PACKAGE_NAME}")

if(WIN32 AND CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES64")
endif()

include(CPack)
