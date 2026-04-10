// Copyright (c) Deptrum Inc. All Rights Reserved.

/**
 * @file dt_stream_profile.hpp
 * @brief Stream configuration parameters for resolution, format and sensor properties.
 */

#ifndef DEPTRUM_HPP_DT_STREAM_PROFILE_HPP_
#define DEPTRUM_HPP_DT_STREAM_PROFILE_HPP_

#include <iostream>
#include <memory>
#include "deptrum/h/dt_error.h"
#include "deptrum/h/dt_stream_profile.h"
#include "dt_types.hpp"

namespace dt {

/**
 * @brief Base class for stream configuration profiles.
 */
class StreamProfile : public std::enable_shared_from_this<StreamProfile> {
 public:
  StreamProfile(StreamProfile& stream_profile) = delete;
  StreamProfile& operator=(StreamProfile& stream_profile) = delete;

  StreamProfile(StreamProfile&& stream_profile) noexcept : impl_(stream_profile.impl_) {
    stream_profile.impl_ = nullptr;
  }

  StreamProfile& operator=(StreamProfile&& stream_profile) noexcept {
    if (this != &stream_profile) {
      dt_delete_stream_profile(impl_);
      impl_ = stream_profile.impl_;
      stream_profile.impl_ = nullptr;
    }
    return *this;
  }

  virtual ~StreamProfile() noexcept {
    if (impl_) {
      dt_delete_stream_profile(impl_);
    }
  }

  /**
   * @brief Retrieves underlying implementation pointer.
   *
   * @return const dt_stream_profile* Internal representation.
   */
  const dt_stream_profile* GetImpl() const { return impl_; }

  /**
   * @brief Gets data encoding format.
   *
   * @return DtFormat Stream data format.
   */
  DtFormat GetFormat() const {
    dt_error* error = nullptr;
    auto format = dt_stream_profile_get_format(impl_, &error);
    Error::Handle(&error);
    return format;
  }

  /**
   * @brief Gets stream classification type.
   *
   * @return DtStreamType Stream category identifier.
   */
  DtStreamType GetType() const {
    dt_error* error = nullptr;
    auto type = dt_stream_profile_get_type(impl_, &error);
    Error::Handle(&error);
    return type;
  }

  /**
   * @brief Gets transformation to target stream.
   *
   * @param[in] target Destination stream profile.
   * @return DtExtrinsic Transform parameters.
   */
  DtExtrinsic GetExtrinsicTo(std::shared_ptr<StreamProfile> target) const {
    dt_error* error = nullptr;
    auto extrinsic = dt_stream_profile_get_extrinsic_to(
        impl_,
        const_cast<dt_stream_profile_t*>(target->GetImpl()),
        &error);
    Error::Handle(&error);
    return extrinsic;
  }

  /**
   * @brief Sets transformation to target stream.
   *
   * @param[in] target Destination stream profile.
   * @param[in] extrinsic Transform parameters.
   */
  void BindExtrinsicTo(std::shared_ptr<StreamProfile> target, const DtExtrinsic& extrinsic) {
    dt_error* error = nullptr;
    dt_stream_profile_set_extrinsic_to(const_cast<dt_stream_profile_t*>(impl_),
                                       const_cast<const dt_stream_profile_t*>(target->GetImpl()),
                                       extrinsic,
                                       &error);
    Error::Handle(&error);
  }

  /**
   * @brief Sets transformation to target stream type.
   *
   * @param[in] target_stream_type Destination stream category.
   * @param[in] extrinsic Transform parameters.
   */
  void BindExtrinsicTo(const DtStreamType& target_stream_type, const DtExtrinsic& extrinsic) {
    dt_error* error = nullptr;
    dt_stream_profile_set_extrinsic_to_type(const_cast<dt_stream_profile_t*>(impl_),
                                            target_stream_type,
                                            extrinsic,
                                            &error);
    Error::Handle(&error);
  }

  /**
   * @brief Checks runtime type compatibility.
   *
   * @tparam T Target profile type.
   * @return bool True if convertible to target type.
   */
  template<typename T>
  bool Is() const;

  /**
   * @brief Converts to specific profile type.
   *
   * @tparam T Target profile type.
   * @return std::shared_ptr<T> Converted profile instance.
   * @throw std::runtime_error On incompatible type conversion.
   */
  template<typename T>
  std::shared_ptr<T> As() {
    if (!Is<T>()) {
      throw std::runtime_error("Unsupported operation: Type mismatch");
    }
    return std::dynamic_pointer_cast<T>(shared_from_this());
  }

  /**
   * @brief Converts to specific const profile type.
   *
   * @tparam T Target profile type.
   * @return std::shared_ptr<const T> Converted profile instance.
   * @throw std::runtime_error On incompatible type conversion.
   */
  template<typename T>
  std::shared_ptr<const T> As() const {
    if (!Is<T>()) {
      throw std::runtime_error("Unsupported operation: Type mismatch");
    }
    return std::static_pointer_cast<const T>(shared_from_this());
  }

 protected:
  explicit StreamProfile(const dt_stream_profile_t* impl) : impl_(impl) {}

 protected:
  const dt_stream_profile_t* impl_ = nullptr;
};

/**
 * @brief Video stream configuration profile.
 */
class VideoStreamProfile : public StreamProfile {
 public:
  explicit VideoStreamProfile(const dt_stream_profile_t* impl) : StreamProfile(impl) {}
  ~VideoStreamProfile() noexcept override = default;

  /**
   * @brief Gets frames per second setting.
   *
   * @return uint32_t Configured frame rate.
   */
  uint32_t GetFps() const {
    dt_error* error = nullptr;
    auto fps = dt_video_stream_profile_get_fps(impl_, &error);
    Error::Handle(&error);
    return fps;
  }

  /**
   * @brief Gets horizontal resolution.
   *
   * @return uint32_t Image width in pixels.
   */
  uint32_t GetWidth() const {
    dt_error* error = nullptr;
    auto width = dt_video_stream_profile_get_width(impl_, &error);
    Error::Handle(&error);
    return width;
  }

  /**
   * @brief Gets vertical resolution.
   *
   * @return uint32_t Image height in pixels.
   */
  uint32_t GetHeight() const {
    dt_error* error = nullptr;
    auto height = dt_video_stream_profile_get_height(impl_, &error);
    Error::Handle(&error);
    return height;
  }

  /**
   * @brief Gets optical projection parameters.
   *
   * @return DtCameraIntrinsic Camera intrinsic parameters.
   */
  DtCameraIntrinsic GetIntrinsic() const {
    dt_error* error = nullptr;
    auto intrinsic = dt_video_stream_profile_get_intrinsic(impl_, &error);
    Error::Handle(&error);
    return intrinsic;
  }

  /**
   * @brief Sets optical projection parameters.
   *
   * @param[in] intrinsic Camera intrinsic parameters.
   */
  void SetIntrinsic(const DtCameraIntrinsic& intrinsic) {
    dt_error* error = nullptr;
    dt_video_stream_profile_set_intrinsic(const_cast<dt_stream_profile_t*>(impl_),
                                          intrinsic,
                                          &error);
    Error::Handle(&error);
  }

  /**
   * @brief Gets optical distortion parameters.
   *
   * @return DtCameraDistortion Brown-Conrady distortion coefficients.
   */
  DtCameraDistortion GetDistortion() const {
    dt_error* error = nullptr;
    auto distortion = dt_video_stream_profile_get_distortion(impl_, &error);
    Error::Handle(&error);
    return distortion;
  }

  /**
   * @brief Sets optical distortion parameters.
   *
   * @param[in] distortion Brown-Conrady distortion coefficients.
   */
  void SetDistortion(const DtCameraDistortion& distortion) {
    dt_error* error = nullptr;
    dt_video_stream_profile_set_distortion(const_cast<dt_stream_profile_t*>(impl_),
                                           distortion,
                                           &error);
    Error::Handle(&error);
  }
};

/**
 * @brief Accelerometer stream configuration profile.
 */
class AccelStreamProfile : public StreamProfile {
 public:
  explicit AccelStreamProfile(const dt_stream_profile_t* impl) : StreamProfile(impl) {}
  ~AccelStreamProfile() noexcept override = default;

  /**
   * @brief Gets measurement range.
   *
   * @return DtAccelFullScaleRange Acceleration measurement range.
   */
  DtAccelFullScaleRange GetFullScaleRange() const {
    dt_error* error = nullptr;
    auto full_scale_range = dt_accel_stream_profile_get_full_scale_range(impl_, &error);
    Error::Handle(&error);
    return full_scale_range;
  }

  /**
   * @brief Gets sampling frequency.
   *
   * @return DtAccelSampleRate Samples per second.
   */
  DtAccelSampleRate GetSampleRate() const {
    dt_error* error = nullptr;
    auto sample_rate = dt_accel_stream_profile_get_sample_rate(impl_, &error);
    Error::Handle(&error);
    return sample_rate;
  }

  /**
   * @brief Gets sensor calibration parameters.
   *
   * @return DtAccelIntrinsic Sensor intrinsic parameters.
   */
  DtAccelIntrinsic GetIntrinsic() const {
    dt_error* error = nullptr;
    auto intrinsic = dt_accel_stream_profile_get_intrinsic(impl_, &error);
    Error::Handle(&error);
    return intrinsic;
  }
};

/**
 * @brief Gyroscope stream configuration profile.
 */
class GyroStreamProfile : public StreamProfile {
 public:
  explicit GyroStreamProfile(const dt_stream_profile_t* impl) : StreamProfile(impl) {}
  ~GyroStreamProfile() noexcept override = default;

  /**
   * @brief Gets measurement range.
   *
   * @return DtGyroFullScaleRange Angular velocity measurement range.
   */
  DtGyroFullScaleRange GetFullScaleRange() const {
    dt_error* error = nullptr;
    auto full_scale_range = dt_gyro_stream_profile_get_full_scale_range(impl_, &error);
    Error::Handle(&error);
    return full_scale_range;
  }

  /**
   * @brief Gets sampling frequency.
   *
   * @return DtGyroSampleRate Samples per second.
   */
  DtGyroSampleRate GetSampleRate() const {
    dt_error* error = nullptr;
    auto sample_rate = dt_gyro_stream_profile_get_sample_rate(impl_, &error);
    Error::Handle(&error);
    return sample_rate;
  }

  /**
   * @brief Gets sensor calibration parameters.
   *
   * @return DtGyroIntrinsic Sensor intrinsic parameters.
   */
  DtGyroIntrinsic GetIntrinsic() const {
    dt_error* error = nullptr;
    auto intrinsic = dt_gyro_stream_get_intrinsic(impl_, &error);
    Error::Handle(&error);
    return intrinsic;
  }
};

/**
 * @brief Palm stream configuration profile.
 */
class PalmStreamProfile : public StreamProfile {
 public:
  explicit PalmStreamProfile(const dt_stream_profile_t* impl) : StreamProfile(impl) {}
  ~PalmStreamProfile() noexcept override = default;
};

// StreamProfile type identification implementation.
template<typename T>
bool StreamProfile::Is() const {
  switch (this->GetType()) {
    case kDtStreamVideo:
    case kDtStreamIr:
    case kDtStreamIrLeft:
    case kDtStreamIrRight:
    case kDtStreamColor:
    case kDtStreamColorLeft:
    case kDtStreamColorRight:
    case kDtStreamDepth:
    case kDtStreamRawPhase:
      return typeid(T) == typeid(VideoStreamProfile);
    case kDtStreamAccel:
      return typeid(T) == typeid(AccelStreamProfile);
    case kDtStreamGyro:
      return typeid(T) == typeid(GyroStreamProfile);
    case kDtStreamPalm:
      return typeid(T) == typeid(PalmStreamProfile);
    default:
      break;
  }
  return false;
}

/**
 * @brief Stream profile instance factory.
 */
class StreamProfileFactory {
 public:
  /**
   * @brief Creates typed stream profile instance.
   *
   * @param[in] impl Underlying implementation pointer.
   * @return std::shared_ptr<StreamProfile> Typed profile instance.
   */
  static std::shared_ptr<StreamProfile> Create(const dt_stream_profile_t* impl) {
    dt_error* error = nullptr;
    const auto type = dt_stream_profile_get_type(impl, &error);
    Error::Handle(&error);
    switch (type) {
      case kDtStreamIr:
      case kDtStreamIrLeft:
      case kDtStreamIrRight:
      case kDtStreamDepth:
      case kDtStreamColor:
      case kDtStreamColorLeft:
      case kDtStreamColorRight:
      case kDtStreamVideo:
      case kDtStreamRawPhase:
        return std::make_shared<VideoStreamProfile>(impl);
      case kDtStreamAccel:
        return std::make_shared<AccelStreamProfile>(impl);
      case kDtStreamGyro:
        return std::make_shared<GyroStreamProfile>(impl);
      case kDtStreamPalm:
        return std::make_shared<PalmStreamProfile>(impl);
      default: {
        dt_error* err = dt_create_error(kDtStatusError,
                                        "Unsupported stream type",
                                        "StreamProfileFactory::Create",
                                        "",
                                        kDtExceptionTypeInvalidValue);
        Error::Handle(&err);
        return nullptr;
      }
    }
  }
};

/**
 * @brief Collection of stream configuration profiles.
 */
class StreamProfileList {
 public:
  explicit StreamProfileList(dt_stream_profile_list_t* impl) : impl_(impl) {}
  ~StreamProfileList() noexcept { dt_delete_stream_profile_list(impl_); }

  /**
   * @brief Gets number of profiles in collection.
   *
   * @return uint32_t Profile count.
   */
  uint32_t GetCount() const {
    dt_error* error = nullptr;
    auto count = dt_stream_profile_list_get_count(impl_, &error);
    Error::Handle(&error);
    return count;
  }

  /**
   * @brief Retrieves profile by index.
   *
   * @param[in] index Positional index (0 to count-1).
   * @return std::shared_ptr<StreamProfile> Profile at index position.
   */
  std::shared_ptr<StreamProfile> GetProfile(uint32_t index) const {
    dt_error* error = nullptr;
    auto profile = dt_stream_profile_list_get_profile(impl_, index, &error);
    Error::Handle(&error);
    return StreamProfileFactory::Create(profile);
  }

  /**
   * @brief Finds matching video stream profile.
   *
   * @param[in] width Target width (kDtWidthAny for wildcard).
   * @param[in] height Target height (kDtHeightAny for wildcard).
   * @param[in] format Target format (kDtFormatAny for wildcard).
   * @param[in] fps Target frame rate (kDtFpsAny for wildcard).
   * @return std::shared_ptr<VideoStreamProfile> First matching profile.
   */
  std::shared_ptr<VideoStreamProfile> GetVideoStreamProfile(int width = kDtWidthAny,
                                                            int height = kDtHeightAny,
                                                            DtFormat format = kDtFormatAny,
                                                            int fps = kDtFpsAny) const {
    dt_error* error = nullptr;
    auto profile = dt_stream_profile_list_get_video_stream_profile(impl_,
                                                                   width,
                                                                   height,
                                                                   format,
                                                                   fps,
                                                                   &error);
    Error::Handle(&error);
    auto vsp = StreamProfileFactory::Create(profile);
    return vsp->As<VideoStreamProfile>();
  }

  /**
   * @brief Finds matching accelerometer stream profile.
   *
   * @param[in] full_scale_range Target measurement range.
   * @param[in] sample_rate Target sampling rate.
   * @return std::shared_ptr<AccelStreamProfile> First matching profile.
   */
  std::shared_ptr<AccelStreamProfile> GetAccelStreamProfile(DtAccelFullScaleRange full_scale_range,
                                                            DtAccelSampleRate sample_rate) const {
    dt_error* error = nullptr;
    auto profile = dt_stream_profile_list_get_accel_stream_profile(impl_,
                                                                   full_scale_range,
                                                                   sample_rate,
                                                                   &error);
    Error::Handle(&error);
    auto asp = StreamProfileFactory::Create(profile);
    return asp->As<AccelStreamProfile>();
  }

  /**
   * @brief Finds matching gyroscope stream profile.
   *
   * @param[in] full_scale_range Target measurement range.
   * @param[in] sample_rate Target sampling rate.
   * @return std::shared_ptr<GyroStreamProfile> First matching profile.
   */
  std::shared_ptr<GyroStreamProfile> GetGyroStreamProfile(DtGyroFullScaleRange full_scale_range,
                                                          DtGyroSampleRate sample_rate) const {
    dt_error* error = nullptr;
    auto profile = dt_stream_profile_list_get_gyro_stream_profile(impl_,
                                                                  full_scale_range,
                                                                  sample_rate,
                                                                  &error);
    Error::Handle(&error);
    auto gsp = StreamProfileFactory::Create(profile);
    return gsp->As<GyroStreamProfile>();
  }

  /**
   * @brief Finds matching palm stream profile.
   *
   * @return std::shared_ptr<PalmStreamProfile> First matching profile.
   */
  std::shared_ptr<PalmStreamProfile> GetPalmStreamProfile() const {
    dt_error* error = nullptr;
    auto profile = dt_stream_profile_list_get_palm_stream_profile(impl_, &error);
    Error::Handle(&error);
    auto gsp = StreamProfileFactory::Create(profile);
    return gsp->As<PalmStreamProfile>();
  }

 protected:
  const dt_stream_profile_list_t* impl_ = nullptr;
};

}  // namespace dt

#endif  // DEPTRUM_HPP_DT_STREAM_PROFILE_HPP_
