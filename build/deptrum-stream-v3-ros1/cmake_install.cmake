# Install script for directory: /home/dut/lidar6/src/deptrum-stream-v3-ros1

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/dut/lidar6/install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/deptrum_camera/msg" TYPE FILE FILES
    "/home/dut/lidar6/src/deptrum-stream-v3-ros1/msg/DeviceInfo.msg"
    "/home/dut/lidar6/src/deptrum-stream-v3-ros1/msg/Extrinsics.msg"
    "/home/dut/lidar6/src/deptrum-stream-v3-ros1/msg/Metadata.msg"
    "/home/dut/lidar6/src/deptrum-stream-v3-ros1/msg/ImuInfo.msg"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/deptrum_camera/srv" TYPE FILE FILES
    "/home/dut/lidar6/src/deptrum-stream-v3-ros1/srv/GetBool.srv"
    "/home/dut/lidar6/src/deptrum-stream-v3-ros1/srv/SetBool.srv"
    "/home/dut/lidar6/src/deptrum-stream-v3-ros1/srv/GetCameraInfo.srv"
    "/home/dut/lidar6/src/deptrum-stream-v3-ros1/srv/GetCameraParams.srv"
    "/home/dut/lidar6/src/deptrum-stream-v3-ros1/srv/GetDeviceInfo.srv"
    "/home/dut/lidar6/src/deptrum-stream-v3-ros1/srv/GetInt32.srv"
    "/home/dut/lidar6/src/deptrum-stream-v3-ros1/srv/SetFilter.srv"
    "/home/dut/lidar6/src/deptrum-stream-v3-ros1/srv/GetString.srv"
    "/home/dut/lidar6/src/deptrum-stream-v3-ros1/srv/SetInt32.srv"
    "/home/dut/lidar6/src/deptrum-stream-v3-ros1/srv/SetString.srv"
    "/home/dut/lidar6/src/deptrum-stream-v3-ros1/srv/SetArrays.srv"
    "/home/dut/lidar6/src/deptrum-stream-v3-ros1/srv/SetInt32TwoParam.srv"
    "/home/dut/lidar6/src/deptrum-stream-v3-ros1/srv/GetInt32TwoParam.srv"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/deptrum_camera/cmake" TYPE FILE FILES "/home/dut/lidar6/build/deptrum-stream-v3-ros1/catkin_generated/installspace/deptrum_camera-msg-paths.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "/home/dut/lidar6/devel/include/deptrum_camera")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/roseus/ros" TYPE DIRECTORY FILES "/home/dut/lidar6/devel/share/roseus/ros/deptrum_camera")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/common-lisp/ros" TYPE DIRECTORY FILES "/home/dut/lidar6/devel/share/common-lisp/ros/deptrum_camera")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/gennodejs/ros" TYPE DIRECTORY FILES "/home/dut/lidar6/devel/share/gennodejs/ros/deptrum_camera")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  execute_process(COMMAND "/usr/bin/python3" -m compileall "/home/dut/lidar6/devel/lib/python3/dist-packages/deptrum_camera")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/python3/dist-packages" TYPE DIRECTORY FILES "/home/dut/lidar6/devel/lib/python3/dist-packages/deptrum_camera")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/home/dut/lidar6/build/deptrum-stream-v3-ros1/catkin_generated/installspace/deptrum_camera.pc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/deptrum_camera/cmake" TYPE FILE FILES "/home/dut/lidar6/build/deptrum-stream-v3-ros1/catkin_generated/installspace/deptrum_camera-msg-extras.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/deptrum_camera/cmake" TYPE FILE FILES
    "/home/dut/lidar6/build/deptrum-stream-v3-ros1/catkin_generated/installspace/deptrum_cameraConfig.cmake"
    "/home/dut/lidar6/build/deptrum-stream-v3-ros1/catkin_generated/installspace/deptrum_cameraConfig-version.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/deptrum_camera" TYPE FILE FILES "/home/dut/lidar6/src/deptrum-stream-v3-ros1/package.xml")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libdeptrum_camera.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libdeptrum_camera.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libdeptrum_camera.so"
         RPATH ":/home/dut/lidar6/src/deptrum-stream-v3-ros1/ext/deptrum-sdk-linux-x86_64-v2.0.40-18.04/lib")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/dut/lidar6/devel/lib/libdeptrum_camera.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libdeptrum_camera.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libdeptrum_camera.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libdeptrum_camera.so"
         OLD_RPATH ":/home/dut/lidar6/src/deptrum-stream-v3-ros1/ext/deptrum-sdk-linux-x86_64-v2.0.40-18.04/lib:/opt/ros/noetic/lib:"
         NEW_RPATH ":/home/dut/lidar6/src/deptrum-stream-v3-ros1/ext/deptrum-sdk-linux-x86_64-v2.0.40-18.04/lib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libdeptrum_camera.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libdeptrum_camera_nodelet.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libdeptrum_camera_nodelet.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libdeptrum_camera_nodelet.so"
         RPATH ":/home/dut/lidar6/src/deptrum-stream-v3-ros1/ext/deptrum-sdk-linux-x86_64-v2.0.40-18.04/lib")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/dut/lidar6/devel/lib/libdeptrum_camera_nodelet.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libdeptrum_camera_nodelet.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libdeptrum_camera_nodelet.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libdeptrum_camera_nodelet.so"
         OLD_RPATH ":/home/dut/lidar6/src/deptrum-stream-v3-ros1/ext/deptrum-sdk-linux-x86_64-v2.0.40-18.04/lib:/opt/ros/noetic/lib:"
         NEW_RPATH ":/home/dut/lidar6/src/deptrum-stream-v3-ros1/ext/deptrum-sdk-linux-x86_64-v2.0.40-18.04/lib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libdeptrum_camera_nodelet.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/deptrum_camera/deptrum_camera_node" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/deptrum_camera/deptrum_camera_node")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/deptrum_camera/deptrum_camera_node"
         RPATH ":/home/dut/lidar6/src/deptrum-stream-v3-ros1/ext/deptrum-sdk-linux-x86_64-v2.0.40-18.04/lib")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/deptrum_camera" TYPE EXECUTABLE FILES "/home/dut/lidar6/devel/lib/deptrum_camera/deptrum_camera_node")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/deptrum_camera/deptrum_camera_node" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/deptrum_camera/deptrum_camera_node")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/deptrum_camera/deptrum_camera_node"
         OLD_RPATH ":/home/dut/lidar6/src/deptrum-stream-v3-ros1/ext/deptrum-sdk-linux-x86_64-v2.0.40-18.04/lib:/opt/ros/noetic/lib:/home/dut/lidar6/devel/lib:"
         NEW_RPATH ":/home/dut/lidar6/src/deptrum-stream-v3-ros1/ext/deptrum-sdk-linux-x86_64-v2.0.40-18.04/lib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/deptrum_camera/deptrum_camera_node")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/deptrum_camera/list_devices_node" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/deptrum_camera/list_devices_node")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/deptrum_camera/list_devices_node"
         RPATH ":/home/dut/lidar6/src/deptrum-stream-v3-ros1/ext/deptrum-sdk-linux-x86_64-v2.0.40-18.04/lib")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/deptrum_camera" TYPE EXECUTABLE FILES "/home/dut/lidar6/devel/lib/deptrum_camera/list_devices_node")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/deptrum_camera/list_devices_node" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/deptrum_camera/list_devices_node")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/deptrum_camera/list_devices_node"
         OLD_RPATH ":/home/dut/lidar6/src/deptrum-stream-v3-ros1/ext/deptrum-sdk-linux-x86_64-v2.0.40-18.04/lib:/opt/ros/noetic/lib:/home/dut/lidar6/devel/lib:"
         NEW_RPATH ":/home/dut/lidar6/src/deptrum-stream-v3-ros1/ext/deptrum-sdk-linux-x86_64-v2.0.40-18.04/lib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/deptrum_camera/list_devices_node")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/deptrum_camera/" TYPE DIRECTORY FILES "/home/dut/lidar6/src/deptrum-stream-v3-ros1/include")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/deptrum_camera/" TYPE DIRECTORY FILES "/home/dut/lidar6/src/deptrum-stream-v3-ros1/ext/deptrum-sdk-linux-x86_64-v2.0.40-18.04/include")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/deptrum_camera" TYPE DIRECTORY FILES "/home/dut/lidar6/src/deptrum-stream-v3-ros1/launch")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/deptrum_camera" TYPE FILE FILES "/home/dut/lidar6/src/deptrum-stream-v3-ros1/nodelet_plugins.xml")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/" TYPE DIRECTORY FILES "/home/dut/lidar6/src/deptrum-stream-v3-ros1/ext/deptrum-sdk-linux-x86_64-v2.0.40-18.04/lib/" FILES_MATCHING REGEX "/[^/]*\\.so$" REGEX "/[^/]*\\.so\\.[^/]*$")
endif()

