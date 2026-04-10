// Copyright (c) Deptrum Inc. All Rights Reserved.

/**
 * @file dt_pipeline.h
 * @brief Unified interface for device control and computer vision processing.
 *
 * Abstracts camera configuration, streaming, and vision module orchestration.
 * Manages processing blocks that implement computer vision algorithms.
 * Applications consume vision outputs while the pipeline handles threading and synchronization.
 */
#ifndef DEPTRUM_H_DT_PIPELINE_H_
#define DEPTRUM_H_DT_PIPELINE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "dt_types.h"

/**
 * @brief Creates a pipeline instance with default configuration.
 *
 * Allocates resources for device management and vision processing modules.
 *
 * @param[out] error Receives detailed error information if operation fails.
 * @return New pipeline instance handle.
 */
DT_EXPORT dt_pipeline* dt_create_pipeline(dt_error** error);

/**
 * @brief Creates a pipeline instance pre-configured with a specific device.
 *
 * @param[in] dev Device instance to bind to the pipeline (must be valid and open).
 * @param[out] error Receives detailed error information if operation fails.
 * @return New pipeline instance handle.
 */
DT_EXPORT dt_pipeline* dt_create_pipeline_with_device(const dt_device* dev, dt_error** error);

/**
 * @brief Releases all resources associated with a pipeline instance.
 *
 * Stops active streams if running and deallocates internal resources.
 *
 * @param[in] pipeline Pipeline instance to destroy. Handle becomes invalid after call.
 */
DT_EXPORT void dt_delete_pipeline(dt_pipeline* pipeline);

/**
 * @brief Starts streaming pipeline with default configuration.
 *
 * Initiates frame capture and processing loop. Captured frames are dispatched to:
 * - Attached vision modules
 * - Processing blocks
 * - Frame consumers via dt_pipeline_wait_for_frames().
 *
 * @param[in] pipeline Initialized pipeline instance.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_pipeline_start(dt_pipeline* pipeline, dt_error** error);

/**
 * @brief Starts streaming pipeline with custom configuration.
 *
 * @param[in] pipeline Initialized pipeline instance.
 * @param[in] config Runtime parameters for stream configuration (resolution, Fps, etc).
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_pipeline_start_with_config(dt_pipeline* pipeline,
                                             const dt_config* config,
                                             dt_error** error);

/**
 * @brief Starts pipeline with frame callback using default configuration.
 *
 * @note Enabling callbacks disables dt_pipeline_wait_for_frames() for this pipeline.
 *
 * @param[in] pipeline Initialized pipeline instance.
 * @param[in] callback Function to invoke when complete frameset is available.
 * @param[in] user_data Context pointer passed to callback (may be NULL).
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_pipeline_start_with_callback(dt_pipeline* pipeline,
                                               dt_frameset_callback callback,
                                               void* user_data,
                                               dt_error** error);

/**
 * @brief Starts pipeline with custom configuration and frame callback.
 *
 * @param[in] pipeline Initialized pipeline instance.
 * @param[in] config Runtime parameters for stream configuration.
 * @param[in] callback Function to invoke when complete frameset is available.
 * @param[in] user_data Context pointer passed to callback (may be NULL).
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_pipeline_start_with_config_and_callback(dt_pipeline* pipeline,
                                                          const dt_config* config,
                                                          dt_frameset_callback callback,
                                                          void* user_data,
                                                          dt_error** error);

/**
 * @brief Gracefully stops pipeline streaming and processing.
 *
 * Ensures in-flight frames are processed before shutdown.
 *
 * @param[in] pipeline Running pipeline instance.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_pipeline_stop(dt_pipeline* pipeline, dt_error** error);

/**
 * @brief Retrieves current pipeline configuration.
 *
 * Returns default configuration if no custom configuration was applied.
 *
 * @param[in] pipeline Initialized pipeline instance.
 * @param[out] error Receives detailed error information if operation fails.
 * @return Current configuration handle. Ownership remains with pipeline.
 */
DT_EXPORT dt_config* dt_pipeline_get_config(const dt_pipeline* pipeline, dt_error** error);

/**
 * @brief Blocks until synchronized frameset is available or timeout occurs.
 *
 * Returns time-aligned frames from all enabled streams. Drops frames if called slower than
 * production rate. Maintain frame handles minimally to avoid resource exhaustion.
 *
 * @param[in] pipeline Running pipeline instance.
 * @param[in] timeout_ms Maximum wait time in milliseconds (0 = non-blocking).
 * @param[out] error Receives detailed error information if operation fails.
 * @return Frameset container handle. Release with dt_frame_release().
 */
DT_EXPORT dt_frame* dt_pipeline_wait_for_frames(dt_pipeline* pipeline,
                                                uint32_t timeout_ms,
                                                dt_error** error);

/**
 * @brief Retrieves device bound to the pipeline.
 *
 * @param[in] pipeline Initialized pipeline instance.
 * @param[out] error Receives detailed error information if operation fails.
 * @return Bound device handle. Ownership remains with pipeline.
 */
DT_EXPORT dt_device* dt_pipeline_get_device(const dt_pipeline* pipeline, dt_error** error);

/**
 * @brief Retrieves active stream profiles for a sensor type.
 *
 * @param[in] pipeline Initialized pipeline instance.
 * @param[in] sensor_type Target sensor (use dt_device_get_sensor_list() for valid types).
 * @param[out] error Receives detailed error information if operation fails.
 * @return List of active stream profiles. Release with dt_delete_stream_profile_list().
 */
DT_EXPORT dt_stream_profile_list* dt_pipeline_get_stream_profile_list(const dt_pipeline* pipeline,
                                                                      dt_sensor_type sensor_type,
                                                                      dt_error** error);

/**
 * @brief Retrieves current camera intrinsic parameters.
 *
 * Applies depth-to-color (D2C) adjustments if enabled in pipeline configuration.
 *
 * @param[in] pipeline Running pipeline instance.
 * @param[out] error Receives detailed error information if operation fails.
 * @return Camera intrinsic parameters. Returned structure is self-contained.
 */
DT_EXPORT dt_camera_param dt_pipeline_get_camera_param(dt_pipeline* pipeline, dt_error** error);

/**
 * @brief Enables hardware-level frame synchronization across streams.
 *
 * Uses frame timestamps for temporal alignment. Can be toggled dynamically during streaming.
 *
 * @param[in] pipeline Running pipeline instance.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_pipeline_enable_frame_sync(dt_pipeline* pipeline, dt_error** error);

/**
 * @brief Disables hardware-level frame synchronization.
 *
 * @param[in] pipeline Running pipeline instance.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_pipeline_disable_frame_sync(dt_pipeline* pipeline, dt_error** error);

#ifdef __cplusplus
}
#endif

#endif  // DEPTRUM_H_DT_PIPELINE_H_
