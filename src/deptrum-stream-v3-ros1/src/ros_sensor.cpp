// Copyright (c) Deptrum Inc. All Rights Reserved.

#include "deptrum_camera/ros_sensor.h"
#include "ros/ros.h"

namespace deptrum_camera {

RosDtSensor::RosDtSensor(std::shared_ptr<dt::Device> device, std::shared_ptr<dt::Sensor> sensor,
                         std::string name)
    : device_(std::move(device)), sensor_(std::move(sensor)), name_(std::move(name)) {
  CHECK_NOTNULL(sensor_.get());
}

RosDtSensor::~RosDtSensor() { StopStream(); }

void RosDtSensor::StartStream(std::shared_ptr<dt::StreamProfile> profile,
                              dt::Sensor::FrameCallback callback) {
  if (is_started_) {
    return;
  }
  CHECK_NOTNULL(profile.get());
  sensor_->Start(profile, std::move(callback));
  profile_ = profile;
  is_started_ = true;
}

void RosDtSensor::StopStream() {
  if (!is_started_) {
    return;
  }
  sensor_->Stop();
  is_started_ = false;
}

// todo:: 410 sdk后续是否需要读取不同类型的曝光值，同理还有范围
int RosDtSensor::GetExposure() {
  int data = 0;
  switch (sensor_->GetType()) {
    case kDtSensorDepth:
      data = device_->GetIntOption(kDtOptionDepthExposureInt);
      break;
    case kDtSensorColor:
      data = device_->GetIntOption(kDtOptionColorExposureInt);
      break;
    case kDtSensorIrLeft:
    case kDtSensorIrRight:
    case kDtSensorIr:
      data = device_->GetIntOption(kDtOptionIrExposureInt);
      break;
    default:
      ROS_INFO_STREAM(name_ << " does not support get exposure");
      break;
  }
  return data;
}

DtIntOptionRange RosDtSensor::GetExposureRange() {
  DtIntOptionRange range{0, 0};
  switch (sensor_->GetType()) {
    case kDtSensorDepth:
      range = device_->GetIntOptionRange(kDtOptionDepthExposureInt);
      break;
    case kDtSensorColor:
      range = device_->GetIntOptionRange(kDtOptionColorExposureInt);
      break;
    case kDtSensorIrLeft:
    case kDtSensorIrRight:
    case kDtSensorIr:
      range = device_->GetIntOptionRange(kDtOptionIrExposureInt);
      break;
    default:
      ROS_INFO_STREAM(name_ << " does not support get exposure");
      break;
  }
  return range;
}

void RosDtSensor::SetExposure(int data, int data_two) {
  switch (sensor_->GetType()) {
    case kDtSensorDepth: {
      if (device_->GetBoolOption(kDtOptionDepthAutoExposureBool)) {
        throw std::runtime_error("SetExposure is auto exposure true, can not set");
        break;
      }
      auto device_info = device_->GetDeviceInfo();
      CHECK_NOTNULL(device_info);
      auto pid = device_info->GetPid();
      if (IsNebulaSeriesPid(pid)) {
        if (device_->IsOptionSupported(kDtOptionDepthModeInt, kDtPermissionRead)) {
          int depth_mode = device_->GetIntOption(kDtOptionDepthModeInt);
          ROS_INFO_STREAM("SetExposure IsOptionSupported depth mode is" << depth_mode);
          if (depth_mode == 0) {
            if (data_two != kDtOptionMtofDepthExposureInt) {
              throw std::runtime_error(
                  "SetExposure cur camera_mode is 0, and type is not "
                  "kDtOptionMtofDepthExposureInt "
                  "4");
            }
          } else {
            if (data_two != kDtOptionMtofDepthExposureInt &&
                data_two != kDtOptionStofDepthExposureInt) {
              throw std::runtime_error(
                  "SetExposure cur camera_mode not 0, and the type is not 3 or 4");
            }
          }
          device_->SetIntOption((dt_option_id)data_two, data);
          ROS_INFO_STREAM("SetExposure set depth_exposure_mode: "<< data_two << ", depth_exposure: " << data);
        } else {
          device_->SetIntOption(kDtOptionDepthExposureInt, data);
          ROS_INFO_STREAM("SetExposure set kDtOptionDepthExposureInt, camera_mode not read");
        }
      } else {
        device_->SetIntOption(kDtOptionDepthExposureInt, data);
        ROS_INFO_STREAM("SetExposure not 410 set kDtOptionDepthExposureInt");
      }

    } break;
    case kDtSensorColor:
      device_->SetIntOption(kDtOptionColorExposureInt, data);
      break;
    case kDtSensorIrLeft:
    case kDtSensorIrRight:
    case kDtSensorIr:
      device_->SetIntOption(kDtOptionIrExposureInt, data);
      break;
    default:
      ROS_INFO_STREAM(name_ << " does not support set exposure");
      break;
  }
}

int RosDtSensor::GetGain() {
  int data = 0;
  switch (sensor_->GetType()) {
    case kDtSensorDepth:
      data = device_->GetIntOption(kDtOptionDepthGainInt);
      break;
    case kDtSensorIrLeft:
    case kDtSensorIrRight:
    case kDtSensorIr:
      data = device_->GetIntOption(kDtOptionIrGainInt);
      break;
    case kDtSensorColor:
      data = device_->GetIntOption(kDtOptionColorGainInt);
      break;
    default:
      ROS_INFO_STREAM(name_ << " does not support get gain");
      break;
  }
  return data;
}

DtIntOptionRange RosDtSensor::GetGainRange() {
  DtIntOptionRange range{0, 0};
  switch (sensor_->GetType()) {
    case kDtSensorDepth:
      range = device_->GetIntOptionRange(kDtOptionDepthGainInt);
      break;
    case kDtSensorIrLeft:
    case kDtSensorIrRight:
    case kDtSensorIr:
      range = device_->GetIntOptionRange(kDtOptionIrGainInt);
      break;
    case kDtSensorColor:
      range = device_->GetIntOptionRange(kDtOptionColorGainInt);
      break;
    default:
      ROS_INFO_STREAM(name_ << " does not support get gain");
      break;
  }
  return range;
}
void RosDtSensor::SetGain(int data) {
  switch (sensor_->GetType()) {
    case kDtSensorDepth:
      device_->SetIntOption(kDtOptionDepthGainInt, data);
      break;
    case kDtSensorIrLeft:
    case kDtSensorIrRight:
    case kDtSensorIr:
      device_->SetIntOption(kDtOptionIrGainInt, data);
      break;
    case kDtSensorColor:
      device_->SetIntOption(kDtOptionColorGainInt, data);
      break;
    default:
      ROS_INFO_STREAM(name_ << " does not support set gain");
      break;
  }
}

int RosDtSensor::GetWhiteBalance() {
  int data = 0;
  if (sensor_->GetType() == kDtSensorColor) {
    data = device_->GetIntOption(kDtOptionColorWhiteBalanceInt);
  } else {
    ROS_ERROR_STREAM(name_ << " does not support get white balance");
  }
  return data;
}

DtIntOptionRange RosDtSensor::GetWhiteBalanceRange() {
  DtIntOptionRange range{0, 0};
  if (sensor_->GetType() == kDtSensorColor) {
    range = device_->GetIntOptionRange(kDtOptionColorWhiteBalanceInt);
  } else {
    ROS_ERROR_STREAM(name_ << " does not support get white balance");
  }
  return range;
}

void RosDtSensor::SetWhiteBalance(int data) {
  if (sensor_->GetType() == kDtSensorColor) {
    auto range = device_->GetIntOptionRange(kDtOptionColorWhiteBalanceInt);
    if (data < range.min || data > range.max) {
      ROS_ERROR_STREAM(name_ << " white balance value out of range");
      return;
    }
    device_->SetIntOption(kDtOptionColorWhiteBalanceInt, data);
  } else {
    ROS_ERROR_STREAM(name_ << " does not support set white balance");
  }
}

bool RosDtSensor::GetAutoWhiteBalance() {
  bool data = false;
  if (sensor_->GetType() == kDtSensorColor) {
    data = device_->GetBoolOption(kDtOptionColorAutoWhiteBalanceBool);
  } else {
    ROS_ERROR_STREAM(name_ << " does not support get auto white balance");
  }
  return data;
}

void RosDtSensor::SetAutoWhiteBalance(bool data) {
  if (sensor_->GetType() == kDtSensorColor) {
    device_->SetBoolOption(kDtOptionColorAutoWhiteBalanceBool, data);
  } else {
    ROS_ERROR_STREAM(name_ << " does not support set auto white balance");
  }
}

void RosDtSensor::SetAutoExposure(bool data) {
  switch (sensor_->GetType()) {
    case kDtSensorDepth:
      device_->SetBoolOption(kDtOptionDepthAutoExposureBool, data);
      break;
    case kDtSensorIrLeft:
    case kDtSensorIrRight:
    case kDtSensorIr:
      device_->SetBoolOption(kDtOptionIrAutoExposureBool, data);
      break;
    case kDtSensorColor:
      device_->SetBoolOption(kDtOptionColorAutoExposureBool, data);
      break;
    default:
      ROS_ERROR_STREAM(name_ << " does not support set auto exposure");
      break;
  }
}

bool RosDtSensor::GetAutoExposure() {
  bool data = false;
  switch (sensor_->GetType()) {
    case kDtSensorDepth:
      data = device_->GetBoolOption(kDtOptionDepthAutoExposureBool);
      break;
    case kDtSensorIrLeft:
    case kDtSensorIrRight:
    case kDtSensorIr:
      data = device_->GetBoolOption(kDtOptionIrAutoExposureBool);
      break;
    case kDtSensorColor:
      data = device_->GetBoolOption(kDtOptionColorAutoExposureBool);
      break;
    default:
      ROS_ERROR_STREAM(name_ << " does not support set auto exposure");
      break;
  }
  return data;
}

DtSensorType RosDtSensor::GetSensorType() { return sensor_->GetType(); }

bool RosDtSensor::IsMirrored() const { return is_mirrored_; }

std::shared_ptr<dt::StreamProfileList> RosDtSensor::GetStreamProfileList() const {
  return sensor_->GetStreamProfileList();
}

std::shared_ptr<dt::Sensor> RosDtSensor::GetSensor() const { return sensor_; }

}  // namespace deptrum_camera
