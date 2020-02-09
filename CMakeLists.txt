cmake_minimum_required(VERSION 3.0)

set(SIMAGE_MAJOR_VERSION 1)
set(SIMAGE_MINOR_VERSION 8)
set(SIMAGE_MICRO_VERSION 0)
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

include(CheckCXXSourceCompiles)
include(CheckIncludeFiles)
include(CMakePackageConfigHelpers)
include(CMakeDependentOption)
include(GNUInstallDirs)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_MODULE_PATH}")

# ############################################################################
# Provide options to customise the build
# ############################################################################

option(SIMAGE_BUILD_SHARED_LIBS "Build shared libraries" ON)
option(SIMAGE_BUILD_DOCUMENTATION "Build and install API documentation (requires Doxygen)." OFF)
if(WIN32)
  option(SIMAGE_USE_AVIENC "Use Video for Windows for AVI encoding" ON)
  option(SIMAGE_USE_GDIPLUS "Use GDI+ on Windows to load/save images" ON)
elseif(APPLE)
  option(SIMAGE_USE_CGIMAGE "Use CGImage on OS X to load/save images" ON)
  option(SIMAGE_USE_QUICKTIME "Use QuickTime on OS X to load/save images" OFF)
endif()
option(SIMAGE_USE_QIMAGE "Use Qt's QImage to load/save images" OFF)
cmake_dependent_option(SIMAGE_USE_QT5 "Prefer Qt5 over Qt4 if available" ON "SIMAGE_USE_QIMAGE" OFF)
option(SIMAGE_USE_CPACK "If enabled the cpack subrepo is mandatory" OFF)

function(report_prepare)
  set(multiValueArgs IF_APPLE IF_WIN32)
  cmake_parse_arguments(REPORT "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )
  if (REPORT_IF_APPLE AND APPLE) 
    list(APPEND res ${REPORT_IF_APPLE})
  endif()
  if (REPORT_IF_WIN32 AND WIN32) 
    list(APPEND res ${REPORT_IF_WIN32})
  endif()
  list(APPEND res ${REPORT_UNPARSED_ARGUMENTS})
  list(APPEND PACKAGE_OPTIONS ${res} )
  set(PACKAGE_OPTIONS "${PACKAGE_OPTIONS}" PARENT_SCOPE)
endfunction(report_prepare)

# ############################################################################
# Find all necessary and optional simage dependencies
# ############################################################################

# Fail early if one of the required packages cannot be found

if(SIMAGE_USE_QIMAGE)
  if(SIMAGE_USE_QT5)
    find_package(Qt5 COMPONENTS Core Gui QUIET)
  endif()
  
  if(Qt5_FOUND)
    set(QT_FOUND TRUE)
    set(QT_LIBRARIES ${Qt5Core_LIBRARIES} ${Qt5Gui_LIBRARIES})
  else()
    set(QT_USE_IMPORTED_TARGETS ON)
    find_package(Qt4 COMPONENTS QtCore QtGui)
    include(${QT_USE_FILE})
  endif()
endif()

if(WIN32)
  # On Windows GDI+ supports GIF, JPEG, EXIF, PNG, and TIFF.  
  # see https://docs.microsoft.com/en-us/windows/desktop/gdiplus/-gdiplus-types-of-bitmaps-about
  find_package(GdiPlus)
  find_package(Vfw)
elseif(APPLE)
  # On macOS QuickTime supports BMP, GIF, JPEG, JPEG 2000, PNG, TIFF, and TGA.
  # see https://en.wikipedia.org/wiki/QuickTime
  find_package(QuickTime)
endif()
find_package(Jasper)
find_package(OggVorbis)
find_package(Sndfile)

if(APPLE AND SIMAGE_USE_CGIMAGE)
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
endif()

cmake_dependent_option(SIMAGE_AVIENC_SUPPORT "Enable support for AVI encoding (Win32 only)" ON "SIMAGE_USE_AVIENC;VFW_FOUND" OFF)
cmake_dependent_option(SIMAGE_CGIMAGE_SUPPORT "Use CGImage on macOS to load/save images" ON "APPLE;SIMAGE_USE_CGIMAGE;CGIMAGE_FOUND" OFF)
cmake_dependent_option(SIMAGE_GDIPLUS_SUPPORT "Use GDI+ on Windows to load/save images" ON "SIMAGE_USE_GDIPLUS;GDIPLUS_FOUND" OFF)
cmake_dependent_option(SIMAGE_LIBSNDFILE_SUPPORT "Use libsndfile to load/save sampled sound" ON "SNDFILE_FOUND" OFF)
cmake_dependent_option(SIMAGE_OGGVORBIS_SUPPORT "Enable support for ogg/vorbis extensions" ON "OGGVORBIS_FOUND" OFF)
cmake_dependent_option(SIMAGE_QIMAGE_SUPPORT "Use Qt's QImage to load/save images" ON "SIMAGE_USE_QIMAGE;QT_FOUND" OFF)
cmake_dependent_option(SIMAGE_QUICKTIME_SUPPORT "Use QuickTime on macOS to load/save images" ON "SIMAGE_USE_QUICKTIME;QUICKTIME_FOUND" OFF)

option(SIMAGE_EPS_SUPPORT "Enable support for encapsulated postscript export" ON)
option(SIMAGE_MPEG2ENC_SUPPORT "Enable support for mpeg2 encoding" ON)
option(SIMAGE_PIC_SUPPORT "Enable support for PIC images" ON)
option(SIMAGE_RGB_SUPPORT "Enable support for RGB images" ON)
if(NOT SIMAGE_QUICKTIME_SUPPORT AND NOT SIMAGE_CGIMAGE_SUPPORT)
  option(SIMAGE_TGA_SUPPORT "Enable support for Targa images" ON)
endif()
option(SIMAGE_XWD_SUPPORT "Enable support for XWD images (experimental)" OFF)

report_prepare(
  SIMAGE_BUILD_SHARED_LIBS
  SIMAGE_BUILD_DOCUMENTATION
  SIMAGE_USE_QIMAGE
  SIMAGE_USE_QT5
  SIMAGE_LIBSNDFILE_SUPPORT
  SIMAGE_OGGVORBIS_SUPPORT
  SIMAGE_QIMAGE_SUPPORT
  SIMAGE_EPS_SUPPORT
  SIMAGE_MPEG2ENC_SUPPORT
  SIMAGE_PIC_SUPPORT
  SIMAGE_RGB_SUPPORT
  SIMAGE_TGA_SUPPORT
  SIMAGE_XWD_SUPPORT
  IF_WIN32 
    SIMAGE_USE_AVIENC SIMAGE_USE_GDIPLUS SIMAGE_AVIENC_SUPPORT SIMAGE_GDIPLUS_SUPPORT
  IF_APPLE 
    SIMAGE_USE_CGIMAGE SIMAGE_USE_QUICKTIME SIMAGE_CGIMAGE_SUPPORT SIMAGE_QUICKTIME_SUPPORT
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

if(SIMAGE_GDIPLUS_SUPPORT)
  set(CMAKE_REQUIRED_INCLUDES ${GDIPLUS_INCLUDE_DIRS})
  set(CMAKE_REQUIRED_LIBRARIES ${GDIPLUS_LIBRARIES})
endif()

check_include_files(dlfcn.h HAVE_DLFCN_H)
if(WIN32)
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
endif()

if(NOT SIMAGE_QUICKTIME_SUPPORT AND NOT SIMAGE_CGIMAGE_SUPPORT AND NOT SIMAGE_GDIPLUS_SUPPORT)
  find_package(ZLIB)
  find_package(GIF)
  find_package(JPEG)
  find_package(PNG)
  find_package(TIFF)

  set(HAVE_ZLIB ${ZLIB_FOUND})
  set(HAVE_GIFLIB ${GIF_FOUND})
  set(HAVE_JPEGLIB ${JPEG_FOUND})
  set(HAVE_PNGLIB ${PNG_FOUND})
  set(HAVE_TIFFLIB ${TIFF_FOUND})
endif()

set(HAVE_JASPER ${JASPER_FOUND})
set(HAVE_VFW ${VFW_FOUND})

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

set(PACKAGE "simage")
set(PACKAGE_BUGREPORT "coin-support@coin3d.org")
set(PACKAGE_NAME "simage")
set(PACKAGE_STRING "simage ${SIMAGE_VERSION}")
set(PACKAGE_TARNAME "simage")
set(PACKAGE_URL "https://github.com/coin3d/simage")
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
  else()
    set(SIMAGE_DEFAULT_POSTFIX ${SIMAGE_DEFAULT_STATIC_POSTFIX})
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

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${VERSION} SOVERSION ${SIMAGE_SO_VERSION})

target_compile_definitions(${PROJECT_NAME} PRIVATE HAVE_CONFIG_H SIMAGE_INTERNAL SIMAGE_DEBUG=$<CONFIG:Debug>)

if(WIN32)
  if(MSVC)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_NONSTDC_NO_DEPRECATE _CRT_SECURE_NO_DEPRECATE _CRT_SECURE_NO_WARNINGS _USE_MATH_DEFINES)
  endif()
  if(SIMAGE_BUILD_SHARED_LIBS)
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
  target_link_libraries(simage PRIVATE "-framework CoreFoundation")
endif()

if(SIMAGE_GDIPLUS_SUPPORT)
  target_link_libraries(simage PRIVATE ${GDIPLUS_LIBRARIES})
endif()

if(GIF_FOUND)
  target_include_directories(simage PRIVATE ${GIF_INCLUDE_DIR} giflib)
  target_link_libraries(simage PRIVATE ${GIF_LIBRARIES})
endif()

if(JASPER_FOUND)
  target_include_directories(simage PRIVATE ${JASPER_INCLUDE_DIR})
  target_link_libraries(simage PRIVATE ${JASPER_LIBRARIES})
endif()

if(JPEG_FOUND)
  target_include_directories(simage PRIVATE ${JPEG_INCLUDE_DIR})
  target_link_libraries(simage PRIVATE ${JPEG_LIBRARIES})
endif()

if(OGGVORBIS_FOUND)
  target_include_directories(simage PRIVATE ${OGGVORBIS_INCLUDE_DIRS})
  target_link_libraries(simage PRIVATE ${OGGVORBIS_LIBRARIES})
endif()

if(PNG_FOUND)
  target_include_directories(simage PRIVATE ${PNG_INCLUDE_DIR})
  target_link_libraries(simage PRIVATE ${PNG_LIBRARIES})
endif()

if(SIMAGE_QIMAGE_SUPPORT)
  target_link_libraries(simage PRIVATE ${QT_LIBRARIES})
endif()

if(QUICKTIME_FOUND)
  target_include_directories(simage PRIVATE ${QUICKTIME_INCLUDE_DIR})
  target_link_libraries(simage PRIVATE ${QUICKTIME_LIBRARIES})
endif()

if(SNDFILE_FOUND)
  target_include_directories(simage PRIVATE ${SNDFILE_INCLUDE_DIRS})
  target_link_libraries(simage PRIVATE ${SNDFILE_LIBRARIES})
endif()

if(TIFF_FOUND)
  target_include_directories(simage PRIVATE ${TIFF_INCLUDE_DIR})
  target_link_libraries(simage PRIVATE ${TIFF_LIBRARY})
endif()

if(VFW_FOUND)
  target_link_libraries(simage PRIVATE ${VFW_LIBRARIES})
endif()

# ############################################################################
# Build examples
# ############################################################################

set(SIMAGE_EXAMPLE_SOURCE
  ${CMAKE_CURRENT_SOURCE_DIR}/examples/audio2raw.c
  ${CMAKE_CURRENT_SOURCE_DIR}/examples/img2avi.c
#  ${CMAKE_CURRENT_SOURCE_DIR}/examples/mpeg2enc.cpp # requires Coin to build
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

# ############################################################################
# Build tests
# ############################################################################

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

# ############################################################################
# Install pkgconfig file and CMake config package
# ############################################################################

if(UNIX OR MINGW)
  configure_file("${CMAKE_SOURCE_DIR}/${PROJECT_NAME}.pc.cmake.in" ${PROJECT_NAME}.pc @ONLY)
  install(FILES "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc" DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
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
if (SIMAGE_USE_CPACK)
  add_subdirectory(cpack.d)
endif()
