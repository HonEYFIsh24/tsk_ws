// Copyright (c) Deptrum Inc. All Rights Reserved.

#pragma once
#include "deptrum/deptrum.hpp"
#include "constants.h"
#include "utils.h"
#include "json.hpp"
#include <functional>
#include <boost/optional.hpp>
#include <eigen3/Eigen/Dense>
#include "deptrum_camera/DeviceInfo.h"
#include "deptrum_camera/Extrinsics.h"
#include "deptrum_camera/Metadata.h"
#include "deptrum_camera/GetDeviceInfo.h"
#include "deptrum_camera/GetBool.h"
#include "deptrum_camera/GetInt32.h"
#include "deptrum_camera/GetInt32TwoParam.h"
#include "deptrum_camera/GetString.h"
#include "deptrum_camera/GetCameraInfo.h"
#include "deptrum_camera/SetBool.h"
#include "deptrum_camera/SetInt32.h"
#include "deptrum_camera/SetInt32TwoParam.h"
#include "deptrum_camera/SetString.h"
#include "deptrum_camera/GetCameraParams.h"
#include "deptrum_camera/SetFilter.h"
#include "deptrum_camera/SetArrays.h"
#include "std_srvs/SetBool.h"
#include "std_srvs/Empty.h"
#include <boost/filesystem.hpp>
#include <atomic>
#include <mutex>

namespace deptrum_camera {

typedef std::pair<dt_stream_type, int> stream_index_pair;

const stream_index_pair COLOR{kDtStreamColor, 0};
const stream_index_pair DEPTH{kDtStreamDepth, 0};
const stream_index_pair INFRA0{kDtStreamIr, 0};
const stream_index_pair INFRA1{kDtStreamIrLeft, 0};
const stream_index_pair INFRA2{kDtStreamIrRight, 0};

const stream_index_pair GYRO{kDtStreamGyro, 0};
const stream_index_pair ACCEL{kDtStreamAccel, 0};

const std::vector<stream_index_pair> IMAGE_STREAMS = {DEPTH, INFRA0, COLOR, INFRA1, INFRA2};

const std::vector<stream_index_pair> HID_STREAMS = {GYRO, ACCEL};

const std::map<DtStreamType, DtFrameType> STREAM_TYPE_TO_FRAME_TYPE = {
    {kDtStreamColor, kDtFrameColor},
    {kDtStreamDepth, kDtFrameDepth},
    {kDtStreamIr, kDtFrameIr},
    {kDtStreamIrLeft, kDtFrameIrLeft},
    {kDtStreamIrRight, kDtFrameIrRight},
    {kDtStreamGyro, kDtFrameGyro},
    {kDtStreamAccel, kDtFrameAccel},
};

/**
 * @brief The rect of the region of interest
 */
typedef struct {
  uint32_t x0_left;
  uint32_t y0_top;
  uint32_t width;
  uint32_t height;
} CropRoi;
}  // namespace deptrum_camera
