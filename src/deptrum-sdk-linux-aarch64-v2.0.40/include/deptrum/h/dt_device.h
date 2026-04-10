// Copyright (c) Deptrum Inc. All Rights Reserved.

/**
 * @file dt_device.h
 * @brief Provides device management interfaces including device enumeration,
 *        configuration control, sensor access, firmware operations, and device state monitoring.
 */

#ifndef DEPTRUM_H_DT_DEVICE_H_
#define DEPTRUM_H_DT_DEVICE_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "dt_option.h"
#include "dt_types.h"

/**
 * @brief Releases resources associated with a device instance.
 *
 * @param[in] device Target device instance to destroy.
 *             If NULL, no operation is performed.
 */
DT_EXPORT void dt_delete_device(dt_device* device);

/**
 * @brief Releases device list resources.
 *
 * @param[in] list Target device list instance to destroy.
 *             If NULL, no operation is performed.
 */
DT_EXPORT void dt_delete_device_list(dt_device_list* list);

/**
 * @brief Queries device count in a device list.
 *
 * @param[in] list Initialized device list instance.
 * @param[out] error Receives detailed error information if operation fails.
 * @return uint32_t Number of enumerated devices. Returns 0 on error.
 */
DT_EXPORT uint32_t dt_device_list_get_count(const dt_device_list* list, dt_error** error);

/**
 * @brief Creates a device instance from device list.
 *
 * @attention Fails if device is already acquired elsewhere.
 *
 * @param[in] list Initialized device list instance.
 * @param[in] index Zero-based device index in list.
 * @param[out] error Receives detailed error information if operation fails.
 * @return dt_device* New device instance. Returns NULL on failure.
 */
DT_EXPORT dt_device* dt_device_list_get_device(const dt_device_list* list,
                                               uint32_t index,
                                               dt_error** error);

/**
 * @brief Create a device.
 *
 * @attention If the device has already been acquired and created elsewhere, repeated acquisitions
 * will return an error.
 *
 * @param[in] list Device list object.
 * @param[in] serial_number The serial number of the device to create.
 * @param[out] error Receives detailed error information if operation fails.
 * @return dt_device* The created device.
 */
DT_EXPORT dt_device* dt_device_list_get_device_by_serial_number(const dt_device_list* list,
                                                                const char* serial_number,
                                                                dt_error** error);

/**
 * @brief Retrieves a device from the device list by its unique identifier.
 *
 * @brief On Linux platform, for usb device, the uid of the device is composed of bus-port-dev, for
 * example 1-1.2-1. But the SDK will remove the dev number and only keep the bus-port as the uid to
 * create the device, for example 1-1.2, so that we can create a device connected to the specified
 * USB port. Similarly, users can also directly pass in bus-port as uid to create device.
 * @attention If the device has already been acquired and created elsewhere, repeated acquisitions
 * will return an error.
 *
 * @param[in] list Device list to search.
 * @param[in] uid Unique identifier of the target device.
 * @param[out] error Pointer to receive error details. Initialized to NULL before call,
 *                   populated with error object on failure. Caller must release memory.
 * @return Pointer to the requested device instance, or NULL if not found.
 */
DT_EXPORT dt_device* dt_device_list_get_device_by_uid(const dt_device_list* list,
                                                      const char* uid,
                                                      dt_error** error);

/**
 * @brief Retrieves device information from list.
 *
 * @param[in] list Initialized device list instance.
 * @param[in] index Zero-based device index.
 * @param[in] info Type of information to retrieve.
 * @param[out] error Receives detailed error information if operation fails.
 * @return const char* Device name string. Lifetime tied to device list.
 */
DT_EXPORT const char* dt_device_list_get_device_info(const dt_device_list* list,
                                                     uint32_t index,
                                                     dt_camera_info info,
                                                     dt_error** error);

/**
 * @brief Retrieves device information.
 *
 * @param[in] device Initialized device instance.
 * @param[in] info Type of information to retrieve.
 * @param[out] error Receives detailed error information if operation fails.
 * @return const char* Requested information. String lifetime tied to device.
 */
DT_EXPORT const char* dt_device_get_device_info(const dt_device* device,
                                                dt_camera_info info,
                                                dt_error** error);

/**
 * @brief Queries current operational state of device.
 *
 * @param[in] device Initialized device instance.
 * @param[out] error Receives detailed error information if operation fails.
 * @return dt_device_state Current device state information.
 */
DT_EXPORT dt_device_state dt_device_get_device_state(const dt_device* device, dt_error** error);

/**
 * @brief Registers callback for device state changes.
 *
 * @param[in] device Initialized device instance.
 * @param[in] callback Function to invoke on state changes.
 * @param[in] user_data Custom user context passed to callback.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_device_set_state_changed_callback(dt_device* device,
                                                    dt_device_state_callback callback,
                                                    void* user_data,
                                                    dt_error** error);

/**
 * @brief Reboots device hardware.
 *
 * @attention Device disconnects during reboot. Accessing handle post-reboot causes undefined
 * behavior - destroy handle with @ref dt_delete_device and reacquire device.
 *
 * @param[in] device Target device to reboot.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_device_reboot(dt_device* device, dt_error** error);

/**
 * @brief Retrieves all sensors associated with device.
 *
 * @param[in] device Initialized device instance.
 * @param[out] error Receives detailed error information if operation fails.
 * @return dt_sensor_list* Sensor list instance. Must be released by caller.
 *         Returns NULL on failure.
 */
DT_EXPORT dt_sensor_list* dt_device_get_sensor_list(const dt_device* device, dt_error** error);

/**
 * @brief Retrieves specific sensor by type.
 *
 * @param[in] device Initialized device instance.
 * @param[in] type Sensor type identifier.
 * @param[out] error Receives detailed error information if operation fails.
 * @return dt_sensor* Requested sensor instance. Lifetime tied to device.
 */
DT_EXPORT dt_sensor* dt_device_get_sensor(dt_device* device, dt_sensor_type type, dt_error** error);

/**
 * @brief Queries number of supported configuration options.
 *
 * @param[in] device Initialized device instance.
 * @param[out] error Receives detailed error information if operation fails.
 * @return uint32_t Number of available options.
 */
DT_EXPORT uint32_t dt_device_get_supported_option_count(const dt_device* device, dt_error** error);

/**
 * @brief Retrieves option metadata by index.
 *
 * @param[in] device Initialized device instance.
 * @param[in] index Zero-based option index.
 * @param[out] error Receives detailed error information if operation fails.
 * @return dt_option_item Option metadata structure.
 */
DT_EXPORT dt_option_item dt_device_get_supported_option_item(const dt_device* device,
                                                             uint32_t index,
                                                             dt_error** error);

/**
 * @brief Checks option permission availability.
 *
 * @param[in] device Initialized device instance.
 * @param[in] option_id Target option identifier.
 * @param[in] permission Permission type to check.
 * @param[out] error Receives detailed error information if operation fails.
 * @return true Permission granted for specified option.
 * @return false Permission denied or option unavailable.
 */
DT_EXPORT bool dt_device_is_option_supported(const dt_device* device,
                                             dt_option_id option_id,
                                             dt_permission_type permission,
                                             dt_error** error);

/**
 * @brief Sets an integer type of device option.
 *
 * @param[in] device Initialized device instance.
 * @param[in] option_id ID of the option to be set.
 * @param[in] value Value of the option  to be set.
 * @param[out] error Receives detailed error information if operation fails.
 */

DT_EXPORT void dt_device_set_int_option(dt_device* device,
                                        dt_option_id option_id,
                                        int32_t value,
                                        dt_error** error);

/**
 * @brief Gets an integer type of device option.
 *
 * @param[in] device Initialized device instance.
 * @param[in] option_id ID of the option to be get.
 * @param[out] error Receives detailed error information if operation fails.
 * @return int32_t Value of the option.
 */
DT_EXPORT int32_t dt_device_get_int_option(dt_device* device,
                                           dt_option_id option_id,
                                           dt_error** error);

/**
 * @brief Gets the integer type of device option range.
 *
 * @param[in] device Initialized device instance.
 * @param[in] option_id ID of the option to be get.
 * @param[out] error Receives detailed error information if operation fails.
 * @return dt_int_option_range Range of the option.
 */
DT_EXPORT dt_int_option_range dt_device_get_int_option_range(dt_device* device,
                                                             dt_option_id option_id,
                                                             dt_error** error);

/**
 * @brief Sets a float type of device option.
 *
 * @param[in] device Initialized device instance.
 * @param[in] option_id ID of the option to be set.
 * @param[in] value Value of the option to be set.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_device_set_float_option(dt_device* device,
                                          dt_option_id option_id,
                                          float value,
                                          dt_error** error);

/**
 * @brief Gets a float type of device option.
 *
 * @param[in] device Initialized device instance.
 * @param[in] option_id ID of the option to be get.
 * @param[out] error Receives detailed error information if operation fails.
 * @return float Value of the option.
 */
DT_EXPORT float dt_device_get_float_option(dt_device* device,
                                           dt_option_id option_id,
                                           dt_error** error);

/**
 * @brief Gets the float type of device option range.
 *
 * @param[in] device Initialized device instance.
 * @param[in] option_id ID of the option to be get.
 * @param[out] error Receives detailed error information if operation fails.
 * @return dt_float_option_range Range of the option.
 */
DT_EXPORT dt_float_option_range dt_device_get_float_option_range(dt_device* device,
                                                                 dt_option_id option_id,
                                                                 dt_error** error);

/**
 * @brief Sets a boolean type of device option.
 *
 * @param[in] device Initialized device instance.
 * @param[in] option_id ID of the option to be set.
 * @param[in] value Value of the option to be set.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_device_set_bool_option(dt_device* device,
                                         dt_option_id option_id,
                                         bool value,
                                         dt_error** error);

/**
 * @brief Gets a boolean type of device option.
 *
 * @param[in] device Initialized device instance.
 * @param[in] option_id ID of the option to be get.
 * @param[out] error Receives detailed error information if operation fails.
 * @return bool Value of the option.
 */
DT_EXPORT bool dt_device_get_bool_option(dt_device* device,
                                         dt_option_id option_id,
                                         dt_error** error);

/**
 * @brief Gets the boolean type of device option range.
 *
 * @param[in] device Initialized device instance.
 * @param[in] option_id ID of the option to be get.
 * @param[out] error Receives detailed error information if operation fails.
 * @return dt_bool_option_range Range of the option.
 */
DT_EXPORT dt_bool_option_range dt_device_get_bool_option_range(dt_device* device,
                                                               dt_option_id option_id,
                                                               dt_error** error);

/**
 * @brief Sets structured data.
 *
 * @param[in] device Initialized device instance.
 * @param[in] option_id ID of the option to be set.
 * @param[in] data The data to be set.
 * @param[in] data_size Size of the data.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_device_set_structured_data(dt_device* device,
                                             dt_option_id option_id,
                                             const uint8_t* data,
                                             uint32_t data_size,
                                             dt_error** error);

/**
 * @brief Gets structured data of a device option.
 *
 * @param[in] device Initialized device instance.
 * @param[in] option_id ID of the option to be get.
 * @param[out] data The obtained option data.
 * @param[out] data_size The size of the obtained option data.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_device_get_structured_data(dt_device* device,
                                             dt_option_id option_id,
                                             uint8_t* data,
                                             uint32_t* data_size,
                                             dt_error** error);

/**
 * @brief Gets raw data of a device option.
 *
 * @param[in] device Initialized device instance.
 * @param[in] option_id ID of the option to be get.
 * @param[out] cb The get data callback.
 * @param[out] user_data User-defined data that will be returned in the callback.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_device_get_raw_data(dt_device* device,
                                      dt_option_id option_id,
                                      dt_get_data_callback cb,
                                      void* user_data,
                                      dt_error** error);
/**
 * @brief Checks global timestamp support capability.
 *
 * @param[in] device Initialized device instance.
 * @param[out] error Receives detailed error information if operation fails.
 * @return true Device supports global timestamp synchronization.
 * @return false Feature unavailable.
 */
DT_EXPORT bool dt_device_is_global_timestamp_supported(const dt_device* device, dt_error** error);

/**
 * @brief Enables/disables global timestamp synchronization.
 *
 * @param[in] device Initialized device instance.
 * @param[in] enable true to enable synchronization, false to disable.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_device_enable_global_timestamp(dt_device* device, bool enable, dt_error** error);

/**
 * @brief Transfers file from host to device storage.
 *
 * @param[in] device Initialized device instance.
 * @param[in] src_file_path Source file path on host system.
 * @param[in] dst_file_path Destination path on device storage.
 * @param[in] callback Progress and completion callback.
 * @param[in] async true for asynchronous operation (non-blocking).
 * @param[in] user_data Custom context passed to callback.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_device_send_file_to_device(dt_device* device,
                                             const char* src_file_path,
                                             const char* dst_file_path,
                                             dt_file_tran_callback callback,
                                             bool async,
                                             void* user_data,
                                             dt_error** error);

/**
 * @brief Retrieves file from device storage to host.
 *
 * @param[in] device Initialized device instance.
 * @param[in] src_file_path Source file path on device storage.
 * @param[in] dst_file_path Destination path on host system.
 * @param[in] callback Progress and completion callback.
 * @param[in] async true for asynchronous operation (non-blocking).
 * @param[in] user_data Custom context passed to callback.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_device_receive_file_from_device(dt_device* device,
                                                  const char* src_file_path,
                                                  const char* dst_file_path,
                                                  dt_file_tran_callback callback,
                                                  bool async,
                                                  void* user_data,
                                                  dt_error** error);

/**
 * @brief Updates device firmware from file.
 *
 * @param[in] device Initialized device instance.
 * @param[in] path Path to firmware image file on host.
 * @param[in] callback Firmware update progress callback.
 * @param[in] async true for asynchronous operation (non-blocking).
 * @param[in] user_data Custom context passed to callback.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_device_update_firmware(dt_device* device,
                                         const char* path,
                                         dt_device_fw_update_callback callback,
                                         bool async,
                                         void* user_data,
                                         dt_error** error);

/**
 * @brief Updates device firmware from memory buffer.
 *
 * @param[in] device Initialized device instance.
 * @param[in] data Firmware image data buffer.
 * @param[in] data_size Size of firmware data in bytes.
 * @param[in] callback Firmware update progress callback.
 * @param[in] async true for asynchronous operation (non-blocking).
 * @param[in] user_data Custom context passed to callback.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_device_update_firmware_from_data(dt_device* device,
                                                   const uint8_t* data,
                                                   uint32_t data_size,
                                                   dt_device_fw_update_callback callback,
                                                   bool async,
                                                   void* user_data,
                                                   dt_error** error);

/**
 * @brief Writes data to device flash memory.
 *
 * @param[in] device Initialized device instance.
 * @param[in] offset Flash memory offset address.
 * @param[in] data Source data buffer to write.
 * @param[in] data_size Size of data in bytes.
 * @param[in] callback Completion status callback.
 * @param[in] async true for asynchronous operation (non-blocking).
 * @param[in] user_data Custom context passed to callback.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_device_write_flash(dt_device* device,
                                     uint32_t offset,
                                     const void* data,
                                     uint32_t data_size,
                                     dt_set_data_callback callback,
                                     bool async,
                                     void* user_data,
                                     dt_error** error);

/**
 * @brief Reads data from device flash memory.
 *
 * @param[in] device Initialized device instance.
 * @param[in] offset Flash memory offset address.
 * @param[in] data_size Number of bytes to read.
 * @param[in] callback Data reception and progress callback.
 * @param[in] async true for asynchronous operation (non-blocking).
 * @param[in] user_data Custom context passed to callback.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_device_read_flash(dt_device* device,
                                    uint32_t offset,
                                    uint32_t data_size,
                                    dt_get_data_callback callback,
                                    bool async,
                                    void* user_data,
                                    dt_error** error);

/**
 * @brief Controls device heartbeat monitoring.
 *
 * @details When enabled, SDK maintains 3-second heartbeat intervals.
 * @attention Extended heartbeat absence triggers device disconnection and reboot.
 *
 * @param[in] device Initialized device instance.
 * @param[in] enable true to enable heartbeat, false to disable.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_device_enable_heartbeat(dt_device* device, bool enable, dt_error** error);

/**
 * @brief Executes vendor-specific device command (factory use).
 *
 * @param[in] device Initialized device instance.
 * @param[in] send_data Command data buffer to transmit.
 * @param[in] send_data_size Size of transmit buffer in bytes.
 * @param[out] receive_data Response data buffer.
 * @param[in,out] receive_data_size Input: max capacity of receive buffer.
 *                Output: actual received byte count.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_device_send_and_receive_data(dt_device* device,
                                               const uint8_t* send_data,
                                               uint32_t send_data_size,
                                               uint8_t* receive_data,
                                               uint32_t* receive_data_size,
                                               dt_error** error);

/**
 * @brief Retrieves the original camera calibration parameters stored on the device.
 *
 * Provides factory-calibrated camera parameters not adjusted to current streaming configuration.
 *
 * @note Parameters may require scaling or mirroring for current usage. Non-expert users should
 *       prefer dt_pipeline_get_camera_param() for pre-processed parameters.
 *
 * @param[in] device Device instance to query.
 * @param[out] error Receives detailed error information if operation fails.
 * @return Handle to camera parameter collection. Caller must release with
 * dt_delete_camera_param_list.
 */
DT_EXPORT dt_camera_param_list* dt_device_get_calibration_camera_param_list(dt_device* device,
                                                                            dt_error** error);

/**
 * @brief Retrieves the number of camera parameter sets in the collection.
 *
 * @param[in] param_list Camera parameter collection instance.
 * @param[out] error Receives detailed error information if operation fails.
 * @return Count of parameter sets in the collection.
 */
DT_EXPORT uint32_t dt_camera_param_list_get_count(dt_camera_param_list* param_list,
                                                  dt_error** error);

/**
 * @brief Retrieves a specific camera parameter set by index.
 *
 * @param[in] param_list Camera parameter collection instance.
 * @param[in] index Zero-based index of the parameter set to retrieve.
 * @param[out] error Receives detailed error information if operation fails.
 * @return Camera parameter set. Returned structure is self-contained and requires no explicit
 * deletion.
 */
DT_EXPORT dt_camera_param dt_camera_param_list_get_param(dt_camera_param_list* param_list,
                                                         uint32_t index,
                                                         dt_error** error);

/**
 * @brief Releases resources associated with a camera parameter collection.
 *
 * @param[in] param_list Camera parameter collection to destroy.
 */
DT_EXPORT void dt_delete_camera_param_list(dt_camera_param_list* param_list);

/**
 * @brief Synchronizes the device's internal timer with the host system clock.
 *
 * Aligns device timekeeping with host time. Can be called concurrently on multiple devices
 * for system-wide synchronization.
 *
 * @note May cause timestamp discontinuity in active streams during synchronization.
 * @note Periodic synchronization (recommended every 60 minutes) compensates for device timer drift.
 *
 * @param[in] device Device handle to synchronize.
 * @param[out] error Receives detailed error information if operation fails.
 */
DT_EXPORT void dt_device_timer_sync_with_host(dt_device* device, dt_error** error);

#ifdef __cplusplus
}
#endif

#endif  // DEPTRUM_H_DT_DEVICE_H_