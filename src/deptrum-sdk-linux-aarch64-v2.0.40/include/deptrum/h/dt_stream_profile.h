// Copyright (c) Deptrum Inc. All Rights Reserved.

/**
 * @file dt_stream_profile.h
 * @brief Defines stream profile types and operations for retrieving stream characteristics
 *        including dimensions, frame rates, and data formats.
 */

#ifndef DEPTRUM_H_DT_STREAM_PROFILE_H_
#define DEPTRUM_H_DT_STREAM_PROFILE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "dt_types.h"

/**
 * @brief Creates a generic stream profile object.
 *
 * @param[in] type Stream type identifier.
 * @param[in] format Stream data format.
 * @param[out] error Receives detailed error information if operation fails.
 * @return dt_stream_profile* New stream profile instance.
 */
DT_EXPORT dt_stream_profile* dt_create_stream_profile(dt_stream_type type,
                                                      dt_format format,
                                                      dt_error** error);

/**
 * @brief Creates a video-specific stream profile.
 *
 * @param[in] type Video stream type identifier.
 * @param[in] format Video pixel format.
 * @param[in] width Frame width in pixels.
 * @param[in] height Frame height in pixels.
 * @param[in] fps Frames per second.
 * @param[out] error Receives detailed error information if operation fails.
 * @return dt_stream_profile* New video stream profile instance.
 */
DT_EXPORT dt_stream_profile* dt_create_video_stream_profile(dt_stream_type type,
                                                            dt_format format,
                                                            uint32_t width,
                                                            uint32_t height,
                                                            uint32_t fps,
                                                            dt_error** error);

/**
 * @brief Creates an accelerometer-specific stream profile.
 *
 * @param[in] full_scale_range Measurement range in g-units.
 * @param[in] sample_rate Data sampling rate in Hz.
 * @param[out] error Receives detailed error information if operation fails.
 * @return dt_stream_profile* New accelerometer stream profile instance.
 */
DT_EXPORT dt_stream_profile* dt_create_accel_stream_profile(
    dt_accel_full_scale_range full_scale_range,
    dt_accel_sample_rate sample_rate,
    dt_error** error);

/**
 * @brief Creates a gyroscope-specific stream profile.
 *
 * @param[in] full_scale_range Angular measurement range in degrees per second.
 * @param[in] sample_rate Data sampling rate in Hz.
 * @param[out] error Receives detailed error information if operation fails.
 * @return dt_stream_profile* New gyroscope stream profile instance.
 */
DT_EXPORT dt_stream_profile* dt_create_gyro_stream_profile(
    dt_gyro_full_scale_range full_scale_range,
    dt_gyro_sample_rate sample_rate,
    dt_error** error);

/**
 * @brief Creates a palm-specific stream profile.
 *
 * @param[out] error Receives detailed error information if operation fails.
 * @return dt_stream_profile* New palm stream profile instance.
 */
DT_EXPORT dt_stream_profile* dt_create_palm_stream_profile(dt_error** error);

/**
 * @brief Creates a stream profile by copying an existing profile.
 *
 * @param[in] src_profile Source profile to duplicate.
 * @param[out] error Receives detailed error information if operation fails.
 * @return dt_stream_profile* Cloned stream profile instance.
 */
DT_EXPORT dt_stream_profile* dt_create_stream_profile_from_other_stream_profile(
    const dt_stream_profile* src_profile,
    dt_error** error);

/**
 * @brief Creates a new stream profile by modifying the format of an existing profile.
 *
 * @param[in] profile Source profile to clone.
 * @param[in] new_format Target data format for new profile.
 * @param[out] error Receives detailed error information if operation fails.
 * @return dt_stream_profile* New profile with updated format.
 */
DT_EXPORT dt_stream_profile* dt_create_stream_profile_with_new_format(
    const dt_stream_profile* profile,
    dt_format new_format,
    dt_error** error);

/**
 * @brief Releases resources associated with a stream profile.
 *
 * @param[in] profile Target profile to destroy.
 */
DT_EXPORT void dt_delete_stream_profile(const dt_stream_profile* profile);

/**
 * @brief Retrieves the data format of a stream profile.
 *
 * @param[in] profile Target stream profile.
 * @param[out] error Receives detailed error information if operation fails.
 * @return dt_format Current data format.
 */
DT_EXPORT dt_format dt_stream_profile_get_format(const dt_stream_profile* profile,
                                                 dt_error** error);

/**
 * @brief Updates the data format of a stream profile.
 *
 * @param[in] profile Target stream profile.
 * @param[in] format New data format.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_stream_profile_set_format(dt_stream_profile* profile,
                                            dt_format format,
                                            dt_error** error);

/**
 * @brief Retrieves the stream type identifier.
 *
 * @param[in] profile Target stream profile.
 * @param[out] error Receives detailed error information if operation fails.
 * @return dt_stream_type Current stream type.
 */
DT_EXPORT dt_stream_type dt_stream_profile_get_type(const dt_stream_profile* profile,
                                                    dt_error** error);

/**
 * @brief Updates the stream type identifier.
 *
 * @param[in] profile Target stream profile.
 * @param[in] type New stream type.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_stream_profile_set_type(const dt_stream_profile* profile,
                                          dt_stream_type type,
                                          dt_error** error);

/**
 * @brief Retrieves the extrinsic transformation between two sensor streams.
 *
 * @param[in] source Source stream profile.
 * @param[in] target Target stream profile.
 * @param[out] error Receives detailed error information if operation fails.
 * @return dt_extrinsic Transform matrix from source to target.
 */
DT_EXPORT dt_extrinsic dt_stream_profile_get_extrinsic_to(const dt_stream_profile* source,
                                                          dt_stream_profile* target,
                                                          dt_error** error);

/**
 * @brief Configures the extrinsic transformation between two sensor streams.
 *
 * @param[in] source Source stream profile.
 * @param[in] target Target stream profile.
 * @param[in] extrinsic Transform matrix from source to target.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_stream_profile_set_extrinsic_to(dt_stream_profile* source,
                                                  const dt_stream_profile* target,
                                                  dt_extrinsic extrinsic,
                                                  dt_error** error);

/**
 * @brief Configures the extrinsic transformation to a stream type.
 *
 * @param[in] source Source stream profile.
 * @param[in] type Target stream type identifier.
 * @param[in] extrinsic Transform matrix to target type.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_stream_profile_set_extrinsic_to_type(dt_stream_profile* source,
                                                       const dt_stream_type type,
                                                       dt_extrinsic extrinsic,
                                                       dt_error** error);

/**
 * @brief Retrieves frame rate for video streams.
 *
 * @param[in] profile Target video stream profile.
 * @param[out] error Receives detailed error information if operation fails.
 * @return uint32_t Current frames per second.
 */
DT_EXPORT uint32_t dt_video_stream_profile_get_fps(const dt_stream_profile* profile,
                                                   dt_error** error);

/**
 * @brief Retrieves frame width for video streams.
 *
 * @param[in] profile Target video stream profile.
 * @param[out] error Receives detailed error information if operation fails.
 * @return uint32_t Current frame width in pixels.
 */
DT_EXPORT uint32_t dt_video_stream_profile_get_width(const dt_stream_profile* profile,
                                                     dt_error** error);

/**
 * @brief Updates frame width for video streams.
 *
 * @param[in] profile Target video stream profile.
 * @param[in] width New frame width in pixels.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_video_stream_profile_set_width(dt_stream_profile* profile,
                                                 uint32_t width,
                                                 dt_error** error);

/**
 * @brief Retrieves frame height for video streams.
 *
 * @param[in] profile Target video stream profile.
 * @param[out] error Receives detailed error information if operation fails.
 * @return uint32_t Current frame height in pixels.
 */
DT_EXPORT uint32_t dt_video_stream_profile_get_height(const dt_stream_profile* profile,
                                                      dt_error** error);

/**
 * @brief Updates frame height for video streams.
 *
 * @param[in] profile Target video stream profile.
 * @param[in] height New frame height in pixels.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_video_stream_profile_set_height(dt_stream_profile* profile,
                                                  uint32_t height,
                                                  dt_error** error);

/**
 * @brief Retrieves camera intrinsic parameters.
 *
 * @param[in] profile Target video stream profile.
 * @param[out] error Receives detailed error information if operation fails.
 * @return dt_camera_intrinsic Camera intrinsic matrix.
 */
DT_EXPORT dt_camera_intrinsic
dt_video_stream_profile_get_intrinsic(const dt_stream_profile* profile, dt_error** error);

/**
 * @brief Configures camera intrinsic parameters.
 *
 * @param[in] profile Target video stream profile.
 * @param[in] intrinsic New intrinsic matrix.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_video_stream_profile_set_intrinsic(dt_stream_profile* profile,
                                                     dt_camera_intrinsic intrinsic,
                                                     dt_error** error);

/**
 * @brief Retrieves lens distortion parameters.
 *
 * @param[in] profile Target video stream profile.
 * @param[out] error Receives detailed error information if operation fails.
 * @return dt_camera_distortion Distortion coefficients.
 */
DT_EXPORT dt_camera_distortion
dt_video_stream_profile_get_distortion(const dt_stream_profile* profile, dt_error** error);

/**
 * @brief Configures lens distortion parameters.
 *
 * @param[in] profile Target video stream profile.
 * @param[in] distortion New distortion coefficients.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_video_stream_profile_set_distortion(dt_stream_profile* profile,
                                                      dt_camera_distortion distortion,
                                                      dt_error** error);

/**
 * @brief Retrieves full-scale measurement range.
 *
 * @param[in] profile Target accelerometer stream profile.
 * @param[out] error Receives detailed error information if operation fails.
 * @return dt_accel_full_scale_range Current measurement range.
 */
DT_EXPORT dt_accel_full_scale_range
dt_accel_stream_profile_get_full_scale_range(const dt_stream_profile* profile, dt_error** error);

/**
 * @brief Retrieves data sampling rate.
 *
 * @param[in] profile Target accelerometer stream profile.
 * @param[out] error Receives detailed error information if operation fails.
 * @return dt_accel_sample_rate Current sampling frequency (Hz).
 */
DT_EXPORT dt_accel_sample_rate
dt_accel_stream_profile_get_sample_rate(const dt_stream_profile* profile, dt_error** error);

/**
 * @brief Retrieves sensor intrinsic calibration.
 *
 * @param[in] profile Target accelerometer stream profile.
 * @param[out] error Receives detailed error information if operation fails.
 * @return dt_accel_intrinsic Sensor calibration parameters.
 */
DT_EXPORT dt_accel_intrinsic dt_accel_stream_profile_get_intrinsic(const dt_stream_profile* profile,
                                                                   dt_error** error);

/**
 * @brief Configures sensor intrinsic calibration.
 *
 * @param[in] profile Target accelerometer stream profile.
 * @param[in] intrinsic New calibration parameters.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_accel_stream_profile_set_intrinsic(dt_stream_profile* profile,
                                                     dt_accel_intrinsic intrinsic,
                                                     dt_error** error);

/**
 * @brief Retrieves full-scale angular range.
 *
 * @param[in] profile Target gyroscope stream profile.
 * @param[out] error Receives detailed error information if operation fails.
 * @return dt_gyro_full_scale_range Current angular range.
 */
DT_EXPORT dt_gyro_full_scale_range
dt_gyro_stream_profile_get_full_scale_range(const dt_stream_profile* profile, dt_error** error);

/**
 * @brief Retrieves data sampling rate.
 *
 * @param[in] profile Target gyroscope stream profile.
 * @param[out] error Receives detailed error information if operation fails.
 * @return dt_gyro_sample_rate Current sampling frequency (Hz).
 */
DT_EXPORT dt_gyro_sample_rate
dt_gyro_stream_profile_get_sample_rate(const dt_stream_profile* profile, dt_error** error);

/**
 * @brief Retrieves sensor intrinsic calibration.
 *
 * @param[in] profile Target gyroscope stream profile.
 * @param[out] error Receives detailed error information if operation fails.
 * @return dt_gyro_intrinsic Sensor calibration parameters.
 */
DT_EXPORT dt_gyro_intrinsic dt_gyro_stream_get_intrinsic(const dt_stream_profile* profile,
                                                         dt_error** error);

/**
 * @brief Configures sensor intrinsic calibration.
 *
 * @param[in] profile Target gyroscope stream profile.
 * @param[in] intrinsic New calibration parameters.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_gyro_stream_set_intrinsic(dt_stream_profile* profile,
                                            dt_gyro_intrinsic intrinsic,
                                            dt_error** error);

/**
 * @brief Retrieves number of profiles in a list.
 *
 * @param[in] profile_list Target profile list.
 * @param[out] error Receives detailed error information if operation fails.
 * @return uint32_t Profile count.
 */
DT_EXPORT uint32_t dt_stream_profile_list_get_count(const dt_stream_profile_list* profile_list,
                                                    dt_error** error);

/**
 * @brief Retrieves profile by index.
 *
 * @note Caller must release the returned profile using dt_delete_stream_profile().
 *
 * @param[in] profile_list Target profile list.
 * @param[in] index Zero-based profile index.
 * @param[out] error Receives detailed error information if operation fails.
 * @return dt_stream_profile* Retrieved profile instance.
 */
DT_EXPORT dt_stream_profile* dt_stream_profile_list_get_profile(
    const dt_stream_profile_list* profile_list,
    int index,
    dt_error** error);

/**
 * @brief Finds matching video profile from a list.
 *
 * @note Caller must release the returned profile using dt_delete_stream_profile().
 *
 * @param[in] profile_list Target profile list.
 * @param[in] width Target width (kDtWidthAny for wildcard).
 * @param[in] height Target height (kDtHeightAny for wildcard).
 * @param[in] format Target format (kDtFormatAny for wildcard).
 * @param[in] fps Target frame rate (kDtFpsAny for wildcard).
 * @param[out] error Receives detailed error information if operation fails.
 * @return dt_stream_profile* First matching profile found.
 */
DT_EXPORT dt_stream_profile* dt_stream_profile_list_get_video_stream_profile(
    const dt_stream_profile_list* profile_list,
    int width,
    int height,
    dt_format format,
    int fps,
    dt_error** error);

/**
 * @brief Finds matching accelerometer profile from a list.
 *
 * @note Caller must release the returned profile using dt_delete_stream_profile().
 *
 * @param[in] profile_list Target profile list.
 * @param[in] full_scale_range Target range (0 for wildcard).
 * @param[in] sample_rate Target sample rate (0 for wildcard).
 * @param[out] error Receives detailed error information if operation fails.
 * @return dt_stream_profile* First matching profile found.
 */
DT_EXPORT dt_stream_profile* dt_stream_profile_list_get_accel_stream_profile(
    const dt_stream_profile_list* profile_list,
    dt_accel_full_scale_range full_scale_range,
    dt_accel_sample_rate sample_rate,
    dt_error** error);

/**
 * @brief Finds matching gyroscope profile from a list.
 *
 * @note Caller must release the returned profile using dt_delete_stream_profile().
 *
 * @param[in] profile_list Target profile list.
 * @param[in] full_scale_range Target range (0 for wildcard).
 * @param[in] sample_rate Target sample rate (0 for wildcard).
 * @param[out] error Receives detailed error information if operation fails.
 * @return dt_stream_profile* First matching profile found.
 */
DT_EXPORT dt_stream_profile* dt_stream_profile_list_get_gyro_stream_profile(
    const dt_stream_profile_list* profile_list,
    dt_gyro_full_scale_range full_scale_range,
    dt_gyro_sample_rate sample_rate,
    dt_error** error);

/**
 * @brief Finds matching palm profile from a list.
 *
 * @note Caller must release the returned profile using dt_delete_stream_profile().
 *
 * @param[in] profile_list Target profile list.
 * @param[out] error Receives detailed error information if operation fails.
 * @return dt_stream_profile* First matching profile found.
 */
DT_EXPORT dt_stream_profile* dt_stream_profile_list_get_palm_stream_profile(
    const dt_stream_profile_list* profile_list,
    dt_error** error);

/**
 * @brief Releases a stream profile list and all contained profiles.
 *
 * @param[in] profile_list Target list to destroy.
 */
DT_EXPORT void dt_delete_stream_profile_list(const dt_stream_profile_list* profile_list);

#ifdef __cplusplus
}
#endif

#endif  // DEPTRUM_H_DT_STREAM_PROFILE_H_