// Copyright (c) Deptrum Inc. All Rights Reserved.

#include "deptrum_camera/utils.h"
#include <tf2/LinearMath/Quaternion.h>
#include "sensor_msgs/PointCloud2.h"
#include "sensor_msgs/PointCloud.h"

#include "sensor_msgs/point_cloud2_iterator.h"
#include "sensor_msgs/point_cloud_conversion.h"
#include "ros/ros.h"

namespace deptrum_camera {
DtFormat DtFormatFromString(const std::string &format) {
  std::string fixed_format;
  std::transform(format.begin(), format.end(), std::back_inserter(fixed_format),
                 [](const char ch) { return std::isalpha(ch) ? toupper(ch) : ch; });
  if (fixed_format == "MJPG") {
    return kDtFormatMjpg;
  } else if (fixed_format == "YUYV") {
    return kDtFormatYuyv;
  } else if (fixed_format == "YUYV2") {
    return kDtFormatYuy2;
  } else if (fixed_format == "UYVY") {
    return kDtFormatUyvy;
  } else if (fixed_format == "NV12") {
    return kDtFormatNv12;
  } else if (fixed_format == "NV21") {
    return kDtFormatNv21;
  } else if (fixed_format == "H264") {
    return kDtFormatH264;
  } else if (fixed_format == "H265") {
    return kDtFormatH265;
  } else if (fixed_format == "Y16") {
    return kDtFormatY16;
  } else if (fixed_format == "Y8") {
    return kDtFormatY8;
  } else if (fixed_format == "Y10") {
    return kDtFormatY10;
  } else if (fixed_format == "Y11") {
    return kDtFormatY11;
  } else if (fixed_format == "Y12") {
    return kDtFormatY12;
  } else if (fixed_format == "GRAY") {
    return kDtFormatGray;
  } else if (fixed_format == "HEVC") {
    return kDtFormatHevc;
  } else if (fixed_format == "I420") {
    return kDtFormatI420;
  } else if (fixed_format == "ACCEL") {
    return kDtFormatAccel;
  } else if (fixed_format == "GYRO") {
    return kDtFormatGyro;
  } else if (fixed_format == "POINT") {
    return kDtFormatPoint;
  } else if (fixed_format == "RGB_POINT") {
    return kDtFormatRgbPoint;
  } else if (fixed_format == "RLE") {
    return kDtFormatRle;
  } else if (fixed_format == "RGB888" || fixed_format == "RGB") {
    return kDtFormatRgb;
  } else if (fixed_format == "BGR") {
    return kDtFormatBgr;
  } else if (fixed_format == "Y14") {
    return kDtFormatY14;
  } else if (fixed_format == "BGRA") {
    return kDtFormatBgra;
  } else if (fixed_format == "COMPRESSED") {
    return kDtFormatCompressed;
  } else if (fixed_format == "RVL") {
    return kDtFormatRvl;
  } else if (fixed_format == "Z16") {
    return kDtFormatZ16;
  } else if (fixed_format == "YV12") {
    return kDtFormatYv12;
  } else if (fixed_format == "BA81") {
    return kDtFormatBa81;
  } else if (fixed_format == "RGBA") {
    return kDtFormatRgba;
  } else if (fixed_format == "BYR2") {
    return kDtFormatByr2;
  } else if (fixed_format == "RW16") {
    return kDtFormatRw16;
  } else if (fixed_format == "Y32") {
    return kDtFormatY32;
  } else if (fixed_format == "Palm") {
    return kDtFormatPalm;
  }
  //   else if (fixed_format == "DISP16") {
  //     return DT_FORMAT_DISP16;
  //   }
  else {
    return kDtFormatUnknown;
  }
}

std::string DtFormatToString(const DtFormat &format) {
  switch (format) {
    case kDtFormatMjpg:
      return "MJPG";
    case kDtFormatYuyv:
      return "YUYV";
    case kDtFormatYuy2:
      return "YUYV2";
    case kDtFormatUyvy:
      return "UYVY";
    case kDtFormatNv12:
      return "NV12";
    case kDtFormatNv21:
      return "NV21";
    case kDtFormatH264:
      return "H264";
    case kDtFormatH265:
      return "H265";
    case kDtFormatY16:
      return "Y16";
    case kDtFormatY8:
      return "Y8";
    case kDtFormatY10:
      return "Y10";
    case kDtFormatY11:
      return "Y11";
    case kDtFormatY12:
      return "Y12";
    case kDtFormatGray:
      return "GRAY";
    case kDtFormatHevc:
      return "HEVC";
    case kDtFormatI420:
      return "I420";
    case kDtFormatAccel:
      return "ACCEL";
    case kDtFormatGyro:
      return "GYRO";
    case kDtFormatPoint:
      return "POINT";
    case kDtFormatRgbPoint:
      return "RGB_POINT";
    case kDtFormatRle:
      return "RLE";
    case kDtFormatRgb:
      return "RGB888";
    case kDtFormatBgr:
      return "BGR";
    case kDtFormatY14:
      return "Y14";
    case kDtFormatBgra:
      return "BGRA";
    case kDtFormatCompressed:
      return "COMPRESSED";
    case kDtFormatRvl:
      return "RVL";
    case kDtFormatZ16:
      return "Z16";
    case kDtFormatYv12:
      return "YV12";
    case kDtFormatBa81:
      return "BA81";
    case kDtFormatRgba:
      return "RGBA";
    case kDtFormatByr2:
      return "BYR2";
    case kDtFormatRw16:
      return "RW16";
    case kDtFormatY32:
      return "Y32";
    case kDtFormatPalm:
      return "Palm";
    // case DT_FORMAT_DISP16:
    //   return "DISP16";
    default:
      return "UNKNOWN";
  }
}

// std::string DtDeviceTypeToString(const DtDeviceType &type) {
//   switch (type) {
//     case DtDeviceType::DT_STRUCTURED_LIGHT_BINOCULAR_CAMERA:
//       return "structured light binocular camera";
//     case DtDeviceType::DT_STRUCTURED_LIGHT_MONOCULAR_CAMERA:
//       return "structured light monocular camera";
//     case DtDeviceType::DT_TOF_CAMERA:
//       return "tof camera";
//   }
//   return "unknown technology camera";
// }

sensor_msgs::CameraInfo ConvertToCameraInfo(DtCameraIntrinsic intrinsic,
                                            DtCameraDistortion distortion, int width) {
  (void)width;
  sensor_msgs::CameraInfo info;
  info.distortion_model = GetDistortionModels(distortion);
  info.width = intrinsic.width;
  info.height = intrinsic.height;
  info.D.resize(8, 0.0);
  info.D[0] = distortion.k1;
  info.D[1] = distortion.k2;
  info.D[2] = distortion.p1;
  info.D[3] = distortion.p2;
  info.D[4] = distortion.k3;
  info.D[5] = distortion.k4;
  info.D[6] = distortion.k5;
  info.D[7] = distortion.k6;

  bool all_zero = std::all_of(info.D.begin(), info.D.end(), [](double val) { return val == 0.0; });
  info.roi.do_rectify = all_zero;

  info.K.fill(0.0);
  info.K[0] = intrinsic.fx;
  info.K[2] = intrinsic.cx;
  info.K[4] = intrinsic.fy;
  info.K[5] = intrinsic.cy;
  info.K[8] = 1.0;

  info.R.fill(0.0);
  info.R[0] = 1;
  info.R[4] = 1;
  info.R[8] = 1;

  info.P.fill(0.0);
  info.P[0] = info.K[0];
  info.P[2] = info.K[2];
  info.P[5] = info.K[4];
  info.P[6] = info.K[5];
  info.P[10] = 1.0;
  return info;
}

void SaveRgbPointsToPly(std::shared_ptr<dt::Frame> frame, const std::string &fileName) {
  CHECK_NOTNULL(frame.get());
  size_t point_size = frame->GetDataSize() / sizeof(DtColorPoint);
  FILE *fp = fopen(fileName.c_str(), "wb+");
  fprintf(fp, "ply\n");
  CHECK_NOTNULL(fp);
  fprintf(fp, "format ascii 1.0\n");
  fprintf(fp, "element vertex %zu\n", point_size);
  fprintf(fp, "property float x\n");
  fprintf(fp, "property float y\n");
  fprintf(fp, "property float z\n");
  fprintf(fp, "property uchar red\n");
  fprintf(fp, "property uchar green\n");
  fprintf(fp, "property uchar blue\n");
  fprintf(fp, "end_header\n");

  auto *point = (DtColorPoint *)frame->GetData();
  CHECK_NOTNULL(point);
  for (size_t i = 0; i < point_size; i++) {
    fprintf(fp, "%.3f %.3f %.3f %d %d %d\n", point->x, point->y, point->z, (int)point->r,
            (int)point->g, (int)point->b);
    point++;
  }

  fflush(fp);
  fclose(fp);
}

void SaveRgbPointCloudMsgToPly(const sensor_msgs::PointCloud2 &msg, const std::string &fileName) {
  FILE *fp = fopen(fileName.c_str(), "wb+");
  CHECK_NOTNULL(fp);

  sensor_msgs::PointCloud2ConstIterator<float> iter_x(msg, "x");
  sensor_msgs::PointCloud2ConstIterator<float> iter_y(msg, "y");
  sensor_msgs::PointCloud2ConstIterator<float> iter_z(msg, "z");

  // First, count the actual number of valid points
  size_t valid_points = 0;
  for (; iter_x != iter_x.end(); ++iter_x, ++iter_y, ++iter_z) {
    if (!std::isnan(*iter_x) && !std::isnan(*iter_y) && !std::isnan(*iter_z)) {
      ++valid_points;
    }
  }

  // Reset the iterators
  iter_x = sensor_msgs::PointCloud2ConstIterator<float>(msg, "x");
  iter_y = sensor_msgs::PointCloud2ConstIterator<float>(msg, "y");
  iter_z = sensor_msgs::PointCloud2ConstIterator<float>(msg, "z");
  sensor_msgs::PointCloud2ConstIterator<uint8_t> iter_r(msg, "r");
  sensor_msgs::PointCloud2ConstIterator<uint8_t> iter_g(msg, "g");
  sensor_msgs::PointCloud2ConstIterator<uint8_t> iter_b(msg, "b");

  fprintf(fp, "ply\n");
  fprintf(fp, "format ascii 1.0\n");
  fprintf(fp, "element vertex %zu\n", valid_points);
  fprintf(fp, "property float x\n");
  fprintf(fp, "property float y\n");
  fprintf(fp, "property float z\n");
  fprintf(fp, "property uchar red\n");
  fprintf(fp, "property uchar green\n");
  fprintf(fp, "property uchar blue\n");
  fprintf(fp, "end_header\n");

  for (; iter_x != iter_x.end(); ++iter_x, ++iter_y, ++iter_z, ++iter_r, ++iter_g, ++iter_b) {
    if (!std::isnan(*iter_x) && !std::isnan(*iter_y) && !std::isnan(*iter_z)) {
      fprintf(fp, "%.3f %.3f %.3f %d %d %d\n", *iter_x, *iter_y, *iter_z, (int)*iter_r,
              (int)*iter_g, (int)*iter_b);
    }
  }

  fflush(fp);
  fclose(fp);
}

void SaveDepthPointCloudMsgToPly(const sensor_msgs::PointCloud2 &msg, const std::string &fileName) {
  FILE *fp = fopen(fileName.c_str(), "wb+");
  CHECK_NOTNULL(fp);
  sensor_msgs::PointCloud2ConstIterator<float> iter_x(msg, "x");
  sensor_msgs::PointCloud2ConstIterator<float> iter_y(msg, "y");
  sensor_msgs::PointCloud2ConstIterator<float> iter_z(msg, "z");

  // First, count the actual number of valid points
  size_t valid_points = 0;
  for (; iter_x != iter_x.end(); ++iter_x, ++iter_y, ++iter_z) {
    if (!std::isnan(*iter_x) && !std::isnan(*iter_y) && !std::isnan(*iter_z)) {
      ++valid_points;
    }
  }

  // Reset the iterators
  iter_x = sensor_msgs::PointCloud2ConstIterator<float>(msg, "x");
  iter_y = sensor_msgs::PointCloud2ConstIterator<float>(msg, "y");
  iter_z = sensor_msgs::PointCloud2ConstIterator<float>(msg, "z");

  fprintf(fp, "ply\n");
  fprintf(fp, "format ascii 1.0\n");
  fprintf(fp, "element vertex %zu\n", valid_points);
  fprintf(fp, "property float x\n");
  fprintf(fp, "property float y\n");
  fprintf(fp, "property float z\n");
  fprintf(fp, "end_header\n");

  for (; iter_x != iter_x.end(); ++iter_x, ++iter_y, ++iter_z) {
    if (!std::isnan(*iter_x) && !std::isnan(*iter_y) && !std::isnan(*iter_z)) {
      fprintf(fp, "%.3f %.3f %.3f\n", *iter_x, *iter_y, *iter_z);
    }
  }

  fflush(fp);
  fclose(fp);
}

void SavePointsToPly(std::shared_ptr<dt::Frame> frame, const std::string &fileName) {
  size_t point_size = frame->GetDataSize() / sizeof(DtPoint);
  FILE *fp = fopen(fileName.c_str(), "wb+");
  CHECK_NOTNULL(fp);
  fprintf(fp, "ply\n");
  fprintf(fp, "format ascii 1.0\n");
  fprintf(fp, "element vertex %zu\n", point_size);
  fprintf(fp, "property float x\n");
  fprintf(fp, "property float y\n");
  fprintf(fp, "property float z\n");
  fprintf(fp, "end_header\n");

  auto *points = (DtPoint *)frame->GetData();
  CHECK_NOTNULL(points);
  for (size_t i = 0; i < point_size; i++) {
    fprintf(fp, "%.3f %.3f %.3f\n", points->x, points->y, points->z);
    points++;
  }

  fflush(fp);
  fclose(fp);
}

tf2::Quaternion RotationMatrixToQuaternion(const float rotation[9]) {
  Eigen::Matrix3f m;
  // We need to be careful about the order, as RS2 rotation matrix is
  // column-major, while Eigen::Matrix3f expects row-major.
  m << rotation[0], rotation[1], rotation[2], rotation[3], rotation[4], rotation[5], rotation[6],
      rotation[7], rotation[8];
  Eigen::Quaternionf q(m);
  return {q.x(), q.y(), q.z(), q.w()};
}

std::ostream &operator<<(std::ostream &os, const DtCameraParam &rhs) {
  auto depth_intrinsic = rhs.depth_intrinsic;
  auto rgb_intrinsic = rhs.rgb_intrinsic;
  os << "=====depth intrinsic=====\n";
  os << "fx : " << depth_intrinsic.fx << "\n";
  os << "fy : " << depth_intrinsic.fy << "\n";
  os << "cx : " << depth_intrinsic.cx << "\n";
  os << "cy : " << depth_intrinsic.cy << "\n";
  os << "width : " << depth_intrinsic.width << "\n";
  os << "height : " << depth_intrinsic.height << "\n";
  os << "=====rgb intrinsic=====\n";
  os << "fx : " << rgb_intrinsic.fx << "\n";
  os << "fy : " << rgb_intrinsic.fy << "\n";
  os << "cx : " << rgb_intrinsic.cx << "\n";
  os << "cy : " << rgb_intrinsic.cy << "\n";
  os << "width : " << rgb_intrinsic.width << "\n";
  os << "height : " << rgb_intrinsic.height << "\n";
  return os;
}

Extrinsics DtExtrinsicsToMsg(const DtD2CTransform &extrinsics, const std::string &frame_id) {
  Extrinsics msg;
  for (int i = 0; i < 9; ++i) {
    msg.rotation[i] = extrinsics.rot[i];
    if (i < 3) {
      msg.translation[i] = extrinsics.trans[i] / 1000.0;
    }
  }

  msg.header.frame_id = frame_id;
  return msg;
}

ros::Time FromMsToRosTime(uint64_t ms) {
  auto total = static_cast<uint64_t>(ms * 1e6);
  uint64_t sec = total / 1000000000;
  uint64_t nano_sec = total % 1000000000;
  ros::Time stamp(sec, nano_sec);
  return stamp;
}

ros::Time FromUsToRosTime(uint64_t us) {
  auto total = static_cast<uint64_t>(us * 1e3);
  uint64_t sec = total / 1000000000;
  uint64_t nano_sec = total % 1000000000;
  ros::Time stamp(sec, nano_sec);
  return stamp;
}

kDtSampleRate SampleRateFromString(std::string &sample_rate) {
  // covert to lower case
  std::transform(sample_rate.begin(), sample_rate.end(), sample_rate.begin(), ::tolower);
  if (sample_rate == "1.5625hz") {
    return kDtSampleRate15625Hz;
  } else if (sample_rate == "3.125hz") {
    return kDtSampleRate3125Hz;
  } else if (sample_rate == "6.25hz") {
    return kDtSampleRate625Hz;
  } else if (sample_rate == "12.5hz") {
    return kDtSampleRate125Hz;
  } else if (sample_rate == "25hz") {
    return kDtSampleRate25Hz;
  } else if (sample_rate == "50hz") {
    return kDtSampleRate50Hz;
  } else if (sample_rate == "100hz") {
    return kDtSampleRate100Hz;
  } else if (sample_rate == "200hz") {
    return kDtSampleRate200Hz;
  } else if (sample_rate == "400hz") {
    return kDtSampleRate400Hz;
  } else if (sample_rate == "500hz") {
    return kDtSampleRate500Hz;
  } else if (sample_rate == "800hz") {
    return kDtSampleRate800Hz;
  } else if (sample_rate == "1khz") {
    return kDtSampleRate1kHz;
  } else if (sample_rate == "2khz") {
    return kDtSampleRate2kHz;
  } else if (sample_rate == "4khz") {
    return kDtSampleRate4kHz;
  } else if (sample_rate == "8khz") {
    return kDtSampleRate8kHz;
  } else if (sample_rate == "16khz") {
    return kDtSampleRate16kHz;
  } else if (sample_rate == "32khz") {
    return kDtSampleRate32kHz;
  } else {
    ROS_ERROR_STREAM("Unknown kDtSampleRate: " << sample_rate);
    return kDtSampleRate100Hz;
  }
}

std::string SampleRateToString(const kDtSampleRate &sample_rate) {
  switch (sample_rate) {
    case kDtSampleRate15625Hz:
      return "1.5625Hz";
    case kDtSampleRate3125Hz:
      return "3.125Hz";
    case kDtSampleRate625Hz:
      return "6.25Hz";
    case kDtSampleRate125Hz:
      return "12.5Hz";
    case kDtSampleRate25Hz:
      return "25Hz";
    case kDtSampleRate50Hz:
      return "50Hz";
    case kDtSampleRate100Hz:
      return "100Hz";
    case kDtSampleRate200Hz:
      return "200Hz";
    case kDtSampleRate500Hz:
      return "500Hz";
    case kDtSampleRate1kHz:
      return "1kHz";
    case kDtSampleRate2kHz:
      return "2kHz";
    case kDtSampleRate4kHz:
      return "4kHz";
    case kDtSampleRate8kHz:
      return "8kHz";
    case kDtSampleRate16kHz:
      return "16kHz";
    case kDtSampleRate32kHz:
      return "32kHz";
    default:
      return "100Hz";
  }
}

dt_gyro_full_scale_range FullGyroScaleRangeFromString(std::string &full_scale_range) {
  std::transform(full_scale_range.begin(), full_scale_range.end(), full_scale_range.begin(),
                 ::tolower);
  if (full_scale_range == "16dps") {
    return kDtGyroFs16Dps;
  } else if (full_scale_range == "31dps") {
    return kDtGyroFs31Dps;
  } else if (full_scale_range == "62dps") {
    return kDtGyroFs62Dps;
  } else if (full_scale_range == "125dps") {
    return kDtGyroFs125Dps;
  } else if (full_scale_range == "250dps") {
    return kDtGyroFs250Dps;
  } else if (full_scale_range == "400dps") {
    return kDtGyroFs400Dps;
  } else if (full_scale_range == "500dps") {
    return kDtGyroFs500Dps;
  } else if (full_scale_range == "800dps") {
    return kDtGyroFs800Dps;
  } else if (full_scale_range == "1000dps") {
    return kDtGyroFs1000Dps;
  } else if (full_scale_range == "2000dps") {
    return kDtGyroFs2000Dps;
  } else {
    ROS_ERROR_STREAM("Unknown dt_gyro_full_scale_range: " << full_scale_range);
    return kDtGyroFs2000Dps;
  }
}

std::string FullGyroScaleRangeToString(const dt_gyro_full_scale_range &full_scale_range) {
  switch (full_scale_range) {
    case kDtGyroFs16Dps:
      return "16dps";
    case kDtGyroFs31Dps:
      return "31dps";
    case kDtGyroFs62Dps:
      return "62dps";
    case kDtGyroFs125Dps:
      return "125dps";
    case kDtGyroFs250Dps:
      return "250dps";
    case kDtGyroFs400Dps:
      return "400dps";
    case kDtGyroFs500Dps:
      return "500dps";
    case kDtGyroFs800Dps:
      return "800dps";
    case kDtGyroFs1000Dps:
      return "1000dps";
    case kDtGyroFs2000Dps:
      return "2000dps";
    default:
      return "16dps";
  }
}

DtAccelFullScaleRange FullAccelScaleRangeFromString(std::string &full_scale_range) {
  std::transform(full_scale_range.begin(), full_scale_range.end(), full_scale_range.begin(),
                 ::tolower);
  if (full_scale_range == "2g") {
    return kDtAccelFs2g;
  } else if (full_scale_range == "4g") {
    return kDtAccelFs4g;
  } else if (full_scale_range == "8g") {
    return kDtAccelFs8g;
  } else if (full_scale_range == "16g") {
    return kDtAccelFs16g;
  } else if (full_scale_range == "3g") {
    return kDtAccelFs3g;
  } else if (full_scale_range == "6g") {
    return kDtAccelFs6g;
  } else if (full_scale_range == "12g") {
    return kDtAccelFs12g;
  } else if (full_scale_range == "24g") {
    return kDtAccelFs24g;
  } else {
    ROS_ERROR_STREAM("Unknown DT_ACCEL_FULL_SCALE_RANGE: " << full_scale_range);
    return kDtAccelFs16g;
  }
}

std::string FullAccelScaleRangeToString(const DtAccelFullScaleRange &full_scale_range) {
  switch (full_scale_range) {
    case kDtAccelFs2g:
      return "2g";
    case kDtAccelFs4g:
      return "4g";
    case kDtAccelFs8g:
      return "8g";
    case kDtAccelFs16g:
      return "16g";
    case kDtAccelFs3g:
      return "3g";
    case kDtAccelFs6g:
      return "6g";
    case kDtAccelFs12g:
      return "12g";
    case kDtAccelFs24g:
      return "24g";
    default:
      return "2g";
  }
}

bool IsValidJpeg(const std::shared_ptr<dt::ColorFrame> &frame) {
  if (frame->GetDataSize() < 2) {  // Checking both start and end markers, so minimal size is 4
    return false;
  }

  const auto *data = static_cast<const uint8_t *>(frame->GetData());

  // Check for JPEG start marker
  if (data[0] != 0xFF || data[1] != 0xD8) {
    return false;
  }

  return true;
}

std::string FourccToString(const uint32_t fourcc) {
  std::string str;
  str += (fourcc & 0xFF);
  str += ((fourcc >> 8) & 0xFF);
  str += ((fourcc >> 16) & 0xFF);
  str += ((fourcc >> 24) & 0xFF);
  return str;
}

std::string MetaDataTypeToString(const DtFrameMetadataType &meta_data_type) {
  switch (meta_data_type) {
    case DtFrameMetadataType::kDtFrameMetadataTypeTimestamp:
      return "timestamp";
    case DtFrameMetadataType::kDtFrameMetadataTypeSensorTimestamp:
      return "sensor_timestamp";
    case DtFrameMetadataType::kDtFrameMetadataTypeFrameNumber:
      return "frame_number";
    case DtFrameMetadataType::kDtFrameMetadataTypeAutoExposure:
      return "auto_exposure";
    case DtFrameMetadataType::kDtFrameMetadataTypeExposure:
      return "exposure";
    case DtFrameMetadataType::kDtFrameMetadataTypeGain:
      return "gain";
    case DtFrameMetadataType::kDtFrameMetadataTypeAutoWhiteBalance:
      return "auto_white_balance";
    case DtFrameMetadataType::kDtFrameMetadataTypeWhiteBalance:
      return "white_balance";
    case DtFrameMetadataType::kDtFrameMetadataTypeBrightness:
      return "brightness";
    case DtFrameMetadataType::kDtFrameMetadataTypeContrast:
      return "contrast";
    case DtFrameMetadataType::kDtFrameMetadataTypeSaturation:
      return "saturation";
    case DtFrameMetadataType::kDtFrameMetadataTypeSharpness:
      return "sharpness";
    case DtFrameMetadataType::kDtFrameMetadataTypeBacklightCompensation:
      return "backlight_compensation";
    case DtFrameMetadataType::kDtFrameMetadataTypeHue:
      return "hue";
    case DtFrameMetadataType::kDtFrameMetadataTypeGamma:
      return "gamma";
    case DtFrameMetadataType::kDtFrameMetadataTypeActualFrameRate:
      return "actualFrameRate";
    case DtFrameMetadataType::kDtFrameMetadataTypeFrameRate:
      return "frameRate";
    case DtFrameMetadataType::kDtFrameMetadataTypeAeRoiX:
      return "ae_roi_x";
    case DtFrameMetadataType::kDtFrameMetadataTypeAeRoiY:
      return "ae_roi_y";
    case DtFrameMetadataType::kDtFrameMetadataTypeAeRoiWidth:
      return "ae_roi_width";
    case DtFrameMetadataType::kDtFrameMetadataTypeAeRoiHeight:
      return "ae_roi_height";
    case DtFrameMetadataType::kDtFrameMetadataTypeLaserStatus:
      return "laserStatus";
    case DtFrameMetadataType::kDtFrameMetadataTypeTxTemperature:
      return "txTemperature";    
    case DtFrameMetadataType::kDtFrameMetadataTypeTxDriverTemperature:
      return "txDriverTemperature";
    case DtFrameMetadataType::kDtFrameMetadataTypeSocTemperature:
      return "socTemperature";
    case DtFrameMetadataType::kDtFrameMetadataTypeMainBoardTemperature:
      return "mainBoardTemperature";
    case DtFrameMetadataType::kDtFrameMetadataTypeMtofExposure:
      return "mtofExposure";
    case DtFrameMetadataType::kDtFrameMetadataTypeStofExposure:
      return "stofExposure";
    case DtFrameMetadataType::kDtFrameMetadataTypeFrameType:
      return "frameType";
    case DtFrameMetadataType::kDtFrameMetadataTypeFrameContextId:
      return "frameContextId";
    case DtFrameMetadataType::kDtFrameMetadataTypePalmWorkMode:
      return "palmWorkMode";
    case DtFrameMetadataType::kDtFrameMetadataTypeHandType:
      return "handType";
    // case DtFrameMetadataType::DT_FRAME_METADATA_TYPE_CROP_ROI_LEFT:
    //   return "crop_roi_left";
    // case DtFrameMetadataType::DT_FRAME_METADATA_TYPE_CROP_ROI_TOP:
    //   return "crop_roi_top";
    // case DtFrameMetadataType::DT_FRAME_METADATA_TYPE_CROP_ROI_RIGHT:
    //   return "crop_roi_right";
    // case DtFrameMetadataType::DT_FRAME_METADATA_TYPE_CROP_ROI_BOTTOM:
    //   return "crop_roi_bottom";
    default:
      return "unknown";
  }
}

// DtHoleFillingMode HoleFillingModeFromString(const std::string &hole_filling_mode) {
//   if (hole_filling_mode == "FILL_TOP") {
//     return DT_HOLE_FILL_TOP;
//   } else if (hole_filling_mode == "FILL_NEAREST") {
//     return DT_HOLE_FILL_NEAREST;
//   } else if (hole_filling_mode == "FILL_FAREST") {
//     return DT_HOLE_FILL_FAREST;
//   } else {
//     return DT_HOLE_FILL_NEAREST;
//   }
// }

std::ostream &operator<<(std::ostream &os, const DtFormat &rhs) {
  os << DtFormatToString(rhs);
  return os;
}

std::string DtSensorTypeToString(const DtSensorType &type) {
  switch (type) {
    case kDtSensorUnknown:
      return "UNKNOWN";
    case kDtSensorIr:
      return "IR";
    case kDtSensorColor:
      return "COLOR";
    case kDtSensorDepth:
      return "DEPTH";
    case kDtSensorAccel:
      return "ACCEL";
    case kDtSensorGyro:
      return "GYRO";
    case kDtSensorIrLeft:
      return "LEFT_IR";
    case kDtSensorIrRight:
      return "RIGHT_IR";
    case kDtSensorRawPhase:
      return "RAW_PHASE";
    case kDtSensorColorLeft:
      return "LEFT_COLOR";
    case kDtSensorColorRight:
      return "RIGHT_COLOR";
    case kDtSensorPalm:
      return "PALM";
    default:
      return "UNKNOWN";
  }
}
std::ostream &operator<<(std::ostream &os, const DtSensorType &rhs) {
  os << DtSensorTypeToString(rhs);
  return os;
}

DtStreamType DtStreamTypeFromString(const std::string &stream_type) {
  std::string upper_stream_type = stream_type;
  std::transform(upper_stream_type.begin(), upper_stream_type.end(), upper_stream_type.begin(),
                 ::toupper);
  if (upper_stream_type == "VIDEO") {
    return kDtStreamVideo;
  } else if (upper_stream_type == "IR") {
    return kDtStreamIr;
  } else if (upper_stream_type == "COLOR") {
    return kDtStreamColor;
  } else if (upper_stream_type == "DEPTH") {
    return kDtStreamDepth;
  } else if (upper_stream_type == "ACCEL") {
    return kDtStreamAccel;
  } else if (upper_stream_type == "GYRO") {
    return kDtStreamGyro;
  } else if (upper_stream_type == "IR_LEFT") {
    return kDtStreamIrLeft;
  } else if (upper_stream_type == "IR_RIGHT") {
    return kDtStreamIrRight;
  } else if (upper_stream_type == "RAW_PHASE") {
    return kDtStreamRawPhase;
  } else if (upper_stream_type == "LEFT_COLOR") {
    return kDtStreamColorLeft;
  } else if (upper_stream_type == "RIGHT_COLOR") {
    return kDtStreamColorRight;
  } else if (upper_stream_type == "PALM") {
    return kDtStreamPalm;
  } else {
    return kDtStreamUnknown;
  }
}
std::string GetDistortionModels(DtCameraDistortion distortion) {
  switch (distortion.model) {
    case kDtDistortionNone:
      return sensor_msgs::distortion_models::PLUMB_BOB;
    case kDtDistortionModifiedBrownConrady:
      return sensor_msgs::distortion_models::PLUMB_BOB;
    case kDtDistortionInverseBrownConrady:
      return sensor_msgs::distortion_models::PLUMB_BOB;
    case kDtDistortionBrownConrady:
      return sensor_msgs::distortion_models::PLUMB_BOB;
    case kDtDistortionBrownConradyK6:
      return sensor_msgs::distortion_models::PLUMB_BOB;
    case kDtDistortionKannalaBrandt4:
      return sensor_msgs::distortion_models::EQUIDISTANT;
    default:
      return sensor_msgs::distortion_models::PLUMB_BOB;
  }
}

bool IsNebula410Pid(const char *pid) {
  const char *NEBULA_410_PID = "0x4410";
  return strcmp(pid, NEBULA_410_PID) == 0;
}

bool IsNebulaSeriesPid(const char *pid) {
  const char *NEBULA_410_PID = "0x4410";
  const char *NEBULA_280_PID = "0x4480";
  return (strcmp(pid, NEBULA_410_PID) == 0) || (strcmp(pid, NEBULA_280_PID) == 0);
}

bool IsNebula280Pid(const char *pid) {
  const char *NEBULA_280_PID = "0x4480";
  return strcmp(pid, NEBULA_280_PID) == 0;
}

bool IsPisca1000Pid(const char *pid) {
  const char *PISCA_1000_PID = "0x5100";
  return strcmp(pid, PISCA_1000_PID) == 0;
}

}  // namespace deptrum_camera
