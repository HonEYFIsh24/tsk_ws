// Copyright (c) Deptrum Inc. All Rights Reserved.

/**
 * @file dt_filter.h
 * @brief Defines processing units for point cloud generation, format conversion, and other data
 * transformation operations.
 */

#ifndef DEPTRUM_H_DT_FILTER_H_
#define DEPTRUM_H_DT_FILTER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "dt_types.h"

/**
 * @brief Creates a new filter instance with the specified identifier.
 *
 * @attention Returns NULL if the requested filter is private and its creator hasn't been activated.
 *
 * @param[in] name Unique identifier of the filter to create.
 * @param[out] error Receives detailed error information if operation fails.
 * @return Pointer to the created filter object. Returns NULL on failure.
 */
DT_EXPORT dt_filter* dt_create_filter(const char* name, dt_error** error);

/**
 * @brief Releases resources associated with a filter.
 *
 * @param[in] filter Target filter object to destroy.
 */
DT_EXPORT void dt_delete_filter(dt_filter* filter);

/**
 * @brief Retrieves the CSV-formatted configuration schema for the filter.
 *
 * @details Format contains: Parameter name, type ("int"/"float"/"bool"), min value, max value,
 *          step size, default value, and description (comma-separated).
 *
 * @param[in] filter Target filter object to query.
 * @param[out] error Receives detailed error information if operation fails.
 * @return CSV-formatted string describing the filter's configuration schema.
 */
DT_EXPORT const char* dt_filter_get_config_schema(const dt_filter* filter, dt_error** error);

/**
 * @brief Retrieves structured configuration schema as a list of items.
 *
 * @attention Caller must release the list using @ref dt_delete_filter_config_schema_list.
 *
 * @param[in] filter Target filter object to query.
 * @param[out] error Receives detailed error information if operation fails.
 * @return Pointer to the configuration schema list object.
 */
DT_EXPORT dt_filter_config_schema_list* dt_filter_get_config_schema_list(const dt_filter* filter,
                                                                         dt_error** error);

/**
 * @brief Updates filter configuration using command-style arguments.
 *
 * @attention Arguments must match the schema from @ref dt_filter_get_config_schema.
 *
 * @param[in] filter Target filter object to configure.
 * @param[in] argc Number of configuration arguments.
 * @param[in] argv Array of configuration values.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_filter_update_config(dt_filter* filter,
                                       uint8_t argc,
                                       const char** argv,
                                       dt_error** error);

/**
 * @brief Retrieves a configuration parameter's value as double-precision.
 *
 * @attention Actual data type is determined by the filter's schema.
 *
 * @param[in] filter Target filter object to query.
 * @param[in] config_name Name of the configuration parameter.
 * @param[out] error Receives detailed error information if operation fails.
 * @return Current value of the parameter (automatically cast to double).
 */
DT_EXPORT double dt_filter_get_config_value(const dt_filter* filter,
                                            const char* config_name,
                                            dt_error** error);

/**
 * @brief Modifies a configuration parameter's value.
 *
 * @attention Value will be cast to the type specified in the filter's schema.
 *
 * @param[in] filter Target filter object to configure.
 * @param[in] config_name Name of the configuration parameter.
 * @param[in] value New value to set (as double-precision).
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_filter_set_config_value(dt_filter* filter,
                                          const char* config_name,
                                          double value,
                                          dt_error** error);

/**
 * @brief Processes a frame synchronously and returns the result.
 *
 * @param[in] filter Target filter object to execute processing.
 * @param[in] frame Input frame data to process.
 * @param[out] error Receives detailed error information if operation fails.
 * @return Pointer to the processed output frame.
 */
DT_EXPORT dt_frame* dt_filter_process(dt_filter* filter, const dt_frame* frame, dt_error** error);

/**
 * @brief Sets the callback handler for asynchronous processing results.
 *
 * @param[in] filter Target filter object to configure.
 * @param[in] callback Function to invoke when processing completes.
 * @param[in] user_data Custom context pointer passed to the callback.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_filter_set_callback(dt_filter* filter,
                                      dt_filter_callback callback,
                                      void* user_data,
                                      dt_error** error);

/**
 * @brief Submits a frame for asynchronous processing.
 *
 * @attention Increases the frame's reference count; caller must still release original frame.
 *
 * @param[in] filter Target filter object to process frame.
 * @param[in] frame Input frame data to process.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_filter_push_frame(dt_filter* filter, const dt_frame* frame, dt_error** error);

/**
 * @brief Resets internal state and clears processing caches.
 *
 * @param[in] filter Target filter object to reset.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_filter_reset(dt_filter* filter, dt_error** error);
/**
 * @brief Enables or disables frame processing functionality.
 *
 * @note When disabled, outputs direct clones of input frames without processing.
 *
 * @param[in] filter Target filter object to configure.
 * @param[in] enable true to enable processing, false to bypass.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_filter_enable(dt_filter* filter, bool enable, dt_error** error);

/**
 * @brief Checks whether frame processing is currently enabled.
 *
 * @param[in] filter Target filter object to query.
 * @param[out] error Receives detailed error information if operation fails.
 * @return true if processing is active, false if bypassing.
 */
DT_EXPORT bool dt_filter_is_enabled(const dt_filter* filter, dt_error** error);

/**
 * @brief Retrieves the unique identifier of a filter.
 *
 * @param[in] filter Target filter object to query.
 * @param[out] error Receives detailed error information if operation fails.
 * @return Null-terminated string containing the filter's name.
 */
DT_EXPORT const char* dt_filter_get_name(const dt_filter* filter, dt_error** error);

/**
 * @brief Configures alignment target stream profile for align filters.
 *
 * @note Required when target stream isn't active (no frames to derive intrinsics/extrinsics).
 *
 * @param[in] filter Target align filter object to configure.
 * @param[in] align_to_stream_profile Reference stream profile for alignment operations.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_align_filter_set_align_to_stream_profile(
    dt_filter* filter,
    const dt_stream_profile* align_to_stream_profile,
    dt_error** error);

/**
 * @brief Retrieves the number of filters in a list.
 *
 * @param[in] filter_list Target filter list to query.
 * @param[out] error Receives detailed error information if operation fails.
 * @return Total count of filters in the list.
 */
DT_EXPORT uint32_t dt_filter_list_get_count(const dt_filter_list* filter_list, dt_error** error);

/**
 * @brief Retrieves a filter by its index in a list.
 *
 * @param[in] filter_list Target filter list to query.
 * @param[in] index Zero-based position of the filter to retrieve.
 * @param[out] error Receives detailed error information if operation fails.
 * @return Pointer to the filter at the specified index.
 */
DT_EXPORT dt_filter* dt_filter_list_get_filter(const dt_filter_list* filter_list,
                                               uint32_t index,
                                               dt_error** error);

/**
 * @brief Releases resources associated with a filter list.
 *
 * @param[in] filter_list Target filter list to destroy.
 */
DT_EXPORT void dt_delete_filter_list(dt_filter_list* filter_list);

/**
 * @brief Retrieves the number of items in a configuration schema list.
 *
 * @param[in] config_schema_list Target schema list to query.
 * @param[out] error Receives detailed error information if operation fails.
 * @return Total count of configuration items.
 */
DT_EXPORT uint32_t
dt_filter_config_schema_list_get_count(const dt_filter_config_schema_list* config_schema_list,
                                       dt_error** error);

/**
 * @brief Retrieves a configuration schema item by index.
 *
 * @param[in] config_schema_list Target schema list to query.
 * @param[in] index Zero-based position of the item to retrieve.
 * @param[out] error Receives detailed error information if operation fails.
 * @return Configuration schema item at the specified index.
 */
DT_EXPORT dt_filter_config_schema_item
dt_filter_config_schema_list_get_item(const dt_filter_config_schema_list* config_schema_list,
                                      uint32_t index,
                                      dt_error** error);

/**
 * @brief Releases resources associated with a configuration schema list.
 *
 * @param[in] config_schema_list Target schema list to destroy.
 */
DT_EXPORT void dt_delete_filter_config_schema_list(
    dt_filter_config_schema_list* config_schema_list);

#ifdef __cplusplus
}
#endif

#endif  // DEPTRUM_H_DT_FILTER_H_