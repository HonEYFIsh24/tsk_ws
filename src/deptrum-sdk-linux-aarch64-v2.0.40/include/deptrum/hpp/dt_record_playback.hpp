// Copyright (c) Deptrum Inc. All Rights Reserved.

/**
 * @file dt_record_playback.hpp
 * @brief Provides interfaces for recording and playback devices including data streaming
 * operations.
 */

#ifndef DEPTRUM_HPP_DT_RECORD_PLAYBACK_HPP_
#define DEPTRUM_HPP_DT_RECORD_PLAYBACK_HPP_

#include "deptrum/h/dt_record_playback.h"
#include "deptrum/hpp/dt_device.hpp"
#include "dt_error.hpp"
#include "dt_types.hpp"

namespace dt {

typedef std::function<void(DtPlaybackStatus status)> PlaybackStatusChangeCallback;

/**
 * @class RecordDevice
 * @brief Records device data to a specified file.
 */
class RecordDevice {
 public:
  /**
   * @brief Constructs a RecordDevice instance.
   * @param[in] device Device instance to record from.
   * @param[in] file Output file path for recording.
   * @param[in] compression_enabled Enable compression for recorded data (default: true).
   */
  explicit RecordDevice(std::shared_ptr<Device> device,
                        const std::string& file,
                        bool compression_enabled = true) {
    dt_error* error = nullptr;
    impl_ = dt_create_record_device(device->GetImpl(), file.c_str(), compression_enabled, &error);
    Error::Handle(&error);
  }

  /**
   * @brief Destructor for RecordDevice.
   */
  virtual ~RecordDevice() noexcept { dt_delete_record_device(impl_); }

  RecordDevice(RecordDevice&& other) noexcept {
    if (this != &other) {
      impl_ = other.impl_;
      other.impl_ = nullptr;
    }
  }

  RecordDevice& operator=(RecordDevice&& other) noexcept {
    if (this != &other) {
      impl_ = other.impl_;
      other.impl_ = nullptr;
    }
    return *this;
  }

  RecordDevice(const RecordDevice&) = delete;
  RecordDevice& operator=(const RecordDevice&) = delete;

  /**
   * @brief Pauses the recording process.
   */
  void Pause() {
    dt_error* error = nullptr;
    dt_record_device_pause(impl_, &error);
    Error::Handle(&error);
  }

  /**
   * @brief Resumes a paused recording.
   */
  void Resume() {
    dt_error* error = nullptr;
    dt_record_device_resume(impl_, &error);
    Error::Handle(&error);
  }

 private:
  dt_record_device_t* impl_ = nullptr;
};

/**
 * @class PlaybackDevice
 * @brief Playback device for recorded data files.
 */
class PlaybackDevice : public Device {
 public:
  /**
   * @brief Constructs a PlaybackDevice instance.
   * @param[in] file Input file path for playback.
   */
  explicit PlaybackDevice(const std::string& file) : Device(nullptr) {
    dt_error* error = nullptr;
    impl_ = dt_create_playback_device(file.c_str(), &error);
    Error::Handle(&error);
  }

  ~PlaybackDevice() noexcept override = default;

  PlaybackDevice(PlaybackDevice&& other) noexcept : Device(std::move(other)) {}

  PlaybackDevice& operator=(PlaybackDevice&& other) noexcept {
    Device::operator=(std::move(other));
    return *this;
  }

  PlaybackDevice(const PlaybackDevice&) = delete;
  PlaybackDevice& operator=(const PlaybackDevice&) = delete;

  /**
   * @brief Pauses the playback stream.
   */
  void Pause() {
    dt_error* error = nullptr;
    dt_playback_device_pause(impl_, &error);
    Error::Handle(&error);
  }

  /**
   * @brief Resumes paused playback.
   */
  void Resume() {
    dt_error* error = nullptr;
    dt_playback_device_resume(impl_, &error);
    Error::Handle(&error);
  }

  /**
   * @brief Seeks to specific timestamp in playback.
   * @param[in] timestamp Target timestamp in milliseconds.
   */
  void Seek(const int64_t timestamp) {
    dt_error* error = nullptr;
    dt_playback_device_seek(impl_, timestamp, &error);
    Error::Handle(&error);
  }

  /**
   * @brief Sets playback speed multiplier.
   * @param[in] rate Playback rate multiplier (1.0 = normal speed).
   */
  void SetPlaybackRate(const float rate) {
    dt_error* error = nullptr;
    dt_playback_device_set_playback_rate(impl_, rate, &error);
    Error::Handle(&error);
  }

  /**
   * @brief Registers callback for playback state changes.
   * @param[in] callback Callback function to register.
   */
  void SetPlaybackStatusChangeCallback(PlaybackStatusChangeCallback callback) {
    callback_ = callback;
    dt_error* error = nullptr;
    dt_playback_device_set_playback_status_changed_callback(
        impl_,
        &PlaybackDevice::DtPlaybackStatusCallback,
        this,
        &error);
    Error::Handle(&error);
  }

  /**
   * @brief Retrieves current playback state.
   * @return Current playback status value.
   */
  DtPlaybackStatus GetPlaybackStatus() const {
    dt_error* error = nullptr;
    DtPlaybackStatus status = dt_playback_device_get_current_playback_status(impl_, &error);
    Error::Handle(&error);
    return status;
  }

  /**
   * @brief Gets current playback position.
   * @return Current playback position in milliseconds.
   */
  uint64_t GetPosition() const {
    dt_error* error = nullptr;
    uint64_t position = dt_playback_device_get_position(impl_, &error);
    Error::Handle(&error);
    return position;
  }

  /**
   * @brief Gets total playback duration.
   * @return Total playback duration in milliseconds.
   */
  uint64_t GetDuration() const {
    dt_error* error = nullptr;
    uint64_t duration = dt_playback_device_get_duration(impl_, &error);
    Error::Handle(&error);
    return duration;
  }

 private:
  static void DtPlaybackStatusCallback(DtPlaybackStatus status, void* user_data) {
    auto* playback_device = static_cast<PlaybackDevice*>(user_data);
    if (playback_device != nullptr && playback_device->callback_ != nullptr) {
      playback_device->callback_(status);
    }
  }

 private:
  PlaybackStatusChangeCallback callback_;
};

}  // namespace dt

#endif  // DEPTRUM_HPP_DT_RECORD_PLAYBACK_HPP_
