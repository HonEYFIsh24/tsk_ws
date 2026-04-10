# Copyright (c) Deptrum Inc. All Rights Reserved.

cmake_minimum_required(VERSION 3.11)

file(GLOB subdirectories RELATIVE ${CMAKE_CURRENT_LIST_DIR} "*")
option(BUILD_GRAPHICAL_EXAMPLES "Build examples with OpenGL" OFF)

if(WIN32)
  find_package(Threads REQUIRED) 
  set(dt_threads Threads::Threads)
elseif(UNIX AND NOT ANDROID)
  set(dt_threads pthread)
elseif(ANDROID)
  set(dt_threads)
endif()

# dependecies
if(NOT WIN32)
  if (POLICY CMP0072)
    cmake_policy(SET CMP0072 NEW)
  endif()
  set(OpenGL_GL_PREFERENCE "GLVND")
  find_package(OpenGL QUIET)
  if(${OpenGL_FOUND} AND NOT ${CMAKE_CROSSCOMPILING})
    if(NOT TARGET OpenGL::GL)
      set(BUILD_GRAPHICAL_EXAMPLES ON)
    endif()
  endif()
else()
  find_package(OpenGL QUIET)
  if(${OpenGL_FOUND})
    set(BUILD_GRAPHICAL_EXAMPLES ON)
  endif()
endif()

find_package(OpenCV QUIET)
if(BUILD_GRAPHICAL_EXAMPLES)
  message(STATUS "OPENGL_LIBRARIES: ${OPENGL_LIBRARIES}")

  find_package(GLFW CONFIG QUIET)
  if(NOT ${GLFW_FOUND})
    include(FetchContent)
    set(FETCHCONTENT_QUIET OFF)
    set(FETCHCONTENT_RETRIES 3)
    set(FETCHCONTENT_TIMEOUT 30)

    FetchContent_Declare(
      glfw
      URL http://192.168.104.3/data/third_party/src/glfw-3.3.zip
      URL https://github.com/glfw/glfw/releases/download/3.3/glfw-3.3.zip
      URL_HASH SHA256=36FDA4CB173E3EB2928C976B0E9B5014E2E5D12B9B787EFA0AA29FFC41C37C4A
    )
    FetchContent_GetProperties(glfw)
    if(NOT glfw_POPULATED)
      FetchContent_Populate(glfw)
      set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs")
      set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs")
      set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")
      set(GLFW_INSTALL OFF CACHE BOOL "Generate installation target")
      add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})
    endif()
  else()
    set(BUILD_GRAPHICAL_EXAMPLES OFF)
    message(STATUS "OpenGL or GLFW not found, skip building graphical examples")
  endif()
endif(BUILD_GRAPHICAL_EXAMPLES)

# utils
add_subdirectory(utils)

# c examples
add_subdirectory(c)

# cpp examples
add_subdirectory(enumerate)
add_subdirectory(device_control)
add_subdirectory(device_firmware_update)
add_subdirectory(imu)
add_subdirectory(point_cloud)
add_subdirectory(multi_devices_no_image)
add_subdirectory(metadata)
add_subdirectory(palm)
add_subdirectory(logger)

# cpp examples with OpenGl Requirements
if(BUILD_GRAPHICAL_EXAMPLES)
    add_subdirectory(capture)
    add_subdirectory(callback)
    add_subdirectory(color)
    add_subdirectory(coordinate_transform)
    add_subdirectory(depth)
    add_subdirectory(raw)
    add_subdirectory(device_record)
    add_subdirectory(device_playback)
    add_subdirectory(hot_plugin)
    add_subdirectory(multi_devices)
    add_subdirectory(multi_streams)
    add_subdirectory(post_processing)
    add_subdirectory(point_cloud_capture)
endif()

if(${OpenCV_FOUND})
  add_subdirectory(save_to_disk)
endif()

if(BUILD_GRAPHICAL_EXAMPLES)
  add_compile_definitions(BUILD_GRAPHICAL_EXAMPLES)
endif()
add_subdirectory(test_tool)

# InstallRequiredSystemLibraries
include(InstallRequiredSystemLibraries)
