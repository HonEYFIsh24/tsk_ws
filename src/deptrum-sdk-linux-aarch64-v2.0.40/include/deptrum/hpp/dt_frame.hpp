// Copyright (c) Deptrum Inc. All Rights Reserved.

/**
 * @file dt_frame.hpp
 * @brief Frame types for accessing image and sensor data.
 */

#ifndef DEPTRUM_HPP_DT_FRAME_HPP_
#define DEPTRUM_HPP_DT_FRAME_HPP_

#include <functional>
#include <iostream>
#include <memory>
#include <typeinfo>
#include "deptrum/h/dt_frame.h"
#include "deptrum/hpp/dt_error.hpp"
#include "deptrum/hpp/dt_stream_profile.hpp"
#include "dt_types.hpp"

/**
 * Frame class inheritance hierarchy:
 *        Frame
 *         |
 *     +-----------+----------+----------+----------+-----------+
 *     |           |          |          |          |           |
 *  VideoFrame PointsFrame AccelFrame GyroFrame  PalmFrame   FrameSet
 *       |
 *  +---------------+-----------------------+
 *  |               |                       |
 * ColorFrame    DepthFrame              IrFrame
 *       |                                  |
 * +-----+-------------+              +-----+-----+
 * |                   |              |           |
 * ColorLeftFrame  ColorRightFrame  IRLeftFrame  IRRightFrame
 */

namespace dt {
class Device;
class Sensor;

/**
 * @brief Base class for all frame types.
 */
class Frame : public std::enable_shared_from_this<Frame> {
 public:
  /**
   * @brief Constructs a frame owning the underlying implementation.
   *
   * @param[in] impl Internal frame implementation (ownership transferred).
   */
  explicit Frame(const dt_frame* impl) : impl_(impl) {}

  /**
   * @brief Destroys the frame and releases underlying resources.
   */
  virtual ~Frame() noexcept {
    if (impl_) {
      dt_delete_frame(impl_);
      impl_ = nullptr;
    }
  }

  /**
   * @brief Retrieves underlying implementation pointer.
   *
   * @return const dt_frame* Internal frame representation.
   */
  const dt_frame* GetImpl() const { return impl_; }

  /**
   * @brief Gets frame classification type.
   *
   * @return DtFrameType Frame category identifier.
   */
  virtual DtFrameType GetType() const {
    dt_error* error = nullptr;
    auto type = dt_frame_get_type(impl_, &error);
    Error::Handle(&error);
    return type;
  }

  /**
   * @brief Gets data encoding format.
   *
   * @return DtFormat Data representation format.
   */
  virtual DtFormat GetFormat() const {
    dt_error* error = nullptr;
    auto format = dt_frame_get_format(impl_, &error);
    Error::Handle(&error);
    return format;
  }

  /**
   * @brief Gets sequential frame identifier.
   *
   * @note Sequence increments monotonically per stream.
   *
   * @return uint64_t Frame sequence number.
   */
  virtual uint64_t GetIndex() const {
    dt_error* error = nullptr;
    auto index = dt_frame_get_index(impl_, &error);
    Error::Handle(&error);
    return index;
  }

  /**
   * @brief Gets pointer to raw frame data.
   *
   * @return uint8_t* Mutable data buffer.
   */
  virtual uint8_t* GetData() const {
    dt_error* error = nullptr;
    auto data = dt_frame_get_data(impl_, &error);
    Error::Handle(&error);
    return data;
  }

  /**
   * @brief Gets size of frame data buffer.
   *
   * @return uint32_t Data buffer size in bytes.
   */
  virtual uint32_t GetDataSize() const {
    dt_error* error = nullptr;
    auto data_size = dt_frame_get_data_size(impl_, &error);
    Error::Handle(&error);
    return data_size;
  }

  /**
   * @brief Gets descriptive frame name.
   *
   * @return const char* Human-readable frame identifier.
   */
  virtual const char* GetName() const {
    dt_error* error = nullptr;
    const char* name = dt_frame_get_name(impl_, &error);
    Error::Handle(&error);
    return name;
  }

  /**
   * @brief Gets device-captured timestamp.
   *
   * @return uint64_t Capture time in microseconds (device clock domain).
   */
  uint64_t GetTimeStampUs() const {
    dt_error* error = nullptr;
    auto time_stamp_us = dt_frame_get_timestamp_us(impl_, &error);
    Error::Handle(&error);
    return time_stamp_us;
  }

  /**
   * @brief Gets host-received timestamp.
   *
   * @return uint64_t Reception time in microseconds (host clock domain).
   */
  uint64_t GetSystemTimeStampUs() const {
    dt_error* error = nullptr;
    auto system_time_stamp_us = dt_frame_get_system_timestamp_us(impl_, &error);
    Error::Handle(&error);
    return system_time_stamp_us;
  }

  /**
   * @brief Gets drift-compensated global timestamp.
   *
   * @attention Requires explicit enablement via Device::enable_global_timestamp().
   * @attention Device support verified by Device::is_global_timestamp_supported().
   *
   * @return uint64_t Synchronized timestamp in microseconds (host clock domain).
   */
  uint64_t GetGlobalTimeStampUs() const {
    dt_error* error = nullptr;
    auto global_time_stamp_us = dt_frame_get_global_timestamp_us(impl_, &error);
    Error::Handle(&error);
    return global_time_stamp_us;
  }

  /**
   * @brief Gets frame metadata buffer.
   *
   * @return uint8_t* Pointer to metadata storage.
   */
  uint8_t* GetMetadata() const {
    dt_error* error = nullptr;
    auto metadata = dt_frame_get_metadata(impl_, &error);
    Error::Handle(&error);
    return metadata;
  }

  /**
   * @brief Gets metadata buffer size.
   *
   * @return uint32_t Metadata storage size in bytes.
   */
  uint32_t GetMetadataSize() const {
    dt_error* error = nullptr;
    auto metadata_size = dt_frame_get_metadata_size(impl_, &error);
    Error::Handle(&error);
    return metadata_size;
  }

  /**
   * @brief Checks for specific metadata presence.
   *
   * @param[in] type Metadata category identifier.
   * @return bool True if metadata exists.
   */
  bool HasMetadata(DtFrameMetadataType type) const {
    dt_error* error = nullptr;
    auto result = dt_frame_has_metadata(impl_, type, &error);
    Error::Handle(&error);
    return result;
  }

  /**
   * @brief Retrieves typed metadata value.
   *
   * @param[in] type Metadata category identifier.
   * @return int64_t Stored metadata value.
   */
  int64_t GetMetadataValue(DtFrameMetadataType type) const {
    dt_error* error = nullptr;
    auto value = dt_frame_get_metadata_value(impl_, type, &error);
    Error::Handle(&error);
    return value;
  }

  /**
   * @brief Gets stream configuration profile.
   *
   * @return std::shared_ptr<StreamProfile> Associated stream profile.
   */
  std::shared_ptr<StreamProfile> GetStreamProfile() const {
    dt_error* error = nullptr;
    auto profile = dt_frame_get_stream_profile(impl_, &error);
    Error::Handle(&error);
    return StreamProfileFactory::Create(profile);
  }

  /**
   * @brief Gets originating sensor.
   *
   * @return std::shared_ptr<Sensor> Frame source sensor.
   */
  std::shared_ptr<Sensor> GetSensor() const {
    dt_error* error = nullptr;
    auto sensor = dt_frame_get_sensor(impl_, &error);
    Error::Handle(&error);
    return std::make_shared<Sensor>(sensor);
  }

  /**
   * @brief Gets originating device.
   *
   * @return std::shared_ptr<Device> Frame source device.
   */
  std::shared_ptr<Device> GetDevice() const {
    dt_error* error = nullptr;
    auto device = dt_frame_get_device(impl_, &error);
    Error::Handle(&error);
    return std::make_shared<Device>(device);
  }

  /**
   * @brief Checks runtime type compatibility.
   *
   * @tparam T Target frame type.
   * @return bool True if convertible to target type.
   */
  template<typename T>
  bool Is() const;

  /**
   * @brief Converts to specific frame type.
   *
   * @tparam T Target frame type.
   * @return std::shared_ptr<T> Converted frame instance.
   * @throw std::runtime_error On incompatible type conversion.
   */
  template<typename T>
  std::shared_ptr<T> As() {
    if (!Is<T>()) {
      throw std::runtime_error("Unsupported operation: Type mismatch");
    }

    dt_error* error = nullptr;
    dt_frame_add_ref(impl_, &error);
    Error::Handle(&error);

    return std::make_shared<T>(impl_);
  }

  /**
   * @brief Converts to specific const frame type.
   *
   * @tparam T Target frame type.
   * @return std::shared_ptr<const T> Converted frame instance.
   * @throw std::runtime_error On incompatible type conversion.
   */
  template<typename T>
  std::shared_ptr<const T> As() const {
    if (!Is<const T>()) {
      throw std::runtime_error("Unsupported operation: Type mismatch");
    }

    dt_error* error = nullptr;
    dt_frame_add_ref(impl_, &error);
    Error::Handle(&error);

    return std::make_shared<const T>(impl_);
  }

 protected:
  const dt_frame* impl_ = nullptr;
};

/**
 * @brief Video frame specialization.
 */
class VideoFrame : public Frame {
 public:
  explicit VideoFrame(const dt_frame* impl) : Frame(impl) {}

  ~VideoFrame() noexcept override = default;

  /**
   * @brief Gets frame width in pixels.
   *
   * @return uint32_t Image width.
   */
  uint32_t GetWidth() const {
    dt_error* error = nullptr;
    auto width = dt_video_frame_get_width(impl_, &error);
    Error::Handle(&error);
    return width;
  }

  /**
   * @brief Gets frame height in pixels.
   *
   * @return uint32_t Image height.
   */
  uint32_t GetHeight() const {
    dt_error* error = nullptr;
    auto height = dt_video_frame_get_height(impl_, &error);
    Error::Handle(&error);
    return height;
  }

  /**
   * @brief Gets valid bits per pixel.
   *
   * @attention Only valid for Y8/Y10/Y11/Y12/Y14/Y16 formats.
   *
   * @return uint8_t Effective bits per pixel.
   */
  uint8_t GetPixelAvailableBitSize() const {
    dt_error* error = nullptr;
    auto bit_size = dt_video_frame_get_pixel_available_bit_size(impl_, &error);
    Error::Handle(&error);
    return bit_size;
  }
};

/**
 * @brief RGB color frame specialization.
 */
class ColorFrame : public VideoFrame {
 public:
  explicit ColorFrame(const dt_frame* impl) : VideoFrame(impl) {}
  ~ColorFrame() noexcept override = default;
};

/**
 * @brief Raw phase data frame specialization.
 */
class RawPhaseFrame : public VideoFrame {
 public:
  explicit RawPhaseFrame(const dt_frame* impl) : VideoFrame(impl) {}
  ~RawPhaseFrame() noexcept override = default;
};

/**
 * @brief Depth map frame specialization.
 */
class DepthFrame : public VideoFrame {
 public:
  explicit DepthFrame(const dt_frame* impl) : VideoFrame(impl) {}
  ~DepthFrame() noexcept override = default;

  /**
   * @brief Gets depth value scaling factor.
   *
   * @return float Depth value multiplier (millimeters per unit).
   */
  float GetValueScale() const {
    dt_error* error = nullptr;
    auto scale = dt_depth_frame_get_value_scale(impl_, &error);
    Error::Handle(&error);
    return scale;
  }
};

/**
 * @brief Infrared frame specialization.
 */
class IrFrame : public VideoFrame {
 public:
  explicit IrFrame(const dt_frame* impl) : VideoFrame(impl) {}
  ~IrFrame() noexcept override = default;
};

/**
 * @brief Point cloud data container.
 */
class PointsFrame : public Frame {
 public:
  explicit PointsFrame(const dt_frame* impl) : Frame(impl) {}
  ~PointsFrame() noexcept override = default;

  /**
   * @brief Gets point coordinate scaling factor.
   *
   * @return float Position multiplier (millimeters per unit).
   */
  float GetCoordinateValueScale() const {
    dt_error* error = nullptr;
    auto scale = dt_points_frame_get_coordinate_value_scale(impl_, &error);
    Error::Handle(&error);
    return scale;
  }

  /**
   * @brief Gets point cloud width.
   *
   * @return uint32_t Point cloud width.
   */
  uint32_t GetWidth() const {
    dt_error* error = nullptr;
    auto width = dt_point_cloud_frame_get_width(impl_, &error);
    Error::Handle(&error);
    return width;
  }

  /**
   * @brief Gets point cloud height.
   *
   * @return uint32_t Point cloud height.
   */
  uint32_t GetHeight() const {
    dt_error* error = nullptr;
    auto height = dt_point_cloud_frame_get_height(impl_, &error);
    Error::Handle(&error);
    return height;
  }
};

/**
 * @brief Accelerometer data frame.
 */
class AccelFrame : public Frame {
 public:
  explicit AccelFrame(const dt_frame* impl) : Frame(impl) {}
  ~AccelFrame() noexcept override = default;

  /**
   * @brief Gets acceleration values.
   *
   * @return DtAccelValue Acceleration vector.
   */
  DtAccelValue GetValue() const {
    dt_error* error = nullptr;
    auto value = dt_accel_frame_get_value(impl_, &error);
    Error::Handle(&error);
    return value;
  }

  /**
   * @brief Gets sensor temperature.
   *
   * @return float Temperature in Celsius.
   */
  float GetTemperature() const {
    dt_error* error = nullptr;
    auto temp = dt_accel_frame_get_temperature(impl_, &error);
    Error::Handle(&error);
    return temp;
  }
};

/**
 * @brief Gyroscope data frame.
 */
class GyroFrame : public Frame {
 public:
  explicit GyroFrame(const dt_frame* impl) : Frame(impl) {}
  ~GyroFrame() noexcept override = default;

  /**
   * @brief Gets angular velocity values.
   *
   * @return DtGyroValue Rotation rate vector.
   */
  DtGyroValue GetValue() const {
    dt_error* error = nullptr;
    auto value = dt_gyro_frame_get_value(impl_, &error);
    Error::Handle(&error);
    return value;
  }

  /**
   * @brief Gets sensor temperature.
   *
   * @return float Temperature in Celsius.
   */
  float GetTemperature() const {
    dt_error* error = nullptr;
    auto temperature = dt_gyro_frame_get_temperature(impl_, &error);
    Error::Handle(&error);
    return temperature;
  }
};

/**
 * @brief Palm data frame.
 */
class PalmFrame : public Frame {
 public:
  explicit PalmFrame(const dt_frame* impl) : Frame(impl) {}
  ~PalmFrame() noexcept override = default;
};

/**
 * @brief Synchronized multi-frame container.
 */
class FrameSet : public Frame {
 public:
  explicit FrameSet(const dt_frame* impl) : Frame(impl) {}
  ~FrameSet() noexcept override = default;

  /**
   * @brief Gets contained frame count.
   *
   * @return uint32_t Number of contained frames.
   */
  uint32_t GetCount() const {
    dt_error* error = nullptr;
    auto count = dt_frameset_get_count(impl_, &error);
    Error::Handle(&error);
    return count;
  }

  /**
   * @brief Gets frame by type identifier.
   *
   * @param[in] frame_type Frame category identifier.
   * @return std::shared_ptr<Frame> Matching frame instance.
   */
  std::shared_ptr<Frame> GetFrame(DtFrameType frame_type) const {
    dt_error* error = nullptr;
    auto frame = dt_frameset_get_frame(impl_, frame_type, &error);
    if (!frame) {
      return nullptr;
    }
    Error::Handle(&error);
    return std::make_shared<Frame>(frame);
  }

  /**
   * @brief Gets frame by positional index.
   *
   * @param[in] index Frame position in set.
   * @return std::shared_ptr<Frame> Frame at index position.
   */
  std::shared_ptr<Frame> GetFrameByIndex(uint32_t index) const {
    dt_error* error = nullptr;
    auto frame = dt_frameset_get_frame_by_index(impl_, index, &error);
    if (!frame) {
      return nullptr;
    }
    Error::Handle(&error);
    return std::make_shared<Frame>(frame);
  }

  /**
   * @brief Gets frame by name identifier.
   *
   * @param[in] name Frame name identifier.
   * @return std::shared_ptr<Frame> Matching frame instance.
   */
  std::shared_ptr<Frame> GetFrameByName(const char* name) const {
    dt_error* error = nullptr;
    auto frame = dt_frameset_get_frame_by_name(impl_, name, &error);
    if (!frame) {
      return nullptr;
    }
    Error::Handle(&error);
    return std::make_shared<Frame>(frame);
  }

  /**
   * @brief Adds frame to collection.
   *
   * @attention Replaces existing frames of same type.
   *
   * @param[in] frame Frame to add.
   */
  void PushFrame(std::shared_ptr<const Frame> frame) const {
    dt_error* error = nullptr;
    auto un_const_impl = const_cast<dt_frame*>(impl_);
    auto other_impl = frame->GetImpl();
    dt_frameset_push_frame(un_const_impl, other_impl, &error);
    Error::Handle(&error);
  }

  /**
   * @brief Gets depth frame from set.
   *
   * @return std::shared_ptr<DepthFrame> Depth frame instance.
   */
  std::shared_ptr<DepthFrame> GetDepthFrame() const {
    auto frame = GetFrame(kDtFrameDepth);
    if (frame == nullptr) {
      return nullptr;
    }
    return frame->As<dt::DepthFrame>();
  }

  /**
   * @brief Gets color frame from set.
   *
   * @return std::shared_ptr<ColorFrame> Color frame instance.
   */
  std::shared_ptr<ColorFrame> GetColorFrame() const {
    auto frame = GetFrame(kDtFrameColor);
    if (frame == nullptr) {
      return nullptr;
    }
    return frame->As<dt::ColorFrame>();
  }

  /**
   * @brief Gets infrared frame from set.
   *
   * @return std::shared_ptr<IrFrame> Infrared frame instance.
   */
  std::shared_ptr<IrFrame> GetIrFrame() const {
    auto frame = GetFrame(kDtFrameIr);
    if (frame == nullptr) {
      return nullptr;
    }
    return frame->As<dt::IrFrame>();
  }

  /**
   * @brief Gets point cloud frame from set.
   *
   * @return std::shared_ptr<PointsFrame> Point cloud instance.
   */
  std::shared_ptr<PointsFrame> GetPointsFrame() const {
    auto frame = GetFrame(kDtFramePoints);
    if (frame == nullptr) {
      return nullptr;
    }
    return frame->As<dt::PointsFrame>();
  }
};

/**
 * @brief Frame object construction utilities.
 */
class FrameFactory {
 public:
  /**
   * @brief Creates generic frame with specified parameters.
   *
   * @param[in] frame_type Frame category identifier.
   * @param[in] format Data encoding format.
   * @param[in] data_size Data buffer size in bytes.
   * @return std::shared_ptr<Frame> Allocated frame instance.
   */
  static std::shared_ptr<Frame> CreateFrame(DtFrameType frame_type,
                                            DtFormat format,
                                            uint32_t data_size) {
    dt_error* error = nullptr;
    auto impl = dt_create_frame(frame_type, format, data_size, &error);
    Error::Handle(&error);
    return std::make_shared<Frame>(impl);
  }

  /**
   * @brief Creates empty multi-frame container.
   *
   * @return std::shared_ptr<FrameSet> Allocated frame set.
   */
  static std::shared_ptr<FrameSet> CreateFrameSet() {
    dt_error* error = nullptr;
    auto impl = dt_create_frameset(&error);
    Error::Handle(&error);
    return std::make_shared<FrameSet>(impl);
  }

  /**
   * @brief Creates video frame with specified geometry.
   *
   * @param[in] frame_type Video frame category.
   * @param[in] format Pixel data format.
   * @param[in] width Image width in pixels.
   * @param[in] height Image height in pixels.
   * @param[in] stride Row stride in bytes (0=auto-calculate).
   * @return std::shared_ptr<VideoFrame> Allocated video frame.
   */
  static std::shared_ptr<VideoFrame> CreateVideoFrame(DtFrameType frame_type,
                                                      DtFormat format,
                                                      uint32_t width,
                                                      uint32_t height,
                                                      uint32_t stride = 0) {
    dt_error* error = nullptr;
    auto impl = dt_create_video_frame(frame_type, format, width, height, stride, &error);
    Error::Handle(&error);
    auto frame = std::make_shared<Frame>(impl);
    return frame->As<VideoFrame>();
  }

  /**
   * @brief Creates frame clone from existing instance.
   *
   * @param[in] other_frame Source frame to clone.
   * @param[in] should_copy_data Enable data buffer duplication.
   * @return std::shared_ptr<Frame> Cloned frame instance.
   */
  static std::shared_ptr<Frame> CreateFrameFromOtherFrame(std::shared_ptr<const Frame> other_frame,
                                                          bool should_copy_data = true) {
    dt_error* error = nullptr;
    auto other_impl = other_frame->GetImpl();
    auto impl = dt_create_frame_from_other_frame(other_impl, should_copy_data, &error);
    Error::Handle(&error);
    return std::make_shared<Frame>(impl);
  }

  /**
   * @brief Creates frame matching stream profile parameters.
   *
   * @param[in] profile Stream configuration template.
   * @return std::shared_ptr<Frame> Allocated frame instance.
   */
  static std::shared_ptr<Frame> CreateFrameFromStreamProfile(
      std::shared_ptr<const StreamProfile> profile) {
    dt_error* error = nullptr;
    auto impl = dt_create_frame_from_stream_profile(profile->GetImpl(), &error);
    Error::Handle(&error);
    return std::make_shared<Frame>(impl);
  }

  /**
   * @brief External buffer destruction callback signature.
   */
  typedef std::function<void(uint8_t*)> BufferDestroyCallback;

  /**
   * @brief Creates frame referencing external memory.
   *
   * @param[in] frame_type Frame category identifier.
   * @param[in] format Data encoding format.
   * @param[in] buffer External data buffer.
   * @param[in] destroy_callback Resource cleanup handler.
   * @param[in] buffer_size External buffer capacity.
   * @return std::shared_ptr<Frame> Frame wrapping external memory.
   */
  static std::shared_ptr<Frame> CreateFrameFromBuffer(DtFrameType frame_type,
                                                      DtFormat format,
                                                      uint8_t* buffer,
                                                      BufferDestroyCallback destroy_callback,
                                                      uint32_t buffer_size) {
    dt_error* error = nullptr;
    auto ctx = new BufferDestroyContext{destroy_callback};
    auto impl = dt_create_frame_from_buffer(frame_type,
                                            format,
                                            buffer,
                                            buffer_size,
                                            &FrameFactory::BufferDestroy,
                                            ctx,
                                            &error);
    Error::Handle(&error);
    return std::make_shared<Frame>(impl);
  }

  /**
   * @brief Creates video frame referencing external memory.
   *
   * @param[in] frame_type Video frame category.
   * @param[in] format Pixel data format.
   * @param[in] width Image width in pixels.
   * @param[in] height Image height in pixels.
   * @param[in] buffer External data buffer.
   * @param[in] destroy_callback Resource cleanup handler.
   * @param[in] buffer_size External buffer capacity.
   * @param[in] stride Row stride in bytes (0=auto-calculate).
   * @return std::shared_ptr<VideoFrame> Video frame wrapping external memory.
   */
  static std::shared_ptr<VideoFrame> CreateVideoFrameFromBuffer(
      DtFrameType frame_type,
      DtFormat format,
      uint32_t width,
      uint32_t height,
      uint8_t* buffer,
      BufferDestroyCallback destroy_callback,
      uint32_t buffer_size,
      uint32_t stride = 0) {
    dt_error* error = nullptr;
    auto ctx = new BufferDestroyContext{destroy_callback};
    auto impl = dt_create_video_frame_from_buffer(frame_type,
                                                  format,
                                                  width,
                                                  height,
                                                  stride,
                                                  buffer,
                                                  buffer_size,
                                                  &FrameFactory::BufferDestroy,
                                                  ctx,
                                                  &error);
    Error::Handle(&error);
    auto frame = std::make_shared<Frame>(impl);
    return frame->As<VideoFrame>();
  }

 private:
  struct BufferDestroyContext {
    BufferDestroyCallback callback;
  };

  static void BufferDestroy(uint8_t* buffer, void* context) {
    auto* ctx = static_cast<BufferDestroyContext*>(context);
    if (ctx->callback) {
      ctx->callback(buffer);
    }
    delete ctx;
  }
};

/**
 * @brief Frame timestamp manipulation utilities.
 */
class FrameHelper : public FrameFactory {
 public:
  /**
   * @brief Sets device-capture timestamp.
   *
   * @param[in] frame Target frame instance.
   * @param[in] device_timestamp_us Timestamp in microseconds.
   */
  static void SetFrameDeviceTimestampUs(std::shared_ptr<Frame> frame,
                                        uint64_t device_timestamp_us) {
    dt_error* error = nullptr;
    auto impl = const_cast<dt_frame*>(frame->GetImpl());
    dt_frame_set_timestamp_us(impl, device_timestamp_us, &error);
    Error::Handle(&error);
  }
};

// Frame type identification implementation.
template<typename T>
bool Frame::Is() const {
  switch (this->GetType()) {
    case kDtFrameIrLeft:
    case kDtFrameIrRight:
    case kDtFrameIr:
      return (typeid(T) == typeid(IrFrame) || typeid(T) == typeid(VideoFrame));
    case kDtFrameDepth:
      return (typeid(T) == typeid(DepthFrame) || typeid(T) == typeid(VideoFrame));
    case kDtFrameColor:
    case kDtFrameColorLeft:
    case kDtFrameColorRight:
      return (typeid(T) == typeid(ColorFrame) || typeid(T) == typeid(VideoFrame));
    case kDtFrameGyro:
      return (typeid(T) == typeid(GyroFrame));
    case kDtFrameAccel:
      return (typeid(T) == typeid(AccelFrame));
    case kDtFramePoints:
      return (typeid(T) == typeid(PointsFrame));
    case kDtFrameRawPhase:
      return (typeid(T) == typeid(RawPhaseFrame) || typeid(T) == typeid(VideoFrame));
    case kDtFrameSet:
      return (typeid(T) == typeid(FrameSet));
    case kDtFramePalm:
      return (typeid(T) == typeid(PalmFrame));
    default:
      std::cout << "dt::Frame::is() did not catch frame type: " << (int) this->GetType()
                << std::endl;
      break;
  }
  return false;
}

}  // namespace dt

#endif  // DEPTRUM_HPP_DT_FRAME_HPP_
