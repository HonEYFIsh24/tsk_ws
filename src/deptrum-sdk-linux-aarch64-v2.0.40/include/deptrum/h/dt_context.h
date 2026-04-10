// Copyright (c) Deptrum Inc. All Rights Reserved.

/**
 * @file dt_context.h
 * @brief Manages SDK runtime environment, handles resource allocation/release,
 *        and coordinates multi-device operations including enumeration and monitoring.
 */

#ifndef DEPTRUM_H_DT_CONTEXT_H_
#define DEPTRUM_H_DT_CONTEXT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "dt_types.h"

/**
 * @brief Creates a context object using the default configuration file.
 *
 * @param[out] error Receives detailed error information if operation fails.
 * @return Pointer to the initialized context object. Returns NULL on failure.
 */
DT_EXPORT dt_context* dt_create_context(dt_error** error);

/**
 * @brief Creates a context object with a specified configuration file.
 *
 * @param[in] config_file_path Path to custom configuration file.
 *             If NULL, uses default configuration.
 * @param[out] error Receives detailed error information if operation fails.
 * @return Pointer to the initialized context object. Returns NULL on failure.
 */
DT_EXPORT dt_context* dt_create_context_with_config(const char* config_file_path, dt_error** error);

/**
 * @brief Releases all resources associated with a context object.
 *
 * @param[in] context Target context object to destroy. If NULL, no operation is performed.
 */
DT_EXPORT void dt_delete_context(dt_context* context);

/**
 * @brief Retrieves a list of enumerated devices.
 *
 * @param[in] context Initialized context object.
 * @param[out] error Receives detailed error information if operation fails.
 * @return Pointer to device list. Must be released by caller. Returns NULL on failure.
 */
DT_EXPORT dt_device_list* dt_query_device_list(dt_context* context, dt_error** error);

/**
 * @brief Creates a network device instance for communication.
 *
 * @param[in] context Initialized context object.
 * @param[in] address IP address of the target device.
 * @param[in] port Network port of the target device.
 * @param[out] error Receives detailed error information if operation fails.
 * @return Pointer to created device object. Returns NULL on failure.
 */
DT_EXPORT dt_device* dt_create_net_device(dt_context* context,
                                          const char* address,
                                          uint16_t port,
                                          dt_error** error);

/**
 * @brief Enables/disables automatic network device discovery.
 *
 * @attention Discovered devices require host-subnet alignment for connectivity.
 * @attention Default behavior can be configured via settings.
 *
 * @param[in] context Initialized context object.
 * @param[in] enable true activates discovery, false disables it.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_enable_net_device_enumeration(dt_context* context, bool enable, dt_error** error);

/**
 * @brief Registers a callback for device connection/disconnection events.
 *
 * @attention Supports multiple callbacks. Each call adds a new callback.
 * @attention Caller must release returned device lists in callback parameters.
 *
 * @param[in] context Initialized context object.
 * @param[in] callback Function pointer to event handler.
 * @param[in] user_data Custom user data passed to callback function.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_set_device_changed_callback(dt_context* context,
                                              dt_device_changed_callback callback,
                                              void* user_data,
                                              dt_error** error);

/**
 * @brief Synchronizes host-device clocks across all created devices.
 *
 * @param[in] context Initialized context object.
 * @param[in] repeat_interval_msec Resynchronization interval in milliseconds.
 *             Value 0 performs single synchronization.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_enable_device_clock_sync(dt_context* context,
                                           uint64_t repeat_interval_msec,
                                           dt_error** error);

/**
 * @brief Selects UVC backend implementation for Linux environments.
 *
 * @attention Linux exclusive feature. Affects subsequently created devices.
 *
 * @param[in] context Initialized context object.
 * @param[in] backend_type Preferred backend implementation type.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_set_uvc_backend_type(dt_context* context,
                                       dt_uvc_backend_type backend_type,
                                       dt_error** error);

#ifdef __cplusplus
}
#endif

#endif  // DEPTRUM_DT_CONTEXT_H