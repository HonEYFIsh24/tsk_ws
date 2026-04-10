// Copyright (c) Deptrum Inc. All Rights Reserved.

#include "deptrum_camera/dt_camera_node_driver.h"
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <ros/package.h>
#include <regex>
#include <sys/mman.h>
#include <iomanip>  // For std::put_time

#include <boost/filesystem.hpp>

namespace deptrum_camera {
backward::SignalHandling sh;

std::string g_camera_name = "camera";

void signalHandler(int signum) {
  std::cout << "Received signal: " << signum << std::endl;

  std::string log_dir = "Log/";

  // get current time
  std::time_t now = std::time(nullptr);
  std::tm *local_time = std::localtime(&now);

  // format date and time to string, format as "2024_05_20_12_34_56"
  std::ostringstream time_stream;
  time_stream << std::put_time(local_time, "%Y_%m_%d_%H_%M_%S");

  // generate log file name
  std::string log_file_name = g_camera_name + "_crash_stack_trace_" + time_stream.str() + ".log";
  std::string log_file_path = log_dir + log_file_name;

  if (!boost::filesystem::exists(log_dir)) {
    boost::filesystem::create_directories(log_dir);
  }
  auto abs_path = boost::filesystem::absolute(log_dir);
  std::cout << "Log crash stack trace to " << abs_path.string() << "/" << log_file_name
            << std::endl;
  std::ofstream log_file(log_file_path, std::ios::app);

  if (log_file.is_open()) {
    log_file << "Received signal: " << signum << std::endl;

    backward::StackTrace st;
    st.load_here(32);  // Capture stack
    backward::Printer p;
    p.print(st, log_file);  // Print stack to log file
  }

  log_file.close();
  exit(signum);  // Exit program
}
DtCameraNodeDriver::DtCameraNodeDriver(ros::NodeHandle &nh, ros::NodeHandle &nh_private)
    : nh_(nh),
      nh_private_(nh_private),
      config_path_(ros::package::getPath("deptrum_camera") + "/config/DeptrumSDKConfig.xml") {
  Init();
}

DtCameraNodeDriver::~DtCameraNodeDriver() {
  is_alive_ = false;
  if (reset_device_thread_ && reset_device_thread_->joinable()) {
    reset_device_cv_.notify_all();
    reset_device_thread_->join();
  }
  if (query_thread_ && query_thread_->joinable()) {
    query_thread_->join();
  }
}

void DtCameraNodeDriver::Init() {
  is_alive_ = true;
  signal(SIGSEGV, signalHandler);  // segment fault
  signal(SIGABRT, signalHandler);  // abort
  signal(SIGFPE, signalHandler);   // float point exception
  signal(SIGILL, signalHandler);   // illegal instruction
  prefix_paths = std::getenv("CMAKE_PREFIX_PATH");
  colon_pos = prefix_paths.find(':');
  first_prefix = prefix_paths.substr(0, colon_pos);
  // extension_path_ = first_prefix + "/lib/extensions";
  // dt::Context::setExtensionsDirectory(extension_path_.c_str());
  ctx_ = std::make_shared<dt::Context>();
  auto log_level = nh_private_.param<std::string>("log_level", "info");
  g_camera_name = nh_private_.param<std::string>("camera_name", "camera");
  auto dt_log_level = DtLogSeverityFromString(log_level);
  dt::Logger::SetLoggerToConsole(dt_log_level);
  det_device_lock_shm_fd_ = shm_open(DET_DEFAULT_LOCK_NAME.c_str(), O_CREAT | O_RDWR, 0666);
  if (det_device_lock_shm_fd_ < 0) {
    ROS_ERROR_STREAM("Failed to open shared memory " << DET_DEFAULT_LOCK_NAME);
    return;
  }
  int ret = ftruncate(det_device_lock_shm_fd_, sizeof(pthread_mutex_t));
  if (ret < 0) {
    ROS_ERROR_STREAM("Failed to truncate shared memory " << DET_DEFAULT_LOCK_NAME);
    return;
  }
  det_device_lock_shm_addr_ =
      static_cast<uint8_t *>(mmap(NULL, sizeof(pthread_mutex_t), PROT_READ | PROT_WRITE, MAP_SHARED,
                                  det_device_lock_shm_fd_, 0));
  if (det_device_lock_shm_addr_ == MAP_FAILED) {
    ROS_ERROR_STREAM("Failed to map shared memory " << DET_DEFAULT_LOCK_NAME);
    return;
  }
  pthread_mutexattr_init(&det_device_lock_attr_);
  pthread_mutexattr_setpshared(&det_device_lock_attr_, PTHREAD_PROCESS_SHARED);
  det_device_lock_ = (pthread_mutex_t *)det_device_lock_shm_addr_;
  pthread_mutex_init(det_device_lock_, &det_device_lock_attr_);
  serial_number_ = nh_private_.param<std::string>("serial_number", "");
  usb_port_ = nh_private_.param<std::string>("usb_port", "");
  connection_delay_ = nh_private_.param<int>("connection_delay", 100);
  device_num_ = static_cast<int>(nh_private_.param<int>("device_num", 1));
  enumerate_net_device_ = nh_private_.param<bool>("enumerate_net_device", false);
  ip_address_ = nh_private_.param<std::string>("ip_address", "");
  port_ = nh_private_.param<int>("port", 0);
  enable_hardware_reset_ = nh_private_.param<bool>("enable_hardware_reset", false);
  uvc_backend_ = nh_private_.param<std::string>("uvc_backend", "libuvc");
  // preset_firmware_path_ = nh_private_.param<std::string>("preset_firmware_path", "");
  upgrade_firmware_ = nh_private_.param<std::string>("upgrade_firmware", "");
  reboot_service_srv_ = nh_.advertiseService<std_srvs::EmptyRequest, std_srvs::EmptyResponse>(
      "/" + g_camera_name + "/reboot_device",
      [this](std_srvs::EmptyRequest &request, std_srvs::EmptyResponse &response) {
        return RebootDeviceServiceCallback(request, response);
      });
  if (uvc_backend_ == "libuvc") {
    ctx_->SetUvcBackendType(kDtUvcBackendTypeLibuvc);
    ROS_INFO_STREAM("SetUvcBackendType:" << uvc_backend_);
  } else if (uvc_backend_ == "v4l2") {
    ctx_->SetUvcBackendType(kDtUvcBackendTypeV4l2);
    ROS_INFO_STREAM("SetUvcBackendType:" << uvc_backend_);
  } else {
    ctx_->SetUvcBackendType(kDtUvcBackendTypeLibuvc);
    ROS_INFO_STREAM("SetUvcBackendType:" << uvc_backend_);
  }
  ctx_->EnableNetDeviceEnumeration(enumerate_net_device_);
  check_connection_timer_ =
      nh_.createWallTimer(ros::WallDuration(1.0),
                          [this](const ros::WallTimerEvent &) { this->CheckConnectionTimer(); });
  ctx_->SetDeviceChangedCallback([this](const std::shared_ptr<dt::DeviceList> &removed_list,
                                        const std::shared_ptr<dt::DeviceList> &added_list) {
    DeviceConnectCallback(added_list);
    DeviceDisconnectCallback(removed_list);
  });
  query_thread_ = std::make_shared<std::thread>([this]() { QueryDevice(); });
  reset_device_thread_ = std::make_shared<std::thread>([this]() { ResetDeviceThread(); });
}

std::shared_ptr<dt::Device> DtCameraNodeDriver::SelectDevice(
    const std::shared_ptr<dt::DeviceList> &list) {
  std::shared_ptr<dt::Device> device = nullptr;
  if (!ip_address_.empty() && port_ != 0) {
    ROS_INFO_STREAM("Connecting to device with net ip: " << ip_address_);
    device = SelectDeviceByNetIp(list, ip_address_);
  } else if (!serial_number_.empty()) {
    ROS_INFO_STREAM("Connecting to device with serial number: " << serial_number_);
    device = SelectDeviceBySerialNumber(list, serial_number_);
  } else if (!usb_port_.empty()) {
    ROS_INFO_STREAM("Connecting to device with usb port: " << usb_port_);
    device = SelectDeviceByUsbPort(list, usb_port_);
  } else if (device_num_ == 1) {
    ROS_INFO_STREAM("Connecting to the default device");
    return list->GetDevice(0);
  }
  if (device == nullptr) {
    ROS_WARN_THROTTLE(1.0, "Device with serial number %s not found", serial_number_.c_str());
    device_connected_ = false;
    return nullptr;
  }

  return device;
}

std::shared_ptr<dt::Device> DtCameraNodeDriver::SelectDeviceBySerialNumber(
    const std::shared_ptr<dt::DeviceList> &list, const std::string &serial_number) {
  for (size_t i = 0; i < list->GetCount(); i++) {
    std::lock_guard<decltype(device_lock_)> lock(device_lock_);
    try {
      // auto pid = list->GetPid(i);
      // if (isOpenNIDevice(pid)) {
      //   // openNI device
      //   auto dev = list->GetDevice(i);
      //   auto device_info = dev->GetDeviceInfo();
      //   if (device_info->GetSerialNumber() == serial_number) {
      //     ROS_INFO_STREAM("Device serial number " << device_info->GetSerialNumber() << " matched");
      //     return dev;
      //   }
      // } else {
      //   std::string sn = list->GetSerialNumber(i);
      //   ROS_INFO_STREAM("Device serial number: " << sn);
      //   if (sn == serial_number) {
      //     ROS_INFO_STREAM("Device serial number <<" << sn << " matched");
      //     return list->GetDevice(i);
      //   }
      // }

      std::string sn = list->GetSerialNumber(i);
        ROS_INFO_STREAM("Device serial number: " << sn);
        if (sn == serial_number) {
          ROS_INFO_STREAM("Device serial number <<" << sn << " matched");
          return list->GetDevice(i);
        }
    } catch (dt::Error &e) {
      ROS_ERROR_STREAM("Failed to get device info " << e.GetMessage());
    } catch (std::exception &e) {
      ROS_ERROR_STREAM("Failed to get device info " << e.what());
    } catch (...) {
      ROS_ERROR_STREAM("Failed to get device info");
    }
  }
  return nullptr;
}

std::shared_ptr<dt::Device> DtCameraNodeDriver::SelectDeviceByUsbPort(
    const std::shared_ptr<dt::DeviceList> &list, const std::string &usb_port) {
  try {
    ROS_INFO_STREAM("SelectDeviceByUsbPort : Before device lock lock");
    std::lock_guard<decltype(device_lock_)> lock(device_lock_);
    ROS_INFO_STREAM("SelectDeviceByUsbPort : After device lock lock");
    auto device = list->GetDeviceByUid(usb_port.c_str());
    ROS_INFO_STREAM("SelectDeviceByUsbPort : After getDeviceByUid");
    return device;
  } catch (dt::Error &e) {
    ROS_ERROR_STREAM("Failed to get device info " << e.GetMessage());
  } catch (std::exception &e) {
    ROS_ERROR_STREAM("Failed to get device info " << e.what());
  } catch (...) {
    ROS_ERROR_STREAM("Failed to get device info");
  }

  return nullptr;
}

std::shared_ptr<dt::Device> DtCameraNodeDriver::SelectDeviceByNetIp(
    const std::shared_ptr<dt::DeviceList> &list, const std::string &net_ip) {
  ROS_INFO_STREAM("Before lock: Select device net ip: " << net_ip);
  std::lock_guard<decltype(device_lock_)> lock(device_lock_);
  ROS_INFO_STREAM("After lock: Select device net ip: " << net_ip);
  for (size_t i = 0; i < list->GetCount(); i++) {
    try {
      if (std::string(list->GetConnectionType(i)) != "Ethernet") {
        continue;
      }
      if (list->GetIpAddress(i) == nullptr) {
        continue;
      }
      ROS_INFO_STREAM("FindDeviceByNetIP device net ip " << list->GetIpAddress(i));
      if (std::string(list->GetIpAddress(i)) == net_ip) {
        ROS_INFO_STREAM("getDeviceByNetIP device net ip " << net_ip << " done");
        return list->GetDevice(i);
      }
    } catch (dt::Error &e) {
      ROS_INFO_STREAM("Failed to get device info " << e.GetMessage());
      continue;
    } catch (std::exception &e) {
      ROS_INFO_STREAM("Failed to get device info " << e.what());
      continue;
    } catch (...) {
      ROS_INFO_STREAM("Failed to get device info");
      continue;
    }
  }

  return nullptr;
}

void DtCameraNodeDriver::InitializeDevice(const std::shared_ptr<dt::Device> &device) {
  std::lock_guard<decltype(device_lock_)> lock(device_lock_);
  if (device_) {
    ROS_WARN("device_ is not null, reset device_");
    device_.reset();
  }
  if (enable_hardware_reset_ && !hardware_reset_done_) {
    ROS_INFO("Reboot device");
    device->Reboot();
    ROS_INFO("Reboot device done");
    device_connected_ = false;
    hardware_reset_done_ = true;
    return;
  }
  device_ = device;
  // updatePresetFirmware(preset_firmware_path_);
  device_info_ = device_->GetDeviceInfo();
  device_uid_ = device_info_->GetUid();
  CHECK_NOTNULL(device_.get());
  if (dt_camera_node_) {
    dt_camera_node_.reset();
  }
  ROS_INFO_STREAM("test---- make_shared DtCameraNode");
  dt_camera_node_ = std::make_shared<DtCameraNode>(nh_, nh_private_, device_);
  if (!upgrade_firmware_.empty()) {
    device_->UpdateFirmware(
        upgrade_firmware_.c_str(),
        std::bind(&DtCameraNodeDriver::FirmwareUpdateCallback, this, std::placeholders::_1,
                  std::placeholders::_2, std::placeholders::_3),
        false);
  }
  if (dt_camera_node_ && dt_camera_node_->IsInitialized()) {
    ROS_INFO_STREAM("test---- Device  connected");
    device_connected_ = true;
  } else {
    ROS_INFO_STREAM("test---- Device  disconnected");
    device_connected_ = false;
    dt_camera_node_.reset();
    return;
  }
  // if (!isOpenNIDevice(device_info_->GetPid())) {
  //   ctx_->enableDeviceClockSync(1800000);
  // }
  CHECK_NOTNULL(device_info_.get());
  std::string connection_type = device_info_->GetConnectionType();
  ROS_INFO_STREAM("Device " << device_info_->GetName() << " connected");
  ROS_INFO_STREAM("Serial number: " << device_info_->GetSerialNumber());
  ROS_INFO_STREAM("Firmware version: " << device_info_->GetFirmwareVersion());
  ROS_INFO_STREAM("Hardware version: " << device_info_->GetHardwareVersion());
  ROS_INFO_STREAM("device uid: " << device_info_->GetUid());
  ROS_INFO_STREAM("device connection Type: " << connection_type);
  ROS_INFO_STREAM("Current node pid: " << getpid());
}

void DtCameraNodeDriver::DeviceConnectCallback(const std::shared_ptr<dt::DeviceList> &list) {
  ROS_INFO_STREAM("deviceConnectCallback : deviceConnectCallback start");
  CHECK_NOTNULL(list.get());
  if (device_connected_) {
    return;
  }
  if (list->GetCount() == 0) {
    ROS_WARN("No device found");
    return;
  }
  bool start_device_failed = false;
  try {
    std::this_thread::sleep_for(std::chrono::milliseconds(connection_delay_));
    ROS_INFO_STREAM("deviceConnectCallback : Before process lock lock");
    pthread_mutex_lock(det_device_lock_);
    ROS_INFO_STREAM("deviceConnectCallback : After process lock lock");
    std::shared_ptr<int> lock_guard(nullptr,
                                    [this](int *) { pthread_mutex_unlock(det_device_lock_); });
    ROS_INFO_STREAM("deviceConnectCallback : selectDevice start");
    auto device = SelectDevice(list);
    ROS_INFO_STREAM("deviceConnectCallback : selectDevice end");
    if (device == nullptr) {
      if (!serial_number_.empty()) {
        ROS_WARN_THROTTLE(1.0, "Device with serial number %s not found", serial_number_.c_str());
      } else if (!usb_port_.empty()) {
        ROS_WARN_THROTTLE(1.0, "Device with usb port %s not found", usb_port_.c_str());
      }
      device_connected_ = false;
      ROS_WARN_STREAM("deviceConnectCallback : start device failed, return");
      return;
    }
    InitializeDevice(device);
  } catch (dt::Error &e) {
    start_device_failed = true;
    ROS_ERROR_STREAM("Failed to initialize device " << e.GetMessage());
  } catch (std::exception &e) {
    start_device_failed = true;
    ROS_ERROR_STREAM("Failed to initialize device " << e.what());
  } catch (...) {
    start_device_failed = true;
    ROS_ERROR_STREAM("Failed to initialize device");
  }
  if (start_device_failed) {
    std::unique_lock<decltype(reset_device_lock_)> reset_lock(reset_device_lock_);
    reset_device_ = true;
    reset_device_cv_.notify_all();
  }
  ROS_INFO_STREAM("deviceConnectCallback : deviceConnectCallback end");
}

void DtCameraNodeDriver::ConnectNetDevice(const std::string &ip_address, int port) {
  if (ip_address.empty() || port == 0) {
    ROS_ERROR_STREAM("Invalid ip address or port");
    return;
  }
  ROS_INFO_STREAM("Connecting to net device " << ip_address << ":" << port);
  auto device = ctx_->CreateNetDevice(ip_address.c_str(), port);
  if (device == nullptr) {
    ROS_ERROR_STREAM("Failed to create net device");
    return;
  }
  InitializeDevice(device);
}

void DtCameraNodeDriver::CheckConnectionTimer() {
  if (!device_connected_) {
    ROS_DEBUG_STREAM("wait for device " << serial_number_ << " to be connected");
  } else if (!dt_camera_node_) {
    device_connected_ = false;
  }
}

void DtCameraNodeDriver::DeviceDisconnectCallback(
    const std::shared_ptr<dt::DeviceList> &device_list) {
  CHECK_NOTNULL(device_list.get());
  if (device_list->GetCount() == 0) {
    ROS_DEBUG_STREAM("device list is empty");
    return;
  }
  ROS_INFO("Device disconnected");
  if (device_info_ != nullptr) {
    ROS_INFO_STREAM("current node serial " << device_info_->GetSerialNumber());
  }
  for (size_t i = 0; i < device_list->GetCount(); i++) {
    std::string device_uid = device_list->GetUid(i);
    ROS_INFO_STREAM("Device with uid " << device_uid << " disconnected");
    if (device_uid == device_uid_) {
      ROS_INFO_STREAM("deviceDisconnectCallback : Before reset device, wait for device lock");
      std::unique_lock<decltype(reset_device_lock_)> reset_lock(reset_device_lock_);
      reset_device_ = true;
      reset_device_cv_.notify_all();
      ROS_INFO_STREAM(device_uid << " reset device " << device_uid << " notification sent");
      break;
    }
  }
  ROS_INFO_STREAM("deviceDisconnectCallback : deviceDisconnectCallback end");
}

DtLogSeverity DtCameraNodeDriver::DtLogSeverityFromString(const std::string &log_level) {
  if (log_level == "debug") {
    return DtLogSeverity::kDtLogSeverityDebug;
  } else if (log_level == "warn") {
    return DtLogSeverity::kDtLogSeverityWarn;
  } else if (log_level == "error") {
    return DtLogSeverity::kDtLogSeverityError;
  } else if (log_level == "fatal") {
    return DtLogSeverity::kDtLogSeverityFatal;
  } else if (log_level == "info") {
    return DtLogSeverity::kDtLogSeverityInfo;
  } else {
    return DtLogSeverity::kDtLogSeverityOff;
  }
}

void DtCameraNodeDriver::QueryDevice() {
  while (is_alive_ && ros::ok() && !device_connected_) {
    ROS_INFO_STREAM("queryDevice: first query device");
    if (!enumerate_net_device_ && !ip_address_.empty() && port_ != 0) {
      ROS_INFO_STREAM("queryDevice: connect to net device " << ip_address_ << ":" << port_);
      ConnectNetDevice(ip_address_, port_);
    } else {
      auto list = ctx_->QueryDeviceList();
      CHECK_NOTNULL(list.get());
      if (list->GetCount() == 0) {
        ROS_WARN_STREAM("No device found, using callback to wait for device");
        return;
      }
      DeviceConnectCallback(list);
      if (hardware_reset_done_) {
        break;
      }
    }
  }
}

void DtCameraNodeDriver::ResetDeviceThread() {
  while (is_alive_ && ros::ok()) {
    std::unique_lock<decltype(reset_device_lock_)> lock(reset_device_lock_);
    reset_device_cv_.wait(lock, [this]() { return !is_alive_ || reset_device_; });
    if (!is_alive_) {
      break;
    }
    ROS_INFO_STREAM("ResetDeviceThread: device is disconnected, reset device start");
    {
      std::lock_guard<decltype(device_lock_)> device_lock(device_lock_);
      dt_camera_node_.reset();
      ROS_INFO_STREAM("ResetDeviceThread: device is disconnected, reset device");
      device_.reset();
      device_info_.reset();
      device_connected_ = false;
      device_uid_.clear();
      reset_device_ = false;
    }
    ROS_INFO_STREAM("ResetDeviceThread: device is disconnected, reset device end");
  }
}

std::string DtCameraNodeDriver::ParseUsbPort(const std::string &line) {
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

bool DtCameraNodeDriver::RebootDeviceServiceCallback(std_srvs::EmptyRequest &req,
                                                     std_srvs::EmptyResponse &res) {
  (void)req;
  (void)res;
  if (!device_connected_) {
    ROS_INFO("Device not connected");
    return false;
  }
  ROS_INFO("Reboot device");
  dt_camera_node_->RebootDevice();
  device_connected_ = false;
  device_ = nullptr;
  return true;
}
// void DtCameraNodeDriver::updatePresetFirmware(std::string path) {
//   if (path.empty()) {
//     return;
//   } else {
//     std::stringstream ss(path);
//     std::string path_segment;
//     std::vector<std::string> paths;
//     DtFwUpdateState updateState = kDtStatStart;
//     bool firstCall = true;

//     while (std::getline(ss, path_segment, ',')) {
//       paths.push_back(path_segment);
//     }
//     uint8_t index = 0;
//     uint8_t count = static_cast<uint8_t>(paths.size());
//     char (*filePaths)[DT_PATH_MAX] = new char[count][DT_PATH_MAX];
//     ROS_INFO_STREAM("paths.cout : " << (uint32_t)count);
//     for (const auto &p : paths) {
//       strcpy(filePaths[index], p.c_str());
//       ROS_INFO_STREAM("path: " << (uint32_t)index << ":" << filePaths[index]);
//       index++;
//     }
//     ROS_INFO_STREAM("Start to update optional depth preset, please wait a moment...");
//     try {
//       device_->updateOptionalDepthPresets(
//           filePaths, count,
//           [this, &updateState, &firstCall](DtFwUpdateState state, const char *message,
//                                            uint8_t percent) {
//             updateState = state;
//             presetUpdateCallback(firstCall, state, message, percent);
//             // firstCall = false;
//           });

//       delete[] filePaths;
//       filePaths = nullptr;
//       if (updateState == kDtStatDone || updateState == kDtStatDoneWithDuplicates) {
//         ROS_INFO_STREAM("After updating the preset: ");
//         auto presetList = device_->getAvailablePresetList();
//         ROS_INFO_STREAM("Preset count: " << presetList->GetCount());
//         for (uint32_t i = 0; i < presetList->GetCount(); ++i) {
//           ROS_INFO_STREAM("  - " << presetList->GetName(i));
//         }
//         ROS_INFO_STREAM("Current preset: " << device_->getCurrentPresetName());
//         std::string key = "PresetVer";
//         if (device_->isExtensionInfoExist(key)) {
//           std::string value = device_->getExtensionInfo(key);
//           ROS_INFO_STREAM("Preset version: " << value);
//         } else {
//           ROS_INFO_STREAM("PresetVer: ");
//         }
//       }
//     } catch (dt::Error &e) {
//       ROS_ERROR_STREAM("Failed to update Preset Firmware " << e.getMessage());
//     } catch (std::exception &e) {
//       ROS_ERROR_STREAM("Failed to update Preset Firmware " << e.what());
//     } catch (...) {
//       ROS_ERROR_STREAM("Failed to update Preset Firmware");
//     }
//   }
// }
// void DtCameraNodeDriver::presetUpdateCallback(bool firstCall, DtFwUpdateState state,
//                                               const char *message, uint8_t percent) {
//   if (!firstCall) {
//     std::cout << "\033[3F";
//   }

//   std::cout << "\033[K";
//   std::cout << "Progress: " << static_cast<uint32_t>(percent) << "%" << std::endl;

//   std::cout << "\033[K";
//   std::cout << "Status  : ";
//   switch (state) {
//     case kDtStatVerifySuccess:
//       std::cout << "Image file verification success" << std::endl;
//       break;
//     case kDtStatFileTransfer:
//       std::cout << "File transfer in progress" << std::endl;
//       break;
//     case kDtStatDone:
//       std::cout << "Update completed" << std::endl;
//       break;
//     case kDtStatDoneWithDuplicates:
//       std::cout << "Update completed, duplicated presets have been ignored" << std::endl;
//       break;
//     case kDtStatInProgress:
//       std::cout << "Update in progress" << std::endl;
//       break;
//     case kDtStatStart:
//       std::cout << "Starting the update" << std::endl;
//       break;
//     case kDtStatVerifyImage:
//       std::cout << "Verifying image file" << std::endl;
//       break;
//     default:
//       std::cout << "Unknown status or error" << std::endl;
//       break;
//   }

//   std::cout << "\033[K";
//   std::cout << "Message : " << message << std::endl << std::flush;
// }
void DtCameraNodeDriver::FirmwareUpdateCallback(DtFwUpdateState state, const char *message,
                                                uint8_t percent) {
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
  if (state == kDtStatDone) {
    ROS_INFO_STREAM("Reboot device");
    dt_camera_node_->RebootDevice();
    device_connected_ = false;
    upgrade_firmware_ = "";
  }
}
}  // namespace deptrum_camera
