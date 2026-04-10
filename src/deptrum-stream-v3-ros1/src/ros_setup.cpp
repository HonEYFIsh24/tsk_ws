// Copyright (c) Deptrum Inc. All Rights Reserved.

#include "deptrum_camera/dt_camera_node.h"
#include "deptrum_camera/utils.h"
#include <std_msgs/String.h>

namespace deptrum_camera {

void DtCameraNode::SetupConfig() {
  stream_name_[DEPTH] = "depth";
  unit_step_size_[DEPTH] = sizeof(uint16_t);
  format_[DEPTH] = kDtFormatY16;
  image_format_[DEPTH] = CV_16UC1;
  encoding_[DEPTH] = sensor_msgs::image_encodings::TYPE_16UC1;
  format_str_[DEPTH] = "Y16";

  stream_name_[COLOR] = "color";
  unit_step_size_[COLOR] = 3;
  format_[COLOR] = kDtFormatRgb;
  image_format_[COLOR] = CV_8UC3;
  encoding_[COLOR] = sensor_msgs::image_encodings::RGB8;
  format_str_[COLOR] = "RGB";

  stream_name_[INFRA0] = "ir";
  unit_step_size_[INFRA0] = sizeof(uint16_t);
  format_[INFRA0] = kDtFormatY16;
  image_format_[INFRA0] = CV_16UC1;
  encoding_[INFRA0] = sensor_msgs::image_encodings::MONO16;
  format_str_[INFRA0] = "Y16";

  stream_name_[INFRA1] = "left_ir";
  unit_step_size_[INFRA1] = sizeof(uint16_t);
  format_[INFRA1] = kDtFormatY16;
  image_format_[INFRA1] = CV_16UC1;
  encoding_[INFRA1] = sensor_msgs::image_encodings::MONO16;
  format_str_[INFRA1] = "Y16";

  stream_name_[INFRA2] = "right_ir";
  unit_step_size_[INFRA2] = sizeof(uint16_t);
  format_[INFRA2] = kDtFormatY16;
  image_format_[INFRA2] = CV_16UC1;
  encoding_[INFRA2] = sensor_msgs::image_encodings::MONO16;
  format_str_[INFRA2] = "Y16";
}

void DtCameraNode::SelectBaseStream() {
  if (enable_stream_[DEPTH]) {
    base_stream_ = DEPTH;
  } else if (enable_stream_[INFRA0]) {
    base_stream_ = INFRA0;
  } else if (enable_stream_[INFRA1]) {
    base_stream_ = INFRA1;
  } else if (enable_stream_[INFRA2]) {
    base_stream_ = INFRA2;
  } else if (enable_stream_[COLOR]) {
    base_stream_ = COLOR;
  } else {
    ROS_ERROR_STREAM("No base stream is enabled!");
  }
}
void DtCameraNode::SetupColorPostProcessFilter() {
  auto device_info = device_->GetDeviceInfo();
  CHECK_NOTNULL(device_info);
  auto pid = device_info->GetPid();
  if (IsNebulaSeriesPid(pid)) {
    ROS_ERROR_STREAM("Nebula series not support SENSOR_COLOR");
    return;
  }

  auto color_sensor = device_->GetSensor(kDtSensorColor);
  color_filter_list_ = color_sensor->CreateRecommendedFilters();
  if (color_filter_list_.empty()) {
    ROS_ERROR_STREAM("Failed to get color sensor filter list");
    // return;
  }
  for (size_t i = 0; i < color_filter_list_.size(); i++) {
    auto filter = color_filter_list_[i];
    std::map<std::string, bool> filter_params = {
        {"DecimationFilter", enable_color_decimation_filter_},
    };
    std::string filter_name = filter->GetName();
    ROS_INFO_STREAM("Setting " << filter_name << "......");
    if (filter_params.find(filter_name) != filter_params.end()) {
      std::string value = filter_params[filter_name] ? "true" : "false";
      ROS_INFO_STREAM("set color " << filter_name << " to " << value);
      filter->Enable(filter_params[filter_name]);
    }
    if (filter_name == "DecimationFilter" && enable_color_decimation_filter_) {
      auto decimation_filter = filter->As<dt::DecimationFilter>();
      auto range = decimation_filter->GetScaleRange();
      if (color_decimation_filter_scale_ != -1 && color_decimation_filter_scale_ <= range.max &&
          color_decimation_filter_scale_ >= range.min) {
        ROS_INFO_STREAM("Set color decimation filter scale value to "
                        << color_decimation_filter_scale_);
        decimation_filter->SetScaleValue(color_decimation_filter_scale_);
      }
      if (color_decimation_filter_scale_ != -1 && (color_decimation_filter_scale_ < range.min ||
                                                   color_decimation_filter_scale_ > range.max)) {
        ROS_ERROR_STREAM("Color Decimation filter scale value is out of range "
                         << range.min << " - " << range.max);
      }
    }
  }
}

void DtCameraNode::SetupDepthPostProcessFilter() {
  // set depth sensor to filter
  auto depth_sensor = device_->GetSensor(kDtSensorDepth);
  depth_filter_list_ = depth_sensor->CreateRecommendedFilters();
  if (depth_filter_list_.empty()) {
    ROS_WARN_STREAM("Failed to get depth sensor filter list");
    return;
  }
  for (size_t i = 0; i < depth_filter_list_.size(); i++) {
    auto filter = depth_filter_list_[i];
    std::map<std::string, bool> filter_params = {
        {"DecimationFilter", enable_decimation_filter_},
        {"HDRMerge", enable_hdr_merge_},
        {"SequenceIdFilter", enable_sequenced_filter_},
        {"SpatialAdvancedFilter", enable_spatial_filter_},
        {"TemporalFilter", enable_temporal_filter_},
        {"HoleFillingFilter", enable_hole_filling_filter_},
        {"DisparityTransform", enable_disaparity_to_depth_},
        {"ThresholdFilter", enable_threshold_filter_},
    };
    std::string filter_name = filter->GetName();
    ROS_INFO_STREAM("Setting " << filter_name << "......");
    if (filter_params.find(filter_name) != filter_params.end()) {
      std::string value = filter_params[filter_name] ? "true" : "false";
      ROS_INFO_STREAM("set " << filter_name << " to " << value);
      filter->Enable(filter_params[filter_name]);
      filter_status_[filter_name] = filter_params[filter_name];
    }
    if (filter_name == "DecimationFilter" && enable_decimation_filter_) {
      auto decimation_filter = filter->As<dt::DecimationFilter>();
      if (decimation_filter_scale_range_ != -1) {
        decimation_filter->SetScaleValue(decimation_filter_scale_range_);
      }
    } else if (filter_name == "ThresholdFilter" && enable_threshold_filter_) {
      auto threshold_filter = filter->As<dt::ThresholdFilter>();
      if (threshold_filter_min_ != -1 && threshold_filter_max_ != -1) {
        threshold_filter->SetValueRange(threshold_filter_min_, threshold_filter_max_);
      }
    } 
    else {
      ROS_INFO_STREAM("Skip setting " << filter_name);
    }
  }
  set_filter_srv_ = nh_.advertiseService<SetFilterRequest, SetFilterResponse>(
      "/" + camera_name_ + "/" + "set_filter",
      [this](SetFilterRequest& request, SetFilterResponse& response) {
        response.success = SetFilterCallback(request, response);
        return response.success;
      });
}
void DtCameraNode::SetupDevices() {
  auto sensor_list = device_->GetSensorList();
  for (size_t i = 0; i < sensor_list->GetCount(); i++) {
    auto sensor = sensor_list->GetSensor(i);
    auto profiles = sensor->GetStreamProfileList();
    for (size_t j = 0; j < profiles->GetCount(); j++) {
      auto profile = profiles->GetProfile(j);
      stream_index_pair sip{profile->GetType(), 0};
      if (sensors_.find(sip) == sensors_.end()) {
        sensors_[sip] = std::make_shared<RosDtSensor>(device_, sensor, stream_name_[sip]);
      }
      if (imu_sensor_.find(sip) == imu_sensor_.end()) {
        imu_sensor_[sip] = sensor;
      }
    }
  }
  for (const auto& item : enable_stream_) {
    auto stream_index = item.first;
    auto enable = item.second;
    if (enable && sensors_.find(stream_index) == sensors_.end()) {
      ROS_INFO_STREAM(stream_name_[stream_index]
                      << "sensor isn't supported by current device! -- Skipping...");
      enable_stream_[stream_index] = false;
    }
    if (enable) {
      // set rotation
      DtOptionId rotationPropertyID = kDtOptionDepthRotateInt;
      if (stream_index == COLOR) {
        rotationPropertyID = kDtOptionColorRotateInt;
      } else if (stream_index == DEPTH) {
        rotationPropertyID = kDtOptionDepthRotateInt;
      } else if (stream_index == INFRA0) {
        rotationPropertyID = kDtOptionIrRotateInt;
      } else if (stream_index == INFRA1) {
        rotationPropertyID = kDtOptionIrRotateInt;
      } else if (stream_index == INFRA2) {
        rotationPropertyID = kDtOptionIrRightRotateInt;
      }
      if (image_rotation_[stream_index] != -1 &&
          device_->IsOptionSupported(rotationPropertyID, kDtPermissionWrite)) {
        ROS_INFO_STREAM("Setting " << stream_name_[stream_index] << " rotation to "
                                   << image_rotation_[stream_index]);
        device_->SetIntOption(rotationPropertyID, image_rotation_[stream_index]);
      }
      // set flip
      DtOptionId flipPropertyID = kDtOptionDepthFlipBool;
      if (stream_index == COLOR) {
        flipPropertyID = kDtOptionColorFlipBool;
      } else if (stream_index == DEPTH) {
        flipPropertyID = kDtOptionDepthFlipBool;
      } else if (stream_index == INFRA0) {
        flipPropertyID = kDtOptionIrFlipBool;
      } else if (stream_index == INFRA1) {
        flipPropertyID = kDtOptionIrFlipBool;
      } else if (stream_index == INFRA2) {
        flipPropertyID = kDtOptionIrRightFlipBool;
      }
      if (device_->IsOptionSupported(flipPropertyID, kDtPermissionWrite)) {
        ROS_INFO_STREAM("Setting " << stream_name_[stream_index] << " flip to "
                                   << (image_flip_[stream_index] ? "ON" : "OFF"));
        device_->SetBoolOption(flipPropertyID, image_flip_[stream_index]);
      }
      // set mirror
      DtOptionId mirrorPropertyID = kDtOptionDepthMirrorBool;
      if (stream_index == COLOR) {
        mirrorPropertyID = kDtOptionColorMirrorBool;
      } else if (stream_index == DEPTH) {
        mirrorPropertyID = kDtOptionDepthMirrorBool;
      } else if (stream_index == INFRA0) {
        mirrorPropertyID = kDtOptionIrMirrorBool;
      } else if (stream_index == INFRA1) {
        mirrorPropertyID = kDtOptionIrMirrorBool;
      } else if (stream_index == INFRA2) {
        mirrorPropertyID = kDtOptionIrRightMirrorBool;
      }
      if (device_->IsOptionSupported(mirrorPropertyID, kDtPermissionWrite)) {
        ROS_INFO_STREAM("Setting " << stream_name_[stream_index] << " mirror to "
                                   << (image_mirror_[stream_index] ? "ON" : "OFF"));
        device_->SetBoolOption(mirrorPropertyID, image_mirror_[stream_index]);
      }
    }
  }
  if (enable_d2c_viewer_) {
    d2c_viewer_ = std::make_shared<D2CViewer>(nh_, nh_private_);
  }
  CHECK_NOTNULL(device_info_.get());
  if (enable_pipeline_) {
    pipeline_ = std::make_shared<dt::Pipeline>(device_);
  }
  if (enable_sync_output_accel_gyro_) {
    imuPipeline_ = std::make_shared<dt::Pipeline>(device_);
  }

  try {
    CheckAndSetDepthMode();
    CheckAndSetDeviceMode();
    SetDepthCropExposureRoi();

    ROS_INFO_STREAM("test---- kDtOptionHeartbeatBool");
    if (device_->IsOptionSupported(kDtOptionHeartbeatBool, kDtPermissionReadWrite)) {
      ROS_INFO_STREAM("Setting heartbeat to " << (enable_heartbeat_ ? "true" : "false"));
      device_->SetBoolOption(kDtOptionHeartbeatBool, enable_heartbeat_);
    }

    ROS_INFO_STREAM("test---- kDtOptionColorAutoExposurePriorityInt");
    if (device_->IsOptionSupported(kDtOptionColorAutoExposurePriorityInt, kDtPermissionWrite)) {
      int set_enable_color_auto_exposure_priority = enable_color_auto_exposure_priority_ ? 1 : 0;
      ROS_INFO_STREAM("Setting color auto exposure priority to "
                      << (set_enable_color_auto_exposure_priority ? "ON" : "OFF"));
      device_->SetIntOption(kDtOptionColorAutoExposurePriorityInt,
                            set_enable_color_auto_exposure_priority);
    }
    if (device_->IsOptionSupported(kDtOptionColorAutoWhiteBalanceBool, kDtPermissionWrite)) {
      ROS_INFO_STREAM("Setting color auto white balance to "
                      << (enable_color_auto_white_balance_ ? "ON" : "OFF"));
      device_->SetBoolOption(kDtOptionColorAutoWhiteBalanceBool, enable_color_auto_white_balance_);
    }

    ROS_INFO_STREAM("test---- kDtOptionColorBacklightCompensationInt");
    if (device_->IsOptionSupported(kDtOptionColorBacklightCompensationInt, kDtPermissionWrite)) {
      int set_enable_color_backlight_compenstation = enable_color_backlight_compenstation_ ? 1 : 0;
      ROS_INFO_STREAM("Setting color backlight compenstation to "
                      << (set_enable_color_backlight_compenstation ? "ON" : "OFF"));
      device_->SetIntOption(kDtOptionColorBacklightCompensationInt,
                            set_enable_color_backlight_compenstation);
    }
    if (!color_powerline_freq_.empty() &&
        device_->IsOptionSupported(kDtOptionColorPowerLineFrequencyInt, kDtPermissionWrite)) {
      if (color_powerline_freq_ == "disable") {
        device_->SetIntOption(kDtOptionColorPowerLineFrequencyInt, 0);
      } else if (color_powerline_freq_ == "50hz") {
        device_->SetIntOption(kDtOptionColorPowerLineFrequencyInt, 1);
      } else if (color_powerline_freq_ == "60hz") {
        device_->SetIntOption(kDtOptionColorPowerLineFrequencyInt, 2);
      } else if (color_powerline_freq_ == "auto") {
        device_->SetIntOption(kDtOptionColorPowerLineFrequencyInt, 3);
      }
      ROS_INFO_STREAM("Setting color powerline freq to " << color_powerline_freq_);
    }

    ROS_INFO_STREAM("test---- kDtOptionColorAutoExposureBool");
    if (device_->IsOptionSupported(kDtOptionColorAutoExposureBool, kDtPermissionReadWrite)) {
      device_->SetBoolOption(kDtOptionColorAutoExposureBool, enable_color_auto_exposure_);
    }
    if (color_exposure_ != -1 &&
        device_->IsOptionSupported(kDtOptionColorExposureInt, kDtPermissionReadWrite)) {
      device_->SetIntOption(kDtOptionColorExposureInt, color_exposure_);
    }
    if (color_gain_ != -1 &&
        device_->IsOptionSupported(kDtOptionColorGainInt, kDtPermissionWrite)) {
      auto range = device_->GetIntOptionRange(kDtOptionColorGainInt);
      if (color_gain_ < range.min || color_gain_ > range.max) {
        ROS_ERROR_STREAM("color gain value is out of range[" << range.min << "," << range.max
                                                             << "]please check the value");
      } else {
        ROS_INFO_STREAM("Setting color gain to " << color_gain_);
        device_->SetIntOption(kDtOptionColorGainInt, color_gain_);
      }
    }

    ROS_INFO_STREAM("test---- kDtOptionColorBrightnessInt");
    if (color_brightness_ != -1 &&
        device_->IsOptionSupported(kDtOptionColorBrightnessInt, kDtPermissionWrite)) {
      auto range = device_->GetIntOptionRange(kDtOptionColorBrightnessInt);
      if (color_brightness_ < range.min || color_brightness_ > range.max) {
        ROS_ERROR_STREAM("color brightness value is out of range[" << range.min << "," << range.max
                                                                   << "]please check the value");
      } else {
        ROS_INFO_STREAM("Setting color brightness to " << color_brightness_);
        device_->SetIntOption(kDtOptionColorBrightnessInt, color_brightness_);
      }
    }
    if (color_sharpness_ != -1 &&
        device_->IsOptionSupported(kDtOptionColorSharpnessInt, kDtPermissionWrite)) {
      auto range = device_->GetIntOptionRange(kDtOptionColorSharpnessInt);
      if (color_sharpness_ < range.min || color_sharpness_ > range.max) {
        ROS_ERROR_STREAM("color sharpness value is out of range[" << range.min << "," << range.max
                                                                  << "]please check the value");
      } else {
        ROS_INFO_STREAM("Setting color sharpness to " << color_sharpness_);
        device_->SetIntOption(kDtOptionColorSharpnessInt, color_sharpness_);
      }
    }

    ROS_INFO_STREAM("test---- kDtOptionColorGammaInt");
    if (color_gamma_ != -1 &&
        device_->IsOptionSupported(kDtOptionColorGammaInt, kDtPermissionWrite)) {
      auto range = device_->GetIntOptionRange(kDtOptionColorGammaInt);
      if (color_gamma_ < range.min || color_gamma_ > range.max) {
        ROS_ERROR_STREAM("color gamm value is out of range[" << range.min << "," << range.max
                                                             << "]please check the value");
      } else {
        ROS_INFO_STREAM("Setting color gamm to " << color_gamma_);
        device_->SetIntOption(kDtOptionColorGammaInt, color_gamma_);
      }
    }
    if (color_white_balance_ != -1 &&
        device_->IsOptionSupported(kDtOptionColorWhiteBalanceInt, kDtPermissionWrite)) {
      auto range = device_->GetIntOptionRange(kDtOptionColorWhiteBalanceInt);
      if (color_white_balance_ < range.min || color_white_balance_ > range.max) {
        ROS_ERROR_STREAM("color white balance value is out of range["
                         << range.min << "," << range.max << "]please check the value");
      } else {
        ROS_INFO_STREAM("Setting color white balance to " << color_white_balance_);
        device_->SetIntOption(kDtOptionColorWhiteBalanceInt, color_white_balance_);
      }
    }

    ROS_INFO_STREAM("test---- kDtOptionColorSaturationInt");
    if (color_saturation_ != -1 &&
        device_->IsOptionSupported(kDtOptionColorSaturationInt, kDtPermissionWrite)) {
      auto range = device_->GetIntOptionRange(kDtOptionColorSaturationInt);
      if (color_saturation_ < range.min || color_saturation_ > range.max) {
        ROS_ERROR_STREAM("color saturation value is out of range[" << range.min << "," << range.max
                                                                   << "]please check the value");
      } else {
        ROS_INFO_STREAM("Setting color saturation to " << color_saturation_);
        device_->SetIntOption(kDtOptionColorSaturationInt, color_saturation_);
      }
    }
    if (color_constrast_ != -1 &&
        device_->IsOptionSupported(kDtOptionColorContrastInt, kDtPermissionWrite)) {
      auto range = device_->GetIntOptionRange(kDtOptionColorContrastInt);
      if (color_constrast_ < range.min || color_constrast_ > range.max) {
        ROS_ERROR_STREAM("color constrast value is out of range[" << range.min << "," << range.max
                                                                  << "]please check the value");
      } else {
        ROS_INFO_STREAM("Setting color constrast to " << color_constrast_);
        device_->SetIntOption(kDtOptionColorContrastInt, color_constrast_);
      }
    }

    ROS_INFO_STREAM("test---- kDtOptionColorHueInt");
    if (color_hue_ != -1 && device_->IsOptionSupported(kDtOptionColorHueInt, kDtPermissionWrite)) {
      auto range = device_->GetIntOptionRange(kDtOptionColorHueInt);
      if (color_hue_ < range.min || color_hue_ > range.max) {
        ROS_ERROR_STREAM("color hue value is out of range[" << range.min << "," << range.max
                                                            << "]please check the value");
      } else {
        ROS_INFO_STREAM("Setting color hue to " << color_hue_);
        device_->SetIntOption(kDtOptionColorHueInt, color_hue_);
      }
    }
    if (color_ae_max_exposure_ != -1 &&
        device_->IsOptionSupported(kDtOptionColorAeMaxExposureInt, kDtPermissionWrite)) {
      auto range = device_->GetIntOptionRange(kDtOptionColorAeMaxExposureInt);
      if (color_ae_max_exposure_ < range.min || color_ae_max_exposure_ > range.max) {
        ROS_ERROR_STREAM("color AE max exposure value is out of range["
                         << range.min << "," << range.max << "]please check the value");
      } else {
        ROS_INFO_STREAM("Setting color AE max exposure to " << color_ae_max_exposure_);
        device_->SetIntOption(kDtOptionColorAeMaxExposureInt, color_ae_max_exposure_);
      }
    }

    ROS_INFO_STREAM("test---- kDtOptionDepthAutoExposureBool");
    if (device_->IsOptionSupported(kDtOptionDepthAutoExposureBool, kDtPermissionReadWrite)) {
      ROS_INFO_STREAM("test---- enable_depth_auto_exposure_ " << enable_depth_auto_exposure_);
      device_->SetBoolOption(kDtOptionDepthAutoExposureBool, enable_depth_auto_exposure_);

      auto device_info = device_->GetDeviceInfo();
      CHECK_NOTNULL(device_info);
      auto pid = device_info->GetPid();
      if (!enable_depth_auto_exposure_) {
        if (IsNebulaSeriesPid(pid)) {
          if (kDtOptionStofDepthExposureInt != depth_exposure_mode_ &&
              kDtOptionMtofDepthExposureInt != depth_exposure_mode_ && camera_mode_ != 0 &&
              camera_mode_ != 1 && camera_mode_ != 2) {
            throw std::runtime_error(
                "SetupDevices depth_exposure_mode_ is not 3 or 4, or camera_mode_ not 0,1,2");
          }

          if (camera_mode_ == 0 && depth_exposure_mode_ != kDtOptionMtofDepthExposureInt) {
            std::runtime_error("SetupDevices camera_mode_ is 0 and depth_exposure_mode_ is stof");
          }
          if ((depth_exposure_mode_ == kDtOptionMtofDepthExposureInt &&
               depth_exposure_mtof_value_ != -1) ||
              (depth_exposure_mode_ == kDtOptionStofDepthExposureInt &&
               depth_exposure_stof_value_ != -1)) {
            device_->SetIntOption((dt_option_id)depth_exposure_mode_,
                                  depth_exposure_mode_ == kDtOptionMtofDepthExposureInt
                                      ? depth_exposure_mtof_value_
                                      : depth_exposure_stof_value_);
            ROS_INFO_STREAM("test---- depth_exposure_mode_ "
                            << depth_exposure_mode_ << "value: " << depth_exposure_mtof_value_
                            << "  or " << depth_exposure_stof_value_);
          }
        } else {
          // 非410
          if (depth_exposure_ != -1 &&
              device_->IsOptionSupported(kDtOptionDepthExposureInt, kDtPermissionReadWrite)) {
            device_->SetIntOption(kDtOptionDepthExposureInt, depth_exposure_);
          }
        }
      } else {
        ROS_INFO_STREAM(
            "test---- enable_depth_auto_exposure_ is ture , do not set EXPOSURE_INT_VALUE");
      }
    }

    if (ir_exposure_ != -1 &&
        device_->IsOptionSupported(kDtOptionIrExposureInt, kDtPermissionReadWrite)) {
      device_->SetIntOption(kDtOptionIrExposureInt, ir_exposure_);
    }

    ROS_INFO_STREAM("test---- kDtOptionIrAeMaxExposureInt");
    if (ir_ae_max_exposure_ != -1 &&
        device_->IsOptionSupported(kDtOptionIrAeMaxExposureInt, kDtPermissionWrite)) {
      auto range = device_->GetIntOptionRange(kDtOptionIrAeMaxExposureInt);
      if (ir_ae_max_exposure_ < range.min || ir_ae_max_exposure_ > range.max) {
        ROS_ERROR_STREAM("IR AE max exposure value is out of range["
                         << range.min << "," << range.max << "]please check the value");
      } else {
        ROS_INFO_STREAM("Setting IR AE max exposure to " << ir_ae_max_exposure_);
        device_->SetIntOption(kDtOptionIrAeMaxExposureInt, ir_ae_max_exposure_);
      }
    }
    if (device_->IsOptionSupported(kDtOptionLaserControlInt, kDtPermissionReadWrite)) {
      device_->SetIntOption(kDtOptionLaserControlInt, enable_laser_);
    }
    if (device_->IsOptionSupported(kDtOptionLaserBool, kDtPermissionReadWrite)) {
      device_->SetIntOption(kDtOptionLaserBool, enable_laser_);
    }

    ROS_INFO_STREAM("test---- kDtOptionColorAutoExposureBool");
    if (device_->IsOptionSupported(kDtOptionColorAutoExposureBool, kDtPermissionWrite)) {
      device_->SetBoolOption(kDtOptionColorAutoExposureBool, enable_color_auto_exposure_);
    }

    if (device_->IsOptionSupported(kDtOptionIrAutoExposureBool, kDtPermissionWrite)) {
      device_->SetBoolOption(kDtOptionIrAutoExposureBool, enable_ir_auto_exposure_);
    }

  } catch (const dt::Error& e) {
    ROS_ERROR_STREAM("Failed to setup devices: " << e.GetMessage());
  } catch (const std::exception& e) {
    ROS_ERROR_STREAM("Failed to setup devices: " << e.what());
  }
}

void DtCameraNode::SetupFrameCallback() {
  for (const auto& stream_index : IMAGE_STREAMS) {
    if (enable_stream_[stream_index]) {
      ROS_INFO_STREAM("test---- SetupFrameCallback  can frame_callback_  enable_stream_: "
                      << enable_stream_[stream_index]);
      auto callback = [this, stream_index](std::shared_ptr<dt::Frame> frame) {
        this->OnNewFrameCallback(frame, stream_index);
      };
      frame_callback_[stream_index] = callback;
    }
  }
}

bool DtCameraNode::SetupFormatConvertType(DtFormat type) {
  switch (type) {
    case kDtFormatI420:
      format_convert_filter_.SetFormatConvertType(kDtFormatI420ToRgb);
      break;
    case kDtFormatYuyv:
      format_convert_filter_.SetFormatConvertType(kDtFormatYuyvToRgb);
      break;
    case kDtFormatNv21:
      format_convert_filter_.SetFormatConvertType(kDtFormatNv21ToRgb);
      break;
    case kDtFormatNv12:
      format_convert_filter_.SetFormatConvertType(kDtFormatNv12ToRgb);
      break;
    case kDtFormatUyvy:
      format_convert_filter_.SetFormatConvertType(kDtFormatUyvyToRgb);
      break;
    default:
      return false;
  }
  return true;
}

void DtCameraNode::PrintProfiles(const std::shared_ptr<dt::Sensor>& sensor) {
  auto profiles = sensor->GetStreamProfileList();
  for (size_t j = 0; j < profiles->GetCount(); j++) {
    auto origin_profile = profiles->GetProfile(j);
    if (sensor->GetType() == kDtSensorColor) {
      auto profile = origin_profile->As<dt::VideoStreamProfile>();
      ROS_INFO_STREAM("available color profile: "
                      << profile->GetWidth() << "x" << profile->GetHeight() << " "
                      << profile->GetFps() << "fps " << (int)profile->GetFormat());
    } else if (sensor->GetType() == kDtSensorDepth) {
      auto profile = origin_profile->As<dt::VideoStreamProfile>();
      ROS_INFO_STREAM("available depth profile: "
                      << profile->GetWidth() << "x" << profile->GetHeight() << " "
                      << profile->GetFps() << "fps " << (int)profile->GetFormat());
    } else if (sensor->GetType() == kDtSensorIr) {
      auto profile = origin_profile->As<dt::VideoStreamProfile>();
      ROS_INFO_STREAM("available ir profile: " << profile->GetWidth() << "x" << profile->GetHeight()
                                               << " " << profile->GetFps() << "fps "
                                               << (int)profile->GetFormat());
    } else if (sensor->GetType() == kDtSensorAccel) {
      auto profile = origin_profile->As<dt::AccelStreamProfile>();
      ROS_INFO_STREAM("available accel profile: sampleRate "
                      << SampleRateToString(profile->GetSampleRate()) << "  full scale_range "
                      << FullAccelScaleRangeToString(profile->GetFullScaleRange()));
    } else if (sensor->GetType() == kDtSensorGyro) {
      auto profile = origin_profile->As<dt::GyroStreamProfile>();
      ROS_INFO_STREAM("available gyro profile: sampleRate "
                      << SampleRateToString(profile->GetSampleRate()) << "  full scale_range "
                      << FullGyroScaleRangeToString(profile->GetFullScaleRange()));
    } else {
      ROS_INFO_STREAM("unknown profile: " << (int)sensor->GetType());
    }
  }
}

void DtCameraNode::SetupProfiles() {
  for (const auto& stream_index : IMAGE_STREAMS) {
    if (!enable_stream_[stream_index] && stream_index != base_stream_) {
      continue;
    }
    try {
      auto profile_list = sensors_[stream_index]->GetStreamProfileList();
      supported_profiles_[stream_index] = profile_list;
      std::shared_ptr<dt::VideoStreamProfile> selected_profile = nullptr;
      if (width_[stream_index] == 0 && height_[stream_index] == 0 && fps_[stream_index] == 0 &&
          format_[stream_index] == kDtFormatUnknown) {
        selected_profile = profile_list->GetProfile(0)->As<dt::VideoStreamProfile>();
      } else {
        selected_profile = profile_list->GetVideoStreamProfile(
            width_[stream_index], height_[stream_index], format_[stream_index], fps_[stream_index]);
      }

      auto default_profile = profile_list->GetProfile(0)->As<dt::VideoStreamProfile>();
      if (!selected_profile) {
        ROS_WARN_STREAM("Given stream configuration is not supported by the device! "
                        << " Stream: " << stream_name_[stream_index]
                        << ", Width: " << width_[stream_index]
                        << ", Height: " << height_[stream_index] << ", FPS: " << fps_[stream_index]
                        << ", Format: " << (int)format_[stream_index]);
        if (default_profile) {
          ROS_WARN_STREAM("Using default profile instead.");
          ROS_WARN_STREAM("default FPS " << default_profile->GetFps());
          selected_profile = default_profile;
        } else {
          ROS_WARN_STREAM(" NO default_profile found , Stream: " << stream_index.first
                                                                 << " will be disable");
          enable_stream_[stream_index] = false;
          continue;
        }
      }
      CHECK_NOTNULL(selected_profile.get());
      stream_profile_[stream_index] = selected_profile;
      int width = static_cast<int>(selected_profile->GetWidth());
      int height = static_cast<int>(selected_profile->GetHeight());
      int fps = static_cast<int>(selected_profile->GetFps());
      UpdateImageConfig(stream_index, selected_profile);
      width_[stream_index] = width;
      height_[stream_index] = height;
      fps_[stream_index] = fps;
      if (selected_profile->GetFormat() == kDtFormatBgra) {
        images_[stream_index] = cv::Mat(height, width, CV_8UC4, cv::Scalar(0, 0, 0, 0));
        encoding_[COLOR] = sensor_msgs::image_encodings::BGRA8;
        unit_step_size_[stream_index] = 4;
      } else if (selected_profile->GetFormat() == kDtFormatRgba) {
        images_[stream_index] = cv::Mat(height, width, CV_8UC4, cv::Scalar(0, 0, 0, 0));
        encoding_[COLOR] = sensor_msgs::image_encodings::RGBA8;
        unit_step_size_[stream_index] = 4;
      } else {
        images_[stream_index] =
            cv::Mat(height, width, image_format_[stream_index], cv::Scalar(0, 0, 0));
      }
      ROS_INFO_STREAM(" stream " << stream_name_[stream_index] << " is enabled - width: " << width
                                 << ", height: " << height << ", fps: " << fps << ", "
                                 << "Format: " << (int)selected_profile->GetFormat());
    } catch (const dt::Error& e) {
      ROS_ERROR_STREAM("Failed to setup  "
                       << stream_name_[stream_index] << " profile: " << width_[stream_index] << "x"
                       << height_[stream_index] << " " << fps_[stream_index] << "fps "
                       << DtFormatToString(format_[stream_index]) << " ERROR:" << e.GetMessage());
      PrintProfiles(sensors_[stream_index]->GetSensor());
      ROS_ERROR(
          "Error: The device might be connected via USB 2.0. Please verify your launch file "
          "configuration and "
          "try again. The current process will now exit.");
      exit(1);
    }
  }
  // IMU
  for (const auto& stream_index : HID_STREAMS) {
    if (!enable_stream_[stream_index]) {
      continue;
    }
    try {
      auto profile_list = sensors_[stream_index]->GetStreamProfileList();
      supported_profiles_[stream_index] = profile_list;
      if (stream_index == ACCEL) {
        auto full_scale_range = FullAccelScaleRangeFromString(imu_range_[stream_index]);
        auto sample_rate = SampleRateFromString(imu_rate_[stream_index]);
        auto profile = profile_list->GetAccelStreamProfile(full_scale_range, sample_rate);
        stream_profile_[stream_index] = profile;
      } else if (stream_index == GYRO) {
        auto full_scale_range = FullGyroScaleRangeFromString(imu_range_[stream_index]);
        auto sample_rate = SampleRateFromString(imu_rate_[stream_index]);
        auto profile = profile_list->GetGyroStreamProfile(full_scale_range, sample_rate);
        stream_profile_[stream_index] = profile;
      }
      ROS_INFO_STREAM("stream " << stream_name_[stream_index] << " full scale range "
                                << imu_range_[stream_index] << " sample rate "
                                << imu_rate_[stream_index]);
    } catch (const dt::Error& e) {
      ROS_ERROR_STREAM("Failed to setup << " << stream_name_[stream_index]
                                             << " profile: " << e.GetMessage());
      enable_stream_[stream_index] = false;
      stream_profile_[stream_index] = nullptr;
    }
  }

  if (depth_registration_ || enable_colored_point_cloud_) {
    align_filter_ = std::make_shared<dt::Align>(align_target_stream_);
  }
}
void DtCameraNode::UpdateImageConfig(
    const stream_index_pair& stream_index,
    const std::shared_ptr<dt::VideoStreamProfile>& selected_profile) {
  if (selected_profile->GetFormat() == kDtFormatY8) {
    image_format_[stream_index] = CV_8UC1;
    encoding_[stream_index] = stream_index.first == kDtStreamDepth
                                  ? sensor_msgs::image_encodings::TYPE_8UC1
                                  : sensor_msgs::image_encodings::MONO8;
    unit_step_size_[stream_index] = sizeof(uint8_t);
  }
  if (selected_profile->GetFormat() == kDtFormatMjpg) {
    if (stream_index.first == kDtStreamIr || stream_index.first == kDtStreamIrLeft ||
        stream_index.first == kDtStreamIrRight) {
      image_format_[stream_index] = CV_8UC1;
      encoding_[stream_index] = sensor_msgs::image_encodings::MONO8;
      unit_step_size_[stream_index] = sizeof(uint8_t);
    }
  }
  if (selected_profile->GetFormat() == kDtFormatY16 && stream_index == COLOR) {
    image_format_[stream_index] = CV_16UC1;
    encoding_[stream_index] = sensor_msgs::image_encodings::MONO16;
    unit_step_size_[stream_index] = sizeof(uint16_t);
  }
}

void DtCameraNode::SetupTopics() {
  SetupPublishers();
  ROS_INFO_STREAM("test---- SetupTopics publish_tf_: " << publish_tf_);
  if (publish_tf_) {
    PublishStaticTransforms();
  }
}

void DtCameraNode::SetupPublishers() {
  image_transport::ImageTransport image_transport(nh_);
  for (const auto& stream_index : IMAGE_STREAMS) {
    if (!enable_stream_[stream_index]) {
      continue;
    }
    ROS_INFO_STREAM("test---- SetupPublishers " << stream_name_[stream_index]);
    std::string name = stream_name_[stream_index];
    std::string topic_name = "/" + camera_name_ + "/" + name + "/image_raw";
    image_transport::SubscriberStatusCallback it_subscribed_cb =
        boost::bind(&DtCameraNode::ImageSubscribedCallback, this, stream_index);
    image_transport::SubscriberStatusCallback it_unsubscribed_cb =
        boost::bind(&DtCameraNode::ImageUnsubscribedCallback, this, stream_index);
    image_publishers_[stream_index] =
        image_transport.advertise(topic_name, 1, it_subscribed_cb, it_unsubscribed_cb);
    topic_name = "/" + camera_name_ + "/" + name + "/camera_info";
    ros::SubscriberStatusCallback image_subscribed_cb =
        boost::bind(&DtCameraNode::ImageSubscribedCallback, this, stream_index);
    ros::SubscriberStatusCallback image_unsubscribed_cb =
        boost::bind(&DtCameraNode::ImageUnsubscribedCallback, this, stream_index);
    camera_info_publishers_[stream_index] = nh_.advertise<sensor_msgs::CameraInfo>(
        topic_name, 1, image_subscribed_cb, image_unsubscribed_cb);
    CHECK_NOTNULL(device_info_.get());
    // if (isXxxPID(device_info_->GetPid())) {
    //   metadata_publishers_[stream_index] =
    //       nh_.advertise<deptrum_camera::Metadata>("/" + camera_name_ + "/" + name + "/metadata",
    //       1,
    //                                              image_subscribed_cb, image_unsubscribed_cb);
    // }
  }
  ROS_INFO_STREAM("test---- SetupPublishers  enable_point_cloud_:"
                  << enable_point_cloud_ << " enable_stream[DEPTH]:" << enable_stream_[DEPTH]
                  << " so can to PointCloudSubscribedCallback");
  if (enable_point_cloud_ && enable_stream_[DEPTH]) {
    ros::SubscriberStatusCallback depth_cloud_subscribed_cb =
        boost::bind(&DtCameraNode::PointCloudSubscribedCallback, this);
    ros::SubscriberStatusCallback depth_cloud_unsubscribed_cb =
        boost::bind(&DtCameraNode::PointCloudUnsubscribedCallback, this);
    depth_cloud_pub_ = nh_.advertise<sensor_msgs::PointCloud2>(
        "depth/points", 1, depth_cloud_subscribed_cb, depth_cloud_unsubscribed_cb);

    depth_cloud_pub_second_ = nh_.advertise<sensor_msgs::PointCloud2>(
        "depth/points_second_sd", 1, depth_cloud_subscribed_cb, depth_cloud_unsubscribed_cb);
  }
  if (enable_colored_point_cloud_ && enable_stream_[DEPTH] && enable_stream_[COLOR]) {
    ros::SubscriberStatusCallback depth_registered_cloud_subscribed_cb =
        boost::bind(&DtCameraNode::ColoredPointCloudSubscribedCallback, this);
    ros::SubscriberStatusCallback depth_registered_cloud_unsubscribed_cb =
        boost::bind(&DtCameraNode::ColoredPointCloudUnsubscribedCallback, this);
    depth_registered_cloud_pub_ = nh_.advertise<sensor_msgs::PointCloud2>(
        "depth_registered/points", 1, depth_registered_cloud_subscribed_cb,
        depth_registered_cloud_unsubscribed_cb);
  }

  if (enable_sync_output_accel_gyro_) {
    std::string topic_name = stream_name_[GYRO] + "_" + stream_name_[ACCEL] + "/sample";
    ros::SubscriberStatusCallback imu_subscribed_cb =
        boost::bind(&DtCameraNode::ImuSubscribedCallback, this, GYRO);
    ros::SubscriberStatusCallback imu_unsubscribed_cb =
        boost::bind(&DtCameraNode::ImuUnsubscribedCallback, this, GYRO);
    imu_gyro_accel_publisher_ =
        nh_.advertise<sensor_msgs::Imu>(topic_name, 1, imu_subscribed_cb, imu_unsubscribed_cb);
    topic_name = stream_name_[GYRO] + "/imu_info";
    imu_info_publishers_[GYRO] = nh_.advertise<deptrum_camera::ImuInfo>(
        topic_name, 1, imu_subscribed_cb, imu_unsubscribed_cb);
    topic_name = stream_name_[ACCEL] + "/imu_info";
    imu_info_publishers_[ACCEL] = nh_.advertise<deptrum_camera::ImuInfo>(
        topic_name, 1, imu_subscribed_cb, imu_unsubscribed_cb);
  } else {
    for (const auto& stream_index : HID_STREAMS) {
      if (!enable_stream_[stream_index]) {
        continue;
      }
      std::string topic_name = stream_name_[stream_index] + "/sample";
      ros::SubscriberStatusCallback imu_subscribed_cb =
          boost::bind(&DtCameraNode::ImuSubscribedCallback, this, stream_index);
      ros::SubscriberStatusCallback imu_unsubscribed_cb =
          boost::bind(&DtCameraNode::ImuUnsubscribedCallback, this, stream_index);
      imu_publishers_[stream_index] =
          nh_.advertise<sensor_msgs::Imu>(topic_name, 1, imu_subscribed_cb, imu_unsubscribed_cb);
      topic_name = stream_name_[stream_index] + "/imu_info";
      imu_info_publishers_[stream_index] = nh_.advertise<deptrum_camera::ImuInfo>(
          topic_name, 1, imu_subscribed_cb, imu_unsubscribed_cb);
    }
  }
  if (enable_stream_[DEPTH] && enable_stream_[INFRA0]) {
    depth_to_other_extrinsics_publishers_[INFRA0] =
        nh_.advertise<deptrum_camera::Extrinsics>("/" + camera_name_ + "/depth_to_ir", 1, true);
  }
  if (enable_stream_[DEPTH] && enable_stream_[COLOR]) {
    depth_to_other_extrinsics_publishers_[COLOR] =
        nh_.advertise<deptrum_camera::Extrinsics>("/" + camera_name_ + "/depth_to_color", 1, true);
  }
  if (enable_stream_[DEPTH] && enable_stream_[INFRA1]) {
    depth_to_other_extrinsics_publishers_[INFRA1] = nh_.advertise<deptrum_camera::Extrinsics>(
        "/" + camera_name_ + "/depth_to_left_ir", 1, true);
  }
  if (enable_stream_[DEPTH] && enable_stream_[INFRA2]) {
    depth_to_other_extrinsics_publishers_[INFRA2] = nh_.advertise<deptrum_camera::Extrinsics>(
        "/" + camera_name_ + "/depth_to_right_ir", 1, true);
  }
  if (enable_stream_[DEPTH] && enable_stream_[ACCEL]) {
    depth_to_other_extrinsics_publishers_[ACCEL] =
        nh_.advertise<deptrum_camera::Extrinsics>("/" + camera_name_ + "/depth_to_accel", 1, true);
  }
  if (enable_stream_[DEPTH] && enable_stream_[GYRO]) {
    depth_to_other_extrinsics_publishers_[GYRO] =
        nh_.advertise<deptrum_camera::Extrinsics>("/" + camera_name_ + "/depth_to_gyro", 1, true);
  }
  filter_status_pub_ =
      nh_.advertise<std_msgs::String>("/" + camera_name_ + "/filter_status", 1, true);
  std_msgs::String msg;
  msg.data = filter_status_.dump(2);
  filter_status_pub_.publish(msg);
  sdk_version_pub_ = nh_.advertise<std_msgs::String>("/" + camera_name_ + "/sdk_version", 1, true);
  auto device_info = device_->GetDeviceInfo();
  nlohmann::json data;
  std_msgs::String sdk_msg;
  data["firmware_version"] = device_info->GetFirmwareVersion();
  data["supported_min_sdk_version"] = device_info->GetSupportedMinSdkVersion();
  data["ros_sdk_version"] = DT_ROS_VERSION_STR;
  std::string major = std::to_string(dt::Version::GetMajor());
  std::string minor = std::to_string(dt::Version::GetMinor());
  std::string patch = std::to_string(dt::Version::GetPatch());
  std::string version = major + "." + minor + "." + patch;
  data["dt_sdk_version"] = version;
  sdk_msg.data = data.dump(2);
  sdk_version_pub_.publish(sdk_msg);
}

void DtCameraNode::SetupCameraInfo() {
  color_camera_info_manager_ = std::make_shared<camera_info_manager::CameraInfoManager>(
      nh_rgb_, camera_name_ + "_" + stream_name_[COLOR], color_info_uri_);
  ir_camera_info_manager_ = std::make_shared<camera_info_manager::CameraInfoManager>(
      nh_ir_, camera_name_ + "_" + stream_name_[INFRA0], ir_info_uri_);
  auto param = GetCameraParam();
  if (param) {
    camera_infos_[DEPTH] = ConvertToCameraInfo(param->depth_intrinsic, param->depth_distortion,
                                               param->depth_intrinsic.width);
    camera_infos_[INFRA0] = ConvertToCameraInfo(param->depth_intrinsic, param->depth_distortion,
                                                param->depth_intrinsic.width);
    camera_infos_[COLOR] = ConvertToCameraInfo(param->rgb_intrinsic, param->rgb_distortion,
                                               param->rgb_intrinsic.width);
  } else {
    // ROS_WARN_STREAM("Failed to get camera parameters");
  }
}

void DtCameraNode::CheckAndSetDepthMode() {
  try {
    bool is_supported = device_->IsOptionSupported(kDtOptionDepthModeInt, kDtPermissionReadWrite);
    if (is_supported) {
      int depth_mode = device_->GetIntOption(kDtOptionDepthModeInt);
      ROS_INFO_STREAM("CheckAndSetDepthMode get when init depth mode is" << depth_mode);

      device_->SetIntOption(kDtOptionDepthModeInt, camera_mode_);
      ROS_INFO_STREAM("CheckAndSetDepthModeDevice SetDepthMode" << camera_mode_);
    }
  } catch (dt::Error& e) {
    ROS_INFO_STREAM("CheckAndSetDeviceMode error msg:" << e.GetMessage());
  }
}
void DtCameraNode::CheckAndSetDeviceMode() {
  bool is_supported =
      device_->IsOptionSupported(kDtOptionSlaveDeviceSyncStatusBool, kDtPermissionReadWrite);
  if (is_supported) {
    bool is_slave = device_->GetBoolOption(kDtOptionSlaveDeviceSyncStatusBool);
    ROS_INFO_STREAM("CheckAndSetDeviceMode  get when init device mode is"
                    << (is_slave ? "Slave" : "Master") << " mode.");
    if (device_mode_ == 0) {
      // 设置为主模式
      device_->SetBoolOption(kDtOptionSlaveDeviceSyncStatusBool, false);
      ROS_INFO_STREAM("CheckAndSetDeviceMode set to Master mode.");
    } else if (device_mode_ == 1) {
      // 设置为从模式
      device_->SetBoolOption(kDtOptionSlaveDeviceSyncStatusBool, true);
      ROS_INFO_STREAM("CheckAndSetDeviceMode Device set to Slave  mode.");
    }
  }
}

void DtCameraNode::SetupPipelineConfig() {
  if (pipeline_config_) {
    pipeline_config_.reset();
  }
  pipeline_config_ = std::make_shared<dt::Config>();
  // auto pid = device_info_->GetPid();

  // if (!isXxxPID(pid) && depth_registration_ && enable_stream_[COLOR] &&
  //     enable_stream_[DEPTH]) {
  //   DtAlignMode align_mode = align_mode_ == "HW" ? ALIGN_D2C_HW_MODE : ALIGN_D2C_SW_MODE;
  //   ROS_INFO_STREAM("set align mode to " << align_mode_);
  //   pipeline_config_->setAlignMode(align_mode);
  //   pipeline_config_->setDepthScaleRequire(enable_depth_scale_);
  // }

  // if (depth_registration_ && enable_stream_[COLOR] && enable_stream_[DEPTH]) {
  //   DtAlignMode align_mode = align_mode_ == "HW" ? ALIGN_D2C_HW_MODE : ALIGN_D2C_SW_MODE;
  //   ROS_INFO_STREAM("SetupPipelineConfig SetupPipelineConfig set align mode to " << align_mode_);
  //   pipeline_config_->setAlignMode(align_mode);
  //   pipeline_config_->setDepthScaleRequire(enable_depth_scale_);
  // }

  for (const auto& stream_index : IMAGE_STREAMS) {
    if (enable_stream_[stream_index]) {
      ROS_INFO_STREAM("SetupPipelineConfig Enable " << stream_name_[stream_index] << " stream");
      auto profile = stream_profile_[stream_index]->As<dt::VideoStreamProfile>();

      if (stream_index == COLOR && enable_stream_[COLOR] && align_filter_) {
        auto video_profile = profile;
        ROS_INFO_STREAM("SetupPipelineConfig color video_profile: "
                        << video_profile->GetWidth() << "x" << video_profile->GetHeight() << " "
                        << video_profile->GetFps() << "fps " << (int)video_profile->GetFormat());
        align_filter_->SetAlignToStreamProfile(video_profile);
      }

      ROS_INFO_STREAM("SetupPipelineConfig Stream "
                      << stream_name_[stream_index] << " width: " << profile->GetWidth()
                      << " height: " << profile->GetHeight() << " fps: " << profile->GetFps()
                      << " format: " << (int)profile->GetFormat());

      pipeline_config_->EnableStream(stream_profile_[stream_index]);
    }
  }
}

void DtCameraNode::ReadDefaultGain() {
  for (const auto& stream_index : IMAGE_STREAMS) {
    if (!enable_stream_[stream_index]) {
      continue;
    }
    try {
      auto sensor = sensors_[stream_index];
      CHECK_NOTNULL(sensor.get());
      auto gain = sensor->GetGain();
      ROS_INFO_STREAM("stream " << stream_name_[stream_index] << " gain " << gain);
      default_gain_[stream_index] = gain;
    } catch (dt::Error& e) {
      default_gain_[stream_index] = 0;
      ROS_DEBUG_STREAM("get gain error " << e.GetMessage());
    }
  }
}

void DtCameraNode::ReadDefaultExposure() {
  for (const auto& stream_index : IMAGE_STREAMS) {
    if (!enable_stream_[stream_index]) {
      continue;
    }
    try {
      auto sensor = sensors_[stream_index];
      CHECK_NOTNULL(sensor.get());
      auto exposure = sensor->GetExposure();
      bool is_auto_exposure = sensor->GetAutoExposure();
      ROS_INFO_STREAM("stream " << stream_name_[stream_index] << " exposure " << exposure
                                << " auto exposure " << is_auto_exposure);
      ROS_INFO_STREAM("stream " << stream_name_[stream_index] << " exposure " << exposure);
      default_exposure_[stream_index] = exposure;
    } catch (dt::Error& e) {
      default_exposure_[stream_index] = 0;
      ROS_DEBUG_STREAM("get " << stream_name_[stream_index] << " exposure error "
                              << e.GetMessage());
    }
  }
}

void DtCameraNode::ReadDefaultWhiteBalance() {
  try {
    auto sensor = sensors_[COLOR];
    if (!sensor) {
      ROS_INFO_STREAM("does not have color sensor");
      return;
    }
    CHECK_NOTNULL(sensor.get());
    auto wb = sensor->GetWhiteBalance();
    ROS_INFO_STREAM("stream " << stream_name_[COLOR] << " wb " << wb);
    default_white_balance_ = wb;
  } catch (dt::Error& e) {
    default_white_balance_ = 0;
    ROS_DEBUG_STREAM("get white balance error " << e.GetMessage());
  }
}

void DtCameraNode::SetDepthCropExposureRoi() {
  try {
    // todo::确认401
    auto device_info = device_->GetDeviceInfo();
    CHECK_NOTNULL(device_info);
    auto pid = device_info->GetPid();
    if (!IsNebulaSeriesPid(pid)) {
      throw std::runtime_error("SetDepthCropExposureRoi current pid not supprort");
    }
    ROS_INFO_STREAM("SetDepthCropExposureRoi launch depth_crop_roi_mtof_enable is "
                    << depth_crop_roi_mtof_enable_);
    if (depth_crop_roi_mtof_enable_) {
      // ROS_INFO_STREAM("test---- aaaaadepth_crop_roi_mtof_enable_:"
      //                 << depth_crop_roi_mtof_enable_ << " ,depth_crop_roi_mtof_left_: " << depth_crop_roi_mtof_left_
      //                 << " ,depth_crop_roi_mtof_width_: " << depth_crop_roi_mtof_width_ << " ,depth_crop_roi_mtof_height_ :" << depth_crop_roi_mtof_height_);
      ROS_INFO_STREAM("test---- width_[DEPTH]:"
                      <<width_[DEPTH]
                      << " ,height_[DEPTH]: " << height_[DEPTH]);                
      auto config = CropRoi();
      uint32_t data_size = sizeof(config);
      config.x0_left = depth_crop_roi_mtof_left_;
      config.y0_top = depth_crop_roi_mtof_top_;
      config.width = depth_crop_roi_mtof_width_;
      config.height = depth_crop_roi_mtof_height_;
      
      ROS_INFO_STREAM("SetDepthCropExposureRoi mtof roi x0:"
                      << (int)config.x0_left << " ,top: " << (int)config.y0_top
                      << " ,width: " << (int)config.width << " ,height :" << (int)config.height);
      device_->SetStructuredData(kDtOptionMtofDepthCropRoiStruct,
                                 reinterpret_cast<const uint8_t*>(&config), sizeof(config));
    }

    ROS_INFO_STREAM("SetDepthCropExposureRoi launch depth_crop_roi_stof_enable is "
                    << depth_crop_roi_stof_enable_);
    if (depth_crop_roi_stof_enable_) {
      auto config = CropRoi();
      uint32_t data_size = sizeof(config);

      config.x0_left = depth_crop_roi_stof_left_;
      config.y0_top = depth_crop_roi_stof_top_;
      config.width = depth_crop_roi_stof_width_;
      config.height = depth_crop_roi_stof_height_;
    
      ROS_INFO_STREAM("SetDepthCropExposureRoi stof roi x0:"
                      << (int)config.x0_left << " ,top: " << (int)config.y0_top
                      << " ,width: " << (int)config.width << " ,height :" << (int)config.height);
      device_->SetStructuredData(kDtOptionStofDepthCropRoiStruct,
                                 reinterpret_cast<const uint8_t*>(&config), sizeof(config));
    }

  } catch (const dt::Error& e) {
    ROS_ERROR_STREAM("SetDepthCropExposureRoi Failed to setStructuredData: " << e.GetMessage());
  } catch (const std::exception& e) {
    ROS_ERROR_STREAM("SetDepthCropExposureRoi Failed to setStructuredData: " << e.what());
  } catch (...) {
    ROS_ERROR_STREAM("unknown error");
  }
}

void DtCameraNode::SetDepthAutoExposureRoi() {
  static bool depth_roi_has_run = false;
  if (depth_roi_has_run) {
    return;
  }
  if (device_->IsOptionSupported(kDtOptionDepthAeRoiStruct, kDtPermissionReadWrite)) {
    auto config = DtRegionOfInterest();
    uint32_t data_size = sizeof(config);
    device_->GetStructuredData(kDtOptionDepthAeRoiStruct, reinterpret_cast<uint8_t*>(&config),
                               &data_size);
    if (depth_ae_roi_left_ != -1) {
      config.x = (depth_ae_roi_left_ < 0) ? 0 : depth_ae_roi_left_;
      config.x = (depth_ae_roi_left_ > width_[DEPTH] - 1) ? width_[DEPTH] - 1 : config.x;
    }
    if (depth_ae_roi_top_ != -1) {
      config.y = (depth_ae_roi_top_ < 0) ? 0 : depth_ae_roi_top_;
      config.y = (depth_ae_roi_top_ > height_[DEPTH] - 1) ? height_[DEPTH] - 1 : config.y;
    }
    if (depth_ae_roi_right_ != -1) {
      config.width = (depth_ae_roi_right_ < 0) ? 0 : depth_ae_roi_right_;
      config.width = (depth_ae_roi_right_ > width_[DEPTH] - 1) ? width_[DEPTH] - 1 : config.width;
    }
    if (depth_ae_roi_bottom_ != -1) {
      config.height = (depth_ae_roi_bottom_ < 0) ? 0 : depth_ae_roi_bottom_;
      config.height =
          (depth_ae_roi_bottom_ > height_[DEPTH] - 1) ? height_[DEPTH] - 1 : config.height;
    }
    device_->SetStructuredData(kDtOptionDepthAeRoiStruct, reinterpret_cast<const uint8_t*>(&config),
                               sizeof(config));
    device_->GetStructuredData(kDtOptionDepthAeRoiStruct, reinterpret_cast<uint8_t*>(&config),
                               &data_size);
    ROS_INFO_STREAM("Setting depth AE ROI to " << config.x << ", " << config.y << ", "
                                               << config.width << ", " << config.height);
  }
  depth_roi_has_run = true;
}

void DtCameraNode::SetColorAutoExposureRoi() {
  static bool color_roi_has_run = false;
  if (color_roi_has_run) {
    return;
  }
  if (device_->IsOptionSupported(kDtOptionColorAeRoiStruct, kDtPermissionReadWrite)) {
    auto config = DtRegionOfInterest();
    uint32_t data_size = sizeof(config);
    device_->GetStructuredData(kDtOptionColorAeRoiStruct, reinterpret_cast<uint8_t*>(&config),
                               &data_size);
    if (color_ae_roi_left_ != -1) {
      config.x = (color_ae_roi_left_ < 0) ? 0 : color_ae_roi_left_;
      config.x = (color_ae_roi_left_ > width_[COLOR] - 1) ? width_[COLOR] - 1 : config.x;
    }
    if (color_ae_roi_top_ != -1) {
      config.y = (color_ae_roi_top_ < 0) ? 0 : color_ae_roi_top_;
      config.y = (color_ae_roi_top_ > height_[COLOR] - 1) ? height_[COLOR] - 1 : config.y;
    }
    if (color_ae_roi_right_ != -1) {
      config.width = (color_ae_roi_right_ < 0) ? 0 : color_ae_roi_right_;
      config.width = (color_ae_roi_right_ > width_[COLOR] - 1) ? width_[COLOR] - 1 : config.width;
    }
    if (color_ae_roi_bottom_ != -1) {
      config.height = (color_ae_roi_bottom_ < 0) ? 0 : color_ae_roi_bottom_;
      config.height =
          (color_ae_roi_bottom_ > height_[COLOR] - 1) ? height_[COLOR] - 1 : config.height;
    }
    device_->SetStructuredData(kDtOptionColorAeRoiStruct, reinterpret_cast<const uint8_t*>(&config),
                               sizeof(config));
    device_->GetStructuredData(kDtOptionColorAeRoiStruct, reinterpret_cast<uint8_t*>(&config),
                               &data_size);
    ROS_INFO_STREAM("Setting color AE ROI to " << config.x << ", " << config.y << ", "
                                               << config.width << ", " << config.height);
  }
  color_roi_has_run = true;
}

bool DtCameraNode::SetFilterCallback(SetFilterRequest& request, SetFilterResponse& response) {
  try {
    ROS_INFO_STREAM("filter_name: " << request.filter_name << "  filter_enable: "
                                    << (request.filter_enable ? "true" : "false"));
    auto it = std::remove_if(depth_filter_list_.begin(), depth_filter_list_.end(),
                             [&request](const std::shared_ptr<dt::Filter>& filter) {
                               return filter->GetName() == request.filter_name;
                             });
    depth_filter_list_.erase(it, depth_filter_list_.end());
    if (request.filter_name == "DecimationFilter") {
      auto decimation_filter = std::make_shared<dt::DecimationFilter>();
      decimation_filter->Enable(request.filter_enable);
      depth_filter_list_.push_back(decimation_filter);
      if (request.filter_param.size() > 0) {
        auto range = decimation_filter->GetScaleRange();
        auto decimation_filter_scale = request.filter_param[0];
        if (decimation_filter_scale <= range.max && decimation_filter_scale >= range.min) {
          ROS_INFO_STREAM("Set decimation filter scale value to " << decimation_filter_scale);
          decimation_filter->SetScaleValue(decimation_filter_scale);
        }
        if (decimation_filter_scale != -1 &&
            (decimation_filter_scale < range.min || decimation_filter_scale > range.max)) {
          ROS_ERROR_STREAM("Decimation filter scale value is out of range " << range.min << " - "
                                                                            << range.max);
        }
      } else {
        response.message =
            "The filter switch setting is successful, but the filter parameter setting fails";
        return true;
      }
    }
    else if (request.filter_name == "ThresholdFilter") {
      auto threshold_filter = std::make_shared<dt::ThresholdFilter>();
      threshold_filter->Enable(request.filter_enable);
      depth_filter_list_.push_back(threshold_filter);
      if (request.filter_param.size() > 1) {
        auto threshold_filter_min = request.filter_param[0];
        auto threshold_filter_max = request.filter_param[1];
        threshold_filter->SetValueRange(threshold_filter_min, threshold_filter_max);
        ROS_INFO_STREAM("Set threshold filter value range to " << threshold_filter_min << " - "
                                                               << threshold_filter_max);
      } else {
        response.message =
            "The filter switch setting is successful, but the filter parameter setting fails";
        return true;
      }
    } 
    else {
      ROS_INFO_STREAM(request.filter_name
                      << "Cannot be set\n"
                      << "The filter_name value that can be set is "
                         "DecimationFilter, HDRMerge, SequenceIdFilter, ThresholdFilter, Nois"
                         "eRemovalFilter, SpatialAdvancedFilter and TemporalFilter");
    }
    for (auto& filter : depth_filter_list_) {
      std::cout << " - " << filter->GetName() << ": "
                << (filter->IsEnabled() ? "enabled" : "disabled") << std::endl;
      auto configSchemaVec = filter->GetConfigSchemaVec();
      for (auto& configSchema : configSchemaVec) {
        std::cout << "    - {" << configSchema.name << ", " << configSchema.type << ", "
                  << configSchema.min << ", " << configSchema.max << ", " << configSchema.step
                  << ", " << configSchema.def << ", " << configSchema.desc << "}" << std::endl;
      }
    }
    return response.success = true;
  } catch (const dt::Error& e) {
    ROS_ERROR_STREAM("Failed to set filter: " << e.GetMessage());
    return response.success = false;
  } catch (const std::exception& e) {
    ROS_ERROR_STREAM("Failed to set filter: " << e.what());
    return response.success = false;
  } catch (...) {
    ROS_ERROR_STREAM("unknown error");
    return response.success = false;
  }
}
}  // namespace deptrum_camera
