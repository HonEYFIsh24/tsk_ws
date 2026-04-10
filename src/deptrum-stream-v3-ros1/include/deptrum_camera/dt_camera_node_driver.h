// Copyright (c) Deptrum Inc. All Rights Reserved.

#pragma once
#include "dt_camera_node.h"
#include <thread>
#include <mutex>
#include <semaphore.h>
#include <pthread.h>
#define BACKWARD_HAS_DW 1
#include <backward-cpp/backward.hpp>

namespace deptrum_camera {

class DtCameraNodeDriver {
 public:
  explicit DtCameraNodeDriver(ros::NodeHandle& nh, ros::NodeHandle& nh_private);

  ~DtCameraNodeDriver();

 private:
  void Init();

  std::shared_ptr<dt::Device> SelectDevice(const std::shared_ptr<dt::DeviceList>& list);

  bool RebootDeviceServiceCallback(std_srvs::EmptyRequest& req, std_srvs::EmptyResponse& res);

  std::shared_ptr<dt::Device> SelectDeviceBySerialNumber(
      const std::shared_ptr<dt::DeviceList>& list, const std::string& serial_number);
  std::shared_ptr<dt::Device> SelectDeviceByUsbPort(const std::shared_ptr<dt::DeviceList>& list,
                                                    const std::string& usb_port);
  std::shared_ptr<dt::Device> SelectDeviceByNetIp(const std::shared_ptr<dt::DeviceList>& list,
                                                  const std::string& net_ip);

  void InitializeDevice(const std::shared_ptr<dt::Device>& device);

  void DeviceConnectCallback(const std::shared_ptr<dt::DeviceList>& list);

  void ConnectNetDevice(const std::string& ip_address, int port);

  void CheckConnectionTimer();

  void DeviceDisconnectCallback(const std::shared_ptr<dt::DeviceList>& device_list);

  static DtLogSeverity DtLogSeverityFromString(const std::string& log_level);

  void QueryDevice();

  void ResetDeviceThread();

  static std::string ParseUsbPort(const std::string& line);

  void FirmwareUpdateCallback(DtFwUpdateState state, const char* message, uint8_t percent);

  ros::NodeHandle nh_;
  ros::NodeHandle nh_private_;
  std::string config_path_;
  std::shared_ptr<dt::Context> ctx_ = nullptr;
  std::shared_ptr<DtCameraNode> dt_camera_node_ = nullptr;
  std::shared_ptr<dt::Device> device_ = nullptr;
  std::shared_ptr<dt::DeviceInfo> device_info_ = nullptr;
  ros::WallTimer check_connection_timer_;
  std::atomic_bool device_connected_{false};
  std::atomic_bool is_alive_{false};
  std::string serial_number_;
  std::string device_uid_;
  std::string log_level_;
  std::string usb_port_;
  int connection_delay_ = 1.0;
  std::shared_ptr<std::thread> query_thread_ = nullptr;
  std::recursive_mutex device_lock_;
  int device_num_ = 1;
  bool enumerate_net_device_ = false;
  std::shared_ptr<std::thread> reset_device_thread_ = nullptr;
  std::condition_variable reset_device_cv_;
  std::atomic_bool reset_device_{false};
  std::mutex reset_device_lock_;
  pthread_mutex_t* det_device_lock_ = nullptr;
  pthread_mutexattr_t det_device_lock_attr_;
  uint8_t* det_device_lock_shm_addr_ = nullptr;
  int det_device_lock_shm_fd_ = -1;
  // net work config
  std::string ip_address_;
  int port_ = 0;
  ros::ServiceServer reboot_service_srv_;
  static backward::SignalHandling sh;
  bool enable_hardware_reset_ = false;
  bool hardware_reset_done_ = false;
  std::string prefix_paths;
  std::string first_prefix;
  std::size_t colon_pos;
  std::string uvc_backend_;
  std::string upgrade_firmware_;
};
}  // namespace deptrum_camera
