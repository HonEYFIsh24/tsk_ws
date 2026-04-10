// Copyright (c) Deptrum Inc. All Rights Reserved.

#include <algorithm>
#include <cctype>
#include <deptrum/deptrum.hpp>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include "utils.hpp"

void FirmwareUpdateCallback(DtFwUpdateState state, const char* message, uint8_t percent);
bool GetFirmwarePath(std::string& firmware_path);
bool SelectDevice(int& device_index);
void PrintDeviceList();

bool first_call = true;
std::vector<std::shared_ptr<dt::Device>> devices{};

int main() try {
  // Create a context to access the connected devices
  std::shared_ptr<dt::Context> context = std::make_shared<dt::Context>();

#if defined(__linux__)
  // On Linux, it is recommended to use the libuvc backend for device access as v4l2 is not always
  // reliable on some systems for firmware update.
  context->SetUvcBackendType(kDtUvcBackendTypeLibuvc);
#endif

  // Get connected devices from the context
  std::shared_ptr<dt::DeviceList> device_list = context->QueryDeviceList();
  if (device_list->GetCount() == 0) {
    std::cout << "No device found. Please connect a device first!" << std::endl;
    std::cout << "Press any key to exit..." << std::endl;
    dt_utils::WaitForKeyPressed();
    return 0;
  }

  for (uint32_t i = 0; i < device_list->GetCount(); ++i) {
    devices.push_back(device_list->GetDevice(i));
  }
  std::cout << "Devices found:" << std::endl;
  PrintDeviceList();

  while (true) {
    first_call = true;
    int device_index = -1;

    if (!SelectDevice(device_index)) {
      break;
    }

    std::string firmware_path;
    if (!GetFirmwarePath(firmware_path)) {
      break;
    }

    std::cout << "Upgrading device firmware, please wait...\n\n";
    try {
      // Set async to false to synchronously block and wait for the device firmware upgrade to
      // complete.
      devices[device_index]->UpdateFirmware(firmware_path.c_str(), FirmwareUpdateCallback, false);
    } catch (dt::Error& e) {
      // If the update fails, will throw an exception.
      std::cerr << "\nThe upgrade was interrupted! An error occurred! " << std::endl;
      std::cerr << "Error message: " << e.What() << std::endl;
      std::cout << "Press any key to exit." << std::endl;
      dt_utils::WaitForKeyPressed();
      break;
    }

    std::string input;
    std::cout << "Enter 'Q' or 'q' to quit, or any other key to continue: ";
    std::getline(std::cin, input);
    if (input == "Q" || input == "q") {
      break;
    }
  }
} catch (dt::Error& e) {
  std::cerr << e << std::endl;
  std::cout << "\nPress any key to exit.";
  dt_utils::WaitForKeyPressed();
  exit(EXIT_FAILURE);
}

void FirmwareUpdateCallback(DtFwUpdateState state, const char* message, uint8_t percent) {
  if (first_call) {
    first_call = !first_call;
  } else {
    std::cout << "\033[3F";  // Move cursor up 3 lines
  }

  std::cout << "\033[K";  // Clear the current line
  std::cout << "Progress: " << static_cast<uint32_t>(percent) << "%" << std::endl;

  std::cout << "\033[K";
  std::cout << "Status  : ";
  switch (state) {
    case kDtStatVerifySuccess:
      std::cout << "Image file verification success" << std::endl;
      break;
    case kDtStatFileTransfer:
      std::cout << "File transfer in progress" << std::endl;
      break;
    case kDtStatDone:
      std::cout << "Update completed" << std::endl;
      break;
    case kDtStatInProgress:
      std::cout << "Upgrade in progress" << std::endl;
      break;
    case kDtStatStart:
      std::cout << "Starting the upgrade" << std::endl;
      break;
    case kDtStatVerifyImage:
      std::cout << "Verifying image file" << std::endl;
      break;
    default:
      std::cout << "Unknown status or error" << std::endl;
      break;
  }

  std::cout << "\033[K";
  std::cout << "Message : " << message << std::endl << std::flush;
}

bool GetFirmwarePath(std::string& firmware_path) {
  std::cout << "Please input the path of the firmware file (.sign.tar.gz) to be updated:"
            << std::endl;
  std::cout << "(Enter 'Q' or 'q' to quit): " << std::endl;
  std::cout << "Path: ";
  std::string input;
  std::getline(std::cin, input);

  if (input == "Q" || input == "q") {
    exit(EXIT_SUCCESS);
  }

  // Remove leading and trailing whitespaces
  input.erase(
      std::find_if(input.rbegin(), input.rend(), [](unsigned char ch) { return !std::isspace(ch); })
          .base(),
      input.end());

  // Remove leading and trailing quotes
  if (!input.empty() && input.front() == '\'' && input.back() == '\'') {
    input = input.substr(1, input.size() - 2);
  }

  firmware_path = input;
  std::cout << "Firmware file confirmed: " << firmware_path << std::endl << std::endl;
  return true;
}

void PrintDeviceList() {
  std::cout << "--------------------------------------------------------------------------------\n";
  for (uint32_t i = 0; i < devices.size(); ++i) {
    std::cout << "[" << i << "] "
              << "Device: " << devices[i]->GetDeviceInfo()->GetName();
    std::cout << " | SN: " << devices[i]->GetDeviceInfo()->GetSerialNumber();
    std::cout << " | Firmware version: " << devices[i]->GetDeviceInfo()->GetFirmwareVersion()
              << std::endl;
  }
  std::cout
      << "---------------------------------------------------------------------------------\n";
}

bool SelectDevice(int& device_index) {
  std::string input;
  while (true) {
    std::cout << "Please select a device to update the firmware, enter 'l' to list devices, or "
                 "enter 'q' to quit: "
              << std::endl;
    std::cout << "Device index: ";
    std::getline(std::cin, input);

    if (input == "Q" || input == "q") {
      return false;
    }

    if (input == "l" || input == "L") {
      PrintDeviceList();
      continue;
    }

    try {
      device_index = std::stoi(input);
      if (device_index < 0 || device_index >= static_cast<int>(devices.size())) {
        std::cout << "Invalid input, please enter a valid index number." << std::endl;
        continue;
      }
      std::cout << std::endl;
      break;
    } catch (...) {
      std::cout << "Invalid input, please enter a valid index number." << std::endl;
      continue;
    }
  }
  return true;
}