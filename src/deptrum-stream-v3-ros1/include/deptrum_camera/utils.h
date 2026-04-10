// Copyright (c) Deptrum Inc. All Rights Reserved.

#pragma once

#include "sensor_msgs/CameraInfo.h"
#include "sensor_msgs/distortion_models.h"
#include "deptrum/deptrum.hpp"
#include <tf2/LinearMath/Quaternion.h>
#include "types.h"
#include "sensor_msgs/PointCloud2.h"
#include "deptrum_camera/Extrinsics.h"
#include <opencv2/opencv.hpp>

// Utility function for failure messages
namespace deptrum_camera {
inline void LogFatal(const char *file, int line, const std::string &message) {
  std::cerr << "Check failed at " << file << ":" << line << ": " << message << std::endl;
  std::abort();
}
}  // namespace deptrum_camera

// Macros for checking conditions and comparing values
#define CHECK(condition) \
  (!(condition) ? LogFatal(__FILE__, __LINE__, "Check failed: " #condition) : (void)0)

template <typename T1, typename T2>
void CheckOp(const char *expr, const char *file, int line, T1 val1, T2 val2, bool result) {
  if (!result) {
    std::ostringstream os;
    os << "Check failed: " << expr << " (" << val1 << " vs. " << val2 << ")";
    deptrum_camera::LogFatal(file, line, os.str());
  }
}

#define CHECK_OP(opname, op, val1, val2) \
  CheckOp(#val1 " " #op " " #val2, __FILE__, __LINE__, val1, val2, (val1)op(val2))

#define CHECK_EQ(val1, val2) CHECK_OP(_EQ, ==, val1, val2)
#define CHECK_NE(val1, val2) CHECK_OP(_NE, !=, val1, val2)
#define CHECK_LE(val1, val2) CHECK_OP(_LE, <=, val1, val2)
#define CHECK_LT(val1, val2) CHECK_OP(_LT, <, val1, val2)
#define CHECK_GE(val1, val2) CHECK_OP(_GE, >=, val1, val2)
#define CHECK_GT(val1, val2) CHECK_OP(_GT, >, val1, val2)

// Overload for raw pointers
template <typename T>
T *CheckNotNull(T *ptr, const char *file, int line) {
  if (ptr == nullptr) {
    std::ostringstream os;
    os << "Null pointer passed to CheckNotNull at " << file << ":" << line;
    deptrum_camera::LogFatal(file, line, os.str());
  }
  return ptr;
}

// Template for smart pointers like std::shared_ptr, std::unique_ptr
template <typename T>
T &CheckNotNull(T &ptr, const char *file, int line) {
  if (ptr == nullptr) {
    std::ostringstream os;
    os << "Null pointer passed to CheckNotNull at " << file << ":" << line;
    deptrum_camera::LogFatal(file, line, os.str());
  }
  return ptr;
}

#if defined(CHECK_NOTNULL)
#undef CHECK_NOTNULL
#endif
#define CHECK_NOTNULL(val) CheckNotNull(val, __FILE__, __LINE__)

namespace deptrum_camera {
DtFormat DtFormatFromString(const std::string &format);

std::string DtFormatToString(const DtFormat &format);

// std::string DtDeviceTypeToString(const DtDeviceType &type);

sensor_msgs::CameraInfo ConvertToCameraInfo(DtCameraIntrinsic intrinsic,
                                            DtCameraDistortion distortion, int width);

void SavePointsToPly(std::shared_ptr<dt::Frame> frame, const std::string &fileName);

void SaveRgbPointsToPly(std::shared_ptr<dt::Frame> frame, const std::string &fileName);

void SaveRgbPointCloudMsgToPly(const sensor_msgs::PointCloud2 &msg, const std::string &fileName);

void SaveDepthPointCloudMsgToPly(const sensor_msgs::PointCloud2 &msg, const std::string &fileName);

tf2::Quaternion RotationMatrixToQuaternion(const float rotation[9]);

std::ostream &operator<<(std::ostream &os, const DtCameraParam &rhs);

Extrinsics DtExtrinsicsToMsg(const DtD2CTransform &extrinsics, const std::string &frame_id);

ros::Time FromMsToRosTime(uint64_t ms);

ros::Time FromUsToRosTime(uint64_t us);

kDtSampleRate SampleRateFromString(std::string &sample_rate);

std::string SampleRateToString(const kDtSampleRate &sample_rate);

dt_gyro_full_scale_range FullGyroScaleRangeFromString(std::string &full_scale_range);

std::string FullGyroScaleRangeToString(const dt_gyro_full_scale_range &full_scale_range);

DtAccelFullScaleRange FullAccelScaleRangeFromString(std::string &full_scale_range);

std::string FullAccelScaleRangeToString(const DtAccelFullScaleRange &full_scale_range);

bool IsValidJpeg(const std::shared_ptr<dt::ColorFrame> &frame);

std::string FourccToString(uint32_t fourcc);

std::string MetaDataTypeToString(const DtFrameMetadataType &meta_data_type);

// DtHoleFillingMode HoleFillingModeFromString(const std::string &hole_filling_mode);

std::ostream &operator<<(std::ostream &os, const DtFormat &rhs);

std::string DtSensorTypeToString(const DtSensorType &type);

DtStreamType DtStreamTypeFromString(const std::string &stream_type);

std::ostream &operator<<(std::ostream &os, const DtSensorType &rhs);

std::string GetDistortionModels(DtCameraDistortion distortion);


bool IsNebula410Pid(const char *pid);

bool IsNebulaSeriesPid(const char *pid);

bool IsNebula280Pid(const char *pid);

bool IsPisca1000Pid(const char *pid);

}  // namespace deptrum_camera
