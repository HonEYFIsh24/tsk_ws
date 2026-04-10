// Copyright (c) Deptrum Inc. All Rights Reserved.

/**
 * @file dt_filter.hpp
 * @brief Defines the Filter class, the core processing unit for point cloud generation,
 *        format conversion, and other operations in the SDK.
 */

#ifndef DEPTRUM_HPP_DT_FILTER_HPP_
#define DEPTRUM_HPP_DT_FILTER_HPP_

#include <cstring>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>
#include "deptrum/h/dt_filter.h"
#include "deptrum/h/dt_frame.h"
#include "dt_error.hpp"
#include "dt_frame.hpp"
#include "dt_types.hpp"

namespace dt {

/**
 * @brief Callback function type for filter processing results.
 * @param frame Shared pointer to the processed Frame object.
 */
typedef std::function<void(std::shared_ptr<Frame>)> FilterCallback;

// Forward declarations and helper templates remain unchanged
template<typename T>
struct RangeTraits {
  using value_type = void;
};

template<>
struct RangeTraits<DtUint8OptionRange> {
  using value_type = uint8_t;
};

template<>
struct RangeTraits<DtUint16OptionRange> {
  using value_type = uint16_t;
};

template<>
struct RangeTraits<DtIntOptionRange> {
  using value_type = uint32_t;
};

template<>
struct RangeTraits<DtFloatOptionRange> {
  using value_type = float;
};

/**
 * @brief Get T Property Range.
 */
template<typename T>
T GetOptionRange(const DtFilterConfigSchemaItem& item, const double cur) {
  // If T type is illegal, T will be void
  using value_type = typename RangeTraits<T>::value_type;
  T range{};
  // Compilate error will be reported here if T is void
  range.cur = static_cast<value_type>(cur);
  range.def = static_cast<value_type>(item.def);
  range.max = static_cast<value_type>(item.max);
  range.min = static_cast<value_type>(item.min);
  range.step = static_cast<value_type>(item.step);
  return range;
}

/**
 * @brief Base class for all processing filters in the SDK.
 *
 * @details Provides core functionality for frame processing, configuration,
 *          and state management. Derived classes implement specific processing logic.
 */
class Filter : public std::enable_shared_from_this<Filter> {
 public:
  /**
   * @brief Constructs a filter from an existing implementation handle.
   *
   * @param[in] impl Raw pointer to the underlying dt_filter implementation.
   */
  explicit Filter(dt_filter* impl) { Init(impl); }

  /**
   * @brief Virtual destructor ensures proper cleanup of resources.
   */
  virtual ~Filter() noexcept {
    if (impl_ != nullptr) {
      dt_delete_filter(impl_);
    }
  }

  /**
   * @brief Gets the underlying implementation handle.
   *
   * @return dt_filter* Raw pointer to the filter implementation.
   */
  dt_filter* GetImpl() const { return impl_; }

  /**
   * @brief Gets the filter type name.
   *
   * @return const std::string& Reference to the filter's name string.
   */
  virtual const std::string& GetName() const { return name_; }

  /**
   * @brief Resets the filter to its initial state.
   *
   * @details Clears internal caches, stops processing threads,
   *          and flushes pending frames for asynchronous processing.
   */
  virtual void Reset() const {
    dt_error* error = nullptr;
    dt_filter_reset(impl_, &error);
    Error::Handle(&error);
  }

  /**
   * @brief Enables or disables the filter.
   *
   * @param[in] enable True to enable processing, false to disable.
   */
  virtual void Enable(bool enable) const {
    dt_error* error = nullptr;
    dt_filter_enable(impl_, enable, &error);
    Error::Handle(&error);
  }

  /**
   * @brief Checks if the filter is currently enabled.
   *
   * @return bool True if enabled, false otherwise.
   */
  virtual bool IsEnabled() const {
    dt_error* error = nullptr;
    bool enable = dt_filter_is_enabled(impl_, &error);
    Error::Handle(&error);
    return enable;
  }

  /**
   * @brief Processes a frame synchronously.
   *
   * @param[in] frame Input frame to be processed.
   * @return std::shared_ptr<Frame> Processed output frame.
   */
  virtual std::shared_ptr<Frame> Process(std::shared_ptr<const Frame> frame) const {
    dt_error* error = nullptr;
    auto result = dt_filter_process(impl_, frame->GetImpl(), &error);
    Error::Handle(&error);
    return result ? std::make_shared<Frame>(result) : nullptr;
  }

  /**
   * @brief Queues a frame for asynchronous processing.
   *
   * @param[in] frame Input frame to queue for processing.
   */
  virtual void PushFrame(std::shared_ptr<Frame> frame) const {
    dt_error* error = nullptr;
    dt_filter_push_frame(impl_, frame->GetImpl(), &error);
    Error::Handle(&error);
  }

  /**
   * @brief Sets the callback for asynchronous processing results.
   *
   * @param[in] callback Callback function to handle processed frames.
   */
  virtual void SetCallBack(FilterCallback callback) {
    callback_ = callback;
    dt_error* error = nullptr;
    dt_filter_set_callback(impl_, &Filter::DtFilterCallback, this, &error);
    Error::Handle(&error);
  }

  /**
   * @brief Gets the configuration schema in CSV format.
   *
   * @details Schema format:
   *          Parameter name, Type ("int"/"float"/"bool"),
   *          Min value, Max value, Value step, Default value, Description
   *
   * @return std::string CSV-formatted configuration schema.
   */
  virtual std::string GetConfigSchema() const {
    dt_error* error = nullptr;
    auto schema = dt_filter_get_config_schema(impl_, &error);
    Error::Handle(&error);
    return schema;
  }

  /**
   * @brief Gets the configuration schema as structured data.
   *
   * @return std::vector<DtFilterConfigSchemaItem> Vector of configuration items.
   */
  virtual std::vector<DtFilterConfigSchemaItem> GetConfigSchemaVec() const {
    return config_schema_vec_;
  }

  /**
   * @brief Sets a configuration value by name.
   *
   * @param[in] config_name Name of the configuration parameter.
   * @param[in] value Configuration value (internally cast to appropriate type).
   */
  virtual void SetConfigValue(const std::string& config_name, double value) const {
    dt_error* error = nullptr;
    dt_filter_set_config_value(impl_, config_name.c_str(), value, &error);
    Error::Handle(&error);
  }

  /**
   * @brief Gets a configuration value by name.
   *
   * @param[in] config_name Name of the configuration parameter.
   * @return double Current value of the parameter (cast to double).
   */
  virtual double GetConfigValue(const std::string& config_name) const {
    dt_error* error = nullptr;
    double value = dt_filter_get_config_value(impl_, config_name.c_str(), &error);
    Error::Handle(&error);
    return value;
  }

  /**
   * @brief Checks if the filter matches a specific derived type.
   *
   * @tparam T Target filter type to check against.
   * @return bool True if filter is of type T, false otherwise.
   */
  template<typename T>
  bool Is();

  /**
   * @brief Casts the filter to a specific derived type.
   *
   * @tparam T Target filter type for casting.
   * @return std::shared_ptr<T> Shared pointer to the casted filter.
   * @throws std::runtime_error If the filter is not of the requested type.
   */
  template<typename T>
  std::shared_ptr<T> As() {
    if (!Is<T>()) {
      throw std::runtime_error("Invalid filter cast: Type mismatch");
    }
    return std::static_pointer_cast<T>(shared_from_this());
  }

 protected:
  /**
   * @brief Default constructor for derived classes.
   */
  Filter() = default;

  /**
   * @brief Initializes the filter with implementation details.
   *
   * @param[in] impl Raw pointer to the underlying filter implementation.
   */
  virtual void Init(dt_filter* impl) {
    impl_ = impl;
    dt_error* error = nullptr;
    name_ = dt_filter_get_name(impl_, &error);
    Error::Handle(&error);

    auto config_schema_list = dt_filter_get_config_schema_list(impl_, &error);
    Error::Handle(&error);

    auto count = dt_filter_config_schema_list_get_count(config_schema_list, &error);
    Error::Handle(&error);

    for (uint32_t i = 0; i < count; i++) {
      auto item = dt_filter_config_schema_list_get_item(config_schema_list, i, &error);
      Error::Handle(&error);
      config_schema_vec_.emplace_back(item);
    }

    dt_delete_filter_config_schema_list(config_schema_list);
  }

 protected:
  dt_filter* impl_ = nullptr;                                ///< Underlying filter implementation
  std::string name_;                                         ///< Filter type name
  FilterCallback callback_;                                  ///< Asynchronous processing callback
  std::vector<DtFilterConfigSchemaItem> config_schema_vec_;  ///< Configuration schema

 private:
  /**
   * @brief Internal callback adapter for C-style callbacks.
   *
   * @param[in] frame Raw pointer to processed frame.
   * @param[in] user_data Pointer to the owning Filter instance.
   */
  static void DtFilterCallback(dt_frame* frame, void* user_data) {
    auto filter = static_cast<Filter*>(user_data);
    if (filter && filter->callback_) {
      filter->callback_(std::make_shared<Frame>(frame));
    }
  }
};

/**
 * @brief Factory class for creating filter instances.
 */
class FilterFactory {
 public:
  /**
   * @brief Creates a filter by type name.
   *
   * @param[in] name Type name of the filter to create.
   * @return std::shared_ptr<Filter> Newly created filter instance.
   */
  static std::shared_ptr<Filter> CreateFilter(const std::string& name) {
    dt_error* error = nullptr;
    auto impl = dt_create_filter(name.c_str(), &error);
    Error::Handle(&error);
    return std::make_shared<Filter>(impl);
  }
};

/**
 * @brief Specialized filter for point cloud generation.
 */
class PointCloudFilter : public Filter {
 public:
  /**
   * @brief Constructs a PointCloudFilter instance.
   */
  PointCloudFilter() {
    dt_error* error = nullptr;
    auto impl = dt_create_filter("PointCloudFilter", &error);
    Error::Handle(&error);
    Init(impl);
  }

  /**
   * @brief Virtual destructor for proper cleanup.
   */
  ~PointCloudFilter() noexcept override = default;

  /**
   * @brief Sets the output point cloud format.
   *
   * @param[in] format Target point cloud format (kDtFormatPoint or kDtFormatRgbPoint).
   */
  void SetCreatePointFormat(DtFormat format) {
    SetConfigValue("point_format", static_cast<double>(format));
  }

  /**
   * @brief Sets the coordinate scaling factor for point cloud data.
   *
   * @param[in] factor Scaling factor for coordinate values.
   */
  void SetCoordinateDataScaled(float factor) { SetConfigValue("coordinate_data_scale", factor); }

  /**
   * @brief Set point cloud color data normalization.
   * @brief If normalization is required, the output point cloud frame's color data will be
   * normalized to the range [0, 1].
   *
   * @attention This function only works for when create point format is set to kDtFormatRgbPoint.
   *
   * @param[in] state Whether normalization is required.
   */
  void SetColorDataNormalization(bool state) { SetConfigValue("color_data_normalization", state); }

  /**
   * @brief Set the point cloud coordinate system.
   *
   * @param[in] type The coordinate system type.
   */
  void SetCoordinateSystem(DtCoordinateSystemType type) {
    SetConfigValue("coordinate_system_type", static_cast<double>(type));
  }

  /**
   * @brief Set the output point cloud zero point.
   *
   * @param[in] state If true, the output point cloud with zero point; otherwise, the output point
   * cloud will not have zero point.
   */
  void SetOutputZeroPoint(bool state) { SetConfigValue("output_zero_point", state); }
};

/**
 * @brief Align for depth to other or other to depth.
 */
class Align : public Filter {
 public:
  Align(DtStreamType alignToStreamType) {
    dt_error* error = nullptr;
    auto impl = dt_create_filter("Align", &error);
    Error::Handle(&error);
    Init(impl);

    SetConfigValue("align_type", static_cast<double>(alignToStreamType));
  }

  ~Align() noexcept override = default;

  DtStreamType GetAlignToStreamType() {
    return static_cast<DtStreamType>(static_cast<int>(GetConfigValue("align_type")));
  }

  /**
   * @brief Sets whether the output frame resolution should match the target resolution.
   *        When enabled, the output frame resolution will be adjusted to match (same as) the target
   * resolution. When disabled, the output frame resolution will match the original resolution while
   * maintaining the aspect ratio of the target resolution.
   *
   * @param[in] state If true, output frame resolution will match the target resolution; otherwise,
   * it will maintain the original resolution with the target's aspect ratio.
   */
  void SetMatchTargetResolution(bool state) { SetConfigValue("match_target_res", state); }

  /**
   * @brief Set the Align To Stream Profile
   * @brief  It is useful when the align target stream dose not started (without any frame to get
   * intrinsics and extrinsics).
   *
   * @param[in] profile The Align To Stream Profile.
   */
  void SetAlignToStreamProfile(std::shared_ptr<const StreamProfile> profile) {
    dt_error* error = nullptr;
    dt_align_filter_set_align_to_stream_profile(impl_, profile->GetImpl(), &error);
    Error::Handle(&error);
  }
};

/**
 * @brief The FormatConvertFilter class is a subclass of Filter that performs format conversion.
 */
class FormatConvertFilter : public Filter {
 public:
  FormatConvertFilter() {
    dt_error* error = nullptr;
    auto impl = dt_create_filter("FormatConverter", &error);
    Error::Handle(&error);
    Init(impl);
  }

  ~FormatConvertFilter() noexcept override = default;

  /**
   * @brief Set the format conversion type.
   *
   * @param[in] type The format conversion type.
   */
  void SetFormatConvertType(DtConvertFormat type) {
    SetConfigValue("convert_type", static_cast<double>(type));
  }
};

/**
 * @brief Decimation filter, reducing complexity by subsampling depth maps and losing depth details.
 */
class DecimationFilter : public Filter {
 public:
  DecimationFilter() {
    dt_error* error = nullptr;
    auto impl = dt_create_filter("DecimationFilter", &error);
    Error::Handle(&error);
    Init(impl);
  }

  ~DecimationFilter() noexcept override = default;

  /**
   * @brief Set the decimation filter scale value.
   *
   * @param[in] value The decimation filter scale value.
   */
  void SetScaleValue(uint8_t value) { SetConfigValue("decimate", static_cast<double>(value)); }

  /**
   * @brief Get the decimation filter scale value.
   */
  uint8_t GetScaleValue() { return static_cast<uint8_t>(GetConfigValue("decimate")); }

  /**
   * @brief Get the option range of the decimation filter scale value.
   */
  DtUint8OptionRange GetScaleRange() {
    DtUint8OptionRange scale_range{};
    if (config_schema_vec_.size() != 0) {
      const auto& item = config_schema_vec_[0];
      scale_range = GetOptionRange<DtUint8OptionRange>(item, GetConfigValue("decimate"));
    }
    return scale_range;
  }
};

/**
 * @brief Creates depth Thresholding filter
 * By controlling min and max options on the block.
 */
class ThresholdFilter : public Filter {
 public:
  ThresholdFilter() {
    dt_error* error = nullptr;
    auto impl = dt_create_filter("ThresholdFilter", &error);
    Error::Handle(&error);
    Init(impl);
  }

  ~ThresholdFilter() noexcept override = default;

  /**
   * @brief Get the threshold filter min range.
   *
   * @return DtIntOptionRange The range of the threshold filter min.
   */
  DtIntOptionRange GetMinRange() {
    DtIntOptionRange range{};
    const auto& schema_vec = GetConfigSchemaVec();
    for (const auto& item : schema_vec) {
      if (strcmp(item.name, "min") == 0) {
        range = GetOptionRange<DtIntOptionRange>(item, GetConfigValue("min"));
        break;
      }
    }
    return range;
  }

  /**
   * @brief Get the threshold filter max range.
   *
   * @return DtIntOptionRange The range of the threshold filter max.
   */
  DtIntOptionRange GetMaxRange() {
    DtIntOptionRange range{};
    const auto& schema_vec = GetConfigSchemaVec();
    for (const auto& item : schema_vec) {
      if (strcmp(item.name, "max") == 0) {
        range = GetOptionRange<DtIntOptionRange>(item, GetConfigValue("max"));
        break;
      }
    }
    return range;
  }

  /**
   * @brief Set the threshold filter max and min range.
   *
   * @param[in] min The threshold filter min range.
   * @param[in] max The threshold filter max range.
   */
  bool SetValueRange(uint16_t min, uint16_t max) {
    if (min >= max) {
      return false;
    }
    SetConfigValue("min", min);
    SetConfigValue("max", max);
    return true;
  }
};

class DtFilterList {
 public:
  explicit DtFilterList(dt_filter_list_t* impl) : impl_(impl) {}

  ~DtFilterList() noexcept { dt_delete_filter_list(impl_); }

  /**
   * @brief Get the number of filters.
   *
   * @return uint32_t The number of filters.
   */
  uint32_t GetCount() const {
    dt_error* error = nullptr;
    auto count = dt_filter_list_get_count(impl_, &error);
    Error::Handle(&error);
    return count;
  }

  /**
   * @brief Get the Filter object at the specified index.
   *
   * @param[in] index The filter index. The range is [0, count-1]. If the index exceeds the range,
   * an exception will be thrown.
   * @return std::shared_ptr<Filter> The filter object.
   */
  std::shared_ptr<Filter> GetFilter(uint32_t index) {
    dt_error* error = nullptr;
    auto filter = dt_filter_list_get_filter(impl_, index, &error);
    Error::Handle(&error);
    return std::make_shared<Filter>(filter);
  }

 private:
  dt_filter_list_t* impl_ = nullptr;
};

/**
 * @brief Define the Filter type map
 */
static const std::unordered_map<std::string, std::type_index> dt_filter_type_map = {
    {"PointCloudFilter", typeid(PointCloudFilter)},
    {"Align", typeid(Align)},
    {"FormatConverter", typeid(FormatConvertFilter)},
    {"DecimationFilter", typeid(DecimationFilter)},
    {"ThresholdFilter", typeid(ThresholdFilter)}};

/**
 * @brief Define the is() template function for the Filter class.
 */
template<typename T>
bool Filter::Is() {
  std::string name = GetName();
  auto it = dt_filter_type_map.find(name);
  if (it != dt_filter_type_map.end()) {
    return std::type_index(typeid(T)) == it->second;
  }
  return false;
}

}  // namespace dt

#endif  // DEPTRUM_HPP_DT_FILTER_HPP_
