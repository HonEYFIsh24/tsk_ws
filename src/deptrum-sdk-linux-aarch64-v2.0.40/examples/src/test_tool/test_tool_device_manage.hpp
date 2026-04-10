#ifndef SRC_TEST_TOOL_DEVICE_MANAGE_H_
#define SRC_TEST_TOOL_DEVICE_MANAGE_H_
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>
#include "deptrum/deptrum.hpp"
#include "test_tool_device.hpp"

namespace dt {
extern std::condition_variable cv_;

class TestToolDeviceManage {
 public:
  TestToolDeviceManage();
  ~TestToolDeviceManage();
  int Init();
  int RefreshDeviceList();
  void PrintSelectDevice();

 private:
  void HandleDeviceConnected(std::shared_ptr<dt::DeviceList> devices);
  void HandleDeviceDisconnected(std::shared_ptr<dt::DeviceList> disconnect_list);
  void HandleDeviceConnectManager();

 public:
  std::atomic<bool> exe_is_stop_{false};
  std::shared_ptr<DeviceList> device_list_;
  std::vector<std::shared_ptr<TestToolDevice>> device_vec_;
  std::shared_ptr<Context> contex_{nullptr};
  static bool reboot_flag_;

  std::thread device_connect_manager_;
  std::condition_variable cv_device_connect_;
  std::mutex mutex_device_connect_;
  std::atomic<bool> device_connect_manager_is_running_{true};
  std::shared_ptr<dt::DeviceList> device_connect_list_{nullptr};
};
}  // namespace dt
#endif