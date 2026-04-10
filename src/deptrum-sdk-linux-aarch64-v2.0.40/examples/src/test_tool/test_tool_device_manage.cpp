#include "test_tool_device_manage.hpp"
#include "test_utils.hpp"
#include "utils.hpp"

namespace dt {
std::condition_variable cv_;
bool TestToolDeviceManage::reboot_flag_ = false;

// Device connection callback
void TestToolDeviceManage::HandleDeviceConnected(std::shared_ptr<dt::DeviceList> devices) {
  // Get the number of connected devices
  if (devices->GetCount() == 0) {
    return;
  }
  const auto device_count = devices->GetCount();
  for (uint32_t i = 0; i < device_count; i++) {
    std::string device_sn = devices->GetSerialNumber(i);
    std::cout << "Found device connected, SN: " << device_sn << std::endl;
  }

  {
    std::unique_lock<std::mutex> lk(mutex_device_connect_);
    device_connect_list_ = devices;
    // std::cout << "notify_all" << std::endl;
    cv_device_connect_.notify_all();
  }
}

// Device disconnect callback
void TestToolDeviceManage::HandleDeviceDisconnected(
    std::shared_ptr<dt::DeviceList> disconnect_list) {
  const auto device_count = disconnect_list->GetCount();
  for (uint32_t i = 0; i < device_count; i++) {
    std::string device_sn = disconnect_list->GetSerialNumber(i);
    std::cout << "Device disconnected, SN: " << device_sn << std::endl;
  }
}

void TestToolDeviceManage::HandleDeviceConnectManager() {
  std::cout << "handleDeviceConnectManager start" << std::endl;
  while (true) {
    std::unique_lock<std::mutex> lk(mutex_device_connect_);
    cv_device_connect_.wait(lk, [this] {
      return device_connect_list_ != nullptr || !device_connect_manager_is_running_;
    });
    if (!device_connect_manager_is_running_) {
      break;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    const auto device_count = device_connect_list_->GetCount();
    for (uint32_t i = 0; i < device_count; i++) {
      std::string device_sn = device_connect_list_->GetSerialNumber(i);
      std::cout << "Found device connected, SN: " << device_sn << std::endl;

      std::string uid = device_connect_list_->GetUid(i);

      for (auto& device_v : device_vec_) {
        if (uid == device_v->GetDeivceUid()) {
          device_v->UnInit();
          std::cout << "UnInit device: " << device_sn << std::endl;
          try {
            device_v->Reset(device_connect_list_->GetDevice(i));
          } catch (const std::exception& e) { std::cerr << e.what() << '\n'; }

          TestToolDeviceManage::reboot_flag_ = true;
          break;
        }
      }
    }
    device_connect_list_ = nullptr;
  }

  std::cout << "handleDeviceConnectManager end" << std::endl;
}

TestToolDeviceManage::TestToolDeviceManage() {
  contex_ = std::make_shared<Context>();
  device_connect_manager_ = std::thread(&TestToolDeviceManage::HandleDeviceConnectManager, this);
}

TestToolDeviceManage::~TestToolDeviceManage() {
  {
    std::unique_lock<std::mutex> lk(mutex_device_connect_);
    device_connect_manager_is_running_ = false;
    cv_device_connect_.notify_all();
  }
  if (device_connect_manager_.joinable()) {
    device_connect_manager_.join();
  }
}

int TestToolDeviceManage::Init() {
#ifdef _WIN32
  SetConsoleOutputCP(CP_UTF8);
  SetConsoleCP(CP_UTF8);
#endif
  contex_->SetDeviceChangedCallback([this](std::shared_ptr<dt::DeviceList> removed_list,
                                           std::shared_ptr<dt::DeviceList> added_list) {
    this->HandleDeviceDisconnected(removed_list);
    this->HandleDeviceConnected(added_list);
  });
  RefreshDeviceList();
  PrintSelectDevice();
  return 0;
}

int TestToolDeviceManage::RefreshDeviceList() {
  try {
    /********************* get device list ****************************/
    SetColorBoldYellowText("-------------------");
    SetColorBoldYellowText("get device list");
    SetColorBoldYellowText("-------------------");
    device_list_ = contex_->QueryDeviceList();
    /********************* check device count *************************/
    int count = device_list_->GetCount();
    if (count == 0) {
      std::cout << "No device found!" << std::endl;
      return -1;
    }
    std::cout << "[GetDeviceCount] Found device count: " << count << std::endl;
    /********************* get device info ****************************/
    for (int index = 0; index < count; index++) {
      std::string device_sn = device_list_->GetSerialNumber(index);
      std::cout << "[GetSerialNumberByIndex] Found device connected, index: " << index
                << " SN: " << device_sn << std::endl;
      std::string connection_type = device_list_->GetConnectionType(index);
      std::cout << "[GetConnectionTypeByIndex] Found device connected, index: " << index
                << " connection_type: " << connection_type << std::endl;
      std::string name = device_list_->GetName(index);
      std::cout << "[GetNameByIndex] Found device connected,index: " << index << " name: " << name
                << std::endl;
      std::string vid = device_list_->GetVid(index);
      std::cout << "[GetVidByIndex] Found device connected, index: " << index << " vid: " << vid
                << std::endl;
      std::string pid = device_list_->GetPid(index);
      std::cout << "[GetPidByIndex] Found device connected, index: " << index << " pid: " << pid
                << std::endl;
      std::string port_path = device_list_->GetUid(index);
      std::cout << "[GetPortPathByIndex] Found device connected, index: " << index
                << " port: " << port_path << std::endl;
      std::cout << std::endl;

      std::shared_ptr<Device> device_ptr = device_list_->GetDevice(index);
      std::shared_ptr<TestToolDevice> test_tool_device_ptr = nullptr;
      test_tool_device_ptr = std::make_shared<TestToolDevice>(device_ptr);
      if (test_tool_device_ptr != nullptr) {
        device_vec_.emplace_back(test_tool_device_ptr);
      }
    }
  } catch (dt::Error& e) {
    std::cerr << "function:" << e.GetFunction() << "\nargs:" << e.GetArgs()
              << "\nmessage:" << e.What() << "\ntype:" << e.GetExceptionType() << std::endl;
    std::cout << "\nPress any key to exit.";
    dt_utils::WaitForKeyPressed();
    exit(EXIT_FAILURE);
  }

  return 0;
}

void TestToolDeviceManage::PrintSelectDevice() {
  while (1) {
    SetColorBoldYellowText("-------------------");
    SetColorBoldYellowText("select device");
    SetColorBoldYellowText("-------------------");
    int count = device_list_->GetCount();
    for (int index = 0; index < count; index++) {
      std::string usb_port = device_list_->GetUid(index);
      SetBlueText("device[" + std::to_string(index) + "]  usb_port: " + usb_port + " ");

      try {
        // Start the multi-device time synchronization function
        contex_->EnableDeviceClockSync(60000);  // update and sync every minitor
      } catch (dt::Error& e) {
        std::cerr << "function:" << e.GetFunction() << "\nargs:" << e.GetArgs()
                  << "\nmessage:" << e.What() << "\ntype:" << e.GetExceptionType() << std::endl;
      }
    }
    SetBlueText("r/R show print info again");
    SetBlueText("q/Q exit the program");
    char device_index;
    std::cout << "enter device index[0, " << count << "): ";
    std::cin >> device_index;
    std::cout << std::endl;
    if ('q' == device_index || 'Q' == device_index) {
      exe_is_stop_ = true;
      break;
    }
    if ('r' == device_index || 'R' == device_index)
      continue;
    if (device_index - 48 >= count || device_index - 48 < 0) {
      std::cout << "what are you doing, entered an unknown num" << std::endl;
      continue;
    }
    device_vec_[device_index - 48]->PrintDeviceOptions();
    std::cout << std::endl;
    std::cout << std::endl;
  }

  device_vec_.clear();
}
}  // namespace dt