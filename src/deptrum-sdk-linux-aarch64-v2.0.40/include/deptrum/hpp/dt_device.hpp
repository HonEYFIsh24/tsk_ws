// Copyright (c) Deptrum Inc. All Rights Reserved.

/**
 * @file dt_device.hpp
 * @brief Device related types, including operations such as getting and creating a device, setting
 * and obtaining device attributes, and obtaining sensors.
 */

#ifndef DEPTRUM_HPP_DT_DEVICE_HPP_
#define DEPTRUM_HPP_DT_DEVICE_HPP_

#include <chrono>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include "deptrum/h/dt_device.h"
#include "deptrum/h/dt_option.h"
#include "deptrum/hpp/dt_filter.hpp"
#include "deptrum/hpp/dt_sensor.hpp"
#include "dt_error.hpp"
#include "dt_types.hpp"

namespace dt {

class DeviceInfo;
class SensorList;
class CameraParamList;

class Device {
 public:
  /**
   * @brief Handler for device firmware update progress reporting.
   *
   * @param[in] state Current firmware update state.
   * @param[in] message Status information during update.
   * @param[in] percent Completion percentage (0-100).
   */
  typedef std::function<void(DtFwUpdateState state, const char* message, uint8_t percent)>
      DeviceFwUpdateCallback;

  /**
   * @brief Handler for device file transfer progress reporting.
   *
   * @param[in] state Current file transfer state.
   * @param[in] message Status information during transfer.
   * @param[in] percent Completion percentage (0-100).
   */
  typedef std::function<void(DtFileTranState state, const char* message, uint8_t percent)>
      FileTransferCallback;

  /**
   * @brief Handler for device state change notifications.
   *
   * @param[in] state Current device state.
   * @param[in] message Description of state change.
   */
  typedef std::function<void(DtDeviceState state, const char* message)> DeviceStateChangedCallback;

  /**
   * @brief Handler for asynchronous raw data retrieval.
   *
   * @param[in] state Data transfer state.
   * @param[in] data_chunk Pointer to received data chunk.
   */
  typedef std::function<void(DtDataTranState state, DtDataChunk* data_chunk)> GetDataCallback;

  /**
   * @brief Handler for asynchronous raw data transfer progress.
   *
   * @param[in] state Data transfer state.
   * @param[in] percent Completion percentage (0-100).
   */
  typedef std::function<void(DtDataTranState state, uint8_t percent)> SetDataCallback;

 public:
  /**
   * @brief Represents a physical RGBD camera device.
   * @param[in] impl Native device handle.
   */
  explicit Device(dt_device_t* impl) : impl_(impl) {}

  Device(Device&& other) noexcept : impl_(other.impl_) { other.impl_ = nullptr; }

  Device& operator=(Device&& other) noexcept {
    if (this != &other) {
      dt_delete_device(impl_);
      impl_ = other.impl_;
      other.impl_ = nullptr;
    }
    return *this;
  }

  Device(const Device&) = delete;
  Device& operator=(const Device&) = delete;

  virtual ~Device() noexcept { dt_delete_device(impl_); }

  dt_device_t* GetImpl() const { return impl_; }

  /**
   * @brief Get device information.
   * @return Device information object.
   */
  std::shared_ptr<DeviceInfo> GetDeviceInfo() const { return std::make_shared<DeviceInfo>(impl_); }

  /**
   * @brief Get list of available sensors.
   * @return Sensor list object.
   */
  std::shared_ptr<SensorList> GetSensorList() const {
    dt_error* error = nullptr;
    auto list = dt_device_get_sensor_list(impl_, &error);
    Error::Handle(&error);
    return std::make_shared<SensorList>(list);
  }

  /**
   * @brief Get specific type of sensor.
   * @param[in] type Sensor type to retrieve.
   * @return Sensor instance or nullptr if unavailable.
   * @note Opens device connection automatically if closed.
   */
  std::shared_ptr<Sensor> GetSensor(DtSensorType type) const {
    dt_error* error = nullptr;
    auto sensor = dt_device_get_sensor(impl_, type, &error);
    Error::Handle(&error);
    return std::make_shared<Sensor>(sensor);
  }

  /**
   * @brief Get current device state.
   * @return Device state information.
   */
  DtDeviceState GetDeviceState() {
    dt_error* error = nullptr;
    auto state = dt_device_get_device_state(impl_, &error);
    Error::Handle(&error);
    return state;
  }

  /**
   * @brief Get camera calibration parameters.
   * @return Camera parameter list.
   */
  std::shared_ptr<CameraParamList> GetCalibrationCameraParamList() {
    dt_error* error = nullptr;
    auto param_list = dt_device_get_calibration_camera_param_list(impl_, &error);
    Error::Handle(&error);
    return std::make_shared<CameraParamList>(param_list);
  }

  /**
   * @brief Set integer-type device option.
   * @param[in] option_id Target option identifier.
   * @param[in] value Value to set.
   */
  void SetIntOption(DtOptionId option_id, int32_t value) const {
    dt_error* error = nullptr;
    dt_device_set_int_option(impl_, option_id, value, &error);
    Error::Handle(&error);
  }

  /**
   * @brief Set floating-point-type device option.
   * @param[in] option_id Target option identifier.
   * @param[in] value Value to set.
   */
  void SetFloatOption(DtOptionId option_id, float value) const {
    dt_error* error = nullptr;
    dt_device_set_float_option(impl_, option_id, value, &error);
    Error::Handle(&error);
  }

  /**
   * @brief Set boolean-type device option.
   * @param[in] option_id Target option identifier.
   * @param[in] value Value to set.
   */
  void SetBoolOption(DtOptionId option_id, bool value) const {
    dt_error* error = nullptr;
    dt_device_set_bool_option(impl_, option_id, value, &error);
    Error::Handle(&error);
  }

  /**
   * @brief Get integer-type device option value.
   * @param[in] option_id Target option identifier.
   * @return Current option value.
   */
  int32_t GetIntOption(DtOptionId option_id) const {
    dt_error* error = nullptr;
    auto value = dt_device_get_int_option(impl_, option_id, &error);
    Error::Handle(&error);
    return value;
  }

  /**
   * @brief Get floating-point-type device option value.
   * @param[in] option_id Target option identifier.
   * @return Current option value.
   */
  float GetFloatOption(DtOptionId option_id) const {
    dt_error* error = nullptr;
    auto value = dt_device_get_float_option(impl_, option_id, &error);
    Error::Handle(&error);
    return value;
  }

  /**
   * @brief Get boolean-type device option value.
   * @param[in] option_id Target option identifier.
   * @return Current option value.
   */
  bool GetBoolOption(DtOptionId option_id) const {
    dt_error* error = nullptr;
    auto value = dt_device_get_bool_option(impl_, option_id, &error);
    Error::Handle(&error);
    return value;
  }

  /**
   * @brief Get valid range for integer option.
   * @param[in] option_id Target option identifier.
   * @return Valid value range including defaults.
   */
  DtIntOptionRange GetIntOptionRange(DtOptionId option_id) const {
    dt_error* error = nullptr;
    auto range = dt_device_get_int_option_range(impl_, option_id, &error);
    Error::Handle(&error);
    return range;
  }

  /**
   * @brief Get valid range for floating-point option.
   * @param[in] option_id Target option identifier.
   * @return Valid value range including defaults.
   */
  DtFloatOptionRange GetFloatOptionRange(DtOptionId option_id) const {
    dt_error* error = nullptr;
    auto range = dt_device_get_float_option_range(impl_, option_id, &error);
    Error::Handle(&error);
    return range;
  }

  /**
   * @brief Get valid range for boolean option.
   * @param[in] option_id Target option identifier.
   * @return Valid value range including defaults.
   */
  DtBoolOptionRange GetBoolOptionRange(DtOptionId option_id) const {
    dt_error* error = nullptr;
    auto range = dt_device_get_bool_option_range(impl_, option_id, &error);
    Error::Handle(&error);
    return range;
  }

  /**
   * @brief Set structured binary data option.
   * @param[in] option_id Target option identifier.
   * @param[in] data Pointer to source data buffer.
   * @param[in] data_size Size of data in bytes.
   */
  void SetStructuredData(DtOptionId option_id, const uint8_t* data, uint32_t data_size) const {
    dt_error* error = nullptr;
    dt_device_set_structured_data(impl_, option_id, data, data_size, &error);
    Error::Handle(&error);
  }

  /**
   * @brief Get structured binary data option.
   * @param[in] option_id Target option identifier.
   * @param[out] data Buffer to receive data (pre-allocated).
   * @param[in,out] data_size Input: buffer capacity, Output: actual data size.
   */
  void GetStructuredData(DtOptionId option_id, uint8_t* data, uint32_t* data_size) const {
    dt_error* error = nullptr;
    dt_device_get_structured_data(impl_, option_id, data, data_size, &error);
    Error::Handle(&error);
  }

  /**
   * @brief Send file to device storage.
   * @param[in] src_file_path Local source file path.
   * @param[in] dst_file_path Device destination path.
   * @param[in] callback Progress reporting handler.
   * @param[in] async Execute asynchronously when true (default).
   */
  void SendFileToDevice(const char* src_file_path,
                        const char* dst_file_path,
                        FileTransferCallback callback,
                        bool async = true) {
    dt_error* error = nullptr;
    file_transfer_callback_ = callback;
    dt_device_send_file_to_device(impl_,
                                  src_file_path,
                                  dst_file_path,
                                  &Device::DtFileTransferCallback,
                                  async,
                                  this,
                                  &error);
    Error::Handle(&error);
  }

  /**
   * @brief Retrieve file from device storage.
   * @param[in] src_file_path Device source file path.
   * @param[in] dst_file_path Local destination path.
   * @param[in] callback Progress reporting handler.
   * @param[in] async Execute asynchronously when true (default).
   */
  void ReceiveFileFromDevice(const char* src_file_path,
                             const char* dst_file_path,
                             FileTransferCallback callback,
                             bool async = true) {
    dt_error* error = nullptr;
    file_transfer_callback_ = callback;
    dt_device_receive_file_from_device(impl_,
                                       src_file_path,
                                       dst_file_path,
                                       &Device::DtFileTransferCallback,
                                       async,
                                       this,
                                       &error);
    Error::Handle(&error);
  }

  /**
   * @brief Get count of supported options.
   * @return Number of available options.
   */
  int GetSupportedOptionCount() const {
    dt_error* error = nullptr;
    auto count = dt_device_get_supported_option_count(impl_, &error);
    Error::Handle(&error);
    return count;
  }

  /**
   * @brief Get option metadata by index.
   * @param[in] index Option index (0 to count-1).
   * @return Option descriptor structure.
   */
  DtOptionItem GetSupportedOption(uint32_t index) const {
    dt_error* error = nullptr;
    auto item = dt_device_get_supported_option_item(impl_, index, &error);
    Error::Handle(&error);
    return item;
  }

  /**
   * @brief Check option permission support.
   * @param[in] option_id Target option identifier.
   * @param[in] permission Permission type to verify.
   * @return true if permission supported, false otherwise.
   */
  bool IsOptionSupported(DtOptionId option_id, DtPermissionType permission) const {
    dt_error* error = nullptr;
    auto result = dt_device_is_option_supported(impl_, option_id, permission, &error);
    Error::Handle(&error);
    return result;
  }

  /**
   * @brief Check global timestamp support.
   * @return true if supported, false otherwise.
   */
  bool IsGlobalTimestampSupported() const {
    dt_error* error = nullptr;
    auto result = dt_device_is_global_timestamp_supported(impl_, &error);
    Error::Handle(&error);
    return result;
  }

  /**
   * @brief Enable/disable global timestamp.
   * @param[in] enable true to enable, false to disable.
   */
  void EnableGlobalTimestamp(bool enable) {
    dt_error* error = nullptr;
    dt_device_enable_global_timestamp(impl_, enable, &error);
    Error::Handle(&error);
  }

  /**
   * @brief Write data to device flash memory.
   * @param[in] offset Flash memory offset.
   * @param[in] data Source data buffer.
   * @param[in] data_size Data size in bytes.
   * @param[in] callback Progress reporting handler.
   * @param[in] async Execute asynchronously when true (default false).
   * @warning Flash operations may cause device instability if interrupted.
   */
  void WriteFlash(uint32_t offset,
                  const void* data,
                  uint32_t data_size,
                  SetDataCallback callback,
                  bool async = false) {
    dt_error* error = nullptr;
    set_data_callback_ = callback;
    dt_device_write_flash(impl_,
                          offset,
                          data,
                          data_size,
                          &Device::DtSetDataCallback,
                          async,
                          this,
                          &error);
    Error::Handle(&error);
  }

  /**
   * @brief Read data from device flash memory.
   * @param[in] offset Flash memory offset.
   * @param[in] data_size Data size to read.
   * @param[in] callback Data reception handler.
   * @param[in] async Execute asynchronously when true (default false).
   */
  void ReadFlash(uint32_t offset,
                 uint32_t data_size,
                 GetDataCallback callback,
                 bool async = false) {
    dt_error* error = nullptr;
    get_data_callback_ = callback;
    dt_device_read_flash(impl_, offset, data_size, &Device::DtGetDataCallback, async, this, &error);
    Error::Handle(&error);
  }

  /**
   * @brief Update firmware from file.
   * @param[in] file_path Firmware file path.
   * @param[in] callback Progress reporting handler.
   * @param[in] async Execute asynchronously when true (default).
   * @warning Power interruption during update may brick the device.
   */
  void UpdateFirmware(const char* file_path, DeviceFwUpdateCallback callback, bool async = true) {
    dt_error* error = nullptr;
    fw_update_callback_ = callback;
    dt_device_update_firmware(impl_,
                              file_path,
                              &Device::DtFirmwareUpdateCallback,
                              async,
                              this,
                              &error);
    Error::Handle(&error);
  }

  /**
   * @brief Update firmware from memory buffer.
   * @param[in] firmware_data Firmware binary data.
   * @param[in] firmware_data_size Data size in bytes.
   * @param[in] callback Progress reporting handler.
   * @param[in] async Execute asynchronously when true (default).
   */
  void UpdateFirmwareFromData(const uint8_t* firmware_data,
                              uint32_t firmware_data_size,
                              DeviceFwUpdateCallback callback,
                              bool async = true) {
    dt_error* error = nullptr;
    fw_update_callback_ = callback;
    dt_device_update_firmware_from_data(impl_,
                                        firmware_data,
                                        firmware_data_size,
                                        &Device::DtFirmwareUpdateCallback,
                                        async,
                                        this,
                                        &error);
    Error::Handle(&error);
  }

  /**
   * @brief Register device state change handler.
   * @param[in] callback State change handler.
   * @note Handles events like thermal throttling or unexpected disconnections.
   */
  void SetDeviceStateChangedCallback(DeviceStateChangedCallback callback) {
    dt_error* error = nullptr;
    device_state_change_callback_ = callback;
    dt_device_set_state_changed_callback(impl_,
                                         &Device::DtDeviceStateChangedCallback,
                                         this,
                                         &error);
    Error::Handle(&error);
  }
  /**
   * @brief Reboot the device.
   * @warning Device disconnects during reboot. Discard Device object
   *          and reacquire after reconnection.
   */
  void Reboot() const {
    dt_error* error = nullptr;
    dt_device_reboot(impl_, &error);
    Error::Handle(&error);
  }

  /**
   * @brief Enable/disable device heartbeat.
   * @param[in] enable true to enable heartbeat monitoring.
   * @note Heartbeat prevents device auto-disconnection. SDK emits
   *       signals every 3 seconds when enabled.
   */
  void EnableHeartbeat(bool enable) const {
    dt_error* error = nullptr;
    dt_device_enable_heartbeat(impl_, enable, &error);
    Error::Handle(&error);
  }

  /**
   * @brief Synchronize device clock with host system.
   * @note Causes timestamp discontinuity. Recommended every 60 minutes
   *       to prevent clock drift accumulation.
   */
  void TimerSyncWithHost() const {
    dt_error* error = nullptr;
    dt_device_timer_sync_with_host(impl_, &error);
    Error::Handle(&error);
  }

  /**
   * @brief Low-level device communication for debug/factory use.
   * @param[in] send_data Data to transmit.
   * @param[in] send_data_size Transmission data size.
   * @param[out] receive_data Buffer for response (min 1024 bytes).
   * @param[in,out] receive_data_size Input: buffer size, Output: actual received size.
   * @warning Vendor-specific protocol. Use may void warranty.
   */
  void SendAndReceiveData(const uint8_t* send_data,
                          uint32_t send_data_size,
                          uint8_t* receive_data,
                          uint32_t* receive_data_size) const {
    dt_error* error = nullptr;
    dt_device_send_and_receive_data(impl_,
                                    send_data,
                                    send_data_size,
                                    receive_data,
                                    receive_data_size,
                                    &error);
    Error::Handle(&error);
  }

 private:
  // Static callback adapters remain unchanged
  static void DtDeviceStateChangedCallback(DtDeviceState state,
                                           const char* message,
                                           void* user_data) {
    auto device = static_cast<Device*>(user_data);
    if (device && device->device_state_change_callback_) {
      device->device_state_change_callback_(state, message);
    }
  }

  static void DtFirmwareUpdateCallback(dt_fw_update_state state,
                                       const char* message,
                                       uint8_t percent,
                                       void* user_data) {
    auto device = static_cast<Device*>(user_data);
    if (device != nullptr && device->fw_update_callback_ != nullptr) {
      device->fw_update_callback_(state, message, percent);
    }
  }

  static void DtFileTransferCallback(dt_file_tran_state state,
                                     const char* message,
                                     uint8_t percent,
                                     void* user_data) {
    auto device = static_cast<Device*>(user_data);
    if (device != nullptr && device->file_transfer_callback_ != nullptr) {
      device->file_transfer_callback_(state, message, percent);
    }
  }

  static void DtSetDataCallback(dt_data_tran_state state, uint8_t percent, void* user_data) {
    auto device = static_cast<Device*>(user_data);
    if (device != nullptr && device->set_data_callback_ != nullptr) {
      device->set_data_callback_(state, percent);
    }
  }

  static void DtGetDataCallback(dt_data_tran_state state,
                                dt_data_chunk* data_chunk,
                                void* user_data) {
    auto device = static_cast<Device*>(user_data);
    if (device != nullptr && device->get_data_callback_ != nullptr) {
      device->get_data_callback_(state, data_chunk);
    }
  }

 protected:
  dt_device* impl_ = nullptr;
  DeviceStateChangedCallback device_state_change_callback_;
  DeviceFwUpdateCallback fw_update_callback_;
  FileTransferCallback file_transfer_callback_;
  GetDataCallback get_data_callback_;
  SetDataCallback set_data_callback_;
};

/**
 * @brief A class describing device information, representing the name, id, serial number and other
 * basic information of an RGBD camera.
 */
class DeviceInfo {
 public:
  /**
   * @brief Construct from native device handle.
   * @param[in] impl Native device handle.
   */
  explicit DeviceInfo(dt_device_t* impl) : impl_(impl) {}

  ~DeviceInfo() noexcept = default;

  /**
   * @brief Get device model name.
   * @return Device name string.
   */
  const char* GetName() const {
    dt_error* error = nullptr;
    const char* name = dt_device_get_device_info(impl_, kDtCameraInfoName, &error);
    Error::Handle(&error);
    return name;
  }

  /**
   * @brief Get product ID (PID).
   * @return Product ID string.
   */
  const char* GetPid() const {
    dt_error* error = nullptr;
    const char* pid = dt_device_get_device_info(impl_, kDtCameraInfoProductId, &error);
    Error::Handle(&error);
    return pid;
  }

  /**
   * @brief Get vendor ID (VID).
   * @return Vendor ID string.
   */
  const char* GetVid() const {
    dt_error* error = nullptr;
    const char* vid = dt_device_get_device_info(impl_, kDtCameraInfoVendorId, &error);
    Error::Handle(&error);
    return vid;
  }

  /**
   * @brief Get unique system-assigned ID.
   * @return Unique identifier string.
   */
  const char* GetUid() const {
    dt_error* error = nullptr;
    const char* uid = dt_device_get_device_info(impl_, kDtCameraInfoUniqueId, &error);
    Error::Handle(&error);
    return uid;
  }

  /**
   * @brief Get hardware serial number.
   * @return Serial number string.
   */
  const char* GetSerialNumber() const {
    dt_error* error = nullptr;
    const char* sn = dt_device_get_device_info(impl_, kDtCameraInfoSerialNumber, &error);
    Error::Handle(&error);
    return sn;
  }

  /**
   * @brief Get firmware version.
   * @return Firmware version string.
   */
  const char* GetFirmwareVersion() const {
    dt_error* error = nullptr;
    const char* version = dt_device_get_device_info(impl_, kDtCameraInfoFirmwareVersion, &error);
    Error::Handle(&error);
    return version;
  }

  /**
   * @brief Get connection interface type.
   * @return Interface type (e.g., "USB3.2", "Ethernet").
   */
  const char* GetConnectionType() const {
    dt_error* error = nullptr;
    const char* type = dt_device_get_device_info(impl_, kDtCameraInfoConnectionType, &error);
    Error::Handle(&error);
    return type;
  }

  /**
   * @brief Get hardware version.
   * @return Hardware version string.
   */
  const char* GetHardwareVersion() const {
    dt_error* error = nullptr;
    const char* version = dt_device_get_device_info(impl_, kDtCameraInfoHardwareVersion, &error);
    Error::Handle(&error);
    return version;
  }

  /**
   * @brief Get minimum supported SDK version.
   * @return Minimum SDK version string.
   */
  const char* GetSupportedMinSdkVersion() const {
    dt_error* error = nullptr;
    const char* version = dt_device_get_device_info(impl_,
                                                    kDtCameraInfoSupportedMinSdkVersion,
                                                    &error);
    Error::Handle(&error);
    return version;
  }

  /**
   * @brief Get ASIC/chipset name.
   * @return Chipset name string.
   */
  const char* GetAsicName() const {
    dt_error* error = nullptr;
    const char* name = dt_device_get_device_info(impl_, kDtCameraInfoAsicName, &error);
    Error::Handle(&error);
    return name;
  }

  /**
   * @brief Get network IP address.
   * @return IP address string ("0.0.0.0" for non-network devices).
   */
  const char* GetIpAddress() const {
    dt_error* error = nullptr;
    const char* ip = dt_device_get_device_info(impl_, kDtCameraInfoIpAddress, &error);
    Error::Handle(&error);
    return ip;
  }

  /**
   * @brief Get network MAC address.
   * @return MAC address string ("0:0:0:0:0:0" for non-network devices).
   */
  const char* GetMacAddress() const {
    dt_error* error = nullptr;
    const char* mac = dt_device_get_device_info(impl_, kDtCameraInfoMacAddress, &error);
    Error::Handle(&error);
    return mac;
  }

 private:
  dt_device_t* impl_ = nullptr;
};

/**
 * @brief Class representing a list of devices.
 */
class DeviceList {
 public:
  /**
   * @brief Construct from native device list handle.
   * @param[in] impl Native device list handle.
   */
  explicit DeviceList(dt_device_list_t* impl) : impl_(impl) {}

  /**
   * @brief Release device list resources.
   */
  ~DeviceList() noexcept {
    if (impl_) {
      dt_delete_device_list(impl_);
    }
  }

  /**
   * @brief Get number of devices in list.
   * @return Device count.
   */
  uint32_t GetCount() const {
    dt_error* error = nullptr;
    auto count = dt_device_list_get_count(impl_, &error);
    Error::Handle(&error);
    return count;
  }

  /**
   * @brief Get product ID (PID) by index.
   * @param[in] index Device position in list (0-based).
   * @return Product ID string.
   */
  const char* GetPid(uint32_t index) const {
    dt_error* error = nullptr;
    auto pid = dt_device_list_get_device_info(impl_, index, kDtCameraInfoProductId, &error);
    Error::Handle(&error);
    return pid;
  }

  /**
   * @brief Get vendor ID (VID) by index.
   * @param[in] index Device position in list (0-based).
   * @return Vendor ID string.
   */
  const char* GetVid(uint32_t index) const {
    dt_error* error = nullptr;
    auto vid = dt_device_list_get_device_info(impl_, index, kDtCameraInfoVendorId, &error);
    Error::Handle(&error);
    return vid;
  }

  /**
   * @brief Get unique ID by index.
   * @param[in] index Device position in list (0-based).
   * @return Unique identifier string.
   */
  const char* GetUid(uint32_t index) const {
    dt_error* error = nullptr;
    auto uid = dt_device_list_get_device_info(impl_, index, kDtCameraInfoUniqueId, &error);
    Error::Handle(&error);
    return uid;
  }

  /**
   * @brief Get serial number by index.
   * @param[in] index Device position in list (0-based).
   * @return Hardware serial number string.
   */
  const char* GetSerialNumber(uint32_t index) const {
    dt_error* error = nullptr;
    auto sn = dt_device_list_get_device_info(impl_, index, kDtCameraInfoSerialNumber, &error);
    Error::Handle(&error);
    return sn;
  }

  /**
   * @brief Get device name by index
   * @param[in] index Device position in list (0-based)
   * @return Device name string
   */
  const char* GetName(uint32_t index) const {
    dt_error* error = nullptr;
    auto name = dt_device_list_get_device_info(impl_, index, kDtCameraInfoName, &error);
    Error::Handle(&error);
    return name;
  }

  /**
   * @brief Get connection type by index.
   * @param[in] index Device position in list (0-based).
   * @return Interface type string.
   */
  const char* GetConnectionType(uint32_t index) const {
    dt_error* error = nullptr;
    auto type = dt_device_list_get_device_info(impl_, index, kDtCameraInfoConnectionType, &error);
    Error::Handle(&error);
    return type;
  }

  /**
   * @brief Get IP address by index.
   * @param[in] index Device position in list (0-based).
   * @return IP address string ("0.0.0.0" for non-network devices).
   */
  const char* GetIpAddress(uint32_t index) const {
    dt_error* error = nullptr;
    auto ip = dt_device_list_get_device_info(impl_, index, kDtCameraInfoIpAddress, &error);
    Error::Handle(&error);
    return ip;
  }

  /**
   * @brief Get MAC address by index.
   * @param[in] index Device position in list (0-based).
   * @return MAC address string ("0:0:0:0:0:0" for non-network devices).
   */
  const char* GetLocalMacAddress(uint32_t index) const {
    dt_error* error = nullptr;
    auto mac = dt_device_list_get_device_info(impl_, index, kDtCameraInfoMacAddress, &error);
    Error::Handle(&error);
    return mac;
  }

  /**
   * @brief Create Device instance from list index.
   * @param[in] index Device position in list (0-based).
   * @return Shared pointer to Device instance.
   * @throws Exception if device already acquired.
   */
  std::shared_ptr<Device> GetDevice(uint32_t index) const {
    dt_error* error = nullptr;
    auto device = dt_device_list_get_device(impl_, index, &error);
    Error::Handle(&error);
    return std::make_shared<Device>(device);
  }

  /**
   * @brief Get the device object with the specified serial number.
   *
   * @attention If the device has already been acquired and created elsewhere, repeated
   * acquisition will throw an exception.
   *
   * @param[in] serial_number the serial number of the device to create.
   * @return std::shared_ptr<Device> the device object.
   */
  std::shared_ptr<Device> GetDeviceBySN(const char* serial_number) const {
    dt_error* error = nullptr;
    auto device = dt_device_list_get_device_by_serial_number(impl_, serial_number, &error);
    Error::Handle(&error);
    return std::make_shared<Device>(device);
  }

  /**
   * @brief Get the specified device object from the device list by uid.
   * @brief On Linux platform, for usb device, the uid of the device is composed of bus-port-dev,
   * for example 1-1.2-1. But the SDK will remove the dev number and only keep the bus-port as the
   * uid to create the device, for example 1-1.2, so that we can create a device connected to the
   * specified USB port. Similarly, users can also directly pass in bus-port as uid to create
   * device.
   *
   * @attention If the device has been acquired and created elsewhere, repeated acquisition will
   * throw an exception.
   *
   * @param[in] uid The uid of the device to be created.
   * @return std::shared_ptr<Device> returns the device object.
   */
  std::shared_ptr<Device> GetDeviceByUid(const char* uid) const {
    dt_error* error = nullptr;
    auto device = dt_device_list_get_device_by_uid(impl_, uid, &error);
    Error::Handle(&error);
    return std::make_shared<Device>(device);
  }

 private:
  dt_device_list_t* impl_ = nullptr;
};

/**
 * @brief Class representing a list of camera parameters.
 */
class CameraParamList {
 public:
  explicit CameraParamList(dt_camera_param_list_t* impl) : impl_(impl) {}
  ~CameraParamList() noexcept { dt_delete_camera_param_list(impl_); }

  /**
   * @brief Get the number of camera parameters in the list.
   *
   * @return uint32_t the number of camera parameters in the list.
   */
  uint32_t GetCount() {
    dt_error* error = nullptr;
    auto count = dt_camera_param_list_get_count(impl_, &error);
    Error::Handle(&error);
    return count;
  }

  /**
   * @brief Get the camera parameters for the specified index.
   *
   * @param[in] index the index of the parameter group.
   * @return DtCameraParam the corresponding group parameters.
   */
  DtCameraParam GetCameraParam(uint32_t index) {
    dt_error* error = nullptr;
    auto param = dt_camera_param_list_get_param(impl_, index, &error);
    Error::Handle(&error);
    return param;
  }

 private:
  dt_camera_param_list_t* impl_ = nullptr;
};

}  // namespace dt

#endif  // DEPTRUM_HPP_DT_DEVICE_HPP_
