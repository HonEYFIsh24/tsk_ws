// Copyright (c) Deptrum Inc. All Rights Reserved.

#include <algorithm>
#include <cstring>
#include <deptrum/deptrum.hpp>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
#include "utils.hpp"

std::shared_ptr<dt::Device> SelectDevice(std::shared_ptr<dt::DeviceList> device_list);
std::vector<DtOptionItem> GetOptionList(std::shared_ptr<dt::Device> device);
bool IsPrimaryTypeOption(DtOptionItem option_item);
void PrintfOptionList(std::shared_ptr<dt::Device> device,
                      const std::vector<DtOptionItem>& option_list);
void SetOptionValue(std::shared_ptr<dt::Device> device, DtOptionItem item, std::string str_value);
void GetOptionValue(std::shared_ptr<dt::Device> device, DtOptionItem item);
std::string PermissionTypeToString(DtPermissionType permission);

int main(void) try {
  // Create a Context.
  dt::Context context;

  // Query the list of connected devices
  auto device_list = context.QueryDeviceList();

  bool is_select_device = true;
  while (is_select_device) {
    // select a device to operate
    std::shared_ptr<dt::Device> device = nullptr;
    if (device_list->GetCount() > 0) {
      if (device_list->GetCount() <= 1) {
        // If a single device is plugged in, the first one is selected by default
        device = device_list->GetDevice(0);
      } else {
        device = SelectDevice(device_list);
      }
      auto device_info = device->GetDeviceInfo();
      std::cout << "\n------------------------------------------------------------------------\n";
      std::cout << "Current Device: "
                << " name: " << device_info->GetName() << ", vid: " << device_info->GetVid()
                << ", pid: " << device_info->GetPid() << ", uid: " << device_info->GetUid()
                << std::endl;
    } else {
      std::cout << "Device Not Found" << std::endl;
      is_select_device = false;
      break;
    }

    std::cout << "Input \"?\" to get all properties." << std::endl;

    std::vector<DtOptionItem> option_list = GetOptionList(device);
    std::sort(option_list.begin(),
              option_list.end(),
              [](const DtOptionItem& a, const DtOptionItem& b) { return a.id < b.id; });

    bool is_select_option = true;
    while (is_select_option) {
      std::string choice;
      std::getline(std::cin, choice);

      if (choice != "?") {
        std::istringstream ss(choice);
        std::string tmp;
        std::vector<std::string> control_vec;
        while (ss >> tmp) {
          control_vec.push_back(tmp);
        }

        if (control_vec.size() <= 0)
          continue;

        // exit the program
        if (control_vec.at(0) == "exit") {
          is_select_option = false;
          is_select_device = false;
          break;
        }

        // Check if it matches the input format
        if (control_vec.size() <= 1 || (control_vec.at(1) != "get" && control_vec.at(1) != "set") ||
            control_vec.size() > 3 || (control_vec.at(1) == "set" && control_vec.size() < 3)) {
          std::cout << "Option control usage: [option index] [set] [option value] or "
                       "[option index] [get]"
                    << std::endl;
          continue;
        }
        size_t size = option_list.size();
        size_t select_id = std::atoi(control_vec.at(0).c_str());
        if (select_id >= size) {
          std::cout << "Your selection is out of range, please reselect: " << std::endl;
          continue;
        }

        bool is_get_value = control_vec.at(1) == "get" ? true : false;
        auto option_item = option_list.at(select_id);

        if (is_get_value) {
          // get option value
          GetOptionValue(device, option_item);
        } else {
          // set option value
          SetOptionValue(device, option_item, control_vec.at(2));
        }
      } else {
        PrintfOptionList(device, option_list);
        std::cout << "Please select option.(Option control usage: [option number] [set/get] "
                     "[option value])"
                  << std::endl;
      }
    }
  }

  return 0;
} catch (dt::Error& e) {
  std::cerr << e << std::endl;
  std::cout << "\nPress any key to exit.";
  dt_utils::WaitForKeyPressed();
  exit(EXIT_FAILURE);
}

// Select a device, the name, pid, vid, uid of the device will be printed here, and the
// corresponding device object will be created after selection
std::shared_ptr<dt::Device> SelectDevice(std::shared_ptr<dt::DeviceList> device_list) {
  int dev_count = device_list->GetCount();
  std::cout << "Device list: " << std::endl;
  for (int i = 0; i < dev_count; i++) {
    std::cout << i << ". name: " << device_list->GetName(i) << ", vid: 0x" << std::hex
              << device_list->GetVid(i) << ", pid: 0x" << std::setw(4) << std::setfill('0')
              << device_list->GetPid(i) << ", uid: 0x" << device_list->GetUid(i)
              << ", sn: " << device_list->GetSerialNumber(i) << std::dec << std::endl;
  }
  std::cout << "Select a device: ";

  int dev_index;
  std::cin >> dev_index;
  while (dev_index < 0 || dev_index >= dev_count || std::cin.fail()) {
    std::cin.clear();
    std::cin.ignore();
    std::cout << "Your select is out of range, please reselect: " << std::endl;
    std::cin >> dev_index;
  }

  return device_list->GetDevice(dev_index);
}

// Print a list of supported properties
void PrintfOptionList(std::shared_ptr<dt::Device> device,
                      const std::vector<DtOptionItem>& option_list) {
  std::cout << "size: " << option_list.size() << std::endl;
  if (option_list.empty()) {
    std::cout << "No supported option!" << std::endl;
  }
  std::cout << "\n------------------------------------------------------------------------\n";
  for (size_t i = 0; i < option_list.size(); i++) {
    auto option_item = option_list[i];
    std::string str_range = "";

    DtIntOptionRange int_range;
    DtFloatOptionRange float_range;
    switch (option_item.type) {
      case kDtOptionBool:
        str_range = "Bool value(min:0, max:1, step:1)";
        break;
      case kDtOptionInt: {
        try {
          int_range = device->GetIntOptionRange(option_item.id);
          str_range = "Int value(min:" + std::to_string(int_range.min) +
                      ", max:" + std::to_string(int_range.max) +
                      ", step:" + std::to_string(int_range.step) + ")";
        } catch (...) { std::cout << "get int option range failed." << std::endl; }
      } break;
      case kDtOptionFloat:
        try {
          float_range = device->GetFloatOptionRange(option_item.id);
          str_range = "Float value(min:" + std::to_string(float_range.min) +
                      ", max:" + std::to_string(float_range.max) +
                      ", step:" + std::to_string(float_range.step) + ")";
        } catch (...) { std::cout << "get float option range failed." << std::endl; }
        break;
      default:
        break;
    }

    std::cout.setf(std::ios::right);
    std::cout.fill('0');
    std::cout.width(2);
    std::cout << i << ". ";
    std::cout << option_item.name << "(" << (int) option_item.id << ")";
    std::cout << ", permission=" << PermissionTypeToString(option_item.permission)
              << ", range=" << str_range << std::endl;
  }
  std::cout << "------------------------------------------------------------------------\n";
}

bool IsPrimaryTypeOption(DtOptionItem option_item) {
  return option_item.type == kDtOptionInt || option_item.type == kDtOptionFloat ||
         option_item.type == kDtOptionBool;
}

// Get option list
std::vector<DtOptionItem> GetOptionList(std::shared_ptr<dt::Device> device) {
  std::vector<DtOptionItem> option_vec;
  option_vec.clear();
  uint32_t size = device->GetSupportedOptionCount();
  for (uint32_t i = 0; i < size; i++) {
    DtOptionItem option_item = device->GetSupportedOption(i);
    if (IsPrimaryTypeOption(option_item) && option_item.permission != kDtPermissionDeny) {
      option_vec.push_back(option_item);
    }
  }
  return option_vec;
}

// set properties
void SetOptionValue(std::shared_ptr<dt::Device> device,
                    DtOptionItem option_item,
                    std::string str_value) {
  try {
    int int_value = 0;
    float float_value = 0.0f;
    int bool_value = 0;
    switch (option_item.type) {
      case kDtOptionBool:
        bool_value = std::atoi(str_value.c_str());
        try {
          device->SetBoolOption(option_item.id, bool_value);
        } catch (dt::Error& e) { std::cout << "set bool option fail: " << e.What() << std::endl; }
        std::cout << "option name:" << option_item.name << ",set bool value:" << bool_value
                  << std::endl;
        break;
      case kDtOptionInt:
        int_value = std::atoi(str_value.c_str());
        try {
          device->SetIntOption(option_item.id, int_value);
        } catch (dt::Error& e) { std::cout << "set int option fail: " << e.What() << std::endl; }
        std::cout << "option name:" << option_item.name << ",set int value:" << int_value
                  << std::endl;
        break;
      case kDtOptionFloat:
        float_value = static_cast<float>(std::atof(str_value.c_str()));
        try {
          device->SetFloatOption(option_item.id, float_value);
        } catch (dt::Error& e) { std::cout << "set floar option fail: " << e.What() << std::endl; }
        std::cout << "option name:" << option_item.name << ",set float value:" << float_value
                  << std::endl;
        break;
      default:
        break;
    }
  } catch (...) { std::cout << "set option failed: " << option_item.name << std::endl; }
}

// get option value
void GetOptionValue(std::shared_ptr<dt::Device> device, DtOptionItem option_item) {
  try {
    bool bool_ret = false;
    int int_ret = 0;
    float float_ret = 0.0f;

    switch (option_item.type) {
      case kDtOptionBool:
        try {
          bool_ret = device->GetBoolOption(option_item.id);
        } catch (dt::Error& e) { std::cout << "get bool option failed: " << e.What() << std::endl; }
        std::cout << "option name:" << option_item.name << ",get bool value:" << bool_ret
                  << std::endl;
        break;
      case kDtOptionInt:
        try {
          int_ret = device->GetIntOption(option_item.id);
        } catch (dt::Error& e) { std::cout << "get int option failed: " << e.What() << std::endl; }
        std::cout << "option name:" << option_item.name << ",get int value:" << int_ret
                  << std::endl;
        break;
      case kDtOptionFloat:
        try {
          float_ret = device->GetFloatOption(option_item.id);
        } catch (dt::Error& e) {
          std::cout << "get float option failed: " << e.What() << std::endl;
        }
        std::cout << "option name:" << option_item.name << ",get float value:" << float_ret
                  << std::endl;
        break;
      default:
        break;
    }
  } catch (...) { std::cout << "get option failed: " << option_item.name << std::endl; }
}

std::string PermissionTypeToString(DtPermissionType permission) {
  switch (permission) {
    case kDtPermissionRead:
      return "R/_";
    case kDtPermissionWrite:
      return "_/W";
    case kDtPermissionReadWrite:
      return "R/W";

    default:
      break;
  }
  return "_/_";
}
