# this file contains all helper macros and functions of the Coin3D project.

# option controlled helper for cmake variable dumping during config
function(dump_variable)
  if (COIN_VERBOSE)
    foreach(f ${ARGN})
      if (DEFINED ${f})
        message("${f} = ${${f}}")
      else()
        message("${f} = ***UNDEF***")
      endif()
    endforeach()
  endif()
endfunction()

# modifies the install directory passed by postfix, either substituting 'Coin' 
# with 'Coin${COIN_MAJOR_VERSION}' or adding the same versioned name after the 
# postfix part.
#
# Example: 
# versionize(INCLUDEDIR DOCDIR) will modify
#
# CMAKE_INSTALL_DOCDIR          'share/doc/Coin'          --> 'share/doc/Coin4'
# CMAKE_INSTALL_FULL_DOCDIR     '<prefix>/share/doc/Coin' --> '<prefix>/share/doc/Coin4'
#
# CMAKE_INSTALL_INCLUDEDIR      'include'          --> 'include/Coin4'
# CMAKE_INSTALL_FULL_INCLUDEDIR '<prefix>/include' --> '<prefix>/include/Coin4'
function(versionize)
  foreach(dir ${ARGN})
    set(name      "CMAKE_INSTALL_${dir}")
    set(full-name "CMAKE_INSTALL_FULL_${dir}")
    if(${name} MATCHES ${PROJECT_NAME})
      string(REPLACE "${PROJECT_NAME}" "${PROJECT_NAME}${PROJECT_VERSION_MAJOR}" value      "${${name}}")
      string(REPLACE "${PROJECT_NAME}" "${PROJECT_NAME}${PROJECT_VERSION_MAJOR}" full-value "${${full-name}}")
    else()
      set(value      "${${name}}/${PROJECT_NAME}${PROJECT_VERSION_MAJOR}")
      set(full-value "${CMAKE_INSTALL_PREFIX}/${value}")
    endif()
    set(${name}      ${value}      PARENT_SCOPE)
    set(${full-name} ${full-value} PARENT_SCOPE)
  endforeach()
endfunction()


# Checks all specified types for existence and sets variable and sets a variable HAVE_<type_name>
# if so. Additionally a variable named <type_name> is set to the size of the type.
# Moreover, ${TYPE_VARIABLE} will be set to the first type matching the specified ${TYPE_SIZE}.
macro(find_int_type_with_size TYPE_VARIABLE TYPE_SIZE)
  set(${TYPE_VARIABLE} "")
  foreach(TYPE ${ARGN})
    string(TOUPPER ${TYPE} TYPE_VAR)
    string(REPLACE " " "_" TYPE_VAR ${TYPE_VAR})
    check_type_size(${TYPE} ${TYPE_VAR})
    if((${TYPE_VAR} STREQUAL ${TYPE_SIZE}) AND (NOT ${TYPE_VARIABLE}))
      set(${TYPE_VARIABLE} ${TYPE})
      break()
    endif()
  endforeach()
endmacro()


# Replace the link type of the MSVCRT libraries
# statically: /MT|/MTd
# dynamically: /MD|/MDd
# Since CMake 3.15 this can be set by the CMAKE_MSVC_RUNTIME_LIBRARY variable
# or by setting the MSVC_RUNTIME_LIBRARY property of a target. But for being
# backwards compatible with older CMake versions this is needed.
macro(coin_msvc_link_static_crt _enable_static_crt)
  set(_vars          CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO)
  set(_vars ${_vars} CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
  if(${_enable_static_crt})
    message(STATUS "Build against static Microsoft Visual C runtime library")
    foreach(_flags ${_vars})
      if(${_flags} MATCHES "/MD")
        string(REGEX REPLACE "/MD" "/MT" ${_flags} "${${_flags}}")
      endif()
    endforeach()
    set(PKG_CONFIG_MSVC_LIBC "multithread-static")
  else()
    message(STATUS "Build against dynamic Microsoft Visual C runtime library")
    foreach(_flags ${_vars})
      if(${_flags} MATCHES "/MT")
        string(REGEX REPLACE "/MT" "/MD" ${_flags} "${${_flags}}")
      endif()
    endforeach()
    set(PKG_CONFIG_MSVC_LIBC "multithread-dynamic")
  endif()
endmacro()


function(report_prepare)
  set(multiValueArgs IF_APPLE IF_WIN32 IF_MSVC)
  cmake_parse_arguments(REPORT "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
  if(REPORT_IF_APPLE AND APPLE)
    list(APPEND res ${REPORT_IF_APPLE})
  endif()
  if(REPORT_IF_WIN32 AND WIN32)
    list(APPEND res ${REPORT_IF_WIN32})
  endif()
  if(REPORT_IF_MSVC AND MSVC)
    list(APPEND res ${REPORT_IF_MSVC})
  endif()
  list(APPEND res ${REPORT_UNPARSED_ARGUMENTS})
  list(APPEND PACKAGE_OPTIONS ${res})
  set(PACKAGE_OPTIONS "${PACKAGE_OPTIONS}" PARENT_SCOPE)
endfunction(report_prepare)
