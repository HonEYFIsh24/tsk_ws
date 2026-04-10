// Copyright (c) Deptrum Inc. All Rights Reserved.

#include <deptrum/deptrum.hpp>
#include <iomanip>
#include <iostream>
#include "utils.hpp"

// get input option
int GetInputOption() {
  char input_option = dt_utils::WaitForKeyPressed();
  if (input_option == ESC_KEY) {
    return -1;
  }
  return input_option - '0';
}

// Print stream profile information.
void PrintStreamProfile(std::shared_ptr<dt::StreamProfile> profile, uint32_t index) {
  // Get the video profile.
  auto video_profile = profile->As<dt::VideoStreamProfile>();
  // Get the format.
  auto format_name = profile->GetFormat();
  // Get the width.
  auto width = video_profile->GetWidth();
  // Get the height.
  auto height = video_profile->GetHeight();
  // Get the fps.
  auto fps = video_profile->GetFps();
  std::cout << index << "."
            << "format: " << format_name << ", "
            << "res: " << width << "*" << height << ", "
            << "fps: " << fps << std::endl;
}

// Print accel profile information.
void PrintAccelProfile(std::shared_ptr<dt::StreamProfile> profile, uint32_t index) {
  // Get the profile of accel.
  auto acc_profile = profile->As<dt::AccelStreamProfile>();
  // Get the rate of accel.
  auto acc_rate = acc_profile->GetSampleRate();
  std::cout << index << "."
            << "acc rate: " << acc_rate << std::endl;
}

// Print gyro profile information.
void PrintGyroProfile(std::shared_ptr<dt::StreamProfile> profile, uint32_t index) {
  // Get the profile of gyro.
  auto gyro_profile = profile->As<dt::GyroStreamProfile>();
  // Get the rate of gyro.
  auto gyro_rate = gyro_profile->GetSampleRate();
  std::cout << index << "."
            << "gyro rate: " << gyro_rate << std::endl;
}

// Enumerate stream profiles.
void EnumerateStreamProfiles(std::shared_ptr<dt::Sensor> sensor) {
  // Get the list of stream profiles.
  auto stream_profile_list = sensor->GetStreamProfileList();
  // Get the sensor type.
  auto sensor_type = sensor->GetType();
  for (uint32_t index = 0; index < stream_profile_list->GetCount(); index++) {
    // Get the stream profile.
    auto profile = stream_profile_list->GetProfile(index);
    if (sensor_type == kDtSensorIr || sensor_type == kDtSensorColor ||
        sensor_type == kDtSensorDepth || sensor_type == kDtSensorIrLeft ||
        sensor_type == kDtSensorIrRight || sensor_type == kDtSensorColorLeft ||
        sensor_type == kDtSensorColorRight) {
      PrintStreamProfile(profile, index);
    } else if (sensor_type == kDtSensorAccel) {
      PrintAccelProfile(profile, index);
    } else if (sensor_type == kDtSensorGyro) {
      PrintGyroProfile(profile, index);
    } else {
      break;
    }
  }
}

// Enumerate sensors.
void EnumerateSensors(std::shared_ptr<dt::Device> device) {
  while (true) {
    std::cout << "Sensor list: " << std::endl;
    // Get the list of sensors.
    auto sensor_list = device->GetSensorList();
    for (uint32_t index = 0; index < sensor_list->GetCount(); index++) {
      // Get the sensor type.
      auto sensor_type = sensor_list->GetSensorType(index);
      std::cout << " - " << index << "."
                << "sensor type: " << sensor_type << std::endl;
    }

    std::cout << "Select a sensor to enumerate its streams(input sensor index or \'ESC\' to "
                 "enumerate device): "
              << std::endl;

    // Select a sensor.
    int sensor_selected = dt_utils::GetInputOption();
    if (sensor_selected >= static_cast<int>(sensor_list->GetCount()) || sensor_selected < 0) {
      if (sensor_selected == -1) {
        break;
      } else {
        std::cout << "\nInvalid input, please reselect the sensor!\n";
        continue;
      }
    }

    // Get sensor from sensor_list.
    auto sensor = sensor_list->GetSensor(sensor_selected);
    EnumerateStreamProfiles(sensor);
  }
}

int main(void) try {
  // Create a context.
  dt::Context context;

  while (true) {
    // Query the list of connected devices.
    auto device_list = context.QueryDeviceList();
    if (device_list->GetCount() < 1) {
      std::cout << "No device found! Please connect a supported device and retry this program."
                << std::endl;
      std::cout << "\nPress any key to exit.";
      dt_utils::WaitForKeyPressed();
      return -1;
    }

    std::cout << "enumerated devices: " << std::endl;

    std::shared_ptr<dt::Device> device = nullptr;
    std::shared_ptr<dt::DeviceInfo> device_info = nullptr;
    for (uint32_t index = 0; index < device_list->GetCount(); index++) {
      // Get device from device_list.
      device = device_list->GetDevice(index);
      // Get device information from device
      device_info = device->GetDeviceInfo();
      std::cout << "  " << index << " uid: " << device_info->GetUid()
                << ", device name: " << device_info->GetName()
                << ", device pid: " << device_info->GetPid()
                << ", device vid: " << device_info->GetVid()
                << ", device SN: " << device_info->GetSerialNumber()
                << ", connection type: " << device_info->GetConnectionType() << std::endl;
    }

    std::cout << "Select a device to enumerate its sensors (Input device index or \'ESC\' to exit "
                 "program):"
              << std::endl;

    // select a device.
    int device_selected = dt_utils::GetInputOption();
    if (device_selected >= static_cast<int>(device_list->GetCount()) || device_selected < 0) {
      if (device_selected == -1) {
        break;
      } else {
        std::cout << "\nInvalid input, please reselect the device!\n";
        continue;
      }
    }

    //  Get the device.
    auto selected_device = device_list->GetDevice(device_selected);
    EnumerateSensors(selected_device);
  }

  return 0;
} catch (dt::Error& e) {
  std::cerr << e << std::endl;
  std::cout << "\nPress any key to exit.";
  dt_utils::WaitForKeyPressed();
  exit(EXIT_FAILURE);
}
