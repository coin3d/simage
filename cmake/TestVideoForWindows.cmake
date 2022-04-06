# Check for vfw32 support
IF(NOT DEFINED VFW_FOUND)
  TRY_COMPILE(USE_VIDEO_FOR_WINDOWS_DEFAULT
    ${CMAKE_BINARY_DIR}/CMakeTmp
    ${CMAKE_SOURCE_DIR}/cmake/Testvfw32.cxx
    CMAKE_FLAGS "-DLINK_LIBRARIES:STRING=vfw32"
    OUTPUT_VARIABLE OUTPUT)
  IF(USE_VIDEO_FOR_WINDOWS_DEFAULT)
    MESSAGE(STATUS "Checking if vfw32 (Video for Windows) is available -- yes")
    OPTION(VFW_FOUND "Enable using Video for Windows (vfw32) for video input and output." ON)
	set(VFW_LIBRARIES vfw32)
    FILE(APPEND ${CMAKE_BINARY_DIR}/CMakeFiles/CMakeOutput.log
      "Checking if vfw32 is available "
      "passed with the following output:\n"
      "${OUTPUT}\n")
  ELSE()
    MESSAGE(STATUS "Checking if vfw32 (Video for Windows) is available -- no")
    OPTION(VFW_FOUND "Enable using Video for Windows (vfw32) for video input and output." OFF)
    FILE(APPEND ${CMAKE_BINARY_DIR}/CMakeFiles/CMakeOutput.log
      "Checking if vfw32 is available "
      "failed with the following output:\n"
      "${OUTPUT}\n")
  ENDIF()
  MARK_AS_ADVANCED(VFW_FOUND)
ENDIF()
