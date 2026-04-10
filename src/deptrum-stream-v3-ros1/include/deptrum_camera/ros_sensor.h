// Copyright (c) Deptrum Inc. All Rights Reserved.

#pragma once
#include "constants.h"
#include "utils.h"
#include "types.h"

namespace deptrum_camera {
class RosDtSensor {
 public:
  explicit RosDtSensor(std::shared_ptr<dt::Device> device, std::shared_ptr<dt::Sensor> sensor,
                       std::string name);

  ~RosDtSensor();

  void StartStream(std::shared_ptr<dt::StreamProfile> profile, dt::Sensor::FrameCallback callback);

  void StopStream();

  int GetExposure();

  DtIntOptionRange GetExposureRange();

  void SetExposure(int data, int data_two);

  int GetGain();

  DtIntOptionRange GetGainRange();

  void SetGain(int data);

  int GetWhiteBalance();

  DtIntOptionRange GetWhiteBalanceRange();

  void SetWhiteBalance(int data);

  bool GetAutoWhiteBalance();

  void SetAutoWhiteBalance(bool data);

  void SetAutoExposure(bool data);

  bool GetAutoExposure();

  DtSensorType GetSensorType();

  bool IsMirrored() const;

  std::shared_ptr<dt::StreamProfileList> GetStreamProfileList() const;

  std::shared_ptr<dt::Sensor> GetSensor() const;

 private:
  std::shared_ptr<dt::Device> device_ = nullptr;
  std::shared_ptr<dt::Sensor> sensor_ = nullptr;
  std::shared_ptr<dt::StreamProfile> profile_ = nullptr;
  std::string name_;
  bool is_started_ = false;
  bool is_mirrored_ = false;
};
}  // namespace deptrum_camera
