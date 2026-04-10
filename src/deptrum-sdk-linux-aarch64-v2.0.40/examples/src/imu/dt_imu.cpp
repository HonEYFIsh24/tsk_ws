// Copyright (c) Deptrum Inc. All Rights Reserved.

#include <deptrum/deptrum.hpp>
#include <iostream>
#include <mutex>
#include "utils.hpp"
#include "utils_types.h"

void PrintImuValue(DtFloat3D dt_float3d,
                   uint64_t index,
                   uint64_t time_stamp_us,
                   float temperature,
                   DtFrameType type,
                   const std::string& unit_str) {
  std::cout << "frame index: " << index << std::endl;
  std::cout << type << " Frame: \n\r{\n\r"
            << "  tsp = " << time_stamp_us << "\n\r"
            << "  temperature = " << temperature << "\n\r"
            << "  " << type << ".x = " << dt_float3d.x << unit_str << "\n\r"
            << "  " << type << ".y = " << dt_float3d.y << unit_str << "\n\r"
            << "  " << type << ".z = " << dt_float3d.z << unit_str << "\n\r"
            << "}\n\r" << std::endl;
}

int main() try {
  // Create a pipeline with default device.
  dt::Pipeline pipe;

  // Configure which streams to enable or disable for the Pipeline by creating a Config.
  std::shared_ptr<dt::Config> config = std::make_shared<dt::Config>();

  // Enable Accel stream.
  config->EnableAccelStream();

  // Enable Gyro stream.
  config->EnableGyroStream();

  // Start the pipeline with config.
  pipe.Start(config);

  while (true) {
    auto key = dt_utils::WaitForKeyPressed(1);
    if (key == ESC_KEY) {  // Esc key to exit.
      break;
    }
    auto frame_set = pipe.WaitForFrames();
    if (frame_set == nullptr) {
      continue;
    }

    auto accel_frame_raw = frame_set->GetFrame(kDtFrameAccel);
    if (accel_frame_raw == nullptr) {
      continue;
    }
    auto accel_frame = accel_frame_raw->As<dt::AccelFrame>();
    auto accel_index = accel_frame->GetIndex();
    auto accel_time_stamp_us = accel_frame->GetTimeStampUs();
    auto accel_temperature = accel_frame->GetTemperature();
    auto accel_type = accel_frame->GetType();
    if (accel_index % 50 == 0) {  // print information every 50 frames.
      auto accel_value = accel_frame->GetValue();
      PrintImuValue(accel_value,
                    accel_index,
                    accel_time_stamp_us,
                    accel_temperature,
                    accel_type,
                    "m/s^2");
    }

    auto gyro_frame_raw = frame_set->GetFrame(kDtFrameGyro);
    if (gyro_frame_raw == nullptr) {
      continue;
    }
    auto gyro_frame = gyro_frame_raw->As<dt::GyroFrame>();
    auto gyro_index = gyro_frame->GetIndex();
    auto gyro_time_stamp_us = gyro_frame->GetTimeStampUs();
    auto gyro_temperature = gyro_frame->GetTemperature();
    auto gyro_type = gyro_frame->GetType();
    if (gyro_index % 50 == 0) {  // print information every 50 frames.
      auto gyro_value = gyro_frame->GetValue();
      PrintImuValue(gyro_value,
                    gyro_index,
                    gyro_time_stamp_us,
                    gyro_temperature,
                    gyro_type,
                    "rad/s");
    }
  }

  pipe.Stop();

  return 0;
} catch (dt::Error& e) {
  std::cerr << e << std::endl;
  std::cout << "\nPress any key to exit.";
  dt_utils::WaitForKeyPressed();
  exit(EXIT_FAILURE);
}
