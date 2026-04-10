// Copyright (c) Deptrum Inc. All Rights Reserved.

/**
 * @file dt_context.hpp
 * @brief SDK context management class providing entry point to core functionality.
 *
 * @details Manages device enumeration, connection lifecycle, callback handling,
 * and multi-device synchronization. Acts as primary access point to SDK resources.
 */

#ifndef DEPTRUM_HPP_DT_CONTEXT_HPP_
#define DEPTRUM_HPP_DT_CONTEXT_HPP_

#include <functional>
#include <memory>
#include "deptrum/h/dt_context.h"
#include "dt_error.hpp"
#include "dt_types.hpp"

namespace dt {

// Forward declarations
class Device;
class DeviceInfo;
class DeviceList;

/**
 * @brief Central management class for SDK runtime environment.
 */
class Context {
 public:
  /**
   * @brief Device change notification callback signature.
   *
   * @param removed_list Container of devices removed from the system.
   * @param added_list Container of devices newly detected in the system.
   */
  typedef std::function<void(std::shared_ptr<DeviceList> removed_list,
                             std::shared_ptr<DeviceList> added_list)>
      DeviceChangedCallback;

 public:
  /**
   * @brief Initializes SDK runtime environment.
   *
   * @param[in] config_path Path to configuration file. Empty path loads
   *            default configuration.
   */
  explicit Context(const char* config_path = "") {
    dt_error* error = nullptr;
    impl_ = dt_create_context_with_config(config_path, &error);
    Error::Handle(&error);
  }

  /**
   * @brief Releases all SDK resources and terminates runtime environment.
   */
  ~Context() noexcept { dt_delete_context(impl_); }

  /**
   * @brief Enumerates currently available devices.
   *
   * @return std::shared_ptr<DeviceList> Container of detected device references.
   */
  std::shared_ptr<DeviceList> QueryDeviceList() const {
    dt_error* error = nullptr;
    auto list = dt_query_device_list(impl_, &error);
    Error::Handle(&error);
    return std::make_shared<DeviceList>(list);
  }

  /**
   * @brief Controls network device discovery mechanism.
   *
   * @param[in] enable Enable state for network device discovery.
   *
   * @note Discovery uses GVCP protocol. Devices outside host subnet may be
   *       detected but remain non-connectable.
   */
  void EnableNetDeviceEnumeration(bool enable) const {
    dt_error* error = nullptr;
    dt_enable_net_device_enumeration(impl_, enable, &error);
    Error::Handle(&error);
  }

  /**
   * @brief Establishes connection to network device.
   *
   * @param[in] address IPv4 address of target device (e.g., "192.168.1.10")
   * @param[in] port Network port (currently only 8080 supported)
   * @return std::shared_ptr<Device> Reference to created device instance.
   */
  std::shared_ptr<Device> CreateNetDevice(const char* address, uint16_t port) const {
    dt_error* error = nullptr;
    auto device = dt_create_net_device(impl_, address, port, &error);
    Error::Handle(&error);
    return std::make_shared<Device>(device);
  }

  /**
   * @brief Registers device connection/disconnection callback.
   *
   * @param[in] callback Function to trigger on device topology changes.
   *
   * @note Supports multiple callbacks. Subsequent calls append to callback list.
   */
  void SetDeviceChangedCallback(DeviceChangedCallback callback) {
    device_changed_callback_ = callback;
    dt_error* error = nullptr;
    dt_set_device_changed_callback(impl_, &Context::DtDeviceChangedCallback, this, &error);
    Error::Handle(&error);
  }

  /**
   * @brief Synchronizes device clocks with host system.
   *
   * @param[in] repeat_interval_msec Synchronization interval (ms). Zero value executes single sync.
   */
  void EnableDeviceClockSync(uint64_t repeat_interval_msec) const {
    dt_error* error = nullptr;
    dt_enable_device_clock_sync(impl_, repeat_interval_msec, &error);
    Error::Handle(&error);
  }

  /**
   * @brief Configures UVC backend implementation (Linux only).
   *
   * @param[in] type Backend implementation type.
   *
   * @attention Configuration applies to subsequently created devices.
   */
  void SetUvcBackendType(DtUvcBackendType type) const {
    dt_error* error = nullptr;
    dt_set_uvc_backend_type(impl_, type, &error);
    Error::Handle(&error);
  }

 private:
  // Internal callback adapter
  static void DtDeviceChangedCallback(dt_device_list* removed_list,
                                      dt_device_list* added_list,
                                      void* user_data) {
    auto ctx = static_cast<Context*>(user_data);
    if (ctx != nullptr && ctx->device_changed_callback_ != nullptr) {
      auto removed = std::make_shared<DeviceList>(removed_list);
      auto added = std::make_shared<DeviceList>(added_list);
      ctx->device_changed_callback_(removed, added);
    }
  }

 private:
  dt_context* impl_ = nullptr;
  DeviceChangedCallback device_changed_callback_;
};

}  // namespace dt

#endif  // DEPTRUM_HPP_DT_CONTEXT_HPP_