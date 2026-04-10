// Copyright (c) Deptrum Inc. All Rights Reserved.

/**
 * @file dt_pipeline.hpp
 * @brief Advanced API for synchronized stream and frame operations
 */

#ifndef DEPTRUM_HPP_DT_PIPELINE_HPP_
#define DEPTRUM_HPP_DT_PIPELINE_HPP_

#include <functional>
#include <memory>
#include "deptrum/h/dt_config.h"
#include "deptrum/h/dt_pipeline.h"
#include "deptrum/hpp/dt_type_helper.hpp"
#include "deptrum/hpp/dt_types.hpp"
#include "dt_device.hpp"
#include "dt_frame.hpp"
#include "dt_stream_profile.hpp"

namespace dt {

/**
 * @brief Configuration manager for pipeline stream parameters.
 */
class Config {
 public:
  /**
   * @brief Constructs a new pipeline configuration instance.
   */
  Config() {
    dt_error* error = nullptr;
    impl_ = dt_create_config(&error);
    Error::Handle(&error);
  }

  explicit Config(dt_config_t* impl) : impl_(impl) {}

  /**
   * @brief Destroys the configuration instance.
   */
  ~Config() noexcept { dt_delete_config(impl_); }

  dt_config_t* GetImpl() const { return impl_; }

  /**
   * @brief Enables a stream by its stream type.
   *
   * @param[in] stream_type Stream type to enable.
   */
  void EnableStream(DtStreamType stream_type) const {
    dt_error* error = nullptr;
    dt_config_enable_stream(impl_, stream_type, &error);
    Error::Handle(&error);
  }

  /**
   * @brief Enables a stream by its sensor type.
   *
   * @param[in] sensor_type Sensor type to enable.
   */
  void EnableStream(DtSensorType sensor_type) const {
    auto stream_type = dt::TypeHelper::ConvertSensorTypeToStreamType(sensor_type);
    EnableStream(stream_type);
  }

  /**
   * @brief Enables a stream with specific profile settings.
   *
   * @param[in] stream_profile Preconfigured stream profile.
   */
  void EnableStream(std::shared_ptr<const StreamProfile> stream_profile) const {
    dt_error* error = nullptr;
    auto c_stream_profile = stream_profile->GetImpl();
    dt_config_enable_stream_with_stream_profile(impl_, c_stream_profile, &error);
    Error::Handle(&error);
  }

  /**
   * @deprecated Use EnableStream with profile instead.
   * @brief Enables all available streams.
   */
  void EnableAllStream() {
    dt_error* error = nullptr;
    dt_config_enable_all_stream(impl_, &error);
    Error::Handle(&error);
  }

  /**
   * @brief Enables a video stream with configurable parameters.
   *
   * @param[in] stream_type Video stream type.
   * @param[in] width Video width (default: kDtWidthAny).
   * @param[in] height Video height (default: kDtHeightAny).
   * @param[in] fps Frame rate (default: kDtFpsAny).
   * @param[in] format Pixel format (default: kDtFormatAny).
   */
  void EnableVideoStream(DtStreamType stream_type,
                         uint32_t width = kDtWidthAny,
                         uint32_t height = kDtHeightAny,
                         uint32_t fps = kDtFpsAny,
                         DtFormat format = kDtFormatAny) const {
    dt_error* error = nullptr;
    dt_config_enable_video_stream(impl_, stream_type, width, height, fps, format, &error);
    Error::Handle(&error);
  }

  /**
   * @brief Enables a video stream by sensor type.
   *
   * @param[in] sensor_type Sensor type to enable.
   * @param[in] width Video width (default: kDtWidthAny).
   * @param[in] height Video height (default: kDtHeightAny).
   * @param[in] fps Frame rate (default: kDtFpsAny).
   * @param[in] format Pixel format (default: kDtFormatAny).
   */
  void EnableVideoStream(DtSensorType sensor_type,
                         uint32_t width = kDtWidthAny,
                         uint32_t height = kDtHeightAny,
                         uint32_t fps = kDtFpsAny,
                         DtFormat format = kDtFormatAny) const {
    auto stream_type = dt::TypeHelper::ConvertSensorTypeToStreamType(sensor_type);
    EnableVideoStream(stream_type, width, height, fps, format);
  }

  /**
   * @brief Enables an accelerometer stream.
   *
   * @param[in] full_scale_range Measurement range (default: kDtAccelFullScaleRangeAny).
   * @param[in] sample_rate Sampling rate (default: kDtAccelSampleRateAny).
   */
  void EnableAccelStream(DtAccelFullScaleRange full_scale_range = kDtAccelFullScaleRangeAny,
                         DtAccelSampleRate sample_rate = kDtAccelSampleRateAny) const {
    dt_error* error = nullptr;
    dt_config_enable_accel_stream(impl_, full_scale_range, sample_rate, &error);
    Error::Handle(&error);
  }

  /**
   * @brief Enables a gyroscope stream.
   *
   * @param[in] full_scale_range Measurement range (default: kDtGyroFullScaleRangeAny).
   * @param[in] sample_rate Sampling rate (default: kDtGyroSampleRateAny).
   */
  void EnableGyroStream(DtGyroFullScaleRange full_scale_range = kDtGyroFullScaleRangeAny,
                        DtGyroSampleRate sample_rate = kDtGyroSampleRateAny) const {
    dt_error* error = nullptr;
    dt_config_enable_gyro_stream(impl_, full_scale_range, sample_rate, &error);
    Error::Handle(&error);
  }

  /**
   * @brief Enables a palm stream.
   */
  void EnablePalmStream() const {
    dt_error* error = nullptr;
    dt_config_enable_palm_stream(impl_, &error);
    Error::Handle(&error);
  }

  /**
   * @brief Disables a specific stream.
   *
   * @param[in] stream_type Stream type to disable.
   */
  void DisableStream(DtStreamType stream_type) const {
    dt_error* error = nullptr;
    dt_config_disable_stream(impl_, stream_type, &error);
    Error::Handle(&error);
  }

  /**
   * @brief Disables a specific sensor stream.
   *
   * @param[in] sensor_type Sensor type to disable.
   */
  void DisableStream(DtSensorType sensor_type) const {
    auto stream_type = dt::TypeHelper::ConvertSensorTypeToStreamType(sensor_type);
    DisableStream(stream_type);
  }

  /**
   * @brief Disables all configured streams.
   */
  void DisableAllStream() const {
    dt_error* error = nullptr;
    dt_config_disable_all_stream(impl_, &error);
    Error::Handle(&error);
  }

  /**
   * @brief Retrieves the list of enabled stream profiles.
   *
   * @return std::shared_ptr<StreamProfileList> Active stream configurations.
   */
  std::shared_ptr<StreamProfileList> GetEnabledStreamProfileList() const {
    dt_error* error = nullptr;
    auto list = dt_config_get_enabled_stream_profile_list(impl_, &error);
    Error::Handle(&error);
    return std::make_shared<StreamProfileList>(list);
  }

 private:
  dt_config_t* impl_ = nullptr;
};

/**
 * @brief High-level interface for synchronized RGBD data processing.
 */
class Pipeline {
 public:
  /**
   * @brief Callback type for frameset arrival events.
   *
   * @param[in] frame Completed frameset data.
   */
  typedef std::function<void(std::shared_ptr<FrameSet> frame)> FrameSetCallback;

 public:
  /**
   * @brief Constructs a pipeline for the first available device.
   *
   * @throw Exception if device already in use.
   */
  Pipeline() {
    dt_error* error = nullptr;
    impl_ = dt_create_pipeline(&error);
    Error::Handle(&error);
  }

  /**
   * @brief Constructs a pipeline bound to a specific device.
   *
   * @param[in] device Pre-initialized device instance.
   */
  explicit Pipeline(std::shared_ptr<Device> device) {
    dt_error* error = nullptr;
    impl_ = dt_create_pipeline_with_device(device->GetImpl(), &error);
    Error::Handle(&error);
  }

  /**
   * @brief Destroys the pipeline instance.
   */
  ~Pipeline() noexcept { dt_delete_pipeline(impl_); }

  /**
   * @brief Starts pipeline processing.
   *
   * @param[in] config Stream configuration (default: nullptr).
   */
  void Start(std::shared_ptr<Config> config = nullptr) const {
    dt_error* error = nullptr;
    dt_config_t* config_impl = config == nullptr ? nullptr : config->GetImpl();
    dt_pipeline_start_with_config(impl_, config_impl, &error);
    Error::Handle(&error);
  }

  /**
   * @brief Starts pipeline with frameset callback.
   *
   * @param[in] config Stream configuration.
   * @param[in] callback Frameset arrival handler.
   */
  void Start(std::shared_ptr<Config> config, FrameSetCallback callback) {
    callback_ = callback;
    dt_error* error = nullptr;
    dt_pipeline_start_with_config_and_callback(impl_,
                                               config ? config->GetImpl() : nullptr,
                                               &Pipeline::DtFrameSetCallback,
                                               this,
                                               &error);
    Error::Handle(&error);
  }

  /**
   * @brief Stops pipeline processing.
   */
  void Stop() const {
    dt_error* error = nullptr;
    dt_pipeline_stop(impl_, &error);
    Error::Handle(&error);
  }

  /**
   * @brief Retrieves active pipeline configuration.
   *
   * @return std::shared_ptr<Config> Current configuration.
   */
  std::shared_ptr<Config> GetConfig() const {
    dt_error* error = nullptr;
    auto config = dt_pipeline_get_config(impl_, &error);
    Error::Handle(&error);
    return std::make_shared<Config>(config);
  }

  /**
   * @brief Blocks until synchronized frameset is available.
   *
   * @param[in] timeout_ms Maximum wait time in milliseconds (default: 1000).
   * @return std::shared_ptr<FrameSet> Captured frameset.
   */
  std::shared_ptr<FrameSet> WaitForFrames(uint32_t timeout_ms = 1000) const {
    dt_error* error = nullptr;
    auto frame_set = dt_pipeline_wait_for_frames(impl_, timeout_ms, &error);
    if (frame_set == nullptr) {
      return nullptr;
    }
    Error::Handle(&error);
    return std::make_shared<FrameSet>(frame_set);
  }

  /**
   * @brief Retrieves associated device instance.
   *
   * @return std::shared_ptr<Device> Bound device.
   */
  std::shared_ptr<Device> GetDevice() const {
    dt_error* error = nullptr;
    auto device = dt_pipeline_get_device(impl_, &error);
    Error::Handle(&error);
    return std::make_shared<Device>(device);
  }

  /**
   * @brief Retrieves available stream profiles for a sensor.
   *
   * @param[in] sensor_type Target sensor type.
   * @return std::shared_ptr<StreamProfileList> Compatible stream profiles.
   */
  std::shared_ptr<StreamProfileList> GetStreamProfileList(DtSensorType sensor_type) const {
    dt_error* error = nullptr;
    auto list = dt_pipeline_get_stream_profile_list(impl_, sensor_type, &error);
    Error::Handle(&error);
    return std::make_shared<StreamProfileList>(list);
  }

  /**
   * @brief Retrieves calibrated camera parameters.
   *
   * @return DtCameraParam Camera intrinsic/extrinsic parameters.
   */
  DtCameraParam GetCameraParam() {
    dt_error* error = nullptr;
    DtCameraParam camera_param = dt_pipeline_get_camera_param(impl_, &error);
    Error::Handle(&error);
    return camera_param;
  }

  /**
   * @brief Enables hardware frame synchronization.
   */
  void EnableFrameSync() const {
    dt_error* error = nullptr;
    dt_pipeline_enable_frame_sync(impl_, &error);
    Error::Handle(&error);
  }

  /**
   * @brief Disables hardware frame synchronization.
   */
  void DisableFrameSync() const {
    dt_error* error = nullptr;
    dt_pipeline_disable_frame_sync(impl_, &error);
    Error::Handle(&error);
  }

 private:
  static void DtFrameSetCallback(dt_frame_t* frame_set, void* user_data) {
    auto pipeline = static_cast<Pipeline*>(user_data);
    if (pipeline != nullptr && pipeline->callback_ != nullptr) {
      pipeline->callback_(std::make_shared<FrameSet>(frame_set));
    }
  }

 private:
  dt_pipeline_t* impl_ = nullptr;
  FrameSetCallback callback_;
};

}  // namespace dt

#endif  // DEPTRUM_HPP_DT_PIPELINE_HPP_
