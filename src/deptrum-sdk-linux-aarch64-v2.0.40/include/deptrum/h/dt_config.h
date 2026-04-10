// Copyright (c) Deptrum Inc. All Rights Reserved.

/**
 * @file dt_config.h
 * @brief Manages pipeline configuration including stream selection, device filtering,
 *        and pre-streaming device configuration.
 */
#ifndef DEPTRUM_H_DT_CONFIG_H_
#define DEPTRUM_H_DT_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "dt_types.h"

/**
 * @brief Creates a pipeline configuration instance.
 *
 * @details Provides stream filtering and device validation capabilities. When absent,
 *          pipeline auto-resolves devices. Allows device selection and pre-configuration.
 *
 * @param[out] error Receives detailed error information if operation fails.
 * @return Pointer to new configuration instance. Returns NULL on failure.
 */
DT_EXPORT dt_config* dt_create_config(dt_error** error);

/**
 * @brief Releases configuration resources.
 *
 * @param[in] config Target configuration instance to destroy.
 *             If NULL, no operation is performed.
 */
DT_EXPORT void dt_delete_config(dt_config* config);

/**
 * @brief Retrieves enabled stream profiles from configuration.
 *
 * @param[in] config Initialized configuration instance.
 * @param[out] error Receives detailed error information if operation fails.
 * @return Pointer to stream profile list. Must be released by caller using
 *         @ref dt_delete_stream_profile_list. Returns NULL on failure.
 */
DT_EXPORT dt_stream_profile_list* dt_config_get_enabled_stream_profile_list(const dt_config* config,
                                                                            dt_error** error);

/**
 * @brief Enables all streams defined in pipeline configuration.
 *
 * @param[in] config Initialized configuration instance.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_config_enable_all_stream(dt_config* config, dt_error** error);

/**
 * @brief Enables a stream with default profile settings.
 *
 * @param[in] config Initialized configuration instance.
 * @param[in] stream_type Target stream type identifier.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_config_enable_stream(dt_config* config,
                                       dt_stream_type stream_type,
                                       dt_error** error);

/**
 * @brief Enables a stream with specific profile parameters.
 *
 * @param[in] config Initialized configuration instance.
 * @param[in] profile Stream profile containing configuration parameters.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_config_enable_stream_with_stream_profile(dt_config* config,
                                                           const dt_stream_profile* profile,
                                                           dt_error** error);

/**
 * @brief Enables a video stream with explicit parameters.
 *
 * @attention Restricted to video stream types (kDtStreamIr, kDtStreamColor, etc.).
 *
 * @param[in] config Initialized configuration instance.
 * @param[in] stream_type Video stream type identifier.
 * @param[in] width Frame width in pixels.
 * @param[in] height Frame height in pixels.
 * @param[in] fps Frame rate in frames per second.
 * @param[in] format Pixel data format.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_config_enable_video_stream(dt_config* config,
                                             dt_stream_type stream_type,
                                             uint32_t width,
                                             uint32_t height,
                                             uint32_t fps,
                                             dt_format format,
                                             dt_error** error);

/**
 * @brief Enables accelerometer stream with specified parameters.
 *
 * @param[in] config Initialized configuration instance.
 * @param[in] full_scale_range Accelerometer measurement range.
 * @param[in] sample_rate Sampling frequency in Hz.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_config_enable_accel_stream(dt_config* config,
                                             dt_accel_full_scale_range full_scale_range,
                                             dt_accel_sample_rate sample_rate,
                                             dt_error** error);

/**
 * @brief Enables gyroscope stream with specified parameters.
 *
 * @param[in] config Initialized configuration instance.
 * @param[in] full_scale_range Gyroscope measurement range.
 * @param[in] sample_rate Sampling frequency in Hz.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_config_enable_gyro_stream(dt_config* config,
                                            dt_gyro_full_scale_range full_scale_range,
                                            dt_gyro_sample_rate sample_rate,
                                            dt_error** error);

/**
 * @brief Enables palm stream with specified parameters.
 *
 * @param[in] config Initialized configuration instance.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_config_enable_palm_stream(dt_config* config, dt_error** error);

/**
 * @brief Disables all streams in configuration.
 *
 * @param[in] config Initialized configuration instance.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_config_disable_all_stream(dt_config* config, dt_error** error);

/**
 * @brief Disables specific stream type in configuration.
 *
 * @param[in] config Initialized configuration instance.
 * @param[in] type Stream type identifier to disable.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_config_disable_stream(dt_config* config, dt_stream_type type, dt_error** error);

#ifdef __cplusplus
}
#endif

#endif  // DEPTRUM_H_DT_CONFIG_H_