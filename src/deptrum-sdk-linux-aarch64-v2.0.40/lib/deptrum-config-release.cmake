#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "dt::deptrum" for configuration "Release"
set_property(TARGET dt::deptrum APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(dt::deptrum PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libdeptrum.so.2.0.40"
  IMPORTED_SONAME_RELEASE "libdeptrum.so.2"
  )

list(APPEND _IMPORT_CHECK_TARGETS dt::deptrum )
list(APPEND _IMPORT_CHECK_FILES_FOR_dt::deptrum "${_IMPORT_PREFIX}/lib/libdeptrum.so.2.0.40" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
