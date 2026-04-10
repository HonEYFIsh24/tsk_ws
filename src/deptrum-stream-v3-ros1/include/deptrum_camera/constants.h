// Copyright (c) Deptrum Inc. All Rights Reserved.

#pragma once

#include <string>
#include <cstdlib>

#define THREAD_NUM 4

#define DT_ROS_MAJOR_VERSION 0
#define DT_ROS_MINOR_VERSION 0
#define DT_ROS_PATCH_VERSION 8

#ifndef STRINGIFY
#define STRINGIFY(arg) #arg
#endif
#ifndef VAR_ARG_STRING
#define VAR_ARG_STRING(arg) STRINGIFY(arg)
#endif

#define STRINGIFY(arg) #arg
#define VAR_ARG_STRING(arg) STRINGIFY(arg)
/* Return version in "X.Y.Z" format */
#define DT_ROS_VERSION_STR \
  (VAR_ARG_STRING(DT_ROS_MAJOR_VERSION.DT_ROS_MINOR_VERSION.DT_ROS_PATCH_VERSION))

namespace deptrum_camera {

const int IMAGE_WIDTH = 640;
const int IMAGE_HEIGHT = 480;
const int IMAGE_FPS = 30;


const std::string DET_DEFAULT_LOCK_NAME = "deptrum_device.lock";
}  // namespace deptrum_camera
