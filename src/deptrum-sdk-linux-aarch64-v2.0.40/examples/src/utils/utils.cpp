// Copyright (c) Deptrum Inc. All Rights Reserved.

#include "utils.hpp"
#include <chrono>
#include "types.hpp"
#include "utils_c.h"

namespace dt_utils {
char WaitForKeyPressed(uint32_t timeout_ms) {
  return dt_utils_wait_for_key_press(timeout_ms);
}

uint64_t GetNowTimesMs() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::system_clock::now().time_since_epoch())
      .count();
}

int GetInputOption() {
  char input_option = dt_utils::WaitForKeyPressed();
  if (input_option == ESC_KEY) {
    return -1;
  }
  return input_option - '0';
}

void CheckDepthMode(std::shared_ptr<dt::Device> device) {
  bool is_supported = device->IsOptionSupported(kDtOptionDepthModeInt, kDtPermissionReadWrite);
  if (is_supported) {
    int depth_mode = device->GetIntOption(kDtOptionDepthModeInt);
    std::cout << "Device is currently mode" << depth_mode << std::endl;
    std::cout << "0: DEPTH_MODE_0,\n"
                 "1: DEPTH_MODE_1,\n"
                 "2: DEPTH_MODE_2,\n"
                 "3: DEPTH_MODE_3(only for nebula280),\n";
    std::cout
        << "Enter 0 to set to CAMERA_MODE_0 mode, or 1 to set to CAMERA_MODE_1 mode, or 2 to set "
           "to CAMERA_MODE_2 mode:"
        << std::endl;
    int user_choice;
    std::cin >> user_choice;
    device->SetIntOption(kDtOptionDepthModeInt, user_choice);
  }
}
void CheckDeviceMode(std::shared_ptr<dt::Device> device) {
  bool is_supported = device->IsOptionSupported(kDtOptionSlaveDeviceSyncStatusBool,
                                                kDtPermissionReadWrite);
  if (is_supported) {
    bool is_slave = device->GetBoolOption(kDtOptionSlaveDeviceSyncStatusBool);
    std::cout << "Device is currently " << (is_slave ? "Slave" : "Master") << " mode." << std::endl;
    std::cout << "Enter 0 to set to Master mode, or 1 to set to Slave mode: ";
    int user_choice;
    std::cin >> user_choice;
    if (user_choice == 0) {
      // 设置为主模式
      device->SetBoolOption(kDtOptionSlaveDeviceSyncStatusBool, false);
      std::cout << "Device set to Master mode." << std::endl;
    } else if (user_choice == 1) {
      // 设置为从模式
      device->SetBoolOption(kDtOptionSlaveDeviceSyncStatusBool, true);
      std::cout << "Device set to Slave  mode." << std::endl;
    }
  }
}
std::shared_ptr<dt::StreamProfile> SelectStreamProfileList(std::shared_ptr<dt::Device> device) {
  auto sensor_list = device->GetSensorList();
  int sensor_count = sensor_list->GetCount();

  for (int i = 0; i < sensor_count; i++) {
    auto sensor = sensor_list->GetSensor(i);
    auto profile_list = sensor->GetStreamProfileList();
    int profile_count = profile_list->GetCount();

    std::cout << "Sensor " << i << " stream profile list count: " << profile_count << std::endl;

    for (int j = 0; j < profile_count; j++) {
      std::shared_ptr<dt::StreamProfile> stream_profile = profile_list->GetProfile(j);
      std::cout << "[" << j << "] "
                << "Stream type: " << stream_profile->GetType()
                << ", Format: " << stream_profile->GetFormat();

      if (stream_profile->Is<dt::VideoStreamProfile>()) {
        auto video_stream_profile = stream_profile->As<dt::VideoStreamProfile>();
        std::cout << ", Fps: " << video_stream_profile->GetFps()
                  << ", Width: " << video_stream_profile->GetWidth()
                  << ", Height: " << video_stream_profile->GetHeight();
      }
      std::cout << std::endl;
    }

    int selected_index = -1;
    std::cout << "Select a stream profile index for sensor " << i << ": ";
    std::cin >> selected_index;

    if (selected_index >= 0 && selected_index < profile_count) {
      return profile_list->GetProfile(selected_index);
    } else {
      std::cout << "Invalid index. Skipping." << std::endl;
    }
  }

  return nullptr;
}

}  // namespace dt_utils
