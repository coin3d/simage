# Check for gdiplus support
IF(NOT DEFINED GDIPLUS_FOUND)
  TRY_COMPILE(USE_GDIPLUS_DEFAULT
    ${CMAKE_BINARY_DIR}/CMakeTmp
    ${CMAKE_SOURCE_DIR}/cmake/TestGdiPlus.cxx
    CMAKE_FLAGS "-DLINK_LIBRARIES:STRING=gdiplus"
    OUTPUT_VARIABLE OUTPUT)
  IF(USE_GDIPLUS_DEFAULT)
    MESSAGE(STATUS "Checking if GDI+ is available -- yes")
    OPTION(GDIPLUS_FOUND "Enable using GDI+ (gdiplus) on Windows." ON)
	set(GDIPLUS_LIBRARIES gdiplus)
    FILE(APPEND ${CMAKE_BINARY_DIR}/CMakeFiles/CMakeOutput.log
      "Checking if gdiplus is available "
      "passed with the following output:\n"
      "${OUTPUT}\n")
  ELSE()
    MESSAGE(STATUS "Checking if GDI+ is available -- no")
    OPTION(GDIPLUS_FOUND "Enable using GDI+ (gdiplus) on Windows." OFF)
    FILE(APPEND ${CMAKE_BINARY_DIR}/CMakeFiles/CMakeOutput.log
      "Checking if gdiplus is available "
      "failed with the following output:\n"
      "${OUTPUT}\n")
  ENDIF()
  MARK_AS_ADVANCED(GDIPLUS_FOUND)
ENDIF()

IF(GDIPLUS_FOUND)
	IF (NOT DEFINED HAVE_GDIPLUS_LOCKBITS_RECTARG_POINTER)
	  TRY_COMPILE(GDIPLUS_HAS_LOCKBITS
		${CMAKE_BINARY_DIR}/CMakeTmp
		${CMAKE_SOURCE_DIR}/cmake/TestGdiPlusLockBits.cxx
		CMAKE_FLAGS "-DLINK_LIBRARIES:STRING=gdiplus"
		OUTPUT_VARIABLE OUTPUT)
	  IF(GDIPLUS_HAS_LOCKBITS)
		MESSAGE(STATUS "Checking if GDI+ supports lockbits -- yes")
		OPTION(HAVE_GDIPLUS_LOCKBITS_RECTARG_POINTER "GDI+ supports LockBits." ON)
		FILE(APPEND ${CMAKE_BINARY_DIR}/CMakeFiles/CMakeOutput.log
		  "Checking if gdiplus supports lockbits "
		  "passed with the following output:\n"
		  "${OUTPUT}\n")
	  ELSE()
		MESSAGE(STATUS "Checking if GDI+ supports lockbits -- no")
		OPTION(HAVE_GDIPLUS_LOCKBITS_RECTARG_POINTER "GDI+ supports LockBits." OFF)
		FILE(APPEND ${CMAKE_BINARY_DIR}/CMakeFiles/CMakeOutput.log
		  "Checking if gdiplus supports lockbits "
		  "failed with the following output:\n"
		  "${OUTPUT}\n")
	  ENDIF()
	  MARK_AS_ADVANCED(HAVE_GDIPLUS_LOCKBITS_RECTARG_POINTER)
	ENDIF()
ENDIF()
	