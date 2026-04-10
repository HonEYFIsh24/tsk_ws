// Copyright (c) Deptrum Inc. All Rights Reserved.

#include <deptrum/deptrum.hpp>
#include <iomanip>
#include <iostream>
#include "utils.hpp"

void PrintDeviceList(const std::string& prompt, std::shared_ptr<dt::DeviceList> device_list) {
  auto count = device_list->GetCount();
  if (count == 0) {
    return;
  }
  std::cout << count << " device(s) " << prompt << ": " << std::endl;
  if (prompt == "added") {
    std::this_thread::sleep_for(std::chrono::seconds(3));
    for (uint32_t i = 0; i < count; i++) {
      auto device = device_list->GetDevice(i);
      auto uid = device->GetDeviceInfo()->GetUid();
      auto vid = device->GetDeviceInfo()->GetVid();
      auto pid = device->GetDeviceInfo()->GetPid();
      auto serial_number = device->GetDeviceInfo()->GetSerialNumber();
      auto connection = device->GetDeviceInfo()->GetConnectionType();
      std::cout << " - uid: " << uid << ", vid: " << vid << ", pid: " << pid
                << ", serial number: " << serial_number << ", connection: " << connection
                << std::endl;
    }
  } else if (prompt == "removed") {
    for (uint32_t i = 0; i < count; i++) {
      auto uid = device_list->GetUid(i);
      auto vid = device_list->GetVid(i);
      auto pid = device_list->GetPid(i);
      auto serial_number = device_list->GetSerialNumber(i);
      auto connection = device_list->GetConnectionType(i);
      std::cout << " - uid: " << uid << ", vid: 0x" << std::hex << std::setfill('0') << std::setw(4)
                << vid << ", pid: 0x" << std::setw(4) << pid << ", serial number: " << serial_number
                << ", connection: " << connection << std::endl;
    }
  }
  std::cout << std::endl;
}

void RebootDevices(std::shared_ptr<dt::DeviceList> device_list) {
  for (uint32_t i = 0; i < device_list->GetCount(); i++) {
    // get device from device list
    auto device = device_list->GetDevice(i);

    // reboot device
    device->Reboot();
  }
}

int main(void) try {
  // create context
  dt::Context ctx;

  // register device callback
  ctx.SetDeviceChangedCallback([](std::shared_ptr<dt::DeviceList> removed_list,
                                  std::shared_ptr<dt::DeviceList> device_list) {
    PrintDeviceList("added", device_list);
    PrintDeviceList("removed", removed_list);
  });

  // query current device list
  auto current_list = ctx.QueryDeviceList();
  PrintDeviceList("connected", current_list);

  std::cout << "Press 'r' to reboot the connected devices to trigger the device disconnect and "
               "reconnect event, or manually unplug and plugin the device."
            << std::endl;
  std::cout << "Press 'Esc' to exit." << std::endl << std::endl;

  // main loop, wait for key press
  while (true) {
    auto key = dt_utils::WaitForKeyPressed(100);
    // Press the esc key to exit
    if (key == 27) {
      break;
    } else if (key == 'r' || key == 'R') {
      // update device list
      current_list = ctx.QueryDeviceList();

      std::cout << "Rebooting devices..." << std::endl;
      RebootDevices(current_list);
    }
  }

  return 0;
} catch (dt::Error& e) {
  std::cerr << e << std::endl;
  std::cout << "\nPress any key to exit.";
  dt_utils::WaitForKeyPressed();
  exit(EXIT_FAILURE);
}
