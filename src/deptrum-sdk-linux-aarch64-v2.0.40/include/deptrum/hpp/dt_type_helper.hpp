// Copyright (c) Deptrum Inc. All Rights Reserved.

/**
 * @file dt_type_helper.hpp
 * @brief Helper functions for converting type enumerations to strings.
 */

#ifndef DEPTRUM_HPP_DT_TYPE_HELPER_HPP_
#define DEPTRUM_HPP_DT_TYPE_HELPER_HPP_

#include <functional>
#include <iostream>
#include <string>
#include "deptrum/h/dt_type_helper.h"
#include "deptrum/h/dt_types.h"

namespace dt {

/**
 * @class TypeHelper
 * @brief Provides utilities for converting enumerated types to strings and checking type
 * categories.
 */
class TypeHelper {
 public:
  /**
   * @brief Converts a sensor type to its corresponding stream type.
   * @param[in] sensor_type Sensor type to convert.
   * @return Corresponding stream type enumeration value.
   */
  static DtStreamType ConvertSensorTypeToStreamType(DtSensorType sensor_type) {
    return dt_sensor_type_to_stream_type(sensor_type);
  }

  /**
   * @brief Checks if a sensor type is classified as a video sensor.
   * @param[in] type Sensor type to check.
   * @return True if the sensor is a video sensor, false otherwise.
   */
  static bool IsVideoSensorType(DtSensorType type) { return DT_IS_VIDEO_SENSOR_TYPE(type); }

  /**
   * @brief Checks if a stream type is classified as a video stream.
   * @param[in] type Stream type to check.
   * @return True if the stream is a video stream, false otherwise.
   */
  static bool IsVideoStreamType(DtStreamType type) { return DT_IS_VIDEO_STREAM_TYPE(type); }
};

}  // namespace dt

inline std::ostream& operator<<(std::ostream& o, DtCameraInfo camera_info) {
  return o << dt_camera_info_to_string(camera_info);
}

inline std::ostream& operator<<(std::ostream& o, DtBreathMode breath_mode) {
  return o << dt_breath_mode_to_string(breath_mode);
}

inline std::ostream& operator<<(std::ostream& o, DtDimPalmMode dim_palm_mode) {
  return o << dt_dim_palm_mode_to_string(dim_palm_mode);
}

inline std::ostream& operator<<(std::ostream& o, DtStatus status) {
  return o << dt_status_to_string(status);
}

inline std::ostream& operator<<(std::ostream& o, DtLogSeverity severity) {
  return o << dt_log_severity_to_string(severity);
}

inline std::ostream& operator<<(std::ostream& o, DtSensorType sensor_type) {
  return o << dt_sensor_type_to_string(sensor_type);
}

inline std::ostream& operator<<(std::ostream& o, DtStreamType stream_type) {
  return o << dt_stream_type_to_string(stream_type);
}

inline std::ostream& operator<<(std::ostream& o, DtFrameType frame_type) {
  return o << dt_frame_type_to_string(frame_type);
}

inline std::ostream& operator<<(std::ostream& o, DtFormat format) {
  return o << dt_format_to_string(format);
}

inline std::ostream& operator<<(std::ostream& o, DtConvertFormat convert_format) {
  return o << dt_convert_format_to_string(convert_format);
}

inline std::ostream& operator<<(std::ostream& o, DtImuSampleRate imu_sample_rate) {
  return o << dt_imu_sample_rate_to_string(imu_sample_rate);
}

inline std::ostream& operator<<(std::ostream& o, DtGyroFullScaleRange gyro_full_scale_range) {
  return o << dt_gyro_full_scale_range_to_string(gyro_full_scale_range);
}

inline std::ostream& operator<<(std::ostream& o, DtAccelFullScaleRange accel_full_scale_range) {
  return o << dt_accel_full_scale_range_to_string(accel_full_scale_range);
}

inline std::ostream& operator<<(std::ostream& o, DtMediaType media_type) {
  return o << dt_media_type_to_string(media_type);
}

inline std::ostream& operator<<(std::ostream& o, DtMediaState media_state) {
  return o << dt_media_state_to_string(media_state);
}

inline std::ostream& operator<<(std::ostream& o, DtFrameMetadataType frame_metadata_type) {
  return o << dt_frame_metadata_type_to_string(frame_metadata_type);
}

inline std::ostream& operator<<(std::ostream& o, DtPlaybackStatus playback_status) {
  return o << dt_playback_status_to_string(playback_status);
}

inline std::ostream& operator<<(std::ostream& o, DtExceptionType exception_type) {
  return o << dt_exception_type_to_string(exception_type);
}

inline std::ostream& operator<<(std::ostream& o, DtOptionId option_id) {
  return o << dt_option_id_to_string(option_id);
}

inline std::ostream& operator<<(std::ostream& o, DtPermissionType permission_type) {
  return o << dt_permission_type_to_string(permission_type);
}

inline std::ostream& operator<<(std::ostream& o, DtOptionType option_type) {
  return o << dt_option_type_to_string(option_type);
}

#endif  // DEPTRUM_HPP_DT_TYPE_HELPER_HPP_
