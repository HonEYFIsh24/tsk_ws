// Copyright (c) Deptrum Inc. All Rights Reserved.

/**
 * @file dt_sensor.h
 * @brief Sensor management interfaces for configuring, controlling, and monitoring sensor streams.
 *
 * Provides functionality to query sensor capabilities, manage streaming profiles, control data
 * flow, and obtain recommended processing filters. Supports multiple sensor types and
 * configurations.
 */

#ifndef DEPTRUM_H_DT_SENSOR_H_
#define DEPTRUM_H_DT_SENSOR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "dt_types.h"

/**
 * @brief Retrieves the number of sensors in a sensor list.
 *
 * @param[in] sensor_list Sensor list container instance.
 * @param[out] error Receives detailed error information if operation fails.
 * @return Number of sensors contained in the list.
 */
DT_EXPORT uint32_t dt_sensor_list_get_count(const dt_sensor_list* sensor_list, dt_error** error);

/**
 * @brief Retrieves sensor type at specified index in sensor list.
 *
 * @param[in] sensor_list Sensor list container instance.
 * @param[in] index Zero-based position in sensor list (must be < list count).
 * @param[out] error Receives detailed error information if operation fails.
 * @return Sensor type identifier.
 */
DT_EXPORT dt_sensor_type dt_sensor_list_get_sensor_type(const dt_sensor_list* sensor_list,
                                                        uint32_t index,
                                                        dt_error** error);

/**
 * @brief Retrieves sensor by type from sensor list.
 *
 * @param[in] sensor_list Sensor list container instance.
 * @param[in] sensor_type Target sensor type to locate.
 * @param[out] error Receives detailed error information if operation fails.
 * @return Sensor handle if found, NULL if not present.
 */
DT_EXPORT dt_sensor* dt_sensor_list_get_sensor_by_type(const dt_sensor_list* sensor_list,
                                                       dt_sensor_type sensor_type,
                                                       dt_error** error);

/**
 * @brief Retrieves sensor by index from sensor list.
 *
 * @param[in] sensor_list Sensor list container instance.
 * @param[in] index Zero-based position in sensor list (must be < list count).
 * @param[out] error Receives detailed error information if operation fails.
 * @return Sensor handle at specified position.
 */
DT_EXPORT dt_sensor* dt_sensor_list_get_sensor(const dt_sensor_list* sensor_list,
                                               uint32_t index,
                                               dt_error** error);

/**
 * @brief Releases resources associated with a sensor list.
 *
 * @param[in] sensor_list Sensor list instance to destroy.
 */
DT_EXPORT void dt_delete_sensor_list(dt_sensor_list* sensor_list);

/**
 * @brief Retrieves type identifier of a sensor.
 *
 * @param[in] sensor Sensor instance.
 * @param[out] error Receives detailed error information if operation fails.
 * @return Sensor type identifier.
 */
DT_EXPORT dt_sensor_type dt_sensor_get_type(const dt_sensor* sensor, dt_error** error);

/**
 * @brief Retrieves available streaming configurations for a sensor.
 *
 * @param[in] sensor Sensor instance.
 * @param[out] error Receives detailed error information if operation fails.
 * @return List of supported stream profiles. Release with dt_delete_stream_profile_list().
 */
DT_EXPORT dt_stream_profile_list* dt_sensor_get_stream_profile_list(const dt_sensor* sensor,
                                                                    dt_error** error);

/**
 * @brief Generates recommended processing filters for a sensor.
 *
 * @param[in] sensor Sensor instance.
 * @param[out] error Receives detailed error information if operation fails.
 * @return Filter list handle. Release with dt_delete_filter_list().
 */
DT_EXPORT dt_filter_list* dt_sensor_create_recommended_filter_list(const dt_sensor* sensor,
                                                                   dt_error** error);

/**
 * @brief Starts sensor streaming with specified configuration and data callback.
 *
 * @param[in] sensor Sensor instance to activate.
 * @param[in] profile Streaming configuration to apply.
 * @param[in] callback Frame data notification callback (may not be NULL).
 * @param[in] user_data Context pointer passed to callback (may be NULL).
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_sensor_start(dt_sensor* sensor,
                               const dt_stream_profile* profile,
                               dt_frame_callback callback,
                               void* user_data,
                               dt_error** error);

/**
 * @brief Dynamically changes streaming configuration.
 *
 * @pre Sensor must be in active streaming state.
 *
 * @param[in] sensor Sensor instance to reconfigure.
 * @param[in] profile New streaming configuration to apply.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_sensor_switch_profile(dt_sensor* sensor,
                                        dt_stream_profile* profile,
                                        dt_error** error);

/**
 * @brief Stops active sensor streaming.
 *
 * @param[in] sensor Sensor instance to deactivate.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_sensor_stop(dt_sensor* sensor, dt_error** error);

/**
 * @brief Releases resources associated with a sensor.
 *
 * @param[in] sensor Sensor instance to destroy.
 */
DT_EXPORT void dt_delete_sensor(dt_sensor* sensor);

#ifdef __cplusplus
}
#endif

#endif  // DEPTRUM_H_DT_SENSOR_H_