// Copyright (c) Deptrum Inc. All Rights Reserved.
#include <ros/ros.h>
#include <deptrum_camera/types.h>
#include <deptrum_camera/utils.h>
#include <string>
#include <regex>
#include <thread>
std::string ParseUsbPort(const std::string &line) {
  std::string port_id;
  std::regex self_regex("(?:[^ ]+/usb[0-9]+[0-9./-]*/){0,1}([0-9.-]+)(:){0,1}[^ ]*",
                        std::regex_constants::ECMAScript);
  std::smatch base_match;
  bool found = std::regex_match(line, base_match, self_regex);
  if (found) {
    port_id = base_match[1].str();
    if (base_match[2].str().empty())  // This is libuvc string. Remove counter is exists.
    {
      std::regex end_regex = std::regex(".+(-[0-9]+$)", std::regex_constants::ECMAScript);
      bool found_end = std::regex_match(port_id, base_match, end_regex);
      if (found_end) {
        port_id = port_id.substr(0, port_id.size() - base_match[1].str().size());
      }
    }
  }
  return port_id;
}
int main() {
  try {
    dt::Logger::SetLoggerSeverity(DtLogSeverity::kDtLogSeverityOff);
    auto context = std::make_shared<dt::Context>();
    auto list = context->QueryDeviceList();
    for (size_t i = 0; i < list->GetCount(); i++) {
      if (std::string(list->GetConnectionType(i)) != "Ethernet") {
        std::string serial = list->GetSerialNumber(i);
        std::string uid = list->GetUid(i);
        auto usb_port = ParseUsbPort(uid);
        auto connection_type = list->GetConnectionType(i);
        std::stringstream pid_hex;
        pid_hex << std::hex << std::setw(4) << std::setfill('0') << list->GetPid(i);
        ROS_INFO_STREAM("- Name: " << list->GetName(i) << ", PID: 0x" << pid_hex.str()
                                   << ", SN/ID: " << serial << ", Connection: " << connection_type);
        ROS_INFO_STREAM("serial: " << serial);
        ROS_INFO_STREAM("port id : " << usb_port);
        ROS_INFO_STREAM("usb connect type: " << connection_type);
        std::cout << std::endl;
      } else {
        std::string serial = list->GetSerialNumber(i);
        auto connection_type = list->GetConnectionType(i);
        auto ip_address = list->GetIpAddress(i);
        std::stringstream pid_hex;
        pid_hex << std::hex << std::setw(4) << std::setfill('0') << list->GetPid(i);
        ROS_INFO_STREAM("- Name: " << list->GetName(i) << ", PID: 0x" << pid_hex.str()
                                   << ", SN/ID: " << serial << ", Connection: " << connection_type);
        ROS_INFO_STREAM("serial: " << serial);
        ROS_INFO_STREAM("ip address: " << ip_address);
        ROS_INFO_STREAM("usb connect type: " << connection_type);
        std::cout << std::endl;
      }
    }
  } catch (dt::Error &e) {
    ROS_ERROR_STREAM("list_device_node: " << e.GetMessage());
  } catch (const std::exception &e) {
    ROS_ERROR_STREAM("list_device_node: " << e.what());
  } catch (...) {
    ROS_ERROR_STREAM("list_device_node: " << "unknown error");
  }
  return 0;
}
