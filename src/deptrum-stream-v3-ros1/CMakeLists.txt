cmake_minimum_required(VERSION 3.0.2)
project(deptrum_camera)

# Compiler settings
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -O3")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fPIC -g3")
set(CMAKE_BUILD_TYPE "Release")

option(ENABLE_SANITIZER "Enable sanitizer options" OFF)

if (ENABLE_SANITIZER)
    set(SANITIZER_FLAGS "-fsanitize=address -static-libasan ")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SANITIZER_FLAGS}")

    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${SANITIZER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${SANITIZER_FLAGS}")
endif()

# Options
option(USE_RK_HW_DECODER "Use Rockchip hardware decoder" OFF)
option(USE_NV_HW_DECODER "Use Nvidia hardware decoder" OFF)

#ci设置版本
set(ROS_MAJOR_VERSION_NEBULA401 0)
set(ROS_MINOR_VERSION_NEBULA401  0)
set(ROS_REVISION_VERSION_NEBULA401 8)

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/version_ros_nebula.h.in"
  "${CMAKE_CURRENT_SOURCE_DIR}/version_ros_nebula.h"
   )

#download sdk
set(EXTERNAL_DIR_NAME ext)
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${EXTERNAL_DIR_NAME}")
  execute_process(
    COMMAND echo "make directory ${EXTERNAL_DIR_NAME}..."
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/${EXTERNAL_DIR_NAME}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )
endif()


string(TOLOWER "${CMAKE_SYSTEM_NAME}" LOWER_SYSTEM_NAME)
string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" LOWER_PROCESSOR_NAME)
set(SYSTEM_PLATFORM "${LOWER_SYSTEM_NAME}-${LOWER_PROCESSOR_NAME}")
message(STATUS "SYSTEM_PLATFORM: ${SYSTEM_PLATFORM}")

#get linux verson
if(EXISTS "/etc/os-release")
    file(STRINGS "/etc/os-release" os_release)
    
    set(LINUX_VERSION "")
    foreach(line IN LISTS os_release)
        if(line MATCHES "^VERSION_ID=\"?([0-9]+\\.[0-9]+)\"?")
            set(LINUX_VERSION "${CMAKE_MATCH_1}")
            break() 
        endif()
    endforeach()
endif()

# 输出结果
if(LINUX_VERSION)
    message(STATUS "Detected Linux version: ${LINUX_VERSION}")
    if(LINUX_VERSION EQUAL 16.04)
      set(NAS_LINUX_VERSION 16.04)
    else()
      set(NAS_LINUX_VERSION 18.04)
    endif()
    
else()
    message(WARNING "Failed to detect Linux version")
    set(NAS_LINUX_VERSION 18.04)
endif()

#SDK Version
set(DEPTRUM_STREAM_SDK_VERSION  2.0.40)
set(DEPTRUM_SDK_LIBS deptrum)
set(STREAM_SDK_PKG_NAME  deptrum-sdk-${SYSTEM_PLATFORM}-v${DEPTRUM_STREAM_SDK_VERSION})

include(cmake/Download.cmake)
if(${LOWER_PROCESSOR_NAME} STREQUAL "aarch64")
set(STREAM_SDK_PACKAGE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${EXTERNAL_DIR_NAME}/${STREAM_SDK_PKG_NAME})
else()
set(STREAM_SDK_PACKAGE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${EXTERNAL_DIR_NAME}/${STREAM_SDK_PKG_NAME}-${NAS_LINUX_VERSION})
endif()

download_sdk(deptrum-sdk v${DEPTRUM_STREAM_SDK_VERSION})

file(REMOVE ${STREAM_SDK_PACKAGE_PATH}/InstallDriver.txt)
file(REMOVE_RECURSE ${STREAM_SDK_PACKAGE_PATH}/doc)

message(STATUS "link_directories DEPTRUM_SDK_STATIC_LIBRARY_DIRS: ${DEPTRUM_SDK_STATIC_LIBRARY_DIRS}")

set(CMAKE_BUILD_RPATH "${CMAKE_BUILD_RPATH}:${DEPTRUM_SDK_STATIC_LIBRARY_DIRS}")
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_RPATH}:${DEPTRUM_SDK_STATIC_LIBRARY_DIRS}")


if (USE_NV_HW_DECODER)
  set(JETSON_MULTI_MEDIA_API_DIR /usr/src/jetson_multimedia_api)
  set(JETSON_MULTI_MEDIA_API_CLASS_DIR ${JETSON_MULTI_MEDIA_API_DIR}/samples/common/classes)
  set(JETSON_MULTI_MEDIA_API_INCLUDE_DIR ${JETSON_MULTI_MEDIA_API_DIR}/include/)
  set(LIBJPEG8B_INCLUDE_DIR ${JETSON_MULTI_MEDIA_API_INCLUDE_DIR}/libjpeg-8b)
  set(TEGRA_ARMABI /usr/lib/aarch64-linux-gnu/)
  set(NV_LIBRARIES
    -lnvjpeg -lnvbufsurface -lnvbufsurftransform -lyuv -lv4l2
  )
  list(APPEND NV_LIBRARIES
    -L${TEGRA_ARMABI} -L${TEGRA_ARMABI}/tegra)
endif ()
# Dependencies
find_package(catkin REQUIRED
  camera_info_manager
  cv_bridge
  dynamic_reconfigure
  image_geometry
  image_transport
  message_filters
  message_generation
  roscpp
  sensor_msgs
  std_srvs
  tf2
  tf2_ros
  pluginlib
  nodelet
  diagnostic_updater
)
set(SERVICE_FILES
  GetBool.srv
  SetBool.srv
  GetCameraInfo.srv
  GetCameraParams.srv
  GetDeviceInfo.srv
  GetInt32.srv
  SetFilter.srv
  GetString.srv
  SetInt32.srv
  SetString.srv
  SetArrays.srv
  SetInt32TwoParam.srv
  GetInt32TwoParam.srv
)

find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(Boost REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(Threads REQUIRED)
find_package(Boost COMPONENTS filesystem system REQUIRED)
# check if boost was found
if(Boost_FOUND)
    message(STATUS "Boost found!")
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
else()
    message(FATAL_ERROR "Boost not found!")
endif()

# Additional dependencies based on options
if (USE_RK_HW_DECODER)
  pkg_search_module(RK_MPP REQUIRED rockchip_mpp)
  if (NOT RK_MPP_FOUND)
    message(FATAL_ERROR "Rockchip MPP not found")
  endif ()
  pkg_search_module(RGA librga)
  if (NOT RGA_FOUND)
    message(STATUS "Rockchip RGA not found, use libyuv instead")
    add_definitions(-DUSE_LIBYUV)
    add_compile_options(-lyuv)
  endif ()
endif ()

# Message generation
add_message_files(FILES DeviceInfo.msg Extrinsics.msg Metadata.msg ImuInfo.msg)
add_service_files(FILES ${SERVICE_FILES})
generate_messages(DEPENDENCIES std_msgs sensor_msgs)

# Catkin package
catkin_package(
  INCLUDE_DIRS include
  LIBRARIES ${PROJECT_NAME}
  CATKIN_DEPENDS
  camera_info_manager
  cv_bridge
  dynamic_reconfigure
  image_geometry
  image_transport
  message_filters
  message_runtime
  roscpp
  sensor_msgs
  std_srvs
  tf2
  tf2_ros
  backward_ros
)

# Include directories
set(COMMON_INCLUDE_DIRS
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  ${catkin_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
  ${OpenCV_INCLUDED_DIRS}
  ${DEPTRUM_SDK_INCLUDE_DIRS}
)
if (USE_NV_HW_DECODER)
  list(APPEND COMMON_INCLUDE_DIRS
    ${JETSON_MULTI_MEDIA_API_INCLUDE_DIR}
    ${LIBJPEG8B_INCLUDE_DIR}
  )
endif ()

if (USE_RK_HW_DECODER)
  list(APPEND COMMON_INCLUDE_DIRS
    ${RK_MPP_INCLUDE_DIRS}
    ${RGA_INCLUDE_DIR})
endif ()

# Source files
set(SOURCE_FILES
  src/d2c_viewer.cpp
  src/dt_camera_node.cpp
  src/dt_camera_node_driver.cpp
  src/ros_sensor.cpp
  src/ros_service.cpp
  src/utils.cpp
  src/ros_setup.cpp
  src/jpeg_decoder.cpp
)

# Additional source files based on options
if (USE_RK_HW_DECODER)
  add_definitions(-DUSE_RK_HW_DECODER)
  list(APPEND SOURCE_FILES src/rk_mpp_decoder.cpp)
endif ()


if (USE_NV_HW_DECODER)
  add_definitions(-DUSE_NV_HW_DECODER)
  list(APPEND SOURCE_FILES src/jetson_nv_decoder.cpp)
  # append jetson_multimedia_api source files
  list(APPEND SOURCE_FILES
    ${JETSON_MULTI_MEDIA_API_CLASS_DIR}/NvBuffer.cpp
    ${JETSON_MULTI_MEDIA_API_CLASS_DIR}/NvElement.cpp
    ${JETSON_MULTI_MEDIA_API_CLASS_DIR}/NvElementProfiler.cpp
    ${JETSON_MULTI_MEDIA_API_CLASS_DIR}/NvJpegDecoder.cpp
    ${JETSON_MULTI_MEDIA_API_CLASS_DIR}/NvJpegEncoder.cpp
    ${JETSON_MULTI_MEDIA_API_CLASS_DIR}/NvLogging.cpp
    ${JETSON_MULTI_MEDIA_API_CLASS_DIR}/NvUtils.cpp
    ${JETSON_MULTI_MEDIA_API_CLASS_DIR}/NvV4l2Element.cpp
    ${JETSON_MULTI_MEDIA_API_CLASS_DIR}/NvV4l2ElementPlane.cpp
    ${JETSON_MULTI_MEDIA_API_CLASS_DIR}/NvVideoDecoder.cpp
    ${JETSON_MULTI_MEDIA_API_CLASS_DIR}/NvVideoEncoder.cpp
    ${JETSON_MULTI_MEDIA_API_CLASS_DIR}/NvBufSurface.cpp
  )
endif ()

# Link libraries
set(COMMON_LINK_LIBRARIES
  ${catkin_LIBRARIES}
  ${OpenCV_LIBS}
  ${Boost_LIBRARIES}
  -l${DEPTRUM_SDK_LIBS}
  -L${DEPTRUM_SDK_STATIC_LIBRARY_DIRS}
  Threads::Threads
  -lrt
  -ldw # for stack trace
)

if (USE_RK_HW_DECODER)
  list(APPEND COMMON_LINK_LIBRARIES
    ${RK_MPP_LIBRARIES}
    ${RGA_LIBRARIES}
  )
elseif (USE_NV_HW_DECODER)
  list(APPEND COMMON_LINK_LIBRARIES
    ${NV_LIBRARIES}
  )
endif ()

# Add libraries
add_library(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${COMMON_LINK_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC ${COMMON_INCLUDE_DIRS})

add_library(${PROJECT_NAME}_nodelet ${SOURCE_FILES} src/ros_nodelet.cpp)
target_link_libraries(${PROJECT_NAME}_nodelet ${COMMON_LINK_LIBRARIES})
target_include_directories(${PROJECT_NAME}_nodelet PUBLIC ${COMMON_INCLUDE_DIRS})

# Add dependencies
add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}_generate_messages_cpp)
add_dependencies(${PROJECT_NAME}_nodelet ${PROJECT_NAME}_generate_messages_cpp)

# Macro for adding executables
macro(add_deptrum_executable TARGET SOURCES)
  add_executable(${TARGET} ${SOURCES})
  target_link_libraries(${TARGET} ${COMMON_LINK_LIBRARIES} ${PROJECT_NAME})
  target_include_directories(${TARGET} PUBLIC ${COMMON_INCLUDE_DIRS})
endmacro()

# Add nodes
add_deptrum_executable(list_devices_node src/list_devices_node.cpp)
add_deptrum_executable(deptrum_camera_node src/main.cpp)

# Install
install(TARGETS ${PROJECT_NAME} ${PROJECT_NAME}_nodelet ${EXECUTABLES}
  deptrum_camera_node
  list_devices_node
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

if(DEFINED ENV{BUILDING_PACKAGE})
  # Install udev rules
  install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/scripts/99-deptrum-libusb.rules
    DESTINATION /etc/udev/rules.d
  )
endif()

install(DIRECTORY include DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}/)
install(DIRECTORY ${DEPTRUM_SDK_INCLUDE_DIRS} DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}/)
install(DIRECTORY launch DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})
# install(DIRECTORY config DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})
install(FILES nodelet_plugins.xml DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})
install(DIRECTORY ${DEPTRUM_SDK_STATIC_LIBRARY_DIRS}/
  DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}/
  FILES_MATCHING PATTERN "*.so"
  PATTERN "*.so.*"
)
