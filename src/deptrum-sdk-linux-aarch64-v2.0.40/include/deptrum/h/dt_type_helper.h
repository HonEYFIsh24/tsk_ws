// Copyright (c) Deptrum Inc. All Rights Reserved.

/**
 * @file dt_type_helper.h
 * @brief Helper functions for converting type enumerations to strings.
 */

#ifndef DEPTRUM_H_DT_TYPE_HELPER_H_
#define DEPTRUM_H_DT_TYPE_HELPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "dt_option.h"
#include "dt_types.h"


/**
 * @brief Converts a camera info enumeration to its string representation.
 *
 * @param[in] camera_info Camera info enumeration value.
 * @return const char* String representation of the camera info enumeration.
 */
DT_EXPORT const char* dt_camera_info_to_string(dt_camera_info camera_info);

/**
 * @brief Converts a breath mode enumeration to its string representation.
 *
 * @param[in] breath_mode Breath mode enumeration value.
 * @return const char* String representation of the breath mode enumeration.
 */
DT_EXPORT const char* dt_breath_mode_to_string(dt_breath_mode breath_mode);

/**
 * @brief Converts a dim palm mode enumeration to its string representation.
 *
 * @param[in] dim_palm_mode Dim palm mode enumeration value.
 * @return const char* String representation of the dim palm mode enumeration.
 */
DT_EXPORT const char* dt_dim_palm_mode_to_string(dt_dim_palm_mode dim_palm_mode);

/**
 * @brief Converts a status enumeration to its string representation.
 *
 * @param[in] status Status enumeration value.
 * @return const char* String representation of the status enumeration.
 */
DT_EXPORT const char* dt_status_to_string(dt_status status);

/**
 * @brief Converts a log severity enumeration to its string representation.
 *
 * @param[in] log_severity Log severity enumeration value.
 * @return const char* String representation of the log severity enumeration.
 */
DT_EXPORT const char* dt_log_severity_to_string(dt_log_severity log_severity);

/**
 * @brief Converts a sensor type enumeration to its string representation.
 *
 * @param[in] sensor_type Sensor type enumeration value.
 * @return const char* String representation of the sensor type enumeration.
 */
DT_EXPORT const char* dt_sensor_type_to_string(dt_sensor_type sensor_type);

/**
 * @brief Converts a stream type enumeration to its string representation.
 *
 * @param[in] stream_type Stream type enumeration value.
 * @return const char* String representation of the stream type enumeration.
 */
DT_EXPORT const char* dt_stream_type_to_string(dt_stream_type stream_type);

/**
 * @brief Converts a frame type enumeration to its string representation.
 *
 * @param[in] frame_type Frame type enumeration value.
 * @return const char* String representation of the frame type enumeration.
 */
DT_EXPORT const char* dt_frame_type_to_string(dt_frame_type frame_type);

/**
 * @brief Converts a pixel format enumeration to its string representation.
 *
 * @param[in] format Pixel format enumeration value.
 * @return const char* String representation of the pixel format enumeration.
 */
DT_EXPORT const char* dt_format_to_string(dt_format format);

/**
 * @brief Converts a convert format enumeration to its string representation.
 *
 * @param[in] convert_format Convert format enumeration value.
 * @return const char* String representation of the convert format enumeration.
 */
DT_EXPORT const char* dt_convert_format_to_string(dt_convert_format convert_format);

/**
 * @brief Converts an IMU sample rate enumeration to its string representation.
 *
 * @param[in] imu_sample_rate IMU sample rate enumeration value.
 * @return const char* String representation of the IMU sample rate enumeration.
 */
DT_EXPORT const char* dt_imu_sample_rate_to_string(dt_imu_sample_rate imu_sample_rate);

/**
 * @brief Converts a gyroscope full scale range enumeration to its string representation.
 *
 * @param[in] gyro_full_scale_range Gyroscope full scale range enumeration value.
 * @return const char* String representation of the gyroscope full scale range enumeration.
 */
DT_EXPORT const char* dt_gyro_full_scale_range_to_string(
    dt_gyro_full_scale_range gyro_full_scale_range);

/**
 * @brief Converts an accelerometer full scale range enumeration to its string representation.
 *
 * @param[in] accel_full_scale_range Accelerometer full scale range enumeration value.
 * @return const char* String representation of the accelerometer full scale range enumeration.
 */
DT_EXPORT const char* dt_accel_full_scale_range_to_string(
    dt_accel_full_scale_range accel_full_scale_range);

/**
 * @brief Converts a media type enumeration to its string representation.
 *
 * @param[in] media_type Media type enumeration value.
 * @return const char* String representation of the media type enumeration.
 */
DT_EXPORT const char* dt_media_type_to_string(dt_media_type media_type);

/**
 * @brief Converts a media state enumeration to its string representation.
 *
 * @param[in] media_state Media state enumeration value.
 * @return const char* String representation of the media state enumeration.
 */
DT_EXPORT const char* dt_media_state_to_string(dt_media_state media_state);

/**
 * @brief Converts a frame metadata type enumeration to its string representation.
 *
 * @param[in] frame_metadata_type Frame metadata type enumeration value.
 * @return const char* String representation of the frame metadata type enumeration.
 */
DT_EXPORT const char* dt_frame_metadata_type_to_string(dt_frame_metadata_type frame_metadata_type);

/**
 * @brief Converts an exception type enumeration to its string representation.
 *
 * @param[in] exception_type Exception type enumeration value.
 * @return const char* String representation of the exception type enumeration.
 */
DT_EXPORT const char* dt_exception_type_to_string(dt_exception_type exception_type);

/**
 * @brief Converts an option ID enumeration to its string representation.
 *
 * @param[in] option_id Option ID enumeration value.
 * @return const char* String representation of the option ID enumeration.
 */
DT_EXPORT const char* dt_option_id_to_string(dt_option_id option_id);

/**
 * @brief Converts a permission type enumeration to its string representation.
 *
 * @param[in] permission_type Permission type enumeration value.
 * @return const char* String representation of the permission type enumeration.
 */
DT_EXPORT const char* dt_permission_type_to_string(dt_permission_type permission_type);

/**
 * @brief Converts an option type enumeration to its string representation.
 *
 * @param[in] option_type Option type enumeration value.
 * @return const char* String representation of the option type enumeration.
 */
DT_EXPORT const char* dt_option_type_to_string(dt_option_type option_type);

/**
 * @brief Converts a sensor type enumeration to its corresponding stream type.
 *
 * @param[in] sensor_type Source sensor type enumeration.
 * @return DtStreamType Corresponding stream type enumeration.
 */
DT_EXPORT DtStreamType dt_sensor_type_to_stream_type(dt_sensor_type sensor_type);

#ifdef __cplusplus
}
#endif

#endif  // DEPTRUM_H_DT_TYPE_HELPER_H_
