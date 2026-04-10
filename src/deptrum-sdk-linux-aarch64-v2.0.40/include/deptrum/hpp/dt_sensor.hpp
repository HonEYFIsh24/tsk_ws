// Copyright (c) Deptrum Inc. All Rights Reserved.

/**
 * @file dt_sensor.hpp
 * @brief Defines sensor-related types for stream configuration, operation, and property management.
 */

#ifndef DEPTRUM_HPP_DT_SENSOR_HPP_
#define DEPTRUM_HPP_DT_SENSOR_HPP_

#include <functional>
#include <memory>
#include <vector>
#include "deptrum/h/dt_filter.h"
#include "deptrum/h/dt_sensor.h"
#include "deptrum/hpp/dt_filter.hpp"
#include "dt_device.hpp"
#include "dt_error.hpp"
#include "dt_frame.hpp"
#include "dt_stream_profile.hpp"
#include "dt_types.hpp"

namespace dt {

/**
 * @class Sensor
 * @brief Represents a device sensor for stream configuration and data acquisition.
 */
class Sensor {
 public:
  /**
   * @brief Callback function for frame data.
   * @param[in] frame Frame data received from the sensor.
   */
  typedef std::function<void(std::shared_ptr<Frame> frame)> FrameCallback;

  /**
   * @brief Constructs a Sensor instance from a native handle.
   * @param[in] impl Native sensor handle.
   */
  explicit Sensor(dt_sensor_t* impl) : impl_(impl) {}

  /**
   * @brief Move constructor for Sensor.
   * @param[in] sensor Source sensor to move from.
   */
  Sensor(Sensor&& sensor) noexcept : impl_(sensor.impl_) { sensor.impl_ = nullptr; }

  /**
   * @brief Move assignment operator for Sensor.
   * @param[in] sensor Source sensor to move from.
   * @return Reference to this sensor instance.
   */
  Sensor& operator=(Sensor&& sensor) noexcept {
    if (this != &sensor) {
      dt_delete_sensor(impl_);
      impl_ = sensor.impl_;
      sensor.impl_ = nullptr;
    }
    return *this;
  }

  Sensor(const Sensor& sensor) = delete;
  Sensor& operator=(const Sensor& sensor) = delete;

  /**
   * @brief Destructor for Sensor.
   */
  virtual ~Sensor() noexcept { dt_delete_sensor(impl_); }

  /**
   * @brief Retrieves the type of the sensor.
   * @return Sensor type enumeration value.
   */
  DtSensorType GetType() const {
    dt_error* error = nullptr;
    auto type = dt_sensor_get_type(impl_, &error);
    Error::Handle(&error);
    return type;
  }

  /**
   * @brief Gets the list of available stream profiles.
   * @return Shared pointer to the stream profile list.
   */
  std::shared_ptr<StreamProfileList> GetStreamProfileList() const {
    dt_error* error = nullptr;
    auto list = dt_sensor_get_stream_profile_list(impl_, &error);
    Error::Handle(&error);
    return std::make_shared<StreamProfileList>(list);
  }

  /**
   * @brief Creates a list of recommended filters for this sensor.
   * @return Vector of shared pointers to recommended filters.
   */
  std::vector<std::shared_ptr<Filter>> CreateRecommendedFilters() const {
    dt_error* error = nullptr;
    auto list = dt_sensor_create_recommended_filter_list(impl_, &error);
    Error::Handle(&error);
    auto filter_count = dt_filter_list_get_count(list, &error);

    std::vector<std::shared_ptr<Filter>> filters;
    for (uint32_t i = 0; i < filter_count; i++) {
      auto filter_impl = dt_filter_list_get_filter(list, i, &error);
      Error::Handle(&error);
      filters.push_back(std::make_shared<Filter>(filter_impl));
    }
    dt_delete_filter_list(list);
    return filters;
  }

  /**
   * @brief Starts streaming data with a specific configuration.
   * @param[in] stream_profile Stream configuration to use.
   * @param[in] callback Callback function for frame data reception.
   */
  void Start(std::shared_ptr<StreamProfile> stream_profile, FrameCallback callback) {
    dt_error* error = nullptr;
    callback_ = std::move(callback);
    dt_sensor_start(impl_,
                    const_cast<dt_stream_profile_t*>(stream_profile->GetImpl()),
                    &Sensor::DtFrameCallback,
                    this,
                    &error);
    Error::Handle(&error);
  }

  /**
   * @brief Stops the active data stream.
   */
  void Stop() const {
    dt_error* error = nullptr;
    dt_sensor_stop(impl_, &error);
    Error::Handle(&error);
  }

  /**
   * @brief Dynamically switches to a different stream configuration.
   * @param[in] stream_profile New stream configuration to apply.
   */
  void SwitchProfile(std::shared_ptr<StreamProfile> stream_profile) {
    dt_error* error = nullptr;
    dt_sensor_switch_profile(impl_,
                             const_cast<dt_stream_profile_t*>(stream_profile->GetImpl()),
                             &error);
    Error::Handle(&error);
  }

 private:
  /**
   * @brief Internal callback handler for frame data.
   * @param[in] frame Native frame handle.
   * @param[in] user_data User context pointer (Sensor instance).
   */
  static void DtFrameCallback(dt_frame* frame, void* user_data) {
    auto sensor = static_cast<Sensor*>(user_data);
    if (sensor != nullptr && sensor->callback_ != nullptr) {
      sensor->callback_(std::make_shared<Frame>(frame));
    }
  }

 private:
  dt_sensor_t* impl_ = nullptr;
  FrameCallback callback_;
};

/**
 * @class SensorList
 * @brief Represents a collection of sensors associated with a device.
 */
class SensorList {
 public:
  /**
   * @brief Constructs a SensorList from a native handle.
   * @param[in] impl Native sensor list handle.
   */
  explicit SensorList(dt_sensor_list_t* impl) : impl_(impl) {}

  /**
   * @brief Destructor for SensorList.
   */
  ~SensorList() noexcept { dt_delete_sensor_list(impl_); }

  /**
   * @brief Gets the number of sensors in the list.
   * @return Count of available sensors.
   */
  uint32_t GetCount() const {
    dt_error* error = nullptr;
    auto count = dt_sensor_list_get_count(impl_, &error);
    Error::Handle(&error);
    return count;
  }

  /**
   * @brief Retrieves the type of a specific sensor.
   * @param[in] index Zero-based sensor index.
   * @return Sensor type enumeration value.
   */
  DtSensorType GetSensorType(uint32_t index) const {
    dt_error* error = nullptr;
    auto type = dt_sensor_list_get_sensor_type(impl_, index, &error);
    Error::Handle(&error);
    return type;
  }

  /**
   * @brief Retrieves a sensor by its index.
   * @param[in] index Zero-based sensor index (valid range: [0, count-1]).
   * @return Shared pointer to the requested sensor.
   * @throws dt::Error if index is out of range.
   */
  std::shared_ptr<Sensor> GetSensor(uint32_t index) const {
    dt_error* error = nullptr;
    auto sensor = dt_sensor_list_get_sensor(impl_, index, &error);
    Error::Handle(&error);
    return std::make_shared<Sensor>(sensor);
  }

  /**
   * @brief Retrieves a sensor by its type.
   * @param[in] sensor_type Type of sensor to retrieve.
   * @return Shared pointer to the requested sensor, or nullptr if not found.
   */
  std::shared_ptr<Sensor> GetSensor(DtSensorType sensor_type) const {
    dt_error* error = nullptr;
    auto sensor = dt_sensor_list_get_sensor_by_type(impl_, sensor_type, &error);
    Error::Handle(&error);
    return std::make_shared<Sensor>(sensor);
  }

 private:
  dt_sensor_list_t* impl_ = nullptr;
};

}  // namespace dt

#endif  // DEPTRUM_HPP_DT_SENSOR_HPP_
