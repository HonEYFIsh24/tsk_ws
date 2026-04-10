// Copyright (c) Deptrum Inc. All Rights Reserved.

/**
 * @file dt_record_playback.h
 * @brief Device recording and playback interfaces for capturing and replaying sensor data streams.
 *
 * Provides functionality to record live device streams to files and replay recorded sessions.
 * Supports compression, seeking, playback speed control, and status monitoring.
 */

#ifndef DEPTRUM_H_DT_RECORD_PLAYBACK_H_
#define DEPTRUM_H_DT_RECORD_PLAYBACK_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "dt_device.h"

/**
 * @brief Creates a recording device bound to an existing device.
 *
 * @param[in] device Source device to record from (must be valid and initialized).
 * @param[in] file_path Output file path for recording data.
 * @param[in] compression_enabled Enable compression to reduce file size.
 * @param[out] error Receives detailed error information if operation fails.
 * @return New recording device handle. Release with dt_delete_record_device().
 */
DT_EXPORT dt_record_device* dt_create_record_device(dt_device* device,
                                                    const char* file_path,
                                                    bool compression_enabled,
                                                    dt_error** error);

/**
 * @brief Pauses active recording session.
 *
 * @param[in] recorder Recording device instance.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_record_device_pause(dt_record_device* recorder, dt_error** error);

/**
 * @brief Resumes paused recording session.
 *
 * @param[in] recorder Recording device instance.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_record_device_resume(dt_record_device* recorder, dt_error** error);

/**
 * @brief Releases resources associated with a recording device.
 *
 * @param[in] recorder Recording device instance to destroy.
 */
DT_EXPORT void dt_delete_record_device(dt_record_device* recorder);

/**
 * @brief Creates a playback device from recorded file.
 *
 * @param[in] file_path Path to playback file (typically .bag format).
 * @param[out] error Receives detailed error information if operation fails.
 * @return New playback device handle. Release with dt_delete_device().
 */
DT_EXPORT dt_device* dt_create_playback_device(const char* file_path, dt_error** error);

/**
 * @brief Pauses active playback session.
 *
 * @param[in] player Playback device instance.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_playback_device_pause(dt_device* player, dt_error** error);

/**
 * @brief Resumes paused playback session.
 *
 * @param[in] player Playback device instance.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_playback_device_resume(dt_device* player, dt_error** error);

/**
 * @brief Sets playback position within recording.
 *
 * @param[in] player Playback device instance.
 * @param[in] timestamp Target position in milliseconds from recording start.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_playback_device_seek(dt_device* player, uint64_t timestamp, dt_error** error);

/**
 * @brief Controls playback speed.
 *
 * @param[in] player Playback device instance.
 * @param[in] rate Playback multiplier (1.0 = real-time, 2.0 = double speed).
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_playback_device_set_playback_rate(dt_device* player,
                                                    float rate,
                                                    dt_error** error);

/**
 * @brief Registers callback for playback status changes.
 *
 * @param[in] player Playback device instance.
 * @param[in] callback Function to invoke on status changes.
 * @param[in] user_data Context pointer passed to callback (may be NULL).
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_playback_device_set_playback_status_changed_callback(
    dt_device* player,
    dt_playback_status_changed_callback callback,
    void* user_data,
    dt_error** error);

/**
 * @brief Retrieves current playback status.
 *
 * @param[in] player Playback device instance.
 * @param[out] error Receives detailed error information if operation fails.
 * @return Current playback state (playing, paused, stopped).
 */
DT_EXPORT dt_playback_status dt_playback_device_get_current_playback_status(dt_device* player,
                                                                            dt_error** error);

/**
 * @brief Retrieves current playback position.
 *
 * @param[in] player Playback device instance.
 * @param[out] error Receives detailed error information if operation fails.
 * @return Current position in milliseconds from recording start.
 */
DT_EXPORT uint64_t dt_playback_device_get_position(dt_device* player, dt_error** error);

/**
 * @brief Retrieves total recording duration.
 *
 * @param[in] player Playback device instance.
 * @param[out] error Receives detailed error information if operation fails.
 * @return Total duration in milliseconds of recording.
 */
DT_EXPORT uint64_t dt_playback_device_get_duration(dt_device* player, dt_error** error);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // DEPTRUM_H_DT_RECORD_PLAYBACK_H_