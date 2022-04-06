cmake_minimum_required(VERSION 3.0)

set(SIMAGE_MAJOR_VERSION 1)
set(SIMAGE_MINOR_VERSION 8)
set(SIMAGE_MICRO_VERSION 1)
set(SIMAGE_BETA_VERSION)
set(SIMAGE_VERSION ${SIMAGE_MAJOR_VERSION}.${SIMAGE_MINOR_VERSION}.${SIMAGE_MICRO_VERSION}${SIMAGE_BETA_VERSION})

project(simage VERSION ${SIMAGE_MAJOR_VERSION}.${SIMAGE_MINOR_VERSION}.${SIMAGE_MICRO_VERSION})
string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWER)

# ############################################################################
# these will be removed after upgrading CMake minimum version
set(PROJECT_DESCRIPTION   "Image file format library abstraction layer")
# ############################################################################

string(TIMESTAMP SIMAGE_BUILD_YEAR "%Y")
math(EXPR SIMAGE_SO_VERSION ${PROJECT_VERSION_MAJOR}*20)
set(VERSION ${SIMAGE_VERSION})

if(POLICY CMP0054)
  # CMake version 3.14.5 warns when the policy is not set and uses OLD behaviour
  cmake_policy(SET CMP0054 NEW)
endif()

if(POLICY CMP0075)
  # get rid of CMAKE_REQUIRED_LIBRARIES warning from CMake 3.12
  cmake_policy(SET CMP0075 NEW)
endif()

# ############################################################################
# Prevent in-source builds, as they often cause severe build problems
# ############################################################################

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  message(FATAL_ERROR "${CMAKE_PROJECT_NAME} requires an out of source build. Please create a separate build directory and run 'cmake <path_to_${CMAKE_PROJECT_NAME}> [options]' from there.")
endif()

# ############################################################################
# Include necessary submodules
# ############################################################################

# Add path for Coin specific utility scripts
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

include(CheckCXXSourceCompiles)
include(CheckIncludeFiles)
include(CMakePackageConfigHelpers)
include(CMakeDependentOption)
include(GNUInstallDirs)
include(CoinCMakeUtilities)

# ############################################################################
# Provide options to customise the build
# ############################################################################

option(SIMAGE_BUILD_SHARED_LIBS "Build shared libraries" ON)
option(SIMAGE_BUILD_EXAMPLES "Build examples" ON)
option(SIMAGE_BUILD_TESTS "Build tests" ON)
option(SIMAGE_BUILD_DOCUMENTATION "Build and install API documentation (requires Doxygen)." OFF)
cmake_dependent_option(SIMAGE_USE_AVIENC "Use Video for Windows for AVI encoding" ON "WIN32" OFF)
cmake_dependent_option(SIMAGE_USE_GDIPLUS "Use GDI+ on Windows to load/save images" ON "WIN32" OFF)
cmake_dependent_option(SIMAGE_USE_CGIMAGE "Use CGImage on OS X to load/save images" ON "APPLE" OFF)
cmake_dependent_option(SIMAGE_USE_QUICKTIME "Use QuickTime on OS X to load/save images" OFF "APPLE" OFF)
option(SIMAGE_USE_QIMAGE "Use Qt's QImage to load/save images" OFF)
cmake_dependent_option(SIMAGE_USE_QT6 "Prefer Qt6 over Qt5 if available" OFF "SIMAGE_USE_QIMAGE" OFF)
cmake_dependent_option(SIMAGE_USE_QT5 "Prefer Qt5 over Qt4 if available" ON "SIMAGE_USE_QIMAGE" OFF)
option(SIMAGE_USE_CPACK "If enabled the cpack subrepo is mandatory" OFF)
option(SIMAGE_USE_STATIC_LIBS "Linking prefers static libs over CMake default" OFF)
cmake_dependent_option(SIMAGE_USE_MSVC_STATIC_RUNTIME "Build against static Microsoft Visual C runtime library." OFF "WIN32" OFF)


message(STATUS "SDK version : ${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}")

# ############################################################################
# Find all necessary and optional simage dependencies
# ############################################################################

# Fail early if one of the required packages cannot be found

if(SIMAGE_USE_QIMAGE)
  if(SIMAGE_USE_QT6)
    find_package(Qt6 COMPONENTS Core Gui QUIET)
  endif()

  if(NOT Qt6_FOUND AND SIMAGE_USE_QT5)
    find_package(Qt5 COMPONENTS Core Gui QUIET)
  endif()

  if(NOT Qt6_FOUND AND NOT Qt5_FOUND)
    set(QT_USE_IMPORTED_TARGETS ON)
    find_package(Qt4 COMPONENTS QtCore QtGui REQUIRED)
  endif()

  if(Qt6_FOUND)
    set(QT_FOUND TRUE)
    set(SIMAGE_QT_TARGETS Qt6::Core Qt6::Gui)
    list(APPEND PKG_CONFIG_REQUIRES_PRIVATE "Qt6Core" "Qt6Gui")
  elseif(Qt5_FOUND)
    set(QT_FOUND TRUE)
    set(SIMAGE_QT_TARGETS Qt5::Core Qt5::Gui)
    list(APPEND PKG_CONFIG_REQUIRES_PRIVATE "Qt5Core" "Qt5Gui")
  else()
    set(SIMAGE_QT_TARGETS Qt4::QtCore Qt4::QtGui)
    include(${QT_USE_FILE})
    list(APPEND PKG_CONFIG_REQUIRES_PRIVATE "QtCore" "QtGui")
  endif()
endif()

# Prefer linking of static libs over CMake default rules by adjusting CMAKE_FIND_LIBRARY_SUFFIXES
if(SIMAGE_USE_STATIC_LIBS)
  set(_simage_ORIG_CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_FIND_LIBRARY_SUFFIXES})
  if(WIN32)
    set(CMAKE_FIND_LIBRARY_SUFFIXES .lib .a ${CMAKE_FIND_LIBRARY_SUFFIXES})
  else()
    set(CMAKE_FIND_LIBRARY_SUFFIXES .a )
  endif()
endif()

# On Windows GDI+ supports GIF, JPEG, EXIF, PNG, and TIFF.
# see https://docs.microsoft.com/en-us/windows/desktop/gdiplus/-gdiplus-types-of-bitmaps-about
if(SIMAGE_USE_GDIPLUS)
  include(TestGdiPlus)
endif()
if(SIMAGE_USE_AVIENC)
  include(TestVideoForWindows)
endif()

# On macOS QuickTime supports BMP, GIF, JPEG, JPEG 2000, PNG, TIFF, and TGA.
# see https://en.wikipedia.org/wiki/QuickTime
if(SIMAGE_USE_QUICKTIME)
  find_package(QuickTime REQUIRED)
endif()

if(SIMAGE_USE_CGIMAGE)
  find_library(APPLICATION_SERVICES ApplicationServices)
  find_library(CORE_FOUNDATION CoreFoundation)
  set(CMAKE_REQUIRED_LIBRARIES ${APPLICATION_SERVICES} ${CORE_FOUNDATION})
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
  unset(CMAKE_REQUIRED_LIBRARIES)
endif()

cmake_dependent_option(SIMAGE_AVIENC_SUPPORT "Enable support for AVI encoding (Win32 only)" ON "SIMAGE_USE_AVIENC;VFW_FOUND" OFF)
cmake_dependent_option(SIMAGE_CGIMAGE_SUPPORT "Use CGImage on macOS to load/save images" ON "SIMAGE_USE_CGIMAGE;CGIMAGE_FOUND" OFF)
cmake_dependent_option(SIMAGE_GDIPLUS_SUPPORT "Use GDI+ on Windows to load/save images" ON "SIMAGE_USE_GDIPLUS;GDIPLUS_FOUND" OFF)
cmake_dependent_option(SIMAGE_QIMAGE_SUPPORT "Use Qt's QImage to load/save images" ON "SIMAGE_USE_QIMAGE;QT_FOUND" OFF)
cmake_dependent_option(SIMAGE_QUICKTIME_SUPPORT "Use QuickTime on macOS to load/save images" ON "SIMAGE_USE_QUICKTIME;QUICKTIME_FOUND" OFF)

option(SIMAGE_LIBJASPER_SUPPORT "Use libjasper to load/save JPEG 2000 images" OFF)
option(SIMAGE_LIBSNDFILE_SUPPORT "Use libsndfile to load/save sampled sound" ON)
option(SIMAGE_OGGVORBIS_SUPPORT "Enable support for ogg/vorbis extensions" ON)
option(SIMAGE_EPS_SUPPORT "Enable support for encapsulated postscript export" ON)
option(SIMAGE_MPEG2ENC_SUPPORT "Enable support for mpeg2 encoding" ON)
option(SIMAGE_PIC_SUPPORT "Enable support for PIC images" ON)
option(SIMAGE_RGB_SUPPORT "Enable support for RGB images" ON)
cmake_dependent_option(SIMAGE_TGA_SUPPORT "Enable support for Targa images" ON "NOT SIMAGE_QUICKTIME_SUPPORT;NOT SIMAGE_CGIMAGE_SUPPORT" OFF)
option(SIMAGE_XWD_SUPPORT "Enable support for XWD images (experimental)" OFF)
cmake_dependent_option(SIMAGE_ZLIB_SUPPORT "Enable support for zlib library" ON "NOT SIMAGE_QUICKTIME_SUPPORT;NOT SIMAGE_CGIMAGE_SUPPORT;NOT SIMAGE_GDIPLUS_SUPPORT" OFF)
cmake_dependent_option(SIMAGE_GIF_SUPPORT "Enable support for GIF images" ON "NOT SIMAGE_QUICKTIME_SUPPORT;NOT SIMAGE_CGIMAGE_SUPPORT;NOT SIMAGE_GDIPLUS_SUPPORT" OFF)
cmake_dependent_option(SIMAGE_JPEG_SUPPORT "Enable support for JPEG images" ON "NOT SIMAGE_QUICKTIME_SUPPORT;NOT SIMAGE_CGIMAGE_SUPPORT;NOT SIMAGE_GDIPLUS_SUPPORT" OFF)
cmake_dependent_option(SIMAGE_PNG_SUPPORT "Enable support for PNG images" ON "NOT SIMAGE_QUICKTIME_SUPPORT;NOT SIMAGE_CGIMAGE_SUPPORT;NOT SIMAGE_GDIPLUS_SUPPORT" OFF)
cmake_dependent_option(SIMAGE_TIFF_SUPPORT "Enable support for TIFF images" ON "NOT SIMAGE_QUICKTIME_SUPPORT;NOT SIMAGE_CGIMAGE_SUPPORT;NOT SIMAGE_GDIPLUS_SUPPORT" OFF)

report_prepare(
  SIMAGE_BUILD_SHARED_LIBS
  SIMAGE_BUILD_DOCUMENTATION
  SIMAGE_USE_QIMAGE
  SIMAGE_USE_QT5
  SIMAGE_LIBJASPER_SUPPORT
  SIMAGE_LIBSNDFILE_SUPPORT
  SIMAGE_OGGVORBIS_SUPPORT
  SIMAGE_QIMAGE_SUPPORT
  SIMAGE_EPS_SUPPORT
  SIMAGE_MPEG2ENC_SUPPORT
  SIMAGE_PIC_SUPPORT
  SIMAGE_RGB_SUPPORT
  SIMAGE_TGA_SUPPORT
  SIMAGE_XWD_SUPPORT
  SIMAGE_ZLIB_SUPPORT
  SIMAGE_GIF_SUPPORT
  SIMAGE_JPEG_SUPPORT
  SIMAGE_PNG_SUPPORT
  SIMAGE_TIFF_SUPPORT
  IF_WIN32
    SIMAGE_USE_AVIENC
    SIMAGE_USE_GDIPLUS
    SIMAGE_AVIENC_SUPPORT
    SIMAGE_GDIPLUS_SUPPORT
  IF_APPLE
    SIMAGE_USE_CGIMAGE
    SIMAGE_USE_QUICKTIME
    SIMAGE_CGIMAGE_SUPPORT
    SIMAGE_QUICKTIME_SUPPORT
)

# ##########################################################################
# Setup build environment
# ##########################################################################

if(NOT CMAKE_BUILD_TYPE)
  # Has no effect for multi configuration generators (VisualStudio, Xcode).
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose type of build, options are Debug, Release, RelWithDebInfo, MinSizeRel." FORCE)
endif()
# Set common output directories for all targets built.
# First for the generic no-config case (e.g. with mingw)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
# Second, for multi-config builds (e.g. msvc)
foreach (_config ${CMAKE_CONFIGURATION_TYPES})
  string(TOUPPER ${_config} _config)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${_config} "${CMAKE_BINARY_DIR}/lib")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${_config} "${CMAKE_BINARY_DIR}/lib")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${_config} "${CMAKE_BINARY_DIR}/bin")
endforeach()

if(SIMAGE_LIBJASPER_SUPPORT)
  find_package(Jasper REQUIRED)
  if(JASPER_FOUND AND SIMAGE_USE_STATIC_LIBS)
    add_definitions(-DJAS_DLL=0)
  endif()
endif()

if(SIMAGE_LIBSNDFILE_SUPPORT)
  find_package(Sndfile REQUIRED)
  if(SNDFILE_FOUND)
    find_package(FLAC REQUIRED)
  endif()
endif()

if(SIMAGE_OGGVORBIS_SUPPORT)
  find_package(Ogg REQUIRED)
  find_package(Vorbis REQUIRED)
  if(OGG_FOUND)
    find_package(Opus)
  endif()
endif()

if(NOT SIMAGE_QUICKTIME_SUPPORT AND NOT SIMAGE_CGIMAGE_SUPPORT AND NOT SIMAGE_GDIPLUS_SUPPORT)
  if(SIMAGE_ZLIB_SUPPORT)
    find_package(ZLIB REQUIRED)
  endif()
  if(SIMAGE_GIF_SUPPORT)
    find_package(GIF REQUIRED)
  endif()
  if(SIMAGE_JPEG_SUPPORT)
    find_package(JPEG REQUIRED)
  endif()
  if(SIMAGE_PNG_SUPPORT)
    find_package(PNG REQUIRED)
  endif()
  if(SIMAGE_TIFF_SUPPORT)
    find_package(TIFF REQUIRED)
    if(TIFF_FOUND)
      find_package(LibLZMA)
      find_package(Zstd)
    endif()
  endif()

  set(HAVE_ZLIB ${ZLIB_FOUND})
  set(HAVE_GIFLIB ${GIF_FOUND})
  set(HAVE_JPEGLIB ${JPEG_FOUND})
  set(HAVE_PNGLIB ${PNG_FOUND})
  set(HAVE_TIFFLIB ${TIFF_FOUND})
endif()

set(HAVE_JASPER ${JASPER_FOUND})
set(HAVE_VFW ${VFW_FOUND})

check_include_files(dlfcn.h HAVE_DLFCN_H)
check_include_files(guile/gh.h HAVE_GUILE_GH_H)
check_include_files(inttypes.h HAVE_INTTYPES_H)
check_include_files(libguile.h HAVE_LIBGUILE_H)
check_include_files(memory.h HAVE_MEMORY_H)
check_include_files(stdint.h HAVE_STDINT_H)
check_include_files(stdlib.h HAVE_STDLIB_H)
check_include_files(strings.h HAVE_STRINGS_H)
check_include_files(string.h HAVE_STRING_H)
check_include_files(sys/stat.h HAVE_SYS_STAT_H)
check_include_files(sys/types.h HAVE_SYS_TYPES_H)
check_include_files(unistd.h HAVE_UNISTD_H)
check_include_files(windows.h HAVE_WINDOWS_H)
check_include_files("assert.h;ctype.h;errno.h;float.h;limits.h;locale.h;math.h;setjmp.h;signal.h;stdarg.h;stddef.h;stdio.h;stdlib.h;string.h;time.h" STDC_HEADERS)

set(PACKAGE "simage")
set(PACKAGE_BUGREPORT "coin-support@coin3d.org")
set(PACKAGE_NAME "simage")
set(PACKAGE_STRING "simage ${SIMAGE_VERSION}")
set(PACKAGE_TARNAME "simage")
set(PACKAGE_URL "https://github.com/coin3d/simage")
set(PACKAGE_VERSION "${SIMAGE_VERSION}")
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
endif()

if(SIMAGE_TGA_SUPPORT)
  list(APPEND HDRS include/simage_tga.h)
  list(APPEND SRCS src/simage_tga.c)
endif()

if(NOT SIMAGE_QUICKTIME_SUPPORT AND NOT SIMAGE_CGIMAGE_SUPPORT AND NOT SIMAGE_GDIPLUS_SUPPORT)
  list(
    APPEND
    HDRS
    include/simage_gif.h
    include/simage_jpeg.h
    include/simage_png.h
    include/simage_tiff.h
  )
  list(
    APPEND
    SRCS
    src/simage_gif.c
    src/simage_jpeg.c
    src/simage_png.c
    src/simage_tiff.c
  )
  if(GIF_FOUND AND NOT GIF_VERSION VERSION_LESS 5.1.9)
    list(APPEND HDRS giflib/quantize.h)
    list(APPEND SRCS giflib/quantize.c)
  endif()
endif()

if(WIN32)
  if(MSVC)
    if (MSVC_VERSION GREATER 1500 OR MSVC_VERSION EQUAL 1500)
      option(SIMAGE_BUILD_MSVC_MP "Enable build with multiple processes in Visual Studio" ON)
    else()
      set(SIMAGE_BUILD_MSVC_MP OFF CACHE BOOL "Compiler option /MP requires at least Visual Studio 2008 (VS9) or newer" FORCE)
    endif()
    if(SIMAGE_BUILD_MSVC_MP)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
    endif ()
  endif()

  # Can also be set for MSYS and MinGW
  coin_msvc_link_static_crt(${SIMAGE_USE_MSVC_STATIC_RUNTIME})
endif()

if(WIN32)
  if(MINGW)
    set(SIMAGE_DEFAULT_SHARED_POSTFIX "")
    set(SIMAGE_DEFAULT_STATIC_POSTFIX "")
  else()
    # On Windows the major version number is part of the library name
    set(SIMAGE_DEFAULT_SHARED_POSTFIX ${PROJECT_VERSION_MAJOR})
    set(SIMAGE_DEFAULT_STATIC_POSTFIX ${PROJECT_VERSION_MAJOR}s)
  endif()
  if(SIMAGE_BUILD_SHARED_LIBS)
    set(SIMAGE_DEFAULT_POSTFIX ${SIMAGE_DEFAULT_SHARED_POSTFIX})
    set(PKG_CONFIG_CPPFLAGS "${PKG_CONFIG_CPPFLAGS} -DSIMAGE_DLL")
  else()
    set(SIMAGE_DEFAULT_POSTFIX ${SIMAGE_DEFAULT_STATIC_POSTFIX})
    set(PKG_CONFIG_CPPFLAGS "${PKG_CONFIG_CPPFLAGS} -DSIMAGE_NOT_DLL")
  endif()
  set(CMAKE_RELEASE_POSTFIX ${SIMAGE_DEFAULT_POSTFIX})
  set(CMAKE_MINSIZEREL_POSTFIX ${SIMAGE_DEFAULT_POSTFIX})
  set(CMAKE_RELWITHDEBINFO_POSTFIX ${SIMAGE_DEFAULT_POSTFIX})
  set(CMAKE_DEBUG_POSTFIX ${SIMAGE_DEFAULT_POSTFIX}d)
elseif(APPLE)
else()
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

if(SIMAGE_BUILD_SHARED_LIBS)
  add_library(${PROJECT_NAME} SHARED ${HDRS} ${COMMON_HDRS} ${SRCS} ${COMMON_SRCS} ${MOC_SRCS})
else()
  add_library(${PROJECT_NAME} STATIC ${HDRS} ${COMMON_HDRS} ${SRCS} ${COMMON_SRCS} ${MOC_SRCS})
endif()

if(WIN32 AND NOT MINGW)
  set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION ${SIMAGE_SO_VERSION})
else()
  set_target_properties(${PROJECT_NAME} PROPERTIES VERSION "${SIMAGE_SO_VERSION}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}" SOVERSION ${SIMAGE_SO_VERSION})
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE HAVE_CONFIG_H SIMAGE_INTERNAL SIMAGE_DEBUG=$<CONFIG:Debug>)

if(WIN32)
  if(MSVC)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_NONSTDC_NO_DEPRECATE _CRT_SECURE_NO_DEPRECATE _CRT_SECURE_NO_WARNINGS _USE_MATH_DEFINES)
  endif()
  if(SIMAGE_BUILD_SHARED_LIBS)
    if(MINGW)
      set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_NAME ${PROJECT_NAME}-${SIMAGE_SO_VERSION})
    endif()
    configure_file(${PROJECT_NAME}.rc.cmake.in ${PROJECT_NAME}.rc)
    target_sources(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.rc")
    target_compile_definitions(${PROJECT_NAME} PRIVATE SIMAGE_MAKE_DLL INTERFACE SIMAGE_DLL)
  else()
    target_compile_definitions(${PROJECT_NAME} INTERFACE SIMAGE_NOT_DLL)
    if(MSVC)
      set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Z7")                       # Override default /Zi to embed
      set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /Z7")     # debugging info in the .lib.
    endif()
  endif()
endif()

target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

if(SIMAGE_CGIMAGE_SUPPORT)
  target_link_libraries(${PROJECT_NAME} PRIVATE ${APPLICATION_SERVICES} ${CORE_FOUNDATION})
  list(APPEND LIB_DEPENDENCIES_PRIVATE ${APPLICATION_SERVICES} ${CORE_FOUNDATION})
endif()

if(SIMAGE_GDIPLUS_SUPPORT)
  target_link_libraries(${PROJECT_NAME} PRIVATE ${GDIPLUS_LIBRARIES})
  list(APPEND LIB_DEPENDENCIES_PRIVATE ${GDIPLUS_LIBRARIES})
endif()

if(SIMAGE_GIF_SUPPORT)
  target_include_directories(${PROJECT_NAME} PRIVATE ${GIF_INCLUDE_DIR} $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/giflib>)
  target_link_libraries(${PROJECT_NAME} PRIVATE ${GIF_LIBRARIES})
  #list(APPEND PKG_CONFIG_REQUIRES_PRIVATE "libgif") #does not provide .pc file
  list(APPEND INCLUDE_DEPENDENCIES ${GIF_INCLUDE_DIR})
  list(APPEND LIB_DEPENDENCIES_PRIVATE ${GIF_LIBRARIES})
endif()

if(SIMAGE_LIBJASPER_SUPPORT)
  list(APPEND PKG_CONFIG_REQUIRES_PRIVATE "jasper")
  target_include_directories(${PROJECT_NAME} PRIVATE ${JASPER_INCLUDE_DIR})
  target_link_libraries(${PROJECT_NAME} PRIVATE ${JASPER_LIBRARIES})
endif()

if(SIMAGE_JPEG_SUPPORT)
  list(APPEND PKG_CONFIG_REQUIRES_PRIVATE "libjpeg")
  target_include_directories(${PROJECT_NAME} PRIVATE ${JPEG_INCLUDE_DIR})
  target_link_libraries(${PROJECT_NAME} PRIVATE ${JPEG_LIBRARIES})
endif()

if(SIMAGE_PNG_SUPPORT)
  list(APPEND PKG_CONFIG_REQUIRES_PRIVATE "libpng")
  target_include_directories(${PROJECT_NAME} PRIVATE ${PNG_INCLUDE_DIR})
  target_link_libraries(${PROJECT_NAME} PRIVATE ${PNG_LIBRARIES})
  if(UNIX)
    target_link_libraries(${PROJECT_NAME} PRIVATE "m")
  endif()
endif()

if(SIMAGE_QUICKTIME_SUPPORT)
  list(APPEND PKG_CONFIG_REQUIRES_PRIVATE "QuickTime")
  target_include_directories(${PROJECT_NAME} PRIVATE ${QUICKTIME_INCLUDE_DIR})
  target_link_libraries(${PROJECT_NAME} PRIVATE ${QUICKTIME_LIBRARIES})
endif()

if(SIMAGE_LIBSNDFILE_SUPPORT)
  list(APPEND PKG_CONFIG_REQUIRES_PRIVATE "sndfile")
  target_include_directories(${PROJECT_NAME} PRIVATE ${SNDFILE_INCLUDE_DIRS})
  target_link_libraries(${PROJECT_NAME} PRIVATE ${SNDFILE_LIBRARIES})
  if(FLAC_FOUND)
    list(APPEND PKG_CONFIG_REQUIRES_PRIVATE "flac")
    target_include_directories(${PROJECT_NAME} PRIVATE ${FLAC_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${FLAC_LIBRARIES})
  endif()
endif()

if(SIMAGE_OGGVORBIS_SUPPORT)
  list(APPEND PKG_CONFIG_REQUIRES_PRIVATE "vorbis" "vorbisenc" "vorbisfile" "ogg")
  target_include_directories(${PROJECT_NAME} PRIVATE ${VORBIS_INCLUDE_DIRS} ${OGG_INCLUDE_DIRS})
  target_link_libraries(${PROJECT_NAME} PRIVATE ${VORBIS_LIBRARIES} ${OGG_LIBRARIES})
  if(OPUS_FOUND)
    list(APPEND PKG_CONFIG_REQUIRES_PRIVATE "opus")
    target_include_directories(${PROJECT_NAME} PRIVATE ${OPUS_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${OPUS_LIBRARIES})
  endif()
endif()

if(SIMAGE_TIFF_SUPPORT)
  list(APPEND PKG_CONFIG_REQUIRES_PRIVATE "libtiff-4")
  target_include_directories(${PROJECT_NAME} PRIVATE ${TIFF_INCLUDE_DIR})
  target_link_libraries(${PROJECT_NAME} PRIVATE ${TIFF_LIBRARY})
  if(LIBLZMA_FOUND)
    list(APPEND PKG_CONFIG_REQUIRES_PRIVATE "liblzma")
    target_include_directories(${PROJECT_NAME} PRIVATE ${LIBLZMA_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBLZMA_LIBRARIES})
  endif()
  if(ZSTD_FOUND)
    list(APPEND PKG_CONFIG_REQUIRES_PRIVATE "libzstd")
    target_include_directories(${PROJECT_NAME} PRIVATE ${ZSTD_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${ZSTD_LIBRARIES})
  endif()
endif()

if(SIMAGE_AVIENC_SUPPORT)
  target_link_libraries(${PROJECT_NAME} PRIVATE ${VFW_LIBRARIES})
  list(APPEND LIB_DEPENDENCIES_PRIVATE ${VFW_LIBRARIES})
endif()

if(SIMAGE_ZLIB_SUPPORT)
  list(APPEND PKG_CONFIG_REQUIRES_PRIVATE "zlib")
  target_include_directories(${PROJECT_NAME} PRIVATE ${ZLIB_INCLUDE_DIRS})
  target_link_libraries(${PROJECT_NAME} PRIVATE ${ZLIB_LIBRARIES})
endif()

target_include_directories(simage PRIVATE ${INCLUDE_DEPENDENCIES})
target_link_libraries(simage PRIVATE ${LIB_DEPENDENCIES_PRIVATE} ${SIMAGE_QT_TARGETS})

# Restore the original find library suffixes
if(SIMAGE_USE_STATIC_LIBS)
  set(CMAKE_FIND_LIBRARY_SUFFIXES ${_simage_ORIG_CMAKE_FIND_LIBRARY_SUFFIXES})
endif()

if(WIN32 AND MINGW AND SIMAGE_BUILD_SHARED_LIBS)
  # This has to be the last step for successful static linking the MinGW libraries.
  # Otherwise wrong resources are linked to libsimage-20.dll, the ones from libwinpthread.
  # The following linker error is thrown but it is expected as the resulting lib/dll is correct:
  # C:/msys64/mingw64/x86_64-w64-mingw32/bin/ld.exe: .rsrc merge failure: duplicate leaf: type: 10 (VERSION) name: 1 lang: 409
  # see: https://stackoverflow.com/questions/63013037/mingw-winpthreads-overrides-rc-resource-file-information for further details.
  # Be sure to handle this error in your scripts or in the CI.
  # add "-v" to see the full link command
  target_link_libraries(${PROJECT_NAME} PRIVATE -static-libgcc -static-libstdc++ -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive)
endif()

# ############################################################################
# Build examples
# ############################################################################

if(SIMAGE_BUILD_EXAMPLES)
  set(SIMAGE_EXAMPLE_SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/examples/audio2raw.c
    ${CMAKE_CURRENT_SOURCE_DIR}/examples/img2avi.c
#    ${CMAKE_CURRENT_SOURCE_DIR}/examples/mpeg2enc.cpp # requires Coin to build
    ${CMAKE_CURRENT_SOURCE_DIR}/examples/simage-convert.c
    ${CMAKE_CURRENT_SOURCE_DIR}/examples/simage-read-line-test.c
  )

  foreach(_source ${SIMAGE_EXAMPLE_SOURCE})
    get_filename_component(_example ${_source} NAME_WE)
    add_executable(${_example} ${_source})
    target_compile_definitions(${_example} PRIVATE _CRT_NONSTDC_NO_DEPRECATE _CRT_SECURE_NO_DEPRECATE _CRT_SECURE_NO_WARNINGS _USE_MATH_DEFINES)
    target_link_libraries(${_example} simage)
    if(UNIX)
      target_link_libraries(${_example} m)
    endif()
  endforeach()
endif()

# ############################################################################
# Build tests
# ############################################################################

if(SIMAGE_BUILD_TESTS)
  enable_testing()

  add_executable(loaders tests/loaders.c)
  target_link_libraries(loaders simage)
  target_compile_definitions(loaders PRIVATE _CRT_NONSTDC_NO_DEPRECATE _CRT_SECURE_NO_DEPRECATE _CRT_SECURE_NO_WARNINGS _USE_MATH_DEFINES)
  if(UNIX)
    target_link_libraries(loaders m)
  endif()

  add_test(
    loaders
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/loaders
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/img.gif
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/img.jpg
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/img.png
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/img.rgb
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/img.tga
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/img.tif
  )
endif()

# Add a target to generate API documentation with Doxygen
if(SIMAGE_BUILD_DOCUMENTATION)
  find_package(Doxygen)
  if(NOT DOXYGEN_FOUND)
    message(FATAL_ERROR "Doxygen is needed to build the documentation.")
  endif()

# ############################################################################
# Setup documentation options
# ############################################################################
  set(GENERATE_HTMLHELP NO)
  set(DOXYGEN_GENERATE_MAN NO)
  set(GENERATE_QHP NO)
  set(GENERATE_TREEVIEW YES)
  set(DOXYGEN_INTERNAL_DOCS NO)
  set(DOXYGEN_EXTRACT_PRIVATE NO)
  set(DOXYGEN_WARN_IF_UNDOCUMENTED YES)
  set(DOXYGEN_EXCLUDE)

  set(DOXYFILE "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")
  set(DOXYGEN_OUTPUT "${CMAKE_BINARY_DIR}/html/index.html")
  configure_file("${CMAKE_CURRENT_SOURCE_DIR}/docs/simage.doxygen.cmake.in" ${DOXYFILE} @ONLY)

# ############################################################################
# Setup documentation targets
# ############################################################################
  add_custom_command(
    OUTPUT ${DOXYGEN_OUTPUT}
    COMMAND ${CMAKE_COMMAND} -E echo_append "Generating API documentation with Doxygen "
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE}
    COMMAND ${CMAKE_COMMAND} -E echo "done."
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    DEPENDS ${DOXYFILE}
  )
  add_custom_target(documentation ALL DEPENDS ${DOXYGEN_OUTPUT})

# ############################################################################
# Install built documentation files
# ############################################################################
  install(DIRECTORY "${CMAKE_BINARY_DIR}/html" DESTINATION ${CMAKE_INSTALL_DOCDIR} COMPONENT documentation REGEX ".*\\.(chm|qch)" EXCLUDE)
endif()

# ############################################################################
# Install headers
# ############################################################################

install(FILES include/simage.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT development)

# ############################################################################
# Install targets
# ############################################################################

if(NOT CMAKE_VERSION VERSION_LESS 3.12)
  install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT development
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT runtime NAMELINK_COMPONENT development
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT runtime
  )
  if(SIMAGE_BUILD_SHARED_LIBS)
    if(MSVC)
      install(FILES $<TARGET_PDB_FILE:${PROJECT_NAME}> DESTINATION ${CMAKE_INSTALL_BINDIR} CONFIGURATIONS Debug RelWithDebInfo COMPONENT debug)
    endif()
  endif()
else()
  install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT development
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT runtime NAMELINK_SKIP
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT runtime
  )
  if(SIMAGE_BUILD_SHARED_LIBS)
    install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT development NAMELINK_ONLY)
    if(MSVC)
      install(FILES $<TARGET_PDB_FILE:${PROJECT_NAME}> DESTINATION ${CMAKE_INSTALL_BINDIR} CONFIGURATIONS Debug RelWithDebInfo COMPONENT debug)
    endif()
  endif()
endif()

# ############################################################################
# Install pkgconfig file and CMake config package
# ############################################################################

if(UNIX OR MINGW)
  if(DEFINED LIB_DEPENDENCIES)
    list(REMOVE_DUPLICATES LIB_DEPENDENCIES)
    coin_add_pkg_config_lib_dependencies(PKG_CONFIG_LIBS "${LIB_DEPENDENCIES}")
  endif()
  if(DEFINED LIB_DEPENDENCIES_PRIVATE)
    list(REMOVE_DUPLICATES LIB_DEPENDENCIES_PRIVATE)
    coin_add_pkg_config_lib_dependencies(PKG_CONFIG_LIBS_PRIVATE "${LIB_DEPENDENCIES_PRIVATE}")
  endif()

  string(REGEX REPLACE "-I${CMAKE_INSTALL_PREFIX}" "-I\${prefix}" _full_inc_dir "-I${CMAKE_INSTALL_FULL_INCLUDEDIR}")
  set(PKG_CONFIG_CPPFLAGS "${PKG_CONFIG_CPPFLAGS} ${_full_inc_dir}")
  string(REGEX REPLACE "-L${CMAKE_INSTALL_PREFIX}" "-L\${exec_prefix}" _full_lib_dir "-L${CMAKE_INSTALL_FULL_LIBDIR}")
  set(PKG_CONFIG_LDFLAGS "${PKG_CONFIG_LDFLAGS} ${_full_lib_dir} -l${PROJECT_NAME}")

  if(DEFINED INCLUDE_DEPENDENCIES)
    list(REMOVE_DUPLICATES INCLUDE_DEPENDENCIES)
  endif()
  foreach(_inc ${INCLUDE_DEPENDENCIES})
    set(PKG_CONFIG_CPPFLAGS "${PKG_CONFIG_CPPFLAGS} -I${_inc}")
  endforeach()

  set(PKG_CONFIG_REQUIRES "$<$<BOOL:${PKG_CONFIG_REQUIRES}>:$<JOIN:${PKG_CONFIG_REQUIRES}, >>")
  set(PKG_CONFIG_REQUIRES_PRIVATE "$<$<BOOL:${PKG_CONFIG_REQUIRES_PRIVATE}>:$<JOIN:${PKG_CONFIG_REQUIRES_PRIVATE}, >>")

  set(PKG_CONFIG_CFLAGS "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_RELEASE} ${PKG_CONFIG_CFLAGS}")
  set(PKG_CONFIG_CXXFLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} ${PKG_CONFIG_CXXFLAGS}")

  # PKG-CONFIG settings
  # according to https://www.gnu.org/prep/standards/html_node/Directory-Variables.html
  # .pc files should be written as ${prefix}/include instead of /usr/local/include
  set(prefix "${CMAKE_INSTALL_PREFIX}")
  set(exec_prefix "\${prefix}")
  string(REGEX REPLACE "^${CMAKE_INSTALL_PREFIX}" "\${exec_prefix}" bindir "${CMAKE_INSTALL_FULL_BINDIR}")
  string(REGEX REPLACE "^${CMAKE_INSTALL_PREFIX}" "\${exec_prefix}" libdir "${CMAKE_INSTALL_FULL_LIBDIR}")
  string(REGEX REPLACE "^${CMAKE_INSTALL_PREFIX}" "\${prefix}" includedir "${CMAKE_INSTALL_FULL_INCLUDEDIR}")
  string(REGEX REPLACE "^${CMAKE_INSTALL_PREFIX}" "\${prefix}" datarootdir "${CMAKE_INSTALL_FULL_DATAROOTDIR}")

  string(REGEX REPLACE "^${CMAKE_INSTALL_FULL_DATAROOTDIR}" "\${datarootdir}" datadir "${CMAKE_INSTALL_FULL_DATADIR}")
  string(REGEX REPLACE "^${CMAKE_INSTALL_FULL_DATAROOTDIR}" "\${datarootdir}" infodir "${CMAKE_INSTALL_FULL_INFODIR}")
  string(REGEX REPLACE "^${CMAKE_INSTALL_FULL_DATAROOTDIR}" "\${datarootdir}" mandir "${CMAKE_INSTALL_FULL_MANDIR}")
  string(REGEX REPLACE "^${CMAKE_INSTALL_FULL_DATAROOTDIR}" "\${datarootdir}" docdir "${CMAKE_INSTALL_FULL_DOCDIR}")

  # HTMLDIR is not defined in GNUInstallDirs.cmake as of CMake 3.18
  #string(REGEX REPLACE "^${CMAKE_INSTALL_FULL_DOCDIR}" "\${docdir}" htmldir "${CMAKE_INSTALL_FULL_HTMLDIR}")
  set(htmldir "\${docdir}/html")
  #set(frameworkdir "${PKG_CONFIG_FRAMEWORK_DIR}")

  # Fix variable for MinGW
  if(MINGW AND NOT CMAKE_CXX_LIBRARY_ARCHITECTURE)
    if("${CMAKE_CXX_COMPILER}" MATCHES ".*/mingw64/.*$")
      set(CMAKE_CXX_LIBRARY_ARCHITECTURE "x86_64-w64-mingw32")
    elseif("${CMAKE_CXX_COMPILER}" MATCHES ".*/mingw32/.*$")
      set(CMAKE_CXX_LIBRARY_ARCHITECTURE "i686-w64-mingw32")
    endif()
  endif()

  set(PKG_CONFIG_URL "https://www.github.com/coin3d/simage")
  set(PKG_CONFIG_HOST "${CMAKE_CXX_LIBRARY_ARCHITECTURE}")
  set(PKG_CONFIG_COMPILER "${CMAKE_C_COMPILER}")
  if ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
    set(PKG_CONFIG_COMPILER_IS_GCC "yes")
  else()
    set(PKG_CONFIG_COMPILER_IS_GCC "no")
  endif()
  string(REPLACE "." "" PKG_CONFIG_OBJEXT "${CMAKE_CXX_OUTPUT_EXTENSION}")

  configure_file("${CMAKE_SOURCE_DIR}/simage.pc.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/simage.pc.in" @ONLY)
  # Deal with items configured with generator expressions:
  file(GENERATE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/simage.pc" INPUT "${CMAKE_CURRENT_BINARY_DIR}/simage.pc.in")
  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/simage.pc" DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
  configure_file("${CMAKE_CURRENT_SOURCE_DIR}/simage-config" "${CMAKE_CURRENT_BINARY_DIR}/simage-config" COPYONLY)
  install(PROGRAMS "${CMAKE_CURRENT_BINARY_DIR}/simage-config" DESTINATION ${CMAKE_INSTALL_BINDIR})
  configure_file("${CMAKE_CURRENT_SOURCE_DIR}/simage.cfg.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/simage-default.cfg.in" @ONLY)
  # Deal with items configured with generator expressions:
  file(GENERATE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/simage-default.cfg" INPUT "${CMAKE_CURRENT_BINARY_DIR}/simage-default.cfg.in")
  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/simage-default.cfg" DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/Coin/conf)
endif()

configure_package_config_file(${PROJECT_NAME_LOWER}-config.cmake.in ${PROJECT_NAME_LOWER}-config.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}-${PROJECT_VERSION}
)

write_basic_package_version_file("${CMAKE_BINARY_DIR}/${PROJECT_NAME_LOWER}-config-version.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY ExactVersion
)

# To make the component usable not only from the install directory but also from the build directory
export(
  TARGETS ${PROJECT_NAME}
  FILE ${PROJECT_NAME_LOWER}-export.cmake
)

install(FILES "${CMAKE_BINARY_DIR}/${PROJECT_NAME_LOWER}-config.cmake" "${CMAKE_BINARY_DIR}/${PROJECT_NAME_LOWER}-config-version.cmake"
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}-${PROJECT_VERSION}
  COMPONENT development
)

install(EXPORT ${PROJECT_NAME}
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}-${PROJECT_VERSION}
  NAMESPACE ${PROJECT_NAME}::
  FILE ${PROJECT_NAME_LOWER}-export.cmake
  COMPONENT development
)

# ############################################################################
# New CPACK section, please see the README file inside cpack.d directory.
if(SIMAGE_USE_CPACK)
  add_subdirectory(cpack.d)
endif()
